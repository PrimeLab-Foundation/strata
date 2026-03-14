# Strata Benchmark Results

Generated: 2026-03-14 22:27:32

## Environment

- Commit: f56e5ee86d30b2c22a38d81d24f698617174fe82
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
| strata        | users.json   | 157.036  | 196.077     | 198.071  | 339.6    |
| orjson        | users.json   | 183.619  | 248.756     | 262.879  | 477.7    |
| msgspec       | users.json   | 193.867  | 267.857     | 270.884  | 468.2    |
| ujson         | users.json   | 267.404  | 364.538     | 366.440  | 590.5    |
| json (stdlib) | users.json   | 368.218  | 445.214     | 456.562  | 512.1    |
| orjson        | users.ndjson | 229.786  | 284.348     | 287.545  | 747.0    |
| msgspec       | users.ndjson | 245.466  | 294.256     | 303.718  | 741.6    |
| strata        | users.ndjson | 284.915  | 353.433     | 360.374  | 747.0    |
| ujson         | users.ndjson | 325.259  | 402.791     | 410.067  | 864.0    |
| json (stdlib) | users.ndjson | 423.532  | 479.982     | 485.220  | 744.3    |

## load (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| orjson        | users.json | 190.244  | 262.707     | 263.380  | 706.3    |
| msgspec       | users.json | 201.495  | 277.027     | 279.244  | 740.5    |
| ujson         | users.json | 271.511  | 371.561     | 384.671  | 863.0    |
| json (stdlib) | users.json | 377.414  | 454.230     | 470.559  | 740.5    |
| strata        | users.json | 407.149  | 468.247     | 492.266  | 548.4    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| orjson  | users.json | 75.789   | 75.852      | 76.900   | 967.6    |
| msgspec | users.json | 75.712   | 75.903      | 77.509   | 1163.8   |
| strata  | users.json | 86.539   | 88.169      | 95.766   | 811.0    |
| ujson   | users.json | 286.252  | 287.768     | 292.684  | 1363.7   |
| json    | users.json | 402.311  | 403.359     | 410.102  | 1564.9   |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| orjson        | users.json | 81.328   | 82.635      | 82.854   | 1764.6   |
| msgspec       | users.json | 86.599   | 92.099      | 105.954  | 2088.3   |
| strata        | users.json | 111.424  | 131.685     | 136.465  | 1608.8   |
| ujson         | users.json | 293.624  | 294.040     | 302.153  | 1920.4   |
| json (stdlib) | users.json | 2823.614 | 2837.322    | 2851.146 | 1950.4   |

## search (mem_eff=True)

| Query           | Library          | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------- | -------- | ------- | -------- |
| all user ids    | strata (mem_eff) | 229.371  | 4000    | 1728.5   |
| all item prices | strata (mem_eff) | 254.652  | 789913  | 1261.2   |
| recursive price | strata (mem_eff) | 249.469  | 789913  | 1511.6   |

## search (mem_eff=False)

| Query           | Library | Min (ms) | Results | RSS (MB) |
| --------------- | ------- | -------- | ------- | -------- |
| all user ids    | strata  | 227.213  | 4000    | 1728.5   |
| all item prices | strata  | 245.745  | 789913  | 1750.0   |
| recursive price | strata  | 438.718  | 789913  | 1547.0   |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.684    | 4000    | 1733.6   |
|                                            | jmespath    | 2.129    | 4000    | 1734.0   |
|                                            | jsonpath-ng | 75.782   | 4000    | 1737.3   |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 39.054   | 789913  | 1736.4   |
|                                            | jmespath    | 376.371  | 4000    | 1023.8   |
|                                            | jsonpath-ng | 1601.661 | 789913  | 1022.9   |
| $..price                                   | strata      | 56.205   | 789913  | 913.4    |
|                                            | jsonpath-ng | 5700.734 | 789913  | 912.5    |
| $.users\[?(@.age>30)\]                     | strata      | 0.807    | 3212    | 915.1    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 57.172   | 26406   | 915.1    |

## Summary

| Category              | Strata Rank                   | vs #1                             |
| --------------------- | ----------------------------- | --------------------------------- |
| Parsing (JSON)        | **#1** / 5                    | **16.9% faster** than #2 (orjson) |
| Parsing (NDJSON)      | **#3** / 5                    | 24.0% behind #1 (orjson)          |
| Load (file)           | **#5** / 5                    | 114.0% behind #1 (orjson)         |
| Serialization (dumps) | **#3** / 5                    | 14.3% behind #1 (msgspec)         |
| Dump (file)           | **#3** / 5                    | 37.0% behind #1 (orjson)          |
| Search (file)         | mem_eff faster in 1/3 queries | -                                 |
| JSONPath              | **#1** in 5/5 queries         | -                                 |
