# Strata Benchmark Results

Generated: 2026-01-31 00:28:45

## Configuration

- Repeat: 5
- Warmup: 2
- Datasets: benchmarks/data/generated/users.json, benchmarks/data/generated/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users        | 15.496   | 20.320      | 21.389   | 99.6     |
| orjson        | users        | 16.795   | 25.510      | 28.139   | 130.2    |
| msgspec       | users        | 23.253   | 34.070      | 48.149   | 136.9    |
| ujson         | users        | 29.099   | 40.945      | 43.955   | 148.0    |
| json (stdlib) | users        | 36.671   | 45.576      | 46.781   | 145.4    |
| orjson        | users.ndjson | 19.142   | 29.285      | 32.571   | 177.7    |
| msgspec       | users.ndjson | 19.266   | 29.480      | 30.766   | 177.3    |
| ujson         | users.ndjson | 24.672   | 37.929      | 39.040   | 162.8    |
| json (stdlib) | users.ndjson | 33.064   | 42.856      | 45.585   | 161.0    |
| strata        | users.ndjson | 36.528   | 44.791      | 51.596   | 152.6    |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | query | 0.310    | 5000    |
|                                            | jmespath              | 1.207    | 5000    |
| $.users\[*\].orders\[*\].items\[\*\].price | query | 3.154    | 62331   |
|                                            | jmespath              | 30.919   | 5000    |
| $..price                                   | jmespath              | 29.566   | 5000    |
|                                            | query | 30.852   | 62331   |
| $.users\[?(@.age>30)\]                     | query | 0.373    | 3999    |
| $..orders\[?(@.status=="shipped")\]        | query | 32.636   | 16804   |
