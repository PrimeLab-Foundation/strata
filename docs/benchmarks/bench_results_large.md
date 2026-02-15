# Strata Benchmark Results — LARGE
Generated: 2026-02-15 02:37:17

## Environment
- Commit: 1ee1b0e0e39eb07c8f4c2eab05f16140b4c205bc
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
- json: benchmarks/data/generated/large/users.json (43.85 MB, 4000 records)
- ndjson: benchmarks/data/generated/large/users.ndjson (43.85 MB, 4000 records, 4000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (43.85 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 178.42   | 234.47      | 264.52   | 196.12 | 1402.4   |
| msgspec       | 199.28   | 244.44      | 272.73   | 188.12 | 1383.8   |
| ujson         | 242.18   | 314.41      | 335.02   | 146.25 | 1505.4   |
| strata        | 290.16   | 315.86      | 318.21   | 145.58 | 1263.4   |
| json (stdlib) | 294.84   | 348.61      | 351.72   | 131.91 | 1383.9   |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| strata        | 176.25   | 198.75      | 200.89   | 231.34 | 2135.8   |
| orjson        | 156.71   | 212.52      | 212.60   | 216.36 | 2139.0   |
| msgspec       | 161.01   | 218.33      | 228.94   | 210.60 | 2116.3   |
| ujson         | 212.18   | 287.53      | 293.10   | 159.91 | 2238.0   |
| json (stdlib) | 268.59   | 329.13      | 332.84   | 139.70 | 2119.2   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 40.86    | 41.33       | 41.63    | 986.76 | 40779839       | 2644.1   |
| msgspec | 51.59    | 52.11       | 52.42    | 782.56 | 40779839       | 2959.9   |
| ujson   | 173.33   | 173.64      | 174.73   | 234.85 | 40779839       | 3271.4   |
| json    | 279.98   | 281.83      | 283.57   | 163.16 | 45983395       | 3398.3   |
| strata  | 408.31   | 413.33      | 421.79   | 115.30 | 47655981       | 2329.7   |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 38.97    | 39.16       | 39.73    | 1041.29 | 40779839       | 3694.1   |
| msgspec | 48.72    | 49.10       | 49.39    | 830.54  | 40779839       | 3685.2   |
| ujson   | 175.98   | 177.83      | 185.27   | 229.32  | 40779839       | 4018.5   |
| json    | 275.30   | 279.16      | 284.18   | 164.72  | 45983395       | 4162.3   |
| strata  | 410.35   | 411.94      | 414.93   | 115.69  | 47655981       | 3398.3   |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 3344249.76 | 1       | 9854.8   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2180960.80 | 1       | 9699.9   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1833250.09 | 1       | 9637.9   |
| Extract all user IDs                        | strata      | 0.58     | 0.59        | 0.61     | 78015.18   | 4000    | 5017.8   |
| Extract all user IDs                        | jmespath    | 1.79     | 1.80        | 1.87     | 25562.31   | 4000    | 5018.1   |
| Extract all user IDs                        | jsonpath-ng | 90.46    | 90.49       | 91.11    | 508.14     | 4000    | 5020.2   |
| Extract all user names                      | strata      | 0.65     | 0.65        | 0.66     | 70382.68   | 4000    | 10407.4  |
| Extract all user names                      | jmespath    | 1.42     | 1.47        | 35.03    | 31326.51   | 4000    | 10368.5  |
| Extract all user names                      | jsonpath-ng | 89.96    | 101.22      | 104.52   | 454.29     | 4000    | 10292.9  |
| Extract nested timestamp field              | strata      | 1.11     | 1.17        | 1.21     | 39223.80   | 4000    | 7017.7   |
| Extract nested timestamp field              | jmespath    | 2.56     | 2.65        | 2.70     | 17377.36   | 4000    | 7017.8   |
| Extract nested timestamp field              | jsonpath-ng | 92.73    | 93.18       | 93.26    | 493.48     | 4000    | 7019.2   |
| Extract order item prices (double wildcard) | strata      | 50.92    | 52.63       | 52.88    | 873.72     | 789913  | 8410.9   |
| Extract order item prices (double wildcard) | jmespath    | 271.05   | 272.57      | 273.14   | 168.70     | 4000    | 8428.6   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1099.40  | 1105.60     | 1113.25  | 41.59      | 789913  | 8441.2   |
| Filter users by age (numeric predicate)     | strata      | 1.14     | 1.15        | 1.28     | 39900.24   | 3212    | 12127.8  |
| Filter users by age (numeric predicate)     | jmespath    | 4.84     | 5.00        | 5.48     | 9190.48    | 3212    | 12051.5  |
| Recursively find all prices                 | strata      | 123.39   | 127.84      | 197.33   | 359.69     | 789913  | 12496.5  |
| Recursively find all prices                 | jsonpath-ng | 3883.97  | 3931.21     | 3933.47  | 11.70      | 789913  | 11302.4  |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 3459476.30 | 10      | 12118.5  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 1892943.44 | 10      | 12095.5  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1365830.26 | 10      | 12077.1  |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 2866187.36 | 1       | 11661.0  |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2308667.45 | 1       | 11621.1  |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2039810.83 | 1       | 11544.6  |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1925839.84 | 0       | 12081.0  |
| Extract all user IDs                        | jmespath    | 2.07     | 2.25        | 2.40     | 20396.00   | 4000    | 12532.2  |
| Extract all user IDs                        | strata      | 89.43    | 92.39       | 102.06   | 497.69     | 4000    | 12802.7  |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2284689.01 | 0       | 11360.2  |
| Extract all user names                      | jmespath    | 2.02     | 2.02        | 2.03     | 22763.95   | 4000    | 11391.6  |
| Extract all user names                      | strata      | 89.24    | 89.40       | 89.57    | 514.30     | 4000    | 11521.2  |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2185126.09 | 0       | 11822.4  |
| Extract nested timestamp field              | jmespath    | 3.45     | 3.45        | 3.46     | 13323.82   | 4000    | 11854.8  |
| Extract nested timestamp field              | strata      | 96.01    | 99.70       | 100.83   | 461.17     | 4000    | 11951.8  |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2342898.08 | 0       | 11677.0  |
| Extract order item prices (double wildcard) | strata      | 128.86   | 129.67      | 130.06   | 354.58     | 789913  | 11768.4  |
| Extract order item prices (double wildcard) | jmespath    | 179.31   | 180.15      | 181.06   | 255.22     | 789913  | 11725.5  |
| Filter users by age (numeric predicate)     | jmespath    | 5.14     | 5.20        | 5.31     | 8850.42    | 3212    | 11284.2  |
| Filter users by age (numeric predicate)     | strata      | 89.28    | 90.02       | 99.94    | 510.78     | 3212    | 11284.2  |
| NDJSON first record id                      | strata      | 0.02     | 0.02        | 0.02     | 2844026.41 | 1       | 13251.6  |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2211502.10 | 1       | 12938.6  |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 1873499.39 | 1       | 13133.9  |
| Recursively find all prices                 | strata      | 171.85   | 173.32      | 177.74   | 265.29     | 789913  | 11254.9  |
| Recursively find all prices                 | jsonpath-ng | 3934.65  | 3984.39     | 3989.73  | 11.54      | 789913  | 10225.3  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 1949682.72 | 10      | 10156.2  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 1774102.00 | 10      | 10213.9  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1350666.60 | 10      | 10126.3  |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 563.66   | 644.11      | 741.39   | 571.13 | 11996.9  |
| strata_cursor_reparse | 9746.72  | 10547.39    | 11108.47 | 34.88  | 12956.6  |
- Speedup: 16.38x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 617.73   | 626.86      | 634.94   | 660.14 | 9868.8   |
| strata_cursor_reparse | 3806.23  | 3852.22     | 3885.69  | 107.42 | 10072.7  |
- Speedup: 6.15x

## Summary
- loads json: orjson (234.47 ms)
- loads ndjson: strata (198.75 ms)
- dumps str: orjson (41.33 ms)
- dumps bytes: orjson (39.16 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (626.86 ms)
- Rule 13: strata not #1 in: loads/json, dumps/str, dumps/bytes
- Strata gap in loads/json: 34.7% behind orjson
- Strata gap in dumps/str: 900.1% behind orjson
- Strata gap in dumps/bytes: 951.9% behind orjson
