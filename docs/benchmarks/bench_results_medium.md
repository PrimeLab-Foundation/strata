# Strata Benchmark Results — MEDIUM
Generated: 2026-02-18 13:12:41

## Environment
- Commit: 572edda55e994922217874e725e6904ab7bad84a
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
| orjson        | 25.41    | 34.92       | 39.04    | 187.59 | 145.5    |
| msgspec       | 26.25    | 35.63       | 37.08    | 183.86 | 146.3    |
| strata        | 38.24    | 43.48       | 46.26    | 150.69 | 126.6    |
| ujson         | 36.42    | 50.66       | 52.66    | 129.32 | 163.1    |
| json (stdlib) | 43.88    | 52.27       | 52.94    | 125.35 | 152.6    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 22.98    | 32.48       | 33.46    | 201.68 | 215.3    |
| orjson        | 22.10    | 32.51       | 33.48    | 201.49 | 217.5    |
| ujson         | 30.41    | 41.63       | 43.90    | 157.32 | 232.2    |
| strata        | 41.45    | 45.95       | 48.23    | 142.53 | 217.3    |
| json (stdlib) | 37.84    | 48.10       | 50.02    | 136.17 | 216.3    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.54     | 6.64        | 6.80     | 874.77 | 5811025        | 131.8    |
| msgspec | 7.60     | 8.32        | 8.51     | 698.72 | 5811025        | 176.0    |
| ujson   | 25.12    | 25.48       | 26.10    | 228.04 | 5811025        | 176.5    |
| json    | 39.69    | 40.10       | 42.20    | 163.39 | 6551664        | 226.8    |
| strata  | 55.28    | 56.36       | 59.08    | 118.73 | 6690995        | 127.1    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.11     | 6.29        | 6.75     | 923.48 | 5811025        | 131.0    |
| msgspec | 6.98     | 7.17        | 7.23     | 810.51 | 5811025        | 174.4    |
| ujson   | 24.73    | 25.44       | 25.54    | 228.38 | 5811025        | 174.7    |
| json    | 40.96    | 41.87       | 43.70    | 156.48 | 6551664        | 225.1    |
| strata  | 55.34    | 57.21       | 58.32    | 116.96 | 6690995        | 127.4    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 503974.83 | 1       | 526.3    |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 347108.06 | 1       | 526.3    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 268334.66 | 1       | 526.7    |
| Extract all user IDs                        | strata      | 0.25     | 0.26        | 0.27     | 24966.61  | 2000    | 526.5    |
| Extract all user IDs                        | jmespath    | 0.46     | 0.50        | 0.51     | 13158.17  | 2000    | 526.8    |
| Extract all user IDs                        | jsonpath-ng | 6.04     | 6.11        | 7.25     | 1072.99   | 2000    | 528.0    |
| Extract all user names                      | strata      | 0.32     | 0.32        | 0.33     | 20730.42  | 2000    | 526.7    |
| Extract all user names                      | jmespath    | 0.46     | 0.66        | 0.68     | 9862.02   | 2000    | 526.8    |
| Extract all user names                      | jsonpath-ng | 7.68     | 8.10        | 8.17     | 808.48    | 2000    | 527.9    |
| Extract nested timestamp field              | strata      | 0.42     | 0.43        | 0.49     | 15261.59  | 2000    | 526.4    |
| Extract nested timestamp field              | jmespath    | 1.24     | 1.35        | 1.39     | 4864.64   | 2000    | 526.5    |
| Extract nested timestamp field              | jsonpath-ng | 8.92     | 8.93        | 9.13     | 733.92    | 2000    | 528.0    |
| Extract order item prices (double wildcard) | strata      | 6.03     | 6.12        | 7.32     | 1071.14   | 100999  | 537.0    |
| Extract order item prices (double wildcard) | jmespath    | 33.85    | 33.85       | 34.51    | 193.53    | 2000    | 540.3    |
| Extract order item prices (double wildcard) | jsonpath-ng | 143.56   | 148.40      | 150.73   | 44.15     | 100999  | 545.2    |
| Filter users by age (numeric predicate)     | strata      | 0.44     | 0.45        | 0.49     | 14670.65  | 1606    | 526.8    |
| Filter users by age (numeric predicate)     | jmespath    | 2.36     | 2.46        | 3.20     | 2661.97   | 1606    | 526.9    |
| Recursively find all prices                 | strata      | 12.80    | 13.00       | 13.80    | 503.93    | 100999  | 542.2    |
| Recursively find all prices                 | jsonpath-ng | 507.94   | 524.49      | 527.42   | 12.49     | 100999  | 547.8    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.02     | 507249.93 | 10      | 526.3    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.03     | 373483.53 | 10      | 526.3    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 197535.53 | 10      | 526.8    |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 363869.19 | 1       | 155.4    |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 301133.60 | 1       | 155.4    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 250301.82 | 1       | 155.7    |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 258542.11 | 0       | 168.3    |
| Extract all user IDs                        | jmespath    | 0.71     | 0.89        | 0.91     | 7376.77   | 2000    | 168.3    |
| Extract all user IDs                        | strata      | 6.62     | 6.94        | 8.03     | 944.15    | 2000    | 156.7    |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.02        | 0.02     | 339518.44 | 0       | 169.5    |
| Extract all user names                      | jmespath    | 0.73     | 0.78        | 0.83     | 8363.48   | 2000    | 169.5    |
| Extract all user names                      | strata      | 5.38     | 6.26        | 6.37     | 1045.62   | 2000    | 157.3    |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.03     | 293272.44 | 0       | 153.8    |
| Extract nested timestamp field              | jmespath    | 1.53     | 1.59        | 1.91     | 4129.56   | 2000    | 153.7    |
| Extract nested timestamp field              | strata      | 7.24     | 7.24        | 7.31     | 904.79    | 2000    | 153.6    |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.01     | 0.02        | 0.02     | 328846.84 | 0       | 167.7    |
| Extract order item prices (double wildcard) | strata      | 11.56    | 11.62       | 11.94    | 563.48    | 100999  | 164.3    |
| Extract order item prices (double wildcard) | jmespath    | 27.21    | 27.25       | 27.63    | 240.39    | 100999  | 167.7    |
| Filter users by age (numeric predicate)     | jmespath    | 2.52     | 2.54        | 2.62     | 2580.64   | 1606    | 156.6    |
| Filter users by age (numeric predicate)     | strata      | 5.69     | 6.97        | 7.13     | 939.22    | 1606    | 156.4    |
| NDJSON first record id                      | strata      | 0.01     | 0.02        | 0.02     | 406180.60 | 1       | 148.6    |
| NDJSON first record id                      | jmespath    | 0.01     | 0.02        | 0.02     | 347775.85 | 1       | 148.6    |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 335157.77 | 1       | 149.0    |
| Recursively find all prices                 | strata      | 18.03    | 18.18       | 18.43    | 360.23    | 100999  | 163.9    |
| Recursively find all prices                 | jsonpath-ng | 519.35   | 519.40      | 526.96   | 12.61     | 100999  | 169.6    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.02        | 0.02     | 326129.51 | 10      | 155.5    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 293259.44 | 10      | 155.5    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.05     | 181516.37 | 10      | 155.8    |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 75.58    | 100.29      | 118.14   | 522.62 | 738.3    |
| strata_cursor_reparse | 476.36   | 524.29      | 563.49   | 99.97  | 2870.5   |
- Speedup: 5.23x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 75.00    | 76.93       | 78.18    | 766.29 | 95.2     |
| strata_cursor_reparse | 537.11   | 542.23      | 550.91   | 108.71 | 161.0    |
- Speedup: 7.05x

## Summary
- loads json: orjson (34.92 ms)
- loads ndjson: msgspec (32.48 ms)
- dumps str: orjson (6.64 ms)
- dumps bytes: orjson (6.29 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (76.93 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 24.5% behind orjson
- Strata gap in loads/ndjson: 41.5% behind msgspec
- Strata gap in dumps/str: 748.4% behind orjson
- Strata gap in dumps/bytes: 809.2% behind orjson
