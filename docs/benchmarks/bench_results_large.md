# Strata Benchmark Results

Generated: 2026-03-17 15:17:42

## Environment

- Commit: 75205226403b1b3dd7be908ebcd6e9ece844e9a9
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
| strata        | flat         | 6.014    | 6.992       | 7.244    | 501.8    |
| msgspec       | flat         | 8.221    | 10.011      | 14.060   | 501.8    |
| orjson        | flat         | 8.530    | 10.138      | 14.276   | 501.8    |
| ujson         | flat         | 14.037   | 15.830      | 15.967   | 501.8    |
| json (stdlib) | flat         | 23.719   | 24.420      | 26.351   | 504.8    |
| strata        | mixed        | 6.636    | 8.021       | 8.203    | 785.3    |
| msgspec       | mixed        | 12.424   | 12.487      | 13.095   | 790.9    |
| orjson        | mixed        | 12.214   | 14.265      | 19.021   | 790.9    |
| ujson         | mixed        | 13.703   | 17.013      | 17.821   | 790.9    |
| json (stdlib) | mixed        | 25.019   | 27.039      | 28.945   | 793.3    |
| strata        | nested       | 5.046    | 6.376       | 7.004    | 500.2    |
| orjson        | nested       | 13.682   | 18.186      | 18.998   | 500.2    |
| msgspec       | nested       | 17.119   | 18.201      | 18.765   | 500.2    |
| ujson         | nested       | 17.266   | 21.678      | 23.981   | 500.2    |
| json (stdlib) | nested       | 26.799   | 27.491      | 38.062   | 500.2    |
| strata        | users.json   | 161.388  | 199.551     | 205.117  | 339.9    |
| orjson        | users.json   | 205.801  | 278.681     | 284.898  | 477.9    |
| msgspec       | users.json   | 210.887  | 286.945     | 311.702  | 467.4    |
| ujson         | users.json   | 292.901  | 409.092     | 419.220  | 589.8    |
| json (stdlib) | users.json   | 412.494  | 492.018     | 512.840  | 511.3    |
| strata        | users.ndjson | 173.689  | 211.471     | 223.287  | 558.4    |
| msgspec       | users.ndjson | 280.615  | 327.550     | 343.492  | 603.1    |
| orjson        | users.ndjson | 266.144  | 336.237     | 338.121  | 608.5    |
| ujson         | users.ndjson | 356.180  | 474.687     | 485.217  | 725.6    |
| json (stdlib) | users.ndjson | 477.777  | 540.427     | 546.772  | 605.8    |
| orjson        | wide_arrays  | 32.479   | 39.235      | 51.718   | 447.8    |
| strata        | wide_arrays  | 33.470   | 39.646      | 43.379   | 422.2    |
| msgspec       | wide_arrays  | 36.213   | 47.375      | 55.088   | 458.5    |
| ujson         | wide_arrays  | 69.474   | 71.601      | 76.589   | 457.5    |
| json (stdlib) | wide_arrays  | 126.376  | 145.657     | 152.572  | 472.5    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 6.764    | 7.607       | 7.678    | 504.8    |
| orjson        | flat         | 9.138    | 10.264      | 10.360   | 504.8    |
| msgspec       | flat         | 9.481    | 10.720      | 11.104   | 507.7    |
| ujson         | flat         | 15.260   | 17.129      | 17.466   | 507.7    |
| json (stdlib) | flat         | 25.953   | 26.221      | 26.462   | 507.7    |
| strata        | mixed        | 7.274    | 8.760       | 8.885    | 793.3    |
| msgspec       | mixed        | 12.145   | 13.003      | 13.143   | 795.7    |
| orjson        | mixed        | 12.793   | 13.066      | 14.857   | 793.3    |
| ujson         | mixed        | 15.226   | 17.986      | 24.592   | 795.7    |
| json (stdlib) | mixed        | 24.912   | 31.911      | 32.501   | 795.7    |
| strata        | nested       | 5.433    | 6.748       | 7.142    | 500.2    |
| msgspec       | nested       | 15.120   | 18.528      | 34.908   | 502.1    |
| ujson         | nested       | 16.896   | 25.223      | 37.723   | 502.1    |
| orjson        | nested       | 14.130   | 25.609      | 27.449   | 500.2    |
| json (stdlib) | nested       | 22.778   | 27.413      | 38.652   | 502.1    |
| strata        | users.json   | 167.316  | 213.006     | 216.294  | 522.1    |
| orjson        | users.json   | 204.270  | 281.294     | 295.140  | 522.1    |
| msgspec       | users.json   | 210.196  | 297.598     | 313.742  | 555.2    |
| ujson         | users.json   | 298.687  | 422.931     | 427.657  | 677.7    |
| json (stdlib) | users.json   | 428.267  | 505.173     | 514.729  | 555.2    |
| strata        | users.ndjson | 180.984  | 219.090     | 223.545  | 608.7    |
| orjson        | users.ndjson | 267.320  | 317.520     | 332.186  | 608.8    |
| msgspec       | users.ndjson | 284.200  | 324.615     | 345.635  | 647.0    |
| ujson         | users.ndjson | 368.066  | 486.255     | 499.131  | 769.5    |
| json (stdlib) | users.ndjson | 479.577  | 537.125     | 543.087  | 648.7    |
| orjson        | wide_arrays  | 30.328   | 41.230      | 50.698   | 475.5    |
| strata        | wide_arrays  | 35.092   | 42.844      | 45.606   | 474.5    |
| msgspec       | wide_arrays  | 45.574   | 47.690      | 49.372   | 490.5    |
| ujson         | wide_arrays  | 68.877   | 74.920      | 79.950   | 489.5    |
| json (stdlib) | wide_arrays  | 129.638  | 140.727     | 144.982  | 491.5    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.243    | 2.252       | 2.269    | 507.7    |
| orjson  | flat        | 2.926    | 2.933       | 2.952    | 503.4    |
| msgspec | flat        | 2.919    | 2.944       | 2.989    | 503.4    |
| ujson   | flat        | 11.985   | 12.098      | 12.196   | 505.4    |
| json    | flat        | 22.724   | 22.911      | 23.084   | 505.3    |
| strata  | mixed       | 2.455    | 2.460       | 2.464    | 795.7    |
| msgspec | mixed       | 2.825    | 2.834       | 3.007    | 798.1    |
| orjson  | mixed       | 3.157    | 3.228       | 3.895    | 793.9    |
| ujson   | mixed       | 12.600   | 12.858      | 12.899   | 798.1    |
| json    | mixed       | 28.161   | 28.527      | 28.569   | 805.5    |
| strata  | nested      | 1.844    | 1.879       | 1.941    | 502.1    |
| orjson  | nested      | 1.969    | 2.015       | 2.360    | 502.1    |
| msgspec | nested      | 1.996    | 2.038       | 2.084    | 502.1    |
| ujson   | nested      | 10.031   | 10.048      | 10.049   | 503.3    |
| json    | nested      | 13.134   | 13.303      | 13.309   | 503.3    |
| strata  | users.json  | 46.510   | 47.321      | 51.153   | 590.7    |
| orjson  | users.json  | 80.546   | 81.620      | 83.358   | 747.5    |
| msgspec | users.json  | 82.202   | 82.425      | 83.679   | 944.7    |
| ujson   | users.json  | 298.119  | 298.231     | 300.004  | 1139.9   |
| json    | users.json  | 425.521  | 426.213     | 427.506  | 1354.5   |
| strata  | wide_arrays | 13.674   | 13.715      | 14.110   | 489.5    |
| msgspec | wide_arrays | 17.099   | 17.170      | 17.739   | 535.0    |
| orjson  | wide_arrays | 17.508   | 17.750      | 17.914   | 498.2    |
| ujson   | wide_arrays | 72.898   | 73.058      | 76.046   | 581.0    |
| json    | wide_arrays | 201.699  | 202.030     | 202.263  | 633.2    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 3.104    | 3.151       | 3.247    | 505.3    |
| orjson        | flat        | 3.365    | 3.625       | 3.812    | 505.2    |
| msgspec       | flat        | 3.399    | 3.833       | 4.116    | 505.2    |
| ujson         | flat        | 12.899   | 13.290      | 14.650   | 505.2    |
| json (stdlib) | flat        | 83.154   | 83.612      | 84.131   | 505.2    |
| strata        | mixed       | 3.297    | 3.426       | 3.667    | 805.5    |
| orjson        | mixed       | 3.823    | 4.091       | 4.099    | 805.3    |
| msgspec       | mixed       | 3.384    | 4.172       | 4.446    | 805.3    |
| ujson         | mixed       | 13.276   | 13.500      | 13.609   | 805.3    |
| json (stdlib) | mixed       | 95.861   | 96.315      | 96.396   | 805.3    |
| strata        | nested      | 2.151    | 2.218       | 2.246    | 503.3    |
| msgspec       | nested      | 2.479    | 2.511       | 2.588    | 503.3    |
| orjson        | nested      | 2.531    | 2.548       | 3.236    | 503.3    |
| ujson         | nested      | 10.320   | 10.457      | 10.566   | 503.3    |
| json (stdlib) | nested      | 120.302  | 120.505     | 121.417  | 503.3    |
| strata        | users.json  | 54.878   | 57.754      | 60.636   | 1398.4   |
| orjson        | users.json  | 93.765   | 103.959     | 112.075  | 1554.2   |
| msgspec       | users.json  | 102.547  | 114.233     | 114.308  | 1865.6   |
| ujson         | users.json  | 320.824  | 322.920     | 329.154  | 1709.8   |
| json (stdlib) | users.json  | 3016.609 | 3017.500    | 3026.193 | 1688.7   |
| strata        | wide_arrays | 16.096   | 16.228      | 17.061   | 633.2    |
| orjson        | wide_arrays | 19.359   | 19.542      | 22.148   | 682.3    |
| msgspec       | wide_arrays | 20.048   | 20.306      | 26.136   | 780.5    |
| ujson         | wide_arrays | 75.174   | 75.248      | 77.273   | 731.3    |
| json (stdlib) | wide_arrays | 450.904  | 451.724     | 451.855  | 780.5    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 76.545   | 4000    | 1469.8   |
|                                | orjson+jsonpath-ng | 209.974  | 4000    | 1401.1   |
|                                | orjson+jmespath    | 225.278  | 4000    | 1394.1   |
| all item prices                | strata             | 86.075   | 789913  | 1427.0   |
|                                | orjson+jmespath    | 547.801  | 4000    | 1488.8   |
|                                | orjson+jsonpath-ng | 1949.677 | 789913  | 910.1    |
| recursive price                | strata             | 84.021   | 789913  | 883.3    |
|                                | orjson+jsonpath-ng | 6311.176 | 789913  | 712.4    |
| flat:$.records\[\*\].id        | strata             | 3.750    | 5000    | 505.2    |
|                                | orjson+jmespath    | 10.684   | 5000    | 505.2    |
|                                | orjson+jsonpath-ng | 17.042   | 5000    | 502.2    |
| flat:$.records\[0\]            | strata             | 3.691    | 1       | 502.2    |
|                                | orjson+jsonpath-ng | 8.907    | 1       | 502.2    |
|                                | orjson+jmespath    | 9.036    | 1       | 502.2    |
| nested:$.records\[\*\].id      | strata             | 2.825    | 5000    | 503.3    |
|                                | orjson+jmespath    | 16.677   | 5000    | 503.3    |
|                                | orjson+jsonpath-ng | 22.776   | 5000    | 503.3    |
| nested:$.records\[0\]          | strata             | 2.754    | 1       | 503.3    |
|                                | orjson+jsonpath-ng | 15.026   | 1       | 503.3    |
|                                | orjson+jmespath    | 15.230   | 1       | 503.3    |
| wide_arrays:$.records\[\*\].id | strata             | 26.392   | 5000    | 780.5    |
|                                | orjson+jmespath    | 37.350   | 5000    | 780.5    |
|                                | orjson+jsonpath-ng | 43.472   | 5000    | 780.5    |
| wide_arrays:$.records\[0\]     | strata             | 26.680   | 1       | 780.5    |
|                                | orjson+jsonpath-ng | 34.837   | 1       | 780.5    |
|                                | orjson+jmespath    | 37.084   | 1       | 780.5    |
| mixed:$.records\[\*\].id       | strata             | 4.399    | 5000    | 805.3    |
|                                | orjson+jmespath    | 12.221   | 5000    | 805.3    |
|                                | orjson+jsonpath-ng | 18.209   | 5000    | 805.3    |
| mixed:$.records\[0\]           | strata             | 4.257    | 1       | 805.4    |
|                                | orjson+jsonpath-ng | 10.782   | 1       | 805.4    |
|                                | orjson+jmespath    | 11.242   | 1       | 805.4    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.758    | 4000    | 940.9    |
|                                            | jmespath    | 2.226    | 4000    | 940.9    |
|                                            | jsonpath-ng | 90.683   | 4000    | 942.6    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 43.813   | 789913  | 942.7    |
|                                            | jmespath    | 426.776  | 4000    | 957.6    |
|                                            | jsonpath-ng | 1730.168 | 789913  | 805.8    |
| $..price                                   | strata      | 58.637   | 789913  | 819.1    |
|                                            | jsonpath-ng | 6235.385 | 789913  | 817.9    |
| $.users\[?(@.age>30)\]                     | strata      | 0.835    | 3212    | 821.5    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 62.059   | 26406   | 821.5    |
| flat:$.records\[\*\].id                    | strata      | 0.424    | 5000    | 502.2    |
|                                            | jmespath    | 1.669    | 5000    | 502.2    |
|                                            | jsonpath-ng | 9.653    | 5000    | 502.2    |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 502.2    |
|                                            | jsonpath-ng | 0.011    | 1       | 502.2    |
|                                            | jmespath    | 0.012    | 1       | 502.2    |
| nested:$.records\[\*\].id                  | strata      | 0.462    | 5000    | 503.4    |
|                                            | jmespath    | 1.793    | 5000    | 503.4    |
|                                            | jsonpath-ng | 16.433   | 5000    | 503.4    |
| nested:$.records\[0\]                      | strata      | 0.009    | 1       | 503.4    |
|                                            | jmespath    | 0.010    | 1       | 503.4    |
|                                            | jsonpath-ng | 0.016    | 1       | 503.4    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.263    | 5000    | 780.5    |
|                                            | jmespath    | 1.743    | 5000    | 780.5    |
|                                            | jsonpath-ng | 16.401   | 5000    | 780.5    |
| wide_arrays:$.records\[0\]                 | strata      | 0.010    | 1       | 780.5    |
|                                            | jmespath    | 0.010    | 1       | 780.5    |
|                                            | jsonpath-ng | 0.011    | 1       | 780.5    |
| mixed:$.records\[\*\].id                   | strata      | 0.358    | 5000    | 805.4    |
|                                            | jmespath    | 1.862    | 5000    | 805.4    |
|                                            | jsonpath-ng | 12.038   | 5000    | 805.4    |
| mixed:$.records\[0\]                       | jmespath    | 0.009    | 1       | 805.4    |
|                                            | jsonpath-ng | 0.010    | 1       | 805.4    |
|                                            | strata      | 0.010    | 1       | 805.4    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **27.5% faster** than #2 (orjson)  |
| loads (NDJSON)      | **#1** / 5              | **53.2% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **22.1% faster** than #2 (orjson)  |
| load (NDJSON file)  | **#1** / 5              | **47.7% faster** than #2 (orjson)  |
| dumps               | **#1** / 5              | **73.2% faster** than #2 (orjson)  |
| dump                | **#1** / 5              | **70.9% faster** than #2 (orjson)  |
| loads (flat)        | **#1** / 5              | **36.7% faster** than #2 (msgspec) |
| load (flat)         | **#1** / 5              | **35.1% faster** than #2 (orjson)  |
| dumps (flat)        | **#1** / 5              | **30.1% faster** than #2 (msgspec) |
| dump (flat)         | **#1** / 5              | **8.4% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **171.2% faster** than #2 (orjson) |
| load (nested)       | **#1** / 5              | **160.1% faster** than #2 (orjson) |
| dumps (nested)      | **#1** / 5              | **6.7% faster** than #2 (orjson)   |
| dump (nested)       | **#1** / 5              | **15.3% faster** than #2 (msgspec) |
| loads (wide_arrays) | **#2** / 5              | 3.1% behind #1 (orjson)            |
| load (wide_arrays)  | **#2** / 5              | 15.7% behind #1 (orjson)           |
| dumps (wide_arrays) | **#1** / 5              | **25.0% faster** than #2 (msgspec) |
| dump (wide_arrays)  | **#1** / 5              | **20.3% faster** than #2 (orjson)  |
| loads (mixed)       | **#1** / 5              | **84.0% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **67.0% faster** than #2 (msgspec) |
| dumps (mixed)       | **#1** / 5              | **15.0% faster** than #2 (msgspec) |
| dump (mixed)        | **#1** / 5              | **2.7% faster** than #2 (msgspec)  |
| search (JSONPath)   | **#1** in 11/11 queries | avg **890% faster**                |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
