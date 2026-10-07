#ifndef FLEXSHARED_H
#define FLEXSHARED_H
// JTDX-VU: what the FlexRadio VITA-49 rig (FlexTransceiver, rig thread) knows
// about the radio, for the Flex panel (GUI thread), and the panel's commands
// going the other way.  One mutex around all of it; the panel polls a copy.
// Port of the MSHV-Mac Flex panel's hooks (network.cpp, "flex native
// vita-49", Copyright (C) 2026 Manoj Ramawarrier VU2CPL).
#include <QMutex>
#include <QString>
#include <QStringList>
#include <QHash>

struct FlexStatus
{
  bool up {false};                 // a session is running
  QString status;                  // one line for the top of the panel
  QString model;
  int slice {-1};
  int dax_channel {0};
  bool tx {false};                 // DAX TX stream up: JTDX-VU owns the transmitter
  qint64 frequency {0};            // Hz, the slice's
  QString rxant, txant, mode;
  QStringList ant_list, tx_ant_list, mode_list;
  bool meters_ok {false};
  double fwd_w {0}, ref_w {0}, swr {0};
  QHash<QString, double> meter;    // by name (ALC, PATEMP, +13.8A, LEVEL...), latest value
  int rfpower {-1}, tunepower {-1}, maxpower {-1};   // -1: not reported yet
  bool hwalc {false};
  int atu_present {-1};            // -1 unknown, 0 none, 1 present
  QString atu_status, atu_refused;
  bool atu_enabled {false}, atu_memories {false}, atu_using_mem {false}, atu_after_cycle {false};
  bool spkr_supported {false}, spkr_mute {false};
};

class FlexShared
{
public:
  static FlexShared & instance () {static FlexShared s; return s;}
  FlexStatus snapshot () {QMutexLocker l {&m_}; return s_;}
  template <typename F> void update (F f) {QMutexLocker l {&m_}; f (s_);}
  // commands from the panel; "{slice}" is replaced by the slice in use
  void command (QString const& c) {QMutexLocker l {&m_}; if (s_.up) q_ << c;}
  QStringList take () {QMutexLocker l {&m_}; QStringList r; r.swap (q_); return r;}
  void reset () {QMutexLocker l {&m_}; s_ = FlexStatus {}; q_.clear ();}
private:
  FlexShared () = default;
  QMutex m_;
  FlexStatus s_;
  QStringList q_;
};
#endif
