"""Compare two builds of the extension by swapping the `.so` between processes.

A build comparison on a desktop that is also being used has one honest shape,
and this is it:

* **Same virtualenv, same interpreter, same rival binaries.** Only the strata
  extension changes between launches, so nothing else can explain a difference.
* **Fresh process per launch.** A loaded shared library cannot be replaced in
  place; a driver that pretends otherwise is measuring the first build twice.
* **A-B-B-A blocks.** Linear drift over a block cancels inside the block: the
  mean of the two A launches and the mean of the two B launches sit at the same
  point in time. The order is predeclared on the command line and every launch
  is kept, including the ones that disagree.
* **A repeated baseline at the end.** The trailing A against the leading A is
  the session's own drift, measured rather than assumed.
* **A/A first.** Run this with two builds of *unchanged* source before running
  it with a candidate. Whatever spread A/A shows is the floor; an effect
  smaller than the floor is not an effect, and this tool cannot make it one.

Safety: the driver overwrites the extension of the checkout it lives in and
refuses any other target. Point it at another tree's `.so` and it exits.

usage:
  ab_builds.py --build A=<so> --build B=<so> --target <so> --out <tsv>
               [--order ABBA] [--blocks 3] [--tail A] [--repeat 60]
  ab_builds.py --analyze <tsv> [--baseline-build A]
"""

from __future__ import annotations

import argparse
import hashlib
import shutil
import statistics
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

from benchmarks import ab_rounds

PROJECT_ROOT = Path(__file__).resolve().parents[1]


def _digest(path: Path) -> str:
    return hashlib.md5(path.read_bytes()).hexdigest()  # noqa: S324


def _check_target(target: Path) -> Path:
    resolved = target.resolve()
    try:
        resolved.relative_to(PROJECT_ROOT)
    except ValueError:
        raise SystemExit(
            f"refusing to write outside this checkout: {resolved} is not under {PROJECT_ROOT}"
        ) from None
    return resolved


def run(args: argparse.Namespace) -> int:
    builds = {}
    for entry in args.build:
        tag, _, path = entry.partition("=")
        if not path:
            raise SystemExit(f"--build wants TAG=PATH, got {entry!r}")
        builds[tag] = Path(path).resolve()
        if not builds[tag].exists():
            raise SystemExit(f"no such build: {builds[tag]}")
    target = _check_target(Path(args.target))

    order = list(args.order) * args.blocks + list(args.tail)
    unknown = sorted(set(order) - set(builds))
    if unknown:
        raise SystemExit(f"order names builds that were not given: {unknown}")

    print(f"# target   {target}", file=sys.stderr)
    for tag, path in builds.items():
        print(f"# build {tag}  {path}  md5={_digest(path)}", file=sys.stderr)
    print(f"# order    {''.join(order)} ({len(order)} launches)", file=sys.stderr)

    lines: list[str] = ["\t".join(ab_rounds.TSV_HEADER)]
    for index, tag in enumerate(order):
        shutil.copy2(builds[tag], target)
        command = [
            sys.executable,
            "benchmarks/dumps_rows_probe.py",
            "--build",
            tag,
            "--tag",
            f"L{index:02d}",
            "--tier",
            args.tier,
            "--repeat",
            str(args.repeat),
            "--identify",
        ]
        for dataset in args.dataset or []:
            command += ["--dataset", dataset]
        print(f"# launch {index:02d} build={tag}", file=sys.stderr, flush=True)
        result = subprocess.run(  # noqa: S603
            command,
            cwd=PROJECT_ROOT,
            capture_output=True,
            text=True,
            env={**args.env, "PYTHONPATH": str(PROJECT_ROOT)},
            check=True,
        )
        sys.stderr.write(result.stderr)
        lines.extend(line for line in result.stdout.splitlines() if line.strip())

    Path(args.out).write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"# wrote {args.out}", file=sys.stderr)
    return analyze(argparse.Namespace(analyze=args.out, baseline_build=order[0]))


def _read(path: Path) -> list[dict[str, str]]:
    rows = []
    with open(path, encoding="utf-8") as handle:
        header = handle.readline().rstrip("\n").split("\t")
        for line in handle:
            if not line.strip() or line.startswith("#"):
                continue
            rows.append(dict(zip(header, line.rstrip("\n").split("\t"), strict=True)))
    return rows


