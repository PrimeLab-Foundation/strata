# Strata Benchmark Results — SMALL-FLAGS-NONPGO
Generated: 2026-02-14 21:14:28

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
| orjson        | 5.31     | 7.25        | 7.71     | 138.98 | 424.3    |
| msgspec       | 5.79     | 7.56        | 7.83     | 133.39 | 425.4    |
| ujson         | 6.32     | 9.55        | 10.37    | 105.62 | 427.5    |
| json (stdlib) | 8.45     | 10.21       | 10.29    | 98.77  | 427.5    |
| strata        | 12.16    | 15.40       | 18.70    | 65.45  | 420.6    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 5.14     | 6.71        | 6.86     | 150.16 | 427.5    |
| msgspec       | 4.64     | 6.84        | 7.10     | 147.28 | 427.5    |
| strata        | 7.49     | 8.12        | 8.44     | 124.08 | 427.5    |
| ujson         | 6.29     | 8.79        | 9.10     | 114.56 | 429.6    |
| json (stdlib) | 8.08     | 9.81        | 10.67    | 102.68 | 429.6    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 0.86     | 0.91        | 0.95     | 978.83 | 895062         | 431.6    |
| msgspec | 1.11     | 1.13        | 1.17     | 791.59 | 895062         | 431.6    |
| strata  | 3.86     | 4.05        | 4.08     | 246.81 | 1000153        | 431.6    |
| ujson   | 4.12     | 4.18        | 4.36     | 214.37 | 895062         | 431.7    |
| json    | 6.26     | 6.46        | 6.52     | 156.12 | 1008198        | 432.0    |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.85     | 0.86        | 0.97     | 1039.56 | 895062         | 432.0    |
| msgspec | 0.96     | 1.08        | 1.10     | 831.62  | 895062         | 432.0    |
| strata  | 3.88     | 3.98        | 4.02     | 251.26  | 1000153        | 432.0    |
| ujson   | 4.07     | 4.17        | 4.21     | 214.68  | 895062         | 432.0    |
| json    | 6.27     | 6.36        | 6.71     | 158.56  | 1008198        | 432.0    |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s     | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 53530.77 | 1       | 599.0    |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 49079.82 | 1       | 599.0    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 41431.70 | 1       | 599.0    |
| Extract all user IDs                        | jmespath    | 0.33     | 0.36        | 0.40     | 2831.02  | 1000    | 462.5    |
| Extract all user IDs                        | strata      | 1.40     | 1.42        | 1.63     | 707.53   | 1000    | 462.5    |
| Extract all user IDs                        | jsonpath-ng | 4.05     | 4.59        | 4.72     | 219.83   | 1000    | 462.5    |
| Extract all user names                      | jmespath    | 0.21     | 0.28        | 0.37     | 3656.75  | 1000    | 611.5    |
| Extract all user names                      | strata      | 1.49     | 1.50        | 1.56     | 673.99   | 1000    | 611.5    |
| Extract all user names                      | jsonpath-ng | 4.29     | 4.49        | 4.58     | 224.48   | 1000    | 611.5    |
| Extract nested timestamp field              | jmespath    | 0.73     | 0.73        | 0.75     | 1379.20  | 1000    | 523.0    |
| Extract nested timestamp field              | strata      | 1.51     | 1.52        | 1.63     | 662.94   | 1000    | 523.0    |
| Extract nested timestamp field              | jsonpath-ng | 4.98     | 5.65        | 5.79     | 178.52   | 1000    | 523.0    |
| Extract order item prices (double wildcard) | jmespath    | 8.66     | 8.82        | 9.11     | 114.32   | 1000    | 562.2    |
| Extract order item prices (double wildcard) | strata      | 16.71    | 16.74       | 16.86    | 60.22    | 12046   | 562.2    |
| Extract order item prices (double wildcard) | jsonpath-ng | 22.07    | 22.62       | 23.97    | 44.57    | 12046   | 562.3    |
| Filter users by age (numeric predicate)     | strata      | 1.18     | 1.22        | 1.25     | 829.45   | 794     | 643.1    |
| Filter users by age (numeric predicate)     | jmespath    | 1.33     | 1.33        | 1.34     | 757.12   | 794     | 643.1    |
| Recursively find all prices                 | strata      | 16.12    | 16.27       | 16.44    | 61.98    | 12046   | 677.6    |
| Recursively find all prices                 | jsonpath-ng | 83.10    | 83.89       | 84.00    | 12.02    | 12046   | 677.6    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 47444.64 | 10      | 703.2    |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 33842.37 | 10      | 703.2    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 31100.92 | 10      | 703.2    |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s     | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 49330.79 | 1       | 1330.0   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 48248.45 | 1       | 1330.0   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 44763.87 | 1       | 1330.0   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 54199.38 | 0       | 1329.9   |
| Extract all user IDs                        | jmespath    | 0.39     | 0.45        | 0.47     | 2255.53  | 1000    | 1329.9   |
| Extract all user IDs                        | strata      | 4.85     | 5.33        | 5.54     | 189.04   | 1000    | 1329.9   |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 48343.41 | 0       | 1330.0   |
| Extract all user names                      | jmespath    | 0.42     | 0.44        | 0.46     | 2278.28  | 1000    | 1330.0   |
| Extract all user names                      | strata      | 5.28     | 5.36        | 5.56     | 188.08   | 1000    | 1330.0   |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 45781.24 | 0       | 1329.9   |
| Extract nested timestamp field              | jmespath    | 0.85     | 0.85        | 0.89     | 1180.30  | 1000    | 1329.9   |
| Extract nested timestamp field              | strata      | 5.52     | 5.66        | 5.70     | 177.96   | 1000    | 1329.9   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.02     | 52210.18 | 0       | 1330.0   |
| Extract order item prices (double wildcard) | jmespath    | 4.51     | 4.75        | 4.82     | 211.83   | 12046   | 1330.0   |
| Extract order item prices (double wildcard) | strata      | 16.73    | 16.80       | 16.82    | 59.97    | 12046   | 1329.9   |
| Filter users by age (numeric predicate)     | strata      | 1.19     | 1.19        | 1.19     | 847.77   | 794     | 1330.0   |
| Filter users by age (numeric predicate)     | jmespath    | 1.37     | 1.49        | 1.50     | 677.44   | 794     | 1330.0   |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 53009.84 | 1       | 1328.6   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 48734.05 | 1       | 1328.6   |
| NDJSON first record id                      | strata      | 0.02     | 0.03        | 0.03     | 39628.07 | 1       | 1328.6   |
| Recursively find all prices                 | strata      | 17.96    | 18.04       | 18.10    | 55.84    | 12046   | 1330.0   |
| Recursively find all prices                 | jsonpath-ng | 83.36    | 83.77       | 84.28    | 12.02    | 12046   | 1330.0   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 43475.08 | 10      | 1330.1   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 30291.35 | 10      | 1330.1   |
| Slice first 10 users, extract IDs           | strata      | 0.04     | 0.04        | 0.04     | 24741.75 | 10      | 1330.1   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 48.87    | 49.10       | 49.95    | 164.27 | 743.2    |
| strata_cursor_reparse | 126.84   | 131.42      | 132.78   | 61.37  | 1327.4   |
- Speedup: 2.68x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 50.73    | 52.17       | 53.57    | 173.76 | 1330.1   |
| strata_cursor_reparse | 133.27   | 133.98      | 134.73   | 67.66  | 1331.9   |
- Speedup: 2.57x

## Summary
- loads json: orjson (7.25 ms)
- loads ndjson: orjson (6.71 ms)
- dumps str: orjson (0.91 ms)
- dumps bytes: orjson (0.86 ms)
- search: jsonpath-ng (0.02 ms)
- cursor reuse: strata_cursor_reuse (49.10 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 112.4% behind orjson
- Strata gap in loads/ndjson: 21.0% behind orjson
- Strata gap in dumps/str: 343.2% behind orjson
- Strata gap in dumps/bytes: 362.3% behind orjson
