"""Per-function sizes of _strata's __TEXT,__text in the M15 arms (Mach-O, arm64).

Size of a symbol = distance to the next __text symbol (the section end for the last);
`nm -nm` lists local symbols too (the images are not stripped of locals). Inlining is read
from which callees exist as out-of-line symbols and from the `bl` targets of `write()`
(otool -tV), since the PGO+LTO images carry no DWARF.
usage: python3 symbol_table.py <evidence M15 dir>  -> stdout
"""

import re
import subprocess
import sys

ARMS = ["armA", "armA2", "armBheld2", "armBown2"]
SO = "_strata.cpython-314-darwin.so"
WRITE = "__ZN6strata8bindings12_GLOBAL__N_110Serializer5writeEP7_object"


def text_symbols(path):
    rows = []
    for line in subprocess.run(
        ["nm", "-nm", path], capture_output=True, text=True
    ).stdout.splitlines():
        m = re.match(r"([0-9a-f]+) \(__TEXT,__text\) .*?(\S+)$", line)
        if m:
            rows.append((int(m.group(1), 16), m.group(2)))
    sec = subprocess.run(["otool", "-l", path], capture_output=True, text=True).stdout
    m = re.search(
        r"sectname __text\n\s+segname __TEXT\n\s+addr 0x([0-9a-f]+)\n\s+size 0x([0-9a-f]+)", sec
    )
    end = int(m.group(1), 16) + int(m.group(2), 16)
    rows.sort()
    out = {}
    for i, (addr, name) in enumerate(rows):
        nxt = rows[i + 1][0] if i + 1 < len(rows) else end
        out[name] = (addr, nxt - addr)
    return out


def demangle(names):
    res = subprocess.run(["c++filt"], input="\n".join(names), capture_output=True, text=True)
    return dict(zip(names, res.stdout.splitlines()))


def calls_from(path, symbol):
    dis = subprocess.run(
        ["otool", "-tV", "-p", symbol, path], capture_output=True, text=True
    ).stdout
    targets = {}
    labels = 0
    for line in dis.splitlines()[2:]:
        if line.endswith(":"):
            labels += 1
            if labels > 1:
                break
            continue
        m = re.search(r"\s(bl|b)\s+(\S+)(?:\s*; symbol stub for: (\S+))?", line)
        if m and (m.group(1) == "bl" or m.group(2).startswith("_") or m.group(3)):
            name = (m.group(3) or m.group(2)) + (" [tail b]" if m.group(1) == "b" else "")
            targets[name] = targets.get(name, 0) + 1
    return targets


def main():
    base = sys.argv[1]
    tables = {arm: text_symbols(f"{base}/{arm}/{SO}") for arm in ARMS}
    names = sorted(set().union(*tables.values()))
    dem = demangle(names)
    print("size of every __text symbol, bytes (0 = no out-of-line symbol)")
    print("\t".join(ARMS) + "\tsymbol")
    for name in sorted(names, key=lambda n: -max(tables[a].get(n, (0, 0))[1] for a in ARMS)):
        print("\t".join(str(tables[a].get(name, (0, 0))[1]) for a in ARMS) + "\t" + dem[name])
    print()
    for arm in ARMS:
        total = sum(size for _, size in tables[arm].values())
        print(f"{arm}: {len(tables[arm])} symbols, {total} bytes")
    print()
    for arm in ["armA", "armBheld2", "armBown2"]:
        path = f"{base}/{arm}/{SO}"
        t = calls_from(path, WRITE)
        tdem = demangle(sorted(t))
        print(f"== bl targets of Serializer::write in {arm} ({tables[arm][WRITE][1]} bytes)")
        for k in sorted(t):
            print(f"  {t[k]:3d}  {tdem[k]}")


if __name__ == "__main__":
    main()
