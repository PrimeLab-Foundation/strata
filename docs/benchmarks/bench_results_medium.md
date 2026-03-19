# Strata Benchmark Results

Generated: 2026-03-19 02:46:05

## Environment

- Commit: 42f0bfe95d82803fac6378a50057ee7f0b40daca
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
| strata        | flat         | 2.870    | 3.355       | 3.360    | 270.5    |
| orjson        | flat         | 4.257    | 4.531       | 4.967    | 273.0    |
| msgspec       | flat         | 4.468    | 4.699       | 5.107    | 273.0    |
| ujson         | flat         | 6.728    | 7.044       | 7.358    | 273.0    |
| json (stdlib) | flat         | 10.297   | 10.581      | 10.643   | 273.0    |
| strata        | mixed        | 3.204    | 3.874       | 3.922    | 260.8    |
| msgspec       | mixed        | 4.761    | 5.921       | 6.078    | 263.0    |
| orjson        | mixed        | 5.199    | 6.484       | 7.202    | 263.0    |
| ujson         | mixed        | 7.011    | 7.836       | 8.417    | 263.0    |
| json (stdlib) | mixed        | 10.490   | 12.131      | 12.721   | 263.0    |
| strata        | nested       | 3.036    | 3.736       | 3.766    | 279.6    |
| orjson        | nested       | 5.944    | 7.115       | 7.148    | 282.0    |
| msgspec       | nested       | 5.481    | 7.909       | 8.003    | 282.0    |
| ujson         | nested       | 6.294    | 8.636       | 8.675    | 282.0    |
| json (stdlib) | nested       | 8.920    | 10.730      | 14.886   | 282.0    |
| strata        | users.json   | 25.445   | 31.818      | 32.721   | 73.6     |
| orjson        | users.json   | 27.397   | 42.532      | 51.647   | 93.5     |
| msgspec       | users.json   | 29.238   | 44.262      | 57.910   | 92.3     |
| ujson         | users.json   | 41.343   | 58.879      | 60.972   | 109.9    |
| json (stdlib) | users.json   | 55.596   | 70.144      | 82.847   | 98.5     |
| strata        | users.ndjson | 27.853   | 33.705      | 35.085   | 252.7    |
| orjson        | users.ndjson | 40.175   | 40.965      | 41.504   | 260.8    |
| msgspec       | users.ndjson | 39.454   | 41.101      | 41.493   | 261.9    |
| ujson         | users.ndjson | 47.737   | 56.210      | 68.223   | 260.9    |
| json (stdlib) | users.ndjson | 72.728   | 72.892      | 73.173   | 262.9    |
| strata        | wide_arrays  | 13.044   | 16.025      | 16.196   | 294.8    |
| orjson        | wide_arrays  | 13.567   | 17.186      | 17.418   | 305.1    |
| msgspec       | wide_arrays  | 17.228   | 21.120      | 21.945   | 305.1    |
| ujson         | wide_arrays  | 26.600   | 29.294      | 33.699   | 305.1    |
| json (stdlib) | wide_arrays  | 58.291   | 58.651      | 59.176   | 305.1    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 3.191    | 3.629       | 3.732    | 273.0    |
| orjson        | flat         | 4.427    | 4.635       | 4.694    | 273.0    |
| msgspec       | flat         | 5.702    | 6.607       | 7.877    | 273.7    |
| ujson         | flat         | 7.956    | 9.407       | 9.521    | 273.7    |
| json (stdlib) | flat         | 11.744   | 11.806      | 12.343   | 273.7    |
| strata        | mixed        | 3.082    | 3.645       | 3.779    | 263.0    |
| msgspec       | mixed        | 5.736    | 6.852       | 6.871    | 263.0    |
| orjson        | mixed        | 5.737    | 7.208       | 8.142    | 263.0    |
| ujson         | mixed        | 10.108   | 11.620      | 13.053   | 263.0    |
| json (stdlib) | mixed        | 12.681   | 12.932      | 14.531   | 263.0    |
| strata        | nested       | 3.362    | 4.147       | 4.488    | 282.0    |
| msgspec       | nested       | 5.336    | 6.792       | 7.660    | 282.0    |
| orjson        | nested       | 6.064    | 7.936       | 8.026    | 282.0    |
| ujson         | nested       | 7.930    | 9.238       | 11.239   | 282.0    |
| json (stdlib) | nested       | 10.010   | 10.772      | 11.089   | 282.0    |
| strata        | users.json   | 25.902   | 32.669      | 33.632   | 99.9     |
| orjson        | users.json   | 27.123   | 40.840      | 41.184   | 99.9     |
| msgspec       | users.json   | 29.195   | 49.805      | 57.432   | 104.9    |
| ujson         | users.json   | 44.953   | 59.077      | 67.130   | 122.6    |
| json (stdlib) | users.json   | 59.650   | 77.454      | 78.061   | 104.9    |
| strata        | users.ndjson | 28.852   | 34.689      | 35.594   | 265.0    |
| msgspec       | users.ndjson | 40.172   | 41.398      | 67.201   | 267.0    |
| orjson        | users.ndjson | 42.180   | 42.849      | 45.130   | 266.0    |
| ujson         | users.ndjson | 52.165   | 55.160      | 55.987   | 266.0    |
| json (stdlib) | users.ndjson | 66.564   | 67.318      | 73.250   | 268.0    |
| strata        | wide_arrays  | 14.661   | 16.365      | 17.039   | 305.1    |
| orjson        | wide_arrays  | 16.500   | 18.354      | 18.537   | 304.8    |
| msgspec       | wide_arrays  | 22.342   | 22.696      | 23.126   | 246.1    |
| ujson         | wide_arrays  | 28.351   | 31.078      | 32.527   | 246.1    |
| json (stdlib) | wide_arrays  | 59.848   | 60.999      | 61.683   | 246.1    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 1.086    | 1.090       | 1.097    | 273.7    |
| orjson  | flat        | 1.175    | 1.195       | 1.286    | 276.0    |
| msgspec | flat        | 1.324    | 1.345       | 1.400    | 277.0    |
| ujson   | flat        | 5.441    | 5.561       | 5.572    | 279.6    |
| json    | flat        | 9.525    | 9.563       | 10.230   | 279.6    |
| orjson  | mixed       | 1.219    | 1.229       | 1.301    | 263.0    |
| strata  | mixed       | 1.279    | 1.284       | 1.378    | 263.0    |
| msgspec | mixed       | 1.319    | 1.325       | 1.344    | 263.0    |
| ujson   | mixed       | 5.272    | 5.669       | 5.844    | 263.0    |
| json    | mixed       | 12.163   | 12.248      | 12.273   | 263.0    |
| orjson  | nested      | 0.815    | 0.816       | 0.834    | 282.7    |
| msgspec | nested      | 0.835    | 0.853       | 0.925    | 282.7    |
| strata  | nested      | 0.867    | 0.868       | 1.050    | 282.7    |
| ujson   | nested      | 4.187    | 4.224       | 4.258    | 282.7    |
| json    | nested      | 5.860    | 6.105       | 6.132    | 282.7    |
| strata  | users.json  | 9.208    | 9.341       | 9.897    | 109.1    |
| msgspec | users.json  | 11.803   | 11.997      | 13.409   | 159.8    |
| orjson  | users.json  | 12.439   | 12.455      | 12.940   | 131.4    |
| ujson   | users.json  | 47.035   | 47.963      | 51.789   | 165.6    |
| json    | users.json  | 62.084   | 62.403      | 65.556   | 190.9    |
| strata  | wide_arrays | 6.598    | 6.891       | 7.007    | 246.1    |
| orjson  | wide_arrays | 8.036    | 8.201       | 8.334    | 236.5    |
| msgspec | wide_arrays | 8.360    | 8.889       | 9.422    | 237.8    |
| ujson   | wide_arrays | 31.253   | 33.168      | 37.932   | 247.1    |
| json    | wide_arrays | 91.504   | 91.655      | 94.067   | 239.6    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.239    | 1.304       | 1.396    | 279.6    |
| orjson        | flat        | 1.407    | 1.458       | 1.513    | 279.6    |
| msgspec       | flat        | 1.563    | 1.653       | 2.048    | 279.6    |
| ujson         | flat        | 5.404    | 5.452       | 5.904    | 279.6    |
| json (stdlib) | flat        | 36.275   | 37.172      | 37.370   | 279.6    |
| msgspec       | mixed       | 1.755    | 1.777       | 1.843    | 263.0    |
| strata        | mixed       | 1.447    | 2.117       | 2.135    | 263.0    |
| orjson        | mixed       | 1.958    | 2.376       | 2.858    | 263.0    |
| ujson         | mixed       | 6.317    | 6.410       | 6.704    | 263.0    |
| json (stdlib) | mixed       | 42.724   | 44.206      | 48.980   | 263.0    |
| orjson        | nested      | 1.207    | 1.243       | 1.357    | 282.7    |
| msgspec       | nested      | 1.233    | 1.258       | 1.359    | 282.7    |
| strata        | nested      | 1.214    | 1.262       | 1.306    | 282.7    |
| ujson         | nested      | 4.793    | 4.866       | 5.074    | 282.7    |
| json (stdlib) | nested      | 49.843   | 50.555      | 51.098   | 282.7    |
| strata        | users.json  | 10.404   | 10.447      | 10.692   | 197.2    |
| msgspec       | users.json  | 12.405   | 12.734      | 12.899   | 243.9    |
| orjson        | users.json  | 12.537   | 12.752      | 12.855   | 220.5    |
| ujson         | users.json  | 46.892   | 47.141      | 48.983   | 221.6    |
| json (stdlib) | users.json  | 461.825  | 462.186     | 483.424  | 244.0    |
| strata        | wide_arrays | 7.186    | 7.384       | 7.825    | 239.6    |
| orjson        | wide_arrays | 8.782    | 9.197       | 9.782    | 245.2    |
| msgspec       | wide_arrays | 9.527    | 9.669       | 9.805    | 260.7    |
| ujson         | wide_arrays | 32.963   | 33.283      | 33.917   | 260.6    |
| json (stdlib) | wide_arrays | 184.471  | 193.961     | 195.802  | 260.7    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 12.858   | 2000    | 241.0    |
|                                | orjson+jmespath    | 35.904   | 2000    | 244.2    |
|                                | orjson+jsonpath-ng | 37.873   | 2000    | 246.3    |
| all item prices                | strata             | 14.211   | 100999  | 248.0    |
|                                | orjson+jmespath    | 86.468   | 2000    | 257.2    |
|                                | orjson+jsonpath-ng | 270.624  | 100999  | 264.2    |
| recursive price                | strata             | 15.480   | 100999  | 262.0    |
|                                | orjson+jsonpath-ng | 920.079  | 100999  | 264.7    |
| flat:$.records\[\*\].id        | strata             | 2.073    | 2000    | 279.6    |
|                                | orjson+jmespath    | 5.360    | 2000    | 279.6    |
|                                | orjson+jsonpath-ng | 7.906    | 2000    | 279.6    |
| flat:$.records\[0\]            | strata             | 2.000    | 1       | 279.6    |
|                                | orjson+jsonpath-ng | 4.199    | 1       | 279.6    |
|                                | orjson+jmespath    | 4.342    | 1       | 279.6    |
| nested:$.records\[\*\].id      | strata             | 1.549    | 2000    | 282.7    |
|                                | orjson+jmespath    | 6.817    | 2000    | 282.7    |
|                                | orjson+jsonpath-ng | 9.169    | 2000    | 282.7    |
| nested:$.records\[0\]          | strata             | 1.617    | 1       | 282.7    |
|                                | orjson+jmespath    | 5.174    | 1       | 282.7    |
|                                | orjson+jsonpath-ng | 5.196    | 1       | 282.7    |
| wide_arrays:$.records\[\*\].id | strata             | 9.744    | 2000    | 260.7    |
|                                | orjson+jmespath    | 15.490   | 2000    | 260.7    |
|                                | orjson+jsonpath-ng | 20.345   | 2000    | 260.7    |
| wide_arrays:$.records\[0\]     | strata             | 10.363   | 1       | 260.7    |
|                                | orjson+jmespath    | 15.457   | 1       | 260.7    |
|                                | orjson+jsonpath-ng | 16.466   | 1       | 260.7    |
| mixed:$.records\[\*\].id       | strata             | 1.888    | 2000    | 263.0    |
|                                | orjson+jmespath    | 5.407    | 2000    | 263.0    |
|                                | orjson+jsonpath-ng | 12.532   | 2000    | 263.0    |
| mixed:$.records\[0\]           | strata             | 2.069    | 1       | 263.0    |
|                                | orjson+jsonpath-ng | 5.075    | 1       | 263.0    |
|                                | orjson+jmespath    | 6.004    | 1       | 263.0    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.313    | 2000    | 286.9    |
|                                            | jmespath    | 1.011    | 2000    | 286.9    |
|                                            | jsonpath-ng | 17.278   | 2000    | 287.7    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.336    | 100999  | 287.8    |
|                                            | jmespath    | 64.537   | 2000    | 292.5    |
|                                            | jsonpath-ng | 272.137  | 100999  | 294.0    |
| $..price                                   | strata      | 8.542    | 100999  | 293.7    |
|                                            | jsonpath-ng | 904.954  | 100999  | 277.0    |
| $.users\[?(@.age>30)\]                     | strata      | 0.346    | 1606    | 275.0    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.447    | 6650    | 275.0    |
| flat:$.records\[\*\].id                    | strata      | 0.126    | 2000    | 279.6    |
|                                            | jmespath    | 0.695    | 2000    | 279.6    |
|                                            | jsonpath-ng | 5.728    | 2000    | 279.6    |
| flat:$.records\[0\]                        | strata      | 0.009    | 1       | 279.6    |
|                                            | jmespath    | 0.010    | 1       | 279.6    |
|                                            | jsonpath-ng | 0.011    | 1       | 279.6    |
| nested:$.records\[\*\].id                  | strata      | 0.109    | 2000    | 282.7    |
|                                            | jmespath    | 0.771    | 2000    | 282.7    |
|                                            | jsonpath-ng | 6.750    | 2000    | 282.7    |
| nested:$.records\[0\]                      | strata      | 0.006    | 1       | 282.7    |
|                                            | jmespath    | 0.016    | 1       | 282.7    |
|                                            | jsonpath-ng | 0.017    | 1       | 282.7    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.136    | 2000    | 260.7    |
|                                            | jmespath    | 0.840    | 2000    | 260.7    |
|                                            | jsonpath-ng | 7.646    | 2000    | 260.7    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 260.7    |
|                                            | jmespath    | 0.009    | 1       | 260.7    |
|                                            | jsonpath-ng | 0.025    | 1       | 260.7    |
| mixed:$.records\[\*\].id                   | strata      | 0.137    | 2000    | 263.0    |
|                                            | jmespath    | 0.771    | 2000    | 263.0    |
|                                            | jsonpath-ng | 5.318    | 2000    | 263.0    |
| mixed:$.records\[0\]                       | jsonpath-ng | 0.009    | 1       | 263.0    |
|                                            | jmespath    | 0.009    | 1       | 263.0    |
|                                            | strata      | 0.010    | 1       | 263.0    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **7.7% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **41.7% faster** than #2 (msgspec) |
| load (JSON file)    | **#1** / 5              | **4.7% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **39.2% faster** than #2 (msgspec) |
| dumps               | **#1** / 5              | **28.2% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **19.2% faster** than #2 (msgspec) |
| loads (flat)        | **#1** / 5              | **48.3% faster** than #2 (orjson)  |
| load (flat)         | **#1** / 5              | **38.8% faster** than #2 (orjson)  |
| dumps (flat)        | **#1** / 5              | **8.2% faster** than #2 (orjson)   |
| dump (flat)         | **#1** / 5              | **13.6% faster** than #2 (orjson)  |
| loads (nested)      | **#1** / 5              | **80.5% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **58.7% faster** than #2 (msgspec) |
| dumps (nested)      | **#3** / 5              | 6.4% behind #1 (orjson)            |
| dump (nested)       | **#2** / 5              | 0.6% behind #1 (orjson)            |
| loads (wide_arrays) | **#1** / 5              | **4.0% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **12.5% faster** than #2 (orjson)  |
| dumps (wide_arrays) | **#1** / 5              | **21.8% faster** than #2 (orjson)  |
| dump (wide_arrays)  | **#1** / 5              | **22.2% faster** than #2 (orjson)  |
| loads (mixed)       | **#1** / 5              | **48.6% faster** than #2 (msgspec) |
| load (mixed)        | **#1** / 5              | **86.1% faster** than #2 (msgspec) |
| dumps (mixed)       | **#2** / 5              | 4.9% behind #1 (orjson)            |
| dump (mixed)        | **#1** / 5              | **21.3% faster** than #2 (msgspec) |
| search (JSONPath)   | **#1** in 11/11 queries | avg **709% faster**                |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
