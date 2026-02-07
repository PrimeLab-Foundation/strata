# ADR-0001: Hybrid SAX/Visitor and Python Builder Architecture

## Status
Proposed

## Context
The current Strata parser is a recursive-descent engine that builds a C++ DOM (`strata::JsonValue`). When used from Python, this leads to "double materialization":
1. C++ parser builds a C++ DOM.
2. Python adapter converts the C++ DOM into Python objects.

This process is inefficient due to redundant allocations and multiple traversals of the data. To achieve best-in-class performance, we need a way to build Python objects directly during parsing, without polluting the core C++ engine with Python dependencies.

## Decision
We will implement a **Hybrid SAX/Visitor architecture**:
1. **Core Parser**: Will be refactored to drive a SAX-style handler interface (`JsonSaxHandler`). It will remain pure C++ and unaware of Python.
2. **Handlers/Builders**:
    - `DomBuilderHandler`: A pure C++ handler that reconstructs the `strata::JsonValue` DOM (preserving existing behavior).
    - `PythonObjectBuilder`: A Python-specific handler (located in `src/strata/bindings/`) that builds Python objects directly from SAX events.
3. **Optional Tape**: While SAX is the primary interface, the architecture should allow for an intermediate "tape" representation in the future for even greater performance.

## Detailed Design

### Event API Sketch (`JsonSaxHandler`)
```cpp
class JsonSaxHandler {
public:
    virtual ~JsonSaxHandler() = default;
    virtual bool on_null() = 0;
    virtual bool on_bool(bool v) = 0;
    virtual bool on_int(int64_t v) = 0;
    virtual bool on_uint(uint64_t v) = 0;
    virtual bool on_double(double v) = 0;
    virtual bool on_string(std::string_view v) = 0;
    virtual bool on_start_object(size_t size_hint = 0) = 0;
    virtual bool on_key(std::string_view v) = 0;
    virtual bool on_end_object() = 0;
    virtual bool on_start_array(size_t size_hint = 0) = 0;
    virtual bool on_end_array() = 0;
};
```

### Lifetime Rules
- `std::string_view` arguments in `on_string` and `on_key` are **only valid during the callback**. Handlers must copy the data if they need it to persist (e.g., `DomBuilderHandler` will copy into `std::string`, `PythonObjectBuilder` will create `PyUnicode`).

### Error Model
- The parser will return a `Result<void>` (or similar) indicating success or failure.
- Errors will include:
    - `byte_offset`: Position in the input where the error occurred.
    - `code`: Error category (e.g., `UnexpectedCharacter`, `InvalidUnicode`).
    - `message`: Human-readable description.

### Stack Strategy
- The parser will use an **iterative stack** for tracking nesting depth and container state to avoid stack overflow on deeply nested JSON.
- Maximum nesting depth will be configurable.

## Alternatives Considered
1. **Direct Core + Python.h**: Fast, but violates "Core Purity" rule and makes the core dependent on Python.
2. **Tape-only**: Good performance, but slightly more complex to implement than SAX and may still require a second pass.
3. **Keep DOM**: Simple, but hits a performance ceiling due to double materialization.

## Benchmark Plan
- **Primary metric**: `loads()` latency in Python.
- **Goal**: >20% reduction in median latency for `users.json` dataset.
- **Secondary metrics**: RSS (memory usage), C++ parsing speed.

## Rollout Plan
1. Implement `JsonSaxHandler` and SAX-based core parser.
2. Implement `DomBuilderHandler` and verify parity with current parser via existing tests.
3. Once parity is proven, switch `loads()` to use `PythonObjectBuilder`.
4. Retain `strata::parse_json` (now using `DomBuilderHandler`) for C++ users.
