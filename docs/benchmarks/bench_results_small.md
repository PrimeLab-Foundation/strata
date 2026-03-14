# Strata Benchmark Results

Generated: 2026-03-15 01:39:09

## Environment

- Commit: 85dcf578761e98ba791b90249e4985e5c20eda03
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
| strata        | users.json   | 3.707    | 4.276       | 4.694    | 35.1     |
| msgspec       | users.json   | 4.243    | 5.847       | 6.487    | 42.2     |
| orjson        | users.json   | 4.375    | 6.030       | 6.414    | 39.1     |
| ujson         | users.json   | 5.595    | 9.029       | 9.117    | 41.2     |
| json (stdlib) | users.json   | 8.257    | 9.936       | 10.565   | 43.2     |
| strata        | users.ndjson | 3.466    | 4.360       | 4.787    | 60.2     |
| orjson        | users.ndjson | 4.727    | 6.028       | 6.228    | 61.2     |
| msgspec       | users.ndjson | 4.406    | 6.046       | 6.221    | 61.3     |
| ujson         | users.ndjson | 6.390    | 8.515       | 8.734    | 61.3     |
| json (stdlib) | users.ndjson | 9.019    | 10.673      | 10.677   | 61.3     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 3.458    | 4.297       | 4.306    | 43.3     |
| orjson        | users.json   | 4.434    | 5.912       | 6.076    | 44.2     |
| msgspec       | users.json   | 4.742    | 6.870       | 7.072    | 48.3     |
| ujson         | users.json   | 6.201    | 8.652       | 9.930    | 48.3     |
| json (stdlib) | users.json   | 8.670    | 10.551      | 10.635   | 48.3     |
| strata        | users.ndjson | 3.604    | 4.576       | 4.795    | 61.3     |
| orjson        | users.ndjson | 4.787    | 6.062       | 6.846    | 61.4     |
| msgspec       | users.ndjson | 4.651    | 6.157       | 6.630    | 63.3     |
| ujson         | users.ndjson | 6.307    | 8.493       | 8.574    | 63.3     |
| json (stdlib) | users.ndjson | 9.034    | 11.314      | 11.416   | 63.3     |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 1.440    | 1.447       | 1.531    | 48.3     |
| msgspec | users.json | 1.433    | 1.456       | 1.500    | 50.6     |
| orjson  | users.json | 1.418    | 1.460       | 1.679    | 50.2     |
| ujson   | users.json | 6.072    | 6.097       | 6.308    | 50.6     |
| json    | users.json | 8.708    | 8.783       | 9.002    | 50.9     |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 1.634    | 1.816       | 1.929    | 50.9     |
| orjson        | users.json | 1.707    | 1.927       | 1.932    | 50.9     |
| msgspec       | users.json | 2.087    | 2.176       | 2.196    | 50.9     |
| ujson         | users.json | 6.500    | 6.651       | 6.797    | 50.9     |
| json (stdlib) | users.json | 66.945   | 67.068      | 67.768   | 50.9     |

## search (mem_eff=True)

| Query           | Library                      | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------------------- | -------- | ------- | -------- |
| all user ids    | orjson+jmespath (mem_eff)    | 5.356    | 1000    | 56.2     |
|                 | orjson+jsonpath-ng (mem_eff) | 6.991    | 1000    | 56.8     |
|                 | strata (mem_eff)             | 9.840    | 1000    | 56.1     |
| all item prices | strata (mem_eff)             | 10.440   | 12046   | 56.9     |
|                 | orjson+jmespath (mem_eff)    | 13.165   | 1000    | 56.9     |
|                 | orjson+jsonpath-ng (mem_eff) | 34.419   | 12046   | 56.8     |
| recursive price | strata (mem_eff)             | 10.602   | 12046   | 56.1     |
|                 | orjson+jsonpath-ng (mem_eff) | 125.189  | 12046   | 57.3     |

## search (mem_eff=False)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 5.004    | 1000    | 51.0     |
|                 | orjson+jmespath    | 5.356    | 1000    | 56.2     |
|                 | orjson+jsonpath-ng | 6.991    | 1000    | 56.8     |
| all item prices | strata             | 6.317    | 12046   | 56.8     |
|                 | orjson+jmespath    | 13.165   | 1000    | 56.9     |
|                 | orjson+jsonpath-ng | 34.419   | 12046   | 56.8     |
| recursive price | strata             | 6.534    | 12046   | 56.1     |
|                 | orjson+jsonpath-ng | 125.189  | 12046   | 57.3     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.067    | 1000    | 57.2     |
|                                            | jmespath    | 0.392    | 1000    | 57.2     |
|                                            | jsonpath-ng | 3.770    | 1000    | 57.2     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.904    | 12046   | 57.2     |
|                                            | jmespath    | 10.730   | 1000    | 57.2     |
|                                            | jsonpath-ng | 32.034   | 12046   | 59.8     |
| $..price                                   | strata      | 1.269    | 12046   | 58.8     |
|                                            | jsonpath-ng | 122.016  | 12046   | 60.8     |
| $.users\[?(@.age>30)\]                     | strata      | 0.108    | 794     | 60.2     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.474    | 1604    | 60.2     |

## Summary

| Category               | Strata Rank           | vs #1                              |
| ---------------------- | --------------------- | ---------------------------------- |
| loads (JSON)           | **#1** / 5            | **14.4% faster** than #2 (msgspec) |
| loads (NDJSON)         | **#1** / 5            | **27.1% faster** than #2 (msgspec) |
| load (JSON file)       | **#1** / 5            | **28.2% faster** than #2 (orjson)  |
| load (NDJSON file)     | **#1** / 5            | **29.1% faster** than #2 (msgspec) |
| dumps                  | **#3** / 5            | 1.6% behind #1 (orjson)            |
| dump                   | **#1** / 5            | **4.5% faster** than #2 (orjson)   |
| search (mem_eff=True)  | #1 in 2/3 queries     | -                                  |
| search (mem_eff=False) | **#1** in 3/3 queries | avg **644% faster**                |
| query (JSONPath)       | **#1** in 5/5 queries | -                                  |
