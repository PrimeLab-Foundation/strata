# Strata Benchmark Results

Generated: 2026-02-13 03:03:45

## Environment

- Commit: 4ec9f875bd9c5821d060084e19b1070754ee66b0
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
| orjson        | users.json   | 134.859  | 193.811     | 194.952  | 716.6    |
| msgspec       | users.json   | 154.230  | 229.056     | 383.809  | 709.1    |
| strata        | users.json   | 210.820  | 239.292     | 243.525  | 570.8    |
| ujson         | users.json   | 300.932  | 310.389     | 387.910  | 831.5    |
| json (stdlib) | users.json   | 271.565  | 323.286     | 327.456  | 709.1    |
| orjson        | users.ndjson | 209.355  | 237.476     | 241.562  | 735.2    |
| msgspec       | users.ndjson | 200.349  | 238.121     | 248.829  | 728.8    |
| ujson         | users.ndjson | 265.964  | 339.487     | 342.583  | 852.2    |
| json (stdlib) | users.ndjson | 325.823  | 352.352     | 363.913  | 731.5    |
| strata        | users.ndjson | 402.769  | 475.967     | 485.375  | 684.1    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.692    | 4000    |
|                                     | jmespath    | 1.960    | 4000    |
|                                     | jsonpath-ng | 37.638   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 30.526   | 789913  |
|                                     | jmespath    | 230.686  | 4000    |
|                                     | jsonpath-ng | 1147.391 | 789913  |
| $..price                            | jmespath    | 234.563  | 4000    |
|                                     | query       | 281.649  | 789913  |
|                                     | jsonpath-ng | 4057.387 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.677    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 296.583  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 137.10   | 190.10      | 199.03   | 241.90 | 716.9    |
| msgspec       | 144.40   | 200.08      | 206.78   | 229.82 | 708.2    |
| strata        | 216.76   | 238.25      | 242.95   | 193.00 | 570.0    |
| ujson         | 196.70   | 276.20      | 291.44   | 166.49 | 830.8    |
| json (stdlib) | 256.22   | 322.31      | 327.34   | 142.67 | 708.3    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 191.17   | 233.57      | 236.08   | 196.85 | 1407.5   |
| strata        | 237.47   | 239.22      | 245.99   | 192.21 | 1408.2   |
| msgspec       | 193.19   | 240.35      | 246.50   | 191.30 | 1399.8   |
| ujson         | 264.72   | 335.57      | 340.41   | 137.02 | 1522.4   |
| json (stdlib) | 324.80   | 355.74      | 356.93   | 129.25 | 1402.6   |
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
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.03     | 1620560.72 | 1       | 9194.0   |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1618193.07 | 1       | 9173.0   |
| Deep path navigation                        | jmespath    | 0.03     | 0.03        | 0.04     | 1316180.33 | 1       | 9173.0   |
| Extract all user IDs                        | jmespath    | 1.84     | 1.85        | 1.87     | 24848.89   | 4000    | 5557.6   |
| Extract all user IDs                        | strata      | 5.78     | 5.86        | 5.94     | 7845.99    | 4000    | 5557.2   |
| Extract all user IDs                        | jsonpath-ng | 37.24    | 37.68       | 38.12    | 1220.41    | 4000    | 5559.7   |
| Extract all user names                      | jmespath    | 1.45     | 1.46        | 1.48     | 31436.72   | 4000    | 10655.3  |
| Extract all user names                      | strata      | 5.58     | 5.62        | 5.65     | 8188.93    | 4000    | 10695.8  |
| Extract all user names                      | jsonpath-ng | 36.70    | 37.04       | 37.38    | 1241.41    | 4000    | 10641.7  |
| Extract nested timestamp field              | jmespath    | 2.88     | 2.92        | 2.96     | 15737.07   | 4000    | 8309.9   |
| Extract nested timestamp field              | strata      | 5.95     | 5.98        | 6.01     | 7685.54    | 4000    | 8385.3   |
| Extract nested timestamp field              | jsonpath-ng | 40.39    | 40.78       | 41.16    | 1127.70    | 4000    | 8241.9   |
| Extract order item prices (double wildcard) | jmespath    | 234.01   | 234.06      | 234.10   | 196.46     | 4000    | 8651.3   |
| Extract order item prices (double wildcard) | strata      | 966.45   | 986.31      | 1006.17  | 46.62      | 789913  | 8642.9   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1186.06  | 1195.19     | 1204.32  | 38.47      | 789913  | 8319.2   |
| Filter users by age (numeric predicate)     | strata      | 4.53     | 4.57        | 4.62     | 10052.98   | 3212    | 11957.5  |
| Filter users by age (numeric predicate)     | jmespath    | 5.41     | 5.47        | 5.54     | 8400.20    | 3212    | 11920.5  |
| Recursively find all prices                 | strata      | 1032.44  | 1047.63     | 1062.82  | 43.89      | 789913  | 13382.7  |
| Recursively find all prices                 | jsonpath-ng | 4133.77  | 4137.61     | 4141.45  | 11.11      | 789913  | 12801.5  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 1946387.26 | 10      | 13569.1  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.04     | 0.04        | 0.04     | 1270715.97 | 10      | 13569.1  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.04        | 0.04     | 1264148.55 | 10      | 13647.0  |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2861.11  | 2873.07     | 2885.04  | 128.04 | 13356.2  |
| strata_cursor_reparse | 8753.83  | 9443.32     | 10132.81 | 38.96  | 15612.4  |
- Speedup (reuse vs reparse): 3.29x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.02     | 0.02        | 0.02     | 2195977.69 | 1       | 2512.5   |
| Deep path navigation                        | strata              | 0.02     | 0.02        | 0.02     | 1862456.15 | 1       | 2528.5   |
| Deep path navigation                        | jsonpath-ng         | 0.03     | 0.03        | 0.03     | 1531599.95 | 1       | 2511.5   |
| Extract all user IDs                        | jsonpath-ng         | 0.03     | 0.03        | 0.04     | 1484211.49 | 0       | 2435.2   |
| Extract all user IDs                        | jmespath            | 2.14     | 2.40        | 2.65     | 19187.05   | 4000    | 2435.2   |
| Extract all user IDs                        | strata              | 40.53    | 41.08       | 41.63    | 1119.23    | 4000    | 2430.3   |
| Extract all user IDs                        | strata_ndjson_fused | 90.33    | 92.35       | 94.37    | 497.88     | 4000    | 2435.2   |
| Extract all user IDs                        | strata_ndjson_full  | 92.15    | 93.14       | 94.13    | 493.65     | 4000    | 2434.6   |
| Extract all user names                      | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 1792882.45 | 0       | 2514.3   |
| Extract all user names                      | jmespath            | 2.02     | 2.23        | 2.44     | 20619.14   | 4000    | 2514.3   |
| Extract all user names                      | strata              | 40.11    | 40.89       | 41.67    | 1124.55    | 4000    | 2513.2   |
| Extract all user names                      | strata_ndjson_fused | 89.97    | 91.60       | 93.24    | 501.93     | 4000    | 2514.3   |
| Extract all user names                      | strata_ndjson_full  | 97.23    | 98.45       | 99.67    | 467.03     | 4000    | 2514.0   |
| Extract nested timestamp field              | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 1795789.59 | 0       | 2450.0   |
| Extract nested timestamp field              | jmespath            | 3.64     | 3.66        | 3.68     | 12565.39   | 4000    | 2450.0   |
| Extract nested timestamp field              | strata              | 40.02    | 40.80       | 41.58    | 1127.04    | 4000    | 2450.0   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.02     | 0.03        | 0.04     | 1467442.67 | 0       | 2516.0   |
| Extract order item prices (double wildcard) | jmespath            | 193.98   | 196.41      | 198.84   | 234.10     | 789913  | 2512.0   |
| Extract order item prices (double wildcard) | strata              | 1155.83  | 1163.31     | 1170.80  | 39.52      | 789913  | 2478.0   |
| Filter users by age (numeric predicate)     | jmespath            | 6.10     | 6.25        | 6.40     | 7356.63    | 3212    | 2517.7   |
| Filter users by age (numeric predicate)     | strata              | 40.80    | 41.16       | 41.52    | 1117.12    | 3212    | 2517.7   |
| NDJSON first record id                      | jmespath            | 0.02     | 0.02        | 0.02     | 2340393.90 | 1       | 1468.2   |
| NDJSON first record id                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 1904224.20 | 1       | 1468.6   |
| NDJSON first record id                      | strata              | 0.02     | 0.02        | 0.03     | 1845339.16 | 1       | 1468.2   |
| Recursively find all prices                 | strata              | 1114.44  | 1140.54     | 1166.65  | 40.31      | 789913  | 2539.0   |
| Recursively find all prices                 | jsonpath-ng         | 4107.60  | 4123.57     | 4139.55  | 11.15      | 789913  | 2535.1   |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.02     | 2172220.40 | 10      | 2530.8   |
| Slice first 10 users, extract IDs           | strata              | 0.03     | 0.04        | 0.04     | 1262580.32 | 10      | 2539.8   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.03     | 0.04        | 0.04     | 1250442.18 | 10      | 2528.8   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2734.01  | 2758.86     | 2783.71  | 149.99 | 2541.6   |
| strata_cursor_reparse | 6493.25  | 6535.19     | 6577.13  | 63.32  | 3666.4   |
- Speedup (reuse vs reparse): 2.37x
