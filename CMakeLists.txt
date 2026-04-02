# CMakeLists.txt — for CLion code intelligence only.
# Actual build system: make build
cmake_minimum_required(VERSION 3.25)
project(strata LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(cpp/include)

file(GLOB_RECURSE SOURCES cpp/src/*.cpp)
file(GLOB_RECURSE HEADERS cpp/include/*.hpp)
file(GLOB_RECURSE TESTS cpp/tests/*.cpp)

add_library(strata_lib ${SOURCES} ${HEADERS})
