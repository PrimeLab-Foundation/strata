# ADR-0003: Token Tape Format

## Status
Accepted

## Context

The current Strata parser uses a SAX-based architecture (per ADR-0001) where the parser emits events to a handler that builds either a C++ DOM (`DomBuilderHandler`) or Python objects (`PythonObjectBuilder`). This architecture has proven effective for single-pass parsing scenarios.

However, several use cases would benefit from an intermediate representation:

1. **Repeated Access Patterns**: Applications that parse JSON once but query it multiple times (e.g., JSONPath queries, validation pipelines) currently must either:
   - Re-parse the JSON on each access (slow)
   - Build full DOM/Python objects upfront (memory-intensive)

2. **Selective Materialization**: Large JSON documents where only portions are accessed don't need full object construction.

3. **Two-Phase Building**: Some workflows benefit from parsing validation before object construction.

ADR-0001 explicitly mentions "Optional Tape" as a future enhancement. This ADR formalizes that design.

## Decision

We will implement a **Token Tape Format** as an intermediate representation:

1. **TapeBuilder**: A new `JsonSaxHandler` implementation that writes SAX events to a compact tape format.

2. **JsonTape**: An immutable data structure holding the parsed tape, suitable for:
   - Building C++ `JsonValue` DOM via `TapeToDomBuilder`
   - Building Python objects via `TapeToPythonBuilder` (in bindings)
   - Direct iteration for streaming/validation scenarios

3. **Core Purity**: The tape implementation remains pure C++ (no `Python.h`). Python-specific builders live in `src/strata/bindings/`.

## Detailed Design

### Tape Format Specification

The tape uses three contiguous buffers:

1. **Token Buffer** (`std::vector<uint8_t>`): Stream of token types
2. **Values Buffer** (`std::vector<uint64_t>`): Packed numeric values, offsets, and counts
3. **String Buffer** (`std::string`): Concatenated strings and keys

#### Token Types (1 byte each)

```cpp
enum class TapeToken : uint8_t {
    Null       = 0,   // No value entry
    True       = 1,   // No value entry
    False      = 2,   // No value entry
    Int64      = 3,   // values[i] = bit_cast<uint64_t>(int64_value)
    Uint64     = 4,   // values[i] = uint64_value
    Double     = 5,   // values[i] = bit_cast<uint64_t>(double_value)
    String     = 6,   // values[i] = offset, values[i+1] = length
    Key        = 7,   // values[i] = offset, values[i+1] = length
    StartObject= 8,   // values[i] = element_count (keys + values)
    EndObject  = 9,   // No value entry
    StartArray = 10,  // values[i] = element_count
    EndArray   = 11,  // No value entry
    RootEnd    = 255, // Sentinel marking end of tape
};
```

#### Value Encoding

| Token Type   | Values Consumed | Description                           |
|--------------|-----------------|---------------------------------------|
| Null         | 0               | -                                     |
| True         | 0               | -                                     |
| False        | 0               | -                                     |
| Int64        | 1               | `bit_cast<uint64_t>(int64_t)`         |
| Uint64       | 1               | Direct uint64_t                       |
| Double       | 1               | `bit_cast<uint64_t>(double)`          |
| String       | 2               | offset into strings_, length          |
| Key          | 2               | offset into strings_, length          |
| StartObject  | 1               | Number of key-value pairs             |
| EndObject    | 0               | -                                     |
| StartArray   | 1               | Number of elements                    |
| EndArray     | 0               | -                                     |

### API Design

#### Core Tape Classes (include/strata/json/json_tape.hpp)

```cpp
namespace strata {

// Token tape - immutable after construction
class JsonTape {
public:
    // Iteration
    class Iterator;
    Iterator begin() const;
    Iterator end() const;

    // Direct access to buffers (for builders)
    const std::vector<uint8_t>& tokens() const;
    const std::vector<uint64_t>& values() const;
    const std::string& strings() const;

    // Stats
    size_t token_count() const;
    size_t memory_usage() const;
};

// Iterator for reading tape
class JsonTape::Iterator {
public:
    TapeToken token() const;

    // Value access (only valid for appropriate token types)
    int64_t as_int64() const;
    uint64_t as_uint64() const;
    double as_double() const;
    std::string_view as_string() const;
    size_t container_size() const;  // For StartObject/StartArray

    Iterator& operator++();
    bool operator==(const Iterator& other) const;
};

// SAX handler that builds tape
class TapeBuilder : public JsonSaxHandler {
public:
    TapeBuilder();

    // JsonSaxHandler interface
    bool on_null() override;
    bool on_bool(bool v) override;
    bool on_int(int64_t v) override;
    bool on_uint(uint64_t v) override;
    bool on_double(double v) override;
    bool on_string(std::string_view v, bool has_escapes) override;
    bool on_start_object(size_t size_hint) override;
    bool on_key(std::string_view v, bool has_escapes) override;
    bool on_end_object() override;
    bool on_start_array(size_t size_hint) override;
    bool on_end_array() override;

    // Extract built tape (moves ownership)
    JsonTape build();
};

// Build DOM from tape
Result<JsonValue> tape_to_dom(const JsonTape& tape);

} // namespace strata
```

