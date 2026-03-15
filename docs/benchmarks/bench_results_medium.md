# Strata Benchmark Results

Generated: 2026-03-15 03:51:18

## Environment

- Commit: 18a352195ff912a27133fc13a4b7ba44d44cbf4a
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
| strata        | users.json   | 24.657   | 30.568      | 31.095   | 71.2     |
| orjson        | users.json   | 29.008   | 38.042      | 45.570   | 91.1     |
| msgspec       | users.json   | 27.978   | 41.996      | 50.373   | 91.9     |
| ujson         | users.json   | 49.250   | 56.433      | 71.880   | 109.6    |
| json (stdlib) | users.json   | 54.274   | 69.180      | 80.112   | 98.2     |
| strata        | users.ndjson | 22.898   | 28.279      | 32.832   | 249.6    |
| msgspec       | users.ndjson | 34.568   | 36.474      | 38.045   | 259.6    |
| orjson        | users.ndjson | 34.068   | 38.204      | 42.054   | 258.6    |
| ujson         | users.ndjson | 46.811   | 50.913      | 54.396   | 258.6    |
| json (stdlib) | users.ndjson | 60.917   | 65.444      | 66.097   | 260.6    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 24.164   | 29.625      | 30.359   | 97.6     |
| orjson        | users.json   | 31.519   | 39.802      | 42.545   | 97.6     |
| msgspec       | users.json   | 27.520   | 43.133      | 50.252   | 104.5    |
| ujson         | users.json   | 42.206   | 58.744      | 70.274   | 122.2    |
| json (stdlib) | users.json   | 66.724   | 71.058      | 88.664   | 104.5    |
| strata        | users.ndjson | 24.096   | 29.549      | 32.350   | 261.6    |
| msgspec       | users.ndjson | 37.085   | 37.170      | 40.710   | 264.7    |
| orjson        | users.ndjson | 31.678   | 38.193      | 42.691   | 262.7    |
| ujson         | users.ndjson | 51.999   | 56.023      | 57.695   | 263.7    |
| json (stdlib) | users.ndjson | 63.605   | 66.905      | 67.123   | 265.7    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 9.700    | 9.826       | 10.032   | 108.7    |
| msgspec | users.json | 10.585   | 10.630      | 11.158   | 159.3    |
| orjson  | users.json | 10.502   | 10.704      | 11.145   | 131.0    |
| ujson   | users.json | 39.937   | 40.035      | 40.662   | 167.2    |
| json    | users.json | 57.569   | 57.919      | 60.280   | 192.5    |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 10.406   | 10.416      | 12.056   | 198.8    |
| msgspec       | users.json | 11.093   | 11.280      | 12.081   | 243.5    |
| orjson        | users.json | 11.347   | 11.412      | 11.576   | 221.1    |
| ujson         | users.json | 41.514   | 42.405      | 42.581   | 221.2    |
| json (stdlib) | users.json | 427.255  | 444.383     | 444.875  | 243.6    |

## search (file-based)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 10.904   | 2000    | 240.6    |
|                 | orjson+jmespath    | 32.316   | 2000    | 244.0    |
|                 | orjson+jsonpath-ng | 33.930   | 2000    | 246.2    |
| all item prices | strata             | 12.300   | 100999  | 247.4    |
|                 | orjson+jmespath    | 79.638   | 2000    | 256.6    |
|                 | orjson+jsonpath-ng | 261.157  | 100999  | 263.8    |
| recursive price | strata             | 11.762   | 100999  | 260.8    |
|                 | orjson+jsonpath-ng | 932.775  | 100999  | 264.7    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.306    | 2000    | 286.9    |
|                                            | jmespath    | 1.034    | 2000    | 286.9    |
|                                            | jsonpath-ng | 16.911   | 2000    | 287.7    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 5.857    | 100999  | 287.7    |
|                                            | jmespath    | 61.425   | 2000    | 291.6    |
|                                            | jsonpath-ng | 256.348  | 100999  | 293.9    |
| $..price                                   | strata      | 8.014    | 100999  | 294.4    |
|                                            | jsonpath-ng | 896.753  | 100999  | 276.8    |
| $.users\[?(@.age>30)\]                     | strata      | 0.352    | 1606    | 274.5    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.063    | 6650    | 273.5    |

## Summary

| Category           | Strata Rank           | vs #1                              |
| ------------------ | --------------------- | ---------------------------------- |
| loads (JSON)       | **#1** / 5            | **13.5% faster** than #2 (msgspec) |
| loads (NDJSON)     | **#1** / 5            | **48.8% faster** than #2 (orjson)  |
| load (JSON file)   | **#1** / 5            | **13.9% faster** than #2 (msgspec) |
| load (NDJSON file) | **#1** / 5            | **31.5% faster** than #2 (orjson)  |
| dumps              | **#1** / 5            | **8.3% faster** than #2 (orjson)   |
| dump               | **#1** / 5            | **6.6% faster** than #2 (msgspec)  |
| search (JSONPath)  | **#1** in 3/3 queries | avg **2858% faster**               |
| query (JSONPath)   | **#1** in 5/5 queries | -                                  |
