# JSON Core Module

This directory contains the core JSON processing implementation for Strata.

## Files

### Parsing
- **`json_parse.cpp`** - Main JSON parser with SAX-style event emission
- **`json_tape.cpp`** - Token tape format for two-phase parsing
- **`json_lazy_cursor.cpp`** - Lazy cursor for zero-copy navigation

### Serialization
- **`json_serialize.cpp`** - JSON serialization (dumps) implementation

### Document & Cursor
- **`json_document.cpp`** - Document wrapper combining parsing and serialization
- **`json_cursor.cpp`** - Eager cursor for DOM navigation

### NDJSON
- **`ndjson_stream.cpp`** - NDJSON streaming/iteration
- **`parallel_ndjson.cpp`** - Parallel NDJSON processing

### Memory
- **`json_mmap.cpp`** - Memory-mapped file I/O

## Architecture

```
                    ┌─────────────────┐
                    │   Input JSON    │
                    └────────┬────────┘
                             │
                    ┌────────▼────────┐
                    │   json_parse    │ ← SIMD UTF-8 validation
                    │   (SAX events)  │ ← fast_parse for numbers
                    └────────┬────────┘
                             │
           ┌─────────────────┼─────────────────┐
           │                 │                 │
    ┌──────▼──────┐  ┌───────▼───────┐  ┌──────▼──────┐
    │  JsonValue  │  │  Token Tape   │  │   Python    │
    │    (DOM)    │  │ (two-phase)   │  │   Objects   │
    └──────┬──────┘  └───────┬───────┘  └─────────────┘
           │                 │
    ┌──────▼──────┐  ┌───────▼───────┐
    │   Cursor    │  │ Lazy Cursor   │
    │ (navigation)│  │ (zero-copy)   │
    └─────────────┘  └───────────────┘
```

## Key Design Decisions

1. **SAX-style core**: Parser emits events, allowing multiple build targets
2. **Lazy string unescape**: Defer string processing until needed (ADR-0002)
3. **Token tape**: Compact IR for two-phase parsing (ADR-0003)
4. **SIMD validation**: Upfront UTF-8 validation before parsing

## Dependencies

- `../util/` - SIMD utilities, fast parsing, float formatting
- `../../include/strata/json/` - Public headers

## See Also

- `docs/adr/ADR-0001-hybrid-sax-and-python-builder.md`
- `docs/adr/ADR-0002-lazy-string-unescape.md`
- `docs/adr/ADR-0003-token-tape-format.md`
