# Decision record: the fused record writer (M11)

Opened 2026-08-16. Status: **in progress**.

## Problem, with its evidence file

`dumps` of small multi-schema record documents (`mixed.json`: 500 records,
4 rotating schemas, 34 KB) ranks #2 behind orjson on the x86 CI legs with a
seventeen-plus-sample median of ~1.07–1.09x, while ranking #1 on
macos-arm64 and measuring **parity in isolation on the very same x86
runners** (docs/decisions.md, 2026-08-15/16). The row has been certified
against five hypotheses: schema-cache layout and GC-traversal warm-up
asymmetry (both fixed, both helped), L1I text splitting, instruction-width
(AVX2), and harness call order (all measured null). What remains is not a
single sink a profiler names but the *sum of per-record machinery* under
the harness's cold-state condition — `write_mapping` carries 34–45% of
self time across every profile taken.

## Decision

Add a **one-pass emit path for the array-of-records shape** — the shape
every certified row is made of — replacing the current two-pass
collect-then-emit per record:

- Today: `write_mapping` walks the dict once into `keys[24]`/`values[24]`
  staging arrays (classify, width check), selects the schema way, then a
  second loop emits prepared keys and dispatches each staged value.

- Fused: for a list whose elements are exact dicts, a record loop walks the
  rawdict entry array **once**, resolving the schema way from the first
  key, emitting `"key":` from the inline slot row and dispatching each
  value as it is visited. No second walk, and no staging of the *keys* at
  all.

  The one staging row that remains is the row of value pointers: the
  verification pass loads every `me_value` anyway (to prove the slot is
  occupied), and it keeps them so the emit loop never reads the entry array
  again. It cannot: user code running under a value — an `int` subclass's
  `__str__`, a large exact `int`'s decimal conversion on CPython 3.12+, a
  cycle warning — can resize the dict and free that table
  (E26-FIX1, 2026-09-06). The row is a copy of pointers the one pass already
  has in hand, not a second read of the dict, and holding it is what lets the
  fused writer emit the same bytes as the general path under mutation.

  Since E26-P23 (2026-09-12) the entry array the pass walks is not always the
  dict's own. A `DICT_KEYS_GENERAL` table — 24-byte `{hash, key, value}`
  entries, which is what `_PyDict_NewPresized` and therefore every record
  `strata.loads` builds above five keys has — is **compacted** into the
  16-byte `{key, value}` shape first, by one `cold`, out-of-line pass into the
  lease's single 25-entry scratch, on the branch that used to return nullptr
  and send the record to `write_mapping`. There is deliberately no second
  instantiation of this body and no runtime stride or layout flag: the ledger
  already priced both (a second instantiation trains cold under the
  gate-inclusive profile, `dumps mixed` +5–18%; a flag live across the
  verification loop is `dumps flat` +2.2–3.0%), so the only shape left is one
  that makes the two layouts identical *before* the loop starts. What the
  per-key loops cost, per ISA rather than in general (E26-P23 in
  docs/performance/experiment-ledger.md has the method and the counts): on
  arm64 the verification loop, the emit loop and `write_mapping`'s collection
  loop are reproduced instruction for instruction modulo register renaming; on
  x86-64 they keep their sequences apart from frame-slot renumbering, and the
  collection loop comes out one memory operand *better* than before, with
  `_PyBool_Type` hoisted into a register. The unicode path loads nothing new on
  either ISA — the scratch is found by the cold callee off `this`, never passed
  in — and the compacted array obeys the same rule as the raw one: dead the
  moment the staged row is filled, now for a second reason as well, since a
  nested record's compaction overwrites the one scratch.

  That row is **leased, not a local array** — one per dict nesting level, in
  the same per-thread state as the schemas it serves
  (`SchemaCacheLease::StagedRow`), and shared with `write_mapping`'s
  `keys`/`values` staging for the same level. A local array here is a local
  array in every function this body is inlined into, and the profile inlines
  it into the per-value dispatcher: on `-fprofile-use` a 192-byte row in
  `write` and `write_sequence` is a 448-byte frame and a stack-protector
  canary paid by every value of every document, arrays of scalars included
  (build/evidence/E26-P2/BUILDS.md). Leasing it is what lets this body stay
  inlinable — which is also what keeps the canary from moving from once per
  array to once per record on the legs that default to
  `-fstack-protector-strong` (E26-FIX1 v3).

