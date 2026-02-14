# Strata Benchmark Results — SMALL-PGO
Generated: 2026-02-14 22:06:05

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
- json: benchmarks/data/generated/small/users.json (0.96 MB, 1000 records)
- ndjson: benchmarks/data/generated/small/users.ndjson (0.96 MB, 1000 records, 1000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (0.96 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 5.37     | 7.22        | 7.78     | 139.63 | 365.0    |
| msgspec       | 5.77     | 7.40        | 7.70     | 136.23 | 366.0    |
| ujson         | 6.78     | 9.46        | 9.94     | 106.58 | 368.1    |
| json (stdlib) | 8.30     | 9.87        | 10.32    | 102.19 | 368.1    |
| strata        | 11.02    | 11.31       | 16.92    | 89.18  | 361.2    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 4.60     | 6.57        | 7.06     | 153.23 | 368.2    |
| msgspec       | 4.47     | 6.59        | 7.02     | 152.77 | 368.2    |
| strata        | 6.65     | 7.28        | 7.36     | 138.30 | 368.2    |
| ujson         | 6.51     | 9.15        | 9.30     | 110.05 | 370.2    |
| json (stdlib) | 8.08     | 9.70        | 10.24    | 103.80 | 370.2    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.88     | 0.89        | 0.90     | 1004.75 | 895062         | 372.3    |
| msgspec | 1.06     | 1.08        | 1.17     | 827.29  | 895062         | 372.4    |
| strata  | 3.74     | 3.75        | 3.80     | 266.43  | 1000153        | 372.2    |
| ujson   | 4.07     | 4.18        | 4.21     | 214.30  | 895062         | 372.4    |
| json    | 6.22     | 6.46        | 6.56     | 156.18  | 1008198        | 372.4    |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.86     | 0.87        | 0.93     | 1025.42 | 895062         | 372.4    |
| msgspec | 1.06     | 1.08        | 1.13     | 828.95  | 895062         | 372.4    |
| strata  | 3.57     | 3.69        | 3.95     | 270.96  | 1000153        | 372.4    |
| ujson   | 4.10     | 4.26        | 4.44     | 209.94  | 895062         | 372.4    |
| json    | 6.42     | 6.52        | 6.62     | 154.73  | 1008198        | 372.4    |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s     | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 60040.35 | 1       | 537.1    |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 53298.76 | 1       | 537.1    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 44316.40 | 1       | 537.1    |
| Extract all user IDs                        | jmespath    | 0.29     | 0.31        | 0.34     | 3277.36  | 1000    | 406.6    |
| Extract all user IDs                        | strata      | 1.23     | 1.42        | 1.48     | 707.86   | 1000    | 406.5    |
| Extract all user IDs                        | jsonpath-ng | 4.47     | 4.60        | 4.76     | 219.22   | 1000    | 406.7    |
| Extract all user names                      | jmespath    | 0.27     | 0.27        | 0.34     | 3719.15  | 1000    | 563.3    |
| Extract all user names                      | strata      | 1.19     | 1.24        | 1.25     | 811.37   | 1000    | 563.3    |
| Extract all user names                      | jsonpath-ng | 4.48     | 4.71        | 4.78     | 214.22   | 1000    | 563.3    |
| Extract nested timestamp field              | jmespath    | 0.63     | 0.64        | 0.73     | 1566.34  | 1000    | 466.9    |
| Extract nested timestamp field              | strata      | 1.49     | 1.52        | 1.63     | 663.76   | 1000    | 466.9    |
| Extract nested timestamp field              | jsonpath-ng | 5.52     | 5.57        | 5.65     | 180.87   | 1000    | 466.9    |
| Extract order item prices (double wildcard) | jmespath    | 8.48     | 8.54        | 8.90     | 118.09   | 1000    | 507.1    |
| Extract order item prices (double wildcard) | strata      | 14.17    | 14.54       | 14.58    | 69.36    | 12046   | 507.1    |
| Extract order item prices (double wildcard) | jsonpath-ng | 21.94    | 22.30       | 22.48    | 45.21    | 12046   | 507.1    |
| Filter users by age (numeric predicate)     | strata      | 1.24     | 1.26        | 1.28     | 799.26   | 794     | 577.1    |
| Filter users by age (numeric predicate)     | jmespath    | 1.33     | 1.33        | 1.37     | 759.09   | 794     | 577.1    |
| Recursively find all prices                 | strata      | 15.89    | 15.93       | 16.38    | 63.28    | 12046   | 600.7    |
| Recursively find all prices                 | jsonpath-ng | 80.03    | 80.46       | 81.04    | 12.53    | 12046   | 600.7    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 47074.63 | 10      | 626.3    |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 32566.63 | 10      | 626.3    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.05     | 29544.27 | 10      | 626.3    |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s     | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | -------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.03     | 53009.84 | 1       | 1211.6   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.04     | 45436.28 | 1       | 1211.6   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 41039.31 | 1       | 1211.6   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 45696.07 | 0       | 1211.4   |
| Extract all user IDs                        | jmespath    | 0.35     | 0.49        | 0.54     | 2052.00  | 1000    | 1211.4   |
| Extract all user IDs                        | strata      | 5.18     | 5.21        | 5.42     | 193.43   | 1000    | 1211.4   |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 50676.06 | 0       | 1211.6   |
| Extract all user names                      | jmespath    | 0.46     | 0.46        | 0.49     | 2175.94  | 1000    | 1211.6   |
| Extract all user names                      | strata      | 4.64     | 5.29        | 5.49     | 190.54   | 1000    | 1211.6   |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 59246.35 | 0       | 1211.4   |
| Extract nested timestamp field              | jmespath    | 0.85     | 0.87        | 0.90     | 1157.85  | 1000    | 1211.4   |
| Extract nested timestamp field              | strata      | 5.42     | 5.48        | 5.60     | 183.82   | 1000    | 1211.4   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.03     | 52321.40 | 0       | 1211.6   |
| Extract order item prices (double wildcard) | jmespath    | 4.54     | 4.69        | 4.93     | 214.67   | 12046   | 1211.6   |
| Extract order item prices (double wildcard) | strata      | 15.94    | 16.15       | 16.32    | 62.36    | 12046   | 1211.4   |
| Filter users by age (numeric predicate)     | strata      | 1.22     | 1.31        | 1.38     | 766.48   | 794     | 1211.6   |
| Filter users by age (numeric predicate)     | jmespath    | 1.36     | 1.39        | 1.41     | 723.38   | 794     | 1211.6   |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 53124.50 | 1       | 1210.2   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 43165.76 | 1       | 1210.2   |
| NDJSON first record id                      | strata      | 0.02     | 0.02        | 0.04     | 41109.70 | 1       | 1210.2   |
| Recursively find all prices                 | strata      | 17.54    | 17.60       | 18.23    | 57.24    | 12046   | 1211.7   |
| Recursively find all prices                 | jsonpath-ng | 80.57    | 80.91       | 81.38    | 12.45    | 12046   | 1211.7   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 43710.92 | 10      | 1211.7   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 30065.29 | 10      | 1211.7   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 29335.83 | 10      | 1211.7   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 43.85    | 48.01       | 49.08    | 167.99 | 670.6    |
| strata_cursor_reparse | 115.28   | 122.12      | 127.13   | 66.05  | 1207.5   |
- Speedup: 2.54x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 53.68    | 53.88       | 54.52    | 168.23 | 1211.8   |
| strata_cursor_reparse | 128.68   | 128.91      | 130.16   | 70.32  | 1213.4   |
- Speedup: 2.39x

## Summary
- loads json: orjson (7.22 ms)
- loads ndjson: orjson (6.57 ms)
- dumps str: orjson (0.89 ms)
- dumps bytes: orjson (0.87 ms)
- search: jmespath (0.02 ms)
- cursor reuse: strata_cursor_reuse (48.01 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 56.6% behind orjson
- Strata gap in loads/ndjson: 10.8% behind orjson
- Strata gap in dumps/str: 321.4% behind orjson
- Strata gap in dumps/bytes: 322.9% behind orjson
