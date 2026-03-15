# Strata Benchmark Results

Generated: 2026-03-15 03:09:55

## Environment

- Commit: 5c264ca1e42ba846ee2425bce2f825e70fdfafb1
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
| strata        | users.json   | 3.859    | 4.894       | 6.280    | 35.0     |
| orjson        | users.json   | 4.591    | 6.001       | 7.142    | 39.1     |
| msgspec       | users.json   | 4.839    | 6.148       | 6.348    | 42.2     |
| ujson         | users.json   | 6.734    | 8.783       | 10.354   | 41.1     |
| json (stdlib) | users.json   | 9.697    | 12.153      | 15.127   | 44.1     |
| strata        | users.ndjson | 4.098    | 5.490       | 7.335    | 56.6     |
| msgspec       | users.ndjson | 5.018    | 6.653       | 6.691    | 57.8     |
| orjson        | users.ndjson | 5.964    | 7.384       | 7.878    | 57.6     |
| ujson         | users.ndjson | 6.417    | 9.556       | 9.902    | 57.8     |
| json (stdlib) | users.ndjson | 11.142   | 12.994      | 15.541   | 58.6     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 3.690    | 4.714       | 4.780    | 44.2     |
| orjson        | users.json   | 5.206    | 6.576       | 6.784    | 44.2     |
| ujson         | users.json   | 6.688    | 9.378       | 9.919    | 48.3     |
| msgspec       | users.json   | 4.955    | 9.536       | 10.371   | 48.3     |
| json (stdlib) | users.json   | 10.860   | 11.183      | 11.495   | 48.3     |
| strata        | users.ndjson | 3.981    | 4.997       | 5.079    | 58.6     |
| orjson        | users.ndjson | 4.569    | 6.379       | 7.188    | 58.6     |
| msgspec       | users.ndjson | 6.724    | 7.174       | 8.062    | 59.0     |
| ujson         | users.ndjson | 7.161    | 8.911       | 9.902    | 59.0     |
| json (stdlib) | users.ndjson | 9.847    | 11.762      | 12.529   | 59.0     |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 1.468    | 1.480       | 1.500    | 48.9     |
| orjson  | users.json | 1.474    | 1.495       | 1.678    | 50.8     |
| msgspec | users.json | 1.450    | 1.506       | 1.757    | 51.7     |
| ujson   | users.json | 6.435    | 6.524       | 6.638    | 52.7     |
| json    | users.json | 8.814    | 8.832       | 9.112    | 53.2     |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 1.657    | 1.706       | 1.779    | 53.2     |
| orjson        | users.json | 1.735    | 1.786       | 1.817    | 53.2     |
| msgspec       | users.json | 1.872    | 2.234       | 2.265    | 53.2     |
| ujson         | users.json | 6.780    | 6.985       | 7.087    | 53.2     |
| json (stdlib) | users.json | 67.488   | 68.924      | 69.085   | 53.2     |

## search (file-based)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 1.608    | 1000    | 53.2     |
|                 | orjson+jmespath    | 5.784    | 1000    | 53.3     |
|                 | orjson+jsonpath-ng | 6.923    | 1000    | 54.0     |
| all item prices | strata             | 1.857    | 12046   | 54.0     |
|                 | orjson+jmespath    | 13.893   | 1000    | 54.0     |
|                 | orjson+jsonpath-ng | 35.869   | 12046   | 54.0     |
| recursive price | strata             | 1.802    | 12046   | 53.3     |
|                 | orjson+jsonpath-ng | 126.719  | 12046   | 54.4     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.102    | 1000    | 53.6     |
|                                            | jmespath    | 0.383    | 1000    | 53.6     |
|                                            | jsonpath-ng | 4.428    | 1000    | 53.6     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.914    | 12046   | 53.6     |
|                                            | jmespath    | 10.914   | 1000    | 53.9     |
|                                            | jsonpath-ng | 32.405   | 12046   | 58.1     |
| $..price                                   | strata      | 1.269    | 12046   | 56.2     |
|                                            | jsonpath-ng | 125.502  | 12046   | 58.2     |
| $.users\[?(@.age>30)\]                     | strata      | 0.143    | 794     | 56.6     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.499    | 1604    | 56.6     |

## Summary

| Category           | Strata Rank           | vs #1                              |
| ------------------ | --------------------- | ---------------------------------- |
| loads (JSON)       | **#1** / 5            | **19.0% faster** than #2 (orjson)  |
| loads (NDJSON)     | **#1** / 5            | **22.5% faster** than #2 (msgspec) |
| load (JSON file)   | **#1** / 5            | **34.3% faster** than #2 (msgspec) |
| load (NDJSON file) | **#1** / 5            | **14.8% faster** than #2 (orjson)  |
| dumps              | **#2** / 5            | 1.3% behind #1 (msgspec)           |
| dump               | **#1** / 5            | **4.7% faster** than #2 (orjson)   |
| search (JSONPath)  | **#1** in 3/3 queries | avg **2613% faster**               |
| query (JSONPath)   | **#1** in 5/5 queries | -                                  |
