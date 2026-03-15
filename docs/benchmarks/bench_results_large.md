# Strata Benchmark Results

Generated: 2026-03-15 03:13:04

## Environment

- Commit: 5c264ca1e42ba846ee2425bce2f825e70fdfafb1
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
| strata        | users.json   | 173.749  | 215.763     | 233.018  | 339.9    |
| orjson        | users.json   | 225.717  | 269.062     | 304.222  | 477.9    |
| msgspec       | users.json   | 209.183  | 298.305     | 306.416  | 468.4    |
| ujson         | users.json   | 304.506  | 408.588     | 446.664  | 590.8    |
| json (stdlib) | users.json   | 399.320  | 483.732     | 489.611  | 512.3    |
| strata        | users.ndjson | 176.189  | 211.053     | 234.498  | 456.3    |
| msgspec       | users.ndjson | 278.458  | 331.498     | 355.841  | 490.1    |
| orjson        | users.ndjson | 264.522  | 337.204     | 361.956  | 497.6    |
| ujson         | users.ndjson | 359.939  | 460.294     | 460.385  | 612.6    |
| json (stdlib) | users.ndjson | 471.540  | 523.769     | 547.713  | 492.8    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 175.771  | 214.277     | 223.100  | 522.1    |
| orjson        | users.json   | 237.847  | 278.235     | 312.221  | 521.1    |
| msgspec       | users.json   | 218.852  | 309.500     | 312.367  | 554.0    |
| ujson         | users.json   | 288.529  | 422.624     | 550.170  | 676.5    |
| json (stdlib) | users.json   | 406.525  | 496.521     | 511.471  | 554.0    |
| strata        | users.ndjson | 181.655  | 224.526     | 225.518  | 498.7    |
| orjson        | users.ndjson | 262.518  | 321.362     | 338.531  | 496.8    |
| msgspec       | users.ndjson | 293.553  | 346.803     | 372.791  | 473.0    |
| json (stdlib) | users.ndjson | 456.398  | 532.352     | 542.682  | 475.6    |
| ujson         | users.ndjson | 374.395  | 538.735     | 573.203  | 656.6    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 72.981   | 73.385      | 75.723   | 588.6    |
| msgspec | users.json | 77.434   | 79.401      | 88.653   | 940.7    |
| orjson  | users.json | 78.867   | 79.689      | 82.467   | 745.3    |
| ujson   | users.json | 296.021  | 296.974     | 298.228  | 1137.6   |
| json    | users.json | 421.342  | 424.693     | 445.340  | 998.0    |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 77.038   | 81.364      | 87.191   | 1013.6   |
| orjson        | users.json | 111.097  | 111.255     | 111.744  | 1096.8   |
| msgspec       | users.json | 108.129  | 111.486     | 117.354  | 1403.3   |
| ujson         | users.json | 323.028  | 330.555     | 385.488  | 1247.5   |
| json (stdlib) | users.json | 2969.794 | 2996.199    | 3013.939 | 1073.2   |

## search (file-based)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 71.331   | 4000    | 856.3    |
|                 | orjson+jmespath    | 204.517  | 4000    | 1004.4   |
|                 | orjson+jsonpath-ng | 214.558  | 4000    | 1014.3   |
| all item prices | strata             | 80.583   | 789913  | 1040.5   |
|                 | orjson+jmespath    | 560.382  | 4000    | 1102.3   |
|                 | orjson+jsonpath-ng | 1970.401 | 789913  | 960.8    |
| recursive price | strata             | 85.900   | 789913  | 935.0    |
|                 | orjson+jsonpath-ng | 6256.548 | 789913  | 713.1    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.771    | 4000    | 958.5    |
|                                            | jmespath    | 2.216    | 4000    | 958.5    |
|                                            | jsonpath-ng | 79.503   | 4000    | 960.2    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 42.377   | 789913  | 967.0    |
|                                            | jmespath    | 426.988  | 4000    | 981.7    |
|                                            | jsonpath-ng | 1710.772 | 789913  | 791.3    |
| $..price                                   | strata      | 56.745   | 789913  | 812.5    |
|                                            | jsonpath-ng | 6161.584 | 789913  | 750.8    |
| $.users\[?(@.age>30)\]                     | strata      | 0.848    | 3212    | 700.3    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 61.069   | 26406   | 629.6    |

## Summary

| Category           | Strata Rank           | vs #1                              |
| ------------------ | --------------------- | ---------------------------------- |
| loads (JSON)       | **#1** / 5            | **20.4% faster** than #2 (msgspec) |
| loads (NDJSON)     | **#1** / 5            | **50.1% faster** than #2 (orjson)  |
| load (JSON file)   | **#1** / 5            | **24.5% faster** than #2 (msgspec) |
| load (NDJSON file) | **#1** / 5            | **44.5% faster** than #2 (orjson)  |
| dumps              | **#1** / 5            | **6.1% faster** than #2 (msgspec)  |
| dump               | **#1** / 5            | **40.4% faster** than #2 (msgspec) |
| search (JSONPath)  | **#1** in 3/3 queries | avg **2655% faster**               |
| query (JSONPath)   | **#1** in 5/5 queries | -                                  |
