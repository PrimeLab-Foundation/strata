"""Check 4 attribution, per test: which tests of the given files reach write_native.

Usage: check4_attribute_nodes.py <tree> <attr-dir> <test-file> ...

Same instrumented image as check4_attribute.py. Collects each file's test ids
and runs every test in its own pytest process (4 at a time), each with its own
LLVM_PROFILE_FILE. A test's write_native entry count is read from native::is_numpy,
which the unsupported path calls exactly once per write_native entry (its one
nonzero counter; arm B's profile: 842 = write_native's entry count 842). Prints
every test with a nonzero count, then per-file totals.
"""

import concurrent.futures
import os
import re
import subprocess
import sys
from pathlib import Path

PROFDATA = subprocess.run(
    ["xcrun", "--find", "llvm-profdata"], capture_output=True, text=True, check=True
).stdout.strip()
IS_NUMPY = "_ZN6strata8bindings6native8is_numpyEP7_object"
WRITE_NATIVE = "Serializer12write_nativeEP7_object"


def measure(tree, raw_dir, node):
    raw_dir.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    env["LLVM_PROFILE_FILE"] = str(raw_dir / "%p-%m.profraw")
    py = str(tree / ".venv/bin/python")
    proc = subprocess.run(
        [py, "-m", "pytest", "-q", "-p", "no:cacheprovider", node],
        cwd=tree,
        env=env,
        capture_output=True,
        text=True,
    )
    raws = sorted(map(str, raw_dir.glob("*.profraw")))
    merged = raw_dir / "merged.profdata"
    subprocess.run([PROFDATA, "merge", "-o", str(merged), *raws], check=True, capture_output=True)
    text = subprocess.run(
        [PROFDATA, "show", "--all-functions", "--counts", str(merged)],
        capture_output=True,
        text=True,
        check=True,
    ).stdout
    got = {}
    for label, needle in (("entries", IS_NUMPY), ("write_native_sum", WRITE_NATIVE)):
        m = re.search(r"\S*" + needle + r":\n(?:.*\n){2}\s*Block counts: \[([^\]]*)\]", text)
        got[label] = sum(int(x) for x in m.group(1).split(",") if x.strip()) if m else 0
    return node, proc.returncode, got


def main():
    tree, attr, *files = sys.argv[1:]
    tree, attr = Path(tree), Path(attr)
    py = str(tree / ".venv/bin/python")
    nodes = []
    for f in files:
        out = subprocess.run(
            [py, "-m", "pytest", "--collect-only", "-q", "-p", "no:cacheprovider", f],
            cwd=tree,
            capture_output=True,
            text=True,
        ).stdout
        nodes += [l.strip() for l in out.splitlines() if "::" in l]
    print(f"# tests: {len(nodes)} in {len(files)} files")
    results = []
    with concurrent.futures.ThreadPoolExecutor(4) as pool:
        futs = [
            pool.submit(measure, tree, attr / "raw-nodes" / f"n{i:04d}", n)
            for i, n in enumerate(nodes)
        ]
        for fut in futs:
            results.append(fut.result())
    per_file = {}
    for node, rc, got in results:
        f = node.split("::")[0]
        per_file[f] = per_file.get(f, 0) + got["entries"]
        if got["entries"] or got["write_native_sum"] or rc:
            print(f"{got['entries']:5d} entries  rc={rc}  {node}")
    for f, n in per_file.items():
        print(f"# {f}: {n} write_native entries summed over its tests")


if __name__ == "__main__":
    main()
