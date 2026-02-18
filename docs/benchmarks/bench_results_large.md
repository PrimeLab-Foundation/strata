# Strata Benchmark Results — LARGE
Generated: 2026-02-18 23:28:11

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
- json: benchmarks/data/generated/large/users.json (43.85 MB, 4000 records)
- ndjson: benchmarks/data/generated/large/users.ndjson (43.85 MB, 4000 records, 4000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (43.85 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 156.17   | 210.66      | 217.38   | 218.29 | 1047.0   |
| msgspec       | 168.38   | 213.24      | 214.57   | 215.64 | 1031.3   |
| ujson         | 214.10   | 283.38      | 292.05   | 162.27 | 1151.8   |
| json (stdlib) | 269.25   | 324.28      | 325.34   | 141.80 | 1031.4   |
| strata        | 331.09   | 354.70      | 369.17   | 129.64 | 910.0    |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 182.25   | 187.72      | 196.27   | 244.94 | 1674.0   |
| msgspec       | 195.73   | 204.29      | 213.62   | 225.07 | 1657.4   |
| strata        | 174.57   | 216.27      | 223.78   | 212.60 | 1670.8   |
| ujson         | 247.85   | 270.98      | 310.22   | 169.68 | 1781.1   |
| json (stdlib) | 297.39   | 311.40      | 318.94   | 147.65 | 1660.4   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 45.51    | 46.64       | 48.36    | 874.35 | 40779839       | 772.7    |
| msgspec | 50.66    | 52.07       | 55.11    | 783.15 | 40779839       | 1052.8   |
| ujson   | 175.72   | 177.78      | 179.46   | 229.38 | 40779839       | 1329.2   |
| json    | 284.01   | 287.41      | 288.44   | 159.99 | 45983395       | 1685.5   |
| strata  | 409.34   | 410.95      | 420.15   | 115.97 | 47655981       | 762.6    |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 42.16    | 43.03       | 43.75    | 947.81 | 40779839       | 754.8    |
| msgspec | 48.88    | 49.68       | 50.38    | 820.77 | 40779839       | 1005.5   |
| ujson   | 178.05   | 179.30      | 192.50   | 227.44 | 40779839       | 1314.1   |
| json    | 288.21   | 291.92      | 299.28   | 157.52 | 45983395       | 1670.4   |
| strata  | 412.82   | 413.91      | 423.43   | 115.14 | 47655981       | 762.7    |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 2958836.79 | 1       | 5361.7   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2270787.23 | 1       | 5361.7   |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1625370.78 | 1       | 5362.2   |
| Extract all user IDs                        | strata      | 0.56     | 0.61        | 0.82     | 75995.18   | 4000    | 3817.0   |
| Extract all user IDs                        | jmespath    | 1.86     | 1.86        | 2.05     | 24696.26   | 4000    | 3817.3   |
| Extract all user IDs                        | jsonpath-ng | 31.48    | 31.62       | 31.70    | 1454.37    | 4000    | 3819.2   |
| Extract all user names                      | strata      | 0.73     | 1.04        | 1.04     | 44412.31   | 4000    | 5362.8   |
| Extract all user names                      | jmespath    | 1.33     | 1.40        | 1.54     | 32897.17   | 4000    | 5363.1   |
| Extract all user names                      | jsonpath-ng | 31.57    | 31.86       | 33.51    | 1443.36    | 4000    | 5365.2   |
| Extract nested timestamp field              | strata      | 0.93     | 0.97        | 1.12     | 47235.13   | 4000    | 4606.2   |
| Extract nested timestamp field              | jmespath    | 2.54     | 2.65        | 2.80     | 17382.84   | 4000    | 4594.8   |
| Extract nested timestamp field              | jsonpath-ng | 33.89    | 34.09       | 38.17    | 1349.00    | 4000    | 4163.8   |
| Extract order item prices (double wildcard) | strata      | 49.84    | 51.66       | 57.30    | 890.13     | 789913  | 5347.7   |
| Extract order item prices (double wildcard) | jmespath    | 213.98   | 215.53      | 217.88   | 213.35     | 4000    | 5358.7   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1055.73  | 1057.48     | 1065.09  | 43.48      | 789913  | 5075.2   |
| Filter users by age (numeric predicate)     | strata      | 1.14     | 1.14        | 1.16     | 40185.05   | 3212    | 5362.7   |
| Filter users by age (numeric predicate)     | jmespath    | 4.78     | 4.82        | 4.87     | 9537.32    | 3212    | 5362.9   |
| Recursively find all prices                 | strata      | 114.43   | 115.52      | 120.15   | 398.05     | 789913  | 5395.9   |
| Recursively find all prices                 | jsonpath-ng | 3783.09  | 3811.24     | 3823.29  | 12.07      | 789913  | 5012.8   |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.02     | 3162105.07 | 10      | 5362.5   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 1812150.09 | 10      | 5362.5   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1369207.15 | 10      | 5362.9   |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 3525483.36 | 1       | 1429.0   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2633566.39 | 1       | 1429.5   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2313427.45 | 1       | 1429.0   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2198288.51 | 0       | 1505.1   |
| Extract all user IDs                        | jmespath    | 1.99     | 2.00        | 2.06     | 22988.74   | 4000    | 1505.1   |
| Extract all user IDs                        | strata      | 29.14    | 29.26       | 29.43    | 1571.40    | 4000    | 1473.3   |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.01        | 0.02     | 3548335.49 | 0       | 1464.0   |
| Extract all user names                      | jmespath    | 1.90     | 1.92        | 1.95     | 23909.71   | 4000    | 1464.0   |
| Extract all user names                      | strata      | 29.11    | 29.43       | 31.45    | 1562.42    | 4000    | 1432.9   |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2118082.49 | 0       | 1433.5   |
| Extract nested timestamp field              | jmespath    | 3.18     | 3.20        | 3.32     | 14359.58   | 4000    | 1433.4   |
| Extract nested timestamp field              | strata      | 30.08    | 30.09       | 33.19    | 1527.87    | 4000    | 1432.9   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2074222.16 | 0       | 1572.2   |
| Extract order item prices (double wildcard) | strata      | 64.56    | 65.21       | 68.91    | 705.13     | 789913  | 1520.3   |
| Extract order item prices (double wildcard) | jmespath    | 178.65   | 179.24      | 182.00   | 256.53     | 789913  | 1572.2   |
| Filter users by age (numeric predicate)     | jmespath    | 5.00     | 5.07        | 5.09     | 9061.76    | 3212    | 1473.2   |
| Filter users by age (numeric predicate)     | strata      | 29.11    | 29.32       | 30.99    | 1568.23    | 3212    | 1473.2   |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 2659143.34 | 1       | 1469.9   |
| NDJSON first record id                      | strata      | 0.01     | 0.02        | 0.02     | 2658991.17 | 1       | 1469.9   |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.02        | 0.03     | 2313427.45 | 1       | 1470.3   |
| Recursively find all prices                 | strata      | 102.66   | 104.19      | 104.92   | 441.31     | 789913  | 1519.5   |
| Recursively find all prices                 | jsonpath-ng | 3647.20  | 3712.59     | 3800.53  | 12.38      | 789913  | 1614.4   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 1851546.67 | 10      | 1470.0   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 1556458.49 | 10      | 1470.0   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 1258260.85 | 10      | 1470.4   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 899.84   | 992.55      | 1218.96  | 370.63 | 5877.0   |
| strata_cursor_reparse | 9950.44  | 10673.38    | 10695.93 | 34.47  | 10874.4  |
- Speedup: 10.75x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 502.67   | 515.05      | 539.89   | 803.44 | 1050.5   |
| strata_cursor_reparse | 3575.61  | 3625.35     | 3688.77  | 114.14 | 2150.2   |
- Speedup: 7.04x

## Summary
- loads json: orjson (210.66 ms)
- loads ndjson: orjson (187.72 ms)
- dumps str: orjson (46.64 ms)
- dumps bytes: orjson (43.03 ms)
- search: jsonpath-ng (0.01 ms)
- cursor reuse: strata_cursor_reuse (515.05 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 68.4% behind orjson
- Strata gap in loads/ndjson: 15.2% behind orjson
- Strata gap in dumps/str: 781.1% behind orjson
- Strata gap in dumps/bytes: 862.0% behind orjson
