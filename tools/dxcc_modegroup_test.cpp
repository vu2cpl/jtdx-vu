// JTDX-VU DXCC "per mode" check: ADIF::dxccModeGroup and
// ADIF::matchCountryModeGroup, which the Club Log "New mode" alert, the
// decode window's "New DXCC on Mode" colour and the auto-seq wanted rank use.
// A confirmed RTTY QSO must cover FT8 (both are Data for DXCC), as MSHV and
// DXCA count it; the old exact-name check called Cameroon on FT8 a new mode.
//
// Build (macOS, Homebrew qt@5):
//   Q=/opt/homebrew/opt/qt@5; M=$(mktemp -d)
//   clang++ -std=c++17 -DQT5 -I. -F$Q/lib -I$Q/lib/QtCore.framework/Headers \
//     -framework QtCore -Wl,-rpath,$Q/lib \
//     tools/dxcc_modegroup_test.cpp logbook/adif.cpp logbook/countrydat.cpp Radio.cpp \
//     -o $M/dxcc_modegroup_test
// Build (Linux):
//   M=$(mktemp -d)
//   g++ -std=c++17 -fPIC -DQT5 -I. tools/dxcc_modegroup_test.cpp logbook/adif.cpp \
//     logbook/countrydat.cpp Radio.cpp $(pkg-config --cflags --libs Qt5Core) \
//     -o $M/dxcc_modegroup_test
// Run: $M/dxcc_modegroup_test path/to/cty.dat
//   (the cty.dat shipped in the app's Resources/jtdx; prints each check,
//   exits 1 if any failed)

#include <QCoreApplication>
#include <QFile>
#include <QString>
#include <QTemporaryDir>
#include <cstdio>

#include "logbook/adif.h"
#include "logbook/countrydat.h"

namespace
{
  int failures = 0;

  void check (bool ok, char const * what)
  {
    std::printf ("%s  %s\n", ok ? "ok  " : "FAIL", what);
    if (!ok) ++failures;
  }

  // a Club Log style export: header, then one record per line
  QByteArray const export_adif =
    "test export\n<EOH>\n"
    // Cameroon: phone, CW and RTTY confirmed by card, FT8 not confirmed
    "<CALL:5>TJ2TT<QSO_DATE:8>20180316<TIME_ON:4>1200<BAND:3>20M<MODE:3>SSB<QSL_RCVD:1>V<EOR>\n"
    "<CALL:5>TJ2TT<QSO_DATE:8>20180316<TIME_ON:4>1300<BAND:3>17M<MODE:2>CW<QSL_RCVD:1>V<EOR>\n"
    "<CALL:5>TJ2TT<QSO_DATE:8>20180316<TIME_ON:4>1400<BAND:3>20M<MODE:4>RTTY<QSL_RCVD:1>V<EOR>\n"
    "<CALL:5>TJ1GD<QSO_DATE:8>20261110<TIME_ON:4>1500<BAND:3>20M<MODE:3>FT8<QSL_RCVD:1>N<EOR>\n"
    // Mongolia: SSB only, confirmed
    "<CALL:5>JT1CO<QSO_DATE:8>20200101<TIME_ON:4>1000<BAND:3>20M<MODE:3>SSB<LOTW_QSL_RCVD:1>Y<EOR>\n"
    // Chad: FT4 (MFSK + SUBMODE), confirmed
    "<CALL:5>TT8XX<QSO_DATE:8>20240101<TIME_ON:4>1000<BAND:3>15M<MODE:4>MFSK<SUBMODE:3>FT4<LOTW_QSL_RCVD:1>Y<EOR>\n";
}

