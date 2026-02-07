Based on the benchmark results and codebase analysis, here's a comprehensive list of improvements to make Strata #1 in all categories.

---

### Current Status Summary

| Area | Current Rank | Gap to Leader |
|------|-------------|---------------|
| loads/small | #5 | -135% vs orjson |
| loads/medium | #1 ✅ | +43% vs orjson |
| loads/large | #4 | -39% vs msgspec |
| dumps (all) | #3 | -104% to -150% vs orjson |
| ndjson/small | #1 ✅ | +102% vs msgspec |
| ndjson/medium | #3 | -10% vs msgspec |
| ndjson/large | #4 | -45% vs orjson |
| jsonpath | #1 ✅ | Already winning |

---

### Priority 1: Parsing (`loads`) Improvements

#### 1.1 SIMD Structural Parsing
- **Current**: SIMD used for UTF-8 validation and some string scanning, but core parsing is scalar
- **Improvement**: Implement simdjson-style structural classification
  - Use AVX2/NEON to identify all structural characters (`{`, `}`, `[`, `]`, `,`, `:`, `"`) in 32/16-byte chunks
  - Build a structural index (tape of positions) in one pass
  - Feed the index to a scalar state machine for value extraction
- **Files**: `src/strata/json/json_parse.cpp`, `include/strata/util/simd_string.hpp`
- **Expected gain**: 1.5-2x for large files

#### 1.2 Lazy UTF-8 Validation
- **Current**: Full UTF-8 validation pass before parsing
- **Improvement**: Already have `validate_utf8_lazy()` - ensure it's used on the hot path
  - Only validate non-ASCII content
  - Skip validation for pure ASCII (detected via SIMD)
- **Expected gain**: 5-10% for ASCII-heavy content

#### 1.3 Arena-Based String Storage
- **Current**: Every string creates a new `std::string` allocation
- **Improvement**:
  - Use `std::string_view` into the original input where possible (no escapes)
  - For escaped strings, allocate from a thread-local arena
  - Consider lazy string unescaping (ADR-0002 exists)
- **Files**: `src/strata/json/json_parse.cpp`, `include/strata/util/arena_allocator.hpp`
- **Expected gain**: 20-30% reduction in allocation overhead

#### 1.4 Optimized Number Parsing
- **Current**: Standard number parsing path
- **Improvement**:
  - SIMD-accelerated digit scanning
  - Fast paths for common integer ranges (-9999 to 9999)
  - Use hardware-accelerated float parsing where available
- **Files**: `include/strata/util/fast_parse.hpp`
- **Expected gain**: 10-15%

#### 1.5 Better Memory Layout for JsonValue
- **Current**: `std::variant<nullptr_t, bool, Number, std::string, Array, Object>`
- **Improvement**:
  - Consider tagged union with small-string optimization (SSO for strings ≤23 bytes)
  - Use index-based arrays instead of `std::vector<JsonValue>`
  - Pre-reserve arrays/objects based on heuristics
- **Files**: `include/strata/json/json_core.hpp`
- **Expected gain**: 15-25%

---

### Priority 2: Serialization (`dumps`) Improvements

#### 2.1 Match Competitor Output Size
- **Current**: Output ~12% larger (1,037,143 bytes vs 1,009,303 bytes on medium)
- **Improvement**:
  - Remove unnecessary spaces/formatting
  - Use orjson-style compact number formatting
  - Don't add `.0` suffix to integers represented as doubles
- **Files**: `src/strata/json/json_serialize.cpp`, `src/strata/bindings/python_dumps.cpp`
- **Expected gain**: 10-12% (matching byte count = proportional throughput gain)

#### 2.2 Eliminate snprintf from Hot Path
- **Current**: Some number formatting still uses `std::snprintf`
- **Improvement**:
  - Use Dragonbox consistently for all float formatting
  - Use digit-pair table for integer formatting (already partially done)
  - Ensure no fallback to slow formatting paths
- **Files**: `src/strata/bindings/python_dumps.cpp`, `src/strata/util/dragonbox.hpp`
- **Expected gain**: 15-20%

#### 2.3 SIMD String Escaping
- **Current**: Some SIMD for escape detection, but escape writing is scalar
- **Improvement**:
  - Use SIMD to copy clean string segments in bulk
  - Single-pass scan-and-copy with minimal branches
  - Already have `escape_or_copy_string_simd()` - ensure it's optimal
- **Files**: `src/strata/util/simd_string.cpp`
- **Expected gain**: 10-20% for string-heavy data

#### 2.4 Buffer Pre-allocation Strategy
- **Current**: `out.reserve(1024)` initial capacity
- **Improvement**:
  - Estimate output size based on input characteristics
  - Use geometric growth (2x) instead of standard vector growth
  - Thread-local reusable buffer (already exists via `g_serialize_buffer`)
- **Files**: `src/strata/util/output_buffer.hpp`
- **Expected gain**: 5-10%

#### 2.5 Avoid Python Object Tree Walk
- **Current**: Dumps walks Python dict/list recursively with type checks
- **Improvement**:
  - Cache type information
  - Use batch operations for homogeneous arrays
  - Consider specialized paths for common patterns (list of dicts)
- **Files**: `src/strata/bindings/python_dumps.cpp`
- **Expected gain**: 10-15%

---

