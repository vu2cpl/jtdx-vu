#!/bin/bash
# AP7 bench: N simulated FT2 QSO sequences (CQ, reply, report, R-report, RR73,
# 73, then two noise-only periods) at one SNR, decoded in order with ft2dec so
# the AP7 history works as it does live.  Reports per period how many of N
# decoded plain / via AP7, and every decode that was not the planned message.
#   a7bench.sh <ft2dec binary> <snr> [N=40] [set dir, default a7set-<snr>]
# The set is made once with ft2sim (no fading, fresh noise per file) and kept.
DEC=$1; SNR=$2; N=${3:-40}; B="$(cd "$(dirname "$0")" && pwd)"; SET=${4:-$B/a7set-$SNR}
TAIL=${5:-}     # "noisetail": periods 3..8 are noise only (false-decode test: the history has a pair, the band has nothing)
SIM="$(cd "$B/../../../.." && pwd)/build/ft2sim"
MSGS=("CQ K1ABC FN42" "K1ABC W9XYZ EN37" "W9XYZ K1ABC -10" "K1ABC W9XYZ R-12" "W9XYZ K1ABC RR73" "K1ABC W9XYZ 73")
if [ ! -e "$SET/s$(printf %02d $N)/p8.wav" ]; then
  mkdir -p "$SET"; cd "$SET"
  for k in 1 2 3 4 5 6; do
    S=$SNR; [ "$TAIL" = noisetail ] && [ $k -ge 3 ] && S=-60
    "$SIM" "${MSGS[$((k-1))]}" 1500 0.0 0.0 0.0 $N $S >/dev/null
    for i in $(seq 1 $N); do d=$(printf "s%02d" $i); mkdir -p $d; mv $(printf "000000_%06d.wav" $i) $d/p$k.wav; done
  done
  "$SIM" "CQ K1ABC FN42" 1500 0.0 0.0 0.0 $((2*N)) -60 >/dev/null     # noise-only periods
  for i in $(seq 1 $N); do d=$(printf "s%02d" $i); mv $(printf "000000_%06d.wav" $((2*i-1))) $d/p7.wav; mv $(printf "000000_%06d.wav" $((2*i))) $d/p8.wav; done
  cd - >/dev/null
fi
A7TAIL=$TAIL python3 - "$DEC" "$SET" "$N" <<'PY'
import sys, subprocess, re, os
dec, S, N = sys.argv[1], sys.argv[2], int(sys.argv[3])
MSGS = ["CQ K1ABC FN42", "K1ABC W9XYZ EN37", "W9XYZ K1ABC -10", "K1ABC W9XYZ R-12", "W9XYZ K1ABC RR73", "K1ABC W9XYZ 73", "", ""]
if os.environ.get("A7TAIL") == "noisetail": MSGS[2:6] = ["", "", "", ""]
plain = [0]*8; ap7 = [0]*8; false = []
# one decoder run over all sequences back to back (32 s per QSO, as live), so the
# AP7 history carries from one QSO to the next the way it does on the air;
# A7SEPARATE=1 restarts the decoder for every QSO instead
runs = [[i] for i in range(1, N+1)] if os.environ.get("A7SEPARATE") else [list(range(1, N+1))]
for seqs in runs:
    files = [os.path.join(S, "s%02d" % i, "p%d.wav" % k) for i in seqs for k in range(1, 9)]
    out = subprocess.run([dec, "1500", "3"] + files, capture_output=True, text=True).stdout
    k = 0; i = seqs[0]
    for line in out.splitlines():
        mi = re.search(r"s(\d\d)/p\d\.wav$", line)
        if mi: i = int(mi.group(1))
        mm = re.search(r"p(\d)\.wav$", line)
        if mm: k = int(mm.group(1)); continue
        m = re.match(r"\s*(-?\d+)\s+(-?[\d.]+)\s+(\d+)\s+(.*?)\s*(7?)$", line)
        if not m or "decodes" in line: continue
        msg = m.group(4).strip(); is7 = m.group(5) == "7"
        if msg == MSGS[k-1]: (ap7 if is7 else plain)[k-1] += 1
        else: false.append("s%02d p%d: %s" % (i, k, line.strip()))
print("period:      CQ  reply report R-rpt  RR73    73  noise noise")
print("plain :  " + " ".join("%5d" % v for v in plain))
print("AP7   :  " + " ".join("%5d" % v for v in ap7))
print("all   :  " + " ".join("%5d" % (plain[j]+ap7[j]) for j in range(8)) + "   of %d" % N)
print("false decodes: %d" % len(false)); [print("  " + f) for f in false[:20]]
PY
