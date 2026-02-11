#!/usr/bin/env bash
# Run C++ tests without CMake (Rule 16: automation in scripts/).
# Canonical interface: make test-cpp
#
# Performance:
#   1. Core sources → .o files compiled in parallel (once, cached).
#   2. Core objects → static library (ar, once, cached).
#   3. All 23 test .cpp → .o compiled in parallel.
#   4. All 23 test .o → executables linked in parallel against the library.
#   5. Executables run sequentially (output stays readable).
#
# Cold build: ~10 s.  Warm (no source changes): ~4 s.
set -e

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

BUILD_DIR="${BUILD_CPP_TESTS_DIR:-build_cpp_tests}"
OBJ_DIR="$BUILD_DIR/obj"
mkdir -p "$OBJ_DIR"

CXX="${CXX:-clang++}"
CXXFLAGS="-std=c++20 -Wall -Wextra -O2"
INCLUDES="-Iinclude -Isrc"
HEADER_FIND_EXPR=( -type f \( -name '*.h' -o -name '*.hpp' -o -name '*.hh' -o -name '*.inl' \) )

header_newer_than() {
    local target="$1"
    if find include src "${HEADER_FIND_EXPR[@]}" -newer "$target" -print -quit | grep -q .; then
        return 0
    fi
    return 1
}

CORE_SOURCES=(
    src/strata/json/json_parse.cpp
    src/strata/json/json_cursor.cpp
    src/strata/json/json_document.cpp
    src/strata/json/json_serialize.cpp
    src/strata/json/json_tape.cpp
    src/strata/json/json_mmap.cpp
    src/strata/json/json_lazy_cursor.cpp
    src/strata/json/ndjson_stream.cpp
    src/strata/json/parallel_ndjson.cpp
    src/strata/search/search_compile.cpp
    src/strata/search/search_eval.cpp
    src/strata/util/ryu_dtoa.cpp
    src/strata/util/dragonbox.cpp
    src/strata/util/simd_escape.cpp
    src/strata/util/simd_newline.cpp
    src/strata/util/simd_utf8.cpp
    src/strata/util/simd_numbers.cpp
    src/strata/util/simd_structural.cpp
    src/strata/util/fast_parse.cpp
    src/strata/util/thread_pool.cpp
)

# All tests: "name:file" pairs
TESTS=(
    "json_parse_tests:tests/cpp/test_json_parse.cpp"
    "json_cursor_tests:tests/cpp/test_json_cursor.cpp"
    "json_document_tests:tests/cpp/test_json_document.cpp"
    "json_serialize_tests:tests/cpp/test_json_serialize.cpp"
    "search_tests:tests/cpp/test_search.cpp"
    "search_advanced_tests:tests/cpp/test_search_advanced.cpp"
    "ndjson_tests:tests/cpp/test_ndjson.cpp"
    "parallel_ndjson_tests:tests/cpp/test_parallel_ndjson.cpp"
    "json_tape_tests:tests/cpp/test_json_tape.cpp"
    "json_mmap_tests:tests/cpp/test_json_mmap.cpp"
    "lazy_cursor_tests:tests/cpp/test_lazy_cursor.cpp"
    "lazy_string_tests:tests/cpp/test_lazy_string.cpp"
    "simd_escape_tests:tests/cpp/test_simd_escape.cpp"
    "simd_newline_tests:tests/cpp/test_simd_newline.cpp"
    "simd_structural_tests:tests/cpp/test_simd_structural.cpp"
    "unicode_and_escapes_tests:tests/cpp/test_unicode_and_escapes.cpp"
    "fast_parse_tests:tests/cpp/test_fast_parse.cpp"
    "parsing_errors_tests:tests/cpp/test_parsing_errors.cpp"
    "integer_optimization_tests:tests/cpp/test_integer_optimization.cpp"
    "float_precision_tests:tests/cpp/test_float_precision.cpp"
    "output_buffer_tests:tests/cpp/test_output_buffer.cpp"
    "stress_large_tests:tests/cpp/test_stress_large.cpp"
    "stress_deep_tests:tests/cpp/test_stress_deep.cpp"
)

LIB="$BUILD_DIR/libstrata_core.a"

# ── Step 1: compile core objects in parallel (cached) ──────────────────
echo "════════════════════════════════════════════════════════════════"
echo "  Step 1/4: Compiling core object files"
echo "════════════════════════════════════════════════════════════════"

CORE_OBJECTS=()
PIDS=()

for src in "${CORE_SOURCES[@]}"; do
    obj="$OBJ_DIR/$(basename "${src%.cpp}.o")"
    CORE_OBJECTS+=("$obj")
    if [[ -f "$obj" ]] && [[ "$obj" -nt "$src" ]] && ! header_newer_than "$obj"; then
        continue
    fi
    $CXX $CXXFLAGS $INCLUDES -c "$src" -o "$obj" &
    PIDS+=($!)
done

for pid in "${PIDS[@]}"; do
    wait "$pid" || { echo "❌ Core object compilation failed"; exit 1; }
