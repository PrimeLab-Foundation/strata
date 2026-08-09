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

# Core translation units linked into every suite. Empty until M1 introduces the
# value model; keep this list and the source list in setup.py in step.
set(STRATA_CORE_SOURCES)

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
