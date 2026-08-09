# Decision Log

One line per decision. Append-only; never edit past entries — supersede with
a new line. Format: `date · area · decision (and the rejected alternative when it clarifies)`. Anything ambiguous during implementation gets decided
*here*, visibly — never silently in code.

- 2026-08-09 · api · `compile_path` renamed `compile` (mirrors `re.compile`).
- 2026-08-09 · api · `parse_json`/`parse_ndjson` dropped; cursors via `return_type="cursor"`, NDJSON via `load`.
- 2026-08-09 · api · Folder mode added to `load`/`dump`/`search`; `dump` splits by key(s) via `split_by`.
- 2026-08-09 · api · Versioning: calver `YYYY.M.D`, start `2026.8.9`; single source `__init__.py` (rejected: DDMMYYYY — doesn't order under PEP 440).
- 2026-08-09 · parse · Integers parse exactly at any size, stdlib-json-compatible (rejected: raise beyond int64 like orjson).
- 2026-08-09 · ndjson · Invalid lines raise `ValueError` unless `skip_errors=True`; uniform across eager/iterator/folder (rejected: previous silent skip / partial list).
- 2026-08-09 · folder · `split_by` groups by JSON string form (`true`/`false`, decimal ints); same-call collisions and case-only collisions error.
- 2026-08-09 · folder · Discovery: recursive, case-insensitive extensions, hidden entries pruned, symlinks not followed, bytewise ordering.
- 2026-08-09 · config · `cycle_policy` default `"warn"`, active from process start; reported and actual behavior always agree (previous impl started on `ignore` while reporting `warn`).
- 2026-08-09 · jsonpath · All invalid expressions raise `ValueError` (previous impl leaked `RuntimeError` on unclosed quotes).
- 2026-08-09 · jsonpath · Target law: `search(f, e) == query(load(f), e)` — resolves the SAX-vs-DOM recursive-descent divergence in favor of query semantics.
- 2026-08-09 · build · One C++ test harness: CMake/ctest is the single registry (rejected: previous three parallel harnesses with hand-maintained lists).
- 2026-08-09 · build · `make test` runs C++ suite + pytest over `tests/py` AND `tests/unit`.
- 2026-08-09 · bench · Regression-gate comparisons use median of ≥ 10 repeats; thresholds 2% median/p95, 5% RSS (canonical: docs/context/benchmarks.md).
- 2026-08-09 · process · Human-only commits; agent supplies message; no AI mentions in git history.
- 2026-08-09 · build · M0 ships a module-init-only `strata._strata`: the gates live in `build_ext`, which setuptools skips entirely when `ext_modules` is empty, so without an extension `make install` would run neither gate (rejected: hooking the gate to another command).
- 2026-08-09 · build · CMake configures into `build/cmake`, not the bare `build/` that setuptools owns — a stale `CMakeCache.txt` beside `build/lib.*` can poison a wheel build.
- 2026-08-09 · build · C++ test targets compile with `-UNDEBUG` and a Release default; otherwise a Release configure would strip every `assert` and the assert-based suites would pass vacuously.
- 2026-08-09 · build · `PYTHON ?= python3` (rejected: the blueprint's `python3.14` — the floor is 3.10 and CI spans 3.10–3.14).
- 2026-08-09 · build · `SKIP_TESTS=1` is refused outright when `CI` is set (rejected: the blueprint's warning-only banner — convention.md bans it in CI, and a warning is not a ban).
- 2026-08-09 · build · `make gate` runs three steps at M0 (C++ tests → force reinstall → Python suites); its coverage phase lands with the coverage tooling at M9, absent rather than `|| true`-swallowed.
- 2026-08-09 · build · M0 omits the bench/fuzz/pgo/coverage/tag targets, the `bench` extra, the LTO+PGO setup.py knobs and `[tool.cibuildwheel]`; each returns in the milestone that builds its tooling (M5/M9/M10).
- 2026-08-09 · build · The C++ core source list stays duplicated between `CMakeLists.txt` and `setup.py` while both are empty; M1 must introduce one shared definition before the first core `.cpp` lands (the blueprint's two lists had already drifted).
- 2026-08-09 · build · The post-build gate launches pytest through a `-c` bootstrap that injects `sys.path`; pip's build-isolation `sitecustomize` rewrites `sys.path` at startup, so `PYTHONPATH` alone silently loses the staging directory.
- 2026-08-09 · package · Project URLs point at the real remote `https://github.com/PrimeLab-Foundation/strata` (no doc names one; the blueprint's `github.com/example/strata` placeholders are forbidden).
- 2026-08-09 · package · pyproject uses the PEP 639 SPDX form `license = "MIT"` + `license-files` (setuptools ≥ 77), dropping the deprecated table form and the license classifier.
- 2026-08-09 · tests · The version contract is pinned structurally (pyproject declares `dynamic` + no build file restates the literal) rather than by comparing `importlib.metadata`, which during the post-build gate still reports the previous install and would fail every version bump.
- 2026-08-09 · lint · `make fmt` takes clang-format from the `dev` extra (PyPI wheel), pinned to the same major as the pre-commit hook so the two cannot disagree (rejected: a system clang-format, absent on a fresh machine).
- 2026-08-09 · lint · Markdown is excluded from ruff (`extend-exclude`) and from the `trailing-whitespace` hook: ruff ≥ 0.16 reformats Python blocks inside Markdown, and mdformat re-emits frontmatter with a trailing space the whitespace hook would strip forever. mdformat (`--number`) + markdownlint own Markdown; adopting them normalized the existing docs once.
- 2026-08-09 · lint · `.ruff.toml` drops the dead `legacy_tests/**` and `tools/**` per-file-ignores (only benchmarks/ and docs/ are lint-exempt); `.editorconfig` gains `[Makefile] indent_style = tab`, since GNU make requires tab-indented recipes.
- 2026-08-09 · build · The C++ gate runs ctest with `--no-tests=error` and CMake fails configure when a `tests/cpp/test_*.cpp` is unregistered: verified that plain ctest exits 0 on an empty registry, so "green" would otherwise not imply "the suite ran".
- 2026-08-09 · build · `cpp_tests.py` passes `--config`/`--build-config` and resolves cmake/ctest beside `sys.executable` before PATH, so multi-config generators do not silently fall back to Debug and `make gate`'s two drivers cannot configure one build tree with two different CMake binaries.
- 2026-08-09 · build · `cmake>=3.20` is a `dev` extra: `make test` drives ctest, so the documented `make dev && make test` path must not depend on a system CMake that no make target can install.
- 2026-08-09 · build · Open for M1: C++ test binaries compile without `-march=native` while the extension compiles with it, so SIMD paths would be tested under different flags than they ship with. Decide flag parity (mirror the extension flags, or register two configurations) before the first SIMD source lands.
- 2026-08-09 · core · `Result::unwrap()`, `Result::value_or()` and `FlatMap::count()` are not rebuilt: all three had zero callers anywhere in the previous implementation, which checked `ok()` and read `value` directly. Add them when a caller exists (rejected: porting the blueprint header verbatim, dead API included).
- 2026-08-09 · core · `Result` gives both members default initialisers and `ok()` is `constexpr`, so a Result built without a value holds a default-constructed `T` instead of an indeterminate one; the guarantee is pinned by a `consteval` check, because the equivalent runtime assertion would be reading uninitialised memory that usually happens to be zero.
- 2026-08-09 · core · `FlatMap::emplace()` keeps the blueprint's append-without-duplicate-check behaviour, now stated as an explicit caller contract and pinned by test: `find()` returns the first match, and the parser's duplicate-key policy is built on exactly that.
- 2026-08-09 · core · Core purity is enforced by `tests/unit/test_core_purity.py` (scans `include/` + `src/strata/` minus `bindings/`, with a positive control so it cannot pass vacuously); core files avoid naming the CPython header even in prose so the roadmap's documented grep stays literally empty.
- 2026-08-09 · build · Supersedes the M1 obligation logged above: M1 added no core `.cpp` (the value model is header-only), so the single shared C++ source-list definition is due at M2, before the first core translation unit.
- 2026-08-09 · parse · SAX gains `on_big_int(std::string_view)` carrying the raw token, so integers beyond int64 reach handlers exactly; `on_uint` and the `size_hint` parameters are not rebuilt (never emitted / always 0). The previous implementation accumulated only the first 19 digits and returned that truncation as the value.
- 2026-08-09 · parse · Number conversion uses `std::from_chars` — correctly rounded and locale-independent — instead of the blueprint's Clinger fast path plus `strtod`, whose result depends on the process locale's decimal separator. The fast path may return at M5 with a benchmark behind it.
- 2026-08-09 · parse · Values outside the double range saturate to ±inf / 0 like stdlib `json`, rather than failing the parse (`1e400`, `1e-400`).
- 2026-08-09 · parse · A leading UTF-8 BOM is rejected: it is not part of a JSON text (rejected: stdlib's behaviour of stripping it from bytes input). Found by oracle-diffing against `json.loads`; pinned by test alongside the other deliberate divergences (NaN/Infinity, escaped lone surrogates, CESU-8 surrogates).
- 2026-08-09 · parse · Scan primitives live in `include/strata/util/scan.hpp` as `validate_utf8`/`skip_whitespace`/`find_next_escape`, named for what they do rather than how; vectorised bodies replace them at M5 behind the same names (rejected: the blueprint's `simd_*` names, which already described scalar code).
- 2026-08-09 · parse · M2 ships the parser scalar and without the speculative-key hook or the inline array fast path; both are performance work for M5, where a benchmark can justify them.
- 2026-08-09 · parse · The duplicate-key policy (enum, thread-local setting, warning list) lands with the DOM builder, which cannot avoid choosing a behaviour; wiring it to `strata.config` stays in M4.
- 2026-08-09 · build · Core translation units are enumerated once in `src/strata/core_sources.txt`, read by CMakeLists.txt and setup.py, and pinned by a contract test. The file is ASCII-only because CMake's `file(STRINGS)` treats non-ASCII bytes as separators and silently splits comment lines into bogus entries. This discharges the shared-source-list obligation logged for M2.
- 2026-08-09 · build · Fuzz targets are opt-in (`-DFUZZ=ON`) and fail *configure* with an actionable message when the toolchain has no libFuzzer runtime — Apple's clang never ships one, so the blueprint's raw "libclang_rt.fuzzer_osx.a not found" link error was the first sign. Seed corpus and scheduled runs stay at M9.
- 2026-08-09 · ci · The M0 CI skeleton is three test legs (ubuntu 3.10/3.14, macOS 3.12) plus a style job, on `*-latest` runner labels (rejected: the blueprint's eight-leg matrix on pinned 2022–2024 images); the full matrix is M9.
