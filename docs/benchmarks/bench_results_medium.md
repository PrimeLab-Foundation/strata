# Strata Benchmark Results

Generated: 2026-03-17 15:51:40

## Environment

- Commit: 33d6835349dd084a915ba2ba437233c105a366f8
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
| strata        | flat         | 2.860    | 3.314       | 3.411    | 272.1    |
| msgspec       | flat         | 3.700    | 4.272       | 4.451    | 274.6    |
| orjson        | flat         | 4.459    | 4.852       | 6.764    | 274.6    |
| ujson         | flat         | 7.127    | 7.691       | 8.135    | 274.6    |
| json (stdlib) | flat         | 9.834    | 10.395      | 10.952   | 274.8    |
| strata        | mixed        | 3.044    | 3.733       | 3.743    | 296.9    |
| orjson        | mixed        | 4.780    | 5.151       | 5.675    | 299.2    |
| ujson         | mixed        | 6.541    | 7.831       | 8.483    | 299.2    |
| msgspec       | mixed        | 4.577    | 9.063       | 12.322   | 299.2    |
| json (stdlib) | mixed        | 11.875   | 12.236      | 12.497   | 299.2    |
| strata        | nested       | 2.430    | 2.978       | 3.002    | 280.1    |
| orjson        | nested       | 4.662    | 6.477       | 13.590   | 282.5    |
| msgspec       | nested       | 5.055    | 6.966       | 13.735   | 282.6    |
| ujson         | nested       | 7.381    | 8.070       | 8.201    | 282.6    |
| json (stdlib) | nested       | 9.596    | 10.812      | 12.972   | 282.6    |
| strata        | users.json   | 25.567   | 32.638      | 36.128   | 73.3     |
| orjson        | users.json   | 28.548   | 43.614      | 43.857   | 93.2     |
| msgspec       | users.json   | 32.424   | 45.395      | 62.931   | 92.1     |
| ujson         | users.json   | 42.265   | 62.630      | 76.054   | 109.7    |
| json (stdlib) | users.json   | 61.219   | 71.957      | 72.566   | 98.3     |
| strata        | users.ndjson | 25.784   | 32.089      | 33.190   | 255.4    |
| msgspec       | users.ndjson | 37.859   | 38.303      | 39.986   | 263.5    |
| orjson        | users.ndjson | 34.880   | 38.823      | 40.048   | 262.4    |
| ujson         | users.ndjson | 46.205   | 54.284      | 58.190   | 262.5    |
| json (stdlib) | users.ndjson | 64.707   | 68.743      | 69.629   | 264.5    |
| orjson        | wide_arrays  | 12.907   | 15.940      | 16.930   | 305.0    |
| strata        | wide_arrays  | 13.582   | 16.294      | 17.390   | 294.8    |
| msgspec       | wide_arrays  | 15.949   | 19.429      | 19.899   | 308.5    |
| ujson         | wide_arrays  | 26.346   | 32.102      | 34.279   | 308.5    |
| json (stdlib) | wide_arrays  | 52.982   | 55.982      | 63.855   | 308.5    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 3.023    | 3.441       | 3.646    | 274.8    |
| msgspec       | flat         | 4.343    | 4.702       | 5.040    | 275.4    |
| orjson        | flat         | 5.285    | 5.319       | 6.859    | 274.8    |
| ujson         | flat         | 6.257    | 7.602       | 7.734    | 275.4    |
| json (stdlib) | flat         | 11.122   | 11.333      | 12.084   | 275.4    |
| strata        | mixed        | 3.250    | 3.866       | 4.321    | 299.2    |
| orjson        | mixed        | 4.199    | 5.929       | 6.271    | 299.2    |
| msgspec       | mixed        | 4.886    | 6.101       | 7.035    | 299.3    |
| ujson         | mixed        | 9.481    | 10.172      | 13.217   | 299.3    |
| json (stdlib) | mixed        | 11.683   | 15.155      | 16.943   | 299.3    |
| strata        | nested       | 2.936    | 3.110       | 3.120    | 282.6    |
| orjson        | nested       | 5.462    | 6.347       | 13.607   | 282.6    |
| msgspec       | nested       | 6.763    | 8.297       | 10.080   | 282.6    |
| ujson         | nested       | 7.864    | 8.449       | 8.549    | 282.6    |
| json (stdlib) | nested       | 9.404    | 16.661      | 17.831   | 282.6    |
| strata        | users.json   | 25.284   | 31.001      | 35.194   | 99.8     |
| orjson        | users.json   | 30.528   | 39.716      | 42.339   | 99.8     |
| msgspec       | users.json   | 38.724   | 43.032      | 51.036   | 104.7    |
| ujson         | users.json   | 45.269   | 63.230      | 69.319   | 122.4    |
| json (stdlib) | users.json   | 58.952   | 75.463      | 101.027  | 104.7    |
| strata        | users.ndjson | 26.890   | 32.819      | 36.017   | 266.5    |
| msgspec       | users.ndjson | 38.176   | 38.356      | 39.877   | 268.5    |
| orjson        | users.ndjson | 35.214   | 39.835      | 47.969   | 268.5    |
| ujson         | users.ndjson | 54.316   | 58.572      | 58.692   | 267.5    |
| json (stdlib) | users.ndjson | 70.337   | 71.184      | 71.370   | 269.5    |
| strata        | wide_arrays  | 14.308   | 17.006      | 18.501   | 246.0    |
| orjson        | wide_arrays  | 12.974   | 19.004      | 22.533   | 245.6    |
| msgspec       | wide_arrays  | 18.746   | 21.987      | 25.913   | 245.6    |
| ujson         | wide_arrays  | 27.472   | 30.680      | 36.300   | 245.6    |
| json (stdlib) | wide_arrays  | 52.577   | 56.369      | 57.201   | 245.6    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.949    | 0.996       | 1.135    | 275.4    |
| orjson  | flat        | 1.014    | 1.081       | 1.110    | 277.7    |
| msgspec | flat        | 1.174    | 1.245       | 1.260    | 277.9    |
| ujson   | flat        | 4.870    | 5.059       | 5.126    | 280.6    |
| json    | flat        | 9.038    | 9.252       | 9.394    | 281.1    |
| strata  | mixed       | 1.040    | 1.079       | 1.704    | 299.3    |
| orjson  | mixed       | 1.099    | 1.161       | 1.394    | 299.3    |
| msgspec | mixed       | 1.187    | 1.224       | 1.266    | 299.3    |
| ujson   | mixed       | 5.342    | 5.409       | 5.515    | 299.4    |
| json    | mixed       | 11.868   | 12.103      | 12.138   | 299.4    |
| orjson  | nested      | 0.787    | 0.812       | 1.153    | 282.6    |
| strata  | nested      | 0.822    | 0.863       | 0.864    | 282.6    |
| msgspec | nested      | 0.903    | 0.929       | 1.018    | 282.6    |
| ujson   | nested      | 4.126    | 4.169       | 4.584    | 282.6    |
| json    | nested      | 5.214    | 5.394       | 5.598    | 282.6    |
| strata  | users.json  | 6.772    | 6.864       | 7.543    | 108.8    |
| msgspec | users.json  | 10.792   | 10.849      | 12.151   | 161.0    |
| orjson  | users.json  | 11.024   | 11.268      | 12.596   | 132.2    |
| ujson   | users.json  | 41.475   | 42.213      | 43.417   | 167.3    |
| json    | users.json  | 59.102   | 59.152      | 59.555   | 192.7    |
| strata  | wide_arrays | 5.497    | 5.894       | 6.238    | 245.6    |
| msgspec | wide_arrays | 6.911    | 6.941       | 6.956    | 255.8    |
| orjson  | wide_arrays | 7.259    | 7.496       | 8.485    | 236.0    |
| ujson   | wide_arrays | 30.673   | 31.145      | 31.768   | 257.8    |
| json    | wide_arrays | 86.847   | 87.888      | 88.113   | 278.6    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 1.283    | 1.363       | 1.394    | 281.1    |
| strata        | flat        | 1.316    | 1.380       | 1.873    | 281.1    |
| msgspec       | flat        | 1.471    | 1.623       | 1.705    | 281.1    |
| ujson         | flat        | 5.522    | 5.557       | 5.579    | 281.1    |
| json (stdlib) | flat        | 32.911   | 33.168      | 33.620   | 281.1    |
| strata        | mixed       | 1.413    | 1.415       | 1.592    | 299.4    |
| msgspec       | mixed       | 1.461    | 1.468       | 1.504    | 299.4    |
| orjson        | mixed       | 1.391    | 1.471       | 1.572    | 299.4    |
| ujson         | mixed       | 5.683    | 5.776       | 6.124    | 299.4    |
| json (stdlib) | mixed       | 39.731   | 40.104      | 43.278   | 299.4    |
| strata        | nested      | 1.070    | 1.153       | 1.160    | 282.6    |
| orjson        | nested      | 1.171    | 1.172       | 1.583    | 282.6    |
| msgspec       | nested      | 1.152    | 1.330       | 1.378    | 282.6    |
| ujson         | nested      | 4.157    | 4.225       | 4.411    | 282.6    |
| json (stdlib) | nested      | 47.999   | 48.437      | 48.442   | 282.6    |
| strata        | users.json  | 7.438    | 8.587       | 8.716    | 198.9    |
| msgspec       | users.json  | 12.102   | 12.232      | 13.010   | 243.7    |
| orjson        | users.json  | 11.819   | 12.377      | 12.770   | 221.2    |
| ujson         | users.json  | 43.273   | 43.469      | 45.230   | 221.3    |
| json (stdlib) | users.json  | 442.821  | 444.256     | 455.448  | 243.7    |
| strata        | wide_arrays | 6.832    | 7.511       | 8.506    | 278.6    |
| msgspec       | wide_arrays | 7.973    | 8.323       | 9.298    | 296.9    |
| orjson        | wide_arrays | 8.564    | 8.815       | 8.935    | 276.4    |
| ujson         | wide_arrays | 30.806   | 31.316      | 32.638   | 278.3    |
| json (stdlib) | wide_arrays | 183.986  | 189.493     | 197.199  | 296.9    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 12.628   | 2000    | 240.8    |
|                                | orjson+jmespath    | 32.778   | 2000    | 243.9    |
|                                | orjson+jsonpath-ng | 34.636   | 2000    | 246.2    |
| all item prices                | strata             | 14.305   | 100999  | 247.0    |
|                                | orjson+jmespath    | 87.088   | 2000    | 256.2    |
|                                | orjson+jsonpath-ng | 278.398  | 100999  | 263.6    |
| recursive price                | strata             | 13.867   | 100999  | 261.3    |
|                                | orjson+jsonpath-ng | 923.656  | 100999  | 265.5    |
| flat:$.records\[\*\].id        | strata             | 1.913    | 2000    | 281.1    |
|                                | orjson+jmespath    | 5.095    | 2000    | 281.1    |
|                                | orjson+jsonpath-ng | 7.207    | 2000    | 280.1    |
| flat:$.records\[0\]            | strata             | 1.988    | 1       | 280.1    |
|                                | orjson+jsonpath-ng | 4.275    | 1       | 280.1    |
|                                | orjson+jmespath    | 4.388    | 1       | 280.1    |
| nested:$.records\[\*\].id      | strata             | 1.491    | 2000    | 282.6    |
|                                | orjson+jmespath    | 5.792    | 2000    | 282.6    |
|                                | orjson+jsonpath-ng | 7.855    | 2000    | 282.6    |
| nested:$.records\[0\]          | strata             | 1.444    | 1       | 282.6    |
|                                | orjson+jmespath    | 5.083    | 1       | 282.6    |
|                                | orjson+jsonpath-ng | 5.227    | 1       | 282.6    |
| wide_arrays:$.records\[\*\].id | strata             | 10.779   | 2000    | 296.9    |
|                                | orjson+jmespath    | 15.544   | 2000    | 296.9    |
|                                | orjson+jsonpath-ng | 18.011   | 2000    | 296.9    |
| wide_arrays:$.records\[0\]     | strata             | 10.933   | 1       | 296.9    |
|                                | orjson+jsonpath-ng | 14.940   | 1       | 296.9    |
|                                | orjson+jmespath    | 15.828   | 1       | 296.9    |
| mixed:$.records\[\*\].id       | strata             | 1.967    | 2000    | 299.4    |
|                                | orjson+jmespath    | 5.526    | 2000    | 299.4    |
|                                | orjson+jsonpath-ng | 8.187    | 2000    | 299.4    |
| mixed:$.records\[0\]           | strata             | 1.949    | 1       | 299.4    |
|                                | orjson+jmespath    | 4.993    | 1       | 299.4    |
|                                | orjson+jsonpath-ng | 5.335    | 1       | 299.4    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.351    | 2000    | 287.7    |
|                                            | jmespath    | 1.121    | 2000    | 287.7    |
|                                            | jsonpath-ng | 16.651   | 2000    | 288.5    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.209    | 100999  | 288.6    |
|                                            | jmespath    | 66.875   | 2000    | 293.4    |
|                                            | jsonpath-ng | 252.536  | 100999  | 294.7    |
| $..price                                   | strata      | 8.052    | 100999  | 295.1    |
|                                            | jsonpath-ng | 918.741  | 100999  | 277.7    |
| $.users\[?(@.age>30)\]                     | strata      | 0.405    | 1606    | 275.8    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.920    | 6650    | 275.8    |
| flat:$.records\[\*\].id                    | strata      | 0.123    | 2000    | 280.1    |
|                                            | jmespath    | 0.662    | 2000    | 280.1    |
|                                            | jsonpath-ng | 4.520    | 2000    | 280.1    |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 280.1    |
|                                            | jmespath    | 0.007    | 1       | 280.1    |
|                                            | jsonpath-ng | 0.007    | 1       | 280.1    |
| nested:$.records\[\*\].id                  | strata      | 0.108    | 2000    | 282.6    |
|                                            | jmespath    | 0.784    | 2000    | 282.6    |
|                                            | jsonpath-ng | 5.546    | 2000    | 282.6    |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 282.6    |
|                                            | jmespath    | 0.008    | 1       | 282.6    |
|                                            | jsonpath-ng | 0.009    | 1       | 282.6    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.109    | 2000    | 296.9    |
|                                            | jmespath    | 0.728    | 2000    | 296.9    |
|                                            | jsonpath-ng | 7.937    | 2000    | 296.9    |
| wide_arrays:$.records\[0\]                 | strata      | 0.009    | 1       | 296.9    |
|                                            | jsonpath-ng | 0.015    | 1       | 296.9    |
|                                            | jmespath    | 0.016    | 1       | 296.9    |
| mixed:$.records\[\*\].id                   | strata      | 0.126    | 2000    | 299.4    |
|                                            | jmespath    | 0.723    | 2000    | 299.4    |
|                                            | jsonpath-ng | 5.023    | 2000    | 299.4    |
| mixed:$.records\[0\]                       | jsonpath-ng | 0.009    | 1       | 299.4    |
|                                            | jmespath    | 0.013    | 1       | 299.4    |
|                                            | strata      | 0.015    | 1       | 299.4    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **11.7% faster** than #2 (orjson)  |
| loads (NDJSON)      | **#1** / 5              | **35.3% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **20.7% faster** than #2 (orjson)  |
| load (NDJSON file)  | **#1** / 5              | **31.0% faster** than #2 (orjson)  |
| dumps               | **#1** / 5              | **59.4% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **58.9% faster** than #2 (orjson)  |
| loads (flat)        | **#1** / 5              | **29.4% faster** than #2 (msgspec) |
| load (flat)         | **#1** / 5              | **43.7% faster** than #2 (msgspec) |
| dumps (flat)        | **#1** / 5              | **6.9% faster** than #2 (orjson)   |
| dump (flat)         | **#2** / 5              | 2.6% behind #1 (orjson)            |
| loads (nested)      | **#1** / 5              | **91.9% faster** than #2 (orjson)  |
| load (nested)       | **#1** / 5              | **86.1% faster** than #2 (orjson)  |
| dumps (nested)      | **#2** / 5              | 4.5% behind #1 (orjson)            |
| dump (nested)       | **#1** / 5              | **7.7% faster** than #2 (msgspec)  |
| loads (wide_arrays) | **#2** / 5              | 5.2% behind #1 (orjson)            |
| load (wide_arrays)  | **#2** / 5              | 10.3% behind #1 (orjson)           |
| dumps (wide_arrays) | **#1** / 5              | **25.7% faster** than #2 (msgspec) |
| dump (wide_arrays)  | **#1** / 5              | **16.7% faster** than #2 (msgspec) |
| loads (mixed)       | **#1** / 5              | **50.3% faster** than #2 (msgspec) |
| load (mixed)        | **#1** / 5              | **29.2% faster** than #2 (orjson)  |
| dumps (mixed)       | **#1** / 5              | **5.7% faster** than #2 (orjson)   |
| dump (mixed)        | **#2** / 5              | 1.6% behind #1 (orjson)            |
| search (JSONPath)   | **#1** in 11/11 queries | avg **770% faster**                |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
