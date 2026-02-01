---
alwaysApply: true
---
# Rule: Repository Layout + Naming Conventions (Non-Negotiable)

Keep structure and naming predictable across C++ and Python.

## Repository layout
- C++ production code: `src/`
- C++ public headers: `include/`
- Python package: `python/strata/` (or `src/strata/` if repo uses src-layout)
- C++ tests: `tests/cpp/`
- Python tests: `tests/py/`
- Benchmarks: `bench/` or `benchmarks/`
- Docs: `docs/` only
- Legacy/experimental: `cpp_bkp/` or `experiments/` (must not be linked/imported into production)

## Naming conventions — C++ (C++20)
- Namespace: `strata` and nested namespaces (e.g., `strata::json`, `strata::ndjson`, `strata::jsonpath`)
- Types: `PascalCase`
- Functions/methods: `snake_case`
- Constants: `kPascalCase`
- Filenames: `snake_case` for `.hpp/.cpp`
- Prefer `#pragma once`
- Public API headers must be minimal; internal headers should live in `src/` or `include/strata/detail/`.

## Naming conventions — Python
- Modules/functions: `snake_case`
- Public classes: `PascalCase`
- Keep exports minimal via `strata/__init__.py`
- Internal helpers must be prefixed with `_` and not exported.
