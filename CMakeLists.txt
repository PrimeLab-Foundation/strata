cmake_minimum_required(VERSION 3.20)
project(strata LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(CheckIPOSupported)

option(STRATA_ENABLE_LTO "Enable LTO/IPO for performance builds" OFF)

set(STRATA_LTO_ENABLED OFF)
if(STRATA_ENABLE_LTO)
    check_ipo_supported(RESULT STRATA_LTO_ENABLED OUTPUT STRATA_LTO_ERROR)
    if(NOT STRATA_LTO_ENABLED)
        message(FATAL_ERROR "STRATA_ENABLE_LTO=ON but IPO/LTO not supported: ${STRATA_LTO_ERROR}")
    endif()
    if(NOT CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo")
        message(WARNING "STRATA_ENABLE_LTO=ON with build type '${CMAKE_BUILD_TYPE}' (recommended: Release)")
    endif()
endif()

function(strata_apply_lto target_name)
    if(STRATA_LTO_ENABLED)
        set_property(TARGET ${target_name} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endfunction()

# ---------------------------------------------------------------------------
# 1) C++ UNIT TEST EXECUTABLE
# ---------------------------------------------------------------------------

enable_testing()

# ---------------------------------------------------------------------------
# C++ Unit Tests (using actual project structure)
# ---------------------------------------------------------------------------

# Core source files
set(STRATA_CORE_SOURCES
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
)

# Include directories
set(STRATA_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ---------------------------------------------------------------------------
# 1) JSON Parse Tests
# ---------------------------------------------------------------------------
add_executable(json_parse_tests
    tests/cpp/test_json_parse.cpp
    ${STRATA_CORE_SOURCES}
)
target_include_directories(json_parse_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(json_parse_tests PRIVATE -Wall -Wextra)
strata_apply_lto(json_parse_tests)
add_test(NAME json_parse_tests COMMAND json_parse_tests)

# ---------------------------------------------------------------------------
# 2) JSON Cursor Tests
# ---------------------------------------------------------------------------
add_executable(json_cursor_tests
    tests/cpp/test_json_cursor.cpp
    ${STRATA_CORE_SOURCES}
)
target_include_directories(json_cursor_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(json_cursor_tests PRIVATE -Wall -Wextra)
strata_apply_lto(json_cursor_tests)
add_test(NAME json_cursor_tests COMMAND json_cursor_tests)

# ---------------------------------------------------------------------------
# 3) JSON Document Tests
# ---------------------------------------------------------------------------
add_executable(json_document_tests
    tests/cpp/test_json_document.cpp
    ${STRATA_CORE_SOURCES}
)
target_include_directories(json_document_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(json_document_tests PRIVATE -Wall -Wextra)
strata_apply_lto(json_document_tests)
add_test(NAME json_document_tests COMMAND json_document_tests)

# ---------------------------------------------------------------------------
# 4) JSON Serialize Tests
# ---------------------------------------------------------------------------
add_executable(json_serialize_tests
    tests/cpp/test_json_serialize.cpp
    ${STRATA_CORE_SOURCES}
)
target_include_directories(json_serialize_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(json_serialize_tests PRIVATE -Wall -Wextra)
strata_apply_lto(json_serialize_tests)
add_test(NAME json_serialize_tests COMMAND json_serialize_tests)

# ---------------------------------------------------------------------------
# 5) JSONPath Tests
# ---------------------------------------------------------------------------
add_executable(jsonpath_tests
    tests/cpp/test_jsonpath.cpp
    ${STRATA_CORE_SOURCES}
)
target_include_directories(jsonpath_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(jsonpath_tests PRIVATE -Wall -Wextra)
strata_apply_lto(jsonpath_tests)
add_test(NAME jsonpath_tests COMMAND jsonpath_tests)

# ---------------------------------------------------------------------------
# 6) NDJSON Tests
# ---------------------------------------------------------------------------
add_executable(ndjson_tests
    tests/cpp/test_ndjson.cpp
    ${STRATA_CORE_SOURCES}
)
target_include_directories(ndjson_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(ndjson_tests PRIVATE -Wall -Wextra)
strata_apply_lto(ndjson_tests)
add_test(NAME ndjson_tests COMMAND ndjson_tests)

# ---------------------------------------------------------------------------
# 7) Float Precision Tests (mirrors Python TestFloatPrecision)
# ---------------------------------------------------------------------------
add_executable(float_precision_tests
    tests/cpp/test_float_precision.cpp
    ${STRATA_CORE_SOURCES}
)
target_include_directories(float_precision_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(float_precision_tests PRIVATE -Wall -Wextra)
strata_apply_lto(float_precision_tests)
add_test(NAME float_precision_tests COMMAND float_precision_tests)

# ---------------------------------------------------------------------------
# 8) Output Buffer + SIMD String Tests
# ---------------------------------------------------------------------------
add_executable(output_buffer_tests
    tests/cpp/test_output_buffer.cpp
    ${STRATA_CORE_SOURCES}
)
target_include_directories(output_buffer_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(output_buffer_tests PRIVATE -Wall -Wextra)
strata_apply_lto(output_buffer_tests)
add_test(NAME output_buffer_tests COMMAND output_buffer_tests)

# ---------------------------------------------------------------------------
# 9) Stress Tests (large payloads, deep nesting)
# ---------------------------------------------------------------------------
add_executable(stress_large_tests
    tests/cpp/test_stress_large.cpp
    ${STRATA_CORE_SOURCES}
)
target_include_directories(stress_large_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(stress_large_tests PRIVATE -Wall -Wextra)
strata_apply_lto(stress_large_tests)
add_test(NAME stress_large_tests COMMAND stress_large_tests)

add_executable(stress_deep_tests
    tests/cpp/test_stress_deep.cpp
    ${STRATA_CORE_SOURCES}
)
target_include_directories(stress_deep_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(stress_deep_tests PRIVATE -Wall -Wextra)
strata_apply_lto(stress_deep_tests)
add_test(NAME stress_deep_tests COMMAND stress_deep_tests)

# ---------------------------------------------------------------------------
# 10) SIMD Structural Indexer Tests
# ---------------------------------------------------------------------------
set(STRATA_SIMD_SOURCES
    src/strata/simd/classifier.cpp
    src/strata/simd/structural_indexer.cpp
    src/strata/simd/index_builder.cpp
    src/strata/simd/dispatch.cpp
)

add_executable(simd_tests
    tests/cpp/test_simd.cpp
    ${STRATA_SIMD_SOURCES}
)
target_include_directories(simd_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(simd_tests PRIVATE -Wall -Wextra)
strata_apply_lto(simd_tests)
add_test(NAME simd_tests COMMAND simd_tests)

# ---------------------------------------------------------------------------
# 11) SIMD Benchmark (not a test — build only, run manually)
# ---------------------------------------------------------------------------
add_executable(bench_simd
    tests/cpp/bench_simd.cpp
    ${STRATA_SIMD_SOURCES}
)
target_include_directories(bench_simd PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(bench_simd PRIVATE -Wall -Wextra -O3)
strata_apply_lto(bench_simd)

# ---------------------------------------------------------------------------
# 12) Speculative Parsing Engine Tests
# ---------------------------------------------------------------------------
set(STRATA_SPECULATIVE_SOURCES
    src/strata/speculative/transition_model.cpp
    src/strata/speculative/fast_paths.cpp
    src/strata/speculative/parser.cpp
)

add_executable(speculative_tests
    tests/cpp/test_speculative.cpp
    ${STRATA_SPECULATIVE_SOURCES}
    ${STRATA_SIMD_SOURCES}
)
target_include_directories(speculative_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(speculative_tests PRIVATE -Wall -Wextra)
strata_apply_lto(speculative_tests)
add_test(NAME speculative_tests COMMAND speculative_tests)

# ---------------------------------------------------------------------------
# 13) Speculative Parsing Benchmark (build only, run manually)
# ---------------------------------------------------------------------------
add_executable(bench_speculative
    tests/cpp/bench_speculative.cpp
    ${STRATA_SPECULATIVE_SOURCES}
    ${STRATA_SIMD_SOURCES}
)
target_include_directories(bench_speculative PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(bench_speculative PRIVATE -Wall -Wextra -O3)
strata_apply_lto(bench_speculative)

# ---------------------------------------------------------------------------
# Fuzz targets (libFuzzer + ASan + UBSan) — optional
# ---------------------------------------------------------------------------
option(FUZZ "Build libFuzzer fuzz targets (clang, -fsanitize=fuzzer,address,undefined)" OFF)
if(FUZZ)
    add_subdirectory(tests/fuzz)
endif()
