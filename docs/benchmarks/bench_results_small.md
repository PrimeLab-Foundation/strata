# Strata Benchmark Results

Generated: 2026-03-18 19:16:01

## Environment

- Commit: 23a4c6bba7eea184d34febf8fc1d36f00a2d9935
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
| orjson        | flat         | 0.741    | 0.748       | 0.851    | 61.5     |
| msgspec       | flat         | 0.830    | 0.877       | 0.887    | 61.5     |
| strata        | flat         | 0.769    | 0.929       | 1.025    | 60.8     |
| ujson         | flat         | 1.438    | 1.505       | 1.600    | 61.5     |
| json (stdlib) | flat         | 2.448    | 2.529       | 2.571    | 61.5     |
| orjson        | mixed        | 0.603    | 0.791       | 0.901    | 75.8     |
| msgspec       | mixed        | 0.701    | 0.880       | 0.918    | 75.8     |
| strata        | mixed        | 0.900    | 0.925       | 0.941    | 75.8     |
| ujson         | mixed        | 1.331    | 1.398       | 1.409    | 75.8     |
| json (stdlib) | mixed        | 2.517    | 2.521       | 2.544    | 75.8     |
| strata        | nested       | 0.646    | 0.780       | 0.809    | 61.2     |
| msgspec       | nested       | 2.217    | 2.325       | 2.562    | 61.2     |
| ujson         | nested       | 1.932    | 2.340       | 2.526    | 61.2     |
| orjson        | nested       | 2.117    | 2.383       | 2.445    | 61.2     |
| json (stdlib) | nested       | 3.135    | 3.327       | 3.345    | 61.2     |
| strata        | users.json   | 4.949    | 5.799       | 6.239    | 34.0     |
| msgspec       | users.json   | 8.855    | 9.118       | 9.499    | 41.1     |
| orjson        | users.json   | 8.484    | 9.664       | 9.898    | 38.0     |
| ujson         | users.json   | 9.479    | 11.263      | 13.850   | 41.0     |
| json (stdlib) | users.json   | 11.099   | 13.559      | 14.110   | 42.9     |
| strata        | users.ndjson | 5.460    | 5.673       | 6.494    | 56.2     |
| orjson        | users.ndjson | 6.172    | 7.487       | 8.870    | 57.2     |
| msgspec       | users.ndjson | 6.557    | 8.143       | 8.893    | 59.2     |
| ujson         | users.ndjson | 10.939   | 11.229      | 12.633   | 59.2     |
| json (stdlib) | users.ndjson | 11.606   | 14.824      | 15.971   | 59.2     |
| strata        | wide_arrays  | 2.917    | 3.378       | 3.688    | 64.3     |
| orjson        | wide_arrays  | 4.591    | 4.914       | 5.770    | 66.9     |
| msgspec       | wide_arrays  | 5.748    | 5.874       | 6.099    | 66.9     |
| ujson         | wide_arrays  | 7.859    | 7.931       | 8.360    | 66.9     |
| json (stdlib) | wide_arrays  | 16.443   | 18.146      | 19.062   | 66.9     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 1.004    | 1.013       | 1.086    | 61.5     |
| orjson        | flat         | 0.990    | 1.030       | 1.121    | 61.5     |
| msgspec       | flat         | 0.901    | 1.074       | 1.454    | 61.7     |
| ujson         | flat         | 1.623    | 1.750       | 1.965    | 61.7     |
| json (stdlib) | flat         | 2.483    | 2.568       | 2.784    | 61.7     |
| msgspec       | mixed        | 0.761    | 0.899       | 0.944    | 75.8     |
| orjson        | mixed        | 0.726    | 0.930       | 1.415    | 75.8     |
| strata        | mixed        | 1.003    | 1.009       | 1.061    | 75.8     |
| ujson         | mixed        | 1.286    | 1.532       | 2.625    | 75.8     |
| json (stdlib) | mixed        | 2.342    | 2.406       | 2.672    | 75.8     |
| strata        | nested       | 0.706    | 0.834       | 0.879    | 61.2     |
| orjson        | nested       | 1.699    | 2.158       | 2.470    | 61.2     |
| ujson         | nested       | 2.063    | 2.334       | 2.406    | 61.2     |
| msgspec       | nested       | 2.007    | 4.002       | 4.512    | 61.2     |
| json (stdlib) | nested       | 5.209    | 5.365       | 6.461    | 61.2     |
| strata        | users.json   | 4.723    | 5.770       | 6.747    | 43.0     |
| orjson        | users.json   | 7.033    | 7.834       | 7.891    | 43.9     |
| msgspec       | users.json   | 6.732    | 8.342       | 9.526    | 48.1     |
| ujson         | users.json   | 11.714   | 12.690      | 20.002   | 48.1     |
| json (stdlib) | users.json   | 11.309   | 14.058      | 16.640   | 48.1     |
| strata        | users.ndjson | 5.042    | 5.801       | 5.971    | 59.2     |
| orjson        | users.ndjson | 6.718    | 8.216       | 11.838   | 59.2     |
| msgspec       | users.ndjson | 5.903    | 8.265       | 8.594    | 60.2     |
| ujson         | users.ndjson | 8.840    | 11.430      | 15.782   | 60.2     |
| json (stdlib) | users.ndjson | 13.023   | 14.019      | 14.373   | 60.2     |
| strata        | wide_arrays  | 3.174    | 3.884       | 3.904    | 68.2     |
| orjson        | wide_arrays  | 5.067    | 5.838       | 9.981    | 69.5     |
| msgspec       | wide_arrays  | 5.441    | 6.143       | 8.491    | 69.9     |
| ujson         | wide_arrays  | 9.315    | 9.444       | 9.885    | 69.9     |
| json (stdlib) | wide_arrays  | 17.739   | 18.582      | 19.280   | 71.2     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.281    | 0.284       | 0.285    | 61.7     |
| strata  | flat        | 0.274    | 0.286       | 0.293    | 61.7     |
| msgspec | flat        | 0.361    | 0.361       | 0.378    | 61.8     |
| ujson   | flat        | 1.404    | 1.487       | 1.547    | 62.2     |
| json    | flat        | 2.590    | 2.610       | 3.509    | 62.2     |
| msgspec | mixed       | 0.330    | 0.356       | 0.361    | 75.8     |
| orjson  | mixed       | 0.345    | 0.398       | 0.461    | 75.8     |
| strata  | mixed       | 0.311    | 0.400       | 0.515    | 75.8     |
| ujson   | mixed       | 1.518    | 1.550       | 2.289    | 75.8     |
| json    | mixed       | 3.382    | 3.412       | 3.968    | 75.8     |
| orjson  | nested      | 0.226    | 0.238       | 0.241    | 61.2     |
| msgspec | nested      | 0.237    | 0.245       | 0.251    | 61.2     |
| strata  | nested      | 0.240    | 0.250       | 0.253    | 61.2     |
| ujson   | nested      | 1.066    | 1.142       | 1.178    | 61.2     |
| json    | nested      | 1.526    | 1.543       | 1.588    | 61.2     |
| strata  | users.json  | 1.309    | 1.327       | 1.460    | 49.0     |
| orjson  | users.json  | 1.851    | 1.858       | 1.903    | 51.9     |
| msgspec | users.json  | 1.831    | 1.878       | 1.927    | 52.2     |
| ujson   | users.json  | 7.848    | 7.851       | 7.898    | 52.2     |
| json    | users.json  | 11.021   | 11.212      | 11.407   | 52.7     |
| strata  | wide_arrays | 1.666    | 1.720       | 1.726    | 72.4     |
| orjson  | wide_arrays | 1.775    | 1.783       | 1.949    | 74.5     |
| msgspec | wide_arrays | 1.751    | 1.913       | 1.913    | 74.5     |
| ujson   | wide_arrays | 8.075    | 8.234       | 8.575    | 75.7     |
| json    | wide_arrays | 21.904   | 22.148      | 23.392   | 75.7     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.485    | 0.534       | 0.649    | 62.2     |
| msgspec       | flat        | 0.629    | 0.722       | 0.836    | 62.2     |
| orjson        | flat        | 0.644    | 0.754       | 0.976    | 62.2     |
| ujson         | flat        | 1.859    | 1.894       | 2.459    | 62.2     |
| json (stdlib) | flat        | 9.771    | 10.107      | 10.731   | 62.2     |
| orjson        | mixed       | 0.536    | 0.562       | 0.576    | 75.8     |
| msgspec       | mixed       | 0.597    | 0.659       | 1.076    | 75.8     |
| strata        | mixed       | 0.617    | 0.673       | 0.965    | 75.8     |
| ujson         | mixed       | 1.853    | 1.877       | 1.995    | 75.8     |
| json (stdlib) | mixed       | 11.661   | 12.034      | 12.906   | 75.8     |
| orjson        | nested      | 0.410    | 0.423       | 0.428    | 61.2     |
| strata        | nested      | 0.399    | 0.450       | 0.462    | 61.2     |
| msgspec       | nested      | 0.477    | 0.534       | 0.581    | 61.2     |
| ujson         | nested      | 1.291    | 1.311       | 1.321    | 61.2     |
| json (stdlib) | nested      | 13.883   | 14.227      | 14.782   | 61.2     |
| strata        | users.json  | 1.854    | 2.204       | 2.297    | 52.7     |
| orjson        | users.json  | 2.391    | 2.429       | 3.100    | 52.7     |
| msgspec       | users.json  | 2.135    | 2.573       | 2.763    | 52.7     |
| ujson         | users.json  | 8.379    | 8.464       | 8.935    | 52.7     |
| json (stdlib) | users.json  | 78.667   | 84.810      | 87.477   | 52.8     |
| strata        | wide_arrays | 2.227    | 2.302       | 2.553    | 75.7     |
| msgspec       | wide_arrays | 2.348    | 2.581       | 2.691    | 75.8     |
| orjson        | wide_arrays | 2.445    | 2.657       | 2.818    | 75.7     |
| ujson         | wide_arrays | 9.499    | 9.728       | 10.918   | 75.7     |
| json (stdlib) | wide_arrays | 49.440   | 54.104      | 54.995   | 75.8     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 2.312    | 1000    | 52.8     |
|                                | orjson+jmespath    | 6.998    | 1000    | 52.9     |
|                                | orjson+jsonpath-ng | 8.436    | 1000    | 53.6     |
| all item prices                | strata             | 2.331    | 12046   | 53.6     |
|                                | orjson+jmespath    | 15.876   | 1000    | 53.7     |
|                                | orjson+jsonpath-ng | 41.829   | 12046   | 53.8     |
| recursive price                | strata             | 2.476    | 12046   | 53.4     |
|                                | orjson+jsonpath-ng | 143.072  | 12046   | 54.2     |
| flat:$.records\[\*\].id        | strata             | 0.599    | 500     | 62.2     |
|                                | orjson+jmespath    | 0.972    | 500     | 62.2     |
|                                | orjson+jsonpath-ng | 2.698    | 500     | 61.2     |
| flat:$.records\[0\]            | strata             | 0.536    | 1       | 61.2     |
|                                | orjson+jsonpath-ng | 0.737    | 1       | 61.2     |
|                                | orjson+jmespath    | 0.785    | 1       | 61.2     |
| nested:$.records\[\*\].id      | strata             | 0.509    | 500     | 61.2     |
|                                | orjson+jsonpath-ng | 2.781    | 500     | 61.2     |
|                                | orjson+jmespath    | 3.163    | 500     | 61.2     |
| nested:$.records\[0\]          | strata             | 0.504    | 1       | 61.2     |
|                                | orjson+jsonpath-ng | 1.817    | 1       | 61.2     |
|                                | orjson+jmespath    | 2.155    | 1       | 61.2     |
| wide_arrays:$.records\[\*\].id | strata             | 2.525    | 500     | 75.8     |
|                                | orjson+jmespath    | 5.042    | 500     | 75.8     |
|                                | orjson+jsonpath-ng | 5.486    | 500     | 75.8     |
| wide_arrays:$.records\[0\]     | strata             | 2.470    | 1       | 75.8     |
|                                | orjson+jsonpath-ng | 5.220    | 1       | 75.8     |
|                                | orjson+jmespath    | 5.774    | 1       | 75.8     |
| mixed:$.records\[\*\].id       | strata             | 0.503    | 500     | 75.8     |
|                                | orjson+jmespath    | 0.957    | 500     | 75.8     |
|                                | orjson+jsonpath-ng | 3.625    | 500     | 75.8     |
| mixed:$.records\[0\]           | strata             | 0.597    | 1       | 75.8     |
|                                | orjson+jsonpath-ng | 0.695    | 1       | 75.8     |
|                                | orjson+jmespath    | 0.761    | 1       | 75.8     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.132    | 1000    | 55.1     |
|                                            | jmespath    | 0.498    | 1000    | 55.1     |
|                                            | jsonpath-ng | 4.628    | 1000    | 55.1     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 1.017    | 12046   | 55.2     |
|                                            | jmespath    | 15.020   | 1000    | 55.2     |
|                                            | jsonpath-ng | 39.619   | 12046   | 57.8     |
| $..price                                   | strata      | 1.524    | 12046   | 55.8     |
|                                            | jsonpath-ng | 156.556  | 12046   | 58.8     |
| $.users\[?(@.age>30)\]                     | strata      | 0.121    | 794     | 56.2     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.767    | 1604    | 56.2     |
| flat:$.records\[\*\].id                    | strata      | 0.034    | 500     | 61.2     |
|                                            | jmespath    | 0.182    | 500     | 61.2     |
|                                            | jsonpath-ng | 3.177    | 500     | 61.2     |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 61.2     |
|                                            | jsonpath-ng | 0.008    | 1       | 61.2     |
|                                            | jmespath    | 0.015    | 1       | 61.2     |
| nested:$.records\[\*\].id                  | strata      | 0.039    | 500     | 61.3     |
|                                            | jmespath    | 0.210    | 500     | 61.3     |
|                                            | jsonpath-ng | 1.814    | 500     | 61.3     |
| nested:$.records\[0\]                      | strata      | 0.006    | 1       | 61.3     |
|                                            | jsonpath-ng | 0.007    | 1       | 61.3     |
|                                            | jmespath    | 0.011    | 1       | 61.3     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.052    | 500     | 75.8     |
|                                            | jmespath    | 0.200    | 500     | 75.8     |
|                                            | jsonpath-ng | 2.431    | 500     | 75.8     |
| wide_arrays:$.records\[0\]                 | strata      | 0.006    | 1       | 75.8     |
|                                            | jmespath    | 0.013    | 1       | 75.8     |
|                                            | jsonpath-ng | 0.017    | 1       | 75.8     |
| mixed:$.records\[\*\].id                   | strata      | 0.036    | 500     | 75.8     |
|                                            | jmespath    | 0.203    | 500     | 75.8     |
|                                            | jsonpath-ng | 2.085    | 500     | 75.8     |
| mixed:$.records\[0\]                       | strata      | 0.002    | 1       | 75.8     |
|                                            | jmespath    | 0.006    | 1       | 75.8     |
|                                            | jsonpath-ng | 0.007    | 1       | 75.8     |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **71.4% faster** than #2 (orjson)  |
| loads (NDJSON)      | **#1** / 5              | **13.0% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **42.5% faster** than #2 (msgspec) |
| load (NDJSON file)  | **#1** / 5              | **17.1% faster** than #2 (msgspec) |
| dumps               | **#1** / 5              | **39.9% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **15.2% faster** than #2 (msgspec) |
| loads (flat)        | **#2** / 5              | 3.8% behind #1 (orjson)            |
| load (flat)         | **#3** / 5              | 11.4% behind #1 (msgspec)          |
| dumps (flat)        | **#1** / 5              | **2.6% faster** than #2 (orjson)   |
| dump (flat)         | **#1** / 5              | **29.6% faster** than #2 (msgspec) |
| loads (nested)      | **#1** / 5              | **199.3% faster** than #2 (ujson)  |
| load (nested)       | **#1** / 5              | **140.8% faster** than #2 (orjson) |
| dumps (nested)      | **#3** / 5              | 6.4% behind #1 (orjson)            |
| dump (nested)       | **#1** / 5              | **2.9% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#1** / 5              | **57.4% faster** than #2 (orjson)  |
| load (wide_arrays)  | **#1** / 5              | **59.6% faster** than #2 (orjson)  |
| dumps (wide_arrays) | **#1** / 5              | **5.1% faster** than #2 (msgspec)  |
| dump (wide_arrays)  | **#1** / 5              | **5.4% faster** than #2 (msgspec)  |
| loads (mixed)       | **#3** / 5              | 49.2% behind #1 (orjson)           |
| load (mixed)        | **#3** / 5              | 38.1% behind #1 (orjson)           |
| dumps (mixed)       | **#1** / 5              | **6.1% faster** than #2 (msgspec)  |
| dump (mixed)        | **#3** / 5              | 15.1% behind #1 (orjson)           |
| search (JSONPath)   | **#1** in 11/11 queries | avg **690% faster**                |
| query (JSONPath)    | **#1** in 13/13 queries | -                                  |
