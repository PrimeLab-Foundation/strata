# Strata

Fast JSON for Python: parsing, serialization, and JSONPath querying powered by a
dependency-free C++20 engine with hand-written CPython C-API bindings (no pybind11).
Benchmarked against orjson, msgspec, ujson, and stdlib json; the previous
implementation was #1 in most categories (see `docs/benchmarking/SKILL.md`).

## Repository structure

> **Keep this tree current:** whenever a folder is added, removed, or changes
> purpose, update this section in the same change — and move entries out of
> the *planned* block as the rebuild makes them real.

```
.
├── CLAUDE.md                # this file — entry point and documentation index
├── README.md                # public-facing overview
├── LICENSE                  # MIT
├── docs/
│   ├── context/             # always loaded: convention, styleguide, api, workflow, benchmarks
│   ├── roadmap/             # skill doc: rebuild milestones with acceptance criteria
│   ├── decisions.md         # append-only decision log (ambiguities resolved visibly)
│   ├── architecture/        # skill doc: C++ core blueprint and invariants
│   ├── bindings/            # skill doc: CPython binding layer techniques
│   ├── jsonpath/            # skill doc: search/query grammar and evaluators
│   ├── build-and-test/      # skill doc: builds, tests, CI, fuzzing, coverage, PGO
│   ├── benchmarking/        # skill doc: benchmark suite, gating, standings
│   ├── performance/         # skill doc: optimization playbook + negative results
│   ├── history/             # skill doc: project lineage and ../archive/ map
│   └── benchmarks/          # machine-written results: tier reports, ci/ per platform-arch, ci_summary.md
├── .clang-format / .ruff.toml / .editorconfig / .markdownlint.yaml   # style configs
├── .pre-commit-config.yaml  # style gates: ruff, clang-format, mdformat, markdownlint
├── .github/workflows/       # ci.yml (matrix, coverage, style, corpus) + fuzz/benchmark/pgo
├── pyproject.toml           # PEP 621 metadata; version read dynamically from the facade
├── setup.py                 # extension build + the two test gates (TestGatedBuildExt)
├── CMakeLists.txt           # the single C++ test registry (ctest)
├── MANIFEST.in              # sdist contents
├── Makefile                 # the single user-facing interface; targets forward to scripts/
├── scripts/                 # automation: cpp_tests, py_tests, asan_py_tests, fmt, lint, gate, coverage, fuzz, pgo_*
├── include/strata/          # public C++ headers (core; never CPython)
│   ├── json/                # value model, SAX handler, parser, parse + serialize API
│   └── util/                # scan.hpp (utf-8/whitespace/escapes), fast_parse.hpp, dtoa.hpp
├── src/strata/
│   ├── core_sources.txt     # the single core source list, read by CMake and setup.py
│   ├── json/                # json_parse.cpp (DOM builder), json_serialize.cpp
│   ├── search/              # jsonpath_compile.cpp, jsonpath_eval.cpp
│   ├── util/                # scan.cpp, dtoa.cpp, folder.cpp
│   └── bindings/            # CPython layer: module, loads, dumps, files, ndjson, cursor
├── python/strata/           # thin facade: __init__, serialize (loads/dumps), config
├── tests/
│   ├── cpp/                 # assert-based suites, registered in CMakeLists.txt
│   ├── py/                  # integration tests
│   ├── unit/                # clause-by-clause contract suite
│   └── fuzz/                # libFuzzer targets (opt-in -DFUZZ=ON) + committed seed corpus/
│
├── benchmarks/              # harness, datasets, regression gate and the CI fetch/summary tools
└── experiments/             # isolated prototypes, never linked into production (itoa/ and dtoa/: the number-writer variants; footprint/: the serializer's table and state sizes; benchmark-*.patch: gated, unaccepted serializer prototypes)
```

## Rebuild in progress

