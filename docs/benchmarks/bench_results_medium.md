# Strata Benchmark Results — MEDIUM
Generated: 2026-02-19 17:24:21

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
- json: benchmarks/data/generated/medium/users.json (6.25 MB, 2000 records)
- ndjson: benchmarks/data/generated/medium/users.ndjson (6.25 MB, 2000 records, 2000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (6.25 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 23.53    | 33.20       | 35.77    | 197.36 | 144.9    |
| msgspec       | 25.83    | 34.79       | 36.10    | 188.31 | 144.7    |
| strata        | 41.15    | 44.90       | 44.95    | 145.91 | 123.8    |
| ujson         | 33.04    | 46.39       | 47.02    | 141.24 | 162.5    |
| json (stdlib) | 41.77    | 51.97       | 53.66    | 126.06 | 151.0    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 22.16    | 30.78       | 31.89    | 212.76 | 187.4    |
| msgspec       | 22.17    | 31.96       | 32.64    | 204.92 | 184.2    |
| ujson         | 29.53    | 42.36       | 42.69    | 154.63 | 203.0    |
| strata        | 42.22    | 46.53       | 46.62    | 140.76 | 187.2    |
| json (stdlib) | 38.17    | 50.77       | 52.33    | 129.01 | 186.0    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.90     | 7.00        | 7.44     | 829.87 | 5811025        | 131.7    |
| msgspec | 7.33     | 7.51        | 8.02     | 774.18 | 5811025        | 175.2    |
| ujson   | 26.25    | 26.56       | 27.89    | 218.79 | 5811025        | 176.4    |
| json    | 41.58    | 41.84       | 42.00    | 156.60 | 6551664        | 226.6    |
| strata  | 57.69    | 58.02       | 59.05    | 115.31 | 6690995        | 127.1    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.02     | 6.20        | 6.27     | 937.28 | 5811025        | 129.9    |
| msgspec | 7.21     | 7.25        | 7.34     | 801.06 | 5811025        | 173.3    |
| ujson   | 25.43    | 25.82       | 26.80    | 225.08 | 5811025        | 173.6    |
| json    | 42.15    | 42.45       | 43.32    | 154.33 | 6551664        | 225.6    |
| strata  | 55.52    | 55.73       | 56.99    | 120.05 | 6690995        | 126.9    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 705088.05 | 1       | 526.4    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 534828.18 | 1       | 526.4    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 459766.15 | 1       | 526.8    |
| Extract all user IDs                        | strata      | 0.20     | 0.21        | 0.24     | 31727.19  | 2000    | 526.4    |
| Extract all user IDs                        | jmespath    | 0.58     | 0.72        | 0.79     | 9095.84   | 2000    | 526.6    |
| Extract all user IDs                        | jsonpath-ng | 7.25     | 7.78        | 7.79     | 842.05    | 2000    | 527.9    |
| Extract all user names                      | strata      | 0.21     | 0.21        | 0.23     | 30716.88  | 2000    | 526.8    |
| Extract all user names                      | jmespath    | 0.45     | 0.46        | 0.48     | 14374.24  | 2000    | 526.8    |
| Extract all user names                      | jsonpath-ng | 6.89     | 7.39        | 9.13     | 886.51    | 2000    | 527.8    |
| Extract nested timestamp field              | strata      | 0.37     | 0.37        | 0.39     | 17623.84  | 2000    | 526.5    |
| Extract nested timestamp field              | jmespath    | 1.28     | 1.33        | 1.37     | 4936.89   | 2000    | 526.7    |
| Extract nested timestamp field              | jsonpath-ng | 7.93     | 8.48        | 8.96     | 772.28    | 2000    | 528.3    |
| Extract order item prices (double wildcard) | strata      | 5.44     | 5.72        | 6.28     | 1144.55   | 100999  | 535.6    |
| Extract order item prices (double wildcard) | jmespath    | 35.10    | 35.88       | 36.49    | 182.60    | 2000    | 538.9    |
| Extract order item prices (double wildcard) | jsonpath-ng | 139.28   | 140.96      | 144.34   | 46.48     | 100999  | 543.6    |
| Filter users by age (numeric predicate)     | strata      | 0.34     | 0.34        | 0.35     | 19173.28  | 1606    | 526.6    |
| Filter users by age (numeric predicate)     | jmespath    | 2.40     | 2.42        | 2.45     | 2708.79   | 1606    | 526.8    |
| Recursively find all prices                 | strata      | 13.10    | 13.28       | 13.72    | 493.49    | 100999  | 539.5    |
| Recursively find all prices                 | jsonpath-ng | 512.39   | 521.03      | 525.50   | 12.57     | 100999  | 543.2    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 859237.92 | 10      | 526.4    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 467974.38 | 10      | 526.4    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 282301.54 | 10      | 526.8    |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 563410.95 | 1       | 151.7    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 515355.40 | 1       | 151.7    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 390045.17 | 1       | 152.1    |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 614009.66 | 0       | 163.7    |
| Extract all user IDs                        | jmespath    | 0.67     | 0.69        | 0.73     | 9502.58   | 2000    | 163.7    |
| Extract all user IDs                        | strata      | 5.44     | 5.45        | 5.62     | 1201.17   | 2000    | 152.9    |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.01        | 0.02     | 469239.32 | 0       | 162.6    |
| Extract all user names                      | jmespath    | 0.71     | 0.73        | 0.81     | 8913.11   | 2000    | 162.6    |
| Extract all user names                      | strata      | 5.54     | 5.58        | 5.59     | 1173.52   | 2000    | 150.6    |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.01        | 0.02     | 557418.55 | 0       | 153.7    |
| Extract nested timestamp field              | jmespath    | 1.44     | 1.50        | 1.52     | 4362.68   | 2000    | 153.7    |
| Extract nested timestamp field              | strata      | 6.05     | 6.19        | 6.47     | 1058.70   | 2000    | 153.4    |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.01     | 0.01        | 0.01     | 621291.93 | 0       | 167.2    |
| Extract order item prices (double wildcard) | strata      | 9.90     | 10.10       | 10.42    | 648.79    | 100999  | 164.1    |
| Extract order item prices (double wildcard) | jmespath    | 27.05    | 27.20       | 27.59    | 240.75    | 100999  | 167.1    |
| Filter users by age (numeric predicate)     | jmespath    | 2.59     | 2.61        | 2.65     | 2506.76   | 1606    | 152.8    |
| Filter users by age (numeric predicate)     | strata      | 5.48     | 5.51        | 5.82     | 1189.26   | 1606    | 152.8    |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.01     | 582189.96 | 1       | 151.6    |
| NDJSON first record id                      | strata      | 0.01     | 0.01        | 0.01     | 559416.64 | 1       | 151.6    |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.01        | 0.02     | 534666.56 | 1       | 152.0    |
| Recursively find all prices                 | strata      | 16.70    | 16.75       | 17.56    | 390.97    | 100999  | 164.3    |
| Recursively find all prices                 | jsonpath-ng | 501.30   | 508.65      | 510.11   | 12.88     | 100999  | 170.9    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 469239.32 | 10      | 148.3    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.02     | 446557.35 | 10      | 148.3    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 260247.53 | 10      | 148.7    |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 82.55    | 91.82       | 110.53   | 570.80 | 862.3    |
| strata_cursor_reparse | 450.91   | 453.83      | 513.26   | 115.49 | 2450.3   |
- Speedup: 4.94x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 74.00    | 75.31       | 76.95    | 782.74 | 95.3     |
| strata_cursor_reparse | 519.76   | 524.77      | 525.59   | 112.33 | 160.9    |
- Speedup: 6.97x

## Summary
- loads json: orjson (33.20 ms)
- loads ndjson: orjson (30.78 ms)
- dumps str: orjson (7.00 ms)
- dumps bytes: orjson (6.20 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (75.31 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 35.3% behind orjson
- Strata gap in loads/ndjson: 51.1% behind orjson
- Strata gap in dumps/str: 728.6% behind orjson
- Strata gap in dumps/bytes: 798.9% behind orjson
