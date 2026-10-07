// JTDX-VU: FlexRadio panel - see flexpanel.h.  Port of the MSHV-Mac Flex panel
// (Copyright (C) 2026 Manoj Ramawarrier VU2CPL); comments on why each guard is
// there are kept from it.
#include "flexpanel.h"
#include "flexshared.h"
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QTimer>
#include <QVBoxLayout>

namespace
{
  QLabel * value_label ()
  {
    auto l = new QLabel {"--"};
    l->setFrameStyle (QFrame::Panel | QFrame::Sunken);
    l->setAlignment (Qt::AlignCenter);
    l->setMinimumWidth (84);
    return l;
  }
  void cmd (QString const& c) {FlexShared::instance ().command (c);}
}

FlexPanel::FlexPanel (QSettings * settings, std::function<QString (qint64)> band_of, QWidget * parent)
  : QDialog {parent}
  , settings_ {settings}
  , band_of_ {band_of}
{
  setWindowTitle (tr ("FlexRadio"));
  setWindowFlags (windowFlags () & ~Qt::WindowContextHelpButtonHint);
  setModal (false);                  // non-modal: keep working with it open

  auto V = new QVBoxLayout {this};
  V->setContentsMargins (8, 8, 8, 8);
  V->setSpacing (6);
  l_conn_ = new QLabel;
  l_conn_->setAlignment (Qt::AlignCenter);
  V->addWidget (l_conn_);

  auto gb_m = new QGroupBox {tr ("Radio meters")};
  auto G = new QGridLayout {gb_m};
  int r = 0;
  l_fwd_ = value_label (); G->addWidget (new QLabel {tr ("Forward power")}, r, 0); G->addWidget (l_fwd_, r++, 1);
  l_swr_ = value_label (); G->addWidget (new QLabel {tr ("SWR")}, r, 0); G->addWidget (l_swr_, r++, 1);
  l_ref_ = value_label (); G->addWidget (new QLabel {tr ("Reflected")}, r, 0); G->addWidget (l_ref_, r++, 1);
  l_alc_ = value_label (); G->addWidget (new QLabel {tr ("ALC")}, r, 0); G->addWidget (l_alc_, r++, 1);
  l_patemp_ = value_label (); G->addWidget (new QLabel {tr ("PA temperature")}, r, 0); G->addWidget (l_patemp_, r++, 1);
  l_volts_ = value_label (); G->addWidget (new QLabel {tr ("Supply")}, r, 0); G->addWidget (l_volts_, r++, 1);
  V->addWidget (gb_m);

  auto gb_c = new QGroupBox {tr ("Slice")};
  auto C = new QGridLayout {gb_c};
  cb_rxant_ = new QComboBox; cb_txant_ = new QComboBox; cb_mode_ = new QComboBox;
  C->addWidget (new QLabel {tr ("RX antenna")}, 0, 0); C->addWidget (cb_rxant_, 0, 1);
  C->addWidget (new QLabel {tr ("TX antenna")}, 1, 0); C->addWidget (cb_txant_, 1, 1);
  C->addWidget (new QLabel {tr ("Mode")}, 2, 0); C->addWidget (cb_mode_, 2, 1);
  V->addWidget (gb_c);

  // Transmit power: radio-global, not slice properties.  Spinboxes committing
  // on Enter / focus-out, never per keystroke - an amplifier is downstream.
  auto gb_p = new QGroupBox {tr ("Transmit power")};
  auto P = new QGridLayout {gb_p};
  sb_rfpower_ = new QSpinBox; sb_tunepower_ = new QSpinBox; sb_maxpower_ = new QSpinBox;
  for (auto * b : {sb_rfpower_, sb_tunepower_, sb_maxpower_})
    {
      b->setRange (0, 100);
      b->setSuffix (" %");
      b->setKeyboardTracking (false);
    }
  sb_rfpower_->setToolTip (tr ("RF drive for normal transmit (transmit set rfpower) - the TX slider\n"
                               "in the main window is the same setting.\n"
                               "Takes effect on Enter or when the field loses focus."));
  sb_tunepower_->setToolTip (tr ("RF drive for TUNE only (transmit set tunepower), kept separate so\n"
                                 "a tune-up does not hit the amplifier at full drive."));
  sb_maxpower_->setToolTip (tr ("Ceiling the radio enforces on the two above (max_power_level)."));
  cb_hwalc_ = new QCheckBox {tr ("Hardware ALC")};
  cb_hwalc_->setToolTip (tr ("Let an external amplifier's ALC line control drive (hwalc_enabled)."));
  int pr = 0;
  P->addWidget (new QLabel {tr ("RF power")}, pr, 0); P->addWidget (sb_rfpower_, pr++, 1);
  P->addWidget (new QLabel {tr ("Tune power")}, pr, 0); P->addWidget (sb_tunepower_, pr++, 1);
  P->addWidget (new QLabel {tr ("Max power")}, pr, 0); P->addWidget (sb_maxpower_, pr++, 1);
  P->addWidget (cb_hwalc_, pr, 0, 1, 2);
  V->addWidget (gb_p);

  // The radio's antenna tuner, hidden until the radio says it has one.
  gb_atu_ = new QGroupBox {tr ("Antenna tuner")};
  auto T = new QGridLayout {gb_atu_};
  l_atu_ = value_label ();
  pb_atu_tune_ = new QPushButton {tr ("Tune")};
  pb_atu_bypass_ = new QPushButton {tr ("Bypass")};
  cb_atu_mem_ = new QCheckBox {tr ("Memories")};
  pb_atu_tune_->setToolTip (tr ("Runs the radio's antenna tuner (atu start).\n"
                                "The radio TRANSMITS while it tunes - put an amplifier in standby first."));
  pb_atu_bypass_->setToolTip (tr ("Takes the tuner out of the antenna path (atu bypass)."));
  cb_atu_mem_->setToolTip (tr ("Use the tuner's stored settings (atu set memories_enabled)."));
  // NEVER a default button: in a QDialog the first push button takes Return,
  // so Enter in a power box (as its tooltip says to commit) would press Tune
  // and key the radio - found 2026-10-07 in a scripted test
  for (auto * b : {pb_atu_tune_, pb_atu_bypass_}) {b->setAutoDefault (false); b->setDefault (false);}
  T->addWidget (new QLabel {tr ("Status")}, 0, 0); T->addWidget (l_atu_, 0, 1);
  T->addWidget (pb_atu_tune_, 1, 0); T->addWidget (pb_atu_bypass_, 1, 1);
  T->addWidget (cb_atu_mem_, 2, 0, 1, 2);
  gb_atu_->hide ();
  V->addWidget (gb_atu_);

  auto gb_a = new QGroupBox {tr ("Local audio")};
  auto A = new QVBoxLayout {gb_a};
  cb_localmute_ = new QCheckBox {tr ("Mute front speaker")};
  cb_localmute_->setToolTip (tr ("Mutes the speaker in the radio's front panel (M series only).\n"
                                 "Line-out, headphones and the DAX audio JTDX-VU decodes are unaffected."));
  A->addWidget (cb_localmute_);
  l_model_ = new QLabel;
  A->addWidget (l_model_);
  V->addWidget (gb_a);

  // The operator's own antenna pick wins over a restore that has not fired yet.
  connect (cb_rxant_, QOverload<int>::of (&QComboBox::currentIndexChanged), this, [this] {
      if (filling_) return; restore_ = 0; cmd ("slice s {slice} rxant=" + cb_rxant_->currentText ());});
  connect (cb_txant_, QOverload<int>::of (&QComboBox::currentIndexChanged), this, [this] {
      if (filling_) return; restore_ = 0; cmd ("slice s {slice} txant=" + cb_txant_->currentText ());});
  connect (cb_mode_, QOverload<int>::of (&QComboBox::currentIndexChanged), this, [this] {
      if (filling_) return; cmd ("slice s {slice} mode=" + cb_mode_->currentText ());});
  connect (cb_localmute_, &QCheckBox::toggled, this, [this] (bool on) {
      if (filling_) return; cmd (QString {"mixer front_speaker mute %1"}.arg (on ? "on" : "off"));});
  connect (sb_rfpower_, &QSpinBox::editingFinished, this, [this] {
      send_if_edited (sb_rfpower_, shown_rf_, FlexShared::instance ().snapshot ().rfpower, "rfpower");});
  connect (sb_tunepower_, &QSpinBox::editingFinished, this, [this] {
      send_if_edited (sb_tunepower_, shown_tune_, FlexShared::instance ().snapshot ().tunepower, "tunepower");});
  connect (sb_maxpower_, &QSpinBox::editingFinished, this, [this] {
      send_if_edited (sb_maxpower_, shown_max_, FlexShared::instance ().snapshot ().maxpower, "max_power_level");});
  connect (cb_hwalc_, &QCheckBox::toggled, this, [this] (bool on) {
      if (filling_) return; cmd (QString {"transmit set hwalc_enabled=%1"}.arg (on ? 1 : 0));});
  // a tune cycle keys the radio: only ever from this click
  connect (pb_atu_tune_, &QPushButton::clicked, this, [] {cmd ("atu start");});
  connect (pb_atu_bypass_, &QPushButton::clicked, this, [] {cmd ("atu bypass");});
  connect (cb_atu_mem_, &QCheckBox::toggled, this, [this] (bool on) {
      if (filling_) return; cmd (QString {"atu set memories_enabled=%1"}.arg (on ? 1 : 0));});

  // per-band antennas, saved in the profile: "40m:ANT1:ANT1#2m:XVTA:XVTA"
  if (settings_)
    for (auto const& e : settings_->value ("FlexPanel/BandAntennas").toString ().split ('#', Qt::SkipEmptyParts))
      {
        auto const f = e.split (':');
        if (f.size () == 3 && !f[1].isEmpty () && !f[2].isEmpty ()) band_ant_[f[0]] = f[1] + ':' + f[2];
      }

  // runs while hidden too, so the antenna memory works with the panel closed
  timer_ = new QTimer {this};
  connect (timer_, &QTimer::timeout, this, &FlexPanel::refresh);
  timer_->start (500);
  refresh ();
}

