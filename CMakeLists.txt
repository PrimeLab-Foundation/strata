cmake_minimum_required(VERSION 3.20)
project(strata LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ---------------------------------------------------------------------------
# 1) C++ UNIT TEST EXECUTABLE
# ---------------------------------------------------------------------------

enable_testing()

# Test executable for dummy.{hpp,cpp}
add_executable(dummy_tests
    strata/cpp/dummy/test_dummy.cpp
    strata/cpp/dummy/dummy.cpp
)

# Make sure dummy.hpp is visible as #include "dummy.hpp"
target_include_directories(dummy_tests PRIVATE
    strata/cpp/dummy
)

add_test(NAME dummy_tests COMMAND dummy_tests)

# ---------------------------------------------------------------------------
# 2) OPTIONAL: build the pybind11 module via CMake (not used by pip/setup.py)
# ---------------------------------------------------------------------------
# This is purely for local experimentation; your real Python wheel is built
# via setup.py / pyproject.toml, not via this CMakeLists.

find_package(pybind11 CONFIG QUIET)

if(pybind11_FOUND)
    message(STATUS "pybind11 found, building CMake pybind module '_strata'")

    pybind11_add_module(_strata
        strata/cpp/module_pybind.cpp
        strata/cpp/dummy/dummy.cpp
    )

    # Ensure dummy.hpp is visible to module_pybind.cpp
    target_include_directories(_strata PRIVATE
        strata/cpp/dummy
    )
else()
    message(STATUS "pybind11 not found, skipping CMake pybind module '_strata'")
endif()
