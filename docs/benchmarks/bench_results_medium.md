# Strata Benchmark Results

Generated: 2026-03-18 00:40:11

## Environment

- Commit: 5b6b37b39dfd1e18b3f00b8af9c26aa1e984d69c
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/medium/users.json, benchmarks/data/generated/medium/users.ndjson
- Random schemas: flat, nested, wide_arrays, mixed (2000 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 2.775    | 3.332       | 3.444    | 274.3    |
| orjson        | flat         | 3.567    | 4.093       | 4.140    | 276.8    |
| msgspec       | flat         | 4.096    | 5.199       | 5.604    | 276.8    |
| ujson         | flat         | 6.013    | 6.870       | 6.937    | 276.8    |
| json (stdlib) | flat         | 10.064   | 10.183      | 10.601   | 276.8    |
| strata        | mixed        | 2.799    | 3.462       | 3.584    | 290.1    |
| orjson        | mixed        | 4.090    | 4.841       | 4.850    | 292.4    |
| msgspec       | mixed        | 4.746    | 5.311       | 5.657    | 292.4    |
| ujson         | mixed        | 6.972    | 8.495       | 9.646    | 292.4    |
| json (stdlib) | mixed        | 10.142   | 10.898      | 15.380   | 292.4    |
| strata        | nested       | 2.468    | 3.259       | 3.443    | 280.6    |
| ujson         | nested       | 7.438    | 8.388       | 9.849    | 283.0    |
| msgspec       | nested       | 5.015    | 8.796       | 13.604   | 283.0    |
| orjson        | nested       | 6.472    | 8.837       | 9.049    | 283.0    |
| json (stdlib) | nested       | 9.221    | 10.216      | 10.662   | 283.0    |
| strata        | users.json   | 25.844   | 31.547      | 33.185   | 73.6     |
| orjson        | users.json   | 26.947   | 38.677      | 54.208   | 93.5     |
| msgspec       | users.json   | 33.745   | 43.384      | 44.471   | 92.3     |
| ujson         | users.json   | 39.912   | 58.432      | 61.351   | 110.0    |
| json (stdlib) | users.json   | 55.978   | 71.366      | 78.179   | 98.6     |
| strata        | users.ndjson | 25.615   | 32.350      | 32.354   | 256.1    |
| orjson        | users.ndjson | 36.239   | 38.679      | 39.059   | 264.2    |
| msgspec       | users.ndjson | 38.861   | 39.875      | 44.592   | 265.3    |
| ujson         | users.ndjson | 50.863   | 51.401      | 56.699   | 264.3    |
| json (stdlib) | users.ndjson | 65.473   | 65.862      | 72.967   | 266.3    |
| orjson        | wide_arrays  | 14.318   | 15.340      | 16.571   | 304.4    |
| strata        | wide_arrays  | 13.772   | 16.036      | 16.724   | 294.2    |
| msgspec       | wide_arrays  | 15.144   | 18.670      | 18.881   | 304.4    |
| ujson         | wide_arrays  | 24.579   | 28.189      | 33.243   | 304.4    |
| json (stdlib) | wide_arrays  | 51.220   | 55.338      | 56.360   | 304.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 2.918    | 3.406       | 3.432    | 276.8    |
| orjson        | flat         | 4.080    | 4.553       | 4.563    | 276.8    |
| msgspec       | flat         | 4.560    | 4.753       | 4.957    | 276.8    |
| ujson         | flat         | 6.295    | 8.308       | 11.219   | 276.8    |
| json (stdlib) | flat         | 10.601   | 10.617      | 10.762   | 276.8    |
| strata        | mixed        | 3.288    | 3.664       | 3.673    | 292.4    |
| orjson        | mixed        | 4.147    | 5.431       | 5.999    | 292.4    |
| msgspec       | mixed        | 5.521    | 5.814       | 5.825    | 292.4    |
| ujson         | mixed        | 6.924    | 7.572       | 10.819   | 292.4    |
| json (stdlib) | mixed        | 10.581   | 11.537      | 11.899   | 292.4    |
| strata        | nested       | 2.598    | 3.078       | 3.154    | 283.0    |
| msgspec       | nested       | 4.597    | 6.982       | 8.061    | 283.0    |
| orjson        | nested       | 7.826    | 7.931       | 8.193    | 283.0    |
| ujson         | nested       | 6.487    | 8.637       | 15.379   | 283.0    |
| json (stdlib) | nested       | 8.736    | 9.638       | 9.779    | 283.0    |
| strata        | users.json   | 24.964   | 31.367      | 34.369   | 100.0    |
| orjson        | users.json   | 27.565   | 42.009      | 56.943   | 100.0    |
| msgspec       | users.json   | 29.756   | 42.984      | 59.317   | 104.9    |
| ujson         | users.json   | 40.806   | 62.660      | 73.033   | 122.6    |
| json (stdlib) | users.json   | 65.908   | 71.912      | 75.266   | 104.9    |
| strata        | users.ndjson | 26.870   | 32.179      | 32.678   | 268.3    |
| orjson        | users.ndjson | 35.491   | 36.453      | 37.720   | 269.4    |
| msgspec       | users.ndjson | 36.482   | 38.452      | 38.905   | 270.4    |
| ujson         | users.ndjson | 53.256   | 54.288      | 54.919   | 269.4    |
| json (stdlib) | users.ndjson | 65.147   | 65.149      | 77.892   | 271.4    |
| orjson        | wide_arrays  | 12.589   | 16.162      | 21.010   | 304.1    |
| strata        | wide_arrays  | 13.872   | 16.322      | 16.430   | 304.4    |
| msgspec       | wide_arrays  | 15.978   | 19.615      | 19.624   | 304.1    |
| ujson         | wide_arrays  | 26.030   | 30.025      | 30.129   | 304.1    |
| json (stdlib) | wide_arrays  | 55.181   | 56.922      | 62.209   | 304.1    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.898    | 0.898       | 0.909    | 276.8    |
| orjson  | flat        | 1.021    | 1.023       | 1.138    | 279.1    |
| msgspec | flat        | 1.159    | 1.160       | 1.165    | 279.1    |
| ujson   | flat        | 4.841    | 4.866       | 5.065    | 279.5    |
| json    | flat        | 9.046    | 9.083       | 9.115    | 279.5    |
| strata  | mixed       | 1.016    | 1.055       | 1.215    | 292.4    |
| orjson  | mixed       | 1.121    | 1.142       | 1.143    | 292.4    |
| msgspec | mixed       | 1.179    | 1.182       | 1.194    | 292.4    |
| ujson   | mixed       | 5.079    | 5.079       | 5.156    | 292.4    |
| json    | mixed       | 11.663   | 12.037      | 12.395   | 292.4    |
| strata  | nested      | 0.752    | 0.753       | 1.148    | 283.0    |
| msgspec | nested      | 0.804    | 0.819       | 1.800    | 283.0    |
| orjson  | nested      | 0.796    | 0.847       | 0.861    | 283.0    |
| ujson   | nested      | 4.018    | 4.220       | 4.368    | 283.0    |
| json    | nested      | 5.227    | 5.251       | 5.283    | 283.0    |
| strata  | users.json  | 6.679    | 6.856       | 7.736    | 109.1    |
| msgspec | users.json  | 10.747   | 10.799      | 10.921   | 161.2    |
| orjson  | users.json  | 11.534   | 11.893      | 13.690   | 132.4    |
| ujson   | users.json  | 41.261   | 41.937      | 42.902   | 167.6    |
| json    | users.json  | 59.202   | 59.266      | 59.398   | 192.9    |
| strata  | wide_arrays | 5.488    | 5.505       | 6.248    | 306.5    |
| msgspec | wide_arrays | 6.904    | 6.960       | 9.021    | 316.6    |
| orjson  | wide_arrays | 6.985    | 6.999       | 7.116    | 296.9    |
| ujson   | wide_arrays | 28.663   | 28.763      | 28.791   | 318.6    |
| json    | wide_arrays | 80.588   | 81.100      | 81.160   | 339.5    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.233    | 1.310       | 1.349    | 279.5    |
| orjson        | flat        | 1.409    | 1.455       | 1.491    | 279.5    |
| msgspec       | flat        | 1.571    | 1.623       | 1.703    | 280.5    |
| ujson         | flat        | 5.297    | 5.492       | 6.330    | 280.5    |
| json (stdlib) | flat        | 33.805   | 33.982      | 34.497   | 280.5    |
| strata        | mixed       | 1.219    | 1.236       | 1.300    | 292.4    |
| orjson        | mixed       | 1.361    | 1.372       | 1.440    | 292.4    |
| msgspec       | mixed       | 1.707    | 1.762       | 2.261    | 292.4    |
| ujson         | mixed       | 5.562    | 5.610       | 5.643    | 292.4    |
| json (stdlib) | mixed       | 38.794   | 39.037      | 45.678   | 292.4    |
| strata        | nested      | 0.941    | 1.013       | 1.022    | 283.0    |
| orjson        | nested      | 1.054    | 1.077       | 1.206    | 283.0    |
| msgspec       | nested      | 1.121    | 1.169       | 1.216    | 283.0    |
| ujson         | nested      | 4.588    | 4.818       | 4.828    | 283.0    |
| json (stdlib) | nested      | 48.084   | 53.223      | 55.077   | 283.0    |
| strata        | users.json  | 7.637    | 7.779       | 7.992    | 199.2    |
| orjson        | users.json  | 11.669   | 11.867      | 14.201   | 221.5    |
| msgspec       | users.json  | 11.798   | 12.222      | 12.278   | 243.9    |
| ujson         | users.json  | 43.083   | 44.336      | 44.349   | 221.6    |
| json (stdlib) | users.json  | 434.290  | 449.871     | 458.107  | 244.0    |
| strata        | wide_arrays | 6.683    | 6.962       | 7.053    | 339.5    |
| msgspec       | wide_arrays | 8.396    | 8.404       | 8.408    | 290.1    |
| orjson        | wide_arrays | 7.963    | 8.445       | 8.931    | 337.2    |
| ujson         | wide_arrays | 29.982   | 30.452      | 30.856   | 270.4    |
| json (stdlib) | wide_arrays | 179.648  | 179.732     | 180.254  | 290.1    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 12.341   | 2000    | 241.1    |
|                                | orjson+jmespath    | 30.173   | 2000    | 244.2    |
|                                | orjson+jsonpath-ng | 32.819   | 2000    | 246.3    |
| all item prices                | strata             | 13.929   | 100999  | 248.3    |
|                                | orjson+jmespath    | 81.689   | 2000    | 257.5    |
|                                | orjson+jsonpath-ng | 260.532  | 100999  | 264.7    |
| recursive price                | strata             | 13.546   | 100999  | 264.0    |
|                                | orjson+jsonpath-ng | 878.952  | 100999  | 267.7    |
| flat:$.records\[\*\].id        | strata             | 1.806    | 2000    | 280.6    |
|                                | orjson+jmespath    | 4.567    | 2000    | 280.6    |
|                                | orjson+jsonpath-ng | 6.840    | 2000    | 280.6    |
| flat:$.records\[0\]            | strata             | 1.834    | 1       | 280.6    |
|                                | orjson+jsonpath-ng | 3.889    | 1       | 280.6    |
|                                | orjson+jmespath    | 4.062    | 1       | 280.6    |
| nested:$.records\[\*\].id      | strata             | 1.482    | 2000    | 283.0    |
|                                | orjson+jmespath    | 6.428    | 2000    | 283.0    |
|                                | orjson+jsonpath-ng | 9.030    | 2000    | 282.0    |
| nested:$.records\[0\]          | strata             | 1.391    | 1       | 282.0    |
|                                | orjson+jsonpath-ng | 4.970    | 1       | 282.0    |
|                                | orjson+jmespath    | 5.103    | 1       | 282.0    |
| wide_arrays:$.records\[\*\].id | strata             | 10.767   | 2000    | 290.1    |
|                                | orjson+jmespath    | 15.987   | 2000    | 290.1    |
|                                | orjson+jsonpath-ng | 17.667   | 2000    | 290.1    |
| wide_arrays:$.records\[0\]     | strata             | 10.695   | 1       | 290.1    |
|                                | orjson+jsonpath-ng | 14.334   | 1       | 290.1    |
|                                | orjson+jmespath    | 14.701   | 1       | 290.1    |
| mixed:$.records\[\*\].id       | strata             | 1.944    | 2000    | 292.4    |
|                                | orjson+jmespath    | 5.842    | 2000    | 292.4    |
|                                | orjson+jsonpath-ng | 7.572    | 2000    | 292.4    |
| mixed:$.records\[0\]           | strata             | 1.893    | 1       | 292.4    |
|                                | orjson+jsonpath-ng | 4.557    | 1       | 292.4    |
|                                | orjson+jmespath    | 4.876    | 1       | 292.4    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.294    | 2000    | 289.9    |
|                                            | jmespath    | 0.889    | 2000    | 289.9    |
|                                            | jsonpath-ng | 15.923   | 2000    | 290.7    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.145    | 100999  | 290.7    |
|                                            | jmespath    | 62.967   | 2000    | 294.5    |
|                                            | jsonpath-ng | 254.042  | 100999  | 296.9    |
| $..price                                   | strata      | 8.148    | 100999  | 296.8    |
|                                            | jsonpath-ng | 884.588  | 100999  | 280.4    |
| $.users\[?(@.age>30)\]                     | strata      | 0.322    | 1606    | 277.4    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.130    | 6650    | 277.4    |
| flat:$.records\[\*\].id                    | strata      | 0.119    | 2000    | 280.6    |
|                                            | jmespath    | 0.676    | 2000    | 280.6    |
|                                            | jsonpath-ng | 4.504    | 2000    | 280.6    |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 280.6    |
|                                            | jmespath    | 0.006    | 1       | 280.6    |
|                                            | jsonpath-ng | 0.008    | 1       | 280.6    |
| nested:$.records\[\*\].id                  | strata      | 0.105    | 2000    | 282.0    |
|                                            | jmespath    | 0.731    | 2000    | 282.0    |
|                                            | jsonpath-ng | 6.834    | 2000    | 282.0    |
| nested:$.records\[0\]                      | strata      | 0.004    | 1       | 282.0    |
|                                            | jsonpath-ng | 0.008    | 1       | 282.0    |
|                                            | jmespath    | 0.010    | 1       | 282.0    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.110    | 2000    | 290.1    |
|                                            | jmespath    | 0.681    | 2000    | 290.1    |
|                                            | jsonpath-ng | 7.003    | 2000    | 290.1    |
| wide_arrays:$.records\[0\]                 | jmespath    | 0.009    | 1       | 290.1    |
|                                            | strata      | 0.010    | 1       | 290.1    |
|                                            | jsonpath-ng | 0.011    | 1       | 290.1    |
| mixed:$.records\[\*\].id                   | strata      | 0.113    | 2000    | 292.4    |
|                                            | jmespath    | 0.675    | 2000    | 292.4    |
|                                            | jsonpath-ng | 5.726    | 2000    | 292.4    |
| mixed:$.records\[0\]                       | strata      | 0.004    | 1       | 292.4    |
|                                            | jmespath    | 0.007    | 1       | 292.4    |
|                                            | jsonpath-ng | 0.008    | 1       | 292.4    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **4.3% faster** than #2 (orjson)    |
| loads (NDJSON)      | **#1** / 5              | **41.5% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **10.4% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **32.1% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **60.9% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **52.8% faster** than #2 (orjson)   |
| loads (flat)        | **#1** / 5              | **28.5% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **39.8% faster** than #2 (orjson)   |
| dumps (flat)        | **#1** / 5              | **13.7% faster** than #2 (orjson)   |
| dump (flat)         | **#1** / 5              | **14.3% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **103.2% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **76.9% faster** than #2 (msgspec)  |
| dumps (nested)      | **#1** / 5              | **5.8% faster** than #2 (orjson)    |
| dump (nested)       | **#1** / 5              | **12.0% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#1** / 5              | **4.0% faster** than #2 (orjson)    |
| load (wide_arrays)  | **#2** / 5              | 10.2% behind #1 (orjson)            |
| dumps (wide_arrays) | **#1** / 5              | **25.8% faster** than #2 (msgspec)  |
| dump (wide_arrays)  | **#1** / 5              | **19.2% faster** than #2 (orjson)   |
| loads (mixed)       | **#1** / 5              | **46.1% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **26.1% faster** than #2 (orjson)   |
| dumps (mixed)       | **#1** / 5              | **10.3% faster** than #2 (orjson)   |
| dump (mixed)        | **#1** / 5              | **11.7% faster** than #2 (orjson)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **754% faster**                 |
| query (JSONPath)    | **#1** in 12/13 queries | -                                   |
