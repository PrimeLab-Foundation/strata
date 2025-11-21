cmake_minimum_required(VERSION 3.20)
project(strata LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ---------------------------------------------------------------------------
# 1) C++ UNIT TEST EXECUTABLE
# ---------------------------------------------------------------------------

enable_testing()

# ---------------------------------------------------------------------------
# 1) dummy tests
# ---------------------------------------------------------------------------
# Test executable for dummy.{hpp,cpp}
add_executable(dummy_tests
    strata/cpp/tests/test_dummy.cpp
    strata/cpp/dummy/dummy.cpp
)
# Make sure dummy.hpp is visible as #include "dummy.hpp"
target_include_directories(dummy_tests PRIVATE
    strata/cpp/dummy
)

add_test(NAME dummy_tests COMMAND dummy_tests)

# ---------------------------------------------------------------------------
# 2) json_cursor tests (no simdjson)
# ---------------------------------------------------------------------------
add_executable(json_cursor_tests
    strata/cpp/tests/test_json_cursor.cpp
    strata/cpp/json_cursor.cpp
    strata/cpp/json_core.hpp        # headers are optional here but harmless
)

target_include_directories(json_cursor_tests PRIVATE
    strata/cpp
)

add_test(NAME json_cursor_tests COMMAND json_cursor_tests)

# ---------------------------------------------------------------------------
# 3) json_document tests
# ---------------------------------------------------------------------------
add_executable(json_document_tests
    strata/cpp/tests/test_json_document.cpp
    strata/cpp/json_document.cpp
    strata/cpp/json_cursor.cpp
    strata/cpp/json_parse.cpp
)

target_include_directories(json_document_tests PRIVATE
    strata/cpp
)

add_test(NAME json_document_tests COMMAND json_document_tests)
