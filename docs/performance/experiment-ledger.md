# Experiment ledger

## Published September 7 verification: `75cfb42`

Two complete same-revision benchmark runs, 34143749167 and 34143751498,
passed PGO and supportability on all five platforms and scored **128/135**
and **133/135**. Linux ARM64 and Windows `dumps mixed` missed in both runs;
the other misses varied. There are no rounded ties. `make bench-ci` refreshed
the tracked reports, raw companions, and `ci_summary.md` from the second run.
Both whole artifact sets remain archived. This completes the two-run evidence
collection, but does not satisfy 135/135 or resolve the local failed regression
gate. Details and exact per-platform counts are in
[the execution record](plan-execution-2026-09-07.md#published-revision-follow-up).

## September 7 plan continuation on `work/september-7-plan`

Baseline: `3f45c9c`, the current main integration. The three pre-existing
untracked review/plan documents remain owned by their author. T0 reconciliation
found T1/T2 and the A/B portion of T3 already integrated, as were the accepted
E26-P6/P7 changes and their Windows check. No rejected optimization was rerun.

The canonical-provenance portion of T3 was still absent despite the earlier
"T1–T3 delivered" shorthand. This continuation adds raw report companions,
build identities, profile-input manifests, validated full-precision gating,
and transactional companion transport. It also supplies T7 real-file phase
controls and T8's separate supplementary scope and Python-version endpoints.
Independent review required preserving incremental-build attribution, rejecting
missing companions, checking binary/source consistency, requiring folder-loop
coverage, and accurately labeling diagnostic timing protocols; those changes
are included.

Initial `make test` exposed a stale installed extension: 23 private-cache
tests failed while the current source already contained the fix. A gated
rebuild restored the current source; the subsequent full run passed 15 C++
suites and 2,232 Python tests before the final additional diagnostic tests.
The standalone 100-call reproduction now has zero key-reference growth in
both output modes. Evidence and final validation are recorded in
`build/evidence/september-7-plan/` and the execution handoff document.

Three six-block, 60-sample serial campaigns completed: baseline/candidate,
identical-binary A/A, and two fresh candidate builds with matching training
source/data. Every effect remains below its row's A/A floor. The six real
parse/file-load rows show baseline/candidate normalized effects from -0.24%
to +0.28%; this local T5 experiment is inconclusive and does not close E26-P8.

The subsequent complete canonical baseline/candidate comparison matched all
81 rows and FAILED every tier: 19/30/17 metric threshold breaches for
small/medium/large. Candidate standings were 26/27, 27/27, 27/27 against a
27/27 baseline at each tier; the separate 3.0x gates all passed. Small file
dump mixed median/p95 increased 23.4%/133.3%, illustrating why standings
cannot waive the failed regression gate. All contradictory evidence remains
under `build/evidence/september-7-plan/`; the published baseline was untouched.

No additional runtime or PGO-recipe optimization is accepted here. Historical
E26-P8, T6's native N2/Windows residual rows, and deciding T7 native file
attribution remain unresolved. T9 requires quiet-machine resolution of the
failed comparison, publication of the final source, and two complete
five-platform runs; historical 132/135 and 128/135 runs are not standings for
this branch. See [the execution record](plan-execution-2026-09-07.md).

The campaign ledger required by `fable-5.1-opus-5-improvement-plan.md` §6:
one entry per experiment, opened before it starts and closed with one
outcome — accepted, rejected or inconclusive. Raw evidence lives under
`build/evidence/<id>/` (gitignored build tree) and, for runner
measurements, in the profile job logs named here; the curated numbers go
to `docs/decisions.md` and `docs/performance/SKILL.md`.

## E26-P0 — trustworthy two-microsecond comparison

- Opened 2026-09-06 · owner: Opus measurement · reviewers: lead, Opus reviewer
- Parent revision: 90dd3b7 (production tree identical to 32c5fa4, the measured
  revision: no diff in `include/`, `src/`, `python/`, `setup.py`, the PGO
  scripts or `CMakeLists.txt`)
- Hypothesis: the linux-arm64 `dumps mixed` deficit (0.067 vs 0.065 ms on runs
  34012087358 and 34012082501) is a fixed per-call cost after GC or other
  work; the existing diagnostics (`dumps_loop.py`'s subtracted `none` arm,
  `decompose_dumps_records.py`'s padded `uniform` control and its
  strata-then-orjson ordering, cachegrind on an `armv8.2-a` rebuild) are not
  yet controls a two-microsecond claim can rest on
- Deciding workloads: small `mixed.json` `dumps` hot, GC-only, GC+sweep and the
  canonical five-engine interleave, kept separate; A/A of two fresh PGO builds
  of unchanged source; serial A-B-B-A rounds with a repeated baseline
- Expected cost: none in production code (diagnostics only)
- Rejection condition: P0 is not "rejected"; it completes when each measured
  binary is identified, the target and its control variance reproduce, and
  valid evidence is distinguished from inconclusive evidence
- Outcome: **accepted 2026-09-06 with two named gaps** (packet:
  `build/evidence/E26-P0/PACKET.md`, branch `exp/p0-controls` 0fed2e4).
  Findings the campaign now rests on: the PGO pipeline is deterministic on the
  M1 (two `make pgo` builds byte-identical in `__text`); the A/A floor with 59
  launches is 0.87% on `dumps mixed` bytes and a single launch pair resolves
  nothing; on the N2 the same-code between-run spreads are ISOLATED 2.8 pp,
  INTERLEAVED 6.2 pp, COLD 22.7 pp against a 3.1 pp deficit. Two wave-26 legs
  do not survive corrected controls: the cachegrind mechanism was simulated
  on a plain `-O3` rebuild (on a PGO+LTO simulated build strata's own I1
  misses per cold call are 217 against orjson's 119, not 1,941), and "behind
  after GC" was the ISOLATED probe's all-strata-then-all-orjson ordering
  (measured directly with the preamble outside the timer, per process:
  0.995x). What survives: the hot lead, empty dicts 3.4x, short inner lists
  1.35x, the integer-width buckets. The row now reads as a shared-process
  effect (behind only when a rival runs between the calls in the same
  process). Gaps: the quiet-machine baseline check (still owed) and the N2 `gc-pair`
  rung — closed the same day by profile run 34028757638 on 0fed2e4: whole
  document, order-balanced, both engines in one process, preamble outside the
  timer, three draws of 300 pairs: 1.014x \[1.009, 1.020\], 1.016x \[1.011,
  1.024\], 1.004x \[0.999, 1.008\]. The ladder on the N2 now reads: alone under
  GC 0.995x; one rival alternating 1.004–1.016x; five engines interleaved
  1.03–1.09x. The deficit grows with the number of rivals that run between
  strata's calls — the signature of a working set re-fetched after eviction
  rather than of per-call work.
  Deciding instruments for P1/P3: `dumps_loop.py pair N gc-pair` on the N2,
  `ab_builds.py` A-B-B-A ≥24 launches with the `ab_floor.py` A/A floor beside
  every effect, cachegrind only on a PGO+LTO build.

## E26-P1 — cycle-stack allocation as part of the fixed cost

- Opened 2026-09-06 · owner: lead · reviewer: Opus reviewer · timing
  reproduction: Opus measurement
- Parent revision: 90dd3b7
- Hypothesis: `Serializer::open_` (`std::vector<PyObject*>`) is constructed
  per `dumps` call and grown on the first container push, so ordinary
  shallow documents execute allocation/growth code on every call; an inline
  prefix with a growing overflow would remove that work without a
  per-element tax
- Source: `src/strata/bindings/python_dumps.cpp` (`open_`, `Frame`,
  `write_sequence`, `write_record_fused`); `map_depth_` stays independent of
  the cycle stack (2026-08-10 decision)
- Deciding workloads: allocation/growth counts and maximum frame depth on
  `mixed`, `users` and controls; then the P0 comparison set
- Expected cost: a few hundred bytes of stack per call; no per-element work
- Rejection condition: allocation/growth events are absent or negligible in
  the target's fixed overhead, or the prototype adds latency or stack growth
  beyond its inline capacity's justification
- Attribution 2026-09-06 (profile runs 34028923065 and 34031501916 on
  `exp/p1-attribution`, callgrind on the arm64 leg, the 200-call run minus the
  0-call run, per caller): one `dumps` of small mixed.json makes three
  `operator new` / `operator delete` pairs through glibc — one from
  `std::vector<PyObject*>::push_back` (the cycle stack's first growth) and two
  from the bindings' own construction — plus the output block's pymalloc
  round trip; users.json four pairs. orjson makes none through `operator new` and two `realloc`s of its buffer. Three heap pairs cost of the order
  of 150 ns on this core, about 0.3% of a 50 µs call — below the instrument's
  A/A floor (0.87%) and below the row's own deficit; and the corrected
  GC-alone reading (0.995x) says the per-call fixed cost is already at
  parity.
- Outcome: **rejected 2026-09-06** — allocation and growth work is present
  but negligible in the target's fixed overhead; no prototype.

## E26-P2 — the M1 `dumps flat` PGO anomaly

- Opened 2026-09-06 · owner: Opus build · reviewers: lead, Opus reviewer
- Parent revision: 90dd3b7
- Hypothesis: the wave-26 record of a `make pgo` build ~13% slower than a
  plain `-O3` build on small/medium `dumps flat` (220.6 / 889.6 µs against
  ~195 / ~787) is one of: a stale artifact surviving a header or flag change,
  a profile/corpus difference between the two PGO builds compared, an actual
  compiler layout effect of `-fprofile-use -flto` on the flat-record path, or
  machine drift in that session
- Source: `setup.py`, `scripts/pgo_build.sh`, `scripts/pgo_training*.py`,
  `scripts/pgo_build_clang_cl.py`; the serializer functions the flat row
  exercises, compared between the plain and PGO builds by disassembly
- Deciding workloads: fresh plain and PGO builds of unchanged source, two
  repetitions of the PGO build, controlled compiler/Python/CPU/dataset
  order/profile inputs; `dumps flat` small/medium in an A/A-calibrated
  drift-free comparison
- Expected cost: none unless a reproduced cause authorises a build change
- Rejection condition: not reproduced on unchanged source with fresh builds
  → recorded as unresolved, and no PGO baseline from that session may be
  used to accept P1 or P3
- Inspection stage closed 2026-09-06 (`build/evidence/E26-P2/INSPECTION.md`):
  the shipped profile merges three raw profiles — two gate `pytest` runs and
  the training workload — because `scripts/pgo_build.sh` exports
  `LLVM_PROFILE_FILE` before the phase-1 install's post-build gate; the
  deciding wave-26 PGO A/B compared against a non-reproducible main binary
  (a different profile, 16 symbols differing, parser code included); the
  plain→PGO step changes `-flto=thin` and `-fprofile-use` together and they
  have never been separated; `build_ext --inplace` sites in profile.yml lack
  `--force` (latent, not triggered); the training corpus never produces a
  scalar-only record wider than three keys; the reports' compiler_flags line
  is built from environment variables. No build change authorised yet.
- Build stage closed 2026-09-06 (`build/evidence/E26-P2/BUILDS.md`, nine
  arms): every A/A pair byte-identical in `__text` (P0 confirmed); the plain
  → LTO step moves 0.13% of the text, the plain → `-fprofile-use` step
  40.9% — the profile, not ThinLTO, transforms the serializer; the gate
  pytest runs contribute 47.5% of all merged counts (99.7% of
  `dumps_to_python`'s, 64.5% of `write_sequence`'s) and removing them moves
  122 symbols / 46,400 bytes, `parse_array` −7,524, the serializer's flat-row
  functions +30% larger; the training-only recipe is deterministic. New
  mechanism: every `-fprofile-use` build loads `___stack_chk_guard` in
  `Serializer::write` (6 sites) and `write_sequence` (4 sites) — none in the
  plain or LTO-only builds — with frames 80 → 448 and 176 → 480 bytes,
  because the profile inlines the dict writer's `keys[24]`/`values[24]` rows
  into the per-value dispatcher and Ubuntu's default stack protector then
  guards it; paid on every `write` call (1,375 per mixed.json document). The
  `--force` gap is live on this host (a second `build_ext --inplace` with a
  different `STRATA_MARCH` recompiles nothing and still prints both gates
  green). No build change applied; the timing window decides.
- Timing window closed 2026-09-06 (`build/evidence/E26-P2/TIMING.md`, 12
  A-B-B-A runs of 33 launches, 362,340 samples; the user's PyCharm at
  500–600% CPU throughout, so the orjson-normalised estimator carried the
  verdicts — A/A floors 0.6–1.5% normalised, 9.5–12.4% raw). The anomaly
  reproduces and is larger than recorded: shipped PGO+LTO against plain `-O3`
  on `dumps flat` +17.0% small / +18.1% medium (bytes), both draws within
  1.7 pp, while the same binary is *faster* than plain on `dumps nested`
  −8.0%, `loads mixed` −8.0%, `loads users` −6.5%, `loads flat` −3%. It
  decomposes: ThinLTO alone +7.9% (an 8% regression behind a −0.13% text
  change), the gate-test profile ~+9 pp; clean-profile PGO costs nothing
  beyond ThinLTO. `-fprofile-use` without LTO does not explain it (+2.1%
  over shipped). Training-only profile against shipped: `dumps flat` −9.4%
  small / −8.8% medium, `loads flat` −1.0% (favourable, inside the floor),
  neutral on `dumps nested`/`wide_arrays`, `loads mixed`/`users`; adverse
  inside the floor: `dumps users` +1.9%, medium `dumps mixed` +0.8%. Every
  arm remains ahead of orjson on every row. Nothing here is the N2.
- Lead's decision: the profile separation is authorised on hygiene grounds
  (the profile becomes what the documentation says it is) and supported on
  speed for the M1's `dumps flat`; it is a production build change on every
  POSIX binary and enters P5 with the full gate and two five-platform
  samples; the two adverse signals are carried as open until the N2 and the
  official harness read them. ThinLTO stays (it is part of the loads gains).
- Build patch authored 2026-09-06 (`exp/e26-p2-profile` 0747d73,
  `build/evidence/E26-P2/PATCH.md`): `scripts/pgo_build.sh` diverts the
  phase-1 install's gate and `gate_tests()` to a junk profile directory and
  runs the training workload alone into the raw directory, with a guard
  requiring exactly one raw profile; `scripts/pgo_build_clang_cl.py` sets
  aside any profile that lands outside the raw directory; `scripts/pgo_build_msvc.py`
  separates by order (the gate's `.pgc` files moved out before training,
  since `/GENPROFILE` bakes the path in); `docs/build-and-test/SKILL.md`
  states what the profile is. Two `make pgo` runs each merge one raw profile
  with two diverted, both gates green, and the extension reproduces the
  timing window's T arm byte-for-byte in `__text` (0 symbol differences
  against `builds/T1.so`) and T3's profile bit-for-bit from a different
  worktree — deterministic under the patch. Corpus, seed and flags
  untouched. Conditions: the Windows clang-cl leg must show "merging 1 raw
  profile" in CI; the MSVC path is exercised by no workflow (kept for
  symmetry, unproven); the gcc branch's `GCOV_PREFIX` nesting is a
  pre-existing question; the two adverse-inconclusive rows stay open and
  the change is not the deficit-closing one. Independent review
  (`build/evidence/PGO-REVIEW/REVIEW.md`): not refuted — scope, flags,
  corpus and seed confirmed untouched, the recipe reproduced — with one hold
  the lead upholds: the training-only profile changes `parse_array`'s
  generated code most of all (an 8,000-byte fully inlined loop becomes 476
  bytes calling out per element), and nothing measured so far reads the
  `loads`/`load wide_arrays` rows on either machine under the new profile.
  Those rows are the value cursor's gains and must be read before this
  lands: the runner half is profile run on `exp/e26-p2-profile` (the arm64
  job builds with the patched `make pgo`), the M1 half a predeclared G1 vs
  T1 A-B-B-A on `loads`/`load wide_arrays` in the next window.
- Runner half read 2026-09-06 (profile run 34044733475 on
  `exp/e26-p2-profile`; the arm64 job's `make pgo` logged "merging 1 raw
  profile (the training workload)"): 400 `loads` of wide_arrays 19.03 G
  instructions / 1.587 s against the shipped profile's 19.00 G / 1.596 s
  (run 34012088721) — unchanged inside the band; `loads mixed` 0.898x
  isolated / 0.884x interleaved against 0.898x / 0.892x; `dumps mixed`
  1.018x isolated / 1.026x interleaved against 1.031x / 1.026x. The
  per-element probe's cheapest classes moved the wrong way (nulls 4.71 →
  5.37 ns, bools 5.32 → 8.62) without any trace in the whole-document loop
  that contains 32,000 of each — the probe's single-class lists are
  layout-sensitive in a way the documents are not, as the wave-25 cap draws
  had already shown. One draw. The M1 half (G1 vs T1 on `loads`/`load wide_arrays`, A-B-B-A with the A/A floor) is owed in the next window.
- Outcome: open (M1 parse-row measurement, then P5)
- - Superseded 2026-09-07 (E26-P5b): on the fixed source the training-only
    profile reads +3.3–6.3% on the three serializer rows measured (`dumps flat`,
    `users`, `mixed`) and +1.5–1.7% on `loads wide_arrays` against the shipped
    recipe; the recipe change is withdrawn from the candidate and kept on
    `exp/e26-p2-profile`.

## E26-AUDIT — the serializer's cycle/depth/reentrancy contract

- Opened and closed 2026-09-06 · owner: Opus reviewer · evidence
  `build/evidence/E26-AUDIT/AUDIT.md`, tests on `exp/audit-tests` 535a0c6
- Outcome: **accepted for integration** (tests only, +24 test functions; the
  block is moved into `tests/unit/test_dumps_contract.py` by the lead to keep
  the ~800-LOC rule). Mutation testing showed the suite at 90dd3b7 accepted an
  off-by-one depth boundary and a dropped fused-writer cycle frame. Two
  pre-existing defects opened as their own items below.

## E26-FIX1 — borrowed pointers held across re-entrant Python (memory safety)

- Opened 2026-09-06 · owner: Opus worker · reviewer: a different Opus worker
- Parent revision: 90dd3b7
- Defect: `write_sequence` captures `PySequence_Fast_ITEMS`/`GET_SIZE` once and
  runs arbitrary Python inside the loop (a cycle warning under the default
  policy, `__str__` of an int subclass beyond int64); a callback that mutates
  the list leaves the pointer dangling (`repro/iso2.py`: SIGSEGV on the
  shipped extension; stdlib `json` handles the input). The raw dict walk has
  the same class of exposure (`repro/iso3.py`).
- Hypothesis: re-reading size and item pointer after every call that can run
  Python, and re-validating the dict layout, removes the access with no cost
  on the scalar-run path
- Rejection condition: any remaining freed-memory read under ASan, a changed
  contract clause, or new loads per scalar element
- Implemented 2026-09-06 on `exp/fix1-reentrant-mutation` 07f8e21 (author:
  Opus worker; `build/evidence/E26-FIX1/FIX.md`): ownership (a container
  holds a strong reference to itself while user code can run beneath it,
  via the deferred push or `Frame`) and freshness (no borrowed pointer into
  a container survives a user-code step); lists and tuples followed live as
  stdlib `json` does, dicts of at most 24 exact-`str` keys emitted as the row
  read on entry (the only rule both the general and the fused writer can
  produce byte-identically), wider or subclass-keyed dicts followed live; 90
  contract cases (12 SIGSEGV, 32 failures on the unfixed build); ASan gate
  clean. Independent review (`build/evidence/E26-FIX1-REVIEW/REVIEW.md`, 179
  adversarial cases per build, sanitised): **not refuted**, three required
  changes — the `str`-subclass-key routing is a fourth demonstrated
  use-after-free (`SchemaCacheLease::Schema::remember` releasing an evicted
  key whose `__del__` empties a staged row), not a defensive extra; the
  mutation contract must be written into api.md, decisions.md and
  fused_record_writer.md; and the cost table must carry `write_sequence`
  (+102 instructions, frame 176 → 416 B, a stack-protector canary from the
  inlined 192-byte row) and the corrected per-element composition. The
  review also found a pre-existing defect, opened as E26-FIX2.
- Lead's decisions: (1) the ownership rule is right but its cost is per
  container that holds a container (1,502 refcount operations per
  mixed.json call, 61,786 per users.json) — the revision must own the open
  chain only around the two user-code sites (`PyErr_WarnEx`, `PyObject_Str`
  on a non-exact int), which is zero cost on ordinary documents, or measure
  the per-container form and justify it; (2) `write_record_fused` keeps its
  row out of `write_sequence`'s frame (`STRATA_NOINLINE` or an out-of-line
  row), the same mechanism E26-P2 found PGO applying to `write`; (3) the
  documentation goes in with the code.
- Revision v2 2026-09-06 (`exp/fix1-reentrant-mutation-v2` 40fcedf,
  `build/evidence/E26-FIX1/FIX-v2.md`): ownership latched at the user-code
  sites and held until the frame or row pops (the literal incref/run/decref
  form frees an open container the walk still re-reads — the author
  demonstrated it); zero reference-count operations per call on every
  benchmark dataset (was 2 / 1,502 / 13,002 / 61,786 / 4,002);
  `write_record_fused` out of line, so `write_sequence` reads 544
  instructions, a 144-byte frame and no canary (766/176/0 at 90dd3b7,
  868/416/7 at v1); behaviour byte-identical to v1 on every corpus.
  Second review (`build/evidence/FIX1-REVIEW/REVIEW.md`, 2 of the
  workflow): **refuted** on one finding — the raw-dict layout probe runs
  once per process on first use *inside* the walk and makes a GC-tracked
  allocation, so a collection can fire `__del__` on a cold process before
  any latch exists (the enumeration "user code runs at exactly three steps"
  is false as written; reproducer in `finding_probe_gc.txt`); plus the
  cost table omits that the canary moved from once per array to once per
  record (`write_record_fused` 336 instructions / 368-byte frame / 7 canary
  references), which on the Linux legs' `-fstack-protector-strong` is paid
  on both paths, and the row store and armed-flag load in the fused loop.
- Lead's decision: v3 resolves the layout probe before the serializer is
  constructed (module init or the first statement of `dumps_to_python`),
  adds the cold-process contract test, states the per-record canary and the
  fused loop's per-element loads in the cost table, and is re-reviewed by a
  different worker; the decisions entry is the lead's (written). Storage
  decision, superseding "`STRATA_NOINLINE` on `write_record_fused`": no stack
  array on the hot path — the staged key and value rows move into storage
  the serializer leases per depth beside the schemas they serve, bounded by
  `kMaxCachedDepth`, so no inlining decision of the profile can put a row
  into `write`'s or `write_sequence`'s frame. That is also E26-P3's
  experiment, so v3 carries its static evidence (canary references, frames,
  instruction counts under `-fprofile-use` and `-fstack-protector-strong`)
  and P3's timing follows on the integrated tree.
