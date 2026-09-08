"""Full canonical before/candidate reports from two test-gated PGO binaries.

Each arm runs in a fresh process with a staged copy of the Python facade.
The installed extension and metadata are never replaced. Reports remain
diagnostic evidence: patched builds cannot become clean CI standings.
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

from benchmarks.ab_builds import PROJECT_ROOT, _check_build_identity

DATASETS = (
    "users.json",
    "users.ndjson",
    "flat.json",
    "nested.json",
    "wide_arrays.json",
    "mixed.json",
)
BOOTSTRAP = """
import pathlib, sys
stage = pathlib.Path(sys.argv.pop(1))
sys.path.insert(0, str(stage))
import strata._strata as extension
assert pathlib.Path(extension.__file__).resolve().parent == stage / 'strata'
from benchmarks.bench_main import main
raise SystemExit(main())
"""


def measure(binary: Path, output: Path, data: Path, tier: str, repeat: int) -> None:
    """Stage only this arm; even a failing child cannot damage the install."""
    with tempfile.TemporaryDirectory(prefix="strata-canonical-") as scratch:
        stage = Path(scratch).resolve()
        package = stage / "strata"
        shutil.copytree(
            PROJECT_ROOT / "python" / "strata",
            package,
            ignore=shutil.ignore_patterns("_strata*", "__pycache__"),
        )
        target = package / ("_strata" + sysconfig.get_config_var("EXT_SUFFIX"))
        shutil.copy2(binary, target)
        shutil.copy2(
            binary.with_name(binary.name + ".build.json"),
            target.with_name(target.name + ".build.json"),
        )
        command = [
            sys.executable,
            "-c",
            BOOTSTRAP,
            str(stage),
            "--name",
            tier,
            "--repeat",
            str(repeat),
            "--warmup",
            "2",
            "--output",
            str(output),
        ]
        for dataset in DATASETS:
            command.extend(("--dataset", str(data / tier / dataset)))
        subprocess.run(command, cwd=PROJECT_ROOT, check=True)


def run(
    before: Path, candidate: Path, output: Path, data: Path, tiers: list[str], repeat: int
) -> int:
    for binary in (before, candidate):
        _check_build_identity(binary)
    # Preflight all tiers before spending time on the first one.
    for tier in tiers:
        for dataset in DATASETS:
            if not (data / tier / dataset).is_file():
                raise ValueError(f"missing dataset: {data / tier / dataset}")
    output.mkdir(parents=True, exist_ok=False)
    results = []
    for tier in tiers:
        name = f"bench_results_{tier}.md"
        for arm, binary in (("A", before), ("B", candidate)):
            print(f"Canonical {tier}: {arm}", flush=True)
            measure(binary, output / arm / name, data, tier, repeat)
        baseline = output / f"baseline-{tier}.json"
        command = [sys.executable, "-m", "benchmarks.regression_check"]
        subprocess.run(
            [*command, str(output / "A" / name), "--baseline", str(baseline), "--save-baseline"],
            cwd=PROJECT_ROOT,
            check=True,
        )
        with (output / f"gate-{tier}.txt").open("w", encoding="utf-8") as log:
            result = subprocess.run(
                [*command, str(output / "B" / name), "--baseline", str(baseline)],
                cwd=PROJECT_ROOT,
                stdout=log,
                stderr=subprocess.STDOUT,
                check=False,
            )
        results.append({"tier": tier, "gate_exit_code": result.returncode})
        (output / "gates.json").write_text(json.dumps(results, indent=2) + "\n")
        print((output / f"gate-{tier}.txt").read_text(), flush=True)
    return int(any(result["gate_exit_code"] != 0 for result in results))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--before", type=Path, required=True)
    parser.add_argument("--candidate", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--data", type=Path, default=PROJECT_ROOT / "benchmarks/data/generated")
    parser.add_argument("--tier", choices=("small", "medium", "large"), action="append")
    parser.add_argument("--repeat", type=int, default=60)
    args = parser.parse_args()
    if args.repeat < 10:
        parser.error("canonical regression checks require at least 10 repeats")
    tiers = list(dict.fromkeys(args.tier or ["small"]))
    return run(
        args.before.resolve(),
        args.candidate.resolve(),
        args.output.resolve(),
        args.data.resolve(),
        tiers,
        args.repeat,
    )


if __name__ == "__main__":
    raise SystemExit(main())
