# Strata Benchmark Results

Generated: 2026-03-15 20:19:10

## Environment

- Commit: 07f85a0f96b182dd7aeee9835dc618399d95321a
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
| msgspec       | flat         | 0.657    | 0.782       | 0.806    | 60.3     |
| orjson        | flat         | 0.628    | 0.829       | 0.902    | 60.3     |
| strata        | flat         | 0.836    | 0.841       | 0.890    | 59.1     |
| ujson         | flat         | 1.083    | 1.201       | 1.306    | 60.3     |
| json (stdlib) | flat         | 2.053    | 2.100       | 2.246    | 60.3     |
| orjson        | mixed        | 0.520    | 0.720       | 0.816    | 74.2     |
| msgspec       | mixed        | 0.646    | 0.760       | 0.819    | 74.2     |
| strata        | mixed        | 0.726    | 0.894       | 0.916    | 74.2     |
| ujson         | mixed        | 0.991    | 1.179       | 1.258    | 74.2     |
| json (stdlib) | mixed        | 2.170    | 2.316       | 2.350    | 74.2     |
| strata        | nested       | 0.766    | 0.875       | 0.902    | 60.4     |
| orjson        | nested       | 1.701    | 1.806       | 2.054    | 60.4     |
| msgspec       | nested       | 1.621    | 1.858       | 2.028    | 60.4     |
| ujson         | nested       | 1.983    | 2.255       | 2.272    | 60.4     |
| json (stdlib) | nested       | 2.801    | 2.837       | 2.883    | 60.4     |
| strata        | users.json   | 4.324    | 5.277       | 5.466    | 34.2     |
| msgspec       | users.json   | 4.866    | 6.796       | 6.865    | 41.3     |
| orjson        | users.json   | 5.948    | 7.213       | 8.183    | 38.1     |
| ujson         | users.json   | 6.437    | 10.533      | 12.423   | 41.2     |
| json (stdlib) | users.json   | 9.049    | 11.376      | 14.108   | 44.2     |
| strata        | users.ndjson | 4.223    | 5.058       | 5.899    | 56.0     |
| msgspec       | users.ndjson | 4.733    | 6.461       | 6.512    | 58.0     |
| orjson        | users.ndjson | 4.633    | 6.760       | 7.447    | 57.0     |
| ujson         | users.ndjson | 6.761    | 9.358       | 9.513    | 58.0     |
| json (stdlib) | users.ndjson | 11.428   | 12.332      | 13.847   | 58.0     |
| strata        | wide_arrays  | 2.576    | 3.185       | 3.304    | 62.5     |
| msgspec       | wide_arrays  | 4.440    | 5.098       | 5.226    | 65.2     |
| orjson        | wide_arrays  | 3.481    | 7.147       | 7.324    | 65.1     |
| ujson         | wide_arrays  | 6.679    | 8.663       | 10.838   | 65.2     |
| json (stdlib) | wide_arrays  | 14.307   | 14.853      | 17.362   | 67.1     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.766    | 0.782       | 0.841    | 60.3     |
| msgspec       | flat         | 0.786    | 0.901       | 1.021    | 60.3     |
| strata        | flat         | 0.850    | 0.947       | 1.033    | 60.3     |
| ujson         | flat         | 1.310    | 1.432       | 1.562    | 60.3     |
| json (stdlib) | flat         | 2.246    | 2.276       | 2.363    | 60.3     |
| msgspec       | mixed        | 0.783    | 0.878       | 0.955    | 74.2     |
| strata        | mixed        | 0.800    | 0.952       | 0.993    | 74.2     |
| orjson        | mixed        | 0.595    | 1.042       | 1.178    | 74.2     |
| ujson         | mixed        | 1.235    | 1.414       | 1.638    | 74.2     |
| json (stdlib) | mixed        | 2.483    | 2.583       | 2.621    | 74.2     |
| strata        | nested       | 0.805    | 0.915       | 1.306    | 60.4     |
| msgspec       | nested       | 1.831    | 2.119       | 2.216    | 60.4     |
| ujson         | nested       | 2.264    | 2.716       | 2.784    | 60.4     |
| json (stdlib) | nested       | 2.755    | 2.930       | 3.094    | 60.4     |
| orjson        | nested       | 1.791    | 2.969       | 3.696    | 60.4     |
| strata        | users.json   | 4.336    | 5.013       | 5.868    | 44.2     |
| orjson        | users.json   | 4.896    | 6.326       | 7.560    | 44.2     |
| msgspec       | users.json   | 5.223    | 7.027       | 10.123   | 47.3     |
| ujson         | users.json   | 6.402    | 9.092       | 9.170    | 47.3     |
| json (stdlib) | users.json   | 9.815    | 11.397      | 11.531   | 48.3     |
| strata        | users.ndjson | 4.463    | 5.507       | 5.594    | 58.0     |
| orjson        | users.ndjson | 4.633    | 6.472       | 6.965    | 58.0     |
| msgspec       | users.ndjson | 6.088    | 8.830       | 9.282    | 59.1     |
| ujson         | users.ndjson | 7.000    | 10.485      | 13.671   | 59.1     |
| json (stdlib) | users.ndjson | 9.821    | 11.710      | 11.738   | 59.1     |
| strata        | wide_arrays  | 3.461    | 3.515       | 3.912    | 67.1     |
| orjson        | wide_arrays  | 4.026    | 4.838       | 4.924    | 68.4     |
| msgspec       | wide_arrays  | 6.118    | 7.079       | 8.934    | 71.1     |
| ujson         | wide_arrays  | 7.495    | 8.486       | 8.770    | 71.1     |
| json (stdlib) | wide_arrays  | 14.423   | 15.199      | 15.434   | 71.1     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.277    | 0.290       | 0.305    | 60.3     |
| msgspec | flat        | 0.298    | 0.300       | 0.309    | 60.4     |
| strata  | flat        | 0.328    | 0.367       | 0.371    | 60.3     |
| ujson   | flat        | 1.216    | 1.251       | 1.306    | 60.4     |
| json    | flat        | 2.279    | 2.365       | 2.373    | 60.4     |
| orjson  | mixed       | 0.286    | 0.290       | 0.301    | 74.2     |
| msgspec | mixed       | 0.308    | 0.320       | 0.331    | 74.2     |
| strata  | mixed       | 0.358    | 0.371       | 0.379    | 74.2     |
| ujson   | mixed       | 1.325    | 1.340       | 1.365    | 74.2     |
| json    | mixed       | 3.016    | 3.107       | 3.122    | 74.2     |
| orjson  | nested      | 0.197    | 0.197       | 0.203    | 60.4     |
| strata  | nested      | 0.202    | 0.208       | 0.216    | 60.4     |
| msgspec | nested      | 0.202    | 0.212       | 0.220    | 60.4     |
| ujson   | nested      | 0.984    | 1.010       | 1.016    | 60.5     |
| json    | nested      | 1.420    | 1.427       | 1.444    | 60.5     |
| orjson  | users.json  | 1.475    | 1.476       | 1.648    | 50.4     |
| msgspec | users.json  | 1.477    | 1.566       | 1.716    | 52.2     |
| strata  | users.json  | 1.595    | 1.601       | 1.612    | 48.3     |
| ujson   | users.json  | 6.339    | 6.383       | 6.489    | 52.4     |
| json    | users.json  | 8.722    | 8.891       | 9.084    | 53.0     |
| orjson  | wide_arrays | 1.552    | 1.570       | 1.796    | 74.1     |
| msgspec | wide_arrays | 1.709    | 1.824       | 1.858    | 74.2     |
| strata  | wide_arrays | 1.843    | 1.875       | 1.951    | 72.3     |
| ujson   | wide_arrays | 6.974    | 7.131       | 7.204    | 74.2     |
| json    | wide_arrays | 19.775   | 20.355      | 20.466   | 74.2     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 0.470    | 0.470       | 0.481    | 60.4     |
| strata        | flat        | 0.492    | 0.505       | 0.526    | 60.4     |
| msgspec       | flat        | 0.536    | 0.551       | 0.623    | 60.4     |
| ujson         | flat        | 1.439    | 1.475       | 1.547    | 60.4     |
| json (stdlib) | flat        | 8.336    | 8.655       | 8.887    | 60.4     |
| orjson        | mixed       | 0.555    | 0.558       | 0.582    | 74.2     |
| msgspec       | mixed       | 0.589    | 0.612       | 0.634    | 74.2     |
| strata        | mixed       | 0.578    | 0.706       | 0.745    | 74.2     |
| ujson         | mixed       | 1.569    | 1.639       | 1.866    | 74.2     |
| json (stdlib) | mixed       | 10.123   | 10.346      | 11.079   | 74.2     |
| strata        | nested      | 0.301    | 0.362       | 0.397    | 60.5     |
| msgspec       | nested      | 0.395    | 0.414       | 0.464    | 60.5     |
| orjson        | nested      | 0.665    | 0.776       | 0.828    | 60.5     |
| ujson         | nested      | 1.254    | 1.281       | 1.355    | 60.5     |
| json (stdlib) | nested      | 12.180   | 12.276      | 12.334   | 60.5     |
| msgspec       | users.json  | 1.756    | 1.763       | 1.811    | 53.0     |
| orjson        | users.json  | 1.702    | 1.776       | 1.807    | 53.0     |
| strata        | users.json  | 1.799    | 1.880       | 1.915    | 53.0     |
| ujson         | users.json  | 6.650    | 6.873       | 6.919    | 53.0     |
| json (stdlib) | users.json  | 67.403   | 67.639      | 68.335   | 53.0     |
| orjson        | wide_arrays | 1.895    | 1.954       | 2.005    | 74.2     |
| msgspec       | wide_arrays | 1.951    | 2.023       | 2.074    | 74.2     |
| strata        | wide_arrays | 2.272    | 2.353       | 2.411    | 74.2     |
| ujson         | wide_arrays | 7.509    | 7.571       | 7.731    | 74.2     |
| json (stdlib) | wide_arrays | 45.284   | 45.329      | 45.814   | 74.2     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.944    | 1000    | 53.0     |
|                                | orjson+jmespath    | 5.517    | 1000    | 53.1     |
|                                | orjson+jsonpath-ng | 7.291    | 1000    | 53.6     |
| all item prices                | strata             | 2.077    | 12046   | 53.7     |
|                                | orjson+jmespath    | 14.064   | 1000    | 53.7     |
|                                | orjson+jsonpath-ng | 38.608   | 12046   | 53.6     |
| recursive price                | strata             | 2.213    | 12046   | 53.2     |
|                                | orjson+jsonpath-ng | 129.297  | 12046   | 54.0     |
| flat:$.records\[\*\].id        | strata             | 0.492    | 500     | 60.4     |
|                                | orjson+jmespath    | 0.862    | 500     | 60.4     |
|                                | orjson+jsonpath-ng | 2.756    | 500     | 60.4     |
| flat:$.records\[0\]            | strata             | 0.459    | 1       | 60.4     |
|                                | orjson+jmespath    | 0.666    | 1       | 60.4     |
|                                | orjson+jsonpath-ng | 0.675    | 1       | 60.4     |
| nested:$.records\[\*\].id      | strata             | 0.427    | 500     | 60.5     |
|                                | orjson+jmespath    | 1.885    | 500     | 60.5     |
|                                | orjson+jsonpath-ng | 2.511    | 500     | 60.5     |
| nested:$.records\[0\]          | strata             | 0.423    | 1       | 60.5     |
|                                | orjson+jmespath    | 1.857    | 1       | 60.5     |
|                                | orjson+jsonpath-ng | 1.874    | 1       | 60.5     |
| wide_arrays:$.records\[\*\].id | strata             | 2.244    | 500     | 74.2     |
|                                | orjson+jmespath    | 4.222    | 500     | 74.2     |
|                                | orjson+jsonpath-ng | 4.880    | 500     | 74.2     |
| wide_arrays:$.records\[0\]     | strata             | 2.206    | 1       | 74.2     |
|                                | orjson+jsonpath-ng | 4.149    | 1       | 74.2     |
|                                | orjson+jmespath    | 4.176    | 1       | 74.2     |
| mixed:$.records\[\*\].id       | strata             | 0.475    | 500     | 74.2     |
|                                | orjson+jmespath    | 0.948    | 500     | 74.2     |
|                                | orjson+jsonpath-ng | 2.751    | 500     | 74.2     |
| mixed:$.records\[0\]           | strata             | 0.511    | 1       | 74.2     |
|                                | orjson+jsonpath-ng | 0.644    | 1       | 74.2     |
|                                | orjson+jmespath    | 0.650    | 1       | 74.2     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.089    | 1000    | 53.9     |
|                                            | jmespath    | 0.419    | 1000    | 53.9     |
|                                            | jsonpath-ng | 4.301    | 1000    | 53.9     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.936    | 12046   | 53.9     |
|                                            | jmespath    | 11.223   | 1000    | 53.9     |
|                                            | jsonpath-ng | 33.649   | 12046   | 56.6     |
| $..price                                   | strata      | 1.366    | 12046   | 56.6     |
|                                            | jsonpath-ng | 126.929  | 12046   | 58.6     |
| $.users\[?(@.age>30)\]                     | strata      | 0.121    | 794     | 56.9     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.588    | 1604    | 56.0     |
| flat:$.records\[\*\].id                    | strata      | 0.027    | 500     | 60.4     |
|                                            | jmespath    | 0.169    | 500     | 60.4     |
|                                            | jsonpath-ng | 1.714    | 500     | 60.4     |
| flat:$.records\[0\]                        | jmespath    | 0.006    | 1       | 60.4     |
|                                            | jsonpath-ng | 0.006    | 1       | 60.4     |
|                                            | strata      | 0.011    | 1       | 60.4     |
| nested:$.records\[\*\].id                  | strata      | 0.027    | 500     | 60.5     |
|                                            | jmespath    | 0.174    | 500     | 60.5     |
|                                            | jsonpath-ng | 2.010    | 500     | 60.5     |
| nested:$.records\[0\]                      | strata      | 0.004    | 1       | 60.5     |
|                                            | jmespath    | 0.006    | 1       | 60.5     |
|                                            | jsonpath-ng | 0.007    | 1       | 60.5     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.031    | 500     | 74.2     |
|                                            | jmespath    | 0.193    | 500     | 74.2     |
|                                            | jsonpath-ng | 2.425    | 500     | 74.2     |
| wide_arrays:$.records\[0\]                 | strata      | 0.003    | 1       | 74.2     |
|                                            | jmespath    | 0.006    | 1       | 74.2     |
|                                            | jsonpath-ng | 0.018    | 1       | 74.2     |
| mixed:$.records\[\*\].id                   | strata      | 0.026    | 500     | 74.2     |
|                                            | jmespath    | 0.177    | 500     | 74.2     |
|                                            | jsonpath-ng | 1.988    | 500     | 74.2     |
| mixed:$.records\[0\]                       | strata      | 0.004    | 1       | 74.2     |
|                                            | jmespath    | 0.018    | 1       | 74.2     |
|                                            | jsonpath-ng | 0.020    | 1       | 74.2     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **12.5% faster** than #2 (msgspec)  |
| loads (NDJSON)      | **#1** / 5              | **9.7% faster** than #2 (orjson)    |
| load (JSON file)    | **#1** / 5              | **12.9% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **3.8% faster** than #2 (orjson)    |
| dumps               | **#3** / 5              | 8.1% behind #1 (orjson)             |
| dump                | **#3** / 5              | 5.7% behind #1 (orjson)             |
| loads (flat)        | **#3** / 5              | 33.1% behind #1 (orjson)            |
| load (flat)         | **#3** / 5              | 11.0% behind #1 (orjson)            |
| dumps (flat)        | **#3** / 5              | 18.7% behind #1 (orjson)            |
| dump (flat)         | **#2** / 5              | 4.8% behind #1 (orjson)             |
| loads (nested)      | **#1** / 5              | **111.6% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **122.6% faster** than #2 (orjson)  |
| dumps (nested)      | **#3** / 5              | 2.5% behind #1 (orjson)             |
| dump (nested)       | **#1** / 5              | **31.2% faster** than #2 (msgspec)  |
| loads (wide_arrays) | **#1** / 5              | **35.1% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **16.3% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#3** / 5              | 18.7% behind #1 (orjson)            |
| dump (wide_arrays)  | **#3** / 5              | 19.9% behind #1 (orjson)            |
| loads (mixed)       | **#3** / 5              | 39.5% behind #1 (orjson)            |
| load (mixed)        | **#3** / 5              | 34.4% behind #1 (orjson)            |
| dumps (mixed)       | **#3** / 5              | 25.4% behind #1 (orjson)            |
| dump (mixed)        | **#2** / 5              | 4.1% behind #1 (orjson)             |
| search (JSONPath)   | **#1** in 11/11 queries | avg **692% faster**                 |
| query (JSONPath)    | **#1** in 12/13 queries | -                                   |
