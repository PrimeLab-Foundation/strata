# Strata Benchmark Results

Generated: 2026-02-14 13:19:45

## Environment

- Commit: c97b616486f501631c86fcd91188be9d26c26404
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
| orjson        | users.json   | 2.366    | 3.912       | 4.390    | 324.3    |
| msgspec       | users.json   | 4.156    | 5.304       | 6.242    | 327.4    |
| json (stdlib) | users.json   | 6.167    | 6.199       | 6.466    | 329.4    |
| ujson         | users.json   | 4.178    | 6.473       | 6.858    | 326.4    |
| strata        | users.json   | 11.300   | 11.421      | 17.894   | 319.9    |
| orjson        | users.ndjson | 2.900    | 3.525       | 3.874    | 332.9    |
| msgspec       | users.ndjson | 2.607    | 3.743       | 3.989    | 334.9    |
| ujson         | users.ndjson | 4.714    | 5.895       | 6.078    | 334.9    |
| json (stdlib) | users.ndjson | 5.861    | 6.931       | 7.081    | 334.9    |
| strata        | users.ndjson | 6.935    | 8.761       | 8.998    | 331.0    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.030    | 1000    |
|                                     | jmespath    | 0.204    | 1000    |
|                                     | jsonpath-ng | 1.631    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.543    | 12046   |
|                                     | jmespath    | 5.431    | 1000    |
|                                     | jsonpath-ng | 19.098   | 12046   |
| $..price                            | jmespath    | 5.634    | 1000    |
|                                     | query       | 5.748    | 12046   |
|                                     | jsonpath-ng | 72.986   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.035    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.222    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 2.55     | 3.92        | 4.96     | 257.22 | 323.0    |
| orjson        | 2.79     | 4.02        | 4.04     | 250.88 | 322.9    |
| json (stdlib) | 5.48     | 6.17        | 6.28     | 163.47 | 326.0    |
| ujson         | 3.73     | 6.44        | 6.60     | 156.63 | 326.1    |
| strata        | 10.84    | 11.49       | 18.35    | 87.74  | 319.1    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 2.67     | 3.57        | 4.42     | 281.96 | 36.5     |
| msgspec       | 2.78     | 3.66        | 4.56     | 275.03 | 36.6     |
| ujson         | 3.74     | 5.89        | 6.04     | 171.01 | 39.6     |
| json (stdlib) | 5.62     | 6.63        | 6.74     | 151.96 | 38.6     |
| strata        | 6.43     | 6.76        | 6.89     | 148.92 | 37.0     |
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
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.01     | 191272.29 | 1       | 251.2    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 75972.90  | 1       | 251.2    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 69033.42  | 1       | 251.2    |
| Extract all user IDs                        | jmespath    | 0.22     | 0.25        | 0.27     | 4098.02   | 1000    | 97.9     |
| Extract all user IDs                        | strata      | 1.13     | 1.15        | 1.16     | 879.50    | 1000    | 97.8     |
| Extract all user IDs                        | jsonpath-ng | 1.76     | 1.99        | 2.23     | 505.67    | 1000    | 98.7     |
| Extract all user names                      | jmespath    | 0.18     | 0.20        | 0.22     | 5149.88   | 1000    | 283.8    |
| Extract all user names                      | strata      | 1.16     | 1.20        | 1.24     | 839.81    | 1000    | 283.8    |
| Extract all user names                      | jsonpath-ng | 1.51     | 1.55        | 1.59     | 650.98    | 1000    | 283.8    |
| Extract nested timestamp field              | jmespath    | 0.59     | 0.60        | 0.61     | 1677.01   | 1000    | 159.5    |
| Extract nested timestamp field              | strata      | 1.26     | 1.26        | 1.26     | 799.50    | 1000    | 159.5    |
| Extract nested timestamp field              | jsonpath-ng | 2.08     | 2.10        | 2.12     | 480.34    | 1000    | 159.8    |
| Extract order item prices (double wildcard) | jmespath    | 5.20     | 5.23        | 5.26     | 192.74    | 1000    | 210.9    |
| Extract order item prices (double wildcard) | strata      | 14.11    | 14.21       | 14.32    | 70.94     | 12046   | 210.4    |
| Extract order item prices (double wildcard) | jsonpath-ng | 16.88    | 16.90       | 16.93    | 59.64     | 12046   | 215.1    |
| Filter users by age (numeric predicate)     | strata      | 0.95     | 0.99        | 1.03     | 1017.82   | 794     | 306.9    |
| Filter users by age (numeric predicate)     | jmespath    | 1.09     | 1.15        | 1.22     | 875.84    | 794     | 306.9    |
| Recursively find all prices                 | strata      | 16.60    | 16.81       | 17.03    | 59.97     | 12046   | 307.5    |
| Recursively find all prices                 | jsonpath-ng | 67.58    | 67.87       | 68.16    | 14.85     | 12046   | 309.2    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 72662.94  | 10      | 318.2    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.02     | 55307.36  | 10      | 318.2    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.03     | 54867.94  | 10      | 318.2    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 41.45    | 42.54       | 43.62    | 189.61 | 376.7    |
| strata_cursor_reparse | 110.96   | 114.01      | 117.06   | 70.74  | 823.0    |
- Speedup (reuse vs reparse): 2.68x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.01        | 0.01     | 149667.51 | 1       | 54.8     |
| Deep path navigation                        | strata              | 0.00     | 0.01        | 0.02     | 84079.40  | 1       | 54.8     |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 66498.56  | 1       | 54.8     |
| Extract all user IDs                        | jsonpath-ng         | 0.00     | 0.01        | 0.02     | 103079.22 | 0       | 53.6     |
| Extract all user IDs                        | jmespath            | 0.21     | 0.22        | 0.23     | 4520.32   | 1000    | 53.6     |
| Extract all user IDs                        | strata              | 2.04     | 2.14        | 2.24     | 469.96    | 1000    | 53.2     |
| Extract all user IDs                        | strata_ndjson_fused | 2.58     | 2.65        | 2.72     | 380.34    | 1000    | 53.6     |
| Extract all user IDs                        | strata_ndjson_full  | 8.97     | 9.08        | 9.19     | 110.95    | 1000    | 53.5     |
| Extract all user names                      | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 133181.63 | 0       | 54.8     |
| Extract all user names                      | jmespath            | 0.27     | 0.29        | 0.30     | 3533.99   | 1000    | 54.8     |
| Extract all user names                      | strata              | 1.89     | 1.99        | 2.09     | 505.71    | 1000    | 54.8     |
| Extract all user names                      | strata_ndjson_fused | 2.82     | 3.08        | 3.35     | 326.48    | 1000    | 54.8     |
| Extract all user names                      | strata_ndjson_full  | 8.77     | 8.87        | 8.98     | 113.49    | 1000    | 54.8     |
| Extract nested timestamp field              | jsonpath-ng         | 0.00     | 0.00        | 0.01     | 231324.22 | 0       | 53.8     |
| Extract nested timestamp field              | jmespath            | 0.61     | 0.67        | 0.72     | 1511.96   | 1000    | 53.8     |
| Extract nested timestamp field              | strata              | 2.23     | 2.33        | 2.43     | 433.06    | 1000    | 53.8     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.00     | 0.00        | 0.00     | 217770.22 | 0       | 54.8     |
| Extract order item prices (double wildcard) | jmespath            | 3.51     | 3.52        | 3.53     | 286.25    | 12046   | 54.8     |
| Extract order item prices (double wildcard) | strata              | 12.48    | 12.59       | 12.70    | 80.00     | 12046   | 54.5     |
| Filter users by age (numeric predicate)     | strata              | 0.93     | 0.94        | 0.96     | 1067.83   | 794     | 54.9     |
| Filter users by age (numeric predicate)     | jmespath            | 1.15     | 1.19        | 1.23     | 846.55    | 794     | 54.9     |
| NDJSON first record id                      | strata              | 0.01     | 0.01        | 0.01     | 92614.87  | 1       | 44.1     |
| NDJSON first record id                      | jmespath            | 0.01     | 0.01        | 0.02     | 72590.03  | 1       | 44.1     |
| NDJSON first record id                      | jsonpath-ng         | 0.02     | 0.02        | 0.03     | 43553.10  | 1       | 44.4     |
| Recursively find all prices                 | strata              | 13.52    | 13.59       | 13.66    | 74.12     | 12046   | 55.1     |
| Recursively find all prices                 | jsonpath-ng         | 67.05    | 67.24       | 67.44    | 14.98     | 12046   | 57.0     |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.01        | 0.02     | 80574.96  | 10      | 57.0     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 70886.24  | 10      | 56.1     |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.02        | 0.02     | 56281.60  | 10      | 57.0     |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 38.82    | 39.58       | 40.35    | 229.00 | 56.3     |
| strata_cursor_reparse | 105.66   | 106.09      | 106.51   | 85.45  | 64.6     |
- Speedup (reuse vs reparse): 2.68x
