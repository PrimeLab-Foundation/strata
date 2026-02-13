# Strata Benchmark Results

Generated: 2026-02-14 01:13:23

## Environment

- Commit: e8443f3b87ee8f0b4d32fbf0d4c653dcb146cde0
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
| orjson        | users.json   | 125.118  | 176.149     | 177.988  | 731.4    |
| msgspec       | users.json   | 130.755  | 182.553     | 191.287  | 724.0    |
| ujson         | users.json   | 184.512  | 256.772     | 258.693  | 846.4    |
| json (stdlib) | users.json   | 239.380  | 293.570     | 294.465  | 724.0    |
| strata        | users.json   | 291.902  | 316.869     | 318.451  | 593.3    |
| orjson        | users.ndjson | 166.989  | 206.051     | 206.596  | 740.9    |
| msgspec       | users.ndjson | 171.539  | 210.366     | 210.761  | 734.5    |
| ujson         | users.ndjson | 234.224  | 294.826     | 298.928  | 858.0    |
| json (stdlib) | users.ndjson | 280.971  | 328.235     | 329.833  | 737.2    |
| strata        | users.ndjson | 371.354  | 418.261     | 432.748  | 689.9    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.594    | 4000    |
|                                     | jmespath    | 1.576    | 4000    |
|                                     | jsonpath-ng | 32.014   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 27.531   | 789913  |
|                                     | jmespath    | 215.477  | 4000    |
|                                     | jsonpath-ng | 1040.679 | 789913  |
| $..price                            | jmespath    | 217.930  | 4000    |
|                                     | query       | 261.320  | 789913  |
|                                     | jsonpath-ng | 3680.434 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.598    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 281.457  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 125.50   | 178.92      | 182.36   | 257.00 | 731.8    |
| msgspec       | 132.23   | 185.46      | 186.74   | 247.95 | 723.2    |
| ujson         | 181.91   | 265.55      | 269.88   | 173.17 | 845.7    |
| json (stdlib) | 241.92   | 293.15      | 295.37   | 156.86 | 723.2    |
| strata        | 292.55   | 319.02      | 320.87   | 144.14 | 593.7    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 165.19   | 208.14      | 211.63   | 220.90 | 1408.5   |
| msgspec       | 173.32   | 211.66      | 212.20   | 217.23 | 1400.8   |
| strata        | 203.98   | 218.16      | 219.78   | 210.76 | 1409.3   |
| ujson         | 235.59   | 292.77      | 294.74   | 157.05 | 1523.4   |
| json (stdlib) | 284.76   | 324.66      | 329.29   | 141.62 | 1403.7   |
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
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 2575522.44 | 1       | 10228.6  |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2055078.08 | 1       | 10175.7  |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 1857915.00 | 1       | 10207.6  |
| Extract all user IDs                        | jmespath    | 1.68     | 1.72        | 1.75     | 26781.56   | 4000    | 5557.1   |
| Extract all user IDs                        | strata      | 4.99     | 5.10        | 5.21     | 9013.15    | 4000    | 5556.8   |
| Extract all user IDs                        | jsonpath-ng | 31.83    | 31.85       | 31.86    | 1443.88    | 4000    | 5559.2   |
| Extract all user names                      | jmespath    | 1.47     | 1.55        | 1.63     | 29711.02   | 4000    | 11196.8  |
| Extract all user names                      | strata      | 5.23     | 5.24        | 5.25     | 8770.89    | 4000    | 11225.8  |
| Extract all user names                      | jsonpath-ng | 31.57    | 31.63       | 31.69    | 1453.82    | 4000    | 11130.0  |
| Extract nested timestamp field              | jmespath    | 2.63     | 2.64        | 2.66     | 17402.98   | 4000    | 10359.1  |
| Extract nested timestamp field              | strata      | 5.18     | 5.24        | 5.31     | 8770.19    | 4000    | 10359.1  |
| Extract nested timestamp field              | jsonpath-ng | 35.29    | 35.54       | 35.80    | 1293.70    | 4000    | 10360.4  |
| Extract order item prices (double wildcard) | jmespath    | 224.29   | 224.87      | 225.45   | 204.49     | 4000    | 10143.3  |
| Extract order item prices (double wildcard) | strata      | 851.69   | 885.19      | 918.69   | 51.95      | 789913  | 10143.9  |
| Extract order item prices (double wildcard) | jsonpath-ng | 1077.27  | 1087.35     | 1097.43  | 42.29      | 789913  | 9910.2   |
| Filter users by age (numeric predicate)     | strata      | 4.18     | 4.24        | 4.30     | 10850.63   | 3212    | 12276.8  |
| Filter users by age (numeric predicate)     | jmespath    | 4.80     | 4.86        | 4.92     | 9460.87    | 3212    | 12217.1  |
| Recursively find all prices                 | strata      | 904.26   | 914.51      | 924.75   | 50.28      | 789913  | 13289.4  |
| Recursively find all prices                 | jsonpath-ng | 3880.25  | 3939.01     | 3997.77  | 11.67      | 789913  | 12973.2  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 2358122.46 | 10      | 14277.1  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.04     | 1364998.66 | 10      | 14292.1  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.05     | 1066801.23 | 10      | 14277.1  |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2656.95  | 2789.97     | 2923.00  | 131.85 | 15303.7  |
| strata_cursor_reparse | 9246.51  | 9876.80     | 10507.08 | 37.25  | 16374.3  |
- Speedup (reuse vs reparse): 3.54x
