// JTDX-VU (VU2CPL): tell the operator when a newer JTDX-VU release is out.
// See updatecheck.h.

#include "updatecheck.h"

#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QRegularExpression>
#include <QDateTime>
#include <QTimer>
#include <QUrl>
#include <QDesktopServices>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QFont>

#include "revision_utils.hpp"

namespace
{
  char const * const latest_release_api = "https://api.github.com/repos/vu2cpl/jtdx-vu/releases/latest";
  char const * const releases_page = "https://github.com/vu2cpl/jtdx-vu/releases/latest";
  int const timeout_ms = 10 * 1000;
  int const first_look_ms = 10 * 1000;           // the automatic look after start
  int const look_interval_ms = 3600 * 1000;      // then one every hour while running
  int const automatic_interval_s = 24 * 3600;    // since the last successful check
  int const failure_backoff_s = 3600;            // since a failed automatic attempt

  // settings (JTDX-VU's own group, beside NonStop, WantedShow, ...)
  char const * const last_check_key = "JTDXVU/UpdateLastCheck";    // UTC, ISO 8601; last SUCCESSFUL check (sent at)
  char const * const skip_version_key = "JTDXVU/UpdateSkipVersion";  // tag_name the operator skipped

  // "v0.7.10" -> (0, 7, 10); "1.0" -> (1, 0); nothing numeric -> ()
  QList<qlonglong> version_parts (QString v)
  {
    v = v.trimmed ();
    if (v.startsWith ('v', Qt::CaseInsensitive)) v.remove (0, 1);
    QList<qlonglong> parts;
    for (auto const& part : v.split (QRegularExpression {"[^0-9]+"}))
      {
        if (!part.isEmpty ()) parts << part.toLongLong ();
      }
    return parts;
  }

  // the browser only ever goes to GitHub, whatever the reply says
  QUrl release_page (QString const& html_url)
  {
    QUrl const url {html_url};
    if (url.isValid () && url.scheme () == "https" && url.host () == "github.com") return url;
    return QUrl {releases_page};
  }
}

bool UpdateCheck::automatic_check_due (bool enabled, QString const& version, bool busy, bool window_open,
                                       QDateTime const& now, QDateTime const& last_success,
                                       QDateTime const& last_failure)
{
  if (!enabled || busy || window_open) return false;   // never two checks, never a second dialog
  if (version.contains ("dev", Qt::CaseInsensitive)) return false;   // development build: Help menu only
  // a time in the future (clock set back) does not hold a check off
  auto const within = [&now] (QDateTime const& then, int seconds) {
      return then.isValid () && then <= now && then.secsTo (now) < seconds;
    };
  return !within (last_success, automatic_interval_s) && !within (last_failure, failure_backoff_s);
}

bool UpdateCheck::is_newer (QString const& candidate, QString const& current)
{
  auto const a = version_parts (candidate);
  auto const b = version_parts (current);
  for (int i = 0; i < qMax (a.size (), b.size ()); ++i)
    {
      auto const x = i < a.size () ? a[i] : 0;
      auto const y = i < b.size () ? b[i] : 0;
      if (x != y) return x > y;
    }
  return false;
}

QString UpdateCheck::current_version ()
{
  // Test hook, inert unless set: JTDXVU_UPDATE_TEST_VERSION=0.0.1 makes the
  // check compare against, and show, 0.0.1 instead of this build's own
  // version, so the "newer release" dialog can be seen against the live API.
  // The User-Agent still carries the real version.
  auto const test = QString::fromLocal8Bit (qgetenv ("JTDXVU_UPDATE_TEST_VERSION")).trimmed ();
  return test.isEmpty () ? jtdxvu_version () : test;
}

UpdateCheck::UpdateCheck (QSettings * settings, QWidget * window)
  : QObject {window}
  , settings_ {settings}
  , window_ {window}
  , timeout_ {new QTimer {this}}
  , hourly_ {new QTimer {this}}
{
  // Precise: Qt runs a coarse timer this long as a very coarse one, which
  // may fire up to half a second early - and a look just short of 24 h (or
  // 1 h) after a check would leave it to the next look, an hour later
  hourly_->setTimerType (Qt::PreciseTimer);
  hourly_->setInterval (look_interval_ms);
  connect (hourly_, &QTimer::timeout, this, &UpdateCheck::check_automatic);
  timeout_->setSingleShot (true);
  timeout_->setInterval (timeout_ms);
  connect (timeout_, &QTimer::timeout, this, [this] {
      if (reply_)
        {
          timed_out_ = true;
          reply_->abort ();   // finished () follows, on_reply reports the timeout
        }
    });
}

