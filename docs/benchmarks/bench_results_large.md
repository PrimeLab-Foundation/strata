# Strata Benchmark Results

Generated: 2026-03-19 02:15:01

## Environment

- Commit: dfd6aa590c1dd14d42395de2cde1a4a3e49aa958
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
| strata        | flat         | 7.219    | 8.211       | 8.648    | 451.4    |
| msgspec       | flat         | 8.709    | 9.934       | 10.123   | 451.4    |
| orjson        | flat         | 8.779    | 10.521      | 11.000   | 451.4    |
| ujson         | flat         | 13.430   | 16.237      | 16.240   | 451.4    |
| json (stdlib) | flat         | 23.128   | 24.971      | 25.231   | 454.4    |
| strata        | mixed        | 6.784    | 8.296       | 8.326    | 701.3    |
| orjson        | mixed        | 9.050    | 11.785      | 12.351   | 706.9    |
| msgspec       | mixed        | 11.214   | 15.314      | 15.525   | 706.9    |
| ujson         | mixed        | 15.366   | 23.681      | 26.596   | 706.9    |
| json (stdlib) | mixed        | 24.618   | 27.393      | 27.410   | 709.3    |
| strata        | nested       | 6.883    | 8.019       | 8.114    | 454.5    |
| orjson        | nested       | 14.230   | 17.301      | 17.463   | 454.5    |
| ujson         | nested       | 15.750   | 21.165      | 23.018   | 454.5    |
| msgspec       | nested       | 16.008   | 23.777      | 31.337   | 454.5    |
| json (stdlib) | nested       | 22.726   | 27.131      | 27.527   | 454.5    |
| strata        | users.json   | 191.063  | 225.670     | 228.898  | 340.2    |
| orjson        | users.json   | 205.962  | 284.955     | 289.847  | 478.2    |
| msgspec       | users.json   | 218.804  | 324.833     | 335.944  | 467.7    |
| ujson         | users.json   | 299.908  | 400.242     | 418.743  | 590.1    |
| json (stdlib) | users.json   | 414.122  | 494.660     | 538.551  | 511.6    |
| strata        | users.ndjson | 189.031  | 229.675     | 230.842  | 508.7    |
| orjson        | users.ndjson | 273.938  | 335.511     | 337.621  | 559.1    |
| msgspec       | users.ndjson | 258.157  | 337.312     | 349.368  | 553.7    |
| ujson         | users.ndjson | 372.791  | 428.987     | 452.653  | 676.2    |
| json (stdlib) | users.ndjson | 472.578  | 501.725     | 517.340  | 556.4    |
| strata        | wide_arrays  | 29.420   | 35.831      | 39.232   | 373.4    |
| orjson        | wide_arrays  | 28.446   | 41.873      | 53.055   | 399.0    |
| msgspec       | wide_arrays  | 36.774   | 45.650      | 50.287   | 401.0    |
| ujson         | wide_arrays  | 59.964   | 68.987      | 71.807   | 400.0    |
| json (stdlib) | wide_arrays  | 125.332  | 134.752     | 135.297  | 415.0    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 8.765    | 8.895       | 9.269    | 457.3    |
| msgspec       | flat         | 9.048    | 10.314      | 10.607   | 460.2    |
| orjson        | flat         | 9.126    | 10.809      | 10.919   | 457.3    |
| ujson         | flat         | 15.600   | 17.045      | 19.588   | 460.2    |
| json (stdlib) | flat         | 23.847   | 25.165      | 25.318   | 460.2    |
| strata        | mixed        | 7.358    | 8.758       | 8.774    | 709.3    |
| orjson        | mixed        | 9.910    | 12.792      | 12.926   | 709.3    |
| msgspec       | mixed        | 10.301   | 12.847      | 14.231   | 711.7    |
| ujson         | mixed        | 14.981   | 18.133      | 18.474   | 711.7    |
| json (stdlib) | mixed        | 29.019   | 29.396      | 31.874   | 711.7    |
| strata        | nested       | 6.959    | 8.303       | 8.535    | 454.5    |
| msgspec       | nested       | 14.892   | 18.865      | 38.951   | 456.3    |
| ujson         | nested       | 16.644   | 22.046      | 22.774   | 456.3    |
| orjson        | nested       | 23.588   | 23.868      | 45.071   | 454.5    |
| json (stdlib) | nested       | 26.169   | 27.675      | 31.060   | 456.3    |
| strata        | users.json   | 189.256  | 230.470     | 234.293  | 522.4    |
| msgspec       | users.json   | 241.519  | 277.170     | 294.569  | 555.5    |
| orjson        | users.json   | 229.524  | 321.906     | 327.882  | 522.4    |
| ujson         | users.json   | 309.949  | 406.590     | 446.670  | 678.0    |
| json (stdlib) | users.json   | 395.663  | 477.508     | 478.543  | 555.5    |
| strata        | users.ndjson | 194.969  | 239.502     | 243.660  | 559.2    |
| msgspec       | users.ndjson | 279.870  | 315.142     | 347.353  | 597.6    |
| orjson        | users.ndjson | 250.526  | 315.806     | 339.942  | 559.3    |
| ujson         | users.ndjson | 389.045  | 432.241     | 471.706  | 720.1    |
| json (stdlib) | users.ndjson | 449.549  | 493.114     | 553.913  | 599.3    |
| strata        | wide_arrays  | 34.640   | 37.722      | 38.941   | 427.1    |
| orjson        | wide_arrays  | 29.953   | 39.904      | 40.867   | 428.1    |
| msgspec       | wide_arrays  | 47.844   | 49.301      | 49.659   | 443.1    |
| ujson         | wide_arrays  | 64.503   | 73.095      | 74.230   | 442.1    |
| json (stdlib) | wide_arrays  | 136.979  | 137.717     | 137.727  | 444.1    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.345    | 2.352       | 2.526    | 462.0    |
| msgspec | flat        | 2.947    | 2.951       | 3.016    | 457.7    |
| orjson  | flat        | 2.978    | 3.037       | 3.099    | 457.7    |
| ujson   | flat        | 12.061   | 12.129      | 12.204   | 459.7    |
| json    | flat        | 22.697   | 22.778      | 22.953   | 459.7    |
| strata  | mixed       | 2.671    | 2.676       | 2.773    | 711.7    |
| msgspec | mixed       | 2.808    | 2.820       | 3.053    | 714.3    |
| orjson  | mixed       | 3.053    | 3.168       | 3.235    | 709.7    |
| ujson   | mixed       | 12.457   | 12.485      | 12.568   | 716.5    |
| json    | mixed       | 27.679   | 28.381      | 28.497   | 724.0    |
| orjson  | nested      | 1.943    | 1.956       | 1.979    | 456.3    |
| strata  | nested      | 2.003    | 2.013       | 2.026    | 456.3    |
| msgspec | nested      | 2.335    | 2.497       | 2.704    | 456.4    |
| ujson   | nested      | 10.007   | 10.021      | 10.048   | 458.0    |
| json    | nested      | 13.346   | 13.418      | 13.513   | 458.2    |
| strata  | users.json  | 59.426   | 59.476      | 60.416   | 591.1    |
| msgspec | users.json  | 78.427   | 78.755      | 80.495   | 942.9    |
| orjson  | users.json  | 79.296   | 79.582      | 80.240   | 747.4    |
| ujson   | users.json  | 293.154  | 293.496     | 294.366  | 1137.8   |
| json    | users.json  | 424.208  | 424.229     | 425.628  | 1354.4   |
| strata  | wide_arrays | 15.115   | 16.532      | 16.760   | 449.2    |
| msgspec | wide_arrays | 16.855   | 17.100      | 17.143   | 482.4    |
| orjson  | wide_arrays | 17.528   | 17.549      | 18.475   | 457.8    |
| ujson   | wide_arrays | 70.992   | 72.637      | 74.146   | 517.5    |
| json    | wide_arrays | 199.206  | 200.993     | 201.786  | 569.6    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 2.738    | 2.775       | 2.845    | 459.7    |
| msgspec       | flat        | 3.518    | 3.564       | 3.652    | 459.5    |
| orjson        | flat        | 3.559    | 3.688       | 3.881    | 459.5    |
| ujson         | flat        | 12.819   | 12.870      | 12.939   | 459.5    |
| json (stdlib) | flat        | 79.462   | 81.795      | 82.240   | 459.5    |
| strata        | mixed       | 3.247    | 3.271       | 3.408    | 724.0    |
| msgspec       | mixed       | 3.349    | 3.398       | 3.514    | 728.2    |
| orjson        | mixed       | 3.726    | 3.735       | 3.838    | 725.9    |
| ujson         | mixed       | 13.295   | 13.354      | 13.735   | 728.2    |
| json (stdlib) | mixed       | 93.739   | 95.198      | 95.538   | 728.2    |
| msgspec       | nested      | 2.381    | 2.402       | 2.554    | 459.5    |
| strata        | nested      | 2.367    | 2.546       | 3.045    | 458.2    |
| orjson        | nested      | 2.717    | 3.016       | 3.302    | 458.2    |
| ujson         | nested      | 10.559   | 10.725      | 10.773   | 459.5    |
| json (stdlib) | nested      | 115.249  | 118.750     | 119.412  | 459.5    |
| strata        | users.json  | 81.053   | 84.379      | 85.051   | 1398.2   |
| orjson        | users.json  | 100.857  | 101.103     | 103.631  | 1554.0   |
| msgspec       | users.json  | 97.478   | 104.242     | 109.228  | 1865.7   |
| ujson         | users.json  | 313.600  | 318.465     | 319.761  | 1709.9   |
| json (stdlib) | users.json  | 2933.464 | 2965.367    | 2969.905 | 575.6    |
| strata        | wide_arrays | 17.339   | 17.403      | 17.562   | 569.6    |
| msgspec       | wide_arrays | 20.354   | 20.356      | 20.489   | 696.4    |
| orjson        | wide_arrays | 19.584   | 20.921      | 22.322   | 598.2    |
| ujson         | wide_arrays | 75.486   | 75.931      | 77.234   | 647.3    |
| json (stdlib) | wide_arrays | 444.094  | 445.939     | 449.937  | 696.4    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 83.961   | 4000    | 419.5    |
|                                | orjson+jmespath    | 197.760  | 4000    | 641.6    |
|                                | orjson+jsonpath-ng | 200.798  | 4000    | 648.6    |
| all item prices                | strata             | 96.569   | 789913  | 677.8    |
|                                | orjson+jmespath    | 543.269  | 4000    | 738.7    |
|                                | orjson+jsonpath-ng | 1932.278 | 789913  | 731.9    |
| recursive price                | strata             | 94.356   | 789913  | 705.1    |
|                                | orjson+jsonpath-ng | 6380.065 | 789913  | 656.7    |
| flat:$.records\[\*\].id        | strata             | 4.800    | 5000    | 459.5    |
|                                | orjson+jmespath    | 10.769   | 5000    | 459.5    |
|                                | orjson+jsonpath-ng | 16.907   | 5000    | 456.5    |
| flat:$.records\[0\]            | strata             | 4.762    | 1       | 456.5    |
|                                | orjson+jsonpath-ng | 9.377    | 1       | 456.5    |
|                                | orjson+jmespath    | 9.885    | 1       | 456.5    |
| nested:$.records\[\*\].id      | strata             | 3.649    | 5000    | 459.5    |
|                                | orjson+jmespath    | 18.289   | 5000    | 459.5    |
|                                | orjson+jsonpath-ng | 21.742   | 5000    | 459.5    |
| nested:$.records\[0\]          | strata             | 3.512    | 1       | 459.5    |
|                                | orjson+jmespath    | 14.638   | 1       | 459.5    |
|                                | orjson+jsonpath-ng | 16.998   | 1       | 459.5    |
| wide_arrays:$.records\[\*\].id | strata             | 23.215   | 5000    | 696.4    |
|                                | orjson+jmespath    | 38.726   | 5000    | 696.4    |
|                                | orjson+jsonpath-ng | 43.999   | 5000    | 696.4    |
| wide_arrays:$.records\[0\]     | strata             | 23.227   | 1       | 696.4    |
|                                | orjson+jsonpath-ng | 35.403   | 1       | 696.5    |
|                                | orjson+jmespath    | 36.691   | 1       | 696.5    |
| mixed:$.records\[\*\].id       | strata             | 4.390    | 5000    | 728.2    |
|                                | orjson+jmespath    | 12.289   | 5000    | 728.2    |
|                                | orjson+jsonpath-ng | 19.626   | 5000    | 728.2    |
| mixed:$.records\[0\]           | strata             | 4.271    | 1       | 728.2    |
|                                | orjson+jsonpath-ng | 10.445   | 1       | 728.2    |
|                                | orjson+jmespath    | 10.511   | 1       | 728.2    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.724    | 4000    | 885.2    |
|                                            | jmespath    | 2.325    | 4000    | 885.2    |
|                                            | jsonpath-ng | 79.071   | 4000    | 886.9    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 44.025   | 789913  | 749.1    |
|                                            | jmespath    | 395.433  | 4000    | 763.9    |
|                                            | jsonpath-ng | 1714.137 | 789913  | 762.2    |
| $..price                                   | strata      | 58.334   | 789913  | 769.5    |
|                                            | jsonpath-ng | 6112.217 | 789913  | 768.3    |
| $.users\[?(@.age>30)\]                     | strata      | 0.846    | 3212    | 771.8    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 61.365   | 26406   | 771.9    |
| flat:$.records\[\*\].id                    | strata      | 0.323    | 5000    | 456.5    |
|                                            | jmespath    | 1.701    | 5000    | 456.5    |
|                                            | jsonpath-ng | 9.357    | 5000    | 456.5    |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 456.5    |
|                                            | jmespath    | 0.008    | 1       | 456.5    |
|                                            | jsonpath-ng | 0.009    | 1       | 456.5    |
| nested:$.records\[\*\].id                  | strata      | 0.481    | 5000    | 459.5    |
|                                            | jmespath    | 1.777    | 5000    | 459.5    |
|                                            | jsonpath-ng | 15.187   | 5000    | 459.5    |
| nested:$.records\[0\]                      | strata      | 0.008    | 1       | 459.5    |
|                                            | jsonpath-ng | 0.010    | 1       | 459.5    |
|                                            | jmespath    | 0.010    | 1       | 459.5    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.257    | 5000    | 696.5    |
|                                            | jmespath    | 1.986    | 5000    | 696.5    |
|                                            | jsonpath-ng | 16.136   | 5000    | 696.5    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 696.5    |
|                                            | jmespath    | 0.009    | 1       | 696.5    |
|                                            | jsonpath-ng | 0.011    | 1       | 696.5    |
| mixed:$.records\[\*\].id                   | strata      | 0.581    | 5000    | 728.2    |
|                                            | jmespath    | 2.077    | 5000    | 728.2    |
|                                            | jsonpath-ng | 13.149   | 5000    | 728.2    |
| mixed:$.records\[0\]                       | strata      | 0.009    | 1       | 728.2    |
|                                            | jsonpath-ng | 0.010    | 1       | 728.2    |
|                                            | jmespath    | 0.018    | 1       | 728.2    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **7.8% faster** than #2 (orjson)    |
| loads (NDJSON)      | **#1** / 5              | **36.6% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **21.3% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **28.5% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **32.0% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **20.3% faster** than #2 (msgspec)  |
| loads (flat)        | **#1** / 5              | **20.6% faster** than #2 (msgspec)  |
| load (flat)         | **#1** / 5              | **3.2% faster** than #2 (msgspec)   |
| dumps (flat)        | **#1** / 5              | **25.7% faster** than #2 (msgspec)  |
| dump (flat)         | **#1** / 5              | **28.5% faster** than #2 (msgspec)  |
| loads (nested)      | **#1** / 5              | **106.7% faster** than #2 (orjson)  |
| load (nested)       | **#1** / 5              | **114.0% faster** than #2 (msgspec) |
| dumps (nested)      | **#2** / 5              | 3.1% behind #1 (orjson)             |
| dump (nested)       | **#1** / 5              | **0.6% faster** than #2 (msgspec)   |
| loads (wide_arrays) | **#2** / 5              | 3.4% behind #1 (orjson)             |
| load (wide_arrays)  | **#2** / 5              | 15.6% behind #1 (orjson)            |
| dumps (wide_arrays) | **#1** / 5              | **11.5% faster** than #2 (msgspec)  |
| dump (wide_arrays)  | **#1** / 5              | **12.9% faster** than #2 (orjson)   |
| loads (mixed)       | **#1** / 5              | **33.4% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **34.7% faster** than #2 (orjson)   |
| dumps (mixed)       | **#1** / 5              | **5.1% faster** than #2 (msgspec)   |
| dump (mixed)        | **#1** / 5              | **3.1% faster** than #2 (msgspec)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **786% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
