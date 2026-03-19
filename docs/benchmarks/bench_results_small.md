# Strata Benchmark Results

Generated: 2026-03-19 02:11:23

## Environment

- Commit: dfd6aa590c1dd14d42395de2cde1a4a3e49aa958
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
| orjson        | flat         | 0.606    | 0.730       | 0.738    | 61.3     |
| msgspec       | flat         | 0.636    | 0.771       | 0.787    | 61.3     |
| strata        | flat         | 0.745    | 0.875       | 0.896    | 60.7     |
| ujson         | flat         | 1.143    | 1.305       | 1.351    | 61.3     |
| json (stdlib) | flat         | 2.285    | 2.329       | 2.427    | 61.3     |
| orjson        | mixed        | 0.508    | 0.647       | 0.680    | 75.1     |
| msgspec       | mixed        | 0.606    | 0.805       | 0.934    | 75.1     |
| strata        | mixed        | 0.769    | 0.897       | 1.029    | 75.1     |
| ujson         | mixed        | 1.164    | 1.257       | 1.356    | 75.1     |
| json (stdlib) | mixed        | 2.205    | 2.383       | 2.538    | 75.1     |
| strata        | nested       | 0.718    | 0.826       | 0.833    | 61.4     |
| msgspec       | nested       | 2.274    | 2.355       | 2.511    | 61.4     |
| orjson        | nested       | 1.912    | 2.636       | 3.211    | 61.4     |
| json (stdlib) | nested       | 2.976    | 3.166       | 3.168    | 61.4     |
| ujson         | nested       | 3.015    | 3.188       | 3.828    | 61.4     |
| strata        | users.json   | 4.218    | 5.035       | 5.268    | 34.3     |
| msgspec       | users.json   | 6.474    | 7.273       | 7.778    | 41.4     |
| orjson        | users.json   | 5.272    | 7.657       | 7.785    | 38.2     |
| ujson         | users.json   | 7.052    | 10.333      | 10.738   | 41.2     |
| json (stdlib) | users.json   | 9.935    | 11.095      | 12.114   | 44.3     |
| strata        | users.ndjson | 4.174    | 5.123       | 5.759    | 56.7     |
| orjson        | users.ndjson | 5.169    | 6.740       | 7.824    | 57.7     |
| msgspec       | users.ndjson | 5.231    | 8.126       | 8.492    | 58.7     |
| ujson         | users.ndjson | 6.864    | 8.785       | 9.888    | 58.7     |
| json (stdlib) | users.ndjson | 11.820   | 13.824      | 15.580   | 58.7     |
| strata        | wide_arrays  | 3.435    | 3.674       | 10.207   | 64.5     |
| orjson        | wide_arrays  | 3.904    | 4.628       | 4.705    | 67.1     |
| msgspec       | wide_arrays  | 4.840    | 6.065       | 6.835    | 68.0     |
| ujson         | wide_arrays  | 8.052    | 8.255       | 8.274    | 68.0     |
| json (stdlib) | wide_arrays  | 13.907   | 16.265      | 18.120   | 69.3     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.765    | 0.939       | 1.159    | 61.3     |
| msgspec       | flat         | 0.891    | 0.951       | 1.088    | 61.3     |
| strata        | flat         | 0.883    | 0.952       | 0.987    | 61.3     |
| ujson         | flat         | 1.505    | 1.647       | 1.801    | 61.3     |
| json (stdlib) | flat         | 2.477    | 2.577       | 2.838    | 61.3     |
| orjson        | mixed        | 0.861    | 0.868       | 0.992    | 75.1     |
| msgspec       | mixed        | 0.907    | 0.982       | 1.043    | 75.1     |
| strata        | mixed        | 1.114    | 1.145       | 1.194    | 75.1     |
| ujson         | mixed        | 1.354    | 1.495       | 1.579    | 75.1     |
| json (stdlib) | mixed        | 2.623    | 2.843       | 2.915    | 75.1     |
| strata        | nested       | 0.929    | 1.044       | 1.154    | 61.4     |
| orjson        | nested       | 2.343    | 2.579       | 3.104    | 61.4     |
| msgspec       | nested       | 2.503    | 2.707       | 3.293    | 61.4     |
| ujson         | nested       | 2.575    | 3.083       | 3.110    | 61.4     |
| json (stdlib) | nested       | 2.787    | 3.310       | 3.803    | 61.4     |
| strata        | users.json   | 4.321    | 4.919       | 4.999    | 44.3     |
| orjson        | users.json   | 4.663    | 6.717       | 6.860    | 44.3     |
| msgspec       | users.json   | 5.044    | 6.856       | 8.639    | 47.4     |
| ujson         | users.json   | 7.384    | 9.232       | 9.881    | 47.4     |
| json (stdlib) | users.json   | 10.495   | 14.965      | 18.291   | 47.4     |
| strata        | users.ndjson | 4.979    | 5.955       | 6.080    | 58.7     |
| msgspec       | users.ndjson | 5.642    | 6.586       | 7.646    | 60.1     |
| orjson        | users.ndjson | 5.687    | 7.579       | 7.870    | 58.7     |
| ujson         | users.ndjson | 8.267    | 10.017      | 11.195   | 60.1     |
| json (stdlib) | users.ndjson | 10.077   | 12.335      | 13.121   | 60.1     |
| strata        | wide_arrays  | 3.374    | 4.481       | 4.579    | 69.3     |
| orjson        | wide_arrays  | 4.116    | 4.985       | 5.062    | 69.3     |
| msgspec       | wide_arrays  | 5.572    | 5.994       | 7.145    | 70.7     |
| ujson         | wide_arrays  | 8.332    | 9.029       | 9.082    | 70.7     |
| json (stdlib) | wide_arrays  | 15.864   | 15.976      | 16.739   | 70.7     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.259    | 0.271       | 0.273    | 61.3     |
| orjson  | flat        | 0.286    | 0.288       | 0.291    | 61.3     |
| msgspec | flat        | 0.335    | 0.340       | 0.359    | 61.3     |
| ujson   | flat        | 1.285    | 1.400       | 1.418    | 61.3     |
| json    | flat        | 2.639    | 2.644       | 2.674    | 61.3     |
| orjson  | mixed       | 0.325    | 0.330       | 0.352    | 75.1     |
| strata  | mixed       | 0.354    | 0.362       | 0.389    | 75.1     |
| msgspec | mixed       | 0.375    | 0.378       | 0.495    | 75.1     |
| ujson   | mixed       | 1.491    | 1.525       | 1.545    | 75.1     |
| json    | mixed       | 3.476    | 3.478       | 3.508    | 75.1     |
| orjson  | nested      | 0.223    | 0.229       | 0.253    | 61.4     |
| msgspec | nested      | 0.231    | 0.235       | 0.236    | 61.4     |
| strata  | nested      | 0.232    | 0.237       | 0.240    | 61.4     |
| ujson   | nested      | 1.123    | 1.178       | 1.545    | 61.4     |
| json    | nested      | 1.484    | 1.522       | 1.673    | 61.4     |
| strata  | users.json  | 1.472    | 1.475       | 1.570    | 48.3     |
| orjson  | users.json  | 1.666    | 1.684       | 1.752    | 51.1     |
| msgspec | users.json  | 1.694    | 1.784       | 1.887    | 52.8     |
| ujson   | users.json  | 6.700    | 6.976       | 7.316    | 52.9     |
| json    | users.json  | 10.086   | 10.115      | 10.276   | 53.4     |
| orjson  | wide_arrays | 1.618    | 1.639       | 2.114    | 73.8     |
| strata  | wide_arrays | 1.611    | 1.642       | 1.649    | 72.0     |
| msgspec | wide_arrays | 1.714    | 1.718       | 1.748    | 73.8     |
| ujson   | wide_arrays | 7.490    | 7.510       | 7.652    | 73.8     |
| json    | wide_arrays | 20.588   | 20.956      | 22.503   | 73.8     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| msgspec       | flat        | 0.577    | 0.583       | 0.616    | 61.3     |
| strata        | flat        | 0.565    | 0.588       | 0.597    | 61.3     |
| orjson        | flat        | 0.651    | 0.667       | 0.681    | 61.3     |
| ujson         | flat        | 1.654    | 1.731       | 1.864    | 61.3     |
| json (stdlib) | flat        | 9.491    | 9.493       | 9.547    | 61.3     |
| strata        | mixed       | 0.478    | 0.505       | 0.544    | 75.1     |
| orjson        | mixed       | 0.517    | 0.552       | 0.558    | 75.1     |
| msgspec       | mixed       | 0.586    | 0.621       | 0.667    | 75.1     |
| ujson         | mixed       | 1.746    | 1.783       | 1.854    | 75.1     |
| json (stdlib) | mixed       | 10.487   | 10.588      | 11.380   | 75.1     |
| strata        | nested      | 0.360    | 0.366       | 0.371    | 61.4     |
| orjson        | nested      | 0.421    | 0.439       | 0.456    | 61.4     |
| msgspec       | nested      | 0.384    | 0.516       | 0.520    | 61.4     |
| ujson         | nested      | 1.207    | 1.225       | 1.246    | 61.4     |
| json (stdlib) | nested      | 12.589   | 12.608      | 12.763   | 61.4     |
| strata        | users.json  | 1.789    | 1.823       | 1.874    | 53.4     |
| orjson        | users.json  | 1.890    | 1.917       | 1.926    | 53.4     |
| msgspec       | users.json  | 1.843    | 2.056       | 2.067    | 53.4     |
| ujson         | users.json  | 7.243    | 7.445       | 7.766    | 53.4     |
| json (stdlib) | users.json  | 71.797   | 74.642      | 75.671   | 53.4     |
| strata        | wide_arrays | 2.209    | 2.308       | 2.346    | 73.8     |
| orjson        | wide_arrays | 2.558    | 2.580       | 2.721    | 73.8     |
| msgspec       | wide_arrays | 2.548    | 2.604       | 2.611    | 75.1     |
| ujson         | wide_arrays | 8.548    | 8.665       | 8.738    | 75.1     |
| json (stdlib) | wide_arrays | 45.449   | 47.183      | 50.477   | 75.1     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 2.008    | 1000    | 53.5     |
|                                | orjson+jmespath    | 5.617    | 1000    | 53.7     |
|                                | orjson+jsonpath-ng | 6.894    | 1000    | 54.3     |
| all item prices                | strata             | 2.359    | 12046   | 54.3     |
|                                | orjson+jmespath    | 15.721   | 1000    | 54.3     |
|                                | orjson+jsonpath-ng | 39.130   | 12046   | 54.2     |
| recursive price                | strata             | 2.249    | 12046   | 53.9     |
|                                | orjson+jsonpath-ng | 128.603  | 12046   | 54.7     |
| flat:$.records\[\*\].id        | strata             | 0.560    | 500     | 61.4     |
|                                | orjson+jmespath    | 0.923    | 500     | 61.4     |
|                                | orjson+jsonpath-ng | 2.567    | 500     | 61.4     |
| flat:$.records\[0\]            | strata             | 0.538    | 1       | 61.4     |
|                                | orjson+jmespath    | 0.704    | 1       | 61.4     |
|                                | orjson+jsonpath-ng | 0.757    | 1       | 61.4     |
| nested:$.records\[\*\].id      | strata             | 0.407    | 500     | 61.4     |
|                                | orjson+jmespath    | 2.376    | 500     | 61.4     |
|                                | orjson+jsonpath-ng | 3.452    | 500     | 61.4     |
| nested:$.records\[0\]          | strata             | 0.470    | 1       | 61.4     |
|                                | orjson+jmespath    | 2.249    | 1       | 61.4     |
|                                | orjson+jsonpath-ng | 2.478    | 1       | 61.4     |
| wide_arrays:$.records\[\*\].id | strata             | 2.413    | 500     | 75.1     |
|                                | orjson+jmespath    | 4.989    | 500     | 75.1     |
|                                | orjson+jsonpath-ng | 5.719    | 500     | 75.1     |
| wide_arrays:$.records\[0\]     | strata             | 2.531    | 1       | 75.1     |
|                                | orjson+jmespath    | 4.477    | 1       | 75.1     |
|                                | orjson+jsonpath-ng | 4.561    | 1       | 75.1     |
| mixed:$.records\[\*\].id       | strata             | 0.543    | 500     | 75.1     |
|                                | orjson+jmespath    | 0.847    | 500     | 75.1     |
|                                | orjson+jsonpath-ng | 2.540    | 500     | 75.1     |
| mixed:$.records\[0\]           | strata             | 0.551    | 1       | 75.1     |
|                                | orjson+jsonpath-ng | 0.714    | 1       | 75.1     |
|                                | orjson+jmespath    | 0.714    | 1       | 75.1     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.093    | 1000    | 54.5     |
|                                            | jmespath    | 0.428    | 1000    | 54.5     |
|                                            | jsonpath-ng | 4.136    | 1000    | 54.5     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.925    | 12046   | 54.5     |
|                                            | jmespath    | 10.863   | 1000    | 54.5     |
|                                            | jsonpath-ng | 34.791   | 12046   | 57.2     |
| $..price                                   | strata      | 1.321    | 12046   | 56.3     |
|                                            | jsonpath-ng | 130.516  | 12046   | 58.3     |
| $.users\[?(@.age>30)\]                     | strata      | 0.121    | 794     | 56.6     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.526    | 1604    | 56.7     |
| flat:$.records\[\*\].id                    | strata      | 0.034    | 500     | 61.4     |
|                                            | jmespath    | 0.180    | 500     | 61.4     |
|                                            | jsonpath-ng | 1.657    | 500     | 61.4     |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 61.4     |
|                                            | jsonpath-ng | 0.008    | 1       | 61.4     |
|                                            | jmespath    | 0.014    | 1       | 61.4     |
| nested:$.records\[\*\].id                  | strata      | 0.035    | 500     | 61.4     |
|                                            | jmespath    | 0.222    | 500     | 61.4     |
|                                            | jsonpath-ng | 2.287    | 500     | 61.4     |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 61.5     |
|                                            | jmespath    | 0.007    | 1       | 61.5     |
|                                            | jsonpath-ng | 0.020    | 1       | 61.5     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.032    | 500     | 75.1     |
|                                            | jmespath    | 0.208    | 500     | 75.1     |
|                                            | jsonpath-ng | 2.420    | 500     | 75.1     |
| wide_arrays:$.records\[0\]                 | strata      | 0.003    | 1       | 75.1     |
|                                            | jmespath    | 0.006    | 1       | 75.1     |
|                                            | jsonpath-ng | 0.010    | 1       | 75.1     |
| mixed:$.records\[\*\].id                   | strata      | 0.033    | 500     | 75.1     |
|                                            | jmespath    | 0.208    | 500     | 75.1     |
|                                            | jsonpath-ng | 2.033    | 500     | 75.1     |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 75.1     |
|                                            | jmespath    | 0.007    | 1       | 75.1     |
|                                            | jsonpath-ng | 0.009    | 1       | 75.1     |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **25.0% faster** than #2 (orjson)  |
| loads (NDJSON)      | **#1** / 5              | **23.9% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **7.9% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **13.3% faster** than #2 (msgspec) |
| dumps               | **#1** / 5              | **13.2% faster** than #2 (orjson)  |
| dump                | **#1** / 5              | **3.0% faster** than #2 (msgspec)  |
| loads (flat)        | **#3** / 5              | 22.9% behind #1 (orjson)           |
| load (flat)         | **#2** / 5              | 15.5% behind #1 (orjson)           |
| dumps (flat)        | **#1** / 5              | **10.3% faster** than #2 (orjson)  |
| dump (flat)         | **#1** / 5              | **2.0% faster** than #2 (msgspec)  |
| loads (nested)      | **#1** / 5              | **166.4% faster** than #2 (orjson) |
| load (nested)       | **#1** / 5              | **152.3% faster** than #2 (orjson) |
| dumps (nested)      | **#3** / 5              | 3.8% behind #1 (orjson)            |
| dump (nested)       | **#1** / 5              | **6.7% faster** than #2 (msgspec)  |
| loads (wide_arrays) | **#1** / 5              | **13.6% faster** than #2 (orjson)  |
| load (wide_arrays)  | **#1** / 5              | **22.0% faster** than #2 (orjson)  |
| dumps (wide_arrays) | **#1** / 5              | **0.5% faster** than #2 (orjson)   |
| dump (wide_arrays)  | **#1** / 5              | **15.4% faster** than #2 (msgspec) |
| loads (mixed)       | **#3** / 5              | 51.3% behind #1 (orjson)           |
| load (mixed)        | **#3** / 5              | 29.4% behind #1 (orjson)           |
| dumps (mixed)       | **#2** / 5              | 8.9% behind #1 (orjson)            |
| dump (mixed)        | **#1** / 5              | **8.2% faster** than #2 (orjson)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **690% faster**                |
| query (JSONPath)    | **#1** in 13/13 queries | -                                  |
