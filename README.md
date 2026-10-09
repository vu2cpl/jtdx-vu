# JTDX-VU for VUCG community — v0.7.3

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

| Platform | File | Runs on |
|---|---|---|
| macOS, Apple Silicon | `JTDX-VU-<ver>-macos-arm64.zip` | macOS 14 Sonoma or later |
| macOS, Intel | `JTDX-VU-<ver>-macos-x86_64.zip` | macOS 15 Sequoia or later |
| Windows x64 | `JTDX-VU-<ver>-windows-x64.zip` | Windows 10 / 11, 64-bit |
| Raspberry Pi / Linux arm64 | `jtdx-vu-<ver>-linux-arm64.deb` | Raspberry Pi OS / Debian 12 (bookworm), 64-bit |

Each file has a matching `.sha256` checksum. JTDX-VU installs alongside a
stock JTDX on macOS and Windows; on the Pi the package replaces a stock
`jtdx` package.

## How to install

### macOS (Apple Silicon or Intel)

1. Download the zip for your Mac (Apple menu > About This Mac: "Apple
   M…" chip = arm64, "Intel" = x86_64) and double-click it to unzip.
2. Drag **JTDX-VU** into **Applications**.
3. **First launch.** The app is ad-hoc signed, not notarized, so macOS
   blocks the first open:
   - macOS 15 Sequoia and later: double-click JTDX-VU, click **Done** on
     the warning, then open **System Settings > Privacy & Security**,
     scroll down and click **Open Anyway** next to "JTDX-VU was
     blocked", and confirm.
   - macOS 14 Sonoma: right-click (Control-click) JTDX-VU > **Open** >
     **Open**.
   - Or, in Terminal: `xattr -dr com.apple.quarantine /Applications/JTDX-VU.app`
4. **Allow the microphone** when asked; that is how macOS names access to
   your radio's sound card. If you clicked Don't Allow, turn JTDX-VU on in
   System Settings > Privacy & Security > Microphone.
5. **Shared memory (one-time, every Mac).** The decoder needs more SysV
   shared memory than the macOS default; without it JTDX-VU stops with
   "Unable to create shared memory segment". In Terminal:
   ```bash
   sudo sysctl -w kern.sysv.shmall=32768 kern.sysv.shmmax=33554432
   ```
   That lasts until the next restart. To make it permanent, download
   [`Darwin/com.jtdx.sysctl.plist`](Darwin/com.jtdx.sysctl.plist) from
   this repo and run
   `sudo cp com.jtdx.sysctl.plist /Library/LaunchDaemons/`.
   It may need redoing after a macOS upgrade.
6. Set up your callsign, radio and audio in **JTDX-VU > Settings…**
   (Cmd+,).

**Upgrading:** quit JTDX-VU, replace the app in Applications with the
new one, and repeat step 3 if macOS asks. Settings
(`~/Library/Preferences/JTDX-VU.ini`) and data
(`~/Library/Application Support/JTDX-VU`) are kept.

### Windows 10 / 11 (64-bit)

1. Download `JTDX-VU-<ver>-windows-x64.zip`. Before unzipping,
   right-click the zip > **Properties**, tick **Unblock**, OK; this saves
   a warning for every file inside.
2. Right-click > **Extract All…** to a folder of your choice, for example
   `C:\JTDX-VU`. No installer and no administrator rights are needed.
3. Run **`bin\jtdx.exe`** in that folder. If Windows SmartScreen says
   "Windows protected your PC", click **More info > Run anyway** (the
   program is not code-signed). Right-click `jtdx.exe` > Send to >
   Desktop (create shortcut) for a desktop icon.
4. If Windows Firewall asks, allow access on private networks (JTDX-VU
   talks UDP to logging programs such as Log4OM, N1MM or JTAlert).
5. For rig control through OmniRig, install
   [OmniRig](https://www.dxatlas.com/OmniRig/) separately. Hamlib, TCI
   and the other rig interfaces are built in.

**Upgrading:** quit JTDX-VU and extract the new zip over the old folder
(or into a new one). Settings are stored in your Windows profile, not
in the program folder, so they are kept.

### Raspberry Pi / Debian 12 (arm64)

Needs a 64-bit Raspberry Pi OS (Bookworm) or Debian 12 on arm64 - a Pi 4
or Pi 5 is recommended.

1. Download `jtdx-vu-<ver>-linux-arm64.deb`, then in a terminal in the
   download folder:
   ```bash
   sudo apt update
   sudo apt install ./jtdx-vu-<ver>-linux-arm64.deb
   ```
   Keep the `./` - it tells apt to install the file rather than look for
   a package by that name. apt pulls in Qt, Hamlib and FFTW. A stock
   `jtdx` package can't be installed alongside; apt offers to remove it.
2. For CAT control over a USB serial cable, give your user access to
   serial ports once, then log out and back in:
   ```bash
   sudo usermod -aG dialout $USER
   ```
3. Start **JTDX-VU** from the menu (Sound & Video / Ham Radio), or run
   `jtdx` in a terminal.

**Upgrading:** install the new `.deb` the same way; settings are kept.
**Removing:** `sudo apt remove jtdx-vu`.

## Common problems

- **"Unable to create shared memory segment" (macOS).** The one-time
  shared-memory setting is missing on this Mac. See step 5 of the macOS
  install above.
- **The same message decoded at several frequencies**, 50 or 100 Hz apart
  and weaker than the real one. The receive audio is too loud and is
  clipping, which turns any mains hum into copies of every strong signal.
  Turn the receive audio down at the source (the rig's USB audio level,
  or SDR-Control) until the level meter reads about 30–40 dB on a quiet
  band. If copies remain at that level, look for a hum source, such as a
  laptop charger causing a ground loop. Unlike MSHV, JTDX shows the same
  message again when it is decoded more than 45 Hz away. Tick
  **Misc > Hide FT8 dupe messages** to drop weaker repeats instead. It is
  saved per profile, and a level that is too high still costs weak decodes.
- **A menu is missing (macOS laptops).** On a MacBook with a notch,
  macOS hides app menus that do not fit beside the menu-bar icons, and
  JTDX-VU has many menus (Misc, Profile, Language, Help are the first to
  go). Quit some menu-bar apps, or pick a "More Space" resolution in
  System Settings > Displays, and the menus come back.

## What's different from stock JTDX

- **Native build on current macOS.** Builds with Homebrew Qt 5.15,
  Hamlib 4.7 and gfortran 16. `macos-bundle.sh` produces an app that
  does not depend on Homebrew at run time.
- **Its own identity.** The app, window titles, dialogs and PSK
  Reporter ID all say JTDX-VU. The title bar reads just "JTDX-VU for
  VUCG V0.7.3"; the JTDX / WSJT-X base and credits are in Help >
  About. Settings are in
  `~/Library/Preferences/JTDX-VU.ini` and data in
  `~/Library/Application Support/JTDX-VU`.
  - On first launch, JTDX-VU copies the stock JTDX settings and data
    across and repoints the paths in the .ini. The stock files are only
    read.
- **Tells you when a new release is out** (since v0.7.3). Once a
  day JTDX-VU asks GitHub for the latest JTDX-VU release: about 10 s after
  start, or later while it stays open, whenever a day has passed since the
  last check that got an answer. If it is newer than yours, a window shows
  its release notes with **Download** (opens the release page in your
  browser), **Skip This Version** and **Remind Me Later**; otherwise it
  says nothing. When GitHub can't be reached it also says nothing, and
  tries again an hour later and at the next start. Nothing is downloaded
  or installed. **Help > Check for Updates...** checks at any time and
  always says what it found. Turn the daily check off with Settings >
  General > "Check for updates automatically"; development builds (a
  version with "dev" in it) only check from the Help menu. The only
  request is one anonymous `GET` to `api.github.com`
  (`/repos/vu2cpl/jtdx-vu/releases/latest`); no other server is involved.
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
- **Show only needed decodes.** A **New only** tick above the decodes
  (since v0.6.0; a four-way "Show:" dropdown before) hides
  everything that doesn't get a "new" colour from Settings > Notifications -
  so what counts as new (DXCC, on this band, in this mode, zones, grid,
  prefix, call) is chosen once, there. It uses Club Log when enabled, turns
  amber while active, and is live: ticking it re-filters every decode
  already on screen, and a just-worked station drops out. Traffic with your
  call and your QSO partner always shows, and the Rx Frequency pane is never
  filtered.
- **Notifications page redesigned** (since v0.6.0): one row per kind of
  "new" in priority order, each with its band / mode checks, colour squares
  and beep on one line, plus "Already worked", "Messages and markers" and a
  live preview.
- **AutoSeq goes for what your log needs first** (since v0.6.0). Always
  on, no switch: when picking a CQ to answer, and when choosing between
  stations calling your CQ, AutoSeq takes a **new DXCC** first, then a
  DXCC **new on this band**, then one **new in this mode** (the same
  tests as the Show selector, from Club Log when enabled), and only then
  the usual JTDX order. A wanted station is picked even when AnsB4 /
  CallB4 are off; with none heard, nothing changes.
- **Queue a station for AutoSeq** (since v0.6.0). Right-click a decode
  in either pane > **Queue CALL**. AutoSeq calls it the next time it is
  decoded (its CQ, or the end of its QSO), ahead of wanted stations and
  callers, in queue order, and takes it off the queue once logged. The
  queue shows above the Rx Frequency pane while it has calls; right-click
  there (or the decode again) to remove one, or clear it. A queued QSO
  counts as your own run, so CNS carries on afterwards. FT modes only.
- **AutoSeq calls only stations decoded right now** (since v0.6.0). JTDX
  could go back to a station that called your CQ up to 5 minutes ago and
  isn't heard any more; now every AutoSeq pick must be in the latest
  decode period.
- **UDP errors on the status bar** (since v0.6.0). A wrong or unreachable
  UDP server (Settings > Reporting) used to pop up a "Network Error" box on
  every send; now the status bar says "UDP host:port: ... - check Settings >
  Reporting", and a failed name lookup is retried every 30 s.
- **70cm and 23cm band buttons** (since v0.7.0, asked for by Frank PH2M
  for the European UHF/SHF FT8/FT4 activity). Tick them in View > Band &
  Mode Buttons... as for 4m and 2m. A 23cm FT8 working frequency
  (1296.174 MHz, as in WSJT-X) is in the default frequency list for new
  or reset lists; 70cm already had 432.174. Since v0.7.2, FT4 has its
  own spots there too (432.170 and 1296.170 MHz, as
  PH2M uses them), and there is an **8m** button (FT8 on 40.680 MHz,
  Region 1) for stations with an 8m permit. These rows are added once to
  an existing frequency list on first start; no reset needed.
- **QO-100** (since v0.7.0). A **QO-100** band button tunes FT8 on the
  satellite (10489.540 MHz downlink / 2400.040 MHz uplink). QSOs made on the
  transponder log as a satellite QSO, as LoTW wants: FREQ / BAND = uplink
  (13cm), FREQ_RX / BAND_RX = downlink (3cm), PROP_MODE SAT, SAT_NAME
  QO-100, SAT_MODE SX - as MSHV logs them - whichever side the radio's dial
  shows. New DXCC / band colours check against 13cm, the band these QSOs are
  logged on. RUMlog drops satellite fields from the usual WSJT-X UDP path, so
  with a secondary UDP server set (e.g. a RUMlog bridge) a QO-100 QSO is sent
  only as ADIF there.
- **TX and RX sliders** (since v0.6.0). The Pwr slider is now labelled
  **TX**. With TCI audio it shows the SDR program's own drive setting and
  moves with it, so nothing changes the radio's power until you move it
  (it used to show 0 after a start, and moving it set the SDR program's RF
  power). With sound card audio it is the transmit audio level as before,
  now shown at its saved level from the start. A new **RX** slider beside
  it raises or lowers the received audio going to the decoder and the
  level meter (-20 to +20 dB, 0 = as received), for any radio.
- **Choose which wanted filters show** (since v0.6.0): View > Wanted
  filters (or right-click a filter's label) shows or hides Callsign,
  Prefix, Grid and Country one by one, to give the Rx window more room. A
  hidden filter with text in it still applies.
- **A new band starts with no automatic calling** (since v0.5.2).
  Changing band - from the band buttons or combo box, or on the radio /
  SDR program - switches **Auto CQ** off, and the status bar says so.
  **AnsB4** and **1 QSO** stay as you set them (they were switched off too
  up to v0.5.3); a mode change leaves all of them alone.
- **No crash while TCI is reconnecting** (since v0.7.0). A frequency, PTT
  or rig restart arriving while the TCI link was still connecting could
  crash JTDX-VU (same fault as the quit crash fixed in v0.4.0, now closed for every
  path).
- **Rig mode shown** (since v0.7.0). The round rig-status light left of the
  frequency is now a box showing the mode the rig reports (DIGU, USB,
  LSB...), in the frequency's font: green when it is the mode set in
  Settings > Radio, yellow when not - a radio left in LSB after a band change
  stands out. Orange while connecting, red ERR on a rig control failure
  (click it to reset, as before). Works in light and dark style.
- **FlexRadio over VITA-49, no SmartSDR needed** (since v0.7.0). Rig *FlexRadio VITA-49 Slice A..H* with the radio's
  address (port 4992) in the CAT port field talks SmartSDR straight to a
  FLEX-6000/8000: it takes or creates a slice, tunes it, sets DIGU and keys
  it, and with *Use TCI / VITA-49 Audio* ticked the receive audio comes in
  as a DAX stream over VITA-49 and the transmit audio goes out the same way,
  so no DAX driver, virtual sound card or TCI bridge sits in the path. One
  API session, so the radio's relays click once per over. Ported from the
  MSHV-Mac backend. No split: use Fake It or None. The TX slider is the
  radio's RF power (it follows the radio, which keeps power per band; while
  tuning it is the tune power). **View > FlexRadio Panel...** shows the
  radio's meters and the slice's antennas and mode, and sets transmit power,
  the antenna tuner and the M-series front speaker, as MSHV's Flex panel
  does; antennas are remembered per band for transverter users. Each
  connection puts the slice back on your last frequency in your data mode. Connecting and
  receiving work on a FLEX-6600; transmit is so far verified only against a
  software stand-in (`tools/flex_selftest.sh`). Each connection appends a
  trace to `flex_trace.txt` in the profile's data folder. If you copy a
  QO-100 profile for the Flex, set its *QO-100 rig IF* back to 0.
- **TCI reconnects on its own** (since v0.5.2). If the SDR program
  (AetherSDR, ExpertSDR, Thetis, ...) is closed or restarted, Tx halts
  and the status bar says "TCI: ... reconnecting every 5 s" instead of
  the Rig Control Error box; once the SDR program is back it shows "TCI
  reconnected" and carries on. Other rig types behave as before.
  With TCI audio on, JTDX-VU also watches the receive audio: if none
  arrives for 10 s (Tx aside), it asks for it again once, then says
  "no audio from the SDR program - restart it" and keeps retrying until
  audio is back ("TCI audio back"). AetherSDR 26.9.5 can stop sending
  TCI audio after the last audio client has been gone for over 10 s;
  restarting AetherSDR clears it, JTDX-VU can stay open.
- **Call Non-Stop:** the **Auto CQ** button in the right-hand button
  column, directly under Monitor in every mode (since v0.5.3), also "CNS" in the AutoSeq menu. After each logged QSO, JTDX-VU goes straight back to CQ
  or the next caller instead of halting Tx. Only QSOs from your own CQ
  continue: when you answer someone (double-click their CQ or QSO), Tx
  halts after that QSO, as with **1 QSO**, and while CNS is on AutoSeq
  answers only stations calling you, never other CQs. CNS is off in
  Hound mode. A station that doesn't
  answer is dropped after the AutoSeq counters (Settings > Sequencing),
  as usual. While CNS is on the Tx watchdog is the **Auto CQ time
  limit** (Settings > JTTY, 5 minutes by default; 10 minutes fixed up to
  v0.5.0), so unanswered CQ still stops. While CNS is on, the button
  counts that watchdog down (since v0.7.2) - "4:37", or "12m" from 10
  minutes up, on a second line under "Auto CQ" so the button keeps its
  width (since v0.7.3; v0.7.2 widened it): it runs down while you transmit and holds still while Tx
  is idle, like the "WD" box. Auto CQ and **1 QSO** exclude each
  other (since v0.6.0): switching Auto CQ on clears 1 QSO, and pressing
  1 QSO during an Auto CQ run means "finish this QSO, then stop" - both go
  off when Tx halts after it.
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
  - **A JTTY screen of its own** (new in v0.5.0): the FT-only controls
    (Report, CL, Hound, AutoTX, AutoSeq, Wanted and the wanted filters,
    Bypass, 1 QSO, AnsB4, Enable Tx, Hint, SWL, AGCc, Filter, Decode,
    TX Even, the period progress bar) are hidden while JTTY is selected
    and come back in the other modes. The rest is regrouped: all the
    buttons (Tune, Monitor, Auto CQ, Halt Tx, Log QSO, Erase, Clear DX)
    in the right-hand column, the macro panel across the bottom, and
    **S / R / Name** QSO fields in a column beside Tx / Rx (RST sent and received, logged instead of a
    fixed 599; the name comes from the log when known), and a **Calls
    heard** list of the stations sending in the JTTY decodes (the call
    after DE, a call sent twice, the call in a CQ - not the station being
    called; since v0.6.0), coloured like Band
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
  - **Auto logging from a macro** (since v0.7.3): put %LOG in a macro (e.g. "TU %H 73
    %LOG") and sending it also logs the QSO, straight in without the Log
    QSO dialog; %LOG itself is not sent, and a macro of just %LOG only
    logs. A QSO is logged once: pressing it again, or the "TU ..." message
    that normally opens the dialog, leaves an already-logged QSO alone
    until the DX call changes. The Log QSO button still opens the dialog.
  - **Auto CQ** (right-hand button column) repeats a macro with a gap
    after each call, 10 s by default, and stops after the **time limit**
    (5 minutes by default, since v0.5.1), which the button counts down
    while it runs (since v0.7.2); pressing it again turns it off and
    halts a CQ on the air (Esc and Halt Tx stop any JTTY send). In the FT
    modes the same button is Call Non-Stop. Settings > JTTY sets the
    macro, gap, call limit and stop-on-my-call, the station details, and
    named **macro sets**, each with its own %E exchange ("Contest
    (WSJT-X)" sends %RST %N, "Ragchew / DX" sends %RST). Serial Number
    shows only when the active set uses %N.
  - JTTY QSOs are logged as ADIF MODE MFSK / SUBMODE JTTY, like FT4.

  JTTY is by Joe Taylor K1JT, Steve Franke K9AN, Rob G4KLA and the
  WSJT-X team.
- **Profiles** (Profile menu): one complete settings set per rig —
  VITA-49, TCI, DAX, a QO-100 setup with its own CAT and audio — each with
  its own log and data directory. *New profile from current...* copies the
  current settings, switching restarts the program, and a plain launch
  opens the profile last used. Under the hood these are JTDX's `-r <name>`
  instances (`JTDX-VU - <name>.ini`), so `-r` on the command line still
  works.
  QO-100 through a transverter with SDR-Control for Icom: Settings >
  Frequencies > Frequency Calibration > *QO-100 rig IF* (e.g. 28.540) is
  what JTDX-VU sends the rig for the QO-100 channel; the readback (downlink,
  uplink or IF) is shown as 10489.540 and the log carries 2400.040 /
  10489.540, PROP_MODE SAT, SAT_NAME QO-100.
- **FT2** (new in v0.3.0): FT4's protocol at twice
  the speed — 3.75 s periods, 41.67 baud. FT2 was created by Martino
  Merola IU8LMC (ARI Caserta), with Salvatore Raccampo 9H1SR as lead C++
  developer; it first went on air in Decodium on 2026-02-16
  ([ft2.it](https://ft2.it)), and MSHV by Hrisimir LZ2HV added it in
  2.76.5 on 2026-03-24. JTDX-VU uses no Decodium code: it decodes and
  transmits FT2 with a scaled copy of JTDX's own FT4 chain, with MSHV's
  implementation as the reference. Default dial frequencies
  follow MSHV (14.084, 7.052, 21.144 MHz …). Add the FT2 button in
  View > Band & Mode Buttons... if you want it on the switcher.
  Since v0.6.0 the decoder also has MSHV's log-sum-exp demapper and
  "AP7": once a call pair has been decoded, the station's expected reply
  two periods later is matched against the likely messages for that pair,
  so reports a few dB below the normal floor still decode (shown with a
  trailing `7`). Measured on identical audio (a scripted QSO plus noise played
  to both over TCI, 2026-10-07): JTDX-VU and MSHV decode FT2 to the same
  floor, everything at -12 dB, about half at -15, nothing at -17, with no
  false decodes. MSHV prints FT2 reports 2-3 dB lower than JTDX-VU for the
  same signal, so compare counts, not numbers. Since v0.7.0 AP7
  keeps a call pair across short quiet spells and accepts decodes by MSHV's
  own rule; at -15 dB that roughly doubled the decodes (25 -> 47 of 120,
  MSHV 61), with no false AP7 decodes.

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
