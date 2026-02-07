# Strata Benchmark Results

Generated: 2026-02-01 03:00:37

## Environment

- Commit: 0dd6fec82903a7473ecc5709b020843b28cff0e7
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/large/users.json, benchmarks/data/generated/large/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users.json   | 125.077  | 179.587     | 182.365  | 759.6    |
| msgspec       | users.json   | 134.371  | 186.903     | 193.100  | 751.1    |
| ujson         | users.json   | 184.772  | 265.163     | 307.450  | 873.5    |
| json (stdlib) | users.json   | 243.662  | 298.173     | 305.675  | 795.0    |
| strata        | users.json   | 310.084  | 367.856     | 436.472  | 771.0    |
| orjson        | users.ndjson | 169.214  | 206.709     | 217.506  | 774.8    |
| msgspec       | users.ndjson | 179.410  | 222.963     | 224.961  | 769.2    |
| ujson         | users.ndjson | 276.827  | 303.357     | 305.885  | 892.7    |
| strata        | users.ndjson | 281.798  | 341.934     | 346.893  | 873.6    |
| json (stdlib) | users.ndjson | 295.602  | 361.121     | 399.635  | 761.6    |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | eval_query (baseline) | 0.621    | 4000    |
|                                            | jmespath              | 1.570    | 4000    |
|                                            | jsonpath-ng           | 32.151   | 4000    |
| $.users\[*\].orders\[*\].items\[\*\].price | eval_query (baseline) | 27.291   | 789913  |
|                                            | jmespath              | 215.874  | 4000    |
|                                            | jsonpath-ng           | 1080.817 | 789913  |
| $..price                                   | jmespath              | 218.731  | 4000    |
|                                            | eval_query (baseline) | 264.605  | 789913  |
|                                            | jsonpath-ng           | 3809.092 | 789913  |
| $.users\[?(@.age>30)\]                     | eval_query (baseline) | 0.597    | 3212    |
| $..orders\[?(@.status=="shipped")\]        | eval_query (baseline) | 281.879  | 52812   |
