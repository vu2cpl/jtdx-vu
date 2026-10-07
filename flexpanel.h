#ifndef FLEXPANEL_H
#define FLEXPANEL_H
// JTDX-VU: FlexRadio panel for the VITA-49 rig type - the radio's meters,
// RX / TX antenna and mode of the slice, transmit power, antenna tuner and the
// M-series front speaker, plus per-band antenna memory (transverter bands).
// Port of the MSHV-Mac Flex panel (flexpanel.cpp, Copyright (C) 2026 Manoj
// Ramawarrier VU2CPL); data and commands go through FlexShared.
#include <QDialog>
#include <QMap>
#include <functional>
class QLabel;
class QComboBox;
class QCheckBox;
class QSpinBox;
class QGroupBox;
class QPushButton;
class QTimer;
class QSettings;

class FlexPanel : public QDialog
{
  Q_OBJECT
public:
  // band_of: the band name of a frequency in Hz, empty outside the bands
  FlexPanel (QSettings * settings, std::function<QString (qint64)> band_of, QWidget * parent = nullptr);

private:
  void refresh ();
  void fill_combo (QComboBox * box, QStringList const& items, QString const& current);
  void fill_spin (QSpinBox * box, int value, int & shown);
  void send_if_edited (QSpinBox * box, int & shown, int radio_value, char const * key);
  QString band_ant (QString const& band, bool tx) const;
  void save_band_ants ();

  QSettings * settings_;
  std::function<QString (qint64)> band_of_;
  QLabel * l_conn_, * l_fwd_, * l_ref_, * l_swr_, * l_alc_, * l_patemp_, * l_volts_, * l_model_, * l_atu_;
  QComboBox * cb_rxant_, * cb_txant_, * cb_mode_;
  QCheckBox * cb_localmute_, * cb_hwalc_, * cb_atu_mem_;
  QSpinBox * sb_rfpower_, * sb_tunepower_, * sb_maxpower_;
  QGroupBox * gb_atu_;
  QPushButton * pb_atu_tune_, * pb_atu_bypass_;
  QTimer * timer_;
  bool filling_ {false};
  int shown_rf_ {-1}, shown_tune_ {-1}, shown_max_ {-1};   // what WE last displayed
  bool was_up_ {false};
  // per-band antennas: band name -> "rxant:txant".  A transverter band makes
  // the radio move the TX antenna and leave RX where it was (MM0CEZ, 2026-09-17),
  // so each band gets its own pair back a moment after the retune.
  QMap<QString, QString> band_ant_;
  QString band_;                     // band the slice is on, empty = none / unknown
  int restore_ {0};                  // refresh ticks left in a restore, 0 = none
  enum {RestoreTicks = 6, SendFirst = 4, SendAgain = 1};
};
#endif
