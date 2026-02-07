# ADR-0004: Lazy Parsing Cursor

## Status
Accepted

## Context

Strata currently provides several JSON parsing modes:

1. **Full parsing** (`parse_json()`): Builds complete `JsonValue` DOM regardless of access patterns
2. **SAX parsing** (`parse_sax()`): Streams events but still visits every token
3. **Tape format** (`parse_to_tape()`): Creates compact IR but still materializes everything
4. **Post-parse cursor** (`JsonCursor`): Navigates already-parsed `JsonValue*`
5. **JSONPath**: Operates on fully parsed `JsonDocument`

**Problem**: For large JSON documents where only a small portion is accessed (e.g., extracting `$.user.email` from a 10MB API response), the current approach wastes time and memory parsing unused data.

**Use cases that would benefit**:
- Extracting a single field from a large API response
- Partial iteration over large arrays (first N elements)
- Query-driven parsing where JSONPath only needs specific paths
- Memory-constrained environments processing large documents

## Decision

Implement a **Lazy Parsing Cursor** (`LazyJsonCursor`) that operates directly on raw JSON input and parses only the minimum bytes needed for each operation.

Key design choices:

1. **Direct string_view operation**: The cursor operates on `std::string_view` input, avoiding data copies
2. **Skip-based navigation**: Use efficient skip algorithms to bypass unneeded values without parsing them
3. **On-demand materialization**: Parse and construct values only when explicitly requested
4. **Core purity**: The cursor implementation remains pure C++ with no Python dependencies
5. **Iterative algorithms**: Use explicit depth counters instead of recursion to prevent stack overflow

## Detailed Design

### Position Tracking

```cpp
struct CursorPosition {
    size_t offset;    // Current byte position in input
    size_t line;      // Line number (for error messages)
    size_t column;    // Column number (for error messages)
};
```

### Core LazyJsonCursor Class

```cpp
class LazyJsonCursor {
public:
    explicit LazyJsonCursor(std::string_view input);
    LazyJsonCursor(std::string_view input, CursorPosition pos);

    // Type detection (peeks at current position)
    bool is_null() const;
    bool is_bool() const;
    bool is_number() const;
    bool is_string() const;
    bool is_array() const;
    bool is_object() const;

    // Navigation (returns new cursor positioned at target)
    Result<LazyJsonCursor> get_field(std::string_view key) const;
    Result<LazyJsonCursor> get_at(size_t index) const;

    // Selective materialization
    Result<JsonValue> materialize() const;          // Full subtree
    Result<bool> materialize_bool() const;
    Result<double> materialize_number() const;
    Result<std::string> materialize_string() const;

    // Skip current value, return cursor after it
    Result<LazyJsonCursor> skip() const;

    // Error context
    CursorPosition position() const;
    std::string_view remaining() const;

    // Object/array iteration (lazy)
    class FieldIterator;
    class ElementIterator;

    Result<FieldIterator> iter_fields() const;
    Result<ElementIterator> iter_elements() const;

private:
    std::string_view input_;
    CursorPosition pos_;
};
```

### Skip Algorithms

The skip algorithms are central to the lazy cursor's performance. They traverse JSON values without parsing their content.

#### Skip Value (Entry Point)

```cpp
Result<size_t> LazyJsonCursor::skip_value(size_t from) const {
    if (from >= input_.size()) return {Status::ParseError, from};

    char c = input_[from];
    switch (c) {
        case '"': return skip_string(from);
        case '{': case '[': return skip_container(from);
        case 't': case 'f': return skip_bool(from);
        case 'n': return skip_null(from);
        default:
            if (c == '-' || (c >= '0' && c <= '9'))
                return skip_number(from);
            return {Status::ParseError, from};
    }
}
```

#### Skip Container (Object/Array)

Uses iterative bracket counting to find matching close bracket:

```cpp
Result<size_t> LazyJsonCursor::skip_container(size_t from) const {
    char opener = input_[from];
    char closer = (opener == '{') ? '}' : ']';

    int depth = 1;
    size_t pos = from + 1;

    while (pos < input_.size() && depth > 0) {
        char c = input_[pos];
        switch (c) {
            case '{': case '[':
                ++depth;
                ++pos;
                break;
            case '}': case ']':
                --depth;
                ++pos;
                break;
            case '"': {
                // Skip string (handles escapes)
                auto result = skip_string(pos);
                if (!result.ok()) return result;
                pos = result.value;
                break;
            }
            default:
                ++pos;
        }
    }

    if (depth != 0) return {Status::ParseError, pos};
    return {Status::Ok, pos};
}
```

