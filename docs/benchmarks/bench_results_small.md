# Strata Benchmark Results

Generated: 2026-02-08 01:37:04

## Environment

- Commit: 046be0a58ea6da59b900e1e2f2bedde80affdad7
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
| orjson | users.json | 2.455 | 3.403 | 3.714 | 45.8 |
| msgspec | users.json | 2.681 | 3.821 | 3.822 | 48.9 |
| strata | users.json | 4.455 | 5.140 | 5.268 | 41.7 |
| ujson | users.json | 3.943 | 5.866 | 6.248 | 46.8 |
| json (stdlib) | users.json | 5.124 | 6.112 | 6.179 | 48.9 |
| orjson | users.ndjson | 2.751 | 3.562 | 3.571 | 52.2 |
| msgspec | users.ndjson | 2.776 | 3.637 | 3.741 | 54.3 |
| ujson | users.ndjson | 4.016 | 5.541 | 5.674 | 54.3 |
| json (stdlib) | users.ndjson | 5.503 | 6.601 | 6.613 | 54.3 |
| strata | users.ndjson | 6.408 | 7.532 | 7.817 | 51.2 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | eval_query (baseline) | 0.026 | 1000 |
|  | jmespath | 0.187 | 1000 |
|  | jsonpath-ng | 1.554 | 1000 |
| $.users[*].orders[*].items[*].price | eval_query (baseline) | 0.545 | 12046 |
|  | jmespath | 5.423 | 1000 |
|  | jsonpath-ng | 18.130 | 12046 |
| $..price | eval_query (baseline) | 6.046 | 12046 |
|  | jmespath | 6.046 | 1000 |
|  | jsonpath-ng | 72.361 | 12046 |
| $.users[?(@.age>30)] | eval_query (baseline) | 0.028 | 794 |
| $..orders[?(@.status=="shipped")] | eval_query (baseline) | 6.569 | 3208 |
