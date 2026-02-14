# Strata Benchmark Results — MEDIUM-PGO
Generated: 2026-02-14 22:06:58

## Environment
- Commit: ce4d80c0cfcd8bbfe70be79dc3e5b1150ad13bb4
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
| strata        | 31.26    | 36.61       | 37.39    | 178.95 | 184.9    |
| orjson        | 34.51    | 43.78       | 45.77    | 149.64 | 204.8    |
| msgspec       | 34.73    | 47.06       | 48.50    | 139.22 | 202.6    |
| ujson         | 43.21    | 56.97       | 57.90    | 115.00 | 219.4    |
| json (stdlib) | 51.26    | 63.14       | 63.83    | 103.76 | 202.7    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 29.54    | 40.95       | 42.30    | 159.93 | 273.8    |
| orjson        | 29.39    | 41.18       | 41.78    | 159.07 | 273.2    |
| strata        | 46.43    | 50.30       | 52.50    | 130.21 | 273.1    |
| ujson         | 38.40    | 52.15       | 55.38    | 125.59 | 289.8    |
| json (stdlib) | 48.18    | 59.45       | 60.43    | 110.17 | 278.9    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| msgspec | 7.97     | 8.08        | 8.26     | 718.83 | 5811025        | 413.8    |
| orjson  | 7.85     | 8.10        | 8.22     | 717.42 | 5811025        | 366.8    |
| ujson   | 27.07    | 27.16       | 27.25    | 213.93 | 5811025        | 414.3    |
| strata  | 29.57    | 29.78       | 30.18    | 224.70 | 6690995        | 365.6    |
| json    | 43.30    | 43.59       | 43.99    | 150.31 | 6551664        | 464.5    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.87     | 6.97        | 7.09     | 833.81 | 5811025        | 513.0    |
| msgspec | 7.76     | 7.77        | 7.85     | 747.63 | 5811025        | 557.7    |
| ujson   | 27.29    | 28.36       | 29.03    | 204.91 | 5811025        | 559.6    |
| strata  | 29.38    | 29.75       | 29.87    | 224.93 | 6690995        | 515.8    |
| json    | 43.57    | 43.75       | 44.10    | 149.76 | 6551664        | 609.8    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 322202.38 | 1       | 1708.4   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 264713.55 | 1       | 1708.4   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 233296.57 | 1       | 1708.4   |
| Extract all user IDs                        | jmespath    | 0.87     | 0.98        | 1.00     | 6672.32   | 2000    | 1012.5   |
| Extract all user IDs                        | strata      | 2.75     | 2.88        | 2.99     | 2272.29   | 2000    | 1012.5   |
| Extract all user IDs                        | jsonpath-ng | 22.16    | 22.56       | 22.64    | 290.44    | 2000    | 1012.6   |
| Extract all user names                      | jmespath    | 0.66     | 0.66        | 0.68     | 9920.50   | 2000    | 2068.3   |
| Extract all user names                      | strata      | 2.52     | 2.60        | 2.68     | 2524.16   | 2000    | 2068.3   |
| Extract all user names                      | jsonpath-ng | 22.11    | 22.53       | 25.50    | 290.81    | 2000    | 2068.3   |
| Extract nested timestamp field              | jmespath    | 1.43     | 1.57        | 1.57     | 4166.73   | 2000    | 1439.6   |
| Extract nested timestamp field              | strata      | 3.13     | 3.17        | 3.24     | 2065.74   | 2000    | 1439.6   |
| Extract nested timestamp field              | jsonpath-ng | 23.56    | 23.70       | 24.21    | 276.44    | 2000    | 1439.7   |
| Extract order item prices (double wildcard) | jmespath    | 50.59    | 51.22       | 51.94    | 127.91    | 2000    | 1781.4   |
| Extract order item prices (double wildcard) | strata      | 117.94   | 118.52      | 119.28   | 55.28     | 100999  | 1781.4   |
| Extract order item prices (double wildcard) | jsonpath-ng | 161.79   | 162.34      | 164.60   | 40.36     | 100999  | 1784.3   |
| Filter users by age (numeric predicate)     | strata      | 2.11     | 2.55        | 2.57     | 2568.02   | 1606    | 2178.1   |
| Filter users by age (numeric predicate)     | jmespath    | 2.64     | 2.69        | 2.70     | 2432.02   | 1606    | 2178.1   |
| Recursively find all prices                 | strata      | 143.11   | 144.07      | 145.67   | 45.48     | 100999  | 2349.6   |
| Recursively find all prices                 | jsonpath-ng | 601.72   | 606.22      | 609.08   | 10.81     | 100999  | 2351.5   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 284347.98 | 10      | 2436.5   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.04     | 193406.91 | 10      | 2436.5   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.05     | 175295.39 | 10      | 2436.5   |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 265082.41 | 1       | 4712.4   |
| Deep path navigation                        | jmespath    | 0.02     | 0.03        | 0.03     | 241088.57 | 1       | 4712.4   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 230151.42 | 1       | 4712.4   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.04     | 302874.27 | 0       | 4710.0   |
| Extract all user IDs                        | jmespath    | 0.99     | 1.11        | 1.13     | 5913.68   | 2000    | 4710.0   |
| Extract all user IDs                        | strata      | 23.21    | 23.27       | 23.55    | 281.52    | 2000    | 4709.6   |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 318841.92 | 0       | 4712.4   |
| Extract all user names                      | jmespath    | 1.04     | 1.11        | 1.13     | 5893.29   | 2000    | 4712.4   |
| Extract all user names                      | strata      | 23.21    | 23.44       | 23.46    | 279.48    | 2000    | 4712.4   |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 304055.25 | 0       | 4710.3   |
| Extract nested timestamp field              | jmespath    | 1.82     | 1.82        | 1.87     | 3603.25   | 2000    | 4710.3   |
| Extract nested timestamp field              | strata      | 22.64    | 23.38       | 23.40    | 280.19    | 2000    | 4710.3   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.03     | 298852.58 | 0       | 4712.2   |
| Extract order item prices (double wildcard) | jmespath    | 29.68    | 30.03       | 30.30    | 218.13    | 100999  | 4712.2   |
| Extract order item prices (double wildcard) | strata      | 151.94   | 152.74      | 152.92   | 42.88     | 100999  | 4711.4   |
| Filter users by age (numeric predicate)     | jmespath    | 2.95     | 3.01        | 3.05     | 2178.95   | 1606    | 4712.5   |
| Filter users by age (numeric predicate)     | strata      | 22.62    | 22.84       | 23.19    | 286.80    | 1606    | 4712.5   |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 354821.89 | 1       | 4692.4   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 266886.26 | 1       | 4692.4   |
| NDJSON first record id                      | strata      | 0.02     | 0.03        | 0.03     | 256436.85 | 1       | 4692.4   |
| Recursively find all prices                 | strata      | 161.05   | 162.52      | 162.98   | 40.30     | 100999  | 4712.5   |
| Recursively find all prices                 | jsonpath-ng | 607.24   | 611.39      | 612.80   | 10.71     | 100999  | 4715.2   |
| Slice first 10 users, extract IDs           | jmespath    | 0.03     | 0.03        | 0.03     | 243708.01 | 10      | 4714.2   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 198474.40 | 10      | 4714.0   |
| Slice first 10 users, extract IDs           | strata      | 0.04     | 0.04        | 0.05     | 160236.19 | 10      | 4714.2   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 338.72   | 351.49      | 353.27   | 149.12 | 2655.6   |
| strata_cursor_reparse | 740.23   | 741.58      | 767.34   | 70.68  | 4695.0   |
- Speedup: 2.11x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 365.26   | 365.71      | 366.21   | 161.18 | 4714.0   |
| strata_cursor_reparse | 830.53   | 836.16      | 839.19   | 70.50  | 4732.7   |
- Speedup: 2.29x

## Summary
- loads json: strata (36.61 ms)
- loads ndjson: msgspec (40.95 ms)
- dumps str: msgspec (8.08 ms)
- dumps bytes: orjson (6.97 ms)
- search: jmespath (0.02 ms)
- cursor reuse: strata_cursor_reuse (351.49 ms)
- Rule 13: strata not #1 in: loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/ndjson: 22.8% behind msgspec
- Strata gap in dumps/str: 268.4% behind msgspec
- Strata gap in dumps/bytes: 326.8% behind orjson
