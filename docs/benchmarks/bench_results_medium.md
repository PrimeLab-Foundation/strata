# Strata Benchmark Results

Generated: 2026-03-16 02:15:31

## Environment

- Commit: 3f0d7fe886713eb553cbe090544e067cb4f6daf3
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
| strata        | flat         | 2.491    | 2.960       | 3.068    | 271.3    |
| orjson        | flat         | 4.090    | 4.166       | 4.398    | 273.8    |
| msgspec       | flat         | 4.136    | 4.397       | 7.330    | 273.8    |
| ujson         | flat         | 6.204    | 6.329       | 7.317    | 273.8    |
| json (stdlib) | flat         | 9.871    | 10.177      | 10.272   | 273.8    |
| strata        | mixed        | 2.647    | 3.141       | 3.157    | 266.5    |
| orjson        | mixed        | 3.979    | 4.990       | 5.060    | 268.8    |
| msgspec       | mixed        | 4.221    | 5.260       | 8.549    | 268.8    |
| ujson         | mixed        | 6.235    | 6.849       | 12.265   | 268.8    |
| json (stdlib) | mixed        | 10.233   | 10.986      | 11.338   | 268.8    |
| strata        | nested       | 2.210    | 2.730       | 3.031    | 278.1    |
| orjson        | nested       | 4.856    | 5.669       | 7.075    | 280.5    |
| msgspec       | nested       | 4.485    | 6.161       | 9.982    | 280.5    |
| ujson         | nested       | 7.066    | 7.950       | 8.248    | 280.5    |
| json (stdlib) | nested       | 7.729    | 9.063       | 9.874    | 280.5    |
| strata        | users.json   | 23.294   | 30.370      | 32.395   | 73.5     |
| orjson        | users.json   | 33.487   | 43.334      | 48.083   | 93.4     |
| msgspec       | users.json   | 29.855   | 51.055      | 57.897   | 92.2     |
| ujson         | users.json   | 43.871   | 70.545      | 76.328   | 109.9    |
| json (stdlib) | users.json   | 65.016   | 71.925      | 74.316   | 98.5     |
| strata        | users.ndjson | 22.271   | 27.525      | 29.555   | 253.6    |
| orjson        | users.ndjson | 35.218   | 35.400      | 40.321   | 261.6    |
| msgspec       | users.ndjson | 42.784   | 45.122      | 48.268   | 262.8    |
| ujson         | users.ndjson | 44.706   | 54.688      | 62.123   | 261.8    |
| json (stdlib) | users.ndjson | 63.479   | 65.643      | 68.791   | 263.8    |
| strata        | wide_arrays  | 12.226   | 15.087      | 15.214   | 293.1    |
| orjson        | wide_arrays  | 12.428   | 16.397      | 22.541   | 303.4    |
| msgspec       | wide_arrays  | 18.435   | 18.575      | 19.489   | 303.4    |
| ujson         | wide_arrays  | 27.557   | 28.608      | 28.704   | 303.4    |
| json (stdlib) | wide_arrays  | 50.110   | 54.276      | 59.717   | 303.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 2.766    | 3.142       | 3.173    | 273.8    |
| orjson        | flat         | 4.119    | 5.011       | 5.142    | 273.8    |
| msgspec       | flat         | 4.949    | 6.935       | 7.783    | 273.9    |
| ujson         | flat         | 6.474    | 7.123       | 7.197    | 273.9    |
| json (stdlib) | flat         | 10.026   | 10.180      | 10.352   | 273.9    |
| strata        | mixed        | 2.934    | 3.317       | 3.868    | 268.8    |
| msgspec       | mixed        | 5.020    | 5.710       | 5.937    | 268.8    |
| orjson        | mixed        | 5.970    | 6.237       | 8.422    | 268.8    |
| ujson         | mixed        | 8.080    | 9.615       | 10.801   | 268.8    |
| json (stdlib) | mixed        | 11.982   | 12.063      | 13.558   | 268.8    |
| strata        | nested       | 2.347    | 3.019       | 3.104    | 280.5    |
| orjson        | nested       | 5.256    | 6.374       | 12.532   | 280.5    |
| msgspec       | nested       | 6.272    | 6.958       | 8.298    | 280.5    |
| ujson         | nested       | 5.791    | 7.430       | 8.008    | 280.5    |
| json (stdlib) | nested       | 8.012    | 12.807      | 14.806   | 280.5    |
| strata        | users.json   | 22.766   | 28.762      | 29.316   | 99.9     |
| msgspec       | users.json   | 28.256   | 42.300      | 49.804   | 104.8    |
| orjson        | users.json   | 26.085   | 43.814      | 50.469   | 99.9     |
| ujson         | users.json   | 41.474   | 62.357      | 63.581   | 122.5    |
| json (stdlib) | users.json   | 57.394   | 69.073      | 85.768   | 104.8    |
| strata        | users.ndjson | 24.260   | 28.522      | 31.041   | 265.8    |
| orjson        | users.ndjson | 34.002   | 36.475      | 38.281   | 266.8    |
| msgspec       | users.ndjson | 37.483   | 40.245      | 42.798   | 267.8    |
| ujson         | users.ndjson | 54.548   | 54.633      | 57.120   | 266.8    |
| json (stdlib) | users.ndjson | 61.867   | 62.558      | 67.552   | 268.8    |
| strata        | wide_arrays  | 12.752   | 15.020      | 15.102   | 303.4    |
| orjson        | wide_arrays  | 14.928   | 15.691      | 15.707   | 303.0    |
| msgspec       | wide_arrays  | 18.837   | 29.846      | 30.400   | 303.0    |
| ujson         | wide_arrays  | 30.211   | 30.877      | 32.893   | 303.0    |
| json (stdlib) | wide_arrays  | 55.438   | 56.224      | 58.094   | 303.0    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.952    | 0.955       | 1.047    | 276.2    |
| strata  | flat        | 1.077    | 1.093       | 1.093    | 273.9    |
| msgspec | flat        | 1.103    | 1.125       | 1.140    | 276.2    |
| ujson   | flat        | 4.726    | 4.851       | 4.858    | 277.8    |
| json    | flat        | 8.732    | 8.809       | 8.827    | 277.8    |
| msgspec | mixed       | 1.123    | 1.171       | 1.187    | 268.8    |
| strata  | mixed       | 1.199    | 1.211       | 1.252    | 268.8    |
| orjson  | mixed       | 1.099    | 1.363       | 1.523    | 268.8    |
| ujson   | mixed       | 5.146    | 5.271       | 5.288    | 268.8    |
| json    | mixed       | 11.191   | 11.276      | 11.414   | 268.8    |
| msgspec | nested      | 0.753    | 0.764       | 1.000    | 280.5    |
| strata  | nested      | 0.824    | 0.833       | 0.840    | 280.5    |
| orjson  | nested      | 0.748    | 0.833       | 0.916    | 280.5    |
| ujson   | nested      | 3.892    | 3.945       | 3.960    | 280.6    |
| json    | nested      | 5.501    | 5.546       | 5.736    | 281.0    |
| strata  | users.json  | 8.164    | 8.406       | 9.734    | 109.1    |
| msgspec | users.json  | 10.356   | 10.648      | 11.777   | 159.7    |
| orjson  | users.json  | 10.766   | 10.937      | 12.343   | 131.4    |
| ujson   | users.json  | 40.523   | 40.732      | 41.054   | 167.3    |
| json    | users.json  | 58.686   | 58.972      | 64.049   | 192.6    |
| orjson  | wide_arrays | 7.051    | 7.310       | 8.066    | 295.8    |
| strata  | wide_arrays | 7.312    | 7.532       | 7.615    | 305.4    |
| msgspec | wide_arrays | 6.890    | 7.544       | 8.244    | 256.9    |
| ujson   | wide_arrays | 28.105   | 28.375      | 28.485   | 258.9    |
| json    | wide_arrays | 78.218   | 78.346      | 78.347   | 255.2    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.376    | 1.400       | 1.438    | 277.8    |
| orjson        | flat        | 1.349    | 1.404       | 2.917    | 277.8    |
| msgspec       | flat        | 1.440    | 1.453       | 1.497    | 278.1    |
| ujson         | flat        | 5.111    | 5.127       | 5.291    | 278.1    |
| json (stdlib) | flat        | 31.872   | 31.923      | 32.003   | 278.1    |
| msgspec       | mixed       | 1.449    | 1.456       | 1.527    | 268.8    |
| orjson        | mixed       | 1.448    | 1.532       | 2.148    | 268.8    |
| strata        | mixed       | 1.769    | 1.819       | 1.956    | 268.8    |
| ujson         | mixed       | 5.494    | 5.538       | 6.042    | 268.8    |
| json (stdlib) | mixed       | 37.720   | 37.875      | 37.892   | 268.8    |
| msgspec       | nested      | 1.419    | 1.457       | 1.484    | 281.0    |
| orjson        | nested      | 1.265    | 1.494       | 1.676    | 281.0    |
| strata        | nested      | 1.233    | 1.516       | 1.583    | 281.0    |
| ujson         | nested      | 4.641    | 4.721       | 4.948    | 281.0    |
| json (stdlib) | nested      | 46.060   | 46.286      | 49.206   | 281.0    |
| strata        | users.json  | 9.602    | 9.678       | 9.763    | 198.9    |
| orjson        | users.json  | 11.808   | 12.467      | 18.389   | 221.2    |
| msgspec       | users.json  | 14.166   | 14.965      | 17.188   | 243.9    |
| ujson         | users.json  | 46.752   | 47.093      | 47.884   | 221.6    |
| json (stdlib) | users.json  | 432.484  | 434.063     | 435.351  | 244.0    |
| strata        | wide_arrays | 8.283    | 8.701       | 8.858    | 255.2    |
| msgspec       | wide_arrays | 8.532    | 8.798       | 10.469   | 266.5    |
| orjson        | wide_arrays | 8.746    | 9.086       | 10.797   | 260.8    |
| ujson         | wide_arrays | 30.582   | 31.091      | 32.001   | 266.4    |
| json (stdlib) | wide_arrays | 180.866  | 180.963     | 182.038  | 266.5    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 12.109   | 2000    | 241.0    |
|                                | orjson+jmespath    | 32.474   | 2000    | 244.1    |
|                                | orjson+jsonpath-ng | 33.922   | 2000    | 246.3    |
| all item prices                | strata             | 12.930   | 100999  | 247.4    |
|                                | orjson+jmespath    | 77.642   | 2000    | 256.6    |
|                                | orjson+jsonpath-ng | 264.072  | 100999  | 263.5    |
| recursive price                | strata             | 12.668   | 100999  | 261.3    |
|                                | orjson+jsonpath-ng | 903.661  | 100999  | 265.5    |
| flat:$.records\[\*\].id        | strata             | 1.863    | 2000    | 278.1    |
|                                | orjson+jmespath    | 4.801    | 2000    | 278.1    |
|                                | orjson+jsonpath-ng | 7.126    | 2000    | 278.1    |
| flat:$.records\[0\]            | strata             | 1.756    | 1       | 278.1    |
|                                | orjson+jsonpath-ng | 3.893    | 1       | 278.1    |
|                                | orjson+jmespath    | 4.032    | 1       | 278.1    |
| nested:$.records\[\*\].id      | strata             | 1.429    | 2000    | 281.0    |
|                                | orjson+jmespath    | 5.310    | 2000    | 281.0    |
|                                | orjson+jsonpath-ng | 7.803    | 2000    | 281.0    |
| nested:$.records\[0\]          | strata             | 1.362    | 1       | 281.0    |
|                                | orjson+jmespath    | 4.745    | 1       | 281.0    |
|                                | orjson+jsonpath-ng | 4.879    | 1       | 281.0    |
| wide_arrays:$.records\[\*\].id | strata             | 10.619   | 2000    | 266.5    |
|                                | orjson+jmespath    | 16.755   | 2000    | 266.5    |
|                                | orjson+jsonpath-ng | 18.152   | 2000    | 266.5    |
| wide_arrays:$.records\[0\]     | strata             | 10.550   | 1       | 266.5    |
|                                | orjson+jsonpath-ng | 14.547   | 1       | 266.5    |
|                                | orjson+jmespath    | 18.438   | 1       | 266.5    |
| mixed:$.records\[\*\].id       | strata             | 1.842    | 2000    | 268.8    |
|                                | orjson+jmespath    | 5.726    | 2000    | 268.8    |
|                                | orjson+jsonpath-ng | 7.293    | 2000    | 268.8    |
| mixed:$.records\[0\]           | strata             | 1.876    | 1       | 268.8    |
|                                | orjson+jmespath    | 4.575    | 1       | 268.8    |
|                                | orjson+jsonpath-ng | 5.304    | 1       | 268.8    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.327    | 2000    | 287.7    |
|                                            | jmespath    | 1.066    | 2000    | 287.7    |
|                                            | jsonpath-ng | 16.383   | 2000    | 288.5    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.339    | 100999  | 288.5    |
|                                            | jmespath    | 60.424   | 2000    | 292.3    |
|                                            | jsonpath-ng | 248.766  | 100999  | 294.7    |
| $..price                                   | strata      | 8.167    | 100999  | 294.3    |
|                                            | jsonpath-ng | 896.484  | 100999  | 276.9    |
| $.users\[?(@.age>30)\]                     | strata      | 0.424    | 1606    | 274.9    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.267    | 6650    | 275.0    |
| flat:$.records\[\*\].id                    | strata      | 0.149    | 2000    | 278.1    |
|                                            | jmespath    | 0.659    | 2000    | 278.1    |
|                                            | jsonpath-ng | 4.056    | 2000    | 278.1    |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 278.1    |
|                                            | jmespath    | 0.008    | 1       | 278.1    |
|                                            | jsonpath-ng | 0.011    | 1       | 278.1    |
| nested:$.records\[\*\].id                  | strata      | 0.106    | 2000    | 281.0    |
|                                            | jmespath    | 0.657    | 2000    | 281.0    |
|                                            | jsonpath-ng | 5.155    | 2000    | 281.0    |
| nested:$.records\[0\]                      | jmespath    | 0.008    | 1       | 281.0    |
|                                            | jsonpath-ng | 0.009    | 1       | 281.0    |
|                                            | strata      | 0.009    | 1       | 281.0    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.108    | 2000    | 266.5    |
|                                            | jmespath    | 0.661    | 2000    | 266.5    |
|                                            | jsonpath-ng | 7.033    | 2000    | 266.5    |
| wide_arrays:$.records\[0\]                 | strata      | 0.006    | 1       | 266.5    |
|                                            | jmespath    | 0.010    | 1       | 266.5    |
|                                            | jsonpath-ng | 0.010    | 1       | 266.5    |
| mixed:$.records\[\*\].id                   | strata      | 0.108    | 2000    | 268.8    |
|                                            | jmespath    | 0.708    | 2000    | 268.8    |
|                                            | jsonpath-ng | 4.826    | 2000    | 268.8    |
| mixed:$.records\[0\]                       | strata      | 0.006    | 1       | 268.8    |
|                                            | jsonpath-ng | 0.008    | 1       | 268.8    |
|                                            | jmespath    | 0.009    | 1       | 268.8    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **28.2% faster** than #2 (msgspec)  |
| loads (NDJSON)      | **#1** / 5              | **58.1% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **14.6% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **40.2% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **26.8% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **23.0% faster** than #2 (orjson)   |
| loads (flat)        | **#1** / 5              | **64.2% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **48.9% faster** than #2 (orjson)   |
| dumps (flat)        | **#2** / 5              | 13.2% behind #1 (orjson)            |
| dump (flat)         | **#2** / 5              | 2.0% behind #1 (orjson)             |
| loads (nested)      | **#1** / 5              | **102.9% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **123.9% faster** than #2 (orjson)  |
| dumps (nested)      | **#3** / 5              | 10.2% behind #1 (orjson)            |
| dump (nested)       | **#1** / 5              | **2.6% faster** than #2 (orjson)    |
| loads (wide_arrays) | **#1** / 5              | **1.6% faster** than #2 (orjson)    |
| load (wide_arrays)  | **#1** / 5              | **17.1% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#3** / 5              | 6.1% behind #1 (msgspec)            |
| dump (wide_arrays)  | **#1** / 5              | **3.0% faster** than #2 (msgspec)   |
| loads (mixed)       | **#1** / 5              | **50.4% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **71.1% faster** than #2 (msgspec)  |
| dumps (mixed)       | **#3** / 5              | 9.1% behind #1 (orjson)             |
| dump (mixed)        | **#3** / 5              | 22.2% behind #1 (orjson)            |
| search (JSONPath)   | **#1** in 11/11 queries | avg **814% faster**                 |
| query (JSONPath)    | **#1** in 12/13 queries | -                                   |
