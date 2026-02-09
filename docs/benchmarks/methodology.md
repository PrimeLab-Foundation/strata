# Benchmark Methodology

## Overview

Strata benchmarks follow strict fairness rules to ensure reproducible, unbiased performance comparisons against competitor JSON libraries.

## Automation

- Weekly CI benchmarks are scheduled in `/.github/workflows/benchmark.yml`.
- Results are written to `docs/benchmarks/bench_results.md` and uploaded as artifacts.
- CI fails if benchmark output contains **ERROR** rows.
- **Regression check**: After each run, `benchmarks/regression_check.py` compares Strata metrics (median time, RSS) to a stored baseline in `benchmarks/results/baseline.json`. If any Strata metric regresses by more than 5%, the job fails. No baseline file means the check is skipped (exit 0).

## Fairness Rules

### 1. Same Input Data

All libraries process **identical input datasets** for each benchmark:

- Same JSON files
- Same byte sequences
- No library-specific optimizations in data format

### 2. Warmup Phase

Before timing measurements:

- Run each operation `warmup` times (default: 1-2 iterations)
- Ensures JIT compilation, cache warming, and steady-state
- Warmup results are **not** included in reported times

### 3. Multiple Iterations

Each measurement:

- Runs `repeat` times (default: 3-10 iterations)
- Garbage collection forced between iterations
- Reports: **min**, **median**, **p95**, and optionally mean

**Why these metrics:**

- **Min**: Best-case performance (ideal conditions)
- **Median**: Typical performance (robust to outliers)
- **P95**: Near-worst-case (captures tail latency)

### 4. Memory Tracking

When `psutil` is available:

- Record RSS (Resident Set Size) after parsing
- Measure peak memory usage
- Report in MB for human readability

### 5. Error Handling

- Libraries that fail to parse valid input are marked as **ERROR**
- Error messages are captured and reported
- Partial results are never used

### 6. Search Strata Mode (Fair Comparison)

For Search benchmarks, the **default** is cursor (query only), so query-engine speed is compared fairly without changing any option. Use `--strata-mode` only when you want to measure a different workflow:

- **cursor** (default): `parse_json_file(path)` once, then `search(cursor, path)` — **query only**; fair comparison vs jmespath/jsonpath-ng.
- **string**: `search(text, path)` — parse+query per call (no dumps); fairer when comparing “one parse + one query”.
- **dict**: `search(loads(text), path)` — each call does serialize→parse→query; use `--strata-mode dict` for loads()+search() in-memory workflow.

Default (cursor) gives fair query-engine comparison; use `--strata-mode dict` when comparing Strata’s query engine to jmespath/jsonpath-ng. See `docs/benchmarks/strata_performance_analysis.md`.

## Competitor Libraries

Benchmarks compare against:

| Library           | Version  | Language       | Notes                              |
| ----------------- | -------- | -------------- | ---------------------------------- |
| **orjson**        | Latest   | Rust           | Fast serialization/deserialization |
| **ujson**         | Latest   | C              | Ultra JSON                         |
| **msgspec**       | Latest   | C + Python     | Modern fast serialization          |
| **ijson**         | Latest   | Python + C     | Streaming JSON parser              |
| **pysimdjson**    | Latest   | C++ (simdjson) | SIMD-accelerated                   |
| **jmespath**      | Latest   | Python         | Search queries                   |
| **jsonpath-ng**   | Latest   | Python         | Search queries                   |
| **json (stdlib)** | Built-in | Python         | Baseline reference                 |

**Import Rules:**

- Competitor libraries imported **only** in `benchmarks/` directory
- **Never** imported in `strata/` core code
- No fallback dependencies

## Environment Recording

Each benchmark run records:

### System Information

- **CPU**: Model, cores, frequency
- **OS**: Name and version
- **Python**: Version (e.g., 3.14.2)
- **Compiler**: For C++ extensions (clang++, g++, version, flags)

### Build Configuration

