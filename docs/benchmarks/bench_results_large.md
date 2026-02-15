# Strata Benchmark Results — LARGE
Generated: 2026-02-15 00:28:02

## Environment
- Commit: e67973e9173167da1e3b024e759eef7bdc164292
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
| orjson        | 184.52   | 236.17      | 237.18   | 194.70 | 1402.7   |
| msgspec       | 193.55   | 245.81      | 247.44   | 187.07 | 1384.1   |
| ujson         | 241.02   | 315.81      | 320.11   | 145.60 | 1505.7   |
| strata        | 313.34   | 336.98      | 337.91   | 136.46 | 1264.6   |
| json (stdlib) | 302.70   | 351.64      | 354.61   | 130.77 | 1384.2   |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| strata        | 182.92   | 207.39      | 209.42   | 221.70 | 2094.8   |
| orjson        | 161.23   | 218.12      | 220.94   | 210.80 | 2105.6   |
| msgspec       | 168.24   | 224.29      | 226.54   | 205.00 | 2082.9   |
| ujson         | 216.55   | 293.85      | 294.86   | 156.47 | 2204.3   |
| json (stdlib) | 274.64   | 336.33      | 337.76   | 136.71 | 2085.6   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 41.61    | 42.24       | 43.06    | 965.43 | 40779839       | 2578.3   |
| msgspec | 51.89    | 52.92       | 53.33    | 770.58 | 40779839       | 2894.6   |
| ujson   | 174.54   | 177.79      | 179.38   | 229.37 | 40779839       | 3206.3   |
| strata  | 209.38   | 211.68      | 298.37   | 225.14 | 47655981       | 2263.3   |
| json    | 286.73   | 287.41      | 294.12   | 159.99 | 45983395       | 3228.9   |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 39.28    | 39.68       | 39.83    | 1027.84 | 40779839       | 3537.7   |
| msgspec | 49.32    | 49.95       | 50.12    | 816.46  | 40779839       | 3846.1   |
| ujson   | 183.30   | 186.53      | 189.29   | 218.63  | 40779839       | 3596.1   |
| strata  | 208.12   | 209.04      | 211.23   | 227.98  | 47655981       | 3242.0   |
| json    | 281.41   | 283.85      | 290.49   | 162.00  | 45983395       | 3855.9   |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2323332.21 | 1       | 10403.7  |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 2086172.60 | 1       | 10426.7  |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1771453.87 | 1       | 10400.7  |
| Extract all user IDs                        | jmespath    | 1.78     | 1.81        | 11.93    | 25360.24   | 4000    | 7934.6   |
| Extract all user IDs                        | strata      | 4.34     | 4.52        | 5.13     | 10172.66   | 4000    | 8783.6   |
| Extract all user IDs                        | jsonpath-ng | 91.60    | 92.78       | 93.12    | 495.59     | 4000    | 7936.4   |
| Extract all user names                      | jmespath    | 1.28     | 1.35        | 1.55     | 34094.38   | 4000    | 11265.0  |
| Extract all user names                      | strata      | 4.62     | 4.70        | 49.19    | 9786.39    | 4000    | 11385.7  |
| Extract all user names                      | jsonpath-ng | 91.27    | 92.04       | 92.48    | 499.60     | 4000    | 11265.0  |
| Extract nested timestamp field              | jmespath    | 2.66     | 2.74        | 2.77     | 16766.46   | 4000    | 8457.1   |
| Extract nested timestamp field              | strata      | 5.51     | 5.52        | 5.70     | 8332.46    | 4000    | 8457.1   |
| Extract nested timestamp field              | jsonpath-ng | 94.14    | 94.18       | 94.51    | 488.26     | 4000    | 8458.6   |
| Extract order item prices (double wildcard) | jmespath    | 273.86   | 275.47      | 275.49   | 166.92     | 4000    | 9646.4   |
| Extract order item prices (double wildcard) | strata      | 819.26   | 896.15      | 930.19   | 51.31      | 789913  | 9662.3   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1125.24  | 1137.35     | 1154.57  | 40.43      | 789913  | 9196.7   |
| Filter users by age (numeric predicate)     | strata      | 4.20     | 4.30        | 4.57     | 10700.24   | 3212    | 12616.7  |
| Filter users by age (numeric predicate)     | jmespath    | 4.62     | 4.70        | 4.82     | 9785.52    | 3212    | 12585.3  |
| Recursively find all prices                 | strata      | 889.55   | 891.26      | 893.89   | 51.59      | 789913  | 13935.4  |
| Recursively find all prices                 | jsonpath-ng | 3794.19  | 3798.20     | 3834.86  | 12.11      | 789913  | 12981.2  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 2086172.60 | 10      | 13412.0  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1337698.77 | 10      | 13355.7  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.04        | 0.04     | 1228943.29 | 10      | 13541.4  |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2323129.55 | 1       | 13072.4  |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2247391.20 | 1       | 13000.9  |
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.03     | 1836089.81 | 1       | 13110.6  |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2155315.24 | 0       | 13732.5  |
| Extract all user IDs                        | jmespath    | 2.05     | 2.08        | 2.09     | 22087.78   | 4000    | 13775.8  |
| Extract all user IDs                        | strata      | 93.17    | 93.75       | 94.10    | 490.42     | 4000    | 13936.2  |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2313427.45 | 0       | 12768.2  |
| Extract all user names                      | jmespath    | 2.01     | 2.04        | 2.21     | 22558.72   | 4000    | 12904.9  |
| Extract all user names                      | strata      | 93.92    | 94.07       | 94.95    | 488.79     | 4000    | 12972.5  |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2256655.82 | 0       | 13520.1  |
| Extract nested timestamp field              | jmespath    | 3.14     | 3.16        | 3.17     | 14554.85   | 4000    | 13622.4  |
| Extract nested timestamp field              | strata      | 94.55    | 95.04       | 95.21    | 483.78     | 4000    | 13622.3  |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2105958.61 | 0       | 13171.9  |
| Extract order item prices (double wildcard) | jmespath    | 178.98   | 180.59      | 181.19   | 254.61     | 789913  | 13252.8  |
| Extract order item prices (double wildcard) | strata      | 995.04   | 996.11      | 1020.72  | 46.16      | 789913  | 13442.2  |
| Filter users by age (numeric predicate)     | jmespath    | 5.03     | 5.07        | 5.19     | 9071.89    | 3212    | 12762.6  |
| Filter users by age (numeric predicate)     | strata      | 92.23    | 92.76       | 92.88    | 495.70     | 3212    | 12762.5  |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 2658995.65 | 1       | 14375.4  |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2419967.69 | 1       | 13955.7  |
| NDJSON first record id                      | strata      | 0.02     | 0.03        | 0.03     | 1794315.99 | 1       | 14569.8  |
| Recursively find all prices                 | strata      | 1008.67  | 1032.21     | 1077.33  | 44.54      | 789913  | 12410.1  |
| Recursively find all prices                 | jsonpath-ng | 3756.56  | 3768.10     | 3786.61  | 12.20      | 789913  | 11820.1  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 1988300.06 | 10      | 11800.1  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 1268396.31 | 10      | 11800.1  |
| Slice first 10 users, extract IDs           | strata      | 0.04     | 0.04        | 0.04     | 1223409.14 | 10      | 11828.1  |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2520.18  | 2737.16     | 2806.09  | 134.40 | 13768.1  |
| strata_cursor_reparse | 10654.55 | 11394.32    | 11673.51 | 32.29  | 14120.6  |
- Speedup: 4.16x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2390.75  | 2391.60     | 2414.05  | 173.03 | 11765.4  |
| strata_cursor_reparse | 5496.40  | 5499.56     | 5530.05  | 75.25  | 11498.8  |
- Speedup: 2.30x

## Summary
- loads json: orjson (236.17 ms)
- loads ndjson: strata (207.39 ms)
- dumps str: orjson (42.24 ms)
- dumps bytes: orjson (39.68 ms)
- search: jmespath (0.02 ms)
- cursor reuse: strata_cursor_reuse (2391.60 ms)
- Rule 13: strata not #1 in: loads/json, dumps/str, dumps/bytes
- Strata gap in loads/json: 42.7% behind orjson
- Strata gap in dumps/str: 401.1% behind orjson
- Strata gap in dumps/bytes: 426.9% behind orjson
