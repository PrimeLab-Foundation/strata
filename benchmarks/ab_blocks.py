"""Block-paired, orjson-normalised effect per row from an `ab_builds.py` TSV.

For every launch the strata median is divided by the orjson median measured in
the same process (the drift control); consecutive launch pairs of one build
are averaged and each ABBA block yields one B/A effect. Printed per row and
engine: the per-block effects, their median and spread, and how many blocks
are positive. This is the estimator the P5 measurement review recomputed the
E26-P5 window with (build/evidence/P5-REVIEW-MEASUREMENT); it contains no
statistics `ab_floor.py` does not also report, only the per-block view.

usage: ab_blocks.py <tsv>
"""

import statistics, sys
from collections import defaultdict


def read(p):
    rows = []
    with open(p) as h:
        hdr = h.readline().rstrip("\n").split("\t")
        for l in h:
            if l.strip():
                rows.append(dict(zip(hdr, l.rstrip("\n").split("\t"))))
    return rows


g = defaultdict(list)
for r in read(sys.argv[1]):
    g[(r["build"], r["tag"], r["engine"], r["row"])].append(float(r["ms"]))
launches = sorted({(k[1], k[0]) for k in g})


def med(b, t, e, d):
    s = g.get((b, t, e, d))
    return statistics.median(s) if s else None


print(
    f"{'row':<24}{'eng':<14}{'blocks (normalised per-launch, ABBA-paired)':<62}{'median':>8}{'spread':>8}{'pos':>5}"
)
for d in sorted({k[3] for k in g}):
    for eng, riv in (
        ("strata-bytes", "orjson-bytes"),
        ("strata-str", "orjson-bytes"),
        ("strata-loads", "orjson-loads"),
    ):
        A = []
        B = []
        for t, b in launches:
            m = med(b, t, eng, d)
            o = med(b, t, riv, d)
            if m is None or o is None:
                continue
            (A if b == "A" else B).append(m / o)
        if not A or not B:
            continue
        n = min(len(A) // 2, len(B) // 2)
        eff = [
            statistics.fmean(B[i * 2 : i * 2 + 2]) / statistics.fmean(A[i * 2 : i * 2 + 2]) - 1
            for i in range(n)
        ]
        s = " ".join(f"{e * 100:+.2f}" for e in eff)
        print(
            f"{d:<24}{eng:<14}{s:<62}{statistics.median(eff) * 100:>7.2f}%{(max(eff) - min(eff)) * 100:>7.2f}{sum(1 for e in eff if e > 0):>4}/{len(eff)}"
        )
