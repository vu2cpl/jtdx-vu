#!/bin/bash
# JTDX-VU: band changes on a FlexRadio VITA-49 test instance (-r flextest)
# against tools/fake_flex.py, no radio.  Clicks band buttons through macOS
# accessibility (addressed by the test instance's own PID, so a running
# JTDX-VU is never touched), times the GUI's replies after each click, and
# keeps the fake radio's log and flex_trace.txt in /tmp/flex_bandtest.
# Env: BANDS="40m 20m", FAKE_NO_ECHO=1 / FAKE_GUI_SLICE=1 / FAKE_BAND_MODE=1 /
# FAKE_TUNE_DELAY=0.8 (see fake_flex.py), DIALFREQ, QO100, JTDX=<binary>.
# Needs the six FT2 periods in /tmp/ft2_autotest-sig (made by ft2_autotest.sh).
# Used 2026-10-07 to reproduce and fix the band-change drop seen on a FLEX-6600.
# start fake radio (late tune echo) + flextest instance, click band buttons, report
set -u
J=/Users/manoj/projects/JTDX; T=$J/tools; JTDX="${JTDX:-$J/build/bundle/JTDX-VU.app/Contents/MacOS/jtdx}"
W=/tmp/flex_bandtest; INI="$HOME/Library/Preferences/JTDX-VU - flextest.ini"; APP="$HOME/Library/Application Support/JTDX-VU - flextest"
rm -rf $W "$APP"; rm -f "$INI"; mkdir -p $W/tx
FAKE_GUI_SLICE=${FAKE_GUI_SLICE:-} FAKE_NO_ECHO=${FAKE_NO_ECHO:-} FAKE_BAND_MODE=${FAKE_BAND_MODE:-1} FAKE_TUNE_DELAY=${FAKE_TUNE_DELAY:-0.8} python3 $T/fake_flex.py --sig /tmp/ft2_autotest-sig --snr -10 --repeats 1 --wait 600 --txdir $W/tx --log $W/fake.txt --forever > $W/fake.log 2>&1 &
sleep 2
cat > "$INI" <<INI
[Common]
Mode=FT8
DialFreq=${DIALFREQ:-14074000}
[Configuration]
MyCall=VU2CPL
MyGrid=MK83te
Rig=FlexRadio VITA-49 Slice A
PTTMethod=@Variant(\0\0\0\x7f\0\0\0\x1eTransceiverFactory::PTTMethod\0\0\0\0\xfPTT_method_CAT\0)
DataMode=@Variant(\0\0\0\x7f\0\0\0\x18\x43onfiguration::DataMode\0\0\0\0\xf\x64\x61ta_mode_data\0)
CATTCIPort=127.0.0.1:4992
TCIAudio=true
QO100RigMHz=${QO100:-0}
Polling=1
UDPServerPort=2299
AcceptUDPRequests=false
INI
"$JTDX" -r flextest > $W/jtdx.log 2>&1 &
P=$!; sleep 14
click() { osascript -e "tell application \"System Events\" to tell (first process whose unix id is $P) to click (first button of window 1 whose name is \"$1\")" 2>&1 | head -1; }
for b in ${BANDS:-40m 20m 15m 40m 20m}; do
  t0=$(python3 -c 'import time;print(time.time())'); r=$(click $b); t1=$(python3 -c 'import time;print(time.time())')
  worst=0; for k in $(seq 1 12); do a=$(python3 -c 'import time;print(time.time())'); osascript -e "tell application \"System Events\" to tell (first process whose unix id is $P) to get name of window 1" >/dev/null 2>&1; z=$(python3 -c 'import time;print(time.time())'); worst=$(python3 -c "print(max($worst, round($z-$a,2)))"); sleep 0.4; done
  echo "click $b -> took $(python3 -c "print(round($t1-$t0,2))") s; slowest GUI reply in the next ~8 s: ${worst} s"
done
echo "== fake radio saw tune/mode:"; grep -E "slice tune|slice set|xmit" $W/fake.txt | sed 's/^[0-9.]* //' 
echo "== qt warnings:"; grep -c "already called exec" $W/jtdx.log; grep -m3 -i "exec\|warn" $W/jtdx.log
sample $P 2 -file $W/sample.txt >/dev/null 2>&1; grep -c "wait_ms" $W/sample.txt
osascript -e "tell application \"System Events\" to tell (first process whose unix id is $P)
  click menu item \"Quit JTDX-VU\" of menu 1 of menu bar item 2 of menu bar 1
end tell" >/dev/null 2>&1
for i in $(seq 1 30); do kill -0 $P 2>/dev/null || break; sleep 0.5; done
kill -0 $P 2>/dev/null && { echo "test instance did not quit - killing"; kill $P; }
pkill -f 'jtdxjt9 -s JTDX-VU - flextest' 2>/dev/null; kill $(pgrep -f fake_flex.py) 2>/dev/null
cp "$APP"/flex_trace.txt $W/ 2>/dev/null; rm -f "$INI"; rm -rf "$APP"
