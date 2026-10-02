// JTDX-VU: controls for JTTY mode (WSJT-X 3.2's period-free RTTY-style
// keyboard mode).  Lives as an extra page of the right-hand controls stack,
// shown in place of the FT8 Tx-message tabs while JTTY is selected.
//
//   row 0: macro set, Bank 1/2/3, F Tol, Lower case, Include time
//   rows 1-2: the shown bank's eight macro buttons (label from the set)
//   row 3: Send message, message entry
//   row 4: Call next (%Q), what is queued to send next, Serial Number (%N)
//
// The macros are edited in Settings > JTTY; the panel only shows and sends
// them.  F1-F8 send bank 1, Shift+F1-F8 bank 2 and Option (Alt)+F1-F8
// bank 3, whichever bank is on screen.
//
// The panel knows nothing about calls or the radio: it emits what the
// operator asked for and MainWindow expands the variables and transmits.
// While it is visible and its window is active, it takes the F-keys and Esc
// ahead of the menu shortcuts.

#ifndef JTTYPANEL_H
#define JTTYPANEL_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QColor>
#include <QDateTime>
#include <QVector>

#include "jttysettings.h"

class QComboBox;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QSettings;
class QEvent;
class QLabel;
class QListWidget;
class QListWidgetItem;

class JttyPanel final
  : public QWidget
{
  Q_OBJECT

public:
  explicit JttyPanel (QSettings * settings, QWidget * parent = nullptr);

  int ftol () const;               // Hz, half-width of the Rx-frequency window
  bool lowerCase () const;         // render decodes (and send text) in lower case
  bool includeTime () const;       // prefix each line with its UTC start time
  QString macro (int key) const;   // key 1..24, the template
  QString callNext () const;       // %Q
  int serialNumber () const;       // %N
  void setSerialNumber (int);
  void setQueue (QStringList const& messages);   // waiting to go out after the current one

  // WSJT-X 3.2's native templates, key 1..8
  static QString defaultMacro (int key);

  void reloadMacros ();            // the macro sets changed in Settings

  Q_SIGNAL void ftolChanged (int hz) const;
  Q_SIGNAL void displayOptionsChanged () const;
  Q_SIGNAL void transmitRequested (QString const& text) const;   // free text from the entry
  Q_SIGNAL void macroRequested (int key) const;                  // 1..24
  Q_SIGNAL void haltRequested () const;

protected:
  bool eventFilter (QObject *, QEvent *) override;

private:
  void submitEntry ();
  void showBank (int bank);
  void updateSerialVisibility ();   // shown only when the active set uses %N

  QSettings * settings_;
  JttySettings::MacroSet set_;     // the active set
  int bank_ {0};
  QComboBox * setCombo_;
  QPushButton * bankButtons_[JttySettings::bankCount];
  QComboBox * ftol_;
  QCheckBox * lowerCase_;
  QCheckBox * includeTime_;
  QPushButton * buttons_[JttySettings::keysPerBank];
  QLineEdit * entry_;
  QPushButton * send_;
  QLabel * queue_;
  QLineEdit * callNext_;
  QSpinBox * serial_;
  QLabel * serialLabel_;
};

// JTDX-VU: calls seen in JTTY decodes, newest first, coloured by what they
// would be for the log (new DXCC / band / mode, new call, worked).  A click
// picks the call; MainWindow fills the list and does the colouring.
class JttyHeardList final
  : public QWidget
{
  Q_OBJECT

public:
  struct Entry
  {
    QString call;
    int frequency {0};             // audio Hz
    QDateTime lastHeard;           // UTC
    int count {0};
    QColor background;             // invalid = plain
    QString status;                // "New DXCC", "Worked", ...
    QString country;
  };

  explicit JttyHeardList (QWidget * parent = nullptr);

  void heard (Entry const&);       // add or update in place (new calls go first)
  void clear ();
  void setListFont (QFont const&);  // the decode panes' font
  QStringList calls () const;
  void restyle (Entry const&);     // colour / status changed (log updated)

  Q_SIGNAL void picked (QString const& call, int frequency) const;

private:
  void showItem (QListWidgetItem *, Entry const&);

  QListWidget * list_;
  QLabel * title_;
  int const maxEntries_ {40};
};

#endif
