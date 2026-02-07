# Python Bindings Module

Python C API bindings for Strata. This module bridges the C++ core to Python.

## Files

### Core Module
- **`python_module.cpp`** - Module initialization and method table

### Parsing
- **`python_loads.cpp`** - `strata.loads()` implementation
- **`python_document.cpp`** - Document/cursor Python interface
- **`python_lazy_cursor.cpp`** - Lazy cursor Python bindings
- **`python_tape.cpp`** - Token tape Python interface

### Serialization
- **`python_dumps.cpp`** - `strata.dumps()` and `dumps_bytes()` implementation

### NDJSON
- **`python_ndjson.cpp`** - NDJSON streaming/parsing bindings

### JSONPath
- **`python_jsonpath.cpp`** - JSONPath query bindings

### Memory
- **`python_mmap.cpp`** - Memory-mapped file I/O bindings

### Headers
- **`python_document.h`** - Document type definitions
- **`python_types.h`** - Common Python type utilities
- **`python_object_builder.h`** - SAX handler for building Python objects

## Architecture

```
Python API
    │
    ▼
┌─────────────────────────────────────────┐
│           python_module.cpp             │
│         (PyMethodDef table)             │
└───────────────────┬─────────────────────┘
                    │
    ┌───────────────┼───────────────┐
    │               │               │
    ▼               ▼               ▼
python_loads   python_dumps   python_ndjson
    │               │               │
    ▼               ▼               ▼
┌─────────────────────────────────────────┐
│              C++ Core                   │
│  (json_parse, json_serialize, etc.)    │
└─────────────────────────────────────────┘
```

## Key Design Decisions

### No pybind11 Dependency
Strata uses the raw Python C API directly. This:
- Eliminates external dependencies
- Gives precise control over object creation
- Enables direct-to-Python parsing optimization

### Direct-to-Python Parsing
`python_object_builder.h` implements a SAX handler that creates Python objects
during parsing, avoiding intermediate C++ DOM materialization.

### Error Mapping
C++ errors are mapped to Python exceptions:
- `Status::ParseError` → `ValueError`
- `Status::TypeMismatch` → `RuntimeError`
- `Status::KeyNotFound` → `KeyError`
- `Status::IndexOutOfBounds` → `IndexError`

### Reference Counting
All functions properly manage Python reference counts:
- `Py_INCREF`/`Py_DECREF` for borrowed references
- `Py_NewRef` for new references
- RAII wrappers where appropriate

## Building

The bindings are compiled as a Python extension module:
```bash
pip install -e .  # or
make build
```

## See Also

- `docs/design/python_c_api_migration_plan.md` - Migration from pybind11
- `docs/adr/ADR-0001-hybrid-sax-and-python-builder.md` - Parser architecture
- `.junie/guidelines.md` Section 13 - Binding policy