done
echo "  ✅ ${#CORE_OBJECTS[@]} core objects ready"

# ── Step 2: archive into static library (cached) ──────────────────────
echo ""
echo "════════════════════════════════════════════════════════════════"
echo "  Step 2/4: Building static library"
echo "════════════════════════════════════════════════════════════════"

# Rebuild the library if any object is newer.
NEED_AR=false
if [[ ! -f "$LIB" ]]; then
    NEED_AR=true
else
    for obj in "${CORE_OBJECTS[@]}"; do
        if [[ "$obj" -nt "$LIB" ]]; then
            NEED_AR=true
            break
        fi
    done
fi

if $NEED_AR; then
    ar rcs "$LIB" "${CORE_OBJECTS[@]}"
    echo "  ✅ $LIB created"
else
    echo "  ✅ $LIB up to date"
fi

# ── Step 3: compile + link all test executables in parallel ────────────
echo ""
echo "════════════════════════════════════════════════════════════════"
echo "  Step 3/4: Building ${#TESTS[@]} test executables (parallel)"
echo "════════════════════════════════════════════════════════════════"

PIDS=()
BUILD_LOGS_DIR="$BUILD_DIR/logs"
mkdir -p "$BUILD_LOGS_DIR"

for entry in "${TESTS[@]}"; do
    name="${entry%%:*}"
    file="${entry#*:}"
    (
        # Compile test .cpp → .o (skip if cached)
        test_obj="$OBJ_DIR/${name}.o"
        if [[ -f "$test_obj" ]] && [[ "$test_obj" -nt "$file" ]] && ! header_newer_than "$test_obj"; then
            :
        else
            $CXX $CXXFLAGS $INCLUDES -c "$file" -o "$test_obj" 2>"$BUILD_LOGS_DIR/${name}_compile.log" || {
                echo "COMPILE_FAIL" > "$BUILD_LOGS_DIR/${name}.status"
                exit 1
            }
        fi
        # Link test .o + library → executable (skip if cached)
        exe="$BUILD_DIR/$name"
        if [[ ! "$exe" -nt "$test_obj" ]] || [[ ! "$exe" -nt "$LIB" ]]; then
            $CXX $CXXFLAGS "$test_obj" "$LIB" -o "$exe" 2>"$BUILD_LOGS_DIR/${name}_link.log" || {
                echo "LINK_FAIL" > "$BUILD_LOGS_DIR/${name}.status"
                exit 1
            }
        fi
        echo "OK" > "$BUILD_LOGS_DIR/${name}.status"
    ) &
    PIDS+=($!)
done

# Wait for all builds.
BUILD_OK=true
for pid in "${PIDS[@]}"; do
    wait "$pid" || BUILD_OK=false
done

# Check per-test status.
BUILD_FAILED=()
for entry in "${TESTS[@]}"; do
    name="${entry%%:*}"
    status_file="$BUILD_LOGS_DIR/${name}.status"
    if [[ ! -f "$status_file" ]] || [[ "$(cat "$status_file")" != "OK" ]]; then
        BUILD_FAILED+=("$name")
    fi
done

if [[ ${#BUILD_FAILED[@]} -gt 0 ]]; then
    echo ""
    for name in "${BUILD_FAILED[@]}"; do
        echo "  ❌ Build failed: $name"
        [[ -f "$BUILD_LOGS_DIR/${name}_compile.log" ]] && cat "$BUILD_LOGS_DIR/${name}_compile.log"
        [[ -f "$BUILD_LOGS_DIR/${name}_link.log" ]]    && cat "$BUILD_LOGS_DIR/${name}_link.log"
    done
    exit 1
fi

echo "  ✅ All ${#TESTS[@]} executables built"

# ── Step 4: run tests sequentially (readable output) ──────────────────
echo ""
echo "════════════════════════════════════════════════════════════════"
echo "  Step 4/4: Running ${#TESTS[@]} test suites"
echo "════════════════════════════════════════════════════════════════"
echo ""

PASSED=0
FAILED=0

for entry in "${TESTS[@]}"; do
    name="${entry%%:*}"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo "  Running: $name"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    if "$BUILD_DIR/$name"; then
        echo "────────────────────────────────────────────────────────────────"
        echo "✅ $name: PASSED"
        echo ""
        PASSED=$((PASSED + 1))
    else
        echo "────────────────────────────────────────────────────────────────"
        echo "❌ $name: FAILED"
        echo ""
        FAILED=$((FAILED + 1))
    fi
done

echo "════════════════════════════════════════════════════════════════"
echo "  C++ Test Results"
echo "════════════════════════════════════════════════════════════════"
echo ""
echo "  ✅ Passed: $PASSED"
echo "  ❌ Failed: $FAILED"
echo "  📊 Total:  $((PASSED + FAILED))"
echo ""
if [ $FAILED -eq 0 ]; then
    echo "  🎉 All C++ tests passed!"
    exit 0
else
    echo "  ⚠️  Some tests failed"
    exit 1
fi
