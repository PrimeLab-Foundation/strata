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

## Native continuation, September 8

Published revision: `c9a337dbb05c11244dab08a00bbc4ad33a6f4b0d`.
The fresh-build control [34159096738](https://github.com/PrimeLab-Foundation/strata/actions/runs/34159096738)
completed successfully on all five platforms. All ten binary hashes match
their sidecars, both arms name the pinned commit, and every build records
complete compilation. Candidate identities are marked dirty only because of
untracked diagnostic `ab/cpu.txt` / `ab/experiment.txt`; every tracked-source
patch is empty. These diagnostic identities must not be treated as clean
canonical benchmark identities.

For small `dumps mixed` bytes, the normalized fresh-build effects and matching
identical-binary floors were:

| Platform       | Effect | 95% block interval | A/A floor |
| -------------- | ------ | ------------------ | --------- |
| Linux ARM64    | +0.15% | -0.35..+0.96%      | 1.72%     |
| Linux x86-64   | -0.57% | -2.29..+0.87%      | 2.55%     |
| macOS ARM64    | +1.30% | -1.56..+5.99%      | 4.47%     |
| macOS x86-64   | +4.15% | -3.54..+8.50%      | 7.60%     |
| Windows x86-64 | -0.03% | -0.79..+1.74%      | 0.38%     |

No target effect is resolved. Mac ARM file `dump mixed` nevertheless reports
-6.05% (interval -10.14..-4.38%), past its 4.97% identical-binary floor,
without any source change. `otool` extraction confirms that A and B have
identical `__TEXT,__text` on both macOS architectures. Thus this apparent
file improvement is not evidence of changed executable code; a single A/A
interval cannot certify such a candidate gain. Preserve this contradictory
control and require confirmation rather than interpreting the successful
workflow status as a performance acceptance.

Isolated native experiments dispatched with both refs pinned to `c9a337d`,
so the only intended source delta is the selected patch and its tests:

- Schema recovery: [34163602199](https://github.com/PrimeLab-Foundation/strata/actions/runs/34163602199).
- File newline: [34163603711](https://github.com/PrimeLab-Foundation/strata/actions/runs/34163603711).

Both use six ABBA blocks, 60 samples and matching A/A controls on all five
platforms. Results are pending; neither prototype has been reintroduced into
production. Full control artifacts and macOS text hashes are archived under
`build/evidence/benchmark-lead/native-control-34159096738/`. These are A/B
diagnostics, so canonical `ci_summary.md` remains the latest published
133/135 result until new complete canonical reports exist.

### E26-P9 and current integration boundary

A third prototype routes exact root/nested dictionaries to the existing
out-of-line fused writer. Both PGO builds used identical test/training source
manifests and data. The six-block, 60-sample A/B result shows -2.21% small
mixed bytes and -9.51% nested bytes beyond matching A/A floors. Canonical
regression checks nevertheless failed twice: 15 metric breaches at 10
samples, 18 at the predeclared 60-sample confirmation. Both builds ranked
27/27 in the latter; that does not waive the gate. Both full PGO test phases
and the candidate's ASan/UBSan checks passed (15 C++ suites/2,249 Python tests).

The prototype is removed from production and retained with its tests as
`experiments/benchmark-nested-mappings.patch`. The workflow adds a
`nested-mappings` choice; `combined` still means only the original schema and
newline patches. Native execution of this new choice requires publication.
Canonical Make targets now honor existing `BENCH_REPEAT`/`BENCH_WARMUP`
variables, with the original 10/2 defaults and thresholds preserved.

Initial native isolated results (remaining jobs still pending): schema
recovery improves Linux x86 serialization, including mixed bytes -1.87%, but
nested file dump slows +2.32% (interval +0.57..+2.96%, A/A floor 0.50%).
Windows mixed bytes is unresolved (-0.07%, interval -0.77..+1.72%, floor
1.34%). Linux ARM small mixed is also unresolved. File-newline ARM results
do not yet establish a resolved file-write gain. No isolated experiment is
accepted, and the combination has not been dispatched.

File-newline follow-up: Windows file flat/mixed/nested normalized changes are
-1.26%/-1.62%/-1.45%, below respective A/A floors 1.74%/2.67%/2.08%; raw flat
and nested times are actually +1.31%/+0.71%, so their relative gains are not
Strata speedups. Linux x86 small mixed bytes regresses +2.67% (raw +2.72%,
interval +2.28..+2.99%, floor 1.42%); medium mixed bytes also slows. This
experiment does not qualify for production or a combined run. Both isolated
experiments have four successful platform jobs; macOS Intel remains running.
Full native acceptance is not claimed. `ci_summary.md` was regenerated and
correctly remains 133/135 for the latest complete canonical run.

The new native `nested-mappings` option applies its four mutation tests to
both worktrees before PGO training. Pinning both refs to the same new revision
therefore preserves the matched-test design used locally; only the candidate
receives the runtime change. Both implementations passed these tests locally.

Retained-change validation: restored production source/binary passed
`make test` (15 C++ suites, 2,245 Python tests); changed-file pre-commit hooks,
workflow shell syntax, full prototype patch applicability and tests-only
baseline patch selection passed. The repository baseline is unchanged.
Native publication of E26-P9 remains the next dependency; no commit or push
was performed by this session.

### E26-P9 complete native result and confirmation

Native run [34166567410](https://github.com/PrimeLab-Foundation/strata/actions/runs/34166567410)
completed all five platforms successfully with both refs pinned to
`5802ff3ccf926029ed59791aa9ce294f22663cdb`. Build/test success is not performance
acceptance. Raw Strata mixed-bytes timing changes (negative is faster):

| Platform       | Small  | Medium |
| -------------- | ------ | ------ |
| Linux ARM64    | -5.61% | -5.14% |
| Linux x86_64   | -0.36% | +1.88% |
| macOS ARM64    | -2.43% | +1.28% |
| macOS x86_64   | -5.95% | -6.10% |
| Windows x86_64 | -1.85% | -2.00% |

Linux x86 medium mixed bytes has a normalized +2.21% change, interval
+1.09..+5.17%, exceeding its 0.96% A/A floor. Windows mixed-byte estimates
remain below their noise floors. macOS ARM medium has a normalized gain
while its raw Strata time increased; do not describe that as a speedup.
Artifacts for all five platforms are retained under
`build/evidence/benchmark-lead/native-nested-34166567410/`.

One confirmation run,
[34186143209](https://github.com/PrimeLab-Foundation/strata/actions/runs/34186143209),
uses the same pinned refs, six blocks, 60 samples and matched-test PGO recipe.
This checks repeatability of the target gains and the Linux x86 regression;
it does not replace the first result or relax any gate. If the regression
persists, investigate the affected generated code and revise or reject the
prototype before integration. Do not rerun until a favorable result appears.

The objective remains every declared row on every supported platform, not
merely successful CI jobs. After an acceptable candidate is established:
run the full correctness and sanitizer gates, pass canonical regression checks
across small/medium/large tiers, and obtain two complete canonical five-platform
benchmark runs. Fetch those reports with `make bench-ci` to regenerate
`ci_summary.md`; the current canonical result remains 133/135. No runtime
optimization has yet been accepted from these isolated experiments.

### Native confirmation and full-workload validation (September 8)

Confirmation run [34186143209](https://github.com/PrimeLab-Foundation/strata/actions/runs/34186143209)
completed all five platforms. Raw mixed-bytes changes for small/medium were
Linux ARM64 -5.68%/-4.93%, Linux x86 -1.61%/+0.79%, macOS ARM
+3.20%/-2.56%, macOS Intel -9.72%/-1.84%, and Windows -2.21%/-1.43%.
The Linux x86 medium normalized effect became +0.18%, interval
-0.69..+0.93%, within its 0.80% A/A floor; the first run's regression did
not repeat. Windows remains unresolved. All 20 archived binaries across the
two runs match their sidecar hashes and record complete compilation. Both
arms have identical test/training-source and training-data manifests on all
ten platform/run pairs. Verification is retained in
`build/evidence/benchmark-lead/native-nested-verification.json`.

These selected-row results justify checking the full workload, not accepting
P9. A fixed new-session local sequence uses the same archived matched-test
PGO binaries, A then B per tier, at 60/20/10 repeats for small/medium/large.
Reports and unchanged-threshold gates are retained in
`build/evidence/benchmark-lead/p9/full-gate/`; earlier failed comparisons are
preserved. Small and medium each rank 27/27 in both arms but fail regression
checks. Small mixed median is +2.6% and p95 +11.5%; medium mixed median
+10.1% and p95 +26.5%. Unchanged parser/query controls also worsen. This
remains a failed acceptance, not evidence that the production optimization
has landed.

The fixed sequence completed: both arms rank 27/27 in all three tiers, but
small/medium/large have 14/24/16 regression-metric breaches respectively.
Large includes users serialization p95 +265.2%, users parsing p95 +61.4%,
and whole-process query RSS +51.8%. The original installed binary was
restored and verified against SHA256
`959543f23a19cc9d0ba026fc5c74aabc92c1ae56f0ce899ade388ea0777e2aae`.

A separate, non-timing cache-history probe rejects the proposed retirement
explanation for this canonical prefix: after 126 serialization calls per
users/flat/nested/wide dataset, new keys are still retained at dictionary
depths 1–3, as in a fresh process. The positive churn control disables
retention at depth 1. The existing history-cliff bug is real, but this probe
does not establish it as the cause of P9's failed canonical checks. Do not
combine schema recovery with P9 on that unsupported explanation. Script and
results are retained in `p9/full-gate/history.py` and `history.json`.

The new `validation=canonical` option on the native A/B workflow runs the
complete small-tier 27-row suite on each of the five runners and applies the
same-machine regression gate. It preserves matched-test PGO for P9 and
archives both canonical reports, full-precision samples and failures.
`make probe-canonical-builds` exposes the same operation locally, including
multiple tiers. Arms use staged facades and never replace the installed
extension or metadata. Existing output directories are refused.

Next publication should run this canonical validation with both refs pinned
to the published revision and `experiment=nested-mappings`, `repeat=60`.
The runtime patch remains isolated until these outstanding gates are resolved.
Only subsequent clean production `benchmark.yml` runs can update the canonical
135-row tracker; this diagnostic workflow deliberately cannot relabel patched
binaries as a clean production SHA.

The new staging runner was exercised end to end with the identical archived
A binary on both arms, small tier at 60 repeats. Both companions identify
SHA256 `82dc285413125ed40e7f3d1ea5403171adc50926ec6053064566249489c7a57c`.
That unchanged-binary control still fails 16 canonical metrics, including
parser medians and +12.1% whole-process query RSS. Evidence is under
`p9/canonical-AA/`. This demonstrates variation without an extension change;
it does not retroactively pass P9 or establish a per-row noise allowance.
The installed original binary and its matching metadata remain intact.

Validation of the retained tooling: `make test` passed 15 C++ suites and
2,248 Python tests, including staging cleanup on child failure, dataset
preflight, preserved failed gates and continued checks of later tiers.
All 16 workflow shell steps passed syntax validation. No runtime source or
published performance baseline was changed. The next native canonical run
requires the human's commit/push of this workflow and runner first.

### E26-P9a: narrow by platform and investigate the remaining tails

Full native canonical run
[34253218374](https://github.com/PrimeLab-Foundation/strata/actions/runs/34253218374)
completed with failed regression gates on all five platforms. Baseline/candidate
standings were Linux ARM64 26/27 -> 27/27, Linux x86 and both Macs 27/27 ->
27/27, and Windows 25/27 -> 25/27. Thus the candidate totals 133/135 against
132/135 for this run's baseline. Candidate Windows misses are mixed `dumps`
(1.037x orjson) and mixed file `dump` (1.023x). These are patched diagnostic
standings, not a replacement for `ci_summary.md`.

Gate breaches by platform: Linux ARM64 2, Linux x86 18, macOS ARM 16,
macOS Intel 22, Windows 32. All ten archived binaries match their sidecars
and record complete compilation. Linux ARM64's only breaches are:

- Wide-array file dump p95 1.771473 -> 1.807150 ms (+2.014%); median
  +0.377%. Orjson's median/p95 move +2.440%/+4.217% in that row.
- ID query p95 0.113280 -> 0.117376 ms (+3.616%); median +0.066%.
  Both arms' first/second-half query medians remain near 0.102 ms.

The 60-sample p95 reads the fourth-largest observation. These results do not
establish a sustained throughput regression, but both remain gate failures.
Raw tail inspection is retained in `p9-linux-arm64/tail-audit.json`; neither
rounding the first breach to 2% nor changing the thresholds is permitted.

Implement `experiments/benchmark-nested-mappings-linux-arm64.patch`, selectable
as `nested-mappings-linux-arm64`. Only Linux ARM64 receives P9's dispatch;
other targets retain the original call. Baseline and candidate keep the same
four mutation tests. This is an isolated, unaccepted candidate, not an ARM
tail fix. The whole Darwin preprocessed serializer is byte-identical before
and after (SHA256 `a22c66e6a305fc3c57ffe2a91751ce8541233e6c4e61f1362860e47686f45b55`).
Predicate checks cover Linux ARM64/x86, macOS ARM64/x86 and Windows x86.
`make test` with the patch applied passed 15 C++ suites/2,252 Python tests;
then the runtime/test patch was removed again. Evidence is in
`build/evidence/benchmark-lead/p9-linux-arm64/`.

Two authorized investigations are running, pinned to the published `5bbec77`:

- Unchanged-source canonical comparison:
  [34257791864](https://github.com/PrimeLab-Foundation/strata/actions/runs/34257791864),
  both refs identical, `experiment=none`, 60 repeats. This includes fresh-PGO
  build variation and runner variation; it is not an identical-binary A/A and
  cannot by itself waive P9's failures.
- Fresh profiling, including Windows decomposition:
  [34257653984](https://github.com/PrimeLab-Foundation/strata/actions/runs/34257653984).

The profiler update adds a Windows-only scope for subsequent runs and archives
PGO identity plus real-file phase controls before plain-toolchain rebuilds.
This makes the Windows serialization and file-cost investigations separable.
It does not revive the rejected newline prototype or establish a Windows fix.
The new narrowed candidate and Windows file collection require publication;
first read the pending control/profile results, then validate the narrowed
candidate with the complete native canonical gate. No production runtime or
published regression baseline has been changed.

### September 9: revised next experiment after native results

The narrowed run 34265403380 is complete and unaccepted: Linux ARM64 27/27,
all-platform candidate 132/135, all five regression gates failed. Correlated
rival tails support interference for two ARM64 failures; mixed file load
remains unresolved. Keep the existing patch isolated and retain every sample.
See the ledger's September 9 follow-up for the exact comparisons.

Windows PGO profiling 34265407120 identifies full-precision floats and mixed
scalar workloads as the next serializer targets; records-only already leads.
Inspect the scalar dispatch and float emission code, form one bounded
prototype, and measure matched-test PGO against the retained baseline before
native canonical validation. File opening dominates a Python phase control
but is shared by competitors, so it is not yet an evidenced optimization.

P10 tested resuming float/string runs after heterogeneous elements. It passes
correctness but provides no local mixed gain and worsens nested/wide-array
serialization. The patch remains isolated; do not dispatch it as a candidate
for integration. Continue with full-precision float emission, whose Windows
homogeneous bucket independently trails at 1.147x, preserving the existing
short-decimal path and shortest-round-trip oracle.

P11 isolates a direct 16/17-digit count after Dragonbox conversion. Both PGO
arms pass all correctness tests; local paired bytes gains remain unresolved
and a wide-array control moves adversely. Full small-tier validation ranks both arms 27/27 but fails ten regression
checks, including mixed dumps median +2.3% and users median +4.2%. P11 is
unaccepted. The `float-digit-count` native workflow selector applies only this
patch; both refs must name the same published revision to match tests and
training sources. Native Windows measurement is needed before any claim
about its deficit. No production change has been accepted.

P12 checks object sharing before choosing a string optimization. All mixed
strings are clean ASCII; the real values occupy 502 objects for 258 distinct
values. Pooling equal values changes local timings for both engines, so this
is not evidence for a Strata-specific cache fix. The Windows PGO profile now
archives the value-preserving control. Publish the prepared workflow and
probe changes before native investigation; local and remote remain at
663c480 as of this check. Do not dispatch an old revision expecting new code.

P13 tested first-key-first cache lookup with matched PGO, exact disassembly
comparison, six paired blocks and an identical-binary control. It changes the
intended loads but does not improve mixed serialization. Keep it isolated.
The next record-path investigation should examine emission and reservation
costs rather than cache-way comparison order; retain all ownership and
reentrancy protections. No production optimization is accepted yet.

P14 combines the cached key and exact compact integer under one reservation.
Both matched PGO builds pass all correctness tests; paired flat bytes improves
7.88% raw and 7.51% normalized beyond the 1.17% control floor. Full local
small-tier flat median improves 6.85%, but 23 timing/RSS checks fail, including
mixed median +3.0%. Keep P14 isolated. After publication, use native
`record-int-reserve` with `validation=canonical`, 60 repeats, and both refs
pinned to the published SHA;
the baseline receives the same two regression tests. No production benchmark
or accepted baseline changes until the complete gates pass.

P14 native run 34346286916 has now completed. All five gates fail; both
arms total 131/135, and Linux ARM64/Windows mixed remain behind. Reject
integration and preserve the patch as evidence. The clean production summary
is regenerated and remains 133/135 at 75cfb42. No rerun of the same candidate
is planned; the failed canonical checks remain authoritative.

P15 independently combined key and short ASCII-value reservations. Matched
PGO and boundary tests pass, but the paired screen shows no mixed gain and
a resolved users regression. Reject this inline expansion; its fused body
grows by 331 ARM64 instructions. Keep the patch isolated and retain all raw
samples. Further scalar reservation work needs a shared emission body that
avoids duplicating the scan and fallback machinery, followed by codegen and
matched-PGO screening before native validation. Do not combine P14 and P15
or treat either flat-record gain as evidence that mixed is fixed.

P16 tested that shared-body follow-up with the same boundary tests and PGO
baseline. The compiler expands the fused body further to 1,388 instructions;
mixed remains unchanged and users bytes regresses 2.36% raw beyond its floor.
Reject it too. End this scalar reservation line. The next implementation
needs native instruction-level evidence for the remaining mixed-record
overhead, with a codegen check that the proposed cost actually decreases;
neither source factoring nor a win on flat records establishes that mechanism.
