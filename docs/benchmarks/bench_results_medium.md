# Strata Benchmark Results

Generated: 2026-03-14 18:02:22

## Environment

- Commit: 22c9816d37efe25be99b366140f428346a4b6250
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/medium/users.json, benchmarks/data/generated/medium/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 23.312   | 29.474      | 29.818   | 72.3     |
| orjson        | users.json   | 27.416   | 39.442      | 41.290   | 92.3     |
| msgspec       | users.json   | 29.090   | 42.789      | 43.948   | 92.1     |
| ujson         | users.json   | 40.435   | 57.976      | 58.012   | 110.7    |
| json (stdlib) | users.json   | 54.314   | 68.371      | 70.636   | 98.3     |
| strata        | users.ndjson | 22.659   | 29.123      | 30.037   | 107.2    |
| orjson        | users.ndjson | 32.247   | 37.091      | 37.361   | 114.3    |
| msgspec       | users.ndjson | 34.770   | 37.808      | 39.950   | 114.1    |
| ujson         | users.ndjson | 48.092   | 52.165      | 54.083   | 131.8    |
| json (stdlib) | users.ndjson | 62.820   | 64.962      | 67.116   | 116.6    |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | eval_query (baseline) | 0.266    | 2000    |
|                                            | jmespath              | 0.835    | 2000    |
|                                            | jsonpath-ng           | 9.690    | 2000    |
| $.users\[*\].orders\[*\].items\[\*\].price | eval_query (baseline) | 7.082    | 100999  |
|                                            | jmespath              | 52.156   | 2000    |
|                                            | jsonpath-ng           | 217.509  | 100999  |
| $..price                                   | jmespath              | 53.346   | 2000    |
|                                            | eval_query (baseline) | 66.361   | 100999  |
|                                            | jsonpath-ng           | 809.864  | 100999  |
| $.users\[?(@.age>30)\]                     | eval_query (baseline) | 0.252    | 1606    |
| $..orders\[?(@.status=="shipped")\]        | eval_query (baseline) | 68.304   | 13300   |