**The implementation was deliberately removed and is being rebuilt from scratch**
following the documentation in `docs/`, one milestone per session
(`docs/roadmap/SKILL.md`). Landed so far: M0 (scaffolding — build, both test
layers, style gates, CI skeleton), M1 (core value model: `JsonValue`,
`FlatMap`, `Status`/`Result`), M2 (SAX parser, DOM builder, `parse_json`),
M3 (serializer and shortest round-trip float formatting) M4 (the binding layer: `loads`, `dumps`,
`config`), M5 (benchmark harness, regression gate and a first performance
layer) M6 (file I/O, NDJSON and cursor
mode), M7 (JSONPath: `query`, `search`, `compile`) and M8 (folder mode for
`load`, `dump` and `search`), M9 (fuzzing with a committed corpus, coverage,
PGO+LTO, full CI) and the M10 performance waves, closed by the quiet-machine standings sweep, and the post-release waves 11–25 (the digit-word float writer, the parse-side digit runs and short-number head, the prediction cursor, the serializer's cold footprint, the exact-fit output block with its staged tail, the short-string scan, the tiered integer writer, the clang-cl profile-guided Windows build, the parse side's short-integer dispatch with the number head's second word, the head's long-fraction step for full-precision doubles, the builder's one-pass string copy, the key predictor's per-input retirement, the parse without a UTF-8 sweep ahead of the builder, the wave-24 digit chain and float head-group word for the Neoverse-N2 rows, and the wave-25 value cursor — the array element loop holding the builder's staging cursor, behind an explicit 1024-container nesting cap). Standing at 2026-09-04: local rolls read medium and large 27/27 and, after wave 22 scoped the key predictor's retirement to one input (the coin band's cause: mixed.json's record depth stayed retired for the thread's life after other datasets), small 27/27 on three of four genuinely quiet draws (loads mixed 0.95x, 1.00x, 1.00x, the fourth a 1.01x tie); the CI standings (`make bench-ci`) read 128–131/135 across four five-platform samples (2026-09-05, the repository public and the Linux arm64 leg live): Linux x86_64 27/27 on every draw, macOS x86_64 on three, Windows once at 27/27 with its `dumps mixed` band reaching parity (1.09x, 1.03x, a 1.00x tie, 1.02x), macOS arm64 25–26/27 on a different spiky shared-runner row each draw, and Linux arm64 (a Neoverse-N2) 24/27 on every draw — `loads`/`load wide_arrays` at 1.04–1.10x and `dumps mixed` at 1.03–1.11x, the one persistent deficit, in the number code that core does not overlap, annotated instruction by instruction in docs/decisions.md. The wave-25 tree (2026-09-06) rolled locally at load 3–5 — small and large 27/27, medium 26/27 with `dump mixed` at 1.04x of msgspec on a busy desktop — and read no row worse than the +0.6% noise band against HEAD's own PGO build in an in-process A/B (`loads users` −2.9%, `mixed` −4.2%, `wide_arrays` +0.4%, `dumps mixed` −7.0%); published as 32c5fa4 (2026-09-06), its two five-platform samples read 129/135 and 134/135 with the Neoverse-N2's parse rows at #1 on both (`loads wide_arrays` 0.94x, `load wide_arrays` 0.81–0.83x) and one row left behind: linux-arm64 `dumps mixed` at 1.03x on both draws. Wave 26 (2026-09-06) rejected three levers on that row, and the campaign's P0 then corrected its mechanism: strata leads hot (0.97x) and at parity alone under GC (0.995x); it trails only when rivals run between its calls in the same process (one rival 1.00–1.02x, five 1.03–1.09x), and the i-cache figure the wave quoted was simulated on a plain rebuild (on a PGO+LTO build the surplus is 98 misses per cold call, not 2,283) — docs/performance/experiment-ledger.md. The campaign of 2026-09-06 (docs/performance/fable-5.1-opus-5-improvement-plan.md; ledger docs/performance/experiment-ledger.md) found and fixed a use-after-free in `dumps` under re-entrant mutation and a surrogate-key defect that produced invalid JSON, and tried a training-only PGO profile (the shipped one is 47.5% test suite) that E26-P5b then withdrew: on the fixed source it costs +3.3–6.3% on each of the three serializer rows measured and the whole of the +1.5–1.7% `loads wide_arrays` lean, while the source fixes alone under the shipped recipe read `dumps flat` −8.5%/−8.1% (small/medium), `dumps users` −6.6%/−6.5% and `dumps mixed` −2.7%/−3.0% — the small mixed cell inside its own A/A floor — against the shipped build in a drift-free window with the parse rows unmoved; both independent P5 reviews of the first composition returned not refuted, the recomposed candidate (fixes under the shipped recipe) rolled small and medium 27/27 and large 27/27 on a second draw (the first read `dump mixed` 1.25x on a busy desktop; both draws recorded in the ledger's E26-P5 entry), and, published as 79fa3df (2026-09-07), its two five-platform samples read 132/135 and 128/135 — macos-arm64's serializer rows improved as the M1 window predicted (`dumps flat` 0.94x → 0.83x, `users` 0.73x → 0.66x), the N2's in-memory serializer rows did not move (`dumps mixed` 1.01–1.04x), the x86 legs' serializer rows moved against strata with orjson flat (linux-x86_64 `dumps flat` 0.78x → 0.83x, macos-x86_64 0.77x → 0.84x; a same-runner A/B of the two revisions on an EPYC then read `dumps wide_arrays` +7.0%, `dumps flat` +3.6–4.0%, parse rows unchanged), and macos-arm64's parse rows moved +0.04–0.05 in ratio on unchanged parse source (a profile-layout candidate: the gate-inclusive recipe absorbed about 180 new serializer tests) — both E26-P6 in the ledger; the x86 mechanism was found by a static codegen diff (SysV's register budget: the element loop's live re-read of the list, `write_mapping_body`'s sixth argument, the fused writer's out-lining), confirmed by same-runner A/B runs on both x86 machine classes, and fixed on `exp/p6p7-integration` (a user-code step counter makes the array loop reload a list's bounds only when user code ran; `write_mapping_body` back to four arguments; `-fomit-frame-pointer` on x86-64): with the profile held equal the code alone reads linux-x86_64 `dumps wide_arrays` −5.6% and `dumps users` −2.4%, arm64 neutral, and the flag takes the i7's medium `dumps flat` and `mixed` by 1–2% of strata's own time on the quiet draw (the larger normalised readings were orjson slowing); the same runs found that the gate-inclusive PGO recipe makes every test addition a several-percent change on some row (E26-P8, a training-only profile with a fuller training workload). The plan of 2026-09-07's P0 items — the re-entrant lease's private schema keys released (T1, its destructor priced at zero on three legs), evidence gates on a declared 27-row workload with a staged fetch (T2), one blocked, bootstrapped A/B estimator with a self-restoring driver (T3) — are integrated with E26-P6/P7 on `exp/p0p6-integration` after independent reviews, and the Windows check met T4's cross-platform clause (small `dumps flat` −2.0%, medium `dumps users` −4.4%, nothing worse past its floor). The benchmark-lead continuation (2026-09-07 to 09-10, `work/september-7-plan` and `work/benchmark-lead`) added canonical provenance (a JSON companion of raw samples beside every new report, `*.build.json` beside every built extension, gates that read full precision), the six-row supplementary scope, a five-platform same-runner A/B workflow with isolated-patch selectors and a full-canonical mode, and 3.10/3.14 CI legs on every platform; its two five-platform samples of 75cfb42 (source identical to main's) read 128/135 and 133/135 with only linux-arm64 and windows `dumps mixed` behind on both draws (1.02x and 1.01x on the second). It accepted no runtime change: fifteen serializer prototypes are kept as `experiments/benchmark-*.patch` with ledger entries E26-P9 to E26-P22, all no-go except E26-P9 (exact dicts reached through the general writer routed to the existing fused record writer), which reproduced on two native draws — N2 `dumps mixed` −5.2%/−5.6% small and −5.0%/−5.3% medium, macos-x86_64 −4.9%/−8.6% — reached 27/27 on the N2 in a full canonical native run, and was held only by the canonical regression gate, which the same-session identical-binary control (16 breaches on the dev Mac; 10–48 on the runners) shows to be undecidable under load. Its review then found the fused writer had never probed the open-container stack (a warmed repeated dict reached as a list element was emitted once more before the placeholder, on main too); the revival on `exp/p9-fused-dicts` (2026-09-11) probes on the new value path only, hands a cycle to the general writer, leaves the element loop as it was with that gap recorded in docs/decisions.md, and against the tests-only arm on all five runners reads N2 `dumps mixed` −5.8%/−6.1%, `dumps users` −3.5%, EPYC `dumps flat` −4.5%, gains on both Macs, no Windows row resolved past its floor, one resolved loss of +0.3% — accepted for integration pending the plan's two five-platform samples of the merged revision. What remains is that integration, E26-P8, the coin band, the deferred JSONPath grammar extensions, and the backlog in `docs/benchmarking/SKILL.md`. The rebuild is versioned calver,
`YYYY.M.D` of release — started at `2026.8.9`, released as `2026.8.10`
(see `docs/context/api.md`).

The complete previous implementation (v0.2.0, all tests green) is preserved on
branch `backup/pre-reset-main` and in `../archive/` — file paths, line numbers,
and commit hashes cited throughout `docs/` refer to it. Use it as the blueprint
and correctness oracle, not as code to copy blindly: known bugs, dead code, and
broken tooling are called out in the docs and should not be reproduced.

## Target interface (what the rebuild must provide)

```bash
make install        # editable install — C++ tests gate the build, Python tests gate the result
make test           # both test suites
make bench-small    # benchmark vs orjson/msgspec/ujson → docs/benchmarks/
make fmt lint       # ruff format + clang-format; ruff check
make gate           # full compliance gate: C++ tests → reinstall → Python tests → coverage
make pgo            # PGO+LTO two-phase optimized build
```

Never finish a session with failing tests. Never commit benchmark regressions —
see the benchmark gate in `docs/context/convention.md`.

## Part 1 — Always loaded

The following context files are imported into every session:

@docs/context/convention.md
@docs/context/styleguide.md
@docs/context/api.md
@docs/context/workflow.md
@docs/context/benchmarks.md

## Part 2 — Loaded on demand

Read these before working on the matching area. Each is a self-contained skill
doc (`SKILL.md` with YAML frontmatter) under `docs/`:

| Doc                            | Read when                                                                                                                                                              |
| ------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `docs/roadmap/SKILL.md`        | **Every implementation session** — pick the current milestone; one increment per session, acceptance criteria = definition of done                                     |
| `docs/architecture/SKILL.md`   | Building or touching the C++ core: parser, DOM, serializers, NDJSON, mmap, error/memory model, invariants, dead-code map of the previous implementation                |
| `docs/bindings/SKILL.md`       | Building or touching the CPython bindings / Python facade: KeyCache, speculative keys, dumps fast paths, config→policy mapping, GIL/GC posture, CPython internals used |
| `docs/jsonpath/SKILL.md`       | Search/query work: supported grammar, the three evaluators, SAX streaming search, known semantic gaps                                                                  |
| `docs/build-and-test/SKILL.md` | Setting up builds, tests, CI, fuzzing, coverage, or PGO — includes the previous implementation's broken-tooling list (do not reproduce)                                |
| `docs/benchmarking/SKILL.md`   | Running or designing benchmarks, the regression baseline, standings achieved by the previous implementation                                                            |
| `docs/performance/SKILL.md`    | Optimizing anything — what worked (with commits and measured wins), what failed (negative results), and salvageable unmerged work                                      |
| `docs/history/SKILL.md`        | Project lineage, the archive repos in `../archive/`, and where past experiments live                                                                                   |

`docs/context/` and the skill folders above are curated documentation;
`docs/benchmarks/` is reserved for machine-written benchmark results once the
benchmark tooling exists again.
