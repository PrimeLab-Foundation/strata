# Strata Benchmark Results — MEDIUM
Generated: 2026-02-18 00:44:02

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
- json: benchmarks/data/generated/medium/users.json (6.25 MB, 2000 records)
- ndjson: benchmarks/data/generated/medium/users.ndjson (6.25 MB, 2000 records, 2000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (6.25 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 23.37    | 32.70       | 33.52    | 200.38 | 145.1    |
| msgspec       | 27.16    | 34.94       | 36.71    | 187.53 | 145.9    |
| strata        | 40.01    | 42.20       | 42.62    | 155.24 | 126.2    |
| ujson         | 32.56    | 45.42       | 48.22    | 144.26 | 162.7    |
| json (stdlib) | 41.33    | 49.52       | 51.00    | 132.31 | 152.2    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 22.27    | 29.96       | 30.33    | 218.61 | 187.4    |
| msgspec       | 20.78    | 31.08       | 33.11    | 210.76 | 185.2    |
| ujson         | 29.75    | 41.66       | 43.30    | 157.20 | 202.1    |
| strata        | 39.82    | 47.03       | 49.37    | 139.27 | 187.2    |
| json (stdlib) | 36.94    | 47.29       | 49.29    | 138.49 | 186.2    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.50     | 6.51        | 6.92     | 892.57 | 5811025        | 130.8    |
| msgspec | 7.38     | 7.48        | 8.27     | 776.62 | 5811025        | 176.0    |
| ujson   | 25.05    | 25.44       | 25.71    | 228.41 | 5811025        | 176.5    |
| json    | 40.44    | 40.68       | 41.15    | 161.06 | 6551664        | 226.8    |
| strata  | 54.63    | 56.51       | 58.17    | 118.40 | 6690995        | 127.1    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.40     | 6.54        | 6.83     | 887.96 | 5811025        | 130.2    |
| msgspec | 7.14     | 7.37        | 7.50     | 788.72 | 5811025        | 173.7    |
| ujson   | 25.69    | 25.84       | 26.83    | 224.84 | 5811025        | 175.8    |
| json    | 41.28    | 41.53       | 41.83    | 157.76 | 6551664        | 226.1    |
| strata  | 55.21    | 55.39       | 56.30    | 120.80 | 6690995        | 127.3    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 510530.82 | 1       | 527.1    |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 453150.65 | 1       | 527.1    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 423837.13 | 1       | 527.6    |
| Extract all user IDs                        | strata      | 0.27     | 0.27        | 0.28     | 24083.28  | 2000    | 526.5    |
| Extract all user IDs                        | jmespath    | 0.52     | 0.56        | 0.57     | 11661.21  | 2000    | 526.6    |
| Extract all user IDs                        | jsonpath-ng | 6.10     | 6.21        | 6.98     | 1055.05   | 2000    | 527.8    |
| Extract all user names                      | strata      | 0.29     | 0.29        | 0.32     | 22414.86  | 2000    | 526.7    |
| Extract all user names                      | jmespath    | 0.47     | 0.48        | 0.49     | 13741.14  | 2000    | 526.8    |
| Extract all user names                      | jsonpath-ng | 6.40     | 6.49        | 7.59     | 1008.81   | 2000    | 527.8    |
| Extract nested timestamp field              | strata      | 0.41     | 0.43        | 0.55     | 15263.05  | 2000    | 526.7    |
| Extract nested timestamp field              | jmespath    | 1.23     | 1.24        | 1.25     | 5279.17   | 2000    | 526.7    |
| Extract nested timestamp field              | jsonpath-ng | 6.88     | 7.60        | 7.63     | 861.69    | 2000    | 528.2    |
| Extract order item prices (double wildcard) | strata      | 5.32     | 5.70        | 5.83     | 1149.48   | 100999  | 535.4    |
| Extract order item prices (double wildcard) | jmespath    | 34.62    | 34.68       | 35.38    | 188.94    | 2000    | 538.6    |
| Extract order item prices (double wildcard) | jsonpath-ng | 140.23   | 140.52      | 140.80   | 46.63     | 100999  | 545.2    |
| Filter users by age (numeric predicate)     | strata      | 0.40     | 0.42        | 0.44     | 15582.21  | 1606    | 526.6    |
| Filter users by age (numeric predicate)     | jmespath    | 2.23     | 2.43        | 2.43     | 2698.93   | 1606    | 526.7    |
| Recursively find all prices                 | strata      | 12.91    | 12.92       | 14.02    | 506.96    | 100999  | 539.4    |
| Recursively find all prices                 | jsonpath-ng | 505.82   | 508.52      | 512.88   | 12.88     | 100999  | 543.2    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.02     | 489843.51 | 10      | 526.5    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 356553.25 | 10      | 526.5    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 247232.35 | 10      | 527.0    |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 447840.57 | 1       | 155.2    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 316270.77 | 1       | 155.5    |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 290553.03 | 1       | 155.2    |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 473480.56 | 0       | 167.8    |
| Extract all user IDs                        | jmespath    | 0.80     | 0.92        | 1.02     | 7148.00   | 2000    | 167.8    |
| Extract all user IDs                        | strata      | 6.20     | 6.44        | 6.87     | 1016.47   | 2000    | 156.5    |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 273849.22 | 0       | 169.4    |
| Extract all user names                      | jmespath    | 0.84     | 0.88        | 0.97     | 7470.73   | 2000    | 169.4    |
| Extract all user names                      | strata      | 6.75     | 7.07        | 7.40     | 925.88    | 2000    | 157.3    |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.01        | 0.02     | 527475.75 | 0       | 157.3    |
| Extract nested timestamp field              | jmespath    | 1.42     | 1.47        | 1.47     | 4457.70   | 2000    | 157.3    |
| Extract nested timestamp field              | strata      | 5.92     | 6.10        | 7.19     | 1074.18   | 2000    | 157.2    |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.03     | 298852.58 | 0       | 168.9    |
| Extract order item prices (double wildcard) | strata      | 10.05    | 10.50       | 10.88    | 623.88    | 100999  | 164.3    |
| Extract order item prices (double wildcard) | jmespath    | 27.08    | 27.82       | 28.24    | 235.43    | 100999  | 168.8    |
| Filter users by age (numeric predicate)     | jmespath    | 2.58     | 2.63        | 2.65     | 2491.59   | 1606    | 156.7    |
| Filter users by age (numeric predicate)     | strata      | 6.58     | 6.89        | 7.16     | 951.05    | 1606    | 156.7    |
| NDJSON first record id                      | strata      | 0.01     | 0.01        | 0.02     | 674597.46 | 1       | 155.5    |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.02     | 538311.53 | 1       | 155.5    |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.02        | 0.02     | 369849.20 | 1       | 155.8    |
| Recursively find all prices                 | strata      | 17.91    | 17.94       | 19.17    | 365.11    | 100999  | 164.3    |
| Recursively find all prices                 | jsonpath-ng | 507.96   | 511.74      | 519.70   | 12.80     | 100999  | 173.8    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 306416.27 | 10      | 151.7    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 269168.00 | 10      | 151.7    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.05     | 184934.94 | 10      | 152.0    |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 70.63    | 84.96       | 107.32   | 616.93 | 626.2    |
| strata_cursor_reparse | 467.30   | 487.10      | 570.89   | 107.60 | 2767.2   |
- Speedup: 5.73x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 75.83    | 79.10       | 80.51    | 745.19 | 94.9     |
| strata_cursor_reparse | 539.22   | 540.13      | 549.68   | 109.13 | 160.3    |
- Speedup: 6.83x

## Summary
- loads json: orjson (32.70 ms)
- loads ndjson: orjson (29.96 ms)
- dumps str: orjson (6.51 ms)
- dumps bytes: orjson (6.54 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (79.10 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 29.1% behind orjson
- Strata gap in loads/ndjson: 57.0% behind orjson
- Strata gap in dumps/str: 768.0% behind orjson
- Strata gap in dumps/bytes: 746.4% behind orjson
