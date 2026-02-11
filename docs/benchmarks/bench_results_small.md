# Strata Benchmark Results

Generated: 2026-02-11 22:50:08

## Environment

- Commit: 9ffdd71efb8fe8bda6bb58fb8f1387ca32d249cf
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
| orjson        | users.json   | 2.525    | 3.460       | 4.127    | 324.2    |
| msgspec       | users.json   | 2.899    | 3.534       | 3.708    | 328.4    |
| ujson         | users.json   | 3.919    | 5.431       | 5.936    | 326.3    |
| json (stdlib) | users.json   | 5.217    | 6.200       | 6.233    | 328.4    |
| strata        | users.json   | 10.349   | 10.624      | 17.823   | 319.8    |
| msgspec       | users.ndjson | 2.803    | 3.521       | 3.751    | 333.8    |
| orjson        | users.ndjson | 2.837    | 3.579       | 3.594    | 331.8    |
| ujson         | users.ndjson | 4.104    | 5.355       | 5.694    | 333.8    |
| json (stdlib) | users.ndjson | 5.468    | 6.497       | 6.553    | 333.8    |
| strata        | users.ndjson | 6.315    | 7.332       | 7.450    | 329.9    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.027    | 1000    |
|                                     | jmespath    | 0.188    | 1000    |
|                                     | jsonpath-ng | 1.587    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.524    | 12046   |
|                                     | jmespath    | 5.466    | 1000    |
|                                     | jsonpath-ng | 17.748   | 12046   |
| $..price                            | jmespath    | 5.730    | 1000    |
|                                     | query       | 5.881    | 12046   |
|                                     | jsonpath-ng | 72.451   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.032    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.128    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 2.61     | 3.64        | 3.81     | 322.8    |
| msgspec       | 3.06     | 3.64        | 4.11     | 322.9    |
| ujson         | 4.26     | 5.59        | 5.75     | 326.0    |
| json (stdlib) | 4.91     | 5.93        | 5.95     | 325.0    |
| strata        | 10.34    | 10.81       | 16.57    | 319.0    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 2.75     | 3.53        | 3.55     | 36.5     |
| msgspec       | 2.79     | 3.69        | 3.92     | 36.6     |
| ujson         | 4.04     | 5.38        | 5.91     | 39.6     |
| strata        | 5.63     | 6.09        | 6.32     | 37.0     |
| json (stdlib) | 5.51     | 6.58        | 6.59     | 38.6     |
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
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.00     | 1       | 223.8    |
| Deep path navigation                        | jmespath    | 0.00     | 0.01        | 0.01     | 1       | 223.8    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 1       | 223.8    |
| Extract all user IDs                        | jmespath    | 0.19     | 0.20        | 0.22     | 1000    | 97.7     |
| Extract all user IDs                        | strata      | 1.15     | 1.16        | 1.17     | 1000    | 97.7     |
| Extract all user IDs                        | jsonpath-ng | 1.54     | 1.56        | 1.59     | 1000    | 98.4     |
| Extract all user names                      | jmespath    | 0.19     | 0.19        | 0.19     | 1000    | 249.3    |
| Extract all user names                      | strata      | 1.17     | 1.19        | 1.21     | 1000    | 249.3    |
| Extract all user names                      | jsonpath-ng | 1.62     | 1.68        | 1.73     | 1000    | 249.3    |
| Extract nested timestamp field              | jmespath    | 0.55     | 0.56        | 0.57     | 1000    | 158.9    |
| Extract nested timestamp field              | strata      | 1.05     | 1.08        | 1.12     | 1000    | 158.9    |
| Extract nested timestamp field              | jsonpath-ng | 2.11     | 2.18        | 2.25     | 1000    | 159.2    |
| Extract order item prices (double wildcard) | jmespath    | 5.45     | 5.52        | 5.59     | 1000    | 191.3    |
| Extract order item prices (double wildcard) | strata      | 14.04    | 14.10       | 14.17    | 12046   | 190.8    |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.17    | 17.31       | 17.44    | 12046   | 194.6    |
| Filter users by age (numeric predicate)     | strata      | 0.89     | 0.94        | 0.99     | 794     | 273.7    |
| Filter users by age (numeric predicate)     | jmespath    | 1.10     | 1.11        | 1.11     | 794     | 273.7    |
| Recursively find all prices                 | strata      | 33.55    | 33.63       | 33.72    | 12046   | 314.7    |
| Recursively find all prices                 | jsonpath-ng | 71.69    | 71.93       | 72.16    | 12046   | 316.4    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.02     | 10      | 336.1    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 10      | 336.1    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 10      | 336.1    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 0       | 55.7     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 1       | 55.7     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 1       | 55.7     |
| Extract all user IDs                        | jmespath    | 0.22     | 0.22        | 0.23     | 1000    | 44.1     |
| Extract all user IDs                        | jsonpath-ng | 1.55     | 1.59        | 1.63     | 1000    | 44.7     |
| Extract all user IDs                        | strata      | 2.50     | 2.55        | 2.61     | 0       | 44.1     |
| Extract all user names                      | jmespath    | 0.23     | 0.24        | 0.24     | 1000    | 55.7     |
| Extract all user names                      | jsonpath-ng | 1.68     | 1.75        | 1.81     | 1000    | 55.7     |
| Extract all user names                      | strata      | 2.47     | 2.59        | 2.71     | 0       | 55.7     |
| Extract nested timestamp field              | jmespath    | 0.58     | 0.63        | 0.68     | 1000    | 53.2     |
| Extract nested timestamp field              | jsonpath-ng | 2.08     | 2.23        | 2.38     | 1000    | 53.4     |
| Extract nested timestamp field              | strata      | 2.54     | 2.59        | 2.64     | 0       | 53.2     |
| Extract order item prices (double wildcard) | strata      | 2.55     | 2.58        | 2.60     | 0       | 53.4     |
| Extract order item prices (double wildcard) | jmespath    | 3.52     | 3.58        | 3.63     | 12046   | 53.5     |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.93    | 18.04       | 18.14    | 12046   | 56.6     |
| Filter users by age (numeric predicate)     | strata      | 0.01     | 0.01        | 0.01     | 0       | 55.7     |
| Filter users by age (numeric predicate)     | jmespath    | 1.07     | 1.08        | 1.08     | 794     | 55.7     |
| Recursively find all prices                 | strata      | 31.96    | 32.19       | 32.42    | 878     | 55.8     |
| Recursively find all prices                 | jsonpath-ng | 71.61    | 71.94       | 72.27    | 12046   | 56.6     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 0       | 56.6     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 10      | 56.6     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.02     | 10      | 55.8     |
