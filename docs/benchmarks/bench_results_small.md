# Strata Benchmark Results

Generated: 2026-02-01 19:43:01

## Environment

- Commit: 0dd6fec82903a7473ecc5709b020843b28cff0e7
- OS: macOS-26.2-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users.json   | 4.359    | 5.753       | 6.247    | 43.6     |
| msgspec       | users.json   | 4.421    | 5.934       | 6.689    | 48.7     |
| ujson         | users.json   | 5.629    | 8.797       | 9.217    | 46.7     |
| json (stdlib) | users.json   | 8.620    | 9.835       | 10.399   | 50.6     |
| strata        | users.json   | 11.455   | 13.437      | 13.732   | 44.9     |
| msgspec       | users.ndjson | 4.729    | 6.351       | 6.384    | 55.6     |
| orjson        | users.ndjson | 4.067    | 6.389       | 6.639    | 54.6     |
| ujson         | users.ndjson | 5.760    | 8.290       | 8.830    | 55.6     |
| json (stdlib) | users.ndjson | 9.440    | 10.555      | 11.111   | 55.6     |
| strata        | users.ndjson | 11.088   | 13.256      | 14.077   | 54.6     |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | eval_query (baseline) | 0.053    | 1000    |
|                                            | jmespath              | 0.328    | 1000    |
|                                            | jsonpath-ng           | 2.718    | 1000    |
| $.users\[*\].orders\[*\].items\[\*\].price | eval_query (baseline) | 1.031    | 12046   |
|                                            | jmespath              | 9.283    | 1000    |
|                                            | jsonpath-ng           | 28.558   | 12046   |
| $..price                                   | jmespath              | 9.832    | 1000    |
|                                            | eval_query (baseline) | 10.793   | 12046   |
|                                            | jsonpath-ng           | 116.803  | 12046   |
| $.users\[?(@.age>30)\]                     | eval_query (baseline) | 0.062    | 794     |
| $..orders\[?(@.status=="shipped")\]        | eval_query (baseline) | 11.179   | 3208    |
