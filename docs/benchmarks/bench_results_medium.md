# Strata Benchmark Results

Generated: 2026-02-09 03:18:54

## Environment

- Commit: 29338dbc6a83118ecc29eecf4ac54f0879da7dca
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/medium/users.json, benchmarks/data/generated/medium/users.ndjson

## Parsing Benchmarks

| Library | Dataset | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|---------|----------|-------------|----------|---------|
| orjson | users.json | 18.295 | 26.393 | 27.243 | 123.5 |
| msgspec | users.json | 19.463 | 28.289 | 28.898 | 123.3 |
| strata | users.json | 27.964 | 32.014 | 32.404 | 103.2 |
| ujson | users.json | 27.212 | 39.441 | 40.223 | 142.0 |
| json (stdlib) | users.json | 34.546 | 43.435 | 44.262 | 123.3 |
| orjson | users.ndjson | 22.059 | 23.743 | 24.652 | 132.8 |
| msgspec | users.ndjson | 22.372 | 24.667 | 25.624 | 132.5 |
| ujson | users.ndjson | 30.660 | 35.208 | 36.144 | 151.2 |
| strata | users.ndjson | 30.146 | 39.359 | 40.673 | 127.4 |
| json (stdlib) | users.ndjson | 38.490 | 41.508 | 41.752 | 134.9 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | eval_query (baseline) | 0.112 | 2000 |
|  | jmespath | 0.415 | 2000 |
|  | jsonpath-ng | 6.099 | 2000 |
| $.users[*].orders[*].items[*].price | eval_query (baseline) | 3.892 | 100999 |
|  | jmespath | 33.650 | 2000 |
|  | jsonpath-ng | 135.493 | 100999 |
| $..price | jmespath | 33.269 | 2000 |
|  | eval_query (baseline) | 37.999 | 100999 |
|  | jsonpath-ng | 505.208 | 100999 |
| $.users[?(@.age>30)] | eval_query (baseline) | 0.116 | 1606 |
| $..orders[?(@.status=="shipped")] | eval_query (baseline) | 40.192 | 13300 |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 18.21 | 26.64 | 27.10 | 124.9 |
| msgspec | 19.93 | 28.48 | 29.09 | 123.6 |
| strata | 28.59 | 32.27 | 33.18 | 103.6 |
| ujson | 27.93 | 39.34 | 41.05 | 141.4 |
| json (stdlib) | 34.92 | 44.43 | 44.60 | 123.7 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 22.21 | 24.32 | 24.33 | 151.0 |
| msgspec | 24.14 | 25.16 | 25.53 | 149.8 |
| ujson | 31.77 | 35.45 | 35.99 | 167.5 |
| json (stdlib) | 39.73 | 41.59 | 42.10 | 150.7 |
| strata | 51.28 | 51.42 | 53.37 | 150.8 |
## Search Benchmarks (JSONPath)

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | strata | 0.01 | 0.01 | 0.01 | 1 | 254.7 |
| Deep path navigation | jmespath | 0.01 | 0.02 | 0.02 | 1 | 252.7 |
| Deep path navigation | jsonpath-ng | 0.01 | 0.02 | 0.02 | 1 | 252.7 |
| Extract all user IDs | jmespath | 0.46 | 0.49 | 0.52 | 2000 | 172.0 |
| Extract all user IDs | strata | 1.55 | 1.55 | 1.55 | 2000 | 171.8 |
| Extract all user IDs | jsonpath-ng | 6.20 | 6.32 | 6.44 | 2000 | 173.1 |
| Extract all user names | jmespath | 0.43 | 0.46 | 0.48 | 2000 | 252.7 |
| Extract all user names | strata | 1.94 | 2.22 | 2.51 | 2000 | 252.7 |
| Extract all user names | jsonpath-ng | 6.12 | 6.27 | 6.41 | 2000 | 252.7 |
| Extract nested timestamp field | jmespath | 1.15 | 1.18 | 1.21 | 2000 | 228.1 |
| Extract nested timestamp field | strata | 1.73 | 1.75 | 1.76 | 2000 | 228.1 |
| Extract nested timestamp field | jsonpath-ng | 7.38 | 7.60 | 7.81 | 2000 | 228.7 |
| Extract order item prices (double wildcard) | jmespath | 33.89 | 34.20 | 34.51 | 2000 | 253.1 |
| Extract order item prices (double wildcard) | strata | 84.30 | 88.52 | 92.74 | 100999 | 250.7 |
| Extract order item prices (double wildcard) | jsonpath-ng | 134.19 | 134.45 | 134.71 | 100999 | 257.0 |
| Filter users by age (numeric predicate) | jsonpath-ng | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | strata | 1.95 | 2.04 | 2.12 | 1606 | 252.8 |
| Filter users by age (numeric predicate) | jmespath | 2.36 | 2.38 | 2.39 | 1606 | 252.8 |
| Recursively find all prices | strata | 162.35 | 164.07 | 165.79 | 100999 | 257.5 |
| Recursively find all prices | jsonpath-ng | 501.22 | 503.73 | 506.23 | 100999 | 260.2 |
| Slice first 10 users, extract IDs | jmespath | 0.02 | 0.02 | 0.02 | 10 | 258.6 |
| Slice first 10 users, extract IDs | strata | 0.02 | 0.02 | 0.03 | 10 | 258.6 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.03 | 0.03 | 0.03 | 10 | 258.6 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | strata | ERROR | - | - | - | - |
| Deep path navigation | jmespath | 0.01 | 0.01 | 0.01 | 1 | 167.0 |
| Deep path navigation | jsonpath-ng | 0.02 | 0.03 | 0.03 | 1 | 167.0 |
| Extract all user IDs | strata | ERROR | - | - | - | - |
| Extract all user IDs | jmespath | 0.95 | 0.96 | 0.98 | 2000 | 155.6 |
| Extract all user IDs | jsonpath-ng | 12.28 | 12.38 | 12.49 | 2000 | 156.7 |
| Extract all user names | strata | ERROR | - | - | - | - |
| Extract all user names | jmespath | 0.93 | 0.97 | 1.00 | 2000 | 170.9 |
| Extract all user names | jsonpath-ng | 14.35 | 15.17 | 15.99 | 2000 | 171.0 |
| Extract nested timestamp field | strata | ERROR | - | - | - | - |
| Extract nested timestamp field | jmespath | 1.39 | 1.53 | 1.66 | 2000 | 159.6 |
| Extract nested timestamp field | jsonpath-ng | 13.41 | 13.55 | 13.70 | 2000 | 159.6 |
| Extract order item prices (double wildcard) | strata | ERROR | - | - | - | - |
| Extract order item prices (double wildcard) | jmespath | 26.81 | 27.62 | 28.42 | 100999 | 163.6 |
| Extract order item prices (double wildcard) | jsonpath-ng | 151.24 | 154.56 | 157.88 | 100999 | 168.5 |
| Filter users by age (numeric predicate) | strata | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | jsonpath-ng | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | jmespath | 2.57 | 2.62 | 2.66 | 1606 | 170.0 |
| Recursively find all prices | strata | ERROR | - | - | - | - |
| Recursively find all prices | jsonpath-ng | 549.10 | 553.16 | 557.23 | 100999 | 174.8 |
| Slice first 10 users, extract IDs | strata | ERROR | - | - | - | - |
| Slice first 10 users, extract IDs | jmespath | 0.02 | 0.02 | 0.02 | 10 | 173.1 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.03 | 0.05 | 0.06 | 10 | 173.1 |
