# Strata Benchmark Results

Generated: 2026-02-09 15:57:19

## Environment

- Commit: 997f6c7369aa9f5a3a7262bf5222e121a1ac2e09
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
| msgspec | users.json | 20.668 | 29.681 | 31.538 | 123.6 |
| orjson | users.json | 21.291 | 32.635 | 41.585 | 123.7 |
| strata | users.json | 29.467 | 33.273 | 33.556 | 103.5 |
| ujson | users.json | 31.230 | 43.477 | 50.636 | 142.2 |
| json (stdlib) | users.json | 35.827 | 46.683 | 47.075 | 123.6 |
| orjson | users.ndjson | 21.978 | 24.589 | 24.839 | 136.6 |
| msgspec | users.ndjson | 22.688 | 24.835 | 26.203 | 136.4 |
| ujson | users.ndjson | 31.961 | 35.799 | 36.879 | 155.1 |
| strata | users.ndjson | 30.206 | 39.597 | 40.721 | 131.2 |
| json (stdlib) | users.ndjson | 39.006 | 40.533 | 41.878 | 138.8 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | query | 0.135 | 2000 |
|  | jmespath | 0.482 | 2000 |
|  | jsonpath-ng | 6.573 | 2000 |
| $.users[*].orders[*].items[*].price | query | 3.902 | 100999 |
|  | jmespath | 33.796 | 2000 |
|  | jsonpath-ng | 139.021 | 100999 |
| $..price | jmespath | 34.703 | 2000 |
|  | query | 40.480 | 100999 |
|  | jsonpath-ng | 509.242 | 100999 |
| $.users[?(@.age>30)] | query | 0.210 | 1606 |
| $..orders[?(@.status=="shipped")] | query | 42.590 | 13300 |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 18.48 | 27.16 | 27.80 | 125.9 |
| msgspec | 19.92 | 28.91 | 30.17 | 123.6 |
| strata | 29.39 | 32.99 | 33.15 | 103.6 |
| ujson | 28.85 | 40.56 | 41.16 | 141.4 |
| json (stdlib) | 34.74 | 44.11 | 44.33 | 123.7 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 23.22 | 24.84 | 25.89 | 152.2 |
| msgspec | 23.89 | 25.51 | 26.19 | 149.9 |
| ujson | 36.10 | 36.56 | 37.07 | 167.7 |
| json (stdlib) | 40.15 | 42.56 | 43.05 | 150.8 |
| strata | 51.21 | 52.31 | 54.48 | 151.9 |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | strata | 0.01 | 0.01 | 0.01 | 1 | 254.5 |
| Deep path navigation | jmespath | 0.01 | 0.01 | 0.01 | 1 | 253.5 |
| Deep path navigation | jsonpath-ng | 0.01 | 0.02 | 0.02 | 1 | 253.5 |
| Extract all user IDs | jmespath | 0.57 | 0.60 | 0.63 | 2000 | 172.1 |
| Extract all user IDs | strata | 1.67 | 1.75 | 1.82 | 2000 | 171.9 |
| Extract all user IDs | jsonpath-ng | 6.24 | 6.66 | 7.07 | 2000 | 173.2 |
| Extract all user names | jmespath | 0.48 | 0.48 | 0.49 | 2000 | 253.5 |
| Extract all user names | strata | 1.88 | 1.96 | 2.05 | 2000 | 253.5 |
| Extract all user names | jsonpath-ng | 6.35 | 6.49 | 6.63 | 2000 | 253.5 |
| Extract nested timestamp field | jmespath | 1.25 | 1.41 | 1.56 | 2000 | 228.5 |
| Extract nested timestamp field | strata | 2.02 | 2.12 | 2.22 | 2000 | 228.5 |
| Extract nested timestamp field | jsonpath-ng | 7.92 | 8.58 | 9.24 | 2000 | 229.1 |
| Extract order item prices (double wildcard) | jmespath | 34.41 | 35.72 | 37.03 | 2000 | 254.2 |
| Extract order item prices (double wildcard) | strata | 91.73 | 92.98 | 94.22 | 100999 | 252.2 |
| Extract order item prices (double wildcard) | jsonpath-ng | 137.13 | 139.91 | 142.70 | 100999 | 256.8 |
| Filter users by age (numeric predicate) | strata | 1.62 | 1.67 | 1.72 | 1606 | 253.6 |
| Filter users by age (numeric predicate) | jmespath | 2.33 | 2.46 | 2.58 | 1606 | 253.6 |
| Recursively find all prices | strata | 166.06 | 167.29 | 168.53 | 100999 | 257.4 |
| Recursively find all prices | jsonpath-ng | 510.06 | 524.61 | 539.15 | 100999 | 261.6 |
| Slice first 10 users, extract IDs | jmespath | 0.01 | 0.01 | 0.02 | 10 | 260.6 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.02 | 0.02 | 0.02 | 10 | 260.0 |
| Slice first 10 users, extract IDs | strata | 0.02 | 0.02 | 0.02 | 10 | 260.6 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | jmespath | 0.01 | 0.02 | 0.02 | 1 | 294.6 |
| Deep path navigation | jsonpath-ng | 0.02 | 0.03 | 0.03 | 1 | 294.6 |
| Deep path navigation | strata | 65.49 | 66.92 | 68.35 | 1 | 294.6 |
| Extract all user IDs | jmespath | 0.78 | 0.79 | 0.81 | 2000 | 222.9 |
| Extract all user IDs | jsonpath-ng | 12.00 | 12.12 | 12.24 | 2000 | 223.9 |
| Extract all user IDs | strata | 69.83 | 71.03 | 72.23 | 2000 | 222.8 |
| Extract all user names | jmespath | 0.92 | 0.93 | 0.93 | 2000 | 311.8 |
| Extract all user names | jsonpath-ng | 12.42 | 13.16 | 13.91 | 2000 | 311.8 |
| Extract all user names | strata | 71.21 | 71.41 | 71.61 | 2000 | 311.8 |
| Extract nested timestamp field | jmespath | 1.36 | 1.38 | 1.40 | 2000 | 246.2 |
| Extract nested timestamp field | jsonpath-ng | 13.46 | 13.53 | 13.60 | 2000 | 246.8 |
| Extract nested timestamp field | strata | 66.90 | 67.08 | 67.26 | 2000 | 246.2 |
| Extract order item prices (double wildcard) | jmespath | 26.52 | 26.69 | 26.86 | 100999 | 279.2 |
| Extract order item prices (double wildcard) | jsonpath-ng | 151.85 | 152.23 | 152.62 | 100999 | 281.6 |
| Extract order item prices (double wildcard) | strata | 168.22 | 170.57 | 172.92 | 100999 | 278.9 |
| Filter users by age (numeric predicate) | jmespath | 2.31 | 2.41 | 2.50 | 1606 | 336.2 |
| Filter users by age (numeric predicate) | strata | 66.38 | 66.45 | 66.51 | 1606 | 336.2 |
| Recursively find all prices | strata | 230.04 | 232.74 | 235.44 | 100999 | 367.7 |
| Recursively find all prices | jsonpath-ng | 535.65 | 535.72 | 535.80 | 100999 | 370.5 |
| Slice first 10 users, extract IDs | jmespath | 0.02 | 0.02 | 0.03 | 10 | 387.3 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.02 | 0.02 | 0.02 | 10 | 387.3 |
| Slice first 10 users, extract IDs | strata | 65.29 | 65.50 | 65.72 | 10 | 387.3 |
