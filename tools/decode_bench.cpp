// JTDX-VU decode benchmark: feeds recorded 15 s FT8 .wav files (12 kHz mono 16-bit,
// e.g. WSJT-X's save folder) to a real jtdxjt9 through the same shared memory and
// .lock handshake MainWindow uses, and times each decode.
//
// Build (macOS, Homebrew qt@5):
//   Q=/opt/homebrew/opt/qt@5/lib
//   clang++ -std=c++17 -O2 -I. -F$Q -I$Q/QtCore.framework/Headers -framework QtCore \
//     -Wl,-rpath,$Q tools/decode_bench.cpp -o decode_bench
// Build (Linux): g++ -std=c++17 -O2 -I. $(pkg-config --cflags --libs Qt5Core) -fPIC \
//     tools/decode_bench.cpp -o decode_bench
// Run:
//   ./decode_bench /Applications/JTDX-VU.app/Contents/MacOS/jtdxjt9 <threads|0=auto> a.wav b.wav ...
// Prints one CSV line per file: file,wall_ms,cpu_ms,decodes  (then a summary).
// DECODE_BENCH_SHOW=1 also prints every decode to stderr, to compare two builds' output.
// Settings follow Manoj's profiles: depth 3, FT8 cycles 3, sensitivity 2 (subpass),
// AGC compensation, hint on, filter off, 200-3000 Hz.

#include <QCoreApplication>
#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QSharedMemory>
#include <QTemporaryDir>
#include <QThread>
#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>
#ifdef __APPLE__
#include <libproc.h>
#include <mach/mach_time.h>
#else
#include <unistd.h>
#endif

#include "commons.h"

static dec_data_t dd;

static bool read_wav (QString const& path, std::vector<short>& out)
{
  QFile f {path};
  if (!f.open (QIODevice::ReadOnly)) return false;
  QByteArray b = f.readAll ();
  if (b.size () < 12 || !b.startsWith ("RIFF") || b.mid (8, 4) != "WAVE") return false;
  int p = 12;
  while (p + 8 <= b.size ())
    {
      QByteArray id = b.mid (p, 4);
      quint32 len; std::memcpy (&len, b.constData () + p + 4, 4);
      if (id == "data")
        {
          int n = std::min<int> (len, b.size () - p - 8) / 2;
          out.assign (reinterpret_cast<short const*> (b.constData () + p + 8),
                      reinterpret_cast<short const*> (b.constData () + p + 8) + n);
          return true;
        }
      p += 8 + len + (len & 1);
    }
  return false;
}

static double cpu_ms (qint64 pid)
{
#ifdef __APPLE__
  rusage_info_v2 ri;
  if (proc_pid_rusage (static_cast<int> (pid), RUSAGE_INFO_V2, reinterpret_cast<rusage_info_t*> (&ri))) return -1;
  mach_timebase_info_data_t tb; mach_timebase_info (&tb);
  return double (ri.ri_user_time + ri.ri_system_time) * tb.numer / tb.denom / 1e6;
#else
  QFile f {QString {"/proc/%1/stat"}.arg (pid)};
  if (!f.open (QIODevice::ReadOnly)) return -1;
  auto parts = QString {f.readAll ()}.section (')', 1).split (' ', Qt::SkipEmptyParts);
  return (parts.value (11).toDouble () + parts.value (12).toDouble ()) * 1000.0 / sysconf (_SC_CLK_TCK);
#endif
}

static void set_params (int nutc, int threads)
{
  auto& p = dd.params;
  auto pad = [] (char* dst, char const* s, int n) { std::memset (dst, ' ', n); std::memcpy (dst, s, std::min<int> (n, std::strlen (s))); };
  pad (p.mycall, "VU2CPL", 12); pad (p.mybcall, "VU2CPL", 12);
  pad (p.hiscall, "", 12); pad (p.hisbcall, "", 12); pad (p.hisgrid, "", 6);
  p.napwid = 5; p.nQSOProgress = 0; p.nftx = 1500; p.nutc = nutc; p.ntrperiod = 15;
  p.nfqso = 1500; p.nfa = 200; p.nfSplit = 2500; p.nfb = 3000; p.ntol = 50;
  p.ndepth = 3; p.ncandthin = 100; p.ndtcenter = 0; p.nft8cycles = 3; p.nft8swlcycles = 3;
  p.nmode = 8; p.nranera = 3; p.ntrials10 = 1; p.ntrialsrxf10 = 1; p.naggressive = 1;
  p.nprepass = 4; p.ntopfreq65 = 5000; p.nmt = threads; p.nft8rxfsens = 3; p.nft4depth = 3;
  p.ndiskdat = true; p.newdat = true; p.nagain = false; p.nagainfil = false; p.nswl = false;
  p.nfilter = false; p.nstophint = true; p.nagcc = true; p.nhint = true;
  p.lft8lowth = true; p.lft8subpass = true; p.lhideft8dupes = false;
  p.lmycallstd = true; p.lhiscallstd = false; p.lwidedxcsearch = false;
}

