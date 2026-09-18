# Decision record: the `dumps` unsupported-type hook (`default=`)

Status: **accepted and implemented** (2026-09-18, `exp/m12-default-hook`) —
criteria 1–4, 7 and 8 of roadmap M12 are met; criteria 5 (the tests-matched
five-platform A/B) and 6 (two CI samples) are pending dispatch. Four points the
record left underdetermined were resolved in implementation and are recorded in
`docs/decisions.md` under 2026-09-18: the chain bound names the returned object
and not the walk, the facade omits the keyword when there is no hook, no
`tests/cpp` file is added, and the PGO prohibition binds
`scripts/pgo_training.py` (pinned by a test) while the gate tests the recipe
also profiles are what criterion 5 prices.
Area: `src/strata/bindings/` only. Nothing in `include/strata/` or
`src/strata/{json,search,util}` changes; the C++ core gains no surface.

## Problem

`strata.dumps` supports a closed type set (`docs/context/api.md`, Parse &
serialize). Everything else ends at one branch —
`src/strata/bindings/python_dumps.cpp:228`,
`PyErr_Format(PyExc_TypeError, "Object of type %s is not JSON serializable", ...)` —
with no escape hatch. Every rival has one: stdlib `json` and `ujson` take
`default=`, orjson takes `default=` plus a fixed set of native emitters,
msgspec takes `enc_hook=`. A caller with `datetime`, `UUID`, `Decimal`,
`Enum` or a dataclass in the tree today has to pre-walk the structure in
Python, which costs more than the parse and the serialize together and makes
strata unusable for the workloads that most want it.

## Decision

**Option C: `default=callable` now (stdlib/orjson semantics), native
emitters per type later, each admitted only on a measured win.**

Rejected:

- **A alone** (`default=` and nothing else, forever) — leaves the six common
  types paying a Python call per object with no path to fixing it. Not a
  refutation of A's design, only of A as the end state.
- **B now** (native emitters for `datetime`/`date`/`UUID`/`Decimal`/`Enum`/
  dataclass, no hook) — refused on three grounds, none of them the hot path:
  B's type probes would sit at the same already-failing tail as A's hook, so
  neither is a hot-path cost. What B costs is (i) *coverage*: it answers six
  types and leaves the seventh (`pydantic`, `attrs`, `numpy` scalars, ORM
  rows) with no answer at all, so A has to be built anyway; (ii) *semantics*:
  six type surfaces (tz-aware vs naive, `Decimal` precision and NaN, `Enum`
  of non-scalar values, `dataclass` field order and `InitVar`) become
  test-pinned contract before any caller has asked for a specific spelling;
  (iii) *evidence*: the win B claims over A is the per-object cost of a
  Python call, and that number does not exist yet. The ledger's rule applies
  (`docs/performance/SKILL.md`, negative results): a lever with no measured
  win does not land. B is not rejected — it is deferred behind its own
  measurement, per type.
- **A sentinel protocol** (`__json__`/`__strata_json__` dunder lookup) — a
  type lookup on the failing tail is as cheap as the hook, but it requires
  the caller to own the type. Most unsupported types are third-party. Can be
  added later; orthogonal.

### Admission gate for each B increment

A native emitter for one type lands only when all of these hold: a
microbenchmark (≥ 30 repeats, `docs/context/benchmarks.md`) shows the hook's
per-object cost for that type and the emitter's, on the same host; the
five-platform same-runner A/B against a tests-matched arm resolves no
canonical row past its A/A floor; the emitter's output is pinned against
stdlib `json` with a documented `default` as the oracle; and the type's
semantics get a line in `docs/context/api.md`. One type, one A/B, one
ledger entry.

## Public contract

```python
strata.dumps(obj, *, return_type="str", default=None)
strata.dump(obj, path, *, split_by=None, default=None)
```

`default` is keyword-only, `None` or a callable of one argument. It is called
**only** where `dumps` would otherwise raise
`TypeError("Object of type %s is not JSON serializable")`, and its return
value is serialized in the unsupported object's place.

### Error contract (every message below is test-pinned)

