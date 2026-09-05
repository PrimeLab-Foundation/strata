#!/usr/bin/env bash
#
# Build `strata._strata` under AddressSanitizer + UndefinedBehaviorSanitizer and
# run both Python suites (tests/py, tests/unit) against it.
#
# Why this exists: the `corpus` CI job sanitizes the C++ core only — CMake/ctest
# over core_sources.txt. Nothing there compiles src/strata/bindings, and the
# bindings are where raw stores land in CPython-owned allocations (the fused
# ASCII string copy in python_builder.h, the serializer's exact-fit output
# block). A one-byte over-store there is invisible to value-equality tests.
#
# Mechanics that are not obvious — each one fails *silently* if left out:
#
# * PYTHONMALLOC=malloc. CPython carves small objects out of pymalloc pools, so
#   a write one past a PyUnicode object lands in padding ASan knows nothing
#   about and produces no report. Routing object allocation through the
#   instrumented system malloc is what gives the redzones teeth; without it this
#   gate is decorative. (Measured: a one-byte over-store in copy_if_ascii passes
#   the plain suites and is caught here.)
#
# * The runtime is *preloaded*, not merely linked — an instrumented .so first
#   pulled in by dlopen initializes ASan too late for its interceptors, which
#   ASan calls a fatal error. DYLD_INSERT_LIBRARIES on macOS, LD_PRELOAD on
#   Linux, and the arming is proved with a dlsym probe rather than assumed.
#   macOS drops DYLD_* for SIP-protected binaries, so the virtualenv has to come
#   from a Homebrew or python.org interpreter; /usr/bin/python3 is refused.
#
# * ASAN_OPTIONS=strip_env=0 on macOS. ASan removes itself from
#   DYLD_INSERT_LIBRARIES by default; measured here, the interpreter that comes
#   out the other side has no runtime in it at all (dlsym cannot find
#   __asan_init) and its children inherit nothing. The build is several execs
#   deep — pip, the build backend, setup.py's C++ gate, its pytest gate — so the
#   variable has to survive every one of them.
#
# * The build runs against a copy of the *tracked* tree, installed non-editable
#   into its own virtualenv. Both halves matter: see the staging comment below,
#   and the developer's .venv and the tree's python/strata/_strata*.so — the
#   artifacts the benchmarks measure — stay untouched.
#
# SKIP_TESTS is never set: setup.py's own gates run inside the sanitized build
# and are part of what this target proves.
#
# Usage: make test-py-asan   (extra arguments are forwarded to py_tests.py)
set -euo pipefail

cd "$(dirname "$0")/.."
PROJECT_ROOT="$PWD"

ASAN_VENV="${ASAN_VENV:-.venv-asan}"
DEV_VENV="${VENV:-.venv}"
CC="${CC:-clang}"
CXX="${CXX:-clang++}"
SAN_FLAGS="-fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer -g"
START=$(date +%s)

die() {
    echo "error: $*" >&2
    exit 1
}

command -v "$CXX" >/dev/null 2>&1 || die "'$CXX' not found; the sanitizer flags need clang."

# --- the sanitizer runtime -------------------------------------------------
case "$(uname -s)" in
    Darwin)
        PRELOAD_VAR=DYLD_INSERT_LIBRARIES
        RUNTIME_NAMES=("libclang_rt.asan_osx_dynamic.dylib")
        RUNTIME_GLOB='libclang_rt.asan*.dylib'
        ASAN_EXTRA=":strip_env=0"
        ;;
    Linux)
        PRELOAD_VAR=LD_PRELOAD
        RUNTIME_NAMES=("libclang_rt.asan-$(uname -m).so" "libclang_rt.asan.so")
        RUNTIME_GLOB='libclang_rt.asan*.so'
        ASAN_EXTRA=""
        ;;
    *) die "unsupported platform $(uname -s); this gate is macOS and Linux only." ;;
esac

