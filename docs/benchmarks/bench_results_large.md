# Strata Benchmark Results

Generated: 2026-03-20 01:07:53

## Environment

- Commit: aee3e869a1ae51b05b838b86bb2239e87e02b166
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
| strata        | flat         | 7.746    | 9.835       | 9.998    | 331.6    |
| msgspec       | flat         | 11.558   | 13.761      | 13.980   | 331.7    |
| orjson        | flat         | 13.352   | 14.574      | 16.161   | 331.6    |
| ujson         | flat         | 18.388   | 21.589      | 21.819   | 331.7    |
| json (stdlib) | flat         | 31.125   | 33.222      | 34.037   | 334.6    |
| strata        | mixed        | 6.860    | 8.227       | 8.421    | 367.7    |
| orjson        | mixed        | 16.471   | 17.865      | 22.153   | 373.3    |
| msgspec       | mixed        | 14.450   | 17.943      | 18.619   | 373.3    |
| ujson         | mixed        | 19.380   | 24.046      | 24.159   | 373.3    |
| json (stdlib) | mixed        | 34.556   | 39.917      | 46.553   | 375.8    |
| strata        | nested       | 6.283    | 8.258       | 8.672    | 347.9    |
| msgspec       | nested       | 24.021   | 25.369      | 28.605   | 347.9    |
| orjson        | nested       | 18.517   | 27.378      | 27.448   | 347.9    |
| json (stdlib) | nested       | 29.365   | 30.134      | 32.925   | 347.9    |
| ujson         | nested       | 22.673   | 34.388      | 41.607   | 347.9    |
| strata        | users.json   | 180.507  | 223.441     | 412.987  | 362.8    |
| msgspec       | users.json   | 208.511  | 292.574     | 301.566  | 385.1    |
| orjson        | users.json   | 199.782  | 293.657     | 346.570  | 502.1    |
| json (stdlib) | users.json   | 399.007  | 554.444     | 864.752  | 370.1    |
| ujson         | users.json   | 393.824  | 595.286     | 631.759  | 507.4    |
| strata        | users.ndjson | 188.014  | 277.383     | 296.875  | 576.6    |
| msgspec       | users.ndjson | 337.933  | 338.615     | 384.199  | 469.3    |
| orjson        | users.ndjson | 321.098  | 345.055     | 396.577  | 626.7    |
| ujson         | users.ndjson | 399.479  | 466.965     | 853.431  | 591.6    |
| json (stdlib) | users.ndjson | 518.414  | 596.341     | 691.635  | 385.3    |
| strata        | wide_arrays  | 34.862   | 38.998      | 45.511   | 309.1    |
| msgspec       | wide_arrays  | 36.586   | 49.016      | 56.976   | 335.8    |
| orjson        | wide_arrays  | 50.233   | 54.604      | 58.230   | 334.8    |
| ujson         | wide_arrays  | 65.896   | 68.949      | 76.641   | 334.9    |
| json (stdlib) | wide_arrays  | 126.478  | 134.757     | 152.060  | 349.8    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| msgspec       | flat         | 10.679   | 11.831      | 13.529   | 340.5    |
| strata        | flat         | 10.591   | 12.399      | 13.393   | 337.6    |
| orjson        | flat         | 12.700   | 16.323      | 25.740   | 337.6    |
| ujson         | flat         | 22.034   | 22.314      | 24.661   | 340.5    |
| json (stdlib) | flat         | 24.563   | 26.127      | 30.050   | 340.5    |
| strata        | mixed        | 10.387   | 11.383      | 12.302   | 375.8    |
| orjson        | mixed        | 12.527   | 13.769      | 14.992   | 375.8    |
| msgspec       | mixed        | 11.229   | 13.785      | 16.194   | 378.2    |
| ujson         | mixed        | 15.869   | 18.015      | 21.413   | 378.2    |
| json (stdlib) | mixed        | 28.685   | 31.472      | 33.117   | 378.2    |
| strata        | nested       | 6.757    | 7.811       | 8.435    | 347.9    |
| orjson        | nested       | 20.394   | 25.506      | 48.225   | 347.9    |
| msgspec       | nested       | 20.557   | 31.392      | 38.133   | 351.4    |
| ujson         | nested       | 23.547   | 33.774      | 34.271   | 351.4    |
| json (stdlib) | nested       | 34.659   | 34.727      | 36.678   | 351.4    |
| strata        | users.json   | 181.363  | 225.899     | 232.170  | 406.5    |
| orjson        | users.json   | 218.622  | 306.637     | 336.709  | 546.7    |
| msgspec       | users.json   | 226.797  | 329.765     | 344.784  | 579.9    |
| ujson         | users.json   | 376.498  | 468.992     | 513.692  | 702.3    |
| json (stdlib) | users.json   | 515.798  | 565.989     | 599.469  | 579.9    |
| strata        | users.ndjson | 248.743  | 254.147     | 299.115  | 389.0    |
| msgspec       | users.ndjson | 332.761  | 333.121     | 424.690  | 471.8    |
| orjson        | users.ndjson | 276.911  | 383.574     | 404.418  | 433.5    |
| ujson         | users.ndjson | 482.461  | 504.358     | 564.566  | 594.3    |
| json (stdlib) | users.ndjson | 552.816  | 598.393     | 600.298  | 473.5    |
| strata        | wide_arrays  | 26.740   | 35.875      | 39.923   | 361.9    |
| orjson        | wide_arrays  | 38.819   | 42.705      | 52.232   | 362.9    |
| msgspec       | wide_arrays  | 42.045   | 48.446      | 49.599   | 377.9    |
| ujson         | wide_arrays  | 63.616   | 82.077      | 84.759   | 376.9    |
| json (stdlib) | wide_arrays  | 127.931  | 137.887     | 139.459  | 378.9    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.338    | 2.351       | 2.358    | 343.2    |
| msgspec | flat        | 2.871    | 2.918       | 4.273    | 342.5    |
| orjson  | flat        | 2.925    | 2.961       | 3.065    | 342.4    |
| ujson   | flat        | 12.237   | 12.279      | 17.588   | 346.8    |
| json    | flat        | 22.750   | 22.969      | 23.600   | 346.7    |
| strata  | mixed       | 3.054    | 3.516       | 3.537    | 378.2    |
| msgspec | mixed       | 3.897    | 3.901       | 4.131    | 384.8    |
| orjson  | mixed       | 4.097    | 4.140       | 4.144    | 380.8    |
| ujson   | mixed       | 17.273   | 17.288      | 17.558   | 384.8    |
| json    | mixed       | 38.985   | 39.990      | 49.412   | 389.9    |
| msgspec | nested      | 1.943    | 2.005       | 2.098    | 351.4    |
| orjson  | nested      | 2.066    | 2.078       | 2.176    | 351.4    |
| strata  | nested      | 2.210    | 2.413       | 2.662    | 351.4    |
| ujson   | nested      | 10.039   | 10.256      | 11.139   | 354.8    |
| json    | nested      | 12.942   | 19.548      | 21.399   | 354.8    |
| strata  | users.json  | 73.211   | 76.215      | 84.883   | 615.5    |
| orjson  | users.json  | 81.845   | 84.956      | 88.230   | 773.8    |
| msgspec | users.json  | 100.272  | 102.008     | 113.376  | 969.3    |
| ujson   | users.json  | 353.465  | 368.304     | 424.425  | 663.7    |
| json    | users.json  | 447.438  | 453.987     | 523.039  | 836.8    |
| strata  | wide_arrays | 15.306   | 15.760      | 15.851   | 387.4    |
| msgspec | wide_arrays | 17.757   | 17.799      | 18.295   | 428.6    |
| orjson  | wide_arrays | 16.572   | 18.367      | 19.050   | 404.1    |
| ujson   | wide_arrays | 93.340   | 99.535      | 99.760   | 477.6    |
| json    | wide_arrays | 200.267  | 259.563     | 275.688  | 515.8    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 3.030    | 3.159       | 3.422    | 346.7    |
| msgspec       | flat        | 3.463    | 3.467       | 4.077    | 348.9    |
| orjson        | flat        | 3.683    | 4.144       | 4.281    | 346.8    |
| ujson         | flat        | 13.015   | 15.043      | 17.554   | 348.8    |
| json (stdlib) | flat        | 79.584   | 80.976      | 81.124   | 348.9    |
| msgspec       | mixed       | 4.010    | 4.139       | 7.435    | 392.4    |
| strata        | mixed       | 5.330    | 5.554       | 6.155    | 389.9    |
| orjson        | mixed       | 5.582    | 5.678       | 5.748    | 392.2    |
| ujson         | mixed       | 16.823   | 20.876      | 23.501   | 392.4    |
| json (stdlib) | mixed       | 98.610   | 98.739      | 102.760  | 392.4    |
| strata        | nested      | 2.415    | 2.474       | 5.371    | 354.8    |
| orjson        | nested      | 2.705    | 2.763       | 3.280    | 354.8    |
| msgspec       | nested      | 3.046    | 3.170       | 3.467    | 354.8    |
| ujson         | nested      | 12.705   | 12.816      | 12.860   | 354.8    |
| json (stdlib) | nested      | 119.845  | 132.460     | 163.263  | 354.8    |
| strata        | users.json  | 93.609   | 101.890     | 117.290  | 320.0    |
| orjson        | users.json  | 113.391  | 115.892     | 120.637  | 479.1    |
| msgspec       | users.json  | 124.561  | 128.557     | 140.725  | 831.5    |
| ujson         | users.json  | 343.689  | 364.746     | 409.741  | 675.4    |
| json (stdlib) | users.json  | 3308.317 | 3397.223    | 3400.726 | 369.1    |
| msgspec       | wide_arrays | 23.154   | 25.183      | 27.917   | 310.9    |
| strata        | wide_arrays | 25.071   | 25.300      | 25.372   | 515.8    |
| orjson        | wide_arrays | 25.360   | 27.897      | 32.321   | 564.8    |
| ujson         | wide_arrays | 80.385   | 84.660      | 196.335  | 219.5    |
| json (stdlib) | wide_arrays | 488.309  | 494.826     | 567.020  | 311.0    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 116.963  | 4000    | 236.2    |
|                                | orjson+jsonpath-ng | 241.283  | 4000    | 598.7    |
|                                | orjson+jmespath    | 248.895  | 4000    | 590.2    |
| all item prices                | strata             | 99.916   | 789913  | 629.3    |
|                                | orjson+jmespath    | 571.850  | 4000    | 443.3    |
|                                | orjson+jsonpath-ng | 2228.222 | 789913  | 628.3    |
| recursive price                | strata             | 106.329  | 789913  | 465.5    |
|                                | orjson+jsonpath-ng | 7036.503 | 789913  | 722.2    |
| flat:$.records\[\*\].id        | strata             | 4.348    | 5000    | 351.9    |
|                                | orjson+jmespath    | 11.496   | 5000    | 351.9    |
|                                | orjson+jsonpath-ng | 18.474   | 5000    | 348.9    |
| flat:$.records\[0\]            | strata             | 4.374    | 1       | 348.9    |
|                                | orjson+jsonpath-ng | 9.236    | 1       | 348.9    |
|                                | orjson+jmespath    | 9.602    | 1       | 348.9    |
| nested:$.records\[\*\].id      | strata             | 3.694    | 5000    | 354.8    |
|                                | orjson+jmespath    | 22.864   | 5000    | 354.8    |
|                                | orjson+jsonpath-ng | 32.158   | 5000    | 353.8    |
| nested:$.records\[0\]          | strata             | 4.846    | 1       | 353.8    |
|                                | orjson+jsonpath-ng | 15.599   | 1       | 353.8    |
|                                | orjson+jmespath    | 17.873   | 1       | 353.8    |
| wide_arrays:$.records\[\*\].id | strata             | 20.326   | 5000    | 321.1    |
|                                | orjson+jmespath    | 44.306   | 5000    | 357.9    |
|                                | orjson+jsonpath-ng | 67.446   | 5000    | 360.2    |
| wide_arrays:$.records\[0\]     | strata             | 26.988   | 1       | 360.6    |
|                                | orjson+jmespath    | 36.282   | 1       | 360.6    |
|                                | orjson+jsonpath-ng | 40.197   | 1       | 360.6    |
| mixed:$.records\[\*\].id       | strata             | 4.161    | 5000    | 392.4    |
|                                | orjson+jmespath    | 13.024   | 5000    | 392.4    |
|                                | orjson+jsonpath-ng | 17.984   | 5000    | 392.4    |
| mixed:$.records\[0\]           | strata             | 4.115    | 1       | 392.4    |
|                                | orjson+jsonpath-ng | 12.172   | 1       | 392.4    |
|                                | orjson+jmespath    | 12.402   | 1       | 392.4    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.734    | 4000    | 971.1    |
|                                            | jmespath    | 2.573    | 4000    | 971.1    |
|                                            | jsonpath-ng | 93.114   | 4000    | 972.8    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 48.831   | 789913  | 972.5    |
|                                            | jmespath    | 402.163  | 4000    | 988.1    |
|                                            | jsonpath-ng | 1865.863 | 789913  | 639.8    |
| $..price                                   | strata      | 63.728   | 789913  | 632.7    |
|                                            | jsonpath-ng | 6894.709 | 789913  | 733.1    |
| $.users\[?(@.age>30)\]                     | strata      | 0.906    | 3212    | 749.9    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 75.200   | 26406   | 749.0    |
| flat:$.records\[\*\].id                    | strata      | 0.361    | 5000    | 349.0    |
|                                            | jmespath    | 1.714    | 5000    | 349.0    |
|                                            | jsonpath-ng | 9.431    | 5000    | 349.0    |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 349.0    |
|                                            | jmespath    | 0.007    | 1       | 349.0    |
|                                            | jsonpath-ng | 0.009    | 1       | 349.0    |
| nested:$.records\[\*\].id                  | strata      | 0.412    | 5000    | 353.8    |
|                                            | jmespath    | 2.355    | 5000    | 353.8    |
|                                            | jsonpath-ng | 21.747   | 5000    | 353.8    |
| nested:$.records\[0\]                      | jsonpath-ng | 0.012    | 1       | 353.8    |
|                                            | jmespath    | 0.013    | 1       | 353.8    |
|                                            | strata      | 0.013    | 1       | 353.8    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.354    | 5000    | 363.8    |
|                                            | jmespath    | 2.561    | 5000    | 363.8    |
|                                            | jsonpath-ng | 22.568   | 5000    | 363.8    |
| wide_arrays:$.records\[0\]                 | strata      | 0.010    | 1       | 363.8    |
|                                            | jmespath    | 0.011    | 1       | 363.8    |
|                                            | jsonpath-ng | 0.014    | 1       | 363.8    |
| mixed:$.records\[\*\].id                   | strata      | 0.384    | 5000    | 392.4    |
|                                            | jmespath    | 1.929    | 5000    | 392.4    |
|                                            | jsonpath-ng | 14.379   | 5000    | 392.4    |
| mixed:$.records\[0\]                       | jmespath    | 0.008    | 1       | 392.4    |
|                                            | strata      | 0.009    | 1       | 392.4    |
|                                            | jsonpath-ng | 0.017    | 1       | 392.4    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **10.7% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **70.8% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **20.5% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **11.3% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **11.8% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **21.1% faster** than #2 (orjson)   |
| loads (flat)        | **#1** / 5              | **49.2% faster** than #2 (msgspec)  |
| load (flat)         | **#1** / 5              | **0.8% faster** than #2 (msgspec)   |
| dumps (flat)        | **#1** / 5              | **22.8% faster** than #2 (msgspec)  |
| dump (flat)         | **#1** / 5              | **14.3% faster** than #2 (msgspec)  |
| loads (nested)      | **#1** / 5              | **194.7% faster** than #2 (orjson)  |
| load (nested)       | **#1** / 5              | **201.8% faster** than #2 (orjson)  |
| dumps (nested)      | **#3** / 5              | 13.8% behind #1 (msgspec)           |
| dump (nested)       | **#1** / 5              | **12.0% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#1** / 5              | **4.9% faster** than #2 (msgspec)   |
| load (wide_arrays)  | **#1** / 5              | **45.2% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **8.3% faster** than #2 (orjson)    |
| dump (wide_arrays)  | **#2** / 5              | 8.3% behind #1 (msgspec)            |
| loads (mixed)       | **#1** / 5              | **110.6% faster** than #2 (msgspec) |
| load (mixed)        | **#1** / 5              | **8.1% faster** than #2 (msgspec)   |
| dumps (mixed)       | **#1** / 5              | **27.6% faster** than #2 (msgspec)  |
| dump (mixed)        | **#2** / 5              | 32.9% behind #1 (msgspec)           |
| search (JSONPath)   | **#1** in 11/11 queries | avg **789% faster**                 |
| query (JSONPath)    | **#1** in 11/13 queries | -                                   |