| Condition                                                            | Result                                                                                                                                                                                                                                                                                                                                        |
| -------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `default` is neither `None` nor callable                             | `TypeError("default must be callable, not %s")` (tp_name), raised at the fastcall boundary before any byte is produced                                                                                                                                                                                                                        |
| unsupported type, `default is None`                                  | unchanged `TypeError("Object of type %s is not JSON serializable")` — byte-for-byte the message today's callers test against                                                                                                                                                                                                                  |
| the callable raises                                                  | the exception **propagates unchanged**: same type, same args, no chaining, no wrapping, no `__context__` fabrication. `KeyboardInterrupt`, `MemoryError`, `SystemExit` included. Nothing is written to the destination of a `dump` (same property as the `UnicodeEncodeError` rule already in api.md — `dump` writes only a completed buffer) |
| the callable returns an unsupported type                             | `TypeError("default() returned an object of type %s that is not JSON serializable")` — a **distinct** message, and the callable is **not** invoked a second time                                                                                                                                                                              |
| the callable returns `None`                                          | JSON `null`. `None` is a supported value, not a "cannot handle" sentinel                                                                                                                                                                                                                                                                      |
| the callable returns a `str` with no UTF-8 encoding (lone surrogate) | `UnicodeEncodeError`, as for any other `str` — unchanged                                                                                                                                                                                                                                                                                      |
| a non-`str` **dict key**                                             | unchanged `TypeError("keys must be str, not %s")` (`python_dumps.cpp:1154`, `:1190`, `:1451`). **`default` never applies to keys**                                                                                                                                                                                                            |
| a `split_by` value that is not `str`/`int`/`bool`                    | unchanged `ValueError`/`TypeError`. **`default` never applies to split values** — grouping happens before serialization                                                                                                                                                                                                                       |

Keys and split values are excluded deliberately: a key hook would put user
code inside the KeyCache and the key predictor's speculative path, which is
the hottest code in the serializer, for a case no rival supports either.
Explicit in the signature, explicit in the messages.

**Chain bound: 1.** One `default` call per object. stdlib `json` re-enters
`default` on the returned object without limit (its circular-reference marker
is what eventually stops `default=lambda o: o`); orjson caps the chain at 254
and raises `TypeError`. strata raises on the first unsupported return. This
is a divergence and gets a line in api.md: a caller who wants a chain writes
the loop in their own callable, where it is visible.

## Re-entrancy — the hook is the fifth user-code step

`docs/context/api.md` ("Mutation during serialization") enumerates four
steps at which user code can run inside `dumps`. The hook is a fifth, and
unlike the other four it is **not** rare — it runs once per unsupported
object, which is the whole point of the feature. The api.md clause must be
amended in the same change:

1. add the hook as a fifth step, and its consequent step: releasing the
   reference the hook returned can fire a `__del__` or a weakref callback,
   exactly as the existing orphan rule describes;
2. **amend the sentence "nothing else in a successful `dumps` calls into
   Python or allocates an object the collector tracks, so no collection can
   run one either."** With a hook present that is false: the callable
   allocates tracked objects, so a GC pass — and every `__del__` it fires —
   can run inside a *successful* `dumps`. The mutation rules that follow
   (lists and tuples followed live; a dict of ≤ 24 exact-`str` keys below 64
   levels emitted as the row read on entry; wider dicts followed live) are
   unchanged and continue to hold, because they are properties of the
   writers, not of which step re-entered.

Machinery — reuse, do not invent:

- **`user_steps_`** (`python_dumps.cpp:86`, `:401`, `:424`, `:439`,
  incremented at `:1596`): the walk's own user-code counter. The hook call
  site must `++user_steps_` so the sequence loop re-derives `ob_item` and
  `ob_size` after it (the precedent is the 2026-09-06 use-after-free: a
  hoisted item array read freed memory when user code resized the list —
  `docs/performance/experiment-ledger.md`, E26-FIX1/FIX2). `user_steps_` is
  the *only* sanctioned signal; E26-P24's rejected lever (3) records why it
  must not be reused for table growth, and that reasoning does not apply
  here — the hook genuinely runs user code.
- **`latch()`** (`python_dumps.cpp:~500`, used by
  `emit_cycle_placeholder()`): take the references the walk still needs
  *before* the hook runs, and before the returned reference is released.
  Same discipline as the `cycle_policy="warn"` handler.
