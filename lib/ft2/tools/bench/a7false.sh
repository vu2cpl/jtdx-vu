#!/bin/bash
# AP7 false-decode stress: N sequences of 8 periods decoded back to back.
# Periods 1-2 seed the history with K1ABC/W9XYZ at -10 dB; periods 3-6 carry
#   noise    : nothing
#   other    : a DIFFERENT station, N5XYZ, answering K1ABC at -16 dB on the same
#              frequency (K1ABC N5XYZ R-12 / 73 ...) - AP7 must not turn these
#              into the expected partner's "K1ABC W9XYZ ..."
# Any decode that is not a transmitted message counts as false.
#   a7false.sh <ft2dec> <noise|other> [N=200]
DEC=$1; KIND=$2; N=${3:-200}; B="$(cd "$(dirname "$0")" && pwd)"; SET=$B/a7false-$KIND-$N
SIM="$(cd "$B/../../../.." && pwd)/build/ft2sim"
SEED=("CQ K1ABC FN42" "K1ABC W9XYZ EN37")
OTHER=("N5XYZ K1ABC -10" "K1ABC N5XYZ R-12" "N5XYZ K1ABC RR73" "K1ABC N5XYZ 73")
if [ ! -e "$SET/p8_$(printf %03d $N).wav" ]; then
  mkdir -p "$SET"; cd "$SET"
  for k in 1 2 3 4 5 6 7 8; do
    if [ $k -le 2 ]; then m=${SEED[$((k-1))]}; s=-10
    elif [ $k -le 6 ] && [ "$KIND" = other ]; then m=${OTHER[$((k-3))]}; s=-16
    else m="CQ K1ABC FN42"; s=-60; fi
    "$SIM" "$m" 1500 0.0 0.0 0.0 $N $s >/dev/null
    for i in $(seq 1 $N); do mv $(printf "000000_%06d.wav" $i) $(printf "p%d_%03d.wav" $k $i); done
  done
  cd - >/dev/null
fi
files=$(for i in $(seq -f %03g 1 $N); do for k in 1 2 3 4 5 6 7 8; do echo $SET/p${k}_$i.wav; done; done)
"$DEC" 1500 3 $files | python3 -c '
import sys, re
sent = {"CQ K1ABC FN42","K1ABC W9XYZ EN37","N5XYZ K1ABC -10","K1ABC N5XYZ R-12","N5XYZ K1ABC RR73","K1ABC N5XYZ 73"}
ap7=false=false7=0; bad=[]
for l in sys.stdin:
    m = re.match(r"\s*(-?\d+)\s+(-?[\d.]+)\s+(\d+)\s+(.*?)\s*(7?)$", l)
    if not m or "decodes" in l or l.startswith("/"): continue
    msg, is7 = m.group(4).strip(), m.group(5) == "7"
    ap7 += is7
    if msg not in sent:
        false += 1; false7 += is7; bad.append(l.rstrip())
print("AP7 decodes %d, false decodes %d (of them via AP7: %d)" % (ap7, false, false7))
for b in bad[:12]: print("  " + b)
'
