# Strata

Fast JSON parsing, serialization, and search for Python.

## Public API

`strata` exposes these core functions:

- `search(source, expression, **kwargs) -> list`
- `query(data, expression, **kwargs) -> list`
- `compile_path(expression) -> CompiledPath`
- `load(source, **kwargs) -> object`
- `dump(target, obj, **kwargs) -> int`
- `loads(source, **kwargs) -> object`
- `dumps(obj, **kwargs) -> str | bytes | bytearray`

### search

Search JSON or NDJSON data for values matching a JSONPath expression.

- `source`: file path (str or Path), JSON text (str or bytes), dict/list, JsonCursor/NdjsonCursor,
  or file-like object
- `expression`: JSONPath string or `CompiledPath` (e.g. `"$.users[*].id"`)
- `ndjson`: optional bool to force NDJSON mode
- `skip_errors`: skip malformed NDJSON lines when enabled
- `on_error`: NDJSON error handling: `"skip"`, `"warn"`, or `"error"` (default)
- `parallel`, `num_threads`: accepted for compatibility; NDJSON search streams line-by-line
- returns: list of matches (JSON) or list of `{"line": int, "matches": list}` (NDJSON)

```python
import strata

ids = strata.search("users.json", "$.users[*].id")
matches = strata.search("orders.ndjson", "$.price")
# [{"line": 1, "matches": [9.99]}, {"line": 4, "matches": [12.5, 3.2]}]

data = {"users": [{"id": 1}, {"id": 2}]}
ids = strata.search(data, "$.users[*].id")

# Reuse an NDJSON cursor for repeated queries
import strata.ndjson as ndjson

cursor = ndjson.parse_ndjson_file("orders.ndjson")
matches = strata.search(cursor, "$.price")
```

### compile_path

Compile a JSONPath expression for repeated use.

```python
import strata

path = strata.compile_path("$.users[*].id")
ids = strata.search("users.json", path)
```

### query

Search a Python dict or list using JSONPath (alias of `search` for dict/list data).

- `data`: dict or list
- `expression`: JSONPath string
- returns: list of matches

```python
import strata

data = {"users": [{"id": 1}, {"id": 2}]}
ids = strata.query(data, "$.users[*].id")
```

### load

Load JSON or NDJSON from a file path or file-like object.

- Detects NDJSON via `.ndjson` extension or `ndjson=True`.
- Returns the parsed JSON value; NDJSON returns a list of records.

```python
import strata

doc = strata.load("users.json")
records = strata.load("users.ndjson")
```

### dump

Write JSON or NDJSON to a file path or file-like object.

- `ndjson`: True to write one JSON record per line (or use `.ndjson` extension)
- `return_type`: `'str'`, `'bytes'`, or `'bytearray'` (default chosen per target)
- returns: line count written (1 for JSON, N for NDJSON records)

```python
import strata

count = strata.dump("out.json", {"ok": True})
count = strata.dump("out.ndjson", [{"id": 1}, {"id": 2}], ndjson=True)
```

### loads

Parse JSON text into a Python object.

- `source`: str, bytes, bytearray, or memoryview

```python
import strata

obj = strata.loads('{"a": 1}')
```

### dumps

Serialize a Python object to JSON.

- `return_type`: `'str'`, `'bytes'`, or `'bytearray'`

```python
import strata

text = strata.dumps({"a": 1})
raw = strata.dumps({"a": 1}, return_type="bytes")
```
