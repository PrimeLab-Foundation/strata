# Strata Benchmark Results

Generated: 2026-03-15 02:04:59

## Environment

- Commit: ae530e0c1e5e4d01eab91b20240642c5ccc9faba
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
| strata        | users.json   | 3.661    | 4.442       | 4.628    | 35.3     |
| orjson        | users.json   | 4.205    | 5.744       | 5.948    | 39.3     |
| msgspec       | users.json   | 4.359    | 5.768       | 5.886    | 42.5     |
| ujson         | users.json   | 5.544    | 7.654       | 8.257    | 41.4     |
| json (stdlib) | users.json   | 8.005    | 9.526       | 9.762    | 43.4     |
| strata        | users.ndjson | 3.453    | 4.302       | 4.367    | 53.9     |
| orjson        | users.ndjson | 4.384    | 5.824       | 5.902    | 55.0     |
| msgspec       | users.ndjson | 4.364    | 5.888       | 6.003    | 55.1     |
| ujson         | users.ndjson | 5.854    | 7.897       | 8.037    | 55.1     |
| json (stdlib) | users.ndjson | 9.601    | 10.409      | 10.760   | 55.1     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 3.471    | 4.242       | 4.268    | 43.5     |
| orjson        | users.json   | 4.289    | 5.853       | 6.242    | 43.5     |
| msgspec       | users.json   | 4.374    | 5.954       | 5.972    | 46.4     |
| ujson         | users.json   | 5.978    | 7.905       | 8.599    | 46.4     |
| json (stdlib) | users.json   | 8.144    | 9.755       | 9.778    | 46.4     |
| strata        | users.ndjson | 3.610    | 4.478       | 4.555    | 55.1     |
| orjson        | users.ndjson | 4.313    | 5.760       | 5.843    | 55.1     |
| msgspec       | users.ndjson | 4.365    | 5.987       | 6.176    | 56.1     |
| ujson         | users.ndjson | 6.082    | 8.078       | 8.211    | 56.1     |
| json (stdlib) | users.ndjson | 8.862    | 10.529      | 10.640   | 56.1     |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 1.386    | 1.388       | 1.461    | 47.2     |
| msgspec | users.json | 1.404    | 1.419       | 1.450    | 49.6     |
| orjson  | users.json | 1.428    | 1.438       | 1.541    | 49.4     |
| ujson   | users.json | 5.886    | 5.892       | 5.921    | 49.7     |
| json    | users.json | 8.418    | 8.418       | 8.430    | 50.1     |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 1.590    | 1.614       | 1.633    | 50.1     |
| orjson        | users.json | 1.598    | 1.621       | 1.628    | 50.1     |
| msgspec       | users.json | 1.611    | 1.624       | 1.655    | 50.1     |
| ujson         | users.json | 6.323    | 6.330       | 6.334    | 50.1     |
| json (stdlib) | users.json | 62.945   | 64.185      | 65.312   | 50.1     |

## search (mem_eff=True)

| Query           | Library                      | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------------------- | -------- | ------- | -------- |
| all user ids    | strata (mem_eff)             | 1.458    | 1000    | 50.1     |
|                 | orjson+jmespath (mem_eff)    | 5.139    | 1000    | 50.2     |
|                 | orjson+jsonpath-ng (mem_eff) | 6.272    | 1000    | 50.7     |
| all item prices | strata (mem_eff)             | 1.602    | 12046   | 50.7     |
|                 | orjson+jmespath (mem_eff)    | 12.545   | 1000    | 50.8     |
|                 | orjson+jsonpath-ng (mem_eff) | 33.030   | 12046   | 50.5     |
| recursive price | strata (mem_eff)             | 1.627    | 12046   | 49.8     |
|                 | orjson+jsonpath-ng (mem_eff) | 119.188  | 12046   | 51.0     |

## search (mem_eff=False)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 1.459    | 1000    | 50.1     |
|                 | orjson+jmespath    | 5.139    | 1000    | 50.2     |
|                 | orjson+jsonpath-ng | 6.272    | 1000    | 50.7     |
| all item prices | strata             | 1.594    | 12046   | 50.7     |
|                 | orjson+jmespath    | 12.545   | 1000    | 50.8     |
|                 | orjson+jsonpath-ng | 33.030   | 12046   | 50.5     |
| recursive price | strata             | 1.613    | 12046   | 49.8     |
|                 | orjson+jsonpath-ng | 119.188  | 12046   | 51.0     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.061    | 1000    | 50.9     |
|                                            | jmespath    | 0.346    | 1000    | 50.9     |
|                                            | jsonpath-ng | 3.499    | 1000    | 50.9     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.910    | 12046   | 50.9     |
|                                            | jmespath    | 9.857    | 1000    | 50.9     |
|                                            | jsonpath-ng | 29.564   | 12046   | 54.5     |
| $..price                                   | strata      | 1.263    | 12046   | 53.5     |
|                                            | jsonpath-ng | 117.395  | 12046   | 55.5     |
| $.users\[?(@.age>30)\]                     | strata      | 0.100    | 794     | 53.9     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.365    | 1604    | 53.9     |

## Summary

| Category               | Strata Rank           | vs #1                              |
| ---------------------- | --------------------- | ---------------------------------- |
| loads (JSON)           | **#1** / 5            | **14.9% faster** than #2 (orjson)  |
| loads (NDJSON)         | **#1** / 5            | **26.4% faster** than #2 (msgspec) |
| load (JSON file)       | **#1** / 5            | **23.5% faster** than #2 (orjson)  |
| load (NDJSON file)     | **#1** / 5            | **19.5% faster** than #2 (orjson)  |
| dumps                  | **#1** / 5            | **1.3% faster** than #2 (msgspec)  |
| dump                   | **#1** / 5            | **0.5% faster** than #2 (orjson)   |
| search (mem_eff=True)  | **#1** in 3/3 queries | avg **2721% faster**               |
| search (mem_eff=False) | **#1** in 3/3 queries | avg **2743% faster**               |
| query (JSONPath)       | **#1** in 5/5 queries | -                                  |
