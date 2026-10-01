#!/bin/bash
# Linux twin of run_quit_test.sh, for a Pi or other X11 host (needs Xvfb, xdotool,
# ImageMagick import). Quits via File > Exit clicks (Alt+F4 does not reach the
# window without a window manager). Run it from its own ssh command: its pgrep
# guards would otherwise match the calling shell.
# usage: tciquit.sh <jtdx binary> <delay s>
BIN="$1"; DELAY="$2"; export DISPLAY=:7
pgrep -f "Xvfb :[7]" >/dev/null || { Xvfb :7 -screen 0 1366x768x24 >/tmp/xvfb.log 2>&1 & sleep 2; }
pgrep -f "silent_tc[i].py" >/dev/null || { python3 ~/src/jtdx-vu/tools/silent_tci.py /tmp/tciconn.log >/tmp/tcisrv.out 2>&1 & sleep 1; }
cat > ~/.config/"JTDX-VU - tcitest.ini" <<"INI"
[Common]
Mode=FT8
[Configuration]
MyCall=VU2CPL
MyGrid=MK83te
Rig=TCI Client RX1
CATTCIPort=127.0.0.1:50099
TCIAudio=false
Polling=1
INI
: > /tmp/tciconn.log
"$BIN" -r tcitest > /tmp/tciapp.log 2>&1 &
P=$!
for i in $(seq 1 300); do grep -q connected /tmp/tciconn.log && break; sleep 0.1; done
grep -q connected /tmp/tciconn.log || { echo "no TCI connect seen"; kill $P; exit 2; }
sleep "$DELAY"
W=$(xdotool search --pid $P --name "tcitest for VUCG" | head -1)
xdotool mousemove 20 30 click 1; sleep 0.3; xdotool mousemove 46 310 click 1; sleep 1.5; import -window root /tmp/tciquit.png; xdotool search --pid $P --name . getwindowname %@ > /tmp/tciwins.txt 2>/dev/null
for i in $(seq 1 100); do kill -0 $P 2>/dev/null || break; sleep 0.1; done
if kill -0 $P 2>/dev/null; then echo "still running after 10 s"; kill -9 $P; fi
wait $P; RC=$?
pkill -f "jtdxjt9 -s JTDX-VU - tcites[t]"
echo "exit status $RC $( [ $RC -eq 139 ] && echo SEGFAULT )"
