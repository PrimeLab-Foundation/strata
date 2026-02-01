# Strata API - Simplified Reference

**Version**: 0.2.0\
**Date**: 2026-01-30

______________________________________________________________________

## 🎯 **Design Philosophy**

Strata provides a **simple, intuitive API** with sensible defaults:

- **Minimal**: Small surface area, easy to learn
- **Consistent**: Similar patterns across all functions
- **Fast**: C++ core, zero-copy where possible
- **Flexible**: Supports both simple and advanced use cases

______________________________________________________________________

## 📦 **Core API**

### **JSON Parsing & Serialization**

```python
import strata

# Parse JSON → Python object
data = strata.loads('{"name": "Alice", "age": 30}')
# → {'name': 'Alice', 'age': 30.0}

# Serialize Python object → JSON string
json_str = strata.dumps({"name": "Bob", "age": 25})
# → '{"name":"Bob","age":25}'

# Serialize to bytes (faster, recommended)
json_bytes = strata.dumps_bytes({"x": 1})
# → b'{"x":1}'
```

**Simple, familiar API** - Just like `json.loads()` and `json.dumps()`!

______________________________________________________________________

### **Configuration**

```python
# Duplicate object keys while parsing
strata.set_duplicate_key_policy("first")  # first (default), last, error, warn

# Cycles during serialization
strata.set_cycle_policy("warn")  # warn (default), error, ignore
```

**Notes**:

- Duplicate keys are detected during parsing; policy controls warning/error behavior.
- Cycles are handled during serialization; warn/ignore emit `null` at the cycle.

### **NDJSON Streaming**

```python
# Lazy iteration (memory efficient)
for line in strata.iter_ndjson(data):
    print(line)

# Parse all at once (faster for small files)
lines = strata.parse_ndjson(data)
```

**Use case**: Log files, streaming data, large datasets

______________________________________________________________________

### **JSONPath Queries**

```python
data = {"users": [{"id": 1, "name": "Alice"}, {"id": 2, "name": "Bob"}]}

# Simple query
names = strata.search(data, "$.users[*].name")
# → ['Alice', 'Bob']

# Pre-compile for reuse (faster)
path = strata.compile_path("$.users[*].id")
ids1 = strata.search(data1, path)
ids2 = strata.search(data2, path)
```

**Key insight**: Both string paths and CompiledPath objects work with `search()`!

______________________________________________________________________

## 🚀 **Advanced API** (Optional)

### **Memory-Mapped I/O**

```python
from strata import mmap_io

# Zero-copy file parsing (great for large files)
cursor = mmap_io.parse_json_file("large_file.json")
data = cursor.to_python()
```

### **Document/Cursor API**

```python
# Low-level access for advanced use cases
doc = strata.JsonDocument(...)
cursor = strata.JsonCursor(...)
```

**Note**: Most users don't need these - use `loads()` and `search()` instead!

______________________________________________________________________

## 📊 **API Comparison**

### Before (Complex):

```python
# Too many ways to do the same thing
from strata.serialize import loads, dumps
from strata.ndjson import NdjsonStream
from strata.jsonpath import compile_path, CompiledPath

stream = NdjsonStream.from_string(data)
while stream.has_next():
    line = stream.next_line()
    
path = compile_path("$.users[*].id")
# Can't use path with search()? Confusing!
```

### After (Simple):

```python
# One obvious way
import strata

# Everything just works
for line in strata.iter_ndjson(data):
    process(line)

path = strata.compile_path("$.users[*].id")
results = strata.search(data, path)  # ✅ Works!
```

______________________________________________________________________

## ✅ **Key Improvements**

1. **Unified `search()` function**

   - Accepts both string paths and CompiledPath objects
   - No need to remember which function to use

1. **Simple imports**

   - Everything from `import strata`
   - No submodule confusion

1. **Consistent naming**

   - `loads`/`dumps` (like stdlib `json`)
   - `iter_ndjson`/`parse_ndjson` (clear intent)
   - `search`/`compile_path` (intuitive)

1. **Sensible defaults**

   - No required parameters beyond the obvious
   - Works out of the box

______________________________________________________________________

## 🎯 **Migration Guide**

### From stdlib `json`:

```python
# Before
import json
data = json.loads(text)
text = json.dumps(data)

# After - drop-in replacement
import strata
data = strata.loads(text)
text = strata.dumps(data)
```

### From `orjson`:

```python
# Before
import orjson
data = orjson.loads(bytes_data)
bytes = orjson.dumps(data)

# After
import strata
data = strata.loads(bytes_data)  # Accepts str or bytes
bytes = strata.dumps_bytes(data)  # Explicit bytes output
```

______________________________________________________________________

## 📖 **Complete API Reference**

| Function                           | Input                   | Output        | Use Case                    |
| ---------------------------------- | ----------------------- | ------------- | --------------------------- |
| `loads(data)`                      | str/bytes               | Python object | Parse JSON                  |
| `dumps(obj)`                       | Python object           | str           | Serialize to string         |
| `dumps_bytes(obj)`                 | Python object           | bytes         | Serialize to bytes (faster) |
| `iter_ndjson(data)`                | str/bytes               | Iterator      | Stream NDJSON lines         |
| `parse_ndjson(data)`               | str/bytes               | List          | Parse all NDJSON            |
| `search(data, path)`               | data + str/CompiledPath | List          | Query with JSONPath         |
| `compile_path(expr)`               | str                     | CompiledPath  | Pre-compile path            |
| `set_duplicate_key_policy(policy)` | str                     | None          | Duplicate-key strategy      |
| `set_cycle_policy(policy)`         | str                     | None          | Cycle strategy in dumps     |

______________________________________________________________________

## 🏆 **Result**

**Simple, fast, intuitive** - That's the Strata promise! 🚀

______________________________________________________________________

*API Design: 2026-01-30*\
*Philosophy: Less is more*