int main (int argc, char* argv[])
{
  QCoreApplication app {argc, argv};
  auto args = app.arguments ();
  if (args.size () < 4) { std::fprintf (stderr, "usage: decode_bench <jtdxjt9> <threads|0> file.wav...\n"); return 1; }
  QString const exe = args[1];
  int const threads = args[2].toInt ();
  bool const show = qEnvironmentVariableIsSet ("DECODE_BENCH_SHOW");
  QTemporaryDir tmp;
  QDir t {tmp.path ()};
  QString const key = QString {"JTDXVU-bench-%1"}.arg (QCoreApplication::applicationPid ());
  QSharedMemory shm {key};
  if (!shm.create (sizeof (dec_data_t))) { std::fprintf (stderr, "shm: %s\n", qPrintable (shm.errorString ())); return 1; }
  QFile {t.filePath (".lock")}.open (QIODevice::ReadWrite);

  QProcess jt9;
  auto env = QProcessEnvironment::systemEnvironment ();
  env.insert ("OMP_STACKSIZE", "10M");
  jt9.setProcessEnvironment (env);
  jt9.setProcessChannelMode (QProcess::MergedChannels);
  jt9.start (exe, {"-s", key, "-w", "1", "-m", "3", "-e", QFileInfo {exe}.absolutePath (),
                   "-a", t.path (), "-t", t.path (), "-r", t.path ()});
  if (!jt9.waitForStarted (5000)) { std::fprintf (stderr, "cannot start %s\n", qPrintable (exe)); return 1; }

  std::vector<double> walls;
  long total_dec = 0;
  std::printf ("file,wall_ms,cpu_ms,decodes\n");
  for (int i = 3; i < args.size (); ++i)
    {
      std::vector<short> s;
      if (!read_wav (args[i], s)) { std::fprintf (stderr, "skip %s\n", qPrintable (args[i])); continue; }
      std::memset (&dd, 0, sizeof dd);
      std::copy_n (s.begin (), std::min<size_t> (s.size (), 180000), dd.d2);
      // nutc HHMMSS from WSJT-X's YYMMDD_HHMMSS.wav
      int nutc = QFileInfo {args[i]}.baseName ().section ('_', 1).left (6).toInt ();
      set_params (nutc, threads);
      std::memcpy (shm.data (), &dd, sizeof dd);
      double c0 = cpu_ms (jt9.processId ());
      QElapsedTimer et; et.start ();
      QFile::remove (t.filePath (".lock"));
      int ndec = 0; bool done = false;
      while (!done && jt9.state () == QProcess::Running)
        {
          if (!jt9.canReadLine ()) { jt9.waitForReadyRead (2000); if (et.elapsed () > 60000) break; continue; }
          QByteArray l = jt9.readLine ();
          if (l.startsWith ("<DecodeFinished>")) done = true;
          else if (l.size () > 20 && l.contains (" ~ "))
            {
              ++ndec;
              if (show) std::fprintf (stderr, "%s", l.constData ());   // DECODE_BENCH_SHOW=1: the decodes themselves
            }
        }
      double wall = et.nsecsElapsed () / 1e6;
      double cpu = cpu_ms (jt9.processId ()) - c0;
      QFile {t.filePath (".lock")}.open (QIODevice::ReadWrite);
      // jtdxjt9 polls for the new .lock only every 100 ms after a decode: give it
      // time to see it, or removing it for the next file goes unnoticed and it hangs
      QThread::msleep (300);
      if (!done) { std::fprintf (stderr, "no DecodeFinished for %s\n", qPrintable (args[i])); break; }
      walls.push_back (wall); total_dec += ndec;
      std::printf ("%s,%.0f,%.0f,%d\n", qPrintable (QFileInfo {args[i]}.fileName ()), wall, cpu, ndec);
      std::fflush (stdout);
    }
  QFile {t.filePath (".quit")}.open (QIODevice::ReadWrite);
  QFile::remove (t.filePath (".lock"));
  jt9.waitForFinished (3000);
  if (jt9.state () != QProcess::NotRunning) { jt9.kill (); jt9.waitForFinished (1000); }
  if (!walls.empty ())
    {
      std::vector<double> w = walls; std::sort (w.begin (), w.end ());
      double sum = 0; for (double x : w) sum += x;
      std::fprintf (stderr, "threads=%d files=%zu decodes=%ld mean=%.0f median=%.0f p95=%.0f max=%.0f ms\n",
                    threads, w.size (), total_dec, sum / w.size (), w[w.size () / 2],
                    w[std::min (w.size () - 1, w.size () * 95 / 100)], w.back ());
    }
  return 0;
}
