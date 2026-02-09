# Strata Benchmark Results

Generated: 2026-02-09 17:42:06

## Environment

- Commit: b1c61c3a33340b140cf44500ca98dc46a704f09c
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
| orjson        | users.json   | 24.222   | 35.475      | 37.216   | 123.7    |
| msgspec       | users.json   | 25.023   | 36.815      | 38.130   | 123.6    |
| strata        | users.json   | 33.064   | 37.707      | 38.192   | 103.5    |
| ujson         | users.json   | 36.031   | 50.612      | 51.043   | 142.2    |
| json (stdlib) | users.json   | 42.819   | 56.838      | 58.558   | 123.6    |
| orjson        | users.ndjson | 28.225   | 28.288      | 31.483   | 131.9    |
| msgspec       | users.ndjson | 27.548   | 30.129      | 32.968   | 131.7    |
| ujson         | users.ndjson | 39.371   | 42.622      | 43.030   | 150.4    |
| json (stdlib) | users.ndjson | 46.542   | 47.055      | 50.363   | 134.1    |
| strata        | users.ndjson | 35.497   | 48.938      | 52.063   | 126.5    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.243    | 2000    |
|                                     | jmespath    | 0.727    | 2000    |
|                                     | jsonpath-ng | 9.488    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 4.736    | 100999  |
|                                     | jmespath    | 39.588   | 2000    |
|                                     | jsonpath-ng | 177.483  | 100999  |
| $..price                            | jmespath    | 40.118   | 2000    |
|                                     | query       | 42.006   | 100999  |
|                                     | jsonpath-ng | 574.630  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.288    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 44.854   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 27.99    | 35.75       | 39.25    | 126.3    |
| strata        | 36.13    | 37.80       | 39.86    | 104.0    |
| msgspec       | 27.33    | 38.68       | 39.02    | 124.1    |
| ujson         | 35.77    | 50.19       | 60.32    | 141.8    |
| json (stdlib) | 44.07    | 55.03       | 56.81    | 124.1    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 27.86    | 32.50       | 35.00    | 170.5    |
| msgspec       | 38.57    | 38.96       | 40.18    | 168.2    |
| ujson         | 44.79    | 47.00       | 56.61    | 186.0    |
| json (stdlib) | 48.08    | 48.74       | 49.49    | 169.1    |
| strata        | 61.06    | 62.50       | 62.62    | 170.2    |
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
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 1       | 254.3    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1       | 253.3    |
| Deep path navigation                        | jmespath    | 0.03     | 0.03        | 0.03     | 1       | 253.3    |
| Extract all user IDs                        | jmespath    | 0.83     | 0.87        | 0.92     | 2000    | 172.3    |
| Extract all user IDs                        | strata      | 2.12     | 2.21        | 2.29     | 2000    | 172.1    |
| Extract all user IDs                        | jsonpath-ng | 9.19     | 9.52        | 9.85     | 2000    | 173.4    |
| Extract all user names                      | jmespath    | 0.71     | 0.82        | 0.93     | 2000    | 253.3    |
| Extract all user names                      | strata      | 2.42     | 2.52        | 2.63     | 2000    | 253.3    |
| Extract all user names                      | jsonpath-ng | 10.25    | 10.33       | 10.41    | 2000    | 253.3    |
| Extract nested timestamp field              | jmespath    | 1.45     | 1.50        | 1.55     | 2000    | 228.5    |
| Extract nested timestamp field              | strata      | 2.39     | 2.45        | 2.50     | 2000    | 228.5    |
| Extract nested timestamp field              | jsonpath-ng | 10.42    | 10.49       | 10.57    | 2000    | 229.1    |
| Extract order item prices (double wildcard) | jmespath    | 40.65    | 45.65       | 50.65    | 2000    | 253.7    |
| Extract order item prices (double wildcard) | strata      | 111.70   | 112.43      | 113.16   | 100999  | 251.7    |
| Extract order item prices (double wildcard) | jsonpath-ng | 159.94   | 164.52      | 169.10   | 100999  | 256.6    |
| Filter users by age (numeric predicate)     | strata      | 1.88     | 1.91        | 1.93     | 1606    | 253.4    |
| Filter users by age (numeric predicate)     | jmespath    | 2.95     | 2.96        | 2.98     | 1606    | 253.4    |
| Recursively find all prices                 | strata      | 193.83   | 194.62      | 195.41   | 100999  | 256.2    |
| Recursively find all prices                 | jsonpath-ng | 575.75   | 590.34      | 604.94   | 100999  | 255.3    |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 10      | 253.7    |
| Slice first 10 users, extract IDs           | jmespath    | 0.03     | 0.03        | 0.03     | 10      | 253.7    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.04     | 0.04        | 0.05     | 10      | 253.7    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.03        | 0.03     | 1       | 298.9    |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1       | 298.9    |
| Deep path navigation                        | strata      | 71.54    | 72.15       | 72.76    | 1       | 298.9    |
| Extract all user IDs                        | jmespath    | 1.07     | 1.07        | 1.07     | 2000    | 224.5    |
| Extract all user IDs                        | jsonpath-ng | 16.34    | 16.42       | 16.51    | 2000    | 225.5    |
| Extract all user IDs                        | strata      | 78.55    | 80.32       | 82.08    | 2000    | 224.4    |
| Extract all user names                      | jmespath    | 1.09     | 1.10        | 1.11     | 2000    | 316.0    |
| Extract all user names                      | jsonpath-ng | 16.11    | 16.32       | 16.52    | 2000    | 316.0    |
| Extract all user names                      | strata      | 75.26    | 75.80       | 76.34    | 2000    | 316.0    |
| Extract nested timestamp field              | jmespath    | 1.80     | 1.83        | 1.86     | 2000    | 247.7    |
| Extract nested timestamp field              | jsonpath-ng | 18.22    | 18.58       | 18.93    | 2000    | 248.3    |
| Extract nested timestamp field              | strata      | 74.74    | 74.87       | 75.00    | 2000    | 247.7    |
| Extract order item prices (double wildcard) | jmespath    | 30.25    | 30.46       | 30.68    | 100999  | 281.4    |
| Extract order item prices (double wildcard) | jsonpath-ng | 175.99   | 178.73      | 181.47   | 100999  | 286.0    |
| Extract order item prices (double wildcard) | strata      | 182.08   | 182.24      | 182.40   | 100999  | 280.8    |
| Filter users by age (numeric predicate)     | jmespath    | 2.93     | 2.95        | 2.98     | 1606    | 340.5    |
| Filter users by age (numeric predicate)     | strata      | 74.73    | 76.28       | 77.83    | 1606    | 340.5    |
| Recursively find all prices                 | strata      | 250.58   | 250.89      | 251.20   | 100999  | 368.2    |
| Recursively find all prices                 | jsonpath-ng | 594.65   | 639.21      | 683.78   | 100999  | 268.6    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 10      | 286.8    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.04     | 0.04        | 0.05     | 10      | 286.8    |
| Slice first 10 users, extract IDs           | strata      | 72.25    | 78.81       | 85.38    | 10      | 286.8    |
