// JTDX-VU (VU2CPL): tell the operator when a newer JTDX-VU release is out.
//
// One request, to GitHub's public API, nothing else:
//   GET https://api.github.com/repos/vu2cpl/jtdx-vu/releases/latest
// (no token; User-Agent JTDX-VU/<version>; 10 s timeout).  Nothing is
// downloaded or installed - the dialog's Download button only opens the
// release page in the browser.
//
// - Automatic: MainWindow calls check_automatic () ~10 s after start when
//   Settings > General "Check for updates automatically" is on.  At most
//   once per 24 h (JTDXVU/UpdateLastCheck), silent on any failure, and quiet
//   about a release the operator chose to skip (JTDXVU/UpdateSkipVersion).
// - Manual: Help > Check for Updates... calls check_manual (), which always
//   reports: the update dialog, "You're up to date", or why it failed.
//
// Dialogs are non-modal, so the main window (Halt Tx, JTTY typing) stays
// usable; the automatic one also opens without taking the keyboard focus.

#ifndef UPDATECHECK_H
#define UPDATECHECK_H

#include <QObject>
#include <QPointer>
#include <QString>

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

  void check_automatic ();
  void check_manual ();
  bool busy () const {return reply_ != nullptr;}   // a check is under way

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
  bool manual_ {false};       // report the result of the check under way
  bool timed_out_ {false};
  QPointer<QWidget> dialog_;  // the one dialog or message on screen
};

#endif
