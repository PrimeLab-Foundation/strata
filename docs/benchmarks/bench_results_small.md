# Strata Benchmark Results

Generated: 2026-03-14 22:14:38

## Environment

- Commit: a41eea07ede3c1eca4ad03b9ad1a728387619922
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
| strata        | users.json   | 3.487    | 4.239       | 4.411    | 35.2     |
| orjson        | users.json   | 4.006    | 5.448       | 6.197    | 38.4     |
| msgspec       | users.json   | 4.644    | 5.818       | 6.322    | 41.6     |
| ujson         | users.json   | 5.786    | 7.554       | 8.242    | 40.5     |
| json (stdlib) | users.json   | 8.343    | 9.830       | 10.252   | 44.4     |
| orjson        | users.ndjson | 4.248    | 5.625       | 5.758    | 54.8     |
| msgspec       | users.ndjson | 4.465    | 6.009       | 6.180    | 56.8     |
| strata        | users.ndjson | 6.999    | 7.614       | 7.797    | 54.8     |
| ujson         | users.ndjson | 6.654    | 8.189       | 8.702    | 56.8     |
| json (stdlib) | users.ndjson | 9.308    | 10.679      | 10.738   | 56.8     |

## Serialization Benchmarks

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| orjson  | users.json | 1.396    | 1.435       | 1.571    | 48.1     |
| msgspec | users.json | 1.422    | 1.430       | 1.438    | 49.2     |
| strata  | users.json | 1.694    | 1.724       | 1.905    | 45.3     |
| ujson   | users.json | 5.923    | 5.936       | 6.044    | 49.4     |
| json    | users.json | 8.443    | 8.450       | 8.620    | 49.7     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.063    | 1000    | 48.8     |
|                                            | jmespath    | 0.365    | 1000    | 49.2     |
|                                            | jsonpath-ng | 3.503    | 1000    | 51.0     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.832    | 12046   | 51.0     |
|                                            | jmespath    | 9.816    | 1000    | 51.7     |
|                                            | jsonpath-ng | 29.383   | 12046   | 56.1     |
| $..price                                   | strata      | 1.208    | 12046   | 55.2     |
|                                            | jsonpath-ng | 115.927  | 12046   | 57.2     |
| $.users\[?(@.age>30)\]                     | strata      | 0.102    | 794     | 54.7     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.383    | 1604    | 54.7     |

## Summary

| Category         | Strata Rank           | vs #1                             |
| ---------------- | --------------------- | --------------------------------- |
| Parsing (JSON)   | **#1** / 5            | **14.9% faster** than #2 (orjson) |
| Parsing (NDJSON) | **#4** / 5            | 64.7% behind #1 (orjson)          |
| Serialization    | **#3** / 5            | 21.3% behind #1 (orjson)          |
| JSONPath         | **#1** in 5/5 queries | -                                 |
