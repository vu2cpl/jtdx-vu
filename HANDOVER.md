# HANDOVER — JTDX-VU

Last updated: 2026-09-28

## Current state

- **Repo:** `vu2cpl/jtdx-vu`, **PUBLIC since 2026-09-28** (after a
  secret scan, see below). Default branch `jtdx-vu`. The `upstream`
  remote is `jtdx-project/jtdx`; upstream has been dormant since
  2022-03 at tag 159.
- **Version:** JTDX-VU **0.1.0** (`JTDXVU_VERSION` in `Versions.cmake`),
  on JTDX 2.2.159.
- **Installed app:** `/Applications/JTDX-VU.app` is current: all app
  code up to `4642e1ee`, installed 2026-09-28. It was built locally, so
  it runs on macOS 26+ only. Settings are in
  `~/Library/Preferences/JTDX-VU.ini` and data in
  `~/Library/Application Support/JTDX-VU`.
- **Release v0.1.0: PUBLISHED 2026-09-28.** It's at
  https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.1.0 with the macOS
  arm64 zip, the Windows x64 zip and the Pi `.deb`, each with a
  `.sha256`. **macOS Intel is still building:** it compiles Homebrew
  deps from source, which can take up to the 6 h limit. The tag-triggered
  macos run attaches it automatically; the earlier workflow_dispatch run
  `36376193759` builds the same zip and would need a manual
  `gh release upload`. Copies of the ready builds are in `~/Desktop/jdxvu/`.
- **Website:** vu2cpl.com has a JTDX-VU card (Utilities & Tools, after
  MSHV-Mac) and a project page at `/projects/jtdx-vu/` with screenshots
  and a v0.1.0 downloads table. The Intel row should become direct links
  once that zip lands.
- **Stock JTDX:** `/Applications/jtdx.app`, with its `JTDX.ini` and
  `Application Support/JTDX`, is still installed and must be left
  alone. JTDX-VU only read it once, for the first-launch migration.
- **Shared-memory sysctl:** set by
  `/Library/LaunchDaemons/com.jtdx.sysctl.plist`. It is now shmmax 32
  MiB / shmall 32768 pages; see the 2026-09-28 "stock JTDX" entry. It
  may need redoing after a macOS upgrade.
- **Build and bundle:** see `README.md`. `./macos-bundle.sh` runs the
  CMake install step, then macdeployqt and the fix-ups, and signs the
  result: `build/bundle/JTDX-VU.app`.

## What changed

### 2026-09-28 — Windows CI green, v0.1.0 released

- It took 12 runs to get Windows green. Each fix is in `windows.yml` or
  CMake:
  - MSYS2 has no `hamlib` package, so it's built from 4.7.2 source.
  - OmniRig has to be installed for `dumpcpp`, plus a `win64` typelib
    key.
  - MSYS2 names the tools `dumpcpp-qt5` and `windeployqt-qt5`, so both
    CMake lookups accept those names. `if (DUMPCPP-NOTFOUND)` never
    fired, which disguised this as "install OmniRig".
  - FindFFTW3 skipped the threads library on Windows, but MSYS2 ships
    `libfftw3f_threads` separately, so it's now added when found.
  - windeployqt needs `--no-angle --no-opengl-sw`; MSYS2 Qt has no
    `libGLESv2`.
  - The ldd DLL sweep is best-effort (`cp -n` exits 1 on skip in
    coreutils ≥ 9.2).
- The Windows zip is 55 MB, 127 files: `bin/` holds the exes, Qt, Hamlib,
  FFTW and gfortran DLLs and plugins, and `share/jtdx/` holds the data.
  It's not yet run on a real PC.
- The release was created with `gh release create v0.1.0 --target
  jtdx-vu`, using the notes plus assets from the green CI runs and the Pi
  build. All assets were checked with anonymous curl (200).

### 2026-09-28 — cross-platform builds, public repo, website

- **Portable code** (`bce19552`):
  - Secrets go to the macOS Keychain, the Windows Credential Manager
    (`CredReadW`/`CredWriteW`), or the `.ini` `[Secrets]` group on
    Linux.
  - Desktop alerts use osascript on macOS, `notify-send` on Linux, and
    a `QSystemTrayIcon` toast on Windows (also the Linux fallback).
  - The settings key is still `macOS`; the label is now "Desktop
    notification".
