# Strata Benchmark Results

Generated: 2026-03-14 22:25:30

## Environment

- Commit: f56e5ee86d30b2c22a38d81d24f698617174fe82
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 3.682    | 4.266       | 4.492    | 35.3     |
| msgspec       | users.json   | 4.426    | 5.917       | 6.077    | 42.5     |
| orjson        | users.json   | 4.236    | 5.951       | 6.196    | 39.4     |
| ujson         | users.json   | 5.660    | 7.688       | 8.184    | 41.4     |
| json (stdlib) | users.json   | 8.557    | 9.652       | 9.829    | 43.5     |
| msgspec       | users.ndjson | 4.734    | 6.007       | 6.380    | 64.8     |
| orjson        | users.ndjson | 4.415    | 6.049       | 6.254    | 63.8     |
| strata        | users.ndjson | 6.844    | 7.873       | 8.222    | 63.7     |
| ujson         | users.ndjson | 6.604    | 8.480       | 8.540    | 65.8     |
| json (stdlib) | users.ndjson | 9.103    | 10.485      | 11.789   | 64.8     |

## load (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| orjson        | users.json | 4.241    | 5.766       | 6.402    | 48.7     |
| msgspec       | users.json | 4.784    | 6.090       | 6.099    | 52.8     |
| ujson         | users.json | 6.531    | 8.029       | 9.102    | 52.8     |
| json (stdlib) | users.json | 8.605    | 9.863       | 10.570   | 52.8     |
| strata        | users.json | 9.454    | 10.509      | 11.041   | 48.7     |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| orjson  | users.json | 1.468    | 1.500       | 1.557    | 56.4     |
| msgspec | users.json | 1.517    | 1.530       | 1.575    | 58.2     |
| strata  | users.json | 1.715    | 1.742       | 1.784    | 53.6     |
| ujson   | users.json | 5.901    | 6.025       | 6.172    | 58.3     |
| json    | users.json | 8.366    | 8.375       | 8.650    | 58.5     |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| msgspec       | users.json | 1.697    | 1.744       | 1.752    | 58.6     |
| orjson        | users.json | 1.673    | 1.754       | 1.765    | 58.6     |
| strata        | users.json | 3.001    | 3.088       | 3.115    | 58.6     |
| ujson         | users.json | 6.328    | 6.597       | 6.638    | 58.6     |
| json (stdlib) | users.json | 65.669   | 66.926      | 67.659   | 58.6     |

## search (mem_eff=True)

| Query           | Library          | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------- | -------- | ------- | -------- |
| all user ids    | strata (mem_eff) | 5.451    | 1000    | 58.7     |
| all item prices | strata (mem_eff) | 5.656    | 12046   | 58.8     |
| recursive price | strata (mem_eff) | 5.849    | 12046   | 58.8     |

## search (mem_eff=False)

| Query           | Library | Min (ms) | Results | RSS (MB) |
| --------------- | ------- | -------- | ------- | -------- |
| all user ids    | strata  | 5.306    | 1000    | 58.7     |
| all item prices | strata  | 5.552    | 12046   | 58.8     |
| recursive price | strata  | 10.335   | 12046   | 58.8     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.072    | 1000    | 58.8     |
|                                            | jmespath    | 0.363    | 1000    | 58.9     |
|                                            | jsonpath-ng | 4.073    | 1000    | 59.4     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.866    | 12046   | 59.4     |
|                                            | jmespath    | 10.137   | 1000    | 59.4     |
|                                            | jsonpath-ng | 31.445   | 12046   | 63.2     |
| $..price                                   | strata      | 1.217    | 12046   | 61.2     |
|                                            | jsonpath-ng | 117.997  | 12046   | 63.3     |
| $.users\[?(@.age>30)\]                     | strata      | 0.103    | 794     | 62.6     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.397    | 1604    | 62.6     |

## Summary

| Category              | Strata Rank                   | vs #1                             |
| --------------------- | ----------------------------- | --------------------------------- |
| Parsing (JSON)        | **#1** / 5                    | **15.0% faster** than #2 (orjson) |
| Parsing (NDJSON)      | **#4** / 5                    | 55.0% behind #1 (orjson)          |
| Load (file)           | **#5** / 5                    | 122.9% behind #1 (orjson)         |
| Serialization (dumps) | **#3** / 5                    | 16.8% behind #1 (orjson)          |
| Dump (file)           | **#3** / 5                    | 79.3% behind #1 (orjson)          |
| Search (file)         | mem_eff faster in 1/3 queries | -                                 |
| JSONPath              | **#1** in 5/5 queries         | -                                 |
