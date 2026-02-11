# Strata Benchmark Results

Generated: 2026-02-11 22:51:16

## Environment

- Commit: 9ffdd71efb8fe8bda6bb58fb8f1387ca32d249cf
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
| orjson        | users.json   | 133.606  | 182.682     | 194.611  | 12077.7  |
| msgspec       | users.json   | 132.454  | 184.688     | 193.023  | 11608.8  |
| ujson         | users.json   | 188.157  | 259.511     | 276.446  | 11731.2  |
| json (stdlib) | users.json   | 242.087  | 294.221     | 296.512  | 11652.7  |
| strata        | users.json   | 1478.471 | 1585.603    | 1831.744 | 12109.2  |
| orjson        | users.ndjson | 168.587  | 206.585     | 212.309  | 11398.5  |
| msgspec       | users.ndjson | 176.338  | 213.805     | 214.369  | 11205.0  |
| ujson         | users.ndjson | 233.909  | 293.771     | 302.505  | 11328.5  |
| json (stdlib) | users.ndjson | 285.428  | 322.303     | 323.620  | 11207.7  |
| strata        | users.ndjson | 327.429  | 383.767     | 397.685  | 11349.6  |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.596    | 4000    |
|                                     | jmespath    | 1.556    | 4000    |
|                                     | jsonpath-ng | 31.816   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 27.593   | 789913  |
|                                     | jmespath    | 216.172  | 4000    |
|                                     | jsonpath-ng | 1044.061 | 789913  |
| $..price                            | jmespath    | 216.997  | 4000    |
|                                     | query       | 264.247  | 789913  |
|                                     | jsonpath-ng | 3702.077 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.568    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 276.929  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 126.74   | 180.81      | 194.78   | 15658.5  |
| msgspec       | 136.24   | 187.65      | 192.91   | 15558.4  |
| ujson         | 185.64   | 268.00      | 269.05   | 15432.0  |
| json (stdlib) | 244.26   | 299.70      | 300.60   | 15353.5  |
| strata        | 1302.70  | 1590.99     | 2656.22  | 15597.3  |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 160.57   | 199.71      | 203.11   | 1405.1   |
| msgspec       | 168.83   | 205.84      | 208.44   | 1399.4   |
| strata        | 204.49   | 209.32      | 211.24   | 1407.9   |
| ujson         | 226.91   | 284.91      | 292.97   | 1522.0   |
| json (stdlib) | 277.52   | 314.28      | 319.10   | 1402.3   |
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
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 1       | 13112.5  |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 1       | 13135.5  |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 1       | 13112.5  |
| Extract all user IDs                        | jmespath    | 1.75     | 1.76        | 1.77     | 4000    | 5557.8   |
| Extract all user IDs                        | strata      | 4.91     | 4.94        | 4.97     | 4000    | 5557.7   |
| Extract all user IDs                        | jsonpath-ng | 31.71    | 32.66       | 33.62    | 4000    | 5559.9   |
| Extract all user names                      | jmespath    | 1.23     | 1.33        | 1.43     | 4000    | 13586.6  |
| Extract all user names                      | strata      | 4.32     | 4.41        | 4.51     | 4000    | 13586.6  |
| Extract all user names                      | jsonpath-ng | 31.06    | 31.09       | 31.12    | 4000    | 13586.6  |
| Extract nested timestamp field              | jmespath    | 2.48     | 2.49        | 2.50     | 4000    | 10420.2  |
| Extract nested timestamp field              | strata      | 5.46     | 5.46        | 5.47     | 4000    | 10420.1  |
| Extract nested timestamp field              | jsonpath-ng | 33.56    | 34.24       | 34.92    | 4000    | 10421.5  |
| Extract order item prices (double wildcard) | jmespath    | 214.81   | 215.93      | 217.04   | 4000    | 12006.8  |
| Extract order item prices (double wildcard) | strata      | 810.67   | 810.72      | 810.78   | 789913  | 11993.1  |
| Extract order item prices (double wildcard) | jsonpath-ng | 1057.59  | 1063.76     | 1069.92  | 789913  | 12011.5  |
| Filter users by age (numeric predicate)     | strata      | 4.00     | 4.01        | 4.03     | 3212    | 15526.4  |
| Filter users by age (numeric predicate)     | jmespath    | 4.73     | 4.78        | 4.83     | 3212    | 15526.4  |
| Recursively find all prices                 | strata      | 1192.71  | 1209.00     | 1225.29  | 789913  | 16178.0  |
| Recursively find all prices                 | jsonpath-ng | 3679.96  | 3721.42     | 3762.88  | 789913  | 15569.3  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 10      | 15979.2  |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.03     | 10      | 15980.2  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 10      | 15997.2  |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 1       | 2508.4   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 1       | 2508.4   |
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.03     | 0       | 2508.4   |
| Extract all user IDs                        | jmespath    | 1.99     | 2.04        | 2.09     | 4000    | 1468.7   |
| Extract all user IDs                        | strata      | 11.46    | 11.50       | 11.53    | 0       | 1468.7   |
| Extract all user IDs                        | jsonpath-ng | 32.07    | 32.09       | 32.11    | 4000    | 1470.6   |
| Extract all user names                      | jmespath    | 1.91     | 1.93        | 1.95     | 4000    | 2513.0   |
| Extract all user names                      | strata      | 11.31    | 11.36       | 11.41    | 0       | 2513.0   |
| Extract all user names                      | jsonpath-ng | 31.86    | 31.92       | 31.99    | 4000    | 2513.0   |
| Extract nested timestamp field              | jmespath    | 3.18     | 3.21        | 3.23     | 4000    | 2385.4   |
| Extract nested timestamp field              | strata      | 11.49    | 11.50       | 11.51    | 0       | 2385.3   |
| Extract nested timestamp field              | jsonpath-ng | 34.64    | 34.72       | 34.79    | 4000    | 2386.5   |
| Extract order item prices (double wildcard) | strata      | 11.10    | 11.28       | 11.47    | 0       | 2407.4   |
| Extract order item prices (double wildcard) | jmespath    | 179.72   | 180.62      | 181.51   | 789913  | 2448.5   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1034.75  | 1034.95     | 1035.15  | 789913  | 2507.0   |
| Filter users by age (numeric predicate)     | strata      | 0.02     | 0.03        | 0.03     | 0       | 2515.4   |
| Filter users by age (numeric predicate)     | jmespath    | 5.09     | 5.19        | 5.28     | 3212    | 2515.4   |
| Recursively find all prices                 | strata      | 1209.05  | 1213.96     | 1218.87  | 3898    | 2545.2   |
| Recursively find all prices                 | jsonpath-ng | 3671.86  | 3677.55     | 3683.24  | 789913  | 2603.8   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 2594.6   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 10      | 2593.6   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.03     | 0       | 2613.6   |