- **Pi / Linux arm64:** built natively on **meridianpi5**
  (`pi@192.168.1.164`; the `.local` name isn't in known_hosts), Raspberry
  Pi OS Bookworm, with apt Qt 5.15.8, Hamlib 4.5.4 and gfortran 12.
  - Source in `~/src/jtdx-vu` (rsync'd, not cloned), built with
    `nice make -j3` so Meridian keeps a core.
  - `cpack -G DEB` produces `jtdx-vu-<ver>-linux-arm64.deb`: package
    `jtdx-vu`, `Conflicts: jtdx`.
  - Dependencies come from SHLIBDEPS plus
    `libqt5multimedia5-plugins`. The upstream list named a nonexistent
    `hamlib` package.
  - Checked with `apt-get install -s` and a headless offscreen start.
  - The Pi needed `libhamlib-utils` installed for the `rigctl*-jtdx`
    install rules.
- **CI** (`.github/workflows/`):
  - `windows.yml`: MSYS2 MINGW64 on windows-latest. It installs
    OmniRig and adds a `win64` typelib key (OmniRig registers `win32`
    only). It builds Hamlib 4.7.2 from source (MSYS2 doesn't package
    it), then `windeployqt` plus ldd-collected MinGW DLLs go into a zip.
  - `macos.yml`: arm64 on macos-14 (runs on macOS 14+), Intel on
    macos-15-intel (runs on 15+). Both use `macos-bundle.sh` and
    produce `ditto` zips.
  - Both run on `workflow_dispatch` or `v*` tags. A tag attaches the
    zips to the release with `gh release upload --clobber`.
- **Why CI rather than local, for Intel and Windows:**
  - An Intel Homebrew does exist in `/usr/local`, left from MSHV. It
    already has qt@5, fftw, boost, cmake, hamlib 4.7.2 and libusb, but
    on macOS 27 it's **Tier 3**: `brew install gcc` compiles from
    source, and the result only runs on 27+. I stopped that build
    part-way.
  - The .170 Windows box has only the MSVC/Rust toolchain and runs the
    Meridian station, so it was left alone.
- **Repo made public** at Manoj's instruction, after a scan:
  - Pattern-scanned all 14 JTDX-VU commits.
  - Checked the real Keychain password, the Telegram token and the chat
    ID against the full history: no hits.
  - No private IPs, host names or emails were found. The author email
    vu2cpl@gmail.com is already public via other repos.
  - The `.deb` maintainer field uses
    `vu2cpl@users.noreply.github.com`.
- **Website:** card plus `/projects/jtdx-vu/` page, both in the
  vu2cpl.github.io repo; see that repo's HANDOVER. The Club Log dialog
  screenshot has the email and chat ID painted over.

### 2026-09-28 — compact switcher row, band default frequency

- The two switcher rows took about 70 px and crushed the right-hand
  panel in Manoj's 1088x546 window. They are now **one row**: modes |
  bands | Show filter. Buttons expand to fill the row and never take
  focus.
- A band button now prefers the frequency-list row flagged `default_`
  for the current mode (14.074 FT8, 14.080 FT4), then any row for that
  exact mode, then any row on the band. Before, it took whichever row
  came first.

### 2026-09-28 — app icon with "VU"

- `icons/Darwin/jtdx.iconset/*` (all 10 sizes) is JTDX's compass star
  with a large green (0,150,60) "VU", Arial Bold with a white outline,
  in the bottom-right corner. Manoj chose it from orange/red/green and
  badge/letters previews. It was drawn at 1024 px with PIL, scaled
  down, then built into `jtdx.icns` by the existing CMake `iconutil`
  step.
- The original icon is in git history (upstream `2a0e2bea`).

### 2026-09-28 — band/mode buttons and a "show only new" filter

- **`bandmodeswitcher.{h,cpp}`** is modelled on MSHV's
  `HvWBtSw` (LZ2HV). It adds a mode row (FT8, FT4, JT9, JT65, T10,
  JT9+JT65, WSPR-2) and a band row (160m–2m), above the decode panes.
  - MainWindow wraps the .ui central widget with `takeCentralWidget()`
    rather than editing `mainwindow.ui`.
  - A mode button triggers the existing `ui->actionXXX`.
  - A band button (`MainWindow::switch_to_band`) picks that band's
    first row in the mode-filtered `m_config.frequencies()` and calls
    `on_bandComboBox_activated`, the same as the band combo. If there's
    no working frequency, the status bar says so.
  - The active highlight is refreshed by a 500 ms timer from
    `m_freqNominal`/`m_mode`, so rig-side changes show too.
  - Buttons have `Qt::NoFocus`, so they never pull focus from the
    message fields.
  - Settings are in `[Switcher]`. The dialog is View > Band & Mode
    Buttons....
- **"Show:" filter** at the end of the mode row: `DisplayText::setNewOnly`
  on `decodedTextBrowser` only, never the Rx Frequency pane.
  - Levels: 1 = hide unless new DXCC, 2 = also new band, 3 = also new
    band+mode. Each level is checked with `LogBook::matchDXCC`, so it
    follows Club Log.
  - Always shown: `std_type 2` (my call) and the current QSO partner.
    Unknown entities are hidden.
  - Applied before the existing bypass flags, which still win.
  - The selector is amber while filtering. The level is saved in
    `[Switcher] NewOnly`.
- Verified: both rows render, the green highlight follows band and
  mode, and the dialog persists its choices. The filter's hide/show on
  live decodes is **not yet seen on air**.

### 2026-09-28 — short title

- `program_title()` now returns only "JTDX-VU for VUCG V<version>", at
  Manoj's request that the title not be cluttered.
- The base version, revision and "derivative work based on JTDX by HF
  community and WSJT-X by K1JT" credit moved to the About box, which
  still carries the full JTDX and WSJT-X credit list.
- The WAV ISFT metadata uses `program_title()` too, so it is now the
  short form.

### 2026-09-28 — Telegram confirmed

- Manoj ran the Telegram "Send test" from File > Club Log & Alerts...,
  and it arrived. The token is in the Keychain.

### 2026-09-28 — Club Log "refused" with an empty reply: FIXED

- **Symptom:** the status line showed "Club Log refused:" with no
  text, and nothing had ever downloaded.
- **Cause:** `eqsl.cpp` (and `wsprnet.cpp`/`DisplayManual`) connect to
  the **shared** `QNetworkAccessManager::finished` and `readAll()`
  every reply. The manager's `finished` fires before the reply's own,
  so eQSL drained the 10 MB body and `on_adif_reply` saw an empty one.
  The credentials were fine: curl and a standalone Qt program both got
  the full log.
- **Fix:** `ClubLog` owns its own `QNetworkAccessManager`, which also
  serves Telegram. An empty reply now reports "empty reply (HTTP n)".
  The QSO count label is refreshed after a Club Log reload.
- **Verified** with a throwaway instance and a copy of Manoj's .ini: a
  real fetch returned 57,648 QSOs, and the FT8 count with
  confirmed-only showed 11,883, exactly matching the confirmed FT8
  QSOs in the file.

### 2026-09-28 — Settings dialog layout on macOS; no focus stealing

- **Settings tabs overlapped** (Notifications worst) and the tab bar
  needed scroll arrows. `Configuration.ui` pins `minimumSize` 686x586,
  which was sized for Windows fonts. An explicit minimum overrides the
  layout-derived one, so with the macOS font the dialog shrank below
  what its layouts need, and a saved geometry of 686x618 kept it there.
  - `Configuration.cpp` now calls `setMinimumSize (0, 0)` after
    `setupUi`, and grows the restored geometry to at least
    `minimumSizeHint()`. It now opens at about 801x871, with all tabs
    visible and no overlap.
  - Verified in a throwaway instance with a copy of Manoj's .ini.
- **JTDX grabbed focus every cycle while monitoring.** "Enable main
  window popup" (Misc menu; `EnableMainwindowPopup=true`, inherited
  from stock JTDX) made `displaytext.cpp` and the first-decode beep in
  `mainwindow.cpp` call `showNormal/raise/setActiveWindow` on
  new-DXCC/grid/call/my-call lines. Club Log makes those lines more
  frequent.
  - Both now call `QApplication::alert()` instead, which bounces the
    Dock icon and never moves focus.
  - The UDP "window to front" options, off by default, are unchanged.
- **Test-instance hygiene:** `pgrep -f` patterns with `\|` don't
  alternate in BSD pgrep, which gave a false "quit" once and left a
  test instance running while its folders were deleted. Track the
  exact PID (`$!`), quit through System Events with a
  frontmost-PID check, then delete the files.

### 2026-09-28 — stock JTDX "Unable to create shared memory segment"

- **Cause:** the stock `/Applications/jtdx.app` (2.2.159-32A, x86_64
  under Rosetta) calls `shmget` for **16,572,248 bytes**. It uses newer
  source than this repo, whose `dec_data` is 13,692,248 bytes. The
  shmmax of 14,680,064, taken from this repo's 2022 plist, is too small
  for it, so shmget returns EINVAL.
  - Measured with a `DYLD_INSERT_LIBRARIES` shmget interposer.
- **Fix:** `Darwin/com.jtdx.sysctl.plist` now sets shmmax to 33554432
  and shmall to 32768 (128 MB). That fits both apps side by side with
  headroom. Manoj applies it with sudo.
- Also removed two orphaned 13.7 MB segments, plus their semaphores and
  temp files, left by `--rig-name test`/`vtest` instances killed during
  testing. **Lesson:** stop JTDX test instances by quitting them, not
  with `pkill`. SysV segments outlive a killed process and eat the
  shmall quota.

### 2026-09-28 — v0.1.0, "for VUCG community"

- `Versions.cmake` gains `JTDXVU_VERSION 0.1.0`, passed to the code as
  `JTDXVU_VERSION` through `wsjtx_config.h.in` and read with
  `jtdxvu_version()` in `revision_utils`.
- `applicationVersion()` now returns the JTDX-VU version. That version
  is used in the title, About, the Log QSO title, the ALL.TXT
  Transmitting/Tune lines, PSK Reporter and UDP MessageClient.
- `version()` stays at the upstream 2.2.159 and is still used for the
  manual file names, the WSPRnet upload and the release-candidate
  check.
- The title reads "JTDX-VU for VUCG community v0.1.0, based on JTDX
  v2.2.159 by HF community and WSJT-X by K1JT". The About box adds a
  "Built by VU2CPL on JTDX v2.2.159" line, and the full JTDX and
  WSJT-X credits are unchanged.
- In Info.plist, CFBundleShortVersionString and CFBundleVersion are
  0.1.0, and CFBundleLongVersionString is "JTDX-VU 0.1.0 (based on
  JTDX 2.2.159)".