int main (int argc, char * argv[])
{
  QCoreApplication app {argc, argv};
  if (argc < 2 || !QFile::exists (argv[1]))
    {
      std::fprintf (stderr, "usage: %s path/to/cty.dat\n", argv[0]);
      return 2;
    }

  // the groups themselves
  check (ADIF::dxccModeGroup ("FT8") == "DATA", "FT8 is Data");
  check (ADIF::dxccModeGroup ("rtty") == "DATA", "RTTY is Data (case-insensitive)");
  check (ADIF::dxccModeGroup ("JTTY") == "DATA", "JTTY is Data");
  check (ADIF::dxccModeGroup ("CW") == "CW", "CW is CW");
  check (ADIF::dxccModeGroup ("SSB") == "PHONE", "SSB is Phone");
  check (ADIF::dxccModeGroup ("FM") == "PHONE", "FM is Phone");
  check (ADIF::dxccModeGroup ("DIGITALVOICE") == "PHONE", "digital voice is Phone");
  check (ADIF::dxccModeGroup ("").isEmpty (), "no mode, no group");

  QTemporaryDir dir;
  auto const path = dir.filePath ("clublog_log.adi");
  QFile f {path};
  if (!f.open (QIODevice::WriteOnly) || f.write (export_adif) != export_adif.size ()) return 2;
  f.close ();

  CountryDat countries;
  countries.init (argv[1], "");
  countries.load ();
  auto entity = [&] (char const * call) {
    auto const c = countries.find (call).split (',');
    return c.size () < 3 ? QString {} : c[0] + ',' + c[1] + ',' + c[2];
  };
  auto const cameroon = entity ("TJ1GD");
  auto const mongolia = entity ("JT1CO");
  auto const chad = entity ("TT8XX");
  check (!cameroon.isEmpty () && !mongolia.isEmpty () && !chad.isEmpty (), "cty.dat resolves the test calls");

  // Club Log with "Confirmed only" ticked: the unconfirmed TJ1GD FT8 is skipped
  ADIF adif;
  adif.init ("", &countries);
  adif.setClubLog (path, true, "");
  adif.load ("", "", "");

  check (adif.matchCountry (cameroon, "", ""), "Cameroon worked");
  check (!adif.matchCountry (cameroon, "", "FT8"), "exact name: no confirmed FT8 with Cameroon (the old check)");
  check (adif.matchCountryModeGroup (cameroon, "FT8"), "Cameroon FT8 covered by confirmed RTTY (TJ1GD on 12m: no New mode)");
  check (adif.matchCountryModeGroup (cameroon, "FT4"), "Cameroon FT4 covered by confirmed RTTY");
  check (adif.matchCountryModeGroup (cameroon, "CW"), "Cameroon CW confirmed");
  check (adif.matchCountryModeGroup (cameroon, "USB"), "Cameroon USB covered by confirmed SSB");
  check (!adif.matchCountryModeGroup (mongolia, "FT8"), "Mongolia, SSB only: FT8 is still a new mode");
  check (!adif.matchCountryModeGroup (mongolia, "CW"), "Mongolia, SSB only: CW is still a new mode");
  check (adif.matchCountryModeGroup (chad, "FT8"), "Chad: FT4 (MFSK submode) covers FT8");
  check (adif.matchCountryModeGroup (mongolia, ""), "no mode: nothing to flag");

  // all QSOs: the unconfirmed TJ1GD FT8 counts as well
  ADIF all;
  all.init ("", &countries);
  all.setClubLog (path, false, "");
  all.load ("", "", "");
  check (all.matchCountry (cameroon, "", "FT8"), "all QSOs: Cameroon FT8 by exact name");
  check (all.matchCountryModeGroup (cameroon, "FT8"), "all QSOs: Cameroon FT8 by group");

  // a QSO logged now (ADIF::add) feeds the group too
  all.add ("JT1CO", "40m", "FT8", "20261117", "", "");
  check (all.matchCountryModeGroup (mongolia, "RTTY"), "after logging FT8, Mongolia RTTY is covered");

  std::printf (failures ? "%d FAILED\n" : "all passed\n", failures);
  return failures ? 1 : 0;
}
