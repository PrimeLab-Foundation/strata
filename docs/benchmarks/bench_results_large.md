# Strata Benchmark Results — LARGE
Generated: 2026-02-18 13:16:15

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
- json: benchmarks/data/generated/large/users.json (43.85 MB, 4000 records)
- ndjson: benchmarks/data/generated/large/users.ndjson (43.85 MB, 4000 records, 4000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (43.85 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 156.90   | 212.44      | 228.80   | 216.45 | 1046.9   |
| msgspec       | 167.03   | 228.63      | 244.44   | 201.13 | 1031.3   |
| ujson         | 231.19   | 310.57      | 346.37   | 148.06 | 1151.8   |
| json (stdlib) | 291.13   | 341.87      | 357.89   | 134.50 | 1031.4   |
| strata        | 330.52   | 360.06      | 381.20   | 127.71 | 909.9    |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 190.01   | 197.80      | 200.86   | 232.46 | 1672.2   |
| msgspec       | 202.87   | 212.41      | 218.82   | 216.46 | 1655.6   |
| strata        | 195.87   | 219.42      | 241.42   | 209.55 | 1668.8   |
| ujson         | 253.64   | 283.01      | 356.04   | 162.46 | 1767.2   |
| json (stdlib) | 310.41   | 316.71      | 327.60   | 145.18 | 1646.6   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 47.42    | 48.45       | 49.64    | 841.67 | 40779839       | 772.8    |
| msgspec | 53.50    | 54.84       | 60.43    | 743.65 | 40779839       | 1052.9   |
| ujson   | 184.88   | 186.43      | 190.87   | 218.75 | 40779839       | 1329.6   |
| json    | 291.99   | 295.79      | 299.96   | 155.46 | 45983395       | 1685.8   |
| strata  | 419.40   | 421.35      | 430.99   | 113.10 | 47655981       | 762.7    |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 42.52    | 43.57       | 45.82    | 935.91 | 40779839       | 754.8    |
| msgspec | 50.64    | 51.19       | 54.48    | 796.70 | 40779839       | 1005.5   |
| ujson   | 181.12   | 185.74      | 187.96   | 219.56 | 40779839       | 1314.1   |
| json    | 295.20   | 300.69      | 311.35   | 152.93 | 45983395       | 1176.2   |
| strata  | 421.73   | 424.30      | 425.79   | 112.32 | 47655981       | 763.3    |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 3180270.41 | 1       | 5362.1   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 1942932.81 | 1       | 5299.3   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1679758.43 | 1       | 4953.8   |
| Extract all user IDs                        | strata      | 0.59     | 0.61        | 0.65     | 74927.16   | 4000    | 3904.0   |
| Extract all user IDs                        | jmespath    | 1.78     | 1.93        | 1.94     | 23772.22   | 4000    | 3876.4   |
| Extract all user IDs                        | jsonpath-ng | 34.89    | 35.27       | 37.31    | 1303.79    | 4000    | 3818.4   |
| Extract all user names                      | strata      | 0.71     | 0.76        | 0.87     | 60844.72   | 4000    | 5362.3   |
| Extract all user names                      | jmespath    | 1.43     | 1.55        | 1.59     | 29650.77   | 4000    | 4770.0   |
| Extract all user names                      | jsonpath-ng | 35.88    | 36.08       | 36.12    | 1274.47    | 4000    | 4108.4   |
| Extract nested timestamp field              | strata      | 1.00     | 1.09        | 1.14     | 42003.56   | 4000    | 5071.1   |
| Extract nested timestamp field              | jmespath    | 2.59     | 3.04        | 3.07     | 15130.89   | 4000    | 4948.8   |
| Extract nested timestamp field              | jsonpath-ng | 35.31    | 40.34       | 40.90    | 1140.00    | 4000    | 4936.6   |
| Extract order item prices (double wildcard) | strata      | 57.65    | 58.84       | 60.48    | 781.44     | 789913  | 5263.6   |
| Extract order item prices (double wildcard) | jmespath    | 219.76   | 221.93      | 230.86   | 207.20     | 4000    | 5231.7   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1115.48  | 1120.34     | 1128.32  | 41.04      | 789913  | 4707.3   |
| Filter users by age (numeric predicate)     | strata      | 1.19     | 1.20        | 1.22     | 38328.82   | 3212    | 5022.5   |
| Filter users by age (numeric predicate)     | jmespath    | 4.80     | 4.82        | 4.91     | 9532.21    | 3212    | 4990.1   |
| Recursively find all prices                 | strata      | 118.37   | 119.49      | 124.03   | 384.83     | 789913  | 5278.5   |
| Recursively find all prices                 | jsonpath-ng | 3936.70  | 3947.60     | 3970.81  | 11.65      | 789913  | 4283.4   |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.02     | 3246039.82 | 10      | 5281.2   |
| Slice first 10 users, extract IDs           | jmespath    | 0.03     | 0.03        | 0.03     | 1615832.75 | 10      | 5216.5   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1332850.88 | 10      | 4893.1   |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2151080.56 | 1       | 1429.0   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 1984693.20 | 1       | 1429.0   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1870296.68 | 1       | 1429.4   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2086087.03 | 0       | 1505.3   |
| Extract all user IDs                        | jmespath    | 2.06     | 2.12        | 2.30     | 21638.20   | 4000    | 1505.3   |
| Extract all user IDs                        | strata      | 29.80    | 30.85       | 32.45    | 1490.40    | 4000    | 1473.2   |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 1860837.18 | 0       | 1505.3   |
| Extract all user names                      | jmespath    | 2.01     | 2.04        | 2.13     | 22592.45   | 4000    | 1505.3   |
| Extract all user names                      | strata      | 32.76    | 34.79       | 37.27    | 1321.45    | 4000    | 1474.4   |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2109918.70 | 0       | 1474.5   |
| Extract nested timestamp field              | jmespath    | 3.36     | 3.43        | 3.47     | 13411.26   | 4000    | 1474.5   |
| Extract nested timestamp field              | strata      | 30.95    | 31.63       | 32.07    | 1453.77    | 4000    | 1474.2   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1946219.28 | 0       | 1578.0   |
| Extract order item prices (double wildcard) | strata      | 67.79    | 69.11       | 75.68    | 665.34     | 789913  | 1520.1   |
| Extract order item prices (double wildcard) | jmespath    | 195.02   | 198.97      | 203.73   | 231.09     | 789913  | 1578.0   |
| Filter users by age (numeric predicate)     | jmespath    | 5.31     | 5.46        | 5.54     | 8425.63    | 3212    | 1473.7   |
| Filter users by age (numeric predicate)     | strata      | 32.95    | 34.13       | 34.35    | 1347.05    | 3212    | 1473.3   |
| NDJSON first record id                      | strata      | 0.01     | 0.02        | 0.03     | 2704667.57 | 1       | 1429.0   |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.04     | 2554407.29 | 1       | 1429.0   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2275193.27 | 1       | 1429.5   |
| Recursively find all prices                 | strata      | 108.14   | 113.86      | 114.00   | 403.83     | 789913  | 1478.9   |
| Recursively find all prices                 | jsonpath-ng | 3841.97  | 3941.56     | 3952.04  | 11.67      | 789913  | 1572.5   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 2163737.02 | 10      | 1428.7   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.04        | 0.04     | 1295194.67 | 10      | 1428.7   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.05     | 1223376.93 | 10      | 1429.1   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 1058.58  | 1061.01     | 1174.01  | 346.72 | 5838.6   |
| strata_cursor_reparse | 11431.30 | 12046.80    | 12241.25 | 30.54  | 9331.4   |
- Speedup: 11.35x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 513.18   | 525.18      | 553.84   | 787.94 | 1049.1   |
| strata_cursor_reparse | 3635.80  | 3679.08     | 3721.16  | 112.48 | 2148.4   |
- Speedup: 7.01x

## Summary
- loads json: orjson (212.44 ms)
- loads ndjson: orjson (197.80 ms)
- dumps str: orjson (48.45 ms)
- dumps bytes: orjson (43.57 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (525.18 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 69.5% behind orjson
- Strata gap in loads/ndjson: 10.9% behind orjson
- Strata gap in dumps/str: 769.7% behind orjson
- Strata gap in dumps/bytes: 873.8% behind orjson
