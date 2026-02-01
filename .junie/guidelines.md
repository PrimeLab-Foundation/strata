# Strata Developer & Agent Guidelines

This document serves as both the operating contract for the Junie agent and a guide for developers working on the Strata JSON engine.

## Developer Guide

### Build and Configuration

- **Initialize**: `make dev` (sets up venv and dev tools).
- **Standard Install**: `make install` (gated by tests).
- **Full Validation**: `make gate` (runs C++ tests, build, Python tests, and coverage).
- **PGO Build**: `make pgo`.
- **LTO**: Enabled via `STRATA_ENABLE_LTO=1`.

### Testing

- **Run all**: `make test`
- **C++ only**: `make test-cpp` (executes `scripts/run_cpp_tests.sh`).
- **Python only**: `make test-py` (executes `pytest tests/py/`).
- **Adding C++ tests**: Create `tests/cpp/test_*.cpp` with `main()` and `assert()`. Add to `scripts/run_cpp_tests.sh`.
- **Adding Python tests**: Create `tests/py/test_*.py` using `pytest`.

**Example C++ Test**:

```cpp
#include "strata/json/json_core.hpp"
#include "strata/json/json_parse.hpp"
#include <cassert>

int main() {
    auto result = strata::parse_json("{\"demo\": true}");
    assert(result.ok() && result.value.is_object());
    return 0;
}
```

**Example Python Test**:

```python
import strata
def test_demo():
    assert strata.loads('{"demo": true}') == {"demo": True}
```

### Development Workflow

- **Formatting**: `make fmt` (runs `ruff` and `clang-format`).
- **Linting**: `make lint` and `make typecheck`.
- **Benchmarking**: Required for hot-path changes. Use `make bench-small` and update `docs/benchmarks/progress_log.md`.

______________________________________________________________________

## Junie Agent Operating Contract

## 1) North Star

- **Correctness first, then performance, then maintainability.**
- Strata’s product claim is **best-in-class speed with low memory** for:
  - `loads` / parsing
  - `dumps` / serialization
  - NDJSON streaming/iteration
  - search/query (JSONPath-like or project-defined)

______________________________________________________________________

## 2) Architecture Boundaries (Non‑Negotiable)

### C++ owns all CPU-heavy work

Implement in **C++20 core**:

- JSON parsing (`loads`)
- NDJSON iteration/streaming
- serialization (`dumps`)
- view/slice/zero-copy mode (when applicable)
- search/query evaluation

### Python is a thin wrapper only

Python may do:

- argument normalization
- calling into **existing** native entrypoints (as-is)
- exception mapping (Python-side only)
- return formatting

**Forbidden:**

- implementing parsing/search/slicing in Python as a “temporary” solution
  - If not implemented in C++, **raise `NotImplementedError`**.
- changing any native binding layer to “make it work”.

### No dependency fallbacks

Core must **not** delegate to other JSON engines (rapidjson/simdjson/orjson/ujson/etc).
Competitor libs are allowed **only** inside **benchmark harnesses**.

______________________________________________________________________

## 3) Repository Layout + Naming

### Layout

- C++ production code: `src/`
- C++ public headers: `include/`
- Python package: `python/strata/` (or `src/strata/` if repo uses src-layout)
- C++ tests: `tests/cpp/`
- Python tests: `tests/py/`
- Benchmarks: `bench/` or `benchmarks/`
- Docs: `docs/` only
- Legacy/experimental: `cpp_bkp/` or `experiments/` (must not be linked/imported into production)

### Naming conventions

**C++**

- namespace: `strata` (plus nested namespaces like `strata::json`, `strata::ndjson`, `strata::jsonpath`)
- types: `PascalCase`
- functions/methods: `snake_case`
- constants: `kPascalCase`
- filenames: `snake_case` for `.hpp/.cpp`
- prefer `#pragma once`
- keep public headers minimal; internal headers go in `src/` or `include/strata/detail/`

