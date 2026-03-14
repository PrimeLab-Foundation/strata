# Strata Benchmark Results

Generated: 2026-03-14 19:09:24

## Environment

- Commit: b1b82c0eadfc3d6c934130915684a2e84fa5df5e
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
| strata        | users.json   | 157.508  | 193.945     | 195.930  | 339.8    |
| orjson        | users.json   | 187.810  | 250.095     | 250.920  | 477.9    |
| msgspec       | users.json   | 190.561  | 266.225     | 267.261  | 468.3    |
| ujson         | users.json   | 260.714  | 358.825     | 367.900  | 591.7    |
| json (stdlib) | users.json   | 365.785  | 438.787     | 443.884  | 511.2    |
| strata        | users.ndjson | 182.539  | 199.451     | 202.769  | 508.2    |
| orjson        | users.ndjson | 234.405  | 285.042     | 288.409  | 559.5    |
| msgspec       | users.ndjson | 243.847  | 296.512     | 300.916  | 554.0    |
| ujson         | users.ndjson | 324.438  | 398.564     | 409.902  | 676.5    |
| json (stdlib) | users.ndjson | 419.890  | 477.358     | 481.127  | 556.7    |

## Serialization Benchmarks

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| msgspec | users.json | 74.366   | 74.631      | 75.216   | 900.3    |
| orjson  | users.json | 74.673   | 74.895      | 75.940   | 703.1    |
| strata  | users.json | 87.123   | 87.172      | 87.261   | 546.7    |
| ujson   | users.json | 277.289  | 277.844     | 278.740  | 1095.5   |
| json    | users.json | 394.345  | 398.197     | 402.550  | 1310.2   |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.708    | 4000    | 1609.0   |
|                                            | jmespath    | 2.082    | 4000    | 1609.4   |
|                                            | jsonpath-ng | 76.568   | 4000    | 1612.6   |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 41.259   | 789913  | 1635.8   |
|                                            | jmespath    | 372.717  | 4000    | 1512.4   |
|                                            | jsonpath-ng | 1563.840 | 789913  | 1523.5   |
| $..price                                   | strata      | 53.409   | 789913  | 1258.4   |
|                                            | jsonpath-ng | 5654.220 | 789913  | 769.7    |
| $.users\[?(@.age>30)\]                     | strata      | 0.816    | 3212    | 772.4    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 59.774   | 26406   | 771.4    |

## Summary

| Category         | Strata Rank           | vs #1                             |
| ---------------- | --------------------- | --------------------------------- |
| Parsing (JSON)   | **#1** / 5            | **19.2% faster** than #2 (orjson) |
| Parsing (NDJSON) | **#1** / 5            | **28.4% faster** than #2 (orjson) |
| Serialization    | **#3** / 5            | 17.2% behind #1 (msgspec)         |
| JSONPath         | **#1** in 5/5 queries | -                                 |