### Priority 3: NDJSON Improvements

#### 3.1 Parallel NDJSON Parsing
- **Current**: ADR-0005 exists for parallel NDJSON
- **Improvement**:
  - Use thread pool for parallel line parsing
  - SIMD newline detection (already implemented)
  - Batch Python object creation
- **Files**: `src/strata/json/parallel_ndjson.cpp`
- **Expected gain**: 2-4x on multi-core systems

#### 3.2 Line Buffer Reuse
- **Current**: Each line is parsed independently
- **Improvement**:
  - Reuse arena/buffers across lines
  - Parse multiple lines before yielding to Python (batching)
  - Minimize per-line overhead
- **Files**: `src/strata/json/ndjson_stream.cpp`
- **Expected gain**: 20-30%

#### 3.3 Streaming NDJSON with Incremental Parsing
- **Current**: Split lines, then parse each
- **Improvement**:
  - Single-pass incremental parser
  - Don't materialize line strings
  - Stream results via Python iterator
- **Files**: `experiments/ndjson_chunk_scan/`
- **Expected gain**: 15-25%

---

### Priority 4: Build & Compilation Optimizations

#### 4.1 Enable PGO (Profile-Guided Optimization)
- **Current**: Non-PGO builds in benchmarks
- **Improvement**:
  - Enable PGO+LTO for release builds
  - Already documented in `scripts/pgo_build.sh`
- **Expected gain**: 20-33% (documented in `docs/development/pgo_performance.md`)

#### 4.2 Targeted Compilation Flags
- **Current**: Standard optimization flags
- **Improvement**:
  - Enable `-march=native` for local builds
  - Use `-ffast-math` for non-critical float operations
  - Enable LTO (Link-Time Optimization)
- **Expected gain**: 5-15%

#### 4.3 Hot/Cold Path Separation
- **Current**: Mixed hot and cold code paths
- **Improvement**:
  - Use `[[likely]]`/`[[unlikely]]` attributes (already using `LIKELY`/`UNLIKELY` macros)
  - Ensure error paths don't pollute instruction cache
  - Inline critical hot paths
- **Expected gain**: 5-10%

---

### Priority 5: Python Binding Optimizations

#### 5.1 Reduce GC Pressure
- **Current**: GC dominates parsing time in profiling
- **Improvement**:
  - Already pausing GC during parsing (`::PyGcPause gc_pause`)
  - Consider batch object creation
  - Pre-allocate Python containers with estimated sizes
- **Files**: `src/strata/bindings/python_loads.cpp`
- **Expected gain**: 10-20%

#### 5.2 Key Cache Optimization
- **Current**: Robin hood hash with pre-warmed common keys
- **Improvement**:
  - Expand common key list based on benchmark data
  - Consider perfect hashing for very common keys
  - Reduce hash collisions
- **Files**: `src/strata/bindings/python_object_builder.h`
- **Expected gain**: 5-10%

#### 5.3 Direct Integer Representation
- **Current**: All numbers stored as `double`, then converted to Python float
- **Improvement**:
  - Detect integers during parsing
  - Create `PyLong_FromLong()` directly for integers
  - Avoid float round-trip for exact integers
- **Files**: `src/strata/bindings/python_object_builder.h`
- **Expected gain**: 5-10%

---

### Priority 6: JSONPath Optimizations (Already Winning, But Can Improve)

#### 6.1 PyObject Traversal for Dict Input
- **Current**: `search(dict, path)` does dumps→parse→query
- **Improvement**:
  - Walk PyDict/PyList directly from C++
  - No serialization/re-parsing
- **Files**: `src/strata/bindings/python_jsonpath.cpp`
- **Expected gain**: 10-40x for dict input scenario

#### 6.2 Compiled JSONPath Queries
- **Current**: Parse path on every query
- **Improvement**:
  - Cache compiled query objects
  - Reuse parsed path structure
- **Files**: `src/strata/search/jsonpath.cpp`
- **Expected gain**: 5-15% for repeated queries

---

### Implementation Roadmap

**Phase 1 (1-2 weeks): Quick Wins**
1. Enable PGO+LTO in builds
2. Fix output size to match competitors
3. Ensure all SIMD paths are active

**Phase 2 (2-4 weeks): Parsing Improvements**
1. SIMD structural parsing (experimental first)
2. Arena-based string storage
3. Optimized number parsing

**Phase 3 (2-4 weeks): Serialization Improvements**
1. Eliminate snprintf
2. Optimize buffer management
3. SIMD string escaping improvements

**Phase 4 (2-4 weeks): NDJSON & Polish**
1. Parallel NDJSON implementation
2. Line buffer reuse
3. Final optimization passes

---

### Expected Outcome

| Area | Current | Target | Strategy |
|------|---------|--------|----------|
| loads/small | #5 | #1-2 | SIMD, arena allocation |
| loads/medium | #1 | #1 | Maintain lead |
| loads/large | #4 | #1-2 | SIMD, memory layout |
| dumps/all | #3 | #1-2 | Output size, snprintf elimination |
| ndjson/all | #1-4 | #1 | Parallel parsing, buffer reuse |
| jsonpath | #1 | #1 | Maintain lead, add PyObject traversal |

**Target**: Win 10-12/12 benchmarks with consistent #1 or close-#2 rankings.

