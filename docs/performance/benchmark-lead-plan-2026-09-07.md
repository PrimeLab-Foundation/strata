# Plan: reproducible benchmark leads

Base: `cb22551`, branch `work/benchmark-lead`. Runtime baseline is `75cfb42`;
the intervening commit contains reports. Preserve the two existing untracked
plans. This plan is executed as measured experiments, not a promise that a
shared runner will always rank first.

## Evidence and objective

The latest complete runs of the same runtime scored 128/135 and 133/135.
Linux ARM64 and Windows `dumps mixed` missed in both, by 8.00%/2.29% and
3.33%/1.46% respectively. File dump flat/mixed on macOS ARM, dumps mixed on
macOS Intel, and file dump nested/mixed on Windows missed only in the first.
The local canonical regression gate remains failed despite diagnostic A/A
controls finding no resolved effect. These are separate facts.

Completion requires two complete same-revision five-platform runs with all
135 declared rows first, a resolved same-host regression gate, correctness
and sanitizer gates, and preserved full evidence. A supportability pass is
insufficient. Keep medium/large and supplementary coverage separate from the
135-row series. Never weaken semantics, rivals, workloads, or thresholds.

## Implementation sequence

1. **Establish the current baseline.** Run both test layers, archive the
   installed production binary/profile identity, and record current canonical
   rows before editing runtime code. Preserve all unfavorable measurements.
2. **Recover serializer schemas between documents.** The current source still
   permanently retires each schema depth after 64 misses. Confirm the effect
   using stable records before/after unrelated schema churn. Scope the miss
   budget and retirement to a serialization call while retaining owned,
   prepared schemas across calls. Reset only when acquiring the idle shared
   lease; nested calls must keep private state. Test failure, reentrancy,
   mutation, and bounded ownership. This repairs a workload-history cliff;
   it is not presumed to explain the canonical mixed deficit. The old E26-P4b
   ledger's acceptance line does not match the current source.
3. **Remove the separate file newline write.** Prototype an internal
   bytes-serialization mode that appends the required newline before sealing
   its private result, with an independent exact-size hint. File output can
   then use one contiguous write on Windows and POSIX without copying an
   already sealed bytes object. Preserve partial-write loops, error handling,
   output truncation, POSIX 0644, and serialize-before-open behavior. Compare
   file rows independently; reject if the cost moves into serialization.
4. **Attribute the persistent mixed row.** Use the existing records
   decomposition and actual PGO code on native N2 and Windows. Keep canonical
   five-engine, hot, and GC-only conditions distinct. Prototype only a cost
   visible above measurement uncertainty. Do not revive rejected cycle-stack,
   integer-layout, cached-kind, or broad code-footprint changes without new
   evidence. Protect every shape and parse/search/query control.
5. **Control PGO variation.** Retain the shipped gate-inclusive recipe for
   initial comparisons. New tests change its profile: compare same-source
   builds and the original binary separately. A training-only experiment may
   proceed only with broader API-derived training, fixed seeds independent of
   benchmark data, full correctness gates outside the merged training profile,
   and native comparisons. Do not ship a recipe merely to freeze its counts.
6. **Integrate only supported changes.** Six complete serial ABBA blocks,
   60 samples per target/control row, trailing baseline, comparable A/A floor,
   identified artifacts, and canonical confirmation are required. Gains must
   clear uncertainty; >2% median/p95 or >5% RSS regressions require resolution
   or reversion. Record each candidate as accepted, rejected, or inconclusive.
7. **Publish evidence after human publication.** Run the full correctness
   matrix and two five-platform production PGO benchmarks of the exact pushed
   revision. Archive both runs, update `ci_summary.md` through `make bench-ci`
   whenever the new reports are ready, and classify every remaining miss.
   Human-owned commits/pushes remain the publication boundary.

## Execution log

- Opened: current source and the two complete CI runs reconciled. Production
  schema retirement is permanent; the planned file newline path is distinct
  from the rejected bytes-return copy experiment. No optimization accepted yet.

- Implemented per-document schema recovery and the contiguous file newline
  prototype, with focused ownership, reentrancy, boundary-size, failure-before-
  truncate and real partial-write tests. Full gate: 15 C++ suites, 2,260 Python
  tests, 100% facade coverage. ASan/UBSan: all 2,260 Python tests passed.

- Production PGO baseline SHA-256 starts `959543f23a19`; schema-only
  `1bb4d4fa84ed`; combined `ac3e145e0c58`. Full identities, profiles, raw inputs,
  logs and measurements are in `build/evidence/benchmark-lead/`.

