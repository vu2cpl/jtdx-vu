// JTDX-VU: JTTY control row.  See jttypanel.h.

#include "jttypanel.h"

#include <QSettings>
#include <QHBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>

JttyPanel::JttyPanel (QSettings * settings, QWidget * parent)
  : QWidget {parent}
  , settings_ {settings}
  , ftol_ {new QComboBox}
  , lowerCase_ {new QCheckBox {tr ("Lower case")}}
  , includeTime_ {new QCheckBox {tr ("Include time")}}
{
  settings_->beginGroup ("JTTY");
  int const ftol = settings_->value ("Ftol", 100).toInt ();
  lowerCase_->setChecked (settings_->value ("LowerCase", false).toBool ());
  includeTime_->setChecked (settings_->value ("IncludeTime", true).toBool ());
  settings_->endGroup ();

  // same tolerance ladder as WSJT-X's sbFtol_2
  for (int hz : {2, 5, 10, 20, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500})
    ftol_->addItem (QString::number (hz), hz);
  int const idx = ftol_->findData (ftol);
  ftol_->setCurrentIndex (idx >= 0 ? idx : ftol_->findData (100));
  ftol_->setToolTip (tr ("F Tol: decodes within this many Hz of the Rx frequency go to the Rx Frequency pane"));
  ftol_->setFocusPolicy (Qt::NoFocus);
  lowerCase_->setFocusPolicy (Qt::NoFocus);
  includeTime_->setFocusPolicy (Qt::NoFocus);

  auto row = new QHBoxLayout {this};
  row->setContentsMargins (0, 0, 0, 0);
  row->setSpacing (6);
  auto title = new QLabel {tr ("JTTY")};
  title->setStyleSheet ("font-weight: bold;");
  row->addWidget (title);
  row->addSpacing (8);
  row->addWidget (new QLabel {tr ("F Tol")});
  row->addWidget (ftol_);
  row->addSpacing (12);
  row->addWidget (lowerCase_);
  row->addWidget (includeTime_);
  row->addStretch ();

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
