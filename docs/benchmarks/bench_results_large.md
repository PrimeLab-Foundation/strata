# Strata Benchmark Results

Generated: 2026-03-19 17:17:26

## Environment

- Commit: a14321cee5dc0690142acfa20e3e17374e5476f2
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
| strata        | flat         | 7.286    | 8.290       | 8.535    | 501.4    |
| msgspec       | flat         | 7.807    | 9.330       | 9.691    | 501.4    |
| orjson        | flat         | 7.834    | 9.638       | 10.034   | 501.4    |
| ujson         | flat         | 12.373   | 14.964      | 15.051   | 501.4    |
| json (stdlib) | flat         | 22.561   | 23.763      | 24.314   | 504.3    |
| strata        | mixed        | 7.053    | 8.441       | 8.490    | 767.0    |
| orjson        | mixed        | 9.085    | 11.853      | 12.143   | 772.6    |
| msgspec       | mixed        | 14.143   | 14.462      | 17.344   | 772.6    |
| ujson         | mixed        | 14.117   | 16.908      | 23.239   | 772.6    |
| json (stdlib) | mixed        | 24.431   | 27.047      | 27.151   | 775.0    |
| strata        | nested       | 6.873    | 8.165       | 8.171    | 499.8    |
| orjson        | nested       | 14.152   | 17.434      | 18.185   | 499.8    |
| ujson         | nested       | 18.023   | 21.080      | 43.002   | 499.8    |
| msgspec       | nested       | 21.465   | 23.135      | 24.171   | 499.7    |
| json (stdlib) | nested       | 25.079   | 26.835      | 38.619   | 499.5    |
| strata        | users.json   | 196.266  | 228.968     | 232.206  | 339.9    |
| msgspec       | users.json   | 216.088  | 287.658     | 309.071  | 467.4    |
| orjson        | users.json   | 204.140  | 328.199     | 331.968  | 477.9    |
| ujson         | users.json   | 284.998  | 410.756     | 455.061  | 589.8    |
| json (stdlib) | users.json   | 410.068  | 496.789     | 520.570  | 511.3    |
| strata        | users.ndjson | 190.014  | 230.842     | 234.364  | 560.7    |
| orjson        | users.ndjson | 256.100  | 299.780     | 308.803  | 611.1    |
| msgspec       | users.ndjson | 301.287  | 359.999     | 370.671  | 605.6    |
| ujson         | users.ndjson | 367.050  | 425.895     | 466.482  | 728.1    |
| json (stdlib) | users.ndjson | 465.811  | 501.566     | 562.435  | 608.3    |
| strata        | wide_arrays  | 29.411   | 35.748      | 37.759   | 427.0    |
| orjson        | wide_arrays  | 28.899   | 39.682      | 56.029   | 452.6    |
| msgspec       | wide_arrays  | 36.609   | 46.776      | 48.804   | 462.3    |
| ujson         | wide_arrays  | 59.527   | 72.138      | 79.200   | 461.4    |
| json (stdlib) | wide_arrays  | 126.721  | 141.267     | 148.390  | 476.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 7.602    | 8.549       | 8.683    | 504.3    |
| orjson        | flat         | 8.136    | 9.862       | 9.977    | 504.3    |
| msgspec       | flat         | 8.126    | 10.261      | 10.361   | 507.3    |
| ujson         | flat         | 13.733   | 15.824      | 15.938   | 507.3    |
| json (stdlib) | flat         | 22.802   | 24.132      | 24.267   | 507.3    |
| strata        | mixed        | 7.500    | 8.778       | 8.822    | 775.0    |
| msgspec       | mixed        | 10.469   | 13.035      | 20.526   | 777.4    |
| ujson         | mixed        | 15.353   | 18.963      | 20.876   | 777.4    |
| orjson        | mixed        | 12.868   | 19.659      | 22.658   | 775.0    |
| json (stdlib) | mixed        | 25.001   | 30.069      | 37.628   | 777.4    |
| strata        | nested       | 7.206    | 8.569       | 8.773    | 499.5    |
| msgspec       | nested       | 14.858   | 18.473      | 18.900   | 501.4    |
| ujson         | nested       | 22.035   | 22.971      | 24.323   | 501.4    |
| orjson        | nested       | 16.048   | 24.105      | 40.531   | 499.5    |
| json (stdlib) | nested       | 27.810   | 34.822      | 37.895   | 501.5    |
| strata        | users.json   | 189.015  | 238.606     | 238.704  | 522.1    |
| msgspec       | users.json   | 219.549  | 314.070     | 322.565  | 555.2    |
| orjson        | users.json   | 217.846  | 323.550     | 333.941  | 522.1    |
| ujson         | users.json   | 298.820  | 421.832     | 442.215  | 677.7    |
| json (stdlib) | users.json   | 403.927  | 481.790     | 482.539  | 555.2    |
| strata        | users.ndjson | 195.537  | 235.854     | 245.398  | 611.2    |
| orjson        | users.ndjson | 240.452  | 302.015     | 312.703  | 611.3    |
| msgspec       | users.ndjson | 253.026  | 310.353     | 314.795  | 649.6    |
| ujson         | users.ndjson | 355.705  | 429.594     | 436.646  | 772.0    |
| json (stdlib) | users.ndjson | 438.846  | 485.688     | 500.061  | 651.2    |
| strata        | wide_arrays  | 30.575   | 37.950      | 39.310   | 478.4    |
| orjson        | wide_arrays  | 30.850   | 40.057      | 44.694   | 479.4    |
| msgspec       | wide_arrays  | 38.812   | 49.741      | 63.730   | 494.4    |
| ujson         | wide_arrays  | 68.209   | 79.151      | 89.063   | 493.4    |
| json (stdlib) | wide_arrays  | 129.696  | 138.866     | 141.907  | 495.4    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.239    | 2.240       | 2.254    | 507.3    |
| orjson  | flat        | 2.835    | 2.845       | 2.910    | 502.9    |
| msgspec | flat        | 2.860    | 2.862       | 2.902    | 503.0    |
| ujson   | flat        | 11.977   | 12.024      | 12.050   | 505.0    |
| json    | flat        | 22.550   | 22.586      | 22.605   | 504.9    |
| strata  | mixed       | 2.709    | 2.709       | 2.718    | 777.4    |
| msgspec | mixed       | 2.809    | 2.896       | 3.084    | 777.6    |
| orjson  | mixed       | 3.145    | 3.200       | 3.414    | 773.1    |
| ujson   | mixed       | 12.521   | 12.555      | 12.725   | 779.9    |
| json    | mixed       | 28.254   | 28.329      | 28.624   | 787.3    |
| orjson  | nested      | 1.941    | 1.946       | 2.161    | 501.6    |
| msgspec | nested      | 1.942    | 1.966       | 1.974    | 501.7    |
| strata  | nested      | 1.973    | 1.974       | 1.974    | 501.5    |
| ujson   | nested      | 9.955    | 10.054      | 10.098   | 505.0    |
| json    | nested      | 12.899   | 12.926      | 13.010   | 505.0    |
| strata  | users.json  | 58.453   | 58.584      | 59.310   | 590.8    |
| orjson  | users.json  | 79.367   | 79.478      | 81.827   | 747.5    |
| msgspec | users.json  | 79.587   | 81.506      | 91.056   | 943.0    |
| ujson   | users.json  | 292.289  | 294.405     | 301.208  | 1140.0   |
| json    | users.json  | 419.183  | 422.815     | 427.193  | 1354.6   |
| strata  | wide_arrays | 15.331   | 15.384      | 15.430   | 494.4    |
| msgspec | wide_arrays | 16.907   | 17.069      | 17.563   | 527.6    |
| orjson  | wide_arrays | 18.019   | 18.317      | 20.743   | 503.0    |
| ujson   | wide_arrays | 74.947   | 75.566      | 78.507   | 562.7    |
| json    | wide_arrays | 198.649  | 198.655     | 200.172  | 614.8    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 2.683    | 3.036       | 4.502    | 504.9    |
| msgspec       | flat        | 3.589    | 3.604       | 3.763    | 504.8    |
| orjson        | flat        | 3.426    | 3.656       | 3.937    | 504.7    |
| ujson         | flat        | 12.602   | 12.630      | 12.658   | 504.7    |
| json (stdlib) | flat        | 80.872   | 80.905      | 81.108   | 504.8    |
| strata        | mixed       | 3.068    | 3.095       | 3.712    | 787.3    |
| msgspec       | mixed       | 3.309    | 3.319       | 3.330    | 787.2    |
| orjson        | mixed       | 3.588    | 3.817       | 4.029    | 787.2    |
| ujson         | mixed       | 13.072   | 13.113      | 13.187   | 787.2    |
| json (stdlib) | mixed       | 95.547   | 95.781      | 96.031   | 787.2    |
| strata        | nested      | 2.325    | 2.398       | 2.413    | 505.0    |
| orjson        | nested      | 2.422    | 2.467       | 2.550    | 505.0    |
| msgspec       | nested      | 2.534    | 2.603       | 2.617    | 505.0    |
| ujson         | nested      | 10.279   | 10.285      | 10.927   | 505.0    |
| json (stdlib) | nested      | 117.772  | 118.361     | 119.613  | 505.0    |
| strata        | users.json  | 67.121   | 91.932      | 97.465   | 1398.5   |
| orjson        | users.json  | 117.551  | 120.322     | 129.025  | 1554.2   |
| msgspec       | users.json  | 113.426  | 122.314     | 130.558  | 1263.2   |
| ujson         | users.json  | 331.041  | 331.688     | 337.276  | 1707.8   |
| json (stdlib) | users.json  | 3019.104 | 3034.622    | 3040.810 | 1001.7   |
| strata        | wide_arrays | 19.634   | 21.702      | 23.285   | 614.8    |
| orjson        | wide_arrays | 22.745   | 22.754      | 24.675   | 663.9    |
| msgspec       | wide_arrays | 24.703   | 24.752      | 33.147   | 762.1    |
| ujson         | wide_arrays | 77.252   | 79.188      | 81.361   | 713.0    |
| json (stdlib) | wide_arrays | 445.124  | 450.420     | 453.105  | 762.1    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 87.294   | 4000    | 786.8    |
|                                | orjson+jmespath    | 208.545  | 4000    | 1139.6   |
|                                | orjson+jsonpath-ng | 212.561  | 4000    | 1147.6   |
| all item prices                | strata             | 95.283   | 789913  | 1176.5   |
|                                | orjson+jmespath    | 547.885  | 4000    | 1237.4   |
|                                | orjson+jsonpath-ng | 1965.463 | 789913  | 911.8    |
| recursive price                | strata             | 92.406   | 789913  | 885.6    |
|                                | orjson+jsonpath-ng | 6273.751 | 789913  | 714.7    |
| flat:$.records\[\*\].id        | strata             | 4.686    | 5000    | 504.8    |
|                                | orjson+jmespath    | 11.380   | 5000    | 504.8    |
|                                | orjson+jsonpath-ng | 16.698   | 5000    | 501.8    |
| flat:$.records\[0\]            | strata             | 4.423    | 1       | 501.8    |
|                                | orjson+jsonpath-ng | 9.938    | 1       | 501.8    |
|                                | orjson+jmespath    | 10.058   | 1       | 501.8    |
| nested:$.records\[\*\].id      | strata             | 3.570    | 5000    | 505.0    |
|                                | orjson+jmespath    | 18.423   | 5000    | 505.0    |
|                                | orjson+jsonpath-ng | 21.708   | 5000    | 505.0    |
| nested:$.records\[0\]          | strata             | 3.404    | 1       | 505.0    |
|                                | orjson+jsonpath-ng | 14.369   | 1       | 505.0    |
|                                | orjson+jmespath    | 16.388   | 1       | 505.0    |
| wide_arrays:$.records\[\*\].id | strata             | 22.801   | 5000    | 762.1    |
|                                | orjson+jmespath    | 36.817   | 5000    | 762.1    |
|                                | orjson+jsonpath-ng | 42.564   | 5000    | 762.1    |
| wide_arrays:$.records\[0\]     | strata             | 22.616   | 1       | 762.1    |
|                                | orjson+jsonpath-ng | 34.200   | 1       | 762.1    |
|                                | orjson+jmespath    | 35.646   | 1       | 762.1    |
| mixed:$.records\[\*\].id       | strata             | 4.248    | 5000    | 787.2    |
|                                | orjson+jmespath    | 12.205   | 5000    | 787.2    |
|                                | orjson+jsonpath-ng | 19.413   | 5000    | 787.2    |
| mixed:$.records\[0\]           | strata             | 4.238    | 1       | 787.2    |
|                                | orjson+jsonpath-ng | 10.325   | 1       | 787.2    |
|                                | orjson+jmespath    | 10.495   | 1       | 787.2    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.718    | 4000    | 943.1    |
|                                            | jmespath    | 2.179    | 4000    | 943.1    |
|                                            | jsonpath-ng | 79.260   | 4000    | 944.8    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 43.946   | 789913  | 945.0    |
|                                            | jmespath    | 408.909  | 4000    | 959.8    |
|                                            | jsonpath-ng | 1703.623 | 789913  | 808.1    |
| $..price                                   | strata      | 56.018   | 789913  | 821.4    |
|                                            | jsonpath-ng | 6039.072 | 789913  | 820.2    |
| $.users\[?(@.age>30)\]                     | strata      | 0.827    | 3212    | 823.7    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 60.837   | 26406   | 822.8    |
| flat:$.records\[\*\].id                    | strata      | 0.316    | 5000    | 501.8    |
|                                            | jmespath    | 1.697    | 5000    | 501.8    |
|                                            | jsonpath-ng | 9.437    | 5000    | 501.8    |
| flat:$.records\[0\]                        | strata      | 0.005    | 1       | 501.8    |
|                                            | jmespath    | 0.008    | 1       | 501.8    |
|                                            | jsonpath-ng | 0.010    | 1       | 501.8    |
| nested:$.records\[\*\].id                  | strata      | 0.496    | 5000    | 505.0    |
|                                            | jmespath    | 1.938    | 5000    | 505.0    |
|                                            | jsonpath-ng | 17.015   | 5000    | 505.0    |
| nested:$.records\[0\]                      | strata      | 0.007    | 1       | 505.1    |
|                                            | jmespath    | 0.008    | 1       | 505.1    |
|                                            | jsonpath-ng | 0.012    | 1       | 505.1    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.252    | 5000    | 762.1    |
|                                            | jmespath    | 1.818    | 5000    | 762.1    |
|                                            | jsonpath-ng | 16.142   | 5000    | 762.1    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 762.1    |
|                                            | jmespath    | 0.009    | 1       | 762.1    |
|                                            | jsonpath-ng | 0.011    | 1       | 762.1    |
| mixed:$.records\[\*\].id                   | strata      | 0.301    | 5000    | 787.2    |
|                                            | jmespath    | 2.023    | 5000    | 787.2    |
|                                            | jsonpath-ng | 12.057   | 5000    | 787.2    |
| mixed:$.records\[0\]                       | strata      | 0.004    | 1       | 787.2    |
|                                            | jmespath    | 0.008    | 1       | 787.2    |
|                                            | jsonpath-ng | 0.009    | 1       | 787.2    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **4.0% faster** than #2 (orjson)    |
| loads (NDJSON)      | **#1** / 5              | **34.8% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **15.3% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **23.0% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **35.8% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **69.0% faster** than #2 (msgspec)  |
| loads (flat)        | **#1** / 5              | **7.1% faster** than #2 (msgspec)   |
| load (flat)         | **#1** / 5              | **6.9% faster** than #2 (msgspec)   |
| dumps (flat)        | **#1** / 5              | **26.6% faster** than #2 (orjson)   |
| dump (flat)         | **#1** / 5              | **27.7% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **105.9% faster** than #2 (orjson)  |
| load (nested)       | **#1** / 5              | **106.2% faster** than #2 (msgspec) |
| dumps (nested)      | **#3** / 5              | 1.7% behind #1 (orjson)             |
| dump (nested)       | **#1** / 5              | **4.2% faster** than #2 (orjson)    |
| loads (wide_arrays) | **#2** / 5              | 1.8% behind #1 (orjson)             |
| load (wide_arrays)  | **#1** / 5              | **0.9% faster** than #2 (orjson)    |
| dumps (wide_arrays) | **#1** / 5              | **10.3% faster** than #2 (msgspec)  |
| dump (wide_arrays)  | **#1** / 5              | **15.8% faster** than #2 (orjson)   |
| loads (mixed)       | **#1** / 5              | **28.8% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **39.6% faster** than #2 (msgspec)  |
| dumps (mixed)       | **#1** / 5              | **3.7% faster** than #2 (msgspec)   |
| dump (mixed)        | **#1** / 5              | **7.9% faster** than #2 (msgspec)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **796% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
