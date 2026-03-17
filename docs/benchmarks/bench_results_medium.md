# Strata Benchmark Results

Generated: 2026-03-17 15:14:37

## Environment

- Commit: 75205226403b1b3dd7be908ebcd6e9ece844e9a9
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
| strata        | flat         | 2.316    | 2.758       | 2.835    | 272.4    |
| orjson        | flat         | 3.612    | 4.157       | 4.354    | 274.9    |
| msgspec       | flat         | 4.254    | 4.422       | 4.573    | 274.9    |
| ujson         | flat         | 6.331    | 9.183       | 12.464   | 274.9    |
| json (stdlib) | flat         | 9.842    | 9.878       | 10.122   | 274.9    |
| strata        | mixed        | 2.683    | 3.243       | 3.563    | 301.4    |
| orjson        | mixed        | 4.618    | 4.860       | 5.563    | 303.7    |
| msgspec       | mixed        | 5.686    | 5.887       | 8.214    | 303.7    |
| ujson         | mixed        | 6.237    | 7.411       | 11.791   | 303.7    |
| json (stdlib) | mixed        | 10.438   | 11.525      | 11.535   | 303.7    |
| strata        | nested       | 1.964    | 2.450       | 2.498    | 280.7    |
| msgspec       | nested       | 4.680    | 6.170       | 6.383    | 283.1    |
| orjson        | nested       | 7.411    | 7.988       | 8.027    | 283.0    |
| ujson         | nested       | 5.537    | 10.323      | 12.470   | 283.1    |
| json (stdlib) | nested       | 8.997    | 10.394      | 10.459   | 283.1    |
| strata        | users.json   | 23.042   | 29.362      | 32.146   | 73.6     |
| orjson        | users.json   | 28.241   | 40.136      | 52.581   | 93.5     |
| msgspec       | users.json   | 29.471   | 42.695      | 58.164   | 92.3     |
| ujson         | users.json   | 48.480   | 58.573      | 77.783   | 110.0    |
| json (stdlib) | users.json   | 59.128   | 71.727      | 73.327   | 98.6     |
| strata        | users.ndjson | 23.185   | 29.716      | 32.363   | 254.7    |
| orjson        | users.ndjson | 33.768   | 36.482      | 47.264   | 262.7    |
| msgspec       | users.ndjson | 37.512   | 39.393      | 42.565   | 263.8    |
| ujson         | users.ndjson | 45.019   | 52.144      | 56.201   | 262.8    |
| json (stdlib) | users.ndjson | 63.411   | 69.545      | 72.079   | 264.8    |
| orjson        | wide_arrays  | 11.972   | 15.403      | 15.585   | 305.5    |
| strata        | wide_arrays  | 16.117   | 16.384      | 17.798   | 295.3    |
| msgspec       | wide_arrays  | 16.651   | 18.971      | 19.161   | 305.5    |
| ujson         | wide_arrays  | 24.607   | 32.082      | 32.614   | 305.5    |
| json (stdlib) | wide_arrays  | 52.186   | 55.978      | 58.357   | 305.5    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 2.484    | 2.876       | 2.883    | 274.9    |
| msgspec       | flat         | 4.422    | 4.549       | 4.861    | 274.9    |
| orjson        | flat         | 4.001    | 5.364       | 6.521    | 274.9    |
| ujson         | flat         | 6.445    | 6.848       | 6.895    | 274.9    |
| json (stdlib) | flat         | 9.934    | 10.200      | 11.988   | 274.9    |
| strata        | mixed        | 2.852    | 3.601       | 3.890    | 303.7    |
| msgspec       | mixed        | 4.756    | 5.777       | 5.832    | 303.7    |
| orjson        | mixed        | 6.101    | 7.300       | 9.028    | 303.7    |
| ujson         | mixed        | 7.042    | 7.921       | 8.072    | 303.7    |
| json (stdlib) | mixed        | 11.022   | 12.528      | 13.460   | 303.7    |
| strata        | nested       | 2.124    | 2.651       | 2.666    | 283.1    |
| msgspec       | nested       | 4.864    | 6.379       | 6.637    | 283.1    |
| orjson        | nested       | 5.367    | 6.664       | 6.923    | 283.1    |
| ujson         | nested       | 5.920    | 10.740      | 12.728   | 283.1    |
| json (stdlib) | nested       | 9.997    | 10.918      | 12.055   | 283.1    |
| strata        | users.json   | 23.179   | 30.200      | 31.445   | 100.0    |
| orjson        | users.json   | 40.479   | 42.341      | 54.461   | 100.0    |
| msgspec       | users.json   | 38.764   | 43.740      | 56.587   | 104.9    |
| ujson         | users.json   | 41.128   | 58.214      | 73.367   | 122.6    |
| json (stdlib) | users.json   | 56.354   | 70.537      | 76.957   | 104.9    |
| strata        | users.ndjson | 24.093   | 31.215      | 31.926   | 266.8    |
| orjson        | users.ndjson | 33.863   | 36.010      | 46.873   | 267.9    |
| msgspec       | users.ndjson | 35.368   | 38.221      | 38.534   | 268.9    |
| ujson         | users.ndjson | 51.272   | 53.326      | 54.479   | 267.9    |
| json (stdlib) | users.ndjson | 64.235   | 65.614      | 68.188   | 269.9    |
| orjson        | wide_arrays  | 12.813   | 16.858      | 19.502   | 305.2    |
| strata        | wide_arrays  | 14.105   | 17.575      | 17.973   | 305.6    |
| msgspec       | wide_arrays  | 16.414   | 20.790      | 23.123   | 305.2    |
| ujson         | wide_arrays  | 27.815   | 30.193      | 31.114   | 305.2    |
| json (stdlib) | wide_arrays  | 53.099   | 58.055      | 58.723   | 305.2    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.874    | 0.899       | 0.949    | 274.9    |
| orjson  | flat        | 0.994    | 0.999       | 1.138    | 277.2    |
| msgspec | flat        | 1.157    | 1.160       | 1.206    | 277.5    |
| ujson   | flat        | 4.779    | 4.899       | 4.950    | 280.2    |
| json    | flat        | 8.994    | 9.005       | 9.055    | 280.7    |
| strata  | mixed       | 1.038    | 1.043       | 1.156    | 303.7    |
| orjson  | mixed       | 1.079    | 1.080       | 1.082    | 303.7    |
| msgspec | mixed       | 1.211    | 1.375       | 1.416    | 303.7    |
| ujson   | mixed       | 5.372    | 5.461       | 5.481    | 303.7    |
| json    | mixed       | 11.585   | 11.837      | 11.889   | 303.7    |
| orjson  | nested      | 0.774    | 0.783       | 0.796    | 283.1    |
| msgspec | nested      | 0.781    | 0.790       | 0.802    | 283.1    |
| strata  | nested      | 0.753    | 0.887       | 0.941    | 283.1    |
| ujson   | nested      | 3.956    | 3.964       | 3.967    | 283.1    |
| json    | nested      | 5.212    | 5.241       | 5.310    | 283.1    |
| strata  | users.json  | 6.698    | 6.719       | 6.759    | 109.1    |
| msgspec | users.json  | 10.893   | 10.899      | 10.951   | 159.8    |
| orjson  | users.json  | 11.278   | 11.484      | 12.148   | 131.4    |
| ujson   | users.json  | 41.352   | 41.359      | 41.382   | 165.6    |
| json    | users.json  | 59.423   | 59.552      | 59.682   | 190.9    |
| strata  | wide_arrays | 5.493    | 5.556       | 5.635    | 307.6    |
| msgspec | wide_arrays | 6.889    | 6.905       | 6.971    | 317.7    |
| orjson  | wide_arrays | 7.655    | 7.676       | 8.478    | 298.0    |
| ujson   | wide_arrays | 28.401   | 28.796      | 29.426   | 319.7    |
| json    | wide_arrays | 80.930   | 81.004      | 81.228   | 340.6    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.245    | 1.277       | 1.363    | 280.7    |
| msgspec       | flat        | 1.606    | 1.647       | 1.659    | 280.7    |
| orjson        | flat        | 1.423    | 1.824       | 2.176    | 280.7    |
| ujson         | flat        | 5.311    | 5.375       | 5.889    | 280.7    |
| json (stdlib) | flat        | 32.952   | 33.388      | 37.169   | 280.7    |
| strata        | mixed       | 1.307    | 1.315       | 1.377    | 303.7    |
| orjson        | mixed       | 1.479    | 1.518       | 1.765    | 303.7    |
| msgspec       | mixed       | 1.785    | 1.800       | 1.880    | 303.7    |
| ujson         | mixed       | 5.680    | 5.978       | 6.107    | 303.7    |
| json (stdlib) | mixed       | 39.633   | 39.941      | 40.192   | 303.7    |
| orjson        | nested      | 0.982    | 0.994       | 1.074    | 283.1    |
| strata        | nested      | 0.996    | 0.998       | 1.184    | 283.1    |
| msgspec       | nested      | 1.019    | 1.029       | 1.095    | 283.1    |
| ujson         | nested      | 4.183    | 4.191       | 4.210    | 283.1    |
| json (stdlib) | nested      | 47.276   | 47.349      | 47.499   | 283.1    |
| strata        | users.json  | 8.136    | 9.719       | 9.757    | 197.2    |
| orjson        | users.json  | 11.734   | 11.922      | 12.360   | 219.5    |
| msgspec       | users.json  | 14.155   | 15.611      | 17.445   | 243.5    |
| ujson         | users.json  | 43.265   | 43.918      | 44.734   | 221.2    |
| json (stdlib) | users.json  | 437.298  | 440.131     | 444.940  | 243.6    |
| strata        | wide_arrays | 6.624    | 7.812       | 7.963    | 340.6    |
| msgspec       | wide_arrays | 8.400    | 8.922       | 10.081   | 360.0    |
| orjson        | wide_arrays | 8.904    | 9.140       | 9.270    | 338.3    |
| ujson         | wide_arrays | 30.615   | 31.137      | 31.179   | 340.3    |
| json (stdlib) | wide_arrays | 179.977  | 182.332     | 197.471  | 301.4    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 10.814   | 2000    | 241.6    |
|                                | orjson+jsonpath-ng | 34.103   | 2000    | 246.0    |
|                                | orjson+jmespath    | 34.540   | 2000    | 243.8    |
| all item prices                | strata             | 12.380   | 100999  | 247.1    |
|                                | orjson+jmespath    | 81.771   | 2000    | 256.4    |
|                                | orjson+jsonpath-ng | 260.980  | 100999  | 262.8    |
| recursive price                | strata             | 12.162   | 100999  | 261.3    |
|                                | orjson+jsonpath-ng | 891.921  | 100999  | 265.6    |
| flat:$.records\[\*\].id        | strata             | 1.476    | 2000    | 280.7    |
|                                | orjson+jmespath    | 4.638    | 2000    | 280.7    |
|                                | orjson+jsonpath-ng | 7.169    | 2000    | 280.7    |
| flat:$.records\[0\]            | strata             | 1.382    | 1       | 280.7    |
|                                | orjson+jmespath    | 3.975    | 1       | 280.7    |
|                                | orjson+jsonpath-ng | 4.048    | 1       | 280.7    |
| nested:$.records\[\*\].id      | strata             | 1.103    | 2000    | 283.1    |
|                                | orjson+jmespath    | 5.554    | 2000    | 283.1    |
|                                | orjson+jsonpath-ng | 7.822    | 2000    | 283.1    |
| nested:$.records\[0\]          | strata             | 1.137    | 1       | 283.1    |
|                                | orjson+jsonpath-ng | 4.950    | 1       | 283.1    |
|                                | orjson+jmespath    | 5.277    | 1       | 283.1    |
| wide_arrays:$.records\[\*\].id | strata             | 10.687   | 2000    | 301.4    |
|                                | orjson+jmespath    | 15.728   | 2000    | 301.4    |
|                                | orjson+jsonpath-ng | 18.340   | 2000    | 301.4    |
| wide_arrays:$.records\[0\]     | strata             | 10.803   | 1       | 301.4    |
|                                | orjson+jsonpath-ng | 14.741   | 1       | 301.4    |
|                                | orjson+jmespath    | 15.121   | 1       | 301.4    |
| mixed:$.records\[\*\].id       | strata             | 1.754    | 2000    | 303.7    |
|                                | orjson+jmespath    | 5.240    | 2000    | 303.7    |
|                                | orjson+jsonpath-ng | 7.934    | 2000    | 303.7    |
| mixed:$.records\[0\]           | strata             | 1.828    | 1       | 303.7    |
|                                | orjson+jsonpath-ng | 4.871    | 1       | 303.7    |
|                                | orjson+jmespath    | 7.268    | 1       | 303.7    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.275    | 2000    | 287.8    |
|                                            | jmespath    | 0.889    | 2000    | 287.8    |
|                                            | jsonpath-ng | 15.607   | 2000    | 288.6    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.113    | 100999  | 288.6    |
|                                            | jmespath    | 61.445   | 2000    | 292.4    |
|                                            | jsonpath-ng | 258.109  | 100999  | 294.8    |
| $..price                                   | strata      | 8.191    | 100999  | 294.5    |
|                                            | jsonpath-ng | 894.339  | 100999  | 278.0    |
| $.users\[?(@.age>30)\]                     | strata      | 0.348    | 1606    | 275.0    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.317    | 6650    | 275.0    |
| flat:$.records\[\*\].id                    | strata      | 0.122    | 2000    | 280.7    |
|                                            | jmespath    | 0.664    | 2000    | 280.7    |
|                                            | jsonpath-ng | 5.017    | 2000    | 280.7    |
| flat:$.records\[0\]                        | strata      | 0.005    | 1       | 280.7    |
|                                            | jmespath    | 0.006    | 1       | 280.7    |
|                                            | jsonpath-ng | 0.008    | 1       | 280.7    |
| nested:$.records\[\*\].id                  | strata      | 0.109    | 2000    | 283.1    |
|                                            | jmespath    | 0.703    | 2000    | 283.1    |
|                                            | jsonpath-ng | 5.299    | 2000    | 283.1    |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 283.1    |
|                                            | jmespath    | 0.006    | 1       | 283.1    |
|                                            | jsonpath-ng | 0.009    | 1       | 283.1    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.110    | 2000    | 301.4    |
|                                            | jmespath    | 0.737    | 2000    | 301.4    |
|                                            | jsonpath-ng | 6.927    | 2000    | 301.4    |
| wide_arrays:$.records\[0\]                 | strata      | 0.008    | 1       | 301.4    |
|                                            | jmespath    | 0.009    | 1       | 301.4    |
|                                            | jsonpath-ng | 0.018    | 1       | 301.4    |
| mixed:$.records\[\*\].id                   | strata      | 0.111    | 2000    | 303.7    |
|                                            | jmespath    | 0.679    | 2000    | 303.7    |
|                                            | jsonpath-ng | 5.356    | 2000    | 303.7    |
| mixed:$.records\[0\]                       | jsonpath-ng | 0.008    | 1       | 303.7    |
|                                            | jmespath    | 0.008    | 1       | 303.7    |
|                                            | strata      | 0.011    | 1       | 303.7    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **22.6% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **45.6% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **67.2% faster** than #2 (msgspec)  |
| load (NDJSON file)  | **#1** / 5              | **40.5% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **62.6% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **44.2% faster** than #2 (orjson)   |
| loads (flat)        | **#1** / 5              | **55.9% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **61.1% faster** than #2 (orjson)   |
| dumps (flat)        | **#1** / 5              | **13.7% faster** than #2 (orjson)   |
| dump (flat)         | **#1** / 5              | **14.3% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **138.3% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **129.0% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **2.7% faster** than #2 (orjson)    |
| dump (nested)       | **#2** / 5              | 1.4% behind #1 (orjson)             |
| loads (wide_arrays) | **#2** / 5              | 34.6% behind #1 (orjson)            |
| load (wide_arrays)  | **#2** / 5              | 10.1% behind #1 (orjson)            |
| dumps (wide_arrays) | **#1** / 5              | **25.4% faster** than #2 (msgspec)  |
| dump (wide_arrays)  | **#1** / 5              | **26.8% faster** than #2 (msgspec)  |
| loads (mixed)       | **#1** / 5              | **72.1% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **66.8% faster** than #2 (msgspec)  |
| dumps (mixed)       | **#1** / 5              | **3.9% faster** than #2 (orjson)    |
| dump (mixed)        | **#1** / 5              | **13.2% faster** than #2 (orjson)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **873% faster**                 |
| query (JSONPath)    | **#1** in 12/13 queries | -                                   |