#### Parsing API Extensions (include/strata/json/json_parse.hpp)

```cpp
// Parse JSON to tape
Result<JsonTape> parse_to_tape(std::string_view text);
```

### Memory Layout

```
JsonTape:
┌─────────────────────────────────────────────────┐
│ tokens_: [Tok][Tok][Tok][Tok][Tok][Tok][...]    │  (1 byte each)
├─────────────────────────────────────────────────┤
│ values_: [Val64][Val64][Val64][...]             │  (8 bytes each)
├─────────────────────────────────────────────────┤
│ strings_: "key1value1key2..."                   │  (variable)
└─────────────────────────────────────────────────┘
```

### Container Size Recording

For objects and arrays, we need to record element counts to enable efficient iteration. The TapeBuilder uses a deferred-write strategy:

1. On `StartObject`/`StartArray`: Push a placeholder, remember position
2. On `EndObject`/`EndArray`: Write actual count to the remembered position

This approach avoids a second pass while maintaining O(1) container size lookup.

### Lifetime Rules

- `std::string_view` from `Iterator::as_string()` points into `JsonTape::strings_`
- Views are valid as long as the `JsonTape` object lives
- `JsonTape` is immutable after construction from `TapeBuilder::build()`

### Error Model

Tape building inherits errors from the SAX parser:
- Parsing errors (malformed JSON) are returned from `parse_to_tape()`
- Tape iteration errors (e.g., reading int64 from Double token) result in undefined behavior (debug assertions)

### Thread Safety

- `JsonTape` is immutable after construction, safe for concurrent reads
- `TapeBuilder` is single-threaded during construction
- Multiple threads can create `Iterator` instances on the same `JsonTape`

## File Structure

```
include/strata/json/
├── json_tape.hpp          # Tape types, TapeBuilder, Iterator
│
src/strata/json/
├── json_tape.cpp          # TapeBuilder and tape_to_dom implementation
│
src/strata/bindings/
├── python_tape.cpp        # TapeToPythonBuilder (optional)
│
tests/cpp/
├── test_json_tape.cpp     # C++ unit tests
│
tests/py/
├── test_tape.py           # Python parity tests
```

## Alternatives Considered

1. **SAX-Only (No Tape)**: Simpler, but doesn't support repeated access without re-parsing.

2. **simdjson-Style Tape**: Uses a single interleaved buffer with structural tokens pointing forward/backward. More cache-friendly but more complex to iterate and build from.

3. **DOM with Lazy Nodes**: Keep DOM structure but lazily parse values. Adds complexity to `JsonValue` type.

4. **Memory-Mapped Tape**: Persist tape to file for cross-process sharing. Deferred to future work.

## Benchmark Plan

### Success Criteria

1. **Single-pass parsing**: Tape building should be within 10% of direct DOM building
2. **Repeated queries**: Tape + selective DOM building should be 15-25% faster than re-parsing
3. **Memory**: Tape should use less memory than full DOM for documents where <50% is accessed

### Benchmark Scenarios

1. **Parse-once-use-once**: Compare `parse_json()` vs `parse_to_tape()` + `tape_to_dom()`
2. **Parse-once-query-many**: Parse tape once, run 10 JSONPath queries. Compare to re-parsing.
3. **Selective materialization**: Large document, access only root keys. Compare memory.

### Metrics to Record

- Median latency (ns)
- P95 latency (ns)
- Peak RSS (KB)
- Throughput (MB/s)

Results will be logged in `docs/benchmarks/progress_log.md`.

## Risks and Mitigations

| Risk | Mitigation |
|------|------------|
| Tape overhead negates benefits for single-pass use | Maintain direct SAX→DOM path; tape is optional |
| String copying to tape buffer adds latency | Consider string deduplication for repeated keys |
| API complexity | Keep `parse_json()` as primary API; tape is advanced |

## Implementation Phases

1. **Phase 1**: Core tape infrastructure (`JsonTape`, `TapeBuilder`, `Iterator`)
2. **Phase 2**: DOM builder from tape (`tape_to_dom`)
3. **Phase 3**: Integration with parsing API (`parse_to_tape`)
4. **Phase 4**: Python bindings (optional, in `src/strata/bindings/`)
5. **Phase 5**: Benchmarks and optimization

## References

- ADR-0001: Hybrid SAX/Visitor and Python Builder Architecture
- simdjson tape format: https://github.com/simdjson/simdjson/blob/master/doc/tape.md
- RapidJSON in-situ parsing: https://rapidjson.org/md_doc_dom.html
