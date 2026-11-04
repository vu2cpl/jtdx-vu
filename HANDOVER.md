# HANDOVER — JTDX-VU

Last updated: 2026-11-04

## Current state

- **Repo:** `vu2cpl/jtdx-vu`, **PUBLIC since 2026-09-28** (after a
  secret scan, see below). Default branch `jtdx-vu`. The `upstream`
  remote is `jtdx-project/jtdx`; upstream has been dormant since
  2022-03 at tag 159.
- **Local files (2026-10-07):** `~/Desktop/jdxvu/` was cleared — the release copies, the `test/` trial zips and
  the `prev-install/` app and `.ini` backups that entries below point to are gone (marked "removed"); every
  release v0.1.0–v0.7.0 is on GitHub with its files. Correspondence, QSO files and other people's
  recordings live in `~/projects/JTDX-records/` (outside this public repo, with a README), not in
  `~/Downloads`.
- **Version:** JTDX-VU **0.7.5** released 2026-10-17; **`Versions.cmake` is now 0.7.6-rc7**
  (local test build 2026-10-22: rc6 plus the Flex key-up / reconnect fix; rc6 is the
  last GitHub pre-release and is installed on the Mac mini). rc1-rc7 are
  unreleased work for v0.7.6 (see "What changed" 2026-10-18 .. 2026-10-22); Manoj calls the release.
  Previous line, kept for history: JTDX-VU **0.7.5** (released 2026-10-17) (`JTDXVU_VERSION` in `Versions.cmake`),
  on JTDX 2.2.159. v0.1.0 was the first release; v0.2.0 adds CNS, the
  live Show filter and the Windows fixes; v0.2.1 makes CNS respect the
  AutoSeq give-up counters again; v0.3.0 adds JTTY and FT2; v0.4.0 adds
  JTTY Auto CQ, Settings > JTTY (macro sets, per-set exchange, station
  variables), click-to-pick calls, the one "Auto CQ" button, and fixes
  the TCI quit crash, the Settings dialog on small screens and JTTY
  logging (599, MFSK/JTTY, start time); v0.5.0 gives JTTY its own
  screen (QSO fields, calls heard, 24 macros, type-ahead); v0.5.1 adds
  the Auto CQ time limit and fixes window resizing and the pane split;
  v0.5.2 keeps CNS to our own CQ runs (off in Hound), switches Auto CQ /
  AnsB4 / 1 QSO off on a band change, and adds TCI auto-reconnect and a
  TCI audio watchdog; v0.5.3 puts Auto CQ under Monitor in every mode
  and stops the WD box covering status messages; v0.6.0 adds wanted-first
  AutoSeq, the right-click queue, the redesigned Notifications page with a
  New only tick and Background highlight, TX/RX sliders (TX follows the
  TCI drive), Auto CQ / 1 QSO exclusive, per-filter wanted hiding, keeps
  AnsB4 / 1 QSO on band change, and fixes the Fake It split runaway over TCI;
  v0.7.0 adds the FlexRadio VITA-49 rig type and panel, QO-100, profiles, the
  rig mode beside the frequency, FT2 decoding level with MSHV (demapper, AP7
  and its fixes), 70cm / 23cm buttons, and fixes the TCI reconnect and JTTY
  false-decode crashes; v0.7.1 (2026-10-08) only corrects the About box
  credits (FT2: IU8LMC + 9H1SR / Decodium; JTTY); v0.7.2 (2026-10-08) adds
  FT4 on 70cm / 23cm, the 8m button, one-time seeding of new default rows
  and the Auto CQ countdown on the button; v0.7.3 (2026-10-09) adds the
  GitHub release update check (Help > Check for Updates... plus a daily
  automatic check, hourly retry after a failure, off for "dev" versions;
  `e9796edb`, `0bd95894`), `%LOG` in a JTTY macro (`6fcbe714`), and the
  Auto CQ button fixes: fixed width, countdown all minute (`a00f05c6`,
  `e64be38d`); v0.7.4 (2026-10-10) adds the Flex `udp_register` (audio
  through a firewall / NAT / VPN), the no-audio reconnect stop, one frequency
  at connect, a clickable ERR box, the "FlexRadio:" box with Discover, Show
  US State, and the QO-100 13cm Club Log / JTTY lookups. Unreleased since
  v0.7.4: the country column lined up on bold lines; Canadian provinces in
  Show US State (both 2026-10-11); a third and fourth Flex copy join a GUI
  client's station; the Intel start-up crash fix; Settings boxes ignore an
  unfocused wheel (all 2026-10-13); no late Tx start after the first 20% of the
  period (2026-10-15).
- **Release v0.5.0: COMPLETE 2026-10-02** — https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.5.0,
  tag `v0.5.0` on `2a6c710f`. Notes carry a full per-OS "How to
  install" (same text as README's new section). All four builds plus
  `.sha256` attached; every asset downloaded anonymously and its
  checksum OK; arm64 app reports 0.5.0; copies in `~/Desktop/jdxvu/v0.5.0/` (removed 2026-10-07).
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
- **Release v0.6.0: COMPLETE 2026-10-04** — https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.6.0,
  tag `v0.6.0` on `b1592998`: wanted-first AutoSeq, right-click queue,
  current-decode rule, redesigned Notifications page + New only tick +
  Background highlight, TX/RX sliders, Auto CQ / 1 QSO exclusive, per-filter
  wanted hiding, AnsB4 / 1 QSO kept on band change, JTTY heard = senders,
  UDP errors on the status bar, Fake It split runaway fix. Notes say the
  queue, wanted-first, sliders and Auto CQ / 1 QSO are not yet tried on air,
  and that AetherSDR#6116 (TCI audio) is merged.
  - CI run 37221289182 (macOS) and 37221289166 (Windows), green. All
    four builds plus `.sha256` downloaded anonymously (200), every
    checksum OK, arm64 app reports 0.6.0 / Rev b15929; copies in
    `~/Desktop/jdxvu/v0.6.0/` (removed 2026-10-07).
  - Pi `.deb` on meridianpi5 (prefix /usr/local): Version 0.6.0,
    Conflicts: jtdx, `apt-get install -s` clean, uploaded by hand.
  - Website: paragraph "AutoSeq goes for what you need, a station queue
    and new Notifications settings (v0.6.0)", table / release link / apt
    command on v0.6.0, card sentence; pushed (`14806fc`) after all eight
    URLs answered 200.
  - Installed on the Mac mini 2026-10-05 from the verified release zip (reports 0.6.0, Rev b15929).
- **Release v0.5.3: COMPLETE 2026-10-03** — https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.5.3,
  tag `v0.5.3` on `bcad6f8c`: Auto CQ under Monitor everywhere
  (`41936ec2`), status-bar overlap fix (`776ef8e0`); notes add a "Known
  issue (AetherSDR)" section pointing at aethersdr/AetherSDR#6006.
  - CI run 37111236434 (macOS) and 37111236405 (Windows), green. All
    four builds plus `.sha256` downloaded anonymously, every checksum OK,
    arm64 app reports 0.5.3; copies in `~/Desktop/jdxvu/v0.5.3/` (removed 2026-10-07).
  - Pi `.deb` on meridianpi5 (prefix /usr/local): `apt-get install -s`
    clean, Conflicts: jtdx, uploaded by hand.
  - Installed on the Mac (local build of `bcad6f8c`, reports 0.5.3).
  - Website: paragraph "Auto CQ in one place (v0.5.3)" (with the
    AetherSDR #6006 note), table / release link / apt command on v0.5.3,
    card sentence; pushed (`f212028`) after all eight URLs answered 206.
- **Release v0.5.2: COMPLETE 2026-10-03** — https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.5.2,
  tag `v0.5.2` on `4ba63a58`. Cut three times the same morning, each
  time with Manoj's OK: `05360fe3` (CQ-only CNS), `ea5630c3` (+ TCI
  reconnect), `4ba63a58` (+ audio watchdog, band-change switch-off); the
  earlier two were deleted, so early downloaders may have an older 0.5.2.
  - CI run 37108417045 (macOS) and 37108417041 (Windows), green. All
    four builds plus `.sha256` downloaded anonymously, every checksum OK,
    arm64 app reports 0.5.2 with the band-change code; copies in
    `~/Desktop/jdxvu/v0.5.2/` (removed 2026-10-07).
  - Pi `.deb` from `4ba63a58` on meridianpi5 (prefix /usr/local, cmake
    re-run): `apt-get install -s` clean, uploaded by hand.
  - Website: project page paragraph "CQ-only Call Non-Stop, TCI
    reconnect and band-change safety (v0.5.2)", table / release link /
    apt command on v0.5.2, card sentence; pushed (`3e6be2d`, `e794f79`,
    rebased over the Club Log refresh) after all eight URLs answered.
  - The status-bar overlap fix (`776ef8e0`) came after the tag; it is in
    v0.5.3.
- **Release v0.5.1: COMPLETE 2026-10-03** — https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.5.1,
  tag `v0.5.1` on `89a3b8b5`. Ships the three fixes made after v0.5.0:
  Auto CQ time limit, main-window resize, even decode-pane split (see
  the 2026-10-02 entries). Notes = v0.5.1 changes + the v0.5.0 install
  guide with the version changed.
  - CI run 37055496531 (macOS) and 37055496515 (Windows), all green.
    All four builds plus `.sha256` downloaded anonymously, every
    checksum OK, arm64 app reports 0.5.1; copies in
    `~/Desktop/jdxvu/v0.5.1/` (removed 2026-10-07).
  - Website: card sentence + project page paragraph "Auto CQ time limit
    and window fixes (v0.5.1)", table / release link / apt command on
    v0.5.1; pushed (`7a9b778`) after all eight URLs answered 206.
  - Pi `.deb` built on meridianpi5 (rsync of `git ls-files`, prefix
    /usr/local, `nice make -j3`): `apt-get install -s` clean,
    Conflicts: jtdx, 8 s headless start OK, uploaded by hand with its
    `.sha256`.
  - **Installed on the Mac** 2026-10-03: local build of `89a3b8b5`
    (reports 0.5.1, macOS 26+ only). The previous local 0.5.0+fixes app
    is `~/Desktop/jdxvu/prev-install/JTDX-VU-0.5.0-local.app` (removed 2026-10-07). A stray
    `jtdxjt9` (PPID 1, from 22:20 the night before, main app gone) was
    holding the shared memory and was killed first.
- **Release v0.4.0: COMPLETE 2026-10-01** — https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.4.0,
  tag `v0.4.0` on `bf707bc8`. All four builds plus `.sha256` attached
  and verified (anonymous download, every checksum OK, arm64 app reports
  0.4.0); copies in `~/Desktop/jdxvu/v0.4.0/` (removed 2026-10-07).
  - CI run 36880480905 (macOS) and 36880480879 (Windows), all green.
    **The Intel keg cache works:** the Intel job took 10 min (14:57→15:08
    UTC) instead of 2.5–3.5 h. The arm64 job was the slow one (~50 min
    in Dependencies) but passed.
  - Pi `.deb` built on meridianpi5 with `CMAKE_INSTALL_PREFIX=/usr/local`
    (same as v0.3.0 — the README had said `/usr`, corrected), `apt-get
    install -s` clean, 8 s headless start OK, uploaded by hand.
  - Website updated the same evening (card + page, downloads table on
    v0.4.0), pushed after all assets answered.
- **Release v0.7.1: COMPLETE 2026-10-08** — https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.7.1,
  tag on `079026e7`; About box credits only. Details under What changed.
- **Release v0.7.2: COMPLETE 2026-10-08** — https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.7.2,
  tag `v0.7.2` on `0edfa30a`. Cut from the MSHV-Mac session (Manoj: "cut a new
  release. both JTDX and MSHV"); see "v0.7.2 released" under What changed.
- **Release v0.7.3: COMPLETE 2026-10-09** — https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.7.3,
  tag `v0.7.3` on `71c7bac8`: update check, JTTY `%LOG`, Auto CQ button fixes.
  Cut from the MSHV-Mac session on Manoj's OK; see "v0.7.3 released" under
  What changed.
- **Release v0.7.0: COMPLETE 2026-10-07** — https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.7.0,
  tag `v0.7.0` on **`29d8cab0`**: the FlexRadio VITA-49 rig type and panel,
  QO-100, profiles, the rig mode beside the frequency, FT2 level with MSHV
  (demapper, AP7 and its two fixes), 70cm / 23cm buttons, TCI-reconnect and
  JTTY false-decode crash fixes. Manoj: "looks good for a release", then
  "Publish v0.7.0 now" with Flex transmit marked as tested on a simulated
  radio only (the notes say so and advise low power).
  - **Re-cut once, with Manoj's OK:** the tag first went on `658f0902`, whose
    `lib/ft2/get_ft2_bitmetrics.f90` had two lines over Fortran's 132-column
    limit. GCC 16 (the Mac) accepts them; GCC 12 (Pi) rejects them, and
    MSYS2's would have too. Fixed in `29d8cab0` (same statements, one per
    line; AP7 bench identical), the two CI runs on the old commit cancelled
    before uploading anything, the tag force-moved. **Gotcha: the Mac's
    compiler will not catch this - check `awk 'length > 132' lib/**/*.f90`
    before tagging.**
  - CI run 37558235427 (macOS) and 37558235504 (Windows), green. All eight
    files downloaded anonymously (200), every checksum OK, arm64 app reports
    0.7.0 and its signature verifies; copies in `~/Desktop/jdxvu/v0.7.0/` (removed 2026-10-07).
  - Installed on the Mac mini 2026-10-07 ~18:30 from the release zip (sha256 OK, `ditto -x -k`,
    reports 0.7.0, executable identical to the release). It replaced a local build from 2026-10-06
    23:01 (about `f77d80d9`, still labelled 0.6.0), now in the Trash as
    "JTDX-VU 0.6.0-dev (replaced 2026-10-07 by v0.7.0).app". Settings untouched. Ad-hoc signed, not
    notarised (spctl "rejected"), same as every earlier install.
  - Pi `.deb` on meridianpi5 (prefix /usr/local, rsync of `git ls-files`,
    `nice make -j3`, `cpack -G DEB`): Version 0.7.0, Conflicts: jtdx,
    `apt-get install -s` clean, uploaded by hand with its `.sha256`.
  - Website: paragraph "FlexRadio without SmartSDR, QO-100, profiles and the
    rig mode at a glance (v0.7.0)", table / release link / apt command on
    v0.7.0, "Tested so far" and card updated; pushed (`e477301`) after all
    eight URLs answered 200.
  - Reply to Frank PH2M (`~/projects/JTDX-records/jtdx-vu-v0.7.0-reply-to-PH2M.txt`)
    sent by Manoj 2026-10-07.
- **Installed app:** `/Applications/JTDX-VU.app` is the **v0.7.3 release**
  (arm64 zip from GitHub, sha256 OK, `ditto`), installed 2026-10-09 05:46:
  reports 0.7.3, `codesign --verify --deep --strict` OK, executable and whole
  bundle identical to the release. JTDX-VU was not running, so it was not
  relaunched. The previous copy (local build of `6fcbe714`, labelled 0.7.2)
  is in the Trash as `JTDX-VU.app-prev-20261009-054607`. The release zip
  runs on macOS 14+; local builds run on macOS 26+ only. Settings are in
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
  an anonymous ranged GET). Copies in `~/Desktop/jdxvu/v0.3.0/` (removed 2026-10-07).
  `/Applications/JTDX-VU.app` is **v0.4.0** (local build of tag `v0.4.0`;
  title "JTDX-VU for VUCG V0.4.0"), installed 2026-10-01 20:36. Macro
  sets migrated (Default/Ragchew "599", Contest "599 %N", active
  Ragchew / DX). The previous v0.3.0 bundle is
  in `~/Desktop/jdxvu/prev-install/` (removed 2026-10-07).
- **Release v0.2.1: COMPLETE 2026-09-28**. It's at
  https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.2.1 with all four
  builds plus a `.sha256` for each:
  - The tag CI attached the Windows, macOS arm64 and macOS Intel zips.
    The Intel build took about 2.5 h and the attach step now works on
    Windows too.
  - The Pi `.deb` was built on meridianpi5 and uploaded by hand.
  - Copies are in `~/Desktop/jdxvu/v0.2.1/` (removed 2026-10-07).
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
  result ad-hoc: `build/bundle/JTDX-VU.app`.
- **Releases are Developer ID signed + notarised (from 2026-10-14):** after the
  tag CI attaches the ad-hoc zips, download the two macOS zips, run
  `./macos-notarize.sh JTDX-VU-<ver>-macos-arm64.zip JTDX-VU-<ver>-macos-x86_64.zip`
  (identity "Developer ID Application: Manoj Ramawarrier (CHVNJ85C9F)", notary
  keychain profile `MSHV-NOTARY` shared with MSHV-Mac), then
  `gh release upload <tag> JTDX-VU-*-macos-*.zip* --clobber`. Check with
  `spctl -a -vv -t exec` → "Notarized Developer ID".

## What changed

### 2026-11-04 — Double-clicked DX no longer dropped to CQ when he works others (for the next build)

Manoj, calling 6W/IK2HKT on 10m (TCI FLEX HF, 12:04-12:06 UTC): every over
started `6W/IK2HKT VU2CPL -17` and within a second switched to `CQ VU2CPL MK83`
(ALL.TXT "Retransmitting"). Cause: stock JTDX `process_Auto` drops the DX when
Settings > Sequencing "answer CQ" / "answer in call" counters are on (on in all
his profiles, 3 tries) and the tries are used up **or** the DX sent a message
to someone else (`m_reply_other`); with nobody else to answer it falls through
to `on_txb6_clicked()` = CQ. The 2026-10-20 KK4CDK fix exempted double-click
picks only from the `readFromStdout` halt, not from this drop. Now
`process_Auto` skips both drops when the DX is `m_dblClickCall` (base call), so
a picked DX is called until the Tx watchdog, as in WSJT-X/MSHV; AutoSeq's own
picks keep the counters. Builds; not tried on air.

### 2026-10-22 — 0.7.6-rc7: Flex key-up miss no longer QSYs to 14.100 and calls CQ there; a dead session no longer holds the radio keyed

On air with the FLEX-6600 on 30m, rc5 and rc6 jumped to **14.100** three times
(2026-10-21 18:42, 21:54 and 22:05 UTC) and, with Tx on, sent CQ there (the
radio reported TRANSMITTING; 14.100 is the NCDXF beacon frequency). The
`flex_trace.txt` showed the chain each time: `xmit 1` not answered within 1 s ->
`failed to key (xmit)` -> rig failure -> TCI-style reconnect 5 s later -> the new
GUI client is handed a fresh slice at 14.100 USB -> JTDX-VU followed it.
The restore in `handle_transceiver_update` ("start on the last frequency", only
on an offline -> online transition) never ran: `Configuration::handle_transceiver_failure`
closes the rig without sending an offline update, so the main window still had
the rig online, and the new FlexTransceiver has no `last_set_frequency_`.
A normal launch was fine (`applying after start`).

Fixes:
- `FlexTransceiver::do_ptt`: wait 3 s (was 1 s) for the `xmit 1` reply; a miss
  sends `xmit 0`, stops the DAX TX audio, reports PTT off and leaves the link up.
  Only three misses in a row (`key_failures_`) set the error and reconnect.
- `MainWindow::guiUpdate`: a Flex key-up still not confirmed halfway through the
  period releases PTT (`g_iptt=0`) with Tx left enabled, so the next cycle keys
  again; status bar "FlexRadio did not key - trying again next cycle".
- `MainWindow::handle_transceiver_failure`: for FlexRadio VITA, mark `m_rigState`
  offline, so the reconnect goes through the same restore as a launch.
- `tools/fake_flex.py`: `FAKE_XMIT_MISS=N` leaves the first N key-ups unanswered;
  `tools/flex_keymiss_test.sh` (throwaway `-r flexkm`, port 4994, `FAKE_GUI_SLICE`).

Same evening, a second rc7 change: **the radio stuck in TX.** Manoj moved the Mac
mini's cable (switch port 8 → 4, see vlan-setup HANDOVER) in the middle of an over;
its address changed .1.222 → .10.226, the `xmit 0` went out on the dead session, and
the FLEX-6600 stayed TRANSMITTING (owned by the dead client 0x034D99CE) ~53 s, through
JTDX-VU's reconnect, until Manoj restarted the radio. Fixes:
- `FlexTransceiver::drop_stale_session` (in `start_session`, after `sub client all`,
  before `client gui`): the previous handle and host are remembered across sessions;
  if the radio still lists that handle as a connected `program=JTDX-VU` client,
  `client disconnect 0x<handle>` closes it - which unkeys the radio and frees its
  GUI place and slice. Only our own previous session is ever closed.
- `do_ptt (false)`: an `xmit 0` not confirmed within 2 s sets "Flex: unkey not
  confirmed" and reconnects at once (was: ignored, then the 10 s audio watchdog).
- `tools/fake_flex.py`: `FAKE_FREEZE_KEYED=1` turns the first client that keys into
  a dead peer (nothing in or out, its close unseen, held keyed until `client
  disconnect` or 120 s); `client disconnect` is implemented.
  `tools/flex_stale_tx_test.sh` (throwaway `-r flexst`, port 4995).
  Result: keyed 22:36:15, unkey lost 22:36:28.7, reconnect + old session closed,
  "TX released" 22:36:36.8 (8 s; 30 s with the watchdog only), back on 21.074.
  Not tried on the real radio: `client disconnect <handle>` is SmartSDR API as
  FlexLib uses it, but this firmware's reply is unchecked.

Tested on the fake radio (bundle build of this change): MISS=1 -> one over skipped,
no reconnect, the next three overs keyed on 21.074; MISS=3 -> three skipped, one
reconnect, the new slice tuned straight back to 21.074, no Tx on 14.100. After a
real failure (three misses) Tx is halted as for every rig error (Enable Tx off),
unchanged. Not yet tried on the real radio. Why the 6600 sometimes takes >1 s to
answer `xmit 1` is not known.

### 2026-10-22 — 0.7.6-rc6: rc5 plus the Flex warning fix, short Flex status, Background highlight everywhere, 160m crossband with India (test build)

Manoj: "make rc6". `Versions.cmake` 0.7.6-rc5 -> rc6. rc6 = rc5 + `2f98b9f0`
(Flex warning from the stream's tx= flag; rc4/rc5 warned falsely on a working
radio) + `a4204703`/`2f7c32a3` (Flex panel: old layout, short status texts) +
`fc4c49bb` (Background highlight off covers Tx lines, QSY lines, the marker
cell) + `7f49d3d1` (Misc > Crossband 160m operation with India). No Fortran
changed. Local macOS build clean. GitHub pre-release `v0.7.6-rc6` (tag CI
green, 6 files; all three zips in `~/Desktop/jdxvu/test` for the MacBook).
- **Installed on the Mac mini** 2026-10-22 ~02:03 IST: arm64 zip sha256 OK,
  reports 0.7.6-rc6, codesign OK, binary identical; rc5 app in the Trash. The
  running copy was the FLEX VITA49 profile (plain launch, `Profiles/Last`); it
  was quit after the over ended (`xmit 0` in `flex_trace.txt`) and relaunched
  the same way with `open --stderr ~/Desktop/jtdx-stderr.log`. New Flex
  session: own dax_tx stream `tx=1`, no "no TX audio" warning (the rc4/rc5
  false alarm is gone on this radio).

### 2026-10-22 — 160m crossband with India (for the next build)

Manoj: "add a crossband op option with India. Rx 1808/Tx 1840, if op call is
VU then inverted, only for 160m". Stock JTDX already has Misc > Crossband
160m operation with Japan (1908/1840, 68 kHz) and Korea (1810/1840, 30 kHz)
in `MainWindow` (~7843, where Fake It / Rig split sets `m_freqTxNominal`).
Added `actionCrossband_160m_VU` ("...with India", tooltip), setting
`[Common] Crossband160mVU` (default off), `m_crossbandVUOptionEnabled`:
base 1808000 and my cty prefix not VU* -> Tx +32000 (1840); base 1840000 and
prefix VU* (VU, VU4, VU7) -> Tx -32000 (1808). `FrequencyList.cpp` default
list gains 1808000 FT8 (ALL regions); existing profiles keep their list.
- **Tested:** `tools/crossband_vu_test.sh` (fake TCI, Fake It, push the dial,
  one CQ): VU2CPL on 1.840 sent `vfo:0,0,1808000` for the over and back to
  1840000; K1ABC on 1.808 sent 1840000 and back to 1808000.

### 2026-10-22 — Background highlight off now means no background anywhere (for the next build)

Manoj: "how to remove background highlight for my call? ... or for anything
in this messages/markers?" (Background highlight already off). That switch
(`highlightBackground_`, 2026-10-04) was applied only in
`displayDecodedText`; three things still painted a background: our own Tx
lines (`displayTransmittedText`, bg = My Tx colour), QSY lines (pink), and
the marker cell before the country (`appendText`'s `servbg`: red for my call
std_type 2, blue std_type 5, yellow "?", the "new" colour on std_type 3).
Now `appendText` (every line goes through it), with the switch off, moves any
non-plain line background to the text colour and the marker cell's colour to
the marker character, lifted to lightness 150 in dark style as the decode
lines are. Decode lines arrive already plain, so they are unchanged. Default
on, so nothing changes for anyone with it on. Builds; not checked on screen.

### 2026-10-22 — Flex "refused DAX TX" warning was a false alarm on a working radio; now from the stream's tx= flag (for the next build)

Manoj (FLEX-6600, one copy, Slice A, rc5): the Flex panel said "radio refused
DAX TX: no transmit audio" while the radio was transmitting normally. His
`flex_trace.txt` (FLEX VITA49 profile, 2026-10-21 18:44): `dax audio set 1
tx=1` at every key-up answers `R|50001000|` on his working radio too, so the
rc4 check (`on_reply_` on that command, `9b31adfb`) was wrong - and so was the
2026-10-19 reading of KK4CDK's trace that blamed that reply. The real
difference: our own dax_tx stream status reads **`tx=1`** on Manoj's radio and
stayed **`tx=0`** on Miguel's joined copy 3. Now `parse_status` sets
`tx_refused_` from `\btx=0\b` on our own (not borrowed) TX stream once
`ready_`, traces the change, and republishes (panel "radio takes no DAX TX
from this copy: no transmit audio", the same status-bar warning). The key-up
`on_reply_` is gone. `tools/fake_flex.py` reports tx=1, so its self-tests stay
quiet. Builds; checked against both real traces by reading, not run on a
radio.
- **Panel width:** that long status line widened the Flex panel (Manoj's
  screenshot). A word-wrap version (`a4204703`) made the panel narrow and
  cramped; Manoj: "horrible UI". Reverted to the old panel layout, and the
  status texts kept short instead: ", no TX audio" (no TX stream, or ours
  tx=0) and ", transmit (station's)" (a borrowed stream). Scratch harness:
  one line, 206 px wide (188 px with ", transmit").

### 2026-10-20 — 0.7.6-rc5: rc4 plus double-click keeps calling a busy station (test build)

Manoj: "build the changes". `Versions.cmake` 0.7.6-rc4 -> rc5. rc5 = rc4 +
`333627d4` (double-click keeps calling, entry below) + `f0e6a69c` (README
wording). No Fortran changed. Local macOS build clean. GitHub pre-release
`v0.7.6-rc5` (tag CI green, 6 files). Miguel's reply (double-click keeps
calling, Queue CALL as the extra, rc5 link) is in
`~/projects/JTDX-records/jtdx-vu-reply-to-KK4CDK-2026-10-20.txt`, for Manoj
to send.
- **Installed on the Mac mini** 2026-10-20 01:31 IST from the arm64 zip
  (sha256 OK, `ditto`, reports 0.7.6-rc5, `codesign --verify --deep --strict`
  OK, binary identical to the zip; rc3 app in the Trash). Relaunched with
  `open --stderr ~/Desktop/jtdx-stderr.log ... -r "Aether QO-100"`. Note:
  `open --stderr` APPENDS, so the azdist error still in that file is the
  2026-10-19 rc3 one; a new one would appear after the latest font line.
  rc4 was never installed (superseded before the zip landed). All three rc5
  zips are in `~/Desktop/jdxvu/test` for the MacBook (Manoj remote).

### 2026-10-20 — Double-click keeps calling a busy station (for the next build)

