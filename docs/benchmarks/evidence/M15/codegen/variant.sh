#!/usr/bin/env bash
# Screen one source variant of python_dumps.cpp (the cg-b worktree as it stands)
# against main's objects: compile the TU for both ISAs with build.sh's flags,
# save the tree's diff against 2f494a9, the write() instruction diff (funcdiff.py)
# and the per-function summary of the object (allfuncdiff.py).
# Usage: variant.sh <name>
set -euo pipefail
E="$(cd "$(dirname "$0")" && pwd)"
TREE="$HOME/worktrees/strata/cg-b"
NAME="$1"
OUT="$E/variants/$NAME"
PY=/opt/homebrew/bin/python3.14
PYINC="$("$PY" -c 'import sysconfig; print(sysconfig.get_paths()["include"])')"
COMMON=(-fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g
    -O3 -Wall -I"$TREE/include" -I"$PYINC" -std=c++20 -O3 -D_LIBCPP_DISABLE_AVAILABILITY)
SYM=__ZN6strata8bindings12_GLOBAL__N_110Serializer5writeEP7_object
mkdir -p "$OUT"
git -C "$TREE" diff 2f494a9 -- src include >"$OUT/variant.patch"
for arch in arm64 x86_64; do
    case "$arch" in
        arm64) ISA=(-arch arm64 -march=native) ;;
        x86_64) ISA=(-arch x86_64 -fomit-frame-pointer -march=x86-64-v3) ;;
    esac
    (cd "$TREE" && clang++ "${COMMON[@]}" "${ISA[@]}" -c src/strata/bindings/python_dumps.cpp \
        -o "$OUT/python_dumps.$arch.o")
    "$PY" "$E/funcdiff.py" "$E/main/$arch/obj/python_dumps.o" "$OUT/python_dumps.$arch.o" "$SYM" \
        >"$OUT/write_diff.$arch.txt"
    "$PY" "$E/funcdiff.py" "$E/main/$arch/obj/python_dumps.o" "$OUT/python_dumps.$arch.o" "$SYM" \
        --full >"$OUT/write_diff_full.$arch.txt"
    "$PY" "$E/allfuncdiff.py" "$E/main/$arch/obj/python_dumps.o" "$OUT/python_dumps.$arch.o" \
        >"$OUT/funcs.python_dumps.$arch.txt"
    echo "$arch: $(sed -n 2p "$OUT/write_diff.$arch.txt")"
    grep -E '^# (functions|outlined)|^DIFFERS' "$OUT/funcs.python_dumps.$arch.txt" \
        | sed -E 's/__ZN6strata8bindings12_GLOBAL__N_110Serializer/Serializer::/; s/^/    /'
done
