# Strata Benchmark Results — SMALL
Generated: 2026-02-19 17:23:55

## Environment
- Commit: d3e6e38abca804fbb6f76be6e473253e4d2eb6c0
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
| msgspec       | 4.15     | 4.75        | 5.39     | 212.23 | 425.6    |
| orjson        | 3.18     | 5.00        | 5.78     | 201.61 | 422.6    |
| ujson         | 5.46     | 7.41        | 8.00     | 136.02 | 425.7    |
| json (stdlib) | 5.86     | 7.70        | 7.86     | 130.88 | 425.7    |
| strata        | 11.20    | 13.40       | 18.06    | 75.25  | 418.7    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 3.24     | 4.46        | 4.54     | 225.86 | 53.3     |
| orjson        | 3.32     | 4.56        | 4.79     | 220.68 | 52.2     |
| ujson         | 4.75     | 6.62        | 7.08     | 152.12 | 55.4     |
| strata        | 7.51     | 8.02        | 8.25     | 125.54 | 51.8     |
| json (stdlib) | 6.87     | 8.37        | 8.56     | 120.29 | 56.4     |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 0.79     | 0.91        | 2.81     | 987.16 | 895062         | 43.9     |
| msgspec | 1.07     | 1.10        | 1.21     | 810.38 | 895062         | 44.3     |
| ujson   | 3.93     | 4.09        | 4.41     | 218.96 | 895062         | 46.1     |
| json    | 6.10     | 6.32        | 6.41     | 159.51 | 1008198        | 46.3     |
| strata  | 7.06     | 7.36        | 7.66     | 135.83 | 1000153        | 41.1     |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.76     | 0.80        | 0.82     | 1125.04 | 895062         | 43.4     |
| msgspec | 0.96     | 0.99        | 1.08     | 906.09  | 895062         | 44.6     |
| ujson   | 4.02     | 4.05        | 4.19     | 221.04  | 895062         | 44.8     |
| json    | 6.20     | 6.22        | 6.39     | 162.00  | 1008198        | 45.2     |
| strata  | 7.04     | 7.35        | 7.42     | 136.07  | 1000153        | 43.1     |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.01     | 322623.91 | 1       | 94.7     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 146647.73 | 1       | 94.8     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.03     | 115777.82 | 1       | 95.2     |
| Extract all user IDs                        | strata      | 0.03     | 0.03        | 0.03     | 33747.19  | 1000    | 94.9     |
| Extract all user IDs                        | jmespath    | 0.19     | 0.19        | 0.24     | 5332.04   | 1000    | 95.0     |
| Extract all user IDs                        | jsonpath-ng | 1.68     | 1.75        | 2.06     | 577.39    | 1000    | 95.7     |
| Extract all user names                      | strata      | 0.05     | 0.06        | 0.07     | 17271.06  | 1000    | 94.7     |
| Extract all user names                      | jmespath    | 0.20     | 0.21        | 0.22     | 4898.14   | 1000    | 94.8     |
| Extract all user names                      | jsonpath-ng | 1.72     | 1.81        | 2.10     | 556.17    | 1000    | 95.5     |
| Extract nested timestamp field              | strata      | 0.06     | 0.12        | 0.12     | 8747.92   | 1000    | 94.9     |
| Extract nested timestamp field              | jmespath    | 0.58     | 0.60        | 0.60     | 1691.61   | 1000    | 95.1     |
| Extract nested timestamp field              | jsonpath-ng | 2.25     | 2.29        | 2.65     | 439.98    | 1000    | 96.1     |
| Extract order item prices (double wildcard) | strata      | 0.56     | 0.60        | 0.74     | 1694.09   | 12046   | 95.8     |
| Extract order item prices (double wildcard) | jmespath    | 5.84     | 6.16        | 6.43     | 163.64    | 1000    | 96.6     |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.83    | 19.16       | 19.36    | 52.62     | 12046   | 100.5    |
| Filter users by age (numeric predicate)     | strata      | 0.08     | 0.09        | 0.12     | 10706.49  | 794     | 95.0     |
| Filter users by age (numeric predicate)     | jmespath    | 1.10     | 1.15        | 1.18     | 879.11    | 794     | 95.1     |
| Recursively find all prices                 | strata      | 1.92     | 1.96        | 2.36     | 515.11    | 12046   | 96.5     |
| Recursively find all prices                 | jsonpath-ng | 75.25    | 75.46       | 75.63    | 13.36     | 12046   | 99.7     |
| Slice first 10 users, extract IDs           | strata      | 0.00     | 0.00        | 0.01     | 237221.25 | 10      | 94.9     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 123462.73 | 10      | 94.9     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.02     | 65749.21  | 10      | 95.4     |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 140550.73 | 1       | 46.0     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 132821.65 | 1       | 46.1     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 51214.68  | 1       | 46.3     |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 95922.37  | 0       | 46.9     |
| Extract all user IDs                        | jmespath    | 0.25     | 0.26        | 0.27     | 3920.92   | 1000    | 46.9     |
| Extract all user IDs                        | strata      | 1.13     | 1.14        | 1.80     | 880.95    | 1000    | 46.1     |
| Extract all user names                      | jsonpath-ng | 0.00     | 0.00        | 0.01     | 244166.41 | 0       | 47.1     |
| Extract all user names                      | jmespath    | 0.23     | 0.23        | 0.24     | 4407.82   | 1000    | 47.1     |
| Extract all user names                      | strata      | 1.13     | 1.19        | 1.27     | 846.26    | 1000    | 46.5     |
| Extract nested timestamp field              | jsonpath-ng | 0.00     | 0.01        | 0.01     | 106954.01 | 0       | 46.8     |
| Extract nested timestamp field              | jmespath    | 0.61     | 0.65        | 0.67     | 1560.02   | 1000    | 46.8     |
| Extract nested timestamp field              | strata      | 1.15     | 1.32        | 1.99     | 762.54    | 1000    | 46.7     |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.00     | 0.00        | 0.01     | 254468.38 | 0       | 46.6     |
| Extract order item prices (double wildcard) | strata      | 0.69     | 0.70        | 0.80     | 1449.02   | 12046   | 46.4     |
| Extract order item prices (double wildcard) | jmespath    | 3.65     | 3.68        | 3.69     | 273.65    | 12046   | 46.6     |
| Filter users by age (numeric predicate)     | strata      | 0.29     | 0.31        | 0.33     | 3268.76   | 794     | 46.7     |
| Filter users by age (numeric predicate)     | jmespath    | 1.20     | 1.24        | 1.25     | 814.00    | 794     | 46.8     |
| NDJSON first record id                      | strata      | 0.00     | 0.01        | 0.01     | 183123.51 | 1       | 45.8     |
| NDJSON first record id                      | jsonpath-ng | 0.00     | 0.01        | 0.01     | 180369.02 | 1       | 46.0     |
| NDJSON first record id                      | jmespath    | 0.00     | 0.01        | 0.01     | 177760.05 | 1       | 45.8     |
| Recursively find all prices                 | strata      | 1.87     | 1.99        | 2.10     | 505.70    | 12046   | 46.9     |
| Recursively find all prices                 | jsonpath-ng | 74.29    | 75.08       | 75.20    | 13.42     | 12046   | 50.3     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 125898.33 | 10      | 45.0     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.01        | 0.02     | 68089.88  | 10      | 45.3     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.02        | 0.02     | 58108.00  | 10      | 45.0     |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 9.86     | 10.33       | 11.73    | 780.68 | 120.4    |
| strata_cursor_reparse | 82.44    | 83.30       | 85.95    | 96.82  | 702.5    |
- Speedup: 8.06x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 13.25    | 13.64       | 13.71    | 664.75 | 36.5     |
| strata_cursor_reparse | 85.32    | 88.70       | 89.55    | 102.19 | 45.0     |
- Speedup: 6.50x

## Summary
- loads json: msgspec (4.75 ms)
- loads ndjson: msgspec (4.46 ms)
- dumps str: orjson (0.91 ms)
- dumps bytes: orjson (0.80 ms)
- search: strata (0.00 ms)
- cursor reuse: strata_cursor_reuse (10.33 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 182.0% behind msgspec
- Strata gap in loads/ndjson: 79.9% behind msgspec
- Strata gap in dumps/str: 712.1% behind orjson
- Strata gap in dumps/bytes: 823.9% behind orjson
