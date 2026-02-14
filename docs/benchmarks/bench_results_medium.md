# Strata Benchmark Results

Generated: 2026-02-14 17:43:58

## Environment

- Commit: 479cf62079eea032a9baeadd1f59e2c2c2375686
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
| orjson        | users.json   | 19.491   | 26.977      | 29.315   | 130.7    |
| msgspec       | users.json   | 20.643   | 29.177      | 30.303   | 130.5    |
| ujson         | users.json   | 30.075   | 40.586      | 40.791   | 149.1    |
| json (stdlib) | users.json   | 36.137   | 44.664      | 45.974   | 130.5    |
| strata        | users.json   | 44.935   | 47.733      | 47.942   | 110.7    |
| orjson        | users.ndjson | 22.876   | 23.988      | 25.345   | 152.0    |
| msgspec       | users.ndjson | 24.189   | 25.601      | 26.593   | 152.9    |
| ujson         | users.ndjson | 33.364   | 36.093      | 36.516   | 170.6    |
| json (stdlib) | users.ndjson | 40.919   | 42.694      | 43.299   | 154.7    |
| strata        | users.ndjson | 52.694   | 61.405      | 63.249   | 147.0    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.123    | 2000    |
|                                     | jmespath    | 0.508    | 2000    |
|                                     | jsonpath-ng | 6.497    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 3.981    | 100999  |
|                                     | jmespath    | 34.528   | 2000    |
|                                     | jsonpath-ng | 137.703  | 100999  |
| $..price                            | jmespath    | 34.521   | 2000    |
|                                     | query       | 39.079   | 100999  |
|                                     | jsonpath-ng | 507.592  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.152    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 41.625   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 19.58    | 27.36       | 28.74    | 239.50 | 129.8    |
| msgspec       | 21.27    | 29.49       | 30.45    | 222.14 | 130.5    |
| ujson         | 28.96    | 39.76       | 41.07    | 164.80 | 148.3    |
| json (stdlib) | 35.90    | 44.66       | 45.71    | 146.69 | 130.6    |
| strata        | 45.51    | 48.37       | 48.93    | 135.44 | 109.9    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 24.47    | 26.31       | 26.39    | 248.95 | 180.0    |
| msgspec       | 25.05    | 26.57       | 26.88    | 246.49 | 179.8    |
| ujson         | 35.97    | 39.41       | 40.39    | 166.20 | 197.7    |
| json (stdlib) | 42.73    | 43.17       | 44.84    | 151.71 | 180.8    |
| strata        | 48.23    | 50.34       | 52.32    | 130.11 | 178.8    |
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
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 457087.35 | 1       | 1399.3   |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 451823.21 | 1       | 1400.3   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 341454.79 | 1       | 1399.3   |
| Extract all user IDs                        | jmespath    | 0.74     | 0.75        | 0.75     | 8770.63   | 2000    | 554.7    |
| Extract all user IDs                        | strata      | 2.82     | 2.93        | 3.04     | 2235.33   | 2000    | 554.5    |
| Extract all user IDs                        | jsonpath-ng | 8.22     | 8.40        | 8.59     | 779.56    | 2000    | 555.8    |
| Extract all user names                      | jmespath    | 0.49     | 0.51        | 0.53     | 12844.81  | 2000    | 1727.0   |
| Extract all user names                      | strata      | 2.69     | 2.73        | 2.76     | 2402.21   | 2000    | 1727.0   |
| Extract all user names                      | jsonpath-ng | 6.84     | 7.03        | 7.23     | 931.44    | 2000    | 1727.0   |
| Extract nested timestamp field              | jmespath    | 1.41     | 1.42        | 1.44     | 4599.81   | 2000    | 987.4    |
| Extract nested timestamp field              | strata      | 3.30     | 3.50        | 3.71     | 1870.76   | 2000    | 987.4    |
| Extract nested timestamp field              | jsonpath-ng | 8.38     | 8.83        | 9.28     | 741.83    | 2000    | 988.0    |
| Extract order item prices (double wildcard) | jmespath    | 36.54    | 36.90       | 37.25    | 177.57    | 2000    | 1243.9   |
| Extract order item prices (double wildcard) | strata      | 124.65   | 126.35      | 128.05   | 51.85     | 100999  | 1241.9   |
| Extract order item prices (double wildcard) | jsonpath-ng | 139.52   | 141.62      | 143.72   | 46.26     | 100999  | 1247.6   |
| Filter users by age (numeric predicate)     | strata      | 2.39     | 2.41        | 2.44     | 2716.84   | 1606    | 1801.5   |
| Filter users by age (numeric predicate)     | jmespath    | 2.48     | 2.57        | 2.65     | 2553.98   | 1606    | 1801.5   |
| Recursively find all prices                 | strata      | 135.79   | 136.09      | 136.39   | 48.14     | 100999  | 1927.8   |
| Recursively find all prices                 | jsonpath-ng | 521.69   | 525.23      | 528.76   | 12.47     | 100999  | 1934.5   |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 343315.67 | 10      | 2048.0   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 295006.95 | 10      | 2048.0   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.03     | 266506.60 | 10      | 2048.0   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 319.48   | 325.94      | 332.40   | 160.80 | 2407.1   |
| strata_cursor_reparse | 769.23   | 785.95      | 802.66   | 66.69  | 4022.1   |
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
| Deep path navigation                        | strata              | 0.02     | 0.02        | 0.02     | 310042.73 | 1       | 225.4    |
| Deep path navigation                        | jmespath            | 0.02     | 0.02        | 0.03     | 296585.93 | 1       | 225.4    |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 289225.35 | 1       | 225.4    |
| Extract all user IDs                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 290559.60 | 0       | 214.1    |
| Extract all user IDs                        | jmespath            | 0.89     | 0.93        | 0.96     | 7065.11   | 2000    | 214.1    |
| Extract all user IDs                        | strata              | 8.36     | 8.95        | 9.54     | 731.71    | 2000    | 212.9    |
| Extract all user IDs                        | strata_ndjson_fused | 29.73    | 29.99       | 30.25    | 218.37    | 2000    | 213.9    |
| Extract all user IDs                        | strata_ndjson_full  | 30.12    | 30.48       | 30.85    | 214.86    | 2000    | 213.8    |
| Extract all user names                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 328855.73 | 0       | 225.5    |
| Extract all user names                      | jmespath            | 0.78     | 0.80        | 0.83     | 8143.17   | 2000    | 225.5    |
| Extract all user names                      | strata              | 8.31     | 8.49        | 8.67     | 771.57    | 2000    | 225.4    |
| Extract all user names                      | strata_ndjson_fused | 29.82    | 29.90       | 29.99    | 219.03    | 2000    | 225.5    |
| Extract all user names                      | strata_ndjson_full  | 30.24    | 30.32       | 30.41    | 215.99    | 2000    | 225.5    |
| Extract nested timestamp field              | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 331275.46 | 0       | 214.5    |
| Extract nested timestamp field              | jmespath            | 1.59     | 1.61        | 1.62     | 4078.82   | 2000    | 214.5    |
| Extract nested timestamp field              | strata              | 8.43     | 8.52        | 8.61     | 768.81    | 2000    | 214.4    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 307914.67 | 0       | 225.4    |
| Extract order item prices (double wildcard) | jmespath            | 27.21    | 27.49       | 27.77    | 238.22    | 100999  | 225.4    |
| Extract order item prices (double wildcard) | strata              | 127.86   | 128.74      | 129.62   | 50.88     | 100999  | 222.0    |
| Filter users by age (numeric predicate)     | jmespath            | 2.69     | 2.69        | 2.70     | 2430.71   | 1606    | 225.5    |
| Filter users by age (numeric predicate)     | strata              | 8.54     | 8.69        | 8.84     | 753.82    | 1606    | 225.5    |
| NDJSON first record id                      | jmespath            | 0.01     | 0.02        | 0.02     | 429485.45 | 1       | 153.8    |
| NDJSON first record id                      | jsonpath-ng         | 0.02     | 0.02        | 0.03     | 264408.12 | 1       | 154.1    |
| NDJSON first record id                      | strata              | 0.02     | 0.03        | 0.03     | 242768.53 | 1       | 153.8    |
| Recursively find all prices                 | strata              | 136.12   | 136.45      | 136.77   | 48.00     | 100999  | 225.6    |
| Recursively find all prices                 | jsonpath-ng         | 514.26   | 518.97      | 523.69   | 12.62     | 100999  | 232.6    |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.02     | 289749.85 | 10      | 231.6    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.02        | 0.03     | 284507.77 | 10      | 230.4    |
| Slice first 10 users, extract IDs           | strata              | 0.03     | 0.03        | 0.03     | 233916.10 | 10      | 231.6    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 296.81   | 297.42      | 298.03   | 198.20 | 230.5    |
| strata_cursor_reparse | 798.07   | 801.20      | 804.32   | 73.57  | 294.3    |
- Speedup (reuse vs reparse): 2.69x
