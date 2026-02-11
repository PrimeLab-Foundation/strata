# Strata Benchmark Results

Generated: 2026-02-11 23:34:58

## Environment

- Commit: ec4905efccfcf6fe97a503fe7701cd9771c3d6f0
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
| orjson        | users.json   | 2.559    | 3.807       | 3.946    | 348.9    |
| msgspec       | users.json   | 2.721    | 3.866       | 4.847    | 353.0    |
| ujson         | users.json   | 3.797    | 6.112       | 6.240    | 350.9    |
| json (stdlib) | users.json   | 5.504    | 6.305       | 6.762    | 353.0    |
| strata        | users.json   | 11.370   | 12.125      | 16.131   | 344.4    |
| orjson        | users.ndjson | 2.854    | 3.667       | 3.674    | 356.5    |
| msgspec       | users.ndjson | 2.995    | 3.683       | 3.735    | 358.5    |
| ujson         | users.ndjson | 4.004    | 5.742       | 6.069    | 358.5    |
| json (stdlib) | users.ndjson | 5.639    | 6.788       | 6.906    | 358.5    |
| strata        | users.ndjson | 6.828    | 7.725       | 7.932    | 354.5    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.026    | 1000    |
|                                     | jmespath    | 0.179    | 1000    |
|                                     | jsonpath-ng | 1.637    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.578    | 12046   |
|                                     | jmespath    | 5.639    | 1000    |
|                                     | jsonpath-ng | 18.581   | 12046   |
| $..price                            | jmespath    | 5.892    | 1000    |
|                                     | query       | 6.157    | 12046   |
|                                     | jsonpath-ng | 72.839   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.032    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.464    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 2.82     | 3.46        | 3.92     | 322.9    |
| msgspec       | 2.71     | 3.88        | 4.76     | 323.0    |
| ujson         | 3.76     | 5.70        | 5.87     | 326.0    |
| json (stdlib) | 5.12     | 6.13        | 6.47     | 325.0    |
| strata        | 10.49    | 10.76       | 16.75    | 319.1    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 2.85     | 3.54        | 3.62     | 36.5     |
| msgspec       | 2.88     | 3.71        | 4.18     | 36.6     |
| ujson         | 3.99     | 5.59        | 6.26     | 39.6     |
| strata        | 6.04     | 6.52        | 6.74     | 37.0     |
| json (stdlib) | 5.66     | 6.60        | 6.95     | 38.6     |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.01     | 1       | 224.8    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 224.8    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 1       | 224.8    |
| Extract all user IDs                        | jmespath    | 0.22     | 0.23        | 0.24     | 1000    | 97.6     |
| Extract all user IDs                        | strata      | 1.22     | 1.26        | 1.29     | 1000    | 97.5     |
| Extract all user IDs                        | jsonpath-ng | 1.62     | 1.69        | 1.76     | 1000    | 98.4     |
| Extract all user names                      | jmespath    | 0.21     | 0.22        | 0.22     | 1000    | 257.3    |
| Extract all user names                      | strata      | 1.24     | 1.26        | 1.27     | 1000    | 257.3    |
| Extract all user names                      | jsonpath-ng | 1.64     | 1.86        | 2.09     | 1000    | 257.3    |
| Extract nested timestamp field              | jmespath    | 0.55     | 0.57        | 0.60     | 1000    | 158.9    |
| Extract nested timestamp field              | strata      | 1.08     | 1.09        | 1.09     | 1000    | 158.8    |
| Extract nested timestamp field              | jsonpath-ng | 2.14     | 2.40        | 2.66     | 1000    | 159.3    |
| Extract order item prices (double wildcard) | jmespath    | 5.42     | 5.63        | 5.85     | 1000    | 191.8    |
| Extract order item prices (double wildcard) | strata      | 14.33    | 14.34       | 14.35    | 12046   | 191.3    |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.85    | 17.96       | 18.08    | 12046   | 195.1    |
| Filter users by age (numeric predicate)     | strata      | 0.90     | 0.95        | 1.01     | 794     | 283.4    |
| Filter users by age (numeric predicate)     | jmespath    | 1.17     | 1.19        | 1.21     | 794     | 283.4    |
| Recursively find all prices                 | strata      | 32.32    | 32.81       | 33.30    | 12046   | 309.1    |
| Recursively find all prices                 | jsonpath-ng | 73.78    | 73.79       | 73.79    | 12046   | 310.8    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 10      | 330.5    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 10      | 330.5    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.02     | 10      | 330.5    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.01     | 0       | 56.0     |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 1       | 56.0     |
| Deep path navigation                        | jmespath            | 0.01     | 0.02        | 0.02     | 1       | 56.0     |
| Extract all user IDs                        | jmespath            | 0.23     | 0.24        | 0.26     | 1000    | 53.7     |
| Extract all user IDs                        | jsonpath-ng         | 1.59     | 1.77        | 1.95     | 1000    | 53.7     |
| Extract all user IDs                        | strata              | 2.66     | 2.70        | 2.74     | 0       | 53.6     |
| Extract all user IDs                        | strata_ndjson_fused | 4.01     | 4.07        | 4.13     | 0       | 53.7     |
| Extract all user IDs                        | strata_ndjson_full  | 9.00     | 9.01        | 9.01     | 0       | 53.7     |
| Extract all user names                      | jmespath            | 0.24     | 0.25        | 0.26     | 1000    | 56.1     |
| Extract all user names                      | jsonpath-ng         | 1.66     | 1.74        | 1.82     | 1000    | 56.1     |
| Extract all user names                      | strata              | 2.63     | 2.71        | 2.79     | 0       | 56.0     |
| Extract all user names                      | strata_ndjson_fused | 4.18     | 4.19        | 4.21     | 0       | 56.1     |
| Extract all user names                      | strata_ndjson_full  | 8.84     | 8.86        | 8.88     | 0       | 56.0     |
| Extract nested timestamp field              | jmespath            | 0.60     | 0.64        | 0.67     | 1000    | 53.7     |
| Extract nested timestamp field              | jsonpath-ng         | 2.39     | 2.42        | 2.45     | 1000    | 53.8     |
| Extract nested timestamp field              | strata              | 2.67     | 2.68        | 2.70     | 0       | 53.7     |
| Extract order item prices (double wildcard) | strata              | 2.63     | 2.66        | 2.69     | 0       | 53.8     |
| Extract order item prices (double wildcard) | jmespath            | 3.63     | 3.65        | 3.67     | 12046   | 53.9     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 18.25    | 18.83       | 19.42    | 12046   | 57.0     |
| Filter users by age (numeric predicate)     | strata              | 0.01     | 0.01        | 0.01     | 0       | 56.1     |
| Filter users by age (numeric predicate)     | jmespath            | 1.18     | 1.25        | 1.32     | 794     | 56.1     |
| NDJSON root field (id)                      | jsonpath-ng         | 0.00     | 0.01        | 0.01     | 0       | 45.2     |
| NDJSON root field (id)                      | strata              | 2.03     | 2.06        | 2.10     | 1000    | 44.8     |
| NDJSON root field (id)                      | strata_ndjson_fused | 3.05     | 3.10        | 3.16     | 1000    | 45.2     |
| NDJSON root field (id)                      | strata_ndjson_full  | 8.11     | 8.35        | 8.59     | 1000    | 45.2     |
| Recursively find all prices                 | strata              | 32.85    | 32.91       | 32.96    | 878     | 56.2     |
| Recursively find all prices                 | jsonpath-ng         | 74.01    | 74.63       | 75.26    | 12046   | 58.0     |
| Slice first 10 users, extract IDs           | strata              | 0.01     | 0.01        | 0.01     | 0       | 57.0     |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.01        | 0.01     | 10      | 57.0     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 10      | 56.2     |
