# Strata Benchmark Results

Generated: 2026-02-12 01:09:17

## Environment

- Commit: 4b1934ea6b90104063201b6ada1fd68de1090565
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/medium/users.json, benchmarks/data/generated/medium/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users.json   | 18.217   | 26.050      | 26.636   | 2714.9   |
| msgspec       | users.json   | 19.399   | 27.308      | 28.245   | 2714.7   |
| ujson         | users.json   | 26.666   | 38.856      | 39.965   | 2733.3   |
| json (stdlib) | users.json   | 34.224   | 43.249      | 43.335   | 2714.7   |
| strata        | users.json   | 68.194   | 75.319      | 119.207  | 2693.3   |
| orjson        | users.ndjson | 21.177   | 23.124      | 23.862   | 2695.9   |
| msgspec       | users.ndjson | 21.893   | 23.719      | 24.602   | 2696.8   |
| ujson         | users.ndjson | 30.734   | 34.607      | 35.548   | 2714.5   |
| json (stdlib) | users.ndjson | 37.937   | 40.405      | 41.234   | 2699.3   |
| strata        | users.ndjson | 45.531   | 54.972      | 55.738   | 2728.7   |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.158    | 2000    |
|                                     | jmespath    | 0.424    | 2000    |
|                                     | jsonpath-ng | 5.909    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 3.675    | 100999  |
|                                     | jmespath    | 32.609   | 2000    |
|                                     | jsonpath-ng | 128.353  | 100999  |
| $..price                            | jmespath    | 33.290   | 2000    |
|                                     | query       | 37.787   | 100999  |
|                                     | jsonpath-ng | 492.501  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.135    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 40.095   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 18.57    | 26.43       | 27.13    | 247.84 | 2714.1   |
| msgspec       | 20.01    | 28.54       | 29.21    | 229.53 | 2714.9   |
| ujson         | 27.33    | 38.70       | 38.76    | 169.31 | 2732.7   |
| json (stdlib) | 35.57    | 43.31       | 44.84    | 151.27 | 2715.0   |
| strata        | 72.59    | 73.07       | 119.79   | 89.66  | 2694.5   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 22.47    | 23.55       | 24.02    | 278.12 | 143.4    |
| msgspec       | 22.59    | 24.55       | 24.67    | 266.84 | 143.2    |
| ujson         | 30.79    | 35.58       | 35.66    | 184.09 | 161.1    |
| json (stdlib) | 39.08    | 40.96       | 41.07    | 159.89 | 144.2    |
| strata        | 45.69    | 47.63       | 47.79    | 137.52 | 142.3    |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 595633.00 | 1       | 1244.9   |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 530327.45 | 1       | 1244.9   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 382578.93 | 1       | 1244.9   |
| Extract all user IDs                        | jmespath    | 0.48     | 0.55        | 0.61     | 12004.42  | 2000    | 553.5    |
| Extract all user IDs                        | strata      | 2.42     | 2.43        | 2.45     | 2692.14   | 2000    | 553.3    |
| Extract all user IDs                        | jsonpath-ng | 6.07     | 6.15        | 6.23     | 1065.46   | 2000    | 554.7    |
| Extract all user names                      | jmespath    | 0.43     | 0.48        | 0.53     | 13646.34  | 2000    | 1325.2   |
| Extract all user names                      | strata      | 2.17     | 2.17        | 2.17     | 3021.43   | 2000    | 1325.2   |
| Extract all user names                      | jsonpath-ng | 6.07     | 6.09        | 6.11     | 1075.97   | 2000    | 1325.2   |
| Extract nested timestamp field              | jmespath    | 1.13     | 1.19        | 1.26     | 5494.73   | 2000    | 985.9    |
| Extract nested timestamp field              | strata      | 2.38     | 2.39        | 2.40     | 2741.83   | 2000    | 985.8    |
| Extract nested timestamp field              | jsonpath-ng | 7.15     | 7.24        | 7.32     | 905.32    | 2000    | 986.5    |
| Extract order item prices (double wildcard) | jmespath    | 33.07    | 33.13       | 33.19    | 197.76    | 2000    | 1153.7   |
| Extract order item prices (double wildcard) | strata      | 116.82   | 116.83      | 116.84   | 56.08     | 100999  | 1151.7   |
| Extract order item prices (double wildcard) | jsonpath-ng | 132.62   | 133.60      | 134.58   | 49.04     | 100999  | 1156.3   |
| Filter users by age (numeric predicate)     | strata      | 1.89     | 1.89        | 1.90     | 3459.40   | 1606    | 1431.9   |
| Filter users by age (numeric predicate)     | jmespath    | 2.32     | 2.36        | 2.41     | 2771.80   | 1606    | 1431.9   |
| Recursively find all prices                 | strata      | 172.91   | 174.34      | 175.76   | 37.58     | 100999  | 1492.2   |
| Recursively find all prices                 | jsonpath-ng | 494.50   | 496.21      | 497.91   | 13.20     | 100999  | 1496.9   |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 477213.46 | 10      | 1554.7   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 269704.56 | 10      | 1554.7   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.04        | 0.06     | 149896.21 | 10      | 1554.7   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 365.41   | 366.27      | 367.13   | 143.10 | 1953.6   |
| strata_cursor_reparse | 746.64   | 762.63      | 778.62   | 68.73  | 3305.9   |
- Speedup (reuse vs reparse): 2.08x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.01        | 0.01     | 563410.95 | 1       | 213.9    |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.01     | 468518.49 | 0       | 213.9    |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.02        | 0.03     | 334447.55 | 1       | 213.9    |
| Extract all user IDs                        | jmespath            | 0.71     | 0.71        | 0.72     | 9185.24   | 2000    | 206.1    |
| Extract all user IDs                        | strata              | 5.48     | 5.49        | 5.50     | 1192.81   | 0       | 205.9    |
| Extract all user IDs                        | jsonpath-ng         | 5.97     | 6.17        | 6.37     | 1061.72   | 2000    | 206.1    |
| Extract all user IDs                        | strata_ndjson_full  | 21.37    | 21.40       | 21.43    | 306.07    | 0       | 206.0    |
| Extract all user IDs                        | strata_ndjson_fused | 21.51    | 21.52       | 21.54    | 304.29    | 0       | 206.0    |
| Extract all user names                      | jmespath            | 0.64     | 0.68        | 0.71     | 9690.03   | 2000    | 213.9    |
| Extract all user names                      | strata              | 5.46     | 5.48        | 5.50     | 1195.34   | 0       | 213.9    |
| Extract all user names                      | jsonpath-ng         | 6.25     | 6.36        | 6.47     | 1029.87   | 2000    | 213.9    |
| Extract all user names                      | strata_ndjson_fused | 21.33    | 21.41       | 21.49    | 305.90    | 0       | 213.9    |
| Extract all user names                      | strata_ndjson_full  | 21.44    | 21.48       | 21.53    | 304.88    | 0       | 213.9    |
| Extract nested timestamp field              | jmespath            | 1.31     | 1.34        | 1.37     | 4887.65   | 2000    | 206.4    |
| Extract nested timestamp field              | strata              | 5.49     | 5.59        | 5.69     | 1171.85   | 0       | 206.4    |
| Extract nested timestamp field              | jsonpath-ng         | 7.57     | 8.13        | 8.69     | 805.83    | 2000    | 206.4    |
| Extract order item prices (double wildcard) | strata              | 5.62     | 5.62        | 5.63     | 1164.49   | 0       | 206.4    |
| Extract order item prices (double wildcard) | jmespath            | 25.70    | 25.85       | 26.01    | 253.33    | 100999  | 209.3    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 129.82   | 130.80      | 131.77   | 50.07     | 100999  | 215.9    |
| Filter users by age (numeric predicate)     | strata              | 0.01     | 0.01        | 0.01     | 475611.97 | 0       | 214.0    |
| Filter users by age (numeric predicate)     | jmespath            | 2.53     | 2.53        | 2.53     | 2590.91   | 1606    | 214.0    |
| NDJSON root field (id)                      | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 465058.61 | 0       | 148.9    |
| NDJSON root field (id)                      | strata              | 6.79     | 6.86        | 6.92     | 955.28    | 2000    | 147.8    |
| NDJSON root field (id)                      | strata_ndjson_fused | 27.14    | 27.17       | 27.19    | 241.10    | 2000    | 148.9    |
| NDJSON root field (id)                      | strata_ndjson_full  | 27.21    | 27.28       | 27.35    | 240.12    | 2000    | 148.9    |
| Recursively find all prices                 | strata              | 188.69   | 190.47      | 192.25   | 34.39     | 1908    | 219.1    |
| Recursively find all prices                 | jsonpath-ng         | 504.03   | 505.60      | 507.18   | 12.95     | 100999  | 221.4    |
| Slice first 10 users, extract IDs           | strata              | 0.01     | 0.01        | 0.01     | 489675.30 | 0       | 221.4    |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.01        | 0.02     | 439706.81 | 10      | 220.4    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 287903.16 | 10      | 219.4    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 272.41   | 272.74      | 273.08   | 216.13 | 219.5    |
| strata_cursor_reparse | 703.10   | 713.84      | 724.59   | 82.58  | 282.9    |
- Speedup (reuse vs reparse): 2.62x
