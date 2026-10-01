// JTDX-VU: JTTY mode - GUI side.
//
// JTTY is WSJT-X 3.2's period-free, RTTY-style keyboard mode (K1JT, K9AN,
// G4KLA, N9ADG, DL3WDG, W3SZ, KJ5HST, KD0BTO; GPL v3).  The decoder runs
// in-process on the shared dec_data.d2 ring buffer on every audio chunk,
// and reports messages that grow frame by frame, keyed by message id.  This
// file is a port of WSJT-X's widgets/mainwindow_jtty.cpp to JTDX, without
// the N1MM/MMTTY bridge, transmit evidence and replay machinery.

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <array>
#include <algorithm>
#include <cmath>
#include <cstddef>

#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextBlock>
#include <QScrollBar>
#include <QRegularExpression>
#include <QStatusBar>
#include <QTimer>
#include <QAction>

#include "commons.h"
#include "jttypanel.h"
#include "jttysettings.h"
#include "displaytext.h"
#include "widegraph.h"
#include "Detector.hpp"
#include "Modulator.hpp"
#include "JTDXDateTime.h"
#include "Radio.hpp"

extern dec_data_t dec_data;   // the shared decoder buffer, defined in mainwindow.cpp

namespace
{
  constexpr int jttyMaxUpdates = 30;      // BATCH_SIZE in jtty_get_updates
  constexpr int jttyMessageSize = 80;     // MESSAGE_LENGTH
  constexpr int jttyUpdateBufferSize = jttyMaxUpdates * jttyMessageSize;
  constexpr int jttyNsps = 384;           // samples per symbol at 12000 Sa/s: 31.25 baud
  // gfortran (>= 8) passes hidden character lengths as size_t
  using jtty_charlen_t = std::size_t;

  QString formatJttyDecodeLine (float frequency, QString const& message)
  {
    QString const frequencyText = QStringLiteral ("%1").arg (qRound (frequency), 4);
    return message.isEmpty () ? frequencyText : frequencyText + QStringLiteral ("  ") + message;
  }

  QString jttyLineTimeLabel (QDateTime const& utc)
  {
    return utc.isValid () ? utc.toUTC ().toString ("hhmmss") : QString {};
  }

  struct JttyMessageUpdate
  {
    qint64 messageId {0};
    float frequency {0.f};
    QString text;
    float sequenceStart {0.f};
    bool complete {false};
  };

  // Later frames retain their admitted message identity despite decoder frequency drift.
  bool shouldApplyToQsoHistory (bool alreadyPresent, float frequency, float rxFrequency, float tolerance)
  {
    return alreadyPresent || std::abs (frequency - rxFrequency) < tolerance;
  }
}

extern "C" {
  void rjtty_sub_ (short int d2[], int * k, int * nsps, int * nfa, int * nfb, float * f0, float * ftol);
  void jtty_get_updates_ (char text_blocks[], qint64 message_ids[], float frequencies[],
                          float start_tsync[], bool eom[], int * count, jtty_charlen_t);
  void genjtty_profile_ (char * msg, int const * exchange_profile, int itone[], int * nsym, jtty_charlen_t);
  void gen_jttywave_ (int itone[], int * nsym, int * nsps, float * bt, float * fsample, float * f0,
                      float xjunk[], float wave[], int * icmplx, int * nwave);
}

