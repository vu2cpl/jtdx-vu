# HANDOVER — JTDX-VU

Last updated: 2026-10-02

## Current state

- **Repo:** `vu2cpl/jtdx-vu`, **PUBLIC since 2026-09-28** (after a
  secret scan, see below). Default branch `jtdx-vu`. The `upstream`
  remote is `jtdx-project/jtdx`; upstream has been dormant since
  2022-03 at tag 159.
- **Version:** JTDX-VU **0.5.0** (`JTDXVU_VERSION` in `Versions.cmake`),
  on JTDX 2.2.159. v0.1.0 was the first release; v0.2.0 adds CNS, the
  live Show filter and the Windows fixes; v0.2.1 makes CNS respect the
  AutoSeq give-up counters again; v0.3.0 adds JTTY and FT2; v0.4.0 adds
  JTTY Auto CQ, Settings > JTTY (macro sets, per-set exchange, station
  variables), click-to-pick calls, the one "Auto CQ" button, and fixes
  the TCI quit crash, the Settings dialog on small screens and JTTY
  logging (599, MFSK/JTTY, start time); v0.5.0 gives JTTY its own
  screen (QSO fields, calls heard, 24 macros, type-ahead).
- **Release v0.5.0: COMPLETE 2026-10-02** — https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.5.0,
  tag `v0.5.0` on `2a6c710f`. Notes carry a full per-OS "How to
  install" (same text as README's new section). All four builds plus
  `.sha256` attached; every asset downloaded anonymously and its
  checksum OK; arm64 app reports 0.5.0; copies in `~/Desktop/jdxvu/v0.5.0/`.
  - CI run 36974037893 (macOS: Intel ~10 min on the keg cache, arm64
    ~70 min) and 36974038144 (Windows), all green.
  - Pi `.deb` on meridianpi5 (prefix /usr/local): `apt-get install -s`
    clean, Conflicts: jtdx, 8 s headless start OK, uploaded by hand.
    (Careful: `pkill -f "<pattern>"` inside `ssh '...'` matches the
    remote shell's own command line and kills the session - use
    `pkill -f "[j]tdx..."`.)
  - Website: card sentence + project page (paragraph "JTTY screen
    (v0.5.0)", install paragraph pointing at the README guide, table on
    v0.5.0). Pushed early by the website session (`07b899a`) with two
    "Building" placeholder rows; direct links restored in `3d34286` once
    all eight URLs answered 206.
- **After v0.5.0 on `jtdx-vu` (unreleased):** Auto CQ time limit (see
  the 2026-10-02 "Auto CQ time limit" entry). **Installed** 2026-10-02
  (local build, still reports 0.5.0). Main-window resize fix (2026-10-02
  entry) **installed** 2026-10-02 21:56; the app it replaced is in
  `~/Desktop/jdxvu/prev-install/JTDX-VU.app.before-resize`. Even
  decode-pane split (same day) **installed** 2026-10-02 22:50; the app
  it replaced is `~/Desktop/jdxvu/prev-install/JTDX-VU-before-split.app`.
- **Release v0.4.0: COMPLETE 2026-10-01** — https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.4.0,
  tag `v0.4.0` on `bf707bc8`. All four builds plus `.sha256` attached
  and verified (anonymous download, every checksum OK, arm64 app reports
  0.4.0); copies in `~/Desktop/jdxvu/v0.4.0/`.
  - CI run 36880480905 (macOS) and 36880480879 (Windows), all green.
    **The Intel keg cache works:** the Intel job took 10 min (14:57→15:08
    UTC) instead of 2.5–3.5 h. The arm64 job was the slow one (~50 min
    in Dependencies) but passed.
  - Pi `.deb` built on meridianpi5 with `CMAKE_INSTALL_PREFIX=/usr/local`
    (same as v0.3.0 — the README had said `/usr`, corrected), `apt-get
    install -s` clean, 8 s headless start OK, uploaded by hand.
  - Website updated the same evening (card + page, downloads table on
    v0.4.0), pushed after all assets answered.
- **Unreleased on `jtdx-vu` after v0.4.0:** the JTTY screen redesign
  (2026-10-02 entry): FT-only controls hidden in JTTY, S / R / Name
  fields, calls heard, 24 macros in 3 banks, set picker, type-ahead.
  Built and tested in a throwaway instance; **installed 2026-10-02
  11:40, re-installed 11:45 (tidied layout) and 11:55 (no CW ID)** (bundle of `6ae5e051`'s code). Previous v0.4.0 app and the
  pre-migration .ini (`JTDX-VU.ini.before-redesign`) are in
  `~/Desktop/jdxvu/prev-install/`.
- **Installed app:** `/Applications/JTDX-VU.app` is current: all app
  code up to `4642e1ee`, installed 2026-09-28. It was built locally, so
  it runs on macOS 26+ only. Settings are in
  `~/Library/Preferences/JTDX-VU.ini` and data in
  `~/Library/Application Support/JTDX-VU`.
- **Release v0.3.0: COMPLETE 2026-09-29** — all four builds attached — JTTY + FT2 + separate
  New-DXCC-on-band/mode colours + DIGU re-assert buttons. Tag `v0.3.0`
  pushed. Windows zip and Pi `.deb` (meridianpi5) attached; the CI
  **arm64 job failed in its Dependencies step** — `brew install`
  exited 1 on macos-14 after pouring every bottle, because the runner
  image links `openssl@1.1` and `openssl@3`'s link step collided with
  it (not the Tier 3 notice, which was just the last thing printed) —
  so the arm64 zip was built locally with the same `macos-bundle.sh` +
  `ditto` steps and uploaded by hand; Intel came from CI as usual.
  `macos.yml` now unlinks `openssl@1.1` first and tolerates a non-zero
  exit when `brew list` shows all formulae; it also **caches the Intel
  job's source-built kegs** (`actions/cache`, one tarball of the kegs
  that step installed, key `brew-kegs-macos-15-intel-v1`; bump the
  suffix to rebuild) so later releases skip the ~3.5 h compile. All
  these commits are after the tag; the tag itself was not moved.
  **Two more lost Intel runs on the way** (tag run 2 h 20, first
  seeding run 3 h 45, both with everything compiled): `brew unlink
  openssl@1.1` removes nothing — the image's `bin/openssl` isn't a
  tracked link — so the tolerance fallback is what actually matters;
  and the first cache step wrote its list into `/usr/local`, which is
  not writable. The cache now lives in `~/brew-cache`, is saved by an
  explicit `actions/cache/save` step right after Dependencies (a post
  step is skipped when the job fails later), and cannot fail the build.
  `hashFiles()` only sees the workspace, so the save is gated on a
  step output instead.
  Third run (36586507561) passed: Intel zip taken from its artifact,
  checksum verified, uploaded by hand; cache saved (277 MiB). **Confirmed at
  v0.4.0:** the Intel job restored the cache and finished in ~10 min. The arm64 job passed twice on the
  fixed workflow. Website updated the same morning (card + page,
  downloads table on v0.3.0; all three attached assets answer 206 to
  an anonymous ranged GET). Copies in `~/Desktop/jdxvu/v0.3.0/`.
  `/Applications/JTDX-VU.app` is **v0.4.0** (local build of tag `v0.4.0`;
  title "JTDX-VU for VUCG V0.4.0"), installed 2026-10-01 20:36. Macro
  sets migrated (Default/Ragchew "599", Contest "599 %N", active
  Ragchew / DX). The previous v0.3.0 bundle is
  in `~/Desktop/jdxvu/prev-install/`.
- **Release v0.2.1: COMPLETE 2026-09-28**. It's at
  https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.2.1 with all four
  builds plus a `.sha256` for each:
  - The tag CI attached the Windows, macOS arm64 and macOS Intel zips.
    The Intel build took about 2.5 h and the attach step now works on
    Windows too.
  - The Pi `.deb` was built on meridianpi5 and uploaded by hand.
  - Copies are in `~/Desktop/jdxvu/v0.2.1/`.
  - v0.2.0's notes say "superseded by v0.2.1": its CNS never gave up on
    a silent station. Its Intel build was cancelled.
  - v0.1.0 remains published.
- **Branch `jtty`** (pushed): JTTY mode port from WSJT-X 3.2.0-rc1.
  Phase 1 (Fortran lib), phase 2 (RX in the GUI) and phase 3 (TX) done;
  **first JTTY QSO made on air 2026-09-28** via TCI. The sound-card TX
  path is still only loopback-verified. Controls page now mirrors
  WSJT-X 3.2.0-rc1's. **FT2 added 2026-09-29** on the same branch: RX
  verified in the GUI on simulated signals, TX loopback-verified, **not
  yet on the air** (needs an MSHV FT2 station to interoperate with).
  Not in any release.
- **Website:** vu2cpl.com has a JTDX-VU card (Utilities & Tools, after
  MSHV-Mac) and a project page at `/projects/jtdx-vu/` with screenshots
  and a v0.2.1 downloads table linking all four builds.
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

