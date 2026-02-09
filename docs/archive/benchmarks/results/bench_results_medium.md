# Strata Benchmark Results

Generated: 2026-02-05 02:24:22

## Environment

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
| orjson | users.json | 19.132 | 27.018 | 28.602 | 92.0 |
| msgspec | users.json | 20.403 | 29.286 | 31.085 | 91.8 |
| strata | users.json | 27.363 | 31.233 | 31.951 | 71.7 |
| ujson | users.json | 27.298 | 40.160 | 40.467 | 110.4 |
| json (stdlib) | users.json | 35.057 | 44.470 | 46.747 | 98.1 |
| orjson | users.ndjson | 22.613 | 24.301 | 24.773 | 114.5 |
| msgspec | users.ndjson | 22.165 | 25.492 | 25.738 | 114.2 |
| ujson | users.ndjson | 31.501 | 35.629 | 35.683 | 132.9 |
| strata | users.ndjson | 30.880 | 40.427 | 42.286 | 109.1 |
| json (stdlib) | users.ndjson | 38.561 | 42.593 | 42.607 | 116.6 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | query | 0.158 | 2000 |
|  | jmespath | 0.430 | 2000 |
|  | jsonpath-ng | 6.852 | 2000 |
| $.users[*].orders[*].items[*].price | query | 3.765 | 100999 |
|  | jmespath | 33.200 | 2000 |
|  | jsonpath-ng | 138.080 | 100999 |
| $..price | jmespath | 34.340 | 2000 |
|  | query | 37.386 | 100999 |
|  | jsonpath-ng | 500.362 | 100999 |
| $.users[?(@.age>30)] | query | 0.176 | 1606 |
| $..orders[?(@.status=="shipped")] | query | 39.469 | 13300 |
