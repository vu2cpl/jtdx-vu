// JTDX-VU: JTTY controls page.  See jttypanel.h.

#include "jttypanel.h"

#include <QSettings>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QApplication>
#include <QKeyEvent>

namespace
{
  // WSJT-X 3.2 native F-key templates (doc/user_guide jtty.adoc)
  struct Macro {char const * tip; char const * tpl;};
  Macro const macros[8] = {
    {"CQ",                    "CQ %M CQ"},
    {"HisCall Exchange",      "%H %E"},
    {"TU CQ",                 "%H TU CQ %M CQ"},
    {"MyCall",                "%M"},
    {"HisCall",               "%H"},
    {"Now QueuedCall Exchange","TU NOW %Q %E"},
    {"HisCall AGN ?",         "%H AGN?"},
    {"Exchange",              "%E"},
  };
}

QString JttyPanel::defaultMacro (int key)
{
  return (key >= 1 && key <= 8) ? QString::fromLatin1 (macros[key - 1].tpl) : QString {};
}

JttyPanel::JttyPanel (QSettings * settings, QWidget * parent)
  : QWidget {parent}
  , settings_ {settings}
  , ftol_ {new QComboBox}
  , lowerCase_ {new QCheckBox {tr ("Lower case")}}
  , includeTime_ {new QCheckBox {tr ("Include time")}}
  , entry_ {new QLineEdit}
  , send_ {new QPushButton {tr ("Send message")}}
  , halt_ {new QPushButton {tr ("Halt")}}
  , callNext_ {new QLineEdit}
  , serial_ {new QSpinBox}
{
  settings_->beginGroup ("JTTY");
  int const ftol = settings_->value ("Ftol", 100).toInt ();
  lowerCase_->setChecked (settings_->value ("LowerCase", false).toBool ());
  includeTime_->setChecked (settings_->value ("IncludeTime", true).toBool ());
  for (int i = 0; i < 8; ++i)
    {
      macros_[i] = new QLineEdit {settings_->value (QString {"Msg%1"}.arg (i + 1), defaultMacro (i + 1)).toString ()};
      macros_[i]->setToolTip (tr (macros[i].tip));
    }
  serial_->setRange (1, 9999);
  serial_->setValue (settings_->value ("SerialNumber", 1).toInt ());
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
  entry_->setPlaceholderText (tr ("Press Enter to send and clear the message"));
  entry_->setToolTip (tr ("Press Enter to send and clear the message."));
  entry_->setMaxLength (80);
  send_->setToolTip (tr ("Send and clear the message. Press Enter in the field."));
  halt_->setToolTip (tr ("Stop transmitting (Esc)"));
  halt_->setEnabled (false);
  callNext_->setToolTip (tr ("Callsign to be worked next (%Q)."));
  callNext_->setMaximumWidth (110);
  serial_->setToolTip (tr ("Serial number of QSO for contest exchange (%N; %E sends 599 %N)."));

  auto grid = new QGridLayout {this};
  grid->setContentsMargins (2, 2, 2, 2);
  grid->setHorizontalSpacing (4);
  grid->setVerticalSpacing (2);

  auto options = new QHBoxLayout;
  options->setSpacing (6);
  options->addWidget (new QLabel {tr ("F Tol")});
  options->addWidget (ftol_);
  options->addSpacing (8);
  options->addWidget (lowerCase_);
  options->addWidget (includeTime_);
  options->addStretch ();
  grid->addLayout (options, 0, 0, 1, 4);

  for (int key = 1; key <= 8; ++key)
    {
      auto b = new QPushButton {QString {"F%1"}.arg (key)};
      b->setFocusPolicy (Qt::NoFocus);
      b->setToolTip (tr (macros[key - 1].tip));
      b->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
      connect (b, &QPushButton::clicked, this, [this, key] {Q_EMIT macroRequested (key);});
      int const row = key <= 4 ? 1 : 3;
      int const col = (key - 1) % 4;
      grid->addWidget (b, row, col);
      grid->addWidget (macros_[key - 1], row + 1, col);
    }

  auto sendRow = new QHBoxLayout;
  sendRow->setSpacing (4);
  sendRow->addWidget (send_);
  sendRow->addWidget (entry_, 1);
  sendRow->addWidget (halt_);
  grid->addLayout (sendRow, 5, 0, 1, 4);

  auto contestRow = new QHBoxLayout;
  contestRow->setSpacing (4);
  contestRow->addWidget (new QLabel {tr ("Call next")});
  contestRow->addWidget (callNext_);
  contestRow->addStretch ();
  contestRow->addWidget (new QLabel {tr ("Serial Number")});
  contestRow->addWidget (serial_);
  grid->addLayout (contestRow, 6, 0, 1, 4);

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
  for (int i = 0; i < 8; ++i)
    {
      connect (macros_[i], &QLineEdit::editingFinished, this, [this, i] {
          settings_->beginGroup ("JTTY");
          settings_->setValue (QString {"Msg%1"}.arg (i + 1), macros_[i]->text ().trimmed ());
          settings_->endGroup ();
        });
    }
  connect (serial_, QOverload<int>::of (&QSpinBox::valueChanged), this, [this] (int n) {
      settings_->beginGroup ("JTTY");
      settings_->setValue ("SerialNumber", n);
      settings_->endGroup ();
    });
  connect (entry_, &QLineEdit::returnPressed, this, &JttyPanel::submitEntry);
  connect (send_, &QPushButton::clicked, this, &JttyPanel::submitEntry);
  connect (halt_, &QPushButton::clicked, this, [this] {Q_EMIT haltRequested ();});

  // F1-F8 and Esc ahead of the menu shortcuts (F1 help, F2 settings, ...)
  qApp->installEventFilter (this);
}

bool JttyPanel::eventFilter (QObject * watched, QEvent * event)
{
  if ((event->type () == QEvent::ShortcutOverride || event->type () == QEvent::KeyPress)
      && isVisible () && QApplication::activeWindow () == window ())
    {
      auto ke = static_cast<QKeyEvent *> (event);
      int const key = ke->key ();
      bool const fkey = key >= Qt::Key_F1 && key <= Qt::Key_F8 && ke->modifiers () == Qt::NoModifier;
      if (fkey || key == Qt::Key_Escape)
        {
          if (event->type () == QEvent::ShortcutOverride)
            {
              event->accept ();       // deliver as a plain key press, not a shortcut
              return true;
            }
          if (fkey) Q_EMIT macroRequested (key - Qt::Key_F1 + 1);
          else Q_EMIT haltRequested ();
          return true;
        }
    }
  return QWidget::eventFilter (watched, event);
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

QString JttyPanel::macro (int key) const
{
  return (key >= 1 && key <= 8) ? macros_[key - 1]->text ().trimmed () : QString {};
}

QString JttyPanel::callNext () const
{
  return callNext_->text ().trimmed ().toUpper ();
}

int JttyPanel::serialNumber () const
{
  return serial_->value ();
}

void JttyPanel::setSerialNumber (int n)
{
  serial_->setValue (n);
}
