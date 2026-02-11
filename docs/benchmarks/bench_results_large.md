# Strata Benchmark Results

Generated: 2026-02-11 11:13:13

## Environment

- Commit: af3826e258bad9e6738f3a70f573dcc93723b4e5
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
| orjson        | users.json   | 131.417  | 180.106     | 187.579  | 711.1    |
| msgspec       | users.json   | 140.538  | 193.950     | 209.429  | 704.5    |
| strata        | users.json   | 184.630  | 209.820     | 211.022  | 566.2    |
| ujson         | users.json   | 185.487  | 271.580     | 276.344  | 826.9    |
| json (stdlib) | users.json   | 237.063  | 292.722     | 308.196  | 704.5    |
| orjson        | users.ndjson | 175.792  | 206.659     | 210.179  | 597.4    |
| msgspec       | users.ndjson | 179.167  | 220.301     | 220.788  | 590.9    |
| strata        | users.ndjson | 229.621  | 257.045     | 267.008  | 543.2    |
| ujson         | users.ndjson | 244.126  | 297.030     | 299.523  | 714.4    |
| json (stdlib) | users.ndjson | 286.817  | 325.855     | 327.346  | 593.7    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.607    | 4000    |
|                                     | jmespath    | 1.659    | 4000    |
|                                     | jsonpath-ng | 32.240   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 27.369   | 789913  |
|                                     | jmespath    | 210.751  | 4000    |
|                                     | jsonpath-ng | 1011.805 | 789913  |
| $..price                            | jmespath    | 209.291  | 4000    |
|                                     | query       | 255.171  | 789913  |
|                                     | jsonpath-ng | 3595.852 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.619    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 275.190  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 121.97   | 173.29      | 180.78   | 713.3    |
| msgspec       | 137.25   | 188.43      | 191.01   | 703.7    |
| strata        | 187.74   | 214.70      | 215.52   | 565.4    |
| ujson         | 185.52   | 259.91      | 264.83   | 826.2    |
| json (stdlib) | 240.43   | 294.10      | 298.27   | 703.7    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 169.73   | 205.45      | 207.22   | 877.9    |
| msgspec       | 178.59   | 221.14      | 227.97   | 871.2    |
| strata        | 211.19   | 223.41      | 225.45   | 880.7    |
| ujson         | 239.80   | 295.02      | 309.31   | 993.8    |
| json (stdlib) | 285.54   | 335.04      | 344.43   | 874.0    |
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
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 1       | 1582.9   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 1       | 1605.9   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 1       | 1582.9   |
| Extract all user IDs                        | jmespath    | 1.74     | 1.75        | 1.76     | 4000    | 1059.5   |
| Extract all user IDs                        | strata      | 3.64     | 3.93        | 4.23     | 4000    | 1059.2   |
| Extract all user IDs                        | jsonpath-ng | 33.82    | 34.68       | 35.54    | 4000    | 1061.4   |
| Extract all user names                      | jmespath    | 1.27     | 1.32        | 1.37     | 4000    | 1583.0   |
| Extract all user names                      | strata      | 3.67     | 3.76        | 3.85     | 4000    | 1583.0   |
| Extract all user names                      | jsonpath-ng | 32.35    | 34.17       | 35.98    | 4000    | 1583.0   |
| Extract nested timestamp field              | jmespath    | 2.59     | 2.62        | 2.65     | 4000    | 1411.5   |
| Extract nested timestamp field              | strata      | 4.05     | 4.20        | 4.34     | 4000    | 1411.5   |
| Extract nested timestamp field              | jsonpath-ng | 34.66    | 35.46       | 36.26    | 4000    | 1412.6   |
| Extract order item prices (double wildcard) | jmespath    | 226.09   | 226.62      | 227.15   | 4000    | 1600.4   |
| Extract order item prices (double wildcard) | strata      | 744.14   | 747.55      | 750.96   | 789913  | 1586.7   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1089.22  | 1091.31     | 1093.40  | 789913  | 1615.7   |
| Filter users by age (numeric predicate)     | strata      | 3.05     | 3.18        | 3.30     | 3212    | 1583.1   |
| Filter users by age (numeric predicate)     | jmespath    | 4.61     | 4.64        | 4.68     | 3212    | 1583.1   |
| Recursively find all prices                 | strata      | 1031.06  | 1033.38     | 1035.69  | 789913  | 1621.8   |
| Recursively find all prices                 | jsonpath-ng | 3810.46  | 3866.43     | 3922.40  | 789913  | 1626.2   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 1612.2   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 10      | 1610.8   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 10      | 1629.2   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 1       | 1452.7   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 1       | 1452.7   |
| Deep path navigation                        | strata      | 0.03     | 0.03        | 0.03     | 0       | 1452.7   |
| Extract all user IDs                        | jmespath    | 2.29     | 2.37        | 2.46     | 4000    | 952.9    |
| Extract all user IDs                        | strata      | 11.20    | 11.62       | 12.05    | 0       | 952.6    |
| Extract all user IDs                        | jsonpath-ng | 33.15    | 33.15       | 33.15    | 4000    | 954.8    |
| Extract all user names                      | jmespath    | 2.10     | 2.21        | 2.32     | 4000    | 1452.8   |
| Extract all user names                      | strata      | 10.60    | 10.87       | 11.14    | 0       | 1452.8   |
| Extract all user names                      | jsonpath-ng | 33.67    | 34.42       | 35.16    | 4000    | 1452.8   |
| Extract nested timestamp field              | jmespath    | 3.12     | 3.15        | 3.18     | 4000    | 1353.5   |
| Extract nested timestamp field              | strata      | 10.38    | 10.60       | 10.82    | 0       | 1353.5   |
| Extract nested timestamp field              | jsonpath-ng | 35.72    | 36.44       | 37.16    | 4000    | 1354.5   |
| Extract order item prices (double wildcard) | strata      | 10.68    | 11.08       | 11.49    | 0       | 1354.9   |
| Extract order item prices (double wildcard) | jmespath    | 185.56   | 189.06      | 192.56   | 789913  | 1395.2   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1081.83  | 1092.89     | 1103.94  | 789913  | 1454.4   |
| Filter users by age (numeric predicate)     | strata      | 0.03     | 0.03        | 0.03     | 0       | 1452.8   |
| Filter users by age (numeric predicate)     | jmespath    | 5.40     | 5.41        | 5.41     | 3212    | 1452.8   |
| Recursively find all prices                 | strata      | 1076.28  | 1098.69     | 1121.11  | 3898    | 1487.9   |
| Recursively find all prices                 | jsonpath-ng | 3794.46  | 3808.69     | 3822.92  | 789913  | 1544.8   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 1527.9   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 0       | 1545.9   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.04     | 0.04        | 0.05     | 10      | 1526.9   |
