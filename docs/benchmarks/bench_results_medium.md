# Strata Benchmark Results — MEDIUM
Generated: 2026-02-15 02:32:39

## Environment
- Commit: e67973e9173167da1e3b024e759eef7bdc164292
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
| strata        | 32.91    | 36.97       | 41.21    | 177.23 | 184.6    |
| msgspec       | 29.40    | 38.50       | 38.84    | 170.19 | 202.3    |
| orjson        | 28.79    | 38.53       | 41.49    | 170.05 | 204.5    |
| ujson         | 35.60    | 49.71       | 54.81    | 131.80 | 219.1    |
| json (stdlib) | 45.97    | 54.02       | 56.41    | 121.28 | 202.4    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 26.04    | 34.72       | 37.63    | 188.65 | 242.7    |
| msgspec       | 25.76    | 38.27       | 39.79    | 171.13 | 243.2    |
| strata        | 41.78    | 47.11       | 49.08    | 139.02 | 242.5    |
| ujson         | 36.97    | 52.07       | 55.14    | 125.77 | 259.3    |
| json (stdlib) | 41.59    | 54.43       | 55.03    | 120.33 | 248.4    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.42     | 6.49        | 6.73     | 895.48 | 5811025        | 309.3    |
| msgspec | 7.03     | 7.13        | 7.28     | 815.32 | 5811025        | 352.0    |
| ujson   | 23.23    | 23.62       | 24.73    | 246.01 | 5811025        | 352.2    |
| json    | 38.50    | 38.88       | 39.59    | 168.52 | 6551664        | 402.5    |
| strata  | 52.24    | 56.06       | 57.62    | 119.35 | 6690995        | 301.7    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.25     | 6.43        | 6.65     | 903.84 | 5811025        | 399.7    |
| msgspec | 6.80     | 6.87        | 7.46     | 845.75 | 5811025        | 445.1    |
| ujson   | 24.06    | 25.79       | 26.55    | 225.35 | 5811025        | 447.3    |
| json    | 38.34    | 39.03       | 41.38    | 167.86 | 6551664        | 497.4    |
| strata  | 52.49    | 53.04       | 53.20    | 126.16 | 6690995        | 402.5    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 529427.10 | 1       | 1738.2   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 333146.56 | 1       | 1738.2   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 262496.97 | 1       | 1738.2   |
| Extract all user IDs                        | strata      | 0.24     | 0.25        | 0.26     | 26628.23  | 2000    | 900.8    |
| Extract all user IDs                        | jmespath    | 0.70     | 0.77        | 0.81     | 8541.93   | 2000    | 900.8    |
| Extract all user IDs                        | jsonpath-ng | 18.23    | 18.25       | 19.09    | 358.92    | 2000    | 900.8    |
| Extract all user names                      | strata      | 0.31     | 0.34        | 0.46     | 19283.78  | 2000    | 1869.9   |
| Extract all user names                      | jmespath    | 0.57     | 0.65        | 0.66     | 10147.13  | 2000    | 1869.9   |
| Extract all user names                      | jsonpath-ng | 17.42    | 17.67       | 18.28    | 370.75    | 2000    | 1869.9   |
| Extract nested timestamp field              | strata      | 0.43     | 0.44        | 0.46     | 14804.62  | 2000    | 1140.8   |
| Extract nested timestamp field              | jmespath    | 1.24     | 1.25        | 1.32     | 5250.96   | 2000    | 1140.8   |
| Extract nested timestamp field              | jsonpath-ng | 18.83    | 19.64       | 20.17    | 333.66    | 2000    | 1140.8   |
| Extract order item prices (double wildcard) | strata      | 5.96     | 6.41        | 7.10     | 1022.29   | 100999  | 1493.8   |
| Extract order item prices (double wildcard) | jmespath    | 43.44    | 43.60       | 43.70    | 150.28    | 2000    | 1493.8   |
| Extract order item prices (double wildcard) | jsonpath-ng | 138.55   | 138.64      | 142.65   | 47.26     | 100999  | 1497.1   |
| Filter users by age (numeric predicate)     | strata      | 0.50     | 0.51        | 0.53     | 12787.90  | 1606    | 1992.2   |
| Filter users by age (numeric predicate)     | jmespath    | 2.37     | 2.44        | 2.46     | 2686.94   | 1606    | 1992.2   |
| Recursively find all prices                 | strata      | 15.03    | 15.72       | 16.32    | 416.79    | 100999  | 2167.9   |
| Recursively find all prices                 | jsonpath-ng | 525.45   | 527.15      | 531.80   | 12.43     | 100999  | 2169.8   |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 903678.17 | 10      | 2247.1   |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 465184.46 | 10      | 2247.1   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 254433.63 | 10      | 2247.1   |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 523971.91 | 1       | 3917.0   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 319495.10 | 1       | 3917.0   |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.03     | 313126.11 | 1       | 3917.0   |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 551549.58 | 0       | 3915.2   |
| Extract all user IDs                        | jmespath    | 0.93     | 0.96        | 0.99     | 6820.19   | 2000    | 3915.2   |
| Extract all user IDs                        | strata      | 16.07    | 16.39       | 16.45    | 399.60    | 2000    | 3905.5   |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.02        | 0.02     | 358884.65 | 0       | 3917.1   |
| Extract all user names                      | jmespath    | 0.93     | 0.95        | 0.97     | 6902.55   | 2000    | 3917.1   |
| Extract all user names                      | strata      | 19.30    | 19.42       | 20.93    | 337.35    | 2000    | 3917.1   |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.02        | 0.02     | 318206.69 | 0       | 3915.5   |
| Extract nested timestamp field              | jmespath    | 1.59     | 1.70        | 1.71     | 3846.70   | 2000    | 3915.5   |
| Extract nested timestamp field              | strata      | 16.89    | 17.43       | 17.86    | 375.83    | 2000    | 3915.5   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.02     | 336605.03 | 0       | 3916.8   |
| Extract order item prices (double wildcard) | strata      | 20.92    | 21.04       | 21.25    | 311.29    | 100999  | 3916.6   |
| Extract order item prices (double wildcard) | jmespath    | 25.60    | 25.77       | 26.48    | 254.20    | 100999  | 3916.8   |
| Filter users by age (numeric predicate)     | jmespath    | 2.45     | 2.52        | 2.53     | 2601.31   | 1606    | 3917.1   |
| Filter users by age (numeric predicate)     | strata      | 16.41    | 16.46       | 16.61    | 398.03    | 1606    | 3917.1   |
| NDJSON first record id                      | strata      | 0.01     | 0.01        | 0.02     | 451700.39 | 1       | 3888.1   |
| NDJSON first record id                      | jmespath    | 0.01     | 0.02        | 0.02     | 329542.19 | 1       | 3888.1   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 326129.04 | 1       | 3888.1   |
| Recursively find all prices                 | strata      | 27.83    | 28.03       | 28.31    | 233.63    | 100999  | 3917.1   |
| Recursively find all prices                 | jsonpath-ng | 538.76   | 549.18      | 555.01   | 11.93     | 100999  | 3918.3   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 328155.38 | 10      | 3917.3   |
| Slice first 10 users, extract IDs           | jmespath    | 0.03     | 0.03        | 0.03     | 242957.63 | 10      | 3917.3   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 194785.08 | 10      | 3917.1   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 77.85    | 80.27       | 85.77    | 652.93 | 2387.0   |
| strata_cursor_reparse | 475.73   | 491.57      | 563.12   | 106.62 | 3875.5   |
- Speedup: 6.12x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 94.64    | 102.94      | 106.73   | 572.62 | 3917.2   |
| strata_cursor_reparse | 555.13   | 561.61      | 604.20   | 104.96 | 3936.1   |
- Speedup: 5.46x

## Summary
- loads json: strata (36.97 ms)
- loads ndjson: orjson (34.72 ms)
- dumps str: orjson (6.49 ms)
- dumps bytes: orjson (6.43 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (80.27 ms)
- Rule 13: strata not #1 in: loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/ndjson: 35.7% behind orjson
- Strata gap in dumps/str: 763.9% behind orjson
- Strata gap in dumps/bytes: 724.9% behind orjson
