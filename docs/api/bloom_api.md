# Bloom Subsystem API Reference

The Bloom subsystem provides probabilistic data structures for fast key filtering, duplicate detection, and schema validation during JSON parsing. All Bloom filters use wyhash with Kirschner-Mitzenmacher double hashing and a cache-line-aware blocked design for larger filters.

**Headers:**

- `strata/bloom/bloom_filter.h`
- `strata/bloom/sizing.h`
- `strata/bloom/key_filter.h`
- `strata/bloom/dedup_filter.h`
- `strata/bloom/schema_filter.h`
- `strata/bloom/parser_integration.h`

**Namespace:** `strata::bloom`

______________________________________________________________________

## BloomFilter\<NumBits, NumHashes>

Compile-time configurable Bloom filter with double hashing.

```cpp
template <size_t NumBits = 256, size_t NumHashes = 3>
class BloomFilter;
```

- **Template Parameters:**
  - `NumBits` -- Total bit count; must be a multiple of 256 (AVX2 alignment). Default: 256.
  - `NumHashes` -- Number of hash probes per query; must be in \[2, 8\]. Default: 3.
- **Static Asserts:** `NumBits % 256 == 0` and `2 <= NumHashes <= 8`.

### Blocked Design

For filters where `NumBits > 512` (more than one cache line), a blocked design is used: `h1` selects a 512-bit (64-byte) block, and all `k` probes land within that block. This guarantees every query touches exactly one cache line.

### Constants

```cpp
static constexpr size_t BYTE_SIZE   = NumBits / 8;
static constexpr size_t NUM_BITS    = NumBits;
static constexpr size_t NUM_HASHES  = NumHashes;
static constexpr bool   BLOCKED     = (BYTE_SIZE > 64);
static constexpr size_t BLOCK_BITS  = 512;
static constexpr size_t BLOCK_BYTES = 64;
static constexpr size_t NUM_BLOCKS  = BLOCKED ? (BYTE_SIZE / BLOCK_BYTES) : 1;
```

### Constructor

```cpp
BloomFilter() noexcept;
```

Initializes the filter to the empty state (all bits zero).

### Methods

#### insert

```cpp
void insert(std::string_view key) noexcept;
```

Insert a key into the filter by setting `NumHashes` bits determined by double hashing.

- **Parameters:** `key` -- The key to insert.
- **Complexity:** O(NumHashes). Constant for any given template instantiation.

#### insert_all

```cpp
void insert_all(std::initializer_list<std::string_view> keys) noexcept;
```

Bulk insert from an initializer list.

#### maybe_contains

```cpp
[[nodiscard]] bool maybe_contains(std::string_view key) const noexcept;
```

Query the filter.

- **Parameters:** `key` -- The key to look up.
- **Returns:** `true` if the key MIGHT be in the set (possible false positive), `false` if DEFINITELY NOT in the set (no false negatives).
- **Complexity:** O(NumHashes).

#### clear

```cpp
void clear() noexcept;
```

Reset the filter to the empty state (all bits zero).

#### count_set_bits

```cpp
[[nodiscard]] size_t count_set_bits() const noexcept;
```

Count the number of set bits (population count). Uses `__builtin_popcountll` over 8-byte words.

- **Returns:** Number of bits set to 1 in the filter.

#### fill_ratio

```cpp
[[nodiscard]] double fill_ratio() const noexcept;
```

Fraction of bits that are set: `count_set_bits() / NUM_BITS`.

#### estimated_false_positive_rate

```cpp
[[nodiscard]] double estimated_false_positive_rate() const noexcept;
```

Estimated false-positive rate based on the current fill level. Uses the formula `FPR = (fill_ratio)^k`.

#### merge

```cpp
void merge(const BloomFilter& other) noexcept;
```

Merge another filter via bitwise OR (union semantics). Used for combining per-thread filters after parallel processing.

______________________________________________________________________

## Sizing Utilities

### optimal_bits

```cpp
constexpr size_t optimal_bits(size_t num_keys, double false_positive_rate) noexcept;
```

Compute optimal number of bits: `m = -(n * ln(p)) / (ln(2))^2`.

- **Parameters:**
  - `num_keys` -- Expected number of keys to insert.
  - `false_positive_rate` -- Target FPR (e.g., 0.01 for 1%).
- **Returns:** Optimal number of bits (not yet aligned to 256).

### optimal_hashes

```cpp
constexpr size_t optimal_hashes(size_t num_bits, size_t num_keys) noexcept;
```

Compute optimal number of hash functions: `k = (m / n) * ln(2)`, clamped to \[2, 8\].

### align_bits

```cpp
constexpr size_t align_bits(size_t bits) noexcept;
```

Round up to the nearest multiple of 256 for AVX2 alignment.

### Preset

