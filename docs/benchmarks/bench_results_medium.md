# Strata Benchmark Results

Generated: 2026-03-15 01:22:57

## Environment

- Commit: ad401a8927f1d1bf16672f43d7402ae108e7d54a
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
| strata        | users.json   | 24.607   | 30.667      | 35.075   | 71.3     |
| orjson        | users.json   | 28.677   | 41.600      | 53.436   | 91.2     |
| msgspec       | users.json   | 32.156   | 42.502      | 43.580   | 92.0     |
| ujson         | users.json   | 40.291   | 58.797      | 62.207   | 109.6    |
| json (stdlib) | users.json   | 55.633   | 68.611      | 78.608   | 98.3     |
| strata        | users.ndjson | 22.050   | 28.721      | 30.896   | 179.1    |
| orjson        | users.ndjson | 32.505   | 37.374      | 46.421   | 162.1    |
| msgspec       | users.ndjson | 35.220   | 38.034      | 39.316   | 163.2    |
| ujson         | users.ndjson | 42.649   | 53.941      | 54.534   | 162.2    |
| json (stdlib) | users.ndjson | 62.787   | 64.177      | 64.678   | 164.2    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 23.833   | 30.969      | 33.864   | 97.7     |
| orjson        | users.json   | 26.248   | 41.976      | 53.136   | 97.7     |
| msgspec       | users.json   | 32.383   | 43.270      | 43.469   | 104.6    |
| ujson         | users.json   | 39.944   | 58.382      | 63.778   | 122.3    |
| json (stdlib) | users.json   | 56.009   | 70.158      | 91.495   | 104.6    |
| strata        | users.ndjson | 23.426   | 29.304      | 29.538   | 165.2    |
| orjson        | users.ndjson | 34.446   | 35.347      | 35.791   | 172.5    |
| msgspec       | users.ndjson | 34.428   | 37.527      | 47.305   | 180.8    |
| ujson         | users.ndjson | 48.062   | 53.213      | 60.224   | 179.8    |
| json (stdlib) | users.ndjson | 60.199   | 62.189      | 74.065   | 181.8    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 10.332   | 10.495      | 11.644   | 108.8    |
| orjson  | users.json | 10.837   | 10.885      | 11.351   | 131.9    |
| msgspec | users.json | 10.679   | 10.954      | 12.299   | 161.0    |
| ujson   | users.json | 41.221   | 41.522      | 42.594   | 167.1    |
| json    | users.json | 57.867   | 58.073      | 58.580   | 192.4    |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 10.711   | 10.734      | 12.127   | 198.7    |
| msgspec       | users.json | 11.512   | 11.698      | 12.809   | 243.3    |
| orjson        | users.json | 11.846   | 13.177      | 14.314   | 221.0    |
| ujson         | users.json | 41.827   | 42.123      | 43.289   | 221.0    |
| json (stdlib) | users.json | 434.333  | 434.629     | 437.703  | 243.4    |

## search (mem_eff=True)

| Query           | Library                      | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------------------- | -------- | ------- | -------- |
| all user ids    | orjson+jmespath (mem_eff)    | 32.594   | 2000    | 145.0    |
|                 | orjson+jsonpath-ng (mem_eff) | 35.145   | 2000    | 147.1    |
|                 | strata (mem_eff)             | 63.917   | 2000    | 124.5    |
| all item prices | strata (mem_eff)             | 67.430   | 100999  | 151.5    |
|                 | orjson+jmespath (mem_eff)    | 83.167   | 2000    | 158.7    |
|                 | orjson+jsonpath-ng (mem_eff) | 264.376  | 100999  | 165.0    |
| recursive price | strata (mem_eff)             | 69.287   | 100999  | 164.3    |
|                 | orjson+jsonpath-ng (mem_eff) | 890.542  | 100999  | 166.9    |

## search (mem_eff=False)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 29.391   | 2000    | 243.5    |
|                 | orjson+jmespath    | 32.594   | 2000    | 145.0    |
|                 | orjson+jsonpath-ng | 35.145   | 2000    | 147.1    |
| all item prices | strata             | 37.943   | 100999  | 150.4    |
|                 | orjson+jmespath    | 83.167   | 2000    | 158.7    |
|                 | orjson+jsonpath-ng | 264.376  | 100999  | 165.0    |
| recursive price | strata             | 40.703   | 100999  | 164.3    |
|                 | orjson+jsonpath-ng | 890.542  | 100999  | 166.9    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.295    | 2000    | 195.4    |
|                                            | jmespath    | 1.032    | 2000    | 195.4    |
|                                            | jsonpath-ng | 16.236   | 2000    | 196.2    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.164    | 100999  | 196.2    |
|                                            | jmespath    | 61.564   | 2000    | 200.0    |
|                                            | jsonpath-ng | 251.585  | 100999  | 202.3    |
| $..price                                   | strata      | 8.349    | 100999  | 202.0    |
|                                            | jsonpath-ng | 870.681  | 100999  | 204.4    |
| $.users\[?(@.age>30)\]                     | strata      | 0.354    | 1606    | 203.0    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.465    | 6650    | 203.0    |

## Summary

| Category              | Strata Rank                   | vs #1                              |
| --------------------- | ----------------------------- | ---------------------------------- |
| Parsing (JSON)        | **#1** / 5                    | **16.5% faster** than #2 (orjson)  |
| Parsing (NDJSON)      | **#1** / 5                    | **47.4% faster** than #2 (orjson)  |
| Load JSON (file)      | **#1** / 5                    | **10.1% faster** than #2 (orjson)  |
| Load NDJSON (file)    | **#1** / 5                    | **47.0% faster** than #2 (msgspec) |
| Serialization (dumps) | **#1** / 5                    | **3.4% faster** than #2 (msgspec)  |
| Dump (file)           | **#1** / 5                    | **7.5% faster** than #2 (msgspec)  |
| Search (file)         | mem_eff faster in 0/3 queries | -                                  |
| JSONPath              | **#1** in 5/5 queries         | -                                  |
