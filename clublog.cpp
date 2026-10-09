// JTDX-VU (VU2CPL): Club Log DXCC status source and needed-DXCC alerts.
// See clublog.h.

#include "clublog.h"

#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QProcess>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QCoreApplication>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QStyle>
#include <QTimer>
#if defined (Q_OS_WIN)
# include <windows.h>
# include <wincred.h>
# include <string>
#endif

#include "logbook/logbook.h"
#include "logbook/adif.h"
#include "Radio.hpp"

namespace
{
  char const * const adif_file_name = "clublog_log.adi";
  char const * const keychain_service = "JTDX-VU";
  char const * const clublog_password_account = "clublog-app-password";
  char const * const telegram_token_account = "telegram-bot-token";
  int const stale_hours = 24;
  int const local_overlap_days = 7;     // local QSOs this long before a fetch are also counted

  // Secrets (Club Log app password, Telegram bot token) are kept out of
  // JTDX-VU.ini where the OS has a credential store: the macOS Keychain
  // (security tool driven through stdin, so they never appear in argv)
  // and the Windows Credential Manager.  Linux has no store we can rely
  // on (a Pi often has no desktop keyring), so they go in the .ini there.
#if defined (Q_OS_MAC)
  QString keychain_quote (QString s)
  {
    return '"' + s.replace ('\\', "\\\\").replace ('"', "\\\"") + '"';
  }

  QString secret_read (QSettings *, QString const& account)
  {
    QProcess p;
    p.start ("/usr/bin/security", {"find-generic-password", "-s", keychain_service, "-a", account, "-w"});
    if (!p.waitForFinished (5000) || p.exitCode () != 0) return {};
    return QString::fromUtf8 (p.readAllStandardOutput ()).trimmed ();
  }

  bool secret_write (QSettings *, QString const& account, QString const& secret)
  {
    QProcess p;
    p.start ("/usr/bin/security", {"-i"});
    if (!p.waitForStarted (5000)) return false;
    p.write (QString {"add-generic-password -U -s %1 -a %2 -w %3\n"}
             .arg (keychain_quote (keychain_service), keychain_quote (account), keychain_quote (secret)).toUtf8 ());
    p.closeWriteChannel ();
    return p.waitForFinished (5000) && p.exitCode () == 0;
  }

  QString const secret_store_name = QObject::tr ("Keychain");
#elif defined (Q_OS_WIN)
  std::wstring credential_target (QString const& account)
  {
    return (QString {keychain_service} + '/' + account).toStdWString ();
  }

  QString secret_read (QSettings *, QString const& account)
  {
    PCREDENTIALW cred {nullptr};
    auto target = credential_target (account);
    if (!CredReadW (target.c_str (), CRED_TYPE_GENERIC, 0, &cred)) return {};
    auto value = QString::fromUtf8 (reinterpret_cast<char const *> (cred->CredentialBlob), cred->CredentialBlobSize);
    CredFree (cred);
    return value;
  }

  bool secret_write (QSettings *, QString const& account, QString const& secret)
  {
    auto target = credential_target (account);
    auto user = QString {keychain_service}.toStdWString ();
    auto blob = secret.toUtf8 ();
    CREDENTIALW cred {};
    cred.Type = CRED_TYPE_GENERIC;
    cred.TargetName = const_cast<LPWSTR> (target.c_str ());
    cred.UserName = const_cast<LPWSTR> (user.c_str ());
    cred.CredentialBlobSize = static_cast<DWORD> (blob.size ());
    cred.CredentialBlob = reinterpret_cast<LPBYTE> (blob.data ());
    cred.Persist = CRED_PERSIST_LOCAL_MACHINE;
    return CredWriteW (&cred, 0);
  }

  QString const secret_store_name = QObject::tr ("Credential Manager");
#else
  QString secret_read (QSettings * settings, QString const& account)
  {
    return settings->value ("Secrets/" + account).toString ();
  }

  bool secret_write (QSettings * settings, QString const& account, QString const& secret)
  {
    settings->setValue ("Secrets/" + account, secret);
    return true;
  }

  QString const secret_store_name = QObject::tr ("settings file");
#endif

