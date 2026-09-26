# Decision record: `dumps_with_default` — the unsupported-type hook as a separate entry point

Status: **draft** (2026-09-26) — design only, no code written. Successor to
[`dumps_default_hook.md`](dumps_default_hook.md), whose in-signature shape
(`dumps(..., default=)`) was implemented twice and refused by its own kill
criterion (docs/performance/experiment-ledger.md, M12). This record keeps that
record's semantics and changes only where the hook lives; everything below that
is not restated is inherited from it. Roadmap: M12b.

Area: `src/strata/bindings/` and packaging. Nothing in `include/strata/` or
`src/strata/{json,search,util}` changes; the C++ core gains no surface.

## What M12 established, and what this record must therefore guarantee

Run 36254514783 held the profile equal by construction (both arms trained on
main's test suite and payload) and still resolved losses whose cause is code:
linux-x86_64 `dumps flat` +0.99–1.67%, the same row the first implementation's
source-alone pair lost on both of its draws with both arms pinned. Two
implementations, three draws, one sign. What they share is a null test on
`Serializer::write`'s unsupported tail plus two words of walker state; what the
first attempt's draws 3/4 added is that a *rigid* shift of byte-identical parse
code (+224 to +608 B, 16 B on macos-x86_64) could not be separated from host
drift. So:

- **"Unchanged apart from" is not a property this project can price.** The M12
  instrument resolves 0.5–1.5% effects on single rows, and effects of that size
  followed from a tail test, a 16-byte frame growth and a rigid layout shift.
- The successor has to leave the `dumps` path **byte-identical** — not
  instruction-count-identical, not "the hot head identical": the same bytes at
  the same addresses — or it inherits M12's attribution problem.

## Decision

**A second extension module, `strata._dumps_hook`, compiled from the same
serializer source with the hook enabled; `strata._strata` is built from
token-identical sources with unchanged flags and therefore does not change at
all.** The public entry point is
`strata.dumps_with_default(obj, default, *, return_type="str")`.

### Codegen isolation — options and why this one

| Option                                                                                | `dumps` object code                                                                                                                                                                                                 | `_strata` linked image                                                                                                                                                                                                                                                                                                                                    | Why not / why                                                                                                                                               |
| ------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------- |
| In-signature hook (M12)                                                               | tail null test, frame +16 B                                                                                                                                                                                         | shifts                                                                                                                                                                                                                                                                                                                                                    | refused by M12's kill criterion                                                                                                                             |
| Shared cold machinery (callable in the lease's per-call state, write's tail tests it) | tail null test remains                                                                                                                                                                                              | shifts                                                                                                                                                                                                                                                                                                                                                    | the first attempt's source-alone pair (pin on both arms, the hook the only difference) still lost `dumps flat`: the tail test alone is enough (ledger, M12) |
| Template twin in `python_dumps.cpp` (`Serializer<kHooked>`)                           | *probably* identical for `<false>`, not provably: instantiation order, symbol names and per-caller inlining are the compiler's (E26-P9: a second instantiation of the fused writer trains and lays out differently) | `python_dumps.o` grows ~29 KB, so every later function moves                                                                                                                                                                                                                                                                                              | no proof of identity, and a guaranteed layout shift                                                                                                         |
| Same-source second TU in the same image, linked last                                  | byte-identical by construction (token-identical preprocessing)                                                                                                                                                      | registration needs a `PyMethodDef` entry in `python_module.cpp` (its `__data` grows 32 B, so data after it moves); the new TU's `__text` precedes `__TEXT,__const` on Mach-O, so every constant table moves by ~30 KB; on ELF with PGO, zero-count functions may be grouped into `.text.unlikely` ahead of hot code (not verified for this build's flags) | object-level zero, image-level not: the class of effect draws 3/4 could not attribute. **Named fallback** if the separate module's tooling cost is refused  |
| **Same-source TU in a separate extension module**                                     | **byte-identical by construction**                                                                                                                                                                                  | **byte-identical**: no `_strata` source token, flag or source-list entry changes                                                                                                                                                                                                                                                                          | chosen                                                                                                                                                      |

**How one source serves both images.** `python_dumps.cpp` keeps every writer,
once. The hook's additions (the walker's `default_`/`hooked_` state,
`set_default`, `write_unsupported`, the tail test in `write`) sit under
`#if defined(STRATA_DUMPS_HOOK)`, and the few definitions only `_strata` owns
(`dumps_to_python`, `prepare_dumps_runtime`, `g_cycle_policy` and its
accessors) under `#if !defined(STRATA_DUMPS_HOOK)`. The one token the hooked
build must read differently — `g_cycle_policy` in the two cold cycle handlers —
is spelled through a macro that expands to exactly `g_cycle_policy` in the
unhooked build. A new TU, `src/strata/bindings/python_dumps_hook.cpp`, defines
`STRATA_DUMPS_HOOK`, includes `python_dumps.cpp`, and adds the module init, the
fastcall entry and the cycle-policy bridge. Consequences:

