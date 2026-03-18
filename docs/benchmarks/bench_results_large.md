# Strata Benchmark Results

Generated: 2026-03-18 23:09:14

## Environment

- Commit: fe648ede876d126597669efe7e2ea296a394b703
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/users.json, benchmarks/data/generated/users.ndjson
- Random schemas: flat, nested, wide_arrays, mixed (5000 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 7.512    | 9.001       | 9.136    | 220.5    |
| msgspec       | flat         | 11.073   | 11.157      | 13.468   | 226.7    |
| orjson        | flat         | 9.802    | 12.521      | 13.364   | 226.7    |
| ujson         | flat         | 18.508   | 21.279      | 24.619   | 226.7    |
| json (stdlib) | flat         | 25.296   | 27.557      | 28.635   | 226.7    |
| strata        | mixed        | 6.028    | 7.487       | 7.507    | 508.5    |
| orjson        | mixed        | 9.419    | 12.831      | 19.690   | 514.1    |
| msgspec       | mixed        | 10.286   | 15.820      | 19.637   | 514.1    |
| ujson         | mixed        | 14.767   | 17.801      | 18.190   | 514.1    |
| json (stdlib) | mixed        | 24.181   | 31.608      | 36.743   | 516.5    |
| strata        | nested       | 5.730    | 7.050       | 7.750    | 225.6    |
| orjson        | nested       | 15.368   | 18.598      | 20.133   | 225.7    |
| msgspec       | nested       | 14.422   | 20.205      | 27.067   | 225.7    |
| ujson         | nested       | 24.379   | 24.657      | 33.529   | 225.7    |
| json (stdlib) | nested       | 23.294   | 27.907      | 47.979   | 227.5    |
| strata        | users.json   | 24.915   | 25.180      | 29.885   | 64.4     |
| orjson        | users.json   | 28.338   | 41.050      | 64.643   | 80.5     |
| msgspec       | users.json   | 37.565   | 51.101      | 52.368   | 80.2     |
| ujson         | users.json   | 44.618   | 58.747      | 62.566   | 94.2     |
| json (stdlib) | users.json   | 52.197   | 64.080      | 69.678   | 85.2     |
| strata        | users.ndjson | 21.585   | 26.657      | 26.896   | 197.3    |
| msgspec       | users.ndjson | 31.289   | 31.910      | 33.854   | 204.9    |
| orjson        | users.ndjson | 32.934   | 33.391      | 33.523   | 203.8    |
| ujson         | users.ndjson | 46.171   | 46.477      | 58.900   | 204.9    |
| json (stdlib) | users.ndjson | 57.267   | 58.888      | 60.750   | 205.9    |
| strata        | wide_arrays  | 27.975   | 36.919      | 37.486   | 282.5    |
| orjson        | wide_arrays  | 32.291   | 47.250      | 54.637   | 254.5    |
| msgspec       | wide_arrays  | 41.186   | 51.738      | 65.156   | 264.5    |
| ujson         | wide_arrays  | 79.104   | 81.208      | 91.439   | 265.5    |
| json (stdlib) | wide_arrays  | 128.835  | 139.757     | 158.219  | 277.6    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 7.822    | 9.224       | 11.497   | 226.7    |
| orjson        | flat         | 9.196    | 10.444      | 11.003   | 224.6    |
| msgspec       | flat         | 9.357    | 11.049      | 11.483   | 224.6    |
| ujson         | flat         | 17.870   | 21.218      | 26.349   | 224.6    |
| json (stdlib) | flat         | 24.114   | 25.513      | 25.987   | 224.6    |
| strata        | mixed        | 7.102    | 7.971       | 8.258    | 516.5    |
| orjson        | mixed        | 9.988    | 12.711      | 20.604   | 516.5    |
| msgspec       | mixed        | 10.961   | 18.337      | 23.374   | 518.9    |
| ujson         | mixed        | 15.287   | 18.918      | 19.480   | 518.9    |
| json (stdlib) | mixed        | 25.066   | 28.760      | 30.278   | 518.9    |
| strata        | nested       | 6.126    | 7.387       | 7.438    | 227.5    |
| msgspec       | nested       | 14.790   | 19.688      | 20.793   | 229.5    |
| orjson        | nested       | 14.894   | 19.806      | 21.180   | 227.5    |
| ujson         | nested       | 22.352   | 22.712      | 30.455   | 229.5    |
| json (stdlib) | nested       | 25.498   | 27.888      | 40.469   | 233.4    |
| strata        | users.json   | 21.597   | 28.081      | 28.320   | 85.7     |
| orjson        | users.json   | 27.340   | 39.099      | 44.604   | 85.7     |
| msgspec       | users.json   | 28.445   | 39.919      | 41.275   | 90.3     |
| ujson         | users.json   | 37.577   | 60.572      | 76.129   | 104.4    |
| json (stdlib) | users.json   | 65.785   | 70.174      | 74.249   | 90.3     |
| strata        | users.ndjson | 19.927   | 24.667      | 25.321   | 206.9    |
| msgspec       | users.ndjson | 33.112   | 33.159      | 34.088   | 210.4    |
| orjson        | users.ndjson | 32.076   | 33.249      | 37.552   | 207.9    |
| ujson         | users.ndjson | 45.008   | 49.400      | 50.729   | 210.4    |
| json (stdlib) | users.ndjson | 58.990   | 66.427      | 68.583   | 211.4    |
| strata        | wide_arrays  | 28.426   | 36.809      | 37.705   | 277.3    |
| orjson        | wide_arrays  | 31.956   | 43.738      | 44.354   | 290.3    |
| msgspec       | wide_arrays  | 40.549   | 53.221      | 60.493   | 304.4    |
| ujson         | wide_arrays  | 65.627   | 79.920      | 92.428   | 303.5    |
| json (stdlib) | wide_arrays  | 139.212  | 148.285     | 148.576  | 305.4    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.265    | 2.300       | 2.589    | 227.4    |
| orjson  | flat        | 2.394    | 2.404       | 2.538    | 227.4    |
| msgspec | flat        | 2.856    | 2.898       | 2.911    | 227.4    |
| ujson   | flat        | 12.059   | 12.108      | 12.227   | 227.4    |
| json    | flat        | 22.571   | 22.878      | 25.009   | 227.6    |
| strata  | mixed       | 2.610    | 2.646       | 3.161    | 518.9    |
| msgspec | mixed       | 2.809    | 2.831       | 3.025    | 521.5    |
| orjson  | mixed       | 3.018    | 3.225       | 3.268    | 517.0    |
| ujson   | mixed       | 12.448   | 12.719      | 12.800   | 523.5    |
| json    | mixed       | 28.094   | 28.107      | 28.174   | 533.2    |
| strata  | nested      | 1.944    | 1.950       | 2.037    | 233.4    |
| orjson  | nested      | 2.011    | 2.018       | 2.331    | 233.8    |
| msgspec | nested      | 2.007    | 2.024       | 2.038    | 234.0    |
| ujson   | nested      | 9.701    | 9.920       | 10.545   | 234.0    |
| json    | nested      | 13.124   | 13.649      | 14.256   | 234.3    |
| strata  | users.json  | 5.738    | 5.938       | 7.033    | 94.4     |
| msgspec | users.json  | 8.889    | 9.187       | 9.950    | 133.0    |
| orjson  | users.json  | 8.863    | 9.371       | 10.884   | 113.9    |
| ujson   | users.json  | 36.063   | 36.520      | 37.579   | 138.1    |
| json    | users.json  | 49.168   | 53.318      | 59.780   | 160.1    |
| strata  | wide_arrays | 14.453   | 14.487      | 15.438   | 315.7    |
| orjson  | wide_arrays | 18.119   | 18.141      | 18.470   | 328.4    |
| msgspec | wide_arrays | 17.641   | 18.145      | 18.347   | 350.0    |
| ujson   | wide_arrays | 74.750   | 76.936      | 77.495   | 376.3    |
| json    | wide_arrays | 209.507  | 217.825     | 230.806  | 428.5    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 2.728    | 2.766       | 2.778    | 227.6    |
| orjson        | flat        | 3.015    | 3.044       | 3.093    | 227.6    |
| msgspec       | flat        | 3.550    | 3.560       | 3.708    | 227.6    |
| ujson         | flat        | 12.737   | 12.944      | 13.042   | 227.6    |
| json (stdlib) | flat        | 82.080   | 82.326      | 82.466   | 227.6    |
| strata        | mixed       | 2.914    | 2.939       | 3.049    | 533.2    |
| msgspec       | mixed       | 3.304    | 3.339       | 3.412    | 533.2    |
| orjson        | mixed       | 3.562    | 3.734       | 3.795    | 533.2    |
| ujson         | mixed       | 13.444   | 13.565      | 13.568   | 533.2    |
| json (stdlib) | mixed       | 96.304   | 96.974      | 97.817   | 533.2    |
| strata        | nested      | 2.266    | 2.389       | 2.524    | 234.3    |
| orjson        | nested      | 2.470    | 2.499       | 2.516    | 234.3    |
| msgspec       | nested      | 2.491    | 2.505       | 2.555    | 234.3    |
| ujson         | nested      | 10.911   | 11.248      | 11.500   | 234.3    |
| json (stdlib) | nested      | 117.826  | 118.275     | 118.526  | 234.3    |
| strata        | users.json  | 6.464    | 6.637       | 8.298    | 165.2    |
| orjson        | users.json  | 9.017    | 9.044       | 10.774   | 183.3    |
| msgspec       | users.json  | 8.957    | 9.154       | 11.106   | 196.5    |
| ujson         | users.json  | 33.660   | 34.291      | 35.964   | 183.5    |
| json (stdlib) | users.json  | 409.780  | 421.997     | 453.328  | 196.5    |
| strata        | wide_arrays | 16.745   | 16.860      | 17.354   | 428.5    |
| msgspec       | wide_arrays | 20.500   | 21.110      | 21.335   | 524.7    |
| orjson        | wide_arrays | 21.077   | 21.330      | 22.049   | 426.5    |
| ujson         | wide_arrays | 78.495   | 78.502      | 78.734   | 475.5    |
| json (stdlib) | wide_arrays | 459.453  | 461.698     | 467.627  | 524.7    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 11.873   | 5000    | 196.7    |
|                                | orjson+jmespath    | 30.228   | 5000    | 197.1    |
|                                | orjson+jsonpath-ng | 37.233   | 5000    | 200.5    |
| all item prices                | strata             | 12.666   | 62331   | 200.8    |
|                                | orjson+jmespath    | 68.546   | 5000    | 205.7    |
|                                | orjson+jsonpath-ng | 220.467  | 62331   | 212.6    |
| recursive price                | strata             | 12.515   | 62331   | 209.8    |
|                                | orjson+jsonpath-ng | 725.062  | 62331   | 212.4    |
| flat:$.records\[\*\].id        | strata             | 4.489    | 5000    | 227.6    |
|                                | orjson+jmespath    | 11.299   | 5000    | 227.6    |
|                                | orjson+jsonpath-ng | 17.117   | 5000    | 225.6    |
| flat:$.records\[0\]            | strata             | 4.798    | 1       | 225.6    |
|                                | orjson+jsonpath-ng | 9.644    | 1       | 225.6    |
|                                | orjson+jmespath    | 10.183   | 1       | 225.6    |
| nested:$.records\[\*\].id      | strata             | 3.759    | 5000    | 234.3    |
|                                | orjson+jmespath    | 17.366   | 5000    | 234.3    |
|                                | orjson+jsonpath-ng | 25.332   | 5000    | 234.3    |
| nested:$.records\[0\]          | strata             | 4.054    | 1       | 234.3    |
|                                | orjson+jsonpath-ng | 15.239   | 1       | 234.3    |
|                                | orjson+jmespath    | 17.894   | 1       | 234.3    |
| wide_arrays:$.records\[\*\].id | strata             | 21.789   | 5000    | 524.7    |
|                                | orjson+jmespath    | 38.823   | 5000    | 524.7    |
|                                | orjson+jsonpath-ng | 46.219   | 5000    | 524.7    |
| wide_arrays:$.records\[0\]     | strata             | 22.672   | 1       | 524.7    |
|                                | orjson+jsonpath-ng | 34.717   | 1       | 524.7    |
|                                | orjson+jmespath    | 36.879   | 1       | 524.7    |
| mixed:$.records\[\*\].id       | strata             | 4.341    | 5000    | 533.2    |
|                                | orjson+jmespath    | 12.739   | 5000    | 533.2    |
|                                | orjson+jsonpath-ng | 20.814   | 5000    | 533.2    |
| mixed:$.records\[0\]           | strata             | 4.265    | 1       | 533.2    |
|                                | orjson+jmespath    | 10.622   | 1       | 533.2    |
|                                | orjson+jsonpath-ng | 10.992   | 1       | 533.2    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.734    | 5000    | 230.4    |
|                                            | jmespath    | 2.423    | 5000    | 230.4    |
|                                            | jsonpath-ng | 22.049   | 5000    | 232.5    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 4.882    | 62331   | 232.1    |
|                                            | jmespath    | 56.658   | 5000    | 235.1    |
|                                            | jsonpath-ng | 236.747  | 62331   | 240.2    |
| $..price                                   | strata      | 6.657    | 62331   | 239.2    |
|                                            | jsonpath-ng | 780.467  | 62331   | 241.4    |
| $.users\[?(@.age>30)\]                     | strata      | 0.915    | 3999    | 238.4    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 8.760    | 8402    | 213.4    |
| flat:$.records\[\*\].id                    | strata      | 0.322    | 5000    | 225.6    |
|                                            | jmespath    | 1.663    | 5000    | 225.6    |
|                                            | jsonpath-ng | 9.464    | 5000    | 225.6    |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 225.6    |
|                                            | jmespath    | 0.008    | 1       | 225.6    |
|                                            | jsonpath-ng | 0.013    | 1       | 225.6    |
| nested:$.records\[\*\].id                  | strata      | 0.614    | 5000    | 234.3    |
|                                            | jmespath    | 1.816    | 5000    | 234.3    |
|                                            | jsonpath-ng | 18.728   | 5000    | 186.2    |
| nested:$.records\[0\]                      | strata      | 0.009    | 1       | 186.2    |
|                                            | jmespath    | 0.016    | 1       | 186.2    |
|                                            | jsonpath-ng | 0.020    | 1       | 186.2    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.263    | 5000    | 524.7    |
|                                            | jmespath    | 1.754    | 5000    | 524.7    |
|                                            | jsonpath-ng | 16.165   | 5000    | 524.7    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 524.7    |
|                                            | jmespath    | 0.010    | 1       | 524.7    |
|                                            | jsonpath-ng | 0.010    | 1       | 524.7    |
| mixed:$.records\[\*\].id                   | strata      | 0.312    | 5000    | 533.2    |
|                                            | jmespath    | 1.773    | 5000    | 533.2    |
|                                            | jsonpath-ng | 11.550   | 5000    | 533.2    |
| mixed:$.records\[0\]                       | strata      | 0.005    | 1       | 533.2    |
|                                            | jsonpath-ng | 0.011    | 1       | 533.2    |
|                                            | jmespath    | 0.012    | 1       | 533.2    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **13.7% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **45.0% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **26.6% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **61.0% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **54.5% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **38.6% faster** than #2 (msgspec)  |
| loads (flat)        | **#1** / 5              | **30.5% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **17.6% faster** than #2 (orjson)   |
| dumps (flat)        | **#1** / 5              | **5.7% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **10.6% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **151.7% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **141.4% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **3.2% faster** than #2 (msgspec)   |
| dump (nested)       | **#1** / 5              | **9.0% faster** than #2 (orjson)    |
| loads (wide_arrays) | **#1** / 5              | **15.4% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **12.4% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **22.1% faster** than #2 (msgspec)  |
| dump (wide_arrays)  | **#1** / 5              | **22.4% faster** than #2 (msgspec)  |
| loads (mixed)       | **#1** / 5              | **56.3% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **40.6% faster** than #2 (orjson)   |
| dumps (mixed)       | **#1** / 5              | **7.6% faster** than #2 (msgspec)   |
| dump (mixed)        | **#1** / 5              | **13.4% faster** than #2 (msgspec)  |
| search (JSONPath)   | **#1** in 11/11 queries | avg **696% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
