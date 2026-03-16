# Strata Benchmark Results

Generated: 2026-03-16 15:43:23

## Environment

- Commit: a676cc453da8a24953da243df7465313ffd5cbfd
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
| orjson        | flat         | 0.608    | 0.706       | 0.785    | 59.5     |
| msgspec       | flat         | 0.628    | 0.733       | 0.755    | 59.5     |
| strata        | flat         | 0.615    | 0.736       | 0.767    | 58.8     |
| ujson         | flat         | 1.104    | 1.275       | 1.291    | 59.5     |
| json (stdlib) | flat         | 2.078    | 2.221       | 2.274    | 59.5     |
| orjson        | mixed        | 0.543    | 0.637       | 0.704    | 77.0     |
| msgspec       | mixed        | 0.644    | 0.731       | 0.775    | 77.0     |
| strata        | mixed        | 0.707    | 0.795       | 0.800    | 77.0     |
| ujson         | mixed        | 0.996    | 1.162       | 1.192    | 77.0     |
| json (stdlib) | mixed        | 2.115    | 2.271       | 2.273    | 77.0     |
| strata        | nested       | 0.511    | 0.742       | 0.755    | 60.4     |
| msgspec       | nested       | 1.582    | 1.875       | 2.060    | 60.5     |
| orjson        | nested       | 1.731    | 2.218       | 2.861    | 60.5     |
| ujson         | nested       | 1.950    | 2.241       | 2.247    | 60.5     |
| json (stdlib) | nested       | 2.503    | 2.897       | 3.225    | 60.5     |
| strata        | users.json   | 3.250    | 4.089       | 4.294    | 34.3     |
| orjson        | users.json   | 4.437    | 6.458       | 6.791    | 38.2     |
| msgspec       | users.json   | 4.773    | 6.629       | 6.915    | 41.4     |
| ujson         | users.json   | 6.334    | 9.364       | 12.789   | 41.3     |
| json (stdlib) | users.json   | 8.674    | 10.501      | 10.679   | 44.3     |
| strata        | users.ndjson | 3.650    | 4.515       | 5.762    | 55.1     |
| orjson        | users.ndjson | 4.706    | 6.360       | 7.190    | 56.1     |
| msgspec       | users.ndjson | 4.712    | 6.391       | 6.525    | 57.1     |
| ujson         | users.ndjson | 6.255    | 8.756       | 9.337    | 57.1     |
| json (stdlib) | users.ndjson | 9.717    | 12.501      | 14.564   | 57.1     |
| strata        | wide_arrays  | 2.784    | 3.572       | 4.002    | 62.5     |
| orjson        | wide_arrays  | 4.480    | 4.625       | 5.888    | 65.1     |
| msgspec       | wide_arrays  | 4.425    | 5.104       | 5.154    | 65.2     |
| ujson         | wide_arrays  | 6.884    | 7.706       | 7.878    | 65.2     |
| json (stdlib) | wide_arrays  | 13.611   | 15.999      | 17.026   | 66.5     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 0.713    | 0.839       | 0.984    | 59.5     |
| orjson        | flat         | 0.687    | 0.940       | 1.100    | 59.5     |
| msgspec       | flat         | 0.877    | 1.059       | 1.120    | 60.0     |
| ujson         | flat         | 1.520    | 1.566       | 1.627    | 60.0     |
| json (stdlib) | flat         | 2.334    | 2.363       | 2.377    | 60.0     |
| orjson        | mixed        | 0.590    | 0.738       | 0.758    | 77.0     |
| strata        | mixed        | 0.722    | 0.861       | 0.880    | 77.0     |
| msgspec       | mixed        | 0.823    | 0.865       | 1.065    | 77.0     |
| ujson         | mixed        | 1.430    | 1.449       | 1.800    | 77.0     |
| json (stdlib) | mixed        | 2.216    | 2.320       | 2.489    | 77.0     |
| strata        | nested       | 0.586    | 0.710       | 0.755    | 60.5     |
| msgspec       | nested       | 1.766    | 1.979       | 1.979    | 60.5     |
| orjson        | nested       | 1.692    | 1.979       | 3.639    | 60.5     |
| ujson         | nested       | 2.770    | 2.906       | 3.131    | 60.5     |
| json (stdlib) | nested       | 2.783    | 4.604       | 5.098    | 60.5     |
| strata        | users.json   | 3.095    | 4.112       | 4.177    | 44.4     |
| orjson        | users.json   | 4.483    | 6.263       | 9.500    | 45.4     |
| msgspec       | users.json   | 4.949    | 7.261       | 11.000   | 48.5     |
| ujson         | users.json   | 7.082    | 8.988       | 9.544    | 48.5     |
| json (stdlib) | users.json   | 9.366    | 11.191      | 11.420   | 48.5     |
| strata        | users.ndjson | 4.136    | 4.703       | 5.409    | 57.1     |
| orjson        | users.ndjson | 4.717    | 6.371       | 6.577    | 57.1     |
| msgspec       | users.ndjson | 4.885    | 6.466       | 6.766    | 58.2     |
| ujson         | users.ndjson | 6.670    | 9.084       | 9.360    | 58.2     |
| json (stdlib) | users.ndjson | 9.550    | 12.169      | 14.031   | 58.2     |
| strata        | wide_arrays  | 3.796    | 3.810       | 3.832    | 66.5     |
| orjson        | wide_arrays  | 3.698    | 4.668       | 4.676    | 67.4     |
| msgspec       | wide_arrays  | 4.838    | 5.303       | 5.341    | 68.7     |
| ujson         | wide_arrays  | 7.363    | 7.892       | 7.962    | 68.7     |
| json (stdlib) | wide_arrays  | 15.101   | 16.583      | 17.195   | 70.0     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.248    | 0.262       | 0.267    | 60.0     |
| strata  | flat        | 0.274    | 0.279       | 0.280    | 60.0     |
| msgspec | flat        | 0.315    | 0.315       | 0.319    | 60.0     |
| ujson   | flat        | 1.240    | 1.243       | 1.264    | 60.4     |
| json    | flat        | 2.321    | 2.342       | 2.352    | 60.4     |
| orjson  | mixed       | 0.280    | 0.282       | 0.363    | 77.0     |
| msgspec | mixed       | 0.310    | 0.316       | 0.346    | 77.0     |
| strata  | mixed       | 0.310    | 0.334       | 0.365    | 77.0     |
| ujson   | mixed       | 1.345    | 1.351       | 1.355    | 77.0     |
| json    | mixed       | 3.035    | 3.055       | 3.071    | 77.0     |
| strata  | nested      | 0.175    | 0.177       | 0.181    | 60.5     |
| orjson  | nested      | 0.199    | 0.200       | 0.209    | 60.5     |
| msgspec | nested      | 0.203    | 0.214       | 0.218    | 60.5     |
| ujson   | nested      | 1.001    | 1.008       | 1.022    | 60.5     |
| json    | nested      | 1.355    | 1.394       | 1.429    | 60.5     |
| strata  | users.json  | 1.154    | 1.157       | 1.248    | 49.1     |
| orjson  | users.json  | 1.531    | 1.564       | 1.830    | 49.9     |
| msgspec | users.json  | 1.594    | 1.597       | 1.643    | 51.7     |
| ujson   | users.json  | 6.430    | 6.434       | 6.548    | 51.7     |
| json    | users.json  | 8.987    | 8.990       | 9.429    | 52.0     |
| orjson  | wide_arrays | 1.548    | 1.582       | 1.835    | 73.1     |
| msgspec | wide_arrays | 1.666    | 1.701       | 1.784    | 74.4     |
| strata  | wide_arrays | 1.745    | 1.756       | 1.758    | 71.2     |
| ujson   | wide_arrays | 7.491    | 7.795       | 7.803    | 76.9     |
| json    | wide_arrays | 19.897   | 20.129      | 20.279   | 77.0     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.454    | 0.456       | 0.470    | 60.4     |
| orjson        | flat        | 0.450    | 0.522       | 0.558    | 60.4     |
| msgspec       | flat        | 0.543    | 0.559       | 0.618    | 60.4     |
| ujson         | flat        | 1.474    | 1.511       | 1.536    | 60.4     |
| json (stdlib) | flat        | 8.594    | 8.666       | 8.713    | 60.4     |
| orjson        | mixed       | 0.458    | 0.465       | 0.471    | 77.0     |
| strata        | mixed       | 0.461    | 0.486       | 0.511    | 77.0     |
| msgspec       | mixed       | 0.559    | 0.732       | 0.767    | 77.0     |
| ujson         | mixed       | 1.511    | 1.542       | 1.585    | 77.0     |
| json (stdlib) | mixed       | 10.229   | 10.345      | 10.414   | 77.0     |
| strata        | nested      | 0.318    | 0.327       | 0.333    | 60.5     |
| orjson        | nested      | 0.360    | 0.361       | 0.366    | 60.5     |
| msgspec       | nested      | 0.375    | 0.388       | 0.397    | 60.5     |
| ujson         | nested      | 1.169    | 1.230       | 1.266    | 60.5     |
| json (stdlib) | nested      | 12.093   | 12.147      | 12.395   | 60.5     |
| strata        | users.json  | 1.752    | 1.990       | 2.019    | 52.0     |
| msgspec       | users.json  | 1.864    | 2.061       | 2.318    | 52.0     |
| orjson        | users.json  | 2.133    | 2.145       | 2.200    | 52.0     |
| ujson         | users.json  | 7.161    | 7.171       | 7.206    | 52.0     |
| json (stdlib) | users.json  | 68.312   | 68.402      | 68.477   | 52.0     |
| strata        | wide_arrays | 1.983    | 2.015       | 2.055    | 77.0     |
| orjson        | wide_arrays | 1.917    | 2.037       | 2.242    | 77.0     |
| msgspec       | wide_arrays | 2.095    | 2.659       | 2.827    | 77.0     |
| ujson         | wide_arrays | 7.537    | 7.596       | 7.673    | 77.0     |
| json (stdlib) | wide_arrays | 45.640   | 46.044      | 46.381   | 77.0     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.411    | 1000    | 52.1     |
|                                | orjson+jmespath    | 5.942    | 1000    | 52.2     |
|                                | orjson+jsonpath-ng | 6.974    | 1000    | 52.8     |
| all item prices                | strata             | 1.591    | 12046   | 52.8     |
|                                | orjson+jmespath    | 14.023   | 1000    | 52.8     |
|                                | orjson+jsonpath-ng | 37.695   | 12046   | 52.7     |
| recursive price                | strata             | 1.668    | 12046   | 52.3     |
|                                | orjson+jsonpath-ng | 130.116  | 12046   | 53.2     |
| flat:$.records\[\*\].id        | strata             | 0.418    | 500     | 60.4     |
|                                | orjson+jmespath    | 0.875    | 500     | 60.4     |
|                                | orjson+jsonpath-ng | 2.374    | 500     | 60.4     |
| flat:$.records\[0\]            | strata             | 0.431    | 1       | 60.4     |
|                                | orjson+jsonpath-ng | 0.696    | 1       | 60.4     |
|                                | orjson+jmespath    | 0.699    | 1       | 60.4     |
| nested:$.records\[\*\].id      | strata             | 0.310    | 500     | 60.5     |
|                                | orjson+jmespath    | 1.829    | 500     | 60.5     |
|                                | orjson+jsonpath-ng | 2.329    | 500     | 60.5     |
| nested:$.records\[0\]          | strata             | 0.309    | 1       | 60.5     |
|                                | orjson+jsonpath-ng | 1.830    | 1       | 60.5     |
|                                | orjson+jmespath    | 2.058    | 1       | 60.5     |
| wide_arrays:$.records\[\*\].id | strata             | 2.407    | 500     | 77.0     |
|                                | orjson+jmespath    | 4.186    | 500     | 77.0     |
|                                | orjson+jsonpath-ng | 4.950    | 500     | 77.0     |
| wide_arrays:$.records\[0\]     | strata             | 2.358    | 1       | 77.0     |
|                                | orjson+jmespath    | 4.217    | 1       | 77.0     |
|                                | orjson+jsonpath-ng | 4.272    | 1       | 77.0     |
| mixed:$.records\[\*\].id       | strata             | 0.457    | 500     | 77.0     |
|                                | orjson+jmespath    | 0.804    | 500     | 77.0     |
|                                | orjson+jsonpath-ng | 2.423    | 500     | 77.0     |
| mixed:$.records\[0\]           | strata             | 0.459    | 1       | 77.0     |
|                                | orjson+jmespath    | 0.628    | 1       | 77.0     |
|                                | orjson+jsonpath-ng | 0.702    | 1       | 77.0     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.076    | 1000    | 54.0     |
|                                            | jmespath    | 0.392    | 1000    | 54.0     |
|                                            | jsonpath-ng | 4.137    | 1000    | 54.0     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.971    | 12046   | 54.1     |
|                                            | jmespath    | 10.795   | 1000    | 54.1     |
|                                            | jsonpath-ng | 32.737   | 12046   | 56.8     |
| $..price                                   | strata      | 1.304    | 12046   | 56.8     |
|                                            | jsonpath-ng | 124.893  | 12046   | 58.8     |
| $.users\[?(@.age>30)\]                     | strata      | 0.120    | 794     | 55.1     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.506    | 1604    | 55.1     |
| flat:$.records\[\*\].id                    | strata      | 0.033    | 500     | 60.4     |
|                                            | jmespath    | 0.173    | 500     | 60.4     |
|                                            | jsonpath-ng | 2.223    | 500     | 60.4     |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 60.4     |
|                                            | jmespath    | 0.005    | 1       | 60.4     |
|                                            | jsonpath-ng | 0.006    | 1       | 60.4     |
| nested:$.records\[\*\].id                  | strata      | 0.027    | 500     | 60.5     |
|                                            | jmespath    | 0.176    | 500     | 60.5     |
|                                            | jsonpath-ng | 1.943    | 500     | 60.5     |
| nested:$.records\[0\]                      | strata      | 0.002    | 1       | 60.5     |
|                                            | jmespath    | 0.006    | 1       | 60.5     |
|                                            | jsonpath-ng | 0.008    | 1       | 60.5     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.028    | 500     | 77.0     |
|                                            | jmespath    | 0.178    | 500     | 77.0     |
|                                            | jsonpath-ng | 2.239    | 500     | 77.0     |
| wide_arrays:$.records\[0\]                 | strata      | 0.003    | 1       | 77.0     |
|                                            | jsonpath-ng | 0.007    | 1       | 77.0     |
|                                            | jmespath    | 0.009    | 1       | 77.0     |
| mixed:$.records\[\*\].id                   | strata      | 0.031    | 500     | 77.0     |
|                                            | jmespath    | 0.174    | 500     | 77.0     |
|                                            | jsonpath-ng | 1.714    | 500     | 77.0     |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 77.0     |
|                                            | jsonpath-ng | 0.010    | 1       | 77.0     |
|                                            | jmespath    | 0.013    | 1       | 77.0     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **36.5% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **29.0% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **44.8% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **14.0% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **32.6% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **6.4% faster** than #2 (msgspec)   |
| loads (flat)        | **#2** / 5              | 1.2% behind #1 (orjson)             |
| load (flat)         | **#2** / 5              | 3.8% behind #1 (orjson)             |
| dumps (flat)        | **#2** / 5              | 10.5% behind #1 (orjson)            |
| dump (flat)         | **#2** / 5              | 0.9% behind #1 (orjson)             |
| loads (nested)      | **#1** / 5              | **209.8% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **188.7% faster** than #2 (orjson)  |
| dumps (nested)      | **#1** / 5              | **13.9% faster** than #2 (orjson)   |
| dump (nested)       | **#1** / 5              | **13.1% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#1** / 5              | **58.9% faster** than #2 (msgspec)  |
| load (wide_arrays)  | **#2** / 5              | 2.6% behind #1 (orjson)             |
| dumps (wide_arrays) | **#3** / 5              | 12.7% behind #1 (orjson)            |
| dump (wide_arrays)  | **#2** / 5              | 3.4% behind #1 (orjson)             |
| loads (mixed)       | **#3** / 5              | 30.2% behind #1 (orjson)            |
| load (mixed)        | **#2** / 5              | 22.4% behind #1 (orjson)            |
| dumps (mixed)       | **#2** / 5              | 10.5% behind #1 (orjson)            |
| dump (mixed)        | **#2** / 5              | 0.8% behind #1 (orjson)             |
| search (JSONPath)   | **#1** in 11/11 queries | avg **929% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
