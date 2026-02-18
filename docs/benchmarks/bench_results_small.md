# Strata Benchmark Results — SMALL
Generated: 2026-02-18 23:24:26

## Environment
- Commit: a7c872feace49a5ab38ccada8541a1be5cfcd459
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
| orjson        | 2.78     | 3.88        | 4.06     | 259.93 | 349.8    |
| msgspec       | 2.94     | 3.97        | 4.05     | 254.16 | 352.9    |
| ujson         | 3.91     | 5.97        | 6.18     | 168.97 | 352.9    |
| json (stdlib) | 5.23     | 6.41        | 6.43     | 157.33 | 352.9    |
| strata        | 9.89     | 11.29       | 18.10    | 89.28  | 346.4    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 2.92     | 3.88        | 4.08     | 259.46 | 50.5     |
| msgspec       | 3.03     | 3.89        | 4.11     | 259.09 | 51.6     |
| ujson         | 4.11     | 5.77        | 5.81     | 174.51 | 53.7     |
| json (stdlib) | 5.72     | 6.61        | 6.64     | 152.26 | 54.7     |
| strata        | 7.12     | 7.59        | 8.02     | 132.64 | 50.5     |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.74     | 0.78        | 0.88     | 1147.76 | 895062         | 42.2     |
| msgspec | 0.91     | 0.93        | 1.02     | 961.74  | 895062         | 42.4     |
| ujson   | 3.72     | 3.80        | 3.99     | 235.26  | 895062         | 42.4     |
| json    | 5.65     | 5.69        | 5.73     | 177.26  | 1008198        | 42.8     |
| strata  | 6.75     | 6.89        | 7.03     | 145.06  | 1000153        | 41.1     |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.69     | 0.70        | 0.72     | 1281.86 | 895062         | 43.4     |
| msgspec | 0.88     | 0.90        | 0.96     | 991.90  | 895062         | 44.6     |
| ujson   | 3.64     | 3.70        | 3.74     | 242.16  | 895062         | 44.8     |
| json    | 5.67     | 5.68        | 5.69     | 177.56  | 1008198        | 45.3     |
| strata  | 6.66     | 6.74        | 6.83     | 148.43  | 1000153        | 43.1     |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.00     | 237224.50 | 1       | 94.6     |
| Deep path navigation                        | jmespath    | 0.00     | 0.01        | 0.01     | 199959.71 | 1       | 94.6     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 141502.13 | 1       | 95.1     |
| Extract all user IDs                        | strata      | 0.03     | 0.03        | 0.04     | 33699.85  | 1000    | 94.7     |
| Extract all user IDs                        | jmespath    | 0.18     | 0.18        | 0.19     | 5488.05   | 1000    | 94.8     |
| Extract all user IDs                        | jsonpath-ng | 1.50     | 1.51        | 1.53     | 668.46    | 1000    | 95.5     |
| Extract all user names                      | strata      | 0.06     | 0.09        | 0.13     | 10626.59  | 1000    | 94.8     |
| Extract all user names                      | jmespath    | 0.18     | 0.19        | 0.19     | 5427.71   | 1000    | 95.0     |
| Extract all user names                      | jsonpath-ng | 1.61     | 1.63        | 1.89     | 617.42    | 1000    | 95.8     |
| Extract nested timestamp field              | strata      | 0.05     | 0.06        | 0.09     | 18220.57  | 1000    | 94.8     |
| Extract nested timestamp field              | jmespath    | 0.51     | 0.53        | 0.55     | 1893.92   | 1000    | 95.0     |
| Extract nested timestamp field              | jsonpath-ng | 2.09     | 2.09        | 2.10     | 481.87    | 1000    | 95.9     |
| Extract order item prices (double wildcard) | strata      | 0.48     | 0.48        | 0.52     | 2106.27   | 12046   | 95.7     |
| Extract order item prices (double wildcard) | jmespath    | 5.15     | 5.21        | 5.24     | 193.60    | 1000    | 96.5     |
| Extract order item prices (double wildcard) | jsonpath-ng | 16.89    | 17.03       | 17.12    | 59.20     | 12046   | 100.3    |
| Filter users by age (numeric predicate)     | strata      | 0.06     | 0.06        | 0.06     | 16908.98  | 794     | 94.9     |
| Filter users by age (numeric predicate)     | jmespath    | 1.05     | 1.06        | 1.07     | 953.94    | 794     | 95.0     |
| Recursively find all prices                 | strata      | 1.42     | 1.47        | 1.51     | 687.92    | 12046   | 96.2     |
| Recursively find all prices                 | jsonpath-ng | 70.36    | 70.66       | 71.56    | 14.27     | 12046   | 99.2     |
| Slice first 10 users, extract IDs           | strata      | 0.00     | 0.00        | 0.00     | 562616.44 | 10      | 94.6     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 163481.58 | 10      | 94.6     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.01        | 0.02     | 72013.89  | 10      | 95.0     |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.00     | 447646.84 | 1       | 45.9     |
| Deep path navigation                        | jmespath    | 0.00     | 0.00        | 0.01     | 210224.17 | 1       | 46.0     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 152029.07 | 1       | 46.2     |
| Extract all user IDs                        | jsonpath-ng | 0.00     | 0.00        | 0.01     | 309906.88 | 0       | 47.4     |
| Extract all user IDs                        | jmespath    | 0.22     | 0.22        | 0.22     | 4600.77   | 1000    | 47.4     |
| Extract all user IDs                        | strata      | 1.02     | 1.02        | 1.07     | 982.78    | 1000    | 46.7     |
| Extract all user names                      | jsonpath-ng | 0.00     | 0.00        | 0.01     | 306042.57 | 0       | 46.4     |
| Extract all user names                      | jmespath    | 0.22     | 0.22        | 0.28     | 4646.77   | 1000    | 46.3     |
| Extract all user names                      | strata      | 1.01     | 1.03        | 1.13     | 980.99    | 1000    | 45.7     |
| Extract nested timestamp field              | jsonpath-ng | 0.00     | 0.00        | 0.01     | 262697.23 | 0       | 46.7     |
| Extract nested timestamp field              | jmespath    | 0.56     | 0.58        | 0.67     | 1740.15   | 1000    | 46.7     |
| Extract nested timestamp field              | strata      | 1.22     | 1.29        | 1.37     | 782.46    | 1000    | 46.7     |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.00     | 0.00        | 0.01     | 277844.80 | 0       | 47.6     |
| Extract order item prices (double wildcard) | strata      | 0.69     | 0.69        | 0.78     | 1459.08   | 12046   | 47.4     |
| Extract order item prices (double wildcard) | jmespath    | 3.39     | 3.44        | 3.48     | 292.86    | 12046   | 47.6     |
| Filter users by age (numeric predicate)     | strata      | 0.27     | 0.28        | 0.30     | 3614.85   | 794     | 46.2     |
| Filter users by age (numeric predicate)     | jmespath    | 1.04     | 1.07        | 1.07     | 943.24    | 794     | 46.2     |
| NDJSON first record id                      | strata      | 0.00     | 0.00        | 0.00     | 549486.85 | 1       | 46.1     |
| NDJSON first record id                      | jmespath    | 0.00     | 0.00        | 0.01     | 228026.58 | 1       | 46.1     |
| NDJSON first record id                      | jsonpath-ng | 0.00     | 0.00        | 0.01     | 203102.77 | 1       | 46.3     |
| Recursively find all prices                 | strata      | 1.79     | 1.80        | 1.94     | 560.37    | 12046   | 47.4     |
| Recursively find all prices                 | jsonpath-ng | 69.83    | 70.54       | 70.58    | 14.28     | 12046   | 50.8     |
| Slice first 10 users, extract IDs           | strata      | 0.00     | 0.01        | 0.01     | 191845.81 | 10      | 45.7     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 155959.02 | 10      | 45.7     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.01        | 0.01     | 73026.82  | 10      | 46.0     |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 9.21     | 9.64        | 11.48    | 836.38 | 120.1    |
| strata_cursor_reparse | 69.58    | 71.79       | 78.77    | 112.35 | 674.4    |
- Speedup: 7.44x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 12.59    | 12.66       | 12.75    | 715.87 | 36.5     |
| strata_cursor_reparse | 80.52    | 81.24       | 81.40    | 111.57 | 45.1     |
- Speedup: 6.42x

## Summary
- loads json: orjson (3.88 ms)
- loads ndjson: orjson (3.88 ms)
- dumps str: orjson (0.78 ms)
- dumps bytes: orjson (0.70 ms)
- search: strata (0.00 ms)
- cursor reuse: strata_cursor_reuse (9.64 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 191.2% behind orjson
- Strata gap in loads/ndjson: 95.6% behind orjson
- Strata gap in dumps/str: 784.2% behind orjson
- Strata gap in dumps/bytes: 865.0% behind orjson
