# Strata Benchmark Results

Generated: 2026-02-09 02:47:21

## Environment

- Commit: 56fb161e478e029b69335528dad4957c4bdaf79f
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/large/users.json, benchmarks/data/generated/large/users.ndjson

## Parsing Benchmarks

| Library | Dataset | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|---------|----------|-------------|----------|---------|
| orjson | users.json | 135.221 | 190.118 | 194.812 | 712.1 |
| msgspec | users.json | 143.025 | 202.406 | 204.313 | 704.5 |
| strata | users.json | 205.773 | 234.158 | 235.755 | 566.2 |
| ujson | users.json | 202.114 | 279.538 | 291.746 | 826.9 |
| json (stdlib) | users.json | 256.851 | 315.283 | 319.737 | 704.5 |
| orjson | users.ndjson | 169.900 | 206.995 | 210.825 | 595.7 |
| msgspec | users.ndjson | 198.210 | 231.173 | 231.812 | 591.1 |
| strata | users.ndjson | 238.696 | 273.145 | 275.146 | 543.5 |
| ujson | users.ndjson | 243.738 | 299.094 | 305.977 | 714.5 |
| json (stdlib) | users.ndjson | 293.146 | 358.936 | 366.114 | 593.8 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | eval_query (baseline) | 0.639 | 4000 |
|  | jmespath | 1.676 | 4000 |
|  | jsonpath-ng | 35.591 | 4000 |
| $.users[*].orders[*].items[*].price | eval_query (baseline) | 30.890 | 789913 |
|  | jmespath | 228.688 | 4000 |
|  | jsonpath-ng | 1134.373 | 789913 |
| $..price | jmespath | 221.563 | 4000 |
|  | eval_query (baseline) | 278.395 | 789913 |
|  | jsonpath-ng | 3813.787 | 789913 |
| $.users[?(@.age>30)] | eval_query (baseline) | 0.585 | 3212 |
| $..orders[?(@.status=="shipped")] | eval_query (baseline) | 285.933 | 52812 |
