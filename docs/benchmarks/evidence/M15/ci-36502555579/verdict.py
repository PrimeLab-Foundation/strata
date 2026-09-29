"""Classify ab_blocks output: resolved = CI excludes 0 and |effect| > A/A floor."""
import sys
from pathlib import Path


def rows(path):
    out = []
    for line in Path(path).read_text(encoding="utf-8", errors="replace").splitlines():
        p = line.split()
        if not p or not p[0].startswith(("sm-", "md-")) or len(p) < 12 or "blocks" in line:
            continue
        try:
            eff, lo, hi = (float(p[i].rstrip("%")) for i in (6, 7, 8))
            floor = float(p[11].rstrip("%"))
        except ValueError:
            continue
        out.append((p[0], p[1], eff, lo, hi, floor, p[9]))
    return out


def classify(r):
    _, _, eff, lo, hi, floor, _ = r
    if lo > 0 and eff > floor:
        return "LOSS"
    if hi < 0 and -eff > floor:
        return "GAIN"
    return ""


for leg in sys.argv[1:]:
    ab = Path(leg) / "ab"
    for name in ("R1_blocks.txt", "A2_blocks.txt", "comparison.txt"):
        f = ab / name
        if not f.is_file():
            continue
        rs = rows(f)
        flagged = [r for r in rs if classify(r)]
        worst = max(rs, key=lambda r: r[2]) if rs else None
        print(f"== {Path(leg).name} {name}: series={len(rs)} resolved={len(flagged)} "
              f"max={worst[0]} {worst[1]} {worst[2]:+.2f}% [{worst[3]:+.2f},{worst[4]:+.2f}] floor {worst[5]:.2f}" if worst else "")
        for r in flagged:
            print(f"   {classify(r)} {r[0]:22} {r[1]:14} {r[2]:+6.2f}% [{r[3]:+.2f},{r[4]:+.2f}] floor {r[5]:.2f} {r[6]}")
        over2 = [r for r in rs if r[2] > 2.0]
        for r in over2:
            print(f"   >+2% point: {r[0]} {r[1]} {r[2]:+.2f}% [{r[3]:+.2f},{r[4]:+.2f}] floor {r[5]:.2f} {classify(r) or 'unresolved'}")
