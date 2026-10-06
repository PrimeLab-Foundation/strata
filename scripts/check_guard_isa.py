#!/usr/bin/env python3
"""Scan what runs before the CPU guard in a strata image for above-baseline x86-64 code.

Usage: ``python scripts/check_guard_isa.py IMAGE [IMAGE...]``

For each x86-64 ELF or thin Mach-O image: its static initializers (ELF
``.init_array``, ``.ctors``, ``.preinit_array`` and ``DT_INIT``; Mach-O
``__mod_init_func`` and ``__init_offsets``) and the local functions they call,
then each ``PyInit_*`` entry with its outlined ``.cold`` parts and every defined
function they reach by direct call or tail jump, transitively at any depth, short
of the module body (``create_strata_module``, ``create_hook_module``), which runs
only after the check (``src/strata/bindings/python_cpu_guard.h``). Each is
disassembled and scanned for instructions beyond the x86-64 baseline: VEX and
EVEX by encoding, the non-VEX v2/v3 instructions by mnemonic. Every call an entry
makes is printed with its target, so the listing shows where the module body is
reached.

Exit status 0 when clean; 1 on any above-baseline instruction, a symbol that
disassembles to nothing, or an image with no ``PyInit_*`` entry; 2 when a tool
the scan needs is not on ``PATH``. ``find_tools`` is the probe
``scripts/release.py check-install --identity`` runs before calling this script
on x86-64 POSIX. Promoted from ``docs/benchmarks/evidence/T7/``, whose copy stays
as T7 recorded it.
"""

from __future__ import annotations

import re
import shutil
import subprocess
import sys
from collections.abc import Callable
from pathlib import Path

ELF_MAGIC = b"\x7fELF"
EM_X86_64 = 62
MACHO_MAGIC_64 = b"\xcf\xfa\xed\xfe"
CPU_TYPE_X86_64 = 0x01000007
# The tools each format's scan runs, each with the names it answers to, in order.
TOOLS = {
    "elf": {
        "objdump": ("objdump", "llvm-objdump"),
        "nm": ("nm", "llvm-nm"),
        "readelf": ("readelf", "llvm-readelf"),
    },
    "macho": {
        "objdump": ("objdump", "llvm-objdump"),
        "nm": ("nm", "llvm-nm"),
        "otool": ("otool", "llvm-otool"),
    },
}
LEGACY_PREFIXES = frozenset({0x66, 0x67, 0xF2, 0xF3, 0x2E, 0x3E, 0x26, 0x64, 0x65, 0x36, 0xF0})
NON_VEX_ABOVE_BASELINE = re.compile(
    r"^(popcnt|lzcnt|tzcnt|movbe|crc32|cmpxchg16b|"
    r"lddqu|haddp[sd]|hsubp[sd]|addsubp[sd]|movddup|movs[hl]dup|"  # SSE3
    r"pshufb|palignr|pmaddubsw|ph(add|sub)(s?w|d)|pabs[bwd]|psign[bwd]|pmulhrsw|"  # SSSE3
    r"pblendvb|blendv?p[sd]|pblendw|pmin[su][bdw]|pmax[su][bdw]|ptest|pmov[sz]x\w+|"
    r"pinsr[bdq]|pextr[bdq]|round[sp][sd]|dpp[sd]|pmulld|pmuldq|pcmpeqq|packusdw|"
    r"insertps|extractps|mpsadbw|phminposuw|movntdqa|"  # SSE4.1
    r"pcmpgtq|pcmp[ei]str[im])$",  # SSE4.2
)
LINE = re.compile(r"^\s*([0-9a-f]+):\s+((?:[0-9a-f]{2} )+)\s*(.*)$")
CALL_TARGET = re.compile(r"(?:call|jmp)\s+(?:0x)?([0-9a-f]+)\s+<([^>@+]+)>")
ENTRY = re.compile(r"_?PyInit_\w+?(?:\.cold(?:\.[0-9]+)?)?")
# The module body each entry reaches only once the check has passed.
MODULE_BODY = re.compile(r"create_(?:strata|hook)_module")

