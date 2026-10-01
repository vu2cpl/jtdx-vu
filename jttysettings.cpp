// JTDX-VU: JTTY page of the Settings dialog.  See jttysettings.h.

#include "jttysettings.h"

#include <QSettings>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QInputDialog>
#include <QMessageBox>

namespace
{
  // WSJT-X 3.2 native F-key templates (doc/user_guide jtty.adoc)
  struct Macro {char const * tip; char const * tpl;};
  Macro const native[8] = {
    {"CQ",                     "CQ %M CQ"},
    {"HisCall Exchange",       "%H %E"},
    {"TU CQ",                  "%H TU CQ %M CQ"},
    {"MyCall",                 "%M"},
    {"HisCall",                "%H"},
    {"Now QueuedCall Exchange","TU NOW %Q %E"},
    {"HisCall AGN ?",          "%H AGN?"},
    {"Exchange",               "%E"},
  };
  // a conversational set for everyday (non-contest) QSOs
  char const * const ragchew[8] = {
    "CQ CQ CQ DE %M %M %M K",
    "%H DE %M TNX FER CALL UR %E %E %H DE %M K",
    "%H DE %M TU 73 %H DE %M SK",
    "%M",
    "%H",
    "%H DE %M K",
    "%H AGN? DE %M K",
    "QRZ? DE %M K",
  };
  QString const group {"JTTY"};
  QString const contestName {QT_TRANSLATE_NOOP ("JttySettings", "Contest (WSJT-X)")};
  QString const ragchewName {QT_TRANSLATE_NOOP ("JttySettings", "Ragchew / DX")};

  struct GroupGuard
  {
    explicit GroupGuard (QSettings * s) : s_ {s} {s_->beginGroup (group);}
    ~GroupGuard () {s_->endGroup ();}
    QSettings * s_;
  };

  QStringList normalised (QStringList list)
  {
    while (list.size () < 8) list << QString {};
    return list.mid (0, 8);
  }
}

namespace JttySettings
{
  QString defaultMacro (int key)
  {
    return (key >= 1 && key <= 8) ? QString::fromLatin1 (native[key - 1].tpl) : QString {};
  }

  QString macroTip (int key)
  {
    return (key >= 1 && key <= 8) ? QString::fromLatin1 (native[key - 1].tip) : QString {};
  }

  QVector<MacroSet> builtInSets ()
  {
    MacroSet contest {contestName, {}};
    MacroSet chat {ragchewName, {}};
    for (int key = 1; key <= 8; ++key)
      {
        contest.macros << defaultMacro (key);
        chat.macros << QString::fromLatin1 (ragchew[key - 1]);
      }
    return {contest, chat};
  }

  QVector<MacroSet> readSets (QSettings * s)
  {
    QVector<MacroSet> sets;
    {
      GroupGuard g {s};
      int const n = s->beginReadArray ("MacroSets");
      for (int i = 0; i < n; ++i)
        {
          s->setArrayIndex (i);
          MacroSet set;
          set.name = s->value ("name").toString ().trimmed ();
          for (int key = 1; key <= 8; ++key)
            set.macros << s->value (QString {"Msg%1"}.arg (key)).toString ();
          if (!set.name.isEmpty ()) sets << set;
        }
      s->endArray ();
    }
    if (sets.isEmpty ())
      {
        // first use: the panel's old single set becomes "Default", the
        // built-in sets follow
        MacroSet old {QObject::tr ("Default"), {}};
        {
          GroupGuard g {s};
          for (int key = 1; key <= 8; ++key)
            old.macros << s->value (QString {"Msg%1"}.arg (key), defaultMacro (key)).toString ();
        }
        sets << old << builtInSets ();
        writeSets (s, sets);
        setActiveSetName (s, old.name);
      }
    return sets;
  }

  void writeSets (QSettings * s, QVector<MacroSet> const& sets)
  {
    GroupGuard g {s};
    s->remove ("MacroSets");
    s->beginWriteArray ("MacroSets", sets.size ());
    for (int i = 0; i < sets.size (); ++i)
      {
        s->setArrayIndex (i);
        s->setValue ("name", sets[i].name);
        auto const macros = normalised (sets[i].macros);
        for (int key = 1; key <= 8; ++key)
          s->setValue (QString {"Msg%1"}.arg (key), macros[key - 1]);
      }
    s->endArray ();
  }

  QString activeSetName (QSettings * s)
  {
    GroupGuard g {s};
    return s->value ("ActiveMacroSet").toString ();
  }

  void setActiveSetName (QSettings * s, QString const& name)
  {
    GroupGuard g {s};
    s->setValue ("ActiveMacroSet", name);
  }

  MacroSet activeSet (QSettings * s)
  {
    auto const sets = readSets (s);
    auto const name = activeSetName (s);
    for (auto const& set : sets)
      if (set.name == name) return {set.name, normalised (set.macros)};
    return {sets.front ().name, normalised (sets.front ().macros)};
  }

