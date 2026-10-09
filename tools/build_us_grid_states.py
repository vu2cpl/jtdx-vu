#!/usr/bin/env python3
"""Build JTDX-VU's grid -> US state table (us_grid_states.txt).

For every 4-character Maidenhead square that touches a US state, list the
states it covers, the largest share of the square's US land first: a CQ from
EM78 then shows as U.S.A.:KY-IN-OH (Show US State). Alaska and Hawaii are
included: on the band they are their own DXCC entities, but the display
uses the table only for U.S.A. stations anyway. DC is counted as MD, as WAS
does. Territories are left out because they are separate DXCC entities.

Source: US Census Bureau cartographic boundary file cb_<year>_us_state_500k
(public domain), e.g.
  https://www2.census.gov/geo/tiger/GENZ2023/shp/cb_2023_us_state_500k.zip
Needs shapely and pyshp:
  python3 -m venv v && v/bin/pip install shapely pyshp
  v/bin/python tools/build_us_grid_states.py cb_2023_us_state_500k.shp us_grid_states.txt

A state is listed when it holds at least MIN_SHARE of the square's US land,
so a few hectares of a neighbour across a river do not add a third state.
"""
import math
import sys

import shapefile
from shapely.geometry import box, shape
from shapely.ops import unary_union

MIN_SHARE = 0.02
STATES = {
    "AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE", "FL", "GA", "HI", "ID", "IL", "IN", "IA",
    "KS", "KY", "LA", "ME", "MD", "MA", "MI", "MN", "MS", "MO", "MT", "NE", "NV", "NH", "NJ",
    "NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA", "RI", "SC", "SD", "TN", "TX", "UT", "VT",
    "VA", "WA", "WV", "WI", "WY",
}


def grid_name(lon0, lat0):
    """Square whose south-west corner is (lon0, lat0); lon0 even, both whole degrees."""
    lon = lon0 + 180
    lat = lat0 + 90
    return (chr(ord("A") + lon // 20) + chr(ord("A") + lat // 10)
            + str((lon % 20) // 2) + str(lat % 10))


def main(shp_path, out_path):
    shapes = {}
    for rec in shapefile.Reader(shp_path).iterShapeRecords():
        st = rec.record["STUSPS"]
        if st == "DC":
            st = "MD"
        if st not in STATES:
            continue
        geom = shape(rec.shape.__geo_interface__)
        shapes[st] = unary_union([shapes[st], geom]) if st in shapes else geom
    if len(shapes) != 50:
        sys.exit(f"expected 50 states, got {len(shapes)}")

    table = {}
    for st, geom in shapes.items():
        # Alaska crosses 180: its parts are split by sign, each walked in its own range
        parts = getattr(geom, "geoms", [geom])
        for part in parts:
            minx, miny, maxx, maxy = part.bounds
            for lon0 in range(int(math.floor(minx / 2) * 2), int(math.ceil(maxx)), 2):
                for lat0 in range(int(math.floor(miny)), int(math.ceil(maxy))):
                    cell = box(lon0, lat0, lon0 + 2, lat0 + 1)
                    area = part.intersection(cell).area
                    if area > 0:
                        g = grid_name(lon0, lat0)
                        table.setdefault(g, {})
                        table[g][st] = table[g].get(st, 0.0) + area

    lines = []
    for g in sorted(table):
        areas = table[g]
        total = sum(areas.values())
        states = [s for s, a in sorted(areas.items(), key=lambda kv: -kv[1]) if a / total >= MIN_SHARE]
        lines.append(g + " " + " ".join(states))
    with open(out_path, "w") as f:
        f.write("# JTDX-VU grid -> US state(s), largest share first; from US Census cb_us_state_500k\n")
        f.write("\n".join(lines) + "\n")
    print(f"{len(lines)} squares -> {out_path}")


if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2])
