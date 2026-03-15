# Strata Benchmark Results

Generated: 2026-03-15 18:34:27

## Environment

- Commit: 67ef7fc31abef4549164a63037b9929eceed01a5
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
| orjson        | flat         | 0.556    | 0.717       | 0.740    | 61.5     |
| msgspec       | flat         | 0.639    | 0.722       | 0.828    | 61.5     |
| ujson         | flat         | 1.127    | 1.194       | 1.289    | 61.5     |
| strata        | flat         | 1.345    | 1.515       | 1.549    | 60.3     |
| json (stdlib) | flat         | 1.954    | 2.155       | 2.225    | 61.5     |
| orjson        | mixed        | 0.576    | 0.607       | 0.621    | 77.3     |
| msgspec       | mixed        | 0.567    | 0.765       | 0.837    | 77.3     |
| ujson         | mixed        | 0.974    | 1.179       | 1.183    | 77.3     |
| strata        | mixed        | 1.542    | 1.660       | 1.742    | 77.3     |
| json (stdlib) | mixed        | 2.193    | 2.195       | 2.289    | 77.3     |
| orjson        | nested       | 1.493    | 1.686       | 2.028    | 61.5     |
| msgspec       | nested       | 1.622    | 1.861       | 2.039    | 61.5     |
| ujson         | nested       | 1.753    | 2.178       | 2.193    | 61.5     |
| strata        | nested       | 2.112    | 2.263       | 2.328    | 61.5     |
| json (stdlib) | nested       | 2.354    | 2.518       | 2.894    | 61.5     |
| msgspec       | users.json   | 4.646    | 6.401       | 6.728    | 41.1     |
| orjson        | users.json   | 4.430    | 6.459       | 6.533    | 38.0     |
| ujson         | users.json   | 6.273    | 8.401       | 8.892    | 41.0     |
| json (stdlib) | users.json   | 8.840    | 10.225      | 10.492   | 44.0     |
| strata        | users.json   | 11.289   | 11.591      | 12.129   | 34.3     |
| orjson        | users.ndjson | 4.248    | 5.901       | 5.929    | 55.0     |
| msgspec       | users.ndjson | 4.381    | 5.970       | 6.209    | 57.0     |
| ujson         | users.ndjson | 5.958    | 7.959       | 8.294    | 57.0     |
| json (stdlib) | users.ndjson | 9.054    | 10.860      | 10.902   | 57.0     |
| strata        | users.ndjson | 10.742   | 11.669      | 11.949   | 54.0     |
| orjson        | wide_arrays  | 3.650    | 3.956       | 4.340    | 66.6     |
| msgspec       | wide_arrays  | 4.344    | 4.866       | 4.939    | 66.6     |
| ujson         | wide_arrays  | 7.010    | 7.170       | 7.612    | 66.6     |
| strata        | wide_arrays  | 6.763    | 7.558       | 7.572    | 64.0     |
| json (stdlib) | wide_arrays  | 12.792   | 13.402      | 13.686   | 67.9     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.715    | 0.840       | 0.858    | 61.5     |
| msgspec       | flat         | 0.870    | 0.905       | 0.918    | 61.5     |
| ujson         | flat         | 1.251    | 1.477       | 1.561    | 61.5     |
| strata        | flat         | 1.490    | 1.559       | 1.570    | 61.5     |
| json (stdlib) | flat         | 2.165    | 2.265       | 2.277    | 61.5     |
| orjson        | mixed        | 0.556    | 0.720       | 0.907    | 77.3     |
| msgspec       | mixed        | 0.675    | 0.811       | 0.979    | 77.3     |
| ujson         | mixed        | 1.178    | 1.346       | 1.519    | 77.3     |
| strata        | mixed        | 1.639    | 1.696       | 1.905    | 77.3     |
| json (stdlib) | mixed        | 2.153    | 2.348       | 2.350    | 77.3     |
| orjson        | nested       | 1.650    | 1.939       | 2.215    | 61.5     |
| msgspec       | nested       | 1.611    | 1.956       | 2.020    | 61.5     |
| ujson         | nested       | 2.036    | 2.143       | 2.522    | 61.5     |
| strata        | nested       | 2.226    | 2.334       | 2.405    | 61.5     |
| json (stdlib) | nested       | 2.565    | 2.700       | 2.956    | 61.5     |
| orjson        | users.json   | 4.302    | 6.375       | 6.987    | 45.0     |
| msgspec       | users.json   | 4.841    | 6.403       | 6.449    | 47.1     |
| ujson         | users.json   | 6.262    | 8.217       | 8.759    | 47.1     |
| json (stdlib) | users.json   | 9.040    | 10.335      | 10.699   | 47.1     |
| strata        | users.json   | 11.103   | 11.670      | 12.000   | 44.0     |
| orjson        | users.ndjson | 4.524    | 5.809       | 6.198    | 57.0     |
| msgspec       | users.ndjson | 4.637    | 6.065       | 6.153    | 59.1     |
| ujson         | users.ndjson | 6.634    | 8.513       | 8.863    | 59.1     |
| json (stdlib) | users.ndjson | 9.252    | 10.836      | 11.557   | 59.1     |
| strata        | users.ndjson | 11.044   | 11.858      | 12.009   | 57.0     |
| orjson        | wide_arrays  | 3.457    | 4.319       | 4.552    | 67.9     |
| msgspec       | wide_arrays  | 4.725    | 5.300       | 5.514    | 69.2     |
| ujson         | wide_arrays  | 7.197    | 7.454       | 7.685    | 69.2     |
| strata        | wide_arrays  | 7.431    | 7.726       | 7.862    | 67.9     |
| json (stdlib) | wide_arrays  | 13.347   | 14.072      | 14.169   | 69.2     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.235    | 0.236       | 0.241    | 61.5     |
| msgspec | flat        | 0.288    | 0.291       | 0.309    | 61.5     |
| strata  | flat        | 0.295    | 0.316       | 0.318    | 61.5     |
| ujson   | flat        | 1.156    | 1.174       | 1.229    | 61.5     |
| json    | flat        | 2.235    | 2.264       | 2.330    | 61.5     |
| orjson  | mixed       | 0.281    | 0.284       | 0.322    | 77.3     |
| strata  | mixed       | 0.323    | 0.324       | 0.331    | 77.3     |
| msgspec | mixed       | 0.300    | 0.326       | 0.342    | 77.3     |
| ujson   | mixed       | 1.249    | 1.278       | 1.412    | 77.3     |
| json    | mixed       | 2.861    | 2.944       | 3.082    | 77.3     |
| strata  | nested      | 0.161    | 0.170       | 0.175    | 61.5     |
| orjson  | nested      | 0.182    | 0.196       | 0.207    | 61.5     |
| msgspec | nested      | 0.200    | 0.214       | 0.235    | 61.5     |
| ujson   | nested      | 0.966    | 0.985       | 0.999    | 61.5     |
| json    | nested      | 1.315    | 1.336       | 1.346    | 61.5     |
| msgspec | users.json  | 1.420    | 1.443       | 1.575    | 51.6     |
| orjson  | users.json  | 1.413    | 1.444       | 1.626    | 49.3     |
| strata  | users.json  | 1.470    | 1.505       | 1.607    | 47.1     |
| ujson   | users.json  | 6.241    | 6.243       | 6.252    | 51.6     |
| json    | users.json  | 8.326    | 8.598       | 8.958    | 51.9     |
| orjson  | wide_arrays | 1.459    | 1.525       | 1.688    | 72.3     |
| msgspec | wide_arrays | 1.585    | 1.595       | 1.687    | 72.7     |
| strata  | wide_arrays | 1.749    | 1.764       | 1.831    | 70.5     |
| ujson   | wide_arrays | 6.944    | 7.075       | 7.210    | 75.2     |
| json    | wide_arrays | 19.114   | 19.397      | 19.503   | 77.3     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.426    | 0.450       | 0.579    | 61.5     |
| orjson        | flat        | 0.434    | 0.454       | 0.482    | 61.5     |
| msgspec       | flat        | 0.541    | 0.571       | 0.586    | 61.5     |
| ujson         | flat        | 1.435    | 1.468       | 1.469    | 61.5     |
| json (stdlib) | flat        | 8.137    | 8.251       | 8.477    | 61.5     |
| strata        | mixed       | 0.457    | 0.477       | 0.547    | 77.3     |
| orjson        | mixed       | 0.496    | 0.517       | 0.519    | 77.3     |
| msgspec       | mixed       | 0.489    | 0.540       | 0.583    | 77.3     |
| ujson         | mixed       | 1.473    | 1.586       | 1.644    | 77.3     |
| json (stdlib) | mixed       | 9.791    | 9.929       | 10.003   | 77.3     |
| strata        | nested      | 0.301    | 0.354       | 0.356    | 61.5     |
| msgspec       | nested      | 0.376    | 0.414       | 0.558    | 61.5     |
| orjson        | nested      | 0.374    | 0.414       | 0.445    | 61.5     |
| ujson         | nested      | 1.206    | 1.207       | 1.253    | 61.5     |
| json (stdlib) | nested      | 11.566   | 11.718      | 11.840   | 61.5     |
| orjson        | users.json  | 1.688    | 1.754       | 1.870    | 51.9     |
| msgspec       | users.json  | 1.657    | 1.789       | 1.811    | 51.9     |
| strata        | users.json  | 1.822    | 1.887       | 1.893    | 51.9     |
| ujson         | users.json  | 6.318    | 6.445       | 6.504    | 51.9     |
| json (stdlib) | users.json  | 62.934   | 63.051      | 64.941   | 51.9     |
| orjson        | wide_arrays | 1.915    | 1.930       | 2.018    | 77.3     |
| strata        | wide_arrays | 2.024    | 2.104       | 2.158    | 77.3     |
| msgspec       | wide_arrays | 2.112    | 2.145       | 2.364    | 77.3     |
| ujson         | wide_arrays | 7.356    | 7.367       | 7.416    | 77.3     |
| json (stdlib) | wide_arrays | 42.729   | 43.417      | 43.487   | 77.3     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.428    | 1000    | 52.0     |
|                                | orjson+jmespath    | 5.244    | 1000    | 52.0     |
|                                | orjson+jsonpath-ng | 6.423    | 1000    | 52.5     |
| all item prices                | strata             | 1.763    | 12046   | 52.5     |
|                                | orjson+jmespath    | 13.125   | 1000    | 52.5     |
|                                | orjson+jsonpath-ng | 34.680   | 12046   | 52.4     |
| recursive price                | strata             | 1.717    | 12046   | 52.1     |
|                                | orjson+jsonpath-ng | 126.628  | 12046   | 52.9     |
| flat:$.records\[\*\].id        | strata             | 0.384    | 500     | 61.5     |
|                                | orjson+jmespath    | 0.794    | 500     | 61.5     |
|                                | orjson+jsonpath-ng | 2.154    | 500     | 61.5     |
| flat:$.records\[0\]            | strata             | 0.380    | 1       | 61.5     |
|                                | orjson+jmespath    | 0.629    | 1       | 61.5     |
|                                | orjson+jsonpath-ng | 0.636    | 1       | 61.5     |
| nested:$.records\[\*\].id      | strata             | 0.317    | 500     | 61.5     |
|                                | orjson+jmespath    | 1.685    | 500     | 61.5     |
|                                | orjson+jsonpath-ng | 2.400    | 500     | 61.5     |
| nested:$.records\[0\]          | strata             | 0.296    | 1       | 61.5     |
|                                | orjson+jmespath    | 1.531    | 1       | 61.5     |
|                                | orjson+jsonpath-ng | 1.789    | 1       | 61.5     |
| wide_arrays:$.records\[\*\].id | strata             | 1.817    | 500     | 77.3     |
|                                | orjson+jmespath    | 3.900    | 500     | 77.3     |
|                                | orjson+jsonpath-ng | 4.576    | 500     | 77.3     |
| wide_arrays:$.records\[0\]     | strata             | 1.869    | 1       | 77.3     |
|                                | orjson+jsonpath-ng | 3.944    | 1       | 77.3     |
|                                | orjson+jmespath    | 3.968    | 1       | 77.3     |
| mixed:$.records\[\*\].id       | strata             | 0.371    | 500     | 77.3     |
|                                | orjson+jmespath    | 0.770    | 500     | 77.3     |
|                                | orjson+jsonpath-ng | 2.422    | 500     | 77.3     |
| mixed:$.records\[0\]           | strata             | 0.396    | 1       | 77.3     |
|                                | orjson+jsonpath-ng | 0.599    | 1       | 77.3     |
|                                | orjson+jmespath    | 0.608    | 1       | 77.3     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.066    | 1000    | 52.9     |
|                                            | jmespath    | 0.418    | 1000    | 52.9     |
|                                            | jsonpath-ng | 4.198    | 1000    | 52.9     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.897    | 12046   | 52.9     |
|                                            | jmespath    | 10.453   | 1000    | 52.9     |
|                                            | jsonpath-ng | 29.962   | 12046   | 55.8     |
| $..price                                   | strata      | 1.208    | 12046   | 55.8     |
|                                            | jsonpath-ng | 116.709  | 12046   | 56.8     |
| $.users\[?(@.age>30)\]                     | strata      | 0.082    | 794     | 54.0     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.369    | 1604    | 54.0     |
| flat:$.records\[\*\].id                    | strata      | 0.035    | 500     | 61.5     |
|                                            | jmespath    | 0.161    | 500     | 61.5     |
|                                            | jsonpath-ng | 1.766    | 500     | 61.5     |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 61.5     |
|                                            | jmespath    | 0.005    | 1       | 61.5     |
|                                            | jsonpath-ng | 0.007    | 1       | 61.5     |
| nested:$.records\[\*\].id                  | strata      | 0.024    | 500     | 61.5     |
|                                            | jmespath    | 0.163    | 500     | 61.5     |
|                                            | jsonpath-ng | 1.633    | 500     | 61.5     |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 61.5     |
|                                            | jmespath    | 0.005    | 1       | 61.5     |
|                                            | jsonpath-ng | 0.007    | 1       | 61.5     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.029    | 500     | 77.3     |
|                                            | jmespath    | 0.179    | 500     | 77.3     |
|                                            | jsonpath-ng | 2.286    | 500     | 77.3     |
| wide_arrays:$.records\[0\]                 | strata      | 0.004    | 1       | 77.3     |
|                                            | jmespath    | 0.007    | 1       | 77.3     |
|                                            | jsonpath-ng | 0.009    | 1       | 77.3     |
| mixed:$.records\[\*\].id                   | strata      | 0.031    | 500     | 77.3     |
|                                            | jmespath    | 0.170    | 500     | 77.3     |
|                                            | jsonpath-ng | 1.714    | 500     | 77.3     |
| mixed:$.records\[0\]                       | strata      | 0.002    | 1       | 77.3     |
|                                            | jmespath    | 0.005    | 1       | 77.3     |
|                                            | jsonpath-ng | 0.006    | 1       | 77.3     |

