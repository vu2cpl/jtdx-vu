#!/bin/bash
# Decode the FIXED mild-fading set (0.5 Hz spread, 1 ms delay). usage: runfade.sh <binary> [label]
DEC=$1; L=${2:-$(basename $DEC)}; F=$(dirname "$0")/fixedfade
printf "%-6s %s   [%s]\n" "SNR" "decoded/30" "$L"
for d in $F/snr-10 $F/snr-12 $F/snr-13 $F/snr-14 $F/snr-15 $F/snr-16; do
  ok=0; for f in $d/*.wav; do $DEC 1500 3 "$f" 2>/dev/null | grep -q "CQ W9XYZ EN37" && ok=$((ok+1)); done
  printf "%-6s %d\n" "$(basename $d | sed s/snr//)" "$ok"
done
