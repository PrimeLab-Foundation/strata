# Strata Benchmark Results — SMALL
Generated: 2026-02-15 19:01:28

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
- json: benchmarks/data/generated/small/users.json (0.96 MB, 1000 records)
- ndjson: benchmarks/data/generated/small/users.ndjson (0.96 MB, 1000 records, 1000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (0.96 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 3.48     | 4.80        | 4.94     | 210.21 | 323.8    |
| orjson        | 3.69     | 5.26        | 5.74     | 191.65 | 322.7    |
| ujson         | 4.60     | 6.76        | 7.29     | 149.11 | 325.9    |
| json (stdlib) | 5.55     | 7.09        | 7.53     | 142.13 | 325.9    |
| strata        | 9.79     | 12.86       | 15.69    | 78.42  | 319.0    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 3.21     | 4.42        | 4.62     | 227.72 | 326.0    |
| msgspec       | 3.36     | 4.54        | 4.57     | 221.96 | 326.0    |
| ujson         | 5.62     | 6.92        | 8.42     | 145.63 | 328.0    |
| json (stdlib) | 6.03     | 7.23        | 7.27     | 139.27 | 328.0    |
| strata        | 6.89     | 7.41        | 7.60     | 135.97 | 325.9    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.72     | 0.73        | 0.73     | 1232.44 | 895062         | 330.0    |
| msgspec | 0.91     | 0.92        | 0.93     | 976.74  | 895062         | 330.1    |
| ujson   | 3.55     | 3.58        | 3.67     | 249.77  | 895062         | 330.1    |
| json    | 5.47     | 5.59        | 5.78     | 180.25  | 1008198        | 330.4    |
| strata  | 6.62     | 6.66        | 6.73     | 150.26  | 1000153        | 330.0    |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.69     | 0.70        | 0.71     | 1285.09 | 895062         | 330.4    |
| msgspec | 0.87     | 0.88        | 0.89     | 1019.09 | 895062         | 330.4    |
| ujson   | 3.53     | 3.58        | 3.60     | 249.91  | 895062         | 330.4    |
| json    | 5.35     | 5.36        | 5.40     | 188.22  | 1008198        | 330.5    |
| strata  | 6.62     | 6.68        | 6.86     | 149.66  | 1000153        | 330.4    |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.01     | 123447.77 | 1       | 483.0    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 102961.26 | 1       | 483.0    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 89953.41  | 1       | 483.0    |
| Extract all user IDs                        | strata      | 0.06     | 0.07        | 0.07     | 14158.29  | 1000    | 369.0    |
| Extract all user IDs                        | jmespath    | 0.20     | 0.21        | 0.21     | 4747.25   | 1000    | 369.0    |
| Extract all user IDs                        | jsonpath-ng | 2.57     | 2.57        | 2.78     | 392.05    | 1000    | 369.1    |
| Extract all user names                      | strata      | 0.08     | 0.10        | 0.11     | 10575.54  | 1000    | 499.8    |
| Extract all user names                      | jmespath    | 0.19     | 0.21        | 0.25     | 4905.07   | 1000    | 499.8    |
| Extract all user names                      | jsonpath-ng | 2.57     | 2.75        | 3.25     | 366.47    | 1000    | 499.8    |
| Extract nested timestamp field              | strata      | 0.12     | 0.13        | 0.15     | 8060.23   | 1000    | 428.9    |
| Extract nested timestamp field              | jmespath    | 0.54     | 0.54        | 0.56     | 1851.32   | 1000    | 429.0    |
| Extract nested timestamp field              | jsonpath-ng | 3.13     | 3.16        | 3.57     | 319.25    | 1000    | 429.0    |
| Extract order item prices (double wildcard) | strata      | 0.65     | 0.67        | 0.70     | 1515.80   | 12046   | 451.3    |
| Extract order item prices (double wildcard) | jmespath    | 6.42     | 6.53        | 7.31     | 154.39    | 1000    | 451.3    |
| Extract order item prices (double wildcard) | jsonpath-ng | 16.95    | 17.07       | 17.37    | 59.07     | 12046   | 451.5    |
| Filter users by age (numeric predicate)     | strata      | 0.12     | 0.13        | 0.15     | 7568.60   | 794     | 525.0    |
| Filter users by age (numeric predicate)     | jmespath    | 1.09     | 1.10        | 1.10     | 918.46    | 794     | 525.0    |
| Recursively find all prices                 | strata      | 1.98     | 2.01        | 2.10     | 500.62    | 12046   | 540.3    |
| Recursively find all prices                 | jsonpath-ng | 68.17    | 69.29       | 69.54    | 14.55     | 12046   | 540.3    |
| Slice first 10 users, extract IDs           | strata      | 0.00     | 0.01        | 0.01     | 172813.95 | 10      | 562.6    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 52603.44  | 10      | 562.6    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 34031.97  | 10      | 562.6    |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.01     | 187383.22 | 1       | 1014.1   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 76255.73  | 1       | 1014.1   |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 60890.38  | 1       | 1014.1   |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 113485.74 | 0       | 1014.0   |
| Extract all user IDs                        | jmespath    | 0.33     | 0.34        | 0.37     | 2981.69   | 1000    | 1014.0   |
| Extract all user IDs                        | strata      | 2.08     | 2.10        | 2.35     | 479.21    | 1000    | 1014.0   |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.01        | 0.01     | 142177.95 | 0       | 1014.1   |
| Extract all user names                      | jmespath    | 0.26     | 0.32        | 0.33     | 3191.51   | 1000    | 1014.1   |
| Extract all user names                      | strata      | 2.32     | 2.87        | 2.95     | 350.92    | 1000    | 1014.1   |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.02        | 0.02     | 54938.48  | 0       | 1014.0   |
| Extract nested timestamp field              | jmespath    | 0.62     | 0.64        | 0.66     | 1562.95   | 1000    | 1014.0   |
| Extract nested timestamp field              | strata      | 2.37     | 2.48        | 2.77     | 406.86    | 1000    | 1014.0   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.01     | 0.01        | 0.02     | 110376.52 | 0       | 1014.1   |
| Extract order item prices (double wildcard) | strata      | 0.69     | 0.69        | 0.73     | 1449.88   | 12046   | 1014.0   |
| Extract order item prices (double wildcard) | jmespath    | 3.51     | 3.53        | 3.76     | 285.39    | 12046   | 1014.1   |
| Filter users by age (numeric predicate)     | strata      | 0.20     | 0.22        | 0.22     | 4670.10   | 794     | 1014.2   |
| Filter users by age (numeric predicate)     | jmespath    | 1.14     | 1.21        | 1.26     | 834.74    | 794     | 1014.2   |
| NDJSON first record id                      | strata      | 0.01     | 0.01        | 0.01     | 165574.29 | 1       | 1012.6   |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.01        | 0.01     | 127218.29 | 1       | 1012.6   |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.01     | 114557.14 | 1       | 1012.6   |
| Recursively find all prices                 | strata      | 1.79     | 1.85        | 1.89     | 545.35    | 12046   | 1014.3   |
| Recursively find all prices                 | jsonpath-ng | 68.15    | 68.35       | 68.56    | 14.74     | 12046   | 1014.6   |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.02     | 117923.59 | 10      | 1014.6   |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 89194.55  | 10      | 1014.6   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 34384.39  | 10      | 1014.6   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 10.23    | 10.28       | 10.52    | 784.39 | 576.2    |
| strata_cursor_reparse | 73.04    | 74.55       | 77.28    | 108.19 | 1010.2   |
- Speedup: 7.25x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 14.67    | 14.87       | 15.24    | 609.69 | 1014.7   |
| strata_cursor_reparse | 86.91    | 87.51       | 89.12    | 103.58 | 1016.5   |
- Speedup: 5.89x

## Summary
- loads json: msgspec (4.80 ms)
- loads ndjson: orjson (4.42 ms)
- dumps str: orjson (0.73 ms)
- dumps bytes: orjson (0.70 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (10.28 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 168.1% behind msgspec
- Strata gap in loads/ndjson: 67.5% behind orjson
- Strata gap in dumps/str: 816.5% behind orjson
- Strata gap in dumps/bytes: 859.5% behind orjson
