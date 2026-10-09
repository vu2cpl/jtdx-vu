#!/usr/bin/env python3
"""Build JTDX-VU's callsign -> US state table (us_call_states.txt).

The fallback leg of Show US State, used when a decode carries no grid: the
state of the licence address in the FCC amateur database. Same rules as
DXCA's FCC distillation (vu2cpl/dxca, crates/dxca-connect/src/fcc.rs):
active licences only (HD.dat status A), so a reassigned call never answers
with its previous holder's state; the 50 states, DC counted as MD,
territories left out; later EN.dat rows for a call win.

The FCC knows the licence address only: a W6 living in Ohio shows as CA.

Source: the FCC's weekly complete dump (public domain, ~200 MB):
  https://data.fcc.gov/download/pub/uls/complete/l_amat.zip
  python3 tools/build_us_call_states.py l_amat.zip us_call_states.txt
Output: sorted "CALL ST" lines (~8 MB), binary-searched by JTDX-VU.
data.fcc.gov refuses "Accept-Encoding: gzip" (and "identity"): download
with plain curl, which sends no such header.
"""
import io
import sys
import zipfile

STATES = {
    "AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE", "FL", "GA", "HI", "ID", "IL", "IN", "IA",
    "KS", "KY", "LA", "ME", "MD", "MA", "MI", "MN", "MS", "MO", "MT", "NE", "NV", "NH", "NJ",
    "NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA", "RI", "SC", "SD", "TN", "TX", "UT", "VT",
    "VA", "WA", "WV", "WI", "WY",
}


def lines(zf, name):
    with zf.open(name) as raw:
        for line in io.TextIOWrapper(raw, encoding="utf-8", errors="replace"):
            yield line.rstrip("\r\n")


def main(zip_path, out_path):
    with zipfile.ZipFile(zip_path) as zf:
        active = set()
        for line in lines(zf, "HD.dat"):
            f = line.split("|")
            if len(f) > 5 and f[5] == "A":
                active.add(f[1])
        if not active:
            sys.exit("HD.dat: no active licences - wrong file?")
        table = {}
        for line in lines(zf, "EN.dat"):
            f = line.split("|")
            if len(f) < 18 or f[0] != "EN" or f[1] not in active:
                continue
            call = f[4].strip().upper()
            st = f[17].strip().upper()
            if st == "DC":
                st = "MD"
            if call and st in STATES:
                table[call] = st
    if len(table) < 100000:
        sys.exit(f"only {len(table)} calls - refusing a suspiciously small table")
    with open(out_path, "w") as out:
        for call in sorted(table):
            out.write(f"{call} {table[call]}\n")
    print(f"{len(table)} calls -> {out_path}")


if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2])
