# Strata Benchmark Results

Generated: 2026-02-13 00:09:32

## Environment

- Commit: 454fe03a16725c7bac048e9e5926d9bf888849a6
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
| orjson        | users.json   | 2.864    | 3.730       | 5.116    | 324.5    |
| msgspec       | users.json   | 2.751    | 4.883       | 5.025    | 327.7    |
| ujson         | users.json   | 4.372    | 6.488       | 6.783    | 326.6    |
| json (stdlib) | users.json   | 5.972    | 7.853       | 8.035    | 329.4    |
| strata        | users.json   | 11.578   | 11.822      | 18.543   | 320.1    |
| orjson        | users.ndjson | 3.029    | 3.853       | 4.167    | 334.1    |
| msgspec       | users.ndjson | 2.770    | 4.058       | 4.856    | 336.1    |
| ujson         | users.ndjson | 4.281    | 5.817       | 6.077    | 336.1    |
| json (stdlib) | users.ndjson | 5.897    | 7.016       | 7.617    | 336.1    |
| strata        | users.ndjson | 7.911    | 8.605       | 9.143    | 332.2    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.048    | 1000    |
|                                     | jmespath    | 0.246    | 1000    |
|                                     | jsonpath-ng | 1.929    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.666    | 12046   |
|                                     | jmespath    | 6.141    | 1000    |
|                                     | jsonpath-ng | 19.715   | 12046   |
| $..price                            | jmespath    | 6.043    | 1000    |
|                                     | query       | 6.511    | 12046   |
|                                     | jsonpath-ng | 74.353   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.037    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.283    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 3.22     | 4.24        | 4.32     | 237.78 | 378.7    |
| msgspec       | 3.51     | 4.63        | 6.22     | 217.82 | 378.8    |
| ujson         | 4.19     | 6.30        | 6.42     | 160.04 | 381.8    |
| json (stdlib) | 5.80     | 6.87        | 7.05     | 146.70 | 380.8    |
| strata        | 12.62    | 14.89       | 20.96    | 67.70  | 374.9    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 2.94     | 3.89        | 4.39     | 258.80 | 36.7     |
| msgspec       | 3.23     | 4.19        | 4.35     | 240.10 | 36.8     |
| ujson         | 3.94     | 7.06        | 7.13     | 142.69 | 39.8     |
| json (stdlib) | 5.59     | 7.25        | 7.61     | 138.98 | 38.8     |
| strata        | 6.92     | 7.68        | 7.78     | 131.10 | 37.2     |
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
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 137873.14 | 1       | 246.5    |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 92355.38  | 1       | 246.5    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 68643.22  | 1       | 246.5    |
| Extract all user IDs                        | jmespath    | 0.23     | 0.25        | 0.27     | 4046.28   | 1000    | 97.6     |
| Extract all user IDs                        | strata      | 1.13     | 1.19        | 1.24     | 849.29    | 1000    | 97.4     |
| Extract all user IDs                        | jsonpath-ng | 1.75     | 1.83        | 1.90     | 552.30    | 1000    | 98.5     |
| Extract all user names                      | jmespath    | 0.20     | 0.21        | 0.22     | 4883.30   | 1000    | 291.0    |
| Extract all user names                      | strata      | 1.03     | 1.04        | 1.05     | 967.89    | 1000    | 291.0    |
| Extract all user names                      | jsonpath-ng | 1.88     | 2.00        | 2.12     | 503.58    | 1000    | 291.0    |
| Extract nested timestamp field              | jmespath    | 0.60     | 0.60        | 0.61     | 1668.22   | 1000    | 159.2    |
| Extract nested timestamp field              | strata      | 1.40     | 1.43        | 1.46     | 706.11    | 1000    | 159.1    |
| Extract nested timestamp field              | jsonpath-ng | 2.28     | 2.38        | 2.47     | 424.05    | 1000    | 159.6    |
| Extract order item prices (double wildcard) | jmespath    | 6.14     | 6.28        | 6.42     | 160.58    | 1000    | 210.8    |
| Extract order item prices (double wildcard) | strata      | 13.75    | 14.42       | 15.08    | 69.94     | 12046   | 210.2    |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.72    | 18.84       | 18.97    | 53.51     | 12046   | 215.3    |
| Filter users by age (numeric predicate)     | strata      | 1.00     | 1.02        | 1.04     | 990.35    | 794     | 317.4    |
| Filter users by age (numeric predicate)     | jmespath    | 1.17     | 1.20        | 1.24     | 836.94    | 794     | 317.4    |
| Recursively find all prices                 | strata      | 15.06    | 15.20       | 15.33    | 66.35     | 12046   | 347.2    |
| Recursively find all prices                 | jsonpath-ng | 74.82    | 75.86       | 76.91    | 13.29     | 12046   | 348.9    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 80924.52  | 10      | 378.9    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 62121.31  | 10      | 378.9    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 41790.59  | 10      | 378.9    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 46.50    | 47.64       | 48.77    | 169.31 | 443.5    |
| strata_cursor_reparse | 131.96   | 134.26      | 136.56   | 60.07  | 983.7    |
- Speedup (reuse vs reparse): 2.82x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 112428.07 | 1       | 55.2     |
| Deep path navigation                        | jmespath            | 0.01     | 0.01        | 0.02     | 83500.91  | 1       | 55.2     |
| Deep path navigation                        | strata              | 0.01     | 0.02        | 0.02     | 60734.31  | 1       | 55.2     |
| Extract all user IDs                        | strata              | 0.21     | 0.23        | 0.24     | 4416.68   | 0       | 53.3     |
| Extract all user IDs                        | jmespath            | 0.28     | 0.33        | 0.39     | 3032.55   | 1000    | 53.4     |
| Extract all user IDs                        | jsonpath-ng         | 1.62     | 1.86        | 2.10     | 542.06    | 1000    | 53.4     |
| Extract all user IDs                        | strata_ndjson_fused | 4.60     | 4.98        | 5.37     | 202.19    | 0       | 53.4     |
| Extract all user IDs                        | strata_ndjson_full  | 6.89     | 6.92        | 6.94     | 145.62    | 0       | 53.3     |
| Extract all user names                      | strata              | 0.13     | 0.14        | 0.16     | 6985.26   | 0       | 55.2     |
| Extract all user names                      | jmespath            | 0.22     | 0.22        | 0.22     | 4571.19   | 1000    | 55.2     |
| Extract all user names                      | jsonpath-ng         | 1.67     | 1.73        | 1.78     | 583.40    | 1000    | 55.2     |
| Extract all user names                      | strata_ndjson_fused | 4.53     | 4.77        | 5.01     | 211.17    | 0       | 55.2     |
| Extract all user names                      | strata_ndjson_full  | 6.64     | 6.93        | 7.21     | 145.38    | 0       | 55.2     |
| Extract nested timestamp field              | strata              | 0.14     | 0.15        | 0.15     | 6901.50   | 0       | 53.5     |
| Extract nested timestamp field              | jmespath            | 0.57     | 0.58        | 0.59     | 1729.14   | 1000    | 53.5     |
| Extract nested timestamp field              | jsonpath-ng         | 2.59     | 2.60        | 2.62     | 386.85    | 1000    | 53.6     |
| Extract order item prices (double wildcard) | strata              | 0.21     | 0.21        | 0.21     | 4856.83   | 0       | 53.6     |
| Extract order item prices (double wildcard) | jmespath            | 3.76     | 3.89        | 4.02     | 258.84    | 12046   | 54.1     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 20.44    | 20.61       | 20.77    | 48.88     | 12046   | 57.2     |
| Filter users by age (numeric predicate)     | strata              | 0.01     | 0.01        | 0.01     | 136567.77 | 0       | 55.2     |
| Filter users by age (numeric predicate)     | jmespath            | 1.17     | 1.18        | 1.18     | 854.59    | 794     | 55.2     |
| NDJSON root field (id)                      | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 121472.24 | 0       | 45.0     |
| NDJSON root field (id)                      | strata              | 2.42     | 2.60        | 2.77     | 387.96    | 1000    | 44.6     |
| NDJSON root field (id)                      | strata_ndjson_fused | 3.13     | 3.23        | 3.33     | 311.77    | 1000    | 45.0     |
| NDJSON root field (id)                      | strata_ndjson_full  | 10.48    | 10.58       | 10.69    | 95.19     | 1000    | 45.0     |
| Recursively find all prices                 | strata              | 14.53    | 14.55       | 14.56    | 69.23     | 878     | 55.4     |
| Recursively find all prices                 | jsonpath-ng         | 75.24    | 80.69       | 86.13    | 12.48     | 12046   | 58.3     |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.02     | 49687.32  | 10      | 57.3     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 46131.42  | 10      | 56.5     |
| Slice first 10 users, extract IDs           | strata              | 0.03     | 0.03        | 0.04     | 30675.59  | 10      | 57.3     |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 29.51    | 30.32       | 31.12    | 299.00 | 56.5     |
| strata_cursor_reparse | 102.72   | 103.84      | 104.95   | 87.30  | 64.8     |
- Speedup (reuse vs reparse): 3.43x
