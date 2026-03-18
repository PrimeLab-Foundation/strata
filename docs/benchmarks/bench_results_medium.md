# Strata Benchmark Results

Generated: 2026-03-18 15:02:04

## Environment

- Commit: 26bebc11f29d91458f5425c35bb69bdfbc551de7
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
| strata        | flat         | 2.801    | 3.247       | 3.338    | 269.7    |
| orjson        | flat         | 3.764    | 4.282       | 4.707    | 272.2    |
| msgspec       | flat         | 3.773    | 4.621       | 4.664    | 272.2    |
| ujson         | flat         | 6.723    | 6.966       | 7.510    | 272.2    |
| json (stdlib) | flat         | 9.737    | 10.300      | 10.600   | 272.2    |
| strata        | mixed        | 2.853    | 3.356       | 3.547    | 357.3    |
| orjson        | mixed        | 4.504    | 4.857       | 12.249   | 301.0    |
| msgspec       | mixed        | 4.856    | 5.443       | 5.547    | 301.0    |
| ujson         | mixed        | 5.724    | 6.997       | 7.829    | 301.0    |
| json (stdlib) | mixed        | 9.753    | 10.684      | 10.805   | 301.0    |
| strata        | nested       | 2.717    | 2.932       | 2.947    | 277.8    |
| msgspec       | nested       | 4.679    | 6.372       | 6.690    | 277.8    |
| orjson        | nested       | 5.433    | 6.655       | 8.980    | 277.8    |
| ujson         | nested       | 6.778    | 7.818       | 8.078    | 277.8    |
| json (stdlib) | nested       | 8.200    | 9.300       | 11.058   | 277.8    |
| strata        | users.json   | 24.617   | 30.295      | 30.609   | 73.5     |
| orjson        | users.json   | 27.545   | 39.871      | 46.794   | 93.4     |
| msgspec       | users.json   | 27.884   | 39.878      | 40.598   | 92.2     |
| ujson         | users.json   | 38.506   | 54.095      | 54.984   | 109.9    |
| json (stdlib) | users.json   | 54.616   | 65.748      | 67.578   | 98.5     |
| strata        | users.ndjson | 25.466   | 31.037      | 32.068   | 251.9    |
| orjson        | users.ndjson | 33.165   | 35.105      | 35.933   | 260.0    |
| msgspec       | users.ndjson | 35.525   | 36.113      | 38.646   | 261.2    |
| ujson         | users.ndjson | 46.010   | 49.886      | 52.292   | 260.2    |
| json (stdlib) | users.ndjson | 63.304   | 64.311      | 66.068   | 262.2    |
| orjson        | wide_arrays  | 12.110   | 15.635      | 18.410   | 300.3    |
| strata        | wide_arrays  | 15.954   | 17.420      | 19.207   | 290.0    |
| msgspec       | wide_arrays  | 14.986   | 18.592      | 18.781   | 300.3    |
| ujson         | wide_arrays  | 24.020   | 27.659      | 28.603   | 300.3    |
| json (stdlib) | wide_arrays  | 52.812   | 53.231      | 55.170   | 300.3    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 3.102    | 3.353       | 3.888    | 272.2    |
| msgspec       | flat         | 4.690    | 4.745       | 4.967    | 273.8    |
| orjson        | flat         | 4.518    | 4.752       | 5.512    | 272.2    |
| ujson         | flat         | 6.806    | 7.425       | 8.391    | 273.8    |
| json (stdlib) | flat         | 9.965    | 10.534      | 11.784   | 273.8    |
| strata        | mixed        | 3.097    | 3.707       | 3.878    | 301.0    |
| orjson        | mixed        | 5.116    | 5.130       | 5.724    | 301.0    |
| msgspec       | mixed        | 5.296    | 5.590       | 5.900    | 301.0    |
| ujson         | mixed        | 7.239    | 7.631       | 7.898    | 301.0    |
| json (stdlib) | mixed        | 11.210   | 11.211      | 13.910   | 301.0    |
| strata        | nested       | 2.702    | 3.100       | 3.143    | 277.8    |
| msgspec       | nested       | 4.958    | 6.149       | 6.298    | 277.8    |
| orjson        | nested       | 4.717    | 6.444       | 6.669    | 277.8    |
| ujson         | nested       | 6.009    | 7.766       | 7.799    | 277.8    |
| json (stdlib) | nested       | 7.985    | 9.873       | 10.247   | 277.8    |
| strata        | users.json   | 24.873   | 30.705      | 31.462   | 99.9     |
| orjson        | users.json   | 26.416   | 39.340      | 39.986   | 99.9     |
| msgspec       | users.json   | 28.434   | 40.740      | 43.392   | 104.8    |
| ujson         | users.json   | 40.598   | 56.235      | 58.769   | 122.5    |
| json (stdlib) | users.json   | 55.143   | 68.676      | 69.895   | 104.8    |
| strata        | users.ndjson | 27.276   | 32.031      | 33.355   | 264.2    |
| orjson        | users.ndjson | 33.161   | 35.411      | 36.662   | 265.2    |
| msgspec       | users.ndjson | 34.046   | 36.253      | 37.118   | 266.2    |
| ujson         | users.ndjson | 46.239   | 51.849      | 53.084   | 265.2    |
| json (stdlib) | users.ndjson | 64.862   | 66.399      | 67.310   | 267.2    |
| strata        | wide_arrays  | 13.629   | 16.128      | 16.157   | 300.3    |
| orjson        | wide_arrays  | 12.746   | 16.312      | 16.508   | 300.0    |
| msgspec       | wide_arrays  | 15.921   | 19.253      | 19.363   | 300.0    |
| ujson         | wide_arrays  | 26.229   | 29.485      | 29.499   | 300.0    |
| json (stdlib) | wide_arrays  | 52.010   | 55.389      | 55.423   | 300.0    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.983    | 0.983       | 1.080    | 276.1    |
| strata  | flat        | 0.905    | 1.005       | 1.152    | 273.8    |
| msgspec | flat        | 1.145    | 1.155       | 1.167    | 276.2    |
| ujson   | flat        | 4.795    | 5.198       | 5.396    | 277.8    |
| json    | flat        | 9.008    | 9.050       | 9.218    | 277.8    |
| strata  | mixed       | 0.977    | 1.004       | 1.026    | 301.0    |
| orjson  | mixed       | 1.065    | 1.066       | 1.075    | 301.0    |
| msgspec | mixed       | 1.151    | 1.155       | 1.198    | 301.0    |
| ujson   | mixed       | 5.061    | 5.069       | 5.088    | 301.0    |
| json    | mixed       | 11.346   | 11.591      | 11.703   | 301.0    |
| strata  | nested      | 0.731    | 0.739       | 0.742    | 277.8    |
| orjson  | nested      | 0.785    | 0.789       | 0.795    | 277.8    |
| msgspec | nested      | 0.786    | 0.791       | 0.798    | 277.8    |
| ujson   | nested      | 3.855    | 3.968       | 4.111    | 277.8    |
| json    | nested      | 5.542    | 5.638       | 5.665    | 277.8    |
| strata  | users.json  | 6.623    | 6.645       | 6.719    | 108.6    |
| msgspec | users.json  | 10.896   | 10.923      | 11.020   | 159.3    |
| orjson  | users.json  | 10.934   | 10.983      | 11.061   | 130.9    |
| ujson   | users.json  | 42.587   | 42.661      | 43.252   | 165.1    |
| json    | users.json  | 58.118   | 58.144      | 58.942   | 192.3    |
| strata  | wide_arrays | 5.441    | 5.442       | 5.451    | 304.9    |
| msgspec | wide_arrays | 6.835    | 6.903       | 6.904    | 315.0    |
| orjson  | wide_arrays | 6.992    | 7.050       | 7.088    | 295.3    |
| ujson   | wide_arrays | 28.511   | 28.519      | 28.535   | 317.0    |
| json    | wide_arrays | 80.146   | 83.310      | 83.946   | 337.9    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.171    | 1.177       | 1.222    | 277.8    |
| orjson        | flat        | 1.370    | 1.407       | 1.484    | 277.8    |
| msgspec       | flat        | 1.454    | 1.488       | 1.518    | 277.8    |
| ujson         | flat        | 5.201    | 5.276       | 5.284    | 277.8    |
| json (stdlib) | flat        | 32.957   | 33.107      | 34.116   | 277.8    |
| strata        | mixed       | 1.220    | 1.235       | 1.239    | 301.0    |
| orjson        | mixed       | 1.341    | 1.346       | 1.359    | 301.0    |
| msgspec       | mixed       | 1.409    | 1.418       | 1.469    | 301.0    |
| ujson         | mixed       | 5.290    | 5.352       | 5.423    | 301.0    |
| json (stdlib) | mixed       | 38.438   | 38.510      | 38.546   | 301.0    |
| strata        | nested      | 0.963    | 1.009       | 1.018    | 277.8    |
| orjson        | nested      | 1.110    | 1.118       | 1.144    | 277.8    |
| msgspec       | nested      | 1.113    | 1.175       | 1.182    | 277.8    |
| ujson         | nested      | 4.240    | 4.402       | 4.446    | 277.8    |
| json (stdlib) | nested      | 48.013   | 48.586      | 49.166   | 277.8    |
| strata        | users.json  | 7.417    | 7.677       | 7.778    | 198.6    |
| orjson        | users.json  | 11.722   | 11.995      | 11.999   | 220.9    |
| msgspec       | users.json  | 11.863   | 12.175      | 12.385   | 243.4    |
| ujson         | users.json  | 43.325   | 43.376      | 44.807   | 221.1    |
| json (stdlib) | users.json  | 425.170  | 429.425     | 432.827  | 243.4    |
| strata        | wide_arrays | 6.269    | 6.353       | 7.549    | 337.9    |
| msgspec       | wide_arrays | 7.946    | 8.403       | 8.600    | 357.3    |
| orjson        | wide_arrays | 8.283    | 9.417       | 10.462   | 335.6    |
| ujson         | wide_arrays | 30.032   | 30.255      | 30.485   | 337.6    |
| json (stdlib) | wide_arrays | 181.294  | 181.601     | 182.888  | 357.3    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 12.187   | 2000    | 240.5    |
|                                | orjson+jmespath    | 32.229   | 2000    | 243.6    |
|                                | orjson+jsonpath-ng | 32.338   | 2000    | 245.8    |
| all item prices                | strata             | 13.747   | 100999  | 246.2    |
|                                | orjson+jmespath    | 80.448   | 2000    | 255.4    |
|                                | orjson+jsonpath-ng | 257.932  | 100999  | 262.8    |
| recursive price                | strata             | 13.526   | 100999  | 259.7    |
|                                | orjson+jsonpath-ng | 869.725  | 100999  | 263.8    |
| flat:$.records\[\*\].id        | strata             | 1.917    | 2000    | 277.8    |
|                                | orjson+jmespath    | 4.820    | 2000    | 277.8    |
|                                | orjson+jsonpath-ng | 7.261    | 2000    | 277.8    |
| flat:$.records\[0\]            | strata             | 1.784    | 1       | 277.8    |
|                                | orjson+jsonpath-ng | 4.063    | 1       | 277.8    |
|                                | orjson+jmespath    | 4.354    | 1       | 277.8    |
| nested:$.records\[\*\].id      | strata             | 1.486    | 2000    | 277.9    |
|                                | orjson+jmespath    | 6.654    | 2000    | 277.9    |
|                                | orjson+jsonpath-ng | 8.503    | 2000    | 277.9    |
| nested:$.records\[0\]          | strata             | 1.346    | 1       | 277.9    |
|                                | orjson+jmespath    | 5.271    | 1       | 277.9    |
|                                | orjson+jsonpath-ng | 5.435    | 1       | 277.9    |
| wide_arrays:$.records\[\*\].id | strata             | 10.753   | 2000    | 357.3    |
|                                | orjson+jmespath    | 15.423   | 2000    | 357.3    |
|                                | orjson+jsonpath-ng | 18.407   | 2000    | 357.3    |
| wide_arrays:$.records\[0\]     | strata             | 10.812   | 1       | 357.3    |
|                                | orjson+jsonpath-ng | 14.471   | 1       | 357.3    |
|                                | orjson+jmespath    | 14.889   | 1       | 357.3    |
| mixed:$.records\[\*\].id       | strata             | 1.900    | 2000    | 301.0    |
|                                | orjson+jmespath    | 5.110    | 2000    | 301.0    |
|                                | orjson+jsonpath-ng | 7.342    | 2000    | 301.0    |
| mixed:$.records\[0\]           | strata             | 1.931    | 1       | 301.0    |
|                                | orjson+jsonpath-ng | 4.403    | 1       | 301.0    |
|                                | orjson+jmespath    | 4.480    | 1       | 301.0    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.283    | 2000    | 286.0    |
|                                            | jmespath    | 0.934    | 2000    | 286.0    |
|                                            | jsonpath-ng | 15.989   | 2000    | 286.8    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.149    | 100999  | 286.8    |
|                                            | jmespath    | 61.363   | 2000    | 290.6    |
|                                            | jsonpath-ng | 239.828  | 100999  | 293.0    |
| $..price                                   | strata      | 8.215    | 100999  | 292.7    |
|                                            | jsonpath-ng | 869.204  | 100999  | 295.0    |
| $.users\[?(@.age>30)\]                     | strata      | 0.320    | 1606    | 273.2    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.398    | 6650    | 273.2    |
| flat:$.records\[\*\].id                    | strata      | 0.125    | 2000    | 277.8    |
|                                            | jmespath    | 0.672    | 2000    | 277.8    |
|                                            | jsonpath-ng | 4.286    | 2000    | 277.8    |
| flat:$.records\[0\]                        | jmespath    | 0.007    | 1       | 277.8    |
|                                            | strata      | 0.009    | 1       | 277.8    |
|                                            | jsonpath-ng | 0.015    | 1       | 277.8    |
| nested:$.records\[\*\].id                  | strata      | 0.116    | 2000    | 277.9    |
|                                            | jmespath    | 0.680    | 2000    | 277.9    |
|                                            | jsonpath-ng | 5.918    | 2000    | 277.9    |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 277.9    |
|                                            | jsonpath-ng | 0.010    | 1       | 277.9    |
|                                            | jmespath    | 0.016    | 1       | 277.9    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.111    | 2000    | 357.3    |
|                                            | jmespath    | 0.697    | 2000    | 357.3    |
|                                            | jsonpath-ng | 6.812    | 2000    | 357.3    |
| wide_arrays:$.records\[0\]                 | strata      | 0.007    | 1       | 357.3    |
|                                            | jmespath    | 0.012    | 1       | 357.3    |
|                                            | jsonpath-ng | 0.017    | 1       | 357.3    |
| mixed:$.records\[\*\].id                   | strata      | 0.109    | 2000    | 301.1    |
|                                            | jmespath    | 0.677    | 2000    | 301.1    |
|                                            | jsonpath-ng | 4.855    | 2000    | 301.1    |
| mixed:$.records\[0\]                       | strata      | 0.004    | 1       | 301.1    |
|                                            | jmespath    | 0.006    | 1       | 301.1    |
|                                            | jsonpath-ng | 0.007    | 1       | 301.1    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **11.9% faster** than #2 (orjson)  |
| loads (NDJSON)      | **#1** / 5              | **30.2% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **6.2% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **21.6% faster** than #2 (orjson)  |
| dumps               | **#1** / 5              | **64.5% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **58.0% faster** than #2 (orjson)  |
| loads (flat)        | **#1** / 5              | **34.4% faster** than #2 (orjson)  |
| load (flat)         | **#1** / 5              | **45.6% faster** than #2 (orjson)  |
| dumps (flat)        | **#1** / 5              | **8.6% faster** than #2 (orjson)   |
| dump (flat)         | **#1** / 5              | **17.0% faster** than #2 (orjson)  |
| loads (nested)      | **#1** / 5              | **72.2% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **74.6% faster** than #2 (orjson)  |
| dumps (nested)      | **#1** / 5              | **7.5% faster** than #2 (orjson)   |
| dump (nested)       | **#1** / 5              | **15.2% faster** than #2 (orjson)  |
| loads (wide_arrays) | **#3** / 5              | 31.7% behind #1 (orjson)           |
| load (wide_arrays)  | **#2** / 5              | 6.9% behind #1 (orjson)            |
| dumps (wide_arrays) | **#1** / 5              | **25.6% faster** than #2 (msgspec) |
| dump (wide_arrays)  | **#1** / 5              | **26.8% faster** than #2 (msgspec) |
| loads (mixed)       | **#1** / 5              | **57.8% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **65.2% faster** than #2 (orjson)  |
| dumps (mixed)       | **#1** / 5              | **8.9% faster** than #2 (orjson)   |
| dump (mixed)        | **#1** / 5              | **9.9% faster** than #2 (orjson)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **752% faster**                |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
