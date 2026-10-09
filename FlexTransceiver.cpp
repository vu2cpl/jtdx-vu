#include "FlexTransceiver.hpp"

#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include <QEventLoop>
#include <QDateTime>
#include <QRegularExpression>
#include <QThread>
#include <QDir>
#include <QStandardPaths>
#include <qmath.h>
#include <limits>
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#include <QRandomGenerator>
#endif
#include "commons.h"
#include "JTDXDateTime.h"
#include "flexshared.h"

#include "moc_FlexTransceiver.cpp"

extern "C" {
  void   fil4_(qint16*, qint32*, qint16*, qint32*, float*);
}
extern dec_data dec_data;
extern float gran();

#define RAMP_INCREMENT 64
#if defined (WSJT_SOFT_KEYING)
# define SOFT_KEYING WSJT_SOFT_KEYING
#else
# define SOFT_KEYING 1
#endif

double constexpr FlexTransceiver::m_twoPi;

namespace
{
  char const * const flex_name_prefix {"FlexRadio VITA-49 Slice "};
  int const flex_slices {8};                   // A .. H, as MSHV offers
  quint16 const flex_api_port {4992};
  quint16 const flex_udp_port {4993};           // the radio sends its VITA-49 from here
  quint16 const flex_vita_port {4991};         // the radio's inbound VITA port
  int const flex_rate {24000};                 // DAX, both ways
  int const flex_frames_per_packet {128};      // 5.33 ms
  quint32 const flex_rx_class {0x534C03E3u};
  quint32 const flex_tx_class {0x534C0123u};
  quint32 const flex_meter_class {0x534C8002u};

  inline quint32 be32 (char const * p)
  {
    unsigned char const * u = reinterpret_cast<unsigned char const *> (p);
    return (quint32 (u[0]) << 24) | (quint32 (u[1]) << 16) | (quint32 (u[2]) << 8) | quint32 (u[3]);
  }
  inline float be_float (char const * p)
  {
    union {quint32 i; float f;} u;
    u.i = be32 (p);
    return u.f;
  }
  inline void put_be32 (char * p, quint32 v)
  {
    p[0] = char ((v >> 24) & 0xff); p[1] = char ((v >> 16) & 0xff); p[2] = char ((v >> 8) & 0xff); p[3] = char (v & 0xff);
  }
  // VITA-49 IF data packet: where the payload is
  bool parse_vita (char const * data, int len, int * off, int * bytes)
  {
    if (len < 8) return false;
    quint32 const h = be32 (data);
    int const ptype = (h >> 28) & 0x0f;
    bool const class_id = h & 0x08000000;
    bool const trailer = h & 0x04000000;
    int const tsi = (h >> 22) & 3, tsf = (h >> 20) & 3;
    int const packet_bytes = (h & 0xffff) * 4;
    if (packet_bytes <= 0 || packet_bytes > len) return false;
    if (ptype != 1 && ptype != 3) return false;
    int idx = 8;
    if (class_id) idx += 8;
    if (tsi) idx += 4;
    if (tsf) idx += 8;
    int end = packet_bytes - (trailer ? 4 : 0);
    if (end <= idx) return false;
    *off = idx; *bytes = end - idx;
    return true;
  }
}

void FlexTransceiver::register_transceivers (TransceiverFactory::Transceivers * registry, unsigned first_id)
{
  for (int i = 0; i < flex_slices; ++i)
    (*registry)[flex_name_prefix + QString (QChar ('A' + i))]
      = TransceiverFactory::Capabilities {first_id + i, TransceiverFactory::Capabilities::tci, true};
}

int FlexTransceiver::slices_offered () {return flex_slices;}

FlexTransceiver::FlexTransceiver (int slice, QString const& address, bool use_for_ptt, int poll_interval, QObject * parent)
  : PollingTransceiver {poll_interval, parent}
  , slice_letter_ {slice}
  , use_for_ptt_ {use_for_ptt}
  , do_snr_ {(poll_interval & do__snr) == do__snr}
  , do_pwr_ {(poll_interval & do__pwr) == do__pwr}
  , tci_audio_ {(poll_interval & tci__audio) == tci__audio}
  , socket_ {nullptr}
  , audio_ {nullptr}
  , tx_timer_ {nullptr}
  , wait_timer_ {nullptr}
  , wait_loop_ {nullptr}
  , seq_ {1}
  , handle_ {0}
  , connected_ {false}
  , ready_ {false}
  , slice_ {-1}
  , slice_created_ {false}
  , dax_ch_ {slice + 1}
  , rx_stream_ {0}
  , tx_stream_ {0}
  , tx_dax_ {false}
  , frequency_ {0}
  , last_set_frequency_ {0}
  , mode_ {UNK}
  , PTT_ {false}
  , busy_ {false}
  , m_jtdxtime {nullptr}
  , m_downSampleFactor {4}
  , m_samplesPerFFT {6912 / 2}
  , m_buffer (new short [max_buffer_size * 4])
  , m_bufferPos {0}
  , audio_on_ {false}
  , m_phi {0.0}
  , m_toneSpacing {0.0}
  , m_fSpread {0.0}
  , m_state {Idle}
  , m_tuning {false}
  , m_cwLevel {false}
  , m_j0 {-1}
  , m_toneFrequency0 {1500.0}
  , audioSampleRate {48000u}
  , tx_keyed_ {false}
  , tx_packet_count_ {0}
  , tx_sent_frames_ {0}
  , tx_gain_ {1.0f}
  , debug_file_ {QDir (QStandardPaths::writableLocation (QStandardPaths::DataLocation)).absoluteFilePath ("jtdx_debug.txt").toStdString ()}
{
  auto const a = address.trimmed ();
  int const colon = a.lastIndexOf (':');
  host_ = colon > 0 ? a.left (colon) : a;
  port_ = colon > 0 ? a.mid (colon + 1).toUShort () : flex_api_port;
  if (!port_) port_ = flex_api_port;
  if (host_.isEmpty ()) host_ = "localhost";
}

FlexTransceiver::~FlexTransceiver ()
{
}

