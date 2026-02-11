# Strata Benchmark Results

Generated: 2026-02-12 01:09:07

## Environment

- Commit: 4b1934ea6b90104063201b6ada1fd68de1090565
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
| msgspec       | users.json   | 2.834    | 3.740       | 4.180    | 327.4    |
| orjson        | users.json   | 2.429    | 4.315       | 4.468    | 324.2    |
| ujson         | users.json   | 3.807    | 5.834       | 6.198    | 326.3    |
| json (stdlib) | users.json   | 5.509    | 6.046       | 6.212    | 328.4    |
| strata        | users.json   | 10.478   | 10.893      | 15.919   | 319.8    |
| orjson        | users.ndjson | 2.580    | 3.491       | 3.589    | 332.0    |
| msgspec       | users.ndjson | 2.688    | 3.617       | 3.747    | 334.1    |
| ujson         | users.ndjson | 3.801    | 5.286       | 5.381    | 334.1    |
| json (stdlib) | users.ndjson | 5.527    | 6.451       | 6.651    | 334.1    |
| strata        | users.ndjson | 6.544    | 7.418       | 7.432    | 330.1    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.038    | 1000    |
|                                     | jmespath    | 0.184    | 1000    |
|                                     | jsonpath-ng | 1.572    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.528    | 12046   |
|                                     | jmespath    | 5.647    | 1000    |
|                                     | jsonpath-ng | 18.076   | 12046   |
| $..price                            | jmespath    | 5.475    | 1000    |
|                                     | query       | 6.007    | 12046   |
|                                     | jsonpath-ng | 70.441   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.029    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.075    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 2.71     | 3.43        | 3.93     | 293.59 | 322.9    |
| msgspec       | 2.67     | 3.88        | 4.43     | 259.96 | 323.0    |
| ujson         | 3.81     | 5.50        | 5.77     | 183.35 | 326.1    |
| json (stdlib) | 5.45     | 5.98        | 6.39     | 168.66 | 325.1    |
| strata        | 10.00    | 10.21       | 15.95    | 98.77  | 319.1    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 2.77     | 3.61        | 3.98     | 278.91 | 36.6     |
| orjson        | 2.78     | 3.78        | 3.80     | 266.30 | 36.5     |
| ujson         | 3.98     | 5.44        | 5.70     | 184.99 | 39.7     |
| strata        | 5.80     | 6.30        | 6.35     | 159.75 | 36.0     |
| json (stdlib) | 5.50     | 6.52        | 6.71     | 154.54 | 38.7     |
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
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.01     | 179235.23 | 1       | 241.1    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 126024.71 | 1       | 241.1    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 116058.22 | 1       | 241.1    |
| Extract all user IDs                        | jmespath    | 0.22     | 0.22        | 0.22     | 4572.34   | 1000    | 97.9     |
| Extract all user IDs                        | strata      | 1.12     | 1.13        | 1.14     | 891.04    | 1000    | 97.8     |
| Extract all user IDs                        | jsonpath-ng | 1.77     | 1.84        | 1.90     | 548.88    | 1000    | 98.6     |
| Extract all user names                      | jmespath    | 0.19     | 0.21        | 0.23     | 4784.80   | 1000    | 288.7    |
| Extract all user names                      | strata      | 1.14     | 1.21        | 1.27     | 835.99    | 1000    | 288.7    |
| Extract all user names                      | jsonpath-ng | 1.52     | 1.56        | 1.60     | 646.30    | 1000    | 288.7    |
| Extract nested timestamp field              | jmespath    | 0.55     | 0.56        | 0.56     | 1809.51   | 1000    | 159.2    |
| Extract nested timestamp field              | strata      | 1.02     | 1.06        | 1.09     | 954.02    | 1000    | 159.1    |
| Extract nested timestamp field              | jsonpath-ng | 2.09     | 2.30        | 2.51     | 438.57    | 1000    | 159.5    |
| Extract order item prices (double wildcard) | jmespath    | 5.40     | 5.51        | 5.61     | 183.05    | 1000    | 192.0    |
| Extract order item prices (double wildcard) | strata      | 13.69    | 13.73       | 13.76    | 73.44     | 12046   | 191.5    |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.59    | 17.66       | 17.73    | 57.09     | 12046   | 195.4    |
| Filter users by age (numeric predicate)     | strata      | 0.98     | 0.99        | 1.00     | 1020.14   | 794     | 318.8    |
| Filter users by age (numeric predicate)     | jmespath    | 1.11     | 1.11        | 1.12     | 906.21    | 794     | 318.8    |
| Recursively find all prices                 | strata      | 31.28    | 31.51       | 31.74    | 32.00     | 12046   | 327.6    |
| Recursively find all prices                 | jsonpath-ng | 73.07    | 73.24       | 73.41    | 13.77     | 12046   | 329.3    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 87990.72  | 10      | 349.5    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.02        | 0.02     | 57609.67  | 10      | 349.5    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 46801.51  | 10      | 349.5    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 59.09    | 59.10       | 59.12    | 136.47 | 378.4    |
| strata_cursor_reparse | 122.34   | 123.92      | 125.50   | 65.09  | 841.7    |
- Speedup (reuse vs reparse): 2.10x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.01        | 0.01     | 177744.07 | 1       | 54.8     |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.01     | 160623.07 | 0       | 54.8     |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 128238.74 | 1       | 54.8     |
| Extract all user IDs                        | jmespath            | 0.24     | 0.26        | 0.28     | 3898.16   | 1000    | 53.5     |
| Extract all user IDs                        | jsonpath-ng         | 1.58     | 1.70        | 1.83     | 591.27    | 1000    | 53.5     |
| Extract all user IDs                        | strata              | 2.55     | 2.60        | 2.64     | 387.86    | 0       | 53.5     |
| Extract all user IDs                        | strata_ndjson_fused | 3.95     | 3.97        | 3.99     | 253.60    | 0       | 53.5     |
| Extract all user IDs                        | strata_ndjson_full  | 8.39     | 8.42        | 8.46     | 119.58    | 0       | 53.5     |
| Extract all user names                      | jmespath            | 0.20     | 0.21        | 0.22     | 4706.03   | 1000    | 54.9     |
| Extract all user names                      | jsonpath-ng         | 1.48     | 1.71        | 1.95     | 587.28    | 1000    | 54.9     |
| Extract all user names                      | strata              | 2.58     | 2.60        | 2.61     | 387.95    | 0       | 54.8     |
| Extract all user names                      | strata_ndjson_fused | 3.99     | 4.01        | 4.04     | 250.95    | 0       | 54.9     |
| Extract all user names                      | strata_ndjson_full  | 8.51     | 8.57        | 8.62     | 117.58    | 0       | 54.9     |
| Extract nested timestamp field              | jmespath            | 0.54     | 0.55        | 0.55     | 1846.07   | 1000    | 53.6     |
| Extract nested timestamp field              | jsonpath-ng         | 2.12     | 2.15        | 2.19     | 467.68    | 1000    | 53.7     |
| Extract nested timestamp field              | strata              | 2.51     | 2.51        | 2.51     | 401.77    | 0       | 53.6     |
| Extract order item prices (double wildcard) | strata              | 2.60     | 2.66        | 2.71     | 379.18    | 0       | 53.7     |
| Extract order item prices (double wildcard) | jmespath            | 3.63     | 3.67        | 3.72     | 274.15    | 12046   | 53.8     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 18.21    | 18.42       | 18.63    | 54.67     | 12046   | 56.9     |
| Filter users by age (numeric predicate)     | strata              | 0.01     | 0.01        | 0.01     | 183124.97 | 0       | 54.9     |
| Filter users by age (numeric predicate)     | jmespath            | 1.08     | 1.08        | 1.08     | 936.68    | 794     | 54.9     |
| NDJSON root field (id)                      | jsonpath-ng         | 0.00     | 0.01        | 0.01     | 117346.79 | 0       | 45.0     |
| NDJSON root field (id)                      | strata              | 2.00     | 2.06        | 2.12     | 488.37    | 1000    | 44.7     |
| NDJSON root field (id)                      | strata_ndjson_fused | 2.55     | 2.70        | 2.84     | 373.64    | 1000    | 45.0     |
| NDJSON root field (id)                      | strata_ndjson_full  | 7.81     | 7.84        | 7.87     | 128.53    | 1000    | 45.0     |
| Recursively find all prices                 | strata              | 31.49    | 31.53       | 31.56    | 31.94     | 878     | 55.0     |
| Recursively find all prices                 | jsonpath-ng         | 71.38    | 71.77       | 72.16    | 14.03     | 12046   | 56.8     |
| Slice first 10 users, extract IDs           | strata              | 0.00     | 0.01        | 0.01     | 195741.23 | 0       | 56.8     |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.02     | 54504.41  | 10      | 56.8     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.01     | 0.02        | 0.03     | 49381.60  | 10      | 56.0     |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 52.08    | 52.34       | 52.60    | 173.18 | 56.1     |
| strata_cursor_reparse | 119.40   | 119.48      | 119.55   | 75.87  | 64.4     |
- Speedup (reuse vs reparse): 2.28x
