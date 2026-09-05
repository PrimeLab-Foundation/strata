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

## Current state (after M9 — hardening & tooling)

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
  unless building universal2; MSVC `/std:c++20 /O2 (+/arch:AVX2)`. On top of
  those, `STRATA_ENABLE_LTO=1` adds `-flto=thin` (gcc: `-flto`) and
  `PGO_MODE=generate|use` adds **IR-level** `-fprofile-generate` /
  `-fprofile-use=$STRATA_PGO_PROFILE` to both compile and link. `use` with a
  missing or unset profile is a hard error, not a silent plain build. Under
  MSVC the same knobs speak LTCG: LTO is `/GL`+`/LTCG`, and PGO_MODE links
  `/GENPROFILE:PGD=$STRATA_PGO_PROFILE` → `/USEPROFILE:PGD=...` (the profile
  is the `.pgd` both phases share; PGO implies LTCG, and the /USEPROFILE
  link merges the training `.pgc` files itself — no pgomgr step).
  `scripts/pgo_build_msvc.py` orchestrates the two phases on Windows —
  training, gates and verification mirroring `pgo_build.sh` — and stages
  pgort140.dll beside python.exe first, because the instrumented extension
  depends on it and Python 3.8+ does not consult PATH for extension-module
  dependencies. **The Windows build the benchmark leg measures is clang-cl,
  not MSVC:** `STRATA_WIN_COMPILER=clang-cl` makes setup.py compile with
  clang-cl (same command line, the MSVC linker, LTCG stripped) and spell
  PGO with clang's own flags behind `/clang:` — `-fprofile-generate`, with
  clang's runtime directory (`clang -print-runtime-dir`) on the link line
  so link.exe finds the profile runtime every instrumented object names
  itself, then `-fprofile-use` against the llvm-profdata merge — and
  `scripts/pgo_build_clang_cl.py` drives the two phases exactly as
  `pgo_build.sh` does (no DLL staging: clang's profile runtime is a static
  library, unlike pgort140.dll). Measured on one commit with three
  toolchains (docs/decisions.md, 2026-09-03), MSVC compiles the
  serializer's record and float paths 20–30% slower than clang-cl, which
  reads them at parity with the LLVM-built rivals; MSVC stays a tested
  compiler in the CI matrix so both remain green. No LTO with clang-cl
  yet: its bitcode objects need lld-link, which setuptools does not drive.
