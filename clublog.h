// JTDX-VU (VU2CPL): Club Log DXCC status source and needed-DXCC alerts.
//
// Club Log's ADIF export of the operator's log replaces the local
// wsjtx_log.adi as JTDX's worked-before source (so the existing New DXCC /
// Band / Band+Mode colouring, filters and priorities follow Club Log), and
// decodes of needed entities raise macOS and/or Telegram alerts.
// Concept and API usage ported from the MSHV-Mac private build.

#ifndef CLUBLOG_H
#define CLUBLOG_H

#include <QObject>
#include <QHash>
#include <QDateTime>
#include <QString>

class QSettings;
class QNetworkAccessManager;
class QNetworkReply;
class QWidget;
class LogBook;
class QSystemTrayIcon;

class ClubLog final
  : public QObject
{
  Q_OBJECT

public:
  explicit ClubLog (QSettings * settings, QNetworkAccessManager * network_manager, QObject * parent = nullptr);

  // worked-before source, for LogBook::init
  bool enabled () const {return enabled_;}
  bool confirmed_only () const {return confirmed_only_;}
  QString adif_path () const;           // cached Club Log export, "" when not in use
  QString local_since () const;         // yyyyMMddhhmmss: local QSOs from here on are added

  void refresh ();                      // fetch now
  void refresh_if_stale ();             // fetch when enabled and cache is older than 24 h

  // alert on a decode from 'call' if its DXCC is needed on this band / mode
  void check_decode (LogBook & logbook, QString const& call, double dial_freq, QString const& mode);

  void settings_dialog (QWidget * parent, QString const& my_callsign);

  Q_SIGNAL void log_updated () const;   // cache replaced or source settings changed
  Q_SIGNAL void status_message (QString const&) const;

private:
  enum Level {ATNO, NewBand, NewMode};  // as in MSHV

  void read_settings ();
  void write_settings () const;
  void on_adif_reply (QNetworkReply *);
  void notify (Level, QString const& call, QString const& country, QString const& band, QString const& mode);
  void send_desktop (QString const& title, QString const& body) const;  // macOS banner / Windows tray / Linux notify-send
  void send_telegram (QString const& title, QString const& body, QWidget * report_to = nullptr) const;
  QString status_text () const;

  QSettings * settings_;
  QNetworkAccessManager * network_manager_;
  QNetworkReply * pending_ {nullptr};

  // Club Log
  bool enabled_ {false};
  bool confirmed_only_ {false};
  QString email_;
  QString callsign_;
  QDateTime last_fetch_;                // UTC
  int qso_count_ {0};

  // alerts
  bool alert_atno_ {true};
  bool alert_band_ {true};
  bool alert_mode_ {false};
  bool macos_ {false};
  bool telegram_ {false};
  QString telegram_chat_;
  int cooldown_min_ {15};
  QHash<QString, QDateTime> last_alert_;
  mutable QSystemTrayIcon * tray_ {nullptr};  // Windows / Linux fallback
};

#endif
