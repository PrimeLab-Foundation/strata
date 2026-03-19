# Speculative Subsystem API Reference

The Speculative subsystem implements a Markov-model-based prediction engine for JSON parsing. It predicts the next value type (integer, float, string, bool, null, object, array) based on structural context, then dispatches to a specialized fast-path parser. When the prediction is correct (a "hit"), generic dispatch overhead is eliminated. When incorrect (a "miss"), the system falls back to the standard parser with minimal penalty.

**Headers:**

- `strata/speculative/types.h`
- `strata/speculative/fast_paths.h`
- `strata/speculative/transition_model.h`
- `strata/speculative/parser.h`

**Namespace:** `strata::speculative`

______________________________________________________________________

## ValueType

Enumeration of the nine value types that the speculative engine predicts.

```cpp
enum class ValueType : uint8_t {
    INTEGER      = 0, // JSON number, no decimal point or exponent
    FLOAT        = 1, // JSON number with '.' or 'e'/'E'
    SHORT_STRING = 2, // string <= 32 bytes, no escapes
    LONG_STRING  = 3, // string > 32 bytes or contains escapes
    BOOL_TRUE    = 4,
    BOOL_FALSE   = 5,
    NULL_VALUE   = 6,
    OBJECT       = 7, // nested {
    ARRAY        = 8, // nested [
    NUM_TYPES    = 9
};
```

______________________________________________________________________

## ParserContext

Encodes the current position in the JSON structure so the transition model can learn position-dependent patterns (e.g., "the 'age' key always maps to an integer").

```cpp
struct ParserContext {
    uint16_t  key_hash;         // hash of current key (0 if in array)
    uint8_t   nesting_depth;    // current depth (capped at 255)
    uint8_t   array_index;      // position in array (capped at 255)
    ValueType last_value_type;  // what was just parsed
    bool      in_array;         // true if parent is array, false if object
};
```

______________________________________________________________________

## context_id

```cpp
inline uint32_t context_id(const ParserContext& ctx) noexcept;
```

Map a `ParserContext` to a compact 32-bit integer used as a hash key for transition table lookup.

Packs the most predictive fields into 32 bits:

- `key_hash`: 16 bits (dominates in objects)
- `nesting_depth`: 5 bits (capped; deep nesting is rare)
- `array_index`: 5 bits (first 32 positions matter most)
- `last_value_type`: 4 bits (9 types fit in 4 bits)
- `in_array`: 1 bit

Applies the murmurhash3 32-bit finalizer for uniform distribution across the bucket space.

- **Parameters:** `ctx` -- The current parser context.
- **Returns:** 32-bit context identifier.
- **Complexity:** O(1). Pure integer arithmetic, no branches.

______________________________________________________________________

## hash_key

```cpp
inline uint16_t hash_key(const uint8_t* key, size_t len) noexcept;
```

FNV-1a 16-bit hash for JSON keys. Collisions are acceptable -- they reduce prediction accuracy for colliding keys but never cause incorrect parses.

- **Parameters:**
  - `key` -- Pointer to the key bytes.
  - `len` -- Length of the key in bytes.
- **Returns:** 16-bit hash value (XOR-folded from 32-bit FNV-1a).
- **Complexity:** O(len).

______________________________________________________________________

## ParseResult

Result returned by every fast-path parser function.

```cpp
struct ParseResult {
    bool      success;          // false = speculation failed, use generic parser
    size_t    bytes_consumed;   // number of bytes consumed from data
    JsonValue value;            // parsed value (valid only if success == true)
};
```

______________________________________________________________________

## FastPaths

Collection of specialized, optimized parsers for each predicted value type. Each fast-path assumes the predicted type and parses without generic dispatch. If the assumption is violated, returns `{.success = false}` immediately so the caller can fall back.

### Constructor

```cpp
explicit FastPaths(strata::util::Arena& arena) noexcept;
```

- **Parameters:** `arena` -- Arena allocator used for string allocation during parsing.

### Methods

#### parse_integer

```cpp
ParseResult parse_integer(const uint8_t* data, size_t remaining) noexcept;
```

Parse a JSON integer using the SWAR (SIMD Within A Register) technique. Loads 8 bytes at a time into a `uint64_t`, subtracts `0x3030303030303030` (ASCII '0' from each byte), checks if any byte exceeds 9 to mark end of digit run, then reduces with SWAR multiplication.

- **Parameters:**
  - `data` -- Pointer to the start of the value.
  - `remaining` -- Number of bytes available from `data` to end of document.
- **Returns:** `ParseResult` with the parsed integer value on success.
- **Throughput target:** 4+ GB/s on integer-heavy JSON.

#### parse_float

```cpp
ParseResult parse_float(const uint8_t* data, size_t remaining) noexcept;
```

Parse a JSON floating-point number. Expects digits with decimal point and/or exponent. Uses Lemire's algorithm via `fast_float` for fast strtod conversion.

#### parse_short_string

```cpp
ParseResult parse_short_string(const uint8_t* data, size_t remaining) noexcept;
```