KK4CDK: double-click a station still in a QSO, JTDX "holds the call but it
doesn't resume after his QSO is finished". The halt at `readFromStdout`
(`mainwindow.cpp` ~4533: DX sends a standard non-final message to someone
else, Enable Tx on, and `halttxreplyother` or our Tx within `m_nguardfreq`)
switched Enable Tx off and kept the DX call. Manoj: double-click must just
call the station, in QSO or not, as in WSJT-X/JTDX/MSHV; the right-click
queue is only an extra for waiting. (A first try that re-enabled Tx when he
finished was discarded on Manoj's word.) Now `doubleClickOnCall` stores
`m_dblClickCall = m_hisCall` after `processMessage`, and that halt skips a
DX call equal (base call) to it; AutoSeq's picks still halt as before.
Cleared on a logged QSO. It is the only halt of that kind. Builds; not
tried on air. Reply drafted:
`~/projects/JTDX-records/jtdx-vu-reply-to-KK4CDK-2026-10-20.txt`.

### 2026-10-19 — 0.7.6-rc4: rc3 plus the macOS exit-code-2 fix, the Flex DAX TX warning, TCI refused-frequency fix (test build)

Manoj: yes to rc4 with the warning. `Versions.cmake` 0.7.6-rc3 -> rc4. rc4 =
rc3 + `871a6379` (Fortran string lengths as size_t on macOS: the random
quits) + `9b31adfb` (Flex copies 3-4 warning, TCI refused frequency). No
Fortran source changed (the header and CMake did). Local macOS build clean.
GitHub pre-release `v0.7.6-rc4`.

### 2026-10-19 — Flex copies 3-4: warn when the radio refuses DAX TX; TCI: a refused frequency no longer drops the link (for the next build)

**KK4CDK's trace** (copy 3, rc1, FLEX-6700, `flex_trace.txt` 15:43:24 session):
joined GUI client 0x2b89b3f4 as non-GUI; `stream create type=dax_tx` was
**granted** (`R16|0|84000003`, our own handle 0x422EF785), so rc1's fallback
(runs only on refusal) never ran and no warning showed. The stream stays
`tx=0`; other clients' dax_tx streams are never reported to us even with
`sub dax all`; at key-up `dax audio set 4 tx=1` -> `R20|50001000` and
`xmit 1` -> 0, so the radio keys with no audio. Only a station's own GUI
client carries TX audio on his radio. Stock JTDX 2.2.160 does four copies
because it goes through SmartSDR + DAX (one station, DAX mixes the copies).
- `FlexTransceiver::do_ptt`: an `on_reply_` on that `dax audio set N tx=1`;
  a non-zero code sets `tx_refused_`, traces "!! radio refused DAX TX for this
  copy (0x...)" and republishes status: panel "radio refused DAX TX: no
  transmit audio", `FlexStatus::tx_missing` -> main window status bar "FlexRadio:
  the radio gives this copy no transmit audio (only the first two copies on a
  FLEX can transmit)". Cleared on close. Not tried on a real radio.
- Ideas left: copy 1 publishes its dax_tx id and copy 3 sends on it (unknown
  whether the radio accepts it), or copies 3-4 hand audio to copy 1/2 over
  localhost, DAX-style. Reply sent 2026-10-19
  (`~/projects/JTDX-records/jtdx-vu-reply-to-KK4CDK-2026-10-19b.txt`): use
  SmartSDR + DAX for all four copies; the coloured square before the country
  is stock JTDX's "new" category colour; LoTW Fetch Now is in rc3.

**TCI refused frequency** (Manoj, QO-100, Aether QO-100 profile): with Split =
Fake It, Aether dropped JTDX-VU 0.2 s into every over. Aether's log: JTDX-VU
sent `vfo:0,0,10489539757`, Aether answered `vfo:0,0,10489540000` (slice
`lock:0,true` - AetherSDR mirrors the IC-705's dial lock, CI-V 16 50, onto
the slice, `RadioModel.cpp` ~1204), and `TCITransceiver::do_frequency` set
"TCI failed set rxfreq", which closes and reconnects. Manoj unlocked it with
Aether's slice lock button and Fake It works. Now `vfo_answered_rx_` /
`vfo_answered_other_` are set by the server's vfo reply; a mismatch with an
answer (and no band change) is taken as the radio's frequency (qWarning, no
error); no answer at all is still an error.
- `tools/fake_tci.py` `FAKE_VFO_LOCK=1` (refuses vfo changes);
  `tools/tci_vfolock_test.sh` (Fake It, Tx 2500 Hz, CQ for HOLD s): fixed
  build 1 connect, 3 refusals, both overs complete (peak 0.999); rc3 the
  first over cut 1 s in and a reconnect.

### 2026-10-19 — macOS: JTDX-VU quitting by itself (exit code 2) fixed: Fortran string lengths passed as int (for the next build)

The silent quits on 2026-10-18 (18:10:51, 18:12:36 UTC) and 2026-10-19
~08:55 IST (after Manoj tried Fake It) were Fortran runtime errors. The
rc3 copy started with `open --stderr ~/Desktop/jtdx-stderr.log` caught it:
`At line 34 of file lib/azdist.f90: Substring out of bounds: upper bound (5)
of 'hisgrid' exceeds string length (3)`. Both C++ calls pass 8-character
grids, so the length itself was wrong.
- **Cause:** `wsjtx_config.h.in` picked the type of gfortran's hidden
  CHARACTER length arguments (`fortran_charlen_t`) with `#if __GNUC__ > 7`.
  The C++ compiler on macOS is clang, which reports `__GNUC__` = 4, so every
  macOS build declared them `int`, while gfortran 8+ reads `size_t`. On arm64
  `azdist_`'s 10th and 11th arguments go on the stack; Apple packs two 4-byte
  ints into one 8-byte slot, so Fortran read MyGrid's length right by luck and
  HisGrid's from whatever was next on the stack. Mac-only: the Windows (MinGW)
  and Linux builds use gcc and were already on `size_t`. Every macOS release
  had it; it struck only when the stack garbage was below 5.
- **Fix:** CMake sets `WSJT_FORTRAN_CHARLEN_SIZE_T` when the Fortran compiler
  is GNU >= 8; the header uses it (gcc's `__GNUC__` test kept for others, not
  for clang). The `size_t` branch included `<cstddef>` inside the header's
  `extern "C"` block, which libc++ rejects; it now includes `<stddef.h>`.
