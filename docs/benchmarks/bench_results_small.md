# Strata Benchmark Results

Generated: 2026-03-17 15:51:04

## Environment

- Commit: 33d6835349dd084a915ba2ba437233c105a366f8
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson
- Random schemas: flat, nested, wide_arrays, mixed (500 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.647    | 0.650       | 0.689    | 60.9     |
| strata        | flat         | 0.717    | 0.801       | 0.826    | 60.2     |
| msgspec       | flat         | 0.607    | 0.928       | 0.981    | 60.9     |
| ujson         | flat         | 1.109    | 1.375       | 1.675    | 60.9     |
| json (stdlib) | flat         | 2.024    | 2.165       | 2.200    | 60.9     |
| orjson        | mixed        | 0.518    | 0.674       | 0.886    | 76.8     |
| msgspec       | mixed        | 0.658    | 0.822       | 0.972    | 76.8     |
| strata        | mixed        | 0.742    | 0.948       | 1.010    | 76.8     |
| ujson         | mixed        | 1.066    | 1.177       | 1.938    | 76.8     |
| json (stdlib) | mixed        | 2.086    | 2.242       | 2.431    | 76.8     |
| strata        | nested       | 0.644    | 0.750       | 0.801    | 61.7     |
| orjson        | nested       | 1.658    | 1.875       | 2.060    | 61.7     |
| ujson         | nested       | 1.887    | 2.210       | 4.499    | 61.7     |
| msgspec       | nested       | 1.950    | 2.546       | 2.648    | 61.7     |
| json (stdlib) | nested       | 3.011    | 3.154       | 3.167    | 61.7     |
| strata        | users.json   | 3.926    | 5.424       | 5.457    | 34.0     |
| orjson        | users.json   | 5.638    | 6.233       | 7.125    | 38.0     |
| msgspec       | users.json   | 4.580    | 6.359       | 6.999    | 41.2     |
| json (stdlib) | users.json   | 8.781    | 10.947      | 11.107   | 43.9     |
| ujson         | users.json   | 9.684    | 12.665      | 14.397   | 41.0     |
| strata        | users.ndjson | 4.586    | 5.292       | 5.434    | 56.6     |
| orjson        | users.ndjson | 5.567    | 6.949       | 7.112    | 57.6     |
| msgspec       | users.ndjson | 4.712    | 8.458       | 10.191   | 58.6     |
| ujson         | users.ndjson | 7.840    | 10.475      | 14.092   | 58.6     |
| json (stdlib) | users.ndjson | 10.435   | 11.728      | 12.441   | 58.6     |
| strata        | wide_arrays  | 3.994    | 4.066       | 4.550    | 64.7     |
| orjson        | wide_arrays  | 3.772    | 4.469       | 5.018    | 67.3     |
| msgspec       | wide_arrays  | 4.449    | 6.780       | 7.442    | 67.4     |
| ujson         | wide_arrays  | 6.918    | 8.539       | 8.920    | 67.4     |
| json (stdlib) | wide_arrays  | 13.784   | 15.119      | 17.654   | 68.7     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.675    | 0.809       | 0.864    | 60.9     |
| strata        | flat         | 0.841    | 0.891       | 0.948    | 60.9     |
| msgspec       | flat         | 0.895    | 0.902       | 1.065    | 60.9     |
| ujson         | flat         | 1.375    | 1.417       | 1.596    | 60.9     |
| json (stdlib) | flat         | 2.320    | 2.336       | 3.103    | 61.5     |
| orjson        | mixed        | 0.663    | 0.821       | 0.844    | 76.8     |
| msgspec       | mixed        | 0.795    | 0.834       | 0.916    | 76.8     |
| strata        | mixed        | 0.946    | 1.050       | 1.454    | 76.8     |
| ujson         | mixed        | 1.229    | 1.325       | 1.339    | 76.8     |
| json (stdlib) | mixed        | 2.243    | 2.369       | 3.011    | 76.8     |
| strata        | nested       | 0.760    | 0.960       | 1.022    | 61.7     |
| msgspec       | nested       | 1.973    | 2.187       | 2.685    | 61.7     |
| ujson         | nested       | 2.202    | 2.459       | 2.606    | 61.7     |
| orjson        | nested       | 1.873    | 2.950       | 3.832    | 61.7     |
| json (stdlib) | nested       | 2.953    | 3.091       | 3.254    | 61.7     |
| strata        | users.json   | 3.812    | 4.663       | 4.685    | 43.9     |
| orjson        | users.json   | 6.182    | 7.421       | 12.351   | 44.9     |
| ujson         | users.json   | 7.998    | 9.733       | 10.089   | 48.1     |
| json (stdlib) | users.json   | 10.275   | 12.293      | 12.691   | 48.1     |
| msgspec       | users.json   | 7.843    | 13.254      | 24.409   | 48.1     |
| strata        | users.ndjson | 4.252    | 6.245       | 6.370    | 58.6     |
| msgspec       | users.ndjson | 4.865    | 6.517       | 6.518    | 59.7     |
| orjson        | users.ndjson | 6.453    | 6.779       | 6.928    | 58.6     |
| ujson         | users.ndjson | 9.505    | 10.290      | 11.949   | 59.7     |
| json (stdlib) | users.ndjson | 10.516   | 12.064      | 16.551   | 59.7     |
| strata        | wide_arrays  | 3.680    | 4.178       | 4.525    | 68.7     |
| orjson        | wide_arrays  | 3.930    | 5.402       | 6.864    | 69.7     |
| msgspec       | wide_arrays  | 5.248    | 7.320       | 9.783    | 72.4     |
| ujson         | wide_arrays  | 8.249    | 11.024      | 12.286   | 72.4     |
| json (stdlib) | wide_arrays  | 14.011   | 15.685      | 15.777   | 72.4     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.235    | 0.245       | 0.262    | 61.5     |
| orjson  | flat        | 0.251    | 0.253       | 0.258    | 61.6     |
| msgspec | flat        | 0.318    | 0.380       | 0.420    | 61.6     |
| ujson   | flat        | 1.253    | 1.274       | 1.327    | 61.6     |
| json    | flat        | 2.325    | 2.330       | 2.353    | 61.6     |
| strata  | mixed       | 0.263    | 0.265       | 0.270    | 76.8     |
| msgspec | mixed       | 0.344    | 0.361       | 0.392    | 76.8     |
| orjson  | mixed       | 0.308    | 0.377       | 0.410    | 76.8     |
| ujson   | mixed       | 1.307    | 1.369       | 1.394    | 76.8     |
| json    | mixed       | 2.979    | 3.005       | 3.138    | 76.8     |
| strata  | nested      | 0.191    | 0.193       | 0.202    | 61.7     |
| orjson  | nested      | 0.204    | 0.216       | 0.223    | 61.7     |
| msgspec | nested      | 0.252    | 0.261       | 0.262    | 61.7     |
| ujson   | nested      | 1.024    | 1.031       | 1.206    | 61.7     |
| json    | nested      | 1.374    | 1.424       | 1.495    | 61.7     |
| strata  | users.json  | 1.090    | 1.100       | 1.134    | 48.1     |
| msgspec | users.json  | 1.484    | 1.571       | 1.747    | 52.1     |
| orjson  | users.json  | 1.555    | 1.621       | 1.775    | 50.2     |
| ujson   | users.json  | 6.406    | 6.530       | 6.656    | 53.0     |
| json    | users.json  | 9.116    | 9.122       | 9.239    | 53.3     |
| strata  | wide_arrays | 1.398    | 1.465       | 1.530    | 73.6     |
| orjson  | wide_arrays | 1.663    | 1.677       | 1.873    | 75.5     |
| msgspec | wide_arrays | 1.679    | 1.730       | 1.940    | 75.5     |
| ujson   | wide_arrays | 7.266    | 7.273       | 7.499    | 76.8     |
| json    | wide_arrays | 20.435   | 21.159      | 21.207   | 76.8     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 0.401    | 0.417       | 0.465    | 61.6     |
| strata        | flat        | 0.429    | 0.449       | 0.485    | 61.6     |
| msgspec       | flat        | 0.588    | 0.632       | 0.678    | 61.6     |
| ujson         | flat        | 1.610    | 1.654       | 1.857    | 61.6     |
| json (stdlib) | flat        | 8.497    | 8.726       | 8.945    | 61.6     |
| strata        | mixed       | 0.404    | 0.447       | 0.499    | 76.8     |
| orjson        | mixed       | 0.440    | 0.453       | 0.462    | 76.8     |
| msgspec       | mixed       | 0.495    | 0.525       | 0.572    | 76.8     |
| ujson         | mixed       | 1.546    | 1.813       | 2.001    | 76.8     |
| json (stdlib) | mixed       | 10.111   | 10.214      | 10.343   | 76.8     |
| msgspec       | nested      | 0.349    | 0.351       | 0.367    | 61.7     |
| orjson        | nested      | 0.381    | 0.417       | 0.431    | 61.7     |
| strata        | nested      | 0.407    | 0.480       | 0.710    | 61.7     |
| ujson         | nested      | 1.195    | 1.206       | 1.324    | 61.7     |
| json (stdlib) | nested      | 12.458   | 12.742      | 12.875   | 61.7     |
| strata        | users.json  | 1.385    | 1.412       | 1.485    | 53.3     |
| orjson        | users.json  | 1.822    | 2.038       | 2.144    | 53.3     |
| msgspec       | users.json  | 1.816    | 2.288       | 2.549    | 53.3     |
| ujson         | users.json  | 6.793    | 6.839       | 7.226    | 53.3     |
| json (stdlib) | users.json  | 70.531   | 72.704      | 73.327   | 53.3     |
| strata        | wide_arrays | 1.737    | 1.766       | 1.870    | 76.8     |
| orjson        | wide_arrays | 2.043    | 2.051       | 2.053    | 76.8     |
| msgspec       | wide_arrays | 2.105    | 2.699       | 2.779    | 76.8     |
| ujson         | wide_arrays | 7.697    | 7.968       | 7.971    | 76.8     |
| json (stdlib) | wide_arrays | 46.165   | 46.746      | 47.068   | 76.8     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 2.113    | 1000    | 53.4     |
|                                | orjson+jmespath    | 6.183    | 1000    | 53.5     |
|                                | orjson+jsonpath-ng | 6.978    | 1000    | 54.0     |
| all item prices                | strata             | 2.207    | 12046   | 54.1     |
|                                | orjson+jmespath    | 15.254   | 1000    | 54.1     |
|                                | orjson+jsonpath-ng | 37.008   | 12046   | 54.0     |
| recursive price                | strata             | 2.115    | 12046   | 53.6     |
|                                | orjson+jsonpath-ng | 134.537  | 12046   | 54.4     |
| flat:$.records\[\*\].id        | strata             | 0.494    | 500     | 61.6     |
|                                | orjson+jmespath    | 0.835    | 500     | 61.6     |
|                                | orjson+jsonpath-ng | 2.446    | 500     | 61.6     |
| flat:$.records\[0\]            | strata             | 0.493    | 1       | 61.6     |
|                                | orjson+jmespath    | 0.664    | 1       | 61.6     |
|                                | orjson+jsonpath-ng | 0.695    | 1       | 61.6     |
| nested:$.records\[\*\].id      | strata             | 0.381    | 500     | 61.7     |
|                                | orjson+jmespath    | 1.788    | 500     | 61.7     |
|                                | orjson+jsonpath-ng | 2.759    | 500     | 61.7     |
| nested:$.records\[0\]          | strata             | 0.468    | 1       | 61.7     |
|                                | orjson+jsonpath-ng | 1.694    | 1       | 61.7     |
|                                | orjson+jmespath    | 1.919    | 1       | 61.7     |
| wide_arrays:$.records\[\*\].id | strata             | 2.740    | 500     | 76.8     |
|                                | orjson+jmespath    | 4.314    | 500     | 76.8     |
|                                | orjson+jsonpath-ng | 4.747    | 500     | 76.8     |
| wide_arrays:$.records\[0\]     | strata             | 2.799    | 1       | 76.8     |
|                                | orjson+jmespath    | 4.168    | 1       | 76.8     |
|                                | orjson+jsonpath-ng | 4.184    | 1       | 76.8     |
| mixed:$.records\[\*\].id       | strata             | 0.592    | 500     | 76.8     |
|                                | orjson+jmespath    | 0.834    | 500     | 76.8     |
|                                | orjson+jsonpath-ng | 2.461    | 500     | 76.8     |
| mixed:$.records\[0\]           | strata             | 0.529    | 1       | 76.8     |
|                                | orjson+jsonpath-ng | 0.634    | 1       | 76.8     |
|                                | orjson+jmespath    | 0.654    | 1       | 76.8     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.083    | 1000    | 55.3     |
|                                            | jmespath    | 0.420    | 1000    | 55.3     |
|                                            | jsonpath-ng | 4.712    | 1000    | 55.3     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.914    | 12046   | 55.3     |
|                                            | jmespath    | 11.112   | 1000    | 55.3     |
|                                            | jsonpath-ng | 32.339   | 12046   | 58.0     |
| $..price                                   | strata      | 1.270    | 12046   | 58.0     |
|                                            | jsonpath-ng | 131.026  | 12046   | 60.2     |
| $.users\[?(@.age>30)\]                     | strata      | 0.135    | 794     | 56.6     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.462    | 1604    | 56.6     |
| flat:$.records\[\*\].id                    | strata      | 0.034    | 500     | 61.6     |
|                                            | jmespath    | 0.179    | 500     | 61.6     |
|                                            | jsonpath-ng | 1.706    | 500     | 61.6     |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 61.6     |
|                                            | jmespath    | 0.005    | 1       | 61.6     |
|                                            | jsonpath-ng | 0.006    | 1       | 61.6     |
| nested:$.records\[\*\].id                  | strata      | 0.028    | 500     | 61.7     |
|                                            | jmespath    | 0.206    | 500     | 61.7     |
|                                            | jsonpath-ng | 2.030    | 500     | 61.7     |
| nested:$.records\[0\]                      | strata      | 0.002    | 1       | 61.7     |
|                                            | jmespath    | 0.006    | 1       | 61.7     |
|                                            | jsonpath-ng | 0.007    | 1       | 61.7     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.030    | 500     | 76.8     |
|                                            | jmespath    | 0.180    | 500     | 76.8     |
|                                            | jsonpath-ng | 2.611    | 500     | 76.8     |
| wide_arrays:$.records\[0\]                 | strata      | 0.004    | 1       | 76.8     |
|                                            | jmespath    | 0.008    | 1       | 76.8     |
|                                            | jsonpath-ng | 0.009    | 1       | 76.8     |
| mixed:$.records\[\*\].id                   | strata      | 0.038    | 500     | 76.8     |
|                                            | jmespath    | 0.172    | 500     | 76.8     |
|                                            | jsonpath-ng | 1.893    | 500     | 76.8     |
| mixed:$.records\[0\]                       | jmespath    | 0.006    | 1       | 76.8     |
|                                            | strata      | 0.007    | 1       | 76.8     |
|                                            | jsonpath-ng | 0.013    | 1       | 76.8     |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **16.7% faster** than #2 (msgspec) |
| loads (NDJSON)      | **#1** / 5              | **2.7% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **62.2% faster** than #2 (orjson)  |
| load (NDJSON file)  | **#1** / 5              | **14.4% faster** than #2 (msgspec) |
| dumps               | **#1** / 5              | **36.2% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **31.1% faster** than #2 (msgspec) |
| loads (flat)        | **#3** / 5              | 18.1% behind #1 (msgspec)          |
| load (flat)         | **#2** / 5              | 24.6% behind #1 (orjson)           |
| dumps (flat)        | **#1** / 5              | **6.4% faster** than #2 (orjson)   |
| dump (flat)         | **#2** / 5              | 7.0% behind #1 (orjson)            |
| loads (nested)      | **#1** / 5              | **157.5% faster** than #2 (orjson) |
| load (nested)       | **#1** / 5              | **146.5% faster** than #2 (orjson) |
| dumps (nested)      | **#1** / 5              | **6.5% faster** than #2 (orjson)   |
| dump (nested)       | **#3** / 5              | 16.4% behind #1 (msgspec)          |
| loads (wide_arrays) | **#2** / 5              | 5.9% behind #1 (orjson)            |
| load (wide_arrays)  | **#1** / 5              | **6.8% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **19.0% faster** than #2 (orjson)  |
| dump (wide_arrays)  | **#1** / 5              | **17.6% faster** than #2 (orjson)  |
| loads (mixed)       | **#3** / 5              | 43.3% behind #1 (orjson)           |
| load (mixed)        | **#3** / 5              | 42.5% behind #1 (orjson)           |
| dumps (mixed)       | **#1** / 5              | **16.8% faster** than #2 (orjson)  |
| dump (mixed)        | **#1** / 5              | **8.9% faster** than #2 (orjson)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **723% faster**                |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