- Revision v3 2026-09-06 (`exp/fix1-reentrant-mutation-v3` 629e9c2,
  `build/evidence/E26-FIX1/FIX-v3.md`): the raw-dict layout probe resolved
  at module initialisation (`prepare_dumps_runtime()` from `PyInit__strata`);
  the staged key and value rows and the row-lock node moved into one leased
  `StagedRow` per nesting level in the schema lease, so under the
  training-only profile `write` keeps 90dd3b7's instruction stream (238
  instructions, 80-byte frame, no canary), `write_sequence` reads 639/128/0
  against 923/160/0, and with `-fstack-protector-strong` no function gains
  a canary site against 90dd3b7; the fused loop 12 loads / 3 stores per
  element (a mechanical count of 90dd3b7's gives 23/7). Third review
  (`build/evidence/FIX1-V3-REVIEW/REVIEW.md`): the mechanism confirmed
  (leased rows attacked with 16 more cases, re-entrancy across
  `kMaxCachedDepth`, four threads), **refuted on the shipped contract**:
  on CPython ≥ 3.12 an exact int above ~10,000 decimal digits is converted
  by the `_pylong` Python module — bytecode, imports and GC-tracked
  allocations — so with `sys.set_int_max_str_digits` raised a plain dict of
  plain scalars runs user code on the elided all-scalar path where nothing
  is latched (deterministic SIGSEGV, pre-existing at 90dd3b7; ASan
  heap-use-after-free from `gc_collect_main`); the profiled codegen tables
  were measured against a profile that was not archived (the archived
  profiles reproduce different figures: `write_mapping_body` +9
  instructions and +4.5% serializer text, not +172 and +14.2%).
- Lead's decision: close it in code (option b) — `is_plain_scalar` rejects
  a non-compact `PyLong` so a large int arms the container and registers
  the row, `write_int` latches before `PyObject_Str` for every non-compact
  int; the header and api.md enumerate every user-code site; the codegen
  tables are re-measured against an archived profile; a contract test from
  the reproducer; v4 by a worker, reviewed by a fourth independent worker.
- Revision v4 2026-09-06 (`exp/fix1-reentrant-mutation-v4` c4f7982,
  `build/evidence/E26-FIX1/v4/`): three changes, one more than the
  decision named — `is_plain_scalar` refuses a non-compact `PyLong`
  (`PyUnstable_Long_IsCompact` on ≥ 3.12, the digit count on 3.10/3.11;
  both bounds |v| \< 2^30), `write_int` latches unconditionally at the
  beyond-int64 arm, and it holds a strong reference to the int across
  `PyObject_Str`, because `long_to_decimal_string` imports `_pylong` before
  handing the value over and a collection there frees the int itself (4 of
  24 runs crashed without the guard). The reproducer exits 0 on 8/8 plain
  and 3/3 ASan runs (139 on v3); a load-bearing cold-start contract test;
  1,948 tests on 3.14, 3.11 and 3.10; every earlier transcript identical;
  zero latch, reference-count and guard operations on all 18 dataset
  tiers (no benchmark dataset contains a non-compact int). The codegen
  tables are re-measured against three archived profiles with the
  reproduction command; a finding on the way: every `clang -S` emission of
  this campaign omitted `-DNDEBUG`, which the shipping build defines, so an
  assert in `_PyLong_IsCompact` had been preventing `is_plain_scalar`'s
  inlining in those tables — their absolute numbers are artefacts, the
  relative conclusions and the objdump-based canary finding of E26-P2
  (taken from shipping binaries) stand. At the shipping define v4 keeps
  v3's instruction stream in `write`, `write_scalar_run`,
  `write_string_bytes`, `write_mapping_uncached`, `Frame` and
  `write_mapping_body`; serializer text +2.6% under the profile; no canary
  site anywhere; the compact-int fast path unchanged. Open for the lead:
  the conservative compactness bound (ints in \[2^30, 2^63) lose frame
  elision, none in any dataset); a 16-instruction-smaller classifier
  variant that costs every str a compare (a timing decision). Fourth review (`build/evidence/FIX1-V4-REVIEW/REVIEW.md`): a 420-process
  matrix (14 shapes × 5 int magnitudes × 3 mutators × 2 modes, threshold
  (1,1,1), 500 pending finalizers) 420/420 clean on v4 against 36 SIGSEGVs on
  the v3 control, 112 `_pylong` cells clean under ASan+UBSan, CPython 3.10,
  3.11, 3.13 and 3.14, all fifteen assemblies and every archived transcript
  reproduced, the user-code enumeration re-derived by reading every CPython
  call — and **refuted on six stale comments only** (the file header's
  neighbours, `latch()`'s doc and two contract-test docstrings still said
  "two steps"; one restated the exact premise that caused the defect), plus
  a pre-existing scope error in api.md (the row-as-read rule holds only
  below 64 levels of dict nesting; deeper dicts are followed live) and two
  optional wordings. The reviewer states the code, tests, codegen tables
  and behaviour need no change.
- Lead's decision: accept with fixup — the six comment edits and the api.md
  clause are applied by the lead at integration and listed for the P5
  review of the integrated candidate.
- Outcome: **accepted 2026-09-06** (integration in P5)

## E26-FIX2 — a surrogate key poisons the schema cache (invalid JSON)

- Opened 2026-09-06 (from the E26-FIX1 review, defect F5) · owner: Opus
  worker · reviewer: a different Opus worker
- Defect, pre-existing at 90dd3b7: `build_schema`'s `PyUnicode_AsUTF8AndSize`
  failure arm clears `schema.keys` without releasing them and leaves the
  way's `counts`/`first_keys`/`key_row` matching, so the next call hits the
  way with an empty key vector, marks it prepared with zero spans, and emits
  no key bytes: `strata.dumps({"\ud800": 1, "b": 2})` raises
  `UnicodeEncodeError` twice, then returns `{1,2}` silently.
- Rejection condition: any input that still yields invalid JSON, or a
  behaviour change on valid keys
- Authored 2026-09-06 (`exp/fix2-surrogate-key` 2df173f,
  `build/evidence/E26-FIX2/FIX2.md`): `Schema::forget()` releases the owned
  keys and blanks the key row, `DepthSchemas::invalidate(way)` un-matches
  the way before releasing, `build_schema` calls it on failure, and the
  64-miss retirement path goes through the same routine (gaining the `wide`
  reset and row blanking it lacked); an optional `remember()` reserve hunk
  closes an unreachable double-release on `bad_alloc`. The defect reached
  disk (`strata.dump` wrote `{1,2}` on the third call). 27 contract tests
  on fresh threads (16 fail at 90dd3b7), 4,000 random documents
  byte-identical on both builds and to stdlib. Open for the lead: a
  repeatedly failing shape now retires that thread's depth after ~130
  caught failures (acceptable on an error path); +512 B of cold text and
  `remember` outlined by the optional hunk. Independent review
  (`build/evidence/FIX2-REVIEW/REVIEW.md`, also under CPython 3.10.20): not
  refuted, the defect confirmed worse than reported (every key position,
  widths 1–24, reaches disk through `dump`), no required changes; the
  patch's layout effect on `write_mapping_body`/`select` is read in P5's
  A/B.
- Outcome: **accepted 2026-09-06** (integration in P5)

## E26-P4b — schema retirement is permanent per thread (coverage and cliff)

- Opened 2026-09-06 (from the audit's Finding 2) · owner: lead · unscheduled
- `DepthSchemas::select` retires a depth after 64 misses and never un-retires
  it; under `make test-py`'s canonical order the fused record writer is dead
  at the top-level depth for the pre-existing record tests. Product question:
  scope retirement per input as wave 22 did for the parse-side predictor.
  Not the row's cause (the P0 reordered-records control shows no schema
  rotation effect); a robustness item for P4.
- Historical outcome entry: "accepted on two i7 draws; on
  `exp/p6p7-integration` with the P6 change". Correction (2026-09-07): this
  attribution is unsupported by `cb22551`, whose source still permanently
  retires the cache. Reopened under the benchmark-lead plan; do not treat
  those i7 draws as evidence for per-document recovery.

## E26-P3 — keep the dict writer's staging rows out of the per-value dispatcher

- Opened 2026-09-06 · owner: lead (implementation delegated) · reviewer: Opus
  reviewer · timing: Opus measurement
- Parent revision: 90dd3b7 plus E26-FIX1 v2 (same file region; sequenced
  after it lands)
- Hypothesis: under `-fprofile-use` the profile inlines `write_mapping`'s
  `keys[24]`/`values[24]` rows into `Serializer::write` (and, on the fix's
  first form, `write_record_fused`'s value row into `write_sequence`), so the
  per-value dispatcher grows a 448-byte frame and the toolchain's default
  `-fstack-protector-strong` (Apple clang 21 on the M1, the host BUILDS.md
  records) guards it with a canary on every one of the ~1,375 `write` calls a
  mixed.json document makes (E26-P2 BUILDS.md §2: six `___stack_chk_guard`
  sites in `write`, four in `write_sequence`, none in the plain or LTO-only
  builds). Keeping the rows in out-of-line functions that are entered once per
  dict rather than once per value removes the canary and the frame traffic
  from the hot dispatcher without changing what executes per dict.
- Source: `src/strata/bindings/python_dumps.cpp` (`write`, `write_mapping`,
  `write_mapping_body`, `write_record_fused`, `write_sequence`); no
  semantic change; the fused writer stays the semantic twin of the general
  path
- Deciding workloads: static first — `clang -S` at setup.py's flags with
  `-fprofile-use` on the training-only profile: canary sites, frame sizes,
  instruction counts of `write`/`write_sequence` before and after; then
  `dumps_loop.py pair N gc-pair` on the N2 via a profile dispatch, and an
  M1 PGO-against-PGO A-B-B-A at ≥24 launches with the A/A floor
- Expected cost: one call per dict where the row was inlined (already the
  case for `write_mapping_body` in the plain build)
- Rejection condition: no reduction in canary sites/frame of the dispatcher
  under the profile, or no movement of the N2 `gc-pair` reading outside its
  interval, or any M1 row worse than its floor
- Outcome: open (waits for E26-FIX1 v2)

## E26-P4a — the exact-empty-dict path

- Opened 2026-09-06 · unscheduled · owner: lead
- Evidence (E26-P0, reproduced on both machines with tight intervals):
  empty dicts 13.1 vs orjson's 3.9 ns on the N2 (3.38x), 2.13x on the M1 —
  `write_record_fused` hands `{}` to `write_mapping`, which builds a cycle
  `Frame` and scans `open_` for an object that cannot contain anything.
  mixed.json contains none; a product improvement, not a route to the row.
- Outcome: open

## E26-P4c — short scalar sequences

- Opened 2026-09-06 · unscheduled · owner: lead
- Evidence (E26-P0): the values-only subset (500 lists of 1.75 elements,
  mixed str/int) 24.2 vs 18.0 ns per list on the N2 (1.35x), reproduced;
  belongs to the `wide_arrays`/`nested` families rather than this row.
  Measure varied lengths and element mixes, never a benchmark-specific
  length.
- Outcome: open

## E26-FIX2b — the re-entrant schema lease's fallback cache never releases its keys

- Opened 2026-09-07 · owner: open · found by the FIX2 reviewer
  (`build/evidence/FIX2-REVIEW/REVIEW.md` §6, `probes/probe3.py`) while
  auditing `2df173f`; pre-existing on 90dd3b7 and identical on the
  candidate, not introduced and not fixed by E26-FIX1 or E26-FIX2.
- Defect: `SchemaCacheLease::fallback_`
  (`src/strata/bindings/python_dumps_output.h`) is a plain vector of
  `DepthSchemas` whose `Schema` slots own key references and have no
  destructor. A nested `dumps` — user code running inside `dumps` (a
  `__del__` fired by `remember()`'s eviction, a `cycle_policy="warn"`
  warnings hook, an `int` subclass's `__str__`) calling `dumps` again —
  leases `fallback_`, `Py_INCREF`s the keys it remembers, and the lease is
  destroyed without releasing them: keys remembered by a nested `dumps` stay
  alive for the process's life, one set per nested call, while the same keys
  remembered by a top-level `dumps` die when the shared cache evicts them.
- Scope: only nested calls, which only the four user-code steps of the
  mutation contract can produce; output is unaffected. A leak, not a
  memory-safety defect.
- Fix shape: release the fallback's remembered keys when the lease ends
  (`forget()` on every way, or a `Schema` destructor); a test that a nested
  `dumps` leaves the key's refcount where it found it.
- Outcome: open

## E26-P5b — separate the profile recipe from the source fixes

- Opened 2026-09-07 · owner: lead · the measurement review's one further
  check: E26-P2 priced the recipe alone (same source) at −9% on `dumps flat`; E26-P5 priced recipe + FIX1 + FIX2 at −2.0/−1.6% on the same row;
  no instrument had priced the source fixes in isolation.

- Design: arm C = the candidate source built under the shipped (HEAD)
  `scripts/pgo_build.sh`, so C differs from the candidate (B) only by the
  profile recipe and from the shipped build (A) only by the source fixes;
  A-B-B-A rounds with orjson in-process as the control on `dumps flat`,
  `dumps users`, `dumps mixed`, `loads wide_arrays`, two tiers.

- - Arms and window (2026-09-07 00:24–00:39, load 1.1–2.2, the desktop
    otherwise idle; `build/evidence/E26-P5b/`, PLAN.md predeclared before
    timing): A = the shipped build ae107e76…, B = the candidate 48bb4952…, C =
    the candidate source built in the P5 code reviewer's worktree with HEAD's
    `scripts/pgo_build.sh` restored (11 raw profiles merged, the shipped
    recipe's gate runs included; md5 a7637fa6db631ae6c5cd6147ed6b1b27, profile
    33bc4a07…). W1 = C → B and W2 = A → C, ABBA ×6 + A, 60 repeats, eight row
    specs on two tiers; AA = C → C, two blocks. Every launch's extension md5 is
    on `logs/*.stderr`; the candidate was restored to the slot afterwards
    (48bb4952…).

- - Result, normalised by orjson, per-launch ABBA-block paired
    (`analysis/*_block_normalised.txt`; bootstrap intervals in `*_floor.txt`,
    the normalised A/A half-widths 0.1–1.0% on every row but small `dumps mixed`, 2.5% bytes and 1.8% str — a floor estimated from two blocks, the
    weakest instrument here):

  | row (bytes / str)          | W1 recipe alone, C → B                                   | W2 source fixes alone, A → C                                                                                                    |
  | -------------------------- | -------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------- |
  | small `dumps flat`         | **+6.1% / +5.8%**, 6/6 blocks positive, \[+5.96, +6.89\] | **−8.5% / −7.8%**, 0/6, \[−9.08, −8.12\]                                                                                        |
  | medium `dumps flat`        | **+6.3% / +5.9%**, 6/6, \[+5.84, +6.75\]                 | **−8.1% / −7.9%**, 0/6, \[−8.43, −7.77\]                                                                                        |
  | small `dumps users`        | +4.4% / +4.1%, 6/6                                       | −6.6% / −6.0%, 0/6                                                                                                              |
  | medium `dumps users`       | +4.9% / +4.9%, 6/6                                       | −6.5% / −5.9%, 0/6                                                                                                              |
  | small `dumps mixed`        | +4.4% / +3.9%, 6/6                                       | −2.7% / −2.4%, 0/6 (inside the 2.5% / 1.8% floor; the medium row clears at a 0.1% floor, and the raw estimator clears here too) |
  | medium `dumps mixed`       | +3.6% / +3.3%, 6/6                                       | −3.0% / −2.7%, 0/6                                                                                                              |
  | small `loads wide_arrays`  | +1.5%, 6/6                                               | 0.0%, 4/6 (inside the 0.6% floor)                                                                                               |
  | medium `loads wide_arrays` | +1.7%, 6/6                                               | +0.2%, 4/6 (inside the 0.9% floor)                                                                                              |

- - The two windows compose to E26-P5 within about a point on every row (small
    `dumps flat` bytes: (1 − 0.085)(1 + 0.061) − 1 = −2.9% against P5's −2.0%;
    `dumps users` −2.5% / −1.9% exactly; `loads wide_arrays` +1.5% / +1.9%
    against +2.1% / +2.1%), so the three arms are consistent and the
    decomposition is trustworthy.

- - Reading. On the source that ships, the training-only profile is a loss on
    every measured row — four to six percent on the serializer rows and the
    whole of the `loads wide_arrays` lean — and the source fixes alone are worth
    six to eight and a half percent on `dumps flat` and `dumps users` and about
    three on medium `dumps mixed` — the small cell reads the same sign in every
    block but inside its own A/A floor — with the parse rows unmoved. E26-P2's
    −9% was the contaminated profile's damage to the unfixed source:
    `-fprofile-use` inlined the dict rows into `write` and paid a
    stack-protector canary and a 448-byte frame per value; E26-FIX1 v4 leases
    those rows and so removes the damage under any inlining decision, after
    which the training-only profile is the slower of the two. What the +3.3–6.3%
    residual is remains unattributed: both arms of the fixed source carry no
    stack-protector site and 176/208-byte `write` frames, so the canary is not
    the difference between them. E26-P2's conclusion is superseded, not wrong:
    it measured what it said on the source it measured.

- - Decision: the candidate is recomposed as the source fixes under the
    shipped recipe — `scripts/pgo_build.sh`, `pgo_build_clang_cl.py`,
    `pgo_build_msvc.py` and `docs/build-and-test/SKILL.md` restored to 90dd3b7
    in the working tree; the profile-separation change stays on
    `exp/e26-p2-profile` (0747d73) with this entry as its negative result, retry
    only with a training workload that reads at least as well as the
    gate-inclusive profile on the fixed source. The main tree's `make pgo` under
    the shipped recipe is the recomposed build (its md5 and gates are in the P5
    entry); the P5 roll and its reports are re-taken on it.

- Outcome: measured; the recipe change is rejected on the fixed source;
  independently reviewed (`build/evidence/P5B-REVIEW/`, not refuted, wording
  corrections applied)

## E26-P6 — the x86 cost of the re-entrancy fix's serializer changes

- - Opened 2026-09-07 · owner: open · from the two five-platform samples on
    79fa3df (E26-P5): on runners comparable with the previous sample, the x86
    legs read the serializer rows +2–6% against orjson (linux-x86_64 `dumps flat` 0.78x → 0.83x, macos-x86_64 0.77x → 0.84x, `dumps users` +2–3%) while
    the arm64 legs read −8..−12% in ratio, as the M1 window predicted. The fixes
    are correctness (a use-after-free); they stay. The cost is to be attributed
    and removed on x86.
- - Hypotheses, in the order to test: register pressure — the leased
    `StagedRow` pointer, the `RowNode` registration and the latched ownership
    per container are three more live values in `write_mapping_body` /
    `write_record_fused`, cheap on arm64's 31 registers and spilled on x86-64's
    16; the `RowLock` list push/pop per container on the x86 legs'
    `-fstack-protector-strong`; a PGO layout change (the profile is
    gate-inclusive on every leg). Instruction count and simulated i-cache do not
    explain it (the 79fa3df linux-x86_64 cachegrind reads strata below orjson on
    both).
- - Instruments: a same-host A-B-B-A on an x86 machine — the P0 driver
    (`benchmarks/ab_rows.py`, `probe-ab-rows`) run in a `workflow_dispatch` job
    on the linux-x86_64 and windows legs with both SHAs' extensions built in the
    same job, orjson in-process as the control; then a per-function
    perf/cachegrind diff of `write_mapping_body` and `write_record_fused`
    between 32c5fa4 and 79fa3df on that leg (the profile leg's x86 artifacts for
    32c5fa4 are not archived; dispatch `profile.yml` on a tag of it).
- - Predeclared 2026-09-07 (before any run): `.github/workflows/ab_x86.yml` on
    branch `exp/p6-x86-ab`, `workflow_dispatch` with A = 32c5fa4 (the shipped
    build's source) and B = `exp/p6-x86-ab` at 93799cd (main 2ac7dbe plus the workflow and `benchmarks/ab_blocks.py`, dispatched under `profile.yml`'s name because the dispatch API resolves workflow names on the default branch only; the source identical to 79fa3df), both
    built by `make pgo` under the shipped recipe on the SAME runner, on the
    linux-x86_64 (ubuntu-latest, clang) and macos-x86_64 (macos-15-intel, the
    i7-8700B the samples saw on every draw) legs. Rows: small `dumps flat`/`users`/`mixed`/`wide_arrays`, small `loads wide_arrays` (the
    parse-side control: the source there is unchanged), medium `dumps flat`/`users`/`mixed`; ABBA ×6 + A at 60 repeats, orjson in-process as the
    drift control, then an A/A of arm A over two blocks as the floor. Reading
    rule: `ab_blocks.py`'s normalised per-launch, ABBA-block paired effect with
    `ab_floor.py`'s bootstrap interval; a row moves when the interval excludes
    the A/A floor of the same estimator. Expectation from the samples: B/A on
    `dumps flat` +3..+7% and `users` +2..+4% on both legs, `loads wide_arrays`
    inside its floor; a reading inside the floors everywhere would mean the
    samples' x86 signal was the runner lottery after all. Also per arm:
    serializer symbol sizes, `write*` frame sizes and stack-protector sites
    (objdump / otool), and on Linux a perf profile of the small `dumps flat` and
    `users` loops, to point at the function the cost sits in.
- - Result, linux-x86_64 (run 34065637472, an AMD EPYC 9V74 with 4 vCPUs; A =
    32c5fa4 at md5 a9d5a90e…, B = 93799cd at 00ac9ccf…, both `make pgo` on that
    runner; 25 + 9 launches, every launch's extension md5 on
    `R1.stderr`/`AA.stderr`; artifacts under
    `build/evidence/E26-P6/run_34065637472/`): B/A normalised, ABBA-block paired
    — `dumps wide_arrays` **+7.0% / +6.5%** (bytes / str; six of six blocks;
    interval \[+6.4, +7.3\] against an A/A floor of 0.5%), `dumps flat` **+3.6% /
    +3.5%** small and **+4.0% / +3.7%** medium (six of six; floors 1.3% / 1.2%),
    `dumps users` +1.4% / +1.5% small (inside its 2.1% floor) and +1.9% / +1.7%
    medium (clears a 0.5% floor), `dumps mixed` +0.7% / +0.9% (inside floors of
    1.1% / 0.9%), and `loads wide_arrays` −0.1% (the parse control, unchanged,
    floor 2.0%). The x86 cost is confirmed on one machine under one recipe:
    arrays of scalars pay most, which is the element loop (H1 in CODEGEN.md),
    then flat records (H2/H3), and the mixed row hardly at all. Per-arm frames
    on that PGO+LTO build (`frames.{A,B}.txt`): `write` 440 → 168 bytes,
    `write_mapping_body` 504 → 136, `write_record_fused` out of line at 120 —
    the frames shrank while the loops slowed, so frame size is not the cost. The
    perf step failed: hosted runners deny `perf_event_open` without CAP_PERFMON
    (`kernel.perf_event_paranoid`); the workflow now tries the sysctl and
    continues on error.
- - - Result, macos-x86_64 (the same run; the i7-8700B the samples saw on every
      draw, frame pointer pinned; A = 32c5fa4 at b75059a9…, B = 93799cd at
      60808147…, both `make pgo` on that VM; 25 + 9 launches, md5s verified): B/A
      normalised, ABBA-block paired — `dumps flat` **+7.3% / +7.0%** small and
      **+7.5% / +6.7%** medium (six of six; floors 2.0% / 1.2%), medium `dumps mixed` **+6.4% / +5.5%** (six of six; floor 2.7%), small `dumps mixed` +3.8%
      / +4.3% and `dumps users` +3.9% / +3.6% small, +3.6% / −0.5% medium (five or
      six of six positive but inside this VM's wide A/A floors of 5–12% on those
      rows), and — unlike the EPYC — `dumps wide_arrays` +0.3% / +0.1% and `loads wide_arrays` +0.2%, both unchanged. The two machines split the mechanism the
      way CODEGEN.md predicted: with %rbp pinned (Apple clang's default) the
      record writer pays — `write_mapping_body`'s sixth argument evicting `this`
      and the fused writer's per-record call — so flat records and the mixed dicts
      cost 6–7% and arrays of scalars nothing; with -fomit-frame-pointer (Linux
      clang's default) `this` stays in a register and the surviving cost is the
      element loop's re-read, so arrays of scalars pay 7% and flat records 4%.
      Both are the fix's x86 codegen, neither is the profile. (The otool frame
      scan on this leg matched only the canary sites — the `subq` regex expected
      spaces where otool prints a tab; fixed in the workflow for the next
      dispatch.)
- - Result, run 2 (34067095664, branch head c7509f4 — the same source plus the
    workflow's macos-arm64 leg and the parse rows as measured rows; artifacts
    under `build/evidence/E26-P6/run_34067095664/`). linux-x86_64, a second EPYC
    9V74 draw: `dumps wide_arrays` **+6.6% / +6.3%** (six of six, floor 0.5%),
    `dumps flat` **+3.6% / +3.5%** small and **+4.4% / +4.1%** medium (six of
    six; floors 4.3% and 3.2% on this draw's noisier A/A, so the interval, not
    the floor, is the evidence: every block between +2.9% and +4.7%), `dumps users` and `dumps mixed` +0.6..+1.3% (inside floors), `loads wide_arrays`
    +0.1%, `loads users` +0.9%, `loads mixed` +0.9% (inside floors of 2–18%) —
    the first draw reproduced. macos-arm64, one Apple M1 VM with both revisions
    built on it: `dumps users` **−5.7% / −3.7%** small and **−5.1% / −4.0%**
    medium (zero of six blocks positive), `dumps flat` +1.2% / −1.3% small and
    +1.8% / +3.0% medium, `dumps mixed` −3.4% / −1.5% small and −1.1% / −3.7%
    medium (all inside this VM's floors of 2–7%), `dumps wide_arrays` −0.1%; the
    parse rows `loads wide_arrays` +0.3% small / −2.3% medium, `loads users`
    −1.9%, `loads mixed` +0.3%, every one inside the VM's 7–14% raw floors. So
    the arm64 serializer gain the M1 window measured is there on the VM too
    (users), and the macos-arm64 parse-row shift the five-platform samples
    showed (0.04–0.05 in ratio) does not reproduce with both revisions on one
    VM: it was host variance between draws, not the revision — the
    profile-layout hypothesis for it is not supported and is dropped.
- - Result, run 2 on macos-x86_64 (the same i7-8700B VM class, B = c7509f4):
    `dumps flat` **+7.9% / +7.2%** small and **+7.9% / +6.6%** medium, `dumps mixed` **+6.9% / +6.9%** small and **+6.1% / +5.9%** medium (six of six
    everywhere; floors 2–3% on flat and medium mixed), `dumps users` +1.7% /
    +1.7% small and +2.5% / +3.0% medium (inside floors of 6–12%), `dumps wide_arrays` −1.3% / −1.7% (inside a 14–16% floor), the parse rows `loads wide_arrays` +1.2% / −0.3%, `loads users` −1.2%, `loads mixed` +0.4% (inside
    7–20% raw floors) — the first draw reproduced on the frame-pointer build:
    the record writer pays, the element loop does not. Four same-runner windows
    on two x86 machine classes now agree; the fix is the cause and the codegen
    diff names the three sites.
- Scope widened 2026-09-07 02:30: the macos-arm64 parse rows (see the E26-P5
  correction) join the question — the same-runner A/B runs on that leg too,
  with `loads wide_arrays` and `load users` as measured rows, not only
  controls.
- - Mechanism (2026-09-07, `build/evidence/E26-P6/CODEGEN.md`, a static diff
    of python_dumps.cpp compiled -O3 without PGO for x86-64 with the frame
    pointer pinned, x86-64 -fomit-frame-pointer, x86-64 -fstack-protector-strong
    and arm64): the fix's cost is the SysV x86-64 register budget — five
    allocatable callee-saved GPRs with %rbp pinned (six without) and six integer
    argument registers, against AAPCS64's ten and eight. (H1) `write_sequence`'s
    element loop re-reads the list's ob_item/ob_size every element (the
    live-follow contract); arm64 keeps both in callee-saved registers (one
    `ldp`, a register back-edge compare), x86 round-trips them through the frame
    with a store-to-load forward on the loop-carried compare (15 frame ops per
    element with the frame pointer, 13 without, against arm64's 4). (H2)
    `write_mapping_body` grew a sixth integer parameter (`own_from`) — exactly
    SysV's limit — and a `RowLock` live to frame pop, so `this` is spilled and
    reloaded twice per key in the prepared-key loop (35 → 43 instructions, 2 → 5
    frame ops on the frame-pointer build; 34 → 39 and 1 → 2 without the frame
    pointer; 36 → 40 and 0 → 0 on arm64). (H3) `write_record_fused` went out of
    line on an arm64 profile measurement, a call and a six-register prologue per
    record on x86. Rejected there: instruction footprint (hot text +13.9% x86
    against +14.1% arm64) and the stack protector (the after has fewer canary
    sites). The same-runner windows fit the split: the frame-pointer build
    (macos-x86_64) pays in the record writer, the Linux build in the element
    loop.
- - Implementation (2026-09-07, `exp/p6-x86-regs` 6cfbc62 on main 2ac7dbe;
    `build/evidence/E26-P6/IMPL.md`; author an Opus worker, reviewed separately
    below): a 64-bit `user_steps_` counter on the Serializer, bumped at the head
    of `latch()` (the choke point before the three steps that invoke user code)
    and on the two release arms that can fire a `__del__` (`close_container()`'s
    latched-container release and `~RowLock`'s latched-row release), with the
    enumeration and the proof that the release bumps are belt-and-braces in the
    member's comment; `write_sequence`'s element loop keeps the list's bounds
    loop-invariant and re-derives them only when the counter moved (the
    `from_list` flag is gone with it). `write_mapping_body` drops two parameters
    that re-stated `staged_row(map_depth_)` and `map_depth_`: six → four integer
    arguments. The two `RowLock`s stay two (they cover non-contiguous rows). H3
    was refused on evidence: inlining `write_record_fused` on this source takes
    the record key loop from 10 to 23 frame ops per key on the frame-pointer
    build (11 → 17 without), because `write_sequence`'s loop-carried values then
    compete for the same five registers; the before's inlined form paid only 9
    because it carried neither the deferred frame nor the row lock the fix
    requires — the out-lining stays on every target with both measurements in
    its comment. Codegen, steady state per list element on x86: frame ops 5 → 3
    (frame pointer), 3 → 2 (without), the back edge now a compare against a slot
    never stored in the loop; `write_mapping_body`'s prepared-key loop 39/5 →
    38/4 (frame pointer) and 35/2 → 34/2 (without, the before's count exactly),
    arm64 40/0 → 36/0. Gates in the worktree: fmt/lint clean, C++ 15/15, gated
    install, 2,099 Python tests (26 new in
    `tests/unit/test_dumps_user_step_counter.py`, 20 of which fail with the
    bumps removed and the original fault reproduces), ASan gate clean, `make gate` passed. arm64 sanity on this M1 (three windows, load 1.3–1.5, A/A
    floor 0.2–2.4%): every row inside its floor — `dumps flat` +0.3% / +0.1%
    pooled, `dumps mixed` −0.9% / −0.3%, `loads wide_arrays` −0.04% — a real but
    sub-resolution one-instruction cost per element on arm64, the N2 leg not yet
    seen. Open: `this` still reloads twice per key in `write_mapping_body`
    because the loop re-indexes the `schemas_` vector (a fixed array in the
    leased state would remove it at ~182 KB per thread; needs the ru_maxrss
    probe).
- - Predeclared (2026-09-07 03:20, before the run): the fix branch's own
    same-runner A/B, A = main 2ac7dbe, B = `exp/p6-x86-regs`, on linux-x86_64,
    macos-x86_64 and macos-arm64 (the branch's `profile.yml` carries the
    workflow with A fixed). Prediction: B beats A on the serializer rows on both
    x86 machine classes — the Linux build on `dumps wide_arrays` and `dumps flat` (H1), the frame-pointer build on `dumps flat` and `dumps mixed` (H2) —
    by less than main trails 32c5fa4 (H3 and the residual `this` reloads
    remain); the parse rows unchanged; arm64 inside its floors. A reading inside
    the floors on x86 prices H1 and H2 at zero and sends the work back to the
    codegen.
- - Acceptance A/B, run 34069600714 (A = main 2ac7dbe, B = `exp/p6-x86-regs`
    f0a2325 = 6cfbc62 plus the workflow files; artifacts under
    `build/evidence/E26-P6/run_34069600714/`). linux-x86_64 — a third x86 class,
    an Intel Xeon Platinum 8573C: B/A normalised, ABBA-block paired — `dumps wide_arrays` **−4.1% / −3.3%** (zero of six blocks positive; interval \[−4.4,
    −3.7\] against a 1.2% floor), `dumps users` **−2.7% / −3.9%** small and
    **−2.6% / −2.2%** medium (zero of six; intervals clear of floors of
    0.5–1.9%), `dumps flat` **−1.6% / −1.5%** small (\[−2.1, −1.5\], floor 3.3% on
    this draw's A/A — the interval is the evidence) and −0.4% medium (inside
    1.05%), `dumps mixed` −1.4% / −1.3% small and +0.1% / −1.0% medium (inside
    2–8% floors). The parse rows, whose source did not change: `loads wide_arrays` **+1.4%** (six of six blocks; interval \[+0.7, +1.7\] against a
    1.1% raw floor — at the edge, not inside), `loads users` +0.8% and `loads mixed` +0.8% (five of six, inside floors of 0.8–3.0%). The fix recovers
    about half of what main lost on this class (main trailed 32c5fa4 by
    +3.6..+4.4% on `dumps flat` and +6.6..+7.0% on `dumps wide_arrays`), and the
    extension's changed layout or profile — the 26 new tests are part of the
    gate-inclusive profile — leans on the parse rows by about a point, the
    mechanism the E26-P5 correction named and the macos-arm64 VM could not
    resolve. macos-arm64 (an M1 VM): every row inside its floor — `dumps flat`
    +0.8% / +1.1% small and +0.4% / −0.3% medium, `dumps users` +0.6% / +0.1%,
    `dumps wide_arrays` −1.7% / −0.4%, small `dumps mixed` +3.5% / +2.3% (four
    and five of six, inside floors of 4.9% / 5.2%), the parse rows inside 7–21%
    raw floors. Neutral on arm64, as the M1 windows read.
- - Acceptance A/B, macos-x86_64 (the i7-8700B, frame pointer pinned): the fix
    does not recover this build's cost — `dumps flat` **+1.9% / +1.5%** small
    (six and five of six blocks positive; interval \[+1.5, +2.1\] against a 3.7%
    A/A floor) and +1.4% / +2.1% medium (five of six; floor 1.7% / 1.0%), `dumps users` +1.2% / +1.3% small and +0.2% / +5.5% medium (inside 4–5% floors),
    `dumps mixed`, `dumps wide_arrays` and every parse row inside their floors.
    Consistently signed but inside the floors on the record rows: the
    frame-pointer build's record-writer cost (H2's residual `this` reloads, H3's
    per-record call) is untouched by this change, and the element-loop epoch
    compare may cost it a little. So the fix is a Linux-x86 gain (about half of
    main's loss on the serializer rows), neutral on arm64, and not a gain on the
    Darwin x86 build; the lever CODEGEN.md's control names for that build is
    freeing the frame pointer (`-fomit-frame-pointer`, Linux clang's default at
    -O3), which puts `this` back in a register — E26-P7 below.
- - Review (2026-09-07 04:10, `build/evidence/P6-REVIEW/REVIEW.md`, a
    non-author Opus reviewer; not refuted): the user-code sites enumerated from
    the code match the counter's bumps — `latch()`'s three call sites, the two
    latched releases — with every error path returning through the loops; a
    121-case adversarial matrix (six sites × four mutations × two depths × two
    modes, plus a `__del__` that re-enters `dumps` on the list being written) is
    byte-identical between main and the branch and clean under ASan+UBSan, and
    the same matrix on a build with the bumps removed is a SIGSEGV with ASan's
    heap-use-after-free at the element read. Two required fixes, applied: the
    two cycle-warning tests depended on pytest's warnings plugin resetting
    `__warningregistry__` (they now run under `warnings.catch_warnings()` with
    `simplefilter("always")` and pass with `-p no:warnings`), and IMPL.md's
    arm64 `write_mapping_body` cell read 40 where the loop has 36 instructions
    in every revision (the four `Ltmp` markers counted as instructions) — no
    arm64 win was ever there. Noted for the record: the change moves the failure
    mode — main's unconditional re-read was safe against any user-code site, the
    counter is safe only while the four-step enumeration stays complete
    (complete today, verified twice); a future missed step would cost a
    use-after-free rather than wrong output, which the member's comment and the
    new tests guard. The reviewer's out-of-scope flag — the Linux parse control
    rows moved +0.75..+1.4% as consistently as the serializer rows improved — is
    E26-P7b.
- - Acceptance A/B with the profile's share removed (run 34073739543, branch
    `exp/p6-acceptance2`; A = `exp/p7b-tests-only` f955ed0 = main plus the same
    26 tests, B = the P6 branch, so both profiles see the same suite and the
    difference is the code). linux-x86_64, an EPYC 7763: `dumps wide_arrays`
    **−5.8% / −5.4%** (zero of six blocks positive; intervals \[−6.0, −5.5\] and
    \[−6.0, −5.1\] against floors of 0.4% / 0.6%), `dumps users` **−2.5% / −2.6%**
    small and **−2.2% / −2.2%** medium (zero or one of six; intervals clear of
    1.1–3.0% floors), `dumps flat` +0.4% / −0.3% small and +0.1% / −1.2% medium
    (inside this draw's 5% flat floors), `dumps mixed` −0.3..−0.9% (inside), the
    parse rows `loads wide_arrays` +0.8% / −0.5% (four of six; \[−1.1, −0.04\]
    raw), `loads users` +0.6%, `loads mixed` +1.4% (six of six; \[+0.4, +2.1\]
    against a 2.5% raw floor — inside, consistently signed, the same order as
    the layout noise the P7b control showed in the other direction). The code
    alone is worth about −5.6% on arrays of scalars and −2.4% on records on
    Linux x86, larger than the first acceptance read because that B arm carried
    the tests' profile cost.
- - The same clean acceptance on macos-x86_64 (the i7 VM) resolved nothing:
    this draw's A/A floors ran 4–24% (the VM pool degraded through the night —
    2% floors at 02:30, 10–17% after 04:30), and every row sits inside them:
    `dumps flat` −0.7% / −0.3% small and +0.6% / −0.3% medium, `dumps users`
    −1.9% / −2.7% small, `dumps wide_arrays` −2.5% / −2.7%, `dumps mixed` −0.6%
    / −1.2% small and +0.1% / −2.5% medium, the parse rows +0.1..+3.8% (five of
    six on `loads wide_arrays`, inside a 24% raw floor). Read with the first
    acceptance draw (floors 1–4%, `dumps flat` +1.5..+2.1%), the P6 code alone
    is at worst neutral on the frame-pointer build and its record-writer cost
    there is what E26-P7's flag addresses; a quieter i7 draw is owed before that
    leg's number is quoted.
- - Windows check (T4's cross-platform clause; run 34085356001, branch
    `exp/p6-windows-ab` 5411e60 = `exp/p6p7-integration` plus T3's driver, A =
    2ac7dbe; both arms built by `scripts/pgo_build_clang_cl.py` on one
    windows-latest runner, an AMD Zen 4 (Family 25 Model 17); T3's estimator
    with a six-block A/A; every launch's md5 on the logs): B/A normalised —
    small `dumps flat` **−2.0% / −2.5%** (zero of six blocks positive; intervals
    \[−2.6, −1.2\] and \[−3.2, −1.4\] against floors of 1.6% / 1.9%), medium `dumps users` **−4.4% / −3.8%** (zero of six; \[−6.8, −1.7\] against 0.8%), medium
    `dumps flat` −1.5% / −3.1% and small `dumps users` −1.1% / −1.2% and `dumps mixed` −1.3% (inside floors of 1.5–8%), medium `dumps mixed` −14% / −9%
    discarded — the rival's raw time jumped +17.7% and the A/A floor on that row
    is 12–27% — and small `dumps wide_arrays` **+3.3% / +2.8%** (five of six;
    \[−0.1, +6.4\] and \[+0.4, +6.0\] against floors of 4.3% / 3.9%: inside, and
    mostly the rival moving — raw strata +1.0% / +0.7%, raw rival −2.4%); the
    parse rows and `dump mixed` (a real file write) inside their floors. The
    Linux gain on arrays of scalars does not appear under clang-cl's codegen,
    and the sign there stays unresolved; no serializer row is worse past its
    floor on any leg. The clang-cl x64 build has no frame pointer to free, so
    E26-P7 is inert here by construction.
- Outcome: accepted with E26-P7 — the code alone (profile held equal) reads
  linux-x86_64 `dumps wide_arrays` −5.6% and `dumps users` −2.4%, arm64
  neutral, and the frame-pointer flag takes the Darwin x86 record rows by
  2–3%; T4's cross-platform clause is met (Linux x86 gain, Darwin x86 gain from
  the flag, arm64 neutral on two hosts, Windows flat/users gain with
  wide_arrays inconclusive); integrated with the P0 branches on
  `exp/p0p6-integration` for the human; E26-P8 (the profile) is the next
  item

## E26-P7 — free the frame pointer on x86-64 builds

- - Opened 2026-09-07 04:30 · owner: lead · from E26-P6: CODEGEN.md's
    -fomit-frame-pointer control shows `this` back in a callee-saved register in
    `write_mapping_body`'s prepared-key loop (35/2 → 34/2 instructions/frame
    ops, the before's shape) once %rbp is free, and the acceptance A/B read the
    Darwin x86 build (frame pointer pinned) at +1.4.. +2.1% on `dumps flat` for
    the P6 change while the Linux build (no frame pointer) gained. Linux clang
    already omits the frame pointer at -O3; Apple clang keeps it.
- - Change: `setup.py` appends `-fomit-frame-pointer` on x86-64 POSIX builds
    (branch `exp/p7-x86-nofp`, forked from the reviewed P6 branch d5365b3);
    arm64 unchanged. Predeclared before the run: A = d5365b3 (P6), B = P6 + the
    flag, both x86 legs, the same rows and reading rule as E26-P6. Prediction:
    macos-x86_64 `dumps flat` and `dumps mixed` improve by several percent (the
    record writer's `this` reloads and the fused writer's prologue both shrink);
    linux-x86_64 inside its floors (the flag is already the default there, so a
    move would mean the flag is not the only difference); parse rows unchanged
    on both.
- - Result, linux-x86_64 (run 34072322704, an EPYC 9V74; A = d5365b3, B =
    8044fbb; md5s verified): every row inside its floor — `dumps flat` +0.5% /
    +0.2%, `dumps wide_arrays` −0.2% / −0.4%, `dumps users` −0.7% / −0.9%, parse
    rows ±0.4% — as predicted for a build where the flag is already the default.
    The macos-x86_64 leg is the measurement.
- - Result, macos-x86_64 (the same run; the i7-8700B; A = d5365b3 at
    a405b22a…, B = 8044fbb at 25d3df03…; md5s verified): B/A normalised,
    ABBA-block paired — medium `dumps flat` **−3.4% / −3.2%** (one and two of
    six blocks positive; intervals \[−5.9, −3.2\] and \[−7.8, −3.1\] against A/A
    floors of 2.4% / 4.0% — both clear), small `dumps flat` −1.5% / −1.7%
    (\[−5.4, −3.3\] bytes against a 5.0% floor — inside), `dumps mixed` −2.8% /
    −1.4% medium and −2.0% / −1.4% small, `dumps users` −1.7% / +0.8% medium and
    −1.7% / −1.8% small, `dumps wide_arrays` −4.0% / −2.4% (zero of six
    positive; \[−6.4, −1.1\] against a 10.8% floor), the parse rows −3.1..−0.7% —
    seventeen of eighteen rows negative, but this VM draw's A/A floors ran 2–25%
    (the noisiest window of the night), so only the medium flat row is resolved.
    Frames per arm: `write` 0x88 → 0x78, `write_scalar_run` 0x48 → 0x38,
    `build_schema` 0x68 → 0x48 — the freed %rbp shows as one slot fewer in each.
    A confirmation draw is dispatched (run 34075068617); the flag is read on the
    two together.
- - Confirmation draw (run 34075068617; the i7 VM at floors of 0.3–3.5% on the
    serializer rows this time; A = d5365b3 at ccfb6255…, B = 8044fbb at
    25d3df03…; md5s verified): B/A normalised, ABBA-block paired — medium `dumps flat` **−2.4% / −2.0%** (zero of six blocks positive; intervals \[−2.9, −2.0\]
    and \[−2.5, −1.7\] against floors of 0.3% / 1.8%), small `dumps flat` **−2.1%
    / −2.4%** (\[−2.4, −2.0\] against 2.5% — at the edge; str \[−3.6, −2.7\] against
    1.6% — clear), `dumps mixed` **−1.7% / −1.6%** medium and −2.1% / −1.6%
    small (zero to two of six; intervals clear of 0.8–1.7% floors), `dumps users` −3.1% / −3.4% medium and −2.9% / −2.2% small (one or two of six;
    inside this draw's 2.4–8% floors on those rows), `dumps wide_arrays` −3.6% /
    −3.6% (\[−6.0, −1.3\] against 3.0% — marginal), the parse rows −3.1..+0.0%
    inside 6–11% raw floors. The two i7 draws agree in direction on every
    serializer row and the quiet one resolves flat and mixed; Linux reads inside
    its floors on both draws (the flag is its default). Accepted:
    `-fomit-frame-pointer` on x86-64 builds is worth 2–3% on the Darwin x86
    serializer rows, nothing on Linux, and arm64 is untouched by construction.
- - Restated (2026-09-07 07:20, from T3's single estimator with raw columns):
    the flag's evidence is strata's own movement on the confirmation draw —
    medium `dumps flat` raw strata −1.9% (rival +0.9%, normalised −2.4%) and
    medium `dumps mixed` raw strata −1.2% (rival +0.6%, normalised −1.7%); the
    first draw's medium `dumps flat` −3.4% and the confirmation draw's two small
    rows were rival movement (raw strata +0.7%, +0.8%, −0.7% against rival
    +4.5%, +3.1%, +1.9%) and are not evidence for it. Accepted magnitude: about
    1–2% on the Darwin x86 record rows, not 2–3%; direction unchanged, Linux
    neutral, arm64 untouched.
- Outcome: accepted on the confirmation draw's medium rows at 1–2%; on
  `exp/p6p7-integration` with the P6 change

## E26-P7b — the profile's share of the P6 parse-row lean

- - Opened 2026-09-07 04:30 · owner: lead · from E26-P6's acceptance A/B on
    linux-x86_64: the parse control rows moved +0.75..+1.4% (six of six blocks
    on `loads wide_arrays`) although no parse source changed. The gate-inclusive
    PGO recipe merges the test suites' profiles, and the P6 branch adds 26
    serializer tests to the gate, so every function's profile counts and layout
    can move.
- - Design: branch `exp/p7b-tests-only` = main 2ac7dbe plus only
    `tests/unit/test_dumps_user_step_counter.py` (and the A/B workflow files); A
    = 2ac7dbe, B = that branch, linux-x86_64 only. Prediction if the profile is
    the mechanism: `loads wide_arrays` reads about +1% with the serializer rows
    unmoved; if it reads inside its floor, the lean is the P6 code's layout (or
    link order) and the profile hypothesis is dropped for the second time.
- - Result (run 34072326424, linux-x86_64, an AMD EPYC 7763; A = 2ac7dbe at
    d6dd4eb9…, B = f955ed0 at 42f3dc21…, the source identical, 26 tests added to
    the gate; 25 + 9 launches, md5s verified): B/A normalised, ABBA-block paired
    — `dumps wide_arrays` **+3.7% / +3.5%** (five of six blocks; interval \[+3.5,
    +4.0\] against a 0.8% floor), small `dumps users` +1.5% / +0.9% (six of six;
    \[+1.2, +2.0\], floor 1.0%), `dumps flat` **−1.7% / −0.8%** small and −1.2% /
    −0.5% medium (\[−2.0, −1.5\] on small bytes, floor 1.4%), `dumps mixed` +1.0%
    / +0.4% and the parse rows `loads wide_arrays` −1.4% (one of six; \[−1.35,
    0.00\] against a 2.6% raw floor), `loads mixed` +0.9%, `loads users` +0.3% —
    inside their floors. Reading: the profile's share is real and large on the
    serializer — with no source change, 26 more gate tests move `dumps wide_arrays` by nearly four percent and `dumps flat` by nearly two, in
    opposite directions — and it does not explain the P6 parse-row lean
    (opposite sign here, both draws near their floors: unattributed noise at the
    1–1.5% level). Two consequences. First, E26-P6's acceptance A/B understated
    the code: its B arm carried this profile cost, so the code alone is worth
    more than −4.1% on `dumps wide_arrays`; the clean comparison — the P6 branch
    against main plus the same tests — is dispatched as run E26-P6/acceptance2.
    Second, and systemic: under the shipped recipe every test added to the gate
    is a performance change of several percent on some row, in a direction
    nobody chose; the training-only recipe (E26-P2) lost on the fixed source
    because its training workload covers less of the serializer than the suite
    does, not because test counts are good counts. E26-P8 below.
- Outcome: measured on Linux; the profile's share on the serializer is
  several percent per test-suite change (see E26-P8), the parse-row lean
  not reproduced

## E26-P8 — a PGO profile that does not move with the test suite

- - Opened 2026-09-07 04:45 · owner: open · from E26-P7b: the shipped recipe
    merges the two gate pytest runs' profiles with the training run (47.5% of
    all counts on 2026-09-06), so the profile — and with it the layout, inlining
    and hot/cold splits of every function — changes whenever a test is added, in
    directions nobody chose: 26 serializer tests moved `dumps wide_arrays` +3.7%
    and `dumps flat` −1.7% on linux-x86_64 with the source untouched. E26-P2's
    training-only recipe was the right shape and lost (E26-P5b: +3.3–6.3% on the
    serializer rows) because the training workload covers less of the serializer
    than the suite does, not because test counts are good counts.
- - Design: keep the profile training-only (the E26-P2 scripts on
    `archive/exp/e26-p2-profile`, 0747d73) and grow `pgo_training.py`'s workload
    until it reads at least as well as the gate-inclusive profile on the fixed
    source on both x86 machine classes and the M1 — records of the small tier's
    shapes in bytes and str, wide arrays of every scalar kind, nested and mixed
    documents, the cursor and NDJSON paths — chosen from the API's surface,
    never from the benchmark datasets or seed 42 (the plan's rule). Acceptance:
    the same-runner A/B of the two recipes on the same source inside the floors
    or better on every row on three legs, then a five-platform sample. Until
    then, every change that adds tests to the gate must be priced with a
    tests-only control like E26-P7b before its A/B is read.
- Step 1 measured (2026-09-11/12; evidence `build/evidence/benchmark-lead/p8/`,
  arms `exp/p8-b1`, `exp/p8-b2`, `exp/p8-b12`, `exp/p8-b12t`). First an exact
  per-process attribution of main's profile (the training run is the one raw
  profile with no cycle or re-entrancy counts; entry counts checked by
  re-instrumenting each unit and letting the profile reader propagate them,
  TRAIN + TESTS = ALL for every one of 424 functions; `p8/analysis/`): the
  tests are 55.35% of all counts, and the fused record writer's training
  entries fall back 70.8% of the time — 58% at the retired-depth check,
  because the corpus varied a key at depth two on every record and gave two
  top-level shapes one (count, first key) pair, retiring both depths inside
  the first document; 13% at `entry_array() == nullptr`, because the training
  payload is `strata.loads`'s, whose presized dicts carry a general-kind keys
  table (E26-P23). Two arms fix those in the scripts alone, under the
  unchanged recipe: b1 builds the serializer's payload with `json.loads` (one
  strata-parsed arm kept); b2 gives the top-level shapes distinct key counts
  under one first key, keeps every optional list under a key every record
  carries, and moves the varying key to depth nine below a chain of one-key
  dicts; b12 is both. The training counters move as designed (the README's
  table: presized fallbacks 18,985 → 364, retired 86,341 → 17,706, the emit
  loop four times hotter, the general writer's training entries 260,226 →
  18,250). The rows do not: M1 screens against main's arm (fresh A/A, six
  blocks of sixty) read every row inside its floor for all three arms, b1's
  small file `dump nested` −2.3% the one resolved figure; runner A/B
  34645895948 (34f1805 against b12): N2 `dumps users` +1.1..+1.4% and five
  parse rows +1.4..+1.9% (`loads users`, `loads mixed`, `loads` and `load wide_arrays`, NDJSON `load users`) resolved against floors under 1%, `dumps flat` −0.5..−0.9%, mixed inside; M2 Pro VM and Windows no row past a floor; EPYC `dumps users` −1.9% medium and −2.6% small in its favour, small `dumps wide_arrays` (str) +1.4% against, the rest inside floors that reach 9% on that draw; i7 small `dumps flat` +1.9% and `loads mixed` +1.7% against, nothing else resolved. Every arm's binary matches its sidecar; the arms' training sources differ by design (`native-p8-corpus-34645895948/verification-all.json`).
  The 2026-08-15 reading holds with the fused writer in place: the corpus's
  job is branch coverage, and shaping its hot phase after the cache's rules
  moves training counts, not layouts, and on the N2 moves the parse rows the
  wrong way.
- Step 2 measured the same night: the training-only recipe (b12t: the gate
  runs' profiles written to `build/pgo/discard/` and never merged, only the
  training run's, recipe `training-only-posix-v1`) on the b12 workload,
  against main's arm on a quiet M1 (load 2.7, fresh A/A, `p8q_b12t`): `dumps flat` +12.9% medium and +11.7% small, medium `dumps mixed` +1.3..+1.4%,
  small `dumps users` +0.7..+1.5%, six of six; `wide_arrays` inside. E26-P5b's
  loss is larger with the fuller workload, not smaller: whatever the suite's
  bytes-mode `dumps` calls teach the profile about the flat path, the
  workload's do not, and the attribution above does not yet say what it is.
- Outcome: **step 1 closed as measured, no arm adopted; the corpus and the
  recipe stay as they are.** The recipe question stays open with two data
  points against training-only and the attribution tooling to price the next
  attempt; the presize mechanism moves to E26-P23 as a source fix. Branches
  kept as the record.

## T0 (plan of 2026-09-07) — inventory and reconciliation

- - 2026-09-07 06:00 · owner: lead. The plan
    `docs/performance/fable-5.1-opus-5-plan-2026-09-07.md` (with
    `ci-review-2026-09-07.md`) supersedes the September-6 draft's order. Safe
    production source: 79fa3df (checkout 2ac7dbe); the working tree's only
    source-affecting staged files are the diagnostic `ab_x86.yml` and
    `ab_blocks.py`. Reconciliation of the work that landed while the review was
    written: E26-P6's x86 mechanism, acceptance runs, review and fix branch
    (`exp/p6-x86-regs` d5365b3), E26-P7 (frame pointer, accepted on two i7
    draws), E26-P7b (the profile's share) and E26-P8 (opened) are all above.
    Under the plan they map to T4 candidate 1 ("reduce unnecessary list-storage
    refreshes", proof reviewed, Linux evidence with the profile held equal) plus
    a build-flag change the plan does not list; T4's acceptance rule still owes
    T3's single estimator recomputed over the saved TSVs and a Windows check, so
    the integration branch `exp/p6p7-integration` (7f66e81) is held, not handed
    over. E26-P6's macos-arm64 same-VM A/B answers part of T5 (`loads` rows
    unmoved within 7–14% floors; the file-load rows not yet executed). E26-P8 is
    T5's profile-recipe note read the other way: the review's finding 8 and P7b
    agree that the gate-inclusive recipe moves parser and serializer layout with
    the test suite. The leak in finding 1 is the ledger's E26-FIX2b. Evidence by
    run and host is under `build/evidence/E26-P5/`, `E26-P6/`, `E26-P7/`,
    `E26-P7b/`, `P6-REVIEW/`, `P5B-REVIEW/`, `P5-REVIEW-*`.
- - Dispatch (2026-09-07 06:05): T1 (Ownership: the private-cache key leak,
    `python_dumps_output.h`), T2 (Reporting: report validation, regression
    coverage, transactional fetch) and T3 (Measurement: one A/B estimator, the
    driver's restore-and-persist repairs, rival per operation, three-arm
    handling, the profile step in bytes mode) run in parallel in isolated
    worktrees with non-overlapping allowed files; T3's sidecars wait for T2's
    `harness.py`. Each returns to a reviewer other than its author before
    integration.
- - Integration (08:20): `exp/p0-integration` c9344cc = main + T1 + T2 + T3
    after their follow-ups, two merge commits, no conflicts; gates on this host:
    fmt/lint clean, gated install, C++ 15/15, 2,188 Python tests, the ASan gate.
    The human fast-forwards it (scratchpad `staging_plan_p0.txt`). T4's Windows
    check for the held P6+P7 branch runs as `exp/p6-windows-ab` (5411e60 =
    `exp/p6p7-integration` + T3's driver, A = 2ac7dbe, both built by
    `pgo_build_clang_cl.py` on one windows-latest runner; run 34085356001) —
    predeclared: the P6 code alone read −5.6% on `dumps wide_arrays` and −2.4%
    on `dumps users` on Linux x86 with the profile held equal; clang-cl's x64
    build has no frame pointer to free, so the flag is inert there; a Windows
    reading inside its floors or better on every serializer row satisfies T4's
    cross-platform clause, a cost past its floor sends the change back.
- Outcome: reconciled; T1–T3 delivered, reviewed and integrated on a
  branch; T4's Windows check in flight

## T1 — release the private schema cache's owned keys (E26-FIX2b)

- - 2026-09-07 06:35 · author: an Opus worker · branch
    `exp/t1-private-cache-leak` a417333 (parent 2ac7dbe), +55/−1 in
    `python_dumps_output.h` only: `~SchemaCacheLease` releases the private
    state's remembered keys through the existing `DepthSchemas::invalidate` on
    every way of every depth when `fallback_` is set; `Schema` is made move-only
    (deleted copies, defaulted noexcept moves, static_asserts) so
    `schemas_.resize` keeps relocating by move and no destructor turns it into a
    copy — the double-decrement trap the plan named. Lifetime argument: the
    lease is the first local of `dumps_to_python` so it is destroyed last, after
    every Frame, RowLock and the output; it runs on every return path and during
    unwinding; only exact `str` keys are ever remembered, so the release can
    neither re-enter nor clobber a pending error; ownership stays one reference
    per stored pointer and `forget` is idempotent; the shared per-thread state
    stays immortal (its shutdown policy, now stated in a comment). Reproduced
    first on unmodified 2ac7dbe (refcount +100 over 100 re-entrant calls), reads
    0 after. Tests: `tests/unit/test_dumps_private_cache.py`, 28 cases on fresh
    threads (both modes, 24-key rows, prepared and wide schemas, five levels,
    three leases deep, the warnings-hook re-entry, failing inner calls,
    UnicodeEncodeError in `build_schema`, shared-cache controls); 21 fail on the
    unfixed build. Gates in the worktree: fmt/lint clean, C++ 15/15, gated
    install, 2,101 Python tests, ASan gate clean, `make gate` passed, and the
    same on CPython 3.10.20. No timing; the walk's functions are byte-identical
    in source, but the 28 added gate tests move the profile (E26-P7b), so the
    change is priced with a tests-only control before any row is attributed to
    it.
- Review (`build/evidence/T1-REVIEW/REVIEW.md`, not refuted): the leak
  reproduced on 2ac7dbe (+100 in both modes) and absent on a417333 in the
  reviewer's own fresh processes and worktrees; every refutation attempt
  failed — 64-miss retirement inside a private state, an exception out
  of `build_schema`'s failure arm, the outer call failing after the inner
  succeeded, three leases deep with 70 levels (relocation by move proven
  at run time), a key whose last reference is the cache's, a `str`
  subclass key (never cached), 20 threads with nested calls, and `dumps`
  from an `atexit` handler and from `__del__` at finalization — all
  exactly 0 drift and clean under ASan+UBSan. Required before acceptance:
  the tests-only PGO control (28 added gate tests move the profile), run
  as `exp/t1-ab` (A = `exp/t1-tests-only`, main plus the test file; B =
  the fix) on three legs; optional: the vacuous `noexcept` static_assert,
  two coverage cases, one line in docs/bindings/SKILL.md — applied in a
  follow-up.
- - Control design corrected (07:35): the tests-only arm (main plus the leak
    tests, `exp/t1-tests-only` 8fe57af) cannot build — 21 of the 28 tests fail
    on the unfixed source by design, so its gated install fails (run
    34079726524, cancelled). The clean control is the fix WITHOUT its test file
    against main (`exp/t1-source-ab` 6aece1c, A = 2ac7dbe): both arms pass the
    same 2,073-test gate, so the profile sees one suite on both and the
    difference is the destructor's source effect (run 34080402187, three legs,
    A/A at six blocks). The tests' own profile effect is the recipe's property
    (E26-P8), not the fix's.
- - Source-alone A/B (run 34080402187, `exp/t1-source-ab` 6aece1c = the fix
    without its test file, against main 2ac7dbe; both arms under the same
    2,073-test gate; A/A at six blocks; md5s verified per launch). linux-x86_64
    (an EPYC 9V74): every serializer row slightly faster — `dumps flat` −0.9% /
    −0.7% medium (\[−1.2, −0.7\], floor 0.6%: clears) and −1.0% / −1.4% small
    (\[−1.3, −0.7\], floor 0.6%: clears), `dumps users` −1.1% / −0.5% medium
    (\[−1.3, −0.5\], floor 0.5%: clears) and −0.7% / −0.7% small, `dumps mixed`
    −0.9% / −0.6% medium (\[−1.2, −0.3\], floor 0.7%: at the edge) and −0.6% /
    −0.3% small (inside), `dumps wide_arrays` −0.9% / −1.0% (\[−1.2, −0.6\], floor
    1.7%: inside); the parse rows +0.7% / −0.3% (`loads wide_arrays`), +1.7%
    (`loads mixed`, \[+0.0, +3.0\] against a 2.9% raw floor — inside), +0.9%
    (`loads users`, inside). macos-arm64 (an M1 VM): every row inside its floor
    (`dumps flat` +2.0% / +0.6% medium against 2.1% / 3.1%, the rest within
    ±1.4%). The destructor's source effect is zero or slightly favourable — a
    cold out-of-line routine shifting layout — and T1's acceptance clause is
    satisfied on two legs; the i7 leg's draw follows.
- - The i7 leg (the same run): every row inside this VM's 2.4–18% floors
    (`dumps flat` +0.4% / −0.2% medium and +1.3% / +2.1% small against 2.4–3.3%,
    `dumps wide_arrays` −3.9% / −3.0% against 5.7% / 4.9%, the parse rows
    −1.1..+0.6%). Three legs, no cost anywhere: T1's acceptance clause is
    satisfied.
- Outcome: reviewed not refuted; source-alone A/B clean on three legs;
  the follow-up (the vacuous assertion, two coverage cases, the bindings
  doc line) landed as a961a43; ready to integrate

## T2 — evidence gates on a declared workload

- - 2026-09-07 06:35 · author: an Opus worker · branch `exp/t2-evidence-gates`
    aed6fb6. `harness.validate_report` is the one validity and completeness
    check (ERROR rows, non-finite, negative or zero timings, unusable RSS, min ≤
    median ≤ p95, duplicate keys, unreadable rows kept as `Report.malformed`, an
    empty report, a declared row without a usable strata measurement — all
    fatal; extra and uncomparable rows disclosed); the workload is declared
    (`WORKLOAD_DATASETS`, `QUERY_LABELS`, `workload_rows()` = the 27 strata
    rows, `CI_PLATFORMS` = five legs) and verified against all eight committed
    reports. `supportability_check` requires strata in every declared row and
    category and fails an empty report (3.0x bound kept); `ci_summary` takes
    both denominators from the declaration, adds an Evidence section (complete /
    unverified / INCOMPLETE / INVALID / MISMATCH / MISSING per platform),
    cross-checks each report's commit and platform against `run_info.json`,
    exits 1 when evidence is missing or misattributed, and never lets an invalid
    platform shrink the goal; `regression_check` fails on zero matched entries,
    a baseline row absent from the candidate or a missing metric, reports new
    rows as ungated, keeps the thresholds; `ci_fetch` verifies identity and
    coverage before placing and swaps a staged replacement atomically (a failing
    second write leaves the previous set and `run_info.json` byte-identical);
    `make bench-check` runs the regression gate through the Make interface. 37
    new tests through the CLIs' exit codes; all eight false passes the review
    reproduced now fail; regenerating `ci_summary.md` from the committed reports
    reproduces 128/135 byte-for-byte except the Evidence section and one caption
    word. `make test` green (15/15, 2,111). Owed by the lead: the benchmarking
    docs for the new flags and exit codes.
- Review (`build/evidence/T2-REVIEW/REVIEW.md`, not refuted): every
  false-pass case closes with the documented exit code, the declared
  workload matches `bench_main.run` and all eight committed reports,
  ranking rules and thresholds are byte-identical, 128/135 reproduces.
  Required before T9 publishes from this summary: only declared
  platforms may count toward the headline and the tally (an undeclared
  `windows-arm64` report could read "Goal met on 6/5" or hide a MISSING
  leg behind "5/5"), section cells take their numerator from declared
  rows only, and `ci_fetch` must recover or name a fetch's leftover
  `.previous-*`/`.staging-*` siblings after a hard interrupt; non-blocking:
  qualify the Evidence closing sentence when provenance is unverified,
  a documented exit code for a placement `OSError`, one convention for an
  invalid report body across the CLIs, `BENCH_CI_FLAGS` — applied in a
  follow-up.
- - Follow-up (547ee70): only declared platforms count toward the headline,
    the tally and the section cells; leftover `.previous-*`/`.staging-*`
    siblings are recovered or named and refused; the Evidence closing sentence
    qualifies unverified provenance; a placement `OSError` has a documented exit
    code; one convention for an invalid report body; `BENCH_CI_FLAGS` reaches
    `make bench-ci`.
- Outcome: reviewed not refuted; follow-up landed

## T3 — one A/B estimator and a recoverable driver

- - 2026-09-07 06:35 · author: an Opus worker · branch `exp/t3-ab-estimator`
    1f5a58a. `benchmarks/ab_blocks.py` is the single analysis: chronological
    ABBA blocks validated from the launch order, normalised launch = strata
    median / the operation's rival in the same process (`RIVAL_BY_ENGINE`: dumps
    → orjson-bytes, loads → orjson-loads, file ops → their compositions), block
    effect = mean(B)/mean(A) − 1, aggregate = median of block effects,
    whole-block bootstrap (2,000 resamples, seed 42) for the interval and the
    A/A floor; raw strata, raw rival and normalised effect from one packet;
    `ab_floor.py` and `ab_builds.py --analyze` are views of it; misordered
    launches, a non-baseline tail, a third arm without `--pair`, missing rival
    rows and non-finite samples are refused. The driver persists every launch's
    samples as it goes, restores the original extension in a `finally` on both
    paths and verifies its hash, and refuses to run from a process that imported
    the target; `rows_probe.py` identifies the machine portably (no
    unconditional `os.getloadavg`) and gains real `load`, `ndload` and `dump`
    operations; `ab_x86.yml`'s perf loop measures bytes. 24 tests, a
    known-effect fixture under linear drift recovered to 1e-4. Recomputation of
    the saved packets: E26-P6's headline reproduces exactly (run 34065637472:
    `dumps wide_arrays` +7.01%, `dumps flat` +3.61% / +3.98%); E26-P7's
    confirmation draw re-reads with two corrections — small `dumps flat` −2.13%
    \[−2.71, −1.80\] clears its 1.28% floor (the ledger had called it at the
    edge), but on that draw small `dumps flat` and small `dumps wide_arrays`
    move because orjson slowed (+3.1% and +1.9% raw) while strata read +0.8% and
    −0.7% raw, so the flag's acceptance rests on the medium rows (`dumps flat`
    −2.4%, `dumps mixed` −1.7%, strata itself faster) and the two small rows are
    not evidence for it. Every saved A/A floor comes from two blocks; the
    workflow's A/A is raised to six before a small effect is certified on a
    floor.
- Review (`build/evidence/T3-REVIEW/REVIEW.md`, not refuted): an
  independent 30-line implementation of the statistic matches the
  recomputed tables on both packets to 0.005 pp; the rival table matches
  `bench_main`'s compositions; every rejection fires on crafted
  fixtures; the driver persisted six launches through a `kill -9` and
  restored the extension on both paths. Required before a deciding
  campaign: the driver must not overwrite an existing `.ab_original` (a
  crashed campaign's only copy of the original), one `min_samples`
  default across the views, and the workflow's A/A at the candidate's
  block count (a two-block bootstrap interval covers ~51%, six ~94%) —
  applied in a follow-up. The reviewer extended the E26-P7 re-reading to
  the FIRST i7 draw: its headline medium `dumps flat` −3.4% was entirely
  rival movement (raw strata +0.7%, raw rival +4.5%), so E26-P7's
  accepted magnitude is restated below.
- - Follow-up (5aa05d8, finished by the lead after the worker stalled for an
    hour with its edits half-applied; its worktree diff was taken as the base
    and the gates re-run): the driver refuses to run over a stale `.ab_original`
    whose hash differs from the target and restores from it first; one
    `DEFAULT_MIN_SAMPLES` across the four views; the workflow's A/A at the
    candidate's block count with a warning below four blocks; `--pair` lists
    dropped launches; the first i7 draw recomputed with raw columns
    (`build/evidence/T3-AB-ESTIMATOR/REPORT.md`, Follow-up): its medium `dumps flat` −3.4% is rival movement, its small `dumps wide_arrays` −4.0% is
    strata's (zero of six positive) against a 4.7% floor, so E26-P7's evidence
    stays the confirmation draw's medium record rows. 33 tests; fmt/lint clean;
    C++ 15/15; `tests/unit` 875 passed.
- Outcome: reviewed not refuted; follow-up landed; E26-P7 restated

## E26-P5 — integration and final standings

- Opened 2026-09-06 · owner: lead · reviewers: Opus reviewer and Opus
  measurement on the integrated candidate
- Candidate: 90dd3b7 + the P0 diagnostics and audit tests + E26-FIX1 v4
  (c4f7982, six comment fixups applied at integration) + E26-FIX2 (2df173f)
  - the E26-P2 profile patch (0747d73); `docs/context/api.md` reconciled by
    hand (the four-step mutation contract scoped below 64 levels of dict
    nesting, the `UnicodeEncodeError` clause). **Recomposed 2026-09-07 after
    E26-P5b: the profile patch is withdrawn** — the four recipe files are
    back at 90dd3b7 and the candidate is the source fixes under the shipped
    recipe; the window, roll and gates below that name the 48bb4952… build
    describe the first composition and are kept as its record, and the
    recomposed build's gates, window (E26-P5b W2, the A → C arm) and roll are
    in the "Recomposed candidate" bullet at the end of this entry.
- Gates on the integrated tree: `make fmt lint` and both pre-commit passes
  clean; `make test-cpp` 15/15; `make install` gated and `make test-py`
  2,073 passed; `make test-py-asan` passed in 68 s with zero reports; `make gate` passed (facade 100%); `make pgo` under the new recipe merged exactly
  one raw profile (ten gate profiles diverted) with both gates green on both
  phases — extension md5 48bb4952a671cbf8e3c075b749b743bd, `__text` 211,396
  bytes, profile md5 e9f5eeafe70ec6d4da7d7ab58d512d2c.
- Before/after window (`build/evidence/E26-P5/WINDOW.md`, raw under
  `raw/`): the shipped build (32c5fa4, shipped recipe) against the candidate,
  ABBA ×8 + A over 20 rows on two tiers, orjson in-process as the control,
  one AA block of the candidate against itself as the floor; the desktop in
  use throughout (load 7–12, WindowServer ~40%). Normalised by orjson:
  `dumps flat` −2.0% small / −1.6% medium, `dumps users` −2.5% / −1.9%
  (bytes; str reads better on three of the four cells),
  `dumps mixed` +0.5% / −0.1% (inside floors of 1.9% / 0.4%), `dumps nested` +0.6% / +1.1% (small inside its 1.6% floor; medium's floor 0.4%,
  eight of eight blocks positive at +0.5..+1.5% raw — a small consistent
  cost), `dumps wide_arrays` flat; parser rows (the recipe's effect alone,
  the code untouched): `loads flat` −0.5% / −1.0%, `mixed`/`nested`/`users`
  within ±0.6%, **`loads wide_arrays` +2.1% on both tiers** (raw +1.8% /
  +1.4%; normalised ratio-of-medians +1.9% / +1.8%; normalised per-launch,
  ABBA-block paired +2.13% / +2.10%, eight of eight blocks positive on both
  tiers; paired bootstrap \[+1.49, +2.70\] and \[+1.65, +2.76\] against A/A
  normalised floors of 1.31% and 1.16%, so the effect clears its floor — the
  P5 measurement review's recomputation,
  `build/evidence/P5-REVIEW-MEASUREMENT/recompute/`) — the parse-row lean the
  profile review predicted from `parse_array`'s de-inlining, at the campaign
  plan's +2% investigate-or-revert bar, not below it. The disposition is a
  deferral, not a pass: the five-platform samples decide it, and the profile
  recipe is the change to revert first. Every leg's current margin absorbs it
  (tightest, the Neoverse-N2 at 0.94x, would become 0.96x), which is an
  arithmetic extrapolation from one M1 window to four other targets, not a
  measurement on them; the arm64 runner had read the same document loop
  unchanged under the new recipe.
- Roll (`PGO_MODE=use STRATA_ENABLE_LTO=1 make bench-all`, load 3–5): small
  27/27, medium 26/27 (`dump flat` 1.00x, a rounded tie), large 27/27.
  `regression_check` breaches against the committed September-4 baseline
  (different revision, no recorded provenance) and against the small-tier
  before report of the shipped build taken on this host ten hours earlier, on
  source-untouched control rows as much as touched ones
  (`build/evidence/E26-P5/regression_vs_*.txt`) — the harness gate cannot size
  an effect on this host, as P0 established. It does not contradict the
  window: on `loads wide_arrays` the two agree in sign (+7.5% median against
  the window's +2.1%). The window is the estimate with drift control; the
  plan's fix-or-revert clause on that row is deferred to the five-platform
  samples, not discharged. The baseline is not refreshed.
- Disclosures the measurement review required (2026-09-07): the reading
  rule's intervals were computed after the fact with `benchmarks/ab_floor.py`
  by the reviewer, not by the lead; WINDOW.md's "AA floor" column is the RAW
  A/A effect and must not be read against the normalised effect beside it (on
  `loads wide_arrays` small the raw floor is 0.12%, the normalised 1.31%);
  E26-P2 supplies no floor for `loads wide_arrays`, `loads nested`, `dumps mixed` str or any medium row but `dumps flat`, `dumps mixed` bytes and
  `loads flat`, and where it does its floors come from a much noisier machine
  than this window; the 9-launch A/A here was taken in the quietest stretch of
  the night. PLAN.md predeclared `load wide_arrays` and `load users.ndjson`;
  `rows_probe.py` has no file-load op and they were not measured — the harness
  roll reads `load wide_arrays` +3.2% median / +8.6% p95 against the before
  report with no drift-free cross-check. The A/B confounds the source fixes
  with the profile recipe: E26-P2's same-source window reads the recipe alone
  at −9% on `dumps flat`, this one reads recipe + fixes at −2.0/−1.6%, and the
  ~7.5pp between them is unmeasured (FIX-v4.md: "No timing was taken") —
  E26-P5b below separates them. The three tier reports carry `commit: 90dd3b7`, the parent of the uncommitted tree they measured (the harness
  records `git rev-parse --short HEAD` with no dirty check), and their
  `compiler_flags` line echoes `PGO_MODE`/`STRATA_ENABLE_LTO` at report time
  rather than describing the binary (E26-P0 PROVENANCE.md §6): in this packet
  `before/` reads `-O3` and `after/` reads `(PGO)` although both binaries were
  PGO builds. `before/bench_results_{medium,large}.md` are copies of an
  earlier session's committed reports and were not compared;
  `before_baseline.json` covers the small tier only.
- Review (2026-09-07, `build/evidence/P5-REVIEW-CODE/REVIEW.md` and
  `P5-REVIEW-MEASUREMENT/REVIEW.md`): both not refuted. Code: the tree is
  exactly the five accepted branch diffs plus the declared fixups (24 of 31
  files byte-identical to a union worktree), every gate re-run green in the
  reviewer's own worktree (2,073 tests, ASan clean, 3,000-document
  differential identical to 90dd3b7), protections intact; three staging-plan
  corrections required and applied (`docs/context/benchmarks.md` dropped from
  commit 1, one subject for commit 4, `docs/decisions.md` staged whole in
  commit 2 because its eight additions are one unsplittable hunk).
  Measurement: analyze output and ranks reproduce byte-identically, every
  launch loaded the extension it claims; the `loads wide_arrays` wording above
  and the disclosures are its required changes, applied verbatim.
- - Recomposed candidate (2026-09-07, after E26-P5b): the working tree with
    `scripts/pgo_build.sh`, `pgo_build_clang_cl.py`, `pgo_build_msvc.py` and
    `docs/build-and-test/SKILL.md` restored to 90dd3b7 (33 status entries).
    `make pgo` under the shipped recipe: 11 raw profiles merged, C++ 15/15 and
    2,073 Python tests green on both phases, profile md5
    33bc4a07b591919ec32aba3bfe71a5d4 — the same profile arm C's build produced
    in the reviewer's worktree — and an extension (md5
    4bca2d1d0e6204d276b88b2c93027dc0, `__text` 215,060 bytes) whose `__text`
    section is byte-identical to arm C's (both f6938d66556f14b982c2a7c23976e5d6,
    with `__const`, `__cstring`, `__data` and `__unwind_info` identical as well;
    the file md5s differ only in the debug-map SO/OSO build paths, their
    timestamps and the LC_UUID), so E26-P5b's W2 window (A → C: the shipped
    build to this code) is the recomposed candidate's before/after: `dumps flat`
    −8.5% / −8.1%, `dumps users` −6.6% / −6.5%, `dumps mixed` −2.7% / −3.0%,
    `loads wide_arrays` 0.0% / +0.2%, the other rows not re-measured on this
    composition (the first composition's window read them within ±1.2% and the
    recipe was the only other variable). The source is unchanged from the
    reviewed composition, so the ASan gate, `make gate` and the differential the
    code reviewer ran stand. Roll (`PGO_MODE=use STRATA_ENABLE_LTO=1 make bench-all`, started 00:43 at load 4.3 as the compile load decayed; reports
    copied to `build/evidence/E26-P5/recomposed/after/`): small 27/27, medium
    27/27 (no rounded ties), large 26/27 on its first draw — `dump mixed` at
    1.25x of orjson with strata's minimum 0.550 ms under a median of 0.808, a
    spike on a 0.5 ms file-write row while the desktop's WindowServer and a
    renderer sat at ~40% CPU each — and **27/27 on a second draw** taken at
    01:01–01:07 at load 3.7–3.9 (`dump mixed` 0.92x, strata 0.56 ms against
    orjson's 0.61); the committed large report is the second draw, whole and
    unedited, the first is archived beside it
    (`after/bench_results_large_draw1.md`), and both are reported here so the
    second is not read as the only one. The harness's own in-process comparison
    against the first composition's reports agrees with the window in sign on
    every in-memory serializer row (`dumps flat` −2.9% / −6.5% / −5.6% small /
    medium / large, `dumps users` −4.6% / −7.9% / −3.8%, `dumps mixed` −17.8% /
    −8.3% / −0.6%, orjson within ±3% on those rows but +8.6% on medium mixed) —
    a harness reading, not evidence of size. `regression_check` against the
    committed baseline and against the shipped build's small-tier before report
    breaches on untouched rows as before (`loads flat` +9.5%, `query $[*].id`
    p95 +10.8%; `recomposed/regression_vs_*.txt`): undecidable on this host, the
    window is the evidence, the baseline is not refreshed.
- Published 2026-09-07 01:28 as 79fa3df (one commit of the whole recomposed
  tree; the plan file left uncommitted). Predeclared before any result:
  `benchmark.yml` run 34064158421 is the five-platform sample and run
  34064174240, dispatched 21 s later on the same SHA, is the confirmation run;
  `profile.yml` run 34064175618 carries the serializer instruments. Both
  samples are fetched whole with `ci_fetch --run <id>` and summarised with
  `ci_summary`; a 135/135 claim needs both to read it on this SHA.
- - Two five-platform samples on 79fa3df (2026-09-07, both archived whole
    under `build/evidence/E26-P5/ci_run1/` and `ci_run2/`; `docs/benchmarks/ci/`
    holds the second as the tooling last wrote it): run 34064158421 reads
    **132/135** — linux-x86_64 and macos-arm64 27/27, linux-arm64 26/27 (`dumps mixed` 1.04x), macos-x86_64 26/27 (`dumps mixed` 1.10x), windows 26/27
    (`dumps flat` 1.08x); the confirmation run 34064174240, dispatched 21 s
    later, reads **128/135** — macos-arm64 and macos-x86_64 27/27, linux-arm64
    26/27 (`dumps mixed` 1.01x), linux-x86_64 23/27 (`dumps flat` 1.02x, `dumps wide_arrays` 1.04x, `dumps mixed` 1.08x, `dump wide_arrays` 1.02x), windows
    25/27 (`dumps mixed` 1.08x, `dump mixed` 1.06x). The previous SHA 32c5fa4
    had read 129/135 and 134/135. The confirmation run does not confirm; no
    135/135 is claimed; the goal is not met on 79fa3df.
- - Reading, leg by leg. macos-arm64 (an Apple M1 VM on all three draws) reads
    the fixes as the M1 window predicted: `dumps flat` 0.94x → 0.83x / 0.85x,
    `dumps users` 0.73x → 0.66x / 0.66x, `dumps nested` 0.64x → 0.60x, strata's
    medians −20..−31% (the previous draw was a slower VM, orjson −11..−21% too).
    linux-arm64 (the Neoverse-N2): `dumps flat` 0.77x → 0.80x / 0.78x, `dumps users` 0.79x → 0.78x / 0.77x, the persistent `dumps mixed` 1.03x → 1.04x /
    1.01x, the parse rows unchanged (`loads wide_arrays` 0.94x on all three
    draws). x86 is the finding: on the two draws whose runner read the parse
    rows within ±1% of the previous sample — linux-x86_64 sample 1 (`loads wide_arrays` −0.3%, `loads mixed` +0.5%) and macos-x86_64 sample 1 (the same
    i7-8700B on every draw; `loads wide_arrays` +0.9%) — the serializer rows
    moved against strata with orjson flat: linux-x86_64 `dumps flat` +6.5%,
    `users` +3.2%, `wide_arrays` +5.6% (orjson −0.6%, −0.2%, +2.4%; ratios 0.78x
    → 0.83x, 0.83x → 0.86x, 0.85x → 0.87x), macos-x86_64 `dumps flat` +5.4%,
    `users` +2.0% (orjson −3.0%, −0.9%; ratios 0.77x → 0.84x, 0.75x → 0.77x).
    Sample 2's linux-x86_64 draw ran on a faster host (parse rows −19% / −10%)
    and its 1.02–1.08x serializer ratios are that machine's, not comparable with
    the previous sample; the Windows leg drew three different processors (Intel
    Model 207, Model 173, AMD Family 25) so its three draws are three machines.
    What survives the runner lottery: the source fixes cost the x86 serializer
    rows about +2–6% against orjson, the direction the arm64 host could not see
    (E26-P5b measured them at −6.6..−8.5% there). Opened as E26-P6; the
    linux-x86_64 profile leg's artifacts for 79fa3df are archived
    (`build/evidence/E26-P5/profile_run_34064175618/`: cachegrind at repeat 60,
    strata 1.38e9 instructions and 842k first-level instruction misses against
    orjson's 1.50e9 and 1.29M, so the x86 cost is not instruction count or
    i-cache in that simulation).
- - Correction (2026-09-07 02:30, on a reader's challenge that the moves are
    not x86-only): recomputed over every row and leg, the rows whose
    strata/best-rival ratio is worse than the 32c5fa4 sample by at least 0.03 on
    BOTH new samples are — x86 serializer: linux-x86_64 `dumps flat` 0.78x →
    0.83x / 1.02x, macos-x86_64 `dumps flat` 0.77x → 0.84x / 0.83x, windows
    `dumps flat` 0.80x → 1.08x / 0.86x and `dumps nested` 0.69x → 0.73x / 0.97x;
    **macos-arm64 parse rows**: `loads wide_arrays` 0.80x → 0.85x / 0.84x, `load wide_arrays` 0.80x → 0.84x / 0.84x, `load users` 0.66x → 0.71x / 0.71x,
    `search $..orders[*].total` 0.30x → 0.33x / 0.34x; file-write rows on three
    legs (linux-arm64 `dump mixed` 0.88x → 0.94x / 0.97x, macos-x86_64 `dump flat`/`mixed`/`nested`, windows `load mixed` 0.68x → 0.78x / 0.80x). The
    Neoverse-N2's in-memory serializer rows did not move at all (`dumps flat`
    0.77x → 0.80x / 0.78x, `users` 0.79x → 0.78x / 0.77x, `nested` and
    `wide_arrays` ±0.01) — it shows neither the M1's gain nor the x86 loss,
    which fits the register-budget mechanism (AAPCS64 on both arm64 cores) but
    not a "gain on arm64" claim; the sentence above that said the arm64 legs
    improved as predicted is true of macos-arm64's serializer rows only. The
    macos-arm64 parse rows are a second open signal: the parse source is
    unchanged, the M1 window read `loads wide_arrays` at 0.0% / +0.2% for the
    fixes under the shipped recipe, and what did change on every leg is the PGO
    profile itself — the recipe merges the gate suites' runs and the gate grew
    by about 180 serializer tests, so every leg's layout moved, parse code
    included. Two draws on shared M1 VMs cannot separate that from host
    variance; the macos-arm64 leg is added to the same-runner A/B (E26-P6) so
    both revisions are timed on one VM.
