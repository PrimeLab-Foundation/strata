# Strata Benchmark Results

Generated: 2026-03-18 19:54:28

## Environment

- Commit: f5870e0f4dbf614611ada7aa37a627b2e43d1579
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
| strata        | flat         | 7.202    | 8.318       | 8.340    | 506.0    |
| orjson        | flat         | 8.071    | 9.660       | 9.840    | 506.0    |
| msgspec       | flat         | 9.328    | 10.174      | 10.251   | 506.0    |
| ujson         | flat         | 13.587   | 19.079      | 22.314   | 506.0    |
| json (stdlib) | flat         | 23.040   | 24.731      | 25.187   | 508.9    |
| strata        | mixed        | 6.690    | 8.097       | 8.207    | 793.9    |
| orjson        | mixed        | 11.857   | 14.167      | 15.460   | 799.5    |
| msgspec       | mixed        | 13.285   | 15.468      | 16.561   | 799.5    |
| ujson         | mixed        | 13.699   | 16.884      | 17.267   | 799.5    |
| json (stdlib) | mixed        | 24.697   | 27.354      | 27.490   | 801.9    |
| strata        | nested       | 6.559    | 7.419       | 8.302    | 503.3    |
| orjson        | nested       | 13.621   | 17.354      | 27.188   | 503.3    |
| msgspec       | nested       | 17.518   | 20.148      | 26.739   | 503.3    |
| ujson         | nested       | 16.530   | 21.365      | 21.850   | 503.3    |
| json (stdlib) | nested       | 22.093   | 26.776      | 42.853   | 503.3    |
| strata        | users.json   | 176.784  | 216.113     | 220.210  | 340.2    |
| orjson        | users.json   | 192.450  | 264.915     | 269.116  | 478.3    |
| msgspec       | users.json   | 207.819  | 286.953     | 287.497  | 467.8    |
| ujson         | users.json   | 300.414  | 402.557     | 415.097  | 590.1    |
| json (stdlib) | users.json   | 395.602  | 467.363     | 500.597  | 511.7    |
| strata        | users.ndjson | 189.385  | 229.099     | 241.654  | 560.5    |
| orjson        | users.ndjson | 270.181  | 320.612     | 335.198  | 610.7    |
| msgspec       | users.ndjson | 277.584  | 339.109     | 353.484  | 605.2    |
| ujson         | users.ndjson | 382.325  | 435.709     | 441.978  | 727.7    |
| json (stdlib) | users.ndjson | 460.745  | 522.316     | 531.721  | 607.9    |
| strata        | wide_arrays  | 29.589   | 36.058      | 39.512   | 424.9    |
| orjson        | wide_arrays  | 27.882   | 38.624      | 44.315   | 450.5    |
| msgspec       | wide_arrays  | 38.501   | 48.849      | 57.380   | 452.5    |
| ujson         | wide_arrays  | 59.992   | 72.369      | 76.085   | 451.5    |
| json (stdlib) | wide_arrays  | 128.679  | 138.648     | 146.668  | 466.5    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 7.873    | 8.883       | 8.962    | 508.9    |
| orjson        | flat         | 8.754    | 10.125      | 10.262   | 508.9    |
| msgspec       | flat         | 9.403    | 11.343      | 15.406   | 511.9    |
| ujson         | flat         | 14.854   | 16.589      | 17.278   | 511.9    |
| json (stdlib) | flat         | 23.155   | 24.826      | 25.536   | 511.9    |
| strata        | mixed        | 7.457    | 8.891       | 8.896    | 801.9    |
| orjson        | mixed        | 10.221   | 12.581      | 12.612   | 801.9    |
| msgspec       | mixed        | 13.560   | 15.345      | 17.583   | 804.3    |
| ujson         | mixed        | 15.541   | 18.630      | 18.721   | 804.3    |
| json (stdlib) | mixed        | 25.153   | 27.571      | 29.538   | 804.3    |
| strata        | nested       | 6.889    | 7.719       | 7.832    | 503.3    |
| orjson        | nested       | 14.418   | 18.167      | 18.753   | 503.3    |
| msgspec       | nested       | 14.495   | 22.910      | 29.788   | 506.8    |
| ujson         | nested       | 22.191   | 25.488      | 29.174   | 506.8    |
| json (stdlib) | nested       | 22.600   | 27.221      | 44.739   | 506.8    |
| strata        | users.json   | 183.102  | 223.646     | 226.321  | 522.5    |
| orjson        | users.json   | 197.227  | 294.803     | 299.811  | 522.5    |
| msgspec       | users.json   | 208.376  | 296.395     | 316.356  | 555.6    |
| ujson         | users.json   | 312.338  | 399.980     | 418.226  | 678.1    |
| json (stdlib) | users.json   | 415.720  | 501.203     | 513.879  | 555.6    |
| strata        | users.ndjson | 195.089  | 237.723     | 239.659  | 610.8    |
| orjson        | users.ndjson | 269.439  | 333.419     | 346.114  | 610.9    |
| msgspec       | users.ndjson | 265.192  | 341.080     | 348.137  | 649.2    |
| ujson         | users.ndjson | 380.535  | 466.209     | 485.481  | 771.7    |
| json (stdlib) | users.ndjson | 438.869  | 510.945     | 521.688  | 651.9    |
| strata        | wide_arrays  | 30.669   | 38.168      | 41.489   | 468.5    |
| orjson        | wide_arrays  | 29.880   | 39.259      | 54.929   | 469.5    |
| msgspec       | wide_arrays  | 38.252   | 49.500      | 55.597   | 484.5    |
| ujson         | wide_arrays  | 68.480   | 75.065      | 75.474   | 483.5    |
| json (stdlib) | wide_arrays  | 128.802  | 141.008     | 153.546  | 485.5    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.149    | 2.209       | 2.265    | 511.9    |
| msgspec | flat        | 2.897    | 3.007       | 3.148    | 507.5    |
| orjson  | flat        | 2.957    | 3.118       | 3.416    | 507.5    |
| ujson   | flat        | 11.994   | 12.016      | 12.130   | 509.5    |
| json    | flat        | 22.793   | 22.890      | 23.440   | 509.5    |
| strata  | mixed       | 2.452    | 2.454       | 2.454    | 804.3    |
| msgspec | mixed       | 2.831    | 2.839       | 3.108    | 806.7    |
| orjson  | mixed       | 3.038    | 3.081       | 3.089    | 802.4    |
| ujson   | mixed       | 12.519   | 12.541      | 12.689   | 806.7    |
| json    | mixed       | 28.068   | 28.123      | 28.316   | 814.1    |
| strata  | nested      | 1.844    | 1.854       | 1.855    | 506.8    |
| msgspec | nested      | 1.959    | 1.972       | 1.992    | 506.8    |
| orjson  | nested      | 1.998    | 2.415       | 2.945    | 506.8    |
| ujson   | nested      | 9.891    | 10.058      | 10.157   | 507.0    |
| json    | nested      | 13.021   | 13.130      | 13.256   | 507.0    |
| strata  | users.json  | 44.195   | 46.936      | 53.180   | 591.1    |
| orjson  | users.json  | 79.804   | 81.687      | 92.457   | 747.9    |
| msgspec | users.json  | 84.649   | 84.793      | 88.336   | 945.0    |
| ujson   | users.json  | 298.339  | 312.155     | 327.413  | 1078.4   |
| json    | users.json  | 417.616  | 417.651     | 419.286  | 1293.1   |
| strata  | wide_arrays | 13.715   | 13.719      | 13.724   | 483.5    |
| orjson  | wide_arrays | 16.231   | 16.251      | 16.942   | 532.6    |
| msgspec | wide_arrays | 17.448   | 17.560      | 18.518   | 581.7    |
| ujson   | wide_arrays | 71.735   | 72.191      | 72.586   | 630.7    |
| json    | wide_arrays | 201.246  | 201.672     | 201.698  | 682.8    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 2.739    | 2.762       | 2.997    | 509.5    |
| orjson        | flat        | 3.642    | 3.696       | 4.147    | 509.3    |
| msgspec       | flat        | 3.751    | 3.804       | 3.991    | 509.3    |
| ujson         | flat        | 12.817   | 13.022      | 13.049   | 509.3    |
| json (stdlib) | flat        | 80.533   | 81.451      | 82.045   | 509.3    |
| strata        | mixed       | 2.982    | 2.982       | 3.081    | 814.1    |
| msgspec       | mixed       | 3.432    | 3.465       | 3.508    | 813.9    |
| orjson        | mixed       | 3.644    | 3.790       | 3.790    | 813.9    |
| ujson         | mixed       | 13.392   | 13.528      | 13.677   | 813.9    |
| json (stdlib) | mixed       | 94.884   | 95.168      | 95.488   | 813.9    |
| orjson        | nested      | 2.446    | 2.459       | 2.485    | 507.0    |
| msgspec       | nested      | 2.452    | 2.483       | 2.521    | 507.0    |
| strata        | nested      | 2.342    | 2.595       | 3.133    | 507.0    |
| ujson         | nested      | 10.263   | 10.853      | 10.877   | 507.0    |
| json (stdlib) | nested      | 118.446  | 119.406     | 120.398  | 507.0    |
| strata        | users.json  | 59.706   | 61.351      | 61.762   | 1337.0   |
| orjson        | users.json  | 105.680  | 108.906     | 110.835  | 1492.7   |
| msgspec       | users.json  | 101.960  | 109.772     | 112.370  | 1804.1   |
| ujson         | users.json  | 306.754  | 325.231     | 326.639  | 1648.4   |
| json (stdlib) | users.json  | 2981.422 | 3016.925    | 3025.907 | 1588.3   |
| strata        | wide_arrays | 16.405   | 16.635      | 18.884   | 682.8    |
| msgspec       | wide_arrays | 19.956   | 20.247      | 22.762   | 789.0    |
| orjson        | wide_arrays | 19.752   | 21.351      | 22.988   | 695.2    |
| ujson         | wide_arrays | 74.572   | 75.878      | 77.348   | 739.9    |
| json (stdlib) | wide_arrays | 446.001  | 446.562     | 447.569  | 789.0    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 86.566   | 4000    | 1369.4   |
|                                | orjson+jmespath    | 207.917  | 4000    | 1316.6   |
|                                | orjson+jsonpath-ng | 217.254  | 4000    | 1323.4   |
| all item prices                | strata             | 93.764   | 789913  | 1350.5   |
|                                | orjson+jmespath    | 560.446  | 4000    | 1411.4   |
|                                | orjson+jsonpath-ng | 1949.945 | 789913  | 873.4    |
| recursive price                | strata             | 95.528   | 789913  | 846.6    |
|                                | orjson+jsonpath-ng | 6295.672 | 789913  | 714.5    |
| flat:$.records\[\*\].id        | strata             | 4.560    | 5000    | 509.3    |
|                                | orjson+jmespath    | 11.777   | 5000    | 509.3    |
|                                | orjson+jsonpath-ng | 16.765   | 5000    | 506.3    |
| flat:$.records\[0\]            | strata             | 4.472    | 1       | 506.3    |
|                                | orjson+jsonpath-ng | 9.370    | 1       | 506.3    |
|                                | orjson+jmespath    | 10.376   | 1       | 506.3    |
| nested:$.records\[\*\].id      | strata             | 3.611    | 5000    | 507.0    |
|                                | orjson+jmespath    | 16.913   | 5000    | 507.0    |
|                                | orjson+jsonpath-ng | 22.052   | 5000    | 507.0    |
| nested:$.records\[0\]          | strata             | 3.561    | 1       | 507.0    |
|                                | orjson+jmespath    | 14.379   | 1       | 507.0    |
|                                | orjson+jsonpath-ng | 14.893   | 1       | 507.0    |
| wide_arrays:$.records\[\*\].id | strata             | 23.049   | 5000    | 789.0    |
|                                | orjson+jmespath    | 36.581   | 5000    | 789.0    |
|                                | orjson+jsonpath-ng | 43.270   | 5000    | 789.0    |
| wide_arrays:$.records\[0\]     | strata             | 23.080   | 1       | 789.0    |
|                                | orjson+jsonpath-ng | 34.501   | 1       | 789.0    |
|                                | orjson+jmespath    | 36.442   | 1       | 789.0    |
| mixed:$.records\[\*\].id       | strata             | 4.370    | 5000    | 813.9    |
|                                | orjson+jmespath    | 12.602   | 5000    | 813.9    |
|                                | orjson+jsonpath-ng | 17.946   | 5000    | 813.9    |
| mixed:$.records\[0\]           | strata             | 4.276    | 1       | 813.9    |
|                                | orjson+jsonpath-ng | 10.333   | 1       | 813.9    |
|                                | orjson+jmespath    | 10.556   | 1       | 813.9    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.726    | 4000    | 943.0    |
|                                            | jmespath    | 2.179    | 4000    | 943.0    |
|                                            | jsonpath-ng | 80.383   | 4000    | 944.7    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 42.281   | 789913  | 944.8    |
|                                            | jmespath    | 408.310  | 4000    | 959.7    |
|                                            | jsonpath-ng | 1682.303 | 789913  | 807.9    |
| $..price                                   | strata      | 65.242   | 789913  | 821.6    |
|                                            | jsonpath-ng | 6129.803 | 789913  | 820.0    |
| $.users\[?(@.age>30)\]                     | strata      | 0.813    | 3212    | 823.6    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 63.743   | 26406   | 823.6    |
| flat:$.records\[\*\].id                    | strata      | 0.323    | 5000    | 506.3    |
|                                            | jmespath    | 1.649    | 5000    | 506.3    |
|                                            | jsonpath-ng | 9.559    | 5000    | 506.3    |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 506.3    |
|                                            | jmespath    | 0.008    | 1       | 506.3    |
|                                            | jsonpath-ng | 0.010    | 1       | 506.3    |
| nested:$.records\[\*\].id                  | strata      | 0.457    | 5000    | 507.0    |
|                                            | jmespath    | 1.747    | 5000    | 507.0    |
|                                            | jsonpath-ng | 15.657   | 5000    | 507.0    |
| nested:$.records\[0\]                      | strata      | 0.010    | 1       | 507.0    |
|                                            | jsonpath-ng | 0.011    | 1       | 507.0    |
|                                            | jmespath    | 0.012    | 1       | 507.0    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.262    | 5000    | 789.0    |
|                                            | jmespath    | 1.753    | 5000    | 789.0    |
|                                            | jsonpath-ng | 16.265   | 5000    | 789.0    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 789.0    |
|                                            | jmespath    | 0.009    | 1       | 789.0    |
|                                            | jsonpath-ng | 0.010    | 1       | 789.0    |
| mixed:$.records\[\*\].id                   | strata      | 0.313    | 5000    | 813.9    |
|                                            | jmespath    | 1.698    | 5000    | 813.9    |
|                                            | jsonpath-ng | 11.589   | 5000    | 813.9    |
| mixed:$.records\[0\]                       | strata      | 0.004    | 1       | 813.9    |
|                                            | jsonpath-ng | 0.008    | 1       | 813.9    |
|                                            | jmespath    | 0.008    | 1       | 813.9    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **8.9% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **42.7% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **7.7% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **35.9% faster** than #2 (msgspec) |
| dumps               | **#1** / 5              | **80.6% faster** than #2 (orjson)  |
| dump                | **#1** / 5              | **70.8% faster** than #2 (msgspec) |
| loads (flat)        | **#1** / 5              | **12.1% faster** than #2 (orjson)  |
| load (flat)         | **#1** / 5              | **11.2% faster** than #2 (orjson)  |
| dumps (flat)        | **#1** / 5              | **34.8% faster** than #2 (msgspec) |
| dump (flat)         | **#1** / 5              | **33.0% faster** than #2 (orjson)  |
| loads (nested)      | **#1** / 5              | **107.7% faster** than #2 (orjson) |
| load (nested)       | **#1** / 5              | **109.3% faster** than #2 (orjson) |
| dumps (nested)      | **#1** / 5              | **6.2% faster** than #2 (msgspec)  |
| dump (nested)       | **#1** / 5              | **4.5% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#2** / 5              | 6.1% behind #1 (orjson)            |
| load (wide_arrays)  | **#2** / 5              | 2.6% behind #1 (orjson)            |
| dumps (wide_arrays) | **#1** / 5              | **18.3% faster** than #2 (orjson)  |
| dump (wide_arrays)  | **#1** / 5              | **20.4% faster** than #2 (orjson)  |
| loads (mixed)       | **#1** / 5              | **77.2% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **37.1% faster** than #2 (orjson)  |
| dumps (mixed)       | **#1** / 5              | **15.5% faster** than #2 (msgspec) |
| dump (mixed)        | **#1** / 5              | **15.1% faster** than #2 (msgspec) |
| search (JSONPath)   | **#1** in 11/11 queries | avg **773% faster**                |
| query (JSONPath)    | **#1** in 13/13 queries | -                                  |
