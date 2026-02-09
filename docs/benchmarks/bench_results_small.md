# Strata Benchmark Results

Generated: 2026-02-09 15:57:10

## Environment

- Commit: 997f6c7369aa9f5a3a7262bf5222e121a1ac2e09
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
| msgspec | users.json | 2.744 | 4.061 | 4.071 | 48.6 |
| orjson | users.json | 2.523 | 4.634 | 4.684 | 44.5 |
| strata | users.json | 4.815 | 5.402 | 5.492 | 41.4 |
| ujson | users.json | 4.110 | 6.300 | 7.351 | 46.5 |
| json (stdlib) | users.json | 5.271 | 6.566 | 7.020 | 49.6 |
| orjson | users.ndjson | 2.805 | 3.806 | 3.941 | 53.7 |
| msgspec | users.ndjson | 2.915 | 4.007 | 4.152 | 55.7 |
| ujson | users.ndjson | 4.531 | 6.090 | 6.471 | 55.7 |
| strata | users.ndjson | 5.781 | 6.270 | 6.573 | 52.7 |
| json (stdlib) | users.ndjson | 5.775 | 7.197 | 7.387 | 55.7 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | query | 0.029 | 1000 |
|  | jmespath | 0.195 | 1000 |
|  | jsonpath-ng | 1.907 | 1000 |
| $.users[*].orders[*].items[*].price | query | 0.604 | 12046 |
|  | jmespath | 5.501 | 1000 |
|  | jsonpath-ng | 18.709 | 12046 |
| $..price | jmespath | 5.818 | 1000 |
|  | query | 6.196 | 12046 |
|  | jsonpath-ng | 75.872 | 12046 |
| $.users[?(@.age>30)] | query | 0.031 | 794 |
| $..orders[?(@.status=="shipped")] | query | 6.503 | 3208 |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| msgspec | 3.07 | 4.04 | 4.13 | 43.3 |
| orjson | 3.37 | 4.06 | 4.94 | 43.2 |
| strata | 4.68 | 5.05 | 5.26 | 39.7 |
| ujson | 3.84 | 6.47 | 6.83 | 46.4 |
| json (stdlib) | 5.78 | 6.73 | 7.31 | 45.8 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 3.05 | 3.73 | 4.15 | 36.3 |
| msgspec | 3.34 | 4.04 | 4.13 | 36.3 |
| strata | 3.98 | 4.67 | 4.70 | 35.7 |
| ujson | 4.26 | 5.82 | 6.12 | 39.4 |
| json (stdlib) | 5.98 | 7.17 | 7.54 | 38.4 |
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
| Deep path navigation | strata | 0.01 | 0.01 | 0.02 | 1 | 63.6 |
| Deep path navigation | jmespath | 0.01 | 0.01 | 0.01 | 1 | 63.6 |
| Deep path navigation | jsonpath-ng | 0.01 | 0.02 | 0.02 | 1 | 63.6 |
| Extract all user IDs | jmespath | 0.21 | 0.21 | 0.22 | 1000 | 49.4 |
| Extract all user IDs | strata | 0.97 | 1.00 | 1.03 | 1000 | 49.2 |
| Extract all user IDs | jsonpath-ng | 1.63 | 1.68 | 1.74 | 1000 | 50.2 |
| Extract all user names | jmespath | 0.20 | 0.23 | 0.26 | 1000 | 63.6 |
| Extract all user names | strata | 1.09 | 1.09 | 1.10 | 1000 | 63.6 |
| Extract all user names | jsonpath-ng | 1.62 | 1.88 | 2.14 | 1000 | 63.6 |
| Extract nested timestamp field | jmespath | 0.54 | 0.56 | 0.57 | 1000 | 60.8 |
| Extract nested timestamp field | strata | 0.94 | 0.98 | 1.02 | 1000 | 60.8 |
| Extract nested timestamp field | jsonpath-ng | 2.25 | 2.27 | 2.29 | 1000 | 61.2 |
| Extract order item prices (double wildcard) | jmespath | 6.23 | 6.47 | 6.70 | 1000 | 62.1 |
| Extract order item prices (double wildcard) | strata | 11.46 | 11.46 | 11.47 | 12046 | 61.5 |
| Extract order item prices (double wildcard) | jsonpath-ng | 19.19 | 19.74 | 20.30 | 12046 | 65.8 |
| Filter users by age (numeric predicate) | strata | 0.85 | 0.89 | 0.93 | 794 | 63.6 |
| Filter users by age (numeric predicate) | jmespath | 1.14 | 1.22 | 1.30 | 794 | 63.6 |
| Recursively find all prices | strata | 32.54 | 34.22 | 35.90 | 12046 | 66.9 |
| Recursively find all prices | jsonpath-ng | 74.91 | 75.90 | 76.88 | 12046 | 68.6 |
| Slice first 10 users, extract IDs | jmespath | 0.01 | 0.01 | 0.01 | 10 | 66.9 |
| Slice first 10 users, extract IDs | strata | 0.01 | 0.02 | 0.02 | 10 | 66.9 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.02 | 0.03 | 0.03 | 10 | 66.9 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | jmespath | 0.01 | 0.01 | 0.01 | 1 | 63.6 |
| Deep path navigation | jsonpath-ng | 0.01 | 0.01 | 0.01 | 1 | 63.6 |
| Deep path navigation | strata | 9.36 | 9.48 | 9.61 | 1 | 63.6 |
| Extract all user IDs | jmespath | 0.28 | 0.31 | 0.33 | 1000 | 57.5 |
| Extract all user IDs | jsonpath-ng | 2.76 | 2.82 | 2.87 | 1000 | 58.2 |
| Extract all user IDs | strata | 10.71 | 10.72 | 10.73 | 1000 | 57.3 |
| Extract all user names | jmespath | 0.38 | 0.42 | 0.46 | 1000 | 64.1 |
| Extract all user names | jsonpath-ng | 3.38 | 3.45 | 3.51 | 1000 | 64.1 |
| Extract all user names | strata | 10.14 | 10.32 | 10.50 | 1000 | 64.1 |
| Extract nested timestamp field | jmespath | 0.71 | 0.73 | 0.76 | 1000 | 61.8 |
| Extract nested timestamp field | jsonpath-ng | 3.38 | 3.45 | 3.53 | 1000 | 62.4 |
| Extract nested timestamp field | strata | 10.92 | 11.03 | 11.15 | 1000 | 61.7 |
| Extract order item prices (double wildcard) | jmespath | 3.79 | 3.84 | 3.89 | 12046 | 61.7 |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.74 | 18.84 | 18.95 | 12046 | 63.2 |
| Extract order item prices (double wildcard) | strata | 21.20 | 21.36 | 21.53 | 12046 | 61.5 |
| Filter users by age (numeric predicate) | jmespath | 1.21 | 1.36 | 1.52 | 794 | 64.5 |
| Filter users by age (numeric predicate) | strata | 10.01 | 10.07 | 10.13 | 794 | 64.5 |
| Recursively find all prices | strata | 39.24 | 39.89 | 40.53 | 12046 | 65.9 |
| Recursively find all prices | jsonpath-ng | 74.00 | 74.38 | 74.76 | 12046 | 67.5 |
| Slice first 10 users, extract IDs | jmespath | 0.01 | 0.01 | 0.01 | 10 | 67.5 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.01 | 0.01 | 0.02 | 10 | 67.5 |
| Slice first 10 users, extract IDs | strata | 9.26 | 9.28 | 9.30 | 10 | 67.5 |
