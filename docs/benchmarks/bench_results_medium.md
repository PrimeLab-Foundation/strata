# Strata Benchmark Results

Generated: 2026-02-01 02:59:20

## Environment

- Commit: 0dd6fec82903a7473ecc5709b020843b28cff0e7
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/medium/users.json, benchmarks/data/generated/medium/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users.json   | 19.716   | 26.465      | 28.372   | 133.7    |
| msgspec       | users.json   | 19.540   | 28.564      | 28.813   | 132.5    |
| ujson         | users.json   | 27.363   | 42.556      | 43.832   | 151.1    |
| json (stdlib) | users.json   | 35.124   | 44.126      | 45.297   | 138.8    |
| strata        | users.json   | 44.560   | 54.917      | 58.647   | 134.9    |
| orjson        | users.ndjson | 21.546   | 22.993      | 23.953   | 156.5    |
| msgspec       | users.ndjson | 23.594   | 25.240      | 25.393   | 155.3    |
| ujson         | users.ndjson | 30.453   | 34.884      | 35.460   | 174.0    |
| json (stdlib) | users.ndjson | 38.096   | 40.453      | 42.161   | 157.7    |
| strata        | users.ndjson | 38.615   | 50.393      | 51.725   | 171.6    |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | eval_query (baseline) | 0.130    | 2000    |
|                                            | jmespath              | 0.441    | 2000    |
|                                            | jsonpath-ng           | 5.992    | 2000    |
| $.users\[*\].orders\[*\].items\[\*\].price | eval_query (baseline) | 3.602    | 100999  |
|                                            | jmespath              | 31.820   | 2000    |
|                                            | jsonpath-ng           | 132.693  | 100999  |
| $..price                                   | jmespath              | 33.405   | 2000    |
|                                            | eval_query (baseline) | 37.742   | 100999  |
|                                            | jsonpath-ng           | 500.486  | 100999  |
| $.users\[?(@.age>30)\]                     | eval_query (baseline) | 0.156    | 1606    |
| $..orders\[?(@.status=="shipped")\]        | eval_query (baseline) | 39.786   | 13300   |