- Outcome: reviewed in its first composition; recomposed after E26-P5b and
  reviewed again (`build/evidence/P5B-REVIEW/REVIEW.md`, 2026-09-07: not
  refuted on the measurement, provenance, identity and roll all reproduced;
  seven reporting corrections required and applied — the duplicated E26-P2
  copy removed, the small `dumps mixed` cell marked inside its floor, the
  recipe's cost stated as +3.3–6.3% on the three rows measured, a
  negative-results row added, the packet's large report set to the committed
  draw); published; two five-platform samples taken (132/135, 128/135); the x86 serializer cost is E26-P6

## E26-P9 — reuse the fused writer for nested exact dictionaries

- Opened 2026-09-08 on `c9a337d`; not accepted.

- Named cost: native ARM64 hot profile 34146265191 places 8.48% in
  `write_mapping_body`. Exact dictionaries reached through the scalar
  dispatcher currently bypass the fused writer. Reuse the existing guarded,
  out-of-line path for those dictionaries, including root dictionaries.

- Preserve all existing fallbacks, staged mutation semantics, recursion limits
  and private leases. Add no cache state or duplicated scalar machinery.

- Decide with production-PGO mixed/flat/users/nested/wide controls, root/nested
  mutation contracts and sanitizers. Reject a gain below the matching floor
  or unresolved canonical regressions. Baseline binary/provenance retained.

