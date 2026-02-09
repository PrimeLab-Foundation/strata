# Strata Benchmark Results

Generated: 2026-02-09 03:18:46

## Environment

- Commit: 29338dbc6a83118ecc29eecf4ac54f0879da7dca
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson

## Parsing Benchmarks

| Library | Dataset | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|---------|----------|-------------|----------|---------|
| msgspec | users.json | 2.917 | 3.553 | 3.874 | 48.7 |
| orjson | users.json | 2.496 | 3.834 | 4.152 | 45.5 |
| strata | users.json | 4.133 | 4.861 | 4.879 | 41.5 |
| ujson | users.json | 4.311 | 5.728 | 5.871 | 46.6 |
| json (stdlib) | users.json | 5.584 | 6.304 | 6.313 | 50.1 |
| msgspec | users.ndjson | 2.724 | 3.499 | 3.680 | 55.4 |
| orjson | users.ndjson | 2.824 | 3.587 | 3.898 | 53.4 |
| ujson | users.ndjson | 4.006 | 5.332 | 5.566 | 55.4 |
| strata | users.ndjson | 5.101 | 5.535 | 5.725 | 52.4 |
| json (stdlib) | users.ndjson | 5.528 | 6.515 | 6.618 | 55.4 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | eval_query (baseline) | 0.029 | 1000 |
|  | jmespath | 0.198 | 1000 |
|  | jsonpath-ng | 1.583 | 1000 |
| $.users[*].orders[*].items[*].price | eval_query (baseline) | 0.568 | 12046 |
|  | jmespath | 5.554 | 1000 |
|  | jsonpath-ng | 18.394 | 12046 |
| $..price | jmespath | 5.925 | 1000 |
|  | eval_query (baseline) | 6.132 | 12046 |
|  | jsonpath-ng | 71.880 | 12046 |
| $.users[?(@.age>30)] | eval_query (baseline) | 0.029 | 794 |
| $..orders[?(@.status=="shipped")] | eval_query (baseline) | 6.442 | 3208 |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| msgspec | 2.67 | 3.58 | 4.28 | 43.0 |
| orjson | 2.78 | 3.65 | 3.85 | 42.9 |
| strata | 4.37 | 4.84 | 4.97 | 39.5 |
| ujson | 3.81 | 5.29 | 5.72 | 46.1 |
| json (stdlib) | 5.09 | 6.11 | 6.31 | 45.5 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 2.77 | 3.58 | 3.82 | 36.1 |
| msgspec | 2.67 | 3.63 | 4.09 | 36.2 |
| strata | 3.92 | 4.16 | 4.34 | 35.6 |
| ujson | 4.03 | 5.39 | 5.84 | 39.2 |
| json (stdlib) | 5.67 | 6.59 | 6.73 | 38.2 |
## Search Benchmarks (JSONPath)

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | strata | 0.00 | 0.01 | 0.01 | 1 | 63.1 |
| Deep path navigation | jmespath | 0.01 | 0.01 | 0.01 | 1 | 63.1 |
| Deep path navigation | jsonpath-ng | 0.01 | 0.01 | 0.02 | 1 | 63.1 |
| Extract all user IDs | jmespath | 0.19 | 0.20 | 0.22 | 1000 | 49.5 |
| Extract all user IDs | strata | 1.44 | 1.44 | 1.45 | 1000 | 49.3 |
| Extract all user IDs | jsonpath-ng | 1.55 | 1.61 | 1.68 | 1000 | 50.2 |
| Extract all user names | jmespath | 0.18 | 0.19 | 0.19 | 1000 | 63.4 |
| Extract all user names | strata | 0.95 | 0.98 | 1.01 | 1000 | 63.4 |
| Extract all user names | jsonpath-ng | 1.60 | 1.66 | 1.71 | 1000 | 63.4 |
| Extract nested timestamp field | jmespath | 0.55 | 0.56 | 0.57 | 1000 | 60.9 |
| Extract nested timestamp field | strata | 1.27 | 1.30 | 1.33 | 1000 | 60.9 |
| Extract nested timestamp field | jsonpath-ng | 2.17 | 2.26 | 2.36 | 1000 | 61.2 |
| Extract order item prices (double wildcard) | jmespath | 5.56 | 5.72 | 5.88 | 1000 | 61.9 |
| Extract order item prices (double wildcard) | strata | 9.24 | 9.29 | 9.34 | 12046 | 61.4 |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.45 | 17.60 | 17.74 | 12046 | 65.3 |
| Filter users by age (numeric predicate) | jsonpath-ng | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | strata | 0.74 | 0.79 | 0.84 | 794 | 63.4 |
| Filter users by age (numeric predicate) | jmespath | 1.10 | 1.14 | 1.18 | 794 | 63.4 |
| Recursively find all prices | strata | 30.14 | 30.64 | 31.15 | 12046 | 66.8 |
| Recursively find all prices | jsonpath-ng | 73.51 | 73.52 | 73.53 | 12046 | 68.5 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.01 | 0.01 | 0.01 | 10 | 66.8 |
| Slice first 10 users, extract IDs | strata | 0.01 | 0.01 | 0.02 | 10 | 66.8 |
| Slice first 10 users, extract IDs | jmespath | 0.01 | 0.02 | 0.03 | 10 | 66.8 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | strata | ERROR | - | - | - | - |
| Deep path navigation | jmespath | 0.01 | 0.01 | 0.01 | 1 | 51.4 |
| Deep path navigation | jsonpath-ng | 0.01 | 0.01 | 0.02 | 1 | 51.4 |
| Extract all user IDs | strata | ERROR | - | - | - | - |
| Extract all user IDs | jmespath | 0.23 | 0.24 | 0.25 | 1000 | 46.5 |
| Extract all user IDs | jsonpath-ng | 1.97 | 2.04 | 2.12 | 1000 | 47.3 |
| Extract all user names | strata | ERROR | - | - | - | - |
| Extract all user names | jmespath | 0.28 | 0.29 | 0.30 | 1000 | 53.2 |
| Extract all user names | jsonpath-ng | 2.08 | 2.55 | 3.03 | 1000 | 53.2 |
| Extract nested timestamp field | strata | ERROR | - | - | - | - |
| Extract nested timestamp field | jmespath | 0.64 | 0.66 | 0.67 | 1000 | 49.9 |
| Extract nested timestamp field | jsonpath-ng | 2.66 | 2.70 | 2.73 | 1000 | 50.5 |
| Extract order item prices (double wildcard) | strata | ERROR | - | - | - | - |
| Extract order item prices (double wildcard) | jmespath | 3.58 | 3.64 | 3.71 | 12046 | 49.2 |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.70 | 17.74 | 17.77 | 12046 | 52.0 |
| Filter users by age (numeric predicate) | strata | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | jsonpath-ng | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | jmespath | 1.13 | 1.16 | 1.18 | 794 | 53.0 |
| Recursively find all prices | strata | ERROR | - | - | - | - |
| Recursively find all prices | jsonpath-ng | 71.47 | 71.62 | 71.78 | 12046 | 54.2 |
| Slice first 10 users, extract IDs | strata | ERROR | - | - | - | - |
| Slice first 10 users, extract IDs | jmespath | 0.01 | 0.01 | 0.01 | 10 | 54.2 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.02 | 0.02 | 0.02 | 10 | 54.2 |
