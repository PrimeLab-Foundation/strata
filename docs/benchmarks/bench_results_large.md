# Strata Benchmark Results

Generated: 2026-02-14 13:29:44

## Environment

- Commit: 9db40c620a2b0b66c54eb6b960270e5e11f2dd93
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
| orjson        | users.json   | 141.341  | 196.163     | 205.447  | 731.7    |
| msgspec       | users.json   | 147.587  | 208.533     | 209.281  | 724.2    |
| ujson         | users.json   | 206.660  | 287.554     | 292.311  | 846.6    |
| json (stdlib) | users.json   | 269.703  | 328.005     | 332.176  | 724.2    |
| strata        | users.json   | 309.485  | 336.073     | 336.975  | 593.6    |
| orjson        | users.ndjson | 199.607  | 236.218     | 238.913  | 742.3    |
| msgspec       | users.ndjson | 201.790  | 241.460     | 244.800  | 735.8    |
| ujson         | users.ndjson | 272.757  | 335.548     | 338.726  | 859.3    |
| json (stdlib) | users.ndjson | 324.607  | 370.235     | 380.920  | 738.6    |
| strata        | users.ndjson | 374.289  | 456.716     | 458.801  | 690.2    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.688    | 4000    |
|                                     | jmespath    | 1.970    | 4000    |
|                                     | jsonpath-ng | 38.611   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 31.093   | 789913  |
|                                     | jmespath    | 232.796  | 4000    |
|                                     | jsonpath-ng | 1163.276 | 789913  |
| $..price                            | jmespath    | 237.056  | 4000    |
|                                     | query       | 288.555  | 789913  |
|                                     | jsonpath-ng | 3676.562 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.622    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 285.727  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 138.35   | 192.50      | 192.84   | 238.88 | 732.0    |
| msgspec       | 139.73   | 199.65      | 203.50   | 230.32 | 723.5    |
| ujson         | 197.03   | 278.80      | 279.89   | 164.93 | 846.0    |
| json (stdlib) | 253.56   | 314.84      | 320.99   | 146.05 | 723.6    |
| strata        | 303.31   | 319.50      | 321.34   | 143.92 | 594.0    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 190.62   | 229.30      | 231.26   | 200.52 | 1402.7   |
| msgspec       | 203.59   | 238.93      | 245.93   | 192.44 | 1395.0   |
| strata        | 236.17   | 240.35      | 243.16   | 191.30 | 1403.5   |
| ujson         | 270.55   | 329.17      | 334.82   | 139.68 | 1517.6   |
| json (stdlib) | 321.78   | 365.68      | 367.22   | 125.74 | 1397.8   |
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
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 2460649.37 | 1       | 12337.4  |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2161788.10 | 1       | 12316.4  |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1544569.71 | 1       | 12316.4  |
| Extract all user IDs                        | jmespath    | 2.05     | 2.07        | 2.10     | 22176.93   | 4000    | 5557.3   |
| Extract all user IDs                        | strata      | 5.70     | 5.78        | 5.86     | 7949.67    | 4000    | 5557.3   |
| Extract all user IDs                        | jsonpath-ng | 39.08    | 39.23       | 39.39    | 1172.00    | 4000    | 5559.3   |
| Extract all user names                      | jmespath    | 1.46     | 1.47        | 1.48     | 31286.11   | 4000    | 12177.8  |
| Extract all user names                      | strata      | 4.75     | 4.91        | 5.08     | 9356.56    | 4000    | 12187.9  |
| Extract all user names                      | jsonpath-ng | 36.93    | 37.45       | 37.96    | 1227.97    | 4000    | 12144.0  |
| Extract nested timestamp field              | jmespath    | 2.89     | 3.04        | 3.20     | 15114.93   | 4000    | 10420.0  |
| Extract nested timestamp field              | strata      | 5.97     | 6.21        | 6.46     | 7400.39    | 4000    | 10420.0  |
| Extract nested timestamp field              | jsonpath-ng | 40.71    | 41.26       | 41.81    | 1114.37    | 4000    | 10421.2  |
| Extract order item prices (double wildcard) | jmespath    | 240.48   | 241.54      | 242.61   | 190.37     | 4000    | 12342.9  |
| Extract order item prices (double wildcard) | strata      | 985.07   | 995.37      | 1005.66  | 46.20      | 789913  | 12334.9  |
| Extract order item prices (double wildcard) | jsonpath-ng | 1192.94  | 1199.25     | 1205.56  | 38.34      | 789913  | 11961.9  |
| Filter users by age (numeric predicate)     | strata      | 4.83     | 4.85        | 4.87     | 9481.72    | 3212    | 12376.3  |
| Filter users by age (numeric predicate)     | jmespath    | 5.04     | 5.08        | 5.12     | 9051.77    | 3212    | 12376.4  |
| Recursively find all prices                 | strata      | 1079.45  | 1080.95     | 1082.46  | 42.54      | 789913  | 13571.0  |
| Recursively find all prices                 | jsonpath-ng | 4155.94  | 4163.27     | 4170.60  | 11.05      | 789913  | 13151.3  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 1818172.05 | 10      | 14738.4  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1344209.75 | 10      | 14738.4  |
| Slice first 10 users, extract IDs           | strata      | 0.04     | 0.05        | 0.05     | 1002821.86 | 10      | 14753.4  |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2807.97  | 2845.57     | 2883.16  | 129.28 | 15704.8  |
| strata_cursor_reparse | 8325.62  | 9126.58     | 9927.53  | 40.31  | 17366.0  |
- Speedup (reuse vs reparse): 3.21x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.02     | 0.02        | 0.02     | 2209166.53 | 1       | 2507.2   |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 1735104.09 | 1       | 2506.2   |
| Deep path navigation                        | strata              | 0.02     | 0.03        | 0.03     | 1684751.17 | 1       | 2523.2   |
| Extract all user IDs                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2070397.52 | 0       | 2436.0   |
| Extract all user IDs                        | jmespath            | 2.34     | 2.41        | 2.48     | 19068.37   | 4000    | 2435.9   |
| Extract all user IDs                        | strata              | 39.50    | 40.25       | 41.00    | 1142.21    | 4000    | 2431.0   |
| Extract all user IDs                        | strata_ndjson_fused | 86.01    | 88.26       | 90.50    | 520.97     | 4000    | 2435.9   |
| Extract all user IDs                        | strata_ndjson_full  | 87.50    | 88.71       | 89.93    | 518.28     | 4000    | 2435.7   |
| Extract all user names                      | jsonpath-ng         | 0.02     | 0.02        | 0.03     | 1902608.59 | 0       | 2509.2   |
| Extract all user names                      | jmespath            | 2.22     | 2.69        | 3.15     | 17115.37   | 4000    | 2509.2   |
| Extract all user names                      | strata              | 38.70    | 39.72       | 40.75    | 1157.53    | 4000    | 2508.3   |
| Extract all user names                      | strata_ndjson_full  | 85.52    | 86.88       | 88.25    | 529.20     | 4000    | 2509.1   |
| Extract all user names                      | strata_ndjson_fused | 95.54    | 95.71       | 95.88    | 480.40     | 4000    | 2509.2   |
| Extract nested timestamp field              | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2474270.42 | 0       | 2442.2   |
| Extract nested timestamp field              | jmespath            | 3.28     | 3.39        | 3.50     | 13564.16   | 4000    | 2442.2   |
| Extract nested timestamp field              | strata              | 40.84    | 41.08       | 41.32    | 1119.28    | 4000    | 2442.2   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2303690.99 | 0       | 2510.2   |
| Extract order item prices (double wildcard) | jmespath            | 198.04   | 200.64      | 203.25   | 229.16     | 789913  | 2506.2   |
| Extract order item prices (double wildcard) | strata              | 1076.38  | 1090.16     | 1103.93  | 42.18      | 789913  | 2472.4   |
| Filter users by age (numeric predicate)     | jmespath            | 5.79     | 5.80        | 5.80     | 7930.73    | 3212    | 2510.6   |
| Filter users by age (numeric predicate)     | strata              | 37.69    | 38.56       | 39.42    | 1192.45    | 3212    | 2510.6   |
| NDJSON first record id                      | jmespath            | 0.02     | 0.02        | 0.02     | 2095926.37 | 1       | 1468.9   |
| NDJSON first record id                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2064540.57 | 1       | 1469.2   |
| NDJSON first record id                      | strata              | 0.02     | 0.04        | 0.06     | 1107937.07 | 1       | 1468.9   |
| Recursively find all prices                 | strata              | 1114.06  | 1131.34     | 1148.62  | 40.64      | 789913  | 2531.2   |
| Recursively find all prices                 | jsonpath-ng         | 4162.27  | 4162.79     | 4163.31  | 11.05      | 789913  | 2532.0   |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.04        | 0.05     | 1234346.12 | 10      | 2528.0   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.03     | 0.04        | 0.04     | 1185921.98 | 10      | 2526.0   |
| Slice first 10 users, extract IDs           | strata              | 0.04     | 0.05        | 0.06     | 911990.80  | 10      | 2537.0   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2684.93  | 2685.75     | 2686.57  | 154.08 | 2532.6   |
| strata_cursor_reparse | 5988.62  | 6202.83     | 6417.03  | 66.71  | 3607.2   |
- Speedup (reuse vs reparse): 2.31x