- Local result: paired, matched-test production PGO ABBA (six blocks, 60
  samples, matched A/A) found small mixed bytes -2.21% (raw -1.44%, interval
  -3.17..-0.26%, floor 1.36%), medium mixed bytes -1.81% (floor 1.23%),
  and nested bytes -9.51% (raw -9.33%, floor 1.14%). Flat/wide effects stayed
  within their A/A floors. The two PGO workload-source and training-data
  manifests are identical; the runtime source delta is one dispatch call.

- Both canonical checks failed: initial 10-sample comparison had 15 metric
  breaches; a predeclared 60-sample confirmation had 18, despite both builds
  ranking 27/27 in that confirmation. Preserve all reports; no baseline
  replaced. The confirmation's failures include parser/query controls and
  process RSS, so the paired serializer gains do not resolve the gate.

- Outcome: **not accepted for production**. Reverted runtime/tests into
  `experiments/benchmark-nested-mappings.patch`; native investigation is
  selectable after publication. Correctness: both PGO phases passed 15 C++
  suites/2,249 Python tests, and candidate ASan/UBSan passed all 2,249 tests.
  Evidence: `build/evidence/benchmark-lead/p9/`.

- Native follow-up: both five-platform runs 34166567410 and 34186143209
  completed. Linux ARM64 raw small/medium mixed gains repeat at about 5–6%.
  macOS Intel also improves in both runs; Windows remains unresolved. Linux
  x86 medium's first normalized +2.21% regression does not repeat (+0.18%
  in the confirmation, within the 0.80% floor). All 20 binaries match their
  sidecars; all ten A/B pairs have matched test/training-source and data
  manifests. These are selected-row comparisons, not a canonical acceptance.

