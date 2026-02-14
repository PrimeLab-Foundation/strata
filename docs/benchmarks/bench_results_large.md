# Strata Benchmark Results — LARGE
Generated: 2026-02-14 19:41:44

## Environment
- Commit: ce2c30a88ba06496f61d061303b781b3665f5fe9
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
| orjson        | 182.40   | 236.13      | 237.07   | 194.74 | 1402.0   |
| msgspec       | 190.62   | 242.61      | 244.05   | 189.54 | 1384.3   |
| ujson         | 238.01   | 313.57      | 321.92   | 146.64 | 1505.9   |
| strata        | 301.95   | 329.65      | 336.06   | 139.49 | 1263.9   |
| json (stdlib) | 299.67   | 352.09      | 359.22   | 130.60 | 1384.4   |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| strata        | 190.96   | 212.50      | 215.92   | 216.38 | 2108.5   |
| orjson        | 158.65   | 214.48      | 218.50   | 214.38 | 2119.5   |
| msgspec       | 169.24   | 221.50      | 223.83   | 207.58 | 2096.8   |
| ujson         | 216.16   | 292.06      | 294.46   | 157.43 | 2218.3   |
| json (stdlib) | 271.82   | 331.53      | 333.94   | 138.69 | 2099.5   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 45.88    | 46.22       | 47.85    | 882.26 | 40779839       | 2574.5   |
| msgspec | 56.82    | 57.47       | 57.73    | 709.53 | 40779839       | 2574.0   |
| ujson   | 180.10   | 181.48      | 183.48   | 224.71 | 40779839       | 2582.2   |
| strata  | 214.51   | 215.71      | 226.13   | 220.93 | 47655981       | 2505.2   |
| json    | 285.75   | 288.89      | 298.55   | 159.17 | 45983395       | 2573.6   |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 39.50    | 39.65       | 39.94    | 1028.56 | 40779839       | 3201.4   |
| msgspec | 49.80    | 49.96       | 50.04    | 816.22  | 40779839       | 3509.7   |
| ujson   | 177.86   | 183.57      | 186.71   | 222.14  | 40779839       | 3476.6   |
| strata  | 212.81   | 214.32      | 216.65   | 222.36  | 47655981       | 2937.6   |
| json    | 288.39   | 290.60      | 291.43   | 158.23  | 45983395       | 3542.9   |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2338099.01 | 1       | 11083.5  |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 2261404.04 | 1       | 11106.5  |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1754555.72 | 1       | 10973.6  |
| Extract all user IDs                        | jmespath    | 1.78     | 1.79        | 10.90    | 25653.82   | 4000    | 7941.3   |
| Extract all user IDs                        | strata      | 4.86     | 4.94        | 5.01     | 9313.72    | 4000    | 8619.0   |
| Extract all user IDs                        | jsonpath-ng | 90.53    | 90.67       | 90.93    | 507.14     | 4000    | 7943.2   |
| Extract all user names                      | jmespath    | 1.25     | 1.39        | 1.42     | 33124.28   | 4000    | 11675.9  |
| Extract all user names                      | strata      | 4.95     | 4.96        | 5.74     | 9280.12    | 4000    | 11686.5  |
| Extract all user names                      | jsonpath-ng | 90.16    | 90.67       | 91.12    | 507.14     | 4000    | 11655.6  |
| Extract nested timestamp field              | jmespath    | 2.56     | 2.67        | 2.83     | 17248.09   | 4000    | 8667.4   |
| Extract nested timestamp field              | strata      | 5.55     | 5.56        | 5.63     | 8273.99    | 4000    | 8667.3   |
| Extract nested timestamp field              | jsonpath-ng | 93.13    | 93.40       | 95.44    | 492.32     | 4000    | 8668.7   |
| Extract order item prices (double wildcard) | jmespath    | 272.80   | 274.65      | 285.32   | 167.43     | 4000    | 9590.2   |
| Extract order item prices (double wildcard) | strata      | 824.53   | 831.65      | 837.14   | 55.29      | 789913  | 9572.4   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1110.20  | 1115.39     | 1149.62  | 41.23      | 789913  | 9598.6   |
| Filter users by age (numeric predicate)     | strata      | 4.63     | 4.67        | 4.69     | 9839.62    | 3212    | 13218.9  |
| Filter users by age (numeric predicate)     | jmespath    | 4.78     | 4.82        | 4.90     | 9530.98    | 3212    | 13218.9  |
| Recursively find all prices                 | strata      | 914.73   | 916.68      | 919.73   | 50.16      | 789913  | 14089.0  |
| Recursively find all prices                 | jsonpath-ng | 3757.85  | 3793.21     | 3802.48  | 12.12      | 789913  | 13350.8  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 1936142.44 | 10      | 13878.9  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 1446382.32 | 10      | 14005.4  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 1280303.24 | 10      | 13824.4  |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 2414755.97 | 1       | 12169.0  |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 1991915.04 | 1       | 12122.0  |
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.03     | 1649484.12 | 1       | 12228.9  |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2247391.20 | 0       | 13209.4  |
| Extract all user IDs                        | jmespath    | 2.05     | 2.09        | 2.14     | 22017.70   | 4000    | 14442.6  |
| Extract all user IDs                        | strata      | 92.80    | 93.23       | 93.28    | 493.16     | 4000    | 14789.5  |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2172218.91 | 0       | 11891.0  |
| Extract all user names                      | jmespath    | 2.01     | 2.05        | 2.06     | 22397.56   | 4000    | 11891.0  |
| Extract all user names                      | strata      | 92.34    | 92.69       | 92.73    | 496.05     | 4000    | 12071.9  |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2233850.81 | 0       | 12896.1  |
| Extract nested timestamp field              | jmespath    | 3.31     | 3.35        | 3.40     | 13737.49   | 4000    | 12938.1  |
| Extract nested timestamp field              | strata      | 93.60    | 93.95       | 94.65    | 489.40     | 4000    | 13094.2  |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2242898.81 | 0       | 12412.0  |
| Extract order item prices (double wildcard) | jmespath    | 179.01   | 179.30      | 180.35   | 256.45     | 789913  | 12500.2  |
| Extract order item prices (double wildcard) | strata      | 1017.72  | 1042.33     | 1084.99  | 44.11      | 789913  | 12582.3  |
| Filter users by age (numeric predicate)     | jmespath    | 5.10     | 5.21        | 5.23     | 8819.01    | 3212    | 11789.5  |
| Filter users by age (numeric predicate)     | strata      | 92.37    | 92.59       | 97.61    | 496.58     | 3212    | 11792.4  |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 2621252.89 | 1       | 15140.7  |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2256655.82 | 1       | 14848.1  |
| NDJSON first record id                      | strata      | 0.02     | 0.02        | 0.03     | 1981186.36 | 1       | 15177.7  |
| Recursively find all prices                 | strata      | 944.00   | 946.48      | 1081.72  | 48.58      | 789913  | 11673.8  |
| Recursively find all prices                 | jsonpath-ng | 3762.60  | 3765.72     | 3784.80  | 12.21      | 789913  | 11084.7  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 1925837.49 | 10      | 11063.5  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1398612.93 | 10      | 10915.3  |
| Slice first 10 users, extract IDs           | strata      | 0.04     | 0.04        | 0.04     | 1227491.77 | 10      | 11091.5  |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2506.06  | 2899.05     | 2919.06  | 126.89 | 13939.1  |
| strata_cursor_reparse | 11040.25 | 11696.16    | 11722.59 | 31.45  | 15610.9  |
- Speedup: 4.03x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2448.65  | 2452.96     | 2464.69  | 168.70 | 10928.2  |
| strata_cursor_reparse | 5897.38  | 6103.69     | 6159.89  | 67.80  | 10329.5  |
- Speedup: 2.49x

## Summary
- loads json: orjson (236.13 ms)
- loads ndjson: strata (212.50 ms)
- dumps str: orjson (46.22 ms)
- dumps bytes: orjson (39.65 ms)
- search: jmespath (0.02 ms)
- cursor reuse: strata_cursor_reuse (2452.96 ms)
- Rule 13: strata not #1 in: loads/json, dumps/str, dumps/bytes
- Strata gap in loads/json: 39.6% behind orjson
- Strata gap in dumps/str: 366.7% behind orjson
- Strata gap in dumps/bytes: 440.6% behind orjson