### 2026-10-02 — Decode panes split about half each, in every mode

Manoj, after the resize fix: "left pane and right pane should be almost
equal ... all modes", without shrinking buttons, fonts or labels.

- **Regression from the v0.5.0 JTTY screen, fixed:** a QStackedWidget is
  as wide as its widest page, shown or not. The JTTY macro panel (585 px)
  shares `controls_stack_widget` with the FT Tx-message tabs (281 px), so
  every FT mode's right pane had a 796 px minimum and the splitter could
  not go near half (a 1462 px window stopped at about 650 | 800). Hidden
  pages now get an Ignored size policy (their own policy is restored when
  shown). FT8 minimum window width 1063 -> 813.
- **Even split:** `MainWindow::evenSplit()` sets the splitter to half
  each, at start-up (after the saved `vertSplitter` state), on every mode
  change (`jttyApplyLayout`, which every mode runs) and on width changes.
  `QSplitter::setSizes` keeps each side at least its minimum, so nothing
  is squeezed: in a narrow window the right pane gets its minimum and the
  left the rest. The handle can still be dragged; the next resize or mode
  change evens it again.
- **Verified** in a throwaway `-r rsz` instance at Manoj's 1462x887: FT8,
  FT2 and JTTY each 730 | 730; FT8 at 1000x700 keeps the right pane's
  minimum; FT8 and JTTY minimums clean (no clipping). Each mode was
  started from the .ini (Mode=), because synthetic menu clicks stopped
  reaching the test instance.
- Test gotcha: after a test instance is killed, the next one can show
  "Subprocess failed with exit code 2"; that dialog swallows all input.
  Kill both processes, wait a few seconds, and relaunch.
- **Installed** 2026-10-02 22:50; TCI audio came straight back this time
  (AetherSDR not restarted).

### 2026-10-02 — Main window resizes properly (JTTY spare height; no overlap when small)

Manoj: the main window "does not resize as needed" (on his Mac).

- **JTTY, large window:** spare height went to empty gaps. The stock
  stretch (`verticalLayout_12` 1,6,2) gave the top controls and the macro
  row a share, and the calls-heard widget (list fixed at two rows) took
  half of the middle and padded it round its title. Now, in JTTY only,
  the top controls, calls heard and the macro panel keep their natural
  height and the Rx pane takes all the rest. Calls heard stays two rows
  (Manoj's choice, same day). `m_jttyStretchSaved` now saves and restores
  the stretch of all three layouts it changes, so FT modes are stock.
- **Any mode, small window:** `mainwindow.ui` pins a 733x422 minimum,
  which overrides the layouts' own (the same trap as the Settings dialog
  on 2026-09-28), so the window shrank until controls overlapped. The
  constructor now calls `setMinimumSize (0, 0)`; the window stops at
  the layout minimum: about 1063x608 in FT8, 929x632 in JTTY (Mac font).
  Both fit a 1366x768 screen.
- **Installed** 2026-10-02 21:56 (live app quit cleanly first; old app
  in `~/Desktop/jdxvu/prev-install/JTDX-VU.app.before-resize`).
- **After the install: no audio, black waterfall.** Not the build:
  TCI was connected (AetherSDR, 127.0.0.1:50001) but AetherSDR sent no
  audio (about 10 KB in total; a live stream is about 100 KB/s). The
  previous build got no audio either. Restarting AetherSDR fixed it, and
  the new build then received audio. If TCI audio stops after JTDX-VU
  quits and restarts, restart AetherSDR first. `nettop -P -p <pid> -l 2
  -s 3 -J bytes_in` shows whether audio is arriving.
- Backup gotcha: `JTDX-VU.app.before-resize` is not a `.app` name, so
  macOS won't launch it (`open` error 162, binary SIGKILLed). To run it,
  `ditto` it to a `*.app` name first.
- **Verified** in a throwaway `-r rsz` instance (copy of the .ini, Rig
  None): sizes from 300x200 (clamped) to 2400x1300 in FT8 and JTTY,
  and JTTY -> FT8 -> JTTY by the Mode menu; FT8 identical to before.
- Test gotcha: copying `jtdxjt9` from `/Applications/JTDX-VU.app` into
  `build/jtdx.app` does not work any more (it needs the bundle's
  Frameworks: dyld "Library missing", then a "Subprocess error" dialog
  that swallows all input). Copy `build/jtdxjt9` instead.

### 2026-10-02 — Auto CQ time limit, 5 min by default (for the next release)

Manoj: "next release auto cq to be limited to 5 minutes" - both jobs of
the button, as a setting defaulting to 5.

- `[JTTY] AutoCqMinutes` (1-60, default 5), "Time limit" in Settings >
  JTTY's Auto CQ box (`JttySettings::autoCqMinutes`).
- JTTY: `m_jttyAutoCqStartMs` set when Auto CQ is switched on;
  `jttyAutoCqFire` stops with "N min time limit (Settings > JTTY)"
  instead of calling again once that much time has passed (a CQ already
  on the air finishes).
- FT modes: `watchdog_minutes()` is now this setting (was a fixed 10)
  while Call Non-Stop is on; menu text and tooltips say so.
- Verified in a throwaway instance with the limit at 1 min and a 3 s
  gap: CNS off - five CQs 06:55:14..06:56:04, then no more ("WD 9m" =
  the normal watchdog); CNS on - "WD 1m" shown and the watchdog expired
  at a minute. Installed locally 2026-10-02; goes out with the next
  release.

### 2026-10-02 — JTTY: a refused macro blinks its key

Manoj: "make the F keys flash when DX call is missing". Every refusal in
`jttyMacro` (no DX call, no name for %NAME, no Call next, a station
detail or my call unset) now goes through one `refuse` lambda: status-bar
message as before, plus `JttyPanel::flashKey(key)` - the key's button
blinks red three times (160 ms steps), or its Bank button when that bank
isn't on screen - and `JttyPanel::flash()` on the field to fill in (DX
Call, or Name). Verified with a burst of window captures after F2 with no
DX call: the F2 button red / normal / red / normal / red. Installed.

### 2026-10-02 — JTTY: no CW ID after a macro; shorter calls-heard list

- **Regression from the redesign, fixed:** Manoj heard Morse ("VI" -
  VU2CPL) after each JTTY macro. PTT-on in `guiUpdate` arms a CW ID for
  every mode (`icw[0]=m_ncw`); the FT tone block used to clear it, and
  the redesign skips that block for JTTY, so the Modulator (whose CW ID
  test is `m_TRperiod > 16`, and JTTY's nominal period is 120 s) sent the
  call in CW after the wave. Now `icw[0]=0` in the JTTY branch of the
  tone block and in the "No CW ID in FT8" line (FT or JTTY).
- Calls-heard list two rows high (scrolls beyond that).
- "Many F keys not working" was the `%H` guard: in "Ragchew / DX" F2,
  F3, F5 and F7 need a DX call, and say so only in the status bar.
  Verified in a throwaway instance with real key events: F2 refused
  ("enter the DX call first"), F4 sent; after picking LZ2HV, F5 sent
  and F7 queued and sent.

### 2026-10-02 — JTTY layout tidied (rearranged, no gaps)

Manoj on the first install: "needs a rearrangement of buttons, it's all
scattered" - hiding widgets inside the FT-built layouts left holes (S / R
/ Name floating, empty third grid column, three buttons at the foot of a
mostly empty right column, Halt / Log / Erase / Clear DX stretched tall
in their own column, gaps between the panel rows).

- **Tx/Rx grid** (`gridLayout`) in JTTY: column 0 Tx / arrows / Rx /
  Split, column 1 S / R / Name / S meter (S meter moved from [1,2] to
  [4,1] and back on leaving), so column 2 empties and collapses. S and R
  are separate label+field widgets (`m_jttyQsoFields`, `m_jttyRcvdField`).
- **All buttons in the right-hand column** (`verticalLayout_2`): Halt Tx,
  Log QSO, Erase and Clear DX move under Auto CQ, taking Tune's size
  limits and policy (their own saved as widget properties and restored,
  with their `gridLayout_9` cells, on leaving JTTY). The macro panel now
  has the whole bottom row.
- **Panel:** rows packed to the top (spare height in a stretch row
  below), macro buttons a little taller.
- Verified in a throwaway instance: JTTY layout as described, FT8 after
  it identical to before, back to JTTY the same. Installed 11:45.

### 2026-10-02 — JTTY screen redesign: FT-only controls hidden, QSO fields, calls heard, 24 macros, type-ahead

Manoj: redesign the UI for JTTY only - lots of unused / non-functional
buttons; use the space for more memory macros etc. He chose all four
proposals (24 macros in 3 banks, set picker on the panel, QSO strip with
RST + name, calls-heard list) plus calls coloured from the Club Log data,
macro editing only in Settings, and queue-while-transmitting.

