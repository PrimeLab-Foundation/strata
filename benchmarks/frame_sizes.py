"""What every serializer function spends on entering and leaving itself.

The `dumps mixed` annotation on the Neoverse-N2 found no hot loop -- 2,015
sampled lines in `write_scalar_run` and not one of them reaching 8% -- but it
did find that `Serializer::write`'s prologue and epilogue carried 11.8% of
that function's own samples. `write` runs once per value and once per
container, 1,375 times per mixed.json document, so what it saves and restores
on entry is a real share of the row, and the same instructions are re-fetched
when the harness hands the call a cold cache.

Code size (benchmarks/symbol_sizes.py) does not show that: a function can be
small and still open a wide frame, and a function that inlines a rare tail
carries the tail's registers on every call. This reads the prologue itself.

usage: frame_sizes.py <shared object> [<symbol substring> ...]

Defaults to the serializer's own symbols. `objdump -dC` is the disassembler.
Both mainstream architectures are read: on AArch64 the frame is `sub sp` or a
pre-indexed `stp`, and the saved pairs are `stp` of x19-x28, x29/x30 or
d8-d15; on x86-64 it is `sub $N,%rsp` plus one `push` per saved register.
An architecture neither pattern matches reports the instruction count only,
rather than a zero that would read as a measurement.
"""

import re
import subprocess
import sys

DEFAULT_FILTERS = ("Serializer::", "dumps_to_python", "StagedOutput::")

FUNCTION = re.compile(r"^[0-9a-f]+ <(.+)>:$")
INSTRUCTION = re.compile(r"^\s+[0-9a-f]+:\s+(.*)$")

ARM_SUB_SP = re.compile(r"^sub\s+sp,\s*sp,\s*#(0x[0-9a-f]+|\d+)")
ARM_STP_PRE = re.compile(r"^stp\s+\S+,\s*\S+,\s*\[sp,\s*#-(0x[0-9a-f]+|\d+)\]!")
ARM_STP = re.compile(r"^stp\s+(\S+),")
ARM_SAVED = re.compile(r"^(x19|x2[0-8]|x29|d8|d9|d1[0-5])$")

X86_SUB_SP = re.compile(r"^sub\s+\$(0x[0-9a-f]+|\d+),%rsp")
X86_PUSH = re.compile(r"^push\s+%(rbx|rbp|r1[2-5])$")


def number(text: str) -> int:
    return int(text, 16) if text.startswith("0x") else int(text)


def disassemble(path: str) -> dict[str, list[str]]:
    proc = subprocess.run(
        ["objdump", "-dC", "--no-show-raw-insn", path],
        capture_output=True,
        text=True,
        check=False,
    )
    if proc.returncode != 0:
        print(f"objdump failed on {path}: {proc.stderr.strip()[:200]}", file=sys.stderr)
        return {}
    functions: dict[str, list[str]] = {}
    current: list[str] | None = None
    for line in proc.stdout.splitlines():
        header = FUNCTION.match(line)
        if header:
            current = functions.setdefault(header.group(1), [])
            continue
        body = INSTRUCTION.match(line)
        if body is not None and current is not None:
            current.append(re.sub(r"\s+", " ", body.group(1).split("//")[0].strip()))
    return functions


# The prologue is not always the first thing a function does: a compiler that
# shrink-wraps puts an early exit ahead of it (`run_strings` opens with a
# `cmp`/`b.lt` before saving anything). So the scan runs to the first call or
# return rather than stopping at the first instruction it does not recognise.
STOP = ("bl ", "blr ", "ret", "b ")


def prologue(body: list[str]) -> tuple[int, int, int]:
    """(frame bytes, saved register pairs, prologue instructions)."""
    frame = 0
    pairs = 0
    count = 0
    for text in body[:24]:
        if text.startswith(STOP):
            break
        pre = ARM_STP_PRE.match(text)
        sub = ARM_SUB_SP.match(text)
        push = X86_PUSH.match(text)
        x86 = X86_SUB_SP.match(text)
        stp = ARM_STP.match(text)
        if pre is not None:
            frame = max(frame, number(pre.group(1)))
            pairs += 1
        elif sub is not None:
            frame = max(frame, number(sub.group(1)))
        elif x86 is not None:
            frame = max(frame, number(x86.group(1)))
        elif push is not None:
            pairs += 1
        elif stp is not None and ARM_SAVED.match(stp.group(1)):
            pairs += 1
        else:
            continue
        count += 1
    return frame, pairs, count


def main() -> int:
    if len(sys.argv) < 2:
        print(__doc__)
        return 2
    path = sys.argv[1]
    filters = tuple(sys.argv[2:]) or DEFAULT_FILTERS
    functions = disassemble(path)
    if not functions:
        print("no disassembly — the object is stripped, or objdump is missing")
        return 0
    rows = []
    for name, body in functions.items():
        if not any(key in name for key in filters) or ".cold" in name:
            continue
        frame, pairs, count = prologue(body)
        rows.append((frame, pairs, count, len(body), name))
    rows.sort(reverse=True)
    print(f"== {path}: frame bytes, saved pairs, prologue instructions, total instructions")
    for frame, pairs, count, total, name in rows:
        print(f"   {frame:6d} {pairs:4d} {count:4d} {total:7d}  {name[:120]}")
    print(
        f"   {sum(row[0] for row in rows):6d} {sum(row[1] for row in rows):4d} "
        f"{sum(row[2] for row in rows):4d} {sum(row[3] for row in rows):7d}  TOTAL "
        f"over {len(rows)} symbols"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