- Full-workload follow-up: a fixed new-session local small/medium/large
  sequence retains its reports in `p9/full-gate/`. Small and medium both
  rank 27/27 for each arm but fail unchanged regression thresholds, including
  mixed serialization and unchanged parser/query controls. Add native
  `validation=canonical` to measure every declared small-tier row on each
  runner before considering integration. The runtime patch remains isolated;
  see the [execution plan](benchmark-lead-plan-2026-09-07.md) for complete
  results and the next published-run configuration.

- Review (2026-09-10, independent of the author): **the patch as written has
  a correctness defect.** `write_record_fused` has no `open_` scan; the only
  cycle checks are `write_sequence`'s `std::find` before it dispatches an
  element and `write_mapping`'s `Frame::repeated()`. P9 sends every exact
  dict to the fused writer, so a cyclic dict whose shape is already prepared
  at the re-entry depth is emitted once more before the placeholder, and the
  bytes depend on the thread's schema-cache history. Confirmed by a
  differential test of seven shapes on plain gated builds of HEAD and the
  patch, each shape warmed three times (`build/evidence/benchmark-lead/p9/ cycle-defect/`): five differ — `{"name":"root","self":<self>}` reads
  `{"name":"root","self":null}` on HEAD and
  `{"name":"root","self":{"name":"root","self":null}}` with P9; a two-level
  self-reference, two mutually recursive dicts, a 24-key self-reference and
  an a→b→a pair likewise; the two list-mediated shapes are identical because
  the sequence loop's check still fires. `cycle_policy="error"` still raises,
  one level later. The 2,249-test run did not catch it because
  `test_config.py`'s cycle case never warms the shape at depth 2 first, and
  the patch's own test is a characterization test that passes unchanged on
  HEAD (verified). Repair if revived: hoist the `std::find` /
  `emit_cycle_placeholder()` pair to the fused writer's entry — one linear
  scan of a usually empty vector, but in the hot record loop, so every
  P9 number above must be re-measured after it. The reviewer also judged
  P9a's `#if defined(__linux__) && defined(__aarch64__)` dispatch
  unacceptable under the styleguide (a policy switch, not an implementation
  with a portable twin; compiled in on one of five legs). P14 was found
  correct (reservation 17 + 20 bytes, compact ints cannot run Python, bools
  excluded by the exact-type test, growth handled by `overflow`).

- Revived 2026-09-10/11 on `exp/p9-fused-dicts` as two commits with matched
  tests: (T) the mirrored cycle-contract tests (`test_dumps_cycles_fused.py`:
  six shapes, cold and warmed, three policies, both output types, plus
  records whose scalar prefix carries the output across the stage and block
  boundaries), the patch's mutation test and an emptied-to-scalars parity
  test — all of which the unchanged tree passes; (P) the dispatch, entering
  through `write_record_fused_value`, which probes `open_` before the row
  and hands a hit to `write_mapping`. Three ways of probing the element
  loop's records as well were built, measured and declined, each on the
  runners or on M1 PGO arms against the tests-only arm (six blocks of
  sixty, matched recipe and training data; evidence under
  `build/evidence/benchmark-lead/p9/native-*` and `p9/local-screen`):

  - deciding the first container in the verification pass so the emit loop
    arms by index (run 34545656328): `dumps flat` +16.7%/+13.9% on the N2,
    +20.1%/+16.4% on the EPYC, +12.6%/+10.5% on the M1 VM, +10.6%/+8.2% on
    the i7, +12.7% on Windows, six of six blocks past floors under 2%;
    `dumps users` N2 +5.2%; reproduced on the M1 (flat +14.3%/+13.2%);
  - the unconditional scan before the row (the review's plain-build shape),
    M1 PGO arms: flat and mixed inside their floors, `dumps users` +2.3%
    (six of six, floor 0.27%) — the five-deep scan on the item records;
  - a probe at the first container value with the record's bytes taken
    back through a mark on the staged output, M1 PGO arms: `dumps users`
    +4.0%/+4.1%, medium mixed +1.8%, flat inside its floor — the mark and
    the probe block are per-record work on three-field records.
    The dispatch measured against the first of those probes (run
    34545658395, matched tests): N2 `dumps mixed` −4.2%/−3.7% (small/medium),
    small file `dump nested` −2.7%, `dump mixed` −1.7%, users inside its
    floor, `dumps wide_arrays` +0.5% (floor 0.3%); EPYC `dumps mixed`
    −5.1%/−2.1%, users −4.1%/−4.1%, flat −1.6%/−2.0%, wide_arrays −8.0%,
    file `dump nested` −7.4%; i7 `dumps flat` +4.7%/+5.9% past 2.2%/0.7%
    floors, mixed inside floors; M1 VM medium flat +2.3% at its floor;
    Windows small `dumps mixed` +1.7% normalised (raw +1.2%, floor 1.4%), a
    second draw of that leg (run 34523596127) +2.0% (raw +0.6%).
    On the M1 itself that probe plus the dispatch moved no mixed row. The
    first dispatch (runs 34523593547, 34523596127) lost both Linux legs to a
    `NameError` in the harness's Linux-only CPU-name reader, fixed on
    `work/benchmark-lead` (3f3425b).
  - The branch as it stands (the value-path probe, the element loop unprobed)
    on the M1 against the tests-only arm (`p9/local-screen/local3_P3.tsv`
    with `local2_AA.tsv`; PGO arms, six blocks of sixty): `dumps users`
    −0.05%/−0.22% inside floors of 0.55%/1.20%; medium `dumps mixed` −1.53%
    (interval −1.77..−0.90%, floor 1.51%), small −2.84% unresolved (interval
    to +1.62%); small file `dump nested` −5.24% (−8.62..−1.04%, floor 3.50%);
    flat inside its floors; small `dumps wide_arrays` +1.28% (raw +0.84%,
    interval +0.53..+2.34%, floor 0.91%), the one resolved loss here, on a
    row that runs no probe.
  - **Native, the decisive draw** (run 34568043329, 6739050 against b490f81,
    `experiment=none`, six blocks of sixty, matched recipe and training data,
    every arm's binary matching its sidecar; evidence
    `p9/native-dispatch-vs-tests-34568043329/`). Normalised effect, then the
    interval and the leg's own floor. N2 (linux-arm64): `dumps mixed`
    **−5.77%** small (−7.44..−3.16%, floor 1.25%) and **−6.14%** medium
    (−6.78..−5.81%, floor 1.08%); `dumps users` −3.51%/−3.44%; small file
    `dump nested` −3.91%, `dump mixed` −1.37%; medium `dumps flat` −0.32%;
    the one resolved loss on any leg: small `dumps wide_arrays` +0.32% (raw
    +0.18%, floor 0.26%); the parse rows inside their floors. EPYC
    (linux-x86_64): `dumps flat` −4.53%/−4.77%, `dumps users` −1.82%/−1.14%,
    file `dump nested` −2.83%; `dumps mixed` −1.57%/+0.70% inside floors of
    1.78%/1.51%. M1 VM (macos-arm64): medium `dumps mixed` −1.97% (−5.54..−1.00%, floor 1.58%), medium `dumps flat` −1.72% (interval to +1.52%, floor 1.48%); nothing else past a floor of 1.5–8%. i7
    (macos-x86_64): small `dumps mixed` −3.66% (−4.46..−0.93%, floor 2.76%),
    file `dump nested` −3.45%; medium `dumps flat` +0.97% inside its 1.94%
    floor. Windows (Zen 4, clang-cl PGO): no row resolved outside its floor (medium `dumps users` +1.00% against a 0.89% floor, interval −3.89..+3.11%); small `dumps mixed` −1.32% normalised, raw −1.36%. On the two rows behind in
    the latest five-platform sample (N2 `dumps mixed` 1.023x, Windows `dumps mixed` 1.015x) this reads −6% and about −1.4% of strata's own time;
    whether the second flips is for the two samples of the integrated
    revision to say.
  - Branch review (2026-09-11; contract/docs and perf/style lenses complete,
    the correctness lens re-run after a session limit): six findings
    confirmed of ten, the four refuted ones (the test docstring, api.md's
    scope, the decision entry's standing, the wrapper's inlining growth) each
    traced on the artifacts. Applied: the M1 screen above was unreported;
    "13–20% on every runner" was 8–20% depending on the runner; a "1.5% on
    plain builds" figure had no evidence in the repository and is dropped;
    the emptied-to-scalars parity test re-entered only through a list
    element, so it now runs both routes and a container-valued re-entry pins
    the probe's hand-off to the general writer's frame; the wrapper's doc
    block had landed between the fused writer's own block and its function (a
    comment-only move; the object code is byte-identical). Recorded as the
    next runtime follow-up, not done now: the probe precedes the fused
    writer's fallback checks, so a value dict the fused writer rejects (a
    schema miss, a wide or retired shape, depth past 64) pays the probe and
    then `write_mapping`'s own frame scan — canonical rows hit their schemas
    and pay one, a dict chain deeper than 64 levels pays twice its O(depth)
    scan per level; the fix is to move the probe after the last fallback,
    gated by the caller, and re-measure.
  - Outcome: **accepted for integration on the measurement above**; merged
    as c20ac86 (2026-09-11). The element-loop gap stays recorded in
    docs/decisions.md (2026-09-11).

