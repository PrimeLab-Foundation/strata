# Strata Benchmark Results

Generated: 2026-03-14 22:14:51

## Environment

- Commit: a41eea07ede3c1eca4ad03b9ad1a728387619922
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
| strata        | users.json   | 22.895   | 29.219      | 29.394   | 73.5     |
| orjson        | users.json   | 26.657   | 37.054      | 38.531   | 93.4     |
| msgspec       | users.json   | 26.934   | 39.758      | 40.348   | 92.2     |
| ujson         | users.json   | 38.422   | 54.469      | 61.255   | 110.8    |
| json (stdlib) | users.json   | 53.280   | 64.229      | 66.884   | 98.5     |
| orjson        | users.ndjson | 30.839   | 35.458      | 35.604   | 208.4    |
| msgspec       | users.ndjson | 32.383   | 35.824      | 36.353   | 211.6    |
| strata        | users.ndjson | 39.018   | 40.777      | 41.321   | 208.4    |
| ujson         | users.ndjson | 43.286   | 48.333      | 50.556   | 227.0    |
| json (stdlib) | users.ndjson | 58.890   | 61.101      | 63.396   | 212.6    |

## Serialization Benchmarks

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| msgspec | users.json | 10.096   | 10.118      | 10.296   | 154.5    |
| orjson  | users.json | 10.349   | 10.369      | 10.952   | 125.2    |
| strata  | users.json | 11.818   | 11.836      | 11.893   | 102.8    |
| ujson   | users.json | 39.036   | 39.318      | 39.686   | 160.3    |
| json    | users.json | 56.288   | 56.540      | 57.218   | 185.6    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.285    | 2000    | 229.5    |
|                                            | jmespath    | 0.851    | 2000    | 229.8    |
|                                            | jsonpath-ng | 15.173   | 2000    | 232.1    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 5.866    | 100999  | 233.3    |
|                                            | jmespath    | 58.002   | 2000    | 237.4    |
|                                            | jsonpath-ng | 232.849  | 100999  | 241.4    |
| $..price                                   | strata      | 7.801    | 100999  | 242.0    |
|                                            | jsonpath-ng | 811.743  | 100999  | 245.6    |
| $.users\[?(@.age>30)\]                     | strata      | 0.307    | 1606    | 244.5    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 8.364    | 6650    | 243.5    |

## Summary

| Category         | Strata Rank           | vs #1                             |
| ---------------- | --------------------- | --------------------------------- |
| Parsing (JSON)   | **#1** / 5            | **16.4% faster** than #2 (orjson) |
| Parsing (NDJSON) | **#3** / 5            | 26.5% behind #1 (orjson)          |
| Serialization    | **#3** / 5            | 17.1% behind #1 (msgspec)         |
| JSONPath         | **#1** in 5/5 queries | -                                 |
