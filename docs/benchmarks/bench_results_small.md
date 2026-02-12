# Strata Benchmark Results

Generated: 2026-02-12 22:20:16

## Environment

- Commit: 63b52a91dc763c26eb4637cbc9ea21a3e346893a
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
| msgspec       | users.json   | 4.115    | 5.735       | 5.851    | 397.9    |
| orjson        | users.json   | 4.050    | 6.019       | 6.062    | 394.8    |
| ujson         | users.json   | 5.364    | 7.677       | 8.226    | 396.9    |
| json (stdlib) | users.json   | 6.772    | 8.435       | 8.647    | 398.9    |
| strata        | users.json   | 19.717   | 19.819      | 24.456   | 390.4    |
| orjson        | users.ndjson | 3.665    | 5.202       | 5.602    | 402.0    |
| msgspec       | users.ndjson | 3.544    | 5.401       | 5.892    | 404.0    |
| ujson         | users.ndjson | 4.984    | 7.497       | 7.500    | 404.0    |
| json (stdlib) | users.ndjson | 6.598    | 8.257       | 8.397    | 404.0    |
| strata        | users.ndjson | 9.492    | 11.100      | 11.745   | 400.1    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.047    | 1000    |
|                                     | jmespath    | 0.265    | 1000    |
|                                     | jsonpath-ng | 2.597    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.689    | 12046   |
|                                     | jmespath    | 7.353    | 1000    |
|                                     | jsonpath-ng | 22.465   | 12046   |
| $..price                            | query       | 6.681    | 12046   |
|                                     | jmespath    | 7.263    | 1000    |
|                                     | jsonpath-ng | 83.345   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.050    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 7.249    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 3.95     | 5.19        | 5.82     | 194.39 | 341.0    |
| msgspec       | 3.92     | 5.90        | 6.17     | 170.94 | 341.0    |
| ujson         | 5.33     | 7.89        | 8.17     | 127.72 | 344.1    |
| json (stdlib) | 6.89     | 8.13        | 8.80     | 124.03 | 343.1    |
| strata        | 15.33    | 17.62       | 18.19    | 57.23  | 337.1    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 3.46     | 4.52        | 5.32     | 222.81 | 36.9     |
| orjson        | 3.39     | 5.09        | 5.26     | 198.00 | 36.8     |
| ujson         | 4.88     | 7.07        | 7.26     | 142.51 | 39.9     |
| strata        | 7.64     | 8.44        | 8.49     | 119.30 | 37.3     |
| json (stdlib) | 6.29     | 8.49        | 8.77     | 118.67 | 38.9     |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s     | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 57887.53 | 1       | 258.7    |
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.03     | 47772.83 | 1       | 258.7    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.03     | 43874.75 | 1       | 258.7    |
| Extract all user IDs                        | jmespath    | 0.34     | 0.39        | 0.45     | 2560.50  | 1000    | 98.3     |
| Extract all user IDs                        | strata      | 1.49     | 1.54        | 1.58     | 656.58   | 1000    | 98.2     |
| Extract all user IDs                        | jsonpath-ng | 2.37     | 2.49        | 2.60     | 405.08   | 1000    | 99.0     |
| Extract all user names                      | jmespath    | 0.25     | 0.26        | 0.27     | 3862.82  | 1000    | 291.2    |
| Extract all user names                      | strata      | 1.70     | 1.77        | 1.84     | 568.47   | 1000    | 291.2    |
| Extract all user names                      | jsonpath-ng | 2.69     | 2.77        | 2.85     | 364.30   | 1000    | 291.2    |
| Extract nested timestamp field              | jmespath    | 0.68     | 0.71        | 0.73     | 1425.22  | 1000    | 159.8    |
| Extract nested timestamp field              | strata      | 1.70     | 1.94        | 2.18     | 519.06   | 1000    | 159.8    |
| Extract nested timestamp field              | jsonpath-ng | 3.03     | 3.06        | 3.09     | 329.09   | 1000    | 160.2    |
| Extract order item prices (double wildcard) | jmespath    | 6.65     | 6.78        | 6.92     | 148.62   | 1000    | 212.0    |
| Extract order item prices (double wildcard) | strata      | 16.53    | 16.59       | 16.64    | 60.79    | 12046   | 211.5    |
| Extract order item prices (double wildcard) | jsonpath-ng | 21.24    | 21.53       | 21.82    | 46.83    | 12046   | 216.2    |
| Filter users by age (numeric predicate)     | strata      | 1.22     | 1.23        | 1.25     | 818.72   | 794     | 321.4    |
| Filter users by age (numeric predicate)     | jmespath    | 1.36     | 1.39        | 1.42     | 724.70   | 794     | 321.4    |
| Recursively find all prices                 | strata      | 18.70    | 18.89       | 19.07    | 53.38    | 12046   | 352.2    |
| Recursively find all prices                 | jsonpath-ng | 82.85    | 82.96       | 83.06    | 12.15    | 12046   | 353.9    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 63844.35 | 10      | 382.2    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.03        | 0.03     | 35093.40 | 10      | 382.2    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.04     | 0.04        | 0.04     | 24627.74 | 10      | 382.2    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 55.63    | 56.20       | 56.78    | 143.51 | 458.9    |
| strata_cursor_reparse | 149.13   | 152.44      | 155.75   | 52.91  | 932.2    |
- Speedup (reuse vs reparse): 2.71x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s     | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | -------- | ------- | -------- |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.02     | 69863.48 | 1       | 54.8     |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 55568.94 | 1       | 54.8     |
| Deep path navigation                        | jmespath            | 0.03     | 0.03        | 0.03     | 36295.03 | 1       | 54.8     |
| Extract all user IDs                        | strata              | 0.21     | 0.26        | 0.30     | 3938.49  | 0       | 53.0     |
| Extract all user IDs                        | jmespath            | 0.41     | 0.43        | 0.44     | 2366.95  | 1000    | 53.1     |
| Extract all user IDs                        | jsonpath-ng         | 2.67     | 2.76        | 2.84     | 365.22   | 1000    | 53.1     |
| Extract all user IDs                        | strata_ndjson_fused | 5.48     | 5.48        | 5.49     | 183.72   | 0       | 53.0     |
| Extract all user IDs                        | strata_ndjson_full  | 7.74     | 7.78        | 7.82     | 129.48   | 0       | 53.0     |
| Extract all user names                      | strata              | 0.26     | 0.26        | 0.27     | 3837.82  | 0       | 54.9     |
| Extract all user names                      | jmespath            | 0.48     | 0.64        | 0.80     | 1576.29  | 1000    | 54.9     |
| Extract all user names                      | jsonpath-ng         | 3.07     | 3.23        | 3.39     | 311.68   | 1000    | 54.9     |
| Extract all user names                      | strata_ndjson_fused | 5.36     | 5.44        | 5.52     | 185.22   | 0       | 54.9     |
| Extract all user names                      | strata_ndjson_full  | 7.72     | 7.72        | 7.72     | 130.42   | 0       | 54.9     |
| Extract nested timestamp field              | strata              | 0.22     | 0.22        | 0.23     | 4478.46  | 0       | 53.2     |
| Extract nested timestamp field              | jmespath            | 0.86     | 0.87        | 0.87     | 1161.08  | 1000    | 53.2     |
| Extract nested timestamp field              | jsonpath-ng         | 3.12     | 3.30        | 3.48     | 305.22   | 1000    | 53.3     |
| Extract order item prices (double wildcard) | strata              | 0.24     | 0.24        | 0.24     | 4172.70  | 0       | 53.4     |
| Extract order item prices (double wildcard) | jmespath            | 4.59     | 4.67        | 4.74     | 215.85   | 12046   | 53.8     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 22.75    | 22.84       | 22.94    | 44.09    | 12046   | 56.9     |
| Filter users by age (numeric predicate)     | strata              | 0.02     | 0.02        | 0.02     | 54380.83 | 0       | 54.9     |
| Filter users by age (numeric predicate)     | jmespath            | 1.59     | 1.61        | 1.62     | 627.46   | 794     | 54.9     |
| NDJSON root field (id)                      | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 67426.76 | 0       | 44.7     |
| NDJSON root field (id)                      | strata              | 3.38     | 3.42        | 3.45     | 294.79   | 1000    | 44.3     |
| NDJSON root field (id)                      | strata_ndjson_fused | 4.15     | 4.20        | 4.26     | 239.75   | 1000    | 44.7     |
| NDJSON root field (id)                      | strata_ndjson_full  | 11.11    | 11.35       | 11.58    | 88.77    | 1000    | 44.6     |
| Recursively find all prices                 | strata              | 18.67    | 18.69       | 18.72    | 53.88    | 878     | 55.0     |
| Recursively find all prices                 | jsonpath-ng         | 82.79    | 83.30       | 83.80    | 12.09    | 12046   | 56.8     |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.02     | 57553.54 | 10      | 56.8     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.03     | 0.04        | 0.04     | 28640.52 | 10      | 56.0     |
| Slice first 10 users, extract IDs           | strata              | 0.04     | 0.04        | 0.05     | 23491.25 | 10      | 56.8     |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 32.67    | 32.88       | 33.09    | 275.70 | 56.1     |
| strata_cursor_reparse | 118.93   | 119.01      | 119.09   | 76.17  | 64.4     |
- Speedup (reuse vs reparse): 3.62x
