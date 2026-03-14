# Strata Benchmark Results

Generated: 2026-03-14 01:38:24

## Environment

- Commit: b41fcb56074becce8657c76f62cdb3c4367522ec
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| msgspec       | users.json   | 4.205    | 5.905       | 6.443    | 41.6     |
| orjson        | users.json   | 4.045    | 5.929       | 6.072    | 38.5     |
| ujson         | users.json   | 5.786    | 7.789       | 8.588    | 40.5     |
| strata        | users.json   | 7.418    | 8.239       | 9.013    | 35.4     |
| json (stdlib) | users.json   | 8.339    | 10.116      | 10.455   | 44.4     |
| orjson        | users.ndjson | 4.078    | 5.580       | 6.242    | 52.9     |
| msgspec       | users.ndjson | 4.765    | 6.356       | 6.359    | 54.9     |
| ujson         | users.ndjson | 6.344    | 8.280       | 8.683    | 54.9     |
| json (stdlib) | users.ndjson | 9.196    | 10.728      | 10.963   | 54.9     |
| strata        | users.ndjson | 11.970   | 12.833      | 13.321   | 51.9     |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | eval_query (baseline) | 0.069    | 1000    |
|                                            | jmespath              | 0.333    | 1000    |
|                                            | jsonpath-ng           | 2.671    | 1000    |
| $.users\[*\].orders\[*\].items\[\*\].price | eval_query (baseline) | 1.066    | 12046   |
|                                            | jmespath              | 9.160    | 1000    |
|                                            | jsonpath-ng           | 28.308   | 12046   |
| $..price                                   | jmespath              | 9.645    | 1000    |
|                                            | eval_query (baseline) | 10.344   | 12046   |
|                                            | jsonpath-ng           | 115.479  | 12046   |
| $.users\[?(@.age>30)\]                     | eval_query (baseline) | 0.065    | 794     |
| $..orders\[?(@.status=="shipped")\]        | eval_query (baseline) | 10.695   | 3208    |
