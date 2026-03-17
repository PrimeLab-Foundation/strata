# Strata Benchmark Results

Generated: 2026-03-17 19:44:23

## Environment

- Commit: aa7d859c5354df2117bd93cad8c93b96f7917890
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 5
- Warmup: 1
- Datasets: benchmarks/data/generated/users.json, benchmarks/data/generated/users.ndjson
- Random schemas: flat, nested, wide_arrays, mixed (500 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.572    | 0.663       | 0.671    | 204.2    |
| msgspec       | flat         | 0.613    | 0.721       | 0.758    | 204.2    |
| strata        | flat         | 0.715    | 0.801       | 0.917    | 204.2    |
| ujson         | flat         | 1.091    | 1.232       | 1.407    | 204.2    |
| json (stdlib) | flat         | 2.122    | 2.139       | 7.496    | 204.2    |
| orjson        | mixed        | 0.480    | 0.617       | 0.642    | 218.1    |
| msgspec       | mixed        | 0.589    | 0.701       | 0.739    | 218.1    |
| strata        | mixed        | 0.682    | 0.818       | 0.869    | 218.1    |
| ujson         | mixed        | 0.995    | 1.163       | 1.179    | 218.1    |
| json (stdlib) | mixed        | 2.276    | 2.358       | 2.775    | 218.1    |
| strata        | nested       | 0.662    | 0.785       | 0.849    | 204.3    |
| msgspec       | nested       | 1.525    | 1.714       | 1.767    | 204.9    |
| orjson        | nested       | 1.571    | 1.785       | 1.927    | 204.9    |
| ujson         | nested       | 1.975    | 2.094       | 2.367    | 204.9    |
| json (stdlib) | nested       | 2.456    | 2.586       | 2.644    | 204.9    |
| strata        | users.json   | 22.165   | 26.654      | 29.130   | 65.1     |
| orjson        | users.json   | 23.504   | 34.608      | 40.312   | 81.1     |
| msgspec       | users.json   | 25.946   | 38.893      | 51.309   | 80.9     |
| ujson         | users.json   | 32.899   | 51.623      | 69.071   | 94.9     |
| json (stdlib) | users.json   | 48.718   | 61.158      | 64.400   | 85.9     |
| strata        | users.ndjson | 23.058   | 26.819      | 27.886   | 241.4    |
| orjson        | users.ndjson | 31.854   | 32.941      | 39.438   | 248.9    |
| msgspec       | users.ndjson | 33.100   | 33.694      | 37.998   | 252.0    |
| ujson         | users.ndjson | 41.049   | 46.579      | 53.431   | 252.0    |
| json (stdlib) | users.ndjson | 57.870   | 58.180      | 66.524   | 254.0    |
| strata        | wide_arrays  | 2.918    | 3.608       | 4.499    | 206.4    |
| orjson        | wide_arrays  | 3.799    | 4.935       | 6.405    | 209.0    |
| msgspec       | wide_arrays  | 4.365    | 5.911       | 10.542   | 209.8    |
| ujson         | wide_arrays  | 7.023    | 8.217       | 8.732    | 209.8    |
| json (stdlib) | wide_arrays  | 14.694   | 15.264      | 16.427   | 210.3    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| msgspec       | flat         | 0.709    | 0.801       | 0.850    | 204.2    |
| orjson        | flat         | 0.846    | 0.934       | 1.131    | 204.2    |
| strata        | flat         | 0.753    | 1.063       | 1.142    | 204.2    |
| ujson         | flat         | 1.400    | 1.468       | 2.388    | 204.2    |
| json (stdlib) | flat         | 2.207    | 2.338       | 2.518    | 204.2    |
| orjson        | mixed        | 0.674    | 0.765       | 1.041    | 218.1    |
| msgspec       | mixed        | 0.663    | 0.883       | 0.941    | 218.1    |
| strata        | mixed        | 0.862    | 0.984       | 1.138    | 218.1    |
| ujson         | mixed        | 1.139    | 1.459       | 2.272    | 218.1    |
| json (stdlib) | mixed        | 2.176    | 2.426       | 2.758    | 218.1    |
| strata        | nested       | 0.695    | 0.826       | 0.959    | 204.9    |
| orjson        | nested       | 1.910    | 2.077       | 3.067    | 204.9    |
| msgspec       | nested       | 1.645    | 2.091       | 2.247    | 204.9    |
| ujson         | nested       | 2.359    | 2.821       | 3.548    | 204.9    |
| json (stdlib) | nested       | 2.715    | 2.918       | 3.040    | 204.9    |
| strata        | users.json   | 20.950   | 28.126      | 30.680   | 86.4     |
| msgspec       | users.json   | 33.550   | 35.958      | 51.973   | 98.6     |
| orjson        | users.json   | 27.245   | 40.679      | 51.331   | 86.5     |
| ujson         | users.json   | 43.990   | 49.466      | 60.791   | 105.2    |
| json (stdlib) | users.json   | 47.540   | 59.269      | 63.910   | 100.6    |
| strata        | users.ndjson | 22.236   | 26.744      | 28.588   | 254.0    |
| orjson        | users.ndjson | 31.025   | 33.264      | 37.238   | 199.2    |
| msgspec       | users.ndjson | 32.661   | 38.374      | 42.109   | 201.8    |
| ujson         | users.ndjson | 44.519   | 47.744      | 57.812   | 203.8    |
| json (stdlib) | users.ndjson | 58.246   | 61.168      | 71.586   | 203.8    |
| strata        | wide_arrays  | 3.347    | 3.860       | 3.896    | 210.4    |
| orjson        | wide_arrays  | 4.611    | 5.019       | 8.221    | 210.4    |
| msgspec       | wide_arrays  | 5.519    | 5.861       | 10.735   | 213.1    |
| ujson         | wide_arrays  | 8.378    | 8.502       | 12.425   | 213.1    |
| json (stdlib) | wide_arrays  | 15.353   | 16.018      | 17.347   | 213.1    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.236    | 0.238       | 0.257    | 204.2    |
| orjson  | flat        | 0.247    | 0.249       | 0.254    | 204.2    |
| msgspec | flat        | 0.299    | 0.304       | 0.341    | 204.2    |
| ujson   | flat        | 1.222    | 1.245       | 1.264    | 204.2    |
| json    | flat        | 2.285    | 2.299       | 2.320    | 204.2    |
| strata  | mixed       | 0.265    | 0.273       | 0.278    | 218.1    |
| orjson  | mixed       | 0.278    | 0.291       | 0.322    | 218.1    |
| msgspec | mixed       | 0.307    | 0.318       | 0.329    | 218.1    |
| ujson   | mixed       | 1.321    | 1.332       | 1.381    | 218.1    |
| json    | mixed       | 2.994    | 3.018       | 3.177    | 218.1    |
| strata  | nested      | 0.181    | 0.183       | 0.212    | 204.9    |
| orjson  | nested      | 0.192    | 0.205       | 0.207    | 204.9    |
| msgspec | nested      | 0.200    | 0.208       | 0.220    | 204.9    |
| ujson   | nested      | 0.974    | 0.981       | 0.990    | 204.9    |
| json    | nested      | 1.337    | 1.432       | 2.257    | 204.9    |
| strata  | users.json  | 5.593    | 5.663       | 7.918    | 105.1    |
| orjson  | users.json  | 8.260    | 8.299       | 8.704    | 133.1    |
| msgspec | users.json  | 8.733    | 8.965       | 10.093   | 162.1    |
| ujson   | users.json  | 33.042   | 33.878      | 36.332   | 166.9    |
| json    | users.json  | 47.040   | 47.199      | 48.735   | 197.8    |
| strata  | wide_arrays | 1.372    | 1.396       | 1.401    | 213.1    |
| orjson  | wide_arrays | 1.549    | 1.558       | 1.834    | 215.0    |
| msgspec | wide_arrays | 1.651    | 1.758       | 1.959    | 215.1    |
| ujson   | wide_arrays | 7.212    | 7.340       | 7.678    | 218.0    |
| json    | wide_arrays | 20.089   | 20.252      | 26.679   | 218.1    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 0.478    | 0.485       | 0.561    | 204.2    |
| msgspec       | flat        | 0.490    | 0.506       | 0.680    | 204.2    |
| strata        | flat        | 0.417    | 0.588       | 0.640    | 204.2    |
| ujson         | flat        | 1.393    | 1.443       | 1.507    | 204.2    |
| json (stdlib) | flat        | 8.258    | 8.317       | 8.756    | 204.2    |
| strata        | mixed       | 0.389    | 0.424       | 0.451    | 218.1    |
| orjson        | mixed       | 0.436    | 0.477       | 0.733    | 218.1    |
| msgspec       | mixed       | 0.500    | 0.568       | 0.639    | 218.1    |
| ujson         | mixed       | 1.477    | 1.533       | 1.997    | 218.1    |
| json (stdlib) | mixed       | 10.055   | 10.347      | 11.436   | 218.1    |
| strata        | nested      | 0.329    | 0.388       | 0.552    | 204.9    |
| msgspec       | nested      | 0.342    | 0.438       | 0.723    | 204.9    |
| orjson        | nested      | 0.394    | 0.590       | 0.653    | 204.9    |
| ujson         | nested      | 1.140    | 1.161       | 1.654    | 204.9    |
| json (stdlib) | nested      | 12.193   | 12.301      | 12.449   | 204.9    |
| strata        | users.json  | 6.311    | 6.381       | 7.357    | 202.8    |
| msgspec       | users.json  | 8.890    | 9.063       | 9.499    | 252.3    |
| orjson        | users.json  | 8.824    | 9.510       | 11.285   | 230.0    |
| ujson         | users.json  | 35.083   | 35.853      | 41.711   | 230.1    |
| json (stdlib) | users.json  | 352.017  | 358.059     | 358.586  | 252.3    |
| strata        | wide_arrays | 1.689    | 1.707       | 1.839    | 218.1    |
| orjson        | wide_arrays | 1.875    | 1.890       | 1.953    | 218.1    |
| msgspec       | wide_arrays | 2.033    | 2.355       | 4.852    | 218.1    |
| ujson         | wide_arrays | 7.666    | 8.238       | 9.660    | 218.1    |
| json (stdlib) | wide_arrays | 44.913   | 45.543      | 46.306   | 218.1    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 9.462    | 5000    | 253.4    |
|                                | orjson+jmespath    | 28.696   | 5000    | 253.6    |
|                                | orjson+jsonpath-ng | 35.621   | 5000    | 254.4    |
| all item prices                | strata             | 10.777   | 62331   | 254.4    |
|                                | orjson+jmespath    | 69.316   | 5000    | 254.4    |
|                                | orjson+jsonpath-ng | 225.185  | 62331   | 257.5    |
| recursive price                | strata             | 10.862   | 62331   | 257.1    |
|                                | orjson+jsonpath-ng | 693.146  | 62331   | 260.1    |
| flat:$.records\[\*\].id        | strata             | 0.451    | 500     | 204.2    |
|                                | orjson+jmespath    | 0.831    | 500     | 204.2    |
|                                | orjson+jsonpath-ng | 2.544    | 500     | 204.2    |
| flat:$.records\[0\]            | strata             | 0.496    | 1       | 204.3    |
|                                | orjson+jsonpath-ng | 0.658    | 1       | 204.3    |
|                                | orjson+jmespath    | 0.664    | 1       | 204.3    |
| nested:$.records\[\*\].id      | strata             | 0.342    | 500     | 204.9    |
|                                | orjson+jmespath    | 1.787    | 500     | 204.9    |
|                                | orjson+jsonpath-ng | 2.325    | 500     | 204.9    |
| nested:$.records\[0\]          | strata             | 0.341    | 1       | 204.9    |
|                                | orjson+jmespath    | 1.688    | 1       | 204.9    |
|                                | orjson+jsonpath-ng | 1.815    | 1       | 204.9    |
| wide_arrays:$.records\[\*\].id | strata             | 2.408    | 500     | 218.1    |
|                                | orjson+jmespath    | 4.697    | 500     | 218.1    |
|                                | orjson+jsonpath-ng | 4.868    | 500     | 218.1    |
| wide_arrays:$.records\[0\]     | strata             | 2.299    | 1       | 218.1    |
|                                | orjson+jsonpath-ng | 4.164    | 1       | 218.1    |
|                                | orjson+jmespath    | 4.655    | 1       | 218.1    |
| mixed:$.records\[\*\].id       | strata             | 0.497    | 500     | 218.1    |
|                                | orjson+jmespath    | 0.812    | 500     | 218.1    |
|                                | orjson+jsonpath-ng | 2.382    | 500     | 218.1    |
| mixed:$.records\[0\]           | strata             | 0.460    | 1       | 218.1    |
|                                | orjson+jsonpath-ng | 0.640    | 1       | 218.1    |
|                                | orjson+jmespath    | 0.659    | 1       | 218.1    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.724    | 5000    | 276.2    |
|                                            | jmespath    | 2.326    | 5000    | 276.2    |
|                                            | jsonpath-ng | 22.063   | 5000    | 278.2    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 4.747    | 62331   | 277.8    |
|                                            | jmespath    | 56.290   | 5000    | 282.8    |
|                                            | jsonpath-ng | 218.279  | 62331   | 256.4    |
| $..price                                   | strata      | 6.516    | 62331   | 254.3    |
|                                            | jsonpath-ng | 716.993  | 62331   | 256.5    |
| $.users\[?(@.age>30)\]                     | strata      | 0.812    | 3999    | 253.5    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 8.952    | 8402    | 253.5    |
| flat:$.records\[\*\].id                    | strata      | 0.032    | 500     | 204.3    |
|                                            | jmespath    | 0.171    | 500     | 204.3    |
|                                            | jsonpath-ng | 1.567    | 500     | 204.3    |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 204.3    |
|                                            | jmespath    | 0.005    | 1       | 204.3    |
|                                            | jsonpath-ng | 0.009    | 1       | 204.3    |
| nested:$.records\[\*\].id                  | strata      | 0.033    | 500     | 204.9    |
|                                            | jmespath    | 0.174    | 500     | 204.9    |
|                                            | jsonpath-ng | 1.750    | 500     | 204.9    |
| nested:$.records\[0\]                      | strata      | 0.002    | 1       | 204.9    |
|                                            | jmespath    | 0.005    | 1       | 204.9    |
|                                            | jsonpath-ng | 0.007    | 1       | 204.9    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.034    | 500     | 218.1    |
|                                            | jmespath    | 0.180    | 500     | 218.1    |
|                                            | jsonpath-ng | 2.370    | 500     | 218.1    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 218.1    |
|                                            | jmespath    | 0.007    | 1       | 218.1    |
|                                            | jsonpath-ng | 0.008    | 1       | 218.1    |
| mixed:$.records\[\*\].id                   | strata      | 0.036    | 500     | 218.1    |
|                                            | jmespath    | 0.187    | 500     | 218.1    |
|                                            | jsonpath-ng | 1.880    | 500     | 218.1    |
| mixed:$.records\[0\]                       | strata      | 0.002    | 1       | 218.1    |
|                                            | jmespath    | 0.006    | 1       | 218.1    |
|                                            | jsonpath-ng | 0.007    | 1       | 218.1    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **6.0% faster** than #2 (orjson)    |
| loads (NDJSON)      | **#1** / 5              | **38.1% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **30.0% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **39.5% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **47.7% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **39.8% faster** than #2 (orjson)   |
| loads (flat)        | **#3** / 5              | 25.0% behind #1 (orjson)            |
| load (flat)         | **#2** / 5              | 6.2% behind #1 (msgspec)            |
| dumps (flat)        | **#1** / 5              | **4.5% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **14.7% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **130.2% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **136.6% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **5.7% faster** than #2 (orjson)    |
| dump (nested)       | **#1** / 5              | **4.0% faster** than #2 (msgspec)   |
| loads (wide_arrays) | **#1** / 5              | **30.2% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **37.8% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **12.9% faster** than #2 (orjson)   |
| dump (wide_arrays)  | **#1** / 5              | **11.0% faster** than #2 (orjson)   |
| loads (mixed)       | **#3** / 5              | 42.1% behind #1 (orjson)            |
| load (mixed)        | **#3** / 5              | 29.9% behind #1 (msgspec)           |
| dumps (mixed)       | **#1** / 5              | **4.7% faster** than #2 (orjson)    |
| dump (mixed)        | **#1** / 5              | **12.0% faster** than #2 (orjson)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **749% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
