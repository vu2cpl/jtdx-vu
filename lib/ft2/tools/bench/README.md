# FT2 decoder bench (JTDX-VU)

Scratch tools from the 2026-10 FT2 sensitivity work (HANDOVER "FT2" entries),
kept here so the benches can be re-run. They are not built by CMake.

- `runfixed.sh / runfade.sh / runfast.sh <ft2dec binary>` — decode a FIXED
  file set (`fixed/snr-NN`, `fixedfade/`, `fastfade/`, 30 files each, made
  with `ft2sim` — regenerate; `ft2sim` reseeds from time+PID, so never
  compare runs on freshly generated sets).
- `a7unit.f90` — call the AP7 matcher `a7d` directly on one file:
  `a7unit file.wav CALL1 CALL2 GRID4 freq`.
- `truecw.f90` / `truetones.f90` — the 174-bit codeword / 103 tones of a
  message, for diffing against the decoder's hard bits. Use FT4's `rvec`
  (they do now; the first copies carried FT8's and cost a session).
- `enccheck.f90` — proves `encode174_91` output is a valid `bpdecode174_91`
  codeword.

Build like: `gfortran -O2 -Ibuild -Ilib -Jbuild -o a7unit a7unit.f90
build/libwsjt_fort.a build/libwsjt_cxx.a $(pkg-config --libs fftw3f fftw3) -lstdc++`
from the repo root after a normal build.

## Live comparison with MSHV, same audio (2026-10-07)

`tools/ft2_autotest.sh --snr -12,-14,-15,-16,-17 --repeats 4` (repo `tools/`)
runs the two decoders side by side with nothing to click: `tools/ft2_tci_sim.py`
is a TCI server that streams Gaussian noise plus a scripted FT2 QSO (the six
messages CQ / reply / report / R-report / RR73 / 73, one per 3.75 s period
from a 15 s boundary, made by `ft2sim` at full scale) at an exact 48 kHz; an
MSHV head (RX only, decodes to its ALL txt) and a JTDX-VU `-r tcitest`
instance both listen; `tools/ft2_score.py` lines both logs up with the
schedule.  SNR is set in 2500 Hz (signal amplitude from the noise RMS);
JTDX-VU reports it back within 1 dB, MSHV 2-3 dB lower (it keeps FT4's
constant for FT2).  Results in `autotest/`: the 2026-10-07 sweep had both
decoders at 24/24 at -12 dB, 20 (JTDX-VU) vs 22 (MSHV) at -14, 10 vs 10 at
-15, 2 vs 1 at -16 and nothing from -17 down; no false decodes in 192
periods.  AP7 fired on both (1 of JTDX-VU's 20 at -14; 3 of MSHV's 10 at -15).

