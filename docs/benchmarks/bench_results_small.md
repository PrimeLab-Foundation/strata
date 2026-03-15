# Strata Benchmark Results

Generated: 2026-03-15 02:17:15

## Environment

- Commit: 1ad8828b764067899097f4fab331b0010dccd034
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
| strata        | users.json   | 3.500    | 4.233       | 4.711    | 37.7     |
| orjson        | users.json   | 4.367    | 5.542       | 6.010    | 41.7     |
| msgspec       | users.json   | 4.597    | 6.489       | 6.738    | 44.9     |
| ujson         | users.json   | 6.142    | 7.848       | 8.142    | 43.8     |
| json (stdlib) | users.json   | 8.758    | 9.814       | 9.897    | 46.8     |
| strata        | users.ndjson | 3.553    | 4.419       | 4.590    | 60.5     |
| orjson        | users.ndjson | 4.899    | 6.469       | 6.926    | 61.5     |
| msgspec       | users.ndjson | 4.780    | 6.620       | 7.125    | 62.6     |
| ujson         | users.ndjson | 6.632    | 8.671       | 8.847    | 63.6     |
| json (stdlib) | users.ndjson | 10.197   | 11.224      | 11.390   | 62.6     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 3.487    | 4.267       | 4.543    | 46.8     |
| orjson        | users.json   | 4.354    | 5.912       | 6.344    | 46.8     |
| msgspec       | users.json   | 4.728    | 6.153       | 6.578    | 49.9     |
| ujson         | users.json   | 6.470    | 8.135       | 8.651    | 49.9     |
| json (stdlib) | users.json   | 8.970    | 9.915       | 10.185   | 49.9     |
| strata        | users.ndjson | 3.894    | 4.574       | 4.916    | 62.6     |
| msgspec       | users.ndjson | 4.524    | 6.031       | 7.087    | 63.6     |
| orjson        | users.ndjson | 4.441    | 7.041       | 7.120    | 62.6     |
| ujson         | users.ndjson | 6.986    | 8.901       | 10.150   | 64.6     |
| json (stdlib) | users.ndjson | 9.742    | 10.566      | 10.835   | 63.6     |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 1.392    | 1.393       | 1.494    | 49.9     |
| msgspec | users.json | 1.396    | 1.414       | 1.498    | 53.8     |
| orjson  | users.json | 1.508    | 1.547       | 1.623    | 53.7     |
| ujson   | users.json | 6.128    | 6.170       | 6.315    | 56.0     |
| json    | users.json | 8.619    | 8.639       | 8.721    | 56.7     |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 1.543    | 1.614       | 1.697    | 56.7     |
| orjson        | users.json | 1.676    | 1.766       | 1.828    | 56.7     |
| msgspec       | users.json | 1.690    | 1.817       | 1.827    | 56.7     |
| ujson         | users.json | 6.577    | 6.595       | 6.757    | 56.7     |
| json (stdlib) | users.json | 65.167   | 65.360      | 66.168   | 56.7     |

## search (file-based)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 1.497    | 1000    | 56.7     |
|                 | orjson+jmespath    | 5.229    | 1000    | 56.8     |
|                 | orjson+jsonpath-ng | 6.732    | 1000    | 57.3     |
| all item prices | strata             | 1.670    | 12046   | 57.3     |
|                 | orjson+jmespath    | 13.285   | 1000    | 57.3     |
|                 | orjson+jsonpath-ng | 35.425   | 12046   | 57.0     |
| recursive price | strata             | 1.719    | 12046   | 57.4     |
|                 | orjson+jsonpath-ng | 124.139  | 12046   | 58.5     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.089    | 1000    | 58.4     |
|                                            | jmespath    | 0.405    | 1000    | 58.4     |
|                                            | jsonpath-ng | 3.791    | 1000    | 58.4     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.912    | 12046   | 58.5     |
|                                            | jmespath    | 10.062   | 1000    | 58.5     |
|                                            | jsonpath-ng | 32.009   | 12046   | 62.1     |
| $..price                                   | strata      | 1.308    | 12046   | 60.1     |
|                                            | jsonpath-ng | 121.721  | 12046   | 62.1     |
| $.users\[?(@.age>30)\]                     | strata      | 0.110    | 794     | 61.5     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.468    | 1604    | 60.5     |

## Summary

| Category               | Strata Rank           | vs #1                              |
| ---------------------- | --------------------- | ---------------------------------- |
| loads (JSON)           | **#1** / 5            | **24.7% faster** than #2 (orjson)  |
| loads (NDJSON)         | **#1** / 5            | **34.5% faster** than #2 (msgspec) |
| load (JSON file)       | **#1** / 5            | **24.9% faster** than #2 (orjson)  |
| load (NDJSON file)     | **#1** / 5            | **14.0% faster** than #2 (orjson)  |
| dumps                  | **#1** / 5            | **0.3% faster** than #2 (msgspec)  |
| dump                   | **#1** / 5            | **8.6% faster** than #2 (orjson)   |
| search (mem_eff=False) | **#1** in 3/3 queries | avg **2688% faster**               |
| query (JSONPath)       | **#1** in 5/5 queries | -                                  |
