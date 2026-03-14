# Strata Benchmark Results

Generated: 2026-03-15 00:49:02

## Environment

- Commit: 2aa556684ac83988a6fdc0519b66ed3431409436
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
| strata        | users.json   | 3.653    | 4.953       | 5.098    | 34.7     |
| orjson        | users.json   | 4.284    | 5.825       | 6.151    | 38.7     |
| msgspec       | users.json   | 6.925    | 7.059       | 11.304   | 41.8     |
| ujson         | users.json   | 8.885    | 9.270       | 12.197   | 40.8     |
| json (stdlib) | users.json   | 9.189    | 10.191      | 10.733   | 43.6     |
| orjson        | users.ndjson | 4.934    | 6.148       | 6.873    | 63.0     |
| msgspec       | users.ndjson | 4.670    | 6.349       | 6.917    | 63.1     |
| ujson         | users.ndjson | 6.116    | 8.933       | 9.607    | 63.1     |
| strata        | users.ndjson | 8.545    | 10.511      | 11.407   | 63.0     |
| json (stdlib) | users.ndjson | 9.514    | 12.889      | 15.228   | 63.1     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 3.535    | 4.436       | 4.544    | 43.6     |
| orjson        | users.json   | 5.216    | 6.371       | 7.651    | 43.6     |
| msgspec       | users.json   | 6.823    | 8.242       | 8.631    | 47.8     |
| ujson         | users.json   | 8.982    | 9.739       | 12.312   | 47.8     |
| json (stdlib) | users.json   | 8.640    | 11.191      | 11.248   | 47.8     |
| strata        | users.ndjson | 3.660    | 4.637       | 5.335    | 63.1     |
| msgspec       | users.ndjson | 4.824    | 6.370       | 7.208    | 64.1     |
| orjson        | users.ndjson | 5.048    | 7.400       | 9.939    | 63.1     |
| ujson         | users.ndjson | 7.043    | 8.748       | 9.719    | 64.1     |
| json (stdlib) | users.ndjson | 10.115   | 11.305      | 11.844   | 64.1     |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 1.487    | 1.520       | 1.554    | 47.8     |
| msgspec | users.json | 1.483    | 1.547       | 2.024    | 52.3     |
| orjson  | users.json | 1.564    | 1.568       | 1.661    | 50.3     |
| ujson   | users.json | 6.810    | 6.857       | 6.976    | 53.0     |
| json    | users.json | 8.577    | 8.611       | 8.931    | 53.3     |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| orjson        | users.json | 1.710    | 1.725       | 1.953    | 53.4     |
| msgspec       | users.json | 1.694    | 1.733       | 1.735    | 53.4     |
| strata        | users.json | 1.726    | 1.918       | 2.112    | 53.4     |
| ujson         | users.json | 6.692    | 6.745       | 6.831    | 53.4     |
| json (stdlib) | users.json | 66.275   | 67.611      | 69.014   | 53.4     |

## search (mem_eff=True)

| Query           | Library                      | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------------------- | -------- | ------- | -------- |
| all user ids    | orjson+jmespath (mem_eff)    | 5.476    | 1000    | 58.9     |
|                 | orjson+jsonpath-ng (mem_eff) | 6.528    | 1000    | 59.5     |
|                 | strata (mem_eff)             | 10.460   | 1000    | 58.7     |
| all item prices | strata (mem_eff)             | 10.722   | 12046   | 59.6     |
|                 | orjson+jmespath (mem_eff)    | 14.204   | 1000    | 59.6     |
|                 | orjson+jsonpath-ng (mem_eff) | 37.676   | 12046   | 59.5     |
| recursive price | strata (mem_eff)             | 11.233   | 12046   | 58.8     |
|                 | orjson+jsonpath-ng (mem_eff) | 130.205  | 12046   | 60.0     |

## search (mem_eff=False)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 5.156    | 1000    | 53.4     |
|                 | orjson+jmespath    | 5.476    | 1000    | 58.9     |
|                 | orjson+jsonpath-ng | 6.528    | 1000    | 59.5     |
| all item prices | strata             | 6.330    | 12046   | 59.6     |
|                 | orjson+jmespath    | 14.204   | 1000    | 59.6     |
|                 | orjson+jsonpath-ng | 37.676   | 12046   | 59.5     |
| recursive price | strata             | 6.961    | 12046   | 58.8     |
|                 | orjson+jsonpath-ng | 130.205  | 12046   | 60.0     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.074    | 1000    | 59.9     |
|                                            | jmespath    | 0.375    | 1000    | 59.9     |
|                                            | jsonpath-ng | 4.087    | 1000    | 59.9     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.887    | 12046   | 59.9     |
|                                            | jmespath    | 10.632   | 1000    | 59.9     |
|                                            | jsonpath-ng | 32.778   | 12046   | 63.6     |
| $..price                                   | strata      | 1.269    | 12046   | 61.6     |
|                                            | jsonpath-ng | 126.245  | 12046   | 63.6     |
| $.users\[?(@.age>30)\]                     | strata      | 0.109    | 794     | 62.9     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.493    | 1604    | 62.0     |

## Summary

| Category              | Strata Rank                   | vs #1                              |
| --------------------- | ----------------------------- | ---------------------------------- |
| Parsing (JSON)        | **#1** / 5                    | **17.3% faster** than #2 (orjson)  |
| Parsing (NDJSON)      | **#4** / 5                    | 83.0% behind #1 (msgspec)          |
| Load JSON (file)      | **#1** / 5                    | **47.5% faster** than #2 (orjson)  |
| Load NDJSON (file)    | **#1** / 5                    | **31.8% faster** than #2 (msgspec) |
| Serialization (dumps) | **#2** / 5                    | 0.3% behind #1 (msgspec)           |
| Dump (file)           | **#3** / 5                    | 1.9% behind #1 (msgspec)           |
| Search (file)         | mem_eff faster in 0/3 queries | -                                  |
| JSONPath              | **#1** in 5/5 queries         | -                                  |
