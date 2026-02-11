cmake_minimum_required(VERSION 3.20)
project(strata LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(CheckIPOSupported)

option(STRATA_ENABLE_LTO "Enable LTO/IPO for performance builds" OFF)
option(STRATA_ENABLE_COVERAGE "Enable coverage instrumentation" OFF)

if(STRATA_ENABLE_COVERAGE)
    message(STATUS "Coverage instrumentation enabled")
    add_compile_options(-O0 -g -fprofile-instr-generate -fcoverage-mapping)
    add_link_options(-fprofile-instr-generate)
endif()

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
    src/strata/json/json_tape.cpp
    src/strata/json/json_cursor.cpp
    src/strata/json/json_lazy_cursor.cpp
    src/strata/json/json_document.cpp
    src/strata/json/json_mmap.cpp
    src/strata/json/json_serialize.cpp
    src/strata/json/ndjson_stream.cpp
    src/strata/json/parallel_ndjson.cpp
    src/strata/search/search_compile.cpp
    src/strata/search/search_eval.cpp
    src/strata/util/ryu_dtoa.cpp
    src/strata/util/simd_numbers.cpp
    src/strata/util/dragonbox.cpp
    src/strata/util/simd_escape.cpp
    src/strata/util/simd_newline.cpp
    src/strata/util/simd_utf8.cpp
    src/strata/util/simd_structural.cpp
    src/strata/util/fast_parse.cpp
    src/strata/util/thread_pool.cpp
)