- **`SchemaCacheLease`** (`python_dumps_output.h`). A hook that calls
  `dumps` again is a nested call, which leases `fallback_` — the path that
  used to leak every remembered key (E26-FIX2b). That leak is **already
  fixed on main**: T1's `~SchemaCacheLease` releases the fallback state's
  keys (`python_dumps_output.h:560-562`, ledger T1, destructor priced at
  zero on three legs, review not refuted). It stays a **prerequisite** of
  this milestone rather than work: with a hook, a caller who writes
  `default=lambda o: strata.dumps(o.payload)` makes the nested lease
  ordinary instead of rare, so the milestone re-pins the fix with a
  refcount test that drives the nested call *through the hook itself*.

## Depth, cycles and the caps

- The hook is invoked at the current `open_count_`, inside no new frame. The
  returned value re-enters `write_value`, so a returned container takes the
  ordinary `write_sequence`/`write_mapping` path: its own push increments
  `open_count_`, and at the boundary it raises
  `ValueError("Maximum serialization depth exceeded")` exactly as a direct
  child of the same parent would. The cap stays `Py_GetRecursionLimit()`
  (`python_dumps.cpp:170`, `:372`).
- Added C-stack recursion is **one** `write_value` frame per hooked object,
  bounded by the chain bound of 1. No new unbounded recursion is introduced;
  this is the property that makes the chain bound a safety rule and not a
  taste rule.
- The unsupported object is **not** pushed onto `open_` — it is never
  written, only consulted. The returned value goes through the unchanged
  cycle probe (`std::find(open_.begin(), open_.end(), object)`), so a hook
  returning a container that is already open reports a cycle under the
  active `cycle_policy` — `null` + `RuntimeWarning`, `ValueError`, or silent
  `null` — identical to that container appearing directly at that position.
  The one-container-late placeholder caveat (`docs/decisions.md`,
  2026-09-11 and 2026-09-12) applies to a returned value exactly as it
  applies to a directly-reached one, by whichever writer takes it: no new
  caveat, no exemption.
- `emit_cycle_placeholder()` is `STRATA_COLD_FN` and calls `latch()` before
  the warning; the hook site follows the same shape.

## `loads` needs nothing

No parse-side counterpart lands here, and this is a decision, not an
oversight:

1. There is no failing branch to hang it on. JSON's type set is closed, so
   `loads` has no "unsupported input type" case — the inverse feature is not
   an escape hatch but a *transform* (`object_hook`/`dec_hook`), a different
   thing with a different justification.
2. Placement is hostile. The only place an `object_hook` can run is inside
   `PythonObjectBuilder` at container close, i.e. inside the hot build path,
   the key predictor's speculative-key machinery and its per-input
   retirement (wave 22). The serializer's hook sits on a branch no canonical
   row executes; a builder hook sits on the branch every row executes. Any
   such proposal needs its own record and its own five-leg A/B.
3. The round-trip law is unaffected: a hook's output is ordinary JSON, so
   `loads` returns the plain form of whatever the callable produced. Nothing
   in `docs/context/api.md`'s round-trip clauses moves.

## Hot-path placement

The requirement is **codegen neutrality for `default=None`**.

- **Argument plumbing.** `strata_dumps` (`python_module.cpp:277`) parses
  keywords by walking `kwnames` with one `PyUnicode_CompareWithASCIIString`;
  `default` adds a second compare **inside the `kwnames != nullptr` branch
  only**, so `dumps(obj)` — and every canonical benchmark call — pays
  nothing. `dumps_to_python` (`python_types.h:95`) grows one parameter
  (`PyObject* default_fn`, `nullptr` for absent), stored once into the
  walker's state. It is **not** threaded through the writers: E26-P6 priced
  exactly that, and `write_mapping_body`'s sixth argument was one of the
  three findings that cost the x86 legs 2–6% until it went back to four.
- **The branch.** The hook test replaces the `PyErr_Format` at
  `python_dumps.cpp:228` with a call to one `STRATA_COLD_FN`
  (`python_types.h:50`) out-of-line function that either invokes the hook or
  raises. The null test is reached only after every supported type check has
  failed, i.e. never on any canonical row.
- **The PGO trap.** E26-P23 is the precedent to obey: when the training
  payload exercised a path marked `cold`, block placement believed the
  profile over the attribute and Windows lost `dumps mixed` +4.3% (run
  34665612473), which only the paired training payload fixed (run
  34670240916). Therefore: **the PGO training workload must contain no
  `default=` call and no unsupported-type raise.** The attribute alone is not
  protection; the profile is what decides.
