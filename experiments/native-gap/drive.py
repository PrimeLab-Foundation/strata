"""A-B-B-A launches of native_probe.py over hook-image arms, and their reading.

Each arm is a directory holding a copy of the facade and `_strata` from the
checkout plus the arm's own `_dumps_hook` image, so no launch ever overwrites
the checkout's extension, and every arm runs the same `_strata` bytes.

    drive.py prepare --arm base=python/strata/_dumps_hook...so --arm lto=build/x.so --dir D
    drive.py run --dir D --order ABBA --blocks 4 --arms base,lto --out o.jsonl -- --row rec ...
    drive.py read o.jsonl [--base base]

`read` prints, per row and library, each arm's median per-call time across
all its launches' samples, and per non-base arm the median of the per-block
paired ratios (arm / base, strata only) with the range over blocks, plus
strata / msgspec per arm.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import shutil
import statistics
import subprocess
import sys
import sysconfig
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
FACADE = ROOT / "python" / "strata"
SUFFIX = sysconfig.get_config_var("EXT_SUFFIX")


def _digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()[:16]


def prepare(args) -> int:
    base = Path(args.dir)
    for spec in args.arm:
        tag, image = spec.split("=", 1)
        package = base / tag / "strata"
        if package.exists():
            shutil.rmtree(package)
        shutil.copytree(
            FACADE,
            package,
            ignore=shutil.ignore_patterns(
                "__pycache__", "_dumps_hook*", "*.build.json", "*cpython-31[0-3]*"
            ),
        )
        shutil.copy2(image, package / f"_dumps_hook{SUFFIX}")
        print(
            f"{tag}: hook {_digest(Path(image))}  _strata {_digest(package / f'_strata{SUFFIX}')}"
        )
    return 0


def _order(pattern: str, blocks: int, arms: list[str]) -> list[str]:
    letters = {chr(ord("A") + i): arm for i, arm in enumerate(arms)}
    return [letters[c] for _ in range(blocks) for c in pattern]


def run(args, probe_args: list[str]) -> int:
    arms = args.arms.split(",")
    order = _order(args.order, args.blocks, arms)
    out = Path(args.out)
    with out.open("a", encoding="utf-8") as sink:
        for launch, arm in enumerate(order):
            env = dict(os.environ)
            env["PYTHONPATH"] = str(Path(args.dir).resolve() / arm)
            env["ARM"] = arm
            command = [
                sys.executable,
                str(Path(__file__).with_name("native_probe.py")),
                *probe_args,
            ]
            result = subprocess.run(  # noqa: S603
                command, cwd=ROOT, env=env, capture_output=True, text=True, check=False
            )
            if result.returncode != 0:
                sys.stderr.write(result.stderr)
                raise SystemExit(f"launch {launch} ({arm}) failed")
            for line in result.stdout.splitlines():
                record = json.loads(line)
                record["launch"] = launch
                record["block"] = launch // len(args.order)
                sink.write(json.dumps(record) + "\n")
            sink.flush()
            print(f"launch {launch:02d} {arm} done", file=sys.stderr, flush=True)
    return 0


def read(args) -> int:
    records = [json.loads(line) for line in Path(args.path).read_text().splitlines() if line]
    by = defaultdict(list)  # (row, lib, arm) -> samples
    per_block = defaultdict(list)  # (row, lib, arm, block) -> samples
    for r in records:
        by[(r["row"], r["lib"], r["tag"])].extend(r["s"])
        per_block[(r["row"], r["lib"], r["tag"], r["block"])].extend(r["s"])
    rows = sorted({k[0] for k in by}, key=[r["row"] for r in records].index)
    arms = sorted({k[2] for k in by}, key=[r["tag"] for r in records].index)
    base = args.base or arms[0]
    blocks = sorted({k[3] for k in per_block})
    print(f"{'row':<16}{'lib':<9}" + "".join(f"{a:>14}" for a in arms) + "   n")
    for row in rows:
        libs = sorted({k[1] for k in by if k[0] == row}, key=["strata", "msgspec", "orjson"].index)
        for lib in libs:
            cells = []
            count = 0
            for arm in arms:
                samples = by.get((row, lib, arm))
                if samples:
                    cells.append(f"{statistics.median(samples) * 1e6:>12.2f}us")
                    count = len(samples)
                else:
                    cells.append(f"{'-':>14}")
            print(f"{row:<16}{lib:<9}" + "".join(cells) + f"  {count}")
        for arm in arms:
            s = by.get((row, "strata", arm))
            m = by.get((row, "msgspec", arm))
            if s and m:
                print(
                    f"{'':<16}strata/msgspec [{arm}] {statistics.median(s) / statistics.median(m):.3f}"
                )
        for arm in arms:
            if arm == base:
                continue
            ratios = []
            for block in blocks:
                a = per_block.get((row, "strata", base, block))
                b = per_block.get((row, "strata", arm, block))
                if a and b:
                    ratios.append(statistics.median(b) / statistics.median(a))
            if ratios:
                print(
                    f"{'':<16}{arm}/{base} strata: median {statistics.median(ratios):.4f} "
                    f"[{min(ratios):.4f}, {max(ratios):.4f}] over {len(ratios)} blocks"
                )
    return 0


def main(argv: list[str] | None = None) -> int:
    argv = sys.argv[1:] if argv is None else argv
    probe_args: list[str] = []
    if "--" in argv:
        split = argv.index("--")
        argv, probe_args = argv[:split], argv[split + 1 :]
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("prepare")
    p.add_argument("--arm", action="append", required=True)
    p.add_argument("--dir", required=True)
    r = sub.add_parser("run")
    r.add_argument("--dir", required=True)
    r.add_argument("--arms", required=True)
    r.add_argument("--order", default="ABBA")
    r.add_argument("--blocks", type=int, default=3)
    r.add_argument("--out", required=True)
    q = sub.add_parser("read")
    q.add_argument("path")
    q.add_argument("--base")
    args = parser.parse_args(argv)
    if args.command == "prepare":
        return prepare(args)
    if args.command == "run":
        return run(args, probe_args)
    return read(args)


if __name__ == "__main__":
    raise SystemExit(main())
