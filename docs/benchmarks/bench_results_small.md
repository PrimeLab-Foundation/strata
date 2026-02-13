# Strata Benchmark Results

Generated: 2026-02-14 01:12:12

## Environment

- Commit: e8443f3b87ee8f0b4d32fbf0d4c653dcb146cde0
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
| orjson        | users.json   | 2.512    | 4.080       | 4.126    | 324.3    |
| msgspec       | users.json   | 2.940    | 4.088       | 4.303    | 327.4    |
| ujson         | users.json   | 4.081    | 5.840       | 6.529    | 326.3    |
| json (stdlib) | users.json   | 5.334    | 6.548       | 7.243    | 328.4    |
| strata        | users.json   | 10.853   | 11.727      | 16.823   | 319.9    |
| orjson        | users.ndjson | 2.871    | 3.700       | 3.769    | 331.8    |
| msgspec       | users.ndjson | 2.738    | 3.836       | 3.948    | 333.8    |
| ujson         | users.ndjson | 4.011    | 5.458       | 5.869    | 333.8    |
| json (stdlib) | users.ndjson | 5.723    | 7.059       | 7.428    | 333.8    |
| strata        | users.ndjson | 7.297    | 8.913       | 9.252    | 329.8    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.029    | 1000    |
|                                     | jmespath    | 0.185    | 1000    |
|                                     | jsonpath-ng | 1.666    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.546    | 12046   |
|                                     | jmespath    | 5.668    | 1000    |
|                                     | jsonpath-ng | 18.887   | 12046   |
| $..price                            | query       | 6.130    | 12046   |
|                                     | jmespath    | 6.187    | 1000    |
|                                     | jsonpath-ng | 73.666   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.030    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.520    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 3.05     | 3.53        | 3.68     | 285.59 | 323.0    |
| msgspec       | 3.30     | 4.15        | 4.29     | 242.99 | 323.1    |
| ujson         | 4.03     | 6.00        | 6.00     | 168.13 | 326.1    |
| json (stdlib) | 5.22     | 6.42        | 6.48     | 157.02 | 325.1    |
| strata        | 11.68    | 12.24       | 18.56    | 82.37  | 319.2    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 2.54     | 3.60        | 3.83     | 279.95 | 36.6     |
| msgspec       | 2.86     | 3.79        | 3.86     | 265.41 | 36.7     |
| ujson         | 4.32     | 5.71        | 5.96     | 176.24 | 39.8     |
| json (stdlib) | 5.66     | 6.86        | 6.98     | 146.72 | 38.8     |
| strata        | 6.66     | 7.38        | 7.47     | 136.42 | 37.1     |
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
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.00     | 300595.87 | 1       | 225.3    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 176613.53 | 1       | 225.3    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 146200.56 | 1       | 225.3    |
| Extract all user IDs                        | jmespath    | 0.19     | 0.19        | 0.19     | 5222.13   | 1000    | 97.9     |
| Extract all user IDs                        | strata      | 1.16     | 1.18        | 1.20     | 855.83    | 1000    | 97.8     |
| Extract all user IDs                        | jsonpath-ng | 1.63     | 1.83        | 2.02     | 551.59    | 1000    | 98.7     |
| Extract all user names                      | jmespath    | 0.20     | 0.21        | 0.22     | 4760.79   | 1000    | 250.5    |
| Extract all user names                      | strata      | 1.26     | 1.27        | 1.28     | 791.51    | 1000    | 250.5    |
| Extract all user names                      | jsonpath-ng | 1.57     | 1.78        | 1.98     | 567.77    | 1000    | 250.5    |
| Extract nested timestamp field              | jmespath    | 0.52     | 0.55        | 0.58     | 1835.52   | 1000    | 159.1    |
| Extract nested timestamp field              | strata      | 1.30     | 1.38        | 1.46     | 730.90    | 1000    | 159.1    |
| Extract nested timestamp field              | jsonpath-ng | 2.12     | 2.23        | 2.35     | 451.74    | 1000    | 159.4    |
| Extract order item prices (double wildcard) | jmespath    | 5.87     | 5.92        | 5.96     | 170.37    | 1000    | 191.9    |
| Extract order item prices (double wildcard) | strata      | 14.69    | 14.99       | 15.30    | 67.24     | 12046   | 191.3    |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.38    | 18.38       | 18.39    | 54.84     | 12046   | 196.1    |
| Filter users by age (numeric predicate)     | strata      | 0.90     | 0.90        | 0.91     | 1114.52   | 794     | 276.7    |
| Filter users by age (numeric predicate)     | jmespath    | 1.12     | 1.19        | 1.26     | 847.92    | 794     | 276.7    |
| Recursively find all prices                 | strata      | 15.88    | 16.40       | 16.91    | 61.48     | 12046   | 300.7    |
| Recursively find all prices                 | jsonpath-ng | 72.90    | 73.17       | 73.45    | 13.78     | 12046   | 302.4    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 126024.71 | 10      | 326.7    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.01        | 0.01     | 75261.13  | 10      | 326.7    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.03        | 0.05     | 31838.00  | 10      | 326.7    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 42.73    | 43.20       | 43.68    | 186.70 | 376.5    |
| strata_cursor_reparse | 109.64   | 111.87      | 114.10   | 72.10  | 853.5    |
- Speedup (reuse vs reparse): 2.59x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata              | 0.00     | 0.00        | 0.00     | 246648.40 | 1       | 55.0     |
| Deep path navigation                        | jmespath            | 0.01     | 0.01        | 0.01     | 105558.54 | 1       | 55.0     |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 55568.92  | 1       | 55.0     |
| Extract all user IDs                        | jsonpath-ng         | 0.01     | 0.08        | 0.14     | 13380.81  | 0       | 53.9     |
| Extract all user IDs                        | jmespath            | 0.26     | 0.26        | 0.26     | 3898.79   | 1000    | 53.9     |
| Extract all user IDs                        | strata              | 2.06     | 2.16        | 2.27     | 465.88    | 1000    | 53.4     |
| Extract all user IDs                        | strata_ndjson_fused | 3.07     | 3.22        | 3.37     | 312.47    | 1000    | 53.8     |
| Extract all user IDs                        | strata_ndjson_full  | 9.13     | 9.22        | 9.31     | 109.23    | 1000    | 53.8     |
| Extract all user names                      | jsonpath-ng         | 0.00     | 0.01        | 0.01     | 175804.97 | 0       | 55.1     |
| Extract all user names                      | jmespath            | 0.26     | 0.28        | 0.29     | 3633.05   | 1000    | 55.1     |
| Extract all user names                      | strata              | 2.11     | 2.20        | 2.29     | 457.21    | 1000    | 55.1     |
| Extract all user names                      | strata_ndjson_fused | 2.96     | 2.99        | 3.02     | 336.84    | 1000    | 55.1     |
| Extract all user names                      | strata_ndjson_full  | 9.49     | 9.57        | 9.65     | 105.24    | 1000    | 55.1     |
| Extract nested timestamp field              | jsonpath-ng         | 0.00     | 0.01        | 0.01     | 185930.98 | 0       | 54.2     |
| Extract nested timestamp field              | jmespath            | 0.61     | 0.68        | 0.75     | 1482.02   | 1000    | 54.2     |
| Extract nested timestamp field              | strata              | 2.18     | 2.21        | 2.24     | 456.19    | 1000    | 54.2     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 91562.54  | 0       | 55.0     |
| Extract order item prices (double wildcard) | jmespath            | 3.67     | 3.69        | 3.71     | 272.78    | 12046   | 55.0     |
| Extract order item prices (double wildcard) | strata              | 13.28    | 13.45       | 13.63    | 74.87     | 12046   | 54.9     |
| Filter users by age (numeric predicate)     | strata              | 0.92     | 0.95        | 0.97     | 1062.85   | 794     | 55.1     |
| Filter users by age (numeric predicate)     | jmespath            | 1.09     | 1.17        | 1.24     | 862.32    | 794     | 55.1     |
| NDJSON first record id                      | strata              | 0.00     | 0.01        | 0.01     | 157483.49 | 1       | 44.3     |
| NDJSON first record id                      | jmespath            | 0.01     | 0.01        | 0.01     | 120563.44 | 1       | 44.3     |
| NDJSON first record id                      | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 75306.53  | 1       | 44.6     |
| Recursively find all prices                 | strata              | 14.72    | 14.74       | 14.75    | 68.35     | 12046   | 55.3     |
| Recursively find all prices                 | jsonpath-ng         | 72.57    | 73.24       | 73.92    | 13.75     | 12046   | 57.2     |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.01        | 0.02     | 76738.07  | 10      | 57.2     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 60431.83  | 10      | 56.3     |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.02        | 0.02     | 59536.97  | 10      | 57.2     |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 41.10    | 42.26       | 43.42    | 214.50 | 56.4     |
| strata_cursor_reparse | 112.63   | 112.98      | 113.33   | 80.23  | 64.9     |
- Speedup (reuse vs reparse): 2.67x
