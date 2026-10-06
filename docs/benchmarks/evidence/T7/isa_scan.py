"""Scan objdump -d output (stdin) for instructions beyond the x86-64 baseline.

Usage: objdump -d -M intel --disassemble=SYM FILE | python3 isa_scan.py LABEL
Flags VEX/EVEX by encoding (C4/C5/62 opcode byte after legacy+REX prefixes;
in 64-bit mode those bytes are always VEX/EVEX) and the non-VEX v2/v3
instructions by mnemonic. Prints calls so their targets can be checked.
"""

import re
import sys

LEGACY = {0x66, 0x67, 0xF2, 0xF3, 0x2E, 0x3E, 0x26, 0x64, 0x65, 0x36, 0xF0}
NON_VEX_ABOVE_BASELINE = re.compile(
    r"^(popcnt|lzcnt|tzcnt|movbe|crc32|cmpxchg16b|"
    r"lddqu|haddp[sd]|hsubp[sd]|addsubp[sd]|movddup|movs[hl]dup|"  # SSE3
    r"pshufb|palignr|pmaddubsw|ph(add|sub)(s?w|d)|pabs[bwd]|psign[bwd]|pmulhrsw|"  # SSSE3
    r"pblendvb|blendv?p[sd]|pblendw|pmin[su][bdw]|pmax[su][bdw]|ptest|pmov[sz]x\w+|"
    r"pinsr[bdq]|pextr[bdq]|round[sp][sd]|dpp[sd]|pmulld|pmuldq|pcmpeqq|packusdw|"
    r"insertps|extractps|mpsadbw|phminposuw|movntdqa|"  # SSE4.1
    r"pcmpgtq|pcmp[ei]str[im])$"  # SSE4.2
)
LINE = re.compile(r"^\s*([0-9a-f]+):\s+((?:[0-9a-f]{2} )+)\s*(.*)$")


def opcode_kind(raw: list[int]) -> str:
    i = 0
    while i < len(raw) and raw[i] in LEGACY:
        i += 1
    if i < len(raw) and 0x40 <= raw[i] <= 0x4F:
        i += 1
    if i < len(raw) and raw[i] in (0xC4, 0xC5):
        return "VEX"
    if i < len(raw) and raw[i] == 0x62:
        return "EVEX"
    return ""


def main() -> int:
    label = sys.argv[1] if len(sys.argv) > 1 else "input"
    count = 0
    bad = []
    calls = []
    for line in sys.stdin:
        m = LINE.match(line)
        if not m:
            continue
        raw = [int(b, 16) for b in m.group(2).split()]
        text = m.group(3).strip()
        if not text:  # continuation line of a long instruction
            continue
        count += 1
        mnem = text.split()[0]
        kind = opcode_kind(raw)
        if kind or NON_VEX_ABOVE_BASELINE.match(mnem):
            bad.append(f"{m.group(1)}: {kind or 'non-VEX v2/v3'} {text}")
        if mnem in ("call", "jmp") and ("<" in text):
            calls.append(f"{m.group(1)}: {text}")
    print(f"[{label}] instructions={count} above-baseline={len(bad)}")
    for b in bad:
        print("  BAD", b)
    for c in calls:
        print("  CALL/JMP", c)
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
