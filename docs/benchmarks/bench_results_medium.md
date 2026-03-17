# Strata Benchmark Results

Generated: 2026-03-17 23:11:30

## Environment

- Commit: 424c79cc06825213a77b7bb3e900fe94cce6309b
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/medium/users.json, benchmarks/data/generated/medium/users.ndjson
- Random schemas: flat, nested, wide_arrays, mixed (2000 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 2.820    | 3.449       | 3.600    | 271.2    |
| orjson        | flat         | 4.290    | 4.743       | 5.017    | 273.7    |
| msgspec       | flat         | 4.145    | 4.936       | 5.434    | 273.7    |
| ujson         | flat         | 6.496    | 6.668       | 7.068    | 273.7    |
| json (stdlib) | flat         | 9.752    | 10.853      | 12.724   | 273.7    |
| strata        | mixed        | 2.878    | 3.401       | 3.502    | 294.5    |
| orjson        | mixed        | 4.381    | 5.434       | 5.985    | 296.7    |
| msgspec       | mixed        | 5.140    | 6.840       | 7.435    | 296.7    |
| ujson         | mixed        | 7.004    | 7.035       | 7.874    | 296.7    |
| json (stdlib) | mixed        | 11.372   | 12.405      | 13.795   | 296.7    |
| strata        | nested       | 2.624    | 3.012       | 3.560    | 277.5    |
| msgspec       | nested       | 6.142    | 7.554       | 8.022    | 279.9    |
| ujson         | nested       | 5.835    | 7.802       | 7.938    | 279.9    |
| orjson        | nested       | 5.354    | 8.911       | 15.243   | 279.9    |
| json (stdlib) | nested       | 10.242   | 10.593      | 12.628   | 279.9    |
| strata        | users.json   | 27.991   | 31.077      | 31.956   | 73.4     |
| orjson        | users.json   | 28.038   | 39.773      | 53.092   | 93.3     |
| msgspec       | users.json   | 38.659   | 43.812      | 55.424   | 92.1     |
| ujson         | users.json   | 46.116   | 57.491      | 65.650   | 109.7    |
| json (stdlib) | users.json   | 57.692   | 72.419      | 78.757   | 98.4     |
| strata        | users.ndjson | 25.913   | 34.117      | 35.326   | 253.5    |
| orjson        | users.ndjson | 33.881   | 37.312      | 39.514   | 261.5    |
| msgspec       | users.ndjson | 36.237   | 39.335      | 45.295   | 262.7    |
| ujson         | users.ndjson | 45.238   | 54.807      | 61.368   | 261.7    |
| json (stdlib) | users.ndjson | 64.389   | 68.744      | 75.325   | 263.7    |
| orjson        | wide_arrays  | 14.155   | 15.451      | 15.813   | 302.3    |
| strata        | wide_arrays  | 13.159   | 15.552      | 15.858   | 292.1    |
| msgspec       | wide_arrays  | 15.875   | 20.586      | 23.733   | 305.8    |
| ujson         | wide_arrays  | 28.352   | 31.219      | 33.010   | 305.8    |
| json (stdlib) | wide_arrays  | 56.660   | 58.877      | 85.428   | 243.3    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 3.101    | 3.709       | 4.137    | 273.7    |
| msgspec       | flat         | 4.436    | 5.321       | 5.579    | 273.7    |
| orjson        | flat         | 5.130    | 5.517       | 6.245    | 273.7    |
| ujson         | flat         | 7.469    | 7.845       | 9.293    | 273.7    |
| json (stdlib) | flat         | 10.320   | 11.537      | 11.591   | 273.7    |
| strata        | mixed        | 3.159    | 3.952       | 4.048    | 296.7    |
| orjson        | mixed        | 4.511    | 5.666       | 5.773    | 296.7    |
| ujson         | mixed        | 6.660    | 8.346       | 11.043   | 296.7    |
| msgspec       | mixed        | 5.368    | 10.121      | 12.550   | 296.7    |
| json (stdlib) | mixed        | 11.369   | 13.192      | 15.468   | 296.7    |
| strata        | nested       | 2.678    | 3.199       | 3.672    | 279.9    |
| orjson        | nested       | 5.278    | 6.368       | 6.916    | 279.9    |
| ujson         | nested       | 6.759    | 8.030       | 8.981    | 279.9    |
| msgspec       | nested       | 5.667    | 8.889       | 11.435   | 279.9    |
| json (stdlib) | nested       | 11.067   | 12.612      | 14.623   | 279.9    |
| strata        | users.json   | 27.412   | 31.227      | 32.402   | 99.8     |
| orjson        | users.json   | 30.123   | 41.724      | 56.085   | 99.8     |
| msgspec       | users.json   | 38.865   | 44.061      | 44.157   | 104.7    |
| ujson         | users.json   | 58.667   | 60.790      | 64.357   | 122.4    |
| json (stdlib) | users.json   | 59.964   | 73.974      | 74.346   | 104.7    |
| strata        | users.ndjson | 27.489   | 33.691      | 34.341   | 265.7    |
| orjson        | users.ndjson | 37.124   | 39.534      | 41.178   | 266.7    |
| msgspec       | users.ndjson | 38.994   | 39.993      | 46.291   | 267.7    |
| ujson         | users.ndjson | 56.397   | 60.306      | 63.897   | 266.7    |
| json (stdlib) | users.ndjson | 65.981   | 66.820      | 69.293   | 268.7    |
| orjson        | wide_arrays  | 13.106   | 18.470      | 18.583   | 243.0    |
| strata        | wide_arrays  | 14.550   | 19.469      | 20.611   | 243.3    |
| msgspec       | wide_arrays  | 18.461   | 20.708      | 21.577   | 243.0    |
| ujson         | wide_arrays  | 31.170   | 32.332      | 36.198   | 243.0    |
| json (stdlib) | wide_arrays  | 56.419   | 58.960      | 60.522   | 243.0    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.926    | 1.069       | 1.210    | 274.0    |
| orjson  | flat        | 1.001    | 1.123       | 1.464    | 276.3    |
| msgspec | flat        | 1.184    | 1.230       | 1.308    | 276.3    |
| ujson   | flat        | 4.856    | 5.050       | 5.263    | 276.3    |
| json    | flat        | 9.055    | 9.245       | 9.425    | 276.3    |
| strata  | mixed       | 1.015    | 1.045       | 1.065    | 296.7    |
| orjson  | mixed       | 1.097    | 1.130       | 1.153    | 296.7    |
| msgspec | mixed       | 1.216    | 1.243       | 1.537    | 296.7    |
| ujson   | mixed       | 5.214    | 6.414       | 6.537    | 296.8    |
| json    | mixed       | 12.238   | 12.255      | 13.346   | 296.8    |
| strata  | nested      | 0.745    | 0.763       | 0.766    | 279.9    |
| msgspec | nested      | 0.788    | 0.797       | 0.800    | 279.9    |
| orjson  | nested      | 0.774    | 0.807       | 0.989    | 279.9    |
| ujson   | nested      | 3.967    | 3.982       | 3.988    | 279.9    |
| json    | nested      | 5.224    | 5.369       | 6.248    | 279.9    |
| strata  | users.json  | 6.887    | 7.244       | 7.966    | 109.0    |
| msgspec | users.json  | 11.263   | 11.333      | 11.717   | 161.3    |
| orjson  | users.json  | 11.644   | 11.943      | 12.394   | 132.2    |
| ujson   | users.json  | 41.690   | 41.976      | 43.650   | 167.1    |
| json    | users.json  | 59.643   | 59.912      | 60.686   | 192.3    |
| strata  | wide_arrays | 5.524    | 5.638       | 5.812    | 243.0    |
| msgspec | wide_arrays | 6.895    | 7.321       | 7.539    | 253.1    |
| orjson  | wide_arrays | 7.167    | 7.359       | 7.478    | 233.4    |
| ujson   | wide_arrays | 29.070   | 29.533      | 30.624   | 255.1    |
| json    | wide_arrays | 80.933   | 81.543      | 82.207   | 276.0    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.158    | 1.227       | 1.299    | 276.3    |
| orjson        | flat        | 1.385    | 1.435       | 1.797    | 276.3    |
| msgspec       | flat        | 1.681    | 1.790       | 2.004    | 277.4    |
| ujson         | flat        | 5.198    | 6.323       | 6.490    | 277.4    |
| json (stdlib) | flat        | 34.735   | 35.390      | 36.336   | 277.4    |
| strata        | mixed       | 1.310    | 1.342       | 1.639    | 296.8    |
| orjson        | mixed       | 1.501    | 1.568       | 1.590    | 296.8    |
| msgspec       | mixed       | 1.677    | 2.414       | 2.707    | 296.8    |
| ujson         | mixed       | 5.492    | 6.004       | 6.479    | 296.8    |
| json (stdlib) | mixed       | 40.391   | 40.425      | 43.206   | 296.8    |
| msgspec       | nested      | 1.063    | 1.144       | 1.181    | 279.9    |
| orjson        | nested      | 1.109    | 1.152       | 1.604    | 279.9    |
| strata        | nested      | 1.036    | 1.262       | 1.282    | 279.9    |
| ujson         | nested      | 4.790    | 4.945       | 5.157    | 279.9    |
| json (stdlib) | nested      | 54.119   | 60.634      | 63.252   | 279.9    |
| strata        | users.json  | 8.754    | 8.826       | 8.977    | 198.6    |
| orjson        | users.json  | 11.800   | 12.191      | 12.498   | 220.9    |
| msgspec       | users.json  | 12.093   | 13.399      | 13.690   | 243.4    |
| ujson         | users.json  | 43.357   | 44.206      | 44.664   | 221.0    |
| json (stdlib) | users.json  | 439.223  | 441.490     | 447.952  | 243.4    |
| strata        | wide_arrays | 6.591    | 6.749       | 7.985    | 276.0    |
| msgspec       | wide_arrays | 8.825    | 9.041       | 9.118    | 294.4    |
| orjson        | wide_arrays | 8.851    | 9.312       | 9.855    | 273.9    |
| ujson         | wide_arrays | 30.908   | 31.287      | 31.400   | 275.9    |
| json (stdlib) | wide_arrays | 183.816  | 187.267     | 188.714  | 294.4    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 12.445   | 2000    | 240.5    |
|                                | orjson+jmespath    | 32.002   | 2000    | 243.7    |
|                                | orjson+jsonpath-ng | 34.345   | 2000    | 246.0    |
| all item prices                | strata             | 14.968   | 100999  | 246.7    |
|                                | orjson+jmespath    | 86.161   | 2000    | 256.7    |
|                                | orjson+jsonpath-ng | 289.206  | 100999  | 262.5    |
| recursive price                | strata             | 13.701   | 100999  | 260.6    |
|                                | orjson+jsonpath-ng | 949.723  | 100999  | 266.2    |
| flat:$.records\[\*\].id        | strata             | 1.946    | 2000    | 277.4    |
|                                | orjson+jmespath    | 5.382    | 2000    | 277.5    |
|                                | orjson+jsonpath-ng | 7.500    | 2000    | 277.5    |
| flat:$.records\[0\]            | strata             | 1.883    | 1       | 277.5    |
|                                | orjson+jsonpath-ng | 4.151    | 1       | 277.5    |
|                                | orjson+jmespath    | 4.499    | 1       | 277.5    |
| nested:$.records\[\*\].id      | strata             | 1.485    | 2000    | 279.9    |
|                                | orjson+jmespath    | 6.311    | 2000    | 279.9    |
|                                | orjson+jsonpath-ng | 8.008    | 2000    | 279.9    |
| nested:$.records\[0\]          | strata             | 1.495    | 1       | 279.9    |
|                                | orjson+jsonpath-ng | 5.602    | 1       | 279.9    |
|                                | orjson+jmespath    | 6.583    | 1       | 279.9    |
| wide_arrays:$.records\[\*\].id | strata             | 10.906   | 2000    | 294.4    |
|                                | orjson+jmespath    | 16.486   | 2000    | 294.4    |
|                                | orjson+jsonpath-ng | 18.578   | 2000    | 294.4    |
| wide_arrays:$.records\[0\]     | strata             | 10.806   | 1       | 294.4    |
|                                | orjson+jmespath    | 15.191   | 1       | 294.5    |
|                                | orjson+jsonpath-ng | 15.323   | 1       | 294.5    |
| mixed:$.records\[\*\].id       | strata             | 2.073    | 2000    | 296.8    |
|                                | orjson+jmespath    | 5.343    | 2000    | 296.8    |
|                                | orjson+jsonpath-ng | 7.657    | 2000    | 296.8    |
| mixed:$.records\[0\]           | strata             | 1.909    | 1       | 296.8    |
|                                | orjson+jmespath    | 5.409    | 1       | 296.8    |
|                                | orjson+jsonpath-ng | 5.681    | 1       | 296.8    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.312    | 2000    | 288.4    |
|                                            | jmespath    | 1.032    | 2000    | 288.4    |
|                                            | jsonpath-ng | 18.106   | 2000    | 289.2    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.186    | 100999  | 289.2    |
|                                            | jmespath    | 64.717   | 2000    | 294.0    |
|                                            | jsonpath-ng | 259.528  | 100999  | 295.4    |
| $..price                                   | strata      | 8.308    | 100999  | 295.3    |
|                                            | jsonpath-ng | 888.306  | 100999  | 277.8    |
| $.users\[?(@.age>30)\]                     | strata      | 0.373    | 1606    | 275.9    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.310    | 6650    | 275.9    |
| flat:$.records\[\*\].id                    | strata      | 0.125    | 2000    | 277.5    |
|                                            | jmespath    | 0.681    | 2000    | 277.5    |
|                                            | jsonpath-ng | 4.684    | 2000    | 277.5    |
| flat:$.records\[0\]                        | strata      | 0.008    | 1       | 277.5    |
|                                            | jsonpath-ng | 0.010    | 1       | 277.5    |
|                                            | jmespath    | 0.016    | 1       | 277.5    |
| nested:$.records\[\*\].id                  | strata      | 0.140    | 2000    | 279.9    |
|                                            | jmespath    | 0.721    | 2000    | 279.9    |
|                                            | jsonpath-ng | 7.034    | 2000    | 279.9    |
| nested:$.records\[0\]                      | jsonpath-ng | 0.008    | 1       | 279.9    |
|                                            | strata      | 0.011    | 1       | 279.9    |
|                                            | jmespath    | 0.015    | 1       | 279.9    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.103    | 2000    | 294.5    |
|                                            | jmespath    | 0.712    | 2000    | 294.5    |
|                                            | jsonpath-ng | 7.204    | 2000    | 294.5    |
| wide_arrays:$.records\[0\]                 | strata      | 0.007    | 1       | 294.5    |
|                                            | jmespath    | 0.013    | 1       | 294.5    |
|                                            | jsonpath-ng | 0.024    | 1       | 294.5    |
| mixed:$.records\[\*\].id                   | strata      | 0.126    | 2000    | 296.8    |
|                                            | jmespath    | 0.702    | 2000    | 296.8    |
|                                            | jsonpath-ng | 5.366    | 2000    | 296.8    |
| mixed:$.records\[0\]                       | strata      | 0.004    | 1       | 296.8    |
|                                            | jmespath    | 0.008    | 1       | 296.8    |
|                                            | jsonpath-ng | 0.009    | 1       | 296.8    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **0.2% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **30.7% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **9.9% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **35.1% faster** than #2 (orjson)  |
| dumps               | **#1** / 5              | **63.5% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **34.8% faster** than #2 (orjson)  |
| loads (flat)        | **#1** / 5              | **47.0% faster** than #2 (msgspec) |
| load (flat)         | **#1** / 5              | **43.1% faster** than #2 (msgspec) |
| dumps (flat)        | **#1** / 5              | **8.1% faster** than #2 (orjson)   |
| dump (flat)         | **#1** / 5              | **19.6% faster** than #2 (orjson)  |
| loads (nested)      | **#1** / 5              | **104.0% faster** than #2 (orjson) |
| load (nested)       | **#1** / 5              | **97.1% faster** than #2 (orjson)  |
| dumps (nested)      | **#1** / 5              | **3.9% faster** than #2 (orjson)   |
| dump (nested)       | **#1** / 5              | **2.6% faster** than #2 (msgspec)  |
| loads (wide_arrays) | **#1** / 5              | **7.6% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#2** / 5              | 11.0% behind #1 (orjson)           |
| dumps (wide_arrays) | **#1** / 5              | **24.8% faster** than #2 (msgspec) |
| dump (wide_arrays)  | **#1** / 5              | **33.9% faster** than #2 (msgspec) |
| loads (mixed)       | **#1** / 5              | **52.2% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **42.8% faster** than #2 (orjson)  |
| dumps (mixed)       | **#1** / 5              | **8.1% faster** than #2 (orjson)   |
| dump (mixed)        | **#1** / 5              | **14.6% faster** than #2 (orjson)  |
| search (JSONPath)   | **#1** in 11/11 queries | avg **799% faster**                |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
