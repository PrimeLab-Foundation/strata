# Strata Benchmark Results

Generated: 2026-01-31 02:53:42

## Environment

- Commit: 0dd6fec82903a7473ecc5709b020843b28cff0e7
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2
- Compiler: Apple clang version 17.0.0 (clang-1700.6.3.2)
- Missing libraries: pysimdjson, jsonpath_ng

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/users.json, benchmarks/data/generated/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users        | 16.975   | 20.231      | 25.407   | 101.0    |
| orjson        | users        | 16.863   | 24.899      | 27.149   | 129.5    |
| msgspec       | users        | 24.137   | 34.866      | 41.690   | 136.0    |
| json (stdlib) | users        | 35.883   | 42.984      | 45.594   | 141.5    |
| ujson         | users        | 29.031   | 45.406      | 49.343   | 147.2    |
| orjson        | users.ndjson | 19.266   | 28.213      | 28.290   | 167.7    |
| msgspec       | users.ndjson | 19.405   | 30.735      | 32.401   | 168.4    |
| ujson         | users.ndjson | 24.298   | 36.807      | 37.394   | 161.8    |
| json (stdlib) | users.ndjson | 32.118   | 42.480      | 43.267   | 155.8    |
| strata        | users.ndjson | 36.490   | 42.854      | 44.732   | 153.8    |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | query | 0.203    | 5000    |
|                                            | jmespath              | 1.062    | 5000    |
| $.users\[*\].orders\[*\].items\[\*\].price | query | 3.151    | 62331   |
|                                            | jmespath              | 29.109   | 5000    |
| $..price                                   | jmespath              | 30.063   | 5000    |
|                                            | query | 31.289   | 62331   |
| $.users\[?(@.age>30)\]                     | query | 0.319    | 3999    |
| $..orders\[?(@.status=="shipped")\]        | query | 33.240   | 16804   |
