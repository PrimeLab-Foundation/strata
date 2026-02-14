# Strata Benchmark Results

Generated: 2026-02-14 17:45:01

## Environment

- Commit: 479cf62079eea032a9baeadd1f59e2c2c2375686
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
| orjson        | users.json   | 133.500  | 180.013     | 184.570  | 733.4    |
| msgspec       | users.json   | 133.827  | 188.490     | 192.972  | 724.9    |
| ujson         | users.json   | 187.891  | 263.340     | 264.007  | 847.3    |
| json (stdlib) | users.json   | 244.502  | 301.882     | 303.887  | 724.9    |
| strata        | users.json   | 306.041  | 326.940     | 329.578  | 595.3    |
| orjson        | users.ndjson | 170.700  | 211.593     | 214.511  | 742.9    |
| msgspec       | users.ndjson | 176.866  | 232.401     | 233.107  | 736.5    |
| ujson         | users.ndjson | 239.428  | 294.145     | 300.409  | 860.0    |
| json (stdlib) | users.ndjson | 293.780  | 332.877     | 334.535  | 739.2    |
| strata        | users.ndjson | 363.706  | 432.815     | 446.999  | 692.9    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.587    | 4000    |
|                                     | jmespath    | 1.535    | 4000    |
|                                     | jsonpath-ng | 32.297   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 28.400   | 789913  |
|                                     | jmespath    | 219.468  | 4000    |
|                                     | jsonpath-ng | 1045.512 | 789913  |
| $..price                            | jmespath    | 218.518  | 4000    |
|                                     | query       | 266.169  | 789913  |
|                                     | jsonpath-ng | 3737.317 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.599    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 280.648  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 126.18   | 182.57      | 182.70   | 251.86 | 732.3    |
| msgspec       | 138.15   | 189.18      | 189.99   | 243.07 | 723.8    |
| ujson         | 188.57   | 262.56      | 265.07   | 175.14 | 846.4    |
| json (stdlib) | 244.07   | 299.41      | 300.36   | 153.58 | 723.9    |
| strata        | 301.79   | 325.30      | 326.23   | 141.36 | 594.2    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 168.97   | 210.35      | 216.05   | 218.59 | 1373.4   |
| strata        | 201.81   | 215.48      | 218.26   | 213.38 | 1365.7   |
| msgspec       | 174.75   | 218.75      | 221.05   | 210.19 | 1364.6   |
| ujson         | 240.92   | 304.19      | 306.61   | 151.15 | 1487.2   |
| json (stdlib) | 302.49   | 332.65      | 340.49   | 138.22 | 1367.4   |
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
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 2584496.11 | 1       | 9589.5   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 2373273.88 | 1       | 9610.5   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2108120.68 | 1       | 9428.5   |
| Extract all user IDs                        | jmespath    | 1.69     | 1.70        | 1.71     | 27036.13   | 4000    | 5557.8   |
| Extract all user IDs                        | strata      | 5.09     | 5.19        | 5.29     | 8861.67    | 4000    | 5557.5   |
| Extract all user IDs                        | jsonpath-ng | 32.09    | 32.10       | 32.10    | 1432.67    | 4000    | 5559.9   |
| Extract all user names                      | jmespath    | 1.29     | 1.29        | 1.29     | 35673.70   | 4000    | 10416.2  |
| Extract all user names                      | strata      | 4.41     | 4.41        | 4.42     | 10415.81   | 4000    | 10446.7  |
| Extract all user names                      | jsonpath-ng | 31.87    | 32.22       | 32.58    | 1426.96    | 4000    | 10321.5  |
| Extract nested timestamp field              | jmespath    | 2.54     | 2.54        | 2.55     | 18077.01   | 4000    | 8919.8   |
| Extract nested timestamp field              | strata      | 5.68     | 5.71        | 5.74     | 8057.22    | 4000    | 8929.2   |
| Extract nested timestamp field              | jsonpath-ng | 34.56    | 34.75       | 34.94    | 1323.16    | 4000    | 8921.1   |
| Extract order item prices (double wildcard) | jmespath    | 218.14   | 218.90      | 219.66   | 210.07     | 4000    | 9095.7   |
| Extract order item prices (double wildcard) | strata      | 960.97   | 962.26      | 963.54   | 47.79      | 789913  | 9081.8   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1078.82  | 1081.17     | 1083.53  | 42.53      | 789913  | 8731.3   |
| Filter users by age (numeric predicate)     | strata      | 4.72     | 4.83        | 4.94     | 9522.38    | 3212    | 12484.5  |
| Filter users by age (numeric predicate)     | jmespath    | 4.87     | 4.93        | 4.99     | 9327.30    | 3212    | 12478.3  |
| Recursively find all prices                 | strata      | 922.09   | 929.85      | 937.62   | 49.45      | 789913  | 13632.3  |
| Recursively find all prices                 | jsonpath-ng | 3754.17  | 3788.75     | 3823.34  | 12.14      | 789913  | 13054.1  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 2039899.22 | 10      | 14652.2  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1397841.47 | 10      | 14647.3  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.04     | 1366702.77 | 10      | 14667.2  |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2627.48  | 2793.69     | 2959.90  | 131.68 | 15090.7  |
| strata_cursor_reparse | 10843.09 | 11007.89    | 11172.68 | 33.42  | 15981.3  |
- Speedup (reuse vs reparse): 3.94x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.02     | 0.02        | 0.02     | 2313429.14 | 1       | 2475.5   |
| Deep path navigation                        | strata              | 0.02     | 0.02        | 0.02     | 2064587.11 | 1       | 2491.5   |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 1995502.85 | 1       | 2473.5   |
| Extract all user IDs                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2189494.49 | 0       | 2394.9   |
| Extract all user IDs                        | jmespath            | 2.01     | 2.03        | 2.04     | 22697.46   | 4000    | 2394.9   |
| Extract all user IDs                        | strata              | 32.77    | 33.15       | 33.54    | 1386.90    | 4000    | 2389.0   |
| Extract all user IDs                        | strata_ndjson_fused | 81.11    | 82.75       | 84.39    | 555.65     | 4000    | 2394.7   |
| Extract all user IDs                        | strata_ndjson_full  | 84.55    | 84.90       | 85.25    | 541.59     | 4000    | 2394.2   |
| Extract all user names                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2213634.01 | 0       | 2488.6   |
| Extract all user names                      | jmespath            | 1.92     | 2.00        | 2.08     | 22976.29   | 4000    | 2488.6   |
| Extract all user names                      | strata              | 33.75    | 33.79       | 33.82    | 1360.79    | 4000    | 2487.7   |
| Extract all user names                      | strata_ndjson_full  | 79.60    | 82.01       | 84.41    | 560.67     | 4000    | 2488.4   |
| Extract all user names                      | strata_ndjson_fused | 83.76    | 83.90       | 84.05    | 547.99     | 4000    | 2488.6   |
| Extract nested timestamp field              | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2330487.20 | 0       | 2402.2   |
| Extract nested timestamp field              | jmespath            | 3.10     | 3.15        | 3.21     | 14580.43   | 4000    | 2402.2   |
| Extract nested timestamp field              | strata              | 34.03    | 34.10       | 34.17    | 1348.47    | 4000    | 2402.2   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2263599.42 | 0       | 2482.3   |
| Extract order item prices (double wildcard) | jmespath            | 182.44   | 185.26      | 188.09   | 248.18     | 789913  | 2478.3   |
| Extract order item prices (double wildcard) | strata              | 1007.82  | 1012.94     | 1018.05  | 45.39      | 789913  | 2446.3   |
| Filter users by age (numeric predicate)     | jmespath            | 5.20     | 5.23        | 5.27     | 8785.34    | 3212    | 2492.5   |
| Filter users by age (numeric predicate)     | strata              | 32.45    | 33.12       | 33.80    | 1388.06    | 3212    | 2492.5   |
| NDJSON first record id                      | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 2643176.16 | 1       | 1427.7   |
| NDJSON first record id                      | jmespath            | 0.02     | 0.02        | 0.02     | 2474205.52 | 1       | 1427.3   |
| NDJSON first record id                      | strata              | 0.02     | 0.02        | 0.03     | 2028515.87 | 1       | 1427.3   |
| Recursively find all prices                 | strata              | 971.18   | 1000.71     | 1030.25  | 45.95      | 789913  | 2511.7   |
| Recursively find all prices                 | jsonpath-ng         | 3734.36  | 3743.95     | 3753.54  | 12.28      | 789913  | 2509.9   |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.02     | 1946176.12 | 10      | 2680.7   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.03     | 0.03        | 0.03     | 1529485.87 | 10      | 2678.7   |
| Slice first 10 users, extract IDs           | strata              | 0.03     | 0.04        | 0.04     | 1271307.82 | 10      | 2691.7   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2422.61  | 2425.91     | 2429.21  | 170.58 | 2691.5   |
| strata_cursor_reparse | 5862.56  | 5874.63     | 5886.70  | 70.44  | 3791.7   |
- Speedup (reuse vs reparse): 2.42x
