# Strata Benchmark Results

Generated: 2026-03-18 13:21:42

## Environment

- Commit: 61b2175f7dd9503191430e1dff7b62dbe525ea97
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
| strata        | flat         | 2.878    | 3.220       | 3.329    | 270.2    |
| orjson        | flat         | 3.536    | 4.354       | 4.414    | 272.7    |
| msgspec       | flat         | 3.626    | 4.515       | 4.666    | 272.7    |
| ujson         | flat         | 5.868    | 6.524       | 6.931    | 272.7    |
| json (stdlib) | flat         | 10.152   | 10.164      | 10.180   | 272.7    |
| strata        | mixed        | 2.826    | 3.455       | 3.538    | 295.9    |
| orjson        | mixed        | 4.296    | 4.863       | 5.261    | 295.9    |
| msgspec       | mixed        | 5.452    | 5.801       | 6.252    | 295.9    |
| ujson         | mixed        | 6.980    | 7.394       | 8.765    | 295.9    |
| json (stdlib) | mixed        | 10.598   | 11.435      | 11.793   | 295.9    |
| strata        | nested       | 2.392    | 2.954       | 3.018    | 278.4    |
| msgspec       | nested       | 5.838    | 6.526       | 8.529    | 278.5    |
| orjson        | nested       | 4.745    | 7.230       | 7.955    | 278.4    |
| ujson         | nested       | 6.686    | 7.755       | 8.942    | 278.5    |
| json (stdlib) | nested       | 9.577    | 9.771       | 10.251   | 278.5    |
| strata        | users.json   | 25.016   | 31.697      | 32.093   | 73.5     |
| orjson        | users.json   | 31.123   | 41.150      | 44.289   | 93.4     |
| msgspec       | users.json   | 29.291   | 43.245      | 43.265   | 92.2     |
| ujson         | users.json   | 42.514   | 57.368      | 58.915   | 109.8    |
| json (stdlib) | users.json   | 60.368   | 68.043      | 70.655   | 98.4     |
| strata        | users.ndjson | 25.738   | 31.688      | 32.651   | 252.5    |
| orjson        | users.ndjson | 33.206   | 34.810      | 36.122   | 260.5    |
| msgspec       | users.ndjson | 36.454   | 36.709      | 38.472   | 261.6    |
| ujson         | users.ndjson | 44.019   | 49.655      | 52.907   | 260.6    |
| json (stdlib) | users.ndjson | 63.613   | 65.583      | 66.198   | 262.6    |
| orjson        | wide_arrays  | 11.789   | 14.799      | 14.999   | 301.0    |
| strata        | wide_arrays  | 12.785   | 15.465      | 15.614   | 290.8    |
| msgspec       | wide_arrays  | 15.194   | 18.594      | 18.815   | 301.1    |
| ujson         | wide_arrays  | 24.289   | 27.760      | 27.846   | 301.1    |
| json (stdlib) | wide_arrays  | 51.200   | 53.810      | 54.656   | 301.1    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 3.030    | 3.369       | 3.455    | 272.7    |
| orjson        | flat         | 4.137    | 4.620       | 4.749    | 273.1    |
| msgspec       | flat         | 4.208    | 4.678       | 5.152    | 273.1    |
| ujson         | flat         | 6.138    | 7.109       | 7.150    | 273.1    |
| json (stdlib) | flat         | 10.349   | 10.466      | 11.001   | 274.3    |
| strata        | mixed        | 3.185    | 3.790       | 4.458    | 295.9    |
| orjson        | mixed        | 5.141    | 5.375       | 5.569    | 295.9    |
| msgspec       | mixed        | 5.194    | 5.759       | 7.008    | 295.9    |
| ujson         | mixed        | 7.315    | 7.778       | 8.610    | 295.9    |
| json (stdlib) | mixed        | 11.272   | 11.398      | 12.633   | 295.9    |
| strata        | nested       | 2.665    | 3.233       | 3.412    | 278.5    |
| msgspec       | nested       | 6.031    | 6.263       | 8.633    | 278.5    |
| orjson        | nested       | 6.445    | 6.739       | 6.938    | 278.5    |
| ujson         | nested       | 6.692    | 8.451       | 9.038    | 278.5    |
| json (stdlib) | nested       | 8.922    | 10.114      | 12.044   | 278.5    |
| strata        | users.json   | 24.906   | 31.329      | 31.880   | 99.8     |
| orjson        | users.json   | 26.439   | 38.106      | 39.982   | 99.8     |
| msgspec       | users.json   | 29.243   | 41.482      | 43.870   | 104.8    |
| ujson         | users.json   | 39.801   | 56.266      | 57.660   | 122.5    |
| json (stdlib) | users.json   | 58.392   | 69.532      | 75.658   | 104.8    |
| strata        | users.ndjson | 26.148   | 32.464      | 32.539   | 264.6    |
| msgspec       | users.ndjson | 35.774   | 36.891      | 38.436   | 266.7    |
| orjson        | users.ndjson | 39.037   | 45.558      | 46.618   | 265.7    |
| ujson         | users.ndjson | 52.037   | 55.276      | 64.662   | 265.7    |
| json (stdlib) | users.ndjson | 62.011   | 64.602      | 65.440   | 267.7    |
| strata        | wide_arrays  | 13.870   | 16.380      | 16.632   | 301.1    |
| orjson        | wide_arrays  | 13.554   | 16.599      | 16.916   | 300.8    |
| msgspec       | wide_arrays  | 16.251   | 19.567      | 22.253   | 300.8    |
| ujson         | wide_arrays  | 26.818   | 29.603      | 30.329   | 300.8    |
| json (stdlib) | wide_arrays  | 57.794   | 61.169      | 67.271   | 300.8    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.905    | 0.909       | 0.921    | 274.3    |
| orjson  | flat        | 0.984    | 0.990       | 1.093    | 276.6    |
| msgspec | flat        | 1.169    | 1.199       | 1.255    | 277.6    |
| ujson   | flat        | 4.838    | 5.147       | 5.185    | 277.6    |
| json    | flat        | 9.208    | 9.806       | 9.902    | 278.0    |
| strata  | mixed       | 1.018    | 1.030       | 1.124    | 295.9    |
| orjson  | mixed       | 1.047    | 1.070       | 1.168    | 295.9    |
| msgspec | mixed       | 1.166    | 1.199       | 1.298    | 295.9    |
| ujson   | mixed       | 5.231    | 5.480       | 5.516    | 296.5    |
| json    | mixed       | 11.371   | 11.449      | 11.742   | 296.5    |
| strata  | nested      | 0.741    | 0.779       | 1.161    | 278.5    |
| orjson  | nested      | 0.789    | 0.790       | 1.100    | 278.5    |
| msgspec | nested      | 0.787    | 0.799       | 1.707    | 278.5    |
| ujson   | nested      | 4.016    | 4.173       | 4.230    | 278.5    |
| json    | nested      | 5.748    | 5.783       | 7.529    | 278.5    |
| strata  | users.json  | 6.701    | 6.730       | 6.808    | 109.0    |
| orjson  | users.json  | 10.779   | 10.876      | 11.007   | 131.3    |
| msgspec | users.json  | 11.093   | 11.385      | 12.408   | 159.6    |
| ujson   | users.json  | 42.642   | 43.119      | 43.918   | 167.1    |
| json    | users.json  | 59.451   | 59.480      | 60.234   | 192.4    |
| msgspec | wide_arrays | 6.996    | 7.054       | 8.646    | 316.2    |
| strata  | wide_arrays | 6.048    | 9.059       | 9.227    | 305.7    |
| orjson  | wide_arrays | 8.996    | 9.643       | 10.092   | 296.4    |
| ujson   | wide_arrays | 29.186   | 29.266      | 30.725   | 318.1    |
| json    | wide_arrays | 80.717   | 80.846      | 81.487   | 339.0    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.162    | 1.218       | 1.237    | 278.0    |
| orjson        | flat        | 1.425    | 1.470       | 1.813    | 278.0    |
| msgspec       | flat        | 1.449    | 1.533       | 1.654    | 278.4    |
| ujson         | flat        | 5.357    | 5.490       | 6.429    | 278.4    |
| json (stdlib) | flat        | 32.976   | 33.131      | 33.417   | 278.4    |
| strata        | mixed       | 1.295    | 1.385       | 1.559    | 296.5    |
| orjson        | mixed       | 1.443    | 1.445       | 1.473    | 296.5    |
| msgspec       | mixed       | 1.477    | 1.524       | 1.599    | 296.5    |
| ujson         | mixed       | 5.531    | 5.612       | 5.820    | 296.5    |
| json (stdlib) | mixed       | 38.701   | 38.879      | 39.884   | 296.5    |
| strata        | nested      | 1.026    | 1.029       | 1.394    | 278.5    |
| msgspec       | nested      | 1.067    | 1.108       | 1.123    | 278.6    |
| orjson        | nested      | 1.134    | 1.275       | 1.559    | 278.6    |
| ujson         | nested      | 4.182    | 4.207       | 4.350    | 278.6    |
| json (stdlib) | nested      | 47.287   | 47.287      | 47.860   | 278.6    |
| strata        | users.json  | 7.633    | 7.699       | 7.757    | 198.6    |
| orjson        | users.json  | 11.638   | 11.804      | 11.815   | 220.9    |
| msgspec       | users.json  | 11.730   | 12.264      | 12.607   | 243.4    |
| ujson         | users.json  | 42.652   | 43.302      | 44.103   | 221.1    |
| json (stdlib) | users.json  | 431.356  | 434.023     | 441.724  | 243.4    |
| strata        | wide_arrays | 6.187    | 6.328       | 6.485    | 339.0    |
| msgspec       | wide_arrays | 7.843    | 7.967       | 7.980    | 358.5    |
| orjson        | wide_arrays | 7.957    | 8.463       | 8.775    | 336.8    |
| ujson         | wide_arrays | 29.784   | 29.819      | 29.844   | 338.7    |
| json (stdlib) | wide_arrays | 178.065  | 178.449     | 180.417  | 358.5    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 12.556   | 2000    | 240.5    |
|                                | orjson+jmespath    | 32.372   | 2000    | 243.6    |
|                                | orjson+jsonpath-ng | 34.148   | 2000    | 245.9    |
| all item prices                | strata             | 13.504   | 100999  | 247.2    |
|                                | orjson+jmespath    | 85.357   | 2000    | 256.4    |
|                                | orjson+jsonpath-ng | 259.198  | 100999  | 262.9    |
| recursive price                | strata             | 13.655   | 100999  | 261.5    |
|                                | orjson+jsonpath-ng | 865.188  | 100999  | 265.3    |
| flat:$.records\[\*\].id        | strata             | 1.848    | 2000    | 278.4    |
|                                | orjson+jmespath    | 4.999    | 2000    | 278.4    |
|                                | orjson+jsonpath-ng | 7.384    | 2000    | 278.4    |
| flat:$.records\[0\]            | strata             | 1.880    | 1       | 278.4    |
|                                | orjson+jsonpath-ng | 4.094    | 1       | 278.4    |
|                                | orjson+jmespath    | 4.380    | 1       | 278.4    |
| nested:$.records\[\*\].id      | strata             | 1.437    | 2000    | 278.6    |
|                                | orjson+jmespath    | 5.720    | 2000    | 278.6    |
|                                | orjson+jsonpath-ng | 7.854    | 2000    | 278.6    |
| nested:$.records\[0\]          | strata             | 1.424    | 1       | 278.6    |
|                                | orjson+jmespath    | 5.026    | 1       | 278.6    |
|                                | orjson+jsonpath-ng | 5.767    | 1       | 278.6    |
| wide_arrays:$.records\[\*\].id | strata             | 10.862   | 2000    | 358.5    |
|                                | orjson+jmespath    | 15.559   | 2000    | 358.5    |
|                                | orjson+jsonpath-ng | 19.143   | 2000    | 358.5    |
| wide_arrays:$.records\[0\]     | strata             | 10.675   | 1       | 358.5    |
|                                | orjson+jsonpath-ng | 15.307   | 1       | 295.9    |
|                                | orjson+jmespath    | 15.869   | 1       | 358.5    |
| mixed:$.records\[\*\].id       | strata             | 1.963    | 2000    | 296.5    |
|                                | orjson+jmespath    | 5.111    | 2000    | 296.5    |
|                                | orjson+jsonpath-ng | 7.513    | 2000    | 296.5    |
| mixed:$.records\[0\]           | strata             | 2.024    | 1       | 296.5    |
|                                | orjson+jsonpath-ng | 4.551    | 1       | 296.5    |
|                                | orjson+jmespath    | 4.872    | 1       | 296.5    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.288    | 2000    | 287.6    |
|                                            | jmespath    | 0.908    | 2000    | 287.6    |
|                                            | jsonpath-ng | 15.303   | 2000    | 288.4    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.175    | 100999  | 288.4    |
|                                            | jmespath    | 60.428   | 2000    | 293.2    |
|                                            | jsonpath-ng | 237.064  | 100999  | 294.6    |
| $..price                                   | strata      | 8.345    | 100999  | 294.2    |
|                                            | jsonpath-ng | 860.879  | 100999  | 296.6    |
| $.users\[?(@.age>30)\]                     | strata      | 0.334    | 1606    | 274.8    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 8.672    | 6650    | 274.8    |
| flat:$.records\[\*\].id                    | strata      | 0.126    | 2000    | 278.4    |
|                                            | jmespath    | 0.676    | 2000    | 278.4    |
|                                            | jsonpath-ng | 4.174    | 2000    | 278.4    |
| flat:$.records\[0\]                        | jmespath    | 0.007    | 1       | 278.4    |
|                                            | jsonpath-ng | 0.008    | 1       | 278.4    |
|                                            | strata      | 0.008    | 1       | 278.4    |
| nested:$.records\[\*\].id                  | strata      | 0.115    | 2000    | 278.6    |
|                                            | jmespath    | 0.728    | 2000    | 278.6    |
|                                            | jsonpath-ng | 5.911    | 2000    | 278.6    |
| nested:$.records\[0\]                      | strata      | 0.004    | 1       | 278.6    |
|                                            | jsonpath-ng | 0.008    | 1       | 278.6    |
|                                            | jmespath    | 0.008    | 1       | 278.6    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.102    | 2000    | 295.9    |
|                                            | jmespath    | 0.704    | 2000    | 295.9    |
|                                            | jsonpath-ng | 6.933    | 2000    | 295.9    |
| wide_arrays:$.records\[0\]                 | strata      | 0.011    | 1       | 295.9    |
|                                            | jmespath    | 0.017    | 1       | 295.9    |
|                                            | jsonpath-ng | 0.023    | 1       | 295.9    |
| mixed:$.records\[\*\].id                   | strata      | 0.110    | 2000    | 296.5    |
|                                            | jmespath    | 0.676    | 2000    | 296.5    |
|                                            | jsonpath-ng | 4.819    | 2000    | 296.5    |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 296.5    |
|                                            | jmespath    | 0.006    | 1       | 296.5    |
|                                            | jsonpath-ng | 0.020    | 1       | 296.5    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **17.1% faster** than #2 (msgspec)  |
| loads (NDJSON)      | **#1** / 5              | **29.0% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **6.2% faster** than #2 (orjson)    |
| load (NDJSON file)  | **#1** / 5              | **36.8% faster** than #2 (msgspec)  |
| dumps               | **#1** / 5              | **60.9% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **52.5% faster** than #2 (orjson)   |
| loads (flat)        | **#1** / 5              | **22.9% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **36.5% faster** than #2 (orjson)   |
| dumps (flat)        | **#1** / 5              | **8.8% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **22.6% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **98.3% faster** than #2 (orjson)   |
| load (nested)       | **#1** / 5              | **126.3% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **6.2% faster** than #2 (msgspec)   |
| dump (nested)       | **#1** / 5              | **4.0% faster** than #2 (msgspec)   |
| loads (wide_arrays) | **#2** / 5              | 8.4% behind #1 (orjson)             |
| load (wide_arrays)  | **#2** / 5              | 2.3% behind #1 (orjson)             |
| dumps (wide_arrays) | **#1** / 5              | **15.7% faster** than #2 (msgspec)  |
| dump (wide_arrays)  | **#1** / 5              | **26.8% faster** than #2 (msgspec)  |
| loads (mixed)       | **#1** / 5              | **52.0% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **61.4% faster** than #2 (orjson)   |
| dumps (mixed)       | **#1** / 5              | **2.9% faster** than #2 (orjson)    |
| dump (mixed)        | **#1** / 5              | **11.4% faster** than #2 (orjson)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **740% faster**                 |
| query (JSONPath)    | **#1** in 12/13 queries | -                                   |
