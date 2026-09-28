"""Check 2 (final): serializer state layout and dumps_to_python's frame, main vs candidate.

Usage: check2.py <main arm dir> <candidate arm dir>   (each: <isa>/record-layouts..., <isa>/obj/)

Per ISA: sizeof(Serializer) and every field offset (from -fdump-record-layouts);
the record that holds `stage_` (name, stage_ offset, sizeof); dumps_to_python's
frame from its prologue (arm64: pre-indexed stp + `sub sp`; x86-64: pushes +
`subq $N, %rsp`) and funcdiff's instruction counts for the whole function.
"""

import re
import subprocess
import sys

sys.path.insert(
    0, "/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/codegen"
)
from funcdiff import stream  # noqa: E402

D2P = "__ZN6strata8bindings15dumps_to_pythonEP7_objectb"


def records(path):
    """[(name, [lines], sizeof)] of every top-level record dump."""
    out, cur = [], None
    for line in open(path):
        m = re.match(r"\s+0 \| (?:class|struct) (.*)$", line)
        if m and cur is None:
            cur = [m.group(1).strip(), [line.rstrip()]]
            continue
        if cur is not None:
            cur[1].append(line.rstrip())
            s = re.search(r"\[sizeof=(\d+)", line)
            if s:
                out.append((cur[0], cur[1], int(s.group(1))))
                cur = None
    return out


def fields(lines):
    """Direct members (two-space indent after '|')."""
    res = []
    for ln in lines[1:]:
        m = re.match(r"\s+(\d+) \|   (\S.*?) (\w+)$", ln)
        if m:
            res.append((int(m.group(1)), m.group(3)))
    return res


def frame(insns, isa):
    if isa == "arm64":
        pre = sub = 0
        for i in insns[:24]:
            m = re.match(r"stp \S+, \S+, \[sp, #-(0x[0-9a-f]+|\d+)\]!", i)
            if m:
                pre += int(m.group(1), 0)
            m = re.match(r"sub sp, sp, #(0x[0-9a-f]+|\d+)(, lsl #12)?$", i)
            if m:
                sub += int(m.group(1), 0) << (12 if m.group(2) else 0)
        return f"stp {pre} + sub sp {sub} = {pre + sub} bytes"
    pushes = sub = 0
    chk = None
    for i in insns[:24]:
        if i.startswith("pushq") and chk is None:
            pushes += 1
        m = re.match(r"movl \$(0x[0-9a-f]+), %eax", i)
        if m:
            chk = int(m.group(1), 0)
        m = re.match(r"subq \$(0x[0-9a-f]+|\d+), %rsp", i)
        if m:
            sub += int(m.group(1), 0)
    if chk is not None:  # pushq %rax / chkstk / subq %rax,%rsp / popq %rax
        pushes -= 1
        sub += chk
    return f"{pushes} pushq ({pushes * 8}) + {sub} = {pushes * 8 + sub} bytes (+8 return address)"


def main():
    base, cand = sys.argv[1], sys.argv[2]
    for isa in ("arm64", "x86_64"):
        print(f"== {isa}")
        summary = {}
        for label, root in (("main", base), ("cand", cand)):
            recs = records(f"{root}/{isa}/record-layouts.python_dumps.txt")
            ser = [r for r in recs if r[0].endswith("::Serializer")]
            stage = [
                r
                for r in recs
                if any(
                    ln.endswith(" stage_") and "|   " in ln and re.match(r"\s+\d+ \|   \S", ln)
                    for ln in r[1]
                )
            ]
            s = ser[-1]
            st = stage[-1] if stage else None
            st_off = next((o for o, n in fields(st[1]) if n == "stage_"), None) if st else None
            _, ins = stream(f"{root}/{isa}/obj/python_dumps.o", D2P)
            summary[label] = (
                s[2],
                fields(s[1]),
                st[0] if st else None,
                st_off,
                st[2] if st else None,
                frame(ins, isa),
                len(ins),
                ins,
            )
            print(
                f"  {label}: sizeof(Serializer)={s[2]}; stage_ in {st[0] if st else '?'} at "
                f"offset {st_off}, sizeof {st[2] if st else '?'}; dumps_to_python "
                f"{len(ins)} insns, frame {frame(ins, isa)}"
            )
        m, c = summary["main"], summary["cand"]
        print(f"  Serializer fields identical: {m[1] == c[1]}")
        if m[1] != c[1]:
            print(f"    main {m[1]}\n    cand {c[1]}")
        print(f"  dumps_to_python stream identical: {m[7] == c[7]}")


if __name__ == "__main__":
    main()