- `pyproject.toml` — version is `dynamic`, read via
  `[tool.setuptools.dynamic] version = {attr = "strata.__version__"}`. Only the
  `dev` extra exists (cmake, pytest, pytest-cov, ruff, pre-commit, clang-format
  pinned to the pre-commit hook's major); the `bench` extra returns at M5.
  CMake is a dev dependency because `make test` drives ctest — without it the
  documented `make dev && make test` path would need a system CMake that no
  make target can install.
- `src/strata/core_sources.txt` — the one list of core translation units, read
  by CMakeLists.txt (suites and fuzz targets) and by setup.py (the extension),
  and pinned by `tests/unit/test_build_manifest.py`: a core `.cpp` that is not
  listed, or a listed file that is missing, fails the Python suites. Keep it
  ASCII — CMake's `file(STRINGS)` treats non-ASCII bytes as separators.
- `tests/fuzz/` — `fuzz_loads.cpp` and `fuzz_ndjson.cpp` plus their own
  CMakeLists, built only with `-DFUZZ=ON`. It probes for the libFuzzer runtime
  at configure time and fails with an actionable message when the toolchain
  lacks one (Apple's clang always does), instead of the blueprint's raw
  missing-`libclang_rt.fuzzer_osx.a` link error. The seed corpus is committed
  under `tests/fuzz/corpus/` — see "Fuzzing" below.
- `CMakeLists.txt` — single registry. One suite is one
  `strata_add_cpp_test(target, source...)` line; default build type is Release,
  tests run with the source tree as their working directory, and test targets
  get `-UNDEBUG` so `assert` cannot be
  compiled out into a vacuously passing suite. After the registrations a glob
  over `tests/cpp/test_*.cpp` fails configure if any suite was never
  registered — the registry is only "single" if nothing can sit outside it.
  `-DCOVERAGE=ON` instruments that same registry for llvm-cov, so coverage can
  never measure a different source list than the tests do.
- `.pre-commit-config.yaml` — ruff + ruff-format, whitespace/EOL/YAML/TOML
  hooks, clang-format **covering include/ and src/**, mdformat (`--number`) and
  markdownlint. No mypy hook. Markdown is excluded from `trailing-whitespace`:
  mdformat re-emits YAML frontmatter with a trailing space that the whitespace
  hook would strip and mdformat would restore, so the pair never converges.
- `.github/workflows/` — `ci.yml` (push + PR): a `test` matrix of ubuntu
  3.10–3.14, macos-15-intel (x86_64), macos-latest (arm64) and windows-latest, each
  running ctest → the test-gated install → the Python suites; a `coverage` job
  (both layers, report uploaded); a `style` job; a `corpus` job that
  replays the fuzz corpus under ASan+UBSan on every push and then builds the
  extension itself under the sanitizers and runs both Python suites against
  it (`make test-py-asan`, 45-minute budget); and a `bindings-asan-macos`
  job running that same gate on macos-latest, the platform it was validated
  on. `fuzz.yml` (weekly
  Tue 04:00 UTC), `benchmark.yml` (weekly Mon 02:00 UTC: the cross-platform
  supportability pipeline — the same suite on linux-x86_64, macos-x86_64,
  macos-arm64 and windows-x86_64/MSVC, so both CPU architectures run the
  tripwire — gated by `benchmarks/supportability_check.py` on ERROR rows,
  category coverage and a loose strata-vs-best-rival ratio bound; absolute
  times are never compared across machines. Every leg benchmarks the PGO
  build — the build the release wheel ships — against the competitors'
  released wheels: the POSIX legs run `make pgo` first, the Windows leg
  runs `scripts/pgo_build_clang_cl.py` (clang's PGO under clang-cl; the
  MSVC twin `scripts/pgo_build_msvc.py` remains for the LTCG build), and
  each report's compiler_flags line records which build its leg measured. One
  report artifact per leg, named `benchmark-<os>-<arch>`; `make bench-ci`
  fetches the latest run into `docs/benchmarks/ci/` and rebuilds the
  per-platform standings summary — see `docs/benchmarking/SKILL.md`),
  `pgo.yml` (weekly Mon 03:00 UTC, uploads a PGO+LTO wheel).
  Visibility-guarded `test-linux-arm` (ci.yml) and `bench-linux-arm`
  (benchmark.yml) jobs cover NEON-on-Linux; they skip cleanly while the repo
  is private (GitHub's arm64 Linux hosted runners serve public repos only)
  and activate on going public.

Toolchain floor for number conversion: floating-point `std::from_chars`
(MSVC 2017+, libstdc++ 11+, libc++ 20+). Apple SDKs through Xcode 16 ship the
integral overloads only, so `util::from_chars_double` (fast_parse.hpp) gates
on `__cpp_lib_to_chars` and falls back to an observably identical `strtod_l`
twin there — copied NUL-terminated token, explicit "C" locale, ERANGE folded
to from_chars semantics. Compile with `-DSTRATA_FORCE_STRTOD_FALLBACK` to run
the suites over the twin on a machine whose library has the real thing; any
other platform without FP from_chars fails the build with `#error` rather
than guessing.

Not built yet, by milestone: release/tag tooling (M10). `make gate` runs the
C++ tests → force reinstall → Python suites → both coverage reports. No step
is suffixed with `|| true`.

## Makefile (the single interface — targets forward to `scripts/`)

| Group    | Targets                                                                                                                                                                                                                                                                                                                                                                                                  |
| -------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Setup    | `venv`, `dev` (venv + pre-commit), `install` (gated editable install), `install-skip-tests`, `install-dev/-bench/-all`                                                                                                                                                                                                                                                                                   |
| Build    | `build` (sdist+wheel), `cpp-build` (CMake)                                                                                                                                                                                                                                                                                                                                                               |
| Test     | `test` = `test-py` + `test-cpp`. **Target change:** rebuilt `test-py` runs `tests/py` *and* `tests/unit` (the previous one ran `tests/py` only, so `make test` skipped the contract mirrors); rebuilt `test-cpp` drives ctest. `gate` (5-step compliance gate); `test-py-asan` builds the extension under ASan+UBSan in `.venv-asan` and runs both suites against it — deliberately *not* part of `test` |
| Coverage | `coverage`, `coverage-cpp` (llvm-cov over the CMake registry — the previous impl's had its own drifted source list, see below), `coverage-py` (pytest-cov)                                                                                                                                                                                                                                               |
| Bench    | `bench-data`, `bench-small/-medium/-large`, `bench-all`, `bench-baseline`, `bench-ci` (fetch CI reports + summary), `bench-ci-summary`                                                                                                                                                                                                                                                                   |
| Fuzz/PGO | `fuzz-build`, `fuzz-run`, `fuzz` (→ `scripts/fuzz.sh`), `pgo` (→ `scripts/pgo_build.sh`)                                                                                                                                                                                                                                                                                                                 |
| Lint     | `fmt` (ruff format + clang-format), `lint` (ruff check), `pre-commit-check`                                                                                                                                                                                                                                                                                                                              |
| Misc     | `clean`, `clean-venv`, `tag-create/-delete/-update`, `help`                                                                                                                                                                                                                                                                                                                                              |

`PYTHON ?= python3`. There is no `VERSION` variable: the version has exactly
one home, `python/strata/__init__.py`, which pyproject reads dynamically. The
previous implementation kept it in three places and they drifted.

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
`-DFUZZ=ON` (`scripts/fuzz.sh`; on macOS it selects Homebrew LLVM
automatically). Env: `FUZZ_TIME` (120 s default), artifacts → `fuzz_crashes/`.

The seed corpus **is committed**: `tests/fuzz/corpus/loads` (37 seeds) and
`tests/fuzz/corpus/ndjson` (13). Seeds are named for their intent — a
`bad_`-prefixed seed must be rejected, every other seed must parse — and
`fuzz.sh` checks both directories are non-empty before spending its budget.

`tests/cpp/test_fuzz_corpus.cpp` replays the whole corpus on every
`make test`, plus every truncation and single-byte mutation of it. That keeps
the seeds honest (a seed that changes meaning fails the build) and gives the
corpus value on toolchains with no libFuzzer runtime — **Apple's clang ships
none**, so `make fuzz` on stock macOS stops at the configure-time probe with
an actionable message. `brew install llvm` is the fix; CI fuzzes on Linux.

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

### Sanitized bindings (`make test-py-asan`)

`scripts/asan_py_tests.sh` closes the gap the `corpus` job leaves. That job compiles `core_sources.txt` only, so `src/strata/bindings` — where the raw stores into CPython-owned allocations live (the fused ASCII copy in `python_builder.h`, the serializer's exact-fit output block) — had never been built instrumented anywhere, and a one-byte over-store there is invisible to value-equality tests.

The gate installs a copy of the tracked tree, non-editable, into its own virtualenv `.venv-asan` (already gitignored by `.venv*/`) with `CXX=clang++` and `CXXFLAGS='-fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer -g'`, then runs `tests/py` + `tests/unit` against the installed extension. `SKIP_TESTS` is never set, so setup.py's own gates — the C++ suite before the compile, both Python suites after — run inside the sanitized build and the suites are exercised twice. Failure is fatal by construction: `-fno-sanitize-recover=all` plus `abort_on_error=1` turn any report into a `SIGABRT`, and the non-zero status propagates out through pytest and pip.

Five mechanics carry it, and each one fails *silently* if it is left out:

1. **`PYTHONMALLOC=malloc`.** CPython carves small objects out of pymalloc pools, so a write one past a `PyUnicode` object lands in padding ASan knows nothing about and produces no report. Routing object allocation through the instrumented system malloc is what gives the redzones teeth — with it, the planted over-store below reports its allocation as `malloc → PyUnicode_New`. Without it, this gate is decorative.
2. **Preload the runtime, do not merely link it.** An instrumented `.so` first pulled in by `dlopen` initializes ASan too late for its interceptors, which ASan reports as a fatal error. `DYLD_INSERT_LIBRARIES` on macOS, `LD_PRELOAD` on Linux; the path comes from `clang++ -print-file-name=libclang_rt.asan_osx_dynamic.dylib` (macOS) or `libclang_rt.asan-$(uname -m).so` (Linux), and is believed only when it names a file that exists. The script then *proves* the arming — `ctypes.CDLL(None).__asan_init` must resolve in the interpreter — instead of assuming it. macOS drops `DYLD_*` for SIP-protected binaries, so the virtualenv must be built from a Homebrew or python.org interpreter; a `/usr/bin/python3` base is refused outright.
3. **`ASAN_OPTIONS=strip_env=0` on macOS.** ASan removes itself from `DYLD_INSERT_LIBRARIES` by default; measured here, the interpreter that comes out the other side has no runtime in it at all (`dlsym` cannot find `__asan_init`) and its children inherit nothing. The build runs several execs deep — pip, the build backend, setup.py's C++ gate, its pytest gate — so the variable has to survive every one of them.
4. **A copy of the tracked tree, never the working directory.** setup.py's post-build gate copies `python/strata` over its staging directory before running pytest, and an editable install leaves its in-place `_strata*.so` there: build in the repository and the gate tests the *ordinary* extension and reports green having sanitized nothing. `git ls-files` into `build/asan-ext/tree` has neither problem and costs a fifth of a second. It also keeps sanitized object files out of the repository's `build/`, which the next ordinary build would adopt — setuptools compares timestamps and knows nothing about flags. For the same reason `force = 1` (via `DIST_EXTRA_CONFIG`, with `parallel`) is what makes a header edit recompile at all: headers are not in `ext.depends`.
5. **`log_path`.** pytest captures file descriptor 2, so a report written to stderr dies with the aborting process — the failure survives, the diagnosis does not. Reports go to `build/asan-ext/logs/asan.<pid>` and an `EXIT` trap prints them.

The C++ suite is built **unsanitized** here: the staged CMake cache is seeded with an empty `CXXFLAGS` first, because CMake reads that variable only at first configure. Sanitizing the core is the `corpus` job's own step immediately before this one, and repeating it here only trips over the signed-integer overflow in `tests/cpp/test_scan.cpp`'s `power *= 10` loop (`1000000000000000000 * 10` on the last iteration), which `-fno-sanitize-recover` turns fatal — a real finding, but not this gate's.

**Measured 2026-09-05** (macOS arm64, Apple clang 21, one-minute load ~9 — a busy machine, and this target is not a benchmark): `make test-py-asan` 55 s wall on a cold staged tree (the C++ suite compiled from scratch) and 35 s on a re-run, 1765 tests green, of which the sanitized pytest run itself is 2.5 s against 1.1 s unsanitized. Evidence that the sanitizer is really live: the installed extension is 1,106,624 bytes against 332,320 for the ordinary build, carries 34 undefined `___asan_*` symbols (`nm -u`), and `otool -L` names `@rpath/libclang_rt.asan_osx_dynamic.dylib`.

**Teeth.** A one-byte over-store planted in `copy_if_ascii` — `dst[len + 1] = byte` on the short-string tail, one past the `PyUnicode` allocation — fails the gate in 28 s, inside setup.py's own post-build pytest gate, with `AddressSanitizer: heap-buffer-overflow ... WRITE of size 1 ... #0 ... copy_if_ascii ... python_builder.h` and the allocation trace `malloc → PyUnicode_New`. Under pymalloc the same store is invisible *unless* it happens to cross a size class: the request is `len + 41` bytes, so `len ≡ 7 (mod 16)` puts the byte in the next block — an unrestricted mutant segfaults `make test-py` on a length-7 string, while the same mutant restricted to `len < 7` passes `make test-py` 1765/1765 and is still caught here. That is the whole argument for the gate in one measurement: whether the bug is loud or silent is decided by an allocator size class, not by the bug.

**The CI step is written but has not run.** `ci.yml`'s `corpus` job gained a `Build and run the Python suites under ASan + UBSan` step after the ctest one, with the job timeout raised 20 → 45 minutes. GitHub Actions is refusing every job on this org while the spending limit is exhausted, so the Linux half — `LD_PRELOAD`, the runtime-name probe, and the `LDCXXSHARED` swap that stops CPython's recorded `g++ -shared` from linking clang-instrumented objects against the wrong runtime — is written from the documentation and unexercised.

## Known-broken / stale inventory of the previous implementation (do not reproduce)

1. `make coverage-cpp` — source list named deleted files
   (`search/jsonpath.cpp`, `util/simd_string.cpp`), omitted `dragonbox.cpp`;
   failures swallowed by `|| true`. *Fixed in the rebuild:* `-DCOVERAGE=ON`
   instruments the one CMake registry, so there is no second list to drift,
   and no step is suffixed with `|| true`.
2. `make typecheck` + pre-commit mypy hook — target `src/strata` has no Python;
   `mypy.ini` doesn't exist. Inert.
3. `benchmark.yml` dataset paths (above) and the regression gate. *Fixed in
   the rebuild:* the workflow calls `make bench-data` + `make bench-small`, so
   the paths are the Makefile's, and gates via `benchmarks/regression_check.py`
   at the documented 2%/5% thresholds instead of the old `--threshold 5`.
4. `tests/fuzz/corpus/` missing (above). *Fixed in the rebuild:* committed,
   and replayed by a ctest suite so it cannot rot unnoticed.
5. Makefile `test` target defined twice (later wins, with a make warning).
6. `make dev` installs pybind11 (unused — raw C API project).
7. pyproject Homepage/Repository are `github.com/example/strata` placeholders.
8. Stale doc references in scripts/workflow comments may point at pre-restructure
   paths; canonical docs are `docs/`.
