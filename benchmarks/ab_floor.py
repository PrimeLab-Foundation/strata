"""What resolution does an A/A run leave? Two estimators, side by side.

Feed it the TSVs `ab_builds.py` wrote for two builds of *unchanged* source.
Every difference it prints is known to be zero, so what it prints is the
instrument's floor: the smallest effect that could be distinguished from
nothing on this machine, in this session, with these launch counts.

Two estimators, because they have different floors and the campaign needs
both:

* **raw** -- build B's launch medians against build A's. This is what a
  strata-versus-strata candidate comparison sees, and it carries every
  machine drift between launches.
* **normalised** -- the same, after dividing each launch's strata median by
  the orjson median measured *in the same process*. orjson is unchanged in
  every launch, so anything it moved by is the machine, and the ratio removes
  it. This is also the quantity the CI standings are made of (a rank is a
  within-run ratio), which makes it the estimator a "3% behind orjson" claim
  should be argued in.

For each estimator: the spread of per-launch values, a bootstrap interval on
the paired effect, and the half-width that an effect must exceed to be
distinguishable. Nothing here is a p-value; it is a floor.

usage: ab_floor.py <tsv> [<tsv> ...] [--baseline A] [--rival orjson-bytes]
"""

from __future__ import annotations

import argparse
import statistics
from collections import defaultdict
from pathlib import Path

from benchmarks import ab_rounds


def read(paths: list[Path]) -> dict[tuple[str, str, str, str, str], list[float]]:
    samples: dict[tuple[str, str, str, str, str], list[float]] = defaultdict(list)
    for path in paths:
        with open(path, encoding="utf-8") as handle:
            header = handle.readline().rstrip("\n").split("\t")
            for line in handle:
                if not line.strip():
                    continue
                row = dict(zip(header, line.rstrip("\n").split("\t"), strict=True))
                key = (path.name, row["build"], row["tag"], row["engine"], row["row"])
                samples[key].append(float(row["ms"]))
    return samples


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("tsv", nargs="+", type=Path)
    parser.add_argument("--baseline", default="A")
    parser.add_argument("--rival", default="orjson-bytes")
    args = parser.parse_args(argv)

    samples = read(args.tsv)
    files = sorted({key[0] for key in samples})
    datasets = sorted({key[4] for key in samples})
    engines = sorted({key[3] for key in samples if key[3] != args.rival})

    print(f"files: {files}")
    print(f"baseline build: {args.baseline}   drift control: {args.rival}")
    print()
    header = f"{'row':<14}{'engine':<14}{'estimator':<12}"
    header += f"{'launches':>9}{'effect':>10}{'ci low':>10}{'ci high':>10}"
    header += f"{'min':>9}{'max':>9}{'floor':>9}"
    print(header)
    for dataset in datasets:
        for engine in engines:
            for estimator in ("raw", "normalised"):
                per_build: dict[str, list[float]] = defaultdict(list)
                for key, values in sorted(samples.items()):
                    if key[4] != dataset or key[3] != engine:
                        continue
                    value = statistics.median(values)
                    if estimator == "normalised":
                        rival = samples.get((key[0], key[1], key[2], args.rival, dataset))
                        if not rival:
                            continue
                        value /= statistics.median(rival)
                    per_build[key[1]].append(value)
                base = per_build.get(args.baseline, [])
                other = [
                    v
                    for build, values in per_build.items()
                    if build != args.baseline
                    for v in values
                ]
                if not base or not other:
                    continue
                # Per-launch effects against the baseline's own median: the
                # spread of these is the floor a single launch pair carries.
                centre = statistics.median(base)
                singles = [value / centre - 1.0 for value in other]
                ratio = ab_rounds.paired_ratio(
                    other[: min(len(base), len(other))], base[: min(len(base), len(other))]
                )
                floor = max(abs(ratio.low - 1.0), abs(ratio.high - 1.0))
                print(
                    f"{dataset:<14}{engine:<14}{estimator:<12}"
                    f"{len(base) + len(other):>9}"
                    f"{(ratio.ratio - 1) * 100:>9.2f}%"
                    f"{(ratio.low - 1) * 100:>9.2f}%"
                    f"{(ratio.high - 1) * 100:>9.2f}%"
                    f"{min(singles) * 100:>8.2f}%"
                    f"{max(singles) * 100:>8.2f}%"
                    f"{floor * 100:>8.2f}%"
                )
    print()
    print("effect/ci: build B against build A -- known to be zero, so read them as noise.")
    print("min/max:   the extreme single-launch reading against the baseline's median.")
    print("floor:     the wider half of the interval; an effect below it is not resolvable here.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
