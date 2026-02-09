# Strata Benchmark Results

Generated: 2026-02-09 12:57:41

## Environment

- Commit: 488cf8bec2f660d9fa5b253b8abf51696ad6b26d
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
| msgspec | users.json | 2.660 | 3.838 | 4.234 | 48.6 |
| orjson | users.json | 2.471 | 4.241 | 4.440 | 44.5 |
| strata | users.json | 4.276 | 4.744 | 4.819 | 41.4 |
| ujson | users.json | 4.192 | 5.910 | 6.176 | 46.5 |
| json (stdlib) | users.json | 5.072 | 6.344 | 6.535 | 49.0 |
| orjson | users.ndjson | 2.645 | 3.678 | 3.809 | 52.4 |
| msgspec | users.ndjson | 2.803 | 3.821 | 4.053 | 54.4 |
| strata | users.ndjson | 5.021 | 5.546 | 5.748 | 51.4 |
| ujson | users.ndjson | 4.075 | 5.836 | 6.042 | 54.4 |
| json (stdlib) | users.ndjson | 5.712 | 6.540 | 6.910 | 54.4 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | query | 0.037 | 1000 |
|  | jmespath | 0.187 | 1000 |
|  | jsonpath-ng | 1.571 | 1000 |
| $.users[*].orders[*].items[*].price | query | 0.543 | 12046 |
|  | jmespath | 5.613 | 1000 |
|  | jsonpath-ng | 17.946 | 12046 |
| $..price | jmespath | 5.804 | 1000 |
|  | query | 5.971 | 12046 |
|  | jsonpath-ng | 71.991 | 12046 |
| $.users[?(@.age>30)] | query | 0.029 | 794 |
| $..orders[?(@.status=="shipped")] | query | 6.702 | 3208 |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 2.91 | 3.56 | 3.88 | 42.9 |
| msgspec | 2.86 | 3.73 | 4.14 | 43.0 |
| strata | 4.55 | 4.82 | 5.11 | 39.5 |
| ujson | 3.89 | 6.01 | 6.34 | 46.1 |
| json (stdlib) | 5.04 | 6.23 | 6.42 | 45.5 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 2.69 | 3.62 | 3.86 | 36.2 |
| msgspec | 2.89 | 3.85 | 3.87 | 36.2 |
| strata | 3.81 | 4.39 | 4.53 | 35.7 |
| ujson | 4.09 | 5.68 | 5.77 | 39.3 |
| json (stdlib) | 5.52 | 6.64 | 6.85 | 38.3 |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | strata | 0.00 | 0.00 | 0.00 | 1 | 63.1 |
| Deep path navigation | jmespath | 0.00 | 0.01 | 0.01 | 1 | 63.1 |
| Deep path navigation | jsonpath-ng | 0.01 | 0.01 | 0.02 | 1 | 63.1 |
| Extract all user IDs | jmespath | 0.19 | 0.21 | 0.23 | 1000 | 49.5 |
| Extract all user IDs | strata | 1.35 | 1.35 | 1.35 | 1000 | 49.4 |
| Extract all user IDs | jsonpath-ng | 1.55 | 1.62 | 1.68 | 1000 | 50.3 |
| Extract all user names | jmespath | 0.21 | 0.21 | 0.21 | 1000 | 63.1 |
| Extract all user names | strata | 0.88 | 1.02 | 1.17 | 1000 | 63.1 |
| Extract all user names | jsonpath-ng | 1.52 | 1.68 | 1.83 | 1000 | 63.1 |
| Extract nested timestamp field | jmespath | 0.63 | 0.63 | 0.63 | 1000 | 60.9 |
| Extract nested timestamp field | strata | 1.08 | 1.24 | 1.40 | 1000 | 60.9 |
| Extract nested timestamp field | jsonpath-ng | 2.28 | 2.41 | 2.54 | 1000 | 61.2 |
| Extract order item prices (double wildcard) | jmespath | 5.58 | 5.65 | 5.72 | 1000 | 62.0 |
| Extract order item prices (double wildcard) | strata | 9.23 | 9.37 | 9.50 | 12046 | 61.5 |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.42 | 17.53 | 17.64 | 12046 | 65.3 |
| Filter users by age (numeric predicate) | strata | 0.95 | 0.96 | 0.97 | 794 | 63.2 |
| Filter users by age (numeric predicate) | jmespath | 1.13 | 1.13 | 1.13 | 794 | 63.2 |
| Recursively find all prices | strata | 28.35 | 28.47 | 28.58 | 12046 | 63.5 |
| Recursively find all prices | jsonpath-ng | 72.92 | 73.10 | 73.28 | 12046 | 65.2 |
| Slice first 10 users, extract IDs | jmespath | 0.01 | 0.01 | 0.01 | 10 | 63.5 |
| Slice first 10 users, extract IDs | strata | 0.01 | 0.01 | 0.01 | 10 | 63.5 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.01 | 0.02 | 0.02 | 10 | 63.5 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | jmespath | 0.01 | 0.01 | 0.01 | 1 | 62.2 |
| Deep path navigation | jsonpath-ng | 0.01 | 0.01 | 0.01 | 1 | 62.3 |
| Deep path navigation | strata | 9.21 | 9.29 | 9.36 | 1 | 62.2 |
| Extract all user IDs | jmespath | 0.26 | 0.28 | 0.30 | 1000 | 57.3 |
| Extract all user IDs | jsonpath-ng | 2.10 | 2.22 | 2.33 | 1000 | 58.0 |
| Extract all user IDs | strata | 10.10 | 10.21 | 10.33 | 1000 | 57.2 |
| Extract all user names | jmespath | 0.23 | 0.26 | 0.29 | 1000 | 62.8 |
| Extract all user names | jsonpath-ng | 2.19 | 2.40 | 2.61 | 1000 | 62.8 |
| Extract all user names | strata | 9.77 | 9.91 | 10.06 | 1000 | 62.8 |
| Extract nested timestamp field | jmespath | 0.62 | 0.62 | 0.62 | 1000 | 60.6 |
| Extract nested timestamp field | jsonpath-ng | 2.72 | 2.80 | 2.88 | 1000 | 61.2 |
| Extract nested timestamp field | strata | 9.78 | 10.01 | 10.24 | 1000 | 60.6 |
| Extract order item prices (double wildcard) | jmespath | 3.55 | 3.64 | 3.73 | 12046 | 60.5 |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.05 | 18.06 | 18.07 | 12046 | 61.8 |
| Extract order item prices (double wildcard) | strata | 18.76 | 19.58 | 20.40 | 12046 | 60.3 |
| Filter users by age (numeric predicate) | jmespath | 1.15 | 1.16 | 1.17 | 794 | 62.9 |
| Filter users by age (numeric predicate) | strata | 9.65 | 9.74 | 9.83 | 794 | 62.9 |
| Recursively find all prices | strata | 38.41 | 38.68 | 38.95 | 12046 | 64.4 |
| Recursively find all prices | jsonpath-ng | 71.78 | 72.22 | 72.66 | 12046 | 66.0 |
| Slice first 10 users, extract IDs | jmespath | 0.01 | 0.01 | 0.01 | 10 | 66.0 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.02 | 0.02 | 0.03 | 10 | 66.0 |
| Slice first 10 users, extract IDs | strata | 8.84 | 9.00 | 9.17 | 10 | 66.0 |