void MainWindow::jttyModeSelected ()
{
  if (m_mode == "WSPR-2") killFile ();
  m_mode = "JTTY";
  WSPR_config (false);
  switch_mode (Modes::JTTY);
  m_modeTx = "JTTY";
  m_actionJTTY->setChecked (true);
  mode_label->setStyleSheet (QString ("QLabel{background: %1}").arg (Radio::convert_dark ("#f0c27a", m_useDarkStyle)));
  ui->pbTxMode->setText ("Tx JTTY");
  ui->pbTxMode->setEnabled (false);
  on_AutoSeqButton_clicked (false);          // no auto-sequencing in a keyboard mode
  // JTTY has no T/R period.  A nominal period keeps the detector's ring
  // buffer and the waterfall cycling; rjtty_sub restarts its own state on
  // every buffer wrap.  120 s fills JTDX's NTMAX (120 s) buffer exactly.
  m_TRperiod = 120.0;
  m_modulator->setPeriod (m_TRperiod);
  m_detector->setPeriod (m_TRperiod);
  // diskDat() sweeps a wav in m_FFTSize (3456) steps up to m_hsymStop with no
  // bound on k: 120 s * 12000 / 3456 = 416.7 steps keeps it inside d2.
  m_hsymStop = 417;
  // a frequency list saved before JTTY existed has no JTTY rows, so the band
  // combo and the band buttons would have nothing to pick
  if (m_config.seed_default_frequencies (Modes::JTTY))
    statusBar ()->showMessage (tr ("JTTY dial frequencies added to Settings > Frequencies"), 8000);
  ui->RxFreqSpinBox->setValue (1500);         // JTTY convention, as WSJT-X
  ui->TxFreqSpinBox->setValue (1500);
  flushJttyDecodeLines ();
  commonActions ();
  enableHoundAccess (false);
  ui->decodedTextLabel->setText ("Freq  Message");
  ui->decodedTextLabel2->setText ("Freq  Message");
}

// Forget the current JTTY session: start a fresh display group on the next
// decode and drop the per-message state.
void MainWindow::flushJttyDecodeLines ()
{
  m_jttyAllFreqLines.clear ();
  m_jttyQsoLines.clear ();
  m_jttyAllFreqsGroupStart = QTextBlock ();
  m_jttyQsoGroupStart = QTextBlock ();
  m_jttyQsoGroupEnd = QTextBlock ();
  m_jttyQsoGroupEndPosition = -1;
  m_jttyLastAllFreqsK = -1;
}

