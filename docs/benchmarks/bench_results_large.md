# Strata Benchmark Results

Generated: 2026-03-17 22:37:39

## Environment

- Commit: aa7d859c5354df2117bd93cad8c93b96f7917890
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
| strata        | flat         | 6.447    | 7.411       | 7.863    | 510.0    |
| orjson        | flat         | 8.270    | 9.571       | 13.445   | 510.0    |
| msgspec       | flat         | 9.122    | 10.104      | 11.147   | 510.0    |
| ujson         | flat         | 14.392   | 16.742      | 17.600   | 510.0    |
| json (stdlib) | flat         | 24.491   | 25.032      | 25.749   | 513.0    |
| strata        | mixed        | 6.765    | 7.892       | 8.234    | 742.5    |
| orjson        | mixed        | 12.078   | 13.678      | 15.360   | 748.1    |
| msgspec       | mixed        | 13.333   | 15.124      | 16.912   | 748.1    |
| ujson         | mixed        | 14.103   | 20.278      | 33.222   | 748.1    |
| json (stdlib) | mixed        | 27.951   | 29.674      | 34.215   | 750.5    |
| strata        | nested       | 5.744    | 9.286       | 10.850   | 507.4    |
| msgspec       | nested       | 13.616   | 19.342      | 30.430   | 507.4    |
| orjson        | nested       | 14.721   | 21.790      | 24.097   | 507.4    |
| ujson         | nested       | 22.637   | 24.375      | 43.591   | 507.4    |
| json (stdlib) | nested       | 24.949   | 28.873      | 29.900   | 507.4    |
| strata        | users.json   | 178.811  | 213.537     | 241.990  | 339.8    |
| orjson        | users.json   | 202.934  | 280.439     | 288.772  | 477.9    |
| msgspec       | users.json   | 216.463  | 310.729     | 312.742  | 467.3    |
| ujson         | users.json   | 326.965  | 423.975     | 431.679  | 589.7    |
| json (stdlib) | users.json   | 417.963  | 489.670     | 509.993  | 511.2    |
| strata        | users.ndjson | 181.526  | 226.966     | 243.218  | 568.5    |
| orjson        | users.ndjson | 291.621  | 325.998     | 336.598  | 618.8    |
| msgspec       | users.ndjson | 295.626  | 339.029     | 362.206  | 612.3    |
| ujson         | users.ndjson | 402.576  | 484.291     | 492.768  | 734.8    |
| json (stdlib) | users.ndjson | 506.854  | 520.686     | 569.189  | 615.0    |
| strata        | wide_arrays  | 30.963   | 37.894      | 47.863   | 423.0    |
| orjson        | wide_arrays  | 38.571   | 42.907      | 43.561   | 449.6    |
| msgspec       | wide_arrays  | 39.987   | 49.518      | 53.121   | 459.3    |
| ujson         | wide_arrays  | 68.824   | 73.142      | 94.055   | 458.4    |
| json (stdlib) | wide_arrays  | 137.597  | 145.878     | 149.949  | 473.3    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 7.148    | 8.180       | 8.501    | 513.0    |
| orjson        | flat         | 9.237    | 10.315      | 10.685   | 513.0    |
| msgspec       | flat         | 9.819    | 11.045      | 11.530   | 515.9    |
| ujson         | flat         | 17.932   | 18.297      | 21.100   | 515.9    |
| json (stdlib) | flat         | 25.826   | 26.226      | 28.284   | 515.9    |
| strata        | mixed        | 7.126    | 8.466       | 9.399    | 750.5    |
| orjson        | mixed        | 10.580   | 12.852      | 14.361   | 750.5    |
| msgspec       | mixed        | 11.655   | 13.902      | 14.719   | 752.9    |
| ujson         | mixed        | 16.108   | 25.740      | 34.165   | 752.9    |
| json (stdlib) | mixed        | 27.633   | 28.705      | 28.949   | 752.9    |
| strata        | nested       | 6.143    | 7.341       | 7.445    | 507.4    |
| msgspec       | nested       | 14.850   | 23.328      | 32.240   | 509.1    |
| ujson         | nested       | 23.982   | 26.979      | 28.745   | 509.1    |
| json (stdlib) | nested       | 25.774   | 29.762      | 33.685   | 509.1    |
| orjson        | nested       | 14.807   | 40.796      | 46.224   | 507.4    |
| strata        | users.json   | 178.487  | 225.332     | 230.825  | 522.0    |
| orjson        | users.json   | 203.730  | 293.588     | 296.129  | 522.0    |
| msgspec       | users.json   | 218.895  | 308.136     | 323.769  | 555.2    |
| ujson         | users.json   | 304.858  | 432.945     | 438.431  | 677.6    |
| json (stdlib) | users.json   | 430.419  | 499.770     | 511.418  | 555.2    |
| strata        | users.ndjson | 185.674  | 234.639     | 238.051  | 617.8    |
| orjson        | users.ndjson | 282.325  | 330.819     | 340.791  | 617.9    |
| msgspec       | users.ndjson | 295.926  | 341.682     | 371.789  | 656.2    |
| ujson         | users.ndjson | 400.357  | 503.590     | 512.658  | 778.7    |
| json (stdlib) | users.ndjson | 475.074  | 533.696     | 562.240  | 657.9    |
| strata        | wide_arrays  | 35.611   | 39.836      | 47.226   | 475.3    |
| orjson        | wide_arrays  | 30.678   | 43.120      | 48.275   | 476.3    |
| msgspec       | wide_arrays  | 46.595   | 54.668      | 62.100   | 491.3    |
| ujson         | wide_arrays  | 67.960   | 77.778      | 88.159   | 490.3    |
| json (stdlib) | wide_arrays  | 135.082  | 143.467     | 159.587  | 492.3    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.289    | 2.308       | 2.333    | 515.9    |
| orjson  | flat        | 2.891    | 2.959       | 2.964    | 511.5    |
| msgspec | flat        | 2.873    | 3.026       | 3.193    | 511.6    |
| ujson   | flat        | 12.200   | 12.565      | 12.715   | 513.5    |
| json    | flat        | 22.933   | 22.966      | 23.445   | 513.5    |
| strata  | mixed       | 2.579    | 2.680       | 3.058    | 752.9    |
| msgspec | mixed       | 2.862    | 2.876       | 3.098    | 755.5    |
| orjson  | mixed       | 3.092    | 3.296       | 3.486    | 751.0    |
| ujson   | mixed       | 12.565   | 12.641      | 12.986   | 757.7    |
| json    | mixed       | 28.832   | 29.164      | 30.813   | 765.2    |
| strata  | nested      | 1.959    | 1.990       | 2.142    | 509.1    |
| msgspec | nested      | 1.999    | 2.012       | 2.071    | 509.1    |
| orjson  | nested      | 2.032    | 2.276       | 2.535    | 509.1    |
| ujson   | nested      | 9.808    | 9.966       | 10.404   | 509.1    |
| json    | nested      | 13.085   | 13.089      | 13.616   | 509.1    |
| strata  | users.json  | 51.233   | 51.252      | 52.089   | 590.7    |
| orjson  | users.json  | 81.243   | 82.918      | 86.511   | 747.5    |
| msgspec | users.json  | 83.780   | 84.146      | 87.420   | 943.8    |
| ujson   | users.json  | 300.902  | 302.688     | 308.245  | 1139.7   |
| json    | users.json  | 428.231  | 431.567     | 444.590  | 1354.3   |
| strata  | wide_arrays | 14.713   | 15.609      | 15.846   | 490.3    |
| msgspec | wide_arrays | 17.131   | 17.439      | 18.498   | 523.6    |
| orjson  | wide_arrays | 17.891   | 19.510      | 19.774   | 499.0    |
| ujson   | wide_arrays | 73.124   | 73.407      | 74.750   | 558.7    |
| json    | wide_arrays | 204.199  | 207.472     | 213.055  | 610.8    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 2.967    | 3.086       | 3.759    | 513.5    |
| msgspec       | flat        | 3.592    | 3.638       | 3.717    | 513.3    |
| orjson        | flat        | 3.414    | 4.006       | 4.510    | 513.3    |
| ujson         | flat        | 13.375   | 13.496      | 13.639   | 513.3    |
| json (stdlib) | flat        | 89.449   | 91.178      | 114.247  | 513.3    |
| msgspec       | mixed       | 3.602    | 3.609       | 3.638    | 769.4    |
| strata        | mixed       | 3.169    | 3.688       | 4.359    | 765.2    |
| orjson        | mixed       | 3.757    | 3.830       | 3.894    | 767.1    |
| ujson         | mixed       | 13.661   | 13.675      | 14.405   | 769.4    |
| json (stdlib) | mixed       | 98.697   | 100.444     | 111.698  | 769.4    |
| strata        | nested      | 2.275    | 2.517       | 2.659    | 509.1    |
| orjson        | nested      | 2.522    | 2.687       | 3.165    | 509.1    |
| msgspec       | nested      | 2.719    | 2.924       | 2.969    | 509.1    |
| ujson         | nested      | 10.415   | 10.709      | 10.945   | 509.1    |
| json (stdlib) | nested      | 122.595  | 126.004     | 144.991  | 509.1    |
| strata        | users.json  | 54.753   | 81.545      | 90.233   | 1398.2   |
| orjson        | users.json  | 111.236  | 113.167     | 122.333  | 1553.9   |
| msgspec       | users.json  | 108.417  | 119.290     | 124.442  | 1865.5   |
| ujson         | users.json  | 334.843  | 337.166     | 345.400  | 1709.8   |
| json (stdlib) | users.json  | 3079.857 | 3083.009    | 3102.914 | 1727.6   |
| strata        | wide_arrays | 16.854   | 16.943      | 17.275   | 610.8    |
| orjson        | wide_arrays | 19.492   | 19.868      | 24.691   | 639.5    |
| msgspec       | wide_arrays | 20.113   | 20.424      | 24.680   | 737.6    |
| ujson         | wide_arrays | 76.662   | 81.850      | 83.614   | 688.5    |
| json (stdlib) | wide_arrays | 452.214  | 457.626     | 493.248  | 737.6    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 92.412   | 4000    | 1507.7   |
|                                | orjson+jmespath    | 220.915  | 4000    | 1432.0   |
|                                | orjson+jsonpath-ng | 222.228  | 4000    | 1439.9   |
| all item prices                | strata             | 100.871  | 789913  | 1464.9   |
|                                | orjson+jmespath    | 556.936  | 4000    | 1525.8   |
|                                | orjson+jsonpath-ng | 1979.682 | 789913  | 948.2    |
| recursive price                | strata             | 98.502   | 789913  | 925.3    |
|                                | orjson+jsonpath-ng | 6453.009 | 789913  | 715.4    |
| flat:$.records\[\*\].id        | strata             | 4.834    | 5000    | 513.3    |
|                                | orjson+jmespath    | 11.366   | 5000    | 513.3    |
|                                | orjson+jsonpath-ng | 17.527   | 5000    | 510.3    |
| flat:$.records\[0\]            | strata             | 4.588    | 1       | 510.4    |
|                                | orjson+jmespath    | 9.451    | 1       | 510.4    |
|                                | orjson+jsonpath-ng | 9.457    | 1       | 510.4    |
| nested:$.records\[\*\].id      | strata             | 3.793    | 5000    | 509.1    |
|                                | orjson+jmespath    | 17.710   | 5000    | 509.1    |
|                                | orjson+jsonpath-ng | 24.654   | 5000    | 508.1    |
| nested:$.records\[0\]          | strata             | 4.237    | 1       | 508.1    |
|                                | orjson+jmespath    | 16.688   | 1       | 508.1    |
|                                | orjson+jsonpath-ng | 16.810   | 1       | 508.1    |
| wide_arrays:$.records\[\*\].id | strata             | 25.008   | 5000    | 737.6    |
|                                | orjson+jmespath    | 38.573   | 5000    | 737.6    |
|                                | orjson+jsonpath-ng | 45.401   | 5000    | 737.6    |
| wide_arrays:$.records\[0\]     | strata             | 24.974   | 1       | 737.7    |
|                                | orjson+jsonpath-ng | 35.692   | 1       | 737.7    |
|                                | orjson+jmespath    | 38.410   | 1       | 737.7    |
| mixed:$.records\[\*\].id       | strata             | 4.733    | 5000    | 769.4    |
|                                | orjson+jmespath    | 12.796   | 5000    | 769.4    |
|                                | orjson+jsonpath-ng | 18.833   | 5000    | 769.4    |
| mixed:$.records\[0\]           | strata             | 4.532    | 1       | 769.4    |
|                                | orjson+jsonpath-ng | 10.903   | 1       | 769.4    |
|                                | orjson+jmespath    | 11.293   | 1       | 769.4    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.744    | 4000    | 943.9    |
|                                            | jmespath    | 2.207    | 4000    | 944.0    |
|                                            | jsonpath-ng | 83.368   | 4000    | 945.7    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 45.318   | 789913  | 945.7    |
|                                            | jmespath    | 410.428  | 4000    | 960.6    |
|                                            | jsonpath-ng | 1732.966 | 789913  | 814.8    |
| $..price                                   | strata      | 61.764   | 789913  | 829.2    |
|                                            | jsonpath-ng | 6338.881 | 789913  | 828.0    |
| $.users\[?(@.age>30)\]                     | strata      | 0.825    | 3212    | 830.5    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 62.588   | 26406   | 830.5    |
| flat:$.records\[\*\].id                    | strata      | 0.385    | 5000    | 510.4    |
|                                            | jmespath    | 1.778    | 5000    | 510.4    |
|                                            | jsonpath-ng | 10.190   | 5000    | 509.4    |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 509.4    |
|                                            | jsonpath-ng | 0.015    | 1       | 509.4    |
|                                            | jmespath    | 0.015    | 1       | 509.4    |
| nested:$.records\[\*\].id                  | strata      | 0.555    | 5000    | 508.1    |
|                                            | jmespath    | 2.148    | 5000    | 508.1    |
|                                            | jsonpath-ng | 17.810   | 5000    | 508.1    |
| nested:$.records\[0\]                      | strata      | 0.008    | 1       | 508.1    |
|                                            | jsonpath-ng | 0.016    | 1       | 508.1    |
|                                            | jmespath    | 0.017    | 1       | 508.1    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.273    | 5000    | 737.7    |
|                                            | jmespath    | 1.862    | 5000    | 737.7    |
|                                            | jsonpath-ng | 16.437   | 5000    | 737.7    |
| wide_arrays:$.records\[0\]                 | strata      | 0.010    | 1       | 737.7    |
|                                            | jmespath    | 0.011    | 1       | 737.7    |
|                                            | jsonpath-ng | 0.012    | 1       | 737.7    |
| mixed:$.records\[\*\].id                   | strata      | 0.314    | 5000    | 769.4    |
|                                            | jmespath    | 1.719    | 5000    | 769.4    |
|                                            | jsonpath-ng | 12.331   | 5000    | 769.4    |
| mixed:$.records\[0\]                       | strata      | 0.005    | 1       | 769.4    |
|                                            | jmespath    | 0.009    | 1       | 769.4    |
|                                            | jsonpath-ng | 0.009    | 1       | 769.4    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **13.5% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **60.6% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **14.1% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **52.1% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **58.6% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **98.0% faster** than #2 (msgspec)  |
| loads (flat)        | **#1** / 5              | **28.3% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **29.2% faster** than #2 (orjson)   |
| dumps (flat)        | **#1** / 5              | **25.5% faster** than #2 (msgspec)  |
| dump (flat)         | **#1** / 5              | **15.1% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **137.1% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **141.0% faster** than #2 (orjson)  |
| dumps (nested)      | **#1** / 5              | **2.0% faster** than #2 (msgspec)   |
| dump (nested)       | **#1** / 5              | **10.9% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#1** / 5              | **24.6% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#2** / 5              | 16.1% behind #1 (orjson)            |
| dumps (wide_arrays) | **#1** / 5              | **16.4% faster** than #2 (msgspec)  |
| dump (wide_arrays)  | **#1** / 5              | **15.6% faster** than #2 (orjson)   |
| loads (mixed)       | **#1** / 5              | **78.5% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **48.5% faster** than #2 (orjson)   |
| dumps (mixed)       | **#1** / 5              | **11.0% faster** than #2 (msgspec)  |
| dump (mixed)        | **#1** / 5              | **13.7% faster** than #2 (msgspec)  |
| search (JSONPath)   | **#1** in 11/11 queries | avg **759% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
