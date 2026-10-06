#!/bin/bash
# Decode the FIXED file set with one ft2dec binary. usage: runfixed.sh <binary> [label]
DEC=$1; L=${2:-$(basename $DEC)}; F=$(dirname "$0")/fixed
printf "%-6s %s   [%s]\n" "SNR" "decoded/30" "$L"
for d in $F/snr-13 $F/snr-14 $F/snr-15 $F/snr-16 $F/snr-17 $F/snr-18; do
  ok=0; for f in $d/*.wav; do $DEC 1500 3 "$f" 2>/dev/null | grep -q "CQ W9XYZ EN37" && ok=$((ok+1)); done
  printf "%-6s %d\n" "$(basename $d | sed s/snr//)" "$ok"
done