- Schema-history diagnostic: baseline stable serialization rose from 15.33 us
  warm to 21.46 us after churn. The schema-only PGO build measured 14.17 us
  warm and 14.10 us after churn. The within-build history cliff is repaired;
  cross-build speed differences here are not acceptance evidence.

- The first combined A/B attempt accidentally selected an old CPython 3.12
  extension with a filename glob for the 3.14 interpreter. Launch B crashed;
  the driver restored the original. Crash UUID `5AA8B146-EF72-3FBC-ACC0-6AB4A7223ED8`
  matches that old binary exactly. Preserve `invalid-abi-ab.*`, exclude it
  from comparisons. New CLI preflight requires hash-matching sidecars and
  rejects known incompatible versioned Python headers; four tests pass.

- Valid combined ABBA and matching identical-binary control: six blocks,
  60 samples, nine workloads (14 measured Strata modes), trailing baseline.
  No normalized effect cleared its matching floor with a resolved interval.
  Mixed bytes: +0.93%, interval +0.06..+2.32%, floor 1.16%; file mixed:
  -0.79%, interval -1.00..+4.89%, floor 3.96%. Inconclusive, not accepted.

- Native baseline diagnostic run `34146265191` completed all four jobs.
  ARM64 hot samples attribute 42.72% to `Serializer::write`, 28.11% to
  `write_record_fused`; the cold sample is dominated by setup and is not
  serializer-only attribution. Windows PGO mixed interleave is 1.109x orjson;
  records-only 1.088x, floats 1.114x. These diagnostics do not replace the
  canonical five-platform standings or identify an accepted numeric rewrite.

- Prepared same-host PGO A/B workflow for all five platforms, with native
  Windows and ARM64 controls, explicit file rows, and archived binary/profile
  identities. Execution of unpublished changes awaits human publication.

## Integration decision and remaining work

**Neither runtime prototype is accepted.** Both were removed from production
source after failed regression checks and preserved with their tests as
`experiments/benchmark-schema-recovery.patch` and
`experiments/benchmark-file-newline.patch`. The published runtime is restored
locally, including the original production PGO binary. The schema cliff is
reproduced and its proposed fix passes functional/sanitizer tests, but this is
not enough to bypass the performance contract.

The complete same-session canonical sweep measured baseline 27/27 in each
of small, medium and large; combined candidate 26/27, 27/27 and 27/27. Its
small file `dump flat` lost to orjson by 6.22%. Strict comparisons against the
captured baseline failed with 11, 12 and 12 metric breaches respectively.
The schema-only small comparison also failed (14 breaches). These are failed
gates, not evidence that every individual difference is caused by the patch.
The six-block A/B control resolving no change does not override them.
Against the repository's recorded baseline, small remains failed; medium and
large have no recorded baseline scope and therefore cannot pass that gate.
No baseline was replaced. Temporary comparison baselines only preserve the
before measurements under the evidence directory.

Next execution after human publication:

1. Dispatch `ab_x86.yml` with `experiment=none` and both refs at the new
   published revision: fresh builds of unchanged source plus identical-binary
   controls on all five platforms.
2. Dispatch each isolated experiment, then `combined` only if both individual
   experiments qualify. Every run applies the selected patch and its tests
   before the candidate's full production PGO gates. Retain raw artifacts.
3. Use native Linux ARM64 and Windows effects to accept/reject the prototypes;
   continue numeric/record attribution for persistent mixed misses. No numeric
   rewrite was justified or implemented during this local campaign.
4. Apply only accepted patches to a production branch, run correctness and two
   complete five-platform benchmarks of the human-pushed clean revision, and
   regenerate `ci_summary.md` using `make bench-ci` as soon as reports are ready.

**The 135/135 objective is still open.** This change implements the diagnostic
and native experiment path; it does not claim to have made Strata first in
every benchmark. The latest canonical CI summary remains the valid 133/135
report for `75cfb42`, not a score for these unpublished experiments.

## Final validation of the retained change

- `make test`: 15 C++ suites and 2,245 Python tests passed on the restored
  production extension. Four new tests exercise the A/B identity preflight.
- Changed-file pre-commit hooks and `make lint` passed. YAML parsing, all 14
  workflow shell bodies (`bash -n`), five-platform coverage, and all three
  isolated/combined `git apply --check` selections passed locally. Native
  execution of the new workflow is still pending publication.
- `make bench-ci-summary` regenerated the valid latest report: five platforms,
  133/135 at first place, `75cfb42`. No new candidate CI score exists.
- Original runtime hash `959543f23a19cc9d0ba026fc5c74aabc92c1ae56f0ce899ade388ea0777e2aae`
  restored; no production C++ diff retained. The native prototypes passed the
  earlier full gate and sanitizers before removal; this is separate from their
  failed performance acceptance. No commit or push performed.
