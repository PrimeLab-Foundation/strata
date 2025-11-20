cmake_minimum_required(VERSION 3.20)
project(turbojsonpath LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Fetch external dependencies
include(FetchContent)

# jsoncons (header-only JSON library) - use URL to get just headers
FetchContent_Declare(
    jsoncons
    URL https://github.com/danielaparker/jsoncons/archive/refs/heads/master.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_GetProperties(jsoncons)
if(NOT jsoncons_POPULATED)
    FetchContent_Populate(jsoncons)
endif()

# simdjson (fast JSON parser)
FetchContent_Declare(
    simdjson
    GIT_REPOSITORY https://github.com/simdjson/simdjson.git
    GIT_TAG v3.10.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(simdjson)

# Warnings as errors in CI only
if(DEFINED ENV{CI})
  if(MSVC)
    add_compile_options(/W4 /WX)
  else()
    add_compile_options(-Wall -Wextra -Werror)
  endif()
endif()

# pybind11
set(PYBIND11_FINDPYTHON ON)
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/extern/pybind11/CMakeLists.txt)
  add_subdirectory(extern/pybind11 EXCLUDE_FROM_ALL)
else()
  find_package(pybind11 CONFIG REQUIRED)
endif()

# Prefer /cpp for sources; fallback to /src if /cpp is not present yet
set(TJP_CORE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cpp")
if (NOT EXISTS "${TJP_CORE_DIR}/scanner.cpp")
  set(TJP_CORE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
endif()

set(TJP_CORE_SOURCES
    ${TJP_CORE_DIR}/scanner.cpp
    ${TJP_CORE_DIR}/json_parser.cpp
    ${TJP_CORE_DIR}/jsonpath_compiler.cpp
    ${TJP_CORE_DIR}/jsonpath_executor.cpp
    ${TJP_CORE_DIR}/ndjson_stream.cpp
    ${TJP_CORE_DIR}/jp_compile.cpp
    ${TJP_CORE_DIR}/jp_eval_pushdown.cpp
)

add_library(tjp_core OBJECT ${TJP_CORE_SOURCES})

target_include_directories(tjp_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${pybind11_INCLUDE_DIRS}
    ${jsoncons_SOURCE_DIR}/include
    ${simdjson_SOURCE_DIR}/include
)

target_link_libraries(tjp_core PUBLIC simdjson)

# Build Python module named _tjp and install into turbojsonpath/
set(TJP_MOD_SRC ${TJP_CORE_DIR}/module_pybind.cpp)
pybind11_add_module(_tjp ${TJP_MOD_SRC} $<TARGET_OBJECTS:tjp_core>)

target_include_directories(_tjp PRIVATE
    include
    ${jsoncons_SOURCE_DIR}/include
    ${simdjson_SOURCE_DIR}/include
)

target_link_libraries(_tjp PRIVATE simdjson)

# Optimization flags (portable across compilers)
# - Use /O2 on MSVC
# - Prefer -O3 and try a native tuning flag when supported
#   * AppleClang: -mcpu=native (clang on macOS does not support -march=native)
#   * Clang/GCC:  -march=native if available
include(CheckCXXCompilerFlag)

if(MSVC)
  target_compile_options(tjp_core PRIVATE /O2 /DNDEBUG)
  target_compile_options(_tjp PRIVATE /O2 /DNDEBUG)
else()
  set(_TJP_COMMON_OPTS -O3 -DNDEBUG)
  set(_TJP_NATIVE_FLAG "")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    check_cxx_compiler_flag("-mcpu=native" HAS_MCPU_NATIVE)
    if(HAS_MCPU_NATIVE)
      set(_TJP_NATIVE_FLAG -mcpu=native)
    endif()
  else()
    check_cxx_compiler_flag("-march=native" HAS_MARCH_NATIVE)
    if(HAS_MARCH_NATIVE)
      set(_TJP_NATIVE_FLAG -march=native)
    endif()
  endif()

  if(_TJP_NATIVE_FLAG)
    target_compile_options(tjp_core PRIVATE ${_TJP_COMMON_OPTS} ${_TJP_NATIVE_FLAG})
    target_compile_options(_tjp PRIVATE ${_TJP_COMMON_OPTS} ${_TJP_NATIVE_FLAG})
  else()
    target_compile_options(tjp_core PRIVATE ${_TJP_COMMON_OPTS})
    target_compile_options(_tjp PRIVATE ${_TJP_COMMON_OPTS})
  endif()
endif()

install(TARGETS _tjp
        LIBRARY DESTINATION turbojsonpath
        ARCHIVE DESTINATION turbojsonpath)

# Pure-Python package is provided via src/ layout and packaged by scikit-build-core
# No need to install Python sources from CMake.

# Install public headers (for sdists)
install(DIRECTORY include/ DESTINATION turbojsonpath/include)
