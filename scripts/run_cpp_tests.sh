#!/usr/bin/env bash
# Run C++ tests without CMake (Rule 16: automation in scripts/).
# Canonical interface: make test-cpp
set -e

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

BUILD_DIR="${BUILD_CPP_TESTS_DIR:-build_cpp_tests}"
mkdir -p "$BUILD_DIR"

CXX="${CXX:-clang++}"
CXXFLAGS="-std=c++20 -Wall -Wextra -O2"
INCLUDES="-Iinclude -Isrc"

CORE_SOURCES="
    src/strata/json/json_parse.cpp
    src/strata/json/json_cursor.cpp
    src/strata/json/json_document.cpp
    src/strata/json/json_serialize.cpp
    src/strata/json/ndjson_stream.cpp
    src/strata/search/jsonpath_compile.cpp
    src/strata/search/jsonpath_eval.cpp
    src/strata/util/ryu_dtoa.cpp
    src/strata/util/dragonbox.cpp
    src/strata/util/simd_escape.cpp
    src/strata/util/simd_newline.cpp
    src/strata/util/simd_utf8.cpp
    src/strata/util/fast_parse.cpp
    src/strata/simd/classifier.cpp
    src/strata/simd/structural_indexer.cpp
    src/strata/simd/index_builder.cpp
    src/strata/simd/dispatch.cpp
    src/strata/speculative/transition_model.cpp
    src/strata/speculative/fast_paths.cpp
    src/strata/speculative/parser.cpp
    src/strata/bloom/bloom_filter.cpp
    src/strata/bloom/key_filter.cpp
    src/strata/bloom/schema_filter.cpp
    src/strata/bloom/dedup_filter.cpp
    src/strata/bloom/parser_integration.cpp
"

PASSED=0
FAILED=0

run_test() {
    local test_name=$1
    local test_file=$2

    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo "  Building: $test_name"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

    if $CXX $CXXFLAGS $INCLUDES $test_file $CORE_SOURCES -o "$BUILD_DIR/$test_name" 2>&1; then
        echo "✅ Build successful"
        echo ""
        echo "Running tests..."
        echo "────────────────────────────────────────────────────────────────"
        if "$BUILD_DIR/$test_name"; then
            echo "────────────────────────────────────────────────────────────────"
            echo "✅ $test_name: PASSED"
            echo ""
            PASSED=$((PASSED + 1))
        else
            echo "────────────────────────────────────────────────────────────────"
            echo "❌ $test_name: FAILED"
            echo ""
            FAILED=$((FAILED + 1))
        fi
    else
        echo "❌ Build failed for $test_name"
        echo ""
        FAILED=$((FAILED + 1))
    fi
}

echo "════════════════════════════════════════════════════════════════"
echo "  Building & Running C++ Tests"
echo "════════════════════════════════════════════════════════════════"
echo ""

run_test "json_parse_tests" "tests/cpp/test_json_parse.cpp"
run_test "json_cursor_tests" "tests/cpp/test_json_cursor.cpp"
run_test "json_document_tests" "tests/cpp/test_json_document.cpp"
run_test "json_serialize_tests" "tests/cpp/test_json_serialize.cpp"
run_test "jsonpath_tests" "tests/cpp/test_jsonpath.cpp"
run_test "ndjson_tests" "tests/cpp/test_ndjson.cpp"
run_test "float_precision_tests" "tests/cpp/test_float_precision.cpp"
run_test "output_buffer_tests" "tests/cpp/test_output_buffer.cpp"
run_test "stress_large_tests" "tests/cpp/test_stress_large.cpp"
run_test "stress_deep_tests" "tests/cpp/test_stress_deep.cpp"

# SIMD structural indexer tests — uses its own source set (not CORE_SOURCES)
SIMD_SOURCES="
    src/strata/simd/classifier.cpp
    src/strata/simd/structural_indexer.cpp
    src/strata/simd/index_builder.cpp
    src/strata/simd/dispatch.cpp
"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "  Building: simd_tests"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
if $CXX $CXXFLAGS $INCLUDES tests/cpp/test_simd.cpp $SIMD_SOURCES -o "$BUILD_DIR/simd_tests" 2>&1; then
    echo "✅ Build successful"
    echo ""
    echo "Running tests..."
    echo "────────────────────────────────────────────────────────────────"
    if "$BUILD_DIR/simd_tests"; then
        echo "────────────────────────────────────────────────────────────────"
        echo "✅ simd_tests: PASSED"
        echo ""
        PASSED=$((PASSED + 1))
    else
        echo "────────────────────────────────────────────────────────────────"
        echo "❌ simd_tests: FAILED"
        echo ""
        FAILED=$((FAILED + 1))
    fi
else
    echo "❌ Build failed for simd_tests"
    echo ""
    FAILED=$((FAILED + 1))
fi

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