Scanner = Callable[[str], tuple[int, list[str], list[str]]]


def _run(*args: str) -> str:
    return subprocess.run(args, capture_output=True, text=True, check=True).stdout


def image_format(image: Path) -> str:
    """``"elf"`` or ``"macho"`` for an x86-64 image; anything else exits."""
    with image.open("rb") as handle:
        head = handle.read(20)
    if head[:4] == ELF_MAGIC and int.from_bytes(head[18:20], "little") == EM_X86_64:
        return "elf"
    if head[:4] == MACHO_MAGIC_64 and int.from_bytes(head[4:8], "little") == CPU_TYPE_X86_64:
        return "macho"
    raise SystemExit(f"{image}: not an x86-64 ELF or thin x86-64 Mach-O image")


def find_tools(fmt: str) -> tuple[dict[str, str], list[str]]:
    """Each tool ``fmt``'s scan runs, resolved on ``PATH``, and the tools that are not."""
    found: dict[str, str] = {}
    missing: list[str] = []
    for tool, names in TOOLS[fmt].items():
        path = next((hit for hit in map(shutil.which, names) if hit), None)
        if path is None:
            missing.append(tool)
        else:
            found[tool] = path
    return found, missing


def opcode_kind(raw: list[int]) -> str:
    """``"VEX"``/``"EVEX"`` when the opcode byte after legacy and REX prefixes is one.

    In 64-bit mode C4/C5 and 62 at that position are always VEX and EVEX.
    """
    i = 0
    while i < len(raw) and raw[i] in LEGACY_PREFIXES:
        i += 1
    if i < len(raw) and 0x40 <= raw[i] <= 0x4F:
        i += 1
    if i < len(raw) and raw[i] in (0xC4, 0xC5):
        return "VEX"
    if i < len(raw) and raw[i] == 0x62:
        return "EVEX"
    return ""


def scan(listing: str) -> tuple[int, list[str], list[str]]:
    """Instruction count, above-baseline instructions and calls of one disassembly."""
    bad: list[str] = []
    calls: list[str] = []
    count = 0
    for line in listing.splitlines():
        m = LINE.match(line)
        if not m or not m.group(3).strip():  # continuation line of a long instruction
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


def _disassemble(objdump: str, llvm: bool, image: Path, symbol: str) -> str:
    if llvm:
        return _run(
            objdump,
            "-d",
            "--x86-asm-syntax=intel",
            f"--disassemble-symbols={symbol}",
            str(image),
        )
    return _run(objdump, "-d", "-M", "intel", "-w", f"--disassemble={symbol}", str(image))


def _symbols(nm: str, image: Path) -> dict[int, str]:
    """Address -> name of the defined text symbols, in address order."""
    table: dict[int, str] = {}
    for line in _run(nm, "-n", str(image)).splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[1] in "tTwW":
            table.setdefault(int(parts[0], 16), parts[2])
    return table


def _elf_initializers(readelf: str, image: Path) -> list[int]:
    sections = {}
    for line in _run(readelf, "-SW", str(image)).splitlines():
        m = re.search(r"\]\s+(\.\S+)\s+\S+\s+([0-9a-f]+)\s+[0-9a-f]+\s+([0-9a-f]+)", line)
        if m:
            sections[m.group(1)] = (int(m.group(2), 16), int(m.group(3), 16))
    targets = []
    relocs = _run(readelf, "-rW", str(image))
    for name in (".init_array", ".ctors", ".preinit_array"):
        if name not in sections:
            continue
        start, size = sections[name]
        print(f"  {name}: {size // 8} entries")
        for line in relocs.splitlines():
            m = re.match(r"\s*([0-9a-f]+)\s+[0-9a-f]+\s+R_X86_64_(RELATIVE|64)\s+(.*)$", line)
            if m and start <= int(m.group(1), 16) < start + size:
                targets.append(int(m.group(3).split()[-1], 16))
    m = re.search(r"\(INIT\)\s+0x([0-9a-f]+)", _run(readelf, "-dW", str(image)))
    if m:
        print("  DT_INIT present")
        targets.append(int(m.group(1), 16))
    return targets


