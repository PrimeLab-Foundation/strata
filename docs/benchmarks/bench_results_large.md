# Strata Benchmark Results

Generated: 2026-02-12 12:31:20

## Environment

- Commit: 32da0b8032dd813bbd9c3eb030f05d1c87dd120f
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
| orjson        | users.json   | 127.267  | 181.800     | 194.860  | 9206.9   |
| msgspec       | users.json   | 134.169  | 185.383     | 196.189  | 8889.2   |
| ujson         | users.json   | 208.398  | 262.276     | 268.867  | 9011.6   |
| json (stdlib) | users.json   | 250.998  | 294.355     | 301.128  | 8933.1   |
| strata        | users.json   | 1689.955 | 1722.705    | 2061.698 | 9471.7   |
| orjson        | users.ndjson | 169.924  | 210.957     | 211.822  | 8707.2   |
| msgspec       | users.ndjson | 191.305  | 219.222     | 233.218  | 8560.1   |
| ujson         | users.ndjson | 248.011  | 309.733     | 319.983  | 8683.5   |
| json (stdlib) | users.ndjson | 298.745  | 333.522     | 343.795  | 8562.8   |
| strata        | users.ndjson | 359.846  | 417.767     | 440.636  | 8735.0   |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.608    | 4000    |
|                                     | jmespath    | 1.651    | 4000    |
|                                     | jsonpath-ng | 32.782   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 27.739   | 789913  |
|                                     | jmespath    | 215.431  | 4000    |
|                                     | jsonpath-ng | 1069.565 | 789913  |
| $..price                            | jmespath    | 212.855  | 4000    |
|                                     | query       | 259.618  | 789913  |
|                                     | jsonpath-ng | 3765.171 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.634    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 275.250  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 139.65   | 187.25      | 200.22   | 245.57 | 12398.8  |
| orjson        | 145.58   | 198.28      | 215.74   | 231.91 | 12411.1  |
| ujson         | 195.17   | 274.44      | 286.68   | 167.55 | 12465.9  |
| json (stdlib) | 243.63   | 298.03      | 316.55   | 154.29 | 12387.3  |
| strata        | 1428.94  | 1701.42     | 2457.12  | 27.03  | 11868.4  |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 165.74   | 199.86      | 270.69   | 230.06 | 1403.1   |
| strata        | 201.37   | 209.58      | 211.57   | 219.39 | 1403.8   |
| msgspec       | 182.60   | 221.51      | 223.72   | 207.57 | 1395.4   |
| ujson         | 248.07   | 289.26      | 296.53   | 158.95 | 1518.0   |
| json (stdlib) | 293.91   | 334.82      | 343.76   | 137.33 | 1398.3   |
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
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2161838.38 | 1       | 11422.2  |
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.03     | 1774255.27 | 1       | 11443.2  |
| Deep path navigation                        | jmespath    | 0.03     | 0.03        | 0.03     | 1654554.83 | 1       | 11422.2  |
| Extract all user IDs                        | jmespath    | 1.80     | 1.82        | 1.85     | 25218.55   | 4000    | 5558.2   |
| Extract all user IDs                        | strata      | 4.40     | 4.44        | 4.49     | 10346.23   | 4000    | 5557.9   |
| Extract all user IDs                        | jsonpath-ng | 38.43    | 39.61       | 40.78    | 1161.04    | 4000    | 5560.1   |
| Extract all user names                      | jmespath    | 1.30     | 1.30        | 1.31     | 35272.36   | 4000    | 11817.7  |
| Extract all user names                      | strata      | 4.57     | 4.89        | 5.20     | 9412.06    | 4000    | 11828.3  |
| Extract all user names                      | jsonpath-ng | 33.43    | 35.07       | 36.72    | 1311.02    | 4000    | 11817.8  |
| Extract nested timestamp field              | jmespath    | 2.67     | 2.72        | 2.78     | 16880.84   | 4000    | 10419.9  |
| Extract nested timestamp field              | strata      | 6.51     | 6.69        | 6.87     | 6877.35    | 4000    | 10419.7  |
| Extract nested timestamp field              | jsonpath-ng | 35.79    | 35.90       | 36.01    | 1280.94    | 4000    | 10421.1  |
| Extract order item prices (double wildcard) | jmespath    | 226.60   | 228.09      | 229.57   | 201.61     | 4000    | 11484.4  |
| Extract order item prices (double wildcard) | strata      | 820.88   | 851.82      | 882.77   | 53.98      | 789913  | 11493.3  |
| Extract order item prices (double wildcard) | jsonpath-ng | 1210.16  | 1236.58     | 1263.01  | 37.19      | 789913  | 11097.5  |
| Filter users by age (numeric predicate)     | strata      | 4.01     | 4.04        | 4.07     | 11393.07   | 3212    | 11804.5  |
| Filter users by age (numeric predicate)     | jmespath    | 4.65     | 4.82        | 4.99     | 9538.43    | 3212    | 11804.6  |
| Recursively find all prices                 | strata      | 1126.91  | 1160.50     | 1194.09  | 39.62      | 789913  | 12683.8  |
| Recursively find all prices                 | jsonpath-ng | 3812.17  | 3824.28     | 3836.40  | 12.02      | 789913  | 12401.2  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 2170308.34 | 10      | 13023.6  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2036149.15 | 10      | 13023.6  |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.03        | 0.03     | 1622962.81 | 10      | 13038.6  |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2895.51  | 2968.56     | 3041.62  | 123.92 | 14264.4  |
| strata_cursor_reparse | 9440.16  | 9589.65     | 9739.13  | 38.36  | 16988.9  |
- Speedup (reuse vs reparse): 3.23x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 3104092.67 | 1       | 2731.2   |
| Deep path navigation                        | jmespath            | 0.01     | 0.02        | 0.02     | 2584345.71 | 1       | 2731.2   |
| Deep path navigation                        | strata              | 0.02     | 0.03        | 0.03     | 1809072.11 | 0       | 2731.2   |
| Extract all user IDs                        | jmespath            | 2.00     | 2.06        | 2.12     | 22331.39   | 4000    | 2438.8   |
| Extract all user IDs                        | strata              | 11.22    | 12.17       | 13.12    | 3777.25    | 0       | 2435.6   |
| Extract all user IDs                        | jsonpath-ng         | 33.17    | 33.59       | 34.00    | 1368.91    | 4000    | 2438.8   |
| Extract all user IDs                        | strata_ndjson_full  | 51.00    | 51.20       | 51.39    | 898.09     | 0       | 2438.2   |
| Extract all user IDs                        | strata_ndjson_fused | 50.46    | 51.79       | 53.12    | 887.84     | 0       | 2438.8   |
| Extract all user names                      | jmespath            | 1.90     | 1.92        | 1.95     | 23900.14   | 4000    | 2739.2   |
| Extract all user names                      | strata              | 10.88    | 10.94       | 11.00    | 4203.26    | 0       | 2737.3   |
| Extract all user names                      | jsonpath-ng         | 33.03    | 33.13       | 33.24    | 1387.66    | 4000    | 2739.2   |
| Extract all user names                      | strata_ndjson_fused | 49.25    | 50.35       | 51.45    | 913.26     | 0       | 2739.2   |
| Extract all user names                      | strata_ndjson_full  | 53.89    | 54.93       | 55.98    | 837.01     | 0       | 2739.0   |
| Extract nested timestamp field              | jmespath            | 3.61     | 3.64        | 3.68     | 12625.63   | 4000    | 2443.9   |
| Extract nested timestamp field              | strata              | 11.54    | 11.69       | 11.84    | 3933.50    | 0       | 2443.9   |
| Extract nested timestamp field              | jsonpath-ng         | 38.74    | 39.20       | 39.66    | 1172.99    | 4000    | 2443.9   |
| Extract order item prices (double wildcard) | strata              | 11.90    | 11.91       | 11.91    | 3862.14    | 0       | 2459.1   |
| Extract order item prices (double wildcard) | jmespath            | 181.20   | 184.48      | 187.77   | 249.23     | 789913  | 2491.5   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 1029.96  | 1039.09     | 1048.22  | 44.25      | 789913  | 2498.4   |
| Filter users by age (numeric predicate)     | strata              | 0.03     | 0.03        | 0.03     | 1691195.98 | 0       | 2742.1   |
| Filter users by age (numeric predicate)     | jmespath            | 5.00     | 5.01        | 5.02     | 9179.12    | 3212    | 2742.1   |
| NDJSON root field (id)                      | jsonpath-ng         | 0.03     | 0.03        | 0.03     | 1682193.06 | 0       | 1484.8   |
| NDJSON root field (id)                      | strata              | 38.78    | 39.73       | 40.68    | 1157.32    | 4000    | 1471.4   |
| NDJSON root field (id)                      | strata_ndjson_fused | 84.41    | 87.30       | 90.19    | 526.66     | 4000    | 1484.8   |
| NDJSON root field (id)                      | strata_ndjson_full  | 87.92    | 88.48       | 89.05    | 519.63     | 4000    | 1484.5   |
| Recursively find all prices                 | strata              | 1196.36  | 1213.41     | 1230.46  | 37.89      | 3898    | 2798.0   |
| Recursively find all prices                 | jsonpath-ng         | 3653.28  | 3653.30     | 3653.33  | 12.59      | 789913  | 2821.1   |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.02     | 2818744.96 | 10      | 2812.2   |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.02        | 0.02     | 1907541.96 | 0       | 2827.2   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.03        | 0.04     | 1569718.06 | 10      | 2810.2   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 1684.84  | 1690.71     | 1696.58  | 244.76 | 2823.0   |
| strata_cursor_reparse | 4947.88  | 4991.76     | 5035.64  | 82.90  | 3932.1   |
- Speedup (reuse vs reparse): 2.95x
