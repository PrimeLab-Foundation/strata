# Strata Benchmark Results

Generated: 2026-03-15 01:38:31

## Environment

- Commit: 85dcf578761e98ba791b90249e4985e5c20eda03
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
| strata        | users.json   | 23.113   | 29.321      | 29.933   | 75.1     |
| orjson        | users.json   | 28.591   | 39.663      | 40.164   | 95.0     |
| msgspec       | users.json   | 29.062   | 42.093      | 45.869   | 94.8     |
| ujson         | users.json   | 39.042   | 57.368      | 58.066   | 112.5    |
| json (stdlib) | users.json   | 54.919   | 67.432      | 69.414   | 101.1    |
| strata        | users.ndjson | 22.434   | 27.989      | 29.166   | 186.2    |
| orjson        | users.ndjson | 30.939   | 34.096      | 35.517   | 170.2    |
| msgspec       | users.ndjson | 33.145   | 34.820      | 35.404   | 170.3    |
| ujson         | users.ndjson | 43.482   | 49.155      | 50.255   | 169.3    |
| json (stdlib) | users.ndjson | 59.551   | 63.526      | 64.372   | 171.3    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 23.694   | 30.583      | 30.660   | 101.5    |
| orjson        | users.json   | 26.249   | 38.349      | 38.640   | 101.5    |
| msgspec       | users.json   | 28.175   | 39.556      | 41.059   | 107.4    |
| ujson         | users.json   | 38.104   | 54.821      | 55.768   | 125.1    |
| json (stdlib) | users.json   | 52.080   | 66.234      | 67.279   | 107.4    |
| strata        | users.ndjson | 23.521   | 29.361      | 29.854   | 173.3    |
| orjson        | users.ndjson | 34.422   | 35.178      | 36.527   | 180.6    |
| msgspec       | users.ndjson | 35.338   | 37.192      | 37.332   | 187.8    |
| ujson         | users.ndjson | 46.597   | 52.718      | 53.362   | 186.8    |
| json (stdlib) | users.ndjson | 61.954   | 63.159      | 63.886   | 188.8    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 9.623    | 9.673       | 9.823    | 111.3    |
| orjson  | users.json | 10.445   | 10.470      | 10.515   | 133.6    |
| msgspec | users.json | 10.457   | 10.585      | 10.770   | 161.9    |
| ujson   | users.json | 39.363   | 39.383      | 39.528   | 167.7    |
| json    | users.json | 56.275   | 56.374      | 56.749   | 193.0    |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 10.511   | 10.624      | 10.662   | 199.3    |
| msgspec       | users.json | 11.306   | 11.520      | 11.860   | 245.9    |
| orjson        | users.json | 11.523   | 11.559      | 11.643   | 222.4    |
| ujson         | users.json | 41.120   | 41.512      | 41.701   | 223.6    |
| json (stdlib) | users.json | 418.894  | 420.798     | 423.309  | 245.9    |

## search (mem_eff=True)

| Query           | Library                      | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------------------- | -------- | ------- | -------- |
| all user ids    | orjson+jmespath (mem_eff)    | 28.943   | 2000    | 147.5    |
|                 | orjson+jsonpath-ng (mem_eff) | 30.702   | 2000    | 149.7    |
|                 | strata (mem_eff)             | 61.655   | 2000    | 127.1    |
| all item prices | strata (mem_eff)             | 66.269   | 100999  | 153.5    |
|                 | orjson+jmespath (mem_eff)    | 79.300   | 2000    | 160.7    |
|                 | orjson+jsonpath-ng (mem_eff) | 246.443  | 100999  | 166.5    |
| recursive price | strata (mem_eff)             | 67.925   | 100999  | 166.1    |
|                 | orjson+jsonpath-ng (mem_eff) | 837.421  | 100999  | 170.5    |

## search (mem_eff=False)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | orjson+jmespath    | 28.943   | 2000    | 147.5    |
|                 | strata             | 29.066   | 2000    | 246.0    |
|                 | orjson+jsonpath-ng | 30.702   | 2000    | 149.7    |
| all item prices | strata             | 34.059   | 100999  | 152.7    |
|                 | orjson+jmespath    | 79.300   | 2000    | 160.7    |
|                 | orjson+jsonpath-ng | 246.443  | 100999  | 166.5    |
| recursive price | strata             | 36.538   | 100999  | 166.1    |
|                 | orjson+jsonpath-ng | 837.421  | 100999  | 170.5    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.312    | 2000    | 198.0    |
|                                            | jmespath    | 1.020    | 2000    | 198.0    |
|                                            | jsonpath-ng | 16.239   | 2000    | 198.8    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 5.965    | 100999  | 198.8    |
|                                            | jmespath    | 59.290   | 2000    | 203.6    |
|                                            | jsonpath-ng | 229.100  | 100999  | 205.0    |
| $..price                                   | strata      | 8.062    | 100999  | 205.4    |
|                                            | jsonpath-ng | 829.829  | 100999  | 207.8    |
| $.users\[?(@.age>30)\]                     | strata      | 0.396    | 1606    | 207.2    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.050    | 6650    | 207.2    |

## Summary

| Category               | Strata Rank           | vs #1                             |
| ---------------------- | --------------------- | --------------------------------- |
| loads (JSON)           | **#1** / 5            | **23.7% faster** than #2 (orjson) |
| loads (NDJSON)         | **#1** / 5            | **37.9% faster** than #2 (orjson) |
| load (JSON file)       | **#1** / 5            | **10.8% faster** than #2 (orjson) |
| load (NDJSON file)     | **#1** / 5            | **46.3% faster** than #2 (orjson) |
| dumps                  | **#1** / 5            | **8.5% faster** than #2 (orjson)  |
| dump                   | **#1** / 5            | **7.6% faster** than #2 (msgspec) |
| search (mem_eff=True)  | #1 in 2/3 queries     | -                                 |
| search (mem_eff=False) | #1 in 2/3 queries     | -                                 |
| query (JSONPath)       | **#1** in 5/5 queries | -                                 |
