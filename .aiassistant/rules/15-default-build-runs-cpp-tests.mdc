---
alwaysApply: true
---
# Rule: Default Build Runs C++ Tests (Fail Build on Any Failure)

Running C++ tests is **default behavior** of building the package. A build that does not run (and pass) C++ tests is invalid.

## Default behavior requirement
- Any standard build entrypoint MUST execute the C++ test suite automatically, including:
  - `pip install .` / `pip wheel .` / `python -m build`
  - `cmake --build ...`
  - `make build` / `ninja`
  - CI “build” jobs
- Tests are not an optional “extra step”. They are part of “build”.

## Fail-fast gate
- If **any** C++ test fails, the build MUST:
  - exit non-zero
  - produce a clear test log output
  - stop immediately (no packaging / wheel publish)

## Recommended implementation pattern
Provide a single canonical build wrapper that always runs tests, e.g.:
- `./scripts/build.sh` or `make build`
which performs:
1) configure
2) build
3) **ctest** (required; fail-fast)
4) package wheel/sdist (only if tests pass)

If the repo uses CMake, ensure the default build target triggers `ctest` via the wrapper or an equivalent mandatory step.

## Notes
- Developers may have a separate *explicit* escape hatch (e.g. `SKIP_TESTS=1`) ONLY if:
  - it is off by default,
  - strongly discouraged,
  - cannot be used in CI/release,
  - and its use is logged prominently.
