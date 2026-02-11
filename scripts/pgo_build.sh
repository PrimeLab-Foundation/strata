#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

PYTHON="${PYTHON:-python3.14}"
VENV="${VENV:-.venv}"
PGO_DIR="${PGO_DIR:-build/pgo}"
GEN_DIR="${PGO_DIR}/gen"
PROFILE_DATA="${PGO_DIR}/strata.profdata"
PGO_BENCH_REPEAT="${PGO_BENCH_REPEAT:-50}"
PGO_BENCH_WARMUP="${PGO_BENCH_WARMUP:-3}"

ensure_venv() {
  if [ ! -d "$VENV" ]; then
    "$PYTHON" -m venv "$VENV"
  fi
  "$VENV/bin/python" -m pip install -U pip setuptools wheel
  "$VENV/bin/python" -m pip install -e ".[dev,bench]"
  export VENV
  export PYTHON="$VENV/bin/python"
}

detect_compiler() {
  local cxx_bin="${CXX:-c++}"
  local version_output
  version_output="$("$cxx_bin" --version 2>/dev/null || true)"
  case "$version_output" in
    *clang*) echo "clang" ;;
    *gcc*|*g++*) echo "gcc" ;;
    *) echo "unknown" ;;
  esac
}

merge_profraw() {
  local profdata_bin=""
  if command -v llvm-profdata >/dev/null 2>&1; then
    profdata_bin="llvm-profdata"
  elif command -v llvm-profdata-14 >/dev/null 2>&1; then
    profdata_bin="llvm-profdata-14"
  elif command -v llvm-profdata-15 >/dev/null 2>&1; then
    profdata_bin="llvm-profdata-15"
  elif command -v xcrun >/dev/null 2>&1; then
    xcrun llvm-profdata merge -output="$PROFILE_DATA" "$GEN_DIR"/*.profraw
    return
  fi
  if [ -n "$profdata_bin" ]; then
    "$profdata_bin" merge -output="$PROFILE_DATA" "$GEN_DIR"/*.profraw
    return
  fi
  echo "Error: llvm-profdata not found. Install LLVM or Xcode command line tools."
  exit 1
}

# Dataset configuration for PGO profiling.
# Default: medium dataset provides good balance of representativeness and build time.
# For release builds, consider using 'large' for maximum profile coverage.
# Set PGO_DATASET_SIZE to 'small', 'medium', or 'large' to override.
PGO_DATASET_SIZE="${PGO_DATASET_SIZE:-medium}"

# All dataset paths (used for comprehensive profiling)
BENCH_SMALL_JSON="${ROOT_DIR}/benchmarks/data/generated/small/users.json"
BENCH_SMALL_NDJSON="${ROOT_DIR}/benchmarks/data/generated/small/users.ndjson"
BENCH_MEDIUM_JSON="${ROOT_DIR}/benchmarks/data/generated/medium/users.json"
BENCH_MEDIUM_NDJSON="${ROOT_DIR}/benchmarks/data/generated/medium/users.ndjson"
BENCH_LARGE_JSON="${ROOT_DIR}/benchmarks/data/generated/large/users.json"
BENCH_LARGE_NDJSON="${ROOT_DIR}/benchmarks/data/generated/large/users.ndjson"

# Select primary dataset based on PGO_DATASET_SIZE
case "$PGO_DATASET_SIZE" in
  small)
    PGO_PRIMARY_JSON="$BENCH_SMALL_JSON"
    PGO_PRIMARY_NDJSON="$BENCH_SMALL_NDJSON"
    ;;
  large)
    PGO_PRIMARY_JSON="$BENCH_LARGE_JSON"
    PGO_PRIMARY_NDJSON="$BENCH_LARGE_NDJSON"
    ;;
  medium|*)
    PGO_PRIMARY_JSON="$BENCH_MEDIUM_JSON"
    PGO_PRIMARY_NDJSON="$BENCH_MEDIUM_NDJSON"
    ;;
esac


run_gate_tests() {
  make test-cpp
  make test-py
}

# Ensure benchmark data exists (generated once). PGO runs only on LARGE data for representative profile.
# Use PYTHON=python so Make expands $(VENV)/bin/$(PYTHON) to .venv/bin/python (not .venv/bin/.venv/bin/python).
ensure_bench_data() {
  make bench-data
}

# Run comprehensive benchmark suite to generate representative profile data.
# Covers all hot paths: loads, dumps, NDJSON streaming, search queries.
run_benchmarks() {
  ensure_bench_data
  echo "Running PGO profiling benchmarks with $PGO_DATASET_SIZE dataset..."
  echo "  Primary JSON:   $PGO_PRIMARY_JSON"
  echo "  Primary NDJSON: $PGO_PRIMARY_NDJSON"

  # Main benchmark suite (comprehensive workload)
  PYTHONPATH=. "$VENV/bin/python" -m benchmarks.bench_main \
    --dataset "$PGO_PRIMARY_JSON" \
    --dataset "$PGO_PRIMARY_NDJSON" \
    --repeat "$PGO_BENCH_REPEAT" --warmup "$PGO_BENCH_WARMUP" --output "$PGO_DIR/bench_results_pgo.md"

  # Individual benchmark modules for targeted profiling:
  # 1. loads - parsing JSON to Python objects
  PYTHONPATH=. "$VENV/bin/python" -m benchmarks.bench_loads --data "$PGO_PRIMARY_JSON" --repeat "$PGO_BENCH_REPEAT" --warmup "$PGO_BENCH_WARMUP"

  # 2. dumps - serializing Python objects to JSON strings
  PYTHONPATH=. "$VENV/bin/python" -m benchmarks.bench_dumps --data "$PGO_PRIMARY_JSON" --repeat "$PGO_BENCH_REPEAT" --warmup "$PGO_BENCH_WARMUP"

  # 3. NDJSON streaming - line-by-line parsing
  PYTHONPATH=. "$VENV/bin/python" -m benchmarks.bench_ndjson --data "$PGO_PRIMARY_NDJSON" --repeat "$PGO_BENCH_REPEAT" --warmup "$PGO_BENCH_WARMUP"

  # 4. Search queries - search and query evaluation
  PYTHONPATH=. "$VENV/bin/python" -m benchmarks.bench_search --data "$PGO_PRIMARY_JSON" --repeat "$PGO_BENCH_REPEAT" --warmup "$PGO_BENCH_WARMUP"

  # Optional: Also run with small dataset for edge-case coverage if using larger primary
  if [ "$PGO_DATASET_SIZE" != "small" ]; then
    echo "Running additional small dataset for edge-case coverage..."
    PYTHONPATH=. "$VENV/bin/python" -m benchmarks.bench_loads --data "$BENCH_SMALL_JSON" --repeat 5 --warmup 1
    PYTHONPATH=. "$VENV/bin/python" -m benchmarks.bench_dumps --data "$BENCH_SMALL_JSON" --repeat 5 --warmup 1
  fi
}

compiler_kind="$(detect_compiler)"

echo "==> PGO workflow starting"
echo "    Compiler:     $compiler_kind"
echo "    Dataset size: $PGO_DATASET_SIZE"
echo "    Repeat:       $PGO_BENCH_REPEAT"
echo "    Warmup:       $PGO_BENCH_WARMUP"
ensure_venv

rm -rf "$PGO_DIR"
mkdir -p "$GEN_DIR"

echo "==> Phase 1: PGO generate build"
export STRATA_ENABLE_LTO=0
export PGO_MODE=generate
export STRATA_PGO_PROFILE="$PROFILE_DATA"

if [ "$compiler_kind" = "clang" ]; then
  export LLVM_PROFILE_FILE="$GEN_DIR/%p.profraw"
elif [ "$compiler_kind" = "gcc" ]; then
  export GCOV_PREFIX="$GEN_DIR"
  export GCOV_PREFIX_STRIP=0
fi

"$VENV/bin/python" -m pip install --force-reinstall --no-deps -e .
run_gate_tests
run_benchmarks

if [ "$compiler_kind" = "clang" ]; then
  merge_profraw
fi

echo "==> Phase 2: PGO use build (with LTO)"
unset LLVM_PROFILE_FILE GCOV_PREFIX GCOV_PREFIX_STRIP
export STRATA_ENABLE_LTO=1
export PGO_MODE=use
export STRATA_PGO_PROFILE="$PROFILE_DATA"

"$VENV/bin/python" -m pip install --force-reinstall --no-deps -e .
run_gate_tests
run_benchmarks

echo "==> PGO workflow complete"
echo ""
echo "Summary:"
echo "  Profile data:   $PROFILE_DATA"
echo "  Dataset used:   $PGO_DATASET_SIZE"
echo "  Benchmark results: $PGO_DIR/bench_results_pgo.md"
echo ""
echo "The extension is now optimized with PGO + LTO."
echo "For release builds, consider: PGO_DATASET_SIZE=large make pgo"
