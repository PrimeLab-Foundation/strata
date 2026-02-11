# Strata Benchmark Results

Generated: 2026-02-12 01:10:25

## Environment

- Commit: 4b1934ea6b90104063201b6ada1fd68de1090565
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
| orjson        | users.json   | 122.009  | 178.476     | 193.599  | 10427.6  |
| msgspec       | users.json   | 132.186  | 186.261     | 188.897  | 10195.2  |
| ujson         | users.json   | 184.816  | 258.822     | 279.663  | 10317.6  |
| json (stdlib) | users.json   | 247.972  | 295.934     | 302.152  | 10203.6  |
| strata        | users.json   | 1530.734 | 1711.667    | 1758.211 | 10404.8  |
| orjson        | users.ndjson | 166.960  | 206.027     | 210.386  | 10332.8  |
| msgspec       | users.ndjson | 174.586  | 211.139     | 214.532  | 10095.0  |
| ujson         | users.ndjson | 234.831  | 293.562     | 307.156  | 10218.5  |
| json (stdlib) | users.ndjson | 286.468  | 324.762     | 329.831  | 10097.7  |
| strata        | users.ndjson | 328.707  | 388.491     | 406.649  | 10351.2  |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.597    | 4000    |
|                                     | jmespath    | 1.523    | 4000    |
|                                     | jsonpath-ng | 31.959   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 27.889   | 789913  |
|                                     | jmespath    | 213.440  | 4000    |
|                                     | jsonpath-ng | 1041.712 | 789913  |
| $..price                            | jmespath    | 216.371  | 4000    |
|                                     | query       | 263.497  | 789913  |
|                                     | jsonpath-ng | 3729.168 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.570    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 279.801  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 125.13   | 180.15      | 195.59   | 255.26 | 12483.9  |
| msgspec       | 136.64   | 193.70      | 205.33   | 237.39 | 12219.2  |
| ujson         | 183.52   | 269.10      | 282.35   | 170.88 | 12030.4  |
| json (stdlib) | 246.93   | 296.44      | 302.35   | 155.12 | 11951.9  |
| strata        | 1512.24  | 1576.89     | 2220.52  | 29.16  | 12455.9  |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 161.63   | 199.93      | 202.01   | 229.98 | 1405.9   |
| msgspec       | 174.41   | 205.72      | 211.93   | 223.50 | 1398.2   |
| strata        | 201.12   | 211.17      | 212.98   | 217.73 | 1406.7   |
| ujson         | 227.27   | 283.94      | 290.14   | 161.93 | 1520.8   |
| json (stdlib) | 277.44   | 318.20      | 319.60   | 144.50 | 1401.0   |
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
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 3721995.74 | 1       | 12552.2  |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 3056999.68 | 1       | 12513.6  |
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 2659305.93 | 1       | 12573.2  |
| Extract all user IDs                        | jmespath    | 1.87     | 1.92        | 1.97     | 23969.97   | 4000    | 5557.3   |
| Extract all user IDs                        | strata      | 4.93     | 4.94        | 4.95     | 9310.22    | 4000    | 5557.1   |
| Extract all user IDs                        | jsonpath-ng | 31.37    | 31.58       | 31.79    | 1456.00    | 4000    | 5559.3   |
| Extract all user names                      | jmespath    | 1.24     | 1.26        | 1.27     | 36609.16   | 4000    | 12716.2  |
| Extract all user names                      | strata      | 4.38     | 4.40        | 4.42     | 10444.10   | 4000    | 12716.1  |
| Extract all user names                      | jsonpath-ng | 31.14    | 31.25       | 31.36    | 1471.60    | 4000    | 12716.3  |
| Extract nested timestamp field              | jmespath    | 2.49     | 2.50        | 2.52     | 18368.71   | 4000    | 10419.9  |
| Extract nested timestamp field              | strata      | 5.51     | 5.54        | 5.57     | 8296.66    | 4000    | 10419.7  |
| Extract nested timestamp field              | jsonpath-ng | 33.99    | 34.00       | 34.02    | 1352.26    | 4000    | 10421.0  |
| Extract order item prices (double wildcard) | jmespath    | 216.87   | 218.55      | 220.23   | 210.40     | 4000    | 12592.7  |
| Extract order item prices (double wildcard) | strata      | 823.25   | 870.25      | 917.25   | 52.84      | 789913  | 12588.2  |
| Extract order item prices (double wildcard) | jsonpath-ng | 1068.04  | 1069.28     | 1070.52  | 43.00      | 789913  | 12229.3  |
| Filter users by age (numeric predicate)     | strata      | 4.09     | 4.11        | 4.14     | 11177.01   | 3212    | 12595.7  |
| Filter users by age (numeric predicate)     | jmespath    | 4.62     | 4.64        | 4.66     | 9911.02    | 3212    | 12595.7  |
| Recursively find all prices                 | strata      | 1235.99  | 1236.50     | 1237.01  | 37.19      | 789913  | 12515.9  |
| Recursively find all prices                 | jsonpath-ng | 3674.11  | 3688.81     | 3703.52  | 12.47      | 789913  | 12127.1  |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 2776355.52 | 10      | 13926.9  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1910958.84 | 10      | 13926.9  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 1531733.19 | 10      | 13941.9  |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2698.12  | 2842.05     | 2985.97  | 129.44 | 15700.5  |
| strata_cursor_reparse | 9746.07  | 10193.63    | 10641.20 | 36.09  | 17356.5  |
- Speedup (reuse vs reparse): 3.59x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.02        | 0.02     | 2811420.79 | 1       | 2735.1   |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 2522320.88 | 1       | 2735.1   |
| Deep path navigation                        | strata              | 0.02     | 0.03        | 0.03     | 1792848.11 | 0       | 2735.1   |
| Extract all user IDs                        | jmespath            | 1.99     | 2.01        | 2.03     | 22871.29   | 4000    | 2438.8   |
| Extract all user IDs                        | strata              | 10.99    | 11.14       | 11.29    | 4127.54    | 0       | 2436.4   |
| Extract all user IDs                        | jsonpath-ng         | 31.84    | 31.90       | 31.96    | 1441.44    | 4000    | 2438.8   |
| Extract all user IDs                        | strata_ndjson_fused | 43.70    | 44.06       | 44.42    | 1043.65    | 0       | 2438.7   |
| Extract all user IDs                        | strata_ndjson_full  | 45.91    | 47.06       | 48.21    | 977.02     | 0       | 2438.4   |
| Extract all user names                      | jmespath            | 1.93     | 1.93        | 1.93     | 23860.35   | 4000    | 2755.0   |
| Extract all user names                      | strata              | 11.18    | 11.25       | 11.31    | 4088.68    | 0       | 2754.3   |
| Extract all user names                      | jsonpath-ng         | 32.00    | 32.02       | 32.04    | 1435.89    | 4000    | 2755.0   |
| Extract all user names                      | strata_ndjson_fused | 44.46    | 46.49       | 48.52    | 988.98     | 0       | 2755.0   |
| Extract all user names                      | strata_ndjson_full  | 46.92    | 49.35       | 51.78    | 931.67     | 0       | 2754.9   |
| Extract nested timestamp field              | jmespath            | 3.06     | 3.08        | 3.10     | 14922.51   | 4000    | 2441.5   |
| Extract nested timestamp field              | strata              | 11.20    | 11.24       | 11.28    | 4091.49    | 0       | 2441.4   |
| Extract nested timestamp field              | jsonpath-ng         | 34.56    | 34.57       | 34.58    | 1330.17    | 4000    | 2441.5   |
| Extract order item prices (double wildcard) | strata              | 11.22    | 11.24       | 11.25    | 4092.47    | 0       | 2460.9   |
| Extract order item prices (double wildcard) | jmespath            | 179.37   | 180.88      | 182.39   | 254.20     | 789913  | 2494.3   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 1024.57  | 1026.85     | 1029.13  | 44.78      | 789913  | 2500.4   |
| Filter users by age (numeric predicate)     | strata              | 0.03     | 0.03        | 0.03     | 1615692.63 | 0       | 2759.1   |
| Filter users by age (numeric predicate)     | jmespath            | 5.13     | 5.23        | 5.33     | 8793.78    | 3212    | 2759.1   |
| NDJSON root field (id)                      | jsonpath-ng         | 0.01     | 0.02        | 0.03     | 2471412.46 | 0       | 1484.9   |
| NDJSON root field (id)                      | strata              | 32.55    | 32.59       | 32.62    | 1410.96    | 4000    | 1471.4   |
| NDJSON root field (id)                      | strata_ndjson_fused | 76.92    | 78.60       | 80.27    | 584.99     | 4000    | 1484.9   |
| NDJSON root field (id)                      | strata_ndjson_full  | 77.64    | 81.88       | 86.11    | 561.58     | 4000    | 1484.0   |
| Recursively find all prices                 | strata              | 1224.19  | 1226.73     | 1229.28  | 37.48      | 3898    | 2797.8   |
| Recursively find all prices                 | jsonpath-ng         | 3658.23  | 3658.50     | 3658.78  | 12.57      | 789913  | 2825.9   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.02        | 0.03     | 1929231.95 | 10      | 2815.4   |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.03     | 1856178.12 | 10      | 2817.4   |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.03        | 0.03     | 1775540.08 | 0       | 2832.4   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 1649.50  | 1651.52     | 1653.53  | 250.57 | 2824.3   |
| strata_cursor_reparse | 4784.94  | 4787.67     | 4790.39  | 86.43  | 3681.0   |
- Speedup (reuse vs reparse): 2.90x
