"""Item 2 (final): training counts of every function the candidate adds, from llvm-profdata.

Usage: new_func_counts.py <B profdata-show.txt> <A profdata-show.txt> <B plain obj dir> <A plain obj dir>

Inputs are `llvm-profdata show --all-functions --counts` of the two profiles (IR-level PGO:
each record carries its CFG hash, `Function count` = entry count, and block counters).
Prints, one line per record, for
  NEW      records B holds that A does not (functions the change added), and
  CHANGED  records in both whose CFG hash changed (functions the change edited),
the entry count, counter count, sum and max; then every plain-build function defined in an
object A does not have (the new TUs) or new in python_dumps.o that has no record at all.
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
        fc = re.search(r"Function count: (\d+)", block)
        bc = re.search(r"Block counts: \[([^\]]*)\]", block)
        counts = [int(x) for x in bc.group(1).split(",") if x.strip()] if bc else []
        out[m.group(1)] = (h, int(fc.group(1)) if fc else None, counts)
    return out


def defined(obj_dir):
    names = {}
    for obj in sorted(Path(obj_dir).glob("*.o")):
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


def where(name):
    prefix, _, _ = name.rpartition(";")
    return prefix.rsplit("/", 1)[-1] if prefix else "(external)"


def main():
    b, a = records(sys.argv[1]), records(sys.argv[2])
    new = {n: v for n, v in b.items() if n not in a}
    changed = {n: v for n, v in b.items() if n in a and a[n][0] != v[0]}
    pretty = demangle(n.rpartition(";")[2] for n in list(new) + list(changed))
    nz = [n for n, v in new.items() if any(v[2]) or (v[1] or 0) > 0]
    print(
        f"# B records {len(b)}, A records {len(a)}; new {len(new)} (nonzero {len(nz)}); "
        f"hash changed {len(changed)}"
    )
    for tag, group in (("NEW", new), ("CHANGED", changed)):
        rows = sorted(group.items(), key=lambda kv: (not any(kv[1][2]), where(kv[0]), kv[0]))
        for n, (h, fc, c) in rows:
            flag = "NONZERO" if any(c) or (fc or 0) > 0 else "zero"
            print(
                f"{tag:7s} {flag:7s} entry={fc!s:<9} counters={len(c):<4d} sum={sum(c):<10d} "
                f"max={max(c) if c else 0:<9d} {where(n):28s} {pretty[n.rpartition(';')[2]]}"
            )
    bdef, adef = defined(sys.argv[3]), defined(sys.argv[4])
    fresh = {s: o for s, o in bdef.items() if s not in adef}
    keys = {n.rpartition(";")[2] for n in b}
    missing = sorted(s for s in fresh if s not in keys)
    print(
        f"# plain-build functions new vs A: {len(fresh)}; without a profile record: {len(missing)}"
    )
    dm = demangle(missing)
    for s in missing:
        print(f"NORECORD {fresh[s]:24s} {dm[s]}")


if __name__ == "__main__":
    main()
