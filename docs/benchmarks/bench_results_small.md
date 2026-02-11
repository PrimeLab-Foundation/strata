# Strata Benchmark Results

Generated: 2026-02-11 21:07:32

## Environment

- Commit: 85c029c4d7f2d9b0ae7d1507059a58bf4cb96a81
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
| msgspec       | users.json   | 2.991    | 3.756       | 3.951    | 48.7     |
| orjson        | users.json   | 2.425    | 4.017       | 4.150    | 44.5     |
| strata        | users.json   | 4.145    | 4.803       | 4.833    | 41.4     |
| ujson         | users.json   | 3.881    | 5.823       | 6.578    | 46.6     |
| json (stdlib) | users.json   | 5.285    | 6.154       | 6.239    | 49.2     |
| msgspec       | users.ndjson | 2.834    | 3.647       | 3.804    | 53.9     |
| orjson        | users.ndjson | 3.079    | 3.743       | 3.924    | 51.9     |
| ujson         | users.ndjson | 4.002    | 5.406       | 5.720    | 53.9     |
| strata        | users.ndjson | 4.575    | 5.854       | 6.008    | 50.9     |
| json (stdlib) | users.ndjson | 5.737    | 6.687       | 6.696    | 53.9     |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.026    | 1000    |
|                                     | jmespath    | 0.180    | 1000    |
|                                     | jsonpath-ng | 1.612    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.537    | 12046   |
|                                     | jmespath    | 5.526    | 1000    |
|                                     | jsonpath-ng | 18.354   | 12046   |
| $..price                            | query       | 6.122    | 12046   |
|                                     | jmespath    | 7.436    | 1000    |
|                                     | jsonpath-ng | 72.393   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.035    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.343    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 2.74     | 3.76        | 4.32     | 43.1     |
| msgspec       | 3.10     | 3.88        | 4.00     | 43.2     |
| strata        | 4.47     | 5.17        | 5.20     | 39.7     |
| ujson         | 4.06     | 5.55        | 6.82     | 46.3     |
| json (stdlib) | 5.56     | 6.87        | 7.02     | 46.7     |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| msgspec       | 2.78     | 3.71        | 4.26     | 36.0     |
| orjson        | 2.60     | 3.79        | 4.09     | 36.0     |
| strata        | 4.10     | 4.73        | 4.97     | 35.6     |
| ujson         | 3.94     | 5.39        | 5.96     | 39.1     |
| json (stdlib) | 5.70     | 7.11        | 7.14     | 38.1     |
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
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.00     | 1       | 62.8     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 1       | 62.8     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 62.8     |
| Extract all user IDs                        | jmespath    | 0.21     | 0.21        | 0.21     | 1000    | 49.2     |
| Extract all user IDs                        | strata      | 0.99     | 0.99        | 1.00     | 1000    | 49.2     |
| Extract all user IDs                        | jsonpath-ng | 1.61     | 1.66        | 1.71     | 1000    | 49.9     |
| Extract all user names                      | jmespath    | 0.19     | 0.23        | 0.26     | 1000    | 62.9     |
| Extract all user names                      | strata      | 0.84     | 0.90        | 0.95     | 1000    | 62.9     |
| Extract all user names                      | jsonpath-ng | 1.55     | 1.65        | 1.76     | 1000    | 62.9     |
| Extract nested timestamp field              | jmespath    | 0.54     | 0.55        | 0.56     | 1000    | 60.5     |
| Extract nested timestamp field              | strata      | 1.30     | 1.32        | 1.34     | 1000    | 60.5     |
| Extract nested timestamp field              | jsonpath-ng | 2.09     | 2.16        | 2.23     | 1000    | 60.9     |
| Extract order item prices (double wildcard) | jmespath    | 5.84     | 5.91        | 5.98     | 1000    | 61.6     |
| Extract order item prices (double wildcard) | strata      | 9.48     | 9.53        | 9.58     | 12046   | 61.1     |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.54    | 17.58       | 17.62    | 12046   | 65.0     |
| Filter users by age (numeric predicate)     | strata      | 0.85     | 0.86        | 0.87     | 794     | 62.9     |
| Filter users by age (numeric predicate)     | jmespath    | 1.10     | 1.11        | 1.12     | 794     | 62.9     |
| Recursively find all prices                 | strata      | 28.59    | 28.71       | 28.83    | 12046   | 63.3     |
| Recursively find all prices                 | jsonpath-ng | 72.95    | 73.18       | 73.42    | 12046   | 64.9     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 10      | 63.3     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.02     | 10      | 63.3     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.02        | 0.02     | 10      | 63.3     |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 0       | 60.5     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 1       | 60.5     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 60.5     |
| Extract all user IDs                        | jmespath    | 0.24     | 0.25        | 0.26     | 1000    | 46.5     |
| Extract all user IDs                        | jsonpath-ng | 1.78     | 1.79        | 1.80     | 1000    | 47.2     |
| Extract all user IDs                        | strata      | 2.58     | 2.59        | 2.59     | 0       | 46.3     |
| Extract all user names                      | jmespath    | 0.23     | 0.23        | 0.23     | 1000    | 60.7     |
| Extract all user names                      | jsonpath-ng | 1.48     | 1.65        | 1.81     | 1000    | 60.7     |
| Extract all user names                      | strata      | 2.54     | 2.61        | 2.68     | 0       | 60.7     |
| Extract nested timestamp field              | jmespath    | 0.58     | 0.60        | 0.63     | 1000    | 57.8     |
| Extract nested timestamp field              | jsonpath-ng | 2.14     | 2.23        | 2.31     | 1000    | 58.1     |
| Extract nested timestamp field              | strata      | 2.50     | 2.56        | 2.63     | 0       | 57.8     |
| Extract order item prices (double wildcard) | strata      | 2.35     | 2.45        | 2.55     | 0       | 58.1     |
| Extract order item prices (double wildcard) | jmespath    | 3.45     | 3.51        | 3.57     | 12046   | 58.4     |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.47    | 18.54       | 18.62    | 12046   | 61.5     |
| Filter users by age (numeric predicate)     | strata      | 0.01     | 0.01        | 0.01     | 0       | 60.7     |
| Filter users by age (numeric predicate)     | jmespath    | 1.12     | 1.18        | 1.24     | 794     | 60.7     |
| Recursively find all prices                 | strata      | 29.13    | 29.14       | 29.14    | 878     | 60.8     |
| Recursively find all prices                 | jsonpath-ng | 72.37    | 72.54       | 72.70    | 12046   | 61.6     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 0       | 61.6     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 10      | 61.6     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.01        | 0.01     | 10      | 60.8     |
