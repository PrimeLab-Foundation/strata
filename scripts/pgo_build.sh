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

# Large dataset only: PGO profile is built from the biggest workload (Rule 13 / benchmark gating).
BENCH_SMALL_JSON="${ROOT_DIR}/benchmarks/data/generated/small/users.json"
BENCH_SMALL_NDJSON="${ROOT_DIR}/benchmarks/data/generated/small/users.ndjson"
#BENCH_MEDIUM_JSON="${ROOT_DIR}/benchmarks/data/generated/medium/users.json"
#BENCH_MEDIUM_NDJSON="${ROOT_DIR}/benchmarks/data/generated/medium/users.ndjson"
#BENCH_LARGE_JSON="${ROOT_DIR}/benchmarks/data/generated/large/users.json"
#BENCH_LARGE_NDJSON="${ROOT_DIR}/benchmarks/data/generated/large/users.ndjson"


run_gate_tests() {
  make test-cpp
  make test-py
}

# Ensure benchmark data exists (generated once). PGO runs only on LARGE data for representative profile.
# Use PYTHON=python so Make expands $(VENV)/bin/$(PYTHON) to .venv/bin/python (not .venv/bin/.venv/bin/python).
ensure_bench_data() {
  make bench-data
}

run_benchmarks() {
  ensure_bench_data
  PYTHONPATH=. "$VENV/bin/python" -m benchmarks.bench_main \
    --dataset "$BENCH_SMALL_JSON" \
    --dataset "$BENCH_SMALL_NDJSON" \
    --repeat "$PGO_BENCH_REPEAT" --warmup "$PGO_BENCH_WARMUP" --output "$PGO_DIR/bench_results_pgo.md"
  PYTHONPATH=. "$VENV/bin/python" -m benchmarks.bench_loads --data "$BENCH_SMALL_JSON" --repeat "$PGO_BENCH_REPEAT" --warmup "$PGO_BENCH_WARMUP"
  PYTHONPATH=. "$VENV/bin/python" -m benchmarks.bench_dumps --data "$BENCH_SMALL_JSON" --repeat "$PGO_BENCH_REPEAT" --warmup "$PGO_BENCH_WARMUP"
  PYTHONPATH=. "$VENV/bin/python" -m benchmarks.bench_ndjson --data "$BENCH_SMALL_NDJSON" --repeat "$PGO_BENCH_REPEAT" --warmup "$PGO_BENCH_WARMUP"
  PYTHONPATH=. "$VENV/bin/python" -m benchmarks.bench_jsonpath --data "$BENCH_SMALL_JSON" --repeat "$PGO_BENCH_REPEAT" --warmup "$PGO_BENCH_WARMUP"
}

compiler_kind="$(detect_compiler)"

echo "==> PGO workflow starting (compiler: $compiler_kind)"
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
echo "Profile data: $PROFILE_DATA"
