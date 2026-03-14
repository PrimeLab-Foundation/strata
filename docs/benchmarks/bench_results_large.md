# Strata Benchmark Results

Generated: 2026-03-15 01:41:49

## Environment

- Commit: 85dcf578761e98ba791b90249e4985e5c20eda03
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/large/users.json, benchmarks/data/generated/large/users.ndjson

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 160.718  | 199.412     | 204.179  | 338.9    |
| orjson        | users.json   | 181.097  | 251.216     | 251.876  | 477.0    |
| msgspec       | users.json   | 194.838  | 267.436     | 278.241  | 467.5    |
| ujson         | users.json   | 276.938  | 369.959     | 373.580  | 590.9    |
| json (stdlib) | users.json   | 383.559  | 451.146     | 455.266  | 512.4    |
| strata        | users.ndjson | 164.636  | 202.343     | 209.814  | 909.8    |
| orjson        | users.ndjson | 232.745  | 288.045     | 290.223  | 958.2    |
| msgspec       | users.ndjson | 251.224  | 302.977     | 310.092  | 952.8    |
| ujson         | users.ndjson | 333.765  | 402.295     | 413.449  | 1075.2   |
| json (stdlib) | users.ndjson | 426.318  | 490.027     | 496.681  | 954.5    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 167.146  | 205.018     | 209.801  | 521.2    |
| orjson        | users.json   | 183.573  | 265.523     | 268.878  | 520.2    |
| msgspec       | users.json   | 205.690  | 273.035     | 277.777  | 556.4    |
| ujson         | users.json   | 283.414  | 376.961     | 382.479  | 678.8    |
| json (stdlib) | users.json   | 385.912  | 465.598     | 465.720  | 556.4    |
| strata        | users.ndjson | 170.672  | 209.779     | 211.578  | 960.3    |
| orjson        | users.ndjson | 240.540  | 288.449     | 299.715  | 960.4    |
| msgspec       | users.ndjson | 249.095  | 294.686     | 302.671  | 996.7    |
| ujson         | users.ndjson | 334.888  | 417.700     | 429.797  | 1119.2   |
| json (stdlib) | users.ndjson | 433.704  | 499.340     | 506.919  | 999.4    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 67.658   | 67.723      | 68.065   | 590.9    |
| msgspec | users.json | 75.695   | 75.913      | 78.491   | 942.8    |
| orjson  | users.json | 75.621   | 75.962      | 76.530   | 747.3    |
| ujson   | users.json | 282.157  | 282.246     | 286.007  | 1139.5   |
| json    | users.json | 403.727  | 405.994     | 408.543  | 1354.2   |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 80.569   | 89.392      | 100.080  | 1398.1   |
| msgspec       | users.json | 96.864   | 98.747      | 100.370  | 1686.9   |
| orjson        | users.json | 111.696  | 113.026     | 114.513  | 1398.2   |
| ujson         | users.json | 312.466  | 314.381     | 322.829  | 1531.2   |
| json (stdlib) | users.json | 2889.048 | 2909.354    | 2920.918 | 1510.0   |

## search (mem_eff=True)

| Query           | Library                      | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------------------- | -------- | ------- | -------- |
| all user ids    | orjson+jmespath (mem_eff)    | 194.709  | 4000    | 823.3    |
|                 | orjson+jsonpath-ng (mem_eff) | 196.883  | 4000    | 826.0    |
|                 | strata (mem_eff)             | 434.828  | 4000    | 684.1    |
| all item prices | strata (mem_eff)             | 462.977  | 789913  | 866.5    |
|                 | orjson+jmespath (mem_eff)    | 515.610  | 4000    | 923.4    |
|                 | orjson+jsonpath-ng (mem_eff) | 1803.340 | 789913  | 995.9    |
| recursive price | strata (mem_eff)             | 463.907  | 789913  | 992.6    |
|                 | orjson+jsonpath-ng (mem_eff) | 5998.844 | 789913  | 1000.0   |

## search (mem_eff=False)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 190.615  | 4000    | 1505.5   |
|                 | orjson+jmespath    | 194.709  | 4000    | 823.3    |
|                 | orjson+jsonpath-ng | 196.883  | 4000    | 826.0    |
| all item prices | strata             | 233.105  | 789913  | 874.0    |
|                 | orjson+jmespath    | 515.610  | 4000    | 923.4    |
|                 | orjson+jsonpath-ng | 1803.340 | 789913  | 995.9    |
| recursive price | strata             | 258.907  | 789913  | 1008.2   |
|                 | orjson+jsonpath-ng | 5998.844 | 789913  | 1000.0   |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.740    | 4000    | 1247.7   |
|                                            | jmespath    | 2.204    | 4000    | 1247.7   |
|                                            | jsonpath-ng | 76.902   | 4000    | 1249.4   |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 42.672   | 789913  | 1255.6   |
|                                            | jmespath    | 382.948  | 4000    | 1270.4   |
|                                            | jsonpath-ng | 1608.507 | 789913  | 1183.2   |
| $..price                                   | strata      | 57.651   | 789913  | 1198.8   |
|                                            | jsonpath-ng | 5882.251 | 789913  | 1109.4   |
| $.users\[?(@.age>30)\]                     | strata      | 0.833    | 3212    | 1127.1   |
| $..orders\[?(@.status=="shipped")\]        | strata      | 59.847   | 26406   | 1127.1   |

## Summary

| Category               | Strata Rank           | vs #1                              |
| ---------------------- | --------------------- | ---------------------------------- |
| loads (JSON)           | **#1** / 5            | **12.7% faster** than #2 (orjson)  |
| loads (NDJSON)         | **#1** / 5            | **41.4% faster** than #2 (orjson)  |
| load (JSON file)       | **#1** / 5            | **9.8% faster** than #2 (orjson)   |
| load (NDJSON file)     | **#1** / 5            | **40.9% faster** than #2 (orjson)  |
| dumps                  | **#1** / 5            | **11.8% faster** than #2 (orjson)  |
| dump                   | **#1** / 5            | **20.2% faster** than #2 (msgspec) |
| search (mem_eff=True)  | #1 in 2/3 queries     | -                                  |
| search (mem_eff=False) | **#1** in 3/3 queries | avg **780% faster**                |
| query (JSONPath)       | **#1** in 5/5 queries | -                                  |
