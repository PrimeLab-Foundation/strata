# Strata Benchmark Results — SMALL-BASELINE-NONPGO
Generated: 2026-02-14 20:59:15

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
| msgspec       | 5.66     | 7.58        | 7.89     | 132.94 | 403.8    |
| orjson        | 4.83     | 7.66        | 8.06     | 131.66 | 402.7    |
| ujson         | 6.86     | 9.88        | 9.94     | 102.03 | 405.8    |
| json (stdlib) | 8.10     | 9.94        | 10.68    | 101.42 | 405.8    |
| strata        | 12.13    | 13.33       | 16.96    | 75.62  | 399.0    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 5.08     | 6.79        | 6.92     | 148.39 | 406.0    |
| msgspec       | 5.25     | 6.95        | 7.34     | 144.93 | 406.0    |
| strata        | 7.76     | 8.41        | 8.48     | 119.82 | 405.9    |
| ujson         | 6.81     | 8.70        | 9.15     | 115.81 | 408.0    |
| json (stdlib) | 8.25     | 9.77        | 10.37    | 103.10 | 408.0    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 0.86     | 0.91        | 0.93     | 984.98 | 895062         | 410.1    |
| msgspec | 0.97     | 1.12        | 1.18     | 800.14 | 895062         | 410.2    |
| strata  | 3.86     | 4.05        | 4.08     | 246.94 | 1000153        | 410.0    |
| ujson   | 4.18     | 4.27        | 4.32     | 209.61 | 895062         | 410.2    |
| json    | 6.49     | 6.70        | 6.97     | 150.51 | 1008198        | 410.2    |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.85     | 0.87        | 0.90     | 1028.07 | 895062         | 410.2    |
| msgspec | 1.12     | 1.16        | 1.21     | 774.61  | 895062         | 410.2    |
| strata  | 3.72     | 4.13        | 4.20     | 242.02  | 1000153        | 410.2    |
| ujson   | 4.21     | 4.27        | 4.36     | 209.65  | 895062         | 410.2    |
| json    | 6.49     | 6.78        | 6.80     | 148.69  | 1008198        | 410.2    |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s     | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 54497.20 | 1       | 593.2    |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 47444.58 | 1       | 593.2    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 46982.50 | 1       | 593.2    |
| Extract all user IDs                        | jmespath    | 0.31     | 0.33        | 0.38     | 3042.47  | 1000    | 443.6    |
| Extract all user IDs                        | strata      | 1.42     | 1.46        | 1.46     | 691.22   | 1000    | 443.5    |
| Extract all user IDs                        | jsonpath-ng | 4.38     | 4.40        | 4.88     | 229.01   | 1000    | 443.7    |
| Extract all user names                      | jmespath    | 0.23     | 0.25        | 0.28     | 4055.78  | 1000    | 619.7    |
| Extract all user names                      | strata      | 1.50     | 1.51        | 1.51     | 668.49   | 1000    | 619.7    |
| Extract all user names                      | jsonpath-ng | 4.49     | 4.80        | 4.89     | 209.91   | 1000    | 619.7    |
| Extract nested timestamp field              | jmespath    | 0.74     | 0.74        | 0.78     | 1353.44  | 1000    | 504.2    |
| Extract nested timestamp field              | strata      | 1.40     | 1.63        | 1.71     | 619.44   | 1000    | 504.2    |
| Extract nested timestamp field              | jsonpath-ng | 4.72     | 5.41        | 5.48     | 186.43   | 1000    | 504.2    |
| Extract order item prices (double wildcard) | jmespath    | 8.97     | 9.01        | 9.12     | 111.88   | 1000    | 547.5    |
| Extract order item prices (double wildcard) | strata      | 16.53    | 16.71       | 16.80    | 60.33    | 12046   | 547.5    |
| Extract order item prices (double wildcard) | jsonpath-ng | 22.38    | 22.39       | 23.37    | 45.02    | 12046   | 547.5    |
| Filter users by age (numeric predicate)     | strata      | 1.06     | 1.08        | 1.11     | 933.19   | 794     | 650.6    |
| Filter users by age (numeric predicate)     | jmespath    | 1.43     | 1.45        | 1.47     | 694.85   | 794     | 650.6    |
| Recursively find all prices                 | strata      | 16.78    | 16.86       | 16.91    | 59.82    | 12046   | 678.9    |
| Recursively find all prices                 | jsonpath-ng | 81.68    | 82.24       | 82.95    | 12.26    | 12046   | 678.9    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 49686.99 | 10      | 697.4    |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 37168.59 | 10      | 697.4    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 27372.14 | 10      | 697.4    |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s     | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 50996.80 | 1       | 1324.4   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 46937.62 | 1       | 1324.4   |
| Deep path navigation                        | strata      | 0.03     | 0.03        | 0.03     | 36459.29 | 1       | 1324.4   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 46575.14 | 0       | 1324.3   |
| Extract all user IDs                        | jmespath    | 0.48     | 0.50        | 0.50     | 1999.71  | 1000    | 1324.3   |
| Extract all user IDs                        | strata      | 5.29     | 5.48        | 5.54     | 183.65   | 1000    | 1324.3   |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 55568.96 | 0       | 1324.5   |
| Extract all user names                      | jmespath    | 0.49     | 0.50        | 0.54     | 2010.35  | 1000    | 1324.5   |
| Extract all user names                      | strata      | 5.18     | 5.38        | 5.49     | 187.22   | 1000    | 1324.4   |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 46935.39 | 0       | 1324.3   |
| Extract nested timestamp field              | jmespath    | 0.70     | 0.83        | 0.92     | 1208.99  | 1000    | 1324.3   |
| Extract nested timestamp field              | strata      | 5.12     | 5.30        | 5.37     | 189.87   | 1000    | 1324.3   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.03     | 40558.43 | 0       | 1324.4   |
| Extract order item prices (double wildcard) | jmespath    | 4.36     | 4.95        | 5.14     | 203.27   | 12046   | 1324.4   |
| Extract order item prices (double wildcard) | strata      | 16.48    | 16.53       | 16.96    | 60.92    | 12046   | 1324.3   |
| Filter users by age (numeric predicate)     | strata      | 1.23     | 1.24        | 1.27     | 810.97   | 794     | 1324.5   |
| Filter users by age (numeric predicate)     | jmespath    | 1.30     | 1.37        | 1.40     | 736.16   | 794     | 1324.5   |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 53009.84 | 1       | 1322.9   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 51212.03 | 1       | 1322.9   |
| NDJSON first record id                      | strata      | 0.02     | 0.02        | 0.03     | 48637.54 | 1       | 1322.9   |
| Recursively find all prices                 | strata      | 17.67    | 18.16       | 18.27    | 55.45    | 12046   | 1324.5   |
| Recursively find all prices                 | jsonpath-ng | 82.22    | 82.64       | 82.74    | 12.19    | 12046   | 1324.5   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 42258.41 | 10      | 1324.5   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.04     | 31069.71 | 10      | 1324.5   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 27406.44 | 10      | 1324.5   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 46.46    | 49.14       | 49.36    | 164.13 | 746.8    |
| strata_cursor_reparse | 124.23   | 125.87      | 126.06   | 64.08  | 1321.8   |
- Speedup: 2.56x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 54.44    | 54.87       | 55.57    | 165.21 | 1324.5   |
| strata_cursor_reparse | 137.42   | 138.09      | 138.61   | 65.64  | 1326.3   |
- Speedup: 2.52x

## Summary
- loads json: msgspec (7.58 ms)
- loads ndjson: orjson (6.79 ms)
- dumps str: orjson (0.91 ms)
- dumps bytes: orjson (0.87 ms)
- search: jsonpath-ng (0.02 ms)
- cursor reuse: strata_cursor_reuse (49.14 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 75.8% behind msgspec
- Strata gap in loads/ndjson: 23.8% behind orjson
- Strata gap in dumps/str: 345.7% behind orjson
- Strata gap in dumps/bytes: 374.7% behind orjson
