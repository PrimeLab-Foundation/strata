# Strata Benchmark Results

Generated: 2026-03-14 01:39:56

## Environment

- Commit: b41fcb56074becce8657c76f62cdb3c4367522ec
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/large/users.json, benchmarks/data/generated/large/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users.json   | 182.039  | 255.339     | 259.016  | 476.8    |
| msgspec       | users.json   | 195.797  | 265.650     | 270.003  | 468.2    |
| strata        | users.json   | 278.004  | 351.805     | 360.663  | 330.0    |
| ujson         | users.json   | 263.319  | 361.794     | 371.619  | 590.6    |
| json (stdlib) | users.json   | 387.416  | 442.375     | 450.857  | 512.1    |
| orjson        | users.ndjson | 237.548  | 287.916     | 290.926  | 454.2    |
| msgspec       | users.ndjson | 245.353  | 297.765     | 302.177  | 448.7    |
| ujson         | users.ndjson | 323.757  | 399.997     | 405.311  | 572.2    |
| strata        | users.ndjson | 405.254  | 458.094     | 464.378  | 431.6    |
| json (stdlib) | users.ndjson | 424.133  | 477.524     | 481.785  | 451.4    |

## Query Benchmarks

| Query                                      | Library               | Min (ms) | Results |
| ------------------------------------------ | --------------------- | -------- | ------- |
| $.users\[\*\].id                           | eval_query (baseline) | 0.871    | 4000    |
|                                            | jmespath              | 2.015    | 4000    |
|                                            | jsonpath-ng           | 42.550   | 4000    |
| $.users\[*\].orders\[*\].items\[\*\].price | eval_query (baseline) | 48.675   | 789913  |
|                                            | jmespath              | 342.530  | 4000    |
|                                            | jsonpath-ng           | 1706.971 | 789913  |
| $..price                                   | jmespath              | 349.181  | 4000    |
|                                            | eval_query (baseline) | 466.508  | 789913  |
|                                            | jsonpath-ng           | 5825.594 | 789913  |
| $.users\[?(@.age>30)\]                     | eval_query (baseline) | 0.745    | 3212    |
| $..orders\[?(@.status=="shipped")\]        | eval_query (baseline) | 466.571  | 52812   |
