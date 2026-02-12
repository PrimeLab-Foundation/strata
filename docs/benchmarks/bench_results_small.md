# Strata Benchmark Results

Generated: 2026-02-12 02:38:49

## Environment

- Commit: 10634f897c158962cdece183af3a38887bc5f426
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
| orjson        | users.json   | 2.688    | 3.880       | 3.918    | 324.3    |
| msgspec       | users.json   | 3.007    | 3.911       | 4.111    | 327.4    |
| ujson         | users.json   | 3.854    | 5.727       | 6.106    | 326.4    |
| json (stdlib) | users.json   | 5.362    | 6.097       | 6.142    | 328.4    |
| strata        | users.json   | 11.084   | 11.489      | 16.484   | 319.9    |
| msgspec       | users.ndjson | 2.853    | 3.568       | 3.588    | 333.8    |
| orjson        | users.ndjson | 2.596    | 3.586       | 3.729    | 331.8    |
| ujson         | users.ndjson | 3.880    | 5.361       | 5.566    | 333.8    |
| json (stdlib) | users.ndjson | 5.535    | 6.470       | 6.591    | 333.8    |
| strata        | users.ndjson | 7.281    | 8.394       | 8.514    | 329.9    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.026    | 1000    |
|                                     | jmespath    | 0.187    | 1000    |
|                                     | jsonpath-ng | 1.666    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.537    | 12046   |
|                                     | jmespath    | 5.560    | 1000    |
|                                     | jsonpath-ng | 18.401   | 12046   |
| $..price                            | jmespath    | 5.626    | 1000    |
|                                     | query       | 6.039    | 12046   |
|                                     | jsonpath-ng | 72.332   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.029    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.524    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 2.87     | 3.60        | 4.11     | 280.06 | 323.0    |
| orjson        | 2.58     | 3.68        | 4.33     | 273.87 | 322.9    |
| ujson         | 4.36     | 5.83        | 6.53     | 173.03 | 326.0    |
| json (stdlib) | 5.34     | 6.13        | 6.48     | 164.49 | 325.0    |
| strata        | 10.49    | 11.17       | 16.80    | 90.22  | 319.1    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 2.76     | 3.50        | 3.56     | 287.36 | 36.5     |
| msgspec       | 2.80     | 3.73        | 4.21     | 269.99 | 36.6     |
| ujson         | 4.06     | 5.38        | 5.98     | 187.29 | 39.7     |
| json (stdlib) | 5.53     | 6.58        | 6.76     | 153.03 | 38.7     |
| strata        | 6.48     | 7.02        | 7.07     | 143.57 | 36.0     |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.01     | 193586.42 | 1       | 224.8    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 163496.05 | 1       | 224.8    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 141501.41 | 1       | 224.8    |
| Extract all user IDs                        | jmespath    | 0.19     | 0.21        | 0.23     | 4743.08   | 1000    | 97.9     |
| Extract all user IDs                        | strata      | 1.20     | 1.21        | 1.23     | 830.56    | 1000    | 97.8     |
| Extract all user IDs                        | jsonpath-ng | 1.49     | 1.63        | 1.78     | 616.98    | 1000    | 98.6     |
| Extract all user names                      | jmespath    | 0.19     | 0.20        | 0.21     | 4989.52   | 1000    | 249.9    |
| Extract all user names                      | strata      | 1.10     | 1.16        | 1.21     | 871.31    | 1000    | 249.9    |
| Extract all user names                      | jsonpath-ng | 1.53     | 1.69        | 1.85     | 597.07    | 1000    | 249.9    |
| Extract nested timestamp field              | jmespath    | 0.52     | 0.53        | 0.54     | 1897.19   | 1000    | 159.1    |
| Extract nested timestamp field              | strata      | 1.04     | 1.08        | 1.12     | 932.31    | 1000    | 159.0    |
| Extract nested timestamp field              | jsonpath-ng | 2.11     | 2.15        | 2.18     | 470.02    | 1000    | 159.4    |
| Extract order item prices (double wildcard) | jmespath    | 5.50     | 5.60        | 5.70     | 180.03    | 1000    | 191.9    |
| Extract order item prices (double wildcard) | strata      | 13.73    | 13.79       | 13.85    | 73.13     | 12046   | 191.4    |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.49    | 17.60       | 17.70    | 57.30     | 12046   | 195.2    |
| Filter users by age (numeric predicate)     | strata      | 0.94     | 0.97        | 1.01     | 1035.60   | 794     | 276.1    |
| Filter users by age (numeric predicate)     | jmespath    | 1.08     | 1.13        | 1.17     | 894.67    | 794     | 276.1    |
| Recursively find all prices                 | strata      | 32.81    | 33.42       | 34.03    | 30.17     | 12046   | 300.3    |
| Recursively find all prices                 | jsonpath-ng | 72.46    | 72.57       | 72.68    | 13.89     | 12046   | 302.0    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 162389.90 | 10      | 323.1    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 81195.00  | 10      | 323.1    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.01        | 0.01     | 73211.71  | 10      | 323.1    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 59.73    | 60.60       | 61.46    | 133.10 | 345.3    |
| strata_cursor_reparse | 126.61   | 126.64      | 126.67   | 63.69  | 597.1    |
- Speedup (reuse vs reparse): 2.09x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.00     | 0.01        | 0.01     | 175805.20 | 1       | 55.0     |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.01     | 167864.44 | 0       | 55.0     |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 103084.51 | 1       | 55.0     |
| Extract all user IDs                        | jmespath            | 0.23     | 0.26        | 0.29     | 3879.08   | 1000    | 52.7     |
| Extract all user IDs                        | jsonpath-ng         | 1.52     | 1.69        | 1.87     | 595.07    | 1000    | 52.7     |
| Extract all user IDs                        | strata              | 2.60     | 2.63        | 2.67     | 382.53    | 0       | 52.7     |
| Extract all user IDs                        | strata_ndjson_fused | 4.50     | 4.53        | 4.55     | 222.56    | 0       | 52.7     |
| Extract all user IDs                        | strata_ndjson_full  | 9.35     | 9.39        | 9.43     | 107.25    | 0       | 52.7     |
| Extract all user names                      | jmespath            | 0.21     | 0.21        | 0.22     | 4698.70   | 1000    | 55.1     |
| Extract all user names                      | jsonpath-ng         | 1.52     | 1.56        | 1.61     | 643.68    | 1000    | 55.1     |
| Extract all user names                      | strata              | 2.62     | 2.64        | 2.65     | 381.97    | 0       | 55.0     |
| Extract all user names                      | strata_ndjson_fused | 4.68     | 4.69        | 4.69     | 214.95    | 0       | 55.1     |
| Extract all user names                      | strata_ndjson_full  | 9.35     | 9.40        | 9.46     | 107.10    | 0       | 55.1     |
| Extract nested timestamp field              | jmespath            | 0.60     | 0.65        | 0.69     | 1556.55   | 1000    | 52.8     |
| Extract nested timestamp field              | jsonpath-ng         | 2.14     | 2.28        | 2.43     | 440.89    | 1000    | 52.9     |
| Extract nested timestamp field              | strata              | 2.56     | 2.58        | 2.60     | 390.58    | 0       | 52.8     |
| Extract order item prices (double wildcard) | strata              | 2.51     | 2.58        | 2.66     | 389.84    | 0       | 52.9     |
| Extract order item prices (double wildcard) | jmespath            | 3.56     | 3.63        | 3.71     | 277.25    | 12046   | 53.0     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 18.54    | 18.61       | 18.68    | 54.12     | 12046   | 56.1     |
| Filter users by age (numeric predicate)     | strata              | 0.01     | 0.01        | 0.01     | 138922.40 | 0       | 55.1     |
| Filter users by age (numeric predicate)     | jmespath            | 1.11     | 1.16        | 1.21     | 870.11    | 794     | 55.1     |
| NDJSON root field (id)                      | jsonpath-ng         | 0.00     | 0.01        | 0.01     | 157990.18 | 0       | 44.3     |
| NDJSON root field (id)                      | strata              | 1.91     | 1.95        | 1.98     | 517.65    | 1000    | 43.8     |
| NDJSON root field (id)                      | strata_ndjson_fused | 2.61     | 2.75        | 2.90     | 365.60    | 1000    | 44.3     |
| NDJSON root field (id)                      | strata_ndjson_full  | 8.69     | 8.77        | 8.86     | 114.81    | 1000    | 44.2     |
| Recursively find all prices                 | strata              | 32.46    | 32.55       | 32.65    | 30.94     | 878     | 55.2     |
| Recursively find all prices                 | jsonpath-ng         | 73.52    | 73.60       | 73.68    | 13.68     | 12046   | 56.0     |
| Slice first 10 users, extract IDs           | strata              | 0.01     | 0.01        | 0.01     | 173907.73 | 0       | 56.0     |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.01        | 0.02     | 75188.44  | 10      | 56.0     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 72590.03  | 10      | 55.2     |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 53.54    | 53.79       | 54.05    | 168.52 | 55.3     |
| strata_cursor_reparse | 126.48   | 126.72      | 126.97   | 71.53  | 63.7     |
- Speedup (reuse vs reparse): 2.36x
