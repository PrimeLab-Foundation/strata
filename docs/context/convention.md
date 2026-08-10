# Project Conventions

Canonical rules for working on strata. These consolidate the former
`.aiassistant/rules/*` and `.junie/guidelines.md` (both removed — this file
is the single source of truth).
North star: correctness first, then performance, then maintainability.

## Architecture boundaries

1. **C++ owns CPU work.** All CPU-heavy logic (parsing, serialization, NDJSON,
   search/query evaluation) is implemented in C++20. Python is a thin facade:
   argument normalization, dispatch, exception mapping. Never implement parsing,
   search, or slicing in Python "temporarily" — if it isn't in C++, raise
   `NotImplementedError`.
2. **Core purity.** The C++ core (`include/strata/`, `src/strata/{json,search,util}`)
   must never include `Python.h`. All CPython code lives in `src/strata/bindings/`.
   The official architecture is Hybrid SAX: the core emits SAX events
   (`JsonSaxHandler` / templated `ParserInline`), and builders above it construct
   either the C++ DOM (`DomBuilderHandler`) or Python objects (`PythonObjectBuilder`
   in bindings). Changes to this architecture require a written decision record in
   `docs/architecture/`.
3. **No dependency fallbacks.** The core depends on no other JSON engine
   (no simdjson/rapidjson/orjson/ujson). Competitor libraries appear only in
   `benchmarks/`. Performance claims must be attributable to strata alone.
4. **No pybind11.** Bindings use the raw CPython C API (see `docs/bindings/SKILL.md`).

## Layout and naming

- C++ sources `src/strata/`, public headers `include/strata/`, Python package
  `python/strata/`, C++ tests `tests/cpp/`, Python tests `tests/py/` + `tests/unit/`,
  benchmarks `benchmarks/`, automation `scripts/`, experiments `experiments/`.
- Documentation lives in `docs/`: `docs/context/` (always-loaded context) and
  `docs/<topic>/SKILL.md` (curated skill docs); `docs/benchmarks/` is reserved
  for machine-written benchmark results only.
- Public API stays small and stable. Every new public API needs: C++ tests +
  Python tests, a `docs/` update, and benchmark coverage if perf-relevant.
- Keep files under ~800 LOC; split by responsibility unless there is a documented
  compelling reason.

## Testing gates

- **Green gate:** never end a session with failing tests, on either layer. Run both
  C++ and Python suites even for single-layer changes. No skip/xfail to hide failures.
- **Cross-layer parity:** every behavior exposed to Python has mirrored tests —
  C++ core tests plus Python contract tests (same outputs, same error categories).
  `tests/py/` = integration; `tests/unit/` = contract mirrors. When you change a
  mirrored file, update both copies.
- **Test-gated builds:** `pip install -e .` / `make install` runs the C++ suite
  before compiling and the Python suite after (setup.py `TestGatedBuildExt`).
  Escape hatch `SKIP_TESTS=1` is banned in CI and releases.
- Coverage target is 100% on both layers; exclusions must be minimal and documented.

## Benchmark discipline

- Any change touching a hot path (parse, serialize, NDJSON, search, allocations,
  compiler flags) requires before/after benchmarks against the competitor set.
- Regression thresholds, protocol, and fairness rules: the canonical contract
  is `docs/context/benchmarks.md` — a threshold breach means do not proceed;
  fix or revert.
- Release positioning ("production-ready") requires #1 rank in the targeted
  benchmark categories, with a reproducible evidence report under `docs/benchmarks/`.

## Memory and errors

- Allocation count and lifetimes are first-class review criteria on hot paths:
  reason explicitly about allocations, RAII ownership, O(n²) avoidance, and
  zero-copy view safety.
- C++ hot paths use `Status`/`Result<T>` — no exceptions in tight loops. The
  binding layer translates C++ errors to stable Python exceptions (`ValueError`
  for parse errors, `TypeError` for type misuse; messages are part of the test
  contract — see `docs/context/api.md`).

## Experiments

- Research prototypes live under `experiments/`, never linked into production.
  Each experiment needs a hypothesis, a minimal prototype, microbenchmarks, and a
  written go/no-go conclusion (record it in `docs/performance/SKILL.md`).
  Read the negative-results table there before re-attempting an old idea —
  several plausible techniques have already lost on measurement.

## Platform supportability

- **Supported platforms** — strata is supportable on all main desktop
  platforms: Linux (x86_64 and arm64), macOS (arm64 and x86_64), and Windows
  (x86_64), on CPython 3.10–3.14. Every one of these has a leg in the CI test
  matrix; a platform is not "supported" because the code looks portable, but
  because CI builds and passes both suites on it.
- **Portable by construction.** Every platform-specific fast path — SIMD
  (NEON/SSE2), SWAR, MSVC intrinsics, CPython-version-gated internals — must
  have a portable fallback with identical observable behavior, selected at
  compile time or proven by a runtime probe, never assumed. The scalar-twin
  test rule (styleguide) and the raw-dict runtime proof are this convention
  applied; new fast paths follow the same shape.
- **Cross-platform benchmark pipeline.** CI runs the *same* benchmark suite
  (same datasets, same harness, same fairness rules) on platforms other than
  the primary development machine — at minimum one non-matching OS *and* one
  non-matching CPU architecture. Its gate is a **supportability tripwire**,
  not a standings claim: the run must produce no ERROR rows, strata must
  complete every category, and no row may fall behind the best rival by more
  than the documented bound (`benchmarks/supportability_check.py`). The bound
  is deliberately loose — it exists to catch a broken or accidentally-scalar
  fast path on hardware the dev machine cannot exercise, not shared-runner
  noise.
- **Absolute times never cross platforms.** The committed regression baseline
  is same-machine, same-session evidence only (see
  `docs/context/benchmarks.md`); CI must not compare its runs against it, and
  headline standings come exclusively from the documented quiet-machine
  protocol. CI benchmark reports are uploaded as artifacts for human reading.

## Automation

- All automation is a script in `scripts/`; the root `Makefile` is the single
  user-facing interface (thin targets forwarding to scripts). Docs and CI reference
  Make targets, not ad-hoc one-liners.

## Agent output format

When reporting completed work, list: files changed; key code decisions (lifetimes,
errors, allocations); tests added/updated on both layers with commands run;
benchmark deltas if a perf path was touched; risks and follow-ups.