// Called from dataSink() on every audio chunk while in JTTY mode.  k is the
// number of valid samples in dec_data.d2.  Returns true if any message
// reached its end-of-message on this call.
bool MainWindow::jtty_decode (int k)
{
  if (m_mode != "JTTY" || !m_jttyPanel) return false;
  int const bufferSamples = int (sizeof (dec_data.d2) / sizeof (dec_data.d2[0]));
  k = qMin (k, bufferSamples);
  if (m_diskData) k = qMin (k, dec_data.params.kin);
  if (k <= 0) return false;

  auto lineDateTimeUtc = [this, k] (float tsync) -> QDateTime {
    // tsync is the message start in seconds from the start of the buffer
    if (m_diskData) return QDateTime {};   // wav start time is not tracked yet
    double const elapsed = qMax (0.0, double (k) / 12000.0 - double (tsync));
    return m_jtdxtime->currentDateTimeUtc2 ().addMSecs (-qRound64 (1000.0 * elapsed));
  };

  int nsps = jttyNsps;
  // Unchanged k means no new samples (diskDat keeps sweeping past the end of
  // a file with k clamped): nothing to decode, and NOT a new session.
  if (k == m_jttyLastAllFreqsK) return false;
  // k going backwards (buffer wrap, new file, mode entry) starts a distinct
  // displayed decode session.
  bool const newSession = (k < m_jttyLastAllFreqsK);
  if (newSession) flushJttyDecodeLines ();
  m_jttyLastAllFreqsK = k;

  float f0 = ui->RxFreqSpinBox->value ();
  float ftol = m_jttyPanel->ftol ();
  int nfa = m_wideGraph->nStartFreq ();
  int nfb = m_wideGraph->Fmax ();

  rjtty_sub_ (dec_data.d2, &k, &nsps, &nfa, &nfb, &f0, &ftol);

  QVector<JttyMessageUpdate> updates;
  int updateCount {0};
  do
    {
      std::array<char, jttyUpdateBufferSize> textBlocks {};
      std::array<qint64, jttyMaxUpdates> messageIds {};
      std::array<float, jttyMaxUpdates> frequencies {};
      std::array<float, jttyMaxUpdates> sequenceStarts {};
      std::array<bool, jttyMaxUpdates> complete {};
      jtty_get_updates_ (textBlocks.data (), messageIds.data (), frequencies.data (),
                         sequenceStarts.data (), complete.data (), &updateCount,
                         jtty_charlen_t {jttyUpdateBufferSize});
      for (int i = 0; i < updateCount; ++i)
        {
          QString const text = QString::fromLatin1 (textBlocks.data () + i * jttyMessageSize, jttyMessageSize).trimmed ();
          if (messageIds[i] <= 0) continue;
          JttyMessageUpdate u;
          u.messageId = messageIds[i];
          u.frequency = frequencies[i];
          u.text = text;
          u.sequenceStart = sequenceStarts[i];
          u.complete = complete[i];
          updates.append (u);
        }
    }
  while (updateCount == jttyMaxUpdates);
  if (updates.isEmpty ()) return false;

  if (m_jttyAutoCq)
    for (auto const& update : updates) jttyAutoCqCheckDecode (update.text);

  // Band Activity: every message on the band, updated in place by id
  bool allChanged {false};
  for (auto const& update : updates)
    {
      auto known = std::find_if (m_jttyAllFreqLines.begin (), m_jttyAllFreqLines.end (),
                                 [&update] (JttyDecodeLine const& line) {return line.messageId == update.messageId;});
      if (known == m_jttyAllFreqLines.end ())
        {
          JttyDecodeLine line;
          line.messageId = update.messageId;
          line.frequency = update.frequency;
          line.text = update.text;
          line.sequenceStart = update.sequenceStart;
          line.messageStartUtc = lineDateTimeUtc (update.sequenceStart);
          line.complete = update.complete;
          m_jttyAllFreqLines.append (line);
          allChanged = true;
        }
      else
        {
          bool const complete = known->complete || update.complete;
          if (known->text != update.text || known->frequency != update.frequency || known->complete != complete) allChanged = true;
          known->text = update.text;
          known->frequency = update.frequency;
          known->complete = complete;
        }
    }
  if (allChanged) renderJttyAllFreqLines ();

  for (auto& known : m_jttyAllFreqLines)
    {
      if (known.complete && !known.written)
        {
          writeToALLTXT ("JTTY Rx " + formatJttyDecodeLine (known.frequency, known.text));
          known.written = true;
        }
    }

  // Rx Frequency pane: messages within F Tol of the Rx frequency
  bool anyEom {false};
  bool anyLineChanged {false};
  for (auto const& update : updates)
    {
      auto known = std::find_if (m_jttyQsoLines.begin (), m_jttyQsoLines.end (),
                                 [&update] (JttyQsoLine const& line) {return line.messageId == update.messageId;});
      bool const alreadyPresent = known != m_jttyQsoLines.end ();
      if (!shouldApplyToQsoHistory (alreadyPresent, update.frequency, f0, ftol)) continue;
      if (update.complete) anyEom = true;
      if (alreadyPresent)
        {
          if (known->text == update.text && known->frequency == update.frequency) continue;
          known->frequency = update.frequency;
          known->text = update.text;
        }
      else
        {
          auto const allLine = std::find_if (m_jttyAllFreqLines.cbegin (), m_jttyAllFreqLines.cend (),
                                             [&update] (JttyDecodeLine const& line) {return line.messageId == update.messageId;});
          QDateTime const startUtc = allLine != m_jttyAllFreqLines.cend () ? allLine->messageStartUtc
                                                                            : lineDateTimeUtc (update.sequenceStart);
          JttyQsoLine qso;
          qso.messageId = update.messageId;
          qso.frequency = update.frequency;
          qso.text = update.text;
          qso.sequenceStart = update.sequenceStart;
          qso.messageStartUtc = startUtc;
          m_jttyQsoLines.append (qso);
        }
      anyLineChanged = true;
    }
  if (anyLineChanged) renderJttyQsoLines ();
  return anyEom;
}

// Re-render after a Lower case / Include time change.
void MainWindow::jttyRefreshDisplay ()
{
  if (m_mode != "JTTY") return;
  renderJttyAllFreqLines ();
  renderJttyQsoLines ();
}

