# Benchmark Analysis Plan (Phase 0/1)

- **Commands to run (baseline/iteration)**: `make bench-small` (runs `bench_main`, `bench_loads`, `bench_dumps`, `bench_ndjson`, `bench_jsonpath` with warmup + repeats), plus targeted reruns of individual benches if a change affects only one area (e.g., `benchmarks.bench_dumps` after serialization tweaks).
- **Datasets**: canonical generated small set in `benchmarks/data/generated/small/users.json` and `users.ndjson` (≈1 MB); reuse existing generated data from `make bench-data`.
- **Metrics tracked**: min/median/p95 latency per benchmark plus RSS when reported; rank relative to competitors. A “win” means ≥5% improvement in median and/or p95 without regressions elsewhere or correctness changes.
- **Raw data location**: `docs/benchmarks/raw/<date>_<machine>/<run_label>/` (append-only), with `bench_results_small.md` updated by `make bench-small`.
- **Baseline label**: `baseline` for today’s run; subsequent themes get their own subfolders (e.g., `parsing_arena`, `dumps_numbers`) with matching entries in `progress_log.md`.
