# Decision record: native types — serializer default-on, opt-in `parse_types`

Status: **accepted for implementation** (2026-09-28), branch `exp/native-types`
over main `38eaa9f`. Roadmap: M15. Scope approved by the user on 2026-09-28:
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
  resolved type table, the dataclass field-name cache, and the conversions from
  a Python object to fields or text. Resolution reads `sys.modules` only — it
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
exactly as today, before any native check:

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
- **Enum** chains are followed in a loop inside `write_native`, not by
  recursion: after `limit` hops (the serializer's depth limit) it raises
  `ValueError("Maximum serialization depth exceeded")`. The final value goes
  through `write()` once.
- **numpy** writes what `item()`/`tolist()` return, which contains only
  supported scalars and lists, so no native object recurses back into the tail.
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

**Forward note, not in this change.** `exp/m13-adapters` (unmerged) hands
frameworks `dumps_with_default` so that their own `default` formats
`datetime`, `Decimal` and `UUID` (Flask as an HTTP date and a string; Django as
ECMA-262 and a string). With this record merged those conversions no longer
reach the framework's `default`, which changes the JSON type of a `Decimal`
from string to number in those responses. M13 must reconcile it before it
merges; the candidate is an opt-out keyword on `dumps_with_default`
(`native_types=False`, the hook image's state only) — recorded, not built.

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
itself. For a JSON object member whose name is registered:

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
   on `write_native`, the revival walk and the new keyword arms.
5. **Keywords**: `parse_types` joins `strata_loads`'s keyword loop as a third
   compare (the facade already passes two keywords per call);
   `load`/`search`/`query` add one format unit. `dumps`'s signature does not
   change.
6. **Import**: `import strata` imports none of `numpy`, `datetime`, `uuid`,
   `decimal`, `dataclasses` (a fresh-interpreter test), and its import time is
   unchanged (paired, fresh interpreters).
7. **Footprint**: `_strata`'s `size -m` **Section `__text`** growth is reported
   per ISA, and attributed per symbol (`benchmarks/symbol_sizes.py`); every
   added function has zero training counts. Estimate: 8–16 KB.

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
