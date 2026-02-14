# Strata Benchmark Results — LARGE
Generated: 2026-02-14 20:50:37

## Environment
- Commit: 9bb824273ac8d1727e85f6fb247640ba83026134
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
| orjson        | 201.34   | 260.39      | 261.35   | 176.60 | 1402.7   |
| msgspec       | 210.07   | 272.53      | 274.40   | 168.73 | 1384.1   |
| ujson         | 267.03   | 350.18      | 351.76   | 131.31 | 1505.6   |
| strata        | 337.01   | 366.20      | 368.98   | 125.57 | 1264.7   |
| json (stdlib) | 319.19   | 381.15      | 383.34   | 120.64 | 1384.2   |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| strata        | 206.35   | 236.11      | 239.21   | 194.74 | 2136.6   |
| orjson        | 177.83   | 238.01      | 241.70   | 193.18 | 2137.7   |
| msgspec       | 185.10   | 245.24      | 247.40   | 187.49 | 2117.0   |
| ujson         | 239.23   | 321.88      | 324.97   | 142.85 | 2238.7   |
| json (stdlib) | 304.15   | 370.71      | 382.08   | 124.03 | 2119.9   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 46.19    | 46.36       | 49.06    | 879.56 | 40779839       | 2882.5   |
| msgspec | 57.67    | 58.15       | 59.19    | 701.31 | 40779839       | 3195.2   |
| ujson   | 195.75   | 195.97      | 198.62   | 208.09 | 40779839       | 3510.7   |
| strata  | 229.73   | 230.92      | 232.20   | 206.37 | 47655981       | 2571.0   |
| json    | 300.14   | 301.48      | 301.93   | 152.53 | 45983395       | 3843.8   |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 43.57    | 44.25       | 46.44    | 921.68 | 40779839       | 4024.5   |
| msgspec | 54.86    | 55.15       | 55.28    | 739.45 | 40779839       | 4164.9   |
| ujson   | 196.68   | 198.62      | 203.18   | 205.32 | 40779839       | 4061.9   |
| strata  | 228.69   | 229.03      | 242.77   | 208.07 | 47655981       | 3853.6   |
| json    | 307.42   | 312.80      | 317.52   | 147.00 | 45983395       | 3359.1   |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 2211592.86 | 1       | 10242.2  |
| Deep path navigation                        | jmespath    | 0.02     | 0.03        | 0.03     | 1839334.64 | 1       | 10212.4  |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1622961.56 | 1       | 10210.4  |
| Extract all user IDs                        | jmespath    | 1.90     | 1.90        | 1.93     | 24174.75   | 4000    | 8274.6   |
| Extract all user IDs                        | strata      | 5.03     | 5.30        | 5.32     | 8668.69    | 4000    | 8274.6   |
| Extract all user IDs                        | jsonpath-ng | 103.88   | 104.43      | 116.51   | 440.31     | 4000    | 7362.0   |
| Extract all user names                      | jmespath    | 1.55     | 1.56        | 1.62     | 29461.60   | 4000    | 11074.1  |
| Extract all user names                      | strata      | 5.03     | 5.29        | 33.77    | 8689.23    | 4000    | 11117.5  |
| Extract all user names                      | jsonpath-ng | 102.65   | 103.23      | 103.39   | 445.46     | 4000    | 11055.5  |
| Extract nested timestamp field              | jmespath    | 3.05     | 3.08        | 3.16     | 14906.68   | 4000    | 9686.5   |
| Extract nested timestamp field              | strata      | 6.29     | 6.46        | 6.54     | 7113.49    | 4000    | 9686.5   |
| Extract nested timestamp field              | jsonpath-ng | 107.35   | 107.61      | 107.71   | 427.30     | 4000    | 9688.3   |
| Extract order item prices (double wildcard) | jmespath    | 297.91   | 298.13      | 299.56   | 154.24     | 4000    | 10106.5  |
| Extract order item prices (double wildcard) | strata      | 928.54   | 1027.66     | 1030.60  | 44.75      | 789913  | 10091.9  |
| Extract order item prices (double wildcard) | jsonpath-ng | 1226.70  | 1227.67     | 1273.74  | 37.46      | 789913  | 9719.8   |
| Filter users by age (numeric predicate)     | strata      | 4.59     | 4.67        | 4.72     | 9855.61    | 3212    | 12425.8  |
| Filter users by age (numeric predicate)     | jmespath    | 5.36     | 5.37        | 5.51     | 8569.80    | 3212    | 12414.8  |
| Recursively find all prices                 | strata      | 994.79   | 1002.22     | 1008.33  | 45.88      | 789913  | 13765.3  |
| Recursively find all prices                 | jsonpath-ng | 4111.16  | 4142.38     | 4247.77  | 11.10      | 789913  | 13133.4  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 1922625.88 | 10      | 13344.2  |
| Slice first 10 users, extract IDs           | strata      | 0.04     | 0.04        | 0.05     | 1130757.86 | 10      | 13368.4  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.05        | 0.05     | 937632.55  | 10      | 13335.5  |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 1981186.36 | 1       | 12350.9  |
| Deep path navigation                        | strata      | 0.03     | 0.03        | 0.04     | 1700295.33 | 1       | 12394.0  |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1608570.28 | 1       | 12317.8  |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2109921.52 | 0       | 12875.7  |
| Extract all user IDs                        | jmespath    | 2.10     | 2.19        | 2.28     | 21038.79   | 4000    | 12904.8  |
| Extract all user IDs                        | strata      | 104.59   | 105.15      | 270.54   | 437.26     | 4000    | 12961.0  |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1949685.12 | 0       | 11978.6  |
| Extract all user names                      | jmespath    | 2.16     | 2.19        | 2.21     | 20993.55   | 4000    | 12052.5  |
| Extract all user names                      | strata      | 105.53   | 105.68      | 106.19   | 435.08     | 4000    | 12270.1  |
| Extract nested timestamp field              | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1782700.19 | 0       | 12642.5  |
| Extract nested timestamp field              | jmespath    | 3.46     | 3.71        | 3.72     | 12379.32   | 4000    | 12678.3  |
| Extract nested timestamp field              | strata      | 106.28   | 106.65      | 404.25   | 431.12     | 4000    | 12781.6  |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2163734.06 | 0       | 12446.8  |
| Extract order item prices (double wildcard) | jmespath    | 192.78   | 194.57      | 194.99   | 236.31     | 789913  | 12476.0  |
| Extract order item prices (double wildcard) | strata      | 1148.42  | 1173.02     | 1490.34  | 39.20      | 789913  | 12460.1  |
| Filter users by age (numeric predicate)     | jmespath    | 5.71     | 5.75        | 5.81     | 8003.26    | 3212    | 10955.0  |
| Filter users by age (numeric predicate)     | strata      | 103.91   | 106.09      | 106.22   | 433.41     | 3212    | 11206.1  |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2308664.08 | 1       | 13509.2  |
| NDJSON first record id                      | strata      | 0.02     | 0.02        | 0.04     | 2002760.55 | 1       | 14042.0  |
| NDJSON first record id                      | jmespath    | 0.02     | 0.03        | 0.03     | 1806086.22 | 1       | 13734.6  |
| Recursively find all prices                 | strata      | 1185.87  | 1193.67     | 1202.91  | 38.52      | 789913  | 10872.2  |
| Recursively find all prices                 | jsonpath-ng | 4106.51  | 4118.85     | 4119.53  | 11.16      | 789913  | 10431.9  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.05     | 1637268.43 | 10      | 10418.0  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.04     | 0.04        | 0.04     | 1182749.28 | 10      | 10418.0  |
| Slice first 10 users, extract IDs           | strata      | 0.04     | 0.04        | 0.07     | 1137653.16 | 10      | 10446.0  |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2896.06  | 2917.45     | 2948.95  | 126.09 | 13601.9  |
| strata_cursor_reparse | 11286.43 | 11990.95    | 12885.26 | 30.68  | 13732.4  |
- Speedup: 4.11x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2693.72  | 2701.57     | 2732.07  | 153.18 | 10430.7  |
| strata_cursor_reparse | 6099.51  | 6106.94     | 6126.41  | 67.76  | 10609.1  |
- Speedup: 2.26x

## Summary
- loads json: orjson (260.39 ms)
- loads ndjson: strata (236.11 ms)
- dumps str: orjson (46.36 ms)
- dumps bytes: orjson (44.25 ms)
- search: jsonpath-ng (0.02 ms)
- cursor reuse: strata_cursor_reuse (2701.57 ms)
- Rule 13: strata not #1 in: loads/json, dumps/str, dumps/bytes
- Strata gap in loads/json: 40.6% behind orjson
- Strata gap in dumps/str: 398.1% behind orjson
- Strata gap in dumps/bytes: 417.6% behind orjson
