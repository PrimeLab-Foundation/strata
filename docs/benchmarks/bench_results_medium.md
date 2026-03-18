# Strata Benchmark Results

Generated: 2026-03-18 19:51:25

## Environment

- Commit: f5870e0f4dbf614611ada7aa37a627b2e43d1579
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
| strata        | flat         | 2.896    | 3.337       | 3.458    | 272.5    |
| orjson        | flat         | 3.756    | 4.448       | 4.720    | 275.0    |
| msgspec       | flat         | 4.070    | 5.087       | 5.872    | 275.0    |
| ujson         | flat         | 6.383    | 6.589       | 7.772    | 275.0    |
| json (stdlib) | flat         | 9.690    | 12.844      | 13.389   | 275.0    |
| strata        | mixed        | 2.941    | 3.319       | 3.455    | 292.6    |
| orjson        | mixed        | 4.162    | 4.838       | 5.288    | 294.9    |
| msgspec       | mixed        | 4.409    | 5.316       | 5.361    | 294.9    |
| ujson         | mixed        | 6.143    | 7.065       | 7.331    | 294.9    |
| json (stdlib) | mixed        | 11.926   | 12.516      | 14.981   | 294.9    |
| strata        | nested       | 2.533    | 3.041       | 3.069    | 279.4    |
| msgspec       | nested       | 4.671    | 5.828       | 6.067    | 281.8    |
| orjson        | nested       | 5.220    | 6.611       | 8.236    | 281.8    |
| ujson         | nested       | 5.431    | 7.308       | 8.239    | 281.8    |
| json (stdlib) | nested       | 10.155   | 10.328      | 14.539   | 281.8    |
| strata        | users.json   | 25.932   | 31.047      | 31.630   | 73.6     |
| orjson        | users.json   | 27.113   | 37.959      | 59.906   | 93.5     |
| msgspec       | users.json   | 29.850   | 41.025      | 43.548   | 92.3     |
| ujson         | users.json   | 47.844   | 56.325      | 57.636   | 109.9    |
| json (stdlib) | users.json   | 55.248   | 67.281      | 79.705   | 98.6     |
| strata        | users.ndjson | 25.244   | 31.724      | 32.879   | 254.8    |
| orjson        | users.ndjson | 35.141   | 37.131      | 38.630   | 262.8    |
| msgspec       | users.ndjson | 36.632   | 37.794      | 38.944   | 264.0    |
| ujson         | users.ndjson | 53.110   | 54.242      | 55.299   | 263.0    |
| json (stdlib) | users.ndjson | 64.645   | 67.086      | 68.423   | 265.0    |
| strata        | wide_arrays  | 11.919   | 14.147      | 14.618   | 294.0    |
| orjson        | wide_arrays  | 12.894   | 15.945      | 17.027   | 304.2    |
| msgspec       | wide_arrays  | 17.377   | 22.123      | 31.286   | 304.2    |
| ujson         | wide_arrays  | 26.840   | 29.536      | 30.233   | 304.2    |
| json (stdlib) | wide_arrays  | 51.679   | 59.935      | 62.446   | 304.2    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 3.285    | 3.548       | 3.582    | 275.0    |
| msgspec       | flat         | 4.484    | 4.723       | 4.934    | 275.4    |
| orjson        | flat         | 4.722    | 4.964       | 5.254    | 275.0    |
| ujson         | flat         | 6.054    | 7.398       | 8.045    | 275.4    |
| json (stdlib) | flat         | 11.089   | 12.404      | 12.449   | 275.4    |
| strata        | mixed        | 3.180    | 3.765       | 3.935    | 294.9    |
| orjson        | mixed        | 4.385    | 4.966       | 5.078    | 294.9    |
| msgspec       | mixed        | 4.277    | 5.634       | 6.148    | 294.9    |
| ujson         | mixed        | 6.961    | 7.666       | 7.769    | 294.9    |
| json (stdlib) | mixed        | 12.128   | 12.467      | 13.658   | 294.9    |
| strata        | nested       | 2.680    | 3.117       | 3.503    | 281.8    |
| orjson        | nested       | 6.010    | 6.141       | 6.187    | 281.8    |
| ujson         | nested       | 5.578    | 8.782       | 9.301    | 281.8    |
| msgspec       | nested       | 7.030    | 9.420       | 13.533   | 281.8    |
| json (stdlib) | nested       | 9.885    | 11.533      | 11.654   | 281.8    |
| strata        | users.json   | 25.337   | 31.323      | 33.809   | 100.0    |
| orjson        | users.json   | 26.652   | 40.521      | 53.969   | 100.0    |
| msgspec       | users.json   | 28.280   | 43.564      | 47.538   | 104.9    |
| ujson         | users.json   | 40.001   | 59.376      | 63.243   | 122.6    |
| json (stdlib) | users.json   | 61.068   | 67.797      | 69.600   | 104.9    |
| strata        | users.ndjson | 26.711   | 33.437      | 35.331   | 267.0    |
| orjson        | users.ndjson | 34.521   | 35.715      | 45.776   | 268.0    |
| msgspec       | users.ndjson | 36.085   | 38.215      | 44.099   | 269.0    |
| ujson         | users.ndjson | 46.998   | 54.773      | 55.098   | 268.0    |
| json (stdlib) | users.ndjson | 63.432   | 67.488      | 68.700   | 270.0    |
| orjson        | wide_arrays  | 14.926   | 16.431      | 17.777   | 303.9    |
| strata        | wide_arrays  | 16.082   | 17.082      | 17.503   | 304.3    |
| msgspec       | wide_arrays  | 16.679   | 19.570      | 20.129   | 303.9    |
| ujson         | wide_arrays  | 25.944   | 32.750      | 34.433   | 303.9    |
| json (stdlib) | wide_arrays  | 52.849   | 55.859      | 60.978   | 241.4    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.906    | 0.910       | 0.998    | 275.4    |
| orjson  | flat        | 1.011    | 1.067       | 1.107    | 277.7    |
| msgspec | flat        | 1.186    | 1.249       | 1.294    | 278.6    |
| ujson   | flat        | 5.310    | 5.520       | 5.948    | 279.4    |
| json    | flat        | 9.451    | 9.734       | 9.738    | 279.4    |
| strata  | mixed       | 1.013    | 1.046       | 1.123    | 294.9    |
| orjson  | mixed       | 1.107    | 1.126       | 1.140    | 294.9    |
| msgspec | mixed       | 1.172    | 1.191       | 1.193    | 294.9    |
| ujson   | mixed       | 5.066    | 5.153       | 5.336    | 294.9    |
| json    | mixed       | 11.486   | 11.585      | 11.815   | 294.9    |
| strata  | nested      | 0.749    | 0.776       | 1.004    | 281.8    |
| orjson  | nested      | 0.779    | 0.787       | 0.802    | 281.8    |
| msgspec | nested      | 0.790    | 0.807       | 0.842    | 281.8    |
| ujson   | nested      | 3.983    | 3.996       | 4.166    | 281.8    |
| json    | nested      | 5.228    | 5.247       | 5.254    | 281.8    |
| strata  | users.json  | 6.522    | 6.535       | 6.559    | 109.2    |
| msgspec | users.json  | 10.871   | 10.921      | 11.001   | 160.0    |
| orjson  | users.json  | 10.927   | 10.963      | 11.052   | 131.6    |
| ujson   | users.json  | 41.560   | 41.872      | 42.306   | 165.8    |
| json    | users.json  | 57.978   | 58.041      | 58.226   | 191.1    |
| strata  | wide_arrays | 5.576    | 5.608       | 5.729    | 241.4    |
| msgspec | wide_arrays | 6.850    | 7.018       | 7.177    | 251.5    |
| orjson  | wide_arrays | 7.109    | 7.307       | 7.392    | 231.8    |
| ujson   | wide_arrays | 28.797   | 29.027      | 29.504   | 253.5    |
| json    | wide_arrays | 80.258   | 81.057      | 81.242   | 274.3    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.153    | 1.281       | 1.334    | 279.4    |
| msgspec       | flat        | 1.547    | 1.560       | 1.592    | 279.4    |
| orjson        | flat        | 1.670    | 1.773       | 2.120    | 279.4    |
| ujson         | flat        | 5.256    | 5.631       | 6.056    | 279.4    |
| json (stdlib) | flat        | 33.219   | 33.310      | 34.454   | 279.4    |
| strata        | mixed       | 1.358    | 1.405       | 1.518    | 294.9    |
| orjson        | mixed       | 1.453    | 1.465       | 1.640    | 294.9    |
| msgspec       | mixed       | 1.525    | 1.528       | 1.562    | 294.9    |
| ujson         | mixed       | 5.488    | 5.552       | 5.690    | 294.9    |
| json (stdlib) | mixed       | 38.991   | 39.141      | 39.228   | 294.9    |
| strata        | nested      | 0.950    | 1.012       | 1.105    | 281.8    |
| orjson        | nested      | 1.079    | 1.131       | 1.152    | 281.8    |
| msgspec       | nested      | 1.209    | 1.396       | 1.684    | 281.8    |
| ujson         | nested      | 4.198    | 4.285       | 4.340    | 281.8    |
| json (stdlib) | nested      | 47.594   | 47.827      | 47.915   | 281.8    |
| strata        | users.json  | 7.499    | 7.519       | 7.519    | 197.4    |
| msgspec       | users.json  | 11.560   | 11.717      | 11.823   | 242.0    |
| orjson        | users.json  | 11.723   | 11.802      | 12.791   | 219.7    |
| ujson         | users.json  | 42.870   | 42.900      | 43.422   | 219.7    |
| json (stdlib) | users.json  | 428.710  | 430.976     | 431.493  | 242.1    |
| strata        | wide_arrays | 6.667    | 6.690       | 7.072    | 274.3    |
| msgspec       | wide_arrays | 7.949    | 8.178       | 8.246    | 292.6    |
| orjson        | wide_arrays | 8.276    | 8.291       | 8.438    | 272.1    |
| ujson         | wide_arrays | 30.115   | 30.346      | 30.645   | 274.1    |
| json (stdlib) | wide_arrays | 178.497  | 178.684     | 179.261  | 292.6    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 12.141   | 2000    | 239.2    |
|                                | orjson+jmespath    | 30.981   | 2000    | 242.3    |
|                                | orjson+jsonpath-ng | 33.978   | 2000    | 244.5    |
| all item prices                | strata             | 14.677   | 100999  | 248.2    |
|                                | orjson+jmespath    | 85.543   | 2000    | 257.4    |
|                                | orjson+jsonpath-ng | 304.629  | 100999  | 264.2    |
| recursive price                | strata             | 14.943   | 100999  | 262.0    |
|                                | orjson+jsonpath-ng | 886.995  | 100999  | 265.4    |
| flat:$.records\[\*\].id        | strata             | 1.895    | 2000    | 279.4    |
|                                | orjson+jmespath    | 4.962    | 2000    | 279.4    |
|                                | orjson+jsonpath-ng | 7.191    | 2000    | 279.4    |
| flat:$.records\[0\]            | strata             | 1.945    | 1       | 279.4    |
|                                | orjson+jsonpath-ng | 4.355    | 1       | 279.4    |
|                                | orjson+jmespath    | 4.771    | 1       | 279.4    |
| nested:$.records\[\*\].id      | strata             | 1.462    | 2000    | 281.8    |
|                                | orjson+jmespath    | 5.560    | 2000    | 281.8    |
|                                | orjson+jsonpath-ng | 7.846    | 2000    | 281.8    |
| nested:$.records\[0\]          | strata             | 1.473    | 1       | 281.8    |
|                                | orjson+jmespath    | 5.038    | 1       | 281.8    |
|                                | orjson+jsonpath-ng | 5.539    | 1       | 281.8    |
| wide_arrays:$.records\[\*\].id | strata             | 9.243    | 2000    | 292.6    |
|                                | orjson+jmespath    | 15.093   | 2000    | 292.6    |
|                                | orjson+jsonpath-ng | 18.231   | 2000    | 292.6    |
| wide_arrays:$.records\[0\]     | strata             | 9.684    | 1       | 292.6    |
|                                | orjson+jmespath    | 14.605   | 1       | 292.6    |
|                                | orjson+jsonpath-ng | 15.280   | 1       | 292.6    |
| mixed:$.records\[\*\].id       | strata             | 1.800    | 2000    | 294.9    |
|                                | orjson+jmespath    | 5.104    | 2000    | 294.9    |
|                                | orjson+jsonpath-ng | 7.771    | 2000    | 294.9    |
| mixed:$.records\[0\]           | strata             | 1.774    | 1       | 294.9    |
|                                | orjson+jsonpath-ng | 4.387    | 1       | 294.9    |
|                                | orjson+jmespath    | 4.678    | 1       | 294.9    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.299    | 2000    | 287.6    |
|                                            | jmespath    | 0.870    | 2000    | 287.6    |
|                                            | jsonpath-ng | 15.654   | 2000    | 288.4    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.190    | 100999  | 288.5    |
|                                            | jmespath    | 61.229   | 2000    | 292.3    |
|                                            | jsonpath-ng | 253.779  | 100999  | 294.6    |
| $..price                                   | strata      | 8.674    | 100999  | 294.4    |
|                                            | jsonpath-ng | 876.243  | 100999  | 278.1    |
| $.users\[?(@.age>30)\]                     | strata      | 0.332    | 1606    | 276.2    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.359    | 6650    | 276.2    |
| flat:$.records\[\*\].id                    | strata      | 0.124    | 2000    | 279.4    |
|                                            | jmespath    | 0.673    | 2000    | 279.4    |
|                                            | jsonpath-ng | 4.236    | 2000    | 279.4    |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 279.4    |
|                                            | jmespath    | 0.007    | 1       | 279.4    |
|                                            | jsonpath-ng | 0.008    | 1       | 279.4    |
| nested:$.records\[\*\].id                  | strata      | 0.141    | 2000    | 281.8    |
|                                            | jmespath    | 0.691    | 2000    | 281.8    |
|                                            | jsonpath-ng | 6.170    | 2000    | 281.8    |
| nested:$.records\[0\]                      | strata      | 0.004    | 1       | 281.8    |
|                                            | jmespath    | 0.007    | 1       | 281.8    |
|                                            | jsonpath-ng | 0.010    | 1       | 281.8    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.105    | 2000    | 292.6    |
|                                            | jmespath    | 0.692    | 2000    | 292.6    |
|                                            | jsonpath-ng | 6.830    | 2000    | 292.6    |
| wide_arrays:$.records\[0\]                 | strata      | 0.006    | 1       | 292.6    |
|                                            | jmespath    | 0.009    | 1       | 292.6    |
|                                            | jsonpath-ng | 0.010    | 1       | 292.6    |
| mixed:$.records\[\*\].id                   | strata      | 0.111    | 2000    | 294.9    |
|                                            | jmespath    | 0.706    | 2000    | 294.9    |
|                                            | jsonpath-ng | 5.155    | 2000    | 294.9    |
| mixed:$.records\[0\]                       | strata      | 0.006    | 1       | 294.9    |
|                                            | jsonpath-ng | 0.009    | 1       | 294.9    |
|                                            | jmespath    | 0.009    | 1       | 294.9    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **4.6% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **39.2% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **5.2% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **29.2% faster** than #2 (orjson)  |
| dumps               | **#1** / 5              | **66.7% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **54.2% faster** than #2 (msgspec) |
| loads (flat)        | **#1** / 5              | **29.7% faster** than #2 (orjson)  |
| load (flat)         | **#1** / 5              | **36.5% faster** than #2 (msgspec) |
| dumps (flat)        | **#1** / 5              | **11.6% faster** than #2 (orjson)  |
| dump (flat)         | **#1** / 5              | **34.2% faster** than #2 (msgspec) |
| loads (nested)      | **#1** / 5              | **84.4% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **108.2% faster** than #2 (ujson)  |
| dumps (nested)      | **#1** / 5              | **4.0% faster** than #2 (orjson)   |
| dump (nested)       | **#1** / 5              | **13.5% faster** than #2 (orjson)  |
| loads (wide_arrays) | **#1** / 5              | **8.2% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#2** / 5              | 7.7% behind #1 (orjson)            |
| dumps (wide_arrays) | **#1** / 5              | **22.9% faster** than #2 (msgspec) |
| dump (wide_arrays)  | **#1** / 5              | **19.2% faster** than #2 (msgspec) |
| loads (mixed)       | **#1** / 5              | **41.5% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **34.5% faster** than #2 (msgspec) |
| dumps (mixed)       | **#1** / 5              | **9.2% faster** than #2 (orjson)   |
| dump (mixed)        | **#1** / 5              | **7.0% faster** than #2 (orjson)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **702% faster**                |
| query (JSONPath)    | **#1** in 13/13 queries | -                                  |
