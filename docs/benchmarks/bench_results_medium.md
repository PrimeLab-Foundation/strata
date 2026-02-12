# Strata Benchmark Results

Generated: 2026-02-13 00:09:43

## Environment

- Commit: 454fe03a16725c7bac048e9e5926d9bf888849a6
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
| msgspec       | users.json   | 24.155   | 30.343      | 38.659   | 2713.5   |
| orjson        | users.json   | 20.053   | 33.273      | 34.504   | 2713.6   |
| ujson         | users.json   | 31.407   | 45.302      | 48.062   | 2732.1   |
| json (stdlib) | users.json   | 43.972   | 49.999      | 54.476   | 2713.5   |
| strata        | users.json   | 80.078   | 118.206     | 296.929  | 2692.0   |
| msgspec       | users.ndjson | 24.730   | 25.391      | 29.028   | 2691.9   |
| orjson        | users.ndjson | 25.550   | 25.987      | 26.059   | 2691.0   |
| ujson         | users.ndjson | 35.024   | 35.904      | 38.633   | 2709.6   |
| json (stdlib) | users.ndjson | 42.141   | 42.683      | 43.962   | 2694.4   |
| strata        | users.ndjson | 54.425   | 62.118      | 63.109   | 2724.8   |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.179    | 2000    |
|                                     | jmespath    | 0.524    | 2000    |
|                                     | jsonpath-ng | 7.707    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 4.045    | 100999  |
|                                     | jmespath    | 34.884   | 2000    |
|                                     | jsonpath-ng | 139.689  | 100999  |
| $..price                            | jmespath    | 34.575   | 2000    |
|                                     | query       | 38.363   | 100999  |
|                                     | jsonpath-ng | 515.669  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.198    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 41.838   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 20.47    | 29.04       | 32.93    | 225.61 | 2822.0   |
| msgspec       | 21.48    | 31.36       | 32.05    | 208.92 | 2822.8   |
| ujson         | 31.02    | 41.55       | 45.52    | 157.68 | 2840.6   |
| json (stdlib) | 37.20    | 45.10       | 46.12    | 145.27 | 2822.8   |
| strata        | 81.44    | 96.49       | 144.66   | 67.90  | 2802.4   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 24.66    | 25.26       | 25.43    | 259.26 | 178.0    |
| msgspec       | 26.42    | 26.77       | 28.38    | 244.68 | 177.7    |
| ujson         | 35.44    | 38.81       | 39.70    | 168.77 | 195.5    |
| json (stdlib) | 41.56    | 44.55       | 48.64    | 147.02 | 178.6    |
| strata        | 44.77    | 50.54       | 53.61    | 129.60 | 176.8    |
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
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 513011.00 | 1       | 1477.2   |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 422687.96 | 1       | 1477.2   |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.02        | 0.02     | 384916.59 | 1       | 1477.2   |
| Extract all user IDs                        | jmespath    | 0.73     | 0.74        | 0.75     | 8829.49   | 2000    | 554.1    |
| Extract all user IDs                        | strata      | 2.52     | 2.64        | 2.77     | 2477.02   | 2000    | 553.9    |
| Extract all user IDs                        | jsonpath-ng | 8.47     | 8.57        | 8.66     | 764.67    | 2000    | 555.4    |
| Extract all user names                      | jmespath    | 0.66     | 0.67        | 0.68     | 9823.81   | 2000    | 1599.2   |
| Extract all user names                      | strata      | 2.50     | 2.51        | 2.52     | 2606.65   | 2000    | 1599.2   |
| Extract all user names                      | jsonpath-ng | 7.96     | 8.00        | 8.03     | 819.41    | 2000    | 1599.2   |
| Extract nested timestamp field              | jmespath    | 1.29     | 1.38        | 1.47     | 4740.64   | 2000    | 986.8    |
| Extract nested timestamp field              | strata      | 2.59     | 2.63        | 2.67     | 2491.42   | 2000    | 986.8    |
| Extract nested timestamp field              | jsonpath-ng | 9.56     | 9.84        | 10.12    | 666.04    | 2000    | 987.5    |
| Extract order item prices (double wildcard) | jmespath    | 36.21    | 44.70       | 53.18    | 146.59    | 2000    | 1302.8   |
| Extract order item prices (double wildcard) | strata      | 131.35   | 132.18      | 133.01   | 49.57     | 100999  | 1300.7   |
| Extract order item prices (double wildcard) | jsonpath-ng | 157.27   | 161.56      | 165.85   | 40.55     | 100999  | 1307.5   |
| Filter users by age (numeric predicate)     | strata      | 2.35     | 2.36        | 2.37     | 2774.83   | 1606    | 1704.2   |
| Filter users by age (numeric predicate)     | jmespath    | 2.58     | 2.59        | 2.60     | 2532.06   | 1606    | 1704.2   |
| Recursively find all prices                 | strata      | 119.42   | 120.53      | 121.64   | 54.36     | 100999  | 1997.5   |
| Recursively find all prices                 | jsonpath-ng | 524.12   | 536.73      | 549.33   | 12.21     | 100999  | 2004.3   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 313222.04 | 10      | 2090.8   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.03        | 0.03     | 236808.55 | 10      | 2090.8   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.04     | 180114.47 | 10      | 2090.8   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 328.77   | 330.99      | 333.20   | 158.36 | 2265.8   |
| strata_cursor_reparse | 805.11   | 832.00      | 858.89   | 63.00  | 3492.9   |
- Speedup (reuse vs reparse): 2.51x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 463692.06 | 1       | 215.4    |
| Deep path navigation                        | strata              | 0.01     | 0.01        | 0.01     | 463020.19 | 1       | 215.4    |
| Deep path navigation                        | jmespath            | 0.01     | 0.02        | 0.03     | 324432.93 | 1       | 215.4    |
| Extract all user IDs                        | strata              | 0.62     | 0.63        | 0.64     | 10357.91  | 0       | 206.3    |
| Extract all user IDs                        | jmespath            | 0.95     | 0.96        | 0.97     | 6840.67   | 2000    | 206.8    |
| Extract all user IDs                        | jsonpath-ng         | 9.44     | 9.48        | 9.52     | 690.82    | 2000    | 206.8    |
| Extract all user IDs                        | strata_ndjson_fused | 22.38    | 22.62       | 22.85    | 289.59    | 0       | 206.5    |
| Extract all user IDs                        | strata_ndjson_full  | 22.26    | 23.09       | 23.92    | 283.65    | 0       | 206.5    |
| Extract all user names                      | strata              | 0.62     | 0.62        | 0.63     | 10516.60  | 0       | 215.5    |
| Extract all user names                      | jmespath            | 0.86     | 0.87        | 0.87     | 7559.29   | 2000    | 215.5    |
| Extract all user names                      | jsonpath-ng         | 8.10     | 8.11        | 8.13     | 807.15    | 2000    | 215.5    |
| Extract all user names                      | strata_ndjson_fused | 20.91    | 21.12       | 21.33    | 310.12    | 0       | 215.5    |
| Extract all user names                      | strata_ndjson_full  | 21.62    | 21.73       | 21.84    | 301.42    | 0       | 215.5    |
| Extract nested timestamp field              | strata              | 0.60     | 0.60        | 0.61     | 10838.18  | 0       | 207.1    |
| Extract nested timestamp field              | jmespath            | 1.66     | 1.70        | 1.73     | 3860.21   | 2000    | 207.1    |
| Extract nested timestamp field              | jsonpath-ng         | 9.14     | 9.74        | 10.34    | 672.46    | 2000    | 207.1    |
| Extract order item prices (double wildcard) | strata              | 0.60     | 0.60        | 0.60     | 10936.98  | 0       | 207.3    |
| Extract order item prices (double wildcard) | jmespath            | 26.66    | 27.26       | 27.86    | 240.26    | 100999  | 210.8    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 138.98   | 139.07      | 139.16   | 47.10     | 100999  | 217.4    |
| Filter users by age (numeric predicate)     | strata              | 0.02     | 0.02        | 0.02     | 372044.41 | 0       | 215.5    |
| Filter users by age (numeric predicate)     | jmespath            | 2.75     | 2.76        | 2.77     | 2373.33   | 1606    | 215.5    |
| NDJSON root field (id)                      | jsonpath-ng         | 0.03     | 0.03        | 0.03     | 217563.92 | 0       | 149.3    |
| NDJSON root field (id)                      | strata              | 9.11     | 9.14        | 9.17     | 716.61    | 2000    | 148.0    |
| NDJSON root field (id)                      | strata_ndjson_full  | 30.68    | 30.84       | 30.99    | 212.41    | 2000    | 149.1    |
| NDJSON root field (id)                      | strata_ndjson_fused | 31.08    | 31.16       | 31.25    | 210.18    | 2000    | 149.3    |
| Recursively find all prices                 | strata              | 133.47   | 134.13      | 134.78   | 48.83     | 1908    | 219.7    |
| Recursively find all prices                 | jsonpath-ng         | 515.58   | 516.08      | 516.57   | 12.69     | 100999  | 222.7    |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.02        | 0.02     | 406167.41 | 10      | 221.7    |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.03        | 0.03     | 259819.22 | 10      | 222.7    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 230317.48 | 10      | 220.7    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 198.64   | 200.88      | 203.12   | 293.44 | 220.9    |
| strata_cursor_reparse | 703.40   | 710.53      | 717.66   | 82.96  | 284.3    |
- Speedup (reuse vs reparse): 3.54x