```cpp
template <size_t ExpectedKeys, int FalsePositivePermille = 10>
struct Preset {
    static constexpr size_t bits;
    static constexpr size_t hashes;
    using type = BloomFilter<bits, hashes>;
};
```

Pre-computed filter configuration from expected key count and FPR. `FalsePositivePermille` is the FPR times 1000 (e.g., 10 = 1%, 1 = 0.1%).

```cpp
// Example: filter for 50 keys at 1% FPR
using MyFilter = bloom::Preset<50, 10>::type;
```

### Preset Aliases

| Alias          | Keys | FPR | Bits  | Hashes | Notes                        |
| -------------- | ---- | --- | ----- | ------ | ---------------------------- |
| `TinyFilter`   | 10   | 1%  | 256   | 3      | Single cache line (32 bytes) |
| `SmallFilter`  | 50   | 1%  | 512   | 3      | Two cache lines              |
| `MediumFilter` | 200  | 1%  | 2048  | 3      | Blocked, 4 cache lines       |
| `LargeFilter`  | 1000 | 1%  | 10240 | 3      | Blocked, ~20 cache lines     |

______________________________________________________________________

## KeyFilter

Selective key extraction filter for the parser hot path. Wraps a `SmallFilter` (512-bit Bloom) with a sorted exact-match fallback.

### Workflow

1. User specifies desired keys (e.g., from a JSONPath query).
1. `KeyFilter` builds a Bloom filter over those keys.
1. For each key encountered during parsing:
   - Bloom filter rejects ~99% of unwanted keys instantly (no string comparison).
   - For the ~1% false positives, binary search on the sorted key set provides the definitive answer.

### Action

```cpp
enum class Action : uint8_t {
    PARSE,     // Key is wanted -- parse the value
    SKIP,      // Key is definitely not wanted -- skip the value
    UNCERTAIN  // Bloom filter says maybe -- do exact check
};
```

### Constructor

```cpp
explicit KeyFilter(std::span<const std::string_view> desired_keys);
```

Build from a list of desired keys. Inserts all keys into the internal `SmallFilter` and stores a sorted copy for exact matching.

### Factory

#### from_paths

```cpp
static KeyFilter from_paths(std::span<const std::string_view> paths);
```

Build from JSONPath-like expressions: `"$.user.name"`, `"$.items[*].price"`. Extracts leaf field names from each path.

### Methods

#### check

```cpp
[[nodiscard]] Action check(std::string_view key) const noexcept;
```

Fast Bloom-only check.

- **Returns:**
  - `SKIP` if the key is definitely not wanted.
  - `UNCERTAIN` if the Bloom filter returned positive (needs exact check).
  - `PARSE` if the filter is empty (accept all keys).

#### exact_match

```cpp
[[nodiscard]] bool exact_match(std::string_view key) const noexcept;
```

For `UNCERTAIN` results, perform exact string comparison via binary search on the sorted key set.

#### should_parse

```cpp
[[nodiscard]] bool should_parse(std::string_view key) const noexcept;
```

Combined check: Bloom filter first, then exact match on positive. This is the hot-path function the parser calls.

- **Returns:** `true` if the key is in the desired set; `false` otherwise.

### Stats

```cpp
struct Stats {
    uint64_t total_checks    = 0;
    uint64_t bloom_rejects   = 0;
    uint64_t bloom_positives = 0;
    uint64_t exact_matches   = 0;
    uint64_t false_positives = 0;

    [[nodiscard]] double reject_rate() const noexcept;
    [[nodiscard]] double false_positive_rate() const noexcept;
};
```

```cpp
[[nodiscard]] Stats stats() const noexcept;
void reset_stats() noexcept;
```

______________________________________________________________________

## DedupFilter

Duplicate key detector using stacked `TinyFilter` (256-bit) Bloom filters. Detects duplicate keys within a single JSON object without maintaining a hash set.

### Memory Budget

Each nesting level uses exactly 32 bytes (256-bit filter) + 2 bytes (key count). The entire stack for depth 32 is `32 * 34 = 1088 bytes`.

### Constructor

```cpp
DedupFilter() noexcept;
```

Initializes depth to 0 and clears the root filter.

### Methods

#### check_and_insert

```cpp
[[nodiscard]] bool check_and_insert(std::string_view key) noexcept;
```

Check if the key was already seen in the current object, then insert it.

- **Returns:** `true` if the key is POSSIBLY a duplicate (Bloom filter positive). On `true`, the caller should do an exact check against already-parsed keys.
- **False positive rate:** ~0.14% for objects with 10 or fewer keys.

#### push

```cpp
void push() noexcept;
```

Enter a nested object: push a new filter level. Maximum depth is 32; deeper nesting is silently ignored.

#### pop

```cpp
void pop() noexcept;
```

Leave a nested object: pop back to the parent level.

#### reset

```cpp
void reset() noexcept;
```

Reset for a new document. Clears all levels and returns to depth 0.

#### Accessors