- **To release a new version:** bump `JTDXVU_VERSION`, re-run cmake,
  build, then run `./macos-bundle.sh`.

### 2026-09-28 — native build, JTDX-VU identity, Club Log and alerts

- **Build fixes for current toolchains.**
  - `lib/jplsubs.f`: `EXTERNAL SPLIT`. gfortran 16 added an F2023
    `SPLIT` intrinsic.
  - CMake 4 needs `-DCMAKE_POLICY_VERSION_MINIMUM=3.5`.
  - Docs and manpages are off, because asciidoctor and a2x aren't
    installed.
- **App name `JTDX-VU`** (`main.cpp`).
  - Titles, dialogs (`JTDXMessageBox.cpp`, `Configuration.cpp`) and
    the PSK Reporter ID all follow `applicationName()`.
  - Multi-instance detection used `applicationName().length()>4`; it is
    now `>7`.
- **First-launch migration** (`main.cpp`,
  `migrate_legacy_jtdx_state`).
  - Runs only if no `JTDX-VU[ - rig].ini` exists yet. It copies
    `JTDX[ - rig].ini` and the stock data dir, and rewrites data-dir
    paths inside the .ini.
- **Resource path bug** (`Configuration.cpp`, `installed_path`).
  - The data and doc paths were `<exe>/../../../jtdx.app/...`. They
    only worked because APFS is case-insensitive. After the bundle was
    renamed, the decoder read the *stock* `/Applications/jtdx.app`'s
    data. Paths now resolve through the bundle the app is running from.
