"""Rebuild `strata._dumps_hook` out of tree with extra flags (phase-0 attribution).

Replays the compile and link commands `setup.py` recorded for the installed
hook image (its `*.build.json` companion), in a scratch directory of its own,
with flags appended to every compile and link command. Nothing in the checkout
is written: the image lands at `--out`. `_strata` is never touched.

    build_hook_variant.py --out arm/_dumps_hook.so --flag=-flto=thin
    build_hook_variant.py --out arm/_dumps_hook.so --flag=-fprofile-generate
    build_hook_variant.py --out arm/_dumps_hook.so --flag=-flto=thin \
        --flag=-fprofile-use=hook.profdata

The recorded commands are the provenance: this reproduces what CI's
`pip install` ran for the hook, plus the named flags, and nothing else.
"""

from __future__ import annotations

import argparse
import json
import shutil
import subprocess
import sys
import sysconfig
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def _companion() -> Path:
    suffix = sysconfig.get_config_var("EXT_SUFFIX")
    return ROOT / "python" / "strata" / f"_dumps_hook{suffix}.build.json"


def _rewrite(command: list[str], scratch: Path, out: Path, flags: list[str]) -> list[str]:
    # The recorded build may itself be profiled (make pgo's phase 3): start plain.
    command = [p for p in command if not p.startswith(("-fprofile-", "-flto"))]
    rewritten: list[str] = []
    is_link = "-c" not in command
    iterator = iter(range(len(command)))
    for index in iterator:
        part = command[index]
        if part == "-o":
            target = command[index + 1]
            next(iterator)
            rewritten += ["-o", str(out) if is_link else str(scratch / Path(target).name)]
            continue
        if is_link and part.endswith(".o"):
            rewritten.append(str(scratch / Path(part).name))
            continue
        rewritten.append(part)
    return rewritten + flags


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--flag", action="append", default=[])
    parser.add_argument("--jobs", type=int, default=8)
    args = parser.parse_args(argv)

    record = json.loads(_companion().read_text(encoding="utf-8"))
    if record["source"]["dirty"]:
        print("note: the recorded build came from a dirty tree", file=sys.stderr)
    commands = record["commands"]
    args.out.parent.mkdir(parents=True, exist_ok=True)
    scratch = Path(tempfile.mkdtemp(prefix="hookvar-"))
    try:
        compiles = [_rewrite(c, scratch, args.out, args.flag) for c in commands if "-c" in c]
        links = [_rewrite(c, scratch, args.out, args.flag) for c in commands if "-c" not in c]
        names = [Path(c[c.index("-o") + 1]).name for c in compiles]
        if len(set(names)) != len(names):
            raise SystemExit("object basenames collide; extend _rewrite to keep directories")
        running: list[subprocess.Popen] = []
        for command in compiles:
            running.append(subprocess.Popen(command, cwd=ROOT))  # noqa: S603
            if len(running) >= args.jobs:
                if running.pop(0).wait() != 0:
                    raise SystemExit("compile failed")
        for process in running:
            if process.wait() != 0:
                raise SystemExit("compile failed")
        for command in links:
            subprocess.run(command, cwd=ROOT, check=True)  # noqa: S603
    finally:
        shutil.rmtree(scratch, ignore_errors=True)
    print(args.out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
