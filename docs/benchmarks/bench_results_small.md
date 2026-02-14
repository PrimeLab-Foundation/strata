# Strata Benchmark Results — SMALL
Generated: 2026-02-14 19:35:47

## Environment
- Commit: ce2c30a88ba06496f61d061303b781b3665f5fe9
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
| msgspec       | 3.58     | 5.05        | 5.28     | 199.67 | 326.1    |
| orjson        | 3.52     | 5.12        | 5.52     | 197.09 | 325.1    |
| ujson         | 4.60     | 7.10        | 7.65     | 141.98 | 328.2    |
| json (stdlib) | 6.10     | 7.45        | 7.51     | 135.34 | 328.2    |
| strata        | 10.21    | 11.56       | 16.59    | 87.19  | 321.3    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 3.39     | 4.48        | 4.61     | 224.65 | 328.3    |
| msgspec       | 3.55     | 4.67        | 4.74     | 215.86 | 328.3    |
| ujson         | 4.62     | 6.64        | 7.14     | 151.78 | 330.3    |
| strata        | 6.69     | 7.20        | 7.30     | 139.84 | 328.3    |
| json (stdlib) | 6.14     | 8.02        | 9.25     | 125.61 | 330.4    |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.76     | 0.78        | 0.79     | 1153.93 | 895062         | 332.4    |
| msgspec | 0.96     | 0.97        | 0.98     | 923.93  | 895062         | 332.4    |
| strata  | 3.44     | 3.58        | 3.59     | 279.61  | 1000153        | 332.4    |
| ujson   | 3.75     | 3.78        | 3.88     | 236.86  | 895062         | 332.5    |
| json    | 5.87     | 5.93        | 6.02     | 169.88  | 1008198        | 332.8    |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.73     | 0.76        | 0.78     | 1171.36 | 895062         | 332.8    |
| msgspec | 0.92     | 1.02        | 1.20     | 877.44  | 895062         | 332.8    |
| strata  | 3.62     | 3.64        | 3.80     | 274.91  | 1000153        | 332.8    |
| ujson   | 3.74     | 3.82        | 3.96     | 234.59  | 895062         | 332.8    |
| json    | 5.81     | 5.82        | 5.85     | 173.28  | 1008198        | 332.8    |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s     | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 73100.28 | 1       | 489.0    |
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 47259.97 | 1       | 489.0    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 43993.46 | 1       | 489.0    |
| Extract all user IDs                        | jmespath    | 0.20     | 0.23        | 0.23     | 4433.28  | 1000    | 374.6    |
| Extract all user IDs                        | strata      | 1.29     | 1.29        | 1.31     | 779.33   | 1000    | 374.6    |
| Extract all user IDs                        | jsonpath-ng | 3.16     | 3.28        | 3.44     | 307.74   | 1000    | 374.7    |
| Extract all user names                      | jmespath    | 0.23     | 0.25        | 0.29     | 4076.95  | 1000    | 507.4    |
| Extract all user names                      | strata      | 1.35     | 1.42        | 1.43     | 709.96   | 1000    | 507.4    |
| Extract all user names                      | jsonpath-ng | 3.45     | 3.56        | 4.23     | 283.55   | 1000    | 507.4    |
| Extract nested timestamp field              | jmespath    | 0.56     | 0.57        | 0.61     | 1757.72  | 1000    | 434.7    |
| Extract nested timestamp field              | strata      | 1.33     | 1.34        | 1.65     | 752.90   | 1000    | 434.7    |
| Extract nested timestamp field              | jsonpath-ng | 3.85     | 3.93        | 4.20     | 256.38   | 1000    | 434.8    |
| Extract order item prices (double wildcard) | jmespath    | 7.21     | 7.53        | 7.76     | 133.82   | 1000    | 457.1    |
| Extract order item prices (double wildcard) | strata      | 14.80    | 15.16       | 15.19    | 66.50    | 12046   | 457.1    |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.41    | 19.00       | 19.35    | 53.07    | 12046   | 457.1    |
| Filter users by age (numeric predicate)     | jmespath    | 1.14     | 1.19        | 1.26     | 849.01   | 794     | 533.0    |
| Filter users by age (numeric predicate)     | strata      | 1.04     | 1.21        | 1.25     | 834.92   | 794     | 533.0    |
| Recursively find all prices                 | strata      | 14.42    | 14.93       | 15.79    | 67.51    | 12046   | 552.3    |
| Recursively find all prices                 | jsonpath-ng | 77.95    | 78.05       | 79.57    | 12.92    | 12046   | 552.3    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 73774.21 | 10      | 594.6    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 42749.24 | 10      | 594.6    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.03        | 0.03     | 37514.34 | 10      | 594.6    |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 54687.92  | 1       | 1251.4   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 49534.57  | 1       | 1251.4   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 41748.67  | 1       | 1251.4   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 48153.85  | 0       | 1251.3   |
| Extract all user IDs                        | jmespath    | 0.41     | 0.49        | 0.49     | 2073.65   | 1000    | 1251.3   |
| Extract all user IDs                        | strata      | 4.96     | 5.04        | 5.60     | 199.86    | 1000    | 1251.3   |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.01        | 0.01     | 116209.36 | 0       | 1251.4   |
| Extract all user names                      | jmespath    | 0.33     | 0.39        | 0.44     | 2606.20   | 1000    | 1251.4   |
| Extract all user names                      | strata      | 4.73     | 4.76        | 5.80     | 211.67    | 1000    | 1251.4   |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.03     | 41966.16  | 0       | 1251.4   |
| Extract nested timestamp field              | jmespath    | 0.71     | 0.82        | 0.90     | 1228.71   | 1000    | 1251.4   |
| Extract nested timestamp field              | strata      | 4.95     | 5.03        | 5.54     | 200.15    | 1000    | 1251.4   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.16     | 41605.52  | 0       | 1251.4   |
| Extract order item prices (double wildcard) | jmespath    | 4.07     | 4.15        | 4.49     | 242.43    | 12046   | 1251.4   |
| Extract order item prices (double wildcard) | strata      | 15.06    | 15.13       | 15.58    | 66.55     | 12046   | 1251.4   |
| Filter users by age (numeric predicate)     | strata      | 1.13     | 1.20        | 1.20     | 840.90    | 794     | 1251.4   |
| Filter users by age (numeric predicate)     | jmespath    | 1.25     | 1.40        | 1.40     | 721.61    | 794     | 1251.4   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 43870.84  | 1       | 1250.0   |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.03     | 43163.93  | 1       | 1250.0   |
| NDJSON first record id                      | strata      | 0.02     | 0.03        | 0.04     | 35032.61  | 1       | 1250.0   |
| Recursively find all prices                 | strata      | 16.99    | 17.22       | 17.46    | 58.48     | 12046   | 1251.5   |
| Recursively find all prices                 | jsonpath-ng | 76.27    | 77.70       | 78.93    | 12.96     | 12046   | 1251.5   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 45098.60  | 10      | 1251.5   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 32315.81  | 10      | 1251.5   |
| Slice first 10 users, extract IDs           | strata      | 0.04     | 0.04        | 0.04     | 25205.53  | 10      | 1251.5   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 45.50    | 46.35       | 49.86    | 174.01 | 665.5    |
| strata_cursor_reparse | 115.16   | 116.19      | 119.90   | 69.42  | 1247.8   |
- Speedup: 2.51x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 49.83    | 51.53       | 51.90    | 175.92 | 1251.5   |
| strata_cursor_reparse | 124.35   | 125.90      | 129.43   | 72.00  | 1253.3   |
- Speedup: 2.44x

## Summary
- loads json: msgspec (5.05 ms)
- loads ndjson: orjson (4.48 ms)
- dumps str: orjson (0.78 ms)
- dumps bytes: orjson (0.76 ms)
- search: jsonpath-ng (0.01 ms)
- cursor reuse: strata_cursor_reuse (46.35 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 129.0% behind msgspec
- Strata gap in loads/ndjson: 60.7% behind orjson
- Strata gap in dumps/str: 361.2% behind orjson
- Strata gap in dumps/bytes: 376.1% behind orjson
