# Strata Benchmark Results

Generated: 2026-02-14 13:19:54

## Environment

- Commit: c97b616486f501631c86fcd91188be9d26c26404
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
| orjson        | users.json   | 18.620   | 25.903      | 26.872   | 130.0    |
| msgspec       | users.json   | 19.231   | 27.341      | 28.667   | 129.9    |
| ujson         | users.json   | 27.101   | 38.050      | 39.029   | 148.5    |
| json (stdlib) | users.json   | 33.836   | 41.653      | 41.747   | 129.9    |
| strata        | users.json   | 42.129   | 43.960      | 44.351   | 110.1    |
| msgspec       | users.ndjson | 23.273   | 23.409      | 24.753   | 153.2    |
| orjson        | users.ndjson | 22.322   | 24.010      | 24.465   | 152.3    |
| ujson         | users.ndjson | 31.413   | 33.367      | 33.858   | 170.9    |
| json (stdlib) | users.ndjson | 38.995   | 39.448      | 39.680   | 155.0    |
| strata        | users.ndjson | 48.704   | 59.221      | 59.402   | 147.3    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.147    | 2000    |
|                                     | jmespath    | 0.480    | 2000    |
|                                     | jsonpath-ng | 6.108    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 3.833    | 100999  |
|                                     | jmespath    | 32.260   | 2000    |
|                                     | jsonpath-ng | 125.438  | 100999  |
| $..price                            | jmespath    | 32.092   | 2000    |
|                                     | query       | 35.530   | 100999  |
|                                     | jsonpath-ng | 515.404  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.231    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 38.065   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 18.69    | 26.01       | 26.89    | 251.89 | 129.3    |
| msgspec       | 20.45    | 28.11       | 29.76    | 233.04 | 130.1    |
| ujson         | 27.28    | 39.28       | 40.84    | 166.81 | 147.9    |
| json (stdlib) | 34.77    | 42.08       | 43.20    | 155.68 | 130.1    |
| strata        | 41.79    | 43.58       | 44.14    | 150.34 | 109.4    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 23.65    | 23.78       | 24.16    | 275.44 | 145.1    |
| orjson        | 22.65    | 24.94       | 27.03    | 262.66 | 145.4    |
| ujson         | 31.80    | 34.03       | 34.37    | 192.49 | 163.0    |
| json (stdlib) | 38.94    | 39.02       | 39.57    | 167.84 | 146.2    |
| strata        | 44.97    | 47.54       | 50.35    | 137.78 | 144.2    |
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
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 507231.93 | 1       | 1573.8   |
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 387282.81 | 1       | 1573.8   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 312601.69 | 1       | 1573.8   |
| Extract all user IDs                        | jmespath    | 0.74     | 0.75        | 0.75     | 8769.89   | 2000    | 553.6    |
| Extract all user IDs                        | strata      | 2.76     | 2.79        | 2.83     | 2344.19   | 2000    | 553.6    |
| Extract all user IDs                        | jsonpath-ng | 7.46     | 7.61        | 7.76     | 861.07    | 2000    | 554.8    |
| Extract all user names                      | jmespath    | 0.51     | 0.52        | 0.52     | 12710.37  | 2000    | 1879.7   |
| Extract all user names                      | strata      | 2.16     | 2.18        | 2.21     | 2999.90   | 2000    | 1879.7   |
| Extract all user names                      | jsonpath-ng | 6.59     | 6.62        | 6.64     | 990.01    | 2000    | 1879.7   |
| Extract nested timestamp field              | jmespath    | 1.40     | 1.40        | 1.40     | 4677.88   | 2000    | 986.0    |
| Extract nested timestamp field              | strata      | 2.68     | 2.69        | 2.70     | 2433.17   | 2000    | 986.0    |
| Extract nested timestamp field              | jsonpath-ng | 8.52     | 8.91        | 9.30     | 735.02    | 2000    | 986.6    |
| Extract order item prices (double wildcard) | jmespath    | 32.15    | 32.55       | 32.95    | 201.27    | 2000    | 1229.7   |
| Extract order item prices (double wildcard) | strata      | 115.79   | 115.87      | 115.95   | 56.54     | 100999  | 1227.7   |
| Extract order item prices (double wildcard) | jsonpath-ng | 128.44   | 128.97      | 129.50   | 50.80     | 100999  | 1237.0   |
| Filter users by age (numeric predicate)     | strata      | 2.28     | 2.29        | 2.29     | 2863.28   | 1606    | 1922.9   |
| Filter users by age (numeric predicate)     | jmespath    | 2.44     | 2.50        | 2.56     | 2618.88   | 1606    | 1923.0   |
| Recursively find all prices                 | strata      | 112.15   | 113.04      | 113.93   | 57.96     | 100999  | 2040.2   |
| Recursively find all prices                 | jsonpath-ng | 469.76   | 470.59      | 471.43   | 13.92     | 100999  | 2043.8   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 294179.15 | 10      | 2155.9   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 287985.25 | 10      | 2155.9   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 244729.91 | 10      | 2155.9   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 301.44   | 306.39      | 311.33   | 171.07 | 2225.3   |
| strata_cursor_reparse | 686.84   | 701.96      | 717.07   | 74.67  | 3874.0   |
- Speedup (reuse vs reparse): 2.29x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 447212.70 | 1       | 217.6    |
| Deep path navigation                        | strata              | 0.01     | 0.02        | 0.02     | 407760.53 | 1       | 217.6    |
| Deep path navigation                        | jmespath            | 0.01     | 0.02        | 0.02     | 359713.14 | 1       | 217.6    |
| Extract all user IDs                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 568472.57 | 0       | 206.6    |
| Extract all user IDs                        | jmespath            | 0.78     | 0.82        | 0.87     | 7959.27   | 2000    | 206.6    |
| Extract all user IDs                        | strata              | 6.85     | 6.89        | 6.93     | 950.27    | 2000    | 205.4    |
| Extract all user IDs                        | strata_ndjson_full  | 28.99    | 29.05       | 29.11    | 225.48    | 2000    | 206.3    |
| Extract all user IDs                        | strata_ndjson_fused | 28.80    | 29.14       | 29.47    | 224.80    | 2000    | 206.4    |
| Extract all user names                      | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 505432.87 | 0       | 217.9    |
| Extract all user names                      | jmespath            | 0.73     | 0.75        | 0.77     | 8723.66   | 2000    | 217.9    |
| Extract all user names                      | strata              | 6.95     | 7.08        | 7.20     | 925.63    | 2000    | 217.6    |
| Extract all user names                      | strata_ndjson_full  | 29.06    | 29.24       | 29.43    | 223.96    | 2000    | 217.7    |
| Extract all user names                      | strata_ndjson_fused | 29.01    | 29.29       | 29.57    | 223.61    | 2000    | 217.9    |
| Extract nested timestamp field              | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 547721.44 | 0       | 207.2    |
| Extract nested timestamp field              | jmespath            | 1.43     | 1.47        | 1.51     | 4445.72   | 2000    | 207.2    |
| Extract nested timestamp field              | strata              | 7.49     | 7.61        | 7.73     | 860.95    | 2000    | 207.1    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 632572.21 | 0       | 217.6    |
| Extract order item prices (double wildcard) | jmespath            | 24.54    | 25.02       | 25.49    | 261.79    | 100999  | 217.6    |
| Extract order item prices (double wildcard) | strata              | 104.64   | 104.78      | 104.92   | 62.51     | 100999  | 214.6    |
| Filter users by age (numeric predicate)     | jmespath            | 2.54     | 2.63        | 2.71     | 2492.04   | 1606    | 218.0    |
| Filter users by age (numeric predicate)     | strata              | 6.92     | 6.99        | 7.06     | 937.10    | 1606    | 218.0    |
| NDJSON first record id                      | jmespath            | 0.01     | 0.02        | 0.02     | 381081.85 | 1       | 146.2    |
| NDJSON first record id                      | strata              | 0.02     | 0.02        | 0.02     | 377871.92 | 1       | 146.2    |
| NDJSON first record id                      | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 341715.01 | 1       | 146.6    |
| Recursively find all prices                 | strata              | 124.86   | 125.14      | 125.42   | 52.34     | 100999  | 218.4    |
| Recursively find all prices                 | jsonpath-ng         | 471.62   | 472.03      | 472.45   | 13.88     | 100999  | 224.0    |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.02        | 0.02     | 422571.90 | 10      | 223.0    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 242391.21 | 10      | 222.8    |
| Slice first 10 users, extract IDs           | strata              | 0.03     | 0.03        | 0.03     | 231166.94 | 10      | 223.0    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 300.95   | 302.50      | 304.05   | 194.87 | 222.9    |
| strata_cursor_reparse | 729.24   | 729.89      | 730.54   | 80.76  | 286.3    |
- Speedup (reuse vs reparse): 2.41x
