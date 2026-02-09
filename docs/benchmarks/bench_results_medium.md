# Strata Benchmark Results

Generated: 2026-02-10 01:08:10

## Environment

- Commit: 1f670ba759142c9cd50c0f98cc520859e2b32391
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
| orjson        | users.json   | 18.326   | 26.280      | 26.816   | 123.5    |
| msgspec       | users.json   | 19.394   | 28.847      | 29.172   | 123.3    |
| strata        | users.json   | 27.884   | 31.908      | 32.159   | 103.2    |
| ujson         | users.json   | 27.582   | 39.593      | 40.163   | 142.0    |
| json (stdlib) | users.json   | 34.332   | 44.017      | 44.242   | 123.3    |
| orjson        | users.ndjson | 22.545   | 23.196      | 24.353   | 136.7    |
| msgspec       | users.ndjson | 21.980   | 23.969      | 24.915   | 136.4    |
| ujson         | users.ndjson | 30.814   | 34.721      | 35.168   | 155.2    |
| strata        | users.ndjson | 29.469   | 38.570      | 39.638   | 131.4    |
| json (stdlib) | users.ndjson | 38.022   | 42.109      | 43.646   | 138.9    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.167    | 2000    |
|                                     | jmespath    | 0.426    | 2000    |
|                                     | jsonpath-ng | 6.121    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 3.801    | 100999  |
|                                     | jmespath    | 33.842   | 2000    |
|                                     | jsonpath-ng | 134.004  | 100999  |
| $..price                            | jmespath    | 34.622   | 2000    |
|                                     | query       | 37.611   | 100999  |
|                                     | jsonpath-ng | 503.011  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.131    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 40.742   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 18.90    | 26.25       | 27.26    | 125.9    |
| msgspec       | 19.38    | 28.73       | 31.56    | 123.6    |
| strata        | 28.86    | 31.95       | 32.75    | 103.6    |
| ujson         | 28.30    | 39.34       | 40.44    | 141.4    |
| json (stdlib) | 34.48    | 44.17       | 44.26    | 123.7    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 22.19    | 24.03       | 24.34    | 152.3    |
| msgspec       | 23.17    | 25.04       | 25.13    | 150.0    |
| ujson         | 32.11    | 36.49       | 37.15    | 167.8    |
| json (stdlib) | 42.01    | 43.70       | 44.09    | 150.9    |
| strata        | 51.15    | 51.80       | 54.16    | 152.0    |
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
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 1       | 254.5    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 253.5    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 1       | 253.5    |
| Extract all user IDs                        | jmespath    | 0.60     | 0.66        | 0.71     | 2000    | 171.9    |
| Extract all user IDs                        | strata      | 1.58     | 1.62        | 1.66     | 2000    | 171.7    |
| Extract all user IDs                        | jsonpath-ng | 6.53     | 6.58        | 6.63     | 2000    | 173.2    |
| Extract all user names                      | jmespath    | 0.42     | 0.46        | 0.50     | 2000    | 253.5    |
| Extract all user names                      | strata      | 1.88     | 1.90        | 1.92     | 2000    | 253.5    |
| Extract all user names                      | jsonpath-ng | 6.18     | 6.24        | 6.30     | 2000    | 253.5    |
| Extract nested timestamp field              | jmespath    | 1.18     | 1.24        | 1.29     | 2000    | 228.2    |
| Extract nested timestamp field              | strata      | 1.68     | 1.70        | 1.71     | 2000    | 228.2    |
| Extract nested timestamp field              | jsonpath-ng | 7.35     | 7.37        | 7.39     | 2000    | 228.8    |
| Extract order item prices (double wildcard) | jmespath    | 34.40    | 34.71       | 35.02    | 2000    | 254.2    |
| Extract order item prices (double wildcard) | strata      | 95.05    | 95.07       | 95.09    | 100999  | 252.2    |
| Extract order item prices (double wildcard) | jsonpath-ng | 135.17   | 139.41      | 143.66   | 100999  | 256.8    |
| Filter users by age (numeric predicate)     | strata      | 1.56     | 1.63        | 1.70     | 1606    | 253.6    |
| Filter users by age (numeric predicate)     | jmespath    | 2.26     | 2.29        | 2.31     | 1606    | 253.6    |
| Recursively find all prices                 | strata      | 164.39   | 164.69      | 165.00   | 100999  | 257.4    |
| Recursively find all prices                 | jsonpath-ng | 497.15   | 503.18      | 509.21   | 100999  | 262.0    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 260.0    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.03     | 10      | 260.0    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 10      | 260.0    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 0       | 224.2    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 1       | 224.2    |
| Deep path navigation                        | jmespath    | 0.02     | 0.03        | 0.04     | 1       | 224.2    |
| Extract all user IDs                        | jmespath    | 0.67     | 0.73        | 0.80     | 2000    | 159.5    |
| Extract all user IDs                        | strata      | 5.27     | 5.28        | 5.29     | 0       | 159.2    |
| Extract all user IDs                        | jsonpath-ng | 6.17     | 6.38        | 6.60     | 2000    | 160.6    |
| Extract all user names                      | jmespath    | 0.67     | 0.69        | 0.71     | 2000    | 224.2    |
| Extract all user names                      | strata      | 5.32     | 5.34        | 5.37     | 0       | 224.2    |
| Extract all user names                      | jsonpath-ng | 6.56     | 6.75        | 6.95     | 2000    | 224.2    |
| Extract nested timestamp field              | jmespath    | 1.39     | 1.42        | 1.45     | 2000    | 217.5    |
| Extract nested timestamp field              | strata      | 5.37     | 5.39        | 5.40     | 0       | 217.4    |
| Extract nested timestamp field              | jsonpath-ng | 7.43     | 7.57        | 7.71     | 2000    | 218.0    |
| Extract order item prices (double wildcard) | strata      | 5.39     | 5.43        | 5.47     | 0       | 218.1    |
| Extract order item prices (double wildcard) | jmespath    | 24.56    | 24.78       | 25.00    | 100999  | 222.8    |
| Extract order item prices (double wildcard) | jsonpath-ng | 132.88   | 133.22      | 133.57   | 100999  | 226.1    |
| Filter users by age (numeric predicate)     | strata      | 0.01     | 0.01        | 0.02     | 0       | 224.2    |
| Filter users by age (numeric predicate)     | jmespath    | 2.48     | 2.52        | 2.55     | 1606    | 224.3    |
| Recursively find all prices                 | strata      | 167.87   | 170.77      | 173.68   | 1908    | 229.2    |
| Recursively find all prices                 | jsonpath-ng | 505.02   | 506.94      | 508.86   | 100999  | 232.5    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 0       | 232.7    |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 231.7    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 10      | 230.7    |
