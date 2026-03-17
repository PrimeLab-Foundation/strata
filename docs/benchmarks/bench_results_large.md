# Strata Benchmark Results

Generated: 2026-03-17 15:54:55

## Environment

- Commit: 33d6835349dd084a915ba2ba437233c105a366f8
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
| strata        | flat         | 7.494    | 8.365       | 8.376    | 509.0    |
| orjson        | flat         | 10.347   | 10.356      | 14.942   | 509.0    |
| msgspec       | flat         | 8.678    | 13.516      | 14.234   | 509.0    |
| ujson         | flat         | 14.311   | 16.552      | 16.839   | 509.0    |
| json (stdlib) | flat         | 24.205   | 25.562      | 25.685   | 512.0    |
| strata        | mixed        | 8.715    | 8.941       | 9.412    | 685.9    |
| msgspec       | mixed        | 14.263   | 14.592      | 20.516   | 691.5    |
| orjson        | mixed        | 11.368   | 15.740      | 16.739   | 691.5    |
| ujson         | mixed        | 19.086   | 20.901      | 26.931   | 691.5    |
| json (stdlib) | mixed        | 26.992   | 28.545      | 42.336   | 693.9    |
| strata        | nested       | 6.135    | 7.562       | 7.881    | 507.4    |
| msgspec       | nested       | 23.484   | 24.329      | 35.487   | 507.4    |
| orjson        | nested       | 20.816   | 27.020      | 35.832   | 507.4    |
| ujson         | nested       | 24.643   | 27.251      | 33.499   | 507.4    |
| json (stdlib) | nested       | 31.077   | 33.539      | 34.138   | 507.4    |
| strata        | users.json   | 184.762  | 228.949     | 234.071  | 340.0    |
| orjson        | users.json   | 186.387  | 284.492     | 292.468  | 478.1    |
| msgspec       | users.json   | 217.780  | 310.319     | 315.535  | 467.6    |
| ujson         | users.json   | 302.961  | 402.265     | 427.848  | 589.9    |
| json (stdlib) | users.json   | 430.723  | 515.833     | 523.813  | 511.4    |
| strata        | users.ndjson | 192.560  | 238.586     | 252.485  | 568.2    |
| orjson        | users.ndjson | 301.133  | 319.797     | 366.115  | 618.6    |
| msgspec       | users.ndjson | 292.945  | 361.217     | 372.394  | 612.2    |
| ujson         | users.ndjson | 387.368  | 472.840     | 485.201  | 734.7    |
| json (stdlib) | users.ndjson | 479.795  | 552.139     | 569.739  | 614.9    |
| orjson        | wide_arrays  | 33.100   | 40.865      | 53.825   | 452.4    |
| strata        | wide_arrays  | 34.459   | 41.759      | 45.129   | 425.8    |
| msgspec       | wide_arrays  | 50.311   | 50.590      | 57.687   | 453.4    |
| ujson         | wide_arrays  | 65.703   | 75.870      | 82.824   | 453.4    |
| json (stdlib) | wide_arrays  | 139.482  | 146.804     | 147.390  | 467.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 9.139    | 9.253       | 9.549    | 512.0    |
| orjson        | flat         | 9.943    | 11.527      | 11.571   | 512.0    |
| msgspec       | flat         | 9.285    | 12.773      | 17.110   | 514.9    |
| ujson         | flat         | 16.679   | 18.545      | 23.477   | 514.9    |
| json (stdlib) | flat         | 24.521   | 26.905      | 29.124   | 514.9    |
| strata        | mixed        | 7.920    | 9.462       | 10.894   | 693.9    |
| msgspec       | mixed        | 11.507   | 13.740      | 24.999   | 696.3    |
| orjson        | mixed        | 11.333   | 14.787      | 14.899   | 693.9    |
| ujson         | mixed        | 19.918   | 21.131      | 23.799   | 696.3    |
| json (stdlib) | mixed        | 26.458   | 28.775      | 38.120   | 696.3    |
| strata        | nested       | 6.520    | 8.000       | 8.094    | 507.4    |
| orjson        | nested       | 14.831   | 19.644      | 35.139   | 507.4    |
| msgspec       | nested       | 14.744   | 19.862      | 24.250   | 510.9    |
| ujson         | nested       | 18.015   | 23.793      | 43.272   | 510.9    |
| json (stdlib) | nested       | 23.584   | 27.362      | 36.293   | 510.9    |
| strata        | users.json   | 184.765  | 243.138     | 248.065  | 522.2    |
| orjson        | users.json   | 237.055  | 295.040     | 302.052  | 522.2    |
| msgspec       | users.json   | 221.858  | 322.127     | 327.159  | 555.4    |
| ujson         | users.json   | 309.536  | 504.094     | 521.615  | 677.8    |
| json (stdlib) | users.json   | 419.274  | 520.448     | 526.073  | 555.4    |
| strata        | users.ndjson | 202.016  | 258.668     | 264.765  | 617.8    |
| orjson        | users.ndjson | 291.074  | 319.464     | 347.098  | 618.0    |
| msgspec       | users.ndjson | 314.386  | 379.615     | 416.858  | 656.2    |
| ujson         | users.ndjson | 430.732  | 482.529     | 507.713  | 778.7    |
| json (stdlib) | users.ndjson | 479.597  | 555.920     | 561.236  | 657.9    |
| orjson        | wide_arrays  | 35.977   | 43.622      | 53.908   | 470.5    |
| strata        | wide_arrays  | 35.777   | 44.724      | 47.099   | 469.5    |
| msgspec       | wide_arrays  | 40.243   | 50.130      | 70.297   | 485.5    |
| ujson         | wide_arrays  | 70.288   | 77.600      | 86.870   | 484.5    |
| json (stdlib) | wide_arrays  | 140.479  | 144.687     | 146.411  | 486.5    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.228    | 2.260       | 3.110    | 514.9    |
| msgspec | flat        | 2.880    | 2.944       | 3.380    | 510.6    |
| orjson  | flat        | 3.199    | 3.478       | 3.596    | 510.6    |
| ujson   | flat        | 12.129   | 12.185      | 12.362   | 512.6    |
| json    | flat        | 22.841   | 22.857      | 23.821   | 512.5    |
| strata  | mixed       | 2.494    | 2.577       | 2.588    | 696.3    |
| orjson  | mixed       | 3.135    | 3.168       | 3.205    | 694.4    |
| msgspec | mixed       | 2.939    | 3.198       | 3.578    | 698.9    |
| ujson   | mixed       | 12.916   | 12.931      | 13.037   | 700.9    |
| json    | mixed       | 29.578   | 29.679      | 30.083   | 710.6    |
| strata  | nested      | 1.942    | 2.044       | 2.602    | 510.9    |
| msgspec | nested      | 2.005    | 2.250       | 2.778    | 511.9    |
| orjson  | nested      | 2.038    | 2.319       | 2.347    | 511.8    |
| ujson   | nested      | 9.806    | 10.061      | 10.344   | 511.9    |
| json    | nested      | 12.374   | 12.461      | 13.643   | 511.9    |
| strata  | users.json  | 48.567   | 48.605      | 57.062   | 590.9    |
| msgspec | users.json  | 81.792   | 82.029      | 83.908   | 944.7    |
| orjson  | users.json  | 83.459   | 84.619      | 91.242   | 748.5    |
| ujson   | users.json  | 312.798  | 314.140     | 318.544  | 1139.9   |
| json    | users.json  | 423.132  | 427.014     | 433.988  | 1354.6   |
| strata  | wide_arrays | 13.995   | 14.152      | 14.312   | 484.5    |
| orjson  | wide_arrays | 17.854   | 18.054      | 18.604   | 493.2    |
| msgspec | wide_arrays | 18.281   | 18.351      | 18.593   | 530.0    |
| ujson   | wide_arrays | 73.815   | 75.366      | 76.232   | 576.0    |
| json    | wide_arrays | 211.494  | 211.699     | 216.933  | 628.1    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 2.829    | 2.887       | 3.080    | 512.5    |
| msgspec       | flat        | 3.578    | 3.659       | 4.247    | 512.4    |
| orjson        | flat        | 3.403    | 3.919       | 4.465    | 512.4    |
| ujson         | flat        | 12.836   | 12.897      | 13.133   | 512.4    |
| json (stdlib) | flat        | 84.326   | 84.879      | 88.699   | 512.4    |
| strata        | mixed       | 2.898    | 3.101       | 4.223    | 710.6    |
| orjson        | mixed       | 3.756    | 3.798       | 3.938    | 710.4    |
| msgspec       | mixed       | 3.773    | 3.959       | 4.815    | 710.4    |
| ujson         | mixed       | 13.971   | 14.744      | 15.355   | 710.4    |
| json (stdlib) | mixed       | 99.871   | 99.931      | 101.253  | 710.4    |
| strata        | nested      | 2.259    | 2.262       | 3.141    | 511.9    |
| orjson        | nested      | 2.490    | 2.503       | 2.965    | 511.9    |
| msgspec       | nested      | 2.535    | 2.596       | 3.432    | 511.9    |
| ujson         | nested      | 10.551   | 11.462      | 12.401   | 511.9    |
| json (stdlib) | nested      | 120.761  | 122.011     | 125.769  | 511.9    |
| strata        | users.json  | 56.291   | 62.486      | 73.544   | 1398.5   |
| orjson        | users.json  | 107.967  | 110.564     | 111.116  | 1554.2   |
| msgspec       | users.json  | 105.560  | 112.673     | 119.491  | 1865.8   |
| ujson         | users.json  | 334.457  | 375.471     | 411.130  | 1710.0   |
| json (stdlib) | users.json  | 3132.744 | 3161.988    | 3171.585 | 1260.8   |
| strata        | wide_arrays | 16.049   | 16.653      | 16.902   | 628.1    |
| orjson        | wide_arrays | 19.164   | 19.237      | 20.837   | 677.2    |
| msgspec       | wide_arrays | 21.826   | 21.979      | 23.899   | 685.0    |
| ujson         | wide_arrays | 78.907   | 80.389      | 82.882   | 685.2    |
| json (stdlib) | wide_arrays | 457.718  | 462.700     | 471.358  | 685.0    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 88.834   | 4000    | 1040.9   |
|                                | orjson+jmespath    | 225.145  | 4000    | 1393.3   |
|                                | orjson+jsonpath-ng | 225.838  | 4000    | 1400.9   |
| all item prices                | strata             | 99.738   | 789913  | 1426.8   |
|                                | orjson+jmespath    | 569.597  | 4000    | 1487.7   |
|                                | orjson+jsonpath-ng | 2034.056 | 789913  | 870.2    |
| recursive price                | strata             | 99.949   | 789913  | 845.2    |
|                                | orjson+jsonpath-ng | 6520.887 | 789913  | 713.2    |
| flat:$.records\[\*\].id        | strata             | 4.702    | 5000    | 512.4    |
|                                | orjson+jmespath    | 11.557   | 5000    | 512.4    |
|                                | orjson+jsonpath-ng | 17.337   | 5000    | 509.4    |
| flat:$.records\[0\]            | strata             | 4.657    | 1       | 509.4    |
|                                | orjson+jsonpath-ng | 9.245    | 1       | 509.4    |
|                                | orjson+jmespath    | 9.840    | 1       | 509.4    |
| nested:$.records\[\*\].id      | strata             | 3.576    | 5000    | 511.9    |
|                                | orjson+jmespath    | 16.822   | 5000    | 511.9    |
|                                | orjson+jsonpath-ng | 23.314   | 5000    | 511.9    |
| nested:$.records\[0\]          | strata             | 3.548    | 1       | 511.9    |
|                                | orjson+jsonpath-ng | 15.527   | 1       | 511.9    |
|                                | orjson+jmespath    | 15.585   | 1       | 511.9    |
| wide_arrays:$.records\[\*\].id | strata             | 27.445   | 5000    | 685.1    |
|                                | orjson+jmespath    | 38.743   | 5000    | 685.1    |
|                                | orjson+jsonpath-ng | 45.747   | 5000    | 685.1    |
| wide_arrays:$.records\[0\]     | strata             | 27.586   | 1       | 685.1    |
|                                | orjson+jsonpath-ng | 35.979   | 1       | 685.1    |
|                                | orjson+jmespath    | 37.535   | 1       | 685.1    |
| mixed:$.records\[\*\].id       | strata             | 4.940    | 5000    | 710.4    |
|                                | orjson+jmespath    | 13.757   | 5000    | 710.4    |
|                                | orjson+jsonpath-ng | 19.216   | 5000    | 710.4    |
| mixed:$.records\[0\]           | strata             | 5.075    | 1       | 710.4    |
|                                | orjson+jmespath    | 11.842   | 1       | 710.4    |
|                                | orjson+jsonpath-ng | 12.150   | 1       | 710.4    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.734    | 4000    | 941.8    |
|                                            | jmespath    | 2.202    | 4000    | 941.8    |
|                                            | jsonpath-ng | 88.835   | 4000    | 943.5    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 45.747   | 789913  | 951.6    |
|                                            | jmespath    | 432.149  | 4000    | 966.4    |
|                                            | jsonpath-ng | 1795.069 | 789913  | 820.7    |
| $..price                                   | strata      | 63.483   | 789913  | 829.0    |
|                                            | jsonpath-ng | 6283.130 | 789913  | 827.8    |
| $.users\[?(@.age>30)\]                     | strata      | 0.848    | 3212    | 830.3    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 62.824   | 26406   | 830.3    |
| flat:$.records\[\*\].id                    | strata      | 0.335    | 5000    | 509.4    |
|                                            | jmespath    | 1.666    | 5000    | 509.4    |
|                                            | jsonpath-ng | 9.737    | 5000    | 508.4    |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 508.4    |
|                                            | jmespath    | 0.008    | 1       | 508.4    |
|                                            | jsonpath-ng | 0.014    | 1       | 508.4    |
| nested:$.records\[\*\].id                  | strata      | 0.808    | 5000    | 511.9    |
|                                            | jmespath    | 1.871    | 5000    | 511.9    |
|                                            | jsonpath-ng | 17.712   | 5000    | 511.9    |
| nested:$.records\[0\]                      | strata      | 0.008    | 1       | 511.9    |
|                                            | jsonpath-ng | 0.015    | 1       | 511.9    |
|                                            | jmespath    | 0.016    | 1       | 511.9    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.269    | 5000    | 685.1    |
|                                            | jmespath    | 1.829    | 5000    | 685.1    |
|                                            | jsonpath-ng | 16.826   | 5000    | 685.1    |
| wide_arrays:$.records\[0\]                 | strata      | 0.006    | 1       | 685.1    |
|                                            | jmespath    | 0.014    | 1       | 685.1    |
|                                            | jsonpath-ng | 0.019    | 1       | 685.1    |
| mixed:$.records\[\*\].id                   | strata      | 0.368    | 5000    | 710.4    |
|                                            | jmespath    | 1.704    | 5000    | 710.4    |
|                                            | jsonpath-ng | 13.420   | 5000    | 710.4    |
| mixed:$.records\[0\]                       | strata      | 0.007    | 1       | 710.4    |
|                                            | jsonpath-ng | 0.009    | 1       | 710.4    |
|                                            | jmespath    | 0.016    | 1       | 710.4    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **0.9% faster** than #2 (orjson)    |
| loads (NDJSON)      | **#1** / 5              | **52.1% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **20.1% faster** than #2 (msgspec)  |
| load (NDJSON file)  | **#1** / 5              | **44.1% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **68.4% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **87.5% faster** than #2 (msgspec)  |
| loads (flat)        | **#1** / 5              | **15.8% faster** than #2 (msgspec)  |
| load (flat)         | **#1** / 5              | **1.6% faster** than #2 (msgspec)   |
| dumps (flat)        | **#1** / 5              | **29.3% faster** than #2 (msgspec)  |
| dump (flat)         | **#1** / 5              | **20.3% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **239.3% faster** than #2 (orjson)  |
| load (nested)       | **#1** / 5              | **126.1% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **3.2% faster** than #2 (msgspec)   |
| dump (nested)       | **#1** / 5              | **10.2% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#2** / 5              | 4.1% behind #1 (orjson)             |
| load (wide_arrays)  | **#1** / 5              | **0.6% faster** than #2 (orjson)    |
| dumps (wide_arrays) | **#1** / 5              | **27.6% faster** than #2 (orjson)   |
| dump (wide_arrays)  | **#1** / 5              | **19.4% faster** than #2 (orjson)   |
| loads (mixed)       | **#1** / 5              | **30.4% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **43.1% faster** than #2 (orjson)   |
| dumps (mixed)       | **#1** / 5              | **17.8% faster** than #2 (msgspec)  |
| dump (mixed)        | **#1** / 5              | **29.6% faster** than #2 (orjson)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **762% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
