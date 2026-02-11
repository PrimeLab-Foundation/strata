# Strata Benchmark Results

Generated: 2026-02-12 00:35:32

## Environment

- Commit: 271ffab0b72d6e1fdcb5aeb8844621760c1bbb21
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users.json   | 3.399    | 3.573       | 3.649    | 324.2    |
| msgspec       | users.json   | 2.694    | 3.703       | 4.176    | 328.4    |
| ujson         | users.json   | 4.516    | 5.424       | 6.095    | 326.3    |
| json (stdlib) | users.json   | 4.930    | 6.095       | 6.254    | 329.4    |
| strata        | users.json   | 9.196    | 10.283      | 15.805   | 319.8    |
| orjson        | users.ndjson | 2.638    | 3.463       | 3.776    | 332.9    |
| msgspec       | users.ndjson | 2.809    | 3.866       | 4.229    | 334.9    |
| ujson         | users.ndjson | 3.797    | 5.577       | 5.863    | 334.9    |
| json (stdlib) | users.ndjson | 5.674    | 6.666       | 6.990    | 334.9    |
| strata        | users.ndjson | 6.666    | 7.576       | 7.686    | 330.9    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.028    | 1000    |
|                                     | jmespath    | 0.200    | 1000    |
|                                     | jsonpath-ng | 1.572    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.542    | 12046   |
|                                     | jmespath    | 5.663    | 1000    |
|                                     | jsonpath-ng | 18.348   | 12046   |
| $..price                            | jmespath    | 5.876    | 1000    |
|                                     | query       | 5.978    | 12046   |
|                                     | jsonpath-ng | 71.596   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.032    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.543    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 2.89     | 3.54        | 4.08     | 322.8    |
| msgspec       | 2.65     | 4.18        | 4.73     | 322.9    |
| ujson         | 3.72     | 5.72        | 6.59     | 326.0    |
| json (stdlib) | 5.45     | 6.18        | 6.55     | 325.0    |
| strata        | 9.59     | 10.12       | 15.54    | 319.0    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 2.74     | 3.60        | 3.80     | 36.5     |
| msgspec       | 2.84     | 3.70        | 3.71     | 36.6     |
| ujson         | 4.27     | 5.81        | 5.99     | 39.6     |
| strata        | 5.93     | 6.62        | 6.70     | 37.0     |
| json (stdlib) | 5.58     | 6.71        | 6.76     | 38.6     |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 1       | 224.2    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 224.2    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 1       | 224.2    |
| Extract all user IDs                        | jmespath    | 0.18     | 0.20        | 0.21     | 1000    | 97.9     |
| Extract all user IDs                        | strata      | 1.26     | 1.28        | 1.30     | 1000    | 97.9     |
| Extract all user IDs                        | jsonpath-ng | 1.53     | 1.60        | 1.68     | 1000    | 98.6     |
| Extract all user names                      | jmespath    | 0.19     | 0.19        | 0.20     | 1000    | 249.4    |
| Extract all user names                      | strata      | 1.18     | 1.20        | 1.23     | 1000    | 249.4    |
| Extract all user names                      | jsonpath-ng | 1.53     | 1.54        | 1.55     | 1000    | 249.4    |
| Extract nested timestamp field              | jmespath    | 0.53     | 0.56        | 0.58     | 1000    | 159.1    |
| Extract nested timestamp field              | strata      | 1.04     | 1.06        | 1.09     | 1000    | 159.1    |
| Extract nested timestamp field              | jsonpath-ng | 2.21     | 2.22        | 2.24     | 1000    | 159.4    |
| Extract order item prices (double wildcard) | jmespath    | 5.55     | 5.75        | 5.95     | 1000    | 191.6    |
| Extract order item prices (double wildcard) | strata      | 14.19    | 14.24       | 14.30    | 12046   | 191.1    |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.53    | 17.58       | 17.62    | 12046   | 194.8    |
| Filter users by age (numeric predicate)     | strata      | 0.99     | 1.01        | 1.04     | 794     | 277.2    |
| Filter users by age (numeric predicate)     | jmespath    | 1.06     | 1.18        | 1.31     | 794     | 277.2    |
| Recursively find all prices                 | strata      | 34.70    | 35.52       | 36.33    | 12046   | 314.9    |
| Recursively find all prices                 | jsonpath-ng | 74.92    | 74.99       | 75.06    | 12046   | 316.9    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.02     | 10      | 339.7    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.03     | 10      | 339.7    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.03     | 10      | 339.7    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.01        | 0.01     | 1       | 55.4     |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.01     | 0       | 55.4     |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 1       | 55.4     |
| Extract all user IDs                        | jmespath            | 0.23     | 0.23        | 0.23     | 1000    | 53.8     |
| Extract all user IDs                        | jsonpath-ng         | 1.53     | 1.57        | 1.60     | 1000    | 53.8     |
| Extract all user IDs                        | strata              | 2.57     | 2.59        | 2.61     | 0       | 53.7     |
| Extract all user IDs                        | strata_ndjson_fused | 4.11     | 4.14        | 4.16     | 0       | 53.7     |
| Extract all user IDs                        | strata_ndjson_full  | 8.63     | 8.74        | 8.84     | 0       | 53.7     |
| Extract all user names                      | jmespath            | 0.23     | 0.23        | 0.24     | 1000    | 55.5     |
| Extract all user names                      | jsonpath-ng         | 1.78     | 1.85        | 1.92     | 1000    | 55.5     |
| Extract all user names                      | strata              | 2.69     | 2.70        | 2.70     | 0       | 55.4     |
| Extract all user names                      | strata_ndjson_fused | 4.11     | 4.18        | 4.25     | 0       | 55.5     |
| Extract all user names                      | strata_ndjson_full  | 8.69     | 8.80        | 8.91     | 0       | 55.5     |
| Extract nested timestamp field              | jmespath            | 0.58     | 0.58        | 0.59     | 1000    | 53.9     |
| Extract nested timestamp field              | jsonpath-ng         | 2.21     | 2.24        | 2.27     | 1000    | 54.0     |
| Extract nested timestamp field              | strata              | 2.67     | 2.70        | 2.72     | 0       | 53.9     |
| Extract order item prices (double wildcard) | strata              | 2.65     | 2.72        | 2.79     | 0       | 54.0     |
| Extract order item prices (double wildcard) | jmespath            | 3.67     | 3.72        | 3.77     | 12046   | 54.3     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 19.58    | 20.17       | 20.76    | 12046   | 57.5     |
| Filter users by age (numeric predicate)     | strata              | 0.01     | 0.01        | 0.01     | 0       | 55.5     |
| Filter users by age (numeric predicate)     | jmespath            | 1.13     | 1.15        | 1.18     | 794     | 55.5     |
| NDJSON root field (id)                      | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 0       | 45.2     |
| NDJSON root field (id)                      | strata              | 1.96     | 1.98        | 2.00     | 1000    | 44.7     |
| NDJSON root field (id)                      | strata_ndjson_fused | 2.57     | 2.70        | 2.83     | 1000    | 45.2     |
| NDJSON root field (id)                      | strata_ndjson_full  | 8.27     | 8.31        | 8.34     | 1000    | 45.2     |
| Recursively find all prices                 | strata              | 33.01    | 33.30       | 33.60    | 878     | 55.6     |
| Recursively find all prices                 | jsonpath-ng         | 74.53    | 74.73       | 74.93    | 12046   | 58.4     |
| Slice first 10 users, extract IDs           | strata              | 0.01     | 0.01        | 0.01     | 0       | 57.4     |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.01        | 0.01     | 10      | 57.4     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 10      | 57.4     |
