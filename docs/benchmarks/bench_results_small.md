# Strata Benchmark Results — SMALL
Generated: 2026-02-14 18:14:09

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
- json: benchmarks/data/generated/small/users.json (0.96 MB, 1000 records)
- ndjson: benchmarks/data/generated/small/users.ndjson (0.96 MB, 1000 records, 1000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (0.96 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 3.71     | 5.19        | 7.05     | 194.19 | 325.0    |
| msgspec       | 3.79     | 5.24        | 5.55     | 192.47 | 326.1    |
| ujson         | 4.97     | 7.20        | 7.33     | 140.06 | 328.2    |
| json (stdlib) | 6.00     | 7.56        | 8.11     | 133.38 | 328.2    |
| strata        | 10.32    | 11.45       | 16.63    | 88.04  | 321.3    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 3.51     | 4.59        | 4.60     | 219.34 | 328.2    |
| msgspec       | 3.52     | 4.71        | 4.94     | 213.94 | 328.2    |
| ujson         | 4.73     | 6.75        | 8.38     | 149.12 | 330.3    |
| strata        | 6.37     | 6.89        | 6.95     | 146.14 | 328.2    |
| json (stdlib) | 6.56     | 7.90        | 8.14     | 127.44 | 330.3    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.77     | 0.77        | 0.79     | 1156.66 | 895062         | 334.1    |
| msgspec | 0.96     | 0.99        | 1.01     | 901.11  | 895062         | 334.2    |
| strata  | 3.49     | 3.62        | 3.65     | 276.43  | 1000153        | 332.3    |
| ujson   | 3.88     | 3.94        | 4.00     | 227.09  | 895062         | 334.3    |
| json    | 5.71     | 5.78        | 6.00     | 174.34  | 1008198        | 334.4    |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.73     | 0.75        | 0.77     | 1190.04 | 895062         | 334.4    |
| msgspec | 0.91     | 0.94        | 0.95     | 953.08  | 895062         | 334.4    |
| strata  | 3.58     | 3.59        | 3.61     | 278.71  | 1000153        | 334.4    |
| ujson   | 3.83     | 3.86        | 3.87     | 231.89  | 895062         | 334.4    |
| json    | 5.78     | 5.79        | 5.83     | 174.07  | 1008198        | 334.4    |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s     | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 88632.80 | 1       | 506.9    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 77309.93 | 1       | 506.9    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 68352.43 | 1       | 506.9    |
| Extract all user IDs                        | jmespath    | 0.20     | 0.23        | 0.31     | 4447.93  | 1000    | 377.0    |
| Extract all user IDs                        | strata      | 1.27     | 1.29        | 1.31     | 779.41   | 1000    | 376.9    |
| Extract all user IDs                        | jsonpath-ng | 3.15     | 3.27        | 3.83     | 308.58   | 1000    | 377.1    |
| Extract all user names                      | jmespath    | 0.21     | 0.22        | 0.23     | 4658.59  | 1000    | 548.3    |
| Extract all user names                      | strata      | 1.32     | 1.34        | 1.38     | 751.01   | 1000    | 548.3    |
| Extract all user names                      | jsonpath-ng | 3.04     | 3.07        | 3.22     | 328.24   | 1000    | 548.3    |
| Extract nested timestamp field              | jmespath    | 0.54     | 0.56        | 0.64     | 1788.77  | 1000    | 437.6    |
| Extract nested timestamp field              | strata      | 1.44     | 1.75        | 1.81     | 574.53   | 1000    | 437.5    |
| Extract nested timestamp field              | jsonpath-ng | 3.50     | 3.58        | 3.72     | 281.74   | 1000    | 437.6    |
| Extract order item prices (double wildcard) | jmespath    | 7.11     | 7.31        | 7.78     | 138.01   | 1000    | 459.5    |
| Extract order item prices (double wildcard) | strata      | 14.93    | 15.15       | 15.70    | 66.53    | 12046   | 459.5    |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.46    | 18.95       | 19.62    | 53.20    | 12046   | 459.5    |
| Filter users by age (numeric predicate)     | strata      | 0.98     | 1.05        | 1.10     | 964.28   | 794     | 579.6    |
| Filter users by age (numeric predicate)     | jmespath    | 1.18     | 1.19        | 1.19     | 849.13   | 794     | 579.6    |
| Recursively find all prices                 | strata      | 14.23    | 14.58       | 15.17    | 69.17    | 12046   | 611.8    |
| Recursively find all prices                 | jsonpath-ng | 73.67    | 73.80       | 74.16    | 13.66    | 12046   | 611.8    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 62523.88 | 10      | 634.0    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 52036.02 | 10      | 634.0    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.03     | 43754.80 | 10      | 634.0    |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 90876.74  | 1       | 1201.6   |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 88543.92  | 1       | 1201.6   |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 70477.02  | 1       | 1201.6   |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 57831.15  | 0       | 1201.4   |
| Extract all user IDs                        | jmespath    | 0.31     | 0.31        | 0.33     | 3200.80   | 1000    | 1201.4   |
| Extract all user IDs                        | strata      | 3.84     | 3.93        | 3.93     | 256.51    | 1000    | 1201.4   |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.01        | 0.01     | 123323.98 | 0       | 1201.6   |
| Extract all user names                      | jmespath    | 0.28     | 0.29        | 0.31     | 3514.46   | 1000    | 1201.6   |
| Extract all user names                      | strata      | 3.67     | 3.90        | 3.91     | 258.10    | 1000    | 1201.6   |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.01        | 0.01     | 123323.98 | 0       | 1201.4   |
| Extract nested timestamp field              | jmespath    | 0.66     | 0.68        | 0.69     | 1480.70   | 1000    | 1201.4   |
| Extract nested timestamp field              | strata      | 3.93     | 3.93        | 4.13     | 256.12    | 1000    | 1201.4   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.01     | 0.01        | 0.02     | 93691.86  | 0       | 1201.6   |
| Extract order item prices (double wildcard) | jmespath    | 3.76     | 3.91        | 3.95     | 257.60    | 12046   | 1201.6   |
| Extract order item prices (double wildcard) | strata      | 14.86    | 15.58       | 15.70    | 64.65     | 12046   | 1201.4   |
| Filter users by age (numeric predicate)     | strata      | 1.20     | 1.23        | 1.25     | 817.91    | 794     | 1201.7   |
| Filter users by age (numeric predicate)     | jmespath    | 1.20     | 1.27        | 1.29     | 794.10    | 794     | 1201.7   |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.01     | 122083.20 | 1       | 1200.0   |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.01        | 0.02     | 106019.85 | 1       | 1200.0   |
| NDJSON first record id                      | strata      | 0.01     | 0.01        | 0.01     | 101143.58 | 1       | 1200.0   |
| Recursively find all prices                 | strata      | 16.10    | 16.51       | 16.97    | 60.99     | 12046   | 1201.7   |
| Recursively find all prices                 | jsonpath-ng | 73.54    | 73.76       | 74.01    | 13.65     | 12046   | 1201.7   |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 74150.61  | 10      | 1201.7   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 46397.06  | 10      | 1201.7   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 38430.52  | 10      | 1201.7   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 42.40    | 43.26       | 44.77    | 186.46 | 663.8    |
| strata_cursor_reparse | 112.00   | 114.77      | 115.20   | 70.28  | 1198.4   |
- Speedup: 2.65x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 48.16    | 48.23       | 49.52    | 187.95 | 1201.8   |
| strata_cursor_reparse | 122.04   | 122.55      | 122.75   | 73.97  | 1203.5   |
- Speedup: 2.54x

## Summary
- loads json: orjson (5.19 ms)
- loads ndjson: orjson (4.59 ms)
- dumps str: orjson (0.77 ms)
- dumps bytes: orjson (0.75 ms)
- search: jsonpath-ng (0.01 ms)
- cursor reuse: strata_cursor_reuse (43.26 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 120.6% behind orjson
- Strata gap in loads/ndjson: 50.1% behind orjson
- Strata gap in dumps/str: 367.6% behind orjson
- Strata gap in dumps/bytes: 377.1% behind orjson
