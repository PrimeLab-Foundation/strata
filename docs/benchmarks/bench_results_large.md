# Strata Benchmark Results

Generated: 2026-02-10 00:00:20

## Environment

- Commit: becfa02e50f22f82704927d5188cb058570b2617
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
| msgspec       | users.json   | 134.804  | 189.635     | 197.717  | 704.3    |
| orjson        | users.json   | 133.982  | 190.090     | 192.937  | 711.9    |
| strata        | users.json   | 197.530  | 231.699     | 243.241  | 566.0    |
| ujson         | users.json   | 203.784  | 260.275     | 294.366  | 827.7    |
| json (stdlib) | users.json   | 256.290  | 312.101     | 316.054  | 704.3    |
| orjson        | users.ndjson | 169.802  | 209.278     | 209.741  | 573.1    |
| msgspec       | users.ndjson | 176.463  | 215.378     | 220.405  | 569.5    |
| strata        | users.ndjson | 240.065  | 267.969     | 268.124  | 522.0    |
| ujson         | users.ndjson | 241.671  | 299.294     | 301.949  | 693.0    |
| json (stdlib) | users.ndjson | 288.157  | 332.433     | 341.750  | 572.2    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.598    | 4000    |
|                                     | jmespath    | 1.638    | 4000    |
|                                     | jsonpath-ng | 35.459   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 28.504   | 789913  |
|                                     | jmespath    | 218.348  | 4000    |
|                                     | jsonpath-ng | 1057.492 | 789913  |
| $..price                            | jmespath    | 216.066  | 4000    |
|                                     | query       | 262.280  | 789913  |
|                                     | jsonpath-ng | 3742.582 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.572    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 280.037  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 129.90   | 178.37      | 178.39   | 712.3    |
| msgspec       | 140.88   | 188.41      | 202.10   | 703.6    |
| strata        | 199.44   | 222.08      | 222.94   | 565.4    |
| ujson         | 185.87   | 265.68      | 267.97   | 826.2    |
| json (stdlib) | 249.91   | 297.33      | 299.80   | 703.8    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 169.27   | 210.77      | 215.68   | 879.5    |
| msgspec       | 176.04   | 216.04      | 218.35   | 869.8    |
| strata        | 212.88   | 228.96      | 229.19   | 878.2    |
| ujson         | 240.63   | 305.30      | 307.30   | 992.4    |
| json (stdlib) | 285.50   | 326.95      | 333.03   | 872.6    |
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
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 1       | 1605.6   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 1       | 1582.6   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1       | 1582.6   |
| Extract all user IDs                        | jmespath    | 1.66     | 1.71        | 1.75     | 4000    | 1058.9   |
| Extract all user IDs                        | strata      | 3.55     | 3.59        | 3.63     | 4000    | 1058.6   |
| Extract all user IDs                        | jsonpath-ng | 31.80    | 35.29       | 38.78    | 4000    | 1061.2   |
| Extract all user names                      | jmespath    | 1.31     | 1.32        | 1.32     | 4000    | 1582.7   |
| Extract all user names                      | strata      | 3.63     | 3.72        | 3.80     | 4000    | 1582.7   |
| Extract all user names                      | jsonpath-ng | 31.38    | 31.82       | 32.27    | 4000    | 1582.7   |
| Extract nested timestamp field              | jmespath    | 2.52     | 2.52        | 2.52     | 4000    | 1411.1   |
| Extract nested timestamp field              | strata      | 4.04     | 4.08        | 4.12     | 4000    | 1411.0   |
| Extract nested timestamp field              | jsonpath-ng | 34.13    | 34.43       | 34.73    | 4000    | 1412.2   |
| Extract order item prices (double wildcard) | jmespath    | 218.76   | 221.27      | 223.77   | 4000    | 1600.5   |
| Extract order item prices (double wildcard) | strata      | 723.60   | 726.19      | 728.77   | 789913  | 1586.8   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1084.86  | 1087.61     | 1090.35  | 789913  | 1615.4   |
| Filter users by age (numeric predicate)     | strata      | 2.69     | 2.96        | 3.23     | 3212    | 1582.8   |
| Filter users by age (numeric predicate)     | jmespath    | 4.70     | 4.73        | 4.75     | 3212    | 1582.8   |
| Recursively find all prices                 | strata      | 1024.29  | 1034.66     | 1045.02  | 789913  | 1623.6   |
| Recursively find all prices                 | jsonpath-ng | 3742.35  | 3745.21     | 3748.06  | 789913  | 1628.0   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.03     | 10      | 1630.5   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 10      | 1613.5   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 10      | 1612.5   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 1       | 1456.0   |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.03     | 1       | 1456.0   |
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.03     | 0       | 1456.0   |
| Extract all user IDs                        | jmespath    | 2.03     | 2.07        | 2.11     | 4000    | 952.6    |
| Extract all user IDs                        | strata      | 10.66    | 10.71       | 10.77    | 0       | 952.5    |
| Extract all user IDs                        | jsonpath-ng | 32.31    | 32.41       | 32.50    | 4000    | 954.5    |
| Extract all user names                      | jmespath    | 1.92     | 1.95        | 1.98     | 4000    | 1456.1   |
| Extract all user names                      | strata      | 10.73    | 11.01       | 11.29    | 0       | 1456.1   |
| Extract all user names                      | jsonpath-ng | 31.89    | 32.49       | 33.09    | 4000    | 1456.1   |
| Extract nested timestamp field              | jmespath    | 3.23     | 3.41        | 3.60     | 4000    | 1353.0   |
| Extract nested timestamp field              | strata      | 10.88    | 11.50       | 12.12    | 0       | 1353.0   |
| Extract nested timestamp field              | jsonpath-ng | 34.44    | 34.69       | 34.94    | 4000    | 1354.0   |
| Extract order item prices (double wildcard) | strata      | 10.60    | 10.77       | 10.93    | 0       | 1354.4   |
| Extract order item prices (double wildcard) | jmespath    | 181.42   | 184.84      | 188.25   | 789913  | 1395.1   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1046.33  | 1055.04     | 1063.76  | 789913  | 1457.8   |
| Filter users by age (numeric predicate)     | strata      | 0.03     | 0.03        | 0.03     | 0       | 1456.2   |
| Filter users by age (numeric predicate)     | jmespath    | 5.19     | 5.27        | 5.36     | 3212    | 1456.2   |
| Recursively find all prices                 | strata      | 1041.69  | 1050.04     | 1058.39  | 3898    | 1494.7   |
| Recursively find all prices                 | jsonpath-ng | 3720.92  | 3729.70     | 3738.48  | 789913  | 1551.6   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 0       | 1550.6   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 10      | 1531.6   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 10      | 1533.6   |
