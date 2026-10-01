// JTDX-VU: the JTTY page of the Settings dialog, and the [JTTY] settings it
// shares with the JTTY controls panel.
//
//   Auto CQ   - which F-key macro to repeat, the gap after each call, an
//               optional limit on the number of calls, and whether a decode
//               containing my call stops it
//   Exchange  - what %E expands to ("599 %N" by default; %N is dropped when
//               the panel's Serial Number is "none")
//   Macro sets - named sets of the eight F-key macros; the panel shows and
//               edits the active one
//
// Storage, group [JTTY]:
//   AutoCqKey, AutoCqGap, AutoCqMax, AutoCqStopOnMyCall, Exchange,
//   ActiveMacroSet, and the array MacroSets/<n>/{name, Msg1..Msg8}.
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
  };

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
  QString exchange (QSettings *);       // default "599 %N"
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
  QLineEdit * exchange_;
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
