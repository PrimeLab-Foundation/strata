# Strata Benchmark Results

Generated: 2026-02-09 23:59:18

## Environment

- Commit: becfa02e50f22f82704927d5188cb058570b2617
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
| orjson        | users.json   | 2.823    | 3.693       | 5.059    | 45.5     |
| msgspec       | users.json   | 2.937    | 3.782       | 3.879    | 48.6     |
| strata        | users.json   | 4.355    | 4.905       | 4.940    | 41.4     |
| json (stdlib) | users.json   | 5.799    | 6.083       | 6.336    | 49.1     |
| ujson         | users.json   | 4.795    | 6.988       | 8.298    | 46.5     |
| orjson        | users.ndjson | 2.827    | 3.533       | 3.908    | 54.4     |
| msgspec       | users.ndjson | 2.745    | 3.688       | 3.701    | 56.5     |
| ujson         | users.ndjson | 4.069    | 5.563       | 5.779    | 56.5     |
| strata        | users.ndjson | 4.870    | 5.602       | 5.782    | 53.5     |
| json (stdlib) | users.ndjson | 5.586    | 6.424       | 6.663    | 56.5     |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.032    | 1000    |
|                                     | jmespath    | 0.191    | 1000    |
|                                     | jsonpath-ng | 1.644    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.508    | 12046   |
|                                     | jmespath    | 5.526    | 1000    |
|                                     | jsonpath-ng | 18.276   | 12046   |
| $..price                            | jmespath    | 5.564    | 1000    |
|                                     | query       | 6.016    | 12046   |
|                                     | jsonpath-ng | 70.132   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.028    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.444    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| msgspec       | 2.60     | 3.65        | 4.89     | 43.1     |
| orjson        | 3.06     | 3.75        | 3.76     | 43.1     |
| strata        | 4.59     | 4.96        | 5.15     | 39.6     |
| json (stdlib) | 4.82     | 5.84        | 6.15     | 45.7     |
| ujson         | 3.78     | 5.92        | 5.95     | 46.2     |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| msgspec       | 3.06     | 3.64        | 3.90     | 36.3     |
| orjson        | 3.27     | 3.64        | 4.03     | 36.2     |
| strata        | 4.61     | 5.05        | 5.19     | 35.7     |
| ujson         | 4.24     | 5.36        | 5.96     | 39.3     |
| json (stdlib) | 5.39     | 6.54        | 6.71     | 38.3     |
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
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.00     | 1       | 64.2     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 1       | 64.2     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 64.2     |
| Extract all user IDs                        | jmespath    | 0.20     | 0.21        | 0.22     | 1000    | 49.4     |
| Extract all user IDs                        | strata      | 0.98     | 0.99        | 1.00     | 1000    | 49.2     |
| Extract all user IDs                        | jsonpath-ng | 1.68     | 1.71        | 1.73     | 1000    | 50.1     |
| Extract all user names                      | jmespath    | 0.18     | 0.20        | 0.22     | 1000    | 64.2     |
| Extract all user names                      | strata      | 0.93     | 0.95        | 0.96     | 1000    | 64.2     |
| Extract all user names                      | jsonpath-ng | 1.58     | 1.71        | 1.85     | 1000    | 64.2     |
| Extract nested timestamp field              | jmespath    | 0.52     | 0.55        | 0.58     | 1000    | 60.9     |
| Extract nested timestamp field              | strata      | 0.92     | 0.96        | 0.99     | 1000    | 60.9     |
| Extract nested timestamp field              | jsonpath-ng | 2.05     | 2.15        | 2.25     | 1000    | 61.3     |
| Extract order item prices (double wildcard) | jmespath    | 5.38     | 5.60        | 5.82     | 1000    | 62.0     |
| Extract order item prices (double wildcard) | strata      | 9.20     | 9.37        | 9.53     | 12046   | 61.5     |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.05    | 17.15       | 17.24    | 12046   | 66.4     |
| Filter users by age (numeric predicate)     | strata      | 1.04     | 1.08        | 1.13     | 794     | 64.2     |
| Filter users by age (numeric predicate)     | jmespath    | 1.09     | 1.14        | 1.19     | 794     | 64.2     |
| Recursively find all prices                 | strata      | 28.28    | 28.58       | 28.87    | 12046   | 64.7     |
| Recursively find all prices                 | jsonpath-ng | 71.15    | 71.39       | 71.63    | 12046   | 66.4     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.01        | 0.01     | 10      | 64.7     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.02        | 0.02     | 10      | 64.7     |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 64.7     |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.01     | 0       | 58.4     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 58.4     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 1       | 58.4     |
| Extract all user IDs                        | jmespath    | 0.25     | 0.25        | 0.25     | 1000    | 46.6     |
| Extract all user IDs                        | jsonpath-ng | 1.54     | 1.56        | 1.58     | 1000    | 47.2     |
| Extract all user IDs                        | strata      | 2.50     | 2.54        | 2.59     | 0       | 46.6     |
| Extract all user names                      | jmespath    | 0.22     | 0.22        | 0.22     | 1000    | 58.4     |
| Extract all user names                      | jsonpath-ng | 1.54     | 1.56        | 1.59     | 1000    | 58.4     |
| Extract all user names                      | strata      | 2.60     | 2.60        | 2.61     | 0       | 58.4     |
| Extract nested timestamp field              | jmespath    | 0.56     | 0.58        | 0.60     | 1000    | 57.9     |
| Extract nested timestamp field              | jsonpath-ng | 2.09     | 2.18        | 2.27     | 1000    | 58.2     |
| Extract nested timestamp field              | strata      | 2.53     | 2.53        | 2.54     | 0       | 57.9     |
| Extract order item prices (double wildcard) | strata      | 2.45     | 2.50        | 2.54     | 0       | 58.2     |
| Extract order item prices (double wildcard) | jmespath    | 3.61     | 3.64        | 3.67     | 12046   | 58.3     |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.31    | 18.37       | 18.42    | 12046   | 60.4     |
| Filter users by age (numeric predicate)     | strata      | 0.01     | 0.01        | 0.01     | 0       | 58.4     |
| Filter users by age (numeric predicate)     | jmespath    | 1.12     | 1.17        | 1.23     | 794     | 58.4     |
| Recursively find all prices                 | strata      | 28.79    | 28.79       | 28.80    | 878     | 59.2     |
| Recursively find all prices                 | jsonpath-ng | 72.30    | 72.52       | 72.74    | 12046   | 62.4     |
| Slice first 10 users, extract IDs           | strata      | 0.00     | 0.01        | 0.01     | 0       | 61.4     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 10      | 61.4     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 10      | 60.6     |
