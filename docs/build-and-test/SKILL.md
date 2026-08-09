---
name: build-and-test
description: Build pipeline, Makefile interface, test architecture (C++/py/unit 
  parity), fuzzing, coverage, PGO, CI workflows — and the authoritative list of 
  currently-broken tooling. Load before changing Makefile, setup.py, 
  CMakeLists.txt, scripts/, tests/, or .github/workflows/.
---

# Build & Test

**Framing:** this doc describes the *previous implementation's* build/test
system (`backup/pre-reset-main`) as the blueprint. Items marked broken are
**do-not-reproduce** notes, not things to fix in place. Target deviations from
the blueprint are called out inline. Bootstrap order: workflow.md "Milestone
zero". What the rebuild has actually built so far is in "Current state"
immediately below; everything after it is blueprint until a milestone makes it
real.

## Current state (after M0 — scaffolding)

Real on this branch:

- `Makefile` — `PYTHON ?= python3`, no `VERSION` variable (the version has one
  home, `python/strata/__init__.py`). Targets: `venv`, `dev`, `install`,
  `install-dev`, `install-skip-tests`, `build`, `cpp-build`, `test`,
  `test-cpp`, `test-py`, `gate`, `fmt`, `lint`, `pre-commit-check`, `clean`,
  `clean-venv`, `scripts-executable`, `help`. Every target carries a `##` help
  string, `.PHONY` is declared once, and each forwards to `scripts/`.
- `scripts/cpp_tests.py` — the only driver of the C++ suite (configure → build
  → ctest into `build/cmake`, kept out of the bare `build/` that setuptools
  owns). Used by `make test-cpp`, `make cpp-build` (`--build-only`, which
  configures and builds but does **not** run ctest), the install gate and CI.
  It passes `--no-tests=error` — ctest otherwise exits 0 when the registry
  produced no tests, so a broken CMakeLists would report a green C++ gate
  having run nothing — and `--config`/`--build-config`, which single-config
  generators ignore and multi-config ones (Visual Studio, Xcode, Ninja
  Multi-Config) need in order not to silently fall back to Debug. Tools are
  resolved beside `sys.executable` before PATH, so the pinned CMake from the
  dev extras or from pip's isolated build env wins over the system one.
- `scripts/py_tests.py` — the only driver of the Python suites; always runs
  `tests/py` **and** `tests/unit`. `--path` prepends import directories, and it
  launches pytest through a `-c` bootstrap: under pip's build isolation a
  `sitecustomize` rewrites `sys.path` at startup, so `PYTHONPATH` alone loses
  the staging directory and the post-build gate cannot see the fresh extension.
- `scripts/fmt.sh` / `scripts/lint.sh` / `scripts/gate.sh`. `fmt.sh` formats
  tracked **and** newly added C/C++ files (`git ls-files --cached --others --exclude-standard`), guarded so an empty match never makes clang-format read
  stdin.
- `setup.py` — `TestGatedBuildExt(build_ext)` around the two gates; both shell
  out to the scripts above, so "the suite" has one definition. `SKIP_TESTS=1`
  still exists but is now *refused* when `CI` is set, rather than warned about.
  Flags: `-std=c++20 -O3 -D_LIBCPP_DISABLE_AVAILABILITY`, plus `-march=native`
  unless building universal2; MSVC `/std:c++20 /O2 (+/arch:AVX2)`. The
  LTO/PGO env knobs are not wired yet — they return with `make pgo` at M9.
