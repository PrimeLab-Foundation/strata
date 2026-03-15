# Strata Benchmark Results

Generated: 2026-03-15 03:10:20

## Environment

- Commit: 5c264ca1e42ba846ee2425bce2f825e70fdfafb1
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
| strata        | users.json   | 26.075   | 33.408      | 38.073   | 71.2     |
| msgspec       | users.json   | 31.755   | 46.255      | 65.238   | 92.0     |
| orjson        | users.json   | 30.181   | 49.552      | 50.188   | 91.1     |
| ujson         | users.json   | 43.880   | 56.856      | 78.902   | 109.6    |
| json (stdlib) | users.json   | 55.197   | 75.836      | 83.596   | 98.2     |
| strata        | users.ndjson | 23.224   | 28.627      | 29.415   | 189.0    |
| orjson        | users.ndjson | 35.107   | 35.757      | 36.997   | 198.0    |
| msgspec       | users.ndjson | 36.921   | 36.931      | 36.932   | 199.0    |
| ujson         | users.ndjson | 43.263   | 49.723      | 52.944   | 198.0    |
| json (stdlib) | users.ndjson | 61.620   | 72.875      | 74.377   | 200.0    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 25.991   | 29.927      | 30.816   | 97.6     |
| orjson        | users.json   | 35.198   | 39.151      | 45.210   | 97.6     |
| msgspec       | users.json   | 41.389   | 42.324      | 44.917   | 104.5    |
| ujson         | users.json   | 41.141   | 68.321      | 75.702   | 122.2    |
| json (stdlib) | users.json   | 57.133   | 70.532      | 89.061   | 104.5    |
| strata        | users.ndjson | 24.010   | 29.573      | 31.441   | 201.0    |
| msgspec       | users.ndjson | 36.026   | 39.775      | 40.522   | 204.8    |
| orjson        | users.ndjson | 36.296   | 41.312      | 43.438   | 202.0    |
| ujson         | users.ndjson | 47.845   | 53.463      | 65.409   | 203.8    |
| json (stdlib) | users.ndjson | 64.474   | 65.601      | 66.057   | 205.8    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 9.911    | 10.143      | 10.688   | 108.9    |
| msgspec | users.json | 10.483   | 10.678      | 12.873   | 161.2    |
| orjson  | users.json | 10.702   | 10.735      | 12.269   | 132.1    |
| ujson   | users.json | 40.525   | 40.976      | 42.061   | 167.2    |
| json    | users.json | 57.721   | 58.058      | 58.999   | 192.5    |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 10.653   | 10.711      | 12.655   | 198.8    |
| orjson        | users.json | 11.385   | 11.407      | 11.672   | 221.1    |
| msgspec       | users.json | 12.141   | 12.370      | 13.806   | 243.6    |
| ujson         | users.json | 42.950   | 44.396      | 44.549   | 221.2    |
| json (stdlib) | users.json | 433.299  | 433.722     | 433.859  | 243.6    |

## search (file-based)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 10.842   | 2000    | 240.6    |
|                 | orjson+jmespath    | 32.419   | 2000    | 243.8    |
|                 | orjson+jsonpath-ng | 33.463   | 2000    | 246.0    |
| all item prices | strata             | 11.916   | 100999  | 247.2    |
|                 | orjson+jmespath    | 81.187   | 2000    | 256.5    |
|                 | orjson+jsonpath-ng | 265.898  | 100999  | 263.5    |
| recursive price | strata             | 12.787   | 100999  | 261.5    |
|                 | orjson+jsonpath-ng | 881.713  | 100999  | 264.7    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.283    | 2000    | 286.9    |
|                                            | jmespath    | 0.963    | 2000    | 286.9    |
|                                            | jsonpath-ng | 15.640   | 2000    | 287.7    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.039    | 100999  | 287.7    |
|                                            | jmespath    | 61.746   | 2000    | 291.6    |
|                                            | jsonpath-ng | 239.742  | 100999  | 293.9    |
| $..price                                   | strata      | 8.174    | 100999  | 294.4    |
|                                            | jsonpath-ng | 875.475  | 100999  | 235.8    |
| $.users\[?(@.age>30)\]                     | strata      | 0.338    | 1606    | 213.9    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 8.606    | 6650    | 213.9    |

## Summary

| Category           | Strata Rank           | vs #1                              |
| ------------------ | --------------------- | ---------------------------------- |
| loads (JSON)       | **#1** / 5            | **15.7% faster** than #2 (orjson)  |
| loads (NDJSON)     | **#1** / 5            | **51.2% faster** than #2 (orjson)  |
| load (JSON file)   | **#1** / 5            | **35.4% faster** than #2 (orjson)  |
| load (NDJSON file) | **#1** / 5            | **50.0% faster** than #2 (msgspec) |
| dumps              | **#1** / 5            | **5.8% faster** than #2 (msgspec)  |
| dump               | **#1** / 5            | **6.9% faster** than #2 (orjson)   |
| search (JSONPath)  | **#1** in 3/3 queries | avg **2525% faster**               |
| query (JSONPath)   | **#1** in 5/5 queries | -                                  |
