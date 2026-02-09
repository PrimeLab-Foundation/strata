# Strata Benchmark Results

Generated: 2026-01-30 01:09:54

## Configuration

- Repeat: 5
- Warmup: 2
- Datasets: benchmarks/data/generated/users.json, benchmarks/data/generated/users.ndjson

## Parsing Benchmarks

| Library       | Dataset | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------- | -------- | ----------- | -------- | -------- |
| strata        | users   | ERROR    | -           | -        | -        |
| orjson        | users   | 15.631   | 24.922      | 25.661   | 150.7    |
| strata        | users   | 26.411   | 30.264      | 31.564   | 125.1    |
| msgspec       | users   | 23.104   | 34.150      | 34.811   | 157.3    |
| ujson         | users   | 29.200   | 41.046      | 45.956   | 168.5    |
| json (stdlib) | users   | 35.954   | 43.509      | 51.716   | 165.8    |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | query | 0.343    | 5000    |
|                                            | jmespath              | 1.247    | 5000    |
| $.users\[*\].orders\[*\].items\[\*\].price | query | 3.009    | 62331   |
|                                            | jmespath              | 29.839   | 5000    |
| $..price                                   | jmespath              | 29.714   | 5000    |
|                                            | query | 30.610   | 62331   |
| $.users\[?(@.age>30)\]                     | query | 0.382    | 3999    |
| $..orders\[?(@.status=="shipped")\]        | query | 32.774   | 16804   |
