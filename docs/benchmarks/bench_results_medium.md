# Strata Benchmark Results

Generated: 2026-03-15 02:04:37

## Environment

- Commit: ae530e0c1e5e4d01eab91b20240642c5ccc9faba
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
| strata        | users.json   | 22.782   | 28.118      | 28.484   | 71.1     |
| orjson        | users.json   | 25.841   | 36.251      | 37.111   | 91.0     |
| msgspec       | users.json   | 26.769   | 38.716      | 40.138   | 91.8     |
| ujson         | users.json   | 36.910   | 52.707      | 53.405   | 109.4    |
| json (stdlib) | users.json   | 52.197   | 64.597      | 65.849   | 98.1     |
| strata        | users.ndjson | 21.850   | 27.659      | 28.326   | 272.0    |
| orjson        | users.ndjson | 30.565   | 32.683      | 33.679   | 281.0    |
| msgspec       | users.ndjson | 32.210   | 34.121      | 35.403   | 262.3    |
| ujson         | users.ndjson | 41.139   | 48.651      | 49.161   | 281.2    |
| json (stdlib) | users.ndjson | 58.470   | 61.243      | 62.201   | 263.3    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 22.825   | 28.989      | 29.329   | 97.4     |
| orjson        | users.json   | 25.237   | 36.303      | 37.622   | 97.4     |
| msgspec       | users.json   | 26.585   | 39.376      | 40.958   | 104.4    |
| ujson         | users.json   | 38.040   | 54.483      | 55.230   | 122.1    |
| json (stdlib) | users.json   | 52.202   | 65.040      | 66.784   | 104.4    |
| strata        | users.ndjson | 22.794   | 29.007      | 29.244   | 264.3    |
| orjson        | users.ndjson | 30.880   | 33.110      | 34.034   | 265.4    |
| msgspec       | users.ndjson | 32.380   | 34.662      | 36.493   | 267.3    |
| ujson         | users.ndjson | 42.922   | 50.161      | 51.060   | 266.3    |
| json (stdlib) | users.ndjson | 58.109   | 60.224      | 60.843   | 268.3    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 9.509    | 9.547       | 9.755    | 108.7    |
| msgspec | users.json | 10.255   | 10.268      | 10.303   | 159.4    |
| orjson  | users.json | 10.402   | 10.420      | 10.506   | 131.1    |
| ujson   | users.json | 39.191   | 39.206      | 39.325   | 165.2    |
| json    | users.json | 55.867   | 55.952      | 56.886   | 192.2    |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 10.522   | 10.710      | 12.367   | 198.4    |
| msgspec       | users.json | 11.213   | 11.561      | 11.642   | 243.0    |
| orjson        | users.json | 11.712   | 11.730      | 11.776   | 220.7    |
| ujson         | users.json | 42.111   | 42.233      | 42.619   | 220.7    |
| json (stdlib) | users.json | 408.144  | 408.351     | 419.861  | 243.1    |

## search (mem_eff=True)

| Query           | Library                      | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------------------- | -------- | ------- | -------- |
| all user ids    | strata (mem_eff)             | 9.644    | 2000    | 240.2    |
|                 | orjson+jmespath (mem_eff)    | 28.248   | 2000    | 243.3    |
|                 | orjson+jsonpath-ng (mem_eff) | 30.090   | 2000    | 245.7    |
| all item prices | strata (mem_eff)             | 10.304   | 100999  | 249.2    |
|                 | orjson+jmespath (mem_eff)    | 74.019   | 2000    | 258.4    |
|                 | orjson+jsonpath-ng (mem_eff) | 239.296  | 100999  | 264.5    |
| recursive price | strata (mem_eff)             | 10.332   | 100999  | 261.4    |
|                 | orjson+jsonpath-ng (mem_eff) | 820.326  | 100999  | 264.7    |

## search (mem_eff=False)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 9.574    | 2000    | 240.2    |
|                 | orjson+jmespath    | 28.248   | 2000    | 243.3    |
|                 | orjson+jsonpath-ng | 30.090   | 2000    | 245.7    |
| all item prices | strata             | 10.528   | 100999  | 247.7    |
|                 | orjson+jmespath    | 74.019   | 2000    | 258.4    |
|                 | orjson+jsonpath-ng | 239.296  | 100999  | 264.5    |
| recursive price | strata             | 10.344   | 100999  | 261.4    |
|                 | orjson+jsonpath-ng | 820.326  | 100999  | 264.7    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.247    | 2000    | 286.8    |
|                                            | jmespath    | 0.794    | 2000    | 286.9    |
|                                            | jsonpath-ng | 14.701   | 2000    | 287.7    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 5.927    | 100999  | 287.7    |
|                                            | jmespath    | 57.936   | 2000    | 291.5    |
|                                            | jsonpath-ng | 228.718  | 100999  | 295.1    |
| $..price                                   | strata      | 7.744    | 100999  | 295.8    |
|                                            | jsonpath-ng | 814.692  | 100999  | 298.3    |
| $.users\[?(@.age>30)\]                     | strata      | 0.308    | 1606    | 296.0    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 8.410    | 6650    | 296.0    |

## Summary

| Category               | Strata Rank           | vs #1                             |
| ---------------------- | --------------------- | --------------------------------- |
| loads (JSON)           | **#1** / 5            | **13.4% faster** than #2 (orjson) |
| loads (NDJSON)         | **#1** / 5            | **39.9% faster** than #2 (orjson) |
| load (JSON file)       | **#1** / 5            | **10.6% faster** than #2 (orjson) |
| load (NDJSON file)     | **#1** / 5            | **35.5% faster** than #2 (orjson) |
| dumps                  | **#1** / 5            | **7.8% faster** than #2 (msgspec) |
| dump                   | **#1** / 5            | **6.6% faster** than #2 (msgspec) |
| search (mem_eff=True)  | **#1** in 3/3 queries | avg **2884% faster**              |
| search (mem_eff=False) | **#1** in 3/3 queries | avg **2876% faster**              |
| query (JSONPath)       | **#1** in 5/5 queries | -                                 |
