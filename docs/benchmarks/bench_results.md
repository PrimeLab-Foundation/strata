# Strata Benchmark Results

Generated: 2026-03-14 03:07:12

## Environment

- Commit: da3fe64dcc1e9344ed60d0ba34a12c4942223201
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 10
- Warmup: 3
- Datasets: benchmarks/data/generated/medium/users.json

## Parsing Benchmarks

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 26.249   | 32.344      | 33.336   | 81.0     |
| orjson        | users.json | 27.005   | 38.742      | 46.475   | 116.3    |
| msgspec       | users.json | 26.328   | 39.747      | 40.878   | 127.1    |
| ujson         | users.json | 37.082   | 54.076      | 55.379   | 139.7    |
| json (stdlib) | users.json | 51.291   | 65.805      | 68.730   | 133.3    |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | eval_query (baseline) | 0.246    | 2000    |
|                                            | jmespath              | 0.807    | 2000    |
|                                            | jsonpath-ng           | 9.268    | 2000    |
| $.users\[*\].orders\[*\].items\[\*\].price | eval_query (baseline) | 6.958    | 100999  |
|                                            | jmespath              | 53.689   | 2000    |
|                                            | jsonpath-ng           | 219.674  | 100999  |
| $..price                                   | jmespath              | 55.227   | 2000    |
|                                            | eval_query (baseline) | 66.618   | 100999  |
|                                            | jsonpath-ng           | 812.170  | 100999  |
| $.users\[?(@.age>30)\]                     | eval_query (baseline) | 0.244    | 1606    |
| $..orders\[?(@.status=="shipped")\]        | eval_query (baseline) | 70.161   | 13300   |
