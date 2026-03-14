# Strata Benchmark Results

Generated: 2026-03-14 22:55:18

## Environment

- Commit: 0e6e809ddab3d4211deac9905bcbd1ca27ed3a66
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 5
- Warmup: 2
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 4.036    | 4.883       | 5.116    | 37.3     |
| orjson        | users.json   | 4.280    | 6.114       | 6.453    | 40.5     |
| msgspec       | users.json   | 4.445    | 6.143       | 6.329    | 42.7     |
| ujson         | users.json   | 5.601    | 8.677       | 8.998    | 42.6     |
| json (stdlib) | users.json   | 8.584    | 9.903       | 10.033   | 44.6     |
| orjson        | users.ndjson | 4.562    | 6.040       | 6.798    | 63.0     |
| msgspec       | users.ndjson | 4.655    | 6.227       | 6.765    | 64.1     |
| ujson         | users.ndjson | 6.431    | 8.623       | 8.801    | 64.1     |
| strata        | users.ndjson | 7.030    | 8.992       | 9.190    | 63.0     |
| json (stdlib) | users.ndjson | 9.638    | 11.193      | 11.290   | 64.1     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 3.791    | 4.647       | 4.734    | 44.6     |
| orjson        | users.json   | 4.090    | 6.091       | 6.222    | 44.6     |
| msgspec       | users.json   | 4.395    | 6.137       | 6.430    | 49.7     |
| ujson         | users.json   | 5.872    | 8.346       | 8.857    | 49.7     |
| json (stdlib) | users.json   | 8.983    | 10.088      | 10.185   | 49.7     |
| strata        | users.ndjson | 4.262    | 4.931       | 5.374    | 64.1     |
| orjson        | users.ndjson | 4.536    | 6.378       | 6.782    | 64.1     |
| msgspec       | users.ndjson | 4.482    | 6.448       | 6.953    | 65.0     |
| ujson         | users.ndjson | 6.410    | 9.055       | 9.266    | 66.0     |
| json (stdlib) | users.ndjson | 9.447    | 11.370      | 11.918   | 65.0     |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| orjson  | users.json | 1.443    | 1.460       | 1.553    | 53.6     |
| msgspec | users.json | 1.459    | 1.463       | 1.475    | 53.7     |
| strata  | users.json | 1.818    | 1.823       | 1.938    | 50.6     |
| ujson   | users.json | 6.148    | 6.160       | 6.197    | 53.7     |
| json    | users.json | 8.631    | 8.646       | 8.792    | 54.5     |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| orjson        | users.json | 1.632    | 1.689       | 1.873    | 54.5     |
| msgspec       | users.json | 1.645    | 1.700       | 2.108    | 54.5     |
| strata        | users.json | 2.005    | 2.018       | 2.053    | 54.5     |
| ujson         | users.json | 6.538    | 6.587       | 6.655    | 54.5     |
| json (stdlib) | users.json | 65.371   | 65.969      | 66.302   | 54.5     |

## search (mem_eff=True)

| Query           | Library                      | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------------------- | -------- | ------- | -------- |
| all user ids    | orjson+jmespath (mem_eff)    | 5.184    | 1000    | 60.0     |
|                 | strata (mem_eff)             | 6.004    | 1000    | 59.9     |
|                 | orjson+jsonpath-ng (mem_eff) | 6.263    | 1000    | 60.5     |
| all item prices | strata (mem_eff)             | 6.697    | 12046   | 60.6     |
|                 | orjson+jmespath (mem_eff)    | 13.103   | 1000    | 60.6     |
|                 | orjson+jsonpath-ng (mem_eff) | 35.081   | 12046   | 60.5     |
| recursive price | strata (mem_eff)             | 6.551    | 12046   | 60.9     |
|                 | orjson+jsonpath-ng (mem_eff) | 124.090  | 12046   | 60.9     |

## search (mem_eff=False)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | orjson+jmespath    | 5.184    | 1000    | 60.0     |
|                 | orjson+jsonpath-ng | 6.263    | 1000    | 60.5     |
|                 | strata             | 6.323    | 1000    | 59.8     |
| all item prices | strata             | 6.429    | 12046   | 60.6     |
|                 | orjson+jmespath    | 13.103   | 1000    | 60.6     |
|                 | orjson+jsonpath-ng | 35.081   | 12046   | 60.5     |
| recursive price | strata             | 12.052   | 12046   | 60.9     |
|                 | orjson+jsonpath-ng | 124.090  | 12046   | 60.9     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.077    | 1000    | 60.9     |
|                                            | jmespath    | 0.355    | 1000    | 60.9     |
|                                            | jsonpath-ng | 4.126    | 1000    | 60.9     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.880    | 12046   | 60.9     |
|                                            | jmespath    | 10.564   | 1000    | 60.9     |
|                                            | jsonpath-ng | 31.610   | 12046   | 63.6     |
| $..price                                   | strata      | 1.196    | 12046   | 62.6     |
|                                            | jsonpath-ng | 120.868  | 12046   | 64.6     |
| $.users\[?(@.age>30)\]                     | strata      | 0.110    | 794     | 62.0     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.418    | 1604    | 62.0     |

## Summary

| Category              | Strata Rank                   | vs #1                             |
| --------------------- | ----------------------------- | --------------------------------- |
| Parsing (JSON)        | **#1** / 5                    | **6.0% faster** than #2 (orjson)  |
| Parsing (NDJSON)      | **#4** / 5                    | 54.1% behind #1 (orjson)          |
| Load JSON (file)      | **#1** / 5                    | **7.9% faster** than #2 (orjson)  |
| Load NDJSON (file)    | **#1** / 5                    | **5.2% faster** than #2 (msgspec) |
| Serialization (dumps) | **#3** / 5                    | 26.0% behind #1 (orjson)          |
| Dump (file)           | **#3** / 5                    | 22.9% behind #1 (orjson)          |
| Search (file)         | mem_eff faster in 1/3 queries | -                                 |
| JSONPath              | **#1** in 5/5 queries         | -                                 |
