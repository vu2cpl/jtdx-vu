// JTDX-VU: JTTY controls page and the calls-heard list.  See jttypanel.h.

#include "jttypanel.h"

#include <QSettings>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QButtonGroup>
#include <QSpinBox>
#include <QLabel>
#include <QListWidget>
#include <QApplication>
#include <QKeyEvent>

namespace
{
  // short key prefix for a button face: F1, ⇧F1, ⌥F1 (Alt-F1 off the Mac)
  QString shortKey (int key)
  {
    int const bank = (key - 1) / JttySettings::keysPerBank;
    auto const f = QString {"F%1"}.arg ((key - 1) % JttySettings::keysPerBank + 1);
#if defined (Q_OS_MAC)
    QString const prefix[JttySettings::bankCount] = {"", QString {QChar {0x21E7}}, QString {QChar {0x2325}}};
#else
    QString const prefix[JttySettings::bankCount] = {"", "S-", "A-"};
#endif
    return prefix[bank] + f;
  }
}

QString JttyPanel::defaultMacro (int key)
{
  return JttySettings::defaultMacro (key);
}

JttyPanel::JttyPanel (QSettings * settings, QWidget * parent)
  : QWidget {parent}
  , settings_ {settings}
  , setCombo_ {new QComboBox}
  , ftol_ {new QComboBox}
  , lowerCase_ {new QCheckBox {tr ("Lower case")}}
  , includeTime_ {new QCheckBox {tr ("Time")}}
  , entry_ {new QLineEdit}
  , send_ {new QPushButton {tr ("Send")}}
  , queue_ {new QLabel}
  , callNext_ {new QLineEdit}
  , serial_ {new QSpinBox}
  , serialLabel_ {new QLabel {tr ("Serial")}}
{
  settings_->beginGroup ("JTTY");
  int const ftol = settings_->value ("Ftol", 100).toInt ();
  lowerCase_->setChecked (settings_->value ("LowerCase", false).toBool ());
  includeTime_->setChecked (settings_->value ("IncludeTime", true).toBool ());
  bank_ = qBound (0, settings_->value ("Bank", 0).toInt (), JttySettings::bankCount - 1);
  serial_->setRange (0, 9999);
  serial_->setSpecialValueText (tr ("none"));   // 0: non-contest, %E sends just the RST
  serial_->setValue (settings_->value ("SerialNumber", 1).toInt ());
  settings_->endGroup ();

  // same tolerance ladder as WSJT-X's sbFtol_2
  for (int hz : {2, 5, 10, 20, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500})
    ftol_->addItem (QString::number (hz), hz);
  int const idx = ftol_->findData (ftol);
  ftol_->setCurrentIndex (idx >= 0 ? idx : ftol_->findData (100));
  ftol_->setToolTip (tr ("F Tol: decodes within this many Hz of the Rx frequency go to the Rx Frequency pane"));
  includeTime_->setToolTip (tr ("Start each decoded line with its UTC time."));
  setCombo_->setToolTip (tr ("Macro set the buttons and keys use. Edit sets in Settings > JTTY."));
  setCombo_->setSizeAdjustPolicy (QComboBox::AdjustToContents);
  for (QWidget * w : {static_cast<QWidget *> (ftol_), static_cast<QWidget *> (lowerCase_),
                      static_cast<QWidget *> (includeTime_), static_cast<QWidget *> (send_),
                      static_cast<QWidget *> (setCombo_)})
    w->setFocusPolicy (Qt::NoFocus);
  entry_->setPlaceholderText (tr ("Type a message, Enter sends it (queued while transmitting)"));
  entry_->setToolTip (tr ("Press Enter to send and clear the message. While a message is going out,\n"
                          "Enter (and the macro keys) queue the next one; Halt or Esc clears the queue."));
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
  serial_->setToolTip (tr ("Serial number of QSO for contest exchange (%N).\n"
                           "Set it to none (0) for a non-contest QSO: %N is then left out of %E."));
  queue_->setToolTip (tr ("Queued: goes out as soon as the current message ends. Halt or Esc clears it."));
  queue_->setTextFormat (Qt::PlainText);

  auto grid = new QGridLayout {this};
  grid->setContentsMargins (2, 2, 2, 2);
  grid->setHorizontalSpacing (4);
  grid->setVerticalSpacing (3);

  auto options = new QHBoxLayout;
  options->setSpacing (4);
  options->addWidget (new QLabel {tr ("Set")});
  options->addWidget (setCombo_);
  options->addSpacing (6);
  auto banks = new QButtonGroup {this};
  for (int b = 0; b < JttySettings::bankCount; ++b)
    {
      auto button = new QPushButton {tr ("Bank %1").arg (b + 1)};
      button->setCheckable (true);
      button->setFocusPolicy (Qt::NoFocus);
      // the window's style sheet gives checked buttons no look of their own
      button->setStyleSheet ("QPushButton:checked{background-color:rgb(64,130,0);color:white;}");
      button->setToolTip (tr ("Show bank %1 (%2-%3). Its keys work whichever bank is shown.")
                          .arg (b + 1).arg (JttySettings::keyName (b * JttySettings::keysPerBank + 1))
                          .arg (QString {"F%1"}.arg (JttySettings::keysPerBank)));
      banks->addButton (button, b);
      bankButtons_[b] = button;
      options->addWidget (button);
    }
  options->addStretch ();
  options->addWidget (new QLabel {tr ("F Tol")});
  options->addWidget (ftol_);
  options->addWidget (lowerCase_);
  options->addWidget (includeTime_);
  grid->addLayout (options, 0, 0, 1, 4);

  for (int i = 0; i < JttySettings::keysPerBank; ++i)
    {
      auto b = new QPushButton;
      b->setFocusPolicy (Qt::NoFocus);
      b->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
      b->setMinimumHeight (b->sizeHint ().height () + 12);
      auto f = b->font ();
      f.setPointSizeF (f.pointSizeF () * 1.05);
      b->setFont (f);
      connect (b, &QPushButton::clicked, this, [this, i] {
          Q_EMIT macroRequested (bank_ * JttySettings::keysPerBank + i + 1);
        });
      buttons_[i] = b;
      grid->addWidget (b, 1 + i / 4, i % 4);
    }

  auto sendRow = new QHBoxLayout;
  sendRow->setSpacing (4);
  sendRow->addWidget (send_);
  sendRow->addWidget (entry_, 1);
  grid->addLayout (sendRow, 3, 0, 1, 4);

  auto contestRow = new QHBoxLayout;
  contestRow->setSpacing (4);
  contestRow->addWidget (new QLabel {tr ("Call next")});
  contestRow->addWidget (callNext_);
  contestRow->addSpacing (8);
  contestRow->addWidget (queue_, 1);
  contestRow->addWidget (serialLabel_);
  contestRow->addWidget (serial_);
  grid->addLayout (contestRow, 4, 0, 1, 4);
  for (int c = 0; c < 4; ++c) grid->setColumnStretch (c, 1);
  grid->setRowStretch (5, 1);      // spare height below, not between the rows

  connect (ftol_, QOverload<int>::of (&QComboBox::currentIndexChanged), this, [this] (int) {
      settings_->beginGroup ("JTTY");
      settings_->setValue ("Ftol", this->ftol ());
      settings_->endGroup ();
      Q_EMIT ftolChanged (this->ftol ());
    });
  auto option = [this] (QCheckBox * box, char const * key) {
    connect (box, &QCheckBox::toggled, this, [this, key] (bool on) {
        settings_->beginGroup ("JTTY");
        settings_->setValue (key, on);
        settings_->endGroup ();
        Q_EMIT displayOptionsChanged ();
      });
  };
  option (lowerCase_, "LowerCase");
  option (includeTime_, "IncludeTime");
  connect (banks, QOverload<int>::of (&QButtonGroup::buttonClicked), this, [this] (int b) {showBank (b);});
  connect (setCombo_, QOverload<int>::of (&QComboBox::activated), this, [this] (int index) {
      JttySettings::setActiveSetName (settings_, setCombo_->itemText (index));
      reloadMacros ();
    });
  connect (serial_, QOverload<int>::of (&QSpinBox::valueChanged), this, [this] (int n) {
      settings_->beginGroup ("JTTY");
      settings_->setValue ("SerialNumber", n);
      settings_->endGroup ();
    });
  connect (entry_, &QLineEdit::returnPressed, this, &JttyPanel::submitEntry);
  connect (send_, &QPushButton::clicked, this, &JttyPanel::submitEntry);

  reloadMacros ();               // the active macro set (Settings > JTTY)
  setQueue ({});

  // F1-F8 (and with Shift / Option) and Esc ahead of the menu shortcuts
  // (F1 help, F2 settings, ...)
  qApp->installEventFilter (this);
}

