# Strata Benchmark Results

Generated: 2026-03-17 22:34:27

## Environment

- Commit: aa7d859c5354df2117bd93cad8c93b96f7917890
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
| strata        | flat         | 2.513    | 3.036       | 3.222    | 273.0    |
| orjson        | flat         | 4.460    | 4.514       | 5.068    | 275.5    |
| msgspec       | flat         | 3.653    | 4.948       | 7.326    | 275.5    |
| ujson         | flat         | 5.985    | 6.835       | 7.008    | 275.5    |
| json (stdlib) | flat         | 10.603   | 11.623      | 11.945   | 275.5    |
| strata        | mixed        | 2.593    | 3.231       | 3.274    | 317.2    |
| orjson        | mixed        | 3.982    | 4.890       | 5.406    | 319.5    |
| msgspec       | mixed        | 5.129    | 5.144       | 5.303    | 319.5    |
| ujson         | mixed        | 7.836    | 8.302       | 11.309   | 319.5    |
| json (stdlib) | mixed        | 10.537   | 11.797      | 11.821   | 319.5    |
| strata        | nested       | 2.324    | 3.127       | 3.144    | 279.5    |
| orjson        | nested       | 4.796    | 6.103       | 6.369    | 281.9    |
| msgspec       | nested       | 6.727    | 6.905       | 8.153    | 281.9    |
| ujson         | nested       | 5.390    | 7.643       | 7.905    | 281.9    |
| json (stdlib) | nested       | 8.540    | 9.809       | 10.244   | 281.9    |
| strata        | users.json   | 23.926   | 30.092      | 32.156   | 73.6     |
| orjson        | users.json   | 39.914   | 44.351      | 44.936   | 93.5     |
| msgspec       | users.json   | 30.393   | 47.824      | 54.259   | 92.3     |
| ujson         | users.json   | 51.861   | 62.896      | 75.486   | 109.9    |
| json (stdlib) | users.json   | 71.524   | 79.187      | 87.058   | 98.5     |
| strata        | users.ndjson | 22.998   | 29.125      | 32.181   | 256.3    |
| msgspec       | users.ndjson | 37.651   | 38.823      | 44.598   | 264.5    |
| orjson        | users.ndjson | 35.863   | 43.542      | 45.017   | 263.3    |
| ujson         | users.ndjson | 54.490   | 56.089      | 56.556   | 263.5    |
| json (stdlib) | users.ndjson | 64.126   | 67.817      | 71.747   | 265.5    |
| strata        | wide_arrays  | 11.249   | 14.001      | 14.987   | 294.1    |
| orjson        | wide_arrays  | 12.399   | 17.256      | 18.447   | 304.3    |
| msgspec       | wide_arrays  | 15.447   | 18.548      | 18.583   | 304.3    |
| ujson         | wide_arrays  | 24.762   | 28.404      | 33.506   | 304.3    |
| json (stdlib) | wide_arrays  | 54.987   | 55.109      | 61.395   | 244.5    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 2.850    | 3.271       | 3.577    | 275.5    |
| orjson        | flat         | 4.023    | 4.709       | 4.761    | 275.5    |
| msgspec       | flat         | 4.209    | 4.744       | 7.158    | 277.2    |
| ujson         | flat         | 6.249    | 7.524       | 8.102    | 277.2    |
| json (stdlib) | flat         | 10.577   | 11.867      | 12.255   | 277.2    |
| strata        | mixed        | 2.992    | 3.452       | 3.551    | 319.5    |
| orjson        | mixed        | 5.700    | 6.201       | 7.651    | 319.5    |
| msgspec       | mixed        | 4.876    | 6.749       | 9.180    | 319.5    |
| ujson         | mixed        | 6.846    | 7.686       | 7.895    | 319.5    |
| json (stdlib) | mixed        | 12.494   | 12.612      | 13.423   | 319.5    |
| strata        | nested       | 2.392    | 2.837       | 2.854    | 281.9    |
| msgspec       | nested       | 5.027    | 6.524       | 7.008    | 281.9    |
| ujson         | nested       | 7.597    | 8.727       | 9.333    | 281.9    |
| orjson        | nested       | 4.603    | 9.324       | 13.337   | 281.9    |
| json (stdlib) | nested       | 8.671    | 12.941      | 14.198   | 281.9    |
| strata        | users.json   | 24.470   | 30.975      | 31.221   | 100.0    |
| msgspec       | users.json   | 32.314   | 45.994      | 51.587   | 104.9    |
| orjson        | users.json   | 29.771   | 49.976      | 51.188   | 100.0    |
| ujson         | users.json   | 50.514   | 65.622      | 67.302   | 122.6    |
| json (stdlib) | users.json   | 56.221   | 74.369      | 85.881   | 104.9    |
| strata        | users.ndjson | 23.912   | 31.050      | 33.185   | 267.5    |
| msgspec       | users.ndjson | 36.915   | 38.396      | 41.781   | 269.5    |
| orjson        | users.ndjson | 33.446   | 38.678      | 39.741   | 269.5    |
| ujson         | users.ndjson | 46.607   | 57.671      | 57.738   | 268.5    |
| json (stdlib) | users.ndjson | 61.967   | 65.968      | 80.339   | 270.5    |
| strata        | wide_arrays  | 12.104   | 14.616      | 14.937   | 244.5    |
| orjson        | wide_arrays  | 14.760   | 16.620      | 19.797   | 244.2    |
| msgspec       | wide_arrays  | 17.321   | 20.120      | 21.403   | 244.2    |
| ujson         | wide_arrays  | 27.623   | 32.252      | 34.179   | 244.2    |
| json (stdlib) | wide_arrays  | 54.018   | 61.459      | 63.313   | 244.2    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.928    | 0.928       | 0.955    | 277.2    |
| orjson  | flat        | 0.979    | 0.982       | 1.074    | 279.5    |
| msgspec | flat        | 1.140    | 1.145       | 1.153    | 279.5    |
| ujson   | flat        | 4.785    | 4.835       | 4.893    | 279.5    |
| json    | flat        | 9.079    | 9.417       | 10.135   | 279.5    |
| strata  | mixed       | 1.063    | 1.086       | 1.107    | 319.5    |
| orjson  | mixed       | 1.079    | 1.086       | 1.281    | 319.5    |
| msgspec | mixed       | 1.170    | 1.177       | 1.193    | 319.5    |
| ujson   | mixed       | 5.165    | 5.260       | 5.509    | 319.5    |
| json    | mixed       | 12.407   | 12.411      | 13.111   | 319.5    |
| orjson  | nested      | 0.758    | 0.775       | 0.787    | 281.9    |
| strata  | nested      | 0.776    | 0.778       | 0.905    | 281.9    |
| msgspec | nested      | 0.803    | 0.808       | 0.812    | 281.9    |
| ujson   | nested      | 4.080    | 4.093       | 4.227    | 281.9    |
| json    | nested      | 5.340    | 5.528       | 5.708    | 281.9    |
| strata  | users.json  | 6.868    | 6.882       | 6.897    | 109.0    |
| msgspec | users.json  | 11.162   | 11.180      | 11.720   | 161.1    |
| orjson  | users.json  | 11.416   | 11.427      | 12.280   | 132.0    |
| ujson   | users.json  | 44.483   | 45.455      | 46.642   | 167.2    |
| json    | users.json  | 59.917   | 60.711      | 62.207   | 192.5    |
| strata  | wide_arrays | 5.816    | 5.818       | 6.157    | 244.2    |
| msgspec | wide_arrays | 6.943    | 6.954       | 7.084    | 259.8    |
| orjson  | wide_arrays | 7.011    | 7.359       | 7.395    | 244.1    |
| ujson   | wide_arrays | 30.125   | 30.794      | 32.185   | 259.8    |
| json    | wide_arrays | 80.403   | 82.004      | 86.318   | 280.6    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.199    | 1.486       | 1.523    | 279.5    |
| orjson        | flat        | 1.495    | 1.682       | 2.044    | 279.5    |
| msgspec       | flat        | 1.725    | 1.837       | 1.949    | 279.5    |
| ujson         | flat        | 5.188    | 5.205       | 5.266    | 279.5    |
| json (stdlib) | flat        | 34.814   | 34.902      | 35.963   | 279.5    |
| msgspec       | mixed       | 1.623    | 1.706       | 1.976    | 319.5    |
| orjson        | mixed       | 1.515    | 1.739       | 1.777    | 319.5    |
| strata        | mixed       | 1.625    | 1.763       | 1.806    | 319.5    |
| ujson         | mixed       | 5.668    | 6.286       | 6.585    | 319.5    |
| json (stdlib) | mixed       | 44.550   | 47.912      | 53.982   | 319.5    |
| strata        | nested      | 1.047    | 1.083       | 1.181    | 281.9    |
| msgspec       | nested      | 1.051    | 1.089       | 1.115    | 281.9    |
| orjson        | nested      | 1.063    | 1.182       | 1.288    | 281.9    |
| ujson         | nested      | 4.192    | 4.210       | 4.214    | 281.9    |
| json (stdlib) | nested      | 48.065   | 49.522      | 49.533   | 281.9    |
| strata        | users.json  | 8.140    | 8.219       | 8.934    | 198.7    |
| orjson        | users.json  | 12.295   | 12.337      | 14.516   | 221.0    |
| msgspec       | users.json  | 11.757   | 13.162      | 13.205   | 243.5    |
| ujson         | users.json  | 42.936   | 43.143      | 49.906   | 221.2    |
| json (stdlib) | users.json  | 457.224  | 457.907     | 611.474  | 243.5    |
| strata        | wide_arrays | 6.803    | 6.920       | 7.262    | 280.6    |
| orjson        | wide_arrays | 7.510    | 7.645       | 8.218    | 295.5    |
| msgspec       | wide_arrays | 8.856    | 8.951       | 9.379    | 317.2    |
| ujson         | wide_arrays | 30.551   | 30.667      | 30.907   | 297.5    |
| json (stdlib) | wide_arrays | 185.617  | 187.939     | 192.049  | 317.2    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 12.666   | 2000    | 240.6    |
|                                | orjson+jmespath    | 31.634   | 2000    | 243.7    |
|                                | orjson+jsonpath-ng | 33.839   | 2000    | 245.9    |
| all item prices                | strata             | 14.239   | 100999  | 246.3    |
|                                | orjson+jmespath    | 89.553   | 2000    | 255.5    |
|                                | orjson+jsonpath-ng | 293.152  | 100999  | 261.9    |
| recursive price                | strata             | 15.132   | 100999  | 260.1    |
|                                | orjson+jsonpath-ng | 909.535  | 100999  | 265.9    |
| flat:$.records\[\*\].id        | strata             | 1.899    | 2000    | 279.5    |
|                                | orjson+jmespath    | 4.794    | 2000    | 279.5    |
|                                | orjson+jsonpath-ng | 6.938    | 2000    | 279.5    |
| flat:$.records\[0\]            | strata             | 1.762    | 1       | 279.5    |
|                                | orjson+jmespath    | 4.053    | 1       | 279.5    |
|                                | orjson+jsonpath-ng | 4.101    | 1       | 279.5    |
| nested:$.records\[\*\].id      | strata             | 1.597    | 2000    | 281.9    |
|                                | orjson+jmespath    | 6.141    | 2000    | 281.9    |
|                                | orjson+jsonpath-ng | 8.367    | 2000    | 281.9    |
| nested:$.records\[0\]          | strata             | 1.604    | 1       | 281.9    |
|                                | orjson+jsonpath-ng | 4.998    | 1       | 281.9    |
|                                | orjson+jmespath    | 5.366    | 1       | 281.9    |
| wide_arrays:$.records\[\*\].id | strata             | 10.153   | 2000    | 317.2    |
|                                | orjson+jmespath    | 15.964   | 2000    | 317.2    |
|                                | orjson+jsonpath-ng | 19.209   | 2000    | 317.2    |
| wide_arrays:$.records\[0\]     | strata             | 10.082   | 1       | 317.2    |
|                                | orjson+jsonpath-ng | 15.425   | 1       | 317.2    |
|                                | orjson+jmespath    | 15.576   | 1       | 317.2    |
| mixed:$.records\[\*\].id       | strata             | 1.985    | 2000    | 319.5    |
|                                | orjson+jmespath    | 5.603    | 2000    | 319.5    |
|                                | orjson+jsonpath-ng | 9.570    | 2000    | 319.5    |
| mixed:$.records\[0\]           | strata             | 1.996    | 1       | 319.5    |
|                                | orjson+jmespath    | 4.731    | 1       | 319.5    |
|                                | orjson+jsonpath-ng | 5.882    | 1       | 319.5    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.307    | 2000    | 288.1    |
|                                            | jmespath    | 1.069    | 2000    | 288.1    |
|                                            | jsonpath-ng | 17.382   | 2000    | 289.0    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 7.242    | 100999  | 289.0    |
|                                            | jmespath    | 63.926   | 2000    | 293.8    |
|                                            | jsonpath-ng | 265.106  | 100999  | 295.1    |
| $..price                                   | strata      | 8.397    | 100999  | 295.6    |
|                                            | jsonpath-ng | 929.713  | 100999  | 279.5    |
| $.users\[?(@.age>30)\]                     | strata      | 0.371    | 1606    | 276.6    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.711    | 6650    | 276.6    |
| flat:$.records\[\*\].id                    | strata      | 0.130    | 2000    | 279.5    |
|                                            | jmespath    | 0.672    | 2000    | 279.5    |
|                                            | jsonpath-ng | 4.213    | 2000    | 279.5    |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 279.5    |
|                                            | jmespath    | 0.007    | 1       | 279.5    |
|                                            | jsonpath-ng | 0.007    | 1       | 279.5    |
| nested:$.records\[\*\].id                  | strata      | 0.118    | 2000    | 281.9    |
|                                            | jmespath    | 0.696    | 2000    | 281.9    |
|                                            | jsonpath-ng | 5.571    | 2000    | 281.9    |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 281.9    |
|                                            | jmespath    | 0.007    | 1       | 281.9    |
|                                            | jsonpath-ng | 0.011    | 1       | 281.9    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.120    | 2000    | 317.2    |
|                                            | jmespath    | 0.830    | 2000    | 317.2    |
|                                            | jsonpath-ng | 7.649    | 2000    | 317.2    |
| wide_arrays:$.records\[0\]                 | strata      | 0.008    | 1       | 317.2    |
|                                            | jmespath    | 0.009    | 1       | 317.2    |
|                                            | jsonpath-ng | 0.023    | 1       | 317.2    |
| mixed:$.records\[\*\].id                   | strata      | 0.117    | 2000    | 319.5    |
|                                            | jmespath    | 0.714    | 2000    | 319.5    |
|                                            | jsonpath-ng | 5.356    | 2000    | 319.5    |
| mixed:$.records\[0\]                       | jsonpath-ng | 0.015    | 1       | 319.5    |
|                                            | strata      | 0.015    | 1       | 319.5    |
|                                            | jmespath    | 0.015    | 1       | 319.5    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **27.0% faster** than #2 (msgspec) |
| loads (NDJSON)      | **#1** / 5              | **55.9% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **21.7% faster** than #2 (orjson)  |
| load (NDJSON file)  | **#1** / 5              | **39.9% faster** than #2 (orjson)  |
| dumps               | **#1** / 5              | **62.5% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **44.4% faster** than #2 (msgspec) |
| loads (flat)        | **#1** / 5              | **45.4% faster** than #2 (msgspec) |
| load (flat)         | **#1** / 5              | **41.1% faster** than #2 (orjson)  |
| dumps (flat)        | **#1** / 5              | **5.5% faster** than #2 (orjson)   |
| dump (flat)         | **#1** / 5              | **24.6% faster** than #2 (orjson)  |
| loads (nested)      | **#1** / 5              | **106.3% faster** than #2 (orjson) |
| load (nested)       | **#1** / 5              | **92.5% faster** than #2 (orjson)  |
| dumps (nested)      | **#2** / 5              | 2.4% behind #1 (orjson)            |
| dump (nested)       | **#1** / 5              | **0.4% faster** than #2 (msgspec)  |
| loads (wide_arrays) | **#1** / 5              | **10.2% faster** than #2 (orjson)  |
| load (wide_arrays)  | **#1** / 5              | **21.9% faster** than #2 (orjson)  |
| dumps (wide_arrays) | **#1** / 5              | **19.4% faster** than #2 (msgspec) |
| dump (wide_arrays)  | **#1** / 5              | **10.4% faster** than #2 (orjson)  |
| loads (mixed)       | **#1** / 5              | **53.6% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **62.9% faster** than #2 (msgspec) |
| dumps (mixed)       | **#1** / 5              | **1.5% faster** than #2 (orjson)   |
| dump (mixed)        | **#3** / 5              | 7.3% behind #1 (orjson)            |
| search (JSONPath)   | **#1** in 11/11 queries | avg **709% faster**                |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
