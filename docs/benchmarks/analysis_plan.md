# Benchmark Analysis Plan (Hybrid Architecture)

## Objective
Measure the performance impact of transitioning from DOM-based parsing to a SAX-based hybrid architecture with a direct-to-Python builder.

## Commands to run
- **Full suite**: `make bench-small` (runs `bench_main`, `bench_loads`, `bench_dumps`, `bench_ndjson`, `bench_jsonpath` with warmup + repeats).
- **Targeted parsing bench**: `PYTHONPATH=. .venv/bin/python3.14 -m benchmarks.bench_loads --data benchmarks/data/generated/small/users.json --repeat 5 --warmup 2`

## Datasets
- **Small**: `benchmarks/data/generated/small/users.json` (≈1 MB, 1000 users).
- **Medium**: `benchmarks/data/generated/medium/users.json` (≈5 MB, 2000 users).
- **Large**: `benchmarks/data/generated/large/users.json` (≈20 MB, 4000 users).

## Metrics tracked
- **Latency**: Min/Median/P95 (primary metric for `loads`).
- **Memory**: Peak RSS during `loads`.
- **Throughput**: MB/s derived from latency and dataset size.

## Baseline
A baseline run will be performed before any changes to the parser. Results will be recorded in `docs/benchmarks/progress_log.md`.
A "win" for Phase 4 (Direct-to-Python) is expected to be >20% improvement in `loads` latency due to avoided double-materialization and reduced C++ allocations.
