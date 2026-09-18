# Decision record (REFUSED): the record emit hand-off

Status: **implemented, measured and refused by kill criterion 1** — see
[the outcome](#outcome-refused-by-kill-criterion-1) at the foot of this record
before reading the rest of it. The design below is preserved as written,
because the reason it failed is not visible in it: every correctness argument
held, all six proof obligations passed, and it still lost. The measurement
that motivated it over-credited the change, and the section on the outcome says
exactly how.

This record amends
[the fused record writer](fused_record_writer.md); it does not replace it, and
every invariant in that document's "Invariants that must survive, byte for
byte" section is carried forward unchanged.

Origin: squad \[emit\], E26-P29 in
[the experiment ledger](../performance/experiment-ledger.md). That entry
measured the prize and refuted two cheaper ways of claiming it. This record is
the third way, written before any implementation because the two refutations
were both "correct, byte-identical, and slower", which is the failure mode a
design review catches and a build does not.

## Problem, with its evidence

`write_record_fused` constructs `DeferredOpen` and `RowLock` before its emit
loop, unconditionally, for every record. Both are needed only from the first
value that can run Python: the deferred open puts the dict on `open_` so
`latch()` can find it, and the row lock registers the staged row so the values
the loop has not yet read survive a mutation below it.

Two bound arms — deliberately incorrect, never shippable, reverted — price what
their presence costs. Each is compared against a base built from the same tree,
with a second copy of base in the same rotation as an identical-binary control.
`open_probe.py`, 60-repeat medians, `gc.collect()` per sample, arms swapped by
replacing the built extension, M1.

Plain `-O3 -march=native`:

| row | A/A control | pair removed | pair and two-pass removed |
| ---------------- | ------- | ------- | ------- |
| `mixed` | +0.82% | −9.80% | −10.81% |
| `flat` | +0.13% | −13.03% | −18.60% |
| `users` | +0.05% | −8.31% | −11.30% |
| `nested` | +0.15% | −13.80% | −18.47% |
| `wide_arrays` | +0.13% | +1.39% | +1.47% |

PGO+LTO, **profile held equal** — one `make pgo` on base under the shipped
gate-inclusive recipe, then every arm rebuilt with `PGO_MODE=use`,
`STRATA_ENABLE_LTO=1` and that same `strata.profdata`, so the only variable is
the source (the E26-P6/P7 method):

| row | A/A control | pair removed | pair and two-pass removed |
| ---------------- | ------- | ------- | ------- |
| `mixed` | +1.49% | **−4.98%** | −7.44% |
| `flat` | +0.23% | **−12.93%** | −16.55% |
| `users` | +0.47% | −4.42% | −7.26% |
| `nested` | +0.90% | −9.38% | −14.31% |
| `wide_arrays` | +1.65% | +3.18% | +0.86% |

So the effect is **not a plain-build artefact**: 51–99% of it survives
PGO+LTO, and on `mixed` the pair alone is 1.95 µs of a 39.08 µs call. It is a
register-allocation effect rather than an instruction count — the pair's
destructors put a cleanup path on every exit of the emit loop, and the loop's
allocation pays for that structure.

Two readings of that table are load-bearing for this design:

1. **The pair, not the second pass, is the prize.** Under the shipped recipe
   the pair alone is 67% of the combined effect on `mixed`, 78% on `flat` and
   66% on `nested`. A design that removes the pair and keeps the two-pass walk
   should therefore land near the middle column, and the middle column is worth
   having on its own.
2. **`wide_arrays` shows a cost**, +3.18% under PGO against a +1.65% control.
   It is list-dominated, so the dict path barely applies and this is most
   likely layout, but it is above its control and is an open question this
   design must answer rather than inherit.

## Decision

Split the emit loop in two at the point where the current loop changes
behaviour anyway.

- **The hot arm** — inside `write_record_fused`, carrying `MappingDepth` and
  nothing else — emits keys while their values are plain scalars. It
  constructs no `DeferredOpen` and no `RowLock`, so it has no cleanup path
  beyond the depth guard the bound arms already carried.
- **The continuation** — one new member, one call site — is entered at the
  first value that is not a plain scalar. It constructs `DeferredOpen` and
  `RowLock`, arms and registers exactly as the current loop does at that same
  value, and finishes the record from that index.

A record whose values are all plain never reaches the continuation and never
pays for the pair. A record that contains a container pays one call, once,
instead of paying the pair's structural cost on every record.

This is not a tail call in the strict sense and must not be written as one:
`MappingDepth level` guards the whole record and has to outlive the
continuation, so the hot arm calls it and returns its result. The bound arms
kept `MappingDepth` and still produced the table above, so the measured effect
is the pair's alone and does not depend on removing the depth guard.

## The carried-state contract

The continuation needs, and takes, exactly five values beside `this`:

| carried | why it cannot be re-derived |
| ------- | --------------------------- |
| `object` | `DeferredOpen::arm` needs the dict; nothing else holds it |
| `depth` | indexes `schemas_`, `staged_row` and `lock_node` |
| `way` | indexes `ways[way]`; a `Schema&` must not be carried, because a nested object may grow `schemas_` and move it |
| `index` | where to resume; the hot arm stopped here |
| `size` | the loop bound, and `RowLock::own`'s end bound |

`row` is **not** carried: the continuation re-derives it as
`staged_row(depth).values`, which is one conditional index. `entries` is not
carried either, and must not be — for a compacted general table it is the
lease's single scratch, dead the moment the verification pass filled the row
(fused_record_writer.md, and the 2026-09-12 compaction decision).

**Six arguments including `this` is the budget, not a coincidence.** SysV
x86-64 passes six integer arguments in registers, so this design uses the last
one and spills nothing. E26-P6 measured what the seventh costs: adding a sixth
argument to `write_mapping_body` was one of the three mechanisms behind that
entry's x86-64 regression, and removing it was part of the fix. A reviewer
should treat any growth of this list as a design change requiring its own
measurement, not an implementation detail.

If it does prove too wide on some leg, the fallback is packing rather than
spilling: `depth` is below `kMaxCachedDepth` (64), `way` below `kWays` (4) and
both `index` and `size` at or below `kMaxSchemaKeys` (24), so all four fit one
`uint32_t` and the contract becomes `this`, `object`, one word. That is a
measurable alternative, not a better default — it trades a spill for shifts in
the continuation's prologue.

## Cycle and re-entrancy semantics: nothing moves

This is the section most likely to go wrong, because the hand-off point is
adjacent to a placement E26-P9 already measured and declined.

- **The arming point does not move.** The continuation arms the container
  immediately before writing the first non-plain value — the same instant
  `DeferredOpen::arm` fires today. The dict is therefore on `open_` whenever
  `latch()` could need it, which is the invariant
  fused_record_writer.md states and the only one that matters here.
- **No cycle probe is added, removed or relocated.** The value path keeps its
  probe in `write_record_fused_value`, ahead of the fallback checks, where the
  2026-09-11 decision put it and where the two alternatives were measured and
  declined (a second instantiation costs `dumps mixed` +18% on the M1 and
  N2 +5–6%; a runtime flag costs `flat` +2–3%).
- **The documented placement caveat must not widen, and does not.** A repeated
  dict reached as an array *element* is still emitted once more before the
  `null`, because the element loop still does not probe the open-container
  stack; the affected population is unchanged — combined tables of at most 24
  exact-`str` keys at a cached depth. This design touches neither the element
  loop nor the probe, and a reviewer should check that claim by diff, not by
  reading this sentence.
- **Do not let this design grow a probe at the hand-off point.** It is the
  obvious-looking place for one, and E26-P9 already priced it: a probe at the
  first container value with the record's bytes taken back costs `dumps users`
  4%. Closing the element-loop gap remains a separate, declined question.
- **`RowLock::own` keeps its exact bounds** — `row`, `index + 1`, `size` — so
  the rows registered for `latch()` are the same set, and the E26-FIX1
  guarantee is unchanged: a large `int` is not a plain scalar
  (`is_plain_scalar` refuses it via `is_compact_int`), so every value that can
  reach `write_int`'s `latch()` path is the continuation's, never the hot
  arm's.

## Error paths

- **A plain value can still fail.** An exact `str` holding a lone surrogate
  raises `UnicodeEncodeError`, and `is_plain_scalar` calls such a `str` plain.
  The hot arm must propagate that failure. Nothing needs unwinding when it
  does: no container was pushed, no row registered, and `MappingDepth`
  unwinds as it does today — which is exactly what the current writer does
  when it fails before arming.
- **Failing can raise but cannot run user code.** The hot arm's values cannot
  execute Python, which is the whole invariant it rests on. Raising and
  running are different things and the record should not be read as conflating
  them.
- **The continuation's two failure returns are the current loop's**, moved
  verbatim, with the pair unwinding through its own destructors as now.
- **The zero-width precondition survives.** `size == 0` is still refused
  before this point, because the opening brace is key 0's separator
  (`emit_slot_key`) and a zero-width record would otherwise reach the closing
  brace having written no opening one. The hot arm inherits the existing
  `assert(size > 0)`.
- **Hand-off at index 0** is the ordinary case for a record whose first value
  is a container: the hot arm emits nothing, the continuation emits key 0, and
  key 0 writes the brace. Nothing special is required, and a test must pin it.

## The byte-identity claim, and how it will be proven

**Claim:** for every input, on every supported platform and CPython version,
the bytes are identical to the current writer's — including under mutation, all
three cycle policies and both return types.

The claim is structural, and a review should check the structure before it
reads a number: both arms emit through the single `emit_slot_key`, so no
separator or brace decision moves; and the split point is where the current
loop already changes behaviour, so no byte-producing decision is relocated.

Proof obligations, none of which may be waived:

1. `p24/differential.py`'s matrix — the three depths, dict depths 1–120 across
   the `kMaxCachedDepth` seam, `str`-subclass keys alone and interleaved, wide
   and narrow schemas alternating at one depth, tuples, `2**70`, `-0.0`,
   `1e300` — plus its 1 500 seeded random documents, against the current
   writer.
2. sha256 of `dumps` over all five small datasets in both `str` and `bytes`
   mode. Necessary, nowhere near sufficient: all five are non-mutating.
3. **A new test that forces the hand-off at every index.** For widths 1 through
   24, a record whose single non-plain value sits at each position in turn,
   including first and last, under both return types. This is the test the
   design exists to need, and no existing file covers it.
4. The cycle and placement suites unchanged and passing:
   `test_dumps_cycles_fused.py`, `test_dumps_general_tables.py`,
   `test_dumps_brace_fold.py`. Unchanged is the point — if any expectation
   needs editing, the placement moved and the design is wrong.
5. The mutation suites, which are what the pair exists for. A bound arm that
   removed the pair crashed the suite outright (SIGKILL on deep input, because
   nothing reached `open_`); the implementation must show the opposite.
6. ASan+UBSan (`make test-py-asan`) over the mutation and cycle suites. The
   pair's whole job is lifetime, and this design changes who constructs it.

## Falsifiable estimate and kill criterion

**Estimate.** The design keeps the two-pass walk, so it should land near the
pair-only column rather than the combined one: `mixed` −3 to −5%, `flat` −10
to −13%, `users` −3 to −4.5%, `nested` −7 to −9.5%, under the shipped recipe
with the profile regenerated.

**Kill criteria**, any one of which ends it:

1. Any of `mixed`, `flat`, `users` or `nested` fails to resolve a gain past its
   own A/A floor under the shipped recipe with a **regenerated** profile. The
   held-profile numbers above are evidence that the prize exists, not that this
   design collects it: a merged change retrains, and E26-P7b showed a test
   addition alone moves rows several percent under this recipe.
2. `wide_arrays` reproduces a resolved cost. Its +3.18% here is unexplained and
   the design owes an answer, not an excuse.
3. The carried state grows past six arguments including `this`, or the
   continuation is instantiated more than once. One body, one call site — the
   E26-P9 hazard is a second copy of an emit loop training cold under the
   gate-inclusive profile, at `dumps mixed` +18%.
4. Any byte of any output changes, or any expectation in the four suites above
   needs editing.

**Expected cost if accepted:** one new member and one call site in
`python_dumps.cpp`, which stays a single translation unit; no new
architecture-specific path, no `#ifdef`, no new placement macro.

## What this design deliberately does not do

- It does not remove the two-pass key walk. Verification must complete before
  any byte is emitted, because a schema whose `(size, first_key)` matched can
  still mismatch at a later key and fall back to `write_mapping` — and after a
  nested value has grown the buffer, `rewind` cannot take those bytes back
  ("never past an overflow"). Under the shipped recipe the second pass is only
  22–34% of the combined effect, so this is a small thing to leave behind.
- It does not dispatch on an all-record "all values plain" flag. Both
  placements of that flag were built, measured and refuted in E26-P29 —
  folded into the verification pass (`flat` +10.5%) and as its own reduction
  over the filled row (`flat` +14.2%) — and the second is why the idea, not
  its position, is what those measurements close.
- It does not touch `write_mapping`, the array element loop, the schema cache,
  the compaction, or any reservation.

## Outcome: refused by kill criterion 1

Implemented on `exp/emit-n2` 2026-09-18 and reverted the same session.
`python_dumps.cpp` is identical to `main`'s.

**All six proof obligations passed.** The differential (obligation 1,
reconstructed — the original `p24/differential.py` no longer exists on disk)
read **3 730 dump results identical** to the pre-change writer across 1 865
documents, including 52 `UnicodeEncodeError` rows from the lone-surrogate
`str` at every position before and after a hand-off, 2 depth-limit
`ValueError`s, and byte-identical cycle placement on all three of
`cycle/direct`, `cycle/element` and `cycle/value`. sha256 over five datasets in
both modes matched (obligation 2). The new every-index test pinned 10 242
documents across widths 1–24 × every position, mirrored into `tests/unit/`
(obligation 3). The cycle and placement suites needed no edit and passed
(obligation 4), as did the mutation suites (obligation 5) and ASan+UBSan —
2 613 passed, 2 skipped, no sanitizer finding (obligation 6). The
implementation was, as far as every correctness instrument can tell, right.

**And it lost.** Two arms, each a full `make pgo` under the shipped recipe with
its **own regenerated profile** (both gated: 2 613 pytest and C++ 15/15 in both
phases of each), trained on **identical test suites** so E26-P7b's
test-addition shift could not confound them, with a second copy of the base
arm in-rotation as the control. Two draws, rotation order reversed:

| row | A/A (draw 1 / 2) | hand-off (draw 1 / 2) | verdict |
| ---------------- | ------- | ------- | ------- |
| `scalars-only` | +0.13% / +0.13% | −0.65% / −0.59% | the only gain |
| `value-dict0` | −0.04% / +0.14% | +3.15% / +3.57% | loss |
| `mixed` | +0.00% / +0.45% | +1.35% / +0.90% | loss |
| `flat` | −0.08% / +0.96% | +0.54% / +0.34% | at the floor |
| `users` | +0.02% / +0.03% | +1.22% / +1.32% | loss |
| `nested` | +0.06% / +0.05% | +3.11% / +3.08% | loss |
| `wide_arrays` | −0.00% / −0.00% | +0.05% / +0.15% | neutral |

Kill criterion 1 required a gain past the A/A floor on each of `mixed`,
`flat`, `users` and `nested`. Three of the four resolve a **loss** instead, at
2× to 60× their control, reproducibly and with the rotation reversed.
Criterion 2 is moot: `wide_arrays`'s +3.18% in the held-profile screen did not
reproduce here (+0.05%/+0.15%), so that figure was an artefact of the bound
arm, not a cost of this design.

**Why, and this is the part worth keeping.** Two things, and the first is a
lesson about the instrument rather than about the code.

1. **The held-profile screen measured the wrong quantity.** It priced *deleting*
   the pair from a base built against a profile, and read −4.98% on `mixed`.
   But with each arm's profile regenerated, PGO lays base's RAII cleanup paths
   out cold on the strength of the profile saying they are never taken — so the
   pair costs a *profiled* base far less than deleting it from one suggests.
   "What the pair costs when removed" and "what a design that relocates it can
   recover" are different numbers, and the held-profile method cannot tell them
   apart. The asymmetry noted in E26-P29 — that the held profile biases against
   the bound arms — was real but pointed the wrong way: the larger effect was
   that the bound deleted work the profile had already made nearly free.
2. **The population is wrong.** The design pays a call for every record that
   contains a container and benefits only records that contain none. In the
   real datasets the first set is the large one: `nested` hands off on
   essentially every record and loses 3.1%, `value-dict0` always hands off and
   loses 3.2–3.6%, and the only row that gains is `scalars-only`, the synthetic
   shape with no container anywhere. `flat`'s 21 plain scalars should have been
   the design's best real row and it sits at its floor.

**What this closes.** The hand-off is refused as specified. So is any variant
that pays per container-carrying record to spare all-plain records — that is
the shape, and the population argument above kills the shape, not this
instance. Together with E26-P29's two refuted all-plain dispatch arms, the
`DeferredOpen`/`RowLock` cost is now established as **not recoverable by
relocation**: three designs, three refusals, and the third one correct by
every instrument. A future attempt needs a mechanism that makes the pair
cheaper *where it is*, or evidence that the prize exists under a regenerated
profile at all — which, on these numbers, is the claim that should be tested
first and was not.

Evidence: `build/evidence/benchmark-lead/p29/` (the differential, its two
identical output files, the held profile and its README) and E26-P30 in
[the ledger](../performance/experiment-ledger.md).
