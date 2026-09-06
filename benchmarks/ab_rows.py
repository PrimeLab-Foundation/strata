"""`ab_builds.py`'s A-B-B-A driver, launching `rows_probe.py` instead.

The only difference from `benchmarks/ab_builds.py` is which unit of work each
launch runs: `rows_probe.py` takes `--row tier:dataset:op` and can therefore
put the small *and* medium `dumps flat` rows and the `loads flat` control in
one process, which the P0 probe's single `--tier` cannot.

Everything that decides a number is imported from `ab_builds` unchanged —
`_digest`, `_check_target` and, above all, `analyze`. This file contains no
statistics of its own.

usage:
  ab_rows.py --build A=<so> --build B=<so> --target <so> --out <tsv>
             --row small:flat:dumps [--row ...]
             [--order ABBA] [--blocks 6] [--tail A] [--repeat 60]
"""

from __future__ import annotations

import argparse
import os
import subprocess
import sys
from pathlib import Path

from benchmarks import ab_builds, ab_rounds


def run(args: argparse.Namespace) -> int:
    builds: dict[str, Path] = {}
    for entry in args.build:
        tag, _, path = entry.partition("=")
        if not path:
            raise SystemExit(f"--build wants TAG=PATH, got {entry!r}")
        builds[tag] = Path(path).resolve()
        if not builds[tag].exists():
            raise SystemExit(f"no such build: {builds[tag]}")
    target = ab_builds._check_target(Path(args.target))

    order = list(args.order) * args.blocks + list(args.tail)
    unknown = sorted(set(order) - set(builds))
    if unknown:
        raise SystemExit(f"order names builds that were not given: {unknown}")

    print(f"# target   {target}", file=sys.stderr)
    for tag, path in builds.items():
        print(f"# build {tag}  {path}  md5={ab_builds._digest(path)}", file=sys.stderr)
    print(f"# order    {''.join(order)} ({len(order)} launches)", file=sys.stderr)
    print(f"# rows     {' '.join(args.row)}  repeat={args.repeat}", file=sys.stderr)

    lines: list[str] = ["\t".join(ab_rounds.TSV_HEADER)]
    import shutil

    for index, tag in enumerate(order):
        shutil.copy2(builds[tag], target)
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
        result = subprocess.run(  # noqa: S603
            command,
            cwd=ab_builds.PROJECT_ROOT,
            capture_output=True,
            text=True,
            env={**os.environ, "PYTHONPATH": str(ab_builds.PROJECT_ROOT)},
            check=True,
        )
        sys.stderr.write(result.stderr)
        sys.stderr.flush()
        lines.extend(line for line in result.stdout.splitlines() if line.strip())

    Path(args.out).write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"# wrote {args.out}", file=sys.stderr)
    return ab_builds.analyze(argparse.Namespace(analyze=args.out, baseline_build=order[0]))


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
    args = parser.parse_args(argv)
    if len(args.build) < 2:
        parser.error("measuring needs --build TAG=PATH twice")
    return run(args)


if __name__ == "__main__":
    raise SystemExit(main())
