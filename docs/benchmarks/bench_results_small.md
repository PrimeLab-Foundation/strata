# Strata Benchmark Results — SMALL
Generated: 2026-02-22 01:26:27

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
- json: benchmarks/data/generated/small/users.json (0.96 MB, 1000 records)
- ndjson: benchmarks/data/generated/small/users.ndjson (0.96 MB, 1000 records, 1000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (0.96 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 3.06     | 4.30        | 4.59     | 234.32 | 336.5    |
| msgspec       | 3.14     | 5.81        | 6.47     | 173.50 | 339.5    |
| ujson         | 4.28     | 6.40        | 6.86     | 157.59 | 339.6    |
| json (stdlib) | 5.92     | 6.86        | 7.64     | 146.98 | 339.6    |
| strata        | 11.54    | 12.65       | 18.59    | 79.69  | 333.3    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 3.13     | 4.22        | 4.30     | 238.47 | 51.4     |
| orjson        | 3.12     | 4.26        | 4.95     | 236.24 | 50.3     |
| ujson         | 4.41     | 6.27        | 6.37     | 160.72 | 53.4     |
| json (stdlib) | 6.11     | 7.16        | 7.25     | 140.59 | 54.4     |
| strata        | 7.93     | 8.38        | 8.85     | 120.19 | 50.2     |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.80     | 0.82        | 0.87     | 1097.73 | 895062         | 42.3     |
| msgspec | 1.03     | 1.04        | 1.05     | 860.02  | 895062         | 42.7     |
| ujson   | 3.93     | 3.97        | 4.03     | 225.42  | 895062         | 45.3     |
| json    | 6.03     | 6.04        | 6.08     | 166.84  | 1008198        | 46.0     |
| strata  | 7.22     | 7.41        | 7.51     | 135.06  | 1000153        | 42.0     |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.78     | 0.79        | 0.80     | 1138.52 | 895062         | 42.3     |
| msgspec | 0.98     | 0.99        | 1.06     | 900.35  | 895062         | 43.4     |
| ujson   | 4.00     | 4.06        | 4.08     | 220.27  | 895062         | 43.6     |
| json    | 6.05     | 6.05        | 6.11     | 166.57  | 1008198        | 44.0     |
| strata  | 7.33     | 7.35        | 7.43     | 136.11  | 1000153        | 41.0     |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.00     | 562927.29 | 1       | 94.9     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 144028.39 | 1       | 95.3     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 136705.39 | 1       | 94.9     |
| Extract all user IDs                        | strata      | 0.03     | 0.03        | 0.04     | 28839.44  | 1000    | 95.2     |
| Extract all user IDs                        | jmespath    | 0.20     | 0.21        | 0.21     | 4875.42   | 1000    | 95.2     |
| Extract all user IDs                        | jsonpath-ng | 1.68     | 1.68        | 1.69     | 600.30    | 1000    | 95.9     |
| Extract all user names                      | strata      | 0.04     | 0.04        | 0.05     | 23491.79  | 1000    | 94.8     |
| Extract all user names                      | jmespath    | 0.21     | 0.22        | 0.22     | 4676.61   | 1000    | 95.0     |
| Extract all user names                      | jsonpath-ng | 1.58     | 1.62        | 1.65     | 621.16    | 1000    | 95.8     |
| Extract nested timestamp field              | strata      | 0.06     | 0.06        | 0.07     | 17100.27  | 1000    | 94.9     |
| Extract nested timestamp field              | jmespath    | 0.59     | 0.59        | 0.59     | 1719.74   | 1000    | 95.0     |
| Extract nested timestamp field              | jsonpath-ng | 2.22     | 2.29        | 2.31     | 440.04    | 1000    | 95.9     |
| Extract order item prices (double wildcard) | strata      | 0.48     | 0.51        | 0.53     | 1959.41   | 12046   | 95.9     |
| Extract order item prices (double wildcard) | jmespath    | 5.62     | 5.63        | 5.80     | 179.08    | 1000    | 96.7     |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.82    | 17.93       | 17.99    | 56.22     | 12046   | 100.6    |
| Filter users by age (numeric predicate)     | strata      | 0.05     | 0.06        | 0.07     | 16393.46  | 794     | 95.0     |
| Filter users by age (numeric predicate)     | jmespath    | 1.12     | 1.14        | 1.15     | 885.03    | 794     | 95.1     |
| Recursively find all prices                 | strata      | 1.46     | 1.52        | 1.55     | 664.56    | 12046   | 96.3     |
| Recursively find all prices                 | jsonpath-ng | 76.04    | 76.69       | 78.67    | 13.15     | 12046   | 99.4     |
| Slice first 10 users, extract IDs           | strata      | 0.00     | 0.00        | 0.00     | 620435.93 | 10      | 94.8     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 148438.63 | 10      | 94.8     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.02     | 64012.79  | 10      | 95.2     |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.00     | 439628.57 | 1       | 45.0     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 188824.83 | 1       | 45.0     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 135047.30 | 1       | 45.3     |
| Extract all user IDs                        | jsonpath-ng | 0.00     | 0.00        | 0.01     | 287765.52 | 0       | 47.0     |
| Extract all user IDs                        | jmespath    | 0.24     | 0.24        | 0.24     | 4227.44   | 1000    | 47.0     |
| Extract all user IDs                        | strata      | 1.14     | 1.16        | 1.20     | 871.71    | 1000    | 46.4     |
| Extract all user names                      | jsonpath-ng | 0.00     | 0.00        | 0.01     | 277844.80 | 0       | 46.6     |
| Extract all user names                      | jmespath    | 0.24     | 0.24        | 0.25     | 4128.53   | 1000    | 46.6     |
| Extract all user names                      | strata      | 1.16     | 1.16        | 1.19     | 867.58    | 1000    | 45.7     |
| Extract nested timestamp field              | jsonpath-ng | 0.00     | 0.00        | 0.01     | 262769.03 | 0       | 46.4     |
| Extract nested timestamp field              | jmespath    | 0.57     | 0.61        | 0.62     | 1647.08   | 1000    | 46.4     |
| Extract nested timestamp field              | strata      | 1.16     | 1.19        | 1.23     | 846.29    | 1000    | 46.4     |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.00     | 0.00        | 0.01     | 262765.04 | 0       | 47.8     |
| Extract order item prices (double wildcard) | strata      | 0.71     | 0.72        | 0.78     | 1407.26   | 12046   | 47.3     |
| Extract order item prices (double wildcard) | jmespath    | 3.64     | 3.70        | 3.71     | 271.88    | 12046   | 47.8     |
| Filter users by age (numeric predicate)     | strata      | 0.29     | 0.29        | 0.35     | 3436.03   | 794     | 47.4     |
| Filter users by age (numeric predicate)     | jmespath    | 1.17     | 1.22        | 1.24     | 825.20    | 794     | 47.5     |
| NDJSON first record id                      | strata      | 0.00     | 0.00        | 0.01     | 274660.56 | 1       | 46.0     |
| NDJSON first record id                      | jmespath    | 0.00     | 0.00        | 0.01     | 221797.89 | 1       | 46.0     |
| NDJSON first record id                      | jsonpath-ng | 0.00     | 0.00        | 0.01     | 213930.50 | 1       | 46.2     |
| Recursively find all prices                 | strata      | 1.88     | 1.89        | 1.98     | 531.79    | 12046   | 47.5     |
| Recursively find all prices                 | jsonpath-ng | 73.60    | 73.74       | 74.06    | 13.66     | 12046   | 51.0     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 145610.57 | 10      | 46.2     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 142176.78 | 10      | 46.2     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.01        | 0.02     | 70471.99  | 10      | 46.5     |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 9.87     | 10.07       | 12.05    | 800.92 | 120.2    |
| strata_cursor_reparse | 74.26    | 77.80       | 82.60    | 103.67 | 674.5    |
- Speedup: 7.73x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 13.50    | 13.70       | 13.82    | 661.83 | 36.6     |
| strata_cursor_reparse | 86.28    | 86.73       | 87.54    | 104.52 | 45.1     |
- Speedup: 6.33x

## Summary
- loads json: orjson (4.30 ms)
- loads ndjson: msgspec (4.22 ms)
- dumps str: orjson (0.82 ms)
- dumps bytes: orjson (0.79 ms)
- search: strata (0.00 ms)
- cursor reuse: strata_cursor_reuse (10.07 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 194.1% behind orjson
- Strata gap in loads/ndjson: 98.4% behind msgspec
- Strata gap in dumps/str: 808.2% behind orjson
- Strata gap in dumps/bytes: 834.7% behind orjson
