# Strata Benchmark Results

Generated: 2026-02-11 11:12:20

## Environment

- Commit: af3826e258bad9e6738f3a70f573dcc93723b4e5
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/medium/users.json, benchmarks/data/generated/medium/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users.json   | 17.836   | 26.265      | 26.947   | 123.6    |
| msgspec       | users.json   | 18.918   | 28.515      | 28.682   | 123.4    |
| strata        | users.json   | 26.746   | 29.770      | 30.127   | 103.3    |
| ujson         | users.json   | 26.629   | 38.314      | 39.476   | 142.0    |
| json (stdlib) | users.json   | 34.165   | 43.129      | 43.955   | 123.4    |
| orjson        | users.ndjson | 21.575   | 23.750      | 24.263   | 132.1    |
| msgspec       | users.ndjson | 21.442   | 24.372      | 25.148   | 132.0    |
| ujson         | users.ndjson | 30.303   | 34.204      | 34.639   | 150.7    |
| strata        | users.ndjson | 27.975   | 37.939      | 38.612   | 126.7    |
| json (stdlib) | users.ndjson | 38.194   | 39.495      | 40.948   | 134.5    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.160    | 2000    |
|                                     | jmespath    | 0.431    | 2000    |
|                                     | jsonpath-ng | 6.621    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 3.787    | 100999  |
|                                     | jmespath    | 32.723   | 2000    |
|                                     | jsonpath-ng | 131.420  | 100999  |
| $..price                            | jmespath    | 32.385   | 2000    |
|                                     | query       | 36.514   | 100999  |
|                                     | jsonpath-ng | 480.512  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.178    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 38.127   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 18.86    | 26.24       | 26.65    | 123.1    |
| msgspec       | 19.58    | 28.65       | 28.79    | 123.8    |
| strata        | 26.40    | 30.30       | 30.34    | 103.8    |
| ujson         | 26.46    | 38.35       | 38.76    | 141.6    |
| json (stdlib) | 34.03    | 43.04       | 43.72    | 123.9    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 21.71    | 23.04       | 23.44    | 150.0    |
| msgspec       | 22.45    | 24.40       | 25.06    | 149.8    |
| ujson         | 31.09    | 34.96       | 35.03    | 167.7    |
| json (stdlib) | 38.53    | 40.43       | 41.05    | 150.8    |
| strata        | 50.82    | 51.35       | 53.54    | 148.8    |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 1       | 254.5    |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 1       | 253.5    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 1       | 253.5    |
| Extract all user IDs                        | jmespath    | 0.74     | 0.83        | 0.91     | 2000    | 172.4    |
| Extract all user IDs                        | strata      | 1.70     | 1.90        | 2.11     | 2000    | 172.2    |
| Extract all user IDs                        | jsonpath-ng | 7.84     | 8.17        | 8.50     | 2000    | 173.5    |
| Extract all user names                      | jmespath    | 0.48     | 0.51        | 0.55     | 2000    | 253.6    |
| Extract all user names                      | strata      | 1.82     | 2.35        | 2.88     | 2000    | 253.6    |
| Extract all user names                      | jsonpath-ng | 6.57     | 6.65        | 6.74     | 2000    | 253.6    |
| Extract nested timestamp field              | jmespath    | 1.13     | 1.21        | 1.29     | 2000    | 228.5    |
| Extract nested timestamp field              | strata      | 1.73     | 1.75        | 1.77     | 2000    | 228.5    |
| Extract nested timestamp field              | jsonpath-ng | 7.64     | 7.72        | 7.80     | 2000    | 229.0    |
| Extract order item prices (double wildcard) | jmespath    | 33.10    | 33.35       | 33.59    | 2000    | 254.3    |
| Extract order item prices (double wildcard) | strata      | 92.67    | 93.54       | 94.42    | 100999  | 252.3    |
| Extract order item prices (double wildcard) | jsonpath-ng | 132.60   | 135.80      | 138.99   | 100999  | 256.8    |
| Filter users by age (numeric predicate)     | strata      | 1.55     | 1.59        | 1.64     | 1606    | 253.6    |
| Filter users by age (numeric predicate)     | jmespath    | 2.32     | 2.36        | 2.41     | 1606    | 253.6    |
| Recursively find all prices                 | strata      | 161.82   | 164.68      | 167.54   | 100999  | 257.4    |
| Recursively find all prices                 | jsonpath-ng | 503.53   | 510.98      | 518.44   | 100999  | 261.0    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 10      | 260.0    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 10      | 260.0    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 10      | 259.4    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 0       | 226.0    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 1       | 226.0    |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 1       | 226.0    |
| Extract all user IDs                        | jmespath    | 0.78     | 0.79        | 0.81     | 2000    | 159.6    |
| Extract all user IDs                        | strata      | 5.04     | 5.12        | 5.20     | 0       | 159.3    |
| Extract all user IDs                        | jsonpath-ng | 6.84     | 6.89        | 6.94     | 2000    | 160.7    |
| Extract all user names                      | jmespath    | 0.81     | 0.89        | 0.98     | 2000    | 226.1    |
| Extract all user names                      | strata      | 5.35     | 5.55        | 5.75     | 0       | 226.1    |
| Extract all user names                      | jsonpath-ng | 6.67     | 6.98        | 7.30     | 2000    | 226.1    |
| Extract nested timestamp field              | jmespath    | 1.40     | 1.44        | 1.48     | 2000    | 217.5    |
| Extract nested timestamp field              | strata      | 5.04     | 5.11        | 5.19     | 0       | 217.5    |
| Extract nested timestamp field              | jsonpath-ng | 8.38     | 8.50        | 8.61     | 2000    | 218.0    |
| Extract order item prices (double wildcard) | strata      | 5.04     | 5.05        | 5.05     | 0       | 218.4    |
| Extract order item prices (double wildcard) | jmespath    | 25.83    | 25.95       | 26.06    | 100999  | 222.2    |
| Extract order item prices (double wildcard) | jsonpath-ng | 138.21   | 138.22      | 138.24   | 100999  | 226.9    |
| Filter users by age (numeric predicate)     | strata      | 0.01     | 0.01        | 0.01     | 0       | 226.1    |
| Filter users by age (numeric predicate)     | jmespath    | 2.45     | 2.51        | 2.57     | 1606    | 226.1    |
| Recursively find all prices                 | strata      | 169.31   | 171.31      | 173.31   | 1908    | 231.0    |
| Recursively find all prices                 | jsonpath-ng | 493.52   | 496.89      | 500.26   | 100999  | 232.0    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 0       | 232.2    |
| Slice first 10 users, extract IDs           | jmespath    | 0.03     | 0.03        | 0.03     | 10      | 231.2    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.05     | 10      | 230.2    |
