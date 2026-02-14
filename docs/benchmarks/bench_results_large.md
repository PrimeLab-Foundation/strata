# Strata Benchmark Results

Generated: 2026-02-14 14:48:05

## Environment

- Commit: 1a605860e0960662442cc11396164b9e31b0f22e
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
| orjson        | users.json   | 130.928  | 183.520     | 186.460  | 731.6    |
| msgspec       | users.json   | 135.588  | 192.161     | 196.426  | 723.0    |
| ujson         | users.json   | 189.477  | 258.925     | 276.900  | 845.4    |
| json (stdlib) | users.json   | 262.222  | 298.988     | 326.813  | 723.0    |
| strata        | users.json   | 305.845  | 331.662     | 348.366  | 593.5    |
| msgspec       | users.ndjson | 181.968  | 221.316     | 224.336  | 733.6    |
| orjson        | users.ndjson | 196.098  | 222.328     | 233.184  | 740.1    |
| ujson         | users.ndjson | 245.339  | 306.481     | 315.711  | 857.1    |
| json (stdlib) | users.ndjson | 313.526  | 334.021     | 336.732  | 736.3    |
| strata        | users.ndjson | 362.346  | 447.059     | 450.833  | 689.0    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.597    | 4000    |
|                                     | jmespath    | 1.536    | 4000    |
|                                     | jsonpath-ng | 32.018   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 29.109   | 789913  |
|                                     | jmespath    | 220.172  | 4000    |
|                                     | jsonpath-ng | 1085.198 | 789913  |
| $..price                            | jmespath    | 230.594  | 4000    |
|                                     | query       | 275.758  | 789913  |
|                                     | jsonpath-ng | 3849.531 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.585    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 284.532  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 129.07   | 178.75      | 181.32   | 257.25 | 731.9    |
| msgspec       | 138.05   | 201.16      | 201.67   | 228.59 | 723.4    |
| ujson         | 200.21   | 262.09      | 265.32   | 175.45 | 845.9    |
| json (stdlib) | 245.07   | 302.03      | 305.36   | 152.25 | 723.4    |
| strata        | 297.06   | 323.12      | 327.66   | 142.31 | 593.8    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 168.96   | 207.88      | 211.07   | 221.18 | 1367.5   |
| msgspec       | 174.66   | 215.04      | 216.75   | 213.82 | 1358.7   |
| strata        | 206.97   | 215.96      | 223.43   | 212.90 | 1365.9   |
| ujson         | 238.34   | 299.58      | 301.75   | 153.48 | 1481.3   |
| json (stdlib) | 286.94   | 337.20      | 457.78   | 136.35 | 1361.5   |
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
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2320929.53 | 1       | 9739.9   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2256907.48 | 1       | 9739.9   |
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.03     | 1620560.30 | 1       | 9760.9   |
| Extract all user IDs                        | jmespath    | 1.69     | 1.73        | 1.78     | 26527.29   | 4000    | 5557.3   |
| Extract all user IDs                        | strata      | 4.98     | 5.05        | 5.12     | 9107.92    | 4000    | 5557.0   |
| Extract all user IDs                        | jsonpath-ng | 31.87    | 31.89       | 31.91    | 1441.96    | 4000    | 5559.3   |
| Extract all user names                      | jmespath    | 1.33     | 1.34        | 1.36     | 34227.10   | 4000    | 11123.0  |
| Extract all user names                      | strata      | 5.03     | 5.14        | 5.24     | 8949.31    | 4000    | 11151.1  |
| Extract all user names                      | jsonpath-ng | 31.73    | 31.94       | 32.15    | 1439.81    | 4000    | 11111.1  |
| Extract nested timestamp field              | jmespath    | 2.56     | 2.59        | 2.63     | 17722.13   | 4000    | 7731.0   |
| Extract nested timestamp field              | strata      | 5.56     | 5.58        | 5.60     | 8240.07    | 4000    | 7796.4   |
| Extract nested timestamp field              | jsonpath-ng | 34.19    | 34.31       | 34.42    | 1340.41    | 4000    | 7732.3   |
| Extract order item prices (double wildcard) | jmespath    | 220.04   | 220.56      | 221.09   | 208.48     | 4000    | 9254.8   |
| Extract order item prices (double wildcard) | strata      | 849.49   | 860.47      | 871.45   | 53.44      | 789913  | 9379.8   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1098.80  | 1102.30     | 1105.79  | 41.72      | 789913  | 8719.9   |
| Filter users by age (numeric predicate)     | jmespath    | 4.77     | 4.81        | 4.86     | 9557.18    | 3212    | 12577.8  |
| Filter users by age (numeric predicate)     | strata      | 4.75     | 4.92        | 5.09     | 9344.75    | 3212    | 12593.5  |
| Recursively find all prices                 | strata      | 923.90   | 934.36      | 944.82   | 49.21      | 789913  | 13690.3  |
| Recursively find all prices                 | jsonpath-ng | 3774.41  | 3794.29     | 3814.16  | 12.12      | 789913  | 13289.3  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 1984865.09 | 10      | 13887.7  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.04     | 1365829.67 | 10      | 13914.1  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 1307590.61 | 10      | 13867.2  |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2566.73  | 2679.50     | 2792.27  | 137.29 | 14401.9  |
| strata_cursor_reparse | 10842.50 | 11053.09    | 11263.69 | 33.28  | 15762.0  |
- Speedup (reuse vs reparse): 4.13x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.02     | 0.02        | 0.02     | 2385997.91 | 1       | 2525.6   |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 1925877.40 | 1       | 2523.6   |
| Deep path navigation                        | strata              | 0.02     | 0.02        | 0.03     | 1870335.43 | 1       | 2540.6   |
| Extract all user IDs                        | jsonpath-ng         | 0.02     | 0.02        | 0.03     | 1881508.44 | 0       | 2434.2   |
| Extract all user IDs                        | jmespath            | 2.04     | 2.05        | 2.05     | 22444.94   | 4000    | 2434.2   |
| Extract all user IDs                        | strata              | 34.67    | 35.31       | 35.96    | 1302.02    | 4000    | 2429.5   |
| Extract all user IDs                        | strata_ndjson_fused | 84.93    | 85.17       | 85.41    | 539.87     | 4000    | 2434.2   |
| Extract all user IDs                        | strata_ndjson_full  | 87.15    | 89.20       | 91.25    | 515.47     | 4000    | 2433.9   |
| Extract all user names                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2011875.86 | 0       | 2526.8   |
| Extract all user names                      | jmespath            | 1.97     | 2.02        | 2.07     | 22771.93   | 4000    | 2526.8   |
| Extract all user names                      | strata              | 33.70    | 34.05       | 34.40    | 1350.27    | 4000    | 2526.5   |
| Extract all user names                      | strata_ndjson_full  | 78.31    | 80.08       | 81.85    | 574.15     | 4000    | 2526.8   |
| Extract all user names                      | strata_ndjson_fused | 81.30    | 82.61       | 83.91    | 556.61     | 4000    | 2526.8   |
| Extract nested timestamp field              | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2072309.44 | 0       | 2451.4   |
| Extract nested timestamp field              | jmespath            | 3.26     | 3.26        | 3.27     | 14095.10   | 4000    | 2451.4   |
| Extract nested timestamp field              | strata              | 34.94    | 36.57       | 38.20    | 1257.29    | 4000    | 2451.4   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 1697689.84 | 0       | 2535.2   |
| Extract order item prices (double wildcard) | jmespath            | 186.78   | 187.36      | 187.94   | 245.41     | 789913  | 2531.2   |
| Extract order item prices (double wildcard) | strata              | 1007.94  | 1016.82     | 1025.70  | 45.22      | 789913  | 2496.3   |
| Filter users by age (numeric predicate)     | jmespath            | 5.31     | 5.38        | 5.45     | 8544.73    | 3212    | 2528.3   |
| Filter users by age (numeric predicate)     | strata              | 35.07    | 35.71       | 36.35    | 1287.58    | 3212    | 2528.3   |
| NDJSON first record id                      | strata              | 0.02     | 0.02        | 0.02     | 2249810.75 | 1       | 1468.2   |
| NDJSON first record id                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2074273.90 | 1       | 1468.5   |
| NDJSON first record id                      | jmespath            | 0.02     | 0.03        | 0.03     | 1769833.93 | 1       | 1468.2   |
| Recursively find all prices                 | strata              | 1016.67  | 1030.42     | 1044.18  | 44.62      | 789913  | 2559.2   |
| Recursively find all prices                 | jsonpath-ng         | 3743.19  | 3747.36     | 3751.53  | 12.27      | 789913  | 2560.0   |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.02     | 1975865.24 | 10      | 2735.1   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 1599282.89 | 10      | 2733.1   |
| Slice first 10 users, extract IDs           | strata              | 0.04     | 0.05        | 0.05     | 1020358.61 | 10      | 2744.1   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2461.27  | 2478.75     | 2496.24  | 166.94 | 2750.3   |
| strata_cursor_reparse | 5826.31  | 5893.89     | 5961.47  | 70.21  | 4032.4   |
- Speedup (reuse vs reparse): 2.38x
