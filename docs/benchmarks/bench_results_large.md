# Strata Benchmark Results

Generated: 2026-03-15 02:07:28

## Environment

- Commit: ae530e0c1e5e4d01eab91b20240642c5ccc9faba
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
| strata        | users.json   | 156.517  | 202.527     | 208.268  | 338.6    |
| orjson        | users.json   | 182.106  | 250.660     | 251.002  | 476.7    |
| msgspec       | users.json   | 190.068  | 267.322     | 267.326  | 467.1    |
| ujson         | users.json   | 258.853  | 357.876     | 363.073  | 590.5    |
| json (stdlib) | users.json   | 373.010  | 461.494     | 474.647  | 512.0    |
| strata        | users.ndjson | 166.135  | 202.782     | 207.801  | 719.5    |
| orjson        | users.ndjson | 235.316  | 285.174     | 293.924  | 767.8    |
| msgspec       | users.ndjson | 246.623  | 299.281     | 306.620  | 762.3    |
| ujson         | users.ndjson | 335.485  | 416.360     | 422.798  | 884.8    |
| json (stdlib) | users.ndjson | 431.368  | 487.352     | 504.617  | 557.0    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 167.900  | 206.024     | 211.278  | 520.9    |
| orjson        | users.json   | 187.226  | 257.946     | 273.766  | 518.9    |
| msgspec       | users.json   | 205.378  | 275.237     | 279.583  | 556.0    |
| ujson         | users.json   | 283.267  | 377.023     | 390.728  | 678.5    |
| json (stdlib) | users.json   | 388.707  | 455.323     | 461.099  | 556.0    |
| strata        | users.ndjson | 167.946  | 207.071     | 213.036  | 562.8    |
| orjson        | users.ndjson | 231.412  | 283.108     | 291.658  | 606.8    |
| msgspec       | users.ndjson | 244.048  | 295.259     | 302.642  | 643.1    |
| ujson         | users.ndjson | 335.847  | 412.902     | 419.312  | 765.5    |
| json (stdlib) | users.ndjson | 428.094  | 478.935     | 485.170  | 645.8    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 67.466   | 67.664      | 68.105   | 590.5    |
| msgspec | users.json | 74.266   | 74.349      | 74.772   | 942.8    |
| orjson  | users.json | 75.283   | 75.423      | 75.605   | 747.3    |
| ujson   | users.json | 278.448  | 278.910     | 279.708  | 1137.7   |
| json    | users.json | 395.960  | 396.931     | 398.250  | 1352.4   |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 73.064   | 78.411      | 79.862   | 1396.3   |
| orjson        | users.json | 81.027   | 82.030      | 101.631  | 1552.0   |
| msgspec       | users.json | 99.124   | 101.032     | 102.276  | 1863.4   |
| ujson         | users.json | 310.629  | 313.261     | 317.202  | 1707.7   |
| json (stdlib) | users.json | 2854.819 | 2869.958    | 2870.673 | 1686.6   |

## search (mem_eff=True)

| Query           | Library                      | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------------------- | -------- | ------- | -------- |
| all user ids    | strata (mem_eff)             | 68.765   | 4000    | 1465.6   |
|                 | orjson+jmespath (mem_eff)    | 193.369  | 4000    | 1818.0   |
|                 | orjson+jsonpath-ng (mem_eff) | 198.846  | 4000    | 1827.0   |
| all item prices | strata (mem_eff)             | 75.965   | 789913  | 1859.1   |
|                 | orjson+jmespath (mem_eff)    | 504.225  | 4000    | 1415.0   |
|                 | orjson+jsonpath-ng (mem_eff) | 1751.003 | 789913  | 1438.8   |
| recursive price | strata (mem_eff)             | 75.317   | 789913  | 936.9    |
|                 | orjson+jsonpath-ng (mem_eff) | 5828.889 | 789913  | 711.0    |

## search (mem_eff=False)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 68.463   | 4000    | 1465.6   |
|                 | orjson+jmespath    | 193.369  | 4000    | 1818.0   |
|                 | orjson+jsonpath-ng | 198.846  | 4000    | 1827.0   |
| all item prices | strata             | 75.971   | 789913  | 1851.2   |
|                 | orjson+jmespath    | 504.225  | 4000    | 1415.0   |
|                 | orjson+jsonpath-ng | 1751.003 | 789913  | 1438.8   |
| recursive price | strata             | 74.743   | 789913  | 936.9    |
|                 | orjson+jsonpath-ng | 5828.889 | 789913  | 711.0    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.695    | 4000    | 938.5    |
|                                            | jmespath    | 2.109    | 4000    | 938.5    |
|                                            | jsonpath-ng | 76.156   | 4000    | 940.3    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 41.173   | 789913  | 948.3    |
|                                            | jmespath    | 373.169  | 4000    | 963.2    |
|                                            | jsonpath-ng | 1580.352 | 789913  | 847.5    |
| $..price                                   | strata      | 56.121   | 789913  | 862.9    |
|                                            | jsonpath-ng | 5906.358 | 789913  | 962.9    |
| $.users\[?(@.age>30)\]                     | strata      | 0.819    | 3212    | 980.6    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 60.283   | 26406   | 979.6    |

## Summary

| Category               | Strata Rank           | vs #1                              |
| ---------------------- | --------------------- | ---------------------------------- |
| loads (JSON)           | **#1** / 5            | **16.3% faster** than #2 (orjson)  |
| loads (NDJSON)         | **#1** / 5            | **41.6% faster** than #2 (orjson)  |
| load (JSON file)       | **#1** / 5            | **11.5% faster** than #2 (orjson)  |
| load (NDJSON file)     | **#1** / 5            | **37.8% faster** than #2 (orjson)  |
| dumps                  | **#1** / 5            | **10.1% faster** than #2 (msgspec) |
| dump                   | **#1** / 5            | **10.9% faster** than #2 (orjson)  |
| search (mem_eff=True)  | **#1** in 3/3 queries | avg **2795% faster**               |
| search (mem_eff=False) | **#1** in 3/3 queries | avg **2815% faster**               |
| query (JSONPath)       | **#1** in 5/5 queries | -                                  |
