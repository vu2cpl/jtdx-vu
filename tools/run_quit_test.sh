#!/bin/bash
# usage: run_quit_test.sh <path to jtdx binary> <delay after TCI connect, s>
BIN="$1"; DELAY="${2:-0.5}"; S="$(cd "$(dirname "$0")" && pwd)"
INI="$HOME/Library/Preferences/JTDX-VU - tcitest.ini"
cat > "$INI" <<'INI'
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
: > "$S/conn.log"
touch "$S/t0.ref"
"$BIN" -r tcitest > "$S/app.log" 2>&1 &
P=$!
for i in $(seq 1 200); do grep -q connected "$S/conn.log" && break; sleep 0.1; done
grep -q connected "$S/conn.log" || { echo "no TCI connect seen"; kill $P; exit 2; }
sleep "$DELAY"
osascript -e "tell application \"System Events\" to tell (first process whose unix id is $P)
  set q to (first menu item of menu 1 of menu bar item 2 of menu bar 1 whose name starts with \"Quit\")
  click q
end tell" 2>&1 | head -2
for i in $(seq 1 100); do kill -0 $P 2>/dev/null || break; sleep 0.1; done
if kill -0 $P 2>/dev/null; then echo "still running after 10 s"; kill $P; fi
wait $P 2>/dev/null; RC=$?
pkill -f "jtdxjt9 -s JTDX-VU - tcites[t]"
sleep 3
NEW=$(find ~/Library/Logs/DiagnosticReports -name 'jtdx-*.ips' -newer "$S/t0.ref" 2>/dev/null)
echo "exit status $RC; new crash reports: ${NEW:-none}"
cat "$S/conn.log"
