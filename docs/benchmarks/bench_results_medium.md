# Strata Benchmark Results

Generated: 2026-02-12 02:12:36

## Environment

- Commit: 334cd9c91bd5c8d0997a34e0a112219177b470f0
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
| orjson        | users.json   | 18.553   | 27.067      | 27.693   | 2715.0   |
| msgspec       | users.json   | 20.417   | 28.843      | 30.628   | 2714.9   |
| ujson         | users.json   | 27.954   | 39.422      | 41.645   | 2733.5   |
| json (stdlib) | users.json   | 34.732   | 44.546      | 45.263   | 2714.9   |
| strata        | users.json   | 76.781   | 82.602      | 128.050  | 2693.4   |
| orjson        | users.ndjson | 21.897   | 24.308      | 24.663   | 2695.9   |
| msgspec       | users.ndjson | 22.606   | 24.480      | 24.925   | 2696.8   |
| ujson         | users.ndjson | 31.151   | 35.270      | 35.295   | 2714.5   |
| json (stdlib) | users.ndjson | 40.058   | 41.302      | 42.933   | 2698.6   |
| strata        | users.ndjson | 51.148   | 61.823      | 62.141   | 2728.7   |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.091    | 2000    |
|                                     | jmespath    | 0.471    | 2000    |
|                                     | jsonpath-ng | 6.504    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 3.857    | 100999  |
|                                     | jmespath    | 33.730   | 2000    |
|                                     | jsonpath-ng | 137.482  | 100999  |
| $..price                            | jmespath    | 36.037   | 2000    |
|                                     | query       | 40.014   | 100999  |
|                                     | jsonpath-ng | 515.615  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.258    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 40.540   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 19.00    | 26.89       | 27.25    | 243.62 | 2714.2   |
| msgspec       | 19.99    | 32.83       | 34.49    | 199.59 | 2715.0   |
| ujson         | 31.30    | 45.47       | 46.50    | 144.10 | 2732.8   |
| json (stdlib) | 39.39    | 50.80       | 52.51    | 128.96 | 2715.0   |
| strata        | 78.01    | 81.16       | 125.55   | 80.73  | 2694.6   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 25.26    | 25.38       | 27.12    | 258.11 | 169.2    |
| orjson        | 24.76    | 27.02       | 27.93    | 242.38 | 169.5    |
| ujson         | 32.77    | 36.25       | 36.38    | 180.66 | 187.0    |
| json (stdlib) | 39.43    | 41.98       | 43.26    | 156.00 | 170.1    |
| strata        | 49.86    | 52.21       | 54.61    | 125.45 | 168.3    |
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
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 321885.76 | 1       | 1532.6   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 275859.55 | 1       | 1532.6   |
| Deep path navigation                        | jmespath    | 0.02     | 0.03        | 0.05     | 188425.94 | 1       | 1532.6   |
| Extract all user IDs                        | jmespath    | 0.79     | 0.90        | 1.01     | 7266.00   | 2000    | 553.7    |
| Extract all user IDs                        | strata      | 2.93     | 3.02        | 3.10     | 2170.05   | 2000    | 553.7    |
| Extract all user IDs                        | jsonpath-ng | 8.32     | 8.54        | 8.76     | 767.19    | 2000    | 554.8    |
| Extract all user names                      | jmespath    | 0.57     | 0.64        | 0.71     | 10213.36  | 2000    | 1707.8   |
| Extract all user names                      | strata      | 2.47     | 2.71        | 2.95     | 2417.57   | 2000    | 1707.8   |
| Extract all user names                      | jsonpath-ng | 7.96     | 8.11        | 8.26     | 807.93    | 2000    | 1707.8   |
| Extract nested timestamp field              | jmespath    | 1.17     | 1.23        | 1.28     | 5348.21   | 2000    | 986.1    |
| Extract nested timestamp field              | strata      | 2.41     | 2.42        | 2.44     | 2702.55   | 2000    | 986.1    |
| Extract nested timestamp field              | jsonpath-ng | 7.45     | 7.57        | 7.69     | 865.79    | 2000    | 986.7    |
| Extract order item prices (double wildcard) | jmespath    | 36.13    | 38.40       | 40.66    | 170.64    | 2000    | 1156.9   |
| Extract order item prices (double wildcard) | strata      | 132.15   | 135.41      | 138.68   | 48.38     | 100999  | 1154.9   |
| Extract order item prices (double wildcard) | jsonpath-ng | 146.05   | 150.49      | 154.93   | 43.54     | 100999  | 1161.2   |
| Filter users by age (numeric predicate)     | jmespath    | 2.34     | 2.36        | 2.37     | 2780.99   | 1606    | 1823.8   |
| Filter users by age (numeric predicate)     | strata      | 2.34     | 2.49        | 2.63     | 2635.42   | 1606    | 1823.8   |
| Recursively find all prices                 | strata      | 197.24   | 199.48      | 201.72   | 32.84     | 100999  | 2080.2   |
| Recursively find all prices                 | jsonpath-ng | 533.36   | 535.14      | 536.92   | 12.24     | 100999  | 2085.3   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 321877.93 | 10      | 2155.4   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 299792.43 | 10      | 2155.4   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 251387.54 | 10      | 2155.4   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 385.57   | 395.88      | 406.19   | 132.40 | 2353.4   |
| strata_cursor_reparse | 850.59   | 898.86      | 947.12   | 58.31  | 4293.4   |
- Speedup (reuse vs reparse): 2.27x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 474887.85 | 1       | 220.6    |
| Deep path navigation                        | jmespath            | 0.01     | 0.02        | 0.02     | 416406.25 | 1       | 220.6    |
| Deep path navigation                        | strata              | 0.02     | 0.02        | 0.02     | 327826.97 | 0       | 220.6    |
| Extract all user IDs                        | jmespath            | 0.74     | 0.80        | 0.87     | 8152.68   | 2000    | 213.7    |
| Extract all user IDs                        | strata              | 5.59     | 5.62        | 5.64     | 1166.08   | 0       | 213.5    |
| Extract all user IDs                        | jsonpath-ng         | 6.63     | 7.15        | 7.67     | 915.97    | 2000    | 213.7    |
| Extract all user IDs                        | strata_ndjson_full  | 22.80    | 22.89       | 22.99    | 286.12    | 0       | 213.7    |
| Extract all user IDs                        | strata_ndjson_fused | 23.64    | 23.91       | 24.17    | 273.97    | 0       | 213.7    |
| Extract all user names                      | jmespath            | 0.85     | 0.88        | 0.91     | 7427.14   | 2000    | 220.7    |
| Extract all user names                      | strata              | 5.56     | 5.63        | 5.69     | 1164.30   | 0       | 220.7    |
| Extract all user names                      | jsonpath-ng         | 8.55     | 8.68        | 8.80     | 754.86    | 2000    | 220.7    |
| Extract all user names                      | strata_ndjson_fused | 23.28    | 23.40       | 23.52    | 279.91    | 0       | 220.7    |
| Extract all user names                      | strata_ndjson_full  | 23.17    | 23.41       | 23.66    | 279.76    | 0       | 220.7    |
| Extract nested timestamp field              | jmespath            | 1.42     | 1.53        | 1.64     | 4291.81   | 2000    | 213.9    |
| Extract nested timestamp field              | strata              | 5.71     | 5.73        | 5.75     | 1142.76   | 0       | 213.9    |
| Extract nested timestamp field              | jsonpath-ng         | 7.70     | 7.74        | 7.78     | 846.13    | 2000    | 213.9    |
| Extract order item prices (double wildcard) | strata              | 5.72     | 5.73        | 5.74     | 1143.04   | 0       | 214.0    |
| Extract order item prices (double wildcard) | jmespath            | 25.80    | 25.92       | 26.05    | 252.64    | 100999  | 216.8    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 133.17   | 135.72      | 138.26   | 48.26     | 100999  | 222.5    |
| Filter users by age (numeric predicate)     | strata              | 0.02     | 0.02        | 0.02     | 313133.19 | 0       | 220.7    |
| Filter users by age (numeric predicate)     | jmespath            | 2.72     | 2.80        | 2.88     | 2342.26   | 1606    | 220.8    |
| NDJSON root field (id)                      | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 354035.39 | 0       | 156.7    |
| NDJSON root field (id)                      | strata              | 9.07     | 9.46        | 9.85     | 692.12    | 2000    | 155.5    |
| NDJSON root field (id)                      | strata_ndjson_fused | 28.87    | 29.14       | 29.41    | 224.75    | 2000    | 156.7    |
| NDJSON root field (id)                      | strata_ndjson_full  | 28.57    | 29.27       | 29.98    | 223.75    | 2000    | 156.6    |
| Recursively find all prices                 | strata              | 204.79   | 207.28      | 209.78   | 31.60     | 1908    | 224.9    |
| Recursively find all prices                 | jsonpath-ng         | 536.67   | 545.43      | 554.19   | 12.01     | 100999  | 228.5    |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.02        | 0.02     | 409353.36 | 10      | 228.0    |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.02        | 0.02     | 353643.44 | 0       | 229.0    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 282714.79 | 10      | 227.0    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 292.36   | 294.49      | 296.61   | 200.17 | 227.2    |
| strata_cursor_reparse | 806.69   | 807.44      | 808.18   | 73.00  | 291.0    |
- Speedup (reuse vs reparse): 2.74x
