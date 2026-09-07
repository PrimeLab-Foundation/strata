# September 7 plan execution

## Published revision follow-up

The human published `75cfb422ede723193706740309d7509adf327f2b` on
`work/september-7-plan`. Two complete five-platform benchmark runs now measure
that exact revision:

| Platform       | Run 34143749167 | Run 34143751498 |
| -------------- | --------------- | --------------- |
| Linux ARM64    | 26/27           | 26/27           |
| Linux x86_64   | 27/27           | 27/27           |
| macOS ARM64    | 25/27           | 27/27           |
| macOS x86_64   | 26/27           | 27/27           |
| Windows x86_64 | 24/27           | 26/27           |
| Total          | 128/135         | 133/135         |

[First run](https://github.com/PrimeLab-Foundation/strata/actions/runs/34143749167)
and [second run](https://github.com/PrimeLab-Foundation/strata/actions/runs/34143751498)
both passed every PGO build and supportability job. No rounded ties were
reported. Linux ARM64 `dumps mixed` missed against orjson by 8.00% / 2.29%;
Windows `dumps mixed` missed by 3.33% / 1.46%. Other misses appeared only in
the first run: macOS ARM file dump flat/mixed, macOS Intel dumps mixed, and
Windows file dump nested/mixed. Neither run establishes the 135/135 objective.

`make bench-ci` fetched the second run and regenerated
`docs/benchmarks/ci_summary.md` and all five report/companion pairs. Every
platform has complete, verified provenance. Whole artifact sets and run
metadata for both runs are archived under
`build/evidence/september-7-plan/ci-<run-id>/`; the cross-run inventory is
`pushed-ci-comparison.json`. The latest summary is not a replacement for the
less favorable first run. Local failed regression gates remain unresolved.

[Draft PR #1](https://github.com/PrimeLab-Foundation/strata/pull/1) triggers the
correctness matrix, which only runs on main pushes or pull requests.
[Correctness run 34143887223](https://github.com/PrimeLab-Foundation/strata/actions/runs/34143887223)
completed successfully, including all Python-version endpoints, coverage,
lint, fuzz sanitizers, and the macOS bindings sanitizer gate.
PGO runs inside each Benchmarks supportability job: `PGO+LTO build (gate tests on both phases)` on POSIX and `clang-cl PGO build (gate tests on both phases)`
on Windows. It is not a separate job in the correctness workflow.

## Pre-publication execution record

Branch: `work/september-7-plan`, based on
`3f45c9c751b1500b7b27992411209f1382670a91`. No commits or pushes were made.
The original three untracked review/plan documents were preserved.

This is a continuation of the [September 7 plan](fable-5.1-opus-5-plan-2026-09-07.md),
not a claim that its performance objective has been met. The current main
already contained the independently reviewed T1/T2/A-B repairs and E26-P6/P7
changes. The remaining implementation delivered here is principally canonical
provenance and supplementary measurement coverage.

| Task | Disposition                                                                                                                                                                                                                                                                                                                                                               |
| ---- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| T0   | Reconciled current source, prior reviews, Windows acceptance evidence, and the open E26-P8 experiment.                                                                                                                                                                                                                                                                    |
| T1   | Existing cleanup retained. Rebuilt a stale installed extension; the 100-call reproduction now grows neither text nor bytes keys. Lifetime suites and sanitizer gate passed.                                                                                                                                                                                               |
| T2   | Existing validity/coverage/transactional-fetch fixes retained. New companions are validated and transported; clean-source claims require a build commit. Incomplete manifests remain unverified.                                                                                                                                                                          |
| T3   | Canonical samples/provenance implemented, including build commands, profile inputs, raw precision, required companion markers, source/binary binding, and legacy compatibility. Existing A/B estimator/driver repairs retained. Six-block before/candidate, identical-binary A/A, and fresh-build controls completed locally; no effect clears the measured timing floor. |
| T4   | Prior E26-P6/P7 changes retained with their recorded acceptance evidence. No additional serializer change accepted.                                                                                                                                                                                                                                                       |
| T5   | Local experiment inconclusive: real small/medium parse/file-load rows show no effect above the A/A floor across the baseline/candidate or fresh-build comparisons. This bounds this session's signal; it does not attribute or close historical E26-P8. The shipped profile recipe is unchanged.                                                                          |
| T6   | Unresolved: native N2/Windows residual-row experiments and a final canonical comparison have not run on this branch.                                                                                                                                                                                                                                                      |
| T7   | Real-file phase control and exact-byte/mode checks implemented. Python phase timings are diagnostic; native syscall attribution and optimization acceptance remain pending.                                                                                                                                                                                               |
| T8   | Separate six-row supplementary scope implemented; representative medium/large CI scheduled separately. Python 3.10/3.14 endpoints added for macOS, Windows, and Linux ARM. Remote execution remains pending.                                                                                                                                                              |
| T9   | Local correctness gates and identified PGO builds passed. The complete matched canonical comparison failed regression thresholds at every tier; quiet-machine resolution remains pending. Exact final remote CI and two complete five-platform runs require human publication.                                                                                            |

Implementation details:

- `harness.py` retains samples and reads validated companions at full precision.
  `provenance.py` binds Markdown, samples, order, aggregates, and binary/source
  identities. Legacy unmarked Markdown retains its original precision.
- `setup.py` records actual compiler/linker commands through the installed
  setuptools command boundary, including Windows `/Tp` source arguments.
  Incomplete compilation cannot invent a source identity; a matching existing
  incremental identity is preserved. Facade copying excludes native binaries
  so wheel test gates cannot overwrite the new extension with an older one.
- `build_identity.py` records binary/tool/profile hashes and profile inputs.
  POSIX, clang-cl, and legacy MSVC PGO producers retain their existing recipes.
  Source archives without their own Git metadata explicitly lack a commit.
  `MANIFEST.in` includes the benchmark modules required by the sdist test gate.
- Summary, regression, supportability, and fetch tools validate companions.
  Fetch stages the full report/companion set before replacement. Display ties
  remain disclosed when raw precision resolves the rank.
- Supplementary NDJSON search queries the complete record list. Folder loops
  include discovery/grouping and must match native results/order or file bytes.
  Missing native or loop rows fail the supplementary gate. These six rows never
  change the canonical 27-per-platform denominator.
- The file diagnostic retains native dump and individually timed Python phase
  controls, checks short-write handling, descriptor cleanup, newline, final
  bytes, and POSIX mode repair. It does not infer native phase costs by
  subtracting medians.

Independent reviews examined provenance/packaging and supplementary semantics.
Their concrete findings were addressed: incremental and Windows attribution,
missing-companion downgrade, missing source commits, full-precision gating,
folder-control completeness, preflight metadata, and file-mode checks.

Local evidence is under `build/evidence/september-7-plan/`. These gitignored
artifacts must accompany a performance review; the curated ledger does not
replace raw evidence. The private-cache reproduction includes its binary hash.
Canonical JSON companions include the PGO binary and profile identities used
for each report, plus the independent checkout identity of the reporting tool.

Validation completed:

- Full compliance gate: 15 C++ suites, 2,237 Python tests, and 100% facade
  coverage. The supported ASAN/UBSAN Python gate also passed all 2,237 tests.
- Four further tooling regressions brought the final PGO gates to 2,241
  passing Python tests and 15 C++ suites in both production-build phases.
  Both candidate PGO builds passed. No native source changed afterward.
- Lint and changed-file pre-commit checks passed. Source distribution and
  wheel builds passed their gates; wheel and sanitizer extension hashes match
  their adjacent build identities. Source archives explicitly lack Git identity.
- Supplementary small: six native rows and three folder-loop controls passed
  with 10 repeats. The real-file diagnostic retained 60 samples and passed
  byte/newline, mode-repair, and descriptor checks.

The first candidate canonical observation was 26/27 small, 27/27 medium,
27/27 large. Small `dump mixed` measured 1.1450x msgspec. All three reports
passed the separate 3.0x supportability threshold. This observation preceded
the matched baseline sweep and remains archived separately as `canonical/`.

The baseline was built at detached `3f45c9c` in
`/private/tmp/strata-plan-before`, with its original runtime and 2,214 Python
tests. Only the setup/build-identity/POSIX profile-manifest instrumentation
was copied before building; the current reporting harness was copied after
PGO training. Each environment used its own interpreter installation with
pinned matching dependencies; all rival native binary hashes matched.

Three serial diagnostic campaigns each retained 25 launches (six ABBA blocks
and a trailing A), 60 samples per row, and 12 named workload rows (17 measured
Strata variants). `before-candidate.tsv` compares the baseline and first
candidate; `same-binary-aa.tsv` measures timing variation; `fresh-build-aa.tsv`
compares two freshly built candidate artifacts. The latter manifests have
identical workload-source hashes, recipe, and training data; raw profiles
differ. Minor reporting metadata/documentation edits between builds are
recorded in their distinct checkout identities and do not change native or
training source.

For the six parser/file-load rows, before/candidate normalized median effects
range from -0.24% to +0.28%, every interval includes zero, and A/A floors range
from 0.30% to 1.02%. Fresh-build effects range from -0.34% to +0.42%; all remain
below their A/A floors. Every serializer/file-write effect in both comparisons
also remains below its row's timing floor. Small `dump mixed` has a raw
before/candidate effect of +3.79%, normalized +0.41%, interval \[-4.92%, +2.98%\],
and A/A floor 2.90%; its rival also slowed. This is inconclusive, not a waived
regression or an accepted optimization. Host load varied during the campaigns.
The interleaved diagnostic protocol is distinct from the canonical harness.

Archived candidate SHA-256 identities:

- First: `b881360c58dacaf701b100b94d6daa3973051996bbfb315b3019b7266b043097`.
- Fresh: `959543f23a19cc9d0ba026fc5c74aabc92c1ae56f0ce899ade388ea0777e2aae`.

The fresh production PGO build is left installed. All original binaries,
metadata, merged profiles, raw profiles, and launch logs are retained under
`builds/` and the evidence root. No partial packet or unfavorable block was
discarded.

The subsequent matched canonical sweep measured the baseline first and fresh
candidate second, each across all tiers with 10 repeats and two warmups.
Both sets passed report/provenance validation and matched all 27 declared
rows per tier. Dataset and rival identities are retained in their companions.

| Tier   | Baseline first-place rows | Candidate first-place rows | Metric threshold breaches | Regression Make exit | Supportability Make exit |
| ------ | ------------------------- | -------------------------- | ------------------------- | -------------------- | ------------------------ |
| Small  | 27/27                     | 26/27                      | 19                        | 2 (failed)           | 0                        |
| Medium | 27/27                     | 27/27                      | 30                        | 2 (failed)           | 0                        |
| Large  | 27/27                     | 27/27                      | 17                        | 2 (failed)           | 0                        |

The underlying regression CLI exits 1, which Make reports as exit 2. These
are threshold failures with complete coverage, not invalid/missing reports.
The candidate small `dump mixed` row is 1.01253x msgspec; no rounded ties
occur in either set. Representative before/after failures are small
`dump mixed` median +23.4%, p95 +133.3%; medium `query users $[*].id` median
+32.5%; and large recursive-total query RSS +25.9%. All 66 metric breaches
remain in the three `*-regression.log` files. RSS is the canonical coarse
shared-process observation and is not attributed to Strata allocations.

**The regression gate failed and is unresolved.** Diagnostic controls below
the timing floor do not waive canonical failures, and first-place rows or a
3.0x pass do not establish acceptance. This session did not establish controlled
quiet-machine conditions; a quiet comparison must resolve the discrepancy
before any performance acceptance. No baseline was refreshed to remove these
failures. `matched-baseline.json` is isolated evidence; the repository's
published baseline and benchmark reports were left intact.

The existing complete historical CI samples both measure `79fa3df`, not this
branch. Their standings remain:

| Platform       | Run 34064158421 | Run 34064174240 |
| -------------- | --------------- | --------------- |
| Linux ARM64    | 26/27           | 26/27           |
| Linux x86_64   | 27/27           | 23/27           |
| macOS ARM64    | 27/27           | 27/27           |
| macOS x86_64   | 26/27           | 27/27           |
| Windows x86_64 | 26/27           | 25/27           |
| Total          | 132/135         | 128/135         |

The [first run](https://github.com/PrimeLab-Foundation/strata/actions/runs/34064158421)
and [second run](https://github.com/PrimeLab-Foundation/strata/actions/runs/34064174240)
are retained in the prior review/ledger. T4's
[Windows acceptance run](https://github.com/PrimeLab-Foundation/strata/actions/runs/34085356001)
measured `5411e60`; it is not a final all-platform standings run. No new
135/135 claim or quiet-machine regression pass is made.

Before performance acceptance, resolve the failed matched regression gate with
a valid quiet-machine comparison. Adding tests can change the gate-inclusive
PGO profile even without runtime-source edits.
After human publication of the reviewed final source, run the correctness
matrix and two complete five-platform benchmark runs of that exact SHA.

Suggested commit message, once acceptance dependencies are resolved:
`Retain benchmark provenance and add separate supplementary coverage`