  void setActiveMacro (QSettings * s, int key, QString const& text)
  {
    if (key < 1 || key > 8) return;
    auto sets = readSets (s);
    auto const name = activeSet (s).name;
    for (auto& set : sets)
      if (set.name == name)
        {
          set.macros = normalised (set.macros);
          set.macros[key - 1] = text;
        }
    writeSets (s, sets);
  }

  int autoCqKey (QSettings * s) {GroupGuard g {s}; return qBound (1, s->value ("AutoCqKey", 1).toInt (), 8);}
  int autoCqGap (QSettings * s) {GroupGuard g {s}; return qBound (1, s->value ("AutoCqGap", 10).toInt (), 300);}
  int autoCqMax (QSettings * s) {GroupGuard g {s}; return qMax (0, s->value ("AutoCqMax", 0).toInt ());}
  bool autoCqStopOnMyCall (QSettings * s) {GroupGuard g {s}; return s->value ("AutoCqStopOnMyCall", true).toBool ();}
  QString exchange (QSettings * s)
  {
    GroupGuard g {s};
    auto const e = s->value ("Exchange", "599 %N").toString ().trimmed ();
    return e.isEmpty () ? QString {"599 %N"} : e;
  }
}

using namespace JttySettings;

JttySettingsPage::JttySettingsPage (QWidget * parent)
  : QWidget {parent}
  , autoCqKey_ {new QComboBox}
  , autoCqGap_ {new QSpinBox}
  , autoCqMax_ {new QSpinBox}
  , stopOnMyCall_ {new QCheckBox {tr ("Stop when my call is decoded")}}
  , exchange_ {new QLineEdit}
  , setCombo_ {new QComboBox}
  , newSet_ {new QPushButton {tr ("New...")}}
  , renameSet_ {new QPushButton {tr ("Rename...")}}
  , deleteSet_ {new QPushButton {tr ("Delete")}}
  , resetSet_ {new QPushButton {tr ("Reset to built-in")}}
{
  for (int key = 1; key <= 8; ++key) autoCqKey_->addItem (QString {"F%1"}.arg (key), key);
  autoCqGap_->setRange (1, 300);
  autoCqGap_->setSuffix (tr (" s"));
  autoCqGap_->setToolTip (tr ("Time to listen after each CQ before calling again."));
  autoCqMax_->setRange (0, 999);
  autoCqMax_->setSpecialValueText (tr ("no limit"));
  autoCqMax_->setToolTip (tr ("Stop after this many calls. Halt, Esc, any other transmission\n"
                              "or picking a DX call stops Auto CQ anyway."));
  stopOnMyCall_->setToolTip (tr ("Stop calling as soon as a decode on any frequency contains my call."));
  autoCqKey_->setToolTip (tr ("The F-key macro Auto CQ sends."));
  exchange_->setToolTip (tr ("What %E sends. %N is the serial number; it is left out when\n"
                             "Serial Number on the JTTY panel is set to none."));

  auto autoBox = new QGroupBox {tr ("Auto CQ")};
  auto autoForm = new QFormLayout {autoBox};
  autoForm->addRow (tr ("Macro"), autoCqKey_);
  autoForm->addRow (tr ("Gap after each call"), autoCqGap_);
  autoForm->addRow (tr ("Stop after"), autoCqMax_);
  autoForm->addRow (stopOnMyCall_);

  auto exchangeBox = new QGroupBox {tr ("Exchange")};
  auto exchangeForm = new QFormLayout {exchangeBox};
  exchangeForm->addRow (tr ("%E sends"), exchange_);

  auto setsBox = new QGroupBox {tr ("Macro sets")};
  auto setsLayout = new QVBoxLayout {setsBox};
  auto setRow = new QHBoxLayout;
  setRow->addWidget (new QLabel {tr ("Active set")});
  setRow->addWidget (setCombo_, 1);
  setRow->addWidget (newSet_);
  setRow->addWidget (renameSet_);
  setRow->addWidget (deleteSet_);
  setRow->addWidget (resetSet_);
  setsLayout->addLayout (setRow);
  auto grid = new QGridLayout;
  for (int key = 1; key <= 8; ++key)
    {
      macros_[key - 1] = new QLineEdit;
      macros_[key - 1]->setMaxLength (80);
      int const row = (key - 1) % 4, col = (key - 1) / 4 * 2;
      grid->addWidget (new QLabel {QString {"F%1"}.arg (key)}, row, col);
      grid->addWidget (macros_[key - 1], row, col + 1);
    }
  grid->setColumnStretch (1, 1);
  grid->setColumnStretch (3, 1);
  setsLayout->addLayout (grid);
  setsLayout->addWidget (new QLabel {tr ("%M my call, %H DX call, %Q Call next (or DX call), %N serial, %E exchange.\n"
                                         "The set chosen here is the one the F-keys use; edits on the JTTY panel go into it.")});

  auto top = new QHBoxLayout;
  top->addWidget (autoBox);
  top->addWidget (exchangeBox, 1);
  auto page = new QVBoxLayout {this};
  page->addLayout (top);
  page->addWidget (setsBox);
  page->addStretch ();

  connect (setCombo_, QOverload<int>::of (&QComboBox::currentIndexChanged), this, [this] (int index) {
      keepEdits ();
      showSet (index);
    });
  connect (newSet_, &QPushButton::clicked, this, [this] {
      keepEdits ();
      bool ok {false};
      auto const name = QInputDialog::getText (this, tr ("New macro set"), tr ("Name:"), QLineEdit::Normal, QString {}, &ok).trimmed ();
      if (!ok || name.isEmpty ()) return;
      for (auto const& set : sets_)
        if (set.name == name) {QMessageBox::warning (this, tr ("Macro sets"), tr ("There is already a set called %1.").arg (name)); return;}
      // start from the set on screen, so a variant is quick to make
      sets_ << MacroSet {name, current_ >= 0 ? sets_[current_].macros : builtInSets ().front ().macros};
      current_ = -1;
      refreshSetCombo ();
      setCombo_->setCurrentIndex (sets_.size () - 1);
    });
  connect (renameSet_, &QPushButton::clicked, this, [this] {
      if (current_ < 0) return;
      keepEdits ();
      bool ok {false};
      auto const name = QInputDialog::getText (this, tr ("Rename macro set"), tr ("Name:"), QLineEdit::Normal, sets_[current_].name, &ok).trimmed ();
      if (!ok || name.isEmpty () || name == sets_[current_].name) return;
      for (auto const& set : sets_)
        if (set.name == name) {QMessageBox::warning (this, tr ("Macro sets"), tr ("There is already a set called %1.").arg (name)); return;}
      sets_[current_].name = name;
      int const keep = current_;
      current_ = -1;
      refreshSetCombo ();
      setCombo_->setCurrentIndex (keep);
    });
  connect (deleteSet_, &QPushButton::clicked, this, [this] {
      if (current_ < 0 || sets_.size () < 2) return;
      sets_.remove (current_);
      current_ = -1;
      refreshSetCombo ();
      setCombo_->setCurrentIndex (0);
      showSet (0);
    });
  connect (resetSet_, &QPushButton::clicked, this, [this] {
      if (current_ < 0) return;
      // a built-in set by name gets its own text back, any other set the
      // WSJT-X contest set
      auto macros = builtInSets ().front ().macros;
      for (auto const& b : builtInSets ()) if (b.name == sets_[current_].name) macros = b.macros;
      for (int i = 0; i < 8; ++i) macros_[i]->setText (macros[i]);
    });
}

