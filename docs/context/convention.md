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
- Regression thresholds: median or p95 worse by >2% in a touched category, or
  RSS/peak memory worse by >5% ⇒ do not proceed; fix or revert. Fairness: same
  datasets, warmup + repeats, report min/median/p95, no winning by changing semantics.
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
  structural tape, object pooling, pre-counting, and memo cycle detection have
  all already lost on measurement.

## Automation

- All automation is a script in `scripts/`; the root `Makefile` is the single
  user-facing interface (thin targets forwarding to scripts). Docs and CI reference
  Make targets, not ad-hoc one-liners.

## Agent output format

When reporting completed work, list: files changed; key code decisions (lifetimes,
errors, allocations); tests added/updated on both layers with commands run;
benchmark deltas if a perf path was touched; risks and follow-ups.
