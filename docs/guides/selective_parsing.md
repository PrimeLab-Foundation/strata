# Selective Parsing Guide

Selective parsing allows you to extract only the JSON fields you need, skipping unwanted values at near-zero cost. When a JSON object has 500 keys but you only need 5, the Bloom filter rejects ~99% of keys without any string comparison, and skipped values are jumped over using the structural index rather than being parsed.

______________________________________________________________________

## How It Works

Selective parsing combines three components:

1. **KeyFilter** -- A Bloom filter (512-bit `SmallFilter`) that provides fast probabilistic rejection of unwanted keys. For the ~1% false positives, a binary search over the sorted desired key set gives the definitive answer.

1. **Structural Index** -- The SIMD-generated array of byte offsets for structural characters (`{`, `}`, `[`, `]`, `:`, `,`). When a value should be skipped, the parser counts braces/brackets in this array to jump directly to the end of the value without reading any of its content.

1. **SelectiveParser** -- The coordinator that calls `KeyFilter::should_parse()` for each key and either parses the value or calls `skip_value()` to advance past it.

The `skip_value()` function is the key optimization:

- For primitives (numbers, strings, bools, null): advance to the next structural character (`,`, `}`, or `]`).
- For objects/arrays: count matching braces/brackets in the structural index until depth returns to the entry level.
- Cost: O(structural characters in the skipped value) with **zero actual value parsing**.

______________________________________________________________________

## Basic Usage

```cpp
#include <strata/bloom/parser_integration.h>
#include <strata/simd/dispatch.h>
#include <cstdio>
#include <cstring>

int main() {
    const char* json = R"({
        "id": 12345,
        "name": "Alice Johnson",
        "email": "alice@example.com",
        "department": "Engineering",
        "salary": 125000.50,
        "active": true,
        "start_date": "2021-03-15",
        "manager": "Bob Smith",
        "location": {"building": "A", "floor": 3, "desk": "A3-42"},
        "skills": ["C++", "Rust", "Python", "Go"],
        "projects": [
            {"name": "Atlas", "role": "Lead"},
            {"name": "Beacon", "role": "Contributor"}
        ]
    })";
    auto len = std::strlen(json);
    auto data = reinterpret_cast<const uint8_t*>(json);

    // 1. Build the structural index (SIMD-accelerated)
    auto index = strata::simd::index_document(data, len);

    // 2. Define desired keys
    std::array<std::string_view, 3> desired = {"name", "email", "salary"};

    // 3. Create the key filter
    strata::bloom::KeyFilter filter(desired);

    // 4. Parse selectively
    strata::bloom::SelectiveParser parser(filter);
    auto result = parser.parse_selective(
        data, len,
        index.positions.data(),
        index.positions.size());

    // Result contains only "name", "email", and "salary"
    // All other fields were skipped without parsing their values

    // 5. Inspect filter effectiveness
    auto stats = parser.parse_stats();
    std::printf("Values parsed:  %llu\n", stats.values_parsed);
    std::printf("Values skipped: %llu\n", stats.values_skipped);
    std::printf("Keys checked:   %llu\n", stats.keys_checked);

    auto filter_stats = filter.stats();
    std::printf("Bloom reject rate:       %.1f%%\n", filter_stats.reject_rate() * 100);
    std::printf("Bloom false positive rate: %.2f%%\n", filter_stats.false_positive_rate() * 100);

    return 0;
}
```

______________________________________________________________________

## Building Filters from JSONPath Expressions

If your desired fields are expressed as JSONPath-like paths, use `KeyFilter::from_paths()`:

```cpp
std::array<std::string_view, 3> paths = {
    "$.user.name",
    "$.user.email",
    "$.items[*].price"
};

// Extracts leaf field names: "name", "email", "price"
auto filter = strata::bloom::KeyFilter::from_paths(paths);
```

This extracts the leaf field names from each path and builds the Bloom filter over them.

______________________________________________________________________

## Selective Parsing with the Speculative Parser

The speculative parser also supports key filtering. When a `KeyFilter` is attached, it skips values for non-matching keys using the structural index, combining speculation benefits with selective parsing:

```cpp
#include <strata/speculative/parser.h>
#include <strata/bloom/key_filter.h>
#include <strata/simd/dispatch.h>
#include <strata/util/arena_allocator.hpp>

int main() {
    auto data = /* ... */;
    auto len = /* ... */;

    auto index = strata::simd::index_document(data, len);

    // Create filter
    std::array<std::string_view, 2> desired = {"name", "age"};
    strata::bloom::KeyFilter filter(desired);

    // Configure speculative parser with key filter
    strata::util::Arena arena;
    strata::speculative::SpeculativeParser::Config config;
    strata::speculative::SpeculativeParser parser(config, arena);
    parser.set_key_filter(&filter);  // filter must outlive the parse call

    auto result = parser.parse(
        data, len,
        index.positions.data(),
        index.positions.size());

    return 0;
}
```

