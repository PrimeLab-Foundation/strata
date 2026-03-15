# Strata Benchmark Results

Generated: 2026-03-15 02:27:31

## Environment

- Commit: 5c264ca1e42ba846ee2425bce2f825e70fdfafb1
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/users.json, benchmarks/data/generated/users.ndjson

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 18.551   | 23.721      | 23.818   | 64.8     |
| orjson        | users.json   | 22.390   | 33.155      | 33.309   | 80.9     |
| msgspec       | users.json   | 23.693   | 36.085      | 36.659   | 79.6     |
| ujson         | users.json   | 31.948   | 48.074      | 48.100   | 93.6     |
| json (stdlib) | users.json   | 43.235   | 54.209      | 55.870   | 84.7     |
| strata        | users.ndjson | 17.365   | 22.522      | 23.264   | 220.9    |
| orjson        | users.ndjson | 28.865   | 29.048      | 30.493   | 227.4    |
| msgspec       | users.ndjson | 29.342   | 31.231      | 32.177   | 228.4    |
| ujson         | users.ndjson | 37.438   | 43.926      | 47.904   | 228.4    |
| json (stdlib) | users.ndjson | 52.155   | 54.882      | 55.046   | 229.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 17.954   | 23.249      | 23.722   | 86.2     |
| orjson        | users.json   | 21.649   | 32.911      | 33.709   | 86.2     |
| msgspec       | users.json   | 23.025   | 34.567      | 34.875   | 89.8     |
| ujson         | users.json   | 31.335   | 45.726      | 48.164   | 103.9    |
| json (stdlib) | users.json   | 42.115   | 53.410      | 54.648   | 89.8     |
| strata        | users.ndjson | 18.480   | 23.711      | 23.832   | 230.4    |
| orjson        | users.ndjson | 26.921   | 28.186      | 28.762   | 231.4    |
| msgspec       | users.ndjson | 30.025   | 31.024      | 31.770   | 233.9    |
| ujson         | users.ndjson | 37.892   | 43.967      | 44.027   | 233.9    |
| json (stdlib) | users.ndjson | 54.197   | 54.740      | 57.861   | 234.9    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 7.198    | 7.205       | 7.220    | 93.9     |
| msgspec | users.json | 7.657    | 7.674       | 7.758    | 131.4    |
| orjson  | users.json | 7.783    | 7.804       | 7.918    | 113.0    |
| ujson   | users.json | 31.504   | 31.527      | 31.669   | 136.2    |
| json    | users.json | 43.604   | 43.711      | 43.831   | 156.8    |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 7.973    | 7.998       | 8.169    | 161.8    |
| msgspec       | users.json | 8.395    | 8.501       | 8.641    | 194.8    |
| orjson        | users.json | 8.623    | 8.863       | 8.901    | 180.6    |
| ujson         | users.json | 32.289   | 32.745      | 33.308   | 181.7    |
| json (stdlib) | users.json | 327.083  | 336.342     | 354.568  | 194.8    |

## search (file-based)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 7.397    | 5000    | 194.9    |
|                 | orjson+jmespath    | 27.680   | 5000    | 195.1    |
|                 | orjson+jsonpath-ng | 34.720   | 5000    | 198.5    |
| all item prices | strata             | 8.037    | 62331   | 198.8    |
|                 | orjson+jmespath    | 68.093   | 5000    | 203.6    |
|                 | orjson+jsonpath-ng | 192.892  | 62331   | 210.5    |
| recursive price | strata             | 7.578    | 62331   | 210.9    |
|                 | orjson+jsonpath-ng | 659.787  | 62331   | 213.0    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.628    | 5000    | 229.1    |
|                                            | jmespath    | 2.045    | 5000    | 229.1    |
|                                            | jsonpath-ng | 20.006   | 5000    | 231.1    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 4.406    | 62331   | 230.7    |
|                                            | jmespath    | 51.138   | 5000    | 234.7    |
|                                            | jsonpath-ng | 190.645  | 62331   | 238.9    |
| $..price                                   | strata      | 6.289    | 62331   | 236.8    |
|                                            | jsonpath-ng | 681.065  | 62331   | 238.9    |
| $.users\[?(@.age>30)\]                     | strata      | 0.780    | 3999    | 235.9    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 7.380    | 8402    | 236.0    |

## Summary

| Category           | Strata Rank           | vs #1                             |
| ------------------ | --------------------- | --------------------------------- |
| loads (JSON)       | **#1** / 5            | **20.7% faster** than #2 (orjson) |
| loads (NDJSON)     | **#1** / 5            | **66.2% faster** than #2 (orjson) |
| load (JSON file)   | **#1** / 5            | **20.6% faster** than #2 (orjson) |
| load (NDJSON file) | **#1** / 5            | **45.7% faster** than #2 (orjson) |
| dumps              | **#1** / 5            | **6.4% faster** than #2 (msgspec) |
| dump               | **#1** / 5            | **5.3% faster** than #2 (msgspec) |
| search (JSONPath)  | **#1** in 3/3 queries | avg **3209% faster**              |
| query (JSONPath)   | **#1** in 5/5 queries | -                                 |
