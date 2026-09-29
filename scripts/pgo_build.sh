#!/usr/bin/env bash
# Two-phase PGO + LTO build. Fronted by `make pgo`.
#
#   phase 1  instrumented build (no LTO) -> training data -> training workload
#            -> gate tests -> llvm-profdata merge
#   phase 2  rebuild with the merged profile and LTO -> gate tests
#   phase 3  (clang) the hook image alone: instrumented -> native training
#            workload -> its own profile -> rebuild with it and LTO -> gate
#            tests; _strata's image is checked unchanged
#            -> verification benchmarks
#
# Both phases run the gate: an optimized build that fails its tests is worth
# nothing, and PGO is exactly the kind of change that can miscompile. Phase 1
# runs it with --training, which leaves the native-type suites out of the
# profile (docs/architecture/native_types.md, "Hot-path protection" 4); phase 2
# runs all of it.
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
    "$VPY" scripts/py_tests.py "$@"
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

# setup.py builds strata._dumps_hook unprofiled in both phases: nothing it runs
# may enter _strata's profile. _strata's instrumented image is the scan's control.
echo "==> PGO: the hook image must carry no profile"
"$VPY" scripts/build_identity.py --check-unprofiled strata._dumps_hook --instrumented strata._strata

echo "==> PGO: generating training data"
"$VPY" scripts/pgo_training_data.py --out-dir "$PGO_DIR"

echo "==> PGO: running the training workload"
PYTHONPATH=. "$VPY" scripts/pgo_training.py \
    --json "$PGO_DIR/train.json" \
    --ndjson "$PGO_DIR/train.ndjson" \
    --work-dir "$WORK_DIR"

echo "==> PGO: gate tests on the instrumented build"
gate_tests --training

if [[ "$KIND" == "clang" ]]; then
    shopt -s nullglob
    raw_files=("$RAW_DIR"/*.profraw)
    shopt -u nullglob
    if [[ "${#raw_files[@]}" -eq 0 ]]; then
        echo "Error: no .profraw files were written — the build was not instrumented." >&2
        exit 1
    fi
    # %m names each file after the signature of the image that wrote it, so one
    # instrumented image (_strata) leaves one signature; a second is another
    # image — the hook — writing counts into this profile.
    signatures="$(printf '%s\n' "${raw_files[@]##*/}" |
        sed -E 's/^[0-9]+-([0-9]+)_[0-9]+\.profraw$/\1/' | sort -u)"
    if [[ "$(printf '%s\n' "$signatures" | wc -l | tr -d ' ')" -ne 1 ]]; then
        echo "Error: raw profiles from more than one instrumented image: $(tr '\n' ' ' <<<"$signatures")" >&2
        exit 1
    fi
    # Resolved into a variable first: `$(profdata_tool) merge ...` would run the
    # helper in a subshell, where its `exit 1` cannot stop this script.
    merge_tool="$(profdata_tool)"
    echo "==> PGO: merging ${#raw_files[@]} raw profiles"
    $merge_tool merge -output="$PROFILE" "${raw_files[@]}"
else
    # GCOV_PREFIX mirrors each object's path, and setup.py compiles the hook
    # under a build_temp of its own, so its counts would sit below that name.
    hook_counts="$(find "$RAW_DIR" -name '*.gcda' -path '*/strata._dumps_hook/*' -print -quit)"
    if [[ -n "$hook_counts" ]]; then
        echo "Error: the hook image wrote profile counts: $hook_counts" >&2
        exit 1
    fi
    # gcc writes .gcda directly; -fprofile-use reads the tree.
    PROFILE="$ROOT_DIR/$RAW_DIR"
fi

# --- phase 2: optimize -----------------------------------------------------
"$VPY" scripts/build_identity.py --profile "$PROFILE" --raw "$RAW_DIR" --recipe gate-inclusive-posix-v1
echo "==> PGO phase 2: optimized build (profile + LTO)"
unset LLVM_PROFILE_FILE GCOV_PREFIX GCOV_PREFIX_STRIP
export PGO_MODE=use
export STRATA_ENABLE_LTO=1
export STRATA_PGO_PROFILE="$PROFILE"
"$VPY" -m pip install --force-reinstall --no-deps -e .

echo "==> PGO: the hook image must carry no profile"
"$VPY" scripts/build_identity.py --check-unprofiled strata._dumps_hook

