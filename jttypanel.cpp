// JTDX-VU: JTTY control rows.  See jttypanel.h.

#include "jttypanel.h"

#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

namespace
{
  // WSJT-X 3.2 native F-key templates (doc/user_guide jtty.adoc); the
  // button labels are the same shorthand WSJT-X shows
  struct Macro {char const * label; char const * tpl;};
  Macro const macros[8] = {
    {"F1 CQ",       "CQ %M CQ"},
    {"F2 Exch",     "%H %E"},
    {"F3 TU CQ",    "%H TU CQ %M CQ"},
    {"F4 My call",  "%M"},
    {"F5 His call", "%H"},
    {"F6 TU NOW",   "TU NOW %Q %E"},
    {"F7 AGN?",     "%H AGN?"},
    {"F8 Exch only","%E"},
  };
}

QString JttyPanel::macroTemplate (int key)
{
  return (key >= 1 && key <= 8) ? QString::fromLatin1 (macros[key - 1].tpl) : QString {};
}

JttyPanel::JttyPanel (QSettings * settings, QWidget * parent)
  : QWidget {parent}
  , settings_ {settings}
  , ftol_ {new QComboBox}
  , lowerCase_ {new QCheckBox {tr ("Lower case")}}
  , includeTime_ {new QCheckBox {tr ("Include time")}}
  , exchange_ {new QLineEdit}
  , entry_ {new QLineEdit}
  , send_ {new QPushButton {tr ("Send")}}
  , halt_ {new QPushButton {tr ("Halt")}}
{
  settings_->beginGroup ("JTTY");
  int const ftol = settings_->value ("Ftol", 100).toInt ();
  lowerCase_->setChecked (settings_->value ("LowerCase", false).toBool ());
  includeTime_->setChecked (settings_->value ("IncludeTime", true).toBool ());
  exchange_->setText (settings_->value ("Exchange", "599 001").toString ());
  settings_->endGroup ();

  // same tolerance ladder as WSJT-X's sbFtol_2
  for (int hz : {2, 5, 10, 20, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500})
    ftol_->addItem (QString::number (hz), hz);
  int const idx = ftol_->findData (ftol);
  ftol_->setCurrentIndex (idx >= 0 ? idx : ftol_->findData (100));
  ftol_->setToolTip (tr ("F Tol: decodes within this many Hz of the Rx frequency go to the Rx Frequency pane"));
  for (QWidget * w : {static_cast<QWidget *> (ftol_), static_cast<QWidget *> (lowerCase_),
                      static_cast<QWidget *> (includeTime_), static_cast<QWidget *> (send_),
                      static_cast<QWidget *> (halt_)})
    w->setFocusPolicy (Qt::NoFocus);
  exchange_->setMaximumWidth (110);
  exchange_->setToolTip (tr ("Contest exchange sent for %E, e.g. 599 001 or 599 MA"));
  entry_->setPlaceholderText (tr ("Type a message and press Enter to transmit"));
  entry_->setMaxLength (80);
  send_->setToolTip (tr ("Transmit the message now (Enter)"));
  halt_->setToolTip (tr ("Stop transmitting"));
  halt_->setEnabled (false);

  auto column = new QVBoxLayout {this};
  column->setContentsMargins (0, 0, 0, 0);
  column->setSpacing (2);

  auto row1 = new QHBoxLayout;
  row1->setSpacing (6);
  auto title = new QLabel {tr ("JTTY")};
  title->setStyleSheet ("font-weight: bold;");
  row1->addWidget (title);
  row1->addSpacing (8);
  row1->addWidget (new QLabel {tr ("F Tol")});
  row1->addWidget (ftol_);
  row1->addSpacing (12);
  row1->addWidget (lowerCase_);
  row1->addWidget (includeTime_);
  row1->addStretch ();
  row1->addWidget (new QLabel {tr ("Exch")});
  row1->addWidget (exchange_);
  column->addLayout (row1);

  auto row2 = new QHBoxLayout;
  row2->setSpacing (2);
  for (int key = 1; key <= 8; ++key)
    {
      auto b = new QPushButton {tr (macros[key - 1].label)};
      b->setFocusPolicy (Qt::NoFocus);
      b->setToolTip (QString::fromLatin1 (macros[key - 1].tpl));
      b->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
      connect (b, &QPushButton::clicked, this, [this, key] {Q_EMIT macroRequested (key);});
      row2->addWidget (b);
    }
  column->addLayout (row2);

  auto row3 = new QHBoxLayout;
  row3->setSpacing (4);
  row3->addWidget (entry_, 1);
  row3->addWidget (send_);
  row3->addWidget (halt_);
  column->addLayout (row3);

  connect (ftol_, QOverload<int>::of (&QComboBox::currentIndexChanged), this, [this] (int) {
      settings_->beginGroup ("JTTY");
      settings_->setValue ("Ftol", this->ftol ());
      settings_->endGroup ();
      Q_EMIT ftolChanged (this->ftol ());
    });
  auto option = [this] (QCheckBox * box, char const * key) {
    connect (box, &QCheckBox::toggled, this, [this, box, key] (bool on) {
        settings_->beginGroup ("JTTY");
        settings_->setValue (key, on);
        settings_->endGroup ();
        Q_EMIT displayOptionsChanged ();
      });
  };
  option (lowerCase_, "LowerCase");
  option (includeTime_, "IncludeTime");
  connect (exchange_, &QLineEdit::editingFinished, this, [this] {
      settings_->beginGroup ("JTTY");
      settings_->setValue ("Exchange", exchange_->text ().trimmed ());
      settings_->endGroup ();
    });
  connect (entry_, &QLineEdit::returnPressed, this, &JttyPanel::submitEntry);
  connect (send_, &QPushButton::clicked, this, &JttyPanel::submitEntry);
  connect (halt_, &QPushButton::clicked, this, [this] {Q_EMIT haltRequested ();});
}

void JttyPanel::submitEntry ()
{
  auto const text = entry_->text ().trimmed ();
  if (text.isEmpty ()) return;
  entry_->clear ();
  Q_EMIT transmitRequested (text);
}

void JttyPanel::setTransmitting (bool on)
{
  halt_->setEnabled (on);
}

int JttyPanel::ftol () const
{
  return ftol_->currentData ().toInt ();
}

bool JttyPanel::lowerCase () const
{
  return lowerCase_->isChecked ();
}

bool JttyPanel::includeTime () const
{
  return includeTime_->isChecked ();
}

QString JttyPanel::exchange () const
{
  return exchange_->text ().trimmed ();
}
