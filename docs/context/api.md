# Public API Reference

Package exports (`python/strata/__init__.py` `__all__`): `loads`, `dumps`, `load`,
`dump`, `search`, `query`, `compile`, `config`, `__version__`.
Also importable: `JsonCursor`. Native module: `strata._strata`.

Deliberate changes vs the previous implementation: `compile_path` is renamed
`compile` (mirroring `re.compile`), and the extra entry points `parse_json`
and `parse_ndjson` are dropped — cursors come from
`loads`/`load` with `return_type="cursor"`, and NDJSON goes through `load`.

## Parse & serialize

```python
strata.loads(source: str | bytes, *, return_type="dict", iterator=False)
```
Parse JSON text. Default returns the full Python tree (`dict|list|str|int|float|bool|None`);
ints are exact within int64 range (no double squashing). Beyond int64: 19-digit
overflow raises `ValueError`; 20+ digit integers are silently mis-parsed (digits
past 19 dropped — known limitation). `return_type="cursor"` returns a lazy
`JsonCursor`. `iterator=True`: dict root yields `(key, value)`, list root yields
elements (eager parse, lazy consumption); scalar roots ignore the flag.
Raises `ValueError` (invalid JSON / bad `return_type`), `TypeError`, `RuntimeError`
(internal C++ error), `RuntimeWarning` under `duplicate_key_policy="warn"`.

```python
strata.dumps(obj, *, return_type="str") -> str | bytes
```
Compact serialization (no whitespace). Supports dict/list/tuple/str/int/float/bool/None;
dict keys must be `str` (else `TypeError`); NaN/±Inf serialize as `null`; big ints
beyond int64 are emitted via their str form. Raises `TypeError` (unsupported type),
`ValueError` ("Maximum serialization depth exceeded" at `sys.getrecursionlimit()`,
or cycle under `cycle_policy="error"`).

## File & folder I/O

```python
strata.load(path, *, return_type="dict", iterator=False)     # str | Path; file or directory
strata.dump(obj, path, *, split_by=None) -> None             # str | Path
```

**File mode** (`path` is a file): `load` dispatches on extension:
`.ndjson`/`.jsonl` → NDJSON (list of records; `iterator=True` parses lazily
line-by-line — the whole file is still read into memory up front — silently
skipping invalid lines; `return_type="cursor"` is a `ValueError`); anything
else → single JSON document. Raises `FileNotFoundError`, `OSError`,
`ValueError` ("Empty file" for JSON). `dump` writes compact JSON + trailing
newline, mode 0644, truncating; `split_by` with a file path is a `ValueError`.

**Folder mode:**

- `load(dirpath)` loads every `*.json`/`*.ndjson`/`*.jsonl` under the
  directory (recursive; other/hidden entries ignored) and returns one list:
  each file's records concatenated in lexicographic relative-path order — a
  `.json` file with a list root contributes its elements, any other root
  contributes the document itself, NDJSON contributes its lines.
  `iterator=True` streams records lazily file-by-file; `return_type="cursor"`
  → `ValueError`. Empty directory → `[]`.
- `dump(records, dirpath, split_by=key_or_keys)` splits a list of dicts into
  files grouped by the value(s) of the given key(s) (`str` or sequence of
  `str`). One key → `dirpath/<value>.json`; N keys → nested directories, one
  level per key, file for the last: `dirpath/<v1>/<v2>.json`. Each file is a
  compact JSON array of that group's records (+ trailing newline), preserving
  input order. Directories are created as needed; colliding files are
  overwritten; unrelated existing files are untouched. Split values must be
  `str`/`int`/`bool` scalars; a record missing a split key, or a value whose
  string form is path-unsafe (empty, `.`, `..`, or containing `/`, `\`, NUL)
  → `ValueError`; non-list `obj` or non-dict record → `TypeError`.
- Round-trip law: `dump(records, d, split_by=ks)` followed by `load(d)`
  returns the same records, grouped in lexicographic key-path order with
  intra-group order preserved.
- Folder mode is **new in the target API** — the previous implementation was
  single-file only, so there is no reference code for it. Per the
  conventions, grouping and serialization live in C++; the facade only
  normalizes `Path` → `str`.

## JSONPath

```python
strata.query(data: dict | list, expression: str | CompiledPath, *, iterator=False) -> list
strata.search(filepath: str | Path, expression: str | CompiledPath, *, iterator=False) -> list
strata.compile(expression: str) -> CompiledPath
```
`query` evaluates directly on Python objects (dict/list/tuple roots only,
else `TypeError`). `search` operates on files (must end `.json`/`.ndjson`/`.jsonl`,
else `TypeError`); `.json` uses streaming SAX search (only matches materialized)
for plain paths — Filter/Slice paths fall back to a full parse of the document,
and NDJSON search materializes each line. Invalid expressions raise
`ValueError("Invalid JSONPath expression")`.

Supported grammar (subset of RFC 9535): `$` (mandatory root), `.field`
(`[A-Za-z0-9_]+`), `["field"]`/`['field']`, `[n]` (negative ok), `[*]`, `.*`,
`..field` (recursive descent, identifier only), `[start:end:step]` (positive step
only), `[?(@.field op value)]` with `== != > >= < <=` (numeric; strings `==`/`!=`
only). **Not** supported: unions, `&&`/`||`, `$..*`, nested filter paths,
existence filters, negative slice step (silently returns `[]`).

## Cursor

```python
strata.loads(source, return_type="cursor") -> JsonCursor   # or load(fp, return_type="cursor")
```
`JsonCursor`: `is_null/is_bool/is_number/is_string/is_array/is_object()`,
`get_bool/get_int/get_float/get_str()` (type mismatch → `RuntimeError`),
`field(key)`, `at(index)`. Missing key → `RuntimeError("field not found")`;
index out of bounds → `RuntimeError("index out of range")` (raised immediately;
test-pinned). `CompiledPath.execute(cursor)` runs a compiled path against a cursor.

## Config

```python
strata.config.set(key, value); strata.config.get(key); strata.config.list()
```
- `duplicate_key_policy`: `"first"` (default) | `"last"` | `"error"` | `"warn"`
- `cycle_policy`: `"warn"` | `"error"` | `"ignore"` — **known bug:** the config
  map is seeded with `"warn"` but the serializer's actual startup behavior is
  `ignore` until `config.set("cycle_policy", ...)` is called once.

Config state is process-global at the map level. `duplicate_key_policy` is
consumed via a **thread-local** variable — it does not propagate to other
threads. `cycle_policy` is a plain process-global — it affects all threads.

## Error contract (test-pinned messages)

Parse errors ⇒ `ValueError("Invalid JSON")`. Cursor misuse ⇒ `RuntimeError`
matching "not an object" / "not an array" / "not a bool|number|string" /
"not found" / "out of range". Unknown config key ⇒ `KeyError` on `config.set`
(`config.get` returns `None`); bad config value ⇒ `ValueError`; wrong value
type ⇒ `TypeError`.

## C++ public surface (for binding work)

`parse_json(string_view) -> Result<JsonValue>` · `parse_sax(string_view, JsonSaxHandler&, bool validate_utf8=true)`
· `parse_sax_inline<Handler>(...)` (devirtualized) · `JsonDocument::from_string` /
`root()` · `JsonCursor` (status-code API + throwing API) · `serialize_json(const JsonValue&)`
· `NdjsonStream` (borrows its buffer; `next()` signals EOF via `Status::KeyNotFound`)
· `compile_jsonpath` / `eval_jsonpath`. Full details: `docs/architecture/SKILL.md`.
