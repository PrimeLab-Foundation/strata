#!/bin/bash
# M12 criterion 4: the symbolized codegen pair, both ISAs.
#
# Prints the two tables the ledger's M12 entry quotes:
#   1. `size -m` Section (__TEXT, __text) per changed translation unit;
#   2. per-symbol instruction counts, and the length of the identical leading
#      run of `Serializer::write` -- the type dispatch every canonical row runs.
#
# Base arm = `main`, candidate arm = the working tree. Same compiler, same
# flags, same headers; -march differs per ISA but is identical between the two
# arms, so every difference is the source's. Neither arm is a PGO build: with
# no profile there is nothing to pin, which is the strictest reading of the
# pinned-profdata rule. Never linked into production (docs/context/convention.md,
# Experiments).
#
# Usage:  bash experiments/m12_codegen_pair.sh [outdir]
#         BASE=<rev> to compare against something other than main.
set -euo pipefail

WT=${WT:-$(git rev-parse --show-toplevel)}
BASE=${BASE:-main}
OUT=${1:-${TMPDIR:-/tmp}/m12-codegen}
PYINC=${PYINC:-$(python3 -c 'import sysconfig; print(sysconfig.get_paths()["include"])')}

TUS="python_dumps python_module python_files python_folder"

rm -rf "$OUT"
mkdir -p "$OUT/base" "$OUT/cand" "$OUT/obj"
git -C "$WT" archive "$BASE" | tar -x -C "$OUT/base"
(cd "$WT" && tar -c include src) | tar -x -C "$OUT/cand"

compile() { # arm arch march
    local arm=$1 arch=$2 march=$3 name
    for name in $TUS; do
        (cd "$OUT/$arm" && clang++ -fno-strict-overflow -Wsign-compare -Wunreachable-code \
            -fno-common -dynamic -DNDEBUG -O3 -Wall -Iinclude -I"$PYINC" \
            -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -arch "$arch" "$march" \
            -c "src/strata/bindings/$name.cpp" -o "$OUT/obj/$arm-$arch-$name.o")
    done
}

for pair in "arm64:-march=native" "x86_64:-march=x86-64-v3"; do
    arch=${pair%%:*}
    compile base "$arch" "${pair#*:}"
    compile cand "$arch" "${pair#*:}"
done

echo "=== size -m  Section (__TEXT, __text), bytes ==="
for arch in arm64 x86_64; do
    bt=0
    ct=0
    for name in $TUS; do
        b=$(size -m "$OUT/obj/base-$arch-$name.o" | awk -F': ' '/Section \(__TEXT, __text\)/{print $2}')
        c=$(size -m "$OUT/obj/cand-$arch-$name.o" | awk -F': ' '/Section \(__TEXT, __text\)/{print $2}')
        printf '%-7s %-15s base=%6d cand=%6d delta=%+d\n' "$arch" "$name" "$b" "$c" "$((c - b))"
        bt=$((bt + b))
        ct=$((ct + c))
    done
    printf '%-7s %-15s base=%6d cand=%6d delta=%+d   <== TOTAL (bound: 512 B)\n' \
        "$arch" ALL "$bt" "$ct" "$((ct - bt))"
done

echo
echo "=== symbolized instruction counts (python_dumps.cpp) ==="
OUT="$OUT" python3 - <<'PY'
import collections
import os
import re
import subprocess

obj = os.path.join(os.environ["OUT"], "obj")
WRITE = "strata::bindings::(anonymous namespace)::Serializer::write(_object*)"


def disassemble(path):
    out = subprocess.run(
        ["objdump", "--disassemble", "--demangle", path],
        capture_output=True, text=True, check=True,
    ).stdout
    counts, bodies, cur = collections.Counter(), collections.defaultdict(list), None
    for line in out.splitlines():
        header = re.match(r"^[0-9a-f]+ <(.+)>:$", line)
        if header:
            cur = header.group(1)
            continue
        if cur and re.match(r"^\s*[0-9a-f]+:", line):
            counts[cur] += 1
            text = line.split("\t")[-1].strip()
            bodies[cur].append(re.sub(r"<[^>]*>", "SYM", re.sub(r"0x[0-9a-f]+", "A", text)))
    return counts, bodies


for arch in ("arm64", "x86_64"):
    bc, bb = disassemble(f"{obj}/base-{arch}-python_dumps.o")
    cc, cb = disassemble(f"{obj}/cand-{arch}-python_dumps.o")
    moved = sorted(
        ((cc[k] - bc[k], k, bc[k], cc[k]) for k in set(bc) | set(cc) if cc[k] != bc[k]),
        key=lambda row: -abs(row[0]),
    )
    print(f"--- {arch}: all symbols {sum(bc.values())} -> {sum(cc.values())} insns")
    for delta, name, before, after in moved[:8]:
        print(f"  {delta:+6d}  base={before:6d} cand={after:6d}  {name[:98]}")
    before, after = bb[WRITE], cb[WRITE]
    shared = 0
    while shared < min(len(before), len(after)) and before[shared] == after[shared]:
        shared += 1
    print(f"  write(): {len(before)} -> {len(after)} insns, "
          f"identical leading run {shared} (the exact-type dispatch chain)")
PY
