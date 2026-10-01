// JTDX-VU: JTTY controls page.  See jttypanel.h.

#include "jttypanel.h"
#include "jttysettings.h"

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

QString JttyPanel::defaultMacro (int key)
{
  return JttySettings::defaultMacro (key);
}

JttyPanel::JttyPanel (QSettings * settings, QWidget * parent)
  : QWidget {parent}
  , settings_ {settings}
  , ftol_ {new QComboBox}
  , lowerCase_ {new QCheckBox {tr ("Lower case")}}
  , includeTime_ {new QCheckBox {tr ("Include time")}}
  , entry_ {new QLineEdit}
  , send_ {new QPushButton {tr ("Send message")}}
  , callNext_ {new QLineEdit}
  , serial_ {new QSpinBox}
  , serialLabel_ {new QLabel {tr ("Serial Number")}}
{
  settings_->beginGroup ("JTTY");
  int const ftol = settings_->value ("Ftol", 100).toInt ();
  lowerCase_->setChecked (settings_->value ("LowerCase", false).toBool ());
  includeTime_->setChecked (settings_->value ("IncludeTime", true).toBool ());
  for (int i = 0; i < 8; ++i) macros_[i] = new QLineEdit;
  serial_->setRange (0, 9999);
  serial_->setSpecialValueText (tr ("none"));   // 0: non-contest, %E sends just 599
  serial_->setValue (settings_->value ("SerialNumber", 1).toInt ());
  settings_->endGroup ();
  reloadMacros ();               // the active macro set (Settings > JTTY)

  // same tolerance ladder as WSJT-X's sbFtol_2
  for (int hz : {2, 5, 10, 20, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500})
    ftol_->addItem (QString::number (hz), hz);
  int const idx = ftol_->findData (ftol);
  ftol_->setCurrentIndex (idx >= 0 ? idx : ftol_->findData (100));
  ftol_->setToolTip (tr ("F Tol: decodes within this many Hz of the Rx frequency go to the Rx Frequency pane"));
  for (QWidget * w : {static_cast<QWidget *> (ftol_), static_cast<QWidget *> (lowerCase_),
                      static_cast<QWidget *> (includeTime_), static_cast<QWidget *> (send_)})
    w->setFocusPolicy (Qt::NoFocus);
  entry_->setPlaceholderText (tr ("Press Enter to send and clear the message"));
  entry_->setToolTip (tr ("Press Enter to send and clear the message."));
  entry_->setMaxLength (80);
  {
    // JTDX-VU: a little larger than the other controls - it's where the
    // operator types during a QSO
    auto f = entry_->font ();
    f.setPointSizeF (f.pointSizeF () * 1.15);
    entry_->setFont (f);
    entry_->setMinimumHeight (entry_->sizeHint ().height () + 6);
  }
  send_->setToolTip (tr ("Send and clear the message. Press Enter in the field."));
  callNext_->setToolTip (tr ("Callsign to be worked next (%Q)."));
  callNext_->setMaximumWidth (110);
  serial_->setToolTip (tr ("Serial number of QSO for contest exchange (%N; %E sends 599 %N).\n"
                           "Set it to none (0) for a non-contest QSO: %E then sends just 599."));

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
      b->setToolTip (JttySettings::macroTip (key));
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
  grid->addLayout (sendRow, 5, 0, 1, 4);

  auto contestRow = new QHBoxLayout;
  contestRow->setSpacing (4);
  contestRow->addWidget (new QLabel {tr ("Call next")});
  contestRow->addWidget (callNext_);
  contestRow->addStretch ();
  contestRow->addWidget (serialLabel_);
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
          JttySettings::setActiveMacro (settings_, i + 1, macros_[i]->text ().trimmed ());
          updateSerialVisibility ();
        });
    }
  connect (serial_, QOverload<int>::of (&QSpinBox::valueChanged), this, [this] (int n) {
      settings_->beginGroup ("JTTY");
      settings_->setValue ("SerialNumber", n);
      settings_->endGroup ();
    });
  connect (entry_, &QLineEdit::returnPressed, this, &JttyPanel::submitEntry);
  connect (send_, &QPushButton::clicked, this, &JttyPanel::submitEntry);

  updateSerialVisibility ();

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

void JttyPanel::reloadMacros ()
{
  auto const set = JttySettings::activeSet (settings_);
  for (int i = 0; i < 8; ++i)
    {
      macros_[i]->setText (set.macros[i]);
      macros_[i]->setToolTip (tr ("%1 (set: %2)").arg (JttySettings::macroTip (i + 1), set.name));
    }
  updateSerialVisibility ();
}

void JttyPanel::updateSerialVisibility ()
{
  if (!serial_->parentWidget ()) return;   // still being built; the constructor calls this again
  bool const on = JttySettings::usesSerial (JttySettings::activeSet (settings_));
  serialLabel_->setVisible (on);
  serial_->setVisible (on);
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
