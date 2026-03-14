# Strata Benchmark Results

Generated: 2026-03-14 22:16:16

## Environment

- Commit: a41eea07ede3c1eca4ad03b9ad1a728387619922
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
| strata        | users.json   | 156.686  | 195.067     | 195.604  | 338.9    |
| orjson        | users.json   | 182.922  | 252.202     | 266.572  | 476.9    |
| msgspec       | users.json   | 193.660  | 267.207     | 271.884  | 468.4    |
| ujson         | users.json   | 261.574  | 362.833     | 366.380  | 591.8    |
| json (stdlib) | users.json   | 374.662  | 450.691     | 452.568  | 512.3    |
| orjson        | users.ndjson | 239.203  | 293.321     | 297.671  | 568.2    |
| msgspec       | users.ndjson | 266.487  | 306.839     | 307.657  | 564.7    |
| strata        | users.ndjson | 294.730  | 354.908     | 369.796  | 568.1    |
| ujson         | users.ndjson | 326.176  | 415.395     | 420.647  | 688.2    |
| json (stdlib) | users.ndjson | 436.226  | 488.352     | 497.584  | 567.4    |

## Serialization Benchmarks

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| msgspec | users.json | 74.490   | 74.947      | 76.091   | 900.0    |
| orjson  | users.json | 76.189   | 76.474      | 77.347   | 704.5    |
| strata  | users.json | 86.200   | 86.218      | 89.160   | 546.8    |
| ujson   | users.json | 280.610  | 282.034     | 285.268  | 1095.9   |
| json    | users.json | 400.296  | 401.834     | 403.595  | 1310.5   |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.724    | 4000    | 1609.4   |
|                                            | jmespath    | 2.130    | 4000    | 1609.7   |
|                                            | jsonpath-ng | 78.621   | 4000    | 1612.9   |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 41.031   | 789913  | 1640.0   |
|                                            | jmespath    | 386.012  | 4000    | 1510.6   |
|                                            | jsonpath-ng | 1580.442 | 789913  | 1528.0   |
| $..price                                   | strata      | 53.798   | 789913  | 1269.0   |
|                                            | jsonpath-ng | 5781.294 | 789913  | 780.3    |
| $.users\[?(@.age>30)\]                     | strata      | 0.859    | 3212    | 783.0    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 57.760   | 26406   | 782.0    |

## Summary

| Category         | Strata Rank           | vs #1                             |
| ---------------- | --------------------- | --------------------------------- |
| Parsing (JSON)   | **#1** / 5            | **16.7% faster** than #2 (orjson) |
| Parsing (NDJSON) | **#3** / 5            | 23.2% behind #1 (orjson)          |
| Serialization    | **#3** / 5            | 15.7% behind #1 (msgspec)         |
| JSONPath         | **#1** in 5/5 queries | -                                 |
