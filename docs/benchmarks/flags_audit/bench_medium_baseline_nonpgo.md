# Strata Benchmark Results — MEDIUM-BASELINE-NONPGO
Generated: 2026-02-14 21:00:08

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
| orjson        | 33.62    | 44.57       | 44.89    | 147.01 | 204.5    |
| strata        | 40.24    | 44.94       | 46.13    | 145.78 | 184.6    |
| msgspec       | 34.52    | 46.58       | 47.20    | 140.66 | 202.3    |
| ujson         | 44.17    | 58.74       | 60.34    | 111.54 | 219.1    |
| json (stdlib) | 51.74    | 62.79       | 64.21    | 104.35 | 202.4    |

### NDJSON — users.ndjson (6.25 MB, 2000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 27.78    | 40.01       | 40.27    | 163.70 | 265.3    |
| msgspec       | 28.47    | 40.27       | 42.69    | 162.64 | 265.8    |
| strata        | 51.04    | 52.47       | 54.73    | 124.83 | 265.1    |
| ujson         | 36.58    | 52.74       | 54.32    | 124.19 | 281.8    |
| json (stdlib) | 48.52    | 59.21       | 60.46    | 110.61 | 270.9    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 7.50     | 7.93        | 8.27     | 732.66 | 5811025        | 359.4    |
| msgspec | 8.33     | 8.38        | 8.65     | 693.23 | 5811025        | 405.3    |
| ujson   | 27.86    | 28.08       | 28.53    | 206.92 | 5811025        | 406.7    |
| strata  | 31.48    | 31.74       | 32.42    | 210.83 | 6690995        | 358.0    |
| json    | 44.29    | 44.99       | 45.13    | 145.64 | 6551664        | 456.8    |

### dumps (bytes) — users.json (6.25 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 6.89     | 7.22        | 7.32     | 804.36 | 5811025        | 505.4    |
| msgspec | 7.90     | 7.97        | 8.15     | 728.72 | 5811025        | 550.0    |
| ujson   | 27.57    | 27.96       | 28.75    | 207.85 | 5811025        | 552.0    |
| strata  | 31.17    | 31.23       | 31.80    | 214.24 | 6690995        | 508.1    |
| json    | 43.93    | 45.24       | 45.47    | 144.82 | 6551664        | 602.1    |

## Search Benchmarks (query)
### JSON — users.json (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 314485.09 | 1       | 1936.5   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 290114.85 | 1       | 1936.5   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.06     | 195811.62 | 1       | 1936.5   |
| Extract all user IDs                        | jmespath    | 0.88     | 0.92        | 1.10     | 7150.20   | 2000    | 1005.3   |
| Extract all user IDs                        | strata      | 2.71     | 2.99        | 3.06     | 2192.08   | 2000    | 1005.3   |
| Extract all user IDs                        | jsonpath-ng | 21.00    | 21.17       | 22.35    | 309.47    | 2000    | 1005.3   |
| Extract all user names                      | jmespath    | 0.68     | 0.70        | 0.77     | 9386.90   | 2000    | 2045.5   |
| Extract all user names                      | strata      | 2.98     | 3.05        | 3.10     | 2147.41   | 2000    | 2045.5   |
| Extract all user names                      | jsonpath-ng | 21.47    | 22.00       | 22.34    | 297.80    | 2000    | 2045.5   |
| Extract nested timestamp field              | jmespath    | 1.39     | 1.52        | 1.59     | 4302.64   | 2000    | 1430.9   |
| Extract nested timestamp field              | strata      | 3.09     | 3.14        | 3.27     | 2087.18   | 2000    | 1430.8   |
| Extract nested timestamp field              | jsonpath-ng | 23.00    | 23.89       | 23.93    | 274.20    | 2000    | 1430.9   |
| Extract order item prices (double wildcard) | jmespath    | 51.82    | 51.96       | 52.20    | 126.09    | 2000    | 1687.2   |
| Extract order item prices (double wildcard) | strata      | 135.43   | 137.28      | 138.58   | 47.72     | 100999  | 1687.2   |
| Extract order item prices (double wildcard) | jsonpath-ng | 166.39   | 166.59      | 166.84   | 39.33     | 100999  | 1690.6   |
| Filter users by age (numeric predicate)     | strata      | 2.52     | 2.53        | 2.59     | 2589.72   | 1606    | 2173.0   |
| Filter users by age (numeric predicate)     | jmespath    | 2.79     | 2.81        | 2.97     | 2334.70   | 1606    | 2173.0   |
| Recursively find all prices                 | strata      | 129.11   | 131.92      | 133.16   | 49.66     | 100999  | 2284.0   |
| Recursively find all prices                 | jsonpath-ng | 610.71   | 614.78      | 616.39   | 10.66     | 100999  | 2286.0   |
| Slice first 10 users, extract IDs           | jmespath    | 0.03     | 0.03        | 0.03     | 245298.06 | 10      | 2360.2   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.04     | 200559.20 | 10      | 2360.2   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 152364.36 | 10      | 2360.2   |

