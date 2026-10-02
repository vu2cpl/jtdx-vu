// JTDX-VU: the JTTY page of the Settings dialog, and the [JTTY] settings it
// shares with the JTTY controls panel.
//
//   Auto CQ   - which F-key macro to repeat, the gap after each call, an
//               optional limit on the number of calls, and whether a decode
//               containing my call stops it
//   Macro sets - named sets of 24 macros in three banks (F1-F8,
//               Shift+F1-F8, Option/Alt+F1-F8), each with a short label for
//               its panel button, and the exchange %E expands to (%N is
//               dropped when Serial Number is "none"); the panel shows the
//               active set as buttons, and shows Serial Number only when
//               that set uses %N
//
// Storage, group [JTTY]:
//   AutoCqKey, AutoCqGap, AutoCqMax, AutoCqMinutes, AutoCqStopOnMyCall, ActiveMacroSet,
//   OpName, Qth, Radio, Antenna (%OP %QTH %TX %ANT),
//   and the array MacroSets/<n>/{name, Msg1..Msg24, Label1..Label24,
//   exchange}.  Banks=3 marks the one-off migration that gave the built-in
//   sets their bank 2-3 macros, labels and the %RST exchange.  A set saved
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
    QStringList macros;         // always macroCount entries: F1..F8, Shift+F1..F8, Opt+F1..F8
    QStringList labels;         // the same count; empty = derived from the macro
    QString exchange;           // what %E sends with this set
  };
  int const keysPerBank = 8;
  int const bankCount = 3;
  int const macroCount = keysPerBank * bankCount;
  QString keyName (int key);                           // 1..24 -> "F1", "Shift+F1", "Opt+F1"
  QString buttonLabel (MacroSet const&, int key);      // the label, or a short form of the macro
  bool usesSerial (MacroSet const&);                   // %N in the exchange or a macro

  QString defaultMacro (int key);                      // WSJT-X 3.2's native templates (1..8)
  QString macroTip (int key);                          // what each native key is for (1..8)
  QVector<MacroSet> builtInSets ();
  QVector<MacroSet> readSets (QSettings *);            // migrates Msg1..8 on first use
  void writeSets (QSettings *, QVector<MacroSet> const&);
  QString activeSetName (QSettings *);
  void setActiveSetName (QSettings *, QString const&);
  MacroSet activeSet (QSettings *);

  int autoCqKey (QSettings *);          // 1..24, default 1
  int autoCqGap (QSettings *);          // seconds, default 10
  int autoCqMax (QSettings *);          // 0 = no limit
  int autoCqMinutes (QSettings *);      // 1..60, default 5: JTTY Auto CQ time limit, and the
                                        // Tx watchdog while Call Non-Stop is on in the FT modes
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
  QSpinBox * autoCqMinutes_;
  QCheckBox * stopOnMyCall_;
  QLineEdit * exchange_;      // the shown set's exchange
  QLineEdit * station_[4];    // %OP %QTH %TX %ANT
  QComboBox * setCombo_;
  QPushButton * newSet_;
  QPushButton * renameSet_;
  QPushButton * deleteSet_;
  QPushButton * resetSet_;
  QLineEdit * macros_[JttySettings::macroCount];
  QLineEdit * labels_[JttySettings::macroCount];
  QVector<JttySettings::MacroSet> sets_;
  int current_ {-1};
};

#endif
