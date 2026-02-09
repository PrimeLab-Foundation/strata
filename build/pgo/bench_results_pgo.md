# Strata Benchmark Results

Generated: 2026-02-08 02:27:07

## Environment

- Commit: 7cbfa28c013c7ba75e24cd26947d7a8cba9305b3
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 50
- Warmup: 3
- Datasets: /Users/borysbardysh/PycharmProjects/personal/PrimeLabFoundation/strata/benchmarks/data/generated/medium/users.json, /Users/borysbardysh/PycharmProjects/personal/PrimeLabFoundation/strata/benchmarks/data/generated/medium/users.ndjson

## Parsing Benchmarks

| Library | Dataset | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|---------|----------|-------------|----------|---------|
| orjson | users.json | 16.847 | 25.662 | 26.718 | 161.8 |
| msgspec | users.json | 18.208 | 27.897 | 29.866 | 181.0 |
| strata | users.json | 25.000 | 28.494 | 29.410 | 136.3 |
| ujson | users.json | 25.077 | 36.704 | 38.359 | 198.9 |
| json (stdlib) | users.json | 34.528 | 44.022 | 44.893 | 181.0 |
| orjson | users.ndjson | 22.398 | 24.103 | 24.817 | 149.0 |
| msgspec | users.ndjson | 22.901 | 25.014 | 26.678 | 165.1 |
| ujson | users.ndjson | 30.278 | 34.108 | 35.282 | 168.1 |
| strata | users.ndjson | 29.228 | 39.221 | 40.835 | 142.0 |
| json (stdlib) | users.ndjson | 38.965 | 41.562 | 42.171 | 165.1 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | eval_query (baseline) | 0.080 | 2000 |
|  | jmespath | 0.403 | 2000 |
|  | jsonpath-ng | 6.279 | 2000 |
| $.users[*].orders[*].items[*].price | eval_query (baseline) | 3.811 | 100999 |
|  | jmespath | 33.584 | 2000 |
|  | jsonpath-ng | 132.980 | 100999 |
| $..price | jmespath | 34.456 | 2000 |
|  | eval_query (baseline) | 37.935 | 100999 |
|  | jsonpath-ng | 500.929 | 100999 |
| $.users[?(@.age>30)] | eval_query (baseline) | 0.115 | 1606 |
| $..orders[?(@.status=="shipped")] | eval_query (baseline) | 42.335 | 13300 |
