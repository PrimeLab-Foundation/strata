# Strata Benchmark Results

Generated: 2026-02-12 12:29:57

## Environment

- Commit: 32da0b8032dd813bbd9c3eb030f05d1c87dd120f
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
| msgspec       | users.json   | 3.028    | 3.518       | 3.839    | 327.5    |
| orjson        | users.json   | 2.372    | 3.578       | 4.540    | 324.3    |
| ujson         | users.json   | 3.483    | 6.066       | 6.704    | 326.4    |
| json (stdlib) | users.json   | 5.641    | 6.100       | 6.364    | 328.5    |
| strata        | users.json   | 11.049   | 11.531      | 17.399   | 319.9    |
| orjson        | users.ndjson | 2.622    | 3.473       | 3.855    | 331.8    |
| msgspec       | users.ndjson | 2.711    | 3.477       | 3.563    | 333.8    |
| ujson         | users.ndjson | 4.029    | 5.447       | 5.532    | 333.8    |
| json (stdlib) | users.ndjson | 5.580    | 6.509       | 6.522    | 333.8    |
| strata        | users.ndjson | 7.544    | 8.135       | 8.338    | 329.9    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.031    | 1000    |
|                                     | jmespath    | 0.181    | 1000    |
|                                     | jsonpath-ng | 1.488    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.534    | 12046   |
|                                     | jmespath    | 5.616    | 1000    |
|                                     | jsonpath-ng | 17.291   | 12046   |
| $..price                            | jmespath    | 5.506    | 1000    |
|                                     | query       | 5.566    | 12046   |
|                                     | jsonpath-ng | 71.310   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.032    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.090    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 3.44     | 4.13        | 4.74     | 243.95 | 324.7    |
| msgspec       | 3.54     | 4.61        | 5.18     | 218.71 | 324.8    |
| json (stdlib) | 5.27     | 5.80        | 6.13     | 173.74 | 326.8    |
| ujson         | 4.50     | 6.35        | 6.62     | 158.67 | 327.8    |
| strata        | 11.38    | 12.28       | 17.19    | 82.12  | 320.8    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 3.05     | 3.54        | 4.17     | 284.74 | 36.5     |
| orjson        | 2.52     | 3.56        | 3.93     | 282.69 | 36.4     |
| ujson         | 4.23     | 6.04        | 6.34     | 166.65 | 39.5     |
| strata        | 6.34     | 6.65        | 6.81     | 151.45 | 35.9     |
| json (stdlib) | 5.46     | 6.90        | 7.39     | 146.06 | 38.5     |
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
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 169203.12 | 1       | 241.8    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 159184.90 | 1       | 241.8    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 102095.97 | 1       | 241.8    |
| Extract all user IDs                        | jmespath    | 0.20     | 0.21        | 0.21     | 4912.05   | 1000    | 97.6     |
| Extract all user IDs                        | strata      | 1.13     | 1.16        | 1.18     | 871.25    | 1000    | 97.5     |
| Extract all user IDs                        | jsonpath-ng | 1.45     | 1.60        | 1.74     | 631.88    | 1000    | 98.4     |
| Extract all user names                      | jmespath    | 0.20     | 0.21        | 0.22     | 4849.05   | 1000    | 272.8    |
| Extract all user names                      | strata      | 1.10     | 1.10        | 1.10     | 915.23    | 1000    | 272.8    |
| Extract all user names                      | jsonpath-ng | 1.47     | 1.59        | 1.71     | 634.61    | 1000    | 272.8    |
| Extract nested timestamp field              | jmespath    | 0.52     | 0.53        | 0.53     | 1919.46   | 1000    | 158.8    |
| Extract nested timestamp field              | strata      | 1.12     | 1.13        | 1.14     | 888.92    | 1000    | 158.8    |
| Extract nested timestamp field              | jsonpath-ng | 2.04     | 2.37        | 2.70     | 424.84    | 1000    | 159.1    |
| Extract order item prices (double wildcard) | jmespath    | 5.37     | 5.71        | 6.05     | 176.55    | 1000    | 210.7    |
| Extract order item prices (double wildcard) | strata      | 12.59    | 12.77       | 12.94    | 78.96     | 12046   | 210.2    |
| Extract order item prices (double wildcard) | jsonpath-ng | 16.76    | 16.88       | 16.99    | 59.74     | 12046   | 215.0    |
| Filter users by age (numeric predicate)     | strata      | 1.00     | 1.07        | 1.15     | 939.01    | 794     | 298.4    |
| Filter users by age (numeric predicate)     | jmespath    | 1.09     | 1.25        | 1.42     | 805.59    | 794     | 298.4    |
| Recursively find all prices                 | strata      | 31.22    | 31.50       | 31.78    | 32.01     | 12046   | 324.9    |
| Recursively find all prices                 | jsonpath-ng | 74.18    | 79.42       | 84.65    | 12.70     | 12046   | 326.8    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 126681.79 | 10      | 347.5    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.03     | 44519.91  | 10      | 347.5    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 44113.77  | 10      | 347.5    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 59.46    | 61.79       | 64.11    | 130.54 | 414.5    |
| strata_cursor_reparse | 124.98   | 126.21      | 127.45   | 63.90  | 679.6    |
- Speedup (reuse vs reparse): 2.04x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.00     | 0.01        | 0.01     | 197332.71 | 1       | 55.7     |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.01     | 160086.79 | 0       | 55.7     |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 74839.25  | 1       | 55.7     |
| Extract all user IDs                        | jmespath            | 0.23     | 0.23        | 0.23     | 4355.00   | 1000    | 53.4     |
| Extract all user IDs                        | jsonpath-ng         | 1.77     | 2.01        | 2.25     | 500.40    | 1000    | 53.4     |
| Extract all user IDs                        | strata              | 2.47     | 2.48        | 2.49     | 406.21    | 0       | 53.4     |
| Extract all user IDs                        | strata_ndjson_fused | 4.44     | 4.78        | 5.13     | 210.51    | 0       | 53.4     |
| Extract all user IDs                        | strata_ndjson_full  | 8.92     | 9.07        | 9.22     | 111.05    | 0       | 53.4     |
| Extract all user names                      | jmespath            | 0.28     | 0.29        | 0.29     | 3507.58   | 1000    | 55.8     |
| Extract all user names                      | jsonpath-ng         | 1.75     | 1.76        | 1.78     | 571.01    | 1000    | 55.8     |
| Extract all user names                      | strata              | 2.52     | 2.52        | 2.53     | 399.04    | 0       | 55.7     |
| Extract all user names                      | strata_ndjson_fused | 4.42     | 4.44        | 4.47     | 226.66    | 0       | 55.8     |
| Extract all user names                      | strata_ndjson_full  | 8.85     | 8.99        | 9.12     | 112.07    | 0       | 55.8     |
| Extract nested timestamp field              | jmespath            | 0.56     | 0.56        | 0.56     | 1799.55   | 1000    | 53.5     |
| Extract nested timestamp field              | jsonpath-ng         | 2.21     | 2.25        | 2.28     | 448.34    | 1000    | 53.6     |
| Extract nested timestamp field              | strata              | 2.49     | 2.50        | 2.52     | 402.24    | 0       | 53.5     |
| Extract order item prices (double wildcard) | strata              | 2.49     | 2.52        | 2.56     | 399.13    | 0       | 53.6     |
| Extract order item prices (double wildcard) | jmespath            | 3.35     | 3.61        | 3.87     | 278.91    | 12046   | 53.7     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 17.87    | 18.10       | 18.32    | 55.65     | 12046   | 56.8     |
| Filter users by age (numeric predicate)     | strata              | 0.01     | 0.01        | 0.01     | 152986.62 | 0       | 55.8     |
| Filter users by age (numeric predicate)     | jmespath            | 1.06     | 1.08        | 1.09     | 936.16    | 794     | 55.8     |
| NDJSON root field (id)                      | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 147843.94 | 0       | 45.1     |
| NDJSON root field (id)                      | strata              | 1.87     | 1.95        | 2.03     | 515.96    | 1000    | 44.7     |
| NDJSON root field (id)                      | strata_ndjson_fused | 2.92     | 3.19        | 3.47     | 315.59    | 1000    | 45.1     |
| NDJSON root field (id)                      | strata_ndjson_full  | 9.92     | 10.38       | 10.84    | 97.05     | 1000    | 45.1     |
| Recursively find all prices                 | strata              | 33.42    | 33.82       | 34.21    | 29.78     | 878     | 56.1     |
| Recursively find all prices                 | jsonpath-ng         | 70.84    | 71.10       | 71.35    | 14.17     | 12046   | 58.1     |
| Slice first 10 users, extract IDs           | strata              | 0.01     | 0.01        | 0.01     | 175805.20 | 0       | 57.1     |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.01        | 0.01     | 108885.11 | 10      | 57.1     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 60353.97  | 10      | 56.3     |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 52.54    | 52.95       | 53.36    | 171.20 | 56.4     |
| strata_cursor_reparse | 123.44   | 124.44      | 125.44   | 72.84  | 64.7     |
- Speedup (reuse vs reparse): 2.35x
