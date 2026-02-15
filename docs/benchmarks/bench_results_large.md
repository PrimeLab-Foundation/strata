# Strata Benchmark Results — LARGE
Generated: 2026-02-16 01:28:36

## Environment
- Commit: 55018296d1e43d763c65e319ab2ce3b86caeecc8
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
| orjson        | 151.01   | 204.30      | 205.57   | 225.07 | 1047.0   |
| msgspec       | 160.33   | 214.87      | 217.67   | 214.01 | 1031.3   |
| ujson         | 210.13   | 286.36      | 290.05   | 160.58 | 1151.9   |
| json (stdlib) | 276.63   | 325.15      | 327.66   | 141.42 | 1031.5   |
| strata        | 301.30   | 327.73      | 330.07   | 140.31 | 910.0    |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 182.90   | 187.54      | 191.19   | 245.17 | 1632.6   |
| msgspec       | 189.50   | 194.40      | 200.68   | 236.52 | 1615.9   |
| strata        | 178.35   | 200.88      | 203.03   | 228.89 | 1628.0   |
| ujson         | 254.01   | 265.25      | 266.17   | 173.34 | 1739.5   |
| json (stdlib) | 298.50   | 305.04      | 308.80   | 150.73 | 1618.7   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 47.13    | 48.80       | 49.76    | 835.63 | 40779839       | 772.8    |
| msgspec | 53.47    | 54.89       | 56.20    | 743.00 | 40779839       | 1052.1   |
| ujson   | 182.83   | 184.65      | 185.65   | 220.85 | 40779839       | 1329.5   |
| json    | 293.68   | 293.88      | 295.14   | 156.47 | 45983395       | 1685.7   |
| strata  | 417.80   | 418.96      | 421.57   | 113.75 | 47655981       | 762.7    |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 41.90    | 42.08       | 42.75    | 969.20 | 40779839       | 754.7    |
| msgspec | 50.04    | 50.69       | 51.67    | 804.43 | 40779839       | 1004.6   |
| ujson   | 181.84   | 186.00      | 186.82   | 219.25 | 40779839       | 1313.9   |
| json    | 288.78   | 290.05      | 294.14   | 158.54 | 45983395       | 1669.9   |
| strata  | 417.90   | 418.80      | 420.11   | 113.79 | 47655981       | 762.8    |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 3284523.69 | 1       | 5362.1   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2280130.14 | 1       | 5362.1   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1963675.73 | 1       | 5362.6   |
| Extract all user IDs                        | strata      | 0.57     | 0.59        | 0.61     | 77641.87   | 4000    | 5362.4   |
| Extract all user IDs                        | jmespath    | 1.72     | 1.81        | 1.89     | 25432.70   | 4000    | 5362.7   |
| Extract all user IDs                        | jsonpath-ng | 32.30    | 33.26       | 37.66    | 1382.71    | 4000    | 5364.7   |
| Extract all user names                      | strata      | 0.66     | 0.68        | 0.95     | 67374.94   | 4000    | 5362.7   |
| Extract all user names                      | jmespath    | 1.28     | 1.41        | 1.43     | 32601.72   | 4000    | 5363.0   |
| Extract all user names                      | jsonpath-ng | 32.70    | 33.56       | 34.78    | 1370.28    | 4000    | 5365.0   |
| Extract nested timestamp field              | strata      | 1.08     | 1.21        | 1.21     | 38159.16   | 4000    | 5362.7   |
| Extract nested timestamp field              | jmespath    | 2.98     | 3.08        | 3.10     | 14952.53   | 4000    | 5362.8   |
| Extract nested timestamp field              | jsonpath-ng | 35.43    | 36.75       | 37.20    | 1251.22    | 4000    | 5365.6   |
| Extract order item prices (double wildcard) | strata      | 51.03    | 59.39       | 81.79    | 774.25     | 789913  | 5419.4   |
| Extract order item prices (double wildcard) | jmespath    | 223.62   | 224.67      | 227.06   | 204.67     | 4000    | 5435.8   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1102.78  | 1105.27     | 1107.16  | 41.60      | 789913  | 5446.0   |
| Filter users by age (numeric predicate)     | strata      | 1.15     | 1.24        | 1.26     | 37102.08   | 3212    | 5363.4   |
| Filter users by age (numeric predicate)     | jmespath    | 4.66     | 4.85        | 5.01     | 9481.52    | 3212    | 5363.8   |
| Recursively find all prices                 | strata      | 117.19   | 117.88      | 123.83   | 390.07     | 789913  | 5398.2   |
| Recursively find all prices                 | jsonpath-ng | 3785.59  | 3816.96     | 3836.75  | 12.05      | 789913  | 5408.7   |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.02     | 3284530.52 | 10      | 5361.4   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 2142949.56 | 10      | 5361.5   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1405832.71 | 10      | 5361.9   |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 2807899.89 | 1       | 1470.3   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 1967032.19 | 1       | 1470.3   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1873497.17 | 1       | 1470.6   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2357916.77 | 0       | 1463.8   |
| Extract all user IDs                        | jmespath    | 2.07     | 2.19        | 2.21     | 20997.55   | 4000    | 1463.7   |
| Extract all user IDs                        | strata      | 30.21    | 30.52       | 30.76    | 1506.70    | 4000    | 1431.7   |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2342898.08 | 0       | 1505.2   |
| Extract all user names                      | jmespath    | 1.94     | 1.94        | 1.96     | 23659.55   | 4000    | 1505.2   |
| Extract all user names                      | strata      | 29.60    | 29.78       | 30.24    | 1544.21    | 4000    | 1473.8   |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.02        | 0.02     | 2452234.17 | 0       | 1433.6   |
| Extract nested timestamp field              | jmespath    | 3.05     | 3.19        | 3.25     | 14430.56   | 4000    | 1433.4   |
| Extract nested timestamp field              | strata      | 30.62    | 30.63       | 32.78    | 1501.11    | 4000    | 1433.0   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2085987.87 | 0       | 1572.7   |
| Extract order item prices (double wildcard) | strata      | 65.76    | 67.04       | 69.20    | 685.84     | 789913  | 1520.2   |
| Extract order item prices (double wildcard) | jmespath    | 179.79   | 182.02      | 182.23   | 252.61     | 789913  | 1572.7   |
| Filter users by age (numeric predicate)     | jmespath    | 5.08     | 5.09        | 5.11     | 9030.99    | 3212    | 1473.2   |
| Filter users by age (numeric predicate)     | strata      | 29.22    | 30.16       | 30.76    | 1524.44    | 3212    | 1473.0   |
| NDJSON first record id                      | strata      | 0.01     | 0.02        | 0.02     | 3015041.63 | 1       | 1428.8   |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.03     | 2280046.42 | 1       | 1428.8   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 1892855.43 | 1       | 1429.3   |
| Recursively find all prices                 | strata      | 105.34   | 106.04      | 107.22   | 433.60     | 789913  | 1520.1   |
| Recursively find all prices                 | jsonpath-ng | 3715.22  | 3720.89     | 3747.05  | 12.36      | 789913  | 1614.8   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 2043528.48 | 10      | 1428.7   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 2043528.48 | 10      | 1428.7   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1414749.64 | 10      | 1429.1   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 709.32   | 800.25      | 911.02   | 459.69 | 10890.8  |
| strata_cursor_reparse | 8034.81  | 8104.25     | 8840.55  | 45.39  | 17833.2  |
- Speedup: 10.13x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 495.09   | 510.02      | 528.64   | 811.37 | 1069.4   |
| strata_cursor_reparse | 3497.96  | 3521.48     | 3571.80  | 117.51 | 2149.6   |
- Speedup: 6.90x

## Summary
- loads json: orjson (204.30 ms)
- loads ndjson: orjson (187.54 ms)
- dumps str: orjson (48.80 ms)
- dumps bytes: orjson (42.08 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (510.02 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 60.4% behind orjson
- Strata gap in loads/ndjson: 7.1% behind orjson
- Strata gap in dumps/str: 758.5% behind orjson
- Strata gap in dumps/bytes: 895.3% behind orjson
