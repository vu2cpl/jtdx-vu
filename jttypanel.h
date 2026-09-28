// JTDX-VU: control row for JTTY mode (WSJT-X 3.2's period-free RTTY-style
// keyboard mode).  Shown only while JTTY is selected, between the band/mode
// switcher and the decode panes.  Phase 2 (RX) controls; the text entry and
// F1-F8 macros arrive with TX.

#ifndef JTTYPANEL_H
#define JTTYPANEL_H

#include <QWidget>

class QComboBox;
class QCheckBox;
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

  Q_SIGNAL void ftolChanged (int hz) const;
  Q_SIGNAL void displayOptionsChanged () const;

private:
  QSettings * settings_;
  QComboBox * ftol_;
  QCheckBox * lowerCase_;
  QCheckBox * includeTime_;
};

#endif
