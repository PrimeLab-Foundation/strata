# Strata Benchmark Results

Generated: 2026-02-11 23:35:06

## Environment

- Commit: ec4905efccfcf6fe97a503fe7701cd9771c3d6f0
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
| orjson        | users.json   | 18.746   | 28.004      | 28.641   | 2714.7   |
| msgspec       | users.json   | 19.805   | 28.215      | 30.079   | 2714.5   |
| ujson         | users.json   | 27.822   | 40.479      | 41.129   | 2733.1   |
| json (stdlib) | users.json   | 34.775   | 44.556      | 45.584   | 2714.5   |
| strata        | users.json   | 74.377   | 78.237      | 123.915  | 2693.1   |
| orjson        | users.ndjson | 22.445   | 23.842      | 24.933   | 2695.8   |
| msgspec       | users.ndjson | 22.959   | 24.899      | 25.589   | 2695.7   |
| ujson         | users.ndjson | 31.439   | 35.324      | 35.541   | 2713.4   |
| json (stdlib) | users.ndjson | 39.571   | 41.261      | 42.746   | 2697.5   |
| strata        | users.ndjson | 47.033   | 56.787      | 57.983   | 2728.5   |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.128    | 2000    |
|                                     | jmespath    | 0.459    | 2000    |
|                                     | jsonpath-ng | 6.367    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 3.893    | 100999  |
|                                     | jmespath    | 34.003   | 2000    |
|                                     | jsonpath-ng | 134.087  | 100999  |
| $..price                            | jmespath    | 35.157   | 2000    |
|                                     | query       | 39.114   | 100999  |
|                                     | jsonpath-ng | 502.961  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.141    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 41.168   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 18.96    | 27.68       | 28.29    | 2714.1   |
| msgspec       | 20.80    | 29.70       | 30.05    | 2714.9   |
| ujson         | 28.08    | 39.81       | 40.34    | 2732.7   |
| json (stdlib) | 36.01    | 45.13       | 45.79    | 2715.0   |
| strata        | 71.42    | 75.95       | 122.75   | 2694.5   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 22.64    | 24.22       | 24.43    | 144.2    |
| msgspec       | 23.43    | 25.30       | 25.67    | 143.9    |
| ujson         | 32.54    | 35.80       | 36.88    | 161.9    |
| json (stdlib) | 39.28    | 41.90       | 42.92    | 145.0    |
| strata        | 45.97    | 48.29       | 48.31    | 142.9    |
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
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 1       | 1300.2   |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 1299.2   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 1       | 1299.2   |
| Extract all user IDs                        | jmespath    | 0.49     | 0.51        | 0.52     | 2000    | 553.5    |
| Extract all user IDs                        | strata      | 2.48     | 2.52        | 2.55     | 2000    | 553.4    |
| Extract all user IDs                        | jsonpath-ng | 7.44     | 7.47        | 7.50     | 2000    | 554.6    |
| Extract all user names                      | jmespath    | 0.46     | 0.46        | 0.47     | 2000    | 1381.0   |
| Extract all user names                      | strata      | 2.12     | 2.14        | 2.17     | 2000    | 1381.0   |
| Extract all user names                      | jsonpath-ng | 6.23     | 6.36        | 6.49     | 2000    | 1381.0   |
| Extract nested timestamp field              | jmespath    | 1.21     | 1.22        | 1.23     | 2000    | 985.7    |
| Extract nested timestamp field              | strata      | 2.55     | 2.56        | 2.57     | 2000    | 985.7    |
| Extract nested timestamp field              | jsonpath-ng | 7.45     | 7.61        | 7.77     | 2000    | 986.3    |
| Extract order item prices (double wildcard) | jmespath    | 33.76    | 33.79       | 33.82    | 2000    | 1152.0   |
| Extract order item prices (double wildcard) | strata      | 120.90   | 121.55      | 122.20   | 100999  | 1149.9   |
| Extract order item prices (double wildcard) | jsonpath-ng | 135.17   | 135.18      | 135.18   | 100999  | 1154.8   |
| Filter users by age (numeric predicate)     | strata      | 2.01     | 2.03        | 2.04     | 1606    | 1464.0   |
| Filter users by age (numeric predicate)     | jmespath    | 2.39     | 2.42        | 2.44     | 1606    | 1464.0   |
| Recursively find all prices                 | strata      | 195.58   | 195.82      | 196.05   | 100999  | 1859.0   |
| Recursively find all prices                 | jsonpath-ng | 504.81   | 504.88      | 504.95   | 100999  | 1863.2   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 2256.8   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 10      | 2256.2   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.03        | 0.03     | 10      | 2256.8   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.02     | 0       | 222.0    |
| Deep path navigation                        | jmespath            | 0.01     | 0.02        | 0.02     | 1       | 222.0    |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 1       | 222.0    |
| Extract all user IDs                        | jmespath            | 0.69     | 0.77        | 0.84     | 2000    | 212.2    |
| Extract all user IDs                        | strata              | 5.45     | 5.54        | 5.63     | 0       | 212.0    |
| Extract all user IDs                        | jsonpath-ng         | 6.58     | 6.79        | 7.01     | 2000    | 212.2    |
| Extract all user IDs                        | strata_ndjson_fused | 28.29    | 28.32       | 28.36    | 0       | 212.2    |
| Extract all user IDs                        | strata_ndjson_full  | 45.79    | 45.94       | 46.09    | 0       | 212.2    |
| Extract all user names                      | jmespath            | 0.73     | 0.75        | 0.77     | 2000    | 222.0    |
| Extract all user names                      | strata              | 5.48     | 5.50        | 5.53     | 0       | 222.0    |
| Extract all user names                      | jsonpath-ng         | 6.43     | 6.49        | 6.55     | 2000    | 222.0    |
| Extract all user names                      | strata_ndjson_fused | 28.24    | 28.35       | 28.45    | 0       | 222.0    |
| Extract all user names                      | strata_ndjson_full  | 45.82    | 46.19       | 46.55    | 0       | 222.0    |
| Extract nested timestamp field              | jmespath            | 1.41     | 1.44        | 1.48     | 2000    | 212.6    |
| Extract nested timestamp field              | strata              | 5.49     | 5.54        | 5.59     | 0       | 212.6    |
| Extract nested timestamp field              | jsonpath-ng         | 7.74     | 7.88        | 8.02     | 2000    | 212.6    |
| Extract order item prices (double wildcard) | strata              | 5.62     | 5.63        | 5.65     | 0       | 212.6    |
| Extract order item prices (double wildcard) | jmespath            | 25.12    | 25.41       | 25.69    | 100999  | 215.5    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 134.25   | 135.49      | 136.73   | 100999  | 223.0    |
| Filter users by age (numeric predicate)     | strata              | 0.01     | 0.02        | 0.02     | 0       | 222.0    |
| Filter users by age (numeric predicate)     | jmespath            | 2.48     | 2.50        | 2.51     | 1606    | 222.1    |
| NDJSON root field (id)                      | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 0       | 155.3    |
| NDJSON root field (id)                      | strata              | 7.12     | 7.14        | 7.16     | 2000    | 154.9    |
| NDJSON root field (id)                      | strata_ndjson_fused | 9.99     | 10.01       | 10.04    | 2000    | 155.3    |
| NDJSON root field (id)                      | strata_ndjson_full  | 48.27    | 48.44       | 48.61    | 2000    | 155.2    |
| Recursively find all prices                 | strata              | 189.85   | 192.71      | 195.57   | 1908    | 225.6    |
| Recursively find all prices                 | jsonpath-ng         | 507.27   | 507.51      | 507.76   | 100999  | 229.8    |
| Slice first 10 users, extract IDs           | strata              | 0.01     | 0.01        | 0.01     | 0       | 230.0    |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.02        | 0.02     | 10      | 229.0    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 10      | 228.1    |
