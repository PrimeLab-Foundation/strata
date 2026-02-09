# Strata Benchmark Results

Generated: 2026-02-01 19:47:33

## Environment

- Commit: 0dd6fec82903a7473ecc5709b020843b28cff0e7
- OS: macOS-26.2-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 5
- Warmup: 2
- Datasets: benchmarks/data/generated/users.json, benchmarks/data/generated/users.ndjson

## Parsing Benchmarks

| Library | Dataset | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|---------|----------|-------------|----------|---------|
| orjson | users.json | 22.432 | 32.798 | 33.594 | 114.7 |
| msgspec | users.json | 21.706 | 33.912 | 35.760 | 114.5 |
| ujson | users.json | 31.943 | 47.944 | 48.176 | 128.5 |
| json (stdlib) | users.json | 42.558 | 54.264 | 55.604 | 119.5 |
| strata | users.json | 59.502 | 77.688 | 94.179 | 116.2 |
| orjson | users.ndjson | 27.544 | 29.453 | 33.435 | 121.8 |
| msgspec | users.ndjson | 28.659 | 30.109 | 31.869 | 125.8 |
| ujson | users.ndjson | 39.308 | 42.885 | 44.999 | 124.8 |
| json (stdlib) | users.ndjson | 53.055 | 54.029 | 55.173 | 127.8 |
| strata | users.ndjson | 54.029 | 64.940 | 67.325 | 145.6 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | query | 0.520 | 5000 |
|  | jmespath | 1.891 | 5000 |
|  | jsonpath-ng | 12.432 | 5000 |
| $.users[*].orders[*].items[*].price | query | 5.178 | 62331 |
|  | jmespath | 45.278 | 5000 |
|  | jsonpath-ng | 172.325 | 62331 |
| $..price | jmespath | 48.264 | 5000 |
|  | query | 53.766 | 62331 |
|  | jsonpath-ng | 630.763 | 62331 |
| $.users[?(@.age>30)] | query | 0.547 | 3999 |
| $..orders[?(@.status=="shipped")] | query | 56.715 | 16804 |
