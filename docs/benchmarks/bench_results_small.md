# Strata Benchmark Results

Generated: 2026-02-14 00:45:58

## Environment

- Commit: 67a3e370df19c36bc7749c21bf64263ace907581
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| msgspec       | users.json   | 2.792    | 3.690       | 4.926    | 327.5    |
| orjson        | users.json   | 2.590    | 3.821       | 3.935    | 324.4    |
| ujson         | users.json   | 3.832    | 5.799       | 6.170    | 326.4    |
| json (stdlib) | users.json   | 6.173    | 7.735       | 8.425    | 328.5    |
| strata        | users.json   | 11.073   | 15.802      | 20.487   | 320.0    |
| msgspec       | users.ndjson | 2.887    | 3.717       | 3.766    | 334.2    |
| orjson        | users.ndjson | 2.725    | 3.890       | 3.945    | 332.1    |
| ujson         | users.ndjson | 3.915    | 5.929       | 6.451    | 334.2    |
| json (stdlib) | users.ndjson | 5.752    | 6.640       | 6.794    | 334.2    |
| strata        | users.ndjson | 7.508    | 8.907       | 8.976    | 330.2    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.038    | 1000    |
|                                     | jmespath    | 0.203    | 1000    |
|                                     | jsonpath-ng | 1.553    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.582    | 12046   |
|                                     | jmespath    | 5.701    | 1000    |
|                                     | jsonpath-ng | 17.694   | 12046   |
| $..price                            | jmespath    | 5.800    | 1000    |
|                                     | query       | 5.816    | 12046   |
|                                     | jsonpath-ng | 70.637   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.028    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.129    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 3.10     | 3.56        | 3.95     | 283.19 | 323.0    |
| msgspec       | 3.15     | 4.11        | 4.34     | 245.20 | 323.0    |
| ujson         | 4.28     | 5.87        | 6.30     | 171.81 | 326.1    |
| json (stdlib) | 5.32     | 6.51        | 6.64     | 154.92 | 326.1    |
| strata        | 11.95    | 12.69       | 19.24    | 79.48  | 319.1    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 3.04     | 3.76        | 3.95     | 267.91 | 36.6     |
| orjson        | 2.73     | 3.89        | 4.13     | 258.65 | 36.6     |
| ujson         | 3.99     | 5.57        | 6.17     | 180.82 | 39.7     |
| json (stdlib) | 5.27     | 6.74        | 6.99     | 149.52 | 38.7     |
| strata        | 6.19     | 7.00        | 7.47     | 143.79 | 37.1     |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.00     | 264445.38 | 1       | 259.6    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 84456.40  | 1       | 259.6    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 59967.18  | 1       | 259.6    |
| Extract all user IDs                        | jmespath    | 0.21     | 0.21        | 0.22     | 4750.04   | 1000    | 98.0     |
| Extract all user IDs                        | strata      | 1.20     | 1.20        | 1.20     | 841.90    | 1000    | 98.0     |
| Extract all user IDs                        | jsonpath-ng | 1.73     | 1.73        | 1.73     | 583.93    | 1000    | 98.7     |
| Extract all user names                      | jmespath    | 0.18     | 0.19        | 0.20     | 5237.95   | 1000    | 291.2    |
| Extract all user names                      | strata      | 0.99     | 1.02        | 1.06     | 983.91    | 1000    | 291.2    |
| Extract all user names                      | jsonpath-ng | 1.63     | 1.87        | 2.11     | 539.85    | 1000    | 291.2    |
| Extract nested timestamp field              | jmespath    | 0.54     | 0.58        | 0.62     | 1740.34   | 1000    | 159.4    |
| Extract nested timestamp field              | strata      | 1.24     | 1.26        | 1.28     | 800.85    | 1000    | 159.4    |
| Extract nested timestamp field              | jsonpath-ng | 2.02     | 2.19        | 2.36     | 459.85    | 1000    | 159.7    |
| Extract order item prices (double wildcard) | jmespath    | 5.57     | 5.58        | 5.58     | 180.79    | 1000    | 210.3    |
| Extract order item prices (double wildcard) | strata      | 14.33    | 14.54       | 14.74    | 69.36     | 12046   | 209.8    |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.44    | 17.48       | 17.53    | 57.67     | 12046   | 214.6    |
| Filter users by age (numeric predicate)     | strata      | 0.88     | 0.91        | 0.95     | 1105.28   | 794     | 321.4    |
| Filter users by age (numeric predicate)     | jmespath    | 1.18     | 1.20        | 1.22     | 838.43    | 794     | 321.4    |
| Recursively find all prices                 | strata      | 15.56    | 15.85       | 16.14    | 63.59     | 12046   | 347.0    |
| Recursively find all prices                 | jsonpath-ng | 71.11    | 71.56       | 72.01    | 14.09     | 12046   | 348.7    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 80924.52  | 10      | 372.2    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.02        | 0.02     | 57611.33  | 10      | 372.2    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 46801.50  | 10      | 372.2    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 41.89    | 43.32       | 44.75    | 186.19 | 394.8    |
| strata_cursor_reparse | 116.63   | 117.24      | 117.86   | 68.79  | 839.2    |
- Speedup (reuse vs reparse): 2.71x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.01        | 0.01     | 175147.88 | 1       | 54.9     |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 98262.24  | 1       | 54.9     |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.01     | 81120.14  | 1       | 54.9     |
| Extract all user IDs                        | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 63778.27  | 0       | 53.6     |
| Extract all user IDs                        | jmespath            | 0.21     | 0.24        | 0.27     | 4152.99   | 1000    | 53.6     |
| Extract all user IDs                        | strata              | 2.11     | 2.15        | 2.20     | 467.92    | 1000    | 53.2     |
| Extract all user IDs                        | strata_ndjson_fused | 2.59     | 2.75        | 2.91     | 366.29    | 1000    | 53.5     |
| Extract all user IDs                        | strata_ndjson_full  | 9.04     | 9.04        | 9.05     | 111.35    | 1000    | 53.5     |
| Extract all user names                      | jsonpath-ng         | 0.00     | 0.01        | 0.01     | 169631.66 | 0       | 54.9     |
| Extract all user names                      | jmespath            | 0.25     | 0.26        | 0.27     | 3916.48   | 1000    | 54.9     |
| Extract all user names                      | strata              | 2.08     | 2.20        | 2.32     | 458.26    | 1000    | 54.9     |
| Extract all user names                      | strata_ndjson_fused | 2.89     | 3.05        | 3.21     | 330.36    | 1000    | 54.9     |
| Extract all user names                      | strata_ndjson_full  | 9.52     | 9.61        | 9.70     | 104.79    | 1000    | 54.9     |
| Extract nested timestamp field              | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 141777.50 | 0       | 53.8     |
| Extract nested timestamp field              | jmespath            | 0.60     | 0.61        | 0.62     | 1648.14   | 1000    | 53.8     |
| Extract nested timestamp field              | strata              | 2.12     | 2.17        | 2.23     | 463.34    | 1000    | 53.8     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.00     | 0.00        | 0.00     | 239322.43 | 0       | 54.9     |
| Extract order item prices (double wildcard) | jmespath            | 3.51     | 3.54        | 3.57     | 284.65    | 12046   | 54.9     |
| Extract order item prices (double wildcard) | strata              | 12.71    | 12.89       | 13.07    | 78.12     | 12046   | 54.6     |
| Filter users by age (numeric predicate)     | strata              | 0.98     | 1.01        | 1.04     | 997.24    | 794     | 54.9     |
| Filter users by age (numeric predicate)     | jmespath            | 1.10     | 1.14        | 1.18     | 883.51    | 794     | 54.9     |
| NDJSON first record id                      | jmespath            | 0.00     | 0.01        | 0.01     | 163331.90 | 1       | 44.1     |
| NDJSON first record id                      | strata              | 0.00     | 0.01        | 0.01     | 106490.54 | 1       | 44.1     |
| NDJSON first record id                      | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 69262.96  | 1       | 44.4     |
| Recursively find all prices                 | strata              | 13.99    | 14.18       | 14.37    | 71.03     | 12046   | 55.1     |
| Recursively find all prices                 | jsonpath-ng         | 70.83    | 70.95       | 71.08    | 14.19     | 12046   | 57.0     |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.01        | 0.02     | 70167.68  | 10      | 57.0     |
| Slice first 10 users, extract IDs           | strata              | 0.01     | 0.02        | 0.02     | 66498.56  | 10      | 57.0     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.03        | 0.05     | 30559.71  | 10      | 56.1     |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 40.50    | 41.08       | 41.67    | 220.63 | 56.3     |
| strata_cursor_reparse | 108.65   | 108.69      | 108.72   | 83.40  | 64.7     |
- Speedup (reuse vs reparse): 2.65x
