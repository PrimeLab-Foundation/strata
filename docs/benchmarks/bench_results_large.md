# Strata Benchmark Results

Generated: 2026-02-12 02:40:08

## Environment

- Commit: 10634f897c158962cdece183af3a38887bc5f426
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
| orjson        | users.json   | 126.924  | 182.216     | 196.955  | 10842.3  |
| msgspec       | users.json   | 134.943  | 189.549     | 190.820  | 10464.2  |
| ujson         | users.json   | 189.265  | 266.016     | 285.779  | 10586.6  |
| json (stdlib) | users.json   | 242.266  | 296.849     | 299.888  | 10508.1  |
| strata        | users.json   | 1517.070 | 1614.347    | 1804.130 | 10751.4  |
| orjson        | users.ndjson | 171.454  | 208.954     | 212.784  | 10772.7  |
| msgspec       | users.ndjson | 176.696  | 216.700     | 218.018  | 10516.3  |
| ujson         | users.ndjson | 238.277  | 300.522     | 308.897  | 10639.8  |
| json (stdlib) | users.ndjson | 291.264  | 326.457     | 329.578  | 10519.0  |
| strata        | users.ndjson | 360.862  | 420.589     | 432.470  | 10761.5  |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.593    | 4000    |
|                                     | jmespath    | 1.576    | 4000    |
|                                     | jsonpath-ng | 32.488   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 29.745   | 789913  |
|                                     | jmespath    | 215.539  | 4000    |
|                                     | jsonpath-ng | 1068.566 | 789913  |
| $..price                            | jmespath    | 218.102  | 4000    |
|                                     | query       | 262.997  | 789913  |
|                                     | jsonpath-ng | 3718.972 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.578    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 279.055  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 125.95   | 181.10      | 204.09   | 253.91 | 11936.4  |
| msgspec       | 134.46   | 192.20      | 195.00   | 239.25 | 11880.3  |
| ujson         | 189.85   | 267.34      | 274.62   | 172.00 | 11790.5  |
| json (stdlib) | 245.36   | 298.09      | 300.56   | 154.26 | 11711.9  |
| strata        | 1552.67  | 1582.70     | 1952.86  | 29.05  | 12073.4  |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 165.26   | 202.14      | 205.59   | 227.46 | 1408.7   |
| msgspec       | 174.52   | 209.62      | 211.56   | 219.35 | 1401.0   |
| strata        | 203.95   | 213.37      | 215.61   | 215.49 | 1409.5   |
| ujson         | 230.92   | 288.80      | 292.56   | 159.21 | 1523.6   |
| json (stdlib) | 284.17   | 320.91      | 324.58   | 143.28 | 1403.9   |
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
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 2665704.00 | 1       | 12506.8  |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 2531219.02 | 1       | 12437.8  |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2436270.45 | 1       | 12485.8  |
| Extract all user IDs                        | jmespath    | 1.68     | 1.78        | 1.87     | 25889.73   | 4000    | 5557.5   |
| Extract all user IDs                        | strata      | 4.87     | 4.89        | 4.91     | 9401.39    | 4000    | 5557.2   |
| Extract all user IDs                        | jsonpath-ng | 31.75    | 32.10       | 32.45    | 1432.37    | 4000    | 5559.5   |
| Extract all user names                      | jmespath    | 1.22     | 1.25        | 1.27     | 36826.00   | 4000    | 12427.2  |
| Extract all user names                      | strata      | 4.41     | 4.43        | 4.44     | 10390.26   | 4000    | 12437.0  |
| Extract all user names                      | jsonpath-ng | 31.73    | 31.92       | 32.12    | 1440.42    | 4000    | 12427.3  |
| Extract nested timestamp field              | jmespath    | 2.57     | 2.57        | 2.57     | 17894.11   | 4000    | 10420.0  |
| Extract nested timestamp field              | strata      | 5.47     | 5.82        | 6.18     | 7896.07    | 4000    | 10419.9  |
| Extract nested timestamp field              | jsonpath-ng | 34.54    | 34.79       | 35.03    | 1321.88    | 4000    | 10421.3  |
| Extract order item prices (double wildcard) | jmespath    | 219.08   | 220.17      | 221.26   | 208.85     | 4000    | 12530.6  |
| Extract order item prices (double wildcard) | strata      | 942.91   | 942.92      | 942.94   | 48.77      | 789913  | 12516.8  |
| Extract order item prices (double wildcard) | jsonpath-ng | 1077.38  | 1111.89     | 1146.40  | 41.36      | 789913  | 12051.5  |
| Filter users by age (numeric predicate)     | strata      | 4.51     | 4.55        | 4.58     | 10114.71   | 3212    | 12339.8  |
| Filter users by age (numeric predicate)     | jmespath    | 4.82     | 4.83        | 4.84     | 9521.81    | 3212    | 12339.8  |
| Recursively find all prices                 | strata      | 1256.08  | 1258.28     | 1260.48  | 36.54      | 789913  | 13589.0  |
| Recursively find all prices                 | jsonpath-ng | 3778.30  | 3810.32     | 3842.34  | 12.07      | 789913  | 13084.2  |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 3048689.86 | 10      | 13698.5  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1884874.20 | 10      | 13698.5  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 1502528.82 | 10      | 13727.1  |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2965.83  | 2984.61     | 3003.39  | 123.25 | 16608.6  |
| strata_cursor_reparse | 9379.21  | 9912.06     | 10444.91 | 37.11  | 16867.0  |
- Speedup (reuse vs reparse): 3.32x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 3255063.98 | 1       | 2731.0   |
| Deep path navigation                        | jmespath            | 0.01     | 0.02        | 0.02     | 2741683.51 | 1       | 2731.0   |
| Deep path navigation                        | strata              | 0.03     | 0.03        | 0.03     | 1575310.59 | 0       | 2731.0   |
| Extract all user IDs                        | jmespath            | 1.97     | 1.98        | 2.00     | 23194.34   | 4000    | 2436.6   |
| Extract all user IDs                        | strata              | 11.45    | 11.47       | 11.49    | 4009.05    | 0       | 2434.0   |
| Extract all user IDs                        | jsonpath-ng         | 32.35    | 32.51       | 32.68    | 1414.10    | 4000    | 2436.6   |
| Extract all user IDs                        | strata_ndjson_fused | 49.71    | 51.35       | 52.99    | 895.38     | 0       | 2436.5   |
| Extract all user IDs                        | strata_ndjson_full  | 53.28    | 53.53       | 53.78    | 858.98     | 0       | 2436.1   |
| Extract all user names                      | jmespath            | 1.94     | 1.94        | 1.95     | 23647.89   | 4000    | 2751.1   |
| Extract all user names                      | strata              | 11.57    | 11.59       | 11.62    | 3965.47    | 0       | 2750.3   |
| Extract all user names                      | jsonpath-ng         | 32.71    | 32.79       | 32.87    | 1402.15    | 4000    | 2751.1   |
| Extract all user names                      | strata_ndjson_fused | 52.79    | 53.34       | 53.88    | 862.08     | 0       | 2751.1   |
| Extract all user names                      | strata_ndjson_full  | 52.59    | 53.84       | 55.09    | 853.99     | 0       | 2751.0   |
| Extract nested timestamp field              | jmespath            | 3.04     | 3.07        | 3.10     | 14976.29   | 4000    | 2439.3   |
| Extract nested timestamp field              | strata              | 11.50    | 11.54       | 11.57    | 3985.80    | 0       | 2439.3   |
| Extract nested timestamp field              | jsonpath-ng         | 35.14    | 35.18       | 35.23    | 1306.83    | 4000    | 2439.3   |
| Extract order item prices (double wildcard) | strata              | 11.56    | 11.60       | 11.65    | 3962.60    | 0       | 2458.8   |
| Extract order item prices (double wildcard) | jmespath            | 179.04   | 181.41      | 183.78   | 253.46     | 789913  | 2492.2   |
| Extract order item prices (double wildcard) | jsonpath-ng         | 1034.22  | 1034.30     | 1034.39  | 44.45      | 789913  | 2498.2   |
| Filter users by age (numeric predicate)     | strata              | 0.03     | 0.03        | 0.03     | 1631197.45 | 0       | 2755.2   |
| Filter users by age (numeric predicate)     | jmespath            | 5.23     | 5.26        | 5.30     | 8738.35    | 3212    | 2755.3   |
| NDJSON root field (id)                      | jsonpath-ng         | 0.01     | 0.02        | 0.03     | 2268235.46 | 0       | 1482.9   |
| NDJSON root field (id)                      | strata              | 33.27    | 33.88       | 34.49    | 1357.17    | 4000    | 1470.2   |
| NDJSON root field (id)                      | strata_ndjson_full  | 83.42    | 84.67       | 85.92    | 543.04     | 4000    | 1482.3   |
| NDJSON root field (id)                      | strata_ndjson_fused | 84.37    | 86.64       | 88.91    | 530.68     | 4000    | 1482.9   |
| Recursively find all prices                 | strata              | 1249.83  | 1254.54     | 1259.25  | 36.65      | 3898    | 2794.5   |
| Recursively find all prices                 | jsonpath-ng         | 3720.71  | 3732.00     | 3743.29  | 12.32      | 789913  | 2822.3   |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.02     | 2444087.15 | 10      | 2810.9   |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.03        | 0.03     | 1820965.48 | 0       | 2826.9   |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 1613311.79 | 10      | 2808.9   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 1719.64  | 1720.44     | 1721.24  | 240.53 | 3151.0   |
| strata_cursor_reparse | 5091.25  | 5123.31     | 5155.37  | 80.77  | 4140.2   |
- Speedup (reuse vs reparse): 2.98x
