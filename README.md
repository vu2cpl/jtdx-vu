# JTDX-VU for VUCG community — v0.4.0

VU2CPL's build of JTDX 2.2.159 for the VUCG community: macOS (Apple
Silicon and Intel), Windows x64 and Raspberry Pi / Linux arm64. It runs
as its own app alongside a stock JTDX install,
and it adds Club Log DXCC status and needed-DXCC alerts.

**Versioning:** JTDX-VU has its own version, set as `JTDXVU_VERSION` in
`Versions.cmake`. That version appears in the title, the About box,
Info.plist, PSK Reporter, ALL.TXT and UDP messages. The upstream base,
JTDX 2.2.159 (`version()`), is still shown in the title and About box,
and is still reported to WSPRnet.

JTDX is by Igor Chernikov UA3DJY, Arvo Järve ES1JA and the HF community,
upstream at [jtdx-project/jtdx](https://github.com/jtdx-project/jtdx).
JTDX itself is derived from WSJT-X by Joe Taylor K1JT, Bill Somerville
G4WJS, Steve Franke K9AN, Nico Palermo IV3NWV and others. The original
`README` and `INSTALL` files are kept unchanged. Licence: GPL v3
(`COPYING`).

The Club Log status and alerts design is ported from the MSHV-Mac
private build.

## Downloads

Get builds from [Releases](https://github.com/vu2cpl/jtdx-vu/releases):

| Platform | File | Notes |
|---|---|---|
| macOS, Apple Silicon | `JTDX-VU-<ver>-macos-arm64.zip` | macOS 14+. Unzip, move to Applications, then right-click > Open the first time (not notarized). Needs the shared-memory step below. |
| macOS, Intel | `JTDX-VU-<ver>-macos-x86_64.zip` | macOS 15+, same steps. |
| Windows x64 | `JTDX-VU-<ver>-windows-x64.zip` | Unzip anywhere and run `bin\jtdx.exe`. |
| Raspberry Pi / Linux arm64 | `jtdx-vu-<ver>-linux-arm64.deb` | Raspberry Pi OS / Debian 12 (bookworm): `sudo apt install ./jtdx-vu-<ver>-linux-arm64.deb`. Conflicts with a stock `jtdx` package. |

## What's different from stock JTDX

- **Native build on current macOS.** Builds with Homebrew Qt 5.15,
  Hamlib 4.7 and gfortran 16. `macos-bundle.sh` produces an app that
  does not depend on Homebrew at run time.
- **Its own identity.** The app, window titles, dialogs and PSK
  Reporter ID all say JTDX-VU. The title bar reads just "JTDX-VU for
  VUCG V0.4.0"; the JTDX / WSJT-X base and credits are in Help >
  About. Settings are in
  `~/Library/Preferences/JTDX-VU.ini` and data in
  `~/Library/Application Support/JTDX-VU`.
  - On first launch, JTDX-VU copies the stock JTDX settings and data
    across and repoints the paths in the .ini. The stock files are only
    read.
- **Club Log worked-before source** (File > Club Log & Alerts...). Your
  Club Log log replaces `wsjtx_log.adi` as the source for JTDX's own
  New DXCC / New Band / New Band+Mode colours, filters and auto-reply
  priorities.
  - Local QSOs from 7 days before the last fetch onward are counted as
    well, so a just-worked station doesn't show as new again.
  - Optional: count only QSOs confirmed by QSL, LoTW or eQSL (`Y` or
    `V`).
  - The log is fetched at start-up when the cached copy is more than
    24 h old, or on demand with "Refresh now".
- **Band and mode buttons** (after the MSHV switcher by LZ2HV). Two
  a single compact row above the decode panes (modes | bands | Show filter): one click changes mode, or moves to a
  band's working frequency for the current mode. The current band and
  mode are shown green. Choose which buttons appear in View > Band &
  Mode Buttons....
- **Show only needed decodes.** The "Show:" selector works as in MSHV:
  **All**, **New DXCC** (entity never worked), **New band** (not
  worked on this band) or **New mode** (not worked in this mode on any
  band). Each option is its own test, and an ATNO counts for all three.
  It uses Club Log when enabled, turns amber while active, and is live:
  changing it re-filters every decode already on screen, and a
  just-worked station drops out. Traffic with your call and your QSO
  partner always shows, and the Rx Frequency pane is never filtered.
- **Call Non-Stop:** the **Auto CQ** button in the right-hand button
  column (under AnsB4), also "CNS" in the AutoSeq menu. After each logged QSO, JTDX-VU goes straight back to CQ
  or the next caller instead of halting Tx. A station that doesn't
  answer is dropped after the AutoSeq counters (Settings > Sequencing),
  as usual. The Tx watchdog is fixed at 10 minutes while CNS is on, so
  unanswered CQ still stops.
- **No Stop button:** Monitor is the one start/stop control for
  monitoring (switching it off also ends a "Decode remaining files" run).
- **Needed-DXCC alerts** when a decoded station is ATNO, a new band or
  a new mode (in this mode on no band yet), as in MSHV. Alerts go to macOS notifications and/or Telegram,
  with a per-call/band cooldown (5–60 min).
  - The Club Log app password and the Telegram bot token are stored in
    the macOS Keychain (service `JTDX-VU`), never in the .ini.
- **JTTY** (new in v0.3.0): WSJT-X 3.2's
  period-free RTTY-style keyboard mode, ported from 3.2.0-rc1 — decoder,
  transmit, macros, Call next and Serial Number. Click, double-click or
  select a callsign in either decode pane to put it in DX Call, as in
  N1MM; punctuation around it is ignored.
  - **A JTTY screen of its own** (after v0.4.0): the FT-only controls
    (Report, CL, Hound, AutoTX, AutoSeq, Wanted and the wanted filters,
    Bypass, 1 QSO, AnsB4, Enable Tx, Hint, SWL, AGCc, Filter, Decode,
    TX Even, the period progress bar) are hidden while JTTY is selected
    and come back in the other modes. The rest is regrouped: all the
    buttons (Tune, Monitor, Auto CQ, Halt Tx, Log QSO, Erase, Clear DX)
    in the right-hand column, the macro panel across the bottom, and
    **S / R / Name** QSO fields in a column beside Tx / Rx (RST sent and received, logged instead of a
    fixed 599; the name comes from the log when known), and a **Calls
    heard** list of every call in the JTTY decodes, coloured like Band
    Activity from the log / Club Log (new DXCC, new on the band, new in
    JTTY, new call, worked). Click a call to make it DX Call and put Rx
    on its frequency.
  - **24 macros in three banks:** F1–F8, Shift+F1–F8 and Option
    (Alt)+F1–F8, shown eight at a time as labelled buttons (Bank 1/2/3);
    each key works whichever bank is on screen. A **Set** picker on the
    panel switches macro sets. Macros and their button labels are edited
    in Settings > JTTY.
  - **Type-ahead:** a macro or typed message sent while another is on the
    air is queued ("Next: …" on the panel) and goes out as soon as the
    current one ends. Halt Tx or Esc clears the queue.
  - **Variables:** %M my call, %H DX call, %Q Call next, %N serial, %E
    the set's exchange, %RST the RST sent, %NAME his name, and station
    details %OP (name), %QTH, %TX (radio), %ANT (antenna). Set Serial
    Number to "none" for a non-contest QSO and %N is left out of %E.
  - **Auto CQ** (right-hand button column) repeats a macro with a gap
    after each call, 10 s by default; pressing it again turns it off and
    halts a CQ on the air (Esc and Halt Tx stop any JTTY send). In the FT
    modes the same button is Call Non-Stop. Settings > JTTY sets the
    macro, gap, call limit and stop-on-my-call, the station details, and
    named **macro sets**, each with its own %E exchange ("Contest
    (WSJT-X)" sends %RST %N, "Ragchew / DX" sends %RST). Serial Number
    shows only when the active set uses %N.
  - JTTY QSOs are logged as ADIF MODE MFSK / SUBMODE JTTY, like FT4.

  JTTY is by Joe Taylor K1JT, Steve Franke K9AN, Rob G4KLA and the
  WSJT-X team.
- **FT2** (new in v0.3.0): FT4's protocol at twice
  the speed — 3.75 s periods, 41.67 baud — as introduced in MSHV. The
  mode was created by Martino IU8LMC (ARI Caserta); MSHV's C++
  implementation is by Hrisimir LZ2HV. JTDX-VU decodes and transmits it
  with a scaled copy of JTDX's own FT4 chain. Default dial frequencies
  follow MSHV (14.084, 7.052, 21.144 MHz …). Add the FT2 button in
  View > Band & Mode Buttons... if you want it on the switcher.

## Build on other platforms

- **Windows and macOS release builds** come from GitHub Actions
  (`.github/workflows/windows.yml`, `macos.yml`). Run them from the
  Actions tab, or push a `v*` tag to attach the builds to that release.
- **Raspberry Pi / Debian 12:**
  `sudo apt install qtbase5-dev qtmultimedia5-dev libqt5websockets5-dev libqt5serialport5-dev libhamlib-dev libhamlib-utils libfftw3-dev gfortran qttools5-dev-tools qttools5-dev libqt5svg5-dev cmake`,
  then
  `cmake .. -DCMAKE_BUILD_TYPE=Release -DWSJT_GENERATE_DOCS=OFF -DWSJT_SKIP_MANPAGES=ON -DCMAKE_INSTALL_PREFIX=/usr/local && make -j3 && cpack -G DEB`.

## Build (macOS, local)

```bash
brew install qt@5 hamlib fftw gcc boost libusb cmake
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
  -DCMAKE_PREFIX_PATH="/opt/homebrew/opt/qt@5;/opt/homebrew" \
  -DCMAKE_Fortran_COMPILER=/opt/homebrew/bin/gfortran \
  -DWSJT_GENERATE_DOCS=OFF -DWSJT_SKIP_MANPAGES=ON \
  -DCMAKE_INSTALL_PREFIX=$PWD/dist
cmake --build . -j12
cd .. && ./macos-bundle.sh      # -> build/bundle/JTDX-VU.app
```

Install with `ditto build/bundle/JTDX-VU.app /Applications/JTDX-VU.app`
after quitting the running copy.

**Shared-memory limit (one-time).** The decoder needs a SysV
shared-memory segment. Without it, JTDX-VU fails with "Unable to create
shared memory segment". JTDX-VU needs 13.7 MB, and the stock JTDX
2.2.159-32A build needs 16.6 MB. The macOS default is 4 MB. Raise the
limit once, sized so both can run side by side:

```bash
sudo sysctl -w kern.sysv.shmall=32768 kern.sysv.shmmax=33554432
sudo cp Darwin/com.jtdx.sysctl.plist /Library/LaunchDaemons/
```