- **`macos-bundle.sh`** does what macdeployqt misses:
  - copies `libgcc_s.1.1` (needed by libgfortran) and `libsharpyuv.0`
    (needed by libwebp*) into the bundle;
  - replaces the `rigctl*-jtdx` symlinks into the Homebrew Cellar with
    real files;
  - checks that no `/opt/homebrew` dependency remains.
- **Club Log worked-before source** (`clublog.{h,cpp}`,
  `logbook/adif.*`, `logbook/logbook.*`, `MainWindow::init_logbook`).
  - Fetch: `POST https://clublog.org/getadif.php` (email, app password,
    call). The response is rewritten one record per line to
    `Application Support/JTDX-VU/clublog_log.adi`.
  - When enabled, `ADIF::load` reads that file instead of
    `wsjtx_log.adi`, with no STATION_CALLSIGN or grid filter, since
    Club Log's log is already one callsign. Local QSOs from 7 days
    before the last fetch onward are added on top.
  - "Confirmed only" skips Club Log QSOs without `Y`/`V` in QSL_RCVD,
    LOTW_QSL_RCVD or EQSL_QSL_RCVD.
  - The fetch runs 3 s after start-up if the cache is more than 24 h
    old, or on "Refresh now".
- **Alerts** (`ClubLog::check_decode`, hooked in
  `MainWindow::readFromStdout` after `deCallAndGrid`).
  - ATNO, new band and new band+mode, checked through JTDX's own
    `LogBook::matchDXCC`. The own call is skipped.
  - Cooldown is keyed on call, band and mode.
  - Transports: macOS notification via `osascript`, and Telegram
    `sendMessage`.
  - Secrets are in the Keychain (service `JTDX-VU`, accounts
    `clublog-app-password` and `telegram-bot-token`) and written
    through `security -i` on stdin.
