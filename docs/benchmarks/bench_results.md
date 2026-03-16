# Strata Benchmark Results

Generated: 2026-03-16 16:34:05

## Environment

- Commit: efd00fdc01e0951561d325921e36a0c1697c3ea1
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/small/users.json
- Random schemas: flat, nested, wide_arrays, mixed (500 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| pysimdjson    | flat        | ERROR    | -           | -        | -        |
| orjson        | flat        | 0.552    | 0.697       | 0.726    | 0.0      |
| strata        | flat        | 0.661    | 0.737       | 0.778    | 0.0      |
| msgspec       | flat        | 0.607    | 0.774       | 0.915    | 0.0      |
| ujson         | flat        | 1.096    | 1.219       | 1.252    | 0.0      |
| json (stdlib) | flat        | 2.088    | 2.108       | 2.133    | 0.0      |
| pysimdjson    | mixed       | ERROR    | -           | -        | -        |
| orjson        | mixed       | 0.475    | 0.613       | 0.627    | 0.0      |
| msgspec       | mixed       | 0.568    | 0.699       | 0.789    | 0.0      |
| strata        | mixed       | 0.672    | 0.792       | 0.814    | 0.0      |
| ujson         | mixed       | 0.956    | 1.136       | 1.138    | 0.0      |
| json (stdlib) | mixed       | 2.072    | 2.235       | 2.294    | 0.0      |
| pysimdjson    | nested      | ERROR    | -           | -        | -        |
| strata        | nested      | 0.540    | 0.669       | 0.683    | 0.0      |
| orjson        | nested      | 1.466    | 1.608       | 1.621    | 0.0      |
| msgspec       | nested      | 1.633    | 1.660       | 1.847    | 0.0      |
| ujson         | nested      | 1.709    | 1.944       | 2.429    | 0.0      |
| json (stdlib) | nested      | 2.373    | 2.430       | 2.500    | 0.0      |
| pysimdjson    | users.json  | ERROR    | -           | -        | -        |
| strata        | users.json  | 3.681    | 4.394       | 4.732    | 0.0      |
| msgspec       | users.json  | 4.468    | 6.014       | 6.230    | 0.0      |
| orjson        | users.json  | 4.557    | 6.098       | 6.577    | 0.0      |
| ujson         | users.json  | 5.884    | 8.410       | 8.941    | 0.0      |
| json (stdlib) | users.json  | 8.091    | 9.839       | 10.167   | 0.0      |
| pysimdjson    | wide_arrays | ERROR    | -           | -        | -        |
| strata        | wide_arrays | 3.040    | 3.468       | 3.743    | 0.0      |
| orjson        | wide_arrays | 3.339    | 3.903       | 3.986    | 0.0      |
| msgspec       | wide_arrays | 3.956    | 4.806       | 4.841    | 0.0      |
| ujson         | wide_arrays | 6.158    | 7.051       | 7.439    | 0.0      |
| json (stdlib) | wide_arrays | 12.903   | 13.821      | 13.952   | 0.0      |

## load (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.786    | 0.826       | 1.013    | 0.0      |
| orjson        | flat        | 0.677    | 0.852       | 0.871    | 0.0      |
| msgspec       | flat        | 0.798    | 0.856       | 0.877    | 0.0      |
| ujson         | flat        | 1.315    | 1.417       | 1.473    | 0.0      |
| json (stdlib) | flat        | 2.137    | 2.223       | 2.625    | 0.0      |
| orjson        | mixed       | 0.588    | 0.781       | 0.782    | 0.0      |
| strata        | mixed       | 0.842    | 0.882       | 1.002    | 0.0      |
| msgspec       | mixed       | 0.788    | 0.891       | 0.898    | 0.0      |
| ujson         | mixed       | 1.215    | 1.279       | 1.308    | 0.0      |
| json (stdlib) | mixed       | 2.121    | 2.254       | 2.283    | 0.0      |
| strata        | nested      | 0.613    | 0.811       | 0.814    | 0.0      |
| orjson        | nested      | 1.760    | 1.807       | 1.921    | 0.0      |
| msgspec       | nested      | 1.849    | 1.938       | 2.055    | 0.0      |
| ujson         | nested      | 1.820    | 2.463       | 2.497    | 0.0      |
| json (stdlib) | nested      | 2.350    | 2.633       | 3.015    | 0.0      |
| strata        | users.json  | 3.340    | 4.156       | 4.239    | 0.0      |
| orjson        | users.json  | 4.245    | 5.768       | 6.002    | 0.0      |
| msgspec       | users.json  | 4.477    | 6.224       | 6.437    | 0.0      |
| ujson         | users.json  | 5.847    | 7.869       | 8.199    | 0.0      |
| json (stdlib) | users.json  | 8.634    | 10.234      | 10.944   | 0.0      |
| strata        | wide_arrays | 3.150    | 3.775       | 3.828    | 0.0      |
| orjson        | wide_arrays | 3.567    | 4.430       | 4.443    | 0.0      |
| msgspec       | wide_arrays | 4.248    | 5.092       | 5.124    | 0.0      |
| ujson         | wide_arrays | 7.045    | 7.729       | 7.910    | 0.0      |
| json (stdlib) | wide_arrays | 13.357   | 14.066      | 14.545   | 0.0      |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.196    | 0.197       | 0.200    | 0.0      |
| msgspec | flat        | 0.304    | 0.321       | 0.332    | 0.0      |
| strata  | flat        | 0.383    | 0.386       | 0.479    | 0.0      |
| ujson   | flat        | 1.196    | 1.240       | 1.277    | 0.0      |
| json    | flat        | 2.291    | 2.291       | 2.379    | 0.0      |
| orjson  | mixed       | 0.278    | 0.290       | 0.300    | 0.0      |
| msgspec | mixed       | 0.309    | 0.317       | 0.351    | 0.0      |
| strata  | mixed       | 0.511    | 0.512       | 0.538    | 0.0      |
| ujson   | mixed       | 1.258    | 1.263       | 1.281    | 0.0      |
| json    | mixed       | 2.967    | 3.094       | 3.103    | 0.0      |
| orjson  | nested      | 0.168    | 0.168       | 0.175    | 0.0      |
| msgspec | nested      | 0.196    | 0.206       | 0.229    | 0.0      |
| strata  | nested      | 0.495    | 0.495       | 0.497    | 0.0      |
| ujson   | nested      | 0.949    | 0.949       | 0.954    | 0.0      |
| json    | nested      | 1.316    | 1.318       | 1.324    | 0.0      |
| orjson  | users.json  | 1.120    | 1.135       | 1.202    | 0.0      |
| msgspec | users.json  | 1.455    | 1.464       | 1.479    | 0.0      |
| strata  | users.json  | 2.793    | 2.818       | 2.852    | 0.0      |
| ujson   | users.json  | 6.118    | 6.128       | 6.263    | 0.0      |
| json    | users.json  | 8.952    | 8.991       | 9.068    | 0.0      |
| msgspec | wide_arrays | 1.606    | 1.666       | 1.667    | 0.0      |
| orjson  | wide_arrays | 1.671    | 1.694       | 1.830    | 0.0      |
| strata  | wide_arrays | 2.666    | 2.679       | 2.681    | 0.0      |
| ujson   | wide_arrays | 6.699    | 6.717       | 6.818    | 0.0      |
| json    | wide_arrays | 19.701   | 19.745      | 19.805   | 0.0      |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| msgspec       | flat        | 0.501    | 0.536       | 0.796    | 0.0      |
| orjson        | flat        | 0.511    | 0.541       | 0.713    | 0.0      |
| strata        | flat        | 0.529    | 0.552       | 0.584    | 0.0      |
| ujson         | flat        | 1.455    | 1.479       | 1.777    | 0.0      |
| json (stdlib) | flat        | 8.588    | 8.601       | 8.924    | 0.0      |
| orjson        | mixed       | 0.454    | 0.482       | 0.496    | 0.0      |
| msgspec       | mixed       | 0.493    | 0.541       | 0.695    | 0.0      |
| strata        | mixed       | 0.645    | 0.647       | 0.675    | 0.0      |
| ujson         | mixed       | 1.465    | 1.582       | 1.685    | 0.0      |
| json (stdlib) | mixed       | 10.032   | 10.190      | 10.411   | 0.0      |
| orjson        | nested      | 0.348    | 0.365       | 0.385    | 0.0      |
| msgspec       | nested      | 0.382    | 0.407       | 0.613    | 0.0      |
| strata        | nested      | 0.635    | 0.691       | 0.706    | 0.0      |
| ujson         | nested      | 1.157    | 1.214       | 1.313    | 0.0      |
| json (stdlib) | nested      | 11.918   | 12.065      | 12.190   | 0.0      |
| orjson        | users.json  | 1.375    | 1.578       | 1.581    | 0.0      |
| msgspec       | users.json  | 1.753    | 2.025       | 2.073    | 0.0      |
| strata        | users.json  | 3.085    | 3.141       | 3.495    | 0.0      |
| ujson         | users.json  | 6.589    | 6.615       | 7.146    | 0.0      |
| json (stdlib) | users.json  | 67.863   | 68.683      | 68.915   | 0.0      |
| msgspec       | wide_arrays | 1.964    | 1.998       | 2.643    | 0.0      |
| orjson        | wide_arrays | 2.326    | 2.520       | 2.783    | 0.0      |
| strata        | wide_arrays | 2.912    | 2.960       | 3.071    | 0.0      |
| ujson         | wide_arrays | 7.102    | 7.583       | 7.647    | 0.0      |
| json (stdlib) | wide_arrays | 45.343   | 45.360      | 45.672   | 0.0      |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.842    | 1000    | 0.0      |
|                                | orjson+jmespath    | 5.633    | 1000    | 0.0      |
|                                | orjson+jsonpath-ng | 6.676    | 1000    | 0.0      |
| all item prices                | strata             | 1.958    | 12046   | 0.0      |
|                                | orjson+jmespath    | 13.440   | 1000    | 0.0      |
|                                | orjson+jsonpath-ng | 39.238   | 12046   | 0.0      |
| recursive price                | strata             | 2.069    | 12046   | 0.0      |
|                                | orjson+jsonpath-ng | 135.783  | 12046   | 0.0      |
| flat:$.records\[\*\].id        | strata             | 0.519    | 500     | 0.0      |
|                                | orjson+jmespath    | 0.825    | 500     | 0.0      |
|                                | orjson+jsonpath-ng | 2.267    | 500     | 0.0      |
| flat:$.records\[0\]            | strata             | 0.480    | 1       | 0.0      |
|                                | orjson+jmespath    | 0.664    | 1       | 0.0      |
|                                | orjson+jsonpath-ng | 0.665    | 1       | 0.0      |
| nested:$.records\[\*\].id      | strata             | 0.413    | 500     | 0.0      |
|                                | orjson+jmespath    | 1.713    | 500     | 0.0      |
|                                | orjson+jsonpath-ng | 2.428    | 500     | 0.0      |
| nested:$.records\[0\]          | strata             | 0.395    | 1       | 0.0      |
|                                | orjson+jmespath    | 1.492    | 1       | 0.0      |
|                                | orjson+jsonpath-ng | 1.503    | 1       | 0.0      |
| wide_arrays:$.records\[\*\].id | strata             | 2.747    | 500     | 0.0      |
|                                | orjson+jmespath    | 4.112    | 500     | 0.0      |
|                                | orjson+jsonpath-ng | 4.692    | 500     | 0.0      |
| wide_arrays:$.records\[0\]     | strata             | 2.756    | 1       | 0.0      |
|                                | orjson+jmespath    | 3.811    | 1       | 0.0      |
|                                | orjson+jsonpath-ng | 3.944    | 1       | 0.0      |
| mixed:$.records\[\*\].id       | strata             | 0.513    | 500     | 0.0      |
|                                | orjson+jmespath    | 0.782    | 500     | 0.0      |
|                                | orjson+jsonpath-ng | 2.239    | 500     | 0.0      |
| mixed:$.records\[0\]           | strata             | 0.520    | 1       | 0.0      |
|                                | orjson+jmespath    | 0.603    | 1       | 0.0      |
|                                | orjson+jsonpath-ng | 0.611    | 1       | 0.0      |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.055    | 1000    | 0.0      |
|                                            | jmespath    | 0.355    | 1000    | 0.0      |
|                                            | jsonpath-ng | 3.869    | 1000    | 0.0      |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.912    | 12046   | 0.0      |
|                                            | jmespath    | 10.338   | 1000    | 0.0      |
|                                            | jsonpath-ng | 34.065   | 12046   | 0.0      |
| $..price                                   | strata      | 1.267    | 12046   | 0.0      |
|                                            | jsonpath-ng | 131.120  | 12046   | 0.0      |
| $.users\[?(@.age>30)\]                     | strata      | 0.118    | 794     | 0.0      |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.467    | 1604    | 0.0      |
| flat:$.records\[\*\].id                    | strata      | 0.032    | 500     | 0.0      |
|                                            | jmespath    | 0.170    | 500     | 0.0      |
|                                            | jsonpath-ng | 1.548    | 500     | 0.0      |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 0.0      |
|                                            | jmespath    | 0.005    | 1       | 0.0      |
|                                            | jsonpath-ng | 0.007    | 1       | 0.0      |
| nested:$.records\[\*\].id                  | strata      | 0.026    | 500     | 0.0      |
|                                            | jmespath    | 0.181    | 500     | 0.0      |
|                                            | jsonpath-ng | 1.708    | 500     | 0.0      |
| nested:$.records\[0\]                      | strata      | 0.002    | 1       | 0.0      |
|                                            | jmespath    | 0.005    | 1       | 0.0      |
|                                            | jsonpath-ng | 0.007    | 1       | 0.0      |
| wide_arrays:$.records\[\*\].id             | strata      | 0.037    | 500     | 0.0      |
|                                            | jmespath    | 0.174    | 500     | 0.0      |
|                                            | jsonpath-ng | 2.248    | 500     | 0.0      |
| wide_arrays:$.records\[0\]                 | strata      | 0.003    | 1       | 0.0      |
|                                            | jmespath    | 0.007    | 1       | 0.0      |
|                                            | jsonpath-ng | 0.009    | 1       | 0.0      |
| mixed:$.records\[\*\].id                   | strata      | 0.029    | 500     | 0.0      |
|                                            | jmespath    | 0.167    | 500     | 0.0      |
|                                            | jsonpath-ng | 1.617    | 500     | 0.0      |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 0.0      |
|                                            | jmespath    | 0.005    | 1       | 0.0      |
|                                            | jsonpath-ng | 0.007    | 1       | 0.0      |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **21.4% faster** than #2 (msgspec) |
| load (JSON file)    | **#1** / 5              | **27.1% faster** than #2 (orjson)  |
| dumps               | **#3** / 5              | 149.4% behind #1 (orjson)          |
| dump                | **#3** / 5              | 124.3% behind #1 (orjson)          |
| loads (flat)        | **#3** / 5              | 19.7% behind #1 (orjson)           |
| load (flat)         | **#2** / 5              | 16.2% behind #1 (orjson)           |
| dumps (flat)        | **#3** / 5              | 95.8% behind #1 (orjson)           |
| dump (flat)         | **#3** / 5              | 5.7% behind #1 (msgspec)           |
| loads (nested)      | **#1** / 5              | **171.4% faster** than #2 (orjson) |
| load (nested)       | **#1** / 5              | **187.1% faster** than #2 (orjson) |
| dumps (nested)      | **#3** / 5              | 193.9% behind #1 (orjson)          |
| dump (nested)       | **#3** / 5              | 82.6% behind #1 (orjson)           |
| loads (wide_arrays) | **#1** / 5              | **9.8% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **13.2% faster** than #2 (orjson)  |
| dumps (wide_arrays) | **#3** / 5              | 66.0% behind #1 (msgspec)          |
| dump (wide_arrays)  | **#3** / 5              | 48.3% behind #1 (msgspec)          |
| loads (mixed)       | **#3** / 5              | 41.4% behind #1 (orjson)           |
| load (mixed)        | **#3** / 5              | 43.1% behind #1 (orjson)           |
| dumps (mixed)       | **#3** / 5              | 83.7% behind #1 (orjson)           |
| dump (mixed)        | **#3** / 5              | 42.0% behind #1 (orjson)           |
| search (JSONPath)   | **#1** in 11/11 queries | avg **737% faster**                |
| query (JSONPath)    | **#1** in 13/13 queries | -                                  |
