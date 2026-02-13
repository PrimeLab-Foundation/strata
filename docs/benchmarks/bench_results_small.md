# Strata Benchmark Results

Generated: 2026-02-13 03:06:11

## Environment

- Commit: 4ec9f875bd9c5821d060084e19b1070754ee66b0
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users.json   | 2.852    | 4.244       | 4.824    | 423.7    |
| msgspec       | users.json   | 2.923    | 4.640       | 5.089    | 426.8    |
| ujson         | users.json   | 4.570    | 6.708       | 6.861    | 425.7    |
| json (stdlib) | users.json   | 5.494    | 6.806       | 7.404    | 427.8    |
| strata        | users.json   | 13.970   | 16.982      | 19.371   | 419.2    |
| orjson        | users.ndjson | 3.407    | 4.877       | 5.032    | 430.8    |
| msgspec       | users.ndjson | 3.492    | 4.962       | 5.145    | 432.9    |
| ujson         | users.ndjson | 4.853    | 7.279       | 7.328    | 432.9    |
| json (stdlib) | users.ndjson | 6.987    | 8.980       | 9.108    | 432.9    |
| strata        | users.ndjson | 9.418    | 10.896      | 10.936   | 428.9    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.032    | 1000    |
|                                     | jmespath    | 0.240    | 1000    |
|                                     | jsonpath-ng | 2.289    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.686    | 12046   |
|                                     | jmespath    | 7.031    | 1000    |
|                                     | jsonpath-ng | 19.730   | 12046   |
| $..price                            | jmespath    | 6.247    | 1000    |
|                                     | query       | 6.427    | 12046   |
|                                     | jsonpath-ng | 78.395   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.045    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 7.467    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 3.87     | 5.19        | 5.56     | 194.21 | 371.4    |
| msgspec       | 3.46     | 5.34        | 5.61     | 188.67 | 371.5    |
| json (stdlib) | 5.65     | 7.37        | 7.43     | 136.80 | 373.5    |
| ujson         | 4.92     | 8.01        | 8.49     | 125.85 | 374.5    |
| strata        | 14.12    | 14.49       | 17.61    | 69.58  | 367.6    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 2.87     | 4.10        | 4.47     | 245.79 | 36.7     |
| msgspec       | 3.05     | 4.61        | 4.61     | 218.64 | 36.8     |
| ujson         | 4.32     | 5.99        | 6.31     | 168.19 | 39.8     |
| json (stdlib) | 6.28     | 7.57        | 7.66     | 133.09 | 38.8     |
| strata        | 7.35     | 7.93        | 7.99     | 127.07 | 37.2     |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 132954.98 | 1       | 244.3    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 111249.48 | 1       | 244.3    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 100193.55 | 1       | 244.3    |
| Extract all user IDs                        | jmespath    | 0.21     | 0.21        | 0.22     | 4719.94   | 1000    | 98.1     |
| Extract all user IDs                        | strata      | 1.34     | 1.37        | 1.39     | 737.55    | 1000    | 98.1     |
| Extract all user IDs                        | jsonpath-ng | 1.84     | 1.85        | 1.87     | 543.80    | 1000    | 98.8     |
| Extract all user names                      | jmespath    | 0.21     | 0.24        | 0.27     | 4175.10   | 1000    | 276.7    |
| Extract all user names                      | strata      | 1.31     | 1.31        | 1.32     | 766.75    | 1000    | 276.7    |
| Extract all user names                      | jsonpath-ng | 1.65     | 1.65        | 1.66     | 609.22    | 1000    | 276.7    |
| Extract nested timestamp field              | jmespath    | 0.65     | 0.72        | 0.78     | 1409.66   | 1000    | 159.4    |
| Extract nested timestamp field              | strata      | 1.29     | 1.33        | 1.37     | 757.18    | 1000    | 159.4    |
| Extract nested timestamp field              | jsonpath-ng | 3.16     | 3.17        | 3.17     | 318.10    | 1000    | 159.7    |
| Extract order item prices (double wildcard) | jmespath    | 6.33     | 6.39        | 6.46     | 157.72    | 1000    | 204.8    |
| Extract order item prices (double wildcard) | strata      | 14.94    | 15.36       | 15.77    | 65.66     | 12046   | 204.3    |
| Extract order item prices (double wildcard) | jsonpath-ng | 19.57    | 19.90       | 20.23    | 50.66     | 12046   | 209.1    |
| Filter users by age (numeric predicate)     | strata      | 1.10     | 1.11        | 1.11     | 912.09    | 794     | 307.0    |
| Filter users by age (numeric predicate)     | jmespath    | 1.20     | 1.26        | 1.31     | 802.60    | 794     | 307.0    |
| Recursively find all prices                 | strata      | 17.85    | 17.91       | 17.96    | 56.31     | 12046   | 337.8    |
| Recursively find all prices                 | jsonpath-ng | 77.29    | 77.80       | 78.30    | 12.96     | 12046   | 339.4    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.02        | 0.02     | 67025.52  | 10      | 379.5    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 50357.03  | 10      | 379.5    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.02        | 0.03     | 43016.45  | 10      | 379.5    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 51.98    | 52.91       | 53.84    | 152.45 | 451.0    |
| strata_cursor_reparse | 134.88   | 137.90      | 140.91   | 58.49  | 938.6    |
- Speedup (reuse vs reparse): 2.61x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 127896.73 | 1       | 55.5     |
| Deep path navigation                        | strata              | 0.01     | 0.02        | 0.02     | 63196.06  | 1       | 55.5     |
| Deep path navigation                        | jmespath            | 0.02     | 0.02        | 0.02     | 50253.81  | 1       | 55.5     |
| Extract all user IDs                        | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 77850.22  | 0       | 54.0     |
| Extract all user IDs                        | jmespath            | 0.25     | 0.25        | 0.25     | 4022.72   | 1000    | 54.0     |
| Extract all user IDs                        | strata              | 2.79     | 2.88        | 2.97     | 350.05    | 1000    | 53.7     |
| Extract all user IDs                        | strata_ndjson_fused | 3.43     | 3.44        | 3.46     | 292.38    | 1000    | 54.0     |
| Extract all user IDs                        | strata_ndjson_full  | 10.34    | 10.49       | 10.65    | 96.01     | 1000    | 54.0     |
| Extract all user names                      | jsonpath-ng         | 0.00     | 0.01        | 0.02     | 82640.95  | 0       | 55.5     |
| Extract all user names                      | jmespath            | 0.27     | 0.27        | 0.27     | 3745.64   | 1000    | 55.5     |
| Extract all user names                      | strata              | 2.54     | 2.67        | 2.80     | 377.61    | 1000    | 55.5     |
| Extract all user names                      | strata_ndjson_fused | 3.39     | 3.47        | 3.55     | 290.14    | 1000    | 55.5     |
| Extract all user names                      | strata_ndjson_full  | 10.41    | 10.51       | 10.61    | 95.81     | 1000    | 55.5     |
| Extract nested timestamp field              | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 67614.62  | 0       | 54.2     |
| Extract nested timestamp field              | jmespath            | 0.62     | 0.67        | 0.73     | 1495.68   | 1000    | 54.2     |
| Extract nested timestamp field              | strata              | 2.52     | 2.77        | 3.01     | 364.25    | 1000    | 54.2     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 92789.76  | 0       | 55.5     |
| Extract order item prices (double wildcard) | jmespath            | 4.01     | 4.04        | 4.08     | 249.07    | 12046   | 55.5     |
| Extract order item prices (double wildcard) | strata              | 14.65    | 15.05       | 15.44    | 66.94     | 12046   | 55.0     |
| Filter users by age (numeric predicate)     | strata              | 1.11     | 1.18        | 1.24     | 856.33    | 794     | 55.5     |
| Filter users by age (numeric predicate)     | jmespath            | 1.31     | 1.43        | 1.54     | 706.40    | 794     | 55.5     |
| NDJSON first record id                      | jmespath            | 0.01     | 0.01        | 0.02     | 80843.37  | 1       | 44.5     |
| NDJSON first record id                      | strata              | 0.01     | 0.02        | 0.02     | 57899.32  | 1       | 44.5     |
| NDJSON first record id                      | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 57143.75  | 1       | 44.8     |
| Recursively find all prices                 | strata              | 16.74    | 23.18       | 29.61    | 43.46     | 12046   | 55.7     |
| Recursively find all prices                 | jsonpath-ng         | 78.91    | 81.98       | 85.06    | 12.29     | 12046   | 57.6     |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.02        | 0.03     | 50835.92  | 10      | 57.7     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.03     | 0.03        | 0.03     | 34433.74  | 10      | 56.8     |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.03        | 0.04     | 32101.07  | 10      | 57.7     |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 46.81    | 47.87       | 48.93    | 189.36 | 56.8     |
| strata_cursor_reparse | 126.32   | 127.05      | 127.79   | 71.34  | 65.2     |
- Speedup (reuse vs reparse): 2.65x
