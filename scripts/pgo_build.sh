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
#
# The gate runs under instrumentation too, and its counters are not production
# behaviour, so they are written to a throwaway directory: the merged profile
# is the training workload alone (see "the profile's counters" below).
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

VENV="${VENV:-.venv}"
VPY="$VENV/bin/python"
PGO_DIR="${PGO_DIR:-build/pgo}"
RAW_DIR="$PGO_DIR/raw"
# Counters written by anything other than the training workload land here and
# are never merged. Kept rather than discarded so a run can be audited.
JUNK_DIR="$PGO_DIR/junk"
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
mkdir -p "$RAW_DIR" "$JUNK_DIR" "$WORK_DIR"

# --- phase 1: instrument ---------------------------------------------------
echo "==> PGO phase 1: instrumented build"
export PGO_MODE=generate
export STRATA_ENABLE_LTO=0
# The profile's counters: every instrumented process — the two gate runs as
# much as the training workload — writes counts wherever these variables
# point when it exits. A test suite is not production behaviour: it calls the
# entry points once each over tiny inputs, so its counts are broad, shallow
# and dominated by call frequency (measured on this recipe: 47.5% of the
# merged counts, 64.5% of write_sequence's and 99.7% of dumps_to_python's,
# against 8.1% of write_scalar_run's inner loop). Left in the merge they
# steer the optimizer with the test files. So the default points at the junk
# directory and only the training workload below is given the real one — the
# merged profile is the documented training workload alone, and the gates
# still run.
if [[ "$KIND" == "clang" ]]; then
    # %m keeps concurrent processes from clobbering one profile file.
    export LLVM_PROFILE_FILE="$ROOT_DIR/$JUNK_DIR/%p-%m.profraw"
    training_profile_env=("LLVM_PROFILE_FILE=$ROOT_DIR/$RAW_DIR/%p-%m.profraw")
else
    export GCOV_PREFIX="$ROOT_DIR/$JUNK_DIR"
    export GCOV_PREFIX_STRIP=0
    training_profile_env=("GCOV_PREFIX=$ROOT_DIR/$RAW_DIR")
fi
# --no-deps: the dependency resolver would otherwise reinstall unrelated
# packages between the two phases and muddy the comparison.
"$VPY" -m pip install --force-reinstall --no-deps -e .

echo "==> PGO: generating training data"
"$VPY" scripts/pgo_training_data.py --out-dir "$PGO_DIR"

echo "==> PGO: running the training workload"
# The one process whose counters are kept.
PYTHONPATH=. env "${training_profile_env[@]}" "$VPY" scripts/pgo_training.py \
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
        echo "Error: the training workload wrote no .profraw into $RAW_DIR — either the" >&2
        echo "       build was not instrumented or LLVM_PROFILE_FILE did not reach it." >&2
        exit 1
    fi
    # Exactly one: scripts/pgo_training.py is a single process and is the only
    # thing pointed at $RAW_DIR. A second file means another instrumented run
    # reached it — the contamination this separation exists to prevent — so it
    # fails the build instead of silently steering the optimizer.
    if [[ "${#raw_files[@]}" -ne 1 ]]; then
        echo "Error: expected one raw profile in $RAW_DIR (the training workload)," >&2
        echo "       found ${#raw_files[@]}:" >&2
        printf '         %s\n' "${raw_files[@]}" >&2
        echo "       Something else ran with LLVM_PROFILE_FILE pointing there." >&2
        exit 1
    fi
    # Resolved into a variable first: `$(profdata_tool) merge ...` would run the
    # helper in a subshell, where its `exit 1` cannot stop this script.
    merge_tool="$(profdata_tool)"
    echo "==> PGO: merging ${#raw_files[@]} raw profile (the training workload)"
    $merge_tool merge -output="$PROFILE" "${raw_files[@]}"
else
    # gcc writes .gcda directly; -fprofile-use reads the tree. The gate runs
    # wrote theirs under $JUNK_DIR, so this tree is the training workload's.
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
