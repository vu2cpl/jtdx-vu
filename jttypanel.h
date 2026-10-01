// JTDX-VU: controls for JTTY mode (WSJT-X 3.2's period-free RTTY-style
// keyboard mode), laid out like WSJT-X 3.2.0-rc1's JTTY page.  Lives as an
// extra page of the right-hand controls stack, shown in place of the FT8
// Tx-message tabs while JTTY is selected.
//
//   row 0: F Tol, Lower case, Include time
//   rows 1-4: F1-F4 / their editable macros / F5-F8 / their macros
//   row 5: Send message, message entry, Halt
//   row 6: Call next (%Q), Serial Number (%N)
//
// The panel knows nothing about calls or the radio: it emits what the
// operator asked for and MainWindow expands %M/%H/%Q/%N/%E and transmits.
// While it is visible and its window is active, it takes F1-F8 and Esc
// ahead of the menu shortcuts.

#ifndef JTTYPANEL_H
#define JTTYPANEL_H

#include <QWidget>
#include <QString>

class QComboBox;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QSettings;
class QEvent;
class QLabel;

class JttyPanel final
  : public QWidget
{
  Q_OBJECT

public:
  explicit JttyPanel (QSettings * settings, QWidget * parent = nullptr);

  int ftol () const;               // Hz, half-width of the Rx-frequency window
  bool lowerCase () const;         // render decodes (and send text) in lower case
  bool includeTime () const;       // prefix each line with its UTC start time
  QString macro (int key) const;   // key 1..8, the editable template
  QString callNext () const;       // %Q
  int serialNumber () const;       // %N
  void setSerialNumber (int);

  // WSJT-X 3.2's native templates, key 1..8
  static QString defaultMacro (int key);

  void reloadMacros ();            // the active macro set changed in Settings

  Q_SIGNAL void ftolChanged (int hz) const;
  Q_SIGNAL void displayOptionsChanged () const;
  Q_SIGNAL void transmitRequested (QString const& text) const;   // free text from the entry
  Q_SIGNAL void macroRequested (int key) const;                  // F1..F8
  Q_SIGNAL void haltRequested () const;

protected:
  bool eventFilter (QObject *, QEvent *) override;

private:
  void submitEntry ();

  QSettings * settings_;
  QComboBox * ftol_;
  QCheckBox * lowerCase_;
  QCheckBox * includeTime_;
  QLineEdit * macros_[8];
  QLineEdit * entry_;
  QPushButton * send_;
  QLineEdit * callNext_;
  QSpinBox * serial_;
  QLabel * serialLabel_;
  void updateSerialVisibility ();   // shown only when the active set uses %N
};

#endif
