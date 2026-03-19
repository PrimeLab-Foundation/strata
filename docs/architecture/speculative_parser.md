# Speculative Parser

The speculative parser accelerates JSON deserialization by predicting the type of each value before examining it, then dispatching directly to a type-specialized fast-path parser that skips the generic classification-and-dispatch logic. A Markov transition model learns the statistical structure of the input -- which value types follow which keys at which nesting depths -- and drives these predictions. When a prediction is correct (the common case after warmup), the fast-path parser avoids branches, generic dispatch, and unnecessary type checks. When a prediction is wrong, the system falls back to the generic parser with no correctness penalty. The model learns online across NDJSON lines, adapting to schema patterns within the first ~100 values.

## Speculative Parsing Flow

```
  Structural index position -> next value to parse
                    |
                    v
  +---------------------------------------+
  |  1. BUILD CONTEXT                     |
  |                                       |
  |  ParserContext = {                     |
  |    key_hash:  FNV-1a 16-bit of key,   |
  |    depth:     current nesting level,   |
  |    array_idx: position in array,       |
  |    last_type: previous value's type,   |
  |    in_array:  parent is array?         |
  |  }                                    |
  +---------------------------------------+
                    |
                    v
  +---------------------------------------+
  |  2. PREDICT                           |
  |                                       |
  |  Prediction = model.predict(context)  |
  |  -> predicted_type, confidence,       |
  |     should_speculate                  |
  |                                       |
  |  If confidence < 0.85 or warmup not   |
  |  reached -> go to step 5 (generic)    |
  +---------------------------------------+
                    |
            (confident prediction)
                    |
                    v
  +---------------------------------------+
  |  3. FAST-PATH DISPATCH                |
  |                                       |
  |  dispatch_table[predicted_type]       |
  |    -> parse_integer (SWAR)            |
  |    -> parse_float (fast_float)        |
  |    -> parse_short_string (SIMD load)  |
  |    -> parse_bool_true (4-byte cmp)    |
  |    -> parse_bool_false (5-byte cmp)   |
  |    -> parse_null (4-byte cmp)         |
  |    -> parse_object_open ('{' check)   |
  |    -> parse_array_open ('[' check)    |
  |                                       |
  |  Returns ParseResult {                |
  |    success, bytes_consumed, value     |
  |  }                                    |
  +---------------------------------------+
                    |
          +---------+---------+
          |                   |
    success=true        success=false
    (speculation hit)   (speculation miss)
          |                   |
          v                   v
  +----------------+  +------------------+
  |  4. USE RESULT |  |  5. GENERIC      |
  |                |  |     FALLBACK     |
  |  Accept the    |  |                  |
  |  parsed value  |  |  Full recursive  |
  |                |  |  descent parser  |
  +----------------+  +------------------+
          |                   |
          +---------+---------+
                    |
                    v
  +---------------------------------------+
  |  6. UPDATE MODEL (online learning)    |
  |                                       |
  |  actual_type = classify(first_byte)   |
  |  model.observe(context, actual_type)  |
  |                                       |
  |  Record stats: prediction correct?    |
  |  speculation attempted? hit?          |
  +---------------------------------------+
                    |
                    v
            Advance to next value
```

## Markov Transition Model

The TransitionModel maintains a lossy hash table that maps parser contexts to value-type probability distributions. It answers the question: "given that I am parsing the value for key 'age' at depth 2, what type is it most likely to be?"

**Table structure**: 4096 buckets, each containing:

- `uint16_t counts[9]` -- one counter per ValueType (INTEGER, FLOAT, SHORT_STRING, LONG_STRING, BOOL_TRUE, BOOL_FALSE, NULL_VALUE, OBJECT, ARRAY)
- `uint32_t total` -- sum of all counts (avoids recomputation)
- `uint32_t context_signature` -- full hash for collision detection

Total size: 4096 * ~30 bytes = ~120 KB, verified by a `static_assert` to fit within 128 KB (L2 cache budget).

**Context hashing**: The `ParserContext` fields are packed into a 32-bit value -- key_hash (16 bits), nesting_depth (5 bits), array_index (5 bits), last_value_type (4 bits), in_array (1 bit) -- then run through a murmurhash3 32-bit finalizer for uniform distribution. The low 12 bits select the bucket (4096 = 2^12).

**Lossy collision handling**: When two contexts hash to the same bucket, they share it. The `context_signature` field records the full hash of the last context that wrote to the bucket, but collisions do not cause eviction or chaining. The colliding context's observations simply accumulate in the same counters. This degrades prediction accuracy for both contexts but never causes an incorrect parse.

**Prediction (hot path, target: \<20 cycles)**:

1. Compute bucket index from context.
1. Unrolled loop over 9 counts to find the maximum.
1. Fixed-point confidence: `confidence_fp = (max_count << 16) / total`. The division is implemented as multiply-and-shift to avoid the `div` instruction.
1. Compare `confidence_fp >= 55706` (which represents 0.85 in Q16 fixed-point).
1. Return `{predicted_type, confidence, should_speculate}`.

