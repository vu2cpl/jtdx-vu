#!/usr/bin/env python3
"""Score an ft2_autotest run: per level, how many of the six QSO messages each
decoder got (JTDX-VU plain / via AP7, MSHV), from the two ALL files and the
player's schedule.  usage: ft2_score.py RESULTS_DIR"""
import os, re, sys, time, calendar
d = sys.argv[1]
MSGS = ["CQ K1ABC FN42", "K1ABC W9XYZ EN37", "W9XYZ K1ABC -10", "K1ABC W9XYZ R-12", "W9XYZ K1ABC RR73", "K1ABC W9XYZ 73"]
runs = []   # (t_start, level, rep)
snr_of = {}
for line in open(os.path.join(d, "schedule.txt")):
    p = line.split()
    if len(p) >= 6 and p[2] == "level": runs.append((float(p[0]), float(p[3]), int(p[5]))); snr_of[float(p[3])] = float(p[7]) if len(p) >= 8 else None
def run_of(t):
    for (ts, g, r) in runs:
        if ts - 1 <= t < ts + 30 - 1: return (g, r), int((t - ts + 1) // 3.75)
    return None, None
def utc(s):   # yyyymmdd_hhmmss -> epoch
    return calendar.timegm(time.strptime(s[:15], "%Y%m%d_%H%M%S"))
jt = {}   # (g, rep, k) -> ('plain'|'ap7', snr)
other_jt = []
f = os.path.join(d, "jtdx_ALL.TXT")
if os.path.exists(f):
    for line in open(f, errors="replace"):
        m = re.match(r"(\d{8}_\d{6})\s+(-?\d+)\s+(-?[\d.]+)\s+(\d+)\s+:\s+(.*?)\s*$", line)
        if not m: continue
        t, snr, dt, fq, msg = m.groups()
        ap7 = msg.endswith(" 7") or msg.endswith("\t7")
        body = re.sub(r"\s+7$", "", msg).strip()
        body = re.sub(r"\s+[a-zA-Z*]$", "", body).strip() if body not in MSGS else body
        run, k = run_of(utc(t))
        if run is None or body not in MSGS: other_jt.append(line.strip()); continue
        key = (run[0], run[1], MSGS.index(body))
        if key not in jt or (ap7 and jt[key][0] == "plain"): jt[key] = ("ap7" if ap7 else "plain", int(snr))
ms = {}     # (g, rep, k) -> ('plain'|'ap7', snr)
other_ms = []
f = os.path.join(d, "mshv_ALL.TXT")
cur_day = None
if os.path.exists(f):
    for line in open(f, errors="replace"):
        line = line.rstrip("\n")
        md = re.match(r"UTC Date: (\d{4}) (\d{2}) (\d{2})", line)
        if md: cur_day = "".join(md.groups()); continue
        parts = line.split("|")
        # dd|RX 14074 FT2|hhmmss|snr|dt|?|message|0 or AP7|?|freq
        if len(parts) < 8 or not parts[1].startswith("RX") or cur_day is None: continue
        tm, snr, msg, flag = parts[2], parts[3], parts[6].strip(), parts[7].strip()
        if not re.fullmatch(r"\d{6}", tm): other_ms.append(line); continue
        run, k = run_of(utc(cur_day + "_" + tm))
        if run is None or msg not in MSGS: other_ms.append(line); continue
        key = (run[0], run[1], MSGS.index(msg))
        v = ("ap7" if flag.startswith("AP7") else "plain", int(float(snr)))
        if key not in ms or (v[0] == "ap7" and ms[key][0] == "plain"): ms[key] = v
levels = sorted({g for (_, g, _) in runs}, reverse=True)
reps = max((r for (_, _, r) in runs), default=-1) + 1
print(f"{'SNR':>5} | {'JTDX plain':>10} {'JTDX AP7':>8} {'JTDX all':>8} | {'MSHV plain':>10} {'MSHV AP7':>8} {'MSHV all':>8} | of {6*reps}   (reported SNR: JTDX / MSHV)")
for g in levels:
    jp = [v for (gg, r, k), v in jt.items() if gg == g and v[0] == "plain"]
    ja = [v for (gg, r, k), v in jt.items() if gg == g and v[0] == "ap7"]
    mp = [v for (gg, r, k), v in ms.items() if gg == g and v[0] == "plain"]
    ma = [v for (gg, r, k), v in ms.items() if gg == g and v[0] == "ap7"]
    import math
    est = snr_of.get(g) if snr_of.get(g) is not None else 20 * math.log10(g) + 32.3
    js = sum(v[1] for v in jp + ja) / max(1, len(jp) + len(ja)); mss = sum(v[1] for v in mp + ma) / max(1, len(mp) + len(ma))
    print(f"{est:>5.0f} | {len(jp):>10} {len(ja):>8} {len(jp)+len(ja):>8} | {len(mp):>10} {len(ma):>8} {len(mp)+len(ma):>8} |        ({js:.0f} / {mss:.0f})")
print("\nper message, pairs = JTDX,MSHV (CQ, reply, report, R-report, RR73, 73): J/M = plain decode, 7 = via AP7, . = missed")
for g in levels:
    row = []
    for r in range(reps):
        cell = ""
        for k in range(6):
            j = jt.get((g, r, k)); m = ms.get((g, r, k))
            cell += ("7" if j and j[0] == "ap7" else "J" if j else ".") + ("7" if m and m[0] == "ap7" else "M" if m else ".") + " "
        row.append(cell.strip())
    print(f"{(snr_of.get(g) if snr_of.get(g) is not None else g):>6}: " + " | ".join(row))
if other_jt: print(f"\nJTDX lines outside the plan or not a QSO message ({len(other_jt)}):"); [print("  " + l) for l in other_jt[:20]]
if other_ms: print(f"\nMSHV lines not matched ({len(other_ms)}):"); [print("  " + l) for l in other_ms[:20]]