#### Skip String

Handles escape sequences correctly:

```cpp
Result<size_t> LazyJsonCursor::skip_string(size_t from) const {
    if (from >= input_.size() || input_[from] != '"')
        return {Status::ParseError, from};

    size_t pos = from + 1;
    while (pos < input_.size()) {
        char c = input_[pos];
        if (c == '"') return {Status::Ok, pos + 1};
        if (c == '\\') {
            pos += 2;  // Skip escape sequence
            continue;
        }
        ++pos;
    }
    return {Status::ParseError, pos};  // Unterminated string
}
```

### Field Lookup Algorithm

```cpp
Result<LazyJsonCursor> LazyJsonCursor::get_field(std::string_view key) const {
    if (!is_object()) return {Status::TypeMismatch, {}};

    size_t pos = skip_whitespace(pos_.offset + 1);  // Skip '{'

    while (pos < input_.size() && input_[pos] != '}') {
        // Parse key
        auto key_result = read_string_content(pos);
        if (!key_result.ok()) return {key_result.status, {}};

        auto [found_key, key_end, has_escapes] = key_result.value;
        pos = skip_whitespace(key_end);

        if (pos >= input_.size() || input_[pos] != ':')
            return {Status::ParseError, {}};
        pos = skip_whitespace(pos + 1);

        // Compare key (using LazyString for escape handling)
        bool matches = !has_escapes ? (found_key == key)
                                    : (LazyString(found_key, true).value() == key);

        if (matches) {
            return {Status::Ok, LazyJsonCursor(input_, {pos, pos_.line, 0})};
        }

        // Skip value we don't need
        auto skip_result = skip_value(pos);
        if (!skip_result.ok()) return {skip_result.status, {}};

        pos = skip_whitespace(skip_result.value);
        if (pos < input_.size() && input_[pos] == ',')
            pos = skip_whitespace(pos + 1);
    }

    return {Status::KeyNotFound, {}};
}
```

### Materialization

Materialization parses the current position into concrete values. It leverages the existing SAX parser infrastructure:

```cpp
Result<JsonValue> LazyJsonCursor::materialize() const {
    std::string_view remaining = input_.substr(pos_.offset);
    return parse_json(remaining);
}

Result<std::string> LazyJsonCursor::materialize_string() const {
    if (!is_string()) return {Status::TypeMismatch, {}};

    auto result = read_string_content(pos_.offset);
    if (!result.ok()) return {result.status, {}};

    auto [content, end, has_escapes] = result.value;
    if (!has_escapes) return {Status::Ok, std::string(content)};
    return {Status::Ok, LazyString(content, true).value()};
}
```

### Iterators

#### Field Iterator (for objects)

```cpp
class LazyJsonCursor::FieldIterator {
public:
    struct Field {
        std::string_view key_raw;
        bool key_has_escapes;
        LazyJsonCursor value;

        std::string key() const {
            if (!key_has_escapes) return std::string(key_raw);
            return LazyString(key_raw, true).value();
        }
    };

    FieldIterator& operator++();
    Field operator*() const;
    bool operator==(const FieldIterator& other) const;
    bool at_end() const;

private:
    std::string_view input_;
    size_t pos_;
    bool at_end_;
};
```

#### Element Iterator (for arrays)

```cpp
class LazyJsonCursor::ElementIterator {
public:
    ElementIterator& operator++();
    LazyJsonCursor operator*() const;
    bool operator==(const ElementIterator& other) const;
    bool at_end() const;

private:
    std::string_view input_;
    size_t pos_;
    bool at_end_;
};
```

### JSONPath Integration

New functions for lazy JSONPath evaluation:

```cpp
// Evaluate JSONPath lazily, materializing only matched values
std::vector<JsonValue> eval_jsonpath_lazy(
    std::string_view json,
    const CompiledPath& path,
    size_t limit = std::numeric_limits<size_t>::max()
);

// Find JSONPath matches without materializing
std::vector<LazyJsonCursor> find_jsonpath_lazy(
    std::string_view json,
    const CompiledPath& path,
    size_t limit = std::numeric_limits<size_t>::max()
);
```

## Lifetime Rules

1. **Input ownership**: `LazyJsonCursor` stores `std::string_view`, not owning data. The input must outlive all cursors derived from it.

