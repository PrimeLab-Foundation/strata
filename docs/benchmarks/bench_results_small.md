# Strata Benchmark Results — SMALL
Generated: 2026-02-19 00:23:17

## Environment
- Commit: 66621b992fb1ccdd3a41329f7022bcf24ff7d7ae
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
| orjson        | 3.10     | 4.90        | 5.25     | 205.84 | 371.0    |
| msgspec       | 3.34     | 5.01        | 5.06     | 201.22 | 373.1    |
| ujson         | 4.79     | 7.39        | 7.64     | 136.45 | 374.2    |
| json (stdlib) | 6.33     | 7.79        | 8.14     | 129.45 | 374.2    |
| strata        | 12.39    | 13.06       | 18.80    | 77.17  | 366.8    |

### NDJSON — users.ndjson (0.96 MB, 1000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 3.69     | 4.64        | 4.84     | 216.98 | 52.4     |
| orjson        | 3.25     | 4.67        | 4.93     | 215.73 | 51.3     |
| ujson         | 4.74     | 6.95        | 7.23     | 144.97 | 54.5     |
| strata        | 7.75     | 8.49        | 8.66     | 118.65 | 51.2     |
| json (stdlib) | 6.29     | 8.53        | 8.69     | 118.07 | 55.5     |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.76     | 0.82        | 0.88     | 1089.10 | 895062         | 44.5     |
| msgspec | 0.90     | 1.03        | 1.20     | 871.53  | 895062         | 44.7     |
| ujson   | 3.81     | 3.96        | 4.22     | 226.30  | 895062         | 45.5     |
| json    | 5.99     | 6.23        | 6.79     | 161.80  | 1008198        | 46.9     |
| strata  | 7.28     | 7.34        | 7.45     | 136.28  | 1000153        | 42.3     |

### dumps (bytes) — users.json (0.96 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s    | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------- | -------------- | -------- |
| orjson  | 0.76     | 0.81        | 0.84     | 1108.21 | 895062         | 43.3     |
| msgspec | 0.99     | 1.02        | 1.06     | 877.83  | 895062         | 44.4     |
| ujson   | 3.92     | 4.06        | 4.21     | 220.21  | 895062         | 46.2     |
| json    | 5.91     | 6.16        | 6.24     | 163.77  | 1008198        | 48.0     |
| strata  | 7.41     | 7.55        | 7.68     | 132.48  | 1000153        | 41.3     |

## Search Benchmarks (query)
### JSON — users.json (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.01     | 135183.92 | 1       | 94.7     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 97175.24  | 1       | 95.2     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 95266.09  | 1       | 94.7     |
| Extract all user IDs                        | strata      | 0.04     | 0.04        | 0.05     | 23769.30  | 1000    | 94.7     |
| Extract all user IDs                        | jmespath    | 0.19     | 0.20        | 0.21     | 5029.47   | 1000    | 94.8     |
| Extract all user IDs                        | jsonpath-ng | 1.60     | 1.61        | 1.77     | 626.97    | 1000    | 95.6     |
| Extract all user names                      | strata      | 0.04     | 0.07        | 0.10     | 13978.48  | 1000    | 94.8     |
| Extract all user names                      | jmespath    | 0.21     | 0.22        | 0.22     | 4637.18   | 1000    | 95.0     |
| Extract all user names                      | jsonpath-ng | 1.70     | 2.15        | 2.58     | 469.05    | 1000    | 95.7     |
| Extract nested timestamp field              | strata      | 0.08     | 0.10        | 0.12     | 10561.69  | 1000    | 95.1     |
| Extract nested timestamp field              | jmespath    | 0.63     | 0.63        | 0.64     | 1611.51   | 1000    | 95.1     |
| Extract nested timestamp field              | jsonpath-ng | 2.36     | 2.39        | 2.88     | 421.26    | 1000    | 96.0     |
| Extract order item prices (double wildcard) | strata      | 0.68     | 0.69        | 0.73     | 1470.21   | 12046   | 95.6     |
| Extract order item prices (double wildcard) | jmespath    | 5.70     | 5.89        | 6.48     | 171.18    | 1000    | 96.5     |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.45    | 18.79       | 19.44    | 53.66     | 12046   | 100.6    |
| Filter users by age (numeric predicate)     | strata      | 0.07     | 0.09        | 0.12     | 11855.29  | 794     | 94.6     |
| Filter users by age (numeric predicate)     | jmespath    | 1.17     | 1.22        | 1.35     | 828.37    | 794     | 94.8     |
| Recursively find all prices                 | strata      | 1.69     | 1.76        | 2.15     | 572.70    | 12046   | 97.0     |
| Recursively find all prices                 | jsonpath-ng | 75.77    | 77.12       | 77.57    | 13.07     | 12046   | 100.0    |
| Slice first 10 users, extract IDs           | strata      | 0.00     | 0.00        | 0.01     | 268854.93 | 10      | 95.1     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 75855.90  | 10      | 95.1     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 36440.41  | 10      | 95.6     |

