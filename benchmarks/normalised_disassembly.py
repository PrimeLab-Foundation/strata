"""Function-by-function comparison of two builds' code, with layout factored out.

Usage: python benchmarks/normalised_disassembly.py <A-image> <B-image>

A diagnostic, never a pass condition (the byte gate is benchmarks/image_identity.py).
Disassembles both images with llvm-objdump and compares each function's
instruction stream after removing what moves with *placement* rather than with
code:

1. ThinLTO's promoted-name suffixes (`.llvm.<N>`, a module hash that follows the
   build directory and the profile);
2. addresses: branch and call targets are printed as `<symbol+offset>`;
3. then, for the functions still differing, page-offset immediates (`#0x...` on
   arm64 `ldr`/`add` after `adrp`, which name GOT and data slots) -- reported
   separately, since a slot's offset follows the order the linker assigned.

Prints how many functions are identical at each level and the first differing
lines of any function that differs beyond (3). Exit status is always 0.
"""

from __future__ import annotations

import difflib
import os
import re
import shutil
import subprocess
import sys


def objdump() -> str:
    candidates = [os.environ.get("LLVM_OBJDUMP"), shutil.which("llvm-objdump")]
    if sys.platform == "darwin":
        found = subprocess.run(["xcrun", "--find", "llvm-objdump"], capture_output=True, text=True)
        candidates.append(found.stdout.strip())
    candidates.append(r"C:\Program Files\LLVM\bin\llvm-objdump.exe")
    for candidate in candidates:
        if candidate and os.path.exists(candidate):
            return candidate
    raise SystemExit("llvm-objdump not found")


def functions(path: str) -> dict[str, list[str]]:
    out = subprocess.run(
        [objdump(), "-d", "--no-show-raw-insn", path], check=True, capture_output=True, text=True
    ).stdout
    result: dict[str, list[str]] = {}
    current = None
    for line in out.splitlines():
        head = re.match(r"^[0-9a-f]+ <(.*)>:$", line)
        if head:
            current = re.sub(r"\.llvm\.\d+", "", head.group(1))
            result[current] = []
            continue
        body = re.match(r"^\s*[0-9a-f]+:\s+(.*)$", line)
        if current is None or not body:
            continue
        insn = re.sub(r"\.llvm\.\d+", "", body.group(1))
        insn = re.sub(r"0x[0-9a-f]+ <([^>]*)>", r"<\1>", insn)
        insn = re.sub(r"\s*(//|#\s|;).*$", "", insn)
        result[current].append(re.sub(r"\s+", " ", insn).strip())
    return result


def slots_normalised(stream: list[str]) -> list[str]:
    return [
        re.sub(r"#0x[0-9a-f]+", "#<slot>", insn) if insn.startswith(("ldr", "add")) else insn
        for insn in stream
    ]


def main() -> int:
    a, b = functions(sys.argv[1]), functions(sys.argv[2])
    common = sorted(set(a) & set(b))
    only = sorted(set(a) ^ set(b))
    differ = [name for name in common if a[name] != b[name]]
    beyond = [name for name in differ if slots_normalised(a[name]) != slots_normalised(b[name])]
    print(f"functions: A {len(a)}, B {len(b)}, only in one {len(only)}")
    print(
        f"identical after name and address normalisation: {len(common) - len(differ)}/{len(common)}"
    )
    print(f"differing only in page-offset slot immediates: {len(differ) - len(beyond)}")
    print(f"differing beyond that: {len(beyond)}")
    for name in only[:10]:
        print(f"  only in one: {name}")
    for name in beyond[:10]:
        lines = [
            line
            for line in difflib.unified_diff(a[name], b[name], lineterm="", n=0)
            if line[:1] in "+-" and not line.startswith(("+++", "---"))
        ]
        print(f"  {name}: {len(lines)} changed lines, e.g. {lines[:4]}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
