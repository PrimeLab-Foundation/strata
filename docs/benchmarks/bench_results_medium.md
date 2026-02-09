# Strata Benchmark Results

Generated: 2026-02-09 19:07:46

## Environment

- Commit: d18dd88f6a1f4b1ea0013c01ecb1e2d84d0466ca
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
| orjson        | users.json   | 18.972   | 27.440      | 27.711   | 123.7    |
| msgspec       | users.json   | 19.918   | 28.840      | 29.939   | 123.5    |
| strata        | users.json   | 28.373   | 33.602      | 34.799   | 103.4    |
| ujson         | users.json   | 28.496   | 41.054      | 41.181   | 142.2    |
| json (stdlib) | users.json   | 35.325   | 45.584      | 46.042   | 123.5    |
| orjson        | users.ndjson | 22.582   | 24.203      | 25.509   | 136.5    |
| msgspec       | users.ndjson | 22.454   | 24.574      | 25.433   | 136.2    |
| ujson         | users.ndjson | 32.124   | 35.375      | 36.413   | 155.0    |
| strata        | users.ndjson | 31.312   | 40.604      | 40.879   | 131.1    |
| json (stdlib) | users.ndjson | 39.651   | 41.899      | 42.989   | 138.7    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.119    | 2000    |
|                                     | jmespath    | 0.437    | 2000    |
|                                     | jsonpath-ng | 6.616    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 4.095    | 100999  |
|                                     | jmespath    | 34.540   | 2000    |
|                                     | jsonpath-ng | 135.238  | 100999  |
| $..price                            | jmespath    | 35.138   | 2000    |
|                                     | query       | 38.475   | 100999  |
|                                     | jsonpath-ng | 514.988  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.133    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 41.645   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 18.53    | 27.03       | 27.39    | 125.9    |
| msgspec       | 19.99    | 29.30       | 29.49    | 123.6    |
| strata        | 29.12    | 32.82       | 33.42    | 103.5    |
| ujson         | 27.66    | 40.08       | 41.15    | 141.4    |
| json (stdlib) | 36.04    | 44.32       | 45.20    | 123.6    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 23.06    | 24.92       | 25.53    | 152.3    |
| msgspec       | 23.88    | 26.24       | 26.28    | 150.0    |
| ujson         | 33.49    | 37.12       | 37.92    | 167.8    |
| json (stdlib) | 40.79    | 42.89       | 43.50    | 150.9    |
| strata        | 51.37    | 51.57       | 53.33    | 151.9    |
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
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 1       | 251.2    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 249.2    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 1       | 249.2    |
| Extract all user IDs                        | jmespath    | 0.65     | 0.76        | 0.86     | 2000    | 172.1    |
| Extract all user IDs                        | strata      | 1.69     | 1.70        | 1.72     | 2000    | 171.8    |
| Extract all user IDs                        | jsonpath-ng | 6.57     | 6.83        | 7.09     | 2000    | 173.2    |
| Extract all user names                      | jmespath    | 0.48     | 0.50        | 0.52     | 2000    | 249.3    |
| Extract all user names                      | strata      | 1.62     | 1.63        | 1.64     | 2000    | 249.3    |
| Extract all user names                      | jsonpath-ng | 6.26     | 6.36        | 6.47     | 2000    | 249.3    |
| Extract nested timestamp field              | jmespath    | 1.19     | 1.22        | 1.25     | 2000    | 228.2    |
| Extract nested timestamp field              | strata      | 1.70     | 1.73        | 1.76     | 2000    | 228.2    |
| Extract nested timestamp field              | jsonpath-ng | 7.52     | 7.55        | 7.58     | 2000    | 228.8    |
| Extract order item prices (double wildcard) | jmespath    | 34.17    | 34.60       | 35.03    | 2000    | 250.8    |
| Extract order item prices (double wildcard) | strata      | 81.00    | 81.07       | 81.14    | 100999  | 248.8    |
| Extract order item prices (double wildcard) | jsonpath-ng | 138.66   | 138.82      | 138.97   | 100999  | 253.5    |
| Filter users by age (numeric predicate)     | strata      | 1.37     | 1.39        | 1.40     | 1606    | 249.3    |
| Filter users by age (numeric predicate)     | jmespath    | 2.41     | 2.45        | 2.48     | 1606    | 249.3    |
| Recursively find all prices                 | strata      | 169.08   | 170.79      | 172.50   | 100999  | 257.6    |
| Recursively find all prices                 | jsonpath-ng | 507.47   | 511.90      | 516.34   | 100999  | 260.7    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 10      | 259.7    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 10      | 259.7    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 10      | 259.1    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 295.1    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 1       | 295.1    |
| Deep path navigation                        | strata      | 65.04    | 65.60       | 66.16    | 1       | 295.1    |
| Extract all user IDs                        | jmespath    | 0.82     | 0.83        | 0.85     | 2000    | 223.5    |
| Extract all user IDs                        | jsonpath-ng | 12.66    | 12.74       | 12.83    | 2000    | 224.5    |
| Extract all user IDs                        | strata      | 69.74    | 70.32       | 70.90    | 2000    | 223.2    |
| Extract all user names                      | jmespath    | 0.83     | 0.84        | 0.85     | 2000    | 312.6    |
| Extract all user names                      | jsonpath-ng | 12.49    | 12.94       | 13.38    | 2000    | 312.7    |
| Extract all user names                      | strata      | 66.96    | 66.99       | 67.01    | 2000    | 312.6    |
| Extract nested timestamp field              | jmespath    | 1.48     | 1.49        | 1.51     | 2000    | 246.8    |
| Extract nested timestamp field              | jsonpath-ng | 13.87    | 13.89       | 13.92    | 2000    | 247.4    |
| Extract nested timestamp field              | strata      | 66.77    | 67.32       | 67.87    | 2000    | 246.8    |
| Extract order item prices (double wildcard) | jmespath    | 27.53    | 27.77       | 28.01    | 100999  | 279.6    |
| Extract order item prices (double wildcard) | jsonpath-ng | 152.57   | 152.78      | 152.98   | 100999  | 282.2    |
| Extract order item prices (double wildcard) | strata      | 162.34   | 163.18      | 164.02   | 100999  | 279.5    |
| Filter users by age (numeric predicate)     | jmespath    | 2.55     | 2.55        | 2.55     | 1606    | 337.1    |
| Filter users by age (numeric predicate)     | strata      | 66.31    | 68.06       | 69.82    | 1606    | 337.1    |
| Recursively find all prices                 | strata      | 234.49   | 234.95      | 235.42   | 100999  | 368.5    |
| Recursively find all prices                 | jsonpath-ng | 534.54   | 535.26      | 535.98   | 100999  | 371.3    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 10      | 388.1    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 10      | 388.1    |
| Slice first 10 users, extract IDs           | strata      | 64.37    | 64.81       | 65.25    | 10      | 388.1    |
