# Strata Benchmark Results

Generated: 2026-02-12 00:35:41

## Environment

- Commit: 271ffab0b72d6e1fdcb5aeb8844621760c1bbb21
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
| orjson        | users.json   | 18.359   | 27.097      | 27.993   | 2833.7   |
| msgspec       | users.json   | 19.391   | 28.562      | 28.569   | 2833.5   |
| ujson         | users.json   | 27.849   | 39.566      | 41.057   | 2852.2   |
| json (stdlib) | users.json   | 35.842   | 44.607      | 46.764   | 2833.5   |
| strata        | users.json   | 66.357   | 78.739      | 122.210  | 2812.1   |
| orjson        | users.ndjson | 23.319   | 23.904      | 25.605   | 2811.1   |
| msgspec       | users.ndjson | 24.332   | 24.772      | 25.565   | 2810.8   |
| ujson         | users.ndjson | 32.445   | 38.582      | 38.669   | 2828.5   |
| json (stdlib) | users.ndjson | 42.359   | 44.658      | 45.724   | 2813.3   |
| strata        | users.ndjson | 46.934   | 55.874      | 56.908   | 2843.9   |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.126    | 2000    |
|                                     | jmespath    | 0.455    | 2000    |
|                                     | jsonpath-ng | 6.195    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 3.840    | 100999  |
|                                     | jmespath    | 33.539   | 2000    |
|                                     | jsonpath-ng | 132.942  | 100999  |
| $..price                            | jmespath    | 34.582   | 2000    |
|                                     | query       | 39.165   | 100999  |
|                                     | jsonpath-ng | 527.779  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.140    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 40.450   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 19.01    | 27.93       | 28.23    | 2799.4   |
| msgspec       | 20.48    | 30.36       | 30.67    | 2800.1   |
| ujson         | 27.13    | 38.52       | 38.72    | 2817.9   |
| json (stdlib) | 35.15    | 44.00       | 44.22    | 2800.2   |
| strata        | 72.19    | 78.07       | 125.94   | 2779.8   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 22.14    | 23.99       | 24.05    | 143.7    |
| msgspec       | 25.28    | 28.82       | 29.62    | 143.4    |
| ujson         | 31.07    | 35.53       | 35.95    | 161.3    |
| json (stdlib) | 39.67    | 41.83       | 43.47    | 144.5    |
| strata        | 45.74    | 48.66       | 49.74    | 142.5    |
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
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 1       | 1421.0   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 1       | 1419.0   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 1       | 1419.0   |
| Extract all user IDs                        | jmespath    | 0.49     | 0.53        | 0.57     | 2000    | 553.4    |
| Extract all user IDs                        | strata      | 2.45     | 2.46        | 2.47     | 2000    | 553.3    |
| Extract all user IDs                        | jsonpath-ng | 6.05     | 6.23        | 6.41     | 2000    | 554.5    |
| Extract all user names                      | jmespath    | 0.60     | 0.65        | 0.69     | 2000    | 1780.0   |
| Extract all user names                      | strata      | 2.51     | 2.55        | 2.59     | 2000    | 1780.0   |
| Extract all user names                      | jsonpath-ng | 7.95     | 8.19        | 8.44     | 2000    | 1780.0   |
| Extract nested timestamp field              | jmespath    | 1.19     | 1.21        | 1.24     | 2000    | 985.6    |
| Extract nested timestamp field              | strata      | 2.35     | 2.39        | 2.43     | 2000    | 985.5    |
| Extract nested timestamp field              | jsonpath-ng | 7.27     | 7.50        | 7.73     | 2000    | 986.2    |
| Extract order item prices (double wildcard) | jmespath    | 34.55    | 34.56       | 34.57    | 2000    | 1155.6   |
| Extract order item prices (double wildcard) | strata      | 119.60   | 128.81      | 138.03   | 100999  | 1153.6   |
| Extract order item prices (double wildcard) | jsonpath-ng | 151.08   | 153.15      | 155.22   | 100999  | 1158.8   |
| Filter users by age (numeric predicate)     | strata      | 1.94     | 2.32        | 2.70     | 1606    | 1883.2   |
| Filter users by age (numeric predicate)     | jmespath    | 2.40     | 2.60        | 2.80     | 1606    | 1883.2   |
| Recursively find all prices                 | strata      | 192.68   | 200.60      | 208.52   | 100999  | 2011.4   |
| Recursively find all prices                 | jsonpath-ng | 511.64   | 527.62      | 543.60   | 100999  | 2014.2   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 2249.4   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 10      | 2248.8   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 10      | 2249.4   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.02     | 0       | 220.7    |
| Deep path navigation                        | jmespath            | 0.02     | 0.02        | 0.03     | 1       | 220.7    |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 1       | 220.7    |
| Extract all user IDs                        | jmespath            | 1.01     | 1.04        | 1.07     | 2000    | 214.2    |
| Extract all user IDs                        | strata              | 5.66     | 5.71        | 5.76     | 0       | 213.8    |
| Extract all user IDs                        | jsonpath-ng         | 7.28     | 8.16        | 9.05     | 2000    | 214.3    |
| Extract all user IDs                        | strata_ndjson_full  | 21.41    | 21.42       | 21.42    | 0       | 214.0    |
| Extract all user IDs                        | strata_ndjson_fused | 23.07    | 23.14       | 23.20    | 0       | 214.0    |
| Extract all user names                      | jmespath            | 0.82     | 0.88        | 0.94     | 2000    | 220.7    |
| Extract all user names                      | strata              | 5.70     | 5.70        | 5.71     | 0       | 220.7    |
| Extract all user names                      | jsonpath-ng         | 7.42     | 7.64        | 7.87     | 2000    | 220.7    |
| Extract all user names                      | strata_ndjson_fused | 21.57    | 21.59       | 21.62    | 0       | 220.7    |
| Extract all user names                      | strata_ndjson_full  | 21.58    | 21.60       | 21.61    | 0       | 220.7    |
| Extract nested timestamp field              | jmespath            | 1.37     | 1.40        | 1.42     | 2000    | 214.6    |
| Extract nested timestamp field              | strata              | 5.78     | 5.78        | 5.78     | 0       | 214.6    |
| Extract nested timestamp field              | jsonpath-ng         | 7.59     | 7.62        | 7.66     | 2000    | 214.6    |
| Extract order item prices (double wildcard) | strata              | 5.69     | 5.72        | 5.74     | 0       | 214.7    |
| Extract order item prices (double wildcard) | jmespath            | 26.33    | 26.75       | 27.16    | 100999  | 218.0    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 131.97   | 132.47      | 132.97   | 100999  | 222.7    |
| Filter users by age (numeric predicate)     | strata              | 0.01     | 0.01        | 0.02     | 0       | 220.8    |
| Filter users by age (numeric predicate)     | jmespath            | 2.50     | 2.52        | 2.54     | 1606    | 220.8    |
| NDJSON root field (id)                      | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 0       | 156.5    |
| NDJSON root field (id)                      | strata              | 9.40     | 9.49        | 9.58     | 2000    | 155.3    |
| NDJSON root field (id)                      | strata_ndjson_fused | 27.33    | 27.47       | 27.62    | 2000    | 156.5    |
| NDJSON root field (id)                      | strata_ndjson_full  | 27.58    | 27.67       | 27.76    | 2000    | 156.4    |
| Recursively find all prices                 | strata              | 197.16   | 200.56      | 203.97   | 1908    | 224.8    |
| Recursively find all prices                 | jsonpath-ng         | 507.50   | 508.77      | 510.05   | 100999  | 228.8    |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.02        | 0.02     | 10      | 228.3    |
| Slice first 10 users, extract IDs           | strata              | 0.01     | 0.02        | 0.02     | 0       | 229.3    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 10      | 227.3    |
