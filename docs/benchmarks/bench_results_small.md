# Strata Benchmark Results

Generated: 2026-03-18 13:21:10

## Environment

- Commit: 61b2175f7dd9503191430e1dff7b62dbe525ea97
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
| orjson        | flat         | 0.568    | 0.694       | 0.728    | 61.5     |
| msgspec       | flat         | 0.627    | 0.727       | 0.926    | 61.5     |
| strata        | flat         | 0.723    | 0.836       | 0.845    | 60.8     |
| ujson         | flat         | 1.049    | 1.261       | 1.508    | 61.5     |
| json (stdlib) | flat         | 2.017    | 2.109       | 2.131    | 61.5     |
| orjson        | mixed        | 0.606    | 0.674       | 0.675    | 76.1     |
| msgspec       | mixed        | 0.596    | 0.708       | 0.737    | 76.1     |
| strata        | mixed        | 0.733    | 0.872       | 0.880    | 76.1     |
| ujson         | mixed        | 0.992    | 1.178       | 1.235    | 76.1     |
| json (stdlib) | mixed        | 2.046    | 2.228       | 2.282    | 76.1     |
| strata        | nested       | 0.713    | 0.769       | 0.911    | 60.6     |
| msgspec       | nested       | 1.653    | 1.755       | 1.856    | 60.6     |
| orjson        | nested       | 1.672    | 1.934       | 2.016    | 60.6     |
| ujson         | nested       | 2.367    | 2.434       | 2.458    | 60.6     |
| json (stdlib) | nested       | 2.310    | 2.534       | 2.611    | 60.6     |
| strata        | users.json   | 3.801    | 4.599       | 4.838    | 34.3     |
| orjson        | users.json   | 4.413    | 5.757       | 6.187    | 38.3     |
| msgspec       | users.json   | 4.720    | 6.170       | 6.235    | 41.5     |
| ujson         | users.json   | 5.770    | 8.142       | 8.953    | 41.3     |
| json (stdlib) | users.json   | 8.659    | 10.366      | 10.372   | 43.9     |
| strata        | users.ndjson | 4.259    | 4.795       | 5.108    | 56.2     |
| orjson        | users.ndjson | 4.382    | 6.103       | 6.499    | 57.2     |
| msgspec       | users.ndjson | 4.760    | 6.216       | 6.632    | 59.2     |
| ujson         | users.ndjson | 6.398    | 8.396       | 8.508    | 59.2     |
| json (stdlib) | users.ndjson | 9.169    | 11.177      | 11.192   | 59.2     |
| strata        | wide_arrays  | 3.187    | 3.792       | 3.792    | 64.0     |
| orjson        | wide_arrays  | 3.680    | 4.269       | 4.286    | 66.6     |
| msgspec       | wide_arrays  | 4.186    | 5.011       | 5.050    | 66.7     |
| ujson         | wide_arrays  | 6.596    | 7.495       | 7.587    | 66.7     |
| json (stdlib) | wide_arrays  | 13.439   | 14.198      | 14.369   | 68.0     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.714    | 0.774       | 0.853    | 61.5     |
| msgspec       | flat         | 0.760    | 0.799       | 0.800    | 61.5     |
| strata        | flat         | 0.780    | 0.888       | 0.895    | 61.5     |
| ujson         | flat         | 1.317    | 1.409       | 1.538    | 61.5     |
| json (stdlib) | flat         | 2.228    | 2.271       | 2.340    | 61.5     |
| orjson        | mixed        | 0.716    | 0.836       | 1.021    | 76.1     |
| strata        | mixed        | 1.025    | 1.026       | 1.080    | 76.1     |
| msgspec       | mixed        | 0.701    | 1.101       | 1.123    | 76.1     |
| ujson         | mixed        | 1.240    | 1.397       | 1.812    | 76.1     |
| json (stdlib) | mixed        | 2.392    | 2.422       | 2.915    | 76.1     |
| strata        | nested       | 0.685    | 0.781       | 0.781    | 60.6     |
| orjson        | nested       | 1.592    | 1.813       | 1.837    | 60.6     |
| msgspec       | nested       | 1.794    | 1.881       | 1.890    | 60.6     |
| ujson         | nested       | 1.892    | 2.251       | 2.661    | 60.6     |
| json (stdlib) | nested       | 2.678    | 2.788       | 2.851    | 60.6     |
| strata        | users.json   | 3.703    | 4.525       | 4.733    | 43.9     |
| orjson        | users.json   | 4.614    | 6.280       | 6.328    | 44.9     |
| msgspec       | users.json   | 5.082    | 6.372       | 7.173    | 48.1     |
| ujson         | users.json   | 6.124    | 8.708       | 9.055    | 48.1     |
| json (stdlib) | users.json   | 9.465    | 10.592      | 10.598   | 48.1     |
| strata        | users.ndjson | 4.150    | 4.986       | 5.163    | 59.2     |
| orjson        | users.ndjson | 4.700    | 6.047       | 6.722    | 59.2     |
| msgspec       | users.ndjson | 4.652    | 6.218       | 6.617    | 60.2     |
| ujson         | users.ndjson | 6.612    | 9.002       | 9.015    | 60.2     |
| json (stdlib) | users.ndjson | 9.162    | 11.054      | 11.277   | 60.2     |
| strata        | wide_arrays  | 3.632    | 4.030       | 4.084    | 68.0     |
| orjson        | wide_arrays  | 4.477    | 4.622       | 8.051    | 68.0     |
| msgspec       | wide_arrays  | 5.090    | 5.522       | 5.612    | 69.2     |
| ujson         | wide_arrays  | 7.998    | 8.239       | 13.586   | 69.2     |
| json (stdlib) | wide_arrays  | 14.221   | 14.730      | 14.806   | 71.8     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.236    | 0.237       | 0.238    | 61.5     |
| orjson  | flat        | 0.251    | 0.259       | 0.263    | 61.5     |
| msgspec | flat        | 0.304    | 0.304       | 0.310    | 61.5     |
| ujson   | flat        | 1.219    | 1.249       | 1.263    | 61.6     |
| json    | flat        | 2.276    | 2.305       | 2.312    | 61.6     |
| strata  | mixed       | 0.278    | 0.281       | 0.287    | 76.1     |
| orjson  | mixed       | 0.283    | 0.295       | 0.299    | 76.1     |
| msgspec | mixed       | 0.322    | 0.344       | 0.375    | 76.1     |
| ujson   | mixed       | 1.367    | 1.371       | 1.698    | 76.1     |
| json    | mixed       | 3.007    | 3.335       | 3.714    | 76.1     |
| strata  | nested      | 0.181    | 0.182       | 0.190    | 60.6     |
| orjson  | nested      | 0.198    | 0.201       | 0.252    | 60.6     |
| msgspec | nested      | 0.199    | 0.203       | 0.205    | 60.6     |
| ujson   | nested      | 1.009    | 1.022       | 1.139    | 60.6     |
| json    | nested      | 1.330    | 1.338       | 1.467    | 60.6     |
| strata  | users.json  | 1.068    | 1.074       | 1.150    | 48.8     |
| msgspec | users.json  | 1.453    | 1.463       | 1.510    | 50.0     |
| orjson  | users.json  | 1.504    | 1.504       | 1.513    | 49.7     |
| ujson   | users.json  | 6.239    | 6.319       | 6.508    | 51.4     |
| json    | users.json  | 8.893    | 8.923       | 8.987    | 52.9     |
| strata  | wide_arrays | 1.379    | 1.382       | 1.382    | 73.1     |
| orjson  | wide_arrays | 1.540    | 1.547       | 1.695    | 75.0     |
| msgspec | wide_arrays | 1.623    | 1.634       | 1.646    | 75.0     |
| ujson   | wide_arrays | 7.078    | 7.120       | 7.141    | 76.1     |
| json    | wide_arrays | 19.872   | 19.943      | 20.021   | 76.1     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.417    | 0.427       | 0.467    | 61.6     |
| msgspec       | flat        | 0.507    | 0.545       | 0.551    | 61.6     |
| orjson        | flat        | 0.465    | 0.549       | 0.560    | 61.6     |
| ujson         | flat        | 1.418    | 1.506       | 1.517    | 61.6     |
| json (stdlib) | flat        | 8.338    | 8.401       | 8.848    | 61.6     |
| strata        | mixed       | 0.423    | 0.424       | 0.455    | 76.1     |
| orjson        | mixed       | 0.487    | 0.588       | 0.634    | 76.1     |
| msgspec       | mixed       | 0.597    | 0.730       | 0.874    | 76.1     |
| ujson         | mixed       | 1.492    | 1.529       | 1.567    | 76.1     |
| json (stdlib) | mixed       | 10.416   | 10.447      | 11.787   | 76.1     |
| strata        | nested      | 0.300    | 0.313       | 0.327    | 60.6     |
| msgspec       | nested      | 0.372    | 0.376       | 0.403    | 60.6     |
| orjson        | nested      | 0.422    | 0.485       | 0.514    | 60.6     |
| ujson         | nested      | 1.220    | 1.281       | 1.549    | 60.6     |
| json (stdlib) | nested      | 12.231   | 12.527      | 13.125   | 60.6     |
| strata        | users.json  | 1.280    | 1.282       | 1.313    | 52.9     |
| msgspec       | users.json  | 1.742    | 1.783       | 1.817    | 52.9     |
| orjson        | users.json  | 1.737    | 1.785       | 1.792    | 52.9     |
| ujson         | users.json  | 6.655    | 6.867       | 6.890    | 52.9     |
| json (stdlib) | users.json  | 66.055   | 66.617      | 66.720   | 52.9     |
| strata        | wide_arrays | 1.620    | 1.624       | 1.710    | 76.1     |
| orjson        | wide_arrays | 1.933    | 2.023       | 2.052    | 76.1     |
| msgspec       | wide_arrays | 2.003    | 2.033       | 2.069    | 76.1     |
| ujson         | wide_arrays | 7.439    | 7.444       | 7.701    | 76.1     |
| json (stdlib) | wide_arrays | 44.083   | 44.788      | 45.107   | 76.1     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.899    | 1000    | 53.0     |
|                                | orjson+jmespath    | 5.349    | 1000    | 53.1     |
|                                | orjson+jsonpath-ng | 6.561    | 1000    | 53.6     |
| all item prices                | strata             | 2.152    | 12046   | 53.6     |
|                                | orjson+jmespath    | 13.298   | 1000    | 53.7     |
|                                | orjson+jsonpath-ng | 35.150   | 12046   | 53.6     |
| recursive price                | strata             | 2.126    | 12046   | 53.3     |
|                                | orjson+jsonpath-ng | 125.288  | 12046   | 54.1     |
| flat:$.records\[\*\].id        | strata             | 0.493    | 500     | 61.6     |
|                                | orjson+jmespath    | 0.952    | 500     | 61.6     |
|                                | orjson+jsonpath-ng | 2.321    | 500     | 60.6     |
| flat:$.records\[0\]            | strata             | 0.516    | 1       | 60.6     |
|                                | orjson+jmespath    | 0.672    | 1       | 60.6     |
|                                | orjson+jsonpath-ng | 0.689    | 1       | 60.6     |
| nested:$.records\[\*\].id      | strata             | 0.411    | 500     | 60.6     |
|                                | orjson+jmespath    | 1.934    | 500     | 60.6     |
|                                | orjson+jsonpath-ng | 2.333    | 500     | 60.6     |
| nested:$.records\[0\]          | strata             | 0.377    | 1       | 60.7     |
|                                | orjson+jmespath    | 1.601    | 1       | 60.7     |
|                                | orjson+jsonpath-ng | 1.672    | 1       | 60.7     |
| wide_arrays:$.records\[\*\].id | strata             | 2.690    | 500     | 76.1     |
|                                | orjson+jmespath    | 4.306    | 500     | 76.1     |
|                                | orjson+jsonpath-ng | 4.793    | 500     | 76.1     |
| wide_arrays:$.records\[0\]     | strata             | 2.671    | 1       | 76.1     |
|                                | orjson+jmespath    | 3.972    | 1       | 76.1     |
|                                | orjson+jsonpath-ng | 4.208    | 1       | 76.1     |
| mixed:$.records\[\*\].id       | strata             | 0.589    | 500     | 76.1     |
|                                | orjson+jmespath    | 0.878    | 500     | 76.1     |
|                                | orjson+jsonpath-ng | 2.602    | 500     | 76.1     |
| mixed:$.records\[0\]           | strata             | 0.573    | 1       | 76.1     |
|                                | orjson+jmespath    | 0.611    | 1       | 76.1     |
|                                | orjson+jsonpath-ng | 0.612    | 1       | 76.1     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.069    | 1000    | 55.1     |
|                                            | jmespath    | 0.393    | 1000    | 55.1     |
|                                            | jsonpath-ng | 4.045    | 1000    | 55.1     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.896    | 12046   | 55.1     |
|                                            | jmespath    | 10.399   | 1000    | 55.1     |
|                                            | jsonpath-ng | 31.724   | 12046   | 57.8     |
| $..price                                   | strata      | 1.285    | 12046   | 55.8     |
|                                            | jsonpath-ng | 122.615  | 12046   | 58.8     |
| $.users\[?(@.age>30)\]                     | strata      | 0.103    | 794     | 56.2     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.440    | 1604    | 56.2     |
| flat:$.records\[\*\].id                    | strata      | 0.035    | 500     | 60.6     |
|                                            | jmespath    | 0.171    | 500     | 60.6     |
|                                            | jsonpath-ng | 1.599    | 500     | 60.6     |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 60.6     |
|                                            | jmespath    | 0.006    | 1       | 60.6     |
|                                            | jsonpath-ng | 0.014    | 1       | 60.6     |
| nested:$.records\[\*\].id                  | strata      | 0.026    | 500     | 60.7     |
|                                            | jmespath    | 0.185    | 500     | 60.7     |
|                                            | jsonpath-ng | 1.732    | 500     | 60.7     |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 60.7     |
|                                            | jmespath    | 0.005    | 1       | 60.7     |
|                                            | jsonpath-ng | 0.016    | 1       | 60.7     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.030    | 500     | 76.1     |
|                                            | jmespath    | 0.183    | 500     | 76.1     |
|                                            | jsonpath-ng | 2.376    | 500     | 76.1     |
| wide_arrays:$.records\[0\]                 | jmespath    | 0.007    | 1       | 76.1     |
|                                            | strata      | 0.009    | 1       | 76.1     |
|                                            | jsonpath-ng | 0.013    | 1       | 76.1     |
| mixed:$.records\[\*\].id                   | strata      | 0.029    | 500     | 76.1     |
|                                            | jmespath    | 0.178    | 500     | 76.1     |
|                                            | jsonpath-ng | 1.781    | 500     | 76.1     |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 76.1     |
|                                            | jsonpath-ng | 0.006    | 1       | 76.1     |
|                                            | jmespath    | 0.006    | 1       | 76.1     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **16.1% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **2.9% faster** than #2 (orjson)    |
| load (JSON file)    | **#1** / 5              | **24.6% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **12.1% faster** than #2 (msgspec)  |
| dumps               | **#1** / 5              | **36.1% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **35.7% faster** than #2 (orjson)   |
| loads (flat)        | **#3** / 5              | 27.3% behind #1 (orjson)            |
| load (flat)         | **#3** / 5              | 9.2% behind #1 (orjson)             |
| dumps (flat)        | **#1** / 5              | **6.3% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **11.5% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **131.8% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **132.5% faster** than #2 (orjson)  |
| dumps (nested)      | **#1** / 5              | **9.0% faster** than #2 (orjson)    |
| dump (nested)       | **#1** / 5              | **24.2% faster** than #2 (msgspec)  |
| loads (wide_arrays) | **#1** / 5              | **15.4% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **23.3% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **11.7% faster** than #2 (orjson)   |
| dump (wide_arrays)  | **#1** / 5              | **19.3% faster** than #2 (orjson)   |
| loads (mixed)       | **#3** / 5              | 23.0% behind #1 (msgspec)           |
| load (mixed)        | **#3** / 5              | 46.3% behind #1 (msgspec)           |
| dumps (mixed)       | **#1** / 5              | **1.7% faster** than #2 (orjson)    |
| dump (mixed)        | **#1** / 5              | **15.0% faster** than #2 (orjson)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **680% faster**                 |
| query (JSONPath)    | **#1** in 12/13 queries | -                                   |
