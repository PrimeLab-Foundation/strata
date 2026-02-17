# Strata Benchmark Results — SMALL
Generated: 2026-02-18 00:43:36

## Environment
- Commit: 9217ce489741272a1a08b92b88515eeecb3e49b3
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
| orjson        | 4.50     | 4.60        | 5.77     | 219.30 | 418.3    |
| msgspec       | 3.46     | 5.04        | 6.08     | 200.01 | 420.4    |
| json (stdlib) | 6.43     | 6.82        | 7.49     | 147.94 | 421.5    |
| ujson         | 4.97     | 7.13        | 7.72     | 141.31 | 421.5    |
| strata        | 10.81    | 13.33       | 18.25    | 75.65  | 413.9    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 3.06     | 4.25        | 5.28     | 236.89 | 51.4     |
| orjson        | 3.26     | 4.27        | 4.91     | 236.06 | 50.3     |
| ujson         | 4.30     | 6.17        | 6.35     | 163.15 | 53.5     |
| json (stdlib) | 5.85     | 7.12        | 7.70     | 141.37 | 54.5     |
| strata        | 7.80     | 8.20        | 8.78     | 122.75 | 50.2     |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.79     | 0.87        | 0.92     | 1029.10 | 895062         | 44.0     |
| msgspec | 1.06     | 1.06        | 1.17     | 841.32  | 895062         | 45.0     |
| ujson   | 3.93     | 4.03        | 4.04     | 222.27  | 895062         | 45.1     |
| json    | 5.73     | 5.87        | 6.04     | 171.82  | 1008198        | 45.4     |
| strata  | 7.03     | 7.15        | 7.57     | 139.92  | 1000153        | 41.1     |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.75     | 0.76        | 0.77     | 1180.76 | 895062         | 41.4     |
| msgspec | 0.94     | 1.03        | 1.04     | 867.31  | 895062         | 42.6     |
| ujson   | 3.80     | 3.99        | 4.00     | 224.47  | 895062         | 42.8     |
| json    | 5.82     | 5.99        | 6.74     | 168.32  | 1008198        | 44.2     |
| strata  | 7.12     | 7.25        | 7.44     | 137.95  | 1000153        | 41.1     |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.01     | 284641.33 | 1       | 95.0     |
| Deep path navigation                        | jmespath    | 0.00     | 0.01        | 0.01     | 180583.95 | 1       | 95.1     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 137487.77 | 1       | 95.5     |
| Extract all user IDs                        | strata      | 0.03     | 0.03        | 0.05     | 32048.98  | 1000    | 94.8     |
| Extract all user IDs                        | jmespath    | 0.19     | 0.20        | 0.21     | 4947.19   | 1000    | 94.9     |
| Extract all user IDs                        | jsonpath-ng | 1.60     | 1.61        | 1.70     | 627.93    | 1000    | 95.7     |
| Extract all user names                      | strata      | 0.04     | 0.04        | 0.05     | 24893.80  | 1000    | 94.6     |
| Extract all user names                      | jmespath    | 0.18     | 0.19        | 0.19     | 5322.67   | 1000    | 94.7     |
| Extract all user names                      | jsonpath-ng | 1.61     | 1.61        | 1.61     | 627.54    | 1000    | 95.4     |
| Extract nested timestamp field              | strata      | 0.06     | 0.07        | 0.15     | 14515.22  | 1000    | 94.7     |
| Extract nested timestamp field              | jmespath    | 0.55     | 0.55        | 0.56     | 1834.76   | 1000    | 94.7     |
| Extract nested timestamp field              | jsonpath-ng | 2.18     | 2.19        | 2.20     | 460.37    | 1000    | 95.6     |
| Extract order item prices (double wildcard) | strata      | 0.55     | 0.72        | 1.03     | 1401.17   | 12046   | 96.0     |
| Extract order item prices (double wildcard) | jmespath    | 5.62     | 5.76        | 5.92     | 174.89    | 1000    | 96.9     |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.79    | 17.96       | 18.38    | 56.13     | 12046   | 100.9    |
| Filter users by age (numeric predicate)     | strata      | 0.05     | 0.06        | 0.07     | 16120.60  | 794     | 94.6     |
| Filter users by age (numeric predicate)     | jmespath    | 1.11     | 1.11        | 1.13     | 905.40    | 794     | 94.7     |
| Recursively find all prices                 | strata      | 1.58     | 1.62        | 1.66     | 620.49    | 12046   | 96.4     |
| Recursively find all prices                 | jsonpath-ng | 73.46    | 73.69       | 73.85    | 13.68     | 12046   | 99.6     |
| Slice first 10 users, extract IDs           | strata      | 0.00     | 0.00        | 0.00     | 653833.75 | 10      | 94.6     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 170390.54 | 10      | 94.6     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.01        | 0.02     | 70542.73  | 10      | 95.0     |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.01     | 212040.35 | 1       | 45.9     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 194965.02 | 1       | 45.9     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 151093.17 | 1       | 46.2     |
| Extract all user IDs                        | jsonpath-ng | 0.00     | 0.00        | 0.01     | 302099.29 | 0       | 47.2     |
| Extract all user IDs                        | jmespath    | 0.22     | 0.24        | 0.25     | 4277.57   | 1000    | 47.2     |
| Extract all user IDs                        | strata      | 1.07     | 1.07        | 1.25     | 942.47    | 1000    | 46.5     |
| Extract all user names                      | jsonpath-ng | 0.00     | 0.00        | 0.01     | 262697.23 | 0       | 47.3     |
| Extract all user names                      | jmespath    | 0.22     | 0.22        | 0.22     | 4578.98   | 1000    | 47.3     |
| Extract all user names                      | strata      | 1.11     | 1.12        | 1.21     | 898.47    | 1000    | 46.7     |
| Extract nested timestamp field              | jsonpath-ng | 0.00     | 0.00        | 0.01     | 210180.76 | 0       | 46.0     |
| Extract nested timestamp field              | jmespath    | 0.56     | 0.60        | 0.61     | 1669.83   | 1000    | 46.0     |
| Extract nested timestamp field              | strata      | 1.08     | 1.15        | 1.21     | 876.39    | 1000    | 45.8     |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.00     | 0.00        | 0.01     | 271625.21 | 0       | 47.8     |
| Extract order item prices (double wildcard) | strata      | 0.75     | 0.81        | 0.85     | 1241.72   | 12046   | 47.3     |
| Extract order item prices (double wildcard) | jmespath    | 3.61     | 3.69        | 3.79     | 273.02    | 12046   | 47.8     |
| Filter users by age (numeric predicate)     | strata      | 0.28     | 0.30        | 0.30     | 3356.35   | 794     | 46.5     |
| Filter users by age (numeric predicate)     | jmespath    | 1.14     | 1.14        | 1.16     | 881.11    | 794     | 46.6     |
| NDJSON first record id                      | jsonpath-ng | 0.00     | 0.00        | 0.01     | 213885.55 | 1       | 46.3     |
| NDJSON first record id                      | strata      | 0.01     | 0.01        | 0.02     | 162239.61 | 1       | 46.0     |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.02     | 137349.90 | 1       | 46.1     |
| Recursively find all prices                 | strata      | 1.86     | 1.87        | 1.93     | 539.56    | 12046   | 47.5     |
| Recursively find all prices                 | jsonpath-ng | 72.68    | 72.80       | 74.44    | 13.84     | 12046   | 50.9     |
| Slice first 10 users, extract IDs           | strata      | 0.00     | 0.01        | 0.01     | 171437.33 | 10      | 46.1     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 157991.08 | 10      | 46.1     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.01        | 0.02     | 77973.89  | 10      | 46.4     |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 9.65     | 10.70       | 11.99    | 754.05 | 136.0    |
| strata_cursor_reparse | 72.38    | 74.75       | 74.96    | 107.90 | 582.4    |
- Speedup: 6.99x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 13.37    | 13.80       | 14.35    | 657.01 | 36.4     |
| strata_cursor_reparse | 84.26    | 84.83       | 84.95    | 106.86 | 45.1     |
- Speedup: 6.15x

## Summary
- loads json: orjson (4.60 ms)
- loads ndjson: msgspec (4.25 ms)
- dumps str: orjson (0.87 ms)
- dumps bytes: orjson (0.76 ms)
- search: strata (0.00 ms)
- cursor reuse: strata_cursor_reuse (10.70 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 189.9% behind orjson
- Strata gap in loads/ndjson: 93.0% behind msgspec
- Strata gap in dumps/str: 721.9% behind orjson
- Strata gap in dumps/bytes: 856.5% behind orjson
