# Strata Benchmark Results

Generated: 2026-02-02 00:20:22

## Environment

- Commit: b65dd5eb9b9cb32e4d31b6cdd5ec62e774f084fc
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
| orjson        | users.json   | 4.605    | 6.119       | 6.431    | 44.6     |
| msgspec       | users.json   | 4.715    | 6.578       | 7.232    | 47.7     |
| ujson         | users.json   | 5.804    | 8.794       | 9.091    | 46.7     |
| json (stdlib) | users.json   | 8.850    | 10.714      | 11.284   | 50.5     |
| strata        | users.json   | 10.964   | 13.585      | 14.161   | 44.9     |
| orjson        | users.ndjson | 4.382    | 6.043       | 6.416    | 53.6     |
| msgspec       | users.ndjson | 4.861    | 6.561       | 6.862    | 55.6     |
| ujson         | users.ndjson | 6.274    | 8.862       | 9.034    | 55.6     |
| json (stdlib) | users.ndjson | 9.597    | 11.456      | 11.630   | 55.6     |
| strata        | users.ndjson | 11.133   | 14.239      | 14.621   | 53.6     |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | eval_query (baseline) | 0.057    | 1000    |
|                                            | jmespath              | 0.341    | 1000    |
|                                            | jsonpath-ng           | 3.174    | 1000    |
| $.users\[*\].orders\[*\].items\[\*\].price | eval_query (baseline) | 1.000    | 12046   |
|                                            | jmespath              | 9.470    | 1000    |
|                                            | jsonpath-ng           | 30.352   | 12046   |
| $..price                                   | jmespath              | 10.299   | 1000    |
|                                            | eval_query (baseline) | 10.904   | 12046   |
|                                            | jsonpath-ng           | 120.132  | 12046   |
| $.users\[?(@.age>30)\]                     | eval_query (baseline) | 0.076    | 794     |
| $..orders\[?(@.status=="shipped")\]        | eval_query (baseline) | 11.378   | 3208    |
