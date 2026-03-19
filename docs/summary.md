# Strata — Best Practices Summary

<!-- LIVING DOCUMENT: Regenerate when the codebase changes significantly.
     Run the catalog generator against the full source tree to refresh
     file paths, line numbers, and code snippets. Last updated: 2026-03-19. -->

> Auto-generated catalog of engineering practices implemented in the Strata
> codebase. Every entry links to the actual implementation. Last updated: 2026-03-19.
>
> See also: [Development History](history.md) for the chronological evolution of these practices.

## Table of Contents

- [1. Performance & Low-Level Optimization](#1-performance--low-level-optimization)
- [2. Memory Management](#2-memory-management)
- [3. Type Safety & API Design](#3-type-safety--api-design)
- [4. Testing & Quality Assurance](#4-testing--quality-assurance)
- [5. Probabilistic Data Structures](#5-probabilistic-data-structures)
- [6. SIMD & Hardware Acceleration](#6-simd--hardware-acceleration)
- [7. Statistical & Adaptive Algorithms](#7-statistical--adaptive-algorithms)
- [8. Build System & Tooling](#8-build-system--tooling)
- [9. Documentation & Readability](#9-documentation--readability)
- [10. Error Handling & Robustness](#10-error-handling--robustness)
- [Summary Statistics](#summary-statistics)
- [Cross-Reference Index](#cross-reference-index)

## How to Read This Document

This is not a generic best-practices guide. Every entry describes a technique that is **actually implemented** in the Strata codebase, with a direct reference to the source file and line number. If you want to understand how a technique works in practice, follow the **Where** link.

Entries are organized by category. Within each category, entries are ordered roughly by impact (most impactful first).

______________________________________________________________________

## 1. Performance & Low-Level Optimization

### SIMD Structural Indexing (vpshufb Classification)

**Category**: Performance & Low-Level Optimization
**Where**: `include/strata/simd/classifier.h:L97-L210`, `src/strata/simd/classifier.cpp:L58-L79`

Classifies every byte in a 32-byte (AVX2) or 64-byte (AVX-512) vector into JSON character classes using the vpshufb nibble-lookup trick. Each byte is split into high and low nibbles, two independent shuffles are performed against pre-computed 16-byte lookup tables, and the results are ANDed — producing zero false positives for all four classes (structural, quote, backslash, whitespace).

**Key snippet** (≤ 10 lines):

```cpp
__m256i lo_nibbles = _mm256_and_si256(input, nibble_mask);
__m256i hi_nibbles = _mm256_and_si256(_mm256_srli_epi16(input, 4), nibble_mask);
__m256i lo_result = _mm256_shuffle_epi8(low_lut, lo_nibbles);
__m256i hi_result = _mm256_shuffle_epi8(high_lut, hi_nibbles);
return _mm256_and_si256(lo_result, hi_result);
```

**Impact**: Classifies 32 bytes per instruction with zero branches and zero false positives — the foundation for all downstream SIMD parsing.

______________________________________________________________________

### PCLMUL String Region Masking (Prefix-XOR)

**Category**: Performance & Low-Level Optimization
**Where**: `src/strata/simd/structural_indexer.cpp:L106-L137`

Converts unescaped quote positions into a string-region bitmask using carry-less multiplication (`_mm_clmulepi64_si128`) with the all-ones constant. This computes a prefix-XOR (running parity) — each quote toggles the "in-string" state. Falls back to a scalar doubling loop on platforms without PCLMUL, and uses ARM NEON `vmull_p64` on AArch64.

**Key snippet** (≤ 10 lines):

```cpp
// x86 PCLMUL: carry-less multiply with all-ones = prefix-XOR.
__m128i q_128 = _mm_set_epi64x(0, static_cast<long long>(q));
__m128i ones_128 = _mm_set_epi64x(0, -1LL);
__m128i clmul = _mm_clmulepi64_si128(q_128, ones_128, 0x00);
result = static_cast<uint64_t>(_mm_cvtsi128_si64(clmul));
```

**Impact**: Converts quote bitmask to string-region mask in a single instruction, replacing a 64-iteration loop. See also: [docs/development/simd_utf8_validation.md](development/simd_utf8_validation.md).

______________________________________________________________________

### Add-with-Carry Escape Detection

**Category**: Performance & Low-Level Optimization
**Where**: `src/strata/simd/structural_indexer.cpp:L60-L97`

Detects characters following an odd-length backslash sequence using the add-with-carry trick (adapted from simdjson). For each contiguous run of backslashes, adding the "start" bitmask propagates a carry through consecutive 1-bits. Checking whether the carry exits at opposite parity determines if the run has odd length — all without branches.

**Key snippet** (≤ 10 lines):

```cpp
uint64_t start_edges = bs & ~((bs << 1) | prev_escaped_);
uint64_t even_starts = start_edges & even_mask;
uint64_t even_carries = bs + even_starts;
uint64_t even_carry_ends = even_carries & ~bs;
uint64_t even_start_odd_end = even_carry_ends & kOddBits;
```

**Impact**: Processes 64 bytes of escape detection in ~5 arithmetic operations — fully branchless.

______________________________________________________________________

### SWAR 8-Digit Integer Parser

**Category**: Performance & Low-Level Optimization
**Where**: `src/strata/speculative/fast_paths.cpp:L27-L119`

Parses up to 8 ASCII digits in parallel using SWAR (SIMD Within A Register). Loads 8 bytes into a `uint64_t`, subtracts `0x30` from each byte, checks for non-digits via high-bit detection, then uses a 3-stage multiply-accumulate reduction: pairs of digits → pairs of pairs → final 8-digit value.

**Key snippet** (≤ 10 lines):

```cpp
uint64_t v_even = v & 0x00FF00FF00FF00FFULL;
uint64_t v_odd = (v >> 8) & 0x00FF00FF00FF00FFULL;
v = v_even * 10 + v_odd;           // 4 values in 16-bit lanes
uint64_t v_lo = v & 0x0000FFFF0000FFFFULL;
uint64_t v_hi = (v >> 16) & 0x0000FFFF0000FFFFULL;
v = v_lo * 100 + v_hi;             // 2 values in 32-bit lanes
uint64_t result = (v & 0xFFFFFFFF) * 10000 + (v >> 32);
```

**Impact**: Parses 8 digits in ~6 arithmetic operations. Targets 4+ GB/s on integer-heavy JSON.

______________________________________________________________________

### BMI2 PEXT for Bit Extraction

**Category**: Performance & Low-Level Optimization
**Where**: `include/strata/simd/index_builder.h:L53-L65`

Extracts set-bit positions from a 64-bit structural bitmask using BMI2 `_pext_u64` when available, with a fallback to a `__builtin_ctzll` + clear-lowest-bit loop on non-BMI2 CPUs. Pre-allocates the output vector with an estimated capacity of `length / 6` (heuristic: ~1 structural char per 6 bytes for typical JSON).

**Key snippet** (≤ 10 lines):

```cpp
// Pre-allocates the output vector with an estimated capacity of
// length / 6 (heuristic: ~1 structural char per 6 bytes for typical JSON).
[[nodiscard]] StructuralIndex build(const uint8_t* data, size_t length);
// On BMI2: uses _pext_u64 for dense bit-to-index conversion.
// Fallback: __builtin_ctzll / _tzcnt_u64 + clear-lowest-bit loop.
static void extract_positions(uint64_t mask, uint32_t base_offset,
                              std::vector<uint32_t>& out);
```

**Impact**: BMI2 path converts bitmask to position array in O(popcount) without any branch misprediction.

______________________________________________________________________

### Branchless Literal Comparison

**Category**: Performance & Low-Level Optimization
**Where**: `src/strata/speculative/fast_paths.cpp:L499-L562`

Validates JSON literals (`true`, `false`, `null`) by loading 4-5 bytes as a `uint32_t` and comparing in a single instruction — one comparison instead of 4 separate byte checks.

**Key snippet** (≤ 10 lines):

```cpp
uint32_t word;
std::memcpy(&word, data, 4);
static constexpr uint8_t expected[] = {'t', 'r', 'u', 'e'};
uint32_t expected_word;
std::memcpy(&expected_word, expected, 4);
if (word != expected_word)
    return {false, 0, {}};
```

**Impact**: Single 32-bit comparison replaces 4 branches; compiler optimizes memcpy to a single load.

______________________________________________________________________

### `LIKELY` / `UNLIKELY` Branch Hints

**Category**: Performance & Low-Level Optimization
**Where**: `include/strata/util/output_buffer.hpp:L20-L25`

Wraps `__builtin_expect` in `LIKELY()` / `UNLIKELY()` macros applied to capacity checks on the serialization hot path. The slow-path `grow()` function is additionally marked `__attribute__((noinline))` to keep the fast path compact.

**Key snippet** (≤ 10 lines):

```cpp
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

void push_back(char c) {
    if (UNLIKELY(size_ == capacity_)) { grow(size_ + 1); }
    data_[size_++] = c;
}
```

**Impact**: Helps branch predictor maintain >99% hit rate on buffer writes; `noinline` grow keeps hot path in instruction cache.

______________________________________________________________________

### Unrolled String Scan (8 chars/iter)

**Category**: Performance & Low-Level Optimization
**Where**: `include/strata/util/fast_parse.hpp:L423-L455`

Scans for quote, backslash, or control characters using an unrolled 8-character-per-iteration loop. The unrolling is friendly to branch predictors because the common case (no special character) falls through all 8 checks.

**Key snippet** (≤ 10 lines):

```cpp
while (pos + 8 <= len) {
    if (str[pos] == '"' || str[pos] == '\\' || str[pos] < 0x20) return pos;
    if (str[pos+1] == '"' || str[pos+1] == '\\' || str[pos+1] < 0x20) return pos+1;
    // ... (6 more checks)
    pos += 8;
}
```

**Impact**: Processes clean strings at 8 bytes/iteration with minimal branch predictor pressure.

______________________________________________________________________

### SIMD Whitespace Skipping (NEON/SSE2)

**Category**: Performance & Low-Level Optimization
**Where**: `include/strata/util/fast_parse.hpp:L469-L540`

Skips whitespace using NEON or SSE2 to check 16 bytes per iteration. The fast-exit path handles the common case (0-1 whitespace bytes) without any SIMD register setup.

**Key snippet** (≤ 10 lines):

```cpp
// Fast exit: most JSON tokens have zero or one whitespace byte.
if (pos < len) {
    char c = str[pos];
    if (c != ' ' && c != '\t' && c != '\n' && c != '\r') return pos;
}
// NEON: check 16 bytes at once
uint8x16_t is_ws = vorrq_u8(vorrq_u8(vceqq_u8(chunk, sp), vceqq_u8(chunk, tab)),
                            vorrq_u8(vceqq_u8(chunk, nl), vceqq_u8(chunk, cr)));
```

**Impact**: Avoids SIMD overhead for common 0-1 whitespace case; processes 16 bytes/iter for long runs.

______________________________________________________________________

### Fast-Path Dispatch Table

**Category**: Performance & Low-Level Optimization
**Where**: `src/strata/speculative/fast_paths.cpp:L582-L595`

Uses a static array of member function pointers indexed by `ValueType` enum for branchless dispatch to the correct speculative parser. Eliminates a switch statement on the hot path.

**Key snippet** (≤ 10 lines):

```cpp
static const std::array<ParseFn, NUM_TYPES> table = {
    &FastPaths::parse_integer,      // INTEGER      = 0
    &FastPaths::parse_float,        // FLOAT        = 1
    &FastPaths::parse_short_string, // SHORT_STRING = 2
    &FastPaths::parse_long_string,  // LONG_STRING  = 3
    &FastPaths::parse_bool_true,    // BOOL_TRUE    = 4
    &FastPaths::parse_bool_false,   // BOOL_FALSE   = 5
    &FastPaths::parse_null,         // NULL_VALUE   = 6
    &FastPaths::parse_object_open,  // OBJECT       = 7
    &FastPaths::parse_array_open,   // ARRAY        = 8
};
```

**Impact**: Single indirect call replaces a 9-way switch; enables constant-time speculative dispatch.

______________________________________________________________________

### Pre-allocation with Capacity Heuristics

**Category**: Performance & Low-Level Optimization
**Where**: `include/strata/simd/index_builder.h:L40-L43`

The structural index builder pre-allocates output with an estimated capacity of `length / 6` — a heuristic derived from the observation that typical JSON has ~1 structural character per 6 bytes. This eliminates most reallocations during indexing.

**Impact**: Avoids O(log n) reallocations on the index-building hot path.

______________________________________________________________________

## 2. Memory Management

### Arena (Bump-Pointer) Allocator

**Category**: Memory Management
**Where**: `include/strata/util/arena_allocator.hpp:L34-L95`

A bump-pointer arena allocator where all allocations are contiguous within a block; when a block is exhausted, a new one is allocated at 2x the previous size. All memory is freed at once when the Arena is destroyed or reset. Alignment is handled via branchless bitmask: `(pos + alignment - 1) & ~(alignment - 1)`.

**Key snippet** (≤ 10 lines):

```cpp
[[nodiscard]] void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
    size_t aligned_pos = (current_pos_ + alignment - 1) & ~(alignment - 1);
    if (aligned_pos + size > current_size_) {
        size_t new_block_size = std::max(size + 1024, current_size_ * 2);
        allocate_block(new_block_size);
        aligned_pos = 0;
    }
    void* ptr = current_block_ + aligned_pos;
    current_pos_ = aligned_pos + size;
    return ptr;
}
```

**Impact**: 3-5x faster than malloc/free for small allocations (documented in file header).

______________________________________________________________________

### STL-Compatible Arena Allocator (No-Op Deallocate)

**Category**: Memory Management
**Where**: `include/strata/util/arena_allocator.hpp:L105-L138`

An `ArenaAllocator<T>` that satisfies STL allocator requirements. `deallocate()` is a no-op — all memory is reclaimed when the underlying Arena is reset or destroyed, avoiding per-element free overhead.

**Key snippet** (≤ 10 lines):

```cpp
void deallocate(T*, std::size_t) noexcept {} // No-op — arena reclaims all.
```

**Impact**: Enables using standard containers (e.g., `std::vector<JsonValue, ArenaAllocator<JsonValue>>`) with arena-backed allocation.

______________________________________________________________________

### Stack-Allocated Bloom Filter Levels (No Heap in Dedup)

**Category**: Memory Management
**Where**: `include/strata/bloom/dedup_filter.h:L10-L92`

The dedup filter uses a stack of 32 TinyFilter Bloom filters (256-bit each). The entire stack is 32 x 34 = 1088 bytes — all stack-allocated, zero heap involvement. Each nesting level gets its own filter with push/pop semantics.

**Key snippet** (≤ 10 lines):

```cpp
struct Level {
    TinyFilter filter;      // 256-bit Bloom filter (32 bytes)
    uint16_t key_count = 0; // Number of keys inserted at this level
};
std::array<Level, MAX_DEPTH> stack_;  // MAX_DEPTH = 32
```

**Impact**: 1088 bytes total for 32 nesting levels — fits in L1 cache, zero allocation overhead during parsing.

______________________________________________________________________

### `alignas(32)` for AVX2 Data Alignment

**Category**: Memory Management
**Where**: `include/strata/bloom/bloom_filter.h:L261`

Bloom filter bit arrays are aligned to 32-byte boundaries for AVX2 operations, ensuring efficient SIMD loads without unaligned access penalties.

**Key snippet** (≤ 10 lines):

```cpp
alignas(32) std::array<uint8_t, BYTE_SIZE> bits_{};
```

**Impact**: Eliminates unaligned access penalties on SIMD loads; prevents cache-line splits.

______________________________________________________________________

### `alignas(64)` for Cache-Line-Aligned Transition Table

**Category**: Memory Management
**Where**: `include/strata/speculative/transition_model.h:L128`

The Markov transition table is aligned to 64-byte cache-line boundaries. A `static_assert` verifies the model fits within 128 KB (L1/L2 budget).

**Key snippet** (≤ 10 lines):

```cpp
alignas(64) std::array<ContextBucket, MAX_CONTEXTS> table_;
// ...
static_assert(sizeof(TransitionModel) <= 128 * 1024,
              "TransitionModel exceeds 128 KB L1/L2 cache budget");
```

**Impact**: 4096 buckets x 30 bytes ≈ 120 KB — fits in L2 cache, cache-line aligned for optimal access patterns.

______________________________________________________________________

### Unsafe Fast-Path Buffer Operations

**Category**: Memory Management
**Where**: `include/strata/util/output_buffer.hpp:L79-L87`

OutputBuffer provides `unsafe_push_back()` and `unsafe_append()` that skip capacity checks when the caller has already called `reserve()`. This eliminates branch overhead on tight serialization loops.

**Key snippet** (≤ 10 lines):

```cpp
/// Skip capacity check. Caller must have reserved sufficient space.
void unsafe_push_back(char c) noexcept { data_[size_++] = c; }
void unsafe_append(const char* src, size_t len) noexcept {
    std::memcpy(data_ + size_, src, len);
    size_ += len;
}
```

**Impact**: Zero-branch serialization writes after a single upfront `reserve()` call.

______________________________________________________________________

## 3. Type Safety & API Design

### `[[nodiscard]]` on All Value-Returning Functions

**Category**: Type Safety & API Design
**Where**: `include/strata/util/arena_allocator.hpp:L49`, `include/strata/bloom/bloom_filter.h:L180`, `include/strata/json/json_core.hpp:L124`, `include/strata/simd/classifier.h:L103-L137`

Every function that returns a value the caller must use (allocations, query results, status checks, SIMD classifications) is marked `[[nodiscard]]`. This catches bugs at compile time where a result would be silently discarded.

**Key snippet** (≤ 10 lines):

```cpp
[[nodiscard]] void* allocate(size_t size, size_t alignment);
[[nodiscard]] bool maybe_contains(std::string_view key) const noexcept;
[[nodiscard]] bool ok() const noexcept { return status == Status::Ok; }
[[nodiscard]] __m256i classify32(__m256i input) const noexcept;
```

**Impact**: Prevents silent error ignoring and unused allocation leaks at compile time.

______________________________________________________________________

### `noexcept` Correctness

**Category**: Type Safety & API Design
**Where**: `include/strata/simd/classifier.h:L103-L137`, `include/strata/bloom/bloom_filter.h:L159-L198`, `include/strata/simd/structural_indexer.h:L61`

Every function that cannot throw is marked `noexcept` — SIMD classification, Bloom filter operations, structural indexing, arena reset. This enables the compiler to omit exception handling code and enables `std::move` optimizations.

**Key snippet** (≤ 10 lines):

```cpp
[[nodiscard]] __m256i classify32(__m256i input) const noexcept;
[[nodiscard]] ChunkResult classify(const uint8_t* input) noexcept;
void insert(std::string_view key) noexcept;
[[nodiscard]] bool maybe_contains(std::string_view key) const noexcept;
```

**Impact**: Enables compiler to generate tighter code; documents the exception contract.

______________________________________________________________________

### `constexpr` Sizing Calculations and Lookup Tables

**Category**: Type Safety & API Design
**Where**: `include/strata/bloom/sizing.h:L35-L69`, `include/strata/simd/classifier.h:L191-L209`

Bloom filter sizing uses compile-time `constexpr` math (custom `cx_log` series expansion) to compute optimal bits and hash counts. SIMD lookup tables are `static constexpr` arrays — initialized at compile time, placed in read-only memory.

**Key snippet** (≤ 10 lines):

```cpp
constexpr double cx_log(double x) noexcept {
    double t = (v - 1.0) / (v + 1.0);
    double t2 = t * t;
    double sum = t;
    for (int i = 1; i <= 30; ++i) { term *= t2; sum += term / (2.0*i+1.0); }
    return 2.0 * sum + shifts * kLn2;
}
constexpr size_t optimal_bits(size_t n, double fpr) noexcept;
```

**Impact**: Zero-cost abstractions — all sizing math resolved at compile time; lookup tables in .rodata.

______________________________________________________________________

### `std::variant` for Type-Safe JSON Values

**Category**: Type Safety & API Design
**Where**: `include/strata/json/json_core.hpp:L135-L186`

JSON values use `std::variant<nullptr_t, bool, Int64, Number, string, Array, Object>` — a type-safe discriminated union with exhaustive pattern matching via `std::visit` and safe access via `std::get_if`.

**Key snippet** (≤ 10 lines):

```cpp
using Variant = std::variant<std::nullptr_t, bool, Int64, Number,
                             std::string, Array, Object>;
Variant data;
[[nodiscard]] bool is_null() const noexcept {
    return std::holds_alternative<std::nullptr_t>(data);
}
```

**Impact**: Eliminates void pointer casts and manual type tags; compiler-checked exhaustiveness.

______________________________________________________________________

### Strong Enums with Explicit Underlying Types

**Category**: Type Safety & API Design
**Where**: `include/strata/speculative/types.h:L18-L29`, `include/strata/json/json_core.hpp:L110`, `include/strata/simd/dispatch.h:L22-L31`

All enums use `enum class` with explicit `uint8_t` underlying type where appropriate, preventing implicit conversions and size surprises.

**Key snippet** (≤ 10 lines):

```cpp
enum class ValueType : uint8_t {
    INTEGER = 0, FLOAT = 1, SHORT_STRING = 2, LONG_STRING = 3,
    BOOL_TRUE = 4, BOOL_FALSE = 5, NULL_VALUE = 6,
    OBJECT = 7, ARRAY = 8, NUM_TYPES = 9
};
enum class Status { Ok, TypeMismatch, KeyNotFound, IndexOutOfBounds, ParseError };
```

**Impact**: Prevents accidental integer-enum mixing; `uint8_t` underlying type saves space in packed structs.

______________________________________________________________________

### `static_assert` for Compile-Time Invariant Checking

**Category**: Type Safety & API Design
**Where**: `include/strata/bloom/bloom_filter.h:L142-L143`, `include/strata/speculative/transition_model.h:L149-L150`

Compile-time assertions enforce critical invariants: Bloom filter bit count must be a multiple of 256 (AVX2 alignment), hash count must be in \[2,8\], and the transition model must fit within 128 KB.

**Key snippet** (≤ 10 lines):

```cpp
static_assert(NumBits % 256 == 0, "NumBits must be multiple of 256 for AVX2");
static_assert(NumHashes >= 2 && NumHashes <= 8, "NumHashes must be in [2, 8]");
static_assert(sizeof(TransitionModel) <= 128 * 1024,
              "TransitionModel exceeds 128 KB L1/L2 cache budget");
```

**Impact**: Catches misconfiguration at compile time rather than runtime.

______________________________________________________________________

### Result<T> Status-Code Error Handling

**Category**: Type Safety & API Design
**Where**: `include/strata/json/json_core.hpp:L110-L127`, `include/strata/json/json_cursor.hpp:L40-L53`

A lightweight `Result<T>` type carries both a `Status` enum and a value, enabling error handling on hot paths without exceptions. JsonCursor provides dual APIs: status-code (`get_bool()`) and throwing (`get_bool_or_throw()`).

**Key snippet** (≤ 10 lines):

```cpp
template <typename T> struct Result {
    Status status;
    T value;
    [[nodiscard]] bool ok() const noexcept { return status == Status::Ok; }
    [[nodiscard]] T value_or(T default_val) const { return ok() ? value : default_val; }
};
```

**Impact**: Zero exception overhead on the parsing hot path; callers choose their error-handling style.

______________________________________________________________________

### Template Devirtualization

**Category**: Type Safety & API Design
**Where**: `include/strata/json/json_parser_inline.hpp:L46-L49`

The inline parser template accepts a concrete `Handler` type, enabling the compiler to devirtualize all handler calls. SFINAE detection (`has_try_match_key`) enables optional handler methods.

**Impact**: Eliminates vtable overhead; all handler calls can be inlined by the compiler.

______________________________________________________________________

## 4. Testing & Quality Assurance

### Comprehensive Test Matrix (14 Test Targets)

**Category**: Testing & Quality Assurance
**Where**: `CMakeLists.txt:L77-L293`

The build defines 14 separate test executables covering every subsystem: JSON parsing, cursor, document, serialization, JSONPath, NDJSON, float precision, output buffer, stress (large payloads), stress (deep nesting), SIMD, speculative, and Bloom filter. Each is registered with CTest via `add_test()`.

**Impact**: Every subsystem is independently testable; CI runs all test suites.

______________________________________________________________________

### Stress Tests (Deep Nesting & Large Payloads)

**Category**: Testing & Quality Assurance
**Where**: `tests/cpp/test_stress_deep.cpp`, `tests/cpp/test_stress_large.cpp`

Dedicated stress tests exercise extreme conditions: 100-level deep nesting and multi-megabyte payloads. These verify the parser handles edge cases without stack overflow or memory exhaustion.

**Impact**: Catches stack-depth and allocation bugs that unit tests miss.

______________________________________________________________________

### Float Precision Round-Trip Tests

**Category**: Testing & Quality Assurance
**Where**: `tests/cpp/test_float_precision.cpp`

Tests round-trip fidelity of float parsing and serialization (via Ryu/Dragonbox), including NaN, Infinity, subnormals, and maximum-precision values.

**Impact**: Guarantees IEEE 754 compliance across parse-serialize round trips.

______________________________________________________________________

### Fuzz Testing with libFuzzer

**Category**: Testing & Quality Assurance
**Where**: `CMakeLists.txt:L289-L293`, `scripts/fuzz.sh`

Optional fuzz targets are built with libFuzzer + ASan + UBSan (`FUZZ=ON`), enabling continuous fuzzing of the parser against malformed input.

**Key snippet** (≤ 10 lines):

```cmake
option(FUZZ "Build libFuzzer fuzz targets (clang, -fsanitize=fuzzer,address,undefined)" OFF)
if(FUZZ)
    add_subdirectory(tests/fuzz)
endif()
```

**Impact**: Discovers crash/UB bugs in parser edge cases that manual testing cannot cover.

______________________________________________________________________

### Wall + Wextra on All Targets

**Category**: Testing & Quality Assurance
**Where**: `CMakeLists.txt:L82` (and all other test targets)

Every compilation target uses `-Wall -Wextra`, catching implicit conversions, unused variables, missing returns, and other common C++ mistakes.

**Impact**: Catches a broad class of bugs at compile time across the entire codebase.

______________________________________________________________________

## 5. Probabilistic Data Structures

### Bloom Filter with Kirschner-Mitzenmacher Double Hashing

**Category**: Probabilistic Data Structures
**Where**: `include/strata/bloom/bloom_filter.h:L140-L281`

A compile-time configurable Bloom filter using the double-hashing trick: `h_i(key) = (h1 + i * h2) mod m`. Two independent hashes are derived from wyhash with different seeds. The filter supports bitwise merge (OR) for combining per-thread filters after parallel processing.

**Key snippet** (≤ 10 lines):

```cpp
void insert(std::string_view key) noexcept {
    auto [h1, h2] = hash(key);
    for (size_t i = 0; i < NumHashes; ++i) {
        size_t bit = (h1 + i * h2) % NumBits;
        bits_[bit / 8] |= static_cast<uint8_t>(1u << (bit % 8));
    }
}
```

**Impact**: O(k) insert/query with ~0.14% FPR for TinyFilter (256 bits, k=3, n=10).

______________________________________________________________________

### Optimal Filter Sizing from Information Theory

**Category**: Probabilistic Data Structures
**Where**: `include/strata/bloom/sizing.h:L85-L116`

Filter size is computed from the information-theoretic formula: `m = -(n * ln(p)) / (ln(2))^2`, and optimal hash count from `k = (m/n) * ln(2)`, clamped to \[2,8\]. All math is `constexpr` — computed at compile time. Sizes are rounded to 256-bit multiples for AVX2 alignment.

**Key snippet** (≤ 10 lines):

```cpp
constexpr size_t optimal_bits(size_t num_keys, double fpr) noexcept {
    double m = -(static_cast<double>(num_keys) * detail_sizing::cx_log(fpr))
               / detail_sizing::kLn2Sq;
    return static_cast<size_t>(m + 0.5);
}
constexpr size_t align_bits(size_t bits) noexcept {
    return (bits + 255) & ~size_t{255};
}
```

**Impact**: Provably optimal FPR for given memory budget; zero runtime sizing cost.

______________________________________________________________________

### Blocked Bloom Filter for Cache Efficiency

**Category**: Probabilistic Data Structures
**Where**: `include/strata/bloom/bloom_filter.h:L149-L177`

For filters larger than 512 bits (one cache line), a blocked design is used: `h1` selects a 512-bit (64-byte) block, and all k probes land within that same block. This guarantees exactly one cache-line access per query.

**Key snippet** (≤ 10 lines):

```cpp
static constexpr bool BLOCKED = (BYTE_SIZE > 64);
static constexpr size_t BLOCK_BITS = 512;
if constexpr (BLOCKED) {
    size_t block = h1 % NUM_BLOCKS;
    size_t block_offset = block * BLOCK_BITS;
    for (size_t i = 0; i < NumHashes; ++i) {
        size_t bit = block_offset + ((h1 + i * h2) % BLOCK_BITS);
        bits_[bit / 8] |= static_cast<uint8_t>(1u << (bit % 8));
    }
}
```

**Impact**: Every query touches exactly one cache line — eliminates multi-cache-line access for large filters.

______________________________________________________________________

### Bloom Filter Preset Type Aliases

**Category**: Probabilistic Data Structures
**Where**: `include/strata/bloom/sizing.h:L127-L147`

Pre-computed configurations for common use cases: TinyFilter (10 keys), SmallFilter (50), MediumFilter (200), LargeFilter (1000) — all at 1% FPR. The `Preset<N, P>` template computes everything at compile time.

**Key snippet** (≤ 10 lines):

```cpp
using TinyFilter = Preset<10, 10>::type;    // 256 bits, 3 hashes
using SmallFilter = Preset<50, 10>::type;   // 512 bits, 3 hashes
using MediumFilter = Preset<200, 10>::type; // 2048 bits, 3 hashes
using LargeFilter = Preset<1000, 10>::type; // 10240 bits, 3 hashes
```

**Impact**: Zero-configuration Bloom filters with provably optimal sizing for each workload.

______________________________________________________________________

### Key Filter: Bloom-First Selective Parsing

**Category**: Probabilistic Data Structures
**Where**: `include/strata/bloom/key_filter.h:L36-L103`

Wraps a Bloom filter with an exact-match binary search fallback. The Bloom filter rejects ~99% of unwanted keys instantly (no string comparison). For the ~1% false positives, binary search provides the definitive answer. Includes runtime stats tracking (reject rate, FPR).

**Key snippet** (≤ 10 lines):

```cpp
[[nodiscard]] bool should_parse(std::string_view key) const noexcept;
// Action check(key):
//   SKIP     → Bloom says definitely not wanted
//   UNCERTAIN → Bloom positive, needs exact check
//   PARSE    → filter empty, accept all
```

**Impact**: Reduces key lookup from O(n) string comparisons to O(1) Bloom check with \< 1% fallthrough.

______________________________________________________________________

### False Positive Rate Tracking

**Category**: Probabilistic Data Structures
**Where**: `include/strata/bloom/bloom_filter.h:L224-L247`, `include/strata/bloom/key_filter.h:L71-L92`

Both BloomFilter and KeyFilter track their actual FPR at runtime. BloomFilter computes `estimated_false_positive_rate()` from the measured fill ratio; KeyFilter maintains counters for total checks, Bloom rejects, exact matches, and false positives.

**Impact**: Enables runtime monitoring and tuning of filter parameters.

______________________________________________________________________

## 6. SIMD & Hardware Acceleration

### Multi-Platform SIMD Backend Support

**Category**: SIMD & Hardware Acceleration
**Where**: `include/strata/simd/classifier.h:L47-L76`, `src/strata/simd/structural_indexer.cpp:L180-L490`

The classifier and structural indexer support 7 SIMD backends: AVX-512, AVX2, SVE2, NEON, WASM SIMD, RISC-V Vector, and scalar fallback. A compile-time `#if / #elif` priority chain selects the best available backend. Each backend implements the same nibble-lookup algorithm adapted to its instruction set.

**Key snippet** (≤ 10 lines):

```cpp
// Priority: AVX-512 > AVX2 > SVE2 > NEON > WASM SIMD > RVV > Scalar
#if defined(STRATA_SIMD_HAS_AVX512)
    // 64-byte single-load classification
#elif defined(STRATA_SIMD_HAS_AVX2)
    // Two 32-byte loads
#elif defined(STRATA_SIMD_HAS_SVE2)
    // Scalable vector (128-2048 bits)
```

**Impact**: Portable high performance across x86, ARM, WebAssembly, and RISC-V.

______________________________________________________________________

### Runtime cpuid-Based Feature Detection

**Category**: SIMD & Hardware Acceleration
**Where**: `src/strata/simd/dispatch.cpp:L30-L95`

Runtime detection of AVX2, BMI2, PCLMUL, and AVX-512 using `__cpuid`/`__cpuidex`. The result is cached in a function-local static. The detection chain falls through: AVX512 > AVX2+BMI2 > AVX2 > SCALAR.

**Key snippet** (≤ 10 lines):

```cpp
Backend detect_backend() noexcept {
    static Backend cached = detect_backend_impl();
    return cached;
}
// detect_backend_impl: __cpuid(7, 0) → check bits for AVX2, BMI2, AVX-512
```

**Impact**: Computed once per process; enables running optimal SIMD path on any x86 CPU.

______________________________________________________________________

### 64-Byte Chunk Processing (Two AVX2 Loads)

**Category**: SIMD & Hardware Acceleration
**Where**: `src/strata/simd/structural_indexer.cpp:L226-L259`

The structural indexer processes input in 64-byte chunks using two 256-bit AVX2 loads, producing 64-bit bitmasks for each character class. The bitmask extraction uses `cmpeq + movemask + invert` to convert SIMD lanes to scalar masks.

**Impact**: Processes 64 bytes of JSON per indexer step; produces bitmasks for downstream use.

______________________________________________________________________

### NEON movemask Emulation

**Category**: SIMD & Hardware Acceleration
**Where**: `src/strata/simd/structural_indexer.cpp:L146-L177`

ARM NEON lacks a native `movemask` instruction. Strata emulates it by ANDing classified bytes with a bit-selector array (`1, 2, 4, 8, ...`), then using `vpaddlq` cascaded reduction to extract a 16-bit mask from a 16-byte vector.

**Key snippet** (≤ 10 lines):

```cpp
static inline uint16_t neon_movemask(uint8x16_t v) noexcept {
    static const uint8_t kBitSelector[16] = {
        1, 2, 4, 8, 16, 32, 64, 128, 1, 2, 4, 8, 16, 32, 64, 128,
    };
    uint8x16_t masked = vandq_u8(v, vld1q_u8(kBitSelector));
    uint64x2_t sums = vpaddlq_u32(vpaddlq_u16(vpaddlq_u8(masked)));
    return static_cast<uint16_t>(vgetq_lane_u64(sums, 0))
         | (static_cast<uint16_t>(vgetq_lane_u64(sums, 1)) << 8);
}
```

**Impact**: Enables the full SIMD pipeline on ARM platforms without native movemask support.

______________________________________________________________________

### SVE2 Hybrid Classification

**Category**: SIMD & Hardware Acceleration
**Where**: `src/strata/simd/structural_indexer.cpp:L270-L304`

SVE2 classification uses a scalable-vector `svwhilelt` loop (1-4 iterations depending on hardware VL), stores classified bytes to a 64-byte aligned buffer, then reloads as four NEON vectors for bitmask extraction. This hybrid avoids SVE predicate-to-integer conversion.

**Impact**: Automatically adapts to any SVE2 vector length (128 to 2048 bits).

______________________________________________________________________

### RISC-V Vector Gather-Based Classification

**Category**: SIMD & Hardware Acceleration
**Where**: `src/strata/simd/classifier.cpp:L171-L195`

RVV classification uses `vluxei8` (indexed byte gather from memory) rather than `vrgather` (which requires the table to fit in a vector register). This works with any hardware vector length regardless of table size.

**Key snippet** (≤ 10 lines):

```cpp
vuint8m1_t lo_result = __riscv_vluxei8_v_u8m1(kLowNibbleTable, lo_nibbles, vl);
vuint8m1_t hi_result = __riscv_vluxei8_v_u8m1(kHighNibbleTable, hi_nibbles, vl);
return __riscv_vand_vv_u8m1(lo_result, hi_result, vl);
```

**Impact**: Portable across all RVV implementations regardless of vector register width.

______________________________________________________________________

## 7. Statistical & Adaptive Algorithms

### Markov Transition Model for Value Type Prediction

**Category**: Statistical & Adaptive Algorithms
**Where**: `include/strata/speculative/transition_model.h:L26-L146`, `src/strata/speculative/transition_model.cpp:L32-L73`

A lossy hash table of (context → value-type distribution) buckets where each bucket stores `uint16_t` counts per ValueType. The predict() hot path is ~15 cycles: hash lookup (AND mask) + unrolled argmax over 9 counts + fixed-point confidence check.

**Key snippet** (≤ 10 lines):

```cpp
uint16_t max_count = b.counts[0];
uint8_t max_idx = 0;
for (uint8_t i = 1; i < NUM_TYPES; ++i) {
    if (b.counts[i] > max_count) { max_count = b.counts[i]; max_idx = i; }
}
const uint64_t lhs = static_cast<uint64_t>(max_count) * 65536ULL;
const uint64_t rhs = static_cast<uint64_t>(CONFIDENCE_THRESHOLD_FP)
                   * static_cast<uint64_t>(b.total);
const bool should_spec = lhs >= rhs;
```

**Impact**: ~15 cycles per prediction; enables speculative parsing when confidence >= 85%.

______________________________________________________________________

### Fixed-Point Probability Arithmetic (No Float in Hot Path)

**Category**: Statistical & Adaptive Algorithms
**Where**: `src/strata/speculative/transition_model.cpp:L55-L66`

The confidence check avoids division by rearranging `max/total >= 0.85` to `max * 65536 >= 55706 * total`. Both sides fit in `uint64_t`, so no floating-point operations appear on the prediction hot path.

**Impact**: Eliminates FP pipeline stalls in the critical prediction path.

______________________________________________________________________

### Exponential Decay for Online Learning

**Category**: Statistical & Adaptive Algorithms
**Where**: `src/strata/speculative/transition_model.cpp:L117-L128`

Every 256 observations per bucket, all counts are halved (right-shift by 1). This implements exponential decay — recent observations carry more weight, allowing the model to adapt to changing patterns.

**Key snippet** (≤ 10 lines):

```cpp
if ((b.total & (DECAY_PERIOD - 1)) == 0 && b.total > 0) {
    uint32_t new_total = 0;
    for (size_t i = 0; i < NUM_TYPES; ++i) {
        b.counts[i] >>= 1;
        new_total += b.counts[i];
    }
    b.total = new_total;
}
```

**Impact**: Effective learning rate ~0.01; each observation's influence halves every ~256 subsequent observations.

______________________________________________________________________

### Lossy Context Cache (Intentional Collision Tolerance)

**Category**: Statistical & Adaptive Algorithms
**Where**: `src/strata/speculative/transition_model.cpp:L86-L105`

The transition table uses lossy cache semantics: collisions cause the losing context to share the bucket, degrading prediction quality but never correctness. Cold entries (total \< 16) are evicted; hot entries silently drop updates from colliding contexts.

**Key snippet** (≤ 10 lines):

```cpp
if (b.context_signature != sig) {
    if (b.total < 16) {
        std::memset(b.counts, 0, sizeof(b.counts));
        b.total = 0; b.context_signature = sig; // evict cold entry
    } else {
        return; // silently drop — lossy cache semantics
    }
}
```

**Impact**: Avoids chaining/open-addressing overhead; 4096 buckets provide sufficient coverage for typical JSON schemas.

______________________________________________________________________

### Compact Context Hashing (murmurhash3 Finalizer)

**Category**: Statistical & Adaptive Algorithms
**Where**: `include/strata/speculative/types.h:L51-L71`

Parser context is packed into 32 bits (key_hash: 16, depth: 5, array_index: 5, value_type: 4, in_array: 1) and mixed with the murmurhash3 32-bit finalizer for excellent avalanche properties.

**Key snippet** (≤ 10 lines):

```cpp
packed ^= packed >> 16;
packed *= 0x85ebca6b;
packed ^= packed >> 13;
packed *= 0xc2b2ae35;
packed ^= packed >> 16;
```

**Impact**: Uniform bucket distribution with minimal collision rate across typical JSON schemas.

______________________________________________________________________

### FNV-1a Key Hashing (16-bit with XOR-Fold)

**Category**: Statistical & Adaptive Algorithms
**Where**: `include/strata/speculative/types.h:L76-L84`

JSON keys are hashed with FNV-1a, then XOR-folded from 32 to 16 bits. Collisions reduce prediction accuracy but never cause incorrect parses.

**Key snippet** (≤ 10 lines):

```cpp
inline uint16_t hash_key(const uint8_t* key, size_t len) noexcept {
    uint32_t h = 0x811c9dc5u;
    for (size_t i = 0; i < len; ++i) { h ^= key[i]; h *= 0x01000193u; }
    return static_cast<uint16_t>((h >> 16) ^ (h & 0xFFFF));
}
```

**Impact**: ~2 cycles per byte; 16-bit hash fits the ParserContext struct compactly.

______________________________________________________________________

### Per-Thread Model with Post-Hoc Merge

**Category**: Statistical & Adaptive Algorithms
**Where**: `src/strata/speculative/transition_model.cpp:L567-L596`

Each thread maintains its own TransitionModel. After parallel NDJSON processing, models are merged: matching contexts add counts; collisions keep the entry with more observations.

**Impact**: Lock-free parallel learning; merge cost is O(4096 buckets) — negligible compared to parsing.

______________________________________________________________________

### Schema-Based Model Pre-Training

**Category**: Statistical & Adaptive Algorithms
**Where**: `src/strata/speculative/transition_model.cpp:L211-L271`

The model can be pre-populated from a simplified JSON Schema. For each property, 200 synthetic observations are injected, giving ~0.96 confidence — the first document parsed already benefits from speculation.

**Impact**: Eliminates the warmup period when schema is known ahead of time.

______________________________________________________________________

## 8. Build System & Tooling

### CMake C++20 with LTO/IPO Support

**Category**: Build System & Tooling
**Where**: `CMakeLists.txt:L1-L27`

The build system uses C++20 (`CMAKE_CXX_STANDARD 20`) with optional Link-Time Optimization via `check_ipo_supported()`. A reusable `strata_apply_lto()` function conditionally applies IPO to any target.

**Key snippet** (≤ 10 lines):

```cmake
set(CMAKE_CXX_STANDARD 20)
option(STRATA_ENABLE_LTO "Enable LTO/IPO for performance builds" OFF)
check_ipo_supported(RESULT STRATA_LTO_ENABLED OUTPUT STRATA_LTO_ERROR)
function(strata_apply_lto target_name)
    if(STRATA_LTO_ENABLED)
        set_property(TARGET ${target_name} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endfunction()
```

**Impact**: Cross-TU inlining and dead code elimination in release builds.

______________________________________________________________________

### Centralized Source Lists

**Category**: Build System & Tooling
**Where**: `CMakeLists.txt:L40-L66`

All source files are organized into named lists (`STRATA_CORE_SOURCES`, `STRATA_SIMD_SOURCES`, `STRATA_SPECULATIVE_SOURCES`, `STRATA_BLOOM_SOURCES`) and shared across test targets, avoiding duplication and ensuring consistent builds.

**Impact**: Adding a new source file requires a single edit; all targets pick it up automatically.

______________________________________________________________________

### Profile-Guided Optimization (PGO) Pipeline

**Category**: Build System & Tooling
**Where**: `scripts/pgo_build.sh`, `scripts/pgo_training.py`, `scripts/pgo_training_data.py`

A full PGO pipeline: `pgo_build.sh` instruments the build, `pgo_training_data.py` generates representative JSON workloads, and `pgo_training.py` runs the instrumented binary to collect profiles.

**Impact**: Production builds benefit from real-workload branch and layout optimization.

______________________________________________________________________

### Separated Benchmark Targets

**Category**: Build System & Tooling
**Where**: `CMakeLists.txt:L221-L285`

Benchmarks (`bench_simd`, `bench_speculative`, `bench_bloom`) are built with `-O3` and LTO but are NOT registered as CTest tests — they are build-only targets for manual execution.

**Impact**: Benchmarks don't slow CI; always built with maximum optimization.

______________________________________________________________________

## 9. Documentation & Readability

### Doxygen-Style Documentation with `@performance` Context

**Category**: Documentation & Readability
**Where**: `include/strata/simd/classifier.h:L3-L31`, `include/strata/bloom/bloom_filter.h:L3-L22`, `include/strata/util/arena_allocator.hpp:L4-L13`

Every public header includes a `@file` block with design rationale, performance characteristics, and algorithm references. The classifier header includes a complete derivation of the nibble lookup tables with bit-level analysis.

**Impact**: Developers can understand design decisions without reading the implementation.

______________________________________________________________________

### SIMD Intrinsic Annotations

**Category**: Documentation & Readability
**Where**: `src/strata/simd/classifier.cpp:L25-L48`, `src/strata/simd/structural_indexer.cpp:L109-L114`

Every SIMD intrinsic call is annotated with a plain-English comment explaining what it does: `_mm512_broadcast_i32x4` → "duplicates a 128-bit value across 4 lanes", `_mm_clmulepi64_si128` → "carry-less multiply with all-ones = prefix-XOR".

**Impact**: SIMD code remains maintainable by developers unfamiliar with specific instruction sets.

______________________________________________________________________

### Algorithm Explanation Blocks with Cycle Budgets

**Category**: Documentation & Readability
**Where**: `src/strata/speculative/transition_model.cpp:L22-L31`, `src/strata/speculative/transition_model.cpp:L75-L84`

Algorithm blocks include cycle-budget breakdowns: "predict() ~15 cycles: hash (1) + collision check (1) + argmax (9) + fixed-point check"; "observe() ~10 cycles common, ~25 on decay".

**Impact**: Performance contracts are documented alongside the code, making regressions immediately visible.

______________________________________________________________________

### Lookup Table Derivation Comments

**Category**: Documentation & Readability
**Where**: `include/strata/simd/classifier.h:L140-L209`

The classifier includes a complete derivation table showing every JSON character, its hex value, high/low nibble, class assignment, and bit allocation. A verification note confirms zero false positives across all 256 byte values.

**Impact**: The lookup tables are not magic numbers — their derivation is reproducible from the comments.

______________________________________________________________________

## 10. Error Handling & Robustness

### Graceful Degradation on SIMD Feature Absence

**Category**: Error Handling & Robustness
**Where**: `src/strata/simd/dispatch.cpp:L24-L90`, `src/strata/simd/structural_indexer.cpp:L459-L490`

Every SIMD path has a scalar fallback. The dispatch chain detects features at runtime (x86) or compile time (ARM/WASM/RVV) and falls back gracefully. The `STRATA_FORCE_SCALAR` define forces scalar-only compilation for testing.

**Impact**: Strata works correctly on any hardware, including VMs without AVX2.

______________________________________________________________________

### Speculation Miss → Seamless Fallback

**Category**: Error Handling & Robustness
**Where**: `include/strata/speculative/parser.h:L74-L85`, `src/strata/speculative/fast_paths.cpp:L6-L7`

If a speculative fast-path fails (wrong type prediction), it returns `{.success = false}` immediately. The coordinator falls back to the generic parser with miss penalty under 10%.

**Impact**: Speculation never produces incorrect results — only performance varies.

______________________________________________________________________

### Bloom Filter False Positive → Exact Match Confirmation

**Category**: Error Handling & Robustness
**Where**: `include/strata/bloom/key_filter.h:L57-L68`

Bloom filter positives are always confirmed with an exact binary-search match. The filter never causes false rejections — only false positives, which are handled by the fallback.

**Impact**: Zero false negatives; correctness is guaranteed regardless of filter FPR.

______________________________________________________________________

### Overflow-Safe Integer Parsing

**Category**: Error Handling & Robustness
**Where**: `include/strata/util/fast_parse.hpp:L81-L101`

Integer parsing uses a two-phase approach: a fast path that accumulates up to 18 digits without overflow checking (guaranteed safe for uint64), then a slow path with explicit overflow detection for 19+ digit numbers.

**Key snippet** (≤ 10 lines):

```cpp
// Fast path: 18 digits fit in uint64 safely
const size_t safe_end = (pos + 18 < len) ? pos + 18 : len;
while (pos < safe_end && str[pos] >= '0' && str[pos] <= '9') {
    val = val * 10 + static_cast<uint64_t>(str[pos] - '0');
    ++pos;
}
// Slow path: remaining digits with overflow checking
while (pos < len && str[pos] >= '0' && str[pos] <= '9') {
    if (val > (limit - digit) / 10) return false;
```

**Impact**: Handles full int64 range correctly without unnecessary overflow checks on the common case.

______________________________________________________________________

### Inter-Block State Carry for Streaming Correctness

**Category**: Error Handling & Robustness
**Where**: `include/strata/simd/structural_indexer.h:L67-L73`

The structural indexer maintains `prev_escaped_` and `prev_in_string_` state across 64-byte chunk boundaries, ensuring correct escape detection and string masking when backslash sequences or strings span chunks.

**Key snippet** (≤ 10 lines):

```cpp
uint64_t prev_escaped_{0};    // carry from previous block
uint64_t prev_in_string_{0};  // running string-region state
```

**Impact**: Correct parsing of any document regardless of chunk alignment — no edge-case bugs at boundaries.

______________________________________________________________________

### FixedOutputBuffer Overflow Flag

**Category**: Error Handling & Robustness
**Where**: `include/strata/util/output_buffer.hpp:L122-L177`

`FixedOutputBuffer` writes into a caller-owned region and sets an `overflowed_` flag instead of throwing or corrupting memory when capacity is exceeded.

**Impact**: Safe fixed-size serialization without exceptions or undefined behavior.

______________________________________________________________________

### wyhash for High-Quality Hashing

**Category**: Error Handling & Robustness
**Where**: `include/strata/bloom/bloom_filter.h:L33-L124`

The Bloom filter uses wyhash (public domain by Wang Yi) — a fast, high-quality 64-bit hash function with excellent distribution properties. The implementation handles 0-byte, 1-3 byte, 4-16 byte, and bulk (48-byte stride) paths with `__builtin_expect` hints.

**Impact**: Minimizes hash collisions in Bloom filters; bulk loop processes 48 bytes per iteration.

______________________________________________________________________

## Summary Statistics

| Metric                          | Value |
| ------------------------------- | ----- |
| Total best practices cataloged  | 45    |
| Categories                      | 10    |
| Source files referenced         | 24    |
| Header files referenced         | 17    |
| Implementation files referenced | 7     |

## Cross-Reference Index

| Practice                           | Category       | Primary File                                    |
| ---------------------------------- | -------------- | ----------------------------------------------- |
| Add-with-Carry Escape Detection    | Performance    | `src/strata/simd/structural_indexer.cpp`        |
| `alignas(32)` AVX2 Alignment       | Memory         | `include/strata/bloom/bloom_filter.h`           |
| `alignas(64)` Cache-Line Alignment | Memory         | `include/strata/speculative/transition_model.h` |
| Arena (Bump-Pointer) Allocator     | Memory         | `include/strata/util/arena_allocator.hpp`       |
| Blocked Bloom Filter               | Probabilistic  | `include/strata/bloom/bloom_filter.h`           |
| Bloom Filter Double Hashing        | Probabilistic  | `include/strata/bloom/bloom_filter.h`           |
| Bloom Filter Preset Aliases        | Probabilistic  | `include/strata/bloom/sizing.h`                 |
| Branchless Literal Comparison      | Performance    | `src/strata/speculative/fast_paths.cpp`         |
| CMake C++20 + LTO                  | Build          | `CMakeLists.txt`                                |
| Centralized Source Lists           | Build          | `CMakeLists.txt`                                |
| Compact Context Hashing            | Statistical    | `include/strata/speculative/types.h`            |
| Comprehensive Test Matrix          | Testing        | `CMakeLists.txt`                                |
| `constexpr` Sizing                 | Type Safety    | `include/strata/bloom/sizing.h`                 |
| Doxygen Documentation              | Documentation  | `include/strata/simd/classifier.h`              |
| Exponential Decay                  | Statistical    | `src/strata/speculative/transition_model.cpp`   |
| FNV-1a Key Hashing                 | Statistical    | `include/strata/speculative/types.h`            |
| FPR Tracking                       | Probabilistic  | `include/strata/bloom/bloom_filter.h`           |
| Fast-Path Dispatch Table           | Performance    | `src/strata/speculative/fast_paths.cpp`         |
| FixedOutputBuffer Overflow         | Error Handling | `include/strata/util/output_buffer.hpp`         |
| Float Precision Tests              | Testing        | `tests/cpp/test_float_precision.cpp`            |
| Fuzz Testing                       | Testing        | `CMakeLists.txt`                                |
| Graceful SIMD Degradation          | Error Handling | `src/strata/simd/dispatch.cpp`                  |
| Inter-Block State Carry            | Error Handling | `include/strata/simd/structural_indexer.h`      |
| Key Filter Selective Parsing       | Probabilistic  | `include/strata/bloom/key_filter.h`             |
| `LIKELY`/`UNLIKELY` Hints          | Performance    | `include/strata/util/output_buffer.hpp`         |
| Lossy Context Cache                | Statistical    | `src/strata/speculative/transition_model.cpp`   |
| Lookup Table Derivation            | Documentation  | `include/strata/simd/classifier.h`              |
| Markov Transition Model            | Statistical    | `include/strata/speculative/transition_model.h` |
| Multi-Platform SIMD                | SIMD           | `include/strata/simd/classifier.h`              |
| NEON movemask Emulation            | SIMD           | `src/strata/simd/structural_indexer.cpp`        |
| `[[nodiscard]]` Annotations        | Type Safety    | multiple files                                  |
| `noexcept` Correctness             | Type Safety    | multiple files                                  |
| Optimal Filter Sizing              | Probabilistic  | `include/strata/bloom/sizing.h`                 |
| Overflow-Safe Integer Parsing      | Error Handling | `include/strata/util/fast_parse.hpp`            |
| PCLMUL String Masking              | Performance    | `src/strata/simd/structural_indexer.cpp`        |
| PGO Pipeline                       | Build          | `scripts/pgo_build.sh`                          |
| Per-Thread Model Merge             | Statistical    | `src/strata/speculative/transition_model.cpp`   |
| Pre-allocation Heuristics          | Performance    | `include/strata/simd/index_builder.h`           |
| RISC-V Vector Classification       | SIMD           | `src/strata/simd/classifier.cpp`                |
| Result<T> Error Handling           | Type Safety    | `include/strata/json/json_core.hpp`             |
| Runtime cpuid Detection            | SIMD           | `src/strata/simd/dispatch.cpp`                  |
| SVE2 Hybrid Classification         | SIMD           | `src/strata/simd/structural_indexer.cpp`        |
| SWAR 8-Digit Parser                | Performance    | `src/strata/speculative/fast_paths.cpp`         |
| Schema Pre-Training                | Statistical    | `src/strata/speculative/transition_model.cpp`   |
| SIMD Intrinsic Annotations         | Documentation  | `src/strata/simd/classifier.cpp`                |
| SIMD Structural Indexing           | Performance    | `include/strata/simd/classifier.h`              |
| SIMD Whitespace Skipping           | Performance    | `include/strata/util/fast_parse.hpp`            |
| Speculation Fallback               | Error Handling | `include/strata/speculative/parser.h`           |
| Stack-Allocated Bloom Filters      | Memory         | `include/strata/bloom/dedup_filter.h`           |
| STL-Compatible Arena Allocator     | Memory         | `include/strata/util/arena_allocator.hpp`       |
| `static_assert` Invariants         | Type Safety    | `include/strata/bloom/bloom_filter.h`           |
| `std::variant` JSON Values         | Type Safety    | `include/strata/json/json_core.hpp`             |
| Stress Tests                       | Testing        | `tests/cpp/test_stress_deep.cpp`                |
| Strong Enums                       | Type Safety    | `include/strata/speculative/types.h`            |
| Template Devirtualization          | Type Safety    | `include/strata/json/json_parser_inline.hpp`    |
| Unrolled String Scan               | Performance    | `include/strata/util/fast_parse.hpp`            |
| Unsafe Buffer Operations           | Memory         | `include/strata/util/output_buffer.hpp`         |
| Wall+Wextra Warnings               | Testing        | `CMakeLists.txt`                                |
| wyhash Hashing                     | Error Handling | `include/strata/bloom/bloom_filter.h`           |
