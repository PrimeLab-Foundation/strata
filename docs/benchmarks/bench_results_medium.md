# Strata Benchmark Results

Generated: 2026-02-14 00:46:08

## Environment

- Commit: 67a3e370df19c36bc7749c21bf64263ace907581
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
| orjson        | users.json   | 18.883   | 26.165      | 28.851   | 128.2    |
| msgspec       | users.json   | 20.095   | 28.319      | 30.256   | 128.1    |
| ujson         | users.json   | 27.766   | 39.093      | 39.801   | 146.7    |
| json (stdlib) | users.json   | 37.142   | 46.981      | 49.550   | 128.1    |
| strata        | users.json   | 47.338   | 48.671      | 48.738   | 107.8    |
| orjson        | users.ndjson | 23.213   | 23.330      | 24.792   | 149.7    |
| msgspec       | users.ndjson | 24.411   | 24.499      | 26.212   | 151.0    |
| ujson         | users.ndjson | 32.524   | 34.617      | 35.480   | 168.3    |
| json (stdlib) | users.ndjson | 39.428   | 39.970      | 41.782   | 153.0    |
| strata        | users.ndjson | 49.403   | 57.947      | 58.318   | 144.7    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.233    | 2000    |
|                                     | jmespath    | 0.583    | 2000    |
|                                     | jsonpath-ng | 7.313    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 4.368    | 100999  |
|                                     | jmespath    | 32.858   | 2000    |
|                                     | jsonpath-ng | 129.443  | 100999  |
| $..price                            | jmespath    | 34.064   | 2000    |
|                                     | query       | 36.843   | 100999  |
|                                     | jsonpath-ng | 488.499  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.165    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 39.915   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 18.32    | 26.06       | 27.22    | 251.45 | 127.4    |
| msgspec       | 20.65    | 28.08       | 28.86    | 233.35 | 128.2    |
| ujson         | 27.12    | 38.61       | 39.01    | 169.67 | 146.0    |
| json (stdlib) | 35.28    | 43.76       | 44.18    | 149.73 | 128.3    |
| strata        | 49.62    | 53.18       | 53.24    | 123.21 | 108.0    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 22.42    | 23.67       | 24.60    | 276.74 | 144.3    |
| msgspec       | 24.11    | 25.73       | 25.93    | 254.54 | 144.0    |
| ujson         | 32.46    | 35.25       | 35.82    | 185.80 | 162.0    |
| json (stdlib) | 39.81    | 41.26       | 43.40    | 158.76 | 145.1    |
| strata        | 47.70    | 49.50       | 49.86    | 132.31 | 143.1    |
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
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 483821.30 | 1       | 1746.9   |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 459105.33 | 1       | 1746.9   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 339605.13 | 1       | 1746.9   |
| Extract all user IDs                        | jmespath    | 0.58     | 0.62        | 0.65     | 10634.73  | 2000    | 553.6    |
| Extract all user IDs                        | strata      | 2.42     | 2.43        | 2.43     | 2698.70   | 2000    | 553.3    |
| Extract all user IDs                        | jsonpath-ng | 6.65     | 6.87        | 7.09     | 953.21    | 2000    | 554.8    |
| Extract all user names                      | jmespath    | 0.50     | 0.53        | 0.55     | 12469.95  | 2000    | 1887.5   |
| Extract all user names                      | strata      | 2.44     | 2.45        | 2.46     | 2675.65   | 2000    | 1887.5   |
| Extract all user names                      | jsonpath-ng | 6.73     | 6.83        | 6.93     | 959.57    | 2000    | 1887.5   |
| Extract nested timestamp field              | jmespath    | 1.18     | 1.22        | 1.25     | 5376.18   | 2000    | 985.9    |
| Extract nested timestamp field              | strata      | 2.33     | 2.34        | 2.35     | 2804.05   | 2000    | 985.9    |
| Extract nested timestamp field              | jsonpath-ng | 8.27     | 8.53        | 8.78     | 768.35    | 2000    | 986.5    |
| Extract order item prices (double wildcard) | jmespath    | 33.49    | 34.68       | 35.86    | 188.93    | 2000    | 1407.7   |
| Extract order item prices (double wildcard) | strata      | 115.68   | 116.34      | 117.00   | 56.32     | 100999  | 1405.6   |
| Extract order item prices (double wildcard) | jsonpath-ng | 133.72   | 134.43      | 135.14   | 48.74     | 100999  | 1410.2   |
| Filter users by age (numeric predicate)     | strata      | 1.90     | 1.92        | 1.93     | 3416.66   | 1606    | 2056.8   |
| Filter users by age (numeric predicate)     | jmespath    | 2.37     | 2.38        | 2.38     | 2757.34   | 1606    | 2056.8   |
| Recursively find all prices                 | strata      | 127.92   | 128.06      | 128.20   | 51.16     | 100999  | 2127.6   |
| Recursively find all prices                 | jsonpath-ng | 498.22   | 498.23      | 498.23   | 13.15     | 100999  | 2131.3   |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 346722.33 | 10      | 2212.0   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 278551.21 | 10      | 2212.0   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 246846.03 | 10      | 2212.0   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 292.42   | 309.11      | 325.80   | 169.56 | 2419.6   |
| strata_cursor_reparse | 692.68   | 718.48      | 744.27   | 72.95  | 3223.3   |
- Speedup (reuse vs reparse): 2.32x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 491217.91 | 1       | 218.4    |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.02     | 465769.56 | 1       | 218.4    |
| Deep path navigation                        | jmespath            | 0.01     | 0.02        | 0.02     | 372933.92 | 1       | 218.4    |
| Extract all user IDs                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 290553.31 | 0       | 206.5    |
| Extract all user IDs                        | jmespath            | 0.80     | 0.81        | 0.82     | 8110.61   | 2000    | 206.5    |
| Extract all user IDs                        | strata              | 6.97     | 7.15        | 7.34     | 915.76    | 2000    | 205.3    |
| Extract all user IDs                        | strata_ndjson_full  | 28.83    | 29.08       | 29.34    | 225.20    | 2000    | 206.2    |
| Extract all user IDs                        | strata_ndjson_fused | 28.72    | 29.12       | 29.52    | 224.90    | 2000    | 206.3    |
| Extract all user names                      | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 570576.88 | 0       | 218.5    |
| Extract all user names                      | jmespath            | 0.83     | 0.86        | 0.89     | 7617.72   | 2000    | 218.5    |
| Extract all user names                      | strata              | 7.20     | 7.31        | 7.41     | 896.51    | 2000    | 218.5    |
| Extract all user names                      | strata_ndjson_full  | 29.01    | 29.60       | 30.18    | 221.30    | 2000    | 218.5    |
| Extract all user names                      | strata_ndjson_fused | 29.35    | 29.81       | 30.28    | 219.68    | 2000    | 218.5    |
| Extract nested timestamp field              | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 321463.36 | 0       | 207.2    |
| Extract nested timestamp field              | jmespath            | 1.49     | 1.54        | 1.59     | 4251.35   | 2000    | 207.2    |
| Extract nested timestamp field              | strata              | 9.04     | 9.08        | 9.11     | 721.43    | 2000    | 207.2    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 419742.21 | 0       | 218.4    |
| Extract order item prices (double wildcard) | jmespath            | 25.24    | 25.79       | 26.34    | 253.94    | 100999  | 218.4    |
| Extract order item prices (double wildcard) | strata              | 122.57   | 125.71      | 128.86   | 52.10     | 100999  | 214.8    |
| Filter users by age (numeric predicate)     | jmespath            | 2.62     | 2.63        | 2.65     | 2486.34   | 1606    | 218.6    |
| Filter users by age (numeric predicate)     | strata              | 6.99     | 7.58        | 8.16     | 864.32    | 1606    | 218.5    |
| NDJSON first record id                      | jmespath            | 0.01     | 0.01        | 0.01     | 517943.17 | 1       | 146.4    |
| NDJSON first record id                      | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 496656.21 | 1       | 146.8    |
| NDJSON first record id                      | strata              | 0.01     | 0.02        | 0.02     | 433034.74 | 1       | 146.4    |
| Recursively find all prices                 | strata              | 127.83   | 127.94      | 128.05   | 51.19     | 100999  | 218.8    |
| Recursively find all prices                 | jsonpath-ng         | 501.31   | 501.41      | 501.52   | 13.06     | 100999  | 221.2    |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.01        | 0.01     | 479231.51 | 10      | 220.2    |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.03        | 0.03     | 260035.90 | 10      | 220.2    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.03     | 0.03        | 0.03     | 200246.27 | 10      | 220.0    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 280.16   | 282.21      | 284.26   | 208.88 | 252.8    |
| strata_cursor_reparse | 756.99   | 757.23      | 757.47   | 77.85  | 316.2    |
- Speedup (reuse vs reparse): 2.68x