// ---------------------------------------------------------------- waits
// Nested event loops on the rig thread, as TCITransceiver: a stop() that
// lands inside one is deferred by TransceiverBase through in_nested_wait().
void FlexTransceiver::wait_ms (int ms)
{
  // A fresh event loop for every wait: a command that arrives while another
  // is waiting (a band change sends frequency, mode and more back to back)
  // nests a second wait, and Qt will not exec() a loop that is already running.
  if (stop_aborting ()) return;
  QEventLoop loop;
  QTimer timer;
  timer.setSingleShot (true);
  connect (&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
  loops_.append (&loop);
  ++nested_waits_;
  timer.start (ms);
  loop.exec ();
  --nested_waits_;
  loops_.removeOne (&loop);
}

void FlexTransceiver::wake ()
{
  for (auto * l : loops_) l->quit ();
}

bool FlexTransceiver::wait_reply (int seq, int ms)
{
  if (seq < 0) return false;
  QElapsedTimer t; t.start ();
  while (!replies_.contains (seq) && t.elapsed () < ms)
    {
      if (stop_aborting () || !connected_) return false;
      wait_ms (qMin (50, ms));
    }
  return replies_.contains (seq);
}

void FlexTransceiver::abort_waits ()
{
  wake ();
}

void FlexTransceiver::trace (QString const& what)
{
  if (!trace_) return;
  if (ftell (trace_) > 16 * 1024 * 1024) return;           // cap
  fprintf (trace_, "%s %8lld %s\n", QDateTime::currentDateTimeUtc ().toString ("hh:mm:ss.zzz").toLatin1 ().constData (),
           trace_clock_.isValid () ? trace_clock_.elapsed () : 0LL, what.toLatin1 ().constData ());
  fflush (trace_);
}

// ---------------------------------------------------------------- session
int FlexTransceiver::send (QString const& cmd)
{
  if (!socket_ || !connected_) return -1;
  if (seq_ > 999999) seq_ = 1;
  int const seq = seq_++;
  trace (QString ("> C%1|%2").arg (seq).arg (cmd));
  socket_->write (QString ("C%1|%2\n").arg (seq).arg (cmd).toLatin1 ());
  socket_->flush ();
  return seq;
}

QString FlexTransceiver::handle_hex () const
{
  return QString ("client_handle=0x%1").arg (handle_, 8, 16, QChar ('0')).toLower ();
}

// The radio sends its VITA-49 (DAX audio, meters) to us unasked, which a
// stateful firewall or NAT between us drops: seen over a WireGuard VPN into
// VU2OY's network, where TCP control worked but no UDP arrived. A packet from
// our UDP socket to the radio's 4993 opens the return path - SmartSDR's
// SmartLink "udp_register". Sent at connect and every 5 s to keep it open;
// on a plain LAN the radio just ignores it.
void FlexTransceiver::udp_register ()
{
  if (!audio_ || !socket_ || !handle_) return;
  QHostAddress const radio = socket_->peerAddress ();
  if (radio.isNull ()) return;
  audio_->writeDatagram (QString ("client udp_register handle=0x%1").arg (handle_, 8, 16, QChar ('0')).toLatin1 (),
                         radio, flex_udp_port);
  last_udp_register_ms_ = QDateTime::currentMSecsSinceEpoch ();
}

void FlexTransceiver::on_connected ()
{
  connected_ = true;
}

void FlexTransceiver::on_disconnected ()
{
  connected_ = false;
  if (ready_ && error_.isEmpty ()) error_ = tr ("Flex connection lost");
  wake ();
}

void FlexTransceiver::on_error ()
{
  if (socket_) error_ = tr ("Flex: %1").arg (socket_->errorString ());
  wake ();
}

void FlexTransceiver::on_ready_read ()
{
  rx_buf_ += socket_->readAll ();
  int nl;
  while ((nl = rx_buf_.indexOf ('\n')) >= 0)
    {
      handle_line (QString::fromLatin1 (rx_buf_.left (nl)).trimmed ());
      rx_buf_.remove (0, nl + 1);
    }
}

// H<handle>, R<seq>|<code>|<body>, S<handle>|<status>, V<version>, M<msg>
void FlexTransceiver::handle_line (QString const& line)
{
  if (line.isEmpty ()) return;
  trace ("< " + line.left (300));
  QChar const tag = line.at (0);
  if (tag == 'H')
    {
      bool ok = false;
      quint32 const h = line.mid (1).toUInt (&ok, 16);
      if (ok) handle_ = h;
      wake ();
      return;
    }
  if (tag == 'R')
    {
      QRegularExpression const mo {"model=\"([^\"]+)\""};
      auto const mm = mo.match (line);
      if (mm.hasMatch ())
        {
          model_ = mm.captured (1);
          // "M" on the end of the model is the only sign of a front speaker
          FlexShared::instance ().update ([this] (FlexStatus & st) {
              st.model = model_;
              st.spkr_supported = model_.trimmed ().toUpper ().endsWith ("M");
            });
        }
      QRegularExpression const ap {"\\batu_present=([01])\\b"};
      auto const am = ap.match (line);
      if (am.hasMatch ()) FlexShared::instance ().update ([am] (FlexStatus & st) {st.atu_present = am.captured (1).toInt ();});
      int const bar = line.indexOf ('|');
      if (bar < 0) return;
      bool ok = false;
      int const seq = line.mid (1, bar - 1).toInt (&ok);
      if (!ok) return;
      QString const rest = line.mid (bar + 1);
      int const bar2 = rest.indexOf ('|');
      quint32 const code = rest.left (bar2 < 0 ? rest.size () : bar2).toUInt (nullptr, 16);
      replies_[seq] = qMakePair (code, bar2 < 0 ? QString () : rest.mid (bar2 + 1));
      if (on_reply_.contains (seq)) on_reply_.take (seq) (code);
      if (replies_.size () > 200) replies_.remove (replies_.firstKey ());
      wake ();
      return;
    }
  if (tag != 'S') return;
  parse_panel_status (line);
  status_log_.append (line);
  if (status_log_.size () > 400) status_log_.removeFirst ();
  // our slice: frequency, mode, tx
  if (slice_ >= 0 && line.contains (QString ("|slice %1 ").arg (slice_))) parse_slice_line (slice_, line);
  // stream ids may come through the status before the reply
  QRegularExpression const st {"\\|stream 0x([0-9A-Fa-f]+)"};
  if (line.contains ("type=dax_rx") && line.toLower ().contains (handle_hex ()))
    {
      auto const m = st.match (line);
      if (m.hasMatch () && !rx_stream_) rx_stream_ = m.captured (1).toUInt (nullptr, 16);
    }
  if (line.contains ("type=dax_tx") && line.toLower ().contains (handle_hex ()))
    {
      auto const m = st.match (line);
      if (m.hasMatch () && !tx_stream_) tx_stream_ = m.captured (1).toUInt (nullptr, 16);
    }
}

void FlexTransceiver::parse_slice_line (int, QString const& line)
{
  QRegularExpression const kv {"\\b(RF_frequency|mode|tx)=([^\\s]+)"};
  auto it = kv.globalMatch (line);
  while (it.hasNext ())
    {
      auto const m = it.next ();
      QString const k = m.captured (1), v = m.captured (2);
      if (k == "RF_frequency")
        {
          Frequency const nf = Frequency (std::llround (v.toDouble () * 1e6));
          if (nf != frequency_) trace (QString ("radio says slice frequency %1").arg (nf));
          frequency_ = nf;
        }
      else if (k == "mode")
        {
          if (v.toUpper () != mode_str_) trace ("radio says slice mode " + v.toUpper ());
          mode_str_ = v.toUpper (); mode_ = unmap_mode (mode_str_);
        }
    }
  last_slice_line_ = line;
}

bool FlexTransceiver::slice_exists (int n) const
{
  QString const want = QString ("|slice %1 ").arg (n);
  bool exists = false;
  for (auto const& l : status_log_)
    {
      if (!l.contains (want)) continue;
      auto const low = l.toLower ();
      if (low.contains ("in_use=1")) exists = true;
      else if (low.contains ("in_use=0")) exists = false;
    }
  return exists;
}

int FlexTransceiver::owned_slice () const
{
  QString const want = handle_hex ();
  int found = -1;
  QRegularExpression const rx {"\\|slice (\\d+)"};
  for (auto const& l : status_log_)
    {
      auto const low = l.toLower ();
      if (!low.contains ("|slice ") || !low.contains ("in_use=1") || !low.contains (want)) continue;
      auto const m = rx.match (l);
      if (m.hasMatch ()) found = m.captured (1).toInt ();
    }
  return found;
}

// index_letter is numbered PER CLIENT (MSHV-Mac, measured 2026-09-10 against a
// second GUI client), so the letter only means something together with our handle
int FlexTransceiver::owned_slice_by_letter (int letter) const
{
  if (letter < 0) return -1;
  QString const want = handle_hex ();
  QRegularExpression const isletter {QString ("index_letter=%1(\\s|$)").arg (QChar ('a' + letter))};
  QRegularExpression const rx {"\\|slice (\\d+)"};
  int found = -1;
  for (auto const& l : status_log_)
    {
      auto const low = l.toLower ();
      if (!low.contains ("|slice ") || !low.contains ("in_use=1") || !low.contains (want)) continue;
      if (!isletter.match (low).hasMatch ()) continue;
      auto const m = rx.match (l);
      if (m.hasMatch ()) found = m.captured (1).toInt ();
    }
  return found;
}

QString FlexTransceiver::map_mode (MODE m) const
{
  switch (m)
    {
    case USB: return "USB";
    case LSB: return "LSB";
    case DIG_U: return "DIGU";
    case DIG_L: return "DIGL";
    case CW: return "CW";
    case AM: return "AM";
    case FM: return "FM";
    case DIG_FM: return "DFM";
    default: break;
    }
  return QString ();
}

Transceiver::MODE FlexTransceiver::unmap_mode (QString const& s) const
{
  if (s == "USB") return USB;
  if (s == "LSB") return LSB;
  if (s == "DIGU") return DIG_U;
  if (s == "DIGL") return DIG_L;
  if (s == "CW") return CW;
  if (s == "AM" || s == "SAM") return AM;
  if (s == "FM" || s == "NFM") return FM;
  if (s == "DFM") return DIG_FM;
  return UNK;
}

// The start-up, measured against a FLEX-6600 (MSHV-Mac): the radio volunteers
// V and H on connect; "client gui" is what makes it show and grant slices;
// "client udpport" must name a socket already bound; "dax audio set" routes a
// slice to our channel; the stream ids arrive in the reply body or the status.
bool FlexTransceiver::start_session ()
{
  socket_->connectToHost (host_, port_);
  QElapsedTimer t; t.start ();
  while (!connected_ && error_.isEmpty () && t.elapsed () < 4000) wait_ms (50);
  if (!connected_) {if (error_.isEmpty ()) error_ = tr ("Flex: no connection to %1:%2").arg (host_).arg (port_); return false;}
  t.restart ();
  while (!handle_ && connected_ && t.elapsed () < 3000) wait_ms (50);
  if (!handle_) {error_ = tr ("Flex: the radio sent no client handle"); return false;}

  int seq = send ("client gui");
  if (!wait_reply (seq, 3000)) {error_ = tr ("Flex: no reply to client gui"); return false;}
  if (replies_[seq].first != 0) {error_ = tr ("Flex: client gui refused (0x%1)").arg (replies_[seq].first, 0, 16); return false;}
  send ("sub slice all");
  send ("sub tx all");
  send ("sub radio all");
  send ("sub meter all");          // the panel's meters and the main window's S / PWR / SWR
  send ("sub atu all");            // read-only; a radio without a tuner never reports
  if (!audio_)
    {
      audio_ = new QUdpSocket {this};
      audio_->bind (QHostAddress (QHostAddress::AnyIPv4), 0);
      connect (audio_, &QUdpSocket::readyRead, this, &FlexTransceiver::on_audio);
    }
  seq = send ("client udpport " + QString::number (audio_->localPort ()));
  if (!wait_reply (seq, 3000) || replies_[seq].first != 0) {error_ = tr ("Flex: client udpport refused"); return false;}
  udp_register ();
  seq = send ("info");
  wait_reply (seq, 2000);
  wait_ms (1200);                                 // let the slice status arrive

  // one slice carries both halves: tune, key, read and listen on the same one
  int pick = owned_slice_by_letter (slice_letter_);
  if (pick < 0) pick = owned_slice ();
  if (pick < 0 && slice_exists (slice_letter_)) pick = slice_letter_;
  if (pick >= 0)
    {
      slice_ = pick;
      slice_created_ = false;
    }
  else
    {
      QString create = "slice create mode=digu";
      if (last_set_frequency_ > 0) create += QString (" freq=%1").arg (double (last_set_frequency_) / 1e6, 0, 'f', 6);
      send (create);
      t.restart ();
      while (owned_slice () < 0 && connected_ && t.elapsed () < 4000) wait_ms (100);
      slice_ = owned_slice ();
      if (slice_ < 0) {error_ = tr ("Flex: slice create not answered"); return false;}
      slice_created_ = true;
    }
  // the slice's full status came with "client gui", before we knew which slice
  // was ours: read it now (frequency, mode, and the panel's antenna / mode lists)
  for (auto const& l : status_log_)
    if (l.contains (QString ("|slice %1 ").arg (slice_))) {parse_slice_line (slice_, l); parse_panel_status (l);}

  if (tci_audio_)
    {
      seq = send (QString ("dax audio set %1 slice=%2").arg (dax_ch_).arg (slice_));
      wait_reply (seq, 2000);
      rx_stream_ = 0;
      seq = send (QString ("stream create type=dax_rx dax_channel=%1").arg (dax_ch_));
      if (!wait_reply (seq, 3000) || replies_[seq].first != 0) {error_ = tr ("Flex: DAX RX stream refused"); return false;}
      if (!replies_[seq].second.trimmed ().isEmpty ())
        {
          bool ok = false;
          quint32 const id = replies_[seq].second.trimmed ().toUInt (&ok, 16);
          if (ok && id) rx_stream_ = id;
        }
      t.restart ();
      while (!rx_stream_ && connected_ && t.elapsed () < 3000) wait_ms (50);
      if (!rx_stream_) {error_ = tr ("Flex: DAX RX stream not granted"); return false;}
      tx_stream_ = 0;
      seq = send ("stream create type=dax_tx");
      if (wait_reply (seq, 3000) && replies_[seq].first == 0)
        {
          if (!replies_[seq].second.trimmed ().isEmpty ())
            {
              bool ok = false;
              quint32 const id = replies_[seq].second.trimmed ().toUInt (&ok, 16);
              if (ok && id) tx_stream_ = id;
            }
          t.restart ();
          while (!tx_stream_ && connected_ && t.elapsed () < 3000) wait_ms (50);
        }
      if (tx_stream_)
        {
          send ("transmit set dax=1");
          send (QString ("dax audio set %1 slice=%2 tx=1").arg (dax_ch_).arg (slice_));
          tx_dax_ = true;
        }
    }
  return true;
}

void FlexTransceiver::release (bool tell_radio)
{
  tx_audio (false);
  if (tell_radio && connected_)
    {
      if (PTT_) send ("xmit 0");
      if (tx_stream_) send (QString ("stream remove 0x%1").arg (tx_stream_, 8, 16, QChar ('0')));
      if (rx_stream_) send (QString ("stream remove 0x%1").arg (rx_stream_, 8, 16, QChar ('0')));
      if (tx_dax_) send ("transmit set dax=0");
      if (slice_ >= 0 && slice_created_) send (QString ("slice remove %1").arg (slice_));
      if (socket_) socket_->flush ();
    }
  rx_stream_ = tx_stream_ = 0;
  tx_dax_ = false;
  slice_ = -1;
  slice_created_ = false;
  PTT_ = false;
}

// ---------------------------------------------------------------- Transceiver
int FlexTransceiver::do_start (JTDXDateTime * jtdxtime)
{
  if (tci_audio_) QThread::currentThread ()->setPriority (QThread::HighPriority);
  m_jtdxtime = jtdxtime;
  if (trace_) {fclose (trace_); trace_ = nullptr;}
  {
    auto const dir = QStandardPaths::writableLocation (QStandardPaths::DataLocation);
    QDir {}.mkpath (dir);
    trace_ = fopen (QDir {dir}.absoluteFilePath ("flex_trace.txt").toLocal8Bit ().constData (), "a");
    trace_clock_.start ();
    trace (QString ("======== start: %1:%2 slice letter %3 audio %4 (asked for %5)").arg (host_).arg (port_).arg (slice_letter_).arg (tci_audio_).arg (last_set_frequency_));
  }
  error_.clear ();
  ready_ = false;
  handle_ = 0;
  replies_.clear ();
  status_log_.clear ();
  rx_buf_.clear ();
  frequency_ = 0;
  mode_ = UNK;
  PTT_ = false;
  busy_ = false;
  last_rx_audio_ms_ = 0;
  m_bufferPos = 0;
  if (!tx_timer_)
    {
      tx_timer_ = new QTimer {this};
      tx_timer_->setTimerType (Qt::PreciseTimer);
      connect (tx_timer_, &QTimer::timeout, this, &FlexTransceiver::on_tx_tick);
    }
  if (!socket_)
    {
      socket_ = new QTcpSocket {this};
      connect (socket_, &QTcpSocket::connected, this, &FlexTransceiver::on_connected);
      connect (socket_, &QTcpSocket::disconnected, this, &FlexTransceiver::on_disconnected);
      connect (socket_, &QTcpSocket::readyRead, this, &FlexTransceiver::on_ready_read);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
      connect (socket_, &QAbstractSocket::errorOccurred, this, &FlexTransceiver::on_error);
#else
      connect (socket_, QOverload<QAbstractSocket::SocketError>::of (&QAbstractSocket::error), this, &FlexTransceiver::on_error);
#endif
    }
  if (!start_session ())
    {
      trace ("start failed: " + error_);
      release (true);
      if (socket_) socket_->abort ();
      connected_ = false;
      throw error {error_.isEmpty () ? tr ("Flex: could not be opened") : error_};
    }
  ready_ = true;
  last_rx_audio_ms_ = QDateTime::currentMSecsSinceEpoch ();
  if (!cmd_timer_)
    {
      cmd_timer_ = new QTimer {this};
      connect (cmd_timer_, &QTimer::timeout, this, &FlexTransceiver::on_panel_commands);
    }
  cmd_timer_->start (100);
  publish_status ();
  // what JTDX-VU asked for while the session was coming up (the startup
  // frequency, a band change during a reconnect) - a fresh GUI client's slice
  // sits at the radio's default (14.100 USB), and JTDX-VU would otherwise
  // follow it there
  if (pending_frequency_ || last_set_frequency_)
    {
      Frequency const f = pending_frequency_ ? pending_frequency_ : last_set_frequency_;
      trace (QString ("applying after start: %1 mode %2").arg (f).arg (pending_mode_));
      do_frequency (f, pending_mode_, true);
    }
  else if (pending_mode_ != UNK) do_mode (pending_mode_);
  do_poll ();
  return 0;
}

void FlexTransceiver::do_stop ()
{
  trace ("do_stop");
  ready_ = false;
  if (cmd_timer_) {cmd_timer_->stop (); delete cmd_timer_; cmd_timer_ = nullptr;}
  FlexShared::instance ().reset ();
  on_reply_.clear ();
  release (true);
  if (socket_)
    {
      if (connected_) {socket_->disconnectFromHost (); if (socket_->state () != QAbstractSocket::UnconnectedState) socket_->waitForDisconnected (300);}
      socket_->abort ();
      delete socket_; socket_ = nullptr;
    }
  connected_ = false;
  if (audio_) {delete audio_; audio_ = nullptr;}
  if (tx_timer_) {tx_timer_->stop (); delete tx_timer_; tx_timer_ = nullptr;}
  wake ();
  m_state = Idle;
}

// A tune or mode change counts as done when the radio answers it OK - as MSHV
// does.  Waiting to see it in the slice status was the 2026-10-07 on-air
// failure: on the FLEX-6600 the band changed (R|0, the TX interlock moved to
// 15m) but no slice line with the new RF_frequency reached us within 2 s, the
// "failure" took the rig offline, and every reconnect got a fresh slice back at
// 14.100 USB.  The status stream still updates frequency_ / mode_ whenever the
// radio does report them (a retune in SmartSDR is followed).
void FlexTransceiver::do_frequency (Frequency f, MODE m, bool /*no_ignore*/)
{
  trace (QString ("do_frequency %1 mode %2 (now %3, busy %4, depth %5)").arg (f).arg (m).arg (frequency_).arg (busy_).arg (nested_waits_));
  last_set_frequency_ = f;
  if (m != UNK) pending_mode_ = m;
  if (!ready_ || slice_ < 0 || busy_)
    {
      // connecting, or another command is in progress: remember it, do it after
      pending_frequency_ = f;
      update_rx_frequency (f);
      if (m != UNK) update_mode (m);
      return;
    }
  busy_ = true;
  pending_frequency_ = f;
  while (pending_frequency_ && ready_ && connected_)
    {
      Frequency const want = pending_frequency_;
      pending_frequency_ = 0;
      if (frequency_ != want)
        {
          int const seq = send (QString ("slice tune %1 %2 autopan=1").arg (slice_).arg (double (want) / 1e6, 0, 'f', 6));
          if (wait_reply (seq, 1500) && replies_[seq].first == 0) frequency_ = want;
          else trace (QString ("!! tune to %1 not confirmed (code 0x%2)").arg (want).arg (replies_.value (seq).first, 0, 16));
        }
      update_rx_frequency (frequency_ ? frequency_ : want);
      if (pending_mode_ != UNK) apply_mode (pending_mode_);
    }
  busy_ = false;
}

bool FlexTransceiver::apply_mode (MODE m)
{
  auto const want = map_mode (m);
  if (want.isEmpty ()) return false;
  bool ok = true;
  if (mode_str_ != want)
    {
      int const seq = send (QString ("slice set %1 mode=%2").arg (slice_).arg (want));
      ok = wait_reply (seq, 1500) && replies_[seq].first == 0;
      if (ok)
        {
          mode_str_ = want; mode_ = m;
          FlexShared::instance ().update ([want] (FlexStatus & st) {st.mode = want;});
        }
      else trace (QString ("!! mode %1 not confirmed (code 0x%2)").arg (want).arg (replies_.value (seq).first, 0, 16));
    }
  update_mode (mode_ != UNK ? mode_ : m);
  return ok;
}

// A Flex has no second VFO: split is Fake It on the JTDX-VU side only
void FlexTransceiver::do_tx_frequency (Frequency tx, MODE, bool)
{
  update_split (false);
  update_other_frequency (0);
  Q_UNUSED (tx);
}

void FlexTransceiver::do_mode (MODE m)
{
  trace (QString ("do_mode %1 (now %2, busy %3, depth %4)").arg (m).arg (mode_str_).arg (busy_).arg (nested_waits_));
  if (map_mode (m).isEmpty ()) return;
  pending_mode_ = m;
  if (!ready_ || slice_ < 0 || busy_) {update_mode (m); return;}   // applied after
  busy_ = true;
  apply_mode (m);
  busy_ = false;
}

void FlexTransceiver::do_ptt (bool on)
{
  trace (QString ("do_ptt %1 (now %2, depth %3)").arg (on).arg (PTT_).arg (nested_waits_));
  if (!use_for_ptt_) throw error {tr ("Flex: PTT must be via CAT")};
  if (!ready_ || slice_ < 0) {update_PTT (on); return;}
  if (on == PTT_) {update_PTT (on); return;}
  if (on)
    {
      // DAX audio just before the key, so the radio has a little buffered
      // when xmit 1 lands (MSHV-Mac, measured)
      if (tx_dax_) tx_audio (true);
      if (tx_dax_) send (QString ("dax audio set %1 tx=1").arg (dax_ch_));
      send (QString ("slice set %1 tx=1").arg (slice_));
      int const seq = send ("xmit 1");
      if (!wait_reply (seq, 1000) || replies_[seq].first != 0) {error_ = tr ("Flex: failed to key (xmit)"); tx_audio (false); return;}
      PTT_ = true;
    }
  else
    {
      int const seq = send ("xmit 0");
      wait_reply (seq, 1000);
      // stop the DAX audio on unkey: a stream still flowing holds the radio in
      // UNKEY_REQUESTED at full power (MSHV-Mac, 2026-09-08)
      tx_audio (false);
      PTT_ = false;
    }
  update_PTT (PTT_);
}

void FlexTransceiver::do_poll ()
{
  if (!connected_ && error_.isEmpty ()) error_ = tr ("Flex connection lost");
  if (ready_ && connected_ && QDateTime::currentMSecsSinceEpoch () - last_udp_register_ms_ > 5000) udp_register ();
  // audio watchdog: connected with DAX on but nothing heard for 10 s
  if (tci_audio_ && ready_ && connected_ && error_.isEmpty () && rx_stream_)
    {
      qint64 const now = QDateTime::currentMSecsSinceEpoch ();
      if (PTT_ || !last_rx_audio_ms_) last_rx_audio_ms_ = now;
      else if (now - last_rx_audio_ms_ > 10000) error_ = tr ("no audio from the radio - check DAX channel %1").arg (dax_ch_);
    }
  if (!error_.isEmpty ()) {trace ("poll -> error: " + error_); ready_ = false; throw error {error_};}
  if (!ready_) throw error {tr ("Flex: not started")};
  if (frequency_) update_rx_frequency (frequency_);
  update_split (false);
  update_mode (mode_);
  update_PTT (PTT_);
  // JTDX-VU's own S-meter / TX power / SWR labels (Settings > Radio ticks),
  // on TCI's scale: level = dBm + 73 (S9 = 0), power in mW, SWR x 100
  if (do_snr_ && !PTT_ && level_dbm_ > -199.) update_level (int (std::lround (level_dbm_)) + 73);
  if (do_pwr_ && PTT_) {update_power (unsigned (std::lround (fwd_w_ * 1000.))); update_swr (unsigned (std::lround (swr_ * 100.)));}
  if (!m_tuning && radio_rfpower_ >= 0) update_drive (radio_rfpower_);   // the TX slider follows the radio
  publish_status ();
}

void FlexTransceiver::do_audio (bool on)
{
  if (on) {dec_data.params.kin = 0; m_bufferPos = 0;}
  audio_on_ = on;
}

void FlexTransceiver::do_period (double period) {m_period = period;}
void FlexTransceiver::do_blocksize (qint32 blocksize) {m_samplesPerFFT = blocksize;}

// The TX slider: with DAX the radio's RF power is its own business; the
// slider attenuates the audio we send, 0 dB at the top
// The TX slider is the radio's RF power, as it is the SDR program's drive with
// TCI (same scale: drive = 100 - volume x 2.2222, slider = 4.5 x drive): the
// slider follows the radio (update_drive in do_poll; a Flex keeps RF power per
// band), and only a slider move sends a new value - nothing at start-up, and
// nothing before the radio has said what it is set to.  While tuning it is the
// radio's tune power instead.  The DAX audio itself always goes at full scale.
void FlexTransceiver::do_txvolume (qreal volume)
{
  tx_gain_ = 1.0f;
  int const drive = qBound (0, int (std::lround (100. - volume * 2.2222222)), 100);
  int & radio = m_tuning ? radio_tunepower_ : radio_rfpower_;
  char const * key = m_tuning ? "tunepower" : "rfpower";
  if (!ready_ || radio < 0 || drive == radio) return;
  trace (QString ("TX slider -> transmit set %1=%2 (was %3)").arg (key).arg (drive).arg (radio));
  send (QString ("transmit set %1=%2").arg (key).arg (drive));
  radio = drive;
  FlexShared::instance ().update ([this] (FlexStatus & st) {st.rfpower = radio_rfpower_; st.tunepower = radio_tunepower_;});
}

void FlexTransceiver::do_tune (bool newState)
{
  m_tuning = newState;
  if (!m_tuning) do_modulator_stop (true);
}

// ---------------------------------------------------------------- Flex panel
// What the panel shows, from the status stream; port of MSHV-Mac's VitaLine()
// (network.cpp) with the same parsing rules.
void FlexTransceiver::parse_panel_status (QString const& line)
{
  auto & sh = FlexShared::instance ();
  if (slice_ >= 0 && line.contains (QString ("|slice %1 ").arg (slice_)))
    {
      QRegularExpression const kv {"\\b(rxant|txant|mode|ant_list|tx_ant_list|mode_list)=([^\\s]+)"};
      auto it = kv.globalMatch (line);
      sh.update ([&it] (FlexStatus & st) {
          while (it.hasNext ())
            {
              auto const m = it.next ();
              QString const k = m.captured (1), v = m.captured (2);
              if (k == "rxant") st.rxant = v;
              else if (k == "txant") st.txant = v;
              else if (k == "mode") st.mode = v.toUpper ();
              else if (k == "ant_list") st.ant_list = v.split (',', Qt::SkipEmptyParts);
              else if (k == "tx_ant_list") st.tx_ant_list = v.split (',', Qt::SkipEmptyParts);
              else if (k == "mode_list") st.mode_list = v.split (',', Qt::SkipEmptyParts);
            }
        });
    }
  // radio-global transmit settings - NOT slice properties
  if (line.contains ("|transmit "))
    {
      QRegularExpression const kv {"\\b(rfpower|tunepower|max_power_level|hwalc_enabled)=(\\d+)"};
      {
        auto i2 = kv.globalMatch (line);
        while (i2.hasNext ())
          {
            auto const m = i2.next ();
            if (m.captured (1) == "rfpower") radio_rfpower_ = m.captured (2).toInt ();
            else if (m.captured (1) == "tunepower") radio_tunepower_ = m.captured (2).toInt ();
          }
      }
      auto it = kv.globalMatch (line);
      sh.update ([&it] (FlexStatus & st) {
          while (it.hasNext ())
            {
              auto const m = it.next ();
              int const v = m.captured (2).toInt ();
              if (m.captured (1) == "rfpower") st.rfpower = v;
              else if (m.captured (1) == "tunepower") st.tunepower = v;
              else if (m.captured (1) == "max_power_level") st.maxpower = v;
              else st.hwalc = v != 0;
            }
        });
    }
  // "atu status=TUNE_MANUAL_BYPASS atu_enabled=1 memories_enabled=0 using_mem=0"
  if (line.contains ("|atu "))
    {
      QRegularExpression const kv {"\\b(status|atu_enabled|memories_enabled|using_mem)=([^\\s]+)"};
      auto it = kv.globalMatch (line);
      sh.update ([&it] (FlexStatus & st) {
          while (it.hasNext ())
            {
              auto const m = it.next ();
              QString const k = m.captured (1), v = m.captured (2);
              if (k == "status")
                {
                  // the radio repeats a status; only a CHANGE says where it came from
                  if (v != st.atu_status) {st.atu_after_cycle = st.atu_status == "TUNE_IN_PROGRESS"; st.atu_status = v;}
                  st.atu_refused.clear ();
                }
              else if (k == "atu_enabled") st.atu_enabled = v == "1";
              else if (k == "memories_enabled") st.atu_memories = v == "1";
              else if (k == "using_mem") st.atu_using_mem = v == "1";
            }
        });
    }
  if (line.contains ("|radio slices="))
    {
      QRegularExpression const fs {"\\bfront_speaker_mute=([01])\\b"};
      auto const m = fs.match (line);
      if (m.hasMatch ()) sh.update ([m] (FlexStatus & st) {st.spkr_mute = m.captured (1) == "1"; st.spkr_supported = true;});
    }
  // meter definitions: "meter 7.src=TX-#7.num=1#7.nam=FWDPWR#7.unit=dBm#..."
  if (line.contains ("|meter "))
    {
      QRegularExpression const f {"(\\d+)\\.(src|num|nam|unit)=([^#\\s]+)"};
      auto it = f.globalMatch (line);
      while (it.hasNext ())
        {
          auto const m = it.next ();
          int const id = m.captured (1).toInt ();
          auto & d = meter_defs_[id];
          QString const k = m.captured (2), v = m.captured (3);
          if (k == "src") d.src = v;
          else if (k == "num") d.num = v.toInt ();
          else if (k == "unit") d.unit = v;
          else
            {
              d.nam = v.toUpper ();
              if (d.nam == "FWDPWR" && meter_fwd_ < 0) meter_fwd_ = id;
              else if (d.nam == "REFPWR" && meter_ref_ < 0) meter_ref_ = id;
              else if (d.nam == "SWR" && meter_swr_ < 0) meter_swr_ = id;
            }
        }
    }
}

// Meter packets (class 0x534C8002): pairs of (id, value) int16 big-endian.
// The scale depends on the unit, verified on the 6600 in MSHV-Mac: dBm / dBFS /
// SWR /128, Volts / Amps /256, degC /64, RPM /1.
void FlexTransceiver::parse_meters (char const * buf, int len)
{
  int off = 0, bytes = 0;
  if (!parse_vita (buf, len, &off, &bytes)) return;
  QHash<QString, double> named;
  bool power = false;
  double fwd = fwd_w_, ref = 0., swr = swr_;
  for (int i = off; i + 4 <= off + bytes; i += 4)
    {
      int const id = (quint8 (buf[i]) << 8) | quint8 (buf[i + 1]);
      qint16 const raw = qint16 ((quint8 (buf[i + 2]) << 8) | quint8 (buf[i + 3]));
      auto const d = meter_defs_.value (id);
      double div = 128.;
      if (d.unit == "Volts" || d.unit == "Amps") div = 256.;
      else if (d.unit == "degC") div = 64.;
      else if (d.unit == "RPM") div = 1.;
      double const v = raw / div;
      if (id == meter_fwd_) {fwd = v > 0. ? std::pow (10., (v - 30.) / 10.) : 0.; power = true;}
      else if (id == meter_ref_) ref = v > 0. ? std::pow (10., (v - 30.) / 10.) : 0.;
      else if (id == meter_swr_) swr = v;
      // the S-meter: this slice's LEVEL meter, dBm
      if (d.nam == "LEVEL" && d.src.startsWith ("SLC") && d.num == slice_) level_dbm_ = v;
      if (!d.nam.isEmpty ()) named[d.nam] = v;
    }
  fwd_w_ = fwd; swr_ = swr;
  FlexShared::instance ().update ([&] (FlexStatus & st) {
      if (power) {st.meters_ok = true; st.fwd_w = fwd; st.ref_w = ref; st.swr = swr;}
      for (auto it = named.cbegin (); it != named.cend (); ++it) st.meter[it.key ()] = it.value ();
    });
}

void FlexTransceiver::publish_status ()
{
  QString const line = QString ("Slice %1, DAX channel %2%3").arg (QChar ('A' + slice_letter_)).arg (dax_ch_)
    .arg (tx_dax_ ? tr (", transmit") : tr (", receive only"));
  FlexShared::instance ().update ([&] (FlexStatus & st) {
      st.up = ready_ && connected_;
      st.status = line;
      st.slice = slice_;
      st.dax_channel = dax_ch_;
      st.tx = tx_dax_;
      st.frequency = qint64 (frequency_);
      if (!model_.isEmpty ()) st.model = model_;
    });
}

// Run what the panel asked for.  The radio reports most changes back on the
// status stream, but the 6600 was seen not to echo every slice change, so a
// change it answers OK is also shown at once.
void FlexTransceiver::on_panel_commands ()
{
  if (!ready_ || !connected_ || slice_ < 0) return;
  for (QString c : FlexShared::instance ().take ())
    {
      c.replace ("{slice}", QString::number (slice_));
      int const seq = send (c);
      if (seq < 0) continue;
      QRegularExpression const sk {"^slice s \\d+ (rxant|txant|mode)=(\\S+)$"};
      auto const m = sk.match (c);
      if (m.hasMatch ())
        {
          QString const k = m.captured (1), v = m.captured (2);
          on_reply_[seq] = [this, k, v] (quint32 code) {
            if (code) {trace (QString ("!! panel %1=%2 refused 0x%3").arg (k).arg (v).arg (code, 0, 16)); return;}
            if (k == "mode") {mode_str_ = v.toUpper (); mode_ = unmap_mode (mode_str_);}
            FlexShared::instance ().update ([k, v] (FlexStatus & st) {
                if (k == "rxant") st.rxant = v; else if (k == "txant") st.txant = v; else st.mode = v.toUpper ();
              });
          };
        }
      else if (c.startsWith ("mixer front_speaker mute "))
        {
          bool const on = c.endsWith (" on");
          // a radio without a front panel answers 0x500000B7: grey the control
          on_reply_[seq] = [on] (quint32 code) {
            FlexShared::instance ().update ([on, code] (FlexStatus & st) {st.spkr_supported = code == 0; if (!code) st.spkr_mute = on;});
          };
        }
      else if (c.startsWith ("atu "))
        {
          FlexShared::instance ().update ([] (FlexStatus & st) {st.atu_refused.clear ();});
          on_reply_[seq] = [] (quint32 code) {
            if (code) FlexShared::instance ().update ([code] (FlexStatus & st) {st.atu_refused = QString::number (code, 16).toUpper ();});
          };
        }
    }
}

// ---------------------------------------------------------------- RX audio
// DAX RX: VITA-49 IF packets, class 0x534C03E3, stereo float32 big-endian
// at 24 kHz with L == R.  The decoder path below (writeAudioData, as
// TCITransceiver) wants 48 kHz stereo frames and decimates by 4 through
// fil4_, whose low-pass removes the images a plain sample doubling makes.
void FlexTransceiver::on_audio ()
{
  static char buf[65536];
  while (audio_ && audio_->hasPendingDatagrams ())
    {
      qint64 const n = audio_->readDatagram (buf, sizeof buf);
      if (n < 16) continue;
      if (be32 (buf + 12) == flex_meter_class) {parse_meters (buf, int (n)); continue;}   // meters share the socket
      if (!rx_stream_ || be32 (buf + 4) != rx_stream_) continue;
      int off = 0, bytes = 0;
      if (!parse_vita (buf, int (n), &off, &bytes)) continue;
      last_rx_audio_ms_ = QDateTime::currentMSecsSinceEpoch ();
      if (!audio_on_) continue;
      int const floats = bytes / 4;
      int const mono = floats / 2;
      rx_floats_.resize (int (mono * 2 * bytesPerFrame * sizeof (float)));
      float * out = reinterpret_cast<float *> (rx_floats_.data ());
      for (int i = 0; i < mono; ++i)
        {
          float const v = be_float (buf + off + i * 8);         // L of frame i
          *out++ = v; *out++ = v;                               // 48 kHz frame 2i
          *out++ = v; *out++ = v;                               // 48 kHz frame 2i+1
        }
      writeAudioData (reinterpret_cast<float *> (rx_floats_.data ()), mono * 2 * bytesPerFrame);
    }
}

void FlexTransceiver::store (float * source, size_t numFrames, qint16 * dest)
{
  static constexpr float K = 0x7FFF;
  for (size_t i {0}; i < numFrames; ++i) dest[i] = vu_rx_sample (K * source[i * 2]);
}

quint32 FlexTransceiver::writeAudioData (float * data, qint32 maxSize)
{
  static unsigned mstr0 = 999999;
  qint64 ms0 = m_jtdxtime->currentMSecsSinceEpoch2 () % 86400000;
  unsigned mstr = ms0 % int (1000.0 * m_period);
  if (mstr < mstr0 / 2) {dec_data.params.kin = 0; m_bufferPos = 0;}
  mstr0 = mstr;
  size_t framesAcceptable ((sizeof (dec_data.d2) / sizeof (dec_data.d2[0]) - dec_data.params.kin) * m_downSampleFactor);
  size_t framesAccepted (qMin (static_cast<size_t> (maxSize / bytesPerFrame), framesAcceptable));
  for (unsigned remaining = framesAccepted; remaining; )
    {
      size_t numFramesProcessed (qMin (m_samplesPerFFT * m_downSampleFactor - m_bufferPos, remaining));
      store (&data[(framesAccepted - remaining) * bytesPerFrame], numFramesProcessed, &m_buffer[m_bufferPos]);
      m_bufferPos += numFramesProcessed;
      if (m_bufferPos == m_samplesPerFFT * m_downSampleFactor)
        {
          qint32 framesToProcess (m_samplesPerFFT * m_downSampleFactor);
          qint32 framesAfterDownSample (m_samplesPerFFT);
          if (dec_data.params.kin >= 0 && dec_data.params.kin < (NTMAX * 12000 - framesAfterDownSample))
            {
              fil4_ (&m_buffer[0], &framesToProcess, &dec_data.d2[dec_data.params.kin], &framesAfterDownSample, &dec_data.dd2[dec_data.params.kin]);
              dec_data.params.kin += framesAfterDownSample;
            }
          Q_EMIT tciframeswritten (dec_data.params.kin);
          m_bufferPos = 0;
        }
      remaining -= numFramesProcessed;
    }
  return maxSize;
}

// ---------------------------------------------------------------- TX audio
// DAX TX is NOT the mirror of DAX RX (MSHV-Mac, verified against the radio's
// own FWDPWR meter): type 1, class 0x534C0123, TSI 3 / TSF 1, 284 bytes =
// 28 of header + 128 mono int16 big-endian at 24 kHz, to UDP 4991.
void FlexTransceiver::tx_audio (bool keyed)
{
  if (keyed)
    {
      if (!tx_stream_ || tx_keyed_) return;
      tx_packet_count_ = 0;
      tx_sent_frames_ = 0;
      tx_clock_.start ();
      tx_keyed_ = true;
      if (tx_timer_) tx_timer_->start (4);
    }
  else
    {
      tx_keyed_ = false;
      if (tx_timer_) tx_timer_->stop ();
    }
}

void FlexTransceiver::on_tx_tick ()
{
  if (!tx_keyed_ || !tx_stream_ || !audio_ || !connected_) return;
  // whole packets against the clock, so a late timer cannot starve the radio
  qint64 const due = (tx_clock_.elapsed () * qint64 (flex_rate)) / 1000;
  int packets = int ((due - tx_sent_frames_) / flex_frames_per_packet);
  if (packets <= 0) return;
  if (packets > 8) packets = 8;
  QHostAddress const radio {socket_->peerAddress ()};
  int const payload = flex_frames_per_packet * 2;
  QByteArray pkt (28 + payload, 0);
  float frames48[flex_frames_per_packet * 2 * 2];        // 256 stereo frames at 48 kHz
  for (int p = 0; p < packets; ++p)
    {
      std::fill (frames48, frames48 + sizeof frames48 / sizeof frames48[0], 0.0f);
      readAudioData (frames48, flex_frames_per_packet * 2 * bytesPerFrame);
      char * d = pkt.data ();
      put_be32 (d, 0x18D00047u | (quint32 (tx_packet_count_ & 0x0f) << 16));
      put_be32 (d + 4, tx_stream_);
      put_be32 (d + 8, 0x00001C2D);                         // FlexRadio OUI
      put_be32 (d + 12, flex_tx_class);
      put_be32 (d + 16, 0); put_be32 (d + 20, 0); put_be32 (d + 24, 0);
      for (int i = 0; i < flex_frames_per_packet; ++i)
        {
          float v = frames48[i * 4] * tx_gain_ * 32767.0f;   // L of every second 48 kHz frame
          if (v > 32767.0f) v = 32767.0f;
          if (v < -32768.0f) v = -32768.0f;
          quint16 const w = quint16 (qint16 (v));
          d[28 + i * 2] = char ((w >> 8) & 0xff);
          d[28 + i * 2 + 1] = char (w & 0xff);
        }
      audio_->writeDatagram (pkt, radio, flex_vita_port);
      tx_packet_count_ = quint8 ((tx_packet_count_ + 1) & 0x0f);
      tx_sent_frames_ += flex_frames_per_packet;
    }
}

// ---------------------------------------------------------------- modulator (as TCITransceiver)
void FlexTransceiver::do_modulator_start (unsigned symbolsLength, double framesPerSymbol,
                                          double frequency, double toneSpacing, bool synchronize, double dBSNR, double TRperiod)
{
  qint64 ms0 = m_jtdxtime->currentMSecsSinceEpoch2 () % 86400000;
  unsigned mstr = ms0 % int (1000.0 * m_period);
  if (m_state != Idle) throw error {tr ("Flex modulator not Idle")};
  m_quickClose = false;
  m_symbolsLength = symbolsLength;
  m_isym0 = std::numeric_limits<unsigned>::max ();
  m_frequency0 = 0.;
  m_phi = 0.;
  m_addNoise = dBSNR < 0.;
  m_nsps = framesPerSymbol;
  m_trfrequency = frequency;
  m_amp = std::numeric_limits<qint16>::max ();
  m_toneSpacing = toneSpacing;
  m_TRperiod = TRperiod;
  unsigned delay_ms = 1000;
  if (m_nsps == 1920) delay_ms = 500;
  else if (m_nsps == 576) delay_ms = 500;
  else if (m_nsps == 288) delay_ms = 500;
  if (m_addNoise)
    {
      m_snr = qPow (10.0, 0.05 * (dBSNR - 6.0));
      m_fac = 3000.0;
      if (m_snr > 1.0) m_fac = 3000.0 / m_snr;
    }
  auto mstr2 = mstr - delay_ms;
  if (mstr <= delay_ms) m_ic = 0;
  else m_ic = mstr2 * (audioSampleRate / 1000);
  m_silentFrames = 0;
  m_periodFree = !synchronize;
  if (m_periodFree) m_ic = 0;
  if (m_ic == 0 && synchronize && !m_tuning)
    m_silentFrames = audioSampleRate / (1000 / delay_ms) - (mstr * (audioSampleRate / 1000));
  m_state = (synchronize && m_silentFrames) ? Synchronizing : Active;
  Q_EMIT tci_mod_active (m_state != Idle);
}

void FlexTransceiver::do_modulator_stop (bool quick)
{
  m_quickClose = quick;
  if (m_state != Idle)
    {
      m_state = Idle;
      Q_EMIT tci_mod_active (false);
    }
}

quint16 FlexTransceiver::readAudioData (float * data, qint32 maxSize)
{
  double toneFrequency = 1500.0;
  if (m_nsps == 6) {toneFrequency = 1000.0; m_trfrequency = 1000.0; m_frequency0 = 1000.0;}
  if (maxSize == 0) return 0;
  qint64 numFrames (maxSize / bytesPerFrame);
  float * samples (reinterpret_cast<float *> (data));
  float * end (samples + numFrames * bytesPerFrame);
  qint64 framesGenerated (0);
  switch (m_state)
    {
    case Synchronizing:
      {
        if (m_silentFrames)
          {
            framesGenerated = qMin (m_silentFrames, numFrames);
            for ( ; samples != end; samples = load (0, samples)) {}
            m_silentFrames -= framesGenerated;
            return framesGenerated * bytesPerFrame;
          }
        m_state = Active;
        Q_EMIT tci_mod_active (true);
        m_cwLevel = false;
        m_ramp = 0;
      }
      // fall through
    case Active:
      {
        unsigned int isym = 0;
        qint16 sample = 0;
        if (!m_tuning) isym = m_ic / (4.0 * m_nsps);
        bool slowCwId = ((isym >= m_symbolsLength) && (icw[0] > 0));
        m_nspd = 2560;
        if (m_TRperiod > 16.0 && slowCwId)
          {
            m_dphi = m_twoPi * m_trfrequency / audioSampleRate;
            unsigned ic0 = m_symbolsLength * 4 * m_nsps;
            unsigned j (0);
            while (samples != end)
              {
                j = (m_ic - ic0) / m_nspd + 1;
                bool level {bool (icw[j])};
                m_phi += m_dphi;
                if (m_phi > m_twoPi) m_phi -= m_twoPi;
                sample = 0;
                float amp = 32767.0;
                float x = 0.0;
                if (m_ramp != 0)
                  {
                    x = qSin (float (m_phi));
                    if (SOFT_KEYING) {amp = qAbs (qint32 (m_ramp)); if (amp > 32767.0) amp = 32767.0;}
                    sample = round (amp * x);
                  }
                if (int (j) <= icw[0] && j < NUM_CW_SYMBOLS)
                  {
                    samples = load (postProcessSample (sample), samples);
                    ++framesGenerated; ++m_ic;
                  }
                else
                  {
                    m_state = Idle;
                    Q_EMIT tci_mod_active (false);
                    return framesGenerated * bytesPerFrame;
                  }
                if ((m_ramp != 0 && m_ramp != std::numeric_limits<qint16>::min ()) || level != m_cwLevel) m_ramp += RAMP_INCREMENT;
                m_cwLevel = level;
              }
            return framesGenerated * bytesPerFrame;
          }
        double const baud (12000.0 / m_nsps);
        unsigned int i0, i1;
        if (m_tuning) i1 = i0 = 9999 * m_nsps;
        else {i0 = (m_symbolsLength - 0.017) * 4.0 * m_nsps; i1 = m_symbolsLength * 4.0 * m_nsps;}
        sample = 0;
        for (unsigned i = 0; i < numFrames && m_ic <= i1; ++i)
          {
            if (m_TRperiod > 16.0 || m_tuning)
              {
                isym = 0;
                if (!m_tuning) isym = m_ic / (4.0 * m_nsps);
                if (isym != m_isym0 || m_trfrequency != m_frequency0)
                  {
                    if (itone[0] >= 100) m_toneFrequency0 = itone[0];
                    else if (m_toneSpacing == 0.0) m_toneFrequency0 = m_trfrequency + itone[isym < NUM_WSPR_SYMBOLS ? isym : NUM_WSPR_SYMBOLS - 1] * baud;
                    else m_toneFrequency0 = m_trfrequency + itone[isym < NUM_WSPR_SYMBOLS ? isym : NUM_WSPR_SYMBOLS - 1] * m_toneSpacing;
                    m_dphi = m_twoPi * m_toneFrequency0 / audioSampleRate;
                    m_isym0 = isym;
                    m_frequency0 = m_trfrequency;
                  }
                int j = m_ic / 480;
                if (m_fSpread > 0.0 && j != m_j0)
                  {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
                    float x1 = QRandomGenerator::global ()->generateDouble ();
                    float x2 = QRandomGenerator::global ()->generateDouble ();
#else
                    float x1 = (float) qrand () / RAND_MAX;
                    float x2 = (float) qrand () / RAND_MAX;
#endif
                    toneFrequency = m_toneFrequency0 + 0.5 * m_fSpread * (x1 + x2 - 1.0);
                    m_dphi = m_twoPi * toneFrequency / audioSampleRate;
                    m_j0 = j;
                  }
                m_phi += m_dphi;
                if (m_phi > m_twoPi) m_phi -= m_twoPi;
                if (m_ic == 0) m_amp = m_amp * 0.008144735;
                if (m_ic > 0 && m_ic < 191) m_amp = m_amp / 0.975;
                if (m_ic > i0) m_amp = 0.99 * m_amp;
                if (m_ic > i1) m_amp = 0.0;
                sample = qRound (m_amp * qSin (m_phi));
              }
            if (!m_tuning && (m_toneSpacing < 0.0)) {m_amp = 32767.0; sample = qRound (m_amp * foxcom_.wave[m_ic]);}
            samples = load (postProcessSample (sample), samples);
            ++framesGenerated; ++m_ic;
          }
        if (m_periodFree && !m_tuning && m_ic > i1) m_amp = 0.0;
        if (m_amp == 0.0)
          {
            if (icw[0] == 0)
              {
                m_state = Idle;
                Q_EMIT tci_mod_active (false);
                return framesGenerated * bytesPerFrame;
              }
            m_phi = 0.0;
          }
        m_frequency0 = m_trfrequency;
        while (samples != end) {samples = load (0, samples); ++framesGenerated;}
        return framesGenerated * bytesPerFrame;
      }
      // fall through
    case Idle:
      while (samples != end) samples = load (0, samples);
    }
  return 0;
}

qint16 FlexTransceiver::postProcessSample (qint16 sample) const
{
  if (m_addNoise)
    {
      qint32 s = m_fac * (gran () + sample * m_snr / 32768.0);
      if (s > std::numeric_limits<qint16>::max ()) s = std::numeric_limits<qint16>::max ();
      if (s < std::numeric_limits<qint16>::min ()) s = std::numeric_limits<qint16>::min ();
      sample = s;
    }
  return sample;
}
