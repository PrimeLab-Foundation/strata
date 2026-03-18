# Strata Benchmark Results

Generated: 2026-03-18 15:04:55

## Environment

- Commit: 26bebc11f29d91458f5425c35bb69bdfbc551de7
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
| strata        | flat         | 7.070    | 8.217       | 8.362    | 505.1    |
| orjson        | flat         | 8.050    | 9.522       | 9.673    | 505.1    |
| msgspec       | flat         | 8.219    | 9.644       | 9.673    | 505.1    |
| ujson         | flat         | 13.386   | 15.365      | 15.511   | 505.1    |
| json (stdlib) | flat         | 22.677   | 23.959      | 23.980   | 508.0    |
| strata        | mixed        | 7.015    | 8.368       | 8.452    | 743.6    |
| orjson        | mixed        | 8.901    | 11.439      | 11.660   | 749.2    |
| msgspec       | mixed        | 9.504    | 12.117      | 12.449   | 749.2    |
| ujson         | mixed        | 13.867   | 16.537      | 16.810   | 749.2    |
| json (stdlib) | mixed        | 24.004   | 26.307      | 26.396   | 751.6    |
| strata        | nested       | 6.063    | 7.288       | 7.372    | 502.4    |
| orjson        | nested       | 12.579   | 16.717      | 16.839   | 502.4    |
| msgspec       | nested       | 13.224   | 17.251      | 17.345   | 502.4    |
| ujson         | nested       | 15.346   | 20.541      | 20.564   | 502.4    |
| json (stdlib) | nested       | 21.175   | 25.171      | 25.788   | 502.4    |
| strata        | users.json   | 173.186  | 216.635     | 220.706  | 339.9    |
| orjson        | users.json   | 188.385  | 272.113     | 273.526  | 477.9    |
| msgspec       | users.json   | 198.778  | 275.536     | 281.648  | 467.4    |
| ujson         | users.json   | 285.721  | 379.120     | 383.439  | 589.8    |
| json (stdlib) | users.json   | 386.052  | 465.061     | 468.685  | 511.3    |
| strata        | users.ndjson | 186.044  | 226.304     | 229.306  | 562.6    |
| orjson        | users.ndjson | 236.139  | 293.891     | 294.630  | 612.8    |
| msgspec       | users.ndjson | 252.370  | 310.725     | 312.243  | 607.3    |
| ujson         | users.ndjson | 335.612  | 414.465     | 423.610  | 729.8    |
| json (stdlib) | users.ndjson | 440.462  | 490.743     | 500.415  | 610.0    |
| orjson        | wide_arrays  | 28.057   | 36.207      | 38.733   | 450.6    |
| strata        | wide_arrays  | 32.664   | 39.109      | 41.254   | 425.1    |
| msgspec       | wide_arrays  | 35.861   | 44.324      | 46.345   | 460.3    |
| ujson         | wide_arrays  | 58.951   | 67.598      | 69.303   | 460.3    |
| json (stdlib) | wide_arrays  | 121.438  | 132.163     | 133.580  | 474.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 7.549    | 8.544       | 8.831    | 508.0    |
| orjson        | flat         | 8.526    | 10.048      | 10.250   | 508.0    |
| msgspec       | flat         | 8.915    | 10.167      | 10.543   | 511.0    |
| ujson         | flat         | 14.222   | 16.313      | 16.537   | 511.0    |
| json (stdlib) | flat         | 23.046   | 24.626      | 24.767   | 511.0    |
| strata        | mixed        | 7.614    | 9.051       | 9.097    | 751.6    |
| orjson        | mixed        | 9.482    | 12.020      | 12.365   | 751.6    |
| msgspec       | mixed        | 10.508   | 12.564      | 13.729   | 754.0    |
| ujson         | mixed        | 14.536   | 17.720      | 17.836   | 754.0    |
| json (stdlib) | mixed        | 25.147   | 28.508      | 28.775   | 754.0    |
| strata        | nested       | 6.426    | 7.527       | 7.608    | 502.4    |
| orjson        | nested       | 13.588   | 17.231      | 17.401   | 502.4    |
| msgspec       | nested       | 13.243   | 17.597      | 18.087   | 504.2    |
| ujson         | nested       | 16.001   | 20.840      | 21.377   | 504.2    |
| json (stdlib) | nested       | 21.981   | 25.866      | 26.734   | 504.2    |
| strata        | users.json   | 175.741  | 220.345     | 221.646  | 522.1    |
| orjson        | users.json   | 190.706  | 271.478     | 272.941  | 522.1    |
| msgspec       | users.json   | 199.448  | 278.587     | 280.049  | 555.3    |
| ujson         | users.json   | 280.053  | 384.025     | 391.231  | 677.8    |
| json (stdlib) | users.json   | 392.111  | 462.622     | 475.543  | 555.3    |
| strata        | users.ndjson | 191.614  | 232.343     | 232.700  | 612.8    |
| orjson        | users.ndjson | 239.680  | 295.111     | 299.856  | 613.0    |
| msgspec       | users.ndjson | 250.500  | 309.786     | 311.974  | 651.3    |
| ujson         | users.ndjson | 344.988  | 420.813     | 432.889  | 773.8    |
| json (stdlib) | users.ndjson | 436.077  | 491.801     | 502.029  | 652.9    |
| orjson        | wide_arrays  | 29.733   | 38.215      | 40.841   | 477.4    |
| strata        | wide_arrays  | 33.937   | 39.556      | 41.799   | 476.4    |
| msgspec       | wide_arrays  | 38.157   | 46.377      | 48.148   | 492.4    |
| ujson         | wide_arrays  | 62.912   | 71.354      | 72.726   | 491.4    |
| json (stdlib) | wide_arrays  | 125.936  | 134.661     | 135.536  | 493.4    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.228    | 2.239       | 2.241    | 511.0    |
| msgspec | flat        | 2.842    | 2.905       | 2.948    | 506.6    |
| orjson  | flat        | 2.869    | 2.929       | 3.001    | 506.6    |
| ujson   | flat        | 11.970   | 11.978      | 12.014   | 508.6    |
| json    | flat        | 22.668   | 22.705      | 22.787   | 508.6    |
| strata  | mixed       | 2.519    | 2.547       | 2.645    | 754.0    |
| msgspec | mixed       | 2.820    | 2.897       | 3.038    | 756.5    |
| orjson  | mixed       | 3.073    | 4.944       | 5.342    | 752.0    |
| ujson   | mixed       | 12.541   | 20.248      | 21.148   | 760.8    |
| json    | mixed       | 28.054   | 28.086      | 28.189   | 768.2    |
| strata  | nested      | 1.824    | 1.825       | 1.837    | 504.2    |
| msgspec | nested      | 1.936    | 1.939       | 1.952    | 504.2    |
| orjson  | nested      | 1.932    | 1.940       | 1.972    | 504.2    |
| ujson   | nested      | 9.800    | 9.981       | 10.014   | 504.2    |
| json    | nested      | 12.842   | 12.855      | 12.859   | 504.2    |
| strata  | users.json  | 43.827   | 43.836      | 44.075   | 590.8    |
| msgspec | users.json  | 76.342   | 76.422      | 76.957   | 942.7    |
| orjson  | users.json  | 77.391   | 77.488      | 77.658   | 747.2    |
| ujson   | users.json  | 294.686  | 295.161     | 297.747  | 1139.4   |
| json    | users.json  | 405.683  | 405.826     | 411.364  | 1354.0   |
| strata  | wide_arrays | 13.655   | 13.674      | 13.786   | 491.4    |
| msgspec | wide_arrays | 16.757   | 17.057      | 17.075   | 524.6    |
| orjson  | wide_arrays | 17.390   | 17.581      | 17.588   | 500.0    |
| ujson   | wide_arrays | 70.086   | 70.146      | 71.804   | 559.7    |
| json    | wide_arrays | 196.042  | 197.418     | 198.395  | 611.8    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 2.587    | 2.590       | 2.607    | 508.6    |
| msgspec       | flat        | 3.447    | 3.451       | 3.548    | 508.4    |
| orjson        | flat        | 3.271    | 3.516       | 3.667    | 508.4    |
| ujson         | flat        | 12.619   | 12.684      | 12.827   | 508.4    |
| json (stdlib) | flat        | 80.840   | 80.864      | 81.140   | 508.4    |
| strata        | mixed       | 2.848    | 2.962       | 3.040    | 768.2    |
| msgspec       | mixed       | 3.345    | 3.383       | 3.483    | 772.4    |
| orjson        | mixed       | 3.547    | 3.598       | 3.675    | 770.2    |
| ujson         | mixed       | 13.199   | 13.344      | 13.370   | 772.4    |
| json (stdlib) | mixed       | 95.765   | 95.769      | 95.786   | 772.4    |
| strata        | nested      | 2.161    | 2.166       | 2.186    | 504.2    |
| msgspec       | nested      | 2.346    | 2.428       | 2.537    | 504.2    |
| orjson        | nested      | 2.385    | 2.447       | 2.465    | 504.2    |
| ujson         | nested      | 10.136   | 10.153      | 10.291   | 504.2    |
| json (stdlib) | nested      | 118.029  | 118.098     | 118.261  | 504.2    |
| strata        | users.json  | 49.660   | 49.724      | 50.974   | 1397.9   |
| msgspec       | users.json  | 82.507   | 83.256      | 83.474   | 1865.2   |
| orjson        | users.json  | 83.172   | 83.940      | 84.495   | 1553.6   |
| ujson         | users.json  | 300.921  | 303.099     | 303.291  | 1709.5   |
| json (stdlib) | users.json  | 2930.637 | 2931.456    | 2948.476 | 1727.2   |
| strata        | wide_arrays | 15.412   | 15.478      | 15.624   | 611.8    |
| orjson        | wide_arrays | 18.994   | 19.016      | 19.716   | 640.5    |
| msgspec       | wide_arrays | 19.787   | 19.847      | 19.891   | 738.7    |
| ujson         | wide_arrays | 74.297   | 75.234      | 75.561   | 689.6    |
| json (stdlib) | wide_arrays | 438.936  | 440.367     | 444.768  | 738.7    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 84.991   | 4000    | 1507.4   |
|                                | orjson+jmespath    | 203.128  | 4000    | 1860.8   |
|                                | orjson+jsonpath-ng | 203.128  | 4000    | 1867.7   |
| all item prices                | strata             | 92.665   | 789913  | 1894.7   |
|                                | orjson+jmespath    | 524.459  | 4000    | 1371.8   |
|                                | orjson+jsonpath-ng | 1823.144 | 789913  | 1402.7   |
| recursive price                | strata             | 93.663   | 789913  | 944.0    |
|                                | orjson+jsonpath-ng | 6066.647 | 789913  | 716.5    |
| flat:$.records\[\*\].id        | strata             | 4.548    | 5000    | 508.4    |
|                                | orjson+jmespath    | 10.768   | 5000    | 508.4    |
|                                | orjson+jsonpath-ng | 16.246   | 5000    | 505.4    |
| flat:$.records\[0\]            | strata             | 4.444    | 1       | 505.4    |
|                                | orjson+jsonpath-ng | 9.006    | 1       | 505.4    |
|                                | orjson+jmespath    | 9.111    | 1       | 505.4    |
| nested:$.records\[\*\].id      | strata             | 3.522    | 5000    | 504.2    |
|                                | orjson+jmespath    | 15.512   | 5000    | 504.2    |
|                                | orjson+jsonpath-ng | 21.274   | 5000    | 504.2    |
| nested:$.records\[0\]          | strata             | 3.448    | 1       | 504.2    |
|                                | orjson+jsonpath-ng | 13.851   | 1       | 504.2    |
|                                | orjson+jmespath    | 13.933   | 1       | 504.2    |
| wide_arrays:$.records\[\*\].id | strata             | 26.879   | 5000    | 738.7    |
|                                | orjson+jmespath    | 35.763   | 5000    | 738.7    |
|                                | orjson+jsonpath-ng | 42.726   | 5000    | 738.7    |
| wide_arrays:$.records\[0\]     | strata             | 26.589   | 1       | 738.7    |
|                                | orjson+jsonpath-ng | 34.119   | 1       | 738.7    |
|                                | orjson+jmespath    | 35.985   | 1       | 738.7    |
| mixed:$.records\[\*\].id       | strata             | 4.666    | 5000    | 772.4    |
|                                | orjson+jmespath    | 12.156   | 5000    | 772.4    |
|                                | orjson+jsonpath-ng | 17.617   | 5000    | 772.4    |
| mixed:$.records\[0\]           | strata             | 4.567    | 1       | 772.4    |
|                                | orjson+jsonpath-ng | 10.404   | 1       | 772.4    |
|                                | orjson+jmespath    | 10.406   | 1       | 772.4    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.700    | 4000    | 945.0    |
|                                            | jmespath    | 2.191    | 4000    | 945.0    |
|                                            | jsonpath-ng | 81.393   | 4000    | 946.7    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 43.148   | 789913  | 946.8    |
|                                            | jmespath    | 394.803  | 4000    | 961.7    |
|                                            | jsonpath-ng | 1622.066 | 789913  | 810.0    |
| $..price                                   | strata      | 57.553   | 789913  | 823.3    |
|                                            | jsonpath-ng | 6013.958 | 789913  | 822.1    |
| $.users\[?(@.age>30)\]                     | strata      | 0.826    | 3212    | 825.7    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 61.940   | 26406   | 824.7    |
| flat:$.records\[\*\].id                    | strata      | 0.315    | 5000    | 505.4    |
|                                            | jmespath    | 1.652    | 5000    | 505.4    |
|                                            | jsonpath-ng | 9.382    | 5000    | 505.4    |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 505.4    |
|                                            | jsonpath-ng | 0.009    | 1       | 505.4    |
|                                            | jmespath    | 0.014    | 1       | 505.4    |
| nested:$.records\[\*\].id                  | strata      | 0.430    | 5000    | 504.2    |
|                                            | jmespath    | 1.750    | 5000    | 504.2    |
|                                            | jsonpath-ng | 15.068   | 5000    | 504.2    |
| nested:$.records\[0\]                      | jmespath    | 0.010    | 1       | 504.2    |
|                                            | jsonpath-ng | 0.011    | 1       | 504.2    |
|                                            | strata      | 0.011    | 1       | 504.2    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.265    | 5000    | 738.7    |
|                                            | jmespath    | 1.747    | 5000    | 738.7    |
|                                            | jsonpath-ng | 15.790   | 5000    | 738.7    |
| wide_arrays:$.records\[0\]                 | strata      | 0.007    | 1       | 738.7    |
|                                            | jsonpath-ng | 0.011    | 1       | 738.7    |
|                                            | jmespath    | 0.020    | 1       | 738.7    |
| mixed:$.records\[\*\].id                   | strata      | 0.303    | 5000    | 772.4    |
|                                            | jmespath    | 1.697    | 5000    | 772.4    |
|                                            | jsonpath-ng | 11.588   | 5000    | 772.4    |
| mixed:$.records\[0\]                       | jmespath    | 0.008    | 1       | 772.4    |
|                                            | strata      | 0.008    | 1       | 772.4    |
|                                            | jsonpath-ng | 0.009    | 1       | 772.4    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **8.8% faster** than #2 (orjson)    |
| loads (NDJSON)      | **#1** / 5              | **26.9% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **8.5% faster** than #2 (orjson)    |
| load (NDJSON file)  | **#1** / 5              | **25.1% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **74.2% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **66.1% faster** than #2 (msgspec)  |
| loads (flat)        | **#1** / 5              | **13.9% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **12.9% faster** than #2 (orjson)   |
| dumps (flat)        | **#1** / 5              | **27.6% faster** than #2 (msgspec)  |
| dump (flat)         | **#1** / 5              | **26.5% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **107.5% faster** than #2 (orjson)  |
| load (nested)       | **#1** / 5              | **106.1% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **5.9% faster** than #2 (orjson)    |
| dump (nested)       | **#1** / 5              | **8.6% faster** than #2 (msgspec)   |
| loads (wide_arrays) | **#2** / 5              | 16.4% behind #1 (orjson)            |
| load (wide_arrays)  | **#2** / 5              | 14.1% behind #1 (orjson)            |
| dumps (wide_arrays) | **#1** / 5              | **22.7% faster** than #2 (msgspec)  |
| dump (wide_arrays)  | **#1** / 5              | **23.2% faster** than #2 (orjson)   |
| loads (mixed)       | **#1** / 5              | **26.9% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **24.5% faster** than #2 (orjson)   |
| dumps (mixed)       | **#1** / 5              | **12.0% faster** than #2 (msgspec)  |
| dump (mixed)        | **#1** / 5              | **17.4% faster** than #2 (msgspec)  |
| search (JSONPath)   | **#1** in 11/11 queries | avg **747% faster**                 |
| query (JSONPath)    | **#1** in 11/13 queries | -                                   |
