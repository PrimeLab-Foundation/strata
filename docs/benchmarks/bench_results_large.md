# Strata Benchmark Results

Generated: 2026-02-09 19:08:43

## Environment

- Commit: d18dd88f6a1f4b1ea0013c01ecb1e2d84d0466ca
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
| orjson        | users.json   | 129.648  | 180.468     | 185.188  | 712.9    |
| msgspec       | users.json   | 133.901  | 193.737     | 197.434  | 704.4    |
| strata        | users.json   | 198.014  | 226.115     | 228.126  | 566.0    |
| ujson         | users.json   | 186.319  | 263.638     | 265.470  | 827.8    |
| json (stdlib) | users.json   | 244.012  | 297.556     | 302.619  | 704.4    |
| orjson        | users.ndjson | 175.418  | 208.977     | 211.297  | 576.6    |
| msgspec       | users.ndjson | 181.948  | 216.905     | 219.823  | 570.0    |
| strata        | users.ndjson | 236.986  | 274.382     | 275.052  | 522.5    |
| ujson         | users.ndjson | 236.547  | 303.622     | 311.229  | 693.5    |
| json (stdlib) | users.ndjson | 290.798  | 328.163     | 334.449  | 572.7    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.615    | 4000    |
|                                     | jmespath    | 1.596    | 4000    |
|                                     | jsonpath-ng | 33.041   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 28.122   | 789913  |
|                                     | jmespath    | 218.818  | 4000    |
|                                     | jsonpath-ng | 1073.846 | 789913  |
| $..price                            | jmespath    | 222.882  | 4000    |
|                                     | query       | 267.234  | 789913  |
|                                     | jsonpath-ng | 3820.962 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.587    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 284.368  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 126.28   | 180.74      | 181.72   | 712.3    |
| msgspec       | 135.88   | 191.45      | 192.56   | 703.6    |
| strata        | 197.22   | 227.30      | 228.09   | 565.4    |
| ujson         | 186.92   | 263.54      | 266.84   | 826.2    |
| json (stdlib) | 248.26   | 302.81      | 306.87   | 703.8    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 168.78   | 210.01      | 214.55   | 879.6    |
| msgspec       | 181.34   | 220.02      | 224.08   | 869.9    |
| strata        | 218.03   | 234.23      | 236.59   | 878.2    |
| ujson         | 238.82   | 305.85      | 306.30   | 992.5    |
| json (stdlib) | 306.78   | 337.31      | 337.89   | 872.7    |
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
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 1583.4   |
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 1       | 1606.4   |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1       | 1583.4   |
| Extract all user IDs                        | jmespath    | 1.78     | 1.84        | 1.91     | 4000    | 1059.3   |
| Extract all user IDs                        | strata      | 4.49     | 5.12        | 5.76     | 4000    | 1058.9   |
| Extract all user IDs                        | jsonpath-ng | 32.57    | 32.69       | 32.81    | 4000    | 1061.2   |
| Extract all user names                      | jmespath    | 1.28     | 1.37        | 1.47     | 4000    | 1583.5   |
| Extract all user names                      | strata      | 3.62     | 3.96        | 4.30     | 4000    | 1583.5   |
| Extract all user names                      | jsonpath-ng | 32.51    | 32.73       | 32.94    | 4000    | 1583.5   |
| Extract nested timestamp field              | jmespath    | 2.63     | 2.68        | 2.73     | 4000    | 1411.6   |
| Extract nested timestamp field              | strata      | 4.12     | 4.21        | 4.31     | 4000    | 1411.6   |
| Extract nested timestamp field              | jsonpath-ng | 35.44    | 36.37       | 37.30    | 4000    | 1412.7   |
| Extract order item prices (double wildcard) | jmespath    | 221.74   | 223.74      | 225.74   | 4000    | 1600.5   |
| Extract order item prices (double wildcard) | strata      | 783.96   | 801.28      | 818.59   | 789913  | 1586.8   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1110.52  | 1117.05     | 1123.58  | 789913  | 1616.2   |
| Filter users by age (numeric predicate)     | strata      | 3.01     | 3.02        | 3.03     | 3212    | 1583.6   |
| Filter users by age (numeric predicate)     | jmespath    | 4.93     | 5.17        | 5.41     | 3212    | 1583.6   |
| Recursively find all prices                 | strata      | 1083.64  | 1101.56     | 1119.48  | 789913  | 1622.4   |
| Recursively find all prices                 | jsonpath-ng | 3886.59  | 3897.45     | 3908.31  | 789913  | 1626.8   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 10      | 1612.4   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 10      | 1629.4   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 10      | 1611.4   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 1       | 1903.4   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1       | 1903.4   |
| Deep path navigation                        | strata      | 486.86   | 487.12      | 487.37   | 1       | 1920.4   |
| Extract all user IDs                        | jmespath    | 2.09     | 2.24        | 2.40     | 4000    | 1370.5   |
| Extract all user IDs                        | jsonpath-ng | 61.80    | 64.98       | 68.17    | 4000    | 1372.4   |
| Extract all user IDs                        | strata      | 489.95   | 496.86      | 503.78   | 4000    | 1370.2   |
| Extract all user names                      | jmespath    | 2.02     | 2.09        | 2.16     | 4000    | 2033.8   |
| Extract all user names                      | jsonpath-ng | 66.68    | 67.65       | 68.61    | 4000    | 2034.2   |
| Extract all user names                      | strata      | 486.73   | 495.79      | 504.84   | 4000    | 2033.8   |
| Extract nested timestamp field              | jmespath    | 3.16     | 3.24        | 3.32     | 4000    | 1512.0   |
| Extract nested timestamp field              | jsonpath-ng | 64.92    | 65.05       | 65.18    | 4000    | 1512.0   |
| Extract nested timestamp field              | strata      | 478.52   | 486.10      | 493.68   | 4000    | 1511.9   |
| Extract order item prices (double wildcard) | jmespath    | 186.07   | 186.93      | 187.80   | 789913  | 1816.3   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1049.12  | 1057.58     | 1066.04  | 789913  | 1825.7   |
| Extract order item prices (double wildcard) | strata      | 1261.10  | 1265.73     | 1270.37  | 789913  | 1805.2   |
| Filter users by age (numeric predicate)     | jmespath    | 5.26     | 5.29        | 5.32     | 3212    | 2179.8   |
| Filter users by age (numeric predicate)     | strata      | 484.05   | 491.98      | 499.91   | 3212    | 2179.8   |
| Recursively find all prices                 | strata      | 1606.50  | 1611.94     | 1617.38  | 789913  | 2218.2   |
| Recursively find all prices                 | jsonpath-ng | 3686.34  | 3753.15     | 3819.96  | 789913  | 1996.5   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 10      | 2095.0   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 10      | 2095.0   |
| Slice first 10 users, extract IDs           | strata      | 468.96   | 470.07      | 471.18   | 10      | 2105.0   |