The general path stays untouched as the single definition of behavior and
the fallback at every deviation: non-`str` key, width past `kMaxSchemaKeys`,
split-table dict, way miss mid-record, subclass values, retired depth.
A record that falls back re-enters `write_mapping` from its start (the
fused loop commits nothing to the output before the fallback decision for
that record).

## Invariants that must survive, byte for byte

- Output identity with the general path on every input (round-trip oracle
  suites; stdlib `json` as the semantic oracle).
- Error contract: `TypeError` for non-`str` keys and unsupported types with
  the pinned messages; depth `ValueError` at the same boundary.
- Cycle policy: record dicts holding containers get the same deferred-frame
  semantics (`open_` scan) as today; the record array's own frame is
  unchanged. Frame elision for all-scalar records must not change what a
  cycle warning observes.
- Schema-cache behavior (4-way select, remember, retire, inline slots) is
  shared state with the general path, not duplicated.
- **A record's opening brace is written by its first key** (E26-P24): the
  prepared-slot emit of both dict writers stores `{` where a later key stores
  `,`, inside the same 17-byte reservation, so neither writer reserves or
  stores the brace on its own. Two consequences are load-bearing. A record of
  **zero** keys has no first key and therefore cannot take that loop — the
  fused writer refuses `size == 0` before it reads `entries[0]`, and
  `write_mapping` writes `{}` itself; a width of at least one is now a
  precondition of `write_mapping_body`, not only of `DepthSchemas::select`, and
  each emit loop asserts it — stripped under `NDEBUG`, so the precondition costs
  release codegen nothing and every debug and sanitizer build checks it on every
  record. A caller that broke it would emit `}` with no `{`: invalid JSON,
  silently, which is the failure this file already records once.
  And the two branches the slot loop does *not* cover — a span too wide for an
  inline slot, and a shape whose bytes are not prepared yet — keep an
  `ensure`/`put` pair of their own. Both writers emit through the single
  `emit_slot_key`, which is what keeps them byte-identical.

## Falsifiable estimate and kill criterion

Eliminating the staging writes/reads (two pointer arrays per record) and
the second walk targets the two-pass overhead visible in every profile.
Estimate: −10..15% on record-shaped dumps under the gc-per-call in-process
instrument. **Kill criterion:** \< 3% improvement on `dumps mixed` under
that instrument on py3.12, or any regression on users/flat/nested/wide —
then this record is closed as a negative result like its five predecessors
(the tiny-dict bypass and slot-kind prediction failures bound the risk:
both died from code-size effects, so the fused loop must reuse the existing
scalar writers rather than inline new dispatch machinery).

## Acceptance (mirrors the M11 roadmap entry)

`dumps mixed` #1 in the majority of ≥ 4 same-code CI samples on
linux-x86_64 and macos-x86_64; no row regresses on any leg; both suites
green; byte-identity pinned.

## September 8 experiment: reuse for nested exact dictionaries

Status: prototype, not accepted. The native ARM64 PGO profile from run
34146265191 attributes 8.48% of hot dumps samples to `write_mapping_body`;
exact dictionaries reached through `Serializer::write` still enter the
general writer even when their schema is prepared. Test routing that exact
`dict` dispatch to the existing out-of-line `write_record_fused`, preserving
its complete validation and general fallback. List dispatch, subclass
handling, cache retirement, staging ownership and depth/cycle semantics must
remain unchanged. No new inlining, scalar-kind cache or per-record storage.
Compare mixed plus flat/users/nested/wide and root-dict mutation/depth
controls. Reject unless gains clear the paired noise floor without a
canonical regression; this does not revive rejected broad footprint changes.

Review outcome (2026-09-10): as written the experiment changes cycle
output. The fused writer had no probe of `open_`: the sequence loop probes
the list it walks, not the record it dispatches, and `write_mapping` carries
its own `Frame::repeated()`. So a dict already open above was emitted once
more before the placeholder whenever its shape was prepared at the re-entry
depth: on the unchanged tree for a repeated dict reached as a list element,
and with P9 for dict values too (five of seven shapes;
`build/evidence/benchmark-lead/p9/cycle-defect/`).

