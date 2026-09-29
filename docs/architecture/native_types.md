# Decision record: native types — serializer default-on, opt-in `parse_types`

Status: **refused in this shape by its own kill criterion** (2026-09-29): paired
draws 2 and 3 (runs 36520746091, 36529483744) resolved macos-x86_64 small
`dumps nested` past +2% on a clean-control leg twice (docs/decisions.md,
2026-09-29). Fallback (b), handed over below, was **not built**: the user
directed a third shape the same day — natives behind a `native=` flag,
`_strata` byte-identical to main — recorded in the last section, "Flag shape
(M15b)", which supersedes default-on and (b). The type-by-type serializer,
parse and error contracts above carry over unchanged to the flag's `True` arm. Accepted for implementation 2026-09-28,
branch `exp/native-types` over main `38eaa9f`. Roadmap: M15. Scope approved by the user on 2026-09-28:
(A) native emitters, on by default, in `dumps`, `dump` and `dumps_with_default`
for `datetime`/`date`/`time`, `uuid.UUID`, `enum.Enum`, dataclasses,
`decimal.Decimal`, `set`/`frozenset` and numpy scalars and arrays; (B) an opt-in
`parse_types` keyword on `loads`, `load`, `search` and `query` that recognizes
RFC 3339 date/time strings and canonical UUIDs, with best-effort `Enum` and
dataclass revival through a caller-provided registry.

This **reopens option B** of [`dumps_default_hook.md`](dumps_default_hook.md)
deliberately. That record deferred native emitters on three grounds: coverage,
which the `default` hook now answers (M12b); semantics, which this record pins
type by type; and evidence, which the admission microbenchmarks below supply.
It also departs from [`dumps_with_default.md`](dumps_with_default.md) on one
point, knowingly: that record required the `dumps` path to stay byte-identical
to main's. A native type that is **on by default** cannot leave it identical,
because the one branch every unsupported object reaches has to dispatch instead
of raise. The design therefore confines the change to that branch, adds no walker
state, and prices what is left against the repository's regression gate
(>2% on a touched row = do not merge), with the M12 evidence as the expected
effect size.

Area: `src/strata/bindings/` (serializer tail, native type table, revival
walk, entry-point keywords), `include/strata/util/` + `src/strata/util/` (pure
text formatting and recognition, no `Python.h`), the facade (keyword
pass-through only), `scripts/` (the PGO training scope), tests on both layers.

## What M12 and M12b established that this record must respect

- **A tail test alone moves a row.** With the profile held equal and the output
  stage pinned on both arms, a null test on `Serializer::write`'s unsupported
  tail lost linux-x86_64 `dumps flat` +0.99–1.67% (run 36254514783; ledger M12).
  The same class of change is unavoidable here. It is below the 2% gate, and it
  is what this record's A/B is sized to read.
- **Walker state growth moves the output stage.** Two words in `Serializer`
  grew `dumps_to_python`'s frame by 16 B and moved `StagedOutput::stage_`
  (`python_dumps_output.h:271`) out of its 64-byte alignment class. This record
  adds **no member** to `Serializer` in `_strata`.
- **PGO believes the profile over `cold`.** When training exercised a path
  marked cold, block placement laid it hot and Windows lost `dumps mixed`
  +4.3% (E26-P23, run 34665612473; fixed by the paired payload, run
  34670240916). The native paths must therefore carry **zero training counts**.
- **Every test addition moves the gate-inclusive profile** (E26-P7b, E26-P8).
  The new tests must stay out of the training run for the same reason.
- **A lazily-resolved static or a GC-tracked allocation inside the walk is a
  user-code step** (`python_dumps.cpp:61-71`, the FIX1-REVIEW segfault). Every
  native conversion that can run Python or allocate a tracked object is one,
  and takes `latch()` first.

## Decision

### Serializer: one tail call from `write()` into a cold member

`Serializer::write`'s unsupported sink (`python_dumps.cpp:252-258`) becomes, in
**both** images, `return write_native(object);`. `write_native` is a
`STRATA_COLD_FN` member (noinline + cold) of `Serializer`, so it can use
`out_`, `Frame`, `latch()` and `write()` without new state. Its last arm is the
old sink: in `_strata` it raises the unchanged
`TypeError("Object of type %s is not JSON serializable")`; in the hook image it
calls `write_unsupported` (the `default` callable). The `#if` that chose between
those two moves from `write()` into `write_native`, so `write()` itself has no
preprocessor branch left and no load, test or state it did not have.

Everything type-specific lives outside the writers' translation unit:

- `include/strata/util/temporal.hpp` + `src/strata/util/temporal.cpp` (core,
  pure C++, listed in `core_sources.txt`): format a date, a time, a UTC offset,
  a UUID from two 64-bit halves; recognize RFC 3339 date/time/date-time and
  canonical UUID text into fields; validate the JSON number grammar.
- `src/strata/bindings/python_native_types.{h,cpp}` (both images): the lazily
  resolved type table, the dataclass field-name cache (at most 1024 types, a
  full cache cleared; an entry stands while the type's `__dataclass_fields__`
  is an exact dict holding the keys and field objects it held when read, by
  identity and in order), and the conversions from
  a Python object to fields or text; `python_numpy_twins.{h,cpp}` (both
  images) holds the numpy twins' runtime proof. Resolution reads `sys.modules` only — it
  never imports a module — so `import strata` imports none of `datetime`,
  `uuid`, `decimal`, `enum`, `dataclasses` or `numpy`, and a type whose module
  is not imported cannot be present in the document anyway.
- `write_native` and its per-kind members stay in `python_dumps.cpp`, inside
  `Serializer`: they are writers (they open frames and recurse through
  `write()`), and the writers stay in one TU per image (convention, Layout).

Options considered for where the dispatch lives:

