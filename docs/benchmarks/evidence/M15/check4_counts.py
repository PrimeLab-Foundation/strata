"""Check 4: training counts of the native-types code in a PGO profile.

Usage: check4_counts.py <B profdata-show.txt> <A profdata-show.txt> <B plain obj dir> <A plain obj dir>

The two text files are `llvm-profdata show --all-functions --counts` of arm B's
and arm A's profiles (IR-level PGO: every function has a record, with its CFG
hash and counter values). Reported:
  1. every record B's profile holds that A's does not (functions the change
     added), grouped by origin, with counter count, total and maximum;
  2. every record in both whose CFG hash changed (functions the change edited);
  3. functions defined in B's plain python_native_types.o, python_parse_types.o
     and temporal.o (and absent from every A object) that have no profile
     record at all.
A record is NONZERO when any counter is nonzero.
"""

import re
import subprocess
import sys
from pathlib import Path


def records(path):
    text = Path(path).read_text()
    out = {}
    for block in re.split(r"\n(?=  \S.*:\n)", text):
        m = re.match(r"\s*(\S.*):\n", block)
        if not m or "Hash:" not in block:
            continue
        h = re.search(r"Hash: (0x[0-9a-f]+)", block).group(1)
        bc = re.search(r"Block counts: \[([^\]]*)\]", block)
        counts = [int(x) for x in bc.group(1).split(",") if x.strip()] if bc else []
        out[m.group(1)] = (h, counts)
    return out


def defined(obj_dir, stems=None):
    names = {}
    for obj in sorted(Path(obj_dir).glob("*.o")):
        if stems and obj.stem not in stems:
            continue
        nm = subprocess.run(["nm", "-m", str(obj)], capture_output=True, text=True, check=True)
        for line in nm.stdout.splitlines():
            if "(__TEXT,__text)" in line:
                sym = line.split()[-1]
                if not sym.startswith(("ltmp", "l_", "_OUTLINED")):
                    names[sym[1:]] = obj.stem
    return names


def demangle(names):
    names = list(names)
    out = subprocess.run(
        ["xcrun", "c++filt"], input="\n".join(names), capture_output=True, text=True, check=True
    ).stdout.splitlines()
    return dict(zip(names, out))


def origin(name):
    prefix, _, mangled = name.rpartition(";")
    if prefix:
        return prefix.rsplit("/", 1)[-1]
    if "N6strata8bindings6native" in mangled:
        return "native (external)"
    if "N6strata8bindings11parse_types" in mangled:
        return "parse_types (external)"
    if "N6strata4util" in mangled:
        return "strata::util (external)"
    return "other (external)"


def main():
    b, a = records(sys.argv[1]), records(sys.argv[2])
    new = {n: v for n, v in b.items() if n not in a}
    changed = {n: v for n, v in b.items() if n in a and a[n][0] != v[0]}
    pretty = demangle(n.rpartition(";")[2] for n in list(new) + list(changed))

    def row(n, v, tag=""):
        h, c = v
        flag = "NONZERO" if any(c) else "zero"
        return (
            f"{flag:7s} {tag}{origin(n):26s} counters={len(c):<4d} sum={sum(c):<10d} "
            f"max={max(c) if c else 0:<9d} {pretty[n.rpartition(';')[2]][:120]}"
        )

    print(
        f"# B records {len(b)}, A records {len(a)}; new in B {len(new)} "
        f"(nonzero {sum(1 for v in new.values() if any(v[1]))}); "
        f"hash changed {len(changed)}; only in A {len(set(a) - set(b))}"
    )
    print("## records new in B")
    for n in sorted(new, key=lambda n: (origin(n), n)):
        print(row(n, new[n]))
    print("## records in both whose CFG hash changed (edited functions)")
    for n in sorted(changed):
        print(row(n, changed[n]))
    print("## records only in A")
    for n in sorted(set(a) - set(b)):
        print("  ", demangle([n.rpartition(";")[2]])[n.rpartition(";")[2]][:120])
    in_a = defined(sys.argv[4])
    want = {
        s: o
        for s, o in defined(
            sys.argv[3], {"python_native_types", "python_parse_types", "temporal"}
        ).items()
        if s not in in_a
    }
    have = {n.rpartition(";")[2] for n in b}
    missing = sorted(s for s in want if s not in have)
    print(
        f"## plain-build functions new in the three TUs: {len(want)}; without a profile record: "
        f"{len(missing)}"
    )
    for s in missing:
        print(f"   no-record {want[s]:22s} {demangle([s])[s][:120]}")


if __name__ == "__main__":
    main()
