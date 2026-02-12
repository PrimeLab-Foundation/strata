# Strata Benchmark Results

Generated: 2026-02-12 02:12:26

## Environment

- Commit: 334cd9c91bd5c8d0997a34e0a112219177b470f0
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| msgspec       | users.json   | 2.738    | 3.646       | 3.742    | 327.5    |
| orjson        | users.json   | 3.523    | 3.729       | 4.737    | 324.3    |
| ujson         | users.json   | 4.157    | 5.576       | 5.873    | 326.4    |
| json (stdlib) | users.json   | 5.246    | 5.986       | 6.348    | 328.5    |
| strata        | users.json   | 10.878   | 10.972      | 16.611   | 319.9    |
| orjson        | users.ndjson | 2.704    | 3.530       | 3.848    | 331.8    |
| msgspec       | users.ndjson | 2.754    | 3.736       | 3.972    | 333.9    |
| ujson         | users.ndjson | 4.065    | 5.536       | 5.901    | 333.9    |
| json (stdlib) | users.ndjson | 5.528    | 6.647       | 6.687    | 333.9    |
| strata        | users.ndjson | 7.124    | 8.054       | 8.081    | 329.9    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.028    | 1000    |
|                                     | jmespath    | 0.185    | 1000    |
|                                     | jsonpath-ng | 1.597    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.573    | 12046   |
|                                     | jmespath    | 5.646    | 1000    |
|                                     | jsonpath-ng | 18.305   | 12046   |
| $..price                            | query       | 5.835    | 12046   |
|                                     | jmespath    | 5.885    | 1000    |
|                                     | jsonpath-ng | 72.108   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.033    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.373    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 2.63     | 3.46        | 4.30     | 291.32 | 322.9    |
| msgspec       | 2.65     | 3.67        | 4.18     | 274.94 | 323.0    |
| ujson         | 3.73     | 5.58        | 6.03     | 180.78 | 326.0    |
| json (stdlib) | 5.22     | 5.98        | 6.37     | 168.49 | 325.0    |
| strata        | 10.70    | 10.98       | 16.82    | 91.84  | 319.1    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 2.82     | 3.65        | 3.85     | 275.68 | 36.5     |
| msgspec       | 2.72     | 3.70        | 4.20     | 272.23 | 36.6     |
| ujson         | 4.01     | 5.58        | 5.68     | 180.37 | 39.6     |
| json (stdlib) | 5.83     | 6.60        | 6.89     | 152.70 | 38.6     |
| strata        | 6.39     | 7.05        | 7.16     | 142.92 | 36.0     |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.00     | 264445.89 | 1       | 225.0    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 82301.89  | 1       | 225.0    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 71485.63  | 1       | 225.0    |
| Extract all user IDs                        | jmespath    | 0.19     | 0.20        | 0.21     | 5077.47   | 1000    | 97.8     |
| Extract all user IDs                        | strata      | 1.13     | 1.13        | 1.14     | 889.91    | 1000    | 97.7     |
| Extract all user IDs                        | jsonpath-ng | 1.53     | 1.61        | 1.69     | 627.01    | 1000    | 98.5     |
| Extract all user names                      | jmespath    | 0.21     | 0.21        | 0.21     | 4723.15   | 1000    | 250.2    |
| Extract all user names                      | strata      | 1.14     | 1.14        | 1.14     | 883.61    | 1000    | 250.2    |
| Extract all user names                      | jsonpath-ng | 1.67     | 1.69        | 1.72     | 595.20    | 1000    | 250.2    |
| Extract nested timestamp field              | jmespath    | 0.55     | 0.59        | 0.63     | 1719.01   | 1000    | 159.1    |
| Extract nested timestamp field              | strata      | 1.09     | 1.10        | 1.10     | 917.99    | 1000    | 159.1    |
| Extract nested timestamp field              | jsonpath-ng | 2.19     | 2.23        | 2.27     | 452.35    | 1000    | 159.5    |
| Extract order item prices (double wildcard) | jmespath    | 5.54     | 5.60        | 5.65     | 180.07    | 1000    | 192.1    |
| Extract order item prices (double wildcard) | strata      | 13.79    | 13.89       | 13.99    | 72.57     | 12046   | 191.5    |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.69    | 17.81       | 17.94    | 56.59     | 12046   | 195.3    |
| Filter users by age (numeric predicate)     | strata      | 0.87     | 0.91        | 0.95     | 1111.29   | 794     | 276.4    |
| Filter users by age (numeric predicate)     | jmespath    | 1.09     | 1.12        | 1.14     | 903.13    | 794     | 276.4    |
| Recursively find all prices                 | strata      | 33.62    | 34.19       | 34.76    | 29.49     | 12046   | 300.6    |
| Recursively find all prices                 | jsonpath-ng | 72.72    | 72.92       | 73.13    | 13.83     | 12046   | 302.3    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 147548.43 | 10      | 323.4    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 83294.67  | 10      | 323.4    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.03     | 44767.01  | 10      | 323.4    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 61.63    | 61.75       | 61.87    | 130.61 | 345.5    |
| strata_cursor_reparse | 137.05   | 141.84      | 146.64   | 56.86  | 812.0    |
- Speedup (reuse vs reparse): 2.30x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.01        | 0.01     | 179726.65 | 1       | 55.4     |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 163331.90 | 1       | 55.4     |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.01     | 160086.98 | 0       | 55.4     |
| Extract all user IDs                        | jmespath            | 0.30     | 0.30        | 0.30     | 3363.59   | 1000    | 53.9     |
| Extract all user IDs                        | jsonpath-ng         | 1.73     | 1.74        | 1.75     | 578.87    | 1000    | 53.9     |
| Extract all user IDs                        | strata              | 2.70     | 2.70        | 2.70     | 373.41    | 0       | 53.6     |
| Extract all user IDs                        | strata_ndjson_fused | 4.66     | 4.67        | 4.68     | 215.63    | 0       | 53.7     |
| Extract all user IDs                        | strata_ndjson_full  | 9.20     | 9.35        | 9.49     | 107.75    | 0       | 53.7     |
| Extract all user names                      | jmespath            | 0.21     | 0.24        | 0.26     | 4230.40   | 1000    | 55.4     |
| Extract all user names                      | jsonpath-ng         | 1.66     | 1.67        | 1.67     | 604.89    | 1000    | 55.4     |
| Extract all user names                      | strata              | 2.56     | 2.57        | 2.58     | 391.99    | 0       | 55.4     |
| Extract all user names                      | strata_ndjson_fused | 4.41     | 4.42        | 4.43     | 227.93    | 0       | 55.4     |
| Extract all user names                      | strata_ndjson_full  | 9.35     | 9.42        | 9.50     | 106.87    | 0       | 55.4     |
| Extract nested timestamp field              | jmespath            | 0.58     | 0.62        | 0.66     | 1623.62   | 1000    | 54.0     |
| Extract nested timestamp field              | jsonpath-ng         | 2.09     | 2.11        | 2.13     | 478.01    | 1000    | 54.0     |
| Extract nested timestamp field              | strata              | 2.61     | 2.62        | 2.62     | 385.04    | 0       | 53.9     |
| Extract order item prices (double wildcard) | strata              | 2.51     | 2.52        | 2.54     | 399.34    | 0       | 54.1     |
| Extract order item prices (double wildcard) | jmespath            | 3.64     | 3.71        | 3.78     | 271.37    | 12046   | 54.4     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 18.43    | 18.54       | 18.64    | 54.33     | 12046   | 57.4     |
| Filter users by age (numeric predicate)     | strata              | 0.01     | 0.01        | 0.01     | 142620.63 | 0       | 55.4     |
| Filter users by age (numeric predicate)     | jmespath            | 1.13     | 1.14        | 1.14     | 886.54    | 794     | 55.4     |
| NDJSON root field (id)                      | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 80574.96  | 0       | 45.2     |
| NDJSON root field (id)                      | strata              | 2.17     | 2.50        | 2.82     | 403.62    | 1000    | 44.8     |
| NDJSON root field (id)                      | strata_ndjson_fused | 3.35     | 3.55        | 3.75     | 283.82    | 1000    | 45.2     |
| NDJSON root field (id)                      | strata_ndjson_full  | 10.22    | 10.26       | 10.30    | 98.18     | 1000    | 45.2     |
| Recursively find all prices                 | strata              | 32.21    | 32.43       | 32.64    | 31.06     | 878     | 55.5     |
| Recursively find all prices                 | jsonpath-ng         | 77.06    | 78.66       | 80.26    | 12.80     | 12046   | 57.5     |
| Slice first 10 users, extract IDs           | strata              | 0.01     | 0.01        | 0.01     | 76374.38  | 0       | 57.6     |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.02        | 0.02     | 63196.06  | 10      | 57.6     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.02        | 0.03     | 44722.12  | 10      | 56.8     |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 54.15    | 54.16       | 54.16    | 167.38 | 56.8     |
| strata_cursor_reparse | 126.55   | 127.91      | 129.27   | 70.87  | 65.2     |
- Speedup (reuse vs reparse): 2.36x
