# Strata Benchmark Results

Generated: 2026-03-19 02:11:59

## Environment

- Commit: dfd6aa590c1dd14d42395de2cde1a4a3e49aa958
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
| strata        | flat         | 3.207    | 3.476       | 3.654    | 273.0    |
| orjson        | flat         | 3.968    | 4.387       | 4.838    | 275.5    |
| msgspec       | flat         | 5.465    | 6.005       | 7.097    | 275.5    |
| ujson         | flat         | 7.405    | 8.217       | 8.517    | 275.5    |
| json (stdlib) | flat         | 11.100   | 11.844      | 11.918   | 275.5    |
| strata        | mixed        | 3.134    | 3.914       | 3.994    | 286.2    |
| orjson        | mixed        | 4.852    | 5.320       | 5.550    | 288.5    |
| msgspec       | mixed        | 4.593    | 6.235       | 7.445    | 288.5    |
| ujson         | mixed        | 6.691    | 7.463       | 8.008    | 288.5    |
| json (stdlib) | mixed        | 12.445   | 13.453      | 19.399   | 288.5    |
| strata        | nested       | 2.738    | 3.315       | 3.339    | 279.3    |
| msgspec       | nested       | 5.721    | 6.821       | 7.881    | 281.8    |
| orjson        | nested       | 5.247    | 7.925       | 9.329    | 281.7    |
| ujson         | nested       | 8.489    | 8.663       | 11.966   | 281.8    |
| json (stdlib) | nested       | 10.429   | 10.652      | 11.673   | 281.8    |
| strata        | users.json   | 26.768   | 34.611      | 36.420   | 73.6     |
| orjson        | users.json   | 32.933   | 42.509      | 45.312   | 93.5     |
| msgspec       | users.json   | 32.168   | 43.880      | 46.857   | 92.3     |
| ujson         | users.json   | 42.706   | 66.201      | 84.750   | 109.9    |
| json (stdlib) | users.json   | 67.844   | 74.910      | 80.080   | 98.5     |
| strata        | users.ndjson | 27.650   | 34.504      | 35.564   | 255.2    |
| orjson        | users.ndjson | 37.881   | 38.411      | 43.208   | 263.3    |
| msgspec       | users.ndjson | 39.489   | 41.608      | 43.521   | 264.5    |
| ujson         | users.ndjson | 49.367   | 56.553      | 66.135   | 263.5    |
| json (stdlib) | users.ndjson | 72.710   | 73.021      | 74.818   | 265.5    |
| strata        | wide_arrays  | 13.160   | 15.488      | 16.080   | 293.9    |
| orjson        | wide_arrays  | 13.324   | 15.595      | 16.045   | 304.2    |
| msgspec       | wide_arrays  | 18.985   | 20.875      | 22.717   | 304.2    |
| ujson         | wide_arrays  | 37.261   | 41.546      | 42.545   | 304.2    |
| json (stdlib) | wide_arrays  | 53.802   | 64.531      | 73.373   | 304.2    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 3.506    | 4.025       | 4.072    | 275.5    |
| msgspec       | flat         | 4.558    | 4.941       | 5.589    | 275.9    |
| orjson        | flat         | 5.099    | 5.356       | 5.565    | 275.5    |
| ujson         | flat         | 7.424    | 8.093       | 8.141    | 275.9    |
| json (stdlib) | flat         | 10.797   | 11.053      | 11.621   | 275.9    |
| strata        | mixed        | 3.476    | 3.892       | 4.115    | 288.5    |
| msgspec       | mixed        | 5.380    | 6.679       | 7.487    | 288.6    |
| orjson        | mixed        | 5.868    | 6.916       | 7.084    | 288.5    |
| ujson         | mixed        | 7.468    | 8.335       | 9.121    | 288.6    |
| json (stdlib) | mixed        | 12.163   | 12.971      | 13.138   | 288.6    |
| strata        | nested       | 3.168    | 3.698       | 3.746    | 281.8    |
| orjson        | nested       | 4.958    | 6.926       | 8.328    | 281.8    |
| msgspec       | nested       | 5.853    | 7.461       | 7.736    | 281.8    |
| ujson         | nested       | 6.500    | 8.433       | 8.788    | 281.8    |
| json (stdlib) | nested       | 10.880   | 13.404      | 16.563   | 281.8    |
| strata        | users.json   | 27.379   | 34.723      | 35.463   | 99.9     |
| msgspec       | users.json   | 33.282   | 44.747      | 50.268   | 104.8    |
| orjson        | users.json   | 32.934   | 56.932      | 61.905   | 99.9     |
| ujson         | users.json   | 46.111   | 71.680      | 86.474   | 122.6    |
| json (stdlib) | users.json   | 65.418   | 74.083      | 79.682   | 104.8    |
| strata        | users.ndjson | 29.007   | 38.163      | 39.922   | 267.5    |
| orjson        | users.ndjson | 37.482   | 38.508      | 43.454   | 268.5    |
| msgspec       | users.ndjson | 38.583   | 40.280      | 42.035   | 269.5    |
| ujson         | users.ndjson | 50.779   | 59.272      | 69.905   | 268.5    |
| json (stdlib) | users.ndjson | 71.265   | 74.477      | 78.130   | 270.5    |
| strata        | wide_arrays  | 14.019   | 16.507      | 17.296   | 304.2    |
| orjson        | wide_arrays  | 13.874   | 17.102      | 17.225   | 235.0    |
| msgspec       | wide_arrays  | 18.679   | 23.141      | 24.722   | 235.0    |
| ujson         | wide_arrays  | 27.405   | 35.378      | 40.371   | 235.0    |
| json (stdlib) | wide_arrays  | 58.490   | 61.918      | 61.941   | 235.0    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.982    | 0.987       | 1.009    | 275.9    |
| orjson  | flat        | 1.021    | 1.024       | 1.146    | 278.2    |
| msgspec | flat        | 1.215    | 1.233       | 1.253    | 278.2    |
| ujson   | flat        | 5.049    | 5.148       | 5.229    | 279.3    |
| json    | flat        | 10.022   | 10.240      | 10.361   | 279.3    |
| orjson  | mixed       | 1.113    | 1.130       | 1.138    | 288.6    |
| strata  | mixed       | 1.156    | 1.160       | 1.163    | 288.6    |
| msgspec | mixed       | 1.318    | 1.374       | 1.557    | 288.6    |
| ujson   | mixed       | 5.759    | 5.885       | 5.904    | 288.6    |
| json    | mixed       | 12.891   | 12.942      | 13.081   | 288.6    |
| orjson  | nested      | 0.810    | 0.826       | 0.891    | 281.8    |
| strata  | nested      | 0.928    | 0.935       | 0.941    | 281.8    |
| msgspec | nested      | 0.924    | 0.943       | 0.980    | 281.8    |
| ujson   | nested      | 4.522    | 4.535       | 4.550    | 281.8    |
| json    | nested      | 5.426    | 5.902       | 6.006    | 281.8    |
| strata  | users.json  | 10.272   | 10.442      | 11.063   | 109.1    |
| msgspec | users.json  | 12.322   | 12.364      | 12.847   | 161.6    |
| orjson  | users.json  | 12.611   | 12.735      | 13.294   | 132.5    |
| ujson   | users.json  | 44.116   | 44.964      | 46.056   | 167.4    |
| json    | users.json  | 62.155   | 62.928      | 67.994   | 192.8    |
| strata  | wide_arrays | 6.441    | 6.961       | 7.020    | 235.0    |
| msgspec | wide_arrays | 7.913    | 8.009       | 8.027    | 245.1    |
| orjson  | wide_arrays | 7.989    | 8.177       | 8.469    | 225.4    |
| ujson   | wide_arrays | 30.268   | 31.129      | 31.215   | 247.1    |
| json    | wide_arrays | 83.645   | 86.982      | 91.894   | 268.0    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.381    | 1.450       | 1.466    | 279.3    |
| orjson        | flat        | 1.503    | 1.504       | 1.524    | 279.3    |
| msgspec       | flat        | 1.809    | 1.904       | 2.058    | 279.3    |
| ujson         | flat        | 5.978    | 6.018       | 6.054    | 279.3    |
| json (stdlib) | flat        | 35.827   | 37.031      | 37.154   | 279.3    |
| orjson        | mixed       | 1.540    | 1.557       | 1.624    | 288.6    |
| strata        | mixed       | 1.622    | 1.681       | 1.916    | 288.6    |
| msgspec       | mixed       | 1.609    | 1.767       | 1.845    | 288.6    |
| ujson         | mixed       | 6.128    | 6.136       | 6.892    | 288.6    |
| json (stdlib) | mixed       | 41.685   | 42.447      | 42.979   | 288.6    |
| msgspec       | nested      | 1.068    | 1.095       | 1.149    | 281.8    |
| strata        | nested      | 1.076    | 1.114       | 1.207    | 281.8    |
| orjson        | nested      | 1.226    | 1.252       | 1.306    | 281.8    |
| ujson         | nested      | 4.353    | 4.415       | 4.555    | 281.8    |
| json (stdlib) | nested      | 52.286   | 53.501      | 54.118   | 281.8    |
| strata        | users.json  | 10.000   | 10.352      | 10.718   | 199.0    |
| orjson        | users.json  | 12.214   | 12.829      | 14.465   | 221.3    |
| msgspec       | users.json  | 12.902   | 13.183      | 13.391   | 244.0    |
| ujson         | users.json  | 43.410   | 46.211      | 49.252   | 221.7    |
| json (stdlib) | users.json  | 443.275  | 451.136     | 453.877  | 244.1    |
| strata        | wide_arrays | 7.722    | 7.735       | 7.841    | 268.0    |
| msgspec       | wide_arrays | 8.301    | 8.458       | 9.178    | 286.2    |
| orjson        | wide_arrays | 9.362    | 9.993       | 10.697   | 265.7    |
| ujson         | wide_arrays | 32.642   | 33.968      | 34.614   | 267.7    |
| json (stdlib) | wide_arrays | 187.370  | 190.219     | 192.354  | 286.2    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 13.105   | 2000    | 241.1    |
|                                | orjson+jsonpath-ng | 35.457   | 2000    | 246.4    |
|                                | orjson+jmespath    | 35.819   | 2000    | 244.2    |
| all item prices                | strata             | 16.410   | 100999  | 248.4    |
|                                | orjson+jmespath    | 89.248   | 2000    | 257.6    |
|                                | orjson+jsonpath-ng | 282.522  | 100999  | 263.5    |
| recursive price                | strata             | 14.264   | 100999  | 261.1    |
|                                | orjson+jsonpath-ng | 875.808  | 100999  | 265.5    |
| flat:$.records\[\*\].id        | strata             | 1.891    | 2000    | 279.3    |
|                                | orjson+jmespath    | 6.026    | 2000    | 279.3    |
|                                | orjson+jsonpath-ng | 8.873    | 2000    | 279.3    |
| flat:$.records\[0\]            | strata             | 2.137    | 1       | 279.3    |
|                                | orjson+jmespath    | 4.783    | 1       | 279.3    |
|                                | orjson+jsonpath-ng | 5.176    | 1       | 279.3    |
| nested:$.records\[\*\].id      | strata             | 1.588    | 2000    | 281.8    |
|                                | orjson+jmespath    | 5.876    | 2000    | 281.8    |
|                                | orjson+jsonpath-ng | 8.700    | 2000    | 281.8    |
| nested:$.records\[0\]          | strata             | 1.502    | 1       | 281.8    |
|                                | orjson+jsonpath-ng | 6.027    | 1       | 281.8    |
|                                | orjson+jmespath    | 7.440    | 1       | 281.8    |
| wide_arrays:$.records\[\*\].id | strata             | 9.798    | 2000    | 286.2    |
|                                | orjson+jmespath    | 18.199   | 2000    | 286.2    |
|                                | orjson+jsonpath-ng | 19.638   | 2000    | 286.2    |
| wide_arrays:$.records\[0\]     | strata             | 9.748    | 1       | 286.2    |
|                                | orjson+jsonpath-ng | 15.716   | 1       | 286.2    |
|                                | orjson+jmespath    | 17.226   | 1       | 286.2    |
| mixed:$.records\[\*\].id       | strata             | 2.099    | 2000    | 288.6    |
|                                | orjson+jmespath    | 6.125    | 2000    | 288.6    |
|                                | orjson+jsonpath-ng | 8.193    | 2000    | 288.6    |
| mixed:$.records\[0\]           | strata             | 1.997    | 1       | 288.6    |
|                                | orjson+jsonpath-ng | 4.763    | 1       | 288.6    |
|                                | orjson+jmespath    | 5.098    | 1       | 288.6    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.303    | 2000    | 287.7    |
|                                            | jmespath    | 1.039    | 2000    | 287.7    |
|                                            | jsonpath-ng | 16.010   | 2000    | 288.5    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.187    | 100999  | 288.6    |
|                                            | jmespath    | 67.779   | 2000    | 292.4    |
|                                            | jsonpath-ng | 280.117  | 100999  | 294.8    |
| $..price                                   | strata      | 8.890    | 100999  | 296.0    |
|                                            | jsonpath-ng | 906.309  | 100999  | 278.5    |
| $.users\[?(@.age>30)\]                     | strata      | 0.335    | 1606    | 276.6    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 10.269   | 6650    | 276.6    |
| flat:$.records\[\*\].id                    | strata      | 0.143    | 2000    | 279.3    |
|                                            | jmespath    | 0.775    | 2000    | 279.3    |
|                                            | jsonpath-ng | 4.685    | 2000    | 279.3    |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 279.3    |
|                                            | jsonpath-ng | 0.007    | 1       | 279.3    |
|                                            | jmespath    | 0.014    | 1       | 279.3    |
| nested:$.records\[\*\].id                  | strata      | 0.122    | 2000    | 281.8    |
|                                            | jmespath    | 0.809    | 2000    | 281.8    |
|                                            | jsonpath-ng | 6.224    | 2000    | 281.8    |
| nested:$.records\[0\]                      | strata      | 0.004    | 1       | 281.8    |
|                                            | jmespath    | 0.009    | 1       | 281.8    |
|                                            | jsonpath-ng | 0.016    | 1       | 281.8    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.122    | 2000    | 286.2    |
|                                            | jmespath    | 0.730    | 2000    | 286.2    |
|                                            | jsonpath-ng | 7.027    | 2000    | 286.2    |
| wide_arrays:$.records\[0\]                 | strata      | 0.010    | 1       | 286.2    |
|                                            | jmespath    | 0.010    | 1       | 286.2    |
|                                            | jsonpath-ng | 0.017    | 1       | 286.2    |
| mixed:$.records\[\*\].id                   | strata      | 0.117    | 2000    | 288.6    |
|                                            | jmespath    | 0.740    | 2000    | 288.6    |
|                                            | jsonpath-ng | 6.984    | 2000    | 288.6    |
| mixed:$.records\[0\]                       | strata      | 0.006    | 1       | 288.6    |
|                                            | jmespath    | 0.008    | 1       | 288.6    |
|                                            | jsonpath-ng | 0.011    | 1       | 288.6    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **20.2% faster** than #2 (msgspec) |
| loads (NDJSON)      | **#1** / 5              | **37.0% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **20.3% faster** than #2 (orjson)  |
| load (NDJSON file)  | **#1** / 5              | **29.2% faster** than #2 (orjson)  |
| dumps               | **#1** / 5              | **20.0% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **22.1% faster** than #2 (orjson)  |
| loads (flat)        | **#1** / 5              | **23.8% faster** than #2 (orjson)  |
| load (flat)         | **#1** / 5              | **30.0% faster** than #2 (msgspec) |
| dumps (flat)        | **#1** / 5              | **4.0% faster** than #2 (orjson)   |
| dump (flat)         | **#1** / 5              | **8.8% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **91.6% faster** than #2 (orjson)  |
| load (nested)       | **#1** / 5              | **56.5% faster** than #2 (orjson)  |
| dumps (nested)      | **#3** / 5              | 14.6% behind #1 (orjson)           |
| dump (nested)       | **#2** / 5              | 0.7% behind #1 (msgspec)           |
| loads (wide_arrays) | **#1** / 5              | **1.2% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#2** / 5              | 1.0% behind #1 (orjson)            |
| dumps (wide_arrays) | **#1** / 5              | **22.9% faster** than #2 (msgspec) |
| dump (wide_arrays)  | **#1** / 5              | **7.5% faster** than #2 (msgspec)  |
| loads (mixed)       | **#1** / 5              | **46.5% faster** than #2 (msgspec) |
| load (mixed)        | **#1** / 5              | **54.8% faster** than #2 (msgspec) |
| dumps (mixed)       | **#2** / 5              | 3.8% behind #1 (orjson)            |
| dump (mixed)        | **#3** / 5              | 5.3% behind #1 (orjson)            |
| search (JSONPath)   | **#1** in 11/11 queries | avg **731% faster**                |
| query (JSONPath)    | **#1** in 13/13 queries | -                                  |
