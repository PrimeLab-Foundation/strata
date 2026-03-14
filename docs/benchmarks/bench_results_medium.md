# Strata Benchmark Results

Generated: 2026-03-14 01:38:36

## Environment

- Commit: b41fcb56074becce8657c76f62cdb3c4367522ec
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/medium/users.json, benchmarks/data/generated/medium/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users.json   | 25.801   | 36.302      | 37.274   | 93.1     |
| msgspec       | users.json   | 26.839   | 38.752      | 39.599   | 91.9     |
| strata        | users.json   | 40.642   | 51.923      | 52.487   | 71.9     |
| ujson         | users.json   | 36.958   | 53.094      | 53.487   | 110.5    |
| json (stdlib) | users.json   | 53.533   | 64.155      | 65.408   | 98.2     |
| orjson        | users.ndjson | 32.205   | 34.791      | 38.172   | 106.4    |
| msgspec       | users.ndjson | 34.434   | 36.410      | 38.223   | 105.3    |
| ujson         | users.ndjson | 44.672   | 50.414      | 52.108   | 123.0    |
| json (stdlib) | users.ndjson | 61.661   | 62.731      | 65.469   | 107.8    |
| strata        | users.ndjson | 60.497   | 72.270      | 73.998   | 128.9    |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | eval_query (baseline) | 0.248    | 2000    |
|                                            | jmespath              | 0.820    | 2000    |
|                                            | jsonpath-ng           | 8.347    | 2000    |
| $.users\[*\].orders\[*\].items\[\*\].price | eval_query (baseline) | 7.397    | 100999  |
|                                            | jmespath              | 52.520   | 2000    |
|                                            | jsonpath-ng           | 210.480  | 100999  |
| $..price                                   | jmespath              | 53.721   | 2000    |
|                                            | eval_query (baseline) | 65.823   | 100999  |
|                                            | jsonpath-ng           | 791.354  | 100999  |
| $.users\[?(@.age>30)\]                     | eval_query (baseline) | 0.236    | 1606    |
| $..orders\[?(@.status=="shipped")\]        | eval_query (baseline) | 68.258   | 13300   |
