# Strata

Fast JSON parsing, serialization, and JSONPath search for Python.

## Public API

`strata` exposes exactly six functions:

- `search(source, expression, **kwargs) -> list`
- `query(data, expression, **kwargs) -> list`
- `load(source, **kwargs) -> object`
- `dump(target, obj, **kwargs) -> int`
- `loads(source, **kwargs) -> object`
- `dumps(obj, **kwargs) -> str | bytes | bytearray`

### search

Search a JSON or NDJSON file for values matching a JSONPath expression.

- `source`: file path (str or Path) or file-like object
- `expression`: JSONPath string (e.g. `"$.users[*].id"`)
- `ndjson`: optional bool to force NDJSON mode
- `skip_errors`, `parallel`, `num_threads`: forwarded to NDJSON parsing when enabled
- returns: list of matches

```python
import strata

ids = strata.search("users.json", "$.users[*].id")
prices = strata.search("orders.ndjson", "$..price")
```

### query

Search a Python dict or list using JSONPath.

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
