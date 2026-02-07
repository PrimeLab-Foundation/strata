# Strata Architecture

This document provides a high-level overview of Strata's architecture.

## Component Overview

```
┌─────────────────────────────────────────────────────────────────────────┐
│                           Python Application                            │
└─────────────────────────────────────────────────────────────────────────┘
                                     │
                                     ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                         Python Wrapper Layer                            │
│                         (python/strata/)                                │
│  • Argument normalization                                               │
│  • Type conversions                                                     │
│  • Error formatting                                                     │
└─────────────────────────────────────────────────────────────────────────┘
                                     │
                                     ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                      Python C API Bindings                              │
│                    (src/strata/bindings/)                               │
│  ┌──────────────┐ ┌──────────────┐ ┌──────────────┐ ┌──────────────┐   │
│  │python_loads  │ │python_dumps  │ │python_ndjson │ │python_jsonpath│   │
│  │  (parsing)   │ │(serializing) │ │ (streaming)  │ │  (queries)   │   │
│  └──────────────┘ └──────────────┘ └──────────────┘ └──────────────┘   │
└─────────────────────────────────────────────────────────────────────────┘
                                     │
                                     ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                           C++ Core Engine                               │
│  ┌─────────────────────────────────────────────────────────────────┐   │
│  │                     JSON Module (src/strata/json/)              │   │
│  │  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐               │   │
│  │  │ json_parse  │ │json_serialize│ │ ndjson_stream│               │   │
│  │  │(SAX parser) │ │  (dumps)    │ │ (streaming) │               │   │
│  │  └─────────────┘ └─────────────┘ └─────────────┘               │   │
│  │  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐               │   │
│  │  │ json_tape   │ │json_document│ │json_lazy_cursor│              │   │
│  │  │ (token IR)  │ │ (wrapper)   │ │ (zero-copy) │               │   │
│  │  └─────────────┘ └─────────────┘ └─────────────┘               │   │
│  └─────────────────────────────────────────────────────────────────┘   │
│  ┌─────────────────────────────────────────────────────────────────┐   │
│  │                   Search Module (src/strata/search/)            │   │
│  │  ┌─────────────────┐ ┌─────────────────┐                       │   │
│  │  │jsonpath_compile │ │ jsonpath_eval   │                       │   │
│  │  │ (path parsing)  │ │  (evaluation)   │                       │   │
│  │  └─────────────────┘ └─────────────────┘                       │   │
│  └─────────────────────────────────────────────────────────────────┘   │
│  ┌─────────────────────────────────────────────────────────────────┐   │
│  │                    Utility Module (src/strata/util/)            │   │
│  │  ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌──────────┐          │   │
│  │  │simd_utf8 │ │simd_escape│ │fast_parse│ │dragonbox │          │   │
│  │  │(validate)│ │(escaping) │ │(numbers) │ │ (floats) │          │   │
│  │  └──────────┘ └──────────┘ └──────────┘ └──────────┘          │   │
│  └─────────────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────────────┘
```

## Data Flow

### Parsing (loads)

```
JSON String
    │
    ▼
┌───────────────────┐
│ SIMD UTF-8 Valid. │ ← Reject invalid UTF-8 upfront
└─────────┬─────────┘
          │
          ▼
┌───────────────────┐
│   SAX Parser      │ ← Emit events: start_object, key, value, etc.
└─────────┬─────────┘
          │
          ├─────────────────┬───────────────────┐
          │                 │                   │
          ▼                 ▼                   ▼
┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐
│  DOM Builder    │ │  Tape Builder   │ │ Python Builder  │
│ (C++ JsonValue) │ │ (Token Tape)    │ │ (PyObject*)     │
└─────────────────┘ └─────────────────┘ └─────────────────┘
```

### Serialization (dumps)

```
Python Object
    │
    ▼
┌───────────────────┐
│ Type Detection    │ ← dict, list, str, int, float, bool, None
└─────────┬─────────┘
          │
          ▼
┌───────────────────┐
│ Recursive Serialize│ ← Handle nested structures
└─────────┬─────────┘
          │
          ├─────────────────┬───────────────────┐
          │                 │                   │
          ▼                 ▼                   ▼
┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐
│ SIMD Escaping   │ │ Dragonbox Float │ │  Integer Conv   │
│ (strings)       │ │ (floats)        │ │  (ints)         │
└─────────────────┘ └─────────────────┘ └─────────────────┘
          │                 │                   │
          └─────────────────┴───────────────────┘
                            │
                            ▼
                    JSON String Output
```

## SIMD Optimization Strategy

```
┌─────────────────────────────────────────────────────────────────┐
│                    SIMD Platform Detection                      │
│                                                                 │
│  #if defined(__AVX2__)     → AVX2 path (256-bit)               │
│  #elif defined(__SSE4_2__) → SSE4.2 path (128-bit)             │
│  #elif defined(__ARM_NEON) → NEON path (128-bit)               │
│  #else                     → Scalar fallback                   │
└─────────────────────────────────────────────────────────────────┘
                            │
        ┌───────────────────┼───────────────────┐
        │                   │                   │
        ▼                   ▼                   ▼
┌───────────────┐   ┌───────────────┐   ┌───────────────┐
│ simd_utf8     │   │ simd_escape   │   │ simd_newline  │
│ • Validate    │   │ • Escape scan │   │ • Line scan   │
│ • Reject bad  │   │ • Fast copy   │   │ • NDJSON      │
└───────────────┘   └───────────────┘   └───────────────┘
```

## Directory Structure

```
strata/
├── include/strata/         # Public C++ headers
│   ├── json/              # JSON types and APIs
│   ├── search/            # JSONPath API
│   └── util/              # Utility headers
│
├── src/strata/            # Implementation
│   ├── json/              # JSON core (parsing, serialization)
│   ├── search/            # JSONPath (compile, eval)
│   ├── util/              # SIMD utilities
│   └── bindings/          # Python C API bindings
│
├── python/strata/         # Python wrapper layer
│
├── tests/
│   ├── cpp/               # C++ tests
│   ├── py/                # Python tests
│   └── fuzz/              # Fuzz tests
│
├── docs/
│   ├── adr/               # Architecture Decision Records
│   ├── api/               # API documentation
│   ├── benchmarks/        # Performance documentation
│   └── archive/           # Historical documents
│
└── benchmarks/            # Benchmark scripts
```

## Key Design Principles

1. **Core stays pure C++**: No Python.h in core files
2. **SAX-style parsing**: Events enable multiple build targets
3. **SIMD everywhere**: UTF-8, escaping, newlines, numbers
4. **Zero-copy where possible**: Lazy cursors, string_view
5. **Exception-free hot paths**: Status/Result types

## See Also

- `docs/adr/` - Architecture Decision Records
- `src/strata/json/README.md` - JSON module details
- `src/strata/util/README.md` - SIMD utility details
- `src/strata/bindings/README.md` - Python binding details
