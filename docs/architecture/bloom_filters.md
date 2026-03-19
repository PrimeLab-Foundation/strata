# Bloom Filter Subsystem

The Bloom filter subsystem provides probabilistic set-membership testing for three parsing optimizations: selective field extraction (parsing only desired keys from large objects), duplicate key detection (RFC 8259 compliance without a hash set), and schema-aware key validation (rejecting unknown keys before parsing their values). All filters share the same core `BloomFilter<NumBits, NumHashes>` template, which uses wyhash for hashing and Kirschner-Mitzenmacher double hashing to derive multiple probe positions. Filters larger than one cache line use a blocked design that confines all probes for a given key to a single 64-byte block, guaranteeing one cache line access per query.

## Filter Hierarchy

```
                    BloomFilter<NumBits, NumHashes>
                    (core template, compile-time sized)
                              |
             +----------------+------------------+
             |                |                  |
         Preset<N, FPR>   Preset<N, FPR>    Preset<N, FPR>
         (constexpr optimal sizing)
             |
  +----------+-----------+-----------+
  |          |           |           |
TinyFilter  SmallFilter MediumFilter LargeFilter
256 bits    512 bits    2048 bits    10240 bits
3 hashes    3 hashes    3 hashes     3 hashes
10 keys     50 keys     200 keys     1000 keys
1% FPR      1% FPR      1% FPR       1% FPR
1 cache ln  1 cache ln  blocked(4)   blocked(20)
             |                           |
             |                           |
      +------+------+            +------+------+
      |             |            |             |
  KeyFilter    DedupFilter  SchemaFilter  SelectiveParser
  (Bloom +     (stacked     (per-depth   (structural
   binary      TinyFilters   SmallFilter   index skip)
   search)     per nesting   + global
               level)        MediumFilter)
```

## Hashing: wyhash and Double Hashing

The filter uses wyhash (public domain, Wang Yi) as its base hash function. wyhash produces 64-bit outputs with strong avalanche properties and processes short keys (typical JSON field names, 4-20 bytes) in the fast path without looping: keys of 4-16 bytes require only two unaligned 4-byte reads and a few multiply-mix operations.

Two independent 64-bit hashes are derived by calling wyhash with two different seed constants:

- `h1 = wyhash(key, len, 0x2d358dccaa6c78a5)`
- `h2 = wyhash(key, len, 0x8bb84b93962eacc9)`

The Kirschner-Mitzenmacher double-hashing technique then derives k probe positions as:

```
probe_i(key) = (h1 + i * h2) mod NumBits    for i = 0, 1, ..., k-1
```

This produces k positions with the same theoretical false-positive rate as k truly independent hash functions, while requiring only two hash computations regardless of k. The `NumHashes` template parameter is constrained to \[2, 8\].

## Blocked Design for Cache Efficiency

For filters where `NumBits > 512` (byte size exceeds one 64-byte cache line), the bit array is partitioned into 512-bit (64-byte) blocks. The hashing changes to:

```
block     = h1 mod NUM_BLOCKS
probe_i   = block_offset + (h1 + i * h2) mod 512    for i = 0, ..., k-1
```

All k probes for a given key land within the same 64-byte block. This guarantees exactly one cache line touch per `insert()` or `maybe_contains()` call, regardless of the total filter size. Without blocking, a 10240-bit LargeFilter would scatter probes across ~20 cache lines, causing 3 cache misses per query on average.

The tradeoff is slightly higher false-positive rates compared to a non-blocked filter of the same size, because the effective bit space per key is reduced from NumBits to 512. In practice, the information-theoretic sizing compensates for this by allocating more total bits.

For the small filters (TinyFilter at 256 bits = 32 bytes, SmallFilter at 512 bits = 64 bytes), the entire filter fits in one cache line, so the blocked design is a no-op controlled by a `constexpr bool BLOCKED = (BYTE_SIZE > 64)`.

## Compile-Time Optimal Sizing

The `sizing.h` module provides `constexpr` functions that compute information-theoretically optimal filter parameters:

**Optimal bits**: `m = -(n * ln(p)) / (ln(2))^2` where n is the expected key count and p is the target false-positive rate. A constexpr natural log implementation using the series expansion `ln(x) = 2 * atanh((x-1)/(x+1))` with 30 terms provides ~12 digits of precision.

**Optimal hashes**: `k = (m/n) * ln(2)`, clamped to \[2, 8\].

**Alignment**: All bit counts are rounded up to the nearest multiple of 256 for AVX2-width alignment.

The `Preset<ExpectedKeys, FalsePositivePermille>` template computes these at compile time and produces a `BloomFilter<bits, hashes>` type alias. The four standard presets are:

| Preset       | Keys | Target FPR | Bits  | Hashes | Bytes | Cache Lines | Blocked? |
| ------------ | ---- | ---------- | ----- | ------ | ----- | ----------- | -------- |
| TinyFilter   | 10   | 1%         | 256   | 3      | 32    | 1           | No       |
| SmallFilter  | 50   | 1%         | 512   | 3      | 64    | 1           | No       |
| MediumFilter | 200  | 1%         | 2048  | 3      | 256   | 4           | Yes      |
| LargeFilter  | 1000 | 1%         | 10240 | 3      | 1280  | 20          | Yes      |

## DedupFilter: Stacked Per-Level Filters

The DedupFilter detects duplicate keys within a single JSON object without maintaining a hash set. It uses a stack of TinyFilter instances (256-bit each), one per nesting level up to a maximum depth of 32.

**Stack operations**:

