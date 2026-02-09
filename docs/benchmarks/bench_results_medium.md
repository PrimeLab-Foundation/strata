# Strata Benchmark Results

Generated: 2026-02-09 03:08:25

## Environment

- Commit: 886c0c601fb9cef6f1f2cd22d687abc4970b6902
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
| orjson | users.json | 21.353 | 30.913 | 31.997 | 123.6 |
| msgspec | users.json | 21.663 | 33.821 | 35.090 | 123.4 |
| strata | users.json | 31.132 | 37.051 | 37.453 | 103.3 |
| ujson | users.json | 31.117 | 45.601 | 45.884 | 142.0 |
| json (stdlib) | users.json | 39.953 | 50.717 | 52.465 | 123.4 |
| orjson | users.ndjson | 25.085 | 26.657 | 27.343 | 136.6 |
| msgspec | users.ndjson | 25.524 | 26.928 | 30.082 | 136.3 |
| ujson | users.ndjson | 36.138 | 40.119 | 40.577 | 155.0 |
| strata | users.ndjson | 35.491 | 45.979 | 47.950 | 131.2 |
| json (stdlib) | users.ndjson | 43.134 | 45.997 | 47.201 | 138.8 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | eval_query (baseline) | 0.252 | 2000 |
|  | jmespath | 0.637 | 2000 |
|  | jsonpath-ng | 8.314 | 2000 |
| $.users[*].orders[*].items[*].price | eval_query (baseline) | 4.851 | 100999 |
|  | jmespath | 38.305 | 2000 |
|  | jsonpath-ng | 158.247 | 100999 |
| $..price | jmespath | 39.377 | 2000 |
|  | eval_query (baseline) | 42.455 | 100999 |
|  | jsonpath-ng | 579.347 | 100999 |
| $.users[?(@.age>30)] | eval_query (baseline) | 0.221 | 1606 |
| $..orders[?(@.status=="shipped")] | eval_query (baseline) | 45.444 | 13300 |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 20.50 | 29.82 | 30.95 | 125.0 |
| msgspec | 21.51 | 32.01 | 32.68 | 123.8 |
| strata | 32.22 | 36.57 | 36.87 | 103.7 |
| ujson | 31.18 | 45.38 | 46.30 | 141.5 |
| json (stdlib) | 38.88 | 49.75 | 52.17 | 123.8 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 25.00 | 27.29 | 28.02 | 170.7 |
| msgspec | 27.06 | 29.04 | 29.69 | 169.4 |
| ujson | 36.30 | 41.58 | 42.48 | 187.2 |
| json (stdlib) | 47.70 | 47.99 | 48.23 | 170.3 |
| strata | 59.54 | 61.62 | 63.53 | 170.5 |
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
| Deep path navigation | strata | 0.02 | 0.02 | 0.02 | 1 | 255.0 |
| Deep path navigation | jmespath | 0.02 | 0.02 | 0.02 | 1 | 253.0 |
| Deep path navigation | jsonpath-ng | 0.02 | 0.02 | 0.02 | 1 | 253.0 |
| Extract all user IDs | jmespath | 0.84 | 0.85 | 0.87 | 2000 | 172.4 |
| Extract all user IDs | strata | 2.22 | 2.30 | 2.38 | 2000 | 172.2 |
| Extract all user IDs | jsonpath-ng | 8.64 | 8.98 | 9.32 | 2000 | 173.7 |
| Extract all user names | jmespath | 0.59 | 0.63 | 0.66 | 2000 | 253.0 |
| Extract all user names | strata | 2.29 | 2.30 | 2.31 | 2000 | 253.0 |
| Extract all user names | jsonpath-ng | 8.09 | 8.18 | 8.27 | 2000 | 253.0 |
| Extract nested timestamp field | jmespath | 1.34 | 1.43 | 1.52 | 2000 | 228.7 |
| Extract nested timestamp field | strata | 2.42 | 2.47 | 2.51 | 2000 | 228.7 |
| Extract nested timestamp field | jsonpath-ng | 9.71 | 9.98 | 10.25 | 2000 | 229.3 |
| Extract order item prices (double wildcard) | jmespath | 39.90 | 40.22 | 40.53 | 2000 | 254.9 |
| Extract order item prices (double wildcard) | strata | 109.65 | 111.62 | 113.59 | 100999 | 252.5 |
| Extract order item prices (double wildcard) | jsonpath-ng | 157.65 | 158.45 | 159.26 | 100999 | 257.3 |
| Filter users by age (numeric predicate) | jsonpath-ng | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | strata | 1.87 | 1.90 | 1.93 | 1606 | 253.1 |
| Filter users by age (numeric predicate) | jmespath | 2.50 | 2.55 | 2.61 | 1606 | 253.1 |
| Recursively find all prices | strata | 189.61 | 190.53 | 191.45 | 100999 | 257.8 |
| Recursively find all prices | jsonpath-ng | 581.05 | 581.36 | 581.68 | 100999 | 261.0 |
| Slice first 10 users, extract IDs | jmespath | 0.02 | 0.02 | 0.02 | 10 | 259.4 |
| Slice first 10 users, extract IDs | strata | 0.02 | 0.03 | 0.03 | 10 | 259.4 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.04 | 0.05 | 0.05 | 10 | 259.4 |
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
| Deep path navigation | jmespath | 0.02 | 0.02 | 0.02 | 1 | 165.3 |
| Deep path navigation | jsonpath-ng | 0.03 | 0.03 | 0.03 | 1 | 165.3 |
| Extract all user IDs | strata | ERROR | - | - | - | - |
| Extract all user IDs | jmespath | 0.91 | 1.07 | 1.23 | 2000 | 155.1 |
| Extract all user IDs | jsonpath-ng | 15.61 | 15.86 | 16.11 | 2000 | 156.2 |
| Extract all user names | strata | ERROR | - | - | - | - |
| Extract all user names | jmespath | 0.97 | 0.98 | 1.00 | 2000 | 169.2 |
| Extract all user names | jsonpath-ng | 14.89 | 15.40 | 15.92 | 2000 | 169.2 |
| Extract nested timestamp field | strata | ERROR | - | - | - | - |
| Extract nested timestamp field | jmespath | 1.74 | 1.85 | 1.97 | 2000 | 159.1 |
| Extract nested timestamp field | jsonpath-ng | 17.27 | 17.53 | 17.79 | 2000 | 159.1 |
| Extract order item prices (double wildcard) | strata | ERROR | - | - | - | - |
| Extract order item prices (double wildcard) | jmespath | 30.07 | 30.19 | 30.32 | 100999 | 164.1 |
| Extract order item prices (double wildcard) | jsonpath-ng | 172.34 | 173.10 | 173.85 | 100999 | 166.8 |
| Filter users by age (numeric predicate) | strata | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | jsonpath-ng | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | jmespath | 2.69 | 2.72 | 2.75 | 1606 | 168.3 |
| Recursively find all prices | strata | ERROR | - | - | - | - |
| Recursively find all prices | jsonpath-ng | 569.15 | 580.82 | 592.48 | 100999 | 173.7 |
| Slice first 10 users, extract IDs | strata | ERROR | - | - | - | - |
| Slice first 10 users, extract IDs | jmespath | 0.02 | 0.02 | 0.02 | 10 | 171.9 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.04 | 0.04 | 0.05 | 10 | 171.9 |