def _macho_initializers(otool: str, image: Path) -> list[int]:
    out = _run(otool, "-l", str(image))
    found = [s for s in ("__mod_init_func", "__init_offsets") if f"sectname {s}" in out]
    for s in ("__mod_init_func", "__init_offsets"):
        print(f"  {s}: {'present' if s in found else 'absent'}")
    if found:
        raise SystemExit(
            f"{image}: Mach-O initializers present; extend this script to resolve them"
        )
    return []


def _report(label: str, count: int, bad: list[str]) -> int:
    print(f"{label} instructions={count} above-baseline={len(bad)}")
    for line in bad:
        print("    BAD", line)
    if count == 0:
        print("    EMPTY: nothing disassembled, so nothing was checked")
    return len(bad) + (count == 0)


def _scan_initializers(pending: list[int], table: dict[int, str], scanner: Scanner) -> int:
    failures = 0
    seen: set[int] = set()
    while pending:
        address = pending.pop()
        if address in seen:
            continue
        seen.add(address)
        name = table.get(address, hex(address))
        count, bad, calls = scanner(name)
        failures += _report(f"  [{name}]", count, bad)
        for call in calls:
            m = CALL_TARGET.search(call)
            if m and "@plt" not in call and int(m.group(1), 16) in table:
                pending.append(int(m.group(1), 16))
    return failures


def _local_targets(calls: list[str], table: dict[int, str]) -> list[int]:
    """Addresses of the defined functions ``calls`` reach, short of the module body."""
    targets = []
    for call in calls:
        m = CALL_TARGET.search(call)
        if m and "@plt" not in call:
            address = int(m.group(1), 16)
            if address in table and not MODULE_BODY.search(table[address]):
                targets.append(address)
    return targets


def _scan_entries(table: dict[int, str], scanner: Scanner) -> int:
    entries = {address: name for address, name in table.items() if ENTRY.fullmatch(name)}
    if not any(".cold" not in name for name in entries.values()):
        print("init entry: no PyInit_* symbol, so nothing was checked")
        return 1
    failures = 0
    pending: list[int] = []
    for entry in entries.values():
        count, bad, calls = scanner(entry)
        failures += _report(f"init entry [{entry}]", count, bad)
        for call in calls:
            print("    CALL", call)
        pending.extend(_local_targets(calls, table))
    seen = set(entries)
    while pending:
        address = pending.pop()
        if address in seen:
            continue
        seen.add(address)
        count, bad, calls = scanner(table[address])
        failures += _report(f"  callee [{table[address]}]", count, bad)
        pending.extend(_local_targets(calls, table))
    return failures


def check(image: Path, fmt: str, tools: dict[str, str]) -> int:
    """Print one image's listing; returns its number of findings."""
    print(f"== {image}")
    llvm = "LLVM" in _run(tools["objdump"], "--version")
    table = _symbols(tools["nm"], image)

    def scanner(symbol: str) -> tuple[int, list[str], list[str]]:
        return scan(_disassemble(tools["objdump"], llvm, image, symbol))

    print("static initializers:")
    if fmt == "elf":
        pending = _elf_initializers(tools["readelf"], image)
    else:
        pending = _macho_initializers(tools["otool"], image)
    return _scan_initializers(pending, table, scanner) + _scan_entries(table, scanner)


def main(argv: list[str] | None = None) -> int:
    images = [Path(arg) for arg in (sys.argv[1:] if argv is None else argv)]
    if not images:
        print("usage: check_guard_isa.py IMAGE [IMAGE...]", file=sys.stderr)
        return 2
    failures = 0
    for image in images:
        fmt = image_format(image)
        tools, missing = find_tools(fmt)
        if missing:
            print(f"{image}: no {', '.join(missing)} on PATH", file=sys.stderr)
            return 2
        failures += check(image, fmt, tools)
    print(f"RESULT: {'FAIL' if failures else 'PASS'} ({failures} findings)")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