void MainWindow::renderJttyAllFreqLines ()
{
  if (m_jttyAllFreqLines.isEmpty ()) return;
  QStringList displayLines;
  for (auto const& line : m_jttyAllFreqLines)
    {
      QString displayLine = formatJttyDecodeLine (line.frequency, line.text);
      if (m_jttyPanel->lowerCase ()) displayLine = displayLine.toLower ();
      if (m_jttyPanel->includeTime ())
        {
          QString const time = jttyLineTimeLabel (line.messageStartUtc);
          if (!time.isEmpty ()) displayLine = time + " " + displayLine;
        }
      displayLines.append (displayLine);
    }

  QTextCharFormat format;
  format.setFont (ui->decodedTextBrowser->contentFont ());
  // JTDX-VU: edit through a document cursor and follow the end only when the
  // view is already there, so a line keeps still while the operator clicks it
  auto * const bar = ui->decodedTextBrowser->verticalScrollBar ();
  bool const follow = bar->value () >= bar->maximum () - 2;
  QTextCursor cursor {ui->decodedTextBrowser->document ()};
  if (m_jttyAllFreqsGroupStart.isValid ())
    {
      cursor.setPosition (m_jttyAllFreqsGroupStart.position ());
      cursor.movePosition (QTextCursor::End, QTextCursor::KeepAnchor);
      cursor.removeSelectedText ();
    }
  else
    {
      cursor.movePosition (QTextCursor::End);
      if (cursor.position () > 0) cursor.insertBlock ();
    }
  m_jttyAllFreqsGroupStart = cursor.block ();
  cursor.insertText (displayLines.join (QChar {'\n'}), format);
  if (follow) bar->setValue (bar->maximum ());
}

void MainWindow::renderJttyQsoLines ()
{
  if (m_jttyQsoLines.isEmpty ()) return;
  auto * const bar = ui->decodedTextBrowser2->verticalScrollBar ();
  bool const follow = bar->value () >= bar->maximum () - 2;
  QTextCursor cursor {ui->decodedTextBrowser2->document ()};
  if (m_jttyQsoGroupStart.isValid () && m_jttyQsoGroupEnd.isValid ()
      && m_jttyQsoGroupEndPosition >= m_jttyQsoGroupStart.position ())
    {
      cursor.setPosition (m_jttyQsoGroupStart.position ());
      cursor.setPosition (m_jttyQsoGroupEndPosition, QTextCursor::KeepAnchor);
      cursor.removeSelectedText ();
    }
  else
    {
      cursor.movePosition (QTextCursor::End);
      if (cursor.position () > 0) cursor.insertBlock ();
    }

  QTextCharFormat format;
  format.setFont (ui->decodedTextBrowser2->contentFont ());
  m_jttyQsoGroupStart = cursor.block ();
  QStringList renderedLines;
  for (auto const& line : m_jttyQsoLines)
    {
      QString display = formatJttyDecodeLine (line.frequency, line.text);
      if (m_jttyPanel->lowerCase ()) display = display.toLower ();
      if (m_jttyPanel->includeTime ())
        {
          QString const time = jttyLineTimeLabel (line.messageStartUtc);
          if (!time.isEmpty ()) display = time + " " + display;
        }
      renderedLines.append (display);
    }
  cursor.insertText (renderedLines.join (QChar {'\n'}), format);
  m_jttyQsoGroupEnd = cursor.block ();
  m_jttyQsoGroupEndPosition = cursor.position ();
  if (follow) bar->setValue (bar->maximum ());
}

// ---- picking a call from the decode panes --------------------------------
//
// JTDX-VU: JTTY lines are free text, so a click (N1MM style), a double-click
// or a mouse selection on a word that looks like a call puts it in DX Call.
// Punctuation around it ("VU2CPL," "<VU2CPL>") is dropped; a slash inside
// stays (VU2OY/P).  Single clicks and selections fail silently, so clicking
// about in the text never nags; a double-click on a non-call says why.

void MainWindow::jttyClickOnCall (bool secondPane)
{
  if (m_mode != "JTTY") return;
  auto * const w = secondPane ? ui->decodedTextBrowser2 : ui->decodedTextBrowser;
  jttyPickCall (w->textCursor (), true);
}

