# Strata Benchmark Results

Generated: 2026-03-17 23:10:54

## Environment

- Commit: 424c79cc06825213a77b7bb3e900fe94cce6309b
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
| orjson        | flat         | 0.580    | 0.680       | 0.717    | 63.2     |
| msgspec       | flat         | 0.622    | 0.731       | 0.742    | 63.2     |
| strata        | flat         | 0.819    | 0.821       | 0.928    | 62.5     |
| ujson         | flat         | 1.091    | 1.226       | 1.873    | 63.2     |
| json (stdlib) | flat         | 2.118    | 2.140       | 2.455    | 63.2     |
| orjson        | mixed        | 0.504    | 0.669       | 0.811    | 77.2     |
| msgspec       | mixed        | 0.623    | 0.747       | 0.757    | 77.2     |
| strata        | mixed        | 0.729    | 0.878       | 0.904    | 77.2     |
| ujson         | mixed        | 1.061    | 1.328       | 1.337    | 77.2     |
| json (stdlib) | mixed        | 2.084    | 2.196       | 2.475    | 77.2     |
| strata        | nested       | 0.652    | 0.765       | 0.796    | 62.2     |
| orjson        | nested       | 1.622    | 1.818       | 1.915    | 62.2     |
| msgspec       | nested       | 2.020    | 2.113       | 2.165    | 62.2     |
| ujson         | nested       | 2.167    | 2.235       | 3.038    | 62.2     |
| json (stdlib) | nested       | 2.674    | 2.725       | 3.374    | 62.2     |
| strata        | users.json   | 4.062    | 5.688       | 6.122    | 34.1     |
| msgspec       | users.json   | 4.913    | 6.432       | 10.621   | 41.2     |
| orjson        | users.json   | 5.474    | 7.011       | 7.840    | 38.1     |
| ujson         | users.json   | 7.109    | 9.649       | 10.821   | 41.1     |
| json (stdlib) | users.json   | 10.717   | 11.248      | 12.050   | 43.7     |
| strata        | users.ndjson | 3.993    | 4.965       | 5.469    | 57.7     |
| msgspec       | users.ndjson | 6.317    | 6.481       | 6.679    | 60.7     |
| orjson        | users.ndjson | 4.913    | 6.701       | 6.888    | 58.7     |
| ujson         | users.ndjson | 6.316    | 9.988       | 12.165   | 60.7     |
| json (stdlib) | users.ndjson | 9.740    | 11.493      | 14.296   | 60.7     |
| strata        | wide_arrays  | 3.602    | 3.852       | 4.053    | 64.6     |
| orjson        | wide_arrays  | 3.496    | 4.349       | 4.873    | 67.2     |
| msgspec       | wide_arrays  | 5.902    | 5.987       | 7.629    | 68.1     |
| ujson         | wide_arrays  | 7.962    | 8.321       | 8.857    | 68.1     |
| json (stdlib) | wide_arrays  | 13.682   | 15.224      | 15.509   | 69.4     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| msgspec       | flat         | 0.718    | 0.798       | 0.809    | 63.2     |
| orjson        | flat         | 0.724    | 0.889       | 1.217    | 63.2     |
| strata        | flat         | 0.870    | 0.894       | 1.065    | 63.2     |
| ujson         | flat         | 1.277    | 1.386       | 1.471    | 63.2     |
| json (stdlib) | flat         | 2.323    | 2.341       | 2.550    | 63.2     |
| msgspec       | mixed        | 0.793    | 0.846       | 0.959    | 77.2     |
| strata        | mixed        | 0.818    | 1.016       | 1.164    | 77.2     |
| orjson        | mixed        | 0.736    | 1.109       | 1.167    | 77.2     |
| ujson         | mixed        | 1.197    | 1.474       | 2.668    | 77.2     |
| json (stdlib) | mixed        | 2.341    | 2.378       | 2.638    | 77.2     |
| strata        | nested       | 0.706    | 0.793       | 0.820    | 62.2     |
| orjson        | nested       | 1.906    | 1.961       | 2.061    | 62.2     |
| ujson         | nested       | 2.311    | 2.422       | 2.927    | 62.2     |
| msgspec       | nested       | 2.456    | 2.842       | 3.497    | 62.2     |
| json (stdlib) | nested       | 3.770    | 4.276       | 5.012    | 62.2     |
| strata        | users.json   | 3.769    | 5.330       | 6.549    | 43.8     |
| orjson        | users.json   | 5.619    | 6.672       | 6.822    | 44.8     |
| ujson         | users.json   | 6.285    | 9.840       | 10.321   | 48.0     |
| msgspec       | users.json   | 5.017    | 9.930       | 10.362   | 48.0     |
| json (stdlib) | users.json   | 10.672   | 11.520      | 12.863   | 48.0     |
| strata        | users.ndjson | 4.340    | 5.380       | 5.771    | 60.7     |
| orjson        | users.ndjson | 6.208    | 6.393       | 6.963    | 60.7     |
| msgspec       | users.ndjson | 5.238    | 6.572       | 7.171    | 61.9     |
| ujson         | users.ndjson | 7.558    | 11.192      | 12.665   | 61.9     |
| json (stdlib) | users.ndjson | 9.617    | 11.714      | 12.459   | 61.9     |
| strata        | wide_arrays  | 4.138    | 4.346       | 5.255    | 69.4     |
| orjson        | wide_arrays  | 4.880    | 4.881       | 5.731    | 69.4     |
| msgspec       | wide_arrays  | 5.507    | 5.999       | 6.028    | 70.8     |
| ujson         | wide_arrays  | 8.512    | 10.105      | 10.621   | 70.8     |
| json (stdlib) | wide_arrays  | 14.112   | 14.948      | 15.090   | 70.8     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.255    | 0.260       | 0.315    | 63.2     |
| orjson  | flat        | 0.251    | 0.261       | 0.265    | 63.2     |
| msgspec | flat        | 0.303    | 0.331       | 0.373    | 63.2     |
| ujson   | flat        | 1.230    | 1.236       | 1.247    | 63.2     |
| json    | flat        | 2.769    | 2.962       | 3.036    | 63.2     |
| strata  | mixed       | 0.270    | 0.271       | 0.283    | 77.2     |
| orjson  | mixed       | 0.284    | 0.286       | 0.312    | 77.2     |
| msgspec | mixed       | 0.329    | 0.334       | 0.339    | 77.2     |
| ujson   | mixed       | 1.312    | 1.336       | 1.343    | 77.2     |
| json    | mixed       | 2.983    | 3.008       | 3.024    | 77.2     |
| strata  | nested      | 0.184    | 0.184       | 0.191    | 62.2     |
| orjson  | nested      | 0.195    | 0.198       | 0.207    | 62.2     |
| msgspec | nested      | 0.203    | 0.213       | 0.215    | 62.2     |
| ujson   | nested      | 0.998    | 1.009       | 1.183    | 62.2     |
| json    | nested      | 1.372    | 1.472       | 1.576    | 62.2     |
| strata  | users.json  | 1.070    | 1.073       | 1.709    | 49.0     |
| orjson  | users.json  | 1.505    | 1.546       | 1.652    | 51.5     |
| msgspec | users.json  | 1.608    | 1.694       | 3.499    | 53.4     |
| ujson   | users.json  | 6.515    | 6.692       | 7.702    | 53.5     |
| json    | users.json  | 8.929    | 8.938       | 9.172    | 54.4     |
| strata  | wide_arrays | 1.376    | 1.463       | 1.518    | 72.0     |
| orjson  | wide_arrays | 1.566    | 1.567       | 1.795    | 73.8     |
| msgspec | wide_arrays | 1.739    | 1.748       | 1.895    | 75.1     |
| ujson   | wide_arrays | 7.185    | 7.268       | 7.299    | 76.8     |
| json    | wide_arrays | 20.077   | 20.229      | 20.281   | 77.2     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 0.510    | 0.560       | 0.592    | 63.2     |
| msgspec       | flat        | 0.652    | 0.658       | 0.760    | 63.2     |
| strata        | flat        | 0.451    | 0.746       | 0.850    | 63.2     |
| ujson         | flat        | 1.725    | 1.914       | 2.693    | 63.2     |
| json (stdlib) | flat        | 8.870    | 9.409       | 9.724    | 63.2     |
| strata        | mixed       | 0.392    | 0.392       | 0.445    | 77.2     |
| orjson        | mixed       | 0.457    | 0.507       | 0.939    | 77.2     |
| msgspec       | mixed       | 0.538    | 0.883       | 1.239    | 77.2     |
| ujson         | mixed       | 1.641    | 1.759       | 1.971    | 77.2     |
| json (stdlib) | mixed       | 10.460   | 10.568      | 11.963   | 77.2     |
| strata        | nested      | 0.299    | 0.324       | 0.376    | 62.2     |
| msgspec       | nested      | 0.371    | 0.420       | 0.630    | 62.2     |
| orjson        | nested      | 0.389    | 0.482       | 0.519    | 62.2     |
| ujson         | nested      | 1.183    | 1.204       | 1.510    | 62.2     |
| json (stdlib) | nested      | 12.270   | 12.951      | 14.890   | 62.2     |
| strata        | users.json  | 1.409    | 1.458       | 1.488    | 54.4     |
| msgspec       | users.json  | 1.778    | 1.790       | 2.163    | 54.4     |
| orjson        | users.json  | 1.821    | 1.862       | 1.885    | 54.4     |
| ujson         | users.json  | 6.790    | 6.899       | 7.173    | 54.4     |
| json (stdlib) | users.json  | 69.715   | 69.854      | 70.253   | 54.4     |
| strata        | wide_arrays | 1.741    | 1.781       | 2.291    | 77.2     |
| orjson        | wide_arrays | 1.839    | 1.933       | 1.989    | 77.2     |
| msgspec       | wide_arrays | 1.972    | 1.994       | 2.119    | 77.2     |
| ujson         | wide_arrays | 7.497    | 7.558       | 7.672    | 77.2     |
| json (stdlib) | wide_arrays | 45.010   | 45.703      | 47.007   | 77.2     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.864    | 1000    | 54.5     |
|                                | orjson+jmespath    | 5.882    | 1000    | 54.6     |
|                                | orjson+jsonpath-ng | 7.035    | 1000    | 55.2     |
| all item prices                | strata             | 2.338    | 12046   | 55.2     |
|                                | orjson+jmespath    | 13.463   | 1000    | 55.2     |
|                                | orjson+jsonpath-ng | 36.006   | 12046   | 55.1     |
| recursive price                | strata             | 2.230    | 12046   | 54.7     |
|                                | orjson+jsonpath-ng | 130.515  | 12046   | 55.6     |
| flat:$.records\[\*\].id        | strata             | 0.501    | 500     | 63.2     |
|                                | orjson+jmespath    | 0.832    | 500     | 63.2     |
|                                | orjson+jsonpath-ng | 2.463    | 500     | 62.2     |
| flat:$.records\[0\]            | strata             | 0.499    | 1       | 62.2     |
|                                | orjson+jsonpath-ng | 0.654    | 1       | 62.2     |
|                                | orjson+jmespath    | 0.678    | 1       | 62.2     |
| nested:$.records\[\*\].id      | strata             | 0.368    | 500     | 62.2     |
|                                | orjson+jmespath    | 2.163    | 500     | 62.2     |
|                                | orjson+jsonpath-ng | 2.601    | 500     | 62.2     |
| nested:$.records\[0\]          | strata             | 0.374    | 1       | 62.2     |
|                                | orjson+jmespath    | 1.597    | 1       | 62.2     |
|                                | orjson+jsonpath-ng | 1.604    | 1       | 62.2     |
| wide_arrays:$.records\[\*\].id | strata             | 2.726    | 500     | 77.2     |
|                                | orjson+jmespath    | 4.705    | 500     | 77.2     |
|                                | orjson+jsonpath-ng | 5.508    | 500     | 77.2     |
| wide_arrays:$.records\[0\]     | strata             | 2.934    | 1       | 77.2     |
|                                | orjson+jmespath    | 4.211    | 1       | 77.2     |
|                                | orjson+jsonpath-ng | 4.382    | 1       | 77.2     |
| mixed:$.records\[\*\].id       | strata             | 0.577    | 500     | 77.2     |
|                                | orjson+jmespath    | 0.788    | 500     | 77.2     |
|                                | orjson+jsonpath-ng | 2.416    | 500     | 77.2     |
| mixed:$.records\[0\]           | strata             | 0.527    | 1       | 77.2     |
|                                | orjson+jmespath    | 0.611    | 1       | 77.2     |
|                                | orjson+jsonpath-ng | 0.689    | 1       | 77.2     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.068    | 1000    | 56.5     |
|                                            | jmespath    | 0.402    | 1000    | 56.5     |
|                                            | jsonpath-ng | 4.720    | 1000    | 56.5     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.942    | 12046   | 56.6     |
|                                            | jmespath    | 11.921   | 1000    | 56.6     |
|                                            | jsonpath-ng | 34.387   | 12046   | 59.2     |
| $..price                                   | strata      | 1.308    | 12046   | 57.3     |
|                                            | jsonpath-ng | 126.031  | 12046   | 60.3     |
| $.users\[?(@.age>30)\]                     | strata      | 0.166    | 794     | 57.6     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.532    | 1604    | 57.7     |
| flat:$.records\[\*\].id                    | strata      | 0.033    | 500     | 62.2     |
|                                            | jmespath    | 0.175    | 500     | 62.2     |
|                                            | jsonpath-ng | 1.791    | 500     | 62.2     |
| flat:$.records\[0\]                        | jmespath    | 0.006    | 1       | 62.2     |
|                                            | jsonpath-ng | 0.008    | 1       | 62.2     |
|                                            | strata      | 0.011    | 1       | 62.2     |
| nested:$.records\[\*\].id                  | strata      | 0.026    | 500     | 62.2     |
|                                            | jmespath    | 0.171    | 500     | 62.2     |
|                                            | jsonpath-ng | 1.848    | 500     | 62.2     |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 62.2     |
|                                            | jmespath    | 0.006    | 1       | 62.2     |
|                                            | jsonpath-ng | 0.008    | 1       | 62.2     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.036    | 500     | 77.2     |
|                                            | jmespath    | 0.177    | 500     | 77.2     |
|                                            | jsonpath-ng | 2.809    | 500     | 77.2     |
| wide_arrays:$.records\[0\]                 | strata      | 0.006    | 1       | 77.2     |
|                                            | jmespath    | 0.007    | 1       | 77.2     |
|                                            | jsonpath-ng | 0.009    | 1       | 77.2     |
| mixed:$.records\[\*\].id                   | strata      | 0.038    | 500     | 77.2     |
|                                            | jmespath    | 0.187    | 500     | 77.2     |
|                                            | jsonpath-ng | 2.026    | 500     | 77.2     |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 77.2     |
|                                            | jmespath    | 0.007    | 1       | 77.2     |
|                                            | jsonpath-ng | 0.011    | 1       | 77.2     |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **21.0% faster** than #2 (msgspec) |
| loads (NDJSON)      | **#1** / 5              | **23.0% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **33.1% faster** than #2 (msgspec) |
| load (NDJSON file)  | **#1** / 5              | **20.7% faster** than #2 (msgspec) |
| dumps               | **#1** / 5              | **40.7% faster** than #2 (orjson)  |
| dump                | **#1** / 5              | **26.2% faster** than #2 (msgspec) |
| loads (flat)        | **#3** / 5              | 41.3% behind #1 (orjson)           |
| load (flat)         | **#3** / 5              | 21.1% behind #1 (msgspec)          |
| dumps (flat)        | **#2** / 5              | 1.9% behind #1 (orjson)            |
| dump (flat)         | **#1** / 5              | **13.1% faster** than #2 (orjson)  |
| loads (nested)      | **#1** / 5              | **148.7% faster** than #2 (orjson) |
| load (nested)       | **#1** / 5              | **170.0% faster** than #2 (orjson) |
| dumps (nested)      | **#1** / 5              | **6.0% faster** than #2 (orjson)   |
| dump (nested)       | **#1** / 5              | **23.9% faster** than #2 (msgspec) |
| loads (wide_arrays) | **#2** / 5              | 3.0% behind #1 (orjson)            |
| load (wide_arrays)  | **#1** / 5              | **18.0% faster** than #2 (orjson)  |
| dumps (wide_arrays) | **#1** / 5              | **13.8% faster** than #2 (orjson)  |
| dump (wide_arrays)  | **#1** / 5              | **5.6% faster** than #2 (orjson)   |
| loads (mixed)       | **#3** / 5              | 44.6% behind #1 (orjson)           |
| load (mixed)        | **#3** / 5              | 11.2% behind #1 (orjson)           |
| dumps (mixed)       | **#1** / 5              | **5.1% faster** than #2 (orjson)   |
| dump (mixed)        | **#1** / 5              | **16.6% faster** than #2 (orjson)  |
| search (JSONPath)   | **#1** in 11/11 queries | avg **684% faster**                |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