QString FlexPanel::band_ant (QString const& band, bool tx) const
{
  auto const l = band_ant_.value (band).split (':');
  if (l.size () != 2) return {};
  return tx ? l[1] : l[0];
}

void FlexPanel::save_band_ants ()
{
  if (!settings_) return;
  QStringList l;
  for (auto it = band_ant_.cbegin (); it != band_ant_.cend (); ++it) l << it.key () + ':' + it.value ();
  auto const v = l.join ('#');
  if (settings_->value ("FlexPanel/BandAntennas").toString () != v) settings_->setValue ("FlexPanel/BandAntennas", v);
}

// Repopulate without the combo's own change signal firing back at the radio.
void FlexPanel::fill_combo (QComboBox * box, QStringList const& items, QString const& current)
{
  if (items.isEmpty ()) return;
  if (box->count () == items.size () && box->currentText () == current) return;
  filling_ = true;
  box->clear ();
  box->addItems (items);
  int const at = items.indexOf (current);
  if (at >= 0) box->setCurrentIndex (at);
  filling_ = false;
}

// Show the radio's value without echoing it back or overwriting a number the
// operator is typing; `shown` records what we displayed.
void FlexPanel::fill_spin (QSpinBox * box, int value, int & shown)
{
  if (value < 0 || box->hasFocus ()) return;
  shown = value;
  if (box->value () == value) return;
  filling_ = true;
  box->setValue (value);
  filling_ = false;
}

