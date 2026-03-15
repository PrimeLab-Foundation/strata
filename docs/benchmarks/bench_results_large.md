# Strata Benchmark Results

Generated: 2026-03-15 19:05:07

## Environment

- Commit: 07f85a0f96b182dd7aeee9835dc618399d95321a
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
| strata        | flat         | 8.042    | 9.141       | 9.310    | 361.8    |
| orjson        | flat         | 8.091    | 9.456       | 10.106   | 368.0    |
| msgspec       | flat         | 10.130   | 10.141      | 10.675   | 368.0    |
| ujson         | flat         | 13.918   | 15.832      | 22.336   | 368.0    |
| json (stdlib) | flat         | 22.670   | 27.486      | 27.639   | 370.9    |
| strata        | mixed        | 6.435    | 7.987       | 8.266    | 669.4    |
| orjson        | mixed        | 9.507    | 12.420      | 15.099   | 677.0    |
| msgspec       | mixed        | 12.057   | 12.841      | 15.683   | 677.0    |
| ujson         | mixed        | 14.717   | 16.990      | 17.605   | 677.0    |
| json (stdlib) | mixed        | 25.836   | 26.202      | 29.491   | 679.4    |
| strata        | nested       | 6.633    | 7.959       | 8.114    | 372.7    |
| orjson        | nested       | 14.178   | 18.247      | 40.771   | 372.7    |
| msgspec       | nested       | 13.132   | 18.470      | 47.161   | 372.7    |
| ujson         | nested       | 16.140   | 21.204      | 41.335   | 372.7    |
| json (stdlib) | nested       | 21.046   | 26.109      | 31.901   | 372.7    |
| strata        | users.json   | 199.397  | 241.492     | 261.174  | 339.9    |
| orjson        | users.json   | 248.474  | 291.361     | 366.905  | 475.0    |
| msgspec       | users.json   | 248.200  | 292.580     | 317.515  | 464.7    |
| ujson         | users.json   | 326.969  | 501.014     | 536.525  | 587.1    |
| json (stdlib) | users.json   | 433.131  | 511.121     | 522.131  | 508.6    |
| strata        | users.ndjson | 189.658  | 240.240     | 241.947  | 473.8    |
| orjson        | users.ndjson | 250.482  | 325.189     | 328.100  | 523.9    |
| msgspec       | users.ndjson | 273.041  | 339.684     | 357.261  | 518.4    |
| ujson         | users.ndjson | 380.230  | 444.463     | 490.894  | 640.9    |
| json (stdlib) | users.ndjson | 454.135  | 525.004     | 551.826  | 521.1    |
| strata        | wide_arrays  | 25.337   | 31.974      | 34.512   | 299.2    |
| orjson        | wide_arrays  | 36.165   | 37.889      | 40.544   | 324.7    |
| msgspec       | wide_arrays  | 44.692   | 46.337      | 46.746   | 335.5    |
| ujson         | wide_arrays  | 66.291   | 69.632      | 73.152   | 334.5    |
| json (stdlib) | wide_arrays  | 127.140  | 131.629     | 136.960  | 349.5    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 8.432    | 9.387       | 9.404    | 373.9    |
| orjson        | flat         | 8.877    | 10.627      | 15.489   | 373.9    |
| msgspec       | flat         | 9.020    | 10.906      | 17.339   | 376.8    |
| ujson         | flat         | 14.841   | 16.771      | 16.874   | 376.8    |
| json (stdlib) | flat         | 23.086   | 24.674      | 24.793   | 376.8    |
| strata        | mixed        | 6.777    | 8.561       | 8.588    | 679.4    |
| orjson        | mixed        | 9.691    | 12.242      | 12.324   | 679.4    |
| msgspec       | mixed        | 12.733   | 13.120      | 20.271   | 681.8    |
| ujson         | mixed        | 17.614   | 17.719      | 21.252   | 681.8    |
| json (stdlib) | mixed        | 26.817   | 29.843      | 35.443   | 681.8    |
| strata        | nested       | 6.742    | 8.169       | 9.008    | 372.7    |
| orjson        | nested       | 13.425   | 17.422      | 40.288   | 372.7    |
| msgspec       | nested       | 13.599   | 17.806      | 31.498   | 372.8    |
| ujson         | nested       | 15.960   | 21.842      | 40.066   | 372.8    |
| json (stdlib) | nested       | 21.815   | 25.699      | 44.292   | 374.5    |
| strata        | users.json   | 214.777  | 270.742     | 280.838  | 519.6    |
| orjson        | users.json   | 242.710  | 289.518     | 300.571  | 519.7    |
| msgspec       | users.json   | 222.477  | 307.099     | 320.322  | 552.8    |
| ujson         | users.json   | 317.741  | 421.324     | 441.109  | 675.3    |
| json (stdlib) | users.json   | 406.220  | 494.611     | 515.974  | 552.9    |
| strata        | users.ndjson | 197.042  | 236.164     | 242.798  | 524.0    |
| orjson        | users.ndjson | 268.925  | 312.219     | 316.491  | 524.1    |
| msgspec       | users.ndjson | 263.076  | 336.987     | 360.222  | 511.2    |
| ujson         | users.ndjson | 381.507  | 453.586     | 621.454  | 633.7    |
| json (stdlib) | users.ndjson | 462.598  | 508.781     | 534.341  | 512.9    |
| strata        | wide_arrays  | 28.869   | 36.225      | 37.459   | 361.6    |
| orjson        | wide_arrays  | 39.561   | 40.506      | 40.865   | 362.6    |
| msgspec       | wide_arrays  | 48.851   | 49.112      | 60.601   | 377.6    |
| ujson         | wide_arrays  | 72.529   | 75.418      | 77.000   | 376.6    |
| json (stdlib) | wide_arrays  | 136.357  | 139.440     | 146.436  | 378.6    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| msgspec | flat        | 2.809    | 2.813       | 2.958    | 375.9    |
| strata  | flat        | 2.996    | 3.047       | 3.083    | 379.6    |
| orjson  | flat        | 2.833    | 3.143       | 3.168    | 375.8    |
| ujson   | flat        | 11.840   | 12.070      | 12.126   | 377.9    |
| json    | flat        | 22.007   | 22.197      | 22.406   | 377.9    |
| msgspec | mixed       | 2.708    | 2.944       | 2.957    | 682.0    |
| orjson  | mixed       | 2.949    | 3.103       | 3.689    | 677.5    |
| strata  | mixed       | 3.047    | 3.135       | 3.240    | 681.8    |
| ujson   | mixed       | 12.189   | 12.275      | 12.388   | 684.3    |
| json    | mixed       | 26.762   | 26.886      | 26.948   | 691.7    |
| orjson  | nested      | 1.835    | 1.844       | 2.154    | 374.5    |
| strata  | nested      | 1.875    | 1.884       | 2.357    | 374.5    |
| msgspec | nested      | 1.885    | 1.906       | 1.919    | 374.5    |
| ujson   | nested      | 9.583    | 9.709       | 9.778    | 377.7    |
| json    | nested      | 12.453   | 12.507      | 12.588   | 377.7    |
| orjson  | users.json  | 80.210   | 83.150      | 94.308   | 745.1    |
| strata  | users.json  | 85.642   | 86.119      | 88.669   | 588.4    |
| msgspec | users.json  | 85.040   | 87.065      | 90.886   | 941.1    |
| ujson   | users.json  | 315.455  | 317.148     | 331.368  | 1138.0   |
| json    | users.json  | 427.608  | 438.731     | 522.174  | 1029.9   |
| msgspec | wide_arrays | 16.446   | 16.506      | 18.051   | 431.6    |
| orjson  | wide_arrays | 17.296   | 17.671      | 19.303   | 394.8    |
| strata  | wide_arrays | 18.238   | 18.454      | 18.711   | 386.1    |
| ujson   | wide_arrays | 70.742   | 71.420      | 71.501   | 477.6    |
| json    | wide_arrays | 193.385  | 194.576     | 196.622  | 529.8    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 3.690    | 4.076       | 4.264    | 377.9    |
| msgspec       | flat        | 4.283    | 4.705       | 5.901    | 377.7    |
| orjson        | flat        | 4.530    | 5.404       | 7.939    | 377.7    |
| ujson         | flat        | 12.903   | 13.092      | 13.241   | 377.7    |
| json (stdlib) | flat        | 80.561   | 81.850      | 87.581   | 377.7    |
| strata        | mixed       | 3.346    | 3.351       | 3.397    | 691.7    |
| msgspec       | mixed       | 3.218    | 3.709       | 3.823    | 691.5    |
| orjson        | mixed       | 3.535    | 3.888       | 4.272    | 691.5    |
| ujson         | mixed       | 12.632   | 12.749      | 12.893   | 691.5    |
| json (stdlib) | mixed       | 91.768   | 91.776      | 91.900   | 691.5    |
| strata        | nested      | 2.512    | 2.597       | 2.680    | 377.7    |
| orjson        | nested      | 2.736    | 2.890       | 3.611    | 377.8    |
| msgspec       | nested      | 2.829    | 2.927       | 2.980    | 377.8    |
| ujson         | nested      | 10.313   | 10.752      | 11.275   | 377.8    |
| json (stdlib) | nested      | 113.528  | 113.709     | 119.975  | 377.8    |
| orjson        | users.json  | 99.605   | 110.876     | 113.362  | 1135.1   |
| strata        | users.json  | 116.025  | 135.153     | 145.110  | 979.4    |
| msgspec       | users.json  | 162.835  | 163.579     | 190.532  | 1097.8   |
| ujson         | users.json  | 301.171  | 303.756     | 308.754  | 1291.0   |
| json (stdlib) | users.json  | 2979.085 | 3057.297    | 3705.112 | 681.6    |
| strata        | wide_arrays | 20.256   | 22.657      | 27.332   | 529.8    |
| msgspec       | wide_arrays | 24.099   | 24.819      | 26.772   | 672.8    |
| orjson        | wide_arrays | 24.859   | 26.405      | 29.132   | 578.9    |
| ujson         | wide_arrays | 82.587   | 85.842      | 87.129   | 623.7    |
| json (stdlib) | wide_arrays | 435.437  | 436.375     | 438.283  | 672.8    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 86.500   | 4000    | 508.3    |
|                                | orjson+jmespath    | 207.986  | 4000    | 861.0    |
|                                | orjson+jsonpath-ng | 214.230  | 4000    | 869.1    |
| all item prices                | strata             | 96.574   | 789913  | 895.7    |
|                                | orjson+jmespath    | 526.451  | 4000    | 956.6    |
|                                | orjson+jsonpath-ng | 1994.375 | 789913  | 944.5    |
| recursive price                | strata             | 95.554   | 789913  | 917.6    |
|                                | orjson+jsonpath-ng | 6301.862 | 789913  | 654.8    |
| flat:$.records\[\*\].id        | strata             | 4.145    | 5000    | 377.7    |
|                                | orjson+jmespath    | 10.774   | 5000    | 377.7    |
|                                | orjson+jsonpath-ng | 16.837   | 5000    | 374.7    |
| flat:$.records\[0\]            | strata             | 4.019    | 1       | 374.7    |
|                                | orjson+jsonpath-ng | 9.168    | 1       | 374.7    |
|                                | orjson+jmespath    | 9.658    | 1       | 374.7    |
| nested:$.records\[\*\].id      | strata             | 3.395    | 5000    | 377.8    |
|                                | orjson+jmespath    | 15.634   | 5000    | 377.8    |
|                                | orjson+jsonpath-ng | 21.129   | 5000    | 377.8    |
| nested:$.records\[0\]          | strata             | 3.316    | 1       | 377.8    |
|                                | orjson+jsonpath-ng | 13.792   | 1       | 377.8    |
|                                | orjson+jmespath    | 13.824   | 1       | 377.8    |
| wide_arrays:$.records\[\*\].id | strata             | 20.423   | 5000    | 672.8    |
|                                | orjson+jmespath    | 35.291   | 5000    | 672.8    |
|                                | orjson+jsonpath-ng | 42.529   | 5000    | 672.8    |
| wide_arrays:$.records\[0\]     | strata             | 20.173   | 1       | 670.8    |
|                                | orjson+jsonpath-ng | 33.507   | 1       | 664.6    |
|                                | orjson+jmespath    | 40.606   | 1       | 670.8    |
| mixed:$.records\[\*\].id       | strata             | 3.914    | 5000    | 691.5    |
|                                | orjson+jmespath    | 12.763   | 5000    | 691.5    |
|                                | orjson+jsonpath-ng | 17.869   | 5000    | 691.5    |
| mixed:$.records\[0\]           | strata             | 3.812    | 1       | 691.5    |
|                                | orjson+jsonpath-ng | 10.307   | 1       | 691.5    |
|                                | orjson+jmespath    | 11.682   | 1       | 691.5    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.687    | 4000    | 855.0    |
|                                            | jmespath    | 2.158    | 4000    | 855.1    |
|                                            | jsonpath-ng | 77.203   | 4000    | 856.8    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 42.313   | 789913  | 858.8    |
|                                            | jmespath    | 392.090  | 4000    | 873.7    |
|                                            | jsonpath-ng | 1753.355 | 789913  | 676.6    |
| $..price                                   | strata      | 58.881   | 789913  | 690.6    |
|                                            | jsonpath-ng | 6089.667 | 789913  | 689.4    |
| $.users\[?(@.age>30)\]                     | strata      | 0.853    | 3212    | 693.0    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 61.147   | 26406   | 693.0    |
| flat:$.records\[\*\].id                    | strata      | 0.260    | 5000    | 374.7    |
|                                            | jmespath    | 1.653    | 5000    | 374.7    |
|                                            | jsonpath-ng | 9.624    | 5000    | 374.7    |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 374.7    |
|                                            | jmespath    | 0.007    | 1       | 374.7    |
|                                            | jsonpath-ng | 0.008    | 1       | 374.7    |
| nested:$.records\[\*\].id                  | strata      | 0.411    | 5000    | 377.8    |
|                                            | jmespath    | 1.705    | 5000    | 377.8    |
|                                            | jsonpath-ng | 15.723   | 5000    | 377.8    |
| nested:$.records\[0\]                      | strata      | 0.004    | 1       | 377.9    |
|                                            | jmespath    | 0.009    | 1       | 377.9    |
|                                            | jsonpath-ng | 0.009    | 1       | 377.9    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.252    | 5000    | 664.6    |
|                                            | jmespath    | 1.643    | 5000    | 664.6    |
|                                            | jsonpath-ng | 15.814   | 5000    | 664.6    |
| wide_arrays:$.records\[0\]                 | strata      | 0.004    | 1       | 664.6    |
|                                            | jmespath    | 0.009    | 1       | 664.6    |
|                                            | jsonpath-ng | 0.011    | 1       | 664.6    |
| mixed:$.records\[\*\].id                   | strata      | 0.254    | 5000    | 691.5    |
|                                            | jmespath    | 1.630    | 5000    | 691.5    |
|                                            | jsonpath-ng | 11.026   | 5000    | 691.5    |
| mixed:$.records\[0\]                       | strata      | 0.005    | 1       | 691.5    |
|                                            | jsonpath-ng | 0.010    | 1       | 691.5    |
|                                            | jmespath    | 0.010    | 1       | 691.5    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **24.5% faster** than #2 (msgspec) |
| loads (NDJSON)      | **#1** / 5              | **32.1% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **3.6% faster** than #2 (msgspec)  |
| load (NDJSON file)  | **#1** / 5              | **33.5% faster** than #2 (msgspec) |
| dumps               | **#3** / 5              | 6.8% behind #1 (orjson)            |
| dump                | **#2** / 5              | 16.5% behind #1 (orjson)           |
| loads (flat)        | **#1** / 5              | **0.6% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **5.3% faster** than #2 (orjson)   |
| dumps (flat)        | **#3** / 5              | 6.6% behind #1 (msgspec)           |
| dump (flat)         | **#1** / 5              | **16.1% faster** than #2 (msgspec) |
| loads (nested)      | **#1** / 5              | **98.0% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **99.1% faster** than #2 (orjson)  |
| dumps (nested)      | **#2** / 5              | 2.1% behind #1 (orjson)            |
| dump (nested)       | **#1** / 5              | **8.9% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#1** / 5              | **42.7% faster** than #2 (orjson)  |
| load (wide_arrays)  | **#1** / 5              | **37.0% faster** than #2 (orjson)  |
| dumps (wide_arrays) | **#3** / 5              | 10.9% behind #1 (msgspec)          |
| dump (wide_arrays)  | **#1** / 5              | **19.0% faster** than #2 (msgspec) |
| loads (mixed)       | **#1** / 5              | **47.7% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **43.0% faster** than #2 (orjson)  |
| dumps (mixed)       | **#3** / 5              | 12.5% behind #1 (msgspec)          |
| dump (mixed)        | **#2** / 5              | 3.9% behind #1 (msgspec)           |
| search (JSONPath)   | **#1** in 11/11 queries | avg **780% faster**                |
| query (JSONPath)    | **#1** in 13/13 queries | -                                  |
