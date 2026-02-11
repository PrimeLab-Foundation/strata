# Strata Benchmark Results

Generated: 2026-02-11 21:07:40

## Environment

- Commit: 85c029c4d7f2d9b0ae7d1507059a58bf4cb96a81
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
| orjson        | users.json   | 18.784   | 27.338      | 27.629   | 123.5    |
| msgspec       | users.json   | 19.858   | 29.704      | 30.205   | 123.3    |
| strata        | users.json   | 28.113   | 32.448      | 32.555   | 103.2    |
| ujson         | users.json   | 28.083   | 39.962      | 40.738   | 141.9    |
| json (stdlib) | users.json   | 34.962   | 44.976      | 45.166   | 123.3    |
| orjson        | users.ndjson | 22.024   | 23.785      | 24.391   | 136.0    |
| msgspec       | users.ndjson | 23.419   | 24.340      | 26.556   | 135.9    |
| ujson         | users.ndjson | 31.888   | 35.570      | 36.480   | 153.6    |
| json (stdlib) | users.ndjson | 39.938   | 41.927      | 42.927   | 138.3    |
| strata        | users.ndjson | 34.039   | 42.553      | 42.928   | 129.6    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.112    | 2000    |
|                                     | jmespath    | 0.440    | 2000    |
|                                     | jsonpath-ng | 6.519    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 3.883    | 100999  |
|                                     | jmespath    | 34.059   | 2000    |
|                                     | jsonpath-ng | 134.503  | 100999  |
| $..price                            | jmespath    | 33.785   | 2000    |
|                                     | query       | 38.443   | 100999  |
|                                     | jsonpath-ng | 500.038  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.145    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 40.971   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 18.81    | 27.21       | 27.35    | 122.9    |
| msgspec       | 20.28    | 29.07       | 29.75    | 123.6    |
| strata        | 28.83    | 32.57       | 33.17    | 103.6    |
| ujson         | 28.30    | 40.48       | 41.18    | 141.4    |
| json (stdlib) | 35.41    | 44.39       | 45.16    | 123.7    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 23.03    | 24.71       | 24.89    | 150.1    |
| msgspec       | 23.58    | 25.29       | 25.65    | 149.8    |
| ujson         | 31.52    | 35.71       | 37.39    | 167.7    |
| json (stdlib) | 40.44    | 42.52       | 43.40    | 150.9    |
| strata        | 50.77    | 51.56       | 54.61    | 148.9    |
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
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 1       | 251.3    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 250.3    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1       | 250.3    |
| Extract all user IDs                        | jmespath    | 0.51     | 0.56        | 0.60     | 2000    | 172.1    |
| Extract all user IDs                        | strata      | 1.70     | 1.70        | 1.70     | 2000    | 171.8    |
| Extract all user IDs                        | jsonpath-ng | 6.23     | 6.30        | 6.37     | 2000    | 173.1    |
| Extract all user names                      | jmespath    | 0.45     | 0.49        | 0.54     | 2000    | 250.3    |
| Extract all user names                      | strata      | 1.60     | 1.64        | 1.68     | 2000    | 250.3    |
| Extract all user names                      | jsonpath-ng | 6.25     | 6.30        | 6.34     | 2000    | 250.3    |
| Extract nested timestamp field              | jmespath    | 1.18     | 1.22        | 1.26     | 2000    | 228.2    |
| Extract nested timestamp field              | strata      | 1.72     | 1.74        | 1.77     | 2000    | 228.2    |
| Extract nested timestamp field              | jsonpath-ng | 7.46     | 7.52        | 7.59     | 2000    | 228.7    |
| Extract order item prices (double wildcard) | jmespath    | 34.46    | 34.59       | 34.73    | 2000    | 250.9    |
| Extract order item prices (double wildcard) | strata      | 78.07    | 78.34       | 78.61    | 100999  | 248.9    |
| Extract order item prices (double wildcard) | jsonpath-ng | 136.60   | 137.47      | 138.35   | 100999  | 253.6    |
| Filter users by age (numeric predicate)     | strata      | 1.37     | 1.57        | 1.77     | 1606    | 250.4    |
| Filter users by age (numeric predicate)     | jmespath    | 2.31     | 2.38        | 2.46     | 1606    | 250.4    |
| Recursively find all prices                 | strata      | 150.34   | 152.37      | 154.39   | 100999  | 253.1    |
| Recursively find all prices                 | jsonpath-ng | 499.10   | 499.22      | 499.33   | 100999  | 260.5    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 259.5    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 10      | 259.5    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 10      | 258.9    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 0       | 225.7    |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 1       | 225.7    |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 1       | 225.7    |
| Extract all user IDs                        | jmespath    | 0.74     | 0.79        | 0.83     | 2000    | 159.4    |
| Extract all user IDs                        | strata      | 4.97     | 5.13        | 5.29     | 0       | 159.4    |
| Extract all user IDs                        | jsonpath-ng | 7.08     | 7.08        | 7.08     | 2000    | 160.5    |
| Extract all user names                      | jmespath    | 0.81     | 0.85        | 0.89     | 2000    | 225.7    |
| Extract all user names                      | strata      | 5.13     | 5.14        | 5.16     | 0       | 225.7    |
| Extract all user names                      | jsonpath-ng | 7.61     | 8.02        | 8.44     | 2000    | 225.7    |
| Extract nested timestamp field              | jmespath    | 1.46     | 1.50        | 1.54     | 2000    | 217.3    |
| Extract nested timestamp field              | strata      | 5.29     | 5.31        | 5.33     | 0       | 217.3    |
| Extract nested timestamp field              | jsonpath-ng | 7.99     | 8.05        | 8.11     | 2000    | 217.8    |
| Extract order item prices (double wildcard) | strata      | 5.25     | 5.27        | 5.29     | 0       | 217.9    |
| Extract order item prices (double wildcard) | jmespath    | 25.79    | 25.98       | 26.16    | 100999  | 220.8    |
| Extract order item prices (double wildcard) | jsonpath-ng | 133.95   | 135.13      | 136.31   | 100999  | 227.5    |
| Filter users by age (numeric predicate)     | strata      | 0.01     | 0.01        | 0.01     | 0       | 225.7    |
| Filter users by age (numeric predicate)     | jmespath    | 2.48     | 2.48        | 2.49     | 1606    | 225.7    |
| Recursively find all prices                 | strata      | 153.70   | 154.19      | 154.67   | 1908    | 231.6    |
| Recursively find all prices                 | jsonpath-ng | 522.95   | 524.52      | 526.09   | 100999  | 234.3    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 0       | 234.5    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 233.5    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 10      | 231.5    |
