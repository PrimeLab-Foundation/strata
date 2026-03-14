# Strata Benchmark Results

Generated: 2026-03-15 00:30:28

## Environment

- Commit: 2aa556684ac83988a6fdc0519b66ed3431409436
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.13.0

## Configuration

- Repeat: 7
- Warmup: 1
- Datasets: benchmarks/data/generated/users.json, benchmarks/data/generated/users.ndjson

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| pysimdjson    | users.json   | ERROR    | -           | -        | -        |
| strata        | users.json   | 19.272   | 24.839      | 27.199   | 67.9     |
| orjson        | users.json   | 24.888   | 28.598      | 37.894   | 83.9     |
| msgspec       | users.json   | 26.888   | 29.770      | 44.862   | 97.8     |
| ujson         | users.json   | 33.857   | 41.085      | 56.857   | 98.9     |
| json (stdlib) | users.json   | 39.706   | 46.024      | 49.122   | 103.0    |
| orjson        | users.ndjson | 22.990   | 27.932      | 29.402   | 165.3    |
| msgspec       | users.ndjson | 24.562   | 29.130      | 33.492   | 169.3    |
| strata        | users.ndjson | 30.107   | 34.626      | 38.183   | 165.3    |
| ujson         | users.ndjson | 33.140   | 41.325      | 43.225   | 169.3    |
| json (stdlib) | users.ndjson | 43.495   | 48.897      | 50.702   | 172.3    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 17.994   | 24.265      | 25.649   | 108.0    |
| orjson        | users.json   | 22.166   | 28.401      | 41.636   | 111.0    |
| msgspec       | users.json   | 24.854   | 30.374      | 40.175   | 119.0    |
| ujson         | users.json   | 32.209   | 41.696      | 42.885   | 119.0    |
| json (stdlib) | users.json   | 40.295   | 47.597      | 61.982   | 122.0    |
| strata        | users.ndjson | 19.065   | 24.150      | 26.595   | 172.3    |
| orjson        | users.ndjson | 23.220   | 28.782      | 31.410   | 172.3    |
| msgspec       | users.ndjson | 25.821   | 29.584      | 32.132   | 180.4    |
| ujson         | users.ndjson | 33.215   | 42.632      | 43.467   | 180.4    |
| json (stdlib) | users.ndjson | 42.472   | 49.367      | 50.240   | 180.4    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 7.393    | 7.449       | 8.492    | 126.5    |
| orjson  | users.json | 7.544    | 7.596       | 9.003    | 163.7    |
| msgspec | users.json | 7.990    | 8.035       | 9.824    | 200.2    |
| ujson   | users.json | 29.914   | 30.242      | 34.262   | 207.0    |
| json    | users.json | 41.880   | 41.930      | 44.860   | 247.6    |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 8.697    | 9.057       | 11.813   | 252.6    |
| orjson        | users.json | 9.225    | 9.378       | 11.876   | 288.9    |
| msgspec       | users.json | 9.679    | 9.870       | 12.396   | 320.1    |
| ujson         | users.json | 31.273   | 32.478      | 32.914   | 288.9    |
| json (stdlib) | users.json | 306.568  | 307.366     | 311.006  | 320.1    |

## search (mem_eff=True)

| Query           | Library                      | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------------------- | -------- | ------- | -------- |
| all user ids    | orjson+jmespath (mem_eff)    | 29.584   | 5000    | 142.8    |
|                 | orjson+jsonpath-ng (mem_eff) | 50.520   | 5000    | 143.1    |
|                 | strata (mem_eff)             | 53.419   | 5000    | 126.7    |
| all item prices | strata (mem_eff)             | 55.619   | 62331   | 143.9    |
|                 | orjson+jmespath (mem_eff)    | 78.658   | 5000    | 143.9    |
|                 | orjson+jsonpath-ng (mem_eff) | 254.107  | 62331   | 144.9    |
| recursive price | strata (mem_eff)             | 56.803   | 62331   | 147.1    |
|                 | orjson+jsonpath-ng (mem_eff) | 669.478  | 62331   | 148.6    |

## search (mem_eff=False)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | orjson+jmespath    | 29.584   | 5000    | 142.8    |
|                 | orjson+jsonpath-ng | 50.520   | 5000    | 143.1    |
|                 | strata             | 52.591   | 5000    | 126.7    |
| all item prices | strata             | 55.767   | 62331   | 143.3    |
|                 | orjson+jmespath    | 78.658   | 5000    | 143.9    |
|                 | orjson+jsonpath-ng | 254.107  | 62331   | 144.9    |
| recursive price | strata             | 77.631   | 62331   | 146.6    |
|                 | orjson+jsonpath-ng | 669.478  | 62331   | 148.6    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.737    | 5000    | 159.5    |
|                                            | jmespath    | 2.038    | 5000    | 159.5    |
|                                            | jsonpath-ng | 8.858    | 5000    | 161.7    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 4.117    | 62331   | 161.7    |
|                                            | jmespath    | 30.540   | 5000    | 165.7    |
|                                            | jsonpath-ng | 237.839  | 62331   | 168.1    |
| $..price                                   | strata      | 5.621    | 62331   | 166.7    |
|                                            | jsonpath-ng | 667.763  | 62331   | 168.1    |
| $.users\[?(@.age>30)\]                     | strata      | 0.798    | 3999    | 165.8    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 7.158    | 8402    | 165.8    |

## Summary

| Category              | Strata Rank                   | vs #1                             |
| --------------------- | ----------------------------- | --------------------------------- |
| Parsing (JSON)        | **#1** / 5                    | **29.1% faster** than #2 (orjson) |
| Parsing (NDJSON)      | **#3** / 5                    | 31.0% behind #1 (orjson)          |
| Load JSON (file)      | **#1** / 5                    | **23.2% faster** than #2 (orjson) |
| Load NDJSON (file)    | **#1** / 5                    | **21.8% faster** than #2 (orjson) |
| Serialization (dumps) | **#1** / 5                    | **2.0% faster** than #2 (orjson)  |
| Dump (file)           | **#1** / 5                    | **6.1% faster** than #2 (orjson)  |
| Search (file)         | mem_eff faster in 2/3 queries | -                                 |
| JSONPath              | **#1** in 5/5 queries         | -                                 |
