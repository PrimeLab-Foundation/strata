# Strata Benchmark Results

Generated: 2026-02-11 11:12:11

## Environment

- Commit: af3826e258bad9e6738f3a70f573dcc93723b4e5
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
| orjson        | users.json   | 2.658    | 4.186       | 4.254    | 44.5     |
| msgspec       | users.json   | 3.241    | 4.404       | 4.422    | 48.7     |
| strata        | users.json   | 5.029    | 5.286       | 5.717    | 41.4     |
| json (stdlib) | users.json   | 4.983    | 5.819       | 6.030    | 49.1     |
| ujson         | users.json   | 4.454    | 6.476       | 8.037    | 46.6     |
| orjson        | users.ndjson | 2.498    | 3.786       | 3.915    | 51.9     |
| msgspec       | users.ndjson | 2.689    | 3.992       | 4.158    | 54.0     |
| ujson         | users.ndjson | 3.786    | 5.450       | 5.516    | 54.0     |
| strata        | users.ndjson | 5.123    | 5.457       | 5.507    | 50.9     |
| json (stdlib) | users.ndjson | 5.679    | 6.203       | 6.527    | 54.0     |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.034    | 1000    |
|                                     | jmespath    | 0.188    | 1000    |
|                                     | jsonpath-ng | 1.510    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.572    | 12046   |
|                                     | jmespath    | 5.245    | 1000    |
|                                     | jsonpath-ng | 17.460   | 12046   |
| $..price                            | jmespath    | 5.528    | 1000    |
|                                     | query       | 5.627    | 12046   |
|                                     | jsonpath-ng | 68.194   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.035    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.017    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| msgspec       | 2.82     | 3.62        | 4.48     | 43.3     |
| orjson        | 2.58     | 3.65        | 3.98     | 43.2     |
| strata        | 4.27     | 4.60        | 4.84     | 39.7     |
| ujson         | 3.86     | 5.65        | 5.99     | 46.3     |
| json (stdlib) | 4.88     | 5.95        | 6.01     | 45.8     |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 2.77     | 3.37        | 3.76     | 36.0     |
| msgspec       | 2.72     | 3.52        | 4.13     | 36.0     |
| strata        | 3.68     | 4.06        | 4.12     | 35.5     |
| ujson         | 3.80     | 5.56        | 6.22     | 39.1     |
| json (stdlib) | 5.54     | 6.16        | 6.63     | 38.1     |
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
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 63.0     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 1       | 63.0     |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 1       | 63.0     |
| Extract all user IDs                        | jmespath    | 0.19     | 0.20        | 0.22     | 1000    | 49.5     |
| Extract all user IDs                        | strata      | 1.21     | 1.23        | 1.25     | 1000    | 49.3     |
| Extract all user IDs                        | jsonpath-ng | 1.62     | 1.69        | 1.76     | 1000    | 50.2     |
| Extract all user names                      | jmespath    | 0.22     | 0.22        | 0.22     | 1000    | 63.0     |
| Extract all user names                      | strata      | 1.30     | 1.35        | 1.39     | 1000    | 63.0     |
| Extract all user names                      | jsonpath-ng | 1.67     | 1.72        | 1.77     | 1000    | 63.0     |
| Extract nested timestamp field              | jmespath    | 0.54     | 0.58        | 0.62     | 1000    | 60.8     |
| Extract nested timestamp field              | strata      | 1.06     | 1.12        | 1.17     | 1000    | 60.8     |
| Extract nested timestamp field              | jsonpath-ng | 2.23     | 2.25        | 2.27     | 1000    | 61.1     |
| Extract order item prices (double wildcard) | jmespath    | 5.24     | 5.43        | 5.63     | 1000    | 61.9     |
| Extract order item prices (double wildcard) | strata      | 9.44     | 9.51        | 9.57     | 12046   | 61.3     |
| Extract order item prices (double wildcard) | jsonpath-ng | 16.57    | 16.65       | 16.72    | 12046   | 65.2     |
| Filter users by age (numeric predicate)     | strata      | 0.81     | 0.83        | 0.86     | 794     | 63.0     |
| Filter users by age (numeric predicate)     | jmespath    | 1.05     | 1.05        | 1.06     | 794     | 63.0     |
| Recursively find all prices                 | strata      | 27.71    | 27.84       | 27.98    | 12046   | 63.4     |
| Recursively find all prices                 | jsonpath-ng | 69.84    | 70.51       | 71.18    | 12046   | 65.1     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 10      | 63.4     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.02        | 0.02     | 10      | 63.4     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 10      | 63.4     |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 0       | 60.6     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 1       | 60.6     |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 1       | 60.6     |
| Extract all user IDs                        | jmespath    | 0.21     | 0.21        | 0.21     | 1000    | 46.5     |
| Extract all user IDs                        | jsonpath-ng | 1.47     | 1.48        | 1.50     | 1000    | 47.2     |
| Extract all user IDs                        | strata      | 2.51     | 2.54        | 2.58     | 0       | 46.4     |
| Extract all user names                      | jmespath    | 0.21     | 0.25        | 0.30     | 1000    | 60.6     |
| Extract all user names                      | jsonpath-ng | 1.49     | 1.83        | 2.16     | 1000    | 60.6     |
| Extract all user names                      | strata      | 2.51     | 2.53        | 2.55     | 0       | 60.6     |
| Extract nested timestamp field              | jmespath    | 0.54     | 0.54        | 0.54     | 1000    | 57.9     |
| Extract nested timestamp field              | jsonpath-ng | 1.97     | 2.00        | 2.03     | 1000    | 58.1     |
| Extract nested timestamp field              | strata      | 2.37     | 2.43        | 2.50     | 0       | 57.8     |
| Extract order item prices (double wildcard) | strata      | 2.46     | 2.51        | 2.55     | 0       | 58.1     |
| Extract order item prices (double wildcard) | jmespath    | 3.48     | 3.52        | 3.55     | 12046   | 58.4     |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.29    | 17.43       | 17.56    | 12046   | 61.5     |
| Filter users by age (numeric predicate)     | strata      | 0.01     | 0.01        | 0.01     | 0       | 60.6     |
| Filter users by age (numeric predicate)     | jmespath    | 1.04     | 1.04        | 1.05     | 794     | 60.6     |
| Recursively find all prices                 | strata      | 27.69    | 27.73       | 27.77    | 878     | 60.7     |
| Recursively find all prices                 | jsonpath-ng | 68.14    | 68.15       | 68.17    | 12046   | 62.5     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 0       | 61.5     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 10      | 61.5     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.02     | 10      | 60.7     |