**Python**

- modules/functions: `snake_case`
- public classes: `PascalCase`
- exports are minimal via `strata/__init__.py`
- internal helpers prefixed with `_` and not exported

______________________________________________________________________

## 4) Documentation Rules

### Docs live only in `docs/`

- All design notes, ADRs, API docs, benchmark methodology/results, and performance notes **must** be under `docs/`.
- Root `README.md` (if present) should be minimal and link into `docs/`.
- All `.md` filenames should be lowercase **except** `README.md`.
- Do not create scattered `.md` outside `docs/`.

______________________________________________________________________

## 5) Public API Discipline

### Keep the surface small and stable

Example target surface:

- `loads`, `dumps`, `iter_ndjson`, `search`, `compile_jsonpath`

Adding any new public API requires:

1. C++ tests + Python tests (where exposed)
1. docs update under `docs/`
1. benchmark coverage if performance-relevant

**Reminder:** Junie must not modify the binding layer; only work within:

- C++ core implementation and headers
- Python wrapper code that calls already-exposed entrypoints

______________________________________________________________________

## 6) Testing Contract (Hard Gates)

### End-of-session “green” requirement

You may not finish a session in a failing state.

Before concluding:

1. Run relevant test suites
1. Ensure **all tests pass** (C++ + Python)
1. Any new/changed behavior has tests on **both sides** when applicable

**Do not** hide failures with skip/xfail unless explicitly documented and approved in-repo.

### Cross-layer contract tests (parity)

For every behavior exposed via Python (`loads/dumps/ndjson/search/jsonpath/...`):

- C++ tests: core correctness + edge cases
- Python tests: wrapper parity (types, exceptions, lifetimes)

Parity assertions should cover:

- same outputs
- same error categories (exception types) and messages where feasible
- same edge-case handling

Minimum edge cases to consider (when applicable):

- `.json` and `.ndjson`
- Unicode handling / escaping
- numeric boundaries; NaN/Infinity policy (if supported)
- ordering promises (if any) and whitespace tolerance
- malformed/truncated input, invalid UTF-8, overflows
- search semantics: missing keys, nested paths, arrays, nulls, duplicates

______________________________________________________________________

## 7) Error Handling Consistency (C++ ↔ Python)

- C++ hot paths:
  - prefer project `Result<T>` / error-code style over exceptions
  - keep exceptions out of tight loops
- Python wrapper:
  - translate native error returns into Python exceptions **without changing native bindings**
  - preserve context (offsets, path, reason) when already available from native API
- Python:
  - raise consistent exception types (e.g., `ValueError` or project `StrataError` if defined)
  - messages must be actionable and stable for tests

If native APIs do not currently return enough error context, document:

- what context is missing
- the desired C++ API shape
  …but do **not** implement binding changes.

______________________________________________________________________

## 8) Memory + Allocations Are First-Class

For hot-path changes, explicitly reason about:

- allocation count + sizes
- ownership/lifetimes (RAII)
- avoiding O(n²)
- minimal includes / low coupling
- view/zero-copy safety constraints

Structural rule:

- Avoid “god files”; if a file exceeds ~800 LOC, split by responsibility unless a documented reason exists.

______________________________________________________________________

## 9) Benchmarks Are Mandatory on Hot Paths

### When benchmarks are required

Any change affecting:

- parsing/serialization throughput
- NDJSON streaming
- search/query performance
- memory arenas/allocations
- zero-copy/view lifetimes
- compiler/build flags impacting performance

### Benchmark hygiene

- identical datasets across libs
- warmups + multiple iterations
- report min/median/p95
- capture environment (CPU/OS/Python/compiler flags)
- measure memory (RSS/peak) when possible

### Logging

Maintain an **append-only** benchmark progress log under:

- `docs/benchmarks/progress_log.md`

Each entry must include:

