# Strata Benchmark Results

Generated: 2026-03-17 14:48:24

## Environment

- Commit: 75205226403b1b3dd7be908ebcd6e9ece844e9a9
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/users.json, benchmarks/data/generated/users.ndjson
- Random schemas: flat, nested, wide_arrays, mixed (500 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.607    | 0.649       | 0.757    | 222.1    |
| strata        | flat         | 0.659    | 0.667       | 0.737    | 222.1    |
| msgspec       | flat         | 0.613    | 0.718       | 0.757    | 222.1    |
| ujson         | flat         | 1.103    | 1.258       | 1.338    | 222.1    |
| json (stdlib) | flat         | 2.051    | 2.136       | 2.186    | 222.1    |
| orjson        | mixed        | 0.501    | 0.635       | 0.640    | 231.8    |
| msgspec       | mixed        | 0.685    | 0.765       | 0.889    | 231.8    |
| strata        | mixed        | 0.705    | 0.818       | 0.847    | 231.8    |
| ujson         | mixed        | 0.973    | 1.196       | 1.207    | 231.8    |
| json (stdlib) | mixed        | 2.213    | 2.244       | 2.317    | 231.8    |
| strata        | nested       | 0.510    | 0.629       | 0.674    | 220.1    |
| msgspec       | nested       | 1.981    | 2.231       | 2.775    | 220.7    |
| orjson        | nested       | 1.648    | 2.455       | 2.652    | 220.7    |
| ujson         | nested       | 2.301    | 2.574       | 3.175    | 220.7    |
| json (stdlib) | nested       | 2.633    | 2.652       | 2.857    | 220.7    |
| strata        | users.json   | 24.865   | 25.053      | 82.061   | 64.6     |
| msgspec       | users.json   | 34.398   | 37.918      | 40.256   | 80.4     |
| orjson        | users.json   | 25.242   | 38.958      | 47.888   | 80.6     |
| ujson         | users.json   | 42.764   | 55.942      | 71.170   | 94.4     |
| json (stdlib) | users.json   | 51.032   | 59.628      | 72.879   | 85.5     |
| strata        | users.ndjson | 19.416   | 24.381      | 24.878   | 221.1    |
| orjson        | users.ndjson | 31.452   | 32.730      | 36.839   | 227.6    |
| msgspec       | users.ndjson | 30.849   | 34.769      | 37.261   | 215.1    |
| ujson         | users.ndjson | 46.349   | 48.092      | 49.875   | 215.1    |
| json (stdlib) | users.ndjson | 60.664   | 64.209      | 66.817   | 216.1    |
| strata        | wide_arrays  | 3.366    | 3.930       | 4.308    | 222.4    |
| orjson        | wide_arrays  | 3.493    | 4.400       | 4.561    | 225.0    |
| msgspec       | wide_arrays  | 4.755    | 5.674       | 5.776    | 225.0    |
| ujson         | wide_arrays  | 7.617    | 8.029       | 8.211    | 225.0    |
| json (stdlib) | wide_arrays  | 15.544   | 16.949      | 17.038   | 225.6    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 0.646    | 0.722       | 0.764    | 222.1    |
| orjson        | flat         | 0.662    | 0.747       | 0.833    | 222.1    |
| msgspec       | flat         | 0.766    | 0.845       | 0.910    | 222.1    |
| ujson         | flat         | 1.228    | 1.397       | 1.487    | 222.1    |
| json (stdlib) | flat         | 2.307    | 2.426       | 2.554    | 222.1    |
| orjson        | mixed        | 0.662    | 0.740       | 0.759    | 231.8    |
| msgspec       | mixed        | 0.659    | 0.808       | 1.022    | 231.8    |
| strata        | mixed        | 0.775    | 0.918       | 0.969    | 231.8    |
| ujson         | mixed        | 1.109    | 1.276       | 1.300    | 231.8    |
| json (stdlib) | mixed        | 2.330    | 2.378       | 2.496    | 231.8    |
| strata        | nested       | 0.568    | 0.679       | 0.689    | 220.7    |
| orjson        | nested       | 1.797    | 1.819       | 1.875    | 220.7    |
| msgspec       | nested       | 1.583    | 1.868       | 1.913    | 220.7    |
| ujson         | nested       | 1.914    | 2.156       | 2.220    | 220.7    |
| json (stdlib) | nested       | 2.629    | 2.708       | 2.981    | 220.7    |
| strata        | users.json   | 18.886   | 24.125      | 25.833   | 85.9     |
| orjson        | users.json   | 22.662   | 34.455      | 36.137   | 85.9     |
| msgspec       | users.json   | 23.766   | 36.903      | 38.060   | 90.6     |
| ujson         | users.json   | 37.238   | 51.971      | 61.291   | 104.7    |
| json (stdlib) | users.json   | 58.045   | 63.298      | 75.862   | 90.6     |
| strata        | users.ndjson | 20.262   | 26.988      | 27.063   | 217.1    |
| msgspec       | users.ndjson | 31.363   | 32.394      | 43.188   | 220.6    |
| orjson        | users.ndjson | 31.329   | 35.553      | 36.456   | 218.1    |
| ujson         | users.ndjson | 42.859   | 55.669      | 55.823   | 220.6    |
| json (stdlib) | users.ndjson | 57.049   | 64.217      | 70.319   | 221.6    |
| strata        | wide_arrays  | 3.597    | 4.246       | 4.414    | 225.6    |
| orjson        | wide_arrays  | 3.909    | 4.761       | 5.225    | 225.6    |
| msgspec       | wide_arrays  | 6.735    | 7.223       | 9.132    | 228.4    |
| ujson         | wide_arrays  | 7.741    | 8.754       | 9.133    | 228.4    |
| json (stdlib) | wide_arrays  | 14.884   | 15.606      | 16.087   | 228.4    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.234    | 0.237       | 0.238    | 222.1    |
| orjson  | flat        | 0.254    | 0.254       | 0.257    | 222.1    |
| msgspec | flat        | 0.312    | 0.316       | 0.316    | 222.1    |
| ujson   | flat        | 1.174    | 1.205       | 1.205    | 222.1    |
| json    | flat        | 2.237    | 2.275       | 2.292    | 222.1    |
| strata  | mixed       | 0.265    | 0.273       | 0.274    | 231.8    |
| orjson  | mixed       | 0.287    | 0.301       | 0.307    | 231.8    |
| msgspec | mixed       | 0.304    | 0.305       | 0.311    | 231.8    |
| ujson   | mixed       | 1.314    | 1.314       | 1.326    | 231.8    |
| json    | mixed       | 2.976    | 2.981       | 2.993    | 231.8    |
| strata  | nested      | 0.181    | 0.191       | 0.195    | 220.7    |
| orjson  | nested      | 0.195    | 0.195       | 0.198    | 220.7    |
| msgspec | nested      | 0.205    | 0.209       | 0.209    | 220.7    |
| ujson   | nested      | 0.986    | 0.995       | 1.012    | 220.7    |
| json    | nested      | 1.397    | 1.422       | 1.487    | 220.7    |
| strata  | users.json  | 5.804    | 5.847       | 7.822    | 94.6     |
| msgspec | users.json  | 8.234    | 8.360       | 9.963    | 133.1    |
| orjson  | users.json  | 8.459    | 8.974       | 10.155   | 114.0    |
| ujson   | users.json  | 33.627   | 33.751      | 36.568   | 138.2    |
| json    | users.json  | 47.031   | 47.326      | 49.948   | 158.7    |
| strata  | wide_arrays | 1.378    | 1.396       | 1.422    | 228.4    |
| orjson  | wide_arrays | 1.546    | 1.554       | 1.750    | 230.3    |
| msgspec | wide_arrays | 1.619    | 1.621       | 1.672    | 230.6    |
| ujson   | wide_arrays | 7.069    | 7.115       | 7.274    | 231.8    |
| json    | wide_arrays | 20.577   | 22.077      | 22.089   | 231.8    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.366    | 0.379       | 0.474    | 222.1    |
| orjson        | flat        | 0.411    | 0.415       | 0.487    | 222.1    |
| msgspec       | flat        | 0.459    | 0.465       | 0.473    | 222.1    |
| ujson         | flat        | 1.380    | 1.391       | 1.407    | 222.1    |
| json (stdlib) | flat        | 8.472    | 8.544       | 8.654    | 222.1    |
| orjson        | mixed       | 0.475    | 0.489       | 0.631    | 231.8    |
| msgspec       | mixed       | 0.469    | 0.496       | 0.497    | 231.8    |
| strata        | mixed       | 0.551    | 0.587       | 0.739    | 231.8    |
| ujson         | mixed       | 1.493    | 1.521       | 1.706    | 231.8    |
| json (stdlib) | mixed       | 10.169   | 10.443      | 10.915   | 231.8    |
| msgspec       | nested      | 0.359    | 0.397       | 0.431    | 220.7    |
| orjson        | nested      | 0.360    | 0.436       | 0.505    | 220.7    |
| strata        | nested      | 0.444    | 0.448       | 0.542    | 220.7    |
| ujson         | nested      | 1.169    | 1.226       | 1.232    | 220.7    |
| json (stdlib) | nested      | 12.113   | 12.354      | 12.680   | 220.7    |
| strata        | users.json  | 6.453    | 7.778       | 11.471   | 163.8    |
| orjson        | users.json  | 9.001    | 9.267       | 9.338    | 181.9    |
| msgspec       | users.json  | 8.688    | 9.483       | 21.266   | 195.1    |
| ujson         | users.json  | 35.191   | 36.309      | 37.683   | 182.1    |
| json (stdlib) | users.json  | 365.441  | 365.868     | 366.555  | 195.1    |
| strata        | wide_arrays | 1.888    | 1.953       | 2.085    | 231.8    |
| orjson        | wide_arrays | 1.920    | 2.023       | 2.296    | 231.8    |
| msgspec       | wide_arrays | 2.158    | 2.168       | 2.281    | 231.8    |
| ujson         | wide_arrays | 8.101    | 8.717       | 8.733    | 231.8    |
| json (stdlib) | wide_arrays | 45.176   | 45.176      | 45.196   | 231.8    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 8.939    | 5000    | 196.3    |
|                                | orjson+jmespath    | 29.180   | 5000    | 196.4    |
|                                | orjson+jsonpath-ng | 35.384   | 5000    | 198.8    |
| all item prices                | strata             | 9.360    | 62331   | 199.1    |
|                                | orjson+jmespath    | 73.952   | 5000    | 204.0    |
|                                | orjson+jsonpath-ng | 225.566  | 62331   | 211.0    |
| recursive price                | strata             | 9.711    | 62331   | 208.5    |
|                                | orjson+jsonpath-ng | 707.302  | 62331   | 210.5    |
| flat:$.records\[\*\].id        | strata             | 0.372    | 500     | 222.1    |
|                                | orjson+jmespath    | 0.824    | 500     | 222.1    |
|                                | orjson+jsonpath-ng | 3.028    | 500     | 220.1    |
| flat:$.records\[0\]            | strata             | 0.391    | 1       | 220.1    |
|                                | orjson+jmespath    | 0.646    | 1       | 220.1    |
|                                | orjson+jsonpath-ng | 0.649    | 1       | 220.1    |
| nested:$.records\[\*\].id      | strata             | 0.327    | 500     | 220.7    |
|                                | orjson+jmespath    | 2.431    | 500     | 220.7    |
|                                | orjson+jsonpath-ng | 2.521    | 500     | 220.7    |
| nested:$.records\[0\]          | strata             | 0.324    | 1       | 220.7    |
|                                | orjson+jmespath    | 1.576    | 1       | 220.7    |
|                                | orjson+jsonpath-ng | 1.598    | 1       | 220.7    |
| wide_arrays:$.records\[\*\].id | strata             | 2.669    | 500     | 231.8    |
|                                | orjson+jmespath    | 4.220    | 500     | 231.8    |
|                                | orjson+jsonpath-ng | 4.773    | 500     | 231.8    |
| wide_arrays:$.records\[0\]     | strata             | 2.644    | 1       | 231.8    |
|                                | orjson+jsonpath-ng | 3.937    | 1       | 231.8    |
|                                | orjson+jmespath    | 3.986    | 1       | 231.8    |
| mixed:$.records\[\*\].id       | strata             | 0.478    | 500     | 231.8    |
|                                | orjson+jmespath    | 0.783    | 500     | 231.8    |
|                                | orjson+jsonpath-ng | 2.503    | 500     | 231.8    |
| mixed:$.records\[0\]           | strata             | 0.479    | 1       | 231.8    |
|                                | orjson+jsonpath-ng | 0.675    | 1       | 231.8    |
|                                | orjson+jmespath    | 0.704    | 1       | 231.8    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.746    | 5000    | 228.6    |
|                                            | jmespath    | 2.407    | 5000    | 228.6    |
|                                            | jsonpath-ng | 23.972   | 5000    | 230.7    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 4.761    | 62331   | 230.3    |
|                                            | jmespath    | 56.184   | 5000    | 234.3    |
|                                            | jsonpath-ng | 225.367  | 62331   | 238.8    |
| $..price                                   | strata      | 6.733    | 62331   | 236.7    |
|                                            | jsonpath-ng | 726.112  | 62331   | 239.2    |
| $.users\[?(@.age>30)\]                     | strata      | 0.880    | 3999    | 236.2    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 8.317    | 8402    | 236.2    |
| flat:$.records\[\*\].id                    | strata      | 0.032    | 500     | 220.1    |
|                                            | jmespath    | 0.172    | 500     | 220.1    |
|                                            | jsonpath-ng | 1.574    | 500     | 220.1    |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 220.1    |
|                                            | jmespath    | 0.005    | 1       | 220.1    |
|                                            | jsonpath-ng | 0.006    | 1       | 220.1    |
| nested:$.records\[\*\].id                  | strata      | 0.029    | 500     | 220.7    |
|                                            | jmespath    | 0.188    | 500     | 220.7    |
|                                            | jsonpath-ng | 1.820    | 500     | 220.7    |
| nested:$.records\[0\]                      | jmespath    | 0.005    | 1       | 220.7    |
|                                            | strata      | 0.006    | 1       | 220.7    |
|                                            | jsonpath-ng | 0.010    | 1       | 220.7    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.033    | 500     | 231.8    |
|                                            | jmespath    | 0.184    | 500     | 231.8    |
|                                            | jsonpath-ng | 2.305    | 500     | 231.8    |
| wide_arrays:$.records\[0\]                 | strata      | 0.004    | 1       | 231.8    |
|                                            | jsonpath-ng | 0.007    | 1       | 231.8    |
|                                            | jmespath    | 0.007    | 1       | 231.8    |
| mixed:$.records\[\*\].id                   | strata      | 0.032    | 500     | 231.8    |
|                                            | jmespath    | 0.172    | 500     | 231.8    |
|                                            | jsonpath-ng | 1.696    | 500     | 231.8    |
| mixed:$.records\[0\]                       | strata      | 0.004    | 1       | 231.8    |
|                                            | jsonpath-ng | 0.007    | 1       | 231.8    |
|                                            | jmespath    | 0.008    | 1       | 231.8    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **1.5% faster** than #2 (orjson)    |
| loads (NDJSON)      | **#1** / 5              | **58.9% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **20.0% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **54.6% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **41.9% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **34.6% faster** than #2 (msgspec)  |
| loads (flat)        | **#3** / 5              | 8.7% behind #1 (orjson)             |
| load (flat)         | **#1** / 5              | **2.5% faster** than #2 (orjson)    |
| dumps (flat)        | **#1** / 5              | **8.2% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **12.3% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **223.2% faster** than #2 (orjson)  |
| load (nested)       | **#1** / 5              | **178.9% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **7.4% faster** than #2 (orjson)    |
| dump (nested)       | **#3** / 5              | 23.7% behind #1 (msgspec)           |
| loads (wide_arrays) | **#1** / 5              | **3.8% faster** than #2 (orjson)    |
| load (wide_arrays)  | **#1** / 5              | **8.7% faster** than #2 (orjson)    |
| dumps (wide_arrays) | **#1** / 5              | **12.2% faster** than #2 (orjson)   |
| dump (wide_arrays)  | **#1** / 5              | **1.7% faster** than #2 (orjson)    |
| loads (mixed)       | **#3** / 5              | 40.8% behind #1 (orjson)            |
| load (mixed)        | **#3** / 5              | 17.7% behind #1 (msgspec)           |
| dumps (mixed)       | **#1** / 5              | **8.4% faster** than #2 (orjson)    |
| dump (mixed)        | **#3** / 5              | 17.5% behind #1 (msgspec)           |
| search (JSONPath)   | **#1** in 11/11 queries | avg **866% faster**                 |
| query (JSONPath)    | **#1** in 12/13 queries | -                                   |
