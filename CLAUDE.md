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
│   ├── context/             # always loaded: convention, styleguide, api, workflow
│   ├── architecture/        # skill doc: C++ core blueprint and invariants
│   ├── bindings/            # skill doc: CPython binding layer techniques
│   ├── jsonpath/            # skill doc: search/query grammar and evaluators
│   ├── build-and-test/      # skill doc: builds, tests, CI, fuzzing, coverage, PGO
│   ├── benchmarking/        # skill doc: benchmark suite, gating, standings
│   ├── performance/         # skill doc: optimization playbook + negative results
│   ├── history/             # skill doc: project lineage and ../archive/ map
│   └── benchmarks/          # machine-written benchmark results (generated; no folder until then)
├── .clang-format / .ruff.toml / .editorconfig / .markdownlint.yaml   # style configs
│
└── planned — the rebuild adds these (layout contract in docs/context/convention.md):
    ├── include/strata/      # public C++ headers
    ├── src/strata/          # C++ engine: json/, search/, util/ + bindings/ (CPython)
    ├── python/strata/       # thin Python facade
    ├── tests/               # cpp/, py/ (integration), unit/ (contract), fuzz/
    ├── benchmarks/          # harness vs orjson/msgspec/ujson
    ├── scripts/             # automation, fronted by the root Makefile
    └── experiments/         # isolated prototypes, never linked into production
```

## Fresh start — documentation only

**This branch contains no code by design.** The implementation was deliberately
removed; the project is to be rebuilt from scratch following the documentation
in `docs/`. The complete previous implementation (v0.2.0, all tests green) is
preserved on branch `backup/pre-reset-main` and in `../archive/` — file paths,
line numbers, and commit hashes cited throughout `docs/` refer to it. Use it as
the blueprint and correctness oracle, not as code to copy blindly: known bugs,
dead code, and broken tooling are called out in the docs and should not be
reproduced.

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

## Part 2 — Loaded on demand

Read these before working on the matching area. Each is a self-contained skill
doc (`SKILL.md` with YAML frontmatter) under `docs/`:

| Doc | Read when |
|---|---|
| `docs/architecture/SKILL.md` | Building or touching the C++ core: parser, DOM, serializers, NDJSON, mmap, error/memory model, invariants, dead-code map of the previous implementation |
| `docs/bindings/SKILL.md` | Building or touching the CPython bindings / Python facade: KeyCache, speculative keys, dumps fast paths, config→policy mapping, GIL/GC posture, CPython internals used |
| `docs/jsonpath/SKILL.md` | Search/query work: supported grammar, the three evaluators, SAX streaming search, known semantic gaps |
| `docs/build-and-test/SKILL.md` | Setting up builds, tests, CI, fuzzing, coverage, or PGO — includes the previous implementation's broken-tooling list (do not reproduce) |
| `docs/benchmarking/SKILL.md` | Running or designing benchmarks, the regression baseline, standings achieved by the previous implementation |
| `docs/performance/SKILL.md` | Optimizing anything — what worked (with commits and measured wins), what failed (negative results), and salvageable unmerged work |
| `docs/history/SKILL.md` | Project lineage, the archive repos in `../archive/`, and where past experiments live |

`docs/context/` and the skill folders above are curated documentation;
`docs/benchmarks/` is reserved for machine-written benchmark results once the
benchmark tooling exists again.
