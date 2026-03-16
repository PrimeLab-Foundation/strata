# Strata Benchmark Results

Generated: 2026-03-16 21:41:12

## Environment

- Commit: c0e3b5a0f341ed8571ff3e143865c7dff53226aa
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
| orjson        | flat         | 0.558    | 0.664       | 0.692    | 61.2     |
| strata        | flat         | 0.609    | 0.725       | 0.782    | 60.6     |
| msgspec       | flat         | 0.686    | 0.762       | 0.772    | 61.2     |
| ujson         | flat         | 1.114    | 1.277       | 1.297    | 61.2     |
| json (stdlib) | flat         | 2.136    | 2.324       | 2.387    | 61.2     |
| orjson        | mixed        | 0.504    | 0.632       | 0.659    | 75.0     |
| strata        | mixed        | 0.633    | 0.784       | 0.789    | 75.0     |
| msgspec       | mixed        | 0.648    | 0.801       | 0.807    | 75.0     |
| ujson         | mixed        | 1.024    | 1.319       | 1.416    | 75.0     |
| json (stdlib) | mixed        | 2.205    | 2.473       | 2.496    | 75.0     |
| strata        | nested       | 0.509    | 0.622       | 0.631    | 60.3     |
| orjson        | nested       | 1.654    | 1.917       | 2.032    | 60.3     |
| msgspec       | nested       | 1.496    | 2.019       | 2.123    | 60.3     |
| ujson         | nested       | 2.006    | 2.202       | 2.867    | 60.3     |
| json (stdlib) | nested       | 2.393    | 2.572       | 2.751    | 60.3     |
| strata        | users.json   | 3.384    | 4.098       | 4.792    | 34.0     |
| orjson        | users.json   | 4.833    | 5.880       | 6.405    | 39.0     |
| msgspec       | users.json   | 4.532    | 6.392       | 6.542    | 41.1     |
| ujson         | users.json   | 6.039    | 9.012       | 9.143    | 41.0     |
| json (stdlib) | users.json   | 8.733    | 11.875      | 12.841   | 44.1     |
| strata        | users.ndjson | 3.321    | 4.339       | 4.687    | 56.0     |
| orjson        | users.ndjson | 4.713    | 6.375       | 6.943    | 57.0     |
| msgspec       | users.ndjson | 5.076    | 8.104       | 8.420    | 59.0     |
| ujson         | users.ndjson | 6.371    | 9.507       | 11.295   | 59.0     |
| json (stdlib) | users.ndjson | 10.138   | 11.348      | 11.742   | 59.0     |
| strata        | wide_arrays  | 3.027    | 3.533       | 3.612    | 63.7     |
| orjson        | wide_arrays  | 3.881    | 4.301       | 4.628    | 66.3     |
| msgspec       | wide_arrays  | 4.181    | 5.058       | 5.480    | 67.2     |
| ujson         | wide_arrays  | 6.895    | 7.920       | 10.548   | 67.2     |
| json (stdlib) | wide_arrays  | 13.503   | 14.633      | 16.761   | 68.6     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.720    | 0.824       | 0.920    | 61.2     |
| msgspec       | flat         | 0.712    | 0.855       | 0.895    | 61.2     |
| strata        | flat         | 0.774    | 0.888       | 1.081    | 61.2     |
| ujson         | flat         | 1.388    | 1.448       | 1.497    | 61.2     |
| json (stdlib) | flat         | 2.205    | 2.331       | 2.444    | 61.2     |
| orjson        | mixed        | 0.659    | 0.738       | 0.803    | 75.0     |
| msgspec       | mixed        | 0.794    | 0.832       | 0.957    | 75.0     |
| strata        | mixed        | 0.868    | 1.024       | 1.195    | 75.0     |
| ujson         | mixed        | 1.455    | 1.585       | 1.872    | 75.0     |
| json (stdlib) | mixed        | 2.196    | 2.319       | 2.438    | 75.0     |
| strata        | nested       | 0.619    | 0.725       | 0.729    | 60.3     |
| msgspec       | nested       | 1.589    | 1.844       | 1.883    | 60.3     |
| ujson         | nested       | 2.565    | 2.591       | 3.113    | 60.3     |
| orjson        | nested       | 1.744    | 2.963       | 3.578    | 60.3     |
| json (stdlib) | nested       | 3.228    | 4.317       | 4.856    | 60.3     |
| strata        | users.json   | 3.250    | 4.145       | 5.091    | 44.1     |
| orjson        | users.json   | 5.049    | 6.601       | 6.841    | 44.1     |
| msgspec       | users.json   | 5.289    | 6.677       | 6.794    | 48.1     |
| ujson         | users.json   | 6.420    | 8.943       | 9.515    | 48.1     |
| json (stdlib) | users.json   | 9.212    | 13.906      | 14.441   | 48.1     |
| strata        | users.ndjson | 3.537    | 4.443       | 4.659    | 59.0     |
| orjson        | users.ndjson | 5.034    | 6.218       | 6.220    | 59.0     |
| msgspec       | users.ndjson | 5.717    | 6.598       | 7.760    | 60.0     |
| ujson         | users.ndjson | 7.314    | 10.151      | 13.081   | 60.0     |
| json (stdlib) | users.ndjson | 10.086   | 11.215      | 11.437   | 60.0     |
| strata        | wide_arrays  | 3.263    | 3.969       | 4.107    | 68.6     |
| orjson        | wide_arrays  | 4.246    | 4.888       | 5.251    | 69.2     |
| msgspec       | wide_arrays  | 5.367    | 5.498       | 5.994    | 71.8     |
| ujson         | wide_arrays  | 7.601    | 8.143       | 8.530    | 71.8     |
| json (stdlib) | wide_arrays  | 13.980   | 15.211      | 17.193   | 71.8     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.253    | 0.264       | 0.265    | 61.2     |
| strata  | flat        | 0.287    | 0.295       | 0.303    | 61.2     |
| msgspec | flat        | 0.309    | 0.311       | 0.313    | 61.2     |
| ujson   | flat        | 1.222    | 1.243       | 1.380    | 61.2     |
| json    | flat        | 2.303    | 2.309       | 2.364    | 61.2     |
| orjson  | mixed       | 0.281    | 0.291       | 0.314    | 75.0     |
| msgspec | mixed       | 0.308    | 0.318       | 0.320    | 75.0     |
| strata  | mixed       | 0.320    | 0.326       | 0.368    | 75.0     |
| ujson   | mixed       | 1.364    | 1.435       | 1.566    | 75.0     |
| json    | mixed       | 3.022    | 3.063       | 3.325    | 75.0     |
| orjson  | nested      | 0.196    | 0.197       | 0.201    | 60.3     |
| strata  | nested      | 0.195    | 0.198       | 0.204    | 60.3     |
| msgspec | nested      | 0.215    | 0.217       | 0.229    | 60.3     |
| ujson   | nested      | 0.994    | 1.002       | 1.018    | 60.3     |
| json    | nested      | 1.366    | 1.366       | 1.377    | 60.3     |
| strata  | users.json  | 1.225    | 1.227       | 1.264    | 48.1     |
| orjson  | users.json  | 1.492    | 1.505       | 1.659    | 50.2     |
| msgspec | users.json  | 1.521    | 1.560       | 1.636    | 52.1     |
| ujson   | users.json  | 6.357    | 6.362       | 6.541    | 52.3     |
| json    | users.json  | 9.019    | 9.026       | 9.138    | 52.6     |
| orjson  | wide_arrays | 1.571    | 1.637       | 1.808    | 74.9     |
| msgspec | wide_arrays | 1.701    | 1.703       | 1.704    | 74.9     |
| strata  | wide_arrays | 1.824    | 1.846       | 1.894    | 73.0     |
| ujson   | wide_arrays | 7.414    | 7.471       | 7.640    | 74.9     |
| json    | wide_arrays | 19.801   | 20.182      | 20.916   | 75.0     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.430    | 0.442       | 0.448    | 61.2     |
| orjson        | flat        | 0.505    | 0.619       | 0.753    | 61.2     |
| msgspec       | flat        | 0.624    | 0.830       | 0.871    | 61.2     |
| ujson         | flat        | 1.531    | 1.570       | 1.612    | 61.2     |
| json (stdlib) | flat        | 8.568    | 8.612       | 8.658    | 61.2     |
| strata        | mixed       | 0.485    | 0.510       | 0.524    | 75.0     |
| orjson        | mixed       | 0.501    | 0.514       | 0.651    | 75.0     |
| msgspec       | mixed       | 0.539    | 0.582       | 0.711    | 75.0     |
| ujson         | mixed       | 1.563    | 1.599       | 1.843    | 75.0     |
| json (stdlib) | mixed       | 10.335   | 10.351      | 10.361   | 75.0     |
| strata        | nested      | 0.352    | 0.362       | 0.364    | 60.3     |
| orjson        | nested      | 0.367    | 0.396       | 0.480    | 60.3     |
| msgspec       | nested      | 0.450    | 0.483       | 0.501    | 60.3     |
| ujson         | nested      | 1.175    | 1.234       | 1.252    | 60.3     |
| json (stdlib) | nested      | 12.189   | 12.301      | 12.415   | 60.3     |
| strata        | users.json  | 1.448    | 1.505       | 1.620    | 52.7     |
| msgspec       | users.json  | 1.764    | 1.839       | 1.861    | 52.7     |
| orjson        | users.json  | 1.956    | 1.982       | 2.483    | 52.7     |
| ujson         | users.json  | 6.928    | 6.944       | 7.052    | 52.7     |
| json (stdlib) | users.json  | 67.737   | 68.505      | 70.161   | 52.7     |
| orjson        | wide_arrays | 1.922    | 1.985       | 2.103    | 75.0     |
| msgspec       | wide_arrays | 2.036    | 2.051       | 2.083    | 75.0     |
| strata        | wide_arrays | 2.145    | 2.247       | 2.273    | 75.0     |
| ujson         | wide_arrays | 7.537    | 7.673       | 7.783    | 75.0     |
| json (stdlib) | wide_arrays | 44.910   | 45.067      | 45.278   | 75.0     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.322    | 1000    | 52.7     |
|                                | orjson+jmespath    | 6.733    | 1000    | 52.8     |
|                                | orjson+jsonpath-ng | 7.008    | 1000    | 53.5     |
| all item prices                | strata             | 1.494    | 12046   | 53.5     |
|                                | orjson+jmespath    | 13.653   | 1000    | 53.5     |
|                                | orjson+jsonpath-ng | 36.001   | 12046   | 53.4     |
| recursive price                | strata             | 1.555    | 12046   | 53.1     |
|                                | orjson+jsonpath-ng | 129.631  | 12046   | 53.9     |
| flat:$.records\[\*\].id        | strata             | 0.375    | 500     | 61.3     |
|                                | orjson+jmespath    | 0.874    | 500     | 61.3     |
|                                | orjson+jsonpath-ng | 2.384    | 500     | 60.3     |
| flat:$.records\[0\]            | strata             | 0.359    | 1       | 60.3     |
|                                | orjson+jsonpath-ng | 0.670    | 1       | 60.3     |
|                                | orjson+jmespath    | 0.714    | 1       | 60.3     |
| nested:$.records\[\*\].id      | strata             | 0.273    | 500     | 60.3     |
|                                | orjson+jmespath    | 1.972    | 500     | 60.3     |
|                                | orjson+jsonpath-ng | 2.471    | 500     | 60.3     |
| nested:$.records\[0\]          | strata             | 0.269    | 1       | 60.3     |
|                                | orjson+jmespath    | 1.692    | 1       | 60.4     |
|                                | orjson+jsonpath-ng | 1.801    | 1       | 60.4     |
| wide_arrays:$.records\[\*\].id | strata             | 2.363    | 500     | 75.0     |
|                                | orjson+jmespath    | 4.598    | 500     | 75.0     |
|                                | orjson+jsonpath-ng | 5.172    | 500     | 75.0     |
| wide_arrays:$.records\[0\]     | strata             | 2.443    | 1       | 75.0     |
|                                | orjson+jsonpath-ng | 4.155    | 1       | 75.0     |
|                                | orjson+jmespath    | 4.251    | 1       | 75.0     |
| mixed:$.records\[\*\].id       | strata             | 0.435    | 500     | 75.0     |
|                                | orjson+jmespath    | 0.817    | 500     | 75.0     |
|                                | orjson+jsonpath-ng | 2.502    | 500     | 75.0     |
| mixed:$.records\[0\]           | strata             | 0.447    | 1       | 75.0     |
|                                | orjson+jmespath    | 0.632    | 1       | 75.0     |
|                                | orjson+jsonpath-ng | 0.634    | 1       | 75.0     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.089    | 1000    | 54.8     |
|                                            | jmespath    | 0.398    | 1000    | 54.8     |
|                                            | jsonpath-ng | 4.164    | 1000    | 54.8     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.920    | 12046   | 54.8     |
|                                            | jmespath    | 10.887   | 1000    | 54.8     |
|                                            | jsonpath-ng | 31.860   | 12046   | 57.5     |
| $..price                                   | strata      | 1.275    | 12046   | 55.5     |
|                                            | jsonpath-ng | 127.316  | 12046   | 58.5     |
| $.users\[?(@.age>30)\]                     | strata      | 0.201    | 794     | 55.8     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.496    | 1604    | 55.9     |
| flat:$.records\[\*\].id                    | strata      | 0.039    | 500     | 60.3     |
|                                            | jmespath    | 0.177    | 500     | 60.3     |
|                                            | jsonpath-ng | 1.696    | 500     | 60.3     |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 60.3     |
|                                            | jsonpath-ng | 0.007    | 1       | 60.3     |
|                                            | jmespath    | 0.007    | 1       | 60.3     |
| nested:$.records\[\*\].id                  | strata      | 0.028    | 500     | 60.4     |
|                                            | jmespath    | 0.172    | 500     | 60.4     |
|                                            | jsonpath-ng | 1.756    | 500     | 60.4     |
| nested:$.records\[0\]                      | strata      | 0.005    | 1       | 60.4     |
|                                            | jmespath    | 0.006    | 1       | 60.4     |
|                                            | jsonpath-ng | 0.009    | 1       | 60.4     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.035    | 500     | 75.0     |
|                                            | jmespath    | 0.184    | 500     | 75.0     |
|                                            | jsonpath-ng | 2.578    | 500     | 75.0     |
| wide_arrays:$.records\[0\]                 | strata      | 0.008    | 1       | 75.0     |
|                                            | jsonpath-ng | 0.008    | 1       | 75.0     |
|                                            | jmespath    | 0.012    | 1       | 75.0     |
| mixed:$.records\[\*\].id                   | strata      | 0.031    | 500     | 75.0     |
|                                            | jmespath    | 0.183    | 500     | 75.0     |
|                                            | jsonpath-ng | 1.686    | 500     | 75.0     |
| mixed:$.records\[0\]                       | strata      | 0.002    | 1       | 75.0     |
|                                            | jsonpath-ng | 0.007    | 1       | 75.0     |
|                                            | jmespath    | 0.008    | 1       | 75.0     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **33.9% faster** than #2 (msgspec)  |
| loads (NDJSON)      | **#1** / 5              | **41.9% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **55.4% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **42.3% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **21.8% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **21.8% faster** than #2 (msgspec)  |
| loads (flat)        | **#2** / 5              | 9.1% behind #1 (orjson)             |
| load (flat)         | **#3** / 5              | 8.7% behind #1 (msgspec)            |
| dumps (flat)        | **#2** / 5              | 13.4% behind #1 (orjson)            |
| dump (flat)         | **#1** / 5              | **17.5% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **193.6% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **156.7% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **0.7% faster** than #2 (orjson)    |
| dump (nested)       | **#1** / 5              | **4.1% faster** than #2 (orjson)    |
| loads (wide_arrays) | **#1** / 5              | **28.2% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **30.1% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#3** / 5              | 16.1% behind #1 (orjson)            |
| dump (wide_arrays)  | **#3** / 5              | 11.6% behind #1 (orjson)            |
| loads (mixed)       | **#2** / 5              | 25.6% behind #1 (orjson)            |
| load (mixed)        | **#3** / 5              | 31.6% behind #1 (orjson)            |
| dumps (mixed)       | **#3** / 5              | 14.1% behind #1 (orjson)            |
| dump (mixed)        | **#1** / 5              | **3.4% faster** than #2 (orjson)    |
| search (JSONPath)   | **#1** in 11/11 queries | avg **1011% faster**                |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
