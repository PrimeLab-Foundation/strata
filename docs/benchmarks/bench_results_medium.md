# Strata Benchmark Results — MEDIUM
Generated: 2026-02-19 01:04:26

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
- json: benchmarks/data/generated/medium/users.json (6.25 MB, 2000 records)
- ndjson: benchmarks/data/generated/medium/users.ndjson (6.25 MB, 2000 records, 2000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (6.25 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 24.82    | 33.78       | 46.64    | 193.97 | 145.1    |
| msgspec       | 26.03    | 35.65       | 36.01    | 183.80 | 145.9    |
| strata        | 40.65    | 44.79       | 46.82    | 146.27 | 125.0    |
| ujson         | 32.71    | 45.64       | 47.89    | 143.56 | 162.7    |
| json (stdlib) | 39.95    | 51.38       | 51.40    | 127.53 | 152.3    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 22.83    | 31.16       | 33.72    | 210.20 | 205.9    |
| msgspec       | 21.64    | 31.57       | 32.94    | 207.46 | 203.6    |
| ujson         | 31.96    | 42.21       | 48.03    | 155.15 | 220.4    |
| json (stdlib) | 36.49    | 46.14       | 48.35    | 141.96 | 204.5    |
| strata        | 44.85    | 47.27       | 50.52    | 138.56 | 205.7    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.14     | 6.31        | 6.84     | 920.73 | 5811025        | 131.7    |
| msgspec | 7.03     | 7.14        | 8.27     | 814.33 | 5811025        | 175.9    |
| ujson   | 24.29    | 25.08       | 27.30    | 231.74 | 5811025        | 176.4    |
| json    | 37.69    | 37.84       | 39.48    | 173.16 | 6551664        | 226.9    |
| strata  | 52.60    | 52.91       | 58.39    | 126.45 | 6690995        | 127.0    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 5.83     | 5.96        | 6.11     | 975.06 | 5811025        | 130.0    |
| msgspec | 6.78     | 6.80        | 6.89     | 854.31 | 5811025        | 173.4    |
| ujson   | 24.77    | 27.24       | 28.33    | 213.34 | 5811025        | 173.7    |
| json    | 42.07    | 42.26       | 42.41    | 155.04 | 6551664        | 224.0    |
| strata  | 54.13    | 54.48       | 57.09    | 122.82 | 6690995        | 127.0    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.03     | 508866.68 | 1       | 526.3    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 347881.56 | 1       | 526.8    |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 330341.89 | 1       | 526.3    |
| Extract all user IDs                        | strata      | 0.25     | 0.25        | 0.29     | 26184.76  | 2000    | 527.3    |
| Extract all user IDs                        | jmespath    | 0.63     | 0.74        | 0.90     | 8803.53   | 2000    | 527.3    |
| Extract all user IDs                        | jsonpath-ng | 6.30     | 6.97        | 7.19     | 940.02    | 2000    | 528.4    |
| Extract all user names                      | strata      | 0.27     | 0.28        | 0.32     | 23745.14  | 2000    | 526.3    |
| Extract all user names                      | jmespath    | 0.50     | 0.60        | 0.61     | 10860.61  | 2000    | 526.5    |
| Extract all user names                      | jsonpath-ng | 6.76     | 6.84        | 7.21     | 957.78    | 2000    | 527.7    |
| Extract nested timestamp field              | strata      | 0.44     | 0.53        | 0.53     | 12446.76  | 2000    | 526.3    |
| Extract nested timestamp field              | jmespath    | 1.30     | 1.34        | 1.41     | 4883.69   | 2000    | 526.6    |
| Extract nested timestamp field              | jsonpath-ng | 9.26     | 9.41        | 10.18    | 696.21    | 2000    | 528.2    |
| Extract order item prices (double wildcard) | strata      | 5.69     | 5.82        | 6.19     | 1126.13   | 100999  | 535.6    |
| Extract order item prices (double wildcard) | jmespath    | 33.45    | 33.59       | 33.64    | 195.04    | 2000    | 538.8    |
| Extract order item prices (double wildcard) | jsonpath-ng | 133.04   | 139.85      | 140.63   | 46.85     | 100999  | 544.6    |
| Filter users by age (numeric predicate)     | strata      | 0.45     | 0.47        | 0.47     | 13985.60  | 1606    | 526.4    |
| Filter users by age (numeric predicate)     | jmespath    | 2.23     | 2.45        | 2.60     | 2677.20   | 1606    | 526.5    |
| Recursively find all prices                 | strata      | 14.92    | 14.97       | 15.35    | 437.58    | 100999  | 539.8    |
| Recursively find all prices                 | jsonpath-ng | 502.98   | 521.01      | 525.60   | 12.57     | 100999  | 543.4    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.02     | 542178.17 | 10      | 526.5    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 276826.95 | 10      | 526.6    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 200559.02 | 10      | 527.0    |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 371611.76 | 1       | 148.6    |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 271487.78 | 1       | 148.6    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 232876.46 | 1       | 149.0    |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 638992.93 | 0       | 161.4    |
| Extract all user IDs                        | jmespath    | 0.64     | 0.66        | 0.68     | 9912.45   | 2000    | 161.3    |
| Extract all user IDs                        | strata      | 6.86     | 7.64        | 7.65     | 856.89    | 2000    | 149.6    |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 310042.52 | 0       | 169.1    |
| Extract all user names                      | jmespath    | 0.84     | 1.01        | 1.09     | 6497.14   | 2000    | 169.1    |
| Extract all user names                      | strata      | 7.04     | 7.35        | 7.67     | 891.52    | 2000    | 157.3    |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.03     | 283227.86 | 0       | 157.4    |
| Extract nested timestamp field              | jmespath    | 1.59     | 1.75        | 1.75     | 3752.76   | 2000    | 157.4    |
| Extract nested timestamp field              | strata      | 7.86     | 7.94        | 8.13     | 825.31    | 2000    | 157.3    |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.01     | 0.01        | 0.02     | 474921.42 | 0       | 168.9    |
| Extract order item prices (double wildcard) | strata      | 11.61    | 11.63       | 12.16    | 563.33    | 100999  | 164.6    |
| Extract order item prices (double wildcard) | jmespath    | 27.15    | 27.31       | 28.04    | 239.79    | 100999  | 168.9    |
| Filter users by age (numeric predicate)     | jmespath    | 2.41     | 2.55        | 2.58     | 2567.23   | 1606    | 157.1    |
| Filter users by age (numeric predicate)     | strata      | 6.48     | 6.68        | 6.77     | 979.99    | 1606    | 156.9    |
| NDJSON first record id                      | strata      | 0.01     | 0.01        | 0.02     | 446557.35 | 1       | 155.4    |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 342465.51 | 1       | 155.7    |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 326113.44 | 1       | 155.4    |
| Recursively find all prices                 | strata      | 17.22    | 17.46       | 17.66    | 375.08    | 100999  | 164.5    |
| Recursively find all prices                 | jsonpath-ng | 504.56   | 505.63      | 508.26   | 12.95     | 100999  | 172.5    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 393963.16 | 10      | 148.5    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.02        | 0.03     | 362180.32 | 10      | 148.5    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.04     | 284767.52 | 10      | 148.8    |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 82.92    | 99.29       | 122.30   | 527.87 | 1055.4   |
| strata_cursor_reparse | 458.79   | 515.01      | 556.55   | 101.77 | 3045.0   |
- Speedup: 5.19x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 76.25    | 77.98       | 79.91    | 755.93 | 95.4     |
| strata_cursor_reparse | 530.51   | 532.15      | 537.59   | 110.77 | 161.0    |
- Speedup: 6.82x

## Summary
- loads json: orjson (33.78 ms)
- loads ndjson: orjson (31.16 ms)
- dumps str: orjson (6.31 ms)
- dumps bytes: orjson (5.96 ms)
- search: jsonpath-ng (0.01 ms)
- cursor reuse: strata_cursor_reuse (77.98 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 32.6% behind orjson
- Strata gap in loads/ndjson: 51.7% behind orjson
- Strata gap in dumps/str: 738.4% behind orjson
- Strata gap in dumps/bytes: 814.1% behind orjson
