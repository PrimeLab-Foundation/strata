# Strata Benchmark Results — SMALL
Generated: 2026-02-14 20:44:03

## Environment
- Commit: 9bb824273ac8d1727e85f6fb247640ba83026134
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
- json: benchmarks/data/generated/small/users.json (0.96 MB, 1000 records)
- ndjson: benchmarks/data/generated/small/users.ndjson (0.96 MB, 1000 records, 1000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (0.96 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 4.49     | 6.40        | 7.04     | 157.47 | 368.9    |
| msgspec       | 5.08     | 6.41        | 7.19     | 157.37 | 370.0    |
| ujson         | 6.25     | 8.73        | 9.09     | 115.49 | 372.0    |
| json (stdlib) | 7.90     | 9.25        | 9.76     | 109.04 | 372.0    |
| strata        | 11.57    | 13.05       | 14.71    | 77.28  | 365.2    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 4.56     | 5.79        | 6.26     | 173.89 | 372.2    |
| orjson        | 3.78     | 6.06        | 6.33     | 166.32 | 372.2    |
| strata        | 7.20     | 8.02        | 8.08     | 125.63 | 372.2    |
| ujson         | 6.13     | 8.22        | 9.06     | 122.51 | 374.2    |
| json (stdlib) | 7.73     | 10.01       | 10.29    | 100.63 | 374.2    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.85     | 0.86        | 0.93     | 1039.56 | 895062         | 376.2    |
| msgspec | 1.04     | 1.08        | 1.15     | 831.81  | 895062         | 376.4    |
| strata  | 3.90     | 3.98        | 4.19     | 251.09  | 1000153        | 376.2    |
| ujson   | 4.08     | 4.11        | 4.14     | 217.70  | 895062         | 376.4    |
| json    | 6.35     | 6.44        | 6.67     | 156.44  | 1008198        | 376.4    |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.81     | 0.82        | 0.85     | 1086.18 | 895062         | 376.4    |
| msgspec | 1.02     | 1.09        | 1.10     | 823.39  | 895062         | 376.4    |
| strata  | 3.87     | 4.03        | 4.22     | 248.07  | 1000153        | 376.4    |
| ujson   | 3.93     | 4.09        | 4.20     | 218.94  | 895062         | 376.4    |
| json    | 6.18     | 6.26        | 6.31     | 160.99  | 1008198        | 376.4    |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s     | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 51373.14 | 1       | 545.0    |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 44316.40 | 1       | 545.0    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 39344.33 | 1       | 545.0    |
| Extract all user IDs                        | jmespath    | 0.33     | 0.34        | 0.35     | 2982.10  | 1000    | 427.9    |
| Extract all user IDs                        | strata      | 1.34     | 1.42        | 1.46     | 710.73   | 1000    | 427.7    |
| Extract all user IDs                        | jsonpath-ng | 4.74     | 4.85        | 4.91     | 208.09   | 1000    | 427.9    |
| Extract all user names                      | jmespath    | 0.26     | 0.27        | 0.31     | 3675.65  | 1000    | 570.9    |
| Extract all user names                      | strata      | 1.36     | 1.47        | 1.50     | 683.58   | 1000    | 570.9    |
| Extract all user names                      | jsonpath-ng | 4.25     | 4.50        | 4.51     | 224.21   | 1000    | 570.9    |
| Extract nested timestamp field              | jmespath    | 0.67     | 0.67        | 0.74     | 1512.39  | 1000    | 487.9    |
| Extract nested timestamp field              | strata      | 1.43     | 1.45        | 1.48     | 696.01   | 1000    | 487.8    |
| Extract nested timestamp field              | jsonpath-ng | 5.18     | 5.27        | 5.35     | 191.18   | 1000    | 487.9    |
| Extract order item prices (double wildcard) | jmespath    | 8.55     | 8.92        | 9.20     | 113.07   | 1000    | 514.7    |
| Extract order item prices (double wildcard) | strata      | 16.19    | 16.45       | 16.71    | 61.30    | 12046   | 514.6    |
| Extract order item prices (double wildcard) | jsonpath-ng | 21.80    | 21.99       | 22.27    | 45.86    | 12046   | 514.7    |
| Filter users by age (numeric predicate)     | strata      | 1.14     | 1.16        | 1.59     | 867.24   | 794     | 595.8    |
| Filter users by age (numeric predicate)     | jmespath    | 1.32     | 1.33        | 1.41     | 758.35   | 794     | 595.8    |
| Recursively find all prices                 | strata      | 17.41    | 17.64       | 17.72    | 57.14    | 12046   | 614.2    |
| Recursively find all prices                 | jsonpath-ng | 80.85    | 80.95       | 81.75    | 12.45    | 12046   | 614.2    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 43597.75 | 10      | 634.7    |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 34567.58 | 10      | 634.7    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 27218.44 | 10      | 634.7    |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s     | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 50783.37 | 1       | 1160.2   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 45350.42 | 1       | 1160.2   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 43243.60 | 1       | 1160.2   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 45607.12 | 0       | 1160.0   |
| Extract all user IDs                        | jmespath    | 0.41     | 0.45        | 0.46     | 2226.24  | 1000    | 1160.0   |
| Extract all user IDs                        | strata      | 4.73     | 5.16        | 5.47     | 195.10   | 1000    | 1160.0   |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 57281.89 | 0       | 1160.2   |
| Extract all user names                      | jmespath    | 0.42     | 0.46        | 0.52     | 2180.25  | 1000    | 1160.2   |
| Extract all user names                      | strata      | 4.89     | 5.09        | 5.61     | 197.70   | 1000    | 1160.2   |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.01        | 0.02     | 78228.05 | 0       | 1160.0   |
| Extract nested timestamp field              | jmespath    | 0.80     | 0.84        | 1.25     | 1205.25  | 1000    | 1160.0   |
| Extract nested timestamp field              | strata      | 5.34     | 5.51        | 5.59     | 182.87   | 1000    | 1160.0   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.02     | 54566.43 | 0       | 1160.2   |
| Extract order item prices (double wildcard) | jmespath    | 4.43     | 4.71        | 4.83     | 213.73   | 12046   | 1160.2   |
| Extract order item prices (double wildcard) | strata      | 15.93    | 15.96       | 16.57    | 63.12    | 12046   | 1160.1   |
| Filter users by age (numeric predicate)     | strata      | 1.17     | 1.17        | 1.18     | 861.49   | 794     | 1160.2   |
| Filter users by age (numeric predicate)     | jmespath    | 1.34     | 1.35        | 1.46     | 747.52   | 794     | 1160.2   |
| NDJSON first record id                      | jmespath    | 0.01     | 0.02        | 0.02     | 58104.68 | 1       | 1158.7   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 47119.89 | 1       | 1158.7   |
| NDJSON first record id                      | strata      | 0.01     | 0.02        | 0.03     | 47027.49 | 1       | 1158.7   |
| Recursively find all prices                 | strata      | 17.54    | 17.55       | 17.62    | 57.39    | 12046   | 1160.2   |
| Recursively find all prices                 | jsonpath-ng | 80.80    | 81.46       | 81.93    | 12.36    | 12046   | 1160.2   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 39955.07 | 10      | 1160.2   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 32101.57 | 10      | 1160.2   |
| Slice first 10 users, extract IDs           | strata      | 0.04     | 0.04        | 0.04     | 25880.39 | 10      | 1160.2   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 48.58    | 49.25       | 50.36    | 163.76 | 679.7    |
| strata_cursor_reparse | 122.85   | 124.47      | 127.50   | 64.80  | 1157.6   |
- Speedup: 2.53x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 53.51    | 53.82       | 54.28    | 168.43 | 1160.3   |
| strata_cursor_reparse | 133.04   | 134.53      | 135.50   | 67.38  | 1162.1   |
- Speedup: 2.50x

## Summary
- loads json: orjson (6.40 ms)
- loads ndjson: msgspec (5.79 ms)
- dumps str: orjson (0.86 ms)
- dumps bytes: orjson (0.82 ms)
- search: jsonpath-ng (0.01 ms)
- cursor reuse: strata_cursor_reuse (49.25 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 103.8% behind orjson
- Strata gap in loads/ndjson: 38.4% behind msgspec
- Strata gap in dumps/str: 362.6% behind orjson
- Strata gap in dumps/bytes: 389.3% behind orjson
