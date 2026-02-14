# Strata Benchmark Results — MEDIUM-FLAGS-NONPGO
Generated: 2026-02-14 21:15:21

## Environment
- Commit: ce4d80c0cfcd8bbfe70be79dc3e5b1150ad13bb4
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
| strata        | 39.89    | 44.34       | 44.84    | 147.77 | 184.7    |
| orjson        | 34.13    | 44.43       | 46.16    | 147.46 | 204.6    |
| msgspec       | 35.62    | 45.47       | 46.89    | 144.08 | 202.4    |
| ujson         | 43.70    | 57.43       | 59.07    | 114.08 | 219.2    |
| json (stdlib) | 52.23    | 63.27       | 64.57    | 103.55 | 202.5    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 28.54    | 39.32       | 40.91    | 166.59 | 276.2    |
| msgspec       | 28.15    | 41.02       | 42.02    | 159.67 | 276.8    |
| strata        | 46.61    | 50.73       | 53.12    | 129.11 | 276.1    |
| ujson         | 35.87    | 51.39       | 53.33    | 127.45 | 292.7    |
| json (stdlib) | 45.99    | 58.90       | 60.72    | 111.20 | 281.8    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| msgspec | 7.86     | 8.15        | 8.26     | 712.98 | 5811025        | 415.8    |
| orjson  | 8.21     | 8.22        | 8.50     | 706.74 | 5811025        | 370.3    |
| ujson   | 27.05    | 27.47       | 28.17    | 211.58 | 5811025        | 416.1    |
| strata  | 30.95    | 31.13       | 31.52    | 214.91 | 6690995        | 368.8    |
| json    | 43.21    | 43.48       | 43.88    | 150.69 | 6551664        | 466.4    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.74     | 7.09        | 7.19     | 819.34 | 5811025        | 514.9    |
| msgspec | 7.70     | 7.82        | 8.10     | 743.21 | 5811025        | 559.9    |
| ujson   | 27.21    | 27.60       | 28.93    | 210.53 | 5811025        | 562.1    |
| strata  | 30.74    | 30.87       | 31.32    | 216.76 | 6690995        | 517.6    |
| json    | 43.57    | 43.59       | 44.05    | 150.31 | 6551664        | 612.3    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 313221.82 | 1       | 1907.9   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 283830.64 | 1       | 1907.9   |
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.03     | 233288.11 | 1       | 1907.9   |
| Extract all user IDs                        | jmespath    | 0.90     | 0.90        | 0.92     | 7263.82   | 2000    | 1015.8   |
| Extract all user IDs                        | strata      | 2.57     | 2.65        | 3.12     | 2468.56   | 2000    | 1015.8   |
| Extract all user IDs                        | jsonpath-ng | 21.86    | 22.27       | 22.67    | 294.22    | 2000    | 1015.8   |
| Extract all user names                      | jmespath    | 0.63     | 0.64        | 0.64     | 10239.65  | 2000    | 2096.3   |
| Extract all user names                      | strata      | 2.72     | 2.74        | 2.75     | 2394.32   | 2000    | 2096.3   |
| Extract all user names                      | jsonpath-ng | 22.26    | 22.67       | 22.73    | 288.95    | 2000    | 2096.3   |
| Extract nested timestamp field              | jmespath    | 1.46     | 1.51        | 1.58     | 4332.16   | 2000    | 1463.9   |
| Extract nested timestamp field              | strata      | 3.10     | 3.16        | 3.55     | 2071.02   | 2000    | 1463.7   |
| Extract nested timestamp field              | jsonpath-ng | 22.96    | 23.52       | 24.33    | 278.60    | 2000    | 1463.9   |
| Extract order item prices (double wildcard) | jmespath    | 51.26    | 51.86       | 52.10    | 126.35    | 2000    | 1765.0   |
| Extract order item prices (double wildcard) | strata      | 120.87   | 124.62      | 133.33   | 52.57     | 100999  | 1765.0   |
| Extract order item prices (double wildcard) | jsonpath-ng | 163.53   | 165.48      | 165.55   | 39.59     | 100999  | 1768.4   |
| Filter users by age (numeric predicate)     | jmespath    | 2.63     | 2.72        | 2.77     | 2411.84   | 1606    | 2217.1   |
| Filter users by age (numeric predicate)     | strata      | 2.72     | 2.76        | 2.81     | 2377.34   | 1606    | 2217.1   |
| Recursively find all prices                 | strata      | 134.90   | 136.54      | 138.42   | 47.99     | 100999  | 2428.7   |
| Recursively find all prices                 | jsonpath-ng | 608.83   | 609.59      | 612.46   | 10.75     | 100999  | 2432.2   |
| Slice first 10 users, extract IDs           | jmespath    | 0.03     | 0.03        | 0.03     | 247232.62 | 10      | 2520.5   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 208260.40 | 10      | 2520.5   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.06     | 187640.68 | 10      | 2520.2   |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 327482.54 | 1       | 4854.9   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 323439.52 | 1       | 4854.9   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 278223.09 | 1       | 4854.9   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 294923.16 | 0       | 4853.1   |
| Extract all user IDs                        | jmespath    | 0.99     | 1.00        | 1.10     | 6531.14   | 2000    | 4853.1   |
| Extract all user IDs                        | strata      | 22.72    | 23.27       | 23.50    | 281.52    | 2000    | 4852.7   |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 326812.93 | 0       | 4854.9   |
| Extract all user names                      | jmespath    | 1.06     | 1.11        | 1.14     | 5876.55   | 2000    | 4854.9   |
| Extract all user names                      | strata      | 22.91    | 23.38       | 23.52    | 280.15    | 2000    | 4854.9   |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.03        | 0.03     | 201788.63 | 0       | 4853.2   |
| Extract nested timestamp field              | jmespath    | 1.67     | 1.77        | 1.83     | 3695.24   | 2000    | 4853.2   |
| Extract nested timestamp field              | strata      | 22.97    | 23.62       | 23.65    | 277.32    | 2000    | 4853.2   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.02     | 298267.56 | 0       | 4854.9   |
| Extract order item prices (double wildcard) | jmespath    | 29.44    | 29.53       | 29.61    | 221.79    | 100999  | 4854.9   |
| Extract order item prices (double wildcard) | strata      | 140.81   | 141.86      | 143.79   | 46.17     | 100999  | 4854.2   |
| Filter users by age (numeric predicate)     | jmespath    | 2.90     | 2.95        | 2.98     | 2222.10   | 1606    | 4854.9   |
| Filter users by age (numeric predicate)     | strata      | 21.98    | 22.20       | 23.21    | 295.06    | 1606    | 4854.9   |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 336604.53 | 1       | 4836.9   |
| NDJSON first record id                      | strata      | 0.02     | 0.02        | 0.03     | 277715.98 | 1       | 4836.9   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 277715.98 | 1       | 4836.9   |
| Recursively find all prices                 | strata      | 148.35   | 149.57      | 152.05   | 43.79     | 100999  | 4854.9   |
| Recursively find all prices                 | jsonpath-ng | 614.49   | 616.94      | 617.37   | 10.62     | 100999  | 4857.2   |
| Slice first 10 users, extract IDs           | jmespath    | 0.03     | 0.03        | 0.03     | 257698.06 | 10      | 4856.2   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.04     | 187577.77 | 10      | 4856.2   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 180059.26 | 10      | 4856.0   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 357.57   | 358.30      | 358.67   | 146.28 | 2782.3   |
| strata_cursor_reparse | 770.91   | 775.38      | 800.27   | 67.60  | 4839.5   |
- Speedup: 2.16x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 340.98   | 342.77      | 343.56   | 171.97 | 4856.1   |
| strata_cursor_reparse | 859.79   | 859.89      | 868.51   | 68.55  | 4876.7   |
- Speedup: 2.51x

## Summary
- loads json: strata (44.34 ms)
- loads ndjson: orjson (39.32 ms)
- dumps str: msgspec (8.15 ms)
- dumps bytes: orjson (7.09 ms)
- search: jmespath (0.02 ms)
- cursor reuse: strata_cursor_reuse (342.77 ms)
- Rule 13: strata not #1 in: loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/ndjson: 29.0% behind orjson
- Strata gap in dumps/str: 282.0% behind msgspec
- Strata gap in dumps/bytes: 335.2% behind orjson