bool MainWindow::jttyPickCall (QTextCursor cursor, bool quiet)
{
  QString token;
  auto const lineText = cursor.block ().text ();
  if (quiet && cursor.hasSelection ())
    token = cursor.selectedText ();                   // what the mouse selected
  else
    {
      // the whole space-delimited token under the pointer (QTextEdit's own
      // word breaks at '/' and punctuation)
      int const pos = (cursor.hasSelection () ? cursor.selectionStart () : cursor.position ())
        - cursor.block ().position ();
      int start = qBound (0, pos, lineText.size ()), end = start;
      while (start > 0 && !lineText.at (start - 1).isSpace ()) --start;
      while (end < lineText.size () && !lineText.at (end).isSpace ()) ++end;
      token = lineText.mid (start, end - start);
    }
  token = token.trimmed ().toUpper ();
  static QRegularExpression const edges {"^[^A-Z0-9]+|[^A-Z0-9]+$"};
  token.remove (edges);
  static QRegularExpression const callLike {"^[A-Z0-9]{1,3}[0-9][A-Z0-9]*[A-Z](/[A-Z0-9]+)?$|^[A-Z0-9]+/[A-Z0-9]{1,3}[0-9][A-Z0-9]*[A-Z]$"};
  if (token.size () >= 3 && callLike.match (token).hasMatch ())
    {
      if (ui->dxCallEntry->text ().trimmed ().toUpper () != token)
        {
          ui->dxCallEntry->setText (token);
          ui->dxGridEntry->clear ();
        }
      return true;
    }
  if (!quiet && !token.isEmpty ())
    statusBar ()->showMessage (tr ("JTTY: \"%1\" doesn't look like a callsign").arg (token), 4000);
  return false;
}

// ---- transmit ----------------------------------------------------------------
//
// JTTY has no T/R period, so nothing here waits for a slot: Send encodes the
// message, generates its wave into foxcom_.wave and opens a short request
// window; guiUpdate then keys up exactly as it does for Tune and keeps PTT
// until the Modulator has played the wave out and gone Idle.

bool MainWindow::jttyModulatorActive () const
{
  return m_tci ? m_tci_mod_active : m_modulator->isActive ();
}

bool MainWindow::jttyTxBusy () const
{
  return m_jttyTxRequestedUntil > 0 || m_jttyTxEndMs > 0 || m_transmitting;
}

// guiUpdate's m_bTxTime for JTTY
bool MainWindow::jttyUpdateTxState ()
{
  if (m_jttyTxRequestedUntil == 0 && m_jttyTxEndMs == 0) return false;
  qint64 const now = m_jtdxtime->currentMSecsSinceEpoch2 ();
  bool wanted;
  if (m_jttyTxEndMs == 0)
    {
      // waiting for PTT and the modulator to come up
      wanted = now < m_jttyTxRequestedUntil;
      if (jttyModulatorActive ())
        {
          qint64 const waveMs = qint64 (m_jttyNsym) * 4 * jttyNsps * 1000 / 48000;
          m_jttyTxEndMs = now + waveMs + 1000;    // hard stop if the modulator never reports Idle
          m_jttyTxRequestedUntil = 0;
        }
    }
  else
    {
      wanted = jttyModulatorActive () && now < m_jttyTxEndMs;
    }
  if (!wanted)
    {
      m_jttyTxEndMs = 0;
      m_jttyTxRequestedUntil = 0;
      jttyAutoCqAfterTx ();
    }
  return wanted;
}

// F1..F8: WSJT-X 3.2's macros - %M my call, %H his call (DX Call entry),
// %Q the Call next field (his call when empty), %N the serial number,
// %E the exchange from Settings > JTTY ("599 %N" by default), without %N
// when Serial Number is 0 (none)
void MainWindow::jttyMacro (int key)
{
  if (!m_jttyPanel) return;
  auto tpl = m_jttyPanel->macro (key);
  if (tpl.isEmpty ()) return;
  auto const my = m_config.my_callsign ().trimmed ().toUpper ();
  auto const his = ui->dxCallEntry->text ().trimmed ().toUpper ();
  auto queued = m_jttyPanel->callNext ();
  if (queued.isEmpty ()) queued = his;
  auto const serial = QString {"%1"}.arg (m_jttyPanel->serialNumber (), 3, 10, QLatin1Char {'0'});
  // %E first (an exchange may use any variable), then the station details:
  // they must go before %Q / %M, or %QTH would read as %Q + "TH"
  auto exchange = JttySettings::exchange (m_settings);
  if (m_jttyPanel->serialNumber () <= 0) exchange = exchange.remove ("%N").simplified ();
  tpl.replace ("%E", exchange);
  for (auto const& v : JttySettings::stationVars)
    {
      if (!tpl.contains (QLatin1String {v.token})) continue;
      auto const value = JttySettings::stationValue (m_settings, v.key);
      if (value.isEmpty ())
        {
          statusBar ()->showMessage (tr ("JTTY: set %1 in Settings > JTTY first").arg (tr (v.label)), 5000);
          return;
        }
      tpl.replace (QLatin1String {v.token}, value);
    }
  if (tpl.contains ("%M") && my.isEmpty ())
    {
      statusBar ()->showMessage (tr ("JTTY: set your callsign in Settings first"), 5000);
      return;
    }
  if (tpl.contains ("%H") && his.isEmpty ())
    {
      statusBar ()->showMessage (tr ("JTTY: enter the DX call first (double-click a decode)"), 5000);
      return;
    }
  if (tpl.contains ("%Q") && queued.isEmpty ())
    {
      statusBar ()->showMessage (tr ("JTTY: fill in Call next (or the DX call) first"), 5000);
      return;
    }
  tpl.replace ("%M", my).replace ("%H", his).replace ("%Q", queued).replace ("%N", serial);
  jtty_tx (tpl);
}

