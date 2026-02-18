# Strata Benchmark Results — MEDIUM
Generated: 2026-02-19 00:23:45

## Environment
- Commit: 66621b992fb1ccdd3a41329f7022bcf24ff7d7ae
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
| orjson        | 24.36    | 34.44       | 35.84    | 190.24 | 145.4    |
| msgspec       | 26.63    | 37.02       | 37.98    | 176.98 | 146.2    |
| strata        | 43.11    | 47.16       | 48.20    | 138.91 | 125.4    |
| ujson         | 36.49    | 49.53       | 51.57    | 132.28 | 163.0    |
| json (stdlib) | 43.90    | 53.03       | 55.02    | 123.54 | 152.6    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 22.70    | 31.06       | 32.97    | 210.89 | 222.8    |
| msgspec       | 22.31    | 33.46       | 34.62    | 195.72 | 220.6    |
| ujson         | 31.15    | 44.53       | 45.60    | 147.07 | 237.4    |
| strata        | 43.27    | 48.00       | 48.38    | 136.46 | 222.6    |
| json (stdlib) | 38.48    | 50.72       | 51.68    | 129.13 | 221.5    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.84     | 7.02        | 7.20     | 827.64 | 5811025        | 131.1    |
| msgspec | 7.71     | 7.75        | 8.03     | 749.68 | 5811025        | 174.5    |
| ujson   | 25.92    | 26.65       | 27.13    | 218.03 | 5811025        | 176.6    |
| json    | 41.03    | 41.52       | 42.65    | 157.78 | 6551664        | 226.7    |
| strata  | 55.77    | 57.51       | 58.31    | 116.35 | 6690995        | 127.4    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.13     | 6.41        | 6.80     | 906.28 | 5811025        | 131.0    |
| msgspec | 7.07     | 7.34        | 7.60     | 792.20 | 5811025        | 175.2    |
| ujson   | 25.58    | 25.75       | 26.04    | 225.71 | 5811025        | 175.5    |
| json    | 41.39    | 41.57       | 42.23    | 157.61 | 6551664        | 225.9    |
| strata  | 55.57    | 56.33       | 56.69    | 118.78 | 6690995        | 127.0    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 496036.92 | 1       | 526.5    |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 333129.31 | 1       | 526.5    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 279293.63 | 1       | 527.0    |
| Extract all user IDs                        | strata      | 0.21     | 0.25        | 0.31     | 26705.19  | 2000    | 526.6    |
| Extract all user IDs                        | jmespath    | 0.52     | 0.54        | 0.60     | 12045.34  | 2000    | 526.8    |
| Extract all user IDs                        | jsonpath-ng | 6.53     | 7.02        | 7.03     | 932.74    | 2000    | 528.0    |
| Extract all user names                      | strata      | 0.36     | 0.43        | 0.56     | 15356.96  | 2000    | 526.8    |
| Extract all user names                      | jmespath    | 0.59     | 0.60        | 0.66     | 10988.12  | 2000    | 527.0    |
| Extract all user names                      | jsonpath-ng | 8.44     | 8.55        | 8.87     | 766.70    | 2000    | 528.1    |
| Extract nested timestamp field              | strata      | 0.52     | 0.52        | 0.55     | 12533.07  | 2000    | 526.7    |
| Extract nested timestamp field              | jmespath    | 1.19     | 1.23        | 1.32     | 5311.80   | 2000    | 526.9    |
| Extract nested timestamp field              | jsonpath-ng | 8.70     | 9.43        | 10.07    | 694.73    | 2000    | 528.5    |
| Extract order item prices (double wildcard) | strata      | 6.78     | 10.68       | 12.36    | 613.72    | 100999  | 537.3    |
| Extract order item prices (double wildcard) | jmespath    | 37.74    | 43.26       | 80.28    | 151.43    | 2000    | 540.4    |
| Extract order item prices (double wildcard) | jsonpath-ng | 147.96   | 148.19      | 148.62   | 44.21     | 100999  | 547.3    |
| Filter users by age (numeric predicate)     | strata      | 0.47     | 0.47        | 0.54     | 13853.73  | 1606    | 526.2    |
| Filter users by age (numeric predicate)     | jmespath    | 2.62     | 2.71        | 3.01     | 2421.61   | 1606    | 526.3    |
| Recursively find all prices                 | strata      | 15.51    | 17.28       | 17.30    | 379.25    | 100999  | 542.9    |
| Recursively find all prices                 | jsonpath-ng | 529.32   | 532.15      | 548.46   | 12.31     | 100999  | 547.7    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 657864.86 | 10      | 526.5    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 310137.72 | 10      | 526.5    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.04        | 0.04     | 176276.43 | 10      | 527.0    |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 390046.53 | 1       | 155.7    |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 315646.18 | 1       | 155.7    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 252725.94 | 1       | 156.0    |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 631292.36 | 0       | 161.3    |
| Extract all user IDs                        | jmespath    | 0.84     | 0.85        | 0.94     | 7694.92   | 2000    | 161.3    |
| Extract all user IDs                        | strata      | 6.96     | 7.08        | 8.17     | 925.71    | 2000    | 149.6    |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.03        | 0.03     | 256426.33 | 0       | 169.3    |
| Extract all user names                      | jmespath    | 0.77     | 0.77        | 0.79     | 8496.85   | 2000    | 169.3    |
| Extract all user names                      | strata      | 6.50     | 6.93        | 8.08     | 944.75    | 2000    | 157.3    |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.02        | 0.02     | 311265.54 | 0       | 157.6    |
| Extract nested timestamp field              | jmespath    | 1.64     | 1.65        | 1.81     | 3975.51   | 2000    | 157.6    |
| Extract nested timestamp field              | strata      | 7.30     | 7.63        | 7.64     | 858.40    | 2000    | 157.6    |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.03     | 292186.48 | 0       | 168.0    |
| Extract order item prices (double wildcard) | strata      | 11.61    | 11.63       | 12.22    | 563.30    | 100999  | 164.6    |
| Extract order item prices (double wildcard) | jmespath    | 28.40    | 29.12       | 30.10    | 224.91    | 100999  | 168.0    |
| Filter users by age (numeric predicate)     | jmespath    | 2.78     | 2.99        | 3.02     | 2191.83   | 1606    | 156.2    |
| Filter users by age (numeric predicate)     | strata      | 6.35     | 6.73        | 6.75     | 973.11    | 1606    | 156.2    |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.03     | 298838.30 | 1       | 155.8    |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 298281.39 | 1       | 156.0    |
| NDJSON first record id                      | strata      | 0.02     | 0.02        | 0.03     | 292721.60 | 1       | 155.8    |
| Recursively find all prices                 | strata      | 18.34    | 18.63       | 20.34    | 351.49    | 100999  | 164.4    |
| Recursively find all prices                 | jsonpath-ng | 525.72   | 530.64      | 533.75   | 12.34     | 100999  | 171.7    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 299988.75 | 10      | 155.4    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.03        | 0.03     | 231845.79 | 10      | 155.4    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 165813.96 | 10      | 155.7    |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 89.24    | 99.05       | 128.96   | 529.14 | 952.2    |
| strata_cursor_reparse | 500.05   | 539.78      | 573.33   | 97.10  | 3409.2   |
- Speedup: 5.45x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 81.15    | 83.33       | 87.29    | 707.39 | 95.5     |
| strata_cursor_reparse | 559.34   | 562.54      | 567.81   | 104.79 | 160.6    |
- Speedup: 6.75x

## Summary
- loads json: orjson (34.44 ms)
- loads ndjson: orjson (31.06 ms)
- dumps str: orjson (7.02 ms)
- dumps bytes: orjson (6.41 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (83.33 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 37.0% behind orjson
- Strata gap in loads/ndjson: 54.5% behind orjson
- Strata gap in dumps/str: 719.0% behind orjson
- Strata gap in dumps/bytes: 778.6% behind orjson
