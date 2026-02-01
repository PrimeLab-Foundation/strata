# Strata Benchmark Results

Generated: 2026-02-02 01:17:50

## Environment

- Commit: d702fd7c34ee1fc0f3bef8fa8d17d6d26c0ee77e
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
| orjson        | users.json   | 4.468    | 6.512       | 6.698    | 44.8     |
| msgspec       | users.json   | 4.767    | 6.767       | 6.970    | 48.0     |
| ujson         | users.json   | 6.326    | 8.580       | 9.675    | 46.9     |
| strata        | users.json   | 8.838    | 10.723      | 10.987   | 45.1     |
| json (stdlib) | users.json   | 8.560    | 11.125      | 11.507   | 50.4     |
| orjson        | users.ndjson | 4.982    | 6.217       | 6.511    | 54.4     |
| msgspec       | users.ndjson | 4.757    | 6.419       | 6.671    | 56.5     |
| ujson         | users.ndjson | 6.372    | 9.037       | 9.247    | 56.5     |
| json (stdlib) | users.ndjson | 9.683    | 11.314      | 11.878   | 56.5     |
| strata        | users.ndjson | 11.343   | 14.479      | 15.016   | 54.4     |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | eval_query (baseline) | 0.057    | 1000    |
|                                            | jmespath              | 0.334    | 1000    |
|                                            | jsonpath-ng           | 2.712    | 1000    |
| $.users\[*\].orders\[*\].items\[\*\].price | eval_query (baseline) | 0.996    | 12046   |
|                                            | jmespath              | 9.359    | 1000    |
|                                            | jsonpath-ng           | 30.642   | 12046   |
| $..price                                   | jmespath              | 10.127   | 1000    |
|                                            | eval_query (baseline) | 10.740   | 12046   |
|                                            | jsonpath-ng           | 119.102  | 12046   |
| $.users\[?(@.age>30)\]                     | eval_query (baseline) | 0.064    | 794     |
| $..orders\[?(@.status=="shipped")\]        | eval_query (baseline) | 11.105   | 3208    |
