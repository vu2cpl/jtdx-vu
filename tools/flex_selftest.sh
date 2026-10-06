#!/bin/bash
# JTDX-VU: exercise the FlexRadio VITA-49 rig type against tools/fake_flex.py,
# no radio: the fake radio answers the SmartSDR start-up, streams DAX RX audio
# (noise + a scripted FT2 QSO) and records the DAX TX audio JTDX-VU sends when
# a CQ is triggered over UDP.  Scores the receive side from ALL.TXT and decodes
# the transmitted audio with build/ft2dec.
#   tools/flex_selftest.sh [--snr -10] [--repeats 1]
set -u
SNR=-10; REPEATS=1
while [ $# -gt 0 ]; do case "$1" in --snr) SNR=$2; shift 2;; --repeats) REPEATS=$2; shift 2;; *) echo "unknown $1"; exit 1;; esac; done
T="$(cd "$(dirname "$0")" && pwd)"; R="$(dirname "$T")"
JTDX="${JTDX:-$R/build/bundle/JTDX-VU.app/Contents/MacOS/jtdx}"
SIG="${SIG:-/tmp/ft2_autotest-sig}"; W=/tmp/flex_selftest
INI="$HOME/Library/Preferences/JTDX-VU - flextest.ini"; APP="$HOME/Library/Application Support/JTDX-VU - flextest"
pgrep -f 'jtdx -r flextest' >/dev/null && { echo "a flextest instance is running - stopped"; exit 1; }
lsof -nP -iTCP:4992 -sTCP:LISTEN >/dev/null 2>&1 && { echo "port 4992 in use (a real SmartSDR?) - stopped"; exit 1; }
[ -e "$SIG/p6.wav" ] || { echo "no signals in $SIG - run tools/ft2_autotest.sh once"; exit 1; }
rm -rf "$W" "$APP"; rm -f "$INI"; mkdir -p "$W/tx"
cleanup() {
  P=$(pgrep -f 'jtdx -r flextest')
  [ -n "$P" ] && osascript -e "tell application \"System Events\" to tell (first process whose unix id is $P)
      click menu item \"Quit JTDX-VU\" of menu 1 of menu bar item 2 of menu bar 1
    end tell" >/dev/null 2>&1
  for i in $(seq 1 30); do pgrep -f 'jtdx -r flextest' >/dev/null || break; sleep 0.5; done
  pgrep -f 'jtdx -r flextest' >/dev/null && pkill -f 'jtdx -r flextest'
  pkill -f 'jtdxjt9 -s JTDX-VU - flextest' 2>/dev/null
  sleep 2; kill $(pgrep -f 'fake_flex.py') 2>/dev/null
  cp "$APP"/*_ALL.TXT "$W/jtdx_ALL.TXT" 2>/dev/null
  rm -f "$INI"; rm -rf "$APP"
}
trap cleanup EXIT
python3 "$T/fake_flex.py" --sig "$SIG" --snr "$SNR" --repeats "$REPEATS" --wait 30 --txdir "$W/tx" --log "$W/fake_flex.txt" --forever > "$W/fake.log" 2>&1 &
FAKE=$!; sleep 2
cat > "$INI" <<INI
[Common]
Mode=FT2
[Configuration]
MyCall=VU2CPL
MyGrid=MK83te
Rig=FlexRadio VITA-49 Slice A
PTTMethod=@Variant(\0\0\0\x7f\0\0\0\x1eTransceiverFactory::PTTMethod\0\0\0\0\xfPTT_method_CAT\0)
CATTCIPort=127.0.0.1:4992
TCIAudio=true
Polling=1
UDPServer=127.0.0.1
UDPServerPort=2299
UDP2ServerPort=2298
AcceptUDPRequests=true
EnableTCPConnection=false
INI
"$JTDX" -r flextest > "$W/jtdx.log" 2>&1 &
# the plan: 8 periods x REPEATS per level, from the first 15 s boundary >= 30 s away
PLAN_S=$(( (REPEATS * 30) * $(echo "$SNR" | tr ',' '\n' | wc -l | tr -d ' ') + 45 ))
echo "receive plan: ${PLAN_S}s"; sleep "$PLAN_S"
echo "== fake radio saw:"; grep -v "^[0-9.]* [0-9_]* level" "$W/fake_flex.txt" | sed 's/^[0-9.]* //' | head -40
echo "== triggering a CQ over UDP"
python3 "$T/udp_ctl.py" 2299 cq 2>&1 | tail -2
sleep 12
cleanup; trap - EXIT
echo "== JTDX-VU decodes"; grep " : " "$W/jtdx_ALL.TXT" | head -20
echo "== transmitted audio"; ls "$W/tx" 2>/dev/null
for f in "$W"/tx/*.wav; do [ -e "$f" ] || continue; python3 - "$f" "$R/build/ft2dec" <<'EOF2'
import sys, wave, array, subprocess
f, dec = sys.argv[1], sys.argv[2]
w = wave.open(f); x = array.array('h'); x.frombytes(w.readframes(w.getnframes()))
rms = (sum(v*v for v in x)/max(1,len(x)))**0.5
print(f"{f}: {len(x)/12000:.2f} s, rms {rms:.0f}")
# the modulator starts 0.5 s into the period; the fake's wav starts at the first packet (= key-down), pad 0.5 s? try as is and shifted
for shift in (0, 6000):
    seg = array.array('h', [0]*shift) + x; seg = seg[:45000] + array.array('h', [0]*max(0, 45000-len(seg[:45000])))
    o = wave.open(f + ".p.wav", 'wb'); o.setnchannels(1); o.setsampwidth(2); o.setframerate(12000); o.writeframes(seg.tobytes()); o.close()
    r = subprocess.run([dec, '1500', '3', f + ".p.wav"], capture_output=True, text=True).stdout
    print(f"  shift {shift}:", [l.strip() for l in r.splitlines() if 'VU2CPL' in l or 'CQ' in l])
EOF2
done
