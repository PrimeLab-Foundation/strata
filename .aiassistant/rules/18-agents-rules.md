---
alwaysApply: true
---
# Rule: AGENTS.md Contract

## Rule 1: The Six-Function Contract

The entire public Python API is:

```
strata.load(source, **kwargs)         -> object
strata.dump(target, obj, **kwargs)    -> int
strata.loads(source, **kwargs)        -> object
strata.dumps(obj, **kwargs)           -> str | bytes | bytearray
strata.search(source, expr, **kwargs) -> list
strata.query(data, expr, **kwargs)    -> list

strata.compile_path(expression)       -> CompiledPath
```

No other public functions in `strata.__init__`. Configuration helpers
(`set_cycle_policy`, `set_duplicate_key_policy`, `set_dumps_type_order`) live in
`strata.serialize`.

## Rule 2: Python = One-Line Pass-Through

Every function body in `__init__.py` is exactly one line:

```python
def search(source, expression, **kwargs):
    return _native.search(source, expression, **kwargs)
```

No branching. No detection. No normalization. No helpers. No imports besides `_native`.
Python does nothing except forward the call.

Forbidden - ANY logic in Python:
```python
# BAD: type checking
if isinstance(source, bytearray):
    source = bytes(source)

# BAD: detection
if _is_pathlike(source):
    ...

# BAD: kwargs interpretation
if return_type == "bytes":
    return _native.dumps_bytes(obj)

# BAD: multiple native calls
compiled = _native.compile_path(expression)
return _native.search(data, compiled)

# BAD: reading file-like objects in Python
data = source.read()
return _native.loads(data)
```

All of the above happens in C++ via the CPython API.

## Rule 3: C++ Handles Everything

C++ handles all logic using the CPython API:

- File-like objects: `PyObject_CallMethod(obj, "read", NULL)`
- PathLike: `PyOS_FSPath(obj)`
- bytearray: `PyByteArray_Check` + `PyByteArray_AS_STRING`
- memoryview: `PyMemoryView_Check` + `PyObject_GetBuffer`
- NDJSON detection: check `.ndjson` extension in C++
- JSON text vs file path: `looks_like_json_text()` in C++
- return_type kwarg: C++ parses it, returns str/bytes/bytearray accordingly
- ndjson/skip_errors/on_error kwargs: C++ parses and routes internally
- Path compilation: C++ compiles expression strings internally
- Strategy selection: mmap vs buffer parse vs cursor eval - all C++

Python knows nothing. Python decides nothing.

## Rule 4: 1:1 Interface

Every Python function maps to exactly one `_native` function with the same name:

| Python | _native | C function |
|---|---|---|
| `strata.loads()` | `_native.loads()` | `strata_loads` |
| `strata.dumps()` | `_native.dumps()` | `strata_dumps` |
| `strata.load()` | `_native.load()` | `strata_load` |
| `strata.dump()` | `_native.dump()` | `strata_dump` |
| `strata.search()` | `_native.search()` | `strata_search` |
| `strata.query()` | `_native.query()` | `strata_query` |
| `strata.compile_path()` | `_native.compile_path()` | `strata_compile_path` |

No function in Python calls a differently-named native function.
No function in Python calls multiple native functions.

## Rule 5: kwargs Are Pure Pass-Through

Python passes `**kwargs` directly to C++. Python does NOT inspect, pop, validate,
normalize, or route based on kwargs. C++ parses all kwargs via
`PyArg_ParseTupleAndKeywords`.

## Rule 6: No Duplicate Logic

If C++ implements it, Python MUST NOT duplicate it. There is no Python-side copy of
NDJSON detection, path detection, type checking, return_type handling, or anything else.

## Rule 7: Naming Consistency

- Public API: `search` (not `jsonpath`)
- C++ internals: `search` (`compile_search_path`, `eval_search_path`)
- File names: `search` (`python_search.cpp`, `search_compile.cpp`, `search_eval.cpp`)
- Test files: `search` (`test_search.py`, `test_search_advanced.py`)

## Rule 8: Testing

- `make test` runs all tests
- Tests use `strata.*` public interface
- Benchmarks use `strata.*` public interface
