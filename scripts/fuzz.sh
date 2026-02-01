#!/usr/bin/env bash
# Fuzzing: build and/or run libFuzzer targets (Rule 16: automation in scripts/).
# Canonical interface: make fuzz-build, make fuzz-run.
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

BUILD_DIR="${FUZZ_BUILD_DIR:-build_fuzz}"
CORPUS_LOADS="${FUZZ_CORPUS_LOADS:-tests/fuzz/corpus/loads}"
CORPUS_NDJSON="${FUZZ_CORPUS_NDJSON:-tests/fuzz/corpus/ndjson}"
MAX_TOTAL_TIME="${FUZZ_TIME:-120}"
ARTIFACT_PREFIX="${FUZZ_ARTIFACT_PREFIX:-fuzz_crashes/}"
CXX="${CXX:-clang++}"

# On macOS, system Clang (Xcode/Command Line Tools) does not ship libFuzzer.
# Prefer Homebrew LLVM when available so that make fuzz works without extra setup.
if [[ "$(uname -s)" == "Darwin" ]]; then
  CXX_RESOLVED="$(command -v "$CXX" 2>/dev/null)" || true
  if [[ "$CXX_RESOLVED" == "/usr/bin/clang++" || "$CXX_RESOLVED" == "/usr/bin/clang" || ( "$CXX" == "clang++" && -z "${CXX_RESOLVED:-}" ) ]]; then
    LLVM_PREFIX="$(command -v brew &>/dev/null && brew --prefix llvm 2>/dev/null)" || true
    if [[ -n "${LLVM_PREFIX:-}" && -x "${LLVM_PREFIX}/bin/clang++" ]]; then
      CXX="${LLVM_PREFIX}/bin/clang++"
    fi
  fi
fi

usage() {
  echo "Usage: $0 build | run | all"
  echo "  build  - Configure (FUZZ=ON) and build fuzz_loads, fuzz_ndjson"
  echo "  run    - Run both fuzzers (requires build first)"
  echo "  all    - build then run"
  echo "Env: FUZZ_BUILD_DIR, FUZZ_CORPUS_LOADS, FUZZ_CORPUS_NDJSON, FUZZ_TIME, FUZZ_ARTIFACT_PREFIX, CXX"
  exit 1
}

cmd_build() {
  # Pre-check: libFuzzer must be available. On macOS, system Clang does not ship it.
  # When using Homebrew LLVM on macOS, link against its libc++ so the fuzzer runtime resolves.
  FUZZ_CHECK_EXTRA=()
  if [[ "$(uname -s)" == "Darwin" && "$CXX" == *"opt/llvm"* || "$CXX" == *"Cellar/llvm"* ]]; then
    LLVM_PREFIX_FUZZ="${CXX%/bin/clang++}"
    [[ -z "$LLVM_PREFIX_FUZZ" ]] && LLVM_PREFIX_FUZZ="$(command -v brew &>/dev/null && brew --prefix llvm 2>/dev/null)" || true
    if [[ -n "${LLVM_PREFIX_FUZZ:-}" && -d "${LLVM_PREFIX_FUZZ}/lib/c++" ]]; then
      FUZZ_CHECK_EXTRA=(-L"${LLVM_PREFIX_FUZZ}/lib/c++" -Wl,-rpath,"${LLVM_PREFIX_FUZZ}/lib/c++")
    fi
  fi
  TMP_CXX="$(mktemp -t fuzz_check.XXXXXX.cpp)"
  trap 'rm -f "$TMP_CXX"' EXIT
  echo 'extern "C" int LLVMFuzzerTestOneInput(const unsigned char*, unsigned long) { return 0; }' > "$TMP_CXX"
  if ! "$CXX" -fsanitize=fuzzer,address -x c++ "$TMP_CXX" -o /dev/null "${FUZZ_CHECK_EXTRA[@]}" 2>/dev/null; then
    if [[ "$(uname -s)" == "Darwin" ]]; then
      echo "Error: libFuzzer is not available with this compiler ($CXX)."
      echo "On macOS, install LLVM via Homebrew and run:"
      echo "  brew install llvm"
      echo "  make fuzz"
      echo "Or set CXX explicitly: CXX=\$(brew --prefix llvm)/bin/clang++ make fuzz"
      echo "See docs/development/fuzzing.md for details."
    else
      echo "Error: libFuzzer not available. Use Clang (e.g. CXX=clang++ make fuzz)."
    fi
    exit 1
  fi

  echo "==> Fuzz: configuring (FUZZ=ON, CXX=$CXX)"
  cmake -B "$BUILD_DIR" -S . -DFUZZ=ON -DCMAKE_CXX_COMPILER="$CXX"
  echo "==> Fuzz: building fuzz_loads fuzz_ndjson"
  cmake --build "$BUILD_DIR" --target fuzz_loads fuzz_ndjson
  echo "==> Fuzz: build complete ($BUILD_DIR/fuzz_loads, $BUILD_DIR/fuzz_ndjson)"
}

# CMake puts fuzz executables in tests/fuzz/ inside the build dir
FUZZ_LOADS="${BUILD_DIR}/tests/fuzz/fuzz_loads"
FUZZ_NDJSON="${BUILD_DIR}/tests/fuzz/fuzz_ndjson"
# Fallback to build root for older or custom layouts
[[ ! -f "$FUZZ_LOADS" && -f "$BUILD_DIR/fuzz_loads" ]] && FUZZ_LOADS="$BUILD_DIR/fuzz_loads"
[[ ! -f "$FUZZ_NDJSON" && -f "$BUILD_DIR/fuzz_ndjson" ]] && FUZZ_NDJSON="$BUILD_DIR/fuzz_ndjson"

cmd_run() {
  if [[ ! -f "$FUZZ_LOADS" ]] || [[ ! -f "$FUZZ_NDJSON" ]]; then
    echo "Error: fuzz targets not found. Run: make fuzz-build"
    exit 1
  fi
  mkdir -p "${ARTIFACT_PREFIX%/}"
  echo "==> Fuzz: running fuzz_loads (max_total_time=${MAX_TOTAL_TIME}s)"
  "$FUZZ_LOADS" "$CORPUS_LOADS" -max_total_time="$MAX_TOTAL_TIME" -artifact_prefix="$ARTIFACT_PREFIX"loads_
  echo "==> Fuzz: running fuzz_ndjson (max_total_time=${MAX_TOTAL_TIME}s)"
  "$FUZZ_NDJSON" "$CORPUS_NDJSON" -max_total_time="$MAX_TOTAL_TIME" -artifact_prefix="$ARTIFACT_PREFIX"ndjson_
  echo "==> Fuzz: run complete (no crashes)"
}

case "${1:-}" in
  build) cmd_build ;;
  run)   cmd_run ;;
  all)   cmd_build; cmd_run ;;
  *)     usage ;;
esac
