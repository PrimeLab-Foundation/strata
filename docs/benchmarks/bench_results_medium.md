# Strata Benchmark Results

Generated: 2026-02-09 23:59:26

## Environment

- Commit: becfa02e50f22f82704927d5188cb058570b2617
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
| orjson        | users.json   | 17.998   | 26.415      | 26.962   | 123.6    |
| msgspec       | users.json   | 19.267   | 27.963      | 28.850   | 123.4    |
| strata        | users.json   | 28.615   | 31.747      | 32.676   | 103.3    |
| ujson         | users.json   | 27.578   | 39.357      | 39.748   | 142.0    |
| json (stdlib) | users.json   | 34.717   | 43.383      | 44.692   | 123.4    |
| orjson        | users.ndjson | 25.522   | 26.542      | 27.923   | 136.5    |
| msgspec       | users.ndjson | 26.177   | 26.889      | 28.604   | 136.3    |
| ujson         | users.ndjson | 35.431   | 38.269      | 38.890   | 155.0    |
| json (stdlib) | users.ndjson | 39.898   | 41.875      | 41.923   | 138.1    |
| strata        | users.ndjson | 32.174   | 42.507      | 44.080   | 131.1    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.125    | 2000    |
|                                     | jmespath    | 0.504    | 2000    |
|                                     | jsonpath-ng | 6.042    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 3.668    | 100999  |
|                                     | jmespath    | 32.400   | 2000    |
|                                     | jsonpath-ng | 131.090  | 100999  |
| $..price                            | jmespath    | 33.665   | 2000    |
|                                     | query       | 37.370   | 100999  |
|                                     | jsonpath-ng | 497.396  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.115    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 40.471   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 18.91    | 26.40       | 27.57    | 125.9    |
| msgspec       | 20.76    | 28.95       | 29.33    | 123.6    |
| strata        | 29.38    | 33.19       | 34.35    | 103.6    |
| ujson         | 26.81    | 39.19       | 39.53    | 141.4    |
| json (stdlib) | 36.51    | 47.11       | 52.00    | 123.7    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 23.66    | 25.60       | 26.44    | 152.3    |
| msgspec       | 24.43    | 26.20       | 26.53    | 150.0    |
| ujson         | 32.93    | 36.66       | 37.97    | 167.8    |
| json (stdlib) | 38.96    | 42.08       | 42.61    | 150.9    |
| strata        | 54.59    | 55.01       | 55.24    | 152.0    |
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
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 1       | 252.2    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 1       | 251.2    |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 1       | 251.2    |
| Extract all user IDs                        | jmespath    | 0.45     | 0.48        | 0.50     | 2000    | 172.0    |
| Extract all user IDs                        | strata      | 1.58     | 1.65        | 1.72     | 2000    | 171.8    |
| Extract all user IDs                        | jsonpath-ng | 5.98     | 6.09        | 6.19     | 2000    | 173.1    |
| Extract all user names                      | jmespath    | 0.45     | 0.48        | 0.51     | 2000    | 251.2    |
| Extract all user names                      | strata      | 1.60     | 1.72        | 1.84     | 2000    | 251.2    |
| Extract all user names                      | jsonpath-ng | 6.06     | 6.62        | 7.17     | 2000    | 251.2    |
| Extract nested timestamp field              | jmespath    | 1.10     | 1.11        | 1.12     | 2000    | 228.2    |
| Extract nested timestamp field              | strata      | 1.70     | 1.70        | 1.70     | 2000    | 228.2    |
| Extract nested timestamp field              | jsonpath-ng | 7.32     | 7.39        | 7.46     | 2000    | 228.8    |
| Extract order item prices (double wildcard) | jmespath    | 37.73    | 37.93       | 38.14    | 2000    | 250.9    |
| Extract order item prices (double wildcard) | strata      | 78.33    | 80.20       | 82.07    | 100999  | 248.9    |
| Extract order item prices (double wildcard) | jsonpath-ng | 137.13   | 140.55      | 143.97   | 100999  | 254.5    |
| Filter users by age (numeric predicate)     | strata      | 1.33     | 1.36        | 1.39     | 1606    | 251.3    |
| Filter users by age (numeric predicate)     | jmespath    | 2.32     | 2.34        | 2.36     | 1606    | 251.3    |
| Recursively find all prices                 | strata      | 150.68   | 152.57      | 154.47   | 100999  | 254.0    |
| Recursively find all prices                 | jsonpath-ng | 507.48   | 510.01      | 512.54   | 100999  | 261.7    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 10      | 259.7    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 10      | 259.7    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 10      | 259.7    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 0       | 224.0    |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 1       | 224.0    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1       | 224.0    |
| Extract all user IDs                        | jmespath    | 0.67     | 0.74        | 0.81     | 2000    | 159.6    |
| Extract all user IDs                        | strata      | 5.32     | 5.34        | 5.36     | 0       | 159.5    |
| Extract all user IDs                        | jsonpath-ng | 6.59     | 6.79        | 6.99     | 2000    | 160.6    |
| Extract all user names                      | jmespath    | 0.73     | 0.73        | 0.74     | 2000    | 224.0    |
| Extract all user names                      | strata      | 5.03     | 5.13        | 5.23     | 0       | 224.0    |
| Extract all user names                      | jsonpath-ng | 6.31     | 6.40        | 6.49     | 2000    | 224.0    |
| Extract nested timestamp field              | jmespath    | 1.59     | 1.72        | 1.85     | 2000    | 217.5    |
| Extract nested timestamp field              | strata      | 5.19     | 5.29        | 5.39     | 0       | 217.5    |
| Extract nested timestamp field              | jsonpath-ng | 9.35     | 9.54        | 9.73     | 2000    | 217.9    |
| Extract order item prices (double wildcard) | strata      | 5.34     | 5.38        | 5.42     | 0       | 218.1    |
| Extract order item prices (double wildcard) | jmespath    | 28.85    | 29.23       | 29.62    | 100999  | 221.3    |
| Extract order item prices (double wildcard) | jsonpath-ng | 134.59   | 134.71      | 134.83   | 100999  | 225.9    |
| Filter users by age (numeric predicate)     | strata      | 0.01     | 0.02        | 0.02     | 0       | 224.0    |
| Filter users by age (numeric predicate)     | jmespath    | 2.49     | 2.55        | 2.61     | 1606    | 224.0    |
| Recursively find all prices                 | strata      | 153.43   | 154.14      | 154.84   | 1908    | 229.8    |
| Recursively find all prices                 | jsonpath-ng | 513.34   | 528.98      | 544.63   | 100999  | 232.9    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 0       | 233.2    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 232.2    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 10      | 231.2    |
