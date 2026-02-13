# Strata Benchmark Results

Generated: 2026-02-14 01:12:22

## Environment

- Commit: e8443f3b87ee8f0b4d32fbf0d4c653dcb146cde0
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/medium/users.json, benchmarks/data/generated/medium/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users.json   | 19.365   | 26.575      | 27.734   | 130.0    |
| msgspec       | users.json   | 20.274   | 28.797      | 32.178   | 129.9    |
| ujson         | users.json   | 27.976   | 39.582      | 39.679   | 148.5    |
| json (stdlib) | users.json   | 35.680   | 43.716      | 44.812   | 129.9    |
| strata        | users.json   | 42.539   | 45.469      | 46.771   | 110.1    |
| orjson        | users.ndjson | 22.482   | 24.419      | 25.091   | 152.0    |
| msgspec       | users.ndjson | 24.105   | 25.784      | 26.579   | 152.9    |
| ujson         | users.ndjson | 32.246   | 35.234      | 35.311   | 170.6    |
| json (stdlib) | users.ndjson | 40.234   | 41.414      | 41.868   | 155.3    |
| strata        | users.ndjson | 52.223   | 59.951      | 60.903   | 147.0    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.117    | 2000    |
|                                     | jmespath    | 0.464    | 2000    |
|                                     | jsonpath-ng | 7.429    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 4.144    | 100999  |
|                                     | jmespath    | 36.323   | 2000    |
|                                     | jsonpath-ng | 135.646  | 100999  |
| $..price                            | jmespath    | 33.803   | 2000    |
|                                     | query       | 37.847   | 100999  |
|                                     | jsonpath-ng | 500.213  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.150    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 40.989   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 18.84    | 26.24       | 27.96    | 249.71 | 129.4    |
| msgspec       | 20.67    | 28.34       | 29.72    | 231.15 | 130.2    |
| ujson         | 28.38    | 39.41       | 39.65    | 166.23 | 147.9    |
| json (stdlib) | 35.09    | 43.55       | 44.01    | 150.43 | 130.2    |
| strata        | 43.31    | 45.50       | 46.38    | 143.99 | 109.5    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 22.62    | 24.38       | 25.33    | 268.70 | 143.6    |
| msgspec       | 24.40    | 25.66       | 25.77    | 255.24 | 143.3    |
| ujson         | 32.59    | 35.95       | 36.59    | 182.20 | 161.2    |
| json (stdlib) | 41.21    | 41.86       | 43.13    | 156.48 | 144.4    |
| strata        | 46.83    | 49.13       | 49.25    | 133.32 | 142.4    |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 560570.45 | 1       | 1276.8   |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 543143.25 | 1       | 1276.8   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 446070.88 | 1       | 1276.8   |
| Extract all user IDs                        | jmespath    | 0.58     | 0.66        | 0.74     | 9994.59   | 2000    | 553.8    |
| Extract all user IDs                        | strata      | 2.43     | 2.44        | 2.46     | 2680.92   | 2000    | 553.5    |
| Extract all user IDs                        | jsonpath-ng | 6.88     | 6.89        | 6.90     | 950.94    | 2000    | 554.9    |
| Extract all user names                      | jmespath    | 0.48     | 0.51        | 0.54     | 12785.82  | 2000    | 1367.7   |
| Extract all user names                      | strata      | 2.20     | 2.24        | 2.27     | 2928.39   | 2000    | 1367.7   |
| Extract all user names                      | jsonpath-ng | 7.02     | 7.14        | 7.26     | 917.51    | 2000    | 1367.7   |
| Extract nested timestamp field              | jmespath    | 1.28     | 1.29        | 1.31     | 5072.01   | 2000    | 985.9    |
| Extract nested timestamp field              | strata      | 2.43     | 2.45        | 2.47     | 2671.79   | 2000    | 985.9    |
| Extract nested timestamp field              | jsonpath-ng | 8.06     | 8.12        | 8.19     | 806.66    | 2000    | 986.5    |
| Extract order item prices (double wildcard) | jmespath    | 34.56    | 34.75       | 34.95    | 188.53    | 2000    | 1184.0   |
| Extract order item prices (double wildcard) | strata      | 119.68   | 120.47      | 121.25   | 54.39     | 100999  | 1182.0   |
| Extract order item prices (double wildcard) | jsonpath-ng | 135.09   | 136.25      | 137.41   | 48.09     | 100999  | 1186.7   |
| Filter users by age (numeric predicate)     | strata      | 1.98     | 1.99        | 1.99     | 3299.82   | 1606    | 1474.8   |
| Filter users by age (numeric predicate)     | jmespath    | 2.46     | 2.52        | 2.58     | 2601.50   | 1606    | 1474.8   |
| Recursively find all prices                 | strata      | 112.70   | 114.01      | 115.32   | 57.47     | 100999  | 1551.3   |
| Recursively find all prices                 | jsonpath-ng | 502.22   | 505.65      | 509.07   | 12.96     | 100999  | 1554.6   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 339975.18 | 10      | 1617.3   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.02     | 273698.75 | 10      | 1617.3   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.03     | 266957.20 | 10      | 1617.3   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 311.11   | 315.85      | 320.58   | 165.95 | 1970.6   |
| strata_cursor_reparse | 716.50   | 739.55      | 762.59   | 70.87  | 3400.8   |
- Speedup (reuse vs reparse): 2.34x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.02     | 480725.87 | 1       | 224.8    |
| Deep path navigation                        | jmespath            | 0.02     | 0.02        | 0.02     | 325109.37 | 1       | 224.8    |
| Deep path navigation                        | jsonpath-ng         | 0.02     | 0.02        | 0.03     | 298845.74 | 1       | 224.8    |
| Extract all user IDs                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 636414.28 | 0       | 213.0    |
| Extract all user IDs                        | jmespath            | 0.83     | 0.83        | 0.83     | 7908.42   | 2000    | 213.0    |
| Extract all user IDs                        | strata              | 7.37     | 7.38        | 7.40     | 886.92    | 2000    | 211.8    |
| Extract all user IDs                        | strata_ndjson_full  | 28.67    | 29.14       | 29.61    | 224.77    | 2000    | 212.8    |
| Extract all user IDs                        | strata_ndjson_fused | 29.18    | 29.51       | 29.84    | 221.96    | 2000    | 212.8    |
| Extract all user names                      | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 440327.55 | 0       | 224.8    |
| Extract all user names                      | jmespath            | 0.75     | 0.79        | 0.82     | 8318.78   | 2000    | 224.8    |
| Extract all user names                      | strata              | 7.30     | 7.34        | 7.37     | 892.79    | 2000    | 224.8    |
| Extract all user names                      | strata_ndjson_fused | 29.47    | 29.60       | 29.73    | 221.26    | 2000    | 224.8    |
| Extract all user names                      | strata_ndjson_full  | 29.34    | 29.75       | 30.15    | 220.18    | 2000    | 224.8    |
| Extract nested timestamp field              | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 306115.86 | 0       | 213.4    |
| Extract nested timestamp field              | jmespath            | 1.64     | 1.65        | 1.67     | 3963.68   | 2000    | 213.4    |
| Extract nested timestamp field              | strata              | 9.05     | 9.17        | 9.29     | 714.49    | 2000    | 213.4    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 495079.58 | 0       | 224.8    |
| Extract order item prices (double wildcard) | jmespath            | 26.03    | 26.71       | 27.39    | 245.23    | 100999  | 224.8    |
| Extract order item prices (double wildcard) | strata              | 132.02   | 132.63      | 133.25   | 49.38     | 100999  | 221.0    |
| Filter users by age (numeric predicate)     | jmespath            | 2.66     | 2.67        | 2.68     | 2452.12   | 1606    | 224.9    |
| Filter users by age (numeric predicate)     | strata              | 6.96     | 7.11        | 7.26     | 921.16    | 1606    | 224.8    |
| NDJSON first record id                      | jmespath            | 0.01     | 0.01        | 0.01     | 506253.50 | 1       | 153.1    |
| NDJSON first record id                      | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 422000.18 | 1       | 153.4    |
| NDJSON first record id                      | strata              | 0.02     | 0.03        | 0.05     | 190076.43 | 1       | 153.1    |
| Recursively find all prices                 | strata              | 130.60   | 130.78      | 130.96   | 50.08     | 100999  | 224.9    |
| Recursively find all prices                 | jsonpath-ng         | 506.17   | 507.54      | 508.91   | 12.90     | 100999  | 227.3    |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.02        | 0.02     | 339140.58 | 10      | 227.1    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.03     | 0.03        | 0.03     | 242579.72 | 10      | 226.1    |
| Slice first 10 users, extract IDs           | strata              | 0.03     | 0.03        | 0.04     | 187467.72 | 10      | 227.1    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 289.34   | 292.25      | 295.16   | 201.70 | 226.3    |
| strata_cursor_reparse | 780.13   | 780.69      | 781.25   | 75.51  | 289.9    |
- Speedup (reuse vs reparse): 2.67x
