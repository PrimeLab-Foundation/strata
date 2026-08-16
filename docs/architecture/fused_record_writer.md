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
  value as it is visited. No staging arrays, no second walk.

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
