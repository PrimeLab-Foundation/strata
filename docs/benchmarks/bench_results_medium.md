# Strata Benchmark Results

Generated: 2026-03-18 20:28:08

## Environment

- Commit: 6da7fbd55a862e7e748708f6f30c5b3b945c0ab3
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
| strata        | flat         | 2.576    | 2.986       | 3.066    | 272.8    |
| orjson        | flat         | 3.752    | 4.308       | 4.533    | 275.3    |
| msgspec       | flat         | 4.045    | 4.977       | 6.603    | 275.3    |
| ujson         | flat         | 6.024    | 6.791       | 6.852    | 275.3    |
| json (stdlib) | flat         | 10.201   | 10.607      | 10.964   | 275.3    |
| strata        | mixed        | 2.653    | 3.009       | 3.075    | 295.4    |
| msgspec       | mixed        | 5.338    | 5.757       | 6.262    | 297.7    |
| orjson        | mixed        | 5.798    | 7.922       | 10.234   | 297.7    |
| ujson         | mixed        | 8.586    | 8.679       | 8.759    | 297.7    |
| json (stdlib) | mixed        | 11.214   | 11.924      | 11.952   | 297.7    |
| strata        | nested       | 2.685    | 2.851       | 2.883    | 278.7    |
| orjson        | nested       | 5.275    | 6.662       | 7.554    | 281.1    |
| msgspec       | nested       | 5.163    | 6.726       | 16.246   | 281.1    |
| ujson         | nested       | 7.259    | 8.788       | 10.659   | 281.1    |
| json (stdlib) | nested       | 10.007   | 11.852      | 12.195   | 281.1    |
| strata        | users.json   | 25.689   | 34.264      | 35.798   | 73.5     |
| msgspec       | users.json   | 38.075   | 42.460      | 50.590   | 92.2     |
| orjson        | users.json   | 35.295   | 54.605      | 67.998   | 93.4     |
| ujson         | users.json   | 48.444   | 68.726      | 91.939   | 109.9    |
| json (stdlib) | users.json   | 55.918   | 71.273      | 79.346   | 98.5     |
| strata        | users.ndjson | 23.334   | 30.765      | 33.075   | 256.1    |
| orjson        | users.ndjson | 34.255   | 37.090      | 38.384   | 263.1    |
| msgspec       | users.ndjson | 37.808   | 38.000      | 40.519   | 264.3    |
| ujson         | users.ndjson | 52.705   | 56.012      | 57.708   | 263.3    |
| json (stdlib) | users.ndjson | 65.510   | 65.858      | 67.465   | 265.3    |
| strata        | wide_arrays  | 10.186   | 14.567      | 26.824   | 293.3    |
| orjson        | wide_arrays  | 12.156   | 17.134      | 42.052   | 303.5    |
| msgspec       | wide_arrays  | 15.760   | 31.883      | 37.989   | 307.0    |
| ujson         | wide_arrays  | 29.063   | 53.972      | 60.500   | 307.0    |
| json (stdlib) | wide_arrays  | 56.777   | 83.537      | 85.078   | 244.5    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 3.079    | 3.439       | 3.513    | 275.3    |
| orjson        | flat         | 3.856    | 4.746       | 4.794    | 275.3    |
| msgspec       | flat         | 4.319    | 4.934       | 6.710    | 275.7    |
| ujson         | flat         | 6.396    | 7.428       | 7.647    | 275.7    |
| json (stdlib) | flat         | 10.667   | 10.990      | 11.570   | 275.7    |
| strata        | mixed        | 3.127    | 3.548       | 4.990    | 297.7    |
| orjson        | mixed        | 5.194    | 5.809       | 6.217    | 297.7    |
| msgspec       | mixed        | 5.331    | 5.822       | 6.125    | 297.7    |
| ujson         | mixed        | 6.609    | 8.159       | 8.218    | 297.7    |
| json (stdlib) | mixed        | 12.446   | 16.557      | 27.373   | 297.7    |
| strata        | nested       | 3.005    | 3.352       | 3.429    | 281.1    |
| orjson        | nested       | 6.182    | 6.579       | 6.840    | 281.1    |
| ujson         | nested       | 7.229    | 8.132       | 8.208    | 281.1    |
| msgspec       | nested       | 8.599    | 8.799       | 10.205   | 281.1    |
| json (stdlib) | nested       | 9.254    | 10.907      | 11.161   | 281.1    |
| strata        | users.json   | 25.749   | 32.967      | 37.805   | 99.9     |
| msgspec       | users.json   | 33.598   | 44.488      | 60.498   | 104.8    |
| orjson        | users.json   | 26.954   | 45.155      | 62.863   | 99.9     |
| ujson         | users.json   | 44.134   | 62.903      | 84.990   | 122.5    |
| json (stdlib) | users.json   | 67.533   | 70.111      | 72.273   | 104.8    |
| strata        | users.ndjson | 24.148   | 30.584      | 31.242   | 267.3    |
| orjson        | users.ndjson | 35.098   | 37.633      | 37.963   | 269.3    |
| msgspec       | users.ndjson | 36.244   | 38.439      | 39.322   | 269.3    |
| ujson         | users.ndjson | 53.982   | 55.104      | 56.544   | 268.3    |
| json (stdlib) | users.ndjson | 65.057   | 70.225      | 72.172   | 270.3    |
| strata        | wide_arrays  | 13.415   | 14.365      | 15.744   | 244.5    |
| msgspec       | wide_arrays  | 16.295   | 20.469      | 21.407   | 244.2    |
| orjson        | wide_arrays  | 20.678   | 23.892      | 25.445   | 244.2    |
| ujson         | wide_arrays  | 26.855   | 32.367      | 32.474   | 244.2    |
| json (stdlib) | wide_arrays  | 52.702   | 56.384      | 71.917   | 244.2    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.906    | 0.913       | 0.914    | 275.7    |
| orjson  | flat        | 0.980    | 0.982       | 1.174    | 278.0    |
| msgspec | flat        | 1.152    | 1.157       | 1.168    | 278.0    |
| ujson   | flat        | 4.807    | 4.880       | 4.918    | 279.7    |
| json    | flat        | 9.043    | 9.090       | 9.218    | 279.7    |
| strata  | mixed       | 1.083    | 1.113       | 1.119    | 297.7    |
| orjson  | mixed       | 1.078    | 1.116       | 1.143    | 297.7    |
| msgspec | mixed       | 1.191    | 1.194       | 4.193    | 297.9    |
| ujson   | mixed       | 5.099    | 5.177       | 5.183    | 297.9    |
| json    | mixed       | 12.506   | 12.683      | 12.806   | 297.9    |
| strata  | nested      | 0.765    | 0.770       | 0.770    | 281.1    |
| msgspec | nested      | 0.783    | 0.785       | 0.792    | 281.1    |
| orjson  | nested      | 0.774    | 0.786       | 0.795    | 281.1    |
| ujson   | nested      | 3.923    | 3.977       | 3.994    | 281.1    |
| json    | nested      | 5.224    | 5.246       | 5.279    | 281.1    |
| strata  | users.json  | 6.760    | 6.761       | 6.826    | 109.1    |
| orjson  | users.json  | 10.826   | 10.854      | 10.929   | 131.4    |
| msgspec | users.json  | 10.849   | 10.924      | 11.218   | 159.7    |
| ujson   | users.json  | 42.303   | 42.360      | 44.144   | 165.6    |
| json    | users.json  | 59.392   | 59.731      | 61.189   | 192.5    |
| strata  | wide_arrays | 5.928    | 6.024       | 6.718    | 244.2    |
| msgspec | wide_arrays | 7.232    | 7.308       | 7.327    | 254.3    |
| orjson  | wide_arrays | 7.289    | 7.322       | 7.664    | 234.5    |
| ujson   | wide_arrays | 29.674   | 30.278      | 30.690   | 256.3    |
| json    | wide_arrays | 84.646   | 85.749      | 93.183   | 277.2    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.216    | 1.349       | 1.434    | 279.7    |
| orjson        | flat        | 1.415    | 1.427       | 1.531    | 279.7    |
| msgspec       | flat        | 1.506    | 1.507       | 5.214    | 279.7    |
| ujson         | flat        | 5.187    | 5.207       | 5.218    | 279.7    |
| json (stdlib) | flat        | 32.730   | 32.881      | 32.935   | 279.7    |
| strata        | mixed       | 1.356    | 1.502       | 1.727    | 297.9    |
| msgspec       | mixed       | 1.477    | 1.541       | 1.569    | 297.9    |
| orjson        | mixed       | 1.631    | 2.243       | 3.020    | 297.9    |
| ujson         | mixed       | 5.493    | 5.786       | 18.447   | 297.9    |
| json (stdlib) | mixed       | 41.000   | 41.515      | 42.267   | 297.9    |
| strata        | nested      | 0.990    | 1.058       | 1.083    | 281.1    |
| orjson        | nested      | 1.055    | 1.065       | 1.074    | 281.1    |
| msgspec       | nested      | 1.159    | 1.174       | 1.411    | 281.1    |
| ujson         | nested      | 4.175    | 4.200       | 4.236    | 281.1    |
| json (stdlib) | nested      | 47.469   | 48.059      | 79.561   | 281.1    |
| strata        | users.json  | 8.279    | 9.644       | 10.731   | 198.8    |
| orjson        | users.json  | 11.862   | 12.080      | 13.246   | 221.1    |
| msgspec       | users.json  | 12.719   | 12.816      | 13.034   | 243.9    |
| ujson         | users.json  | 43.805   | 44.664      | 45.187   | 221.6    |
| json (stdlib) | users.json  | 474.276  | 493.975     | 508.899  | 244.0    |
| strata        | wide_arrays | 6.619    | 6.813       | 7.064    | 277.2    |
| orjson        | wide_arrays | 7.970    | 8.373       | 8.646    | 274.9    |
| msgspec       | wide_arrays | 9.018    | 9.680       | 10.315   | 295.4    |
| ujson         | wide_arrays | 31.946   | 32.158      | 32.460   | 276.9    |
| json (stdlib) | wide_arrays | 181.951  | 186.020     | 193.019  | 295.4    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 13.729   | 2000    | 241.0    |
|                                | orjson+jmespath    | 35.571   | 2000    | 244.1    |
|                                | orjson+jsonpath-ng | 37.868   | 2000    | 246.5    |
| all item prices                | strata             | 14.396   | 100999  | 247.3    |
|                                | orjson+jmespath    | 86.091   | 2000    | 256.7    |
|                                | orjson+jsonpath-ng | 273.975  | 100999  | 263.4    |
| recursive price                | strata             | 14.190   | 100999  | 262.1    |
|                                | orjson+jsonpath-ng | 897.300  | 100999  | 266.3    |
| flat:$.records\[\*\].id        | strata             | 1.791    | 2000    | 279.7    |
|                                | orjson+jmespath    | 4.665    | 2000    | 279.7    |
|                                | orjson+jsonpath-ng | 7.060    | 2000    | 278.7    |
| flat:$.records\[0\]            | strata             | 1.781    | 1       | 278.7    |
|                                | orjson+jsonpath-ng | 3.989    | 1       | 278.7    |
|                                | orjson+jmespath    | 4.668    | 1       | 278.7    |
| nested:$.records\[\*\].id      | strata             | 1.614    | 2000    | 281.1    |
|                                | orjson+jmespath    | 5.748    | 2000    | 281.1    |
|                                | orjson+jsonpath-ng | 9.461    | 2000    | 281.1    |
| nested:$.records\[0\]          | strata             | 1.515    | 1       | 281.1    |
|                                | orjson+jmespath    | 5.147    | 1       | 281.1    |
|                                | orjson+jsonpath-ng | 7.211    | 1       | 281.1    |
| wide_arrays:$.records\[\*\].id | strata             | 8.701    | 2000    | 295.4    |
|                                | orjson+jmespath    | 15.549   | 2000    | 295.4    |
|                                | orjson+jsonpath-ng | 18.415   | 2000    | 295.4    |
| wide_arrays:$.records\[0\]     | strata             | 8.562    | 1       | 295.4    |
|                                | orjson+jsonpath-ng | 14.505   | 1       | 295.4    |
|                                | orjson+jmespath    | 15.089   | 1       | 295.4    |
| mixed:$.records\[\*\].id       | strata             | 1.749    | 2000    | 297.9    |
|                                | orjson+jmespath    | 5.400    | 2000    | 297.9    |
|                                | orjson+jsonpath-ng | 10.067   | 2000    | 297.9    |
| mixed:$.records\[0\]           | strata             | 1.707    | 1       | 297.9    |
|                                | orjson+jsonpath-ng | 4.707    | 1       | 297.9    |
|                                | orjson+jmespath    | 5.162    | 1       | 297.9    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.308    | 2000    | 288.5    |
|                                            | jmespath    | 1.062    | 2000    | 288.5    |
|                                            | jsonpath-ng | 16.879   | 2000    | 289.3    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.192    | 100999  | 289.3    |
|                                            | jmespath    | 62.938   | 2000    | 294.1    |
|                                            | jsonpath-ng | 258.499  | 100999  | 295.5    |
| $..price                                   | strata      | 8.024    | 100999  | 295.9    |
|                                            | jsonpath-ng | 906.770  | 100999  | 279.4    |
| $.users\[?(@.age>30)\]                     | strata      | 0.365    | 1606    | 276.5    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.474    | 6650    | 276.5    |
| flat:$.records\[\*\].id                    | strata      | 0.120    | 2000    | 278.7    |
|                                            | jmespath    | 0.678    | 2000    | 278.7    |
|                                            | jsonpath-ng | 4.202    | 2000    | 278.7    |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 278.7    |
|                                            | jmespath    | 0.006    | 1       | 278.7    |
|                                            | jsonpath-ng | 0.013    | 1       | 278.7    |
| nested:$.records\[\*\].id                  | strata      | 0.111    | 2000    | 281.1    |
|                                            | jmespath    | 0.699    | 2000    | 281.1    |
|                                            | jsonpath-ng | 5.543    | 2000    | 281.1    |
| nested:$.records\[0\]                      | strata      | 0.005    | 1       | 281.1    |
|                                            | jmespath    | 0.009    | 1       | 281.1    |
|                                            | jsonpath-ng | 0.010    | 1       | 281.1    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.111    | 2000    | 295.4    |
|                                            | jmespath    | 0.782    | 2000    | 295.4    |
|                                            | jsonpath-ng | 7.014    | 2000    | 295.4    |
| wide_arrays:$.records\[0\]                 | strata      | 0.010    | 1       | 295.4    |
|                                            | jsonpath-ng | 0.011    | 1       | 295.4    |
|                                            | jmespath    | 0.015    | 1       | 295.4    |
| mixed:$.records\[\*\].id                   | strata      | 0.122    | 2000    | 297.9    |
|                                            | jmespath    | 0.724    | 2000    | 297.9    |
|                                            | jsonpath-ng | 5.889    | 2000    | 297.9    |
| mixed:$.records\[0\]                       | strata      | 0.006    | 1       | 297.9    |
|                                            | jsonpath-ng | 0.009    | 1       | 297.9    |
|                                            | jmespath    | 0.017    | 1       | 297.9    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **37.4% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **46.8% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **4.7% faster** than #2 (orjson)    |
| load (NDJSON file)  | **#1** / 5              | **45.3% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **60.1% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **43.3% faster** than #2 (orjson)   |
| loads (flat)        | **#1** / 5              | **45.7% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **25.2% faster** than #2 (orjson)   |
| dumps (flat)        | **#1** / 5              | **8.2% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **16.3% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **92.3% faster** than #2 (msgspec)  |
| load (nested)       | **#1** / 5              | **105.7% faster** than #2 (orjson)  |
| dumps (nested)      | **#1** / 5              | **1.1% faster** than #2 (orjson)    |
| dump (nested)       | **#1** / 5              | **6.5% faster** than #2 (orjson)    |
| loads (wide_arrays) | **#1** / 5              | **19.3% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **21.5% faster** than #2 (msgspec)  |
| dumps (wide_arrays) | **#1** / 5              | **22.0% faster** than #2 (msgspec)  |
| dump (wide_arrays)  | **#1** / 5              | **20.4% faster** than #2 (orjson)   |
| loads (mixed)       | **#1** / 5              | **101.2% faster** than #2 (msgspec) |
| load (mixed)        | **#1** / 5              | **66.1% faster** than #2 (orjson)   |
| dumps (mixed)       | **#2** / 5              | 0.4% behind #1 (orjson)             |
| dump (mixed)        | **#1** / 5              | **9.0% faster** than #2 (msgspec)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **745% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
