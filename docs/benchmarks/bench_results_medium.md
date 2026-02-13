# Strata Benchmark Results

Generated: 2026-02-13 03:06:25

## Environment

- Commit: 4ec9f875bd9c5821d060084e19b1070754ee66b0
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
| msgspec       | users.json   | 22.070   | 30.866      | 31.273   | 128.4    |
| orjson        | users.json   | 20.920   | 31.135      | 32.078   | 128.5    |
| ujson         | users.json   | 31.530   | 43.391      | 45.119   | 147.0    |
| json (stdlib) | users.json   | 39.340   | 49.281      | 51.138   | 128.4    |
| strata        | users.json   | 58.381   | 60.296      | 61.301   | 107.1    |
| msgspec       | users.ndjson | 24.934   | 27.731      | 28.729   | 150.2    |
| orjson        | users.ndjson | 25.721   | 28.975      | 29.408   | 150.3    |
| ujson         | users.ndjson | 38.770   | 39.177      | 42.029   | 167.9    |
| json (stdlib) | users.ndjson | 44.637   | 46.036      | 46.640   | 152.7    |
| strata        | users.ndjson | 56.585   | 67.533      | 68.186   | 145.3    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.152    | 2000    |
|                                     | jmespath    | 0.601    | 2000    |
|                                     | jsonpath-ng | 8.774    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 4.661    | 100999  |
|                                     | jmespath    | 38.234   | 2000    |
|                                     | jsonpath-ng | 151.240  | 100999  |
| $..price                            | jmespath    | 37.307   | 2000    |
|                                     | query       | 40.144   | 100999  |
|                                     | jsonpath-ng | 553.198  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.240    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 44.081   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 22.09    | 32.34       | 33.27    | 202.57 | 128.4    |
| orjson        | 21.63    | 32.50       | 34.84    | 201.58 | 127.7    |
| ujson         | 29.95    | 44.27       | 44.48    | 147.99 | 146.2    |
| json (stdlib) | 39.11    | 51.14       | 51.59    | 128.12 | 128.5    |
| strata        | 58.36    | 62.09       | 62.37    | 105.52 | 107.2    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 26.42    | 27.64       | 27.73    | 236.95 | 179.0    |
| msgspec       | 26.83    | 27.89       | 28.92    | 234.83 | 178.8    |
| ujson         | 36.55    | 42.64       | 42.99    | 153.62 | 196.7    |
| json (stdlib) | 47.08    | 49.16       | 49.87    | 133.24 | 179.8    |
| strata        | 52.23    | 54.61       | 55.52    | 119.95 | 177.8    |
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
| Deep path navigation                        | jmespath    | 0.02     | 0.03        | 0.03     | 257564.31 | 1       | 1434.6   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 251585.47 | 1       | 1434.6   |
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.04     | 226407.38 | 1       | 1434.6   |
| Extract all user IDs                        | jmespath    | 0.86     | 0.93        | 1.00     | 7034.72   | 2000    | 553.9    |
| Extract all user IDs                        | strata      | 2.97     | 3.05        | 3.12     | 2149.64   | 2000    | 553.7    |
| Extract all user IDs                        | jsonpath-ng | 8.63     | 9.05        | 9.48     | 723.69    | 2000    | 555.1    |
| Extract all user names                      | jmespath    | 0.58     | 0.65        | 0.71     | 10152.69  | 2000    | 1534.8   |
| Extract all user names                      | strata      | 2.94     | 2.99        | 3.04     | 2190.70   | 2000    | 1534.8   |
| Extract all user names                      | jsonpath-ng | 7.96     | 8.25        | 8.54     | 793.87    | 2000    | 1534.8   |
| Extract nested timestamp field              | jmespath    | 1.43     | 1.48        | 1.53     | 4433.79   | 2000    | 986.4    |
| Extract nested timestamp field              | strata      | 2.94     | 3.02        | 3.11     | 2167.21   | 2000    | 986.3    |
| Extract nested timestamp field              | jsonpath-ng | 10.01    | 10.28       | 10.56    | 637.15    | 2000    | 987.0    |
| Extract order item prices (double wildcard) | jmespath    | 36.99    | 37.12       | 37.25    | 176.49    | 2000    | 1233.4   |
| Extract order item prices (double wildcard) | strata      | 133.38   | 136.58      | 139.77   | 47.97     | 100999  | 1231.4   |
| Extract order item prices (double wildcard) | jsonpath-ng | 156.43   | 160.27      | 164.12   | 40.88     | 100999  | 1237.4   |
| Filter users by age (numeric predicate)     | strata      | 2.75     | 2.78        | 2.80     | 2359.76   | 1606    | 1669.9   |
| Filter users by age (numeric predicate)     | jmespath    | 2.85     | 2.87        | 2.90     | 2280.28   | 1606    | 1669.9   |
| Recursively find all prices                 | strata      | 154.97   | 155.24      | 155.50   | 42.20     | 100999  | 1743.1   |
| Recursively find all prices                 | jsonpath-ng | 581.78   | 581.95      | 582.12   | 11.26     | 100999  | 1747.8   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 257766.92 | 10      | 1829.7   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 222244.78 | 10      | 1829.7   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 186638.50 | 10      | 1829.7   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 372.96   | 375.32      | 377.68   | 139.65 | 2100.0   |
| strata_cursor_reparse | 883.02   | 894.57      | 906.11   | 58.59  | 4217.1   |
- Speedup (reuse vs reparse): 2.38x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.02        | 0.02     | 356443.81 | 1       | 225.8    |
| Deep path navigation                        | strata              | 0.02     | 0.02        | 0.02     | 305822.69 | 1       | 225.8    |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 278211.40 | 1       | 225.8    |
| Extract all user IDs                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 294637.91 | 0       | 213.4    |
| Extract all user IDs                        | jmespath            | 0.93     | 0.98        | 1.04     | 6670.98   | 2000    | 213.4    |
| Extract all user IDs                        | strata              | 9.32     | 9.55        | 9.79     | 685.54    | 2000    | 212.2    |
| Extract all user IDs                        | strata_ndjson_full  | 31.97    | 31.98       | 31.99    | 204.81    | 2000    | 213.2    |
| Extract all user IDs                        | strata_ndjson_fused | 32.24    | 32.60       | 32.97    | 200.88    | 2000    | 213.3    |
| Extract all user names                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 311584.13 | 0       | 225.8    |
| Extract all user names                      | jmespath            | 0.99     | 1.00        | 1.01     | 6568.67   | 2000    | 225.8    |
| Extract all user names                      | strata              | 9.29     | 9.47        | 9.64     | 691.98    | 2000    | 225.8    |
| Extract all user names                      | strata_ndjson_full  | 31.36    | 31.57       | 31.78    | 207.46    | 2000    | 225.8    |
| Extract all user names                      | strata_ndjson_fused | 31.60    | 31.94       | 32.28    | 205.08    | 2000    | 225.8    |
| Extract nested timestamp field              | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 314380.77 | 0       | 213.8    |
| Extract nested timestamp field              | jmespath            | 1.81     | 1.81        | 1.82     | 3611.57   | 2000    | 213.8    |
| Extract nested timestamp field              | strata              | 10.02    | 10.44       | 10.86    | 627.38    | 2000    | 213.8    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 289762.82 | 0       | 225.8    |
| Extract order item prices (double wildcard) | jmespath            | 29.32    | 29.87       | 30.43    | 219.25    | 100999  | 225.8    |
| Extract order item prices (double wildcard) | strata              | 131.84   | 132.04      | 132.25   | 49.60     | 100999  | 221.3    |
| Filter users by age (numeric predicate)     | jmespath            | 2.81     | 2.89        | 2.96     | 2268.28   | 1606    | 225.9    |
| Filter users by age (numeric predicate)     | strata              | 8.88     | 9.15        | 9.42     | 715.72    | 1606    | 225.8    |
| NDJSON first record id                      | jmespath            | 0.02     | 0.02        | 0.02     | 334815.08 | 1       | 153.7    |
| NDJSON first record id                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 276263.39 | 1       | 154.0    |
| NDJSON first record id                      | strata              | 0.02     | 0.03        | 0.03     | 234792.42 | 1       | 153.7    |
| Recursively find all prices                 | strata              | 155.38   | 155.42      | 155.47   | 42.14     | 100999  | 226.0    |
| Recursively find all prices                 | jsonpath-ng         | 561.68   | 562.30      | 562.91   | 11.65     | 100999  | 228.4    |
| Slice first 10 users, extract IDs           | jmespath            | 0.03     | 0.03        | 0.03     | 225850.08 | 10      | 227.4    |
| Slice first 10 users, extract IDs           | strata              | 0.03     | 0.04        | 0.04     | 179747.86 | 10      | 227.4    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.03     | 0.04        | 0.05     | 161552.29 | 10      | 227.2    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 352.34   | 356.57      | 360.81   | 165.32 | 227.3    |
| strata_cursor_reparse | 897.61   | 899.28      | 900.95   | 65.55  | 291.1    |
- Speedup (reuse vs reparse): 2.52x
