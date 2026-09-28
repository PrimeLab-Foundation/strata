"""Per-function __text size diff between two builds of the extension.

Usage: symsizes.py <before.so> <after.so>

Sizes are the distance to the next symbol in __text (nm -n), so a function's
size includes its alignment padding; the total over all symbols is the section.
"""

import subprocess
import sys


def sizes(path):
    out = subprocess.run(
        ["nm", "-n", "--defined-only", "-m", path], check=True, capture_output=True, text=True
    ).stdout
    syms = []
    for line in out.splitlines():
        parts = line.split()
        if "(__TEXT,__text)" not in line:
            continue
        syms.append((int(parts[0], 16), parts[-1]))
    sect = subprocess.run(["size", "-m", "-l", path], check=True, capture_output=True, text=True)
    end = None
    for line in sect.stdout.splitlines():
        if "Section __text" in line and "addr" in line:
            # "\tSection __text: 157484 (addr 0x7a0 offset 1952)"
            fields = line.replace(":", " ").replace("(", " ").split()
            size = int(fields[fields.index("__text") + 1])
            addr = int(fields[fields.index("addr") + 1], 16)
            end = addr + size
    result = {}
    for index, (addr, name) in enumerate(syms):
        nxt = syms[index + 1][0] if index + 1 < len(syms) else end
        result[name] = result.get(name, 0) + (nxt - addr)
    return result


def main():
    before, after = sizes(sys.argv[1]), sizes(sys.argv[2])
    rows = []
    for name in sorted(set(before) | set(after)):
        b, a = before.get(name), after.get(name)
        if b != a:
            rows.append(((a or 0) - (b or 0), b, a, name))
    rows.sort(key=lambda r: -abs(r[0]))
    for delta, b, a, name in rows:
        print(f"{delta:+6d}  {str(b):>6} -> {str(a):>6}  {name}")
    print(
        f"total  {sum(before.values())} -> {sum(after.values())}  "
        f"({sum(after.values()) - sum(before.values()):+d})"
    )


if __name__ == "__main__":
    main()
