// JTDX-VU (VU2CPL): tell the operator when a newer JTDX-VU release is out.
//
// One request, to GitHub's public API, nothing else:
//   GET https://api.github.com/repos/vu2cpl/jtdx-vu/releases/latest
// (no token; User-Agent JTDX-VU/<version>; 10 s timeout).  Nothing is
// downloaded or installed - the dialog's Download button only opens the
// release page in the browser.
//
// - Automatic: MainWindow calls start_automatic () once; it looks ~10 s
//   after start and then every hour while the program runs, and a check
//   goes out only when automatic_check_due () says so: Settings > General
//   "Check for updates automatically" on, not a development build, 24 h
//   since the last SUCCESSFUL check (JTDXVU/UpdateLastCheck), 1 h since a
//   failed automatic attempt (in memory only), no check under way and none
//   of these dialogs open.  Silent on any failure, and quiet about a release
//   the operator chose to skip (JTDXVU/UpdateSkipVersion).
// - Manual: Help > Check for Updates... calls check_manual (), which always
//   reports: the update dialog, "You're up to date", or why it failed.
// - Success = HTTP 200 and a JSON object with a tag_name, newer or not; only
//   then is UpdateLastCheck written.  A failure writes nothing, so the next
//   start tries again.
//
// Dialogs are non-modal, so the main window (Halt Tx, JTTY typing) stays
// usable; the automatic one also opens without taking the keyboard focus.

#ifndef UPDATECHECK_H
#define UPDATECHECK_H

#include <functional>

#include <QObject>
#include <QPointer>
#include <QString>
#include <QDateTime>

class QSettings;
class QNetworkAccessManager;
class QNetworkReply;
class QTimer;
class QWidget;

class UpdateCheck final
  : public QObject
{
  Q_OBJECT

public:
  // dialogs are parented to 'window'
  explicit UpdateCheck (QSettings * settings, QWidget * window);

  // the automatic checks: a look ~10 s from now, then one every hour;
  // 'enabled' (the Settings tick) is asked at each look
  void start_automatic (std::function<bool ()> enabled);
  void check_automatic ();   // one look: a check goes out only if due
  void check_manual ();
  bool busy () const {return reply_ != nullptr;}   // a check is under way

  // Pure: should an automatic check go out now?  Only when 'enabled', when
  // 'version' (the program's own, not the test override) does not contain
  // "dev" in any case, when no check is 'busy' and no update dialog or
  // message is open ('window_open'), when 24 h have passed since
  // 'last_success' and 1 h since 'last_failure' (the start of the last
  // failed automatic attempt).  An invalid time, or one in the future (clock
  // set back), does not hold a check off.
  static bool automatic_check_due (bool enabled, QString const& version, bool busy, bool window_open,
                                   QDateTime const& now, QDateTime const& last_success,
                                   QDateTime const& last_failure);

  // Pure: is 'candidate' a later version than 'current'?  A leading v/V is
  // dropped, both are split into integers on any run of non-digits and
  // compared as tuples, a missing part counting as 0 (1.0 == 1.0.0,
  // 0.7.10 > 0.7.2, v0.7.2 == 0.7.2).
  static bool is_newer (QString const& candidate, QString const& current);

  // the version compared against and shown: the program's own
  // (JTDXVU_VERSION), or JTDXVU_UPDATE_TEST_VERSION when that is set
  static QString current_version ();

private:
  void start (bool manual);
  void on_reply (QNetworkReply *);
  void report_failure (QString const& reason);
  void show_message (QString const& text, bool warning);
  void show_update (QString const& tag, QString const& name, QString const& url, QString const& notes);

  QSettings * settings_;
  QWidget * window_;
  QNetworkAccessManager * network_manager_ {nullptr};   // own one, see the .cpp
  QNetworkReply * reply_ {nullptr};
  QTimer * timeout_;
  QTimer * hourly_;                 // the automatic looks after the first
  std::function<bool ()> enabled_;  // Settings > General tick, from start_automatic ()
  bool manual_ {false};       // report the result of the check under way
  bool automatic_ {false};    // the check under way was started automatically
  bool timed_out_ {false};
  QDateTime started_;         // when the check under way was sent (UTC, whole seconds)
  QDateTime last_failure_;    // start of the last failed automatic attempt; never saved
  QPointer<QWidget> dialog_;  // the one dialog or message on screen
};

#endif
