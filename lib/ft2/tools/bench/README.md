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