Parse a short string (32 bytes or fewer, no escape sequences). Uses a single 32-byte SIMD load (or 16-byte on NEON) to scan for closing quote and backslash simultaneously. If no escape is found and the quote is within range, performs a direct copy to the arena.

#### parse_long_string

```cpp
ParseResult parse_long_string(const uint8_t* data, size_t remaining) noexcept;
```

Parse a long string (more than 32 bytes or containing escape sequences). Uses SIMD scan for backslash and quote characters. Processes escape sequences using a lookup table for common escapes (`\n`, `\t`, `\\`, `\"`, etc.).

#### parse_bool_true

```cpp
ParseResult parse_bool_true(const uint8_t* data, size_t remaining) noexcept;
```

Branchless parse of `true` literal. Compares 4 bytes as a single 32-bit integer.

#### parse_bool_false

```cpp
ParseResult parse_bool_false(const uint8_t* data, size_t remaining) noexcept;
```

Branchless parse of `false` literal. Compares 5 bytes as a single 64-bit integer (with masking).

#### parse_null

```cpp
ParseResult parse_null(const uint8_t* data, size_t remaining) noexcept;
```

Branchless parse of `null` literal. Same technique as `parse_bool_true`.

#### parse_object_open / parse_array_open

```cpp
ParseResult parse_object_open(const uint8_t* data, size_t remaining) noexcept;
ParseResult parse_array_open(const uint8_t* data, size_t remaining) noexcept;
```

These do not parse the container -- they confirm the opening character matches the prediction, enabling the coordinator to proceed with recursive descent.

#### dispatch_table

```cpp
static const std::array<ParseFn, NUM_TYPES>& dispatch_table() noexcept;
```

Jump table indexed by `ValueType` for branchless dispatch. OBJECT and ARRAY entries validate the opening character; the coordinator handles recursive descent.

- **Type:** `ParseFn = ParseResult (FastPaths::*)(const uint8_t*, size_t) noexcept`
- **Returns:** Reference to the static dispatch table (9 entries).

______________________________________________________________________

## TransitionModel

Markov transition model that maintains a lossy hash table of (context -> value-type distribution) buckets. Each bucket stores `uint16_t` counts per `ValueType`, enabling fixed-point probability computation without floating-point arithmetic in the hot path.

### Design Constraints

- `predict()` executes in fewer than 20 cycles: no division, no branches in the common path
- `observe()` executes in fewer than 50 cycles: simple increment + periodic decay
- Total model size is less than 128 KB (fits in L1/L2 cache)
- NOT thread-safe: each thread gets its own copy; use `merge()` after parallel processing

### Constants

```cpp
static constexpr size_t MAX_CONTEXTS = 4096;        // 4096 buckets * ~30 bytes ≈ 120 KB
static constexpr float CONFIDENCE_THRESHOLD = 0.85f; // minimum probability to speculate
```

### Prediction

```cpp
struct Prediction {
    ValueType predicted_type;
    float     confidence;       // 0.0 - 1.0
    bool      should_speculate; // confidence >= CONFIDENCE_THRESHOLD
};
```

### Constructor

```cpp
TransitionModel() noexcept;
```

Initializes all buckets to zero counts and resets statistics.

### Methods

#### predict

```cpp
[[nodiscard]] Prediction predict(const ParserContext& ctx) const noexcept;
```

Predict the next value type given the current context. Finds the max count in the bucket using an unrolled loop and compares against the threshold using fixed-point arithmetic. No division -- uses multiply-and-shift.

- **Parameters:** `ctx` -- Current parser context.
- **Returns:** `Prediction` with the predicted type, confidence, and speculation flag.
- **Complexity:** O(1). Constant-time bucket lookup + unrolled max-find over 9 entries.

#### observe

```cpp
void observe(const ParserContext& ctx, ValueType actual) noexcept;
```

Update the model after observing an actual value type. Increments the count for `actual` in the matching bucket. Every 256 observations per bucket, applies exponential decay (right-shift all counts by 1) to adapt to changing patterns.

- **Parameters:**
  - `ctx` -- Context in which the value was observed.
  - `actual` -- The actual value type that was parsed.

#### train_from_schema

```cpp
void train_from_schema(std::string_view schema_json);
```

Pre-populate transition probabilities from a simplified JSON Schema. Recognizes `{"type":"object","properties":{"key":{"type":"..."},...}}` and `{"type":"array","items":{"type":"..."}}`.

- **Parameters:** `schema_json` -- JSON Schema string.

#### train_from_samples

```cpp
void train_from_samples(const uint8_t* data, size_t length, size_t num_docs);
```

Parse sample documents and observe all transitions. `data` may contain `num_docs` newline-delimited JSON documents.

- **Parameters:**
  - `data` -- Raw bytes of the sample documents.
  - `length` -- Total length in bytes.
  - `num_docs` -- Number of newline-delimited documents in `data`.

#### serialize / deserialize

```cpp
[[nodiscard]] std::vector<uint8_t> serialize() const;
static TransitionModel deserialize(const uint8_t* data, size_t length);
```

