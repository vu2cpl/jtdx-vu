#ifndef RADIO_HPP_
#define RADIO_HPP_

#include <QObject>
#include <QLocale>

class QVariant;
class QString;

//
// Declarations common to radio software.
//

namespace Radio
{
  //
  // Frequency types
  //
  using Frequency = quint64;
  using Frequencies = QList<Frequency>;
  using FrequencyDelta = qint64;

  //
  // Frequency type conversion.
  //
  //	QVariant argument is convertible to double and is assumed to
  //	be scaled by (10 ** -scale).
  //
  Frequency frequency (QVariant const&, int scale, QLocale const& = QLocale ());
  FrequencyDelta frequency_delta (QVariant const&, int scale, QLocale const& = QLocale ());

  //
  // Frequency type formatting
  //
  QString frequency_MHz_string (Frequency, QLocale const& = QLocale ());
  QString frequency_MHz_string (FrequencyDelta, QLocale const& = QLocale ());
  QString pretty_frequency_MHz_string (Frequency, QLocale const& = QLocale ());
  QString pretty_frequency_MHz_string (double, int scale, QLocale const& = QLocale ());
  QString pretty_frequency_MHz_string (FrequencyDelta, QLocale const& = QLocale ());

  //
  // Callsigns
  //
  bool is_callsign (QString const&);
  bool is_compound_callsign (QString const&);
  QString base_callsign (QString);
  QString effective_prefix (QString);
  QString striped_prefix (QString);
  // JTDX-VU: QO-100 (Es'hail-2) narrow-band transponder.  The rig / SDR
  // program reports the downlink (10489.5-10490 MHz); the uplink is 8089.5 MHz
  // lower (2400.0-2400.5 MHz).  QSOs log the uplink as FREQ / BAND (13cm) and
  // the downlink as FREQ_RX / BAND_RX (3cm), PROP_MODE SAT, SAT_NAME QO-100.
  constexpr Frequency qo100_offset {8089500000ull};
  constexpr Frequency qo100_ft8 {10489540000ull};     // FT8, downlink (uplink 2400.040)
  // the dial may show either side: the downlink (Rx, 10489.5-10490 MHz) or
  // the uplink (Tx, 2400.0-2400.5 MHz)
  inline bool is_qo100_down (Frequency f) {return f >= 10489500000ull && f <= 10490000000ull;}
  inline bool is_qo100_up (Frequency f) {return f >= 2400000000ull && f <= 2400500000ull;}
  inline bool is_qo100 (Frequency f) {return is_qo100_down (f) || is_qo100_up (f);}
  inline Frequency qo100_uplink (Frequency f) {return is_qo100_down (f) ? f - qo100_offset : f;}
  inline Frequency qo100_downlink (Frequency f) {return is_qo100_up (f) ? f + qo100_offset : f;}
  // Darkstyle Color
  QString convert_dark(QString const& color, bool useDarkStyle);
  QString convert_Smeter(int level, bool Sunits = true);
}

Q_DECLARE_METATYPE (Radio::Frequency);
Q_DECLARE_METATYPE (Radio::Frequencies);
Q_DECLARE_METATYPE (Radio::FrequencyDelta);

#endif
