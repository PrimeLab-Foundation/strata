# Strata Benchmark Results

Generated: 2026-02-09 17:43:14

## Environment

- Commit: b1c61c3a33340b140cf44500ca98dc46a704f09c
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
| orjson        | users.json   | 147.218  | 223.405     | 236.265  | 713.5    |
| msgspec       | users.json   | 155.862  | 239.674     | 247.793  | 647.3    |
| strata        | users.json   | 216.639  | 266.270     | 276.001  | 566.6    |
| ujson         | users.json   | 237.614  | 325.478     | 337.211  | 770.9    |
| json (stdlib) | users.json   | 284.712  | 356.825     | 365.361  | 647.3    |
| msgspec       | users.ndjson | 220.318  | 265.505     | 268.426  | 392.6    |
| orjson        | users.ndjson | 225.209  | 286.282     | 370.027  | 487.2    |
| strata        | users.ndjson | 277.432  | 314.409     | 349.972  | 435.1    |
| ujson         | users.ndjson | 368.305  | 379.929     | 388.824  | 516.0    |
| json (stdlib) | users.ndjson | 363.179  | 391.385     | 413.572  | 382.6    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.709    | 4000    |
|                                     | jmespath    | 1.915    | 4000    |
|                                     | jsonpath-ng | 41.401   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 32.523   | 789913  |
|                                     | jmespath    | 245.705  | 4000    |
|                                     | jsonpath-ng | 1222.591 | 789913  |
| $..price                            | jmespath    | 248.928  | 4000    |
|                                     | query       | 293.224  | 789913  |
|                                     | jsonpath-ng | 4233.628 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.657    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 309.920  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 154.28   | 221.46      | 230.09   | 711.5    |
| msgspec       | 159.56   | 225.40      | 240.92   | 703.8    |
| strata        | 227.91   | 260.40      | 268.22   | 565.5    |
| ujson         | 224.43   | 323.24      | 334.08   | 728.8    |
| json (stdlib) | 282.60   | 356.81      | 380.02   | 639.0    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 212.50   | 257.29      | 270.35   | 879.9    |
| strata        | 253.47   | 268.84      | 320.00   | 877.5    |
| msgspec       | 216.80   | 280.72      | 597.92   | 752.4    |
| ujson         | 311.94   | 384.84      | 390.87   | 874.9    |
| json (stdlib) | 345.39   | 397.26      | 399.62   | 755.2    |
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
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 1       | 1558.9   |
| Deep path navigation                        | jmespath    | 0.03     | 0.03        | 0.03     | 1       | 1535.9   |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1       | 1535.3   |
| Extract all user IDs                        | jmespath    | 2.01     | 2.06        | 2.10     | 4000    | 1059.6   |
| Extract all user IDs                        | strata      | 3.79     | 3.81        | 3.82     | 4000    | 1059.3   |
| Extract all user IDs                        | jsonpath-ng | 40.81    | 41.02       | 41.22    | 4000    | 1061.5   |
| Extract all user names                      | jmespath    | 1.58     | 1.77        | 1.95     | 4000    | 1386.4   |
| Extract all user names                      | strata      | 4.40     | 6.97        | 9.54     | 4000    | 1533.9   |
| Extract all user names                      | jsonpath-ng | 41.90    | 42.10       | 42.29    | 4000    | 1238.7   |
| Extract nested timestamp field              | jmespath    | 3.13     | 3.17        | 3.21     | 4000    | 1192.4   |
| Extract nested timestamp field              | strata      | 4.20     | 4.54        | 4.88     | 4000    | 1400.0   |
| Extract nested timestamp field              | jsonpath-ng | 44.68    | 44.92       | 45.16    | 4000    | 1178.9   |
| Extract order item prices (double wildcard) | jmespath    | 243.75   | 245.84      | 247.92   | 4000    | 1333.6   |
| Extract order item prices (double wildcard) | strata      | 794.37   | 820.49      | 846.60   | 789913  | 1537.0   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1246.68  | 1259.89     | 1273.09  | 789913  | 1311.5   |
| Filter users by age (numeric predicate)     | strata      | 3.74     | 3.76        | 3.78     | 3212    | 1390.4   |
| Filter users by age (numeric predicate)     | jmespath    | 5.80     | 6.01        | 6.21     | 3212    | 1389.7   |
| Recursively find all prices                 | strata      | 1184.34  | 1185.53     | 1186.73  | 789913  | 1557.4   |
| Recursively find all prices                 | jsonpath-ng | 4276.37  | 4334.78     | 4393.18  | 789913  | 349.1    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.03        | 0.03     | 10      | 1430.1   |
| Slice first 10 users, extract IDs           | jmespath    | 0.03     | 0.03        | 0.03     | 10      | 1413.1   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.05     | 0.05        | 0.05     | 10      | 1412.3   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.03     | 0.03        | 0.03     | 1       | 1378.2   |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1       | 1378.2   |
| Deep path navigation                        | strata      | 523.20   | 524.57      | 525.95   | 1       | 1395.2   |
| Extract all user IDs                        | jmespath    | 2.35     | 2.39        | 2.43     | 4000    | 1251.0   |
| Extract all user IDs                        | jsonpath-ng | 76.00    | 76.46       | 76.92    | 4000    | 1253.1   |
| Extract all user IDs                        | strata      | 567.13   | 572.70      | 578.27   | 4000    | 1294.9   |
| Extract all user names                      | jmespath    | 2.18     | 2.28        | 2.38     | 4000    | 1390.3   |
| Extract all user names                      | jsonpath-ng | 76.13    | 76.25       | 76.38    | 4000    | 1386.6   |
| Extract all user names                      | strata      | 525.74   | 527.74      | 529.75   | 4000    | 1431.6   |
| Extract nested timestamp field              | jmespath    | 3.59     | 3.74        | 3.88     | 4000    | 1422.9   |
| Extract nested timestamp field              | jsonpath-ng | 79.56    | 79.75       | 79.94    | 4000    | 1422.9   |
| Extract nested timestamp field              | strata      | 525.10   | 536.63      | 548.16   | 4000    | 1422.9   |
| Extract order item prices (double wildcard) | jmespath    | 206.30   | 206.71      | 207.11   | 789913  | 1704.1   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1202.84  | 1222.51     | 1242.17  | 789913  | 997.1    |
| Extract order item prices (double wildcard) | strata      | 1339.64  | 1376.97     | 1414.30  | 789913  | 1690.3   |
| Filter users by age (numeric predicate)     | jmespath    | 6.03     | 6.15        | 6.28     | 3212    | 1508.0   |
| Filter users by age (numeric predicate)     | strata      | 518.56   | 532.90      | 547.24   | 3212    | 1516.3   |
| Recursively find all prices                 | strata      | 1743.98  | 1746.89     | 1749.80  | 789913  | 1644.1   |
| Recursively find all prices                 | jsonpath-ng | 4177.50  | 4193.92     | 4210.33  | 789913  | 609.6    |
| Slice first 10 users, extract IDs           | jmespath    | 0.03     | 0.03        | 0.03     | 10      | 1389.4   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.04     | 0.05        | 0.05     | 10      | 1380.9   |
| Slice first 10 users, extract IDs           | strata      | 523.22   | 537.55      | 551.87   | 10      | 1399.8   |
