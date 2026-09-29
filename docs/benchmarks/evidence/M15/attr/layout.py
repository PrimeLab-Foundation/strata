"""Layout of the serializer's functions in _strata's __TEXT,__text (Mach-O arm64, PGO+LTO).

For each arm: every Serializer:: symbol and the hot helpers, in address order, with size
(distance to the next __text symbol), address mod 64, and the bl/b targets of the hot writers.
usage: python3 layout.py <label>=<path to _strata .so> ...  -> stdout
"""

import re
import subprocess
import sys

HOT = re.compile(
    r"Serializer::|write_scalar_run|format_double|write_int|write_string|dumps_to_python|"
    r"dumps_to_bytes|py_dumps|dump_to_file|py_dump\b|ScalarRun|OutputBuffer|write_digits|itoa"
)
HOT_WRITERS = [
    "write(_object*)",
    "write_sequence",
    "write_mapping_body",
    "write_record_fused",
    "write_string",
    "write_scalar_run",
]


def text_symbols(path):
    rows = []
    out = subprocess.run(["nm", "-nm", path], capture_output=True, text=True).stdout
    for line in out.splitlines():
        m = re.match(r"([0-9a-f]+) \(__TEXT,__text\) .*?(\S+)$", line)
        if m:
            rows.append((int(m.group(1), 16), m.group(2)))
    sec = subprocess.run(["otool", "-l", path], capture_output=True, text=True).stdout
    m = re.search(
        r"sectname __text\n\s+segname __TEXT\n\s+addr 0x([0-9a-f]+)\n\s+size 0x([0-9a-f]+)", sec
    )
    end = int(m.group(1), 16) + int(m.group(2), 16)
    rows.sort()
    res = []
    for i, (addr, name) in enumerate(rows):
        nxt = rows[i + 1][0] if i + 1 < len(rows) else end
        res.append((addr, nxt - addr, name))
    return res


def demangle(names):
    r = subprocess.run(["c++filt"], input="\n".join(names), capture_output=True, text=True)
    return dict(zip(names, r.stdout.splitlines()))


def short(d):
    d = d.replace("strata::bindings::(anonymous namespace)::", "")
    d = d.replace("strata::bindings::", "b::").replace("strata::", "")
    return d[:110]


def calls_from(path, mangled):
    dis = subprocess.run(
        ["otool", "-tV", "-p", mangled, path], capture_output=True, text=True
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
            name = (m.group(3) or m.group(2)) + (" [tail]" if m.group(1) == "b" else "")
            targets[name] = targets.get(name, 0) + 1
    return targets


def main():
    for arg in sys.argv[1:]:
        label, path = arg.split("=", 1)
        syms = text_symbols(path)
        dem = demangle([n for _, _, n in syms])
        first = syms[0][0]
        print(f"==== {label}: {path}")
        print(f"{'offset':>8} {'size':>6} {'mod64':>5}  symbol")
        idx_hot = []
        for i, (addr, size, name) in enumerate(syms):
            d = dem[name]
            if HOT.search(d):
                idx_hot.append(i)
        shown = set()
        for i in idx_hot:
            for j in (i - 1, i, i + 1):
                if 0 <= j < len(syms) and j not in shown:
                    shown.add(j)
        prev = None
        for j in sorted(shown):
            if prev is not None and j != prev + 1:
                print("     ...")
            addr, size, name = syms[j]
            mark = "*" if j in idx_hot else " "
            print(f"{addr - first:8x} {size:6d} {addr % 64:5d} {mark} {short(dem[name])}")
            prev = j
        print()
        for addr, size, name in syms:
            d = dem[name]
            if "Serializer::" in d and any(w in d for w in HOT_WRITERS):
                t = calls_from(path, name)
                td = demangle(sorted(t))
                print(f"-- calls from {short(d)} ({size} B)")
                for k in sorted(t, key=lambda k: td[k]):
                    print(f"   {t[k]:3d}  {short(td[k])}")
        print()


if __name__ == "__main__":
    main()
