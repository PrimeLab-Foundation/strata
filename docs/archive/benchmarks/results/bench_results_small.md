# Strata Benchmark Results

Generated: 2026-02-05 02:24:43

## Environment

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
| msgspec | users.json | 2.503 | 3.656 | 3.805 | 42.4 |
| strata | users.json | 3.784 | 4.417 | 4.625 | 35.1 |
| orjson | users.json | 2.566 | 5.129 | 5.227 | 39.2 |
| ujson | users.json | 4.496 | 5.518 | 6.042 | 40.3 |
| json (stdlib) | users.json | 5.060 | 6.104 | 6.879 | 43.3 |
| orjson | users.ndjson | 2.740 | 3.703 | 4.238 | 47.1 |
| msgspec | users.ndjson | 2.958 | 3.801 | 4.208 | 49.1 |
| ujson | users.ndjson | 3.975 | 6.374 | 6.585 | 49.1 |
| json (stdlib) | users.ndjson | 5.579 | 6.588 | 6.679 | 49.1 |
| strata | users.ndjson | 5.935 | 6.931 | 7.015 | 46.1 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | query | 0.026 | 1000 |
|  | jmespath | 0.193 | 1000 |
|  | jsonpath-ng | 1.522 | 1000 |
| $.users[*].orders[*].items[*].price | query | 0.539 | 12046 |
|  | jmespath | 5.682 | 1000 |
|  | jsonpath-ng | 17.224 | 12046 |
| $..price | jmespath | 5.684 | 1000 |
|  | query | 5.931 | 12046 |
|  | jsonpath-ng | 74.282 | 12046 |
| $.users[?(@.age>30)] | query | 0.030 | 794 |
| $..orders[?(@.status=="shipped")] | query | 6.291 | 3208 |