def analyze(args: argparse.Namespace) -> int:
    rows = _read(Path(args.analyze))
    # launch medians: (build, launch, engine, row) -> median ms
    grouped: dict[tuple[str, str, str, str], list[float]] = defaultdict(list)
    for row in rows:
        grouped[(row["build"], row["tag"], row["engine"], row["row"])].append(float(row["ms"]))

    launches = sorted({(key[1], key[0]) for key in grouped})
    builds = sorted({key[0] for key in grouped})
    engines = sorted({key[2] for key in grouped})
    datasets = sorted({key[3] for key in grouped})
    base = args.baseline_build or builds[0]
    others = [build for build in builds if build != base]

    print(f"launches: {' '.join(f'{tag}:{build}' for tag, build in launches)}")
    print(f"builds: {builds}   baseline: {base}")
    print()
    print("== launch medians (ms)")
    header = "row/engine".ljust(30) + "".join(f"{tag}:{build:<8}" for tag, build in launches)
    print(header)
    for dataset in datasets:
        for engine in engines:
            cells = []
            for tag, build in launches:
                samples = grouped.get((build, tag, engine, dataset))
                cells.append(f"{statistics.median(samples):<11.4f}" if samples else f"{'-':<11}")
            print(f"{dataset + ' ' + engine:<30}" + "".join(cells))

    print()
    print("== paired A-B blocks (block = one occurrence of each build, in order)")
    print("   effect = median(other)/median(base) - 1 per block, then the median of blocks")
    for dataset in datasets:
        for engine in engines:
            per_build: dict[str, list[float]] = defaultdict(list)
            for tag, build in launches:
                samples = grouped.get((build, tag, engine, dataset))
                if samples:
                    per_build[build].append(statistics.median(samples))
            for other in others:
                a_values, b_values = per_build.get(base, []), per_build.get(other, [])
                if not a_values or not b_values:
                    continue
                blocks = min(len(a_values) // 2, len(b_values) // 2)
                effects = []
                for index in range(blocks):
                    a_mean = statistics.fmean(a_values[index * 2 : index * 2 + 2])
                    b_mean = statistics.fmean(b_values[index * 2 : index * 2 + 2])
                    effects.append(b_mean / a_mean - 1.0)
                if not effects:
                    effects = [statistics.median(b_values) / statistics.median(a_values) - 1.0]
                spread = max(effects) - min(effects)
                overall = statistics.median(b_values) / statistics.median(a_values) - 1.0
                print(
                    f"{dataset:<14}{engine:<14}{other} vs {base}: "
                    f"blocks {' '.join(f'{value * 100:+.2f}%' for value in effects)}  "
                    f"median-of-launches {overall * 100:+.2f}%  block spread {spread * 100:.2f}pp"
                )

    print()
    print("== drift: the trailing baseline launch against the leading one")
    first = [entry for entry in launches if entry[1] == base][0]
    last = [entry for entry in launches if entry[1] == base][-1]
    for dataset in datasets:
        for engine in engines:
            head = grouped.get((base, first[0], engine, dataset))
            tail = grouped.get((base, last[0], engine, dataset))
            if head and tail:
                change = statistics.median(tail) / statistics.median(head) - 1.0
                print(f"{dataset:<14}{engine:<14}{first[0]} -> {last[0]}: {change * 100:+.2f}%")
    return 0


def main(argv: list[str] | None = None) -> int:
    import os

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--analyze", help="read a TSV this tool wrote and report, without measuring"
    )
    parser.add_argument("--baseline-build", default=None)
    parser.add_argument("--build", action="append", default=[], help="TAG=PATH")
    parser.add_argument("--target", help="the .so to overwrite (must be in this checkout)")
    parser.add_argument("--out", help="TSV of every sample")
    parser.add_argument("--order", default="ABBA")
    parser.add_argument("--blocks", type=int, default=3)
    parser.add_argument("--tail", default="A", help="launches appended after the blocks")
    parser.add_argument("--repeat", type=int, default=60)
    parser.add_argument("--tier", default="small")
    parser.add_argument("--dataset", action="append", default=None)
    args = parser.parse_args(argv)
    args.env = dict(os.environ)

    if args.analyze:
        return analyze(args)
    missing = [name for name in ("target", "out") if not getattr(args, name)]
    if missing or len(args.build) < 2:
        parser.error("measuring needs --build TAG=PATH twice, --target and --out")
    return run(args)


if __name__ == "__main__":
    raise SystemExit(main())
