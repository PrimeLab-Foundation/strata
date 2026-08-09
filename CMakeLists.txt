cmake_minimum_required(VERSION 3.20)
project(strata LANGUAGES CXX)

# ---------------------------------------------------------------------------
# The single C++ test registry.
#
# `make test-cpp`, the pre-build gate in setup.py, coverage and CI all drive
# ctest from this file — adding a suite means adding one
# strata_add_cpp_test() line here and nowhere else
# (docs/build-and-test/SKILL.md, "C++ tests — target: ONE harness").
# ---------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# An unset build type produces unoptimized binaries, which made the previous
# implementation's ctest and shell harnesses disagree about stress-test runtime.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

enable_testing()

# ---------------------------------------------------------------------------
# Coverage — opt-in, applied to the same registry the ordinary suites use.
#
#   cmake -S . -B build/coverage -DCOVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
#
# Deliberately NOT a second source list: the previous implementation's coverage
# build kept its own copy, which drifted until it named deleted files and
# measured nothing (docs/build-and-test/SKILL.md, known-broken item 1).
# ---------------------------------------------------------------------------
option(COVERAGE "Instrument the test suites for llvm-cov" OFF)
if(COVERAGE)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR
                "-DCOVERAGE=ON needs Clang (source-based coverage); this is "
                "${CMAKE_CXX_COMPILER_ID}. Configure with "
                "-DCMAKE_CXX_COMPILER=clang++.")
    endif()
    add_compile_options(-fprofile-instr-generate -fcoverage-mapping -O0 -g)
    add_link_options(-fprofile-instr-generate -fcoverage-mapping)
endif()

# Core translation units linked into every suite. The list itself lives in one
# shared manifest that setup.py reads too, so the test binaries and the shipped
# extension cannot be built from different sources.
set(STRATA_CORE_MANIFEST ${CMAKE_CURRENT_SOURCE_DIR}/src/strata/core_sources.txt)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${STRATA_CORE_MANIFEST})

file(STRINGS ${STRATA_CORE_MANIFEST} STRATA_CORE_MANIFEST_LINES)
set(STRATA_CORE_SOURCES "")
foreach(entry IN LISTS STRATA_CORE_MANIFEST_LINES)
    string(STRIP "${entry}" entry)
    if(entry STREQUAL "" OR entry MATCHES "^#")
        continue()
    endif()
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${entry})
        message(FATAL_ERROR "core_sources.txt lists a file that does not exist: ${entry}")
    endif()
    list(APPEND STRATA_CORE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${entry})
endforeach()

set(STRATA_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_property(GLOBAL PROPERTY STRATA_REGISTERED_TEST_SOURCES "")

# strata_add_cpp_test(<target> <source>...)
#
# Registers one assert-based suite (plain assert + main(), no framework).
# NDEBUG is undefined explicitly: without it a Release configure would compile
# every assertion out and the suite would pass vacuously. Tests run from the
# source directory so a suite behaves the same under ctest as by hand.
function(strata_add_cpp_test target)
    add_executable(${target} ${ARGN} ${STRATA_CORE_SOURCES})
    target_include_directories(${target} PRIVATE ${STRATA_INCLUDE_DIRS})
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /UNDEBUG)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -UNDEBUG)
    endif()
    add_test(NAME ${target} COMMAND ${target}
             WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    foreach(source IN LISTS ARGN)
        set_property(GLOBAL APPEND PROPERTY STRATA_REGISTERED_TEST_SOURCES ${source})
    endforeach()
endfunction()

strata_add_cpp_test(smoke_tests tests/cpp/test_smoke.cpp)
strata_add_cpp_test(json_core_tests tests/cpp/test_json_core.cpp)
strata_add_cpp_test(json_parse_tests tests/cpp/test_json_parse.cpp)
strata_add_cpp_test(json_serialize_tests tests/cpp/test_json_serialize.cpp)
strata_add_cpp_test(float_precision_tests tests/cpp/test_float_precision.cpp)
strata_add_cpp_test(json_cursor_tests tests/cpp/test_json_cursor.cpp)
strata_add_cpp_test(ndjson_tests tests/cpp/test_ndjson.cpp)
strata_add_cpp_test(jsonpath_tests tests/cpp/test_jsonpath.cpp)
strata_add_cpp_test(jsonpath_stream_tests tests/cpp/test_jsonpath_stream.cpp)
strata_add_cpp_test(scan_tests tests/cpp/test_scan.cpp)
strata_add_cpp_test(folder_tests tests/cpp/test_folder.cpp)
strata_add_cpp_test(fuzz_corpus_tests tests/cpp/test_fuzz_corpus.cpp)
strata_add_cpp_test(stress_deep_tests tests/cpp/test_stress_deep.cpp)

# The registry is only "the single registry" if nothing can sit outside it: a
# suite added to tests/cpp/ but never registered would be silently uncompiled,
# unrun and unreported. Configure fails instead. The drivers always reconfigure,
# so a newly added file is seen on the next `make test-cpp`.
file(GLOB STRATA_TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} tests/cpp/test_*.cpp)
get_property(STRATA_REGISTERED GLOBAL PROPERTY STRATA_REGISTERED_TEST_SOURCES)
foreach(source IN LISTS STRATA_TEST_SOURCES)
    if(NOT source IN_LIST STRATA_REGISTERED)
        message(FATAL_ERROR
                "${source} is not registered: add a strata_add_cpp_test() line for it "
                "in CMakeLists.txt, the single C++ test registry.")
    endif()
endforeach()

# ---------------------------------------------------------------------------
# Fuzz targets — opt-in, built with libFuzzer + ASan + UBSan.
#
#   cmake -S . -B build/fuzz -DFUZZ=ON -DCMAKE_CXX_COMPILER=clang++
#
# Seeds live in tests/fuzz/corpus/{loads,ndjson} and are committed — the
# previous implementation documented that directory but never added it to git,
# so libFuzzer failed at startup on every scheduled run. fuzz_corpus_tests
# above replays those seeds on every `make test`, which keeps them honest on
# toolchains that have no libFuzzer runtime at all.
# ---------------------------------------------------------------------------
option(FUZZ "Build libFuzzer fuzz targets (clang only)" OFF)
if(FUZZ)
    add_subdirectory(tests/fuzz)
endif()
