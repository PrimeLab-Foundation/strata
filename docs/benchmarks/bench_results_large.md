# Strata Benchmark Results

Generated: 2026-03-19 02:49:29

## Environment

- Commit: 42f0bfe95d82803fac6378a50057ee7f0b40daca
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
| msgspec       | flat         | 8.827    | 10.086      | 10.093   | 554.8    |
| strata        | flat         | 7.296    | 10.823      | 11.472   | 554.8    |
| orjson        | flat         | 9.128    | 11.080      | 14.791   | 554.8    |
| ujson         | flat         | 17.768   | 18.045      | 18.155   | 554.8    |
| json (stdlib) | flat         | 25.747   | 26.279      | 27.725   | 557.8    |
| strata        | mixed        | 6.866    | 8.329       | 8.584    | 824.7    |
| orjson        | mixed        | 8.941    | 11.795      | 18.218   | 830.3    |
| msgspec       | mixed        | 12.460   | 12.686      | 13.375   | 830.3    |
| ujson         | mixed        | 18.667   | 18.812      | 19.821   | 830.3    |
| json (stdlib) | mixed        | 24.299   | 26.897      | 27.300   | 832.7    |
| strata        | nested       | 7.811    | 8.848       | 9.340    | 553.2    |
| msgspec       | nested       | 15.583   | 17.828      | 27.863   | 553.2    |
| orjson        | nested       | 13.927   | 18.985      | 25.169   | 553.2    |
| ujson         | nested       | 16.140   | 21.823      | 40.821   | 553.2    |
| json (stdlib) | nested       | 26.158   | 26.574      | 27.586   | 553.2    |
| strata        | users.json   | 175.983  | 220.292     | 222.871  | 340.1    |
| orjson        | users.json   | 223.654  | 312.018     | 330.221  | 478.2    |
| msgspec       | users.json   | 228.644  | 322.680     | 339.073  | 467.7    |
| ujson         | users.json   | 320.694  | 396.262     | 430.786  | 590.1    |
| json (stdlib) | users.json   | 425.769  | 511.719     | 532.895  | 511.6    |
| strata        | users.ndjson | 188.141  | 231.407     | 232.688  | 609.0    |
| orjson        | users.ndjson | 256.170  | 329.521     | 347.090  | 659.4    |
| msgspec       | users.ndjson | 283.684  | 331.149     | 356.641  | 653.9    |
| ujson         | users.ndjson | 369.967  | 434.967     | 469.032  | 776.4    |
| json (stdlib) | users.ndjson | 476.469  | 523.211     | 547.980  | 656.6    |
| strata        | wide_arrays  | 30.980   | 36.497      | 39.595   | 421.8    |
| orjson        | wide_arrays  | 28.945   | 38.667      | 39.835   | 447.3    |
| msgspec       | wide_arrays  | 36.200   | 48.454      | 49.467   | 448.4    |
| ujson         | wide_arrays  | 66.341   | 70.437      | 72.124   | 448.4    |
| json (stdlib) | wide_arrays  | 126.667  | 139.092     | 146.850  | 462.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 7.617    | 8.678       | 8.754    | 557.8    |
| orjson        | flat         | 8.863    | 10.441      | 10.500   | 557.8    |
| msgspec       | flat         | 9.521    | 11.167      | 11.983   | 560.8    |
| ujson         | flat         | 17.092   | 22.597      | 24.535   | 560.8    |
| json (stdlib) | flat         | 24.537   | 25.035      | 25.140   | 560.8    |
| strata        | mixed        | 7.365    | 8.892       | 8.975    | 832.7    |
| msgspec       | mixed        | 10.786   | 12.970      | 13.198   | 835.1    |
| ujson         | mixed        | 16.640   | 18.134      | 21.006   | 835.1    |
| orjson        | mixed        | 11.099   | 19.942      | 21.766   | 832.7    |
| json (stdlib) | mixed        | 25.006   | 27.472      | 30.692   | 835.1    |
| strata        | nested       | 7.081    | 8.390       | 8.812    | 553.2    |
| msgspec       | nested       | 16.936   | 20.137      | 29.866   | 554.9    |
| orjson        | nested       | 18.623   | 20.638      | 20.782   | 553.2    |
| ujson         | nested       | 19.982   | 23.278      | 36.688   | 554.9    |
| json (stdlib) | nested       | 24.443   | 28.010      | 28.446   | 554.9    |
| strata        | users.json   | 185.373  | 247.050     | 247.531  | 522.4    |
| orjson        | users.json   | 200.072  | 312.708     | 322.406  | 522.4    |
| msgspec       | users.json   | 218.377  | 316.118     | 336.754  | 555.5    |
| ujson         | users.json   | 312.423  | 404.050     | 437.236  | 678.0    |
| json (stdlib) | users.json   | 432.661  | 535.658     | 552.460  | 555.5    |
| strata        | users.ndjson | 197.580  | 236.793     | 240.740  | 659.5    |
| msgspec       | users.ndjson | 268.615  | 336.290     | 350.196  | 698.0    |
| orjson        | users.ndjson | 269.523  | 341.002     | 341.421  | 659.7    |
| ujson         | users.ndjson | 397.630  | 475.030     | 498.461  | 820.5    |
| json (stdlib) | users.ndjson | 472.654  | 515.991     | 548.628  | 699.6    |
| strata        | wide_arrays  | 31.309   | 38.458      | 39.425   | 464.4    |
| orjson        | wide_arrays  | 29.851   | 40.094      | 54.864   | 465.4    |
| msgspec       | wide_arrays  | 42.442   | 48.520      | 49.261   | 480.4    |
| ujson         | wide_arrays  | 63.819   | 75.380      | 86.833   | 479.4    |
| json (stdlib) | wide_arrays  | 126.553  | 141.774     | 146.700  | 481.4    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.352    | 2.409       | 2.513    | 560.8    |
| msgspec | flat        | 2.858    | 2.927       | 2.958    | 556.4    |
| orjson  | flat        | 2.942    | 2.969       | 3.071    | 556.4    |
| ujson   | flat        | 12.168   | 12.265      | 12.353   | 558.4    |
| json    | flat        | 22.576   | 22.591      | 22.802   | 558.4    |
| strata  | mixed       | 2.805    | 2.825       | 2.955    | 835.1    |
| msgspec | mixed       | 2.927    | 3.027       | 3.076    | 839.6    |
| orjson  | mixed       | 3.088    | 3.161       | 3.192    | 835.7    |
| ujson   | mixed       | 12.458   | 12.505      | 12.633   | 839.6    |
| json    | mixed       | 27.836   | 28.019      | 28.062   | 844.7    |
| msgspec | nested      | 2.031    | 2.031       | 2.179    | 555.0    |
| orjson  | nested      | 2.052    | 2.056       | 2.091    | 555.0    |
| strata  | nested      | 2.096    | 2.250       | 2.322    | 554.9    |
| ujson   | nested      | 10.393   | 10.422      | 10.562   | 555.0    |
| json    | nested      | 12.972   | 13.010      | 13.743   | 555.0    |
| strata  | users.json  | 62.865   | 65.299      | 69.704   | 591.1    |
| msgspec | users.json  | 84.751   | 86.125      | 92.341   | 945.0    |
| orjson  | users.json  | 85.100   | 87.498      | 95.781   | 747.9    |
| ujson   | users.json  | 317.875  | 319.423     | 322.263  | 1140.2   |
| json    | users.json  | 443.942  | 448.503     | 454.712  | 1354.9   |
| strata  | wide_arrays | 15.382   | 15.471      | 15.498   | 479.4    |
| orjson  | wide_arrays | 16.449   | 16.462      | 16.650   | 522.6    |
| msgspec | wide_arrays | 17.493   | 18.029      | 18.490   | 571.7    |
| ujson   | wide_arrays | 73.010   | 73.892      | 74.678   | 620.7    |
| json    | wide_arrays | 199.861  | 200.461     | 202.082  | 672.8    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 2.824    | 2.946       | 2.959    | 558.4    |
| msgspec       | flat        | 3.468    | 3.488       | 3.647    | 558.2    |
| orjson        | flat        | 3.710    | 4.006       | 4.024    | 558.2    |
| ujson         | flat        | 13.097   | 13.496      | 13.565   | 558.2    |
| json (stdlib) | flat        | 81.900   | 82.325      | 82.830   | 558.2    |
| msgspec       | mixed       | 3.482    | 3.511       | 3.533    | 847.0    |
| strata        | mixed       | 3.453    | 3.598       | 4.260    | 844.7    |
| orjson        | mixed       | 3.732    | 3.747       | 3.876    | 847.0    |
| ujson         | mixed       | 13.442   | 13.449      | 13.914   | 847.0    |
| json (stdlib) | mixed       | 96.468   | 96.552      | 96.854   | 847.0    |
| msgspec       | nested      | 2.364    | 2.365       | 2.777    | 555.1    |
| orjson        | nested      | 2.837    | 2.888       | 3.068    | 555.0    |
| strata        | nested      | 2.540    | 3.267       | 3.341    | 555.0    |
| ujson         | nested      | 10.290   | 10.535      | 10.979   | 555.1    |
| json (stdlib) | nested      | 123.488  | 123.602     | 132.230  | 555.1    |
| strata        | users.json  | 70.091   | 71.354      | 79.516   | 1398.8   |
| orjson        | users.json  | 101.139  | 103.988     | 113.942  | 1554.5   |
| msgspec       | users.json  | 102.155  | 106.277     | 114.655  | 1865.8   |
| ujson         | users.json  | 329.730  | 334.702     | 339.947  | 1710.2   |
| json (stdlib) | users.json  | 2957.965 | 2964.772    | 2966.941 | 1727.9   |
| strata        | wide_arrays | 17.336   | 17.508      | 22.304   | 672.8    |
| orjson        | wide_arrays | 19.122   | 19.412      | 19.557   | 721.8    |
| msgspec       | wide_arrays | 20.147   | 20.393      | 20.809   | 819.9    |
| ujson         | wide_arrays | 75.479   | 75.786      | 76.030   | 770.8    |
| json (stdlib) | wide_arrays | 444.706  | 448.303     | 449.455  | 819.9    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 86.997   | 4000    | 1507.9   |
|                                | orjson+jmespath    | 205.274  | 4000    | 1432.2   |
|                                | orjson+jsonpath-ng | 208.585  | 4000    | 1440.0   |
| all item prices                | strata             | 94.763   | 789913  | 1465.1   |
|                                | orjson+jmespath    | 542.633  | 4000    | 1526.0   |
|                                | orjson+jsonpath-ng | 1915.666 | 789913  | 1103.6   |
| recursive price                | strata             | 93.639   | 789913  | 1078.1   |
|                                | orjson+jsonpath-ng | 6236.816 | 789913  | 712.6    |
| flat:$.records\[\*\].id        | strata             | 4.585    | 5000    | 558.2    |
|                                | orjson+jmespath    | 10.898   | 5000    | 558.2    |
|                                | orjson+jsonpath-ng | 16.745   | 5000    | 555.2    |
| flat:$.records\[0\]            | strata             | 4.762    | 1       | 555.2    |
|                                | orjson+jsonpath-ng | 10.173   | 1       | 555.2    |
|                                | orjson+jmespath    | 11.124   | 1       | 555.2    |
| nested:$.records\[\*\].id      | strata             | 3.546    | 5000    | 555.1    |
|                                | orjson+jmespath    | 16.756   | 5000    | 555.1    |
|                                | orjson+jsonpath-ng | 23.191   | 5000    | 555.1    |
| nested:$.records\[0\]          | strata             | 3.519    | 1       | 555.1    |
|                                | orjson+jmespath    | 14.514   | 1       | 555.1    |
|                                | orjson+jsonpath-ng | 14.653   | 1       | 555.1    |
| wide_arrays:$.records\[\*\].id | strata             | 23.232   | 5000    | 819.9    |
|                                | orjson+jmespath    | 36.344   | 5000    | 819.9    |
|                                | orjson+jsonpath-ng | 46.064   | 5000    | 819.9    |
| wide_arrays:$.records\[0\]     | strata             | 23.155   | 1       | 819.9    |
|                                | orjson+jsonpath-ng | 34.689   | 1       | 819.9    |
|                                | orjson+jmespath    | 36.112   | 1       | 819.9    |
| mixed:$.records\[\*\].id       | strata             | 4.440    | 5000    | 847.0    |
|                                | orjson+jmespath    | 12.642   | 5000    | 847.0    |
|                                | orjson+jsonpath-ng | 20.128   | 5000    | 847.0    |
| mixed:$.records\[0\]           | strata             | 4.482    | 1       | 847.0    |
|                                | orjson+jsonpath-ng | 11.027   | 1       | 847.0    |
|                                | orjson+jmespath    | 11.583   | 1       | 847.0    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.740    | 4000    | 941.1    |
|                                            | jmespath    | 2.189    | 4000    | 941.1    |
|                                            | jsonpath-ng | 79.546   | 4000    | 942.8    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 41.818   | 789913  | 789.0    |
|                                            | jmespath    | 404.170  | 4000    | 803.8    |
|                                            | jsonpath-ng | 1718.479 | 789913  | 878.8    |
| $..price                                   | strata      | 56.245   | 789913  | 894.1    |
|                                            | jsonpath-ng | 6064.764 | 789913  | 994.1    |
| $.users\[?(@.age>30)\]                     | strata      | 0.815    | 3212    | 828.2    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 64.104   | 26406   | 827.2    |
| flat:$.records\[\*\].id                    | strata      | 0.385    | 5000    | 555.2    |
|                                            | jmespath    | 1.735    | 5000    | 555.2    |
|                                            | jsonpath-ng | 10.227   | 5000    | 555.2    |
| flat:$.records\[0\]                        | strata      | 0.007    | 1       | 555.2    |
|                                            | jmespath    | 0.008    | 1       | 555.2    |
|                                            | jsonpath-ng | 0.014    | 1       | 555.2    |
| nested:$.records\[\*\].id                  | strata      | 0.448    | 5000    | 555.1    |
|                                            | jmespath    | 1.768    | 5000    | 555.1    |
|                                            | jsonpath-ng | 17.462   | 5000    | 555.1    |
| nested:$.records\[0\]                      | strata      | 0.009    | 1       | 555.1    |
|                                            | jsonpath-ng | 0.012    | 1       | 555.1    |
|                                            | jmespath    | 0.015    | 1       | 555.1    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.261    | 5000    | 819.9    |
|                                            | jmespath    | 1.798    | 5000    | 819.9    |
|                                            | jsonpath-ng | 16.029   | 5000    | 819.9    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 819.9    |
|                                            | jmespath    | 0.010    | 1       | 819.9    |
|                                            | jsonpath-ng | 0.011    | 1       | 819.9    |
| mixed:$.records\[\*\].id                   | strata      | 0.331    | 5000    | 847.0    |
|                                            | jmespath    | 1.901    | 5000    | 847.0    |
|                                            | jsonpath-ng | 13.637   | 5000    | 847.0    |
| mixed:$.records\[0\]                       | strata      | 0.010    | 1       | 847.0    |
|                                            | jsonpath-ng | 0.014    | 1       | 847.0    |
|                                            | jmespath    | 0.016    | 1       | 847.0    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **27.1% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **36.2% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **7.9% faster** than #2 (orjson)    |
| load (NDJSON file)  | **#1** / 5              | **36.0% faster** than #2 (msgspec)  |
| dumps               | **#1** / 5              | **34.8% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **44.3% faster** than #2 (orjson)   |
| loads (flat)        | **#1** / 5              | **21.0% faster** than #2 (msgspec)  |
| load (flat)         | **#1** / 5              | **16.4% faster** than #2 (orjson)   |
| dumps (flat)        | **#1** / 5              | **21.5% faster** than #2 (msgspec)  |
| dump (flat)         | **#1** / 5              | **22.8% faster** than #2 (msgspec)  |
| loads (nested)      | **#1** / 5              | **78.3% faster** than #2 (orjson)   |
| load (nested)       | **#1** / 5              | **139.2% faster** than #2 (msgspec) |
| dumps (nested)      | **#3** / 5              | 3.2% behind #1 (msgspec)            |
| dump (nested)       | **#2** / 5              | 7.4% behind #1 (msgspec)            |
| loads (wide_arrays) | **#2** / 5              | 7.0% behind #1 (orjson)             |
| load (wide_arrays)  | **#2** / 5              | 4.9% behind #1 (orjson)             |
| dumps (wide_arrays) | **#1** / 5              | **6.9% faster** than #2 (orjson)    |
| dump (wide_arrays)  | **#1** / 5              | **10.3% faster** than #2 (orjson)   |
| loads (mixed)       | **#1** / 5              | **30.2% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **46.4% faster** than #2 (msgspec)  |
| dumps (mixed)       | **#1** / 5              | **4.3% faster** than #2 (msgspec)   |
| dump (mixed)        | **#1** / 5              | **0.8% faster** than #2 (msgspec)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **777% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
