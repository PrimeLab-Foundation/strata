# Strata Benchmark Results

Generated: 2026-02-13 00:11:00

## Environment

- Commit: 454fe03a16725c7bac048e9e5926d9bf888849a6
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
| msgspec       | users.json   | 141.061  | 193.202     | 198.170  | 11465.9  |
| orjson        | users.json   | 132.474  | 228.469     | 262.466  | 11746.6  |
| ujson         | users.json   | 193.162  | 269.909     | 299.064  | 11588.3  |
| json (stdlib) | users.json   | 255.542  | 311.097     | 336.982  | 11408.0  |
| strata        | users.json   | 1696.114 | 1829.339    | 3877.247 | 12805.0  |
| orjson        | users.ndjson | 179.451  | 223.556     | 225.997  | 8614.9   |
| msgspec       | users.ndjson | 194.327  | 237.314     | 246.446  | 8283.1   |
| ujson         | users.ndjson | 277.040  | 321.709     | 343.172  | 8406.6   |
| json (stdlib) | users.ndjson | 299.881  | 345.920     | 364.487  | 8285.8   |
| strata        | users.ndjson | 390.932  | 437.654     | 438.530  | 8565.5   |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.612    | 4000    |
|                                     | jmespath    | 1.678    | 4000    |
|                                     | jsonpath-ng | 33.274   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 28.282   | 789913  |
|                                     | jmespath    | 230.488  | 4000    |
|                                     | jsonpath-ng | 1129.504 | 789913  |
| $..price                            | jmespath    | 225.010  | 4000    |
|                                     | query       | 276.695  | 789913  |
|                                     | jsonpath-ng | 3879.323 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.610    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 285.304  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 148.85   | 192.37      | 204.18   | 239.04 | 10590.4  |
| msgspec       | 142.83   | 206.54      | 212.74   | 222.64 | 10575.0  |
| ujson         | 208.11   | 297.87      | 301.70   | 154.37 | 10103.2  |
| json (stdlib) | 250.78   | 309.50      | 314.65   | 148.57 | 10024.6  |
| strata        | 1719.69  | 1812.59     | 2194.26  | 25.37  | 10823.0  |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| strata        | 211.30   | 214.40      | 217.31   | 214.45 | 1365.8   |
| orjson        | 175.17   | 215.74      | 220.38   | 213.13 | 1372.9   |
| msgspec       | 188.05   | 225.28      | 226.67   | 204.10 | 1365.2   |
| ujson         | 250.50   | 307.34      | 313.11   | 149.61 | 1487.7   |
| json (stdlib) | 289.19   | 339.83      | 351.40   | 135.30 | 1368.0   |
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
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2306377.29 | 1       | 8553.6   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 2266141.17 | 1       | 8574.6   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1865754.03 | 1       | 8553.6   |
| Extract all user IDs                        | jmespath    | 1.80     | 1.96        | 2.12     | 23447.95   | 4000    | 5558.4   |
| Extract all user IDs                        | strata      | 4.67     | 4.90        | 5.13     | 9378.62    | 4000    | 5558.4   |
| Extract all user IDs                        | jsonpath-ng | 35.07    | 36.30       | 37.52    | 1266.89    | 4000    | 5560.3   |
| Extract all user names                      | jmespath    | 1.41     | 1.46        | 1.50     | 31582.46   | 4000    | 10049.6  |
| Extract all user names                      | strata      | 4.62     | 4.73        | 4.84     | 9721.95    | 4000    | 10092.8  |
| Extract all user names                      | jsonpath-ng | 33.27    | 34.28       | 35.28    | 1341.59    | 4000    | 10049.6  |
| Extract nested timestamp field              | jmespath    | 2.89     | 2.96        | 3.02     | 15548.06   | 4000    | 7781.8   |
| Extract nested timestamp field              | strata      | 5.73     | 5.77        | 5.81     | 7968.87    | 4000    | 7781.8   |
| Extract nested timestamp field              | jsonpath-ng | 36.18    | 36.40       | 36.62    | 1263.28    | 4000    | 7783.4   |
| Extract order item prices (double wildcard) | jmespath    | 224.78   | 225.20      | 225.63   | 204.19     | 4000    | 7500.7   |
| Extract order item prices (double wildcard) | strata      | 893.55   | 894.70      | 895.85   | 51.40      | 789913  | 7615.4   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1130.02  | 1141.05     | 1152.09  | 40.30      | 789913  | 6845.7   |
| Filter users by age (numeric predicate)     | strata      | 4.79     | 4.86        | 4.93     | 9462.25    | 3212    | 11691.0  |
| Filter users by age (numeric predicate)     | jmespath    | 5.09     | 5.19        | 5.29     | 8856.55    | 3212    | 11674.3  |
| Recursively find all prices                 | strata      | 1091.28  | 1093.64     | 1096.00  | 42.05      | 789913  | 11870.3  |
| Recursively find all prices                 | jsonpath-ng | 3979.80  | 3981.07     | 3982.33  | 11.55      | 789913  | 10853.9  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 1897865.65 | 10      | 11103.9  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 1494374.42 | 10      | 11261.6  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.04     | 0.04        | 0.04     | 1070401.90 | 10      | 10644.1  |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2860.05  | 2959.15     | 3058.24  | 124.32 | 11723.3  |
| strata_cursor_reparse | 11369.07 | 12061.86    | 12754.65 | 30.50  | 11004.6  |
- Speedup (reuse vs reparse): 4.08x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata              | 0.01     | 0.02        | 0.02     | 2851522.32 | 1       | 2735.4   |
| Deep path navigation                        | jmespath            | 0.02     | 0.02        | 0.02     | 2398883.05 | 1       | 2735.4   |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 1999102.98 | 1       | 2735.4   |
| Extract all user IDs                        | strata              | 1.27     | 1.28        | 1.29     | 35875.27   | 0       | 2434.4   |
| Extract all user IDs                        | jmespath            | 1.99     | 2.03        | 2.07     | 22680.67   | 4000    | 2436.7   |
| Extract all user IDs                        | jsonpath-ng         | 34.04    | 35.68       | 37.31    | 1288.70    | 4000    | 2436.8   |
| Extract all user IDs                        | strata_ndjson_fused | 50.80    | 51.70       | 52.60    | 889.37     | 0       | 2436.7   |
| Extract all user IDs                        | strata_ndjson_full  | 51.95    | 51.99       | 52.02    | 884.44     | 0       | 2436.2   |
| Extract all user names                      | strata              | 1.32     | 1.47        | 1.62     | 31222.74   | 0       | 2752.2   |
| Extract all user names                      | jmespath            | 1.94     | 1.94        | 1.94     | 23698.93   | 4000    | 2753.4   |
| Extract all user names                      | jsonpath-ng         | 33.00    | 33.17       | 33.35    | 1386.03    | 4000    | 2753.4   |
| Extract all user names                      | strata_ndjson_fused | 50.45    | 50.78       | 51.10    | 905.55     | 0       | 2753.4   |
| Extract all user names                      | strata_ndjson_full  | 50.87    | 51.35       | 51.83    | 895.42     | 0       | 2753.1   |
| Extract nested timestamp field              | strata              | 1.42     | 1.42        | 1.43     | 32345.20   | 0       | 2441.3   |
| Extract nested timestamp field              | jmespath            | 3.13     | 3.23        | 3.33     | 14214.38   | 4000    | 2441.3   |
| Extract nested timestamp field              | jsonpath-ng         | 36.10    | 36.78       | 37.47    | 1250.03    | 4000    | 2441.3   |
| Extract order item prices (double wildcard) | strata              | 1.48     | 1.50        | 1.52     | 30684.47   | 0       | 2461.2   |
| Extract order item prices (double wildcard) | jmespath            | 180.19   | 180.86      | 181.53   | 254.23     | 789913  | 2494.5   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 1067.78  | 1068.64     | 1069.50  | 43.03      | 789913  | 2500.6   |
| Filter users by age (numeric predicate)     | strata              | 0.03     | 0.03        | 0.03     | 1628770.73 | 0       | 2757.5   |
| Filter users by age (numeric predicate)     | jmespath            | 5.22     | 5.24        | 5.27     | 8774.20    | 3212    | 2757.5   |
| NDJSON root field (id)                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 1995460.00 | 0       | 1483.9   |
| NDJSON root field (id)                      | strata              | 34.48    | 34.56       | 34.63    | 1330.53    | 4000    | 1470.6   |
| NDJSON root field (id)                      | strata_ndjson_fused | 85.50    | 86.82       | 88.13    | 529.62     | 4000    | 1483.9   |
| NDJSON root field (id)                      | strata_ndjson_full  | 86.24    | 88.27       | 90.31    | 520.89     | 4000    | 1483.1   |
| Recursively find all prices                 | strata              | 1052.57  | 1056.58     | 1060.59  | 43.52      | 3898    | 2784.4   |
| Recursively find all prices                 | jsonpath-ng         | 3817.76  | 3909.62     | 4001.48  | 11.76      | 789913  | 2812.2   |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.03     | 1968759.11 | 10      | 2834.3   |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.03        | 0.03     | 1679582.00 | 10      | 2849.3   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 1587767.73 | 10      | 2832.3   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 1514.74  | 1522.09     | 1529.45  | 271.87 | 2814.8   |
| strata_cursor_reparse | 5029.28  | 5071.25     | 5113.23  | 81.60  | 4141.3   |
- Speedup (reuse vs reparse): 3.33x
