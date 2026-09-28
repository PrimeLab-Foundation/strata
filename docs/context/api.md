# Public API Reference

Package exports (`python/strata/__init__.py` `__all__`): `loads`, `dumps`,
`dumps_with_default`, `load`, `dump`, `search`, `query`, `compile`, `config`,
`__version__`. Also importable: `JsonCursor`. Native modules: `strata._strata`,
and `strata._dumps_hook` (the serializer with the unsupported-type hook, imported
by the first `dumps_with_default` call only).

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
strata.loads(source: str | bytes, *, return_type="dict", iterator=False, parse_types=False)
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
`duplicate_key_policy="warn"`. `parse_types` revives dates, times, UUIDs and
registered types after the parse: see `parse_types` below.

```python
strata.dumps(obj, *, return_type="str") -> str | bytes
```

Compact serialization (no whitespace). Supports
dict/list/tuple/str/int/float/bool/None and the native types below; dict keys
must be `str` (else `TypeError`, a native key included); NaN/±Inf serialize as
`null`; big ints beyond int64 are emitted via their str form.

**Native types** (design record: `docs/architecture/native_types.md`), on by
default in `dumps`, `dump` and `dumps_with_default`, checked in this order and
only after every branch above — so an `int`, `str`, `float`, `dict`, `list` or
`tuple` **subclass** (an `IntEnum`, a `class E(str, Enum)`, `numpy.float64`) is
written by its base type's rule first:

| #   | Type (subclasses included unless marked exact)                               | Written as                                                                                                                                                                                                                                                                                                                 |
| --- | ---------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 1   | `datetime.datetime` (exact type)                                             | `"YYYY-MM-DDTHH:MM:SS"`, `.ffffff` when the microsecond is nonzero, then `+HH:MM`/`-HH:MM` when aware (tzinfo set and `utcoffset()` not `None`; UTC is `+00:00`). The offset is `days*86400 + seconds` of `tzinfo.utcoffset(dt)` (so `fold` reaches it; microseconds dropped), rounded to the minute half-up in magnitude. |
| 2   | `datetime.date` (exact type)                                                 | `"YYYY-MM-DD"` (years zero-padded to four digits)                                                                                                                                                                                                                                                                          |
| 3   | `datetime.time` (exact type)                                                 | `"HH:MM:SS"`, `.ffffff` when nonzero, and the offset of row 1 when aware (`tzinfo.utcoffset(None)`)                                                                                                                                                                                                                        |
| 4   | `uuid.UUID`                                                                  | `"xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"`, lowercase, from `.int`                                                                                                                                                                                                                                                           |
| 5   | `decimal.Decimal`                                                            | the text of `str(d)` as a raw JSON number (`1.50`, `1E+2`, `-0`); `NaN`, `sNaN` and `±Infinity` → `null`                                                                                                                                                                                                                   |
| 6   | `enum.Enum`                                                                  | `member.value` (read with `getattr`) in the member's place; a value that is itself an `Enum` is followed, up to the depth limit                                                                                                                                                                                            |
| 7   | dataclass instance                                                           | a JSON object of exactly the fields `dataclasses.fields()` lists, in order, each read with `getattr` (extra instance attributes are not written)                                                                                                                                                                           |
| 8   | `set`, `frozenset`                                                           | a JSON array in iteration order                                                                                                                                                                                                                                                                                            |
| 9   | numpy `bool_`/`integer`/`floating` scalar; `ndarray` of dtype kind `b i u f` | `obj.item()` / `obj.tolist()` in the object's place (any shape and strides, 0-d included; `float32` widens exactly: `float32(0.1)` → `0.10000000149011612`)                                                                                                                                                                |

