# Benchmark-lead experiments

These are implemented prototypes, **not accepted production changes**. All
were reverted after canonical regression failures. Their tests and raw local
measurements are described in
[the execution plan](../docs/performance/benchmark-lead-plan-2026-09-07.md).

| Patch                             | Hypothesis                                                                                                | Contracts included                                                                                                   |
| --------------------------------- | --------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- |
| `benchmark-schema-recovery.patch` | Scope schema retirement to one document; retain prepared schemas across calls                             | Recovery after churn/failure, owned keys, nested private cache, fused/general mutation parity                        |
| `benchmark-file-newline.patch`    | Append the newline before sealing private bytes, with a separate size hint; use contiguous partial writes | Boundary sizes, alternating output modes, serialize-before-truncate, real short-write failure and descriptor cleanup |

After the human pushes the branch, dispatch **A/B performance**
(`.github/workflows/ab_x86.yml`). Choose `none`, `schema-recovery`,
`file-newline`, `nested-mappings`, or `combined` in `experiment`. The workflow checks and applies
exactly those patches before building the candidate with production PGO and
its full test gates. Both binaries, identities, profiles, selected patch and
raw samples are uploaded. Baseline and candidate run on the same machine;
A/A uses the same block/sample counts. Windows and Linux ARM64 are included.

Start with `none` and both refs at the same published revision to measure
fresh-build variation. Then test isolated prototypes against `75cfb42`.
Passing a diagnostic does not update canonical standings: accepted changes
must subsequently be applied, human-committed and benchmarked as a clean
production revision twice across all five platforms. Update `ci_summary.md`
through `make bench-ci` when those complete reports are ready.

For local inspection, start from the branch's unmodified production sources:

```sh
git apply --check experiments/benchmark-schema-recovery.patch
git apply experiments/benchmark-schema-recovery.patch
git add -N tests/unit/test_dumps_schema_recovery.py
make gate
make test-py-asan
make pgo
make probe-schema-recovery
```

`git apply --check` rejects a conflicting tree. To remove a prototype, reverse
only its own patch after checking it with `git apply --reverse --check`.
Do not replace unrelated edits. Preserve each `.build.json` beside an archived
binary: the A/B CLI now refuses missing or hash-mismatched identities and
known incompatible versioned Python headers. Never choose an extension using
a glob when multiple Python versions exist in the package directory.

## Nested exact mappings (E26-P9)

`benchmark-nested-mappings.patch` routes exact dictionaries through the
existing guarded, out-of-line fused writer. Includes fresh-thread cached
root/nested mutation tests with reentrant clearing. Both matched-test PGO
builds passed 2,249 tests; the candidate also passed ASan/UBSan. Six-block
comparisons showed about 9.5% lower nested serialization time and 2.2% lower
mixed time, but both canonical comparisons failed regression gates, including
the predeclared 60-sample confirmation. Therefore the patch is not production
code. Select `nested-mappings` for native investigation after publication;
`combined` continues to mean schema recovery plus file newline only.

Canonical Make targets now accept `BENCH_REPEAT` and `BENCH_WARMUP`; their
defaults remain 10 and 2. Increasing samples preserves the workload and gate
thresholds. It does not make a failed regression pass.

For `nested-mappings`, the baseline worktree receives the patch's test-file
change too; those contracts pass on both implementations. Pin both refs to
the same published revision for matched test/training source, as in the local
comparison. Only the candidate receives the runtime dispatch change.

## Full canonical validation before integration

The native A/B workflow also accepts `validation=canonical`. It builds the
same two arms, then runs the complete 27-row small-tier canonical suite for
each in a fresh process. It applies the unchanged median/p95/RSS regression
thresholds and fails the job on a breach. Reports, raw samples, provenance,
comparison baselines and gate output are archived under `ab/canonical/` even
when the gate fails. The default `paired` mode retains the selected-row ABBA
and A/A experiments.

After publication, pin both refs to that same revision and select
`experiment=nested-mappings`, `validation=canonical`, `repeat=60`. This closes
the full-workload evidence gap in the earlier native selected-row runs.
Patched diagnostic builds still cannot replace clean canonical CI standings.

Locally, already-built compatible binaries can be checked without replacing
the installed extension or its metadata:

```sh
make probe-canonical-builds CANONICAL_BUILD_FLAGS="--before build/evidence/A.so --candidate build/evidence/B.so --output build/evidence/canonical-pair --tier small --tier medium --tier large --repeat 60"
```

Keep the build-produced `.build.json` beside each binary and generate the
datasets first with `make bench-data`. An existing output directory is refused
so a later run cannot overwrite failed evidence. Each arm uses a staged copy
of the same facade and validates the imported extension's location. A child
failure leaves the installed package untouched.

## Linux ARM64-only nested mappings (E26-P9a)

`benchmark-nested-mappings-linux-arm64.patch` keeps P9's guarded dispatch only
when both `__linux__` and `__aarch64__` are defined. Other platforms retain
`write_mapping`; the public API, cached row ownership and fallback rules are
unchanged. The four mutation cases remain part of both arms' training sources.
Select `experiment=nested-mappings-linux-arm64` in the native A/B workflow,
with both refs pinned to the same published revision.

