# Strata Experimental Benchmarks
Generated: 2026-02-14 18:25:03

- Data: benchmarks/data/generated/medium/users.json
- Repeat: 5, Warmup: 2

## Materialization Overhead

- Data: benchmarks/data/generated/medium/users.json
- Size: 6.25 MB
- Objects: 889,491

| Benchmark                  | Min (ms) | Median (ms) | Mean (ms) |
| -------------------------- | -------- | ----------- | --------- |
| Strata (parse+materialize) | 41.69    | 47.37       | 46.45     |
| Materialization only       | 84.72    | 93.86       | 94.45     |

## Object Pool (dict presize)

- Data: benchmarks/data/generated/medium/users.json (6.25 MB)
- Iterations: 5, Warmup: 2

| Mode     | Min (ms) | Median (ms) | P95 (ms) | MB/s  |
| -------- | -------- | ----------- | -------- | ----- |
| Pool ON  | 45.89    | 46.47       | 46.80    | 134.5 |
| Pool OFF | 45.47    | 46.52       | 47.15    | 134.3 |

## Parallel JSON Experiment

- Data: benchmarks/data/generated/medium/users.json (6.25 MB)
- Repeat: 5, Warmup: 2
- num_threads: 0, min_chunk_size: 0

| Mode                       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) | Items |
| -------------------------- | -------- | ----------- | -------- | ------ | -------- | ----- |
| strata                     | 45.54    | 46.40       | 47.62    | 141.19 | 116.4    | 2000  |
| strata_parallel_experiment | 57.22    | 57.79       | 58.91    | 113.37 | 258.6    | 2000  |
