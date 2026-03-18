# Strata Benchmark Results

Generated: 2026-03-18 19:20:33

## Environment

- Commit: 23a4c6bba7eea184d34febf8fc1d36f00a2d9935
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
| strata        | flat         | 7.260    | 8.227       | 8.639    | 353.9    |
| orjson        | flat         | 9.614    | 11.013      | 12.235   | 353.9    |
| msgspec       | flat         | 10.167   | 11.668      | 14.271   | 353.9    |
| ujson         | flat         | 15.276   | 17.055      | 18.156   | 353.9    |
| json (stdlib) | flat         | 27.898   | 28.366      | 30.366   | 356.8    |
| strata        | mixed        | 7.865    | 9.346       | 9.779    | 740.5    |
| orjson        | mixed        | 11.527   | 13.363      | 14.271   | 746.1    |
| msgspec       | mixed        | 11.697   | 14.028      | 14.987   | 746.1    |
| ujson         | mixed        | 22.690   | 23.845      | 24.883   | 746.1    |
| json (stdlib) | mixed        | 27.939   | 31.136      | 32.335   | 748.5    |
| strata        | nested       | 7.175    | 8.902       | 8.939    | 361.0    |
| msgspec       | nested       | 15.903   | 23.807      | 43.555   | 361.0    |
| orjson        | nested       | 16.404   | 27.916      | 31.087   | 361.0    |
| json (stdlib) | nested       | 27.786   | 31.198      | 37.138   | 361.0    |
| ujson         | nested       | 19.637   | 32.725      | 39.754   | 361.0    |
| strata        | users.json   | 201.377  | 250.161     | 266.784  | 340.4    |
| orjson        | users.json   | 253.989  | 327.225     | 356.703  | 478.5    |
| msgspec       | users.json   | 277.863  | 371.933     | 401.092  | 468.0    |
| ujson         | users.json   | 365.584  | 451.676     | 585.596  | 590.4    |
| json (stdlib) | users.json   | 574.537  | 615.731     | 735.935  | 511.9    |
| strata        | users.ndjson | 234.009  | 274.534     | 280.727  | 496.5    |
| orjson        | users.ndjson | 355.205  | 409.343     | 436.724  | 547.4    |
| msgspec       | users.ndjson | 400.625  | 415.901     | 490.492  | 540.9    |
| ujson         | users.ndjson | 456.863  | 567.014     | 593.486  | 663.4    |
| json (stdlib) | users.ndjson | 587.314  | 685.573     | 694.987  | 451.7    |
| strata        | wide_arrays  | 33.532   | 40.718      | 42.690   | 305.7    |
| orjson        | wide_arrays  | 34.600   | 47.633      | 50.828   | 332.2    |
| msgspec       | wide_arrays  | 47.445   | 66.872      | 80.052   | 334.3    |
| ujson         | wide_arrays  | 95.137   | 111.325     | 129.402  | 333.3    |
| json (stdlib) | wide_arrays  | 169.165  | 171.692     | 173.665  | 348.3    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 7.964    | 8.949       | 9.058    | 359.8    |
| msgspec       | flat         | 10.246   | 12.086      | 12.779   | 362.7    |
| orjson        | flat         | 9.622    | 15.752      | 16.352   | 359.8    |
| ujson         | flat         | 17.281   | 20.006      | 20.198   | 362.7    |
| json (stdlib) | flat         | 30.910   | 31.138      | 33.565   | 362.7    |
| strata        | mixed        | 7.066    | 8.476       | 8.656    | 748.5    |
| orjson        | mixed        | 16.384   | 16.880      | 18.496   | 748.5    |
| msgspec       | mixed        | 13.809   | 18.414      | 24.961   | 750.9    |
| ujson         | mixed        | 17.708   | 20.335      | 21.746   | 750.9    |
| json (stdlib) | mixed        | 29.332   | 39.530      | 40.733   | 750.9    |
| strata        | nested       | 6.778    | 8.272       | 8.349    | 361.0    |
| msgspec       | nested       | 25.535   | 28.847      | 41.524   | 364.7    |
| orjson        | nested       | 15.344   | 29.208      | 45.628   | 361.0    |
| json (stdlib) | nested       | 31.464   | 34.301      | 91.368   | 365.1    |
| ujson         | nested       | 19.239   | 35.475      | 51.344   | 364.7    |
| strata        | users.json   | 213.964  | 279.557     | 290.642  | 522.7    |
| orjson        | users.json   | 247.196  | 307.520     | 318.145  | 522.7    |
| msgspec       | users.json   | 284.280  | 346.833     | 357.669  | 486.9    |
| ujson         | users.json   | 356.272  | 529.189     | 585.407  | 609.4    |
| json (stdlib) | users.json   | 505.217  | 599.090     | 610.248  | 486.9    |
| strata        | users.ndjson | 240.798  | 282.330     | 289.592  | 454.6    |
| orjson        | users.ndjson | 332.056  | 423.088     | 470.477  | 454.7    |
| msgspec       | users.ndjson | 339.138  | 432.166     | 448.936  | 492.7    |
| ujson         | users.ndjson | 516.515  | 556.990     | 660.481  | 615.2    |
| json (stdlib) | users.ndjson | 580.721  | 638.725     | 711.519  | 495.4    |
| strata        | wide_arrays  | 30.626   | 39.134      | 44.089   | 360.4    |
| orjson        | wide_arrays  | 32.909   | 49.671      | 66.359   | 361.4    |
| msgspec       | wide_arrays  | 46.375   | 54.416      | 56.639   | 376.4    |
| ujson         | wide_arrays  | 80.274   | 84.988      | 109.203  | 375.4    |
| json (stdlib) | wide_arrays  | 163.800  | 168.639     | 169.217  | 377.4    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.562    | 2.571       | 2.624    | 365.4    |
| msgspec | flat        | 3.258    | 3.264       | 3.614    | 361.1    |
| orjson  | flat        | 3.294    | 3.310       | 3.590    | 361.1    |
| ujson   | flat        | 12.617   | 12.926      | 13.682   | 363.2    |
| json    | flat        | 26.196   | 26.578      | 28.932   | 363.1    |
| strata  | mixed       | 2.945    | 3.004       | 3.045    | 750.9    |
| msgspec | mixed       | 3.239    | 3.387       | 3.719    | 751.1    |
| orjson  | mixed       | 3.538    | 3.593       | 3.612    | 746.6    |
| ujson   | mixed       | 14.720   | 14.807      | 15.578   | 753.1    |
| json    | mixed       | 32.579   | 32.957      | 33.955   | 762.7    |
| strata  | nested      | 2.197    | 2.202       | 2.452    | 365.1    |
| msgspec | nested      | 2.204    | 2.260       | 2.440    | 365.1    |
| orjson  | nested      | 2.267    | 2.387       | 2.687    | 365.1    |
| ujson   | nested      | 11.275   | 11.722      | 12.049   | 365.2    |
| json    | nested      | 15.252   | 15.258      | 15.275   | 365.2    |
| strata  | users.json  | 55.553   | 57.402      | 65.166   | 522.5    |
| orjson  | users.json  | 93.628   | 98.154      | 163.350  | 680.8    |
| msgspec | users.json  | 107.541  | 110.486     | 116.522  | 878.7    |
| ujson   | users.json  | 349.367  | 349.615     | 354.725  | 1073.8   |
| json    | users.json  | 479.878  | 484.445     | 491.846  | 1288.6   |
| strata  | wide_arrays | 16.376   | 16.723      | 16.814   | 384.9    |
| orjson  | wide_arrays | 18.294   | 19.889      | 20.645   | 434.0    |
| msgspec | wide_arrays | 19.382   | 20.198      | 24.991   | 483.1    |
| ujson   | wide_arrays | 82.796   | 84.654      | 87.076   | 532.1    |
| json    | wide_arrays | 244.482  | 248.210     | 266.222  | 584.2    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 3.302    | 3.310       | 3.434    | 363.1    |
| msgspec       | flat        | 4.868    | 4.962       | 6.335    | 363.0    |
| orjson        | flat        | 3.932    | 5.513       | 7.038    | 362.9    |
| ujson         | flat        | 15.438   | 15.602      | 18.712   | 363.0    |
| json (stdlib) | flat        | 101.259  | 102.865     | 104.405  | 363.0    |
| strata        | mixed       | 3.352    | 3.384       | 3.793    | 762.7    |
| msgspec       | mixed       | 3.787    | 4.011       | 4.705    | 762.5    |
| orjson        | mixed       | 4.338    | 4.394       | 5.429    | 762.5    |
| ujson         | mixed       | 15.371   | 15.887      | 15.943   | 762.5    |
| json (stdlib) | mixed       | 116.893  | 119.216     | 121.825  | 722.5    |
| msgspec       | nested      | 2.952    | 3.067       | 3.158    | 365.2    |
| orjson        | nested      | 3.083    | 3.131       | 3.268    | 365.2    |
| strata        | nested      | 2.915    | 3.133       | 3.963    | 365.2    |
| ujson         | nested      | 12.101   | 12.763      | 12.789   | 365.2    |
| json (stdlib) | nested      | 139.573  | 145.410     | 163.768  | 365.2    |
| strata        | users.json  | 70.712   | 113.630     | 133.745  | 801.1    |
| msgspec       | users.json  | 125.355  | 126.151     | 126.623  | 1171.9   |
| orjson        | users.json  | 120.604  | 128.656     | 156.191  | 854.0    |
| ujson         | users.json  | 367.269  | 368.554     | 374.440  | 1019.7   |
| json (stdlib) | users.json  | 3526.351 | 3582.573    | 3599.409 | 876.8    |
| strata        | wide_arrays | 19.695   | 20.437      | 21.796   | 584.2    |
| msgspec       | wide_arrays | 22.606   | 22.961      | 22.978   | 725.7    |
| orjson        | wide_arrays | 22.640   | 23.591      | 24.173   | 633.2    |
| ujson         | wide_arrays | 89.027   | 90.370      | 91.643   | 682.2    |
| json (stdlib) | wide_arrays | 551.769  | 555.481     | 558.027  | 725.7    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 104.777  | 4000    | 662.0    |
|                                | orjson+jsonpath-ng | 250.854  | 4000    | 836.8    |
|                                | orjson+jmespath    | 277.570  | 4000    | 1014.6   |
| all item prices                | strata             | 110.338  | 789913  | 860.0    |
|                                | orjson+jmespath    | 614.611  | 4000    | 921.0    |
|                                | orjson+jsonpath-ng | 2280.429 | 789913  | 873.9    |
| recursive price                | strata             | 104.082  | 789913  | 874.4    |
|                                | orjson+jsonpath-ng | 7607.528 | 789913  | 650.5    |
| flat:$.records\[\*\].id        | strata             | 5.595    | 5000    | 365.9    |
|                                | orjson+jmespath    | 13.601   | 5000    | 365.9    |
|                                | orjson+jsonpath-ng | 22.911   | 5000    | 362.9    |
| flat:$.records\[0\]            | strata             | 5.401    | 1       | 363.0    |
|                                | orjson+jmespath    | 12.173   | 1       | 363.0    |
|                                | orjson+jsonpath-ng | 12.309   | 1       | 363.0    |
| nested:$.records\[\*\].id      | strata             | 4.222    | 5000    | 365.2    |
|                                | orjson+jmespath    | 25.194   | 5000    | 365.2    |
|                                | orjson+jsonpath-ng | 25.784   | 5000    | 365.2    |
| nested:$.records\[0\]          | strata             | 4.199    | 1       | 365.2    |
|                                | orjson+jmespath    | 16.455   | 1       | 365.2    |
|                                | orjson+jsonpath-ng | 16.718   | 1       | 365.2    |
| wide_arrays:$.records\[\*\].id | strata             | 26.361   | 5000    | 735.6    |
|                                | orjson+jmespath    | 45.021   | 5000    | 735.6    |
|                                | orjson+jsonpath-ng | 54.273   | 5000    | 735.6    |
| wide_arrays:$.records\[0\]     | strata             | 24.431   | 1       | 735.6    |
|                                | orjson+jsonpath-ng | 39.494   | 1       | 735.6    |
|                                | orjson+jmespath    | 41.648   | 1       | 735.6    |
| mixed:$.records\[\*\].id       | strata             | 4.751    | 5000    | 722.5    |
|                                | orjson+jmespath    | 15.794   | 5000    | 722.5    |
|                                | orjson+jsonpath-ng | 21.337   | 5000    | 722.5    |
| mixed:$.records\[0\]           | strata             | 5.110    | 1       | 722.5    |
|                                | orjson+jmespath    | 13.150   | 1       | 722.5    |
|                                | orjson+jsonpath-ng | 13.979   | 1       | 722.5    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.848    | 4000    | 753.4    |
|                                            | jmespath    | 2.347    | 4000    | 753.7    |
|                                            | jsonpath-ng | 94.039   | 4000    | 755.5    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 49.208   | 789913  | 766.6    |
|                                            | jmespath    | 449.295  | 4000    | 621.2    |
|                                            | jsonpath-ng | 2050.094 | 789913  | 631.9    |
| $..price                                   | strata      | 68.722   | 789913  | 657.2    |
|                                            | jsonpath-ng | 7667.946 | 789913  | 652.3    |
| $.users\[?(@.age>30)\]                     | strata      | 0.885    | 3212    | 671.0    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 86.375   | 26406   | 671.0    |
| flat:$.records\[\*\].id                    | strata      | 0.373    | 5000    | 363.0    |
|                                            | jmespath    | 1.903    | 5000    | 363.0    |
|                                            | jsonpath-ng | 11.466   | 5000    | 362.0    |
| flat:$.records\[0\]                        | strata      | 0.005    | 1       | 362.0    |
|                                            | jmespath    | 0.013    | 1       | 362.0    |
|                                            | jsonpath-ng | 0.031    | 1       | 362.0    |
| nested:$.records\[\*\].id                  | strata      | 0.641    | 5000    | 365.2    |
|                                            | jmespath    | 1.860    | 5000    | 365.2    |
|                                            | jsonpath-ng | 18.808   | 5000    | 365.2    |
| nested:$.records\[0\]                      | strata      | 0.010    | 1       | 365.2    |
|                                            | jsonpath-ng | 0.016    | 1       | 365.2    |
|                                            | jmespath    | 0.019    | 1       | 365.2    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.359    | 5000    | 735.7    |
|                                            | jmespath    | 2.037    | 5000    | 735.7    |
|                                            | jsonpath-ng | 20.617   | 5000    | 735.7    |
| wide_arrays:$.records\[0\]                 | strata      | 0.010    | 1       | 735.7    |
|                                            | jmespath    | 0.017    | 1       | 735.7    |
|                                            | jsonpath-ng | 0.018    | 1       | 735.7    |
| mixed:$.records\[\*\].id                   | strata      | 0.421    | 5000    | 722.5    |
|                                            | jmespath    | 1.911    | 5000    | 722.5    |
|                                            | jsonpath-ng | 14.918   | 5000    | 722.5    |
| mixed:$.records\[0\]                       | strata      | 0.009    | 1       | 722.5    |
|                                            | jmespath    | 0.014    | 1       | 722.5    |
|                                            | jsonpath-ng | 0.019    | 1       | 722.5    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **26.1% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **51.8% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **15.5% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **37.9% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **68.5% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **70.6% faster** than #2 (orjson)   |
| loads (flat)        | **#1** / 5              | **32.4% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **20.8% faster** than #2 (orjson)   |
| dumps (flat)        | **#1** / 5              | **27.1% faster** than #2 (msgspec)  |
| dump (flat)         | **#1** / 5              | **19.1% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **121.6% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **126.4% faster** than #2 (orjson)  |
| dumps (nested)      | **#1** / 5              | **0.3% faster** than #2 (msgspec)   |
| dump (nested)       | **#1** / 5              | **1.3% faster** than #2 (msgspec)   |
| loads (wide_arrays) | **#1** / 5              | **3.2% faster** than #2 (orjson)    |
| load (wide_arrays)  | **#1** / 5              | **7.5% faster** than #2 (orjson)    |
| dumps (wide_arrays) | **#1** / 5              | **11.7% faster** than #2 (orjson)   |
| dump (wide_arrays)  | **#1** / 5              | **14.8% faster** than #2 (msgspec)  |
| loads (mixed)       | **#1** / 5              | **46.5% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **95.4% faster** than #2 (msgspec)  |
| dumps (mixed)       | **#1** / 5              | **10.0% faster** than #2 (msgspec)  |
| dump (mixed)        | **#1** / 5              | **13.0% faster** than #2 (msgspec)  |
| search (JSONPath)   | **#1** in 11/11 queries | avg **853% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
