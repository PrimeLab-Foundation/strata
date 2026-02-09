# Strata Benchmark Results

Generated: 2026-02-09 03:19:49

## Environment

- Commit: 29338dbc6a83118ecc29eecf4ac54f0879da7dca
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
| orjson | users.json | 137.710 | 186.606 | 192.670 | 712.1 |
| msgspec | users.json | 144.999 | 208.364 | 217.838 | 704.5 |
| strata | users.json | 199.610 | 231.648 | 239.129 | 566.2 |
| ujson | users.json | 204.707 | 291.849 | 295.376 | 826.9 |
| json (stdlib) | users.json | 261.437 | 331.205 | 332.454 | 704.5 |
| orjson | users.ndjson | 175.868 | 212.854 | 213.908 | 578.2 |
| msgspec | users.ndjson | 183.544 | 214.080 | 222.219 | 573.6 |
| strata | users.ndjson | 235.545 | 272.231 | 276.964 | 526.0 |
| ujson | users.ndjson | 254.777 | 312.156 | 313.861 | 697.1 |
| json (stdlib) | users.ndjson | 297.748 | 333.235 | 336.395 | 576.3 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | eval_query (baseline) | 0.693 | 4000 |
|  | jmespath | 1.764 | 4000 |
|  | jsonpath-ng | 38.569 | 4000 |
| $.users[*].orders[*].items[*].price | eval_query (baseline) | 31.378 | 789913 |
|  | jmespath | 238.352 | 4000 |
|  | jsonpath-ng | 1103.509 | 789913 |
| $..price | jmespath | 241.246 | 4000 |
|  | eval_query (baseline) | 274.630 | 789913 |
|  | jsonpath-ng | 3824.804 | 789913 |
| $.users[?(@.age>30)] | eval_query (baseline) | 0.573 | 3212 |
| $..orders[?(@.status=="shipped")] | eval_query (baseline) | 286.099 | 52812 |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 123.49 | 177.97 | 180.92 | 712.3 |
| msgspec | 136.89 | 185.42 | 186.39 | 703.6 |
| strata | 199.00 | 222.47 | 226.65 | 565.4 |
| ujson | 187.36 | 261.76 | 267.13 | 826.2 |
| json (stdlib) | 243.24 | 312.71 | 328.66 | 703.8 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 167.79 | 208.17 | 208.98 | 879.9 |
| msgspec | 177.40 | 214.05 | 218.66 | 870.2 |
| strata | 223.92 | 226.09 | 227.29 | 878.6 |
| ujson | 233.85 | 295.78 | 300.42 | 992.8 |
| json (stdlib) | 291.79 | 324.16 | 343.35 | 873.1 |
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
| Deep path navigation | strata | 0.01 | 0.01 | 0.02 | 1 | 1606.7 |
| Deep path navigation | jsonpath-ng | 0.01 | 0.01 | 0.02 | 1 | 1584.7 |
| Deep path navigation | jmespath | 0.01 | 0.02 | 0.02 | 1 | 1584.7 |
| Extract all user IDs | jmespath | 1.68 | 1.72 | 1.77 | 4000 | 1059.6 |
| Extract all user IDs | strata | 3.55 | 3.56 | 3.57 | 4000 | 1059.2 |
| Extract all user IDs | jsonpath-ng | 31.94 | 32.14 | 32.34 | 4000 | 1061.6 |
| Extract all user names | jmespath | 1.24 | 1.24 | 1.25 | 4000 | 1584.8 |
| Extract all user names | strata | 3.63 | 3.63 | 3.63 | 4000 | 1584.8 |
| Extract all user names | jsonpath-ng | 31.74 | 31.77 | 31.81 | 4000 | 1584.8 |
| Extract nested timestamp field | jmespath | 2.52 | 2.54 | 2.56 | 4000 | 1411.5 |
| Extract nested timestamp field | strata | 3.59 | 3.60 | 3.61 | 4000 | 1411.5 |
| Extract nested timestamp field | jsonpath-ng | 34.32 | 34.43 | 34.54 | 4000 | 1412.6 |
| Extract order item prices (double wildcard) | jmespath | 215.87 | 219.85 | 223.82 | 4000 | 1598.1 |
| Extract order item prices (double wildcard) | strata | 746.07 | 748.94 | 751.81 | 789913 | 1584.4 |
| Extract order item prices (double wildcard) | jsonpath-ng | 1079.74 | 1080.56 | 1081.38 | 789913 | 1616.5 |
| Filter users by age (numeric predicate) | jsonpath-ng | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | strata | 2.63 | 2.64 | 2.65 | 3212 | 1584.9 |
| Filter users by age (numeric predicate) | jmespath | 4.82 | 4.83 | 4.83 | 3212 | 1584.9 |
| Recursively find all prices | strata | 1056.31 | 1061.05 | 1065.79 | 789913 | 1624.7 |
| Recursively find all prices | jsonpath-ng | 3724.83 | 3764.82 | 3804.80 | 789913 | 1629.1 |
| Slice first 10 users, extract IDs | jmespath | 0.01 | 0.01 | 0.02 | 10 | 1615.7 |
| Slice first 10 users, extract IDs | strata | 0.02 | 0.02 | 0.02 | 10 | 1631.7 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.02 | 0.02 | 0.02 | 10 | 1614.7 |
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
| Deep path navigation | jmespath | 0.01 | 0.02 | 0.02 | 1 | 954.5 |
| Deep path navigation | jsonpath-ng | 0.03 | 0.03 | 0.03 | 1 | 954.5 |
| Extract all user IDs | strata | ERROR | - | - | - | - |
| Extract all user IDs | jmespath | 1.95 | 1.96 | 1.97 | 4000 | 926.9 |
| Extract all user IDs | jsonpath-ng | 61.02 | 61.30 | 61.57 | 4000 | 928.8 |
| Extract all user names | strata | ERROR | - | - | - | - |
| Extract all user names | jmespath | 2.01 | 2.14 | 2.26 | 4000 | 958.4 |
| Extract all user names | jsonpath-ng | 65.08 | 66.79 | 68.51 | 4000 | 958.9 |
| Extract nested timestamp field | strata | ERROR | - | - | - | - |
| Extract nested timestamp field | jmespath | 3.08 | 3.09 | 3.10 | 4000 | 931.6 |
| Extract nested timestamp field | jsonpath-ng | 66.84 | 67.41 | 67.97 | 4000 | 932.0 |
| Extract order item prices (double wildcard) | strata | ERROR | - | - | - | - |
| Extract order item prices (double wildcard) | jmespath | 181.09 | 183.40 | 185.72 | 789913 | 942.4 |
| Extract order item prices (double wildcard) | jsonpath-ng | 1019.74 | 1024.13 | 1028.52 | 789913 | 952.4 |
| Filter users by age (numeric predicate) | strata | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | jsonpath-ng | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | jmespath | 5.27 | 5.65 | 6.02 | 3212 | 958.6 |
| Recursively find all prices | strata | ERROR | - | - | - | - |
| Recursively find all prices | jsonpath-ng | 4024.48 | 4033.99 | 4043.50 | 789913 | 962.7 |
| Slice first 10 users, extract IDs | strata | ERROR | - | - | - | - |
| Slice first 10 users, extract IDs | jmespath | 0.02 | 0.03 | 0.04 | 10 | 963.6 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.03 | 0.03 | 0.03 | 10 | 963.6 |
