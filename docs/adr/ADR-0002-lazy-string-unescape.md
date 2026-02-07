# ADR-0002: Lazy String Unescape Optimization

## Status
Implemented

## Context
JSON string parsing traditionally processes escape sequences (`\n`, `\t`, `\"`, `\uXXXX`, etc.) immediately during parsing. This means every string is fully unescaped even if:
1. The string is never accessed (common in filtering scenarios)
2. The string is only used for key comparison where raw bytes suffice
3. The string contains no escapes (the majority of real-world JSON strings)

This eager approach incurs unnecessary CPU cycles for escape processing that may never be needed.

## Decision
We implement a **Lazy String Unescape** mechanism that:
1. **Defers escape processing** until the string value is actually accessed
2. **Tracks escape presence** with a boolean flag during parsing
3. **Enables raw byte comparison** for strings without escapes
4. **Caches unescaped values** after first materialization

### Key Changes

#### 1. New `LazyString` Class (`include/strata/util/lazy_string.hpp`)
```cpp
class LazyString {
    std::string raw_;                          // Raw JSON string content
    bool has_escapes_;                         // True if escapes detected
    mutable std::optional<std::string> value_; // Cached unescaped value

public:
    const std::string& value() const;  // Triggers unescape on-demand
    std::string_view raw() const;      // Access raw bytes
    bool has_escapes() const;          // Check escape flag

    // Optimized comparison - fast path for no-escape strings
    bool operator==(const LazyString& other) const;
    bool operator==(std::string_view other) const;
};
```

#### 2. Extended SAX Handler Interface (`json_sax_handler.hpp`)
```cpp
virtual bool on_string(std::string_view v, bool has_escapes = false) = 0;
virtual bool on_key(std::string_view v, bool has_escapes = false) = 0;
```

The `has_escapes` parameter tells handlers whether the string needs unescape processing.

#### 3. Parser Changes (`json_parse.cpp`)
- **Fast path (no escapes)**: Returns raw bytes with `has_escapes=false`
- **Slow path (with escapes)**: Validates escape sequences but passes raw bytes with `has_escapes=true`
- The parser no longer materializes unescaped strings—handlers decide when to unescape

#### 4. Handler Updates
- `DomBuilderHandler`: Uses `LazyString` for deferred unescaping
- `PythonObjectBuilder`: Uses `LazyString` only when escapes are present

## Detailed Design

### Escape Detection
The parser uses SIMD-accelerated scanning (`find_next_escape_simd`) to quickly detect if a string contains escape characters (`\`). This determines the parsing path:

```
String input: "hello\nworld"
              ^
              Escape detected at position 5

Fast scan → has_escapes=true → validate escapes → pass raw bytes to handler
```

### Comparison Optimization
The key performance benefit comes from comparing strings without triggering unescape:

```cpp
// Fast path: neither has escapes - compare raw bytes directly
if (!has_escapes_ && !other.has_escapes_) {
    return raw_ == other.raw_;
}
// Slow path: compare unescaped values
return value() == other.value();
```

This is particularly beneficial for:
- JSONPath queries filtering by key names
- Object field lookups where keys rarely contain escapes
- Large arrays where most string values are never accessed

### Thread Safety
`LazyString` uses `mutable std::optional` for lazy caching. The unescape operation itself is deterministic, making concurrent reads safe if the same value is produced. For strict thread safety in multi-threaded contexts, callers should call `materialize()` from a single thread first.

## Alternatives Considered

### 1. Store Both Raw and Unescaped
Store both representations upfront. Rejected because:
- Doubles memory usage for strings
- Performs unescape work even when not needed

### 2. Always Eager Unescape
Keep current behavior. Rejected because:
- Wastes CPU on strings that are filtered out
- No benefit for strings without escapes

### 3. Escape-Aware Interning
Intern unescaped strings in a pool. Considered but deferred as future enhancement.

## Performance Analysis

### Benchmark Results (6.25 MB users.json)
```
Library           Min (ms)  Median (ms)  RSS (MB)
--------------------------------------------------
orjson               19.15        27.98     114.2
msgspec              20.64        30.10     117.0
strata               25.45        30.29      84.0  ← Lowest memory
ujson                27.25        41.63     132.8
json (stdlib)        36.02        46.26     127.3
```

Strata achieves competitive parsing speed (8.2% behind orjson) while using **26% less memory** than the nearest competitor. The lazy unescape mechanism contributes to memory efficiency by avoiding intermediate string allocations during parsing.

### Expected Improvement Areas
- **JSONPath queries with filters**: 2-5% improvement by avoiding unescape for filtered-out values
- **Key comparison in lookups**: Direct raw byte comparison for keys without escapes
- **Large document streaming**: Memory savings from deferred allocation

## Test Coverage

### Unit Tests (`tests/cpp/test_lazy_string.cpp`)
- Basic functionality (construction, empty strings, deferred access)
- All escape sequences (`\n`, `\r`, `\t`, `\\`, `\"`, `\/`, `\b`, `\f`)
- Unicode escapes (`\uXXXX`)
- Surrogate pairs (emoji: `\uD83D\uDC4B` → 👋)
- Comparison optimization verification
- Copy/move semantics
- Utility methods

### Integration Tests
- All existing parse tests pass (escape handling parity)
- All Python tests pass (336 tests including unicode/escape suite)

## Rollout Plan
1. ✅ Implement `LazyString` class
2. ✅ Extend `JsonSaxHandler` interface with `has_escapes` parameter
3. ✅ Update parser to pass raw bytes with escape flag
4. ✅ Update all handlers (`DomBuilderHandler`, `PythonObjectBuilder`)
5. ✅ Add comprehensive tests
6. ✅ Verify all existing tests pass
7. ✅ Run benchmarks and document results

## Files Changed
- `include/strata/util/lazy_string.hpp` (new)
- `include/strata/json/json_sax_handler.hpp`
- `src/strata/json/json_parse.cpp`
- `src/strata/bindings/python_object_builder.h`
- `tests/cpp/test_lazy_string.cpp` (new)
- `CMakeLists.txt`

## Future Enhancements
1. **Key Interning with LazyString**: Use lazy strings in key cache
2. **JSON Value with LazyString**: Store `LazyString` instead of `std::string` in `JsonValue`
3. **JSONPath Filter Optimization**: Leverage raw comparison in filter evaluation