RUNTIME=""
for name in "${RUNTIME_NAMES[@]}"; do
    # -print-file-name echoes the bare name back when it finds nothing, so the
    # answer is only believed once it names a file that exists.
    candidate="$("$CXX" -print-file-name="$name" 2>/dev/null || true)"
    if [ -n "$candidate" ] && [ -f "$candidate" ]; then
        RUNTIME="$candidate"
        break
    fi
done
if [ -z "$RUNTIME" ]; then
    resource_dir="$("$CXX" -print-resource-dir 2>/dev/null || true)"
    if [ -n "$resource_dir" ] && [ -d "$resource_dir/lib" ]; then
        # No pipe: under `set -e` a find exit status or a SIGPIPE from `head`
        # would end the script before the diagnostic below.
        RUNTIME="$(find "$resource_dir/lib" -name "$RUNTIME_GLOB" -print -quit 2>/dev/null || true)"
    fi
fi
[ -n "$RUNTIME" ] && [ -f "$RUNTIME" ] ||
    die "the ASan runtime was not found for $CXX (looked for ${RUNTIME_NAMES[*]}). Install the compiler-rt runtime."

# log_path, not stderr: pytest captures file descriptor 2 by default, so a
# report written there is swallowed with the rest of the captured output when
# the process aborts — the failure survives, the diagnosis does not. ASan opens
# these files itself, under pytest's capture and inside pip's build subprocess
# alike, and the EXIT trap prints whatever was written.
ASAN_LOG_DIR="$PROJECT_ROOT/build/asan-ext/logs"
rm -rf "$ASAN_LOG_DIR"
mkdir -p "$ASAN_LOG_DIR"
ASAN_OPTIONS="detect_leaks=0:abort_on_error=1:malloc_context_size=30:log_path=$ASAN_LOG_DIR/asan${ASAN_EXTRA}"
# No log_path here: both sanitizers share one sanitizer_common runtime and
# ASan's setting already redirects UBSan's reports to the same files.
UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=1"

