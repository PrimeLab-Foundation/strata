# Strata Benchmark Results

Generated: 2026-03-18 20:47:30

## Environment

- Commit: 6da7fbd55a862e7e748708f6f30c5b3b945c0ab3
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
| orjson        | flat         | 0.552    | 0.641       | 0.658    | 61.6     |
| msgspec       | flat         | 0.605    | 0.703       | 0.717    | 61.6     |
| strata        | flat         | 0.687    | 0.784       | 0.824    | 61.0     |
| ujson         | flat         | 1.085    | 1.226       | 1.226    | 61.6     |
| json (stdlib) | flat         | 2.042    | 2.109       | 2.109    | 61.6     |
| orjson        | mixed        | 0.482    | 0.627       | 0.668    | 77.0     |
| msgspec       | mixed        | 0.591    | 0.753       | 0.804    | 77.0     |
| strata        | mixed        | 0.699    | 0.866       | 0.876    | 77.0     |
| ujson         | mixed        | 1.087    | 1.202       | 1.245    | 77.0     |
| json (stdlib) | mixed        | 2.156    | 2.224       | 2.260    | 77.0     |
| strata        | nested       | 0.604    | 0.727       | 0.746    | 61.7     |
| orjson        | nested       | 1.609    | 1.798       | 1.989    | 61.7     |
| msgspec       | nested       | 1.478    | 1.818       | 1.833    | 61.7     |
| ujson         | nested       | 1.767    | 2.129       | 2.229    | 61.7     |
| json (stdlib) | nested       | 2.370    | 2.658       | 2.916    | 61.7     |
| strata        | users.json   | 3.861    | 4.750       | 5.167    | 33.8     |
| orjson        | users.json   | 4.999    | 6.205       | 6.860    | 37.8     |
| msgspec       | users.json   | 5.167    | 6.875       | 7.020    | 40.9     |
| ujson         | users.json   | 6.126    | 8.698       | 9.008    | 40.8     |
| json (stdlib) | users.json   | 11.127   | 11.447      | 12.140   | 42.9     |
| strata        | users.ndjson | 4.042    | 4.869       | 5.209    | 55.9     |
| orjson        | users.ndjson | 4.940    | 5.902       | 6.493    | 57.0     |
| msgspec       | users.ndjson | 4.619    | 6.132       | 7.471    | 58.0     |
| ujson         | users.ndjson | 6.344    | 8.402       | 9.201    | 58.0     |
| json (stdlib) | users.ndjson | 10.143   | 11.013      | 12.058   | 58.0     |
| strata        | wide_arrays  | 2.842    | 3.481       | 3.493    | 65.7     |
| orjson        | wide_arrays  | 3.630    | 4.147       | 4.204    | 68.3     |
| msgspec       | wide_arrays  | 4.499    | 5.396       | 5.467    | 68.3     |
| ujson         | wide_arrays  | 6.711    | 7.553       | 7.568    | 68.3     |
| json (stdlib) | wide_arrays  | 13.466   | 14.359      | 14.444   | 69.7     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.690    | 0.765       | 0.865    | 61.6     |
| msgspec       | flat         | 0.721    | 0.812       | 0.833    | 61.6     |
| strata        | flat         | 0.755    | 0.868       | 0.954    | 61.6     |
| ujson         | flat         | 1.304    | 1.449       | 1.647    | 61.6     |
| json (stdlib) | flat         | 2.227    | 2.317       | 2.321    | 61.6     |
| orjson        | mixed        | 0.683    | 0.836       | 0.881    | 77.0     |
| msgspec       | mixed        | 0.718    | 0.864       | 0.867    | 77.0     |
| strata        | mixed        | 0.766    | 0.915       | 1.046    | 77.0     |
| ujson         | mixed        | 1.240    | 1.410       | 1.632    | 77.0     |
| json (stdlib) | mixed        | 2.300    | 2.369       | 2.415    | 77.0     |
| strata        | nested       | 0.694    | 0.797       | 0.800    | 61.7     |
| orjson        | nested       | 1.744    | 1.929       | 2.045    | 61.7     |
| msgspec       | nested       | 1.971    | 2.175       | 2.301    | 61.7     |
| ujson         | nested       | 1.989    | 2.379       | 2.392    | 61.7     |
| json (stdlib) | nested       | 2.472    | 2.959       | 3.148    | 61.7     |
| strata        | users.json   | 4.025    | 5.305       | 5.871    | 42.9     |
| orjson        | users.json   | 5.117    | 6.466       | 6.470    | 43.9     |
| msgspec       | users.json   | 5.579    | 6.953       | 8.509    | 47.9     |
| ujson         | users.json   | 6.610    | 8.669       | 9.107    | 47.9     |
| json (stdlib) | users.json   | 9.405    | 10.566      | 10.906   | 48.1     |
| strata        | users.ndjson | 4.335    | 5.546       | 5.847    | 58.0     |
| orjson        | users.ndjson | 4.748    | 6.183       | 6.364    | 58.0     |
| msgspec       | users.ndjson | 4.921    | 6.232       | 6.823    | 60.4     |
| ujson         | users.ndjson | 7.629    | 9.867       | 9.951    | 60.4     |
| json (stdlib) | users.ndjson | 9.399    | 10.957      | 11.122   | 60.4     |
| strata        | wide_arrays  | 3.086    | 3.680       | 3.688    | 69.7     |
| orjson        | wide_arrays  | 4.036    | 4.536       | 4.880    | 69.7     |
| msgspec       | wide_arrays  | 4.879    | 5.396       | 5.499    | 71.3     |
| ujson         | wide_arrays  | 7.192    | 7.825       | 8.010    | 71.3     |
| json (stdlib) | wide_arrays  | 13.707   | 14.297      | 14.504   | 71.3     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.232    | 0.242       | 0.247    | 61.6     |
| orjson  | flat        | 0.246    | 0.246       | 0.246    | 61.6     |
| msgspec | flat        | 0.299    | 0.300       | 0.303    | 61.7     |
| ujson   | flat        | 1.209    | 1.212       | 1.218    | 61.7     |
| json    | flat        | 2.269    | 2.331       | 2.358    | 61.7     |
| strata  | mixed       | 0.264    | 0.275       | 0.279    | 77.0     |
| orjson  | mixed       | 0.283    | 0.289       | 0.325    | 77.0     |
| msgspec | mixed       | 0.316    | 0.322       | 0.344    | 77.0     |
| ujson   | mixed       | 1.311    | 1.320       | 1.335    | 77.0     |
| json    | mixed       | 2.985    | 3.043       | 3.160    | 77.0     |
| strata  | nested      | 0.181    | 0.183       | 0.205    | 61.7     |
| orjson  | nested      | 0.203    | 0.207       | 0.210    | 61.7     |
| msgspec | nested      | 0.219    | 0.221       | 0.225    | 61.7     |
| json    | nested      | 1.356    | 1.360       | 1.361    | 61.9     |
| ujson   | nested      | 1.013    | 2.107       | 2.349    | 61.9     |
| strata  | users.json  | 1.043    | 1.048       | 1.153    | 49.0     |
| msgspec | users.json  | 1.488    | 1.490       | 1.500    | 52.1     |
| orjson  | users.json  | 1.505    | 1.540       | 1.649    | 51.9     |
| ujson   | users.json  | 6.241    | 6.244       | 6.822    | 52.2     |
| json    | users.json  | 8.692    | 8.703       | 8.726    | 52.6     |
| strata  | wide_arrays | 1.329    | 1.370       | 1.386    | 72.5     |
| orjson  | wide_arrays | 1.556    | 1.624       | 1.647    | 74.4     |
| msgspec | wide_arrays | 1.629    | 1.724       | 1.919    | 75.6     |
| ujson   | wide_arrays | 7.120    | 7.135       | 7.207    | 76.9     |
| json    | wide_arrays | 19.925   | 20.004      | 20.024   | 76.9     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.406    | 0.457       | 0.484    | 61.7     |
| msgspec       | flat        | 0.467    | 0.492       | 0.507    | 61.7     |
| orjson        | flat        | 0.459    | 0.544       | 0.545    | 61.7     |
| ujson         | flat        | 1.396    | 1.397       | 1.429    | 61.7     |
| json (stdlib) | flat        | 8.402    | 8.440       | 8.490    | 61.7     |
| strata        | mixed       | 0.421    | 0.425       | 0.431    | 77.0     |
| orjson        | mixed       | 0.467    | 0.494       | 0.497    | 77.0     |
| msgspec       | mixed       | 0.502    | 0.504       | 0.510    | 77.0     |
| ujson         | mixed       | 1.528    | 1.568       | 1.683    | 77.0     |
| json (stdlib) | mixed       | 10.021   | 10.172      | 10.344   | 77.0     |
| strata        | nested      | 0.331    | 0.339       | 0.365    | 61.9     |
| msgspec       | nested      | 0.494    | 0.535       | 1.022    | 62.1     |
| orjson        | nested      | 0.533    | 0.552       | 0.633    | 61.9     |
| ujson         | nested      | 1.206    | 1.248       | 1.772    | 62.1     |
| json (stdlib) | nested      | 12.829   | 13.098      | 14.339   | 62.1     |
| strata        | users.json  | 1.231    | 1.298       | 1.338    | 52.6     |
| orjson        | users.json  | 1.733    | 1.748       | 2.108    | 52.6     |
| msgspec       | users.json  | 1.809    | 1.834       | 2.095    | 52.6     |
| ujson         | users.json  | 6.642    | 6.757       | 6.965    | 52.6     |
| json (stdlib) | users.json  | 66.355   | 66.363      | 66.647   | 52.6     |
| strata        | wide_arrays | 1.620    | 1.721       | 1.748    | 76.9     |
| orjson        | wide_arrays | 1.928    | 1.973       | 1.982    | 76.9     |
| msgspec       | wide_arrays | 1.978    | 2.046       | 2.048    | 77.0     |
| ujson         | wide_arrays | 7.454    | 7.542       | 7.614    | 77.0     |
| json (stdlib) | wide_arrays | 44.487   | 44.697      | 44.712   | 77.0     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.917    | 1000    | 52.7     |
|                                | orjson+jmespath    | 5.502    | 1000    | 52.8     |
|                                | orjson+jsonpath-ng | 6.520    | 1000    | 53.2     |
| all item prices                | strata             | 2.081    | 12046   | 53.3     |
|                                | orjson+jmespath    | 13.024   | 1000    | 53.3     |
|                                | orjson+jsonpath-ng | 34.947   | 12046   | 53.4     |
| recursive price                | strata             | 2.040    | 12046   | 53.0     |
|                                | orjson+jsonpath-ng | 127.107  | 12046   | 54.0     |
| flat:$.records\[\*\].id        | strata             | 0.519    | 500     | 61.7     |
|                                | orjson+jmespath    | 0.805    | 500     | 61.7     |
|                                | orjson+jsonpath-ng | 2.329    | 500     | 61.7     |
| flat:$.records\[0\]            | strata             | 0.489    | 1       | 61.7     |
|                                | orjson+jsonpath-ng | 0.651    | 1       | 61.7     |
|                                | orjson+jmespath    | 0.656    | 1       | 61.7     |
| nested:$.records\[\*\].id      | strata             | 0.381    | 500     | 62.1     |
|                                | orjson+jmespath    | 2.000    | 500     | 62.1     |
|                                | orjson+jsonpath-ng | 2.459    | 500     | 62.1     |
| nested:$.records\[0\]          | strata             | 0.430    | 1       | 62.1     |
|                                | orjson+jsonpath-ng | 1.682    | 1       | 62.1     |
|                                | orjson+jmespath    | 1.748    | 1       | 62.1     |
| wide_arrays:$.records\[\*\].id | strata             | 2.289    | 500     | 77.0     |
|                                | orjson+jmespath    | 4.055    | 500     | 77.0     |
|                                | orjson+jsonpath-ng | 4.678    | 500     | 77.0     |
| wide_arrays:$.records\[0\]     | strata             | 2.288    | 1       | 77.0     |
|                                | orjson+jmespath    | 3.963    | 1       | 77.0     |
|                                | orjson+jsonpath-ng | 4.036    | 1       | 77.0     |
| mixed:$.records\[\*\].id       | strata             | 0.484    | 500     | 77.0     |
|                                | orjson+jmespath    | 0.783    | 500     | 77.0     |
|                                | orjson+jsonpath-ng | 2.403    | 500     | 77.0     |
| mixed:$.records\[0\]           | strata             | 0.483    | 1       | 77.0     |
|                                | orjson+jsonpath-ng | 0.626    | 1       | 77.0     |
|                                | orjson+jmespath    | 0.629    | 1       | 77.0     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.066    | 1000    | 53.9     |
|                                            | jmespath    | 0.387    | 1000    | 53.9     |
|                                            | jsonpath-ng | 3.928    | 1000    | 53.9     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.904    | 12046   | 53.9     |
|                                            | jmespath    | 10.439   | 1000    | 53.9     |
|                                            | jsonpath-ng | 31.208   | 12046   | 56.6     |
| $..price                                   | strata      | 1.314    | 12046   | 55.6     |
|                                            | jsonpath-ng | 122.915  | 12046   | 57.6     |
| $.users\[?(@.age>30)\]                     | strata      | 0.110    | 794     | 55.9     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.489    | 1604    | 55.9     |
| flat:$.records\[\*\].id                    | strata      | 0.033    | 500     | 61.7     |
|                                            | jmespath    | 0.172    | 500     | 61.7     |
|                                            | jsonpath-ng | 1.555    | 500     | 61.7     |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 61.7     |
|                                            | jsonpath-ng | 0.006    | 1       | 61.7     |
|                                            | jmespath    | 0.006    | 1       | 61.7     |
| nested:$.records\[\*\].id                  | strata      | 0.027    | 500     | 62.1     |
|                                            | jmespath    | 0.173    | 500     | 62.1     |
|                                            | jsonpath-ng | 2.491    | 500     | 62.1     |
| nested:$.records\[0\]                      | strata      | 0.002    | 1       | 62.1     |
|                                            | jsonpath-ng | 0.007    | 1       | 62.1     |
|                                            | jmespath    | 0.017    | 1       | 62.1     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.029    | 500     | 77.0     |
|                                            | jmespath    | 0.176    | 500     | 77.0     |
|                                            | jsonpath-ng | 2.227    | 500     | 77.0     |
| wide_arrays:$.records\[0\]                 | strata      | 0.006    | 1       | 77.0     |
|                                            | jmespath    | 0.008    | 1       | 77.0     |
|                                            | jsonpath-ng | 0.008    | 1       | 77.0     |
| mixed:$.records\[\*\].id                   | strata      | 0.029    | 500     | 77.0     |
|                                            | jmespath    | 0.174    | 500     | 77.0     |
|                                            | jsonpath-ng | 1.695    | 500     | 77.0     |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 77.0     |
|                                            | jmespath    | 0.006    | 1       | 77.0     |
|                                            | jsonpath-ng | 0.016    | 1       | 77.0     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **29.5% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **14.3% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **27.1% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **9.5% faster** than #2 (orjson)    |
| dumps               | **#1** / 5              | **42.7% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **40.8% faster** than #2 (orjson)   |
| loads (flat)        | **#3** / 5              | 24.3% behind #1 (orjson)            |
| load (flat)         | **#3** / 5              | 9.4% behind #1 (orjson)             |
| dumps (flat)        | **#1** / 5              | **5.8% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **13.1% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **144.8% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **151.3% faster** than #2 (orjson)  |
| dumps (nested)      | **#1** / 5              | **12.2% faster** than #2 (orjson)   |
| dump (nested)       | **#1** / 5              | **49.4% faster** than #2 (msgspec)  |
| loads (wide_arrays) | **#1** / 5              | **27.7% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **30.8% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **17.1% faster** than #2 (orjson)   |
| dump (wide_arrays)  | **#1** / 5              | **19.1% faster** than #2 (orjson)   |
| loads (mixed)       | **#3** / 5              | 44.9% behind #1 (orjson)            |
| load (mixed)        | **#3** / 5              | 12.2% behind #1 (orjson)            |
| dumps (mixed)       | **#1** / 5              | **7.3% faster** than #2 (orjson)    |
| dump (mixed)        | **#1** / 5              | **11.0% faster** than #2 (orjson)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **717% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
