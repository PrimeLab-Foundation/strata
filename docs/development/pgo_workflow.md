# PGO (Profile-Guided Optimization) Workflow

Profile-Guided Optimization (PGO) is a compiler optimization technique that uses
runtime profiling data to generate more efficient code. For Strata, PGO is expected
to provide **5-15% overall improvement** in parsing and serialization throughput.

## Overview

The PGO workflow consists of two phases:

1. **Generate Phase**: Build with instrumentation, run representative workloads to collect profile data
2. **Use Phase**: Rebuild using the collected profile data to optimize hot paths

Strata's PGO workflow automatically:
- Compiles the extension with profile instrumentation
- Runs comprehensive benchmarks (loads, dumps, NDJSON, JSONPath)
- Merges profile data
- Rebuilds with PGO + LTO (Link-Time Optimization)
- Validates correctness with test suites

## Quick Start

```bash
# Default PGO build (medium dataset, recommended for development)
make pgo

# For releases (large dataset, 100 repetitions)
make pgo-release
```

## Available Make Targets

| Target | Description | Dataset | Use Case |
|--------|-------------|---------|----------|
| `make pgo` | Standard PGO build | medium | Development, CI |
| `make pgo-small` | Fast PGO build | small | Quick iteration |
| `make pgo-large` | Comprehensive PGO build | large | Pre-release |
| `make pgo-release` | Release PGO build | large (100 reps) | Final releases |

## Configuration

Environment variables for fine-tuning the PGO workflow:

| Variable | Default | Description |
|----------|---------|-------------|
| `PGO_DATASET_SIZE` | `medium` | Dataset size: `small`, `medium`, `large` |
| `PGO_BENCH_REPEAT` | `50` | Number of benchmark repetitions |
| `PGO_BENCH_WARMUP` | `3` | Number of warmup iterations |
| `PYTHON` | `python3.14` | Python interpreter to use |
| `CXX` | `c++` (system default) | C++ compiler (clang++ recommended) |
| `VENV` | `.venv` | Virtual environment path |

### Examples

```bash
# Custom configuration
PGO_DATASET_SIZE=large PGO_BENCH_REPEAT=100 make pgo

# Using specific compiler
CXX=clang++ make pgo

# Quick iteration during development
PGO_DATASET_SIZE=small PGO_BENCH_REPEAT=10 make pgo
```

## Profiled Workloads

The PGO profile is generated from comprehensive workloads covering all hot paths:

### 1. JSON Parsing (`loads`)
- Parse JSON strings to Python objects
- Covers: objects, arrays, strings (with escapes), numbers, booleans, null
- Tests unicode handling and deeply nested structures

### 2. JSON Serialization (`dumps`)
- Serialize Python objects to JSON strings
- Covers: dicts, lists, strings, numbers, booleans, None
- Tests output formatting and escape sequences

### 3. NDJSON Streaming
- Line-by-line parsing of newline-delimited JSON
- Covers: streaming iteration, memory-efficient processing
- Tests handling of various line endings

### 4. JSONPath Queries
- Query evaluation and search
- Covers: path compilation, recursive descent, filtering
- Tests various query patterns

## Dataset Sizes

The benchmark data generator creates realistic nested JSON structures:

| Size | Users | Max Orders | Max Items | Typical File Size |
|------|-------|------------|-----------|-------------------|
| small | 1,000 | 10 | 5 | ~2 MB |
| medium | 2,000 | 20 | 10 | ~12 MB |
| large | 4,000 | 40 | 20 | ~50 MB |

## Compiler Support

### Clang (Recommended)
- Uses LLVM's instrumentation-based PGO
- Profile data: `build/pgo/*.profraw` → merged to `strata.profdata`
- Requires: `llvm-profdata` (included with Xcode on macOS)

### GCC
- Uses GCC's PGO infrastructure
- Profile data: `build/pgo/gen/*.gcda`
- Auto-detected by the script

## Output Artifacts

After a successful PGO build:

```
build/pgo/
├── gen/                      # Raw profile data
│   └── *.profraw            # (Clang) or *.gcda (GCC)
├── strata.profdata          # Merged profile (Clang)
└── bench_results_pgo.md     # Benchmark results
```

## Best Practices

### For Releases

1. Always run `make pgo-release` before cutting a release
2. Verify benchmark results show expected improvements
3. Document PGO benchmark results in release notes
4. Store profile data for reproducibility if needed

### For CI/CD

```yaml
# Example CI step
- name: PGO Build
  run: |
    make pgo
    # Verify tests pass with optimized build
    make test
```

### Troubleshooting

**Profile merge fails:**
```bash
# Ensure LLVM tools are available
xcrun llvm-profdata --version  # macOS
llvm-profdata --version        # Linux
```

**Benchmark data missing:**
```bash
# Generate benchmark data first
make bench-data
```

**Compiler not detected:**
```bash
# Explicitly set compiler
CXX=clang++ make pgo
```

## Expected Performance Gains

Based on typical workloads, PGO + LTO provides:

| Operation | Expected Improvement |
|-----------|---------------------|
| `loads` (parsing) | 5-15% |
| `dumps` (serialization) | 5-10% |
| NDJSON iteration | 5-12% |
| JSONPath queries | 8-15% |

Actual gains depend on:
- CPU architecture and cache sizes
- Workload characteristics
- Compiler version

## Technical Details

### PGO Flags (Clang)
- Generate: `-fprofile-instr-generate`
- Use: `-fprofile-instr-use=<profile.profdata>`

### LTO (Link-Time Optimization)
- Combined with PGO in the "use" phase
- Enables whole-program optimization
- Flag: `-flto=thin` (Clang) or `-flto` (GCC)

### Profile Data Format
- Clang uses indexed profile format (`.profdata`)
- Raw data collected per-process (`.profraw`)
- Merged using `llvm-profdata merge`

## See Also

- [Clang PGO Documentation](https://clang.llvm.org/docs/UsersManual.html#profile-guided-optimization)
- [GCC PGO Documentation](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#Optimize-Options)
- Project benchmarks: `make bench-all`
- Benchmark results: `docs/benchmarks/`
