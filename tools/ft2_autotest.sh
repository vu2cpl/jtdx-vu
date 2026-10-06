#!/bin/bash
# JTDX-VU: hands-off FT2 sensitivity comparison, JTDX-VU vs MSHV, same audio.
# tools/ft2_tci_sim.py plays the band - noise plus a scripted FT2 QSO at a
# list of SNRs - as a TCI server; an MSHV head (VU2BBB, RX only) and a JTDX-VU
# `-r tcitest` instance both decode it.  Both write every decode to a file;
# tools/ft2_score.py lines them up with the schedule.  Nothing to click.
#
#   tools/ft2_autotest.sh --snr -10,-13,-15,-17 [--repeats 4] [--nomshv] [--out DIR]
#
# Refuses to start while the live MSHV, an ft2_tci_sim or a tcitest JTDX-VU runs.
set -u
LEVELS=""; SNRS=""; REPEATS=4; MSHV=1; OUT=""
while [ $# -gt 0 ]; do case "$1" in
  --levels) LEVELS=$2; shift 2;; --snr) SNRS=$2; shift 2;; --repeats) REPEATS=$2; shift 2;;
  --nomshv) MSHV=0; shift;; --out) OUT=$2; shift 2;;
  *) echo "unknown $1"; exit 1;; esac; done
[ -n "$LEVELS$SNRS" ] || { echo "usage: $0 --snr -10,-14,-16 | --levels g1,g2,... [--repeats N] [--nomshv]"; exit 1; }
M=/Users/manoj/projects/MSHV-Mac/sent-to-LZ2HV
HEAD="$M/2026-09-16-tci-echoserver-received/heads/mshv-noblock/MSHV.app"
SEED="$M/2026-09-15-rc029-received/build-and-radio-test/rc029-test-settings-scrubbed"
JTDX="${JTDX:-/Users/manoj/projects/JTDX/build/bundle/JTDX-VU.app/Contents/MacOS/jtdx}"
SIG="${SIG:-/tmp/ft2_autotest-sig}"      # the six clean periods, made by ft2sim once and kept
PORT=${PORT:-50002}; W=/tmp/ft2_autotest; T="$(cd "$(dirname "$0")" && pwd)"
INI="$HOME/Library/Preferences/JTDX-VU - tcitest.ini"; APP="$HOME/Library/Application Support/JTDX-VU - tcitest"
[ -n "$OUT" ] || OUT="$W/results-$(date -u +%Y%m%d-%H%M%S)"

pgrep -f 'MacOS/MSHV' >/dev/null && { echo "an MSHV is running - stopped"; exit 1; }
pgrep -f 'ft2_tci_sim.py' >/dev/null && { echo "an ft2_tci_sim is running - stopped"; exit 1; }
lsof -nP -iTCP:$PORT -sTCP:LISTEN >/dev/null 2>&1 && { echo "port $PORT in use - stopped"; exit 1; }
pgrep -f 'jtdx -r tcitest' >/dev/null && { echo "a JTDX-VU tcitest instance is running - stopped"; exit 1; }
[ -x "$JTDX" ] || { echo "missing: $JTDX"; exit 1; }
if [ ! -e "$SIG/p6.wav" ]; then
  FT2SIM="$(dirname "$T")/build/ft2sim"; [ -x "$FT2SIM" ] || { echo "missing: $FT2SIM (build the tree first)"; exit 1; }
  mkdir -p "$SIG"; ( cd "$SIG" && i=0 && while IFS= read -r m; do i=$((i+1)); "$FT2SIM" "$m" 1500 0.0 0.0 0.0 1 99 >/dev/null && mv 000000_000001.wav "p$i.wav"; done <<'MSGS'
CQ K1ABC FN42
K1ABC W9XYZ EN37
W9XYZ K1ABC -10
K1ABC W9XYZ R-12
W9XYZ K1ABC RR73
K1ABC W9XYZ 73
MSGS
  ) || exit 1
  echo "made the six FT2 periods in $SIG"
