# Fuzz targets (libFuzzer + ASan + UBSan)
# Enable with: cmake -B build_fuzz -S . -DFUZZ=ON
# Build: cmake --build build_fuzz --target fuzz_loads
# Run: ./build_fuzz/fuzz_loads tests/fuzz/corpus -max_total_time=60

# Pass each flag as separate list element so -fsanitize= does not swallow the rest
set(FUZZ_FLAGS "-fsanitize=fuzzer,address,undefined" "-fno-omit-frame-pointer" "-g")
set(FUZZ_LINK_FLAGS "-fsanitize=fuzzer,address,undefined")

# On macOS, Homebrew LLVM's libFuzzer runtime was built against LLVM's libc++.
# Link against that libc++ so __hash_memory and other symbols resolve (avoids
# undefined symbol errors when the driver would otherwise pick Apple's libc++).
if(APPLE AND CMAKE_CXX_COMPILER MATCHES "Cellar/llvm|opt/llvm")
  get_filename_component(LLVM_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
  get_filename_component(LLVM_PREFIX "${LLVM_BIN_DIR}" DIRECTORY)
  set(LLVM_LIB_CXX "${LLVM_PREFIX}/lib/c++")
  if(EXISTS "${LLVM_LIB_CXX}")
    list(APPEND FUZZ_LINK_FLAGS "-L${LLVM_LIB_CXX}" "-Wl,-rpath,${LLVM_LIB_CXX}")
  endif()
endif()

# Core sources with absolute path (we are in tests/fuzz/)
set(FUZZ_CORE_SOURCES
    ${CMAKE_SOURCE_DIR}/src/strata/json/json_parse.cpp
    ${CMAKE_SOURCE_DIR}/src/strata/json/json_cursor.cpp
    ${CMAKE_SOURCE_DIR}/src/strata/json/json_document.cpp
    ${CMAKE_SOURCE_DIR}/src/strata/json/json_serialize.cpp
    ${CMAKE_SOURCE_DIR}/src/strata/json/ndjson_stream.cpp
    ${CMAKE_SOURCE_DIR}/src/strata/search/jsonpath_compile.cpp
    ${CMAKE_SOURCE_DIR}/src/strata/search/jsonpath_eval.cpp
    ${CMAKE_SOURCE_DIR}/src/strata/util/ryu_dtoa.cpp
    ${CMAKE_SOURCE_DIR}/src/strata/util/dragonbox.cpp
    ${CMAKE_SOURCE_DIR}/src/strata/util/simd_escape.cpp
    ${CMAKE_SOURCE_DIR}/src/strata/util/simd_newline.cpp
    ${CMAKE_SOURCE_DIR}/src/strata/util/simd_utf8.cpp
    ${CMAKE_SOURCE_DIR}/src/strata/util/fast_parse.cpp
)

set(FUZZ_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)

# fuzz_loads: parse_json (loads)
add_executable(fuzz_loads fuzz_loads.cpp ${FUZZ_CORE_SOURCES})
target_include_directories(fuzz_loads PRIVATE ${FUZZ_INCLUDE_DIRS})
target_compile_options(fuzz_loads PRIVATE ${FUZZ_FLAGS})
target_link_options(fuzz_loads PRIVATE ${FUZZ_LINK_FLAGS})

# fuzz_ndjson: NdjsonStream (iter_ndjson)
add_executable(fuzz_ndjson fuzz_ndjson.cpp ${FUZZ_CORE_SOURCES})
target_include_directories(fuzz_ndjson PRIVATE ${FUZZ_INCLUDE_DIRS})
target_compile_options(fuzz_ndjson PRIVATE ${FUZZ_FLAGS})
target_link_options(fuzz_ndjson PRIVATE ${FUZZ_LINK_FLAGS})
