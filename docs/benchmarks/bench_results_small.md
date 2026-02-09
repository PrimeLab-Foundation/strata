# Strata Benchmark Results

Generated: 2026-02-09 19:07:38

## Environment

- Commit: d18dd88f6a1f4b1ea0013c01ecb1e2d84d0466ca
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
| orjson        | users.json   | 2.587    | 3.573       | 4.613    | 45.0     |
| msgspec       | users.json   | 2.876    | 3.755       | 3.778    | 49.1     |
| strata        | users.json   | 4.357    | 4.859       | 5.003    | 41.9     |
| ujson         | users.json   | 3.956    | 5.572       | 5.595    | 47.0     |
| json (stdlib) | users.json   | 5.437    | 6.240       | 6.405    | 49.1     |
| msgspec       | users.ndjson | 2.842    | 3.859       | 3.928    | 54.1     |
| orjson        | users.ndjson | 2.866    | 3.973       | 4.332    | 52.1     |
| ujson         | users.ndjson | 4.144    | 5.730       | 5.743    | 54.1     |
| strata        | users.ndjson | 4.569    | 6.243       | 6.302    | 51.1     |
| json (stdlib) | users.ndjson | 5.822    | 6.644       | 6.854    | 54.1     |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.035    | 1000    |
|                                     | jmespath    | 0.182    | 1000    |
|                                     | jsonpath-ng | 1.625    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.588    | 12046   |
|                                     | jmespath    | 5.809    | 1000    |
|                                     | jsonpath-ng | 18.283   | 12046   |
| $..price                            | jmespath    | 6.107    | 1000    |
|                                     | query       | 6.260    | 12046   |
|                                     | jsonpath-ng | 73.894   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.029    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.587    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 2.64     | 3.58        | 4.09     | 43.1     |
| msgspec       | 2.87     | 3.93        | 4.19     | 43.2     |
| strata        | 4.75     | 5.02        | 5.13     | 39.6     |
| ujson         | 4.28     | 5.86        | 6.27     | 46.2     |
| json (stdlib) | 5.24     | 6.44        | 6.54     | 45.7     |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| msgspec       | 2.91     | 3.67        | 4.06     | 36.2     |
| orjson        | 2.77     | 3.74        | 3.96     | 36.1     |
| strata        | 3.94     | 4.44        | 4.66     | 35.6     |
| ujson         | 4.25     | 5.65        | 5.96     | 39.3     |
| json (stdlib) | 5.61     | 6.88        | 7.19     | 38.3     |
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
| Deep path navigation                        | jmespath    | 0.00     | 0.01        | 0.01     | 1       | 63.2     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 1       | 63.2     |
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.01     | 1       | 63.2     |
| Extract all user IDs                        | jmespath    | 0.20     | 0.22        | 0.24     | 1000    | 49.3     |
| Extract all user IDs                        | strata      | 1.42     | 1.45        | 1.48     | 1000    | 49.2     |
| Extract all user IDs                        | jsonpath-ng | 1.60     | 1.70        | 1.81     | 1000    | 50.1     |
| Extract all user names                      | jmespath    | 0.20     | 0.21        | 0.21     | 1000    | 63.3     |
| Extract all user names                      | strata      | 0.92     | 0.93        | 0.95     | 1000    | 63.3     |
| Extract all user names                      | jsonpath-ng | 1.60     | 1.72        | 1.83     | 1000    | 63.3     |
| Extract nested timestamp field              | jmespath    | 0.55     | 0.56        | 0.57     | 1000    | 60.8     |
| Extract nested timestamp field              | strata      | 0.91     | 0.92        | 0.92     | 1000    | 60.7     |
| Extract nested timestamp field              | jsonpath-ng | 2.17     | 2.17        | 2.18     | 1000    | 61.1     |
| Extract order item prices (double wildcard) | jmespath    | 5.66     | 5.71        | 5.75     | 1000    | 61.9     |
| Extract order item prices (double wildcard) | strata      | 9.66     | 9.79        | 9.92     | 12046   | 61.3     |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.42    | 19.12       | 19.83    | 12046   | 65.5     |
| Filter users by age (numeric predicate)     | strata      | 1.02     | 1.05        | 1.09     | 794     | 63.3     |
| Filter users by age (numeric predicate)     | jmespath    | 1.19     | 1.23        | 1.26     | 794     | 63.3     |
| Recursively find all prices                 | strata      | 29.62    | 29.76       | 29.89    | 12046   | 63.7     |
| Recursively find all prices                 | jsonpath-ng | 74.34    | 74.34       | 74.35    | 12046   | 65.4     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 10      | 63.7     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 10      | 63.7     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.03     | 10      | 63.7     |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 63.2     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 1       | 63.2     |
| Deep path navigation                        | strata      | 9.21     | 9.37        | 9.54     | 1       | 63.2     |
| Extract all user IDs                        | jmespath    | 0.26     | 0.28        | 0.30     | 1000    | 58.2     |
| Extract all user IDs                        | jsonpath-ng | 2.33     | 2.51        | 2.70     | 1000    | 58.8     |
| Extract all user IDs                        | strata      | 10.02    | 10.16       | 10.29    | 1000    | 58.1     |
| Extract all user names                      | jmespath    | 0.25     | 0.28        | 0.31     | 1000    | 63.7     |
| Extract all user names                      | jsonpath-ng | 2.29     | 2.31        | 2.34     | 1000    | 63.7     |
| Extract all user names                      | strata      | 10.24    | 10.31       | 10.38    | 1000    | 63.7     |
| Extract nested timestamp field              | jmespath    | 0.61     | 0.63        | 0.64     | 1000    | 61.5     |
| Extract nested timestamp field              | jsonpath-ng | 3.37     | 3.40        | 3.43     | 1000    | 62.1     |
| Extract nested timestamp field              | strata      | 10.41    | 10.59       | 10.77    | 1000    | 61.5     |
| Extract order item prices (double wildcard) | jmespath    | 3.77     | 3.79        | 3.82     | 12046   | 61.4     |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.42    | 18.50       | 18.58    | 12046   | 62.8     |
| Extract order item prices (double wildcard) | strata      | 20.06    | 20.82       | 21.59    | 12046   | 61.2     |
| Filter users by age (numeric predicate)     | jmespath    | 1.21     | 1.24        | 1.26     | 794     | 63.9     |
| Filter users by age (numeric predicate)     | strata      | 9.77     | 9.91        | 10.06    | 794     | 63.9     |
| Recursively find all prices                 | strata      | 38.61    | 39.30       | 39.99    | 12046   | 65.4     |
| Recursively find all prices                 | jsonpath-ng | 73.18    | 73.58       | 73.98    | 12046   | 66.9     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.03     | 10      | 67.0     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 10      | 67.0     |
| Slice first 10 users, extract IDs           | strata      | 9.25     | 9.37        | 9.50     | 10      | 67.0     |
