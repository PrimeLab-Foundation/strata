# Strata Benchmark Results

Generated: 2026-02-12 22:22:03

## Environment

- Commit: 63b52a91dc763c26eb4637cbc9ea21a3e346893a
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
| orjson        | users.json   | 154.646  | 229.249     | 229.514  | 5079.9   |
| msgspec       | users.json   | 165.958  | 242.141     | 249.139  | 3588.1   |
| ujson         | users.json   | 230.085  | 337.936     | 344.161  | 4242.4   |
| json (stdlib) | users.json   | 292.949  | 362.864     | 371.846  | 2757.6   |
| strata        | users.json   | 2712.855 | 2721.631    | 6018.712 | 5422.3   |
| orjson        | users.ndjson | 207.584  | 254.991     | 280.324  | 693.6    |
| msgspec       | users.ndjson | 216.622  | 263.643     | 264.462  | 677.3    |
| ujson         | users.ndjson | 297.016  | 368.475     | 420.922  | 800.8    |
| json (stdlib) | users.ndjson | 349.806  | 384.618     | 391.846  | 680.1    |
| strata        | users.ndjson | 419.589  | 493.814     | 530.018  | 642.1    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.738    | 4000    |
|                                     | jmespath    | 2.074    | 4000    |
|                                     | jsonpath-ng | 40.559   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 32.926   | 789913  |
|                                     | jmespath    | 245.902  | 4000    |
|                                     | jsonpath-ng | 1216.089 | 789913  |
| $..price                            | jmespath    | 242.694  | 4000    |
|                                     | query       | 291.607  | 789913  |
|                                     | jsonpath-ng | 4304.069 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.681    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 309.061  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 153.12   | 225.84      | 233.27   | 203.61 | 6208.6   |
| msgspec       | 171.56   | 240.31      | 242.69   | 191.35 | 5216.3   |
| ujson         | 241.90   | 337.04      | 337.79   | 136.43 | 3652.5   |
| json (stdlib) | 288.67   | 357.57      | 366.65   | 128.60 | 2872.8   |
| strata        | 2483.46  | 3369.20     | 5726.15  | 13.65  | 6873.3   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 212.89   | 259.34      | 263.31   | 177.29 | 1398.0   |
| orjson        | 209.56   | 260.28      | 264.20   | 176.65 | 1405.7   |
| strata        | 261.23   | 267.89      | 271.96   | 171.64 | 1406.4   |
| ujson         | 291.21   | 362.70      | 364.53   | 126.77 | 1520.6   |
| json (stdlib) | 397.38   | 402.62      | 457.87   | 114.20 | 1030.2   |
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
| Deep path navigation                        | jmespath    | 0.02     | 0.03        | 0.03     | 1754555.72 | 1       | 3495.1   |
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.03     | 1575448.40 | 1       | 3520.5   |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1344209.75 | 1       | 3495.3   |
| Extract all user IDs                        | jmespath    | 2.02     | 2.06        | 2.10     | 22309.63   | 4000    | 2305.1   |
| Extract all user IDs                        | strata      | 5.97     | 5.99        | 6.01     | 7675.23    | 4000    | 2602.5   |
| Extract all user IDs                        | jsonpath-ng | 39.94    | 40.02       | 40.11    | 1148.93    | 4000    | 1957.7   |
| Extract all user names                      | jmespath    | 1.80     | 1.83        | 1.86     | 25174.25   | 4000    | 3985.2   |
| Extract all user names                      | strata      | 5.33     | 5.42        | 5.51     | 8485.36    | 4000    | 4332.4   |
| Extract all user names                      | jsonpath-ng | 39.44    | 39.78       | 40.13    | 1155.80    | 4000    | 3713.2   |
| Extract nested timestamp field              | jmespath    | 3.08     | 3.10        | 3.13     | 14819.01   | 4000    | 2809.8   |
| Extract nested timestamp field              | strata      | 6.45     | 6.82        | 7.19     | 6740.54    | 4000    | 3035.3   |
| Extract nested timestamp field              | jsonpath-ng | 43.37    | 44.00       | 44.64    | 1044.98    | 4000    | 2626.3   |
| Extract order item prices (double wildcard) | jmespath    | 247.57   | 248.79      | 250.01   | 184.83     | 4000    | 2212.7   |
| Extract order item prices (double wildcard) | strata      | 1200.19  | 1214.83     | 1229.46  | 37.85      | 789913  | 2739.1   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1251.90  | 1277.25     | 1302.60  | 36.00      | 789913  | 738.0    |
| Filter users by age (numeric predicate)     | strata      | 4.99     | 5.19        | 5.39     | 8863.49    | 3212    | 4403.6   |
| Filter users by age (numeric predicate)     | jmespath    | 5.83     | 6.06        | 6.28     | 7592.04    | 3212    | 4393.7   |
| Recursively find all prices                 | strata      | 1065.36  | 1112.50     | 1159.64  | 41.33      | 789913  | 3517.1   |
| Recursively find all prices                 | jsonpath-ng | 4294.47  | 4406.27     | 4518.06  | 10.44      | 789913  | 361.3    |
| Slice first 10 users, extract IDs           | jmespath    | 0.03     | 0.03        | 0.04     | 1316952.95 | 10      | 4718.4   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.04        | 0.05     | 1124398.44 | 10      | 4991.6   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.05     | 0.05        | 0.05     | 960067.41  | 10      | 4212.9   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s  | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ----- | -------- |
| strata_cursor_reuse   | 4982.85  | 5273.41     | 5563.97  | 69.76 | 3501.0   |
| strata_cursor_reparse | 11157.12 | 11841.05    | 12524.97 | 31.07 | 12476.9  |
- Speedup (reuse vs reparse): 2.25x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.02        | 0.03     | 1864003.71 | 1       | 2511.5   |
| Deep path navigation                        | strata              | 0.03     | 0.03        | 0.03     | 1637238.74 | 1       | 2511.5   |
| Deep path navigation                        | jmespath            | 0.03     | 0.03        | 0.03     | 1626408.56 | 1       | 2511.5   |
| Extract all user IDs                        | strata              | 1.36     | 1.48        | 1.59     | 31128.94   | 0       | 2433.6   |
| Extract all user IDs                        | jmespath            | 2.19     | 2.19        | 2.19     | 20999.35   | 4000    | 2437.3   |
| Extract all user IDs                        | jsonpath-ng         | 35.95    | 35.99       | 36.03    | 1277.45    | 4000    | 2437.3   |
| Extract all user IDs                        | strata_ndjson_full  | 53.93    | 54.22       | 54.51    | 847.98     | 0       | 2436.7   |
| Extract all user IDs                        | strata_ndjson_fused | 56.72    | 57.85       | 58.98    | 794.79     | 0       | 2437.0   |
| Extract all user names                      | strata              | 1.40     | 1.50        | 1.59     | 30716.06   | 0       | 2541.8   |
| Extract all user names                      | jmespath            | 2.04     | 2.07        | 2.10     | 22223.90   | 4000    | 2542.1   |
| Extract all user names                      | jsonpath-ng         | 36.35    | 36.71       | 37.06    | 1252.60    | 4000    | 2542.1   |
| Extract all user names                      | strata_ndjson_fused | 50.35    | 54.20       | 58.06    | 848.31     | 0       | 2542.1   |
| Extract all user names                      | strata_ndjson_full  | 53.16    | 54.68       | 56.21    | 840.81     | 0       | 2542.0   |
| Extract nested timestamp field              | strata              | 1.46     | 1.49        | 1.52     | 30801.80   | 0       | 2441.0   |
| Extract nested timestamp field              | jmespath            | 3.29     | 3.33        | 3.36     | 13826.74   | 4000    | 2441.0   |
| Extract nested timestamp field              | jsonpath-ng         | 38.78    | 38.87       | 38.96    | 1182.86    | 4000    | 2441.0   |
| Extract order item prices (double wildcard) | strata              | 1.38     | 1.41        | 1.43     | 32699.85   | 0       | 2454.2   |
| Extract order item prices (double wildcard) | jmespath            | 188.93   | 190.99      | 193.06   | 240.74     | 789913  | 2488.9   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 1126.85  | 1139.12     | 1151.39  | 40.36      | 789913  | 2493.0   |
| Filter users by age (numeric predicate)     | strata              | 0.03     | 0.03        | 0.03     | 1429396.03 | 0       | 2545.6   |
| Filter users by age (numeric predicate)     | jmespath            | 5.68     | 5.69        | 5.69     | 8083.99    | 3212    | 2545.7   |
| NDJSON root field (id)                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2019207.56 | 0       | 1484.5   |
| NDJSON root field (id)                      | strata              | 36.13    | 37.25       | 38.36    | 1234.49    | 4000    | 1470.7   |
| NDJSON root field (id)                      | strata_ndjson_fused | 92.13    | 93.93       | 95.73    | 489.48     | 4000    | 1484.5   |
| NDJSON root field (id)                      | strata_ndjson_full  | 88.65    | 102.36      | 116.07   | 449.20     | 4000    | 1483.9   |
| Recursively find all prices                 | strata              | 1080.33  | 1081.92     | 1083.51  | 42.50      | 3898    | 2574.0   |
| Recursively find all prices                 | jsonpath-ng         | 3897.76  | 3908.72     | 3919.69  | 11.76      | 789913  | 2609.1   |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.03        | 0.04     | 1584348.66 | 10      | 2665.8   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.03     | 0.03        | 0.03     | 1354007.13 | 10      | 2663.8   |
| Slice first 10 users, extract IDs           | strata              | 0.04     | 0.04        | 0.05     | 1093120.57 | 10      | 2681.8   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 1517.91  | 1518.84     | 1519.77  | 272.45 | 2728.6   |
| strata_cursor_reparse | 5090.53  | 5128.21     | 5165.88  | 80.69  | 3819.4   |
- Speedup (reuse vs reparse): 3.38x
