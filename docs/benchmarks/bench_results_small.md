# Strata Benchmark Results

Generated: 2026-02-08 00:45:24

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
| orjson | users.json | 2.518 | 3.848 | 3.927 | 45.5 |
| msgspec | users.json | 2.625 | 3.952 | 4.387 | 48.6 |
| strata | users.json | 4.499 | 4.874 | 4.981 | 41.4 |
| ujson | users.json | 3.999 | 5.608 | 6.176 | 46.5 |
| json (stdlib) | users.json | 5.491 | 6.218 | 6.280 | 49.1 |
| orjson | users.ndjson | 2.593 | 3.417 | 3.901 | 52.5 |
| msgspec | users.ndjson | 2.840 | 3.750 | 3.857 | 54.6 |
| ujson | users.ndjson | 4.003 | 5.398 | 5.828 | 54.6 |
| json (stdlib) | users.ndjson | 5.516 | 6.393 | 6.459 | 54.6 |
| strata | users.ndjson | 6.325 | 7.170 | 7.203 | 51.6 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | eval_query (baseline) | 0.029 | 1000 |
|  | jmespath | 0.204 | 1000 |
|  | jsonpath-ng | 1.566 | 1000 |
| $.users[*].orders[*].items[*].price | eval_query (baseline) | 0.540 | 12046 |
|  | jmespath | 5.339 | 1000 |
|  | jsonpath-ng | 16.879 | 12046 |
| $..price | eval_query (baseline) | 5.769 | 12046 |
|  | jmespath | 5.896 | 1000 |
|  | jsonpath-ng | 71.924 | 12046 |
| $.users[?(@.age>30)] | eval_query (baseline) | 0.029 | 794 |
| $..orders[?(@.status=="shipped")] | eval_query (baseline) | 6.256 | 3208 |
