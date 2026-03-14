# CPython Bindings Layout

Strata uses the **CPython C API** (not pybind11) for Python bindings. All binding code lives under `src/strata/bindings/`.

## Shared headers

| Header              | Purpose                                                                                                                                                                                                                                        |
| ------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `python_types.h`    | `PyObjectPtr`, `STRATA_RETURN_IF_NULL`, `STRATA_CPP_TRY`/`STRATA_CPP_CATCH`, `LIKELY`/`UNLIKELY`                                                                                                                                               |
| `python_convert.h`  | Declares `json_value_to_python(const strata::JsonValue&)` (defined in `python_loads.cpp`) and **inline** `json_value_list_to_python(const std::vector<strata::JsonValue>&)` (in header so callers in jsonpath/ndjson get inlining without LTO) |
| `python_document.h` | Declares `create_py_json_document`, `create_py_json_cursor`, `is_py_json_document`, `is_py_json_cursor`, getters                                                                                                                               |

## Source files

| File                  | Responsibility                                                               |
| --------------------- | ---------------------------------------------------------------------------- |
| `python_module.cpp`   | Module init, method table                                                    |
| `python_loads.cpp`    | `loads()`, `json_value_to_python()`, `json_value_list_to_python()`           |
| `python_dumps.cpp`    | `dumps()` (str and bytes via `return_type` kwarg)                            |
| `python_document.cpp` | `JsonDocument`, `JsonCursor` types                                           |
| `python_ndjson.cpp`   | `NdjsonStream` type                                                          |
| `python_jsonpath.cpp` | `CompiledPath` type, `compile_path()`, `search()`, `query()`                 |
| `python_iterator.cpp` | `StrataListIterator`, `StrataDictIterator`, `StrataNdjsonFileIterator` types |
| `python_mmap.cpp`     | File-based mmap parsing (used internally by `load(return_type='cursor')`)    |

## Conventions

- Use `#include "python_convert.h"` when you need `json_value_to_python` or `json_value_list_to_python`; do not redeclare them with `extern`.
- Use `STRATA_CPP_TRY` / `STRATA_CPP_CATCH` for C++ exception → Python exception conversion in exported functions.
- Use `LIKELY`/`UNLIKELY` from `python_types.h` for hot-path branches (e.g. in dumps).
