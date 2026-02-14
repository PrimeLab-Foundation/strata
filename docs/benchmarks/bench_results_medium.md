# Strata Benchmark Results — MEDIUM
Generated: 2026-02-14 19:36:32

## Environment
- Commit: ce2c30a88ba06496f61d061303b781b3665f5fe9
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
| orjson        | 31.43    | 41.62       | 43.36    | 157.43 | 232.8    |
| msgspec       | 31.82    | 43.30       | 45.45    | 151.30 | 230.6    |
| strata        | 45.90    | 50.11       | 51.51    | 130.74 | 212.9    |
| ujson         | 39.75    | 51.74       | 54.42    | 126.62 | 247.4    |
| json (stdlib) | 47.34    | 55.41       | 58.60    | 118.24 | 230.7    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 24.97    | 34.87       | 35.28    | 187.82 | 305.5    |
| msgspec       | 25.02    | 35.45       | 35.93    | 184.78 | 306.0    |
| ujson         | 32.43    | 45.42       | 46.13    | 144.19 | 321.9    |
| strata        | 45.31    | 49.36       | 51.38    | 132.70 | 305.4    |
| json (stdlib) | 41.45    | 51.03       | 52.16    | 128.36 | 311.0    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.82     | 6.92        | 7.15     | 839.50 | 5811025        | 398.8    |
| msgspec | 7.23     | 7.28        | 7.33     | 797.77 | 5811025        | 443.9    |
| ujson   | 24.56    | 24.93       | 25.14    | 233.10 | 5811025        | 444.0    |
| strata  | 28.55    | 29.10       | 29.30    | 229.93 | 6690995        | 397.5    |
| json    | 39.81    | 40.16       | 40.30    | 163.13 | 6551664        | 494.1    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.00     | 6.22        | 6.31     | 933.87 | 5811025        | 542.7    |
| msgspec | 6.87     | 6.93        | 6.99     | 838.20 | 5811025        | 587.3    |
| ujson   | 24.56    | 25.05       | 25.95    | 231.97 | 5811025        | 590.8    |
| strata  | 28.43    | 28.81       | 28.99    | 232.24 | 6690995        | 545.4    |
| json    | 39.64    | 39.99       | 40.19    | 163.83 | 6551664        | 640.9    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 436777.66 | 1       | 1814.4   |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 373484.15 | 1       | 1814.4   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 309537.28 | 1       | 1814.4   |
| Extract all user IDs                        | jmespath    | 0.70     | 0.71        | 0.72     | 9220.12   | 2000    | 1073.7   |
| Extract all user IDs                        | strata      | 2.39     | 2.41        | 2.47     | 2723.76   | 2000    | 1073.5   |
| Extract all user IDs                        | jsonpath-ng | 17.56    | 17.60       | 17.65    | 372.31    | 2000    | 1073.8   |
| Extract all user names                      | jmespath    | 0.56     | 0.56        | 0.58     | 11705.48  | 2000    | 1885.5   |
| Extract all user names                      | strata      | 2.44     | 2.50        | 2.52     | 2624.25   | 2000    | 1885.5   |
| Extract all user names                      | jsonpath-ng | 17.31    | 17.50       | 17.63    | 374.41    | 2000    | 1885.5   |
| Extract nested timestamp field              | jmespath    | 1.27     | 1.30        | 1.36     | 5021.23   | 2000    | 1523.0   |
| Extract nested timestamp field              | strata      | 2.65     | 2.69        | 2.73     | 2438.17   | 2000    | 1522.9   |
| Extract nested timestamp field              | jsonpath-ng | 18.74    | 18.87       | 18.90    | 347.27    | 2000    | 1523.0   |
| Extract order item prices (double wildcard) | jmespath    | 44.64    | 44.74       | 46.65    | 146.43    | 2000    | 1685.4   |
| Extract order item prices (double wildcard) | strata      | 102.86   | 103.33      | 106.51   | 63.40     | 100999  | 1685.4   |
| Extract order item prices (double wildcard) | jsonpath-ng | 145.48   | 145.72      | 146.98   | 44.96     | 100999  | 1689.0   |
| Filter users by age (numeric predicate)     | strata      | 2.12     | 2.16        | 2.28     | 3039.51   | 1606    | 2281.2   |
| Filter users by age (numeric predicate)     | jmespath    | 2.27     | 2.29        | 2.34     | 2864.38   | 1606    | 2281.2   |
| Recursively find all prices                 | strata      | 127.76   | 128.66      | 131.42   | 50.92     | 100999  | 2431.5   |
| Recursively find all prices                 | jsonpath-ng | 540.51   | 544.14      | 547.99   | 12.04     | 100999  | 2434.2   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 273464.43 | 10      | 2557.1   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 270640.30 | 10      | 2557.1   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.03        | 0.03     | 237879.00 | 10      | 2557.1   |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 424861.31 | 1       | 4199.5   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 377872.08 | 1       | 4199.5   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 312513.44 | 1       | 4199.5   |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 611659.92 | 0       | 4197.4   |
| Extract all user IDs                        | jmespath    | 0.95     | 0.95        | 0.99     | 6877.18   | 2000    | 4197.4   |
| Extract all user IDs                        | strata      | 19.91    | 21.17       | 21.89    | 309.36    | 2000    | 4197.0   |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.02        | 0.02     | 409353.17 | 0       | 4199.6   |
| Extract all user names                      | jmespath    | 0.92     | 0.92        | 0.94     | 7085.49   | 2000    | 4199.6   |
| Extract all user names                      | strata      | 18.59    | 18.60       | 18.62    | 352.21    | 2000    | 4199.5   |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.01        | 0.02     | 520515.52 | 0       | 4197.7   |
| Extract nested timestamp field              | jmespath    | 1.52     | 1.52        | 1.55     | 4305.09   | 2000    | 4197.7   |
| Extract nested timestamp field              | strata      | 18.85    | 19.08       | 19.30    | 343.22    | 2000    | 4197.7   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.01     | 0.01        | 0.02     | 465041.68 | 0       | 4199.5   |
| Extract order item prices (double wildcard) | jmespath    | 27.24    | 27.32       | 27.37    | 239.77    | 100999  | 4199.5   |
| Extract order item prices (double wildcard) | strata      | 134.82   | 135.36      | 135.90   | 48.39     | 100999  | 4199.3   |
| Filter users by age (numeric predicate)     | jmespath    | 2.43     | 2.43        | 2.44     | 2691.59   | 1606    | 4199.6   |
| Filter users by age (numeric predicate)     | strata      | 18.06    | 18.21       | 18.46    | 359.70    | 1606    | 4199.6   |
| NDJSON first record id                      | jmespath    | 0.01     | 0.02        | 0.02     | 425993.67 | 1       | 4179.8   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 313755.63 | 1       | 4179.9   |
| NDJSON first record id                      | strata      | 0.02     | 0.02        | 0.02     | 310660.55 | 1       | 4179.8   |
| Recursively find all prices                 | strata      | 139.70   | 140.34      | 142.22   | 46.67     | 100999  | 4199.6   |
| Recursively find all prices                 | jsonpath-ng | 543.22   | 544.20      | 545.71   | 12.04     | 100999  | 4200.8   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 356443.81 | 10      | 4199.8   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 204147.24 | 10      | 4199.6   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.04     | 192636.87 | 10      | 4199.8   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 310.71   | 313.94      | 320.96   | 166.95 | 2728.5   |
| strata_cursor_reparse | 698.35   | 698.78      | 729.57   | 75.01  | 4182.5   |
- Speedup: 2.23x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 326.71   | 330.29      | 330.89   | 178.47 | 4199.7   |
| strata_cursor_reparse | 815.15   | 815.36      | 819.82   | 72.30  | 4219.4   |
- Speedup: 2.47x

## Summary
- loads json: orjson (41.62 ms)
- loads ndjson: orjson (34.87 ms)
- dumps str: orjson (6.92 ms)
- dumps bytes: orjson (6.22 ms)
- search: jsonpath-ng (0.01 ms)
- cursor reuse: strata_cursor_reuse (313.94 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 20.4% behind orjson
- Strata gap in loads/ndjson: 41.5% behind orjson
- Strata gap in dumps/str: 320.4% behind orjson
- Strata gap in dumps/bytes: 363.0% behind orjson
