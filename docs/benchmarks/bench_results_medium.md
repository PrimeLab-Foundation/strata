# Strata Benchmark Results

Generated: 2026-02-11 22:50:16

## Environment

- Commit: 9ffdd71efb8fe8bda6bb58fb8f1387ca32d249cf
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
| orjson        | users.json   | 18.874   | 26.466      | 28.375   | 2715.7   |
| msgspec       | users.json   | 19.293   | 28.223      | 30.255   | 2715.6   |
| ujson         | users.json   | 27.228   | 39.606      | 41.573   | 2734.2   |
| json (stdlib) | users.json   | 35.029   | 44.005      | 45.000   | 2715.6   |
| strata        | users.json   | 69.388   | 78.221      | 127.953  | 2694.1   |
| orjson        | users.ndjson | 22.755   | 24.165      | 25.846   | 2696.8   |
| msgspec       | users.ndjson | 24.007   | 24.436      | 25.217   | 2696.6   |
| ujson         | users.ndjson | 32.148   | 34.643      | 35.667   | 2714.4   |
| json (stdlib) | users.ndjson | 38.996   | 40.678      | 42.189   | 2699.1   |
| strata        | users.ndjson | 46.940   | 55.342      | 55.426   | 2729.5   |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.149    | 2000    |
|                                     | jmespath    | 0.424    | 2000    |
|                                     | jsonpath-ng | 6.080    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 4.011    | 100999  |
|                                     | jmespath    | 33.238   | 2000    |
|                                     | jsonpath-ng | 131.004  | 100999  |
| $..price                            | jmespath    | 32.957   | 2000    |
|                                     | query       | 38.229   | 100999  |
|                                     | jsonpath-ng | 496.327  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.128    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 41.190   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 19.29    | 27.09       | 27.69    | 2713.9   |
| msgspec       | 19.85    | 28.56       | 28.92    | 2714.7   |
| ujson         | 27.92    | 39.28       | 39.45    | 2732.4   |
| json (stdlib) | 35.20    | 44.07       | 44.28    | 2714.7   |
| strata        | 69.73    | 80.10       | 119.87   | 2694.3   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 21.74    | 23.76       | 24.10    | 143.7    |
| msgspec       | 22.53    | 24.12       | 24.71    | 143.5    |
| ujson         | 31.12    | 35.30       | 35.73    | 161.4    |
| json (stdlib) | 38.52    | 41.66       | 42.37    | 144.5    |
| strata        | 45.74    | 47.77       | 47.95    | 142.5    |
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
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 1       | 1803.4   |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 1       | 1802.4   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 1       | 1802.4   |
| Extract all user IDs                        | jmespath    | 0.48     | 0.52        | 0.55     | 2000    | 553.1    |
| Extract all user IDs                        | strata      | 2.38     | 2.40        | 2.42     | 2000    | 553.0    |
| Extract all user IDs                        | jsonpath-ng | 6.36     | 6.41        | 6.47     | 2000    | 554.1    |
| Extract all user names                      | jmespath    | 0.43     | 0.46        | 0.49     | 2000    | 1944.2   |
| Extract all user names                      | strata      | 2.40     | 2.72        | 3.05     | 2000    | 1944.2   |
| Extract all user names                      | jsonpath-ng | 5.88     | 6.07        | 6.26     | 2000    | 1944.2   |
| Extract nested timestamp field              | jmespath    | 1.13     | 1.16        | 1.18     | 2000    | 985.3    |
| Extract nested timestamp field              | strata      | 2.29     | 2.31        | 2.34     | 2000    | 985.2    |
| Extract nested timestamp field              | jsonpath-ng | 7.44     | 7.75        | 8.06     | 2000    | 985.9    |
| Extract order item prices (double wildcard) | jmespath    | 33.66    | 33.92       | 34.19    | 2000    | 1407.5   |
| Extract order item prices (double wildcard) | strata      | 102.78   | 103.01      | 103.25   | 100999  | 1405.5   |
| Extract order item prices (double wildcard) | jsonpath-ng | 132.89   | 133.25      | 133.61   | 100999  | 1410.4   |
| Filter users by age (numeric predicate)     | jmespath    | 2.26     | 2.30        | 2.33     | 1606    | 2082.2   |
| Filter users by age (numeric predicate)     | strata      | 2.30     | 2.32        | 2.34     | 1606    | 2082.2   |
| Recursively find all prices                 | strata      | 190.98   | 191.00      | 191.02   | 100999  | 2179.0   |
| Recursively find all prices                 | jsonpath-ng | 497.09   | 498.32      | 499.56   | 100999  | 2183.2   |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 10      | 2267.0   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 10      | 2266.5   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.03     | 10      | 2267.0   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 1       | 211.7    |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 0       | 211.7    |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 1       | 211.7    |
| Extract all user IDs                        | jmespath    | 0.67     | 0.73        | 0.79     | 2000    | 153.4    |
| Extract all user IDs                        | strata      | 5.52     | 5.53        | 5.54     | 0       | 153.3    |
| Extract all user IDs                        | jsonpath-ng | 6.17     | 6.22        | 6.28     | 2000    | 154.4    |
| Extract all user names                      | jmespath    | 0.72     | 0.73        | 0.74     | 2000    | 211.7    |
| Extract all user names                      | strata      | 5.26     | 5.32        | 5.37     | 0       | 211.7    |
| Extract all user names                      | jsonpath-ng | 6.05     | 6.18        | 6.32     | 2000    | 211.7    |
| Extract nested timestamp field              | jmespath    | 1.39     | 1.44        | 1.49     | 2000    | 205.2    |
| Extract nested timestamp field              | strata      | 5.49     | 5.51        | 5.53     | 0       | 205.2    |
| Extract nested timestamp field              | jsonpath-ng | 7.63     | 7.74        | 7.85     | 2000    | 205.6    |
| Extract order item prices (double wildcard) | strata      | 5.57     | 5.57        | 5.57     | 0       | 206.1    |
| Extract order item prices (double wildcard) | jmespath    | 25.41    | 25.52       | 25.63    | 100999  | 209.0    |
| Extract order item prices (double wildcard) | jsonpath-ng | 132.25   | 132.42      | 132.59   | 100999  | 213.5    |
| Filter users by age (numeric predicate)     | strata      | 0.01     | 0.01        | 0.01     | 0       | 212.0    |
| Filter users by age (numeric predicate)     | jmespath    | 2.38     | 2.43        | 2.49     | 1606    | 212.0    |
| Recursively find all prices                 | strata      | 189.17   | 190.05      | 190.94   | 1908    | 218.3    |
| Recursively find all prices                 | jsonpath-ng | 495.79   | 496.22      | 496.65   | 100999  | 222.9    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 0       | 222.9    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 10      | 221.9    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 10      | 219.9    |
