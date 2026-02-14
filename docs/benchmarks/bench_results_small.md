# Strata Benchmark Results

Generated: 2026-02-14 17:43:48

## Environment

- Commit: 479cf62079eea032a9baeadd1f59e2c2c2375686
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
| orjson        | users.json   | 2.626    | 3.741       | 4.269    | 304.7    |
| msgspec       | users.json   | 3.056    | 3.910       | 4.351    | 307.8    |
| ujson         | users.json   | 3.863    | 6.293       | 7.026    | 306.8    |
| json (stdlib) | users.json   | 5.450    | 6.325       | 6.698    | 308.8    |
| strata        | users.json   | 11.081   | 11.327      | 17.540   | 300.3    |
| orjson        | users.ndjson | 2.750    | 3.757       | 3.900    | 311.7    |
| msgspec       | users.ndjson | 2.998    | 4.018       | 4.311    | 313.7    |
| ujson         | users.ndjson | 4.324    | 6.098       | 6.109    | 313.7    |
| json (stdlib) | users.ndjson | 5.680    | 6.664       | 6.832    | 313.7    |
| strata        | users.ndjson | 7.735    | 9.124       | 9.325    | 309.8    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.030    | 1000    |
|                                     | jmespath    | 0.199    | 1000    |
|                                     | jsonpath-ng | 1.676    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.560    | 12046   |
|                                     | jmespath    | 5.715    | 1000    |
|                                     | jsonpath-ng | 18.612   | 12046   |
| $..price                            | jmespath    | 6.030    | 1000    |
|                                     | query       | 6.102    | 12046   |
|                                     | jsonpath-ng | 73.080   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.031    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.669    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 2.70     | 3.90        | 4.47     | 258.44 | 303.3    |
| msgspec       | 2.94     | 4.23        | 4.50     | 238.53 | 303.4    |
| json (stdlib) | 5.24     | 6.49        | 6.51     | 155.24 | 305.5    |
| ujson         | 3.86     | 6.52        | 6.79     | 154.65 | 306.5    |
| strata        | 11.67    | 11.68       | 17.46    | 86.35  | 299.5    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 2.87     | 3.73        | 4.11     | 270.33 | 36.8     |
| msgspec       | 2.83     | 3.99        | 4.26     | 252.35 | 36.8     |
| ujson         | 4.22     | 5.84        | 6.25     | 172.37 | 39.9     |
| json (stdlib) | 5.67     | 6.86        | 6.98     | 146.89 | 38.9     |
| strata        | 7.05     | 7.40        | 7.58     | 136.17 | 37.1     |
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
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.01     | 199959.13 | 1       | 235.7    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 143179.49 | 1       | 235.7    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 114133.37 | 1       | 235.7    |
| Extract all user IDs                        | jmespath    | 0.22     | 0.26        | 0.30     | 3905.53   | 1000    | 98.0     |
| Extract all user IDs                        | strata      | 1.31     | 1.35        | 1.39     | 746.58    | 1000    | 98.0     |
| Extract all user IDs                        | jsonpath-ng | 1.83     | 1.99        | 2.15     | 506.14    | 1000    | 98.7     |
| Extract all user names                      | jmespath    | 0.20     | 0.20        | 0.21     | 4945.17   | 1000    | 268.4    |
| Extract all user names                      | strata      | 1.27     | 1.28        | 1.28     | 789.78    | 1000    | 268.4    |
| Extract all user names                      | jsonpath-ng | 1.61     | 1.64        | 1.67     | 615.08    | 1000    | 268.4    |
| Extract nested timestamp field              | jmespath    | 0.61     | 0.61        | 0.62     | 1639.85   | 1000    | 159.3    |
| Extract nested timestamp field              | strata      | 1.41     | 1.49        | 1.56     | 678.52    | 1000    | 159.3    |
| Extract nested timestamp field              | jsonpath-ng | 2.88     | 2.89        | 2.90     | 348.63    | 1000    | 159.7    |
| Extract order item prices (double wildcard) | jmespath    | 6.03     | 6.26        | 6.50     | 160.99    | 1000    | 199.7    |
| Extract order item prices (double wildcard) | strata      | 16.53    | 16.75       | 16.97    | 60.18     | 12046   | 199.2    |
| Extract order item prices (double wildcard) | jsonpath-ng | 19.52    | 19.68       | 19.84    | 51.24     | 12046   | 204.0    |
| Filter users by age (numeric predicate)     | strata      | 1.05     | 1.07        | 1.09     | 943.20    | 794     | 297.3    |
| Filter users by age (numeric predicate)     | jmespath    | 1.16     | 1.20        | 1.23     | 843.09    | 794     | 296.3    |
| Recursively find all prices                 | strata      | 14.71    | 14.76       | 14.81    | 68.30     | 12046   | 333.7    |
| Recursively find all prices                 | jsonpath-ng | 74.43    | 74.43       | 74.44    | 13.54     | 12046   | 335.4    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 104292.78 | 10      | 357.0    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.02        | 0.02     | 61727.68  | 10      | 357.0    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.02     | 59524.61  | 10      | 357.0    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 45.52    | 45.80       | 46.08    | 176.11 | 383.0    |
| strata_cursor_reparse | 116.44   | 118.57      | 120.70   | 68.03  | 836.6    |
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
| Deep path navigation                        | strata              | 0.00     | 0.00        | 0.00     | 229114.29 | 1       | 54.9     |
| Deep path navigation                        | jmespath            | 0.01     | 0.01        | 0.01     | 155454.25 | 1       | 54.9     |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 77478.89  | 1       | 54.9     |
| Extract all user IDs                        | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 84075.98  | 0       | 53.8     |
| Extract all user IDs                        | jmespath            | 0.23     | 0.25        | 0.27     | 3963.99   | 1000    | 53.8     |
| Extract all user IDs                        | strata              | 2.18     | 2.29        | 2.40     | 439.91    | 1000    | 53.5     |
| Extract all user IDs                        | strata_ndjson_fused | 2.75     | 2.93        | 3.12     | 343.21    | 1000    | 53.8     |
| Extract all user IDs                        | strata_ndjson_full  | 9.54     | 9.63        | 9.72     | 104.59    | 1000    | 53.8     |
| Extract all user names                      | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 95544.99  | 0       | 54.9     |
| Extract all user names                      | jmespath            | 0.23     | 0.24        | 0.26     | 4144.09   | 1000    | 54.9     |
| Extract all user names                      | strata              | 2.16     | 2.25        | 2.33     | 448.43    | 1000    | 54.9     |
| Extract all user names                      | strata_ndjson_fused | 2.74     | 2.74        | 2.75     | 367.21    | 1000    | 54.9     |
| Extract all user names                      | strata_ndjson_full  | 9.40     | 9.53        | 9.66     | 105.71    | 1000    | 54.9     |
| Extract nested timestamp field              | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 76252.96  | 0       | 54.0     |
| Extract nested timestamp field              | jmespath            | 0.59     | 0.63        | 0.66     | 1605.98   | 1000    | 54.0     |
| Extract nested timestamp field              | strata              | 2.19     | 2.36        | 2.52     | 427.35    | 1000    | 54.0     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.00     | 0.02        | 0.03     | 63362.99  | 0       | 54.8     |
| Extract order item prices (double wildcard) | jmespath            | 3.81     | 3.84        | 3.87     | 262.34    | 12046   | 54.8     |
| Extract order item prices (double wildcard) | strata              | 13.43    | 13.47       | 13.50    | 74.79     | 12046   | 54.7     |
| Filter users by age (numeric predicate)     | strata              | 0.97     | 0.98        | 0.99     | 1027.52   | 794     | 54.9     |
| Filter users by age (numeric predicate)     | jmespath            | 1.21     | 1.23        | 1.26     | 816.05    | 794     | 54.9     |
| NDJSON first record id                      | jmespath            | 0.01     | 0.01        | 0.01     | 138922.12 | 1       | 44.2     |
| NDJSON first record id                      | jsonpath-ng         | 0.00     | 0.01        | 0.01     | 138520.92 | 1       | 44.4     |
| NDJSON first record id                      | strata              | 0.01     | 0.01        | 0.01     | 70781.53  | 1       | 44.2     |
| Recursively find all prices                 | strata              | 14.79    | 14.81       | 14.83    | 68.03     | 12046   | 55.1     |
| Recursively find all prices                 | jsonpath-ng         | 74.70    | 74.77       | 74.84    | 13.47     | 12046   | 56.9     |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.01        | 0.01     | 125248.57 | 10      | 57.0     |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.02        | 0.02     | 53896.35  | 10      | 57.0     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.02        | 0.03     | 40557.60  | 10      | 56.1     |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 42.63    | 43.05       | 43.47    | 210.55 | 56.3     |
| strata_cursor_reparse | 116.83   | 116.92      | 117.00   | 77.53  | 64.7     |
- Speedup (reuse vs reparse): 2.72x
