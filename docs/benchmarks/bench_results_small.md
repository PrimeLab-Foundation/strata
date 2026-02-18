# Strata Benchmark Results — SMALL
Generated: 2026-02-19 01:03:58

## Environment
- Commit: abb10ebf0228fd507c2dd560e30b883515376188
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
| orjson        | 2.88     | 5.01        | 5.63     | 201.35 | 380.6    |
| msgspec       | 3.11     | 5.29        | 6.01     | 190.68 | 383.7    |
| json (stdlib) | 5.34     | 6.78        | 7.29     | 148.78 | 383.8    |
| ujson         | 4.13     | 7.11        | 7.22     | 141.77 | 383.8    |
| strata        | 13.65    | 16.25       | 25.21    | 62.04  | 377.5    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 3.08     | 4.16        | 4.90     | 241.86 | 51.4     |
| orjson        | 3.13     | 4.20        | 4.38     | 239.54 | 50.3     |
| ujson         | 4.23     | 6.35        | 7.11     | 158.70 | 53.5     |
| json (stdlib) | 6.57     | 7.72        | 9.00     | 130.41 | 54.5     |
| strata        | 7.21     | 7.91        | 7.95     | 127.39 | 50.2     |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.72     | 0.79        | 0.99     | 1132.93 | 895062         | 46.0     |
| msgspec | 0.89     | 0.97        | 1.20     | 924.29  | 895062         | 47.1     |
| ujson   | 3.55     | 3.72        | 3.90     | 240.88  | 895062         | 47.2     |
| json    | 5.70     | 5.74        | 5.74     | 175.58  | 1008198        | 47.5     |
| strata  | 6.80     | 6.89        | 7.00     | 145.23  | 1000153        | 43.1     |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.69     | 0.79        | 0.83     | 1138.39 | 895062         | 44.4     |
| msgspec | 0.89     | 0.95        | 1.02     | 941.10  | 895062         | 45.4     |
| ujson   | 3.63     | 3.68        | 3.86     | 243.23  | 895062         | 45.5     |
| json    | 5.49     | 5.67        | 5.74     | 177.91  | 1008198        | 46.0     |
| strata  | 6.95     | 7.07        | 7.17     | 141.44  | 1000153        | 43.3     |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.00     | 234955.84 | 1       | 94.8     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 112021.87 | 1       | 95.2     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 97959.50  | 1       | 94.8     |
| Extract all user IDs                        | strata      | 0.03     | 0.04        | 0.04     | 26914.74  | 1000    | 94.7     |
| Extract all user IDs                        | jmespath    | 0.20     | 0.20        | 0.23     | 4967.52   | 1000    | 94.9     |
| Extract all user IDs                        | jsonpath-ng | 1.53     | 1.59        | 1.63     | 634.40    | 1000    | 95.7     |
| Extract all user names                      | strata      | 0.05     | 0.12        | 0.13     | 8614.06   | 1000    | 94.8     |
| Extract all user names                      | jmespath    | 0.21     | 0.25        | 0.29     | 3966.69   | 1000    | 94.9     |
| Extract all user names                      | jsonpath-ng | 1.70     | 2.07        | 2.50     | 486.02    | 1000    | 95.7     |
| Extract nested timestamp field              | strata      | 0.06     | 0.09        | 0.17     | 11079.10  | 1000    | 94.8     |
| Extract nested timestamp field              | jmespath    | 0.52     | 0.53        | 0.54     | 1898.53   | 1000    | 94.9     |
| Extract nested timestamp field              | jsonpath-ng | 2.03     | 2.08        | 2.11     | 485.28    | 1000    | 95.8     |
| Extract order item prices (double wildcard) | strata      | 0.50     | 0.52        | 0.58     | 1946.80   | 12046   | 95.9     |
| Extract order item prices (double wildcard) | jmespath    | 5.31     | 5.32        | 5.46     | 189.63    | 1000    | 96.7     |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.43    | 17.63       | 18.29    | 57.18     | 12046   | 100.8    |
| Filter users by age (numeric predicate)     | strata      | 0.06     | 0.10        | 0.14     | 10149.68  | 794     | 94.7     |
| Filter users by age (numeric predicate)     | jmespath    | 1.09     | 1.11        | 1.25     | 907.13    | 794     | 94.8     |
| Recursively find all prices                 | strata      | 1.82     | 1.87        | 2.27     | 538.71    | 12046   | 96.6     |
| Recursively find all prices                 | jsonpath-ng | 72.45    | 72.66       | 75.28    | 13.88     | 12046   | 99.6     |
| Slice first 10 users, extract IDs           | strata      | 0.00     | 0.01        | 0.02     | 135930.80 | 10      | 94.9     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 100397.69 | 10      | 94.9     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 35740.36  | 10      | 95.4     |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.01     | 187385.24 | 1       | 45.2     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 170218.00 | 1       | 45.2     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 123338.92 | 1       | 45.5     |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 83066.93  | 0       | 46.6     |
| Extract all user IDs                        | jmespath    | 0.23     | 0.24        | 0.28     | 4218.58   | 1000    | 46.5     |
| Extract all user IDs                        | strata      | 1.38     | 1.49        | 1.76     | 674.46    | 1000    | 45.9     |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.01        | 0.03     | 116223.41 | 0       | 47.3     |
| Extract all user names                      | jmespath    | 0.26     | 0.29        | 0.31     | 3527.80   | 1000    | 47.3     |
| Extract all user names                      | strata      | 1.03     | 1.12        | 1.45     | 895.74    | 1000    | 46.7     |
| Extract nested timestamp field              | jsonpath-ng | 0.00     | 0.00        | 0.01     | 251794.83 | 0       | 46.8     |
| Extract nested timestamp field              | jmespath    | 0.57     | 0.58        | 0.62     | 1736.78   | 1000    | 46.8     |
| Extract nested timestamp field              | strata      | 1.18     | 1.49        | 1.50     | 676.25    | 1000    | 46.8     |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.00     | 0.00        | 0.01     | 212040.35 | 0       | 48.0     |
| Extract order item prices (double wildcard) | strata      | 0.69     | 0.70        | 0.78     | 1428.89   | 12046   | 47.6     |
| Extract order item prices (double wildcard) | jmespath    | 3.43     | 3.53        | 3.65     | 285.36    | 12046   | 48.0     |
| Filter users by age (numeric predicate)     | strata      | 0.28     | 0.36        | 0.36     | 2763.20   | 794     | 45.6     |
| Filter users by age (numeric predicate)     | jmespath    | 1.13     | 1.14        | 1.17     | 881.79    | 794     | 45.8     |
| NDJSON first record id                      | strata      | 0.00     | 0.00        | 0.01     | 350326.79 | 1       | 46.0     |
| NDJSON first record id                      | jmespath    | 0.00     | 0.01        | 0.02     | 159038.06 | 1       | 46.0     |
| NDJSON first record id                      | jsonpath-ng | 0.00     | 0.01        | 0.01     | 114557.52 | 1       | 46.2     |
| Recursively find all prices                 | strata      | 2.02     | 2.14        | 2.17     | 470.05    | 12046   | 47.2     |
| Recursively find all prices                 | jsonpath-ng | 71.11    | 73.33       | 78.94    | 13.73     | 12046   | 50.9     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 96307.84  | 10      | 46.2     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.02     | 68094.44  | 10      | 46.2     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.04     | 66771.92  | 10      | 46.4     |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 10.13    | 10.98       | 12.88    | 734.45 | 120.1    |
| strata_cursor_reparse | 91.96    | 93.72       | 97.23    | 86.06  | 820.3    |
- Speedup: 8.53x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 14.68    | 14.98       | 14.99    | 604.99 | 36.8     |
| strata_cursor_reparse | 82.93    | 83.03       | 88.26    | 109.17 | 45.3     |
- Speedup: 5.54x

## Summary
- loads json: orjson (5.01 ms)
- loads ndjson: msgspec (4.16 ms)
- dumps str: orjson (0.79 ms)
- dumps bytes: orjson (0.79 ms)
- search: strata (0.00 ms)
- cursor reuse: strata_cursor_reuse (10.98 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 224.6% behind orjson
- Strata gap in loads/ndjson: 89.9% behind msgspec
- Strata gap in dumps/str: 771.7% behind orjson
- Strata gap in dumps/bytes: 799.4% behind orjson
