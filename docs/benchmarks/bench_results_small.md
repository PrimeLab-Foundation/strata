# Strata Benchmark Results

Generated: 2026-03-15 23:51:47

## Environment

- Commit: aa99eea214c1ca13045f33e759bcacb6508755a4
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
| orjson        | flat         | 0.552    | 0.626       | 0.670    | 60.2     |
| msgspec       | flat         | 0.600    | 0.681       | 0.694    | 60.2     |
| strata        | flat         | 0.723    | 0.812       | 0.916    | 59.0     |
| ujson         | flat         | 1.061    | 1.193       | 1.240    | 60.2     |
| json (stdlib) | flat         | 1.999    | 2.065       | 2.124    | 60.2     |
| orjson        | mixed        | 0.480    | 0.611       | 0.634    | 79.9     |
| msgspec       | mixed        | 0.565    | 0.716       | 0.724    | 79.9     |
| strata        | mixed        | 0.675    | 0.806       | 0.824    | 79.9     |
| ujson         | mixed        | 0.939    | 1.109       | 1.123    | 79.9     |
| json (stdlib) | mixed        | 2.101    | 2.155       | 2.302    | 79.9     |
| strata        | nested       | 0.762    | 0.832       | 0.870    | 60.2     |
| msgspec       | nested       | 1.414    | 1.628       | 1.728    | 60.3     |
| orjson        | nested       | 1.431    | 1.632       | 1.693    | 60.3     |
| ujson         | nested       | 1.651    | 1.973       | 1.979    | 60.3     |
| json (stdlib) | nested       | 2.224    | 3.172       | 3.216    | 60.3     |
| strata        | users.json   | 4.442    | 5.183       | 5.196    | 34.2     |
| orjson        | users.json   | 4.968    | 6.828       | 7.075    | 38.1     |
| msgspec       | users.json   | 4.707    | 7.001       | 7.547    | 41.3     |
| ujson         | users.json   | 6.686    | 9.631       | 10.002   | 41.1     |
| json (stdlib) | users.json   | 11.350   | 11.815      | 12.081   | 43.2     |
| strata        | users.ndjson | 4.475    | 5.067       | 5.577    | 55.0     |
| msgspec       | users.ndjson | 4.358    | 6.083       | 6.802    | 57.0     |
| orjson        | users.ndjson | 5.584    | 6.618       | 8.812    | 55.9     |
| ujson         | users.ndjson | 6.979    | 8.306       | 9.024    | 57.0     |
| json (stdlib) | users.ndjson | 9.350    | 14.990      | 15.092   | 57.0     |
| strata        | wide_arrays  | 2.586    | 3.311       | 3.665    | 63.6     |
| orjson        | wide_arrays  | 3.615    | 4.680       | 5.072    | 66.2     |
| msgspec       | wide_arrays  | 4.008    | 4.842       | 5.003    | 67.1     |
| ujson         | wide_arrays  | 6.404    | 7.246       | 7.423    | 67.1     |
| json (stdlib) | wide_arrays  | 12.971   | 16.051      | 16.734   | 68.4     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| msgspec       | flat         | 0.756    | 0.887       | 0.976    | 60.2     |
| orjson        | flat         | 0.798    | 0.996       | 1.040    | 60.2     |
| strata        | flat         | 1.000    | 1.087       | 1.089    | 60.2     |
| ujson         | flat         | 1.265    | 1.408       | 1.597    | 60.2     |
| json (stdlib) | flat         | 2.096    | 2.170       | 2.209    | 60.2     |
| orjson        | mixed        | 0.761    | 0.765       | 0.810    | 79.9     |
| msgspec       | mixed        | 0.649    | 0.806       | 0.818    | 79.9     |
| strata        | mixed        | 0.877    | 0.933       | 0.960    | 79.9     |
| ujson         | mixed        | 1.313    | 1.324       | 1.338    | 79.9     |
| json (stdlib) | mixed        | 2.102    | 2.267       | 2.485    | 79.9     |
| strata        | nested       | 0.760    | 0.885       | 0.892    | 60.3     |
| msgspec       | nested       | 1.774    | 2.075       | 2.130    | 60.3     |
| json (stdlib) | nested       | 2.771    | 2.840       | 2.866    | 60.3     |
| orjson        | nested       | 1.653    | 2.960       | 4.676    | 60.3     |
| ujson         | nested       | 3.322    | 3.328       | 3.450    | 60.3     |
| strata        | users.json   | 4.302    | 4.876       | 4.923    | 43.3     |
| orjson        | users.json   | 4.969    | 6.653       | 6.776    | 44.2     |
| msgspec       | users.json   | 7.480    | 7.838       | 10.924   | 48.3     |
| ujson         | users.json   | 6.687    | 9.056       | 9.855    | 48.3     |
| json (stdlib) | users.json   | 9.499    | 10.698      | 10.912   | 48.3     |
| strata        | users.ndjson | 4.258    | 5.888       | 6.001    | 57.0     |
| orjson        | users.ndjson | 4.761    | 6.266       | 6.290    | 57.0     |
| ujson         | users.ndjson | 6.723    | 8.902       | 9.886    | 59.0     |
| msgspec       | users.ndjson | 4.917    | 9.885       | 9.907    | 59.0     |
| json (stdlib) | users.ndjson | 9.255    | 11.730      | 12.409   | 59.0     |
| strata        | wide_arrays  | 2.801    | 3.404       | 3.427    | 68.4     |
| orjson        | wide_arrays  | 3.624    | 4.445       | 4.484    | 69.8     |
| msgspec       | wide_arrays  | 4.705    | 5.524       | 6.010    | 71.1     |
| ujson         | wide_arrays  | 7.246    | 7.987       | 7.987    | 71.1     |
| json (stdlib) | wide_arrays  | 14.457   | 15.289      | 16.152   | 71.1     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.238    | 0.240       | 0.241    | 60.2     |
| msgspec | flat        | 0.295    | 0.300       | 0.301    | 60.2     |
| strata  | flat        | 0.300    | 0.302       | 0.303    | 60.2     |
| ujson   | flat        | 1.176    | 1.177       | 1.182    | 60.2     |
| json    | flat        | 2.184    | 2.213       | 2.216    | 60.2     |
| orjson  | mixed       | 0.299    | 0.305       | 0.322    | 79.9     |
| msgspec | mixed       | 0.338    | 0.341       | 0.376    | 79.9     |
| strata  | mixed       | 0.363    | 0.374       | 0.381    | 79.9     |
| ujson   | mixed       | 1.410    | 1.430       | 1.818    | 79.9     |
| json    | mixed       | 3.017    | 3.138       | 3.197    | 79.9     |
| orjson  | nested      | 0.189    | 0.215       | 0.219    | 60.3     |
| msgspec | nested      | 0.225    | 0.228       | 0.252    | 60.3     |
| strata  | nested      | 0.213    | 0.254       | 0.270    | 60.3     |
| ujson   | nested      | 0.998    | 1.035       | 1.087    | 60.3     |
| json    | nested      | 1.353    | 1.383       | 1.394    | 60.3     |
| msgspec | users.json  | 1.427    | 1.441       | 1.567    | 50.3     |
| orjson  | users.json  | 1.492    | 1.541       | 1.706    | 50.1     |
| strata  | users.json  | 1.557    | 1.607       | 1.779    | 48.3     |
| ujson   | users.json  | 6.163    | 6.212       | 6.374    | 50.3     |
| json    | users.json  | 8.622    | 8.732       | 9.176    | 50.8     |
| orjson  | wide_arrays | 1.552    | 1.586       | 1.707    | 74.4     |
| msgspec | wide_arrays | 1.712    | 1.728       | 1.856    | 75.7     |
| strata  | wide_arrays | 1.843    | 1.871       | 2.037    | 72.6     |
| ujson   | wide_arrays | 7.327    | 7.465       | 7.523    | 78.6     |
| json    | wide_arrays | 19.835   | 20.329      | 20.906   | 79.0     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 0.504    | 0.569       | 0.823    | 60.2     |
| strata        | flat        | 0.471    | 0.811       | 0.919    | 60.2     |
| msgspec       | flat        | 0.641    | 0.941       | 1.009    | 60.2     |
| ujson         | flat        | 1.592    | 1.674       | 1.806    | 60.2     |
| json (stdlib) | flat        | 7.951    | 8.021       | 8.091    | 60.2     |
| orjson        | mixed       | 0.515    | 0.560       | 0.564    | 79.9     |
| msgspec       | mixed       | 0.584    | 0.596       | 0.630    | 79.9     |
| strata        | mixed       | 0.632    | 0.750       | 0.981    | 79.9     |
| ujson         | mixed       | 1.597    | 1.665       | 1.796    | 79.9     |
| json (stdlib) | mixed       | 10.297   | 10.369      | 10.380   | 79.9     |
| strata        | nested      | 0.349    | 0.401       | 0.439    | 60.3     |
| orjson        | nested      | 0.416    | 0.430       | 0.462    | 60.3     |
| msgspec       | nested      | 0.386    | 0.529       | 0.674    | 60.3     |
| ujson         | nested      | 1.399    | 1.402       | 1.417    | 60.3     |
| json (stdlib) | nested      | 11.619   | 11.621      | 11.687   | 60.3     |
| orjson        | users.json  | 1.727    | 1.759       | 1.845    | 50.8     |
| msgspec       | users.json  | 1.842    | 1.914       | 1.937    | 50.8     |
| strata        | users.json  | 1.968    | 2.176       | 2.192    | 50.8     |
| ujson         | users.json  | 6.738    | 6.866       | 7.085    | 50.8     |
| json (stdlib) | users.json  | 65.826   | 66.271      | 66.933   | 50.8     |
| orjson        | wide_arrays | 2.005    | 2.028       | 2.461    | 79.0     |
| msgspec       | wide_arrays | 2.075    | 2.087       | 2.098    | 79.8     |
| strata        | wide_arrays | 2.194    | 2.351       | 2.374    | 79.0     |
| ujson         | wide_arrays | 7.761    | 8.020       | 8.107    | 79.8     |
| json (stdlib) | wide_arrays | 43.134   | 43.902      | 45.396   | 79.8     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.897    | 1000    | 50.8     |
|                                | orjson+jmespath    | 5.582    | 1000    | 50.9     |
|                                | orjson+jsonpath-ng | 6.790    | 1000    | 51.5     |
| all item prices                | strata             | 2.154    | 12046   | 51.5     |
|                                | orjson+jmespath    | 15.966   | 1000    | 51.5     |
|                                | orjson+jsonpath-ng | 37.275   | 12046   | 51.4     |
| recursive price                | strata             | 2.070    | 12046   | 51.1     |
|                                | orjson+jsonpath-ng | 128.282  | 12046   | 51.9     |
| flat:$.records\[\*\].id        | strata             | 0.418    | 500     | 60.2     |
|                                | orjson+jmespath    | 0.795    | 500     | 60.2     |
|                                | orjson+jsonpath-ng | 2.167    | 500     | 60.2     |
| flat:$.records\[0\]            | strata             | 0.414    | 1       | 60.2     |
|                                | orjson+jsonpath-ng | 0.633    | 1       | 60.2     |
|                                | orjson+jmespath    | 0.634    | 1       | 60.2     |
| nested:$.records\[\*\].id      | strata             | 0.364    | 500     | 60.3     |
|                                | orjson+jmespath    | 1.715    | 500     | 60.3     |
|                                | orjson+jsonpath-ng | 2.219    | 500     | 60.3     |
| nested:$.records\[0\]          | strata             | 0.359    | 1       | 60.3     |
|                                | orjson+jmespath    | 1.569    | 1       | 60.3     |
|                                | orjson+jsonpath-ng | 2.199    | 1       | 60.3     |
| wide_arrays:$.records\[\*\].id | strata             | 2.024    | 500     | 79.8     |
|                                | orjson+jmespath    | 4.022    | 500     | 79.8     |
|                                | orjson+jsonpath-ng | 5.134    | 500     | 79.8     |
| wide_arrays:$.records\[0\]     | strata             | 2.033    | 1       | 79.8     |
|                                | orjson+jsonpath-ng | 3.796    | 1       | 79.8     |
|                                | orjson+jmespath    | 3.806    | 1       | 79.8     |
| mixed:$.records\[\*\].id       | strata             | 0.502    | 500     | 79.9     |
|                                | orjson+jmespath    | 0.792    | 500     | 79.9     |
|                                | orjson+jsonpath-ng | 3.108    | 500     | 79.9     |
| mixed:$.records\[0\]           | strata             | 0.516    | 1       | 79.9     |
|                                | orjson+jsonpath-ng | 0.662    | 1       | 79.9     |
|                                | orjson+jmespath    | 0.687    | 1       | 79.9     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.072    | 1000    | 52.8     |
|                                            | jmespath    | 0.369    | 1000    | 52.8     |
|                                            | jsonpath-ng | 4.178    | 1000    | 52.8     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.906    | 12046   | 52.8     |
|                                            | jmespath    | 11.226   | 1000    | 52.8     |
|                                            | jsonpath-ng | 33.435   | 12046   | 55.5     |
| $..price                                   | strata      | 1.264    | 12046   | 54.5     |
|                                            | jsonpath-ng | 124.795  | 12046   | 57.5     |
| $.users\[?(@.age>30)\]                     | strata      | 0.137    | 794     | 54.9     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.444    | 1604    | 54.9     |
| flat:$.records\[\*\].id                    | strata      | 0.026    | 500     | 60.2     |
|                                            | jmespath    | 0.165    | 500     | 60.2     |
|                                            | jsonpath-ng | 1.974    | 500     | 60.2     |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 60.2     |
|                                            | jmespath    | 0.005    | 1       | 60.2     |
|                                            | jsonpath-ng | 0.007    | 1       | 60.2     |
| nested:$.records\[\*\].id                  | strata      | 0.028    | 500     | 60.3     |
|                                            | jmespath    | 0.168    | 500     | 60.3     |
|                                            | jsonpath-ng | 1.741    | 500     | 60.3     |
| nested:$.records\[0\]                      | strata      | 0.002    | 1       | 60.3     |
|                                            | jmespath    | 0.006    | 1       | 60.3     |
|                                            | jsonpath-ng | 0.007    | 1       | 60.3     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.029    | 500     | 79.9     |
|                                            | jmespath    | 0.186    | 500     | 79.9     |
|                                            | jsonpath-ng | 2.533    | 500     | 79.9     |
| wide_arrays:$.records\[0\]                 | strata      | 0.003    | 1       | 79.9     |
|                                            | jmespath    | 0.007    | 1       | 79.9     |
|                                            | jsonpath-ng | 0.007    | 1       | 79.9     |
| mixed:$.records\[\*\].id                   | strata      | 0.034    | 500     | 79.9     |
|                                            | jmespath    | 0.178    | 500     | 79.9     |
|                                            | jsonpath-ng | 2.281    | 500     | 79.9     |
| mixed:$.records\[0\]                       | strata      | 0.006    | 1       | 79.9     |
|                                            | jmespath    | 0.011    | 1       | 79.9     |
|                                            | jsonpath-ng | 0.028    | 1       | 79.9     |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **6.0% faster** than #2 (msgspec)  |
| loads (NDJSON)      | **#2** / 5              | 2.7% behind #1 (msgspec)           |
| load (JSON file)    | **#1** / 5              | **15.5% faster** than #2 (orjson)  |
| load (NDJSON file)  | **#1** / 5              | **11.8% faster** than #2 (orjson)  |
| dumps               | **#3** / 5              | 9.2% behind #1 (msgspec)           |
| dump                | **#3** / 5              | 14.0% behind #1 (orjson)           |
| loads (flat)        | **#3** / 5              | 31.0% behind #1 (orjson)           |
| load (flat)         | **#3** / 5              | 32.2% behind #1 (msgspec)          |
| dumps (flat)        | **#3** / 5              | 26.1% behind #1 (orjson)           |
| dump (flat)         | **#1** / 5              | **6.9% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **85.7% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **117.5% faster** than #2 (orjson) |
| dumps (nested)      | **#2** / 5              | 12.7% behind #1 (orjson)           |
| dump (nested)       | **#1** / 5              | **10.6% faster** than #2 (msgspec) |
| loads (wide_arrays) | **#1** / 5              | **39.8% faster** than #2 (orjson)  |
| load (wide_arrays)  | **#1** / 5              | **29.4% faster** than #2 (orjson)  |
| dumps (wide_arrays) | **#3** / 5              | 18.8% behind #1 (orjson)           |
| dump (wide_arrays)  | **#3** / 5              | 9.4% behind #1 (orjson)            |
| loads (mixed)       | **#3** / 5              | 40.8% behind #1 (orjson)           |
| load (mixed)        | **#3** / 5              | 35.2% behind #1 (msgspec)          |
| dumps (mixed)       | **#3** / 5              | 21.4% behind #1 (orjson)           |
| dump (mixed)        | **#3** / 5              | 22.7% behind #1 (orjson)           |
| search (JSONPath)   | **#1** in 11/11 queries | avg **732% faster**                |
| query (JSONPath)    | **#1** in 13/13 queries | -                                  |
