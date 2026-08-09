#!/usr/bin/env bash
# Two-phase PGO + LTO build. Fronted by `make pgo`.
#
#   phase 1  instrumented build (no LTO) -> training data -> training workload
#            -> gate tests -> llvm-profdata merge
#   phase 2  rebuild with the merged profile and LTO -> gate tests
#            -> verification benchmarks
#
# Both phases run the full gate: an optimized build that fails its tests is
# worth nothing, and PGO is exactly the kind of change that can miscompile.
# The profile is regenerated from scratch every run — a stale profile silently
# pessimizes the branches it no longer describes.
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

VENV="${VENV:-.venv}"
VPY="$VENV/bin/python"
PGO_DIR="${PGO_DIR:-build/pgo}"
RAW_DIR="$PGO_DIR/raw"
WORK_DIR="$PGO_DIR/work"
PROFILE="$ROOT_DIR/$PGO_DIR/strata.profdata"
BENCH_REPEAT="${PGO_BENCH_REPEAT:-10}"
BENCH_WARMUP="${PGO_BENCH_WARMUP:-2}"
BENCH_DATA="benchmarks/data/generated/small"

if [[ ! -x "$VPY" ]]; then
    echo "Error: $VPY not found. Run: make dev" >&2
    exit 1
fi

compiler_kind() {
    local cxx version
    cxx="${CXX:-c++}"
    version="$("$cxx" --version 2>/dev/null || true)"
    case "$version" in
    *clang*) echo clang ;;
    *gcc* | *g++*) echo gcc ;;
    *) echo unknown ;;
    esac
}

profdata_tool() {
    if command -v llvm-profdata >/dev/null 2>&1; then
        echo "llvm-profdata"
    elif command -v xcrun >/dev/null 2>&1 && xcrun --find llvm-profdata >/dev/null 2>&1; then
        echo "xcrun llvm-profdata"
    else
        echo "Error: llvm-profdata not found. Install LLVM (brew install llvm) or the" >&2
        echo "Xcode command line tools (xcode-select --install)." >&2
        exit 1
    fi
}

gate_tests() {
    "$VPY" scripts/cpp_tests.py
    "$VPY" scripts/py_tests.py
}

KIND="$(compiler_kind)"
if [[ "$KIND" == "unknown" ]]; then
    echo "Error: cannot identify the C++ compiler (${CXX:-c++}); PGO needs clang or gcc." >&2
    exit 1
fi
echo "==> PGO: compiler is $KIND"

rm -rf "$PGO_DIR"
mkdir -p "$RAW_DIR" "$WORK_DIR"

# --- phase 1: instrument ---------------------------------------------------
echo "==> PGO phase 1: instrumented build"
export PGO_MODE=generate
export STRATA_ENABLE_LTO=0
if [[ "$KIND" == "clang" ]]; then
    # %m keeps concurrent processes from clobbering one profile file.
    export LLVM_PROFILE_FILE="$ROOT_DIR/$RAW_DIR/%p-%m.profraw"
else
    export GCOV_PREFIX="$ROOT_DIR/$RAW_DIR"
    export GCOV_PREFIX_STRIP=0
fi
# --no-deps: the dependency resolver would otherwise reinstall unrelated
# packages between the two phases and muddy the comparison.
"$VPY" -m pip install --force-reinstall --no-deps -e .

echo "==> PGO: generating training data"
"$VPY" scripts/pgo_training_data.py --out-dir "$PGO_DIR"

echo "==> PGO: running the training workload"
PYTHONPATH=. "$VPY" scripts/pgo_training.py \
    --json "$PGO_DIR/train.json" \
    --ndjson "$PGO_DIR/train.ndjson" \
    --work-dir "$WORK_DIR"

echo "==> PGO: gate tests on the instrumented build"
gate_tests

if [[ "$KIND" == "clang" ]]; then
    shopt -s nullglob
    raw_files=("$RAW_DIR"/*.profraw)
    shopt -u nullglob
    if [[ "${#raw_files[@]}" -eq 0 ]]; then
        echo "Error: no .profraw files were written — the build was not instrumented." >&2
        exit 1
    fi
    # Resolved into a variable first: `$(profdata_tool) merge ...` would run the
    # helper in a subshell, where its `exit 1` cannot stop this script.
    merge_tool="$(profdata_tool)"
    echo "==> PGO: merging ${#raw_files[@]} raw profiles"
    $merge_tool merge -output="$PROFILE" "${raw_files[@]}"
else
    # gcc writes .gcda directly; -fprofile-use reads the tree.
    PROFILE="$ROOT_DIR/$RAW_DIR"
fi

# --- phase 2: optimize -----------------------------------------------------
echo "==> PGO phase 2: optimized build (profile + LTO)"
unset LLVM_PROFILE_FILE GCOV_PREFIX GCOV_PREFIX_STRIP
export PGO_MODE=use
export STRATA_ENABLE_LTO=1
export STRATA_PGO_PROFILE="$PROFILE"
"$VPY" -m pip install --force-reinstall --no-deps -e .

echo "==> PGO: gate tests on the optimized build"
gate_tests

# --- verification ----------------------------------------------------------
if [[ ! -d "$BENCH_DATA" ]]; then
    echo "==> PGO: generating benchmark data"
    make bench-data
fi

echo "==> PGO: verification benchmarks"
PYTHONPATH=. "$VPY" -m benchmarks.bench_main \
    --name pgo --repeat "$BENCH_REPEAT" --warmup "$BENCH_WARMUP" \
    --dataset "$BENCH_DATA/users.json" \
    --dataset "$BENCH_DATA/flat.json" \
    --dataset "$BENCH_DATA/nested.json" \
    --output "$PGO_DIR/bench_results_pgo.md"

echo "==> PGO complete"
echo "    profile: $PROFILE"
echo "    results: $PGO_DIR/bench_results_pgo.md"
echo "    The installed extension is now the PGO+LTO build."
