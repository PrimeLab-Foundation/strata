# Strata Benchmark Results — LARGE
Generated: 2026-02-19 00:27:13

## Environment
- Commit: 05380f98de509bd7234d23c1e7d23621356e3eda
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2
- Compiler: Apple clang version 17.0.0 (clang-1700.6.3.2)

## Configuration
- Repeat/Warmup (loads): 5 / 2
- Repeat/Warmup (dumps): 5 / 2
- Repeat/Warmup (search): 3 / 1
- Repeat/Warmup (cursor_reuse): 3 / 1

## Datasets
- json: benchmarks/data/generated/large/users.json (43.85 MB, 4000 records)
- ndjson: benchmarks/data/generated/large/users.ndjson (43.85 MB, 4000 records, 4000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (43.85 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 163.45   | 221.80      | 230.13   | 207.32 | 1046.9   |
| msgspec       | 176.04   | 237.91      | 253.01   | 193.28 | 1031.3   |
| ujson         | 228.07   | 307.89      | 312.08   | 149.35 | 1151.8   |
| json (stdlib) | 292.34   | 355.80      | 362.16   | 129.24 | 1031.3   |
| strata        | 372.18   | 403.00      | 405.66   | 114.10 | 906.9    |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 200.59   | 201.33      | 205.53   | 228.38 | 1638.2   |
| msgspec       | 201.95   | 209.20      | 213.14   | 219.78 | 1621.5   |
| strata        | 193.99   | 226.83      | 231.76   | 202.71 | 1627.0   |
| ujson         | 266.46   | 284.99      | 290.56   | 161.34 | 1745.0   |
| json (stdlib) | 317.66   | 325.10      | 333.42   | 141.43 | 1624.3   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 49.53    | 51.42       | 52.14    | 793.09 | 40779839       | 772.9    |
| msgspec | 54.51    | 55.35       | 56.00    | 736.76 | 40779839       | 1052.9   |
| ujson   | 184.21   | 185.51      | 189.67   | 219.83 | 40779839       | 1329.5   |
| json    | 290.36   | 293.69      | 296.19   | 156.57 | 45983395       | 1685.7   |
| strata  | 429.86   | 433.38      | 444.04   | 109.96 | 47655981       | 763.3    |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 43.39    | 43.80       | 44.93    | 931.04 | 40779839       | 754.6    |
| msgspec | 51.05    | 51.33       | 52.42    | 794.45 | 40779839       | 1004.5   |
| ujson   | 184.18   | 186.52      | 186.79   | 218.63 | 40779839       | 1313.8   |
| json    | 291.12   | 294.72      | 299.49   | 156.02 | 45983395       | 1670.1   |
| strata  | 425.78   | 427.78      | 430.44   | 111.40 | 47655981       | 762.6    |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 3135161.98 | 1       | 5361.8   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2378366.52 | 1       | 5361.8   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 1956739.85 | 1       | 5362.2   |
| Extract all user IDs                        | strata      | 0.58     | 0.60        | 0.61     | 76868.57   | 4000    | 4098.7   |
| Extract all user IDs                        | jmespath    | 1.88     | 1.94        | 1.94     | 23747.15   | 4000    | 3785.9   |
| Extract all user IDs                        | jsonpath-ng | 33.98    | 34.83       | 37.37    | 1320.17    | 4000    | 3750.2   |
| Extract all user names                      | strata      | 0.66     | 0.67        | 0.67     | 69031.18   | 4000    | 5362.1   |
| Extract all user names                      | jmespath    | 1.34     | 1.34        | 1.37     | 34306.37   | 4000    | 5362.4   |
| Extract all user names                      | jsonpath-ng | 32.56    | 35.01       | 35.68    | 1313.30    | 4000    | 5364.2   |
| Extract nested timestamp field              | strata      | 1.13     | 1.15        | 1.22     | 40084.31   | 4000    | 5227.5   |
| Extract nested timestamp field              | jmespath    | 2.71     | 2.75        | 2.99     | 16724.79   | 4000    | 5227.8   |
| Extract nested timestamp field              | jsonpath-ng | 35.51    | 37.48       | 40.34    | 1226.84    | 4000    | 5230.5   |
| Extract order item prices (double wildcard) | strata      | 52.85    | 53.58       | 57.49    | 858.29     | 789913  | 5415.6   |
| Extract order item prices (double wildcard) | jmespath    | 220.87   | 222.85      | 224.95   | 206.34     | 4000    | 5432.1   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1093.87  | 1115.37     | 1116.32  | 41.23      | 789913  | 5445.4   |
| Filter users by age (numeric predicate)     | strata      | 1.13     | 1.21        | 1.24     | 37860.69   | 3212    | 5362.7   |
| Filter users by age (numeric predicate)     | jmespath    | 4.93     | 5.02        | 5.37     | 9168.18    | 3212    | 5362.9   |
| Recursively find all prices                 | strata      | 111.87   | 116.89      | 118.06   | 393.41     | 789913  | 5398.3   |
| Recursively find all prices                 | jsonpath-ng | 3817.95  | 3841.74     | 3844.48  | 11.97      | 789913  | 5408.1   |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.04     | 3385366.06 | 10      | 5362.1   |
| Slice first 10 users, extract IDs           | jmespath    | 0.03     | 0.03        | 0.03     | 1601761.38 | 10      | 5362.1   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1491321.35 | 10      | 5362.5   |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 2966411.56 | 1       | 1470.5   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2252126.34 | 1       | 1470.5   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1899347.91 | 1       | 1470.8   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2189489.94 | 0       | 1505.2   |
| Extract all user IDs                        | jmespath    | 2.04     | 2.05        | 2.07     | 22426.68   | 4000    | 1505.2   |
| Extract all user IDs                        | strata      | 31.94    | 32.57       | 32.72    | 1411.67    | 4000    | 1473.3   |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1788520.61 | 0       | 1503.6   |
| Extract all user names                      | jmespath    | 1.89     | 1.95        | 1.98     | 23572.11   | 4000    | 1503.6   |
| Extract all user names                      | strata      | 30.78    | 30.80       | 31.05    | 1493.06    | 4000    | 1474.4   |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.02        | 0.02     | 2261210.02 | 0       | 1474.1   |
| Extract nested timestamp field              | jmespath    | 3.32     | 3.46        | 3.49     | 13294.44   | 4000    | 1474.0   |
| Extract nested timestamp field              | strata      | 31.14    | 32.01       | 35.02    | 1436.39    | 4000    | 1473.9   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2085987.87 | 0       | 1575.2   |
| Extract order item prices (double wildcard) | strata      | 70.15    | 71.27       | 74.47    | 645.17     | 789913  | 1520.0   |
| Extract order item prices (double wildcard) | jmespath    | 194.88   | 197.89      | 201.21   | 232.35     | 789913  | 1575.1   |
| Filter users by age (numeric predicate)     | jmespath    | 5.31     | 5.81        | 5.89     | 7914.91    | 3212    | 1473.1   |
| Filter users by age (numeric predicate)     | strata      | 29.49    | 35.10       | 35.16    | 1310.07    | 3212    | 1472.8   |
| NDJSON first record id                      | strata      | 0.02     | 0.02        | 0.03     | 2357913.25 | 1       | 1470.5   |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.03     | 2298971.77 | 1       | 1470.5   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1905882.78 | 1       | 1470.9   |
| Recursively find all prices                 | strata      | 110.19   | 112.96      | 114.44   | 407.06     | 789913  | 1520.1   |
| Recursively find all prices                 | jsonpath-ng | 3810.29  | 3872.47     | 3877.18  | 11.87      | 789913  | 1615.1   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.04     | 2021071.95 | 10      | 1470.0   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 2021071.95 | 10      | 1470.0   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1845378.50 | 10      | 1470.4   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 808.90   | 945.04      | 1084.87  | 389.26 | 6526.5   |
| strata_cursor_reparse | 10767.81 | 11118.00    | 11285.94 | 33.09  | 10099.6  |
- Speedup: 11.76x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 521.18   | 526.81      | 543.87   | 785.51 | 1071.7   |
| strata_cursor_reparse | 3634.74  | 3638.50     | 3671.96  | 113.73 | 2153.5   |
- Speedup: 6.91x

## Summary
- loads json: orjson (221.80 ms)
- loads ndjson: orjson (201.33 ms)
- dumps str: orjson (51.42 ms)
- dumps bytes: orjson (43.80 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (526.81 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 81.7% behind orjson
- Strata gap in loads/ndjson: 12.7% behind orjson
- Strata gap in dumps/str: 742.8% behind orjson
- Strata gap in dumps/bytes: 876.6% behind orjson
