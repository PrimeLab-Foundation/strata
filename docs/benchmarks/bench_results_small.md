# Strata Benchmark Results

Generated: 2026-02-09 03:08:16

## Environment

- Commit: 886c0c601fb9cef6f1f2cd22d687abc4970b6902
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
| msgspec | users.json | 3.530 | 3.954 | 5.155 | 48.7 |
| orjson | users.json | 3.308 | 4.276 | 4.575 | 45.5 |
| strata | users.json | 4.635 | 5.133 | 5.476 | 41.4 |
| ujson | users.json | 4.791 | 6.264 | 6.913 | 46.6 |
| json (stdlib) | users.json | 5.452 | 6.793 | 7.530 | 50.1 |
| msgspec | users.ndjson | 3.663 | 4.579 | 5.071 | 55.4 |
| orjson | users.ndjson | 3.290 | 4.621 | 4.792 | 53.4 |
| ujson | users.ndjson | 5.136 | 6.614 | 6.764 | 55.4 |
| strata | users.ndjson | 6.046 | 6.964 | 7.732 | 52.4 |
| json (stdlib) | users.ndjson | 6.993 | 7.755 | 7.987 | 55.4 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | eval_query (baseline) | 0.043 | 1000 |
|  | jmespath | 0.187 | 1000 |
|  | jsonpath-ng | 1.894 | 1000 |
| $.users[*].orders[*].items[*].price | eval_query (baseline) | 0.590 | 12046 |
|  | jmespath | 6.722 | 1000 |
|  | jsonpath-ng | 21.110 | 12046 |
| $..price | jmespath | 6.576 | 1000 |
|  | eval_query (baseline) | 6.918 | 12046 |
|  | jsonpath-ng | 82.259 | 12046 |
| $.users[?(@.age>30)] | eval_query (baseline) | 0.042 | 794 |
| $..orders[?(@.status=="shipped")] | eval_query (baseline) | 7.078 | 3208 |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| msgspec | 3.48 | 4.70 | 5.61 | 43.8 |
| orjson | 3.36 | 4.79 | 5.30 | 43.8 |
| strata | 4.45 | 5.37 | 5.43 | 40.3 |
| ujson | 4.66 | 6.70 | 7.09 | 46.9 |
| json (stdlib) | 6.14 | 7.61 | 7.70 | 47.2 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 3.29 | 4.51 | 5.04 | 36.2 |
| msgspec | 3.10 | 4.75 | 4.79 | 36.2 |
| strata | 4.66 | 5.30 | 5.41 | 35.7 |
| ujson | 4.72 | 6.48 | 7.45 | 39.3 |
| json (stdlib) | 6.17 | 7.74 | 8.33 | 38.3 |
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
| Deep path navigation | strata | 0.01 | 0.01 | 0.02 | 1 | 65.3 |
| Deep path navigation | jsonpath-ng | 0.01 | 0.02 | 0.02 | 1 | 65.3 |
| Deep path navigation | jmespath | 0.02 | 0.02 | 0.02 | 1 | 65.3 |
| Extract all user IDs | jmespath | 0.24 | 0.25 | 0.25 | 1000 | 49.3 |
| Extract all user IDs | strata | 1.11 | 1.17 | 1.24 | 1000 | 49.2 |
| Extract all user IDs | jsonpath-ng | 2.19 | 2.37 | 2.54 | 1000 | 50.2 |
| Extract all user names | jmespath | 0.23 | 0.24 | 0.25 | 1000 | 65.3 |
| Extract all user names | strata | 1.10 | 1.15 | 1.21 | 1000 | 65.3 |
| Extract all user names | jsonpath-ng | 1.91 | 2.20 | 2.49 | 1000 | 65.3 |
| Extract nested timestamp field | jmespath | 0.54 | 0.58 | 0.62 | 1000 | 61.0 |
| Extract nested timestamp field | strata | 1.03 | 1.05 | 1.07 | 1000 | 61.0 |
| Extract nested timestamp field | jsonpath-ng | 3.18 | 3.22 | 3.26 | 1000 | 61.3 |
| Extract order item prices (double wildcard) | jmespath | 6.23 | 6.25 | 6.26 | 1000 | 64.1 |
| Extract order item prices (double wildcard) | strata | 11.61 | 11.69 | 11.77 | 12046 | 63.6 |
| Extract order item prices (double wildcard) | jsonpath-ng | 20.83 | 20.84 | 20.84 | 12046 | 67.5 |
| Filter users by age (numeric predicate) | jsonpath-ng | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | strata | 0.84 | 0.93 | 1.02 | 794 | 65.3 |
| Filter users by age (numeric predicate) | jmespath | 1.23 | 1.25 | 1.27 | 794 | 65.3 |
| Recursively find all prices | strata | 33.89 | 34.93 | 35.97 | 12046 | 66.6 |
| Recursively find all prices | jsonpath-ng | 84.10 | 84.17 | 84.25 | 12046 | 68.3 |
| Slice first 10 users, extract IDs | jmespath | 0.01 | 0.02 | 0.02 | 10 | 66.6 |
| Slice first 10 users, extract IDs | strata | 0.03 | 0.03 | 0.03 | 10 | 66.6 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.03 | 0.06 | 0.10 | 10 | 66.6 |
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
| Deep path navigation | jmespath | 0.02 | 0.02 | 0.02 | 1 | 52.8 |
| Deep path navigation | jsonpath-ng | 0.02 | 0.02 | 0.02 | 1 | 52.8 |
| Extract all user IDs | strata | ERROR | - | - | - | - |
| Extract all user IDs | jmespath | 0.27 | 0.36 | 0.45 | 1000 | 48.2 |
| Extract all user IDs | jsonpath-ng | 2.95 | 3.19 | 3.42 | 1000 | 48.9 |
| Extract all user names | strata | ERROR | - | - | - | - |
| Extract all user names | jmespath | 0.34 | 0.39 | 0.44 | 1000 | 54.7 |
| Extract all user names | jsonpath-ng | 2.77 | 3.20 | 3.64 | 1000 | 54.7 |
| Extract nested timestamp field | strata | ERROR | - | - | - | - |
| Extract nested timestamp field | jmespath | 0.84 | 0.84 | 0.85 | 1000 | 51.5 |
| Extract nested timestamp field | jsonpath-ng | 3.73 | 4.01 | 4.28 | 1000 | 52.1 |
| Extract order item prices (double wildcard) | strata | ERROR | - | - | - | - |
| Extract order item prices (double wildcard) | jmespath | 4.30 | 4.41 | 4.52 | 12046 | 50.8 |
| Extract order item prices (double wildcard) | jsonpath-ng | 20.29 | 20.98 | 21.67 | 12046 | 53.5 |
| Filter users by age (numeric predicate) | strata | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | jsonpath-ng | ERROR | - | - | - | - |
| Filter users by age (numeric predicate) | jmespath | 1.44 | 1.47 | 1.50 | 794 | 54.5 |
| Recursively find all prices | strata | ERROR | - | - | - | - |
| Recursively find all prices | jsonpath-ng | 81.17 | 82.26 | 83.36 | 12046 | 56.5 |
| Slice first 10 users, extract IDs | strata | ERROR | - | - | - | - |
| Slice first 10 users, extract IDs | jmespath | 0.02 | 0.02 | 0.02 | 10 | 56.5 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.04 | 0.04 | 0.04 | 10 | 56.5 |
