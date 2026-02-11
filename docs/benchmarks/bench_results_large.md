# Strata Benchmark Results

Generated: 2026-02-11 23:36:09

## Environment

- Commit: ec4905efccfcf6fe97a503fe7701cd9771c3d6f0
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
| orjson        | users.json   | 132.825  | 182.500     | 203.333  | 13279.6  |
| msgspec       | users.json   | 133.273  | 190.355     | 193.215  | 12779.9  |
| ujson         | users.json   | 195.528  | 269.552     | 280.764  | 12902.3  |
| json (stdlib) | users.json   | 244.378  | 300.920     | 302.801  | 12823.8  |
| strata        | users.json   | 1456.565 | 1742.712    | 2203.082 | 13319.9  |
| orjson        | users.ndjson | 174.377  | 208.157     | 219.352  | 11867.7  |
| msgspec       | users.ndjson | 176.481  | 216.672     | 217.722  | 11687.4  |
| ujson         | users.ndjson | 239.789  | 300.862     | 307.294  | 11810.9  |
| json (stdlib) | users.ndjson | 289.042  | 328.281     | 330.569  | 11690.1  |
| strata        | users.ndjson | 333.456  | 393.181     | 411.563  | 11875.9  |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.588    | 4000    |
|                                     | jmespath    | 1.550    | 4000    |
|                                     | jsonpath-ng | 32.023   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 28.379   | 789913  |
|                                     | jmespath    | 218.673  | 4000    |
|                                     | jsonpath-ng | 1063.102 | 789913  |
| $..price                            | jmespath    | 217.053  | 4000    |
|                                     | query       | 265.089  | 789913  |
|                                     | jsonpath-ng | 3790.252 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.579    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 280.439  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 131.72   | 192.71      | 201.01   | 16503.1  |
| msgspec       | 136.35   | 195.27      | 203.05   | 16494.0  |
| ujson         | 189.74   | 270.99      | 286.71   | 16614.7  |
| json (stdlib) | 249.26   | 304.80      | 304.83   | 16536.2  |
| strata        | 1394.02  | 1675.24     | 2884.28  | 16156.3  |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 169.63   | 205.64      | 223.89   | 1404.5   |
| strata        | 206.89   | 216.03      | 221.23   | 1407.3   |
| msgspec       | 177.55   | 217.60      | 219.76   | 1398.8   |
| ujson         | 244.26   | 291.54      | 300.47   | 1521.4   |
| json (stdlib) | 287.05   | 334.86      | 349.36   | 1401.7   |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 14190.6  |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 1       | 14226.8  |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 1       | 14190.6  |
| Extract all user IDs                        | jmespath    | 1.72     | 1.78        | 1.85     | 4000    | 5557.3   |
| Extract all user IDs                        | strata      | 5.01     | 5.12        | 5.23     | 4000    | 5557.2   |
| Extract all user IDs                        | jsonpath-ng | 31.90    | 31.92       | 31.93    | 4000    | 5559.2   |
| Extract all user names                      | jmespath    | 1.23     | 1.25        | 1.26     | 4000    | 14318.0  |
| Extract all user names                      | strata      | 4.45     | 4.65        | 4.85     | 4000    | 14317.9  |
| Extract all user names                      | jsonpath-ng | 31.52    | 31.64       | 31.77    | 4000    | 14243.3  |
| Extract nested timestamp field              | jmespath    | 2.50     | 2.51        | 2.53     | 4000    | 10419.8  |
| Extract nested timestamp field              | strata      | 5.48     | 5.49        | 5.50     | 4000    | 10419.7  |
| Extract nested timestamp field              | jsonpath-ng | 34.48    | 34.57       | 34.66    | 4000    | 10420.9  |
| Extract order item prices (double wildcard) | jmespath    | 216.23   | 217.38      | 218.52   | 4000    | 13423.1  |
| Extract order item prices (double wildcard) | strata      | 845.96   | 900.01      | 954.07   | 789913  | 13409.4  |
| Extract order item prices (double wildcard) | jsonpath-ng | 1066.03  | 1066.35     | 1066.67  | 789913  | 13423.7  |
| Filter users by age (numeric predicate)     | strata      | 4.06     | 4.07        | 4.09     | 3212    | 14300.5  |
| Filter users by age (numeric predicate)     | jmespath    | 4.68     | 4.69        | 4.70     | 3212    | 14300.5  |
| Recursively find all prices                 | strata      | 1242.55  | 1244.34     | 1246.14  | 789913  | 14206.7  |
| Recursively find all prices                 | jsonpath-ng | 3720.52  | 3747.40     | 3774.28  | 789913  | 13831.7  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 10      | 14204.0  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 10      | 14203.0  |
| Slice first 10 users, extract IDs           | strata      | 0.04     | 0.05        | 0.06     | 10      | 14220.0  |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 1       | 2530.3   |
| Deep path navigation                        | jmespath            | 0.01     | 0.02        | 0.02     | 1       | 2530.3   |
| Deep path navigation                        | strata              | 0.02     | 0.03        | 0.03     | 0       | 2530.3   |
| Extract all user IDs                        | jmespath            | 2.02     | 2.03        | 2.04     | 4000    | 2391.5   |
| Extract all user IDs                        | strata              | 11.49    | 11.51       | 11.53    | 0       | 2390.2   |
| Extract all user IDs                        | jsonpath-ng         | 32.06    | 32.30       | 32.55    | 4000    | 2391.5   |
| Extract all user IDs                        | strata_ndjson_fused | 192.60   | 193.69      | 194.78   | 0       | 2391.2   |
| Extract all user IDs                        | strata_ndjson_full  | 283.95   | 285.26      | 286.57   | 0       | 2391.1   |
| Extract all user names                      | jmespath            | 1.92     | 2.00        | 2.09     | 4000    | 2535.2   |
| Extract all user names                      | strata              | 11.59    | 11.60       | 11.62    | 0       | 2535.1   |
| Extract all user names                      | jsonpath-ng         | 32.12    | 32.30       | 32.49    | 4000    | 2535.3   |
| Extract all user names                      | strata_ndjson_fused | 193.14   | 193.25      | 193.36   | 0       | 2535.2   |
| Extract all user names                      | strata_ndjson_full  | 282.31   | 282.92      | 283.53   | 0       | 2535.2   |
| Extract nested timestamp field              | jmespath            | 3.09     | 3.11        | 3.12     | 4000    | 2411.8   |
| Extract nested timestamp field              | strata              | 11.15    | 11.26       | 11.37    | 0       | 2411.8   |
| Extract nested timestamp field              | jsonpath-ng         | 35.08    | 35.13       | 35.17    | 4000    | 2411.8   |
| Extract order item prices (double wildcard) | strata              | 11.49    | 11.53       | 11.57    | 0       | 2431.7   |
| Extract order item prices (double wildcard) | jmespath            | 182.02   | 184.53      | 187.04   | 789913  | 2470.6   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 1034.47  | 1036.25     | 1038.03  | 789913  | 2529.2   |
| Filter users by age (numeric predicate)     | strata              | 0.02     | 0.02        | 0.03     | 0       | 2539.5   |
| Filter users by age (numeric predicate)     | jmespath            | 5.13     | 5.22        | 5.31     | 3212    | 2539.5   |
| NDJSON root field (id)                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 0       | 1433.5   |
| NDJSON root field (id)                      | strata              | 33.03    | 33.51       | 33.99    | 4000    | 1429.7   |
| NDJSON root field (id)                      | strata_ndjson_fused | 45.82    | 46.16       | 46.50    | 4000    | 1433.5   |
| NDJSON root field (id)                      | strata_ndjson_full  | 303.46   | 304.33      | 305.20   | 4000    | 1433.5   |
| Recursively find all prices                 | strata              | 1235.63  | 1239.79     | 1243.95  | 3898    | 2565.1   |
| Recursively find all prices                 | jsonpath-ng         | 3680.22  | 3688.21     | 3696.20  | 789913  | 2623.8   |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.02     | 10      | 2614.6   |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.02        | 0.02     | 0       | 2631.6   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.02        | 0.03     | 10      | 2613.6   |
