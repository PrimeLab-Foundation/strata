# Strata Benchmark Results

Generated: 2026-02-11 21:08:34

## Environment

- Commit: 85c029c4d7f2d9b0ae7d1507059a58bf4cb96a81
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
| orjson        | users.json   | 133.119  | 185.111     | 185.305  | 712.0    |
| msgspec       | users.json   | 130.315  | 191.723     | 192.253  | 704.4    |
| strata        | users.json   | 199.842  | 227.802     | 233.531  | 566.1    |
| ujson         | users.json   | 191.950  | 261.558     | 263.396  | 826.8    |
| json (stdlib) | users.json   | 256.199  | 310.576     | 311.731  | 704.4    |
| orjson        | users.ndjson | 171.969  | 207.467     | 209.887  | 577.5    |
| msgspec       | users.ndjson | 177.136  | 227.646     | 255.972  | 571.1    |
| ujson         | users.ndjson | 242.756  | 297.452     | 303.588  | 694.6    |
| strata        | users.ndjson | 241.880  | 299.454     | 302.959  | 521.3    |
| json (stdlib) | users.ndjson | 294.992  | 334.643     | 341.476  | 573.8    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.592    | 4000    |
|                                     | jmespath    | 1.603    | 4000    |
|                                     | jsonpath-ng | 34.699   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 29.160   | 789913  |
|                                     | jmespath    | 216.859  | 4000    |
|                                     | jsonpath-ng | 1057.621 | 789913  |
| $..price                            | jmespath    | 219.758  | 4000    |
|                                     | query       | 265.526  | 789913  |
|                                     | jsonpath-ng | 3741.301 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.589    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 278.432  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 127.61   | 182.98      | 189.40   | 712.2    |
| msgspec       | 134.66   | 189.04      | 191.97   | 703.6    |
| strata        | 207.88   | 227.92      | 230.04   | 565.3    |
| ujson         | 186.51   | 261.64      | 261.69   | 826.2    |
| json (stdlib) | 249.78   | 306.89      | 311.90   | 703.7    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 172.74   | 212.13      | 212.41   | 875.3    |
| msgspec       | 178.73   | 218.29      | 222.92   | 869.6    |
| strata        | 210.18   | 222.66      | 225.76   | 878.1    |
| ujson         | 245.24   | 304.55      | 305.76   | 992.2    |
| json (stdlib) | 286.60   | 329.94      | 332.77   | 872.4    |
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
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 1       | 1605.2   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 1       | 1582.2   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 1       | 1582.2   |
| Extract all user IDs                        | jmespath    | 1.74     | 1.80        | 1.85     | 4000    | 1058.9   |
| Extract all user IDs                        | strata      | 3.67     | 3.73        | 3.80     | 4000    | 1058.8   |
| Extract all user IDs                        | jsonpath-ng | 36.10    | 36.12       | 36.14    | 4000    | 1060.8   |
| Extract all user names                      | jmespath    | 1.26     | 1.27        | 1.27     | 4000    | 1582.4   |
| Extract all user names                      | strata      | 3.57     | 3.62        | 3.68     | 4000    | 1582.4   |
| Extract all user names                      | jsonpath-ng | 32.14    | 32.20       | 32.26    | 4000    | 1582.4   |
| Extract nested timestamp field              | jmespath    | 2.53     | 2.65        | 2.78     | 4000    | 1410.7   |
| Extract nested timestamp field              | strata      | 4.17     | 4.24        | 4.31     | 4000    | 1410.7   |
| Extract nested timestamp field              | jsonpath-ng | 35.07    | 35.13       | 35.19    | 4000    | 1411.8   |
| Extract order item prices (double wildcard) | jmespath    | 220.42   | 222.45      | 224.48   | 4000    | 1600.1   |
| Extract order item prices (double wildcard) | strata      | 739.49   | 744.27      | 749.05   | 789913  | 1586.4   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1067.04  | 1068.96     | 1070.89  | 789913  | 1615.0   |
| Filter users by age (numeric predicate)     | strata      | 3.17     | 3.19        | 3.22     | 3212    | 1582.4   |
| Filter users by age (numeric predicate)     | jmespath    | 4.72     | 4.78        | 4.83     | 3212    | 1582.4   |
| Recursively find all prices                 | strata      | 1041.04  | 1042.61     | 1044.17  | 789913  | 1623.2   |
| Recursively find all prices                 | jsonpath-ng | 3732.63  | 3732.97     | 3733.32  | 789913  | 1628.7   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 10      | 1630.7   |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.03     | 10      | 1613.7   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 10      | 1612.7   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 1       | 1451.6   |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 1       | 1451.6   |
| Deep path navigation                        | strata      | 0.03     | 0.03        | 0.03     | 0       | 1451.6   |
| Extract all user IDs                        | jmespath    | 2.00     | 2.08        | 2.15     | 4000    | 952.8    |
| Extract all user IDs                        | strata      | 10.55    | 10.61       | 10.66    | 0       | 952.5    |
| Extract all user IDs                        | jsonpath-ng | 32.88    | 33.11       | 33.33    | 4000    | 954.7    |
| Extract all user names                      | jmespath    | 1.96     | 1.99        | 2.01     | 4000    | 1451.7   |
| Extract all user names                      | strata      | 10.65    | 10.65       | 10.65    | 0       | 1451.7   |
| Extract all user names                      | jsonpath-ng | 32.91    | 32.96       | 33.02    | 4000    | 1451.7   |
| Extract nested timestamp field              | jmespath    | 3.26     | 3.27        | 3.28     | 4000    | 1353.0   |
| Extract nested timestamp field              | strata      | 10.57    | 10.60       | 10.62    | 0       | 1353.0   |
| Extract nested timestamp field              | jsonpath-ng | 35.52    | 35.76       | 36.00    | 4000    | 1354.1   |
| Extract order item prices (double wildcard) | strata      | 10.81    | 10.83       | 10.84    | 0       | 1354.9   |
| Extract order item prices (double wildcard) | jmespath    | 179.18   | 179.54      | 179.90   | 789913  | 1396.0   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1047.99  | 1049.67     | 1051.35  | 789913  | 1453.3   |
| Filter users by age (numeric predicate)     | strata      | 0.02     | 0.03        | 0.03     | 0       | 1451.7   |
| Filter users by age (numeric predicate)     | jmespath    | 5.08     | 5.16        | 5.24     | 3212    | 1451.8   |
| Recursively find all prices                 | strata      | 1057.12  | 1059.01     | 1060.91  | 3898    | 1491.1   |
| Recursively find all prices                 | jsonpath-ng | 3687.44  | 3699.08     | 3710.72  | 789913  | 1549.3   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 1530.3   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 0       | 1548.3   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 10      | 1529.3   |
