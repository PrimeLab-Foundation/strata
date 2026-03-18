# Strata Benchmark Results

Generated: 2026-03-18 19:16:43

## Environment

- Commit: 23a4c6bba7eea184d34febf8fc1d36f00a2d9935
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
| strata        | flat         | 3.206    | 3.604       | 3.842    | 226.2    |
| msgspec       | flat         | 4.232    | 5.072       | 5.728    | 228.7    |
| orjson        | flat         | 6.255    | 7.205       | 15.645   | 228.7    |
| ujson         | flat         | 7.478    | 8.034       | 8.489    | 228.7    |
| json (stdlib) | flat         | 13.253   | 18.257      | 21.094   | 228.7    |
| strata        | mixed        | 3.124    | 3.732       | 3.768    | 313.4    |
| msgspec       | mixed        | 5.859    | 5.923       | 6.406    | 245.2    |
| orjson        | mixed        | 6.036    | 7.931       | 11.033   | 313.4    |
| ujson         | mixed        | 7.602    | 9.156       | 11.257   | 245.2    |
| json (stdlib) | mixed        | 13.645   | 15.659      | 15.851   | 245.2    |
| strata        | nested       | 2.522    | 3.109       | 3.264    | 233.8    |
| orjson        | nested       | 5.283    | 6.778       | 7.771    | 233.8    |
| msgspec       | nested       | 5.350    | 7.586       | 10.123   | 233.8    |
| ujson         | nested       | 6.379    | 8.019       | 18.513   | 233.8    |
| json (stdlib) | nested       | 9.470    | 11.101      | 11.478   | 233.8    |
| strata        | users.json   | 27.410   | 39.983      | 43.127   | 73.6     |
| msgspec       | users.json   | 57.061   | 58.360      | 58.747   | 92.3     |
| orjson        | users.json   | 35.300   | 67.149      | 70.439   | 93.5     |
| ujson         | users.json   | 52.552   | 81.918      | 92.036   | 110.0    |
| json (stdlib) | users.json   | 70.628   | 86.428      | 87.065   | 98.6     |
| strata        | users.ndjson | 25.306   | 33.587      | 34.248   | 254.7    |
| orjson        | users.ndjson | 40.778   | 42.947      | 43.047   | 261.7    |
| msgspec       | users.ndjson | 44.694   | 46.340      | 50.504   | 262.8    |
| ujson         | users.ndjson | 58.429   | 64.516      | 66.658   | 261.8    |
| json (stdlib) | users.ndjson | 79.885   | 88.528      | 92.304   | 263.8    |
| strata        | wide_arrays  | 12.748   | 16.176      | 17.249   | 246.0    |
| orjson        | wide_arrays  | 14.163   | 19.708      | 24.465   | 256.2    |
| msgspec       | wide_arrays  | 19.507   | 23.453      | 28.682   | 256.3    |
| ujson         | wide_arrays  | 33.472   | 33.764      | 37.240   | 256.3    |
| json (stdlib) | wide_arrays  | 64.912   | 69.188      | 71.995   | 256.3    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 4.794    | 5.396       | 5.587    | 228.7    |
| orjson        | flat         | 7.326    | 8.302       | 12.265   | 228.7    |
| msgspec       | flat         | 7.506    | 10.592      | 13.971   | 230.0    |
| ujson         | flat         | 11.982   | 13.993      | 21.639   | 230.0    |
| json (stdlib) | flat         | 18.071   | 19.386      | 21.670   | 230.4    |
| strata        | mixed        | 3.494    | 4.149       | 4.347    | 245.2    |
| orjson        | mixed        | 5.368    | 6.289       | 6.910    | 245.2    |
| ujson         | mixed        | 7.239    | 8.799       | 8.846    | 245.2    |
| msgspec       | mixed        | 7.023    | 9.631       | 9.697    | 245.2    |
| json (stdlib) | mixed        | 13.139   | 13.765      | 14.420   | 245.2    |
| strata        | nested       | 2.754    | 3.297       | 3.339    | 233.8    |
| orjson        | nested       | 5.016    | 7.080       | 13.680   | 233.8    |
| msgspec       | nested       | 7.465    | 8.142       | 8.877    | 233.8    |
| ujson         | nested       | 6.954    | 10.149      | 10.626   | 233.8    |
| json (stdlib) | nested       | 9.606    | 11.411      | 12.645   | 233.8    |
| strata        | users.json   | 29.998   | 36.001      | 39.379   | 100.0    |
| orjson        | users.json   | 38.366   | 50.999      | 65.168   | 100.0    |
| msgspec       | users.json   | 37.372   | 62.076      | 78.981   | 104.9    |
| ujson         | users.json   | 51.023   | 75.256      | 103.469  | 122.6    |
| json (stdlib) | users.json   | 69.084   | 81.429      | 85.109   | 104.9    |
| strata        | users.ndjson | 27.831   | 37.100      | 38.795   | 265.8    |
| orjson        | users.ndjson | 43.405   | 49.304      | 50.525   | 267.9    |
| msgspec       | users.ndjson | 46.750   | 51.644      | 56.434   | 267.9    |
| ujson         | users.ndjson | 62.168   | 67.832      | 74.992   | 266.9    |
| json (stdlib) | users.ndjson | 79.083   | 79.692      | 81.584   | 268.9    |
| strata        | wide_arrays  | 12.296   | 16.669      | 16.982   | 256.3    |
| orjson        | wide_arrays  | 20.063   | 20.458      | 21.336   | 256.0    |
| msgspec       | wide_arrays  | 18.969   | 22.655      | 23.157   | 256.0    |
| ujson         | wide_arrays  | 31.014   | 38.363      | 42.615   | 256.0    |
| json (stdlib) | wide_arrays  | 61.676   | 63.068      | 63.308   | 256.0    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 1.125    | 1.128       | 1.128    | 230.4    |
| orjson  | flat        | 1.147    | 1.319       | 4.258    | 232.7    |
| msgspec | flat        | 1.417    | 3.043       | 3.046    | 232.8    |
| ujson   | flat        | 5.510    | 5.694       | 7.409    | 232.8    |
| json    | flat        | 12.266   | 13.620      | 13.703   | 232.8    |
| strata  | mixed       | 1.198    | 1.210       | 1.214    | 245.2    |
| orjson  | mixed       | 1.225    | 1.248       | 1.261    | 245.2    |
| msgspec | mixed       | 1.326    | 1.346       | 1.491    | 245.2    |
| ujson   | mixed       | 5.487    | 5.724       | 6.161    | 245.2    |
| json    | mixed       | 12.436   | 13.022      | 13.233   | 245.2    |
| strata  | nested      | 0.818    | 0.865       | 0.880    | 233.8    |
| msgspec | nested      | 0.866    | 0.965       | 0.987    | 233.8    |
| orjson  | nested      | 0.959    | 0.971       | 1.187    | 233.8    |
| ujson   | nested      | 4.107    | 4.454       | 4.526    | 233.8    |
| json    | nested      | 5.936    | 5.952       | 6.105    | 233.8    |
| strata  | users.json  | 7.680    | 7.767       | 8.662    | 108.9    |
| orjson  | users.json  | 11.465   | 12.073      | 12.825   | 132.3    |
| msgspec | users.json  | 12.487   | 12.632      | 13.089   | 161.4    |
| ujson   | users.json  | 47.331   | 47.593      | 49.280   | 167.5    |
| json    | users.json  | 68.716   | 68.891      | 70.842   | 192.8    |
| strata  | wide_arrays | 6.714    | 6.967       | 7.145    | 260.9    |
| msgspec | wide_arrays | 7.805    | 8.158       | 8.571    | 271.0    |
| orjson  | wide_arrays | 8.353    | 8.680       | 8.923    | 251.3    |
| ujson   | wide_arrays | 31.629   | 32.541      | 33.050   | 273.0    |
| json    | wide_arrays | 92.023   | 93.208      | 93.226   | 293.9    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.485    | 1.510       | 1.885    | 232.8    |
| orjson        | flat        | 1.612    | 1.655       | 1.734    | 232.8    |
| msgspec       | flat        | 2.070    | 2.109       | 2.765    | 233.8    |
| ujson         | flat        | 6.253    | 7.391       | 7.473    | 233.8    |
| json (stdlib) | flat        | 36.714   | 37.819      | 38.038   | 233.8    |
| strata        | mixed       | 1.548    | 1.970       | 2.166    | 245.2    |
| msgspec       | mixed       | 1.794    | 1.986       | 2.133    | 245.2    |
| orjson        | mixed       | 1.949    | 2.000       | 2.137    | 245.2    |
| ujson         | mixed       | 6.693    | 6.744       | 6.806    | 245.2    |
| json (stdlib) | mixed       | 45.267   | 46.486      | 47.035   | 245.2    |
| orjson        | nested      | 1.131    | 1.139       | 1.151    | 233.8    |
| msgspec       | nested      | 1.259    | 1.301       | 1.610    | 233.8    |
| strata        | nested      | 1.135    | 1.337       | 1.927    | 233.8    |
| ujson         | nested      | 4.869    | 5.035       | 5.479    | 233.8    |
| json (stdlib) | nested      | 53.124   | 57.249      | 88.231   | 233.8    |
| strata        | users.json  | 8.563    | 9.193       | 11.104   | 199.0    |
| msgspec       | users.json  | 13.561   | 13.717      | 14.942   | 243.8    |
| orjson        | users.json  | 14.332   | 14.486      | 15.266   | 221.4    |
| ujson         | users.json  | 50.989   | 63.068      | 66.541   | 221.5    |
| json (stdlib) | users.json  | 551.020  | 571.690     | 583.959  | 243.9    |
| strata        | wide_arrays | 8.013    | 8.208       | 9.540    | 293.9    |
| msgspec       | wide_arrays | 8.863    | 9.397       | 9.573    | 313.3    |
| orjson        | wide_arrays | 9.871    | 10.115      | 10.203   | 291.6    |
| ujson         | wide_arrays | 34.399   | 35.993      | 36.201   | 293.6    |
| json (stdlib) | wide_arrays | 201.950  | 202.764     | 204.215  | 313.3    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 13.807   | 2000    | 240.9    |
|                                | orjson+jmespath    | 35.163   | 2000    | 244.0    |
|                                | orjson+jsonpath-ng | 42.836   | 2000    | 246.3    |
| all item prices                | strata             | 14.284   | 100999  | 248.0    |
|                                | orjson+jmespath    | 88.978   | 2000    | 257.2    |
|                                | orjson+jsonpath-ng | 332.215  | 100999  | 264.4    |
| recursive price                | strata             | 18.149   | 100999  | 262.3    |
|                                | orjson+jsonpath-ng | 1073.847 | 100999  | 265.7    |
| flat:$.records\[\*\].id        | strata             | 1.921    | 2000    | 233.8    |
|                                | orjson+jmespath    | 6.611    | 2000    | 233.8    |
|                                | orjson+jsonpath-ng | 7.918    | 2000    | 233.8    |
| flat:$.records\[0\]            | strata             | 2.084    | 1       | 233.8    |
|                                | orjson+jmespath    | 4.457    | 1       | 233.8    |
|                                | orjson+jsonpath-ng | 4.567    | 1       | 233.8    |
| nested:$.records\[\*\].id      | strata             | 1.772    | 2000    | 233.8    |
|                                | orjson+jmespath    | 6.804    | 2000    | 233.8    |
|                                | orjson+jsonpath-ng | 8.568    | 2000    | 233.8    |
| nested:$.records\[0\]          | strata             | 1.665    | 1       | 233.8    |
|                                | orjson+jmespath    | 6.168    | 1       | 233.8    |
|                                | orjson+jsonpath-ng | 6.254    | 1       | 233.8    |
| wide_arrays:$.records\[\*\].id | strata             | 9.611    | 2000    | 313.3    |
|                                | orjson+jmespath    | 17.280   | 2000    | 313.3    |
|                                | orjson+jsonpath-ng | 21.552   | 2000    | 313.3    |
| wide_arrays:$.records\[0\]     | strata             | 10.110   | 1       | 313.4    |
|                                | orjson+jsonpath-ng | 18.318   | 1       | 313.4    |
|                                | orjson+jmespath    | 21.461   | 1       | 313.4    |
| mixed:$.records\[\*\].id       | strata             | 1.972    | 2000    | 245.2    |
|                                | orjson+jmespath    | 8.630    | 2000    | 245.2    |
|                                | orjson+jsonpath-ng | 9.288    | 2000    | 245.2    |
| mixed:$.records\[0\]           | strata             | 2.086    | 1       | 245.2    |
|                                | orjson+jsonpath-ng | 5.419    | 1       | 245.2    |
|                                | orjson+jmespath    | 5.831    | 1       | 245.2    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.335    | 2000    | 287.9    |
|                                            | jmespath    | 1.098    | 2000    | 287.9    |
|                                            | jsonpath-ng | 17.729   | 2000    | 288.7    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.979    | 100999  | 288.7    |
|                                            | jmespath    | 69.614   | 2000    | 293.5    |
|                                            | jsonpath-ng | 321.997  | 100999  | 274.0    |
| $..price                                   | strata      | 9.066    | 100999  | 273.6    |
|                                            | jsonpath-ng | 1005.861 | 100999  | 278.0    |
| $.users\[?(@.age>30)\]                     | strata      | 0.368    | 1606    | 275.0    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.812    | 6650    | 275.1    |
| flat:$.records\[\*\].id                    | strata      | 0.139    | 2000    | 233.8    |
|                                            | jmespath    | 0.750    | 2000    | 233.8    |
|                                            | jsonpath-ng | 5.246    | 2000    | 233.8    |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 233.8    |
|                                            | jmespath    | 0.009    | 1       | 233.8    |
|                                            | jsonpath-ng | 0.010    | 1       | 233.8    |
| nested:$.records\[\*\].id                  | strata      | 0.117    | 2000    | 233.8    |
|                                            | jmespath    | 0.780    | 2000    | 233.8    |
|                                            | jsonpath-ng | 7.052    | 2000    | 233.8    |
| nested:$.records\[0\]                      | strata      | 0.008    | 1       | 233.8    |
|                                            | jsonpath-ng | 0.009    | 1       | 233.8    |
|                                            | jmespath    | 0.016    | 1       | 233.8    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.140    | 2000    | 313.4    |
|                                            | jmespath    | 0.852    | 2000    | 313.4    |
|                                            | jsonpath-ng | 7.979    | 2000    | 313.4    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 313.4    |
|                                            | jmespath    | 0.012    | 1       | 313.4    |
|                                            | jsonpath-ng | 0.021    | 1       | 313.4    |
| mixed:$.records\[\*\].id                   | strata      | 0.146    | 2000    | 245.2    |
|                                            | jmespath    | 0.780    | 2000    | 245.2    |
|                                            | jsonpath-ng | 5.693    | 2000    | 245.2    |
| mixed:$.records\[0\]                       | jmespath    | 0.006    | 1       | 245.2    |
|                                            | strata      | 0.008    | 1       | 245.2    |
|                                            | jsonpath-ng | 0.011    | 1       | 245.2    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **28.8% faster** than #2 (orjson)  |
| loads (NDJSON)      | **#1** / 5              | **61.1% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **24.6% faster** than #2 (msgspec) |
| load (NDJSON file)  | **#1** / 5              | **56.0% faster** than #2 (orjson)  |
| dumps               | **#1** / 5              | **49.3% faster** than #2 (orjson)  |
| dump                | **#1** / 5              | **58.4% faster** than #2 (msgspec) |
| loads (flat)        | **#1** / 5              | **32.0% faster** than #2 (msgspec) |
| load (flat)         | **#1** / 5              | **52.8% faster** than #2 (orjson)  |
| dumps (flat)        | **#1** / 5              | **2.0% faster** than #2 (orjson)   |
| dump (flat)         | **#1** / 5              | **8.5% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **109.5% faster** than #2 (orjson) |
| load (nested)       | **#1** / 5              | **82.1% faster** than #2 (orjson)  |
| dumps (nested)      | **#1** / 5              | **5.8% faster** than #2 (msgspec)  |
| dump (nested)       | **#2** / 5              | 0.4% behind #1 (orjson)            |
| loads (wide_arrays) | **#1** / 5              | **11.1% faster** than #2 (orjson)  |
| load (wide_arrays)  | **#1** / 5              | **54.3% faster** than #2 (msgspec) |
| dumps (wide_arrays) | **#1** / 5              | **16.2% faster** than #2 (msgspec) |
| dump (wide_arrays)  | **#1** / 5              | **10.6% faster** than #2 (msgspec) |
| loads (mixed)       | **#1** / 5              | **87.5% faster** than #2 (msgspec) |
| load (mixed)        | **#1** / 5              | **53.7% faster** than #2 (orjson)  |
| dumps (mixed)       | **#1** / 5              | **2.3% faster** than #2 (orjson)   |
| dump (mixed)        | **#1** / 5              | **15.9% faster** than #2 (msgspec) |
| search (JSONPath)   | **#1** in 11/11 queries | avg **733% faster**                |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
