# Strata Benchmark Results

Generated: 2026-02-12 00:36:47

## Environment

- Commit: 271ffab0b72d6e1fdcb5aeb8844621760c1bbb21
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/large/users.json, benchmarks/data/generated/large/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users.json   | 128.223  | 184.497     | 196.356  | 14847.2  |
| msgspec       | users.json   | 142.726  | 191.470     | 193.262  | 14567.2  |
| ujson         | users.json   | 196.872  | 265.816     | 284.005  | 14689.6  |
| json (stdlib) | users.json   | 262.725  | 330.744     | 341.741  | 14609.7  |
| strata        | users.json   | 1558.432 | 1791.257    | 2465.176 | 14861.5  |
| orjson        | users.ndjson | 172.565  | 211.398     | 211.875  | 13732.4  |
| msgspec       | users.ndjson | 178.312  | 219.205     | 241.791  | 13530.9  |
| ujson         | users.ndjson | 239.640  | 299.680     | 316.068  | 13654.4  |
| json (stdlib) | users.ndjson | 298.488  | 336.354     | 343.033  | 13533.7  |
| strata        | users.ndjson | 349.426  | 407.055     | 426.772  | 13738.2  |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.623    | 4000    |
|                                     | jmespath    | 1.528    | 4000    |
|                                     | jsonpath-ng | 32.608   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 29.631   | 789913  |
|                                     | jmespath    | 226.192  | 4000    |
|                                     | jsonpath-ng | 1125.426 | 789913  |
| $..price                            | jmespath    | 221.811  | 4000    |
|                                     | query       | 267.987  | 789913  |
|                                     | jsonpath-ng | 3889.924 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.569    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 282.506  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 127.11   | 180.27      | 193.60   | 15317.1  |
| msgspec       | 136.37   | 194.22      | 213.18   | 15023.2  |
| ujson         | 185.57   | 268.60      | 270.44   | 14889.6  |
| json (stdlib) | 240.84   | 298.46      | 306.83   | 14811.0  |
| strata        | 1556.64  | 1652.84     | 1682.91  | 15293.4  |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 164.07   | 202.55      | 207.15   | 1403.7   |
| msgspec       | 173.99   | 209.03      | 210.05   | 1398.0   |
| strata        | 206.80   | 219.28      | 222.89   | 1406.4   |
| ujson         | 230.35   | 287.52      | 292.19   | 1520.6   |
| json (stdlib) | 280.77   | 321.51      | 323.84   | 1400.8   |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 1       | 14676.9  |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 1       | 14714.5  |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 1       | 14676.9  |
| Extract all user IDs                        | jmespath    | 1.71     | 1.75        | 1.78     | 4000    | 5557.6   |
| Extract all user IDs                        | strata      | 4.99     | 5.01        | 5.02     | 4000    | 5557.3   |
| Extract all user IDs                        | jsonpath-ng | 31.70    | 31.82       | 31.94    | 4000    | 5559.7   |
| Extract all user names                      | jmespath    | 1.24     | 1.25        | 1.27     | 4000    | 14748.4  |
| Extract all user names                      | strata      | 4.47     | 4.50        | 4.53     | 4000    | 14762.1  |
| Extract all user names                      | jsonpath-ng | 31.01    | 31.11       | 31.20    | 4000    | 14748.5  |
| Extract nested timestamp field              | jmespath    | 2.54     | 2.55        | 2.56     | 4000    | 10420.2  |
| Extract nested timestamp field              | strata      | 5.63     | 5.65        | 5.66     | 4000    | 10420.1  |
| Extract nested timestamp field              | jsonpath-ng | 34.31    | 34.32       | 34.34    | 4000    | 10421.4  |
| Extract order item prices (double wildcard) | jmespath    | 219.41   | 220.35      | 221.29   | 4000    | 13741.5  |
| Extract order item prices (double wildcard) | strata      | 965.78   | 968.93      | 972.08   | 789913  | 13727.7  |
| Extract order item prices (double wildcard) | jsonpath-ng | 1058.38  | 1063.47     | 1068.55  | 789913  | 13746.2  |
| Filter users by age (numeric predicate)     | strata      | 4.48     | 4.55        | 4.61     | 3212    | 14971.4  |
| Filter users by age (numeric predicate)     | jmespath    | 4.72     | 4.72        | 4.72     | 3212    | 14971.5  |
| Recursively find all prices                 | strata      | 1196.64  | 1220.65     | 1244.66  | 789913  | 14668.6  |
| Recursively find all prices                 | jsonpath-ng | 3729.39  | 3731.87     | 3734.34  | 789913  | 14316.9  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 14515.8  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 10      | 14514.8  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.04     | 10      | 14532.8  |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.02     | 0.02        | 0.03     | 1       | 2531.1   |
| Deep path navigation                        | strata              | 0.02     | 0.03        | 0.03     | 0       | 2531.1   |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 1       | 2531.1   |
| Extract all user IDs                        | jmespath            | 1.98     | 1.98        | 1.98     | 4000    | 2437.9   |
| Extract all user IDs                        | strata              | 11.13    | 11.24       | 11.36    | 0       | 2435.5   |
| Extract all user IDs                        | jsonpath-ng         | 32.10    | 32.62       | 33.13    | 4000    | 2437.9   |
| Extract all user IDs                        | strata_ndjson_full  | 45.83    | 46.09       | 46.35    | 0       | 2437.5   |
| Extract all user IDs                        | strata_ndjson_fused | 47.01    | 49.21       | 51.40    | 0       | 2437.9   |
| Extract all user names                      | jmespath            | 1.93     | 1.93        | 1.93     | 4000    | 2537.7   |
| Extract all user names                      | strata              | 11.50    | 11.60       | 11.70    | 0       | 2536.6   |
| Extract all user names                      | jsonpath-ng         | 32.04    | 33.51       | 34.98    | 4000    | 2537.7   |
| Extract all user names                      | strata_ndjson_fused | 45.05    | 45.36       | 45.68    | 0       | 2537.7   |
| Extract all user names                      | strata_ndjson_full  | 45.75    | 46.92       | 48.08    | 0       | 2537.4   |
| Extract nested timestamp field              | jmespath            | 3.08     | 3.11        | 3.14     | 4000    | 2455.2   |
| Extract nested timestamp field              | strata              | 11.57    | 11.58       | 11.60    | 0       | 2455.2   |
| Extract nested timestamp field              | jsonpath-ng         | 34.51    | 34.52       | 34.53    | 4000    | 2455.3   |
| Extract order item prices (double wildcard) | strata              | 11.28    | 11.37       | 11.45    | 0       | 2461.8   |
| Extract order item prices (double wildcard) | jmespath            | 177.01   | 177.13      | 177.24   | 789913  | 2495.3   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 1045.78  | 1070.11     | 1094.44  | 789913  | 2500.5   |
| Filter users by age (numeric predicate)     | strata              | 0.03     | 0.03        | 0.03     | 0       | 2539.8   |
| Filter users by age (numeric predicate)     | jmespath            | 5.14     | 5.15        | 5.15     | 3212    | 2539.8   |
| NDJSON root field (id)                      | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 0       | 1484.6   |
| NDJSON root field (id)                      | strata              | 32.63    | 32.80       | 32.97    | 4000    | 1471.2   |
| NDJSON root field (id)                      | strata_ndjson_fused | 78.92    | 79.08       | 79.23    | 4000    | 1484.6   |
| NDJSON root field (id)                      | strata_ndjson_full  | 78.86    | 80.81       | 82.76    | 4000    | 1483.8   |
| Recursively find all prices                 | strata              | 1233.55  | 1236.86     | 1240.17  | 3898    | 2593.5   |
| Recursively find all prices                 | jsonpath-ng         | 3892.70  | 3906.41     | 3920.13  | 789913  | 2589.2   |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.03     | 10      | 2607.8   |
| Slice first 10 users, extract IDs           | strata              | 0.03     | 0.03        | 0.03     | 0       | 2624.8   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.03     | 0.04        | 0.04     | 10      | 2606.8   |
