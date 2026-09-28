"""One table per analysis JSON written by benchmarks/ab_blocks.py --json.

past floor: |normalised effect| > the A/A floor of the same row/engine.
resolved:   past floor AND the block-bootstrap interval excludes zero.
usage: python3 summarize.py <analysis.json> [...]
"""

import json
import sys


def pct(x):
    return f"{x * 100:+.2f}%"


for path in sys.argv[1:]:
    (a,) = json.load(open(path))
    print(
        f"== {path.rsplit('/', 1)[-1]}  ({a['baseline']} vs {a['candidate']}, {a['blocks']} blocks)"
    )
    print(
        f"{'row':<22}{'engine':<14}{'effect':>9}{'ci low':>9}{'ci high':>9}{'floor':>8}{'pos':>6}  flag"
    )
    worst = None
    for r in a["rows"]:
        eff, lo, hi, fl = r["normalised"], r["ci_low"], r["ci_high"], r["aa_floor"]
        past = fl is not None and abs(eff) > fl
        resolved = past and (lo > 0 or hi < 0)
        flag = (
            ("RESOLVED " + ("loss" if eff > 0 else "gain"))
            if resolved
            else ("past floor, ci spans 0" if past else "")
        )
        print(
            f"{r['row']:<22}{r['engine']:<14}{pct(eff):>9}{pct(lo):>9}{pct(hi):>9}"
            f"{fl * 100:>7.2f}%{r['positive']:>3}/{len(r['block_effects'])}  {flag}"
        )
        canonical = r["engine"] != "strata-str"
        if canonical and (worst is None or eff > worst[0]):
            worst = (eff, r["row"], r["engine"], lo, hi, fl)
    print(
        f"max loss (canonical engines): {worst[1]} {worst[2]} {pct(worst[0])} "
        f"[{pct(worst[3])}, {pct(worst[4])}] floor {worst[5] * 100:.2f}%\n"
    )
