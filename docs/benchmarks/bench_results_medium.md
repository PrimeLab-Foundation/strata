# Strata Benchmark Results

Generated: 2026-02-14 17:08:40

## Environment

- Commit: 8d68dc2d967bbefb0f220f9b3733bf92fb7c966a
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
| orjson        | users.json   | 18.599   | 26.656      | 28.104   | 130.0    |
| msgspec       | users.json   | 20.059   | 28.421      | 28.977   | 129.9    |
| ujson         | users.json   | 28.309   | 39.118      | 39.297   | 148.5    |
| json (stdlib) | users.json   | 35.843   | 43.999      | 44.112   | 129.9    |
| strata        | users.json   | 44.010   | 47.015      | 47.379   | 110.1    |
| orjson        | users.ndjson | 22.400   | 24.074      | 25.367   | 152.2    |
| msgspec       | users.ndjson | 23.099   | 24.838      | 25.831   | 153.1    |
| ujson         | users.ndjson | 32.123   | 35.255      | 35.822   | 170.8    |
| json (stdlib) | users.ndjson | 40.174   | 41.781      | 41.929   | 154.9    |
| strata        | users.ndjson | 50.365   | 59.433      | 59.543   | 147.2    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.151    | 2000    |
|                                     | jmespath    | 0.446    | 2000    |
|                                     | jsonpath-ng | 6.774    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 3.732    | 100999  |
|                                     | jmespath    | 33.705   | 2000    |
|                                     | jsonpath-ng | 141.967  | 100999  |
| $..price                            | jmespath    | 34.572   | 2000    |
|                                     | query       | 38.184   | 100999  |
|                                     | jsonpath-ng | 492.491  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.153    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 40.792   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 19.21    | 26.82       | 28.66    | 244.30 | 129.2    |
| msgspec       | 20.64    | 28.49       | 29.45    | 229.93 | 129.9    |
| ujson         | 28.15    | 39.40       | 40.00    | 166.30 | 147.7    |
| json (stdlib) | 36.05    | 45.43       | 45.51    | 144.22 | 130.0    |
| strata        | 43.97    | 46.47       | 46.77    | 140.97 | 109.2    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 24.15    | 24.37       | 25.48    | 268.78 | 140.5    |
| msgspec       | 23.91    | 25.19       | 25.93    | 260.05 | 140.2    |
| ujson         | 32.82    | 35.94       | 36.28    | 182.25 | 158.1    |
| json (stdlib) | 40.55    | 41.83       | 42.54    | 156.59 | 141.2    |
| strata        | 48.99    | 51.05       | 51.12    | 128.29 | 139.3    |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 554638.78 | 1       | 1307.4   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 430789.69 | 1       | 1307.4   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 289039.82 | 1       | 1307.4   |
| Extract all user IDs                        | jmespath    | 0.59     | 0.62        | 0.65     | 10496.31  | 2000    | 553.5    |
| Extract all user IDs                        | strata      | 2.44     | 2.48        | 2.51     | 2643.60   | 2000    | 553.5    |
| Extract all user IDs                        | jsonpath-ng | 6.65     | 6.74        | 6.82     | 972.53    | 2000    | 554.6    |
| Extract all user names                      | jmespath    | 0.51     | 0.53        | 0.54     | 12447.74  | 2000    | 1387.8   |
| Extract all user names                      | strata      | 2.22     | 2.24        | 2.27     | 2922.10   | 2000    | 1387.8   |
| Extract all user names                      | jsonpath-ng | 7.27     | 7.33        | 7.38     | 894.21    | 2000    | 1387.8   |
| Extract nested timestamp field              | jmespath    | 1.28     | 1.28        | 1.29     | 5108.84   | 2000    | 985.8    |
| Extract nested timestamp field              | strata      | 2.40     | 2.40        | 2.41     | 2724.66   | 2000    | 985.8    |
| Extract nested timestamp field              | jsonpath-ng | 8.36     | 8.41        | 8.45     | 779.17    | 2000    | 986.4    |
| Extract order item prices (double wildcard) | jmespath    | 33.71    | 34.05       | 34.39    | 192.43    | 2000    | 1153.4   |
| Extract order item prices (double wildcard) | strata      | 118.30   | 118.96      | 119.63   | 55.07     | 100999  | 1151.4   |
| Extract order item prices (double wildcard) | jsonpath-ng | 135.30   | 135.41      | 135.53   | 48.38     | 100999  | 1156.0   |
| Filter users by age (numeric predicate)     | strata      | 1.98     | 2.07        | 2.16     | 3167.64   | 1606    | 1519.0   |
| Filter users by age (numeric predicate)     | jmespath    | 2.41     | 2.45        | 2.49     | 2677.52   | 1606    | 1519.1   |
| Recursively find all prices                 | strata      | 127.07   | 127.62      | 128.17   | 51.34     | 100999  | 1566.3   |
| Recursively find all prices                 | jsonpath-ng | 499.64   | 499.68      | 499.71   | 13.11     | 100999  | 1569.2   |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 349031.25 | 10      | 1912.0   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 242654.21 | 10      | 1912.0   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 214660.87 | 10      | 1912.0   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 310.35   | 314.40      | 318.45   | 166.71 | 2308.8   |
| strata_cursor_reparse | 752.51   | 754.36      | 756.21   | 69.48  | 3927.2   |
- Speedup (reuse vs reparse): 2.40x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.01     | 493549.73 | 1       | 218.9    |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 423131.45 | 1       | 218.9    |
| Deep path navigation                        | jmespath            | 0.02     | 0.02        | 0.02     | 345099.99 | 1       | 218.9    |
| Extract all user IDs                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 342089.65 | 0       | 208.0    |
| Extract all user IDs                        | jmespath            | 0.80     | 0.83        | 0.86     | 7927.56   | 2000    | 208.0    |
| Extract all user IDs                        | strata              | 7.05     | 7.10        | 7.14     | 922.99    | 2000    | 207.0    |
| Extract all user IDs                        | strata_ndjson_fused | 28.66    | 28.96       | 29.26    | 226.17    | 2000    | 208.0    |
| Extract all user IDs                        | strata_ndjson_full  | 28.85    | 29.40       | 29.96    | 222.74    | 2000    | 207.8    |
| Extract all user names                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 311265.54 | 0       | 219.0    |
| Extract all user names                      | jmespath            | 0.89     | 0.90        | 0.92     | 7246.19   | 2000    | 219.0    |
| Extract all user names                      | strata              | 7.04     | 7.10        | 7.15     | 922.97    | 2000    | 218.9    |
| Extract all user names                      | strata_ndjson_full  | 31.16    | 31.38       | 31.59    | 208.74    | 2000    | 219.0    |
| Extract all user names                      | strata_ndjson_fused | 31.63    | 31.81       | 32.00    | 205.87    | 2000    | 219.0    |
| Extract nested timestamp field              | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 531951.23 | 0       | 208.5    |
| Extract nested timestamp field              | jmespath            | 1.54     | 1.55        | 1.56     | 4225.07   | 2000    | 208.5    |
| Extract nested timestamp field              | strata              | 7.87     | 7.91        | 7.95     | 827.80    | 2000    | 208.5    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 370738.77 | 0       | 218.9    |
| Extract order item prices (double wildcard) | jmespath            | 26.04    | 26.08       | 26.11    | 251.19    | 100999  | 218.9    |
| Extract order item prices (double wildcard) | strata              | 108.11   | 109.12      | 110.13   | 60.02     | 100999  | 216.0    |
| Filter users by age (numeric predicate)     | jmespath            | 2.37     | 2.54        | 2.71     | 2578.37   | 1606    | 219.0    |
| Filter users by age (numeric predicate)     | strata              | 8.69     | 8.74        | 8.78     | 749.52    | 1606    | 219.0    |
| NDJSON first record id                      | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 447212.70 | 1       | 150.3    |
| NDJSON first record id                      | strata              | 0.01     | 0.02        | 0.02     | 405137.51 | 1       | 150.0    |
| NDJSON first record id                      | jmespath            | 0.02     | 0.03        | 0.03     | 252721.40 | 1       | 150.0    |
| Recursively find all prices                 | strata              | 129.16   | 129.83      | 130.50   | 50.45     | 100999  | 219.1    |
| Recursively find all prices                 | jsonpath-ng         | 501.61   | 502.64      | 503.68   | 13.03     | 100999  | 224.5    |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.02        | 0.02     | 408817.01 | 10      | 223.5    |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.03        | 0.03     | 249510.63 | 10      | 223.5    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.03     | 0.03        | 0.04     | 215038.92 | 10      | 223.3    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 315.16   | 315.22      | 315.28   | 187.00 | 223.4    |
| strata_cursor_reparse | 764.60   | 766.21      | 767.82   | 76.93  | 286.2    |
- Speedup (reuse vs reparse): 2.43x
