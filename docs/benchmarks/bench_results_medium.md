# Strata Benchmark Results

Generated: 2026-02-09 12:57:50

## Environment

- Commit: 488cf8bec2f660d9fa5b253b8abf51696ad6b26d
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
| orjson | users.json | 18.803 | 26.804 | 29.211 | 123.6 |
| msgspec | users.json | 19.932 | 29.400 | 29.495 | 123.5 |
| strata | users.json | 28.166 | 31.757 | 32.325 | 103.4 |
| ujson | users.json | 27.751 | 40.128 | 41.767 | 142.1 |
| json (stdlib) | users.json | 35.728 | 45.003 | 48.653 | 123.5 |
| orjson | users.ndjson | 22.360 | 23.350 | 24.686 | 136.1 |
| msgspec | users.ndjson | 22.621 | 25.298 | 25.348 | 135.8 |
| ujson | users.ndjson | 31.393 | 35.139 | 35.768 | 154.5 |
| strata | users.ndjson | 30.074 | 39.509 | 40.779 | 130.7 |
| json (stdlib) | users.ndjson | 39.435 | 42.140 | 42.502 | 138.3 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | query | 0.229 | 2000 |
|  | jmespath | 0.696 | 2000 |
|  | jsonpath-ng | 7.592 | 2000 |
| $.users[*].orders[*].items[*].price | query | 4.171 | 100999 |
|  | jmespath | 33.672 | 2000 |
|  | jsonpath-ng | 137.807 | 100999 |
| $..price | jmespath | 34.392 | 2000 |
|  | query | 38.179 | 100999 |
|  | jsonpath-ng | 509.834 | 100999 |
| $.users[?(@.age>30)] | query | 0.141 | 1606 |
| $..orders[?(@.status=="shipped")] | query | 40.355 | 13300 |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/medium/users.json
- Input size: 6.25 MB
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 18.50 | 27.27 | 27.29 | 125.8 |
| msgspec | 20.25 | 29.06 | 29.55 | 123.5 |
| strata | 28.42 | 31.29 | 32.42 | 103.5 |
| ujson | 27.14 | 39.73 | 40.34 | 141.3 |
| json (stdlib) | 35.06 | 44.71 | 45.02 | 123.6 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Input size: 6.25 MB
- Lines: 2000
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 23.64 | 24.48 | 24.59 | 151.7 |
| msgspec | 23.67 | 25.46 | 25.55 | 149.5 |
| ujson | 32.28 | 35.78 | 36.43 | 167.2 |
| json (stdlib) | 40.30 | 42.40 | 42.44 | 150.4 |
| strata | 51.38 | 51.69 | 53.54 | 151.5 |
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
| Deep path navigation | strata | 0.01 | 0.01 | 0.01 | 1 | 251.4 |
| Deep path navigation | jmespath | 0.01 | 0.01 | 0.01 | 1 | 250.4 |
| Deep path navigation | jsonpath-ng | 0.01 | 0.01 | 0.02 | 1 | 250.4 |
| Extract all user IDs | jmespath | 0.52 | 0.55 | 0.58 | 2000 | 172.0 |
| Extract all user IDs | strata | 1.70 | 1.71 | 1.73 | 2000 | 171.9 |
| Extract all user IDs | jsonpath-ng | 6.35 | 6.35 | 6.36 | 2000 | 173.3 |
| Extract all user names | jmespath | 0.43 | 0.45 | 0.47 | 2000 | 250.4 |
| Extract all user names | strata | 1.58 | 1.65 | 1.71 | 2000 | 250.4 |
| Extract all user names | jsonpath-ng | 6.54 | 6.56 | 6.57 | 2000 | 250.4 |
| Extract nested timestamp field | jmespath | 1.20 | 1.21 | 1.21 | 2000 | 228.4 |
| Extract nested timestamp field | strata | 1.73 | 1.73 | 1.73 | 2000 | 228.4 |
| Extract nested timestamp field | jsonpath-ng | 7.69 | 7.72 | 7.74 | 2000 | 229.0 |
| Extract order item prices (double wildcard) | jmespath | 33.71 | 34.00 | 34.29 | 2000 | 251.0 |
| Extract order item prices (double wildcard) | strata | 77.58 | 77.87 | 78.17 | 100999 | 249.0 |
| Extract order item prices (double wildcard) | jsonpath-ng | 135.13 | 135.27 | 135.40 | 100999 | 253.7 |
| Filter users by age (numeric predicate) | strata | 1.34 | 1.58 | 1.81 | 1606 | 250.5 |
| Filter users by age (numeric predicate) | jmespath | 2.38 | 2.39 | 2.40 | 1606 | 250.5 |
| Recursively find all prices | strata | 161.44 | 163.91 | 166.38 | 100999 | 256.8 |
| Recursively find all prices | jsonpath-ng | 500.73 | 501.64 | 502.55 | 100999 | 261.5 |
| Slice first 10 users, extract IDs | strata | 0.02 | 0.02 | 0.02 | 10 | 260.5 |
| Slice first 10 users, extract IDs | jmespath | 0.01 | 0.02 | 0.02 | 10 | 260.5 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.02 | 0.02 | 0.02 | 10 | 259.9 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/medium/users.ndjson
- Size: 6.25 MB
- Records: 2000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | jmespath | 0.01 | 0.01 | 0.01 | 1 | 294.8 |
| Deep path navigation | jsonpath-ng | 0.02 | 0.02 | 0.02 | 1 | 294.8 |
| Deep path navigation | strata | 64.69 | 64.88 | 65.08 | 1 | 294.8 |
| Extract all user IDs | jmespath | 0.78 | 0.81 | 0.84 | 2000 | 223.2 |
| Extract all user IDs | jsonpath-ng | 12.25 | 12.61 | 12.97 | 2000 | 224.2 |
| Extract all user IDs | strata | 69.15 | 69.98 | 70.82 | 2000 | 223.0 |
| Extract all user names | jmespath | 0.85 | 0.86 | 0.87 | 2000 | 311.9 |
| Extract all user names | jsonpath-ng | 12.47 | 12.66 | 12.84 | 2000 | 312.0 |
| Extract all user names | strata | 66.33 | 66.67 | 67.01 | 2000 | 311.9 |
| Extract nested timestamp field | jmespath | 1.41 | 1.45 | 1.49 | 2000 | 246.5 |
| Extract nested timestamp field | jsonpath-ng | 13.58 | 13.59 | 13.59 | 2000 | 247.1 |
| Extract nested timestamp field | strata | 66.35 | 66.88 | 67.41 | 2000 | 246.5 |
| Extract order item prices (double wildcard) | jmespath | 25.64 | 25.76 | 25.87 | 100999 | 279.4 |
| Extract order item prices (double wildcard) | jsonpath-ng | 149.93 | 150.63 | 151.33 | 100999 | 281.9 |
| Extract order item prices (double wildcard) | strata | 154.21 | 156.20 | 158.18 | 100999 | 279.2 |
| Filter users by age (numeric predicate) | jmespath | 2.47 | 2.50 | 2.52 | 1606 | 336.8 |
| Filter users by age (numeric predicate) | strata | 66.22 | 66.28 | 66.35 | 1606 | 336.8 |
| Recursively find all prices | strata | 231.03 | 232.10 | 233.17 | 100999 | 368.8 |
| Recursively find all prices | jsonpath-ng | 522.49 | 522.84 | 523.18 | 100999 | 371.6 |
| Slice first 10 users, extract IDs | jmespath | 0.01 | 0.02 | 0.02 | 10 | 388.5 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.02 | 0.02 | 0.02 | 10 | 388.5 |
| Slice first 10 users, extract IDs | strata | 64.51 | 64.55 | 64.59 | 10 | 388.5 |