Rows 1–3 take the exact types only, as orjson does: a `datetime`, `date` or
`time` subclass can carry state its fields do not (pandas' `NaT`, a
`Timestamp`'s nanoseconds), so it is unsupported — the `TypeError` below, or
`default` in `dumps_with_default` (`lambda o: o.isoformat()` is the usual one).
A dataclass, a set or an Enum member opens a container on the object itself:
it takes one level of the depth limit, and a dataclass that contains itself —
or a member met again below its own value, as when `default` returns the member
whose value is the object it was called on — follows `cycle_policy` as a dict
does. The hops of an Enum chain take no level; the chain has its own bound.
`import strata` imports none of `datetime`, `uuid`, `decimal`,
`dataclasses` or `numpy`: the types are looked up in `sys.modules` when a
document first needs them, never imported. `datetime.timedelta`, `complex`,
`bytes`, every other numpy kind, and every other type still raise
`TypeError("Object of type %s is not JSON serializable")`; `dump`'s `split_by`
values stay `str`/`int`/`bool`.

Raises `TypeError` (unsupported type), `ValueError`
("Maximum serialization depth exceeded" at `sys.getrecursionlimit()`, or cycle
under `cycle_policy="error"`), `UnicodeEncodeError` (a `str` key or value with
no UTF-8 encoding — a lone surrogate; the output is UTF-8, so strata refuses it
on **every** call and in both return types, and nothing is written to the
destination of a `dump`; stdlib `json` escapes it or passes it through under
`ensure_ascii=False`, orjson refuses it as `TypeError`). The serializer's limit
is the interpreter's recursion limit (1000 by default), the parser's is 1024
containers, so a tree parsed at depth 1001–1024 needs a raised
`sys.setrecursionlimit` to serialize again — unchanged from before the parse
cap, and stated so the asymmetry is not a surprise.

**Mutation during serialization.** User code can run inside `dumps` at five
steps. Four are rare: the `RuntimeWarning` under `cycle_policy="warn"` (a
warnings filter or `showwarning` hook); `__str__` of an `int` subclass beyond
int64; the decimal conversion of an **exact** `int` beyond int64 that CPython
3.12+ delegates to the `_pylong` Python module — reached above roughly 10 000
digits, so `sys.set_int_max_str_digits` has to permit it, and it imports modules
and runs bytecode; and, as a consequence of any of those, a `__del__` or a
weakref callback fired when the serializer releases what that code orphaned.
The fifth is a native conversion, and only in a document that holds a native
object: looking the native types up in `sys.modules`, a
non-`datetime.timezone` tzinfo's `utcoffset()`, a UUID subclass's `int`,
`Decimal`'s `str()`, an `Enum`'s `value`, the dataclass field lookup and each
field read, a set's iterator, and numpy's dtype, `item()` and `tolist()` — any
of which can also allocate and so run a collection. An exact
`datetime`/`date`/`time` whose tzinfo is `None` or exactly `datetime.timezone`,
and an exact `uuid.UUID`, are formatted without running any of it. Nothing else
in a successful `dumps` calls into Python or allocates an object the collector
tracks, so no collection can run one either. A set resized while it is written
raises the `RuntimeError` its iterator raises; a dataclass's fields are read one
at a time as they are written. If that code mutates
a container being written, `dumps` never reads freed memory: lists and tuples
are followed live, element by element, as stdlib `json` does (a shrunk list ends
there, appended elements are written); a dict of at most 24 exact-`str` keys,
below 64 levels of dict nesting, is emitted as the row read on entry (the only
rule the general and the fused record writer can both produce byte-identically —
this diverges from stdlib `json`, which walks dicts live); wider dicts and dicts
with `str`-subclass keys are followed live. Output on unmutated input is
unchanged.

```python
strata.dumps_with_default(obj, default, *, return_type="str") -> str | bytes
```

`dumps` with a hook for unsupported types (design record:
`docs/architecture/dumps_with_default.md`). For every object `dumps` supports the
output is `dumps`'s, byte for byte; each object of any other type is passed to
`default` once and its return value is serialized in the object's place — as a
value, at that object's depth, so a returned container one level past the limit
raises "Maximum serialization depth exceeded" and a returned container that is
already open is a cycle under the active `cycle_policy`, reported where it was
returned (the array element loop's placement caveat under Config does not apply
to it). The rules, each test-pinned:

- `default` is required, positional or keyword, and must be callable; anything
  else — **`None` included** — raises `TypeError("default must be callable, not %s")` before any byte is produced. Missing it, passing it twice, or an unknown
  keyword raise `TypeError` as for any Python function.
- A document with no unsupported object is byte-identical to `dumps(obj)` in
  both return types, and `default` is never called.
- **Native types come first.** They are part of what `dumps` supports, so
  `default` is **never called for a native object** (a caller whose `default`
  formatted a `datetime` or a `Decimal` gets strata's formatting — orjson's
  precedence too), and a native object `default` returns is written natively.
  A value reached *inside* a native — an Enum's `value`, a dataclass field, a
  set element — is an ordinary position and gets its own call when it is
  unsupported.
- `default` raises ⇒ that exception **propagates unchanged**: same object, same
  type and args, no wrapping or chaining (`KeyboardInterrupt`, `MemoryError` and
  `SystemExit` included).
- **Chain bound 1.** `default` returns an unsupported object ⇒
  `TypeError("default() returned an object of type %s that is not JSON serializable")`, and `default` is **not** called on its own return. This
  diverges from stdlib `json` (which re-enters `default` without limit) and from
  orjson (up to 254 times); a caller who wants a chain writes the loop inside the
  callable. Unsupported objects *nested inside* a returned container are ordinary
  positions and get their own call.
- `default` returns `None` ⇒ `null`; a returned `str` with no UTF-8 encoding ⇒
  `UnicodeEncodeError`, as for any other `str`.
- **Keys are excluded.** A non-`str` dict key raises the unchanged
  `TypeError("keys must be str, not %s")`; `default` is never called for a key.
- `return_type` is `dumps`'s: `"str"` or `"bytes"`, else `ValueError("invalid return_type: %s")`. There is no file counterpart: `dump` has no hook.
- The hook's own extension image (`strata._dumps_hook`) is imported on the first
  call, not by `import strata`. If it cannot be imported, that call — and every
  later one — raises the `ImportError`; the rest of the package is unaffected.

**Mutation during `dumps_with_default`.** User code can run at six steps: the
five of `dumps` above, and `default`, once per unsupported object — not rare,
since running it is the point. `default` allocates what it likes, so inside a
successful call a collection — and every `__del__` it fires — can run, as can a
`__del__` or weakref callback fired when the serializer releases the reference
`default` returned. The writers' rules above hold unchanged: lists and tuples
are followed live; a dict of at most 24 exact-`str` keys below 64 levels of dict
nesting is emitted as the row read on entry; wider dicts and dicts with
`str`-subclass keys are followed live. `dumps` itself can reach no hook, so its
clause above stays at five steps.

## `parse_types` (opt-in, parse side)

`loads`, `load`, `search` and `query` take `parse_types=False` (design record:
`docs/architecture/native_types.md`, "Parse contract"). The default is today's
behaviour, bit for bit: every default call runs the code it ran before. A set
option never reaches the parser or the builder; the document is parsed exactly
as by default and a separate walk then revives the freshly built tree in place.
Anything but the `False` object is validated as the option, so `0` and `None`
are a `TypeError`.

With `parse_types=True`, every JSON **string value** (never a key) that is
exactly one of these is replaced; anything else — including a string that
matches the grammar but names an impossible value (`2024-02-30`, a leap second
`23:59:60`, year `0000`) — stays a `str`:

| Kind      | Grammar (RFC 3339 §5.6, strict)                                            | Becomes                                                                                                          |
| --------- | -------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------- |
| date-time | `YYYY-MM-DD` `T`\|`t` `HH:MM:SS` \[`.` 1–6 digits\] \[`Z`\|`z`\|`±HH:MM`\] | `datetime`; naive without an offset; `Z`, `z`, `+00:00` and `-00:00` → `timezone.utc`; others a fixed `timezone` |
| date      | `YYYY-MM-DD`                                                               | `date`                                                                                                           |
| time      | `HH:MM:SS` \[`.` 1–6 digits\] \[`Z`\|`z`\|`±HH:MM`\]                       | `time`, aware with a fixed `timezone` when an offset is present                                                  |
| UUID      | 8-4-4-4-12 hexadecimal digits, hyphenated, either case                     | `uuid.UUID`                                                                                                      |

A fraction of 7 or more digits stays a `str` (no silent truncation); so does a
space separator, a `+HH:MM:SS` offset, `HH:MM` without seconds, surrounding
whitespace, or any non-ASCII character. Every string `dumps` writes for a
`datetime`, `date`, `time` or `UUID` is recognized, so
`loads(dumps(x), parse_types=True) == x` holds for naive values, for
`timezone`-aware values with whole-minute offsets, and for UUIDs; any other
tzinfo (a `ZoneInfo`) comes back as the fixed offset it had.

**Registry** (`parse_types={"name": T, ...}`, implies `True`): keys must be
`str`, values `Enum` subclasses or dataclass types, checked before parsing; the
dict is copied first, so mutating it during the call changes nothing. The walk
is post-order — a container's contents are revived before the container — and
applies at every depth to each JSON object member whose name is registered:

- an `Enum` `E`: the value becomes `E(value)`; a `ValueError` (no member has
  that value) leaves it as parsed; any other exception propagates;
- a dataclass `D`: a dict value whose keys are all init fields of `D` (the
  `dataclasses.fields()` entries with `init=True`) and include every one with
  neither a default nor a default factory becomes `D(**value)`; any other value
  is left as parsed; exceptions from `D`'s `__init__` or `__post_init__`
  propagate;
- a list value has each element revived by the same rule, one level;
- the value under a registered name is **never** type-recognized itself; its
  contents (a dict's members, a list element's items) follow the ordinary rules.

Per entry point:

- `loads`, `load` (file, NDJSON line by line, folder record by record, eager
  and `iterator=True`): the tree is revived before it is returned or iterated;
  the lazy NDJSON and folder iterators revive each record as they yield it.
  `return_type="cursor"` with `parse_types` set →
  `ValueError("parse_types needs return_type='dict'")`. `skip_errors` still
  covers invalid JSON only; an exception from a registered type propagates.
- `search` (file and folder): `search(f, e, parse_types=p) == query(load(f, parse_types=p), e)`
  for every supported expression — a `.json` file takes the full-parse path
  (parse, revive, evaluate) whatever the expression, an NDJSON file is revived
  line by line before it is evaluated, and a folder concatenates its files'
  results in discovery order (lazily with `iterator=True`). Streaming is for
  the default only. A scalar root matches `$` alone, as on the default path.
- `query`: `parse_types` is a `bool` only. `True` replaces each match that is a
  `str` (subclasses included) by the rule above, in the returned list;
  container and other matches are the caller's own objects, never copied or
  mutated.

The first call with `parse_types` set imports `datetime` and `uuid` if they are
not already imported (the only imports strata makes on its own behalf);
`import strata` still imports neither. Registered types run user code inside
the walk; the walk holds a strong reference to every entry it converts and to
the registry entry it applies, and writes a result back only into a slot or key
that still holds what it read, so a container user code mutates mid-walk is
never read after it is freed (what is returned is then the container as user
code left it). The walk keeps its own stack rather than recursing, so a thread
that can parse a document can revive it (a 1023-deep registered document on a
256 KiB thread stack is test-pinned).

## File & folder I/O

```python
strata.load(path, *, return_type="dict", iterator=False, skip_errors=False,
            parse_types=False)                            # str | Path; file or dir
strata.dump(obj, path, *, split_by=None) -> None          # str | Path
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
strata.query(data: dict | list, expression: str | CompiledPath, *, iterator=False,
             parse_types: bool = False) -> list
strata.search(path: str | Path, expression: str | CompiledPath, *, iterator=False,
              parse_types=False) -> list
strata.compile(expression: str) -> CompiledPath
```

`query` evaluates directly on Python objects (dict/list/tuple roots only,
else `TypeError`). `search` operates on a file or a directory. A file must end
`.json`/`.ndjson`/`.jsonl` (else `TypeError`); `.json` uses streaming SAX
search (only matches materialized) for plain paths — Filter/Slice paths fall
back to a full parse of the document, and NDJSON search materializes each
line. Invalid expressions raise `ValueError("Invalid JSONPath expression")`.
With `parse_types` set, `search` always takes the full-parse path and `query`
recognizes `str` matches (see `parse_types`).

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
  `null` silently. One placement caveat (docs/decisions.md, 2026-09-11 and
  2026-09-12): a repeated **dict** reached as a list element, whose shape the
  thread's serializer cache already holds, is emitted once more before the
  placeholder, so the `null` lands one container late; the warning, the
  `"error"` policy and the bound on recursion are unaffected. It applies to a
  dict of at most 24 exact-`str` keys at a cached depth whose keys table is
  **combined** — unicode- or general-kind — which since 2026-09-12 includes
  the records `strata.loads` itself parses up to that width (those above five
  keys are the ones that changed: they are general-kind and took the general
  writer before; narrower ones were already unicode-kind and already landed
  late). A *split* table (an instance `__dict__`) and a record wider than 24
  keys still take the general writer, and there the `null` lands in the
  container that holds the repeat.

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

Serializing (`dumps`, `dump`, `dumps_with_default`; native types per
`docs/architecture/native_types.md`):

| Condition                                                         | Exception                                                                                                                                     |
| ----------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------- |
| unsupported type                                                  | `TypeError("Object of type %s is not JSON serializable")` (in `dumps_with_default`, the call to `default` instead)                            |
| numpy scalar or array outside kinds `b i u f`                     | the same `TypeError`, with numpy's type name (a `longdouble` included: its `item()` returns itself on every platform, arm64's 64-bit one too) |
| a `datetime`, `date` or `time` subclass                           | the unsupported-type `TypeError` (in `dumps_with_default`, the call to `default` instead)                                                     |
| Enum chain longer than the depth limit                            | `ValueError("Maximum serialization depth exceeded")`                                                                                          |
| Enum member met again below its own value                         | `cycle_policy`: `null` + `RuntimeWarning`, `ValueError("Circular reference detected")`, or `null`                                             |
| `Decimal` subclass whose `str()` is not a JSON number             | `ValueError("str() of a Decimal returned text that is not a JSON number")`                                                                    |
| unset dataclass field                                             | the `AttributeError` from `getattr`, unchanged                                                                                                |
| set mutated while written                                         | the `RuntimeError` from its iterator, unchanged                                                                                               |
| a tzinfo's `utcoffset()` returns neither `None` nor a `timedelta` | `TypeError("tzinfo.utcoffset() must return None or timedelta, not '%s'")`, as `isoformat()` raises                                            |
| a tzinfo's offset is not strictly inside ±24 hours                | `ValueError("offset must be a timedelta strictly between -timedelta(hours=24) and timedelta(hours=24), not %R.")`, as `isoformat()` raises    |
| a `UUID` whose `int` is not an `int` / not in \[0, 2¹²⁸)          | `TypeError("UUID.int must be an int, not %s")` / `ValueError("UUID.int is out of range (need a 128-bit value)")`                              |

