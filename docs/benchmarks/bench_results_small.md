# Strata Benchmark Results

Generated: 2026-03-18 00:39:37

## Environment

- Commit: 5b6b37b39dfd1e18b3f00b8af9c26aa1e984d69c
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
| orjson        | flat         | 0.540    | 0.687       | 0.990    | 61.2     |
| msgspec       | flat         | 0.624    | 0.766       | 0.785    | 61.2     |
| strata        | flat         | 0.746    | 0.795       | 0.854    | 60.5     |
| ujson         | flat         | 1.054    | 1.240       | 1.248    | 61.2     |
| json (stdlib) | flat         | 2.124    | 2.188       | 2.318    | 61.2     |
| orjson        | mixed        | 0.473    | 0.621       | 0.629    | 81.3     |
| msgspec       | mixed        | 0.607    | 0.723       | 0.751    | 81.3     |
| strata        | mixed        | 0.734    | 0.879       | 0.884    | 81.3     |
| ujson         | mixed        | 1.022    | 1.232       | 1.359    | 81.3     |
| json (stdlib) | mixed        | 2.193    | 2.232       | 2.348    | 81.3     |
| strata        | nested       | 0.675    | 0.845       | 0.957    | 61.3     |
| orjson        | nested       | 1.710    | 1.775       | 1.981    | 61.3     |
| msgspec       | nested       | 1.561    | 1.879       | 2.061    | 61.3     |
| ujson         | nested       | 2.276    | 2.294       | 2.523    | 61.3     |
| json (stdlib) | nested       | 2.702    | 2.714       | 2.942    | 61.3     |
| strata        | users.json   | 3.897    | 5.258       | 5.353    | 34.2     |
| orjson        | users.json   | 4.908    | 6.309       | 7.309    | 38.2     |
| msgspec       | users.json   | 4.966    | 7.468       | 8.351    | 41.4     |
| ujson         | users.json   | 7.050    | 8.726       | 9.217    | 41.2     |
| json (stdlib) | users.json   | 10.303   | 11.225      | 13.150   | 44.2     |
| strata        | users.ndjson | 4.012    | 4.904       | 5.362    | 55.9     |
| orjson        | users.ndjson | 4.491    | 6.615       | 7.567    | 56.9     |
| msgspec       | users.ndjson | 4.638    | 6.702       | 9.216    | 59.0     |
| ujson         | users.ndjson | 7.983    | 8.856       | 9.767    | 59.0     |
| json (stdlib) | users.ndjson | 9.571    | 11.535      | 12.075   | 59.0     |
| strata        | wide_arrays  | 3.290    | 3.968       | 3.996    | 63.9     |
| orjson        | wide_arrays  | 3.464    | 4.581       | 4.870    | 66.5     |
| msgspec       | wide_arrays  | 5.759    | 6.363       | 7.404    | 67.4     |
| ujson         | wide_arrays  | 7.606    | 7.715       | 8.440    | 67.4     |
| json (stdlib) | wide_arrays  | 14.294   | 14.568      | 15.965   | 68.7     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.666    | 0.744       | 0.787    | 61.2     |
| msgspec       | flat         | 0.734    | 0.815       | 0.827    | 61.2     |
| strata        | flat         | 0.783    | 0.872       | 0.885    | 61.2     |
| ujson         | flat         | 1.277    | 1.422       | 1.655    | 61.2     |
| json (stdlib) | flat         | 2.393    | 2.400       | 2.623    | 61.2     |
| orjson        | mixed        | 0.580    | 0.714       | 0.794    | 81.3     |
| msgspec       | mixed        | 0.657    | 0.792       | 0.813    | 81.3     |
| strata        | mixed        | 1.015    | 1.062       | 1.204    | 81.3     |
| ujson         | mixed        | 1.165    | 1.304       | 1.342    | 81.3     |
| json (stdlib) | mixed        | 2.214    | 2.293       | 2.333    | 81.3     |
| strata        | nested       | 0.760    | 1.038       | 1.054    | 61.3     |
| orjson        | nested       | 1.851    | 1.971       | 2.287    | 61.3     |
| msgspec       | nested       | 1.599    | 2.321       | 2.336    | 61.3     |
| ujson         | nested       | 2.151    | 2.331       | 2.640    | 61.3     |
| json (stdlib) | nested       | 2.765    | 3.003       | 3.099    | 61.3     |
| strata        | users.json   | 4.027    | 4.893       | 5.111    | 44.2     |
| orjson        | users.json   | 4.644    | 6.115       | 6.279    | 45.2     |
| msgspec       | users.json   | 4.928    | 6.506       | 7.586    | 49.2     |
| ujson         | users.json   | 6.294    | 8.903       | 9.781    | 49.2     |
| json (stdlib) | users.json   | 9.452    | 14.388      | 14.651   | 49.2     |
| strata        | users.ndjson | 4.251    | 5.011       | 5.065    | 59.0     |
| orjson        | users.ndjson | 5.011    | 6.271       | 6.555    | 59.0     |
| msgspec       | users.ndjson | 5.692    | 7.697       | 10.271   | 60.0     |
| ujson         | users.ndjson | 6.695    | 9.004       | 9.103    | 60.0     |
| json (stdlib) | users.ndjson | 9.644    | 11.940      | 13.254   | 60.0     |
| strata        | wide_arrays  | 3.516    | 4.126       | 4.194    | 69.0     |
| orjson        | wide_arrays  | 4.168    | 5.729       | 5.799    | 70.3     |
| msgspec       | wide_arrays  | 5.470    | 6.065       | 6.173    | 75.6     |
| ujson         | wide_arrays  | 7.563    | 8.369       | 10.494   | 75.6     |
| json (stdlib) | wide_arrays  | 15.066   | 15.867      | 16.240   | 75.6     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.246    | 0.246       | 0.247    | 61.2     |
| orjson  | flat        | 0.256    | 0.264       | 0.265    | 61.2     |
| msgspec | flat        | 0.315    | 0.326       | 0.383    | 61.2     |
| ujson   | flat        | 1.259    | 1.277       | 1.376    | 61.2     |
| json    | flat        | 2.306    | 2.378       | 2.437    | 61.2     |
| strata  | mixed       | 0.262    | 0.262       | 0.267    | 81.3     |
| orjson  | mixed       | 0.278    | 0.280       | 0.281    | 81.3     |
| msgspec | mixed       | 0.307    | 0.311       | 0.314    | 81.3     |
| ujson   | mixed       | 1.301    | 1.305       | 1.307    | 81.3     |
| json    | mixed       | 3.001    | 3.004       | 3.009    | 81.3     |
| strata  | nested      | 0.184    | 0.186       | 0.187    | 61.3     |
| orjson  | nested      | 0.195    | 0.196       | 0.196    | 61.3     |
| msgspec | nested      | 0.201    | 0.210       | 0.210    | 61.3     |
| ujson   | nested      | 1.010    | 1.010       | 1.012    | 61.3     |
| json    | nested      | 1.395    | 1.397       | 1.410    | 61.3     |
| strata  | users.json  | 1.117    | 1.118       | 1.223    | 49.2     |
| msgspec | users.json  | 1.477    | 1.493       | 1.520    | 51.1     |
| orjson  | users.json  | 1.521    | 1.596       | 2.150    | 51.0     |
| ujson   | users.json  | 6.357    | 6.481       | 6.745    | 52.2     |
| json    | users.json  | 8.802    | 8.809       | 9.294    | 52.6     |
| strata  | wide_arrays | 1.375    | 1.413       | 2.294    | 76.8     |
| orjson  | wide_arrays | 1.535    | 1.549       | 1.649    | 78.7     |
| msgspec | wide_arrays | 2.102    | 2.110       | 2.240    | 81.2     |
| ujson   | wide_arrays | 7.161    | 7.224       | 8.006    | 81.2     |
| json    | wide_arrays | 20.503   | 20.876      | 22.063   | 81.3     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.389    | 0.437       | 0.441    | 61.2     |
| orjson        | flat        | 0.476    | 0.494       | 0.622    | 61.2     |
| msgspec       | flat        | 0.541    | 0.760       | 1.076    | 61.2     |
| ujson         | flat        | 1.461    | 1.485       | 2.161    | 61.2     |
| json (stdlib) | flat        | 8.418    | 8.471       | 8.494    | 61.2     |
| msgspec       | mixed       | 0.492    | 0.501       | 0.596    | 81.3     |
| strata        | mixed       | 0.399    | 0.587       | 0.673    | 81.3     |
| orjson        | mixed       | 0.667    | 0.733       | 0.753    | 81.3     |
| ujson         | mixed       | 1.515    | 1.543       | 1.552    | 81.3     |
| json (stdlib) | mixed       | 10.007   | 10.011      | 10.023   | 81.3     |
| strata        | nested      | 0.308    | 0.356       | 0.442    | 61.3     |
| msgspec       | nested      | 0.365    | 0.437       | 0.507    | 61.3     |
| orjson        | nested      | 0.410    | 0.446       | 0.618    | 61.3     |
| ujson         | nested      | 1.236    | 1.263       | 1.278    | 61.3     |
| json (stdlib) | nested      | 11.978   | 12.110      | 13.091   | 61.3     |
| strata        | users.json  | 1.247    | 1.285       | 1.371    | 52.6     |
| orjson        | users.json  | 1.794    | 1.971       | 2.005    | 52.6     |
| msgspec       | users.json  | 1.699    | 2.170       | 2.239    | 52.6     |
| ujson         | users.json  | 6.854    | 6.912       | 6.991    | 52.6     |
| json (stdlib) | users.json  | 67.828   | 68.714      | 69.376   | 52.6     |
| strata        | wide_arrays | 1.779    | 1.837       | 3.274    | 81.3     |
| msgspec       | wide_arrays | 2.140    | 2.143       | 2.163    | 81.3     |
| orjson        | wide_arrays | 2.054    | 2.344       | 2.420    | 81.3     |
| ujson         | wide_arrays | 7.649    | 7.824       | 8.266    | 81.3     |
| json (stdlib) | wide_arrays | 44.414   | 45.165      | 45.225   | 81.3     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.938    | 1000    | 52.7     |
|                                | orjson+jmespath    | 5.695    | 1000    | 52.8     |
|                                | orjson+jsonpath-ng | 7.703    | 1000    | 53.4     |
| all item prices                | strata             | 2.040    | 12046   | 53.4     |
|                                | orjson+jmespath    | 13.724   | 1000    | 53.4     |
|                                | orjson+jsonpath-ng | 36.071   | 12046   | 53.3     |
| recursive price                | strata             | 2.215    | 12046   | 53.0     |
|                                | orjson+jsonpath-ng | 128.356  | 12046   | 53.8     |
| flat:$.records\[\*\].id        | strata             | 0.486    | 500     | 61.2     |
|                                | orjson+jmespath    | 0.833    | 500     | 61.2     |
|                                | orjson+jsonpath-ng | 2.547    | 500     | 61.2     |
| flat:$.records\[0\]            | strata             | 0.613    | 1       | 61.2     |
|                                | orjson+jmespath    | 0.649    | 1       | 61.2     |
|                                | orjson+jsonpath-ng | 0.656    | 1       | 61.2     |
| nested:$.records\[\*\].id      | strata             | 0.364    | 500     | 61.3     |
|                                | orjson+jmespath    | 2.116    | 500     | 61.3     |
|                                | orjson+jsonpath-ng | 2.457    | 500     | 61.3     |
| nested:$.records\[0\]          | strata             | 0.375    | 1       | 61.3     |
|                                | orjson+jmespath    | 1.620    | 1       | 61.3     |
|                                | orjson+jsonpath-ng | 1.630    | 1       | 61.3     |
| wide_arrays:$.records\[\*\].id | strata             | 2.670    | 500     | 81.3     |
|                                | orjson+jmespath    | 4.092    | 500     | 81.3     |
|                                | orjson+jsonpath-ng | 5.310    | 500     | 81.3     |
| wide_arrays:$.records\[0\]     | strata             | 2.663    | 1       | 81.3     |
|                                | orjson+jsonpath-ng | 4.014    | 1       | 81.3     |
|                                | orjson+jmespath    | 4.757    | 1       | 81.3     |
| mixed:$.records\[\*\].id       | strata             | 0.511    | 500     | 81.3     |
|                                | orjson+jmespath    | 0.787    | 500     | 81.3     |
|                                | orjson+jsonpath-ng | 2.349    | 500     | 81.3     |
| mixed:$.records\[0\]           | strata             | 0.517    | 1       | 81.4     |
|                                | orjson+jsonpath-ng | 0.620    | 1       | 81.4     |
|                                | orjson+jmespath    | 0.624    | 1       | 81.4     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.081    | 1000    | 54.7     |
|                                            | jmespath    | 0.396    | 1000    | 54.7     |
|                                            | jsonpath-ng | 3.788    | 1000    | 54.7     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.931    | 12046   | 54.8     |
|                                            | jmespath    | 11.019   | 1000    | 54.8     |
|                                            | jsonpath-ng | 32.783   | 12046   | 57.5     |
| $..price                                   | strata      | 1.304    | 12046   | 55.5     |
|                                            | jsonpath-ng | 128.688  | 12046   | 58.5     |
| $.users\[?(@.age>30)\]                     | strata      | 0.123    | 794     | 55.9     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.473    | 1604    | 55.9     |
| flat:$.records\[\*\].id                    | strata      | 0.035    | 500     | 61.2     |
|                                            | jmespath    | 0.186    | 500     | 61.2     |
|                                            | jsonpath-ng | 1.777    | 500     | 61.2     |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 61.2     |
|                                            | jmespath    | 0.007    | 1       | 61.2     |
|                                            | jsonpath-ng | 0.007    | 1       | 61.2     |
| nested:$.records\[\*\].id                  | strata      | 0.027    | 500     | 61.3     |
|                                            | jmespath    | 0.179    | 500     | 61.3     |
|                                            | jsonpath-ng | 1.949    | 500     | 61.3     |
| nested:$.records\[0\]                      | strata      | 0.004    | 1       | 61.3     |
|                                            | jsonpath-ng | 0.006    | 1       | 61.3     |
|                                            | jmespath    | 0.007    | 1       | 61.3     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.038    | 500     | 81.3     |
|                                            | jmespath    | 0.194    | 500     | 81.3     |
|                                            | jsonpath-ng | 2.259    | 500     | 81.3     |
| wide_arrays:$.records\[0\]                 | strata      | 0.003    | 1       | 81.3     |
|                                            | jmespath    | 0.007    | 1       | 81.3     |
|                                            | jsonpath-ng | 0.008    | 1       | 81.3     |
| mixed:$.records\[\*\].id                   | strata      | 0.030    | 500     | 81.4     |
|                                            | jmespath    | 0.189    | 500     | 81.4     |
|                                            | jsonpath-ng | 2.304    | 500     | 81.4     |
| mixed:$.records\[0\]                       | jmespath    | 0.006    | 1       | 81.4     |
|                                            | jsonpath-ng | 0.007    | 1       | 81.4     |
|                                            | strata      | 0.011    | 1       | 81.4     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **25.9% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **11.9% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **15.3% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **17.9% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **32.2% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **36.3% faster** than #2 (msgspec)  |
| loads (flat)        | **#3** / 5              | 38.2% behind #1 (orjson)            |
| load (flat)         | **#3** / 5              | 17.6% behind #1 (orjson)            |
| dumps (flat)        | **#1** / 5              | **4.1% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **22.1% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **131.3% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **110.5% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **5.9% faster** than #2 (orjson)    |
| dump (nested)       | **#1** / 5              | **18.4% faster** than #2 (msgspec)  |
| loads (wide_arrays) | **#1** / 5              | **5.3% faster** than #2 (orjson)    |
| load (wide_arrays)  | **#1** / 5              | **18.6% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **11.6% faster** than #2 (orjson)   |
| dump (wide_arrays)  | **#1** / 5              | **15.5% faster** than #2 (orjson)   |
| loads (mixed)       | **#3** / 5              | 55.1% behind #1 (orjson)            |
| load (mixed)        | **#3** / 5              | 75.2% behind #1 (orjson)            |
| dumps (mixed)       | **#1** / 5              | **6.2% faster** than #2 (orjson)    |
| dump (mixed)        | **#1** / 5              | **23.2% faster** than #2 (msgspec)  |
| search (JSONPath)   | **#1** in 11/11 queries | avg **684% faster**                 |
| query (JSONPath)    | **#1** in 12/13 queries | -                                   |