______________________________________________________________________

## Selective Parsing in NDJSON Streams

For NDJSON data, set the key filter on the `NdjsonStream`:

```cpp
#include <strata/json/ndjson_stream.hpp>
#include <strata/bloom/key_filter.h>

int main() {
    std::string_view ndjson = R"({"id":1,"name":"Alice","age":30,"city":"Portland"}
{"id":2,"name":"Bob","age":25,"city":"Seattle"}
{"id":3,"name":"Charlie","age":35,"city":"Denver"})";

    strata::NdjsonStream stream(ndjson);

    // Only extract "name" and "age" from each record
    std::array<std::string_view, 2> desired = {"name", "age"};
    stream.set_key_filter(
        std::make_unique<strata::bloom::KeyFilter>(desired));

    // Each parsed record will contain only "name" and "age"
    auto results = stream.parse_all_fast(/*skip_errors=*/true);

    for (const auto& doc : results) {
        // doc contains only {"name": "...", "age": N}
    }

    return 0;
}
```

______________________________________________________________________

## Performance Characteristics

### Cost of Skipping vs. Parsing

| Operation                       | Cost       | Description                               |
| ------------------------------- | ---------- | ----------------------------------------- |
| Bloom reject                    | ~2 ns      | wyhash + 3 bit tests (single cache line)  |
| Exact match (on false positive) | ~15-30 ns  | Binary search on sorted key set           |
| Skip primitive value            | ~1-3 ns    | Advance structural index pointer          |
| Skip nested object/array        | ~5-20 ns   | Count braces/brackets in structural index |
| Parse a string value            | ~20-100 ns | Depends on string length                  |
| Parse a number value            | ~10-30 ns  | SWAR/fast_float parsing                   |

### When Selective Parsing Helps Most

- **Wide objects:** JSON objects with many keys where you need only a few. With 100 keys and 3 desired, selective parsing skips ~97% of value parsing.
- **Deep nested values:** Skipping a large nested object (e.g., an embedded sub-document with hundreds of fields) costs only the brace-counting traversal of the structural index.
- **NDJSON streams:** Each line is an independent document; the filter rejects the same unwanted keys on every line.

### When It Adds Overhead

- **Narrow objects:** If an object has only 3-5 keys and you need most of them, the Bloom filter check adds overhead without saving significant work.
- **All keys needed:** If you need every field, do not use selective parsing -- the filter checks are pure overhead.

______________________________________________________________________

## Schema Validation with SchemaFilter

For validating keys against a known JSON schema (rejecting unknown keys), use `SchemaFilter`:

```cpp
#include <strata/bloom/schema_filter.h>

// Build from a JSON Schema string
strata::bloom::SchemaFilter filter(R"({
    "type": "object",
    "properties": {
        "name": {"type": "string"},
        "age": {"type": "integer"},
        "email": {"type": "string"}
    }
})");

// Validate individual keys
auto result = filter.validate("name");
// result.is_valid == true (or needs_exact_check on Bloom positive)

auto unknown = filter.validate("foobar");
// unknown.is_valid == false (Bloom filter definitively rejects)

// Depth-aware validation
filter.add_keys_at_depth({"city", "state", "zip"}, 2);
auto deep = filter.validate_at_depth("city", 2);
// deep.is_valid == true at depth 2

// Batch validation (up to 64 keys)
std::array<std::string_view, 4> keys = {"name", "age", "unknown1", "unknown2"};
uint64_t valid_mask = filter.validate_batch(keys.data(), keys.size());
// valid_mask: bits 0 and 1 set (name, age valid), bits 2 and 3 clear
```

______________________________________________________________________

## Duplicate Key Detection with DedupFilter

To detect duplicate keys within objects during parsing:

```cpp
#include <strata/bloom/dedup_filter.h>

strata::bloom::DedupFilter dedup;

// At each object key during parsing:
if (dedup.check_and_insert("name")) {
    // Possible duplicate -- do exact check against already-parsed keys
}

// Entering a nested object:
dedup.push();

// Leaving a nested object:
dedup.pop();

// New document:
dedup.reset();

// Memory usage: 1088 bytes total for up to 32 nesting levels
```

The false-positive rate for `TinyFilter` with 10 or fewer keys is approximately 0.14%, so exact checks on suspects are rare.
