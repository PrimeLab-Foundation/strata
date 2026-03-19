# Python Bindings

Strata exposes its C++ JSON engine to Python through a native extension module (`strata._strata`) built with the Python C API. No pybind11 or Cython is involved. The pure-Python layer in `python/strata/` provides a thin, ergonomic wrapper over the native functions.

## Installation

### From PyPI

```bash
pip install strata
```

Requires Python 3.10 or later. Binary wheels are provided for macOS and Linux on common architectures.

### From source

```bash
git clone https://github.com/example/strata.git
cd strata
pip install -e .
```

Building from source requires a C++20 compiler (Clang 14+ or GCC 12+) and CMake 3.20+. The build uses `-O3`, link-time optimization, and `-march=native` by default. See [Building from source](#building-from-source) for full details.

## Parsing

Use `strata.loads` to parse a JSON string or bytes object into a Python dict or list.

```python
import strata

data = strata.loads('{"name": "Alice", "age": 30}')
# {'name': 'Alice', 'age': 30}
```

### Accepting bytes

`loads` accepts both `str` and `bytes`. Passing `bytes` avoids an internal UTF-8 decode step.

```python
raw = b'{"id": 1}'
data = strata.loads(raw)
```

### return_type

The `return_type` keyword controls what `loads` returns.

| Value              | Returns                                       |
| ------------------ | --------------------------------------------- |
| `"dict"` (default) | A standard Python `dict` or `list`            |
| `"cursor"`         | A `JsonCursor` for lazy, on-demand navigation |

```python
cursor = strata.loads('{"users": [1, 2, 3]}', return_type="cursor")
```

See [JsonCursor](#jsoncursor) below for cursor usage.

### iterator

When `iterator=True`, `loads` returns a lazy iterator instead of materializing the full result.

```python
# Object: yields (key, value) tuples
for key, value in strata.loads('{"a": 1, "b": 2}', iterator=True):
    print(key, value)

# Array: yields elements one at a time
for item in strata.loads('[1, 2, 3]', iterator=True):
    print(item)
```

## Serialization

Use `strata.dumps` to serialize a Python object to JSON.

```python
json_str = strata.dumps({"x": 1, "y": [2, 3]})
# '{"x":1,"y":[2,3]}'
```

### return_type

| Value             | Returns                 |
| ----------------- | ----------------------- |
| `"str"` (default) | A Python `str`          |
| `"bytes"`         | A Python `bytes` object |

```python
json_bytes = strata.dumps(data, return_type="bytes")
```

Returning `bytes` skips the final UTF-8 encode into a Python `str` and can be faster when you need to write the output to a file or socket directly.

## File I/O

### Loading files

`strata.load` reads and parses a file in a single call. It detects NDJSON/JSONL by file extension (`.ndjson` or `.jsonl`).

```python
# JSON file -> dict or list
data = strata.load("config.json")

# NDJSON file -> list of records
records = strata.load("events.ndjson")
```

The same `return_type` and `iterator` keyword arguments from `loads` are supported.

```python
# Cursor-based loading for large JSON files
cursor = strata.load("large.json", return_type="cursor")

# Stream NDJSON records one at a time
for record in strata.load("events.ndjson", iterator=True):
    process(record)
```

Note: `return_type="cursor"` is not supported for NDJSON files.

### Writing files

`strata.dump` serializes an object and writes it to a file. A trailing newline is appended automatically.

```python
strata.dump({"key": "value"}, "output.json")
```

## JSONPath

Strata includes a built-in JSONPath engine that works on both in-memory data and files on disk.

### query (in-memory)

`strata.query` evaluates a JSONPath expression against a Python `dict` or `list`. It evaluates directly on the Python objects without building an intermediate C++ tree.

```python
data = {"users": [{"id": 1, "name": "Alice"}, {"id": 2, "name": "Bob"}]}

ids = strata.query(data, "$.users[*].id")
# [1, 2]

names = strata.query(data, "$.users[?(@.id > 1)].name")
# ['Bob']
```

Raises `TypeError` if `data` is not a `dict` or `list`.

### search (file-based)

`strata.search` reads a JSON or NDJSON file and evaluates a JSONPath expression without loading the full file into a Python object first.

```python
ids = strata.search("data.json", "$.users[*].id")
```

Accepts `pathlib.Path` objects as well as strings.

### compile_path

When you need to run the same JSONPath expression many times, compile it once and reuse it.

```python
path = strata.compile_path("$.users[*].id")

ids_a = strata.query(data_a, path)
ids_b = strata.query(data_b, path)
ids_c = strata.search("other.json", path)
```

### Iterator support

Both `query` and `search` accept `iterator=True` to yield matches lazily.

```python
for match in strata.query(data, "$.events[*]", iterator=True):
    handle(match)
```

## JsonCursor

A `JsonCursor` provides lazy, on-demand navigation into a parsed JSON document. The document is parsed into a C++ DOM once, and the cursor lets you traverse it without converting the entire tree to Python objects. This is useful when you only need a few fields from a large document.

### Creating a cursor

```python
# From a string
cursor = strata.loads('{"a": {"b": [1, 2, 3]}}', return_type="cursor")

# From the parse_json helper
from strata import parse_json
cursor = parse_json('{"a": {"b": [1, 2, 3]}}')

# From a file
cursor = strata.load("large.json", return_type="cursor")
```

### Navigation

Use `.field(key)` to descend into an object and `.at(index)` to index into an array. Both return a new `JsonCursor`.

```python
inner = cursor.field("a").field("b").at(0)
```

### Type checks

```python
cursor.is_object()   # True if the value is a JSON object
cursor.is_array()    # True if the value is a JSON array
cursor.is_string()   # True if the value is a JSON string
cursor.is_number()   # True if the value is a JSON number
cursor.is_bool()     # True if the value is a JSON boolean
cursor.is_null()     # True if the value is JSON null
```

### Scalar accessors

Call the matching accessor once you know the type.

```python
cursor.field("name").get_str()    # -> str
cursor.field("age").get_int()     # -> int
cursor.field("score").get_float() # -> float
cursor.field("active").get_bool() # -> bool
```

Calling the wrong accessor (for example `get_int()` on a string) raises an exception from the C++ layer.

### Status-code vs throwing API

At the C++ level, cursor navigation returns a cursor whose `.raw()` may be `nullptr` if the field or index does not exist. The Python wrapper returns `None` in that case rather than raising an exception, so you can check for missing fields without try/except.

```python
result = cursor.field("missing_key")
# result is None if the field does not exist
```

The scalar accessors (`get_str`, `get_int`, `get_float`, `get_bool`) throw if the underlying value has the wrong type.

## Configuration

Strata has a global configuration registry that controls parsing behavior.

```python
import strata

# Set a config value
strata.config.set("duplicate_key_policy", "last")

# Get a config value
policy = strata.config.get("duplicate_key_policy")

# List all config keys and values
all_config = strata.config.list()
```

### duplicate_key_policy

Controls behavior when duplicate keys appear in a JSON object.

| Value               | Behavior                                              |
| ------------------- | ----------------------------------------------------- |
| `"first"` (default) | Keep the first occurrence                             |
| `"last"`            | Keep the last occurrence                              |
| `"error"`           | Raise an error                                        |
| `"warn"`            | Emit a `RuntimeWarning` and keep the first occurrence |

### cycle_policy

Controls behavior when circular references are detected during serialization.

| Value              | Behavior                |
| ------------------ | ----------------------- |
| `"warn"` (default) | Emit a warning          |
| `"error"`          | Raise an error          |
| `"ignore"`         | Silently skip the cycle |

Setting an unknown key raises `KeyError`. Setting an invalid value raises `ValueError` or `TypeError`.

## Iterator Protocol

All main functions (`loads`, `load`, `query`, `search`) accept `iterator=True`. The iterator type depends on the data:

- **Objects** yield `(key, value)` tuples via a `DictIterator`.
- **Arrays** yield individual elements via a `ListIterator`.
- **NDJSON files** yield one parsed record per line via an `NdjsonFileIterator`. Malformed lines are silently skipped (the error count is tracked internally).
- **JSONPath results** yield one match at a time.

```python
# Stream a large NDJSON file without loading it all into memory
for record in strata.load("huge.ndjson", iterator=True):
    process(record)

# Iterate over query results
for user in strata.query(data, "$.users[*]", iterator=True):
    print(user["name"])
```

### Low-level NdjsonStream

For finer control over NDJSON parsing, you can use the `NdjsonStream` class directly.

```python
from strata._strata import NdjsonStream

stream = NdjsonStream.from_string(ndjson_text)

while stream.has_next():
    record = stream.next_line()
    process(record)

# Parse all remaining lines at once
records = stream.parse_all(skip_errors=True)

# Or parse in batches
batch = stream.next_batch(100, skip_errors=True)

# Check how many lines failed to parse
print(stream.error_count())
```

## Performance Tips

1. **Pass `bytes` to `loads`** when possible. If your data is already in `bytes` form (from a network socket, file read, etc.), passing it directly avoids an internal copy.

1. **Use `return_type="cursor"` for large documents** when you only need a few fields. The cursor navigates the C++ DOM without materializing the entire Python object tree.

1. **Use `iterator=True` for NDJSON** files to avoid loading all records into memory at once. The `NdjsonFileIterator` parses one line at a time.

1. **Compile JSONPath expressions** with `strata.compile_path` if you run the same query across many documents. This avoids re-parsing the expression each time.

1. **Use `return_type="bytes"` in `dumps`** when writing output to a file or socket. This skips the final UTF-8 encode into a Python `str`.

1. **Batch NDJSON parsing** with `NdjsonStream.next_batch()` when you can process records in chunks. The batch method reuses internal key caches across lines for better throughput than parsing one line at a time.

## Building from Source

### Requirements

- Python 3.10+
- C++20 compiler: Clang 14+ or GCC 12+
- CMake 3.20+
- setuptools, wheel

### Development install

```bash
git clone https://github.com/example/strata.git
cd strata
pip install -e ".[dev]"
```

The build runs in three phases:

1. C++ unit tests via CMake/ctest (pre-build gate)
1. Compilation of the `strata._strata` extension
1. Python tests via pytest (post-build gate)

To skip tests during development (not recommended for releases):

```bash
SKIP_TESTS=1 pip install -e .
```

### Build options

The build respects several environment variables:

| Variable                  | Description                                            |
| ------------------------- | ------------------------------------------------------ |
| `SKIP_TESTS=1`            | Skip pre/post build test gates                         |
| `STRATA_ENABLE_LTO=0`     | Disable link-time optimization (enabled by default)    |
| `STRATA_SCALAR_ONLY=1`    | Disable SIMD backends, use scalar fallback             |
| `PGO_MODE=generate`       | Build with profile-guided optimization instrumentation |
| `PGO_MODE=use`            | Build using collected PGO profile data                 |
| `STRATA_PGO_PROFILE=path` | Path to the PGO profile data file                      |

### CMake build (C++ tests only)

```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```
