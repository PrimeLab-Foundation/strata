"""`ab_builds.py`'s A-B-B-A driver, launching `rows_probe.py` instead.

The only difference from `benchmarks/ab_builds.py` is which unit of work each
launch runs: `rows_probe.py` takes `--row tier:dataset:op` and can therefore
put the small *and* medium `dumps flat` rows and the `loads flat` control in
one process, which the P0 probe's single `--tier` cannot. It also reaches the
file operations — `load`, NDJSON `load` and `dump` — which the `dumps`-only
probe cannot measure at all.

Everything that decides a number is imported from `ab_builds` unchanged —
`drive` (the swap, the incremental TSV, the restore in `finally`), `_digest`,
`_check_target` and `analyze`, which is itself a view over
`benchmarks/ab_blocks`. This file contains no statistics of its own.

usage:
  ab_rows.py --build A=<so> --build B=<so> --target <so> --out <tsv>
             --row small:flat:dumps [--row ...]
             [--order ABBA] [--blocks 6] [--tail A] [--repeat 60]
"""

from __future__ import annotations

import argparse
import os
import sys
from pathlib import Path

from benchmarks import ab_builds


def run(args: argparse.Namespace) -> int:
    builds = ab_builds.parse_builds(args.build)
    target = ab_builds._check_target(Path(args.target))
    order = ab_builds.plan_order(args.order, args.blocks, args.tail, builds)

    print(f"# target   {target}", file=sys.stderr)
    for tag, path in builds.items():
        print(f"# build {tag}  {path}  md5={ab_builds._digest(path)}", file=sys.stderr)
    print(f"# order    {''.join(order)} ({len(order)} launches)", file=sys.stderr)
    print(f"# rows     {' '.join(args.row)}  repeat={args.repeat}", file=sys.stderr)

    def command_for(index: int, tag: str) -> list[str]:
        command = [
            sys.executable,
            "benchmarks/rows_probe.py",
            "--build",
            tag,
            "--tag",
            f"L{index:02d}",
            "--repeat",
            str(args.repeat),
            "--identify",
        ]
        for spec in args.row:
            command += ["--row", spec]
        return command

    ab_builds.drive(
        order,
        builds,
        target,
        Path(args.out),
        ab_builds.subprocess_launch(command_for, dict(os.environ)),
    )
    return ab_builds.analyze(
        argparse.Namespace(analyze=args.out, baseline_build=order[0], min_samples=args.min_samples)
    )


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", action="append", default=[], help="TAG=PATH")
    parser.add_argument("--target", required=True)
    parser.add_argument("--out", required=True)
    parser.add_argument("--row", action="append", required=True)
    parser.add_argument("--order", default="ABBA")
    parser.add_argument("--blocks", type=int, default=6)
    parser.add_argument("--tail", default="A")
    parser.add_argument("--repeat", type=int, default=60)
    parser.add_argument("--min-samples", type=int, default=1)
    args = parser.parse_args(argv)
    if len(args.build) < 2:
        parser.error("measuring needs --build TAG=PATH twice")
    return run(args)


if __name__ == "__main__":
    raise SystemExit(main())