Parsing with `parse_types` (`loads`, `load`, `search`, `query`; per
`docs/architecture/native_types.md`, "Parse contract"):

| Condition                                                        | Exception                                                                            |
| ---------------------------------------------------------------- | ------------------------------------------------------------------------------------ |
| `parse_types` not a `bool` or `dict` (`loads`, `load`, `search`) | `TypeError("parse_types must be a bool or a dict, not %s")`                          |
| registry key not `str`                                           | `TypeError("parse_types keys must be str, not %s")`                                  |
| registry value not an `Enum` subclass or dataclass type          | `TypeError("parse_types values must be Enum subclasses or dataclass types, not %R")` |
| `parse_types` with `return_type="cursor"` (`loads`, `load`)      | `ValueError("parse_types needs return_type='dict'")`                                 |
| `query(..., parse_types=<not a bool>)`                           | `TypeError("query() parse_types must be a bool, not %s")` — `not dict` for a dict    |

## C++ public surface (for binding work)

`parse_json(string_view) -> Result<JsonValue>` · `parse_sax(string_view, JsonSaxHandler&, bool validate_utf8=true)`
· `parse_sax_inline<Handler>(...)` (devirtualized) · `JsonDocument::from_string` /
`root()` · `JsonCursor` (status-code API + throwing API) · `serialize_json(const JsonValue&)`
· `NdjsonStream` (borrows its buffer; `next()` signals EOF via `Status::KeyNotFound`)
· `compile_jsonpath` / `eval_jsonpath`. Full details: `docs/architecture/SKILL.md`.
