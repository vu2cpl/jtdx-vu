// JTDX-VU (VU2CPL): band and mode switcher buttons.  See bandmodeswitcher.h.

#include "bandmodeswitcher.h"

#include <QSettings>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QCoreApplication>
#include <QComboBox>
#include <QLabel>
#include <QFrame>

QStringList const& BandModeSwitcher::all_modes ()
{
  static QStringList const modes {"FT8", "FT4", "JT9", "JT65", "T10", "JT9+JT65", "WSPR-2"};
  return modes;
}

QStringList const& BandModeSwitcher::all_bands ()
{
  static QStringList const bands {"160m", "80m", "60m", "40m", "30m", "20m", "17m", "15m",
                                  "12m", "10m", "6m", "4m", "2m"};
  return bands;
}

BandModeSwitcher::BandModeSwitcher (QSettings * settings, bool dark, QWidget * parent)
  : QWidget {parent}
  , settings_ {settings}
  , dark_ {dark}
  , row_ {new QHBoxLayout {this}}
  , new_only_combo_ {new QComboBox}
{
  settings_->beginGroup ("Switcher");
  show_bands_ = settings_->value ("ShowBands", true).toBool ();
  show_modes_ = settings_->value ("ShowModes", true).toBool ();
  bands_ = settings_->value ("Bands", QStringList {"160m", "80m", "40m", "30m", "20m", "17m",
                                                   "15m", "12m", "10m", "6m"}).toStringList ();
  modes_ = settings_->value ("Modes", QStringList {"FT8", "FT4"}).toStringList ();
  new_only_ = qBound (0, settings_->value ("NewOnly", 0).toInt (), 3);
  settings_->endGroup ();

  new_only_combo_->addItems ({tr ("All"), tr ("New DXCC"), tr ("New DXCC or band"), tr ("New DXCC, band or mode")});
  new_only_combo_->setCurrentIndex (new_only_);
  new_only_combo_->setFocusPolicy (Qt::NoFocus);
  new_only_combo_->setToolTip (tr ("Band Activity: show only decodes from DXCC entities you still need - as in MSHV:\n"
                                   "New DXCC = never worked; band = not on this band; mode = not in this mode on any band.\n"
                                   "Uses Club Log when enabled; traffic with your call and your QSO partner always shows."));
  connect (new_only_combo_, QOverload<int>::of (&QComboBox::currentIndexChanged), this, [this] (int level) {
      new_only_ = level;
      settings_->beginGroup ("Switcher");
      settings_->setValue ("NewOnly", level);
      settings_->endGroup ();
      restyle ();
      Q_EMIT new_only_changed (level);
    });

  row_->setContentsMargins (0, 0, 0, 0);
  row_->setSpacing (2);
  rebuild ();
}

void BandModeSwitcher::rebuild ()
{
  while (auto item = row_->takeAt (0))
    {
      if (item->widget () != new_only_combo_ && !trailing_.contains (item->widget ())) delete item->widget ();
      delete item;
    }
  band_buttons_.clear ();
  mode_buttons_.clear ();

  // one compact row: [modes] | [bands] | Show: [filter]
  auto add = [this] (QString const& text) {
    auto b = new QPushButton {text};
    b->setFocusPolicy (Qt::NoFocus);   // never pull keyboard focus off the message fields
    b->setMinimumWidth (b->fontMetrics ().horizontalAdvance (text) + 12);
    b->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);  // share the row's full width
    row_->addWidget (b);
    return b;
  };
  auto separator = [this] {
    auto line = new QFrame;
    line->setFrameShape (QFrame::VLine);
    line->setFrameShadow (QFrame::Sunken);
    row_->addSpacing (4);
    row_->addWidget (line);
    row_->addSpacing (4);
  };
  // keep the canonical order whatever order the settings list has
  if (show_modes_)
    for (auto const& m : all_modes ())
      if (modes_.contains (m))
        connect (mode_buttons_[m] = add (m), &QPushButton::clicked, this, [this, m] {Q_EMIT mode_clicked (m);});
  if (show_bands_)
    {
      if (!mode_buttons_.isEmpty ()) separator ();
      for (auto const& b : all_bands ())
        if (bands_.contains (b))
          connect (band_buttons_[b] = add (b), &QPushButton::clicked, this, [this, b] {Q_EMIT band_clicked (b);});
    }
  if (mode_buttons_.isEmpty () && band_buttons_.isEmpty ()) row_->addStretch ();
  else separator ();
  row_->addWidget (new QLabel {tr ("Show:")});
  row_->addWidget (new_only_combo_);
  for (auto w : trailing_) row_->addWidget (w);
  restyle ();
}