Resolution (2026-09-11): the dispatch enters through
`write_record_fused_value`, which probes `open_` before the row and hands a
hit to `write_mapping`, so the value path's bytes are the general writer's
by construction. The element loop's records are not probed, as before. Three
ways of probing them were measured and declined (the ledger's E26-P9 revival
entry): the unconditional scan costs `dumps users` about 2% (its item records
sit five containers deep); deciding the first container in the verification
pass, so the emit loop could arm by index, costs `dumps flat` 8–20% depending
on the runner (20% on the EPYC, 8% on the i7), six of six blocks past floor on
every leg; a probe at the first container value with the record's bytes taken
back through a mark on the staged output costs users 4%. Every one of them
is per-record work on records of three fields, where a handful of
instructions is a percent. The element-loop gap is recorded in
docs/decisions.md (2026-09-11). The probe's place ahead of the fallbacks was itself re-measured (the ledger's probe-placement follow-up, 2026-09-11): behind them, as a second instantiation the value-path copy trains cold under the gate-inclusive profile (`dumps mixed` +5–18%, file `dump nested` +35% on the M1), and as a runtime flag the flag is a spill through every record's verification loop (`dumps flat` +2.2–3.0%, M1, `local4_A.tsv`; the ledger's E26-P9 probe-placement follow-up — the +3.5% once quoted here was the EPYC figure from the *second-instantiation* arm, a different experiment); the one scan more on a rejected value dict is the cheaper side.

## E26-P9a: restrict the nested dispatch experiment to Linux ARM64

The full native P9 comparison (34253218374) reaches 27/27 on Linux ARM64,
with mixed serialization median -2.923%, but still fails two p95 metrics.
Other platforms show broader gate failures and Windows retains two misses.
The next isolated patch therefore selects nested fusion only under
`defined(__linux__) && defined(__aarch64__)`. It preserves the original
exact-dict dispatch elsewhere. No schema state, allocation policy, ownership
rule, or public behavior changes beyond the already-tested P9 dispatch.

The platform restriction prevents applying an unqualified optimization to the
other targets; it is not a fix for the Linux p95 failures. Keep both arms'
mutation-test sources matched and retain the full canonical gate on every
platform. Original and narrowed preprocessed Darwin translation units are
byte-identical. The five-platform predicate check selects fusion only on Linux
ARM64. Native validation of the narrowed patch is still required before
production integration.

## P14 experiment: one reservation for a cached key and compact integer

The isolated trial reserves `17 + kInt64BufferSize` bytes when a staged value
is an exact compact integer on CPython 3.12+. The existing 16-byte key-slot
copy (plus optional comma) precedes direct integer formatting inside that
reservation. Other values retain the existing emission and deferred ownership
path. CPython versions without the compact-value API retain that path too.

Only exact compact integers take the shortcut: bools, subclasses and large
integers do not. Compact conversion and formatting cannot run Python, so the
shortcut needs no additional references and cannot invalidate schema or row
storage. The entire key row is still verified before output. If an earlier
value ran a callback, the existing row lock owns the remaining staged values.
The reservation covers both the key's scratch store and the integer writer's
maximum store window; failure propagates through existing allocation handling.
The experiment may lose through extra type checks or register pressure and
must pass matched PGO performance gates before integration.

## P15 experiment: one reservation for a cached key and short ASCII value

This independent prototype starts from production, without P14. Exact compact
ASCII strings whose combined key and string scratch window fits the existing
4096-byte reservation cap reserve once before the key copy. Their value uses
the existing copy-until-escape primitive; a clean copy commits the quotes and
payload. An escape hit rolls back the opening quote and uses the original
string writer. Longer strings, Unicode and subclasses use the original path.
No speculative output is exposed, and the cached key remains committed once.

The combined reservation covers 17 key bytes plus the value length, two quotes
and 16 scratch bytes. Direct header access is restricted to exact compact
ASCII values. Neither their byte access nor copying calls Python. Complete
key verification, schema re-indexing and ownership of values after callbacks
are unchanged. The additional branch, code footprint and repeated scan on an
escape hit are explicit regression risks. This is a bounded emission trial,
not a revival of cached scalar-kind prediction; full performance gates remain
required before adoption.

P16 tests the same reservation through the existing string writer instead of
duplicating its clean-copy arm. A private `pre_reserved` argument suppresses
only that writer's initial capacity check; all escaping, spanning and failure
handling stays shared. Its sole true caller is the fused exact-ASCII branch
with the combined reservation proved above. All other calls retain the default
false argument. This tests P15's duplication cost with identical boundary
tests, not a change to string semantics or a claim that P15's loss is explained.
