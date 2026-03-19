# Strata API Reference

## C++ API

| Subsystem                | Reference                                |
| ------------------------ | ---------------------------------------- |
| SIMD Structural Indexing | [simd_api.md](simd_api.md)               |
| Speculative Parser       | [speculative_api.md](speculative_api.md) |
| Bloom Filters            | [bloom_api.md](bloom_api.md)             |

## Python API

## `strata.loads(source, *, return_type="dict", iterator=False)`

Parse JSON text into a Python object.

**Parameters:**

- `source` (`str | bytes`): JSON text to parse.
- `return_type` (`str`): `"dict"` (default) returns Python dict/list. `"cursor"` returns a `JsonCursor`.
- `iterator` (`bool`): If `True`, returns an iterator instead of the full result.

**Returns:**

- `dict | list` when `return_type="dict"`
- `JsonCursor` when `return_type="cursor"`
- Iterator when `iterator=True`

**Raises:**

- `ValueError`: Invalid JSON.

**Iterator behavior:**

- Object input: yields `(key, value)` tuples
- Array input: yields individual elements

```python
data = strata.loads('{"a": 1, "b": 2}')
# {'a': 1, 'b': 2}

cursor = strata.loads('{"a": 1}', return_type="cursor")
# JsonCursor object

for key, value in strata.loads('{"a": 1, "b": 2}', iterator=True):
    print(key, value)  # 'a' 1, 'b' 2
```

______________________________________________________________________

## `strata.dumps(obj, *, return_type="str")`

Serialize a Python object to JSON.

**Parameters:**

- `obj`: Python object to serialize (dict, list, str, int, float, bool, None).
- `return_type` (`str`): `"str"` (default) or `"bytes"`.

**Returns:**

- `str` when `return_type="str"`
- `bytes` when `return_type="bytes"`

**Raises:**

- `ValueError`: Invalid `return_type`.
- `TypeError`: Unserializable object.

```python
strata.dumps({"key": "value"})
# '{"key":"value"}'

strata.dumps({"key": "value"}, return_type="bytes")
# b'{"key":"value"}'
```

______________________________________________________________________

## `strata.load(filepath, *, return_type="dict", iterator=False)`

Load a JSON, NDJSON, or JSONL file.

**Parameters:**

- `filepath` (`str`): Path to `.json`, `.ndjson`, or `.jsonl` file.
- `return_type` (`str`): `"dict"` or `"cursor"` (JSON only, not NDJSON).
- `iterator` (`bool`): If `True`, returns an iterator.

**Returns:**

- JSON file: dict/list, JsonCursor, or iterator
- NDJSON/JSONL file: list of records, or streaming iterator

**Raises:**

- `FileNotFoundError`: File does not exist.
- `ValueError`: Invalid JSON or invalid `return_type`.

**Iterator behavior:**

- JSON object: yields `(key, value)` tuples
- JSON array: yields elements
- NDJSON: yields one parsed record per line (streaming)

```python
data = strata.load("users.json")
cursor = strata.load("users.json", return_type="cursor")

for record in strata.load("data.ndjson", iterator=True):
    process(record)  # One line at a time
```

______________________________________________________________________

## `strata.dump(obj, filepath)`

Write a Python object to a JSON file.

**Parameters:**

- `obj`: Python object to serialize.
- `filepath` (`str`): Output file path.

```python
strata.dump({"users": [1, 2, 3]}, "output.json")
```

______________________________________________________________________

## `strata.query(data, path, *, iterator=False)`

Query in-memory data using JSONPath.

**Parameters:**

- `data` (`dict | list`): Parsed JSON data. Must be a dict or list.
- `path` (`str | CompiledPath`): JSONPath expression or compiled path.
- `iterator` (`bool`): If `True`, yields results one at a time.

**Returns:** `list` of matched values, or iterator.

**Raises:**

- `TypeError`: `data` is not a dict or list.
- `ValueError`: Invalid JSONPath expression.

```python
data = {"users": [{"id": 1}, {"id": 2}]}
strata.query(data, "$.users[*].id")
# [1, 2]

strata.query("not a dict", "$.x")
# TypeError: query() requires dict or list
```

______________________________________________________________________

## `strata.search(filepath, path, *, iterator=False)`

Search a JSON/NDJSON/JSONL file using JSONPath.

**Parameters:**

- `filepath` (`str | Path`): Path to a `.json`, `.ndjson`, or `.jsonl` file.
- `path` (`str | CompiledPath`): JSONPath expression or compiled path.
- `iterator` (`bool`): If `True`, yields results one at a time.

**Returns:** `list` of matched values, or iterator.

**Raises:**

- `TypeError`: `filepath` is not a string/Path (e.g., dict or list passed).
- `FileNotFoundError`: File does not exist.
- `ValueError`: Invalid JSONPath or file extension.

```python
strata.search("users.json", "$.users[*].id")
# [1, 2, 3]

strata.search({"a": 1}, "$.a")
# TypeError: search() requires a file path
```

______________________________________________________________________

## `strata.compile_path(expression)`

Compile a JSONPath expression for reuse.

**Parameters:**

- `expression` (`str`): JSONPath expression starting with `$`.

**Returns:** `CompiledPath` object.

**Raises:**

- `ValueError`: Invalid JSONPath syntax.

```python
path = strata.compile_path("$.users[*].id")
r1 = strata.query(data1, path)
r2 = strata.query(data2, path)
```

______________________________________________________________________

## `strata.config`

Configuration management.

### `strata.config.set(key, value)`

Set a configuration value.

| Key                    | Type  | Values                                   | Default   |
| ---------------------- | ----- | ---------------------------------------- | --------- |
| `duplicate_key_policy` | `str` | `"first"`, `"last"`, `"error"`, `"warn"` | `"first"` |
| `cycle_policy`         | `str` | `"warn"`, `"error"`, `"ignore"`          | `"warn"`  |

**Raises:**

- `KeyError`: Unknown key.
- `TypeError`: Wrong type for key.
- `ValueError`: Invalid value.

### `strata.config.get(key) -> value`

Get a configuration value. Returns `None` for unknown keys.

### `strata.config.list() -> dict`

List all configuration keys and their current values.

______________________________________________________________________

## Supported JSONPath Syntax

| Syntax                  | Description       | Example            |
| ----------------------- | ----------------- | ------------------ |
| `$`                     | Root              | `$`                |
| `.field`                | Child field       | `$.name`           |
| `["field"]`             | Bracket notation  | `$["special-key"]` |
| `[n]`                   | Array index       | `$[0]`             |
| `[-n]`                  | Negative index    | `$[-1]`            |
| `[*]`                   | Wildcard (array)  | `$[*]`             |
| `.*`                    | Wildcard (object) | `$.*`              |
| `..field`               | Recursive descent | `$..price`         |
| `[start:end]`           | Array slice       | `$[1:4]`           |
| `[start:end:step]`      | Slice with step   | `$[::2]`           |
| `[?(@.field op value)]` | Filter predicate  | `$[?(@.age > 30)]` |

**Filter operators:** `==`, `!=`, `>`, `<`, `>=`, `<=`