- Two five-platform samples of c20ac86 (runs 34590005443 and 34590027501,
  17 s apart, `benchmark.yml`, PGO on every leg; both archived whole under
  `p9/ci-<run>/`, the second placed in `docs/benchmarks/ci/`): **131/135**
  and **132/135**. The N2 reads 27/27 on both — `dumps mixed` 0.969x and
  0.952x, strata 0.062/0.060 ms against 0.066 ms on 75cfb42's sample with
  orjson unmoved at 0.063–0.065 ms: the row the campaign was opened on,
  closed by the code as the A/B predicted. Behind: linux-x86_64 `dumps mixed` 1.03x on an EPYC 7763 (strata 0.067 ms against 0.065 on the 9V74
  draw of 75cfb42's sample, orjson 0.065 against 0.070) and 1.06x on a 9V74
  draw whose every row, parse rows included, reads about 20% faster than
  that sample's (strata 0.053 ms, orjson 0.050) — a row at the report's
  resolution (0.001 ms on 0.05–0.07 ms) on hosts that differ, which the
  dispatch's own A/B read inside its floors with tests matched, leaving the added tests' profile shift (E26-P7b) as the one candidate still to price.
  Windows `dumps mixed` 1.03x/1.03x and file `dump mixed` 1.03x/1.03x on a
  Family 25 Model 1 host (75cfb42's draw was Model 17); macos-arm64 file
  `dump mixed` 1.06x on one draw and 0.86x on the other of the same M2 Pro
  VM class, strata 0.168 against 0.090 ms between them. No 135/135 is
  claimed.

- The profile shift priced (run 34594271053: the tests-only commit b490f81
  against 3f3425b, source unchanged, `experiment=none`, six blocks of
  sixty; a first dispatch, 34594107636, failed at checkout on a mistyped
  base commit and measured nothing). linux-x86_64: `dumps flat`
  +3.44%/+3.37% (small/medium, floors 0.66%), small `dumps wide_arrays`
  +0.70% (floor 0.28%), `dumps mixed` −0.02%/−0.18% inside floors of
  2.52%/2.58%, users inside. N2: `dumps flat` −1.90%/−1.80%, `dumps users`
  −2.48%/−1.68%, small `dumps wide_arrays` −0.68%, all resolved in strata's
  favour; `dumps mixed` inside floors. M1 VM and Windows: nothing past a
  floor. So the added tests move the profile as E26-P7b said tests do —
  against the EPYC's flat records, for the N2's — and do not touch the
  x86 `dumps mixed` row; the dispatch's own draw then reads that row
  neutral with tests matched. The x86 legs' `dumps mixed` standings of
  1.03–1.07x are rival and host movement at the report's resolution, the
  coin band the campaign has met on other legs before. The i7 leg, in last: no row resolved either way — medium `dumps mixed` +2.9% normalised on an interval spanning zero (−1.6..+4.4%), small `dumps mixed` −6.6% inside a 9.8% floor, flat and users inside theirs.

- Probe-placement follow-up (2026-09-11, evening; the review's "move the
  probe after the last fallback, gated by the caller, and re-measure"). Two
  designs, both on `exp/p9-probe-placement`'s tests-only commit 8012f43
  (three cycle cases the fused writer rejects — 25 keys, a `str` subclass
  key, a 70-level chain — mirrored into `test_dumps_cycles_fused.py`; they
  pass on main's build): 9ec69c0 makes `write_record_fused` a `template <bool kProbe>`, `write()` calling the probing instantiation and the
  element loop the other, the probe after the key-row match; 4329952
  (`exp/p9-probe-flag`) keeps one body with a runtime `probe` argument in
  the same place. Both are byte-identical to c20ac86 on every cyclic shape
  (the seven-shape differential and the 124 contract cases), both gates
  green on both PGO phases. **The template copy (9ec69c0): no-go.** M1
  screen against the tests-only arm (`p9/local-screen/local4_Q.tsv` with a
  fresh A/A floor, `local4_AA.tsv`; PGO arms, six blocks of sixty): `dumps mixed` +17.8% medium and +17.9% small, small file `dump nested` +35.3%,
  small `dumps users` +3.0% and medium +0.9%, all six of six blocks against
  floors of 0.7–5%; `flat` and `wide_arrays` inside theirs. Plain builds of
  the two commits are equal on every path-isolating shape (`path_probe.py`:
  records with one child dict 124.4 against 124.2 µs), so the cost is the
  profile's: under the gate-inclusive recipe the value-path copy trains
  mostly on fallbacks (the training corpus's per-record `extra` dict carries
  512 distinct keys, a way miss on every record, and the suite's cycle and
  mutation cases add theirs), and its success epilogue is laid out behind
  its fallback tail (`ret` at +2708 after the `write_mapping` jump at +2040,
  where the element copy and the old body read +1284 and +1416 before
  theirs); a `sample` of a mixed loop puts 16% of the run on that epilogue.
  Runner A/B 34620403974 (8012f43 against 9ec69c0, `experiment=none`, six
  blocks of sixty; evidence `p9/native-probe-placement-34620403974/`): N2
  `dumps mixed` +5.2% small and +6.0% medium, `dumps users` +2.9% and
  +2.4–2.9%, six of six against floors under 1%, `flat` −0.5..−1.0%; EPYC
  `dumps flat` +3.2..+3.6% and file `dump nested` +2.5% (six of six),
  `mixed` and `users` inside floors — E26-P6's x86 layout sensitivity again
  (the value copy compiled to 461 instructions against the element copy's
  747, `write()` 4,256 → 4,160); M2 Pro VM (macos-arm64) `dumps mixed` +16.1% small and +16.5% medium, file `dump nested` +26.4%, six of six against floors of 3.5–8%, `users` and `flat` inside theirs; i7 (macos-x86_64) `dumps flat` +2.9% medium and +1.3% small resolved, file `dump nested` −3.4% resolved, `mixed` inside; Windows (Zen 4, clang-cl PGO) file `dump nested` +2.7% resolved, every other row inside floors of 0.7–23%. Every arm's binary matches its sidecar and both arms trained on one recipe and one suite on every leg (`verification-all.json`). **The runtime flag (4329952): no-go.** One
  body of 932 instructions against 882 with nine more stack accesses — the
  flag is live across the verification loop of every record — and the M1
  screen (`local4_A.tsv`) reads `dumps flat` +3.0% medium and +2.2% small,
  `dumps users` +0.9% and +1.4%, medium `dumps mixed` +2.5%, six of six;
  `wide_arrays` inside its floor. Not dispatched.

  - Outcome: **closed as measured; main keeps the probe ahead of the fused
    writer's fallbacks.** What that costs is one O(depth) scan more on a
    value dict the fused writer rejects — a way miss, a wide, retired or
    `str`-subclass-keyed shape, a chain past 64 levels — and never a
    canonical row; what either fix costs is a canonical row. The three
    rejected-shape cases stay on the branch: a test addition alone moves
    rows under this recipe (E26-P7b), and they pin what the general
    writer's frame already does. docs/decisions.md (2026-09-11) and the
    negative-results table carry the reading.

## E26-P9a — restrict nested mapping fusion to Linux ARM64

- Scope: preserve P9's dispatch only for Linux ARM64; original dispatch on
  Linux x86, both Macs and Windows. No API, allocation or ownership changes.
- Evidence: full native P9 scores 27/27 on Linux ARM64 with mixed median
  -2.923%, but wide-array file dump p95 +2.014% and ID-query p95 +3.616%
  still fail. Other platforms have 16–32 metric breaches.
  Windows remains 25/27. This does not qualify the broad P9 for integration.
- Validation: the narrowed patch passes all 2,252 Python tests and 15 C++
  suites on the development Mac; the complete preprocessed Darwin serializer
  is identical to baseline. Predicate checks select the new path only on
  Linux ARM64. Full Linux execution of the narrowed revision is pending.
- Outcome: isolated, unaccepted patch with matched-test native workflow
  selection. Unchanged-source canonical control 34257791864 and profiling
  run 34257653984 are pending. The original Linux p95 failures remain failed;
  the platform restriction does not repair them.

### September 9 native follow-up

Narrowed canonical run [34265403380](https://github.com/PrimeLab-Foundation/strata/actions/runs/34265403380)
finished with all five regression gates failed. Linux ARM64 again reaches
27/27, but the candidate totals 132/135; this remains an unaccepted experiment.
Its three ARM64 failures are wide-array file dump p95 +30.043%, nested loads
p95 +10.201%, and mixed file load p95 +3.435%.

Raw sample inspection changes the tail investigation: wide-array file dump's
four slowest candidate rounds are 0–3 for Strata, orjson and msgspec alike.
Their p95 changes are +30.043%, +24.725% and +21.329%, respectively, while
medians change -0.748%, -0.864% and -0.030%. Nested loads also slows across
engines around rounds 8–15: orjson p95 +12.036%, msgspec +14.025%.
This supports shared interference, without proving the candidate harmless.
Mixed file load remains unresolved: Strata median +1.642% and p95 +3.435%,
versus orjson -0.146%/-0.027% and msgspec -1.014%/-0.103%.
No samples are dropped and no failed gate is waived. The derived audit is
retained with the downloaded canonical reports as `tail-rival-audit.json`.

Windows-only profile [34265407120](https://github.com/PrimeLab-Foundation/strata/actions/runs/34265407120)
passed. Its initial clang-cl PGO block, before the plain rebuilds, reports
mixed serialization at 1.1091x orjson in alternating pairs (interval
1.0909–1.1364), float subset 1.204x, string subset 1.240x, but records-only
0.967x. Homogeneous full-precision floats remain behind at 1.147x; most
homogeneous string buckets lead. Thus the next serializer experiment should
investigate heterogeneous scalar dispatch and full-precision float emission,
not assume record fusion resolves Windows.

File phase controls show mixed serialization 52.20 microseconds and
open/truncate 170.75 microseconds, but the canonical rivals also open and
truncate on every call. These Python composition controls do not establish
a Strata-specific opening overhead. Defer a file-opening rewrite until a
matched rival control or native profile identifies such overhead. Preserve
serialize-before-truncate and newline semantics in any subsequent prototype.

## E26-P10 — resume exact scalar runs after heterogeneous elements

- Hypothesis: the sequence writer stops specialized scalar emission after the
  first type transition. Resume its existing float and string loops at later
  absolute indices to avoid general per-element dispatch on subsequent runs.
- Contract and risks: see `experiments/benchmark-lead.md`. Twelve new cases
  pin separators at block boundaries, string fallbacks, non-finite floats,
  and the live-list contract after a callback replaces the tail.
- Validation: both matched-test gate-inclusive PGO builds pass 15 C++ suites
  and 2,260 Python tests. Build hashes and complete compilation verify;
  workload source manifests and training data match. The initial baseline
  attempt caught a new test oracle using escaped Unicode; correcting it to
  `ensure_ascii=False` restored the specified UTF-8 expectation before both
  measured builds. The failed attempt is retained separately.
- Six ABBA blocks of 60 samples plus six identical-binary A/A blocks, with
  trailing baselines, found no mixed gain: small/medium bytes raw +0.31%/+0.55%,
  normalized +0.87%/+0.33%, both unresolved against their intervals and floors.
  Nested bytes costs +1.51% raw, +1.61% normalized (interval +0.02% to +2.24%,
  floor 1.38%); wide-array bytes +1.37% raw, +1.03% normalized (interval
  +0.47% to +1.55%, floor 0.83%). Parsing control raw +0.12% is unresolved.
- Outcome: no-go on the development Mac; no full canonical or native run is
  justified by this screen. This does not measure Windows performance. Keep
  the prototype and its tests only in
  `experiments/benchmark-resume-scalar-runs.patch`; production source and
  extension are restored. Evidence, both PGO profiles, binaries, raw TSVs,
  controls and analysis remain under `build/evidence/benchmark-lead/p10/`.

## E26-P11 — direct digit count for long Dragonbox significands

- Mechanism: count 16/17-digit binary64 significands using direct thresholds;
  retain the generic counter below 10^15. The maximum width follows from the
  existing Dragonbox binary64 conversion contract, not the benchmark dataset.
- No conversion, rounding, output, allocation or Python ownership changes.
  Existing independent float-format reference tests cover full-writer output.
- Matched gate-inclusive PGO and paired screening are complete. Keep
  `experiments/benchmark-float-digit-count.patch` isolated; it is not qualified.
- Both PGO arms passed 15 C++ suites and 2,248 Python tests in both phases;
  complete compilation and binary hashes verify, with identical workload
  source manifests and training data. Evidence is retained under
  `build/evidence/benchmark-lead/p11/`.
- Six 60-sample ABBA blocks plus six identical-binary A/A blocks: small/medium
  mixed bytes raw -2.74%/-2.25%, normalized -2.09%/-1.08%. These bytes gains
  remain unresolved: small interval -2.55% to +3.55%, floor 2.24%; medium
  interval -2.01% to -0.30%, floor 1.31%. Preserve the small row's +7.60%
  normalized block; no outlier removal. Medium str mode resolves -1.51%
  normalized against a 1.38% floor. Wide-array bytes raw +0.11%, normalized
  +0.86% (interval +0.34% to +1.02%, floor 0.44%) is an adverse control.
- Full local small-tier canonical validation (60 samples, all 27 rows)
  ranks both arms 27/27 but fails ten unchanged regression checks: mixed
  dumps median +2.3% / p95 +12.0%, users dumps median +4.2% / p95 +21.7%,
  wide-array dumps p95 +3.1%, wide-array file dump p95 +3.3%, mixed loads
  p95 +4.5%, mixed file load p95 +4.3%, nested file load p95 +2.4%, ID query
  median +3.3%. Reports and failures remain in `p11/canonical/`.
- Outcome: no-go for integration. The native A/B workflow has an isolated
  `float-digit-count` selector for investigation after publication; no native
  run has measured this patch. Both refs must use the same published revision.
  Production source and the original extension/metadata are restored. Do not
  rerun unchanged local comparisons looking for a passing draw or substitute
  selected-row gains for the failed canonical gate.

## E26-P12 — value-preserving string identity control

- Inspection: all 750 mixed-data string values are ASCII and contain no
  escapes. Removing duplicate escape scans would not address this row.
  There are 258 distinct values but 502 distinct string objects, unlike the
  synthetic buckets that reuse a small set of string objects.
- Added `make probe-string-identity`: original and cloned full trees, pooled
  full tree, original string leaves and pooled string leaves. Both tree
  controls clone containers; pooling only changes equal string-value sharing.
  Per-engine byte equivalence is checked before timing. Fixture creation is
  untimed; shared alternating rounds retain all samples, ordering, exact
  build identity and object counts. Pooling reduces 502 objects to 258.
- Local production PGO, 60 pairs: pooled/original extracted strings measure
  0.8536x for Strata (interval 0.7486–0.9286) and 0.8889x for orjson
  (0.8432–0.9709). Full pooled/cloned trees measure 1.0639x for Strata
  (1.0156–1.1682) and 1.0672x for orjson (1.0117–1.1042).
- Outcome: these controls move both engines and do not establish a
  Strata-specific instruction or cache bottleneck. Do not add string pooling
  or prefetching to production based on them. The Windows profile workflow
  now saves the same diagnostic before any plain rebuild, beside its PGO
  binary and file controls. Native measurement awaits publication.
- Evidence: `build/evidence/benchmark-lead/p12/string-identity.json` and log.
  No production hot path or canonical benchmark protocol changed.

### P11/P12 native results on 971fe87

Both [A/B 34315373617](https://github.com/PrimeLab-Foundation/strata/actions/runs/34315373617)
and [Windows profile 34315375872](https://github.com/PrimeLab-Foundation/strata/actions/runs/34315375872)
completed successfully. All ten A/B binaries match their manifests and have
complete compilation. Workflow success is not a passed canonical gate:
this run selected paired validation, and P11's local canonical failure stands.

Small mixed bytes raw changes: Linux ARM64 -0.462%, Windows -0.830%,
Linux x86 -1.395%, Mac ARM64 -0.923%, Mac Intel -2.381%. None resolves a
small-row gain against both its interval and A/A floor. Windows normalized
-0.492% has interval -0.929% to +1.384%, floor 2.530%; Linux ARM64 normalized
-0.271% has interval -0.583% to +1.476%, floor 0.987%. Medium mixed resolves
small normalized gains on Linux ARM64 (-0.614%, floor 0.427%) and Linux x86
(-1.465%, floor 1.416%), but these do not prove the remaining small-row deficits
closed. Windows medium is particularly noisy (15.213% floor). Windows small
users str mode moves adversely: raw +1.48%, normalized +5.23%, beyond its
interval and floor. No production integration is justified by these results.

Windows P12 pooling finds no resolved extracted-string gain for Strata:
0.9951x (0.9417–1.0612), versus orjson 0.9590x (0.9234–0.9793). The pooled
full tree is slower than its unpooled clone for Strata: 1.0747x
(1.0162–1.1192), versus orjson 1.0078x (0.9914–1.0267). This does not support
pooling user strings or attributing the deficit to duplicate string objects.
Artifacts are retained under `native-float-34315373617/` and
`windows-strings-34315375872/` within `build/evidence/benchmark-lead/`.

## E26-P13 — first-key-first fused schema lookup

The initial PGO block of Windows profile 34315375872 measures mixed paired
1.1316x orjson (interval 1.1232–1.1409), records-only 1.157x, strings 0.771x,
and full-precision float bucket 1.041x. This differs from the prior profile's
subset ordering; the older string deficit is not a stable optimization target.
The log is retained in `windows-strings-34315375872/profile.log`.

P13 reverses the fused lookup's size/key comparisons. Three of the four
mixed schemas share a size, while their first keys differ, so first-key-first
may reject wrong ways without loading counts. The same two conditions still
must hold; no ownership or output behavior changes. Both matched PGO arms passed 15 C++ suites and 2,249 Python tests in both
phases. Manifests confirm identical training sources/data and complete
compilation; saved binaries match their hashes. Disassembly differs at only
16 instruction words, all in the four cache-way comparisons; every instruction
address and the 219,240-byte text-section size is unchanged. The intended
load-order change is real, without a surrounding code-layout change.

Six ABBA blocks of 60 samples plus six identical-binary A/A blocks find no
mixed bytes gain: small raw +0.13%, normalized -0.03% (interval -1.08% to
+2.03%, floor 1.45%); medium raw +0.06%, normalized -0.49% (interval -1.28%
to +0.06%, floor 0.68%). Flat, nested, users and wide-array controls provide
no reason to advance this mechanism. No-go: no full canonical or native run
is justified by the local screen. Production source, binary and metadata
are restored. Evidence is retained under `build/evidence/benchmark-lead/p13/`,
and the prototype remains in `experiments/benchmark-schema-key-first.patch`.

## E26-P14 — combine cached-key and compact-integer reservation

Published checkpoint 4933c08 contains the prior negative experiments. P14
moves the exact compact-integer case into the fused record emitter, reserving
key-slot scratch plus the integer writer's maximum window once. Existing
row verification, reentrant ownership and all non-compact fallbacks remain.
Two fresh-thread output-growth cases supplement the existing numeric and
mutation suites. Matched-test PGO validation is complete; the prototype
is isolated in `experiments/benchmark-record-int-reserve.patch` and evidence
is under `build/evidence/benchmark-lead/p14/`. Results follow.

P14 local screening: both PGO arms pass 15 C++ suites and 2,251 Python tests
in each phase; exact binary hashes, complete compilation and matching training
inputs verify. Six ABBA blocks of 60 samples plus six identical-binary A/A
blocks resolve flat-record bytes raw -7.88%, normalized -7.51% (interval
-8.10% to -7.16%, floor 1.17%); str raw -6.88%, normalized -6.50% (interval
-7.37% to -6.22%, floor 1.71%). Small/medium mixed bytes raw -0.38%/-0.66%
remain unresolved. Full small-tier canonical validation is complete. The
native `record-int-reserve` selector applies the isolated patch and gives
both PGO arms its two boundary cases; use the same published revision for
both refs. Native measurement and full acceptance are still outstanding.

P14 full local small-tier report (60 samples) ranks both arms 27/27 and
preserves the flat dumps median gain (-6.85%), but fails 23 unchanged checks.
Mixed dumps median +3.0% / p95 +10.7%, nested dumps p95 +5.8%, and several
parser, file and query timings breach; whole-process RSS increases by
5.4–7.5% in multiple rows. All failures are retained in
`p14/canonical/gate-small.txt`; they are not waived as noise. P14 remains
unaccepted. The substantial flat-record gain justifies native investigation
with full validation, not production integration. ASan/UBSan validation of the
new reservation path passes all 2,251 Python tests. Production source, tests,
binary and metadata are restored; the prototype remains only in its patch.

### Native P14 validation and same-source control audit

[P14 run 34346286916](https://github.com/PrimeLab-Foundation/strata/actions/runs/34346286916)
uses b7abe5079fd4bf1df8da5c854c667f3665f26c08 for both refs,
`record-int-reserve`, canonical validation and 60 repeats on all five
platforms. Both arms receive the same regression tests. It is an isolated
candidate investigation, not a clean production standings run.

The previously completed
[same-source control 34257791864](https://github.com/PrimeLab-Foundation/strata/actions/runs/34257791864)
has now been fully downloaded and audited. All ten binaries match their
manifests, compilation is complete, and both arms have matching PGO recipes,
workload sources and training data at 5bbec773f53580b4adf1f1c6c86c5e01d725cfe3.
These are separate PGO builds of the same source, not an identical-binary A/A.
The unchanged gates fail 30 checks on Linux ARM64, 28 on Linux x86, 10 on
Mac ARM64, 48 on Mac Intel and 11 on Windows. Within-run standings total
132/135 for A and 131/135 for B; Windows moves 25/27 to 24/27, Linux ARM64
stays 26/27, and the other three stay 27/27.

Mac Intel mixed dumps median changes +51.99% for Strata, +42.50% for orjson
and +44.60% for msgspec; nested dumps changes +85.64%, +66.37% and +54.05%.
Linux ARM64 mixed changes +9.99%, +8.00% and +8.51%. The shared movement
supports runner variation as a contributor, without attributing every failed
metric or waiving any candidate gate. Evidence and verification are retained
under `build/evidence/benchmark-lead/native-control-34257791864/`.

P14 native validation is complete. All ten saved binaries match their
manifests and complete compilation; both arms have identical training sources,
data and recipes on each platform. Every canonical gate fails. The table
contains raw Strata median changes within each A/B run, not noise-adjusted
effects or production standings.

| Platform       | A / B wins | Failed checks | Flat dumps median | Mixed dumps median |
| -------------- | ---------- | ------------- | ----------------- | ------------------ |
| Linux ARM64    | 26 / 26    | 2             | -5.81%            | -4.02%             |
| Linux x86-64   | 26 / 27    | 8             | -3.88%            | -10.83%            |
| macOS ARM64    | 27 / 27    | 12            | -17.68%           | -8.93%             |
| macOS x86-64   | 27 / 27    | 31            | +4.54%            | -1.53%             |
| Windows x86-64 | 25 / 24    | 20            | -9.15%            | +2.42%             |

Both arms total 131/135. Linux ARM64 mixed still trails orjson at 1.0222x;
its failed checks are mixed dumps p95 +7.27% and ID query p95 +4.56%.
The mixed median's raw improvement accompanies orjson -4.47%, so it does
not close the deficit. Windows mixed still trails at 1.0417x, with median
+2.42% and p95 +30.17%; mixed file dump also trails and wide-array file
dump becomes a third miss. Mac Intel flat dumps regresses despite the gain
on the other four platforms. Full reports, gates, binary verification and
raw effects are retained under
`build/evidence/benchmark-lead/native-record-int-34346286916/` (`audit.json`).
No-go for integration. Regenerating `ci_summary.md` still yields the clean
production result 133/135 from run 34143751498 at 75cfb42, unchanged.

## E26-P15 — combine cached-key and short ASCII reservation

P14's native Linux ARM64 result still trails on mixed serialization. The
mixed input's outer cached records contain 375 integer fields and 500 string
fields. Test a separate reservation/emission shortcut for exact compact ASCII
values, starting from production without P14. The existing copy-until-escape
primitive handles the clean value; an escape hit rolls back the quote and
delegates to the original writer. Combined scratch remains within the existing
reservation cap. No schema kind prediction or ownership policy changes.

Two fresh-thread cases cover both output modes, full-width key rows, buffer
growth, the combined reservation boundary, escapes, Unicode and long strings.
The first baseline attempt exposed a test-oracle mismatch: the shared helper
uses stdlib's ASCII escaping default, while Strata emits UTF-8. The new test
now explicitly uses `ensure_ascii=False`; the failed attempt is retained as
`p15/pgo-A-failed-oracle.log`. No timing used that failed build. Both measured
arms use the corrected identical tests. The architecture proof is in
`docs/architecture/fused_record_writer.md`; the prototype is isolated in
`experiments/benchmark-record-ascii-reserve.patch`. Validation is in progress.

Both matched PGO arms pass 15 C++ suites and 2,251 Python tests in both
phases. Exact binary hashes, complete compilation and identical training
sources/data verify (`p15/verification.json`). Six ABBA blocks of 60 samples
plus six identical-binary A/A blocks show no mixed bytes gain: small raw
+0.49%, normalized +0.97% (interval -1.24% to +1.72%, floor 0.87%); medium
raw +0.75%, normalized +0.23% (interval -0.04% to +1.28%, floor 1.41%).
Users bytes regresses raw +2.89%, normalized +2.57% (interval +1.92% to
+3.64%, floor 1.60%); users str is also slower, normalized +2.08% (interval
+1.47% to +2.76%, floor 1.08%). Flat bytes improves raw -5.32%, normalized
-5.63% (interval -6.20% to -4.33%, floor 0.91%), but does not justify the
users regression or advance the remaining mixed-data deficit.

Disassembly shows the fused writer growing from 883 to 1,214 instructions,
while its stack frame shrinks from 192 to 176 bytes. Static frame-relative
instruction count falls from 66 to 51; this is not dynamic spill attribution.
The larger body supports retaining the code-footprint risk, without proving
the cause of the users regression. Both disassemblies and `codegen.json` are
retained beside the timing evidence. No-go: no full canonical or native run
is justified. Production source, tests, binary and metadata are restored;
`make test` passes all 15 C++ suites and 2,249 Python tests.

## E26-P16 — share the string writer after the combined reservation

P16 removes P15's duplicate clean-copy and escape fallback arm. The existing
private string writer accepts a default-false `pre_reserved` argument;
only the fused exact-short-ASCII caller passes true after its combined
reservation. The same growth, escape and Unicode tests apply. The baseline
is the retained P15 A build (539e264fa47f1e1ca388eb97006a64494fb018b2ab93e4e38f0457c6feee230a),
with identical test sources and training data verified against the new PGO
candidate. This is a separate prototype from production, not a composition
with the integer shortcut. Evidence is under `build/evidence/benchmark-lead/p16/`;
the isolated patch is `experiments/benchmark-record-ascii-shared.patch`.

The candidate passes 15 C++ suites and 2,251 Python tests in both PGO phases;
saved binaries and matched training inputs verify. Six ABBA blocks of 60
samples plus six new identical-binary A/A blocks show small mixed bytes raw
-0.04%, normalized +0.70% (interval -1.04% to +1.20%, floor 2.18%); medium
raw +0.10%, normalized +0.17% (interval -0.59% to +1.34%, floor 0.79%).
Users bytes still regresses raw +2.36%, normalized +2.07% (interval +1.31%
to +2.33%, floor 0.59%); str raw +2.08%, normalized +1.94% (interval +1.55%
to +2.16%, floor 0.64%). Flat bytes improves raw -5.76%, normalized -5.47%
(interval -5.83% to -4.39%, floor 0.56%). No mixed gain resolves.

Sharing source does not reduce the generated body: the fused writer grows
further to 1,388 ARM64 instructions versus P15's 1,214 and baseline's 883.
Its frame remains 176 bytes, with 55 static frame-relative instructions.
The intended code-footprint reduction is therefore falsified for this build.
No-go: retain the isolated patch and do not dispatch full canonical or native
validation. Production source, tests, binary and metadata are restored.

## E26-P17 — profile the actual native interleave before another runtime change

P14 through P16 do not close the mixed-data deficit. Reviewing the existing
ARM64 workflow shows that its retained instruction annotations are selected
from hot calls; the other cold record uses a synthetic 64 MB cache sweep,
whose samples are dominated by copying. Neither annotates the five-encoder
interleave that reproduced the remaining deficit. The existing x86 interleave
report does not fill that ARM64 evidence gap.

Add focused `scope=arm64-interleaved` profiling with a PGO build and two
20,000-round conditions: all encoders resident but only Strata invoked, and
all five invoked in canonical dumps order, with GC before each call. Archive
complete reports and top-eight Strata symbol annotations, raw samples, actual
binary/hash/manifest, CPU/tool facts and each child's provenance. Use software
CPU-clock sampling explicitly; this is instruction-location evidence, not a
cache-counter or canonical performance result. No production runtime changes.
Three tests pin complete C++ clone names and both warmup/GC/call-order controls.
Local `make test` passes 15 C++ suites and 2,252 Python tests; workflow shell
syntax checks pass. Native collection is the next step before selecting a
new runtime mechanism. No scalar-reservation prototype is reintroduced.

The first dispatch, 34353507880 at 4bb8acc, was cancelled during its PGO
build, before sampling: review found the legacy profile helper used a
different encoder order, msgspec entry point and stdlib formatting from the
canonical harness. The worker now uses `bench_main._dumps_callables` directly;
its tests pin the exact warmup/loop sequence, msgspec Encoder entry point,
and compact stdlib separators. This is a protocol correction before evidence
collection, not a rerun selected from unfavorable performance results.

Corrected run 34354095181 at 2b28b82 passed the PGO build and correctness
stages but failed before collection because the runner lacked `rg` for perf
binary discovery. No timing or sampling result was produced. Replace that
dependency with a Bash glob and verify CPU-clock recording immediately after
tool installation, before building. Archive the built extension and PGO input
manifest even if later collection fails. The failed run log is retained.

P17 run 34354876762 at adca5de succeeds on Neoverse-N2. Both conditions
use the same verified PGO binary (4020b53544fd9597c206df2e168029219f0cd5b36a128b60c809a484a393611b),
matching dataset hashes and canonical callables. CPU-clock collection retains
132,792 resident and 701,108 interleaved samples, with zero lost samples.
The loops take 44.22s and 233.80s respectively; those totals include GC and
rival calls and are not Strata latency comparisons. Full artifacts and
verification are under `build/evidence/benchmark-lead/native-interleaved-34354876762/`.

Per-function annotation counts (resident -> interleaved): general write
1,578 -> 1,691; fused record 871 -> 884; mapping body 231 -> 231; string
writer 218 -> 198. One pair does not establish a resolved function-level
regression. In the general writer, address c180 is the compare following
the dependent power-of-ten load in Dragonbox significand digit counting;
its local sampled-period share is 2.09% -> 3.67%. That location supports a
bounded codegen/performance experiment, not a cache-miss attribution. The
fused writer's type-check locations remain prominent in both conditions.
The collector also now retains printed symbols whose global percentage
rounds to 0.00%; a displayed zero did not mean zero captured samples.

## E26-P18 — bit-indexed significand digit-count buckets

Use one bit-indexed pair of base decimal count and threshold after Dragonbox,
instead of the fixed-point multiply and dependent threshold-index sequence.
This is independent of P11's 16/17-digit branch and keeps generic integer
counting and the micro-decimal fast path unchanged. The private constexpr
1 KiB table is validated against a division-based oracle at all 64 interval
endpoints. Output stores, rounding and format layout are unchanged. The
larger table is a cache-footprint risk, so codegen and matched PGO paired/A/A
screening precede any acceptance claim. Architecture proof:
`docs/architecture/float_digit_count.md`; isolated patch:
`experiments/benchmark-significand-buckets.patch`.

Both fresh PGO arms pass both test phases (15 C++ suites and 2,252 Python
cases per phase). Binary identities and matching recipe, training data and
workload-source manifests verify. ARM64 codegen replaces the target's 12
instructions with 9, including a paired threshold/base load; the disassembled
format_double body decreases from 1,020 to 1,015 instructions.

Six paired ABBA blocks, repeat 60, with six matching identical-binary A/A
blocks show medium mixed bytes -1.25% raw / -1.14% normalized
(CI -1.61..-0.84%, floor 0.59%) and str -1.10% / -0.96%
(CI -1.42..-0.69%, floor 0.60%). Small mixed bytes reads -0.82% raw /
-1.36% normalized (CI -1.49..-0.68%, floor 0.83%); str -0.91% /
-0.88% (CI -1.38..-0.11%, floor 0.87%). Small intervals overlap their
control floors, so their gains are not fully resolved. No control row has
a resolved regression. This supports one full canonical small-tier gate;
it is not production qualification. Evidence: `build/evidence/benchmark-lead/p18/`.

The full canonical small-tier screen (repeat 60, all 27 rows) fails 38
checks: examples include users file-dump median +5.5% and p95 +9.8%, nested
dumps RSS +12.1%, and flat loads median +5.4%. Rival timings also move
(e.g. flat file-load orjson 0.695 -> 0.732 ms alongside Strata
0.605 -> 0.638 ms), and RSS is shared across libraries in each dataset
report. These observations do not attribute every failure to this code,
but do not waive any gate. No-go for integration; retain the local medium
mixed gain as diagnostic evidence only. No favorable rerun or native
qualification is claimed. Production source, binary and metadata are restored.

## E26-P19 — sequence type snapshot

The P17 f494 sample is a GOT load of PyDict_Type, not an ob_type reload.
The general write entry already caches Py_TYPE. P19 instead tests sharing
one per-item type snapshot across the heterogeneous sequence loop's plain
scalar check, exact-dict dispatch and general writer entry. No Python callback
lies between capture and dispatch; no snapshot survives a call or iteration.
Design: `docs/architecture/sequence_type_snapshot.md`. Extra argument/register
pressure is the main risk. Fresh matched PGO and codegen checks precede timing.

Both P19 PGO arms pass both phases (15 C++ suites, 2,252 Python tests).
Binary hashes verify and recipe, workload sources and training data match.
A is b1c501990691990fb840282e091fee6ec70a2992c542b15fa32b3eadc31c7edc;
B is 88c141a7100ec57238a060f755fde72d387226cc971ab7df6910c1d184317686.
The supplied-type entry removes its ob_type load, but the general writer
increases from 3,001 to 3,016 ARM64 instructions and its frame from 208 to
224 bytes. The fused writer increases from 883 to 884 instructions, with
its 192-byte frame unchanged. This is a measured register/code-size tradeoff,
not evidence of a timing win. Evidence: `build/evidence/benchmark-lead/p19/`;
patch: `experiments/benchmark-sequence-type-snapshot.patch`.

Six paired ABBA blocks at repeat 60 plus six identical-binary A/A blocks
show no resolved gain. Small mixed bytes is +0.77% raw / +0.77% normalized
(CI +0.02..+3.04%, floor 1.42%); str +1.30% / +0.91%
(CI -0.41..+3.28%, floor 2.43%). Medium mixed bytes is +0.47% raw /
+0.82% normalized (CI -0.07..+1.69%, floor 1.28%); str +0.44% /
+0.66% (CI +0.24..+1.06%, floor 1.55%). The adverse estimates are not
resolved beyond the control floors. No other screened row shows a resolved
gain. No-go: no full canonical or native run justified. The removed type
load does not establish a speedup, and P17's sampled global type load remains.
Production source, binary and matching sidecar are restored.

## E26-P20 — explicit sequence dict-type alias

Test whether expressing the global PyDict_Type address as a const local before
the sequence loop changes register allocation. This does not change lifetime,
callbacks, output or dispatch order. Unlike P19 it adds no writer argument.
Use P19 A's frozen PGO profile for a codegen-only comparison before spending
on fresh training or timing. A local alias may optimize identically: the
compiler already knows the global address, and may deliberately rematerialize
it to avoid another live register. Patch:
`experiments/benchmark-sequence-dict-alias.patch`; evidence:
`build/evidence/benchmark-lead/p20/`. No speed claim from this build.

P20 result: identical 55,503 address/instruction-word pairs across the full
local extension disassembly, using the same verified profile hash
22c39c1fecd901cf95158fe6c89945295f08ae00e8940410112c11c12448dc5e.
The test-gated build passes. No timing run is justified for identical code;
retain as a no-op result and restore production.

## E26-P21 — out-of-line sequence traversal

Following P20's identical codegen, separate sequence traversal from the general
writer using its existing STRATA_NOINLINE_HOT macro. The hypothesis is reduced
dispatch footprint/register interference, at the cost of a call per sequence.
No behavior, state or allocation change. Design:
`docs/architecture/sequence_outline.md`. Use fresh PGO for the changed CFG;
P19's unmodified A is the baseline only if training identities still match.

The first P21 build failed before training: STRATA_NOINLINE is not defined
in this translation unit. Use its existing portable STRATA_NOINLINE_HOT
macro instead. Retain the compiler-failure log; no timing was collected.

Corrected P21 passes both PGO phases (15 C++ suites, 2,252 Python tests).
Recipe, workload sources and training data match P19 A; candidate hash
53c8f7ac9a3b29cd6294781f6ea59ee10eceba2762ed2b21ffbfbd48686de87f.
The general writer shrinks 3,001 -> 2,089 instructions and frame 208 -> 160
bytes. Sequence traversal is 521 instructions with a 128-byte frame; fused
record remains 883 instructions with a 192-byte frame. The loop compares the
item's type directly against x26 at 4878, removing the in-loop GOT load seen
in baseline at 1b30/30d0. This establishes the proposed codegen mechanism,
not its net timing benefit; the extra sequence call still needs measurement.

Six paired ABBA blocks (repeat 60) and six matching identical-binary A/A
blocks show no resolved mixed gain: small bytes +0.82% raw / -0.98%
normalized (CI -2.59..+2.85%, floor 3.39%), str +0.90% / +0.76%
(CI -1.01..+1.63%, floor 1.96%); medium bytes +0.41% / +0.67%
(CI -0.21..+1.20%, floor 2.14%), str +0.69% / +0.60%
(CI -1.06..+2.29%, floor 2.58%). Users str regresses +1.08% raw /
+1.29% normalized (CI +0.93..+1.46%, floor 0.26%); bytes is
+1.38% / +1.57% (CI +0.70..+2.53%, floor 1.37%). No-go for integration;
no full canonical/native run justified. The codegen mechanism is real but
insufficient: shrinking the writer and hoisting this global load do not
outweigh the changed call boundary. Production is restored.

## E26-P22 — remove redundant fused-row verification

The compact combined-unicode gate requires dk_nentries == ma_used before
verification; this excludes holes. Schema selection already matches the
first key, and cache publication preserves first_keys == key_row\[0\]. Omit
per-value null checks and the first key's duplicate check; still copy the
entire row before output and verify every tail key. Proof and CPython source:
`docs/architecture/fused_verification.md`. Mirrored contract tests cover
holes/reinsertions, one-key records and differing tails sharing a first key.
Fresh PGO arms include the same tests. P17's verification loop contains the
redundant branches; instruction samples motivate measurement, not speed claims.

Both PGO phases pass in both arms (15 C++ suites, 2,256 Python tests).
Binary and training identities verify. A hash:
f81dbdd90c3cd0ae10fced51cfe3c796c1b0edee26a1b07f3fdb48edbae4577f;
B hash: c1ed94b8ba2d2c9d08198751255f66b426f3f66436d9c7c3b5351184d5e0368e.
The fused writer is 883 -> 886 ARM64 instructions, with its 192-byte frame
unchanged. Removing loop checks is not the same as shrinking the full body.
Evidence: `build/evidence/benchmark-lead/p22/`; isolated patch:
`experiments/benchmark-fused-tail-verification.patch`.

Six paired ABBA blocks at repeat 60 plus six identical-binary A/A blocks
show no resolved mixed effect or adverse control effect. Small mixed bytes
is -0.37% raw / +0.73% normalized (CI -0.13..+2.25%, floor 1.26%);
str -1.84% / -0.12% (CI -1.07..+0.57%, floor 2.52%). Medium bytes
-0.49% / -0.42% (CI -1.57..+0.85%, floor 0.66%); str -0.82% /
+0.09% (CI -1.60..+0.99%, floor 1.02%). Proceed to a full canonical
small-tier local report and a native diagnostic comparison: the removed
branches occur in the P17 N2 profile, and a neutral Mac screen does not
establish the target platform's effect. Neither step waives canonical gates.
The workflow selector uses identical base/candidate revisions so both include
the new regression tests, with only the runtime patch applied to B.

The full local canonical small-tier gate compares 27/27 rows and fails six
checks: mixed dumps p95 +10.0%, nested dumps p95 +3.3%, mixed loads median
+2.2%, wide-array loads p95 +2.9%, wide-array file-load p95 +4.4%, recursive
total query p95 +2.2%. Integration remains disallowed. Native paired results
will answer the target-platform mechanism only and cannot replace these
failures or qualify production. Sanitizer validation is in progress.

P22's instrumented candidate passes the ASan+UBSan binding gate: 2,256
Python tests, 73 seconds. Source is restored afterward; the developer's
production extension is unchanged. No memory-safety failure was detected,
but performance qualification is still blocked by the six canonical checks.

Native run 34443158775 failed at checkout because actions/checkout treated
abbreviated cand_ref 94ae9ad as a branch/tag. No build or timing occurred.
The failure log is retained. Corrected run 34443212708 pins both refs to
94ae9ad60dff752afde297a37e8797bbaadcf2a4 (full SHA), six paired/A/A blocks,
repeat 60, experiment fused-tail-verification:
https://github.com/PrimeLab-Foundation/strata/actions/runs/34443212708

Native result (2026-09-10, all five legs, `ab_blocks.py` with each leg's own
six-block A/A floor; every arm's binary matches its sidecar, both arms name
94ae9ad, recipes and training inputs match; `verification-all.json` beside
the artifacts). On the target N2 the patch resolves only small gains on the
record rows: small `dumps users` −1.22% normalised (interval −1.42..−1.11%,
floor 0.15%), medium `dumps flat` −0.55% (−0.73..−0.28%, floor 0.30%),
medium `dumps mixed` −0.53% (−0.88..−0.45%, floor 0.40%); small `dumps mixed`
−0.53% is inside its 1.27% floor. On both x86 legs it costs the flat record
rows, and the raw strata column says it is strata slowing, not the rival:
linux-x86_64 small `dumps flat` +5.38% (raw strata +5.37%, interval
+4.90..+6.43%, floor 0.72%, 6/6 blocks), medium +5.08% (+4.21..+5.51%, floor
1.40%, 6/6), small file `dump flat` +3.10% (+2.69..+3.24%, floor 0.45%, 6/6);
windows-x86_64 small `dumps flat` bytes +3.36% (raw +3.31%, interval
+3.10..+3.62%, floor 2.15%, 6/6). The two macOS VMs resolve nothing (floors
1.7–19%). Outcome: **no-go** — under 1.5% on the row it targets and a 3–5%
x86 cost in the fused writer's flat-record path, the same SysV-sensitive code
that E26-P6 priced. The hypothesis is closed; the patch stays isolated.

## E26-P23 — compact general-kind keys tables

**Hypothesis.** A `strata.loads` → `strata.dumps` round trip pays 25–39% over
the same data parsed by stdlib `json`, and the cause is a keys-table layout,
not the serializer's algorithm. CPython 3.11–3.14 gives a dict one of two
combined layouts: `DICT_KEYS_UNICODE` (16-byte `{key, value}` entries) and
`DICT_KEYS_GENERAL` (24-byte `{hash, key, value}`). `_PyDict_NewPresized` —
which the builder calls for every record whose depth hint is above five
members (`python_builder.h` `new_mapping`) — always returns a *general* table,
whatever the key types, and a general table never converts back (CPython
clamps `dictresize`'s `unicode` parameter on a general source, so `.copy()`,
`dict(d)`, `{**d}`, `update` and delete-then-reinsert all preserve it).
`rawdict::entry_array` refused that kind outright, so every such record lost
both the raw walk and the fused record writer while its `json.loads`-built
twin kept both. The `> 5` boundary is CPython's own presize no-op
(`minused <= USABLE_FRACTION(PyDict_MINSIZE) == 5` short-circuits to
`PyDict_New`), measured on 3.11.15/3.12.3/3.13.12/3.14.7 rather than derived —
neither constant is in an installed header.

**Mechanism.** On the branch that already returned nullptr, one `cold`,
out-of-line pass copies the table's `{key, value}` pairs into a per-lease
25-entry scratch (`SchemaCacheLease::State::general`, one slot past
`kMaxSchemaKeys` so `write_mapping`'s `too_many` break still fires in-loop),
and both hot walks then read the single 16-byte shape they always read. The
fused writer's arm triages `ma_values`, the kind and `dk_nentries == ma_used`
before reading one 24-byte entry, so a holed or over-wide general dict costs
the three loads it cost before and the copy is bounded by the dict's *size*,
never by a stale `dk_nentries`; the hole-tolerant, 25-capped form serves
`write_mapping`, whose own walk was `dk_nentries`-long already. The kind test
is `== DICT_KEYS_GENERAL` exactly and `ma_values != nullptr` is tested first,
because a split table's kind is `DICT_KEYS_SPLIT` and `DK_IS_UNICODE` is true
for it. Gated by a runtime layout proof, now a two-bit mask resolved at import
(`prepare_dumps_runtime` forces `proved_layouts()`, not `available()`).

**Three designs rejected before this one**, all recorded in the
negative-results table of docs/performance/SKILL.md:

1. *A runtime entry stride (16 or 24) instead of a compaction.* The stride is
   live across the fused writer's per-key verification loop — the loop E26-P6
   showed is one value from losing `this` to the frame, and where the E26-P9
   probe-placement follow-up priced a carried flag at `dumps flat` +2.2–3.0%
   (M1, `local4_A.tsv`). Worse on x86-64: 24 has no scale-3 encoding.
2. *A second, per-layout instantiation of the fused body.* This is the E26-P9
   template experiment again: a second instantiation trains cold under the
   gate-inclusive PGO recipe (`dumps mixed` +5–18%, file `dump nested` +35% on
   the M1). A cold twin of `write_mapping`'s classification loop would also be
   a second definition of behaviour, which convention.md forbids.
3. *A producer-side fix — make `loads` build unicode-kind tables.*
   `_PyDict_FromItems` is the only exported presized-*unicode* constructor and
   is `PyAPI_FUNC` only on 3.13/3.14 (a plain `extern`, local symbol, on
   3.11/3.12), so it fixes two of the four affected versions and does not link
   on MSVC for the other two; it is last-wins only, so three of four
   `duplicate_key_policy` values cannot use it; and it would cost the parse
   path the `loads flat` −5–7% the presize is worth, to fix a serializer-side
   mismatch.

**A/B protocol and result.** The canonical suite cannot demonstrate this fix:
every benchmark row builds its `dumps` payload with `json.loads`
(benchmarks/bench_main.py:371), so no canonical row contains a general-kind
dict. The evidence is therefore a path probe, and the canonical A/B is proof
of no harm. Probe (micro-benchmark protocol, docs/context/benchmarks.md): 500
records of *w* keys built both ways in one process, `strata.dumps(bytes)`, min
of 200 with `gc.collect()` per iteration, three readings per width; the ratio
is within-process, hence comparable across builds. Strata-built / json-built,
main → this change: **w5 0.99 → 0.99** (below the presize boundary: the
control that identifies the mechanism, and it must not move), **w6 1.42 →
1.07**, **w11 1.35–1.45 → 1.12–1.15**, **w24 1.42 → 1.04**. The earlier PGO
arms read the same shape (M: w5 1.00, w6 1.33–1.44, w11 1.42–1.44, w24 1.45;
P23: w5 0.97–1.00, w6 1.08–1.13, w11 1.13–1.19). The residual is expected: a
general record still pays the compaction and reads 50% more cache lines.

**M1 PGO screen** (build/evidence/benchmark-lead/p23/, arms M = main 34f1805
and P23 = 8c30efc, both PGO+LTO under the gate-inclusive recipe on Python
3.14; `p23.tsv`, eight rows, six ABBA blocks of sixty, against the fresh A/A
floor `p23_AA.tsv`): every row inside its floor except `dumps flat` — medium
str +1.1% and small +1.2/+2.1% normalised, 6/6 — where raw strata reads
+0.4..+0.9% and orjson −0.2..−1.8% in the same launches, i.e. mostly rival
movement at this resolution. `mixed`, `users`, `nested` and `wide_arrays` are
inside. Runner A/B (run 34665612473, the tests-only arm ca5346c against
8d10320, `experiment=none`, six blocks of sixty, every arm's binary matching
its sidecar): the change is a gain on both Linux legs and a **loss on
Windows**. N2 `dumps flat` −1.50%/−1.73% (medium/small), `dumps mixed`
−1.21%/−1.92%, `dumps users` −0.53%, all resolved against floors under
0.5%; against it, small `dumps wide_arrays` +0.52% and `loads wide_arrays`
+0.28%, resolved at the report's resolution. EPYC `dumps flat` −2.05%/−1.96%,
`dumps mixed` −2.32%/−1.40%, `dumps users` −1.88%/−2.03%, file `dump flat`
−1.24%, nothing against. Both Macs resolve nothing either way. Windows (Zen 4,
clang-cl PGO, CPython 3.12): medium `dumps users` **+2.46%** and small `dumps mixed` **+4.32%**, six of six blocks against floors of 1.25% and 1.46% — on
the leg whose `dumps mixed` is one of the rows still behind.

The Windows mechanism, read from the two arms' own profiles (both uploaded
with the run): the training workload's serializer payload is
`strata.loads`'s, so on the candidate every one of its records is a general
table and the training run records **2.4 M counts inside the compaction**
(`compact_general_*`, zero on the base), while `write_mapping`'s raw-walk
block goes from 0.98 M to 1.74 M counts because those records stop taking
`PyDict_Next`. The `cold` attribute says the compaction is rare and the
profile says it is hot; block placement believes the profile. The divergence
is identical on all five legs — the profiles are deterministic and match to
the count — so what differs is only how each toolchain spends it, and
clang-cl spends it on the hot unicode path that the benchmark rows measure.
This is the E26-P7b/E26-P8 coupling once more, now inside a single source
change.

- Outcome: **not merged.** The source change is what it claims on three of
  five legs and the round trip it exists for is fixed (1.45x → 1.08x at six
  keys, 1.43x → 1.14x at eleven, 1.45x → 1.05x at twenty-four of the
  json-built twin), but a resolved loss on Windows `dumps mixed` is
  disqualifying while that row is behind. The next arm pairs it with the
  training payload built by `json.loads` (E26-P8's b1 arm, neutral on its
  own): the workload then trains the writers on the same table kind the
  benchmark measures, the compaction's counters fall to the handful its
  retained strata-parsed arm produces, and the attribute and the profile
  agree. Branches kept.

- The pair measured (run 34670240916, `exp/p23-b1-tests` 56478bc against
  `exp/p23-b1` c4d7260: the same source change with the training payload built
  by `json.loads`, tests and payload matched between the arms, six blocks of
  sixty). The mechanism check first, on the candidate's own training profile:
  `compact_general_*` falls from 2.4 M counts to about 24 k — the handful its
  retained strata-parsed arm produces — and the fused writer trains on
  json-built records again (152 k). The rows follow. **The Windows
  disqualifier is gone**: small `dumps mixed` reads +1.15% inside a 5.44%
  floor (raw +0.90%) where the unpaired change read +4.32% resolved, and
  medium `dumps users` halves to +1.69% bytes (inside its 1.73% floor) and
  +1.44% str (resolved against a 1.31% floor) — the one Windows row still
  against, on a row strata leads by a wide margin rather than one of the two
  it trails. N2: `dumps mixed` **−1.31%/−1.19%** and small `dumps users`
  −0.18%/−0.21% resolved gains, and the unpaired arm's two losses (small
  `dumps wide_arrays` +0.52%, `loads wide_arrays` +0.28%) are gone, both
  inside their floors. i7: medium `dumps users` −2.41% and small `dumps flat`
  −1.25% resolved gains, nothing against. M2 Pro VM: small file `dump wide_arrays` −2.91% gain, nothing against. EPYC drew a loaded runner (its
  own A/A floors reach 10–13% on the record rows), so its −2.5..−5.6% figures
  on `dumps mixed` and `dumps users` are unresolved; its one resolved figure
  is small file `dump wide_arrays` +1.27% against a 0.97% floor. The M1
  screen of the same pair against its own A/A floor resolves nothing against
  it and reads small `dumps wide_arrays` −0.98%/−1.06% and medium `dumps mixed` −0.80% in its favour. The round trip the change exists for is
  unchanged by the pairing: 1.42x → 1.08x at six keys, 1.41x → 1.14x at
  eleven, 1.45x → 1.05x at twenty-four of the json-built twin, and five-key
  records (below the presize boundary) stay at 1.00x.

  - Outcome: **the pair is the candidate for integration** — the source
    change and the training payload together, because the payload is what
    keeps the compaction cold in the profile and neither half stands without
    the other on Windows. Two residual resolved losses, both marginal and
    both on rows strata leads: Windows medium `dumps users` (str) +1.44% and
    EPYC small file `dump wide_arrays` +1.27% on a loaded draw. Merging is
    the user's call; two five-platform samples follow it.

- Two five-platform samples of the merged revision ec53f93 (runs 34681733282
  and 34681741789, 14 s apart, PGO on every leg; archived whole under
  `build/evidence/benchmark-lead/p23/ci-<run>/`, the second placed in
  `docs/benchmarks/ci/`): **133/135** and **131/135** — the best standing
  recorded. The first has four platforms at 27/27, linux-x86_64 among them:
  its `dumps mixed`, behind on both samples of c20ac86, is #1. The second
  keeps both Linux legs and the i7 at 27/27 and adds three coin-band cells
  (macos-arm64 `dumps flat` 1.01x, Windows file `dump nested` 1.00x, both at
  the report's resolution). One row is behind on both draws and on the same
  host (Family 25 Model 1): **Windows `dumps mixed`, 1.06x and 1.08x** —
  strata 0.076/0.075 ms against orjson 0.071/0.069, a stable 6 µs on 500
  records, about 12 ns per record. Windows file `dump mixed` reads 1.01x with
  the same 6 µs inside a 386 µs row, so the file phase is not the gap: the
  serialization is. No 135/135 is claimed; the goal now rests on one row of
  one platform.

**Disassembly method and codegen.** Each ISA compiled at the flags its legs
build with, from a `git archive main` tree and the branch tree in turn:
arm64 `clang++ -O3 -std=c++20 -DNDEBUG -march=native`, x86-64
`clang++ -target x86_64-apple-macos -O3 -std=c++20 -DNDEBUG -fomit-frame-pointer -march=x86-64-v3` (setup.py:419-437 — a Darwin build
sidecar records the universal2 command, which is neither leg's). Instructions
per function counted from `-S` output; loops compared as opcode plus operand
shape, registers normalised, so "identical" means identical modulo register
renaming. Result (main → this change): `write`, `write_sequence`,
`write_mapping_body`, `write_mapping_uncached` **+0 on both ISAs**;
`write_record_fused` 328 → 342 arm64 and 311 → 322 x86-64; `write_mapping`
331 → 340 and 324 → 338; `dumps_to_python` +7 and +4. Frames: arm64 unchanged
(144 bytes in both writers); x86-64 `write_record_fused` 88 → 72 bytes with
44 → 41 frame-relative accesses. Loops: arm64 `write_mapping`'s collection
loop verbatim in all ten regions and the fused writer's loops likewise except
one earlier fold of the entry array's base address; x86-64 the same sequences
apart from frame-slot renumbering, with `write_mapping`'s collection loop
*better* than main's — `_PyBool_Type` moves out of a GOT memory operand into a
preamble-hoisted register.

**What the review changed.** (a) The accessor no longer takes the scratch as
an argument: that shape cost the *unicode* path one dead load per record in
`write_record_fused` on both ISAs and two in `write_mapping` on x86-64 (a
frame reload of `this`, then the field), because the argument had to exist
before the branch. Each writer now keeps `entry_array` inlined as before and
hands its `nullptr` edge to a `cold` member that finds the scratch off `this`.
(b) `[[unlikely]]` on the layout-proof refusal: without it clang merges the
two refusals and lays the merged epilogue in the fallthrough of that branch,
so the unicode path takes a branch where it used to fall through and the first
dict-layout load moves ~150 bytes further from the function entry. With it the
placement is main's. (c) `schemas_.resize` moved behind a `cold` `grow_schemas`
member: the inliner had pulled `std::vector::resize` — its `__append` call and
a `DepthSchemas` destructor loop — into `write_record_fused`, which was most
of the +22 instructions the compaction was first credited with. (d) Every
import-time probe in python_rawdict.h carries `STRATA_COLD_FN`: on ELF that is
what moves a body to `.text.unlikely.` (verified on a minimal TU at the same
clang), and ~3.3 KB of once-per-process code was sitting in hot `.text`
between `Serializer::write` and the rest of the writer cluster on the two
Linux legs.

**Outcome: go.** Merged on the branch with its tests; the round-trip ratio is
the evidence, the canonical A/B and the five-leg runner A/B are the no-harm
gates.

## E26-P24 — the fixed cost of opening a dict

**Hypothesis.** The one canonical row still behind on any platform is Windows
`dumps mixed` (1.06x and 1.08x on two samples of main: strata 0.075 ms against
orjson 0.069 on 500 records). `benchmarks/decompose_dumps_mixed.py` localised it
to the nested values rather than the records or the scalars, and the
nested-container probe (`exp/win-mixed-probe`, a14ecf2; the copy used here is
retained as `build/evidence/benchmark-lead/p24/nested_container_probe.py`, which
extends that commit's 14 rows to 18) then priced one container at a time: the
extra cost per record of a *one-key nested dict* is strata 45.0 ns on Linux
x86_64 and 59.2 on Windows against orjson's 23.9 / 28.7
(`build/evidence/benchmark-lead/mixed-2026-09-12/linux_x86_probe_all.log`,
`windows_probe_all.log`), while on this M1 the same figure is 23.8 against 19.5
(computed from `p24/nested/nested_ab.log`'s A arm, which is main's source: the
`value-dict1` row minus the `scalars-only` row, for each library) — strata's cost
to **open** a container grows about 2.5x from arm64 to the Windows build while
orjson's grows 1.5x, and per-key cost beyond the first is at parity or better
everywhere (on the M1 a *second* key inside that dict costs strata 15.8 ns
against orjson's 28.9). The first cut of this entry quoted an M1 pair of
25.8 against 26.6 for the same row; that pair appears in none of the retained
evidence and is withdrawn in favour of the two numbers above. The probe also ruled out the obvious suspects:
the two dict writers are within 2 ns of each other on every leg (so it is not
either writer's choice), a value whose type rotates per record costs nothing (so
it is not the type ladder), and lists are strata's strength. So: remove fixed
work from the path that opens a dict, portably, with byte-identical output.

**Mechanism — four trims.**

1. `rawdict::available()` is a guarded function-local static, and both dict
   writers consulted it per dict. It is resolved once in the `Serializer`
   constructor into a member, so the hot path loads a field off a `this` it
   already holds instead of a guard byte and then a value.
   `prepare_dumps_runtime()` still forces the proof at import; the constructor
   is in any case a *safer* place to resolve it than mid-walk, because it runs
   before `write()` and therefore outside every frame and staged row (the
   FIX1-REVIEW hazard). It is also a per-*call* cost against a per-*dict*
   saving, which the per-call rows below price rather than assume.
2. `open_.size()` and `schemas_.size()` become plain `size_t` members,
   `open_count_` and `schema_depths_`, each maintained by the one function that
   mutates its container — `push_open`/`close_container` and `grow_schemas`. The
   depth check every container pays, the growth check every dict pays and
   `latch()`'s loop bound are then one load and one compare instead of a
   vector's two-pointer size computation. `grow_schemas` grows monotonically and
   re-reads the vector's size, so a stale counter cannot shrink a table that
   live frames hold rows of. The cycle probes still read `open_`'s iterators,
   which they need anyway — **and so does `write_record_fused_value`'s emptiness
   probe.** That probe was moved to the counter in the first cut of this change
   and moved back by the review: `!open_.empty()` compiles into the very
   two-pointer load and compare `std::find`'s own begin-vs-end entry test
   performs anyway, a counter is a field clang cannot fold with that test, and
   the probe paid a load and a branch of its own for nothing — `write()` grew
   +2 instructions on arm64 and +3 on x86-64, on the path this change exists to
   speed up. The counter is for the depth checks only.
3. The opening brace is folded into the first key's reservation. `emit_slot_key`
   stores `{` where a later key stores `,`, inside the same 17-byte window, so
   `ensure(1)` + `put('{')` — one capacity check, one store, one size update,
   and on the fast path ten arm64 / eight x86-64 instructions — disappears from
   every record. The window is unchanged, so the str-mode contract (a small
   constant that fits the stage) is unchanged. Its precondition is recorded as
   an invariant in `docs/architecture/fused_record_writer.md` and is now an
   `assert` in each emit loop, so every debug and sanitizer build checks it: a
   zero-width dict has no first key to carry the brace and must not reach a slot
   loop.
4. Inside the same emitter the size update is taken **before** the bytes are
   stored. The slot copy writes through a pointer no compiler can prove disjoint
   from the output object's own fields, so an advance placed after it re-reads
   the buffer reference and its cursor: two loads per key, in both dict writers
   on arm64.

Both writers emit through the one `emit_slot_key`, which is what keeps them
byte-identical. It is a plain `inline` member: the `STRATA_INLINE_HOT`
(`always_inline`) this shipped with was withdrawn by the review because it is
inert — with and without the attribute Apple clang 21.0.0 emits **byte-identical
assembly for the whole translation unit** in six configurations
(`-O3 -march=native`, `-O3`, `-O2`, `-Os`; arm64 and the x86-64 leg's flags), and
`emit_slot_key` has no out-of-line symbol in any of them. The macro is gone from
`python_types.h` with a note saying why there is no third placement macro. The
`[[likely]]` on `index != 0` went with it: it is false on *every* iteration of a
one-key record — the shape this change targets — and it made clang move the
brace store out of line, costing `write_record_fused` three x86-64 instructions
for nothing on arm64.

**Codegen.** Standalone `-S` of `python_dumps.cpp`, arm64
`-O3 -march=native -arch arm64` and
`-target x86_64-apple-macos -O3 -fomit-frame-pointer -march=x86-64-v3`, both with
`-std=c++20 -DNDEBUG` (a release compile: the two new `assert`s are stripped, and
the ASan extension, which compiles without `-DNDEBUG`, runs them). Apple clang
21.0.0. **Counting rule**, stated so the absolute numbers are reproducible and
not only the deltas (the first cut's were not): a symbol's region runs from its
label to the `.cfi_endproc` that closes it, so every block clang laid out inside
the symbol counts, cold ones included; an instruction is a line in that region
that is not a directive, a label, blank or a comment; `_OUTLINED_FUNCTION_*` are
separate symbols and are never folded into a caller; frame bytes are the
prologue's own `sub sp, sp, #N` / `subq $N, %rsp`, with a pre-indexed
`stp …, [sp, #-N]!` reported as `+N`. The script and both ISAs' assembly are
retained (`build/evidence/benchmark-lead/p24/codegen/`).

Instructions / frame bytes, per symbol, for main, the first cut (`head`, 7155c33)
and the shipped source after the review's three withdrawals (`fixed`):

| symbol                   | arm64 main | arm64 head    | arm64 fixed   |
| ------------------------ | ---------- | ------------- | ------------- |
| `write`                  | 281 / 80   | 283 / 80      | **281** / 80  |
| `write_sequence`         | 451 / 144  | 449 / 144     | 449 / 144     |
| `write_record_fused`     | 342 / 144  | 293 / **160** | 293 / **160** |
| `write_mapping`          | 340 / 144  | 314 / 144     | 314 / 144     |
| `write_mapping_body`     | 568 / 144  | 600 / **160** | 600 / **160** |
| `write_mapping_uncached` | 84 / 96    | 84 / 96       | 84 / 96       |
| `Serializer::Serializer` | inlined    | 67 / 0 (+48)  | 67 / 0 (+48)  |

| symbol                   | x86-64 main | x86-64 head  | x86-64 fixed |
| ------------------------ | ----------- | ------------ | ------------ |
| `write`                  | 184 / 24    | 187 / 24     | **184** / 24 |
| `write_sequence`         | 400 / 88    | 393 / 88     | 393 / 88     |
| `write_record_fused`     | 322 / 72    | 276 / 72     | **273** / 72 |
| `write_mapping`          | 338 / 72    | 301 / **56** | 301 / **56** |
| `write_mapping_body`     | 566 / 104   | 608 / 104    | 608 / 104    |
| `write_mapping_uncached` | 115 / 24    | 115 / 24     | 115 / 24     |
| `Serializer::Serializer` | inlined     | inlined      | inlined      |

Four cells of the first cut's table
were wrong and are corrected above: `write_record_fused`'s arm64 frame
(144 → **160**, published as unchanged), `write_mapping`'s arm64 delta (−26, not
−16 — the published arm64 figure was the x86 one), its x86-64 delta (−37, not
−26) and its x86-64 frame (72 → **56**, unreported). Every other delta
reproduces; the absolute counts do not, because the first cut's counting rule was
not recorded, which is why this one is. `write_record_fused`'s x86-64 delta
becomes −49 rather than −46 once the `[[likely]]` comes out. The sentence "spill and reload counts are unchanged in every function
on both ISAs except `write_mapping_body`" was false and is replaced by the
measured traffic: arm64 `write_record_fused` 14 → **15** frame stores (17 loads
unchanged), `write_mapping_body` 20 → **25** loads, everything else unchanged;
x86-64 `write_record_fused` 34 → **31** loads, `write_mapping` 35 → **31**,
`write_mapping_body` 49 → **60**, `write_sequence` 0 → 2 stores. The commit
message of 0afc011 and the first `docs/decisions.md` line for this entry say
"every frame size unchanged"; three frames moved — arm64 `write_record_fused` and
`write_mapping_body` +16 each, x86-64 `write_mapping` −16 — and the log's line is
superseded by a later one rather than rewritten, the log being append-only. The
commit message stands as written: this branch's three commits are the base of a
running five-platform A/B and are not rewritten for a correction the ledger and
the log can carry.

**Which trim did what**, from three intermediate builds compiled the same way
(`main`; `reorder_only` = main plus trim 4 only; `nofold` = the shipped source
minus trim 3), arm64 / x86-64 instructions:

| step                   | `write_record_fused` | `write_mapping` | `write_mapping_body` | `write_sequence` |
| ---------------------- | -------------------- | --------------- | -------------------- | ---------------- |
| trim 4 (the reorder)   | −3 / 0               | 0 / 0           | −3 / 0               | 0 / 0            |
| trims 1+2 (the hoists) | −28 / −31            | −26 / −37       | −5 / −5              | −2 / −7          |
| trim 3 (the fold)      | −18 / −18            | 0 / 0           | +40 / +47            | 0 / 0            |

So the two writers' shrink is the guarded-static hoist, as the mechanism claims;
and the arm64 frame growth is **the fold's**, not the counters' — `nofold` keeps
both writers at 144 bytes and 14 spill stores, and the fold is what takes
`write_record_fused` to 160 with 15. The cause is visible in the loop preheader:
arm64 if-converts the separator to `cmp`/`csel w8, w24, w25`/`strb`, with *both*
constants hoisted above the loop (`mov w24,#123`, `mov w25,#44`), one more
loop-live register than main's single comma. `write_mapping_body` grows on both
ISAs because clang now peels the slot loop's first iteration itself (brace
immediates per symbol 1 → 2 on both ISAs) — the compiler's choice, not a second
instantiation in the source, and its per-record cost still falls.

x86-64 does **not** keep a branch, contrary to the code comment this shipped
with: with the `[[likely]]` removed it if-converts too, to
`testq`/`movl $44`/`movl $123`/`cmovel`/`movb`, with the constants materialised in
the loop body and no frame change. The lever-(b) note below is corrected
accordingly.

Per-key loops, counted as *the loop's own blocks on the fallthrough path*
(clang's `in Loop: Header=` annotations give the membership; a conditional branch
is not taken, an unconditional one is followed, the walk stops when it leaves the
loop — `codegen/per_key_loop.py`, with every counted instruction printed in
`codegen/per_key_loop.txt`). This rule is not the first cut's, whose rule was not
recoverable, so the numbers differ from the published 37/35 and 31/25:

- the fused writer's key-and-value loop: arm64 47 → **44**, x86-64 57 → 57 —
  and `reorder_only` already reads 44 / 57, so **the whole of the fused loop's
  per-key gain is trim 4, not the fold**. The fold's benefit in that writer is
  the per-record `ensure(1)`/`put('{')` block it deletes: ten instructions on
  arm64, eight on x86-64, on the fast path.
- `write_mapping_body`'s slot loop: arm64 36 → 33 (trim 4) → **29** (the fold),
  x86-64 34 → 34 → **29**.

**Measurement builds.** Every number below comes from `make pgo` — the
gate-inclusive PGO+LTO recipe, both suites run on both phases — and the two arms
are **tests-matched**: arm A is main's `src/` with this branch's final tests, arm
B is the reviewed source (2eccf36, whose `src/` differs from the tip's only in a
comment -- both compile to identical assembly, checked), so the profile's
test-suite component is identical and the difference between arms is the source
change alone (the separation E26-P7b showed
is necessary). No build in this entry, or anywhere in this work, used
`SKIP_TESTS`; the arms' `*.build.json` are retained beside their `.so`
(`p24/arms/`), recording `-O3 -flto=thin -fprofile-use=…`, 19 translation units,
recipe `gate-inclusive-posix-v1`, and `dirty: false` for arm B at the reviewed
commit (arm A records `dirty: true`, which is exactly what it is: main's `src/`
in a checkout of that commit). The first cut's numbers were plain `-O3`
universal2 builds with no retained samples; they are superseded by these.

**Canonical rows** (`benchmarks/dumps_rows_probe.py` through
`benchmarks/ab_builds.py`, small tier, six ABBA blocks of sixty per draw, orjson
in the same launches as the drift control, the one estimator in
`benchmarks/ab_blocks.py`). Two draws, and an A/A control of the same binary
against itself over four blocks whose floor is `max(|ci low|, |ci high|)`:

| row           | draw 1 bytes                | draw 2 bytes                | draw 1 str | draw 2 str | orjson drift    | A/A floor |
| ------------- | --------------------------- | --------------------------- | ---------- | ---------- | --------------- | --------- |
| `nested`      | **−3.59%** \[−3.75, −2.74\] | **−3.66%** \[−4.27, −2.87\] | −3.16%     | −3.44%     | −0.27% / −0.63% | 0.84%     |
| `mixed`       | **−2.31%** \[−2.78, −1.23\] | **−2.85%** \[−4.46, −1.39\] | −2.47%     | −2.81%     | −0.55% / −0.00% | 2.49%     |
| `users`       | **−2.51%** \[−2.74, −2.34\] | **−2.74%** \[−3.13, −2.52\] | −2.53%     | −2.54%     | −0.41% / −0.14% | 0.44%     |
| `flat`        | **−0.57%** \[−0.73, −0.10\] | **−0.79%** \[−1.78, −0.46\] | −0.69%     | −1.06%     | −0.05% / −0.34% | 0.62%     |
| `wide_arrays` | −0.62% \[−1.10, −0.12\]     | +0.78% \[−1.39, +1.32\]     | −0.44%     | +0.70%     | +0.46% / +0.12% | 0.88%     |

`nested` and `users` resolve past their floors on both draws and in both return
types. `flat` — the row the register-pressure constraint protects, 21 keys per
record — is a gain on both draws, just past its floor. `mixed` is a gain of
2.3–2.9% on both draws with an entirely negative interval, but its own A/A floor
is 2.49%, so it is at the edge of what this session can resolve. `wide_arrays`
changes sign between draws and sits inside its floor: not resolved either way,
which is what four keys and four 64-element arrays should read as.

**Per-container probe** (the nested-container probe, both PGO arms, A-B-B-A over
eight launches, each launch's own medians of sixty with `gc.collect()` per
iteration, 500 records per document; the table is the median of each arm's launch
medians, and **all 18 of the probe's rows are shown**, which the first cut's
seven-of-fourteen table was not):

| document         | A ns/record | B ns/record | delta  | orjson drift |
| ---------------- | ----------- | ----------- | ------ | ------------ |
| `scalars-only`   | 21.5        | 20.5        | −4.65% | +0.41%       |
| `third-scalar`   | 27.2        | 26.1        | −4.04% | +0.61%       |
| `value-dict0`    | 34.7        | 32.9        | −5.19% | −0.31%       |
| `value-list0`    | 30.1        | 29.2        | −2.99% | +0.62%       |
| `value-float`    | 40.2        | 38.7        | −3.73% | +0.19%       |
| `value-int`      | 28.4        | 27.6        | −2.82% | +1.50%       |
| `value-str`      | 27.8        | 26.8        | −3.60% | +0.28%       |
| `value-dict1`    | 45.3        | 43.7        | −3.53% | +0.92%       |
| `value-dict2`    | 61.1        | 59.0        | −3.44% | +0.28%       |
| `value-dict4`    | 89.4        | 87.9        | −1.68% | +0.00%       |
| `value-dict8`    | 146.5       | 145.1       | −0.96% | +0.31%       |
| `value-dict16`   | 262.2       | 261.3       | −0.34% | +0.21%       |
| `value-list1`    | 41.1        | 39.8        | −3.16% | +0.00%       |
| `value-list2`    | 49.0        | 47.5        | −3.06% | +0.43%       |
| `value-list5`    | 74.5        | 73.3        | −1.61% | −0.72%       |
| `value-list16`   | 167.0       | 165.8       | −0.72% | −3.14%       |
| `value-list64`   | 573.8       | 572.2       | −0.28% | −5.37%       |
| `value-rotating` | 41.2        | 39.9        | −3.16% | +0.58%       |

Every row improves, by a nearly constant **0.8–2.1 ns per record** — which is the
shape the mechanism predicts and the first cut described correctly: the gain is
per *record*, not per nested container, because the outer record is a dict too.
The two rows the first cut read as small regressions (`value-dict8` +0.9%,
`value-dict16` +0.3%) are −0.96% and −0.34% here; on a PGO build with a drift
control they were noise in a plain-`-O3` pair.

**The per-call trade, priced rather than asserted.** Trim 1 moves work from
per-dict to per-call, and both tables above use 500-record documents, where a
per-call cost is amortised five hundred times. `p24/percall_probe.py` is one
container per call (batches of 200 calls per sample, six ABBA blocks of sixty,
same estimator, with a four-block A/A control):

| row               | bytes  | str    | A/A floor (bytes / str) | resolved        |
| ----------------- | ------ | ------ | ----------------------- | --------------- |
| `percall-scalar`  | +1.26% | +0.84% | 1.17% / 0.38%           | yes, a cost     |
| `percall-empty`   | +2.65% | +6.10% | 0.99% / 0.67%           | yes, a cost     |
| `percall-record1` | −1.17% | +1.59% | 0.54% / 0.62%           | yes, both signs |
| `percall-record3` | +0.62% | +1.36% | 0.42% / 0.28%           | yes, a cost     |
| `percall-nested1` | +0.74% | −2.24% | 5.38% / 4.66%           | no              |

So the trade is real and small: entering `dumps` on a single scalar costs about
**1 ns more per call** (the `percall-scalar` row has no dict at all, so the only
thing it can see is the constructor, which arm64 also made a 67-instruction
out-of-line function), and a `{}` root about 3 ns. Against that, any document
with more than a handful of containers gains 0.8–2.1 ns per record. The cost is
acknowledged rather than hidden: it is paid once per call, it is under 3 ns, and
the only workloads it dominates are ones that serialize a single scalar or empty
container per call.

**Byte-identity.** 13 512 comparisons against stdlib
`json.dumps(…, separators=(",", ":"), ensure_ascii=False)` in both return types,
0 mismatches, run on a fresh thread (an empty `thread_local` schema cache) and
again on the warm main thread: the five canonical small-tier datasets whole and
record by record, then the adversarial sweep the three reviews used (key byte
widths 0–29 in seven character classes across the 16-byte slot boundary, dict
widths 0–30 across the 24/25 `kMaxSchemaKeys` seam, empty dicts interleaved at
three depths, dict depths 1–120 across the 64-level `kMaxCachedDepth` seam,
`str`-subclass keys alone and interleaved, wide and narrow schemas alternating at
one depth, tuples, `2**70`, `-0.0`, `1e300`), then 1 500 seeded random documents
built from repeated shapes. Script and log: `p24/differential.py`,
`p24/differential_plain_o3.log`.

**Rejected on their own codegen**, all three worth re-reading before they are
proposed again:

1. *Peel the record's first key* out of the emit loop so the loop's separator is
   a literal and no first-key test survives. Worth 23 instructions per record on
   arm64. On SysV x86-64 it turns `index + 1` into a second induction variable,
   spills the slot offset once per key and grows `write_record_fused`'s frame
   from 72 to 120 bytes — break-even near nineteen keys, so a net loss at
   `flat`'s twenty-one, on exactly the row E26-P6 showed is one live value from
   regressing.
2. *Select the separator byte between two registers* instead of branching on the
   first key. This lever is now moot rather than rejected, and the note that
   rejected it was wrong twice: both ISAs if-convert the two-arm branch on their
   own (arm64 `csel`, x86-64 `cmovel`), so what ships *is* a select between two
   registers; and arm64's register is not free — the second loop-live constant is
   where `write_record_fused`'s frame grows 16 bytes and takes one more spill
   store, measured against the same source with the fold removed. Writing the
   select by hand changes nothing the compiler does not already do.
3. *Hold the prepared row across the emit loop and reload it when `schemas_`
   grew* — the `user_steps_` pattern applied to the schema table. The re-index it
   replaces is already one load off `this` plus one off the vector, with the
   whole stride folded into an induction variable, so a load-compare-branch
   reload test is a net loss and it spilled the loop bound on arm64.
   Independently: `user_steps_` cannot be that test, because an all-scalar
   nested record grows the table while running no user code at all — only a
   counter of the table's own size can. The per-key re-index stays.

Also rejected: folding `write_sequence`'s `[` the way the brace folded. A list's
first element is emitted by seven bodies — the five scalar runs, the general loop
and `write_sequence_body` — each with its own "first element bare" convention, so
the fold means passing a separator byte through the runs, which is the
carried-value cost E26-P6 and the E26-P9 probe-placement follow-up both priced,
for one reservation per list. The comment at the bracket says so.

And withdrawn by the review, with the measurement that withdrew it: the
`open_count_` emptiness probe (trim 2, above), the `always_inline` on the key
emitter, and the `[[likely]]` on the separator test.

**Not measured here, and deliberately out of scope.** Two further items the
codegen exposed. `is_plain_scalar` is an out-of-line call per key on both ISAs
and in both revisions — per-key work, not per-open. And the separator could
disappear entirely if `build_schema` baked it into the prepared bytes (slot 0 as
`{"key":`, the rest as `,"key":`), which removes the select and the byte store
but widens `Schema::slots` by 50%, against the cache argument that put those
bytes inline in the first place. Neither belongs in a trim of the open path.

**Evidence** — `build/evidence/benchmark-lead/p24/`:

- `arms/armA_tests_only.so`, `arms/armB_fixed.so` and each one's `.build.json`
  and `…_profile.inputs.json`: the two measured PGO+LTO builds and their
  identities. `arms/armAA_copy.so` is arm A again, for the A/A controls.
- `rows/aa_control.{tsv,txt}`, `rows/rows_ab_draw1.{tsv,txt}`,
  `rows/rows_ab_draw2.{tsv,txt}`: every canonical-row sample, one line per
  sample, with the estimator's output beside it.
- `percall/percall_ab.{tsv,txt}`, `percall/percall_aa.{tsv,txt}` and
  `percall_probe.py`, `run_ab.py`: the per-call arm.
- `nested/nested_ab.log` and `nested_container_probe.py`: all eight launches of
  the per-container probe, verbatim, tagged by arm.
- `codegen/`: `take_codegen.py`, `per_key_loop.py`, `report_codegen.py`, the
  `codegen.json` they produce, `codegen_table.txt`, `per_key_loop.txt`, the
  compile command beside each `.s`, and both ISAs' assembly for `main`,
  `reorder_only`, `nofold`, `head` and the shipped source.
- `differential.py` and `differential_plain_o3.log`: the byte-identity run.
- `gates/`: the `make pgo` logs for both arms, `make_test_final.log` (2530
  passed against the shipped build) and the ASan gate's log (2529 passed, 1
  skipped). The ASan extension is compiled **without** `-DNDEBUG` — checked in
  its own `build.json` — so the two new `assert`s are live in that run rather
  than merely present in the source.

**Outcome: go** on what was actually measured, which is: a tests-matched A/B of
two `make pgo` builds on one M1-class Mac — six ABBA blocks of sixty per draw,
two draws, orjson in the same launches as the drift control, and an A/A control
of the same binary that says what this session can resolve. Four canonical rows
improve, `nested` and `users` past their floors on both draws, `flat` just past
its own, `mixed` by 2.3–2.9% with an interval that is entirely negative but a
floor that is 2.49%, and `wide_arrays` unresolved in both directions. All 18
per-container rows improve. The canonical 27-row regression gate was **not** run
on this revision — `make bench-check` compares against a same-machine baseline
whose own identical-binary control breaches it under load (E26-P9), and this
campaign reads A/B evidence instead; what is claimed here is no resolved loss on
any measured row, not a canonical gate pass. Both suites (2530 tests) and the
ASan+UBSan gate are green, with no `SKIP_TESTS` anywhere, and the shipped
extension is a gated `make install` of the reviewed commit with `dirty: false`.
Five-platform CI remains the acceptance gate for the Windows row this exists to
move; the M1 cannot decide that one, only show the mechanism is real and cheap on
the architecture it can measure. **One sample is owed:** the five-platform A/B
that ran while this was under review measured 7155c33 against c6783e1, i.e. the
source *before* the review's fixes. The merged source differs from it in the
three withdrawals above, so a new five-platform sample of the tip against the
same tests-only base is the outstanding evidence, and the Windows row's verdict
waits on it.

- Five-platform A/B of the first cut (run 34686936643, the tests-only arm
  c6783e1 against 7155c33 — i.e. before the open-count probe and the
  `always_inline` were withdrawn; `experiment=none`, six blocks of sixty,
  every arm's binary matching its sidecar, training matched): **seventeen
  resolved gains, no resolved loss on any leg.** Windows (the leg whose
  `dumps mixed` is the one row still behind): small `dumps mixed`
  **−5.06%/−5.75%** against a 1.3% floor, small `dumps users` −8.17%/−8.87%,
  medium `dumps users` −6.33%/−6.67%, file `dump nested` −2.46%; medium
  `dumps mixed` reads raw −5.38% inside a 25.9% floor. i7: small `dumps mixed` −7.14%, medium −5.82%, medium `dumps users` −5.54%, file `dump nested` −3.70%. EPYC: medium `dumps mixed` −4.05%, small −3.02%, file
  `dump mixed` −2.11%, file `dump nested` −5.27%. N2: `dumps mixed`
  −1.57%/−2.15%, `dumps flat` −0.80%/−0.79%, `dumps users` −0.58%/−0.70%.
  M2 Pro VM: nothing resolved against floors of 1.3–7%, every serializer row
  leaning the same way. Evidence
  `build/evidence/benchmark-lead/p24/native-p24-open-cost-34686936643/`.
  The arithmetic on the standing: Windows `dumps mixed` reads 1.06x and 1.08x
  on the two samples of ec53f93, so −5.1% of strata's own time puts it near
  1.03x — most of the gap, not all of it on those draws.

- A five-platform A/B of the **fixed** tip is owed and dispatched (run
  34690375277: the tests-matched base db073e86 against 9631aa6). The withdrawn pieces cost `write` three
  instructions on x86-64 and two on arm64, and the fixed tip reads better on
  every leg (run 34690375277, `exp/p24-tests-only` db073e86 against 9631aa6,
  evidence `p24/native-p24-fixed-34690375277/`): **no resolved loss on any
  leg**, and the resolved gains are wider than the first cut's everywhere.
  EPYC sweeps its whole serializer set — `dumps mixed` −4.66%/−3.54%, `dumps users` −4.36%/−4.83%, `dumps flat` −2.48%/−2.36%, `dumps wide_arrays`
  −3.70%, file `dump nested` −6.68%, file `dump mixed` −1.81%, file `dump wide_arrays` −2.11%, eighteen rows in all. **Windows small `dumps mixed`
  −6.89%** against a 2.89% floor (the first cut read −5.06%), with `dumps users` −2.77% and nothing against. N2: eight rows, `dumps flat`
  −1.53%/−1.67% where the first cut read −0.80%, `dumps mixed`
  −1.81%/−1.43%, `dumps users` −1.17%/−0.82%, file `dump nested` −1.42%,
  `loads wide_arrays` −0.43%. i7: small `dumps mixed` −4.92%, `loads wide_arrays` −2.94%. M2 Pro VM: `dumps mixed` −3.36%, file `dump mixed`
  −4.64%, `dumps users` −1.81% — a leg that resolved nothing on the first
  cut.

  What that does to the standing: Windows `dumps mixed` reads 1.06x and 1.08x
  on the two samples of ec53f93 (strata 0.076/0.075 ms against orjson
  0.071/0.069). Taking 6.89% off strata's own time gives 0.071/0.070 — at
  parity with the first draw's orjson and a per-cent above the second's. The
  row can flip on a favourable draw and is within the report's resolution on
  an unfavourable one; two five-platform samples after integration are what
  decide it, as always.

- Outcome: **accepted for integration on this measurement** — 24 resolved
  gains across the five legs, no resolved loss anywhere, output byte-identical
  over 13,512 differential comparisons, both gates green with no build
  outside the gated path.