- `push()`: entering a nested object allocates (clears) the next TinyFilter on the stack.
- `pop()`: leaving a nested object returns to the parent level's filter.
- `check_and_insert(key)`: queries the current level's filter, then inserts the key. Returns true if the key was possibly already present (Bloom positive).

**Memory budget**: Each level is 32 bytes (filter) + 2 bytes (key count) = 34 bytes. The full stack for depth 32 is 1088 bytes -- small enough to remain in L1 cache for the entire parse.

**False-positive handling**: The TinyFilter with 10 or fewer keys has an FPR of ~0.14%. On a Bloom positive, the caller performs an exact check against the already-parsed keys for that object. These exact checks are rare enough (\<0.2% of keys) that they do not measurably affect throughput.

**Design choice**: Using Bloom filters instead of a hash set avoids dynamic memory allocation during parsing. The fixed 1088-byte stack is allocated once and reused across documents.

## KeyFilter: Two-Stage Pipeline

The KeyFilter implements selective field extraction -- the parser asks "should I parse this value?" for each object key and skips values of unwanted keys.

**Stage 1 -- Bloom rejection**: A SmallFilter (512-bit, 3 hashes) is populated with the desired key names. For each key encountered during parsing, `maybe_contains()` provides a fast negative: if the filter says no, the key is definitively not in the desired set. This rejects ~99% of unwanted keys without any string comparison.

**Stage 2 -- Binary search exact match**: For the ~1% of keys that pass the Bloom filter (true positives plus false positives), a binary search over the sorted `std::vector<std::string>` of desired keys confirms or rejects the match.

**Action enum**: The `check()` method returns one of:

- `SKIP` -- Bloom filter negative, key is definitively unwanted.
- `UNCERTAIN` -- Bloom filter positive, needs exact check.
- `PARSE` -- filter is empty (accept-all mode).

The combined `should_parse()` method runs both stages and returns a simple boolean.

**JSONPath integration**: `KeyFilter::from_paths()` accepts JSONPath-like expressions (e.g., `"$.user.name"`, `"$.items[*].price"`) and extracts the leaf field names to build the filter.

**Statistics**: The KeyFilter tracks total checks, Bloom rejects, Bloom positives, exact matches, and false positives for profiling filter effectiveness.

## SchemaFilter: Depth-Aware Validation

The SchemaFilter extends the Bloom filter concept with depth awareness. Different nesting levels in a JSON document may have different valid key sets (e.g., `"name"` is valid at depth 1 but `"street"` only at depth 2 inside `"address"`).

**Structure**: A global MediumFilter (2048-bit) holds all valid keys at any depth. An array of 16 SmallFilters (512-bit each) holds per-depth key sets. If the depth exceeds 16, the query falls back to the global filter only.

**Batch validation**: `validate_batch()` checks up to 64 keys at once and returns a 64-bit bitmask indicating which keys passed. This is useful for NDJSON processing where multiple objects can be validated in parallel.

## SelectiveParser: Structural-Index Value Skipping

The SelectiveParser is the integration point between the Bloom filter subsystem and the structural index from the SIMD engine. When the KeyFilter rejects a key, the SelectiveParser needs to advance past the associated value without parsing it.

**Skip algorithm**:

- **Primitives** (strings, numbers, booleans, null): Advance the structural-index cursor to the next structural character (`,`, `}`, or `]`). This is a single index increment.
- **Objects and arrays**: Walk the structural index, incrementing a depth counter on `{`/`[` and decrementing on `}`/`]`, until depth returns to zero. This counts braces/brackets without reading any value content.

The cost of skipping is O(number of structural characters within the skipped value), which is proportional to the structural complexity of the value, not its byte length. A 10 KB string value with no internal structural characters is skipped by advancing one position. A nested object with 100 key-value pairs requires walking ~200 structural positions (all integer increments on a `uint32_t` array).

**Integration with SpeculativeParser**: The SpeculativeParser accepts an optional `KeyFilter*` via `set_key_filter()`. When set, it calls `key_filter_->should_parse(key)` for each object key. On rejection, `skip_value_structural()` advances through the structural index without dispatching to any value parser -- speculative or generic.

## Performance Characteristics

- **Bloom filter query**: ~5-10ns per `maybe_contains()` call for TinyFilter/SmallFilter (single cache line). The dominant cost is the wyhash computation for the key, not the bit probes.
- **KeyFilter end-to-end**: For a JSON object with 500 keys and 5 desired keys, the filter rejects ~495 keys via Bloom (~5ns each = ~2.5us total) and performs ~5-10 exact matches via binary search (~20ns each). Total filtering overhead: ~2.7us vs. parsing all 500 values.
- **Value skipping**: Advancing past a primitive value costs one structural-index increment (~1ns). Skipping a nested object costs O(structural chars) increments. For a typical 100-field object, this is ~200 increments at ~1ns each = ~200ns, compared to several microseconds for full parsing.
- **DedupFilter overhead**: `check_and_insert()` adds ~8-12ns per key (one `maybe_contains()` + one `insert()` on a 256-bit filter). For a typical 10-key object, total overhead is ~100ns. False-positive exact checks occur for \<0.2% of keys.
- **Memory**: TinyFilter: 32 bytes. SmallFilter: 64 bytes. DedupFilter stack: 1088 bytes. KeyFilter: 64 bytes (Bloom) + sorted key vector. SchemaFilter: 256 bytes (global) + 16\*64 bytes (depth filters) + key vectors. All hot-path filters fit comfortably in L1 cache.
