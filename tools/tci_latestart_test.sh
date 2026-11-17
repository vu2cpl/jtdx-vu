#!/bin/bash
# An SDR program that is still starting (SmartSDR for Mac just after it has
# connected to its radio): it greets with "stop; ready;" and says "start;" a
# few seconds later. Runs a throwaway "-r latestarttest" copy on
# tools/fake_tci.py with FAKE_START_DELAY (default 6 s) for HOLD s (default 25)
# and reports how often the client connected and whether audio started
# (0.7.6: no audio_start before the retry; fixed: 1 connect, audio started).
# usage: tools/tci_latestart_test.sh <jtdx binary>
set -u
BIN=$1
S=${S:-$(mktemp -d /tmp/jtdxvu-test.XXXXXX)}; J="$(cd "$(dirname "$0")/.." && pwd)"
INI="$HOME/Library/Preferences/JTDX-VU - latestarttest.ini"
APP="$HOME/Library/Application Support/JTDX-VU - latestarttest"
PORT=50095
[ -e "$INI" ] && { echo "latestarttest ini exists - stopped"; exit 1; }
FAKE_START_DELAY=${FAKE_START_DELAY:-6} python3 "$J/tools/fake_tci.py" "$S/fake.log" $PORT > /dev/null 2>&1 &
SIM=$!
sleep 2
cat > "$INI" <<INI
[Common]
Mode=FT8
[Configuration]
MyCall=VU2CPL
MyGrid=MK83te
Rig=TCI Client RX1
PTTMethod=@Variant(\0\0\0\x7f\0\0\0\x1eTransceiverFactory::PTTMethod\0\0\0\0\xfPTT_method_CAT\0)
CATTCIPort=127.0.0.1:$PORT
TCIAudio=true
Polling=1
UDPServer=127.0.0.1
UDPServerPort=2299
UDP2ServerPort=2298
AcceptUDPRequests=false
EnableTCPConnection=false
CheckForUpdates=false
INI
BEFORE=$(ipcs -m | awk '$5=="manoj"{print $2}' | sort)
"$BIN" -r latestarttest > "$S/app.log" 2>&1 &
P=$!
sleep ${HOLD:-25}
kill $P; sleep 3; kill -9 $P 2>/dev/null
for q in $(pgrep -f "jtdxjt9 -s JTDX-VU - latestarttest"); do kill $q; done
kill $SIM 2>/dev/null
sleep 1
for m in $(comm -13 <(echo "$BEFORE") <(ipcs -m | awk '$5=="manoj"{print $2}' | sort)); do ipcrm -m $m 2>/dev/null; done
rm -f "$INI"; rm -rf "$APP"
echo "connects:      $(grep -c ' connected$' "$S/fake.log")"
echo "late start:    $(grep -c 'late start sent' "$S/fake.log")"
echo "audio_start:   $(grep -c '< audio_start' "$S/fake.log")"
echo "logs in $S"
