# Strata Benchmark Results

Generated: 2026-03-15 00:49:27

## Environment

- Commit: 2aa556684ac83988a6fdc0519b66ed3431409436
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
| strata        | users.json   | 24.745   | 32.443      | 32.609   | 71.6     |
| orjson        | users.json   | 30.263   | 40.555      | 41.173   | 91.5     |
| msgspec       | users.json   | 30.103   | 46.045      | 55.253   | 92.3     |
| ujson         | users.json   | 50.719   | 59.795      | 88.985   | 110.0    |
| json (stdlib) | users.json   | 56.846   | 71.193      | 85.569   | 98.6     |
| msgspec       | users.ndjson | 34.065   | 35.690      | 40.345   | 190.4    |
| orjson        | users.ndjson | 34.130   | 36.743      | 39.726   | 189.4    |
| strata        | users.ndjson | 41.371   | 44.042      | 45.136   | 188.4    |
| ujson         | users.ndjson | 44.438   | 55.786      | 57.662   | 189.4    |
| json (stdlib) | users.ndjson | 65.631   | 66.695      | 67.333   | 191.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 24.897   | 31.804      | 31.909   | 98.0     |
| orjson        | users.json   | 27.639   | 38.636      | 49.266   | 98.0     |
| msgspec       | users.json   | 29.329   | 43.161      | 47.004   | 104.9    |
| ujson         | users.json   | 45.770   | 62.347      | 71.054   | 122.6    |
| json (stdlib) | users.json   | 59.193   | 69.059      | 69.372   | 104.9    |
| strata        | users.ndjson | 24.199   | 29.902      | 30.519   | 192.4    |
| orjson        | users.ndjson | 35.331   | 35.479      | 38.295   | 193.4    |
| msgspec       | users.ndjson | 37.017   | 37.696      | 41.962   | 201.7    |
| ujson         | users.ndjson | 50.611   | 54.734      | 55.900   | 200.7    |
| json (stdlib) | users.ndjson | 62.646   | 65.051      | 65.632   | 202.7    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 9.710    | 9.905       | 10.475   | 109.2    |
| orjson  | users.json | 10.537   | 10.558      | 10.598   | 131.6    |
| msgspec | users.json | 10.556   | 10.605      | 11.936   | 159.9    |
| ujson   | users.json | 39.986   | 40.065      | 40.250   | 165.7    |
| json    | users.json | 57.191   | 57.262      | 57.545   | 191.0    |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 10.431   | 10.597      | 11.538   | 197.3    |
| orjson        | users.json | 11.349   | 11.404      | 11.411   | 219.6    |
| msgspec       | users.json | 11.241   | 11.595      | 12.192   | 241.9    |
| ujson         | users.json | 41.219   | 41.264      | 41.727   | 219.6    |
| json (stdlib) | users.json | 425.525  | 427.825     | 431.090  | 242.1    |

## search (mem_eff=True)

| Query           | Library                      | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------------------- | -------- | ------- | -------- |
| all user ids    | orjson+jmespath (mem_eff)    | 30.950   | 2000    | 143.8    |
|                 | orjson+jsonpath-ng (mem_eff) | 33.013   | 2000    | 145.9    |
|                 | strata (mem_eff)             | 66.013   | 2000    | 123.3    |
| all item prices | strata (mem_eff)             | 70.566   | 100999  | 152.3    |
|                 | orjson+jmespath (mem_eff)    | 82.642   | 2000    | 159.5    |
|                 | orjson+jsonpath-ng (mem_eff) | 265.326  | 100999  | 166.0    |
| recursive price | strata (mem_eff)             | 71.836   | 100999  | 164.7    |
|                 | orjson+jsonpath-ng (mem_eff) | 880.108  | 100999  | 168.0    |

## search (mem_eff=False)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 30.082   | 2000    | 242.2    |
|                 | orjson+jmespath    | 30.950   | 2000    | 143.8    |
|                 | orjson+jsonpath-ng | 33.013   | 2000    | 145.9    |
| all item prices | strata             | 38.612   | 100999  | 150.3    |
|                 | orjson+jmespath    | 82.642   | 2000    | 159.5    |
|                 | orjson+jsonpath-ng | 265.326  | 100999  | 166.0    |
| recursive price | strata             | 40.363   | 100999  | 164.7    |
|                 | orjson+jsonpath-ng | 880.108  | 100999  | 168.0    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.308    | 2000    | 196.5    |
|                                            | jmespath    | 0.887    | 2000    | 196.5    |
|                                            | jsonpath-ng | 15.826   | 2000    | 197.3    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.499    | 100999  | 197.3    |
|                                            | jmespath    | 61.350   | 2000    | 201.1    |
|                                            | jsonpath-ng | 265.082  | 100999  | 203.5    |
| $..price                                   | strata      | 8.406    | 100999  | 203.3    |
|                                            | jsonpath-ng | 884.703  | 100999  | 205.7    |
| $.users\[?(@.age>30)\]                     | strata      | 0.395    | 1606    | 203.4    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.364    | 6650    | 202.4    |

## Summary

| Category              | Strata Rank                   | vs #1                              |
| --------------------- | ----------------------------- | ---------------------------------- |
| Parsing (JSON)        | **#1** / 5                    | **21.7% faster** than #2 (msgspec) |
| Parsing (NDJSON)      | **#3** / 5                    | 21.4% behind #1 (msgspec)          |
| Load JSON (file)      | **#1** / 5                    | **11.0% faster** than #2 (orjson)  |
| Load NDJSON (file)    | **#1** / 5                    | **46.0% faster** than #2 (orjson)  |
| Serialization (dumps) | **#1** / 5                    | **8.5% faster** than #2 (orjson)   |
| Dump (file)           | **#1** / 5                    | **7.8% faster** than #2 (msgspec)  |
| Search (file)         | mem_eff faster in 0/3 queries | -                                  |
| JSONPath              | **#1** in 5/5 queries         | -                                  |
