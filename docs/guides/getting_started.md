# Getting Started with Strata

This guide covers building Strata from source, integrating it into a CMake project, and writing your first program to parse JSON, navigate the result tree, and serialize back to a string.

______________________________________________________________________

## Prerequisites

- **C++20 compiler:** GCC 11+, Clang 14+, or MSVC 2022+
- **CMake 3.20+** (for the CMake build path)
- **Make** (for the Makefile convenience targets)
- **Python 3.14+** (optional, only for the Python bindings and test suite)

Strata auto-detects SIMD capabilities at compile time. For best performance, compile with architecture-native flags:

```bash
# GCC / Clang
-march=native

# MSVC
/arch:AVX2
```

______________________________________________________________________

## Building from Source

### Option 1: Makefile (recommended for development)

The top-level `Makefile` provides convenience targets that handle virtual environment setup, building, testing, and benchmarking.

```bash
# Clone the repository
git clone https://github.com/PrimeLabFoundation/strata.git
cd strata

# Create a virtual environment and install dev dependencies
make dev

# Build and install the extension (editable mode)
make install

# Run all tests (C++ and Python)
make all

# Run only C++ tests
make test-cpp

# Run only Python tests
make test-py
```

### Option 2: CMake (for C++ integration)

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
ctest --output-on-failure
```

#### CMake Options

| Option              | Default | Description                                                           |
| ------------------- | ------- | --------------------------------------------------------------------- |
| `STRATA_ENABLE_LTO` | `OFF`   | Enable Link-Time Optimization for performance builds                  |
| `CMAKE_BUILD_TYPE`  | (none)  | Set to `Release` for optimized builds, `RelWithDebInfo` for profiling |

#### Integrating into Your CMake Project

Add Strata as a subdirectory or use `FetchContent`:

```cmake
include(FetchContent)
FetchContent_Declare(
    strata
    GIT_REPOSITORY https://github.com/PrimeLabFoundation/strata.git
    GIT_TAG main
)
FetchContent_MakeAvailable(strata)

target_link_libraries(my_app PRIVATE strata)
```

______________________________________________________________________

## First Example: Parse, Navigate, Serialize

This example parses a JSON string, navigates the result using `JsonCursor`, and serializes a modified value back to JSON.

```cpp
#include <strata/json/json_parse.hpp>
#include <strata/json/json_cursor.hpp>
#include <strata/json/json_serialize.hpp>
#include <cstdio>

int main() {
    // 1. Parse a JSON string
    const char* input = R"({
        "name": "Alice",
        "age": 30,
        "scores": [95, 87, 92],
        "address": {
            "city": "Portland",
            "state": "OR"
        }
    })";

    auto result = strata::parse_json(input);
    if (!result.ok()) {
        std::fprintf(stderr, "Parse error\n");
        return 1;
    }

    // 2. Navigate with JsonCursor
    strata::JsonCursor root(&result.value());

    // Access a string field
    auto name = root.field("name").get_str();
    std::printf("Name: %s\n", name.c_str());

    // Access a numeric field
    auto age = root.field("age").get_int();
    std::printf("Age: %lld\n", static_cast<long long>(age));

    // Access an array element
    auto first_score = root.field("scores").at(0).get_float();
    std::printf("First score: %.0f\n", first_score);

    // Navigate nested objects
    auto city = root.field("address").field("city").get_str();
    std::printf("City: %s\n", city.c_str());

    // Iterate array elements
    auto scores = root.field("scores");
    std::printf("Scores (%zu): ", scores.array_size());
    for (size_t i = 0; i < scores.array_size(); ++i) {
        std::printf("%.0f ", scores.at(i).get_float());
    }
    std::printf("\n");

    // List object keys
    auto keys = root.object_keys();
    std::printf("Top-level keys: ");
    for (const auto& k : keys) {
        std::printf("%s ", k.c_str());
    }
    std::printf("\n");

    // 3. Serialize back to JSON
    std::string json_out = strata::serialize_json(result.value());
    std::printf("Serialized: %s\n", json_out.c_str());

    return 0;
}
```

### Using the Status-Code API

For performance-sensitive code paths where exceptions are undesirable, use the `Result<T>`-based API:

```cpp
auto name_result = root.get_field("name");
if (name_result.ok()) {
    auto str_result = name_result.value().get_string();
    if (str_result.ok()) {
        std::printf("Name: %.*s\n",
                    static_cast<int>(str_result.value().size()),
                    str_result.value().data());
    }
}

// Numeric access with status codes
auto age_result = root.get_field("age");
if (age_result.ok()) {
    auto int_result = age_result.value().get_int64();
    if (int_result.ok()) {
        std::printf("Age: %lld\n", static_cast<long long>(int_result.value()));
    }
}
```

______________________________________________________________________

## SIMD Backend Verification

To verify which SIMD backend Strata is using at runtime:

```cpp
#include <strata/simd/dispatch.h>
#include <cstdio>

int main() {
    auto backend = strata::simd::detect_backend();
    std::printf("SIMD backend: %s\n", strata::simd::backend_name(backend));
    // Typical output: "AVX2_BMI2", "NEON", "SCALAR", etc.
}
```

______________________________________________________________________

## Serialization to Pre-allocated Buffer

For reduced allocation overhead in hot loops, use `serialize_json_to`:

```cpp
std::string buffer;
buffer.reserve(4096); // pre-allocate once

for (const auto& value : values) {
    strata::serialize_json_to(value, buffer);
    // buffer now contains the JSON string, reused across iterations
    process(buffer);
}
```

______________________________________________________________________

## Duplicate Key Policy

Control how repeated object keys are handled during parsing:

```cpp
// Available policies:
// - DuplicateKeyPolicy::FirstWins  (keep first occurrence)
// - DuplicateKeyPolicy::LastWins   (keep last occurrence, default)
// - DuplicateKeyPolicy::Error      (fail on duplicate)
// - DuplicateKeyPolicy::Warn       (keep last, record warning)
```

______________________________________________________________________

## Next Steps

- [Selective Parsing Guide](selective_parsing.md) -- Parse only the fields you need
- [NDJSON Streaming Guide](ndjson_streaming.md) -- Process newline-delimited JSON streams
- [Performance Tuning Guide](performance_tuning.md) -- Optimize for your hardware and workload
- [SIMD API Reference](../api/simd_api.md) -- Structural indexing internals
- [Speculative API Reference](../api/speculative_api.md) -- Prediction engine details
- [Bloom API Reference](../api/bloom_api.md) -- Bloom filter subsystem