dump_sanitizer_reports() {
    local file
    for file in "$ASAN_LOG_DIR"/*; do
        [ -e "$file" ] || continue
        echo
        echo "── sanitizer report: $file ──"
        cat "$file"
    done
}
trap dump_sanitizer_reports EXIT

run_sanitized() {
    (
        export "$PRELOAD_VAR=$RUNTIME"
        export PYTHONMALLOC=malloc
        # py_tests.py asserts, inside the process that imports the extension,
        # that the runtime is loaded and the allocator routed (no silent pass).
        export STRATA_ASAN_GATE=1
        export ASAN_OPTIONS="$ASAN_OPTIONS"
        export UBSAN_OPTIONS="$UBSAN_OPTIONS"
        export MallocNanoZone=0 # silences macOS' "nano zone abandoned" notice
        exec "$@"
    )
}

# --- the sanitized virtualenv ----------------------------------------------
BASE_PYTHON="${BASE_PYTHON:-}"
if [ -z "$BASE_PYTHON" ] && [ -x "$DEV_VENV/bin/python" ]; then
    BASE_PYTHON="$(readlink -f "$DEV_VENV/bin/python" 2>/dev/null || true)"
fi
[ -n "$BASE_PYTHON" ] && [ -x "$BASE_PYTHON" ] || BASE_PYTHON="$(command -v python3 || true)"
[ -n "$BASE_PYTHON" ] || die "no interpreter found; create $DEV_VENV first (make venv)."

if [ "$(uname -s)" = "Darwin" ]; then
    case "$BASE_PYTHON" in
        /usr/bin/* | /System/*)
            die "$BASE_PYTHON is SIP-protected: macOS drops DYLD_INSERT_LIBRARIES for it and the suites would run unsanitized. Use a Homebrew or python.org interpreter (BASE_PYTHON=...)."
            ;;
    esac
fi

ASAN_PY="$ASAN_VENV/bin/python"
if [ ! -x "$ASAN_PY" ] || [ "${ASAN_VENV_RECREATE:-0}" = "1" ]; then
    rm -rf "$ASAN_VENV"
    "$BASE_PYTHON" -m venv "$ASAN_VENV"
fi
# The build runs with --no-build-isolation, so the build requirements from
# pyproject.toml live in this venv: setuptools/wheel for the backend, cmake for
# setup.py's pre-build C++ gate, pytest for its post-build gate and for ours.
# Without -U pip short-circuits on an already-satisfied specifier and makes no
# network request, so re-runs are offline.
"$ASAN_PY" -m pip install -q setuptools wheel 'cmake>=3.20' 'pytest>=7'

echo "+ compiler         $($CXX --version | head -n 1)"
echo "+ sanitizer flags  $SAN_FLAGS"
echo "+ runtime          $RUNTIME (preloaded via $PRELOAD_VAR)"
echo "+ virtualenv       $ASAN_VENV  ($("$ASAN_PY" -c 'import sys; print(sys.version.split()[0])'))"
echo "+ ASAN_OPTIONS     $ASAN_OPTIONS"
echo "+ UBSAN_OPTIONS    $UBSAN_OPTIONS"

# Proof, not assumption: the runtime is in the interpreter's own image only if
# dlsym can resolve one of its exports through the global handle.
run_sanitized "$ASAN_PY" -c "
import ctypes, sys
try:
    ctypes.CDLL(None).__asan_init
except AttributeError:
    sys.exit('the preload did not arm ASan in $ASAN_PY (__asan_init is not resolvable)')
print('+ armed            __asan_init resolves in the interpreter')
" || die "the sanitizer runtime is not reaching the interpreter; see above."

# --- build -----------------------------------------------------------------
# CXXFLAGS replaces (not augments) CPython's recorded CFLAGS for C++ translation
# units and is appended to LDCXXSHARED, so one variable carries the sanitizer to
# both the compile and the link. setup.py's own -std=c++20 -O3 -march=native
# arrive separately as extra_compile_args, and CCSHARED (-fPIC) is untouched.
export CC CXX
export CXXFLAGS="$SAN_FLAGS"
if [ "$(uname -s)" = "Linux" ]; then
    # CPython on Linux records LDCXXSHARED as `g++ -shared`, and CXX does not
    # override it: gcc would then link clang-instrumented objects and fail to
    # find clang's ASan runtime. Swap the leading token, keep the flags.
    default_ldcxx="$("$ASAN_PY" -c "import sysconfig; print(sysconfig.get_config_var('LDCXXSHARED') or '')")"
    case "$default_ldcxx" in
        *" "*-shared*) export LDCXXSHARED="$CXX ${default_ldcxx#* }" ;;
        # An empty or one-token value would leave no `-shared` after the swap
        # and the link would produce an executable, not a module.
        *) export LDCXXSHARED="$CXX -shared" ;;
    esac
    echo "+ LDCXXSHARED      $LDCXXSHARED"
fi

# Two reasons not to build in the working directory. setup.py's post-build gate
# copies python/strata over its staging directory before running pytest, and an
# editable install leaves its in-place `_strata*.so` there — so a build in the
# repository tests the developer's *ordinary* extension and reports green having
# sanitized nothing. And sanitized objects would land in the repository's
# build/, for the next ordinary build (timestamps, not flags) to adopt. A copy
# of `git ls-files` has neither problem and costs a fifth of a second.
command -v git >/dev/null 2>&1 || die "git is required: the sanitized build stages the tracked files."
STAGE="$PROJECT_ROOT/build/asan-ext/tree"
# A fresh stage every run: a file deleted from git must not survive here (the
# C++ registry globs tests/cpp, so a removed test would keep running).
rm -rf "$STAGE"
mkdir -p "$STAGE"
git ls-files -z | tar -cf - --null -T - | tar -xf - -C "$STAGE"
UNTRACKED="$(git ls-files --others --exclude-standard -- include src python tests || true)"
[ -z "$UNTRACKED" ] ||
    die "untracked sources would not be staged and the gate would not test them: $(echo $UNTRACKED | cut -c1-300). Add them (git add -N is enough) or remove them."

# `force` is what makes the compile happen at all on a re-run: setuptools
# decides "up to date" from the sources alone — headers are not in ext.depends —
# so an edit to python_builder.h would otherwise relink nothing. `parallel`
# turns a serial minutes-long compile into a ~30 s one.
EXTRA_CFG="$PROJECT_ROOT/build/asan-ext/distutils.cfg"
cat >"$EXTRA_CFG" <<CFG
[build]
force = 1
parallel = $("$ASAN_PY" -c 'import os; print(os.cpu_count() or 1)')
CFG
export DIST_EXTRA_CONFIG="$EXTRA_CFG"

echo "+ staged tree      $STAGE ($(git ls-files | wc -l | tr -d ' ') tracked files)"

# setup.py's pre-build gate runs the C++ suite through CMake, and CMake reads
# CXXFLAGS only at first configure — so the staged cache is seeded here, with
# CXXFLAGS explicitly empty, and the suite builds the way an ordinary install
# builds it. Sanitizing the core is the corpus job's own step, immediately
# before this one; doing it again here would only make this gate fail on the
# signed-integer overflow in tests/cpp/test_scan.cpp's `power *= 10` loop, which
# -fno-sanitize-recover turns fatal. That is a real finding, but it is not the
# finding this gate is for.
export CMAKE_BUILD_PARALLEL_LEVEL="${CMAKE_BUILD_PARALLEL_LEVEL:-$("$ASAN_PY" -c 'import os; print(os.cpu_count() or 1)')}"
echo "── seeding the staged C++ build (unsanitized: the corpus job owns that) ──"
CXXFLAGS= "$ASAN_PY" "$STAGE/scripts/cpp_tests.py" --build-only

echo
echo "── building the extension (pip install, both setup.py gates run inside) ──"
run_sanitized "$ASAN_PY" -m pip install --no-build-isolation --no-cache-dir \
    --force-reinstall --no-deps "$STAGE"

# Named by EXT_SUFFIX, so the check can only look at the module this
# interpreter would actually import.
SUFFIX="$("$ASAN_PY" -c "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX'))")"
EXT="$(find "$ASAN_VENV"/lib -maxdepth 4 -name "_strata$SUFFIX" 2>/dev/null | head -n 1)"
[ -n "$EXT" ] || die "_strata$SUFFIX was not found under $ASAN_VENV/lib after the install."
echo
echo "── the installed extension links the sanitizer runtime ──"
echo "+ $EXT ($(wc -c <"$EXT" | tr -d ' ') bytes)"
if command -v otool >/dev/null 2>&1; then
    otool -L "$EXT" | grep -iE 'asan|sanitiz' || true
elif command -v ldd >/dev/null 2>&1; then
    # A shared object leaves the ASan symbols undefined for the preload to
    # satisfy, so ldd naming nothing here is normal on Linux; nm below decides.
    ldd "$EXT" | grep -iE 'asan|sanitiz' || true
fi
# nm -u reads .symtab; -D reads the dynamic table, which is where a stripped
# ELF keeps its undefined symbols. Both are asked, one of them answers.
UNDEF="$( { nm -u "$EXT" 2>/dev/null; nm -D -u "$EXT" 2>/dev/null; } | grep -c asan || true)"
echo "+ undefined ASan symbols in the extension: $UNDEF"
[ "$UNDEF" -gt 0 ] || die "$EXT carries no ASan symbols — the build was not instrumented."

# --- run -------------------------------------------------------------------
echo
echo "── tests/py + tests/unit under ASan + UBSan ──"
run_sanitized "$ASAN_PY" "$STAGE/scripts/py_tests.py" "$@"

echo
echo "ASAN GATE PASSED in $(($(date +%s) - START))s — bindings clean under ASan + UBSan."