### NDJSON — users.ndjson (0.96 MB, 1000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 164438.22 | 1       | 46.2     |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 129959.60 | 1       | 46.2     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.03     | 117909.53 | 1       | 46.4     |
| Extract all user IDs                        | jsonpath-ng | 0.00     | 0.01        | 0.02     | 107916.56 | 0       | 46.4     |
| Extract all user IDs                        | jmespath    | 0.27     | 0.27        | 0.29     | 3767.52   | 1000    | 46.3     |
| Extract all user IDs                        | strata      | 1.17     | 1.26        | 1.27     | 797.27    | 1000    | 45.8     |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.01        | 0.02     | 76493.39  | 0       | 47.6     |
| Extract all user names                      | jmespath    | 0.30     | 0.32        | 0.37     | 3180.18   | 1000    | 47.6     |
| Extract all user names                      | strata      | 1.33     | 1.44        | 1.52     | 700.14    | 1000    | 47.0     |
| Extract nested timestamp field              | jsonpath-ng | 0.01     | 0.02        | 0.03     | 61980.71  | 0       | 45.9     |
| Extract nested timestamp field              | jmespath    | 0.61     | 0.63        | 0.67     | 1602.84   | 1000    | 45.9     |
| Extract nested timestamp field              | strata      | 1.13     | 1.14        | 1.37     | 881.85    | 1000    | 45.9     |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.00     | 0.01        | 0.01     | 106019.53 | 0       | 48.9     |
| Extract order item prices (double wildcard) | strata      | 0.83     | 0.93        | 0.93     | 1085.28   | 12046   | 48.5     |
| Extract order item prices (double wildcard) | jmespath    | 3.85     | 3.94        | 4.11     | 255.34    | 12046   | 48.9     |
| Filter users by age (numeric predicate)     | strata      | 0.29     | 0.32        | 0.40     | 3121.85   | 794     | 47.0     |
| Filter users by age (numeric predicate)     | jmespath    | 1.11     | 1.12        | 1.13     | 897.40    | 794     | 47.0     |
| NDJSON first record id                      | jsonpath-ng | 0.00     | 0.01        | 0.01     | 162241.13 | 1       | 47.3     |
| NDJSON first record id                      | strata      | 0.00     | 0.01        | 0.07     | 139713.21 | 1       | 47.1     |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.01     | 115106.97 | 1       | 47.1     |
| Recursively find all prices                 | strata      | 2.07     | 2.09        | 2.12     | 481.29    | 12046   | 47.4     |
| Recursively find all prices                 | jsonpath-ng | 77.32    | 77.46       | 77.57    | 13.00     | 12046   | 50.7     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.03     | 106478.82 | 10      | 45.1     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.02        | 0.02     | 63609.14  | 10      | 45.0     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 56217.10  | 10      | 45.3     |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 10.84    | 11.92       | 12.93    | 676.89 | 120.7    |
| strata_cursor_reparse | 86.23    | 89.69       | 91.95    | 89.93  | 699.5    |
- Speedup: 7.53x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 14.43    | 14.49       | 14.73    | 625.76 | 36.7     |
| strata_cursor_reparse | 93.01    | 94.13       | 94.35    | 96.30  | 45.3     |
- Speedup: 6.50x

## Summary
- loads json: orjson (4.90 ms)
- loads ndjson: msgspec (4.64 ms)
- dumps str: orjson (0.82 ms)
- dumps bytes: orjson (0.81 ms)
- search: strata (0.00 ms)
- cursor reuse: strata_cursor_reuse (11.92 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 166.7% behind orjson
- Strata gap in loads/ndjson: 82.9% behind msgspec
- Strata gap in dumps/str: 793.0% behind orjson
- Strata gap in dumps/bytes: 834.7% behind orjson