- **Audit first** (code trace, every control): in JTTY these do nothing
  - TX Even, Report, CL, DT, Hound, the "Tx JTTY" label, AutoTX, Wanted,
  AutoSeq, the wanted-call filters + Clr, Bypass, 1 QSO, AnsB4, Enable Tx
  (a JTTY send keys up without it), Hint, Sync, SWL, AGCc, Filter, Decode,
  and the progress bar (counts the nominal 120 s buffer).
- **`jttyApplyLayout()`** (`mainwindow_jtty.cpp`) hides them in JTTY and
  shows them again elsewhere. Called at the end of `commonActions`,
  `dynamicButtonsInit` and `on_cbShowWanted_toggled` (both re-show some),
  plus a once-a-second check in `guiUpdate` for modes set up without
  `commonActions` (WSPR). Halt Tx / Log QSO / Erase / Clear DX move into
  one column of `gridLayout_9` and `horizontalLayout_4`'s stretch goes
  0,1,0 so the macro panel gets the width; all restored on leaving.
  Verified: FT8 afterwards shows every control as before (Hint hidden,
  Sync shown, as FT8 does).
- **QSO fields** (`jttySetupUi()`): S / R / Name in the middle column of
  `gridLayout` (where Report / CL / Tx mode sit). New DX call → RST back
  to 599, Name = `m_name` (looked up from the log); editing Name sets
  `m_name`. Log QSO in JTTY now takes S / R instead of a fixed 599 / 599.
  No received-serial field: ADIF SRX would need plumbing through four log
  paths - in a contest type "599 012" into R.
- **Calls heard** (`JttyHeardList` in `jttypanel.{h,cpp}`), inserted in
  `verticalLayout_9` above the Rx Frequency pane. Every callsign-shaped
  word in a decode (not my call; the last word of a still-growing message
  is skipped as possibly half a call; each message counts a call once via
  `JttyDecodeLine::heardCalls`), newest first, updated in place, 40 max,
  cleared on a band change. Colour (`jttyHeardStatus`): new DXCC > new
  DXCC on band > new DXCC in JTTY > new call > not on band in JTTY >
  worked, using the Settings colours and `LogBook` (so Club Log).
  Recoloured on `ClubLog::log_updated` and after a QSO is logged.
  Click = DX Call + Rx frequency. Font = the decode panes'.
- **Macros**: `MacroSet` now has 24 macros + 24 labels (`Msg1..24`,
  `Label1..24`). Keys: F1-F8, Shift+F1-F8, Option(Alt)+F1-F8, fixed per
  bank whatever bank is shown. The panel has no edit boxes any more: Set
  picker, Bank 1/2/3, eight labelled buttons (label or a short form of
  the macro: variables, DE, K and repeats dropped), tooltips show the key
  and template. Settings > JTTY has a tab per bank with label + macro.
  Auto CQ can use any of the 24.
  **One-off migration** (`Banks=3` in `[JTTY]`): sets named like the
  built-ins get banks 2-3 (contest fills; ragchew rpt/name/QTH/rig/QSL/
  73/QRS...) and labels where the macro is still the built-in text, and
  exchange "599" → "%RST", "599 %N" → "%RST %N" (same text while S is
  599). Manoj's own "Default" set keeps everything, empty banks 2-3.
- **%RST / %NAME**: from the QSO fields; %NAME empty blocks the send with
  a status message. Expanded after %E and before %N (`%N(?!AME)` when
  dropping the serial; `usesSerial` ignores %NAME).
- **Type-ahead**: `jtty_tx` while busy appends to `m_jttyTxQueue` (panel
  shows "Next: ..."); `jttyUpdateTxState` sends the head when a message
  ends (`jttySendQueued`, retries every 200 ms while PTT drops). Halt /
  Esc / leaving JTTY clear it. Auto CQ never queues.
- **Bugs fixed on the way** (found in the audit):
  - Erase in JTTY left the renderer holding `QTextBlock`s of the cleared
    documents → `flushJttyDecodeLines()` after Erase.
  - At JTTY key-up the FT "Calculate Tx tones" block ran: it read the
    hidden FT Tx box (could halt with "empty message"), overwrote
    `m_curMsgTx` / LastTx with a stale FT message, and could run the 73 →
    log logic. Now skipped for JTTY (not Tune); `m_curMsgTx` = the JTTY
    text. The FT-style "Tx @ ..." line in the Rx pane is skipped too
    (JTTY writes its own "Tx:" line).
  - `decode()` returns at once in JTTY (could start jtdxjt9 if primed).
  - Picking a call cleared the grid *after* the log had filled it; now
    the grid is cleared first.
- **Verified** in a throwaway `-r jttyui` instance (copy of the .ini,
  Rig None, alerts off, audio out to the Mac mini speakers, Club Log
  cache copied) fed a mixed wav made with a scratch copy of
  `txtest_jtty` taking a frequency (five stations, 1100-1900 Hz; the GUI
  decoded the three inside the waterfall span): calls heard filled and
  coloured (3B8CW new DXCC red, IK0QKN / LZ2HV worked green); click →
  DX Call + grid from log + Rx 1300; FT8 and back; Bank 2 = migrated
  ragchew banks; F1 then F3 → "Next: LZ2HV TU 73 DE VU2CPL SK", sent
  10.7 s later when the CQ ended (ALL.TXT); Shift+F5 (uses %NAME, Name
  empty) refused; Option+F7 sent "PSE QRS"; Settings > JTTY shows the
  three bank tabs with labels. **Not yet tested:** Log QSO with edited
  S / R / Name, Erase during live decoding, on air.
- **Installed** 2026-10-02 after the tests (the live app had been quit
  cleanly at 11:29). The first launch runs the macro-set migration; the
  .ini from before it is in `~/Desktop/jdxvu/prev-install/`.

### 2026-10-01 — Stop button hidden; Monitor is the start/stop

Manoj: "stop button is redundant. monitor can be used to start and stop
monitoring" (all modes).

- `ui->stopButton` is hidden at start-up and `dynamicButtonsInit` no
  longer shows it at any window height (its three `show()` calls became
  `hide()`). The widget and `on_stopButton_clicked` stay, because code
  still clicks/calls it (Decode-remaining-files and the file-open path).
- Monitor is checkable and already stopped monitoring when unchecked;
  unchecking it now also clears `m_loopall`, the one extra thing Stop
  did (ends a "Decode remaining files in directory" run).
- Verified in a throwaway instance with a 640 px tall window: no Stop;
  Monitor off → grey, meter and progress stop; on → green, receiving.

### 2026-10-01 — JTTY: Auto CQ is its own halt; Halt button removed

Manoj: Auto CQ grey → green, and pressing it again should halt (grey);
then no separate Halt button is needed in JTTY. Keep Stop.

- `jttyAutoCqToggled(false)` stops Auto CQ and, if a send is in
  progress, calls `jttyHalt()`.
- The JTTY Halt button is gone (panel's `halt_`, `haltButton()`,
  `setTransmitting()` removed). Esc still emits `haltRequested`.
- **Halt Tx** (left button group) now also halts a JTTY send:
  `on_stopTxButton_clicked` calls new `jttyClearTx()` (Auto CQ off,
  `m_jttyTxRequestedUntil`/`m_jttyTxEndMs` cleared) in JTTY. `jttyHalt()`
  = `jttyClearTx()` + `haltTx()`, which lands in `on_stopTxButton_clicked`
  again, harmlessly.
- Stop (Monitor off) is unchanged.
- Verified in a throwaway instance: Auto CQ press → green + CQ; press
  again → grey, CQ cut, "Auto CQ stopped: switched off", nothing sent in
  the next 13 s; F1 by hand then Halt Tx → stopped. An extra CQ in an
  earlier run was Manoj clicking the test window, not a bug.

### 2026-10-01 — Auto CQ button in the column; per-set exchange; %OP %QTH %TX %ANT

Manoj, while trying the first Auto CQ build: drop the serial in
ragchew; map Auto CQ onto CNS so there is one button; Halt next to it;
move it down; call it "Auto CQ"; add %OP/%QTH/%TX/%ANT; bigger Send
message box.

- **One "Auto CQ" button** (was the top-row CNS): now in the right-hand
  column under AnsB4 (`verticalLayout_2`), sized like its neighbours.
  In the FT modes it is Call Non-Stop exactly as before (same
  `JTDXVU/NonStop` setting and AutoSeq menu item); in JTTY it toggles
  JTTY Auto CQ. `updateCnsButton()` shows whichever applies and swaps
  the tooltip; the mode hook calls it and stops JTTY Auto CQ on leaving
  JTTY. The panel's own Auto CQ button is gone.
- **Halt** moved from the panel's send row to the column, under Auto CQ,
  visible only in JTTY (`JttyPanel::haltButton()` handed to MainWindow).
