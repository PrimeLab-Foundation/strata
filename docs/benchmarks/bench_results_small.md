# Strata Benchmark Results

Generated: 2026-02-14 14:46:49

## Environment

- Commit: 1a605860e0960662442cc11396164b9e31b0f22e
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
| orjson        | users.json   | 2.453    | 3.819       | 4.336    | 398.9    |
| msgspec       | users.json   | 2.869    | 3.930       | 3.965    | 402.0    |
| ujson         | users.json   | 4.077    | 6.140       | 6.210    | 400.9    |
| json (stdlib) | users.json   | 5.599    | 6.249       | 6.793    | 403.0    |
| strata        | users.json   | 11.715   | 14.966      | 18.172   | 394.5    |
| orjson        | users.ndjson | 3.071    | 3.853       | 3.979    | 406.1    |
| msgspec       | users.ndjson | 3.205    | 4.418       | 4.525    | 408.1    |
| ujson         | users.ndjson | 4.847    | 6.463       | 6.852    | 408.1    |
| json (stdlib) | users.ndjson | 6.337    | 7.588       | 7.635    | 408.1    |
| strata        | users.ndjson | 8.061    | 10.046      | 10.152   | 404.2    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.030    | 1000    |
|                                     | jmespath    | 0.189    | 1000    |
|                                     | jsonpath-ng | 1.710    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.593    | 12046   |
|                                     | jmespath    | 5.531    | 1000    |
|                                     | jsonpath-ng | 18.214   | 12046   |
| $..price                            | jmespath    | 6.057    | 1000    |
|                                     | query       | 6.216    | 12046   |
|                                     | jsonpath-ng | 75.755   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.034    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.543    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 2.82     | 3.84        | 4.84     | 262.84 | 377.8    |
| orjson        | 2.96     | 4.14        | 4.58     | 243.23 | 377.7    |
| ujson         | 4.37     | 5.89        | 6.74     | 171.13 | 380.9    |
| json (stdlib) | 5.61     | 6.63        | 7.13     | 152.14 | 379.9    |
| strata        | 13.66    | 13.79       | 17.86    | 73.11  | 373.9    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| msgspec       | 2.77     | 3.90        | 4.07     | 258.39 | 36.6     |
| orjson        | 2.71     | 3.93        | 4.03     | 256.19 | 36.5     |
| ujson         | 4.24     | 6.18        | 6.26     | 162.93 | 39.6     |
| json (stdlib) | 5.92     | 7.08        | 7.37     | 142.17 | 38.6     |
| strata        | 6.93     | 7.52        | 7.67     | 133.90 | 37.0     |
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
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 131859.56 | 1       | 242.8    |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 89118.47  | 1       | 242.8    |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 82023.95  | 1       | 242.8    |
| Extract all user IDs                        | jmespath    | 0.21     | 0.23        | 0.24     | 4431.64   | 1000    | 98.1     |
| Extract all user IDs                        | strata      | 1.26     | 1.27        | 1.28     | 792.79    | 1000    | 98.0     |
| Extract all user IDs                        | jsonpath-ng | 1.77     | 2.12        | 2.48     | 474.84    | 1000    | 98.8     |
| Extract all user names                      | jmespath    | 0.21     | 0.21        | 0.21     | 4795.71   | 1000    | 276.9    |
| Extract all user names                      | strata      | 1.27     | 1.31        | 1.34     | 771.94    | 1000    | 276.9    |
| Extract all user names                      | jsonpath-ng | 1.56     | 1.68        | 1.80     | 601.03    | 1000    | 275.9    |
| Extract nested timestamp field              | jmespath    | 0.53     | 0.54        | 0.56     | 1857.15   | 1000    | 159.3    |
| Extract nested timestamp field              | strata      | 1.45     | 1.52        | 1.58     | 664.55    | 1000    | 159.2    |
| Extract nested timestamp field              | jsonpath-ng | 2.20     | 2.38        | 2.57     | 423.08    | 1000    | 159.7    |
| Extract order item prices (double wildcard) | jmespath    | 5.72     | 5.93        | 6.14     | 169.96    | 1000    | 202.5    |
| Extract order item prices (double wildcard) | strata      | 15.18    | 15.31       | 15.43    | 65.86     | 12046   | 202.0    |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.30    | 18.45       | 18.60    | 54.64     | 12046   | 206.8    |
| Filter users by age (numeric predicate)     | strata      | 1.15     | 1.15        | 1.15     | 876.28    | 794     | 303.0    |
| Filter users by age (numeric predicate)     | jmespath    | 1.12     | 1.18        | 1.24     | 856.08    | 794     | 303.0    |
| Recursively find all prices                 | strata      | 16.39    | 16.57       | 16.74    | 60.86     | 12046   | 333.4    |
| Recursively find all prices                 | jsonpath-ng | 74.78    | 75.69       | 76.60    | 13.32     | 12046   | 335.1    |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 121287.12 | 10      | 357.1    |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.01        | 0.02     | 73435.62  | 10      | 357.1    |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.03        | 0.03     | 39569.75  | 10      | 357.1    |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 43.55    | 44.35       | 45.15    | 181.88 | 401.3    |
| strata_cursor_reparse | 123.60   | 126.30      | 129.00   | 63.86  | 869.7    |
- Speedup (reuse vs reparse): 2.85x
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library             | Min (ms) | Median (ms) | P95 (ms) | MB/s      | Results | RSS (MB) |
| ------------------------------------------- | ------------------- | -------- | ----------- | -------- | --------- | ------- | -------- |
| Deep path navigation                        | jmespath            | 0.01     | 0.01        | 0.01     | 97865.84  | 1       | 55.1     |
| Deep path navigation                        | strata              | 0.00     | 0.01        | 0.02     | 88543.92  | 1       | 55.1     |
| Deep path navigation                        | jsonpath-ng         | 0.01     | 0.02        | 0.02     | 60811.31  | 1       | 55.1     |
| Extract all user IDs                        | jsonpath-ng         | 0.00     | 0.01        | 0.02     | 91045.18  | 0       | 53.8     |
| Extract all user IDs                        | jmespath            | 0.23     | 0.26        | 0.29     | 3900.05   | 1000    | 53.8     |
| Extract all user IDs                        | strata              | 2.14     | 2.26        | 2.39     | 445.07    | 1000    | 53.4     |
| Extract all user IDs                        | strata_ndjson_fused | 3.44     | 3.50        | 3.57     | 287.56    | 1000    | 53.8     |
| Extract all user IDs                        | strata_ndjson_full  | 8.92     | 9.12        | 9.33     | 110.40    | 1000    | 53.8     |
| Extract all user names                      | jsonpath-ng         | 0.00     | 0.00        | 0.01     | 217746.58 | 0       | 55.1     |
| Extract all user names                      | jmespath            | 0.24     | 0.25        | 0.25     | 4064.66   | 1000    | 55.1     |
| Extract all user names                      | strata              | 2.24     | 2.37        | 2.50     | 424.82    | 1000    | 55.1     |
| Extract all user names                      | strata_ndjson_fused | 2.85     | 3.18        | 3.52     | 316.44    | 1000    | 55.1     |
| Extract all user names                      | strata_ndjson_full  | 9.22     | 9.56        | 9.89     | 105.40    | 1000    | 55.1     |
| Extract nested timestamp field              | jsonpath-ng         | 0.02     | 0.02        | 0.02     | 51486.89  | 0       | 53.9     |
| Extract nested timestamp field              | jmespath            | 0.61     | 0.67        | 0.73     | 1501.91   | 1000    | 53.9     |
| Extract nested timestamp field              | strata              | 2.30     | 2.65        | 3.01     | 379.46    | 1000    | 53.9     |
| Extract order item prices (double wildcard) | jsonpath-ng         | 0.01     | 0.01        | 0.01     | 103079.37 | 0       | 55.1     |
| Extract order item prices (double wildcard) | jmespath            | 4.01     | 4.11        | 4.22     | 244.99    | 12046   | 55.1     |
| Extract order item prices (double wildcard) | strata              | 13.38    | 14.21       | 15.05    | 70.86     | 12046   | 54.7     |
| Filter users by age (numeric predicate)     | strata              | 0.96     | 0.99        | 1.02     | 1017.53   | 794     | 55.1     |
| Filter users by age (numeric predicate)     | jmespath            | 1.21     | 1.23        | 1.26     | 817.99    | 794     | 55.1     |
| NDJSON first record id                      | jmespath            | 0.00     | 0.01        | 0.01     | 121472.35 | 1       | 44.3     |
| NDJSON first record id                      | strata              | 0.01     | 0.01        | 0.02     | 92969.73  | 1       | 44.3     |
| NDJSON first record id                      | jsonpath-ng         | 0.01     | 0.01        | 0.02     | 70991.12  | 1       | 44.6     |
| Recursively find all prices                 | strata              | 14.24    | 14.88       | 15.53    | 67.67     | 12046   | 55.3     |
| Recursively find all prices                 | jsonpath-ng         | 74.46    | 74.58       | 74.70    | 13.50     | 12046   | 57.2     |
| Slice first 10 users, extract IDs           | jmespath            | 0.01     | 0.01        | 0.02     | 68090.01  | 10      | 57.2     |
| Slice first 10 users, extract IDs           | strata              | 0.02     | 0.02        | 0.02     | 50306.52  | 10      | 57.2     |
| Slice first 10 users, extract IDs           | jsonpath-ng         | 0.02     | 0.03        | 0.03     | 37447.49  | 10      | 56.3     |

#### Cursor Reuse (All Queries)

| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 42.52    | 43.86       | 45.19    | 206.69 | 56.5     |
| strata_cursor_reparse | 117.82   | 118.99      | 120.16   | 76.18  | 64.8     |
- Speedup (reuse vs reparse): 2.71x
