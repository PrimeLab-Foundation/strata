# Strata Benchmark Results

Generated: 2026-03-16 15:43:57

## Environment

- Commit: a676cc453da8a24953da243df7465313ffd5cbfd
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/medium/users.json, benchmarks/data/generated/medium/users.ndjson
- Random schemas: flat, nested, wide_arrays, mixed (2000 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 2.470    | 2.902       | 3.013    | 272.3    |
| orjson        | flat         | 3.640    | 4.279       | 4.479    | 274.8    |
| msgspec       | flat         | 3.687    | 4.472       | 6.277    | 274.8    |
| ujson         | flat         | 5.581    | 6.610       | 7.296    | 274.8    |
| json (stdlib) | flat         | 10.100   | 10.445      | 10.524   | 275.2    |
| strata        | mixed        | 2.417    | 3.082       | 3.350    | 297.3    |
| orjson        | mixed        | 4.056    | 5.118       | 5.683    | 299.6    |
| msgspec       | mixed        | 6.208    | 6.501       | 7.299    | 299.6    |
| ujson         | mixed        | 5.911    | 7.434       | 7.604    | 299.6    |
| json (stdlib) | mixed        | 11.424   | 11.514      | 11.881   | 299.6    |
| strata        | nested       | 2.076    | 2.612       | 2.638    | 280.5    |
| msgspec       | nested       | 4.957    | 6.162       | 7.145    | 283.0    |
| orjson        | nested       | 6.821    | 7.751       | 9.129    | 282.9    |
| ujson         | nested       | 7.773    | 8.002       | 8.182    | 283.0    |
| json (stdlib) | nested       | 8.100    | 10.746      | 15.734   | 283.0    |
| strata        | users.json   | 21.419   | 27.739      | 27.994   | 73.5     |
| orjson        | users.json   | 31.131   | 41.557      | 53.101   | 93.5     |
| msgspec       | users.json   | 34.169   | 42.501      | 49.326   | 92.3     |
| ujson         | users.json   | 40.601   | 58.326      | 73.497   | 109.9    |
| json (stdlib) | users.json   | 55.876   | 71.079      | 71.156   | 98.5     |
| strata        | users.ndjson | 21.887   | 28.601      | 28.697   | 254.5    |
| orjson        | users.ndjson | 35.612   | 37.281      | 39.941   | 262.5    |
| msgspec       | users.ndjson | 38.141   | 39.252      | 44.647   | 263.7    |
| ujson         | users.ndjson | 46.957   | 54.061      | 54.740   | 262.7    |
| json (stdlib) | users.ndjson | 66.457   | 67.281      | 76.315   | 264.7    |
| strata        | wide_arrays  | 11.626   | 13.976      | 14.451   | 295.1    |
| msgspec       | wide_arrays  | 15.785   | 19.744      | 19.863   | 305.4    |
| orjson        | wide_arrays  | 12.074   | 20.303      | 22.400   | 305.4    |
| ujson         | wide_arrays  | 25.188   | 28.587      | 33.743   | 305.4    |
| json (stdlib) | wide_arrays  | 52.629   | 55.569      | 56.223   | 305.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 2.760    | 3.120       | 3.182    | 275.2    |
| orjson        | flat         | 3.987    | 4.584       | 4.952    | 275.2    |
| msgspec       | flat         | 3.972    | 4.861       | 7.240    | 277.5    |
| ujson         | flat         | 6.322    | 7.387       | 8.075    | 277.5    |
| json (stdlib) | flat         | 10.452   | 10.708      | 12.319   | 277.5    |
| strata        | mixed        | 2.674    | 3.224       | 3.332    | 299.6    |
| orjson        | mixed        | 4.282    | 5.383       | 8.720    | 299.6    |
| msgspec       | mixed        | 5.082    | 5.854       | 6.795    | 299.6    |
| ujson         | mixed        | 6.746    | 7.665       | 8.091    | 299.6    |
| json (stdlib) | mixed        | 10.599   | 12.109      | 14.331   | 299.6    |
| strata        | nested       | 2.231    | 2.717       | 2.756    | 283.0    |
| orjson        | nested       | 4.962    | 6.718       | 6.871    | 283.0    |
| ujson         | nested       | 6.703    | 8.549       | 9.047    | 283.0    |
| msgspec       | nested       | 7.376    | 10.048      | 12.185   | 283.0    |
| json (stdlib) | nested       | 9.069    | 10.326      | 11.128   | 283.0    |
| strata        | users.json   | 21.221   | 28.957      | 30.955   | 99.9     |
| orjson        | users.json   | 26.874   | 39.138      | 54.703   | 99.9     |
| msgspec       | users.json   | 28.690   | 43.555      | 46.299   | 104.8    |
| ujson         | users.json   | 47.380   | 58.989      | 77.419   | 122.5    |
| json (stdlib) | users.json   | 62.736   | 70.214      | 80.934   | 104.8    |
| strata        | users.ndjson | 22.457   | 28.956      | 31.572   | 266.7    |
| orjson        | users.ndjson | 34.731   | 39.653      | 47.503   | 267.7    |
| msgspec       | users.ndjson | 35.554   | 40.008      | 49.897   | 268.7    |
| ujson         | users.ndjson | 55.515   | 57.763      | 58.876   | 267.7    |
| json (stdlib) | users.ndjson | 65.944   | 67.906      | 71.006   | 269.7    |
| strata        | wide_arrays  | 12.462   | 14.892      | 14.979   | 305.4    |
| orjson        | wide_arrays  | 14.738   | 16.834      | 18.719   | 305.0    |
| msgspec       | wide_arrays  | 17.015   | 20.213      | 22.695   | 305.0    |
| ujson         | wide_arrays  | 30.104   | 31.395      | 31.541   | 305.0    |
| json (stdlib) | wide_arrays  | 53.157   | 57.614      | 58.603   | 305.0    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 1.028    | 1.037       | 1.047    | 277.5    |
| orjson  | flat        | 0.981    | 1.061       | 1.141    | 279.8    |
| msgspec | flat        | 1.161    | 1.175       | 1.200    | 279.8    |
| ujson   | flat        | 4.874    | 4.952       | 5.104    | 280.5    |
| json    | flat        | 9.024    | 9.202       | 9.209    | 280.5    |
| orjson  | mixed       | 1.096    | 1.107       | 1.120    | 299.6    |
| strata  | mixed       | 1.184    | 1.186       | 1.239    | 299.6    |
| msgspec | mixed       | 1.205    | 1.211       | 1.457    | 299.6    |
| ujson   | mixed       | 5.132    | 5.169       | 5.280    | 300.9    |
| json    | mixed       | 11.616   | 11.730      | 12.166   | 300.9    |
| strata  | nested      | 0.694    | 0.698       | 0.702    | 283.0    |
| msgspec | nested      | 0.808    | 0.828       | 0.828    | 283.0    |
| orjson  | nested      | 0.795    | 0.932       | 1.089    | 283.0    |
| ujson   | nested      | 3.976    | 4.067       | 4.206    | 283.0    |
| json    | nested      | 5.285    | 5.301       | 5.426    | 283.0    |
| strata  | users.json  | 7.555    | 7.582       | 7.654    | 109.0    |
| orjson  | users.json  | 10.949   | 10.965      | 11.648   | 132.4    |
| msgspec | users.json  | 10.906   | 11.143      | 11.880   | 161.2    |
| ujson   | users.json  | 41.335   | 41.340      | 41.551   | 167.6    |
| json    | users.json  | 59.745   | 59.747      | 60.537   | 192.8    |
| strata  | wide_arrays | 6.907    | 6.957       | 6.973    | 307.4    |
| msgspec | wide_arrays | 7.028    | 7.047       | 7.174    | 317.6    |
| orjson  | wide_arrays | 7.759    | 7.910       | 8.206    | 297.8    |
| ujson   | wide_arrays | 28.920   | 29.131      | 29.667   | 319.5    |
| json    | wide_arrays | 80.959   | 81.174      | 82.274   | 277.8    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.318    | 1.321       | 1.541    | 280.5    |
| orjson        | flat        | 1.305    | 1.345       | 1.390    | 280.5    |
| msgspec       | flat        | 1.532    | 1.586       | 1.626    | 280.5    |
| ujson         | flat        | 5.198    | 5.232       | 5.245    | 280.5    |
| json (stdlib) | flat        | 33.021   | 33.301      | 33.990   | 280.5    |
| strata        | mixed       | 1.358    | 1.396       | 1.419    | 300.9    |
| orjson        | mixed       | 1.385    | 1.498       | 2.103    | 300.9    |
| msgspec       | mixed       | 1.517    | 1.605       | 2.062    | 300.9    |
| ujson         | mixed       | 5.509    | 5.745       | 5.854    | 300.9    |
| json (stdlib) | mixed       | 39.850   | 40.313      | 41.376   | 300.9    |
| strata        | nested      | 0.897    | 0.952       | 1.094    | 283.0    |
| msgspec       | nested      | 1.081    | 1.139       | 1.180    | 283.0    |
| orjson        | nested      | 1.269    | 1.372       | 1.593    | 283.0    |
| ujson         | nested      | 4.394    | 4.609       | 4.617    | 283.0    |
| json (stdlib) | nested      | 48.110   | 48.432      | 48.644   | 283.0    |
| strata        | users.json  | 9.179    | 9.283       | 10.912   | 199.1    |
| orjson        | users.json  | 12.769   | 12.844      | 14.450   | 221.4    |
| msgspec       | users.json  | 11.875   | 12.959      | 13.744   | 243.9    |
| ujson         | users.json  | 42.925   | 43.571      | 44.575   | 221.5    |
| json (stdlib) | users.json  | 435.266  | 435.457     | 438.234  | 243.9    |
| strata        | wide_arrays | 7.772    | 7.886       | 9.679    | 277.8    |
| orjson        | wide_arrays | 8.308    | 8.446       | 9.570    | 275.6    |
| msgspec       | wide_arrays | 8.172    | 8.821       | 9.934    | 297.3    |
| ujson         | wide_arrays | 29.732   | 29.962      | 30.595   | 277.6    |
| json (stdlib) | wide_arrays | 180.212  | 180.805     | 181.628  | 297.3    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 9.038    | 2000    | 241.0    |
|                                | orjson+jmespath    | 30.893   | 2000    | 244.1    |
|                                | orjson+jsonpath-ng | 34.287   | 2000    | 246.2    |
| all item prices                | strata             | 10.574   | 100999  | 247.2    |
|                                | orjson+jmespath    | 80.597   | 2000    | 256.4    |
|                                | orjson+jsonpath-ng | 265.939  | 100999  | 263.4    |
| recursive price                | strata             | 10.960   | 100999  | 261.2    |
|                                | orjson+jsonpath-ng | 887.070  | 100999  | 266.2    |
| flat:$.records\[\*\].id        | strata             | 1.518    | 2000    | 280.5    |
|                                | orjson+jmespath    | 4.760    | 2000    | 280.5    |
|                                | orjson+jsonpath-ng | 7.770    | 2000    | 280.5    |
| flat:$.records\[0\]            | strata             | 1.610    | 1       | 280.5    |
|                                | orjson+jsonpath-ng | 4.107    | 1       | 280.5    |
|                                | orjson+jmespath    | 4.192    | 1       | 280.5    |
| nested:$.records\[\*\].id      | strata             | 1.058    | 2000    | 283.0    |
|                                | orjson+jmespath    | 6.283    | 2000    | 283.0    |
|                                | orjson+jsonpath-ng | 8.077    | 2000    | 283.0    |
| nested:$.records\[0\]          | strata             | 1.031    | 1       | 283.0    |
|                                | orjson+jmespath    | 5.316    | 1       | 283.0    |
|                                | orjson+jsonpath-ng | 5.660    | 1       | 283.0    |
| wide_arrays:$.records\[\*\].id | strata             | 9.625    | 2000    | 297.3    |
|                                | orjson+jmespath    | 15.413   | 2000    | 297.3    |
|                                | orjson+jsonpath-ng | 18.041   | 2000    | 297.3    |
| wide_arrays:$.records\[0\]     | strata             | 9.642    | 1       | 297.3    |
|                                | orjson+jsonpath-ng | 14.609   | 1       | 297.3    |
|                                | orjson+jmespath    | 15.347   | 1       | 297.3    |
| mixed:$.records\[\*\].id       | strata             | 1.740    | 2000    | 300.9    |
|                                | orjson+jmespath    | 5.333    | 2000    | 300.9    |
|                                | orjson+jsonpath-ng | 7.620    | 2000    | 300.9    |
| mixed:$.records\[0\]           | strata             | 1.573    | 1       | 300.9    |
|                                | orjson+jsonpath-ng | 4.796    | 1       | 300.9    |
|                                | orjson+jmespath    | 5.597    | 1       | 300.9    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.301    | 2000    | 288.5    |
|                                            | jmespath    | 0.917    | 2000    | 288.5    |
|                                            | jsonpath-ng | 16.473   | 2000    | 289.3    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.346    | 100999  | 289.3    |
|                                            | jmespath    | 63.321   | 2000    | 293.2    |
|                                            | jsonpath-ng | 256.064  | 100999  | 295.6    |
| $..price                                   | strata      | 8.375    | 100999  | 295.2    |
|                                            | jsonpath-ng | 879.160  | 100999  | 277.8    |
| $.users\[?(@.age>30)\]                     | strata      | 0.362    | 1606    | 275.8    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.278    | 6650    | 275.8    |
| flat:$.records\[\*\].id                    | strata      | 0.120    | 2000    | 280.5    |
|                                            | jmespath    | 0.670    | 2000    | 280.5    |
|                                            | jsonpath-ng | 4.513    | 2000    | 280.5    |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 280.5    |
|                                            | jsonpath-ng | 0.007    | 1       | 280.5    |
|                                            | jmespath    | 0.009    | 1       | 280.5    |
| nested:$.records\[\*\].id                  | strata      | 0.114    | 2000    | 283.0    |
|                                            | jmespath    | 0.693    | 2000    | 283.0    |
|                                            | jsonpath-ng | 6.193    | 2000    | 283.0    |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 283.0    |
|                                            | jmespath    | 0.007    | 1       | 283.0    |
|                                            | jsonpath-ng | 0.007    | 1       | 283.0    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.153    | 2000    | 297.3    |
|                                            | jmespath    | 0.724    | 2000    | 297.3    |
|                                            | jsonpath-ng | 7.278    | 2000    | 297.3    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 297.3    |
|                                            | jsonpath-ng | 0.009    | 1       | 297.3    |
|                                            | jmespath    | 0.010    | 1       | 297.3    |
| mixed:$.records\[\*\].id                   | strata      | 0.110    | 2000    | 300.9    |
|                                            | jmespath    | 0.741    | 2000    | 300.9    |
|                                            | jsonpath-ng | 5.497    | 2000    | 300.9    |
| mixed:$.records\[0\]                       | strata      | 0.004    | 1       | 300.9    |
|                                            | jsonpath-ng | 0.007    | 1       | 300.9    |
|                                            | jmespath    | 0.008    | 1       | 300.9    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **45.3% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **62.7% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **26.6% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **54.7% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **44.3% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **29.4% faster** than #2 (msgspec)  |
| loads (flat)        | **#1** / 5              | **47.4% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **43.9% faster** than #2 (msgspec)  |
| dumps (flat)        | **#2** / 5              | 4.8% behind #1 (orjson)             |
| dump (flat)         | **#2** / 5              | 1.0% behind #1 (orjson)             |
| loads (nested)      | **#1** / 5              | **138.7% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **122.4% faster** than #2 (orjson)  |
| dumps (nested)      | **#1** / 5              | **14.5% faster** than #2 (orjson)   |
| dump (nested)       | **#1** / 5              | **20.6% faster** than #2 (msgspec)  |
| loads (wide_arrays) | **#1** / 5              | **3.9% faster** than #2 (orjson)    |
| load (wide_arrays)  | **#1** / 5              | **18.3% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **1.8% faster** than #2 (msgspec)   |
| dump (wide_arrays)  | **#1** / 5              | **5.1% faster** than #2 (msgspec)   |
| loads (mixed)       | **#1** / 5              | **67.8% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **60.2% faster** than #2 (orjson)   |
| dumps (mixed)       | **#2** / 5              | 8.0% behind #1 (orjson)             |
| dump (mixed)        | **#1** / 5              | **2.0% faster** than #2 (orjson)    |
| search (JSONPath)   | **#1** in 11/11 queries | avg **973% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