```cpp
[[nodiscard]] uint64_t total_checks() const noexcept;
[[nodiscard]] uint64_t duplicate_suspects() const noexcept;
[[nodiscard]] uint8_t  current_depth() const noexcept;
```

______________________________________________________________________

## SchemaFilter

Schema validation filter for fast-rejecting unknown JSON keys. Provides two levels of filtering: a global filter (any depth) and per-depth filters (keys valid at specific nesting levels).

### Constructor

```cpp
explicit SchemaFilter(std::string_view json_schema);
explicit SchemaFilter(std::span<const std::string_view> allowed_keys);
```

Build from either a JSON Schema string (extracts all `"properties"` keys at all nesting levels) or a flat list of allowed keys.

### ValidationResult

```cpp
struct ValidationResult {
    bool     is_valid;          // false if key is definitely not in schema
    bool     needs_exact_check; // true if Bloom filter returned positive
    uint16_t key_hash;          // cached hash for downstream use
};
```

### Methods

#### validate

```cpp
[[nodiscard]] ValidationResult validate(std::string_view key) const noexcept;
```

Validate a key against the global schema (any depth).

#### validate_batch

```cpp
[[nodiscard]] uint64_t validate_batch(const std::string_view* keys, size_t count) const noexcept;
```

Batch validate: check up to 64 keys at once. Returns a bitmask where bit `i` is set if key `i` passes (might be valid). `count` must be at most 64.

#### validate_at_depth

```cpp
[[nodiscard]] ValidationResult validate_at_depth(std::string_view key, uint8_t depth) const noexcept;
```

Validate a key at a specific nesting depth. If `depth` exceeds `MAX_DEPTH` (16), falls back to the global filter.

#### add_keys_at_depth

```cpp
void add_keys_at_depth(std::span<const std::string_view> keys, uint8_t depth);
```

Add keys valid at a specific depth (for programmatic schema building).

#### num_keys

```cpp
[[nodiscard]] size_t num_keys() const noexcept;
```

Number of schema keys stored.

______________________________________________________________________

## SelectiveParser

Parses only selected fields from JSON objects using Bloom filter rejection and structural index skipping. Combines `IndexBuilder` output with a `KeyFilter`.

The `skip_value()` optimization is the key: for primitives, it advances to the next structural character; for objects/arrays, it counts braces/brackets in the structural index until depth returns to the entry level. This is O(structural_chars_in_value) but does ZERO actual value parsing.

### Constructor

```cpp
explicit SelectiveParser(const KeyFilter& filter);
```

- **Parameters:** `filter` -- Reference to a `KeyFilter` that determines which keys to parse. Must remain valid for the lifetime of the parser.

### Methods

#### parse_selective

```cpp
[[nodiscard]] JsonValue parse_selective(const uint8_t* data, size_t length,
                                        const uint32_t* structural_positions,
                                        size_t num_structural);
```

Parse a JSON document, extracting only desired keys.

- **Parameters:**
  - `data` -- Raw JSON bytes.
  - `length` -- Length in bytes.
  - `structural_positions` -- Array of byte offsets of structural chars (from `IndexBuilder`).
  - `num_structural` -- Number of entries in `structural_positions`.
- **Returns:** `JsonValue` containing only matched key-value pairs for objects. Arrays and non-object top-level values are parsed fully.

### ParseStats

```cpp
struct ParseStats {
    uint64_t values_parsed  = 0;  // Values actually parsed
    uint64_t values_skipped = 0;  // Values skipped via structural index
    uint64_t keys_checked   = 0;  // Keys passed through KeyFilter
};
```

```cpp
[[nodiscard]] ParseStats parse_stats() const noexcept;
void reset_stats() noexcept;
```

______________________________________________________________________

## Usage Example

```cpp
#include <strata/bloom/parser_integration.h>
#include <strata/simd/dispatch.h>
#include <cstdio>

int main() {
    const char* json = R"({
        "name": "Alice", "age": 30, "email": "alice@example.com",
        "address": {"city": "Portland", "state": "OR", "zip": "97201"},
        "scores": [95, 87, 92], "active": true
    })";
    auto len = std::strlen(json);
    auto data = reinterpret_cast<const uint8_t*>(json);

    // Build structural index
    auto index = strata::simd::index_document(data, len);

    // Create a key filter for only "name" and "age"
    std::array<std::string_view, 2> desired = {"name", "age"};
    strata::bloom::KeyFilter filter(desired);

    // Parse selectively
    strata::bloom::SelectiveParser parser(filter);
    auto result = parser.parse_selective(data, len,
                                          index.positions.data(),
                                          index.positions.size());

    // Check stats
    auto stats = parser.parse_stats();
    std::printf("Parsed: %llu, Skipped: %llu, Keys checked: %llu\n",
                stats.values_parsed, stats.values_skipped, stats.keys_checked);
}
```