2. **String views from iterators**: `string_view` values from `FieldIterator` point into the original input and are valid as long as the input is valid.

3. **Materialized values**: Values returned from `materialize*()` methods are owned copies that can outlive the cursor.

4. **Cursor copying**: Cursors are lightweight and can be freely copied. Each copy shares the same input view.

## Error Model

Errors include byte offset for debugging:

- `Status::ParseError`: Malformed JSON at current position
- `Status::TypeMismatch`: Operation incompatible with current value type
- `Status::KeyNotFound`: Requested object key not present
- `Status::IndexOutOfBounds`: Array index exceeds length

Error positions are tracked via `CursorPosition` which includes line and column numbers for human-readable error messages.

## Thread Safety

- `LazyJsonCursor` is **not thread-safe** for concurrent operations on the same cursor
- Multiple threads can safely create and use separate cursors over the same input (read-only input)
- The input `string_view` must not be modified while any cursor references it

## File Structure

```
include/strata/json/
├── json_lazy_cursor.hpp     # LazyJsonCursor, iterators, skip algorithms

src/strata/json/
├── json_lazy_cursor.cpp     # Implementation

src/strata/bindings/
├── python_lazy_cursor.cpp   # Python C-API bindings

include/strata/search/
├── jsonpath.hpp             # Add: eval_jsonpath_lazy(), find_jsonpath_lazy()

src/strata/search/
├── jsonpath_lazy.cpp        # Lazy JSONPath evaluation

python/strata/
├── lazy.py                  # Python LazyCursor wrapper

tests/cpp/
├── test_lazy_cursor.cpp     # C++ unit tests

tests/py/
├── test_lazy_cursor.py      # Python parity tests
```

## Alternatives Considered

1. **Lazy DOM Nodes**: Modify `JsonValue` to defer parsing. Rejected due to complexity increase in core data structure.

2. **Tape + Selective Build**: Parse to tape, then selectively build from tape. Still requires full scan for tape building.

3. **Memory-Mapped Parsing**: Lazy cursor over memory-mapped files. Deferred to future work; current design is compatible with mmap input.

4. **SIMD Skip**: Use SIMD for bracket counting in skip_container. Deferred to optimization phase if benchmarks justify.

## Benchmark Plan

### Success Criteria

1. **Single field extraction**: ≥50% faster than `parse_json()` + navigation for 1MB+ documents
2. **Small document overhead**: ≤20% slower than full parse for documents <10KB with full access
3. **Memory**: Near-constant memory regardless of skipped content size

### Scenarios

| Scenario | Description | Metric |
|----------|-------------|--------|
| `lazy_single_field` | Extract `$.data.items[0].id` from 1MB JSON | Latency, memory |
| `lazy_vs_full_small` | Full parse vs lazy for 10KB, access 1 field | Latency |
| `lazy_vs_full_large` | Full parse vs lazy for 10MB, access 1 field | Latency, memory |
| `lazy_jsonpath_query` | `$..price` on nested 1MB document | Latency |
| `lazy_iteration` | Iterate top-level keys, access 10% of values | Latency |

Results logged in `docs/benchmarks/progress_log.md`.

## Risks and Mitigations

| Risk | Impact | Mitigation |
|------|--------|------------|
| Skip parsing bugs with edge cases | Correctness | Extensive tests with escaped strings, unicode, deep nesting |
| Overhead for small documents | Performance | Document crossover point; recommend `parse_json()` for small/full access |
| Input lifetime confusion | Safety | Clear docs; cursor stores `string_view` not ownership |
| Recursive skip blows stack | Crash | Iterative skip with explicit depth counter |
| SIMD skip complexity | Maintenance | Start scalar; SIMD only if benchmarks justify |

## Implementation Phases

1. **Phase 1**: Core `LazyJsonCursor` with skip algorithms (no materialization)
2. **Phase 2**: Materialization methods (`materialize()`, `materialize_*()`)
3. **Phase 3**: Field/element iterators
4. **Phase 4**: Lazy JSONPath integration
5. **Phase 5**: Python bindings via C-API
6. **Phase 6**: SIMD optimization for skip (optional)
7. **Phase 7**: Benchmarks and tuning

## References

- ADR-0001: Hybrid SAX/Visitor and Python Builder Architecture
- ADR-0003: Token Tape Format
- simdjson On-Demand API: https://github.com/simdjson/simdjson/blob/master/doc/ondemand.md