fi

cleanup() {
  P=$(pgrep -f 'jtdx -r tcitest')
  [ -n "$P" ] && osascript -e "tell application \"System Events\" to tell (first process whose unix id is $P)
      click menu item \"Quit JTDX-VU\" of menu 1 of menu bar item 2 of menu bar 1
    end tell" >/dev/null 2>&1
  for i in $(seq 1 30); do pgrep -f 'jtdx -r tcitest' >/dev/null || break; sleep 0.5; done
  pgrep -f 'jtdx -r tcitest' >/dev/null && { echo "JTDX-VU did not quit via menu, killing"; pkill -f 'jtdx -r tcitest'; sleep 1; }
  pkill -f 'jtdxjt9 -s JTDX-VU - tcitest' 2>/dev/null
  kill $(pgrep -f "$W/head/MSHV.app") 2>/dev/null; sleep 4      # MSHV flushes its ALL txt 3.6 s after a decode
  kill $(pgrep -f 'ft2_tci_sim.py') 2>/dev/null
  mkdir -p "$OUT"
  cp "$APP"/*_ALL.TXT "$OUT/jtdx_ALL.TXT" 2>/dev/null
  cp "$W"/head/MSHV.app/Contents/MacOS/AllTxtMonthly/ALL_*.TXT "$OUT/mshv_ALL.TXT" 2>/dev/null
  cp "$W/schedule.txt" "$W"/*.log "$OUT/" 2>/dev/null
  [ -d "$APP/save" ] && cp -R "$APP/save" "$OUT/jtdx_save"
  rm -f "$INI"; rm -rf "$APP"
}
trap cleanup EXIT
rm -rf "$W" "$APP"; rm -f "$INI"; mkdir -p "$W"
# the band: tools/ft2_tci_sim.py streams noise + the scripted QSO at an exact
# 48 kHz (Christo's TCI_HV echo server, tried first, paces its stream on a Qt
# timer that ran at 47-87 % of real time here - unusable for timing).
python3 "$T/ft2_tci_sim.py" --sig "$SIG" ${SNRS:+--snr "$SNRS"} ${LEVELS:+--levels "$LEVELS"} --repeats "$REPEATS" \
        --port "$PORT" --log "$W/schedule.txt" --wait 30 > "$W/sim.log" 2>&1 &
SIM=$!
sleep 3
if [ $MSHV = 1 ]; then
  ditto "$HEAD" "$W/head/MSHV.app"; d="$W/head/MSHV.app/Contents/MacOS/settings"; ditto "$SEED" "$d"
  mkdir -p "$W/head/MSHV.app/Contents/MacOS/AllTxtMonthly"
  sed -i '' -e 's/^default_rig_name=.*/default_rig_name=TCI Client RX1/' \
            -e 's/^default_device_alsa=.*/default_device_alsa=TCI Client Input/' \
            -e 's/^default_out_dev=.*/default_out_dev=TCI Client Output/' \
            -e 's/^mod_identifaer=.*/mod_identifaer=18/' \
            -e "s/127\.0\.0\.1;50001;/127.0.0.1;$PORT;/g" "$d/ms_settings"
  sed -i '' -e "s/^macr_my_call=.*/macr_my_call=VU2BBB/" "$d/ms_macros"
  "$W/head/MSHV.app/Contents/MacOS/MSHV" > "$W/head.log" 2>&1 &
  sleep 6
fi
cat > "$INI" <<INI
[Common]
Mode=FT2
SaveWav=${SAVEWAV:-0}
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
INI
"$JTDX" -r tcitest > "$W/jtdx.log" 2>&1 &
sleep 10
echo "clients on $PORT: $(lsof -nP -iTCP:$PORT | grep -c ESTABLISHED) connections"
wait $SIM; cat "$W/sim.log"
sleep 6
cleanup; trap - EXIT
echo "== results in $OUT"
python3 "$T/ft2_score.py" "$OUT" | tee "$OUT/score.txt"
