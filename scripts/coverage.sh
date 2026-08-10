#!/usr/bin/env bash
# Coverage for both layers. Fronted by `make coverage-cpp`, `make coverage-py`,
# `make coverage`.
#
# The C++ side drives the SAME CMake registry as `make test-cpp`, just
# configured with -DCOVERAGE=ON. The previous implementation compiled coverage
# from a second, hand-maintained source list which drifted until it named
# deleted files and compiled nothing, and every failure was swallowed by
# `|| true` — so a green coverage run meant nothing. Neither is reproduced
# here: one registry, and errors propagate.
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

VENV="${VENV:-.venv}"
VPY="$VENV/bin/python"
# CI installs into the runner's interpreter and has no .venv (the missing
# fallback failed the coverage job's Python half).
[ -x "$VPY" ] || VPY="$(command -v python3)"
BUILD_DIR="${COVERAGE_BUILD_DIR:-build/coverage}"
REPORT_DIR="${COVERAGE_REPORT_DIR:-build/coverage/report}"

# llvm-cov and llvm-profdata must match the compiler that produced the data.
# Apple's toolchain exposes them only through xcrun.
llvm_tool() {
    local tool="$1"
    if command -v "$tool" >/dev/null 2>&1; then
        echo "$tool"
    elif command -v xcrun >/dev/null 2>&1 && xcrun --find "$tool" >/dev/null 2>&1; then
        echo "xcrun $tool"
    else
        echo "Error: $tool not found. Install LLVM (brew install llvm) or the" >&2
        echo "Xcode command line tools (xcode-select --install)." >&2
        exit 1
    fi
}

cov_cpp() {
    local profdata profcov
    profdata="$(llvm_tool llvm-profdata)"
    profcov="$(llvm_tool llvm-cov)"

    echo "==> coverage: configuring the C++ registry with -DCOVERAGE=ON"
    cmake -B "$BUILD_DIR" -S . -DCOVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
    cmake --build "$BUILD_DIR" --parallel

    rm -rf "$BUILD_DIR/profraw"
    mkdir -p "$BUILD_DIR/profraw" "$REPORT_DIR"

    echo "==> coverage: running the suites"
    # Each suite writes its own .profraw; %p keeps concurrent runs apart.
    LLVM_PROFILE_FILE="$ROOT_DIR/$BUILD_DIR/profraw/%p.profraw" \
        ctest --test-dir "$BUILD_DIR" --output-on-failure --no-tests=error

    local raw_files=("$BUILD_DIR"/profraw/*.profraw)
    if [[ ! -e "${raw_files[0]}" ]]; then
        echo "Error: no .profraw files were produced — the suites ran uninstrumented." >&2
        exit 1
    fi

    echo "==> coverage: merging ${#raw_files[@]} profiles"
    $profdata merge -sparse "${raw_files[@]}" -o "$BUILD_DIR/strata.profdata"

    # Every suite must be reported together: each links only the core sources
    # it needs, so a single binary would show the rest as 0%. llvm-cov takes
    # the first binary positionally and the rest through -object.
    local binaries=()
    while IFS= read -r binary; do
        binaries+=("$binary")
    done < <(find "$BUILD_DIR" -maxdepth 1 -type f -perm -u+x -name '*_tests' | sort)

    if [[ "${#binaries[@]}" -eq 0 ]]; then
        echo "Error: no test binaries found in $BUILD_DIR." >&2
        exit 1
    fi

    local objects=()
    for binary in "${binaries[@]:1}"; do
        objects+=(-object "$binary")
    done

    # llvm-cov takes source *files*, not directories. Listing them explicitly
    # also keeps libc++ headers — which carry coverage mapping of their own —
    # out of the report, so the percentages describe strata and nothing else.
    local sources=()
    while IFS= read -r source; do
        sources+=("$source")
    done < <(find src/strata include/strata \( -name '*.cpp' -o -name '*.hpp' \) | sort)

    if [[ "${#sources[@]}" -eq 0 ]]; then
        echo "Error: no sources found to report on." >&2
        exit 1
    fi

    echo "==> coverage: C++ report (${#sources[@]} files, ${#binaries[@]} binaries)"
    $profcov report "${binaries[0]}" "${objects[@]}" \
        -instr-profile="$BUILD_DIR/strata.profdata" \
        "${sources[@]}"
    $profcov show "${binaries[0]}" "${objects[@]}" \
        -instr-profile="$BUILD_DIR/strata.profdata" \
        -format=html -output-dir="$REPORT_DIR/cpp" \
        "${sources[@]}"
    echo "==> coverage: HTML written to $REPORT_DIR/cpp/index.html"
}

cov_py() {
    if [[ -z "$VPY" || ! -x "$VPY" ]]; then
        echo "Error: no usable python found. Run: make dev" >&2
        exit 1
    fi
    echo "==> coverage: Python suites"
    "$VPY" -m pytest tests/py tests/unit \
        --cov=strata --cov-report=term-missing \
        --cov-report="html:$REPORT_DIR/py"
    echo "==> coverage: HTML written to $REPORT_DIR/py/index.html"
}

case "${1:-all}" in
    cpp) cov_cpp ;;
    py) cov_py ;;
    all)
        cov_cpp
        cov_py
        ;;
    *)
        echo "Usage: scripts/coverage.sh cpp|py|all" >&2
        exit 2
        ;;
esac
