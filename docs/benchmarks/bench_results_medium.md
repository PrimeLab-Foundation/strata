# Strata Benchmark Results

Generated: 2026-02-12 12:30:08

## Environment

- Commit: 32da0b8032dd813bbd9c3eb030f05d1c87dd120f
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
| orjson        | users.json   | 20.089   | 28.132      | 30.491   | 2715.4   |
| msgspec       | users.json   | 19.928   | 28.407      | 29.657   | 2715.3   |
| ujson         | users.json   | 28.804   | 42.665      | 42.760   | 2733.9   |
| json (stdlib) | users.json   | 35.949   | 47.908      | 49.622   | 2715.3   |
| strata        | users.json   | 73.836   | 81.609      | 133.802  | 2693.8   |
| orjson        | users.ndjson | 23.621   | 24.860      | 26.178   | 2696.5   |
| msgspec       | users.ndjson | 25.035   | 25.491      | 27.312   | 2697.3   |
| ujson         | users.ndjson | 31.087   | 36.564      | 37.299   | 2715.0   |
| json (stdlib) | users.ndjson | 40.179   | 41.718      | 44.079   | 2699.2   |
| strata        | users.ndjson | 52.620   | 62.064      | 63.469   | 2729.2   |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.116    | 2000    |
|                                     | jmespath    | 0.454    | 2000    |
|                                     | jsonpath-ng | 7.313    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 4.061    | 100999  |
|                                     | jmespath    | 33.999   | 2000    |
|                                     | jsonpath-ng | 137.758  | 100999  |
| $..price                            | jmespath    | 33.126   | 2000    |
|                                     | query       | 36.934   | 100999  |
|                                     | jsonpath-ng | 499.675  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.207    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 40.306   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 21.36    | 28.23       | 28.95    | 232.08 | 2714.7   |
| msgspec       | 19.86    | 28.73       | 29.49    | 228.02 | 2715.4   |
| ujson         | 26.64    | 42.05       | 42.21    | 155.81 | 2733.2   |
| json (stdlib) | 37.39    | 45.76       | 45.91    | 143.17 | 2715.5   |
| strata        | 80.13    | 84.73       | 128.98   | 77.32  | 2695.1   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 23.29    | 25.63       | 27.55    | 255.56 | 143.9    |
| msgspec       | 24.84    | 26.47       | 28.69    | 247.45 | 143.7    |
| ujson         | 34.72    | 37.70       | 37.94    | 173.72 | 161.6    |
| json (stdlib) | 39.87    | 41.62       | 48.01    | 157.38 | 144.7    |
| strata        | 50.68    | 52.20       | 52.56    | 125.46 | 142.7    |
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
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 541280.93 | 1       | 1552.1   |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 520675.64 | 1       | 1551.1   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 417622.70 | 1       | 1551.1   |
| Extract all user IDs                        | jmespath    | 0.56     | 0.59        | 0.62     | 11101.38  | 2000    | 553.3    |
| Extract all user IDs                        | strata      | 2.49     | 2.54        | 2.60     | 2575.13   | 2000    | 553.1    |
| Extract all user IDs                        | jsonpath-ng | 7.03     | 7.07        | 7.12     | 926.14    | 2000    | 554.6    |
| Extract all user names                      | jmespath    | 0.47     | 0.48        | 0.50     | 13582.10  | 2000    | 1946.5   |
| Extract all user names                      | strata      | 2.40     | 2.41        | 2.41     | 2720.67   | 2000    | 1946.5   |
| Extract all user names                      | jsonpath-ng | 6.68     | 6.72        | 6.75     | 975.48    | 2000    | 1946.5   |
| Extract nested timestamp field              | jmespath    | 1.17     | 1.20        | 1.23     | 5457.63   | 2000    | 985.6    |
| Extract nested timestamp field              | strata      | 2.41     | 2.42        | 2.43     | 2703.88   | 2000    | 985.6    |
| Extract nested timestamp field              | jsonpath-ng | 7.31     | 7.48        | 7.66     | 875.34    | 2000    | 986.3    |
| Extract order item prices (double wildcard) | jmespath    | 34.72    | 35.00       | 35.28    | 187.19    | 2000    | 1155.6   |
| Extract order item prices (double wildcard) | strata      | 117.17   | 117.50      | 117.83   | 55.76     | 100999  | 1153.6   |
| Extract order item prices (double wildcard) | jsonpath-ng | 130.09   | 131.18      | 132.26   | 49.95     | 100999  | 1159.5   |
| Filter users by age (numeric predicate)     | strata      | 2.07     | 2.10        | 2.12     | 3122.35   | 1606    | 2089.5   |
| Filter users by age (numeric predicate)     | jmespath    | 2.23     | 2.24        | 2.26     | 2920.69   | 1606    | 2089.5   |
| Recursively find all prices                 | strata      | 194.42   | 194.48      | 194.54   | 33.69     | 100999  | 2181.1   |
| Recursively find all prices                 | jsonpath-ng | 495.93   | 498.34      | 500.74   | 13.15     | 100999  | 2185.3   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 270405.83 | 10      | 2255.2   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 222561.90 | 10      | 2255.2   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 197911.56 | 10      | 2255.2   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 347.49   | 362.03      | 376.57   | 144.78 | 2440.1   |
| strata_cursor_reparse | 774.25   | 784.67      | 795.09   | 66.80  | 3381.6   |
- Speedup (reuse vs reparse): 2.17x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata              | 0.02     | 0.02        | 0.02     | 377414.85 | 0       | 219.8    |
| Deep path navigation                        | jmespath            | 0.02     | 0.02        | 0.02     | 311265.76 | 1       | 219.8    |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 276019.00 | 1       | 219.8    |
| Extract all user IDs                        | jmespath            | 0.82     | 0.87        | 0.92     | 7524.55   | 2000    | 214.0    |
| Extract all user IDs                        | strata              | 5.55     | 5.77        | 6.00     | 1134.29   | 0       | 213.6    |
| Extract all user IDs                        | jsonpath-ng         | 7.34     | 7.42        | 7.50     | 882.22    | 2000    | 214.1    |
| Extract all user IDs                        | strata_ndjson_full  | 22.75    | 22.97       | 23.18    | 285.19    | 0       | 213.8    |
| Extract all user IDs                        | strata_ndjson_fused | 23.42    | 23.48       | 23.54    | 278.97    | 0       | 213.8    |
| Extract all user names                      | jmespath            | 0.84     | 0.87        | 0.90     | 7521.13   | 2000    | 219.8    |
| Extract all user names                      | strata              | 5.69     | 5.70        | 5.72     | 1148.06   | 0       | 219.8    |
| Extract all user names                      | jsonpath-ng         | 7.86     | 8.04        | 8.21     | 814.84    | 2000    | 219.8    |
| Extract all user names                      | strata_ndjson_full  | 23.01    | 23.01       | 23.02    | 284.62    | 0       | 219.8    |
| Extract all user names                      | strata_ndjson_fused | 24.06    | 24.06       | 24.07    | 272.21    | 0       | 219.8    |
| Extract nested timestamp field              | jmespath            | 1.44     | 1.47        | 1.51     | 4443.14   | 2000    | 214.3    |
| Extract nested timestamp field              | strata              | 5.36     | 5.38        | 5.41     | 1216.57   | 0       | 214.3    |
| Extract nested timestamp field              | jsonpath-ng         | 7.88     | 8.07        | 8.26     | 811.74    | 2000    | 214.3    |
| Extract order item prices (double wildcard) | strata              | 5.28     | 5.34        | 5.41     | 1225.68   | 0       | 214.5    |
| Extract order item prices (double wildcard) | jmespath            | 24.67    | 25.08       | 25.49    | 261.18    | 100999  | 218.0    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 137.11   | 138.87      | 140.63   | 47.16     | 100999  | 221.8    |
| Filter users by age (numeric predicate)     | strata              | 0.01     | 0.02        | 0.02     | 414758.17 | 0       | 219.8    |
| Filter users by age (numeric predicate)     | jmespath            | 2.62     | 2.62        | 2.62     | 2502.63   | 1606    | 219.9    |
| NDJSON root field (id)                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 321455.44 | 0       | 156.6    |
| NDJSON root field (id)                      | strata              | 8.13     | 8.31        | 8.50     | 788.04    | 2000    | 155.3    |
| NDJSON root field (id)                      | strata_ndjson_full  | 29.56    | 29.74       | 29.91    | 220.26    | 2000    | 156.3    |
| NDJSON root field (id)                      | strata_ndjson_fused | 29.49    | 29.84       | 30.19    | 219.52    | 2000    | 156.6    |
| Recursively find all prices                 | strata              | 187.59   | 190.87      | 194.15   | 34.31     | 1908    | 224.0    |
| Recursively find all prices                 | jsonpath-ng         | 485.54   | 491.91      | 498.29   | 13.31     | 100999  | 226.0    |
| Slice first 10 users, extract IDs           | strata              | 0.01     | 0.01        | 0.01     | 479231.26 | 0       | 226.5    |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.02     | 411487.87 | 10      | 225.5    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.03        | 0.04     | 212564.79 | 10      | 224.5    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 264.10   | 285.42      | 306.74   | 206.53 | 224.6    |
| strata_cursor_reparse | 726.83   | 727.02      | 727.22   | 81.08  | 288.4    |
- Speedup (reuse vs reparse): 2.55x
