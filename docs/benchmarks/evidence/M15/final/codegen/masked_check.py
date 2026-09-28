"""Check 1 (final): is main's Serializer::write contained, in order, in the candidate's?

Usage: masked_check.py <main.o> <candidate.o> [symbol-substring]

Streams come from funcdiff.stream (symbolized, addresses dropped, relocations
attached). Then masked, so that only layout -- not codegen -- is removed:
  - in-function branch/call targets <SYM+0x..> -> <SELF>  (tail growth moves them);
  - x86 RIP-relative displacement and its '## <...>' comment -> masked (the
    @relocation that names the target is kept);
  - trailing alignment nops (nop/nopw/nopl/data16) dropped.
Reports: masked instruction counts, whether every main instruction appears in
order (longest common subsequence == len(main)), and each non-equal opcode of
difflib's alignment (tag, main range, candidate range) with its instructions.
"""

import difflib
import pathlib
import re
import sys

sys.path.insert(
    0, "/Users/borysbardysh/PycharmProjects/PrimeLabFoundation/strata/build/evidence/M15/codegen"
)
from funcdiff import stream  # noqa: E402

SYM = "__ZN6strata8bindings12_GLOBAL__N_110Serializer5writeEP7_object"


def mask(insns, symbol):
    out = []
    for i in insns:
        i = i.replace(f"<{symbol}+", "<SELF+")
        i = re.sub(r"<SELF\+0x[0-9a-f]+>", "<SELF>", i)
        i = re.sub(r"<SELF>", "<SELF>", i)
        i = re.sub(r"-?0x[0-9a-f]+\(%rip\)", "D(%rip)", i)
        i = re.sub(r"\s*## <[^>]*>", "", i)
        out.append(i)
    while out and re.match(r"(nop|data16|xchg %ax, %ax)", out[-1]):
        out.pop()
    return out


def main():
    before, after = sys.argv[1], sys.argv[2]
    needle = sys.argv[3] if len(sys.argv) > 3 else SYM
    sb, b = stream(before, needle)
    sa, a = stream(after, needle)
    mb, ma = mask(b, sb), mask(a, sa)
    sm = difflib.SequenceMatcher(a=mb, b=ma, autojunk=False)
    lcs = sum(bl.size for bl in sm.get_matching_blocks())
    print(f"# {sb}")
    print(f"# raw instructions: main {len(b)}, candidate {len(a)}")
    print(f"# masked (nop padding dropped): main {len(mb)}, candidate {len(ma)}")
    print(
        f"# main instructions matched in order: {lcs}/{len(mb)}"
        f" -> {'ALL' if lcs == len(mb) else 'NOT ALL'}"
    )
    ops = [op for op in sm.get_opcodes() if op[0] != "equal"]
    kinds = sorted({op[0] for op in ops})
    print(f"# non-equal opcodes: {len(ops)} ({', '.join(kinds) or 'none'})")
    for tag, i1, i2, j1, j2 in ops:
        print(
            f"{tag} main[{i1}:{i2}] cand[{j1}:{j2}]"
            f"  (main tail position {i1}/{len(mb)}, -{i2 - i1} +{j2 - j1})"
        )
        for x in mb[i1:i2]:
            print(f"  - {x}")
        for x in ma[j1:j2]:
            print(f"  + {x}")


if __name__ == "__main__":
    main()
