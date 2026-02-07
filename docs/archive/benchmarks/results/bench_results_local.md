# Strata Benchmark Results

Generated: 2026-01-31 03:33:54

## Environment

- Commit: 0dd6fec82903a7473ecc5709b020843b28cff0e7
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2
- Compiler: Apple clang version 17.0.0 (clang-1700.6.3.2)
- Missing libraries: pysimdjson, jsonpath_ng

## Configuration

- Repeat: 1
- Warmup: 1
- Datasets: benchmarks/data/generated/users.json, benchmarks/data/generated/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users        | 20.697   | 20.697      | 20.697   | 115.6    |
| msgspec       | users        | 23.659   | 23.659      | 23.659   | 119.8    |
| strata        | users        | 27.758   | 27.758      | 27.758   | 68.8     |
| ujson         | users        | 32.821   | 32.821      | 32.821   | 132.4    |
| json (stdlib) | users        | 35.250   | 35.250      | 35.250   | 121.2    |
| msgspec       | users.ndjson | 21.172   | 21.172      | 21.172   | 118.4    |
| orjson        | users.ndjson | 24.163   | 24.163      | 24.163   | 117.1    |
| ujson         | users.ndjson | 26.436   | 26.436      | 26.436   | 119.2    |
| json (stdlib) | users.ndjson | 34.296   | 34.296      | 34.296   | 110.3    |
| strata        | users.ndjson | 50.310   | 50.310      | 50.310   | 133.4    |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | eval_query (baseline) | 0.337    | 5000    |
|                                            | jmespath              | 1.077    | 5000    |
| $.users\[*\].orders\[*\].items\[\*\].price | eval_query (baseline) | 3.323    | 62331   |
|                                            | jmespath              | 29.025   | 5000    |
| $..price                                   | jmespath              | 30.760   | 5000    |
|                                            | eval_query (baseline) | 31.571   | 62331   |
| $.users\[?(@.age>30)\]                     | eval_query (baseline) | 0.561    | 3999    |
| $..orders\[?(@.status=="shipped")\]        | eval_query (baseline) | 35.278   | 16804   |

## Serialization Benchmarks (dumps)

Command: `python3.14 -m benchmarks.bench_dumps --data benchmarks/data/generated/users.json --repeat 50 --warmup 3`

| Run    | Min (ms) | Median (ms) | P95 (ms) | Size (bytes) | RSS (MB) |
| ------ | -------- | ----------- | -------- | ------------ | -------- |
| Before | 7.52     | 8.19        | 8.99     | 4706770      | 72.0     |
| After  | 7.46     | 7.94        | 8.67     | 4706770      | 71.7     |
