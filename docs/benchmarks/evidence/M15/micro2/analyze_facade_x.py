"""Read facade_x.tsv: per payload, strata.loads ns/call per arm (median, p25-p75 over all
samples), the per-process orjson-normalised ratio, and new/main - 1 on both.
usage: python3 analyze_facade_x.py <micro2 dir>
"""

import collections
import statistics
import sys

d = sys.argv[1]
raw = collections.defaultdict(list)  # (payload, arm, engine) -> samples
per_launch = collections.defaultdict(list)  # (payload, arm, launch, engine) -> samples
for line in open(f"{d}/facade_x.tsv"):
    arm, launch, payload, engine, _rep, ns = line.rstrip("\n").split("\t")
    raw[(payload, arm, engine)].append(float(ns))
    per_launch[(payload, arm, launch, engine)].append(float(ns))


def q(xs):
    qs = statistics.quantiles(xs, n=4)
    return statistics.median(xs), qs[0], qs[2]


for payload in ("tiny", "small-mixed"):
    rows = {}
    for arm in ("main", "new"):
        s = raw[(payload, arm, "strata.loads")]
        ratios = []
        for (p, a, launch, eng), xs in per_launch.items():
            if p == payload and a == arm and eng == "strata.loads":
                oj = statistics.median(per_launch[(p, a, launch, "orjson.loads")])
                ratios.extend(x / oj for x in xs)
        rows[arm] = (q(s), statistics.median(ratios), len(s))
    for arm, ((m, lo, hi), ratio, n) in rows.items():
        print(
            f"{payload:12s} {arm:5s} strata.loads {m:10.1f} ns ({lo:.1f}-{hi:.1f})  "
            f"norm {ratio:.4f}  n={n}"
        )
    (mn, _, _), rn, _ = rows["new"]
    (mm, _, _), rm, _ = rows["main"]
    print(
        f"{payload:12s} new/main-1: raw {(mn / mm - 1) * 100:+.2f}%  "
        f"({mn - mm:+.1f} ns)  normalised {(rn / rm - 1) * 100:+.2f}%"
    )
