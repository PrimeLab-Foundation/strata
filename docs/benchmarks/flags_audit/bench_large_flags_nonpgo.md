# Strata Benchmark Results — LARGE-FLAGS-NONPGO
Generated: 2026-02-14 21:21:05

## Environment
- Commit: ce4d80c0cfcd8bbfe70be79dc3e5b1150ad13bb4
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
| orjson        | 204.25   | 264.71      | 270.55   | 173.71 | 1400.6   |
| msgspec       | 213.18   | 279.86      | 284.03   | 164.31 | 1384.0   |
| ujson         | 273.18   | 356.46      | 359.32   | 129.00 | 1505.6   |
| strata        | 342.20   | 372.59      | 375.68   | 123.41 | 1262.6   |
| json (stdlib) | 337.44   | 396.03      | 401.30   | 116.11 | 1384.1   |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| strata        | 202.76   | 229.77      | 234.21   | 200.11 | 2135.7   |
| orjson        | 180.71   | 244.59      | 245.62   | 187.99 | 2136.9   |
| msgspec       | 186.25   | 250.22      | 251.39   | 183.75 | 2118.2   |
| ujson         | 247.26   | 328.69      | 330.81   | 139.89 | 2239.9   |
| json (stdlib) | 307.91   | 377.38      | 391.47   | 121.84 | 2121.1   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 47.51    | 47.74       | 49.08    | 854.14 | 40779839       | 2884.5   |
| msgspec | 57.87    | 59.78       | 74.47    | 682.20 | 40779839       | 3197.7   |
| ujson   | 193.87   | 194.89      | 195.48   | 209.24 | 40779839       | 3513.3   |
| strata  | 234.61   | 236.63      | 240.18   | 201.39 | 47655981       | 2572.2   |
| json    | 313.21   | 313.55      | 314.23   | 146.66 | 45983395       | 3846.4   |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 49.40    | 51.01       | 51.20    | 799.45 | 40779839       | 3946.1   |
| msgspec | 60.42    | 61.36       | 62.43    | 664.55 | 40779839       | 3880.0   |
| ujson   | 196.10   | 205.86      | 208.12   | 198.09 | 40779839       | 3300.8   |
| strata  | 232.74   | 234.11      | 234.70   | 203.57 | 47655981       | 3934.0   |
| json    | 308.15   | 309.80      | 310.86   | 148.43 | 45983395       | 3651.1   |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 2318528.42 | 1       | 9533.2   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.04     | 1915974.14 | 1       | 9581.3   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1721710.34 | 1       | 9515.0   |
| Extract all user IDs                        | jmespath    | 1.99     | 2.10        | 16.35    | 21870.82   | 4000    | 7714.1   |
| Extract all user IDs                        | strata      | 5.88     | 5.94        | 5.98     | 7747.78    | 4000    | 8533.7   |
| Extract all user IDs                        | jsonpath-ng | 103.55   | 104.83      | 105.80   | 438.66     | 4000    | 7715.9   |
| Extract all user names                      | jmespath    | 1.50     | 1.53        | 1.59     | 30106.99   | 4000    | 11204.7  |
| Extract all user names                      | strata      | 5.28     | 5.30        | 5.37     | 8671.34    | 4000    | 11322.2  |
| Extract all user names                      | jsonpath-ng | 102.49   | 103.83      | 104.11   | 442.86     | 4000    | 11204.8  |
| Extract nested timestamp field              | jmespath    | 2.87     | 3.02        | 3.06     | 15227.55   | 4000    | 8472.1   |
| Extract nested timestamp field              | strata      | 5.92     | 6.08        | 6.56     | 7557.83    | 4000    | 8472.0   |
| Extract nested timestamp field              | jsonpath-ng | 107.34   | 108.03      | 108.71   | 425.64     | 4000    | 8473.4   |
| Extract order item prices (double wildcard) | jmespath    | 302.77   | 304.06      | 305.52   | 151.23     | 4000    | 8823.0   |
| Extract order item prices (double wildcard) | strata      | 959.95   | 1022.80     | 1034.82  | 44.96      | 789913  | 9140.2   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1258.76  | 1259.65     | 1259.71  | 36.50      | 789913  | 8604.6   |
| Filter users by age (numeric predicate)     | strata      | 4.45     | 4.65        | 4.76     | 9879.87    | 3212    | 12364.4  |
| Filter users by age (numeric predicate)     | jmespath    | 5.50     | 5.59        | 5.85     | 8231.90    | 3212    | 12334.1  |
| Recursively find all prices                 | strata      | 988.48   | 990.09      | 995.49   | 46.44      | 789913  | 12829.4  |
| Recursively find all prices                 | jsonpath-ng | 4192.35  | 4232.93     | 4319.28  | 10.86      | 789913  | 11779.3  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 1984866.34 | 10      | 12146.6  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.04     | 1342580.92 | 10      | 12169.6  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.04     | 0.04        | 0.04     | 1069381.82 | 10      | 12125.5  |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2138574.19 | 1       | 12506.5  |
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.03     | 1713538.44 | 1       | 12669.6  |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1618079.68 | 1       | 12496.8  |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1932556.24 | 0       | 13892.0  |
| Extract all user IDs                        | jmespath    | 2.22     | 2.24        | 2.57     | 20494.86   | 4000    | 13983.2  |
| Extract all user IDs                        | strata      | 105.75   | 107.43      | 107.58   | 427.98     | 4000    | 14301.4  |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2054944.93 | 0       | 11521.6  |
| Extract all user names                      | jmespath    | 2.23     | 2.31        | 2.34     | 19870.45   | 4000    | 12356.3  |
| Extract all user names                      | strata      | 106.36   | 106.67      | 106.74   | 431.03     | 4000    | 12500.9  |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2151083.49 | 0       | 12832.9  |
| Extract nested timestamp field              | jmespath    | 3.55     | 3.55        | 3.63     | 12936.30   | 4000    | 13432.1  |
| Extract nested timestamp field              | strata      | 102.78   | 103.48      | 107.70   | 444.34     | 4000    | 13502.9  |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2047349.91 | 0       | 12672.1  |
| Extract order item prices (double wildcard) | jmespath    | 196.04   | 197.18      | 197.21   | 233.19     | 789913  | 12704.7  |
| Extract order item prices (double wildcard) | strata      | 1058.49  | 1132.30     | 1175.24  | 40.61      | 789913  | 12736.8  |
| Filter users by age (numeric predicate)     | jmespath    | 5.89     | 5.94        | 6.19     | 7744.28    | 3212    | 11518.1  |
| Filter users by age (numeric predicate)     | strata      | 106.26   | 106.29      | 106.48   | 432.59     | 3212    | 11518.1  |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2261320.06 | 1       | 14434.6  |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 2189496.01 | 1       | 14529.6  |
| NDJSON first record id                      | strata      | 0.02     | 0.04        | 0.04     | 1167730.99 | 1       | 14626.0  |
| Recursively find all prices                 | strata      | 1127.19  | 1175.13     | 1198.56  | 39.13      | 789913  | 11528.3  |
| Recursively find all prices                 | jsonpath-ng | 4198.24  | 4202.12     | 4205.40  | 10.94      | 789913  | 11069.6  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.04     | 1759976.74 | 10      | 11049.0  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 1246898.90 | 10      | 11049.0  |
| Slice first 10 users, extract IDs           | strata      | 0.04     | 0.04        | 0.04     | 1103496.56 | 10      | 11077.0  |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2813.40  | 2915.88     | 3105.98  | 126.16 | 13495.0  |
| strata_cursor_reparse | 11416.68 | 11478.29    | 12618.16 | 32.05  | 14504.3  |
- Speedup: 3.94x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2762.06  | 2762.97     | 2771.22  | 149.77 | 10985.0  |
| strata_cursor_reparse | 6196.69  | 6201.07     | 6203.87  | 66.73  | 10489.5  |
- Speedup: 2.24x

## Summary
- loads json: orjson (264.71 ms)
- loads ndjson: strata (229.77 ms)
- dumps str: orjson (47.74 ms)
- dumps bytes: orjson (51.01 ms)
- search: jmespath (0.02 ms)
- cursor reuse: strata_cursor_reuse (2762.97 ms)
- Rule 13: strata not #1 in: loads/json, dumps/str, dumps/bytes
- Strata gap in loads/json: 40.8% behind orjson
- Strata gap in dumps/str: 395.6% behind orjson
- Strata gap in dumps/bytes: 358.9% behind orjson
