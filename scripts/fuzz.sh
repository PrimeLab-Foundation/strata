#!/usr/bin/env bash
# Build and run the libFuzzer targets. Fronted by `make fuzz-build`,
# `make fuzz-run`, `make fuzz` (docs/context/convention.md, "Automation").
#
# The corpus directories are committed (tests/fuzz/corpus/{loads,ndjson}) —
# the previous implementation passed them on the command line without ever
# creating them, so every run, including the weekly CI job, died at startup
# with "No such file or directory". They are checked here too, so the failure
# is named rather than inferred from libFuzzer's output.
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

BUILD_DIR="${FUZZ_BUILD_DIR:-build/fuzz}"
CORPUS_LOADS="${FUZZ_CORPUS_LOADS:-tests/fuzz/corpus/loads}"
CORPUS_NDJSON="${FUZZ_CORPUS_NDJSON:-tests/fuzz/corpus/ndjson}"
FUZZ_TIME="${FUZZ_TIME:-120}"
ARTIFACT_DIR="${FUZZ_ARTIFACT_DIR:-fuzz_crashes}"

usage() {
    cat >&2 <<'EOF'
Usage: scripts/fuzz.sh build|run|all

  build  Configure with -DFUZZ=ON and build fuzz_loads and fuzz_ndjson
  run    Run both targets over the committed corpus (requires build)
  all    build, then run

Environment:
  FUZZ_TIME          seconds per target (default 120)
  FUZZ_BUILD_DIR     build directory (default build/fuzz)
  FUZZ_ARTIFACT_DIR  where crashing inputs are written (default fuzz_crashes)
  FUZZ_CORPUS_LOADS / FUZZ_CORPUS_NDJSON   corpus directories
  CXX                compiler; on macOS, Homebrew LLVM is selected automatically
EOF
    exit 2
}

# Apple's clang compiles -fsanitize=fuzzer but ships no libFuzzer runtime, so
# the failure lands at link time as a missing libclang_rt.fuzzer_osx.a. Prefer
# Homebrew's LLVM when the caller has not chosen a compiler.
resolve_compiler() {
    if [[ -n "${CXX:-}" ]]; then
        echo "$CXX"
        return
    fi
    if [[ "$(uname -s)" == "Darwin" ]] && command -v brew >/dev/null 2>&1; then
        local prefix
        prefix="$(brew --prefix llvm 2>/dev/null || true)"
        if [[ -n "$prefix" && -x "$prefix/bin/clang++" ]]; then
            echo "$prefix/bin/clang++"
            return
        fi
    fi
    echo "clang++"
}

require_corpus() {
    local missing=0
    for dir in "$CORPUS_LOADS" "$CORPUS_NDJSON"; do
        if [[ ! -d "$dir" ]]; then
            echo "Error: corpus directory is missing: $dir" >&2
            missing=1
        elif [[ -z "$(ls -A "$dir")" ]]; then
            echo "Error: corpus directory is empty: $dir" >&2
            missing=1
        fi
    done
    if [[ "$missing" -ne 0 ]]; then
        echo "The seed corpus is committed to git; restore it before fuzzing." >&2
        exit 1
    fi
}

cmd_build() {
    local cxx
    cxx="$(resolve_compiler)"
    echo "==> fuzz: configuring (FUZZ=ON, CXX=$cxx)"
    # tests/fuzz/CMakeLists.txt probes for the runtime and fails with an
    # actionable message when the toolchain has none.
    cmake -B "$BUILD_DIR" -S . -DFUZZ=ON -DCMAKE_CXX_COMPILER="$cxx"
    echo "==> fuzz: building"
    cmake --build "$BUILD_DIR" --target fuzz_loads fuzz_ndjson
}

target_path() {
    local name="$1"
    for candidate in "$BUILD_DIR/tests/fuzz/$name" "$BUILD_DIR/$name"; do
        if [[ -x "$candidate" ]]; then
            echo "$candidate"
            return
        fi
    done
    echo "Error: $name not built. Run: make fuzz-build" >&2
    exit 1
}

cmd_run() {
    require_corpus
    local loads ndjson
    loads="$(target_path fuzz_loads)"
    ndjson="$(target_path fuzz_ndjson)"
    mkdir -p "$ARTIFACT_DIR"

    echo "==> fuzz: fuzz_loads for ${FUZZ_TIME}s over $CORPUS_LOADS"
    "$loads" "$CORPUS_LOADS" \
        -max_total_time="$FUZZ_TIME" \
        -artifact_prefix="$ARTIFACT_DIR/loads_"

    echo "==> fuzz: fuzz_ndjson for ${FUZZ_TIME}s over $CORPUS_NDJSON"
    "$ndjson" "$CORPUS_NDJSON" \
        -max_total_time="$FUZZ_TIME" \
        -artifact_prefix="$ARTIFACT_DIR/ndjson_"

    echo "==> fuzz: both targets finished with no crashes"
}

case "${1:-}" in
    build) cmd_build ;;
    run) cmd_run ;;
    all)
        cmd_build
        cmd_run
        ;;
    *) usage ;;
esac