void JttySettingsPage::refreshSetCombo ()
{
  QSignalBlocker block {setCombo_};
  setCombo_->clear ();
  for (auto const& set : sets_) setCombo_->addItem (set.name);
  deleteSet_->setEnabled (sets_.size () > 1);
}

void JttySettingsPage::keepEdits ()
{
  if (current_ < 0 || current_ >= sets_.size ()) return;
  QStringList macros;
  for (auto * edit : macros_) macros << edit->text ().trimmed ();
  sets_[current_].macros = macros;
}

void JttySettingsPage::showSet (int index)
{
  if (index < 0 || index >= sets_.size ()) return;
  current_ = index;
  auto macros = sets_[index].macros;
  while (macros.size () < 8) macros << QString {};
  for (int i = 0; i < 8; ++i) macros_[i]->setText (macros[i]);
}

void JttySettingsPage::load (QSettings * s)
{
  autoCqKey_->setCurrentIndex (autoCqKey (s) - 1);
  autoCqGap_->setValue (autoCqGap (s));
  autoCqMax_->setValue (autoCqMax (s));
  stopOnMyCall_->setChecked (autoCqStopOnMyCall (s));
  exchange_->setText (exchange (s));
  sets_ = readSets (s);
  auto const active = activeSet (s).name;
  current_ = -1;
  refreshSetCombo ();
  int index = 0;
  for (int i = 0; i < sets_.size (); ++i) if (sets_[i].name == active) index = i;
  {
    QSignalBlocker block {setCombo_};
    setCombo_->setCurrentIndex (index);
  }
  showSet (index);
}

void JttySettingsPage::save (QSettings * s)
{
  keepEdits ();
  writeSets (s, sets_);
  if (current_ >= 0) setActiveSetName (s, sets_[current_].name);
  s->beginGroup ("JTTY");
  s->setValue ("AutoCqKey", autoCqKey_->currentData ().toInt ());
  s->setValue ("AutoCqGap", autoCqGap_->value ());
  s->setValue ("AutoCqMax", autoCqMax_->value ());
  s->setValue ("AutoCqStopOnMyCall", stopOnMyCall_->isChecked ());
  s->setValue ("Exchange", exchange_->text ().trimmed ().isEmpty () ? QString {"599 %N"} : exchange_->text ().trimmed ());
  s->endGroup ();
}
