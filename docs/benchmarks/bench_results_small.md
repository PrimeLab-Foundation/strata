# Strata Benchmark Results — SMALL
Generated: 2026-02-16 01:25:00

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
- json: benchmarks/data/generated/small/users.json (0.96 MB, 1000 records)
- ndjson: benchmarks/data/generated/small/users.ndjson (0.96 MB, 1000 records, 1000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (0.96 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 3.30     | 4.35        | 5.24     | 232.01 | 360.2    |
| orjson        | 3.01     | 4.71        | 5.77     | 214.27 | 357.1    |
| ujson         | 4.05     | 6.27        | 8.02     | 160.88 | 360.3    |
| json (stdlib) | 5.72     | 7.46        | 7.63     | 135.22 | 360.3    |
| strata        | 9.31     | 12.63       | 15.59    | 79.83  | 353.7    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 3.62     | 4.02        | 4.19     | 250.60 | 48.3     |
| msgspec       | 3.26     | 4.43        | 4.98     | 227.15 | 49.3     |
| ujson         | 4.21     | 6.18        | 7.06     | 163.09 | 51.4     |
| json (stdlib) | 5.88     | 7.22        | 7.83     | 139.56 | 52.4     |
| strata        | 6.97     | 7.33        | 7.62     | 137.32 | 48.2     |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.73     | 0.75        | 0.78     | 1199.28 | 895062         | 42.1     |
| msgspec | 0.92     | 0.96        | 1.00     | 935.32  | 895062         | 42.3     |
| ujson   | 3.74     | 3.79        | 3.83     | 236.31  | 895062         | 42.3     |
| json    | 5.77     | 5.80        | 5.86     | 173.91  | 1008198        | 42.8     |
| strata  | 6.82     | 7.01        | 7.46     | 142.72  | 1000153        | 41.0     |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.70     | 0.71        | 0.77     | 1264.29 | 895062         | 41.4     |
| msgspec | 0.91     | 0.92        | 0.98     | 975.68  | 895062         | 42.5     |
| ujson   | 3.78     | 3.86        | 3.96     | 231.73  | 895062         | 42.7     |
| json    | 5.59     | 5.80        | 5.98     | 173.83  | 1008198        | 43.1     |
| strata  | 7.00     | 7.01        | 7.10     | 142.69  | 1000153        | 41.1     |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.01     | 504102.50 | 1       | 94.8     |
| Deep path navigation                        | jmespath    | 0.00     | 0.01        | 0.01     | 201639.24 | 1       | 94.9     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 65929.78  | 1       | 95.3     |
| Extract all user IDs                        | strata      | 0.03     | 0.04        | 0.04     | 28300.29  | 1000    | 94.7     |
| Extract all user IDs                        | jmespath    | 0.18     | 0.19        | 0.20     | 5306.31   | 1000    | 94.8     |
| Extract all user IDs                        | jsonpath-ng | 1.56     | 1.58        | 1.59     | 636.32    | 1000    | 95.7     |
| Extract all user names                      | strata      | 0.08     | 0.09        | 0.09     | 11489.44  | 1000    | 95.0     |
| Extract all user names                      | jmespath    | 0.20     | 0.21        | 0.21     | 4870.52   | 1000    | 95.1     |
| Extract all user names                      | jsonpath-ng | 1.60     | 1.64        | 1.74     | 615.18    | 1000    | 95.8     |
| Extract nested timestamp field              | strata      | 0.06     | 0.07        | 0.08     | 14763.05  | 1000    | 94.7     |
| Extract nested timestamp field              | jmespath    | 0.52     | 0.54        | 0.56     | 1863.73   | 1000    | 94.9     |
| Extract nested timestamp field              | jsonpath-ng | 2.08     | 2.14        | 2.45     | 470.47    | 1000    | 95.9     |
| Extract order item prices (double wildcard) | strata      | 0.53     | 0.68        | 0.82     | 1491.60   | 12046   | 96.0     |
| Extract order item prices (double wildcard) | jmespath    | 5.32     | 5.35        | 5.39     | 188.36    | 1000    | 96.7     |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.02    | 17.18       | 18.12    | 58.69     | 12046   | 100.6    |
| Filter users by age (numeric predicate)     | strata      | 0.06     | 0.07        | 0.09     | 14890.38  | 794     | 95.1     |
| Filter users by age (numeric predicate)     | jmespath    | 1.05     | 1.17        | 1.37     | 859.38    | 794     | 95.2     |
| Recursively find all prices                 | strata      | 1.68     | 1.68        | 1.74     | 599.89    | 12046   | 96.2     |
| Recursively find all prices                 | jsonpath-ng | 71.91    | 72.15       | 72.32    | 13.97     | 12046   | 99.2     |
| Slice first 10 users, extract IDs           | strata      | 0.00     | 0.01        | 0.01     | 159197.70 | 10      | 94.8     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 130090.05 | 10      | 94.8     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.02     | 64012.56  | 10      | 95.3     |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 57827.87  | 1       | 46.3     |
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 52207.50  | 1       | 46.3     |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 41391.84  | 1       | 46.5     |
| Extract all user IDs                        | jsonpath-ng | 0.00     | 0.00        | 0.01     | 309904.11 | 0       | 47.0     |
| Extract all user IDs                        | jmespath    | 0.21     | 0.21        | 0.22     | 4727.65   | 1000    | 47.0     |
| Extract all user IDs                        | strata      | 1.05     | 1.06        | 1.12     | 945.75    | 1000    | 46.5     |
| Extract all user names                      | jsonpath-ng | 0.00     | 0.00        | 0.01     | 287767.92 | 0       | 46.2     |
| Extract all user names                      | jmespath    | 0.22     | 0.23        | 0.24     | 4419.10   | 1000    | 46.2     |
| Extract all user names                      | strata      | 1.09     | 1.14        | 1.21     | 887.32    | 1000    | 45.7     |
| Extract nested timestamp field              | jsonpath-ng | 0.00     | 0.01        | 0.01     | 126562.74 | 0       | 46.8     |
| Extract nested timestamp field              | jmespath    | 0.57     | 0.60        | 0.66     | 1690.98   | 1000    | 46.8     |
| Extract nested timestamp field              | strata      | 1.11     | 1.15        | 1.21     | 874.10    | 1000    | 46.6     |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.01     | 0.01        | 0.01     | 122693.06 | 0       | 47.9     |
| Extract order item prices (double wildcard) | strata      | 0.74     | 0.78        | 0.89     | 1291.06   | 12046   | 47.1     |
| Extract order item prices (double wildcard) | jmespath    | 3.68     | 3.76        | 3.81     | 267.57    | 12046   | 47.9     |
| Filter users by age (numeric predicate)     | strata      | 0.28     | 0.33        | 0.35     | 3071.86   | 794     | 46.5     |
| Filter users by age (numeric predicate)     | jmespath    | 1.09     | 1.10        | 1.11     | 913.31    | 794     | 46.6     |
| NDJSON first record id                      | strata      | 0.00     | 0.00        | 0.00     | 464999.82 | 1       | 45.1     |
| NDJSON first record id                      | jsonpath-ng | 0.00     | 0.00        | 0.01     | 203143.31 | 1       | 45.4     |
| NDJSON first record id                      | jmespath    | 0.00     | 0.01        | 0.01     | 177729.01 | 1       | 45.2     |
| Recursively find all prices                 | strata      | 1.86     | 1.88        | 1.93     | 537.07    | 12046   | 46.5     |
| Recursively find all prices                 | jsonpath-ng | 72.37    | 72.80       | 73.07    | 13.84     | 12046   | 50.0     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 162239.61 | 10      | 46.0     |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 57553.58  | 10      | 46.1     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 53009.84  | 10      | 46.3     |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 11.21    | 11.64       | 12.85    | 693.05 | 103.3    |
| strata_cursor_reparse | 72.65    | 79.42       | 87.83    | 101.56 | 711.6    |
- Speedup: 6.82x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 13.17    | 14.25       | 14.53    | 636.05 | 36.6     |
| strata_cursor_reparse | 83.15    | 84.48       | 84.75    | 107.30 | 45.2     |
- Speedup: 5.93x

## Summary
- loads json: msgspec (4.35 ms)
- loads ndjson: orjson (4.02 ms)
- dumps str: orjson (0.75 ms)
- dumps bytes: orjson (0.71 ms)
- search: strata (0.00 ms)
- cursor reuse: strata_cursor_reuse (11.64 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 190.6% behind msgspec
- Strata gap in loads/ndjson: 82.5% behind orjson
- Strata gap in dumps/str: 839.0% behind orjson
- Strata gap in dumps/bytes: 890.0% behind orjson