  QString data_dir ()
  {
    return QStandardPaths::writableLocation (QStandardPaths::DataLocation);
  }
}

ClubLog::ClubLog (QSettings * settings, QNetworkAccessManager * network_manager, QObject * parent)
  : QObject {parent}
  , settings_ {settings}
    // Own manager, not the shared one: EQSL (and others) connect to the
    // shared QNetworkAccessManager::finished and readAll() every reply,
    // which emptied the Club Log body before on_adif_reply saw it.
  , network_manager_ {new QNetworkAccessManager {this}}
{
  Q_UNUSED (network_manager);
  read_settings ();
}

void ClubLog::read_settings ()
{
  settings_->beginGroup ("ClubLog");
  enabled_ = settings_->value ("Enabled", false).toBool ();
  confirmed_only_ = settings_->value ("ConfirmedOnly", false).toBool ();
  email_ = settings_->value ("Email").toString ();
  callsign_ = settings_->value ("Callsign").toString ();
  last_fetch_ = settings_->value ("LastFetch").toDateTime ();
  last_fetch_.setTimeSpec (Qt::UTC);
  qso_count_ = settings_->value ("QSOCount", 0).toInt ();
  settings_->endGroup ();

  settings_->beginGroup ("Alerts");
  alert_atno_ = settings_->value ("ATNO", true).toBool ();
  alert_band_ = settings_->value ("NewBand", true).toBool ();
  alert_mode_ = settings_->value ("NewMode", settings_->value ("NewBandMode", false)).toBool ();
  macos_ = settings_->value ("macOS", false).toBool ();
  telegram_ = settings_->value ("Telegram", false).toBool ();
  telegram_chat_ = settings_->value ("TelegramChatId").toString ();
  cooldown_min_ = settings_->value ("CooldownMinutes", 15).toInt ();
  settings_->endGroup ();
}

void ClubLog::write_settings () const
{
  settings_->beginGroup ("ClubLog");
  settings_->setValue ("Enabled", enabled_);
  settings_->setValue ("ConfirmedOnly", confirmed_only_);
  settings_->setValue ("Email", email_);
  settings_->setValue ("Callsign", callsign_);
  settings_->setValue ("LastFetch", last_fetch_);
  settings_->setValue ("QSOCount", qso_count_);
  settings_->endGroup ();

  settings_->beginGroup ("Alerts");
  settings_->setValue ("ATNO", alert_atno_);
  settings_->setValue ("NewBand", alert_band_);
  settings_->setValue ("NewMode", alert_mode_);
  settings_->remove ("NewBandMode");
  settings_->setValue ("macOS", macos_);
  settings_->setValue ("Telegram", telegram_);
  settings_->setValue ("TelegramChatId", telegram_chat_);
  settings_->setValue ("CooldownMinutes", cooldown_min_);
  settings_->endGroup ();
}

QString ClubLog::adif_path () const
{
  auto path = QDir {data_dir ()}.absoluteFilePath (adif_file_name);
  return enabled_ && QFile::exists (path) ? path : QString {};
}

QString ClubLog::local_since () const
{
  if (!last_fetch_.isValid ()) return {};
  return last_fetch_.addDays (-local_overlap_days).toString ("yyyyMMddhhmmss");
}

void ClubLog::refresh_if_stale ()
{
  if (enabled_ && (!last_fetch_.isValid ()
                   || last_fetch_.secsTo (QDateTime::currentDateTimeUtc ()) >= stale_hours * 3600
                   || !QFile::exists (QDir {data_dir ()}.absoluteFilePath (adif_file_name))))
    {
      refresh ();
    }
}

