# Strata Benchmark Results — SMALL
Generated: 2026-02-18 13:12:13

## Environment
- Commit: 572edda55e994922217874e725e6904ab7bad84a
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
| msgspec       | 3.72     | 4.56        | 6.18     | 221.30 | 395.9    |
| orjson        | 2.84     | 4.98        | 6.10     | 202.49 | 392.8    |
| ujson         | 4.12     | 6.73        | 7.45     | 149.74 | 396.0    |
| json (stdlib) | 5.47     | 7.13        | 7.80     | 141.42 | 396.0    |
| strata        | 10.96    | 12.69       | 17.91    | 79.45  | 389.4    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 3.67     | 4.86        | 5.39     | 207.42 | 55.4     |
| orjson        | 3.87     | 5.53        | 5.91     | 182.13 | 54.3     |
| ujson         | 4.28     | 6.52        | 7.27     | 154.49 | 57.4     |
| json (stdlib) | 6.21     | 7.74        | 7.83     | 130.15 | 58.4     |
| strata        | 8.16     | 8.68        | 9.58     | 115.97 | 54.2     |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.75     | 0.78        | 0.83     | 1148.07 | 895062         | 42.2     |
| msgspec | 0.91     | 0.95        | 1.04     | 944.28  | 895062         | 42.4     |
| ujson   | 3.67     | 3.81        | 3.91     | 234.64  | 895062         | 42.5     |
| json    | 5.73     | 5.74        | 5.81     | 175.73  | 1008198        | 43.8     |
| strata  | 6.92     | 7.05        | 7.20     | 141.77  | 1000153        | 41.1     |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.72     | 0.79        | 0.89     | 1135.39 | 895062         | 44.2     |
| msgspec | 0.92     | 1.03        | 1.11     | 872.13  | 895062         | 44.8     |
| ujson   | 3.61     | 3.87        | 4.05     | 231.36  | 895062         | 46.5     |
| json    | 5.64     | 5.84        | 6.00     | 172.75  | 1008198        | 47.1     |
| strata  | 7.31     | 7.69        | 8.45     | 130.02  | 1000153        | 44.2     |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.02     | 288059.17 | 1       | 94.6     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 132954.98 | 1       | 94.7     |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 45141.75  | 1       | 95.1     |
| Extract all user IDs                        | strata      | 0.06     | 0.09        | 0.10     | 10697.07  | 1000    | 94.9     |
| Extract all user IDs                        | jmespath    | 0.19     | 0.22        | 0.23     | 4648.75   | 1000    | 95.1     |
| Extract all user IDs                        | jsonpath-ng | 1.55     | 1.60        | 1.98     | 632.05    | 1000    | 95.9     |
| Extract all user names                      | strata      | 0.05     | 0.06        | 0.07     | 15742.77  | 1000    | 95.3     |
| Extract all user names                      | jmespath    | 0.18     | 0.19        | 0.19     | 5444.80   | 1000    | 95.4     |
| Extract all user names                      | jsonpath-ng | 1.53     | 1.54        | 1.61     | 655.76    | 1000    | 96.0     |
| Extract nested timestamp field              | strata      | 0.09     | 0.13        | 0.15     | 7733.06   | 1000    | 94.5     |
| Extract nested timestamp field              | jmespath    | 0.52     | 0.54        | 0.54     | 1882.87   | 1000    | 94.6     |
| Extract nested timestamp field              | jsonpath-ng | 2.33     | 2.38        | 2.82     | 424.27    | 1000    | 95.6     |
| Extract order item prices (double wildcard) | strata      | 0.56     | 0.71        | 0.76     | 1426.86   | 12046   | 96.3     |
| Extract order item prices (double wildcard) | jmespath    | 5.51     | 5.89        | 6.32     | 171.19    | 1000    | 97.2     |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.44    | 17.91       | 18.08    | 56.30     | 12046   | 100.8    |
| Filter users by age (numeric predicate)     | strata      | 0.10     | 0.13        | 0.14     | 7737.99   | 794     | 94.7     |
| Filter users by age (numeric predicate)     | jmespath    | 1.07     | 1.08        | 1.30     | 935.32    | 794     | 94.8     |
| Recursively find all prices                 | strata      | 1.70     | 1.77        | 1.96     | 569.19    | 12046   | 96.3     |
| Recursively find all prices                 | jsonpath-ng | 73.06    | 75.23       | 77.54    | 13.40     | 12046   | 99.2     |
| Slice first 10 users, extract IDs           | strata      | 0.00     | 0.00        | 0.00     | 390325.85 | 10      | 94.7     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 84304.56  | 10      | 94.7     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.02     | 61569.20  | 10      | 95.1     |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 148291.05 | 1       | 45.2     |
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.03     | 117346.59 | 1       | 45.2     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.03     | 99475.36  | 1       | 45.4     |
| Extract all user IDs                        | jsonpath-ng | 0.00     | 0.00        | 0.01     | 249180.47 | 0       | 47.1     |
| Extract all user IDs                        | jmespath    | 0.22     | 0.23        | 0.24     | 4423.15   | 1000    | 47.1     |
| Extract all user IDs                        | strata      | 1.20     | 1.23        | 1.28     | 819.46    | 1000    | 46.6     |
| Extract all user names                      | jsonpath-ng | 0.00     | 0.01        | 0.02     | 87262.80  | 0       | 47.4     |
| Extract all user names                      | jmespath    | 0.22     | 0.24        | 0.27     | 4132.76   | 1000    | 47.4     |
| Extract all user names                      | strata      | 1.35     | 1.70        | 1.96     | 594.15    | 1000    | 46.7     |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.02        | 0.02     | 61197.26  | 0       | 46.6     |
| Extract nested timestamp field              | jmespath    | 0.56     | 0.61        | 0.65     | 1649.10   | 1000    | 46.6     |
| Extract nested timestamp field              | strata      | 1.47     | 1.56        | 2.13     | 644.51    | 1000    | 46.5     |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.00     | 0.01        | 0.01     | 143025.28 | 0       | 47.8     |
| Extract order item prices (double wildcard) | strata      | 0.72     | 0.88        | 1.25     | 1139.78   | 12046   | 47.3     |
| Extract order item prices (double wildcard) | jmespath    | 3.49     | 3.51        | 3.56     | 286.61    | 12046   | 47.8     |
| Filter users by age (numeric predicate)     | strata      | 0.27     | 0.27        | 0.31     | 3732.63   | 794     | 45.7     |
| Filter users by age (numeric predicate)     | jmespath    | 1.12     | 1.13        | 1.13     | 894.81    | 794     | 45.8     |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.01     | 181738.69 | 1       | 46.1     |
| NDJSON first record id                      | strata      | 0.00     | 0.01        | 0.01     | 177760.05 | 1       | 46.1     |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.01        | 0.02     | 70065.36  | 1       | 46.3     |
| Recursively find all prices                 | strata      | 1.87     | 1.91        | 2.00     | 526.53    | 12046   | 47.4     |
| Recursively find all prices                 | jsonpath-ng | 71.04    | 72.18       | 72.21    | 13.95     | 12046   | 50.8     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.01        | 0.01     | 71512.93  | 10      | 46.6     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.03     | 57415.80  | 10      | 46.4     |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 54690.95  | 10      | 46.4     |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 12.63    | 12.74       | 13.82    | 632.91 | 171.8    |
| strata_cursor_reparse | 90.11    | 93.75       | 100.99   | 86.04  | 904.3    |
- Speedup: 7.36x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 13.20    | 13.70       | 13.88    | 661.62 | 36.8     |
| strata_cursor_reparse | 87.22    | 87.55       | 88.74    | 103.54 | 45.3     |
- Speedup: 6.39x

## Summary
- loads json: msgspec (4.56 ms)
- loads ndjson: msgspec (4.86 ms)
- dumps str: orjson (0.78 ms)
- dumps bytes: orjson (0.79 ms)
- search: strata (0.00 ms)
- cursor reuse: strata_cursor_reuse (12.74 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 178.5% behind msgspec
- Strata gap in loads/ndjson: 78.9% behind msgspec
- Strata gap in dumps/str: 804.9% behind orjson
- Strata gap in dumps/bytes: 875.7% behind orjson
