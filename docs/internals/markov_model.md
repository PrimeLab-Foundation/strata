# Markov Transition Model Internals

The speculative parsing engine predicts the next JSON value type before
reading a single byte. When the prediction is correct (>85% of the time for
schema-stable workloads), the parser skips generic dispatch and jumps directly
to a specialized fast path. This document describes the data structures and
arithmetic behind the transition model.

**Source files:**

- `include/strata/speculative/transition_model.h`
- `src/strata/speculative/transition_model.cpp`
- `include/strata/speculative/types.h`

______________________________________________________________________

## 1. Lossy Hash Table Design

The transition table is a flat array of 4096 buckets. Each bucket stores
per-type observation counts for a single parser context.

### Memory layout

```
  struct ContextBucket {
      uint16_t counts[9];       // 18 bytes  (one per ValueType)
      uint32_t total;           //  4 bytes
      uint32_t context_signature; // 4 bytes
  };                            // 26 bytes per bucket, padded to 30

  Total: 4096 * 30 = 122,880 bytes ~ 120 KB
```

The table is declared with `alignas(64)` so that adjacent buckets share cache
lines efficiently. At 120 KB, the entire model fits comfortably in L2 cache
(typically 256 KB or more per core).

### Collision handling -- lossy semantics

The table uses open-addressing with no chaining. When two contexts hash to
the same bucket:

1. If the existing bucket is "cold" (total \< 16 observations), the new
   context evicts it -- reset counts, claim the bucket.
1. If the existing bucket is "hot" (total >= 16), the new observation is
   silently dropped.

This means collisions degrade prediction accuracy for the colliding contexts
but never cause an incorrect parse. The speculative parser always validates
its assumption after the fact; a wrong prediction simply falls back to the
generic path.

### Size check

A `static_assert` in the header enforces the 128 KB budget:

```cpp
static_assert(sizeof(TransitionModel) <= 128 * 1024,
              "TransitionModel exceeds 128 KB L1/L2 cache budget");
```

______________________________________________________________________

## 2. Context ID Packing

A `ParserContext` captures "where we are" in the JSON structure:

```cpp
struct ParserContext {
    uint16_t key_hash;         // FNV-1a hash of current key (0 in arrays)
    uint8_t  nesting_depth;    // current depth (0-255)
    uint8_t  array_index;      // position within array (0-255)
    ValueType last_value_type; // what we just parsed (9 types, fits in 4 bits)
    bool     in_array;         // parent is array vs object
};
```

The `context_id()` function packs these fields into a 32-bit integer and
applies the murmurhash3 finalizer for uniform distribution.

### Bit packing layout

```
  Bits 31  30  29-26    25-21        20-16           15-0
       |   |   |         |            |               |
       |   |   type(4b)  array_idx   nesting_depth    key_hash(16b)
       |   |             (5b)        (5b)
       |   in_array(1b)
       (unused)
```

Total: 16 + 5 + 5 + 4 + 1 = 31 bits packed into a `uint32_t`.

```cpp
uint32_t packed = static_cast<uint32_t>(ctx.key_hash);          // bits 0-15
packed ^= static_cast<uint32_t>(ctx.nesting_depth & 0x1F) << 16; // bits 16-20
packed ^= static_cast<uint32_t>(ctx.array_index   & 0x1F) << 21; // bits 21-25
packed ^= static_cast<uint32_t>(ctx.last_value_type)      << 26; // bits 26-29
packed ^= static_cast<uint32_t>(ctx.in_array)              << 30; // bit 30
```

### Murmurhash3 finalizer

The packed bits are then mixed with the murmurhash3 32-bit finalizer to
achieve avalanche (every input bit affects every output bit with ~50%
probability):

```cpp
packed ^= packed >> 16;
packed *= 0x85ebca6b;
packed ^= packed >> 13;
packed *= 0xc2b2ae35;
packed ^= packed >> 16;
```

The bucket index is `context_id & (MAX_CONTEXTS - 1)` (bitwise AND since
4096 is a power of two). The full `context_id` value is stored as
`context_signature` for collision detection.

### Key hashing

The `key_hash` field uses FNV-1a folded to 16 bits:

```cpp
uint32_t h = 0x811c9dc5u;      // FNV offset basis
for (size_t i = 0; i < len; ++i) {
    h ^= key[i];
    h *= 0x01000193u;           // FNV prime
}
return static_cast<uint16_t>((h >> 16) ^ (h & 0xFFFF));  // XOR-fold
```

Collisions in key hashing are acceptable -- they merge the type distributions
of two keys, reducing prediction confidence but never causing incorrect results.

______________________________________________________________________

## 3. Fixed-Point Confidence Check

The hot path of `predict()` must determine whether the most-likely type has
high enough confidence to justify speculating. The naive check is:

```
  confidence = max_count / total
  should_speculate = confidence >= 0.85
```

Division is expensive (20-30 cycles on x86). The code rearranges to avoid it
entirely using fixed-point arithmetic.

### Derivation

Multiply both sides of the inequality by `total * 65536`:

```
  max_count / total >= 0.85
  max_count * 65536 >= 0.85 * 65536 * total
  max_count * 65536 >= 55706 * total
```