- **Exchange per macro set** (`MacroSet::exchange`, array key
  `exchange`). Migration: built-in sets take theirs ("599 %N" contest,
  "599" ragchew), other sets the old global `Exchange` (Manoj's was
  "599"). The global Exchange box is gone; "%E sends" sits under the
  set's macros. **Serial Number** (label + box) on the panel shows only
  when the active set uses %N in its exchange or a macro.
- **%OP %QTH %TX %ANT**: `[JTTY] OpName, Qth, Radio, Antenna`, edited in
  a "Station details (for macros)" box on the tab, stored upper case.
  `jttyMacro` now expands %E first, then these, then %M/%H/%Q/%N, so
  %QTH is not eaten by %Q. An empty one blocks the send with a status
  message naming the field.
- **Send message** entry: font ×1.15 and 6 px taller.
- Verified in throwaway instances: layout in JTTY (Auto CQ + Halt under
  AnsB4, no serial with a "599" set) and FT8 (Auto CQ only); F4 =
  "OP %OP QTH %QTH RIG %TX ANT %ANT" sent "OP MANOJ QTH BANGALORE RIG
  FLEX 6600 ANT HEXBEAM"; Halt in the column stopped it; the tab shows
  station details and the per-set exchange.

### 2026-10-01 — JTTY Auto CQ and a Settings > JTTY tab

Manoj: JTTY only sends once; wants auto CQ with a settable gap (default
10 s), and a JTTY page in Settings for auto CQ, macro sets, etc.

- **`jttysettings.{h,cpp}`** (new): `JttySettingsPage`, added as the last
  tab of Settings by `Configuration` (loaded on every `exec()`, saved on
  OK, then `jtty_settings_changed` → `JttyPanel::reloadMacros`). Shared
  `[JTTY]` helpers in `namespace JttySettings`.
  - Auto CQ: macro key (F1), gap (10 s, 1–300), stop after N calls
    (0 = no limit), stop when my call is decoded (on).
  - Exchange: what %E sends, default `599 %N`; %N is dropped when the
    panel's Serial Number is "none".
  - Macro sets: `MacroSets` array (name, Msg1..8) + `ActiveMacroSet`.
    First read migrates the panel's old `[JTTY] Msg1..8` into "Default"
    and adds the built-in "Contest (WSJT-X)" and "Ragchew / DX" sets.
    New / Rename / Delete / Reset to built-in. Panel edits write into
    the active set.
