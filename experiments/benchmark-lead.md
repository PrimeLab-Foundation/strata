# Benchmark-lead experiments

These are implemented prototypes, **not accepted production changes**. Both
were reverted after canonical regression failures. Their tests and raw local
measurements are described in
[the execution plan](../docs/performance/benchmark-lead-plan-2026-09-07.md).

| Patch                             | Hypothesis                                                                                                | Contracts included                                                                                                   |
| --------------------------------- | --------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- |
| `benchmark-schema-recovery.patch` | Scope schema retirement to one document; retain prepared schemas across calls                             | Recovery after churn/failure, owned keys, nested private cache, fused/general mutation parity                        |
| `benchmark-file-newline.patch`    | Append the newline before sealing private bytes, with a separate size hint; use contiguous partial writes | Boundary sizes, alternating output modes, serialize-before-truncate, real short-write failure and descriptor cleanup |

After the human pushes the branch, dispatch **A/B performance**
(`.github/workflows/ab_x86.yml`). Choose `none`, `schema-recovery`,
`file-newline`, or `combined` in `experiment`. The workflow checks and applies
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