**Observation and decay (target: \<50 cycles)**:

1. Increment `counts[actual_type]` and `total`.
1. Every 256 observations per bucket (`total % 256 == 0`), apply exponential decay: right-shift all 9 counts by 1, recompute total. This halves the influence of old observations, giving the model an effective memory window of ~256 recent values per context. The decay allows the model to adapt when NDJSON streams contain documents with evolving schemas.

## Fast-Path Parsers

Each fast-path parser is a specialized function that assumes a specific value type and parses without generic dispatch overhead. If the assumption is violated (wrong first byte, unexpected format), it returns `{.success = false}` immediately.

**SWAR integer parsing** (`parse_integer`): Loads 8 bytes into a `uint64_t`, subtracts `0x3030303030303030` (ASCII '0' from each byte), checks if any resulting byte exceeds 9 (indicating a non-digit). The `parse_8_digits` helper uses SWAR multiply-and-accumulate to reduce 8 single-digit values into a single integer in ~4 multiplications. Handles sign prefix and verifies the character after the last digit is a JSON delimiter.

**Lemire fast_float** (`parse_float`): Delegates to the fast_float library (vendored in `third_party/fast_float/`) for IEEE 754 double parsing. The fast-path handles the common case (no subnormals, no overflow) in ~20ns. Falls back to a full-precision algorithm for edge cases.

**Branchless literal comparison** (`parse_bool_true`, `parse_bool_false`, `parse_null`): Loads 4 or 5 bytes as a `uint32_t`, compares against the expected literal as a single integer comparison. For `true`: `*(uint32_t*)data == 0x65757274` (little-endian "true"). For `false`: compares 5 bytes. For `null`: 4 bytes. One comparison instead of four.

**Short string** (`parse_short_string`): Single 32-byte SIMD load (or 16-byte on NEON), simultaneous scan for closing quote and backslash. If the closing quote appears within 32 bytes and no backslash is found, the string is copied directly to the arena. This handles the majority of JSON string values (keys, short field values) without the full escape-processing pipeline.

**Long string** (`parse_long_string`): SIMD scan in 32-byte chunks for backslash and quote characters. Escape sequences are processed using a lookup table for common escapes (`\n`, `\t`, `\\`, `\"`, etc.).

**Container openers** (`parse_object_open`, `parse_array_open`): Simply verify the first byte is `{` or `[`. The actual container parsing is handled by the SpeculativeParser's recursive descent -- these fast-paths just confirm that the prediction was correct so the coordinator can proceed without re-examining the byte.

**Dispatch table**: A static `std::array<ParseFn, 9>` of member function pointers indexed by `ValueType`. This enables branchless dispatch via a single indexed call, avoiding a switch statement.

## Online Learning and NDJSON Adaptation

The SpeculativeParser supports two learning modes:

**Warmup phase**: For the first N values (configurable, default 100), the parser always uses the generic path but still calls `model.observe()` for each value. This populates the transition table before speculation begins, avoiding wasted fast-path attempts on an empty model.

**Steady-state online learning**: After warmup, every parsed value (whether via fast-path or generic fallback) generates an `observe()` call. The exponential decay ensures the model tracks evolving patterns.

**NDJSON cross-document learning**: When parsing multiple newline-delimited documents via `parse_ndjson()`, the model persists across document boundaries. The first few documents serve as implicit training data; by the 5th-10th document, the model typically reaches >90% speculation hit rate for structurally consistent NDJSON. The `train_from_samples()` method can also pre-train the model from representative data before parsing begins.

**Schema-based pre-training**: `train_from_schema()` accepts a simplified JSON Schema and pre-populates the transition table with expected type distributions. This eliminates the warmup phase for known schemas.

**Model serialization and merging**: Trained models can be serialized to a compact binary format and reloaded for repeated use. After parallel NDJSON processing (where each thread maintains its own model copy), `merge()` combines per-thread observations into a unified model.

## Performance Characteristics

- **Speculation hit rate**: Typically >90% after warmup on structurally consistent JSON/NDJSON. For highly regular data (e.g., arrays of objects with the same keys), hit rates above 95% are common.
- **Fast-path speedup**: When speculation hits, individual value parsing is 2-5x faster than generic dispatch, depending on value type. Integer parsing via SWAR is the largest win (4-8x for multi-digit integers). Branchless literal comparison saves ~5 cycles per bool/null.
- **Misprediction cost**: A speculation miss costs the time of the failed fast-path attempt (typically 5-15 cycles for the early-exit check) plus the full generic parse. The confidence threshold of 0.85 keeps misprediction rate under 15% in steady state.
- **Model overhead**: `predict()` adds ~15-20 cycles per value (hash computation, bucket lookup, max-finding loop). `observe()` adds ~10-15 cycles. This overhead is recouped when the fast-path saves more than ~30 cycles, which is typical for all value types except very short values.
- **Memory footprint**: The TransitionModel is ~120 KB (fits in L2 cache). The FastPaths object is stateless except for the arena reference. Total per-parser overhead is dominated by the model.