- date/time
- commit hash
- environment (CPU/OS/compiler flags/Python)
- commands used
- key metrics (min/median/p95 + memory where available)
- conclusion: improved / neutral / regressed

### Continuous benchmark loop (baseline → change → post)

For any perf-sensitive change:

1. Capture baseline benchmarks **before** the change
1. Implement the change
1. Run post-change benchmarks
1. Compare results and decide go/no-go

Treat regressions as bugs: fix or revert (unless an explicit in-repo exception exists).

Before ending a session after touching perf-sensitive areas, provide:

- baseline vs post-change deltas (bullets or a small table)
- the path to the updated progress log entry
- confirmation that no regressions remain

______________________________________________________________________

## 10) Production/Release Gate (Benchmark Leadership)

Before tagging a release / promoting to production:

- Strata must rank **#1** on the canonical benchmark suite for the targeted workloads:
  - `loads`, `dumps`, NDJSON, search/query, memory efficiency (where measured)

If not #1:

- release is blocked unless there is an approved, documented exception in-repo.

______________________________________________________________________

## 11) Build Gates + Default “Build Runs Tests”

### Build must fail on test failures

- Build pipeline must run **C++ tests** and **Python tests**.
- Any failure ⇒ non-zero exit ⇒ build failed.
- No merge/tag/release with failing tests.

### Default build runs C++ tests

Standard build entrypoints should run C++ tests automatically:

- `cmake --build ...`
- `make build` / `ninja`
- CI “build” jobs

Optional escape hatch like `SKIP_TESTS=1` is allowed only if:

- off by default
- strongly discouraged
- cannot be used in CI/release
- prominently logged

Recommended: a single gate command (e.g., `make gate`) that runs:

1. configure/build
1. C++ tests (`ctest`)
1. Python tests (`pytest`)
1. coverage checks (if configured)

______________________________________________________________________

## 12) Automation Interface: `scripts/` + Makefile

- All automation logic goes in `scripts/`.
- The root `Makefile` is the **only** user-facing interface:
  - Make targets call `scripts/*`
  - Keep Make targets thin (no complex embedded logic)
- Scripts must be deterministic, fail-fast, and well-logged.
- Use `scripts/common.sh` for shared helpers.

Docs should reference **Make targets** as canonical commands (`make test`, `make bench`, etc.).

______________________________________________________________________

## 13) Research-Driven Experimentation (When It Matters)

For performance-critical work, explore **multiple candidate approaches** (not only straightforward implementations), but:

- never sacrifice correctness for speed
- avoid “clever” changes without measurable wins
- if too complex, keep it as an experiment and document rejection rationale in `docs/`

For each experiment, produce:

1. hypothesis
1. minimal prototype
1. microbenchmarks + datasets
1. go/no-go conclusion with evidence (speed, RSS, complexity)

______________________________________________________________________

## 14) Required Response Format (Every Junie Work Update)

Every response describing work performed must include:

1. **Files changed** (paths)
1. **Key code blocks** (C++ → Python wrapper)
1. **Tests added/updated** (C++ + Python)
1. **Benchmarks added/updated** (if perf path touched)
1. **Perf/memory rationale** (allocations, complexity, lifetimes)
1. **If blocked by “no bindings”:** list the required binding changes (high-level) without implementing them

______________________________________________________________________

## 15) Operational Checklist (Use This Every Time)

- [ ] Identify whether the change touches a hot path (if yes → benchmark loop)
- [ ] Implement CPU-heavy logic in C++ (Python stays thin)
- [ ] Add/adjust tests in **C++ and Python**
- [ ] Run C++ tests + Python tests; end session only when green
- [ ] If perf-sensitive: run baseline + post benchmarks; update `docs/benchmarks/progress_log.md`
- [ ] Keep docs under `docs/` only; update docs when behavior/API changes
- [ ] Keep automation in `scripts/`; expose via Makefile
- [ ] Keep public API minimal; no dependency fallbacks in core
