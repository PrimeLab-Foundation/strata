"""Preserve native samples and complete Strata annotations for two call conditions.

Software CPU-clock samples describe where this process spends CPU time; they
are not hardware cache-miss counts or causal attribution. DSO-relative shares
and absolute sample counts must be considered together. No performance gate
or production ranking is derived from this diagnostic.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
import subprocess
import sys
from pathlib import Path


def sampled_symbols(report: str, limit: int = 8) -> list[str]:
    """Keep printed user symbols, including shares rounded to zero by perf."""
    symbols = []
    for line in report.splitlines():
        match = re.match(r"^\s*([0-9.]+)%\s+\[\.\]\s+(.+?)\s*$", line)
        if match and match[2] not in symbols:
            symbols.append(match[2])
    return symbols[:limit]


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--perf", default="perf")
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--rounds", type=int, default=20000)
    args = parser.parse_args(argv)
    if args.rounds < 1:
        parser.error("rounds must be positive")
    args.output.mkdir(parents=True, exist_ok=False)
    import strata._strata as extension

    binary = Path(extension.__file__).resolve()
    sidecar = Path(str(binary) + ".build.json")
    identity = json.loads(sidecar.read_text())
    digest = hashlib.sha256(binary.read_bytes()).hexdigest()
    if identity["extension_sha256"] != digest or not identity["complete_compilation"]:
        raise ValueError("a verified, completely compiled extension is required")
    shutil.copy2(binary, args.output / binary.name)
    shutil.copy2(sidecar, args.output / sidecar.name)
    commands = []

    def run(command, output):
        commands.append(command)
        (args.output / "commands.json").write_text(json.dumps(commands, indent=2) + "\n")
        with output.open("w") as stdout, Path(str(output) + ".stderr").open("w") as stderr:
            subprocess.run(command, stdout=stdout, stderr=stderr, check=True)

    run([args.perf, "--version"], args.output / "perf-version.txt")
    run(["lscpu"], args.output / "cpu.txt")
    for condition in ("resident", "interleaved"):
        data = args.output / f"{condition}.perf.data"
        metadata = args.output / f"{condition}.json"
        run(
            [
                args.perf,
                "record",
                "-e",
                "cpu-clock:u",
                "-F",
                "2999",
                "-o",
                str(data),
                "--",
                sys.executable,
                "-m",
                "benchmarks.profile_interleaved",
                str(args.rounds),
                "--condition",
                condition,
                "--metadata",
                str(metadata),
            ],
            args.output / f"{condition}.log",
        )
        observed = json.loads(metadata.read_text())
        if observed["extension"]["sha256"] != digest:
            raise ValueError("profile child measured a different extension")
        run(
            [
                args.perf,
                "report",
                "--stdio",
                "--no-children",
                "-g",
                "none",
                "--percent-limit",
                "0",
                "-i",
                str(data),
            ],
            args.output / f"{condition}.all.txt",
        )
        report = args.output / f"{condition}.strata.txt"
        run(
            [
                args.perf,
                "report",
                "--stdio",
                "--no-children",
                "-g",
                "none",
                "--percent-limit",
                "0",
                "--dsos",
                binary.name,
                "-F",
                "overhead,symbol",
                "-i",
                str(data),
            ],
            report,
        )
        symbols = sampled_symbols(report.read_text())
        (args.output / f"{condition}.symbols.json").write_text(json.dumps(symbols, indent=2) + "\n")
        if not symbols:
            raise RuntimeError(
                f"{condition}: no sampled Strata symbols; inspect retained perf logs"
            )
        for index, symbol in enumerate(symbols):
            run(
                [args.perf, "annotate", "--stdio", "-i", str(data), "--symbol", symbol],
                args.output / f"{condition}.annotate-{index}.txt",
            )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