void BandModeSwitcher::set_active (QString const& band, QString const& mode)
{
  if (band == active_band_ && mode == active_mode_) return;
  active_band_ = band;
  active_mode_ = mode;
  restyle ();
}

void BandModeSwitcher::restyle ()
{
  // same greens as the MSHV switcher's active button
  auto const active = dark_ ? QString {"QPushButton{background-color:rgb(64,130,0);color:white;}"}
                            : QString {"QPushButton{background-color:rgb(140,240,140);color:black;}"};
  for (auto i = band_buttons_.cbegin (); i != band_buttons_.cend (); ++i)
    i.value ()->setStyleSheet (i.key () == active_band_ ? active : QString {});
  for (auto i = mode_buttons_.cbegin (); i != mode_buttons_.cend (); ++i)
    i.value ()->setStyleSheet (i.key () == active_mode_ ? active : QString {});
  // an active filter is easy to forget: show it in amber
  new_only_combo_->setStyleSheet (new_only_ ? (dark_ ? "QComboBox{background-color:rgb(150,95,0);color:white;}"
                                                     : "QComboBox{background-color:rgb(255,200,90);color:black;}")
                                            : QString {});
}

void BandModeSwitcher::settings_dialog (QWidget * parent)
{
  QDialog dialog {parent};
  dialog.setWindowTitle (QCoreApplication::applicationName () + " - " + tr ("Band & Mode Buttons"));

  auto mode_box = new QGroupBox {tr ("Mode buttons")};
  auto show_modes = new QCheckBox {tr ("Show mode buttons")};
  show_modes->setChecked (show_modes_);
  auto mode_grid = new QGridLayout {mode_box};
  mode_grid->addWidget (show_modes, 0, 0, 1, 4);
  QMap<QString, QCheckBox *> mode_checks;
  int n = 0;
  for (auto const& m : all_modes ())
    {
      auto c = mode_checks[m] = new QCheckBox {m};
      c->setChecked (modes_.contains (m));
      mode_grid->addWidget (c, 1 + n / 4, n % 4);
      ++n;
    }

  auto band_box = new QGroupBox {tr ("Band buttons")};
  auto show_bands = new QCheckBox {tr ("Show band buttons")};
  show_bands->setChecked (show_bands_);
  auto band_grid = new QGridLayout {band_box};
  band_grid->addWidget (show_bands, 0, 0, 1, 5);
  QMap<QString, QCheckBox *> band_checks;
  n = 0;
  for (auto const& b : all_bands ())
    {
      auto c = band_checks[b] = new QCheckBox {b};
      c->setChecked (bands_.contains (b));
      band_grid->addWidget (c, 1 + n / 5, n % 5);
      ++n;
    }

  auto buttons = new QDialogButtonBox {QDialogButtonBox::Ok | QDialogButtonBox::Cancel};
  connect (buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
  connect (buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
  auto layout = new QVBoxLayout {&dialog};
  layout->addWidget (mode_box);
  layout->addWidget (band_box);
  layout->addWidget (buttons);
  if (dialog.exec () != QDialog::Accepted) return;

  show_modes_ = show_modes->isChecked ();
  show_bands_ = show_bands->isChecked ();
  modes_.clear ();
  for (auto i = mode_checks.cbegin (); i != mode_checks.cend (); ++i)
    if (i.value ()->isChecked ()) modes_ << i.key ();
  bands_.clear ();
  for (auto i = band_checks.cbegin (); i != band_checks.cend (); ++i)
    if (i.value ()->isChecked ()) bands_ << i.key ();

  settings_->beginGroup ("Switcher");
  settings_->setValue ("ShowBands", show_bands_);
  settings_->setValue ("ShowModes", show_modes_);
  settings_->setValue ("Bands", bands_);
  settings_->setValue ("Modes", modes_);
  settings_->endGroup ();
  rebuild ();
}

void BandModeSwitcher::add_trailing_widget (QWidget * w)
{
  trailing_ << w;
  rebuild ();
}
