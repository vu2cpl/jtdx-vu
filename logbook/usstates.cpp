// JTDX-VU: Show US State, see usstates.h
#include "usstates.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include "Radio.hpp"

namespace
{
  QByteArray read_table (QString const& name)
  {
    QDir const data {QStandardPaths::writableLocation (QStandardPaths::DataLocation)};
    QFile file {data.exists (name) ? data.absoluteFilePath (name) : ":/" + name};
    return file.open (QIODevice::ReadOnly) ? file.readAll () : QByteArray {};
  }
}

void UsStates::load ()
{
  loaded_ = true;
  for (auto const& line : read_table ("us_grid_states.txt").split ('\n'))
    {
      if (line.isEmpty () || line.startsWith ('#')) continue;
      auto const f = QString::fromLatin1 (line).split (' ', QString::SkipEmptyParts);
      if (f.size () >= 2) grids_.insert (f[0], QStringList (f.mid (1)).join ('-'));
    }
  calls_ = read_table ("us_call_states.txt");
  offsets_.reserve (calls_.size () / 9);
  for (int i = 0; i < calls_.size (); )
    {
      offsets_ << i;
      int const nl = calls_.indexOf ('\n', i);
      if (nl < 0) break;
      i = nl + 1;
    }
}

QString UsStates::byCall (QString const& call) const
{
  QByteArray const key = call.toLatin1 ();
  int lo = 0, hi = offsets_.size () - 1;
  while (lo <= hi)
    {
      int const mid = (lo + hi) / 2;
      int const start = offsets_[mid];
      int const space = calls_.indexOf (' ', start);
      if (space < 0) return {};
      int const cmp = qstrcmp (calls_.mid (start, space - start), key);
      if (cmp == 0) return QString::fromLatin1 (calls_.mid (space + 1, 2));
      if (cmp < 0) lo = mid + 1; else hi = mid - 1;
    }
  return {};
}

QString UsStates::find (QString const& call, QString const& grid)
{
  if (!loaded_) load ();
  QString const g = grid.trimmed ().left (4).toUpper ();
  if (g.size () == 4 && g != "RR73")
    {
      auto const it = grids_.constFind (g);
      if (it != grids_.constEnd ()) return it.value ();
    }
  QString base = Radio::base_callsign (call.trimmed ().toUpper ());
  if (base.startsWith ('<')) base = base.mid (1, base.size () - 2);
  return byCall (base);
}
