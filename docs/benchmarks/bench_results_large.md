# Strata Benchmark Results

Generated: 2026-03-15 01:26:05

## Environment

- Commit: ad401a8927f1d1bf16672f43d7402ae108e7d54a
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
| strata        | users.json   | 169.958  | 215.855     | 232.597  | 339.9    |
| orjson        | users.json   | 192.644  | 274.550     | 307.448  | 478.0    |
| msgspec       | users.json   | 230.107  | 302.318     | 311.421  | 468.5    |
| ujson         | users.json   | 280.134  | 419.802     | 453.966  | 590.8    |
| json (stdlib) | users.json   | 400.061  | 481.945     | 488.022  | 512.4    |
| strata        | users.ndjson | 171.852  | 212.935     | 217.500  | 576.8    |
| orjson        | users.ndjson | 258.337  | 307.334     | 315.335  | 625.2    |
| msgspec       | users.ndjson | 275.776  | 367.446     | 418.355  | 614.2    |
| ujson         | users.ndjson | 406.252  | 468.363     | 486.241  | 736.7    |
| json (stdlib) | users.ndjson | 450.853  | 513.070     | 533.811  | 615.9    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 175.310  | 221.366     | 238.403  | 522.2    |
| orjson        | users.json   | 199.716  | 271.342     | 274.435  | 521.2    |
| msgspec       | users.json   | 224.182  | 297.135     | 298.709  | 554.1    |
| ujson         | users.json   | 303.028  | 412.430     | 456.908  | 676.6    |
| json (stdlib) | users.json   | 396.855  | 477.598     | 489.693  | 554.1    |
| strata        | users.ndjson | 179.558  | 214.033     | 227.737  | 620.7    |
| msgspec       | users.ndjson | 283.916  | 321.379     | 343.473  | 658.1    |
| orjson        | users.ndjson | 277.967  | 329.733     | 355.176  | 620.8    |
| ujson         | users.ndjson | 375.803  | 456.821     | 465.858  | 780.6    |
| json (stdlib) | users.ndjson | 474.962  | 510.924     | 542.522  | 660.8    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 69.976   | 70.905      | 74.917   | 588.7    |
| msgspec | users.json | 76.252   | 78.061      | 83.400   | 941.2    |
| orjson  | users.json | 79.284   | 80.291      | 86.723   | 745.6    |
| ujson   | users.json | 289.827  | 291.062     | 293.691  | 1138.1   |
| json    | users.json | 416.704  | 417.802     | 426.109  | 1352.8   |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 74.441   | 85.174      | 89.555   | 1396.7   |
| orjson        | users.json | 101.458  | 110.008     | 112.937  | 1552.5   |
| msgspec       | users.json | 109.368  | 112.760     | 128.581  | 1863.8   |
| ujson         | users.json | 320.620  | 323.313     | 324.562  | 1708.2   |
| json (stdlib) | users.json | 2969.520 | 2970.997    | 2992.405 | 1258.9   |

## search (mem_eff=True)

| Query           | Library                      | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------------------- | -------- | ------- | -------- |
| all user ids    | orjson+jsonpath-ng (mem_eff) | 210.008  | 4000    | 824.5    |
|                 | orjson+jmespath (mem_eff)    | 220.341  | 4000    | 821.7    |
|                 | strata (mem_eff)             | 452.502  | 4000    | 682.6    |
| all item prices | strata (mem_eff)             | 468.788  | 789913  | 860.9    |
|                 | orjson+jmespath (mem_eff)    | 532.110  | 4000    | 919.8    |
|                 | orjson+jsonpath-ng (mem_eff) | 1980.088 | 789913  | 805.6    |
| recursive price | strata (mem_eff)             | 478.146  | 789913  | 925.8    |
|                 | orjson+jsonpath-ng (mem_eff) | 6274.727 | 789913  | 941.0    |

## search (mem_eff=False)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 202.887  | 4000    | 1254.4   |
|                 | orjson+jsonpath-ng | 210.008  | 4000    | 824.5    |
|                 | orjson+jmespath    | 220.341  | 4000    | 821.7    |
| all item prices | strata             | 251.289  | 789913  | 871.0    |
|                 | orjson+jmespath    | 532.110  | 4000    | 919.8    |
|                 | orjson+jsonpath-ng | 1980.088 | 789913  | 805.6    |
| recursive price | strata             | 285.191  | 789913  | 941.4    |
|                 | orjson+jsonpath-ng | 6274.727 | 789913  | 941.0    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.736    | 4000    | 1147.4   |
|                                            | jmespath    | 2.150    | 4000    | 1147.4   |
|                                            | jsonpath-ng | 93.821   | 4000    | 1129.9   |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 44.410   | 789913  | 1130.0   |
|                                            | jmespath    | 417.539  | 4000    | 1035.7   |
|                                            | jsonpath-ng | 1732.960 | 789913  | 827.9    |
| $..price                                   | strata      | 58.628   | 789913  | 841.2    |
|                                            | jsonpath-ng | 6171.385 | 789913  | 835.9    |
| $.users\[?(@.age>30)\]                     | strata      | 0.848    | 3212    | 813.7    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 63.451   | 26406   | 812.7    |

## Summary

| Category              | Strata Rank                   | vs #1                             |
| --------------------- | ----------------------------- | --------------------------------- |
| Parsing (JSON)        | **#1** / 5                    | **13.3% faster** than #2 (orjson) |
| Parsing (NDJSON)      | **#1** / 5                    | **50.3% faster** than #2 (orjson) |
| Load JSON (file)      | **#1** / 5                    | **13.9% faster** than #2 (orjson) |
| Load NDJSON (file)    | **#1** / 5                    | **54.8% faster** than #2 (orjson) |
| Serialization (dumps) | **#1** / 5                    | **9.0% faster** than #2 (msgspec) |
| Dump (file)           | **#1** / 5                    | **36.3% faster** than #2 (orjson) |
| Search (file)         | mem_eff faster in 0/3 queries | -                                 |
| JSONPath              | **#1** in 5/5 queries         | -                                 |