# Include directories
set(STRATA_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(STRATA_CORE_LINK ${STRATA_CORE_SOURCES})
if(STRATA_ENABLE_COVERAGE)
    add_library(strata_core_objects OBJECT ${STRATA_CORE_SOURCES})
    target_include_directories(strata_core_objects PRIVATE ${STRATA_INCLUDE_DIRS})
    set(STRATA_CORE_LINK $<TARGET_OBJECTS:strata_core_objects>)
endif()

# ---------------------------------------------------------------------------
# 1) JSON Parse Tests
# ---------------------------------------------------------------------------
add_executable(json_parse_tests
    tests/cpp/test_json_parse.cpp
    ${STRATA_CORE_LINK}
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
    ${STRATA_CORE_LINK}
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
    ${STRATA_CORE_LINK}
)
target_include_directories(json_document_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(json_document_tests PRIVATE -Wall -Wextra)
strata_apply_lto(json_document_tests)
add_test(NAME json_document_tests COMMAND json_document_tests)

# ---------------------------------------------------------------------------
# 3a) JSON Mmap Tests (memory-mapped file parsing)
# ---------------------------------------------------------------------------
add_executable(json_mmap_tests
    tests/cpp/test_json_mmap.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(json_mmap_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(json_mmap_tests PRIVATE -Wall -Wextra)
strata_apply_lto(json_mmap_tests)
add_test(NAME json_mmap_tests COMMAND json_mmap_tests)

# ---------------------------------------------------------------------------
# 4) JSON Serialize Tests
# ---------------------------------------------------------------------------
add_executable(json_serialize_tests
    tests/cpp/test_json_serialize.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(json_serialize_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(json_serialize_tests PRIVATE -Wall -Wextra)
strata_apply_lto(json_serialize_tests)
add_test(NAME json_serialize_tests COMMAND json_serialize_tests)

# ---------------------------------------------------------------------------
# 5) Search Tests
# ---------------------------------------------------------------------------
add_executable(search_tests
    tests/cpp/test_search.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(search_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(search_tests PRIVATE -Wall -Wextra)
strata_apply_lto(search_tests)
add_test(NAME search_tests COMMAND search_tests)

# ---------------------------------------------------------------------------
# 6) NDJSON Tests
# ---------------------------------------------------------------------------
add_executable(ndjson_tests
    tests/cpp/test_ndjson.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(ndjson_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(ndjson_tests PRIVATE -Wall -Wextra)
strata_apply_lto(ndjson_tests)
add_test(NAME ndjson_tests COMMAND ndjson_tests)

# ---------------------------------------------------------------------------
# 6a) Parallel NDJSON Tests
# ---------------------------------------------------------------------------
add_executable(parallel_ndjson_tests
    tests/cpp/test_parallel_ndjson.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(parallel_ndjson_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(parallel_ndjson_tests PRIVATE -Wall -Wextra)
strata_apply_lto(parallel_ndjson_tests)
add_test(NAME parallel_ndjson_tests COMMAND parallel_ndjson_tests)

# ---------------------------------------------------------------------------
# 6b) Thread Pool Tests
# ---------------------------------------------------------------------------
add_executable(thread_pool_tests
    tests/cpp/test_thread_pool.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(thread_pool_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(thread_pool_tests PRIVATE -Wall -Wextra)
strata_apply_lto(thread_pool_tests)
add_test(NAME thread_pool_tests COMMAND thread_pool_tests)

# ---------------------------------------------------------------------------
# 7) Float Precision Tests (mirrors Python TestFloatPrecision)
# ---------------------------------------------------------------------------
add_executable(float_precision_tests
    tests/cpp/test_float_precision.cpp
    ${STRATA_CORE_LINK}
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
    ${STRATA_CORE_LINK}
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
    ${STRATA_CORE_LINK}
)
target_include_directories(stress_large_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(stress_large_tests PRIVATE -Wall -Wextra)
strata_apply_lto(stress_large_tests)
add_test(NAME stress_large_tests COMMAND stress_large_tests)

add_executable(stress_deep_tests
    tests/cpp/test_stress_deep.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(stress_deep_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(stress_deep_tests PRIVATE -Wall -Wextra)
strata_apply_lto(stress_deep_tests)
add_test(NAME stress_deep_tests COMMAND stress_deep_tests)

# ---------------------------------------------------------------------------
# 10) Fast Parse Tests (SWAR number parsing)
# ---------------------------------------------------------------------------
add_executable(fast_parse_tests
    tests/cpp/test_fast_parse.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(fast_parse_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(fast_parse_tests PRIVATE -Wall -Wextra)
strata_apply_lto(fast_parse_tests)
add_test(NAME fast_parse_tests COMMAND fast_parse_tests)

# ---------------------------------------------------------------------------
# 11) LazyString Tests (deferred unescape optimization)
# ---------------------------------------------------------------------------
add_executable(lazy_string_tests
    tests/cpp/test_lazy_string.cpp
)
target_include_directories(lazy_string_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(lazy_string_tests PRIVATE -Wall -Wextra)
strata_apply_lto(lazy_string_tests)
add_test(NAME lazy_string_tests COMMAND lazy_string_tests)

# ---------------------------------------------------------------------------
# 12) JSON Tape Tests (token tape format)
# ---------------------------------------------------------------------------
add_executable(json_tape_tests
    tests/cpp/test_json_tape.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(json_tape_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(json_tape_tests PRIVATE -Wall -Wextra)
strata_apply_lto(json_tape_tests)
add_test(NAME json_tape_tests COMMAND json_tape_tests)

# ---------------------------------------------------------------------------
# 13) Lazy Cursor Tests (selective materialization / lazy parsing)
# ---------------------------------------------------------------------------
add_executable(lazy_cursor_tests
    tests/cpp/test_lazy_cursor.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(lazy_cursor_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(lazy_cursor_tests PRIVATE -Wall -Wextra)
strata_apply_lto(lazy_cursor_tests)
add_test(NAME lazy_cursor_tests COMMAND lazy_cursor_tests)
# ---------------------------------------------------------------------------
# 14) Unicode and Escapes Tests (UTF-8 handling, escape sequences)
# ---------------------------------------------------------------------------
add_executable(unicode_escapes_tests
    tests/cpp/test_unicode_and_escapes.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(unicode_escapes_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(unicode_escapes_tests PRIVATE -Wall -Wextra)
strata_apply_lto(unicode_escapes_tests)
add_test(NAME unicode_escapes_tests COMMAND unicode_escapes_tests)
# ---------------------------------------------------------------------------
# 15) Parsing Errors Tests (error handling for invalid JSON)
# ---------------------------------------------------------------------------
add_executable(parsing_errors_tests
    tests/cpp/test_parsing_errors.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(parsing_errors_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(parsing_errors_tests PRIVATE -Wall -Wextra)
strata_apply_lto(parsing_errors_tests)
add_test(NAME parsing_errors_tests COMMAND parsing_errors_tests)
# ---------------------------------------------------------------------------
# 16) Integer Optimization Tests (integer parsing at various ranges)
# ---------------------------------------------------------------------------
add_executable(integer_optimization_tests
    tests/cpp/test_integer_optimization.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(integer_optimization_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(integer_optimization_tests PRIVATE -Wall -Wextra)
strata_apply_lto(integer_optimization_tests)
add_test(NAME integer_optimization_tests COMMAND integer_optimization_tests)
# ---------------------------------------------------------------------------
# 17) Search Advanced Tests (filter predicates, recursive descent, slicing)
# ---------------------------------------------------------------------------
add_executable(search_advanced_tests
    tests/cpp/test_search_advanced.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(search_advanced_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(search_advanced_tests PRIVATE -Wall -Wextra)
strata_apply_lto(search_advanced_tests)
add_test(NAME search_advanced_tests COMMAND search_advanced_tests)
# ---------------------------------------------------------------------------
# 18) SIMD Escape Tests (SIMD-accelerated JSON string escaping)
# ---------------------------------------------------------------------------
add_executable(simd_escape_tests
    tests/cpp/test_simd_escape.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(simd_escape_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(simd_escape_tests PRIVATE -Wall -Wextra)
strata_apply_lto(simd_escape_tests)
add_test(NAME simd_escape_tests COMMAND simd_escape_tests)

# ---------------------------------------------------------------------------
# 19) SIMD Newline Tests (SIMD-accelerated whitespace handling)
# ---------------------------------------------------------------------------
add_executable(simd_newline_tests
    tests/cpp/test_simd_newline.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(simd_newline_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(simd_newline_tests PRIVATE -Wall -Wextra)
strata_apply_lto(simd_newline_tests)
add_test(NAME simd_newline_tests COMMAND simd_newline_tests)

# ---------------------------------------------------------------------------
# 20) SIMD Structural Tests (SIMD structural character detection)
# ---------------------------------------------------------------------------
add_executable(simd_structural_tests
    tests/cpp/test_simd_structural.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(simd_structural_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(simd_structural_tests PRIVATE -Wall -Wextra)
strata_apply_lto(simd_structural_tests)
add_test(NAME simd_structural_tests COMMAND simd_structural_tests)
# ---------------------------------------------------------------------------
# Benchmarks
# ---------------------------------------------------------------------------
add_executable(parallel_ndjson_bench
    bench/parallel_ndjson_bench.cpp
    ${STRATA_CORE_LINK}
)
target_include_directories(parallel_ndjson_bench PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(parallel_ndjson_bench PRIVATE -Wall -Wextra -O3)
strata_apply_lto(parallel_ndjson_bench)

# ---------------------------------------------------------------------------
# Fuzz targets (libFuzzer + ASan + UBSan) — optional
# ---------------------------------------------------------------------------
option(FUZZ "Build libFuzzer fuzz targets (clang, -fsanitize=fuzzer,address,undefined)" OFF)
if(FUZZ)
    add_subdirectory(tests/fuzz)
endif()