- The settings UI is a code-built dialog, File > Club Log & Alerts...,
  so `Configuration.ui` is untouched and upstream merges stay easy. The
  values are in `[ClubLog]` and `[Alerts]` in JTDX-VU.ini.
- **Dropped:** MSHV's AS (All Slots). JTDX has no Multi-Answer mode,
  so there's no slot restriction for it to lift.

### Verified

- The build runs from `/Applications` with zero `/opt/homebrew` loads.
- The migration copied 112 MB with correct .ini paths. Titles read
  JTDX-VU.
- The decoder's `-r` path is JTDX-VU's own bundle.
- **Loader test** with a synthetic 50k-QSO Club Log file in a
  `--rig-name test` instance (since deleted):
  - the FT8 count matched the file exactly (12,447);
  - confirmed-only matched exactly (3,761);
  - it adds about 0.5 s to start-up.
- The dialog renders, and a macOS notification arrives.

## Open items

- [ ] **macOS Intel zip:** once CI finishes, check it's attached to
      v0.1.0 and swap the Intel row on vu2cpl.com/projects/jtdx-vu/ for
      direct links. Also copy it into `~/Desktop/jdxvu/`.
- [ ] **Cache Hamlib in windows.yml.** It rebuilds from source every run
      and costs about 10 min.
- [ ] **Windows build untested on a real PC.** Check audio, CAT and
      OmniRig, and that notifications appear as tray toasts.
- [ ] **Pi `.deb` untested on air.** Only a dry-run install and a
      headless start have been done so far.
- [ ] **Other Macs:**
      - The CI zips are ad-hoc signed, not notarized, so they need
        right-click > Open on first launch.
      - Every Mac also needs the shared-memory sysctl (README).
      - A notarized build would need the Developer ID used for MSHV.
- [x] ~~GPL: source must be available before distributing~~ — the repo
      went public 2026-09-28.

- [ ] A **real decode alert** hasn't fired yet. Watch for one on air.
- [ ] **CAT control** with Hamlib 4.7.2 (stock, not the JTDX Hamlib
      fork) is not yet verified on the rig.
- [ ] Rows already on screen keep their colour after a Club Log
      refresh, since JTDX colours rows when they're inserted.
- [ ] macOS notifications show the Script Editor icon, because they go
      through osascript.

## Known gotchas

- **Don't use `install/` as the CMake prefix.** On a case-insensitive
  volume it collides with the `INSTALL` file; use `build/dist`, which
  the script handles.
- **CMake's own `fixup_bundle`** in the install step fails on
  `@rpath/libsharpyuv`. That's expected, and the script tolerates it.
- **A `brew upgrade`** of qt@5, hamlib, gcc or webp only affects the
  next bundle, not the installed app. Rebuild and re-bundle after one.
