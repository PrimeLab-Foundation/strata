# Parser Current State (Phase 0)

## Overview

The current parser in `src/strata/json/json_parse.cpp` is a recursive descent parser that directly constructs a `strata::JsonValue` DOM.

## Architecture

- **Type**: Recursive Descent.
- **Output**: `strata::JsonValue` (a `std::variant` of `nullptr_t`, `bool`, `double`, `std::string`, `std::vector<JsonValue>`, and `FlatMap<std::string, JsonValue>`).
- **Flow**: `parse_json` -> `Parser::parse_value` -> (recursive calls to `parse_object`, `parse_array`, etc.).

## Allocations and Performance Bottlenecks

1. **Double Materialization**: When called from Python via `loads()`, the parser first builds a complete C++ DOM. Then, `src/strata/bindings/python_loads.cpp` traverses this DOM to create an equivalent tree of Python objects.
1. **C++ DOM Allocations**:
   - `std::string` for every string value and object key.
   - `std::vector` for every array.
   - `std::vector` (inside `FlatMap`) for every object.
1. **Python Object Allocations**:
   - `PyDict`, `PyList`, `PyUnicode`, `PyFloat`, etc., for every element in the JSON.
1. **No Streaming/SAX**: The current parser doesn't expose any intermediate events, making it impossible to build Python objects during the first pass without including `Python.h` in the parser itself.

## Summary

The current approach is "DOM-first", which is simple and correct but suboptimal for Python `loads()` due to the overhead of building and then converting the intermediate C++ representation. The proposed SAX/Visitor architecture will allow bypassing the C++ DOM for Python `loads()`.
