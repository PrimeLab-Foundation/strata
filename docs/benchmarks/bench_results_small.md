# Strata Benchmark Results

Generated: 2026-03-14 19:40:40

## Environment

- Commit: b1b82c0eadfc3d6c934130915684a2e84fa5df5e
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
| strata        | users.json   | 4.189    | 5.564       | 5.786    | 35.1     |
| msgspec       | users.json   | 4.659    | 6.955       | 7.201    | 41.5     |
| orjson        | users.json   | 6.879    | 7.208       | 7.401    | 38.3     |
| ujson         | users.json   | 5.942    | 9.501       | 9.635    | 40.4     |
| json (stdlib) | users.json   | 8.919    | 10.825      | 14.371   | 44.1     |
| strata        | users.ndjson | 4.031    | 5.005       | 5.149    | 53.9     |
| orjson        | users.ndjson | 4.381    | 6.109       | 6.203    | 54.9     |
| msgspec       | users.ndjson | 5.791    | 7.021       | 7.713    | 55.9     |
| ujson         | users.ndjson | 6.305    | 9.396       | 9.989    | 55.9     |
| json (stdlib) | users.ndjson | 10.068   | 11.544      | 11.777   | 55.9     |

## Serialization Benchmarks

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| msgspec | users.json | 1.464    | 1.481       | 1.554    | 48.2     |
| orjson  | users.json | 1.551    | 1.553       | 1.556    | 47.7     |
| strata  | users.json | 1.837    | 1.865       | 1.894    | 44.9     |
| ujson   | users.json | 6.271    | 6.304       | 6.370    | 48.7     |
| json    | users.json | 8.757    | 8.779       | 8.815    | 48.9     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.088    | 1000    | 48.1     |
|                                            | jmespath    | 0.427    | 1000    | 48.5     |
|                                            | jsonpath-ng | 4.409    | 1000    | 50.2     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.900    | 12046   | 50.2     |
|                                            | jmespath    | 11.337   | 1000    | 51.0     |
|                                            | jsonpath-ng | 32.382   | 12046   | 55.3     |
| $..price                                   | strata      | 1.247    | 12046   | 53.3     |
|                                            | jsonpath-ng | 125.949  | 12046   | 56.3     |
| $.users\[?(@.age>30)\]                     | strata      | 0.121    | 794     | 53.8     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.421    | 1604    | 53.8     |

## Summary

| Category         | Strata Rank           | vs #1                              |
| ---------------- | --------------------- | ---------------------------------- |
| Parsing (JSON)   | **#1** / 5            | **11.2% faster** than #2 (msgspec) |
| Parsing (NDJSON) | **#1** / 5            | **8.7% faster** than #2 (orjson)   |
| Serialization    | **#3** / 5            | 25.4% behind #1 (msgspec)          |
| JSONPath         | **#1** in 5/5 queries | -                                  |
