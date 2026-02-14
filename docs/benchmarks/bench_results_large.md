# Strata Benchmark Results

Generated: 2026-02-14 17:09:41

## Environment

- Commit: 8d68dc2d967bbefb0f220f9b3733bf92fb7c966a
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
| orjson        | users.json   | 125.322  | 178.722     | 179.269  | 732.5    |
| msgspec       | users.json   | 132.447  | 184.945     | 188.982  | 724.0    |
| ujson         | users.json   | 185.251  | 256.065     | 257.328  | 846.4    |
| json (stdlib) | users.json   | 247.732  | 306.929     | 309.730  | 724.0    |
| strata        | users.json   | 291.110  | 316.317     | 318.544  | 594.4    |
| orjson        | users.ndjson | 168.822  | 203.010     | 205.798  | 740.9    |
| msgspec       | users.ndjson | 170.577  | 210.233     | 214.943  | 734.4    |
| ujson         | users.ndjson | 230.994  | 287.419     | 295.460  | 857.9    |
| json (stdlib) | users.ndjson | 286.693  | 327.141     | 327.590  | 737.1    |
| strata        | users.ndjson | 348.737  | 409.012     | 415.647  | 689.9    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.587    | 4000    |
|                                     | jmespath    | 1.582    | 4000    |
|                                     | jsonpath-ng | 31.782   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 28.115   | 789913  |
|                                     | jmespath    | 213.966  | 4000    |
|                                     | jsonpath-ng | 1032.549 | 789913  |
| $..price                            | jmespath    | 214.536  | 4000    |
|                                     | query       | 258.571  | 789913  |
|                                     | jsonpath-ng | 3657.416 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.572    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 277.336  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 124.37   | 175.83      | 178.09   | 261.52 | 731.8    |
| msgspec       | 134.23   | 184.76      | 185.50   | 248.88 | 723.3    |
| ujson         | 181.60   | 257.77      | 261.42   | 178.39 | 845.8    |
| json (stdlib) | 237.63   | 291.23      | 293.73   | 157.89 | 723.4    |
| strata        | 290.36   | 311.81      | 314.92   | 147.47 | 593.7    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 165.39   | 203.87      | 206.23   | 225.53 | 1370.8   |
| strata        | 204.19   | 210.56      | 212.33   | 218.37 | 1362.6   |
| msgspec       | 172.56   | 213.06      | 223.31   | 215.80 | 1362.0   |
| ujson         | 233.08   | 291.53      | 293.00   | 157.72 | 1484.6   |
| json (stdlib) | 280.52   | 319.58      | 324.10   | 143.87 | 1364.8   |
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
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2508163.43 | 1       | 9837.6   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 2151269.67 | 1       | 9886.6   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 1981316.94 | 1       | 9837.7   |
| Extract all user IDs                        | jmespath    | 1.68     | 1.72        | 1.75     | 26747.80   | 4000    | 5557.5   |
| Extract all user IDs                        | strata      | 5.02     | 5.11        | 5.20     | 8993.94    | 4000    | 5557.5   |
| Extract all user IDs                        | jsonpath-ng | 31.73    | 31.74       | 31.75    | 1448.74    | 4000    | 5559.4   |
| Extract all user names                      | jmespath    | 1.24     | 1.35        | 1.46     | 34022.40   | 4000    | 10830.7  |
| Extract all user names                      | strata      | 5.08     | 5.19        | 5.30     | 8857.55    | 4000    | 10830.7  |
| Extract all user names                      | jsonpath-ng | 31.41    | 31.44       | 31.47    | 1462.60    | 4000    | 10820.5  |
| Extract nested timestamp field              | jmespath    | 2.53     | 2.55        | 2.57     | 18014.30   | 4000    | 8115.8   |
| Extract nested timestamp field              | strata      | 5.52     | 5.57        | 5.63     | 8251.20    | 4000    | 8115.8   |
| Extract nested timestamp field              | jsonpath-ng | 33.79    | 33.85       | 33.90    | 1358.61    | 4000    | 8117.0   |
| Extract order item prices (double wildcard) | jmespath    | 217.44   | 226.19      | 234.94   | 203.30     | 4000    | 9044.1   |
| Extract order item prices (double wildcard) | strata      | 910.49   | 912.73      | 914.97   | 50.38      | 789913  | 9062.0   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1053.12  | 1054.55     | 1055.97  | 43.60      | 789913  | 8672.4   |
| Filter users by age (numeric predicate)     | strata      | 4.24     | 4.24        | 4.24     | 10853.03   | 3212    | 12901.0  |
| Filter users by age (numeric predicate)     | jmespath    | 4.55     | 4.60        | 4.65     | 9998.29    | 3212    | 12901.0  |
| Recursively find all prices                 | strata      | 1001.89  | 1008.23     | 1014.58  | 45.61      | 789913  | 14179.5  |
| Recursively find all prices                 | jsonpath-ng | 3696.38  | 3709.43     | 3722.48  | 12.40      | 789913  | 13223.2  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 2153436.48 | 10      | 13966.3  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.04     | 1528525.72 | 10      | 13921.2  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 1416682.77 | 10      | 14001.8  |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2660.32  | 2699.59     | 2738.87  | 136.27 | 14763.2  |
| strata_cursor_reparse | 10034.25 | 10044.68    | 10055.11 | 36.62  | 16518.8  |
- Speedup (reuse vs reparse): 3.72x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.02        | 0.02     | 2855154.18 | 1       | 2716.8   |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2291806.47 | 1       | 2714.8   |
| Deep path navigation                        | strata              | 0.02     | 0.02        | 0.02     | 2227036.44 | 1       | 2731.8   |
| Extract all user IDs                        | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 2836833.39 | 0       | 2393.9   |
| Extract all user IDs                        | jmespath            | 2.02     | 2.03        | 2.04     | 22691.86   | 4000    | 2393.9   |
| Extract all user IDs                        | strata              | 32.96    | 33.20       | 33.45    | 1384.77    | 4000    | 2389.2   |
| Extract all user IDs                        | strata_ndjson_fused | 80.02    | 81.57       | 83.12    | 563.67     | 4000    | 2393.8   |
| Extract all user IDs                        | strata_ndjson_full  | 83.36    | 83.76       | 84.17    | 548.92     | 4000    | 2393.6   |
| Extract all user names                      | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 2873710.53 | 0       | 2722.9   |
| Extract all user names                      | jmespath            | 1.96     | 1.96        | 1.97     | 23415.57   | 4000    | 2722.9   |
| Extract all user names                      | strata              | 33.02    | 33.08       | 33.14    | 1389.88    | 4000    | 2721.4   |
| Extract all user names                      | strata_ndjson_full  | 80.83    | 81.16       | 81.49    | 566.52     | 4000    | 2722.7   |
| Extract all user names                      | strata_ndjson_fused | 82.97    | 83.10       | 83.23    | 553.29     | 4000    | 2722.9   |
| Extract nested timestamp field              | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 2818746.22 | 0       | 2412.6   |
| Extract nested timestamp field              | jmespath            | 2.86     | 2.97        | 3.08     | 15477.04   | 4000    | 2412.6   |
| Extract nested timestamp field              | strata              | 34.23    | 34.26       | 34.29    | 1341.97    | 4000    | 2412.5   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 2950610.73 | 0       | 2504.9   |
| Extract order item prices (double wildcard) | jmespath            | 177.09   | 179.41      | 181.73   | 256.28     | 789913  | 2500.9   |
| Extract order item prices (double wildcard) | strata              | 960.97   | 970.54      | 980.11   | 47.38      | 789913  | 2468.8   |
| Filter users by age (numeric predicate)     | jmespath            | 5.15     | 5.18        | 5.21     | 8875.62    | 3212    | 2727.8   |
| Filter users by age (numeric predicate)     | strata              | 32.40    | 32.48       | 32.56    | 1415.79    | 3212    | 2727.8   |
| NDJSON first record id                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2130300.81 | 1       | 1427.5   |
| NDJSON first record id                      | jmespath            | 0.02     | 0.02        | 0.02     | 1914132.28 | 1       | 1427.0   |
| NDJSON first record id                      | strata              | 0.03     | 0.03        | 0.03     | 1557514.96 | 1       | 1427.0   |
| Recursively find all prices                 | strata              | 995.47   | 996.38      | 997.28   | 46.15      | 789913  | 2748.8   |
| Recursively find all prices                 | jsonpath-ng         | 3639.59  | 3640.79     | 3642.00  | 12.63      | 789913  | 2783.2   |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.02     | 2047304.81 | 10      | 3036.7   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 1587768.12 | 10      | 3034.7   |
| Slice first 10 users, extract IDs           | strata              | 0.03     | 0.03        | 0.03     | 1534743.47 | 10      | 3045.7   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2339.83  | 2343.63     | 2347.44  | 176.57 | 3046.9   |
| strata_cursor_reparse | 5678.32  | 5687.61     | 5696.90  | 72.76  | 4111.3   |
- Speedup (reuse vs reparse): 2.43x
