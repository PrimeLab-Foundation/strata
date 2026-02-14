# Strata Benchmark Results

Generated: 2026-02-14 17:08:30

## Environment

- Commit: 8d68dc2d967bbefb0f220f9b3733bf92fb7c966a
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
| msgspec       | users.json   | 2.549    | 4.149       | 4.153    | 361.3    |
| orjson        | users.json   | 2.714    | 4.271       | 4.637    | 358.2    |
| ujson         | users.json   | 3.769    | 6.067       | 6.460    | 360.2    |
| json (stdlib) | users.json   | 5.276    | 6.309       | 6.676    | 362.3    |
| strata        | users.json   | 13.547   | 14.464      | 17.105   | 353.8    |
| orjson        | users.ndjson | 2.697    | 3.647       | 3.896    | 365.8    |
| msgspec       | users.ndjson | 2.924    | 3.702       | 3.760    | 367.8    |
| ujson         | users.ndjson | 4.014    | 5.758       | 5.896    | 367.8    |
| json (stdlib) | users.ndjson | 5.717    | 6.988       | 7.007    | 367.8    |
| strata        | users.ndjson | 7.918    | 9.361       | 9.439    | 363.9    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.029    | 1000    |
|                                     | jmespath    | 0.189    | 1000    |
|                                     | jsonpath-ng | 1.671    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.567    | 12046   |
|                                     | jmespath    | 6.053    | 1000    |
|                                     | jsonpath-ng | 18.679   | 12046   |
| $..price                            | jmespath    | 5.902    | 1000    |
|                                     | query       | 5.957    | 12046   |
|                                     | jsonpath-ng | 71.915   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.029    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.545    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 2.74     | 3.41        | 4.32     | 295.93 | 431.6    |
| msgspec       | 2.74     | 3.81        | 3.98     | 264.33 | 431.7    |
| ujson         | 3.96     | 5.92        | 6.05     | 170.19 | 434.7    |
| json (stdlib) | 5.51     | 6.40        | 6.76     | 157.59 | 433.7    |
| strata        | 12.62    | 16.03       | 17.01    | 62.91  | 427.8    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 2.90     | 3.69        | 3.90     | 273.07 | 36.6     |
| orjson        | 2.69     | 3.82        | 3.88     | 263.55 | 36.5     |
| ujson         | 4.09     | 5.61        | 5.79     | 179.53 | 39.7     |
| json (stdlib) | 5.74     | 6.96        | 7.03     | 144.62 | 38.7     |
| strata        | 6.75     | 7.63        | 7.68     | 131.93 | 37.0     |
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
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 145767.16 | 1       | 225.1    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 142753.79 | 1       | 225.1    |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 115771.63 | 1       | 225.1    |
| Extract all user IDs                        | jmespath    | 0.21     | 0.23        | 0.25     | 4383.87   | 1000    | 97.8     |
| Extract all user IDs                        | strata      | 1.27     | 1.35        | 1.42     | 748.08    | 1000    | 97.6     |
| Extract all user IDs                        | jsonpath-ng | 1.73     | 1.79        | 1.85     | 563.26    | 1000    | 98.6     |
| Extract all user names                      | jmespath    | 0.18     | 0.19        | 0.20     | 5303.97   | 1000    | 250.2    |
| Extract all user names                      | strata      | 1.13     | 1.15        | 1.17     | 877.62    | 1000    | 250.2    |
| Extract all user names                      | jsonpath-ng | 1.50     | 1.54        | 1.58     | 653.20    | 1000    | 250.2    |
| Extract nested timestamp field              | jmespath    | 0.53     | 0.58        | 0.63     | 1730.50   | 1000    | 159.1    |
| Extract nested timestamp field              | strata      | 1.45     | 1.48        | 1.51     | 682.90    | 1000    | 159.1    |
| Extract nested timestamp field              | jsonpath-ng | 2.18     | 2.32        | 2.45     | 434.83    | 1000    | 159.5    |
| Extract order item prices (double wildcard) | jmespath    | 5.72     | 5.75        | 5.78     | 175.30    | 1000    | 192.1    |
| Extract order item prices (double wildcard) | strata      | 14.60    | 14.77       | 14.93    | 68.27     | 12046   | 191.6    |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.04    | 18.06       | 18.08    | 55.82     | 12046   | 195.3    |
| Filter users by age (numeric predicate)     | jmespath    | 1.07     | 1.09        | 1.12     | 922.61    | 794     | 276.5    |
| Filter users by age (numeric predicate)     | strata      | 1.29     | 1.33        | 1.37     | 759.04    | 794     | 276.5    |
| Recursively find all prices                 | strata      | 15.44    | 15.95       | 16.46    | 63.22     | 12046   | 300.6    |
| Recursively find all prices                 | jsonpath-ng | 72.93    | 72.98       | 73.02    | 13.82     | 12046   | 302.3    |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 77805.06  | 10      | 321.6    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.03     | 48103.36  | 10      | 321.6    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 40463.06  | 10      | 321.6    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 44.45    | 44.88       | 45.30    | 179.72 | 347.1    |
| strata_cursor_reparse | 109.03   | 111.03      | 113.04   | 72.64  | 711.9    |
- Speedup (reuse vs reparse): 2.47x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.00     | 0.01        | 0.01     | 182427.99 | 1       | 54.1     |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.01     | 141767.33 | 1       | 54.1     |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 135803.55 | 1       | 54.1     |
| Extract all user IDs                        | jsonpath-ng         | 0.00     | 0.01        | 0.02     | 100930.75 | 0       | 52.9     |
| Extract all user IDs                        | jmespath            | 0.22     | 0.22        | 0.22     | 4566.88   | 1000    | 52.9     |
| Extract all user IDs                        | strata              | 2.05     | 2.11        | 2.17     | 477.62    | 1000    | 52.5     |
| Extract all user IDs                        | strata_ndjson_fused | 2.81     | 2.87        | 2.94     | 350.71    | 1000    | 52.9     |
| Extract all user IDs                        | strata_ndjson_full  | 9.62     | 9.83        | 10.04    | 102.44    | 1000    | 52.8     |
| Extract all user names                      | jsonpath-ng         | 0.00     | 0.00        | 0.00     | 267086.63 | 0       | 54.1     |
| Extract all user names                      | jmespath            | 0.22     | 0.22        | 0.22     | 4636.96   | 1000    | 54.1     |
| Extract all user names                      | strata              | 2.09     | 2.23        | 2.38     | 451.04    | 1000    | 54.1     |
| Extract all user names                      | strata_ndjson_fused | 2.83     | 2.90        | 2.98     | 346.73    | 1000    | 54.1     |
| Extract all user names                      | strata_ndjson_full  | 9.51     | 9.61        | 9.72     | 104.76    | 1000    | 54.1     |
| Extract nested timestamp field              | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 98661.57  | 0       | 53.0     |
| Extract nested timestamp field              | jmespath            | 0.56     | 0.61        | 0.67     | 1639.37   | 1000    | 53.0     |
| Extract nested timestamp field              | strata              | 2.04     | 2.12        | 2.20     | 476.08    | 1000    | 53.0     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 109375.85 | 0       | 54.1     |
| Extract order item prices (double wildcard) | jmespath            | 3.84     | 3.86        | 3.88     | 260.96    | 12046   | 54.1     |
| Extract order item prices (double wildcard) | strata              | 13.15    | 13.25       | 13.35    | 76.02     | 12046   | 53.8     |
| Filter users by age (numeric predicate)     | strata              | 1.02     | 1.05        | 1.09     | 955.81    | 794     | 54.1     |
| Filter users by age (numeric predicate)     | jmespath            | 1.11     | 1.17        | 1.24     | 858.90    | 794     | 54.1     |
| NDJSON first record id                      | jsonpath-ng         | 0.00     | 0.01        | 0.01     | 179055.49 | 1       | 43.6     |
| NDJSON first record id                      | jmespath            | 0.01     | 0.01        | 0.01     | 155454.25 | 1       | 43.3     |
| NDJSON first record id                      | strata              | 0.01     | 0.02        | 0.02     | 60811.31  | 1       | 43.3     |
| Recursively find all prices                 | strata              | 14.10    | 14.26       | 14.42    | 70.63     | 12046   | 54.4     |
| Recursively find all prices                 | jsonpath-ng         | 73.79    | 73.97       | 74.16    | 13.62     | 12046   | 56.3     |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.02        | 0.02     | 63948.43  | 10      | 56.3     |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.02        | 0.02     | 60659.31  | 10      | 56.3     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 40287.48  | 10      | 55.4     |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 40.96    | 41.43       | 41.90    | 218.79 | 55.5     |
| strata_cursor_reparse | 111.98   | 112.10      | 112.22   | 80.86  | 63.9     |
- Speedup (reuse vs reparse): 2.71x
