# Strata Benchmark Results

Generated: 2026-02-13 01:34:43

## Environment

- Commit: 9b7be5525389f90d56aeaa6b54e263bfaac39521
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
| orjson        | users.json   | 3.148    | 3.800       | 4.431    | 335.0    |
| msgspec       | users.json   | 3.009    | 3.870       | 4.810    | 338.2    |
| ujson         | users.json   | 4.108    | 6.887       | 7.196    | 337.1    |
| json (stdlib) | users.json   | 5.763    | 6.899       | 6.967    | 339.2    |
| strata        | users.json   | 11.952   | 12.509      | 18.375   | 330.6    |
| orjson        | users.ndjson | 2.705    | 3.960       | 4.171    | 342.2    |
| msgspec       | users.ndjson | 3.503    | 4.439       | 4.777    | 344.3    |
| ujson         | users.ndjson | 4.575    | 5.833       | 5.938    | 344.3    |
| json (stdlib) | users.ndjson | 6.285    | 7.124       | 7.272    | 344.3    |
| strata        | users.ndjson | 9.161    | 9.757       | 11.743   | 340.3    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.029    | 1000    |
|                                     | jmespath    | 0.197    | 1000    |
|                                     | jsonpath-ng | 1.963    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.558    | 12046   |
|                                     | jmespath    | 5.875    | 1000    |
|                                     | jsonpath-ng | 19.881   | 12046   |
| $..price                            | jmespath    | 5.569    | 1000    |
|                                     | query       | 6.376    | 12046   |
|                                     | jsonpath-ng | 76.829   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.039    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 7.017    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 2.87     | 3.85        | 4.20     | 261.70 | 323.0    |
| orjson        | 2.56     | 3.90        | 4.56     | 258.45 | 322.9    |
| ujson         | 5.78     | 5.94        | 6.55     | 169.61 | 326.0    |
| json (stdlib) | 4.93     | 6.45        | 7.77     | 156.40 | 325.0    |
| strata        | 11.54    | 12.63       | 18.32    | 79.80  | 319.1    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 2.67     | 3.59        | 4.21     | 280.38 | 36.6     |
| orjson        | 2.75     | 3.95        | 4.43     | 255.29 | 36.6     |
| ujson         | 4.29     | 6.03        | 6.61     | 167.04 | 39.7     |
| json (stdlib) | 6.00     | 6.96        | 7.06     | 144.69 | 38.7     |
| strata        | 6.39     | 7.13        | 7.23     | 141.26 | 37.0     |
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
| Deep path navigation                        | strata      | 0.00     | 0.00        | 0.01     | 204191.95 | 1       | 235.3    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 162389.90 | 1       | 235.3    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 115499.89 | 1       | 235.3    |
| Extract all user IDs                        | jmespath    | 0.19     | 0.22        | 0.25     | 4536.74   | 1000    | 100.1    |
| Extract all user IDs                        | strata      | 1.15     | 1.17        | 1.18     | 864.60    | 1000    | 100.0    |
| Extract all user IDs                        | jsonpath-ng | 1.51     | 1.72        | 1.92     | 587.34    | 1000    | 100.8    |
| Extract all user names                      | jmespath    | 0.19     | 0.21        | 0.22     | 4864.15   | 1000    | 263.9    |
| Extract all user names                      | strata      | 1.09     | 1.10        | 1.10     | 919.94    | 1000    | 263.9    |
| Extract all user names                      | jsonpath-ng | 1.54     | 1.65        | 1.76     | 611.22    | 1000    | 263.9    |
| Extract nested timestamp field              | jmespath    | 0.54     | 0.54        | 0.54     | 1869.92   | 1000    | 161.3    |
| Extract nested timestamp field              | strata      | 1.38     | 1.42        | 1.46     | 709.57    | 1000    | 161.3    |
| Extract nested timestamp field              | jsonpath-ng | 1.97     | 2.16        | 2.36     | 465.75    | 1000    | 161.6    |
| Extract order item prices (double wildcard) | jmespath    | 5.19     | 5.29        | 5.38     | 190.69    | 1000    | 203.4    |
| Extract order item prices (double wildcard) | strata      | 13.04    | 13.70       | 14.35    | 73.61     | 12046   | 202.8    |
| Extract order item prices (double wildcard) | jsonpath-ng | 16.94    | 16.95       | 16.96    | 59.48     | 12046   | 207.6    |
| Filter users by age (numeric predicate)     | jmespath    | 1.10     | 1.12        | 1.14     | 901.28    | 794     | 290.2    |
| Filter users by age (numeric predicate)     | strata      | 1.26     | 1.27        | 1.27     | 795.65    | 794     | 290.2    |
| Recursively find all prices                 | strata      | 13.33    | 13.43       | 13.53    | 75.08     | 12046   | 315.9    |
| Recursively find all prices                 | jsonpath-ng | 68.43    | 68.67       | 68.92    | 14.68     | 12046   | 317.6    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 147537.59 | 10      | 338.8    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.02        | 0.02     | 63095.19  | 10      | 338.8    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 53177.81  | 10      | 338.8    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 41.76    | 44.88       | 48.01    | 179.70 | 362.4    |
| strata_cursor_reparse | 106.94   | 107.83      | 108.71   | 74.80  | 729.7    |
- Speedup (reuse vs reparse): 2.40x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.01        | 0.01     | 172655.72 | 1       | 54.9     |
| Deep path navigation                        | strata              | 0.00     | 0.01        | 0.01     | 140541.03 | 1       | 54.9     |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 60659.28  | 1       | 54.9     |
| Extract all user IDs                        | jsonpath-ng         | 0.00     | 0.01        | 0.02     | 72918.54  | 0       | 53.8     |
| Extract all user IDs                        | jmespath            | 0.23     | 0.28        | 0.32     | 3633.87   | 1000    | 53.8     |
| Extract all user IDs                        | strata              | 1.99     | 2.07        | 2.15     | 486.75    | 1000    | 53.3     |
| Extract all user IDs                        | strata_ndjson_fused | 2.68     | 2.77        | 2.85     | 363.74    | 1000    | 53.7     |
| Extract all user IDs                        | strata_ndjson_full  | 8.97     | 9.00        | 9.04     | 111.86    | 1000    | 53.7     |
| Extract all user names                      | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 185229.87 | 0       | 54.9     |
| Extract all user names                      | jmespath            | 0.22     | 0.22        | 0.22     | 4565.58   | 1000    | 54.9     |
| Extract all user names                      | strata              | 2.02     | 2.03        | 2.04     | 496.13    | 1000    | 54.9     |
| Extract all user names                      | strata_ndjson_fused | 2.59     | 2.82        | 3.06     | 356.84    | 1000    | 54.9     |
| Extract all user names                      | strata_ndjson_full  | 8.87     | 8.96        | 9.04     | 112.45    | 1000    | 54.9     |
| Extract nested timestamp field              | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 146948.88 | 0       | 54.0     |
| Extract nested timestamp field              | jmespath            | 0.58     | 0.60        | 0.62     | 1681.80   | 1000    | 54.0     |
| Extract nested timestamp field              | strata              | 2.18     | 2.18        | 2.19     | 461.04    | 1000    | 54.0     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 71096.39  | 0       | 54.9     |
| Extract order item prices (double wildcard) | jmespath            | 3.53     | 3.53        | 3.54     | 284.98    | 12046   | 54.9     |
| Extract order item prices (double wildcard) | strata              | 12.67    | 12.80       | 12.92    | 78.71     | 12046   | 54.8     |
| Filter users by age (numeric predicate)     | strata              | 0.01     | 0.01        | 0.01     | 157471.31 | 0       | 55.0     |
| Filter users by age (numeric predicate)     | jmespath            | 1.10     | 1.13        | 1.16     | 891.23    | 794     | 55.0     |
| NDJSON first record id                      | strata              | 0.00     | 0.01        | 0.01     | 185947.96 | 1       | 44.2     |
| NDJSON first record id                      | jmespath            | 0.01     | 0.01        | 0.01     | 152986.62 | 1       | 44.2     |
| NDJSON first record id                      | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 126554.87 | 1       | 44.5     |
| Recursively find all prices                 | strata              | 13.70    | 13.81       | 13.92    | 72.94     | 12046   | 55.0     |
| Recursively find all prices                 | jsonpath-ng         | 68.84    | 69.30       | 69.76    | 14.53     | 12046   | 56.9     |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.01        | 0.01     | 84079.40  | 10      | 56.9     |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.02        | 0.02     | 54564.92  | 10      | 56.9     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 52777.90  | 10      | 56.1     |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 38.70    | 39.19       | 39.68    | 231.31 | 56.2     |
| strata_cursor_reparse | 106.66   | 106.95      | 107.25   | 84.75  | 64.7     |
- Speedup (reuse vs reparse): 2.73x
