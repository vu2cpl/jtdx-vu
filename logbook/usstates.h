// JTDX-VU: Show US State - which US state(s) a station is in.
// The grid in the message comes first: a 4-character square maps to the
// state(s) it covers, largest share first ("KY-IN-OH"), from
// us_grid_states.txt (US Census boundaries, tools/build_us_grid_states.py).
// Without a grid, the callsign's FCC licence state from us_call_states.txt
// (tools/build_us_call_states.py; a newer copy in the data directory wins
// over the built-in one). Loaded on first use.
#ifndef USSTATES_H
#define USSTATES_H

#include <QByteArray>
#include <QHash>
#include <QString>
#include <QVector>

class UsStates
{
public:
  QString find (QString const& call, QString const& grid);

private:
  void load ();
  QString byCall (QString const& call) const;

  bool loaded_ {false};
  QHash<QString, QString> grids_;   // "EM78" -> "KY-IN-OH"
  QByteArray calls_;                // sorted "CALL ST\n" lines
  QVector<int> offsets_;            // start of each line in calls_
};

#endif