void MainWindow::jtty_tx (QString message)
{
  if (m_mode != "JTTY" || !m_jttyPanel) return;
  if (m_jttyAutoCq && !m_jttyAutoCqSending) jttyAutoCqStop (tr ("another message sent"));
  if (jttyTxBusy ())
    {
      statusBar ()->showMessage (tr ("JTTY: still transmitting - wait for the message to finish"), 4000);
      return;
    }
  message = message.trimmed ().left (jttyMessageSize);
  if (message.isEmpty ()) return;
  if (m_jttyPanel->lowerCase ()) message = message.toLower ();   // as WSJT-X: the option covers Tx too

  // genjtty takes a fixed 80-character frame and normalises case and
  // unsupported characters itself (lib/jtty/jtty_mod.f90)
  auto frame = message.leftJustified (jttyMessageSize, ' ').toLatin1 ();
  int itone[16 * 59] {};              // room for the longest possible frame
  int nsym {0};
  int const profile {0};              // no contest exchange profile yet
  genjtty_profile_ (frame.data (), &profile, itone, &nsym, jtty_charlen_t {jttyMessageSize});
  if (nsym <= 0)
    {
      statusBar ()->showMessage (tr ("JTTY: message could not be encoded"), 5000);
      return;
    }
  int nsps4 = 4 * jttyNsps;           // 1536 samples per symbol at 48 kHz
  float bt = 2.0f;
  float fsample = 48000.0f;
  float f0 = ui->TxFreqSpinBox->value () - m_XIT;
  int icmplx = 0;
  int nwave = nsps4 * nsym;
  if (nwave > int (sizeof (foxcom_.wave) / sizeof (foxcom_.wave[0])))
    {
      statusBar ()->showMessage (tr ("JTTY: message too long"), 5000);
      return;
    }
  gen_jttywave_ (itone, &nsym, &nsps4, &bt, &fsample, &f0, foxcom_.wave, foxcom_.wave, &icmplx, &nwave);
  m_jttyNsym = nsym;
  m_jttyTxEndMs = 0;
  m_jttyTxRequestedUntil = m_jtdxtime->currentMSecsSinceEpoch2 () + 3000;   // guiUpdate keys up from here

  auto const shown = QString::fromLatin1 (frame).trimmed ().toUpper ();
  m_currentMessage = shown;           // the "Transmitting ..." ALL.TXT line shows it
  writeToALLTXT ("JTTY Tx " + formatJttyDecodeLine (f0, shown));
  // WSJT-X: a "TU ..." message closes the contest QSO - bump the serial and
  // offer the log dialog
  if (shown.startsWith ("TU ") && !ui->dxCallEntry->text ().trimmed ().isEmpty ())
    {
      m_jttyPanel->setSerialNumber (m_jttyPanel->serialNumber () + 1);
      QTimer::singleShot (0, this, SLOT (on_logQSOButton_clicked ()));
    }
  tx_status_label->setText (tr ("Tx: ") + shown.left (30));
  // a Tx line in the Rx Frequency pane, in order with the decodes; negative
  // ids can never collide with the decoder's
  JttyQsoLine line;
  line.messageId = -(++m_jttyTxLineSeq);
  line.frequency = f0;
  line.text = "Tx: " + shown;
  line.messageStartUtc = m_jtdxtime->currentDateTimeUtc2 ();
  m_jttyQsoLines.append (line);
  renderJttyQsoLines ();
}

