# Strata Benchmark Results

Generated: 2026-03-15 19:01:59

## Environment

- Commit: 07f85a0f96b182dd7aeee9835dc618399d95321a
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
| strata        | flat         | 3.755    | 4.082       | 4.106    | 208.0    |
| orjson        | flat         | 4.268    | 4.856       | 5.290    | 213.0    |
| msgspec       | flat         | 4.790    | 5.316       | 7.699    | 213.0    |
| ujson         | flat         | 7.407    | 7.687       | 7.926    | 213.0    |
| json (stdlib) | flat         | 12.155   | 12.943      | 13.188   | 213.0    |
| strata        | mixed        | 4.803    | 5.586       | 5.714    | 273.3    |
| orjson        | mixed        | 6.636    | 7.482       | 7.669    | 273.3    |
| msgspec       | mixed        | 9.215    | 9.912       | 10.224   | 273.3    |
| ujson         | mixed        | 9.406    | 11.000      | 11.421   | 273.3    |
| json (stdlib) | mixed        | 16.174   | 17.584      | 18.120   | 273.3    |
| strata        | nested       | 3.395    | 3.718       | 4.007    | 219.7    |
| msgspec       | nested       | 6.544    | 7.073       | 7.777    | 219.8    |
| orjson        | nested       | 6.357    | 7.096       | 13.466   | 219.7    |
| ujson         | nested       | 7.040    | 9.094       | 9.309    | 219.8    |
| json (stdlib) | nested       | 9.394    | 15.358      | 21.323   | 219.8    |
| strata        | users.json   | 28.794   | 33.540      | 33.875   | 73.4     |
| msgspec       | users.json   | 36.495   | 44.703      | 50.426   | 92.1     |
| orjson        | users.json   | 29.730   | 45.072      | 56.200   | 93.3     |
| ujson         | users.json   | 39.490   | 57.152      | 85.371   | 109.8    |
| json (stdlib) | users.json   | 58.167   | 67.896      | 95.847   | 98.4     |
| strata        | users.ndjson | 28.042   | 38.106      | 40.469   | 195.6    |
| orjson        | users.ndjson | 37.937   | 39.235      | 47.188   | 203.6    |
| msgspec       | users.ndjson | 40.316   | 44.856      | 47.395   | 204.8    |
| ujson         | users.ndjson | 59.404   | 60.405      | 60.448   | 203.8    |
| json (stdlib) | users.ndjson | 75.582   | 77.622      | 80.321   | 205.8    |
| strata        | wide_arrays  | 10.886   | 14.540      | 14.722   | 233.1    |
| orjson        | wide_arrays  | 13.388   | 17.478      | 19.021   | 243.4    |
| msgspec       | wide_arrays  | 16.390   | 20.263      | 21.282   | 243.4    |
| ujson         | wide_arrays  | 28.576   | 33.332      | 39.299   | 243.4    |
| json (stdlib) | wide_arrays  | 58.683   | 62.145      | 70.844   | 217.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 4.094    | 4.678       | 4.934    | 213.1    |
| orjson        | flat         | 4.783    | 5.441       | 5.551    | 213.1    |
| msgspec       | flat         | 5.618    | 6.380       | 7.487    | 213.7    |
| ujson         | flat         | 7.441    | 8.044       | 11.681   | 213.7    |
| json (stdlib) | flat         | 11.739   | 12.033      | 12.152   | 213.7    |
| strata        | mixed        | 4.481    | 5.182       | 5.192    | 273.3    |
| msgspec       | mixed        | 6.637    | 7.629       | 8.538    | 273.3    |
| ujson         | mixed        | 8.870    | 10.588      | 11.426   | 273.3    |
| orjson        | mixed        | 10.794   | 10.916      | 13.659   | 273.3    |
| json (stdlib) | mixed        | 16.650   | 17.079      | 19.770   | 273.3    |
| strata        | nested       | 3.056    | 3.530       | 3.561    | 219.8    |
| orjson        | nested       | 5.875    | 7.311       | 7.938    | 219.8    |
| msgspec       | nested       | 5.729    | 7.575       | 7.681    | 219.8    |
| ujson         | nested       | 7.400    | 9.560       | 15.604   | 219.8    |
| json (stdlib) | nested       | 10.287   | 11.108      | 11.789   | 219.8    |
| strata        | users.json   | 28.118   | 33.405      | 33.915   | 99.7     |
| orjson        | users.json   | 32.666   | 39.729      | 39.989   | 99.7     |
| msgspec       | users.json   | 42.112   | 42.232      | 44.667   | 104.7    |
| ujson         | users.json   | 40.138   | 57.609      | 62.233   | 122.4    |
| json (stdlib) | users.json   | 54.697   | 68.345      | 83.014   | 104.7    |
| strata        | users.ndjson | 32.076   | 40.817      | 44.902   | 207.8    |
| msgspec       | users.ndjson | 42.600   | 44.801      | 45.484   | 207.0    |
| orjson        | users.ndjson | 43.024   | 44.918      | 47.401   | 208.8    |
| ujson         | users.ndjson | 57.837   | 70.044      | 84.783   | 206.0    |
| json (stdlib) | users.ndjson | 71.711   | 75.843      | 84.668   | 208.0    |
| strata        | wide_arrays  | 13.004   | 15.793      | 18.611   | 217.4    |
| orjson        | wide_arrays  | 14.584   | 19.829      | 20.501   | 217.4    |
| msgspec       | wide_arrays  | 18.385   | 23.111      | 24.494   | 220.5    |
| ujson         | wide_arrays  | 33.818   | 33.954      | 35.138   | 220.5    |
| json (stdlib) | wide_arrays  | 63.095   | 67.159      | 68.672   | 220.5    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 1.173    | 1.239       | 1.262    | 216.5    |
| msgspec | flat        | 1.364    | 1.402       | 1.421    | 216.5    |
| strata  | flat        | 1.402    | 1.407       | 1.413    | 213.7    |
| ujson   | flat        | 5.285    | 5.334       | 5.474    | 219.5    |
| json    | flat        | 10.245   | 10.375      | 10.430   | 219.6    |
| orjson  | mixed       | 1.487    | 1.535       | 1.566    | 273.3    |
| msgspec | mixed       | 1.620    | 1.711       | 1.742    | 273.3    |
| strata  | mixed       | 1.879    | 1.925       | 2.003    | 273.3    |
| ujson   | mixed       | 7.191    | 7.489       | 8.133    | 273.3    |
| json    | mixed       | 17.450   | 17.683      | 18.166   | 273.3    |
| orjson  | nested      | 0.816    | 0.821       | 0.889    | 219.8    |
| msgspec | nested      | 0.854    | 0.901       | 0.903    | 219.8    |
| strata  | nested      | 0.923    | 1.122       | 1.157    | 219.8    |
| ujson   | nested      | 4.208    | 4.252       | 4.350    | 219.8    |
| json    | nested      | 5.501    | 5.844       | 6.087    | 219.8    |
| msgspec | users.json  | 10.765   | 10.792      | 11.251   | 159.5    |
| orjson  | users.json  | 10.697   | 10.895      | 11.260   | 131.2    |
| strata  | users.json  | 10.788   | 11.036      | 12.272   | 108.9    |
| ujson   | users.json  | 39.901   | 40.314      | 41.087   | 165.4    |
| json    | users.json  | 57.286   | 57.363      | 57.831   | 190.7    |
| msgspec | wide_arrays | 7.948    | 8.067       | 8.646    | 237.2    |
| orjson  | wide_arrays | 8.186    | 8.271       | 8.286    | 217.5    |
| strata  | wide_arrays | 8.529    | 8.562       | 8.871    | 220.5    |
| ujson   | wide_arrays | 33.498   | 35.281      | 36.320   | 239.2    |
| json    | wide_arrays | 88.579   | 90.516      | 99.337   | 260.0    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 1.445    | 1.541       | 1.635    | 219.6    |
| strata        | flat        | 1.631    | 1.730       | 1.780    | 219.6    |
| msgspec       | flat        | 2.189    | 2.210       | 5.013    | 219.6    |
| ujson         | flat        | 6.197    | 7.013       | 7.072    | 219.6    |
| json (stdlib) | flat        | 41.598   | 42.032      | 42.234   | 219.6    |
| orjson        | mixed       | 2.200    | 2.241       | 2.785    | 273.3    |
| msgspec       | mixed       | 2.339    | 2.363       | 2.481    | 273.3    |
| strata        | mixed       | 2.279    | 2.393       | 2.479    | 273.3    |
| ujson         | mixed       | 7.800    | 8.325       | 8.631    | 273.3    |
| json (stdlib) | mixed       | 47.317   | 53.655      | 54.693   | 273.3    |
| orjson        | nested      | 1.123    | 1.193       | 1.462    | 219.8    |
| strata        | nested      | 1.125    | 1.415       | 1.508    | 219.8    |
| msgspec       | nested      | 1.381    | 1.454       | 1.915    | 219.8    |
| ujson         | nested      | 4.870    | 4.976       | 5.506    | 219.8    |
| json (stdlib) | nested      | 50.046   | 53.233      | 54.319   | 219.8    |
| msgspec       | users.json  | 12.249   | 12.589      | 12.805   | 243.5    |
| orjson        | users.json  | 12.855   | 13.032      | 16.543   | 219.9    |
| strata        | users.json  | 13.657   | 14.945      | 16.934   | 196.9    |
| ujson         | users.json  | 42.808   | 43.029      | 44.067   | 221.2    |
| json (stdlib) | users.json  | 432.257  | 435.256     | 467.436  | 241.7    |
| msgspec       | wide_arrays | 9.386    | 9.530       | 10.322   | 275.8    |
| strata        | wide_arrays | 9.437    | 9.546       | 9.742    | 260.0    |
| orjson        | wide_arrays | 9.431    | 10.954      | 12.486   | 257.8    |
| ujson         | wide_arrays | 37.630   | 38.110      | 40.119   | 262.2    |
| json (stdlib) | wide_arrays | 200.866  | 203.189     | 232.027  | 273.3    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 12.528   | 2000    | 239.8    |
|                                | orjson+jmespath    | 33.727   | 2000    | 242.0    |
|                                | orjson+jsonpath-ng | 36.901   | 2000    | 244.2    |
| all item prices                | strata             | 14.248   | 100999  | 245.1    |
|                                | orjson+jmespath    | 83.391   | 2000    | 254.3    |
|                                | orjson+jsonpath-ng | 266.385  | 100999  | 261.1    |
| recursive price                | strata             | 13.923   | 100999  | 258.4    |
|                                | orjson+jsonpath-ng | 890.011  | 100999  | 200.9    |
| flat:$.records\[\*\].id        | strata             | 1.900    | 2000    | 219.6    |
|                                | orjson+jmespath    | 5.982    | 2000    | 219.6    |
|                                | orjson+jsonpath-ng | 8.073    | 2000    | 219.6    |
| flat:$.records\[0\]            | strata             | 1.879    | 1       | 219.6    |
|                                | orjson+jmespath    | 4.569    | 1       | 219.6    |
|                                | orjson+jsonpath-ng | 4.656    | 1       | 219.6    |
| nested:$.records\[\*\].id      | strata             | 1.473    | 2000    | 219.8    |
|                                | orjson+jmespath    | 6.239    | 2000    | 219.8    |
|                                | orjson+jsonpath-ng | 9.129    | 2000    | 219.8    |
| nested:$.records\[0\]          | strata             | 1.478    | 1       | 219.8    |
|                                | orjson+jmespath    | 5.911    | 1       | 219.8    |
|                                | orjson+jsonpath-ng | 5.983    | 1       | 219.8    |
| wide_arrays:$.records\[\*\].id | strata             | 9.672    | 2000    | 273.3    |
|                                | orjson+jmespath    | 16.974   | 2000    | 273.3    |
|                                | orjson+jsonpath-ng | 20.130   | 2000    | 273.3    |
| wide_arrays:$.records\[0\]     | strata             | 9.528    | 1       | 273.3    |
|                                | orjson+jsonpath-ng | 16.444   | 1       | 273.3    |
|                                | orjson+jmespath    | 17.123   | 1       | 273.3    |
| mixed:$.records\[\*\].id       | strata             | 2.068    | 2000    | 273.3    |
|                                | orjson+jmespath    | 7.517    | 2000    | 273.3    |
|                                | orjson+jsonpath-ng | 10.127   | 2000    | 273.3    |
| mixed:$.records\[0\]           | strata             | 2.077    | 1       | 273.3    |
|                                | orjson+jmespath    | 5.489    | 1       | 273.3    |
|                                | orjson+jsonpath-ng | 6.292    | 1       | 273.3    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.311    | 2000    | 229.4    |
|                                            | jmespath    | 0.907    | 2000    | 229.4    |
|                                            | jsonpath-ng | 16.205   | 2000    | 230.2    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.501    | 100999  | 230.2    |
|                                            | jmespath    | 63.436   | 2000    | 235.1    |
|                                            | jsonpath-ng | 254.676  | 100999  | 236.5    |
| $..price                                   | strata      | 8.725    | 100999  | 236.4    |
|                                            | jsonpath-ng | 932.604  | 100999  | 218.9    |
| $.users\[?(@.age>30)\]                     | strata      | 0.342    | 1606    | 216.9    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 10.169   | 6650    | 216.9    |
| flat:$.records\[\*\].id                    | strata      | 0.120    | 2000    | 219.6    |
|                                            | jmespath    | 0.751    | 2000    | 219.6    |
|                                            | jsonpath-ng | 4.728    | 2000    | 219.6    |
| flat:$.records\[0\]                        | strata      | 0.008    | 1       | 219.7    |
|                                            | jmespath    | 0.020    | 1       | 219.7    |
|                                            | jsonpath-ng | 0.024    | 1       | 219.7    |
| nested:$.records\[\*\].id                  | strata      | 0.158    | 2000    | 219.8    |
|                                            | jmespath    | 0.739    | 2000    | 219.8    |
|                                            | jsonpath-ng | 6.124    | 2000    | 219.8    |
| nested:$.records\[0\]                      | strata      | 0.006    | 1       | 219.8    |
|                                            | jmespath    | 0.014    | 1       | 219.8    |
|                                            | jsonpath-ng | 0.017    | 1       | 219.8    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.144    | 2000    | 273.3    |
|                                            | jmespath    | 0.958    | 2000    | 273.3    |
|                                            | jsonpath-ng | 9.152    | 2000    | 273.3    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 273.3    |
|                                            | jmespath    | 0.017    | 1       | 273.3    |
|                                            | jsonpath-ng | 0.022    | 1       | 273.3    |
| mixed:$.records\[\*\].id                   | strata      | 0.130    | 2000    | 273.3    |
|                                            | jmespath    | 0.960    | 2000    | 273.3    |
|                                            | jsonpath-ng | 6.719    | 2000    | 273.3    |
| mixed:$.records\[0\]                       | jsonpath-ng | 0.010    | 1       | 273.3    |
|                                            | strata      | 0.012    | 1       | 273.3    |
|                                            | jmespath    | 0.014    | 1       | 273.3    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **3.3% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **35.3% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **16.2% faster** than #2 (orjson)  |
| load (NDJSON file)  | **#1** / 5              | **32.8% faster** than #2 (msgspec) |
| dumps               | **#3** / 5              | 0.8% behind #1 (orjson)            |
| dump                | **#3** / 5              | 11.5% behind #1 (msgspec)          |
| loads (flat)        | **#1** / 5              | **13.7% faster** than #2 (orjson)  |
| load (flat)         | **#1** / 5              | **16.8% faster** than #2 (orjson)  |
| dumps (flat)        | **#3** / 5              | 19.5% behind #1 (orjson)           |
| dump (flat)         | **#2** / 5              | 12.9% behind #1 (orjson)           |
| loads (nested)      | **#1** / 5              | **87.3% faster** than #2 (orjson)  |
| load (nested)       | **#1** / 5              | **87.5% faster** than #2 (msgspec) |
| dumps (nested)      | **#3** / 5              | 13.1% behind #1 (orjson)           |
| dump (nested)       | **#2** / 5              | 0.2% behind #1 (orjson)            |
| loads (wide_arrays) | **#1** / 5              | **23.0% faster** than #2 (orjson)  |
| load (wide_arrays)  | **#1** / 5              | **12.2% faster** than #2 (orjson)  |
| dumps (wide_arrays) | **#3** / 5              | 7.3% behind #1 (msgspec)           |
| dump (wide_arrays)  | **#3** / 5              | 0.5% behind #1 (msgspec)           |
| loads (mixed)       | **#1** / 5              | **38.2% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **48.1% faster** than #2 (msgspec) |
| dumps (mixed)       | **#3** / 5              | 26.4% behind #1 (orjson)           |
| dump (mixed)        | **#2** / 5              | 3.6% behind #1 (orjson)            |
| search (JSONPath)   | **#1** in 11/11 queries | avg **773% faster**                |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
