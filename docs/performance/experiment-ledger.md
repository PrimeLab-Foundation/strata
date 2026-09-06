# Experiment ledger

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
- Outcome: open

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
- Outcome: reviewed in its first composition; recomposed after E26-P5b and
  reviewed again (`build/evidence/P5B-REVIEW/REVIEW.md`, 2026-09-07: not
  refuted on the measurement, provenance, identity and roll all reproduced;
  seven reporting corrections required and applied — the duplicated E26-P2
  copy removed, the small `dumps mixed` cell marked inside its floor, the
  recipe's cost stated as +3.3–6.3% on the three rows measured, a
  negative-results row added, the packet's large report set to the committed
  draw); open (publication, two five-platform samples)
