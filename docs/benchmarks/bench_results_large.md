# Strata Benchmark Results — LARGE
Generated: 2026-02-14 18:20:10

## Environment
- Commit: e963a9bbd1de10111d9958909b64228f733ac423
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
| orjson        | 181.86   | 234.68      | 238.39   | 195.94 | 1401.6   |
| msgspec       | 193.66   | 248.72      | 249.24   | 184.88 | 1382.9   |
| ujson         | 241.21   | 317.44      | 320.10   | 144.86 | 1504.5   |
| strata        | 292.19   | 323.80      | 325.59   | 142.01 | 1262.5   |
| json (stdlib) | 302.03   | 360.21      | 361.01   | 127.66 | 1383.0   |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| strata        | 188.85   | 209.75      | 215.62   | 219.21 | 2151.0   |
| orjson        | 160.00   | 217.57      | 225.65   | 211.33 | 2154.0   |
| msgspec       | 174.66   | 223.81      | 232.60   | 205.44 | 2131.4   |
| ujson         | 218.20   | 296.05      | 296.46   | 155.31 | 2253.0   |
| json (stdlib) | 281.95   | 337.99      | 347.76   | 136.04 | 2134.2   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 42.72    | 42.86       | 42.96    | 951.49 | 40779839       | 2897.1   |
| msgspec | 53.09    | 53.65       | 54.91    | 760.15 | 40779839       | 3212.6   |
| ujson   | 177.85   | 178.47      | 178.76   | 228.49 | 40779839       | 3528.4   |
| strata  | 216.96   | 217.67      | 224.96   | 218.94 | 47655981       | 2583.6   |
| json    | 288.94   | 290.63      | 304.34   | 158.22 | 45983395       | 3861.2   |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 40.06    | 41.82       | 43.10    | 975.21 | 40779839       | 4020.4   |
| msgspec | 52.75    | 53.93       | 54.15    | 756.16 | 40779839       | 3933.7   |
| ujson   | 184.40   | 185.67      | 185.84   | 219.64 | 40779839       | 3344.7   |
| strata  | 214.99   | 217.04      | 218.01   | 219.57 | 47655981       | 4134.0   |
| json    | 294.00   | 294.27      | 294.49   | 156.26 | 45983395       | 3391.3   |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2313629.26 | 1       | 10980.4  |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 2094152.62 | 1       | 11003.4  |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1662030.19 | 1       | 10970.8  |
| Extract all user IDs                        | jmespath    | 1.83     | 1.86        | 16.75    | 24773.87   | 4000    | 7651.1   |
| Extract all user IDs                        | strata      | 5.01     | 5.04        | 5.07     | 9119.54    | 4000    | 8613.2   |
| Extract all user IDs                        | jsonpath-ng | 91.12    | 91.37       | 93.35    | 503.29     | 4000    | 7652.9   |
| Extract all user names                      | jmespath    | 1.30     | 1.33        | 1.34     | 34618.44   | 4000    | 12567.0  |
| Extract all user names                      | strata      | 4.54     | 4.57        | 54.15    | 10059.99   | 4000    | 12582.2  |
| Extract all user names                      | jsonpath-ng | 92.71    | 92.88       | 95.32    | 495.07     | 4000    | 12072.5  |
| Extract nested timestamp field              | jmespath    | 2.58     | 2.66        | 2.68     | 17303.25   | 4000    | 8829.0   |
| Extract nested timestamp field              | strata      | 5.66     | 5.82        | 6.04     | 7894.31    | 4000    | 8829.0   |
| Extract nested timestamp field              | jsonpath-ng | 93.97    | 94.22       | 95.39    | 488.03     | 4000    | 8830.3   |
| Extract order item prices (double wildcard) | jmespath    | 280.48   | 284.66      | 290.46   | 161.54     | 4000    | 9599.8   |
| Extract order item prices (double wildcard) | strata      | 972.36   | 974.49      | 979.58   | 47.19      | 789913  | 9741.8   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1130.57  | 1146.62     | 1189.58  | 40.10      | 789913  | 9273.9   |
| Filter users by age (numeric predicate)     | strata      | 4.21     | 4.31        | 4.35     | 10675.81   | 3212    | 13139.4  |
| Filter users by age (numeric predicate)     | jmespath    | 4.80     | 5.03        | 5.07     | 9134.94    | 3212    | 13044.7  |
| Recursively find all prices                 | strata      | 936.33   | 1058.61     | 1070.19  | 43.44      | 789913  | 12831.5  |
| Recursively find all prices                 | jsonpath-ng | 3903.72  | 3991.63     | 4042.49  | 11.52      | 789913  | 12024.8  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 2010203.10 | 10      | 12884.9  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.04     | 1409453.30 | 10      | 12925.9  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1365830.26 | 10      | 12837.8  |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2256655.82 | 1       | 12932.2  |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2036017.46 | 1       | 12911.6  |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 1974125.84 | 1       | 13009.2  |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 1991829.65 | 0       | 14107.2  |
| Extract all user IDs                        | jmespath    | 2.16     | 2.18        | 2.33     | 21094.29   | 4000    | 14324.6  |
| Extract all user IDs                        | strata      | 94.22    | 94.94       | 99.88    | 484.28     | 4000    | 14681.8  |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2176537.31 | 0       | 11963.5  |
| Extract all user names                      | jmespath    | 2.01     | 2.04        | 2.25     | 22558.27   | 4000    | 11972.6  |
| Extract all user names                      | strata      | 93.31    | 93.62       | 93.82    | 491.15     | 4000    | 12891.7  |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2220366.49 | 0       | 13688.2  |
| Extract nested timestamp field              | jmespath    | 3.52     | 3.58        | 3.64     | 12852.83   | 4000    | 13794.2  |
| Extract nested timestamp field              | strata      | 98.48    | 100.79      | 100.82   | 456.17     | 4000    | 13881.5  |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1922455.84 | 0       | 13018.7  |
| Extract order item prices (double wildcard) | jmespath    | 197.18   | 199.19      | 199.96   | 230.83     | 789913  | 13042.5  |
| Extract order item prices (double wildcard) | strata      | 1076.04  | 1086.46     | 1127.44  | 42.32      | 789913  | 13206.4  |
| Filter users by age (numeric predicate)     | jmespath    | 5.25     | 5.28        | 5.44     | 8708.63    | 3212    | 11884.4  |
| Filter users by age (numeric predicate)     | strata      | 92.69    | 92.83       | 93.00    | 495.30     | 3212    | 11884.4  |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 2404150.78 | 1       | 14906.3  |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2220263.51 | 1       | 14821.6  |
| NDJSON first record id                      | strata      | 0.02     | 0.03        | 0.04     | 1823926.58 | 1       | 15057.1  |
| Recursively find all prices                 | strata      | 1082.32  | 1093.26     | 1094.16  | 42.06      | 789913  | 11696.0  |
| Recursively find all prices                 | jsonpath-ng | 3846.67  | 3847.01     | 3968.36  | 11.95      | 789913  | 11132.3  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 1970572.63 | 10      | 11163.3  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1362352.32 | 10      | 11163.3  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.04        | 0.04     | 1274266.64 | 10      | 11191.3  |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2413.95  | 2945.59     | 3061.57  | 124.89 | 13979.1  |
| strata_cursor_reparse | 10772.54 | 11452.50    | 11873.95 | 32.12  | 15073.5  |
- Speedup: 3.89x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2519.37  | 2522.52     | 2526.67  | 164.05 | 11176.0  |
| strata_cursor_reparse | 5944.51  | 5976.00     | 6102.08  | 69.25  | 11131.3  |
- Speedup: 2.37x

## Summary
- loads json: orjson (234.68 ms)
- loads ndjson: strata (209.75 ms)
- dumps str: orjson (42.86 ms)
- dumps bytes: orjson (41.82 ms)
- search: jmespath (0.02 ms)
- cursor reuse: strata_cursor_reuse (2522.52 ms)
- Rule 13: strata not #1 in: loads/json, dumps/str, dumps/bytes
- Strata gap in loads/json: 38.0% behind orjson
- Strata gap in dumps/str: 407.9% behind orjson
- Strata gap in dumps/bytes: 419.0% behind orjson
