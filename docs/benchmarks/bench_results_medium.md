# Strata Benchmark Results

Generated: 2026-02-09 02:46:30

## Environment

- Commit: 56fb161e478e029b69335528dad4957c4bdaf79f
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
| orjson | users.json | 21.588 | 30.644 | 30.947 | 123.6 |
| msgspec | users.json | 22.497 | 32.992 | 34.340 | 123.4 |
| strata | users.json | 31.762 | 35.898 | 36.183 | 103.3 |
| ujson | users.json | 31.500 | 44.279 | 47.613 | 142.0 |
| json (stdlib) | users.json | 39.133 | 51.046 | 51.397 | 123.4 |
| orjson | users.ndjson | 24.687 | 25.264 | 26.744 | 136.5 |
| msgspec | users.ndjson | 24.503 | 26.089 | 27.907 | 136.3 |
| ujson | users.ndjson | 33.829 | 37.847 | 38.715 | 155.0 |
| strata | users.ndjson | 32.364 | 43.316 | 44.136 | 131.2 |
| json (stdlib) | users.ndjson | 42.188 | 43.554 | 45.007 | 138.7 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | eval_query (baseline) | 0.256 | 2000 |
|  | jmespath | 0.700 | 2000 |
|  | jsonpath-ng | 8.153 | 2000 |
| $.users[*].orders[*].items[*].price | eval_query (baseline) | 4.111 | 100999 |
|  | jmespath | 36.264 | 2000 |
|  | jsonpath-ng | 145.805 | 100999 |
| $..price | jmespath | 36.684 | 2000 |
|  | eval_query (baseline) | 39.621 | 100999 |
|  | jsonpath-ng | 535.123 | 100999 |
| $.users[?(@.age>30)] | eval_query (baseline) | 0.195 | 1606 |
| $..orders[?(@.status=="shipped")] | eval_query (baseline) | 42.508 | 13300 |
