// JTDX-VU: the JTTY page of the Settings dialog, and the [JTTY] settings it
// shares with the JTTY controls panel.
//
//   Auto CQ   - which F-key macro to repeat, the gap after each call, an
//               optional limit on the number of calls, and whether a decode
//               containing my call stops it
//   Macro sets - named sets of the eight F-key macros and the exchange %E
//               expands to (%N is dropped when Serial Number is "none");
//               the panel shows and edits the active set, and shows Serial
//               Number only when that set uses %N
//
// Storage, group [JTTY]:
//   AutoCqKey, AutoCqGap, AutoCqMax, AutoCqStopOnMyCall, ActiveMacroSet,
//   OpName, Qth, Radio, Antenna (%OP %QTH %TX %ANT),
//   and the array MacroSets/<n>/{name, Msg1..Msg8, exchange}.  A set saved
//   before sets carried an exchange takes the old global Exchange value
//   (built-in sets take their own).
//   Before macro sets existed the panel kept Msg1..Msg8 directly in [JTTY];
//   those become the "Default" set the first time the sets are read.

#ifndef JTTYSETTINGS_H
#define JTTYSETTINGS_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QVector>

class QSettings;
class QComboBox;
class QSpinBox;
class QCheckBox;
class QLineEdit;
class QPushButton;

namespace JttySettings
{
  struct MacroSet
  {
    QString name;
    QStringList macros;         // always 8 entries, F1..F8
    QString exchange;           // what %E sends with this set
  };
  bool usesSerial (MacroSet const&);                   // %N in the exchange or a macro

  QString defaultMacro (int key);                      // WSJT-X 3.2's native templates
  QString macroTip (int key);                          // what each native key is for
  QVector<MacroSet> builtInSets ();
  QVector<MacroSet> readSets (QSettings *);            // migrates Msg1..8 on first use
  void writeSets (QSettings *, QVector<MacroSet> const&);
  QString activeSetName (QSettings *);
  void setActiveSetName (QSettings *, QString const&);
  MacroSet activeSet (QSettings *);
  void setActiveMacro (QSettings *, int key, QString const& text);   // panel edits

  int autoCqKey (QSettings *);          // 1..8, default 1
  int autoCqGap (QSettings *);          // seconds, default 10
  int autoCqMax (QSettings *);          // 0 = no limit
  bool autoCqStopOnMyCall (QSettings *);
  QString exchange (QSettings *);       // the active set's exchange

  // station details for macros: %OP name, %QTH location, %TX radio, %ANT antenna
  struct StationVar {char const * token; char const * key; char const * label;};
  extern StationVar const stationVars[4];
  QString stationValue (QSettings *, char const * key);
}

class JttySettingsPage final
  : public QWidget
{
  Q_OBJECT

public:
  explicit JttySettingsPage (QWidget * parent = nullptr);

  void load (QSettings *);   // from the stored settings (each time Settings opens)
  void save (QSettings *);   // on OK

private:
  void showSet (int index);
  void keepEdits ();          // copy the line edits back into sets_[current_]
  void refreshSetCombo ();

  QComboBox * autoCqKey_;
  QSpinBox * autoCqGap_;
  QSpinBox * autoCqMax_;
  QCheckBox * stopOnMyCall_;
  QLineEdit * exchange_;      // the shown set's exchange
  QLineEdit * station_[4];    // %OP %QTH %TX %ANT
  QComboBox * setCombo_;
  QPushButton * newSet_;
  QPushButton * renameSet_;
  QPushButton * deleteSet_;
  QPushButton * resetSet_;
  QLineEdit * macros_[8];
  QVector<JttySettings::MacroSet> sets_;
  int current_ {-1};
};

#endif
