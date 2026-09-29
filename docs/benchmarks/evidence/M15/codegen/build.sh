#!/usr/bin/env bash
# M15 static hot-path checks 1, 2, 7 -- the M12 codegen method
# (build/evidence/benchmark-lead/M12/codegen/build_arm.sh), with the binding
# source list read from the tree's own setup.py (BINDING_SOURCES) instead of a
# hard-coded copy, so each arm builds exactly the `_strata` its setup.py builds.
# Plain release build: setup.py's _compile_args(profiled=False) on POSIX, no PGO,
# no LTO. Usage: build.sh <tree> <out-dir> <arm64|x86_64>
#
# arm64 : setup.py's own flags on this host (-march=native).
# x86_64: a cross build; setup.py adds -fomit-frame-pointer on x86-64, and
#         -march is pinned to x86-64-v3 because -march=native names the host.
set -euo pipefail
TREE="$(cd "$1" && pwd)"
OUT="$2"
ARCH="$3"
PY=/opt/homebrew/bin/python3.14
PYINC="$("$PY" -c 'import sysconfig; print(sysconfig.get_paths()["include"])')"
COMMON=(-fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g
    -O3 -Wall -I"$TREE/include" -I"$PYINC" -std=c++20 -O3 -D_LIBCPP_DISABLE_AVAILABILITY)
case "$ARCH" in
    arm64) ISA=(-arch arm64 -march=native) ;;
    x86_64) ISA=(-arch x86_64 -fomit-frame-pointer -march=x86-64-v3) ;;
    *) echo "arch?" >&2; exit 2 ;;
esac
SOURCES=()
while IFS= read -r line; do SOURCES+=("$line"); done < <("$PY" - "$TREE/setup.py" <<'EOF'
import ast, sys
tree = ast.parse(open(sys.argv[1]).read())
for node in tree.body:
    if isinstance(node, ast.Assign) and any(
        isinstance(t, ast.Name) and t.id == "BINDING_SOURCES" for t in node.targets
    ):
        for elt in node.value.elts:
            print(elt.value)
EOF
)
while IFS= read -r line; do
    case "$line" in ''|'#'*) continue ;; esac
    SOURCES+=("$line")
done <"$TREE/src/strata/core_sources.txt"
mkdir -p "$OUT/$ARCH/obj"
printf '%s\n' "${SOURCES[@]}" >"$OUT/$ARCH/sources.txt"
OBJS=()
for src in "${SOURCES[@]}"; do
    obj="$OUT/$ARCH/obj/$(basename "${src%.cpp}").o"
    (cd "$TREE" && clang++ "${COMMON[@]}" "${ISA[@]}" -c "$src" -o "$obj") &
    OBJS+=("$obj")
done
wait
clang++ "${ISA[@]}" -bundle -undefined dynamic_lookup "${OBJS[@]}" -o "$OUT/$ARCH/_strata.so"
size -m "$OUT/$ARCH/_strata.so" >"$OUT/$ARCH/size-m.txt"
size -m "$OUT/$ARCH/obj/python_dumps.o" >"$OUT/$ARCH/size-m-python_dumps.o.txt"
# Record layout of the serializer's state in this TU (check 2).
(cd "$TREE" && clang++ "${COMMON[@]}" "${ISA[@]}" -fsyntax-only -Xclang -fdump-record-layouts \
    src/strata/bindings/python_dumps.cpp) >"$OUT/$ARCH/record-layouts.python_dumps.txt" 2>/dev/null
echo "built $OUT/$ARCH/_strata.so (${#SOURCES[@]} sources)"
