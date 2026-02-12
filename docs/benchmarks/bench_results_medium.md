# Strata Benchmark Results

Generated: 2026-02-13 01:34:52

## Environment

- Commit: 9b7be5525389f90d56aeaa6b54e263bfaac39521
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/medium/users.json, benchmarks/data/generated/medium/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users.json   | 19.459   | 28.006      | 29.030   | 2714.8   |
| msgspec       | users.json   | 20.189   | 28.550      | 30.386   | 2714.7   |
| ujson         | users.json   | 28.213   | 43.016      | 44.254   | 2733.3   |
| json (stdlib) | users.json   | 36.226   | 43.474      | 44.071   | 2714.7   |
| strata        | users.json   | 72.500   | 86.582      | 125.080  | 2693.2   |
| orjson        | users.ndjson | 21.871   | 23.384      | 24.710   | 2696.0   |
| msgspec       | users.ndjson | 22.720   | 24.186      | 25.491   | 2696.8   |
| ujson         | users.ndjson | 31.741   | 34.155      | 34.888   | 2714.5   |
| json (stdlib) | users.ndjson | 38.831   | 39.411      | 40.311   | 2698.7   |
| strata        | users.ndjson | 49.540   | 57.803      | 59.633   | 2729.7   |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.143    | 2000    |
|                                     | jmespath    | 0.453    | 2000    |
|                                     | jsonpath-ng | 6.175    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 3.841    | 100999  |
|                                     | jmespath    | 31.955   | 2000    |
|                                     | jsonpath-ng | 128.008  | 100999  |
| $..price                            | jmespath    | 33.278   | 2000    |
|                                     | query       | 36.394   | 100999  |
|                                     | jsonpath-ng | 474.089  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.174    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 38.799   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 20.95    | 28.38       | 29.38    | 230.87 | 2775.3   |
| orjson        | 19.93    | 29.60       | 31.06    | 221.34 | 2774.6   |
| ujson         | 27.40    | 38.18       | 39.04    | 171.59 | 2793.1   |
| json (stdlib) | 34.83    | 42.14       | 43.54    | 155.49 | 2775.4   |
| strata        | 87.07    | 88.55       | 136.84   | 73.99  | 2754.0   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 22.37    | 24.03       | 24.08    | 272.52 | 144.2    |
| msgspec       | 23.83    | 24.74       | 25.41    | 264.69 | 144.0    |
| ujson         | 32.18    | 34.81       | 34.96    | 188.14 | 161.9    |
| json (stdlib) | 38.69    | 40.32       | 40.71    | 162.43 | 145.0    |
| strata        | 46.88    | 49.41       | 49.55    | 132.54 | 143.0    |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 533914.24 | 1       | 1228.4   |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 507212.79 | 1       | 1227.4   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 454456.09 | 1       | 1227.4   |
| Extract all user IDs                        | jmespath    | 0.57     | 0.62        | 0.67     | 10541.70  | 2000    | 553.7    |
| Extract all user IDs                        | strata      | 2.41     | 2.42        | 2.43     | 2703.02   | 2000    | 553.5    |
| Extract all user IDs                        | jsonpath-ng | 6.89     | 7.04        | 7.20     | 929.99    | 2000    | 554.7    |
| Extract all user names                      | jmespath    | 0.48     | 0.48        | 0.48     | 13599.70  | 2000    | 1310.4   |
| Extract all user names                      | strata      | 2.49     | 2.50        | 2.51     | 2620.40   | 2000    | 1310.4   |
| Extract all user names                      | jsonpath-ng | 6.98     | 7.04        | 7.10     | 930.33    | 2000    | 1310.4   |
| Extract nested timestamp field              | jmespath    | 1.20     | 1.21        | 1.21     | 5431.15   | 2000    | 986.2    |
| Extract nested timestamp field              | strata      | 2.41     | 2.42        | 2.43     | 2706.44   | 2000    | 986.0    |
| Extract nested timestamp field              | jsonpath-ng | 7.64     | 7.85        | 8.07     | 834.28    | 2000    | 986.8    |
| Extract order item prices (double wildcard) | jmespath    | 32.30    | 32.82       | 33.33    | 199.65    | 2000    | 1137.1   |
| Extract order item prices (double wildcard) | strata      | 117.65   | 118.12      | 118.59   | 55.47     | 100999  | 1135.1   |
| Extract order item prices (double wildcard) | jsonpath-ng | 128.63   | 129.10      | 129.58   | 50.75     | 100999  | 1140.6   |
| Filter users by age (numeric predicate)     | strata      | 2.28     | 2.32        | 2.36     | 2825.54   | 1606    | 1706.1   |
| Filter users by age (numeric predicate)     | jmespath    | 2.34     | 2.34        | 2.34     | 2802.18   | 1606    | 1706.1   |
| Recursively find all prices                 | strata      | 125.57   | 125.88      | 126.19   | 52.05     | 100999  | 2107.6   |
| Recursively find all prices                 | jsonpath-ng | 474.68   | 476.19      | 477.71   | 13.76     | 100999  | 2111.6   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 322885.20 | 10      | 2156.1   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 278302.75 | 10      | 2156.1   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 275135.59 | 10      | 2156.1   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 300.66   | 305.46      | 310.26   | 171.59 | 2347.0   |
| strata_cursor_reparse | 727.07   | 729.64      | 732.22   | 71.83  | 3890.3   |
- Speedup (reuse vs reparse): 2.39x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.01        | 0.01     | 548662.20 | 1       | 224.6    |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.01     | 527495.53 | 1       | 224.6    |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 456294.56 | 1       | 224.6    |
| Extract all user IDs                        | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 438470.57 | 0       | 213.8    |
| Extract all user IDs                        | jmespath            | 0.68     | 0.70        | 0.72     | 9336.92   | 2000    | 213.8    |
| Extract all user IDs                        | strata              | 6.99     | 7.03        | 7.06     | 932.09    | 2000    | 212.7    |
| Extract all user IDs                        | strata_ndjson_fused | 28.70    | 29.29       | 29.88    | 223.62    | 2000    | 213.8    |
| Extract all user IDs                        | strata_ndjson_full  | 29.17    | 29.35       | 29.52    | 223.19    | 2000    | 213.8    |
| Extract all user names                      | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 593185.15 | 0       | 224.7    |
| Extract all user names                      | jmespath            | 0.72     | 0.73        | 0.75     | 8945.07   | 2000    | 224.7    |
| Extract all user names                      | strata              | 7.17     | 7.21        | 7.24     | 908.84    | 2000    | 224.7    |
| Extract all user names                      | strata_ndjson_fused | 29.11    | 29.18       | 29.25    | 224.48    | 2000    | 224.7    |
| Extract all user names                      | strata_ndjson_full  | 29.11    | 29.18       | 29.25    | 224.43    | 2000    | 224.7    |
| Extract nested timestamp field              | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 550599.24 | 0       | 214.2    |
| Extract nested timestamp field              | jmespath            | 1.66     | 1.67        | 1.68     | 3924.44   | 2000    | 214.2    |
| Extract nested timestamp field              | strata              | 7.59     | 7.63        | 7.67     | 858.29    | 2000    | 214.2    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 343588.36 | 0       | 224.6    |
| Extract order item prices (double wildcard) | jmespath            | 24.45    | 24.98       | 25.51    | 262.20    | 100999  | 224.6    |
| Extract order item prices (double wildcard) | strata              | 104.76   | 104.91      | 105.07   | 62.43     | 100999  | 221.7    |
| Filter users by age (numeric predicate)     | strata              | 0.01     | 0.01        | 0.01     | 471333.62 | 0       | 224.7    |
| Filter users by age (numeric predicate)     | jmespath            | 2.60     | 2.60        | 2.61     | 2514.72   | 1606    | 224.7    |
| NDJSON first record id                      | strata              | 0.01     | 0.01        | 0.01     | 505432.87 | 1       | 153.7    |
| NDJSON first record id                      | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 494294.74 | 1       | 154.0    |
| NDJSON first record id                      | jmespath            | 0.02     | 0.02        | 0.02     | 354437.69 | 1       | 153.7    |
| Recursively find all prices                 | strata              | 125.31   | 125.33      | 125.36   | 52.26     | 100999  | 224.9    |
| Recursively find all prices                 | jsonpath-ng         | 482.57   | 483.51      | 484.44   | 13.55     | 100999  | 229.7    |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.02     | 378319.29 | 10      | 229.7    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 293541.84 | 10      | 228.7    |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.03        | 0.03     | 253734.65 | 10      | 229.7    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 301.02   | 302.57      | 304.12   | 194.82 | 228.8    |
| strata_cursor_reparse | 737.25   | 751.68      | 766.11   | 78.42  | 292.7    |
- Speedup (reuse vs reparse): 2.48x
