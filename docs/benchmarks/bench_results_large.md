# Strata Benchmark Results

Generated: 2026-03-18 00:43:18

## Environment

- Commit: 5b6b37b39dfd1e18b3f00b8af9c26aa1e984d69c
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
| strata        | flat         | 7.081    | 8.302       | 8.627    | 499.5    |
| orjson        | flat         | 8.210    | 10.013      | 10.077   | 499.5    |
| msgspec       | flat         | 11.144   | 12.229      | 12.681   | 499.5    |
| ujson         | flat         | 14.334   | 16.868      | 17.138   | 499.5    |
| json (stdlib) | flat         | 23.469   | 24.613      | 25.451   | 502.5    |
| strata        | mixed        | 7.020    | 8.590       | 8.675    | 756.9    |
| msgspec       | mixed        | 10.385   | 12.840      | 20.738   | 762.5    |
| orjson        | mixed        | 12.655   | 12.992      | 20.081   | 762.5    |
| ujson         | mixed        | 14.324   | 17.821      | 17.930   | 762.5    |
| json (stdlib) | mixed        | 24.213   | 26.962      | 27.606   | 764.9    |
| strata        | nested       | 6.491    | 7.532       | 7.672    | 497.9    |
| msgspec       | nested       | 15.463   | 19.370      | 38.774   | 497.9    |
| orjson        | nested       | 18.859   | 21.316      | 23.109   | 497.9    |
| ujson         | nested       | 15.838   | 22.371      | 38.501   | 497.9    |
| json (stdlib) | nested       | 22.487   | 27.857      | 44.054   | 497.9    |
| strata        | users.json   | 197.397  | 218.325     | 227.216  | 340.1    |
| orjson        | users.json   | 205.422  | 284.000     | 294.342  | 478.2    |
| msgspec       | users.json   | 221.013  | 301.175     | 303.568  | 467.7    |
| ujson         | users.json   | 281.348  | 412.618     | 417.292  | 590.1    |
| json (stdlib) | users.json   | 422.212  | 491.112     | 509.976  | 511.6    |
| strata        | users.ndjson | 188.330  | 233.714     | 246.708  | 558.0    |
| orjson        | users.ndjson | 277.454  | 318.324     | 337.331  | 608.2    |
| msgspec       | users.ndjson | 290.913  | 332.662     | 353.790  | 602.7    |
| ujson         | users.ndjson | 360.491  | 461.957     | 484.692  | 725.2    |
| json (stdlib) | users.ndjson | 471.484  | 547.787     | 548.461  | 605.4    |
| orjson        | wide_arrays  | 31.844   | 38.825      | 40.340   | 452.2    |
| strata        | wide_arrays  | 33.524   | 40.346      | 42.441   | 426.7    |
| msgspec       | wide_arrays  | 39.373   | 48.598      | 48.663   | 453.3    |
| ujson         | wide_arrays  | 59.699   | 74.176      | 76.313   | 453.3    |
| json (stdlib) | wide_arrays  | 131.638  | 141.982     | 142.434  | 467.3    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 7.810    | 9.214       | 9.794    | 502.5    |
| orjson        | flat         | 9.852    | 10.912      | 11.478   | 502.5    |
| msgspec       | flat         | 12.086   | 13.304      | 15.169   | 505.4    |
| ujson         | flat         | 14.937   | 18.438      | 18.691   | 505.4    |
| json (stdlib) | flat         | 23.494   | 25.407      | 25.530   | 505.4    |
| strata        | mixed        | 7.913    | 9.192       | 9.279    | 764.9    |
| orjson        | mixed        | 12.833   | 13.107      | 13.295   | 764.9    |
| msgspec       | mixed        | 10.875   | 13.323      | 13.648   | 767.3    |
| ujson         | mixed        | 16.794   | 18.782      | 26.470   | 767.3    |
| json (stdlib) | mixed        | 27.964   | 28.033      | 30.773   | 767.3    |
| strata        | nested       | 7.845    | 8.399       | 13.260   | 497.9    |
| orjson        | nested       | 18.733   | 20.286      | 20.868   | 499.7    |
| ujson         | nested       | 17.402   | 23.261      | 40.765   | 502.1    |
| msgspec       | nested       | 19.297   | 23.515      | 31.922   | 502.1    |
| json (stdlib) | nested       | 22.932   | 27.054      | 47.012   | 502.1    |
| strata        | users.json   | 185.817  | 224.732     | 226.560  | 522.5    |
| orjson        | users.json   | 224.964  | 282.874     | 310.183  | 522.5    |
| msgspec       | users.json   | 204.491  | 309.371     | 336.151  | 555.6    |
| ujson         | users.json   | 297.856  | 410.216     | 419.571  | 678.1    |
| json (stdlib) | users.json   | 408.773  | 509.057     | 509.826  | 555.6    |
| strata        | users.ndjson | 194.349  | 236.690     | 240.813  | 608.3    |
| orjson        | users.ndjson | 264.448  | 346.809     | 369.999  | 608.5    |
| msgspec       | users.ndjson | 291.641  | 356.168     | 358.378  | 646.7    |
| ujson         | users.ndjson | 387.711  | 489.577     | 496.756  | 769.2    |
| json (stdlib) | users.ndjson | 476.343  | 523.637     | 527.094  | 648.4    |
| orjson        | wide_arrays  | 30.162   | 41.286      | 43.718   | 470.3    |
| strata        | wide_arrays  | 34.280   | 43.480      | 45.900   | 469.3    |
| msgspec       | wide_arrays  | 44.077   | 49.733      | 53.199   | 485.3    |
| ujson         | wide_arrays  | 63.247   | 75.874      | 77.001   | 484.3    |
| json (stdlib) | wide_arrays  | 128.326  | 140.787     | 141.266  | 486.3    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.232    | 2.237       | 2.315    | 505.4    |
| msgspec | flat        | 3.028    | 3.044       | 3.063    | 501.1    |
| orjson  | flat        | 2.971    | 3.274       | 3.415    | 501.1    |
| ujson   | flat        | 12.100   | 12.162      | 12.921   | 503.1    |
| json    | flat        | 22.536   | 22.679      | 22.960   | 503.0    |
| strata  | mixed       | 2.467    | 2.486       | 2.503    | 767.3    |
| msgspec | mixed       | 2.820    | 2.832       | 2.999    | 767.6    |
| orjson  | mixed       | 3.251    | 3.276       | 3.720    | 763.0    |
| ujson   | mixed       | 12.499   | 12.649      | 12.707   | 769.8    |
| json    | mixed       | 28.125   | 28.139      | 28.181   | 777.2    |
| strata  | nested      | 1.845    | 1.861       | 1.872    | 502.1    |
| msgspec | nested      | 1.966    | 1.970       | 2.223    | 502.1    |
| orjson  | nested      | 1.991    | 2.011       | 2.075    | 502.1    |
| ujson   | nested      | 10.058   | 10.137      | 10.287   | 505.4    |
| json    | nested      | 13.057   | 13.071      | 13.153   | 505.4    |
| strata  | users.json  | 45.107   | 46.996      | 52.459   | 591.1    |
| orjson  | users.json  | 82.331   | 83.696      | 85.389   | 747.9    |
| msgspec | users.json  | 83.591   | 84.407      | 85.342   | 943.4    |
| ujson   | users.json  | 299.695  | 301.030     | 321.825  | 1140.2   |
| json    | users.json  | 424.616  | 424.930     | 425.795  | 1354.9   |
| strata  | wide_arrays | 13.658   | 13.662      | 13.672   | 484.3    |
| msgspec | wide_arrays | 17.022   | 17.208      | 17.303   | 517.5    |
| orjson  | wide_arrays | 17.678   | 17.844      | 19.860   | 493.0    |
| ujson   | wide_arrays | 72.249   | 72.282      | 73.246   | 552.6    |
| json    | wide_arrays | 201.232  | 202.273     | 203.873  | 604.7    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 2.765    | 2.849       | 3.412    | 503.0    |
| orjson        | flat        | 3.599    | 3.726       | 3.984    | 502.9    |
| msgspec       | flat        | 3.619    | 3.796       | 4.135    | 502.9    |
| ujson         | flat        | 12.828   | 13.342      | 13.357   | 502.9    |
| json (stdlib) | flat        | 81.996   | 82.901      | 82.998   | 502.9    |
| strata        | mixed       | 3.572    | 4.272       | 4.368    | 777.2    |
| orjson        | mixed       | 3.885    | 4.319       | 4.423    | 777.1    |
| msgspec       | mixed       | 3.977    | 4.330       | 4.409    | 777.1    |
| ujson         | mixed       | 13.967   | 14.018      | 14.543   | 777.1    |
| json (stdlib) | mixed       | 97.744   | 101.407     | 102.101  | 777.1    |
| strata        | nested      | 2.166    | 2.235       | 2.237    | 505.4    |
| orjson        | nested      | 2.479    | 2.598       | 2.920    | 505.4    |
| msgspec       | nested      | 2.514    | 2.701       | 3.060    | 505.8    |
| ujson         | nested      | 10.310   | 10.430      | 10.876   | 505.8    |
| json (stdlib) | nested      | 119.545  | 119.822     | 121.175  | 505.8    |
| strata        | users.json  | 52.486   | 54.931      | 58.972   | 1398.8   |
| msgspec       | users.json  | 91.756   | 93.139      | 97.907   | 1865.9   |
| orjson        | users.json  | 86.091   | 93.461      | 93.692   | 1554.5   |
| ujson         | users.json  | 320.653  | 329.877     | 336.700  | 1710.2   |
| json (stdlib) | users.json  | 3021.122 | 3030.530    | 3058.891 | 1689.1   |
| strata        | wide_arrays | 16.554   | 16.669      | 52.228   | 604.7    |
| orjson        | wide_arrays | 27.313   | 28.005      | 30.064   | 653.9    |
| msgspec       | wide_arrays | 28.169   | 28.653      | 32.332   | 752.0    |
| ujson         | wide_arrays | 84.551   | 86.776      | 86.997   | 702.9    |
| json (stdlib) | wide_arrays | 457.378  | 463.375     | 467.922  | 752.0    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 86.584   | 4000    | 1469.2   |
|                                | orjson+jsonpath-ng | 216.984  | 4000    | 1401.2   |
|                                | orjson+jmespath    | 219.941  | 4000    | 1393.4   |
| all item prices                | strata             | 94.277   | 789913  | 1426.3   |
|                                | orjson+jmespath    | 534.844  | 4000    | 1487.2   |
|                                | orjson+jsonpath-ng | 1998.182 | 789913  | 909.7    |
| recursive price                | strata             | 93.893   | 789913  | 882.9    |
|                                | orjson+jsonpath-ng | 6395.987 | 789913  | 711.9    |
| flat:$.records\[\*\].id        | strata             | 4.733    | 5000    | 502.9    |
|                                | orjson+jmespath    | 11.019   | 5000    | 502.9    |
|                                | orjson+jsonpath-ng | 16.788   | 5000    | 499.9    |
| flat:$.records\[0\]            | strata             | 4.700    | 1       | 499.9    |
|                                | orjson+jsonpath-ng | 9.367    | 1       | 499.9    |
|                                | orjson+jmespath    | 9.785    | 1       | 499.9    |
| nested:$.records\[\*\].id      | strata             | 3.649    | 5000    | 505.8    |
|                                | orjson+jmespath    | 17.161   | 5000    | 505.8    |
|                                | orjson+jsonpath-ng | 23.745   | 5000    | 505.8    |
| nested:$.records\[0\]          | strata             | 3.518    | 1       | 505.8    |
|                                | orjson+jmespath    | 15.109   | 1       | 505.8    |
|                                | orjson+jsonpath-ng | 15.457   | 1       | 505.8    |
| wide_arrays:$.records\[\*\].id | strata             | 26.913   | 5000    | 752.0    |
|                                | orjson+jmespath    | 37.000   | 5000    | 752.0    |
|                                | orjson+jsonpath-ng | 43.703   | 5000    | 752.0    |
| wide_arrays:$.records\[0\]     | strata             | 26.502   | 1       | 752.0    |
|                                | orjson+jsonpath-ng | 34.900   | 1       | 752.1    |
|                                | orjson+jmespath    | 36.894   | 1       | 752.1    |
| mixed:$.records\[\*\].id       | strata             | 4.811    | 5000    | 777.1    |
|                                | orjson+jmespath    | 12.238   | 5000    | 777.1    |
|                                | orjson+jsonpath-ng | 18.256   | 5000    | 777.1    |
| mixed:$.records\[0\]           | strata             | 4.632    | 1       | 777.1    |
|                                | orjson+jsonpath-ng | 10.571   | 1       | 777.1    |
|                                | orjson+jmespath    | 10.760   | 1       | 777.1    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.731    | 4000    | 940.4    |
|                                            | jmespath    | 2.193    | 4000    | 940.4    |
|                                            | jsonpath-ng | 80.745   | 4000    | 942.1    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 43.416   | 789913  | 942.2    |
|                                            | jmespath    | 414.688  | 4000    | 957.1    |
|                                            | jsonpath-ng | 1735.813 | 789913  | 805.4    |
| $..price                                   | strata      | 59.525   | 789913  | 818.7    |
|                                            | jsonpath-ng | 6219.131 | 789913  | 817.5    |
| $.users\[?(@.age>30)\]                     | strata      | 0.825    | 3212    | 821.1    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 63.780   | 26406   | 821.1    |
| flat:$.records\[\*\].id                    | strata      | 0.341    | 5000    | 499.9    |
|                                            | jmespath    | 1.665    | 5000    | 499.9    |
|                                            | jsonpath-ng | 9.609    | 5000    | 499.9    |
| flat:$.records\[0\]                        | strata      | 0.005    | 1       | 499.9    |
|                                            | jsonpath-ng | 0.008    | 1       | 499.9    |
|                                            | jmespath    | 0.016    | 1       | 499.9    |
| nested:$.records\[\*\].id                  | strata      | 0.467    | 5000    | 505.8    |
|                                            | jmespath    | 1.750    | 5000    | 505.8    |
|                                            | jsonpath-ng | 16.561   | 5000    | 505.8    |
| nested:$.records\[0\]                      | strata      | 0.007    | 1       | 505.8    |
|                                            | jmespath    | 0.016    | 1       | 505.8    |
|                                            | jsonpath-ng | 0.018    | 1       | 505.8    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.260    | 5000    | 752.1    |
|                                            | jmespath    | 1.697    | 5000    | 752.1    |
|                                            | jsonpath-ng | 16.425   | 5000    | 752.1    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 752.1    |
|                                            | jmespath    | 0.009    | 1       | 752.1    |
|                                            | jsonpath-ng | 0.017    | 1       | 752.1    |
| mixed:$.records\[\*\].id                   | strata      | 0.313    | 5000    | 777.1    |
|                                            | jmespath    | 1.695    | 5000    | 777.1    |
|                                            | jsonpath-ng | 12.200   | 5000    | 777.1    |
| mixed:$.records\[0\]                       | strata      | 0.004    | 1       | 777.1    |
|                                            | jsonpath-ng | 0.011    | 1       | 777.1    |
|                                            | jmespath    | 0.011    | 1       | 777.1    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **4.1% faster** than #2 (orjson)    |
| loads (NDJSON)      | **#1** / 5              | **47.3% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **10.0% faster** than #2 (msgspec)  |
| load (NDJSON file)  | **#1** / 5              | **36.1% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **82.5% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **64.0% faster** than #2 (orjson)   |
| loads (flat)        | **#1** / 5              | **15.9% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **26.1% faster** than #2 (orjson)   |
| dumps (flat)        | **#1** / 5              | **33.1% faster** than #2 (orjson)   |
| dump (flat)         | **#1** / 5              | **30.2% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **138.2% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **121.8% faster** than #2 (ujson)   |
| dumps (nested)      | **#1** / 5              | **6.5% faster** than #2 (msgspec)   |
| dump (nested)       | **#1** / 5              | **14.4% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#2** / 5              | 5.3% behind #1 (orjson)             |
| load (wide_arrays)  | **#2** / 5              | 13.7% behind #1 (orjson)            |
| dumps (wide_arrays) | **#1** / 5              | **24.6% faster** than #2 (msgspec)  |
| dump (wide_arrays)  | **#1** / 5              | **65.0% faster** than #2 (orjson)   |
| loads (mixed)       | **#1** / 5              | **47.9% faster** than #2 (msgspec)  |
| load (mixed)        | **#1** / 5              | **37.4% faster** than #2 (msgspec)  |
| dumps (mixed)       | **#1** / 5              | **14.3% faster** than #2 (msgspec)  |
| dump (mixed)        | **#1** / 5              | **8.8% faster** than #2 (orjson)    |
| search (JSONPath)   | **#1** in 11/11 queries | avg **783% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
