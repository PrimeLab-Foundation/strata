# Strata Benchmark Results

Generated: 2026-02-13 01:36:01

## Environment

- Commit: 9b7be5525389f90d56aeaa6b54e263bfaac39521
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/large/users.json, benchmarks/data/generated/large/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users.json   | 121.990  | 175.706     | 192.914  | 13511.7  |
| msgspec       | users.json   | 130.546  | 183.039     | 185.928  | 13183.5  |
| ujson         | users.json   | 181.076  | 255.495     | 274.929  | 13305.9  |
| json (stdlib) | users.json   | 236.113  | 289.555     | 297.813  | 13218.1  |
| strata        | users.json   | 1476.199 | 1584.112    | 1757.488 | 13763.2  |
| orjson        | users.ndjson | 193.488  | 210.555     | 233.537  | 11533.2  |
| msgspec       | users.ndjson | 204.550  | 223.867     | 238.919  | 11407.0  |
| ujson         | users.ndjson | 261.224  | 302.191     | 326.180  | 11530.4  |
| json (stdlib) | users.ndjson | 330.090  | 349.463     | 353.074  | 11409.7  |
| strata        | users.ndjson | 355.642  | 434.622     | 480.101  | 11484.3  |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.594    | 4000    |
|                                     | jmespath    | 1.524    | 4000    |
|                                     | jsonpath-ng | 31.977   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 26.940   | 789913  |
|                                     | jmespath    | 212.308  | 4000    |
|                                     | jsonpath-ng | 1030.383 | 789913  |
| $..price                            | jmespath    | 212.630  | 4000    |
|                                     | query       | 257.957  | 789913  |
|                                     | jsonpath-ng | 3641.156 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.564    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 274.231  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 143.43   | 198.11      | 226.80   | 232.11 | 15156.8  |
| orjson        | 135.43   | 209.78      | 213.02   | 219.20 | 15243.5  |
| ujson         | 187.54   | 283.86      | 291.84   | 161.99 | 15185.8  |
| json (stdlib) | 260.52   | 318.37      | 326.29   | 144.43 | 15107.3  |
| strata        | 1406.76  | 1826.33     | 2679.36  | 25.18  | 14562.5  |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 185.13   | 208.29      | 219.03   | 220.75 | 1399.5   |
| orjson        | 188.76   | 225.23      | 235.81   | 204.14 | 1407.3   |
| strata        | 199.92   | 228.53      | 238.76   | 201.19 | 1408.0   |
| ujson         | 236.84   | 301.47      | 340.97   | 152.52 | 1522.2   |
| json (stdlib) | 321.89   | 330.05      | 330.13   | 139.31 | 1402.4   |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 2624548.30 | 1       | 12925.5  |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2365704.91 | 1       | 12925.5  |
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.03     | 1705742.22 | 1       | 12946.5  |
| Extract all user IDs                        | jmespath    | 1.77     | 1.80        | 1.82     | 25586.60   | 4000    | 5557.5   |
| Extract all user IDs                        | strata      | 4.98     | 5.47        | 5.96     | 8406.79    | 4000    | 5557.2   |
| Extract all user IDs                        | jsonpath-ng | 32.39    | 33.87       | 35.34    | 1357.78    | 4000    | 5559.5   |
| Extract all user names                      | jmespath    | 1.28     | 1.34        | 1.41     | 34269.61   | 4000    | 13107.0  |
| Extract all user names                      | strata      | 5.20     | 5.52        | 5.84     | 8329.73    | 4000    | 13118.4  |
| Extract all user names                      | jsonpath-ng | 31.73    | 32.26       | 32.80    | 1425.27    | 4000    | 13107.1  |
| Extract nested timestamp field              | jmespath    | 3.00     | 3.03        | 3.06     | 15167.28   | 4000    | 10419.9  |
| Extract nested timestamp field              | strata      | 5.51     | 5.55        | 5.59     | 8279.17    | 4000    | 10419.8  |
| Extract nested timestamp field              | jsonpath-ng | 35.26    | 37.86       | 40.46    | 1214.66    | 4000    | 10421.1  |
| Extract order item prices (double wildcard) | jmespath    | 243.88   | 246.28      | 248.67   | 186.71     | 4000    | 13303.7  |
| Extract order item prices (double wildcard) | strata      | 909.95   | 943.70      | 977.46   | 48.73      | 789913  | 13290.0  |
| Extract order item prices (double wildcard) | jsonpath-ng | 1162.92  | 1166.82     | 1170.71  | 39.41      | 789913  | 12669.7  |
| Filter users by age (numeric predicate)     | jmespath    | 5.43     | 5.43        | 5.44     | 8464.11    | 3212    | 13217.3  |
| Filter users by age (numeric predicate)     | strata      | 5.36     | 5.49        | 5.62     | 8377.40    | 3212    | 13217.3  |
| Recursively find all prices                 | strata      | 933.52   | 936.94      | 940.35   | 49.08      | 789913  | 13473.1  |
| Recursively find all prices                 | jsonpath-ng | 3921.63  | 3922.36     | 3923.10  | 11.72      | 789913  | 13121.7  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 1592498.44 | 10      | 15222.2  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.04        | 0.04     | 1202840.59 | 10      | 15237.2  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.05     | 1088364.53 | 10      | 15175.4  |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2718.44  | 2846.12     | 2973.79  | 129.25 | 15483.2  |
| strata_cursor_reparse | 10018.62 | 10053.62    | 10088.62 | 36.59  | 16946.8  |
- Speedup (reuse vs reparse): 3.53x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.02        | 0.02     | 2755319.54 | 1       | 2770.5   |
| Deep path navigation                        | strata              | 0.02     | 0.02        | 0.02     | 2587327.45 | 1       | 2786.5   |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2393700.00 | 1       | 2769.5   |
| Extract all user IDs                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2569255.34 | 0       | 2435.0   |
| Extract all user IDs                        | jmespath            | 1.99     | 2.00        | 2.01     | 22994.24   | 4000    | 2435.0   |
| Extract all user IDs                        | strata              | 33.36    | 34.31       | 35.26    | 1340.17    | 4000    | 2429.2   |
| Extract all user IDs                        | strata_ndjson_fused | 80.41    | 82.42       | 84.42    | 557.89     | 4000    | 2434.8   |
| Extract all user IDs                        | strata_ndjson_full  | 82.67    | 85.01       | 87.35    | 540.86     | 4000    | 2434.1   |
| Extract all user names                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2105910.18 | 0       | 2775.4   |
| Extract all user names                      | jmespath            | 1.91     | 1.92        | 1.93     | 23942.92   | 4000    | 2775.4   |
| Extract all user names                      | strata              | 33.54    | 35.20       | 36.86    | 1306.25    | 4000    | 2773.8   |
| Extract all user names                      | strata_ndjson_fused | 84.26    | 84.46       | 84.67    | 544.38     | 4000    | 2775.4   |
| Extract all user names                      | strata_ndjson_full  | 84.19    | 85.74       | 87.29    | 536.26     | 4000    | 2775.2   |
| Extract nested timestamp field              | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2227035.65 | 0       | 2454.7   |
| Extract nested timestamp field              | jmespath            | 3.13     | 3.19        | 3.25     | 14426.32   | 4000    | 2454.7   |
| Extract nested timestamp field              | strata              | 34.09    | 34.36       | 34.64    | 1338.06    | 4000    | 2454.7   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 2675163.04 | 0       | 2776.2   |
| Extract order item prices (double wildcard) | jmespath            | 177.36   | 179.18      | 180.99   | 256.62     | 789913  | 2772.2   |
| Extract order item prices (double wildcard) | strata              | 952.27   | 952.31      | 952.35   | 48.28      | 789913  | 2737.3   |
| Filter users by age (numeric predicate)     | strata              | 0.03     | 0.03        | 0.03     | 1643324.09 | 0       | 2777.0   |
| Filter users by age (numeric predicate)     | jmespath            | 5.18     | 5.84        | 6.50     | 7874.25    | 3212    | 2777.0   |
| NDJSON first record id                      | strata              | 0.01     | 0.02        | 0.02     | 2877486.94 | 1       | 1467.5   |
| NDJSON first record id                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 1981186.36 | 1       | 1467.9   |
| NDJSON first record id                      | jmespath            | 0.02     | 0.02        | 0.03     | 1979395.41 | 1       | 1467.5   |
| Recursively find all prices                 | strata              | 1067.91  | 1080.95     | 1094.00  | 42.54      | 789913  | 2802.2   |
| Recursively find all prices                 | jsonpath-ng         | 3786.43  | 3791.09     | 3795.74  | 12.13      | 789913  | 2798.8   |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.03        | 0.03     | 1833052.43 | 10      | 2802.1   |
| Slice first 10 users, extract IDs           | strata              | 0.03     | 0.03        | 0.03     | 1577527.34 | 10      | 2810.1   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.03     | 0.03        | 0.03     | 1471340.05 | 10      | 2800.1   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2406.80  | 2409.12     | 2411.43  | 171.77 | 2815.3   |
| strata_cursor_reparse | 5668.40  | 5720.81     | 5773.22  | 72.33  | 4021.4   |
- Speedup (reuse vs reparse): 2.37x