- `pyproject.toml` — version is `dynamic`, read via
  `[tool.setuptools.dynamic] version = {attr = "strata.__version__"}`. Only the
  `dev` extra exists (cmake, pytest, pytest-cov, ruff, pre-commit, clang-format
  pinned to the pre-commit hook's major); the `bench` extra returns at M5.
  CMake is a dev dependency because `make test` drives ctest — without it the
  documented `make dev && make test` path would need a system CMake that no
  make target can install.
- `CMakeLists.txt` — single registry. One suite is one
  `strata_add_cpp_test(target, source...)` line; default build type is Release,
  tests run with the source tree as their working directory, and test targets
  get `-UNDEBUG` so `assert` cannot be
  compiled out into a vacuously passing suite. After the registrations a glob
  over `tests/cpp/test_*.cpp` fails configure if any suite was never
  registered — the registry is only "single" if nothing can sit outside it.
  `STRATA_CORE_SOURCES` is empty until M1.
- `.pre-commit-config.yaml` — ruff + ruff-format, whitespace/EOL/YAML/TOML
  hooks, clang-format **covering include/ and src/**, mdformat (`--number`) and
  markdownlint. No mypy hook. Markdown is excluded from `trailing-whitespace`:
  mdformat re-emits YAML frontmatter with a trailing space that the whitespace
  hook would strip and mdformat would restore, so the pair never converges.
- `.github/workflows/ci.yml` — skeleton: ubuntu 3.10 + 3.14, macOS 3.12, each
  running ctest, the test-gated install and the Python suites, plus a style job
  (`ruff check`, `ruff format --check`, `clang-format --dry-run --Werror`).

Not built yet, by milestone: coverage targets and `coverage-cpp`, fuzzing,
PGO, and the full CI matrix (M9); benchmark targets, datasets and the `bench`
extra (M5); release/tag tooling (M10). `make gate` therefore runs three steps
(C++ tests → force reinstall → Python suites); its coverage phase lands with
the coverage tooling at M9. No step is suffixed with `|| true`.

## Makefile (the single interface — targets forward to `scripts/`)

| Group    | Targets                                                                                                                                                                                                                                                        |
| -------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Setup    | `venv`, `dev` (venv + pre-commit), `install` (gated editable install), `install-skip-tests`, `install-dev/-bench/-all`                                                                                                                                         |
| Build    | `build` (sdist+wheel), `cpp-build` (CMake)                                                                                                                                                                                                                     |
| Test     | `test` = `test-py` + `test-cpp`. **Target change:** rebuilt `test-py` runs `tests/py` *and* `tests/unit` (the previous one ran `tests/py` only, so `make test` skipped the contract mirrors); rebuilt `test-cpp` drives ctest. `gate` (5-step compliance gate) |
| Coverage | `coverage`, `coverage-cpp` (llvm-cov — broken in the previous impl, see below), `coverage-py`                                                                                                                                                                  |
| Bench    | `bench-data`, `bench-small/-medium/-large`, `bench-all`                                                                                                                                                                                                        |
| Fuzz/PGO | `fuzz-build`, `fuzz-run`, `fuzz` (→ `scripts/fuzz.sh`), `pgo` (→ `scripts/pgo_build.sh`)                                                                                                                                                                       |
| Lint     | `fmt` (ruff format + clang-format), `lint` (ruff check), `pre-commit-check`                                                                                                                                                                                    |
| Misc     | `clean`, `clean-venv`, `tag-create/-delete/-update`, `help`                                                                                                                                                                                                    |

`PYTHON ?= python3.14`. Version drift note: pyproject says 0.2.0, Makefile
`VERSION ?= 0.1.0`.

## Build pipeline

`setup.py` (setuptools) builds `strata._strata` from bindings + core + util
sources. Flags: `-std=c++20 -O3 -march=native` (dropped for macOS universal2;
MSVC `/std:c++20 /O2 /arch:AVX2`). Env knobs: `STRATA_ENABLE_LTO=1`,
`PGO_MODE=generate|use`, `STRATA_PGO_PROFILE`, `SKIP_TESTS=1`.

**Test-gated builds** (`TestGatedBuildExt`): every `pip install -e .` runs the
C++ suite (CMake+ctest) *before* compiling the extension and
`pytest tests/py tests/unit` *after*. This is why CI's install step is itself
the gate. `SKIP_TESTS=1` is banned in CI/releases.

**PGO flow** (`scripts/pgo_build.sh`): instrumented build (`PGO_MODE=generate`,
LTO off) → `pgo_training_data.py` writes `build/pgo/train.{json,ndjson}`
(seed `0xDA7A`, `TARGET_MB=10`) → `pgo_training.py` exercises all hot paths →
gate tests → `llvm-profdata merge` → rebuild `PGO_MODE=use` + LTO → gate tests →
verification benchmarks to `build/pgo/bench_results_pgo.md`. Historically worth
~+15% on medium/large parses (measured on the 0.1 branch, commit `22015c8`).

## C++ tests — target: ONE harness

**Target decision:** CMakeLists.txt is the single test registry; `make test-cpp`, the install gate, coverage, and CI all drive **ctest** with
different configure flags. Adding a C++ test = one `add_executable`/`add_test`
pair.

Do not reproduce the previous implementation's three parallel harnesses —
`run_cpp_tests.sh` (hardcoded source list), CMake/ctest, and a coverage
compile with its own list — whose hand-maintained lists drifted until the
coverage build silently compiled nothing (see below).

## Test architecture

- `tests/cpp/` — 10 assert-based suites: parse (incl. UTF-8 rejection contract,
  surrogate pairs), serialize, cursor, document, jsonpath, ndjson (incl. 100k-line
  stress), float precision, output_buffer, stress_deep (depth capped at 100 — the
  C++ parser is recursive), stress_large (~5MB).
- `tests/py/` (integration + bench sanity + stress) vs `tests/unit/` (contract) —
  9 filenames are mirrored; 5 byte-identical, 4 have drifted (unit adds
  TestFloatPrecision, duplicate-key/cycle policy tests, control-char rejection).
  Policy: keep mirrors in sync when touching either.
- Markers: `@pytest.mark.stress`; 100MB tests gated by `RUN_STRESS_100MB`.
- Error-message contract and conventions: see `docs/context/api.md` and the
  styleguide. Round-trip through stdlib `json` as the oracle.
- `tests/fixtures/` (invalid_json.txt, unicode_samples.json) is **orphaned** —
  nothing references it; content is duplicated inline in tests.

## Fuzzing

`tests/fuzz/fuzz_loads.cpp` (bytes → `parse_json`) and `fuzz_ndjson.cpp`
(→ `NdjsonStream`), built with `-fsanitize=fuzzer,address,undefined` via
`-DFUZZ=ON` (`scripts/fuzz.sh`; on macOS auto-switches to Homebrew LLVM).
Env: `FUZZ_TIME` (120 s default), artifacts → `fuzz_crashes/`.
**Broken:** the corpus dirs `tests/fuzz/corpus/{loads,ndjson}` are documented and
passed by fuzz.sh but have never existed in git — libFuzzer errors on start, so
`make fuzz-run` (and the weekly CI job) fails until they are created.

## CI (.github/workflows/)

- `ci.yml` — push to main + PRs; matrix ubuntu 22.04 × py3.10–3.14, macos-13/14,
  windows-2022 (×3.12). Gate = `pip install -e .[dev]` (test-gated build) +
  explicit pytest.
- `benchmark.yml` — weekly Mon 02:00 UTC. **Broken:** references
  `benchmarks/data/generated/users.json`, but `make bench-data` now writes to
  `generated/{small,medium,large}/`. Also runs `regression_check` (itself broken —
  see `docs/benchmarking/SKILL.md`).
- `fuzz.yml` — weekly Tue 04:00 UTC (fails on the missing corpus, above).
- `pgo.yml` — weekly Mon 03:00 UTC; `make pgo` + uploads a PGO+LTO wheel.
- No release/publish workflow; `[tool.cibuildwheel]` in pyproject is configured
  but unused.

## Known-broken / stale inventory of the previous implementation (do not reproduce)

1. `make coverage-cpp` — source list named deleted files
   (`search/jsonpath.cpp`, `util/simd_string.cpp`), omitted `dragonbox.cpp`;
   failures swallowed by `|| true`.
2. `make typecheck` + pre-commit mypy hook — target `src/strata` has no Python;
   `mypy.ini` doesn't exist. Inert.
3. `benchmark.yml` dataset paths (above) and the regression gate.
4. `tests/fuzz/corpus/` missing (above).
5. Makefile `test` target defined twice (later wins, with a make warning).
6. `make dev` installs pybind11 (unused — raw C API project).
7. pyproject Homepage/Repository are `github.com/example/strata` placeholders.
8. Stale doc references in scripts/workflow comments may point at pre-restructure
   paths; canonical docs are `docs/`.
