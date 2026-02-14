# Strata Benchmark Results — MEDIUM
Generated: 2026-02-14 20:44:56

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
- json: benchmarks/data/generated/medium/users.json (6.25 MB, 2000 records)
- ndjson: benchmarks/data/generated/medium/users.ndjson (6.25 MB, 2000 records, 2000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (6.25 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| strata        | 38.61    | 43.07       | 44.00    | 152.11 | 184.6    |
| orjson        | 33.62    | 43.38       | 44.00    | 151.04 | 204.5    |
| msgspec       | 35.59    | 45.84       | 46.20    | 142.94 | 202.3    |
| ujson         | 43.92    | 57.78       | 58.71    | 113.39 | 219.1    |
| json (stdlib) | 50.83    | 62.94       | 63.66    | 104.10 | 202.3    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 28.61    | 39.80       | 40.56    | 164.58 | 235.7    |
| msgspec       | 29.16    | 40.70       | 41.36    | 160.94 | 236.2    |
| strata        | 45.46    | 49.16       | 52.98    | 133.22 | 235.6    |
| ujson         | 37.20    | 51.66       | 52.89    | 126.78 | 252.1    |
| json (stdlib) | 45.65    | 58.51       | 59.28    | 111.95 | 241.2    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 7.73     | 7.87        | 7.97     | 738.62 | 5811025        | 330.0    |
| msgspec | 7.97     | 8.03        | 8.21     | 723.89 | 5811025        | 376.0    |
| ujson   | 27.09    | 27.19       | 27.49    | 213.72 | 5811025        | 376.4    |
| strata  | 31.61    | 31.71       | 32.02    | 210.97 | 6690995        | 327.5    |
| json    | 43.24    | 43.32       | 43.45    | 151.23 | 6551664        | 426.7    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.81     | 6.92        | 7.03     | 840.19 | 5811025        | 475.2    |
| msgspec | 7.58     | 7.78        | 7.83     | 746.51 | 5811025        | 519.8    |
| ujson   | 27.08    | 27.39       | 27.75    | 212.17 | 5811025        | 521.8    |
| strata  | 31.36    | 31.54       | 31.73    | 212.13 | 6690995        | 477.9    |
| json    | 41.95    | 42.38       | 42.55    | 154.59 | 6551664        | 571.9    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 378883.85 | 1       | 1632.4   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 320249.27 | 1       | 1632.4   |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 246460.65 | 1       | 1632.4   |
| Extract all user IDs                        | jmespath    | 0.89     | 0.94        | 0.98     | 6978.52   | 2000    | 975.3    |
| Extract all user IDs                        | strata      | 2.76     | 2.87        | 3.43     | 2281.49   | 2000    | 975.2    |
| Extract all user IDs                        | jsonpath-ng | 21.84    | 21.97       | 22.39    | 298.23    | 2000    | 975.4    |
| Extract all user names                      | jmespath    | 0.57     | 0.70        | 0.77     | 9412.19   | 2000    | 1831.9   |
| Extract all user names                      | strata      | 2.62     | 2.83        | 2.88     | 2312.62   | 2000    | 1831.9   |
| Extract all user names                      | jsonpath-ng | 20.55    | 22.11       | 22.30    | 296.34    | 2000    | 1831.9   |
| Extract nested timestamp field              | jmespath    | 1.41     | 1.42        | 1.45     | 4614.52   | 2000    | 1131.9   |
| Extract nested timestamp field              | strata      | 3.01     | 3.10        | 3.16     | 2114.24   | 2000    | 1131.8   |
| Extract nested timestamp field              | jsonpath-ng | 22.04    | 23.65       | 23.74    | 276.99    | 2000    | 1131.9   |
| Extract order item prices (double wildcard) | jmespath    | 50.57    | 50.73       | 50.82    | 129.15    | 2000    | 1329.9   |
| Extract order item prices (double wildcard) | strata      | 132.92   | 134.08      | 134.34   | 48.86     | 100999  | 1329.9   |
| Extract order item prices (double wildcard) | jsonpath-ng | 162.00   | 162.08      | 162.18   | 40.42     | 100999  | 1333.6   |
| Filter users by age (numeric predicate)     | strata      | 1.99     | 2.55        | 2.61     | 2569.41   | 1606    | 1954.2   |
| Filter users by age (numeric predicate)     | jmespath    | 2.57     | 2.59        | 2.60     | 2532.13   | 1606    | 1954.2   |
| Recursively find all prices                 | strata      | 135.33   | 142.68      | 147.09   | 45.92     | 100999  | 2049.2   |
| Recursively find all prices                 | jsonpath-ng | 598.63   | 599.94      | 601.03   | 10.92     | 100999  | 2052.9   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 262950.15 | 10      | 2171.3   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 213931.88 | 10      | 2171.3   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 173941.05 | 10      | 2171.3   |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 308219.15 | 1       | 4667.7   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 297711.20 | 1       | 4667.7   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 237805.98 | 1       | 4667.7   |
| Extract all user IDs                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 240725.20 | 0       | 4661.2   |
| Extract all user IDs                        | jmespath    | 1.07     | 1.08        | 1.09     | 6046.53   | 2000    | 4661.2   |
| Extract all user IDs                        | strata      | 22.72    | 22.91       | 22.91    | 285.94    | 2000    | 4660.9   |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 279207.56 | 0       | 4667.7   |
| Extract all user names                      | jmespath    | 1.03     | 1.06        | 1.08     | 6173.09   | 2000    | 4667.7   |
| Extract all user names                      | strata      | 22.72    | 23.00       | 23.82    | 284.75    | 2000    | 4667.7   |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.03        | 0.04     | 230151.66 | 0       | 4661.6   |
| Extract nested timestamp field              | jmespath    | 1.80     | 1.81        | 1.86     | 3622.76   | 2000    | 4661.6   |
| Extract nested timestamp field              | strata      | 23.44    | 23.63       | 23.67    | 277.23    | 2000    | 4661.6   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.02     | 315023.61 | 0       | 4664.2   |
| Extract order item prices (double wildcard) | jmespath    | 28.72    | 28.87       | 29.50    | 226.85    | 100999  | 4664.2   |
| Extract order item prices (double wildcard) | strata      | 153.02   | 153.03      | 153.41   | 42.80     | 100999  | 4663.4   |
| Filter users by age (numeric predicate)     | jmespath    | 2.78     | 2.82        | 2.85     | 2324.87   | 1606    | 4667.7   |
| Filter users by age (numeric predicate)     | strata      | 21.96    | 22.41       | 22.67    | 292.26    | 1606    | 4667.7   |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.03     | 284261.06 | 1       | 4643.7   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 280199.02 | 1       | 4643.8   |
| NDJSON first record id                      | strata      | 0.02     | 0.03        | 0.04     | 254760.96 | 1       | 4643.7   |
| Recursively find all prices                 | strata      | 159.43   | 161.26      | 161.37   | 40.62     | 100999  | 4667.8   |
| Recursively find all prices                 | jsonpath-ng | 595.75   | 598.28      | 600.30   | 10.95     | 100999  | 4670.0   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 234965.06 | 10      | 4669.0   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 203613.99 | 10      | 4668.8   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.04        | 0.04     | 160071.73 | 10      | 4669.0   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 347.32   | 347.36      | 348.34   | 150.89 | 2380.5   |
| strata_cursor_reparse | 774.47   | 799.01      | 841.33   | 65.60  | 4637.3   |
- Speedup: 2.30x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 366.72   | 366.97      | 368.73   | 160.63 | 4668.9   |
| strata_cursor_reparse | 864.17   | 867.04      | 901.16   | 67.99  | 4700.3   |
- Speedup: 2.36x

## Summary
- loads json: strata (43.07 ms)
- loads ndjson: orjson (39.80 ms)
- dumps str: orjson (7.87 ms)
- dumps bytes: orjson (6.92 ms)
- search: jmespath (0.02 ms)
- cursor reuse: strata_cursor_reuse (347.36 ms)
- Rule 13: strata not #1 in: loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/ndjson: 23.5% behind orjson
- Strata gap in dumps/str: 303.1% behind orjson
- Strata gap in dumps/bytes: 356.0% behind orjson
