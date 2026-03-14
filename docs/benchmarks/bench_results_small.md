# Strata Benchmark Results

Generated: 2026-03-15 01:23:09

## Environment

- Commit: ad401a8927f1d1bf16672f43d7402ae108e7d54a
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 3.852    | 4.910       | 5.127    | 34.8     |
| msgspec       | users.json   | 5.000    | 6.513       | 7.230    | 41.9     |
| orjson        | users.json   | 4.582    | 6.690       | 6.881    | 38.8     |
| ujson         | users.json   | 5.812    | 8.963       | 10.210   | 40.8     |
| json (stdlib) | users.json   | 10.036   | 11.643      | 12.182   | 42.9     |
| strata        | users.ndjson | 3.762    | 4.850       | 5.187    | 61.3     |
| msgspec       | users.ndjson | 4.633    | 6.358       | 6.525    | 62.4     |
| orjson        | users.ndjson | 5.713    | 6.827       | 9.536    | 62.3     |
| ujson         | users.ndjson | 6.479    | 8.767       | 8.909    | 62.4     |
| json (stdlib) | users.ndjson | 9.564    | 11.237      | 11.874   | 62.4     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 4.130    | 4.657       | 4.869    | 42.9     |
| orjson        | users.json   | 4.774    | 6.520       | 6.634    | 42.9     |
| msgspec       | users.json   | 5.166    | 7.160       | 7.766    | 48.0     |
| ujson         | users.json   | 7.063    | 9.285       | 9.713    | 48.0     |
| json (stdlib) | users.json   | 11.146   | 11.428      | 11.746   | 48.0     |
| strata        | users.ndjson | 4.313    | 4.996       | 5.130    | 62.4     |
| msgspec       | users.ndjson | 4.744    | 6.269       | 6.763    | 63.5     |
| orjson        | users.ndjson | 6.433    | 6.759       | 7.243    | 62.5     |
| ujson         | users.ndjson | 6.673    | 9.181       | 12.159   | 63.5     |
| json (stdlib) | users.ndjson | 10.913   | 13.831      | 13.913   | 63.5     |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 1.434    | 1.458       | 1.553    | 48.9     |
| msgspec | users.json | 1.481    | 1.508       | 1.543    | 52.5     |
| orjson  | users.json | 1.509    | 1.542       | 1.641    | 49.6     |
| ujson   | users.json | 6.254    | 6.342       | 6.481    | 52.8     |
| json    | users.json | 8.812    | 8.831       | 8.911    | 53.0     |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 1.754    | 1.854       | 1.987    | 53.0     |
| orjson        | users.json | 1.842    | 1.895       | 1.930    | 53.0     |
| msgspec       | users.json | 2.032    | 2.043       | 2.171    | 53.0     |
| ujson         | users.json | 6.533    | 6.581       | 6.773    | 53.0     |
| json (stdlib) | users.json | 67.569   | 67.985      | 68.312   | 53.0     |

## search (mem_eff=True)

| Query           | Library                      | Min (ms) | Results | RSS (MB) |
| --------------- | ---------------------------- | -------- | ------- | -------- |
| all user ids    | orjson+jmespath (mem_eff)    | 5.764    | 1000    | 58.4     |
|                 | orjson+jsonpath-ng (mem_eff) | 7.025    | 1000    | 58.9     |
|                 | strata (mem_eff)             | 10.376   | 1000    | 58.3     |
| all item prices | strata (mem_eff)             | 10.919   | 12046   | 58.9     |
|                 | orjson+jmespath (mem_eff)    | 14.558   | 1000    | 59.0     |
|                 | orjson+jsonpath-ng (mem_eff) | 35.278   | 12046   | 58.7     |
| recursive price | strata (mem_eff)             | 10.806   | 12046   | 58.0     |
|                 | orjson+jsonpath-ng (mem_eff) | 127.482  | 12046   | 59.2     |

## search (mem_eff=False)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | orjson+jmespath    | 5.764    | 1000    | 58.4     |
|                 | strata             | 5.767    | 1000    | 53.1     |
|                 | orjson+jsonpath-ng | 7.025    | 1000    | 58.9     |
| all item prices | strata             | 6.295    | 12046   | 58.9     |
|                 | orjson+jmespath    | 14.558   | 1000    | 59.0     |
|                 | orjson+jsonpath-ng | 35.278   | 12046   | 58.7     |
| recursive price | strata             | 6.728    | 12046   | 58.0     |
|                 | orjson+jsonpath-ng | 127.482  | 12046   | 59.2     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.079    | 1000    | 59.2     |
|                                            | jmespath    | 0.399    | 1000    | 59.3     |
|                                            | jsonpath-ng | 4.317    | 1000    | 59.3     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.947    | 12046   | 59.3     |
|                                            | jmespath    | 11.303   | 1000    | 59.3     |
|                                            | jsonpath-ng | 32.444   | 12046   | 62.9     |
| $..price                                   | strata      | 1.283    | 12046   | 61.9     |
|                                            | jsonpath-ng | 124.153  | 12046   | 63.9     |
| $.users\[?(@.age>30)\]                     | strata      | 0.121    | 794     | 62.3     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.520    | 1604    | 61.3     |

## Summary

| Category              | Strata Rank                   | vs #1                              |
| --------------------- | ----------------------------- | ---------------------------------- |
| Parsing (JSON)        | **#1** / 5                    | **18.9% faster** than #2 (orjson)  |
| Parsing (NDJSON)      | **#1** / 5                    | **23.2% faster** than #2 (msgspec) |
| Load JSON (file)      | **#1** / 5                    | **15.6% faster** than #2 (orjson)  |
| Load NDJSON (file)    | **#1** / 5                    | **10.0% faster** than #2 (msgspec) |
| Serialization (dumps) | **#1** / 5                    | **3.3% faster** than #2 (msgspec)  |
| Dump (file)           | **#1** / 5                    | **5.0% faster** than #2 (orjson)   |
| Search (file)         | mem_eff faster in 0/3 queries | -                                  |
| JSONPath              | **#1** in 5/5 queries         | -                                  |