void MainWindow::jttyHalt ()
{
  jttyClearTx ();
  haltTx ("JTTY halt ");                 // calls on_stopTxButton_clicked -> jttyClearTx again, harmless
}

void MainWindow::jttyClearTx ()
{
  if (m_jttyAutoCq) jttyAutoCqStop (tr ("halted"));
  m_jttyTxRequestedUntil = 0;
  m_jttyTxEndMs = 0;
}

// ---- Auto CQ -----------------------------------------------------------------
//
// JTDX-VU: send the Auto CQ macro (Settings > JTTY, F1 by default), listen for
// the gap, send it again.  Halt/Esc, any other transmission, picking or typing
// a DX call, the call limit, a decode with my call (optional) or leaving JTTY
// stops it.

void MainWindow::jttyAutoCqToggled (bool on)
{
  if (!m_jttyPanel) return;
  if (!on)
    {
      // pressing it again while it runs is a Halt as well: stop calling
      // and cut the CQ that may be on the air
      jttyAutoCqStop (tr ("switched off"));
      if (jttyTxBusy ()) jttyHalt ();
      return;
    }
  if (m_mode != "JTTY" || !m_jttyPanel)
    {
      updateCnsButton ();
      return;
    }
  m_jttyAutoCq = true;
  m_jttyAutoCqCount = 0;
  updateCnsButton ();
  if (!jttyTxBusy ()) jttyAutoCqFire ();      // else it starts when this transmission ends
}

void MainWindow::jttyAutoCqFire ()
{
  if (!m_jttyAutoCq) return;
  if (m_mode != "JTTY" || !m_jttyPanel)
    {
      jttyAutoCqStop (tr ("left JTTY"));
      return;
    }
  if (jttyTxBusy ()) return;                   // jttyAutoCqAfterTx reschedules
  int const max = JttySettings::autoCqMax (m_settings);
  if (max > 0 && m_jttyAutoCqCount >= max)
    {
      jttyAutoCqStop (tr ("%n call(s) made", "", m_jttyAutoCqCount));
      return;
    }
  m_jttyAutoCqSending = true;
  jttyMacro (JttySettings::autoCqKey (m_settings));
  m_jttyAutoCqSending = false;
  if (!jttyTxBusy ())
    {
      // the macro could not be sent (no call set, empty macro, ...); its own
      // status message says why
      m_jttyAutoCq = false;
      m_jttyAutoCqTimer->stop ();
      updateCnsButton ();
      return;
    }
  ++m_jttyAutoCqCount;
}

void MainWindow::jttyAutoCqAfterTx ()
{
  if (!m_jttyAutoCq) return;
  int const gap = JttySettings::autoCqGap (m_settings);
  m_jttyAutoCqTimer->start (gap * 1000);
  statusBar ()->showMessage (tr ("Auto CQ: calling again in %1 s").arg (gap), gap * 1000);
}

void MainWindow::jttyAutoCqStop (QString const& why)
{
  bool const was = m_jttyAutoCq;
  m_jttyAutoCq = false;
  if (m_jttyAutoCqTimer) m_jttyAutoCqTimer->stop ();
  updateCnsButton ();
  if (was) statusBar ()->showMessage (tr ("Auto CQ stopped: %1").arg (why), 8000);
}

// a decode that carries my call as a word, and isn't my own CQ heard back
// (CQ ... MYCALL, ... DE MYCALL), means someone is answering
void MainWindow::jttyAutoCqCheckDecode (QString const& text)
{
  if (!JttySettings::autoCqStopOnMyCall (m_settings) || jttyTxBusy ()) return;
  auto const my = m_config.my_callsign ().trimmed ().toUpper ();
  if (my.isEmpty ()) return;
  auto const words = text.toUpper ().split (QRegularExpression {"[^A-Z0-9/]+"}, Qt::SkipEmptyParts);
  if (words.isEmpty () || words.front () == "CQ" || words.front () == "QRZ") return;
  for (int i = 0; i < words.size (); ++i)
    if (words[i] == my && !(i > 0 && words[i - 1] == "DE"))
      {
        jttyAutoCqStop (tr ("%1 decoded").arg (my));
        QApplication::alert (this);
        return;
      }
}
