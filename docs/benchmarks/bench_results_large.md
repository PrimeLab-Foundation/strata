# Strata Benchmark Results

Generated: 2026-03-16 21:44:51

## Environment

- Commit: c0e3b5a0f341ed8571ff3e143865c7dff53226aa
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
| strata        | flat         | 6.364    | 7.282       | 7.595    | 442.1    |
| msgspec       | flat         | 10.339   | 10.666      | 13.426   | 442.1    |
| orjson        | flat         | 8.567    | 10.722      | 13.651   | 442.1    |
| ujson         | flat         | 13.826   | 16.453      | 16.794   | 442.1    |
| json (stdlib) | flat         | 23.697   | 24.834      | 24.906   | 445.0    |
| strata        | mixed        | 6.091    | 7.514       | 7.882    | 696.1    |
| msgspec       | mixed        | 12.535   | 14.731      | 15.411   | 701.7    |
| ujson         | mixed        | 13.941   | 17.299      | 17.332   | 701.7    |
| orjson        | mixed        | 9.410    | 19.368      | 20.243   | 701.7    |
| json (stdlib) | mixed        | 28.445   | 28.687      | 30.697   | 704.1    |
| strata        | nested       | 5.187    | 6.421       | 6.474    | 446.1    |
| orjson        | nested       | 13.532   | 18.051      | 32.897   | 446.1    |
| msgspec       | nested       | 14.062   | 18.545      | 36.104   | 446.1    |
| ujson         | nested       | 16.750   | 22.000      | 29.388   | 446.1    |
| json (stdlib) | nested       | 27.434   | 32.460      | 32.526   | 446.1    |
| strata        | users.json   | 151.106  | 190.242     | 201.648  | 339.8    |
| orjson        | users.json   | 216.933  | 270.916     | 290.179  | 477.8    |
| msgspec       | users.json   | 217.327  | 292.972     | 295.960  | 467.4    |
| ujson         | users.json   | 281.315  | 407.296     | 422.638  | 589.7    |
| json (stdlib) | users.json   | 418.853  | 488.919     | 500.480  | 511.2    |
| strata        | users.ndjson | 163.537  | 212.321     | 219.233  | 498.0    |
| orjson        | users.ndjson | 278.509  | 319.914     | 355.368  | 548.5    |
| msgspec       | users.ndjson | 286.052  | 343.538     | 373.059  | 543.2    |
| ujson         | users.ndjson | 368.061  | 470.178     | 482.340  | 665.7    |
| json (stdlib) | users.ndjson | 474.397  | 523.012     | 545.829  | 544.9    |
| strata        | wide_arrays  | 29.689   | 36.432      | 39.479   | 367.8    |
| orjson        | wide_arrays  | 38.110   | 39.718      | 40.250   | 393.3    |
| msgspec       | wide_arrays  | 38.335   | 48.824      | 57.267   | 395.4    |
| ujson         | wide_arrays  | 60.245   | 72.448      | 75.098   | 394.4    |
| json (stdlib) | wide_arrays  | 134.568  | 138.741     | 140.573  | 409.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 7.031    | 7.958       | 8.052    | 448.0    |
| orjson        | flat         | 9.337    | 10.522      | 10.547   | 448.0    |
| msgspec       | flat         | 9.852    | 10.875      | 10.914   | 450.9    |
| ujson         | flat         | 15.346   | 17.299      | 18.758   | 450.9    |
| json (stdlib) | flat         | 23.998   | 26.516      | 28.576   | 450.9    |
| strata        | mixed        | 6.699    | 8.227       | 9.264    | 704.1    |
| msgspec       | mixed        | 10.989   | 13.220      | 15.635   | 706.5    |
| orjson        | mixed        | 9.611    | 13.470      | 18.868   | 704.1    |
| ujson         | mixed        | 18.526   | 18.622      | 18.678   | 706.5    |
| json (stdlib) | mixed        | 28.385   | 29.068      | 34.616   | 706.5    |
| strata        | nested       | 5.488    | 6.756       | 7.878    | 446.1    |
| orjson        | nested       | 14.154   | 19.087      | 19.628   | 446.1    |
| msgspec       | nested       | 19.198   | 20.291      | 28.397   | 449.6    |
| ujson         | nested       | 22.715   | 23.682      | 24.202   | 449.6    |
| json (stdlib) | nested       | 27.156   | 27.951      | 33.367   | 449.6    |
| strata        | users.json   | 156.090  | 194.664     | 199.968  | 522.1    |
| orjson        | users.json   | 203.696  | 289.145     | 294.082  | 522.1    |
| msgspec       | users.json   | 216.483  | 310.324     | 322.729  | 555.2    |
| ujson         | users.json   | 307.465  | 416.442     | 435.643  | 677.7    |
| json (stdlib) | users.json   | 405.227  | 494.210     | 507.913  | 555.2    |
| strata        | users.ndjson | 168.326  | 217.132     | 229.168  | 548.8    |
| orjson        | users.ndjson | 293.930  | 316.476     | 330.775  | 549.0    |
| msgspec       | users.ndjson | 289.249  | 338.762     | 358.823  | 587.3    |
| ujson         | users.ndjson | 376.999  | 473.829     | 490.451  | 709.8    |
| json (stdlib) | users.ndjson | 469.621  | 536.323     | 571.719  | 590.0    |
| strata        | wide_arrays  | 30.585   | 39.279      | 41.583   | 421.5    |
| orjson        | wide_arrays  | 29.807   | 40.888      | 48.865   | 422.5    |
| msgspec       | wide_arrays  | 39.139   | 49.171      | 61.757   | 437.5    |
| ujson         | wide_arrays  | 63.792   | 73.970      | 83.812   | 436.5    |
| json (stdlib) | wide_arrays  | 137.629  | 142.804     | 148.465  | 438.5    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.586    | 2.619       | 2.707    | 452.6    |
| msgspec | flat        | 2.898    | 2.989       | 3.097    | 448.3    |
| orjson  | flat        | 3.104    | 3.165       | 3.262    | 448.3    |
| ujson   | flat        | 12.177   | 12.219      | 12.262   | 450.3    |
| json    | flat        | 22.589   | 22.957      | 23.292   | 450.3    |
| strata  | mixed       | 2.827    | 3.066       | 5.761    | 706.5    |
| msgspec | mixed       | 2.813    | 3.086       | 3.114    | 709.1    |
| orjson  | mixed       | 3.046    | 3.176       | 3.374    | 704.6    |
| ujson   | mixed       | 12.486   | 12.552      | 12.730   | 711.3    |
| json    | mixed       | 28.199   | 28.303      | 28.512   | 718.8    |
| orjson  | nested      | 1.965    | 1.976       | 2.541    | 449.8    |
| msgspec | nested      | 1.984    | 1.984       | 2.104    | 449.8    |
| strata  | nested      | 1.970    | 2.000       | 2.024    | 449.8    |
| ujson   | nested      | 10.023   | 10.205      | 10.246   | 449.8    |
| json    | nested      | 13.162   | 13.278      | 13.375   | 449.8    |
| strata  | users.json  | 52.529   | 53.343      | 62.669   | 590.8    |
| msgspec | users.json  | 82.624   | 82.973      | 86.685   | 944.4    |
| orjson  | users.json  | 83.283   | 83.456      | 84.687   | 748.2    |
| ujson   | users.json  | 298.724  | 299.459     | 308.778  | 1139.6   |
| json    | users.json  | 425.404  | 427.011     | 429.908  | 1354.3   |
| msgspec | wide_arrays | 16.992   | 17.302      | 17.370   | 477.2    |
| orjson  | wide_arrays | 17.829   | 17.936      | 18.219   | 452.6    |
| strata  | wide_arrays | 17.919   | 18.318      | 19.098   | 444.0    |
| ujson   | wide_arrays | 73.341   | 73.999      | 74.331   | 512.3    |
| json    | wide_arrays | 202.824  | 202.877     | 207.121  | 564.3    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 3.054    | 3.062       | 3.120    | 450.3    |
| msgspec       | flat        | 3.549    | 3.595       | 3.808    | 450.1    |
| orjson        | flat        | 3.527    | 3.752       | 3.908    | 450.1    |
| ujson         | flat        | 13.076   | 13.213      | 13.576   | 450.1    |
| json (stdlib) | flat        | 81.882   | 82.112      | 82.981   | 450.1    |
| strata        | mixed       | 3.140    | 3.177       | 3.212    | 718.8    |
| msgspec       | mixed       | 3.304    | 3.425       | 3.632    | 723.0    |
| orjson        | mixed       | 3.670    | 3.813       | 4.034    | 720.8    |
| ujson         | mixed       | 13.102   | 13.180      | 13.524   | 723.0    |
| json (stdlib) | mixed       | 96.478   | 98.287      | 98.840   | 723.0    |
| orjson        | nested      | 2.427    | 2.446       | 3.318    | 449.8    |
| strata        | nested      | 2.410    | 2.470       | 3.291    | 449.8    |
| msgspec       | nested      | 2.563    | 2.952       | 3.049    | 449.8    |
| ujson         | nested      | 10.598   | 10.870      | 11.023   | 449.8    |
| json (stdlib) | nested      | 119.819  | 121.392     | 122.186  | 449.8    |
| strata        | users.json  | 58.192   | 59.080      | 68.398   | 1398.2   |
| orjson        | users.json  | 96.570   | 101.031     | 106.464  | 1553.9   |
| msgspec       | users.json  | 90.406   | 103.003     | 108.096  | 1075.5   |
| ujson         | users.json  | 325.101  | 327.866     | 329.626  | 1709.7   |
| json (stdlib) | users.json  | 3005.571 | 3011.239    | 3028.014 | 846.8    |
| orjson        | wide_arrays | 19.089   | 20.323      | 20.849   | 593.0    |
| msgspec       | wide_arrays | 20.321   | 20.539      | 21.563   | 691.3    |
| strata        | wide_arrays | 20.223   | 20.963      | 21.751   | 564.3    |
| ujson         | wide_arrays | 75.506   | 76.225      | 76.440   | 642.1    |
| json (stdlib) | wide_arrays | 450.417  | 452.056     | 452.236  | 691.3    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 57.828   | 4000    | 631.7    |
|                                | orjson+jmespath    | 208.093  | 4000    | 984.5    |
|                                | orjson+jsonpath-ng | 211.212  | 4000    | 992.3    |
| all item prices                | strata             | 67.346   | 789913  | 1017.3   |
|                                | orjson+jmespath    | 532.777  | 4000    | 1078.2   |
|                                | orjson+jsonpath-ng | 1891.370 | 789913  | 1001.6   |
| recursive price                | strata             | 70.258   | 789913  | 976.8    |
|                                | orjson+jsonpath-ng | 6346.346 | 789913  | 651.1    |
| flat:$.records\[\*\].id        | strata             | 3.408    | 5000    | 450.1    |
|                                | orjson+jmespath    | 11.136   | 5000    | 450.1    |
|                                | orjson+jsonpath-ng | 17.048   | 5000    | 447.1    |
| flat:$.records\[0\]            | strata             | 3.235    | 1       | 447.1    |
|                                | orjson+jmespath    | 9.372    | 1       | 447.1    |
|                                | orjson+jsonpath-ng | 9.472    | 1       | 447.1    |
| nested:$.records\[\*\].id      | strata             | 2.454    | 5000    | 449.8    |
|                                | orjson+jmespath    | 16.699   | 5000    | 449.8    |
|                                | orjson+jsonpath-ng | 23.332   | 5000    | 448.8    |
| nested:$.records\[0\]          | strata             | 2.430    | 1       | 448.8    |
|                                | orjson+jsonpath-ng | 15.274   | 1       | 448.8    |
|                                | orjson+jmespath    | 15.339   | 1       | 448.8    |
| wide_arrays:$.records\[\*\].id | strata             | 23.823   | 5000    | 691.3    |
|                                | orjson+jmespath    | 37.356   | 5000    | 691.3    |
|                                | orjson+jsonpath-ng | 45.067   | 5000    | 691.3    |
| wide_arrays:$.records\[0\]     | strata             | 23.528   | 1       | 691.3    |
|                                | orjson+jsonpath-ng | 34.969   | 1       | 691.3    |
|                                | orjson+jmespath    | 40.801   | 1       | 691.3    |
| mixed:$.records\[\*\].id       | strata             | 3.868    | 5000    | 723.0    |
|                                | orjson+jmespath    | 12.601   | 5000    | 723.0    |
|                                | orjson+jsonpath-ng | 19.481   | 5000    | 723.0    |
| mixed:$.records\[0\]           | strata             | 3.712    | 1       | 723.0    |
|                                | orjson+jmespath    | 10.577   | 1       | 723.0    |
|                                | orjson+jsonpath-ng | 10.687   | 1       | 723.0    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.707    | 4000    | 879.6    |
|                                            | jmespath    | 2.195    | 4000    | 879.6    |
|                                            | jsonpath-ng | 82.090   | 4000    | 881.3    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 42.814   | 789913  | 880.7    |
|                                            | jmespath    | 411.490  | 4000    | 896.3    |
|                                            | jsonpath-ng | 1744.139 | 789913  | 745.4    |
| $..price                                   | strata      | 58.814   | 789913  | 758.7    |
|                                            | jsonpath-ng | 6205.231 | 789913  | 757.5    |
| $.users\[?(@.age>30)\]                     | strata      | 0.840    | 3212    | 761.1    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 62.842   | 26406   | 761.1    |
| flat:$.records\[\*\].id                    | strata      | 0.319    | 5000    | 447.1    |
|                                            | jmespath    | 1.703    | 5000    | 447.1    |
|                                            | jsonpath-ng | 9.881    | 5000    | 447.1    |
| flat:$.records\[0\]                        | strata      | 0.005    | 1       | 447.1    |
|                                            | jmespath    | 0.009    | 1       | 447.1    |
|                                            | jsonpath-ng | 0.013    | 1       | 447.1    |
| nested:$.records\[\*\].id                  | strata      | 0.459    | 5000    | 448.8    |
|                                            | jmespath    | 1.782    | 5000    | 448.8    |
|                                            | jsonpath-ng | 15.879   | 5000    | 448.8    |
| nested:$.records\[0\]                      | strata      | 0.008    | 1       | 448.8    |
|                                            | jmespath    | 0.010    | 1       | 448.8    |
|                                            | jsonpath-ng | 0.017    | 1       | 448.8    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.264    | 5000    | 691.3    |
|                                            | jmespath    | 1.874    | 5000    | 691.3    |
|                                            | jsonpath-ng | 16.498   | 5000    | 691.3    |
| wide_arrays:$.records\[0\]                 | strata      | 0.007    | 1       | 691.3    |
|                                            | jmespath    | 0.009    | 1       | 691.3    |
|                                            | jsonpath-ng | 0.011    | 1       | 691.3    |
| mixed:$.records\[\*\].id                   | strata      | 0.333    | 5000    | 723.0    |
|                                            | jmespath    | 1.818    | 5000    | 723.0    |
|                                            | jsonpath-ng | 11.659   | 5000    | 723.0    |
| mixed:$.records\[0\]                       | strata      | 0.005    | 1       | 723.0    |
|                                            | jsonpath-ng | 0.011    | 1       | 723.0    |
|                                            | jmespath    | 0.016    | 1       | 723.0    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **43.6% faster** than #2 (orjson)  |
| loads (NDJSON)      | **#1** / 5              | **70.3% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **30.5% faster** than #2 (orjson)  |
| load (NDJSON file)  | **#1** / 5              | **71.8% faster** than #2 (msgspec) |
| dumps               | **#1** / 5              | **57.3% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **55.4% faster** than #2 (msgspec) |
| loads (flat)        | **#1** / 5              | **34.6% faster** than #2 (orjson)  |
| load (flat)         | **#1** / 5              | **32.8% faster** than #2 (orjson)  |
| dumps (flat)        | **#1** / 5              | **12.1% faster** than #2 (msgspec) |
| dump (flat)         | **#1** / 5              | **15.5% faster** than #2 (orjson)  |
| loads (nested)      | **#1** / 5              | **160.9% faster** than #2 (orjson) |
| load (nested)       | **#1** / 5              | **157.9% faster** than #2 (orjson) |
| dumps (nested)      | **#2** / 5              | 0.2% behind #1 (orjson)            |
| dump (nested)       | **#1** / 5              | **0.7% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#1** / 5              | **28.4% faster** than #2 (orjson)  |
| load (wide_arrays)  | **#2** / 5              | 2.6% behind #1 (orjson)            |
| dumps (wide_arrays) | **#3** / 5              | 5.5% behind #1 (msgspec)           |
| dump (wide_arrays)  | **#2** / 5              | 5.9% behind #1 (orjson)            |
| loads (mixed)       | **#1** / 5              | **54.5% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **43.5% faster** than #2 (orjson)  |
| dumps (mixed)       | **#2** / 5              | 0.5% behind #1 (msgspec)           |
| dump (mixed)        | **#1** / 5              | **5.2% faster** than #2 (msgspec)  |
| search (JSONPath)   | **#1** in 11/11 queries | avg **1084% faster**               |
| query (JSONPath)    | **#1** in 13/13 queries | -                                  |
