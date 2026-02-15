# Strata Benchmark Results — MEDIUM
Generated: 2026-02-15 19:02:06

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
- json: benchmarks/data/generated/medium/users.json (6.25 MB, 2000 records)
- ndjson: benchmarks/data/generated/medium/users.ndjson (6.25 MB, 2000 records, 2000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (6.25 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| strata        | 32.12    | 35.71       | 36.15    | 183.48 | 184.6    |
| orjson        | 27.01    | 36.30       | 38.25    | 180.51 | 204.5    |
| msgspec       | 28.19    | 37.92       | 39.20    | 172.79 | 202.3    |
| ujson         | 35.69    | 47.81       | 48.32    | 137.04 | 219.1    |
| json (stdlib) | 43.12    | 53.12       | 54.49    | 123.34 | 202.4    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 23.83    | 32.65       | 33.97    | 200.61 | 242.4    |
| msgspec       | 24.39    | 33.80       | 34.47    | 193.80 | 242.9    |
| ujson         | 31.06    | 42.67       | 43.84    | 153.51 | 259.0    |
| strata        | 41.59    | 46.18       | 46.42    | 141.83 | 242.2    |
| json (stdlib) | 39.20    | 49.34       | 50.35    | 132.76 | 248.1    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.26     | 6.29        | 6.53     | 923.64 | 5811025        | 308.5    |
| msgspec | 7.24     | 7.70        | 7.84     | 754.69 | 5811025        | 352.6    |
| ujson   | 24.01    | 24.42       | 25.52    | 237.96 | 5811025        | 352.9    |
| json    | 38.35    | 39.65       | 43.49    | 165.24 | 6551664        | 403.3    |
| strata  | 50.96    | 51.25       | 51.76    | 130.55 | 6690995        | 301.8    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 5.95     | 6.18        | 6.52     | 939.95 | 5811025        | 400.6    |
| msgspec | 6.80     | 7.02        | 7.49     | 827.93 | 5811025        | 445.2    |
| ujson   | 24.64    | 25.04       | 25.48    | 232.10 | 5811025        | 447.4    |
| json    | 38.32    | 38.43       | 39.05    | 170.46 | 6551664        | 497.5    |
| strata  | 53.74    | 56.09       | 58.67    | 119.29 | 6690995        | 403.3    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 614256.84 | 1       | 1638.5   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 453120.55 | 1       | 1638.5   |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 447977.18 | 1       | 1638.5   |
| Extract all user IDs                        | strata      | 0.35     | 0.38        | 0.38     | 17391.84  | 2000    | 901.1    |
| Extract all user IDs                        | jmespath    | 0.75     | 0.76        | 0.84     | 8631.49   | 2000    | 901.1    |
| Extract all user IDs                        | jsonpath-ng | 17.45    | 17.55       | 18.03    | 373.33    | 2000    | 901.2    |
| Extract all user names                      | strata      | 0.32     | 0.34        | 0.35     | 19133.58  | 2000    | 1768.9   |
| Extract all user names                      | jmespath    | 0.55     | 0.56        | 0.58     | 11650.84  | 2000    | 1768.9   |
| Extract all user names                      | jsonpath-ng | 17.46    | 17.56       | 17.60    | 373.12    | 2000    | 1768.9   |
| Extract nested timestamp field              | strata      | 0.50     | 0.59        | 0.60     | 11146.25  | 2000    | 1141.3   |
| Extract nested timestamp field              | jmespath    | 1.25     | 1.26        | 1.37     | 5206.97   | 2000    | 1141.3   |
| Extract nested timestamp field              | jsonpath-ng | 19.37    | 19.41       | 19.49    | 337.61    | 2000    | 1141.4   |
| Extract order item prices (double wildcard) | strata      | 5.71     | 5.83        | 6.05     | 1124.73   | 100999  | 1497.9   |
| Extract order item prices (double wildcard) | jmespath    | 42.86    | 43.08       | 43.57    | 152.07    | 2000    | 1497.9   |
| Extract order item prices (double wildcard) | jsonpath-ng | 138.27   | 138.60      | 138.83   | 47.27     | 100999  | 1500.8   |
| Filter users by age (numeric predicate)     | strata      | 0.52     | 0.53        | 0.55     | 12276.69  | 1606    | 1856.6   |
| Filter users by age (numeric predicate)     | jmespath    | 2.29     | 2.34        | 2.42     | 2802.85   | 1606    | 1856.6   |
| Recursively find all prices                 | strata      | 13.79    | 13.96       | 14.00    | 469.36    | 100999  | 1937.0   |
| Recursively find all prices                 | jsonpath-ng | 519.77   | 521.06      | 527.90   | 12.57     | 100999  | 1939.1   |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 745184.27 | 10      | 2109.0   |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 439207.21 | 10      | 2109.0   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 310137.72 | 10      | 2109.0   |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 586570.86 | 1       | 4401.6   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 489694.22 | 1       | 4401.6   |
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 414770.98 | 1       | 4401.6   |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 551549.58 | 0       | 4399.5   |
| Extract all user IDs                        | jmespath    | 0.91     | 0.93        | 0.94     | 7021.56   | 2000    | 4399.5   |
| Extract all user IDs                        | strata      | 16.09    | 16.25       | 16.36    | 403.10    | 2000    | 4391.4   |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.01        | 0.01     | 663254.86 | 0       | 4401.7   |
| Extract all user names                      | jmespath    | 0.81     | 0.81        | 0.85     | 8057.80   | 2000    | 4401.7   |
| Extract all user names                      | strata      | 16.23    | 16.35       | 16.51    | 400.67    | 2000    | 4401.7   |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.01        | 0.01     | 708071.89 | 0       | 4399.6   |
| Extract nested timestamp field              | jmespath    | 1.49     | 1.50        | 1.50     | 4378.60   | 2000    | 4399.6   |
| Extract nested timestamp field              | strata      | 16.40    | 16.51       | 16.68    | 396.74    | 2000    | 4399.6   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.01     | 0.01        | 0.02     | 611659.92 | 0       | 4401.6   |
| Extract order item prices (double wildcard) | strata      | 20.68    | 20.86       | 20.89    | 313.98    | 100999  | 4401.3   |
| Extract order item prices (double wildcard) | jmespath    | 25.18    | 25.44       | 25.52    | 257.48    | 100999  | 4401.6   |
| Filter users by age (numeric predicate)     | jmespath    | 2.33     | 2.35        | 2.58     | 2784.91   | 1606    | 4401.7   |
| Filter users by age (numeric predicate)     | strata      | 16.12    | 16.13       | 16.22    | 405.98    | 1606    | 4401.7   |
| NDJSON first record id                      | strata      | 0.01     | 0.01        | 0.01     | 741499.52 | 1       | 4374.6   |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.01        | 0.01     | 668946.67 | 1       | 4374.6   |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.02     | 499021.81 | 1       | 4374.6   |
| Recursively find all prices                 | strata      | 27.70    | 27.74       | 27.75    | 236.10    | 100999  | 4401.7   |
| Recursively find all prices                 | jsonpath-ng | 520.00   | 520.99      | 528.58   | 12.57     | 100999  | 4402.9   |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.02     | 491235.60 | 10      | 4401.9   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 309427.37 | 10      | 4401.9   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 281197.54 | 10      | 4401.7   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 71.43    | 73.29       | 74.82    | 715.18 | 2507.3   |
| strata_cursor_reparse | 450.01   | 481.88      | 504.47   | 108.77 | 4362.0   |
- Speedup: 6.58x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 89.02    | 89.71       | 92.87    | 657.07 | 4401.8   |
| strata_cursor_reparse | 540.52   | 541.43      | 543.27   | 108.87 | 4419.8   |
- Speedup: 6.04x

## Summary
- loads json: strata (35.71 ms)
- loads ndjson: orjson (32.65 ms)
- dumps str: orjson (6.29 ms)
- dumps bytes: orjson (6.18 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (73.29 ms)
- Rule 13: strata not #1 in: loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/ndjson: 41.4% behind orjson
- Strata gap in dumps/str: 714.7% behind orjson
- Strata gap in dumps/bytes: 807.3% behind orjson
