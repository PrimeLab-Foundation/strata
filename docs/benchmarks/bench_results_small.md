# Strata Benchmark Results

Generated: 2026-02-09 17:41:54

## Environment

- Commit: b1c61c3a33340b140cf44500ca98dc46a704f09c
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
| orjson        | users.json   | 3.491    | 5.209       | 5.272    | 46.7     |
| msgspec       | users.json   | 3.962    | 5.431       | 5.603    | 49.9     |
| strata        | users.json   | 5.333    | 5.792       | 55.360   | 42.7     |
| ujson         | users.json   | 5.167    | 7.532       | 8.910    | 47.8     |
| json (stdlib) | users.json   | 6.441    | 8.034       | 8.271    | 49.9     |
| msgspec       | users.ndjson | 3.362    | 4.831       | 4.981    | 48.2     |
| orjson        | users.ndjson | 3.719    | 5.403       | 6.324    | 46.2     |
| ujson         | users.ndjson | 4.977    | 7.599       | 8.422    | 48.2     |
| strata        | users.ndjson | 7.407    | 8.874       | 9.498    | 45.1     |
| json (stdlib) | users.ndjson | 8.832    | 9.223       | 10.046   | 48.2     |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.046    | 1000    |
|                                     | jmespath    | 0.258    | 1000    |
|                                     | jsonpath-ng | 2.362    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.666    | 12046   |
|                                     | jmespath    | 7.199    | 1000    |
|                                     | jsonpath-ng | 21.997   | 12046   |
| $..price                            | query       | 6.681    | 12046   |
|                                     | jmespath    | 7.146    | 1000    |
|                                     | jsonpath-ng | 79.260   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.063    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 7.557    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 3.72     | 5.34        | 5.63     | 44.3     |
| msgspec       | 3.62     | 5.55        | 6.02     | 44.4     |
| strata        | 4.98     | 5.68        | 6.15     | 40.8     |
| ujson         | 5.15     | 7.70        | 7.77     | 47.5     |
| json (stdlib) | 6.30     | 8.72        | 9.52     | 46.5     |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 3.05     | 5.00        | 5.52     | 36.3     |
| msgspec       | 4.99     | 5.47        | 5.72     | 36.4     |
| strata        | 5.71     | 5.97        | 6.05     | 35.8     |
| ujson         | 4.72     | 7.58        | 7.74     | 39.4     |
| json (stdlib) | 6.64     | 8.15        | 8.59     | 38.4     |
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
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 1       | 66.2     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.03     | 1       | 66.2     |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 1       | 66.2     |
| Extract all user IDs                        | jmespath    | 0.24     | 0.26        | 0.28     | 1000    | 50.6     |
| Extract all user IDs                        | strata      | 0.94     | 0.94        | 0.94     | 1000    | 50.5     |
| Extract all user IDs                        | jsonpath-ng | 2.46     | 2.63        | 2.81     | 1000    | 51.5     |
| Extract all user names                      | jmespath    | 0.26     | 0.29        | 0.32     | 1000    | 66.2     |
| Extract all user names                      | strata      | 1.47     | 1.57        | 1.66     | 1000    | 66.2     |
| Extract all user names                      | jsonpath-ng | 2.61     | 2.65        | 2.68     | 1000    | 66.2     |
| Extract nested timestamp field              | jmespath    | 0.60     | 0.61        | 0.61     | 1000    | 62.1     |
| Extract nested timestamp field              | strata      | 1.01     | 1.02        | 1.03     | 1000    | 62.1     |
| Extract nested timestamp field              | jsonpath-ng | 3.07     | 3.07        | 3.08     | 1000    | 62.4     |
| Extract order item prices (double wildcard) | jmespath    | 6.58     | 6.70        | 6.82     | 1000    | 64.9     |
| Extract order item prices (double wildcard) | strata      | 11.09    | 11.41       | 11.73    | 12046   | 64.3     |
| Extract order item prices (double wildcard) | jsonpath-ng | 21.00    | 21.25       | 21.50    | 12046   | 68.4     |
| Filter users by age (numeric predicate)     | strata      | 0.80     | 1.00        | 1.21     | 794     | 66.3     |
| Filter users by age (numeric predicate)     | jmespath    | 1.35     | 1.40        | 1.45     | 794     | 66.3     |
| Recursively find all prices                 | strata      | 33.06    | 34.09       | 35.12    | 12046   | 66.9     |
| Recursively find all prices                 | jsonpath-ng | 81.74    | 82.16       | 82.57    | 12046   | 68.7     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.02        | 0.02     | 10      | 67.0     |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 67.0     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 10      | 67.0     |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 1       | 67.2     |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1       | 67.2     |
| Deep path navigation                        | strata      | 10.49    | 10.56       | 10.64    | 1       | 67.2     |
| Extract all user IDs                        | jmespath    | 0.41     | 0.41        | 0.41     | 1000    | 59.8     |
| Extract all user IDs                        | jsonpath-ng | 3.30     | 3.51        | 3.72     | 1000    | 60.5     |
| Extract all user IDs                        | strata      | 11.50    | 11.64       | 11.79    | 1000    | 59.6     |
| Extract all user names                      | jmespath    | 0.47     | 0.50        | 0.53     | 1000    | 67.8     |
| Extract all user names                      | jsonpath-ng | 3.82     | 4.17        | 4.52     | 1000    | 67.8     |
| Extract all user names                      | strata      | 11.53    | 11.62       | 11.70    | 1000    | 67.8     |
| Extract nested timestamp field              | jmespath    | 1.01     | 1.06        | 1.11     | 1000    | 63.3     |
| Extract nested timestamp field              | jsonpath-ng | 4.84     | 4.85        | 4.86     | 1000    | 63.9     |
| Extract nested timestamp field              | strata      | 11.35    | 11.49       | 11.63    | 1000    | 63.3     |
| Extract order item prices (double wildcard) | jmespath    | 4.43     | 4.56        | 4.69     | 12046   | 65.3     |
| Extract order item prices (double wildcard) | jsonpath-ng | 21.61    | 22.09       | 22.56    | 12046   | 66.8     |
| Extract order item prices (double wildcard) | strata      | 24.02    | 24.17       | 24.33    | 12046   | 65.1     |
| Filter users by age (numeric predicate)     | jmespath    | 1.42     | 1.47        | 1.52     | 794     | 67.9     |
| Filter users by age (numeric predicate)     | strata      | 11.36    | 11.64       | 11.93    | 794     | 67.9     |
| Recursively find all prices                 | strata      | 46.39    | 46.46       | 46.54    | 12046   | 69.4     |
| Recursively find all prices                 | jsonpath-ng | 82.72    | 82.98       | 83.23    | 12046   | 70.9     |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 10      | 71.0     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 10      | 71.0     |
| Slice first 10 users, extract IDs           | strata      | 10.46    | 10.53       | 10.61    | 10      | 71.0     |
