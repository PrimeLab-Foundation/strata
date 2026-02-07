# PGO (Profile-Guided Optimization) Build

Strata supports **Profile-Guided Optimization (PGO)** plus **LTO (Link-Time Optimization)** for production performance. The PGO workflow builds twice: first with instrumentation to collect a profile, then with that profile to optimize hot paths.

## Overview

1. **Phase 1 (generate)**: Build with `-fprofile-instr-generate`, run C++ and Python tests and benchmarks; merge `.profraw` into `strata.profdata`.
2. **Phase 2 (use)**: Build with `-fprofile-instr-use=strata.profdata` and LTO; run tests and benchmarks again.

Only **Clang** is supported for PGO (LLVM profile format). GCC can be used for non-PGO builds.

## Requirements

- **Clang** (and **LLVM** for `llvm-profdata merge`)
- Python 3.10+ with venv
- `make`, C++20 toolchain

On Ubuntu:

```bash
sudo apt-get update
sudo apt-get install -y clang llvm
```

On macOS, Xcode Command Line Tools provide `clang` and `xcrun llvm-profdata`.

## Environment Variables

| Variable | Values | Meaning |
|----------|--------|---------|
| `PGO_MODE` | `generate`, `use`, or unset | Build with instrumentation, with profile use, or normal build |
| `STRATA_ENABLE_LTO` | `0`, `1` | Disable/enable LTO (Phase 2 uses LTO) |
| `STRATA_PGO_PROFILE` | path | Path to `.profdata` file (Phase 2) |
| `PGO_DIR` | path | Directory for profile data (default: `build/pgo`) |
| `LLVM_PROFILE_FILE` | pattern | Where to write `.profraw` (Phase 1; script sets this) |
| `CXX` | compiler | Use `clang++` for PGO |

## Running Locally (canonical)

From the repository root (Rule 16: Make is the interface):

```bash
# Use system Python or set PYTHON to your interpreter; CXX=clang++ required for PGO
export PYTHON=python3.12   # optional
export CXX=clang++         # required for PGO
make pgo
```

The Make target invokes `scripts/pgo_build.sh`, which:

1. Creates/uses `.venv` and installs `.[dev,bench]`
2. Runs Phase 1 (generate), then merges profile data (Clang only)
3. Runs Phase 2 (use + LTO)
4. Runs `make test-cpp` and `make test-py` after each phase
5. Runs benchmarks (dumps, ndjson, jsonpath) to feed the profile

Profile output: `build/pgo/strata.profdata`.

## CI Automation

- **Workflow**: `.github/workflows/pgo.yml`
- **Triggers**: Weekly (Monday 03:00 UTC) and `workflow_dispatch`
- **Runner**: `ubuntu-22.04`, Python 3.12, Clang/LLVM installed via apt
- **Steps**: Checkout, set up Python, install Clang/LLVM; **`make pgo`** (Rule 16: Make → scripts); build wheel with PGO+LTO, upload artifact `pgo-wheel`. Tests and benchmarks must pass.

If any step fails (build, tests, or benchmarks), the job fails. No ERROR rows in benchmark output.

## Release Pipeline

For release builds that use PGO:

1. Run the PGO workflow (or use a cached profile from a previous run).
2. Use the uploaded `pgo-wheel` artifact, or run the wheel step locally with the same env and `build/pgo/strata.profdata` present.

Caching profile data across CI runs is possible but not required; weekly runs regenerate the profile.

## Rollback

- Long CI times: run PGO on schedule only and/or reduce benchmark iterations via `PGO_BENCH_REPEAT` / `PGO_BENCH_WARMUP`.
- To disable PGO in CI: remove or disable `.github/workflows/pgo.yml`.
