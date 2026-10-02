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
#include <QTabWidget>

namespace
{
  // WSJT-X 3.2 native F-key templates (doc/user_guide jtty.adoc)
  struct Macro {char const * tip; char const * tpl; char const * label;};
  Macro const native[8] = {
    {"CQ",                     "CQ %M CQ",       "CQ"},
    {"HisCall Exchange",       "%H %E",          "Exch"},
    {"TU CQ",                  "%H TU CQ %M CQ", "TU CQ"},
    {"MyCall",                 "%M",             "My call"},
    {"HisCall",                "%H",             "His call"},
    {"Now QueuedCall Exchange","TU NOW %Q %E",   "TU next"},
    {"HisCall AGN ?",          "%H AGN?",        "AGN?"},
    {"Exchange",               "%E",             "Exch only"},
  };
  // contest banks 2-3: repeats and fills
  Macro const contestMore[16] = {
    {"", "%H %E %E",                 "Exch x2"},
    {"", "%M %M",                    "My call x2"},
    {"", "%H %H",                    "His call x2"},
    {"", "NR?",                      "NR?"},
    {"", "RST?",                     "RST?"},
    {"", "CALL?",                    "CALL?"},
    {"", "QRZ? %M",                  "QRZ?"},
    {"", "TU %M",                    "TU"},
    {"", "%H TU 73",                 "TU 73"},
    {"", "QRL?",                     "QRL?"},
    {"", "%H QSL TU",                "QSL TU"},
    {"", "%H R %E",                  "R Exch"},
    {"", "%H R",                     "R"},
    {"", "%H AGN NR?",               "AGN NR?"},
    {"", "%M %E",                    "Me+Exch"},
    {"", "SRI QRL",                  "SRI QRL"},
  };
  // a conversational set for everyday (non-contest) QSOs
  Macro const ragchew[24] = {
    {"", "CQ CQ CQ DE %M %M %M K",                          "CQ"},
    {"", "%H DE %M TNX FER CALL UR %E %E %H DE %M K",       "Report"},
    {"", "%H DE %M TU 73 %H DE %M SK",                      "73 SK"},
    {"", "%M",                                              "My call"},
    {"", "%H",                                              "His call"},
    {"", "%H DE %M K",                                      "Over"},
    {"", "%H AGN? DE %M K",                                 "AGN?"},
    {"", "QRZ? DE %M K",                                    "QRZ?"},
    // bank 2: the ragchew itself
    {"", "%H DE %M R R TNX FER RPT %NAME. UR %E %E NAME %OP %OP QTH %QTH %QTH HW? %H DE %M K", "Rpt+Name"},
    {"", "%H DE %M NAME %OP %OP QTH %QTH %QTH %H DE %M K",  "Name QTH"},
    {"", "%H DE %M RIG %TX ANT %ANT %H DE %M K",            "Rig Ant"},
    {"", "%H DE %M QSL VIA LOTW AND CLUBLOG %H DE %M K",    "QSL info"},
    {"", "%H DE %M TNX FER NICE QSO %NAME 73 ES GL %H DE %M SK", "TNX 73"},
    {"", "%H DE %M R R FB %NAME %H DE %M K",                "R FB"},
    {"", "%H DE %M PSE RPT MY RST AND UR NAME %H DE %M K",  "RST? Name?"},
    {"", "%H DE %M QRM QRN PSE AGN %H DE %M K",             "QRM AGN"},
    // bank 3: calling and replying
    {"", "CQ DX CQ DX DE %M %M DX K",                       "CQ DX"},
    {"", "%H %H DE %M %M %M K",                             "Call him"},
    {"", "%H DE %M %M K",                                   "Reply"},
    {"", "%H DE %M R R UR %E %E 73 %H DE %M K",             "Quick QSO"},
    {"", "%H DE %M TU 73 QRZ? DE %M K",                     "73 QRZ?"},
    {"", "QRL? DE %M",                                      "QRL?"},
    {"", "%H DE %M PSE QRS %H DE %M K",                     "QRS"},
    {"", "%H DE %M SRI LOST U %H DE %M K",                  "Lost you"},
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

  QString const contestExchange {"%RST %N"};
  QString const ragchewExchange {"%RST"};

  QStringList normalised (QStringList list)
  {
    while (list.size () < JttySettings::macroCount) list << QString {};
    return list.mid (0, JttySettings::macroCount);
  }

  bool emptyFrom (QStringList const& list, int first)
  {
    for (int i = first; i < list.size (); ++i) if (!list[i].trimmed ().isEmpty ()) return false;
    return true;
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

  QString keyName (int key)
  {
    if (key < 1 || key > macroCount) return {};
    int const bank = (key - 1) / keysPerBank;
    auto const f = QString {"F%1"}.arg ((key - 1) % keysPerBank + 1);
#if defined (Q_OS_MAC)
    char const * const prefix[bankCount] = {"", "Shift+", "Opt+"};
#else
    char const * const prefix[bankCount] = {"", "Shift+", "Alt+"};
#endif
    return QString::fromLatin1 (prefix[bank]) + f;
  }

  QString buttonLabel (MacroSet const& set, int key)
  {
    if (key < 1 || key > macroCount) return {};
    auto const label = set.labels.value (key - 1).trimmed ();
    if (!label.isEmpty ()) return label;
    // no label: the macro's own words - variables, DE, K and repeats
    // dropped ("%H DE %M TU 73 %H DE %M SK" -> "TU 73 SK")
    auto const macro = set.macros.value (key - 1).simplified ();
    QStringList words;
    for (auto const& w : macro.split (' ', Qt::SkipEmptyParts))
      {
        if (w.startsWith ('%') || w == "DE" || w == "K" || w == "KN") continue;
        if (!words.isEmpty () && words.back () == w) continue;
        words << w;
      }
    auto text = words.join (' ');
    if (text.isEmpty ()) text = macro;     // e.g. "%M" alone
    return text.size () > 12 ? text.left (11) + QChar {0x2026} : text;
  }

  bool usesSerial (MacroSet const& set)
  {
    if (set.exchange.contains ("%N") && !set.exchange.contains ("%NAME")) return true;
    for (auto const& m : set.macros)
      if (QString {m}.remove ("%NAME").contains ("%N")) return true;
    return false;
  }

  QVector<MacroSet> builtInSets ()
  {
    MacroSet contest {contestName, {}, {}, contestExchange};
    MacroSet chat {ragchewName, {}, {}, ragchewExchange};
    for (int i = 0; i < macroCount; ++i)
      {
        auto const& c = i < 8 ? native[i] : contestMore[i - 8];
        contest.macros << QString::fromLatin1 (c.tpl);
        contest.labels << QString::fromLatin1 (c.label);
        chat.macros << QString::fromLatin1 (ragchew[i].tpl);
        chat.labels << QString::fromLatin1 (ragchew[i].label);
      }
    return {contest, chat};
  }

  QVector<MacroSet> readSets (QSettings * s)
  {
    QVector<MacroSet> sets;
    bool banksDone {false};
    {
      GroupGuard g {s};
      banksDone = s->value ("Banks", 0).toInt () >= bankCount;
      int const n = s->beginReadArray ("MacroSets");
      for (int i = 0; i < n; ++i)
        {
          s->setArrayIndex (i);
          MacroSet set;
          set.name = s->value ("name").toString ().trimmed ();
          for (int key = 1; key <= macroCount; ++key)
            {
              set.macros << s->value (QString {"Msg%1"}.arg (key)).toString ();
              set.labels << s->value (QString {"Label%1"}.arg (key)).toString ();
            }
          set.exchange = s->value ("exchange").toString ().trimmed ();
          if (!set.name.isEmpty ()) sets << set;
        }
      s->endArray ();
    }
    // sets saved before each carried its own exchange: built-in ones take
    // theirs, others the old global Exchange
    bool migrated {false};
    for (auto& set : sets)
      if (set.exchange.isEmpty ())
        {
          migrated = true;
          if (set.name == contestName) set.exchange = contestExchange;
          else if (set.name == ragchewName) set.exchange = ragchewExchange;
          else
            {
              GroupGuard g {s};
              set.exchange = s->value ("Exchange", contestExchange).toString ().trimmed ();
              if (set.exchange.isEmpty ()) set.exchange = contestExchange;
            }
        }
    // sets saved with eight macros: the built-in ones get their banks 2-3,
    // labels and the %RST exchange (same text while RST sent is 599); the
    // operator's own sets keep their macros and get empty banks
    if (!banksDone && !sets.isEmpty ())
      {
        migrated = true;
        for (auto& set : sets)
          for (auto const& b : builtInSets ())
            if (set.name == b.name)
              {
                set.macros = normalised (set.macros);
                set.labels = normalised (set.labels);
                if (emptyFrom (set.macros, keysPerBank))
                  for (int i = keysPerBank; i < macroCount; ++i)
                    {
                      set.macros[i] = b.macros[i];
                      set.labels[i] = b.labels[i];
                    }
                for (int i = 0; i < keysPerBank; ++i)
                  if (set.labels[i].isEmpty () && set.macros[i] == b.macros[i]) set.labels[i] = b.labels[i];
                if (set.exchange == "599") set.exchange = "%RST";
                else if (set.exchange == "599 %N") set.exchange = "%RST %N";
              }
      }
    if (migrated) writeSets (s, sets);
    if (sets.isEmpty ())
      {
        // first use: the panel's old single set becomes "Default", the
        // built-in sets follow
        MacroSet old {QObject::tr ("Default"), {}, {}, contestExchange};
        {
          GroupGuard g {s};
          for (int key = 1; key <= 8; ++key)
            old.macros << s->value (QString {"Msg%1"}.arg (key), defaultMacro (key)).toString ();
          auto const e = s->value ("Exchange").toString ().trimmed ();
          if (!e.isEmpty ()) old.exchange = e;
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
        auto const labels = normalised (sets[i].labels);
        for (int key = 1; key <= macroCount; ++key)
          {
            s->setValue (QString {"Msg%1"}.arg (key), macros[key - 1]);
            s->setValue (QString {"Label%1"}.arg (key), labels[key - 1].trimmed ());
          }
        s->setValue ("exchange", sets[i].exchange.isEmpty () ? contestExchange : sets[i].exchange);
      }
    s->endArray ();
    s->setValue ("Banks", bankCount);
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
      if (set.name == name) return {set.name, normalised (set.macros), normalised (set.labels), set.exchange};
    return {sets.front ().name, normalised (sets.front ().macros), normalised (sets.front ().labels), sets.front ().exchange};
  }

  int autoCqKey (QSettings * s) {GroupGuard g {s}; return qBound (1, s->value ("AutoCqKey", 1).toInt (), macroCount);}
  int autoCqGap (QSettings * s) {GroupGuard g {s}; return qBound (1, s->value ("AutoCqGap", 10).toInt (), 300);}
  int autoCqMax (QSettings * s) {GroupGuard g {s}; return qMax (0, s->value ("AutoCqMax", 0).toInt ());}
  int autoCqMinutes (QSettings * s) {GroupGuard g {s}; return qBound (1, s->value ("AutoCqMinutes", 5).toInt (), 60);}
  bool autoCqStopOnMyCall (QSettings * s) {GroupGuard g {s}; return s->value ("AutoCqStopOnMyCall", true).toBool ();}
  StationVar const stationVars[4] = {
    {"%OP",  "OpName",  QT_TRANSLATE_NOOP ("JttySettings", "Name (%OP)")},
    {"%QTH", "Qth",     QT_TRANSLATE_NOOP ("JttySettings", "QTH (%QTH)")},
    {"%TX",  "Radio",   QT_TRANSLATE_NOOP ("JttySettings", "Radio (%TX)")},
    {"%ANT", "Antenna", QT_TRANSLATE_NOOP ("JttySettings", "Antenna (%ANT)")},
  };

  QString stationValue (QSettings * s, char const * key)
  {
    GroupGuard g {s};
    return s->value (key).toString ().trimmed ().toUpper ();
  }

  QString exchange (QSettings * s)
  {
    auto const e = activeSet (s).exchange;
    return e.isEmpty () ? contestExchange : e;
  }
}

using namespace JttySettings;

JttySettingsPage::JttySettingsPage (QWidget * parent)
  : QWidget {parent}
  , autoCqKey_ {new QComboBox}
  , autoCqGap_ {new QSpinBox}
  , autoCqMax_ {new QSpinBox}
  , autoCqMinutes_ {new QSpinBox}
  , stopOnMyCall_ {new QCheckBox {tr ("Stop when my call is decoded")}}
  , exchange_ {new QLineEdit}
  , setCombo_ {new QComboBox}
  , newSet_ {new QPushButton {tr ("New...")}}
  , renameSet_ {new QPushButton {tr ("Rename...")}}
  , deleteSet_ {new QPushButton {tr ("Delete")}}
  , resetSet_ {new QPushButton {tr ("Reset to built-in")}}
{
  for (auto*& e : station_) e = new QLineEdit;
  for (int key = 1; key <= macroCount; ++key) autoCqKey_->addItem (keyName (key), key);
  autoCqGap_->setRange (1, 300);
  autoCqGap_->setSuffix (tr (" s"));
  autoCqGap_->setToolTip (tr ("Time to listen after each CQ before calling again."));
  autoCqMax_->setRange (0, 999);
  autoCqMax_->setSpecialValueText (tr ("no limit"));
  autoCqMax_->setToolTip (tr ("Stop after this many calls. Halt, Esc, any other transmission\n"
                              "or picking a DX call stops Auto CQ anyway."));
  autoCqMinutes_->setRange (1, 60);
  autoCqMinutes_->setSuffix (tr (" min"));
  autoCqMinutes_->setToolTip (tr ("Auto CQ stops this long after you start it.\n"
                                  "In FT8 / FT4 / FT2 it is also the Tx watchdog while Call Non-Stop\n"
                                  "(the same Auto CQ button) is on."));
  stopOnMyCall_->setToolTip (tr ("Stop calling as soon as a decode on any frequency contains my call."));
  autoCqKey_->setToolTip (tr ("The macro Auto CQ sends."));
  exchange_->setToolTip (tr ("What %E sends with this set. %N is the serial number; it is left\n"
                             "out when Serial Number on the JTTY panel is set to none."));

  auto autoBox = new QGroupBox {tr ("Auto CQ")};
  auto autoForm = new QFormLayout {autoBox};
  autoForm->addRow (tr ("Macro"), autoCqKey_);
  autoForm->addRow (tr ("Gap after each call"), autoCqGap_);
  autoForm->addRow (tr ("Stop after"), autoCqMax_);
  autoForm->addRow (tr ("Time limit"), autoCqMinutes_);
  autoForm->addRow (stopOnMyCall_);


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
  // one tab per bank: key, button label, macro - two columns of four
  auto banks = new QTabWidget;
  for (int bank = 0; bank < bankCount; ++bank)
    {
      auto page = new QWidget;
      auto grid = new QGridLayout {page};
      grid->setVerticalSpacing (3);
      for (int i = 0; i < keysPerBank; ++i)
        {
          int const index = bank * keysPerBank + i;
          macros_[index] = new QLineEdit;
          macros_[index]->setMaxLength (80);
          labels_[index] = new QLineEdit;
          labels_[index]->setMaxLength (12);
          labels_[index]->setFixedWidth (labels_[index]->fontMetrics ().horizontalAdvance ("WWWWWWWW") + 10);
          labels_[index]->setPlaceholderText (tr ("label"));
          labels_[index]->setToolTip (tr ("What the panel button shows. Empty: a short form of the macro."));
          int const row = i % 4, col = i / 4 * 3;
          grid->addWidget (new QLabel {keyName (index + 1)}, row, col);
          grid->addWidget (labels_[index], row, col + 1);
          grid->addWidget (macros_[index], row, col + 2);
        }
      grid->setColumnStretch (2, 1);
      grid->setColumnStretch (5, 1);
      banks->addTab (page, tr ("Bank %1 (%2-%3)").arg (bank + 1).arg (keyName (bank * keysPerBank + 1))
                     .arg (QString {"F%1"}.arg (keysPerBank)));
    }
  setsLayout->addWidget (banks);
  auto exchangeRow = new QHBoxLayout;
  exchangeRow->addWidget (new QLabel {tr ("%E sends")});
  exchangeRow->addWidget (exchange_, 1);
  setsLayout->addLayout (exchangeRow);
  setsLayout->addWidget (new QLabel {tr ("%M my call, %H DX call, %Q Call next (or DX call), %N serial, %E exchange,\n"
                                         "%RST the RST you send, %NAME his name (both from the JTTY QSO fields),\n"
                                         "%OP name, %QTH location, %TX radio, %ANT antenna (Station details above).\n"
                                         "The set chosen here (or on the JTTY panel) is the one the keys use.\n"
                                         "The panel shows Serial Number only when the set uses %N.")});

  auto stationBox = new QGroupBox {tr ("Station details (for macros)")};
  auto stationForm = new QFormLayout {stationBox};
  for (int i = 0; i < 4; ++i)
    {
      station_[i]->setMaxLength (40);
      stationForm->addRow (tr (stationVars[i].label), station_[i]);
    }
  station_[0]->setPlaceholderText (tr ("e.g. MANOJ"));
  station_[1]->setPlaceholderText (tr ("e.g. BANGALORE"));
  station_[2]->setPlaceholderText (tr ("e.g. FLEX 6600"));
  station_[3]->setPlaceholderText (tr ("e.g. HEXBEAM"));

  auto top = new QHBoxLayout;
  top->addWidget (autoBox);
  top->addWidget (stationBox, 1);
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
      sets_ << (current_ >= 0 ? MacroSet {name, sets_[current_].macros, sets_[current_].labels, sets_[current_].exchange}
                              : builtInSets ().front ());
      if (current_ < 0) sets_.back ().name = name;
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
      auto base = builtInSets ().front ();
      for (auto const& b : builtInSets ()) if (b.name == sets_[current_].name) base = b;
      for (int i = 0; i < macroCount; ++i)
        {
          macros_[i]->setText (base.macros[i]);
          labels_[i]->setText (base.labels[i]);
        }
      exchange_->setText (base.exchange);
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
  QStringList labels;
  for (auto * edit : macros_) macros << edit->text ().trimmed ();
  for (auto * edit : labels_) labels << edit->text ().trimmed ();
  sets_[current_].macros = macros;
  sets_[current_].labels = labels;
  auto const e = exchange_->text ().trimmed ();
  sets_[current_].exchange = e.isEmpty () ? contestExchange : e;
}

void JttySettingsPage::showSet (int index)
{
  if (index < 0 || index >= sets_.size ()) return;
  current_ = index;
  auto macros = sets_[index].macros;
  auto labels = sets_[index].labels;
  while (macros.size () < macroCount) macros << QString {};
  while (labels.size () < macroCount) labels << QString {};
  for (int i = 0; i < macroCount; ++i)
    {
      macros_[i]->setText (macros[i]);
      labels_[i]->setText (labels[i]);
    }
  exchange_->setText (sets_[index].exchange);
}

void JttySettingsPage::load (QSettings * s)
{
  autoCqKey_->setCurrentIndex (autoCqKey (s) - 1);
  autoCqGap_->setValue (autoCqGap (s));
  autoCqMax_->setValue (autoCqMax (s));
  autoCqMinutes_->setValue (autoCqMinutes (s));
  stopOnMyCall_->setChecked (autoCqStopOnMyCall (s));
  for (int i = 0; i < 4; ++i) station_[i]->setText (stationValue (s, stationVars[i].key));
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
  s->setValue ("AutoCqMinutes", autoCqMinutes_->value ());
  s->setValue ("AutoCqStopOnMyCall", stopOnMyCall_->isChecked ());
  for (int i = 0; i < 4; ++i) s->setValue (stationVars[i].key, station_[i]->text ().trimmed ().toUpper ());
  s->endGroup ();
}
