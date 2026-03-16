# Strata Benchmark Results

Generated: 2026-03-16 17:18:01

## Environment

- Commit: efd00fdc01e0951561d325921e36a0c1697c3ea1
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
| orjson        | flat         | 0.563    | 0.647       | 0.670    | 60.2     |
| strata        | flat         | 0.597    | 0.730       | 1.008    | 59.6     |
| msgspec       | flat         | 0.609    | 0.760       | 0.824    | 60.2     |
| ujson         | flat         | 1.156    | 1.328       | 1.342    | 60.2     |
| json (stdlib) | flat         | 2.029    | 2.121       | 2.128    | 60.2     |
| orjson        | mixed        | 0.459    | 0.620       | 0.650    | 74.3     |
| msgspec       | mixed        | 0.598    | 0.712       | 0.731    | 74.3     |
| strata        | mixed        | 0.667    | 0.822       | 0.866    | 74.3     |
| ujson         | mixed        | 1.075    | 1.144       | 1.159    | 74.3     |
| json (stdlib) | mixed        | 2.156    | 2.198       | 2.303    | 74.3     |
| strata        | nested       | 0.561    | 0.611       | 0.634    | 60.3     |
| msgspec       | nested       | 1.482    | 1.699       | 1.734    | 60.3     |
| orjson        | nested       | 1.762    | 1.793       | 2.055    | 60.3     |
| ujson         | nested       | 1.733    | 2.040       | 2.069    | 60.3     |
| json (stdlib) | nested       | 2.404    | 2.582       | 2.687    | 60.3     |
| strata        | users.json   | 3.539    | 4.117       | 4.348    | 34.2     |
| orjson        | users.json   | 4.331    | 6.374       | 7.165    | 38.1     |
| msgspec       | users.json   | 5.441    | 7.062       | 7.371    | 41.3     |
| ujson         | users.json   | 6.661    | 9.597       | 9.637    | 41.2     |
| json (stdlib) | users.json   | 9.552    | 11.317      | 12.188   | 44.2     |
| strata        | users.ndjson | 3.403    | 4.267       | 4.280    | 56.0     |
| msgspec       | users.ndjson | 4.664    | 6.083       | 6.771    | 58.0     |
| orjson        | users.ndjson | 4.774    | 6.323       | 6.421    | 57.0     |
| ujson         | users.ndjson | 6.285    | 8.527       | 8.669    | 58.0     |
| json (stdlib) | users.ndjson | 9.449    | 10.922      | 11.214   | 58.0     |
| strata        | wide_arrays  | 2.911    | 3.529       | 3.903    | 62.5     |
| orjson        | wide_arrays  | 3.536    | 4.188       | 4.258    | 65.1     |
| msgspec       | wide_arrays  | 4.446    | 4.987       | 5.217    | 65.1     |
| ujson         | wide_arrays  | 6.830    | 7.252       | 7.945    | 65.1     |
| json (stdlib) | wide_arrays  | 13.835   | 14.189      | 14.255   | 66.7     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.753    | 0.777       | 0.815    | 60.2     |
| strata        | flat         | 0.791    | 0.897       | 1.234    | 60.2     |
| msgspec       | flat         | 0.851    | 0.909       | 1.517    | 60.2     |
| ujson         | flat         | 1.208    | 1.412       | 1.601    | 60.2     |
| json (stdlib) | flat         | 2.315    | 2.342       | 2.652    | 60.2     |
| orjson        | mixed        | 0.634    | 0.782       | 0.785    | 74.3     |
| msgspec       | mixed        | 0.673    | 0.791       | 0.798    | 74.3     |
| strata        | mixed        | 0.789    | 0.884       | 0.981    | 74.3     |
| ujson         | mixed        | 1.197    | 1.329       | 1.395    | 74.3     |
| json (stdlib) | mixed        | 2.163    | 2.306       | 2.308    | 74.3     |
| strata        | nested       | 0.637    | 0.715       | 0.855    | 60.3     |
| msgspec       | nested       | 1.871    | 2.010       | 2.290    | 60.3     |
| orjson        | nested       | 1.799    | 2.029       | 2.162    | 60.3     |
| ujson         | nested       | 1.997    | 2.182       | 2.470    | 60.3     |
| json (stdlib) | nested       | 2.519    | 2.701       | 3.172    | 60.3     |
| strata        | users.json   | 3.279    | 4.155       | 4.200    | 44.3     |
| orjson        | users.json   | 5.271    | 6.348       | 6.928    | 44.3     |
| msgspec       | users.json   | 5.095    | 6.801       | 7.081    | 47.2     |
| ujson         | users.json   | 6.481    | 8.907       | 9.728    | 47.2     |
| json (stdlib) | users.json   | 9.009    | 10.578      | 10.828   | 48.3     |
| strata        | users.ndjson | 3.665    | 4.379       | 4.596    | 58.0     |
| orjson        | users.ndjson | 4.638    | 5.956       | 6.160    | 58.0     |
| msgspec       | users.ndjson | 5.079    | 6.653       | 6.812    | 59.0     |
| ujson         | users.ndjson | 6.775    | 8.858       | 8.994    | 59.0     |
| json (stdlib) | users.ndjson | 9.658    | 11.140      | 11.691   | 59.0     |
| strata        | wide_arrays  | 3.275    | 3.762       | 3.956    | 66.7     |
| orjson        | wide_arrays  | 3.736    | 4.553       | 4.743    | 68.0     |
| msgspec       | wide_arrays  | 5.011    | 5.238       | 5.699    | 70.6     |
| ujson         | wide_arrays  | 7.357    | 7.949       | 8.202    | 70.6     |
| json (stdlib) | wide_arrays  | 13.710   | 14.300      | 14.476   | 70.6     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.267    | 0.267       | 0.295    | 60.2     |
| orjson  | flat        | 0.271    | 0.271       | 0.272    | 60.2     |
| msgspec | flat        | 0.302    | 0.302       | 0.307    | 60.3     |
| ujson   | flat        | 1.252    | 1.266       | 1.503    | 60.3     |
| json    | flat        | 2.280    | 2.323       | 2.326    | 60.3     |
| orjson  | mixed       | 0.276    | 0.279       | 0.283    | 74.3     |
| strata  | mixed       | 0.306    | 0.308       | 0.308    | 74.3     |
| msgspec | mixed       | 0.301    | 0.315       | 0.368    | 74.5     |
| ujson   | mixed       | 1.310    | 1.313       | 1.350    | 74.5     |
| json    | mixed       | 2.991    | 3.059       | 3.076    | 74.5     |
| orjson  | nested      | 0.189    | 0.193       | 0.197    | 60.3     |
| msgspec | nested      | 0.199    | 0.201       | 0.254    | 60.3     |
| strata  | nested      | 0.202    | 0.202       | 0.203    | 60.3     |
| ujson   | nested      | 0.974    | 0.996       | 1.082    | 60.3     |
| json    | nested      | 1.327    | 1.345       | 1.345    | 60.3     |
| strata  | users.json  | 1.208    | 1.212       | 1.361    | 49.2     |
| orjson  | users.json  | 1.498    | 1.501       | 1.589    | 50.4     |
| msgspec | users.json  | 1.561    | 1.580       | 1.616    | 52.3     |
| ujson   | users.json  | 6.390    | 6.391       | 6.424    | 52.4     |
| json    | users.json  | 8.681    | 8.734       | 9.113    | 52.6     |
| orjson  | wide_arrays | 1.501    | 1.593       | 1.664    | 73.7     |
| msgspec | wide_arrays | 1.643    | 1.647       | 1.678    | 73.8     |
| strata  | wide_arrays | 1.716    | 1.758       | 1.918    | 71.9     |
| ujson   | wide_arrays | 7.168    | 7.239       | 7.245    | 75.1     |
| json    | wide_arrays | 19.882   | 19.910      | 19.983   | 75.1     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 0.468    | 0.481       | 0.577    | 60.3     |
| msgspec       | flat        | 0.483    | 0.491       | 0.539    | 60.3     |
| strata        | flat        | 0.436    | 0.498       | 0.498    | 60.3     |
| ujson         | flat        | 1.578    | 1.619       | 1.661    | 60.3     |
| json (stdlib) | flat        | 8.334    | 8.451       | 8.473    | 60.3     |
| msgspec       | mixed       | 0.476    | 0.477       | 0.505    | 74.5     |
| orjson        | mixed       | 0.477    | 0.477       | 0.605    | 74.5     |
| strata        | mixed       | 0.489    | 0.540       | 0.562    | 74.5     |
| ujson         | mixed       | 1.499    | 1.504       | 1.510    | 74.5     |
| json (stdlib) | mixed       | 9.911    | 10.086      | 10.394   | 74.5     |
| strata        | nested      | 0.345    | 0.346       | 0.432    | 60.3     |
| orjson        | nested      | 0.410    | 0.448       | 0.466    | 60.3     |
| msgspec       | nested      | 0.482    | 0.488       | 0.518    | 60.3     |
| ujson         | nested      | 1.163    | 1.195       | 1.251    | 60.3     |
| json (stdlib) | nested      | 12.042   | 12.047      | 12.056   | 60.3     |
| strata        | users.json  | 1.466    | 1.517       | 1.566    | 52.6     |
| msgspec       | users.json  | 1.761    | 1.763       | 1.890    | 52.6     |
| orjson        | users.json  | 1.738    | 1.870       | 1.879    | 52.6     |
| ujson         | users.json  | 6.775    | 6.796       | 6.827    | 52.6     |
| json (stdlib) | users.json  | 68.512   | 69.135      | 70.594   | 52.7     |
| orjson        | wide_arrays | 1.951    | 1.955       | 1.962    | 75.1     |
| strata        | wide_arrays | 1.976    | 2.068       | 2.133    | 75.1     |
| msgspec       | wide_arrays | 1.944    | 2.107       | 2.193    | 75.1     |
| ujson         | wide_arrays | 7.430    | 7.580       | 7.637    | 75.1     |
| json (stdlib) | wide_arrays | 45.030   | 45.051      | 45.101   | 75.1     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.576    | 1000    | 52.8     |
|                                | orjson+jmespath    | 5.621    | 1000    | 52.9     |
|                                | orjson+jsonpath-ng | 6.775    | 1000    | 53.5     |
| all item prices                | strata             | 1.677    | 12046   | 53.5     |
|                                | orjson+jmespath    | 13.979   | 1000    | 53.6     |
|                                | orjson+jsonpath-ng | 35.721   | 12046   | 53.3     |
| recursive price                | strata             | 1.636    | 12046   | 52.7     |
|                                | orjson+jsonpath-ng | 128.934  | 12046   | 54.0     |
| flat:$.records\[\*\].id        | strata             | 0.393    | 500     | 60.3     |
|                                | orjson+jmespath    | 0.828    | 500     | 60.3     |
|                                | orjson+jsonpath-ng | 2.264    | 500     | 60.3     |
| flat:$.records\[0\]            | strata             | 0.403    | 1       | 60.3     |
|                                | orjson+jsonpath-ng | 0.656    | 1       | 60.3     |
|                                | orjson+jmespath    | 0.678    | 1       | 60.3     |
| nested:$.records\[\*\].id      | strata             | 0.303    | 500     | 60.3     |
|                                | orjson+jmespath    | 1.771    | 500     | 60.3     |
|                                | orjson+jsonpath-ng | 2.331    | 500     | 60.3     |
| nested:$.records\[0\]          | strata             | 0.345    | 1       | 60.3     |
|                                | orjson+jsonpath-ng | 1.608    | 1       | 60.3     |
|                                | orjson+jmespath    | 1.843    | 1       | 60.3     |
| wide_arrays:$.records\[\*\].id | strata             | 2.404    | 500     | 75.1     |
|                                | orjson+jmespath    | 4.066    | 500     | 75.1     |
|                                | orjson+jsonpath-ng | 4.870    | 500     | 75.1     |
| wide_arrays:$.records\[0\]     | strata             | 2.383    | 1       | 75.1     |
|                                | orjson+jmespath    | 3.960    | 1       | 75.1     |
|                                | orjson+jsonpath-ng | 4.091    | 1       | 75.1     |
| mixed:$.records\[\*\].id       | strata             | 0.436    | 500     | 74.5     |
|                                | orjson+jmespath    | 0.809    | 500     | 74.5     |
|                                | orjson+jsonpath-ng | 2.318    | 500     | 74.5     |
| mixed:$.records\[0\]           | strata             | 0.448    | 1       | 74.5     |
|                                | orjson+jsonpath-ng | 0.672    | 1       | 74.5     |
|                                | orjson+jmespath    | 0.682    | 1       | 74.5     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.070    | 1000    | 53.0     |
|                                            | jmespath    | 0.408    | 1000    | 53.0     |
|                                            | jsonpath-ng | 4.355    | 1000    | 53.0     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.916    | 12046   | 53.0     |
|                                            | jmespath    | 11.246   | 1000    | 53.3     |
|                                            | jsonpath-ng | 32.438   | 12046   | 57.5     |
| $..price                                   | strata      | 1.276    | 12046   | 56.5     |
|                                            | jsonpath-ng | 124.237  | 12046   | 58.5     |
| $.users\[?(@.age>30)\]                     | strata      | 0.106    | 794     | 55.9     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.460    | 1604    | 55.9     |
| flat:$.records\[\*\].id                    | strata      | 0.032    | 500     | 60.3     |
|                                            | jmespath    | 0.165    | 500     | 60.3     |
|                                            | jsonpath-ng | 1.551    | 500     | 60.3     |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 60.3     |
|                                            | jmespath    | 0.005    | 1       | 60.3     |
|                                            | jsonpath-ng | 0.008    | 1       | 60.3     |
| nested:$.records\[\*\].id                  | strata      | 0.028    | 500     | 60.3     |
|                                            | jmespath    | 0.172    | 500     | 60.3     |
|                                            | jsonpath-ng | 1.720    | 500     | 60.3     |
| nested:$.records\[0\]                      | strata      | 0.002    | 1       | 60.3     |
|                                            | jmespath    | 0.005    | 1       | 60.3     |
|                                            | jsonpath-ng | 0.006    | 1       | 60.3     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.029    | 500     | 75.1     |
|                                            | jmespath    | 0.191    | 500     | 75.1     |
|                                            | jsonpath-ng | 2.238    | 500     | 75.1     |
| wide_arrays:$.records\[0\]                 | strata      | 0.003    | 1       | 75.1     |
|                                            | jmespath    | 0.015    | 1       | 75.1     |
|                                            | jsonpath-ng | 0.018    | 1       | 75.1     |
| mixed:$.records\[\*\].id                   | strata      | 0.029    | 500     | 74.5     |
|                                            | jmespath    | 0.199    | 500     | 74.5     |
|                                            | jsonpath-ng | 1.944    | 500     | 74.5     |
| mixed:$.records\[0\]                       | jmespath    | 0.006    | 1       | 74.5     |
|                                            | jsonpath-ng | 0.006    | 1       | 74.5     |
|                                            | strata      | 0.007    | 1       | 74.5     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **22.4% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **37.0% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **55.4% faster** than #2 (msgspec)  |
| load (NDJSON file)  | **#1** / 5              | **26.5% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **24.0% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **18.5% faster** than #2 (orjson)   |
| loads (flat)        | **#2** / 5              | 6.0% behind #1 (orjson)             |
| load (flat)         | **#2** / 5              | 5.0% behind #1 (orjson)             |
| dumps (flat)        | **#1** / 5              | **1.6% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **7.5% faster** than #2 (orjson)    |
| loads (nested)      | **#1** / 5              | **164.0% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **182.3% faster** than #2 (orjson)  |
| dumps (nested)      | **#3** / 5              | 6.8% behind #1 (orjson)             |
| dump (nested)       | **#1** / 5              | **18.9% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#1** / 5              | **21.5% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **14.1% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#3** / 5              | 14.3% behind #1 (orjson)            |
| dump (wide_arrays)  | **#3** / 5              | 1.6% behind #1 (msgspec)            |
| loads (mixed)       | **#3** / 5              | 45.5% behind #1 (orjson)            |
| load (mixed)        | **#3** / 5              | 24.4% behind #1 (orjson)            |
| dumps (mixed)       | **#3** / 5              | 10.8% behind #1 (orjson)            |
| dump (mixed)        | **#3** / 5              | 2.8% behind #1 (msgspec)            |
| search (JSONPath)   | **#1** in 11/11 queries | avg **915% faster**                 |
| query (JSONPath)    | **#1** in 12/13 queries | -                                   |
