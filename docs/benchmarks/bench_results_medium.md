# Strata Benchmark Results — MEDIUM
Generated: 2026-02-18 23:24:51

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
- json: benchmarks/data/generated/medium/users.json (6.25 MB, 2000 records)
- ndjson: benchmarks/data/generated/medium/users.ndjson (6.25 MB, 2000 records, 2000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (6.25 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 23.71    | 31.77       | 32.61    | 206.23 | 145.1    |
| msgspec       | 23.92    | 33.10       | 33.43    | 197.96 | 145.9    |
| strata        | 36.99    | 41.16       | 41.19    | 159.19 | 126.2    |
| ujson         | 30.97    | 43.67       | 46.76    | 150.01 | 162.7    |
| json (stdlib) | 38.35    | 47.93       | 48.71    | 136.70 | 152.2    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 20.16    | 28.69       | 29.82    | 228.25 | 187.3    |
| msgspec       | 20.42    | 29.89       | 30.56    | 219.13 | 185.0    |
| ujson         | 27.76    | 39.86       | 40.29    | 164.32 | 201.9    |
| strata        | 41.02    | 44.74       | 45.54    | 146.40 | 187.1    |
| json (stdlib) | 35.92    | 45.58       | 46.74    | 143.70 | 186.1    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.12     | 6.14        | 6.42     | 947.08 | 5811025        | 130.7    |
| msgspec | 6.87     | 6.91        | 7.14     | 841.42 | 5811025        | 174.2    |
| ujson   | 24.17    | 24.83       | 25.01    | 234.07 | 5811025        | 174.4    |
| json    | 38.01    | 38.81       | 39.11    | 168.82 | 6551664        | 226.4    |
| strata  | 51.95    | 52.86       | 53.20    | 126.59 | 6690995        | 127.0    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 5.68     | 5.72        | 7.48     | 1016.56 | 5811025        | 130.0    |
| msgspec | 6.66     | 6.76        | 6.78     | 859.53  | 5811025        | 173.5    |
| ujson   | 24.26    | 24.55       | 24.82    | 236.73  | 5811025        | 173.8    |
| json    | 38.41    | 39.37       | 39.49    | 166.40  | 6551664        | 224.2    |
| strata  | 51.87    | 52.38       | 53.16    | 127.75  | 6690995        | 127.1    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.01     | 1156111.78 | 1       | 526.4    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 591146.88  | 1       | 526.4    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 431996.66  | 1       | 526.9    |
| Extract all user IDs                        | strata      | 0.23     | 0.24        | 0.24     | 27470.29   | 2000    | 526.5    |
| Extract all user IDs                        | jmespath    | 0.44     | 0.44        | 0.45     | 14779.57   | 2000    | 526.7    |
| Extract all user IDs                        | jsonpath-ng | 5.86     | 6.02        | 6.72     | 1088.90    | 2000    | 527.9    |
| Extract all user names                      | strata      | 0.25     | 0.28        | 0.28     | 23655.80   | 2000    | 526.3    |
| Extract all user names                      | jmespath    | 0.42     | 0.42        | 0.44     | 15502.31   | 2000    | 526.4    |
| Extract all user names                      | jsonpath-ng | 5.63     | 5.70        | 6.06     | 1149.15    | 2000    | 527.5    |
| Extract nested timestamp field              | strata      | 0.43     | 0.44        | 0.46     | 14982.36   | 2000    | 526.4    |
| Extract nested timestamp field              | jmespath    | 1.10     | 1.13        | 1.26     | 5780.67    | 2000    | 526.5    |
| Extract nested timestamp field              | jsonpath-ng | 6.89     | 6.95        | 6.97     | 943.22     | 2000    | 528.0    |
| Extract order item prices (double wildcard) | strata      | 5.44     | 5.56        | 6.01     | 1178.75    | 100999  | 535.1    |
| Extract order item prices (double wildcard) | jmespath    | 32.24    | 32.86       | 33.04    | 199.36     | 2000    | 538.3    |
| Extract order item prices (double wildcard) | jsonpath-ng | 130.77   | 131.77      | 132.71   | 49.72      | 100999  | 543.2    |
| Filter users by age (numeric predicate)     | strata      | 0.35     | 0.37        | 0.38     | 17675.34   | 1606    | 526.8    |
| Filter users by age (numeric predicate)     | jmespath    | 2.22     | 2.22        | 2.31     | 2953.92    | 1606    | 526.9    |
| Recursively find all prices                 | strata      | 12.87    | 13.02       | 13.51    | 503.25     | 100999  | 539.6    |
| Recursively find all prices                 | jsonpath-ng | 479.07   | 480.79      | 490.58   | 13.63      | 100999  | 546.2    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 995240.55  | 10      | 526.3    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 472191.38  | 10      | 526.3    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.04     | 273944.64  | 10      | 526.8    |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 573674.22 | 1       | 155.2    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 567511.37 | 1       | 155.2    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 520475.79 | 1       | 155.5    |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 636383.69 | 0       | 167.7    |
| Extract all user IDs                        | jmespath    | 0.62     | 0.65        | 0.69     | 10125.72  | 2000    | 167.7    |
| Extract all user IDs                        | strata      | 5.20     | 5.20        | 5.34     | 1259.33   | 2000    | 156.2    |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.02        | 0.02     | 430670.65 | 0       | 169.2    |
| Extract all user names                      | jmespath    | 0.64     | 0.65        | 0.69     | 10057.05  | 2000    | 169.2    |
| Extract all user names                      | strata      | 5.04     | 5.08        | 5.19     | 1289.80   | 2000    | 157.4    |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.01        | 0.01     | 652226.88 | 0       | 157.4    |
| Extract nested timestamp field              | jmespath    | 1.29     | 1.31        | 1.31     | 5002.28   | 2000    | 157.3    |
| Extract nested timestamp field              | strata      | 5.40     | 5.62        | 5.84     | 1164.78   | 2000    | 157.3    |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.01     | 0.02        | 0.02     | 352434.71 | 0       | 167.2    |
| Extract order item prices (double wildcard) | strata      | 9.93     | 9.98        | 10.38    | 656.15    | 100999  | 164.2    |
| Extract order item prices (double wildcard) | jmespath    | 24.76    | 25.02       | 25.13    | 261.77    | 100999  | 167.2    |
| Filter users by age (numeric predicate)     | jmespath    | 2.54     | 2.67        | 2.73     | 2456.77   | 1606    | 156.6    |
| Filter users by age (numeric predicate)     | strata      | 6.23     | 6.87        | 8.24     | 953.67    | 1606    | 156.4    |
| NDJSON first record id                      | strata      | 0.01     | 0.01        | 0.02     | 770546.82 | 1       | 155.3    |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.01        | 0.02     | 611659.92 | 1       | 155.6    |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.01     | 567462.71 | 1       | 155.3    |
| Recursively find all prices                 | strata      | 17.66    | 17.71       | 18.07    | 369.87    | 100999  | 157.2    |
| Recursively find all prices                 | jsonpath-ng | 499.21   | 501.85      | 502.23   | 13.05     | 100999  | 162.9    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.02     | 456931.74 | 10      | 155.5    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 454332.19 | 10      | 155.5    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.04     | 233574.02 | 10      | 155.8    |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 69.13    | 84.40       | 103.28   | 621.01 | 642.7    |
| strata_cursor_reparse | 437.20   | 444.53      | 537.35   | 117.91 | 2256.0   |
- Speedup: 5.27x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 75.26    | 75.29       | 80.98    | 782.92 | 95.4     |
| strata_cursor_reparse | 517.33   | 521.18      | 522.18   | 113.10 | 160.9    |
- Speedup: 6.92x

## Summary
- loads json: orjson (31.77 ms)
- loads ndjson: orjson (28.69 ms)
- dumps str: orjson (6.14 ms)
- dumps bytes: orjson (5.72 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (75.29 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 29.5% behind orjson
- Strata gap in loads/ndjson: 55.9% behind orjson
- Strata gap in dumps/str: 761.4% behind orjson
- Strata gap in dumps/bytes: 816.2% behind orjson
