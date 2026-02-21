# Strata Benchmark Results — MEDIUM
Generated: 2026-02-22 01:26:52

## Environment
- Commit: 65f65be99e32d586b0d059d02131d20552789de6
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
| orjson        | 24.11    | 32.80       | 33.27    | 199.74 | 145.0    |
| msgspec       | 25.41    | 34.77       | 35.76    | 188.41 | 145.8    |
| ujson         | 34.72    | 45.84       | 48.69    | 142.92 | 162.6    |
| strata        | 42.86    | 46.26       | 46.80    | 141.61 | 124.9    |
| json (stdlib) | 41.07    | 50.22       | 52.85    | 130.46 | 152.2    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 21.35    | 30.60       | 31.18    | 214.01 | 183.6    |
| msgspec       | 21.50    | 31.36       | 31.83    | 208.85 | 181.4    |
| ujson         | 29.79    | 42.06       | 42.27    | 155.72 | 198.3    |
| strata        | 41.53    | 46.27       | 47.70    | 141.55 | 183.5    |
| json (stdlib) | 37.59    | 47.37       | 48.77    | 138.28 | 182.4    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.56     | 6.68        | 6.95     | 869.45 | 5811025        | 131.7    |
| msgspec | 7.52     | 7.57        | 7.61     | 767.89 | 5811025        | 175.2    |
| ujson   | 25.93    | 26.59       | 26.63    | 218.53 | 5811025        | 175.5    |
| json    | 41.26    | 41.32       | 41.54    | 158.56 | 6551664        | 225.8    |
| strata  | 56.05    | 56.87       | 57.18    | 117.65 | 6690995        | 127.0    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 5.96     | 6.28        | 6.33     | 924.89 | 5811025        | 129.9    |
| msgspec | 7.20     | 7.33        | 7.41     | 793.26 | 5811025        | 175.2    |
| ujson   | 26.32    | 26.38       | 26.51    | 220.32 | 5811025        | 175.4    |
| json    | 41.42    | 41.65       | 41.91    | 157.31 | 6551664        | 225.8    |
| strata  | 56.12    | 56.29       | 58.05    | 118.87 | 6690995        | 127.0    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.01     | 1020987.55 | 1       | 526.3    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 497582.88  | 1       | 526.3    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 396085.24  | 1       | 526.8    |
| Extract all user IDs                        | strata      | 0.20     | 0.21        | 0.22     | 31637.86   | 2000    | 526.3    |
| Extract all user IDs                        | jmespath    | 0.47     | 0.47        | 0.51     | 13865.96   | 2000    | 526.4    |
| Extract all user IDs                        | jsonpath-ng | 6.31     | 6.67        | 7.38     | 981.95     | 2000    | 527.5    |
| Extract all user names                      | strata      | 0.21     | 0.22        | 0.26     | 29188.82   | 2000    | 526.8    |
| Extract all user names                      | jmespath    | 0.47     | 0.49        | 0.50     | 13493.53   | 2000    | 526.8    |
| Extract all user names                      | jsonpath-ng | 6.25     | 6.29        | 6.32     | 1041.90    | 2000    | 527.9    |
| Extract nested timestamp field              | strata      | 0.39     | 0.42        | 0.43     | 15722.43   | 2000    | 526.8    |
| Extract nested timestamp field              | jmespath    | 1.21     | 1.22        | 1.23     | 5372.78    | 2000    | 526.9    |
| Extract nested timestamp field              | jsonpath-ng | 7.54     | 7.56        | 7.77     | 866.96     | 2000    | 528.5    |
| Extract order item prices (double wildcard) | strata      | 5.68     | 5.85        | 5.95     | 1120.72    | 100999  | 535.4    |
| Extract order item prices (double wildcard) | jmespath    | 34.73    | 35.10       | 35.45    | 186.64     | 2000    | 538.7    |
| Extract order item prices (double wildcard) | jsonpath-ng | 138.63   | 138.79      | 141.05   | 47.21      | 100999  | 543.7    |
| Filter users by age (numeric predicate)     | strata      | 0.34     | 0.34        | 0.36     | 19131.29   | 1606    | 526.6    |
| Filter users by age (numeric predicate)     | jmespath    | 2.41     | 2.42        | 2.43     | 2708.56    | 1606    | 526.8    |
| Recursively find all prices                 | strata      | 12.30    | 12.72       | 12.99    | 515.17     | 100999  | 539.6    |
| Recursively find all prices                 | jsonpath-ng | 515.43   | 517.53      | 519.42   | 12.66      | 100999  | 546.2    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 1007949.66 | 10      | 526.4    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 449267.29  | 10      | 526.4    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 282290.21  | 10      | 526.9    |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 674597.46 | 1       | 155.4    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 515395.52 | 1       | 155.7    |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 415850.89 | 1       | 155.4    |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 717767.02 | 0       | 161.5    |
| Extract all user IDs                        | jmespath    | 0.71     | 0.75        | 0.76     | 8741.61   | 2000    | 161.5    |
| Extract all user IDs                        | strata      | 5.13     | 5.29        | 5.95     | 1237.15   | 2000    | 149.8    |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.01        | 0.01     | 677599.55 | 0       | 162.2    |
| Extract all user names                      | jmespath    | 0.79     | 0.80        | 0.88     | 8181.53   | 2000    | 162.2    |
| Extract all user names                      | strata      | 5.42     | 5.48        | 5.53     | 1194.67   | 2000    | 150.1    |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.01        | 0.02     | 451700.39 | 0       | 153.6    |
| Extract nested timestamp field              | jmespath    | 1.44     | 1.49        | 1.51     | 4403.25   | 2000    | 153.6    |
| Extract nested timestamp field              | strata      | 5.83     | 6.08        | 6.22     | 1077.42   | 2000    | 153.5    |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.01     | 0.01        | 0.02     | 590962.33 | 0       | 163.8    |
| Extract order item prices (double wildcard) | strata      | 10.17    | 10.28       | 10.55    | 637.36    | 100999  | 160.6    |
| Extract order item prices (double wildcard) | jmespath    | 26.30    | 26.57       | 26.86    | 246.47    | 100999  | 163.7    |
| Filter users by age (numeric predicate)     | jmespath    | 2.62     | 2.66        | 2.67     | 2462.85   | 1606    | 156.5    |
| Filter users by age (numeric predicate)     | strata      | 5.55     | 5.83        | 7.05     | 1124.38   | 1606    | 156.5    |
| NDJSON first record id                      | strata      | 0.01     | 0.01        | 0.01     | 797956.09 | 1       | 155.3    |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.01     | 567462.71 | 1       | 155.3    |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.01        | 0.02     | 538311.53 | 1       | 155.6    |
| Recursively find all prices                 | strata      | 17.32    | 17.99       | 19.19    | 364.15    | 100999  | 160.7    |
| Recursively find all prices                 | jsonpath-ng | 511.20   | 512.78      | 526.89   | 12.77     | 100999  | 169.6    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 463691.58 | 10      | 155.4    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 381548.82 | 10      | 155.4    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 275774.79 | 10      | 155.7    |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 72.47    | 88.48       | 107.57   | 592.36 | 630.8    |
| strata_cursor_reparse | 464.53   | 477.90      | 536.19   | 109.68 | 3191.6   |
- Speedup: 5.40x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 76.08    | 77.50       | 79.11    | 760.64 | 94.8     |
| strata_cursor_reparse | 539.80   | 542.34      | 546.11   | 108.69 | 160.5    |
- Speedup: 7.00x

## Summary
- loads json: orjson (32.80 ms)
- loads ndjson: orjson (30.60 ms)
- dumps str: orjson (6.68 ms)
- dumps bytes: orjson (6.28 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (77.50 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 41.0% behind orjson
- Strata gap in loads/ndjson: 51.2% behind orjson
- Strata gap in dumps/str: 750.9% behind orjson
- Strata gap in dumps/bytes: 795.9% behind orjson
