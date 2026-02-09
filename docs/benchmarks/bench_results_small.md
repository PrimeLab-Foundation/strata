# Strata Benchmark Results

Generated: 2026-02-09 02:46:22

## Environment

- Commit: 56fb161e478e029b69335528dad4957c4bdaf79f
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
| orjson | users.json | 2.971 | 4.889 | 4.913 | 45.6 |
| msgspec | users.json | 3.564 | 4.906 | 4.950 | 48.7 |
| strata | users.json | 4.982 | 5.628 | 5.762 | 41.5 |
| ujson | users.json | 4.662 | 6.500 | 7.527 | 46.7 |
| json (stdlib) | users.json | 6.175 | 7.868 | 7.902 | 50.2 |
| orjson | users.ndjson | 3.313 | 4.585 | 4.685 | 53.3 |
| msgspec | users.ndjson | 3.384 | 4.835 | 4.837 | 55.3 |
| strata | users.ndjson | 6.064 | 6.919 | 7.613 | 52.3 |
| ujson | users.ndjson | 4.780 | 7.047 | 7.135 | 55.3 |
| json (stdlib) | users.ndjson | 5.999 | 7.521 | 8.007 | 55.3 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | eval_query (baseline) | 0.048 | 1000 |
|  | jmespath | 0.234 | 1000 |
|  | jsonpath-ng | 1.723 | 1000 |
| $.users[*].orders[*].items[*].price | eval_query (baseline) | 0.637 | 12046 |
|  | jmespath | 6.637 | 1000 |
|  | jsonpath-ng | 20.941 | 12046 |
| $..price | jmespath | 6.446 | 1000 |
|  | eval_query (baseline) | 6.853 | 12046 |
|  | jsonpath-ng | 78.570 | 12046 |
| $.users[?(@.age>30)] | eval_query (baseline) | 0.032 | 794 |
| $..orders[?(@.status=="shipped")] | eval_query (baseline) | 6.783 | 3208 |
