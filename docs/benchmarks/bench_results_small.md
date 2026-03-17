# Strata Benchmark Results

Generated: 2026-03-17 15:14:03

## Environment

- Commit: 75205226403b1b3dd7be908ebcd6e9ece844e9a9
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson
- Random schemas: flat, nested, wide_arrays, mixed (500 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.559    | 0.687       | 0.700    | 62.6     |
| strata        | flat         | 0.592    | 0.696       | 0.718    | 62.0     |
| msgspec       | flat         | 0.666    | 0.737       | 0.793    | 62.6     |
| ujson         | flat         | 1.091    | 1.261       | 1.290    | 62.6     |
| json (stdlib) | flat         | 2.036    | 2.149       | 2.151    | 62.6     |
| orjson        | mixed        | 0.651    | 0.696       | 0.803    | 78.6     |
| msgspec       | mixed        | 0.612    | 0.799       | 0.953    | 78.6     |
| strata        | mixed        | 0.779    | 0.860       | 0.868    | 78.6     |
| ujson         | mixed        | 0.994    | 1.219       | 1.245    | 78.6     |
| json (stdlib) | mixed        | 2.137    | 2.215       | 2.268    | 78.6     |
| strata        | nested       | 0.500    | 0.616       | 0.630    | 62.8     |
| orjson        | nested       | 1.581    | 2.060       | 2.185    | 62.8     |
| ujson         | nested       | 1.962    | 2.363       | 3.104    | 62.8     |
| msgspec       | nested       | 2.480    | 2.900       | 3.739    | 62.8     |
| json (stdlib) | nested       | 2.603    | 2.902       | 2.979    | 62.8     |
| strata        | users.json   | 3.536    | 4.506       | 4.895    | 33.7     |
| orjson        | users.json   | 4.266    | 6.180       | 7.198    | 37.7     |
| msgspec       | users.json   | 4.979    | 6.552       | 7.460    | 40.8     |
| ujson         | users.json   | 6.378    | 9.751       | 12.720   | 40.7     |
| json (stdlib) | users.json   | 10.533   | 10.679      | 12.561   | 43.7     |
| strata        | users.ndjson | 3.555    | 4.423       | 4.543    | 57.4     |
| orjson        | users.ndjson | 4.750    | 5.959       | 6.411    | 58.4     |
| msgspec       | users.ndjson | 6.133    | 6.878       | 9.708    | 60.4     |
| ujson         | users.ndjson | 7.173    | 9.069       | 9.761    | 60.4     |
| json (stdlib) | users.ndjson | 9.529    | 11.532      | 11.762   | 60.4     |
| strata        | wide_arrays  | 3.457    | 4.262       | 4.307    | 65.0     |
| orjson        | wide_arrays  | 4.366    | 4.513       | 4.516    | 67.6     |
| msgspec       | wide_arrays  | 4.467    | 5.491       | 5.793    | 68.5     |
| ujson         | wide_arrays  | 7.051    | 7.662       | 7.747    | 68.5     |
| json (stdlib) | wide_arrays  | 14.120   | 14.594      | 16.704   | 69.9     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.820    | 0.851       | 0.858    | 62.7     |
| strata        | flat         | 0.723    | 0.900       | 0.966    | 62.7     |
| msgspec       | flat         | 0.836    | 0.953       | 1.001    | 62.7     |
| ujson         | flat         | 1.529    | 1.613       | 1.618    | 62.7     |
| json (stdlib) | flat         | 2.247    | 2.277       | 2.314    | 62.7     |
| orjson        | mixed        | 0.566    | 0.727       | 0.781    | 78.6     |
| msgspec       | mixed        | 0.664    | 0.834       | 1.000    | 78.6     |
| strata        | mixed        | 0.903    | 0.953       | 1.046    | 78.6     |
| ujson         | mixed        | 1.176    | 1.335       | 1.351    | 78.6     |
| json (stdlib) | mixed        | 2.462    | 2.494       | 3.021    | 78.6     |
| strata        | nested       | 0.573    | 0.684       | 0.754    | 62.8     |
| msgspec       | nested       | 1.894    | 1.969       | 2.010    | 62.8     |
| orjson        | nested       | 1.579    | 2.009       | 2.052    | 62.8     |
| ujson         | nested       | 1.999    | 2.270       | 2.371    | 62.8     |
| json (stdlib) | nested       | 2.617    | 3.075       | 3.945    | 62.8     |
| strata        | users.json   | 3.359    | 4.175       | 4.467    | 43.7     |
| orjson        | users.json   | 4.578    | 6.036       | 6.211    | 44.7     |
| msgspec       | users.json   | 5.597    | 7.409       | 10.457   | 47.8     |
| ujson         | users.json   | 8.275    | 9.003       | 10.049   | 47.8     |
| json (stdlib) | users.json   | 10.572   | 11.299      | 13.460   | 47.9     |
| strata        | users.ndjson | 3.710    | 5.621       | 5.654    | 60.4     |
| msgspec       | users.ndjson | 5.431    | 6.399       | 6.513    | 61.4     |
| orjson        | users.ndjson | 6.436    | 6.471       | 6.915    | 60.4     |
| ujson         | users.ndjson | 7.023    | 9.529       | 11.030   | 61.4     |
| json (stdlib) | users.ndjson | 9.411    | 12.291      | 14.798   | 61.4     |
| strata        | wide_arrays  | 3.475    | 4.086       | 4.152    | 69.9     |
| orjson        | wide_arrays  | 4.445    | 4.713       | 4.739    | 69.9     |
| msgspec       | wide_arrays  | 4.732    | 6.162       | 8.230    | 71.7     |
| ujson         | wide_arrays  | 7.622    | 9.046       | 10.719   | 71.7     |
| json (stdlib) | wide_arrays  | 14.024   | 15.017      | 15.115   | 71.7     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.237    | 0.239       | 0.245    | 62.7     |
| orjson  | flat        | 0.249    | 0.253       | 0.269    | 62.7     |
| msgspec | flat        | 0.327    | 0.329       | 0.359    | 62.8     |
| ujson   | flat        | 1.208    | 1.211       | 1.217    | 62.8     |
| json    | flat        | 2.336    | 2.496       | 2.831    | 62.8     |
| strata  | mixed       | 0.262    | 0.263       | 0.264    | 78.6     |
| msgspec | mixed       | 0.311    | 0.313       | 0.316    | 78.6     |
| orjson  | mixed       | 0.299    | 0.354       | 0.477    | 78.6     |
| ujson   | mixed       | 1.310    | 1.311       | 1.319    | 78.6     |
| json    | mixed       | 2.983    | 3.023       | 3.024    | 78.6     |
| orjson  | nested      | 0.194    | 0.200       | 0.217    | 62.8     |
| strata  | nested      | 0.188    | 0.201       | 0.203    | 62.8     |
| msgspec | nested      | 0.210    | 0.217       | 0.224    | 62.8     |
| ujson   | nested      | 1.012    | 1.016       | 1.078    | 62.8     |
| json    | nested      | 1.335    | 1.361       | 1.380    | 62.8     |
| strata  | users.json  | 1.086    | 1.116       | 1.208    | 47.9     |
| msgspec | users.json  | 1.482    | 1.544       | 1.688    | 53.8     |
| orjson  | users.json  | 1.506    | 1.578       | 1.670    | 50.6     |
| ujson   | users.json  | 6.308    | 6.547       | 7.212    | 53.9     |
| json    | users.json  | 8.803    | 8.902       | 9.237    | 54.2     |
| strata  | wide_arrays | 1.384    | 1.441       | 1.531    | 72.9     |
| msgspec | wide_arrays | 1.704    | 1.746       | 1.785    | 77.2     |
| orjson  | wide_arrays | 1.578    | 1.747       | 1.780    | 74.7     |
| ujson   | wide_arrays | 7.095    | 7.252       | 7.577    | 78.5     |
| json    | wide_arrays | 20.102   | 20.112      | 20.316   | 78.6     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.384    | 0.407       | 0.409    | 62.8     |
| msgspec       | flat        | 0.493    | 0.577       | 0.736    | 62.8     |
| orjson        | flat        | 0.482    | 0.619       | 0.657    | 62.8     |
| ujson         | flat        | 1.579    | 1.580       | 1.603    | 62.8     |
| json (stdlib) | flat        | 8.414    | 8.661       | 8.791    | 62.8     |
| strata        | mixed       | 0.379    | 0.396       | 0.477    | 78.6     |
| msgspec       | mixed       | 0.523    | 0.555       | 0.614    | 78.6     |
| orjson        | mixed       | 0.528    | 0.589       | 0.603    | 78.6     |
| ujson         | mixed       | 1.545    | 1.582       | 1.590    | 78.6     |
| json (stdlib) | mixed       | 10.258   | 10.261      | 10.487   | 78.6     |
| strata        | nested      | 0.302    | 0.364       | 0.365    | 62.8     |
| orjson        | nested      | 0.367    | 0.371       | 0.389    | 62.8     |
| msgspec       | nested      | 0.413    | 0.560       | 0.632    | 63.0     |
| ujson         | nested      | 1.178    | 1.179       | 1.234    | 62.8     |
| json (stdlib) | nested      | 12.253   | 12.306      | 12.693   | 63.0     |
| strata        | users.json  | 1.292    | 1.318       | 1.417    | 54.2     |
| orjson        | users.json  | 1.731    | 1.790       | 1.824    | 54.2     |
| msgspec       | users.json  | 1.766    | 2.143       | 2.254    | 54.2     |
| ujson         | users.json  | 6.778    | 6.803       | 6.926    | 54.2     |
| json (stdlib) | users.json  | 67.597   | 67.905      | 68.085   | 54.2     |
| strata        | wide_arrays | 1.831    | 1.939       | 2.091    | 78.6     |
| orjson        | wide_arrays | 1.956    | 2.091       | 2.219    | 78.6     |
| msgspec       | wide_arrays | 2.398    | 2.508       | 3.034    | 78.6     |
| ujson         | wide_arrays | 7.659    | 7.953       | 8.198    | 78.6     |
| json (stdlib) | wide_arrays | 45.446   | 45.992      | 46.250   | 78.6     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.604    | 1000    | 54.3     |
|                                | orjson+jmespath    | 5.855    | 1000    | 54.4     |
|                                | orjson+jsonpath-ng | 6.908    | 1000    | 55.0     |
| all item prices                | strata             | 1.912    | 12046   | 55.1     |
|                                | orjson+jmespath    | 14.405   | 1000    | 55.1     |
|                                | orjson+jsonpath-ng | 36.544   | 12046   | 55.0     |
| recursive price                | strata             | 1.788    | 12046   | 54.7     |
|                                | orjson+jsonpath-ng | 129.327  | 12046   | 55.5     |
| flat:$.records\[\*\].id        | strata             | 0.374    | 500     | 62.8     |
|                                | orjson+jmespath    | 0.872    | 500     | 62.8     |
|                                | orjson+jsonpath-ng | 2.341    | 500     | 62.8     |
| flat:$.records\[0\]            | strata             | 0.408    | 1       | 62.8     |
|                                | orjson+jmespath    | 0.678    | 1       | 62.8     |
|                                | orjson+jsonpath-ng | 0.737    | 1       | 62.8     |
| nested:$.records\[\*\].id      | strata             | 0.299    | 500     | 63.0     |
|                                | orjson+jmespath    | 1.813    | 500     | 63.0     |
|                                | orjson+jsonpath-ng | 2.394    | 500     | 63.0     |
| nested:$.records\[0\]          | strata             | 0.313    | 1       | 63.0     |
|                                | orjson+jsonpath-ng | 1.826    | 1       | 63.0     |
|                                | orjson+jmespath    | 1.883    | 1       | 63.0     |
| wide_arrays:$.records\[\*\].id | strata             | 2.779    | 500     | 78.6     |
|                                | orjson+jmespath    | 4.120    | 500     | 78.6     |
|                                | orjson+jsonpath-ng | 4.728    | 500     | 78.6     |
| wide_arrays:$.records\[0\]     | strata             | 2.656    | 1       | 78.6     |
|                                | orjson+jsonpath-ng | 4.173    | 1       | 78.6     |
|                                | orjson+jmespath    | 4.539    | 1       | 78.6     |
| mixed:$.records\[\*\].id       | strata             | 0.517    | 500     | 78.6     |
|                                | orjson+jmespath    | 0.806    | 500     | 78.6     |
|                                | orjson+jsonpath-ng | 2.329    | 500     | 78.6     |
| mixed:$.records\[0\]           | strata             | 0.479    | 1       | 78.6     |
|                                | orjson+jmespath    | 0.634    | 1       | 78.6     |
|                                | orjson+jsonpath-ng | 0.658    | 1       | 78.6     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.072    | 1000    | 56.3     |
|                                            | jmespath    | 0.412    | 1000    | 56.3     |
|                                            | jsonpath-ng | 4.029    | 1000    | 56.3     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.902    | 12046   | 56.3     |
|                                            | jmespath    | 10.568   | 1000    | 56.3     |
|                                            | jsonpath-ng | 32.290   | 12046   | 59.0     |
| $..price                                   | strata      | 1.240    | 12046   | 57.0     |
|                                            | jsonpath-ng | 128.362  | 12046   | 60.0     |
| $.users\[?(@.age>30)\]                     | strata      | 0.116    | 794     | 57.4     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.461    | 1604    | 57.4     |
| flat:$.records\[\*\].id                    | strata      | 0.035    | 500     | 62.8     |
|                                            | jmespath    | 0.174    | 500     | 62.8     |
|                                            | jsonpath-ng | 1.552    | 500     | 62.8     |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 62.8     |
|                                            | jmespath    | 0.008    | 1       | 62.8     |
|                                            | jsonpath-ng | 0.013    | 1       | 62.8     |
| nested:$.records\[\*\].id                  | strata      | 0.026    | 500     | 63.0     |
|                                            | jmespath    | 0.200    | 500     | 63.0     |
|                                            | jsonpath-ng | 1.910    | 500     | 63.0     |
| nested:$.records\[0\]                      | strata      | 0.002    | 1       | 63.0     |
|                                            | jsonpath-ng | 0.006    | 1       | 63.0     |
|                                            | jmespath    | 0.007    | 1       | 63.0     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.031    | 500     | 78.6     |
|                                            | jmespath    | 0.181    | 500     | 78.6     |
|                                            | jsonpath-ng | 2.235    | 500     | 78.6     |
| wide_arrays:$.records\[0\]                 | strata      | 0.003    | 1       | 78.6     |
|                                            | jmespath    | 0.006    | 1       | 78.6     |
|                                            | jsonpath-ng | 0.009    | 1       | 78.6     |
| mixed:$.records\[\*\].id                   | strata      | 0.030    | 500     | 78.6     |
|                                            | jmespath    | 0.175    | 500     | 78.6     |
|                                            | jsonpath-ng | 1.878    | 500     | 78.6     |
| mixed:$.records\[0\]                       | strata      | 0.006    | 1       | 78.6     |
|                                            | jsonpath-ng | 0.007    | 1       | 78.6     |
|                                            | jmespath    | 0.008    | 1       | 78.6     |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **20.7% faster** than #2 (orjson)  |
| loads (NDJSON)      | **#1** / 5              | **33.6% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **36.3% faster** than #2 (orjson)  |
| load (NDJSON file)  | **#1** / 5              | **46.4% faster** than #2 (msgspec) |
| dumps               | **#1** / 5              | **36.5% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **34.0% faster** than #2 (orjson)  |
| loads (flat)        | **#2** / 5              | 5.8% behind #1 (orjson)            |
| load (flat)         | **#1** / 5              | **13.4% faster** than #2 (orjson)  |
| dumps (flat)        | **#1** / 5              | **4.8% faster** than #2 (orjson)   |
| dump (flat)         | **#1** / 5              | **25.6% faster** than #2 (orjson)  |
| loads (nested)      | **#1** / 5              | **216.4% faster** than #2 (orjson) |
| load (nested)       | **#1** / 5              | **175.7% faster** than #2 (orjson) |
| dumps (nested)      | **#1** / 5              | **3.2% faster** than #2 (orjson)   |
| dump (nested)       | **#1** / 5              | **21.6% faster** than #2 (orjson)  |
| loads (wide_arrays) | **#1** / 5              | **26.3% faster** than #2 (orjson)  |
| load (wide_arrays)  | **#1** / 5              | **27.9% faster** than #2 (orjson)  |
| dumps (wide_arrays) | **#1** / 5              | **14.0% faster** than #2 (orjson)  |
| dump (wide_arrays)  | **#1** / 5              | **6.8% faster** than #2 (orjson)   |
| loads (mixed)       | **#3** / 5              | 27.2% behind #1 (msgspec)          |
| load (mixed)        | **#3** / 5              | 59.6% behind #1 (orjson)           |
| dumps (mixed)       | **#1** / 5              | **14.1% faster** than #2 (orjson)  |
| dump (mixed)        | **#1** / 5              | **38.0% faster** than #2 (msgspec) |
| search (JSONPath)   | **#1** in 11/11 queries | avg **857% faster**                |
| query (JSONPath)    | **#1** in 13/13 queries | -                                  |
