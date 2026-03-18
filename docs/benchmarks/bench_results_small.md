# Strata Benchmark Results

Generated: 2026-03-18 19:50:50

## Environment

- Commit: f5870e0f4dbf614611ada7aa37a627b2e43d1579
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
| orjson        | flat         | 0.572    | 0.693       | 0.794    | 63.7     |
| msgspec       | flat         | 0.628    | 0.788       | 0.795    | 63.7     |
| strata        | flat         | 0.702    | 0.878       | 0.920    | 63.1     |
| ujson         | flat         | 1.140    | 1.373       | 1.456    | 63.7     |
| json (stdlib) | flat         | 2.056    | 2.110       | 2.129    | 63.7     |
| orjson        | mixed        | 0.492    | 0.612       | 0.648    | 78.1     |
| msgspec       | mixed        | 0.620    | 0.769       | 0.769    | 78.1     |
| strata        | mixed        | 0.762    | 0.876       | 0.886    | 78.1     |
| ujson         | mixed        | 1.039    | 1.176       | 1.231    | 78.1     |
| json (stdlib) | mixed        | 2.318    | 2.372       | 2.390    | 78.1     |
| strata        | nested       | 0.653    | 0.724       | 0.740    | 63.8     |
| msgspec       | nested       | 1.758    | 1.946       | 2.204    | 63.8     |
| ujson         | nested       | 1.751    | 2.291       | 2.640    | 63.8     |
| orjson        | nested       | 1.877    | 2.428       | 2.922    | 63.8     |
| json (stdlib) | nested       | 2.591    | 2.619       | 2.692    | 63.8     |
| strata        | users.json   | 3.830    | 4.775       | 4.927    | 34.0     |
| orjson        | users.json   | 4.919    | 5.967       | 6.860    | 38.0     |
| msgspec       | users.json   | 6.483    | 6.739       | 7.957    | 41.2     |
| ujson         | users.json   | 6.532    | 9.111       | 11.408   | 41.0     |
| json (stdlib) | users.json   | 8.909    | 10.196      | 10.405   | 43.9     |
| strata        | users.ndjson | 3.964    | 4.908       | 5.037    | 58.4     |
| msgspec       | users.ndjson | 4.527    | 6.276       | 6.403    | 61.5     |
| orjson        | users.ndjson | 4.699    | 7.256       | 8.222    | 59.4     |
| ujson         | users.ndjson | 7.143    | 9.362       | 9.395    | 61.5     |
| json (stdlib) | users.ndjson | 9.404    | 11.283      | 11.558   | 61.5     |
| strata        | wide_arrays  | 2.868    | 3.493       | 3.831    | 65.5     |
| orjson        | wide_arrays  | 3.370    | 4.280       | 4.435    | 68.1     |
| msgspec       | wide_arrays  | 5.480    | 6.900       | 7.323    | 68.2     |
| ujson         | wide_arrays  | 6.591    | 7.545       | 7.583    | 68.2     |
| json (stdlib) | wide_arrays  | 13.540   | 14.038      | 14.199   | 68.5     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| msgspec       | flat         | 0.724    | 0.807       | 0.821    | 63.7     |
| orjson        | flat         | 0.778    | 0.807       | 0.855    | 63.7     |
| strata        | flat         | 0.849    | 0.938       | 0.976    | 63.7     |
| ujson         | flat         | 1.214    | 1.360       | 1.402    | 63.7     |
| json (stdlib) | flat         | 2.132    | 2.346       | 2.353    | 63.7     |
| msgspec       | mixed        | 0.697    | 0.808       | 0.855    | 78.1     |
| orjson        | mixed        | 0.735    | 0.814       | 0.815    | 78.1     |
| strata        | mixed        | 1.015    | 1.308       | 1.323    | 78.1     |
| ujson         | mixed        | 1.349    | 1.384       | 1.544    | 78.1     |
| json (stdlib) | mixed        | 2.252    | 2.338       | 2.371    | 78.1     |
| strata        | nested       | 0.694    | 0.785       | 0.787    | 63.8     |
| msgspec       | nested       | 1.597    | 1.910       | 2.585    | 63.8     |
| orjson        | nested       | 1.613    | 1.912       | 2.212    | 63.8     |
| ujson         | nested       | 1.866    | 2.180       | 2.194    | 63.8     |
| json (stdlib) | nested       | 3.464    | 4.359       | 4.603    | 63.8     |
| strata        | users.json   | 3.681    | 4.588       | 4.849    | 44.0     |
| orjson        | users.json   | 4.710    | 6.410       | 6.723    | 44.0     |
| msgspec       | users.json   | 5.464    | 7.103       | 8.539    | 47.1     |
| ujson         | users.json   | 6.640    | 9.821       | 13.759   | 47.1     |
| json (stdlib) | users.json   | 9.114    | 10.530      | 11.189   | 48.2     |
| strata        | users.ndjson | 4.111    | 4.956       | 5.180    | 61.5     |
| msgspec       | users.ndjson | 4.625    | 6.280       | 6.589    | 62.5     |
| orjson        | users.ndjson | 4.426    | 6.742       | 8.182    | 61.5     |
| ujson         | users.ndjson | 9.173    | 9.800       | 9.816    | 62.5     |
| json (stdlib) | users.ndjson | 9.242    | 10.961      | 11.214   | 62.5     |
| strata        | wide_arrays  | 3.100    | 3.753       | 3.777    | 68.5     |
| orjson        | wide_arrays  | 4.064    | 4.677       | 5.073    | 68.5     |
| msgspec       | wide_arrays  | 4.659    | 5.297       | 5.743    | 71.1     |
| ujson         | wide_arrays  | 7.909    | 7.964       | 8.060    | 71.1     |
| json (stdlib) | wide_arrays  | 13.637   | 14.592      | 17.658   | 71.1     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.234    | 0.243       | 0.400    | 63.7     |
| orjson  | flat        | 0.261    | 0.263       | 0.265    | 63.7     |
| msgspec | flat        | 0.309    | 0.332       | 0.333    | 63.7     |
| ujson   | flat        | 1.233    | 1.249       | 1.275    | 63.7     |
| json    | flat        | 2.285    | 2.295       | 2.383    | 63.7     |
| strata  | mixed       | 0.263    | 0.263       | 0.264    | 78.1     |
| orjson  | mixed       | 0.284    | 0.285       | 0.285    | 78.1     |
| msgspec | mixed       | 0.308    | 0.311       | 0.313    | 78.1     |
| ujson   | mixed       | 1.310    | 1.314       | 1.314    | 78.1     |
| json    | mixed       | 2.986    | 2.998       | 3.023    | 78.2     |
| strata  | nested      | 0.183    | 0.191       | 0.208    | 63.8     |
| orjson  | nested      | 0.197    | 0.211       | 0.240    | 63.8     |
| msgspec | nested      | 0.208    | 0.224       | 0.232    | 63.8     |
| ujson   | nested      | 1.020    | 1.030       | 1.121    | 63.8     |
| json    | nested      | 1.335    | 1.342       | 1.355    | 63.8     |
| strata  | users.json  | 1.046    | 1.050       | 1.146    | 49.0     |
| msgspec | users.json  | 1.494    | 1.498       | 1.582    | 52.2     |
| orjson  | users.json  | 1.500    | 1.539       | 1.632    | 51.7     |
| ujson   | users.json  | 6.498    | 6.500       | 6.558    | 54.4     |
| json    | users.json  | 8.895    | 8.939       | 9.415    | 54.9     |
| strata  | wide_arrays | 1.394    | 1.492       | 1.508    | 72.4     |
| orjson  | wide_arrays | 1.589    | 1.707       | 1.848    | 74.2     |
| msgspec | wide_arrays | 1.672    | 1.709       | 1.730    | 74.2     |
| ujson   | wide_arrays | 7.023    | 7.096       | 7.386    | 74.8     |
| json    | wide_arrays | 20.061   | 20.116      | 20.291   | 76.2     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 0.451    | 0.455       | 0.456    | 63.7     |
| strata        | flat        | 0.454    | 0.459       | 0.468    | 63.7     |
| msgspec       | flat        | 0.501    | 0.555       | 0.584    | 63.7     |
| ujson         | flat        | 1.409    | 1.449       | 1.490    | 63.7     |
| json (stdlib) | flat        | 8.509    | 8.721       | 8.830    | 63.7     |
| strata        | mixed       | 0.405    | 0.433       | 0.452    | 78.2     |
| orjson        | mixed       | 0.480    | 0.503       | 0.559    | 78.2     |
| msgspec       | mixed       | 0.653    | 0.837       | 0.902    | 78.2     |
| ujson         | mixed       | 1.568    | 1.602       | 1.609    | 78.2     |
| json (stdlib) | mixed       | 10.091   | 10.356      | 10.397   | 78.2     |
| strata        | nested      | 0.317    | 0.326       | 0.354    | 63.8     |
| orjson        | nested      | 0.373    | 0.388       | 0.398    | 63.8     |
| msgspec       | nested      | 0.417    | 0.527       | 0.566    | 63.8     |
| ujson         | nested      | 1.299    | 1.304       | 1.525    | 63.8     |
| json (stdlib) | nested      | 12.064   | 12.234      | 12.498   | 63.8     |
| strata        | users.json  | 1.306    | 1.449       | 1.489    | 54.9     |
| orjson        | users.json  | 1.755    | 1.851       | 1.900    | 54.9     |
| msgspec       | users.json  | 1.825    | 1.873       | 2.114    | 54.9     |
| ujson         | users.json  | 6.691    | 6.740       | 6.817    | 54.9     |
| json (stdlib) | users.json  | 67.120   | 67.242      | 67.339   | 54.9     |
| strata        | wide_arrays | 1.668    | 1.712       | 1.735    | 76.2     |
| orjson        | wide_arrays | 1.975    | 2.012       | 2.022    | 76.2     |
| msgspec       | wide_arrays | 2.445    | 2.454       | 2.615    | 78.1     |
| ujson         | wide_arrays | 7.593    | 7.706       | 8.106    | 78.1     |
| json (stdlib) | wide_arrays | 44.435   | 44.928      | 44.996   | 78.1     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.955    | 1000    | 55.0     |
|                                | orjson+jmespath    | 5.624    | 1000    | 55.1     |
|                                | orjson+jsonpath-ng | 6.848    | 1000    | 55.7     |
| all item prices                | strata             | 2.106    | 12046   | 55.7     |
|                                | orjson+jmespath    | 14.055   | 1000    | 55.7     |
|                                | orjson+jsonpath-ng | 34.974   | 12046   | 55.6     |
| recursive price                | strata             | 2.198    | 12046   | 55.2     |
|                                | orjson+jsonpath-ng | 127.068  | 12046   | 56.1     |
| flat:$.records\[\*\].id        | strata             | 0.517    | 500     | 63.7     |
|                                | orjson+jmespath    | 0.836    | 500     | 63.7     |
|                                | orjson+jsonpath-ng | 2.424    | 500     | 63.7     |
| flat:$.records\[0\]            | strata             | 0.490    | 1       | 63.7     |
|                                | orjson+jsonpath-ng | 0.662    | 1       | 63.7     |
|                                | orjson+jmespath    | 0.680    | 1       | 63.7     |
| nested:$.records\[\*\].id      | strata             | 0.418    | 500     | 63.8     |
|                                | orjson+jmespath    | 2.276    | 500     | 63.8     |
|                                | orjson+jsonpath-ng | 2.474    | 500     | 63.8     |
| nested:$.records\[0\]          | strata             | 0.376    | 1       | 63.8     |
|                                | orjson+jsonpath-ng | 1.590    | 1       | 63.8     |
|                                | orjson+jmespath    | 1.664    | 1       | 63.8     |
| wide_arrays:$.records\[\*\].id | strata             | 2.455    | 500     | 78.1     |
|                                | orjson+jmespath    | 4.580    | 500     | 78.1     |
|                                | orjson+jsonpath-ng | 4.684    | 500     | 78.1     |
| wide_arrays:$.records\[0\]     | strata             | 2.309    | 1       | 78.1     |
|                                | orjson+jsonpath-ng | 3.987    | 1       | 78.1     |
|                                | orjson+jmespath    | 4.023    | 1       | 78.1     |
| mixed:$.records\[\*\].id       | strata             | 0.485    | 500     | 78.2     |
|                                | orjson+jmespath    | 0.799    | 500     | 78.2     |
|                                | orjson+jsonpath-ng | 2.333    | 500     | 78.2     |
| mixed:$.records\[0\]           | strata             | 0.482    | 1       | 78.2     |
|                                | orjson+jsonpath-ng | 0.628    | 1       | 78.2     |
|                                | orjson+jmespath    | 0.636    | 1       | 78.2     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.063    | 1000    | 56.9     |
|                                            | jmespath    | 0.374    | 1000    | 56.9     |
|                                            | jsonpath-ng | 3.806    | 1000    | 56.9     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.868    | 12046   | 56.9     |
|                                            | jmespath    | 10.823   | 1000    | 56.9     |
|                                            | jsonpath-ng | 33.335   | 12046   | 59.8     |
| $..price                                   | strata      | 1.355    | 12046   | 57.9     |
|                                            | jsonpath-ng | 125.185  | 12046   | 60.9     |
| $.users\[?(@.age>30)\]                     | strata      | 0.103    | 794     | 58.3     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.535    | 1604    | 58.3     |
| flat:$.records\[\*\].id                    | strata      | 0.032    | 500     | 63.8     |
|                                            | jmespath    | 0.171    | 500     | 63.8     |
|                                            | jsonpath-ng | 1.608    | 500     | 63.8     |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 63.8     |
|                                            | jmespath    | 0.007    | 1       | 63.8     |
|                                            | jsonpath-ng | 0.013    | 1       | 63.8     |
| nested:$.records\[\*\].id                  | strata      | 0.029    | 500     | 63.8     |
|                                            | jmespath    | 0.171    | 500     | 63.8     |
|                                            | jsonpath-ng | 1.728    | 500     | 63.8     |
| nested:$.records\[0\]                      | strata      | 0.002    | 1       | 63.8     |
|                                            | jmespath    | 0.009    | 1       | 63.8     |
|                                            | jsonpath-ng | 0.010    | 1       | 63.8     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.037    | 500     | 78.1     |
|                                            | jmespath    | 0.181    | 500     | 78.1     |
|                                            | jsonpath-ng | 2.223    | 500     | 78.1     |
| wide_arrays:$.records\[0\]                 | strata      | 0.003    | 1       | 78.1     |
|                                            | jmespath    | 0.007    | 1       | 78.1     |
|                                            | jsonpath-ng | 0.010    | 1       | 78.1     |
| mixed:$.records\[\*\].id                   | strata      | 0.029    | 500     | 78.2     |
|                                            | jmespath    | 0.172    | 500     | 78.2     |
|                                            | jsonpath-ng | 1.698    | 500     | 78.2     |
| mixed:$.records\[0\]                       | strata      | 0.008    | 1       | 78.2     |
|                                            | jsonpath-ng | 0.008    | 1       | 78.2     |
|                                            | jmespath    | 0.019    | 1       | 78.2     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **28.4% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **14.2% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **27.9% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **7.7% faster** than #2 (orjson)    |
| dumps               | **#1** / 5              | **42.8% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **34.3% faster** than #2 (orjson)   |
| loads (flat)        | **#3** / 5              | 22.8% behind #1 (orjson)            |
| load (flat)         | **#3** / 5              | 17.2% behind #1 (msgspec)           |
| dumps (flat)        | **#1** / 5              | **11.4% faster** than #2 (orjson)   |
| dump (flat)         | **#2** / 5              | 0.8% behind #1 (orjson)             |
| loads (nested)      | **#1** / 5              | **168.1% faster** than #2 (ujson)   |
| load (nested)       | **#1** / 5              | **130.2% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **7.8% faster** than #2 (orjson)    |
| dump (nested)       | **#1** / 5              | **17.5% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#1** / 5              | **17.5% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **31.1% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **14.0% faster** than #2 (orjson)   |
| dump (wide_arrays)  | **#1** / 5              | **18.4% faster** than #2 (orjson)   |
| loads (mixed)       | **#3** / 5              | 54.8% behind #1 (orjson)            |
| load (mixed)        | **#3** / 5              | 45.6% behind #1 (msgspec)           |
| dumps (mixed)       | **#1** / 5              | **8.0% faster** than #2 (orjson)    |
| dump (mixed)        | **#1** / 5              | **18.5% faster** than #2 (orjson)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **687% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