Serialize the model to a compact binary format for caching trained models. `deserialize` is a static factory that reconstructs a model from the binary data produced by `serialize()`.

#### merge

```cpp
void merge(const TransitionModel& other) noexcept;
```

Merge another model's observations into this one. Used after parallel NDJSON processing to combine per-thread models.

### Stats

```cpp
struct Stats {
    uint64_t total_predictions = 0;
    uint64_t correct_predictions = 0;
    uint64_t speculations_attempted = 0;
    uint64_t speculations_hit = 0;

    double hit_rate() const noexcept;
    double speculation_rate() const noexcept;
};
```

#### stats / reset_stats

```cpp
[[nodiscard]] Stats stats() const noexcept;
void reset_stats() noexcept;
```

Access or reset model statistics. `hit_rate()` returns `correct_predictions / total_predictions`. `speculation_rate()` returns `speculations_hit / speculations_attempted`.

#### Recording methods

```cpp
void record_prediction() noexcept;
void record_correct_prediction() noexcept;
void record_speculation_attempted() noexcept;
void record_speculation_hit() noexcept;
```

Direct stats manipulation used by `SpeculativeParser` to record prediction outcomes.

______________________________________________________________________

## SpeculativeParser

Coordinator that ties together the Markov transition model, fast-path parsers, and the structural index to parse JSON/NDJSON with statistical prediction.

### Parse Flow

1. Look at the next byte in the structural index.
1. Ask `TransitionModel` for a prediction.
1. If confident, dispatch to the predicted fast-path parser.
1. If fast-path succeeds (hit), use the result.
1. If fast-path fails (miss) or model is not confident, use the generic parser.
1. Update the model with the actual observed type (online learning).

### Config

```cpp
struct Config {
    float  confidence_threshold    = 0.85f;
    bool   enable_online_learning  = true;
    bool   enable_speculation      = true;   // false = always generic (for benchmarking)
    size_t online_learning_warmup  = 100;    // observe N values before speculating
};
```

- `confidence_threshold`: Minimum prediction confidence to attempt fast-path dispatch.
- `enable_online_learning`: When true, the model updates after every value is parsed.
- `enable_speculation`: Set to false to always use the generic parser (useful for A/B benchmarking).
- `online_learning_warmup`: Number of values to observe with the generic parser before activating speculation.

### Constructor

```cpp
SpeculativeParser(const Config& config, strata::util::Arena& arena);
```

- **Parameters:**
  - `config` -- Configuration parameters.
  - `arena` -- Arena allocator for parsed value storage.

### Methods

#### parse

```cpp
JsonValue parse(const uint8_t* data, size_t length,
                const uint32_t* structural_positions, size_t num_structural);
```

Parse a complete JSON document using speculative optimization.

- **Parameters:**
  - `data` -- Raw JSON bytes.
  - `length` -- Document length in bytes.
  - `structural_positions` -- Output from SIMD `IndexBuilder` (byte offsets of structural chars).
  - `num_structural` -- Number of entries in `structural_positions`.
- **Returns:** Root `JsonValue` (allocated on stack/arena as needed).

#### parse_ndjson

```cpp
std::vector<JsonValue> parse_ndjson(const uint8_t* data, size_t length,
                                     const uint32_t* structural_positions,
                                     size_t num_structural);
```

Parse NDJSON: multiple newline-delimited JSON documents. The model learns across documents, improving predictions over time.

- **Parameters:** Same as `parse`.
- **Returns:** Vector of `JsonValue`, one per NDJSON line.

#### model

```cpp
[[nodiscard]] const TransitionModel& model() const noexcept;
[[nodiscard]] TransitionModel& model() noexcept;
```

Access the underlying transition model for serialization, statistics, or merging.

#### set_key_filter

```cpp
void set_key_filter(const bloom::KeyFilter* filter) noexcept;
```

Set an optional key filter for selective parsing. When set, the parser skips values of keys not in the filter. The filter pointer must remain valid for the lifetime of the parse call. Pass `nullptr` to disable filtering.

______________________________________________________________________

## Usage Example

```cpp
#include <strata/simd/dispatch.h>
#include <strata/speculative/parser.h>
#include <strata/util/arena_allocator.hpp>

int main() {
    const char* json = R"({"name":"Alice","age":30,"active":true})";
    auto len = std::strlen(json);
    auto data = reinterpret_cast<const uint8_t*>(json);

    // 1. Build structural index
    auto index = strata::simd::index_document(data, len);

    // 2. Configure speculative parser
    strata::util::Arena arena;
    strata::speculative::SpeculativeParser::Config config;
    config.confidence_threshold = 0.85f;
    config.enable_online_learning = true;

    strata::speculative::SpeculativeParser parser(config, arena);

    // 3. Parse
    auto root = parser.parse(data, len,
                             index.positions.data(),
                             index.positions.size());

    // 4. Check prediction stats
    auto stats = parser.model().stats();
    std::printf("Hit rate: %.1f%%\n", stats.hit_rate() * 100.0);
    std::printf("Speculation rate: %.1f%%\n", stats.speculation_rate() * 100.0);
}
```
