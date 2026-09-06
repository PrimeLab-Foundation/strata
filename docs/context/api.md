# Public API Reference

Package exports (`python/strata/__init__.py` `__all__`): `loads`, `dumps`, `load`,
`dump`, `search`, `query`, `compile`, `config`, `__version__`.
Also importable: `JsonCursor`. Native module: `strata._strata`.

Deliberate changes vs the previous implementation: `compile_path` is renamed
`compile` (mirroring `re.compile`), and the extra entry points `parse_json`
and `parse_ndjson` are dropped — cursors come from
`loads`/`load` with `return_type="cursor"`, and NDJSON goes through `load`.

Versioning is calver (`YYYY.M.D` of release — orders correctly under
PEP 440). The rebuild started at `__version__ = "2026.8.9"` and released as
`2026.8.10` on the quiet-machine standings sweep, bumped at release
time only. Single source of truth: the literal in
`python/strata/__init__.py`; pyproject reads it dynamically — no second copy
anywhere (the previous implementation drifted across three locations).

## Parse & serialize

```python
strata.loads(source: str | bytes, *, return_type="dict", iterator=False)
```

Parse JSON text. Default returns the full Python tree (`dict|list|str|int|float|bool|None`);
integers parse **exactly at any size** (no double squashing; beyond int64 a
slow path builds the arbitrary-precision int — matches stdlib `json`; the
previous implementation mis-parsed 20+ digit ints, do not reproduce).
Invalid UTF-8 in `bytes` input ⇒ `ValueError("Invalid JSON")` — for bytes the
parser is the only validator. `return_type="cursor"` returns a lazy
`JsonCursor`. `iterator=True`: dict root yields `(key, value)`, list root yields
elements (eager parse, lazy consumption); scalar roots ignore the flag.

**Nesting is capped at 1024 open containers.** A document nesting deeper —
counting arrays and objects alike, so `[1]` is depth 1 and `[[1]]` depth 2 —
raises `ValueError("Maximum nesting depth exceeded")`. The parser recurses, so
the cap is what makes a deep document an error instead of a dead process; it
applies to every parsing entry point (`loads` for `str` and `bytes`, `load`,
each NDJSON line, `search`) and to both builders (the Python tree and
`return_type="cursor"`). Depth exactly 1024 parses. For comparison, orjson's cap is the same 1024 (it refuses the 1025th), and stdlib `json` is bounded by the interpreter's recursion guard, which varies by version.

Raises `ValueError` (invalid JSON / nesting past the cap / bad `return_type`),
`TypeError`, `RuntimeError` (internal C++ error), `RuntimeWarning` under
`duplicate_key_policy="warn"`.

```python
strata.dumps(obj, *, return_type="str") -> str | bytes
```

Compact serialization (no whitespace). Supports dict/list/tuple/str/int/float/bool/None;
dict keys must be `str` (else `TypeError`); NaN/±Inf serialize as `null`; big ints
beyond int64 are emitted via their str form. Raises `TypeError` (unsupported type),
`ValueError` ("Maximum serialization depth exceeded" at `sys.getrecursionlimit()`,
or cycle under `cycle_policy="error"`). The serializer's limit is the
interpreter's recursion limit (1000 by default), the parser's is 1024
containers, so a tree parsed at depth 1001–1024 needs a raised
`sys.setrecursionlimit` to serialize again — unchanged from before the parse
cap, and stated so the asymmetry is not a surprise.

## File & folder I/O

```python
strata.load(path, *, return_type="dict", iterator=False, skip_errors=False)  # str | Path; file or dir
strata.dump(obj, path, *, split_by=None) -> None                             # str | Path
```

**File mode** (`path` is a file): `load` dispatches on extension:
`.ndjson`/`.jsonl` → NDJSON list of records; anything else → single JSON
document. **Invalid NDJSON lines raise `ValueError` unless
`skip_errors=True`** (opt-in silencing — uniform across eager, iterator, and
folder modes; the previous implementation silently skipped, do not
reproduce). `iterator=True` parses lazily line-by-line (the whole file is
still read into memory up front; errors surface at the failing line);
`return_type="cursor"` on NDJSON is a `ValueError`. The 1024-container nesting
cap applies per document and per NDJSON line, and NDJSON names the line:
`ValueError("Maximum nesting depth exceeded on line N")`, skipped like any
other bad line under `skip_errors=True`. Raises
`FileNotFoundError`, `OSError`, `ValueError` ("Empty file" for JSON).
`dump` writes compact JSON + trailing newline, mode 0644, truncating;
`split_by` with a file path is a `ValueError`.

**Folder mode:**

- Discovery (shared by `load` and `search`): every `*.json`/`*.ndjson`/
  `*.jsonl` under the directory, recursive; extensions matched
  case-insensitively; hidden files and hidden directories pruned; symlinks
  **not followed**; ordering is bytewise on the `/`-joined relative path.
- `load(dirpath)` returns one list: each file's records concatenated in
  discovery order — a `.json` file with a list root contributes its elements,
  any other root contributes the document itself, NDJSON contributes its
  lines. Per-file errors follow `skip_errors` (False → propagate at the point
  the file is consumed; True → skip the offending file/line).
  `iterator=True` streams records lazily file-by-file; `return_type="cursor"`
  → `ValueError`. Empty directory → `[]`.