bool JttyPanel::eventFilter (QObject * watched, QEvent * event)
{
  if ((event->type () == QEvent::ShortcutOverride || event->type () == QEvent::KeyPress)
      && isVisible () && QApplication::activeWindow () == window ())
    {
      auto ke = static_cast<QKeyEvent *> (event);
      int const key = ke->key ();
      auto const mods = ke->modifiers () & ~Qt::KeypadModifier;
      int bank = -1;
      if (mods == Qt::NoModifier) bank = 0;
      else if (mods == Qt::ShiftModifier) bank = 1;
      else if (mods == Qt::AltModifier) bank = 2;     // Option on the Mac
      bool const fkey = key >= Qt::Key_F1 && key <= Qt::Key_F8 && bank >= 0;
      bool const esc = key == Qt::Key_Escape && mods == Qt::NoModifier;
      if (fkey || esc)
        {
          if (event->type () == QEvent::ShortcutOverride)
            {
              event->accept ();       // deliver as a plain key press, not a shortcut
              return true;
            }
          if (fkey) Q_EMIT macroRequested (bank * JttySettings::keysPerBank + key - Qt::Key_F1 + 1);
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
  set_ = JttySettings::activeSet (settings_);
  {
    QSignalBlocker block {setCombo_};
    setCombo_->clear ();
    for (auto const& s : JttySettings::readSets (settings_)) setCombo_->addItem (s.name);
    setCombo_->setCurrentText (set_.name);
  }
  showBank (bank_);
  updateSerialVisibility ();
}

void JttyPanel::showBank (int bank)
{
  bank_ = qBound (0, bank, JttySettings::bankCount - 1);
  settings_->beginGroup ("JTTY");
  settings_->setValue ("Bank", bank_);
  settings_->endGroup ();
  bankButtons_[bank_]->setChecked (true);
  for (int i = 0; i < JttySettings::keysPerBank; ++i)
    {
      int const key = bank_ * JttySettings::keysPerBank + i + 1;
      auto const tpl = set_.macros.value (key - 1).trimmed ();
      auto * const b = buttons_[i];
      b->setText (shortKey (key) + "  " + (tpl.isEmpty () ? QString {} : JttySettings::buttonLabel (set_, key)));
      b->setEnabled (!tpl.isEmpty ());
      b->setToolTip (tpl.isEmpty () ? tr ("%1: empty in set %2 (Settings > JTTY)").arg (JttySettings::keyName (key), set_.name)
                                    : tr ("%1: %2").arg (JttySettings::keyName (key), tpl));
    }
}

void JttyPanel::setQueue (QStringList const& messages)
{
  if (messages.isEmpty ())
    {
      queue_->clear ();
      return;
    }
  auto const first = messages.front ();
  auto text = tr ("Next: %1").arg (first.size () > 28 ? first.left (27) + QChar {0x2026} : first);
  if (messages.size () > 1) text += tr ("  (+%1)").arg (messages.size () - 1);
  queue_->setText (text);
}

void JttyPanel::updateSerialVisibility ()
{
  bool const on = JttySettings::usesSerial (set_);
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
  return (key >= 1 && key <= JttySettings::macroCount) ? set_.macros.value (key - 1).trimmed () : QString {};
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

// ---- calls heard -------------------------------------------------------------

JttyHeardList::JttyHeardList (QWidget * parent)
  : QWidget {parent}
  , list_ {new QListWidget}
  , title_ {new QLabel {tr ("Calls heard")}}
{
  list_->setViewMode (QListView::IconMode);
  list_->setFlow (QListView::LeftToRight);
  list_->setWrapping (true);
  list_->setResizeMode (QListView::Adjust);
  list_->setMovement (QListView::Static);
  list_->setUniformItemSizes (true);
  list_->setSpacing (2);
  list_->setFocusPolicy (Qt::NoFocus);
  list_->setSelectionMode (QAbstractItemView::NoSelection);
  list_->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
  list_->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
  {
    QFont f {"Courier New"};
    f.setStyleHint (QFont::Monospace);
    setListFont (f);
  }
  list_->setToolTip (tr ("Calls in JTTY decodes, newest first, coloured as for the log\n"
                         "(new DXCC / band / mode, new call, worked). Click one to make it\n"
                         "the DX call and put the Rx frequency on it."));
  auto clearButton = new QPushButton {tr ("Clear")};
  clearButton->setFocusPolicy (Qt::NoFocus);
  clearButton->setToolTip (tr ("Empty the list (it also starts afresh on a band change)."));
  auto head = new QHBoxLayout;
  head->setContentsMargins (0, 0, 0, 0);
  head->addWidget (title_);
  head->addStretch ();
  head->addWidget (clearButton);
  auto box = new QVBoxLayout {this};
  box->setContentsMargins (0, 2, 0, 2);
  box->setSpacing (1);
  box->addLayout (head);
  box->addWidget (list_);

  connect (clearButton, &QPushButton::clicked, this, &JttyHeardList::clear);
  connect (list_, &QListWidget::itemClicked, this, [this] (QListWidgetItem * item) {
      Q_EMIT picked (item->data (Qt::UserRole).toString (), item->data (Qt::UserRole + 1).toInt ());
    });
}

void JttyHeardList::setListFont (QFont const& font)
{
  list_->setFont (font);
  QFontMetrics const m {font};
  int const rowHeight = m.height () + 8;
  list_->setGridSize (QSize {m.horizontalAdvance ("WWWWWWWW 0000") + 14, rowHeight});
  list_->setFixedHeight (2 * rowHeight + 6);
  for (int i = 0; i < list_->count (); ++i) list_->item (i)->setFont (font);
}

QStringList JttyHeardList::calls () const
{
  QStringList calls;
  for (int i = 0; i < list_->count (); ++i) calls << list_->item (i)->data (Qt::UserRole).toString ();
  return calls;
}

void JttyHeardList::heard (Entry const& e)
{
  for (int i = 0; i < list_->count (); ++i)
    {
      auto * item = list_->item (i);
      if (item->data (Qt::UserRole).toString () == e.call)
        {
          // in place: a call doesn't jump about while the mouse is on its way
          Entry updated {e};
          updated.count = item->data (Qt::UserRole + 2).toInt () + 1;
          showItem (item, updated);
          return;
        }
    }
  auto * item = new QListWidgetItem;
  Entry fresh {e};
  fresh.count = 1;
  showItem (item, fresh);
  list_->insertItem (0, item);
  while (list_->count () > maxEntries_) delete list_->takeItem (list_->count () - 1);
  title_->setText (tr ("Calls heard (%1)").arg (list_->count ()));
}

void JttyHeardList::restyle (Entry const& e)
{
  for (int i = 0; i < list_->count (); ++i)
    {
      auto * item = list_->item (i);
      if (item->data (Qt::UserRole).toString () != e.call) continue;
      Entry updated {e};
      updated.frequency = item->data (Qt::UserRole + 1).toInt ();
      updated.count = item->data (Qt::UserRole + 2).toInt ();
      updated.lastHeard = item->data (Qt::UserRole + 3).toDateTime ();
      showItem (item, updated);
    }
}

void JttyHeardList::showItem (QListWidgetItem * item, Entry const& e)
{
  item->setData (Qt::UserRole, e.call);
  item->setData (Qt::UserRole + 1, e.frequency);
  item->setData (Qt::UserRole + 2, e.count);
  item->setData (Qt::UserRole + 3, e.lastHeard);
  item->setText (QString {"%1 %2"}.arg (e.call, -8).arg (e.frequency, 4));
  item->setTextAlignment (Qt::AlignCenter);
  item->setFont (list_->font ());     // the window's style sheet would win over the list's font
  if (e.background.isValid ())
    {
      item->setBackground (e.background);
      // dark or light text, whichever reads on the colour
      item->setForeground (e.background.lightness () > 140 ? QColor {Qt::black} : QColor {Qt::white});
    }
  else
    {
      item->setBackground (QBrush {});
      item->setForeground (QBrush {});
    }
  QStringList tip {e.call};
  if (!e.country.isEmpty ()) tip << e.country;
  if (!e.status.isEmpty ()) tip << e.status;
  tip << tr ("%1 Hz, heard %2x, last %3 UTC").arg (e.frequency).arg (e.count)
           .arg (e.lastHeard.isValid () ? e.lastHeard.toString ("hh:mm:ss") : QString {"-"});
  item->setToolTip (tip.join ('\n'));
}

void JttyHeardList::clear ()
{
  list_->clear ();
  title_->setText (tr ("Calls heard"));
}
