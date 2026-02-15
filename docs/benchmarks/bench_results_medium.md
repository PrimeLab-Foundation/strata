# Strata Benchmark Results — MEDIUM
Generated: 2026-02-16 01:25:26

## Environment
- Commit: 55018296d1e43d763c65e319ab2ce3b86caeecc8
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
| orjson        | 23.97    | 34.55       | 35.00    | 189.61 | 144.9    |
| msgspec       | 29.42    | 35.55       | 37.77    | 184.31 | 145.7    |
| strata        | 34.42    | 38.88       | 39.91    | 168.49 | 126.0    |
| ujson         | 33.25    | 47.77       | 52.26    | 137.15 | 162.5    |
| json (stdlib) | 41.98    | 53.31       | 54.84    | 122.90 | 152.0    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 22.22    | 31.73       | 32.42    | 206.39 | 180.6    |
| msgspec       | 23.64    | 33.11       | 33.70    | 197.81 | 178.3    |
| ujson         | 30.09    | 43.87       | 45.11    | 149.30 | 195.1    |
| strata        | 44.59    | 45.75       | 47.35    | 143.17 | 180.4    |
| json (stdlib) | 37.50    | 46.95       | 47.61    | 139.50 | 179.2    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.38     | 6.72        | 6.98     | 864.92 | 5811025        | 130.8    |
| msgspec | 7.05     | 7.17        | 7.18     | 810.60 | 5811025        | 174.2    |
| ujson   | 25.06    | 25.65       | 26.15    | 226.51 | 5811025        | 176.6    |
| json    | 40.00    | 41.00       | 41.12    | 159.79 | 6551664        | 227.0    |
| strata  | 54.31    | 54.90       | 56.76    | 121.88 | 6690995        | 127.1    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.06     | 6.10        | 6.46     | 952.00 | 5811025        | 131.0    |
| msgspec | 6.87     | 6.99        | 7.23     | 831.20 | 5811025        | 175.2    |
| ujson   | 24.90    | 25.62       | 25.75    | 226.86 | 5811025        | 175.5    |
| json    | 40.15    | 40.45       | 41.10    | 161.97 | 6551664        | 225.9    |
| strata  | 53.69    | 54.04       | 54.94    | 123.83 | 6690995        | 127.0    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 580202.79 | 1       | 526.3    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 403179.14 | 1       | 526.8    |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.03     | 398059.62 | 1       | 526.3    |
| Extract all user IDs                        | strata      | 0.20     | 0.23        | 0.29     | 29005.71  | 2000    | 526.3    |
| Extract all user IDs                        | jmespath    | 0.51     | 0.61        | 0.65     | 10655.99  | 2000    | 526.5    |
| Extract all user IDs                        | jsonpath-ng | 6.40     | 6.63        | 7.49     | 987.79    | 2000    | 527.8    |
| Extract all user names                      | strata      | 0.28     | 0.30        | 0.30     | 21724.17  | 2000    | 526.8    |
| Extract all user names                      | jmespath    | 0.49     | 0.51        | 0.57     | 12744.35  | 2000    | 526.8    |
| Extract all user names                      | jsonpath-ng | 6.45     | 6.99        | 7.40     | 937.43    | 2000    | 527.9    |
| Extract nested timestamp field              | strata      | 0.46     | 0.47        | 0.52     | 14023.01  | 2000    | 526.7    |
| Extract nested timestamp field              | jmespath    | 1.20     | 1.27        | 1.29     | 5161.16   | 2000    | 526.8    |
| Extract nested timestamp field              | jsonpath-ng | 7.34     | 7.62        | 8.49     | 859.44    | 2000    | 528.4    |
| Extract order item prices (double wildcard) | strata      | 5.83     | 6.53        | 7.24     | 1002.73   | 100999  | 536.0    |
| Extract order item prices (double wildcard) | jmespath    | 33.95    | 34.01       | 34.09    | 192.63    | 2000    | 539.3    |
| Extract order item prices (double wildcard) | jsonpath-ng | 136.87   | 138.55      | 141.01   | 47.29     | 100999  | 544.2    |
| Filter users by age (numeric predicate)     | strata      | 0.48     | 0.48        | 0.49     | 13575.06  | 1606    | 526.7    |
| Filter users by age (numeric predicate)     | jmespath    | 2.38     | 2.38        | 2.40     | 2748.04   | 1606    | 526.8    |
| Recursively find all prices                 | strata      | 14.10    | 14.52       | 14.93    | 451.16    | 100999  | 540.9    |
| Recursively find all prices                 | jsonpath-ng | 500.93   | 501.73      | 502.02   | 13.06     | 100999  | 545.8    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 988927.23 | 10      | 526.2    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.03     | 466576.70 | 10      | 526.2    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 207167.19 | 10      | 526.7    |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 510335.92 | 1       | 155.3    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 399978.61 | 1       | 155.6    |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 391024.48 | 1       | 155.3    |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 553507.10 | 0       | 168.4    |
| Extract all user IDs                        | jmespath    | 0.73     | 0.78        | 0.89     | 8382.67   | 2000    | 168.3    |
| Extract all user IDs                        | strata      | 5.71     | 5.94        | 6.48     | 1102.33   | 2000    | 156.6    |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.01        | 0.02     | 505453.02 | 0       | 169.3    |
| Extract all user names                      | jmespath    | 0.66     | 0.76        | 0.77     | 8578.46   | 2000    | 169.3    |
| Extract all user names                      | strata      | 5.66     | 5.75        | 6.73     | 1139.76   | 2000    | 157.2    |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.02        | 0.03     | 415850.89 | 0       | 150.6    |
| Extract nested timestamp field              | jmespath    | 1.37     | 1.41        | 1.46     | 4630.91   | 2000    | 150.5    |
| Extract nested timestamp field              | strata      | 6.02     | 6.37        | 7.22     | 1027.83   | 2000    | 150.2    |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.01     | 0.02        | 0.02     | 301716.18 | 0       | 167.6    |
| Extract order item prices (double wildcard) | strata      | 10.43    | 10.51       | 11.01    | 622.95    | 100999  | 164.2    |
| Extract order item prices (double wildcard) | jmespath    | 26.22    | 26.26       | 26.72    | 249.38    | 100999  | 167.5    |
| Filter users by age (numeric predicate)     | jmespath    | 2.66     | 2.68        | 2.80     | 2446.30   | 1606    | 149.8    |
| Filter users by age (numeric predicate)     | strata      | 5.96     | 6.30        | 6.60     | 1039.85   | 1606    | 149.5    |
| NDJSON first record id                      | strata      | 0.01     | 0.01        | 0.01     | 623776.91 | 1       | 148.1    |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.02     | 542010.45 | 1       | 148.1    |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.02        | 0.03     | 355630.44 | 1       | 148.5    |
| Recursively find all prices                 | strata      | 17.02    | 17.42       | 17.57    | 376.04    | 100999  | 164.4    |
| Recursively find all prices                 | jsonpath-ng | 499.81   | 501.58      | 503.70   | 13.06     | 100999  | 173.9    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 406179.87 | 10      | 155.3    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 392006.67 | 10      | 155.3    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 304635.18 | 10      | 155.6    |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 70.88    | 82.23       | 100.19   | 637.38 | 625.9    |
| strata_cursor_reparse | 433.10   | 435.49      | 493.55   | 120.35 | 2186.2   |
- Speedup: 5.30x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 74.55    | 74.80       | 77.16    | 788.08 | 95.3     |
| strata_cursor_reparse | 521.78   | 522.77      | 529.24   | 112.76 | 160.3    |
- Speedup: 6.99x

## Summary
- loads json: orjson (34.55 ms)
- loads ndjson: orjson (31.73 ms)
- dumps str: orjson (6.72 ms)
- dumps bytes: orjson (6.10 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (74.80 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 12.5% behind orjson
- Strata gap in loads/ndjson: 44.2% behind orjson
- Strata gap in dumps/str: 717.1% behind orjson
- Strata gap in dumps/bytes: 785.2% behind orjson