- `dump(records, dirpath, split_by=key_or_keys)` splits a list of dicts into
  files grouped by the value(s) of the given key(s) (`str` or sequence of
  `str`). One key → `dirpath/<value>.json`; N keys → nested directories, one
  level per key, file for the last: `dirpath/<v1>/<v2>.json`. Each file is a
  compact JSON array of that group's records (+ trailing newline), preserving
  input order. A directory target without `split_by` → `ValueError`.
- Split values must be `str`/`int`/`bool` scalars. **Grouping is by the JSON
  string form**: `str` as-is, `int` as decimal digits, `bool` as
  `true`/`false`. Distinct raw values whose string forms collide (e.g. `1` vs
  `"1"`, `True` vs `"true"`), or names differing only by case (case-insensitive
  filesystems), → `ValueError`. Missing split key, or a string form that is
  path-unsafe (empty, `.`, `..`, contains `/`, `\`, NUL) → `ValueError`;
  non-list `obj` or non-dict record → `TypeError`. Empty `records` creates
  `dirpath` and writes nothing (so `load` → `[]`). Directories are created as
  needed; colliding files from *previous* runs are overwritten; unrelated
  existing files are untouched.
- Round-trip law: for records whose floats are all finite (NaN/±Inf serialize
  as `null` and lose identity), `dump(records, d, split_by=ks)` followed by
  `load(d)` returns the same records, grouped in bytewise key-path order with
  intra-group order preserved.
- Folder mode is **new in the target API** — the previous implementation was
  single-file only, so there is no reference code for it. Per the
  conventions, grouping and serialization live in C++; the facade only
  normalizes `Path` → `str`.

## JSONPath

```python
strata.query(data: dict | list, expression: str | CompiledPath, *, iterator=False) -> list
strata.search(path: str | Path, expression: str | CompiledPath, *, iterator=False) -> list
strata.compile(expression: str) -> CompiledPath
```

`query` evaluates directly on Python objects (dict/list/tuple roots only,
else `TypeError`). `search` operates on a file or a directory. A file must end
`.json`/`.ndjson`/`.jsonl` (else `TypeError`); `.json` uses streaming SAX
search (only matches materialized) for plain paths — Filter/Slice paths fall
back to a full parse of the document, and NDJSON search materializes each
line. Invalid expressions raise `ValueError("Invalid JSONPath expression")`.

**Folder mode:** `search(dirpath, expr)` uses the folder-discovery rules
defined under File & folder I/O and concatenates the matches — equivalent to
running `search` on each discovered file in that order. Per-file semantics
are unchanged (SAX streaming where the path allows it); the expression is
compiled once and reused across files. `iterator=True` streams matches lazily
file-by-file; empty directory → `[]`.

Supported grammar (subset of RFC 9535): `$` (mandatory root), `.field`
(`[A-Za-z0-9_]+`), `["field"]`/`['field']`, `[n]` (negative ok), `[*]`, `.*`,
`..field` (recursive descent, identifier only), `[start:end:step]` (positive step
only), `[?(@.field op value)]` / `[?(@['field'] op value)]` with
`== != > >= < <=` (numeric; strings `==`/`!=` only). **Not** supported:
unions, `&&`/`||`, `$..*`, nested filter paths, existence filters, negative
slice step (silently returns `[]`). All invalid expressions — including
unclosed quotes — raise `ValueError` (the previous implementation leaked
`RuntimeError` for unclosed quotes; do not reproduce). Implementation detail
and edge cases: `docs/jsonpath/SKILL.md`.

## Cursor

```python
strata.loads(source, return_type="cursor") -> JsonCursor   # or load(fp, return_type="cursor")
```

`JsonCursor`: `is_null/is_bool/is_number/is_string/is_array/is_object()`,
`get_bool/get_int/get_float/get_str()` (type mismatch → `RuntimeError`),
`field(key)`, `at(index)`. Missing key → `RuntimeError("field not found")`;
index out of bounds → `RuntimeError("index out of range")` (raised immediately;
test-pinned). The returned `JsonCursor` holds a strong reference to its owning
document — the C++ document-outlives-cursor invariant is satisfied inside the
binding, never exposed to the user. `CompiledPath.execute(cursor)` runs a
compiled path against a cursor.

## Config

```python
strata.config.set(key, value); strata.config.get(key); strata.config.list()
```

- `duplicate_key_policy`: `"first"` (default) | `"last"` | `"error"` | `"warn"`
- `cycle_policy`: `"warn"` (default, **active from process start** — reported
  and actual behavior always agree; the previous implementation started on
  `ignore` while reporting `warn`, do not reproduce) | `"error"` | `"ignore"`.
  On an actual cycle: `"warn"` emits `null` for the cyclic reference and
  raises `RuntimeWarning`; `"error"` raises `ValueError`; `"ignore"` emits
  `null` silently.

Config state is process-global at the map level. `duplicate_key_policy` is
consumed via a **thread-local** variable — it does not propagate to other
threads. `cycle_policy` is a plain process-global — it affects all threads.

## Error contract (test-pinned messages)

Parse errors ⇒ `ValueError("Invalid JSON")`; malformed NDJSON lines ⇒
`ValueError("Invalid JSON on line N")`. Nesting past 1024 containers ⇒
`ValueError("Maximum nesting depth exceeded")`, and on an NDJSON line
`ValueError("Maximum nesting depth exceeded on line N")` — a refusal, kept
distinct from malformed input so a caller can tell the two apart.
Cursor misuse ⇒ `RuntimeError`
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
