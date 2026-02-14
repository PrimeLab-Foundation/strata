# Strata Benchmark Results

Generated: 2026-02-14 14:46:59

## Environment

- Commit: 1a605860e0960662442cc11396164b9e31b0f22e
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
| orjson        | users.json   | 19.592   | 26.906      | 27.703   | 129.9    |
| msgspec       | users.json   | 20.154   | 28.924      | 31.069   | 129.8    |
| ujson         | users.json   | 28.701   | 40.414      | 40.637   | 148.4    |
| json (stdlib) | users.json   | 36.257   | 44.296      | 45.281   | 129.8    |
| strata        | users.json   | 43.729   | 46.050      | 46.759   | 110.0    |
| orjson        | users.ndjson | 23.999   | 24.124      | 25.293   | 152.3    |
| msgspec       | users.ndjson | 24.188   | 24.810      | 26.559   | 153.2    |
| ujson         | users.ndjson | 33.306   | 36.093      | 36.485   | 170.9    |
| json (stdlib) | users.ndjson | 40.961   | 41.611      | 42.910   | 155.7    |
| strata        | users.ndjson | 50.822   | 60.901      | 60.971   | 147.3    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.124    | 2000    |
|                                     | jmespath    | 0.505    | 2000    |
|                                     | jsonpath-ng | 7.051    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 4.229    | 100999  |
|                                     | jmespath    | 34.666   | 2000    |
|                                     | jsonpath-ng | 141.151  | 100999  |
| $..price                            | jmespath    | 35.647   | 2000    |
|                                     | query       | 38.030   | 100999  |
|                                     | jsonpath-ng | 515.420  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.162    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 41.202   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 21.68    | 28.54       | 32.95    | 229.55 | 129.4    |
| msgspec       | 20.59    | 28.67       | 29.57    | 228.49 | 130.2    |
| ujson         | 39.89    | 42.03       | 43.00    | 155.87 | 147.9    |
| json (stdlib) | 36.17    | 46.24       | 51.68    | 141.69 | 130.2    |
| strata        | 44.18    | 46.52       | 47.01    | 140.84 | 109.4    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 24.83    | 25.87       | 30.77    | 253.14 | 173.2    |
| msgspec       | 25.09    | 28.72       | 30.50    | 228.02 | 173.0    |
| ujson         | 37.15    | 40.84       | 41.19    | 160.36 | 190.7    |
| json (stdlib) | 44.90    | 46.56       | 46.77    | 140.67 | 173.9    |
| strata        | 48.36    | 51.35       | 54.31    | 127.55 | 172.0    |
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
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 549820.26 | 1       | 1565.3   |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 505607.64 | 1       | 1566.3   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 311368.65 | 1       | 1565.3   |
| Extract all user IDs                        | jmespath    | 0.77     | 0.77        | 0.77     | 8551.00   | 2000    | 553.9    |
| Extract all user IDs                        | strata      | 2.61     | 2.68        | 2.75     | 2444.16   | 2000    | 553.8    |
| Extract all user IDs                        | jsonpath-ng | 7.19     | 7.40        | 7.61     | 885.18    | 2000    | 555.0    |
| Extract all user names                      | jmespath    | 0.56     | 0.63        | 0.70     | 10412.21  | 2000    | 1856.7   |
| Extract all user names                      | strata      | 2.50     | 2.58        | 2.67     | 2537.64   | 2000    | 1856.7   |
| Extract all user names                      | jsonpath-ng | 7.08     | 7.31        | 7.53     | 896.38    | 2000    | 1856.7   |
| Extract nested timestamp field              | jmespath    | 1.31     | 1.34        | 1.37     | 4901.34   | 2000    | 986.6    |
| Extract nested timestamp field              | strata      | 2.39     | 2.58        | 2.77     | 2543.88   | 2000    | 986.5    |
| Extract nested timestamp field              | jsonpath-ng | 8.66     | 8.71        | 8.76     | 752.23    | 2000    | 987.2    |
| Extract order item prices (double wildcard) | jmespath    | 34.72    | 34.91       | 35.11    | 187.65    | 2000    | 1189.4   |
| Extract order item prices (double wildcard) | strata      | 122.84   | 124.62      | 126.39   | 52.57     | 100999  | 1187.4   |
| Extract order item prices (double wildcard) | jsonpath-ng | 136.55   | 136.61      | 136.66   | 47.96     | 100999  | 1192.4   |
| Filter users by age (numeric predicate)     | strata      | 2.06     | 2.09        | 2.12     | 3134.67   | 1606    | 1997.6   |
| Filter users by age (numeric predicate)     | jmespath    | 2.49     | 2.63        | 2.77     | 2491.98   | 1606    | 1997.6   |
| Recursively find all prices                 | strata      | 131.43   | 132.51      | 133.59   | 49.44     | 100999  | 2123.9   |
| Recursively find all prices                 | jsonpath-ng | 512.71   | 518.51      | 524.31   | 12.64     | 100999  | 2130.2   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 284082.98 | 10      | 2202.6   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 217032.37 | 10      | 2202.6   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 192342.44 | 10      | 2202.6   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 330.92   | 332.06      | 333.20   | 157.84 | 2481.0   |
| strata_cursor_reparse | 764.90   | 798.76      | 832.62   | 65.62  | 3898.5   |
- Speedup (reuse vs reparse): 2.41x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.02     | 0.02        | 0.02     | 287101.68 | 1       | 226.5    |
| Deep path navigation                        | strata              | 0.02     | 0.02        | 0.02     | 282714.79 | 1       | 226.5    |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 229309.40 | 1       | 226.5    |
| Extract all user IDs                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 310962.96 | 0       | 214.3    |
| Extract all user IDs                        | jmespath            | 0.90     | 0.93        | 0.97     | 7013.88   | 2000    | 214.3    |
| Extract all user IDs                        | strata              | 8.38     | 8.65        | 8.92     | 757.13    | 2000    | 213.0    |
| Extract all user IDs                        | strata_ndjson_fused | 29.76    | 30.21       | 30.66    | 216.81    | 2000    | 214.0    |
| Extract all user IDs                        | strata_ndjson_full  | 30.64    | 30.88       | 31.11    | 212.12    | 2000    | 213.9    |
| Extract all user names                      | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 399478.41 | 0       | 226.6    |
| Extract all user names                      | jmespath            | 0.91     | 0.93        | 0.96     | 7010.13   | 2000    | 226.6    |
| Extract all user names                      | strata              | 8.42     | 8.80        | 9.19     | 743.87    | 2000    | 226.5    |
| Extract all user names                      | strata_ndjson_fused | 30.43    | 30.62       | 30.80    | 213.94    | 2000    | 226.6    |
| Extract all user names                      | strata_ndjson_full  | 32.98    | 33.21       | 33.45    | 197.19    | 2000    | 226.6    |
| Extract nested timestamp field              | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 255381.95 | 0       | 214.6    |
| Extract nested timestamp field              | jmespath            | 1.58     | 1.59        | 1.61     | 4111.68   | 2000    | 214.6    |
| Extract nested timestamp field              | strata              | 9.32     | 9.51        | 9.70     | 688.68    | 2000    | 214.6    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 301133.19 | 0       | 226.5    |
| Extract order item prices (double wildcard) | jmespath            | 27.04    | 27.76       | 28.47    | 235.98    | 100999  | 226.5    |
| Extract order item prices (double wildcard) | strata              | 131.08   | 131.36      | 131.64   | 49.86     | 100999  | 222.2    |
| Filter users by age (numeric predicate)     | jmespath            | 2.73     | 2.83        | 2.93     | 2317.37   | 1606    | 226.6    |
| Filter users by age (numeric predicate)     | strata              | 8.21     | 8.43        | 8.66     | 776.70    | 1606    | 226.6    |
| NDJSON first record id                      | jmespath            | 0.02     | 0.02        | 0.02     | 334806.48 | 1       | 153.7    |
| NDJSON first record id                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 304634.97 | 1       | 154.0    |
| NDJSON first record id                      | strata              | 0.02     | 0.04        | 0.06     | 156799.06 | 1       | 153.7    |
| Recursively find all prices                 | strata              | 122.30   | 122.80      | 123.30   | 53.33     | 100999  | 226.6    |
| Recursively find all prices                 | jsonpath-ng         | 521.79   | 523.16      | 524.53   | 12.52     | 100999  | 231.0    |
| Slice first 10 users, extract IDs           | jmespath            | 0.03     | 0.03        | 0.03     | 219385.75 | 10      | 230.0    |
| Slice first 10 users, extract IDs           | strata              | 0.03     | 0.03        | 0.03     | 192875.19 | 10      | 230.0    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.04     | 0.04        | 0.04     | 178727.56 | 10      | 229.8    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 326.77   | 328.54      | 330.32   | 179.42 | 229.9    |
| strata_cursor_reparse | 831.13   | 834.66      | 838.19   | 70.62  | 293.8    |
- Speedup (reuse vs reparse): 2.54x
