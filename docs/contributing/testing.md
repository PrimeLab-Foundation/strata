# Testing Guide

Strata uses assert-based C++ tests and pytest for Python. This document
explains how to run tests, add new ones, measure coverage, and run the
fuzzer.

______________________________________________________________________

## 1. Test Framework

### C++ tests

C++ tests use a lightweight assert-based framework. There is no external test
library dependency (no Google Test, no Catch2). Each test file is a
standalone `main()` that calls test functions and uses `assert()` for
verification.

Common assertion patterns:

```cpp
#include <cassert>
#include <cmath>
#include <iostream>

// Boolean assertion
assert(result.ok());

// Equality assertion
assert(result.value.as_number() == 42.0);

// Approximate floating-point equality
assert(std::abs(result.value.as_number() - 3.14) < 0.001);

// String equality
assert(result.value.as_string() == "hello");
```

Each test function prints a pass message on success:

```cpp
void test_parse_null() {
    auto result = parse_json("null");
    assert(result.ok());
    assert(result.value.is_null());
    std::cout << "test_parse_null passed\n";
}
```

### Python tests

Python tests use pytest and live in `tests/py/` and `tests/unit/`. Run them
via:

```bash
make test-py
# or directly:
.venv/bin/pytest tests/py/ -q
```

______________________________________________________________________

## 2. Running Tests

### All tests (Python + C++)

```bash
make test
```

This runs `test-py` then `test-cpp` and reports overall status.

### C++ tests only

```bash
make test-cpp
```

This invokes `scripts/run_cpp_tests.sh`, which compiles and runs each test
binary independently.

### CMake-based C++ tests

```bash
make cpp-build    # cmake + build
make cpp-test     # ctest
# or equivalently:
make test-cpp-cmake
```

### Individual C++ test binary

To run a single test in isolation:

```bash
cd /path/to/strata
CXX=clang++ scripts/run_cpp_tests.sh
# Or compile manually:
clang++ -std=c++20 -Wall -Wextra -O2 \
    -Iinclude -Isrc \
    tests/cpp/test_json_parse.cpp \
    src/strata/json/json_parse.cpp \
    src/strata/json/json_cursor.cpp \
    src/strata/json/json_document.cpp \
    src/strata/json/json_serialize.cpp \
    src/strata/json/ndjson_stream.cpp \
    src/strata/search/jsonpath_compile.cpp \
    src/strata/search/jsonpath_eval.cpp \
    src/strata/util/ryu_dtoa.cpp \
    src/strata/util/dragonbox.cpp \
    src/strata/util/simd_escape.cpp \
    src/strata/util/simd_newline.cpp \
    src/strata/util/simd_utf8.cpp \
    src/strata/util/fast_parse.cpp \
    src/strata/simd/classifier.cpp \
    src/strata/simd/structural_indexer.cpp \
    src/strata/simd/index_builder.cpp \
    src/strata/simd/dispatch.cpp \
    src/strata/speculative/transition_model.cpp \
    src/strata/speculative/fast_paths.cpp \
    src/strata/speculative/parser.cpp \
    src/strata/bloom/bloom_filter.cpp \
    src/strata/bloom/key_filter.cpp \
    src/strata/bloom/schema_filter.cpp \
    src/strata/bloom/dedup_filter.cpp \
    src/strata/bloom/parser_integration.cpp \
    -o build_cpp_tests/json_parse_tests
./build_cpp_tests/json_parse_tests
```

______________________________________________________________________

## 3. Adding a New C++ Test File

### Step 1: Create the test file

Create `tests/cpp/test_my_feature.cpp`:

```cpp
/**
 * test_my_feature.cpp - Tests for MyFeature
 */

#include "strata/my_feature/my_header.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

void test_basic_functionality() {
    // Arrange
    auto input = make_test_input();

    // Act
    auto result = my_function(input);

    // Assert
    assert(result.ok());
    assert(result.value == expected_value);
    std::cout << "test_basic_functionality passed\n";
}

void test_edge_case_empty() {
    auto result = my_function("");
    assert(!result.ok());
    std::cout << "test_edge_case_empty passed\n";
}

int main() {
    test_basic_functionality();
    test_edge_case_empty();

    std::cout << "\nAll my_feature tests passed!\n";
    return 0;
}
```

### Step 2: Register in run_cpp_tests.sh

Add a `run_test` call to `scripts/run_cpp_tests.sh`:

```bash
run_test "my_feature_tests" "tests/cpp/test_my_feature.cpp"
```

Choose the appropriate runner function:

| Runner          | Source set                           | Use when                     |
| --------------- | ------------------------------------ | ---------------------------- |
| `run_test`      | `CORE_SOURCES` (all library sources) | Tests using the full library |
| `run_simd_test` | `SIMD_SOURCES` only                  | Pure SIMD tests              |
| `run_spec_test` | `SPECULATIVE + SIMD + BLOOM` sources | Speculative parser tests     |

### Step 3: Register in CMakeLists.txt

Add the test target to the root `CMakeLists.txt`:

```cmake
# ---------------------------------------------------------------------------
# N) My Feature Tests
# ---------------------------------------------------------------------------
add_executable(my_feature_tests
    tests/cpp/test_my_feature.cpp
    ${STRATA_CORE_SOURCES}
)
target_include_directories(my_feature_tests PRIVATE ${STRATA_INCLUDE_DIRS})
target_compile_options(my_feature_tests PRIVATE -Wall -Wextra)
strata_apply_lto(my_feature_tests)
add_test(NAME my_feature_tests COMMAND my_feature_tests)
```

### Step 4: Verify

```bash
make test-cpp          # run_cpp_tests.sh path
make cpp-test          # CMake path
```

______________________________________________________________________

## 4. Existing Test Suites

| Test binary              | File                                  | What it tests               |
| ------------------------ | ------------------------------------- | --------------------------- |
| `json_parse_tests`       | `tests/cpp/test_json_parse.cpp`       | Core JSON parsing           |
| `json_cursor_tests`      | `tests/cpp/test_json_cursor.cpp`      | Cursor/iterator API         |
| `json_document_tests`    | `tests/cpp/test_json_document.cpp`    | DOM document model          |
| `json_serialize_tests`   | `tests/cpp/test_json_serialize.cpp`   | Serialization / dumps       |
| `jsonpath_tests`         | `tests/cpp/test_jsonpath.cpp`         | JSONPath queries            |
| `ndjson_tests`           | `tests/cpp/test_ndjson.cpp`           | NDJSON streaming            |
| `float_precision_tests`  | `tests/cpp/test_float_precision.cpp`  | Float round-trip accuracy   |
| `output_buffer_tests`    | `tests/cpp/test_output_buffer.cpp`    | Output buffer management    |
| `stress_large_tests`     | `tests/cpp/test_stress_large.cpp`     | Large document stress tests |
| `stress_deep_tests`      | `tests/cpp/test_stress_deep.cpp`      | Deep nesting stress tests   |
| `simd_tests`             | `tests/cpp/test_simd.cpp`             | SIMD classifier / indexer   |
| `simd_edge_tests`        | `tests/cpp/test_simd_edge.cpp`        | SIMD edge cases             |
| `speculative_tests`      | `tests/cpp/test_speculative.cpp`      | Speculative parser          |
| `speculative_edge_tests` | `tests/cpp/test_speculative_edge.cpp` | Speculative edge cases      |
| `bloom_tests`            | `tests/cpp/test_bloom.cpp`            | Bloom filter operations     |
| `bloom_edge_tests`       | `tests/cpp/test_bloom_edge.cpp`       | Bloom filter edge cases     |
| `integration_tests`      | `tests/cpp/test_integration.cpp`      | End-to-end integration      |

______________________________________________________________________

## 5. Coverage Measurement

### C++ coverage

```bash
make coverage-cpp
```

This compiles each test binary with `-fprofile-instr-generate -fcoverage-mapping` (Clang), runs them to collect `.profraw` files, merges
with `llvm-profdata`, and generates a report with `llvm-cov`. Results go to
`build_coverage/`.

### Python coverage

```bash
make coverage-py
```

Uses pytest-cov to measure coverage of `tests/unit/` against the `strata`
package. HTML report goes to `build_coverage/htmlcov/`.

### Coverage gate

```bash
make coverage-gate
```

Checks that C++ coverage meets the 95% threshold. This is a gating check
for CI -- builds that drop below 95% coverage are rejected.

### Full coverage report

```bash
make coverage        # runs both C++ and Python coverage
```

______________________________________________________________________

## 6. Fuzzing

Strata includes two fuzz targets using libFuzzer:

- `tests/fuzz/fuzz_loads.cpp` -- fuzzes the `loads()` entry point with
  arbitrary byte sequences.
- `tests/fuzz/fuzz_ndjson.cpp` -- fuzzes the NDJSON streaming parser.

### Building fuzz targets

```bash
make fuzz-build
```

This calls `scripts/fuzz.sh build`, which compiles with `-fsanitize=fuzzer`
and address sanitizer.

### Running the fuzzer

```bash
make fuzz
# or with custom settings:
FUZZ_TIME=300 FUZZ_ARTIFACT_PREFIX=fuzz_crashes/ make fuzz-run
```

Environment variables:

| Variable               | Default         | Description                |
| ---------------------- | --------------- | -------------------------- |
| `FUZZ_TIME`            | 120             | Seconds to run each target |
| `FUZZ_ARTIFACT_PREFIX` | `fuzz_crashes/` | Where to save crash inputs |

### Writing a new fuzz target

Create `tests/fuzz/fuzz_my_feature.cpp`:

```cpp
#include "strata/my_feature/my_header.hpp"
#include <cstdint>
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Call the function under test with arbitrary input.
    // The fuzzer will try to find inputs that crash, hang, or trigger
    // sanitizer errors.
    my_function(data, size);
    return 0;
}
```

Add it to `tests/fuzz/CMakeLists.txt` and `scripts/fuzz.sh`.

______________________________________________________________________

## 7. Gate: Full Validation

The `make gate` target runs the complete validation pipeline required before
merging:

```bash
make gate
```

Steps:

1. C++ tests (`make test-cpp`)
1. Build extension (`pip install -e .`)
1. Python tests (`make test-py`)
1. C++ coverage collection
1. Python coverage collection

All steps must pass. This enforces Rules 14 and 15 of the project
development rules.