// Send a power setting ONLY when the operator changed the number: editingFinished
// also fires on a plain focus-out, Flex RF power is per band, and an untouched
// box must never push a stale or start-up value at the radio (MSHV-Mac).
void FlexPanel::send_if_edited (QSpinBox * box, int & shown, int radio_value, char const * key)
{
  if (filling_ || radio_value < 0 || shown < 0 || box->value () == shown) return;
  shown = box->value ();
  cmd (QString {"transmit set %1=%2"}.arg (key).arg (shown));
}

void FlexPanel::refresh ()
{
  auto const st = FlexShared::instance ().snapshot ();
  if (!st.up)
    {
      if (was_up_) {restore_ = 0; band_.clear (); was_up_ = false;}
      l_conn_->setText ("<b>" + tr ("FlexRadio: not connected") + "</b>");
      return;
    }
  if (!was_up_)
    {
      // a new session: the radio's fresh slice reports its own antennas first,
      // so put this band's pair back after the start, as after a band change
      was_up_ = true;
      band_ = band_of_ ? band_of_ (st.frequency) : QString {};
      restore_ = RestoreTicks;
    }
  l_conn_->setText ("<b>" + st.status + "</b>");

  if (st.meters_ok)
    {
      l_fwd_->setText (QString {"%1 W"}.arg (st.fwd_w, 0, 'f', st.fwd_w < 10. ? 2 : 1));
      l_ref_->setText (QString {"%1 W"}.arg (st.ref_w, 0, 'f', 2));
      l_swr_->setText (QString::number (st.swr, 'f', 2));
      // only meaningful under load - an unkeyed radio reads 1.00
      if (st.fwd_w > 0.5 && st.swr >= 3.) l_swr_->setStyleSheet ("QLabel{color:rgb(255,80,80);}");
      else if (st.fwd_w > 0.5 && st.swr >= 2.) l_swr_->setStyleSheet ("QLabel{color:rgb(255,180,60);}");
      else l_swr_->setStyleSheet ({});
    }
  if (st.meter.contains ("ALC")) l_alc_->setText (QString {"%1 dBFS"}.arg (st.meter["ALC"], 0, 'f', 1));
  if (st.meter.contains ("PATEMP")) l_patemp_->setText (QString {"%1 C"}.arg (st.meter["PATEMP"], 0, 'f', 1));
  if (st.meter.contains ("+13.8A")) l_volts_->setText (QString {"%1 V"}.arg (st.meter["+13.8A"], 0, 'f', 2));

  fill_combo (cb_rxant_, st.ant_list, st.rxant);
  fill_combo (cb_txant_, st.tx_ant_list, st.txant);
  fill_combo (cb_mode_, st.mode_list, st.mode);

  // per-band antennas: a band change arms a restore of that band's pair, sent
  // twice a second apart once the radio's own antenna change has landed
  QString const band = band_of_ ? band_of_ (st.frequency) : QString {};
  if (!band.isEmpty () && band != band_) {band_ = band; restore_ = RestoreTicks;}
  if (restore_ > 0)
    {
      if (!st.ant_list.isEmpty () && !st.tx_ant_list.isEmpty ())
        {
          --restore_;
          if (restore_ == SendFirst || restore_ == SendAgain)
            {
              auto const rx = band_ant (band_, false), tx = band_ant (band_, true);
              // never an antenna the radio no longer offers, nor an empty one
              if (st.ant_list.contains (rx) && rx != st.rxant) cmd ("slice s {slice} rxant=" + rx);
              if (st.tx_ant_list.contains (tx) && tx != st.txant) cmd ("slice s {slice} txant=" + tx);
            }
        }
    }
  else if (!band_.isEmpty () && !st.rxant.isEmpty () && !st.txant.isEmpty ())
    {
      band_ant_[band_] = st.rxant + ':' + st.txant;   // learn this band's pair
      save_band_ants ();
    }

  fill_spin (sb_rfpower_, st.rfpower, shown_rf_);
  fill_spin (sb_tunepower_, st.tunepower, shown_tune_);
  fill_spin (sb_maxpower_, st.maxpower, shown_max_);
  if (cb_hwalc_->isChecked () != st.hwalc) {filling_ = true; cb_hwalc_->setChecked (st.hwalc); filling_ = false;}

  // tuner: shown only on a radio that reports one, usable only while JTDX-VU
  // owns the transmitter (a tune keys the radio) and the tuner is enabled
  bool const atu_here = st.atu_present == 1;
  if (gb_atu_->isHidden () == atu_here) gb_atu_->setHidden (!atu_here);
  if (atu_here)
    {
      auto const& a = st.atu_status;
      QString text, color;
      if (a == "TUNE_SUCCESSFUL") {text = tr ("Tuned"); color = "rgb(80,200,80)";}
      else if (a == "TUNE_OK") {text = tr ("OK"); color = "rgb(80,200,80)";}
      else if (a == "TUNE_IN_PROGRESS") text = tr ("Tuning...");
      else if (a == "TUNE_BYPASS" && st.atu_after_cycle) {text = tr ("No tuning required"); color = "rgb(80,200,80)";}
      else if (a == "TUNE_BYPASS") {text = tr ("Bypass"); color = "rgb(255,180,60)";}
      else if (a == "TUNE_MANUAL_BYPASS") {text = tr ("Bypass (manual)"); color = "rgb(255,180,60)";}
      else if (a == "TUNE_FAIL_BYPASS") {text = tr ("Failed, bypassed"); color = "rgb(255,80,80)";}
      else if (a == "TUNE_FAIL") {text = tr ("Failed"); color = "rgb(255,80,80)";}
      else if (a == "TUNE_ABORTED") {text = tr ("Aborted"); color = "rgb(255,80,80)";}
      else if (a == "TUNE_NOT_STARTED") text = tr ("Not tuned");
      else if (a.isEmpty () || a == "NONE") text = "--";
      else text = a;
      if (st.atu_using_mem) text += " (" + tr ("memory") + ")";
      if (!st.atu_refused.isEmpty ()) {text = tr ("Refused by the radio") + " 0x" + st.atu_refused; color = "rgb(255,80,80)";}
      if (l_atu_->text () != text) l_atu_->setText (text);
      QString const css = color.isEmpty () ? QString {} : "QLabel{color:" + color + ";}";
      if (l_atu_->styleSheet () != css) l_atu_->setStyleSheet (css);
      bool const usable = st.tx && st.atu_enabled;
      pb_atu_tune_->setEnabled (usable && a != "TUNE_IN_PROGRESS");
      pb_atu_bypass_->setEnabled (usable);
      cb_atu_mem_->setEnabled (usable);
      if (cb_atu_mem_->isChecked () != st.atu_memories) {filling_ = true; cb_atu_mem_->setChecked (st.atu_memories); filling_ = false;}
    }
  if (cb_localmute_->isEnabled () != st.spkr_supported) cb_localmute_->setEnabled (st.spkr_supported);
  if (cb_localmute_->isChecked () != st.spkr_mute) {filling_ = true; cb_localmute_->setChecked (st.spkr_mute); filling_ = false;}
  if (!st.model.isEmpty ()) l_model_->setText (tr ("Radio: %1").arg (st.model));
}
