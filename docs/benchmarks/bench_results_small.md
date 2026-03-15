# Strata Benchmark Results

Generated: 2026-03-15 03:50:54

## Environment

- Commit: 18a352195ff912a27133fc13a4b7ba44d44cbf4a
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
| strata        | users.json   | 3.919    | 4.604       | 5.058    | 35.3     |
| orjson        | users.json   | 4.439    | 6.343       | 6.418    | 39.3     |
| msgspec       | users.json   | 4.864    | 6.380       | 6.896    | 42.5     |
| json (stdlib) | users.json   | 9.064    | 10.215      | 10.747   | 44.2     |
| ujson         | users.json   | 7.713    | 10.371      | 10.519   | 41.4     |
| strata        | users.ndjson | 3.721    | 4.581       | 4.745    | 56.1     |
| orjson        | users.ndjson | 4.681    | 6.306       | 6.737    | 57.1     |
| msgspec       | users.ndjson | 4.651    | 6.939       | 9.149    | 57.2     |
| ujson         | users.ndjson | 6.283    | 9.422       | 12.802   | 57.2     |
| json (stdlib) | users.ndjson | 9.298    | 11.181      | 11.666   | 58.1     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 3.686    | 4.556       | 4.607    | 44.2     |
| orjson        | users.json   | 4.564    | 6.485       | 8.672    | 45.2     |
| msgspec       | users.json   | 4.982    | 6.640       | 7.039    | 48.3     |
| ujson         | users.json   | 8.959    | 9.834       | 10.506   | 48.3     |
| json (stdlib) | users.json   | 9.177    | 10.890      | 11.650   | 48.3     |
| strata        | users.ndjson | 3.910    | 4.962       | 4.974    | 58.1     |
| msgspec       | users.ndjson | 5.001    | 6.331       | 6.904    | 58.2     |
| orjson        | users.ndjson | 5.296    | 7.186       | 10.359   | 58.1     |
| ujson         | users.ndjson | 7.217    | 10.908      | 13.388   | 58.2     |
| json (stdlib) | users.ndjson | 9.663    | 12.049      | 12.241   | 58.2     |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| msgspec | users.json | 1.465    | 1.490       | 1.676    | 51.5     |
| strata  | users.json | 1.471    | 1.506       | 1.665    | 48.3     |
| orjson  | users.json | 1.595    | 1.652       | 1.720    | 50.4     |
| ujson   | users.json | 6.175    | 6.252       | 6.420    | 51.5     |
| json    | users.json | 8.970    | 9.016       | 9.388    | 52.8     |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 1.701    | 1.816       | 1.853    | 53.0     |
| msgspec       | users.json | 1.796    | 1.833       | 2.007    | 53.0     |
| orjson        | users.json | 1.891    | 2.289       | 2.548    | 53.0     |
| ujson         | users.json | 6.739    | 6.743       | 6.828    | 53.0     |
| json (stdlib) | users.json | 67.400   | 68.349      | 68.753   | 53.0     |

## search (file-based)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 1.797    | 1000    | 53.0     |
|                 | orjson+jmespath    | 5.810    | 1000    | 53.1     |
|                 | orjson+jsonpath-ng | 7.045    | 1000    | 53.7     |
| all item prices | strata             | 1.828    | 12046   | 53.7     |
|                 | orjson+jmespath    | 14.273   | 1000    | 53.7     |
|                 | orjson+jsonpath-ng | 37.304   | 12046   | 53.6     |
| recursive price | strata             | 1.942    | 12046   | 52.9     |
|                 | orjson+jsonpath-ng | 127.956  | 12046   | 54.1     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.079    | 1000    | 53.1     |
|                                            | jmespath    | 0.474    | 1000    | 53.1     |
|                                            | jsonpath-ng | 4.200    | 1000    | 53.1     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.940    | 12046   | 53.1     |
|                                            | jmespath    | 11.802   | 1000    | 53.4     |
|                                            | jsonpath-ng | 31.714   | 12046   | 57.6     |
| $..price                                   | strata      | 1.308    | 12046   | 55.6     |
|                                            | jsonpath-ng | 126.557  | 12046   | 57.6     |
| $.users\[?(@.age>30)\]                     | strata      | 0.187    | 794     | 57.0     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.409    | 1604    | 56.0     |

## Summary

| Category           | Strata Rank           | vs #1                              |
| ------------------ | --------------------- | ---------------------------------- |
| loads (JSON)       | **#1** / 5            | **13.3% faster** than #2 (orjson)  |
| loads (NDJSON)     | **#1** / 5            | **25.0% faster** than #2 (msgspec) |
| load (JSON file)   | **#1** / 5            | **23.8% faster** than #2 (orjson)  |
| load (NDJSON file) | **#1** / 5            | **27.9% faster** than #2 (msgspec) |
| dumps              | **#2** / 5            | 0.4% behind #1 (msgspec)           |
| dump               | **#1** / 5            | **5.6% faster** than #2 (msgspec)  |
| search (JSONPath)  | **#1** in 3/3 queries | avg **2464% faster**               |
| query (JSONPath)   | **#1** in 5/5 queries | -                                  |
