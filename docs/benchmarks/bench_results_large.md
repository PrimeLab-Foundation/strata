# Strata Benchmark Results — LARGE
Generated: 2026-02-19 17:27:52

## Environment
- Commit: d3e6e38abca804fbb6f76be6e473253e4d2eb6c0
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
| orjson        | 155.80   | 208.55      | 210.44   | 220.49 | 803.0    |
| msgspec       | 163.57   | 221.92      | 239.95   | 207.20 | 789.3    |
| ujson         | 216.33   | 292.24      | 297.97   | 157.35 | 913.9    |
| strata        | 287.61   | 312.91      | 316.23   | 146.95 | 662.0    |
| json (stdlib) | 276.54   | 337.88      | 346.48   | 136.10 | 833.3    |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 191.60   | 194.09      | 195.01   | 236.90 | 1669.9   |
| msgspec       | 191.56   | 198.33      | 202.52   | 231.83 | 1654.3   |
| strata        | 188.01   | 219.11      | 252.63   | 209.85 | 1664.7   |
| ujson         | 257.54   | 271.94      | 284.79   | 169.08 | 1777.0   |
| json (stdlib) | 305.60   | 315.58      | 331.68   | 145.70 | 1658.3   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 46.63    | 47.03       | 48.58    | 867.10 | 40779839       | 772.6    |
| msgspec | 52.39    | 53.44       | 54.35    | 763.16 | 40779839       | 1051.9   |
| ujson   | 183.59   | 186.34      | 193.19   | 218.85 | 40779839       | 1329.2   |
| json    | 290.50   | 292.60      | 294.29   | 157.15 | 45983395       | 1685.5   |
| strata  | 420.50   | 420.90      | 427.34   | 113.22 | 47655981       | 762.5    |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 42.85    | 43.84       | 44.93    | 930.30 | 40779839       | 754.4    |
| msgspec | 50.08    | 50.52       | 51.07    | 807.14 | 40779839       | 1005.0   |
| ujson   | 182.00   | 183.44      | 193.48   | 222.30 | 40779839       | 1313.4   |
| json    | 285.30   | 286.57      | 293.33   | 160.46 | 45983395       | 1669.6   |
| strata  | 418.93   | 424.31      | 426.40   | 112.32 | 47655981       | 762.6    |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 3715827.82 | 1       | 5361.7   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2234048.84 | 1       | 5361.7   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1556542.12 | 1       | 5362.1   |
| Extract all user IDs                        | strata      | 0.42     | 0.43        | 0.44     | 107574.06  | 4000    | 4168.2   |
| Extract all user IDs                        | jmespath    | 1.72     | 1.73        | 1.75     | 26535.90   | 4000    | 4151.7   |
| Extract all user IDs                        | jsonpath-ng | 32.84    | 33.14       | 34.07    | 1387.53    | 4000    | 4153.6   |
| Extract all user names                      | strata      | 0.55     | 0.64        | 0.96     | 72362.61   | 4000    | 5361.9   |
| Extract all user names                      | jmespath    | 1.36     | 1.37        | 1.38     | 33499.31   | 4000    | 5362.2   |
| Extract all user names                      | jsonpath-ng | 35.12    | 35.62       | 35.83    | 1290.97    | 4000    | 5364.1   |
| Extract nested timestamp field              | strata      | 0.95     | 0.99        | 1.56     | 46583.15   | 4000    | 5362.3   |
| Extract nested timestamp field              | jmespath    | 2.57     | 2.63        | 2.68     | 17480.86   | 4000    | 5362.6   |
| Extract nested timestamp field              | jsonpath-ng | 36.86    | 38.05       | 41.05    | 1208.50    | 4000    | 5365.6   |
| Extract order item prices (double wildcard) | strata      | 48.85    | 49.23       | 55.54    | 934.07     | 789913  | 5418.6   |
| Extract order item prices (double wildcard) | jmespath    | 219.76   | 219.80      | 222.45   | 209.20     | 4000    | 5435.0   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1118.03  | 1129.08     | 1148.60  | 40.73      | 789913  | 4865.0   |
| Filter users by age (numeric predicate)     | strata      | 0.73     | 0.83        | 1.38     | 55686.82   | 3212    | 5362.0   |
| Filter users by age (numeric predicate)     | jmespath    | 4.92     | 5.10        | 5.29     | 9021.51    | 3212    | 5362.2   |
| Recursively find all prices                 | strata      | 109.57   | 110.41      | 112.70   | 416.48     | 789913  | 5398.0   |
| Recursively find all prices                 | jsonpath-ng | 3983.80  | 4013.44     | 4043.26  | 11.46      | 789913  | 4453.0   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.04     | 2718343.61 | 10      | 5361.9   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 1692683.03 | 10      | 4990.3   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1367536.15 | 10      | 4966.1   |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 3039965.37 | 1       | 1428.5   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2247391.20 | 1       | 1428.5   |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1811996.18 | 1       | 1428.9   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2207047.07 | 0       | 1504.4   |
| Extract all user IDs                        | jmespath    | 2.04     | 2.04        | 2.05     | 22517.75   | 4000    | 1504.4   |
| Extract all user IDs                        | strata      | 31.39    | 33.80       | 35.28    | 1360.14    | 4000    | 1472.3   |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.02        | 0.02     | 2270589.15 | 0       | 1463.9   |
| Extract all user names                      | jmespath    | 1.92     | 1.97        | 2.00     | 23299.38   | 4000    | 1463.9   |
| Extract all user names                      | strata      | 30.41    | 31.66       | 33.34    | 1452.45    | 4000    | 1433.0   |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2117985.95 | 0       | 1474.9   |
| Extract nested timestamp field              | jmespath    | 3.15     | 3.16        | 3.23     | 14534.53   | 4000    | 1474.9   |
| Extract nested timestamp field              | strata      | 33.72    | 35.91       | 52.86    | 1280.50    | 4000    | 1474.5   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2215869.49 | 0       | 1535.9   |
| Extract order item prices (double wildcard) | strata      | 65.56    | 67.91       | 69.83    | 677.04     | 789913  | 1480.2   |
| Extract order item prices (double wildcard) | jmespath    | 185.91   | 186.61      | 191.48   | 246.39     | 789913  | 1535.8   |
| Filter users by age (numeric predicate)     | jmespath    | 5.33     | 5.49        | 5.76     | 8368.38    | 3212    | 1432.1   |
| Filter users by age (numeric predicate)     | strata      | 30.49    | 32.30       | 34.48    | 1423.50    | 3212    | 1431.9   |
| NDJSON first record id                      | strata      | 0.01     | 0.02        | 0.02     | 2990534.57 | 1       | 1428.8   |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.03     | 1967029.74 | 1       | 1428.8   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1956569.17 | 1       | 1429.2   |
| Recursively find all prices                 | strata      | 109.41   | 110.79      | 111.42   | 415.01     | 789913  | 1480.7   |
| Recursively find all prices                 | jsonpath-ng | 3865.92  | 3931.00     | 3951.59  | 11.70      | 789913  | 1574.3   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 2062593.81 | 10      | 1469.9   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 1879852.84 | 10      | 1469.9   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.05     | 1222042.63 | 10      | 1470.2   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 921.56   | 1114.45     | 1156.42  | 330.09 | 6907.1   |
| strata_cursor_reparse | 10596.95 | 11807.96    | 12615.48 | 31.15  | 9568.0   |
- Speedup: 10.60x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 537.99   | 565.31      | 571.14   | 732.01 | 1058.6   |
| strata_cursor_reparse | 3635.64  | 3702.58     | 3708.05  | 111.76 | 2153.2   |
- Speedup: 6.55x

## Summary
- loads json: orjson (208.55 ms)
- loads ndjson: orjson (194.09 ms)
- dumps str: orjson (47.03 ms)
- dumps bytes: orjson (43.84 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (565.31 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 50.0% behind orjson
- Strata gap in loads/ndjson: 12.9% behind orjson
- Strata gap in dumps/str: 795.0% behind orjson
- Strata gap in dumps/bytes: 868.0% behind orjson
