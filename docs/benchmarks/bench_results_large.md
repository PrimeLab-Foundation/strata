# Strata Benchmark Results

Generated: 2026-03-16 02:18:39

## Environment

- Commit: 3f0d7fe886713eb553cbe090544e067cb4f6daf3
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
| strata        | flat         | 6.448    | 7.387       | 7.612    | 509.8    |
| orjson        | flat         | 8.314    | 9.876       | 12.926   | 509.8    |
| msgspec       | flat         | 8.672    | 10.212      | 14.088   | 509.8    |
| ujson         | flat         | 14.213   | 15.674      | 16.029   | 509.8    |
| json (stdlib) | flat         | 24.232   | 24.280      | 24.478   | 512.7    |
| strata        | mixed        | 6.417    | 7.793       | 8.585    | 681.1    |
| orjson        | mixed        | 9.848    | 11.765      | 11.798   | 686.7    |
| msgspec       | mixed        | 9.914    | 12.674      | 15.713   | 686.7    |
| ujson         | mixed        | 13.665   | 17.124      | 22.619   | 686.7    |
| json (stdlib) | mixed        | 23.698   | 26.333      | 28.430   | 689.1    |
| strata        | nested       | 5.642    | 6.779       | 7.117    | 508.2    |
| orjson        | nested       | 13.615   | 17.440      | 31.714   | 508.2    |
| msgspec       | nested       | 13.054   | 17.458      | 25.675   | 508.2    |
| ujson         | nested       | 15.811   | 20.903      | 30.476   | 508.2    |
| json (stdlib) | nested       | 26.254   | 26.659      | 41.686   | 508.2    |
| strata        | users.json   | 162.880  | 209.753     | 211.057  | 339.7    |
| orjson        | users.json   | 197.713  | 287.257     | 299.168  | 477.7    |
| msgspec       | users.json   | 224.155  | 295.205     | 317.102  | 467.2    |
| ujson         | users.json   | 304.015  | 404.609     | 451.267  | 589.6    |
| json (stdlib) | users.json   | 416.500  | 501.253     | 503.017  | 511.1    |
| strata        | users.ndjson | 168.034  | 203.977     | 208.816  | 566.2    |
| orjson        | users.ndjson | 279.160  | 326.618     | 332.931  | 616.5    |
| msgspec       | users.ndjson | 267.980  | 352.022     | 359.356  | 611.0    |
| ujson         | users.ndjson | 377.788  | 446.496     | 447.426  | 733.5    |
| json (stdlib) | users.ndjson | 476.629  | 514.598     | 546.870  | 613.7    |
| strata        | wide_arrays  | 31.325   | 38.584      | 40.010   | 421.1    |
| orjson        | wide_arrays  | 28.268   | 42.004      | 46.418   | 446.7    |
| msgspec       | wide_arrays  | 34.970   | 46.796      | 51.708   | 448.7    |
| ujson         | wide_arrays  | 58.856   | 69.564      | 75.878   | 447.7    |
| json (stdlib) | wide_arrays  | 133.007  | 138.896     | 148.353  | 462.7    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 6.956    | 7.967       | 8.141    | 512.8    |
| msgspec       | flat         | 9.654    | 10.514      | 11.019   | 515.7    |
| orjson        | flat         | 10.034   | 10.539      | 10.911   | 512.8    |
| ujson         | flat         | 14.815   | 19.361      | 19.574   | 515.7    |
| json (stdlib) | flat         | 25.086   | 25.319      | 25.944   | 515.7    |
| strata        | mixed        | 6.628    | 8.062       | 8.526    | 689.1    |
| msgspec       | mixed        | 11.017   | 13.243      | 22.988   | 691.5    |
| orjson        | mixed        | 13.464   | 14.275      | 15.736   | 689.1    |
| ujson         | mixed        | 15.186   | 18.489      | 18.817   | 691.5    |
| json (stdlib) | mixed        | 24.723   | 27.389      | 31.159   | 691.5    |
| strata        | nested       | 6.098    | 6.884       | 7.130    | 508.2    |
| msgspec       | nested       | 15.866   | 17.951      | 21.073   | 508.2    |
| orjson        | nested       | 14.513   | 19.764      | 30.138   | 508.2    |
| ujson         | nested       | 21.789   | 21.803      | 22.074   | 508.2    |
| json (stdlib) | nested       | 22.244   | 26.310      | 31.847   | 508.2    |
| strata        | users.json   | 173.203  | 210.694     | 211.352  | 521.9    |
| orjson        | users.json   | 200.757  | 298.228     | 312.279  | 521.9    |
| msgspec       | users.json   | 228.733  | 300.353     | 317.986  | 555.0    |
| ujson         | users.json   | 308.736  | 415.075     | 415.348  | 677.5    |
| json (stdlib) | users.json   | 418.742  | 512.846     | 522.388  | 555.1    |
| strata        | users.ndjson | 174.709  | 220.657     | 235.231  | 616.6    |
| orjson        | users.ndjson | 266.831  | 320.505     | 342.904  | 616.7    |
| msgspec       | users.ndjson | 279.169  | 332.001     | 343.338  | 655.0    |
| ujson         | users.ndjson | 373.224  | 460.501     | 487.857  | 777.5    |
| json (stdlib) | users.ndjson | 463.053  | 530.615     | 536.259  | 656.6    |
| orjson        | wide_arrays  | 33.571   | 39.537      | 46.465   | 465.7    |
| strata        | wide_arrays  | 31.507   | 40.375      | 41.758   | 464.7    |
| msgspec       | wide_arrays  | 39.856   | 49.822      | 54.076   | 480.7    |
| ujson         | wide_arrays  | 71.079   | 75.010      | 79.230   | 479.7    |
| json (stdlib) | wide_arrays  | 130.032  | 139.154     | 140.457  | 481.7    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.736    | 2.738       | 2.855    | 515.7    |
| msgspec | flat        | 2.789    | 2.962       | 3.221    | 511.4    |
| orjson  | flat        | 3.032    | 3.065       | 3.123    | 511.4    |
| ujson   | flat        | 11.569   | 11.604      | 11.615   | 513.4    |
| json    | flat        | 21.803   | 22.005      | 22.083   | 513.3    |
| strata  | mixed       | 2.932    | 2.965       | 3.122    | 691.5    |
| msgspec | mixed       | 2.739    | 3.019       | 3.256    | 694.2    |
| orjson  | mixed       | 3.048    | 3.053       | 3.098    | 689.6    |
| ujson   | mixed       | 12.335   | 12.399      | 12.871   | 696.1    |
| json    | mixed       | 27.910   | 28.622      | 29.635   | 705.8    |
| orjson  | nested      | 1.884    | 2.084       | 2.383    | 508.2    |
| strata  | nested      | 2.066    | 2.096       | 2.115    | 508.2    |
| msgspec | nested      | 1.911    | 2.153       | 2.478    | 508.2    |
| ujson   | nested      | 9.656    | 9.669       | 10.060   | 508.2    |
| json    | nested      | 12.635   | 12.639      | 12.662   | 508.2    |
| strata  | users.json  | 55.409   | 58.077      | 62.628   | 590.6    |
| msgspec | users.json  | 78.583   | 81.381      | 85.235   | 943.8    |
| orjson  | users.json  | 80.846   | 83.002      | 85.321   | 748.3    |
| ujson   | users.json  | 293.120  | 295.636     | 299.578  | 1138.7   |
| json    | users.json  | 435.816  | 443.978     | 612.786  | 1334.3   |
| msgspec | wide_arrays | 16.679   | 17.299      | 18.929   | 525.2    |
| strata  | wide_arrays | 17.776   | 17.840      | 18.813   | 479.7    |
| orjson  | wide_arrays | 19.195   | 20.746      | 25.290   | 488.4    |
| ujson   | wide_arrays | 70.960   | 71.039      | 72.565   | 571.2    |
| json    | wide_arrays | 198.329  | 201.222     | 201.741  | 623.3    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 3.398    | 3.452       | 4.074    | 513.3    |
| msgspec       | flat        | 3.393    | 3.483       | 4.010    | 513.2    |
| orjson        | flat        | 3.358    | 3.633       | 3.840    | 513.1    |
| ujson         | flat        | 12.604   | 12.837      | 12.954   | 513.1    |
| json (stdlib) | flat        | 78.653   | 78.787      | 78.940   | 513.2    |
| msgspec       | mixed       | 3.362    | 3.630       | 4.021    | 705.7    |
| strata        | mixed       | 3.575    | 3.670       | 4.228    | 705.8    |
| orjson        | mixed       | 3.770    | 3.930       | 4.095    | 705.7    |
| ujson         | mixed       | 13.220   | 13.609      | 14.014   | 705.7    |
| json (stdlib) | mixed       | 93.484   | 93.897      | 94.930   | 705.7    |
| msgspec       | nested      | 2.390    | 2.494       | 2.767    | 508.2    |
| orjson        | nested      | 2.546    | 2.791       | 3.222    | 508.2    |
| strata        | nested      | 2.576    | 2.793       | 2.880    | 508.2    |
| ujson         | nested      | 10.143   | 10.856      | 10.925   | 508.2    |
| json (stdlib) | nested      | 114.578  | 114.891     | 115.201  | 508.2    |
| strata        | users.json  | 83.266   | 83.615      | 90.535   | 824.6    |
| msgspec       | users.json  | 104.283  | 106.681     | 116.183  | 1294.0   |
| orjson        | users.json  | 104.056  | 115.940     | 118.163  | 980.3    |
| ujson         | users.json  | 328.717  | 330.019     | 337.927  | 1138.2   |
| json (stdlib) | users.json  | 3003.589 | 3004.786    | 3022.197 | 987.6    |
| orjson        | wide_arrays | 18.642   | 19.327      | 20.424   | 672.4    |
| strata        | wide_arrays | 20.467   | 20.898      | 22.481   | 623.3    |
| msgspec       | wide_arrays | 26.460   | 27.931      | 29.297   | 680.3    |
| ujson         | wide_arrays | 74.545   | 76.827      | 79.670   | 680.5    |
| json (stdlib) | wide_arrays | 441.971  | 448.399     | 451.405  | 680.3    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 84.968   | 4000    | 767.7    |
|                                | orjson+jmespath    | 213.341  | 4000    | 1120.4   |
|                                | orjson+jsonpath-ng | 216.207  | 4000    | 1128.4   |
| all item prices                | strata             | 88.066   | 789913  | 1155.3   |
|                                | orjson+jmespath    | 559.715  | 4000    | 1216.2   |
|                                | orjson+jsonpath-ng | 2008.658 | 789913  | 1142.3   |
| recursive price                | strata             | 91.919   | 789913  | 1119.2   |
|                                | orjson+jsonpath-ng | 6342.942 | 789913  | 714.8    |
| flat:$.records\[\*\].id        | strata             | 4.475    | 5000    | 513.2    |
|                                | orjson+jmespath    | 10.326   | 5000    | 513.2    |
|                                | orjson+jsonpath-ng | 16.434   | 5000    | 510.2    |
| flat:$.records\[0\]            | strata             | 4.246    | 1       | 510.2    |
|                                | orjson+jsonpath-ng | 8.817    | 1       | 510.2    |
|                                | orjson+jmespath    | 13.652   | 1       | 510.2    |
| nested:$.records\[\*\].id      | strata             | 3.458    | 5000    | 508.2    |
|                                | orjson+jmespath    | 17.656   | 5000    | 508.2    |
|                                | orjson+jsonpath-ng | 22.096   | 5000    | 508.2    |
| nested:$.records\[0\]          | strata             | 3.371    | 1       | 508.2    |
|                                | orjson+jsonpath-ng | 14.537   | 1       | 508.2    |
|                                | orjson+jmespath    | 14.606   | 1       | 508.2    |
| wide_arrays:$.records\[\*\].id | strata             | 26.267   | 5000    | 680.3    |
|                                | orjson+jmespath    | 35.444   | 5000    | 680.3    |
|                                | orjson+jsonpath-ng | 42.224   | 5000    | 680.3    |
| wide_arrays:$.records\[0\]     | strata             | 25.931   | 1       | 680.3    |
|                                | orjson+jsonpath-ng | 34.776   | 1       | 680.3    |
|                                | orjson+jmespath    | 35.960   | 1       | 680.3    |
| mixed:$.records\[\*\].id       | strata             | 4.564    | 5000    | 705.7    |
|                                | orjson+jmespath    | 12.672   | 5000    | 705.7    |
|                                | orjson+jsonpath-ng | 19.994   | 5000    | 705.7    |
| mixed:$.records\[0\]           | strata             | 4.678    | 1       | 705.7    |
|                                | orjson+jsonpath-ng | 10.876   | 1       | 705.7    |
|                                | orjson+jmespath    | 11.356   | 1       | 705.7    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.730    | 4000    | 943.4    |
|                                            | jmespath    | 2.822    | 4000    | 943.4    |
|                                            | jsonpath-ng | 93.110   | 4000    | 945.1    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 44.014   | 789913  | 951.2    |
|                                            | jmespath    | 412.384  | 4000    | 966.1    |
|                                            | jsonpath-ng | 1747.506 | 789913  | 820.4    |
| $..price                                   | strata      | 56.538   | 789913  | 827.6    |
|                                            | jsonpath-ng | 6172.826 | 789913  | 824.6    |
| $.users\[?(@.age>30)\]                     | strata      | 0.855    | 3212    | 828.1    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 62.136   | 26406   | 828.2    |
| flat:$.records\[\*\].id                    | strata      | 0.329    | 5000    | 510.2    |
|                                            | jmespath    | 1.624    | 5000    | 510.2    |
|                                            | jsonpath-ng | 9.451    | 5000    | 510.2    |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 510.2    |
|                                            | jmespath    | 0.009    | 1       | 510.2    |
|                                            | jsonpath-ng | 0.009    | 1       | 510.2    |
| nested:$.records\[\*\].id                  | strata      | 0.430    | 5000    | 508.2    |
|                                            | jmespath    | 1.716    | 5000    | 508.2    |
|                                            | jsonpath-ng | 15.078   | 5000    | 508.2    |
| nested:$.records\[0\]                      | strata      | 0.005    | 1       | 508.2    |
|                                            | jmespath    | 0.010    | 1       | 508.2    |
|                                            | jsonpath-ng | 0.013    | 1       | 508.2    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.289    | 5000    | 680.3    |
|                                            | jmespath    | 1.688    | 5000    | 680.3    |
|                                            | jsonpath-ng | 16.049   | 5000    | 680.3    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 680.3    |
|                                            | jmespath    | 0.010    | 1       | 680.3    |
|                                            | jsonpath-ng | 0.011    | 1       | 680.3    |
| mixed:$.records\[\*\].id                   | strata      | 0.326    | 5000    | 705.7    |
|                                            | jmespath    | 1.683    | 5000    | 705.7    |
|                                            | jsonpath-ng | 12.429   | 5000    | 705.7    |
| mixed:$.records\[0\]                       | strata      | 0.006    | 1       | 705.7    |
|                                            | jmespath    | 0.008    | 1       | 705.7    |
|                                            | jsonpath-ng | 0.011    | 1       | 705.7    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **21.4% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **59.5% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **15.9% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **52.7% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **41.8% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **25.0% faster** than #2 (orjson)   |
| loads (flat)        | **#1** / 5              | **28.9% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **38.8% faster** than #2 (msgspec)  |
| dumps (flat)        | **#1** / 5              | **1.9% faster** than #2 (msgspec)   |
| dump (flat)         | **#3** / 5              | 1.2% behind #1 (orjson)             |
| loads (nested)      | **#1** / 5              | **131.4% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **138.0% faster** than #2 (orjson)  |
| dumps (nested)      | **#3** / 5              | 9.7% behind #1 (orjson)             |
| dump (nested)       | **#3** / 5              | 7.8% behind #1 (msgspec)            |
| loads (wide_arrays) | **#2** / 5              | 10.8% behind #1 (orjson)            |
| load (wide_arrays)  | **#1** / 5              | **6.5% faster** than #2 (orjson)    |
| dumps (wide_arrays) | **#2** / 5              | 6.6% behind #1 (msgspec)            |
| dump (wide_arrays)  | **#2** / 5              | 9.8% behind #1 (orjson)             |
| loads (mixed)       | **#1** / 5              | **53.5% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **66.2% faster** than #2 (msgspec)  |
| dumps (mixed)       | **#2** / 5              | 7.1% behind #1 (msgspec)            |
| dump (mixed)        | **#2** / 5              | 6.3% behind #1 (msgspec)            |
| search (JSONPath)   | **#1** in 11/11 queries | avg **804% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