void UpdateCheck::start_automatic (std::function<bool ()> enabled)
{
  enabled_ = std::move (enabled);
  QTimer::singleShot (first_look_ms, this, [this] {
      check_automatic ();
      // the hourly looks count from here, just after the time this check is
      // stamped with, so a check due 24 h (or a retry due 1 h) later is due
      // by the tick that comes then
      hourly_->start ();
    });
}

void UpdateCheck::check_automatic ()
{
  // JTDX-VU stays open for days: this runs at start and every hour, and
  // the pure automatic_check_due () decides.  The development-build test is
  // on the program's own version, not the JTDXVU_UPDATE_TEST_VERSION one.
  if (automatic_check_due (enabled_ && enabled_ (), jtdxvu_version (), busy (), !dialog_.isNull (),
                           QDateTime::currentDateTimeUtc (),
                           QDateTime::fromString (settings_->value (last_check_key).toString (), Qt::ISODate),
                           last_failure_))
    {
      start (false);
    }
}

void UpdateCheck::check_manual ()
{
  if (reply_)
    {
      manual_ = true;   // the automatic check under way reports its result instead
      return;
    }
  start (true);
}

void UpdateCheck::start (bool manual)
{
  // Own manager, not MainWindow's shared one: EQSL and WSPRnet connect to the
  // shared QNetworkAccessManager::finished and readAll () every reply (see
  // ClubLog's constructor).
  if (!network_manager_) network_manager_ = new QNetworkAccessManager {this};
  manual_ = manual;
  automatic_ = !manual;
  timed_out_ = false;
  // whole seconds, as UpdateLastCheck is stored: never later than the
  // moment sent, so "24 h since" is never short of 24 h
  started_ = QDateTime::currentDateTimeUtc ();
  started_ = started_.addMSecs (-started_.time ().msec ());
  QNetworkRequest request {QUrl {latest_release_api}};
  request.setRawHeader ("Accept", "application/vnd.github+json");
  request.setHeader (QNetworkRequest::UserAgentHeader, QString {"JTDX-VU/" + jtdxvu_version ()});
  request.setAttribute (QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
  auto * reply = network_manager_->get (request);
  reply_ = reply;
  connect (reply, &QNetworkReply::finished, this, [this, reply] {on_reply (reply);});
  timeout_->start ();
}

void UpdateCheck::on_reply (QNetworkReply * reply)
{
  timeout_->stop ();
  reply_ = nullptr;
  reply->deleteLater ();
  if (timed_out_)   // aborted: nothing to read
    {
      report_failure (tr ("no answer from GitHub within %1 s").arg (timeout_ms / 1000));
      return;
    }
  auto const body = reply->readAll ();
  auto const status = reply->attribute (QNetworkRequest::HttpStatusCodeAttribute).toInt ();
  if (reply->error () != QNetworkReply::NoError)
    {
      if (status >= 400)
        {
          // GitHub says why in a JSON "message" (e.g. the hourly rate limit);
          // keep its first sentence, not the advice in brackets after it
          auto message = QJsonDocument::fromJson (body).object ().value ("message").toString ().simplified ();
          auto const bracket = message.indexOf (" (");
          if (bracket > 0) message.truncate (bracket);
          if (message.size () > 160) message = message.left (157) + "...";
          if (message.isEmpty ())
            message = reply->attribute (QNetworkRequest::HttpReasonPhraseAttribute).toString ().trimmed ();
          report_failure (message.isEmpty () ? tr ("HTTP %1").arg (status)
                                             : tr ("HTTP %1 - %2").arg (status).arg (message));
        }
      else
        {
          report_failure (reply->errorString ());
        }
      return;
    }
  if (status != 200)   // a success is a 200 with a release in it, nothing else
    {
      report_failure (tr ("HTTP %1").arg (status));
      return;
    }

  QJsonParseError error;
  auto const document = QJsonDocument::fromJson (body, &error);
  auto const release = document.object ();
  auto const tag = release.value ("tag_name").toString ().trimmed ();
  if (error.error != QJsonParseError::NoError || !document.isObject () || tag.isEmpty ())
    {
      report_failure (tr ("unexpected reply from GitHub"));
      return;
    }

  // A successful check, automatic or manual, newer release or not: the only
  // place UpdateLastCheck is written (with the time it was sent)
  settings_->setValue (last_check_key, started_.toString (Qt::ISODate));
  last_failure_ = QDateTime {};

  auto const current = current_version ();
  if (is_newer (tag, current))
    {
      if (!manual_ && tag == settings_->value (skip_version_key).toString ()) return;   // skipped by the operator
      show_update (tag, release.value ("name").toString ().trimmed (),
                   release.value ("html_url").toString (), release.value ("body").toString ());
    }
  else if (manual_)
    {
      show_message (tr ("You're up to date (JTDX-VU %1)").arg (current), false);
    }
}

void UpdateCheck::report_failure (QString const& reason)
{
  // Nothing is saved, so the next start tries again.  A failed automatic
  // attempt holds the next automatic one off for an hour (in memory only); a
  // manual one changes nothing.
  if (automatic_) last_failure_ = started_;
  // automatic checks stay silent: offline, a timeout or the rate limit is
  // nothing the operator needs to hear about
  if (manual_) show_message (tr ("Couldn't check for updates: %1").arg (reason), true);
}

void UpdateCheck::show_message (QString const& text, bool warning)
{
  if (dialog_) dialog_->close ();
  auto * box = new QMessageBox {warning ? QMessageBox::Warning : QMessageBox::Information,
                                tr ("JTDX-VU Update"), text, QMessageBox::Ok, window_};
  box->setObjectName ("jtdxvuUpdateMessage");
  box->setAttribute (Qt::WA_DeleteOnClose);
  box->setWindowModality (Qt::NonModal);   // a message box is modal by default
  dialog_ = box;
  box->show ();
  box->raise ();
  box->activateWindow ();
}

void UpdateCheck::show_update (QString const& tag, QString const& name, QString const& url, QString const& notes)
{
  if (dialog_) dialog_->close ();
  auto const latest = tag.startsWith ('v', Qt::CaseInsensitive) ? tag.mid (1) : tag;

  auto * dialog = new QDialog {window_};
  dialog->setObjectName ("jtdxvuUpdateDialog");
  dialog->setWindowTitle (tr ("JTDX-VU Update"));
  dialog->setAttribute (Qt::WA_DeleteOnClose);
  auto * layout = new QVBoxLayout {dialog};

  auto * heading = new QLabel {tr ("JTDX-VU %1 is available — you have %2.").arg (latest, current_version ())};
  heading->setObjectName ("heading");
  heading->setTextFormat (Qt::PlainText);
  auto font = heading->font ();
  font.setBold (true);
  heading->setFont (font);
  layout->addWidget (heading);
  // the release title, when it says more than "JTDX-VU v0.7.3"
  auto rest = name;
  rest.remove ("JTDX-VU", Qt::CaseInsensitive);
  rest.remove (tag);
  rest.remove (latest);
  if (rest.contains (QRegularExpression {"\\w"}))
    {
      auto * title = new QLabel {name};
      title->setObjectName ("name");
      title->setTextFormat (Qt::PlainText);
      title->setWordWrap (true);
      layout->addWidget (title);
    }

  auto * text = new QPlainTextEdit;
  text->setObjectName ("notes");
  text->setReadOnly (true);
  auto plain = notes;
  plain.replace ("\r\n", "\n");
  text->setPlainText (plain.trimmed ().isEmpty () ? tr ("(No release notes.)") : plain.trimmed ());
  text->setMinimumSize (560, 320);
  layout->addWidget (text, 1);

  // Skip on the left, Remind Me Later and Download on the right.  Not a
  // QDialogButtonBox: that makes Download the default button when shown, and
  // Return must never open the browser (Esc = Remind Me Later).
  auto * skip = new QPushButton {tr ("Skip This Version")};
  auto * later = new QPushButton {tr ("Remind Me Later")};
  auto * download = new QPushButton {tr ("Download")};
  skip->setObjectName ("skip");
  later->setObjectName ("later");
  download->setObjectName ("download");
  auto * row = new QHBoxLayout;
  row->addWidget (skip);
  row->addStretch (1);
  row->addWidget (later);
  row->addWidget (download);
  for (auto * button : {skip, later, download})
    {
      button->setAutoDefault (false);
      button->setDefault (false);
    }
  layout->addLayout (row);

  connect (download, &QPushButton::clicked, dialog, [dialog, url] {
      QDesktopServices::openUrl (release_page (url));   // the release page; nothing is downloaded here
      dialog->close ();
    });
  connect (skip, &QPushButton::clicked, dialog, [this, dialog, tag] {
      settings_->setValue (skip_version_key, tag);
      dialog->close ();
    });
  connect (later, &QPushButton::clicked, dialog, &QDialog::close);

  dialog_ = dialog;
  if (manual_)
    {
      dialog->show ();
      dialog->raise ();
      dialog->activateWindow ();
    }
  else
    {
      // it pops up on its own: leave the keyboard where the operator is
      // working (JTTY typing, Halt Tx)
      dialog->setAttribute (Qt::WA_ShowWithoutActivating);
      dialog->show ();
    }
}