- **Panel:** Auto CQ toggle beside Halt (green when on).
- **Engine** (`mainwindow_jtty.cpp`): sends the macro, and when
  `jttyUpdateTxState` sees the transmission end, waits the gap and sends
  again. Stops on Halt/Esc, any other send (`jtty_tx` not from Auto CQ),
  DX call picked or typed, the call limit, a decode with my call as a
  word (not one starting CQ/QRZ, not "DE MYCALL", and not while
  transmitting, so my own CQ heard back doesn't count), a macro that
  can't be sent, or leaving JTTY. Status bar says why.
- **Verified** in a throwaway instance (Rig None, audio to the Mac mini
  speaker, gap 3 s, limit 2): two CQs 3 s apart, then "Auto CQ stopped:
  2 call(s) made"; Halt mid-call stops it and nothing follows; the tab
  shows and saves; switching to "Ragchew / DX" and OK updates the panel.
  Stop-on-my-call and stop-on-pick are not exercised yet.
- Note for testing: F2 is a JTTY macro, so Settings must be opened from
  the app menu (Preferences...) while in JTTY.

### 2026-10-01 — crash when quitting during TCI start-up (inherited from JTDX)

- **Crash:** SIGSEGV in `QEventLoop::exec` under `TCITransceiver::do_start`
  on quit (10:35 today; two identical ones from the stock 2.2.159 build
  on 09-28). TCI's `mysleep1/2/3` wait in nested `QEventLoop`s on the rig
  thread; the queued `stop()` from `close_rig()` ran *inside* that wait,
  `do_stop()` deleted the very loop (and timers, commander) the waiting
  code then returned into. Happens whenever TCI is (re)connecting at quit,
  e.g. the TCI server was closed first.
- **Fix:** `TransceiverBase::stop()` checks a new virtual
  `in_nested_wait()`. If set, it sets `stop_aborting_`, calls
  `abort_waits()` (TCI: stop timers, quit running loops) and re-queues
  itself with `QTimer::singleShot(0)`. While aborting, TCI sleeps return at
  once, so the interrupted operation unwinds in milliseconds; the
  re-queued stop then tears down normally (flag cleared first, so its own
  waits work). `offline()` doesn't emit `failure` while aborting, so no
  "Rig failure" box on quit. Other rigs don't override the hooks.
- **Verified** with `tools/silent_tci.py` (a websocket server that
  completes the handshake and never answers) and
  `tools/run_quit_test.sh <jtdx binary> <delay>` (throwaway `-r tcitest`
  instance, quits through that PID's own Quit menu item N s after the TCI
  connect, reports exit status and new crash reports): the old v0.3.0
  binary segfaulted with the same stack; the fixed build exited 0 with no
  crash report at 0.1, 0.5, 1.0, 1.4 and 1.7 s.
- Installed 2026-10-01 (live app had been down since the 10:35 crash).
- **Also on Linux (meridianpi5, Xvfb):** `tools/run_quit_test_linux.sh`
  (File > Exit by xdotool clicks, since keys don't reach the window
  without a window manager). The v0.3.0 `.deb` binary segfaulted (exit
  139) at 0.4 and 0.9 s after connect; the fixed build exited 0 at 0.4,
  0.7, 1.1 and 1.5 s. The fake server makes start-up fail at ~1.7 s with
  a modal "Rig Control Error", so quits have to land before that.
- Three orphaned `jtdxjt9 -s "JTDX-VU - fittest"` helpers from the
  09-30 Settings tests were found still running on the Pi and killed.
  Test scripts must stop the helper too, not just jtdx.

### 2026-10-01 — JTTY QSO start time

- JTTY never set `m_dateTimeQSOOn` (FT8's auto-sequencer does that), so
  every JTTY QSO logged a stale start: four QSOs on 09-30 all have
  TIME_ON 17:32:11, and IK0QKN has start = end. `on_dxCallEntry_textChanged`
  now stamps the start whenever the DX call changes in JTTY (picked or
  typed). The log handler's "more than 10 periods apart → start = end"
  rule still applies (1200 s for JTTY's nominal 120 s period).
- Built, not yet tested in a GUI instance (Manoj was on air) and not
  installed.
- **RUMlog clean-up of existing JTTY QSOs** is Manoj's to do in RUMlog
  (never write its database). From `wsjtx_log.adi` there are seven:
  EA1BAF, IK3CHK (09-28), IK0QKN, YO4CVV, EA3NE, N8DC, LZ2HV (09-30), all
  20m. Mode → MFSK / submode JTTY; LZ2HV's reports were logged -15 / -15
  and should be 599 / 599. Their start times are wrong too; RUMlog keeps
  the end time since 2026-08-28, which is right. Reading RUMlog's store
  from Claude's shell is blocked by macOS container privacy, even with
  Full Disk Access on Claude.app (the `disclaimer` helper makes the
  versioned claude-code binary the responsible process); checked through
  RUMlog's window with computer-use instead ("Operation
  not permitted" on the container even outside Claude's sandbox), so the
  RUMlog side was not checked. It needs the Claude app allowed under
  System Settings > Privacy & Security (Full Disk Access, or "App
  Management"/data from other apps) — Manoj's call.

### 2026-10-01 — JTTY logs as ADIF MFSK / JTTY

- JTTY QSOs were written as `<MODE:4>JTTY`, which LoTW/TQSL don't know
  yet. Both ADIF writers (`logqso.cpp` for the logged-ADIF UDP message
  to RUMlog, `logbook/adif.cpp` for the log file) now treat JTTY like
  FT4 and FT2: `<MODE:4>MFSK <SUBMODE:4>JTTY`. That is the form proposed
  for ADIF 3.1.8, per a search summary Manoj pasted; not checked against
  the ADIF spec itself.
- The ADIF reader already maps MFSK + SUBMODE back to the mode name, so
  worked-before matches both new entries and older `MODE:JTTY` ones.
- QSOs logged before this change stay as MODE JTTY in RUMlog; fix them
  there (or in the .adi) before a LoTW upload.
- Built, not installed: the installed app is still `b6480391`, which
  Manoj is testing on air.

### 2026-09-30 — JTTY: click to pick a call, %E without a serial, RST 599 in the log

Manoj on air: couldn't pick a call from JTTY text; %E always sent
"599 001"; Log QSO showed -15 / -15.

- **Picking a call** (`mainwindow_jtty.cpp`, `jttyPickCall()`):
  - Reproduced in a throwaway instance with a generated wav (the TX
    harness taking the message as an argument, then `JTDXVU_OPEN_WAV`).
    Double-click worked on a clean call, but "VU2CPL," failed ("doesn't
    look like a callsign"): RTTY text glues punctuation to calls.
  - Leading and trailing punctuation is now stripped; a slash inside
    stays (VU2OY/P).
  - A **single click** (N1MM style) or a mouse **selection** also picks
    a call: `DisplayText` emits `leftClickReleased()`, handled only in
    JTTY. Single clicks and selections on non-calls are silent; a
    double-click still says why.
  - Both JTTY renders now edit through a document cursor and only
    follow the end when the view is already at the bottom. Before, every
    new character reset the widget cursor and jumped to the bottom, so
    a call scrolled away while being aimed at on air.
  - Verified by clicking in the test instance: "VU2CPL," → VU2CPL,
    JA1ABC from the Rx Frequency pane, "MANOJ" ignored, double-click on
    VU2OY/P. The no-jump render is not yet seen with live on-air text.
- **%E:** Serial Number now goes down to 0, shown as "none"; then %E is
  just "599". Any serial above 0 keeps WSJT-X's "599 %N".
- **Log QSO in JTTY:** RST sent / received are 599 / 599 instead of the
  FT8 dB reports. Verified in the test instance (dialog cancelled).
- Not released. Ships with the next minor version.
- Test gotcha: the live `/Applications/JTDX-VU.app` and a build-dir test
  instance are both process `jtdx` to System Events. Address test
  windows by **unix id**, or keystrokes land in the live app. A
  build-dir instance also needs `jtdxjt9` beside it
  (`build/jtdx.app/Contents/MacOS/`), or dismissing the subprocess
  error quits it.

### 2026-09-30 — Settings dialog fits small screens (Linux bug report)

- **Report:** a Linux user on v0.3.0 (VU2OY on screen) could not see the Settings
  dialog's OK / Cancel buttons. A silent phone video only showed the
  main window being dragged about, so the cause was found by testing.
- **Cause:** the 2026-09-28 macOS fix makes the dialog at least its
  layout-derived `minimumSizeHint()`. With Linux fonts that is about
  849x894, taller than a 1366x768 laptop, and the minimum stops the
  window manager shrinking it. The button row ends up below the screen.
- **Fix** (`Configuration.cpp`, `fit_to_screen()`, called from
  `read_settings()` and on every `exec()`): when the needed size fits
  the screen's available area (less 16x48 px for the frame) nothing
  changes. When it doesn't, the tab widget is moved into a
  frameless `QScrollArea`, widened by the scroll-bar extent so no
  horizontal bar appears, the layout is re-activated so the new
  minimum takes effect, and the dialog is capped to the screen and
  moved fully onto it.
- **Verified on meridianpi5** under Xvfb 1366x768 (no window manager),
  in a throwaway `-r fittest` instance:
  - released v0.3.0 `.deb` binary: Settings opens at 849x894, buttons
    off-screen — the bug reproduces;
  - patched build: 863x720, tabs scroll, OK / Cancel visible.
  - The Mac build compiles; on a large screen it takes the unchanged
    branch.
- Not in any release yet. Manoj will ship it in the next minor
  version bump along with other fixes.

### 2026-09-29 — FT2 mode (branch `jtty`)

FT2 is IU8LMC's mode as shipped in MSHV: FT4's protocol at twice the
speed — same 77-bit payload, LDPC(174,91), 103 channel symbols with the
four 4×4 Costas arrays, GFSK BT=1 — with 288 samples/symbol at 12 kHz
(41.67 baud), a 3.75 s T/R period and MSHV's dial frequencies
(1843/3578/5360/7052/10144/14084/18108/21144/24923/28184/50320/70159/
144177 kHz). Confirmed by diffing MSHV's `decoderft2.cpp`/`gen_ft2.cpp`
against its FT4 files: apart from LZ2HV's channel-estimation and
averaging extras (not ported), every difference is a halved constant.

So the port is a **scaled clone of JTDX's own Fortran FT4 chain**, not a
transliteration of 2 700 lines of C++:

- `lib/ft2/` — `ft2_params.f90` (NSPS 288, NMAX 41472 = 12 blocks of
  3456 = 3.456 s of the 3.75 s period, NFFT1 1408, NDOWN 9 so the
  downsampled rate is 1333.33 S/s and NSS stays 32), `getcandidates2`,
  `ft2_baseline`, `ft2_downsample`, `sync2d`, `get_ft2_bitmetrics`,
  `subtractft2` (NFILT 700, MSHV's), `gen_ft2wave` (own pulse cache —
  `gen_ft4wave` caches its pulse for the first nsps it sees),
  `agccft2`, `partintft2`, `ft2sim`, `tools/ft2dec.f90`.
  `lib/ft2_decode.f90` and `lib/ft2_mod1.f90` (`ddf2` buffer) mirror
  the FT4 pair. The encoder/tone mapping is FT4's `genft4` verbatim.
- DT search: three segments over −0.52..+1.52 s (MSHV's FT2 window);
  `xdt = ibest/1333.33 − 0.5` — TX starts 0.5 s into the period as
  FT4 does (Modulator/TCI `delay_ms=500` for nsps 288), which is what
  MSHV's decoder assumes too.
- **Two things that were not just halving:** the candidate smoother
  had to stay ±1.8 baud (±9 bins of 8.52 Hz) or the four tones 125 Hz
  apart never merged into one peak (first cut found candidates 13–30
  Hz off and the fine search pinned at −16 Hz); and the reported SNR
  needed its own constant (−11.3 instead of FT4's −14.8, measured
  against `ft2sim` over 24 files: FT4's read 3.5 dB low).
- `decoder.f90`: `nmode=2` branch (filter guard ±178 Hz = 167 Hz
  signal + 11), `ft2_decoded` callback, `partintft2`; `jt9a.f90`:
  npts1 41472 and the `ddf2` copy. `twkfreq1` reads `ft8_mod1`'s
  `twopi`, which the decoder process sets via `cwfilter` on its first
  call — the standalone `ft2dec` has to set it itself (an hour lost to
  a fine search that silently did nothing).
- GUI: `Modes::FT2`, code-created `m_actionFT2` after FT4 in the Mode
  menu, `ft2ModeSelected()` (hsymStop 12, TRperiod 3.75, MSHV's
  `#5cebdc` label), FT2 twins of every `"FT4"` site in mainwindow.cpp
  (decode trigger, dupe guard 3 s, wav length 12×3456, partial-interval
  delay, nmode, guard 168 Hz, band-change 3 s, PSK Reporter mode
  string, txDuration 3.52, wave generation `genft4_`+`gen_ft2wave_` at
  1152 sps, clock colour per 3.75 s slot, QSO-off +4 s, TCI/modulator
  start with 288 sps and toneSpacing −2, watchdog 8 s, "TX 3.75"),
  plotter (bandwidth, filter marks, 17 lines/period), `displaytext`
  ":" marker, ADIF `MFSK`/`FT2` submode as MSHV logs it, switcher
  `all_modes`, default frequencies seeded on first entry like JTTY.
  The TCI `ft4_mode` polling-suppression flag is left off for FT2 (its
  second-of-minute table doesn't map to 3.75 s).

**Verified:** `ft2sim` → `ft2dec` decodes to −14 dB (FT4's −17.5 less
the 3 dB of half the symbol energy — on the mark), exact frequency and
DT over −0.4..+0.8 s; the real `jtdxjt9` path decoded the −10 dB file
in a throwaway `-r ft2test` instance (`-10 0.0 1501 : CQ VU2CPL MK82`
in ALL.TXT, both panes, "TX 3.75", `FT2` status); the GUI TX chain
(`genft4` → `gen_ft2wave` 48 kHz → decimate; `lib/ft2/tools/txtest_ft2.f90`,
compiled by hand like the JTTY one) decodes back through `ft2dec`. **Not yet on the air** — the first test should be with an
MSHV FT2 station (VUCG runs them), watching DT: a systematic ±0.25 s
would mean the start-of-period convention differs from MSHV's.

Left out on purpose: MSHV's Wiener channel estimation and FT2
averaging (decoder-side extras, no protocol impact); the multi-slot
250 Hz TX; SWL windows are simply wider guesses.

### 2026-09-28 — JTTY: first QSO on air; rc1-style controls page (branch `jtty`)

Manoj keyed up on 20 m through the TCI station, ran a CQ and worked one
station (ALL.TXT `202609_ALL.TXT`, 17:36–17:39 UTC) — the phase-3 TX
path works on the air. Three things came out of that session:

- **F-keys did nothing:** JTDX's menu actions own F1/F2/F3/F5/F6/F7 as
  shortcuts, so the key never reached `keyPressEvent`. `JttyPanel` now
  installs a `qApp` event filter: while the panel is visible and its
  window is active, F1–F8 (no modifier) and Esc are accepted at
  `ShortcutOverride` and handled at `KeyPress` (macro / Halt). Other
  windows (Settings, Log QSO) are untouched by the `activeWindow()` gate.
- **The FT8 Tx-message tabs on the right were useless in JTTY.** The
  JTTY panel is now an extra page of `controls_stack_widget`
  (`m_jttyStackIndex`, added in code), current while JTTY is selected
  and swapped back to page 0 on leaving; `WSPR_config` still owns the
  WSPR page. The panel was rebuilt to WSJT-X 3.2.0-rc1's JTTY page:
  F Tol / Lower case / Include time; F1–F4, their **editable** macro
  fields, F5–F8, theirs (`[JTTY]/Msg1..8`, defaults = rc1 native
  templates); Send message + entry + Halt; **Call next** (`%Q`) and
  **Serial Number** (`%N`, `[JTTY]/SerialNumber`). `%E` expands to
  `599 %N`; `%Q` falls back to the DX call. The old Exch field is gone.
  As in rc1, sending a message that starts with `TU ` bumps the serial
  and opens the Log QSO dialog (when a DX call is set); Lower case also
  lower-cases what is sent.
- **Double-click on a decode** in JTTY takes the slash-joined word
  under the pointer and, if it looks like a callsign, puts it in DX Call
  (rc1 puts the raw word there; we add a callsign-shape check so a
  double-click on "599" doesn't clobber the entry). `doubleClickOnCall`
  returns before the FT8 message parser.
- `m_currentMessage` is set on Send so the `Transmitting …  JTTY:` line
  in ALL.TXT carries the text (it was blank tonight).

Layout checked in a throwaway `-r jttylayout` instance (window capture
via `screencapture -l <id>`, ids from `tools/macos-window-id.swift`, a 10-line Swift
`CGWindowListCopyWindowInfo` script — `osascript` lost assistive access
this session). Installed once Manoj quit; **F-keys confirmed working on
the air** the same evening.

### 2026-09-28 — JTTY phase 3: transmit (branch `jtty`)

JTTY has no T/R period, so TX could not go through JTDX's slot-timed
`m_bTxTime` window. Design: the GUI generates the whole 48 kHz wave and
plays it through the existing pre-generated-wave path (`foxcom_.wave`,
`toneSpacing<0`) that stock JTDX already uses for FT8 F/H.

- **`jttypanel.{h,cpp}`** rewritten: row 1 F Tol / Lower case / Include
  time / **Exch** field (`%E`, default `599 001`, `[JTTY]/Exchange`);
  row 2 **F1-F8** with WSJT-X 3.2's native templates (`CQ %M CQ`,
  `%H %E`, `%H TU CQ %M CQ`, `%M`, `%H`, `TU NOW %Q %E`, `%H AGN?`,
  `%E`; `%Q` is treated as `%H`); row 3 free-text entry + Send + Halt.
  Signals only; MainWindow expands the macros (`jttyMacro`) from
  `my_callsign`, `dxCallEntry` and Exch, with status-bar guards.
- **`jtty_tx(QString)`** (`mainwindow_jtty.cpp`): 80-char frame →
  `genjtty_profile_` (profile 0, `itone[16*59]`) → `gen_jttywave_`
  (nsps 1536, BT 2.0, 48 kHz, f0 = TxFreq − XIT) into `foxcom_.wave`,
  then opens a 3 s request window (`m_jttyTxRequestedUntil`). Writes
  `JTTY Tx <f>  <text>` to ALL.TXT, puts a `Tx: …` line (negative
  message id) into the Rx Frequency pane, sets the Tx status label.
  Refuses while a message is still pending or playing (no chaining
  yet). Halt → `haltTx("JTTY halt ")`.
- **Keying (guiUpdate):** for JTTY `m_bTxTime = jttyUpdateTxState()`:
  true from Send until the modulator has come up and gone Idle again
  (hard stop at wave length + 1 s if it never reports Idle). It is
  OR-ed into the `m_transmitting or m_enableTx or m_tune` gate, so a
  JTTY Send keys up **without Enable Tx**, exactly like Tune; the
  autoseq `process_Auto()` call is skipped in JTTY.
- **`transmit()`** JTTY branch: `sendMessage`/`transceiver_modulator_
  start(m_jttyNsym, 384.0, TxFreq−XIT, −4.0, synchronize=false, …)`.
  384 samples/symbol at 12 kHz — the Modulator scales ×4 — so its
  loop end `i1 = nsym·4·nsps` lands exactly on the wave's last sample.
- **Both Modulator copies** (`Modulator.cpp`, and TCITransceiver's
  private copy) got the same three changes: `synchronize=false` now
  means "period-free: start at `m_ic=0` now" (`m_periodFree`); a
  period-free wave sets `m_amp=0` once `m_ic > i1`, so the Idle
  transition fires (in wave mode the loop pins `m_amp` at full scale
  and stock FT8 relies on the T/R window to drop PTT — JTTY has none);
  and the tone block's `itone[isym]` index is clamped to the 162-entry
  global, which a 944-symbol JTTY wave would otherwise overrun.
  Periodic modes are unaffected: they always pass `synchronize=true`.
- **Buffers:** `foxcom_.wave` grew from 606720 to 1450000 floats
  (`commons.h` + `lib/foxgen.f90`, must match) — 16 frames × 59
  symbols × 1536 = 1.45 M samples for the longest frame.
- **Tx audio frequency** set to 1500 on JTTY entry, like Rx.
- **Band buttons did nothing in JTTY** (first on-air attempt): the band
  combo is filtered to the current mode and Manoj's saved frequency
  list predates JTTY, so it had no JTTY rows and `switch_to_band` found
  nothing. `FrequencyList_v2::add_defaults_for_mode()` /
  `Configuration::seed_default_frequencies()` now append the shipped
  JTTY defaults on JTTY entry when the list has none for that mode —
  into both `frequencies_` and the dialog's `next_frequencies_`, or the
  next Settings > OK would write the stale list back. Reuse for FT2.

**Verified offline only:** a Fortran harness (`lib/jtty/tools/txtest_jtty.f90`, compiled by hand against `build/libwsjt_fort.a`) calling the exact
GUI sequence (`genjtty_profile` → `gen_jttywave` at 1536 sps/48 kHz,
decimated ×4) for `CQ VU2CPL CQ` gives 59 symbols / 1.888 s, and
`rjtty 0 0 384 1500 100` decodes it back as `1500  CQ VU2CPL CQ`.
The GUI key-up (PTT, modulator start, self-termination, Halt) has
**not** been exercised yet — first on-air test should be with Rig=None
+ sound card into a dummy load, then the TCI station. Things to watch:
PTT dropping ~1 s after the audio ends (the Idle → `m_btxok=false` →
`stopTx()` chain), Halt mid-message, and a second Send right after the
first.

### 2026-09-28 — JTTY phase 2: RX works in the GUI (branch `jtty`)

JTTY is WSJT-X 3.2's period-free, RTTY-style keyboard mode. Phase 1
(2026-09-28, `081acdf9`) vendored its Fortran library; this phase wires
RX into JTDX's GUI. **All on branch `jtty`; `jdx-vu` is untouched.**

- **Mode entry:** `Modes::JTTY` (+ name) appended to the enum, so the
  frequency-list filter works and saved lists keep their numbering.
  `m_actionJTTY` is created in code into the constructor's `modeGroup`
  and the Mode menu (no `mainwindow.ui` change); the switcher gets a
  JTTY button; the three mode-restore sites call `jttyModeSelected()`.
  Named that way, not `on_actionJTTY_triggered`, so Qt's auto-connect
  doesn't warn about a code-created action.
- **No T/R period:** JTTY has none. As WSJT-X does, a nominal
  `m_TRperiod` keeps the detector ring buffer and waterfall cycling and
  `rjtty_sub` restarts on each wrap. JTDX's buffer is `NTMAX`=120 s
  (WSJT-X uses 180 with a larger one), so 120 s fills it exactly.
  `m_hsymStop=417` because `diskDat()` sweeps a wav in `m_FFTSize`
  steps up to `m_hsymStop` with its `k > kin` guard commented out;
  `jtty_decode` clamps `k` to the buffer and to `kin` as well.
- **Decode hook:** `dataSink()` calls `jtty_decode(k)` before
  `symspec_` on every audio chunk, keeps the waterfall, then returns
  before `setStopHSym()` — jtdxjt9 never runs for JTTY. An unchanged
  `k` returns early (no new samples); only `k` going backwards starts
  a new display session. (First cut treated the clamped-`k` tail of a
  file sweep as a wrap and wiped the decodes.)
- **`mainwindow_jtty.cpp`:** port of WSJT-X's `jtty_decode` — polls
  `jtty_get_updates_`, merges by message id, re-renders the Band
  Activity group (tracked start block, remove-to-end, reinsert) and the
  Rx Frequency group (messages within F Tol of `RxFreqSpinBox`), writes
  `JTTY Rx <freq>  <text>` to ALL.TXT on end-of-message. Left out:
  N1MM/MMTTY bridge, transmit evidence, replay, windowed re-decode.
  `extern dec_data_t dec_data;` — JTDX defines the instance in
  `mainwindow.cpp` with C++ linkage. Structs use field-by-field init:
  JTDX is `-std=c++11`, where NSDMI structs aren't aggregates.
- **`jttypanel.{h,cpp}`:** F Tol ladder (WSJT-X's), Lower case, Include
  time; `[JTTY]` settings; mounted under the switcher, shown by
  `commonActions()` only in JTTY. Rx audio frequency set to 1500 on
  mode entry (JTTY convention).
- **Default dial frequencies** seeded from WSJT-X 3.2 (1838, 3575,
  7090, 10140, 14090, 18100, 21090, 24920, 28090 kHz) — defaults only;
  an existing saved list doesn't pick them up.
- **`JTDXVU_OPEN_WAV=<file>`** opens a wav 4 s after start (File > Open
  body refactored into `openWavFile()`), for deterministic decode tests.
- **`rjtty`** — WSJT-X's standalone JTTY file decoder — is now a build
  target (`build/rjtty smin ndebug nsps f0 ftol file`), the reference
  for the GUI's decodes. Keep it for FT2 too.

**Verified** on the rc1 sample `samples/JTTY/260807_134110.wav` (30 s,
12 kHz) in a throwaway instance with `Mode=JTTY`, `Rig=None`:
- `1507  RAN ALL NIGHT ON BAND NOISE - NO FALSE DECODES!` in ALL.TXT
  8 s after start; in-place growth, panel, mode label and mode restore
  all fine.
- **Ground-truth correction:** the rc1 integration test synthesises its
  `CQ TEST DE KA1ABC…` audio with `sjtty`; the sample wav has no
  published reference. `rjtty 4.6 1 384 1500 50` on the sample gives
  the identical message at ~1506 Hz over 10 frames (3.1–20.1 s).
- A second, incomplete line `1696  4>-P'` is a **single false frame**
  the reference decoder produces too (1695.6 Hz, t 24.4 s, −16 dB,
  nsync 9, 27 hard errors): inherent to the decoder on this file, not
  the port. It never reaches ALL.TXT (no EOM). WSJT-X would show it as
  well; left as a faithful port.

**Open (phase 3, TX):** `genjtty_profile_` → `gen_jttywave_` (nsps 1536
at 48 kHz, bt 2.0) → PCM; play through the Modulator's pre-generated
wave path (`m_toneSpacing<0` reads `foxcom_.wave`, 606,720 floats =
12.6 s; a 16-frame message is 1,449,984 → enlarge). Immediate,
period-free TX start (JTDX starts TX on period boundaries). **Manoj's
station uses TCI audio** (`TCIAudio=true`) — the TCI transmit path
must carry it. Text entry, Send, F1–F8 templates. Then credits/README.

### 2026-09-28 — DXCC colours: New DXCC / on Band / on Mode (MSHV axes)

- Settings → Notifications now has a third DXCC colour, **"New DXCC on
  Mode"**. The existing button was relabelled "New DXCC on Band".
  - The new button is a plain grey button with K1ABC previews on the CQ
    and My Call colours, like the other rows. It is created in code
    (`Configuration::impl` constructor): the nested colour grid is
    found and the rows below "New DXCC on Band" are shifted down one,
    so `Configuration.ui` wasn't restructured.
  - Settings keys: `colorNewDXCCMode` (#ffc070) and
    `colorNewDXCCMode_dark` (#a06010). Getter:
    `Configuration::color_NewDXCCMode()`.
- `displaytext.cpp`: DXCC status uses MSHV's independent axes instead of
  JTDX's band+mode slot:
  - "per band" → not worked on this band (`dxccBandB4`) → band colour.
  - "per mode" → not worked in this mode on any band (`dxccModeB4`) →
    mode colour.
  - An ATNO keeps the New DXCC colour. Priorities are unchanged
    (band/mode 20–21).
- Checked visually in a throwaway instance.

### 2026-09-28 — Show dropdown: All / New DXCC / New band / New mode

- Manoj wants the dropdown exactly as in MSHV, with no cumulative levels
  and no band+mode slot. `DisplayText::needed` is now one test per
  option:
  - 1: DXCC never worked
  - 2: not worked on this band (`matchDXCC` with frequency only)
  - 3: not worked in this mode on any band (`matchDXCC(…, 0, mode)`)
  - An ATNO is "new" for all three.
- Combo labels are "All", "New DXCC", "New band", "New mode". The stored
  `[Switcher] NewOnly` index keeps its number and now means those.

### 2026-09-28 — band/mode buttons re-assert DIGU; band press retry

- **TCI exit restore:** a change to stop JTDX restoring the start mode
  (LSB) on exit was made, then **reverted** at Manoj's request. Restoring
  on exit is fine; he wants DIGU back on use.
- **Why a same-band press didn't fix the mode:**
  `Configuration::transceiver_frequency` only sends when the frequency
  differs or its *cached* mode ≠ the configured one. The cache says
  DIG_U even after the radio went to LSB, and `sync_transceiver` is a
  stub in JTDX. New `Configuration::force_rig_mode(f)` sets the cached
  mode to UNK and re-sends frequency plus mode. TCI's `do_frequency` then
  sends `digu` whenever the radio reports anything else.
- **Band button:**
  - Current band: re-tune to the mode's default frequency plus
    `force_rig_mode`.
  - Other band: switch, and if the band still hasn't changed after
    3 s, press it once more. TCI `do_frequency` returns early while
    `busy_rx_frequency_`, which dropped presses.
- **Mode button:**
  - Mode change: trigger the mode, then `force_rig_mode` after 1.5 s.
  - Same mode: `force_rig_mode` straight away.
- The cause of LSB after restart isn't pinned down (JTDX does ask for
  `digu` at connect; the TCI server on :7374 is RUMlogNG). The buttons
  are the practical fix. **Not yet verified on air.**

### 2026-09-28 — "mode" level is MSHV's new mode (any band)

- Manoj wants the levels exactly as in MSHV: **new DXCC** = never worked;
  **new band** = not on this band; **new mode** = not in this mode on
  **any** band. The third level had been JTDX's band+mode slot.
- In the Show filter (`DisplayText::needed`, level 3) and the third
  alert (`ClubLog::check_decode`, now `Level::NewMode`), the lookup is
  now `matchDXCC(call, ..., 0 /*no freq → no band*/, mode)`, which
  resolves to `matchCountry(country, "", mode)`.
- The alert checkbox reads "New mode", with the setting key `NewMode`.
  The old `NewBandMode` value is read once as a fallback, then removed.
- JTDX's own Settings → Notifications "per band / per mode" colouring is
  unchanged (JTDX semantics).

### 2026-09-28 — "Show:" filter is live, like MSHV

- Before, the filter decided at insert time: filtered decodes were never
  added, so changing the selection only affected later cycles.
- Now every decode line is inserted and **tagged**: `LineMeta :
  QTextBlockUserData` holds the call, dial freq and mode, plus
  `alwaysShow` for my-call traffic, the current QSO partner, JT65
  broadcasts and bypass. Filtered lines are **hidden** with
  `QTextBlock::setVisible(false)`, not dropped. A standalone Qt test
  confirmed QTextEdit's layout collapses hidden blocks (68 → 48 px).
- `DisplayText::setNewOnly` re-walks every tagged line at once.
  `refilter(LogBook)` runs from `MainWindow::init_logbook` (start,
  Club Log refresh, log reload) and after `addAsWorked`, so a
  just-worked station drops out of the "New ..." views. Neededness
  uses `LogBook::matchDXCC` at levels 1–3, as before.
- Alerts, beeps, popups and the `inotified` "shown" bit (UDP filter)
  apply to visible lines only. The filter is **display-only** now, as
  in MSHV: hidden callers still reach `qsoHistory`/AutoSeq, and
  AnsB4/worked-before decides whom AutoSeq answers.
- **Confirmed working by Manoj on air, 2026-09-28.** The release is still
  on hold until he's also happy with CNS (his instruction).

### 2026-09-28 — Non-stop option (keep calling, watchdog-limited)

- **Why auto-continue never happened:** in stock JTDX, with **auto-log
  on**, the end of every QSO halts Tx (`FIN, end of QSO, Autolog`,
  also `S73`/`SRR73` → `autoStopTx` → `haltTx`). The "QSO finished →
  clear DX → next caller" branch in `process_Auto()` runs only when
  logging is manual (`!m_config.autolog()`). So auto-log and
  auto-continue were mutually exclusive, whatever 1 QSO/AnsB4 were set to.
- **Non-stop**, button labelled **CNS** (Call Non-Stop) (`m_nonstop`; the button at the end of the
  switcher row and AutoSeq menu → "Non-stop (until Tx watchdog)";
  saved as `JTDXVU/NonStop`):
  - The continue branch also runs with auto-log once the QSO is logged.
  - `nonstop_continue()` is called at FIN/S73/SRR73, in place of
    `autoStopTx`. It clears DX and goes back to CQ (tx6), so AutoSeq
    answers the next caller. It never applies in single-shot or Hound
    mode, or while a manual-log QSO isn't logged yet.
  - ~~The give-up counters are ignored under CNS~~: **reverted after
    v0.2.0.** Manoj wants a station that doesn't answer dropped after the
    AutoSeq counters (his are `SeqAnswerCQCounterValue=3` and
    `SeqAnswerInCallCounterValue=3`). CNS no longer touches
    `nAnswerCQCounter`/`nAnswerInCallCounter`, and only replaces the
    end-of-QSO halt.
  - **The Tx watchdog is untouched.** It resets when a station calls or
    the Tx message changes, so it only fires after `watchdog` minutes of
    unanswered CQ ("WD 60m" in the status bar).
- **Watchdog fixed at 10 min while Non-stop is on** (Manoj's choice).
  `watchdog_minutes()` returns 10 when `m_nonstop` is set, even if the
  watchdog is disabled in Settings, and the Settings value otherwise.
  All former `m_config.watchdog()` uses in `mainwindow.cpp` go through
  it, including the "WD Nm" label. Toggling Non-stop refreshes the
  label.
- **Not yet verified on air.**

### 2026-09-28 — auto-log and next caller; confirmed-only no longer affects dupes

- **Manoj's goal** is MSHV-like: auto-log, then move straight to the
  next caller. JTDX does it with Reporting → auto-log (was already on;
  prompt off; clear DX on) plus AutoSeq (level 3).
  - The blocker was the **1 QSO** button (`SingleShotQSO`), which halts
    Tx after every QSO. He turned it off, and **AnsB4** too.
- **"Not picking the next caller" after AnsB4 off:** JTDX's autoselect
  (`mainwindow.cpp`, the `time |= 128` line) answers only "new"
  stations under the enabled New-DXCC/grid/prefix/call notifications
  when AnsB4 is off. He had **new DXCC + per band on and new call off**,
  so against the whole Club Log log almost every caller was
  "worked" and skipped.
  - Advice: also enable **new calls + per band**, so un-worked
    stations are answered (skip dupes) with new DXCC/band still first.
- **Bug fixed** (`logbook/adif.cpp` `_loadFile`): with Club Log
  "confirmed only", unconfirmed QSOs were skipped completely, so a
  worked-but-unconfirmed station counted as a *new call* and would be
  answered again as a dupe. Unconfirmed QSOs now still count for the
  callsign (dupe) status; only the DXCC/prefix/zone/grid status and the
  QSO count use confirmed QSOs.

### 2026-09-28 — logged QSOs not reaching RUMlog: stale DXCA address

- **Cause:** JTDX-VU's Reporting → UDP Server was `192.168.1.169:2334`,
  copied from stock JTDX's .ini (dated 2026-09-01). DXCA left noderedpi4
  (.169) for a Docker container on **ubersdr, 192.168.1.109**, on
  2026-09-22, and the .169 install is stopped. So nothing received
  JTDX's UDP, and nothing reached DXCA's verbatim passthrough to RUMlog
  (Mac 192.168.10.226:2237). Not a JTDX-VU bug; the client-ID rename is
  irrelevant because the passthrough forwards datagrams raw.
- **Fix (Manoj, in Settings):** UDP Server `192.168.1.109`, port `2334`,
  which is DXCA's JTDX source. `/api/status` on `.109:7580` had shown no
  JTDX source; live logging to RUMlog works again.
- **Backfill:** today's three QSOs logged before the fix (RU3FM,
  BA7LIP, LY1BZ, 15 m FT8, 09:31–09:34 UTC) were written from JTDX-VU's
  `wsjtx_log.adi` to an ADIF and imported in RUMlog with No dupes / no
  Update. **3 added.** EA1CK (09:35) arrived live.
- **Access note:** RUMlog's database (a sandboxed container) is
  "Operation not permitted" from the Claude app, so the MSHV-skill checker
  couldn't read it. The missing set came from Manoj looking in RUMlog.
  Granting the Claude app Full Disk Access would allow the checker.
- **Stock JTDX and WSJT-X updated too, at Manoj's request, the same
  day:** `JTDX.ini` → `UDPServer=192.168.1.109` (port 2334) and
  `WSJT-X.ini` → `192.168.1.109` (port 2335, DXCA's WSJT-X source). Both
  apps were closed at the time. Only that line changed; backups are
  `~/Library/Preferences/{JTDX,WSJT-X}.ini.bak-20260928-151629`.

### 2026-09-28 — Windows zip re-issued: TLS and desktop alerts

- **Manoj's first run of the v0.1.0 Windows zip** hit two problems:
  - Club Log refresh gave "TLS initialization failed", and Telegram
    also failed.
  - Desktop alerts never appeared.
- **TLS:** Qt 5 loads OpenSSL at run time (`libssl-3-x64.dll` and
  `libcrypto-3-x64.dll` on MSYS2), so neither fixup_bundle nor the ldd
  sweep copied it. `windows.yml` now installs `mingw-w64-x86_64-openssl`,
  copies both DLLs into `bin/`, logs the names QtNetwork asks for, and
  fails if libssl is missing.
- **Alerts** (`clublog.cpp`): `QSystemTrayIcon` was built from
  `QApplication::windowIcon()`, which is empty on Windows because JTDX
  sets its icon only via the .exe resources. Qt won't show an icon-less
  tray entry. It now falls back to a top-level window's icon, then
  `SP_MessageBoxInformation`, and delays the first message by 1 s so a
  new tray icon isn't dropped.
- The v0.1.0 Windows asset and its `.sha256` were replaced (`--clobber`)
  from run `36399210191`, and the release notes carry a re-issue line.
  The mac and Pi assets are unchanged. **Confirmed by Manoj
  2026-09-28: on Windows, Club Log refresh, Telegram "Send test" and
  Desktop "Send test" (tray toast) all work.**

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

- [ ] **Cache Hamlib in windows.yml.** It rebuilds from source every run
      and costs about 10 min.
- [ ] **Windows on Manoj's PC:** Club Log, Telegram and Desktop alerts
      are confirmed. Still to check: audio, CAT and OmniRig on air.
- [ ] **Settings dialog small-screen fix** released in v0.4.0; Manoj
      asked the Linux reporter by DM on 2026-10-01 to confirm on their
      real window manager. Waiting for the answer.
- [ ] **JTTY click-to-pick on live text:** confirm on air that a
      growing line holds still and a single click grabs the call.
- [ ] **JTTY to LoTW:** RUMlog holds the seven JTTY QSOs (checked in
      its window 2026-10-01): mode JTTY, 599/599 (LZ2HV included), end
      times correct, LoTW/eQSL still W. RUMlog shows FT4 as mode "FT4",
      so it keeps the submode as its mode and maps on export; don't hand-
      edit these to MFSK. Before uploading, check what RUMlog sends TQSL
      for JTTY (TQSL config update may be needed).
- [ ] **JTTY start-time fix** installed 2026-10-01; check the next
      JTTY QSO logs a sensible start time.
- [ ] **JTTY screen redesign (2026-10-02):** installed; try on air -
      Log QSO with edited S / R / Name, calls-heard colours with live
      decodes, Shift / Option F-keys on the Mac keyboard, type-ahead
      with PTT through TCI. Manoj to fill labels for his own "Default"
      set in Settings > JTTY if he uses it.
- [ ] **Even decode-pane split + resize fix** (after v0.5.0): installed
      2026-10-02, Manoj: "its fine now". Ship in the next release; check
      the minimum on a Linux / Windows font.
- [ ] **Auto CQ time limit** (after v0.5.0): ship in the next release;
      check on air in JTTY and with CNS in FT8.
- [ ] **JTTY Auto CQ on air:** installed 2026-10-01; check
      stop-on-my-call and stop-on-pick with real replies.
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

- **UDP / logging route:** JTDX-VU → DXCA at `192.168.1.109:2334`
  (ubersdr) → raw passthrough → RUMlog on the Mac at `:2237`. If QSOs
  stop reaching RUMlog, check `curl -s http://192.168.1.109:7580/api/status`
  `spots_per_source` for a JTDX source before anything else. A Windows
  JTDX-VU has its own settings and needs the same address.

- **Don't use `install/` as the CMake prefix.** On a case-insensitive
  volume it collides with the `INSTALL` file; use `build/dist`, which
  the script handles.
- **CMake's own `fixup_bundle`** in the install step fails on
  `@rpath/libsharpyuv`. That's expected, and the script tolerates it.
- **A `brew upgrade`** of qt@5, hamlib, gcc or webp only affects the
  next bundle, not the installed app. Rebuild and re-bundle after one.
