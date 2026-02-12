# Strata Benchmark Results

Generated: 2026-02-12 02:13:50

## Environment

- Commit: 334cd9c91bd5c8d0997a34e0a112219177b470f0
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
| orjson        | users.json   | 141.793  | 202.769     | 216.842  | 13377.7  |
| msgspec       | users.json   | 139.601  | 206.859     | 214.471  | 13014.0  |
| ujson         | users.json   | 202.988  | 281.450     | 286.405  | 13136.4  |
| json (stdlib) | users.json   | 247.849  | 303.465     | 309.309  | 13057.9  |
| strata        | users.json   | 1644.650 | 1691.511    | 2311.945 | 13530.2  |
| orjson        | users.ndjson | 170.778  | 212.870     | 223.066  | 11244.7  |
| msgspec       | users.ndjson | 222.609  | 240.554     | 255.951  | 11026.2  |
| ujson         | users.ndjson | 266.622  | 305.401     | 321.109  | 11149.6  |
| json (stdlib) | users.ndjson | 321.716  | 331.753     | 340.698  | 11028.9  |
| strata        | users.ndjson | 402.804  | 425.926     | 460.511  | 11194.7  |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.594    | 4000    |
|                                     | jmespath    | 1.581    | 4000    |
|                                     | jsonpath-ng | 32.762   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 28.856   | 789913  |
|                                     | jmespath    | 216.445  | 4000    |
|                                     | jsonpath-ng | 1128.727 | 789913  |
| $..price                            | jmespath    | 220.190  | 4000    |
|                                     | query       | 270.773  | 789913  |
|                                     | jsonpath-ng | 3863.449 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.600    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 286.600  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 141.57   | 189.28      | 209.25   | 242.94 | 13687.1  |
| msgspec       | 137.21   | 196.89      | 200.10   | 233.55 | 13663.3  |
| ujson         | 187.37   | 270.27      | 278.49   | 170.14 | 13570.9  |
| json (stdlib) | 247.11   | 306.96      | 327.48   | 149.80 | 13492.3  |
| strata        | 1613.07  | 1766.23     | 2019.21  | 26.03  | 13992.1  |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 170.47   | 204.58      | 205.74   | 224.75 | 1404.7   |
| strata        | 201.30   | 209.71      | 210.67   | 219.25 | 1405.5   |
| msgspec       | 174.82   | 215.32      | 215.42   | 213.54 | 1397.0   |
| ujson         | 235.94   | 294.87      | 310.61   | 155.93 | 1519.6   |
| json (stdlib) | 294.31   | 336.56      | 344.88   | 136.62 | 1399.9   |
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
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.03     | 1899473.06 | 1       | 12959.2  |
| Deep path navigation                        | jmespath    | 0.03     | 0.03        | 0.03     | 1713656.32 | 1       | 12938.2  |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1360797.80 | 1       | 12938.3  |
| Extract all user IDs                        | jmespath    | 1.82     | 1.83        | 1.85     | 25069.03   | 4000    | 5557.7   |
| Extract all user IDs                        | strata      | 5.14     | 5.15        | 5.16     | 8923.33    | 4000    | 5557.6   |
| Extract all user IDs                        | jsonpath-ng | 33.06    | 33.32       | 33.58    | 1380.17    | 4000    | 5559.5   |
| Extract all user names                      | jmespath    | 1.44     | 1.46        | 1.49     | 31417.91   | 4000    | 13168.5  |
| Extract all user names                      | strata      | 5.12     | 5.16        | 5.20     | 8909.71    | 4000    | 13325.2  |
| Extract all user names                      | jsonpath-ng | 32.75    | 34.50       | 36.25    | 1332.78    | 4000    | 13168.5  |
| Extract nested timestamp field              | jmespath    | 2.61     | 2.69        | 2.76     | 17123.77   | 4000    | 10419.7  |
| Extract nested timestamp field              | strata      | 6.05     | 6.18        | 6.31     | 7441.56    | 4000    | 10419.7  |
| Extract nested timestamp field              | jsonpath-ng | 37.20    | 37.82       | 38.45    | 1215.76    | 4000    | 10421.0  |
| Extract order item prices (double wildcard) | jmespath    | 219.82   | 221.90      | 223.97   | 207.23     | 4000    | 12982.1  |
| Extract order item prices (double wildcard) | strata      | 846.97   | 856.63      | 866.29   | 53.68      | 789913  | 13154.7  |
| Extract order item prices (double wildcard) | jsonpath-ng | 1096.19  | 1101.29     | 1106.40  | 41.75      | 789913  | 12625.6  |
| Filter users by age (numeric predicate)     | strata      | 4.08     | 4.12        | 4.15     | 11169.38   | 3212    | 13088.9  |
| Filter users by age (numeric predicate)     | jmespath    | 4.94     | 5.02        | 5.11     | 9158.06    | 3212    | 13015.7  |
| Recursively find all prices                 | strata      | 1169.83  | 1219.40     | 1268.96  | 37.71      | 789913  | 12880.3  |
| Recursively find all prices                 | jsonpath-ng | 3818.76  | 3819.63     | 3820.50  | 12.04      | 789913  | 12258.6  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 1961958.47 | 10      | 12770.5  |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.03        | 0.03     | 1788645.75 | 10      | 12785.5  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 1209421.08 | 10      | 12770.5  |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2896.85  | 2963.17     | 3029.50  | 124.15 | 15054.1  |
| strata_cursor_reparse | 10702.59 | 10875.00    | 11047.41 | 33.83  | 17379.7  |
- Speedup (reuse vs reparse): 3.67x
