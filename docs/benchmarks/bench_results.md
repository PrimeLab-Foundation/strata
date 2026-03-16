# Strata Benchmark Results

Generated: 2026-03-16 02:12:43

## Environment

- Commit: aa99eea214c1ca13045f33e759bcacb6508755a4
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/users.json
- Random schemas: flat, nested, wide_arrays, mixed (500 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| pysimdjson    | flat        | ERROR    | -           | -        | -        |
| orjson        | flat        | 0.535    | 0.636       | 0.637    | 0.0      |
| msgspec       | flat        | 0.579    | 0.676       | 0.711    | 0.0      |
| strata        | flat        | 0.596    | 0.697       | 0.726    | 0.0      |
| ujson         | flat        | 0.994    | 1.146       | 1.153    | 0.0      |
| json (stdlib) | flat        | 1.969    | 2.066       | 2.074    | 0.0      |
| pysimdjson    | mixed       | ERROR    | -           | -        | -        |
| orjson        | mixed       | 0.459    | 0.665       | 0.780    | 0.0      |
| msgspec       | mixed       | 0.572    | 0.719       | 0.786    | 0.0      |
| strata        | mixed       | 0.641    | 0.771       | 0.854    | 0.0      |
| ujson         | mixed       | 1.005    | 1.260       | 1.354    | 0.0      |
| json (stdlib) | mixed       | 2.029    | 2.110       | 2.183    | 0.0      |
| pysimdjson    | nested      | ERROR    | -           | -        | -        |
| strata        | nested      | 0.570    | 0.719       | 0.744    | 0.0      |
| msgspec       | nested      | 1.448    | 1.569       | 1.664    | 0.0      |
| ujson         | nested      | 1.655    | 2.125       | 2.439    | 0.0      |
| json (stdlib) | nested      | 2.220    | 2.410       | 2.416    | 0.0      |
| orjson        | nested      | 1.807    | 3.126       | 3.233    | 0.0      |
| pysimdjson    | users.json  | ERROR    | -           | -        | -        |
| strata        | users.json  | 18.769   | 24.368      | 24.988   | 0.0      |
| msgspec       | users.json  | 24.400   | 37.339      | 41.087   | 0.0      |
| orjson        | users.json  | 30.313   | 38.036      | 39.555   | 0.0      |
| ujson         | users.json  | 37.313   | 50.275      | 51.168   | 0.0      |
| json (stdlib) | users.json  | 53.125   | 59.484      | 75.212   | 0.0      |
| pysimdjson    | wide_arrays | ERROR    | -           | -        | -        |
| strata        | wide_arrays | 2.937    | 3.519       | 3.607    | 0.0      |
| orjson        | wide_arrays | 3.123    | 4.002       | 4.145    | 0.0      |
| msgspec       | wide_arrays | 4.115    | 4.781       | 4.792    | 0.0      |
| ujson         | wide_arrays | 7.517    | 8.351       | 8.443    | 0.0      |
| json (stdlib) | wide_arrays | 12.923   | 13.742      | 15.896   | 0.0      |

## load (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 0.625    | 0.728       | 0.734    | 0.0      |
| msgspec       | flat        | 0.669    | 0.760       | 0.783    | 0.0      |
| strata        | flat        | 0.687    | 0.765       | 0.773    | 0.0      |
| ujson         | flat        | 1.130    | 1.294       | 1.301    | 0.0      |
| json (stdlib) | flat        | 2.065    | 2.310       | 2.413    | 0.0      |
| orjson        | mixed       | 0.557    | 0.668       | 0.716    | 0.0      |
| strata        | mixed       | 0.733    | 0.839       | 0.852    | 0.0      |
| msgspec       | mixed       | 0.745    | 0.939       | 0.957    | 0.0      |
| ujson         | mixed       | 1.061    | 1.240       | 1.241    | 0.0      |
| json (stdlib) | mixed       | 2.252    | 2.302       | 2.419    | 0.0      |
| strata        | nested      | 0.614    | 0.693       | 0.698    | 0.0      |
| orjson        | nested      | 1.473    | 1.661       | 1.738    | 0.0      |
| msgspec       | nested      | 1.544    | 1.696       | 1.820    | 0.0      |
| json (stdlib) | nested      | 2.316    | 2.524       | 2.777    | 0.0      |
| ujson         | nested      | 1.718    | 2.578       | 2.760    | 0.0      |
| strata        | users.json  | 19.156   | 23.799      | 27.521   | 0.0      |
| msgspec       | users.json  | 25.019   | 33.911      | 37.590   | 0.0      |
| orjson        | users.json  | 24.468   | 46.790      | 65.248   | 0.0      |
| json (stdlib) | users.json  | 43.814   | 55.179      | 70.437   | 0.0      |
| ujson         | users.json  | 34.927   | 56.235      | 62.432   | 0.0      |
| strata        | wide_arrays | 3.161    | 3.796       | 4.093    | 0.0      |
| orjson        | wide_arrays | 3.536    | 4.298       | 4.386    | 0.0      |
| msgspec       | wide_arrays | 4.388    | 5.210       | 5.255    | 0.0      |
| ujson         | wide_arrays | 6.745    | 7.577       | 7.688    | 0.0      |
| json (stdlib) | wide_arrays | 13.964   | 14.010      | 14.165   | 0.0      |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.195    | 0.197       | 0.197    | 0.0      |
| strata  | flat        | 0.280    | 0.285       | 0.287    | 0.0      |
| msgspec | flat        | 0.300    | 0.303       | 0.310    | 0.0      |
| ujson   | flat        | 1.138    | 1.147       | 1.150    | 0.0      |
| json    | flat        | 2.189    | 2.190       | 2.194    | 0.0      |
| orjson  | mixed       | 0.282    | 0.285       | 0.287    | 0.0      |
| strata  | mixed       | 0.309    | 0.311       | 0.316    | 0.0      |
| msgspec | mixed       | 0.309    | 0.365       | 0.366    | 0.0      |
| ujson   | mixed       | 1.210    | 1.233       | 1.263    | 0.0      |
| json    | mixed       | 2.870    | 2.871       | 2.891    | 0.0      |
| orjson  | nested      | 0.157    | 0.157       | 0.178    | 0.0      |
| strata  | nested      | 0.203    | 0.203       | 0.212    | 0.0      |
| msgspec | nested      | 0.197    | 0.233       | 0.293    | 0.0      |
| ujson   | nested      | 1.039    | 1.041       | 1.069    | 0.0      |
| json    | nested      | 1.196    | 1.199       | 1.216    | 0.0      |
| orjson  | users.json  | 6.171    | 6.212       | 6.265    | 0.0      |
| strata  | users.json  | 6.430    | 6.520       | 7.181    | 0.0      |
| msgspec | users.json  | 8.281    | 8.654       | 9.231    | 0.0      |
| ujson   | users.json  | 30.854   | 31.068      | 32.978   | 0.0      |
| json    | users.json  | 43.954   | 43.983      | 44.034   | 0.0      |
| orjson  | wide_arrays | 1.594    | 1.595       | 1.757    | 0.0      |
| msgspec | wide_arrays | 1.576    | 1.682       | 1.715    | 0.0      |
| strata  | wide_arrays | 1.768    | 1.769       | 1.773    | 0.0      |
| ujson   | wide_arrays | 6.482    | 6.813       | 6.877    | 0.0      |
| json    | wide_arrays | 19.302   | 19.333      | 19.333   | 0.0      |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 0.393    | 0.438       | 0.618    | 0.0      |
| msgspec       | flat        | 0.474    | 0.508       | 0.529    | 0.0      |
| strata        | flat        | 0.437    | 0.536       | 0.689    | 0.0      |
| ujson         | flat        | 1.393    | 1.413       | 1.444    | 0.0      |
| json (stdlib) | flat        | 8.040    | 8.379       | 8.447    | 0.0      |
| strata        | mixed       | 0.426    | 0.457       | 0.465    | 0.0      |
| msgspec       | mixed       | 0.458    | 0.465       | 0.484    | 0.0      |
| orjson        | mixed       | 0.494    | 0.503       | 0.520    | 0.0      |
| ujson         | mixed       | 1.406    | 1.426       | 1.481    | 0.0      |
| json (stdlib) | mixed       | 9.807    | 9.869       | 10.322   | 0.0      |
| orjson        | nested      | 0.317    | 0.345       | 0.350    | 0.0      |
| strata        | nested      | 0.335    | 0.348       | 0.375    | 0.0      |
| msgspec       | nested      | 0.353    | 0.364       | 0.376    | 0.0      |
| ujson         | nested      | 1.119    | 1.147       | 1.170    | 0.0      |
| json (stdlib) | nested      | 11.812   | 11.934      | 12.068   | 0.0      |
| orjson        | users.json  | 7.449    | 7.701       | 10.990   | 0.0      |
| msgspec       | users.json  | 9.290    | 9.356       | 11.571   | 0.0      |
| strata        | users.json  | 11.450   | 11.868      | 12.054   | 0.0      |
| ujson         | users.json  | 32.577   | 32.715      | 33.079   | 0.0      |
| json (stdlib) | users.json  | 344.731  | 350.193     | 351.244  | 0.0      |
| msgspec       | wide_arrays | 1.962    | 2.044       | 2.762    | 0.0      |
| strata        | wide_arrays | 2.010    | 2.430       | 2.571    | 0.0      |
| orjson        | wide_arrays | 2.414    | 2.606       | 2.896    | 0.0      |
| ujson         | wide_arrays | 7.583    | 7.916       | 7.976    | 0.0      |
| json (stdlib) | wide_arrays | 44.275   | 46.274      | 49.439   | 0.0      |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 9.612    | 5000    | 0.0      |
|                                | orjson+jmespath    | 30.032   | 5000    | 0.0      |
|                                | orjson+jsonpath-ng | 36.979   | 5000    | 0.0      |
| all item prices                | strata             | 10.902   | 62331   | 0.0      |
|                                | orjson+jmespath    | 72.263   | 5000    | 0.0      |
|                                | orjson+jsonpath-ng | 221.580  | 62331   | 0.0      |
| recursive price                | strata             | 10.380   | 62331   | 0.0      |
|                                | orjson+jsonpath-ng | 727.968  | 62331   | 0.0      |
| flat:$.records\[\*\].id        | strata             | 0.495    | 500     | 0.0      |
|                                | orjson+jmespath    | 0.817    | 500     | 0.0      |
|                                | orjson+jsonpath-ng | 2.154    | 500     | 0.0      |
| flat:$.records\[0\]            | strata             | 0.455    | 1       | 0.0      |
|                                | orjson+jmespath    | 0.640    | 1       | 0.0      |
|                                | orjson+jsonpath-ng | 0.650    | 1       | 0.0      |
| nested:$.records\[\*\].id      | strata             | 0.431    | 500     | 0.0      |
|                                | orjson+jmespath    | 1.656    | 500     | 0.0      |
|                                | orjson+jsonpath-ng | 2.358    | 500     | 0.0      |
| nested:$.records\[0\]          | strata             | 0.357    | 1       | 0.0      |
|                                | orjson+jmespath    | 1.488    | 1       | 0.0      |
|                                | orjson+jsonpath-ng | 1.698    | 1       | 0.0      |
| wide_arrays:$.records\[\*\].id | strata             | 2.746    | 500     | 0.0      |
|                                | orjson+jmespath    | 4.372    | 500     | 0.0      |
|                                | orjson+jsonpath-ng | 5.105    | 500     | 0.0      |
| wide_arrays:$.records\[0\]     | strata             | 2.588    | 1       | 0.0      |
|                                | orjson+jsonpath-ng | 3.989    | 1       | 0.0      |
|                                | orjson+jmespath    | 4.031    | 1       | 0.0      |
| mixed:$.records\[\*\].id       | strata             | 0.521    | 500     | 0.0      |
|                                | orjson+jmespath    | 0.782    | 500     | 0.0      |
|                                | orjson+jsonpath-ng | 2.242    | 500     | 0.0      |
| mixed:$.records\[0\]           | strata             | 0.503    | 1       | 0.0      |
|                                | orjson+jmespath    | 0.597    | 1       | 0.0      |
|                                | orjson+jsonpath-ng | 0.608    | 1       | 0.0      |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.983    | 5000    | 0.0      |
|                                            | jmespath    | 2.859    | 5000    | 0.0      |
|                                            | jsonpath-ng | 23.100   | 5000    | 0.0      |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 5.344    | 62331   | 0.0      |
|                                            | jmespath    | 56.934   | 5000    | 0.0      |
|                                            | jsonpath-ng | 245.926  | 62331   | 0.0      |
| $..price                                   | strata      | 6.597    | 62331   | 0.0      |
|                                            | jsonpath-ng | 774.766  | 62331   | 0.0      |
| $.users\[?(@.age>30)\]                     | strata      | 0.920    | 3999    | 0.0      |
| $..orders\[?(@.status=="shipped")\]        | strata      | 7.965    | 8402    | 0.0      |
| flat:$.records\[\*\].id                    | strata      | 0.030    | 500     | 0.0      |
|                                            | jmespath    | 0.164    | 500     | 0.0      |
|                                            | jsonpath-ng | 1.481    | 500     | 0.0      |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 0.0      |
|                                            | jmespath    | 0.005    | 1       | 0.0      |
|                                            | jsonpath-ng | 0.011    | 1       | 0.0      |
| nested:$.records\[\*\].id                  | strata      | 0.039    | 500     | 0.0      |
|                                            | jmespath    | 0.210    | 500     | 0.0      |
|                                            | jsonpath-ng | 1.904    | 500     | 0.0      |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 0.0      |
|                                            | jmespath    | 0.007    | 1       | 0.0      |
|                                            | jsonpath-ng | 0.008    | 1       | 0.0      |
| wide_arrays:$.records\[\*\].id             | strata      | 0.028    | 500     | 0.0      |
|                                            | jmespath    | 0.180    | 500     | 0.0      |
|                                            | jsonpath-ng | 2.128    | 500     | 0.0      |
| wide_arrays:$.records\[0\]                 | strata      | 0.003    | 1       | 0.0      |
|                                            | jmespath    | 0.007    | 1       | 0.0      |
|                                            | jsonpath-ng | 0.008    | 1       | 0.0      |
| mixed:$.records\[\*\].id                   | strata      | 0.032    | 500     | 0.0      |
|                                            | jmespath    | 0.166    | 500     | 0.0      |
|                                            | jsonpath-ng | 1.593    | 500     | 0.0      |
| mixed:$.records\[0\]                       | strata      | 0.005    | 1       | 0.0      |
|                                            | jmespath    | 0.006    | 1       | 0.0      |
|                                            | jsonpath-ng | 0.012    | 1       | 0.0      |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **30.0% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **27.7% faster** than #2 (orjson)   |
| dumps               | **#2** / 5              | 4.2% behind #1 (orjson)             |
| dump                | **#3** / 5              | 53.7% behind #1 (orjson)            |
| loads (flat)        | **#3** / 5              | 11.5% behind #1 (orjson)            |
| load (flat)         | **#3** / 5              | 9.9% behind #1 (orjson)             |
| dumps (flat)        | **#2** / 5              | 43.7% behind #1 (orjson)            |
| dump (flat)         | **#2** / 5              | 11.1% behind #1 (orjson)            |
| loads (nested)      | **#1** / 5              | **154.0% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **139.9% faster** than #2 (orjson)  |
| dumps (nested)      | **#3** / 5              | 29.3% behind #1 (orjson)            |
| dump (nested)       | **#2** / 5              | 5.6% behind #1 (orjson)             |
| loads (wide_arrays) | **#1** / 5              | **6.3% faster** than #2 (orjson)    |
| load (wide_arrays)  | **#1** / 5              | **11.8% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#3** / 5              | 12.2% behind #1 (msgspec)           |
| dump (wide_arrays)  | **#2** / 5              | 2.4% behind #1 (msgspec)            |
| loads (mixed)       | **#3** / 5              | 39.6% behind #1 (orjson)            |
| load (mixed)        | **#2** / 5              | 31.5% behind #1 (orjson)            |
| dumps (mixed)       | **#2** / 5              | 9.4% behind #1 (orjson)             |
| dump (mixed)        | **#1** / 5              | **7.7% faster** than #2 (msgspec)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **780% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