Where `55706 = floor(0.85 * 65536)` is the compile-time constant
`CONFIDENCE_THRESHOLD_FP`.

### Range analysis

- `max_count` is `uint16_t` (max 65535). `max_count * 65536` fits in `uint64_t`.
- `total` is `uint32_t`. `55706 * total` fits in `uint64_t`.
- No overflow possible.

### Implementation

```cpp
const uint64_t lhs = static_cast<uint64_t>(max_count) * 65536ULL;
const uint64_t rhs = static_cast<uint64_t>(CONFIDENCE_THRESHOLD_FP)
                   * static_cast<uint64_t>(b.total);
const bool should_spec = lhs >= rhs;
```

On x86-64, this compiles to two `IMUL` instructions and a `CMP` -- about 6
cycles total vs 20-30 for a division.

______________________________________________________________________

## 4. Exponential Decay

The model adapts to changing patterns via periodic exponential decay. Every
256 observations per bucket, all counts are right-shifted by 1 (halved):

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

### Effective window

Each observation's influence halves every 256 subsequent observations. After
`k` decay cycles, an observation's weight is `1 / 2^k`. The effective window
(sum of geometric series) is:

```
  W = 256 * (1 + 1/2 + 1/4 + ...) = 256 * 2 = 512 observations
```

In practice, this means the model adapts within about 500 observations of a
schema change while remaining stable for consistent workloads.

### Why right-shift instead of multiply

A right-shift by 1 is a single-cycle instruction and maintains integer counts.
Multiplying by a decay factor (e.g., 0.99) would require floating-point
arithmetic in the observation path, violating the "observe \< 50 cycles" budget.

The check `(b.total & (DECAY_PERIOD - 1)) == 0` is a single AND instruction
because `DECAY_PERIOD` (256) is a power of two. It fires once every 256
observations -- amortized cost per observation is 9 shifts / 256 = ~0.04
cycles.

______________________________________________________________________

## 5. Worked Example

Consider parsing NDJSON records of the form:

```json
{"name": "Alice", "age": 30, "active": true}
{"name": "Bob",   "age": 25, "active": false}
```

### Step 1: First record, key "age"

Context:

- `key_hash = FNV1a("age") folded to 16 bits` = suppose 0x1A3F
- `nesting_depth = 1` (inside the top-level object)
- `array_index = 0` (not in an array)
- `last_value_type = SHORT_STRING` (just parsed `"Alice"`)
- `in_array = false`

Packed context ID:

```
  packed  = 0x1A3F                          // key_hash
  packed ^= (1 & 0x1F) << 16   = 0x01_0000 // depth=1
  packed ^= (0 & 0x1F) << 21   = 0         // array_index=0
  packed ^= (2)         << 26   = 0x0800_0000 // SHORT_STRING=2
  packed ^= (0)         << 30   = 0         // in_array=false

  packed = 0x0801_1A3F (before mixing)
```

After murmurhash3 finalizer, suppose `context_id = 0xA7C3E2F1`.

Bucket index = `0xA7C3E2F1 & 0xFFF = 0x2F1 = 753`.

### Step 2: Observe INTEGER

The model increments `bucket[753].counts[INTEGER]` and `bucket[753].total`.
After 200 observations of `"age": <integer>`, the bucket looks like:

```
  counts[INTEGER]      = 200
  counts[FLOAT]        = 0
  counts[SHORT_STRING] = 0
  ...
  total                = 200
```

### Step 3: Predict for new record

When the parser encounters `"age":` in the next record, it queries the model
with the same context. The prediction logic finds:

```
  max_count = 200 (INTEGER)
  total     = 200

  Fixed-point check:
    lhs = 200 * 65536 = 13,107,200
    rhs = 55706 * 200 = 11,141,200
    lhs >= rhs -> true (confidence = 1.00)
```

Result: `Prediction { INTEGER, 1.00, should_speculate=true }`.

The parser jumps directly to the SWAR integer fast path, skipping generic
dispatch. If the value were unexpectedly a float (e.g., `"age": 30.5`), the
integer fast path would fail within a few instructions and fall back to
generic parsing.

### Step 4: Decay

After 56 more observations (total reaches 256), decay fires:

```
  counts[INTEGER] = 200 >> 1 = 100
  new_total = 100
```

The model now weights recent observations more heavily. If the schema changes
(e.g., `"age"` becomes a float), the model adapts within ~256 observations.

______________________________________________________________________

## 6. Training Modes

### Schema-based pre-seeding

`train_from_schema()` accepts a simplified JSON Schema and injects 200
synthetic observations per property. This bootstraps the model so that the
first document already benefits from speculation:

```
  200 observations -> confidence = 200/200 = 1.00 > 0.85 -> speculate
```

### Sample-based training

`train_from_samples()` parses NDJSON sample documents with a lightweight
recursive scanner. For each value encountered, it calls `observe()` with the
appropriate context, naturally building the transition distribution.

### Multi-thread merging

After parallel NDJSON processing, per-thread models are combined via
`merge()`. For same-context buckets, counts are summed (saturating at
`UINT16_MAX`). For colliding contexts, the bucket with more observations
wins -- lossy, but preserves the dominant pattern.
