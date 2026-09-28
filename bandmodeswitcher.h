// JTDX-VU (VU2CPL): one compact row of one-click mode and band buttons,
// plus the "show only new" filter, above the decode panes, after the MSHV "Band / Mode Switcher Buttons" (by LZ2HV).
// The rows, and which bands/modes they show, are chosen in
// View > Band & Mode Buttons... and kept in the [Switcher] settings group.

#ifndef BANDMODESWITCHER_H
#define BANDMODESWITCHER_H

#include <QWidget>
#include <QStringList>
#include <QMap>

class QSettings;
class QHBoxLayout;
class QPushButton;
class QComboBox;

class BandModeSwitcher final
  : public QWidget
{
  Q_OBJECT

public:
  BandModeSwitcher (QSettings * settings, bool dark, QWidget * parent = nullptr);

  static QStringList const& all_modes ();
  static QStringList const& all_bands ();

  void set_active (QString const& band, QString const& mode);  // highlight current
  void settings_dialog (QWidget * parent);

  Q_SIGNAL void band_clicked (QString const& band) const;
  Q_SIGNAL void mode_clicked (QString const& mode) const;
  Q_SIGNAL void new_only_changed (int level) const;  // 0 all, 1 new DXCC, 2 new band, 3 new mode

  int new_only () const {return new_only_;}
  void add_trailing_widget (QWidget *);  // kept at the end of the row across rebuilds

private:
  void rebuild ();
  void restyle ();

  QSettings * settings_;
  bool dark_;
  bool show_bands_ {true};
  bool show_modes_ {true};
  int new_only_ {0};
  QStringList bands_;
  QStringList modes_;
  QString active_band_;
  QString active_mode_;
  QHBoxLayout * row_;
  QComboBox * new_only_combo_;
  QList<QWidget *> trailing_;
  QMap<QString, QPushButton *> band_buttons_;
  QMap<QString, QPushButton *> mode_buttons_;
};

#endif
