# Strata Benchmark Results

Generated: 2026-03-16 17:21:31

## Environment

- Commit: efd00fdc01e0951561d325921e36a0c1697c3ea1
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/large/users.json, benchmarks/data/generated/large/users.ndjson
- Random schemas: flat, nested, wide_arrays, mixed (5000 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 6.528    | 7.395       | 7.902    | 502.5    |
| orjson        | flat         | 8.685    | 10.175      | 10.628   | 502.5    |
| msgspec       | flat         | 9.164    | 10.490      | 11.101   | 502.5    |
| ujson         | flat         | 14.382   | 16.910      | 18.121   | 502.5    |
| json (stdlib) | flat         | 23.577   | 24.810      | 25.162   | 505.4    |
| strata        | mixed        | 6.157    | 7.698       | 7.781    | 789.6    |
| msgspec       | mixed        | 10.648   | 12.936      | 13.531   | 795.2    |
| ujson         | mixed        | 16.648   | 18.314      | 18.383   | 795.2    |
| orjson        | mixed        | 9.497    | 18.802      | 26.178   | 795.2    |
| json (stdlib) | mixed        | 24.379   | 27.171      | 27.556   | 797.6    |
| strata        | nested       | 4.963    | 6.237       | 6.572    | 503.8    |
| msgspec       | nested       | 13.116   | 16.771      | 17.361   | 503.8    |
| orjson        | nested       | 14.807   | 19.793      | 21.065   | 503.8    |
| ujson         | nested       | 15.641   | 20.193      | 20.480   | 503.8    |
| json (stdlib) | nested       | 21.205   | 25.747      | 25.859   | 503.8    |
| strata        | users.json   | 145.673  | 192.386     | 193.835  | 340.0    |
| orjson        | users.json   | 185.834  | 263.907     | 272.291  | 478.1    |
| msgspec       | users.json   | 196.366  | 272.282     | 274.997  | 467.6    |
| ujson         | users.json   | 277.830  | 376.502     | 379.449  | 589.9    |
| json (stdlib) | users.json   | 388.159  | 463.359     | 468.852  | 511.4    |
| strata        | users.ndjson | 162.292  | 202.913     | 205.199  | 562.0    |
| orjson        | users.ndjson | 245.579  | 311.576     | 323.080  | 612.2    |
| msgspec       | users.ndjson | 272.837  | 314.735     | 333.031  | 606.7    |
| ujson         | users.ndjson | 356.452  | 450.161     | 487.718  | 729.2    |
| json (stdlib) | users.ndjson | 455.814  | 524.263     | 544.835  | 608.4    |
| strata        | wide_arrays  | 30.314   | 37.032      | 39.320   | 426.4    |
| orjson        | wide_arrays  | 30.474   | 39.891      | 41.631   | 452.0    |
| msgspec       | wide_arrays  | 36.751   | 47.413      | 47.815   | 461.7    |
| ujson         | wide_arrays  | 60.602   | 71.140      | 72.095   | 460.8    |
| json (stdlib) | wide_arrays  | 129.019  | 136.233     | 144.503  | 475.7    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 6.857    | 7.990       | 8.157    | 505.4    |
| orjson        | flat         | 8.986    | 10.501      | 10.905   | 505.4    |
| msgspec       | flat         | 9.203    | 10.584      | 11.170   | 508.4    |
| ujson         | flat         | 14.616   | 16.841      | 18.052   | 508.4    |
| json (stdlib) | flat         | 23.917   | 25.534      | 25.880   | 508.4    |
| strata        | mixed        | 6.637    | 8.089       | 8.145    | 797.6    |
| orjson        | mixed        | 10.509   | 13.313      | 14.030   | 797.6    |
| msgspec       | mixed        | 11.041   | 13.775      | 13.950   | 800.0    |
| ujson         | mixed        | 15.711   | 19.113      | 19.382   | 800.0    |
| json (stdlib) | mixed        | 25.054   | 27.763      | 28.024   | 800.0    |
| strata        | nested       | 5.240    | 6.395       | 6.552    | 503.8    |
| msgspec       | nested       | 13.914   | 17.792      | 18.061   | 505.5    |
| orjson        | nested       | 14.671   | 18.473      | 19.577   | 503.8    |
| ujson         | nested       | 17.093   | 22.105      | 22.955   | 505.5    |
| json (stdlib) | nested       | 21.919   | 26.625      | 26.653   | 505.5    |
| strata        | users.json   | 151.631  | 189.546     | 191.663  | 522.3    |
| orjson        | users.json   | 190.581  | 261.926     | 267.130  | 522.3    |
| msgspec       | users.json   | 203.908  | 282.171     | 286.274  | 555.4    |
| ujson         | users.json   | 282.711  | 383.820     | 393.020  | 677.9    |
| json (stdlib) | users.json   | 393.772  | 473.044     | 478.143  | 555.4    |
| strata        | users.ndjson | 166.917  | 210.731     | 219.033  | 612.3    |
| orjson        | users.ndjson | 258.378  | 312.147     | 315.676  | 612.4    |
| msgspec       | users.ndjson | 260.812  | 315.886     | 323.541  | 650.7    |
| ujson         | users.ndjson | 357.940  | 436.045     | 471.894  | 773.2    |
| json (stdlib) | users.ndjson | 439.238  | 501.742     | 519.196  | 653.3    |
| strata        | wide_arrays  | 31.560   | 38.236      | 40.242   | 477.7    |
| orjson        | wide_arrays  | 29.939   | 40.044      | 41.273   | 478.7    |
| msgspec       | wide_arrays  | 38.553   | 47.538      | 49.941   | 493.8    |
| ujson         | wide_arrays  | 65.005   | 73.750      | 75.627   | 492.8    |
| json (stdlib) | wide_arrays  | 128.327  | 137.633     | 139.341  | 494.8    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.453    | 2.489       | 2.701    | 508.4    |
| msgspec | flat        | 2.861    | 2.898       | 2.908    | 504.0    |
| orjson  | flat        | 2.922    | 3.029       | 3.104    | 504.0    |
| ujson   | flat        | 11.993   | 12.202      | 12.291   | 506.0    |
| json    | flat        | 22.670   | 23.016      | 23.731   | 506.0    |
| msgspec | mixed       | 2.748    | 2.773       | 3.043    | 802.4    |
| strata  | mixed       | 2.838    | 2.878       | 2.899    | 800.0    |
| orjson  | mixed       | 3.029    | 3.044       | 3.119    | 798.1    |
| ujson   | mixed       | 12.366   | 12.410      | 12.566   | 802.4    |
| json    | mixed       | 27.941   | 27.983      | 28.201   | 809.8    |
| orjson  | nested      | 1.929    | 1.942       | 2.003    | 505.5    |
| strata  | nested      | 1.936    | 1.943       | 1.982    | 505.5    |
| msgspec | nested      | 1.949    | 1.963       | 2.006    | 505.5    |
| ujson   | nested      | 9.938    | 9.960       | 10.102   | 505.5    |
| json    | nested      | 12.901   | 13.043      | 13.106   | 505.5    |
| strata  | users.json  | 50.969   | 51.012      | 51.160   | 590.9    |
| msgspec | users.json  | 76.669   | 76.801      | 77.101   | 943.1    |
| orjson  | users.json  | 78.011   | 78.229      | 78.752   | 747.6    |
| ujson   | users.json  | 289.188  | 291.217     | 299.232  | 1139.5   |
| json    | users.json  | 428.380  | 430.105     | 431.876  | 1216.2   |
| strata  | wide_arrays | 17.468   | 18.069      | 18.198   | 493.8    |
| msgspec | wide_arrays | 17.743   | 18.093      | 19.291   | 539.2    |
| orjson  | wide_arrays | 18.495   | 18.604      | 18.608   | 502.4    |
| ujson   | wide_arrays | 73.004   | 74.985      | 75.172   | 585.2    |
| json    | wide_arrays | 200.973  | 203.065     | 204.282  | 637.3    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 2.881    | 2.931       | 3.042    | 506.0    |
| msgspec       | flat        | 3.440    | 3.624       | 3.844    | 507.8    |
| orjson        | flat        | 3.347    | 3.877       | 3.881    | 505.8    |
| ujson         | flat        | 13.239   | 14.884      | 29.086   | 507.8    |
| json (stdlib) | flat        | 84.048   | 84.409      | 84.530   | 507.8    |
| msgspec       | mixed       | 3.392    | 3.397       | 3.475    | 809.6    |
| strata        | mixed       | 3.265    | 3.754       | 3.945    | 809.8    |
| orjson        | mixed       | 3.601    | 3.807       | 4.173    | 809.6    |
| ujson         | mixed       | 13.197   | 13.227      | 13.297   | 809.6    |
| json (stdlib) | mixed       | 98.449   | 99.311      | 99.791   | 809.6    |
| strata        | nested      | 2.267    | 2.268       | 2.269    | 505.5    |
| orjson        | nested      | 2.431    | 2.475       | 2.691    | 505.5    |
| msgspec       | nested      | 2.491    | 2.557       | 2.806    | 505.5    |
| ujson         | nested      | 10.340   | 10.473      | 10.864   | 505.5    |
| json (stdlib) | nested      | 118.539  | 119.187     | 121.181  | 505.5    |
| strata        | users.json  | 58.730   | 59.015      | 61.017   | 1260.0   |
| orjson        | users.json  | 87.704   | 89.479      | 105.342  | 1415.8   |
| msgspec       | users.json  | 89.251   | 102.621     | 105.586  | 1688.5   |
| ujson         | users.json  | 322.159  | 335.434     | 345.524  | 1571.6   |
| json (stdlib) | users.json  | 3028.673 | 3038.291    | 3238.666 | 1532.8   |
| orjson        | wide_arrays | 19.024   | 19.297      | 19.848   | 686.5    |
| strata        | wide_arrays | 19.629   | 19.872      | 19.885   | 637.3    |
| msgspec       | wide_arrays | 20.459   | 20.495      | 20.825   | 784.7    |
| ujson         | wide_arrays | 76.689   | 76.840      | 77.493   | 735.5    |
| json (stdlib) | wide_arrays | 447.369  | 450.911     | 452.082  | 784.7    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 66.925   | 4000    | 1332.6   |
|                                | orjson+jsonpath-ng | 206.102  | 4000    | 1692.9   |
|                                | orjson+jmespath    | 206.994  | 4000    | 1685.0   |
| all item prices                | strata             | 72.962   | 789913  | 1717.9   |
|                                | orjson+jmespath    | 533.015  | 4000    | 1311.8   |
|                                | orjson+jsonpath-ng | 1885.149 | 789913  | 946.9    |
| recursive price                | strata             | 73.814   | 789913  | 1060.3   |
|                                | orjson+jsonpath-ng | 6237.983 | 789913  | 830.9    |
| flat:$.records\[\*\].id        | strata             | 3.736    | 5000    | 507.8    |
|                                | orjson+jmespath    | 11.664   | 5000    | 507.8    |
|                                | orjson+jsonpath-ng | 17.955   | 5000    | 504.8    |
| flat:$.records\[0\]            | strata             | 3.700    | 1       | 504.8    |
|                                | orjson+jsonpath-ng | 9.515    | 1       | 504.8    |
|                                | orjson+jmespath    | 10.274   | 1       | 504.8    |
| nested:$.records\[\*\].id      | strata             | 2.783    | 5000    | 505.5    |
|                                | orjson+jmespath    | 17.593   | 5000    | 505.5    |
|                                | orjson+jsonpath-ng | 24.214   | 5000    | 504.5    |
| nested:$.records\[0\]          | strata             | 2.696    | 1       | 504.5    |
|                                | orjson+jsonpath-ng | 14.828   | 1       | 504.5    |
|                                | orjson+jmespath    | 15.427   | 1       | 504.5    |
| wide_arrays:$.records\[\*\].id | strata             | 23.966   | 5000    | 784.7    |
|                                | orjson+jmespath    | 37.247   | 5000    | 784.7    |
|                                | orjson+jsonpath-ng | 43.529   | 5000    | 784.7    |
| wide_arrays:$.records\[0\]     | strata             | 23.680   | 1       | 784.7    |
|                                | orjson+jsonpath-ng | 34.887   | 1       | 784.7    |
|                                | orjson+jmespath    | 36.172   | 1       | 784.7    |
| mixed:$.records\[\*\].id       | strata             | 4.116    | 5000    | 809.6    |
|                                | orjson+jmespath    | 12.703   | 5000    | 809.6    |
|                                | orjson+jsonpath-ng | 18.310   | 5000    | 809.6    |
| mixed:$.records\[0\]           | strata             | 3.887    | 1       | 809.6    |
|                                | orjson+jsonpath-ng | 10.973   | 1       | 809.6    |
|                                | orjson+jmespath    | 11.171   | 1       | 809.6    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.785    | 4000    | 1020.5   |
|                                            | jmespath    | 2.290    | 4000    | 1020.5   |
|                                            | jsonpath-ng | 81.897   | 4000    | 1022.2   |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 45.610   | 789913  | 1022.2   |
|                                            | jmespath    | 399.854  | 4000    | 765.3    |
|                                            | jsonpath-ng | 1672.173 | 789913  | 764.2    |
| $..price                                   | strata      | 57.419   | 789913  | 778.9    |
|                                            | jsonpath-ng | 6165.451 | 789913  | 777.7    |
| $.users\[?(@.age>30)\]                     | strata      | 0.838    | 3212    | 780.2    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 60.689   | 26406   | 780.2    |
| flat:$.records\[\*\].id                    | strata      | 0.324    | 5000    | 504.8    |
|                                            | jmespath    | 1.715    | 5000    | 504.8    |
|                                            | jsonpath-ng | 9.376    | 5000    | 504.8    |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 504.8    |
|                                            | jsonpath-ng | 0.008    | 1       | 504.8    |
|                                            | jmespath    | 0.015    | 1       | 504.8    |
| nested:$.records\[\*\].id                  | strata      | 0.427    | 5000    | 504.5    |
|                                            | jmespath    | 1.743    | 5000    | 504.5    |
|                                            | jsonpath-ng | 14.941   | 5000    | 504.5    |
| nested:$.records\[0\]                      | strata      | 0.010    | 1       | 504.5    |
|                                            | jmespath    | 0.016    | 1       | 504.5    |
|                                            | jsonpath-ng | 0.017    | 1       | 504.5    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.263    | 5000    | 784.7    |
|                                            | jmespath    | 1.758    | 5000    | 784.7    |
|                                            | jsonpath-ng | 16.926   | 5000    | 784.7    |
| wide_arrays:$.records\[0\]                 | strata      | 0.011    | 1       | 784.7    |
|                                            | jmespath    | 0.012    | 1       | 784.7    |
|                                            | jsonpath-ng | 0.023    | 1       | 784.7    |
| mixed:$.records\[\*\].id                   | strata      | 0.353    | 5000    | 809.6    |
|                                            | jmespath    | 1.790    | 5000    | 809.6    |
|                                            | jsonpath-ng | 12.290   | 5000    | 809.6    |
| mixed:$.records\[0\]                       | strata      | 0.005    | 1       | 809.6    |
|                                            | jmespath    | 0.010    | 1       | 809.6    |
|                                            | jsonpath-ng | 0.011    | 1       | 809.6    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **27.6% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **51.3% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **25.7% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **54.8% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **50.4% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **49.3% faster** than #2 (orjson)   |
| loads (flat)        | **#1** / 5              | **33.0% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **31.1% faster** than #2 (orjson)   |
| dumps (flat)        | **#1** / 5              | **16.6% faster** than #2 (msgspec)  |
| dump (flat)         | **#1** / 5              | **16.2% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **164.3% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **165.5% faster** than #2 (msgspec) |
| dumps (nested)      | **#2** / 5              | 0.4% behind #1 (orjson)             |
| dump (nested)       | **#1** / 5              | **7.2% faster** than #2 (orjson)    |
| loads (wide_arrays) | **#1** / 5              | **0.5% faster** than #2 (orjson)    |
| load (wide_arrays)  | **#2** / 5              | 5.4% behind #1 (orjson)             |
| dumps (wide_arrays) | **#1** / 5              | **1.6% faster** than #2 (msgspec)   |
| dump (wide_arrays)  | **#2** / 5              | 3.2% behind #1 (orjson)             |
| loads (mixed)       | **#1** / 5              | **54.2% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **58.3% faster** than #2 (orjson)   |
| dumps (mixed)       | **#2** / 5              | 3.3% behind #1 (msgspec)            |
| dump (mixed)        | **#1** / 5              | **3.9% faster** than #2 (msgspec)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **1003% faster**                |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