- **Proof:** scratch C++ -> Fortran call with 9 int pointers + 2 strings
  (azdist's shape): declared `int`, Fortran saw len(HisGrid)=1830709712;
  declared `size_t`, 8. App rebuilt clean. Not yet in a released build.
- Also: when jtdx dies this way its `jtdxjt9` decoder is left running (it was
  killed by hand before restarting).

### 2026-10-19 — 0.7.6-rc3: rc2 plus the LoTW users download (test build)

Manoj: "merge lotw into jtdx-vu and make rc3" (rc2's macOS CI was still
running; rc3 supersedes it). `claude/lotw-fetch-d0bae6` (`2a4691f1`, entry
below) merged cleanly into `jtdx-vu`; `Versions.cmake` 0.7.6-rc2 -> rc3. No
Fortran changed since rc1. Local macOS build clean. Checked with a scratch
CountryDat harness: the new bundled CSV (newest 2026-10-12) gives W1AW
2026-10-07 and UT7QF 2026-10-05; the old 2022 CSV gives nobody (Miguel's "no
dots"). Fetch Now itself not yet clicked in the app. GitHub pre-release
`v0.7.6-rc3` (tag CI green, 6 files; notes corrected: Flex copies 3-4 still
silent on a real radio).
- **Installed on the Mac mini** 2026-10-19 ~01:38 from the arm64 zip (sha256
  OK, `ditto`, reports 0.7.6-rc3, `codesign --verify --deep --strict` OK,
  binary identical to the zip). Ad-hoc signed, not notarised (rc builds aren't;
  only releases go through `macos-notarize.sh`). The 0.7.5 release app is in
  the Trash. Started with `open --stderr ~/Desktop/jtdx-stderr.log -a
  /Applications/JTDX-VU.app --args -r "Aether QO-100"` so a Fortran runtime
  error (the exit-code-2 quits of 2026-10-18, 18:10:51 and 18:12:36 UTC, both
  a few s after a queued station OZ1TDK sent CQ) is captured. rc1 zips in
  `~/Desktop/jdxvu/test` moved to the Trash.
- **KK4CDK, 2026-10-19:** rc1 still gives TX audio on only 2 of 4 copies on
  his FLEX-6700, and the LoTW dot never shows (only `*`, the hinted-decode
  mark). Reply sent asking for copy 3's `flex_trace.txt` and its Flex panel
  status line, plus the manual LoTW CSV workaround. **Waiting on Miguel's
  trace.** An rc3 follow-up draft is in
  `~/projects/JTDX-records/jtdx-vu-reply-to-KK4CDK-2026-10-19.txt` (not sent).

### 2026-10-19 — LoTW user marks: Settings > General "Logbook of the World User Validation" (branch `claude/lotw-fetch-d0bae6`, for the next rc)

Miguel KK4CDK (testing 0.7.6-rc1) never saw LoTW dots as in JTDX
2.2.160-rc10 (he saw `*`, the stock hinted-decode mark, unrelated).
- **Cause:** `LogBook::init` loads `:/lotw-user-activity.csv` (JTDX 2.2.159's
  copy, newest upload 2022-02-20) unless the data directory has its own, and
  `CountryDat::load` keeps only uploads within 365 days, so `getLOTW` was ""
  for everyone and `•` / `°` never appeared.
- **Fix:** as JTDX 2.2.160 (its source isn't published; upstream git stops at
  159), a "Logbook of the World User Validation" group at the bottom of
  Settings > General: "Users CSV file URL:" (default
  `https://lotw.arrl.org/lotw-user-activity.csv`) + **Fetch Now**, "Age of last
  upload less than:" N days (1-9999, default 365) and "lotw CSV Version:
  <date>" = newest upload date in the loaded file (`CountryDat::lotwVersion`).
  Wording and settings keys follow WSJT-X, which 2.2.160 copied
  (`[Configuration] LotWUsersURL`, `LotWDaysSinceLastUpload`). Fetch Now
  (`Configuration::impl::on_LotW_CSV_fetch_push_button_clicked`, own
  QNetworkAccessManager) checks the reply looks like `CALL,yyyy-MM-dd`, writes
  `lotw-user-activity.csv` in the data directory with `QSaveFile` (temp file +
  rename) and emits `Configuration::lotw_users_changed`; MainWindow then runs
  `init_logbook ()` (which now passes the age and sets the version label), so
  marks show on the next decodes without a restart. Changing the age and
  pressing OK reloads the same way. No automatic fetch.
- Bundled `lotw-user-activity.csv` refreshed from ARRL (2026-10-12, 236 500
  calls, 6.3 MB) so a fresh install has marks for a year.
- `CountryDat::load` no longer skips the file's last line (stock quirk).
- **Tested:** macOS build clean. Harnesses (scratch, not committed):
  CountryDat on the old bundled file at 365 days -> W1AW / VU2CPL "" (the bug);
  on the new file -> W1AW 2026-10-07, VU2CPL 2026-10-10, version 2026-10-12;
  K1JT's last upload is 2021-11-15, so "" at 365 days and a date at 3650. A Qt
  harness doing the same GET / check / QSaveFile as Fetch Now got 6 331 560
  bytes and the same dates; `https://lotw.arrl.org/` (HTML) is refused. The
  button itself was not clicked in the GUI (the computer-use tool could only
  reach the running Aether QO-100 copy, same bundle id). No release.

### 2026-10-19 — 0.7.6-rc2: rc1 plus the TCI reconnect fix, Ignore CALL, no repeat logging (test build)

Manoj: "merge and make rc2". `feat/ignore-call` merged into `jtdx-vu`
(`2d8c9331`; only conflict: both new HANDOVER entries at the top, both kept).
`Versions.cmake` 0.7.6-rc1 -> 0.7.6-rc2. rc2 = rc1 + the silent TCI TX audio
after reconnect fix (`74a808de`) + Ignore CALL / no repeat auto-logging
(entries below) + PSK Reporter "JTDX-VU v0.7.6" (`3deb9b25`). No Fortran
changed since rc1. Local macOS build clean; `autoseq_ignore_test` and
`qsoseq_test` pass on the merge. Published as GitHub pre-release
`v0.7.6-rc2` (CI attaches the zips).

### 2026-10-19 — TCI: TX audio silent after an automatic reconnect (for the next build)

Manoj lost transmit on QO-100 (Aether QO-100 profile, AetherSDR dev build ->
IC-705) on 2026-10-18. Aether's log (`aethersdr-20261018-232328.log`): an over
started 23:25:22, at 23:25:25 Aether dropped the client ("RX audio stopped:
backlog limit" after a 1.2 s main-thread stall, closeCode 1009), JTDX-VU
reconnected at 23:25:30, and every over until his restart at 23:41 had PTT
keyed but "TCI TX summary ... peak=0 rms=0".
- **Cause:** `Configuration::impl::close_rig()` disconnects the old rig's
  signals before it can say `tci_mod_active(false)`, so a drop mid-over left
  `MainWindow::m_tci_mod_active` true. `startTx2()` then thought the modulator
  was still running and skipped `transmit()` on every later over: PTT went
  on, the new `TCITransceiver`'s modulator stayed Idle and `readAudioData()`
  answered every TxChrono with zeros. A drop between overs was harmless, which
  is why the 2026-10-03 reconnect tests never saw it. FlexRadio VITA-49 uses
  the same flag and had the same exposure.
- **Fix:** `close_rig()` emits `transceiver_TCImodActive (false)` after the
  disconnect - a closed rig has no modulator running.
- **Test:** `tools/tci_reconnect_tx_test.sh` (new; throwaway `-r tcitxtest`,
  port 50095): CQ over UDP, drop the client 3 s into the first over, wait for
  the reconnect, CQ again (the failure halts Tx, as Manoj re-enabled it by
  hand), print each over's TX peak. `tools/fake_tci.py` now sends TxChrono
  while `trx` is on (1024 frames, `length` counts floats as AetherSDR), logs
  "TX over ... peak=" per over, unkeys a client that goes, and drops the
  client when `<log>.drop` exists. Installed v0.7.5: peak 0.9990 before the
  drop, **0.0000 on both overs after**; this build: **0.9990 on both**.
  (Two fake gotchas met on the way: a frame under 126 bytes must use the 7-bit
  length or Qt drops the connection; and only the first `length` floats of a
  TX frame are filled, the rest of JTDX-VU's buffer is uninitialised.)
- macOS bundle built; not yet tried on air. No release (Manoj decides).

### 2026-10-19 — Ignore a station; no repeat auto-logging (branch `feat/ignore-call`, for the next release)

Manoj on QO-100 FT4: UT7QF missed our RR73 and sent `R+14` about 20 times;
each RR73 JTDX-VU sent back logged the QSO again (3 entries, 18:29 / 18:31 /
18:34 UTC). "need an option to blacklist a caller". Stock JTDX has a hidden
one: Ctrl (Cmd on macOS) + right-click Clear DX calls
`QsoHistory::blacklist` for the current DX call, cleared by
`QsoHistory::init`, shown nowhere.
- **Ignore:** right-click a decode > "Ignore CALL (never answer)" /
  "Stop ignoring CALL" (`DisplayText::ignored_`, `ignoreToggled`).
  `MainWindow::m_ignored`, kept in `[JTDXVU] Ignored` per profile, shown as
  "Ignored (never answered)" under the queue list in `m_queueBox`
  (right-click: Stop ignoring / Clear ignored). `QsoHistory::setIgnored`
  fills `_ignored` (base calls); `black(key)` = `_blackdata` + ignored and
  replaces the three `on_black` lookups in `autoseq`, so an ignored station is
  never autoselected. Ignoring the current DX call halts Tx and clears DX;
  ignoring takes the call off the queue.
- **No repeat logging:** `on_logQSOButton_clicked` skips an automatic log
  (`m_logqso73` + autolog) when base call | band | Tx mode equal
  `m_lastLoggedKey` (set in `acceptQSO2`) within 600 s of
  `m_lastloggedtime`; status message "CALL already logged: not logged
  again". The Log QSO button and JTTY are unchanged.
- **Tested:** `tools/autoseq_ignore_test.cpp` (4 checks; 3 fail on the
  unmodified `qsohistory.cpp`, all pass now); `tools/qsoseq_test.cpp` still
  passes; the app builds (worktree `~/projects/JTDX-ignore`). Not tried on
  air or in the GUI yet. Built in a worktree because the TCI reconnect
  session was editing the main checkout.

### 2026-10-18 — PSK Reporter: one program name per version (for the next build)

Manoj saw PSK Reporter's software list split JTDX-VU into ~40 rows: the
string was `applicationName` (which carries the `--rig-name`, e.g.
"JTDX-VU - Flexradio") + version + " tci " + git revision, stock JTDX code.
PSK Reporter folds known programs (JTDX, WSJT-X) but lists unknown strings
verbatim. Manoj: "make it JTDX-VU v0.7.6". `setLocalStation` in
`mainwindow.cpp` now sends `"JTDX-VU v" + applicationVersion ()` cut at the
first '-' (so 0.7.6-rc1 reports as v0.7.6). Not in v0.7.6-rc1 (tagged
before). A single "JTDX-VU" row would need PSK Reporter's operator to group
it. Builds; Help > About and the title bar still show rc and revision.

### 2026-10-18 — v0.7.6-rc1 published (pre-release); Windows CI fixed for Hamlib

Manoj: "fix and rebuild rc1", then "tag it when green". The cloud session's
branch `claude/beautiful-goldberg-4kaz34` (Flex station TX stream + the
0.7.6-rc1 bump, entries below) was fast-forwarded into `jtdx-vu`; macOS
local build clean, no Fortran changed since v0.7.5.
- **Windows CI broke in Hamlib 4.7.2** (pinned, so MSYS2's newer GCC):
  `precise_time.c` calls `assert()` without `<assert.h>`, now an implicit
  declaration error. Fix: `CFLAGS="-g -O2 -include assert.h"` on Hamlib's
  configure (`46749b04`). Not `CPPFLAGS`: windres gets those and rejects
  `-include` (`version_dll.rc`, run 39086335536). Check run 39087117003 green.
- Tag `v0.7.6-rc1` (lightweight, as the rc tags) on `46749b04`, GitHub
  **pre-release** created first so CI attaches to it. Tag CI: Windows
  39088188850, macOS 39088188872.
- Reply to Miguel (Flex copies 3-4 + Wanted Call beep, rc1 link):
  `~/projects/JTDX-records/jtdx-vu-reply-to-KK4CDK-2026-10-18.txt`, for
  Manoj to send. Flex change untested on a real radio; **waiting on Miguel**.

### 2026-10-18 — 0.7.6-rc1 (Windows + macOS) for KK4CDK: the four-copy TX audio test

Manoj: "start the Windows build as an rc", then "build a rc1 for mac also".
`Versions.cmake` 0.7.5 -> 0.7.6-rc1 on branch `claude/beautiful-goldberg-4kaz34`
(= `jtdx-vu` at `55935b0` + the Flex station TX stream change below), `aacca6a`.
Started by hand on that branch (workflow_dispatch: zips are run artifacts only,
no tag, no release): Windows run 39084731852, macOS run 39084733509. The earlier
macOS run 39084471352 (same code, still labelled 0.7.5) was cancelled. The Mac
zips from a branch run are ad-hoc signed, not notarised (`macos-notarize.sh` is
a Mac step). Not yet
published for Miguel: the rc3 way was a GitHub pre-release tag, which Manoj has
to OK. Reply asking him to test drafted (outside the repo, to go in
`~/projects/JTDX-records/jtdx-vu-reply-to-KK4CDK-2026-10-18.txt`), with a
placeholder for the download link.

### 2026-10-18 — Flex copies 3 and 4 send on the station's DAX TX stream; "no DAX TX stream" warning (UNTESTED on a real radio, no release)

Follow-up to KK4CDK's four-copy report (2026-10-17 entry, and his 2026-10-18 note
that four stock JTDX rc10 copies all send audio through the one shared DAX TX
sound device). Reverses the 2026-10-17 "Flex limitation, no change" call; Manoj
decides whether it ships.
- `FlexTransceiver.cpp`: every `stream 0x... type=dax_tx client_handle=...` and
  `stream 0x... removed` status is kept (`dax_tx_streams_`), and every client's
  `station=` (`client_station_`); "sub dax all" added at start-up. When a copy
  that joined a station (`bound_handle_`) gets no dax_tx of its own, it waits up
  to 1.5 s for `station_tx_stream ()`: the bound GUI client's dax_tx stream, else
  one of a client with the same named station (SmartSDR's DAX program), **never
  another station's** (that would put our audio on the other transmitter). It
  then sends `transmit set dax=1` / `dax audio set N slice=S tx=1` as before
  (`use_tx_stream ()`) and its VITA-49 TX packets carry that stream id.
  `tx_borrowed_`: on quit it does NOT `stream remove` the station's stream or send
  `transmit set dax=0`. If the station's stream is removed, it looks for another,
  else drops to no TX; a station stream that appears later is picked up. Trace
  lines: "own dax_tx refused - sending on the station's DAX TX stream 0x...",
  "!! no DAX TX stream: PTT will key with no audio".
- Warning: `FlexStatus::tx_missing` (DAX audio on, no TX stream); the main window
  shows "FlexRadio: no DAX TX stream for this copy - PTT keys the radio with no
  audio" (30 s) when it is found and again at every key-up. The Flex panel's
  status line says "transmit on the station's DAX TX" / "no DAX TX stream: no
  transmit audio".
- `tools/fake_flex.py`: `FAKE_STATION_TX=1` = one dax_tx stream per station (a
  second create from the same station is refused, code `50000016` invented),
  dax_tx streams reported to every client with "removed" when they go, TX packets
  on an unknown stream id dropped; every transmission is logged with its stream.
  `tools/flex_station_tx_test.sh`: four copies (flexa/flexb GUI clients, flexc/flexd
  join flexa's station), each in turn calls CQ over UDP; macOS or Linux (Xvfb).
- **Checked (Linux build in a cloud session, Ubuntu 24.04 / Qt 5, not macOS: no
  Mac there):** old code against `FAKE_STATION_TX=1`: flexc and flexd key PTT and
  the fake records nothing - Miguel's fault reproduced. New code: all four
  transmit; flexa on its stream 0x84003C2D, flexb on 0x84003C2E, flexc and flexd on
  flexa's 0x84003C2D (full 13.7 s FT8 at ~1500 Hz, RMS ~21800). `NODAX_A=1` (flexa
  without DAX, so flexc makes the station's stream under its own handle): flexd
  finds no stream of flexa's and shows the warning (screenshot seen). A jtdxjt9
  "Subprocess failed with exit code 2" box appeared in that container with four
  copies; not looked into, unrelated to Flex.
- **Unknown until tried on a real radio** (the fake's behaviour here is a guess):
  whether a station's second `stream create type=dax_tx` is refused or granted but
  dead (granted = our own id, no fallback, still silent); whether the radio
  reports other clients' dax_tx streams (with "sub dax all") and under which
  client_handle; and whether it takes TX packets on another client's stream id
  from our UDP port. If Miguel tries it, his `flex_trace.txt` for copy 3 shows
  which. Two copies keyed at once on one station share the stream (audio mixes),
  as stock JTDX copies do through the one sound device.
- macOS build: no Mac in this session; checked by the 0.7.6-rc1 CI run above. No release.

### 2026-10-18 — KK4CDK: Wanted Call beep silent; New only now keeps Wanted Calls (for the next release)

Miguel: the Wanted list's sound alarm doesn't work, and he found no setting
for it. There is none in stock JTDX either: `displayDecodedText` beeps
(`QApplication::beep()`, the OS alert sound, once per decode period) for
every Wanted **Call** match; Wanted Prefix / Grid / Country beep only when
the station is also "new" by Notifications (priority >= 5; the lower-priority
beeps are commented out upstream). The JTDX-VU regression: the "New only"
filter (`vu_hidden`) hid a worked-before Wanted Call, and a hidden line never
beeps. Stock shows it (upstream `c3c4a4d0` "Don't hide B4 call when in wanted
list"). `meta.alwaysShow` now includes `bwantedCall`. README New only item
says so. Builds; not yet tried on air. Still open from the same mail: he says
four stock JTDX rc10 copies on his Flex all send audio (stock uses the one
DAX TX sound device, which every copy shares), against our 2026-10-17
"Flex limitation" call - the idea of reusing the station's existing dax_tx
stream when our create fails now looks like the real fix.

### 2026-10-17 — KK4CDK: four Flex copies, only two get TX audio (Flex limitation, no change)

Miguel ran four JTDX-VU copies on his FLEX-6700: all decode, any one
transmits alone, two transmit independently, but the third and fourth to
connect key PTT with no TX audio. Cause: the radio takes two GUI clients
(stations), and DAX TX is one stream per station. Copies 3 and 4 join a
station (the 2026-10-13 join) and their `stream create type=dax_tx` gets no
usable stream, which `tx_audio()` skips without a warning. `tools/fake_flex.py`
gives every client its own TX stream, so the fake didn't show it. **Manoj:
treat it as a Flex limitation, no change for now.** Ideas left for later:
send on the station's existing dax_tx stream id when our own create fails
(untested on a real radio), or at least a status-bar warning. Reply (use
copies 1-2 for Tx, 3-4 for receive; plus re-answers on RR73/73, Halt and
queue and the queue, pointing at v0.7.5) drafted in
`~/projects/JTDX-records/jtdx-vu-reply-to-KK4CDK-2026-10-17.txt`; Manoj trimmed point 1 (no technical detail) and sent it 2026-10-17.

### 2026-10-17 — v0.7.5 released: Intel crash fix, notarised Mac apps, late Tx cutoff, queue + Halt and queue, four Flex copies

- https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.7.5, annotated tag on
  `eb43fff1` ("Version 0.7.5": `Versions.cmake` 0.7.5-rc6 -> 0.7.5; README
  title, title-bar example, first-launch text (notarised from v0.7.5), and
  every "since the next release" -> "since v0.7.5"). Cut on Manoj's "release
  0.7.5". No Fortran changed since v0.7.4. Contents = rc1-rc6 (68 commits
  since v0.7.4); notes from the v0.7.4 template with the macOS first-launch
  step rewritten for notarised apps.
- **Created as a draft** so v0.7.4 users' update check didn't see a release
  with no files; CI's `gh release view` found the draft and attached to it
  (no duplicate). Published (`--latest`) only after all eight files were in.
  CI: Windows 38982097543 and macOS 38982097544, both green (the MSYS2
  libusb `pacman -U` fix from rc6 held).
- Pi .deb on meridianpi5 (rsync of `git ls-files` at `eb43fff1`, `cmake ..`,
  `nice make -j3`, `cpack -G DEB`): jtdx-vu 0.7.5 arm64, Conflicts: jtdx,
  `apt-get install -s` clean, 8 s offscreen `--test-mode` start OK (no
  jtdxjt9 left; its 13.7 MB shm segment, `~/.qttest`, `/tmp/JTDX-VU - test*`
  removed), uploaded by hand with its `.sha256`.
- Mac zips notarised (arm64 59a47317-2356-4c9e-951d-06a353874f5f, x86_64
  959bef4e-24bb-44d5-9714-6eaf90306b0b, Accepted, stapled) and replaced on
  the draft. After publishing, all eight files downloaded anonymously,
  checksums OK; both Mac apps "Notarized Developer ID", 0.7.5, right arch, no
  AppleDouble entries; Windows `jtdx.exe` 0.7.5 with libusb + Hamlib DLLs;
  `releases/latest` = v0.7.5. sha256:
  - `JTDX-VU-0.7.5-macos-arm64.zip` `16562d8836cea5470c99f9c18d27a8a4434b8323b4de9635e3ec33817636ed62`
  - `JTDX-VU-0.7.5-macos-x86_64.zip` `bb12e766679f27fa4bda0c08abd36b4702026da07c7b314dcc9b2356108db101`
  - `JTDX-VU-0.7.5-windows-x64.zip` `a322e9a5d4d0448bf272cc6e2c47e6311a93204c69a2380e80cdb87d5a8ed6d9`
  - `jtdx-vu-0.7.5-linux-arm64.deb` `775035f7aedb69df71d243895ea80b8a1b43f9ea4912e7b82f9631249ed5379d`
- Issues #1, #4, #5, #6 commented ("Fixed in v0.7.5 ... reopen if it comes
  back") and closed on Manoj's "post and close them". No open issues left.
- Website (vu2cpl.github.io `75f845c`): card sentence, page paragraph
  "Intel crash fixed, notarised Mac apps, a queue that calls by itself and
  four copies on one Flex (v0.7.5)", Queue bullet, downloads / release link /
  apt on v0.7.5, Installing says notarised, Tested-so-far adds KK4CDK's four
  slices. All URLs answered 206 first.
- **Installed on the Mac mini** on Manoj's "quit it, install now": JTDX-VU
  (FLEX VITA49 profile) quit cleanly via AppleScript, the 0.7.5-rc4 app went
  to the Trash, and the arm64 release app went in with `ditto`. It reports
  0.7.5, shows as "Notarized Developer ID", `codesign --verify --deep --strict`
  is OK, the binary is identical to the release, and no shared memory was left.
- `~/Desktop/jdxvu/` (rc6 test zips) moved to the Trash on Manoj's word.
- v0.7.4 users get the in-app update notice for this release.

### 2026-10-16 — 0.7.5-rc6: rc5 plus the queue picks and Halt and queue (test build)

Manoj: "make rc6". `Versions.cmake` 0.7.5-rc6 (`d67a3352`), published as GitHub
**pre-release `v0.7.5-rc6`**. rc6 = rc5 + a queued pick switching Enable Tx on
+ right-click Halt Tx > Halt and queue CALL (both KK4CDK). No Fortran changed
since rc5, so no 132-column check.
- **Windows CI broke on MSYS2:** `error: target not found:
  mingw-w64-x86_64-libusb`, twice (so not a mirror hiccup). MSYS2 has dropped
  libusb from the mingw64 package database (`mingw64.db` lists only
  libusb-win32 / libusbmuxd; ucrt64 still has `ucrt-x86_64-libusb-1.0.30`).
  The signed file `mingw-w64-x86_64-libusb-1.0.30-1-any.pkg.tar.zst` is still
  on repo.msys2.org, so `windows.yml` now installs it with `pacman -U`
  (`ac7804a9`). The tag was moved to `ac7804a9` (`git tag -f` + force push), and
  the macOS run on the old tag was cancelled. Tag CI: Windows 38842511462 (20 min)
  and macOS 38842511318, both green. **Known gotcha / open item:** once MSYS2
  deletes that file the Windows build breaks again. The lasting fix is moving
  the workflow to UCRT64 (`msystem: UCRT64`, `mingw-w64-ucrt-x86_64-*`
  packages, `/ucrt64` prefix for Hamlib).
- Windows zip: anonymous download sha256 OK, `jtdx.exe` says 0.7.5-rc6,
  `libusb-1.0.dll` and `libhamlib-4.dll` bundled. It replaced the rc5 Windows
  zip in `~/Desktop/jdxvu/test/`.
- **macOS zips notarised** after a delay. The first `macos-notarize.sh` run
  failed with "No Keychain password item found for profile: MSHV-NOTARY": the
  credential had gone from the login keychain (it worked for rc5 on
  2026-10-15). Manoj re-ran `notarytool store-credentials MSHV-NOTARY`, and
  both zips were then Accepted (arm64 e0e2eef4-a419-4177-9bb0-c50dbbf18abb,
  x86_64 c0b50933-d5fd-4294-afa6-74988fa169c3), stapled and replaced on the
  release (`--clobber`). Anonymous downloads: checksums OK, `spctl`
  "Notarized Developer ID", ticket validates, 0.7.5-rc6, right arch. All three
  rc6 zips are now in `~/Desktop/jdxvu/test/`, and the rc5 ones were trashed.
- `gh` in this checkout defaults to the **upstream** repo
  (jtdx-project/jtdx), so pass `--repo vu2cpl/jtdx-vu` on every
  `gh release` / `gh run` call.
- Reply posted on #6 (2026-10-16): rc6 keeps the #6 fix, adds Halt and queue
  and the queue switching Enable Tx on. marinfyi builds from source
  ("will git pull and recompile"); no on-air report yet.
- The reply to Miguel (rc6 Windows link) is in
  `~/projects/JTDX-records/jtdx-vu-reply-to-KK4CDK-2026-10-16.txt`. Manoj sent
  it on 2026-10-16. **Waiting on Miguel** to try Halt and queue and the queue
  on his Flex.

### 2026-10-16 — Right-click Halt Tx: "Halt and queue CALL" (for the next release)

Manoj: "add halt and queue to the right click". It covers most of Miguel's
"wait till available" request (declined 2026-10-15) with the queue, and has
no auto-after-N-tries. `queueSetupUi` gives `stopTxButton` a custom context
menu with one item, "Halt and queue CALL" (disabled with no DX call, or in
JTTY). It calls `haltTx("halted, CALL queued")`, then `clearDX`, then queues
the call unless it's already in the queue. The queue entry above (Enable Tx
on a queued pick) then calls the station when it sends CQ, or RR73/73 with
the marker on. README queue item updated.
- **Tested 2026-10-16** (bundle, throwaway `-r qtest` as above, with
  `73RR73Marker=true` and the debug ALL.TXT on). `fake_tci.py` cycled
  `F4LQJ OK1JRD RR73` plus 3 quiet periods (RR73 at :00 of every minute), and
  this time the menu was right-clicked for real (full-screen computer use).
  (1) DX call typed, Enable Tx off, Halt and queue: DX call cleared, queue
  OK1JRD. The 02:41:00 RR73 was picked (RFIN, `queued=1 enableTx=0`), the Tx
  period went 00/30 -> 15/45, and `OK1JRD VU2CPL -08` went out at 02:41:15.07.
  (2) The realistic case: while calling OK1JRD, Halt and queue during Tx at
  02:41:54 ("Halt Tx triggered at TX: halted, OK1JRD queued", "DX Call OK1JRD
  cleared"). The 02:42:00 RR73 was picked and Tx started at 02:42:15.07.
  An earlier run (before the debug log was on) picked OK1JRD but didn't key.
  In that run the Tx period and Enable Tx had been set through accessibility
  presses (`AXPress`), which may not emit Qt's `clicked()`, so the cause
  isn't certain; the two clean runs above both worked. Not tried on air.

### 2026-10-16 — A queued pick switches Enable Tx on (for the next release)

Manoj: "let queued pick turn on enable tx", after Miguel (KK4CDK) reported
that the queue never called anyone. The queue only re-ranks AutoSeq's
autoselect, and autoselect never keyed up by itself. In `process_Auto`, when
the station picked is `queued()` and Enable Tx is off (not JTTY), it now
sets the Tx period opposite the decode first and then clicks Enable Tx, with
a 15 s status message "CALL (queued) heard: Enable Tx switched on". The Tx
period matters because with Tx off both periods are received. The period
comes from the new `QsoHistory::lastTime()` (= `max_r_time`, the time of the
decode the pick came from): `m_txFirst = fmod(t, 2*TR) != 0`, the same rule
as `processMessage`. With Enable Tx off, `on_TxMinuteButton_clicked` doesn't
clear the DX call. The tooltip on the queue list and the README queue item
say so. Unchanged: picks still need the DX Call box empty, a call mode other
than None and AutoSeq on, so a station the user was calling when Tx halted
(DX Call still set) isn't picked up from the queue.
- **Tested 2026-10-16** on the bundle with a throwaway `-r qtest` copy of
  the TCI FLEX HF profile (`CATTCIPort` and UDP on localhost, PSK Reporter
  and SWL off, TxFirst=true). `tools/fake_tci.py` cycled `FAKE_TCI_WAVS`
  `CQ OK1JRD JN79` (ft8sim, -8 dB) plus 3 quiet periods, so OK1JRD CQ'd at
  :00 of every minute. A first OK1JRD decode left the DX Call box empty, so
  stock autoselect doesn't pick up a CQ with Enable Tx off. The right-click
  needed full-screen control, which wasn't granted, so the queue was seeded
  through a temporary env-var hook in a local build (removed, never
  committed). Result: the 02:20:00 CQ decoded, Tx period went 00/30 ->
  15/45, Enable Tx on, `OK1JRD VU2CPL -08` (Skip Tx1 on) keyed at
  02:20:15.06 (fake rig `trx:0,true`). Not tried on air, and the right-click
  path itself wasn't exercised here (that code didn't change).

### 2026-10-16 — KK4CDK: Enable Tx OK on rc5; questions on busy stations and the queue

Miguel: the rc5 late-start cutoff works ("working as it should be"). He asked
three more things; a reply is drafted in
`~/projects/JTDX-records/jtdx-vu-reply-to-KK4CDK-2026-10-16.txt`, and Manoj sends it.
- **No Tx after the DX's RR73/73 to someone else.** This is stock behaviour.
  JTDX halts Tx when the station being called answers another
  (Settings > Sequencing "Halt TX if operator I called answered to other
  operator", or always when our Tx is within the guard band of his
  frequency, `mainwindow.cpp` ~4446), so Enable Tx is off by the time he's
  free. Workarounds in the reply: untick that box and call off his frequency,
  or the queue with "RR73/73" (Notifications, `73RR73Marker`) ticked.
- **"Wait till available" on right-click Halt Tx.** He asked again. It stays
  declined (2026-10-15), and the reply says so.
- **Queue never called anyone.** The queue only reorders AutoSeq's autoselect
  (`process_Auto`, runs only with the DX Call empty and call mode not None).
  It doesn't switch Enable Tx on. It picks a queued station heard in the
  current period: one calling us, answered ahead of all other callers
  (score 100000+, `qsohistory.cpp:180`), or one sending CQ, or RR73/73 with
  the marker on. In practice it
  works while you're running CQ. Probably Enable Tx was off, or a DX call was
  still set. A queued pick now switches Enable Tx on (next entry up).

### 2026-10-15 — KK4CDK: rc5 works on Windows, four Flex slices at once

Manoj: "it worked at Miguel". Miguel's screenshot (1650 UTC): four JTDX-VU
0.7.5-rc5 instances on Windows, each on its own FLEX-6700 VITA-49 slice
(B 17m, C 12m, D 20m, plus 10m), all decoding FT8 with the US state and country
columns filled in, AutoSeq on, his `CQ KK4CDK EL95` loaded. So rc5 runs on his
setup with several slices up together. The screenshot can't show the late Tx
start cutoff or the #6 fix at work; no report on those yet. Manoj sent a mail
asking Miguel how late Enable Tx behaves on rc5 (draft in
`~/projects/JTDX-records/jtdx-vu-reply-to-KK4CDK-2026-10-15b.txt`).
**Waiting on Miguel.**

### 2026-10-15 — 0.7.5-rc5: rc4 plus the late Tx start cutoff and #6 (test build)

Manoj: "make rc5". `Versions.cmake` 0.7.5-rc5 (`223967cf`); published as GitHub
**pre-release `v0.7.5-rc5`** (tag CI: Windows 38763264945 18 min, macOS
38763264795, both green). rc5 = rc4 + the late Tx start cutoff (KK4CDK), the
AutoSeq fix for #6 and the `macos-bundle.sh` @rpath fix (CI's macOS job runs it).
The two macOS zips were notarised with `macos-notarize.sh` (arm64
1cf4b115-e52f-4b3f-bd15-b2b1be8cfcbc, x86_64 46ffbae1-c974-4294-8417-05fc3bc7c666,
both Accepted, stapled) and replaced on the release (`--clobber`); anonymous
downloads checksum OK and "Notarized Developer ID", 0.7.5-rc5, right arch. Windows
zip sha256 OK, `jtdx.exe` says 0.7.5-rc5. All three replaced the rc4 zips in
`~/Desktop/jdxvu/test/` (rc4 trashed). Reply posted on #6 pointing to rc5. Mail to
Miguel with the rc5 Windows link drafted in
`~/projects/JTDX-records/jtdx-vu-reply-to-KK4CDK-2026-10-15.txt`; Manoj sends it.

### 2026-10-15 — macos-bundle.sh copies any missing @rpath library (Homebrew freetype now needs brotli)

A bundle built at 20:28 died at launch: "Could not load the Qt platform plugin
cocoa" - `libqcocoa` -> `libbrotlidec` -> `@rpath/libbrotlicommon.1.dylib`, not in
`Frameworks/`. Homebrew upgraded freetype at 19:05 that day and it now links
brotli; the 13:43 bundle was fine. macdeployqt doesn't follow `@rpath`, and the
script's checks only looked at `@loader_path`/`@executable_path` and absolute
Homebrew paths, so it said OK. Now a loop copies every `@rpath/<lib>` a bundled
dylib names from `$BREW/lib` or `$BREW/opt/*/lib` until none are missing, and a
check fails the script if one still is. The CI macOS job runs the same script,
so the rc5 Mac builds need this once the runners get the new freetype. The
rebuilt bundle launches and ran the #6 test.

### 2026-10-15 — AutoSeq keeps the report when the DX finishes with someone else (issue #6, for the next release)

marinfyi (#6): OK1JRD answered their CQ and got -17, worked F4LQJ meanwhile, and
on `F4LQJ OK1JRD 73` AutoSeq went back to `OK1JRD <mycall> <grid>`. Upstream
JTDX (Arvo ES1JA, 2020): in `QsoHistory::message`, a 73/RR73 to another station
(`RFIN`) resets a station to "free" when `t.status == SREPORT && t.srx_p <
RREPORT` - we sent a report, none came back. The DX's messages to F4LQJ are
`NONE` and don't touch `srx_c`, so `srx_p` was still `RCALL`. Now the reset
skips `srx_p == RCALL` (`qsohistory.cpp:485`): a station that called us keeps
getting the report. Unchanged: the Skip-Tx1 case (we answered its CQ with a
report, `srx_p == RCQ`), a CQ from it, and the 5-min stale-QSO rule. Reply
posted on #6 before the fix (cause + Skip Tx1 workaround, says a change is
being looked at).
- New `tools/qsoseq_test.cpp` (not built by CMake; build lines in its header):
  5/5 pass with the fix, the #6 case fails on the old `qsohistory.cpp`. Full app
  (`build/`) compiles and links.
- **Tested in the app 2026-10-15** (bundle, throwaway `-r q6test`, `tools/fake_tci.py`
  with `FAKE_TCI_WAVS` cycling 12 ft8sim periods: `VU2CPL OK1JRD JN79`, silence,
  `F4LQJ OK1JRD R+11`, silence, `F4LQJ OK1JRD 73`, 7x silence; Enable Tx by UDP
  TriggerCQ; Skip Tx1 off, AnsB4 on). **Old code** (fix reverted for the run):
  after the 73 the status went RFIN and the next Tx was `OK1JRD VU2CPL MK83`,
  twice - #6 reproduced. **Fixed code**, same audio: status stays SREPORT and Tx
  keeps `OK1JRD VU2CPL -08`. Not on air.
- **Why Manoj's own setup never shows #6:** a 73/RR73 to another station only
  reaches QsoHistory as `RFIN` when Settings "73/RR73 marker" (`73RR73Marker`,
  default **on**) is ticked - `displaytext.cpp:480`; his profiles have it off, so
  it arrives as `NONE`. To reproduce, also turn off Counters "answer in call"
  (`SeqAnswerInCallCount`, it drops the DX as soon as it replies to another)
  and use a station not in the log (or AnsB4 on).

### 2026-10-15 — No late Tx start after the first 20% of the period (for the next release)

KK4CDK (Miguel): "sometimes I hit the ENABLE Tx and it triggers TX even though
is out of sync, so that short call is lost". Stock JTDX behaviour: the start
check in `guiUpdate` was `m_bTxTime and fTR<99.0` (always true), so PTT keyed
at any point in the Tx period and the Modulator skipped ahead (`m_ic` from
`mstr - delay_ms`), sending only the tail of the message. Now
`lateStartOk = fTR<0.2` (FT8 3 s, FT4 1.5 s, FT2 0.75 s, JT65/JT9 12 s);
JTTY (period-free) and WSPR (scheduled at `m_nseq==0`) keep the old rule, Tune
is not affected. Past the cutoff Enable Tx stays on and Tx starts at the next
Tx period. The 20% also covers AutoSeq replies armed by a slow decode just
after the period starts (up to 3 s in FT8). **Tested 2026-10-15** on the bundle
(`build/bundle/JTDX-VU.app`, throwaway `-r latetest`, `tools/fake_tci.py`, FT8,
Tx period 15/45): Enable Tx sent by UDP TriggerCQ at set offsets (new
`tools/tx_late_start_drive.py <udp port> <offsets...>`), PTT read from the fake
rig's `trx:0,true` times. +1.0 s and +2.5 s keyed at once; +3.5, +6.0
and +10.0 s did not key and keyed at the next Tx period start (+0.015 s). A
first run on `build/jtdx.app` quit mid-test (no `jtdxjt9` - the known gotcha);
its later cases were void.

Miguel also asked for a right-click on Halt Tx to "wait till available" (auto
after 4 tries, resume when the DX is free) from "a previous version" - not in
JTDX 2.2.159 (nearest: Settings > Counters, the call-after-73 modes).
**Declined** (Manoj, 2026-10-15: not keen); dropped from the reply.

### 2026-10-15 — KK4CDK confirms: no blank rows in Band Activity on Windows

Miguel sent a Windows screenshot of the FT8 pane (20m, 2139-2140 UTC) on the
0.7.5 pre-release: no empty rows, and grid-derived states (`MA-NH`,
`CA-AZ-NV`) and the country column show inline after US lines, which is
where the blank rows used to be. Manoj: "i think the issue is sorted". The
CRLF fix (2026-10-13 entry) is confirmed on Windows; it ships with 0.7.5.

### 2026-10-14 — Developer ID signing + notarisation for releases

Manoj: "use mshv notary script and sign future releases with my certificate".
New `macos-notarize.sh` (adapted from MSHV-Mac `macos/notarize.sh` and the
signing block of its `build-standalone.sh`) and `Darwin/JTDX-VU.entitlements`
(disable-library-validation + audio-input, as MSHV). It takes the CI zips,
re-signs `--deep` with the Developer ID, hardened runtime and `--timestamp`,
checks the audio-input entitlement, submits with `notarytool --wait` (fails
and prints the log unless Accepted), staples, and rewrites the zip
(`ditto --norsrc`) and `.sha256` in place. CI stays ad-hoc so no certificate
or Apple credentials go into GitHub secrets. Tested on a scratch copy of the
rc4 arm64 zip: Apple status Accepted (c4dbabf7-e6de-4619-a3cc-362c541054e9),
`spctl` "Notarized Developer ID"; unzipped with `unzip` (no `._` files),
quarantined, `open`ed: launched translocated with no Gatekeeper prompt and
`jtdxjt9` started. Then, on Manoj's yes, the published **rc4 macOS zips were
replaced** by notarised ones (arm64 08edd53d-963f-4a60-a558-0ee99023f7dc, x86_64
42b015c0-c610-4984-a730-e5185cf316e1, both Accepted; `gh release upload --clobber`
with new `.sha256`); anonymous downloads checksum OK, "Notarized Developer ID",
ticket stapled, 0.7.5-rc4, right arch. `~/Desktop/jdxvu/test/` holds the same
notarised zips. Windows zip untouched (still not code-signed).
README first-launch and release-build text updated.

### 2026-10-14 — 0.7.5-rc4: PR #3 plus issues #1, #2, #4 (test build)

Manoj: "make rc4 with the PR for testing", then "check all open issues first",
"go ahead with all four", "post the replies and build rc4". A first rc4 CI pair
(PR #3 only, `b0d4fb46`) was superseded: its Windows run died on an MSYS2 mirror
timeout and the macOS run was cancelled. rc4 = rc3 + PR #3 + the three fixes below.
Replies posted on #2 (explains the fix, Misc > Bypass all text filters meanwhile)
and #5 (Intel crash fixed from rc2, closes on release). Published as GitHub
**pre-release `v0.7.5-rc4`** (tag on `2ffe13d0`; tag CI: macOS 38472656522, Windows
38472656624, both green; the dispatched runs were cancelled as duplicates). All three
zips sha256 OK, Mac Info.plist and jtdx.exe say 0.7.5-rc4; they replaced the rc3 zips
in `~/Desktop/jdxvu/test/` (rc3 trashed). VU2OY asked on #1 and #4 to test rc4.

- **#1 TCI, AetherSDR leaves our receiver out of its connect burst** (VU2OY):
  `do_start` now sends `vfo:<rx>,0;` when no frequency arrived, and `do_poll` asks
  again instead of throwing on an empty frequency (`8b0f5ab2`). New
  `tools/fake_tci.py` switch `FAKE_NO_VFO=1` and `tools/tci_novfo_test.sh`:
  rc3 (installed app) 5 connects in 25 s and no query; fixed bundle 1 connect,
  1 vfo query, audio started.
- **#4 Flex over a VPN** (VU2OY, `4529e5ed`): (1) a "no audio" failure on a
  connection that had audio is a dropped link, not a firewall - it no longer counts
  toward the two-strikes stop (`m_flexAudioSeen`, set in `dataSink`, read and
  cleared in `handle_transceiver_failure`); (2) the offline updates (frequency 0)
  sent while reconnecting no longer become `m_freqNominal` - with Monitor on they
  had zeroed `m_lastMonitoredFrequency`, so the reconnect followed the radio's
  default 14.100 slice (snapped to 14.074). Found with a temporary stderr trace in
  `handle_transceiver_update` (`last=0` after the offline updates). New
  `tools/flex_reconnect_test.sh` (MODE=kill / mute, on fake_flex with
  FAKE_GUI_SLICE): rc3 kill -> back on 14.074; fixed kill -> 21.074 (7 starts),
  mute -> reconnects on 21.074 (rc3 gave up), FAKE_NO_AUDIO -> still stops after
  2 starts. Not tried on VU2OY's real VPN link. Part (b) of his report (the
  radio's own 14.100 fed back while our tune is busy) did not reproduce on the
  fake radio once (a) was fixed.
- **#2 Bypass hidden** (iw1gb, `ce8e688d`): the Auto CQ button's Ignored vertical
  policy gave it ~1 px in the column and its 40 px minimum painted it over Bypass
  (y=221 h=40 vs y=222), at every window size and language. Now a fixed height.
  Screenshots of the Italian UI at 593 and 750 px show Bypassa between Auto CQ
  and 1 QSO.
- **Test gotcha:** run app tests on `build/bundle/JTDX-VU.app` (`./macos-bundle.sh`),
  not `build/jtdx.app` - the latter has no `jtdxjt9`, a "Subprocess error" box
  opens and, being modal, holds the TCI/Flex retry timer, so reconnect tests
  falsely show no retries. And never relink while a test runs on that binary.

### 2026-10-14 — PR #3 from Vinod VU3ESV merged: double-click edge cases (for the next release)

Manoj: "take vinod pr 3". First outside PR on the repo; merged as-is with a merge
commit, all five of Vinod's commits kept. What it does when a decode is double-clicked
(or a UDP Reply arrives):

- A separator line, blank line or (under AutoSeq) a free-text line no longer flips
  the Tx period. Before, with Enable Tx + AutoSeq on, that flip dropped the QSO
  partner and called CQ in the other period.
- `<our-call> 73` / `RRR` / `RR73` lines that name no sender do nothing. Before,
  they cleared the DX call and armed Tx with an empty Tx5.
- `RR73` is no longer taken as a callsign (`decodedtext.cpp`), so `TU RR73` doesn't
  make "RR73" the DX call.
- `Radio::is_callsign("")` no longer reads past the end of the string.
- New `tools/callparse_test.cpp` (not built by CMake; build lines in its header).

Checked here: the test built against Homebrew qt@5 passes 15/15, and the full app
(`build/`) compiles and links. **Not yet tried in the running app or on air**: the
checklist in the PR body (AutoSeq on/off, separator, `<our-call> 73`, Ctrl/Alt/Ctrl+Alt)
still needs doing. Follow-ups Vinod lists in the PR (a hashed own call isn't
recognised as "to me", JT65 `left(47)` reaching into the country column, the
unreachable `DE` branches, the AutoSeq `nonstandard73` frequency check) are not done.

**Checklist run 2026-10-14** (Manoj: "run the PR #3 checklist on rc4") on the local
rc4 bundle (`2ffe13d0` code), throwaway `-r pr3test` on `tools/fake_tci.py` playing
two `ft8sim` mixes (even: `CQ K1ABC FN42`, `VU2CPL 73`, `TU RR73`, `VU2CPL RR73`,
`VU2CPL W9XYZ -12`; odd: `CQ JA1XYZ PM95`, `TNX 73 GL`), `InsertBlank=true` for
separator lines, `WriteDecodedDebugALLTXT=true`, Monitor off so the pane stays put,
real double-clicks (`tools/click.swift x y 2 cmd|alt|cmdalt` - on macOS Qt's Ctrl is
Cmd; rows found with `tools/decode_rows.py` on a fresh screenshot before each click). All seven items pass:
- AutoSeq on, Enable Tx on, mid-QSO with W9XYZ (Tx3, 15/45): separator,
  `TNX 73 GL` (other period), `VU2CPL 73`, `VU2CPL RR73`, `TU RR73` - DX call,
  period, Tx3 and Enable Tx unchanged every time; DX call never `RR73`.
- AutoSeq off, period 15/45: separator - period unchanged (report reset to 0,
  the older behaviour the PR leaves alone); `TNX 73 GL` from :45 - period flips
  to 00/30, report -8 (its SNR).
- Plain double-click on CQ answers (period flips); Cmd: Tx to the decode's 700 Hz;
  Option: answers with Enable Tx off; Cmd+Option: K1ABC into Wanted, DX unchanged.
- ALL.TXT: "double click on call processed" only for the 4 answered clicks.

### 2026-10-13 — 0.7.5-rc3 (Windows only) for KK4CDK

Manoj: "make rc3 windows build for miguel". `Versions.cmake` 0.7.5-rc3;
Windows run 38410084076 (no macOS rc3 cut - the Mac never had the bug).
`JTDX-VU-0.7.5-rc3-windows-x64.zip` (sha256 OK) is in `~/Desktop/jdxvu/test/`,
the rc2 Windows zip trashed; its `jtdx.exe` embeds `EL96 FL\n` (rc2 had
`\r\n`). Contents: rc2 plus the blank-row fix below. Published as a GitHub
**pre-release** `v0.7.5-rc3` (Windows zip) for Miguel to download - the update
check reads `releases/latest`, which skips pre-releases. The tag also set off
the `v*` CI runs (macOS 38414418685, Windows 38414418267), which add the Mac
rc3 zips and re-upload the Windows one (both runs succeeded). The Mac rc3
zips (sha256 OK) replaced the rc2 ones in `~/Desktop/jdxvu/test/` on
2026-10-14; the rc3 Intel build has not had the Rosetta launch check. Manoj
mailed Miguel the pre-release link on 2026-10-14; he confirmed the blank rows
are gone on 2026-10-15.

### 2026-10-13 — Blank rows in Band Activity on Windows: CRLF in the US-state tables (for the next release)

KK4CDK (Miguel)'s blank rows, solved from his copied pane text: every
blank row sits right after a US line whose state came from the **grid**
(KI6QDH DN71 "WY-NE", W6HDG EL96 "FL"); US lines whose state came from the
callsign (KZ7Y, no grid) had none. The Windows CI checkout
(`actions/checkout`, `core.autocrlf=true`) turns `us_grid_states.txt` /
`ca_grid_provinces.txt` / `us_call_states.txt` into CRLF before rcc embeds
them - the rc2 `jtdx.exe` resource holds `EL96 FL\r\n`. `read_grids`
split on `\n` only, so the state was `FL\r`, and `QTextCursor::insertText`
takes that `\r` as a paragraph break: an empty block after the line. The
call table only reads two characters after the space, so it was clean.
Mac/Linux checkouts are LF, hence never seen here. Fix: `read_table`
drops every `\r` (also covers a user's own CRLF copy in the data
directory), and `.gitattributes` pins the three tables to `eol=lf`.
Canadian provinces from the grid were affected the same way. Builds on the
Mac; **confirmed on Windows by Miguel 2026-10-15.**

### 2026-10-13 — 0.7.5-rc2: rc1 plus the Settings wheel change

Manoj: "cut rc2 with the scroll wheel fix". `Versions.cmake` 0.7.5-rc2 on
`ac0ff136`; macOS CI run 38357866722 and Windows run 38357870178.
`JTDX-VU-0.7.5-rc2-windows-x64.zip` and `-macos-x86_64.zip` (sha256 OK) are in
`~/Desktop/jdxvu/test/`, the rc1 ones trashed; the rc2 Intel build launched
10/10 under Rosetta (crash check). The arm64 zip follows when its job ends.
Contents: Intel start-up crash fix, third/fourth Flex copy join, aligned
country column, Canadian provinces, rc-aware update check, Settings boxes
ignoring an unfocused wheel.

### 2026-10-13 — Settings: combo and spin boxes take the wheel only when focused (for the next release)

Manoj's QO-100 profile came up on the Flex rig at 11:40 (its Flex trace was
born then; only the Rig key had changed; two builds were running that profile
at once). No code copies a rig between profiles (the rc build's Profile switch
from a Flex profile to a rigctl one was tried: the target kept rigctl), so the
likeliest cause is a wheel over the Rig box in Settings > Radio: Qt combos
change on the wheel. Manoj: "ignore scroll wheel - yes". `Configuration::impl`
now gives every QComboBox / QAbstractSpinBox in the dialog StrongFocus and an
event filter that drops QEvent::Wheel unless the box has focus (click first,
then the wheel works as before). **Wheel-tested** 2026-10-13 on a `-r wheeltest`
copy of the rc1 bundle: unfocused scrolls over IARU Region, Tx watchdog and
the Rig box changed nothing; after a click the Tune timer went 30 -> 35 s and
the Rig box moved one entry (None -> ADAT ADT-2) per notch; Cancelled.

### 2026-10-13 — Test builds are release candidates: 0.7.5-rc1 (version scheme)

Manoj: "why not name the test builds as rc XX?" Done: `Versions.cmake`
`JTDXVU_VERSION 0.7.5-rc1` - the title bar, About, Info.plist, PSK Reporter and
the CI zip names (`JTDX-VU-0.7.5-rc1-macos-x86_64.zip` ...) all follow. The
release is cut by setting the plain `0.7.5`. `UpdateCheck::is_newer` now parses
an `rcN` suffix (`parse_version`): numeric parts first, then the release beats
every candidate of its number and rc2 beats rc1 - before, "0.7.5-rc1" parsed as
(0,7,5,1) and would have sorted above the release, so rc testers would never
have been offered it. Ten cases checked in a scratch program linked against
updatecheck.cpp. Automatic checks stay on for rc builds (only "dev" turns them
off). The .deb name would be `jtdx-vu-0.7.5-rc1-...` (Debian would sort that
after 0.7.5; use `0.7.5~rc1` if an rc .deb is ever shipped). The earlier
"trial" zips on the Desktop are replaced by the rc1 CI zips.

### 2026-10-13 — Start-up crash on Intel Macs: uninitialised dark-style flag (for the next release)

- **Report** (Manoj's photo of a user's AI-assistant write-up; Intel Mac, macOS
  15.8): 0.7.3 and 0.7.4 crash before any window about half the launches, in
  `Configuration::impl::vu_notifications_page ()`; 0.7.0 ran reliably. The user
  went back to 0.7.3 and relaunches when it dies.
- **Reproduced here:** the verified 0.7.4 x86_64 app run under Rosetta with a
  throwaway `-r crashtest` profile died 1 of 6 launches (SIGSEGV, EXC_BAD_ACCESS
  at a wild address); the .ips shows `vu_notifications_page ()` <- `impl::impl`
  <- `MainWindow::MainWindow`. arm64 never crashed in any test.
- **Cause:** `impl::impl` calls `vu_notifications_page ()` (line ~1513), which
  ends with `vu_notify_refresh_ ()` painting every colour swatch through the
  `dark (light, darkc)` lambda, before `read_settings ()` (line ~1605) sets
  `useDarkStyle_` - declared without an initialiser (JTDX-VU's redesigned page,
  v0.6.0). On x86_64 clang compiles the conditional as an indexed select, so
  a garbage byte becomes a wild pointer; on arm64 the byte happened to be 0.
- **Fix (`b028c72b`):** `useDarkStyle_ {false}`, also `rig_report_offset_mhz_
  {0.}` and `qo100_rig_mhz_ {0.}` (the other JTDX-VU members without one), and
  `vu_notify_refresh_ ()` once more right after `read_settings ()` so the
  swatches show the real theme. The stock JTDX bools without initialisers
  (filters, rig flags) are only read after `read_settings ()`; left alone.
- **Verified:** arm64 builds and starts with dark style on; the Intel binary
  from macOS CI run 38351958875 (workflow_dispatch on `b028c72b`, sha256 OK)
  launched **20 of 20** times under Rosetta with a throwaway profile,
  alternating UseDarkStyle true/false, no crash reports - against 1 crash in
  6 for the release 0.7.4 x86_64 app the same way. Needs a release for Intel
  users. Builds for the reporters: **0.7.5-rc1** (`2fd4c7b6`), macOS CI run
  38354885914 and Windows run 38354889408, in `~/Desktop/jdxvu/test/` as
  `JTDX-VU-0.7.5-rc1-macos-x86_64.zip` (for the Intel reporter) and
  `JTDX-VU-0.7.5-rc1-windows-x64.zip` (for KK4CDK: the third-copy Flex join,
  the aligned country column, the provinces and this fix), `.sha256` beside
  each, both checked. The earlier "trial" zips (0.7.4-labelled) are in the
  Trash, as is the loose Intel trial app Manoj had opened from one of them.
- TCI audit from the MSHV posts Manoj forwarded (RX2 instance keying the wrong
  slice; crashes): JTDX-VU's `TCITransceiver::do_ptt` keys `trx:<rx_>,true`
  with rx_ 1 for the "TCI Client RX2" rig, so the receiver is right on our
  side; what the SDR program does with `trx:1` (moving the TX slice) is its
  business. Nothing to change.

### 2026-10-13 — Flex: a third and fourth JTDX-VU join a GUI client's station (for the next release)

Manoj: the 6700 has 8 slices, enable it (the limit KK4CDK hit is the radio's
two GUI clients, not slices; models: 6400 / Aurora 510 2 slices, 6600 / 8600 /
Aurora 520 4, 6700 / 8800 8, M suffix irrelevant).

- **FlexTransceiver::start_session:** `sub client all` first; `client gui
  <uuid>` (a fresh QUuid per connection, as SmartSDR sends one, so others can
  bind to us); `client program JTDX-VU`. Refused with **0xF3000001** ->
  `bind_to_gui_client ()`: pick a GUI client from the `client 0x<h> connected
  client_id=<uuid> program=<p>` status lines (JTDX-VU preferred, else any, e.g.
  SmartSDR; disconnected ones dropped), `client bind client_id=<uuid>`,
  `bound_handle_` = that client. Any other refusal code: retry the plain
  `client gui` (older firmware / id syntax), never a bind - so a lone JTDX-VU
  cannot end up inside SmartSDR's station by accident.
- **Slice ownership:** `owner_hex ()` (the bound GUI client's handle, else
  ours) replaces `handle_hex ()` in `owned_slice*`. A joined copy takes over
  only its own letter within that station, and not if that slice already feeds
  a DAX channel (another JTDX-VU's) - it creates one instead; `slice create`'s
  reply body (the index) names the new slice. The old "slice index = letter"
  fallback now applies only to a slice with no `client_handle=` (pre-multiFLEX
  firmware) or ours: on the fake radio without handed slices a second GUI
  copy had taken the first copy's slice 0 through it.
- DAX channel stays letter + 1, so copies on one station with different
  letters use different channels; `do_ptt` already sets `slice set n tx=1`
  before `xmit 1`, so whichever copy keys gets the transmitter.
- **tools/fake_flex.py** rewritten per client: own handles, `FAKE_GUI_LIMIT`
  (default 2) -> F3000001, `sub client all`, `client program/station/bind`,
  slices with owner and per-station `index_letter`, per-client DAX streams and
  audio, status broadcast to every client, one TX slice; a GUI client's slices
  go when it disconnects. Also fixed: without `--sig` it scheduled periods it
  had no signals for and died (KeyError) once a client took audio.
- **Tested** (local bundle): three copies `-r flexa` (Slice A), `flexb`
  (Slice A), `flexc` (Slice B) on the fake, with and without handed slices:
  a and b GUI clients with their own slices, c refused, joined a's station,
  created slice 2 = station letter B on DAX 2, all three stayed connected
  45-60 s. **Not yet on a real radio:** Manoj to run three copies on the 6600
  (two GUI + one joined), watching flex_trace.txt for "joined GUI client";
  unknowns there: whether a bound client may `slice create`, `xmit`, and get
  DAX streams, and whether `client gui <uuid>` is accepted (else the plain
  retry). `tools/flex_multicopy_test.sh` runs the three-copy test on the fake.

### 2026-10-13 — KK4CDK's third report: blank lines, a third VITA-49 instance, typing a frequency

Three items in one mail (v0.7.4 on Windows, FLEX-6700).

- **Blank lines in Band Activity** (screenshot: ~10 empty rows among 22
  decodes of one period, not tied to bold or struck-through lines; he had
  changed fonts on the earlier advice and found no pattern). **Not
  reproduced on the Mac:** a `-r aligntest` copy with New only ON and a log
  where W1AW and DL1ABC are worked hid those two of eight ft8sim lines with no
  gap (`hide_last_block` / `QTextBlock::setVisible(false)` takes no height on
  Qt 5.15 here), and with New only off nothing is hidden at all. His
  screenshot shows struck-through (worked-B4) lines, so New only is probably
  off for him, which would make the blanks real appended lines - something
  the Windows decoder/pipe hands `readFromStdout` that the Mac does not
  (whitespace-only lines would render blank: `appendText` inserts them as
  is). No guard for that exists. **Waiting on Miguel:** is New only ticked;
  copy the pane's text (click in it, Ctrl+A, Ctrl+C) so the blank rows'
  content can be seen.
- **Third JTDX-VU instance refused** (`client gui refused (0xf3000001)`;
  two instances fine; closing one lets the third in; he wants four). That
  is the radio's multiFLEX licence: every FLEX-6000 allows **2 GUI
  clients** (`licensed_clients=2` in the discovery packet) and JTDX-VU
  connects as a GUI client (`client gui`). A third/fourth instance would
  have to connect as a non-GUI client bound to one of the GUI clients
  (`client bind client_id=<uuid>` of a JTDX-VU GUI client, found in the
  `client ... connected client_id=... program=JTDX-VU` status lines already
  kept in `status_log_`), the way SmartSDR's DAX program does; slices then
  belong to that client's station, so `owned_slice*` would have to accept the
  bound handle. Needs a real radio with three instances to test (the 6600
  allows 2 GUI clients too, so Manoj can). About half a day plus the test.
  **Manoj to decide.**
- **Typing a frequency in the main window** ("strictly a dropdown"): it is
  not - the band box is an editable combo (stock JTDX): type `28.088` over
  the band name and press Enter; `LiveFrequencyValidator::fixup` emits
  `valid(f)` -> `band_changed` -> `setRig`. **Tested with the Flex rig type**
  on a `-r freqtest` copy against `tools/fake_flex.py`: the dial showed
  28.088 000, the box 10m, DIGU kept, and the fake radio logged
  `slice tune 0 28.088000 autopan=1`. README now says so. If it fails for him
  on Windows, ask what the box does (red OOB = outside every band).
- Test leftovers: none (both copies cleaned up). Scratch scripts
  the decode-window one is now
  `tools/decode_window_test.sh` (WAVS=..., EXTRA_INI, LOGADI); the freqtest ones were scratch.

### 2026-10-11 — Show US State also shows Canadian provinces (for the next release)

- **Asked for by KK4CDK** (Miguel, after thanking for the US states). Manoj: yes.
- **Grid first:** new `ca_grid_provinces.txt` (1,176 squares, 9.8 KB, built in as a
  resource) from Natural Earth 1:10m admin-1 (public domain, v5.1.1) by new
  `tools/build_ca_grid_provinces.py` - same method as the US table: provinces /
  territories covering at least 2% of the square's Canadian land, largest first
  (FN25 QC-ON, FO64 NL-QC, FN74 NS, EN82 ON).
- **Else the prefix** (no licence table needed): VE/VA 1-9 = NS QC ON MB SK AB BC
  NT NB, VO1/VO2 = NL, VY0 NU, VY1 YT, VY2 PE, special-event prefixes (CF-CK, CZ,
  VB-VD, VG, VX, XJ-XO) by the VE digit except 8; VE0 (maritime mobile) and VY9
  give nothing. A station that moved but kept its call shows the old province.
- **Code:** `UsStates::findCanada` beside `find` (logbook/usstates.cpp); the
  displaytext hook now covers DXCC prefix "VE" as well as "K". Same tick
  (`ShowUSState`), relabelled "Show US State / VE province", tooltip updated.
- **Tested:** a standalone program over 22 cases (all 13 codes, grid vs prefix,
  RR73 ignored, /P, <hashed>, CG3/XM7/VC8, VE0, a non-Canadian grid) - all as
  expected; US lookup unchanged (EM78 KY-IN-OH). Builds.
- **Seen in the app** (2026-10-11, local bundle of `778f2e5a`): ft8sim signals
  (CQ VE3ABC FN25, W1AW VE7XYZ -10, CQ VA2QQ FO64, K1ABC VO1AA R-05, CQ VY2ZZ
  FN86, CQ CG3EV FN03, two non-Canadian) mixed into one 15 s wav and played
  through `FAKE_TCI_WAVS` to a throwaway `-r aligntest` copy: the Rx Frequency
  pane showed "Canada:QC-ON" (grid) and "Canada:BC" (prefix), and the bold and
  plain lines' country columns started at the same x. The Settings label "Show
  US State / VE province" fits in General > Display.

### 2026-10-11 — Decode window: country column lined up on bold lines (for the next release)

- **Report (KK4CDK, v0.7.4 on Windows, screenshots):** with Show US State on,
  the country column of the CQ / 73 lines (drawn bold, stock JTDX) sat ~50 px
  right of the plain lines - "*Canada" as much as "U.S.A.:MO", so not the
  state itself. His font's bold is wider than its regular: Windows fakes bold
  for the fallback of JTDX's default "Courier, 10" by widening each glyph, and
  the column was placed only by space padding. His stock JTDX (Slice-B
  instance) shows every line bold, hence aligned; JTDX-VU took its settings
  from his default JTDX instance. He also asked for "USA:FL" - left as
  "U.S.A.:FL", which is JTDX 2.2.160's own format (his stock screenshot shows
  it); the stock "show DXCC prefix" option gives "K:FL". Manoj sent him the
  font workaround (Settings > General > Decoded Text Font: Consolas / Lucida
  Console, or bold).
- **Fix (`displaytext.cpp`):** `setContentFont` measures the widest character
  cell of the content font in normal / bold / black, upright and italic
  (`charWidth_`); `appendText` puts a tab stop at `(ft + 26) * charWidth_ + 2`
  on the line and a tab before the marker + country, so the column starts at
  the same x whatever the weight. Measured with the same font engine that
  draws, so a faked bold is accounted for. The tab sits after column 49; the
  line parsers (`DecodedText`: columns 23-46; the UDP replay: first space from
  column 35) never reach it. Lines of 50+ characters are unchanged.
- **Tested** on the Mac with Andale Mono 13 (no bold face, so macOS fakes it)
  in a throwaway `-r aligntest` copy fed by new `FAKE_TCI_WAVS` in
  `tools/fake_tci.py` (plays 15 s 12 kHz recordings, one per UTC period, as
  TCI audio - real FT8 decodes with no radio): installed 0.7.4 had the bold
  lines' country ~2 px right of the plain ones; the fixed build lines "Spain",
  "Netherland" and "Italy" (bold) up exactly. Windows' wider faked bold not
  seen directly.

### 2026-10-10 — Decoder speed measured: Mac mini vs Pi 5, threads, `-fbounds-check` (no code change)

Manoj asked whether a Rust rewrite would make JTDX-VU faster; measured first.
New `tools/decode_bench.cpp` drives a real `jtdxjt9` through the same shared
memory and `.lock` handshake as MainWindow with recorded 15 s FT8 .wav files
(Manoj's old WSJT-X `save/`, 2,500 files) at his profile settings (depth 3,
FT8 cycles 3, sensitivity 2, AGCc, hint) and times each decode
(`DECODE_BENCH_SHOW=1` prints the messages to compare builds). Gotcha: after
a decode, jtdxjt9 polls for the new `.lock` only every 100 ms; remove it again
sooner and the decoder never sees it and hangs, so the harness waits 300 ms.

- **Mac mini M4 Pro, installed 0.7.4, 60 files (busy 2020 periods, up to ~50
  decodes):** auto threads (12 procs → 9) mean 489 ms, p95 797, max 931; 1
  thread 1,678 / 3,223 / 3,483; 4 → 699 mean; 8 → 503; 12 → 468. No
  problem: decodes are done ~1 s after the period ends.
- **Pi 5 (meridianpi5), 0.7.4 build, same files:** auto (4 cores → 3) mean
  3,203 ms, p95 6,005, max 7,594; 1 thread max 13,522; 2 → 3,913 mean;
  **4 threads mean 2,771, p95 4,854, max 5,219**. No throttling (57.6 °C,
  0x0). Decoding starts ~14.1 s into the period, so on a busy band the Pi's
  decodes land 3-7 s into the next one - after the reply slot has begun.
  Decode > FT8 decoding > FT8 threads = 4 is the user-side fix (leaves no core
  spare for the GUI/audio; not yet tried in the running app on a Pi).
- **Profile (`sample`, Mac, auto threads):** of the decoder's busy samples
  ~53% are OSD (`osd174_91` + `fetchit91` + `nextpat91`, the depth-3 deep
  decode), ~20% `sync8`, ~9% BP + tanh, ~4% FFTW. Algorithm-bound, not
  language-bound: a Rust port would not change this.
- **`-fbounds-check` in Release Fortran flags** (CMakeLists.txt gfortran
  block, inherited from upstream's initial commit): an A/B of two otherwise
  identical local builds. Mac: 1 thread 1,661 → 1,256 ms (−24%), auto 491 →
  398 ms (−20%). Pi: auto 3,203 → 3,003 (−6%), 4 threads 2,771 → 2,588 (−7%),
  max 5,219 → 4,895. Decodes: over 300 more files single-threaded (repeatable),
  3 of 3,444 messages differ (2 only with the checks, 1 only without) -
  floating-point reordering on borderline candidates, smaller than the
  run-to-run spread of one build with threads (1,529 vs 1,531). Trade-off:
  without the checks an out-of-range index corrupts memory silently instead of
  stopping jtdxjt9 with a runtime error. Not changed; Manoj to decide.
- Nothing installed or changed in the app. Test copies: Pi `~/bench`
  (harness, 60 wavs, the no-check build in `bB`).

### 2026-10-10 — v0.7.4 released: Flex through firewall/VPN, Discover, Show US State, QO-100 13cm alerts

- https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.7.4, annotated tag on
  `7ea63474` ("Version 0.7.4": `Versions.cmake` 0.7.3 → 0.7.4; README title
  and title-bar example; Show US State, Discover and the firewall/VPN audio
  fix marked "since v0.7.4"). Cut on Manoj's "yes. release" after he ran the
  Flex VITA-49 profile on the Mac mini, which is on a different VLAN from the
  radio: connected with the typed address, audio and waterfall OK - so the
  `udp_register` change is fine on his own radio too. Discover found nothing
  there, as expected across subnets; Discover on the radio's own subnet is
  still untested in the app. No Fortran changed since v0.7.3.
- Contents since v0.7.3: `deeb1bbc` (QO-100 13cm lookups), `733aa9aa`
  (udp_register), `28b09e21` ("FlexRadio:" + Discover), `1782dc05` (Show US
  State), `5987d1e1` (no-audio stop, one frequency, ERR), README Common
  problems. Notes from the v0.7.3 template; Windows step 4 now mentions the
  Flex's UDP audio; credits add the US Census / FCC data and DXCA's WAS rules.
- CI macOS 38022453930 and Windows 38022453920 green. Pi .deb on meridianpi5
  (rsync of `git ls-files` at `7ea63474`, `cmake ..`, `nice make -j3`, `cpack -G
  DEB`): jtdx-vu 0.7.4 arm64, Conflicts: jtdx, `apt-get install -s` clean, an
  8 s offscreen `--test-mode` start OK (no jtdxjt9 left; shm 65595, `~/.qttest`,
  `/tmp/JTDX-VU - test*` removed), uploaded by hand with its `.sha256`. It is
  2.3 MB bigger than 0.7.3 (the built-in state tables).
- All eight files downloaded anonymously (200), every checksum OK; both macOS
  apps report 0.7.4 with the right arch and pass `codesign --verify --deep
  --strict`; 0 AppleDouble entries; Windows `jtdx.exe` carries 0.7.4 and the
  releases/latest URL. sha256:
  - `JTDX-VU-0.7.4-macos-arm64.zip` `19914559c04bae796dd42ea78cc72373675c82be2f51f44e825f097dd38cbe59`
  - `JTDX-VU-0.7.4-macos-x86_64.zip` `0079249f82cf3a6c300b6c3868c0a96bdbb089a35f5a8279d4b7246924f8eeb5`
  - `JTDX-VU-0.7.4-windows-x64.zip` `d9ae1dad0fb8c06c867bef64282d0dbbac5bc4243ec6b97bfbe2164f6e829d10`
  - `jtdx-vu-0.7.4-linux-arm64.deb` `1cbf54c3b519093a6694a160559a5e7751c9bddd1b7d9e54d52841dcf48f44fc`
- Installed on the Mac mini from the arm64 release zip (JTDX-VU was not
  running; the 0.7.3 app went to the Trash; executable identical to the
  release, signature verifies). The MacBook still runs the trial build.
- Website (vu2cpl.github.io `bdf2104`): card sentence, page paragraph
  "FlexRadio through a firewall or VPN, Discover and US states (v0.7.4)",
  downloads / release link / apt on v0.7.4; tested-so-far now says the update
  check is confirmed in the installed app and KK4CDK has logged Flex QSOs on
  Windows. Pushed after all eight assets answered.
- Users on v0.7.3 get the in-app update notice for this release (first
  release since the check went in).

### 2026-10-10 — Flex: no-audio reconnect loop stopped, connect-time frequency fight fixed, ERR box stays clickable (released in v0.7.4)

- **From KK4CDK's flex_trace.txt** (FLEX-6700, Windows, same LAN, v0.7.3):
  every connect created the DAX RX stream to his PC (`ip=192.168.1.72`,
  `dax_clients=1`), no audio arrived, and 10 s later it hit `no audio from the radio`.
  The TCI-style auto-reconnect then restarted it every ~15 s, about 30 times in
  10 minutes, re-tuning each time (his relay clicks). Cause: almost certainly
  Windows Firewall; the `udp_register` fix (733aa9aa) should get through it.
  **Confirmed:** after the firewall advice Miguel reported his first QSO
  completed and logged (2026-10-10), on v0.7.3, before any trial reached him.
- **Frequency fight:** at every connect two requests went out 38 ms apart,
  14.074 then 14.100. On coming online, `switch_mode` snapped the radio's
  default 14.100 to the FT8 frequency 14.074, then the deferred "start on the
  last frequency" `force_rig_mode (m_lastMonitoredFrequency)` sent an older
  14.100. **Fix** (`handle_transceiver_update`): for a Flex coming online the
  remembered frequency replaces the radio's before the mode refresh, and the
  deferred call re-asserts the mode on the same `m_freqNominal`. One target.
- **Loop stop** (`handle_transceiver_failure`): a Flex that fails with "no audio"
  twice in a row (`m_flexNoAudioCount`, reset when audio arrives in `dataSink`)
  stops auto-reconnecting. It shows a status line and a message naming the firewall
  (Windows and macOS steps, VPN), and says to click the red ERR box.
- **ERR box:** `on_readFreq_clicked` now calls `rigOpen ()` when offline. A
  failing rig's final offline update no longer repaints `readFreq` green and
  disabled. That had happened on every failure, so the "click ERR to reset"
  tooltip never worked.
- **Tested** against `tools/fake_flex.py` with two new switches: `FAKE_NO_AUDIO`
  (control only, no UDP) and the existing `FAKE_GUI_SLICE` (14.100 USB slice
  handed to the client), with Data mode on. Connect 1 sent one `slice tune 0
  14.074` and one `slice set 0 mode=DIGU`, with no jump back. Connect 2 needed nothing. There were
  exactly two connects, then the message, and the box stayed red "ERR". Clicking ERR was
  not tested (screen locked). The "polluted remembered 14.100" case from his
  trace was reasoned about, not reproduced.

### 2026-10-10 — Show US State (released in v0.7.4)

- **Asked for by** KK4CDK (Miguel). JTDX 2.2.160-rc10 has it, but 2.2.160 is
  a closed beta whose source is not published (the official repo and every
  public fork stop at 159), so it was built new from public data. Manoj
  chose: the decoded grid first, then the callsign lookup.
- **Grid -> states:** `tools/build_us_grid_states.py` (shapely + pyshp, US
  Census `cb_2023_us_state_500k`) gives `us_grid_states.txt`: 750 squares,
  states with at least 2 % of the square's US land, largest first. AK
  across 180 (RO6x) and HI are in. DC is counted as MD. Spot checks: EM78
  KY IN OH (as 2.2.160's own example), EL95 FL, FN31 CT NY, FN20 PA NJ.
- **Call -> state:** `tools/build_us_call_states.py` distils the FCC
  `l_amat.zip` with DXCA's rules (active HD only, 50 states, DC as MD),
  giving `us_call_states.txt`: 816,130 calls, 7.9 MB, sorted, from the
  2026-10-10 dump. Fetch it with plain curl: data.fcc.gov refuses gzip
  Accept-Encoding.
- **App:** both files are Qt resources (TOP_LEVEL_RESOURCES). A newer
  `us_call_states.txt` in the data directory overrides the built-in one.
  `logbook/usstates.{h,cpp}` (`UsStates::find (call, grid)`) loads on first
  use, keeps the grid table in a hash and binary-searches the call table in
  place. `displaytext.cpp` appends `:ST` to the country when the entity
  prefix is `K`. Setting `ShowUSState` (Settings > General > Display, row 3
  col 2), off by default, enabled only with Show DXCC names.
- **Tested:** `UsStates` in a standalone harness. KK4CDK EL95/none -> FL,
  K1JT FN20 -> PA-NJ (grid first; he is in NJ), K1JT none -> NJ, W1AW/4 EM78 ->
  KY-IN-OH, N0AX RR73 -> MO, `<W6YX>` -> CA, unknown -> none, the first table
  row (AA0A) found. **Not yet seen in the running app's decode window.**
  Trial zip on the Desktop.
- **Refresh:** re-run both scripts before a release (the grid table rarely
  changes) and commit the text files.

### 2026-10-10 — FlexRadio address: "FlexRadio:" label, Discover button, clearer error (released in v0.7.4)

- **Report:** KK4CDK (Miguel, FLEX-6700, Windows) via the vu2cpl.com contact
  form: "Flex: no connection to localhost:4992". He could not find where the
  radio's IP goes. It goes in the CAT box that is labelled "TCI Server:"
  (stored as `CATTCIPort`; he had edited `CATNetworkPort`), and a blank box
  meant localhost. His trace also showed `audio 0`: VITA-49 Audio was not
  ticked. Manoj replies himself; a draft was given in the session.
- **Changes:** for a FlexRadio VITA-49 rig the box is now labelled
  **FlexRadio:** with a `192.168.1.20:4992` placeholder, and a **Discover**
  button beside it (`Configuration::impl::flex_discover`) listens 3 s on
  UDP 4992 (ShareAddress) for the radio's discovery broadcast (28-byte
  header + `key=value` text: model, nickname, callsign, ip, port,
  available/licensed clients), fills the box with every radio heard, picks
  the first and lists them. The connect error now says where to set the
  address.
- **Tested:** the receive-and-parse loop, copied verbatim into a standalone Qt
  program, against Manoj's 6600's real broadcast (captured on ubersdr, which
  is on the radio's subnet, and replayed to localhost) found
  `FLEX-6600 "6600" VU2CPL - 192.168.1.148:4992, 2 of 2 clients free`.
  Manoj ran the trial over the VPN: Discover showed the "No FlexRadio heard"
  dialog, as it should, since broadcasts do not cross a VPN. A Discover that
  finds a radio, run from the radio's own subnet, is still untested in the app.
- **From the same report:** "Show US State" was built the same day; see
  its entry above.

### 2026-10-10 — FlexRadio VITA-49 over a VPN: open the UDP return path (released in v0.7.4)

- **Symptom (Manoj):** from the Mac over a WireGuard VPN into VU2OY's
  network (radio 192.168.220.42, Mac 10.7.159.2 on utun10), the VITA-49
  profile connected and showed the frequency but had no audio and no waterfall.
  It kept dropping too: the 10 s "no audio from the radio" watchdog reconnects.
- **Proved with a read-only probe** (non-GUI client: `client udpport` +
  `sub meter all`, count UDP for 12 s): Manoj's own radio on the LAN sent
  347 meter packets from 192.168.1.148:4993; VU2OY's radio sent **0**. Same probe
  sending `client udp_register handle=0x...` from the UDP socket to the radio's
  4993 first: **316** packets arrived. So VU2OY's network drops radio-initiated
  UDP to VPN clients (probably a stateful firewall; his gateway looks like
  UniFi, judging by the WireGuard address range); TCP control is client-initiated, so it passed.
- **Fix:** `FlexTransceiver::udp_register ()` sends that datagram from the
  audio socket to the radio's 4993 right after `client udpport`, and again
  every 5 s from `do_poll`. The radio ignores it on a plain LAN.
- Builds. Trial zip `~/Desktop/jdxvu/test/JTDX-VU-trial-arm64.zip`.
  **Confirmed working on VU2OY's radio over the VPN** (Manoj, 2026-10-10).
  Re-checked on Manoj's own radio from the Mac mini (another VLAN) before
  v0.7.4: audio and waterfall OK. `tools/flex_selftest.sh`
  needs `tools/ft2_autotest.sh` signals first and was skipped.
- Why Manoj's own site never needed it: on his UniFi the Internal -> VPN
  direction is left at Allow (vlan-setup MIGRATION-PLAN "VPN design"), so the
  radio's unsolicited UDP reaches a VPN client. Only the client's replies
  would be blocked, and VITA-49 needs none. VU2OY's gateway lets traffic
  toward VPN clients through only as a reply. His exact rule is unknown.
- **MSHV has the same gap:** LZ2HV's VITA-49 start-up in `network.cpp`
  sends `client udpport` and never any UDP to the radio. Ported to
  MSHV-Mac `895f76f` and deployed 2026-10-10. Not yet run on a radio. The
  diff for Christo is still open (see the MSHV-Mac HANDOVER).


### 2026-10-09 — README: "Common problems" section (docs only)

- New section after "How to install": the macOS shared-memory error
  (points to install step 5) and duplicate decodes 50/100 Hz apart.
- The duplicates came from Manoj's MacBook on QO-100: RX level meter at
  82 dB, so 50 Hz hum mixed into every strong signal (copies 15-20 dB down
  at ±50/100 Hz multiples). Lowering the RX level to ~30-40 dB removed
  them. Not a decoder issue.
- MSHV hid them anyway: it drops a repeated message at any frequency.
  JTDX shows a repeat >45 Hz away unless **Misc > Hide FT8 dupe messages**
  (`HideFT8Dupes`, per profile) is ticked; all four profiles on the Mac
  mini have it off. README now says so.
- Manoj looked for the option in Settings; it is in the Misc menu (menu
  bar). A README guess that the MacBook notch hid Misc was wrong and has
  been removed; the README now says "in the menu bar, not in Settings".

### 2026-10-09 — QO-100: Club Log alert and JTTY status checked the 3cm downlink, not 13cm (released in v0.7.4)

- **Symptom (Manoj, new MacBook install):** YL2KF on QO-100 FT8 alerted as
  "New band" on 3cm, though the 2024-11-09 QSO is in Club Log as BAND 13CM
  / SAT QO-100. Club Log was on and freshly fetched.
- **Cause:** `ClubLog::check_decode` (called with `m_freqNominal`, the
  10489 MHz downlink) and `jttyHeardStatus` (JTTY band / call status) passed
  the dial straight to `LogBook::matchDXCC` / `matchCall`, so they looked up
  "3cm". The decode window, "New only" filter, auto-seq `wanted ()` and
  logging already used `Radio::qo100_uplink` (13cm).
- **Fix:** both now convert with `Radio::qo100_uplink` first; the alert's
  band label reads 13cm too. **Confirmed on air** by Manoj on the MacBook
  (trial zip `~/Desktop/jdxvu/test/JTDX-VU-trial-arm64.zip`, built from
  `deeb1bbc`): DK1KQ (Club Log 13CM / QO-100, 2024-05-25), which had also
  been alerting as new, no longer does. YL2KF itself not re-heard yet.
- Side note, same session: a new Mac also needs the shared-memory sysctl
  (README step 5) — the MacBook stopped with "Unable to create shared memory
  segment" until it was set.

### 2026-10-09 — RUMlog routing checked; bridge turned on in two profiles (settings only)

MSHV's FLEX VITA49 profile sent JTTY QSOs to RUMlog as mode "JTTY" (its WSJT-X "QSO Logged" message reached RUMlog
before the bridge's MFSK/JTTY ADIF; fixed on the MSHV side). JTDX-VU checked for the same fault: none of its four
profiles sends "QSO Logged" to RUMlog (UDP1 goes to DXCA, 192.168.1.109:2334) and its log holds JTTY as MFSK/JTTY, so
no fault. But the default profile (`JTDX-VU.ini`) and "TCI FLEX HF" had the secondary UDP server (the RUMlog bridge)
OFF, so QSOs logged there reached RUMlog by no path. Turned on in both with JTDX-VU closed (`UDP2Server=127.0.0.1`,
`UDP2ServerPort=2233`, `EnableUDP2adifBroadcast=true`), matching FLEX VITA49 and QO-100. No code change.

### 2026-10-09 — v0.7.3 released: update check, JTTY `%LOG`, Auto CQ button fixes

- https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.7.3, annotated tag on
  `71c7bac8` ("Version 0.7.3": `Versions.cmake` 0.7.2 → 0.7.3; README title
  and title-bar example 0.7.3, "since the next release" → "since v0.7.3",
  `%LOG` marked since v0.7.3). Cut from the MSHV-Mac session on Manoj's OK
  (2026-10-09: next patch version, GitHub release, install everywhere it is
  installed). No Fortran changed since v0.7.2, so no 132-column check needed.
- Contents since v0.7.2: the GitHub release update check (`e9796edb`,
  `0bd95894`), `%LOG` in a JTTY macro (`6fcbe714`), the Auto CQ button's
  fixed width and all-minute countdown (`a00f05c6`, `e64be38d`), the README
  title fix.
- Notes from the v0.7.2 template: what's new (the update check: what it asks
  GitHub, the one anonymous request, the Settings > General tick, Help > Check
  for Updates..., "v0.7.3 is the first version with the check"), the
  "AetherSDR users" paragraph, downloads / install with 0.7.3 names, credits
  unchanged. Release created right after the tag push, so CI attached to it.
- CI macOS 37860936587 (arm64 32 min; Intel 12 min on the restored keg cache) and Windows
  37860936572 (17 min) green. Pi .deb on meridianpi5 (rsync of `git ls-files`
  at `71c7bac8`, `cmake ..`, `nice make -j3`, `cpack -G DEB`): Package
  jtdx-vu, Version 0.7.3, arm64, Conflicts: jtdx, `apt-get install -s` clean,
  an 8 s offscreen `--test-mode` start OK (no jtdxjt9 left; `~/.qttest`,
  `/tmp/JTDX-VU - test*`, the log and shm 32801 removed), uploaded by hand
  with its `.sha256`.
- All eight files downloaded anonymously (200), every checksum OK; both macOS
  apps report 0.7.3 with the right arch and pass `codesign --verify --deep
  --strict`; the arm64 zip has 0 AppleDouble entries; Windows `jtdx.exe`
  carries 0.7.3 and the releases/latest URL. sha256:
  - `JTDX-VU-0.7.3-macos-arm64.zip` `bd02bb6efd6bdb1d18a9b26758b680417584eabd8a04e1c72600b6b18a11be96`
  - `JTDX-VU-0.7.3-macos-x86_64.zip` `db32bb597fc5e4c0511faf56e33cca011087cc4dcb29e0fa3d9cc085fc2eb7ff`
  - `JTDX-VU-0.7.3-windows-x64.zip` `ebb1bc961923aa061230e0feda846a3372dc205f65a61ed30c7c4cb3ae4a04dd`
  - `jtdx-vu-0.7.3-linux-arm64.deb` `43f8f643a6439f76e6fc7d1edad2b16fafd6ec88f1d96bb838681ffa415374c4`
- Installed on the Mac mini from the arm64 release zip (see Current state);
  JTDX-VU was not running, so no quit / relaunch.
- Other hosts: meridianpi5 is a build host only (`dpkg -l jtdx-vu` and `jtdx`:
  none), nothing installed. The .170 Windows PC has a v0.1.0 zip extracted at
  `C:\JTDX-VU-0.1.0-windows-x64` (2026-09-28, its data folder
  `%LOCALAPPDATA%\JTDX-VU` last written the same day, not running) that no
  release since has upgraded; left alone - Manoj decides whether it stays.
- Website (vu2cpl.com card / project page / downloads table) NOT updated in
  this cycle; still on v0.7.2.

### 2026-10-09 — Update check: time saved only on success, hourly re-check, no automatic check on dev builds

Manoj's decisions on `e9796edb` (given in the MSHV-Mac session).
`0bd95894`. No release, version bump, tag or install.

- **Only a success is saved.** Success = HTTP 200 and a JSON object with a
  `tag_name`, newer or not (any other status, 204 included, is a failure).
  `JTDXVU/UpdateLastCheck` is written only then, with the time the request
  was *sent* (whole seconds), by automatic and manual checks alike (a
  manual success also restarts the 24 h - our reading; Manoj only said
  manual *failures* leave the throttle alone). A failure (offline,
  timeout, any HTTP error incl. the 403 rate limit, bad JSON, no tag_name)
  writes nothing, so the next start tries again. A failed *automatic*
  attempt sets an in-memory back-off (`last_failure_` = its send time): no
  automatic attempt for 1 h. Manual failures set nothing. Replaces
  "written when tried" from 10-08.
- **Hourly re-check.** `UpdateCheck::start_automatic (enabled)` (MainWindow
  passes `m_config.check_for_updates ()` as a lambda, asked at every look)
  does the ~10 s look, then starts a repeating 1 h QTimer from that moment,
  so a check due 24 h (or a retry due 1 h) later is due by the tick that
  comes then. `Qt::PreciseTimer`: Qt turns a coarse timer over 20 s into a
  very coarse one, which can fire up to 0.5 s early and would push a due
  check an hour later.
- **The decision is pure:** `UpdateCheck::automatic_check_due (enabled,
  version, busy, window_open, now, last_success, last_failure)` - due only
  when the setting is on, `version` has no "dev" (any case), no check is
  under way, none of the checker's dialogs/messages is open (never a
  second dialog; a "Remind Me Later" dialog left open holds checks off
  until closed), 24 h since the last success and 1 h since a failed
  automatic attempt. An invalid time or one in the future (clock set back)
  does not hold it off.
- **Dev builds:** `jtdxvu_version ()` containing "dev" makes no automatic
  request (start or timer); Help > Check for Updates... still checks. The
  test is on the program's own version, so `JTDXVU_UPDATE_TEST_VERSION`
  still only replaces the version compared and shown (on a dev build with
  the override set, still no automatic request).
- Settings tooltip and README paragraph reworded; README title line and
  title-bar example said v0.6.0, now 0.7.2.
- **Tests** (no app launch): a scratch Qt program from the real
  `updatecheck.cpp` + `revision_utils.cpp`, offscreen, browser open
  intercepted, built twice (0.7.2 and `0.7.3-dev`); the api.github.com
  request rewritten by a QNetworkAccessManager subclass to a fake GitHub on
  127.0.0.1, or sent as is through a refused / mute local proxy (nothing
  left the Mac). 27 decision cases (23:59:59 and 23:59:59.5 not due, 24 h
  due; 59:59 not due, 1 h due; dev/DEV/devel; clock set back; stored ISO
  strings). Automatic: success saves the send time and is silent; refused,
  timeout (10 s), 403 rate limit, 404, 500, bad JSON, no tag_name and 204
  each leave the saved time untouched, are silent, back off 59:59 / retry
  at 1 h; manual: each reported ("HTTP 403 - API rate limit exceeded for
  ...", "Connection refused", "no answer from GitHub within 10 s",
  "unexpected reply from GitHub", "HTTP 204", ...), nothing saved, no
  back-off. Newer release and skipped release count as success; an open
  dialog or message holds automatic looks off; busy = one request. Timer:
  nothing for 9 s, one request at ~10 s, then a repeating 3 600 000 ms
  precise timer; with the interval shortened in the scratch, ticks go
  through the decision (quiet within 24 h, send when due, back off after a
  failure, retry after 1 h, quiet while a dialog is open; setting off = no
  request). Dev build: no request at ~10 s or from ticks, manual works,
  override 0.0.1 shows the dialog from manual only. Release build with the
  override `0.7.3-dev`: automatic still goes. **One live request:** HTTP
  200, time saved, silent (X-RateLimit-Remaining 47). App build 0 errors;
  g++ 15.2 / Qt 5.15.18 on ubersdr `-fsyntax-only -Wall -Wextra -pedantic`
  clean, also gnu++17 and deprecated API off (temp dir removed). Harness:
  scratchpad `jtdxvu-uc-1009/` (session scratch, not kept).
- **Not exercised:** the real app (not launched): the hourly timer and the
  dev skip were tested in the scratch only.

### 2026-10-08 — Update check against GitHub releases (for the next release, v0.7.3)

Manoj: every app of his gets an in-app check against its GitHub releases in
its next release - no Sparkle, no extra servers, the same behaviour
everywhere (spec approved by him; done from the MSHV-Mac session).
`e9796edb`. No release, version bump, tag or install was made.

- New `updatecheck.{h,cpp}` (`UpdateCheck`, added to `wsjtx_CXXSRCS`):
  one anonymous `GET https://api.github.com/repos/vu2cpl/jtdx-vu/releases/latest`
  (`Accept: application/vnd.github+json`, `User-Agent: JTDX-VU/<JTDXVU_VERSION>`,
  10 s timeout by a QTimer + `abort ()`, not Qt 5.15's transfer timeout).
  Reads `tag_name`, `name`, `html_url`, `body`. Its own
  QNetworkAccessManager, as ClubLog: EQSL and WSPRnet `readAll ()` every
  reply on MainWindow's shared one.
- `UpdateCheck::is_newer ()` is the pure comparison: leading v/V dropped,
  split on non-digits, integer tuples, missing = 0.
- **Automatic:** MainWindow fires it 10 s after start if
  `Configuration::check_for_updates ()` (Settings > General > Behavior
  "Check for updates automatically", ini `[Configuration] CheckForUpdates`,
  default true). At most once per 24 h: `JTDXVU/UpdateLastCheck` (UTC ISO
  string) is written when the check is *tried*, so an offline start waits a
  day (superseded 10-09: written only after a success, hourly re-check);
  a time in the future does not hold it off. Silent on any failure, and
  for the tag in `JTDXVU/UpdateSkipVersion`. Both keys are per profile (each
  profile is its own ini), so each profile checks once a day.
- **Manual:** Help > Check for Updates... (before About, `NoRole` so it stays
  in Help on macOS) always reports: the dialog, "You're up to date (JTDX-VU
  x)", or "Couldn't check for updates: <reason>" (HTTP errors show GitHub's
  `message`, first sentence). A manual request during an automatic check
  makes that check report.
- **Dialog:** "JTDX-VU <new> is available — you have <current>.", the
  release name only if it says more than "JTDX-VU v<x>" (ours don't), the
  notes as plain text (Markdown as written) in a scrollable read-only box,
  Skip This Version | Remind Me Later | Download. Non-modal; no default
  button (a QDialogButtonBox made Download the default when shown, so Return
  would have opened the browser - plain buttons instead); Esc = Remind Me
  Later; the automatic one opens with `WA_ShowWithoutActivating` so JTTY
  typing and Halt Tx keep the keyboard. Download opens `html_url` only if
  it is https://github.com, else the releases/latest page.
- **Test hook:** `JTDXVU_UPDATE_TEST_VERSION=0.0.1` (inert unless set)
  replaces the version compared and shown; the User-Agent keeps the real one.
- README: a "Tells you when a new release is out" item.
- **Tests** (no app launch; a scratch Qt program built from the real
  `updatecheck.cpp` + `revision_utils.cpp`, `QT_QPA_PLATFORM=offscreen`, the
  browser open intercepted by `QDesktopServices::setUrlHandler`):
  19 comparison cases pass (0.7.10 > 0.7.2, v0.7.2 = 0.7.2, 1.0 = 1.0.0, ...).
  Live: request seen as above, HTTP 200, **tag_name `v0.7.2`**, name
  "JTDX-VU v0.7.2", 6977-char body; as 0.7.2 manual says up to date and
  automatic says nothing, records UpdateLastCheck, a second automatic sends
  nothing. As 0.0.1: dialog with the right heading, non-modal; Download
  opened `https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.7.2`; Later and
  Esc store nothing; Skip stored `v0.7.2`, after which automatic was quiet
  and manual still showed it. Through a proxy on a closed / a mute local
  port (nothing left the Mac): manual "Connection refused" and "no answer
  from GitHub within 10 s" (10.2 s), automatic silent. Rendered offscreen
  to a PNG to check the layout. g++ 15.2 / Qt 5.15.18 on ubersdr:
  `-fsyntax-only -Wall -Wextra -pedantic` clean, also with deprecated Qt
  API disabled (temp dir removed). App build 0 errors.
- **Not exercised:** the HTTP 4xx branch (needs GitHub to refuse, e.g. the
  60/h anonymous rate limit) and the bad-JSON branch; the menu item, the
  Settings checkbox and the 10 s timer were checked in the build (strings,
  `ui_Configuration.h`), not in a running app.

### 2026-10-08 — JTTY: `%LOG` in a macro logs the QSO without the dialog (for the next release, v0.7.3)

Manoj (in the MSHV-Mac session, for MSHV's JTTY, `6adc6e3` there): "can you
add a log qso in macro to enable auto logging?", then "add in jtdx vu also".

- `mainwindow_jtty.cpp` `jttyMacro ()`: a macro containing `%LOG` (also via
  the set's exchange) needs a DX call, has the variable cut out, sends the
  rest (nothing for a macro of just `%LOG`), then sets `m_jttyAutoLog` and
  calls `on_logQSOButton_clicked ()`, which passes autologging to
  `LogQSO::initLogQSO`, so `accept ()` runs without showing the dialog — the
  same path as Settings' auto log; the log file, `acceptQSO2`, the UDP
  "QSO Logged" / ADIF sends and the RUMlog bridge all apply as for a manual
  log. The flag is read and cleared first thing in the button handler, so it
  never leaks to a later click.
- Once per QSO: `acceptQSO2` records `m_jttyLoggedCall` in JTTY, and
  `on_dxCallEntry_textChanged` clears it when the DX call changes.
  `jttyAlreadyLogged ()` makes a repeat `%LOG` say so in the status bar, and
  the WSJT-X "TU ..." rule (serial bump + log dialog) now skips the dialog for
  a QSO already logged — also after a manual Log QSO. The Log QSO button
  itself always opens the dialog.
- Settings > JTTY's variable list and README's JTTY section describe `%LOG`.
- **Test** (`--test-mode`: the settings live in
  `~/.qttest/Library/Preferences/JTDX-VU - test.ini`, NOT
  `~/Library/Preferences/`; seeded from the FLEX VITA49 profile with rig
  None, VOX, "RX Audio" in/out, [Alerts] and [ClubLog] removed, PSK
  Reporter / secondary UDP / TCP / eQSL off, primary UDP to a local
  listener; `jtdxjt9` copied into the build's `Contents/MacOS` and removed
  after): F8 `TU %H 73 %LOG` → one "QSO Logged" to the listener and in
  `wsjtx_log.adi`, no dialog, TX "TU … 73"; F8 again → transmitted, no second
  log; F7 `%LOG` → "already in the log". Test files to the Trash. **First
  attempt ran on a blank default test profile** (seed written to the wrong
  folder): jtdxjt9 exited with code 2 behind a modal "Subprocess Error",
  which also blocked every menu click — quit after closing it. Test tools
  kept in `~/projects/MSHV-Mac/tools/`: `wsjtx_udp_listen.py`, `ax_walk.js`,
  `ax_act.js` (JTDX's widgets are nested, so plain System Events finds
  none). **Installed
  22:41** (`./macos-bundle.sh`, previous copy to the Trash, not launched).

### 2026-10-08 — Auto CQ button: keeps its width, and counts down in the first half of the minute too (for the next release, v0.7.3)

Manoj: "width of autocq button should not change". In v0.7.2 the button
was sized to its text (AnsB4's Preferred policy, max 80 px against Monitor's
68-75 px), so "Auto CQ 0:58" widened it and the right-hand column.

- `mainwindow.cpp`: the button's size policy is Ignored both ways, so its
  size never follows the text; min/max width = Monitor's, min height = two
  text lines (capped at the old 45 px max). `updateCnsCountdown ()` puts the
  count on a second line ("Auto CQ" / "4:37", "12m" from 10 min up).
- Tested off-screen (Qt 5 offscreen platform, scratch program): in a column
  beside an expanding area, the button's geometry was identical for "Auto
  CQ", "Auto CQ\n0:58", "Auto CQ\n12m" at two window heights. Then in the
  real window (`--test-mode` copy of the FLEX VITA49 settings with rig None,
  VOX, audio on the unused "RX Audio" loopback - test-mode settings live in
  `~/.qttest/Library/Preferences/`, not `~/Library/Preferences/`): the
  button stayed Monitor's width through the countdown.
- **Countdown stuck at 5:00 / whole minutes (Manoj), fixed:** the button
  predicted the next minute with `watchdog_counts (next tick)`, which uses
  `m_secTxStopped` - the end of the PREVIOUS transmission. In the first half
  of each minute that is > 32 s before the tick, so it showed the whole
  minute (5:00 on the first one) and only ran in the second half. Seen on
  the test copy calling CQ: 07:02:01 4:00, 07:02:26 4:00 (should be 3:35),
  07:02:56 3:04, 07:03:26 3:00. Now the coming minute counts if Enable Tx is
  on or a transmission is under way (it ends before the tick), else the old
  test. After: 07:06:12 4:48, :32 4:28, :52 4:08, 07:07:12 3:48, :32 3:28,
  :52 3:08. The watchdog itself was always right - only the display lagged.
  Note: like stock JTDX, any click, key press or window activation in
  JTDX-VU resets the watchdog, so the count goes back to 5:00 then.
- Installed on the Mac mini 2026-10-08 (build of `e64be38d`, labelled 0.7.2,
  replacing the v0.7.2 release copy, now in the Trash); trial zip in
  `~/Desktop/jdxvu/test/`. Relaunched on the FLEX VITA49 profile.
- README CNS paragraph updated.

### 2026-10-08 — Auto CQ button counts down what is left (for the next release, v0.7.2)

Manoj asked what the Auto CQ "watchdog" is and wanted a countdown on the
button (the same change went into MSHV-Mac's private JTTY, `9fa653d` there).
Done in the MSHV-Mac session, on a worktree branch, rebased onto `31c422e9`.

- `MainWindow::updateCnsCountdown ()`, called from `updateCnsButton ()` and
  from guiUpdate's once-a-second block: while Auto CQ is on the button reads
  "Auto CQ m:ss", or "Auto CQ 12m" from 10 minutes up (the button is at most
  80 px wide; "Auto CQ 0:58" fills it).
  - JTTY: the Auto CQ time limit from `m_jttyAutoCqStartMs`. It is checked
    before each call, so at 0:00 the CQ on the air still ends.
  - FT modes, CNS on (not Hound, not WSPR): the Tx watchdog
    (`watchdog_minutes ()` - `m_idleMinutes`). It counts whole minutes on
    `minuteTimer`, only while transmitting, so the countdown runs while
    calling and holds at the full minutes while Tx is idle. The minute
    timer's test moved unchanged into `watchdog_counts (qint64 sec)`, so the
    button asks the watchdog's own question; the seconds come from
    `minuteTimer.remainingTime ()`, so the two step together when it drifts.
    0:00 once `m_txwatchdog` has tripped.
- README: CNS and JTTY Auto CQ paragraphs mention the countdown.
- Tested on a `--test-mode` instance (rig None, audio on the unused "RX
  Audio" loopback): JTTY 1:00 -> 0:03 then off; a 15-minute limit shows
  "Auto CQ 15m" and fits; FT8 with CNS idle holds at 2:00 (and at 14m after
  a JTTY transmission had used a minute), the WD box showing the same.
  **Not seen: an FT8 CQ run counting down in m:ss** - Enable Tx could not be
  switched on through accessibility in the test instance. Watch it on air.
- **Test-instance trap, hit doing this:** a plain launch of a build with
  `HOME=<scratch>` is NOT isolated - Qt's ConfigLocation on macOS ignores
  `$HOME`, so it read `Profiles/Last` from the real `JTDX-VU.ini` and ran
  the real **FLEX VITA49** profile (it connected to the radio for ~30 s - its `flex_trace.txt` shows `client gui`, DAX streams, `slice tune 0 14.090000`, DIGU and no `xmit`;
  no slice left behind - checked with
  `MSHV-Mac/tools/flex_clients_slices.py`). Its lasting effect: the
  one-time default-row seeding ran on that profile and wrote
  `JTDXVU/FrequencySeed=1`, but the instance was stopped with SIGTERM
  before the frequency list was saved, so the 432.170 / 1296.170 / 40.680
  rows are NOT in `JTDX-VU - FLEX VITA49.ini` (checked 10:23 by decoding
  `FrequenciesForModes`). Left so, v0.7.2 would skip that profile:
  **remove the `FrequencySeed=1` line from it, with JTDX-VU closed** -
  done 2026-10-08 ~10:40 on Manoj's word (only that line changed; checked
  against a pre-edit copy). Stop a test instance cleanly (Quit / AppleScript `quit`),
  never SIGTERM - that loses unsaved settings and orphans `jtdxjt9`
  (it held SysV shm until cleaned up). Isolate a test run with **`--test-mode`**:
  it skips `Profiles/Last` and uses profile "test"
  (`~/Library/Preferences/JTDX-VU - test.ini`, data in
  `~/Library/Application Support/JTDX-VU - test`), which can be seeded
  first and deleted after. A build-tree `jtdx.app` also needs `jtdxjt9`
  copied into `Contents/MacOS`, or it quits at the subprocess error.

### 2026-10-08 — FT4 on 70cm / 23cm, 8m button, one-time seeding of new default rows (for the next release, v0.7.2)

Frank PH2M (2026-10-08 mail, with IC-7300 / IC-9700 screenshots of the band
buttons) gave the FT4 spots he uses on UHF/SHF and asked for an 8m button.
His "missing worked-before stations" were the Hide-worked button.

- `FrequencyList.cpp`: FT4 432.170 and 1296.170 MHz in the default list.
- `bandmodeswitcher.cpp`: "8m" in the band-button list between 10m and 6m,
  off by default (Bands.cpp already had 8m, and the default list FT8
  40.680 MHz for Region 1).
- New lists get these, but a saved list never sees later defaults and the
  band button then falls back to the band's FT8 row. So
  `FrequencyList_v2::add_default_frequencies` /
  `Configuration::seed_default_frequencies (QList<Frequency>)` add the
  shipped rows on given frequencies that the list lacks, and MainWindow
  calls it once at start for {432.170, 1296.170, 40.680} MHz, guarded by
  `JTDXVU/FrequencySeed` = 1 (a row deleted later stays deleted). For the
  next such batch, bump the seed number and add the frequencies.
- Tested: a `-r seedtest` copy of the FLEX VITA49 profile (rig set to None)
  whose list had none of the three rows; after one start the decoded
  `FrequenciesForModes` held each once and `FrequencySeed=1`; a second start
  added nothing. Real JTDX-VU.ini and FLEX VITA49 ini byte-identical
  afterwards; test profile files deleted.
- Reply to Frank SENT 2026-10-08 (Gmail, "Re: Technical / Node-RED discussion"):
  `~/projects/JTDX-records/jtdx-vu-v0.7.2-reply-to-PH2M.txt` - promises both
  in v0.7.2 and that he'll hear when it's out.

### 2026-10-08 — v0.7.2 released: FT4 on 70cm / 23cm, 8m button, Auto CQ countdown

- https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.7.2, tag on `0edfa30a`
  ("Version 0.7.2": `Versions.cmake` 0.7.1 → 0.7.2, README "since the next
  release after v0.7.1" → "since v0.7.2", countdown lines marked since v0.7.2).
  Cut from the MSHV-Mac session on Manoj's "cut a new release. both JTDX and
  MSHV", which lifted the morning's hold; a relay of that to this session
  expired undelivered (different permission modes), so nothing ran twice.
- Contents since v0.7.1: FT4 on 432.170 / 1296.170 MHz, the 8m button (off by
  default), one-time seeding of those rows into existing lists
  (`JTDXVU/FrequencySeed`), the Auto CQ countdown (`3d5b9b5d`). No Fortran
  changed since v0.7.1, so the 132-column check was not needed.
- Notes from the v0.7.1 template: what's new, an "AetherSDR users" paragraph
  (#6006 is fixed in AetherSDR 26.10.1 — PR #6116 is in that tag — replacing
  the old "known issue"), downloads / install / credits with 0.7.2 names.
  Release created right after the tag push, so CI attached to it.
- CI macOS 37731634422 (28 min) and Windows 37731634426 (17 min) green. Pi
  .deb on meridianpi5 (rsync of `git ls-files` at `0edfa30a`, `cmake ..`,
  `nice make -j3`, `cpack -G DEB`): Package jtdx-vu, Version 0.7.2, arm64,
  Conflicts: jtdx, `apt-get install -s` clean, an 8 s offscreen `--test-mode`
  start OK, sha256 `13276d1caf28b5d8…`, uploaded by hand. The headless run's
  leftovers on the Pi (`~/.qttest`, `/tmp/JTDX-VU - test*`, shm 196662) were
  removed; no jtdxjt9 orphan. (`timeout` SIGTERMs the app: check `pgrep -x
  jtdxjt9` after, and note `pgrep -f` inside `ssh '…'` matches its own shell.)
- All eight files downloaded anonymously (200), every checksum OK; both macOS
  apps report 0.7.2 with the right arch, carry `updateCnsCountdown` /
  `watchdog_counts`, and pass `codesign --verify --deep --strict`; Windows
  `jtdx.exe` carries 0.7.2.
- Installed on the Mac mini from the arm64 release zip (`ditto`), replacing
  v0.7.1 (now in the Trash as "JTDX-VU 0.7.1 (replaced 2026-10-08 by
  v0.7.2).app"); executable identical to the release; not relaunched (it
  would open the FLEX VITA49 profile and connect to the radio — Manoj's call).
- Website: paragraph "FT4 on 70cm and 23cm, an 8m button and an Auto CQ
  countdown (v0.7.2)", downloads table / release link on v0.7.2, card sentence,
  a "Tested so far" line for the countdown; pushed (`c522a65`) after all eight
  URLs answered 200.
- Frank PH2M: "it's out" note
  (`~/projects/JTDX-records/jtdx-vu-v0.7.2-released-to-PH2M.txt`) SENT by Manoj
  the same day. Manoj will test the FT8 countdown on air later.

### 2026-10-08 — v0.7.1 released: About box credits (FT2 corrected, JTTY)

- https://github.com/vu2cpl/jtdx-vu/releases/tag/v0.7.1, tag on `079026e7`
  (version bump only on top of the About box / credit commits). No other
  program change from v0.7.0.
- CI macOS 37716346363 (29 min) and Windows 37716346348 (14 min) green; Pi
  .deb on meridianpi5 (`nice make -j3`, `cpack -G DEB`): Version 0.7.1,
  Conflicts: jtdx, `apt-get install -s` clean, uploaded by hand. All eight
  files downloaded; checksums OK; each binary reports 0.7.1 and carries the
  new credit; arm64 signature verifies. Copies in `~/Desktop/jdxvu/v0.7.1/`.
- Installed on the Mac mini from the release zip (executable identical to
  the release), replacing the trial build; relaunched on the FLEX VITA49
  profile.
- Manoj replied to Martino IU8LMC with links to the GitHub credits and
  About box screenshots.
- mshv-macos-port release notes (public mac11 + mac1, private mac11-priv)
  got the same FT2 credit sentence — see the MSHV-Mac HANDOVER.

### 2026-10-08 — FT2 credit corrected; About box credits JTTY and FT2 (for the next release)

Martino Merola IU8LMC wrote on behalf of the "Decodium / DecoSystem Team"
(2026-10-08, Gmail, subject "Omission of FT2 Developer Credits") asking
that the team be credited as FT2's developers. Manoj replied the same day
(credit had been to Martino personally; asked for team names).

Facts (from ft2.it): FT2 was created by Martino Merola IU8LMC (ARI
Caserta) with Salvatore Raccampo 9H1SR as lead C++ developer; first on
air in Decodium on 2026-02-16; MSHV added it in 2.76.5 on 2026-03-24.
So the old README phrase "as introduced in MSHV" was wrong. Code credit
was already right: JTDX-VU has no Decodium code, its FT2 is JTDX's FT4
chain scaled, with MSHV (LZ2HV) as the reference.

- `about.cpp`: new "JTDX-VU additions" block crediting JTTY (K1JT, K9AN,
  G4KLA, WSJT-X group), FT2 (wording above) and the MSHV-derived UI
  features. The About box previously credited neither mode. Builds clean.
- About box tidy (same day): dialog titled "About JTDX-VU", the additions
  under a bold "JTDX-VU additions" heading, licence line reads "JTDX and
  JTDX-VU are licensed under the terms of version 3 of the GNU General
  Public License (GPL)". Trial build with it: `~/Desktop/jdxvu/test/JTDX-VU-trial-arm64.zip`.
  Installed to /Applications on 2026-10-08 (replacing the v0.7.0 release
  copy, which went to the Trash); About box checked on screen.
- `README.md` FT2 section: same wording, "as introduced in MSHV" removed.
- Same wording on vu2cpl.com (card + JTDX-VU project page) and in the
  mshv-macos-port README (`~/projects/mshv-public-release`).
- v0.7.0 GitHub release notes: credits line updated to the same wording
  (edited in place, 2026-10-08).

### 2026-10-07 — Rig mode shown beside the frequency (for the next release)

Manoj: "what about a mode indicator?" ... "you can reuse the round green
indicator left of frequency?". JTDX-VU never showed the rig's mode, for any
rig. After a too-small pill and two bigger ones he disliked ("no bold",
"looks bad"), five mock-ups were shown. He picked option A (grey box, status
in the text colour), but in dark mode `Radio::convert_dark` darkens every
colour by 0x60 per channel, which made the green text nearly black on the dark
box ("cant see in dark mode"), and he switched to **option C**: the round
`readFreq` button is now a box (72 x 36 px, 1 px grey border, 3 px corners)
with the status in the **background** - the round light's colours: green =
rig OK and in the mode Settings > Radio asks for (Data/Pkt = DIGU, USB = USB,
or any when Settings leaves the mode alone), **yellow** = rig OK in another
mode, orange = connecting (dots), red = failure (ERR, still click to reset) -
and the mode the rig reports (DIGU, USB, LSB, CW, RTTY...; RTTY is the widest
it needs to fit) in the dial frequency's font declaration (`MS Shell Dlg 2`
18 pt; macOS draws both in the system font), normal weight. Background and
text both go through `convert_dark`, as the round light always did, so dark
mode shows light text on darkened green. Split is in the tooltip ("S" only
when no mode is reported). One helper, `MainWindow::set_rig_status_style`,
replaces the old `styleSheet().left(230)` cut in all three places;
`update_rig_mode_indicator` picks text and colour; new
`Configuration::data_mode()`. Checked on the fake Flex in light and dark style
(DIGU on green beside 7.074); yellow / orange / red by logic only.

### 2026-10-07 — FlexRadio panel, TX slider = RF power, start on the last frequency (for the next release)

Manoj after the band-change fix: "jtdx doesnt show mode - its needed. flex
panel is missing?" and "we can reuse the pwr slider which is already there?
can have panel setting also".
- **View > FlexRadio Panel...** (`flexpanel.{h,cpp}`), port of MSHV-Mac's
  Flex panel: radio meters (forward / reflected power, SWR coloured under
  load, ALC, PA temperature, supply), the slice's RX / TX antenna and **mode**
  (the radio's own lists), transmit power (RF, tune, max, hardware ALC; sent
  only when the operator edits a box), the antenna tuner (status, Tune,
  Bypass, Memories; hidden on a radio without one; public in MSHV since
  2026-10-05) and the M-series front-speaker mute (greyed otherwise). Per-band
  antenna memory for transverter bands as in MSHV (MM0CEZ), by JTDX-VU band
  name, saved as `[FlexPanel] BandAntennas` in the profile. Made at start-up
  and kept, so the memory works with the window closed; reopened at start if
  it was open at quit (`FlexPanel/Visible`).
- **Plumbing:** `flexshared.h` - one mutex-guarded status struct + command
  queue between the rig thread and the panel. `FlexTransceiver` adds `sub
  meter all` / `sub atu all`, parses the slice / transmit / atu / radio /
  meter-definition lines and the UDP meter packets (MSHV's unit scaling),
  drains the panel's commands every 100 ms, and shows a change the radio
  answers OK at once (the 6600 does not echo every slice change). The slice's
  full status, which arrives with `client gui` before the slice is chosen, is
  replayed once it is - the panel's lists were empty without that.
- **JTDX-VU's own meters:** with Settings > Radio "S meter" / "Output power"
  ticked, the slice's LEVEL meter drives the S-meter (dBm + 73) and FWDPWR /
  SWR the TX power / SWR labels, as with TCI.
- **TX slider = the radio's RF power**, as it is the SDR program's drive with
  TCI: the slider follows `rfpower` (per band on a Flex), only a slider move
  sends `transmit set rfpower=`, nothing at start-up or before the radio has
  reported; while tuning it is `tunepower`. DAX audio now always goes at full
  scale (it was attenuated by the slider before).
- **Start on the last frequency** (`MainWindow::handle_transceiver_update`):
  every Flex connection is handed the radio's default slice (14.100 USB) and
  JTDX-VU followed it; on each offline -> online transition with a Flex rig
  it now `force_rig_mode`s the last monitored frequency, which also sets the
  data mode (MSHV's "start on the last frequency").
- **Size:** MSHV's single column, compact (Manoj: "make it smaller like mshv
  ... narrow and tall, but smaller size"): font 2 pt under the application
  font, tight margins and spacing, narrower value boxes - 192 x 537 px, under
  the main window's height (was 220 x 760). Size and place are remembered
  (`FlexPanel/Geometry`). A two-column version (363 x 390) was rejected.
- **Found while testing: Return pressed the tuner's Tune button.** In a
  QDialog the first push button is the default, so Enter in a power box - which
  the tooltip says commits the value - started `atu start`, i.e. transmitted.
  Tune / Bypass are now never default buttons. **MSHV-Mac's panel is built the
  same way and very likely has the same hazard** - check it there.
- **Tested against `tools/fake_flex.py`** (now with transmit / atu / meter
  status, a meter stream and the 6600's refusal of `mixer front_speaker`):
  panel shows meters, ANT1 / ANT1 / DIGU, 50 / 10 / 100 %, tuner Bypass,
  FLEX-6600, mute greyed; a panel antenna change reaches the radio; start-up
  from a 40m profile tunes the 14.100 slice to 7.074 DIGU. **Not verified
  (UI automation could not drive them):** the RF power box and the TX slider
  sending `rfpower`, and the slider following a panel change - to check on
  the radio.

### 2026-10-07 — FlexRadio VITA-49: first on-air run on the FLEX-6600, band change fixed

Manoj tried the VITA-49 rig on the 6600 ("changing bands, crashing" ->
"its hanging actually" -> "bands not switching"). Connect, receive and the
DAX streams worked first time. A band change did not.
- **What happened** (trace + 1 s stack samples): `slice tune 0 21.074000`
  was answered `R|0` and the radio did change band (its TX interlock moved to
  15m), but no slice status line with the new `RF_frequency` reached
  JTDX-VU. `do_frequency` waited 2 s for one, gave up with "failed to set
  frequency", the next poll took the rig offline, and the TCI-style
  reconnect began. Each reconnect is a new GUI client, which the 6600 hands a
  fresh slice at its default **14.100 USB**; the frequency JTDX-VU asked for
  during the reconnect was only noted (the session was not ready) and never
  sent, so JTDX-VU followed the radio back to 14.100. Hence "hang" and
  "bands not switching". Same at the very first start: the remembered 10.136
  was never sent.
- **Fix (`FlexTransceiver.cpp`):** a tune or mode change counts as done when
  the radio answers it OK, as MSHV does (MSHV never waits for the status);
  a refusal is traced, not fatal. The slice status still updates the
  frequency / mode whenever the radio reports them, so a retune in SmartSDR
  is followed. Anything asked for while connecting, or while another command
  is in progress, is kept and sent as soon as possible (the startup frequency
  included). Every wait now runs its own event loop - the shared one could
  not nest, and a band change sends frequency, mode and more back to back.
- **Trace:** every connection appends to `flex_trace.txt` in the profile's
  data folder (`~/Library/Application Support/JTDX-VU - <profile>/`):
  each command, reply, status line (first 300 chars), what the radio says
  the slice frequency / mode is, and any unconfirmed command. Capped at 16 MB.
- **Fake radio:** `tools/fake_flex.py` gains `FAKE_GUI_SLICE=1` (a new GUI
  client gets a 14.100 USB slice, as the 6600 does), `FAKE_NO_ECHO=1` (tune /
  mode answered OK but never reported), `FAKE_TUNE_DELAY`, `FAKE_BAND_MODE`.
  With `FAKE_NO_ECHO` the previous build reconnected after a 15m change, as
  on the radio; the fixed one stays on one session through 40/20/15/10/20m,
  the GUI never stalls, and receive / transmit self-tests still pass.
- **GUI harnesses (committed with the clear-session sweep):**
  `tools/flex_bandtest.sh` clicks band buttons on a `-r flextest` instance
  against the fake radio (via accessibility, addressed by that instance's
  PID so a running JTDX-VU is untouched) and times the replies;
  `tools/flex_paneltest.sh` screenshots the FlexRadio panel and the mode
  indicator in RX/TX; `tools/macos-window-list.swift <pid>` lists a PID's
  window IDs for `screencapture -l`. Both scripts need the FT2 periods in
  `/tmp/ft2_autotest-sig` (from `ft2_autotest.sh`).
- Gotcha: Manoj's Flex profile was copied from the QO-100 one and kept
  *QO-100 rig IF = 28.54*; on an HF Flex that maps 10m readbacks to QO-100.
  Set it to 0 in any non-QO-100 profile.
- **Still to do on the radio:** band changes with this build, then the TX
  checks in the open item (RF level, unkey, a QSO).

### 2026-10-07 — FT2 AP7: two porting bugs fixed, most of the gap to MSHV closed (for the next release)

Manoj: "fix the ap7 gap". The reference is MSHV's `DecoderFt2` (`decoderft2.cpp`,
`ft2_a7_save` / `ft2_a7d` / the roll at the top of the decode), not the FT4 one
read first. The matcher, the sync search and the 1.27 / 100 / 95 gates were
already the same. Two things were not:
- **History thrown away after any quiet spell** (`a7_roll`). The 11 s staleness
  test measured from the last period WITH decodes; MSHV measures from the
  previous period, decoded or not. Two silent periods between QSOs plus one
  missed decode wiped the pairs, so the first decode after a gap never got its
  AP7 retry. Now as MSHV: reset only after an 11 s gap in the decoder clock or
  three consecutive periods without a decode.
- **Quality gate too strict.** MSHV's AP7 quality is `1-(nharderrors+dmin)/60`,
  but `ft2_a7d` never hands `dmin` back to its caller (an unnamed `double &`
  parameter), so in practice it is `1-nharderrors/60 >= 0.02`, i.e. at most 58
  hard errors. The port used the real dmin (about 34 at -16 dB), which rejected
  most true decodes: with the pair supplied, the true message ranked first 33/40
  times at -16 dB and was accepted 7/40. The gate now uses hard errors only and
  sits in the caller; a decode that fails it is not shown but still goes into
  the history, as MSHV's `PrintMsg` does.
- **Offline (40 QSOs decoded back to back, `lib/ft2/tools/bench/a7bench.sh`):**
  AP7 decodes at -15 dB 9 -> 41, at -14 dB 20 -> 24; plain decodes unchanged.
  `a7false.sh` (new): 200 sequences of silence after a pair, and 200 of a
  different station answering K1ABC at -16 dB on the same frequency - no false
  AP7 decode in either.
- **Live, same audio as MSHV (`tools/ft2_autotest.sh`, 20 QSOs per level):**

  | SNR | JTDX-VU before | JTDX-VU now (AP7) | MSHV (AP7) | of |
  |---|---|---|---|---|
  | -14 | 102 | 106 (13) | 107 (10) | 120 |
  | -15 | 25 | 47 (14) | 61 (28) | 120 |
  | -16 | 3 | 8 (2) | 11 (5) | 120 |

  `lib/ft2/tools/bench/autotest/2026-10-07-sweep-20qso-ap7fix.txt`. No false
  AP7 decode from JTDX-VU; MSHV made one (`K1ABC W9XYZ +45`). MSHV's own -15
  dB total was 66, 43 and 61 in three runs of the same plan, so the rest of the
  gap is within run-to-run variation plus which messages each plain decoder
  happens to catch (that decides whether the history survives the gap between
  QSOs). Note the sim uses the same two calls in every QSO, which flatters
  history carried across QSOs for both decoders.
- **Tools:** `ft2dec` honours `A7DEBUG=1` (every AP7 attempt: sync, sync
  quality, best/second distance, hard errors, on stderr) and `A7QUAL=` (the
  gate); `a7bench.sh` runs sequences back to back by default
  (`A7SEPARATE=1` for the old one-at-a-time). The generated sets are
  git-ignored.

### 2026-10-07 — JTTY: crash on some false decodes fixed (for the next release)

Found while porting JTTY to MSHV-Mac (`tools/jtty-port` there, local
only). `unpack_jtty_atom` (`lib/jtty/jtty_source_codec.f90`) reads
callsigns with the old `unpack28` in `lib/ft8v2/packjt77sd.f90`, which
has no hash table. For n28 in 532444..6257895 (unused tokens and 22-bit
hashes) it went on to the standard-call branch with a negative n and
read outside its c1..c4 strings. A `-fcheck` build aborts ("Substring
out of bounds: lower bound (-26)"); a normal build returns a junk call
marked valid. About 0.8 % of false JTTY decodes land there.
- **Fix (`face6cf`):** `unpack28` in packjt77sd now returns `QU1RK` with
  `success=.false.` for both ranges, and JTTY's existing success check
  drops them. The vendored JTTY files are unchanged. This module's
  `pack28` never makes those values, so FT8 self-decode (`genft8sd`, the
  only other user) is unaffected.
- **Checked in a cloud session (Linux, gfortran `-fcheck=all`):** every
  n28 from 0 to 2^28-1, old against new. 5,725,452 values were rejected
  cleanly; every other value gave the same call and flag as before.
- **Checked on the Mac 2026-10-07** (corpus and `ref_unpack.f90` from
  MSHV-Mac `tools/jtty-port`; `packjt77sd.f90`, `jtty_source_codec.f90`
  and `jtty_mod.f90` recompiled with `-fcheck=all` and linked ahead of
  `libwsjt_fort.a`, so an out-of-bounds read aborts):
  - **Build:** `make jtdx rjtty` clean.
  - **Negative control:** the same checked build from the pre-fix
    `packjt77sd.f90` aborts on the first unhandled frame at its line 308,
    "lower bound (-26) of 'c2'" - so the harness does catch the bug.
  - **`frames_unhandled_n28.txt`:** all 1466 frames come back invalid
    (empty text, valid=F), exit 0, no runtime error.
  - **`frames_fuzz_nohash.txt`:** 198,534 frames, output byte-identical
    to the old version (checked build and the shipped old `ref_unpack`
    binary alike); 103,299 valid, 95,235 invalid.
  - **JTTY wav replay:** 70 recordings (s000-s039 plus 30 "sticky" ones)
    through `rjtty`: shipped build, fixed checked build and pre-fix
    checked build give byte-identical output (266 decode lines), no
    runtime errors. The replay set does not reach the hash range, so it
    shows the fix changes nothing on real decodes rather than exercising
    it - the corpus checks above do that.
  - Gotcha: `rjtty` keeps file names in `character*80`; give it relative
    names from inside the wav directory or long paths are cut off.

### 2026-10-07 — FlexRadio VITA-49 rig type, ported from MSHV-Mac (for the next release)

Manoj: "Next vita49 to jtdx Vu ... Use MSHV repo ... For reference". The
MSHV-Mac native Flex backend (`MSHV/src/HvRigControl/HvRigCat/network/
network.cpp`, "flex native vita-49", his own code in LZ2HV's layout) is the
reference; this is the same thing in JTDX-VU's Transceiver shape.
- **`FlexTransceiver.{hpp,cpp}`** - a `PollingTransceiver` like TCI:
  SmartSDR control on TCP (host[:port], default 4992, from the TCI port
  field) and DAX audio over VITA-49 UDP, one API session. Start-up as
  measured on the 6600 in MSHV-Mac: V/H greeting, `client gui`, subs,
  `client udpport <bound port>`, `info`, 1.2 s slice-status drain, slice =
  ours-by-letter / ours / the global index / `slice create mode=digu
  freq=<last>`, `dax audio set <ch> slice=<n>`, `stream create type=dax_rx
  dax_channel=<ch>` (id from the reply body or the status line), and with the
  audio tick `stream create type=dax_tx` + `transmit set dax=1` + `dax audio
  set ... tx=1`. DAX channel = slice letter + 1. Tune `slice tune n MHz
  autopan=1`, mode `slice set n mode=`, PTT `dax audio set ch tx=1` /
  `slice set n tx=1` / `xmit 1`, audio keyed just before and stopped on
  unkey. Tear-down removes only what it created. Nested waits with the same
  `in_nested_wait` / `abort_waits` hooks as TCI, so the deferral fixes apply.
- **Audio:** RX packets (class 0x534C03E3, 24 kHz stereo float32 BE, L==R)
  -> mono -> each sample doubled into 48 kHz stereo frames -> the TCI
  `writeAudioData` / `fil4_` decimation (its low-pass removes the doubling
  images) -> `dec_data`, `tciframeswritten`. TX: a 4 ms precise timer sends
  whole 128-frame packets against the clock (type 1, class 0x534C0123, 284
  bytes, mono int16 BE) from the TCI modulator copy, taking L of every
  second 48 kHz frame; the TX slider is a gain on that audio (0 dB at the
  top), RF power is left to the radio. 10 s audio watchdog -> failure ->
  the main window's reconnect loop (`is_tci()` is true for a Flex too).
- **Integration:** `TransceiverFactory` ids `FlexFirstId..FlexLastId`
  (slices A..H), `Capabilities::tci` so the host:port field and CAT PTT
  apply; `Configuration` treats "FlexRadio VITA" like "TCI Cli" (audio
  tick, PTT via CAT only); the tick is now labelled "Use TCI / VITA-49
  Audio". The modulator and RX decimation are copied from TCITransceiver,
  not shared - a refactor there right before a release was not worth it.
- **Not supported:** split (a Flex has one VFO per slice; `do_tx_frequency`
  reports split off - use Fake It or None), S-meter / power / SWR meters,
  the Flex panel extras (antenna, ATU, front speaker) MSHV has.
- **Tested against `tools/fake_flex.py`** (a stand-in radio: the line
  protocol above, DAX RX from the same noise + scripted QSO as the TCI sim,
  DAX TX packets kept as 12 kHz wavs) with `tools/flex_selftest.sh`: the
  command sequence came out exactly as MSHV's, all six QSO messages decoded
  at DT 0.0, and a CQ triggered over UDP was transmitted as DAX packets that
  `ft2dec` decoded back ("CQ VU2CPL MK83", DT -0.03). **Not yet on the
  FLEX-6600** - first live things to check: `client gui` with SmartSDR also
  running (per-client slice letters), the RF level the radio makes from
  full-scale DAX audio, and that `xmit 0` drops the carrier at once.

### 2026-10-07 — FT2: 20-QSO run - the floor is the same, MSHV's AP7 is better at it

`tools/ft2_autotest.sh --snr -14,-15,-16 --repeats 20` (360 periods, 20:54-21:24 UTC,
results in `lib/ft2/tools/bench/autotest/2026-10-07-sweep-20qso.txt`):

  | SNR | JTDX-VU plain / AP7 / all | MSHV plain / AP7 / all | of |
  |---|---|---|---|
  | -14 | 95 / 7 / 102 | 87 / 12 / 99 | 120 |
  | -15 | 23 / 2 / 25 | 33 / 33 / 66 | 120 |
  | -16 | 3 / 0 / 3 | 3 / 0 / 3 | 120 |

- **Fixed the same day - see the AP7 entry above.** Level at -14 and -16. **At -15 MSHV gets 66 to JTDX-VU's 25, and 33 of
  MSHV's are AP7** - in nearly every QSO it pulls the R-report (period 4)
  and the 73 (period 6), the two messages whose pair was decoded two
  periods earlier, where JTDX-VU's AP7 fired twice in 120. The plain
  decoders are within noise of each other (23 vs 33); the gap is the AP7
  acceptance. JTDX-VU's port keeps MSHV's gates (best within 100, 1.27x
  clear of the second best, <=95 hard errors, quality >= 0.02) but
  something upstream of them - the re-sync at the pair's frequency, the
  LLR sets, or the message list - is evidently stricter here. Next decoder
  item: instrument `a7d` on this run's conditions (the sim can replay the
  -15 dB QSO indefinitely) and compare the candidate distances with MSHV's.
- MSHV made **one false AP7 decode** in 360 periods (`W9XYZ K1ABC +32` at
  -21, a message never sent); JTDX-VU none.

### 2026-10-07 — FT2: automated same-audio comparison, JTDX-VU = MSHV (for the next release)

Manoj ("automate the tests... i am not going to sit and do this anymore",
then "do it on your own. good night"). Built and run overnight, nothing to
click:
- **`tools/ft2_tci_sim.py`** - a TCI server that *is* the band: Gaussian noise
  (RMS 0.05) plus a scripted FT2 QSO (CQ K1ABC FN42, K1ABC W9XYZ EN37,
  W9XYZ K1ABC -10, K1ABC W9XYZ R-12, W9XYZ K1ABC RR73, K1ABC W9XYZ 73; one per
  3.75 s period from a 15 s UTC boundary, then two silent periods; `ft2sim`
  at snr 99 = clean full scale, resampled 12 -> 48 kHz with a windowed sinc),
  streamed at an exact 48 kHz on the wall clock (worst lateness 0 ms over 16
  min) as float32 stereo 2048-sample frames to every client that sends
  `audio_start`. Built on `tools/fake_tci.py` (handshake, start-up burst,
  state echo); says `protocol:ExpertSDR3,1.9`, which both clients accept.
  SNR in 2500 Hz: amplitude g = sigma * 10^((snr - 6.8)/20); JTDX-VU reports
  it back within 1 dB (6 -> 6, -14 -> -14), so the calibration holds.
- **`tools/ft2_autotest.sh --snr ... --repeats N [--nomshv]`** - starts the
  sim, an MSHV head (the LZ2HV-tree test head, VU2BBB, FT2, TCI Client RX1,
  RX only, with an `AllTxtMonthly/` dir so it logs decodes), a JTDX-VU
  `-r tcitest` (CAT PTT, UDP moved off RUMlog), waits for the plan, quits
  JTDX-VU through its menu, keeps both logs + schedule in a results dir and
  scores them with **`tools/ft2_score.py`** (per SNR: plain / AP7 / all for
  each decoder, reported-SNR means, a per-message map; lists stray lines =
  false decodes). Refuses to start while the live MSHV runs. Makes the six
  periods itself if `/tmp/ft2_autotest-sig` is missing.
- **Result (`lib/ft2/tools/bench/autotest/2026-10-07-sweep.txt`, 8 SNRs x 4
  QSOs = 192 periods, 20:24-20:40 UTC):**

  | SNR | JTDX-VU plain / AP7 / all | MSHV plain / AP7 / all | of |
  |---|---|---|---|
  | -12 | 24 / 0 / 24 | 24 / 0 / 24 | 24 |
  | -14 | 19 / 1 / 20 | 21 / 1 / 22 | 24 |
  | -15 | 10 / 0 / 10 | 7 / 3 / 10 | 24 |
  | -16 | 2 / 0 / 2 | 1 / 0 / 1 | 24 |
  | -17..-20 | 0 | 0 | 24 each |

  **Same floor.** No false decodes on either side (every logged line was a
  planned message in its period). Reported SNR for the same signal: JTDX-VU
  -12/-13/-14/-16, MSHV -14/-16/-17/-17 - MSHV prints 2-3 dB lower (FT4's
  -14.8 constant kept for FT2 vs JTDX-VU's -11.3), which is the whole of
  Manoj's "MSHV goes down to -21, JTDX stops at -15" from the manual test.
  MSHV's AP7 fires a little more often at -15 (3 vs 0 here), JTDX-VU's once
  at -14; with 4 QSOs per level that is within noise - worth a 20-repeat
  run on -14/-15/-16 only if the AP7 rate itself matters.
- **What did not work, so nobody tries it again:** feeding Christo's TCI_HV
  echo server from a Python TCI client (`tools/tci_player.py`, kept). TCI_HV
  paces its stream on a Qt timer that ran at 47-87 % of real time here
  (varied run to run), so audio played into it came out time-compressed
  (bursts of 1.25 s instead of 2.47) and nothing decoded; with the player
  alone it happened to be fast enough once. Also learned: the server's
  frame `length` counts floats (both channels), JTDX-VU never sends the
  `audio_stream_*` set-up (an MSHV head does), and a websocket frame with a
  non-minimal length encoding makes Qt drop the link at once.
- The harness needs the MSHV test head at
  `MSHV-Mac/sent-to-LZ2HV/2026-09-16-tci-echoserver-received/heads/mshv-noblock`
  and the scrubbed rc029 settings next to it; `JTDX=` and `SIG=` override
  the binary and the signal dir.

### 2026-10-07 — FT2 loopback test run: AP7 seen on air, MSHV still decodes deeper

Manoj ran the loopback test (`tools/ft2_loopback.sh`, Noice slider up step
by step, QSO VU2AAA <-> JTDX-VU `-r tcitest` at each step; log in
`~/Library/Application Support/JTDX-VU - tcitest/202610_ALL.TXT`, 19:17-19:36
UTC 2026-10-06). Test closed by Manoj ("not doing this monkey business"
to the save-the-wavs step) - results as far as they go:
- **AP7 fired once, correctly:** 19:22:33 `VU2CPL VU2AAA RR73  7` when the
  plain decoder missed the period; true message, QSO completed. **No false
  decodes** in the whole run (about 40 CQ decodes, 11 QSOs).
- Every QSO completed, up to the server's new maximum noise, on plain
  decodes (reports -13..-16). The printed SNR sits at -13..-15 from
  mid-range on while the CQ decode rate falls from every period to 1 in 4:
  JTDX-VU's FT2 SNR estimate bottoms out there.
- **Manoj's observation: MSHV (VU2BBB head) "goes down to -21, JTDX stops
  at -15".** Part of that is scale: MSHV reuses its FT4 constant
  (`10*log10(snr)-14.8`, `decoderft4.cpp:2017`) for FT2 and clamps at -21,
  JTDX-VU uses -11.3 (`ft2_decode.f90:355`) - MSHV prints 3.5 dB lower for
  the same signal. The rest is real: MSHV kept decoding the CQ at noise
  levels where JTDX-VU got 1 period in 4. Not quantified (no same-audio
  comparison was made).
- **Setup fixes along the way:** PTT method CAT in the test ini (a missed
  `trx` echo otherwise costs a 5 s reconnect, see the crash entry); the
  echo server's Noice slider spans only +-3.1 dB, so a copy with +-12.5 dB
  was built (`MSHV-Mac/sent-to-LZ2HV/2026-09-16-tci-echoserver-received/
  build-widenoise/`, one constant changed in `NSlidChanged`); the script
  prefers it when present. The MSHV head's Tx level slider stops
  transmitting near zero, so it is no use as the attenuator.
- **Done the same night** (see the next entry): the same-audio comparison
  ran hands-off with `tools/ft2_autotest.sh`; the two decoders have the same
  floor.

### 2026-10-07 — Crash in the TCI reconnect loop (for the next release)

JTDX-VU `-r tcitest` crashed at 00:17 during the FT2 loopback test
(`jtdx-2026-10-07-001741.ips`): SIGSEGV in `QEventLoop::exec` under
`TCITransceiver::do_start`, the same stack as the 2026-10-01 quit crash.
Context from `202610_ALL.TXT`: for the last ~20 minutes every transmission
was followed by "0 MHz" (rig offline) and "14.084 MHz" 5 s later - the TCI
auto-reconnect was cycling on every TX (reason not captured; the status bar
shows it as "TCI: ... - reconnecting every 5 s"), so `do_start` was running
its 1.5 s connect wait in a nested event loop every few seconds, with the TX
slider being moved (echo server log: drive 52 -> 100) and audio dropping
("partial loss of data").
- **Cause:** the 2026-10-01 fix guarded only `TransceiverBase::stop()`.
  `start()`, `set()` with an online/offline transition, and `offline()`
  (the poll's failure path) all call `shutdown()` -> `do_stop()`, which
  deletes the TCI event loops and timers; delivered inside a nested wait
  (`do_start`'s connect sleep, or a command waiting for its echo while the
  poll fails) they delete the loop the waiting code returns into. Which of
  them fired this time is not known for certain (no debug build); all three
  are reachable in the reconnect loop.
- **Fix** (`TransceiverBase.cpp`): the same deferral for all of them. In a
  nested wait, `start()` and an offline `set()` set `stop_aborting_`, end the
  waits (`abort_waits()`) and re-queue themselves with `QTimer::singleShot
  (0)`; `offline()` likewise, clearing the flag before it reports; an online
  `set()` while something is still in progress retries after 50 ms without
  aborting. Non-transition `set()`s are untouched (they nest their own waits,
  as before).
- Builds clean; bundle rebuilt so the loopback test instance has it. Not yet
  reproduced on purpose - the quit test (`tools/run_quit_test.sh`) covers the
  stop path only.
- **Resolved (Manoj, 2026-10-07):** the status bar had said **"TCI failed to
  set ptt"** (`do_ptt`: the `trx` echo not back within 1 s). With the test
  instance's PTT method set to **CAT** the loop went away and a loopback QSO
  completed, so the AP7 test is running. The echo server log matches: `trx`
  lines for the first ~20 overs, then none, only audio_stop/audio_start
  pairs (reconnects). Still worth noting: `error_` in `TCITransceiver` is
  never cleared once set, so after one failed PTT every poll throws until
  the reconnect makes a fresh object - one missed echo costs a 5 s
  reconnect and the over.

### 2026-10-06 — 70cm and 23cm band buttons (for the next release)

Frank PH2M (JO22hc), by email after installing v0.6.0 on his laptop: the
European UHF/SHF FT8/FT4 crowd wants band buttons for 70cm and 23cm. The
switcher's band list (`BandModeSwitcher::all_bands`) ended at 2m, so the
two bands could not even be ticked in View > Band & Mode Buttons.

- `all_bands` now has "70cm" and "23cm" between 2m and QO-100. They are
  opt-in like 4m / 2m (not added to anyone's row automatically, unlike
  QO-100 - most users have no gear there).
- Default frequency list gains 1296.174 FT8 (WSJT-X's entry; stock JTDX
  only had 1296.065 JT65 and 1296.500 WSPR). New / reset lists only.
  70cm already had 432.174 FT8. No FT4 defaults on either band (none in
  WSJT-X either); `switch_to_band` falls back to any row on the band, so
  FT4 on 70cm tunes 432.174 unless the user adds a 70cm FT4 row.
- `Bands.cpp` already knew 70cm (420-450) and 23cm (1240-1300).
- Builds clean on the Mac (`make jtdx`); not tried on air (no UHF/SHF rig
  here). Manoj replied to Frank that it will be fixed; the release build
  with the buttons is still to go out.

### 2026-10-06 — QO-100 with SDR-Control: "QO-100 rig IF" (for the next release)

The afternoon's real story, replacing the "Reported high by" analysis below
(that setting stays, default 0, but it was the wrong fix for the wrong
symptom and is now 0 in Manoj's QO-100 profile).
- **How SDR-Control's rigctl really works** (Manoj's station: radio on
  **28.540**, transverter; SDR-Control SAT/Transverter offsets named from
  the SATELLITE's side, Marcus's convention — "RX offset" 2371.5 = sat RX =
  our uplink 2400.040, "TX offset" 10461 = sat TX = our downlink
  10489.540): `F` sets the RADIO's own frequency (the base), `f` returns
  base + TX offset = **the downlink** — correct and what Manoj wants to
  see. Every odd readback today (12861.040, 20950.540 ... 52333.540) was
  base + offset after JTDX-VU had set the base to 10489.540 or re-sent a
  readback. Nothing was wrong on SDR-Control's side; the Marcus draft is
  withdrawn.
- **Fix:** Settings > Frequencies > Frequency Calibration > **QO-100 rig
  IF** (`[Configuration] QO100RigMHz`, `qo100_rig_mhz_`, 0 = off):
  `Configuration::impl::rig_side()` sends the rig this IF for any QO-100
  nominal (QO-100 button, startup DialFreq, Tx frequency), and
  `handle_transceiver_update` maps a readback of the downlink, the uplink
  or the IF itself to the downlink for display (`as_seen`). Manoj's
  profile: 28.54.
- **Second bug, found on air ("with each tx, freq is going up"):**
  `cached_rig_state_.frequency` — what TransceiverBase::set compares with
  its last request and re-sends on every PTT/mode set — was filled from
  the readback (10489.540), so each over set SDR-Control's base to
  10489.540 and the readback climbed 10461 per TX (10489 → 20950 → 31411 →
  41872 → 52333, ALL.TXT 14:32-14:33). It is now kept on the rig side
  (the IF). Manoj: **"freq is stable now"** (20:1x).
- Also reset by hand (app closed): the profile's saved DialFreq had the
  runaway 52333.54 and would have been sent at the next start.
- **22:55, Manoj: "jtdx vu opens qo100 profile at freq 20950540. why?"**
  Because something else had set SDR-Control's base to 10489.540 (readback
  20950.540 = base + 10461) and JTDX-VU followed the readback at start
  instead of sending its saved dial. Now, in a profile with a QO-100 rig
  IF, a readback in NO amateur band while the last nominal was QO-100 is
  not followed: the rig is put back on the IF (`handle_transceiver_update`,
  `qo100_last_nominal_`); a real retune into a band is still followed.
  The base was put back on 28.540 by hand (rigctl) meanwhile.
- **23:00 - the real cause of the 20950.540 at every start:** JTDX's
  `HamlibTransceiver::do_start` probes the rig's tuning resolution - sets a
  test frequency, reads it back, then **writes the frequency it had read
  back to the rig**. With SDR-Control that write (10489.540) becomes the
  radio base and the readback jumps to 20950.540 at every launch; restarting
  either program cannot escape it (Manoj: "tried restarting sdr control and
  jtdx. still 20950540"). Now a profile with a QO-100 rig IF sets the
  `no__probe` bit in the parameter pack (`gather_rig_data`) and the probe is
  skipped (`no_freq_probe_`). The no-band readback guard above remains as
  the safety net. Installed 23:0x after resetting the base by hand.
- Lesson recorded: I moved the radio twice with `F` "tests" and built two
  fixes on readback numbers without seeing SDR-Control's display; the
  station owner's description of the chain settled it in one line.

### 2026-10-06 — Rig "Reported high by" correction for SDR-Control on QO-100 (for the next release)

Manoj made a profile for QO-100 with SDR-Control for Icom (rig Hamlib NET
rigctl 127.0.0.1:5001, audio CommonRadioAudio In 1 / Out 1) and JTDX-VU
showed **12861.040 MHz**. Measured with rigctl against SDR-Control's server:
`f` returns 12861040000 for a radio set to 10489.540 — SDR-Control's
2371.5 MHz transverter offset (28 MHz IF → 2.4 GHz, set in it on 2026-09-19)
is applied once more on readback; its SET takes the real frequency (`F
12861040000` moved the radio to a readback of 15232.54; `F 10489540000`
restored it, readback 12861.04). So reads are high by 2371.5 MHz, writes are
right, and a Station-Information offset (added on writes too) cannot fix it.
- **Settings > Radio > Frequency Calibration > "Reported high by" (MHz)**
  (`[Configuration] RigReportOffsetMHz`, `rig_report_offset_mhz_`): taken off
  every frequency the rig reports (`handle_transceiver_update`, both the
  cached state and the update passed to the main window), never added to
  what JTDX-VU sets. For this setup: **2371.5**. Per profile, like every
  setting. **Scoped** (Manoj: "if i fix profile, it applies to all
  frequencies" - the same SDR-Control profile also works HF): the offset is
  taken off only when the raw reading is in no amateur band and the
  corrected one is (12861.040 -> 10489.540, 3cm); 14.074 from the same
  server is a band already and passes through untouched. The QO-100 button then sets 10489.540, SDR-Control reports
  12861.040, JTDX-VU shows 10489.540 and logs 13cm/3cm as designed.
- Not yet tried on air (Manoj's instance was running while this was built).
- Profile naming note from the same session: the SDR-Control settings ended
  up in a profile called "TCI FLEX HF" (the Default profile still holds the
  Aether TCI setup). No rename in the menu yet: *New profile from current...*
  as "QO-100" from that instance, then delete the misnamed one.

### 2026-10-06 — Profiles: one settings set per rig, from a Profile menu (for the next release)

Manoj: "implement profiles in mshv and jtdx ... can run 1 profile with
vita49, 1 with tci, 1 with dax etc." (and a QO-100 one: SDR-Control CAT and
audio). A plain launch opens the last used profile; MSHV's copy is a private
feature (its HANDOVER has the details).
- **What a profile is:** JTDX's existing `-r <name>` instance - the settings
  file `JTDX-VU - <name>.ini` and the data directory `JTDX-VU - <name>`
  (log, ALL.TXT, saved files) beside the default ones. Nothing new on disk;
  the Default profile is the plain `JTDX-VU.ini` as before.
- **Profile menu** (`MainWindow::profileMenuSetup/Fill/Switch/New/Delete`,
  inserted before View): every profile found in the settings directory,
  the current one ticked; *New profile from current...* copies the current
  settings file (data-directory paths rewritten, as the JTDX -> JTDX-VU
  migration does) and the data directory, then offers to switch;
  *Delete profile* removes a non-current one after confirmation. A switch
  records `Profiles/Last` in the default settings file, closes, and starts
  the same binary with `-r <name>` from `aboutToQuit` (after the rig and
  audio devices are released). The window title carries the name, as `-r`
  always did.
- **main.cpp:** without `-r` (and not `--test-mode`) the launch reads
  `Profiles/Last` from `JTDX-VU.ini` and opens that profile; `-r <name>`
  still picks one explicitly, so scripts and the `-r tcitest` instances work
  unchanged. Switching to Default writes an empty `Last`.
- **Gotcha:** macOS hides a menu that is empty when the native menu bar is
  built, so the menu is filled once at creation and refilled on
  `aboutToShow`.
- **Tested** (trial bundle, test profiles later removed): menu lists
  Default / proftest; New profile "alpha" created the .ini and data copy
  and the switch restarted as "JTDX-VU - alpha" with jtdxjt9 running;
  `Last=alpha` recorded; Delete removed proftest. Not yet tried with a real
  rig per profile.
- **Found on the way - bundle bug:** `macos-bundle.sh` relied on the CMake
  fixup_bundle stage to copy `libgomp.1.dylib` before that stage errors on
  `@rpath/libsharpyuv`; today it stopped earlier and the bundle shipped
  without it - jtdxjt9 died at launch ("Library missing") and the app
  showed "Subprocess Error". The script now copies libgomp itself and
  fails if any `@loader_path/@executable_path ../Frameworks` dylib is
  missing. The v0.6.0 release zips and the 10:20 trial zip do have libgomp.

### 2026-10-06 — FT2: AP7 ported from MSHV (for the next release)

The last piece of the FT2 sensitivity work (see the open item below): MSHV's
"AP7" a-priori decode, which is where 4 of the 7 decodes JTDX-VU missed in
LZ2HV's recording came from.
- **What it does** (`lib/ft2_decode.f90`, `a7_save` / `a7_roll` / `a7d` /
  `a7_msg`; state in `lib/ft2_mod1.f90`): every decoded call pair is kept
  per even/odd period slot (MSHV `ft2_even_odd`: 15 s clock, 3.75 s
  periods). Two periods later, when the same station is due to answer, the
  decoder re-syncs at that pair's frequency (3 DT segments x 2 sync passes,
  gates smax>=0.5, nsync_qual>=10), demaps once, and tests the ~158 likely
  follow-on messages for the pair (`a7_msg` = MSHV `SetAp7Msg`: reports
  -30..+30, R-reports, RRR/RR73/73, CQ variants, grid) by weighted
  hard-decision distance against each message's codeword on LLR sets A-D.
  Accepted if the best is within 100, stands 1.27x clear of the second
  best, has <=95 hard errors and passes MSHV's quality gate
  `1-(nharderrors+dmin)/60 >= 0.02`. Decodes are flagged `7` (the trailing
  character, where FT4 prints `1`), deduped against the normal decodes and
  fed back into the history. Runs at depth >= 2. `decoder.f90` passes the
  period time (`na7utc`); `tools/ft2dec` advances a 3.75 s clock per file
  so sequences can be tested offline.
- **Bench** (ft2sim, mild fading 0.5 Hz / 1 ms, QSO K1ABC-W9XYZ at 1500 Hz):
  four -10 dB openers seed the history, then four reports at **-18 dB** (the
  normal decoder's floor is about -16), then two noise-only periods. Result:
  the -18 dB `K1ABC W9XYZ RR73` decodes via AP7 (35 hard errors, distance
  23 vs 59); one more report matched the true message but failed the 1.27x
  margin, one was sync-limited, one passed the margin but not the quality
  gate; **0 false decodes** on the noise periods. On a -10 dB sequence the
  matcher picks the true message at distance 1.7 vs 58 (ratio 34). The
  flat / mild-fade / fast-fade regression benches are unchanged (29 26 7 2
  0 0; 30 30 27 28 24 12; 30 30 27 24 15), as AP7 only adds decodes.
- **Gotcha that cost a session:** the AP7 port was first written with the
  **FT8 `rvec`** (77-bit scrambling vector) instead of FT4's. Both start
  the same and diverge at bit 38, so every candidate codeword was wrong
  from the second callsign on and the matcher picked junk with a 1.17
  margin. FT2 uses FT4's `rvec` (`lib/ft4/genft4.f90`,
  `lib/ft4_decode.f90`); anything that builds FT2/FT4 codewords must copy
  that one, not `ft8_decode`'s.
- **Christo's recordings (FT2.zip, 2026-10-06; kept in `~/projects/JTDX-records/`, outside this public repo):** five single 3.75 s periods
  (two duplicated under `16_`/`17_` prefixes, presumably MSHV's SNR). JTDX-VU
  decodes all five - LZ2HV +24/+19, SP9HWY R-12 (-12), SP9HZZ (-13), and
  8 stations in 260304_124133 - and the pre-demapper baseline gets the same
  counts, so these files don't separate the builds; being single periods
  they can't exercise AP7 either. MSHV's own count for the 8-decode file is
  still unknown (needs the MSHV GUI, File > Open WAV).
- **Loopback sensitivity test (Christo's method):** `tools/ft2_loopback.sh
  start|stop` runs his TCI_HV echo server (adds Gaussian noise, "Noice"
  slider) on 127.0.0.1:50002, two isolated MSHV heads (VU2AAA / VU2BBB, FT2,
  TCI Client RX1) and a JTDX-VU `-r tcitest` instance (VU2CPL, TCI audio,
  UDP moved to 2299 so RUMlog never sees it). Brought up and verified
  connected on 2026-10-06; the manual part - start a CQ in the VU2AAA head
  and lower its Tx level step by step, score who still decodes - is
  pending (Manoj: "will do later"). Control of the MSHV / TCI_HV windows
  via computer-use was declined, so that step is by hand.

### 2026-10-05 — QO-100 support (for the next release)

Manoj: QO-100 frequencies / mode, satellite prop mode and sat name. His
station: uplink (Tx) 2.4 GHz, downlink (Rx) 10.489 GHz.
- `Radio.hpp`: `is_qo100_down` (10489.5-10490 MHz), `is_qo100_up`
  (2400.0-2400.5), `qo100_uplink/downlink` (offset 8089.5 MHz),
  `qo100_ft8` = 10489.540 MHz downlink (2400.040 uplink).
- **Logging** (`LogQSO::accept`, both the wsjtx_log.adi record via
  `ADIF::addQSOToFile(..., extra)` and the UDP/TCP ADIF): when the dial is
  on either side of the transponder, FREQ / BAND = uplink (13cm), FREQ_RX /
  BAND_RX = downlink (3cm), `PROP_MODE` SAT, `SAT_NAME` QO-100 (what LoTW
  needs). The dialog's Band field shows 13cm.
- **Worked-before / colours / wanted**: `displayDecodedText` uses the uplink
  frequency, so QO-100 decodes are checked against 13cm, the band QSOs are
  logged on.
- **QO-100 band button** (`BandModeSwitcher::all_bands`, offered once to
  existing setups via `[Switcher] QO100Offered`): switches to FT8 if needed
  and tunes the downlink 10489.540; highlighted while on the transponder.
  Default frequency list gains 10489.540 FT8 (new / reset lists only).
- **Aligned with MSHV-Mac** (Manoj: "we have already implemented the same
  there"). MSHV's 70 QO-100 QSOs in `mshvlog.adi` log BAND 13CM, FREQ
  2400.040000, PROP_MODE SAT, SAT_NAME QO-100, **SAT_MODE SX**, FREQ_RX
  10489.540; its QO-100 user band is the downlink window 10489.4-10489.9
  MHz with 10489.540 for every mode. So: SAT_MODE SX added, downlink window
  from 10489.4 MHz, the button keeps FT4 / FT2 (else FT8), tunes the
  downlink (settles the open question).
- **RUMlog**: RUMlog logs from the WSJT-X "QSO Logged" message (no satellite
  fields) and ignores the Logged-ADIF, so the fields were lost (MSHV-Mac
  `tools/README.md`, `mshv_rumlog_bridge.py`). JTDX-VU now, for a QO-100
  QSO with the secondary UDP server enabled, does **not** send "QSO Logged"
  and lets the ADIF go to that server - set it to the bridge,
  **127.0.0.1:2233** (LaunchAgent `com.vu2cpl.mshv-rumlog-bridge`, running),
  which saves the record into RUMlog via `SaveAdif` with the satellite
  fields. Without the secondary server, "QSO Logged" carries the uplink
  frequency (13cm). Worked-before (`addAsWorked`) and eQSL use the uplink
  band too. Bridge dry run on a JTDX-VU-style record: "would save ... 13cm
  FT8 ... PROP_MODE=SAT, SAT_NAME=QO-100, SAT_MODE=SX, BAND_RX=3cm,
  FREQ_RX=10489.540000". Not yet tried on air.

### 2026-10-04 — Band change keeps AnsB4 and 1 QSO (for the next release)

Manoj: AnsB4 and 1 QSO should stick instead of clearing on each band /
mode change. `band_change_reset()` now switches off only Auto CQ (it
transmits by itself); AnsB4 and 1 QSO are left as set. Mode changes never
reset any of them. Reverses part of the v0.5.2 behaviour.

### 2026-10-04 — Wanted filters: choose which ones show (for the next release)

Manoj (after confirming the Fake It fix works on the MacBook): the wanted
filters above the Rx pane take a lot of room. The existing "Wanted" tick
hides all four; now each can be shown or hidden on its own: **View >
Wanted filters > Callsign / Prefix / Grid / Country**, the same menu on a
right-click of any filter label. `m_wantedShow` bitmask (1 call, 2 prefix,
4 grid, 8 country; `[JTDXVU] WantedShow`, default 15) applied in
`on_cbShowWanted_toggled` (and once after the menu is built, since
readSettings runs first). A hidden filter with text still applies (said
in the label tooltip). Builds; not yet tried on screen.

### 2026-10-04 — Fake It split over TCI: runaway at Tx start (root cause found)

The first candidate (below) did not help. A `JTDX_DEBUG_TO_FILE=ON` build
on the MacBook (zip since deleted; log at
`~/Library/Application Support/JTDX-VU/jtdx_debug.txt`) showed the cause:
at PTT on, `EmulateSplitTransceiver::set` retunes the dial to the Tx
frequency (14074536 = +536 Hz at Tx 2036 Hz); the TCI server echoes
`vfo:0,0,14074536` within ~4 ms, **before** it confirms `trx:0,true`;
`handle_update` sees rig PTT false, "follows the rig", reports 14074536 as
the Rx frequency; MainWindow recomputes Tx = new Rx + 536 and sets again -
a loop of +536 Hz steps every ~1 ms until PTT is confirmed. In the log one
Tx went 14.074 -> 14.211 MHz in 0.35 s (~260 steps) and returned to the
wrong Rx after. Hamlib CAT never echoes that fast, hence never seen
before; the Mac mini runs Split None. Fix: `ptt_requested_` (from the last
`set`) - while we have asked for Tx, the Rx frequency is not taken from the
rig either. The end-of-Tx `restoring_` guard is kept. Builds; test zip (superseded by `JTDX-VU-trial-arm64.zip`).
- Manoj: "still drifting" with that build. Reproduced locally without the
  radio: `FAKE_TRX_DELAY=0.35 tools/fake_tci.py` (Tx echoed late, as
  AetherSDR does) + a `-r` test copy with Fake It, driven over UDP
  (TriggerCQ, type 51, after reading the client id from its heartbeat;
  controller script was scratch). Unfixed /Applications build: one Tx ran
  14.074 -> 13.27 MHz in 0.35 s (725 vfo commands). Fixed build: one vfo
  at Tx start, one at Tx end. Rebuilt with a fresh label (aacaf7) as
  `JTDX-VU-splitfix2-arm64.zip`; asked Manoj to check the revision on the
  MacBook, since the fix works under the logged timing.

### 2026-10-04 — Fake It split: Rx frequency crept up after each Tx (first candidate - not the cause)

Manoj on the MacBook, TCI + Split "Fake It", 14.074: after a Tx the dial
showed 14.077, during the next Tx 14.078 (not AetherSDR - Fake It retunes
the dial itself). Likely race in `EmulateSplitTransceiver` (upstream code):
when PTT drops, the rig can report "not transmitting" while its dial is
still on the Tx frequency; `handle_update` then "follows the rig" and the
shifted dial becomes the new Rx frequency, so every Tx shifts again (with
TCI, Tx = dial + TxHz - 1500, about +1 kHz per Tx at 2500 Hz). Fix: after a
Tx with a shifted frequency (`was_tx_`), `restoring_` keeps reporting the
requested Rx frequency until the rig reports it (or 3 s pass). Builds; not
yet tested - trial zip `~/Desktop/jdxvu/test/JTDX-VU-trial-arm64.zip` (removed 2026-10-07) for
the MacBook. Open: why it never showed before (the mini uses split None;
the MacBook setup is new) - asked Manoj.

### 2026-10-04 — Auto CQ and 1 QSO exclude each other (for the next release)

Manoj asked whether 1 QSO makes sense with Auto CQ on; agreed to make them
exclusive. Switching Auto CQ on (the `set` lambda behind the button / CNS
action) unchecks 1 QSO. Pressing 1 QSO while Auto CQ runs sets
`m_stopAfterQso` ("Auto CQ: stopping after this QSO"); the next Halt Tx -
normally `autoStopTx` at FIN of that QSO, or a manual halt - switches Auto
CQ and 1 QSO off ("Auto CQ stopped after the QSO") in
`on_stopTxButton_clicked`. Band change resets clear the flag too. Builds
clean; not yet tried on air.

### 2026-10-04 — TX slider follows the TCI drive; new RX slider (trial, for the next release)

Manoj: the Pwr slider sat at 0 while Tx worked. Cause: the saved
`OutAttenuation` was only applied on the first band change made from JTDX
(`ui->outAttenuation->value() == 1` sentinel in band_changed), so after a
TCI start the slider showed the .ui value 1, and nothing was sent until it
was moved - the radio kept its own drive (AetherSDR reported 8 %). Moving it
would have set AetherSDR's RF power (top = 100 %), risky with an amp.
- **TCI audio (`m_tci`)**: the slider follows the SDR program. New
  `TransceiverState::drive()` (-1 = unknown, in `!=` and the debug print),
  `TransceiverBase::update_drive`; `TCITransceiver::do_poll` reports the
  `drive:` value it has seen. `MainWindow::handle_transceiver_update` sets
  the slider to 4.5 x drive (the inverse of `do_txvolume`) with signals
  blocked, not while tuning or dragging. Nothing is pushed at start-up.
  Other TCI programs get the same.
- **Sound card audio**: the saved level is shown from the start
  (readSettings); the slider is still the digital audio gain, not radio power.
- Labelled **TX** (was "Pwr"; also "TX<br>N W" while transmitting).
- **RX slider** beside it: receive gain -20..+20 dB (0 = as received),
  `[Common] VURxGainDb`. `g_vuRxGain` (atomic, AudioDevice.cpp) applied
  with clipping in `vu_rx_sample()`, used by `AudioDevice::store` (sound
  card, Detector) and `TCITransceiver::store` (TCI) - so it reaches the
  decoder and the level meter for any radio. Not the radio's AF gain.
- Builds clean; not yet tried live. Per-band power memory still sets the
  slider on band change when enabled (Manoj has it off), which over TCI
  sends that drive to the radio - as before.

### 2026-10-04 — Notifications tab redesigned; main-window "New only" tick (trial, for the next release)

Manoj: keep all six categories (zones, DXCC, grid, prefix, call) but make
the page friendlier, and replace the Show dropdown with one tick as in his
MSHV build. Mockup agreed ("lets try this"); trial build on the Desktop
(now only `~/Desktop/jdxvu/test/JTDX-VU-trial-arm64.zip` (removed 2026-10-07)) for him to try.
- `Configuration::impl::vu_notifications_page()`: the original page
  (`verticalLayout_8`) is parked in a hidden widget; a new scrollable page
  takes its place. **Front end only** - every new control mirrors an
  original widget (clicks it, re-reads it via `toggled`), colour squares
  click the original colour buttons, so slots, enabling rules, save/load
  are unchanged. `vu_notify_refresh_` re-syncs (also at the end of
  `initialize_models`).
  - "New ones": one row per category in priority order (CQ zone, ITU zone,
    DXCC, Grid, Prefix, Call - the top row wins): on/off + never-worked
    colour, "Band" + colour, "Mode" (the old "per band+mode"
    tick: alone = this mode any band, with Band = band+mode slot; on
    DXCC = new in mode, own colour), Beep.
  - "Already worked": Colour + square, Strike through, Underline, Hide.
  - "Messages and markers": CQ/73, My call, My Tx, Other standard colours;
    the marker / beep / RR73 / text-colour ticks, plainer labels.
  - "Preview": six sample lines in the chosen colours, live.
- `BandModeSwitcher`: the Show combo is now a "New only" check box (amber
  when on); `[Switcher] NewOnly` > 0 = on. `DisplayText::needed()` is now
  a member that re-runs the same log checks as the colouring for every
  enabled category (`LineMeta` gained `grid`), so the filter shows exactly
  what Settings colours "new"; a station still drops out once worked.
- Manoj's first look (screenshots): asked why lines have coloured
  backgrounds, for an on/off for that, and for the on-page explanations
  (grammatically wrong) to become hover tooltips. Done:
  - **Background highlight** (`highlightBackground`, `[Configuration]
    HighlightBackground`, default on = JTDX as before). Off: in
    `displayDecodedText` the pane's plain background is kept and the colour
    that would have filled it goes on the text; in dark style it is lifted
    to HSL lightness 150 so dark highlight colours stay readable. The old
    "Inverse text/background color" tick is labelled "Swap text and
    background colours" (it never removed the background) and is greyed
    while Background highlight is off.
  - The section explanations are group-box tooltips; every control has its
    own rewritten tooltip (JTDX's originals are no longer shown here).

### 2026-10-04 — UDP "Network Error" box replaced by a status-bar message (for the next release)

On the MacBook the UDP server was the default 255.255.255.255:2237, which
macOS refuses, so a modal "Network Error / Unable to send a message" box
came up again and again. `MainWindow::networkError` now shows "UDP
host:port: <first line of the error> - check Settings > Reporting" for
15 s (`showStatusMessage`), and on "UDP server lookup failed" re-runs
`set_server` after 30 s (what the box's Retry did). Fix for the MacBook
itself: Settings > Reporting > UDP Server 192.168.1.109 port 2334 (as the
mini) or UDP off. Builds clean; not yet seen live.

### 2026-10-03 — AutoSeq queue, current-decode rule, JTTY calls heard (for the next release)

**Queue.** Manoj: "can we right click a station to add it to a q?" His
choices: called when next heard (not blindly after the QSO), the queue
beats everything, a small list by the panes.
- `DisplayText::contextMenuEvent`: the standard menu plus "Queue CALL
  (call when next decoded)" / "Remove CALL from queue", from the line's
  `LineMeta::call`; `queueToggled(call)` signal. `queue_` (pointer to
  `MainWindow::m_queue`) is null in JTTY, so no entry there.
- `MainWindow::queueSetupUi/queueToggle/queueChanged/queued`: list (label
  + `QListWidget`, max 4 rows) inserted in `verticalLayout_9` above the Rx
  Frequency pane, where JTTY's calls heard sit; shown only with calls in
  it and not in JTTY; right-click: Remove / Clear queue. Logging a queued
  call (`m_lastloggedcall=call` in the log path) removes it (base-call
  match). A queued pick sets `m_cqRunQso`, so CNS continues after it.
- `QsoHistory::setQueue`: `_queue` base call -> rank. `score()` returns
  100000 + 1000*rank for a queued call. Loops 1/2 (callers to our CQ)
  accept a queued station's RCQ/RFIN even after a caller was taken, and an
  ordinary caller can't override a queued pick (`priority < 100000`).
  Loop 3 (others' CQs) also runs when CQ search is off (CNS / no
  CallPrioCQ) but then only for queued calls; RFIN counts for queued
  ones. Direction filters (CQ DX / CQ NA) still apply.
- Not in JTTY; needs AutoSeq on and a call mode other than "None", as any
  autoselect.

**Current-decode rule.** Manoj: "call only if currently decoded to be
enforced in jtdx for normal auto sequence behaviour also". Loops 1 and 3
already required `tt.time == max_r_time`; loop 2 ("my CQ answers not
answered 1st time") took callers from the last 5 min. It now needs
`tt.time == max_r_time` too, so it only adds stations decoded this
period. Continuing the current QSO (DX call set) is unchanged.

**JTTY calls heard.** Manoj: the list also showed "stations being called
by others". `jttyHeardFromLine` now adds only the sender: a call after DE,
a call sent twice, a call in a line starting CQ / QRZ / TEST, or a line
that is just one call; a call right before DE (the one being called) never
goes in. Checked on the default macro texts.

Installed on the Mac (`/Applications/JTDX-VU.app`, revision ec3157, still labelled 0.5.3) for Manoj to try.

**Verified:** the QsoHistory harness (scratch) now has 14 cases, all
pass - added: queued CQ beats a new DXCC; unheard first queued call skipped
for the next heard one; queue works with CQ search off; queued call not
decoded this period not called; a CQ only from the last period not called;
a queued station's CQ beats a wanted caller to our CQ; a caller from the
last period not called. Builds clean. Not yet tried in the GUI or on air.

### 2026-10-03 — AutoSeq picks wanted stations first (for the next release)

Manoj: "auto seq to pickup wanted as per our log. that is new dxcc, mode
or band". Choices he made: wanted first, then others (not wanted-only);
also applies to callers answering our CQ; **always on, no button** ("make
it universal. why need a button?").

- `DisplayText::wanted()` (displaytext.cpp): 3 = DXCC never worked,
  2 = DXCC not worked on this band, 1 = not worked in this mode on any
  band, 0 = otherwise / unknown entity. Same `matchDXCC` tests as the
  Show filter's `needed()`, but independent of the Notifications colour
  settings (JTDX's own priority 20-23 only exists when those are on).
  Passed into `QsoHistory::message()` as a new last argument (default -1
  = leave as is, used for our own TX lines).
- `QsoHistory::QSO::wanted`, reset to 0 when the QSO reaches FIN.
  `QsoHistory::score()` = `100 * wanted + priority` for a wanted station,
  else `priority`. All three autoselect loops (callers to our CQ, callers
  not yet answered, others' CQs) now compare `score()` instead of
  `priority`, so any wanted station outranks every non-wanted one,
  DXCC > band > mode, then JTDX's priority, then signal/distance as
  before. Score >= 100 also clears the AnsB4 / CallB4 thresholds
  (`a_init` / `b_init` = 4). `prio` handed back to MainWindow is still
  JTDX's priority, so the give-up counters behave as before.
- Note: a wanted-by-log station now ranks above JTDX's Wanted Call /
  Prefix / Country lists (priority 17-19) and above new CQZ/ITUZ.
- **Verified** with a harness that links the real `qsohistory.cpp`
  (scratch, not committed): new-mode DXCC beats a new grid with a better
  signal; new DXCC beats new mode; nothing wanted → old order; CallB4 off
  still picks a wanted prio-0 CQ but skips a plain worked one; our CQ: a
  weak wanted caller is answered before a strong ordinary one, and with
  no wanted caller the strongest goes first. Not yet tried on air.

### 2026-10-03 — Auto CQ in the same place in every mode (after v0.5.2)

Manoj: "auto cq button is different in jtty and other modes. need to be
same for motor memory ease". In the FT modes it was 6th in the right-hand
column (Tune, Monitor, Bypass, 1 QSO, AnsB4, Auto CQ); JTTY hides the
three FT-only buttons, so it jumped up to 3rd.

- The button is now inserted directly under `monitorButton` (was under
  `AnsB4Button`), so the column is Tune, Monitor, Auto CQ everywhere;
  FT modes continue with Bypass / 1 QSO / AnsB4, JTTY with Halt Tx /
  Log QSO / Erase / Clear DX (`jttyApplyLayout` inserts those after the
  Auto CQ button, unchanged). Small windows that hide Bypass / 1 QSO /
  AnsB4 by height can no longer move it either.
- **Verified** (`-r tcitest`, Rig None): FT8 → JTTY → FT8, Auto CQ third
  each time.

### 2026-10-03 — Status-bar text no longer drawn under "WD Nm" (after v0.5.2)

Manoj: "text is being doubled" - screenshot: "Band changed: ..." with
"WD 10m" drawn over it. A `QStatusBar` message hides the normal widgets,
but stock `update_watchdog_label()` called `setVisible (true)` on every
update, so the watchdog box came back on top of the text (also seen in
the TCI tests: "WD 6m" over "TCI reconnected"). Stock JTDX bug, made
common by the new messages.

- `update_watchdog_label()` shows the label only while
  `statusBar ()->currentMessage ()` is empty; `QStatusBar::messageChanged`
  re-runs it, so the box returns when a message clears.
- `showStatusMessage()` takes a timeout (default 0 = stays, as before).
  "Band changed ...", "TCI reconnected" and "TCI audio back" clear after
  10 s; the reconnecting / still-no-audio warnings stay until replaced.
- **Verified** (`-r tcitest`, rig-side band change): message clean, no WD
  box; after 10 s Receiving / FT8 / WD 6m / progress all back.
- Installed `972b762b`. The quit → copy → relaunch took longer than
  AetherSDR's 10 s grace, so #6006 struck for real: the watchdog
  re-armed, reconnected (new port) and showed "TCI reconnected, still no
  audio from the SDR program - restart it" cleanly - **first live
  confirmation of the audio watchdog**. To avoid it when installing:
  copy first while the app runs is unsafe, so restart AetherSDR after,
  or relaunch inside 10 s.
- Manoj then restarted AetherSDR (14:12:13) with JTDX-VU left running:
  the same JTDX-VU process (PID 31020) reconnected on its own (new local
  port) and audio flowed again (7 MB within seconds) - **TCI
  auto-reconnect + audio recovery confirmed live**, no JTDX-VU restart.

### 2026-10-03 — Auto CQ, AnsB4 and 1 QSO switch off on a band change (v0.5.2)

Manoj: "auto cq, ans b4, 1 qso all are green even when changing bands" -
they should switch off.

- `MainWindow::band_change_reset()` unchecks the Auto CQ button (CNS in
  the FT modes, JTTY Auto CQ in JTTY - through its own toggled handler),
  `actionAnswerWorkedB4` and `actionSingleShot`, and shows "Band changed:
  <those that were on> switched off".
- Called from `band_changed()` when the band (not just the mode) differs -
  band buttons, band combo, scheduler - and from `displayDialFrequency()`
  when the rig reports a new band (AetherSDR, the Ulanzi deck). The
  latter must not skip the first change after start-up: JTDX's `startup`
  flag there stays true until the first rig-side band change, which
  swallowed the test's 20m → 15m.
- `tools/fake_tci.py`: a `<log>.push` file is sent as one message (e.g.
  `vfo:0,0,21074000;`), to imitate a band change on the SDR side.
- **Verified** (`-r tcitest`, all three on from the .ini / AutoSeq menu):
  rig-side 20m → 15m and the 40m button both switched all three off with
  the message.

### 2026-10-03 — TCI audio watchdog (v0.5.2)

After installing the reconnect build, TCI connected (CAT fine, 21.074)
but no audio. A stdlib probe client sent `audio_start:0` to AetherSDR
26.9.5: acknowledged, 0 bytes of audio in 3 s - AetherSDR's fault, not
JTDX-VU's. Its `TciServer.cpp`: when the last audio client sends
`audio_stop` *or disconnects*, `scheduleDaxRelease()` releases DAX after
`kDaxReleaseGraceMs` (10 s); a later `audio_start` should re-arm through
`ensureDaxForTci()`, and that failed here (old JTDX-VU quit 13:00:30,
new one started 13:01:52, 80 s gap). Restarting AetherSDR fixed it.
Skipping `audio_stop` on quit would not help (disconnect does the same).
Manoj: "this is not a fix" - report it upstream and add detection in
v0.5.2.

- `TCITransceiver`: `last_rx_audio_ms_` is stamped on every receive
  audio frame for our receiver (before the `audio_` check, so Monitor off
  still counts). `do_poll()`: with TCI audio on and connected, 10 s
  without audio (not while PTT) → send `audio_stop` + `audio_start` once
  (`audio_rearmed_`); 10 s more → `error_` "no audio from the SDR
  program - restart it", so the poll fails and the reconnect loop runs.
- `MainWindow`: `m_tciNoAudio` (reason contains "no audio") keeps the
  warning after a reconnect ("TCI reconnected, still no audio from the
  SDR program - restart it") until `dataSink()` sees frames: "TCI audio
  back".
- `tools/fake_tci.py` now streams silent 48 kHz float32 stereo receive
  audio after `audio_start`, unless `<log>.mute` exists (AetherSDR's
  stuck state). Gotcha: a websocket frame under 64 KiB must use the
  16-bit length - Qt drops the connection on the 64-bit form.
- **Verified** (`-r tcitest`, TCI audio on): 25 s of audio, no re-arm;
  mute → re-arm at +10 s, reconnect at +20 s, repeating; unmute → audio
  back on the current connection, no more drops; status messages as
  above; quit exit 0, no crash report.
- AetherSDR: already #6006 upstream; confirmation posted (see Open items).
- `tools/tci_audio_probe.py` (committed after v0.5.3): connects to the
  SDR program's TCI on 127.0.0.1:50001, sends `audio_start:0` and counts
  the binary audio bytes that come back - 0 means the SDR program is not
  sending audio (not a JTDX-VU fault). Changes nothing on the radio.

### 2026-10-03 — TCI reconnects automatically (v0.5.2)

Manoj: "why is tci not reconnecting after update?" - AetherSDR had been
restarted (12:42:56) and JTDX-VU never reconnects to TCI: stock JTDX's
`onDisconnected()` only clears a flag; the poll then fails, and
`rigFailure()` tries once at once (the SDR program isn't back yet) and
then shows the modal Rig Control Error box. He asked for auto-reconnect
in v0.5.2.

- `TCITransceiver::onDisconnected()`: an unexpected drop (`tci_Ready`)
  sets `error_` "TCI connection lost" if `onError` didn't, so the next
  poll reports it.
- `MainWindow::handle_transceiver_failure()`: for a TCI rig, no dialog -
  `m_tciReconnecting`, status "TCI: <reason> - reconnecting every 5 s",
  single-shot `m_tciRetryTimer` (5 s) → `rigOpen()`; a failed attempt
  comes back the same way. Skipped (re-armed) while a modal dialog such
  as Settings is open; stops if the rig is no longer TCI.
- `handle_transceiver_update()`: "TCI reconnected" on the first update
  that is online with a frequency. The update sent while going offline
  (frequency 0) must not count - it did in the first try and stopped the
  retries.
- **Verified** with new `tools/fake_tci.py` (stdlib websocket TCI
  simulator: start-up burst, echoes sets, answers queries; port 50099)
  and a throwaway `-r tcitest` instance (Rig TCI Client RX1, no TCI
  audio): server killed for 12 s → failure, retry refused, retry →
  connected 1 s after the server came back, 14.074 000 green, "TCI
  reconnected". Quit while retrying at 2, 5.3, 6.2 and 10.5 s after the
  drop: exit 0, no crash reports. Not yet tried with AetherSDR or TCI
  audio.
- Installed in /Applications 2026-10-03 (local build of `ea5630c3`,
  0.5.2); v0.5.2 re-cut on `ea5630c3` (first cut on `05360fe3` had only
  the CNS change, deleted after ~20 min with Manoj's OK).

### 2026-10-03 — CNS continues only after QSOs from our own CQ (v0.5.2)

Manoj: continuous QSOs should not happen unless we are CQing - not when
we answer others, and not in Fox/Hound-style special modes; "use 1 QSO
logic automatically for when I answer them".

- `m_cqRunQso`: set when the DX call comes from a station calling us -
  autoselect with a status other than RCQ/SCQ/SCALL, or a double-click
  on a decode that contains our call. Any other pick (their CQ, a
  third-party QSO, a typed call) leaves it false; a DX call change
  resets it (`on_dxCallEntry_textChanged`).
- `singleshot_now()` = 1 QSO, or CNS with a DX call not from our CQ.
  `process_Auto()` takes it once at the top (before clearDX) and uses
  it wherever it used `m_singleshot`; the readFromStdout 73 check too.
  So an answered QSO halts at the end (and on give-up) exactly as 1 QSO.
- `nonstop_active()` = CNS and not Hound (JTDX has no Fox mode; the
  SpecOp contest code is commented out, so Hound is the only special
  mode). Used for the continue branch, `nonstop_continue()` and the
  watchdog limit.
- While CNS is active the "call priority + search CQ" autoselect
  (`time=1`) is off, so a CQ run never jumps onto someone else's CQ.
- Builds clean; installed in /Applications 2026-10-03 (`4deff926`, still
  shows 0.5.1). **Not yet tested** (needs real callers / a CQ to answer).

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
  in `~/Desktop/jdxvu/prev-install/JTDX-VU.app.before-resize` (removed 2026-10-07)).
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
  .ini from before it is in `~/Desktop/jdxvu/prev-install/` (removed 2026-10-07).

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

- [ ] **v0.7.6 (2026-10-22):** rc7 (local) adds the Flex key-up / 14.100 fix - try it
      on the FLEX-6600 (watch `flex_trace.txt` for "xmit 1 not confirmed", "previous
      session ... closed it"); rc6 is the last pre-release. The Flex stalls stopped?
      (Mac mini back on its Trusted port 4 since 2026-10-21 22:25 UTC.) Waiting on KK4CDK to
      try rc5/rc6 (double-click keeps calling, LoTW Fetch Now). Manoj to try on air:
      India crossband (Misc menu, needs Split), Background highlight off on Tx lines
      and the marker cell, Ignore CALL, no repeat logging, TCI reconnect / refused
      frequency fixes. Release when Manoj says.
- [ ] **Issue #8 (WB4TOM, FLEX-8400 + RF2K-S): TX1 never keys the amp** in VITA-49
      mode (v0.7.5, Windows, SmartSDR 4.3.16 not running). RF goes out; SmartSDR MOX
      keys TX1 fine and TX1 is ticked on every band. His "works in WSJT-X /
      JTDX-improved" is through SmartSDR CAT + DAX, so SmartSDR was the GUI client
      there; in his JTDX-VU test JTDX-VU was the GUI client itself (`client gui`).
      Asked him (2026-10-29) for one FT8 Tx with SmartSDR open (JTDX-VU joins as a
      non-GUI client) and `flex_trace.txt` from both runs (File > Open log
      directory; it's under hidden %LOCALAPPDATA%\JTDX-VU, which is why he couldn't
      find it). Trace logs every radio line incl. `sub tx all`, so interlock state at
      key-up should be there. Cause unknown until the trace arrives.
- [ ] **Flex copies 3-4 in VITA-49 mode have no TX audio** on a real FLEX-6700
      (KK4CDK trace: own dax_tx stream stays tx=0). rc6 warns ("no TX audio"). Ideas
      not built: copy 3 sends on copy 1's stream id (unknown if the radio accepts it),
      or copies 3-4 relay audio to copy 1/2 over localhost, DAX-style. Miguel is fine
      with the limit for now (2026-10-20); SmartSDR+DAX works for four copies.
- [ ] **AetherSDR (dev build, local/dev-build):** its ~1.2 s main-thread stalls overflow
      the TCI RX audio backlog and drop the client (seen 2026-10-18 23:25 / 23:59).
      JTDX-VU now recovers; the stall itself is not reported upstream yet.

- [ ] **Issue #6 fix** (2026-10-15): unit test 5/5 and **passed in the app on
      fake TCI** (old code reproduces #6, fixed code keeps the report). Reply on
      #6 posted 2026-10-15 pointing to the rc5 pre-release. Released in
      v0.7.5 and #6 closed 2026-10-17; marinfyi has not yet confirmed on air.
- [x] **Update check in the real app** — TESTED by Manoj 2026-10-09 in the
      installed v0.7.3 ("mac expert, kst2mac, jtdx vu, mshv all tested").
      Was: (`e9796edb`, `0bd95894`, released in
      v0.7.3, installed 2026-10-09, not yet launched): tested only in a
      scratch program built from the same class. On the next launch see Help
      > Check for Updates... say "You're up to date", the Settings > General
      tick, and - once a newer release is out - the dialog ~10 s after start
      (or within an hour of it while running). The v0.7.3 notes describe the
      check. If a development version string is ever used, it must contain
      "dev" to keep automatic checks off.
- [x] **Website on v0.7.3** — DONE 2026-10-09 ~06:20 (vu2cpl.github.io
      `c926dc3`): card sentence, page paragraph "A check for new releases,
      %LOG and a steadier Auto CQ button (v0.7.3)", downloads table /
      release link / apt on v0.7.3 (all eight assets answered 206 first).
- [x] **Stale v0.1.0 on the .170 Windows PC** — REMOVED 2026-10-09 ~06:00 on
      Manoj's "remove the old JTDX-VU from windows". Note: it was deleted
      permanently, not recycled (over SSH the Recycle Bin call silently
      hard-deletes); the identical zip is on the v0.1.0 release and
      `%LOCALAPPDATA%\JTDX-VU` settings were untouched. Was:
      (`C:\JTDX-VU-0.1.0-windows-x64`,
      found 2026-10-09, unused since 09-28): Manoj to say upgrade, remove or
      leave.
- [x] **v0.7.2: hold until a few more features are in** (Manoj, 2026-10-08)
  — lifted the same morning ("cut a new release"); **RELEASED 2026-10-08**
  with the corrected FT2 credit in the notes.
- [x] **Tell Frank PH2M v0.7.2 is out** — SENT by Manoj 2026-10-08 (draft
  `~/projects/JTDX-records/jtdx-vu-v0.7.2-released-to-PH2M.txt`).
- [x] **Watch the Auto CQ countdown in FT8 while calling** (CNS on, Enable Tx
  on) — done 2026-10-08 on a test copy: it held whole minutes in the first
  half of each minute (Manoj saw "stuck at 5:00"); fixed in `e64be38d`,
  then seen running 4:48 -> 3:08 in 20 s steps.
- [x] **FT2 credit** (2026-10-08): wording fixed everywhere incl. the v0.7.0
  release notes; reuse it in future release notes. Martino IU8LMC replied
  with thanks (2026-10-08) - no further names asked for.
- [x] **Discover on the radio's own subnet** — CONFIRMED by Manoj 2026-10-13 at
      VU2OY's shack (radio on the same LAN): Discover found the radio and it
      worked. Was (v0.7.4): only proven by the
      parse loop in a scratch program against the 6600's captured broadcast;
      the Mac mini and the MacBook-over-VPN are both on other subnets. Try it
      from a machine on 192.168.1.x (or ubersdr-side) when convenient. Without
      a radio: `FAKE_DISCOVERY=1 tools/fake_flex.py --port 4993 --forever` sends
      the discovery packet to 127.0.0.1 every second. **Tested in the app
      2026-10-11** (local bundle, throwaway `-r disctest`): Discover listed
      `FLEX-6600 "FakeFlex" VU2CPL - 127.0.0.1:4993, 2 of 2 clients free`,
      filled the box, and Test CAT then went green with the fake radio logging
      the full start-up (sub, client udpport, slice create, dax streams). So
      only the real radio's broadcast on its own subnet is left. (Gotcha: a test
      ini without `PTTMethod` gives "Invalid PTT method" on Test CAT - VOX is
      greyed out for this rig; pick CAT.) (Asked about
      rimuadmin/ProtoFlex, a closed-source Windows-only Flex emulator for
      TeensyMaestro with discovery but no DAX audio: not used.)
- [x] **Intel start-up crash fix: RELEASED in v0.7.5** (2026-10-17; #5 closed).
      Was: verified 20/20 under Rosetta (2026-10-13).
- [ ] **ON HOLD (Manoj, 2026-10-14): a priority list and a blacklist like MSHV.**
      Not started beyond a first look. Starting points: the right-click queue
      (`QsoHistory::setQueue`, score 100000+) is the nearest thing to a manual
      priority; it works in every AutoSeq mode (not JTTY, no effect in WSPR) but has
      only been tested in FT8. Compare with MSHV's lists before designing; keep the
      stock JTDX settings as they are.
- [ ] **rc4 testers:** VU2OY asked (2026-10-14) to test #1 (AetherSDR) and #4
      (Flex over WireGuard) on the v0.7.5-rc4 pre-release; iw1gb told #2 is fixed.
      Waiting on VU2OY. Close #1, #2, #4, #5 when 0.7.5 is released.
- [x] **PR #3 (VU3ESV double-click fixes): try it in the app** - checklist
      PASSED 2026-10-14 on the rc4 bundle (see the PR #3 entry). Still: credit
      Vinod VU3ESV in the release notes.
- [x] **KK4CDK blank lines in Band Activity (Windows):** cause found and
      fixed (CRLF in the embedded US-state tables, 2026-10-13 entry);
      **confirmed by Miguel's screenshot 2026-10-15** - no blank rows.
- [ ] **Late Tx start cutoff (2026-10-15):** passed on fake TCI; in rc5. Manoj
      to mail Miguel the rc5 Windows link (draft in JTDX-records); then wait
      for his report.
- [ ] **Third/fourth VITA-49 copy on one radio** (KK4CDK): BUILT 2026-10-13
      (non-GUI `client bind` fallback, see entry); works on the fake radio;
      **to test on the 6600 with three copies**, then a Windows trial for
      Miguel's 6700 (four copies).
- [ ] **Red ERR box click against the real radio** (v0.7.4): tested only
      against `tools/fake_flex.py`.
- [ ] **FlexRadio VITA-49 rig type on the real FLEX-6600** (2026-10-07: connect,
      receive and DAX streams work on the radio; the band-change failure found
      there is fixed but not yet re-tried on the radio). Check: `client gui` with SmartSDR
      also connected (per-client slice letters), the RF level from full-scale
      DAX audio, that `xmit 0` drops the carrier at once, and a QSO.
- [x] **Tell Frank PH2M v0.7.0 is out** — emailed by Manoj 2026-10-07
      (text in `~/projects/JTDX-records/jtdx-vu-v0.7.0-reply-to-PH2M.txt`). His report
      came 2026-10-08 (buttons in use on IC-7300 / IC-9700; FT4 spots and 8m
      asked for, in v0.7.2); he replied again after the v0.7.2 note.

- [ ] **FT2 weak-signal gap vs MSHV** (2026-10-05 screen recording from
      LZ2HV, KN23, 14.084 FT2, JTDX-VU 0.6.0 beside MSHV 2.76.7, 15
      periods 18:37:52-18:38:45). MSHV 41 decodes, JTDX-VU 34. Everything
      above about -15 dB matched (dB / DT agree). Missed: 3 normal decodes at
      -16..-20 dB (CQ GM4VAC IO77 -16, SP2WEI UA3DIB R-04 -18, SP2WEI UA3DIB
      -06 -20) and 4 MSHV "AP7" decodes at -17..-21 dB (a priori from the
      callsigns of QSOs it is following: SP9FBN/UA3DIB, SQ1EIC/RK3AF). So:
      (1) the scaled-FT4 FT2 chain is a few dB short of MSHV's FT2 decoder
      (our simulation only went to -14 dB); (2) no AP for other stations'
      QSOs in FT2. Plan: Manoj saves MSHV FT2 WAVs (SAVE THIS / SAVE
      PREVIOUS) on a busy band and shares them; then decode them offline in
      both and compare with MSHV's FT2 decoder source (LZ2HV, credit),
      tuning thresholds / passes and adding QSO-tracking AP.
      **2026-10-06 step 1 measured - the hard gates are NOT the loss.**
      Bench: `ft2sim` (fading off) -> `ft2dec`, a FIXED set of 30 files per
      SNR (ft2sim reseeds from time+PID, so regenerating per run gave
      run-to-run noise bigger than the effect; the first "worse" result was
      that). Baseline: -13 30/30, -14 27, -15 7, -16 2, -17 1, -18 0 (the
      -14 dB cliff from 09-29 confirmed). Lowering syncmin 1.2->0.8, smax
      1.2->0.65, nsync_qual 20->13, badsync 8->7, and the return->cycle
      give-up fix - all together and each alone - decoded EXACTLY the same
      files. No false decodes either (0/10 noise-only). So on a clean signal
      every gate passes and the LDPC/demod is what fails; MSHV's deeper
      reach must come from its demapper / channel estimation / extra LLR
      sets and passes, and on air from AP7. Revised order: (1) MSHV's
      log-sum-exp demapper with noise-variance beta + LLR sets D/E and 5
      passes; (2) AP7; (3) the gates only matter on crowded bands (keep the
      return->cycle fix, it is harmless). Source left clean (gates at FT4
      values). Bench scripts in the session scratchpad (regenerate: ft2sim
      ... 30 files per SNR into fixed/snr-NN/).
      **2026-10-06, later: JTDX's fading simulation never worked.** Chasing
      "FT2 fails under the mildest fading" (a 0 dB signal: no decode with
      ft2sim fdop 0.1 / delay 0.5) led to the coarse candidate search, which
      showed the file held NO signal, only noise. Cause: `ft8sim`, `ft4sim`
      and our `ft2sim` copy call `watterson(c,NZZ,NWAVE,fs,delay,fspread)`
      but `lib/watterson.f90` takes `(c,npts,fs,delay,fspread)` - the extra
      NWAVE shifts fs/delay/fspread one slot (delay becomes 12000 ms) and the
      output is noise. Fixed in ft2sim only (`2520b643`); a 0 dB signal now
      decodes under 2 Hz / 2 ms. The upstream ft4sim/ft8sim have the same
      bug and are left alone (upstream is dormant; nobody has reported it).
      So the earlier "faded bench: 0 decodes everywhere" rows are void, the
      decoder is NOT fading-fragile, and the clean-channel results stand.
      Fading bench rerun with the fixed simulator (0.5 Hz, 1 ms, 30/SNR):
      see the next note.
      **Fading bench (fixed simulator, 0.5 Hz / 1 ms, 30 files per SNR):**
      baseline -10 30, -12 30, -13 27, -14 24, -15 21, -16 13; the lowered
      gates again identical to baseline (closed for good); **MSHV's LSE
      demapper -14 29, -15 24, -16 12** - the +0.5-1 dB LZ2HV's comment
      claims, visible only under fading (flat channel: no change, the
      demod is already near-optimum there). Note the baseline decodes
      deeper under fading (-16: 13 vs 2 flat) because the 2/4-symbol
      coherent combining recovers spread energy - which is why the flat
      bench hid everything. Demapper committed (`get_ft2_bitmetrics.f90`:
      power |s|^2, noise variance from the 48 off-tone Costas bins ->
      beta=0.5/var clamped 0.01..50, beta/nsym for coherent sums, exact
      log-sum-exp per bit; normalizebmet unchanged). Checks: strong
      signals +20/0/-10 dB decode with correct SNR, 0/10 false decodes on
      noise, 10/10 message types exact at -8 dB faded. Next, in order:
      (1) LLR sets D (max-|LLR| of A/B/C) and E (mean) + 5 non-AP passes;
      (2) MSHV's channel estimation / MMSE equalisation blend (fading>6 dB);
      (3) AP7. All measurable on the fading bench. Bench scripts and the
      AP7 unit tools now live in `lib/ft2/tools/bench/` (README there);
      regenerate the file sets with the FIXED ft2sim.
      **LLR sets D/E + 5 non-AP passes (MSHV) - measured neutral.** Fading:
      -14 29 (same), -15 25 (was 24), -16 12 (same); flat: -14 27, -16 2 (one
      file back each). Within the noise of 30 files, 0/10 false decodes,
      6/6 message types exact. Kept (MSHV runs it and its equalisation
      blend writes into these sets) but NOT claimed as a gain. The two big
      MSHV pieces still missing - channel estimation / MMSE equalisation
      (built for fading, where the demapper gain showed) and AP7 - are
      what to measure next, not more LLR variants.
      **Channel estimation / MMSE equalisation (MSHV) - measured neutral on
      three channels.** Flat: -14 26 (lse 26), -16 2; slow fade 0.5 Hz/1 ms:
      -14 28, -15 24, -16 12 (vs 29/25/12 without); fast fade 2 Hz/2 ms:
      -13 27, -14 24, -15 15 (vs 27/23/16 without). Its 6 dB fading gate
      opens on 30/30 files INCLUDING the flat channel (12-18 dB "depth" at
      -14 dB is noise scatter in the 16-symbol per-symbol SNR estimate), so
      it was active throughout and still changed nothing: a 16-pilot
      estimate at these SNRs carries too little to equalise with. Kept as
      a faithful port (harmless, 0/10 false, 6/6 exact under 1 Hz/2 ms),
      not claimed. Fast-fade bench shows the demapper+passes work IS a
      real gain there: baseline -13 24, -14 22, -15 12 -> 27/23/16.
      Sensitivity work on simulation is now done.
      **AP7 ported 2026-10-06** (see "What changed"): the -18 dB reports
      in a seeded QSO decode where the normal path stops at about -16,
      0 false decodes on noise. What is left is the on-air comparison
      against MSHV with real FT2 recordings.
      **2026-10-07: same-audio comparison automated and run** (`tools/ft2_autotest.sh`):
      the plain decoders have the same floor; two AP7 porting bugs fixed;
      live 20-QSO result 106/107, 47/61, 8/11 at -14/-15/-16 dB (JTDX-VU /
      MSHV). Remaining: the on-air check with real recordings, and the -15 dB
      AP7 difference, which may be run-to-run variation (MSHV alone 43-66).
- [x] **DECIDED 2026-10-10: not doing it.** Manoj: JTDX users are used to the
      Notifications settings, so JTDX-VU keeps them; the simple, personalised
      DXCC model stays in the private MSHV-Mac build. Was: **simplify to
      MSHV's DXCC model.** MSHV-Mac private build: one checkbox "DXCC: show
      new entity / band / mode only" above the decodes; one Club Log status
      (ATNO / need band / need mode / none) drives colour, filter and alerts;
      nothing chosen twice. Proposal for JTDX-VU: drop the CQZ / ITUZ / grid /
      prefix / call rows (24 ticks, 10 colour buttons) from Settings >
      Notifications; DXCC new / band / mode colours always on; Show dropdown
      -> one "New DXCC / band / mode only" tick box using
      `DisplayText::wanted()`, the same test as AutoSeq wanted-first; drop
      worked-B4 "Don't show". Keep CQ/MyCall/Tx colours, markers, beeps,
      worked-B4 colour/strike, and the Wanted call/prefix/country lists.
- [ ] **AutoSeq queue + current-decode rule + JTTY heard** (unreleased,
      2026-10-03): try right-click Queue in the GUI and on air; check
      the JTTY calls-heard list on live traffic.
- [ ] **AutoSeq wanted-first** (unreleased, 2026-10-03): watch on air
      that a new band/mode DXCC is picked over stronger ordinary CQs and
      callers, and that it stops being picked once logged.
- [ ] **Port wanted-first AutoSeq to MSHV-Mac private** (Manoj's TODO,
      after this ships in JTDX-VU). MSHV has no QsoHistory; its own
      auto-answer choice needs the same DXCC > band > mode ranking.

- [ ] **Decoder speed (2026-10-10 measurements):** decide (a) whether to drop
      `-fbounds-check` from the Release Fortran flags (Mac −20%, Pi −6%,
      decodes unchanged within noise, but out-of-range bugs no longer stop the
      decoder), and (b) whether to recommend FT8 threads = 4 on a Pi, after a
      try in the running app on the Pi. Pi `~/bench` can go once decided.
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
      2026-10-05: Manoj asked for all JTTY QSOs as MODE MFSK / SUBMODE
      JTTY. JTDX-VU's own `wsjtx_log.adi`: the 7 pre-v0.4.0 records (mode
      JTTY, 09-28..09-30) changed to MFSK/JTTY, all 11 now MFSK/JTTY; backup
      `wsjtx_log.before-jtty-mfsk-20261005.adi`. `~/projects/JTDX-records/jtty-qsos-mfsk.adi`
      holds the 11 corrected records for RUMlog / Club Log. RUMlog itself
      not touched (its DB is unreadable from here) - see the caveat above.
      Manoj's RUMlog export (11 QSOs) shows RUMlog writes `<mode:4>JTTY
      <submode:4>JTTY` for all of them, the 4 MFSK-logged ones included, so
      the caveat was wrong: RUMlog itself needs MODE MFSK. Also EA1BAF has
      IK3CHK's grid JN55UR and N8DC has EA3NE's JN11AN (old JTTY bug: grid
      not cleared). All 11 already marked sent to LoTW / eQSL / Club Log.
      `~/projects/JTDX-records/jtty-qsos-mfsk.adi` was rebuilt from the RUMlog export
      (only mode JTTY -> MFSK; RUMlog's correct times kept) - the first
      version came from JTDX-VU's log, whose early start times are wrong.
      **Done 2026-10-06 (Manoj): the 11 QSOs changed to MFSK/JTTY in RUMlog
      and the EA1BAF / N8DC grids corrected by hand.** BUT on 2026-10-09
      RUMlog's ReadAdif export shows all 11 (and the 6 MSHV ones of 10-08)
      as `MODE=JTTY SUBMODE=JTTY` again, while FT4 reads MFSK/FT4: RUMlog
      6.5.1 keeps JTTY as its own mode whatever is imported or edited. Needs
      a RUMlog mode mapping or Tom DL2RUM adding JTTY → MFSK; re-importing
      does nothing.
- [ ] **JTTY start-time fix** installed 2026-10-01; check the next
      JTTY QSO logs a sensible start time.
- [ ] **JTTY screen redesign (2026-10-02):** installed; try on air -
      Log QSO with edited S / R / Name, calls-heard colours with live
      decodes, Shift / Option F-keys on the Mac keyboard, type-ahead
      with PTT through TCI. Manoj to fill labels for his own "Default"
      set in Settings > JTTY if he uses it.
- [ ] **Even decode-pane split + resize fix:** released in v0.5.1,
      Manoj: "its fine now". Check
      the minimum on a Linux / Windows font.
- [ ] **TCI audio watchdog** (v0.5.2): reconnect and recovery confirmed
      live with AetherSDR 2026-10-03; still watch for false "no audio"
      alarms during long JTTY sends or band changes.
- [ ] **AetherSDR TCI audio bug** is upstream
      [aethersdr/AetherSDR#6006](https://github.com/aethersdr/AetherSDR/issues/6006)
      (26.9.5 regression). Fix PR
      [aethersdr/AetherSDR#6116](https://github.com/aethersdr/AetherSDR/pull/6116)
      (skerker, `refreshRxBindings()` in `onDaxStreamUnregistered()`),
      **merged 2026-10-04**; in AetherSDR's next release. **Tested here 2026-10-04** on a local build of
      `c6103737` (FLEX-6600, macOS 26): probe got ~1.15 MB / 3 s after
      each of three DAX releases; JTDX-VU closed/reopened past the grace
      decoded 33 FT8 messages on 15 m. Result posted on the PR as Manoj.
      Manoj is back on the installed 26.9.5 until the fix ships. When it does,
      check TCI audio survives a JTDX-VU close/reopen. Notes in
      `docs/aethersdr-tci-audio-issue.md`.
- [ ] **CNS only after our own CQ** (v0.5.2): check on
      air - a CQ run continues; answering a CQ halts after that QSO;
      Hound halts as before.
- [ ] **Auto CQ time limit:** released in v0.5.1;
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

- **Fortran hidden string lengths are `size_t` (gfortran 8+).** Any new
  `extern "C"` declaration of a Fortran routine with CHARACTER arguments
  must use `fortran_charlen_t` for the hidden lengths, and that type comes
  from CMake (`WSJT_FORTRAN_CHARLEN_SIZE_T`), not `__GNUC__` (clang says 4).
  Getting it wrong on macOS gives random lengths and Fortran runtime errors
  that quit the app with exit code 2 and no crash report (2026-10-19).

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
