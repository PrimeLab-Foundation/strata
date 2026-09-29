"""Item 2 (final; copy of ../check4_entry_counts.py with python_parse_types_walk.cpp and
python_module.cpp added, python_module filtered by the substrings too). Check 4: function entry counts the profile gives the new code.

Usage: check4_entry_counts.py <tree> <_strata .build.json> <out-dir> <function-substring> ...

For python_dumps.cpp, python_native_types.cpp, python_parse_types.cpp and
temporal.cpp, re-runs the build's own compile line (same flags, same
-fprofile-use) with `-S -emit-llvm -mllvm -print-after=pgo-instr-use` and reads
each defined function's `function_entry_count` right after the profile was
attached (before inlining removes any function). Prints the entry count of
every function of the three new TUs and of the python_dumps functions whose
mangled name contains one of the given substrings.
"""

import json
import re
import subprocess
import sys
from pathlib import Path

TUS = (
    "src/strata/bindings/python_dumps.cpp",
    "src/strata/bindings/python_native_types.cpp",
    "src/strata/bindings/python_parse_types.cpp",
    "src/strata/bindings/python_parse_types_walk.cpp",
    "src/strata/bindings/python_module.cpp",
    "src/strata/util/temporal.cpp",
)


def entry_counts(dump):
    md = {m.group(1): m.group(2) for m in re.finditer(r"^(![0-9]+) = (.*)$", dump, re.M)}
    out = {}
    for m in re.finditer(r"^define [^\n]*?@([\w.$]+)\([^\n]*$", dump, re.M):
        prof = re.search(r"!prof (![0-9]+)", m.group(0))
        count = None
        if prof:
            c = re.search(r'function_entry_count", i64 (\d+)', md.get(prof.group(1), ""))
            count = int(c.group(1)) if c else None
        out[m.group(1)] = count
    return out


def main():
    tree, build_json, out_dir, *needles = sys.argv[1:]
    Path(out_dir).mkdir(parents=True, exist_ok=True)
    commands = json.loads(Path(build_json).read_text())["commands"]
    rows = []
    for tu in TUS:
        cmd = list(next(c for c in commands if tu in c))
        cmd[cmd.index("-o") + 1] = "/dev/null"
        cmd += ["-S", "-emit-llvm", "-mllvm", "-print-after=pgo-instr-use"]
        proc = subprocess.run(cmd, cwd=tree, capture_output=True, text=True)
        if proc.returncode:
            raise SystemExit(proc.stderr[-2000:])
        warnings = [l for l in proc.stderr.splitlines() if "warning:" in l and "unused" not in l]
        counts = entry_counts(proc.stderr)
        stem = Path(tu).stem
        for w in warnings:
            print(f"# {stem}: {w.strip()[:200]}")
        for name, count in counts.items():
            if stem not in ("python_dumps", "python_module") or any(n in name for n in needles):
                rows.append((stem, name, count))
    names = [r[1] for r in rows]
    pretty = subprocess.run(
        ["xcrun", "c++filt"], input="\n".join(names), capture_output=True, text=True, check=True
    ).stdout.splitlines()
    for (stem, name, count), p in sorted(
        zip(rows, pretty), key=lambda x: (x[0][0], -(x[0][2] or 0))
    ):
        flag = "NONZERO" if count else ("zero" if count == 0 else "no-count")
        print(f"{flag:8s} {stem:20s} entry={str(count):>8s}  {p[:130]}")


if __name__ == "__main__":
    main()
