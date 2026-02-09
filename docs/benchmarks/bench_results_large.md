# Strata Benchmark Results

Generated: 2026-02-09 03:09:22

## Environment

- Commit: 886c0c601fb9cef6f1f2cd22d687abc4970b6902
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/large/users.json, benchmarks/data/generated/large/users.ndjson

## Parsing Benchmarks

| Library | Dataset | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|---------|----------|-------------|----------|---------|
| orjson | users.json | 136.948 | 183.281 | 186.967 | 711.9 |
| msgspec | users.json | 130.645 | 188.393 | 192.542 | 704.3 |
| strata | users.json | 220.243 | 251.917 | 252.623 | 566.0 |
| ujson | users.json | 185.261 | 263.994 | 265.031 | 826.7 |
| json (stdlib) | users.json | 245.513 | 301.820 | 313.920 | 704.3 |
| orjson | users.ndjson | 172.574 | 209.762 | 214.017 | 576.1 |
| msgspec | users.ndjson | 179.044 | 213.246 | 217.875 | 571.5 |
| strata | users.ndjson | 239.015 | 272.581 | 273.523 | 524.0 |
| ujson | users.ndjson | 251.825 | 298.673 | 304.607 | 695.0 |
| json (stdlib) | users.ndjson | 292.914 | 332.351 | 340.117 | 574.2 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | eval_query (baseline) | 0.592 | 4000 |
|  | jmespath | 1.655 | 4000 |
|  | jsonpath-ng | 33.194 | 4000 |
| $.users[*].orders[*].items[*].price | eval_query (baseline) | 28.558 | 789913 |
|  | jmespath | 221.433 | 4000 |
|  | jsonpath-ng | 1052.875 | 789913 |
| $..price | jmespath | 221.606 | 4000 |
|  | eval_query (baseline) | 272.843 | 789913 |
|  | jsonpath-ng | 3792.040 | 789913 |
| $.users[?(@.age>30)] | eval_query (baseline) | 0.579 | 3212 |
| $..orders[?(@.status=="shipped")] | eval_query (baseline) | 290.080 | 52812 |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 125.34 | 177.65 | 180.76 | 712.3 |
| msgspec | 136.50 | 193.69 | 196.93 | 703.7 |
| strata | 204.60 | 233.13 | 235.12 | 565.4 |
| ujson | 190.10 | 263.55 | 269.74 | 826.2 |
| json (stdlib) | 251.53 | 310.75 | 312.23 | 703.7 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 173.33 | 208.26 | 212.90 | 880.1 |
| msgspec | 183.66 | 215.18 | 215.68 | 870.4 |
| strata | 222.66 | 232.71 | 238.53 | 878.7 |
| ujson | 239.27 | 304.83 | 311.93 | 993.0 |
| json (stdlib) | 297.27 | 337.25 | 344.42 | 873.2 |
## Search Benchmarks (JSONPath)

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | strata | 0.01 | 0.01 | 0.02 | 1 | 1606.2 |
| Deep path navigation | jmespath | 0.02 | 0.02 | 0.03 | 1 | 1584.2 |
| Deep path navigation | jsonpath-ng | 0.02 | 0.05 | 0.08 | 1 | 1584.2 |
| Extract all user IDs | jmespath | 1.71 | 1.76 | 1.81 | 4000 | 1059.2 |
| Extract all user IDs | strata | 3.67 | 3.78 | 3.89 | 4000 | 1058.9 |
| Extract all user IDs | jsonpath-ng | 32.31 | 33.51 | 34.71 | 4000 | 1061.1 |
| Extract all user names | jmespath | 1.29 | 1.31 | 1.32 | 4000 | 1584.4 |
| Extract all user names | strata | 3.78 | 3.92 | 4.06 | 4000 | 1584.4 |
| Extract all user names | jsonpath-ng | 32.38 | 32.39 | 32.41 | 4000 | 1584.4 |
| Extract nested timestamp field | jmespath | 3.03 | 3.03 | 3.03 | 4000 | 1410.9 |
| Extract nested timestamp field | strata | 4.14 | 4.20 | 4.26 | 4000 | 1410.9 |
| Extract nested timestamp field | jsonpath-ng | 34.94 | 35.13 | 35.32 | 4000 | 1412.0 |
| Extract order item prices (double wildcard) | jmespath | 224.17 | 225.38 | 226.58 | 4000 | 1600.0 |
| Extract order item prices (double wildcard) | strata | 772.96 | 775.14 | 777.31 | 789913 | 1586.3 |
| Extract order item prices (double wildcard) | jsonpath-ng | 1126.38 | 1132.91 | 1139.43 | 789913 | 1616.0 |
| Filter users by age (numeric predicate) | jsonpath-ng | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | strata | 3.04 | 3.08 | 3.12 | 3212 | 1584.4 |
| Filter users by age (numeric predicate) | jmespath | 4.90 | 4.92 | 4.94 | 3212 | 1584.5 |
| Recursively find all prices | strata | 1073.84 | 1074.55 | 1075.26 | 789913 | 1624.3 |
| Recursively find all prices | jsonpath-ng | 3841.77 | 3842.09 | 3842.41 | 789913 | 1628.7 |
| Slice first 10 users, extract IDs | jmespath | 0.01 | 0.02 | 0.02 | 10 | 1615.2 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.02 | 0.02 | 0.03 | 10 | 1614.2 |
| Slice first 10 users, extract IDs | strata | 0.02 | 0.02 | 0.03 | 10 | 1631.2 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | strata | ERROR | - | - | - | - |
| Deep path navigation | jsonpath-ng | 0.02 | 0.02 | 0.02 | 1 | 954.6 |
| Deep path navigation | jmespath | 0.02 | 0.02 | 0.03 | 1 | 954.6 |
| Extract all user IDs | strata | ERROR | - | - | - | - |
| Extract all user IDs | jmespath | 2.04 | 2.08 | 2.12 | 4000 | 926.9 |
| Extract all user IDs | jsonpath-ng | 63.18 | 63.42 | 63.65 | 4000 | 928.8 |
| Extract all user names | strata | ERROR | - | - | - | - |
| Extract all user names | jmespath | 2.04 | 2.09 | 2.13 | 4000 | 959.5 |
| Extract all user names | jsonpath-ng | 63.77 | 65.81 | 67.84 | 4000 | 960.0 |
| Extract nested timestamp field | strata | ERROR | - | - | - | - |
| Extract nested timestamp field | jmespath | 3.14 | 3.18 | 3.21 | 4000 | 931.6 |
| Extract nested timestamp field | jsonpath-ng | 64.27 | 64.29 | 64.32 | 4000 | 932.0 |
| Extract order item prices (double wildcard) | strata | ERROR | - | - | - | - |
| Extract order item prices (double wildcard) | jmespath | 181.38 | 183.14 | 184.89 | 789913 | 942.1 |
| Extract order item prices (double wildcard) | jsonpath-ng | 1004.91 | 1010.02 | 1015.13 | 789913 | 952.5 |
| Filter users by age (numeric predicate) | strata | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | jsonpath-ng | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | jmespath | 5.57 | 5.62 | 5.68 | 3212 | 958.7 |
| Recursively find all prices | strata | ERROR | - | - | - | - |
| Recursively find all prices | jsonpath-ng | 3787.55 | 3791.99 | 3796.43 | 789913 | 963.8 |
| Slice first 10 users, extract IDs | strata | ERROR | - | - | - | - |
| Slice first 10 users, extract IDs | jmespath | 0.02 | 0.02 | 0.02 | 10 | 963.7 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.03 | 0.03 | 0.03 | 10 | 963.7 |
