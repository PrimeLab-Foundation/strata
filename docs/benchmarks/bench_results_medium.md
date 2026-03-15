# Strata Benchmark Results

Generated: 2026-03-15 02:25:39

## Environment

- Commit: 1ad8828b764067899097f4fab331b0010dccd034
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
| strata        | users.json   | 22.861   | 28.743      | 29.498   | 71.5     |
| orjson        | users.json   | 27.342   | 40.488      | 40.721   | 91.4     |
| msgspec       | users.json   | 29.870   | 42.026      | 43.520   | 92.2     |
| ujson         | users.json   | 39.921   | 58.088      | 58.715   | 109.8    |
| json (stdlib) | users.json   | 54.837   | 67.992      | 69.925   | 98.5     |
| strata        | users.ndjson | 21.408   | 27.248      | 28.005   | 273.0    |
| orjson        | users.ndjson | 30.525   | 34.481      | 34.512   | 262.2    |
| msgspec       | users.ndjson | 33.520   | 36.079      | 36.859   | 263.3    |
| ujson         | users.ndjson | 43.124   | 50.720      | 51.142   | 262.3    |
| json (stdlib) | users.ndjson | 60.356   | 64.140      | 65.740   | 264.3    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 22.927   | 29.095      | 30.161   | 97.8     |
| orjson        | users.json   | 26.808   | 37.530      | 39.126   | 97.8     |
| msgspec       | users.json   | 26.841   | 40.846      | 41.060   | 104.8    |
| ujson         | users.json   | 38.213   | 55.107      | 56.426   | 122.5    |
| json (stdlib) | users.json   | 52.637   | 68.124      | 68.152   | 104.8    |
| strata        | users.ndjson | 22.799   | 28.698      | 29.050   | 265.4    |
| orjson        | users.ndjson | 32.294   | 34.441      | 35.481   | 266.4    |
| msgspec       | users.ndjson | 34.003   | 35.086      | 35.880   | 268.4    |
| ujson         | users.ndjson | 45.012   | 52.020      | 52.729   | 267.4    |
| json (stdlib) | users.ndjson | 58.349   | 61.358      | 62.438   | 269.4    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 9.803    | 9.809       | 9.839    | 108.9    |
| msgspec | users.json | 10.379   | 10.396      | 10.761   | 160.9    |
| orjson  | users.json | 10.729   | 10.737      | 10.821   | 131.8    |
| ujson   | users.json | 39.807   | 39.848      | 39.983   | 166.8    |
| json    | users.json | 57.240   | 57.256      | 57.336   | 192.1    |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 10.390   | 10.451      | 10.567   | 198.4    |
| msgspec       | users.json | 11.209   | 11.277      | 11.303   | 243.5    |
| orjson        | users.json | 11.425   | 11.645      | 11.928   | 220.7    |
| ujson         | users.json | 40.998   | 41.173      | 41.380   | 221.1    |
| json (stdlib) | users.json | 417.852  | 420.859     | 420.989  | 243.5    |

## search (file-based)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 8.939    | 2000    | 241.5    |
|                 | orjson+jmespath    | 30.809   | 2000    | 243.7    |
|                 | orjson+jsonpath-ng | 31.854   | 2000    | 245.9    |
| all item prices | strata             | 9.730    | 100999  | 247.0    |
|                 | orjson+jmespath    | 75.765   | 2000    | 256.2    |
|                 | orjson+jsonpath-ng | 258.878  | 100999  | 263.4    |
| recursive price | strata             | 10.304   | 100999  | 260.3    |
|                 | orjson+jsonpath-ng | 843.585  | 100999  | 264.3    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.300    | 2000    | 287.2    |
|                                            | jmespath    | 0.921    | 2000    | 287.2    |
|                                            | jsonpath-ng | 14.847   | 2000    | 288.0    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 5.848    | 100999  | 288.4    |
|                                            | jmespath    | 60.125   | 2000    | 292.2    |
|                                            | jsonpath-ng | 239.105  | 100999  | 295.4    |
| $..price                                   | strata      | 7.726    | 100999  | 295.8    |
|                                            | jsonpath-ng | 833.441  | 100999  | 298.2    |
| $.users\[?(@.age>30)\]                     | strata      | 0.331    | 1606    | 295.9    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 8.396    | 6650    | 295.9    |

## Summary

| Category           | Strata Rank           | vs #1                             |
| ------------------ | --------------------- | --------------------------------- |
| loads (JSON)       | **#1** / 5            | **19.6% faster** than #2 (orjson) |
| loads (NDJSON)     | **#1** / 5            | **42.6% faster** than #2 (orjson) |
| load (JSON file)   | **#1** / 5            | **16.9% faster** than #2 (orjson) |
| load (NDJSON file) | **#1** / 5            | **41.6% faster** than #2 (orjson) |
| dumps              | **#1** / 5            | **5.9% faster** than #2 (msgspec) |
| dump               | **#1** / 5            | **7.9% faster** than #2 (msgspec) |
| search (JSONPath)  | **#1** in 3/3 queries | avg **3004% faster**              |
| query (JSONPath)   | **#1** in 5/5 queries | -                                 |
