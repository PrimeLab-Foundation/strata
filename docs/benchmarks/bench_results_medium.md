# Strata Benchmark Results — MEDIUM
Generated: 2026-02-14 18:14:53

## Environment
- Commit: e963a9bbd1de10111d9958909b64228f733ac423
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
- json: benchmarks/data/generated/medium/users.json (6.25 MB, 2000 records)
- ndjson: benchmarks/data/generated/medium/users.ndjson (6.25 MB, 2000 records, 2000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (6.25 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 28.62    | 37.66       | 38.89    | 173.95 | 232.2    |
| msgspec       | 30.39    | 39.24       | 41.61    | 166.97 | 230.0    |
| strata        | 42.45    | 46.70       | 47.49    | 140.30 | 212.3    |
| ujson         | 37.60    | 50.88       | 51.27    | 128.77 | 246.8    |
| json (stdlib) | 44.87    | 55.12       | 55.55    | 118.85 | 230.1    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 24.78    | 34.66       | 35.29    | 188.99 | 251.7    |
| msgspec       | 25.29    | 35.59       | 36.01    | 184.02 | 251.7    |
| ujson         | 32.81    | 45.58       | 46.16    | 143.69 | 267.7    |
| strata        | 44.77    | 49.04       | 50.37    | 133.55 | 251.0    |
| json (stdlib) | 41.18    | 51.61       | 52.52    | 126.92 | 256.8    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.78     | 7.17        | 7.51     | 810.74 | 5811025        | 345.2    |
| msgspec | 7.33     | 7.42        | 7.44     | 783.50 | 5811025        | 390.4    |
| ujson   | 25.33    | 25.36       | 25.87    | 229.12 | 5811025        | 390.6    |
| strata  | 29.11    | 29.43       | 29.58    | 227.33 | 6690995        | 344.0    |
| json    | 40.29    | 40.64       | 40.91    | 161.19 | 6551664        | 440.8    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.10     | 6.34        | 6.48     | 915.99 | 5811025        | 489.4    |
| msgspec | 6.97     | 7.02        | 7.14     | 828.16 | 5811025        | 534.0    |
| ujson   | 25.24    | 25.52       | 26.21    | 227.75 | 5811025        | 536.0    |
| strata  | 29.11    | 29.33       | 29.55    | 228.13 | 6690995        | 492.1    |
| json    | 40.38    | 40.55       | 40.76    | 161.55 | 6551664        | 586.1    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 447976.29 | 1       | 1655.3   |
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 385391.70 | 1       | 1655.3   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 281283.88 | 1       | 1655.4   |
| Extract all user IDs                        | jmespath    | 0.71     | 0.72        | 0.78     | 9142.39   | 2000    | 1018.2   |
| Extract all user IDs                        | strata      | 2.41     | 2.50        | 2.83     | 2617.52   | 2000    | 1018.1   |
| Extract all user IDs                        | jsonpath-ng | 17.49    | 17.63       | 17.85    | 371.65    | 2000    | 1018.2   |
| Extract all user names                      | jmespath    | 0.56     | 0.58        | 0.58     | 11362.90  | 2000    | 1802.2   |
| Extract all user names                      | strata      | 3.12     | 3.20        | 3.38     | 2046.89   | 2000    | 1802.2   |
| Extract all user names                      | jsonpath-ng | 17.51    | 17.60       | 17.71    | 372.22    | 2000    | 1802.2   |
| Extract nested timestamp field              | jmespath    | 1.28     | 1.29        | 1.30     | 5077.66   | 2000    | 1162.2   |
| Extract nested timestamp field              | strata      | 2.74     | 2.76        | 2.79     | 2374.04   | 2000    | 1162.1   |
| Extract nested timestamp field              | jsonpath-ng | 18.82    | 18.87       | 18.88    | 347.23    | 2000    | 1162.2   |
| Extract order item prices (double wildcard) | jmespath    | 44.99    | 46.72       | 46.77    | 140.23    | 2000    | 1569.4   |
| Extract order item prices (double wildcard) | strata      | 119.46   | 121.41      | 123.19   | 53.96     | 100999  | 1569.4   |
| Extract order item prices (double wildcard) | jsonpath-ng | 145.87   | 146.50      | 148.70   | 44.72     | 100999  | 1571.8   |
| Filter users by age (numeric predicate)     | strata      | 2.22     | 2.29        | 2.40     | 2858.13   | 1606    | 1802.2   |
| Filter users by age (numeric predicate)     | jmespath    | 2.32     | 2.37        | 2.40     | 2765.82   | 1606    | 1802.2   |
| Recursively find all prices                 | strata      | 130.26   | 131.07      | 131.14   | 49.99     | 100999  | 2201.5   |
| Recursively find all prices                 | jsonpath-ng | 550.21   | 552.78      | 553.77   | 11.85     | 100999  | 2203.5   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 275372.53 | 10      | 2285.2   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 211064.78 | 10      | 2285.2   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 207989.48 | 10      | 2285.0   |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 431835.89 | 1       | 4052.8   |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 356443.81 | 1       | 4052.8   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 328863.66 | 1       | 4052.8   |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 333740.71 | 0       | 4051.2   |
| Extract all user IDs                        | jmespath    | 0.94     | 0.96        | 0.96     | 6842.45   | 2000    | 4051.2   |
| Extract all user IDs                        | strata      | 18.47    | 18.49       | 18.63    | 354.18    | 2000    | 4050.9   |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.02        | 0.02     | 358061.26 | 0       | 4052.8   |
| Extract all user names                      | jmespath    | 0.93     | 0.94        | 0.94     | 6991.26   | 2000    | 4052.8   |
| Extract all user names                      | strata      | 18.37    | 18.64       | 18.69    | 351.47    | 2000    | 4052.8   |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.02        | 0.02     | 341733.30 | 0       | 4051.7   |
| Extract nested timestamp field              | jmespath    | 1.50     | 1.51        | 1.53     | 4335.84   | 2000    | 4051.7   |
| Extract nested timestamp field              | strata      | 18.82    | 19.26       | 19.42    | 340.12    | 2000    | 4051.7   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.01     | 0.02        | 0.02     | 422558.21 | 0       | 4052.8   |
| Extract order item prices (double wildcard) | jmespath    | 27.46    | 27.76       | 27.97    | 235.97    | 100999  | 4052.8   |
| Extract order item prices (double wildcard) | strata      | 136.21   | 136.71      | 137.16   | 47.91     | 100999  | 4052.7   |
| Filter users by age (numeric predicate)     | jmespath    | 2.41     | 2.42        | 2.43     | 2708.61   | 1606    | 4052.8   |
| Filter users by age (numeric predicate)     | strata      | 18.21    | 18.26       | 19.03    | 358.72    | 1606    | 4052.8   |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.01        | 0.02     | 534665.29 | 1       | 4034.8   |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.02     | 515396.71 | 1       | 4034.7   |
| NDJSON first record id                      | strata      | 0.01     | 0.02        | 0.03     | 305216.91 | 1       | 4034.7   |
| Recursively find all prices                 | strata      | 144.09   | 144.50      | 146.19   | 45.33     | 100999  | 4052.9   |
| Recursively find all prices                 | jsonpath-ng | 549.60   | 549.80      | 550.72   | 11.91     | 100999  | 4054.1   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 294366.50 | 10      | 4053.1   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.04     | 198474.40 | 10      | 4053.1   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 197976.43 | 10      | 4052.9   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 320.19   | 321.11      | 321.94   | 163.23 | 2374.2   |
| strata_cursor_reparse | 733.37   | 769.31      | 785.78   | 68.13  | 4028.3   |
- Speedup: 2.40x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 336.14   | 337.35      | 358.68   | 174.73 | 4053.0   |
| strata_cursor_reparse | 829.41   | 831.53      | 831.83   | 70.89  | 4072.3   |
- Speedup: 2.46x

## Summary
- loads json: orjson (37.66 ms)
- loads ndjson: orjson (34.66 ms)
- dumps str: orjson (7.17 ms)
- dumps bytes: orjson (6.34 ms)
- search: jsonpath-ng (0.01 ms)
- cursor reuse: strata_cursor_reuse (321.11 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 24.0% behind orjson
- Strata gap in loads/ndjson: 41.5% behind orjson
- Strata gap in dumps/str: 310.6% behind orjson
- Strata gap in dumps/bytes: 362.3% behind orjson
