# Strata Benchmark Results

Generated: 2026-03-17 20:47:49

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
| strata        | flat         | 9.151    | 10.143      | 10.237   | 504.5    |
| msgspec       | flat         | 8.907    | 10.775      | 11.114   | 504.6    |
| orjson        | flat         | 8.910    | 11.041      | 12.118   | 504.5    |
| ujson         | flat         | 14.123   | 16.352      | 17.189   | 504.6    |
| json (stdlib) | flat         | 23.594   | 25.084      | 28.349   | 507.5    |
| strata        | mixed        | 9.685    | 11.347      | 11.491   | 782.5    |
| msgspec       | mixed        | 12.924   | 13.173      | 13.358   | 788.1    |
| orjson        | mixed        | 10.249   | 13.334      | 15.697   | 788.1    |
| ujson         | mixed        | 14.404   | 17.742      | 26.937   | 788.1    |
| json (stdlib) | mixed        | 24.778   | 30.922      | 32.312   | 790.5    |
| strata        | nested       | 8.787    | 10.131      | 10.159   | 501.9    |
| orjson        | nested       | 16.596   | 19.676      | 34.994   | 501.9    |
| msgspec       | nested       | 18.410   | 19.920      | 34.659   | 501.9    |
| json (stdlib) | nested       | 22.228   | 26.764      | 38.334   | 501.9    |
| ujson         | nested       | 20.661   | 35.232      | 41.508   | 501.9    |
| orjson        | users.json   | 193.299  | 294.236     | 306.232  | 478.7    |
| strata        | users.json   | 275.344  | 295.036     | 322.027  | 340.7    |
| msgspec       | users.json   | 321.125  | 327.925     | 335.149  | 468.3    |
| ujson         | users.json   | 305.315  | 439.008     | 494.343  | 590.6    |
| json (stdlib) | users.json   | 517.384  | 536.738     | 569.596  | 512.1    |
| strata        | users.ndjson | 264.935  | 320.094     | 326.774  | 563.1    |
| orjson        | users.ndjson | 267.355  | 330.575     | 332.675  | 613.3    |
| msgspec       | users.ndjson | 277.525  | 337.047     | 353.258  | 607.8    |
| ujson         | users.ndjson | 378.510  | 470.067     | 474.825  | 730.3    |
| json (stdlib) | users.ndjson | 494.308  | 537.292     | 550.637  | 610.5    |
| orjson        | wide_arrays  | 29.843   | 40.989      | 46.090   | 451.4    |
| msgspec       | wide_arrays  | 46.407   | 49.649      | 54.195   | 452.5    |
| strata        | wide_arrays  | 50.367   | 58.475      | 59.504   | 424.7    |
| ujson         | wide_arrays  | 70.354   | 72.545      | 74.066   | 451.5    |
| json (stdlib) | wide_arrays  | 127.755  | 138.058     | 145.962  | 466.5    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 8.847    | 10.282      | 14.851   | 507.5    |
| msgspec       | flat         | 9.757    | 10.996      | 11.178   | 510.4    |
| strata        | flat         | 10.673   | 11.158      | 11.528   | 507.5    |
| ujson         | flat         | 14.507   | 17.018      | 17.120   | 510.4    |
| json (stdlib) | flat         | 24.458   | 25.633      | 29.461   | 510.4    |
| strata        | mixed        | 10.390   | 11.739      | 12.025   | 790.5    |
| msgspec       | mixed        | 10.963   | 13.707      | 22.855   | 792.9    |
| orjson        | mixed        | 10.374   | 16.044      | 21.670   | 790.5    |
| ujson         | mixed        | 15.181   | 18.666      | 18.995   | 792.9    |
| json (stdlib) | mixed        | 25.618   | 28.240      | 28.672   | 792.9    |
| strata        | nested       | 8.845    | 10.355      | 10.815   | 501.9    |
| orjson        | nested       | 13.755   | 19.208      | 37.906   | 501.9    |
| msgspec       | nested       | 15.391   | 19.328      | 37.129   | 503.7    |
| ujson         | nested       | 16.630   | 22.081      | 27.001   | 503.7    |
| json (stdlib) | nested       | 22.753   | 27.545      | 40.782   | 503.7    |
| orjson        | users.json   | 222.188  | 280.084     | 329.625  | 523.0    |
| strata        | users.json   | 319.184  | 329.613     | 360.173  | 523.0    |
| msgspec       | users.json   | 227.151  | 342.196     | 346.276  | 556.1    |
| ujson         | users.json   | 316.626  | 456.835     | 488.722  | 678.6    |
| json (stdlib) | users.json   | 451.882  | 556.170     | 575.684  | 556.1    |
| strata        | users.ndjson | 275.559  | 312.524     | 326.139  | 613.4    |
| orjson        | users.ndjson | 283.797  | 329.403     | 331.326  | 613.5    |
| msgspec       | users.ndjson | 302.874  | 332.607     | 413.382  | 651.8    |
| ujson         | users.ndjson | 401.502  | 500.288     | 515.452  | 774.2    |
| json (stdlib) | users.ndjson | 524.566  | 527.527     | 564.742  | 653.4    |
| orjson        | wide_arrays  | 30.913   | 51.179      | 51.544   | 469.5    |
| msgspec       | wide_arrays  | 45.901   | 52.920      | 60.188   | 484.5    |
| strata        | wide_arrays  | 49.909   | 57.826      | 59.580   | 467.5    |
| ujson         | wide_arrays  | 70.330   | 76.368      | 77.985   | 483.5    |
| json (stdlib) | wide_arrays  | 130.776  | 143.098     | 146.774  | 485.5    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| msgspec | flat        | 3.050    | 3.053       | 3.244    | 506.1    |
| strata  | flat        | 2.878    | 3.062       | 3.166    | 510.4    |
| orjson  | flat        | 3.004    | 3.066       | 3.239    | 506.1    |
| ujson   | flat        | 12.591   | 12.913      | 13.059   | 508.1    |
| json    | flat        | 23.050   | 23.110      | 24.288   | 508.1    |
| msgspec | mixed       | 2.880    | 2.918       | 3.134    | 795.3    |
| orjson  | mixed       | 3.326    | 3.336       | 3.652    | 791.1    |
| strata  | mixed       | 3.684    | 3.691       | 3.780    | 792.9    |
| ujson   | mixed       | 12.887   | 12.891      | 13.001   | 795.3    |
| json    | mixed       | 29.090   | 31.447      | 36.296   | 802.7    |
| orjson  | nested      | 1.947    | 1.955       | 1.974    | 503.7    |
| msgspec | nested      | 1.962    | 1.998       | 2.002    | 504.0    |
| strata  | nested      | 2.753    | 2.774       | 3.261    | 503.7    |
| ujson   | nested      | 9.907    | 10.031      | 10.040   | 504.0    |
| json    | nested      | 12.906   | 12.949      | 13.271   | 504.0    |
| strata  | users.json  | 65.923   | 66.724      | 71.554   | 591.6    |
| orjson  | users.json  | 103.672  | 108.680     | 172.128  | 748.8    |
| msgspec | users.json  | 97.914   | 109.614     | 141.682  | 946.7    |
| ujson   | users.json  | 327.027  | 330.011     | 337.464  | 1141.8   |
| json    | users.json  | 443.847  | 486.943     | 627.115  | 1356.5   |
| orjson  | wide_arrays | 16.341   | 16.349      | 16.530   | 533.5    |
| msgspec | wide_arrays | 17.389   | 17.584      | 18.325   | 582.6    |
| strata  | wide_arrays | 20.506   | 20.562      | 20.838   | 484.5    |
| ujson   | wide_arrays | 74.026   | 74.070      | 74.580   | 631.6    |
| json    | wide_arrays | 201.308  | 205.251     | 209.003  | 683.8    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 3.418    | 3.426       | 3.634    | 508.1    |
| msgspec       | flat        | 3.700    | 3.719       | 3.758    | 507.9    |
| orjson        | flat        | 3.300    | 4.701       | 5.465    | 507.9    |
| ujson         | flat        | 13.843   | 13.902      | 14.324   | 507.9    |
| json (stdlib) | flat        | 82.749   | 83.217      | 83.386   | 507.9    |
| orjson        | mixed       | 3.596    | 3.870       | 4.060    | 802.6    |
| msgspec       | mixed       | 3.358    | 3.981       | 4.352    | 802.6    |
| strata        | mixed       | 4.089    | 4.562       | 4.634    | 802.7    |
| ujson         | mixed       | 13.365   | 13.715      | 14.058   | 802.6    |
| json (stdlib) | mixed       | 97.140   | 99.684      | 100.711  | 802.6    |
| orjson        | nested      | 2.426    | 2.449       | 2.621    | 504.0    |
| msgspec       | nested      | 2.415    | 2.732       | 3.158    | 504.0    |
| strata        | nested      | 3.109    | 3.129       | 4.023    | 504.0    |
| ujson         | nested      | 10.321   | 10.550      | 10.834   | 504.0    |
| json (stdlib) | nested      | 118.239  | 118.449     | 118.725  | 504.0    |
| strata        | users.json  | 62.657   | 66.448      | 81.317   | 1400.4   |
| msgspec       | users.json  | 92.342   | 97.662      | 117.070  | 1780.5   |
| orjson        | users.json  | 94.907   | 103.737     | 117.976  | 1556.1   |
| ujson         | users.json  | 307.203  | 308.156     | 311.975  | 1634.2   |
| json (stdlib) | users.json  | 3059.004 | 3146.088    | 3230.635 | 1486.9   |
| msgspec       | wide_arrays | 20.549   | 21.424      | 21.549   | 777.7    |
| orjson        | wide_arrays | 22.017   | 22.313      | 24.317   | 683.8    |
| strata        | wide_arrays | 22.584   | 24.351      | 27.913   | 683.8    |
| ujson         | wide_arrays | 76.812   | 77.645      | 80.221   | 728.6    |
| json (stdlib) | wide_arrays | 453.210  | 457.195     | 462.765  | 777.7    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 122.944  | 4000    | 1267.0   |
|                                | orjson+jsonpath-ng | 220.068  | 4000    | 1627.2   |
|                                | orjson+jmespath    | 222.493  | 4000    | 1619.3   |
| all item prices                | strata             | 161.411  | 789913  | 1654.2   |
|                                | orjson+jmespath    | 548.060  | 4000    | 1715.1   |
|                                | orjson+jsonpath-ng | 1976.872 | 789913  | 1136.9   |
| recursive price                | strata             | 173.172  | 789913  | 555.4    |
|                                | orjson+jsonpath-ng | 6343.562 | 789913  | 717.1    |
| flat:$.records\[\*\].id        | strata             | 6.839    | 5000    | 507.9    |
|                                | orjson+jmespath    | 11.304   | 5000    | 507.9    |
|                                | orjson+jsonpath-ng | 17.205   | 5000    | 504.9    |
| flat:$.records\[0\]            | strata             | 6.351    | 1       | 504.9    |
|                                | orjson+jmespath    | 9.976    | 1       | 504.9    |
|                                | orjson+jsonpath-ng | 10.255   | 1       | 504.9    |
| nested:$.records\[\*\].id      | strata             | 5.112    | 5000    | 504.0    |
|                                | orjson+jmespath    | 16.434   | 5000    | 504.0    |
|                                | orjson+jsonpath-ng | 22.405   | 5000    | 503.0    |
| nested:$.records\[0\]          | strata             | 4.914    | 1       | 503.0    |
|                                | orjson+jsonpath-ng | 14.499   | 1       | 503.0    |
|                                | orjson+jmespath    | 14.592   | 1       | 503.0    |
| wide_arrays:$.records\[\*\].id | orjson+jmespath    | 38.140   | 5000    | 777.7    |
|                                | strata             | 39.067   | 5000    | 777.7    |
|                                | orjson+jsonpath-ng | 46.604   | 5000    | 777.7    |
| wide_arrays:$.records\[0\]     | orjson+jsonpath-ng | 35.854   | 1       | 777.7    |
|                                | orjson+jmespath    | 37.860   | 1       | 777.7    |
|                                | strata             | 39.052   | 1       | 777.7    |
| mixed:$.records\[\*\].id       | strata             | 6.983    | 5000    | 802.6    |
|                                | orjson+jmespath    | 13.196   | 5000    | 802.6    |
|                                | orjson+jsonpath-ng | 18.459   | 5000    | 802.6    |
| mixed:$.records\[0\]           | strata             | 6.616    | 1       | 802.6    |
|                                | orjson+jsonpath-ng | 10.559   | 1       | 802.6    |
|                                | orjson+jmespath    | 11.039   | 1       | 802.6    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.796    | 4000    | 945.6    |
|                                            | jmespath    | 2.203    | 4000    | 945.6    |
|                                            | jsonpath-ng | 87.022   | 4000    | 947.3    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 49.898   | 789913  | 947.4    |
|                                            | jmespath    | 404.740  | 4000    | 962.3    |
|                                            | jsonpath-ng | 1732.501 | 789913  | 816.5    |
| $..price                                   | strata      | 65.639   | 789913  | 823.8    |
|                                            | jsonpath-ng | 6257.231 | 789913  | 822.6    |
| $.users\[?(@.age>30)\]                     | strata      | 1.218    | 3212    | 826.2    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 67.706   | 26406   | 826.2    |
| flat:$.records\[\*\].id                    | strata      | 0.381    | 5000    | 504.9    |
|                                            | jmespath    | 1.770    | 5000    | 504.9    |
|                                            | jsonpath-ng | 10.201   | 5000    | 504.9    |
| flat:$.records\[0\]                        | strata      | 0.006    | 1       | 504.9    |
|                                            | jmespath    | 0.013    | 1       | 504.9    |
|                                            | jsonpath-ng | 0.016    | 1       | 504.9    |
| nested:$.records\[\*\].id                  | strata      | 0.418    | 5000    | 503.0    |
|                                            | jmespath    | 1.795    | 5000    | 503.0    |
|                                            | jsonpath-ng | 15.747   | 5000    | 503.0    |
| nested:$.records\[0\]                      | strata      | 0.005    | 1       | 503.0    |
|                                            | jmespath    | 0.008    | 1       | 503.0    |
|                                            | jsonpath-ng | 0.010    | 1       | 503.0    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.560    | 5000    | 777.7    |
|                                            | jmespath    | 1.913    | 5000    | 777.7    |
|                                            | jsonpath-ng | 17.831   | 5000    | 777.7    |
| wide_arrays:$.records\[0\]                 | strata      | 0.011    | 1       | 777.7    |
|                                            | jmespath    | 0.015    | 1       | 777.7    |
|                                            | jsonpath-ng | 0.018    | 1       | 777.7    |
| mixed:$.records\[\*\].id                   | strata      | 0.396    | 5000    | 802.6    |
|                                            | jmespath    | 1.782    | 5000    | 802.6    |
|                                            | jsonpath-ng | 12.382   | 5000    | 802.6    |
| mixed:$.records\[0\]                       | jmespath    | 0.008    | 1       | 802.6    |
|                                            | strata      | 0.009    | 1       | 802.6    |
|                                            | jsonpath-ng | 0.019    | 1       | 802.6    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#2** / 5              | 42.4% behind #1 (orjson)           |
| loads (NDJSON)      | **#1** / 5              | **0.9% faster** than #2 (orjson)   |
| load (JSON file)    | **#4** / 5              | 43.7% behind #1 (orjson)           |
| load (NDJSON file)  | **#1** / 5              | **3.0% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **48.5% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **47.4% faster** than #2 (msgspec) |
| loads (flat)        | **#3** / 5              | 2.7% behind #1 (msgspec)           |
| load (flat)         | **#3** / 5              | 20.6% behind #1 (orjson)           |
| dumps (flat)        | **#1** / 5              | **4.4% faster** than #2 (orjson)   |
| dump (flat)         | **#2** / 5              | 3.6% behind #1 (orjson)            |
| loads (nested)      | **#1** / 5              | **88.9% faster** than #2 (orjson)  |
| load (nested)       | **#1** / 5              | **55.5% faster** than #2 (orjson)  |
| dumps (nested)      | **#3** / 5              | 41.4% behind #1 (orjson)           |
| dump (nested)       | **#3** / 5              | 28.7% behind #1 (msgspec)          |
| loads (wide_arrays) | **#3** / 5              | 68.8% behind #1 (orjson)           |
| load (wide_arrays)  | **#3** / 5              | 61.5% behind #1 (orjson)           |
| dumps (wide_arrays) | **#3** / 5              | 25.5% behind #1 (orjson)           |
| dump (wide_arrays)  | **#3** / 5              | 9.9% behind #1 (msgspec)           |
| loads (mixed)       | **#1** / 5              | **5.8% faster** than #2 (orjson)   |
| load (mixed)        | **#2** / 5              | 0.2% behind #1 (orjson)            |
| dumps (mixed)       | **#3** / 5              | 27.9% behind #1 (msgspec)          |
| dump (mixed)        | **#3** / 5              | 21.8% behind #1 (msgspec)          |
| search (JSONPath)   | #1 in 9/11 queries      | -                                  |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