This narrows an unaccepted experiment; it does not resolve or waive the two
Linux ARM64 p95 breaches from run 34253218374. Its Linux path is the existing
P9 implementation, while the other targets keep their original dispatch.
The full Darwin translation unit preprocesses identically before and after,
and platform-selection checks cover all five targets. No extra local timing
claim is made from code that preprocesses identically on the development Mac.

The unchanged-source canonical control is
[34257791864](https://github.com/PrimeLab-Foundation/strata/actions/runs/34257791864).
The existing profiling workflow is running at
[34257653984](https://github.com/PrimeLab-Foundation/strata/actions/runs/34257653984).
Both were dispatched from `5bbec773f53580b4adf1f1c6c86c5e01d725cfe3` before
this revision; they cannot validate the new platform selection.

The updated profiling workflow accepts `scope=windows`. It additionally
archives the initial PGO extension and build metadata, plus 60-sample real-file
phase controls for mixed/flat/nested under `profile-windows-pgo`. Collection
happens before the later plain-toolchain rebuilds. Phase controls use Strata's
Python syscall composition and are diagnostic, not rival standings or native
instruction attribution. This collection requires publication of the updated
workflow; it is not part of the already-running profiling job.

## Resumed scalar runs (P10)

`benchmark-resume-scalar-runs.patch` lets the main sequence loop resume the
existing exact float and compact ASCII string writers after another element
kind. It passes an absolute start index, preserving the comma rules; an
escaped, non-ASCII or long string returns to the existing general writer.
The depth-boundary loop retains its original implementation. There is no
new scalar classification cache or float conversion algorithm.

Inputs, emitted JSON and errors are unchanged. The resumed loops hold only
borrowed exact scalar pointers and cannot invoke Python. After a callback,
the outer loop still refreshes the list size and item pointer before the next
iteration. Existing output reservations cover float blocks and string copy
scratch. Twelve focused cases cover block boundaries, separators, escape and
Unicode fallback, non-finite floats, and callback-driven tail replacement.

Hypothesis: recovering direct scalar emission after a container or type
transition reduces the heterogeneous-list dispatch cost seen in the Windows
PGO profile. Risk: extra checks on record-heavy lists and repeated failed
string eligibility checks may outweigh that saving. Build both arms with the
same tests and gate-inclusive PGO; reject on unresolved regressions. The patch
is an unaccepted experiment and is not linked into production.

Local outcome: no-go. Matched-test PGO and the twelve new cases pass, but
six-block paired screening finds no mixed gain and slower nested/wide-array
serialization beyond the session control floors. No Windows claim follows
from this Mac result. The production source, tests and binary are restored;
see E26-P10 in the ledger for retained measurements and hashes in evidence.

## Full-precision significand digit count (P11)

`benchmark-float-digit-count.patch` replaces the generic digit counter only
for Dragonbox significands at or above 10^15: binary64's maximum seventeen
significant digits make the answer 16 plus a comparison against 10^16.
Shorter results keep the existing counter. Conversion, trailing-zero removal,
short-decimal probing, layout, errors and allocation remain unchanged.

Hypothesis: avoid the bit-width estimate and dependent power-table access on
full-precision float emission. The extra threshold branch may cost shorter
results; matched PGO and mixed/record/array controls decide whether to proceed.
Existing float precision tests compare the full writer with an independent
reference over boundary values and random bit patterns, beyond a round-trip
check alone. This remains an isolated experiment pending measurement.

Local P11 outcome: no-go for integration. Both PGO arms pass all tests, but
the 60-sample full small-tier comparison fails ten regression checks despite
27/27 standings in both arms. The isolated `float-digit-count` native A/B
selector is available for investigation after publication; Windows has not
measured this patch. See E26-P11 in the ledger for all retained results.

## First-key-first fused schema lookup (P13)

`benchmark-schema-key-first.patch` reverses two side-effect-free comparisons
in the fused record writer's four-way cache lookup. First-key identity is
checked before record size. Three of mixed.json's four schemas have three
fields, so size rejects few candidates; first-key identity distinguishes all
four. Hypothesis: short-circuit the count load on unrelated keys. The compiler
may already combine or reorder the loads, making the change ineffective.

The match predicate, chosen way, fallback, cached-key ownership, staged row,
reentrancy protection, output and errors are identical. Existing schema and
mutation tests cover this unchanged predicate. Compare matched gate-inclusive
PGO builds and retain the patch only as an experiment until all gates pass.

P13 outcome: no-go. Both PGO builds pass all tests; the disassembly confirms
exactly sixteen changed instruction words and unchanged code layout. Six
paired blocks and the identical-binary control resolve no mixed bytes gain.
Keep the patch isolated; do not promote it on the load-count hypothesis alone.

## Cached-key and compact-integer reservation (P14)

`benchmark-record-int-reserve.patch` reserves space for the cached key slot
and exact compact integer together, then formats the integer directly. It
avoids the general value dispatch and second capacity check for that case.
All other values retain the original emission and ownership path; the shortcut
is compiled only with CPython 3.12+'s compact-integer API. The architecture
note in `docs/architecture/fused_record_writer.md` gives the reservation and
lifetime proof. Two fresh-thread cases exercise cached 24-field records,
integer boundaries and output growth in str/bytes modes. Matched PGO baseline
and candidate use identical tests; this remains an isolated prototype.
