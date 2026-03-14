# Strata Benchmark Results

Generated: 2026-03-14 17:53:46

## Environment

- Commit: 22c9816d37efe25be99b366140f428346a4b6250
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 3.853    | 4.351       | 4.739    | 35.2     |
| orjson        | users.json   | 4.305    | 6.256       | 7.096    | 38.4     |
| msgspec       | users.json   | 4.537    | 6.506       | 6.636    | 42.5     |
| ujson         | users.json   | 6.184    | 8.669       | 9.316    | 40.5     |
| json (stdlib) | users.json   | 8.493    | 10.248      | 10.623   | 44.3     |
| strata        | users.ndjson | 3.572    | 4.476       | 4.529    | 47.7     |
| orjson        | users.ndjson | 4.391    | 5.854       | 5.953    | 48.7     |
| msgspec       | users.ndjson | 4.433    | 5.987       | 6.165    | 50.7     |
| ujson         | users.ndjson | 5.915    | 8.201       | 8.410    | 50.7     |
| json (stdlib) | users.ndjson | 9.004    | 11.013      | 11.277   | 50.7     |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | eval_query (baseline) | 0.062    | 1000    |
|                                            | jmespath              | 0.330    | 1000    |
|                                            | jsonpath-ng           | 2.707    | 1000    |
| $.users\[*\].orders\[*\].items\[\*\].price | eval_query (baseline) | 1.109    | 12046   |
|                                            | jmespath              | 9.561    | 1000    |
|                                            | jsonpath-ng           | 29.852   | 12046   |
| $..price                                   | jmespath              | 10.085   | 1000    |
|                                            | eval_query (baseline) | 10.671   | 12046   |
|                                            | jsonpath-ng           | 120.434  | 12046   |
| $.users\[?(@.age>30)\]                     | eval_query (baseline) | 0.064    | 794     |
| $..orders\[?(@.status=="shipped")\]        | eval_query (baseline) | 11.083   | 3208    |
