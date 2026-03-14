# Strata Benchmark Results

Generated: 2026-03-15 00:52:20

## Environment

- Commit: 2aa556684ac83988a6fdc0519b66ed3431409436
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
| strata        | users.json   | 167.555  | 215.430     | 221.852  | 340.1    |
| orjson        | users.json   | 194.827  | 281.219     | 314.579  | 478.2    |
| msgspec       | users.json   | 213.070  | 298.320     | 312.435  | 468.7    |
| ujson         | users.json   | 294.472  | 411.873     | 460.198  | 591.0    |
| json (stdlib) | users.json   | 406.096  | 498.792     | 500.477  | 512.5    |
| msgspec       | users.ndjson | 270.616  | 318.926     | 330.517  | 920.1    |
| orjson        | users.ndjson | 256.230  | 331.344     | 332.233  | 927.6    |
| strata        | users.ndjson | 327.279  | 402.177     | 422.045  | 927.5    |
| ujson         | users.ndjson | 364.152  | 443.191     | 462.325  | 1042.5   |
| json (stdlib) | users.ndjson | 452.546  | 523.519     | 525.415  | 922.8    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 172.990  | 221.110     | 229.156  | 522.3    |
| orjson        | users.json   | 208.195  | 281.648     | 283.876  | 521.3    |
| msgspec       | users.json   | 214.214  | 309.267     | 312.622  | 554.3    |
| ujson         | users.json   | 319.182  | 409.454     | 600.962  | 676.8    |
| json (stdlib) | users.json   | 426.170  | 498.332     | 505.179  | 554.3    |
| strata        | users.ndjson | 183.824  | 219.160     | 221.559  | 927.6    |
| orjson        | users.ndjson | 260.986  | 317.699     | 323.224  | 926.8    |
| msgspec       | users.ndjson | 262.863  | 329.464     | 348.189  | 964.0    |
| ujson         | users.ndjson | 360.862  | 466.466     | 478.241  | 1086.5   |
| json (stdlib) | users.ndjson | 440.393  | 544.238     | 555.994  | 966.7    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 70.000   | 71.324      | 74.450   | 588.9    |
| orjson  | users.json | 80.550   | 80.859      | 82.427   | 746.9    |
| msgspec | users.json | 80.637   | 82.126      | 86.944   | 943.1    |
| ujson   | users.json | 291.798  | 292.015     | 296.763  | 1138.0   |
| json    | users.json | 420.101  | 421.189     | 424.801  | 1352.8   |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 77.874   | 84.892      | 95.920   | 1396.7   |
| orjson        | users.json | 104.146  | 106.913     | 110.466  | 1552.5   |
| msgspec       | users.json | 105.993  | 108.333     | 114.153  | 1864.1   |
| ujson         | users.json | 320.869  | 323.475     | 325.827  | 1708.2   |
| json (stdlib) | users.json | 2972.428 | 3011.833    | 3017.641 | 1297.6   |

## search (mem_eff=True)

| Query           | Library                      | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------------------- | -------- | ------- | -------- |
| all user ids    | orjson+jmespath (mem_eff)    | 204.837  | 4000    | 821.8    |
|                 | orjson+jsonpath-ng (mem_eff) | 207.328  | 4000    | 824.7    |
|                 | strata (mem_eff)             | 459.044  | 4000    | 682.5    |
| all item prices | strata (mem_eff)             | 486.645  | 789913  | 863.9    |
|                 | orjson+jmespath (mem_eff)    | 546.827  | 4000    | 920.8    |
|                 | orjson+jsonpath-ng (mem_eff) | 1938.000 | 789913  | 807.2    |
| recursive price | strata (mem_eff)             | 492.400  | 789913  | 927.4    |
|                 | orjson+jsonpath-ng (mem_eff) | 6322.092 | 789913  | 1027.3   |

## search (mem_eff=False)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 201.148  | 4000    | 1293.0   |
|                 | orjson+jmespath    | 204.837  | 4000    | 821.8    |
|                 | orjson+jsonpath-ng | 207.328  | 4000    | 824.7    |
| all item prices | strata             | 247.676  | 789913  | 875.3    |
|                 | orjson+jmespath    | 546.827  | 4000    | 920.8    |
|                 | orjson+jsonpath-ng | 1938.000 | 789913  | 807.2    |
| recursive price | strata             | 272.093  | 789913  | 943.0    |
|                 | orjson+jsonpath-ng | 6322.092 | 789913  | 1027.3   |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.727    | 4000    | 1275.0   |
|                                            | jmespath    | 2.238    | 4000    | 1275.0   |
|                                            | jsonpath-ng | 79.179   | 4000    | 1276.7   |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 49.214   | 789913  | 1276.8   |
|                                            | jmespath    | 396.388  | 4000    | 1291.6   |
|                                            | jsonpath-ng | 1720.318 | 789913  | 1186.4   |
| $..price                                   | strata      | 57.801   | 789913  | 1207.8   |
|                                            | jsonpath-ng | 6163.274 | 789913  | 1076.5   |
| $.users\[?(@.age>30)\]                     | strata      | 0.834    | 3212    | 1094.3   |
| $..orders\[?(@.status=="shipped")\]        | strata      | 62.213   | 26406   | 1094.3   |

## Summary

| Category              | Strata Rank                   | vs #1                             |
| --------------------- | ----------------------------- | --------------------------------- |
| Parsing (JSON)        | **#1** / 5                    | **16.3% faster** than #2 (orjson) |
| Parsing (NDJSON)      | **#3** / 5                    | 27.7% behind #1 (orjson)          |
| Load JSON (file)      | **#1** / 5                    | **20.4% faster** than #2 (orjson) |
| Load NDJSON (file)    | **#1** / 5                    | **42.0% faster** than #2 (orjson) |
| Serialization (dumps) | **#1** / 5                    | **15.1% faster** than #2 (orjson) |
| Dump (file)           | **#1** / 5                    | **33.7% faster** than #2 (orjson) |
| Search (file)         | mem_eff faster in 0/3 queries | -                                 |
| JSONPath              | **#1** in 5/5 queries         | -                                 |
