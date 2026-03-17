# Strata Benchmark Results

Generated: 2026-03-17 20:44:34

## Environment

- Commit: aa7d859c5354df2117bd93cad8c93b96f7917890
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
| strata        | flat         | 3.519    | 4.087       | 4.208    | 274.5    |
| msgspec       | flat         | 4.385    | 4.643       | 6.204    | 277.0    |
| orjson        | flat         | 4.567    | 4.658       | 4.865    | 277.0    |
| ujson         | flat         | 6.631    | 7.047       | 10.049   | 277.0    |
| json (stdlib) | flat         | 10.748   | 10.856      | 11.586   | 277.0    |
| strata        | mixed        | 4.119    | 4.484       | 4.898    | 295.9    |
| msgspec       | mixed        | 5.473    | 6.584       | 6.742    | 298.1    |
| orjson        | mixed        | 6.538    | 6.913       | 9.185    | 298.1    |
| json (stdlib) | mixed        | 11.547   | 11.582      | 13.632   | 298.1    |
| ujson         | mixed        | 10.907   | 15.660      | 16.846   | 298.1    |
| strata        | nested       | 3.478    | 3.994       | 4.020    | 281.5    |
| orjson        | nested       | 5.877    | 6.103       | 6.803    | 283.9    |
| msgspec       | nested       | 5.584    | 6.725       | 6.878    | 283.9    |
| ujson         | nested       | 7.990    | 8.052       | 9.137    | 283.9    |
| json (stdlib) | nested       | 9.888    | 9.908       | 10.827   | 283.9    |
| strata        | users.json   | 36.511   | 39.510      | 42.121   | 74.2     |
| orjson        | users.json   | 33.255   | 41.409      | 52.201   | 94.1     |
| msgspec       | users.json   | 35.127   | 64.005      | 71.712   | 92.9     |
| ujson         | users.json   | 43.086   | 76.613      | 78.303   | 110.5    |
| json (stdlib) | users.json   | 72.257   | 96.391      | 102.536  | 99.2     |
| msgspec       | users.ndjson | 35.891   | 37.695      | 40.558   | 265.9    |
| orjson        | users.ndjson | 36.911   | 39.110      | 39.593   | 264.8    |
| strata        | users.ndjson | 35.511   | 41.998      | 44.500   | 256.8    |
| ujson         | users.ndjson | 55.258   | 58.940      | 60.185   | 264.9    |
| json (stdlib) | users.ndjson | 69.444   | 71.558      | 76.821   | 266.9    |
| orjson        | wide_arrays  | 12.578   | 17.507      | 19.780   | 243.7    |
| msgspec       | wide_arrays  | 15.377   | 19.799      | 19.861   | 243.7    |
| strata        | wide_arrays  | 22.207   | 23.045      | 23.066   | 233.5    |
| ujson         | wide_arrays  | 25.779   | 30.686      | 33.390   | 243.7    |
| json (stdlib) | wide_arrays  | 61.256   | 62.953      | 64.449   | 243.7    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 3.733    | 4.159       | 4.221    | 277.0    |
| msgspec       | flat         | 4.023    | 4.808       | 5.299    | 277.8    |
| orjson        | flat         | 4.052    | 4.953       | 5.079    | 277.0    |
| ujson         | flat         | 6.722    | 7.871       | 10.904   | 277.8    |
| json (stdlib) | flat         | 10.783   | 12.017      | 13.759   | 277.8    |
| orjson        | mixed        | 5.281    | 5.613       | 5.939    | 298.1    |
| strata        | mixed        | 5.817    | 6.571       | 6.672    | 298.1    |
| msgspec       | mixed        | 5.515    | 8.068       | 11.482   | 298.1    |
| ujson         | mixed        | 8.004    | 8.497       | 8.624    | 298.1    |
| json (stdlib) | mixed        | 11.917   | 12.675      | 17.673   | 298.6    |
| strata        | nested       | 3.630    | 4.414       | 4.483    | 283.9    |
| orjson        | nested       | 4.662    | 6.097       | 6.345    | 283.9    |
| ujson         | nested       | 8.102    | 9.316       | 10.526   | 283.9    |
| msgspec       | nested       | 4.707    | 9.509       | 9.616    | 283.9    |
| json (stdlib) | nested       | 8.907    | 9.804       | 15.478   | 283.9    |
| msgspec       | users.json   | 41.628   | 43.719      | 47.085   | 105.5    |
| strata        | users.json   | 40.099   | 53.415      | 65.133   | 100.5    |
| orjson        | users.json   | 53.677   | 58.349      | 59.394   | 100.5    |
| ujson         | users.json   | 44.122   | 62.733      | 83.739   | 123.2    |
| json (stdlib) | users.json   | 83.408   | 84.912      | 85.850   | 105.5    |
| orjson        | users.ndjson | 35.362   | 36.111      | 41.477   | 270.0    |
| msgspec       | users.ndjson | 39.869   | 40.559      | 40.570   | 271.0    |
| strata        | users.ndjson | 44.604   | 44.945      | 50.211   | 268.9    |
| ujson         | users.ndjson | 52.636   | 63.874      | 64.130   | 270.0    |
| json (stdlib) | users.ndjson | 66.434   | 68.118      | 73.196   | 272.0    |
| orjson        | wide_arrays  | 12.901   | 17.328      | 18.331   | 243.4    |
| strata        | wide_arrays  | 21.731   | 24.943      | 24.991   | 243.8    |
| msgspec       | wide_arrays  | 19.850   | 26.461      | 30.243   | 243.4    |
| ujson         | wide_arrays  | 30.843   | 33.820      | 38.933   | 243.4    |
| json (stdlib) | wide_arrays  | 65.214   | 65.291      | 70.914   | 243.4    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.988    | 0.996       | 1.109    | 280.1    |
| strata  | flat        | 1.154    | 1.156       | 1.170    | 277.8    |
| msgspec | flat        | 1.167    | 1.202       | 1.248    | 281.0    |
| ujson   | flat        | 4.822    | 4.889       | 5.838    | 281.4    |
| json    | flat        | 9.037    | 9.121       | 9.400    | 281.4    |
| msgspec | mixed       | 1.175    | 1.176       | 1.384    | 298.6    |
| orjson  | mixed       | 1.340    | 1.400       | 1.410    | 298.6    |
| strata  | mixed       | 1.525    | 1.602       | 2.268    | 298.6    |
| ujson   | mixed       | 5.098    | 5.108       | 5.864    | 298.6    |
| json    | mixed       | 12.598   | 13.213      | 14.800   | 298.6    |
| orjson  | nested      | 0.784    | 0.801       | 0.804    | 283.9    |
| msgspec | nested      | 0.786    | 0.808       | 0.810    | 283.9    |
| strata  | nested      | 1.098    | 1.098       | 1.119    | 283.9    |
| ujson   | nested      | 4.010    | 4.400       | 4.417    | 283.9    |
| json    | nested      | 5.273    | 5.324       | 6.402    | 221.3    |
| strata  | users.json  | 8.651    | 8.663       | 8.777    | 109.7    |
| orjson  | users.json  | 11.193   | 11.262      | 11.484   | 132.0    |
| msgspec | users.json  | 11.226   | 11.464      | 12.030   | 160.3    |
| ujson   | users.json  | 41.970   | 42.147      | 43.456   | 166.2    |
| json    | users.json  | 59.425   | 59.671      | 62.097   | 191.5    |
| msgspec | wide_arrays | 6.932    | 7.671       | 8.264    | 253.5    |
| orjson  | wide_arrays | 7.730    | 7.746       | 8.214    | 233.8    |
| strata  | wide_arrays | 8.403    | 10.038      | 10.970   | 243.4    |
| ujson   | wide_arrays | 29.548   | 31.432      | 32.079   | 255.5    |
| json    | wide_arrays | 81.617   | 82.915      | 85.304   | 276.4    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 1.327    | 1.433       | 1.527    | 281.4    |
| strata        | flat        | 1.469    | 1.507       | 1.684    | 281.4    |
| msgspec       | flat        | 1.664    | 1.728       | 1.743    | 281.4    |
| ujson         | flat        | 5.310    | 5.614       | 5.675    | 281.4    |
| json (stdlib) | flat        | 33.125   | 33.203      | 33.665   | 281.4    |
| orjson        | mixed       | 1.569    | 1.637       | 1.797    | 298.6    |
| msgspec       | mixed       | 1.643    | 1.644       | 2.189    | 298.7    |
| strata        | mixed       | 1.798    | 2.284       | 2.582    | 298.6    |
| ujson         | mixed       | 6.176    | 6.430       | 6.687    | 298.7    |
| json (stdlib) | mixed       | 39.640   | 40.403      | 46.937   | 298.7    |
| orjson        | nested      | 1.095    | 1.301       | 1.606    | 221.3    |
| msgspec       | nested      | 1.258    | 1.318       | 2.990    | 221.3    |
| strata        | nested      | 1.327    | 1.435       | 2.911    | 221.3    |
| ujson         | nested      | 4.961    | 5.125       | 5.160    | 221.3    |
| json (stdlib) | nested      | 59.083   | 59.714      | 61.824   | 221.3    |
| strata        | users.json  | 9.421    | 10.290      | 12.904   | 197.7    |
| msgspec       | users.json  | 11.695   | 11.764      | 12.099   | 244.1    |
| orjson        | users.json  | 12.263   | 13.352      | 13.837   | 220.5    |
| ujson         | users.json  | 43.799   | 44.041      | 46.490   | 221.8    |
| json (stdlib) | users.json  | 461.195  | 462.238     | 552.372  | 244.1    |
| orjson        | wide_arrays | 8.167    | 9.533       | 9.875    | 274.1    |
| msgspec       | wide_arrays | 9.349    | 9.549       | 10.549   | 295.9    |
| strata        | wide_arrays | 9.195    | 9.686       | 12.181   | 276.4    |
| ujson         | wide_arrays | 29.890   | 30.973      | 32.883   | 276.1    |
| json (stdlib) | wide_arrays | 197.544  | 198.045     | 208.166  | 295.9    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 18.219   | 2000    | 242.2    |
|                                | orjson+jmespath    | 32.475   | 2000    | 244.3    |
|                                | orjson+jsonpath-ng | 35.277   | 2000    | 246.5    |
| all item prices                | strata             | 23.819   | 100999  | 247.5    |
|                                | orjson+jmespath    | 113.530  | 2000    | 256.8    |
|                                | orjson+jsonpath-ng | 310.275  | 100999  | 263.9    |
| recursive price                | strata             | 25.806   | 100999  | 262.7    |
|                                | orjson+jsonpath-ng | 938.932  | 100999  | 266.1    |
| flat:$.records\[\*\].id        | strata             | 2.701    | 2000    | 281.5    |
|                                | orjson+jmespath    | 5.111    | 2000    | 281.5    |
|                                | orjson+jsonpath-ng | 7.468    | 2000    | 281.5    |
| flat:$.records\[0\]            | strata             | 2.463    | 1       | 281.5    |
|                                | orjson+jsonpath-ng | 4.581    | 1       | 281.5    |
|                                | orjson+jmespath    | 4.890    | 1       | 281.5    |
| nested:$.records\[\*\].id      | strata             | 2.083    | 2000    | 221.3    |
|                                | orjson+jmespath    | 8.718    | 2000    | 221.3    |
|                                | orjson+jsonpath-ng | 10.112   | 2000    | 221.3    |
| nested:$.records\[0\]          | strata             | 2.621    | 1       | 221.3    |
|                                | orjson+jmespath    | 6.538    | 1       | 221.3    |
|                                | orjson+jsonpath-ng | 7.784    | 1       | 221.3    |
| wide_arrays:$.records\[\*\].id | strata             | 15.774   | 2000    | 295.9    |
|                                | orjson+jmespath    | 16.937   | 2000    | 295.9    |
|                                | orjson+jsonpath-ng | 19.320   | 2000    | 295.9    |
| wide_arrays:$.records\[0\]     | orjson+jsonpath-ng | 14.592   | 1       | 295.9    |
|                                | strata             | 15.440   | 1       | 295.9    |
|                                | orjson+jmespath    | 16.260   | 1       | 295.9    |
| mixed:$.records\[\*\].id       | strata             | 2.854    | 2000    | 298.7    |
|                                | orjson+jmespath    | 5.110    | 2000    | 298.7    |
|                                | orjson+jsonpath-ng | 7.578    | 2000    | 298.7    |
| mixed:$.records\[0\]           | strata             | 2.633    | 1       | 298.7    |
|                                | orjson+jsonpath-ng | 4.502    | 1       | 298.7    |
|                                | orjson+jmespath    | 5.018    | 1       | 298.7    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.348    | 2000    | 288.3    |
|                                            | jmespath    | 1.084    | 2000    | 288.3    |
|                                            | jsonpath-ng | 18.034   | 2000    | 289.1    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 7.860    | 100999  | 289.1    |
|                                            | jmespath    | 71.734   | 2000    | 292.9    |
|                                            | jsonpath-ng | 255.773  | 100999  | 296.1    |
| $..price                                   | strata      | 9.349    | 100999  | 296.5    |
|                                            | jsonpath-ng | 950.470  | 100999  | 279.1    |
| $.users\[?(@.age>30)\]                     | strata      | 0.500    | 1606    | 277.1    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.645    | 6650    | 277.1    |
| flat:$.records\[\*\].id                    | strata      | 0.145    | 2000    | 281.5    |
|                                            | jmespath    | 0.679    | 2000    | 281.5    |
|                                            | jsonpath-ng | 4.251    | 2000    | 281.5    |
| flat:$.records\[0\]                        | jsonpath-ng | 0.008    | 1       | 281.5    |
|                                            | strata      | 0.013    | 1       | 281.5    |
|                                            | jmespath    | 0.017    | 1       | 281.5    |
| nested:$.records\[\*\].id                  | strata      | 0.166    | 2000    | 221.3    |
|                                            | jmespath    | 0.722    | 2000    | 221.3    |
|                                            | jsonpath-ng | 6.118    | 2000    | 221.3    |
| nested:$.records\[0\]                      | strata      | 0.009    | 1       | 221.3    |
|                                            | jmespath    | 0.011    | 1       | 221.3    |
|                                            | jsonpath-ng | 0.021    | 1       | 221.3    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.167    | 2000    | 295.9    |
|                                            | jmespath    | 0.682    | 2000    | 295.9    |
|                                            | jsonpath-ng | 7.542    | 2000    | 295.9    |
| wide_arrays:$.records\[0\]                 | strata      | 0.010    | 1       | 295.9    |
|                                            | jmespath    | 0.010    | 1       | 295.9    |
|                                            | jsonpath-ng | 0.018    | 1       | 295.9    |
| mixed:$.records\[\*\].id                   | strata      | 0.138    | 2000    | 298.7    |
|                                            | jmespath    | 0.720    | 2000    | 298.7    |
|                                            | jsonpath-ng | 5.052    | 2000    | 298.7    |
| mixed:$.records\[0\]                       | strata      | 0.011    | 1       | 298.7    |
|                                            | jsonpath-ng | 0.013    | 1       | 298.7    |
|                                            | jmespath    | 0.014    | 1       | 298.7    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#3** / 5              | 9.8% behind #1 (orjson)            |
| loads (NDJSON)      | **#1** / 5              | **1.1% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **3.8% faster** than #2 (msgspec)  |
| load (NDJSON file)  | **#3** / 5              | 26.1% behind #1 (orjson)           |
| dumps               | **#1** / 5              | **29.4% faster** than #2 (orjson)  |
| dump                | **#1** / 5              | **24.1% faster** than #2 (msgspec) |
| loads (flat)        | **#1** / 5              | **24.6% faster** than #2 (msgspec) |
| load (flat)         | **#1** / 5              | **7.8% faster** than #2 (msgspec)  |
| dumps (flat)        | **#2** / 5              | 16.8% behind #1 (orjson)           |
| dump (flat)         | **#2** / 5              | 10.7% behind #1 (orjson)           |
| loads (nested)      | **#1** / 5              | **60.6% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **28.4% faster** than #2 (orjson)  |
| dumps (nested)      | **#3** / 5              | 40.0% behind #1 (orjson)           |
| dump (nested)       | **#3** / 5              | 21.2% behind #1 (orjson)           |
| loads (wide_arrays) | **#3** / 5              | 76.5% behind #1 (orjson)           |
| load (wide_arrays)  | **#3** / 5              | 68.4% behind #1 (orjson)           |
| dumps (wide_arrays) | **#3** / 5              | 21.2% behind #1 (msgspec)          |
| dump (wide_arrays)  | **#2** / 5              | 12.6% behind #1 (orjson)           |
| loads (mixed)       | **#1** / 5              | **32.9% faster** than #2 (msgspec) |
| load (mixed)        | **#3** / 5              | 10.1% behind #1 (orjson)           |
| dumps (mixed)       | **#3** / 5              | 29.8% behind #1 (msgspec)          |
| dump (mixed)        | **#3** / 5              | 14.6% behind #1 (orjson)           |
| search (JSONPath)   | #1 in 10/11 queries     | -                                  |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
