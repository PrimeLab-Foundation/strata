"""What runs before the CPU guard in a strata image, scanned for x86-64-v3 code.

Usage: python3 check_guard_isa.py IMAGE [IMAGE...]

For each x86-64 ELF or Mach-O image: its static initializers (ELF
.init_array/.ctors/DT_INIT, Mach-O __mod_init_func/__init_offsets) and the
local functions they call, then the `PyInit_*` entry itself, disassembled and
scanned for instructions beyond the x86-64 baseline (isa_scan.py's rules: VEX
and EVEX by encoding, the non-VEX v2/v3 instructions by mnemonic). Every call
the entry makes is printed with its target so the listing shows that the
module body is reached only after the check. Exit status 1 on any finding.
"""

import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from isa_scan import LINE, NON_VEX_ABOVE_BASELINE, opcode_kind  # noqa: E402


def run(*args):
    return subprocess.run(args, capture_output=True, text=True, check=True).stdout


def objdump_is_llvm():
    return "LLVM" in run("objdump", "--version")


def disassemble(image, symbol):
    if objdump_is_llvm():
        out = run("objdump", "-d", "--x86-asm-syntax=intel", f"--disassemble-symbols={symbol}", image)
    else:
        out = run("objdump", "-d", "-M", "intel", "-w", f"--disassemble={symbol}", image)
    return out


def scan(listing):
    bad, calls, count = [], [], 0
    for line in listing.splitlines():
        m = LINE.match(line)
        if not m or not m.group(3).strip():
            continue
        count += 1
        text = m.group(3).strip()
        raw = [int(b, 16) for b in m.group(2).split()]
        mnem = text.split()[0]
        kind = opcode_kind(raw)
        if kind or NON_VEX_ABOVE_BASELINE.match(mnem):
            bad.append(f"{m.group(1)}: {kind or 'v2/v3'} {text}")
        if mnem == "call" or (mnem == "jmp" and "<" in text and "+0x" not in text):
            calls.append(f"{m.group(1)}: {text}")
    return count, bad, calls


def symbols(image):
    """address -> name for defined text symbols."""
    table = {}
    for line in run("nm", "-n", image).splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[1] in "tTwW":
            table.setdefault(int(parts[0], 16), parts[2])
    return table


def elf_initializers(image):
    sections = {}
    for line in run("readelf", "-SW", image).splitlines():
        m = re.search(r"\]\s+(\.\S+)\s+\S+\s+([0-9a-f]+)\s+[0-9a-f]+\s+([0-9a-f]+)", line)
        if m:
            sections[m.group(1)] = (int(m.group(2), 16), int(m.group(3), 16))
    targets = []
    relocs = run("readelf", "-rW", image)
    for name in (".init_array", ".ctors", ".preinit_array"):
        if name not in sections:
            continue
        start, size = sections[name]
        print(f"  {name}: {size // 8} entries")
        for line in relocs.splitlines():
            m = re.match(r"\s*([0-9a-f]+)\s+[0-9a-f]+\s+R_X86_64_(RELATIVE|64)\s+(.*)$", line)
            if m and start <= int(m.group(1), 16) < start + size:
                targets.append(int(m.group(3).split()[-1], 16))
    dynamic = run("readelf", "-dW", image)
    m = re.search(r"\(INIT\)\s+0x([0-9a-f]+)", dynamic)
    if m:
        print("  DT_INIT present")
        targets.append(int(m.group(1), 16))
    return targets


def macho_initializers(image):
    out = run("otool", "-l", image)
    found = [s for s in ("__mod_init_func", "__init_offsets") if f"sectname {s}" in out]
    for s in ("__mod_init_func", "__init_offsets"):
        print(f"  {s}: {'present' if s in found else 'absent'}")
    if found:
        raise SystemExit("Mach-O initializers present: extend this script to resolve them")
    return []


def check(image):
    print(f"== {image}")
    head = Path(image).read_bytes()[:4]
    is_elf = head == b"\x7fELF"
    table = symbols(image)
    failures = 0
    print("static initializers:")
    pending = elf_initializers(image) if is_elf else macho_initializers(image)
    seen = set()
    while pending:
        address = pending.pop()
        if address in seen:
            continue
        seen.add(address)
        name = table.get(address, hex(address))
        count, bad, calls = scan(disassemble(image, name))
        print(f"  [{name}] instructions={count} above-baseline={len(bad)}")
        failures += len(bad)
        for b in bad:
            print("    BAD", b)
        for c in calls:
            m = re.search(r"(?:call|jmp)\s+(?:0x)?([0-9a-f]+)\s+<([^>@+]+)>", c)
            if m and "@plt" not in c and int(m.group(1), 16) in table:
                pending.append(int(m.group(1), 16))
    entries = [n for n in table.values() if re.fullmatch(r"_?PyInit_\w+", n)]
    for entry in entries:
        count, bad, calls = scan(disassemble(image, entry))
        print(f"init entry [{entry}] instructions={count} above-baseline={len(bad)}")
        failures += len(bad)
        for b in bad:
            print("    BAD", b)
        for c in calls:
            print("    CALL", c)
    return failures


def main():
    failures = sum(check(image) for image in sys.argv[1:])
    print(f"RESULT: {'FAIL' if failures else 'PASS'} ({failures} above-baseline instructions)")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
