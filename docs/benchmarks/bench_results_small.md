# Strata Benchmark Results

Generated: 2026-03-15 18:57:44

## Environment

- Commit: b45b1fa08982c9fae3371b7fcb950788159f1b6c
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
| orjson        | flat         | 0.544    | 0.649       | 0.705    | 62.5     |
| msgspec       | flat         | 0.640    | 0.790       | 0.884    | 62.5     |
| strata        | flat         | 0.780    | 0.893       | 0.947    | 61.3     |
| ujson         | flat         | 1.066    | 1.194       | 1.233    | 62.5     |
| json (stdlib) | flat         | 2.033    | 2.133       | 2.274    | 62.5     |
| orjson        | mixed        | 0.505    | 0.634       | 0.664    | 76.2     |
| msgspec       | mixed        | 0.589    | 0.737       | 0.740    | 76.2     |
| strata        | mixed        | 0.684    | 0.804       | 0.869    | 76.2     |
| ujson         | mixed        | 0.988    | 1.159       | 1.188    | 76.2     |
| json (stdlib) | mixed        | 2.067    | 2.188       | 2.190    | 76.2     |
| strata        | nested       | 0.693    | 0.781       | 0.871    | 62.5     |
| orjson        | nested       | 1.649    | 1.787       | 2.373    | 62.6     |
| ujson         | nested       | 1.833    | 2.199       | 2.720    | 62.6     |
| json (stdlib) | nested       | 2.879    | 2.888       | 3.733    | 62.6     |
| msgspec       | nested       | 2.713    | 3.613       | 4.306    | 62.6     |
| strata        | users.json   | 4.298    | 5.443       | 5.506    | 34.3     |
| orjson        | users.json   | 4.861    | 6.548       | 6.870    | 38.2     |
| msgspec       | users.json   | 4.693    | 6.683       | 6.734    | 41.4     |
| ujson         | users.json   | 5.818    | 8.439       | 9.420    | 41.3     |
| json (stdlib) | users.json   | 10.644   | 10.922      | 14.571   | 43.4     |
| strata        | users.ndjson | 4.266    | 5.043       | 5.197    | 57.0     |
| orjson        | users.ndjson | 4.590    | 6.183       | 6.715    | 58.0     |
| msgspec       | users.ndjson | 4.520    | 8.300       | 9.318    | 60.0     |
| ujson         | users.ndjson | 6.452    | 8.840       | 9.086    | 60.0     |
| json (stdlib) | users.ndjson | 10.598   | 11.503      | 13.094   | 60.0     |
| strata        | wide_arrays  | 2.477    | 3.248       | 3.363    | 64.9     |
| orjson        | wide_arrays  | 3.512    | 4.161       | 4.452    | 67.5     |
| msgspec       | wide_arrays  | 4.410    | 5.277       | 5.372    | 67.5     |
| ujson         | wide_arrays  | 7.829    | 8.281       | 9.886    | 67.5     |
| json (stdlib) | wide_arrays  | 13.487   | 14.494      | 15.302   | 68.9     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.731    | 0.740       | 0.769    | 62.5     |
| msgspec       | flat         | 0.725    | 0.840       | 0.923    | 62.5     |
| strata        | flat         | 0.946    | 1.001       | 1.051    | 62.5     |
| ujson         | flat         | 1.278    | 1.436       | 1.480    | 62.5     |
| json (stdlib) | flat         | 2.144    | 2.265       | 2.271    | 62.5     |
| orjson        | mixed        | 0.618    | 0.797       | 0.899    | 76.2     |
| strata        | mixed        | 0.787    | 0.945       | 1.011    | 76.2     |
| msgspec       | mixed        | 0.997    | 1.139       | 1.225    | 76.2     |
| ujson         | mixed        | 1.134    | 1.315       | 1.558    | 76.2     |
| json (stdlib) | mixed        | 2.229    | 2.539       | 2.575    | 76.2     |
| strata        | nested       | 0.787    | 0.896       | 0.939    | 62.6     |
| orjson        | nested       | 1.989    | 1.995       | 2.032    | 62.6     |
| msgspec       | nested       | 1.904    | 2.066       | 2.514    | 62.6     |
| ujson         | nested       | 2.014    | 2.400       | 2.567    | 62.6     |
| json (stdlib) | nested       | 2.886    | 2.954       | 3.007    | 62.6     |
| strata        | users.json   | 4.004    | 4.944       | 5.369    | 43.4     |
| orjson        | users.json   | 4.629    | 6.375       | 6.603    | 44.4     |
| msgspec       | users.json   | 4.910    | 6.483       | 10.111   | 47.4     |
| ujson         | users.json   | 6.224    | 9.593       | 9.706    | 47.4     |
| json (stdlib) | users.json   | 10.004   | 11.921      | 13.157   | 47.4     |
| strata        | users.ndjson | 4.428    | 5.433       | 5.649    | 60.0     |
| orjson        | users.ndjson | 4.795    | 6.667       | 6.840    | 60.0     |
| msgspec       | users.ndjson | 5.383    | 7.027       | 7.798    | 61.3     |
| json (stdlib) | users.ndjson | 10.078   | 12.053      | 12.302   | 61.3     |
| ujson         | users.ndjson | 7.249    | 12.874      | 13.553   | 61.3     |
| strata        | wide_arrays  | 2.714    | 4.247       | 4.366    | 68.9     |
| orjson        | wide_arrays  | 4.986    | 5.463       | 5.893    | 68.9     |
| msgspec       | wide_arrays  | 4.580    | 5.565       | 5.912    | 70.2     |
| ujson         | wide_arrays  | 7.386    | 8.079       | 8.270    | 68.9     |
| json (stdlib) | wide_arrays  | 14.281   | 17.652      | 18.335   | 71.5     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.249    | 0.256       | 0.283    | 62.5     |
| msgspec | flat        | 0.302    | 0.321       | 0.352    | 62.5     |
| strata  | flat        | 0.322    | 0.333       | 0.340    | 62.5     |
| ujson   | flat        | 1.236    | 1.250       | 1.290    | 62.5     |
| json    | flat        | 2.298    | 2.331       | 2.428    | 62.5     |
| orjson  | mixed       | 0.278    | 0.290       | 0.323    | 76.2     |
| msgspec | mixed       | 0.316    | 0.321       | 0.336    | 76.2     |
| strata  | mixed       | 0.360    | 0.382       | 0.388    | 76.2     |
| ujson   | mixed       | 1.307    | 1.328       | 1.336    | 76.2     |
| json    | mixed       | 3.024    | 3.059       | 3.076    | 76.2     |
| strata  | nested      | 0.187    | 0.188       | 0.191    | 62.6     |
| orjson  | nested      | 0.203    | 0.218       | 0.224    | 62.6     |
| msgspec | nested      | 0.236    | 0.253       | 0.275    | 62.6     |
| ujson   | nested      | 0.994    | 1.004       | 1.025    | 62.6     |
| json    | nested      | 1.343    | 1.349       | 1.424    | 62.6     |
| msgspec | users.json  | 1.552    | 1.555       | 1.563    | 52.3     |
| orjson  | users.json  | 1.547    | 1.558       | 1.710    | 51.7     |
| strata  | users.json  | 1.601    | 1.629       | 1.741    | 48.2     |
| ujson   | users.json  | 6.500    | 6.607       | 6.722    | 52.5     |
| json    | users.json  | 8.715    | 8.725       | 8.949    | 53.8     |
| orjson  | wide_arrays | 1.567    | 1.602       | 1.780    | 74.9     |
| msgspec | wide_arrays | 1.633    | 1.683       | 1.806    | 76.2     |
| strata  | wide_arrays | 1.977    | 1.990       | 2.009    | 72.7     |
| ujson   | wide_arrays | 7.145    | 7.208       | 7.248    | 76.2     |
| json    | wide_arrays | 19.318   | 19.725      | 20.076   | 76.2     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.581    | 0.603       | 0.778    | 62.5     |
| msgspec       | flat        | 0.528    | 0.664       | 0.671    | 62.5     |
| orjson        | flat        | 0.652    | 0.700       | 0.742    | 62.5     |
| ujson         | flat        | 1.404    | 1.410       | 1.426    | 62.5     |
| json (stdlib) | flat        | 8.385    | 8.435       | 8.491    | 62.5     |
| orjson        | mixed       | 0.487    | 0.495       | 0.513    | 76.2     |
| strata        | mixed       | 0.531    | 0.572       | 0.578    | 76.2     |
| msgspec       | mixed       | 0.553    | 0.805       | 0.830    | 76.2     |
| ujson         | mixed       | 1.554    | 1.591       | 1.668    | 76.2     |
| json (stdlib) | mixed       | 10.481   | 10.509      | 10.560   | 76.2     |
| strata        | nested      | 0.352    | 0.363       | 0.393    | 62.6     |
| msgspec       | nested      | 0.375    | 0.395       | 0.414    | 62.6     |
| orjson        | nested      | 0.388    | 0.433       | 0.477    | 62.6     |
| ujson         | nested      | 1.148    | 1.260       | 1.262    | 62.6     |
| json (stdlib) | nested      | 12.172   | 12.255      | 12.402   | 62.6     |
| orjson        | users.json  | 1.961    | 2.002       | 2.068    | 53.8     |
| strata        | users.json  | 2.097    | 2.161       | 4.541    | 53.8     |
| msgspec       | users.json  | 2.231    | 2.364       | 2.664    | 53.8     |
| ujson         | users.json  | 6.810    | 7.310       | 7.440    | 53.8     |
| json (stdlib) | users.json  | 67.887   | 68.419      | 68.975   | 53.8     |
| orjson        | wide_arrays | 1.879    | 2.001       | 2.066    | 76.2     |
| strata        | wide_arrays | 2.163    | 2.174       | 2.201    | 76.2     |
| msgspec       | wide_arrays | 2.948    | 3.054       | 3.109    | 76.2     |
| ujson         | wide_arrays | 7.720    | 8.060       | 8.130    | 76.2     |
| json (stdlib) | wide_arrays | 45.555   | 47.346      | 47.409   | 76.2     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 2.057    | 1000    | 53.9     |
|                                | orjson+jmespath    | 6.150    | 1000    | 53.9     |
|                                | orjson+jsonpath-ng | 6.580    | 1000    | 54.6     |
| all item prices                | strata             | 2.116    | 12046   | 54.6     |
|                                | orjson+jmespath    | 13.511   | 1000    | 54.6     |
|                                | orjson+jsonpath-ng | 36.319   | 12046   | 54.5     |
| recursive price                | strata             | 2.167    | 12046   | 54.1     |
|                                | orjson+jsonpath-ng | 127.060  | 12046   | 55.0     |
| flat:$.records\[\*\].id        | strata             | 0.438    | 500     | 62.5     |
|                                | orjson+jmespath    | 0.810    | 500     | 62.5     |
|                                | orjson+jsonpath-ng | 2.388    | 500     | 62.5     |
| flat:$.records\[0\]            | strata             | 0.446    | 1       | 62.5     |
|                                | orjson+jmespath    | 0.644    | 1       | 62.5     |
|                                | orjson+jsonpath-ng | 0.779    | 1       | 62.5     |
| nested:$.records\[\*\].id      | strata             | 0.447    | 500     | 62.6     |
|                                | orjson+jmespath    | 1.906    | 500     | 62.6     |
|                                | orjson+jsonpath-ng | 2.325    | 500     | 62.6     |
| nested:$.records\[0\]          | strata             | 0.408    | 1       | 62.6     |
|                                | orjson+jsonpath-ng | 1.749    | 1       | 62.6     |
|                                | orjson+jmespath    | 1.778    | 1       | 62.6     |
| wide_arrays:$.records\[\*\].id | strata             | 2.109    | 500     | 76.2     |
|                                | orjson+jmespath    | 4.285    | 500     | 76.2     |
|                                | orjson+jsonpath-ng | 4.797    | 500     | 76.2     |
| wide_arrays:$.records\[0\]     | strata             | 2.149    | 1       | 76.2     |
|                                | orjson+jsonpath-ng | 4.220    | 1       | 76.2     |
|                                | orjson+jmespath    | 4.686    | 1       | 76.2     |
| mixed:$.records\[\*\].id       | strata             | 0.466    | 500     | 76.2     |
|                                | orjson+jmespath    | 0.786    | 500     | 76.2     |
|                                | orjson+jsonpath-ng | 2.568    | 500     | 76.2     |
| mixed:$.records\[0\]           | strata             | 0.489    | 1       | 76.2     |
|                                | orjson+jmespath    | 0.639    | 1       | 76.2     |
|                                | orjson+jsonpath-ng | 0.668    | 1       | 76.2     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.073    | 1000    | 55.8     |
|                                            | jmespath    | 0.396    | 1000    | 55.9     |
|                                            | jsonpath-ng | 4.061    | 1000    | 55.9     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.916    | 12046   | 55.9     |
|                                            | jmespath    | 11.632   | 1000    | 55.9     |
|                                            | jsonpath-ng | 32.734   | 12046   | 58.5     |
| $..price                                   | strata      | 1.312    | 12046   | 56.5     |
|                                            | jsonpath-ng | 124.354  | 12046   | 59.5     |
| $.users\[?(@.age>30)\]                     | strata      | 0.120    | 794     | 56.9     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.470    | 1604    | 57.0     |
| flat:$.records\[\*\].id                    | strata      | 0.029    | 500     | 62.5     |
|                                            | jmespath    | 0.184    | 500     | 62.5     |
|                                            | jsonpath-ng | 1.650    | 500     | 62.5     |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 62.5     |
|                                            | jsonpath-ng | 0.008    | 1       | 62.5     |
|                                            | jmespath    | 0.010    | 1       | 62.5     |
| nested:$.records\[\*\].id                  | strata      | 0.032    | 500     | 62.6     |
|                                            | jmespath    | 0.168    | 500     | 62.6     |
|                                            | jsonpath-ng | 1.835    | 500     | 62.6     |
| nested:$.records\[0\]                      | jsonpath-ng | 0.007    | 1       | 62.6     |
|                                            | jmespath    | 0.008    | 1       | 62.6     |
|                                            | strata      | 0.010    | 1       | 62.6     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.030    | 500     | 76.2     |
|                                            | jmespath    | 0.186    | 500     | 76.2     |
|                                            | jsonpath-ng | 2.408    | 500     | 76.2     |
| wide_arrays:$.records\[0\]                 | strata      | 0.003    | 1       | 76.2     |
|                                            | jsonpath-ng | 0.010    | 1       | 76.2     |
|                                            | jmespath    | 0.012    | 1       | 76.2     |
| mixed:$.records\[\*\].id                   | strata      | 0.029    | 500     | 76.2     |
|                                            | jmespath    | 0.169    | 500     | 76.2     |
|                                            | jsonpath-ng | 1.856    | 500     | 76.2     |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 76.2     |
|                                            | jmespath    | 0.006    | 1       | 76.2     |
|                                            | jsonpath-ng | 0.007    | 1       | 76.2     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **9.2% faster** than #2 (msgspec)   |
| loads (NDJSON)      | **#1** / 5              | **6.0% faster** than #2 (msgspec)   |
| load (JSON file)    | **#1** / 5              | **15.6% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **8.3% faster** than #2 (orjson)    |
| dumps               | **#3** / 5              | 3.5% behind #1 (orjson)             |
| dump                | **#2** / 5              | 6.9% behind #1 (orjson)             |
| loads (flat)        | **#3** / 5              | 43.4% behind #1 (orjson)            |
| load (flat)         | **#3** / 5              | 30.5% behind #1 (msgspec)           |
| dumps (flat)        | **#3** / 5              | 29.3% behind #1 (orjson)            |
| dump (flat)         | **#2** / 5              | 9.9% behind #1 (msgspec)            |
| loads (nested)      | **#1** / 5              | **138.1% faster** than #2 (orjson)  |
| load (nested)       | **#1** / 5              | **142.0% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **8.3% faster** than #2 (orjson)    |
| dump (nested)       | **#1** / 5              | **6.6% faster** than #2 (msgspec)   |
| loads (wide_arrays) | **#1** / 5              | **41.8% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **68.8% faster** than #2 (msgspec)  |
| dumps (wide_arrays) | **#3** / 5              | 26.1% behind #1 (orjson)            |
| dump (wide_arrays)  | **#2** / 5              | 15.1% behind #1 (orjson)            |
| loads (mixed)       | **#3** / 5              | 35.5% behind #1 (orjson)            |
| load (mixed)        | **#2** / 5              | 27.4% behind #1 (orjson)            |
| dumps (mixed)       | **#3** / 5              | 29.5% behind #1 (orjson)            |
| dump (mixed)        | **#2** / 5              | 9.0% behind #1 (orjson)             |
| search (JSONPath)   | **#1** in 11/11 queries | avg **690% faster**                 |
| query (JSONPath)    | **#1** in 12/13 queries | -                                   |
