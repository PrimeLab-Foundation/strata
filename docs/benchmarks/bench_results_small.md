# Strata Benchmark Results

Generated: 2026-03-19 02:44:39

## Environment

- Commit: 42f0bfe95d82803fac6378a50057ee7f0b40daca
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
| orjson        | flat         | 0.690    | 0.766       | 0.836    | 61.5     |
| msgspec       | flat         | 0.742    | 0.914       | 0.915    | 61.5     |
| strata        | flat         | 0.860    | 0.985       | 1.066    | 60.9     |
| ujson         | flat         | 1.261    | 1.477       | 1.481    | 61.5     |
| json (stdlib) | flat         | 2.323    | 2.435       | 2.466    | 61.5     |
| orjson        | mixed        | 0.566    | 0.669       | 0.687    | 75.5     |
| msgspec       | mixed        | 0.704    | 0.788       | 0.850    | 75.5     |
| strata        | mixed        | 0.806    | 0.969       | 1.042    | 75.5     |
| ujson         | mixed        | 1.032    | 1.224       | 1.232    | 75.5     |
| json (stdlib) | mixed        | 2.382    | 2.471       | 2.802    | 75.5     |
| strata        | nested       | 0.787    | 0.952       | 0.957    | 61.6     |
| msgspec       | nested       | 1.819    | 1.970       | 2.000    | 61.6     |
| orjson        | nested       | 2.434    | 2.504       | 2.610    | 61.6     |
| ujson         | nested       | 2.533    | 2.579       | 2.879    | 61.6     |
| json (stdlib) | nested       | 2.702    | 2.828       | 2.985    | 61.6     |
| strata        | users.json   | 3.966    | 4.889       | 5.152    | 34.5     |
| orjson        | users.json   | 4.403    | 6.437       | 8.211    | 38.5     |
| msgspec       | users.json   | 6.378    | 8.278       | 8.851    | 41.7     |
| ujson         | users.json   | 6.366    | 8.852       | 9.724    | 41.5     |
| json (stdlib) | users.json   | 10.729   | 12.857      | 13.468   | 43.6     |
| strata        | users.ndjson | 4.566    | 5.319       | 6.370    | 56.3     |
| msgspec       | users.ndjson | 5.286    | 6.946       | 7.402    | 59.3     |
| orjson        | users.ndjson | 5.149    | 7.439       | 7.466    | 57.3     |
| ujson         | users.ndjson | 7.337    | 10.781      | 10.841   | 59.3     |
| json (stdlib) | users.ndjson | 10.417   | 12.624      | 12.658   | 59.3     |
| strata        | wide_arrays  | 3.332    | 4.007       | 4.141    | 63.5     |
| orjson        | wide_arrays  | 4.426    | 5.041       | 5.339    | 66.0     |
| msgspec       | wide_arrays  | 4.619    | 5.672       | 5.765    | 66.1     |
| ujson         | wide_arrays  | 8.052    | 8.359       | 8.435    | 66.1     |
| json (stdlib) | wide_arrays  | 15.406   | 16.025      | 16.245   | 67.4     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.825    | 0.897       | 0.918    | 61.5     |
| msgspec       | flat         | 0.803    | 0.925       | 0.941    | 61.5     |
| strata        | flat         | 0.896    | 1.042       | 1.052    | 61.5     |
| ujson         | flat         | 1.402    | 1.602       | 1.603    | 61.5     |
| json (stdlib) | flat         | 2.370    | 2.449       | 2.483    | 61.5     |
| orjson        | mixed        | 0.692    | 0.768       | 0.857    | 75.5     |
| msgspec       | mixed        | 0.697    | 0.835       | 0.858    | 75.5     |
| strata        | mixed        | 0.836    | 1.114       | 1.115    | 75.5     |
| ujson         | mixed        | 1.260    | 1.383       | 1.406    | 75.5     |
| json (stdlib) | mixed        | 2.327    | 2.438       | 2.561    | 75.5     |
| strata        | nested       | 0.854    | 0.926       | 0.949    | 61.6     |
| orjson        | nested       | 1.691    | 1.897       | 2.771    | 61.6     |
| msgspec       | nested       | 2.051    | 2.150       | 2.589    | 61.6     |
| ujson         | nested       | 2.162    | 2.626       | 3.488    | 61.6     |
| json (stdlib) | nested       | 4.028    | 4.792       | 4.914    | 61.6     |
| strata        | users.json   | 3.897    | 4.833       | 5.034    | 43.7     |
| msgspec       | users.json   | 5.280    | 6.464       | 6.932    | 48.7     |
| orjson        | users.json   | 4.994    | 7.164       | 7.429    | 44.6     |
| ujson         | users.json   | 6.929    | 9.704       | 11.368   | 48.7     |
| json (stdlib) | users.json   | 9.415    | 10.958      | 11.360   | 48.7     |
| strata        | users.ndjson | 4.860    | 5.537       | 6.181    | 59.3     |
| orjson        | users.ndjson | 5.637    | 6.764       | 7.515    | 59.3     |
| msgspec       | users.ndjson | 6.773    | 8.809       | 9.749    | 60.3     |
| ujson         | users.ndjson | 7.087    | 9.255       | 10.328   | 60.3     |
| json (stdlib) | users.ndjson | 10.559   | 13.347      | 15.049   | 60.3     |
| strata        | wide_arrays  | 3.705    | 3.830       | 4.041    | 67.4     |
| orjson        | wide_arrays  | 4.015    | 4.584       | 4.628    | 68.7     |
| msgspec       | wide_arrays  | 6.528    | 6.631       | 7.337    | 70.7     |
| ujson         | wide_arrays  | 8.837    | 10.612      | 11.760   | 70.7     |
| json (stdlib) | wide_arrays  | 16.097   | 17.203      | 17.729   | 70.7     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.267    | 0.267       | 0.275    | 61.5     |
| strata  | flat        | 0.278    | 0.283       | 0.300    | 61.5     |
| msgspec | flat        | 0.315    | 0.333       | 0.336    | 61.5     |
| ujson   | flat        | 1.324    | 1.350       | 1.451    | 61.5     |
| json    | flat        | 2.484    | 2.585       | 2.810    | 61.5     |
| orjson  | mixed       | 0.281    | 0.296       | 0.305    | 75.5     |
| strata  | mixed       | 0.291    | 0.298       | 0.342    | 75.5     |
| msgspec | mixed       | 0.320    | 0.341       | 0.344    | 75.5     |
| ujson   | mixed       | 1.368    | 1.378       | 1.536    | 75.5     |
| json    | mixed       | 3.006    | 3.013       | 3.061    | 75.5     |
| orjson  | nested      | 0.217    | 0.229       | 0.263    | 61.6     |
| strata  | nested      | 0.216    | 0.229       | 0.275    | 61.6     |
| msgspec | nested      | 0.223    | 0.250       | 0.256    | 61.6     |
| ujson   | nested      | 1.060    | 1.094       | 1.323    | 61.6     |
| json    | nested      | 1.515    | 1.519       | 1.534    | 61.6     |
| strata  | users.json  | 1.354    | 1.358       | 1.434    | 48.7     |
| msgspec | users.json  | 1.479    | 1.486       | 1.512    | 50.7     |
| orjson  | users.json  | 1.506    | 1.526       | 1.590    | 50.5     |
| ujson   | users.json  | 6.285    | 6.365       | 6.478    | 50.7     |
| json    | users.json  | 8.803    | 8.921       | 9.265    | 51.2     |
| msgspec | wide_arrays | 1.847    | 1.854       | 2.147    | 75.1     |
| orjson  | wide_arrays | 1.763    | 1.893       | 1.942    | 73.8     |
| strata  | wide_arrays | 1.785    | 1.905       | 1.936    | 71.9     |
| ujson   | wide_arrays | 7.612    | 8.180       | 8.192    | 75.4     |
| json    | wide_arrays | 21.303   | 21.405      | 22.452   | 75.5     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 0.438    | 0.452       | 0.453    | 61.5     |
| msgspec       | flat        | 0.473    | 0.478       | 0.484    | 61.5     |
| strata        | flat        | 0.502    | 0.513       | 0.531    | 61.5     |
| ujson         | flat        | 1.454    | 1.459       | 1.475    | 61.5     |
| json (stdlib) | flat        | 8.974    | 8.991       | 9.210    | 61.5     |
| orjson        | mixed       | 0.501    | 0.513       | 0.551    | 75.5     |
| strata        | mixed       | 0.508    | 0.514       | 0.675    | 75.5     |
| msgspec       | mixed       | 0.668    | 0.691       | 0.730    | 75.5     |
| ujson         | mixed       | 1.721    | 1.724       | 1.786    | 75.5     |
| json (stdlib) | mixed       | 11.450   | 11.709      | 11.780   | 75.5     |
| strata        | nested      | 0.388    | 0.409       | 0.448    | 61.6     |
| msgspec       | nested      | 0.452    | 0.468       | 0.506    | 61.6     |
| orjson        | nested      | 0.440    | 0.501       | 0.580    | 61.6     |
| ujson         | nested      | 1.324    | 1.348       | 1.456    | 61.6     |
| json (stdlib) | nested      | 13.743   | 13.874      | 14.017   | 61.6     |
| strata        | users.json  | 1.700    | 1.775       | 1.997    | 51.2     |
| orjson        | users.json  | 1.800    | 1.815       | 1.836    | 51.2     |
| msgspec       | users.json  | 1.816    | 1.933       | 2.015    | 52.0     |
| ujson         | users.json  | 6.691    | 6.729       | 7.414    | 52.0     |
| json (stdlib) | users.json  | 67.382   | 68.276      | 70.702   | 52.2     |
| msgspec       | wide_arrays | 2.321    | 2.331       | 2.463    | 75.5     |
| orjson        | wide_arrays | 2.325    | 2.372       | 3.058    | 75.5     |
| strata        | wide_arrays | 2.261    | 2.462       | 2.518    | 75.5     |
| ujson         | wide_arrays | 8.552    | 8.600       | 8.612    | 75.5     |
| json (stdlib) | wide_arrays | 49.733   | 50.843      | 62.707   | 75.5     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.944    | 1000    | 52.2     |
|                                | orjson+jmespath    | 5.687    | 1000    | 52.3     |
|                                | orjson+jsonpath-ng | 7.618    | 1000    | 52.8     |
| all item prices                | strata             | 2.104    | 12046   | 52.8     |
|                                | orjson+jmespath    | 13.412   | 1000    | 52.8     |
|                                | orjson+jsonpath-ng | 36.931   | 12046   | 52.8     |
| recursive price                | strata             | 2.240    | 12046   | 52.4     |
|                                | orjson+jsonpath-ng | 131.989  | 12046   | 53.2     |
| flat:$.records\[\*\].id        | strata             | 0.561    | 500     | 61.5     |
|                                | orjson+jmespath    | 1.036    | 500     | 61.5     |
|                                | orjson+jsonpath-ng | 2.707    | 500     | 61.5     |
| flat:$.records\[0\]            | strata             | 0.593    | 1       | 61.5     |
|                                | orjson+jsonpath-ng | 0.743    | 1       | 61.5     |
|                                | orjson+jmespath    | 0.776    | 1       | 61.5     |
| nested:$.records\[\*\].id      | strata             | 0.427    | 500     | 61.6     |
|                                | orjson+jmespath    | 1.995    | 500     | 61.6     |
|                                | orjson+jsonpath-ng | 2.521    | 500     | 61.6     |
| nested:$.records\[0\]          | strata             | 0.469    | 1       | 61.6     |
|                                | orjson+jsonpath-ng | 1.675    | 1       | 61.6     |
|                                | orjson+jmespath    | 1.717    | 1       | 61.6     |
| wide_arrays:$.records\[\*\].id | strata             | 2.468    | 500     | 75.5     |
|                                | orjson+jmespath    | 4.414    | 500     | 75.5     |
|                                | orjson+jsonpath-ng | 5.085    | 500     | 75.5     |
| wide_arrays:$.records\[0\]     | strata             | 2.530    | 1       | 75.5     |
|                                | orjson+jsonpath-ng | 4.771    | 1       | 75.5     |
|                                | orjson+jmespath    | 6.629    | 1       | 75.5     |
| mixed:$.records\[\*\].id       | strata             | 0.547    | 500     | 75.5     |
|                                | orjson+jmespath    | 0.895    | 500     | 75.5     |
|                                | orjson+jsonpath-ng | 2.606    | 500     | 75.5     |
| mixed:$.records\[0\]           | strata             | 0.606    | 1       | 75.5     |
|                                | orjson+jsonpath-ng | 0.685    | 1       | 75.5     |
|                                | orjson+jmespath    | 0.718    | 1       | 75.5     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.090    | 1000    | 55.2     |
|                                            | jmespath    | 0.443    | 1000    | 55.2     |
|                                            | jsonpath-ng | 5.071    | 1000    | 55.2     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 1.065    | 12046   | 55.2     |
|                                            | jmespath    | 12.155   | 1000    | 55.2     |
|                                            | jsonpath-ng | 33.600   | 12046   | 57.9     |
| $..price                                   | strata      | 1.373    | 12046   | 55.9     |
|                                            | jsonpath-ng | 124.508  | 12046   | 58.9     |
| $.users\[?(@.age>30)\]                     | strata      | 0.101    | 794     | 56.2     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.573    | 1604    | 56.3     |
| flat:$.records\[\*\].id                    | strata      | 0.039    | 500     | 61.5     |
|                                            | jmespath    | 0.216    | 500     | 61.5     |
|                                            | jsonpath-ng | 1.821    | 500     | 61.5     |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 61.5     |
|                                            | jmespath    | 0.006    | 1       | 61.5     |
|                                            | jsonpath-ng | 0.006    | 1       | 61.5     |
| nested:$.records\[\*\].id                  | strata      | 0.027    | 500     | 61.6     |
|                                            | jmespath    | 0.180    | 500     | 61.6     |
|                                            | jsonpath-ng | 1.727    | 500     | 61.6     |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 61.6     |
|                                            | jmespath    | 0.006    | 1       | 61.6     |
|                                            | jsonpath-ng | 0.015    | 1       | 61.6     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.037    | 500     | 75.5     |
|                                            | jmespath    | 0.211    | 500     | 75.5     |
|                                            | jsonpath-ng | 3.155    | 500     | 75.5     |
| wide_arrays:$.records\[0\]                 | strata      | 0.004    | 1       | 75.5     |
|                                            | jmespath    | 0.008    | 1       | 75.5     |
|                                            | jsonpath-ng | 0.010    | 1       | 75.5     |
| mixed:$.records\[\*\].id                   | strata      | 0.036    | 500     | 75.5     |
|                                            | jmespath    | 0.196    | 500     | 75.5     |
|                                            | jsonpath-ng | 1.844    | 500     | 75.5     |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 75.5     |
|                                            | jmespath    | 0.006    | 1       | 75.5     |
|                                            | jsonpath-ng | 0.007    | 1       | 75.5     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **11.0% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **12.8% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **28.1% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **16.0% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **9.2% faster** than #2 (msgspec)   |
| dump                | **#1** / 5              | **5.9% faster** than #2 (orjson)    |
| loads (flat)        | **#3** / 5              | 24.6% behind #1 (orjson)            |
| load (flat)         | **#3** / 5              | 11.6% behind #1 (msgspec)           |
| dumps (flat)        | **#2** / 5              | 4.2% behind #1 (orjson)             |
| dump (flat)         | **#3** / 5              | 14.7% behind #1 (orjson)            |
| loads (nested)      | **#1** / 5              | **131.0% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **98.0% faster** than #2 (orjson)   |
| dumps (nested)      | **#1** / 5              | **0.5% faster** than #2 (orjson)    |
| dump (nested)       | **#1** / 5              | **13.3% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#1** / 5              | **32.8% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **8.4% faster** than #2 (orjson)    |
| dumps (wide_arrays) | **#2** / 5              | 1.2% behind #1 (orjson)             |
| dump (wide_arrays)  | **#1** / 5              | **2.7% faster** than #2 (msgspec)   |
| loads (mixed)       | **#3** / 5              | 42.5% behind #1 (orjson)            |
| load (mixed)        | **#3** / 5              | 20.7% behind #1 (orjson)            |
| dumps (mixed)       | **#2** / 5              | 3.6% behind #1 (orjson)             |
| dump (mixed)        | **#2** / 5              | 1.5% behind #1 (orjson)             |
| search (JSONPath)   | **#1** in 11/11 queries | avg **682% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
