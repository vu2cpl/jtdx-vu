#!/bin/bash
# NOTE 2026-10-07: tools/ft2_autotest.sh does this with nothing to click (its
# own TCI server plays noise + a scripted QSO to an MSHV head and a JTDX-VU
# test instance, and scores both from their logs).  This script is the manual
# version, kept for a live two-heads session.
#
# JTDX-VU: FT2 loopback sensitivity test against MSHV (LZ2HV's suggestion,
# 2026-10-06: "use HV TCI server, one head of MSHV to TX, slow down TX level
# and test sensitivity").  No radio: Christo's TCI_HV echo server mixes every
# client's TX audio into its RX streams and adds Gaussian noise (its "Noice"
# slider), so one MSHV head transmitting FT2 at a falling TX level is heard by
# a second MSHV head and by a JTDX-VU test instance - compare who still
# decodes.  Everything runs on 127.0.0.1:50002 and is torn down by `stop`.
#
#   tools/ft2_loopback.sh start   # server + MSHV heads VU2AAA/VU2BBB + JTDX-VU -r tcitest
#   tools/ft2_loopback.sh stop    # quit all three, remove the throwaway JTDX-VU config
#
# Then, by hand in the VU2AAA head: start a CQ and lower the Tx level a few dB
# at a time; score JTDX-VU from "~/Library/Application Support/JTDX-VU - tcitest/
# <yyyymm>_ALL.TXT" and the heads from their decode lists.  Answering the CQ
# from JTDX-VU makes a QSO loop that exercises AP7 on both sides.
#
# Paths are the MSHV-Mac project's: the echo server built 2026-09-16 and the
# LZ2HV-tree head build that keeps its settings inside the bundle (the Mac
# build reads ~/Library/Application Support/MSHV, which the live MSHV uses -
# never run a test head from that).
set -u
M=/Users/manoj/projects/MSHV-Mac/sent-to-LZ2HV
# 2026-10-07: the as-received server's Noice slider spans only +-3.1 dB, which
# with the MSHV head's Tx level at its lowest still left the signal at -14 dB -
# too strong to exercise AP7.  build-widenoise/ is the same source with the
# slider widened to +-12.5 dB (middle = 1.0 = as before); used when present.
SRV="$M/2026-09-16-tci-echoserver-received/build-widenoise/echoserver_tci_hv_031/TCI_HV.app/Contents/MacOS/TCI_HV"
[ -x "$SRV" ] || SRV="$M/2026-09-16-tci-echoserver-received/build/echoserver_tci_hv_031/TCI_HV.app/Contents/MacOS/TCI_HV"
HEAD="$M/2026-09-16-tci-echoserver-received/heads/mshv-noblock/MSHV.app"
SEED="$M/2026-09-15-rc029-received/build-and-radio-test/rc029-test-settings-scrubbed"
JTDX="${JTDX:-/Users/manoj/projects/JTDX/build/bundle/JTDX-VU.app/Contents/MacOS/jtdx}"
PORT=${PORT:-50002}
W="${W:-/tmp/ft2_loopback}"
INI="$HOME/Library/Preferences/JTDX-VU - tcitest.ini"

case "${1:-}" in
start)
  pgrep -f 'MacOS/MSHV' >/dev/null && { echo "an MSHV is running - stopped"; exit 1; }
  pgrep -f 'MacOS/TCI_HV' >/dev/null && { echo "a TCI_HV is running - stopped"; exit 1; }
  pgrep -f 'jtdx -r tcitest' >/dev/null && { echo "a JTDX-VU tcitest instance is running - stopped"; exit 1; }
  lsof -nP -iTCP:$PORT -sTCP:LISTEN >/dev/null 2>&1 && { echo "port $PORT in use - stopped"; exit 1; }
  for f in "$SRV" "$HEAD/Contents/MacOS/MSHV" "$JTDX"; do [ -x "$f" ] || { echo "missing: $f"; exit 1; }; done
  rm -rf "$W"; mkdir -p "$W"
  "$SRV" -p "$PORT" > "$W/server.log" 2>&1 &
  sleep 3
  for h in 1 2; do
    ditto "$HEAD" "$W/head$h/MSHV.app"; d="$W/head$h/MSHV.app/Contents/MacOS/settings"; ditto "$SEED" "$d"
    call=VU2AAA; [ $h = 2 ] && call=VU2BBB
    sed -i '' -e 's/^default_rig_name=.*/default_rig_name=TCI Client RX1/' \
              -e 's/^default_device_alsa=.*/default_device_alsa=TCI Client Input/' \
              -e 's/^default_out_dev=.*/default_out_dev=TCI Client Output/' \
              -e 's/^mod_identifaer=.*/mod_identifaer=18/' \
              -e "s/127\.0\.0\.1;50001;/127.0.0.1;$PORT;/g" "$d/ms_settings"      # 18 = FT2
    sed -i '' -e "s/^macr_my_call=.*/macr_my_call=$call/" "$d/ms_macros"
    "$W/head$h/MSHV.app/Contents/MacOS/MSHV" > "$W/head$h.log" 2>&1 &
    sleep 6
  done
  cat > "$INI" <<INI
[Common]
Mode=FT2
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
  sleep 8
  echo "clients on $PORT: $(lsof -nP -iTCP:$PORT | grep -c ESTABLISHED) (expect 6 = 3 clients x 2)"
  echo "now: in the VU2AAA head start a CQ; raise the server's Noice slider step by step (middle = nominal)"
  ;;
stop)
  # Quit JTDX-VU through its menu (never pkill: a killed jtdx leaks SysV shm)
  P=$(pgrep -f 'jtdx -r tcitest')
  [ -n "$P" ] && osascript -e "tell application \"System Events\" to tell (first process whose unix id is $P)
      click menu item \"Quit JTDX-VU\" of menu 1 of menu bar item 2 of menu bar 1
    end tell" >/dev/null 2>&1
  kill $(pgrep -f 'MacOS/MSHV') 2>/dev/null
  osascript -e 'tell application "TCI_HV" to quit' >/dev/null 2>&1; sleep 2
  kill $(pgrep -f 'MacOS/TCI_HV') 2>/dev/null
  for i in $(seq 1 30); do pgrep -f 'jtdx -r tcitest' >/dev/null || break; sleep 0.5; done
  pgrep -fl 'jtdx -r tcitest' && echo "JTDX-VU tcitest still running - quit it from its window (Cmd+Q)"
  rm -f "$INI"; rm -rf "$HOME/Library/Application Support/JTDX-VU - tcitest"
  echo "stopped"
  ;;
*) echo "usage: $0 start|stop"; exit 1;;
esac