- `_strata`'s `python_dumps.cpp` preprocesses to the **same token stream** as
  main's (checkable with `clang++ -E -P`), so with the same compiler and flags
  its object is the same bytes. Every other `_strata` TU is untouched, and so is
  `setup.py`'s `_strata` `Extension` (sources, order, flags).
- The writers keep one definition. The M12 hook code carries over as it is on
  `exp/m12-default-hook-2` (`write_unsupported`, the identity chain bound,
  `set_default`), now inside the `#if`.
- The convention's one-TU rule for the writers holds per image: each image
  compiles all of them in one TU (`docs/context/convention.md`, Layout).

**Shared state across the two images.**

- *Cycle policy.* `config.set("cycle_policy", …)` writes `_strata`'s
  `g_cycle_policy`; the hook image must honour it without `_strata` exporting
  anything new (a capsule would change `_strata`'s init). The hooked build's
  two cold cycle handlers read it at the cycle point through
  `strata._strata.config_get("cycle_policy")`, fetched once at the hook module's
  init. That call runs no user code and allocates one untracked `str`, so it is
  not a user-code step under the five-step contract. Reading at the cycle point
  (not once per call) keeps `dumps`'s semantics: a policy the callable changes
  mid-walk applies to later cycles in both entry points.
- *Raw-dict layout proofs.* `rawdict::proved_layouts()` is an `inline`
  function-local static, one per image. The hook module's init resolves its own
  copy, as `_strata`'s `prepare_dumps_runtime()` does, before any walk — the
  FIX1-REVIEW hazard (a proof resolving mid-walk) must not reappear in the new
  image.
- *Schema cache and output buffer.* `SchemaCacheLease`'s thread-local state and
  `dumps_to_python`'s thread-local buffer are per image. A `dumps` called from
  a hook leases `_strata`'s (free) state; a `dumps_with_default` nested in its
  own hook takes the hook image's fallback — the T1 destructor release applies
  in both images, so E26-FIX2b is re-pinned in both directions.

**Cold-footprint price of the second writer instantiation.** In `_strata`:
**zero bytes**, by construction — the question E26-P24 and the footprint
experiments ask of `dumps` cannot arise. In the hook image: one copy of the
writers (`python_dumps.o` `__text` is 29 040 B arm64 / 29 198 B x86-64 on
main) plus the core code it links (escaping, number formatting), measured by
criterion 4. A process that alternates `dumps` and `dumps_with_default` keeps
two copies of the writers warm, and each competes with the other for L1I the
way a rival does (E26-P0's interleave finding) — a cost of mixed use, on no
canonical row, recorded rather than engineered away.

**PGO.** The hook module is built **without** PGO — plain `-O3`, uninstrumented
in the training phase and without `-fprofile-use` in the optimized phase — so
nothing it executes enters `_strata`'s profile (its core-code copies carry the
same external names as `_strata`'s, and IR PGO keys external functions by name).
A profile for the hook image is a later lever with its own measurement.

## Public contract

```python
strata.dumps_with_default(obj, default, *, return_type="str") -> str | bytes
```

`default` is required, positional-or-keyword, and must be callable. Every rule
of the M12 error table carries over, re-targeted to this entry point:

| Condition                                            | Result                                                                                                                                                          |
| ---------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `default` is not callable — **`None` included**      | `TypeError("default must be callable, not %s")`, before any byte is produced                                                                                    |
| an unsupported type                                  | `default(obj)` is called once and its return written in the object's place, at the object's depth, by the value path                                            |
| the callable raises                                  | propagates unchanged (`KeyboardInterrupt`, `MemoryError`, `SystemExit` included)                                                                                |
| the callable returns an unsupported type             | `TypeError("default() returned an object of type %s that is not JSON serializable")`; the callable is not called on its own return (chain bound 1, by identity) |
| the callable returns `None` / a lone-surrogate `str` | `null` / `UnicodeEncodeError`                                                                                                                                   |
| a non-`str` dict key                                 | the unchanged `TypeError("keys must be str, not %s")`; the callable is never called for a key                                                                   |
| a returned open container                            | a cycle under the active `cycle_policy`, reported where it was returned (the M12 ruling)                                                                        |
| a document with no unsupported object                | **byte-identical to `dumps(obj)`** in both return types; the callable is never called                                                                           |
| an invalid `return_type`                             | `ValueError("invalid return_type: %s")`, as `dumps`                                                                                                             |

The mutation contract (api.md) gains a paragraph for this entry point: five
user-code steps, the fifth (the callable) not rare, a collection possible inside
a successful call, the writers' mutation rules unchanged. **`dumps`'s own
clause stays exactly as on main — four rare steps — because `dumps` can no
longer reach a hook.** That is the contract-side statement of the isolation.

### What does not get a hook

- **`dump` (file and folder mode).** No counterpart in M12b. A file is
  `Path(p).write_bytes(dumps_with_default(obj, f, return_type="bytes") + b"\n")`
  plus the 0644/truncate details `dump` guarantees; folder mode with a hook has
  no request behind it, and giving it one would pull the file and folder writers
  into the hook image or add a second writer definition. Recorded for sign-off.
- **`loads`.** Unchanged reasoning from the M12 record.

### Surface

`__all__` grows by one name. The convention asks for one public entry point per
capability; the hook is a capability no existing entry covers, and this is its
one entry. The facade function delegates to `strata._dumps_hook` with no logic;
the module is imported eagerly by `strata/__init__.py`, keeping "`import strata`
fails loudly when an extension is missing" true for both images. Its import
cost and resident-memory cost are measured (criterion 9).

## Reuse from `exp/m12-default-hook-2`

| Part                                                                                                                                 | Carries over                                                              | Changes                                                                                                                                                |
| ------------------------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `write_unsupported`, the identity chain bound (`hooked_`), `set_default`, `Py_IncRef`/`Py_DecRef` on the cold path                   | unchanged, moved under `#if defined(STRATA_DUMPS_HOOK)`                   | the tail test in `write` exists only in the hooked build                                                                                               |
| the file header's five-step contract                                                                                                 | yes, stated for the hooked build                                          | comments are not tokens, so the header can describe both builds without moving `_strata`'s token stream                                                |
| error table, messages, chain bound 1, key exclusion                                                                                  | yes                                                                       | `default=None` means "refused", not "absent"; the `split_by` rows go with `dump`                                                                       |
| contract tests `tests/unit/test_dumps_default_hook.py` (221) and integration mirrors `tests/py/test_dumps_default_hook.py` (97)      | yes, re-targeted to `dumps_with_default`                                  | `dump`/folder cases dropped; `default=None` identity replaced by the no-unsupported-object identity; corpus-wide oracles use stdlib `json` (see below) |
| `tests/integrations/` (pydantic, attrs, numpy, dataclass rows), `make test-integrations`, its CI job, `scripts/integration_tests.py` | yes, re-targeted                                                          | —                                                                                                                                                      |
| api.md clauses                                                                                                                       | yes, as a new `dumps_with_default` section                                | `dumps`'s and `dump`'s clauses revert to main's text                                                                                                   |
| `default_converter` / keyword-loop changes in `python_module.cpp`, the facade's `default=` on `dumps`/`dump`                         | **no** — they are the in-signature shape                                  | the hook module has its own fastcall entry                                                                                                             |
| the output-stage pin (`alignas(64)` on `stage_`)                                                                                     | **no** — `python_dumps_output.h` is shared, and `_strata` must not change | may be enabled for the hook image alone under the macro, measured there                                                                                |
| the directory-target `BaseException` fix in `strata_dump`                                                                            | **no** — it is a `_strata` change                                         | a separate correctness change on main, its own record line (sign-off)                                                                                  |

**Test composition rule.** The new gate tests run inside the gate-inclusive
profile, and E26-P7b priced a test-suite change at several percent on some row.
The hook image is not profiled, so what the new tests can shift is only what
they run in `_strata`: calls to `strata.dumps`/`loads` and the facade. The
byte-identity oracle against `strata.dumps` therefore runs on a small fixed set
of documents, and corpus-wide checks use `json.loads(dumps_with_default(...))`
against the stdlib. Criterion 6 prices whatever shift remains on the shipped
build.

## Falsifiable estimate and kill criterion

Estimate: `_strata`'s plain builds are byte-identical to main's on both ISAs;
on the runners' PGO arms (both trained on main's suite) `_strata`'s `__text` is
byte-identical too, so criterion 5's A/B reads every row inside its floor on all
five legs — an A/A in all but name.

Kill criterion: if `_strata` cannot be shown byte-identical (criterion 4) —
a token, flag or link-order change it cannot remove — the design is wrong, not
the code slow; nothing is timed until identity holds. If identity holds and the
A/B still resolves a loss, the cause is host or profile nondeterminism, and the
evidence goes to a second draw and the identical-binary control, not to the
code.
