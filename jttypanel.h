// JTDX-VU: control rows for JTTY mode (WSJT-X 3.2's period-free RTTY-style
// keyboard mode).  Shown only while JTTY is selected, between the band/mode
// switcher and the decode panes.
//
//   row 1: F Tol, display options, contest exchange
//   row 2: F1-F8 macro buttons (WSJT-X's native templates)
//   row 3: message entry, Send, Halt
//
// The panel knows nothing about calls or the radio: it emits what the
// operator asked for and MainWindow expands %M/%H/%E and transmits.

#ifndef JTTYPANEL_H
#define JTTYPANEL_H

#include <QWidget>
#include <QString>

class QComboBox;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QSettings;

class JttyPanel final
  : public QWidget
{
  Q_OBJECT

public:
  explicit JttyPanel (QSettings * settings, QWidget * parent = nullptr);

  int ftol () const;               // Hz, half-width of the Rx-frequency window
  bool lowerCase () const;         // render decodes in lower case
  bool includeTime () const;       // prefix each line with its UTC start time
  QString exchange () const;       // %E, e.g. "599 001"

  // WSJT-X's default native templates, key 1..8; %M my call, %H his call,
  // %E exchange, %Q queued call (treated as %H here)
  static QString macroTemplate (int key);

  void setTransmitting (bool on);  // Send/F-keys stay enabled (chaining later); Halt reflects state

  Q_SIGNAL void ftolChanged (int hz) const;
  Q_SIGNAL void displayOptionsChanged () const;
  Q_SIGNAL void transmitRequested (QString const& text) const;   // free text from the entry
  Q_SIGNAL void macroRequested (int key) const;                  // F1..F8
  Q_SIGNAL void haltRequested () const;

private:
  void submitEntry ();

  QSettings * settings_;
  QComboBox * ftol_;
  QCheckBox * lowerCase_;
  QCheckBox * includeTime_;
  QLineEdit * exchange_;
  QLineEdit * entry_;
  QPushButton * send_;
  QPushButton * halt_;
};

#endif
