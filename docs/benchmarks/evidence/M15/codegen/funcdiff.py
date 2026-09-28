"""Symbolized instruction diff of one function between two object files.

Usage: funcdiff.py <before.o> <after.o> <mangled-symbol-substring> [--full]

Disassembles the named function (the first __text symbol whose mangled name
contains the substring) with `llvm-objdump -d -r --demangle`, attaching each
relocation to the instruction it patches, so a call reads `bl <_PyErr_Format>`
and a data load names its symbol instead of an address. In-function branch
targets are already printed as `<function+offset>`. Addresses are dropped. What
is left is the instruction stream; a unified diff of the two is printed, with
the instruction counts and the number of changed lines.
"""

import difflib
import re
import subprocess
import sys

OBJDUMP = subprocess.run(
    ["xcrun", "--find", "llvm-objdump"], check=True, capture_output=True, text=True
).stdout.strip()


def find_symbol(path, needle):
    out = subprocess.run(["nm", "-m", path], check=True, capture_output=True, text=True).stdout
    for line in out.splitlines():
        name = line.split()[-1]
        if needle in name and "(__TEXT,__text)" in line:
            return name
    raise SystemExit(f"{needle} not found in {path}")


def stream(path, needle):
    symbol = find_symbol(path, needle)
    out = subprocess.run(
        [OBJDUMP, "-d", "-r", "--no-show-raw-insn", f"--disassemble-symbols={symbol}", path],
        check=True,
        capture_output=True,
        text=True,
    ).stdout
    insns = []
    for line in out.splitlines():
        reloc = re.match(r"\s*[0-9a-f]+:\s+(\w*RELOC\w*|X86_64_\w+|ARM64_\w+)\s+(\S+)", line)
        if reloc and insns:
            insns[-1] += f"  @{reloc.group(2)}"
            continue
        m = re.match(r"\s*[0-9a-f]+:\s+(.*)", line)
        if not m:
            continue
        insn = m.group(1).strip()
        insn = re.sub(r"0x[0-9a-f]+ <([^>]*)>", r"<\1>", insn)  # target address -> symbol+offset
        insn = re.sub(r"\s*;.*$", "", insn)  # arm64 trailing comments (=value)
        insn = re.sub(r"\s+", " ", insn)
        insns.append(insn)
    return symbol, insns


def main():
    before_path, after_path, needle = sys.argv[1:4]
    context = 10**6 if "--full" in sys.argv else 3
    sb, b = stream(before_path, needle)
    sa, a = stream(after_path, needle)
    diff = list(difflib.unified_diff(b, a, "before", "after", lineterm="", n=context))
    removed = sum(1 for d in diff if d.startswith("-") and not d.startswith("---"))
    added = sum(1 for d in diff if d.startswith("+") and not d.startswith("+++"))
    print(f"# symbol: {sb}")
    print(f"# instructions: before {len(b)}, after {len(a)}; diff -{removed} +{added}")
    for line in diff:
        print(line)


if __name__ == "__main__":
    main()
