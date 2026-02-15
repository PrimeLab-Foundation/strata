# Strata Benchmark Results — DUMPS-OPTIMIZED
Generated: 2026-02-15 00:28:18

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
| msgspec       | 3.80     | 5.26        | 5.77     | 191.68 | 327.5    |
| orjson        | 4.61     | 5.38        | 6.71     | 187.37 | 326.5    |
| ujson         | 4.76     | 7.07        | 7.17     | 142.65 | 329.6    |
| json (stdlib) | 6.35     | 7.34        | 8.64     | 137.43 | 329.6    |
| strata        | 10.20    | 12.66       | 16.58    | 79.64  | 322.8    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 3.51     | 4.73        | 5.29     | 212.91 | 329.8    |
| orjson        | 3.36     | 4.86        | 5.13     | 207.21 | 329.8    |
| strata        | 6.75     | 7.28        | 7.33     | 138.44 | 329.7    |
| ujson         | 5.00     | 7.29        | 7.72     | 138.08 | 331.8    |
| json (stdlib) | 6.29     | 8.11        | 8.57     | 124.14 | 331.8    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.73     | 0.73        | 0.73     | 1223.04 | 895062         | 333.9    |
| msgspec | 0.93     | 0.94        | 0.98     | 954.48  | 895062         | 333.9    |
| strata  | 3.50     | 3.58        | 3.76     | 279.42  | 1000153        | 333.8    |
| ujson   | 3.66     | 3.72        | 3.74     | 240.44  | 895062         | 333.9    |
| json    | 5.80     | 5.91        | 5.96     | 170.61  | 1008198        | 334.1    |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.70     | 0.72        | 0.75     | 1235.21 | 895062         | 334.1    |
| msgspec | 0.93     | 0.95        | 0.97     | 944.49  | 895062         | 334.1    |
| strata  | 3.52     | 3.55        | 3.56     | 281.90  | 1000153        | 334.1    |
| ujson   | 3.61     | 3.72        | 3.79     | 240.31  | 895062         | 334.1    |
| json    | 5.55     | 5.83        | 5.85     | 172.90  | 1008198        | 334.1    |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 114672.13 | 1       | 485.9    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 107061.70 | 1       | 485.9    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 105670.14 | 1       | 485.9    |
| Extract all user IDs                        | jmespath    | 0.20     | 0.20        | 0.21     | 5086.57   | 1000    | 372.8    |
| Extract all user IDs                        | strata      | 1.13     | 1.17        | 1.21     | 859.63    | 1000    | 372.7    |
| Extract all user IDs                        | jsonpath-ng | 2.49     | 2.56        | 3.23     | 393.57    | 1000    | 372.8    |
| Extract all user names                      | jmespath    | 0.20     | 0.20        | 0.20     | 5066.32   | 1000    | 502.1    |
| Extract all user names                      | strata      | 1.13     | 1.62        | 1.65     | 620.70    | 1000    | 502.1    |
| Extract all user names                      | jsonpath-ng | 2.50     | 2.55        | 2.61     | 395.91    | 1000    | 502.1    |
| Extract nested timestamp field              | jmespath    | 0.52     | 0.56        | 0.70     | 1787.59   | 1000    | 432.5    |
| Extract nested timestamp field              | strata      | 1.24     | 1.30        | 1.31     | 775.04    | 1000    | 432.5    |
| Extract nested timestamp field              | jsonpath-ng | 3.15     | 3.16        | 3.18     | 318.92    | 1000    | 432.5    |
| Extract order item prices (double wildcard) | jmespath    | 6.55     | 6.73        | 6.75     | 149.79    | 1000    | 454.4    |
| Extract order item prices (double wildcard) | strata      | 13.78    | 13.84       | 14.53    | 72.87     | 12046   | 454.4    |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.47    | 18.53       | 18.55    | 54.40     | 12046   | 454.5    |
| Filter users by age (numeric predicate)     | strata      | 1.04     | 1.08        | 1.11     | 934.74    | 794     | 526.2    |
| Filter users by age (numeric predicate)     | jmespath    | 1.12     | 1.13        | 1.14     | 893.59    | 794     | 526.2    |
| Recursively find all prices                 | strata      | 13.33    | 13.50       | 13.57    | 74.69     | 12046   | 542.0    |
| Recursively find all prices                 | jsonpath-ng | 72.30    | 72.41       | 73.01    | 13.92     | 12046   | 542.0    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 121586.81 | 10      | 565.0    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 50942.29  | 10      | 565.0    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 39279.95  | 10      | 565.0    |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 113485.74 | 1       | 1025.5   |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 106479.15 | 1       | 1025.5   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 61507.57  | 1       | 1025.5   |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 91913.40  | 0       | 1025.4   |
| Extract all user IDs                        | jmespath    | 0.28     | 0.32        | 0.38     | 3196.58   | 1000    | 1025.4   |
| Extract all user IDs                        | strata      | 3.33     | 3.63        | 4.33     | 277.33    | 1000    | 1025.4   |
| Extract all user names                      | jsonpath-ng | 0.00     | 0.01        | 0.01     | 131366.56 | 0       | 1025.5   |
| Extract all user names                      | jmespath    | 0.25     | 0.27        | 0.33     | 3756.41   | 1000    | 1025.5   |
| Extract all user names                      | strata      | 3.25     | 3.95        | 4.14     | 255.26    | 1000    | 1025.5   |
| Extract nested timestamp field              | jsonpath-ng | 0.00     | 0.01        | 0.01     | 194926.59 | 0       | 1025.4   |
| Extract nested timestamp field              | jmespath    | 0.58     | 0.59        | 0.65     | 1721.69   | 1000    | 1025.4   |
| Extract nested timestamp field              | strata      | 3.19     | 3.52        | 4.39     | 285.96    | 1000    | 1025.4   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.00     | 0.01        | 0.01     | 175163.40 | 0       | 1025.5   |
| Extract order item prices (double wildcard) | jmespath    | 3.61     | 3.69        | 3.77     | 273.24    | 12046   | 1025.5   |
| Extract order item prices (double wildcard) | strata      | 14.11    | 14.54       | 14.59    | 69.26     | 12046   | 1025.4   |
| Filter users by age (numeric predicate)     | strata      | 1.07     | 1.09        | 1.10     | 921.03    | 794     | 1025.5   |
| Filter users by age (numeric predicate)     | jmespath    | 1.15     | 1.16        | 1.21     | 868.64    | 794     | 1025.5   |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.01        | 0.01     | 181736.78 | 1       | 1024.1   |
| NDJSON first record id                      | strata      | 0.01     | 0.01        | 0.02     | 89863.21  | 1       | 1024.1   |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.01     | 85412.74  | 1       | 1024.1   |
| Recursively find all prices                 | strata      | 15.06    | 15.39       | 15.68    | 65.46     | 12046   | 1025.5   |
| Recursively find all prices                 | jsonpath-ng | 72.96    | 73.41       | 73.45    | 13.72     | 12046   | 1025.6   |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 109369.80 | 10      | 1025.6   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 62784.37  | 10      | 1025.6   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 46575.14  | 10      | 1025.6   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 41.50    | 41.62       | 41.99    | 193.79 | 586.8    |
| strata_cursor_reparse | 104.73   | 105.77      | 109.84   | 76.26  | 1021.7   |
- Speedup: 2.54x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 46.32    | 46.67       | 47.08    | 194.23 | 1025.6   |
| strata_cursor_reparse | 118.92   | 119.26      | 121.28   | 76.01  | 1027.5   |
- Speedup: 2.56x

## Summary
- loads json: msgspec (5.26 ms)
- loads ndjson: msgspec (4.73 ms)
- dumps str: orjson (0.73 ms)
- dumps bytes: orjson (0.72 ms)
- search: jsonpath-ng (0.01 ms)
- cursor reuse: strata_cursor_reuse (41.62 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 140.7% behind msgspec
- Strata gap in loads/ndjson: 53.8% behind msgspec
- Strata gap in dumps/str: 389.1% behind orjson
- Strata gap in dumps/bytes: 389.6% behind orjson
