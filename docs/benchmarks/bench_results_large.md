# Strata Benchmark Results

Generated: 2026-02-09 17:02:16

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
| orjson        | users.json   | 144.440  | 209.910     | 220.501  | 712.7    |
| msgspec       | users.json   | 153.239  | 210.367     | 221.392  | 704.1    |
| strata        | users.json   | 198.193  | 246.901     | 258.805  | 565.9    |
| ujson         | users.json   | 204.979  | 292.994     | 327.732  | 827.5    |
| json (stdlib) | users.json   | 267.702  | 327.356     | 335.429  | 704.1    |
| orjson        | users.ndjson | 208.730  | 231.819     | 236.560  | 577.7    |
| msgspec       | users.ndjson | 208.252  | 245.190     | 255.308  | 571.1    |
| strata        | users.ndjson | 287.599  | 297.289     | 326.317  | 523.6    |
| ujson         | users.ndjson | 285.476  | 341.865     | 349.507  | 694.6    |
| json (stdlib) | users.ndjson | 338.395  | 361.283     | 367.039  | 573.8    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.698    | 4000    |
|                                     | jmespath    | 1.772    | 4000    |
|                                     | jsonpath-ng | 36.662   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 33.167   | 789913  |
|                                     | jmespath    | 231.672  | 4000    |
|                                     | jsonpath-ng | 1161.187 | 789913  |
| $..price                            | jmespath    | 236.298  | 4000    |
|                                     | query       | 287.116  | 789913  |
|                                     | jsonpath-ng | 3914.721 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.779    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 304.783  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 143.03   | 199.06      | 206.33   | 712.7    |
| msgspec       | 148.90   | 208.64      | 213.80   | 704.0    |
| strata        | 220.15   | 248.62      | 250.33   | 565.8    |
| ujson         | 216.14   | 293.63      | 296.88   | 826.5    |
| json (stdlib) | 260.88   | 339.81      | 344.46   | 704.0    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 188.32   | 236.32      | 254.58   | 878.7    |
| strata        | 235.89   | 244.31      | 249.01   | 878.4    |
| msgspec       | 199.46   | 246.98      | 249.06   | 870.0    |
| ujson         | 275.01   | 339.08      | 359.53   | 992.6    |
| json (stdlib) | 319.51   | 362.96      | 369.12   | 872.8    |
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
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 1       | 1606.3   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 1       | 1583.3   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1       | 1583.3   |
| Extract all user IDs                        | jmespath    | 1.76     | 1.77        | 1.79     | 4000    | 1059.7   |
| Extract all user IDs                        | strata      | 3.68     | 3.88        | 4.07     | 4000    | 1059.4   |
| Extract all user IDs                        | jsonpath-ng | 36.53    | 37.10       | 37.67    | 4000    | 1061.5   |
| Extract all user names                      | jmespath    | 1.31     | 1.35        | 1.39     | 4000    | 1583.5   |
| Extract all user names                      | strata      | 3.82     | 4.22        | 4.62     | 4000    | 1583.5   |
| Extract all user names                      | jsonpath-ng | 36.18    | 36.50       | 36.81    | 4000    | 1583.5   |
| Extract nested timestamp field              | jmespath    | 2.80     | 2.83        | 2.86     | 4000    | 1411.7   |
| Extract nested timestamp field              | strata      | 4.55     | 4.59        | 4.63     | 4000    | 1411.7   |
| Extract nested timestamp field              | jsonpath-ng | 39.67    | 40.45       | 41.23    | 4000    | 1412.8   |
| Extract order item prices (double wildcard) | jmespath    | 229.79   | 231.67      | 233.56   | 4000    | 1601.0   |
| Extract order item prices (double wildcard) | strata      | 791.69   | 793.92      | 796.15   | 789913  | 1587.3   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1159.48  | 1161.41     | 1163.35  | 789913  | 1616.1   |
| Filter users by age (numeric predicate)     | strata      | 3.62     | 3.63        | 3.64     | 3212    | 1583.5   |
| Filter users by age (numeric predicate)     | jmespath    | 5.08     | 5.11        | 5.13     | 3212    | 1583.6   |
| Recursively find all prices                 | strata      | 1112.67  | 1114.13     | 1115.58  | 789913  | 1624.3   |
| Recursively find all prices                 | jsonpath-ng | 4167.90  | 4169.71     | 4171.52  | 789913  | 1628.7   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 10      | 1614.3   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 10      | 1631.3   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.05     | 10      | 1613.3   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 1       | 1901.8   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1       | 1901.8   |
| Deep path navigation                        | strata      | 511.25   | 512.32      | 513.39   | 1       | 1918.8   |
| Extract all user IDs                        | jmespath    | 2.39     | 2.42        | 2.44     | 4000    | 1369.3   |
| Extract all user IDs                        | jsonpath-ng | 70.43    | 71.33       | 72.23    | 4000    | 1371.2   |
| Extract all user IDs                        | strata      | 526.25   | 540.69      | 555.13   | 4000    | 1369.3   |
| Extract all user names                      | jmespath    | 2.11     | 2.27        | 2.42     | 4000    | 2033.2   |
| Extract all user names                      | jsonpath-ng | 68.93    | 70.84       | 72.75    | 4000    | 2033.7   |
| Extract all user names                      | strata      | 499.76   | 512.14      | 524.52   | 4000    | 2033.2   |
| Extract nested timestamp field              | jmespath    | 3.51     | 3.62        | 3.74     | 4000    | 1511.0   |
| Extract nested timestamp field              | jsonpath-ng | 75.55    | 75.57       | 75.59    | 4000    | 1511.0   |
| Extract nested timestamp field              | strata      | 513.66   | 517.30      | 520.94   | 4000    | 1511.0   |
| Extract order item prices (double wildcard) | jmespath    | 200.22   | 201.96      | 203.70   | 789913  | 1815.0   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1130.24  | 1132.14     | 1134.03  | 789913  | 1824.2   |
| Extract order item prices (double wildcard) | strata      | 1334.21  | 1356.59     | 1378.96  | 789913  | 1803.7   |
| Filter users by age (numeric predicate)     | jmespath    | 5.55     | 5.81        | 6.08     | 3212    | 2178.3   |
| Filter users by age (numeric predicate)     | strata      | 518.71   | 518.97      | 519.23   | 3212    | 2178.3   |
| Recursively find all prices                 | strata      | 1692.95  | 1693.94     | 1694.94  | 789913  | 2287.1   |
| Recursively find all prices                 | jsonpath-ng | 3864.79  | 3924.20     | 3983.61  | 789913  | 1519.9   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 10      | 1801.9   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.04     | 0.04        | 0.04     | 10      | 1607.6   |
| Slice first 10 users, extract IDs           | strata      | 483.45   | 487.32      | 491.18   | 10      | 1811.9   |
