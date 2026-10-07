#!/bin/bash
# JTDX-VU: the FlexRadio panel and the rig-mode indicator on a test instance
# (-r flextest) against tools/fake_flex.py, no radio: opens View > FlexRadio
# Panel..., screenshots the panel and main window (main_rx/tx.png,
# panel_rx/tx.png in /tmp/flex_paneltest), keys a CQ over UDP and lists what
# the fake radio received.  Addressed by the test instance's own PID.  Set
# UseDarkStyle in the ini block below to check dark mode.  Some Qt controls
# (the TX slider, spin boxes) can't be driven through accessibility.
set -u
J=/Users/manoj/projects/JTDX; T=$J/tools; JTDX="${JTDX:-$J/build/bundle/JTDX-VU.app/Contents/MacOS/jtdx}"
W=/tmp/flex_paneltest; INI="$HOME/Library/Preferences/JTDX-VU - flextest.ini"; APP="$HOME/Library/Application Support/JTDX-VU - flextest"
rm -rf $W "$APP"; rm -f "$INI"; mkdir -p $W/tx
FAKE_GUI_SLICE=1 FAKE_NO_ECHO=1 python3 $T/fake_flex.py --sig /tmp/ft2_autotest-sig --snr -10 --repeats 1 --wait 600 --txdir $W/tx --log $W/fake.txt --forever > $W/fake.log 2>&1 &
sleep 2
cat > "$INI" <<INI
[Common]
Mode=FT8
DialFreq=7074000
[Configuration]
UseDarkStyle=false
MyCall=VU2CPL
MyGrid=MK83te
Rig=FlexRadio VITA-49 Slice A
PTTMethod=@Variant(\0\0\0\x7f\0\0\0\x1eTransceiverFactory::PTTMethod\0\0\0\0\xfPTT_method_CAT\0)
DataMode=@Variant(\0\0\0\x7f\0\0\0\x18\x43onfiguration::DataMode\0\0\0\0\xf\x64\x61ta_mode_data\0)
CATTCIPort=127.0.0.1:4992
TCIAudio=true
Polling=1
CATRequestSNR=true
CATRequestPower=true
UDPServerPort=2299
AcceptUDPRequests=true
INI
"$JTDX" -r flextest > $W/jtdx.log 2>&1 &
P=$!; sleep 15
SE() { osascript -e "tell application \"System Events\" to tell (first process whose unix id is $P) to $1" 2>&1; }
echo "== open panel:"; SE 'click menu item "FlexRadio Panel..." of menu 1 of menu bar item "View" of menu bar 1' | head -1; sleep 3
echo "== windows:"; SE 'get name of every window'
shot() { swift $T/macos-window-list.swift $P | while IFS=$'\t' read id name; do case "$name" in FlexRadio) screencapture -x -o -l $id $W/panel_$1.png;; *VUCG*) screencapture -x -o -l $id $W/main_$1.png;; esac; done; }
shot rx
echo "== S-meter (expect S8-ish for -85 dBm):"
echo "== key via UDP CQ"; python3 $T/udp_ctl.py 2299 cq 2>&1 | tail -1; for i in $(seq 1 20); do grep -q "< xmit 1" $W/fake.txt && break; sleep 0.5; done; sleep 1.5
shot tx
echo "== panel during/after TX:"; SE 'get value of every static text of window "FlexRadio"'
echo "== fake radio saw:"; grep -E "transmit set|slice s |slice set|slice tune|atu|mixer|xmit" $W/fake.txt | sed 's/^[0-9.]* //'
cp "$APP"/flex_trace.txt $W/ 2>/dev/null
osascript -e "tell application \"System Events\" to tell (first process whose unix id is $P)
  click menu item \"Quit JTDX-VU\" of menu 1 of menu bar item 2 of menu bar 1
end tell" >/dev/null 2>&1
for i in $(seq 1 30); do kill -0 $P 2>/dev/null || break; sleep 0.5; done
kill -0 $P 2>/dev/null && { echo "did not quit - killing"; kill $P; }
pkill -f 'jtdxjt9 -s JTDX-VU - flextest' 2>/dev/null; kill $(pgrep -f fake_flex.py) 2>/dev/null
rm -f "$INI"; rm -rf "$APP"