### NDJSON — users.ndjson (6.25 MB, 2000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 302873.86 | 1       | 4430.6   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 285276.14 | 1       | 4430.6   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 275775.13 | 1       | 4430.6   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 269621.73 | 0       | 4428.2   |
| Extract all user IDs                        | jmespath    | 1.08     | 1.10        | 1.11     | 5938.48   | 2000    | 4428.2   |
| Extract all user IDs                        | strata      | 22.21    | 22.22       | 22.29    | 294.71    | 2000    | 4427.8   |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 319495.56 | 0       | 4430.7   |
| Extract all user names                      | jmespath    | 0.96     | 1.08        | 1.13     | 6085.15   | 2000    | 4430.7   |
| Extract all user names                      | strata      | 23.16    | 23.30       | 23.36    | 281.11    | 2000    | 4430.7   |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 299988.75 | 0       | 4428.2   |
| Extract nested timestamp field              | jmespath    | 1.68     | 1.77        | 1.86     | 3692.55   | 2000    | 4428.2   |
| Extract nested timestamp field              | strata      | 23.49    | 23.89       | 23.94    | 274.12    | 2000    | 4428.2   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.03     | 299412.42 | 0       | 4430.6   |
| Extract order item prices (double wildcard) | jmespath    | 29.68    | 30.35       | 30.78    | 215.78    | 100999  | 4430.6   |
| Extract order item prices (double wildcard) | strata      | 143.19   | 143.29      | 144.14   | 45.71     | 100999  | 4430.0   |
| Filter users by age (numeric predicate)     | jmespath    | 2.86     | 2.87        | 2.97     | 2281.61   | 1606    | 4430.7   |
| Filter users by age (numeric predicate)     | strata      | 22.56    | 22.77       | 23.18    | 287.64    | 1606    | 4430.7   |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 320135.94 | 1       | 4411.9   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 262869.44 | 1       | 4411.9   |
| NDJSON first record id                      | strata      | 0.02     | 0.03        | 0.03     | 195267.28 | 1       | 4411.9   |
| Recursively find all prices                 | strata      | 149.72   | 150.44      | 150.96   | 43.54     | 100999  | 4430.7   |
| Recursively find all prices                 | jsonpath-ng | 613.31   | 614.58      | 615.28   | 10.66     | 100999  | 4433.8   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 243708.01 | 10      | 4432.9   |
| Slice first 10 users, extract IDs           | strata      | 0.04     | 0.04        | 0.05     | 171605.15 | 10      | 4432.9   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.04     | 0.05        | 0.06     | 121666.12 | 10      | 4432.7   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 344.42   | 353.45      | 364.79   | 148.29 | 2625.8   |
| strata_cursor_reparse | 803.57   | 804.21      | 848.67   | 65.17  | 4405.5   |
- Speedup: 2.28x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 350.29   | 354.02      | 371.00   | 166.51 | 4432.8   |
| strata_cursor_reparse | 883.66   | 884.43      | 894.78   | 66.65  | 4453.3   |
- Speedup: 2.50x

## Summary
- loads json: orjson (44.57 ms)
- loads ndjson: orjson (40.01 ms)
- dumps str: orjson (7.93 ms)
- dumps bytes: orjson (7.22 ms)
- search: jmespath (0.02 ms)
- cursor reuse: strata_cursor_reuse (353.45 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 0.8% behind orjson
- Strata gap in loads/ndjson: 31.1% behind orjson
- Strata gap in dumps/str: 300.1% behind orjson
- Strata gap in dumps/bytes: 332.3% behind orjson
