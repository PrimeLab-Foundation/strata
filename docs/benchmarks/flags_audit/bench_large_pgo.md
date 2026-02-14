# Strata Benchmark Results — LARGE-PGO
Generated: 2026-02-14 22:12:41

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
| orjson        | 204.66   | 261.79      | 273.07   | 175.65 | 1399.2   |
| msgspec       | 212.93   | 276.65      | 281.04   | 166.21 | 1383.6   |
| strata        | 263.07   | 295.45      | 299.03   | 155.64 | 1261.2   |
| ujson         | 271.49   | 357.41      | 366.59   | 128.66 | 1505.1   |
| json (stdlib) | 340.22   | 399.60      | 405.12   | 115.07 | 1383.6   |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| strata        | 203.91   | 232.16      | 235.07   | 198.05 | 2093.4   |
| orjson        | 184.20   | 244.58      | 250.37   | 187.99 | 2095.7   |
| msgspec       | 190.15   | 253.58      | 258.56   | 181.32 | 2078.0   |
| ujson         | 247.56   | 329.91      | 333.93   | 139.37 | 2199.5   |
| json (stdlib) | 315.16   | 379.31      | 390.54   | 121.22 | 2080.8   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 47.28    | 48.72       | 48.91    | 836.95 | 40779839       | 2843.4   |
| msgspec | 58.29    | 59.81       | 75.57    | 681.79 | 40779839       | 3110.8   |
| ujson   | 194.15   | 197.04      | 197.86   | 206.97 | 40779839       | 3426.4   |
| strata  | 227.16   | 229.87      | 231.55   | 207.31 | 47655981       | 2531.9   |
| json    | 306.70   | 310.97      | 314.88   | 147.87 | 45983395       | 3759.3   |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 44.24    | 50.36       | 50.79    | 809.78 | 40779839       | 3905.8   |
| msgspec | 60.84    | 62.08       | 62.78    | 656.89 | 40779839       | 3838.4   |
| ujson   | 198.48   | 208.57      | 219.70   | 195.52 | 40779839       | 3095.9   |
| strata  | 225.23   | 227.88      | 241.02   | 209.12 | 47655981       | 3856.8   |
| json    | 304.95   | 305.58      | 308.81   | 150.48 | 45983395       | 3450.1   |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 2348127.37 | 1       | 9611.5   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 2207131.68 | 1       | 9582.2   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1827129.59 | 1       | 9580.2   |
| Extract all user IDs                        | jmespath    | 1.94     | 1.95        | 2.03     | 23564.61   | 4000    | 7055.8   |
| Extract all user IDs                        | strata      | 5.16     | 5.29        | 5.73     | 8689.30    | 4000    | 7092.6   |
| Extract all user IDs                        | jsonpath-ng | 103.31   | 104.53      | 135.95   | 439.92     | 4000    | 6521.4   |
| Extract all user names                      | jmespath    | 1.51     | 1.54        | 1.63     | 29818.20   | 4000    | 10826.9  |
| Extract all user names                      | strata      | 5.29     | 5.32        | 5.43     | 8649.53    | 4000    | 10826.9  |
| Extract all user names                      | jsonpath-ng | 104.45   | 104.77      | 105.04   | 438.92     | 4000    | 10697.4  |
| Extract nested timestamp field              | jmespath    | 2.97     | 3.00        | 3.20     | 15318.44   | 4000    | 8023.8   |
| Extract nested timestamp field              | strata      | 5.68     | 5.74        | 6.15     | 8008.84    | 4000    | 8023.7   |
| Extract nested timestamp field              | jsonpath-ng | 107.93   | 108.21      | 109.12   | 424.95     | 4000    | 8025.1   |
| Extract order item prices (double wildcard) | jmespath    | 303.44   | 303.98      | 307.98   | 151.27     | 4000    | 8774.5   |
| Extract order item prices (double wildcard) | strata      | 924.80   | 927.92      | 934.97   | 49.56      | 789913  | 8756.7   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1244.48  | 1262.94     | 1293.05  | 36.41      | 789913  | 8726.8   |
| Filter users by age (numeric predicate)     | strata      | 4.78     | 4.83        | 4.84     | 9528.26    | 3212    | 12331.1  |
| Filter users by age (numeric predicate)     | jmespath    | 5.47     | 5.49        | 5.68     | 8368.86    | 3212    | 12331.1  |
| Recursively find all prices                 | strata      | 1062.28  | 1081.43     | 1104.22  | 42.52      | 789913  | 12953.1  |
| Recursively find all prices                 | jsonpath-ng | 4183.42  | 4196.02     | 4199.23  | 10.96      | 789913  | 11323.2  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 1667091.28 | 10      | 12185.3  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.04        | 0.04     | 1271418.21 | 10      | 12298.2  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 1137724.55 | 10      | 12185.3  |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.08     | 2238310.17 | 1       | 11706.1  |
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.03     | 1809002.19 | 1       | 11734.1  |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1505496.67 | 1       | 11706.1  |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2224881.78 | 0       | 13498.8  |
| Extract all user IDs                        | jmespath    | 2.22     | 2.28        | 2.36     | 20140.26   | 4000    | 13507.7  |
| Extract all user IDs                        | strata      | 106.65   | 107.13      | 107.24   | 429.20     | 4000    | 13637.7  |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1854681.09 | 0       | 11468.1  |
| Extract all user names                      | jmespath    | 2.18     | 2.22        | 2.30     | 20689.30   | 4000    | 11474.2  |
| Extract all user names                      | strata      | 106.17   | 106.27      | 106.85   | 432.68     | 4000    | 11642.2  |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2097982.55 | 0       | 13196.8  |
| Extract nested timestamp field              | jmespath    | 3.72     | 3.88        | 3.90     | 11864.88   | 4000    | 13239.1  |
| Extract nested timestamp field              | strata      | 108.20   | 108.31      | 108.35   | 424.53     | 4000    | 13367.6  |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2261316.83 | 0       | 11761.4  |
| Extract order item prices (double wildcard) | jmespath    | 197.59   | 198.40      | 199.96   | 231.76     | 789913  | 11769.4  |
| Extract order item prices (double wildcard) | strata      | 1180.70  | 1182.67     | 1218.32  | 38.88      | 789913  | 11818.2  |
| Filter users by age (numeric predicate)     | jmespath    | 5.85     | 5.94        | 5.96     | 7744.49    | 3212    | 11389.8  |
| Filter users by age (numeric predicate)     | strata      | 102.68   | 106.23      | 106.67   | 432.84     | 3212    | 11407.0  |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 2318210.52 | 1       | 13822.1  |
| NDJSON first record id                      | strata      | 0.02     | 0.02        | 0.03     | 1963587.42 | 1       | 13869.8  |
| NDJSON first record id                      | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1592360.33 | 1       | 13734.0  |
| Recursively find all prices                 | strata      | 1222.15  | 1229.46     | 1232.46  | 37.40      | 789913  | 11234.2  |
| Recursively find all prices                 | jsonpath-ng | 4148.60  | 4163.62     | 4163.78  | 11.04      | 789913  | 10828.6  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 1915809.34 | 10      | 10811.4  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.04        | 0.04     | 1122598.50 | 10      | 10839.4  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.07     | 1070308.53 | 10      | 10811.4  |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 3028.86  | 3076.21     | 3129.20  | 119.58 | 12547.7  |
| strata_cursor_reparse | 11190.90 | 12034.70    | 12715.29 | 30.57  | 13570.5  |
- Speedup: 3.91x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2725.98  | 2738.49     | 2754.37  | 151.11 | 10823.7  |
| strata_cursor_reparse | 5680.34  | 5686.72     | 5725.40  | 72.77  | 10864.7  |
- Speedup: 2.08x

## Summary
- loads json: orjson (261.79 ms)
- loads ndjson: strata (232.16 ms)
- dumps str: orjson (48.72 ms)
- dumps bytes: orjson (50.36 ms)
- search: strata (0.02 ms)
- cursor reuse: strata_cursor_reuse (2738.49 ms)
- Rule 13: strata not #1 in: loads/json, dumps/str, dumps/bytes
- Strata gap in loads/json: 12.9% behind orjson
- Strata gap in dumps/str: 371.8% behind orjson
- Strata gap in dumps/bytes: 352.5% behind orjson
