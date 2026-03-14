# Strata Benchmark Results

Generated: 2026-03-14 19:08:00

## Environment

- Commit: b1b82c0eadfc3d6c934130915684a2e84fa5df5e
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
| strata        | users.json   | 23.522   | 29.831      | 29.833   | 73.4     |
| orjson        | users.json   | 28.527   | 38.749      | 39.569   | 93.3     |
| msgspec       | users.json   | 29.004   | 38.982      | 42.387   | 92.1     |
| ujson         | users.json   | 39.168   | 56.126      | 57.701   | 110.8    |
| json (stdlib) | users.json   | 51.814   | 66.733      | 68.161   | 98.4     |
| strata        | users.ndjson | 22.154   | 28.249      | 29.147   | 200.5    |
| orjson        | users.ndjson | 31.180   | 33.654      | 35.092   | 207.6    |
| msgspec       | users.ndjson | 32.266   | 34.739      | 36.250   | 210.7    |
| ujson         | users.ndjson | 42.824   | 48.543      | 51.182   | 226.1    |
| json (stdlib) | users.ndjson | 58.801   | 62.253      | 62.710   | 211.7    |

## Serialization Benchmarks

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| msgspec | users.json | 10.042   | 10.103      | 10.396   | 154.2    |
| orjson  | users.json | 10.334   | 10.351      | 10.388   | 125.9    |
| strata  | users.json | 11.987   | 12.009      | 12.074   | 102.5    |
| ujson   | users.json | 39.038   | 39.117      | 40.212   | 160.1    |
| json    | users.json | 55.888   | 55.936      | 56.252   | 186.0    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.261    | 2000    | 229.7    |
|                                            | jmespath    | 0.810    | 2000    | 230.0    |
|                                            | jsonpath-ng | 14.748   | 2000    | 232.3    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 5.704    | 100999  | 234.0    |
|                                            | jmespath    | 57.498   | 2000    | 238.0    |
|                                            | jsonpath-ng | 229.982  | 100999  | 241.0    |
| $..price                                   | strata      | 7.656    | 100999  | 242.3    |
|                                            | jsonpath-ng | 820.601  | 100999  | 244.8    |
| $.users\[?(@.age>30)\]                     | strata      | 0.303    | 1606    | 243.6    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 8.240    | 6650    | 242.6    |

## Summary

| Category         | Strata Rank           | vs #1                             |
| ---------------- | --------------------- | --------------------------------- |
| Parsing (JSON)   | **#1** / 5            | **21.3% faster** than #2 (orjson) |
| Parsing (NDJSON) | **#1** / 5            | **40.7% faster** than #2 (orjson) |
| Serialization    | **#3** / 5            | 19.4% behind #1 (msgspec)         |
| JSONPath         | **#1** in 5/5 queries | -                                 |
