# Strata Benchmark Results

Generated: 2026-03-19 17:51:30

## Environment

- Commit: f6a531005609a864792e4e9f097a7fc572656b95
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
| strata        | flat         | 2.875    | 3.336       | 3.499    | 271.4    |
| msgspec       | flat         | 4.229    | 4.927       | 4.933    | 273.9    |
| orjson        | flat         | 6.014    | 7.157       | 7.881    | 273.9    |
| ujson         | flat         | 6.191    | 7.642       | 8.022    | 273.9    |
| json (stdlib) | flat         | 10.221   | 11.107      | 11.389   | 273.9    |
| strata        | mixed        | 3.120    | 3.320       | 3.667    | 289.7    |
| orjson        | mixed        | 4.987    | 6.098       | 6.384    | 289.7    |
| msgspec       | mixed        | 5.350    | 6.738       | 7.139    | 289.7    |
| ujson         | mixed        | 6.588    | 7.087       | 7.454    | 289.7    |
| json (stdlib) | mixed        | 10.462   | 12.772      | 13.301   | 289.7    |
| strata        | nested       | 2.726    | 3.289       | 3.644    | 277.6    |
| msgspec       | nested       | 5.316    | 8.494       | 9.880    | 277.6    |
| ujson         | nested       | 6.556    | 8.564       | 9.287    | 277.6    |
| orjson        | nested       | 7.886    | 8.611       | 8.836    | 277.6    |
| json (stdlib) | nested       | 10.487   | 10.669      | 13.031   | 277.6    |
| strata        | users.json   | 26.027   | 33.430      | 35.252   | 73.5     |
| orjson        | users.json   | 32.163   | 39.509      | 50.472   | 93.4     |
| msgspec       | users.json   | 29.427   | 41.515      | 43.695   | 92.2     |
| ujson         | users.json   | 42.202   | 59.439      | 71.219   | 109.9    |
| json (stdlib) | users.json   | 54.533   | 71.668      | 90.467   | 98.5     |
| strata        | users.ndjson | 26.578   | 32.893      | 33.403   | 253.7    |
| orjson        | users.ndjson | 32.967   | 35.549      | 36.122   | 261.8    |
| msgspec       | users.ndjson | 37.273   | 37.780      | 39.295   | 262.9    |
| ujson         | users.ndjson | 50.271   | 53.822      | 69.249   | 261.9    |
| json (stdlib) | users.ndjson | 70.935   | 71.097      | 71.540   | 263.9    |
| strata        | wide_arrays  | 11.887   | 14.270      | 14.326   | 289.8    |
| orjson        | wide_arrays  | 12.364   | 16.164      | 16.441   | 300.0    |
| msgspec       | wide_arrays  | 19.300   | 20.462      | 22.573   | 300.1    |
| ujson         | wide_arrays  | 25.939   | 29.695      | 30.130   | 300.1    |
| json (stdlib) | wide_arrays  | 53.292   | 55.207      | 55.619   | 300.1    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 3.521    | 3.742       | 3.851    | 273.9    |
| orjson        | flat         | 4.272    | 4.555       | 4.790    | 273.9    |
| msgspec       | flat         | 4.949    | 5.622       | 6.574    | 273.9    |
| ujson         | flat         | 6.025    | 8.466       | 8.565    | 273.9    |
| json (stdlib) | flat         | 11.907   | 11.965      | 12.320   | 273.9    |
| strata        | mixed        | 3.071    | 3.614       | 3.622    | 289.7    |
| msgspec       | mixed        | 5.331    | 7.891       | 15.659   | 289.7    |
| ujson         | mixed        | 7.384    | 8.346       | 8.423    | 289.7    |
| orjson        | mixed        | 8.054    | 9.144       | 9.436    | 289.7    |
| json (stdlib) | mixed        | 11.388   | 11.447      | 11.453   | 289.7    |
| strata        | nested       | 3.012    | 3.552       | 3.650    | 277.6    |
| ujson         | nested       | 6.811    | 8.277       | 10.274   | 277.6    |
| msgspec       | nested       | 5.908    | 8.511       | 8.612    | 277.6    |
| orjson        | nested       | 8.227    | 8.580       | 8.888    | 277.6    |
| json (stdlib) | nested       | 8.133    | 9.491       | 9.520    | 277.6    |
| strata        | users.json   | 25.492   | 31.269      | 32.376   | 99.9     |
| orjson        | users.json   | 26.612   | 40.264      | 50.922   | 99.9     |
| msgspec       | users.json   | 28.808   | 43.744      | 55.714   | 104.8    |
| ujson         | users.json   | 42.296   | 57.843      | 61.446   | 122.5    |
| json (stdlib) | users.json   | 58.520   | 68.376      | 70.658   | 104.8    |
| strata        | users.ndjson | 26.913   | 33.036      | 33.741   | 265.9    |
| orjson        | users.ndjson | 37.094   | 37.663      | 45.823   | 266.9    |
| msgspec       | users.ndjson | 40.296   | 47.193      | 52.945   | 267.9    |
| ujson         | users.ndjson | 48.084   | 54.362      | 56.122   | 266.9    |
| json (stdlib) | users.ndjson | 63.377   | 64.349      | 65.646   | 268.9    |
| strata        | wide_arrays  | 12.483   | 17.288      | 19.543   | 300.1    |
| orjson        | wide_arrays  | 13.309   | 18.643      | 18.978   | 299.7    |
| msgspec       | wide_arrays  | 17.073   | 20.548      | 24.096   | 299.7    |
| ujson         | wide_arrays  | 27.236   | 30.443      | 32.541   | 299.7    |
| json (stdlib) | wide_arrays  | 56.204   | 58.322      | 59.191   | 243.4    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.975    | 0.981       | 1.146    | 273.9    |
| orjson  | flat        | 0.996    | 0.998       | 1.569    | 276.2    |
| msgspec | flat        | 1.159    | 1.403       | 1.884    | 277.3    |
| ujson   | flat        | 4.851    | 5.976       | 8.176    | 277.3    |
| json    | flat        | 9.758    | 11.866      | 16.127   | 277.3    |
| orjson  | mixed       | 1.074    | 1.100       | 1.171    | 289.7    |
| strata  | mixed       | 1.102    | 1.105       | 1.107    | 289.7    |
| msgspec | mixed       | 1.171    | 1.193       | 1.255    | 289.7    |
| ujson   | mixed       | 5.163    | 5.272       | 5.285    | 289.7    |
| json    | mixed       | 11.323   | 11.338      | 12.507   | 289.7    |
| strata  | nested      | 0.786    | 0.786       | 0.824    | 277.6    |
| orjson  | nested      | 0.788    | 0.794       | 0.796    | 277.6    |
| msgspec | nested      | 0.796    | 0.796       | 0.857    | 277.6    |
| ujson   | nested      | 4.124    | 4.221       | 4.277    | 277.6    |
| json    | nested      | 5.236    | 5.428       | 5.530    | 277.6    |
| strata  | users.json  | 8.576    | 8.592       | 8.643    | 108.9    |
| msgspec | users.json  | 11.086   | 11.089      | 11.170   | 159.5    |
| orjson  | users.json  | 11.569   | 11.939      | 12.375   | 131.2    |
| ujson   | users.json  | 41.235   | 41.523      | 42.009   | 165.4    |
| json    | users.json  | 59.471   | 59.510      | 61.921   | 192.1    |
| strata  | wide_arrays | 6.119    | 6.460       | 6.590    | 243.4    |
| msgspec | wide_arrays | 6.917    | 6.927       | 6.971    | 253.6    |
| orjson  | wide_arrays | 7.357    | 7.402       | 7.471    | 233.8    |
| ujson   | wide_arrays | 28.587   | 29.246      | 29.351   | 255.5    |
| json    | wide_arrays | 80.136   | 80.449      | 80.481   | 276.4    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 1.518    | 1.530       | 1.599    | 277.3    |
| strata        | flat        | 1.387    | 1.701       | 4.803    | 277.3    |
| msgspec       | flat        | 1.649    | 2.000       | 2.098    | 277.5    |
| ujson         | flat        | 5.424    | 5.947       | 11.676   | 277.5    |
| json (stdlib) | flat        | 32.771   | 32.850      | 33.217   | 277.5    |
| strata        | mixed       | 1.325    | 1.350       | 1.402    | 289.7    |
| msgspec       | mixed       | 1.562    | 1.940       | 1.957    | 289.7    |
| orjson        | mixed       | 1.597    | 2.134       | 2.616    | 289.7    |
| ujson         | mixed       | 5.853    | 5.881       | 6.230    | 289.7    |
| json (stdlib) | mixed       | 39.151   | 39.395      | 39.553   | 289.7    |
| orjson        | nested      | 1.187    | 1.236       | 1.454    | 277.6    |
| strata        | nested      | 1.109    | 1.272       | 1.345    | 277.6    |
| msgspec       | nested      | 1.623    | 1.642       | 1.731    | 277.6    |
| ujson         | nested      | 4.364    | 4.404       | 4.477    | 277.6    |
| json (stdlib) | nested      | 47.740   | 47.996      | 48.722   | 277.6    |
| strata        | users.json  | 9.528    | 10.350      | 11.525   | 198.4    |
| msgspec       | users.json  | 11.864   | 11.903      | 12.272   | 243.7    |
| orjson        | users.json  | 11.719   | 12.658      | 14.107   | 220.7    |
| ujson         | users.json  | 43.093   | 43.189      | 43.329   | 221.3    |
| json (stdlib) | users.json  | 435.639  | 440.263     | 455.428  | 243.7    |
| strata        | wide_arrays | 7.583    | 7.594       | 7.645    | 276.4    |
| msgspec       | wide_arrays | 8.212    | 8.225       | 8.780    | 289.7    |
| orjson        | wide_arrays | 8.649    | 8.849       | 9.816    | 274.2    |
| ujson         | wide_arrays | 29.610   | 30.300      | 30.549   | 276.1    |
| json (stdlib) | wide_arrays | 183.080  | 183.432     | 191.436  | 289.7    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 12.630   | 2000    | 240.8    |
|                                | orjson+jmespath    | 30.261   | 2000    | 243.9    |
|                                | orjson+jsonpath-ng | 36.986   | 2000    | 246.2    |
| all item prices                | strata             | 15.042   | 100999  | 246.6    |
|                                | orjson+jmespath    | 80.980   | 2000    | 255.8    |
|                                | orjson+jsonpath-ng | 267.904  | 100999  | 263.8    |
| recursive price                | strata             | 13.403   | 100999  | 261.6    |
|                                | orjson+jsonpath-ng | 875.266  | 100999  | 264.9    |
| flat:$.records\[\*\].id        | strata             | 1.830    | 2000    | 277.6    |
|                                | orjson+jmespath    | 4.634    | 2000    | 277.6    |
|                                | orjson+jsonpath-ng | 6.836    | 2000    | 277.6    |
| flat:$.records\[0\]            | strata             | 1.922    | 1       | 277.6    |
|                                | orjson+jsonpath-ng | 4.558    | 1       | 277.6    |
|                                | orjson+jmespath    | 5.530    | 1       | 277.6    |
| nested:$.records\[\*\].id      | strata             | 1.403    | 2000    | 277.6    |
|                                | orjson+jmespath    | 7.893    | 2000    | 277.6    |
|                                | orjson+jsonpath-ng | 9.234    | 2000    | 277.6    |
| nested:$.records\[0\]          | strata             | 1.439    | 1       | 277.6    |
|                                | orjson+jmespath    | 5.950    | 1       | 277.6    |
|                                | orjson+jsonpath-ng | 6.516    | 1       | 277.6    |
| wide_arrays:$.records\[\*\].id | strata             | 9.356    | 2000    | 289.7    |
|                                | orjson+jmespath    | 16.202   | 2000    | 289.7    |
|                                | orjson+jsonpath-ng | 19.713   | 2000    | 289.7    |
| wide_arrays:$.records\[0\]     | strata             | 9.690    | 1       | 289.7    |
|                                | orjson+jsonpath-ng | 14.589   | 1       | 289.7    |
|                                | orjson+jmespath    | 16.602   | 1       | 289.7    |
| mixed:$.records\[\*\].id       | strata             | 1.806    | 2000    | 289.7    |
|                                | orjson+jmespath    | 5.327    | 2000    | 289.7    |
|                                | orjson+jsonpath-ng | 8.101    | 2000    | 289.7    |
| mixed:$.records\[0\]           | strata             | 1.824    | 1       | 289.7    |
|                                | orjson+jmespath    | 4.478    | 1       | 289.7    |
|                                | orjson+jsonpath-ng | 4.653    | 1       | 289.7    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.332    | 2000    | 287.1    |
|                                            | jmespath    | 0.932    | 2000    | 287.1    |
|                                            | jsonpath-ng | 18.291   | 2000    | 287.9    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.026    | 100999  | 288.0    |
|                                            | jmespath    | 62.019   | 2000    | 292.8    |
|                                            | jsonpath-ng | 250.002  | 100999  | 294.1    |
| $..price                                   | strata      | 8.062    | 100999  | 295.4    |
|                                            | jsonpath-ng | 866.739  | 100999  | 278.1    |
| $.users\[?(@.age>30)\]                     | strata      | 0.342    | 1606    | 276.1    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 8.967    | 6650    | 276.1    |
| flat:$.records\[\*\].id                    | strata      | 0.125    | 2000    | 277.6    |
|                                            | jmespath    | 0.729    | 2000    | 277.6    |
|                                            | jsonpath-ng | 4.805    | 2000    | 277.6    |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 277.6    |
|                                            | jmespath    | 0.006    | 1       | 277.6    |
|                                            | jsonpath-ng | 0.018    | 1       | 277.6    |
| nested:$.records\[\*\].id                  | strata      | 0.113    | 2000    | 277.6    |
|                                            | jmespath    | 0.706    | 2000    | 277.6    |
|                                            | jsonpath-ng | 5.508    | 2000    | 277.6    |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 277.6    |
|                                            | jmespath    | 0.008    | 1       | 277.6    |
|                                            | jsonpath-ng | 0.015    | 1       | 277.6    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.110    | 2000    | 289.7    |
|                                            | jmespath    | 0.688    | 2000    | 289.7    |
|                                            | jsonpath-ng | 7.114    | 2000    | 289.7    |
| wide_arrays:$.records\[0\]                 | strata      | 0.009    | 1       | 289.7    |
|                                            | jsonpath-ng | 0.024    | 1       | 289.7    |
|                                            | jmespath    | 0.027    | 1       | 289.7    |
| mixed:$.records\[\*\].id                   | strata      | 0.143    | 2000    | 289.7    |
|                                            | jmespath    | 0.944    | 2000    | 289.7    |
|                                            | jsonpath-ng | 5.776    | 2000    | 289.7    |
| mixed:$.records\[0\]                       | jmespath    | 0.008    | 1       | 289.7    |
|                                            | jsonpath-ng | 0.008    | 1       | 289.7    |
|                                            | strata      | 0.011    | 1       | 289.7    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **13.1% faster** than #2 (msgspec) |
| loads (NDJSON)      | **#1** / 5              | **24.0% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **4.4% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **37.8% faster** than #2 (orjson)  |
| dumps               | **#1** / 5              | **29.3% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **23.0% faster** than #2 (orjson)  |
| loads (flat)        | **#1** / 5              | **47.1% faster** than #2 (msgspec) |
| load (flat)         | **#1** / 5              | **21.3% faster** than #2 (orjson)  |
| dumps (flat)        | **#1** / 5              | **2.2% faster** than #2 (orjson)   |
| dump (flat)         | **#1** / 5              | **9.4% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **95.0% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **96.2% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **0.3% faster** than #2 (orjson)   |
| dump (nested)       | **#1** / 5              | **7.0% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#1** / 5              | **4.0% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **6.6% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **13.0% faster** than #2 (msgspec) |
| dump (wide_arrays)  | **#1** / 5              | **8.3% faster** than #2 (msgspec)  |
| loads (mixed)       | **#1** / 5              | **59.8% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **73.6% faster** than #2 (msgspec) |
| dumps (mixed)       | **#2** / 5              | 2.6% behind #1 (orjson)            |
| dump (mixed)        | **#1** / 5              | **17.9% faster** than #2 (msgspec) |
| search (JSONPath)   | **#1** in 11/11 queries | avg **776% faster**                |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
