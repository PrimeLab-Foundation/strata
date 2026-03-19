# Strata Benchmark Results

Generated: 2026-03-19 17:54:40

## Environment

- Commit: f6a531005609a864792e4e9f097a7fc572656b95
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
| strata        | flat         | 6.979    | 8.168       | 8.310    | 460.1    |
| orjson        | flat         | 7.687    | 9.485       | 9.622    | 460.1    |
| msgspec       | flat         | 9.038    | 12.833      | 13.303   | 460.1    |
| ujson         | flat         | 13.690   | 16.327      | 16.775   | 460.1    |
| json (stdlib) | flat         | 23.236   | 24.618      | 24.800   | 463.0    |
| strata        | mixed        | 6.862    | 8.396       | 8.544    | 768.9    |
| msgspec       | mixed        | 9.827    | 12.415      | 12.902   | 774.5    |
| orjson        | mixed        | 10.366   | 15.784      | 23.913   | 774.5    |
| ujson         | mixed        | 15.087   | 18.936      | 20.509   | 774.5    |
| json (stdlib) | mixed        | 24.053   | 26.679      | 26.948   | 776.9    |
| strata        | nested       | 6.982    | 8.320       | 8.426    | 460.4    |
| msgspec       | nested       | 20.724   | 21.225      | 40.255   | 460.4    |
| orjson        | nested       | 19.273   | 23.350      | 42.873   | 460.4    |
| ujson         | nested       | 16.291   | 25.658      | 44.757   | 460.4    |
| json (stdlib) | nested       | 21.972   | 32.559      | 52.551   | 460.4    |
| strata        | users.json   | 183.266  | 229.929     | 262.240  | 340.0    |
| orjson        | users.json   | 186.300  | 271.533     | 275.033  | 478.0    |
| msgspec       | users.json   | 206.279  | 301.196     | 319.893  | 467.5    |
| ujson         | users.json   | 289.451  | 399.137     | 431.440  | 589.9    |
| json (stdlib) | users.json   | 397.079  | 473.355     | 500.578  | 511.4    |
| strata        | users.ndjson | 208.684  | 239.993     | 246.728  | 676.6    |
| orjson        | users.ndjson | 310.419  | 345.831     | 367.404  | 727.2    |
| msgspec       | users.ndjson | 269.617  | 360.272     | 395.882  | 514.4    |
| ujson         | users.ndjson | 376.050  | 472.849     | 544.664  | 844.3    |
| json (stdlib) | users.ndjson | 485.373  | 545.832     | 547.066  | 517.1    |
| strata        | wide_arrays  | 30.403   | 37.777      | 38.589   | 383.2    |
| orjson        | wide_arrays  | 30.502   | 45.254      | 53.941   | 408.8    |
| msgspec       | wide_arrays  | 42.896   | 48.837      | 49.602   | 418.6    |
| ujson         | wide_arrays  | 60.362   | 70.993      | 72.783   | 418.6    |
| json (stdlib) | wide_arrays  | 123.602  | 140.147     | 142.935  | 432.6    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 7.620    | 8.459       | 8.541    | 466.0    |
| orjson        | flat         | 8.956    | 10.291      | 10.363   | 466.0    |
| msgspec       | flat         | 9.315    | 10.484      | 10.977   | 468.9    |
| ujson         | flat         | 15.207   | 17.041      | 17.738   | 468.9    |
| json (stdlib) | flat         | 25.266   | 25.633      | 30.232   | 468.9    |
| strata        | mixed        | 7.533    | 9.728       | 9.940    | 776.9    |
| msgspec       | mixed        | 11.286   | 13.209      | 13.210   | 779.3    |
| orjson        | mixed        | 10.996   | 15.221      | 15.381   | 776.9    |
| ujson         | mixed        | 15.745   | 18.099      | 18.366   | 779.3    |
| json (stdlib) | mixed        | 28.824   | 29.667      | 39.965   | 779.3    |
| strata        | nested       | 7.283    | 8.319       | 8.432    | 460.4    |
| orjson        | nested       | 15.530   | 18.132      | 18.887   | 460.4    |
| msgspec       | nested       | 16.040   | 18.583      | 30.650   | 462.3    |
| ujson         | nested       | 16.792   | 22.712      | 45.232   | 462.3    |
| json (stdlib) | nested       | 24.581   | 29.884      | 33.179   | 462.3    |
| strata        | users.json   | 187.852  | 225.361     | 236.450  | 522.2    |
| orjson        | users.json   | 190.576  | 271.824     | 272.653  | 522.2    |
| msgspec       | users.json   | 213.591  | 301.550     | 324.752  | 555.3    |
| ujson         | users.json   | 300.031  | 404.584     | 435.261  | 677.8    |
| json (stdlib) | users.json   | 404.904  | 489.415     | 495.334  | 555.3    |
| strata        | users.ndjson | 202.267  | 254.929     | 280.036  | 520.0    |
| orjson        | users.ndjson | 293.986  | 327.538     | 357.166  | 563.9    |
| msgspec       | users.ndjson | 270.184  | 360.825     | 365.603  | 602.2    |
| ujson         | users.ndjson | 368.464  | 474.895     | 488.602  | 724.7    |
| json (stdlib) | users.ndjson | 482.375  | 511.100     | 543.504  | 603.9    |
| strata        | wide_arrays  | 31.253   | 38.046      | 39.272   | 443.7    |
| orjson        | wide_arrays  | 32.098   | 39.827      | 43.417   | 444.7    |
| msgspec       | wide_arrays  | 39.007   | 49.467      | 49.984   | 459.8    |
| ujson         | wide_arrays  | 63.292   | 75.845      | 86.681   | 458.8    |
| json (stdlib) | wide_arrays  | 129.417  | 142.890     | 153.191  | 460.8    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.345    | 2.351       | 2.375    | 468.9    |
| msgspec | flat        | 2.854    | 2.889       | 3.333    | 464.6    |
| orjson  | flat        | 3.039    | 3.085       | 3.187    | 464.6    |
| ujson   | flat        | 12.580   | 12.586      | 13.100   | 466.6    |
| json    | flat        | 28.999   | 30.440      | 31.740   | 466.6    |
| strata  | mixed       | 2.751    | 2.756       | 2.767    | 779.3    |
| msgspec | mixed       | 2.996    | 3.176       | 3.272    | 781.7    |
| orjson  | mixed       | 3.186    | 3.281       | 3.558    | 777.4    |
| ujson   | mixed       | 13.127   | 13.263      | 13.427   | 781.7    |
| json    | mixed       | 29.663   | 29.895      | 32.384   | 789.0    |
| msgspec | nested      | 1.946    | 1.988       | 2.023    | 462.3    |
| orjson  | nested      | 2.002    | 2.048       | 2.096    | 462.3    |
| strata  | nested      | 2.224    | 2.241       | 2.510    | 462.3    |
| ujson   | nested      | 9.774    | 9.948       | 10.139   | 462.3    |
| json    | nested      | 13.100   | 13.363      | 14.181   | 462.3    |
| strata  | users.json  | 58.722   | 59.252      | 69.096   | 590.9    |
| msgspec | users.json  | 78.814   | 79.233      | 80.092   | 943.1    |
| orjson  | users.json  | 80.511   | 80.756      | 90.591   | 747.6    |
| ujson   | users.json  | 294.796  | 296.094     | 303.094  | 1140.0   |
| json    | users.json  | 422.867  | 423.550     | 425.957  | 1354.2   |
| strata  | wide_arrays | 15.298   | 15.385      | 15.541   | 458.8    |
| orjson  | wide_arrays | 16.162   | 16.214      | 16.314   | 507.8    |
| msgspec | wide_arrays | 17.315   | 18.811      | 18.821   | 556.9    |
| ujson   | wide_arrays | 72.009   | 72.248      | 75.305   | 605.9    |
| json    | wide_arrays | 199.842  | 201.337     | 201.819  | 658.0    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 3.439    | 4.378       | 5.885    | 466.6    |
| msgspec       | flat        | 3.896    | 4.469       | 4.870    | 466.4    |
| orjson        | flat        | 3.848    | 4.546       | 7.747    | 466.4    |
| ujson         | flat        | 14.084   | 18.808      | 23.098   | 466.4    |
| json (stdlib) | flat        | 83.156   | 84.938      | 114.282  | 466.4    |
| msgspec       | mixed       | 3.829    | 3.832       | 3.984    | 788.9    |
| strata        | mixed       | 3.569    | 4.182       | 4.391    | 789.0    |
| orjson        | mixed       | 4.079    | 4.226       | 4.823    | 788.9    |
| ujson         | mixed       | 14.853   | 15.002      | 15.101   | 788.9    |
| json (stdlib) | mixed       | 104.039  | 105.636     | 109.915  | 788.9    |
| orjson        | nested      | 2.920    | 2.946       | 4.688    | 462.3    |
| strata        | nested      | 2.797    | 2.967       | 3.031    | 462.3    |
| msgspec       | nested      | 2.882    | 3.841       | 7.336    | 462.3    |
| ujson         | nested      | 10.843   | 13.011      | 15.817   | 462.3    |
| json (stdlib) | nested      | 118.643  | 119.870     | 120.363  | 462.3    |
| strata        | users.json  | 65.831   | 80.808      | 83.128   | 1206.5   |
| msgspec       | users.json  | 101.409  | 103.892     | 108.187  | 1175.3   |
| orjson        | users.json  | 99.306   | 107.885     | 120.428  | 1008.9   |
| ujson         | users.json  | 321.240  | 325.797     | 329.781  | 1061.0   |
| json (stdlib) | users.json  | 2981.551 | 2996.433    | 3031.094 | 841.6    |
| strata        | wide_arrays | 22.376   | 22.683      | 24.074   | 658.0    |
| orjson        | wide_arrays | 25.062   | 25.419      | 28.501   | 670.3    |
| msgspec       | wide_arrays | 26.327   | 28.476      | 31.372   | 764.1    |
| ujson         | wide_arrays | 82.067   | 84.512      | 84.607   | 715.0    |
| json (stdlib) | wide_arrays | 448.344  | 448.537     | 451.891  | 764.1    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 85.908   | 4000    | 625.8    |
|                                | orjson+jmespath    | 202.317  | 4000    | 978.2    |
|                                | orjson+jsonpath-ng | 208.174  | 4000    | 986.2    |
| all item prices                | strata             | 99.376   | 789913  | 1013.1   |
|                                | orjson+jmespath    | 560.103  | 4000    | 1073.9   |
|                                | orjson+jsonpath-ng | 1989.601 | 789913  | 802.9    |
| recursive price                | strata             | 96.192   | 789913  | 776.1    |
|                                | orjson+jsonpath-ng | 6380.759 | 789913  | 668.9    |
| flat:$.records\[\*\].id        | strata             | 5.085    | 5000    | 466.4    |
|                                | orjson+jmespath    | 11.401   | 5000    | 466.4    |
|                                | orjson+jsonpath-ng | 16.611   | 5000    | 463.4    |
| flat:$.records\[0\]            | strata             | 4.728    | 1       | 463.4    |
|                                | orjson+jsonpath-ng | 8.919    | 1       | 463.4    |
|                                | orjson+jmespath    | 9.411    | 1       | 463.4    |
| nested:$.records\[\*\].id      | strata             | 3.660    | 5000    | 462.3    |
|                                | orjson+jmespath    | 16.982   | 5000    | 462.3    |
|                                | orjson+jsonpath-ng | 22.864   | 5000    | 462.3    |
| nested:$.records\[0\]          | strata             | 3.628    | 1       | 462.3    |
|                                | orjson+jsonpath-ng | 15.864   | 1       | 462.3    |
|                                | orjson+jmespath    | 15.953   | 1       | 462.3    |
| wide_arrays:$.records\[\*\].id | strata             | 23.573   | 5000    | 764.1    |
|                                | orjson+jmespath    | 36.566   | 5000    | 764.1    |
|                                | orjson+jsonpath-ng | 45.162   | 5000    | 764.1    |
| wide_arrays:$.records\[0\]     | strata             | 23.081   | 1       | 764.1    |
|                                | orjson+jsonpath-ng | 34.541   | 1       | 764.1    |
|                                | orjson+jmespath    | 38.964   | 1       | 764.1    |
| mixed:$.records\[\*\].id       | strata             | 4.678    | 5000    | 788.9    |
|                                | orjson+jmespath    | 12.934   | 5000    | 788.9    |
|                                | orjson+jsonpath-ng | 18.505   | 5000    | 788.9    |
| mixed:$.records\[0\]           | strata             | 4.349    | 1       | 788.9    |
|                                | orjson+jmespath    | 11.111   | 1       | 788.9    |
|                                | orjson+jsonpath-ng | 11.868   | 1       | 788.9    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.721    | 4000    | 897.5    |
|                                            | jmespath    | 2.302    | 4000    | 897.5    |
|                                            | jsonpath-ng | 82.161   | 4000    | 899.2    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 42.948   | 789913  | 899.3    |
|                                            | jmespath    | 407.251  | 4000    | 914.2    |
|                                            | jsonpath-ng | 1783.726 | 789913  | 798.6    |
| $..price                                   | strata      | 57.783   | 789913  | 822.0    |
|                                            | jsonpath-ng | 6402.399 | 789913  | 922.0    |
| $.users\[?(@.age>30)\]                     | strata      | 0.876    | 3212    | 939.7    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 65.596   | 26406   | 939.7    |
| flat:$.records\[\*\].id                    | strata      | 0.334    | 5000    | 463.4    |
|                                            | jmespath    | 1.864    | 5000    | 463.4    |
|                                            | jsonpath-ng | 10.455   | 5000    | 463.4    |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 463.4    |
|                                            | jmespath    | 0.008    | 1       | 463.4    |
|                                            | jsonpath-ng | 0.016    | 1       | 463.4    |
| nested:$.records\[\*\].id                  | strata      | 0.737    | 5000    | 462.3    |
|                                            | jmespath    | 1.779    | 5000    | 462.3    |
|                                            | jsonpath-ng | 16.493   | 5000    | 462.3    |
| nested:$.records\[0\]                      | jmespath    | 0.010    | 1       | 462.3    |
|                                            | strata      | 0.010    | 1       | 462.3    |
|                                            | jsonpath-ng | 0.018    | 1       | 462.3    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.263    | 5000    | 764.1    |
|                                            | jmespath    | 1.973    | 5000    | 764.1    |
|                                            | jsonpath-ng | 16.194   | 5000    | 764.1    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 764.1    |
|                                            | jsonpath-ng | 0.011    | 1       | 764.1    |
|                                            | jmespath    | 0.013    | 1       | 764.1    |
| mixed:$.records\[\*\].id                   | strata      | 0.405    | 5000    | 788.9    |
|                                            | jmespath    | 1.727    | 5000    | 788.9    |
|                                            | jsonpath-ng | 11.519   | 5000    | 788.9    |
| mixed:$.records\[0\]                       | strata      | 0.004    | 1       | 788.9    |
|                                            | jmespath    | 0.009    | 1       | 788.9    |
|                                            | jsonpath-ng | 0.011    | 1       | 788.9    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **1.7% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **29.2% faster** than #2 (msgspec) |
| load (JSON file)    | **#1** / 5              | **1.5% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **33.6% faster** than #2 (msgspec) |
| dumps               | **#1** / 5              | **34.2% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **50.8% faster** than #2 (orjson)  |
| loads (flat)        | **#1** / 5              | **10.1% faster** than #2 (orjson)  |
| load (flat)         | **#1** / 5              | **17.5% faster** than #2 (orjson)  |
| dumps (flat)        | **#1** / 5              | **21.7% faster** than #2 (msgspec) |
| dump (flat)         | **#1** / 5              | **11.9% faster** than #2 (orjson)  |
| loads (nested)      | **#1** / 5              | **133.3% faster** than #2 (ujson)  |
| load (nested)       | **#1** / 5              | **113.2% faster** than #2 (orjson) |
| dumps (nested)      | **#3** / 5              | 14.3% behind #1 (msgspec)          |
| dump (nested)       | **#1** / 5              | **3.1% faster** than #2 (msgspec)  |
| loads (wide_arrays) | **#1** / 5              | **0.3% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **2.7% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **5.6% faster** than #2 (orjson)   |
| dump (wide_arrays)  | **#1** / 5              | **12.0% faster** than #2 (orjson)  |
| loads (mixed)       | **#1** / 5              | **43.2% faster** than #2 (msgspec) |
| load (mixed)        | **#1** / 5              | **46.0% faster** than #2 (orjson)  |
| dumps (mixed)       | **#1** / 5              | **8.9% faster** than #2 (msgspec)  |
| dump (mixed)        | **#1** / 5              | **7.3% faster** than #2 (msgspec)  |
| search (JSONPath)   | **#1** in 11/11 queries | avg **771% faster**                |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
