# Strata Benchmark Results — LARGE
Generated: 2026-02-15 19:06:23

## Environment
- Commit: 3aec50e8bfc0555c0f180c9f81cd40621c559e6b
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
| orjson        | 177.25   | 227.06      | 238.63   | 202.52 | 1401.5   |
| msgspec       | 182.32   | 235.62      | 240.50   | 195.16 | 1384.0   |
| ujson         | 233.36   | 302.53      | 305.22   | 152.00 | 1505.5   |
| strata        | 285.69   | 311.50      | 314.31   | 147.62 | 1263.5   |
| json (stdlib) | 296.34   | 340.01      | 346.37   | 135.24 | 1384.0   |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| strata        | 172.03   | 194.10      | 199.89   | 236.89 | 2144.9   |
| orjson        | 152.83   | 207.02      | 209.97   | 222.11 | 2148.1   |
| msgspec       | 157.42   | 213.64      | 218.27   | 215.22 | 2126.4   |
| ujson         | 212.99   | 282.23      | 286.13   | 162.92 | 2248.0   |
| json (stdlib) | 262.63   | 321.40      | 322.26   | 143.06 | 2129.3   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 39.97    | 40.17       | 42.07    | 1015.10 | 40779839       | 2654.5   |
| msgspec | 50.16    | 50.51       | 50.95    | 807.31  | 40779839       | 2970.2   |
| ujson   | 170.26   | 171.72      | 172.56   | 237.49  | 40779839       | 3281.8   |
| json    | 270.16   | 271.39      | 281.31   | 169.44  | 45983395       | 3615.2   |
| strata  | 394.44   | 400.30      | 405.12   | 119.05  | 47655981       | 2339.4   |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 37.81    | 37.86       | 38.14    | 1077.16 | 40779839       | 3910.9   |
| msgspec | 47.55    | 47.73       | 62.09    | 854.41  | 40779839       | 3877.4   |
| ujson   | 171.84   | 172.43      | 173.65   | 236.50  | 40779839       | 4152.9   |
| json    | 270.53   | 271.74      | 273.85   | 169.22  | 45983395       | 4241.5   |
| strata  | 396.09   | 397.59      | 397.88   | 119.86  | 47655981       | 3615.2   |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 3385358.80 | 1       | 12734.7  |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 2646528.34 | 1       | 12697.6  |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1889753.35 | 1       | 12654.8  |
| Extract all user IDs                        | strata      | 0.57     | 0.58        | 0.61     | 79809.28   | 4000    | 9078.9   |
| Extract all user IDs                        | jmespath    | 1.78     | 1.80        | 1.92     | 25482.62   | 4000    | 8090.2   |
| Extract all user IDs                        | jsonpath-ng | 90.25    | 92.09       | 93.29    | 499.35     | 4000    | 8091.9   |
| Extract all user names                      | strata      | 0.63     | 0.64        | 0.65     | 71671.67   | 4000    | 13482.2  |
| Extract all user names                      | jmespath    | 1.25     | 1.34        | 1.52     | 34291.43   | 4000    | 13462.2  |
| Extract all user names                      | jsonpath-ng | 89.69    | 90.15       | 91.03    | 510.05     | 4000    | 13409.1  |
| Extract nested timestamp field              | strata      | 0.94     | 0.94        | 1.13     | 48979.31   | 4000    | 11435.6  |
| Extract nested timestamp field              | jmespath    | 2.55     | 2.71        | 2.82     | 16974.31   | 4000    | 11435.7  |
| Extract nested timestamp field              | jsonpath-ng | 92.52    | 92.88       | 98.66    | 495.06     | 4000    | 11436.9  |
| Extract order item prices (double wildcard) | strata      | 50.10    | 50.37       | 52.68    | 912.86     | 789913  | 12564.0  |
| Extract order item prices (double wildcard) | jmespath    | 268.03   | 268.65      | 269.08   | 171.17     | 4000    | 12513.5  |
| Extract order item prices (double wildcard) | jsonpath-ng | 1086.81  | 1086.87     | 1092.00  | 42.31      | 789913  | 12221.7  |
| Filter users by age (numeric predicate)     | strata      | 1.14     | 1.15        | 1.16     | 40040.71   | 3212    | 15192.7  |
| Filter users by age (numeric predicate)     | jmespath    | 4.62     | 4.80        | 4.85     | 9577.88    | 3212    | 15173.8  |
| Recursively find all prices                 | strata      | 110.80   | 111.43      | 112.05   | 412.65     | 789913  | 16474.0  |
| Recursively find all prices                 | jsonpath-ng | 3779.06  | 3808.28     | 3839.59  | 12.07      | 789913  | 15936.4  |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.02     | 3144164.85 | 10      | 17195.6  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 2155506.20 | 10      | 17132.6  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1377779.95 | 10      | 17089.8  |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 2919325.32 | 1       | 18880.7  |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 2441426.25 | 1       | 18818.5  |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2013634.97 | 1       | 18672.8  |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 2525230.26 | 0       | 19299.7  |
| Extract all user IDs                        | jmespath    | 2.11     | 2.11        | 2.20     | 21817.03   | 4000    | 19321.9  |
| Extract all user IDs                        | strata      | 87.43    | 88.30       | 88.59    | 520.70     | 4000    | 19587.8  |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2294264.34 | 0       | 18473.7  |
| Extract all user names                      | jmespath    | 2.04     | 2.05        | 2.06     | 22389.82   | 4000    | 18531.9  |
| Extract all user names                      | strata      | 87.44    | 87.70       | 87.72    | 524.26     | 4000    | 18595.7  |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.02        | 0.02     | 2189492.98 | 0       | 19182.8  |
| Extract nested timestamp field              | jmespath    | 3.32     | 3.33        | 3.47     | 13810.73   | 4000    | 19182.8  |
| Extract nested timestamp field              | strata      | 87.90    | 88.07       | 88.55    | 522.07     | 4000    | 19226.6  |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2224881.78 | 0       | 19060.0  |
| Extract order item prices (double wildcard) | strata      | 124.00   | 126.26      | 127.88   | 364.15     | 789913  | 19170.4  |
| Extract order item prices (double wildcard) | jmespath    | 177.99   | 178.31      | 179.78   | 257.86     | 789913  | 19131.4  |
| Filter users by age (numeric predicate)     | jmespath    | 5.11     | 5.18        | 5.38     | 8869.69    | 3212    | 18381.5  |
| Filter users by age (numeric predicate)     | strata      | 86.91    | 87.34       | 87.49    | 526.46     | 3212    | 18465.5  |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.01        | 0.02     | 3481172.45 | 1       | 19771.6  |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.02     | 3082343.22 | 1       | 19778.2  |
| NDJSON first record id                      | strata      | 0.02     | 0.02        | 0.02     | 2704672.20 | 1       | 19848.6  |
| Recursively find all prices                 | strata      | 164.93   | 165.77      | 168.62   | 277.37     | 789913  | 18412.6  |
| Recursively find all prices                 | jsonpath-ng | 3729.62  | 3748.17     | 3769.80  | 12.27      | 789913  | 18055.1  |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 2220369.61 | 10      | 18062.7  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 1902568.49 | 10      | 18034.7  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1870374.18 | 10      | 18034.7  |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 659.19   | 764.64      | 1058.06  | 481.10 | 19303.6  |
| strata_cursor_reparse | 7272.94  | 7288.71     | 7975.63  | 50.47  | 20669.4  |
- Speedup: 9.53x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 593.46   | 594.51      | 595.71   | 696.06 | 17967.0  |
| strata_cursor_reparse | 3733.88  | 3750.86     | 3866.46  | 110.33 | 16382.0  |
- Speedup: 6.31x

## Summary
- loads json: orjson (227.06 ms)
- loads ndjson: strata (194.10 ms)
- dumps str: orjson (40.17 ms)
- dumps bytes: orjson (37.86 ms)
- search: jsonpath-ng (0.01 ms)
- cursor reuse: strata_cursor_reuse (594.51 ms)
- Rule 13: strata not #1 in: loads/json, dumps/str, dumps/bytes
- Strata gap in loads/json: 37.2% behind orjson
- Strata gap in dumps/str: 896.4% behind orjson
- Strata gap in dumps/bytes: 950.2% behind orjson
