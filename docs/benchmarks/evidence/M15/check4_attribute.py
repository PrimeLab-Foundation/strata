"""Check 4 attribution: which trained tests reach the native code.

Usage: check4_attribute.py <tree> <attr-dir>

<tree> has arm B's instrumented image installed (PGO_MODE=generate, the same
image signature as arm B's phase 1). Runs, each with its own LLVM_PROFILE_FILE:
every test file of the training set (tests/py and tests/unit less the two
native_types directories, scripts/py_tests.py's TEST_PATHS/TRAINING_IGNORES),
one pytest process per file, and the training workload (scripts/pgo_training.py
on the tree's build/pgo training data). Each run's raw profiles are merged and
the counters of the watched functions summed. Prints one row per run with any
nonzero watched counter, then the totals, to compare with arm B's profile.
"""

import os
import re
import subprocess
import sys
from pathlib import Path

WATCH = {
    "write_native": "Serializer12write_nativeEP7_object",
    "classify": "_ZN6strata8bindings6native8classifyEP7_object",
    "format_pure_leaf": "_ZN6strata8bindings6native16format_pure_leafEP7_objectPc",
    "is_numpy": "_ZN6strata8bindings6native8is_numpyEP7_object",
    "resolve_class": "resolve_classERP11_typeobjectP7_objectS7_",
    "loaded_module": "loaded_moduleEP7_object",
    "class_attribute": "class_attributeEP7_objectS4_",
    "prepare_native_runtime": "_ZN6strata8bindings6native22prepare_native_runtimeEv",
    "reset_runtime": "_ZN6strata8bindings11parse_types13reset_runtimeEv",
}
PROFDATA = subprocess.run(
    ["xcrun", "--find", "llvm-profdata"], capture_output=True, text=True, check=True
).stdout.strip()


def counters(raw_dir):
    raws = sorted(Path(raw_dir).glob("*.profraw"))
    if not raws:
        return None
    merged = Path(raw_dir) / "merged.profdata"
    subprocess.run(
        [PROFDATA, "merge", "-o", str(merged), *map(str, raws)], check=True, capture_output=True
    )
    text = subprocess.run(
        [PROFDATA, "show", "--all-functions", "--counts", str(merged)],
        capture_output=True,
        text=True,
        check=True,
    ).stdout
    out = {}
    for block in re.split(r"\n(?=  \S.*:\n)", text):
        m = re.match(r"\s*(\S.*):\n", block)
        bc = re.search(r"Block counts: \[([^\]]*)\]", block)
        if not m or not bc:
            continue
        for label, needle in WATCH.items():
            if needle in m.group(1):
                out[label] = out.get(label, 0) + sum(
                    int(x) for x in bc.group(1).split(",") if x.strip()
                )
    return out


def run(tree, raw_dir, argv, env_extra=None):
    raw_dir.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    env["LLVM_PROFILE_FILE"] = str(raw_dir / "%p-%m.profraw")
    env.update(env_extra or {})
    proc = subprocess.run(argv, cwd=tree, env=env, capture_output=True, text=True)
    tail = [l for l in proc.stdout.splitlines() if re.search(r"passed|failed|error", l)]
    return proc.returncode, (tail[-1] if tail else "")


def main():
    tree, attr = Path(sys.argv[1]), Path(sys.argv[2])
    py = str(tree / ".venv/bin/python")
    files = sorted(
        str(p.relative_to(tree))
        for base in ("tests/py", "tests/unit")
        for p in (tree / base).rglob("test_*.py")
        if "native_types" not in p.parts
    )
    runs = [(f, [py, "-m", "pytest", "-q", "-p", "no:cacheprovider", f]) for f in files]
    runs.append(
        (
            "workload: scripts/pgo_training.py",
            [
                py,
                "scripts/pgo_training.py",
                "--json",
                "build/pgo/train.json",
                "--ndjson",
                "build/pgo/train.ndjson",
                "--work-dir",
                str(attr / "work"),
            ],
        )
    )
    totals = {}
    print(f"# runs: {len(runs)} ({len(files)} test files + the training workload)")
    for i, (label, argv) in enumerate(runs):
        rc, tail = run(
            tree,
            attr / "raw" / f"r{i:03d}",
            argv,
            {"PYTHONPATH": "."} if label.startswith("workload") else None,
        )
        c = counters(attr / "raw" / f"r{i:03d}") or {}
        for k, v in c.items():
            totals[k] = totals.get(k, 0) + v
        hot = {
            k: v for k, v in c.items() if v and k not in ("prepare_native_runtime", "reset_runtime")
        }
        if hot or rc:
            print(f"{label}  rc={rc}  [{tail}]  " + " ".join(f"{k}={v}" for k, v in hot.items()))
    print(
        "# counter sums over all runs: " + " ".join(f"{k}={v}" for k, v in sorted(totals.items()))
    )


if __name__ == "__main__":
    main()
