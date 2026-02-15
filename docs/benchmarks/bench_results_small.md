# Strata Benchmark Results — SMALL
Generated: 2026-02-15 02:31:59

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
- json: benchmarks/data/generated/small/users.json (0.96 MB, 1000 records)
- ndjson: benchmarks/data/generated/small/users.ndjson (0.96 MB, 1000 records, 1000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (0.96 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 4.87     | 6.56        | 6.84     | 153.60 | 334.2    |
| msgspec       | 5.64     | 7.04        | 7.13     | 143.30 | 335.3    |
| ujson         | 6.82     | 8.62        | 9.79     | 116.94 | 337.4    |
| json (stdlib) | 7.59     | 8.66        | 8.87     | 116.39 | 337.4    |
| strata        | 10.50    | 12.68       | 17.08    | 79.49  | 330.5    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 3.66     | 5.43        | 5.90     | 185.45 | 337.5    |
| msgspec       | 4.95     | 6.02        | 6.39     | 167.28 | 337.5    |
| strata        | 7.42     | 7.75        | 7.92     | 130.00 | 337.4    |
| ujson         | 5.55     | 8.16        | 8.35     | 123.41 | 339.5    |
| json (stdlib) | 7.14     | 8.71        | 10.11    | 115.60 | 339.5    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.75     | 0.82        | 0.98     | 1095.49 | 895062         | 341.5    |
| msgspec | 0.97     | 1.05        | 1.20     | 851.39  | 895062         | 341.6    |
| ujson   | 3.59     | 3.80        | 4.03     | 235.41  | 895062         | 341.6    |
| json    | 5.57     | 5.76        | 6.15     | 174.90  | 1008198        | 341.9    |
| strata  | 7.32     | 7.50        | 7.55     | 133.41  | 1000153        | 341.5    |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.72     | 0.74        | 0.77     | 1212.55 | 895062         | 341.9    |
| msgspec | 0.88     | 0.90        | 1.02     | 993.82  | 895062         | 341.9    |
| ujson   | 3.67     | 4.10        | 4.32     | 218.35  | 895062         | 342.0    |
| json    | 5.96     | 6.34        | 7.08     | 159.12  | 1008198        | 342.0    |
| strata  | 6.68     | 7.07        | 7.89     | 141.49  | 1000153        | 341.9    |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 110487.32 | 1       | 506.7    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 81470.50  | 1       | 506.7    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.03     | 76575.76  | 1       | 506.7    |
| Extract all user IDs                        | strata      | 0.08     | 0.08        | 0.20     | 12339.04  | 1000    | 380.7    |
| Extract all user IDs                        | jmespath    | 0.26     | 0.38        | 0.42     | 2661.91   | 1000    | 380.7    |
| Extract all user IDs                        | jsonpath-ng | 2.93     | 3.29        | 3.49     | 306.30    | 1000    | 380.8    |
| Extract all user names                      | strata      | 0.11     | 0.11        | 0.17     | 9364.04   | 1000    | 525.9    |
| Extract all user names                      | jmespath    | 0.19     | 0.20        | 0.22     | 5045.20   | 1000    | 525.9    |
| Extract all user names                      | jsonpath-ng | 3.22     | 3.54        | 4.63     | 284.74    | 1000    | 525.9    |
| Extract nested timestamp field              | strata      | 0.17     | 0.18        | 0.18     | 5682.65   | 1000    | 441.3    |
| Extract nested timestamp field              | jmespath    | 0.55     | 0.57        | 0.59     | 1760.79   | 1000    | 441.3    |
| Extract nested timestamp field              | jsonpath-ng | 4.06     | 4.52        | 4.58     | 223.04    | 1000    | 441.3    |
| Extract order item prices (double wildcard) | strata      | 0.73     | 0.74        | 0.92     | 1356.78   | 12046   | 460.6    |
| Extract order item prices (double wildcard) | jmespath    | 7.92     | 7.97        | 8.15     | 126.48    | 1000    | 460.6    |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.75    | 18.88       | 19.07    | 53.39     | 12046   | 460.8    |
| Filter users by age (numeric predicate)     | strata      | 0.12     | 0.13        | 0.33     | 7637.86   | 794     | 558.7    |
| Filter users by age (numeric predicate)     | jmespath    | 1.09     | 1.20        | 1.21     | 836.91    | 794     | 558.7    |
| Recursively find all prices                 | strata      | 1.90     | 2.08        | 2.23     | 484.91    | 12046   | 575.5    |
| Recursively find all prices                 | jsonpath-ng | 76.40    | 84.07       | 85.47    | 11.99     | 12046   | 575.6    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 90291.74  | 10      | 603.0    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 38715.79  | 10      | 603.0    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 30095.47  | 10      | 603.0    |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.01     | 198109.06 | 1       | 1266.9   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 80846.72  | 1       | 1266.9   |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 77475.85  | 1       | 1266.9   |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 137349.90 | 0       | 1266.5   |
| Extract all user IDs                        | jmespath    | 0.33     | 0.49        | 0.51     | 2056.36   | 1000    | 1266.5   |
| Extract all user IDs                        | strata      | 3.06     | 3.45        | 3.69     | 292.27    | 1000    | 1266.5   |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 44763.87  | 0       | 1266.9   |
| Extract all user names                      | jmespath    | 0.26     | 0.31        | 0.36     | 3218.27   | 1000    | 1266.9   |
| Extract all user names                      | strata      | 2.20     | 2.25        | 2.27     | 447.27    | 1000    | 1266.9   |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.01        | 0.02     | 98666.50  | 0       | 1266.5   |
| Extract nested timestamp field              | jmespath    | 0.62     | 0.64        | 0.71     | 1575.06   | 1000    | 1266.5   |
| Extract nested timestamp field              | strata      | 2.45     | 2.63        | 2.98     | 382.25    | 1000    | 1266.5   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.01     | 0.01        | 0.01     | 143025.87 | 0       | 1266.9   |
| Extract order item prices (double wildcard) | strata      | 0.69     | 0.71        | 0.79     | 1413.76   | 12046   | 1266.5   |
| Extract order item prices (double wildcard) | jmespath    | 3.56     | 3.62        | 3.69     | 278.61    | 12046   | 1266.9   |
| Filter users by age (numeric predicate)     | strata      | 0.22     | 0.24        | 0.24     | 4258.72   | 794     | 1266.9   |
| Filter users by age (numeric predicate)     | jmespath    | 1.16     | 1.18        | 1.20     | 855.94    | 794     | 1266.9   |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.02     | 103747.94 | 1       | 1265.1   |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.01        | 0.02     | 96307.84  | 1       | 1265.1   |
| NDJSON first record id                      | strata      | 0.01     | 0.01        | 0.02     | 67897.11  | 1       | 1265.1   |
| Recursively find all prices                 | strata      | 1.85     | 1.88        | 1.93     | 536.90    | 12046   | 1266.9   |
| Recursively find all prices                 | jsonpath-ng | 69.40    | 69.98       | 69.99    | 14.39     | 12046   | 1266.9   |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.02        | 0.02     | 65508.09  | 10      | 1266.9   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 46307.47  | 10      | 1266.9   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 33388.15  | 10      | 1266.9   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 10.60    | 10.73       | 12.04    | 751.62 | 630.3    |
| strata_cursor_reparse | 84.81    | 86.91       | 91.78    | 92.81  | 1261.5   |
- Speedup: 8.10x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 15.28    | 15.55       | 15.99    | 583.04 | 1267.0   |
| strata_cursor_reparse | 87.72    | 92.38       | 92.81    | 98.13  | 1268.7   |
- Speedup: 5.94x

## Summary
- loads json: orjson (6.56 ms)
- loads ndjson: orjson (5.43 ms)
- dumps str: orjson (0.82 ms)
- dumps bytes: orjson (0.74 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (10.73 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 93.2% behind orjson
- Strata gap in loads/ndjson: 42.6% behind orjson
- Strata gap in dumps/str: 817.6% behind orjson
- Strata gap in dumps/bytes: 857.6% behind orjson
