# Strata

Fast JSON for Python. High-performance parsing, serialization, and querying powered by C++.

## Install

```bash
pip install strata
```

## Quick Start

```python
import strata

# Parse JSON
data = strata.loads('{"users": [{"id": 1}, {"id": 2}]}')

# Serialize
json_str = strata.dumps(data)
json_bytes = strata.dumps(data, return_type="bytes")

# File I/O
data = strata.load("data.json")
strata.dump(data, "output.json")

# NDJSON/JSONL
records = strata.load("data.ndjson")

# JSONPath query (in-memory)
ids = strata.query(data, "$.users[*].id")

# JSONPath search (file-based)
ids = strata.search("data.json", "$.users[*].id")

# Compile path for reuse
path = strata.compile_path("$.users[*].id")
ids = strata.query(data, path)
```

## API Reference

### Parse & Serialize

```python
strata.loads(source: str | bytes, *, return_type="dict", iterator=False)
```

Parse JSON text. `return_type="cursor"` returns a `JsonCursor` for lazy navigation. `iterator=True` yields key-value tuples for objects, elements for arrays.

```python
strata.dumps(obj, *, return_type="str") -> str | bytes
```

Serialize to JSON. `return_type="bytes"` returns bytes.

### File I/O

```python
strata.load(filepath: str, *, return_type="dict", iterator=False)
```

Load `.json`, `.ndjson`, or `.jsonl` files. NDJSON returns a list of records. `iterator=True` streams one record at a time.

```python
strata.dump(obj, filepath: str) -> None
```

### JSONPath

```python
strata.query(data: dict | list, path: str | CompiledPath, *, iterator=False) -> list
```

Query in-memory data. Raises `TypeError` if data is not a dict or list.

```python
strata.search(filepath: str, path: str | CompiledPath, *, mem_eff=None, iterator=False) -> list
```

Search a JSON/NDJSON/JSONL file. Raises `TypeError` if filepath is not a string. `mem_eff=True` enables memory-efficient mode.

```python
strata.compile_path(expression: str) -> CompiledPath
```

Compile a JSONPath expression for reuse across multiple queries.

### Config

```python
strata.config.set(key, value)
strata.config.get(key) -> value
strata.config.list() -> dict
```

Keys:

- `mem_eff` (bool): Memory-efficient mode. Default: `False`.
- `duplicate_key_policy` (str): `"first"` | `"last"` | `"error"` | `"warn"`. Default: `"first"`.
- `cycle_policy` (str): `"warn"` | `"error"` | `"ignore"`. Default: `"warn"`.

### Iterator Support

All main functions accept `iterator=True`:

```python
# Object: yields (key, value) tuples
for key, value in strata.loads('{"a": 1, "b": 2}', iterator=True):
    print(key, value)

# Array: yields elements
for item in strata.loads('[1, 2, 3]', iterator=True):
    print(item)

# NDJSON: yields one record per line
for record in strata.load("data.ndjson", iterator=True):
    print(record)

# Query results: yields matches
for match in strata.query(data, "$.users[*].id", iterator=True):
    print(match)
```

## Benchmarks

Run benchmarks with:

```bash
make bench-small    # 1k users
make bench-medium   # 2k users
make bench-large    # 4k users
make bench-random   # Random schema shapes
make bench-all      # All sizes
```

Results are saved to `docs/benchmarks/`.

## Development

```bash
make dev            # Set up virtual environment
make test           # Run all tests
make bench-small    # Run benchmarks
make coverage       # Collect coverage
```

## License

See LICENSE file.