| Option                                                                                                     | Verdict                                                                                                                                                                                                                                                                          |
| ---------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **S1. Tail call from `write()` into a cold member (chosen)**                                               | no state, no load or test added to `write()`; the tail becomes a two-argument call where it was a three-argument `PyErr_Format`. Not byte-identical; priced by the A/B.                                                                                                          |
| S2. `_strata` raises as today; the entry retries the whole document in the hook image with natives enabled | `_strata`'s serializer stays byte-identical, but every user-code step of the failed first walk (cycle warnings, an `int` subclass's `__str__`) runs twice, a `TypeError` from user code triggers a retry, and every native-bearing document pays a wasted partial walk. Refused. |
| S3. A flag or pointer tested on the tail (M12's shape)                                                     | measured and refused (ledger M12): the test plus state is what lost `dumps flat`.                                                                                                                                                                                                |

**Amendment (2026-09-28, after static check 1 fired on S1).** S1 as built
(`2f494a9`) failed check 1: the tail call reshaped `write()`'s shared return
epilogue and its register allocation, so exact-type success paths moved by one
or two instructions or a taken branch (arm64 281 → 259 instructions: `float`
−1, `None`/`True` +1 and a taken branch; x86-64 186 → 181: finite `float` +1
taken `jmp`, `str` −1, `None`/`True` −2). Three tail variants kept that
reshaping (`not_tail_called` on the callee: 266/186; the same as a static
member: identical; a type predicate ahead of main's literal tail: 293/197 with
register changes in the subclass blocks). One shape restores check 1 on both
ISAs: **S3 without state** — main's literal `PyErr_Format` tail stays the
fall-through, behind `if (native::g_runtime_ready) return write_native(object);`,
a process-global flag the module init sets (arm64 293 instructions, x86-64 194;
with branch targets masked every one of main's instructions appears in order,
and the additions are the flag test, +5/+3, and the tail-call block, +7/+6;
`build/evidence/M15/codegen/variants/`). **This is the chosen shape.** It is
M12's class of change without M12's state growth — a load and branch on a
path no canonical row executes, plus the layout shift it causes — and M12
priced that class at up to +1.7% on one row (linux-x86_64 `dumps flat`),
inside this record's 2% gate. S1's per-type path changes are the worse kind:
they execute on every value `write()` dispatches.

### Parse side: a post-pass over the built tree

`parse_types` never reaches the parser or the builder. The document is parsed
exactly as today — same entry, same `PythonObjectBuilder` instantiation, same
code — and, when `parse_types` is set, a separate cold walk
(`src/strata/bindings/python_parse_types.{h,cpp}`, `_strata` only) revives the
freshly built tree in place. The default builder's codegen is untouched **by
construction**: no source it compiles changes.

| Option                                                                        | Verdict                                                                                                                                                                                                                                                           |
| ----------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **P2. Post-pass revival of the built tree (chosen)**                          | builder byte-identical by construction; one extra O(n) walk, paid only when `parse_types` is set.                                                                                                                                                                 |
| P1. A second `PythonObjectBuilder` instantiation that recognizes during build | faster for the opt-in, but it doubles the builder's text and cannot be *proven* not to move the default instantiation (instantiation order, symbol names, per-caller inlining — the same argument `dumps_with_default.md` made against a template twin). Refused. |
| P3. An `object_hook`-style Python callback per container                      | CPU work in Python and a call per container (convention rule 1). Refused.                                                                                                                                                                                         |

The entry points are the only `_strata` parse-side code that changes: each
gains the keyword, and each default path calls the function it calls today
(`finish_loads` for `loads`, `python_module.cpp:168`) unchanged; a set
`parse_types` takes a separate cold function.

### Registry shape: a key-name mapping

`parse_types` is `False` (default), `True`, or a `dict` mapping JSON object
member names to `Enum` subclasses or dataclass types.

| Option                                                         | Verdict                                                                                                                                              |
| -------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- |
| **R1. `{member_name: type}`, applied at every depth (chosen)** | no new public name; a JSON document names its fields, so the member name is the one context every value has; nesting falls out of a post-order walk. |
| R2. Match a dataclass by a dict's key set                      | ambiguous when two types share fields, and costs a key-set hash per dict. Refused.                                                                   |
| R3. JSONPath expression → type                                 | expressive, but compiling and matching paths during revival is a second evaluator for one feature. Refused; can be layered on R1 later.              |

## Serializer contract

Checked in this order, after every existing branch of `write()` has failed —
so an `int`, `str`, `float`, `dict`, `list` or `tuple` **subclass** is written
exactly as today, before any native check. **Amended 2026-09-28 (review
P1):** rows 1–3 take the **exact** types only, as orjson does; a `datetime`,
`date` or `time` subclass is unsupported (the `TypeError`, or `default` in
`dumps_with_default`), because a subclass can carry state its C fields do not
— pandas' `NaT` formatted from its fields as `"0001-01-01T00:00:00"`, and a
`Timestamp` lost its nanoseconds, where both had raised before:

| #   | Type (instances, subclasses included)                                        | Output                                                                                                                                                                                                                                                                                                                                                                      | orjson 3.12.0                                                                                                                                                          |
| --- | ---------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 1   | `datetime.datetime`                                                          | `"YYYY-MM-DDTHH:MM:SS"`, then `.ffffff` (six digits) when the microsecond is nonzero, then the offset when aware: `+HH:MM`/`-HH:MM`, UTC as `+00:00`. The offset is `days*86400 + seconds` of the normalized `utcoffset()` (its microseconds dropped), rounded to the minute half-up in magnitude. Years zero-padded to four digits; `fold` honoured through `utcoffset()`. | identical, except: a tzinfo whose `utcoffset()` is `None` — orjson writes `+00:00`, strata writes no offset (naive, as `isoformat()` does); a subclass — orjson raises |
| 2   | `datetime.date`                                                              | `"YYYY-MM-DD"`                                                                                                                                                                                                                                                                                                                                                              | identical                                                                                                                                                              |
| 3   | `datetime.time`                                                              | `"HH:MM:SS"` + `.ffffff` when nonzero; when aware (tzinfo set and `utcoffset()` not `None`) the offset as in row 1                                                                                                                                                                                                                                                          | naive identical; orjson raises on any tzinfo                                                                                                                           |
| 4   | `uuid.UUID`                                                                  | `"xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"`, lowercase, from `.int`                                                                                                                                                                                                                                                                                                            | identical; orjson raises on a subclass                                                                                                                                 |
| 5   | `decimal.Decimal`                                                            | finite: the text of `str(d)` as a raw JSON number (`1.50`, `1E+2`, `-0`); `NaN`, `sNaN`, `±Infinity` → `null`. A subclass whose `str()` is neither → `ValueError`                                                                                                                                                                                                           | raises (msgspec: string `"1.50"`)                                                                                                                                      |
| 6   | `enum.Enum` (not caught by a subclass branch above)                          | `member.value`, read with `getattr`, written in the member's place; a value that is itself an `Enum` is followed, up to the depth limit                                                                                                                                                                                                                                     | identical (orjson also lets `int`/`str` subclass flags win: `IntEnum` → `5`, `StrEnum` → `"x"`)                                                                        |
| 7   | dataclass instance (`type(obj)` has `__dataclass_fields__`; not a class)     | a JSON object of exactly the fields `dataclasses.fields(obj)` lists, in that order, each read with `getattr` — `dataclasses.asdict` without the copy; an unset `init=False` field raises the `AttributeError` `getattr` raises                                                                                                                                              | differs: orjson emits the instance `__dict__` (extra attributes included) and skips names starting with `_`                                                            |
| 8   | `set`, `frozenset`                                                           | a JSON array in iteration order                                                                                                                                                                                                                                                                                                                                             | raises (msgspec: array, iteration order)                                                                                                                               |
| 9   | numpy `bool_`/`integer`/`floating` scalar; `ndarray` of dtype kind `b i u f` | `obj.item()` / `obj.tolist()`, written in the object's place (any shape and strides; 0-d included); `float32`/`float16` therefore widen exactly to `float` (`float32(0.1)` → `0.10000000149011612`); every other numpy scalar or dtype → `TypeError`                                                                                                                        | differs: orjson needs `OPT_SERIALIZE_NUMPY`, writes float32-shortest (`0.1`), rejects non-contiguous and 0-d arrays                                                    |

Unchanged: dict keys must be `str` (a native key is the existing
`TypeError("keys must be str, not %s")`); `dump`'s split values stay
`str`/`int`/`bool`; `datetime.timedelta`, `complex`, `bytes` and every other
type still raise `TypeError("Object of type %s is not JSON serializable")`.

**The oracle.** Every row has a reference spelling in Python —
`dt.isoformat()` with the offset rule above, `str(u)`, `d.value`,
`{f.name: getattr(o, f.name) for f in fields(o)}`, `list(s)`, `a.tolist()` —
and the tests pin `strata.dumps(x) == strata.dumps(ref(x))` (byte identity with
the supported-type path) and `json.loads(strata.dumps(x)) == json.loads(json.dumps(x, default=ref))`
over generated corpora. The orjson differential for rows 1–4 and 6 lives in
`tests/integrations/` (orjson is not a gate dependency).

### Re-entrancy: which conversions are user-code steps

A conversion is **pure** when it provably calls nothing the user wrote and
allocates nothing the collector tracks. Pure conversions skip `latch()`:

- an exact `datetime`/`date`/`time` whose `tzinfo` is `None` or exactly
  `datetime.timezone` (field macros; `timezone.utcoffset` is C and returns its
  stored `timedelta`);
- an exact `uuid.UUID` (its `int` slot; the 128-bit split uses
  `PyLong_AsUnsignedLongLongMask` and one untracked `int` shift).

Every other conversion — any subclass, any other `tzinfo`, `Decimal`'s
`str()` (the decimal context can be created lazily), `Enum.value`, dataclass
field reads, set iteration (the iterator is tracked), numpy's `item`/`tolist`
and dtype reads, and **type-table resolution itself** (a `sys.modules` probe
and attribute reads on a module the user may have replaced) — calls `latch()`
first, which bumps `user_steps_` so every enclosing list loop re-derives its
bounds, and holds a strong reference to the object being converted (the latch
covers containers and rows, not the entry being written — `write_int`'s rule).
The header of `python_dumps.cpp` (lines 18–71) is amended to enumerate these as
the native steps; the three ownership facts and the freshness rule stay as
written.

For a document that holds no native object nothing changes: the tail is never
reached. For one that holds only pure natives, no user code runs.

### Frames, cycles and depth

- **Dataclass and set/frozenset** open a `Frame` on the **original object**
  before their first field or element: the linear cycle probe then sees a
  dataclass that contains itself (or a set reached back through a frozen
  dataclass) and applies `cycle_policy` — `null` + `RuntimeWarning`,
  `ValueError`, or silent `null` — exactly as for a dict, which is also what
  stdlib `json` does with `default` (its markers key the object *before*
  `default`). Each takes one level of the depth limit, like a container whose
  frame is not elided. They are written directly (`{`/`[`, keys through the
  same key writer as a dict, values through `write()`), never through a
  temporary dict or list, so they allocate nothing but the set iterator.
  **Amended 2026-09-28 (admission, set10 1.40× the hook):** an exact `set` or
  `frozenset` is walked on its own table, as `setiter_iternext` walks it —
  the size check before each element, then the next slot that is neither
  empty nor the deleted-slot dummy, the table re-read at every step — and
  allocates nothing; a subclass keeps its iterator (its `__iter__` may be its
  own). The walk is proven against the iterator at module init (a one-key set
  discarded yields the dummy; a grown table with deleted slots and a
  frozenset of it must list what the iterator lists) and is off on a
  free-threaded build. A plain-scalar element is written borrowed; any other
  is latched and held first. While only plain elements have been written the
  set cannot have changed, so the walk stops after `used` elements instead of
  scanning to the mask.
- **Enum** chains are followed in a loop inside `write_native`, not by
  recursion: after `limit` hops (the serializer's depth limit) it raises
  `ValueError("Maximum serialization depth exceeded")`. The final value goes
  through `write()` once. **Amended 2026-09-28 (review P0):** the member also
  opens a `Frame` before its value is written. Without it, in the hook image a
  `default` returning `E.A` whose value is the unsupported object it was called
  on recursed without bound (`default` → `E.A` → value → `default` …; the chain
  bound keys on the return, the member, not on its value) and overflowed the C
  stack on CPython 3.13. With the frame the member met again is a cycle under
  `cycle_policy`, and each hop takes a level of the depth limit.
- **numpy** writes what `item()`/`tolist()` return, which contains only
  supported scalars and lists, so no native object recurses back into the tail.
  **Amended 2026-09-28 (admission, `np.int64` 1.27×, `np.float32` 1.26× the
  hook — `item()` builds a 0-d array first):** a scalar whose `dtype.type` is
  its own type and whose `dtype.num` is numpy's `bool_` (0), a sized integer
  (1–10), `float32` (11) or `float16` (23) is read through truth,
  `__index__` or `__float__`, which return the `bool`, `int` or `float` its
  `item()` returns; a subclass, `longdouble` and user dtypes keep `item()`.
  **Amended 2026-09-29 (portable by construction):** those numbers are proven
  once, when numpy resolves in each image — for each of `? b B h H i I l L q Q f e`,
  the scalar type `numpy.dtype(code)` names, built from a probe (`True`, the
  integer type's extreme, `0.1`), has exactly that type, and its own dtype has
  the expected number, kind and C item size; its twin equals its `item()` in
  type and value. If any row fails, every numpy scalar keeps `item()` (identical
  output).
- A dataclass's fields are read one at a time as they are written (followed
  live, like a wide dict); a set resized while it is being written raises the
  `RuntimeError` its iterator raises. The existing rules for lists and dicts
  are unchanged.

### `dumps_with_default`

Natives are part of what `dumps` supports, so the hook image serves them before
the callable: `default` is **never called for a native object**, and an object
`default` returns that is native is written natively. This is a behaviour
change for callers whose `default` formatted these types (for example a
`datetime` as an epoch number): orjson has the same precedence. The chain bound
is unchanged — it applies to an unsupported return; a native return is
supported. A value reached *inside* a native (an Enum's `value`, a field, an
element) is an ordinary position and gets its own `default` call when it is
unsupported.

**Opt-out: `native=False` (built with the M13 merge, 2026-09-29).** The
framework adapters hand `dumps_with_default` a framework's own `default` so
that it formats `datetime`, `Decimal`, `UUID` and dataclasses (Flask as an HTTP
date, a string and `asdict()`; Django as ECMA-262 and a string; structlog as
`repr`; pydantic as `to_jsonable_python` does). Native precedence would bypass
it, so `dumps_with_default` takes `native=True` (keyword-only, a `bool` by
identity): under `native=False` the supported set is `dumps(obj, native=False)`'s, every native family goes to `default`, a native `default`
returns is the chain bound's `TypeError`, and the output is main `38eaa9f`'s
hook byte for byte. The state is hook-only — a `contextvars` variable (greenlet
and gevent keep one context per greenlet, so a walk suspended in `default`
cannot resume under another walk's mode, which a thread-local would allow) and
a process-wide count of opt-out walks in progress, which gates the read in
`classify` and `format_pure_leaf` to one relaxed load per native object while
no opt-out is live. Each hook serializer entry sets its own mode when its
context holds the other and resets it on exit. `python_dumps.cpp` does not
change, so `_strata` stays main's by construction. Decisions: docs/decisions.md,
2026-09-29 (the `native` keyword, its state, the adapter reconciliation).

## Parse contract (`parse_types`)

`parse_types=False` (default) is today's behaviour, bit for bit. With
`parse_types=True`, every JSON **string value** (never a key) that matches one
of these exactly is replaced; anything else — including a string that matches
the grammar but names an impossible value (`2024-02-30`, a leap second
`23:59:60`, year `0000`) — stays a `str`:

| Kind      | Grammar (RFC 3339 §5.6, strict)                                            | Becomes                                                                                                     |
| --------- | -------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- |
| date-time | `YYYY-MM-DD` `T`\|`t` `HH:MM:SS` \[`.` 1–6 digits\] \[`Z`\|`z`\|`±HH:MM`\] | `datetime`; naive without an offset; `Z`, `+00:00` and `-00:00` → `timezone.utc`; others a fixed `timezone` |
| date      | `YYYY-MM-DD`                                                               | `date`                                                                                                      |
| time      | `HH:MM:SS` \[`.` 1–6 digits\] \[`Z`\|`z`\|`±HH:MM`\]                       | `time`, aware with a fixed `timezone` when an offset is present                                             |
| UUID      | 8-4-4-4-12 hexadecimal digits, hyphenated, either case                     | `uuid.UUID`                                                                                                 |

A fraction of 7 or more digits stays a `str` (no silent truncation); so does a
space separator, a `+HH:MM:SS` offset, or `HH:MM` without seconds. Every string
strata's serializer writes for rows 1–4 is recognized, so
`loads(dumps(x), parse_types=True) == x` holds for naive values, for
`timezone`-aware values with whole-minute offsets, and for UUIDs; a `ZoneInfo`
comes back as the fixed offset it had.

**Registry** (`parse_types={"name": T, ...}`, implies `True`): keys must be
`str`, values `Enum` subclasses or dataclass types, checked before parsing. The
walk is post-order, so a container's contents are revived before the container
itself; it keeps an explicit stack of open containers rather than recursing, and
holds each registry entry it applies across the user code that applies it
(review P2 and P1, 2026-09-28). For a JSON object member whose name is registered:

- an `Enum` type `E`: the value is replaced by `E(value)`; a `ValueError` (no
  member has that value) leaves it as parsed; any other exception propagates;
- a dataclass type `D`: a dict value whose keys are all init fields of `D`
  and include every init field without a default is replaced by `D(**value)`;
  any other value is left as parsed; exceptions from `D`'s `__init__` or
  `__post_init__` propagate;
- a list value has each element revived by the same rule (one level);
- a value under a registered name is **never** type-recognized as above: it is
  revived by the registry or left exactly as parsed.

Per entry point:

- `loads`, `load` (file, NDJSON line by line, folder record by record, eager
  and `iterator=True`): the tree is revived before it is returned or iterated;
  lazy iterators revive each item as they yield it. `return_type="cursor"` with
  `parse_types` set → `ValueError("parse_types needs return_type='dict'")`.
  `skip_errors` still covers invalid JSON only; an exception from a registered
  type propagates.
- `search` (file and folder): `search(f, e, parse_types=p) == query(load(f, parse_types=p), e)`
  for every supported expression — with `parse_types` set a `.json` file takes
  the full-parse path that Filter/Slice expressions already take (parse,
  revive, evaluate), and each NDJSON line is revived before it is evaluated.
  Streaming is for the default only.
- `query`: `parse_types` is a `bool` only. `True` replaces each match that is a
  `str` by the rule above; container and other matches are returned as they
  are — the caller's own objects, never copied or mutated. A `dict` →
  `TypeError("query() parse_types must be a bool, not dict")`.

The first call with `parse_types` set imports `datetime` and `uuid` if they are
not already imported (the only imports strata ever makes on its own behalf);
`import strata` still imports neither.

## Error contract (each test-pinned)

| Condition                                               | Exception                                                                            |
| ------------------------------------------------------- | ------------------------------------------------------------------------------------ |
| unsupported type (unchanged)                            | `TypeError("Object of type %s is not JSON serializable")`                            |
| numpy scalar or array outside kinds `b i u f`           | the same `TypeError`, with numpy's type name                                         |
| Enum chain longer than the depth limit                  | `ValueError("Maximum serialization depth exceeded")`                                 |
| `Decimal` subclass whose `str()` is not a JSON number   | `ValueError("str() of a Decimal returned text that is not a JSON number")`           |
| unset dataclass field                                   | the `AttributeError` from `getattr`, unchanged                                       |
| set mutated while written                               | the `RuntimeError` from its iterator, unchanged                                      |
| `parse_types` not a `bool` or `dict`                    | `TypeError("parse_types must be a bool or a dict, not %s")`                          |
| registry key not `str`                                  | `TypeError("parse_types keys must be str, not %s")`                                  |
| registry value not an `Enum` subclass or dataclass type | `TypeError("parse_types values must be Enum subclasses or dataclass types, not %R")` |
| `parse_types` with `return_type="cursor"`               | `ValueError("parse_types needs return_type='dict'")`                                 |
| `query(..., parse_types=<dict>)`                        | `TypeError("query() parse_types must be a bool, not dict")`                          |

## Hot-path protection (each item is an acceptance check)

1. **`write()`** differs from main's only in its tail block, on arm64 and
   x86-64 plain `-O3` builds (the M12 method: `-fomit-frame-pointer -march=x86-64-v3` cross build, instruction diff of the symbol).
2. **No state growth**: `sizeof(Serializer)` in `_strata` equals main's, and
   `dumps_to_python`'s frame size and `stage_`'s frame offset equal main's, so
   the stage keeps its 64-byte alignment class. (The hook image carries the
   same members it carries today.)
3. **Builder untouched**: every symbol of the parser and `PythonObjectBuilder`
   instantiations is byte-identical to main's in plain builds, and
   `finish_loads`, `loads_to_python`, the NDJSON, file and folder readers are
   unchanged in source.
4. **Training scope**: the new tests live under `tests/unit/native_types/` and
   `tests/py/native_types/`; `scripts/py_tests.py --training` ignores both, and
   the three PGO scripts (`pgo_build.sh:56`, `pgo_build_msvc.py:107`,
   `pgo_build_clang_cl.py:74`) pass it on their **instrumented** pass only —
   the optimized pass and every gate run everything. `scripts/pgo_training.py`
   makes no native call. Verified by `llvm-profdata show` reading zero counts
   on the revival walk, the new keyword arms and every per-kind writer.
   **Corrected 2026-09-28 (review P1):** `write_native` itself is not at zero:
   the trained suites' unsupported-type tests (421 calls per pass, 842 in the
   instrumented phase) reach it, and through it `classify`, `format_pure_leaf`
   and the type resolution, before the `TypeError` — the same tests trained
   main's `PyErr_Format` tail with the same counts. Every per-kind writer,
   every conversion and all of the parse side read zero
   (`build/evidence/M15/check4_*`).
5. **Keywords**: `parse_types` joins `strata_loads`'s keyword loop as a third
   compare (the facade already passes two keywords per call);
   `load`/`search`/`query` add one format unit. `dumps`'s signature does not
   change. **Amended 2026-09-28:** measured, the third text compare cost
   +23 ns per `loads` call; the loop now recognizes each keyword by the
   identity of its interned name first (a literal keyword arrives interned)
   and by text only for any other spelling.
6. **Import**: `import strata` imports none of `numpy`, `datetime`, `uuid`,
   `decimal`, `dataclasses` (a fresh-interpreter test), and its import time is
   unchanged (paired, fresh interpreters).
7. **Footprint**: `_strata`'s `size -m` **Section `__text`** growth is reported
   per ISA, and attributed per symbol (`benchmarks/symbol_sizes.py`); every
   added function except `write_native`'s entry has zero training counts.
   Estimate: 8–16 KB — for the serializer alone. Measured: plain builds
   +12 388 B arm64 / +12 272 B x86-64 with the serializer (at `10521a9`'s tail),
   +22 880 B arm64 with the parse side added; the PGO+LTO image +18 308 B
   (222 564 → 240 872). The estimate did not count the parse side.

## Measurement

- **Local A/B (M1, in-process, pinned profile).** Arms: A = main `38eaa9f`
  built `PGO_MODE=use` against main's own profile; B = this branch built
  against **the same profile** (the new tests are outside training, so B's own
  training would be A's up to the edited existing tests — the pin removes even
  that). A-B-B-A rounds over the 27 canonical small-tier rows and the medium
  `dumps` rows (`make probe-ab-rows`), an A/A floor from A against itself in the
  same session (`make probe-ab-floor`), read with `make probe-ab-analyze`.
  Quiet machine per `docs/context/benchmarks.md` (load checked before each run).
- **Admission microbenchmarks** (≥ 30 repeats, same host): per type, the
  per-object cost of `dumps([x]*n)` against `dumps_with_default([x]*n, ref)`
  with the Python reference conversion, and against orjson where it supports
  the type. A type whose native path is not faster than the hook does not ship
  native.
- **Five-leg A/B** (`.github/workflows/ab_x86.yml`): a dispatch plan only, not
  run from this branch — see the ledger entry.

## Falsifiable estimate and kill criterion

Estimate: static checks 1–3 hold on both ISAs; the pinned local A/B reads no
canonical row past its A/A floor by more than the M12 band (≤ +1.7%), and none
past +2%; on the five-leg A/B, linux-x86_64 `dumps flat` may resolve up to
+1.7% (M12's tail-test class), and nothing resolves past +2%.

Kill criterion: if static check 1 or 2 fails, the shape is wrong and nothing is
timed until it holds. If a canonical row resolves past **+2%** (the regression
gate) on the pinned A/B or on a five-leg draw repeated once, and the held-profile
arm attributes it to the code, default-on native serialization does not merge in
this shape; the fallback to put to the user is native types in
`dumps_with_default` only (hook image, `_strata` byte-identical — M12b's
guarantee restored) with the parse side unaffected.

## Migration and rollback

Behaviour changes, each in `docs/context/api.md` and `docs/decisions.md`:
`dumps`/`dump` now serialize nine type families that raised `TypeError`
(callers relying on the error for validation lose it); `dumps_with_default` no
longer calls `default` for them; `loads`, `load`, `search` and `query` gain one
keyword with an unchanged default. Nothing is persisted in a new format, so a
rollback is a revert of the merge; data written with native types is ordinary
JSON that every version reads.

## Ambiguities resolved (logged in `docs/decisions.md`, 2026-09-28)

`int`/`str`/`float`-mixin enums are written by the subclass branch (orjson's
rule, and `write()`'s head unchanged); aware/naive follows Python's definition,
not orjson's `+00:00` for a `None` offset; aware `time` is written with its
offset where orjson raises; offsets round to the minute half-up as orjson does;
dataclasses follow `dataclasses.fields()`, not orjson's `__dict__`; `Decimal`
text is `str()`, non-finite → `null`; numpy follows `item()`/`tolist()` and
widens float32; the precedence order of the table above; registry shape R1 and
its best-effort rules; recognition grammar strictness (1–6 fraction digits,
`T`/`t`, `Z`/`z`, `-00:00` as UTC, UUIDs either case); `query` takes a `bool`
only; `search` with `parse_types` leaves the streaming path.

## Fallback (b) handover (2026-09-29; design only, not built)

Why a successor, and what the measurements say it must do. The shape above
fired its kill criterion on macos-x86_64 small `dumps nested` (bytes +3.98% and
+3.74% on draws 2 and 3, whose A2 on that leg was byte-identical to A, so it
controlled launch noise, not build variation); the Neoverse-N2 resolved a carpet
of sub-2% `dumps` losses on draws 2 and 3 (+0.37 to +1.38%; draw 1, on the
pre-boundary source, resolved one); linux-x86_64's draw-1
`dumps flat` loss was an inliner flip that the boundary (74d78ca) removed. The
static diff of the timed arms (docs/benchmarks/evidence/M15/ci-36520746091/)
leaves two candidate costs on the fired leg: `write()`'s V4 tail (+48 B: a flag
load, a test, a tail-call block) and the layout shift of about 20 KB of added
`_strata` text (native writers, `python_native_types`, `python_numpy_twins`,
`python_parse_types*`, `temporal`) — every other hot writer there is main's
size. Fallback (b) removes the second and keeps the first; (a) — natives in
`dumps_with_default` only — removes both but gives up default-on, which the
user approved and the lead's ruling keeps.

### Shape

**`_strata` keeps only the tail and a cold sink; everything native lives in a
second image.** The second image is `strata._dumps_hook`, which already
compiles `python_dumps.cpp` with every native emitter and the full serializer
(M12b; `setup.py` `HOOK_BINDING_SOURCES`), is imported on first use, and is
built unprofiled, so it cannot perturb `_strata`'s profile. It gains a
PyCapsule (`strata._dumps_hook._native_api`) exporting two C functions; no new
public name.

Moves out of `_strata`'s link (into the hook image and the C++ tests only):
`python_native_types.cpp`, `python_numpy_twins.cpp`, `python_parse_types.cpp`,
`python_parse_types_walk.cpp`, and `src/strata/util/temporal.cpp`, which leaves
`core_sources.txt` for a second manifest (`native_sources.txt`) read by CMake
and by the hook `Extension` — `_strata` links without `-dead_strip`/`--gc-sections`,
so a core source it does not call would still move its layout. The native
writer members of `Serializer` (`write_native` and the per-kind writers,
`write_key_cold`, `NativeFrame`, `push_open_cold`) go under
`#if defined(STRATA_DUMPS_HOOK)`, as `write_unsupported` already does.

Stays in `_strata`:

- `write()`'s tail, token for token as V4 (`if (g_native_bridge) return write_via_bridge(object);`
  then main's `PyErr_Format`), and nothing else in `python_dumps.cpp`.
- One cold sink, `write_via_bridge`, in a new TU linked last
  (`python_native_bridge.cpp`, ~80 lines) reached through a `Serializer`
  friend thunk that exposes only `out_`, `latch()`, `open_`/`open_count_`,
  `depth_limit_` and `user_steps_` — no writer.
- `parse_types` routing moves to the facade: `loads`/`load`/`search`/`query`
  call the hook image's entries when `parse_types is not False` (dispatch, which
  the convention allows; one Python `is` test per call, priced by the per-call
  floor of M12's criterion 9), so `strata_loads`/`strata_load`/`strata_query`/
  `strata_search` return to main's bytes.

### The fragment interface at the unsupported tail

```c
// strata._dumps_hook._native_api (PyCapsule), version 1
typedef struct {
    PyObject* const* open;      // _strata's open-container stack (borrowed, latched)
    Py_ssize_t open_count;      // its depth
    int depth_limit;            // Py_GetRecursionLimit() at the walk's start
    int cycle_policy;           // _strata's g_cycle_policy, read at the call
} StrataNativeContext;

// Serialize one unsupported object as a JSON fragment. Returns 1 and a new bytes
// object in *fragment, 0 when the object is not native (no exception set: the
// caller raises its own TypeError), -1 with an exception set.
int (*encode_native)(PyObject* obj, const StrataNativeContext*, PyObject** fragment);
// parse_types revival lives entirely in the hook image and is called by the facade.
```

- **Order of work in `write_via_bridge`.** Resolve the capsule lazily: the
  import is user code, so `latch()` first and a strong reference on `obj`
  (`write_int`'s rule); a failed import sets `g_native_bridge = false` for the
  process and raises the `ImportError` (no silent fallback). Then call
  `encode_native`; on 1, `out_.write_spanning` the fragment's bytes and release
  it (the release is step 4 of `python_dumps.cpp`'s header; the latch already
  moved `user_steps_`).
- **Depth.** The hook image's walker starts at `open_count` with
  `depth_limit` — a nested container inside a dataclass counts the ancestors in
  `_strata` exactly as today.
- **Cycles.** The hook image's `Frame`/`NativeFrame` probe `open[0..open_count)`
  as well as its own stack, so a dataclass or set that reaches back to a
  container `_strata` has open is a cycle under `cycle_policy`, emitted in the
  fragment (`null` or `ValueError`) and warned from the hook image after its own
  latch. The one-container-late caveat (api.md, Config) is unchanged.
- **Output identity.** A fragment is the bytes the current branch writes for
  that object; the existing `tests/{unit,py}/native_types/` corpora pin it
  unchanged, plus one test that a native object inside a 1024-deep document
  reports the same depth error as today.
- **Cost.** One capsule call and one bytes allocation per native object (the
  fragment) — cold by design; the admission table (docs/benchmarks/evidence/M15/micro2/)
  is re-read and every kind must stay below the `dumps_with_default` hook.

### Byte-parity obligation (acceptance before any timing)

Every `_strata` symbol is byte-identical to main's except `write()`'s tail
block and the functions of `python_native_bridge.cpp` (plus `PyInit__strata`'s
one flag store, if the flag is not set from the bridge), on every leg, by the
M12b tooling on `exp/m12b-ab-arm` / `exp/m15-ab-arm`
(`benchmarks/image_identity.py`, `benchmarks/normalised_disassembly.py`, and
`docs/benchmarks/evidence/M15/linux-symbols/`'s replay of CI's PGO builds):
plain builds on arm64 and x86-64, and the Linux clang 18 replay of the PGO+LTO
images against the run's own profiles. `_strata`'s Section `__text` growth is
the tail plus the bridge, a few hundred bytes, not the 18–25 KB of this branch.

### Residual risk and the experiment to run first

M12 priced a tail test alone at linux-x86_64 `dumps flat` +0.99–1.67% with the
profile held equal; (b) keeps that tail. Before building (b), price the tail by
itself: one paired five-leg draw of main plus only the V4 tail and a bridge stub
that raises the unchanged `TypeError` (no second image, no natives), against
main. If macos-x86_64 small `dumps nested` or the N2 carpet reproduces there,
the cost is the tail and (b) cannot fix it — only (a), or a tail that is not in
`write()` (for example routing through the existing `PyErr_Format` call and
recovering after it, which runs no user code only if the exception is caught
before any handler sees it — unexplored). If the stub is clean, the
layout component was the cost and (b) is the design to build.

## Flag shape (M15b; 2026-09-29, user-directed)

The user's target shape replaces default-on: **`_strata` builds byte-identical
to main `38eaa9f`, serializer and parser both**, and everything native — the
serializer rows 1–9, `parse_types`, `temporal` — lives in the second image,
`strata._dumps_hook`, reached by facade dispatch on a keyword. Both measured
candidate costs of the refused shape (the V4 tail and ~20 KB of `_strata` text)
are gone by construction, so the default path's acceptance is an identity
proof, not an A/B campaign. Milestone M15b (M12 → M12b precedent).

### Surface

| Call                                                                  | `False` (default)                                                                            | `True` / set                                                                                      |
| --------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- |
| `dumps(obj, *, return_type="str", native=False)`                      | `_strata.dumps` — main's call, main's `TypeError`                                            | `_dumps_hook.dumps_native`: this record's serializer contract                                     |
| `dump(obj, path, *, split_by=None, native=False)`                     | `_strata.dump`                                                                               | `_dumps_hook.dump_native`: file and folder mode, main's dispatch and errors                       |
| `loads`/`load`/`search`/`query`(…, `parse_types=False`)               | `_strata`'s entry, main's call                                                               | `_dumps_hook.{loads,load,search,query}_typed`: parse through `_strata`, then revive               |
| `dumps_with_default(obj, default, *, return_type="str", native=True)` | `native=False`: every native family goes to `default` — main `38eaa9f`'s hook, byte for byte | `native=True` (default): native rows first, then `default` (this record's "`dumps_with_default`") |

`native` must be a `bool`: the facade tests `native is False` first (one
identity test on the default path), then `native is True`; anything else is
`TypeError("native must be a bool, not %s")` before any work. `parse_types`
keeps its contract: anything but `False` goes to the hook, which validates it
(`0` and `None` are the `TypeError`) in the error order the tests pin.

### Composition of the hook image

- `python_dumps_hook.cpp` — `python_dumps.cpp` compiled with `STRATA_DUMPS_HOOK`;
  every native writer (`write_native`, the per-kind writers, `NativeFrame`,
  `write_key_cold`, `push_open_cold`, the numpy twins' call sites) is under
  that macro, so `_strata`'s preprocessed token stream of `python_dumps.cpp` is
  main's. `write_unsupported` with no callable raises main's `TypeError`:
  `dumps_native` is `dumps_with_default` without a `default`.
- `python_native_types.cpp`, `python_numpy_twins.cpp`, `python_parse_types.cpp`,
  `python_parse_types_walk.cpp` — hook only.
- `python_files.cpp` and `python_folder.cpp` compiled with the hook macro: their
  **writer halves** (`dump_to_file`, `dump_to_folder` and the grouping they
  use, `file_is_ndjson`) link against the hook's serializer, because
  `dump_to_file` calls `dumps_to_python` and the hook defines that name as its
  native serializer; the readers sit under `#if !defined(STRATA_DUMPS_HOOK)`,
  which leaves `_strata`'s token streams unchanged.
- `src/strata/util/temporal.cpp` leaves `core_sources.txt` (which `_strata`
  links without dead-stripping) for `src/strata/native_sources.txt`, read by
  CMake (the temporal C++ tests) and by the hook `Extension` only.

| Option                                                                                              | Verdict                                                                                                                                             |
| --------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------- |
| **F1. Everything native in the hook, facade routes on a keyword (chosen)**                          | `_strata` main's by construction; the default path pays one Python `is` test (measured per call); `native=True` runs the unprofiled hook image      |
| (b) V4 tail in `_strata`, natives in the hook                                                       | keeps the tail the kill criterion could not separate from layout; default-on dropped by the user, so the tail buys nothing                          |
| S2. Retry the document in the hook after `_strata` raises                                           | refused as before (user code runs twice)                                                                                                            |
| **PA. The hook parses through `_strata`'s public entries, then revives (chosen)**                   | one parser and one config: `duplicate_key_policy` is a thread-local of `_strata`, honoured because `_strata` parses                                 |
| PB. The hook links its own reader copy                                                              | a second parser that `config.set` never reaches (its own thread-local) — refused                                                                    |
| **DD. The writer halves of `python_files.cpp`/`python_folder.cpp` compiled into the hook (chosen)** | one source for the file writer and grouping; `_strata`'s token streams unchanged                                                                    |
| DB. A hook-local copy of the file writer and grouping                                               | duplicated source that can drift — refused                                                                                                          |
| DA. Link every reader TU into the hook                                                              | a second parser, cursor and JSONPath image and every symbol they reference (an unresolved external is a link error on MSVC) for no caller — refused |

`dump_native` repeats `strata_dump`'s dispatch (split_by → folder, a directory
target without `split_by` → `ValueError`) because `python_module.cpp` stays
`_strata`'s; the dump contract tests run on both arms to pin that the two are
the same.

### Acceptance

Default path (the M12b identity proof, each an observable):

1. **Token streams**: every `_strata` translation unit (setup.py
   `BINDING_SOURCES` + `core_sources.txt`), preprocessed with `_strata`'s
   flags, equals main `38eaa9f`'s, on arm64 and x86-64.
2. **Build spec**: `_strata`'s `Extension` (sources, order, macros, compile and
   link arguments) and `core_sources.txt` equal main's.
3. **Plain images**: `_strata`'s code section equals main's
   (`benchmarks/image_identity.py`; normalised disassembly as the diagnostic).
4. **Held profile**: this branch's `_strata` built PGO+LTO against main's
   profile, in the same path, equals main's image built against it — locally on
   the M1, and on every CI leg in the prepared dispatch.
5. **Training scope**: the native suites stay outside the instrumented pass
   (`--training`); trained-scope test files equal main's except the recorded
   removals.

Opt-in path: the existing `tests/{unit,py}/native_types/` corpora pass through
the flag; `native=False` raises main's `TypeError` for every native family;
`dumps_with_default` serves natives first; `import strata` imports neither the
hook image nor a native-type module; the facade's per-call cost is measured in
ns for every routed function (evidence `docs/benchmarks/evidence/M15b/`).

Benchmarks: a separate declared workload, `native-v1` (seeded dataset carrying
`datetime`/`date`/`UUID`/`Decimal`/`Enum`/dataclass/`set`), reported in its own
`ci_summary` section; the canonical 27 rows and 135 denominator are unchanged.

Rollback: revert the merge. `_strata` is main's, so the canonical standings
cannot move through this change.

## Hook profile and native emitter costs (M15c; 2026-09-29)

**Problem.** On native-v1, strata `native=True` trailed msgspec on every leg
(ledger M15b, run 36585989834: `dumps` 1.27–1.60×, `dump` 1.12–1.34×). Phase 0
(ledger M15c) split the M1's 1.52× into the build and the code: the hook image,
built unprofiled and without LTO by the M12b rule, reads 0.874× \[0.865, 0.880\]
of itself with LTO and a profile of its own (1.52× → 1.33×); the rest sat in
three emitters (per object against msgspec: `Enum` +87 ns, dataclass +86 ns,
`UUID` +26 ns) and in a numpy probe every non-pure conversion paid. The
mutation-safety contract (`latch()` per conversion, the row latch per record)
prices at 6.2% of a record walk and is kept as written.

### Hook profile

`_strata`'s build and profile are untouched: its two phases run exactly as
before, with the hook unprofiled throughout and guarded so. A third phase
(clang: `scripts/pgo_build.sh`; clang-cl: `scripts/pgo_build_clang_cl.py`)
then rebuilds the hook alone (`STRATA_EXTENSIONS=strata._dumps_hook`):
instrumented (`STRATA_HOOK_PGO_MODE=generate`), trained by
`scripts/pgo_hook_training.py` — a seeded native corpus of its own (seed 7;
no benchmark module or dataset), plain documents through `native=True`, the
file writer and `dumps_with_default`, never numpy — and rebuilt against that
profile with ThinLTO (`STRATA_HOOK_PGO_MODE=use`; no LTO under clang-cl). The
build gate's runs of the instrumented hook write to a directory that is
discarded: the recipe is training-only, so test additions do not move the
hook's profile (E26-P8's lesson for `_strata`).

| Option                                                                       | Verdict                                                                                                                                                                                                                                       |
| ---------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **H1. A third phase for the hook alone, after `_strata`'s (chosen)**         | `_strata`'s phases, profile and image unchanged by construction; `_strata`'s image hash is checked equal across the phase; each image proven against its own profile with the other's named as foreign (`build_identity.py --check-profiled`) |
| H2. Instrument both images in phase 1, split raw profiles by image signature | changes `_strata`'s phase 1 (its one-signature guard) and mixes the two trainings' timing; refused                                                                                                                                            |
| H3. Train the hook on the gate-inclusive recipe                              | every test addition would move the hook's profile, E26-P8's defect; refused                                                                                                                                                                   |

The invariant M12b stated as "the hook is built unprofiled" becomes: **the
hook never carries `_strata`'s profile, and nothing the hook runs enters it**
— unprofiled while `_strata` trains and optimizes, its own profile after.
gcc keeps the hook plain (every CI leg builds with clang); MSVC refuses the
hook variables.

### Emitters (hook-only source; `_strata`'s token streams unchanged)

1. **Lazy group probes** (`classify` → `evaluate`): each type-table group is
   probed only when the checks ahead of it failed; a loaded-but-unresolved
   group ends the lazy pass and the old order runs. Every result is the old
   one (docs/decisions.md, 2026-09-29).
2. **UUID halves without objects** (`split_uuid_int`): `PyLong_AsNativeBytes`
   (3.13+) or `_PyLong_AsByteArray` read the 128 bits; out of range is the same
   `ValueError` as before.
3. **`Enum.value` as `_value_`** (`capture_enum_value`, `stock_enum_value`):
   proven once when enum resolves, checked per member; row 6's
   `getattr(member, "value")` is kept exactly (docs/decisions.md, 2026-09-29).
4. **Dataclass keys escaped once per type** (`encode_keys`) and
   `__dataclass_fields__` read through the MRO when that is exactly `getattr`
   (`plain_class_attribute`).
5. **Native writers not cold in the hook** (`STRATA_NATIVE_FN`): out of line,
   so the tail stays an inlining boundary, without the size bias `cold` gave
   them.
6. **UUID digits read in place** (`uuid_digits`, proven against the byte export
   when uuid resolves) and **hex written eight digits per word**
   (`util::format_uuid`).
7. **Type verdicts** (`classify`): the kind decided for a type is reused while
   the type's version tag is current -- proven at module init to go stale on
   every modification of the type or a base -- and only when the type alone
   decided it.

### Acceptance

- `bash scripts/token_identity.sh`: every `_strata` TU token-identical to main.
- `make pgo`: phase 3 passes `--check-profiled` both ways and `_strata`'s hash
  check; both gates green on the optimized hook.
- Native rows A/B (≥ 30 repeats per launch, A-B-B-A), canonical rows untouched
  by construction (`_strata` identical), oracle and contract suites green.
- Kill criterion: a native row that reads worse than the shipped hook on any
  leg of the five-leg A/B, or a `_strata` identity failure, stops the merge.