- **Optimization**: Release build (-O3)
- **C++ Standard**: C++20
- **Python Flags**: ABI, debug mode

### Library Versions

- Exact version of each competitor library
- Strata version/commit hash

## Benchmark Categories

### Parsing (`loads`)

- **Small**: \< 10 KB
- **Medium**: 10 KB - 1 MB
- **Large**: > 1 MB
- **Nested objects**: Deep object hierarchies
- **Nested arrays**: Deep array nesting
- **String-heavy**: Many/long strings
- **Number-heavy**: Many numeric values
- **Mixed realistic**: Real-world API response patterns
- **NDJSON inputs**: Benchmarked via `iter_ndjson` when dataset ends with `.ndjson`

### Serialization (`dumps`)

- Same categories as parsing
- Python object → JSON bytes
- Escaping and formatting rules
- Measures min/median/p95 + RSS (when `psutil` is available)

### NDJSON Streaming (`iter_ndjson`)

- Line-by-line iteration
- Throughput (lines/second)
- Memory profile (constant vs. linear)
- File-like object streaming

### Query/Search (`search`)

- Repeated queries (amortized cost)
- Low selectivity (few results)
- High selectivity (many results)
- Deep path navigation
- Array slicing
- Note: JMESPath does not support recursive descent (`$..`), so those queries are skipped for JMESPath comparisons.

## Reproducibility

### Deterministic Data Generation

All datasets use:

- **Fixed random seed** (default: 42)
- Deterministic algorithms
- Documented generation parameters

### Running Benchmarks

```bash
# Quick sanity check
make bench-small

# Full benchmark suite
make bench-all

# Custom configuration
python -m benchmarks.bench_main \
  --dataset benchmarks/data/generated/users.json \
  --dataset benchmarks/data/generated/users.ndjson \
  --repeat 10 \
  --warmup 3 \
  --output docs/benchmarks/bench_results.md
```

### Regression Check

```bash
# Check current results against baseline (fail if >5% regression)
python -m benchmarks.regression_check docs/benchmarks/bench_results.md --threshold 5

# Create or update baseline (e.g. after a known-good run)
python -m benchmarks.regression_check docs/benchmarks/bench_results.md --save-baseline
```

- Baseline is stored in `benchmarks/results/baseline.json`.
- Only **Strata** parsing metrics (median ms, RSS per dataset) are compared; query metrics are compared if present.
- To establish a strata: run `bench_main`, then run `regression_check --save-baseline`, then commit `benchmarks/results/baseline.json`.

### Output Format

Results saved as markdown tables:

```markdown
| Library | Dataset | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|---------|----------|-------------|----------|----------|
| strata  | users   | 22.4     | 24.8        | 26.9     | 106.1    |
| orjson  | users   | 15.6     | 24.0        | 25.5     | 129.7    |
```

## Known Limitations

### Current Implementation

1. **No NDJSON benchmarks** (API not implemented)
1. **No dumps benchmarks** (serialization not implemented)
1. **Limited query benchmarks** (Search not implemented)
1. **pysimdjson often unavailable** (Python 3.14 compatibility)

### Platform Differences

- Results vary by CPU, OS, Python version
- Benchmarks should run on same machine for comparison
- Reported results include environment details

## Acceptance Criteria

Per Strata's mission:

> **Strata must be #1** (or explicitly flag known regressions)

**Target:**

- Parsing: Match or beat `orjson` (within 5%)
- Memory: Best or competitive (within 10%)
- Queries: Beat `jmespath`/`jsonpath-ng` by > 20%

If Strata regresses:

- Document in benchmark results
- File tracking issue with details
- Set deadline for fix

## Future Improvements

1. **Profiling integration**: Collect perf/Instruments data
1. **CI/CD integration**: Automated benchmark runs
1. **Historical tracking**: Track performance over time
1. **Flamegraphs**: Visualize hot paths
1. **Comparison charts**: HTML output with graphs