- **Footprint.** One pointer joins the walker's per-call state. The
  serializer's cold footprint and state size were measured in wave 20 and
  `experiments/footprint/`; the pointer must not displace anything out of
  the hot state's first cache line. If it would, it lives in the per-call
  state the lease already carries.
- **Pricing method.** The added tests move the profile by themselves — the
  gate-inclusive recipe makes the suite 47.5–55% of profile counts, and
  E26-P7b priced a tests-only commit at linux-x86_64 `dumps flat` +3.4% and
  N2 `dumps flat`/`users` −2% (run 34594271053). So the code is priced
  **against an equal-tests arm**: the before-arm carries the same new tests
  with the runtime change reverted, both built by `make pgo` on the same
  runner, ABBA blocks against a fresh A/A floor. A "before" without the
  tests measures the profile, not the hook.

## Test placement

- **`tests/unit/`** — contract mirrors, one named test per api.md clause
  with the clause cited in a comment (`test_dumps_default_hook.py`): each row
  of the error table above; `default` not applying to keys or split values;
  `None` → `null`; chain bound of 1; exception propagation identity
  (`type`, `args`, `__context__` untouched); depth boundary; each
  `cycle_policy` with a hook returning an open container; refcount neutrality
  of a nested `dumps` (E26-FIX2b's pin); `default=None` byte-identity against
  a no-`default` call over the generated corpus.
- **`tests/py/`** — integration: `dump`/folder `dump` with a hook, NDJSON,
  a hook that mutates the container being written (list resize both ways,
  dict clear), a hook that calls `dumps` re-entrantly, and the stdlib oracle
  `json.loads(strata.dumps(o, default=f)) == json.loads(json.dumps(o, default=f))`
  over a generated corpus of JSON-safe hooks.
- **`tests/cpp/`** — **no new file.** The cross-layer parity rule
  (`docs/context/convention.md`, Testing gates) triggers on "behavior
  exposed to Python"; this change adds no core behavior — no header, no
  `src/strata/{json,search,util}` file, and no `serialize_json` surface
  moves. The existing C++ serializer suites stay the byte-identity oracle,
  and `tests/unit/test_core_purity.py` is the guard that the hook never
  leaves `src/strata/bindings/`. This scoping is an ambiguity resolved
  visibly and goes to `docs/decisions.md`.
- **`tests/integrations/`** — a **new third tree**, holding tests that
  import third-party libraries to show the hook composes (`pydantic`,
  `attrs`, `numpy`, a dataclass-heavy ORM row). It is **excluded from
  `pyproject.toml`'s `testpaths`** (today `tests/py`, `tests/unit`), from
  `make test` and `make gate`, and from the PGO training run; it gets its own
  `make test-integrations` target, its own optional-dependency extra, and its
  own CI job. Justification, for `docs/decisions.md`: under the
  gate-inclusive PGO recipe every test addition is a several-percent move on
  some row (E26-P7b, E26-P8), so tests whose cost is dominated by heavy
  third-party imports must not enter the profile; and a green gate must not
  depend on third-party packages the project otherwise has none of
  (convention: no dependency fallbacks).

## Falsifiable estimate and kill criterion

Estimate: with `default=None`, zero canonical rows move past their A/A
floors on any of the five legs, and `write_value`'s object code is unchanged
apart from the tail's null test. Text growth ≤ 512 B, measured as `size -m`
**Section `__text`** (the page-rounded Segment `__TEXT` once hid a +2328 B
regression — use the section).

`write_value` is this file's name for `Serializer::write`
(`src/strata/bindings/python_dumps.cpp`), the walk's type dispatch; the source
has no symbol spelled `write_value`. Measured outcome of the estimate (the
codegen pair, both ISAs, in the ledger's M12 entry): `write()` is **smaller**
on both — 281 → 259 instructions on arm64 and 191 → 187 on x86-64, because the
tail's inline `PyErr_Format` argument setup is replaced by one call to the cold
`write_unsupported` — with the leading 59 (arm64) / 76 (x86-64) instructions,
the whole exact-type dispatch chain, bit-identical. Section `__text` across the
four changed translation units grows **+500 B** on arm64 and **+480 B** on
x86-64, inside the bound.

Kill criterion: if the tests-matched five-leg A/B resolves any canonical row
against strata past its floor and the cause is the code rather than the
profile, the hook does not ship in this shape — the next candidate is a
separate entry point (`dumps_with_default`) whose existence cannot perturb
`dumps`'s codegen at all.
