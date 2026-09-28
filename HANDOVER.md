# HANDOVER — JTDX-VU

Last updated: 2026-09-28

## Current state

- **Repo:** `vu2cpl/jtdx-vu` (private), default branch `jtdx-vu`. The
  `upstream` remote is `jtdx-project/jtdx`; upstream has been dormant
  since 2022-03 at tag 159.
- **Installed app:** `/Applications/JTDX-VU.app` has the rename but
  **not yet Club Log/alerts**: it was running, so it wasn't replaced. The
  current build is `build/bundle/JTDX-VU.app`. Quit JTDX-VU, then
  `ditto build/bundle/JTDX-VU.app /Applications/JTDX-VU.app`.
  Settings are in `~/Library/Preferences/JTDX-VU.ini` and data in
  `~/Library/Application Support/JTDX-VU`.
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

- [ ] **Distributing to VUCG members needs the source to be
      available.** JTDX is GPL v3, and handing the app to other people
      means offering them the source. The repo is private right now, so
      either make it public or give recipients access, before
      distributing. Also decide whether the Club Log and alerts features
      go into the community build.
- [ ] **Stranger Macs.** The bundle is ad-hoc signed, not notarized.
      Other Macs need right-click > Open on first launch, the
      shared-memory sysctl, and macOS 26 or later.

- [ ] **Telegram is untested.** Enter the bot token and chat ID, then
      press Send test.
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
