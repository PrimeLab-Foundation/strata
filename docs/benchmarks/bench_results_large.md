# Strata Benchmark Results

Generated: 2026-02-14 13:20:54

## Environment

- Commit: c97b616486f501631c86fcd91188be9d26c26404
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
| orjson        | users.json   | 118.708  | 170.654     | 172.429  | 731.7    |
| msgspec       | users.json   | 129.479  | 180.856     | 186.077  | 724.2    |
| ujson         | users.json   | 175.713  | 246.196     | 250.664  | 846.6    |
| json (stdlib) | users.json   | 232.535  | 282.110     | 283.248  | 724.2    |
| strata        | users.json   | 275.839  | 300.281     | 303.628  | 593.6    |
| orjson        | users.ndjson | 160.336  | 198.768     | 209.181  | 742.5    |
| msgspec       | users.ndjson | 167.902  | 205.119     | 209.300  | 735.9    |
| ujson         | users.ndjson | 224.321  | 283.807     | 289.795  | 859.3    |
| json (stdlib) | users.ndjson | 275.797  | 312.380     | 313.441  | 738.6    |
| strata        | users.ndjson | 344.689  | 403.023     | 407.465  | 691.5    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.606    | 4000    |
|                                     | jmespath    | 1.571    | 4000    |
|                                     | jsonpath-ng | 31.872   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 26.975   | 789913  |
|                                     | jmespath    | 201.366  | 4000    |
|                                     | jsonpath-ng | 991.311  | 789913  |
| $..price                            | jmespath    | 205.012  | 4000    |
|                                     | query       | 248.670  | 789913  |
|                                     | jsonpath-ng | 3514.756 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.601    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 267.728  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 123.09   | 173.52      | 185.15   | 265.00 | 731.7    |
| msgspec       | 126.72   | 180.27      | 180.44   | 255.09 | 723.2    |
| ujson         | 186.78   | 249.16      | 249.60   | 184.55 | 845.7    |
| json (stdlib) | 238.14   | 289.47      | 297.41   | 158.85 | 723.3    |
| strata        | 279.86   | 304.31      | 306.92   | 151.11 | 593.6    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 162.05   | 202.38      | 204.40   | 227.20 | 1366.9   |
| strata        | 206.90   | 207.73      | 211.15   | 221.34 | 1366.5   |
| msgspec       | 169.72   | 210.32      | 224.88   | 218.62 | 1359.2   |
| ujson         | 251.26   | 286.90      | 296.51   | 160.26 | 1481.8   |
| json (stdlib) | 280.56   | 319.21      | 320.47   | 144.04 | 1362.0   |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2420242.74 | 1       | 10803.0  |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 2277870.96 | 1       | 10859.7  |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2207184.87 | 1       | 10791.4  |
| Extract all user IDs                        | jmespath    | 1.70     | 1.75        | 1.80     | 26307.54   | 4000    | 5557.5   |
| Extract all user IDs                        | strata      | 5.04     | 5.11        | 5.19     | 8996.95    | 4000    | 5557.1   |
| Extract all user IDs                        | jsonpath-ng | 31.78    | 31.82       | 31.87    | 1445.00    | 4000    | 5559.4   |
| Extract all user names                      | jmespath    | 1.27     | 1.27        | 1.27     | 36208.00   | 4000    | 11361.4  |
| Extract all user names                      | strata      | 4.47     | 4.50        | 4.52     | 10226.01   | 4000    | 11394.3  |
| Extract all user names                      | jsonpath-ng | 31.31    | 31.37       | 31.44    | 1465.63    | 4000    | 11327.2  |
| Extract nested timestamp field              | jmespath    | 2.45     | 2.47        | 2.49     | 18586.66   | 4000    | 9942.5   |
| Extract nested timestamp field              | strata      | 5.59     | 5.67        | 5.76     | 8104.71    | 4000    | 9996.3   |
| Extract nested timestamp field              | jsonpath-ng | 33.77    | 34.46       | 35.15    | 1334.51    | 4000    | 9943.7   |
| Extract order item prices (double wildcard) | jmespath    | 206.71   | 208.87      | 211.04   | 220.15     | 4000    | 9857.6   |
| Extract order item prices (double wildcard) | strata      | 897.26   | 900.95      | 904.64   | 51.04      | 789913  | 9843.9   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1037.16  | 1039.43     | 1041.70  | 44.24      | 789913  | 9476.8   |
| Filter users by age (numeric predicate)     | jmespath    | 4.62     | 4.63        | 4.63     | 9939.53    | 3212    | 12502.0  |
| Filter users by age (numeric predicate)     | strata      | 4.79     | 5.30        | 5.81     | 8669.94    | 3212    | 12519.2  |
| Recursively find all prices                 | strata      | 894.30   | 896.62      | 898.93   | 51.29      | 789913  | 14209.1  |
| Recursively find all prices                 | jsonpath-ng | 3692.04  | 3764.65     | 3837.27  | 12.21      | 789913  | 13063.5  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 2335664.85 | 10      | 13930.8  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1711009.99 | 10      | 13930.8  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.04        | 0.04     | 1221468.48 | 10      | 13945.8  |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2657.44  | 2760.67     | 2863.91  | 133.25 | 16453.4  |
| strata_cursor_reparse | 9946.30  | 10179.18    | 10412.07 | 36.14  | 17757.8  |
- Speedup (reuse vs reparse): 3.69x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2320784.19 | 1       | 2489.8   |
| Deep path navigation                        | jmespath            | 0.02     | 0.02        | 0.02     | 2222514.75 | 1       | 2490.8   |
| Deep path navigation                        | strata              | 0.02     | 0.03        | 0.03     | 1817941.81 | 1       | 2506.8   |
| Extract all user IDs                        | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 2818572.72 | 0       | 2394.3   |
| Extract all user IDs                        | jmespath            | 1.97     | 1.98        | 1.98     | 23269.41   | 4000    | 2394.3   |
| Extract all user IDs                        | strata              | 32.87    | 32.98       | 33.09    | 1394.03    | 4000    | 2389.3   |
| Extract all user IDs                        | strata_ndjson_full  | 78.36    | 80.20       | 82.04    | 573.32     | 4000    | 2393.9   |
| Extract all user IDs                        | strata_ndjson_fused | 80.66    | 81.26       | 81.85    | 565.85     | 4000    | 2394.2   |
| Extract all user names                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2298969.26 | 0       | 2717.0   |
| Extract all user names                      | jmespath            | 1.91     | 1.91        | 1.91     | 24091.37   | 4000    | 2717.0   |
| Extract all user names                      | strata              | 33.51    | 33.59       | 33.67    | 1368.80    | 4000    | 2715.4   |
| Extract all user names                      | strata_ndjson_fused | 80.76    | 81.31       | 81.87    | 565.47     | 4000    | 2717.0   |
| Extract all user names                      | strata_ndjson_full  | 81.32    | 82.37       | 83.41    | 558.23     | 4000    | 2716.9   |
| Extract nested timestamp field              | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2224826.95 | 0       | 2413.2   |
| Extract nested timestamp field              | jmespath            | 3.28     | 3.42        | 3.55     | 13460.99   | 4000    | 2413.2   |
| Extract nested timestamp field              | strata              | 34.52    | 35.90       | 37.28    | 1280.82    | 4000    | 2413.2   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 2970533.46 | 0       | 2502.4   |
| Extract order item prices (double wildcard) | jmespath            | 180.49   | 180.60      | 180.70   | 254.60     | 789913  | 2498.4   |
| Extract order item prices (double wildcard) | strata              | 948.48   | 962.15      | 975.83   | 47.79      | 789913  | 2465.9   |
| Filter users by age (numeric predicate)     | jmespath            | 5.08     | 5.17        | 5.26     | 8892.46    | 3212    | 2720.0   |
| Filter users by age (numeric predicate)     | strata              | 33.05    | 33.26       | 33.47    | 1382.55    | 3212    | 2720.0   |
| NDJSON first record id                      | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 2584345.71 | 1       | 1427.7   |
| NDJSON first record id                      | jmespath            | 0.02     | 0.02        | 0.02     | 2242896.42 | 1       | 1427.2   |
| NDJSON first record id                      | strata              | 0.02     | 0.02        | 0.03     | 1917485.27 | 1       | 1427.2   |
| Recursively find all prices                 | strata              | 969.75   | 974.85      | 979.95   | 47.17      | 789913  | 2739.6   |
| Recursively find all prices                 | jsonpath-ng         | 3628.28  | 3635.70     | 3643.11  | 12.65      | 789913  | 2775.8   |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.03        | 0.03     | 1836127.68 | 10      | 2772.2   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.03     | 0.03        | 0.03     | 1366543.07 | 10      | 2770.2   |
| Slice first 10 users, extract IDs           | strata              | 0.04     | 0.04        | 0.04     | 1128896.32 | 10      | 2781.2   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2327.03  | 2332.73     | 2338.42  | 177.40 | 3033.6   |
| strata_cursor_reparse | 5606.75  | 5629.94     | 5653.13  | 73.50  | 3799.1   |
- Speedup (reuse vs reparse): 2.41x
