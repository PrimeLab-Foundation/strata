# Strata Benchmark Results

Generated: 2026-02-12 02:38:59

## Environment

- Commit: 10634f897c158962cdece183af3a38887bc5f426
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
| orjson        | users.json   | 18.311   | 26.415      | 27.827   | 2714.9   |
| msgspec       | users.json   | 19.428   | 28.392      | 28.686   | 2714.7   |
| ujson         | users.json   | 27.220   | 39.199      | 40.339   | 2733.3   |
| json (stdlib) | users.json   | 34.527   | 43.752      | 44.126   | 2714.7   |
| strata        | users.json   | 76.715   | 79.580      | 125.327  | 2693.3   |
| orjson        | users.ndjson | 22.611   | 23.571      | 24.694   | 2696.3   |
| msgspec       | users.ndjson | 22.368   | 24.059      | 24.951   | 2697.2   |
| ujson         | users.ndjson | 31.238   | 34.989      | 35.781   | 2714.9   |
| json (stdlib) | users.ndjson | 39.112   | 41.098      | 41.990   | 2699.7   |
| strata        | users.ndjson | 50.577   | 58.697      | 60.804   | 2729.0   |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.105    | 2000    |
|                                     | jmespath    | 0.457    | 2000    |
|                                     | jsonpath-ng | 6.365    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 3.843    | 100999  |
|                                     | jmespath    | 33.639   | 2000    |
|                                     | jsonpath-ng | 131.952  | 100999  |
| $..price                            | jmespath    | 34.143   | 2000    |
|                                     | query       | 37.873   | 100999  |
|                                     | jsonpath-ng | 496.630  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.128    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 40.662   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 18.76    | 27.20       | 27.22    | 240.91 | 2735.6   |
| msgspec       | 19.99    | 28.69       | 29.36    | 228.37 | 2736.4   |
| ujson         | 27.71    | 38.66       | 39.83    | 169.47 | 2754.2   |
| json (stdlib) | 35.63    | 44.76       | 45.57    | 146.37 | 2736.5   |
| strata        | 78.30    | 80.61       | 124.06   | 81.28  | 2716.0   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 21.95    | 23.70       | 24.00    | 276.35 | 144.3    |
| msgspec       | 22.71    | 24.94       | 25.02    | 262.63 | 144.0    |
| ujson         | 31.13    | 35.34       | 35.58    | 185.35 | 161.9    |
| json (stdlib) | 39.36    | 41.56       | 41.74    | 157.59 | 145.0    |
| strata        | 46.51    | 49.20       | 49.41    | 133.13 | 143.1    |
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
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 376619.16 | 1       | 1538.0   |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 328601.85 | 1       | 1539.0   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 264713.63 | 1       | 1538.0   |
| Extract all user IDs                        | jmespath    | 0.49     | 0.56        | 0.63     | 11665.99  | 2000    | 553.2    |
| Extract all user IDs                        | strata      | 2.38     | 2.38        | 2.39     | 2749.91   | 2000    | 553.0    |
| Extract all user IDs                        | jsonpath-ng | 6.06     | 6.26        | 6.46     | 1045.84   | 2000    | 554.4    |
| Extract all user names                      | jmespath    | 0.47     | 0.49        | 0.51     | 13266.40  | 2000    | 1933.0   |
| Extract all user names                      | strata      | 2.46     | 2.49        | 2.52     | 2633.66   | 2000    | 1933.0   |
| Extract all user names                      | jsonpath-ng | 6.11     | 6.19        | 6.27     | 1058.24   | 2000    | 1933.0   |
| Extract nested timestamp field              | jmespath    | 1.28     | 1.29        | 1.31     | 5068.25   | 2000    | 985.4    |
| Extract nested timestamp field              | strata      | 2.35     | 2.44        | 2.53     | 2689.22   | 2000    | 985.3    |
| Extract nested timestamp field              | jsonpath-ng | 9.05     | 9.14        | 9.24     | 716.45    | 2000    | 986.1    |
| Extract order item prices (double wildcard) | jmespath    | 36.05    | 36.75       | 37.45    | 178.27    | 2000    | 1153.2   |
| Extract order item prices (double wildcard) | strata      | 123.10   | 123.93      | 124.76   | 52.86     | 100999  | 1151.2   |
| Extract order item prices (double wildcard) | jsonpath-ng | 135.45   | 137.46      | 139.47   | 47.66     | 100999  | 1158.3   |
| Filter users by age (numeric predicate)     | strata      | 2.09     | 2.09        | 2.10     | 3127.97   | 1606    | 2078.4   |
| Filter users by age (numeric predicate)     | jmespath    | 2.31     | 2.37        | 2.42     | 2768.70   | 1606    | 2078.4   |
| Recursively find all prices                 | strata      | 194.70   | 195.33      | 195.96   | 33.54     | 100999  | 2178.8   |
| Recursively find all prices                 | jsonpath-ng | 509.42   | 510.92      | 512.42   | 12.82     | 100999  | 2182.4   |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 403688.68 | 10      | 2235.7   |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 217482.59 | 10      | 2235.7   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 206484.91 | 10      | 2235.7   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 374.59   | 377.49      | 380.38   | 138.85 | 2375.2   |
| strata_cursor_reparse | 773.44   | 788.28      | 803.11   | 66.49  | 3781.9   |
- Speedup (reuse vs reparse): 2.09x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.01        | 0.01     | 599977.11 | 1       | 219.1    |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 510375.85 | 1       | 219.1    |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.02     | 444029.28 | 0       | 219.1    |
| Extract all user IDs                        | jmespath            | 0.70     | 0.80        | 0.90     | 8196.25   | 2000    | 213.4    |
| Extract all user IDs                        | strata              | 5.61     | 5.65        | 5.69     | 1159.32   | 0       | 213.2    |
| Extract all user IDs                        | jsonpath-ng         | 6.38     | 6.48        | 6.58     | 1011.16   | 2000    | 213.5    |
| Extract all user IDs                        | strata_ndjson_full  | 22.63    | 22.65       | 22.66    | 289.22    | 0       | 213.4    |
| Extract all user IDs                        | strata_ndjson_fused | 22.80    | 22.80       | 22.81    | 287.24    | 0       | 213.4    |
| Extract all user names                      | jmespath            | 0.68     | 0.72        | 0.76     | 9093.06   | 2000    | 219.3    |
| Extract all user names                      | strata              | 5.64     | 5.67        | 5.70     | 1154.99   | 0       | 219.2    |
| Extract all user names                      | jsonpath-ng         | 6.60     | 6.65        | 6.69     | 985.53    | 2000    | 219.3    |
| Extract all user names                      | strata_ndjson_full  | 22.30    | 22.67       | 23.04    | 288.89    | 0       | 219.3    |
| Extract all user names                      | strata_ndjson_fused | 22.51    | 22.72       | 22.93    | 288.33    | 0       | 219.3    |
| Extract nested timestamp field              | jmespath            | 1.32     | 1.38        | 1.44     | 4746.27   | 2000    | 213.8    |
| Extract nested timestamp field              | strata              | 5.43     | 5.52        | 5.61     | 1185.85   | 0       | 213.8    |
| Extract nested timestamp field              | jsonpath-ng         | 7.70     | 7.78        | 7.86     | 841.36    | 2000    | 213.8    |
| Extract order item prices (double wildcard) | strata              | 5.60     | 5.62        | 5.63     | 1166.09   | 0       | 213.8    |
| Extract order item prices (double wildcard) | jmespath            | 26.05    | 26.61       | 27.18    | 246.09    | 100999  | 216.7    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 133.50   | 135.22      | 136.94   | 48.44     | 100999  | 221.0    |
| Filter users by age (numeric predicate)     | strata              | 0.01     | 0.01        | 0.02     | 455628.02 | 0       | 219.4    |
| Filter users by age (numeric predicate)     | jmespath            | 2.51     | 2.56        | 2.61     | 2561.42   | 1606    | 219.4    |
| NDJSON root field (id)                      | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 479231.00 | 0       | 156.4    |
| NDJSON root field (id)                      | strata              | 7.04     | 7.19        | 7.34     | 910.58    | 2000    | 155.2    |
| NDJSON root field (id)                      | strata_ndjson_fused | 28.46    | 28.54       | 28.62    | 229.47    | 2000    | 156.4    |
| NDJSON root field (id)                      | strata_ndjson_full  | 28.70    | 28.75       | 28.81    | 227.80    | 2000    | 156.4    |
| Recursively find all prices                 | strata              | 193.70   | 194.48      | 195.26   | 33.68     | 1908    | 223.6    |
| Recursively find all prices                 | jsonpath-ng         | 500.11   | 501.19      | 502.27   | 13.07     | 100999  | 229.4    |
| Slice first 10 users, extract IDs           | strata              | 0.01     | 0.01        | 0.01     | 522238.60 | 0       | 230.0    |
| Slice first 10 users, extract IDs           | jmespath            | 0.02     | 0.02        | 0.03     | 296874.87 | 10      | 229.0    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 294366.40 | 10      | 228.0    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 278.88   | 279.53      | 280.18   | 210.88 | 228.1    |
| strata_cursor_reparse | 769.81   | 770.13      | 770.46   | 76.54  | 291.8    |
- Speedup (reuse vs reparse): 2.76x
