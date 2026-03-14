# Strata Benchmark Results

Generated: 2026-03-14 22:25:46

## Environment

- Commit: f56e5ee86d30b2c22a38d81d24f698617174fe82
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/medium/users.json, benchmarks/data/generated/medium/users.ndjson

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 22.352   | 28.024      | 28.707   | 71.3     |
| orjson        | users.json   | 26.556   | 37.641      | 38.489   | 91.2     |
| msgspec       | users.json   | 29.860   | 40.065      | 41.348   | 92.0     |
| ujson         | users.json   | 38.858   | 55.401      | 55.927   | 109.7    |
| json (stdlib) | users.json   | 52.477   | 67.460      | 68.265   | 98.3     |
| orjson        | users.ndjson | 30.345   | 33.111      | 34.253   | 299.7    |
| msgspec       | users.ndjson | 31.632   | 34.168      | 35.281   | 302.7    |
| strata        | users.ndjson | 40.200   | 40.393      | 44.036   | 298.6    |
| ujson         | users.ndjson | 42.490   | 47.839      | 50.254   | 305.2    |
| json (stdlib) | users.ndjson | 58.377   | 65.237      | 65.261   | 303.7    |

## load (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| orjson        | users.json | 27.558   | 37.641      | 39.215   | 124.3    |
| msgspec       | users.json | 28.257   | 40.519      | 42.183   | 131.2    |
| ujson         | users.json | 38.783   | 55.687      | 58.261   | 149.0    |
| json (stdlib) | users.json | 52.199   | 68.318      | 69.908   | 131.3    |
| strata        | users.json | 61.689   | 73.688      | 75.301   | 101.7    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| msgspec | users.json | 10.254   | 10.288      | 10.352   | 186.3    |
| orjson  | users.json | 10.444   | 10.484      | 10.672   | 157.9    |
| strata  | users.json | 12.200   | 12.214      | 12.238   | 135.6    |
| ujson   | users.json | 39.670   | 39.674      | 39.766   | 192.1    |
| json    | users.json | 56.412   | 56.435      | 56.555   | 217.4    |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| msgspec       | users.json | 10.952   | 11.232      | 11.268   | 270.2    |
| orjson        | users.json | 11.270   | 11.323      | 11.459   | 246.0    |
| strata        | users.json | 18.460   | 18.548      | 18.592   | 223.7    |
| ujson         | users.json | 40.398   | 40.417      | 40.556   | 246.1    |
| json (stdlib) | users.json | 408.231  | 414.650     | 417.475  | 270.3    |

## search (mem_eff=True)

| Query           | Library          | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------- | -------- | ------- | -------- |
| all user ids    | strata (mem_eff) | 34.172   | 2000    | 267.3    |
| all item prices | strata (mem_eff) | 36.321   | 100999  | 271.0    |
| recursive price | strata (mem_eff) | 37.049   | 100999  | 281.3    |

## search (mem_eff=False)

| Query           | Library | Min (ms) | Results | RSS (MB) |
| --------------- | ------- | -------- | ------- | -------- |
| all user ids    | strata  | 34.622   | 2000    | 267.3    |
| all item prices | strata  | 36.384   | 100999  | 268.6    |
| recursive price | strata  | 64.698   | 100999  | 284.2    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.299    | 2000    | 312.6    |
|                                            | jmespath    | 0.914    | 2000    | 313.1    |
|                                            | jsonpath-ng | 15.169   | 2000    | 315.4    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 5.772    | 100999  | 315.4    |
|                                            | jmespath    | 59.696   | 2000    | 319.2    |
|                                            | jsonpath-ng | 228.871  | 100999  | 321.7    |
| $..price                                   | strata      | 7.880    | 100999  | 322.5    |
|                                            | jsonpath-ng | 825.923  | 100999  | 324.9    |
| $.users\[?(@.age>30)\]                     | strata      | 0.307    | 1606    | 322.6    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 8.568    | 6650    | 322.6    |

## Summary

| Category              | Strata Rank                   | vs #1                             |
| --------------------- | ----------------------------- | --------------------------------- |
| Parsing (JSON)        | **#1** / 5                    | **18.8% faster** than #2 (orjson) |
| Parsing (NDJSON)      | **#3** / 5                    | 32.5% behind #1 (orjson)          |
| Load (file)           | **#5** / 5                    | 123.9% behind #1 (orjson)         |
| Serialization (dumps) | **#3** / 5                    | 19.0% behind #1 (msgspec)         |
| Dump (file)           | **#3** / 5                    | 68.6% behind #1 (msgspec)         |
| Search (file)         | mem_eff faster in 3/3 queries | -                                 |
| JSONPath              | **#1** in 5/5 queries         | -                                 |
