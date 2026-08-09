# libFuzzer targets. Enabled from the top-level CMakeLists with -DFUZZ=ON.
#
# STRATA_CORE_SOURCES is inherited from the parent scope, so these binaries are
# built from the same manifest as the test suites and the extension. The
# previous implementation kept a fourth copy of the source list here, and it
# drifted from the other three.

set(STRATA_FUZZ_FLAGS -fsanitize=fuzzer,address,undefined -fno-omit-frame-pointer -g)

# Apple's clang ships no libFuzzer runtime, so -fsanitize=fuzzer compiles but
# fails at link with an obscure "libclang_rt.fuzzer_osx.a not found". Probe for
# it here and say what to do instead.
include(CheckCXXSourceCompiles)
set(CMAKE_REQUIRED_FLAGS "-fsanitize=fuzzer")
check_cxx_source_compiles(
    "#include <cstddef>
     #include <cstdint>
     extern \"C\" int LLVMFuzzerTestOneInput(const uint8_t*, size_t) { return 0; }"
    STRATA_HAVE_LIBFUZZER)
unset(CMAKE_REQUIRED_FLAGS)

if(NOT STRATA_HAVE_LIBFUZZER)
    message(FATAL_ERROR
            "-DFUZZ=ON needs a toolchain that provides the libFuzzer runtime, and "
            "${CMAKE_CXX_COMPILER} does not. Apple's clang never ships it; install LLVM "
            "(brew install llvm) and reconfigure with "
            "-DCMAKE_CXX_COMPILER=$(brew --prefix llvm)/bin/clang++.")
endif()

foreach(target fuzz_loads fuzz_ndjson)
    add_executable(${target} ${target}.cpp ${STRATA_CORE_SOURCES})
    target_include_directories(${target} PRIVATE ${STRATA_INCLUDE_DIRS})
    target_compile_options(${target} PRIVATE ${STRATA_FUZZ_FLAGS})
    target_link_options(${target} PRIVATE ${STRATA_FUZZ_FLAGS})
endforeach()
