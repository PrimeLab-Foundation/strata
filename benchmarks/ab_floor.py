"""What resolution does an A/A run leave? The floor, in the one estimator.

Feed it the TSVs `ab_builds.py`/`ab_rows.py` wrote for two builds of
*unchanged* source. Every difference it prints is known to be zero, so what it
prints is the instrument's floor: the smallest effect that could be
distinguished from nothing on this machine, in this session, with these
launch counts.

This is a **view over `benchmarks/ab_blocks`**, not a second estimator. It used
to bootstrap individual launches and report a ratio of medians while
`ab_blocks` reported a median of ABBA block effects, so the interval printed
here was not an interval for the effect printed there — the defect the
7 September review recorded as finding 4, and the reason the E26-P6 reading
rule mixed two statistics. It also defaulted to `--rival orjson-bytes`, which
silently produced no normalised reading at all for the `loads` rows; the rival
is now chosen per operation by `ab_blocks.RIVAL_BY_ENGINE`.

What it adds to `ab_blocks.py` is the framing: the effect column is noise by
construction, and the `floor` column — `max(|ci low|, |ci high|)`, the end of
the block-bootstrap interval furthest from zero — is the number a candidate
effect must exceed before it is an effect at all. A control of fewer than
`ab_blocks.MIN_CONTROL_BLOCKS` blocks cannot carry a 95% interval at all, and
says so in its own output rather than leaving the block count to be noticed.

usage: ab_floor.py <tsv> [<tsv> ...] [--baseline A] [--pair A:B] [--min-samples N]
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

from benchmarks import ab_blocks


def render_floor(analysis: ab_blocks.Analysis) -> str:
    lines = [
        f"file: {analysis.path}",
        f"baseline: {analysis.structure.base}   other arm: {analysis.structure.other}   "
        f"blocks: {len(analysis.structure.blocks)}",
        "",
        f"{'row':<22}{'engine':<14}{'rival':<14}{'blk':>4}"
        f"{'effect':>10}{'ci low':>10}{'ci high':>10}{'floor':>9}{'pos':>6}",
    ]
    for series in analysis.series:
        lines.append(
            f"{series.row:<22}{series.engine:<14}{series.rival:<14}{series.blocks:>4}"
            f"{series.effect * 100:>9.2f}%{series.interval.low * 100:>9.2f}%"
            f"{series.interval.high * 100:>9.2f}%{series.interval.floor * 100:>8.2f}%"
            f"{series.positive:>3}/{series.blocks}"
        )
    lines += [
        "",
        "effect/ci: the same block estimator ab_blocks.py reports -- known to be zero here.",
        "floor:     max(|ci low|, |ci high|); an effect below it is not resolvable.",
    ]
    warning = ab_blocks.control_warning(analysis)
    if warning is not None:
        lines += ["", warning]
    return "\n".join(lines)


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("tsv", nargs="+", type=Path)
    parser.add_argument("--baseline", default=None)
    parser.add_argument("--pair", type=ab_blocks._pair, default=None)
    parser.add_argument("--min-samples", type=int, default=ab_blocks.DEFAULT_MIN_SAMPLES)
    return parser


def main(argv: list[str] | None = None) -> int:
    args = build_parser().parse_args(argv)

    for index, path in enumerate(args.tsv):
        if index:
            print()
        analysis = ab_blocks.analyze(
            path, baseline=args.baseline, pair=args.pair, min_samples=args.min_samples
        )
        print(render_floor(analysis))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except ab_blocks.AnalysisError as error:
        print(f"error: {error}", file=sys.stderr)
        raise SystemExit(2) from None
