"""Mach-O: symbol sizes from address deltas (llvm-nm -n); PE: function sizes from .pdata."""
import re
import subprocess
import sys
from pathlib import Path

LLVM = "/opt/homebrew/opt/llvm/bin/"
HOT = re.compile(
    r"Serializer::(write\(|write_string\(|write_string_bytes|write_mapping_body|write_mapping\(|"
    r"write_record_fused\(|write_scalar_run|write_int|Frame::Frame|push_open|close_container|latch|"
    r"write_mapping_uncached|write_native|write_key_cold|write_dataclass|write_set|write_enum|write_decimal)"
    r"|ParserInline<strata::bindings::PythonObjectBuilder>::(parse_val|parse_array|parse_object|scan_string)"
    r"|strata_loads|strata_dumps"
)


def macho(path):
    out = subprocess.run([LLVM + "llvm-nm", "-n", "-C", "--defined-only", str(path)],
                         capture_output=True, text=True).stdout
    rows = []
    for line in out.splitlines():
        p = line.split(None, 2)
        if len(p) == 3 and p[1].lower() == "t":
            rows.append((int(p[0], 16), p[2].replace("strata::bindings::(anonymous namespace)::", "")))
    table = {}
    for (addr, name), (nxt, _) in zip(rows, rows[1:] + [(rows[-1][0], "")]):
        table[name] = table.get(name, 0) + (nxt - addr)
    return table


def pe_sizes(path):
    out = subprocess.run([LLVM + "llvm-readobj", "--unwind", str(path)], capture_output=True, text=True).stdout
    sizes = []
    for m in re.finditer(r"StartAddress: .*?\(0x([0-9A-F]+)\).*?EndAddress: .*?\(0x([0-9A-F]+)\)", out, re.S):
        sizes.append(int(m.group(2), 16) - int(m.group(1), 16))
    return sorted(sizes, reverse=True)


for leg in sys.argv[1:]:
    arms = Path(leg) / "arms"
    if (arms / "A.pyd").exists():
        t = {a: pe_sizes(arms / f"{a}.pyd") for a in ("A", "A2", "B")}
        print(f"== {Path(leg).name} (.pdata function ranges): count A={len(t['A'])} A2={len(t['A2'])} B={len(t['B'])}")
        for i in range(25):
            print(f"   #{i+1:2d} {t['A'][i]:7d} {t['A2'][i]:7d} {t['B'][i]:7d}")
        continue
    t = {a: macho(arms / f"{a}.so") for a in ("A", "A2", "B")}
    print(f"== {Path(leg).name}")
    for n in sorted({n for x in t.values() for n in x if HOT.search(n)}):
        a, a2, b = (t[x].get(n, 0) for x in ("A", "A2", "B"))
        flag = "" if a == b else ("  <- B" + ("" if a == a2 else " (A2 too)"))
        print(f"   {a:6d} {a2:6d} {b:6d}  {n[:85]}{flag}")
