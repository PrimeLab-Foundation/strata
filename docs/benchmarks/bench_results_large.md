# Strata Benchmark Results

Generated: 2026-02-14 00:47:13

## Environment

- Commit: 67a3e370df19c36bc7749c21bf64263ace907581
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
| orjson        | users.json   | 124.365  | 179.286     | 179.518  | 731.9    |
| msgspec       | users.json   | 135.235  | 187.261     | 190.399  | 724.5    |
| ujson         | users.json   | 187.749  | 258.369     | 259.669  | 846.9    |
| json (stdlib) | users.json   | 240.883  | 293.562     | 303.188  | 724.5    |
| strata        | users.json   | 482.788  | 494.178     | 520.272  | 578.9    |
| orjson        | users.ndjson | 171.375  | 208.504     | 214.595  | 740.4    |
| msgspec       | users.ndjson | 176.329  | 222.855     | 290.299  | 733.9    |
| ujson         | users.ndjson | 236.488  | 302.777     | 311.620  | 857.4    |
| json (stdlib) | users.ndjson | 339.604  | 347.328     | 353.475  | 736.6    |
| strata        | users.ndjson | 448.809  | 562.269     | 1013.129 | 689.4    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.592    | 4000    |
|                                     | jmespath    | 1.541    | 4000    |
|                                     | jsonpath-ng | 32.059   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 28.006   | 789913  |
|                                     | jmespath    | 213.004  | 4000    |
|                                     | jsonpath-ng | 1025.200 | 789913  |
| $..price                            | jmespath    | 216.396  | 4000    |
|                                     | query       | 259.590  | 789913  |
|                                     | jsonpath-ng | 3722.472 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.625    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 282.806  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 131.63   | 189.33      | 200.00   | 242.88 | 732.2    |
| msgspec       | 137.54   | 197.56      | 199.79   | 232.75 | 723.6    |
| ujson         | 192.96   | 262.02      | 267.59   | 175.49 | 846.2    |
| json (stdlib) | 248.21   | 307.81      | 318.60   | 149.39 | 723.7    |
| strata        | 581.12   | 596.97      | 604.13   | 77.03  | 579.2    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 195.66   | 220.05      | 229.03   | 208.95 | 1408.5   |
| msgspec       | 188.06   | 221.86      | 289.29   | 207.25 | 1400.8   |
| strata        | 222.49   | 227.82      | 230.05   | 201.83 | 1409.3   |
| json (stdlib) | 317.62   | 351.01      | 364.68   | 130.99 | 1403.6   |
| ujson         | 361.18   | 362.68      | 373.87   | 126.78 | 1523.4   |
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
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2335663.99 | 1       | 9313.1   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1767130.70 | 1       | 9212.2   |
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.03     | 1737943.92 | 1       | 9486.1   |
| Extract all user IDs                        | jmespath    | 1.69     | 1.84        | 2.00     | 24933.95   | 4000    | 5557.5   |
| Extract all user IDs                        | strata      | 5.03     | 5.15        | 5.26     | 8935.36    | 4000    | 5557.5   |
| Extract all user IDs                        | jsonpath-ng | 32.30    | 32.55       | 32.80    | 1412.72    | 4000    | 5559.4   |
| Extract all user names                      | jmespath    | 1.30     | 1.39        | 1.49     | 33034.55   | 4000    | 9920.5   |
| Extract all user names                      | strata      | 5.14     | 5.26        | 5.37     | 8745.00    | 4000    | 10049.3  |
| Extract all user names                      | jsonpath-ng | 35.44    | 35.96       | 36.48    | 1278.79    | 4000    | 9787.1   |
| Extract nested timestamp field              | jmespath    | 2.59     | 2.59        | 2.59     | 17759.93   | 4000    | 10420.0  |
| Extract nested timestamp field              | strata      | 5.58     | 5.60        | 5.62     | 8208.27    | 4000    | 10419.9  |
| Extract nested timestamp field              | jsonpath-ng | 34.68    | 34.77       | 34.85    | 1322.65    | 4000    | 10421.2  |
| Extract order item prices (double wildcard) | jmespath    | 233.92   | 235.44      | 236.96   | 195.31     | 4000    | 9881.4   |
| Extract order item prices (double wildcard) | strata      | 1096.92  | 1099.74     | 1102.55  | 41.81      | 789913  | 9875.8   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1106.37  | 1106.86     | 1107.35  | 41.54      | 789913  | 9517.4   |
| Filter users by age (numeric predicate)     | strata      | 4.61     | 4.63        | 4.66     | 9921.84    | 3212    | 10665.8  |
| Filter users by age (numeric predicate)     | jmespath    | 4.82     | 4.86        | 4.90     | 9461.69    | 3212    | 10661.4  |
| Recursively find all prices                 | strata      | 972.34   | 989.29      | 1006.23  | 46.48      | 789913  | 10216.6  |
| Recursively find all prices                 | jsonpath-ng | 4047.63  | 4060.86     | 4074.08  | 11.32      | 789913  | 9721.8   |
| Slice first 10 users, extract IDs           | jmespath    | 0.03     | 0.03        | 0.03     | 1636186.99 | 10      | 12041.2  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.04     | 1317745.42 | 10      | 12056.2  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.04     | 0.04        | 0.04     | 1246297.47 | 10      | 11812.2  |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2729.10  | 2832.56     | 2936.02  | 129.87 | 13059.3  |
| strata_cursor_reparse | 10560.98 | 10578.42    | 10595.86 | 34.78  | 14567.6  |
- Speedup (reuse vs reparse): 3.73x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.01        | 0.01     | 3727856.77 | 1       | 2746.4   |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.02     | 3148409.48 | 1       | 2762.4   |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 2608759.97 | 1       | 2745.4   |
| Extract all user IDs                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2013724.81 | 0       | 2436.1   |
| Extract all user IDs                        | jmespath            | 1.97     | 1.97        | 1.98     | 23288.07   | 4000    | 2436.1   |
| Extract all user IDs                        | strata              | 32.81    | 32.85       | 32.89    | 1399.58    | 4000    | 2430.1   |
| Extract all user IDs                        | strata_ndjson_full  | 83.76    | 84.35       | 84.94    | 545.11     | 4000    | 2435.5   |
| Extract all user IDs                        | strata_ndjson_fused | 82.17    | 86.29       | 90.41    | 532.87     | 4000    | 2435.9   |
| Extract all user names                      | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 2675087.17 | 0       | 2750.7   |
| Extract all user names                      | jmespath            | 1.88     | 2.39        | 2.91     | 19220.81   | 4000    | 2750.7   |
| Extract all user names                      | strata              | 33.51    | 34.17       | 34.83    | 1345.72    | 4000    | 2749.8   |
| Extract all user names                      | strata_ndjson_full  | 84.43    | 86.33       | 88.23    | 532.59     | 4000    | 2750.5   |
| Extract all user names                      | strata_ndjson_fused | 86.41    | 87.27       | 88.12    | 526.88     | 4000    | 2750.7   |
| Extract nested timestamp field              | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 4871988.17 | 0       | 2447.5   |
| Extract nested timestamp field              | jmespath            | 2.95     | 2.97        | 2.98     | 15507.27   | 4000    | 2447.5   |
| Extract nested timestamp field              | strata              | 33.82    | 33.89       | 33.97    | 1356.59    | 4000    | 2447.4   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 2807901.14 | 0       | 2517.1   |
| Extract order item prices (double wildcard) | jmespath            | 176.04   | 177.55      | 179.06   | 258.97     | 789913  | 2513.2   |
| Extract order item prices (double wildcard) | strata              | 934.49   | 949.40      | 964.30   | 48.43      | 789913  | 2492.4   |
| Filter users by age (numeric predicate)     | jmespath            | 5.03     | 5.08        | 5.12     | 9059.08    | 3212    | 2761.7   |
| Filter users by age (numeric predicate)     | strata              | 32.45    | 32.47       | 32.48    | 1416.17    | 3212    | 2761.7   |
| NDJSON first record id                      | jmespath            | 0.01     | 0.01        | 0.01     | 3892268.33 | 1       | 1468.1   |
| NDJSON first record id                      | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 2678358.16 | 1       | 1468.5   |
| NDJSON first record id                      | strata              | 0.01     | 0.02        | 0.03     | 1963545.93 | 1       | 1468.1   |
| Recursively find all prices                 | strata              | 977.77   | 979.86      | 981.96   | 46.92      | 789913  | 2781.0   |
| Recursively find all prices                 | jsonpath-ng         | 3616.59  | 3618.99     | 3621.38  | 12.71      | 789913  | 2811.2   |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.01        | 0.01     | 3464651.71 | 10      | 2808.2   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2103934.40 | 10      | 2806.2   |
| Slice first 10 users, extract IDs           | strata              | 0.03     | 0.03        | 0.03     | 1644587.42 | 10      | 2817.2   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2344.77  | 2347.09     | 2349.41  | 176.31 | 2816.3   |
| strata_cursor_reparse | 5586.66  | 5595.35     | 5604.04  | 73.96  | 3600.3   |
- Speedup (reuse vs reparse): 2.38x
