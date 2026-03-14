# Fuzzing (libFuzzer + ASan/UBSan)

Strata uses **libFuzzer** with **AddressSanitizer (ASan)** and **UndefinedBehaviorSanitizer (UBSan)** to find correctness and memory-safety bugs in the JSON parser and NDJSON stream.

## Overview

- **fuzz_loads**: Fuzzes `strata::parse_json` (loads) with arbitrary byte inputs.
- **fuzz_ndjson**: Fuzzes `strata::NdjsonStream` with arbitrary byte inputs.

Builds use `-fsanitize=fuzzer,address,undefined`. Crashes and timeouts are written to artifact files for reproduction.

**Canonical interface (Rule 16):** Use Make targets; scripts are invoked via Make.

## Requirements

- **Clang** (libFuzzer is part of the compiler runtime)
- **CMake** 3.20+
- No Python required for building fuzz targets

On Ubuntu:

```bash
sudo apt-get update
sudo apt-get install -y clang llvm-14 cmake
```

## Building and running (canonical)

From the repository root:

```bash
# Build fuzz targets (configure FUZZ=ON, build fuzz_loads and fuzz_ndjson)
make fuzz-build

# Run both fuzzers (default 120s each; crashes written to fuzz_crashes/)
make fuzz-run

# Build and run in one step
make fuzz
```

Environment variables (optional):

- `FUZZ_TIME` — max total time per fuzzer in seconds (default: 120)
- `FUZZ_ARTIFACT_PREFIX` — directory/prefix for crash artifacts (default: `fuzz_crashes/`)
- `FUZZ_BUILD_DIR` — build directory (default: `build_fuzz`)
- `CXX` — compiler (default: `clang++`)

Example: run for 60 seconds and put crashes in `./crash_`:

```bash
FUZZ_TIME=60 FUZZ_ARTIFACT_PREFIX=crash_ make fuzz-run
```

## Script (for debugging)

The Makefile calls `scripts/fuzz.sh` with subcommands `build` or `run`. To run the script directly:

```bash
bash scripts/fuzz.sh build   # configure and build
bash scripts/fuzz.sh run     # run both fuzzers (requires build first)
bash scripts/fuzz.sh all      # build then run
```

## Corpus

- **tests/fuzz/corpus/loads/**: Minimal valid JSON for `fuzz_loads` (null, object, array).
- **tests/fuzz/corpus/ndjson/**: Minimal NDJSON lines for `fuzz_ndjson`.

Adding more valid or edge-case inputs to the corpus improves coverage. Do not commit large or binary files; keep corpus small and deterministic.

## CI Automation

- **Workflow**: `.github/workflows/fuzz.yml`
- **Triggers**: Weekly (Tuesday 04:00 UTC) and `workflow_dispatch`
- **Runner**: `ubuntu-22.04`, Clang/LLVM-14, CMake
- **Steps**: `make fuzz-run` (Rule 16: Make → scripts). Builds and runs both fuzzers (120s each); on failure, uploads `fuzz_crashes/` as artifact `fuzz-crashes`.

Crashes are captured automatically; download the artifact and reproduce with:

```bash
make fuzz-build
./build_fuzz/fuzz_loads fuzz_crashes/loads_<id>
```

## Platform notes

- **Linux (Ubuntu)**: Clang + LLVM-14 provide libFuzzer and sanitizer runtimes; CI uses this.
- **macOS**: System Clang (Xcode/Command Line Tools) does **not** ship libFuzzer. To run `make fuzz` locally:
  1. Install LLVM via Homebrew: `brew install llvm`
  1. The fuzz script will automatically use `$(brew --prefix llvm)/bin/clang++` when the default compiler is system Clang.
  1. Or set explicitly: `CXX=$(brew --prefix llvm)/bin/clang++ make fuzz`
     If the linker fails with `libclang_rt.fuzzer_osx.a not found`, install LLVM as above or use Linux/CI for fuzzing.

## Rollback

- **CI timeouts**: Reduce `-max_total_time` (e.g. 60s per fuzzer) or run fuzzing on schedule only.
- **Disable fuzzing in CI**: Remove or disable `.github/workflows/fuzz.yml`.
- **Build issues**: Ensure Clang is used (`-DCMAKE_CXX_COMPILER=clang++`); libFuzzer is not supported with GCC.
