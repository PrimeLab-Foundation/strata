# Strata Benchmark Results

Generated: 2026-02-12 22:20:31

## Environment

- Commit: 63b52a91dc763c26eb4637cbc9ea21a3e346893a
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
| orjson        | users.json   | 23.837   | 36.537      | 37.581   | 2349.5   |
| msgspec       | users.json   | 23.571   | 37.136      | 39.486   | 2121.8   |
| ujson         | users.json   | 35.988   | 51.252      | 54.243   | 2150.8   |
| json (stdlib) | users.json   | 42.774   | 53.784      | 56.184   | 2113.8   |
| strata        | users.json   | 188.392  | 193.043     | 338.667  | 2527.5   |
| orjson        | users.ndjson | 27.517   | 28.638      | 30.872   | 1214.4   |
| msgspec       | users.ndjson | 27.905   | 30.162      | 30.511   | 1175.4   |
| ujson         | users.ndjson | 38.638   | 42.401      | 43.888   | 1193.1   |
| json (stdlib) | users.ndjson | 47.869   | 48.724      | 50.997   | 1157.6   |
| strata        | users.ndjson | 59.556   | 72.165      | 76.569   | 1235.5   |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.266    | 2000    |
|                                     | jmespath    | 0.757    | 2000    |
|                                     | jsonpath-ng | 9.860    | 2000    |
| $.users[*].orders[*].items[*].price | query       | 4.742    | 100999  |
|                                     | jmespath    | 39.849   | 2000    |
|                                     | jsonpath-ng | 157.947  | 100999  |
| $..price                            | jmespath    | 40.516   | 2000    |
|                                     | query       | 43.487   | 100999  |
|                                     | jsonpath-ng | 571.768  | 100999  |
| $.users[?(@.age>30)]                | query       | 0.289    | 1606    |
| $..orders[?(@.status=="shipped")]   | query       | 45.302   | 13300   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 23.35    | 34.63       | 37.62    | 189.22 | 2636.7   |
| msgspec       | 24.99    | 37.53       | 38.03    | 174.57 | 2612.4   |
| ujson         | 33.33    | 49.97       | 52.91    | 131.12 | 2575.3   |
| json (stdlib) | 41.86    | 53.15       | 53.25    | 123.27 | 2563.4   |
| strata        | 154.31   | 162.61      | 361.66   | 40.29  | 2662.2   |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 28.73    | 29.57       | 29.81    | 221.47 | 172.8    |
| msgspec       | 29.85    | 30.12       | 30.74    | 217.47 | 172.5    |
| ujson         | 39.45    | 42.18       | 43.79    | 155.26 | 190.4    |
| json (stdlib) | 47.98    | 48.99       | 52.16    | 133.70 | 173.5    |
| strata        | 53.73    | 58.17       | 58.32    | 112.60 | 171.6    |
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
| Deep path navigation                        | jmespath    | 0.02     | 0.03        | 0.03     | 255052.55 | 1       | 1485.7   |
| Deep path navigation                        | strata      | 0.02     | 0.03        | 0.04     | 218083.54 | 1       | 1485.7   |
| Deep path navigation                        | jsonpath-ng | 0.03     | 0.03        | 0.03     | 202108.87 | 1       | 1485.7   |
| Extract all user IDs                        | jmespath    | 0.82     | 0.86        | 0.90     | 7639.31   | 2000    | 554.2    |
| Extract all user IDs                        | strata      | 2.66     | 2.86        | 3.05     | 2292.24   | 2000    | 554.0    |
| Extract all user IDs                        | jsonpath-ng | 9.23     | 9.36        | 9.49     | 699.92    | 2000    | 555.3    |
| Extract all user names                      | jmespath    | 0.68     | 0.68        | 0.69     | 9619.77   | 2000    | 1588.3   |
| Extract all user names                      | strata      | 2.63     | 2.64        | 2.66     | 2480.40   | 2000    | 1588.3   |
| Extract all user names                      | jsonpath-ng | 9.18     | 9.26        | 9.35     | 707.26    | 2000    | 1588.3   |
| Extract nested timestamp field              | jmespath    | 1.49     | 1.73        | 1.97     | 3794.99   | 2000    | 986.3    |
| Extract nested timestamp field              | strata      | 3.22     | 3.24        | 3.27     | 2020.96   | 2000    | 986.2    |
| Extract nested timestamp field              | jsonpath-ng | 11.18    | 11.48       | 11.78    | 570.75    | 2000    | 987.0    |
| Extract order item prices (double wildcard) | jmespath    | 38.93    | 39.56       | 40.20    | 165.59    | 2000    | 1240.1   |
| Extract order item prices (double wildcard) | strata      | 133.38   | 134.83      | 136.27   | 48.59     | 100999  | 1238.1   |
| Extract order item prices (double wildcard) | jsonpath-ng | 156.64   | 158.92      | 161.20   | 41.23     | 100999  | 1246.2   |
| Filter users by age (numeric predicate)     | jmespath    | 2.68     | 2.75        | 2.82     | 2382.01   | 1606    | 1582.2   |
| Filter users by age (numeric predicate)     | strata      | 2.80     | 2.92        | 3.03     | 2245.87   | 1606    | 1611.6   |
| Recursively find all prices                 | strata      | 135.65   | 137.49      | 139.33   | 47.65     | 100999  | 1516.9   |
| Recursively find all prices                 | jsonpath-ng | 579.75   | 581.19      | 582.64   | 11.27     | 100999  | 1418.5   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 269943.52 | 10      | 1643.2   |
| Slice first 10 users, extract IDs           | strata      | 0.04     | 0.04        | 0.04     | 175687.89 | 10      | 1645.3   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.04        | 0.05     | 165867.01 | 10      | 1640.9   |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 417.90   | 446.46      | 475.02   | 117.40 | 1599.6   |
| strata_cursor_reparse | 1079.78  | 1135.28     | 1190.79  | 46.17  | 2720.5   |
- Speedup (reuse vs reparse): 2.54x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.03     | 0.03        | 0.03     | 211702.55 | 1       | 221.0    |
| Deep path navigation                        | strata              | 0.03     | 0.03        | 0.03     | 199608.46 | 1       | 221.0    |
| Deep path navigation                        | jsonpath-ng         | 0.03     | 0.03        | 0.04     | 187245.26 | 1       | 221.0    |
| Extract all user IDs                        | strata              | 0.69     | 0.69        | 0.69     | 9489.38   | 0       | 214.0    |
| Extract all user IDs                        | jmespath            | 1.08     | 1.09        | 1.10     | 6011.96   | 2000    | 214.1    |
| Extract all user IDs                        | jsonpath-ng         | 9.62     | 9.78        | 9.93     | 669.71    | 2000    | 214.2    |
| Extract all user IDs                        | strata_ndjson_full  | 23.15    | 23.41       | 23.68    | 279.73    | 0       | 214.1    |
| Extract all user IDs                        | strata_ndjson_fused | 23.39    | 23.52       | 23.65    | 278.51    | 0       | 214.1    |
| Extract all user names                      | strata              | 0.66     | 0.66        | 0.66     | 9894.36   | 0       | 221.3    |
| Extract all user names                      | jmespath            | 0.93     | 0.95        | 0.96     | 6927.34   | 2000    | 221.3    |
| Extract all user names                      | jsonpath-ng         | 9.53     | 9.87        | 10.22    | 663.27    | 2000    | 221.3    |
| Extract all user names                      | strata_ndjson_full  | 22.64    | 23.06       | 23.49    | 283.99    | 0       | 221.3    |
| Extract all user names                      | strata_ndjson_fused | 22.91    | 23.17       | 23.44    | 282.66    | 0       | 221.3    |
| Extract nested timestamp field              | strata              | 0.65     | 0.65        | 0.65     | 10076.07  | 0       | 214.4    |
| Extract nested timestamp field              | jmespath            | 1.86     | 1.90        | 1.93     | 3453.43   | 2000    | 214.4    |
| Extract nested timestamp field              | jsonpath-ng         | 11.83    | 12.09       | 12.36    | 541.55    | 2000    | 214.4    |
| Extract order item prices (double wildcard) | strata              | 0.71     | 0.75        | 0.79     | 8747.21   | 0       | 214.5    |
| Extract order item prices (double wildcard) | jmespath            | 31.52    | 31.52       | 31.53    | 207.78    | 100999  | 216.5    |
| Extract order item prices (double wildcard) | jsonpath-ng         | 158.28   | 160.27      | 162.26   | 40.87     | 100999  | 221.7    |
| Filter users by age (numeric predicate)     | strata              | 0.02     | 0.02        | 0.02     | 275056.85 | 0       | 221.4    |
| Filter users by age (numeric predicate)     | jmespath            | 2.88     | 2.92        | 2.96     | 2240.43   | 1606    | 221.4    |
| NDJSON root field (id)                      | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 261766.16 | 0       | 157.0    |
| NDJSON root field (id)                      | strata              | 10.44    | 10.62       | 10.79    | 616.87    | 2000    | 155.8    |
| NDJSON root field (id)                      | strata_ndjson_full  | 33.04    | 33.28       | 33.51    | 196.83    | 2000    | 156.9    |
| NDJSON root field (id)                      | strata_ndjson_fused | 33.49    | 33.51       | 33.53    | 195.45    | 2000    | 157.0    |
| Recursively find all prices                 | strata              | 154.56   | 155.18      | 155.79   | 42.21     | 1908    | 224.6    |
| Recursively find all prices                 | jsonpath-ng         | 574.69   | 575.16      | 575.63   | 11.39     | 100999  | 228.9    |
| Slice first 10 users, extract IDs           | jmespath            | 0.03     | 0.03        | 0.03     | 224718.75 | 10      | 227.9    |
| Slice first 10 users, extract IDs           | strata              | 0.04     | 0.04        | 0.04     | 165205.46 | 10      | 228.9    |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.05     | 0.05        | 0.05     | 144079.85 | 10      | 227.0    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 223.27   | 228.17      | 233.07   | 258.35 | 242.5    |
| strata_cursor_reparse | 781.12   | 782.93      | 784.74   | 75.29  | 302.8    |
- Speedup (reuse vs reparse): 3.43x
