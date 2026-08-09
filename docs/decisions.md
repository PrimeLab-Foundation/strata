# Decision Log

One line per decision. Append-only; never edit past entries — supersede with
a new line. Format: `date · area · decision (and the rejected alternative
when it clarifies)`. Anything ambiguous during implementation gets decided
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