echo "==> PGO: gate tests on the optimized build"
gate_tests

# --- phase 3: the hook's own profile ---------------------------------------
# strata._dumps_hook (native=True, parse_types, dumps_with_default) gets a
# profile of its own, trained by a native workload once _strata is optimized
# and uninstrumented, so no count of either image reaches the other's profile
# (docs/architecture/native_types.md, "Hook profile"). Only the hook is
# rebuilt (STRATA_EXTENSIONS), and _strata's image must come out unchanged.
# clang only: the CI legs all build with clang; a gcc build keeps the hook plain.
if [[ "$KIND" == "clang" ]]; then
    HOOK_RAW_DIR="$PGO_DIR/hook-raw"
    HOOK_GATE_DIR="$PGO_DIR/hook-gate-discarded"
    HOOK_PROFILE="$ROOT_DIR/$PGO_DIR/hook.profdata"
    mkdir -p "$HOOK_RAW_DIR" "$HOOK_GATE_DIR"
    strata_image="$("$VPY" -c 'import importlib.util as u; print(u.find_spec("strata._strata").origin)')"
    strata_hash="$(shasum -a 256 "$strata_image" | cut -d' ' -f1)"
    unset PGO_MODE STRATA_PGO_PROFILE
    export STRATA_ENABLE_LTO=0
    export STRATA_EXTENSIONS=strata._dumps_hook

    echo "==> PGO phase 3a: instrumented hook"
    # The build gate runs against the instrumented hook; its counts go to a
    # directory that is thrown away: the hook trains on its workload alone.
    STRATA_HOOK_PGO_MODE=generate LLVM_PROFILE_FILE="$ROOT_DIR/$HOOK_GATE_DIR/%p-%m.profraw" \
        "$VPY" -m pip install --force-reinstall --no-deps -e .
    echo "==> PGO: the hook's native training workload"
    LLVM_PROFILE_FILE="$ROOT_DIR/$HOOK_RAW_DIR/%p-%m.profraw" PYTHONPATH=. \
        "$VPY" scripts/pgo_hook_training.py --work-dir "$WORK_DIR"
    shopt -s nullglob
    hook_raw=("$HOOK_RAW_DIR"/*.profraw)
    shopt -u nullglob
    if [[ "${#hook_raw[@]}" -eq 0 ]]; then
        echo "Error: the hook's training wrote no .profraw -- it was not instrumented." >&2
        exit 1
    fi
    hook_signatures="$(printf '%s\n' "${hook_raw[@]##*/}" |
        sed -E 's/^[0-9]+-([0-9]+)_[0-9]+\.profraw$/\1/' | sort -u)"
    if [[ "$(printf '%s\n' "$hook_signatures" | wc -l | tr -d ' ')" -ne 1 ]]; then
        echo "Error: hook training profiles from more than one image: $(tr '\n' ' ' <<<"$hook_signatures")" >&2
        exit 1
    fi
    merge_tool="$(profdata_tool)"
    $merge_tool merge -output="$HOOK_PROFILE" "${hook_raw[@]}"
    "$VPY" scripts/build_identity.py --profile "$HOOK_PROFILE" --raw "$HOOK_RAW_DIR" \
        --recipe hook-native-training-v1

    echo "==> PGO phase 3b: the hook against its own profile (+ LTO)"
    STRATA_HOOK_PGO_MODE=use STRATA_HOOK_PGO_PROFILE="$HOOK_PROFILE" \
        "$VPY" -m pip install --force-reinstall --no-deps -e .
    unset STRATA_EXTENSIONS STRATA_ENABLE_LTO

    echo "==> PGO: _strata unchanged, the hook on its own profile alone"
    if [[ "$(shasum -a 256 "$strata_image" | cut -d' ' -f1)" != "$strata_hash" ]]; then
        echo "Error: the hook phase changed _strata's image ($strata_image)." >&2
        exit 1
    fi
    "$VPY" scripts/build_identity.py --check-profiled strata._dumps_hook \
        --profile "$HOOK_PROFILE" --foreign "$PROFILE"
    "$VPY" scripts/build_identity.py --check-profiled strata._strata \
        --profile "$PROFILE" --foreign "$HOOK_PROFILE"

    echo "==> PGO: gate tests on the optimized hook"
    gate_tests
else
    echo "==> PGO: $KIND build -- the hook stays unprofiled (phase 3 is clang-only)"
fi

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