void ClubLog::refresh ()
{
  if (pending_) return;
  auto password = secret_read (settings_, clublog_password_account);
  if (email_.isEmpty () || callsign_.isEmpty () || password.isEmpty ())
    {
      Q_EMIT status_message (tr ("Club Log: email, callsign and app password are required"));
      return;
    }
  QNetworkRequest request {QUrl {"https://clublog.org/getadif.php"}};
  request.setHeader (QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
  request.setHeader (QNetworkRequest::UserAgentHeader, "JTDX-VU");
  request.setAttribute (QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
  QUrlQuery form;
  form.addQueryItem ("email", email_);
  form.addQueryItem ("password", password);
  form.addQueryItem ("call", callsign_.toUpper ());
  pending_ = network_manager_->post (request, form.query (QUrl::FullyEncoded).toUtf8 ());
  connect (pending_, &QNetworkReply::finished, this, [this, reply = pending_] {on_adif_reply (reply);});
  Q_EMIT status_message (tr ("Club Log: fetching log for %1 ...").arg (callsign_.toUpper ()));
}

void ClubLog::on_adif_reply (QNetworkReply * reply)
{
  pending_ = nullptr;
  reply->deleteLater ();
  auto body = reply->readAll ();
  if (reply->error () != QNetworkReply::NoError)
    {
      auto detail = QString::fromUtf8 (body.left (200)).trimmed ();
      Q_EMIT status_message (tr ("Club Log fetch failed: %1").arg (detail.isEmpty () ? reply->errorString () : detail));
      return;
    }
  // Club Log answers bad credentials with HTTP 200 and a short plain-text message
  auto upper = body.toUpper ();
  if (body.size () < 32 || !upper.contains ("<EOR>"))
    {
      auto detail = QString::fromUtf8 (body.left (200)).trimmed ();
      Q_EMIT status_message (tr ("Club Log refused: %1").arg (detail.isEmpty () ? tr ("empty reply (HTTP %1)")
                                                                .arg (reply->attribute (QNetworkRequest::HttpStatusCodeAttribute).toInt ())
                                                              : detail));
      return;
    }

  // Rewrite as one record per line after the header, which is what the
  // JTDX ADIF reader expects.  Newlines inside a record become spaces
  // byte for byte, so ADIF field lengths stay valid.
  int start = upper.indexOf ("<EOH>");
  start = start < 0 ? 0 : start + 5;
  QByteArray out;
  out.reserve (body.size () + body.size () / 50);
  out += "JTDX-VU Club Log export " + QDateTime::currentDateTimeUtc ().toString (Qt::ISODate).toUtf8 () + "\n<EOH>\n";
  int count = 0;
  for (int end; (end = upper.indexOf ("<EOR>", start)) >= 0; start = end + 5)
    {
      auto record = body.mid (start, end - start);
      record.replace ('\r', ' ').replace ('\n', ' ');
      record = record.trimmed ();
      if (record.isEmpty ()) continue;
      out += record + "<EOR>\n";
      ++count;
    }

  QDir {}.mkpath (data_dir ());
  QSaveFile file {QDir {data_dir ()}.absoluteFilePath (adif_file_name)};
  if (!file.open (QIODevice::WriteOnly) || file.write (out) != out.size () || !file.commit ())
    {
      Q_EMIT status_message (tr ("Club Log: cannot write %1").arg (file.fileName ()));
      return;
    }
  last_fetch_ = QDateTime::currentDateTimeUtc ();
  qso_count_ = count;
  write_settings ();
  Q_EMIT status_message (status_text ());
  Q_EMIT log_updated ();
}

QString ClubLog::status_text () const
{
  if (!last_fetch_.isValid ()) return tr ("Club Log: not fetched yet");
  return tr ("Club Log: %1 QSOs, fetched %2 UTC").arg (qso_count_).arg (last_fetch_.toString ("yyyy-MM-dd hh:mm"));
}

void ClubLog::check_decode (LogBook & logbook, QString const& call, double dial_freq, QString const& mode)
{
  if (call.isEmpty () || !(macos_ || telegram_) || !(alert_atno_ || alert_band_ || alert_mode_)) return;
  dial_freq = Radio::qo100_uplink (Radio::Frequency (dial_freq));   // QO-100 is logged on 13cm, not the 3cm downlink

  QString country;
  bool worked {true}, worked_slot {true};
  logbook.matchDXCC (call, country, worked, worked_slot);
  if (country.isEmpty () || country.startsWith ("  ,?,")) return;  // unknown entity
  auto band = ADIF::bandFromFrequency (dial_freq / 1.e6);

  Level level;
  if (!worked)
    {
      if (!alert_atno_) return;
      level = ATNO;
    }
  else
    {
      logbook.matchDXCC (call, country, worked, worked_slot, dial_freq);
      if (!worked_slot)
        {
          if (!alert_band_) return;
          level = NewBand;
        }
      else
        {
          // MSHV's "new mode": never worked in this mode on any band
          logbook.matchDXCC (call, country, worked, worked_slot, 0, mode);
          if (worked_slot || !alert_mode_) return;
          level = NewMode;
        }
    }

  auto key = call + '|' + band + '|' + mode;
  auto now = QDateTime::currentDateTimeUtc ();
  auto last = last_alert_.value (key);
  if (last.isValid () && last.secsTo (now) < cooldown_min_ * 60) return;
  last_alert_[key] = now;

  notify (level, call, country.section (',', 2, 2).trimmed (), band, mode);
}

void ClubLog::notify (Level level, QString const& call, QString const& country, QString const& band, QString const& mode)
{
  QString label;
  switch (level)
    {
    case ATNO: label = QString::fromUtf8 ("\xF0\x9F\x94\xB4 ATNO"); break;          // red circle
    case NewBand: label = QString::fromUtf8 ("\xF0\x9F\x9F\xA6 New band"); break;   // blue square
    case NewMode: label = QString::fromUtf8 ("\xF0\x9F\x9F\xA7 New mode"); break; // orange square
    }
  auto title = label + ": " + call;
  auto body = country + QString::fromUtf8 (" \xC2\xB7 ") + band + QString::fromUtf8 (" \xC2\xB7 ") + mode;
  if (macos_) send_desktop (title, body);
  if (telegram_) send_telegram (title, body);
}

void ClubLog::send_desktop (QString const& title, QString const& body) const
{
#if defined (Q_OS_MAC)
  auto quote = [] (QString s) {return '"' + s.replace ('\\', "\\\\").replace ('"', "\\\"") + '"';};
  QProcess::startDetached ("/usr/bin/osascript",
                           {"-e", "display notification " + quote (body) + " with title " + quote (title)
                                  + " subtitle " + quote (QCoreApplication::applicationName ())});
#else
# if !defined (Q_OS_WIN)
  // Linux desktops (incl. Raspberry Pi OS): freedesktop notification
  auto notify_send = QStandardPaths::findExecutable ("notify-send");
  if (!notify_send.isEmpty ())
    {
      QProcess::startDetached (notify_send, {"-a", QCoreApplication::applicationName (), title, body});
      return;
    }
# endif
  // Windows, or Linux without notify-send: a system-tray balloon/toast
  if (!tray_ && QSystemTrayIcon::isSystemTrayAvailable ())
    {
      // Qt will not show a tray icon without a picture, and on Windows JTDX
      // sets its icon only through the .exe resources, so the application
      // icon is empty; fall back to the main window's, then a stock icon
      QIcon icon {QApplication::windowIcon ()};
      if (icon.isNull ())
        for (auto w : QApplication::topLevelWidgets ())
          if (!w->windowIcon ().isNull ()) {icon = w->windowIcon (); break;}
      if (icon.isNull ()) icon = QApplication::style ()->standardIcon (QStyle::SP_MessageBoxInformation);
      tray_ = new QSystemTrayIcon {icon, const_cast<ClubLog *> (this)};
      tray_->setToolTip (QCoreApplication::applicationName ());
      tray_->show ();
      // a freshly shown tray icon can drop the first message; let the
      // shell register it before showing this one
      auto tray = tray_;
      QTimer::singleShot (1000, tray, [tray, title, body] {
          tray->showMessage (title, body, QSystemTrayIcon::Information, 10000);
        });
      return;
    }
  if (tray_) tray_->showMessage (title, body, QSystemTrayIcon::Information, 10000);
#endif
}

void ClubLog::send_telegram (QString const& title, QString const& body, QWidget * report_to) const
{
  auto token = secret_read (settings_, telegram_token_account);
  if (token.isEmpty () || telegram_chat_.isEmpty ())
    {
      if (report_to) QMessageBox::warning (report_to, QCoreApplication::applicationName (), tr ("Telegram bot token and chat ID are required."));
      return;
    }
  QNetworkRequest request {QUrl {"https://api.telegram.org/bot" + token + "/sendMessage"}};
  request.setHeader (QNetworkRequest::ContentTypeHeader, "application/json");
  QJsonObject json {
    {"chat_id", telegram_chat_},
    {"text", "<b>" + title.toHtmlEscaped () + "</b>\n" + body.toHtmlEscaped ()},
    {"parse_mode", "HTML"},
  };
  auto reply = network_manager_->post (request, QJsonDocument {json}.toJson (QJsonDocument::Compact));
  connect (reply, &QNetworkReply::finished, reply, [reply, report_to] {
      reply->deleteLater ();
      if (!report_to) return;
      if (reply->error () == QNetworkReply::NoError)
        QMessageBox::information (report_to, QCoreApplication::applicationName (), tr ("Telegram test message sent."));
      else
        QMessageBox::warning (report_to, QCoreApplication::applicationName (),
                              tr ("Telegram send failed: %1").arg (QString::fromUtf8 (reply->readAll ().left (300))));
    });
}

void ClubLog::settings_dialog (QWidget * parent, QString const& my_callsign)
{
  QDialog dialog {parent};
  dialog.setWindowTitle (QCoreApplication::applicationName () + " - " + tr ("Club Log & Alerts"));

  // Club Log
  auto clublog_box = new QGroupBox {tr ("Club Log worked-before source")};
  auto enable = new QCheckBox {tr ("Use my Club Log log instead of the local log for New DXCC / Band / Mode status")};
  enable->setChecked (enabled_);
  auto email = new QLineEdit {email_};
  auto callsign = new QLineEdit {callsign_.isEmpty () ? my_callsign : callsign_};
  auto password = new QLineEdit;
  password->setEchoMode (QLineEdit::Password);
  bool have_password = !secret_read (settings_, clublog_password_account).isEmpty ();
  password->setPlaceholderText (have_password ? tr ("(saved in %1; type to replace)").arg (secret_store_name) : tr ("Club Log application password"));
  auto confirmed = new QCheckBox {tr ("Count only confirmed QSOs (QSL / LoTW / eQSL) as worked")};
  confirmed->setChecked (confirmed_only_);
  auto status = new QLabel {status_text ()};
  auto refresh_button = new QPushButton {tr ("Refresh now")};
  auto clublog_form = new QFormLayout {clublog_box};
  clublog_form->addRow (enable);
  clublog_form->addRow (tr ("Email:"), email);
  clublog_form->addRow (tr ("Callsign:"), callsign);
  clublog_form->addRow (tr ("App password:"), password);
  clublog_form->addRow (confirmed);
  auto status_row = new QHBoxLayout;
  status_row->addWidget (status, 1);
  status_row->addWidget (refresh_button);
  clublog_form->addRow (status_row);
  clublog_form->addRow (new QLabel {tr ("Fetched at start-up when older than %1 h. Local QSOs from %2 days before "
                                        "the last fetch onward are counted too.").arg (stale_hours).arg (local_overlap_days)});

  // Alerts
  auto alerts_box = new QGroupBox {tr ("Needed-DXCC alerts")};
  auto atno = new QCheckBox {tr ("ATNO (new DXCC)")};
  atno->setChecked (alert_atno_);
  auto band = new QCheckBox {tr ("New band")};
  band->setChecked (alert_band_);
  auto band_mode = new QCheckBox {tr ("New mode")};
  band_mode->setToolTip (tr ("DXCC never worked in this mode on any band (as in MSHV)"));
  band_mode->setChecked (alert_mode_);
  auto levels = new QHBoxLayout;
  levels->addWidget (atno);
  levels->addWidget (band);
  levels->addWidget (band_mode);
  levels->addStretch ();
  auto cooldown = new QSpinBox;
  cooldown->setRange (5, 60);
  cooldown->setSuffix (tr (" min"));
  cooldown->setValue (cooldown_min_);
  auto macos = new QCheckBox {tr ("Desktop notification")};
  macos->setChecked (macos_);
  auto macos_test = new QPushButton {tr ("Send test")};
  auto macos_row = new QHBoxLayout;
  macos_row->addWidget (macos, 1);
  macos_row->addWidget (macos_test);
  auto telegram = new QCheckBox {tr ("Telegram")};
  telegram->setChecked (telegram_);
  auto token = new QLineEdit;
  token->setEchoMode (QLineEdit::Password);
  bool have_token = !secret_read (settings_, telegram_token_account).isEmpty ();
  token->setPlaceholderText (have_token ? tr ("(saved in %1; type to replace)").arg (secret_store_name) : tr ("bot token from @BotFather"));
  auto chat = new QLineEdit {telegram_chat_};
  auto telegram_test = new QPushButton {tr ("Send test")};
  auto telegram_row = new QHBoxLayout;
  telegram_row->addWidget (telegram, 1);
  telegram_row->addWidget (telegram_test);
  auto alerts_form = new QFormLayout {alerts_box};
  alerts_form->addRow (tr ("Alert on:"), levels);
  alerts_form->addRow (tr ("Repeat per call/band after:"), cooldown);
  alerts_form->addRow (macos_row);
  alerts_form->addRow (telegram_row);
  alerts_form->addRow (tr ("Bot token:"), token);
  alerts_form->addRow (tr ("Chat ID:"), chat);

  auto buttons = new QDialogButtonBox {QDialogButtonBox::Ok | QDialogButtonBox::Cancel};
  auto layout = new QVBoxLayout {&dialog};
  layout->addWidget (clublog_box);
  layout->addWidget (alerts_box);
  layout->addWidget (buttons);
  connect (buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
  connect (buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

  // apply the fields to the live state; used by OK and by the test/refresh buttons
  auto apply = [&] {
    auto source_changed = enable->isChecked () != enabled_ || confirmed->isChecked () != confirmed_only_;
    auto account_changed = email->text ().trimmed () != email_ || callsign->text ().trimmed ().toUpper () != callsign_
      || !password->text ().isEmpty ();
    enabled_ = enable->isChecked ();
    confirmed_only_ = confirmed->isChecked ();
    email_ = email->text ().trimmed ();
    callsign_ = callsign->text ().trimmed ().toUpper ();
    if (!password->text ().isEmpty () && secret_write (settings_, clublog_password_account, password->text ()))
      {
        password->clear ();
        password->setPlaceholderText (tr ("(saved in %1; type to replace)").arg (secret_store_name));
      }
    if (!token->text ().isEmpty () && secret_write (settings_, telegram_token_account, token->text ()))
      {
        token->clear ();
        token->setPlaceholderText (tr ("(saved in %1; type to replace)").arg (secret_store_name));
      }
    alert_atno_ = atno->isChecked ();
    alert_band_ = band->isChecked ();
    alert_mode_ = band_mode->isChecked ();
    cooldown_min_ = cooldown->value ();
    macos_ = macos->isChecked ();
    telegram_ = telegram->isChecked ();
    telegram_chat_ = chat->text ().trimmed ();
    write_settings ();
    if (account_changed && enabled_) refresh ();
    else if (source_changed) Q_EMIT log_updated ();
  };

  auto status_connection = connect (this, &ClubLog::status_message, status, &QLabel::setText);
  connect (refresh_button, &QPushButton::clicked, &dialog, [&] {apply (); refresh ();});
  connect (macos_test, &QPushButton::clicked, &dialog, [&] {
      send_desktop (QString::fromUtf8 ("\xF0\x9F\x94\xB4 ATNO: TEST"), tr ("Test alert from %1").arg (QCoreApplication::applicationName ()));
    });
  connect (telegram_test, &QPushButton::clicked, &dialog, [&] {
      apply ();
      send_telegram (QString::fromUtf8 ("\xF0\x9F\x94\xB4 ATNO: TEST"), tr ("Test alert from %1").arg (QCoreApplication::applicationName ()), &dialog);
    });

  if (dialog.exec () == QDialog::Accepted) apply ();
  disconnect (status_connection);
}