## Summary

| Category            | Strata Rank             | vs #1                             |
| ------------------- | ----------------------- | --------------------------------- |
| loads (JSON)        | **#5** / 5              | 154.8% behind #1 (orjson)         |
| loads (NDJSON)      | **#5** / 5              | 152.9% behind #1 (orjson)         |
| load (JSON file)    | **#5** / 5              | 158.1% behind #1 (orjson)         |
| load (NDJSON file)  | **#5** / 5              | 144.1% behind #1 (orjson)         |
| dumps               | **#3** / 5              | 4.0% behind #1 (orjson)           |
| dump                | **#3** / 5              | 10.0% behind #1 (msgspec)         |
| loads (flat)        | **#4** / 5              | 142.0% behind #1 (orjson)         |
| load (flat)         | **#4** / 5              | 108.2% behind #1 (orjson)         |
| dumps (flat)        | **#3** / 5              | 25.5% behind #1 (orjson)          |
| dump (flat)         | **#1** / 5              | **2.1% faster** than #2 (orjson)  |
| loads (nested)      | **#4** / 5              | 41.4% behind #1 (orjson)          |
| load (nested)       | **#4** / 5              | 38.2% behind #1 (msgspec)         |
| dumps (nested)      | **#1** / 5              | **13.5% faster** than #2 (orjson) |
| dump (nested)       | **#1** / 5              | **24.2% faster** than #2 (orjson) |
| loads (wide_arrays) | **#3** / 5              | 85.3% behind #1 (orjson)          |
| load (wide_arrays)  | **#4** / 5              | 114.9% behind #1 (orjson)         |
| dumps (wide_arrays) | **#3** / 5              | 19.8% behind #1 (orjson)          |
| dump (wide_arrays)  | **#2** / 5              | 5.7% behind #1 (orjson)           |
| loads (mixed)       | **#4** / 5              | 172.1% behind #1 (msgspec)        |
| load (mixed)        | **#4** / 5              | 194.7% behind #1 (orjson)         |
| dumps (mixed)       | **#3** / 5              | 15.0% behind #1 (orjson)          |
| dump (mixed)        | **#1** / 5              | **6.9% faster** than #2 (msgspec) |
| search (JSONPath)   | **#1** in 11/11 queries | avg **872% faster**               |
| query (JSONPath)    | **#1** in 13/13 queries | -                                 |
