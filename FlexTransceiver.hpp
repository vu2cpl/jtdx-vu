#ifndef FLEX_TRANSCEIVER_HPP__
#define FLEX_TRANSCEIVER_HPP__

// JTDX-VU: native FlexRadio VITA-49 rig - SmartSDR control over TCP, DAX
// receive audio in and DAX transmit audio out over VITA-49 UDP, straight off
// a FLEX-6000/8000 with no SmartSDR, DAX driver or TCI bridge in the path.
// Ported from the MSHV-Mac backend (network.cpp, "flex native vita-49",
// Copyright (C) 2026 Manoj Ramawarrier VU2CPL, layout agreed with LZ2HV),
// shaped like TCITransceiver so the rest of JTDX-VU sees "a network rig with
// its own audio": the TCI audio tick, tciframeswritten / tci_mod_active and
// the reconnect loop all apply unchanged.
//
// Wire format, measured on a FLEX-6600 (MSHV-Mac, 2026-09-04):
//   RX  radio -> us   type 3, class 0x534C03E3, 24 kHz stereo float32 BE, L==R
//   TX  us -> radio   type 1, class 0x534C0123, 24 kHz MONO int16 BE, 284 bytes

#include <memory>
#include <string>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QMap>
#include <QByteArray>
#include <QElapsedTimer>
#include "TransceiverFactory.hpp"
#include "PollingTransceiver.hpp"
#include "AudioDevice.hpp"   // vu_rx_sample

class QTcpSocket;
class QUdpSocket;
class QTimer;
class QEventLoop;
class JTDXDateTime;

class FlexTransceiver final
  : public PollingTransceiver
{
  Q_OBJECT;

public:
  static void register_transceivers (TransceiverFactory::Transceivers *, unsigned first_id);
  static int slices_offered ();
  // slice: 0 = A .. 7 = H (the letter chosen in Settings); address host[:port]
  explicit FlexTransceiver (int slice, QString const& address, bool use_for_ptt,
                            int poll_interval, QObject * parent = nullptr);
  ~FlexTransceiver ();

private slots:
  void on_connected ();
  void on_disconnected ();
  void on_error ();
  void on_ready_read ();
  void on_audio ();
  void on_tx_tick ();

signals:
  void flex_done ();

protected:
  int do_start (JTDXDateTime*) override;
  void do_stop () override;
  void do_frequency (Frequency, MODE, bool no_ignore) override;
  void do_tx_frequency (Frequency, MODE, bool no_ignore) override;
  void do_mode (MODE) override;
  void do_ptt (bool on) override;
  void do_poll () override;
  void do_audio (bool on) override;
  void do_tune (bool on) override;
  void do_period (double period) override;
  void do_blocksize (qint32 blocksize) override;
  void do_spread (double spread) override {m_fSpread = spread;}
  void do_nsym (int nsym) override {m_symbolsLength = nsym;}
  void do_trfrequency (double f) override {m_trfrequency = f;}
  void do_txvolume (qreal volume) override;
  void do_modulator_start (unsigned symbolsLength, double framesPerSymbol, double frequency,
                           double toneSpacing, bool synchronize = true, double dBSNR = 99., double TRperiod = 60.0) override;
  void do_modulator_stop (bool quick = false) override;
  bool in_nested_wait () const override {return nested_waits_ > 0;}
  void abort_waits () override;

private:
  // ---- control session
  int send (QString const& cmd);                 // -> sequence number, or -1
  bool wait_reply (int seq, int ms);             // nested wait for R<seq>|
  void wait_ms (int ms);                         // nested wait, plain
  void handle_line (QString const& line);
  void parse_slice_line (int slice, QString const& line);
  int owned_slice_by_letter (int letter) const;
  int owned_slice () const;
  bool slice_exists (int n) const;
  QString handle_hex () const;
  bool start_session ();                         // the whole start-up, or error_
  void release (bool tell_radio);
  QString map_mode (MODE) const;
  MODE unmap_mode (QString const&) const;

  // ---- audio (mirrors TCITransceiver)
  quint32 writeAudioData (float * data, qint32 maxSize);
  quint16 readAudioData (float * data, qint32 maxSize);
  qint16 postProcessSample (qint16 sample) const;
  void store (float * source, size_t numFrames, qint16 * dest);
  float * load (qint16 sample, float * dest)
  {
    static constexpr float K1 = 0.999 / 0x7FFF;
    float const value = K1 * static_cast<float> (sample);
    *dest++ = value;
    *dest++ = value;
    return dest;
  }
  void tx_audio (bool keyed);
  enum ModulatorState {Synchronizing, Active, Idle};

  int slice_letter_;                 // what Settings asked for, 0 = A
  QString host_;
  quint16 port_;
  bool use_for_ptt_;
  bool do_snr_;
  bool tci_audio_;                   // JTDX-VU's "TCI audio" tick: we are the sound card
  QString error_;

  QTcpSocket * socket_;
  QUdpSocket * audio_;
  QTimer * tx_timer_;
  QTimer * wait_timer_;
  QEventLoop * wait_loop_;
  int nested_waits_ {0};
  bool stop_aborting_wait_ {false};

  QByteArray rx_buf_;
  int seq_;
  QMap<int, QPair<quint32, QString>> replies_;   // seq -> code, body
  QStringList status_log_;
  QString last_slice_line_;
  quint32 handle_;
  bool connected_;
  bool ready_;
  QString model_;

  int slice_;                        // the slice in use, -1 none
  bool slice_created_;
  int dax_ch_;
  quint32 rx_stream_;
  quint32 tx_stream_;
  bool tx_dax_;

  Frequency frequency_;              // from the radio
  Frequency last_set_frequency_;
  MODE mode_;
  QString mode_str_;
  bool PTT_;
  bool busy_;
  qint64 last_rx_audio_ms_ {0};

  // ---- RX decimation into the decoder
  JTDXDateTime * m_jtdxtime;
  double m_period = 15.0;
  unsigned m_downSampleFactor;
  qint32 m_samplesPerFFT;
  static size_t const max_buffer_size {7 * 512};
  QScopedArrayPointer<short> m_buffer;
  unsigned m_bufferPos;
  bool audio_on_;
  static size_t const bytesPerFrame = 2;
  QByteArray rx_floats_;             // 48 kHz stereo frames made from the 24 kHz mono stream

  // ---- modulator (as TCITransceiver)
  bool m_quickClose = false;
  unsigned m_symbolsLength;
  static double constexpr m_twoPi = 2.0 * 3.141592653589793238462;
  unsigned m_nspd = 2048 + 512;
  double m_phi, m_dphi, m_amp, m_nsps, m_trfrequency, m_frequency0, m_snr, m_fac, m_toneSpacing, m_fSpread, m_TRperiod;
  qint64 m_silentFrames;
  qint16 m_ramp;
  ModulatorState m_state;
  bool m_tuning;
  bool m_periodFree {false};
  bool m_addNoise;
  bool m_cwLevel;
  unsigned m_ic;
  unsigned m_isym0;
  int m_j0;
  double m_toneFrequency0;
  quint32 audioSampleRate;

  // ---- DAX TX pacing
  bool tx_keyed_;
  quint8 tx_packet_count_;
  QElapsedTimer tx_clock_;
  qint64 tx_sent_frames_;
  float tx_gain_;
  QByteArray tx_scratch_;
  std::string debug_file_;
};

#endif
