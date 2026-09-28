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
#include <QTimer>
#include <QAction>

#include "commons.h"
#include "jttypanel.h"
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
  ui->RxFreqSpinBox->setValue (1500);         // JTTY convention, as WSJT-X
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
  QTextCursor cursor = ui->decodedTextBrowser->textCursor ();
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
  ui->decodedTextBrowser->setTextCursor (cursor);
  ui->decodedTextBrowser->ensureCursorVisible ();
}

void MainWindow::renderJttyQsoLines ()
{
  if (m_jttyQsoLines.isEmpty ()) return;
  QTextCursor cursor = ui->decodedTextBrowser2->textCursor ();
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
  ui->decodedTextBrowser2->setTextCursor (cursor);
  ui->decodedTextBrowser2->ensureCursorVisible ();
}
