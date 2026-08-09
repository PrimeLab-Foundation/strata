# Style Guide

## Guiding principles — the Zen of Python (PEP 20)

Structure all code — C++ as much as Python — with the Zen of Python as the
design compass:

- Explicit is better than implicit: no hidden fallbacks, no silent mode
  switches; behavior differences are visible in signatures and named policies.
- Simple is better than complex; flat is better than nested: prefer the
  straightforward algorithm until a benchmark proves the clever one, and keep
  call graphs and nesting shallow (the ~800 LOC file-split rule follows from
  this).
- Readability counts: a hot path may be intricate inside, but its interface
  and invariants must read plainly; document the trick, not the obvious.
- Special cases aren't special enough to break the rules — practicality beats
  purity: fast paths must be observably identical to the general path (the
  SIMD/scalar-twin rule is this tenet applied).
- Errors should never pass silently, unless explicitly silenced: no swallowed
  failures, no partial results posing as success; opt-in policies
  (`skip_errors`-style flags) are the only sanctioned silencing.
- There should be one — and preferably only one — obvious way to do it: one
  public entry point per capability; kill superseded code paths instead of
  keeping them "just in case" (see the dead-code lessons in
  `docs/performance/SKILL.md`).
- If the implementation is hard to explain, it's a bad idea: any optimization
  that cannot be explained in its doc comment and defended with a benchmark
  number does not merge.

## C++ (C++20)

- **Formatting:** `.clang-format` — LLVM base, IndentWidth 4, ColumnLimit 100,
  PointerAlignment Left, includes sorted and regrouped. Run `make fmt`.
  Note: the pre-commit clang-format hook currently excludes `include/` and `src/`
  ("temporarily") — format those with `make fmt` before committing.
- **Naming:** namespace `strata` with nested namespaces (`strata::util`,
  `strata::json` where used); types `PascalCase`; functions and methods
  `snake_case`; constants `kPascalCase`; filenames `snake_case`. Prefer
  `#pragma once` in headers.
- **Annotations:** `[[nodiscard]]` on functions whose ignored return is a bug
  (all `Result<T>` producers, predicates, size accessors); `noexcept` where
  provably non-throwing; Doxygen `@file` blocks and class/method docs on public
  headers (see `18a3521` for the established documentation style).
- **Error handling:** `Status` / `Result<T>` on hot paths; the throwing cursor
  convenience API (`field/at/get_int/...`) exists for the binding layer only.
  No exceptions in tight loops.
- **Performance idioms in use:** templated handlers over virtual dispatch
  (`ParserInline<Handler>`); `LIKELY`/`UNLIKELY` macros on binding hot paths;
  thread-local reusable buffers; `unsafe_*` OutputBuffer writes only after a
  covering `reserve()`; named constants instead of magic numbers
  (`kSerializeInitialCapacity`, `kMaxBatchKeys`, ...).
- SIMD is compile-time dispatched (`__AVX2__` / `__SSE4_2__` / `__ARM_NEON`,
  scalar fallback) — every SIMD function needs a scalar twin with identical
  observable behavior.

## Python (≥ 3.10)

- **Lint/format:** ruff (`.ruff.toml` — line length 100, target py310, rules
  `E,F,I,UP,COM,N,PL,B`; quote style preserved). `benchmarks/`, `docs/`, `tools/`
  are lint-exempt.
- **Naming:** modules and functions `snake_case`, public classes `PascalCase`,
  internal helpers prefixed `_` and not exported. Exports go through
  `python/strata/__init__.py` `__all__` — keep it minimal.
- The facade contains no logic: wrappers normalize arguments (e.g. `Path` → `str`)
  and delegate to `strata._strata`. Docstrings on public functions state the
  signature, behaviors, and raised exceptions.

## Tests

- C++ tests are plain `assert` + `main()`, no framework; one file per subsystem
  in `tests/cpp/`. Register every new C++ test in **both** `CMakeLists.txt` and
  the hardcoded list in `scripts/run_cpp_tests.sh`.
- Python tests use pytest; long-running tests get `@pytest.mark.stress`
  (≥100MB tests also check `RUN_STRESS_100MB`). Deep-nesting tests bump
  `sys.setrecursionlimit` in try/finally. Tests touching `strata.config` must
  restore prior state. Use stdlib `json` as the round-trip oracle.

## Markdown / configs

- `.editorconfig`: UTF-8, LF, final newline, 4-space indent (2 for yml/toml/json/md).
- Markdown is checked by markdownlint + mdformat (pre-commit). Lowercase
  filenames except `README.md`, `CLAUDE.md`, and `SKILL.md` files.
- Commit style follows the existing history: imperative summary of *what changed
  and why it is faster/safer*, mentioning refreshed benchmark results when
  applicable.
