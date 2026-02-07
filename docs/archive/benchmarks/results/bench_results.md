# Strata Benchmark Results

Generated: 2026-01-31 21:01:00

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
| orjson        | users        | 378.154  | 497.858     | 533.189  | 2558.5   |
| msgspec       | users        | 424.426  | 573.136     | 580.760  | 2577.9   |
| strata        | users        | 483.173  | 575.712     | 594.005  | 1945.9   |
| ujson         | users        | 622.058  | 782.517     | 916.795  | 2921.8   |
| json (stdlib) | users        | 759.789  | 903.651     | 918.926  | 2707.7   |
| orjson        | users.ndjson | 523.415  | 523.801     | 565.692  | 1870.2   |
| msgspec       | users.ndjson | 538.424  | 546.179     | 565.143  | 1768.5   |
| ujson         | users.ndjson | 728.806  | 745.211     | 764.402  | 2053.8   |
| json (stdlib) | users.ndjson | 869.334  | 877.735     | 883.110  | 1713.7   |
| strata        | users.ndjson | 880.074  | 1082.316    | 1229.576 | 2464.8   |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | eval_query (baseline) | 0.398    | 2000    |
|                                            | jmespath              | 1.041    | 2000    |
| $.users\[*\].orders\[*\].items\[\*\].price | eval_query (baseline) | 78.208   | 2485232 |
|                                            | jmespath              | 595.810  | 2000    |
| $..price                                   | jmespath              | 574.783  | 2000    |
|                                            | eval_query (baseline) | 770.840  | 2485232 |
| $.users\[?(@.age>30)\]                     | eval_query (baseline) | 0.387    | 1563    |
| $..orders\[?(@.status=="shipped")\]        | eval_query (baseline) | 818.412  | 66414   |
