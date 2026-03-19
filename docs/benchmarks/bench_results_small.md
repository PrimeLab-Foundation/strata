# Strata Benchmark Results

Generated: 2026-03-19 17:50:55

## Environment

- Commit: f6a531005609a864792e4e9f097a7fc572656b95
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
| orjson        | flat         | 0.567    | 0.668       | 0.676    | 61.3     |
| msgspec       | flat         | 0.641    | 0.728       | 0.749    | 61.3     |
| strata        | flat         | 0.713    | 0.815       | 0.880    | 60.7     |
| ujson         | flat         | 1.110    | 1.274       | 1.276    | 61.3     |
| json (stdlib) | flat         | 2.060    | 2.123       | 2.127    | 61.3     |
| orjson        | mixed        | 0.536    | 0.610       | 0.632    | 75.8     |
| msgspec       | mixed        | 0.737    | 0.808       | 1.357    | 75.8     |
| strata        | mixed        | 0.821    | 0.920       | 0.925    | 75.8     |
| ujson         | mixed        | 1.081    | 1.167       | 1.733    | 75.8     |
| json (stdlib) | mixed        | 2.314    | 2.326       | 2.621    | 75.8     |
| strata        | nested       | 0.818    | 0.874       | 1.290    | 61.3     |
| msgspec       | nested       | 2.280    | 2.306       | 2.599    | 61.4     |
| ujson         | nested       | 2.308    | 3.060       | 4.197    | 61.4     |
| json (stdlib) | nested       | 2.892    | 3.229       | 3.299    | 61.4     |
| orjson        | nested       | 3.676    | 4.232       | 5.545    | 61.4     |
| strata        | users.json   | 4.014    | 4.998       | 5.348    | 34.4     |
| msgspec       | users.json   | 4.502    | 6.214       | 6.227    | 41.5     |
| orjson        | users.json   | 4.986    | 6.838       | 7.391    | 38.3     |
| ujson         | users.json   | 5.741    | 8.179       | 8.955    | 41.4     |
| json (stdlib) | users.json   | 8.886    | 10.619      | 11.028   | 43.5     |
| strata        | users.ndjson | 4.081    | 5.269       | 5.279    | 56.1     |
| orjson        | users.ndjson | 4.644    | 6.002       | 6.252    | 57.1     |
| msgspec       | users.ndjson | 5.339    | 6.330       | 7.126    | 58.1     |
| ujson         | users.ndjson | 7.067    | 9.459       | 10.169   | 58.1     |
| json (stdlib) | users.ndjson | 9.472    | 11.122      | 11.390   | 58.1     |
| strata        | wide_arrays  | 2.915    | 3.625       | 3.649    | 63.6     |
| orjson        | wide_arrays  | 3.713    | 4.871       | 4.972    | 66.2     |
| msgspec       | wide_arrays  | 4.671    | 5.070       | 5.671    | 66.2     |
| ujson         | wide_arrays  | 7.078    | 8.197       | 8.370    | 66.2     |
| json (stdlib) | wide_arrays  | 13.390   | 13.897      | 14.007   | 67.5     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 0.824    | 0.903       | 0.978    | 61.3     |
| orjson        | flat         | 0.685    | 0.912       | 0.957    | 61.3     |
| msgspec       | flat         | 0.740    | 1.004       | 1.545    | 61.3     |
| ujson         | flat         | 1.274    | 1.478       | 1.546    | 61.3     |
| json (stdlib) | flat         | 2.367    | 2.410       | 2.420    | 61.3     |
| orjson        | mixed        | 0.673    | 0.760       | 0.793    | 75.8     |
| msgspec       | mixed        | 0.833    | 0.884       | 1.047    | 75.8     |
| strata        | mixed        | 0.820    | 1.005       | 1.018    | 75.8     |
| ujson         | mixed        | 1.553    | 1.989       | 2.895    | 75.8     |
| json (stdlib) | mixed        | 2.621    | 2.636       | 2.711    | 75.8     |
| strata        | nested       | 0.784    | 0.962       | 0.996    | 61.4     |
| orjson        | nested       | 1.834    | 2.002       | 6.954    | 61.4     |
| msgspec       | nested       | 1.839    | 2.218       | 2.867    | 61.8     |
| json (stdlib) | nested       | 2.763    | 3.304       | 3.505    | 61.8     |
| ujson         | nested       | 3.187    | 6.565       | 6.979    | 61.8     |
| strata        | users.json   | 4.282    | 4.837       | 5.247    | 43.5     |
| orjson        | users.json   | 4.888    | 6.480       | 7.094    | 44.4     |
| msgspec       | users.json   | 5.729    | 6.707       | 6.938    | 47.5     |
| ujson         | users.json   | 7.324    | 9.470       | 9.704    | 47.5     |
| json (stdlib) | users.json   | 8.870    | 10.514      | 10.655   | 48.5     |
| strata        | users.ndjson | 4.509    | 5.208       | 5.396    | 58.1     |
| orjson        | users.ndjson | 4.541    | 6.353       | 6.722    | 58.1     |
| ujson         | users.ndjson | 6.862    | 8.860       | 9.095    | 60.1     |
| msgspec       | users.ndjson | 5.534    | 8.911       | 9.468    | 60.1     |
| json (stdlib) | users.ndjson | 11.192   | 11.640      | 14.761   | 60.1     |
| strata        | wide_arrays  | 3.100    | 3.711       | 3.769    | 67.5     |
| orjson        | wide_arrays  | 3.858    | 4.548       | 4.833    | 68.8     |
| msgspec       | wide_arrays  | 4.950    | 5.330       | 5.595    | 71.5     |
| ujson         | wide_arrays  | 7.515    | 8.057       | 8.178    | 71.5     |
| json (stdlib) | wide_arrays  | 13.895   | 14.403      | 14.823   | 71.5     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.245    | 0.248       | 0.260    | 61.3     |
| orjson  | flat        | 0.255    | 0.265       | 0.292    | 61.3     |
| msgspec | flat        | 0.306    | 0.358       | 0.884    | 61.3     |
| ujson   | flat        | 1.225    | 1.254       | 1.638    | 61.3     |
| json    | flat        | 2.290    | 2.292       | 2.302    | 61.3     |
| orjson  | mixed       | 0.296    | 0.324       | 0.339    | 75.8     |
| strata  | mixed       | 0.334    | 0.337       | 0.451    | 75.8     |
| msgspec | mixed       | 0.305    | 0.351       | 0.356    | 75.8     |
| ujson   | mixed       | 1.315    | 1.322       | 1.349    | 75.8     |
| json    | mixed       | 3.185    | 3.294       | 3.610    | 75.8     |
| strata  | nested      | 0.211    | 0.215       | 0.423    | 61.8     |
| orjson  | nested      | 0.206    | 0.215       | 0.395    | 61.8     |
| msgspec | nested      | 0.218    | 0.221       | 0.287    | 61.8     |
| ujson   | nested      | 1.024    | 1.080       | 2.192    | 61.8     |
| json    | nested      | 1.358    | 1.414       | 1.817    | 61.8     |
| strata  | users.json  | 1.344    | 1.347       | 1.434    | 49.4     |
| msgspec | users.json  | 1.498    | 1.499       | 1.509    | 52.5     |
| orjson  | users.json  | 1.516    | 1.549       | 1.583    | 52.2     |
| ujson   | users.json  | 6.370    | 6.371       | 6.386    | 52.5     |
| json    | users.json  | 8.990    | 8.991       | 9.045    | 52.9     |
| strata  | wide_arrays | 1.540    | 1.559       | 1.638    | 72.7     |
| orjson  | wide_arrays | 1.586    | 1.617       | 1.719    | 74.8     |
| msgspec | wide_arrays | 1.697    | 1.749       | 1.781    | 75.1     |
| ujson   | wide_arrays | 7.183    | 7.255       | 7.291    | 75.7     |
| json    | wide_arrays | 19.906   | 19.945      | 20.113   | 75.8     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.375    | 0.405       | 0.435    | 61.3     |
| orjson        | flat        | 0.497    | 0.516       | 0.596    | 61.3     |
| msgspec       | flat        | 0.533    | 0.537       | 0.561    | 61.3     |
| ujson         | flat        | 1.445    | 1.463       | 1.468    | 61.3     |
| json (stdlib) | flat        | 8.386    | 8.418       | 8.427    | 61.3     |
| strata        | mixed       | 0.562    | 0.577       | 0.589    | 75.8     |
| orjson        | mixed       | 0.547    | 0.598       | 0.700    | 75.8     |
| msgspec       | mixed       | 0.546    | 0.598       | 0.735    | 75.8     |
| ujson         | mixed       | 1.600    | 1.699       | 2.016    | 75.8     |
| json (stdlib) | mixed       | 10.215   | 10.295      | 11.457   | 75.8     |
| strata        | nested      | 0.384    | 0.416       | 0.518    | 61.8     |
| orjson        | nested      | 0.407    | 0.448       | 0.526    | 61.8     |
| msgspec       | nested      | 0.483    | 0.528       | 0.529    | 61.8     |
| ujson         | nested      | 1.233    | 1.282       | 1.323    | 61.8     |
| json (stdlib) | nested      | 12.325   | 12.517      | 13.015   | 61.8     |
| strata        | users.json  | 1.682    | 1.714       | 2.206    | 52.9     |
| msgspec       | users.json  | 1.752    | 1.868       | 2.137    | 52.9     |
| orjson        | users.json  | 1.861    | 1.892       | 1.931    | 52.9     |
| ujson         | users.json  | 6.922    | 7.164       | 7.893    | 52.9     |
| json (stdlib) | users.json  | 67.133   | 68.526      | 69.864   | 52.9     |
| orjson        | wide_arrays | 1.946    | 1.977       | 2.037    | 75.8     |
| strata        | wide_arrays | 1.961    | 1.993       | 2.103    | 75.8     |
| msgspec       | wide_arrays | 2.016    | 2.063       | 2.085    | 75.8     |
| ujson         | wide_arrays | 7.532    | 7.578       | 7.624    | 75.8     |
| json (stdlib) | wide_arrays | 45.302   | 45.577      | 45.715   | 75.8     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.908    | 1000    | 53.0     |
|                                | orjson+jsonpath-ng | 7.253    | 1000    | 53.5     |
|                                | orjson+jmespath    | 7.593    | 1000    | 53.0     |
| all item prices                | strata             | 2.354    | 12046   | 53.5     |
|                                | orjson+jmespath    | 14.173   | 1000    | 53.6     |
|                                | orjson+jsonpath-ng | 35.070   | 12046   | 53.5     |
| recursive price                | strata             | 2.225    | 12046   | 53.1     |
|                                | orjson+jsonpath-ng | 128.133  | 12046   | 54.0     |
| flat:$.records\[\*\].id        | strata             | 0.489    | 500     | 61.3     |
|                                | orjson+jmespath    | 0.834    | 500     | 61.3     |
|                                | orjson+jsonpath-ng | 2.236    | 500     | 61.3     |
| flat:$.records\[0\]            | strata             | 0.487    | 1       | 61.3     |
|                                | orjson+jsonpath-ng | 0.653    | 1       | 61.3     |
|                                | orjson+jmespath    | 0.660    | 1       | 61.3     |
| nested:$.records\[\*\].id      | strata             | 0.453    | 500     | 61.8     |
|                                | orjson+jmespath    | 2.135    | 500     | 61.8     |
|                                | orjson+jsonpath-ng | 2.397    | 500     | 61.8     |
| nested:$.records\[0\]          | strata             | 0.380    | 1       | 61.8     |
|                                | orjson+jsonpath-ng | 1.571    | 1       | 61.8     |
|                                | orjson+jmespath    | 1.587    | 1       | 61.8     |
| wide_arrays:$.records\[\*\].id | strata             | 2.554    | 500     | 75.8     |
|                                | orjson+jsonpath-ng | 5.498    | 500     | 75.8     |
|                                | orjson+jmespath    | 6.101    | 500     | 75.8     |
| wide_arrays:$.records\[0\]     | strata             | 2.460    | 1       | 75.8     |
|                                | orjson+jsonpath-ng | 4.191    | 1       | 75.8     |
|                                | orjson+jmespath    | 5.268    | 1       | 75.8     |
| mixed:$.records\[\*\].id       | strata             | 0.491    | 500     | 75.8     |
|                                | orjson+jmespath    | 0.788    | 500     | 75.8     |
|                                | orjson+jsonpath-ng | 2.325    | 500     | 75.8     |
| mixed:$.records\[0\]           | strata             | 0.594    | 1       | 75.8     |
|                                | orjson+jmespath    | 0.616    | 1       | 75.8     |
|                                | orjson+jsonpath-ng | 0.640    | 1       | 75.8     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.074    | 1000    | 53.8     |
|                                            | jmespath    | 0.420    | 1000    | 53.8     |
|                                            | jsonpath-ng | 4.358    | 1000    | 53.8     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.963    | 12046   | 53.9     |
|                                            | jmespath    | 11.828   | 1000    | 53.9     |
|                                            | jsonpath-ng | 32.456   | 12046   | 56.5     |
| $..price                                   | strata      | 1.317    | 12046   | 55.5     |
|                                            | jsonpath-ng | 124.853  | 12046   | 57.6     |
| $.users\[?(@.age>30)\]                     | strata      | 0.158    | 794     | 56.0     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.564    | 1604    | 56.0     |
| flat:$.records\[\*\].id                    | strata      | 0.037    | 500     | 61.3     |
|                                            | jmespath    | 0.173    | 500     | 61.3     |
|                                            | jsonpath-ng | 1.752    | 500     | 61.3     |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 61.3     |
|                                            | jmespath    | 0.006    | 1       | 61.3     |
|                                            | jsonpath-ng | 0.009    | 1       | 61.3     |
| nested:$.records\[\*\].id                  | strata      | 0.026    | 500     | 61.8     |
|                                            | jmespath    | 0.175    | 500     | 61.8     |
|                                            | jsonpath-ng | 1.762    | 500     | 61.8     |
| nested:$.records\[0\]                      | strata      | 0.004    | 1       | 61.8     |
|                                            | jsonpath-ng | 0.010    | 1       | 61.8     |
|                                            | jmespath    | 0.012    | 1       | 61.8     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.029    | 500     | 75.8     |
|                                            | jmespath    | 0.192    | 500     | 75.8     |
|                                            | jsonpath-ng | 2.396    | 500     | 75.8     |
| wide_arrays:$.records\[0\]                 | strata      | 0.004    | 1       | 75.8     |
|                                            | jmespath    | 0.008    | 1       | 75.8     |
|                                            | jsonpath-ng | 0.017    | 1       | 75.8     |
| mixed:$.records\[\*\].id                   | strata      | 0.032    | 500     | 75.8     |
|                                            | jmespath    | 0.207    | 500     | 75.8     |
|                                            | jsonpath-ng | 1.915    | 500     | 75.8     |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 75.8     |
|                                            | jmespath    | 0.006    | 1       | 75.8     |
|                                            | jsonpath-ng | 0.007    | 1       | 75.8     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **12.2% faster** than #2 (msgspec)  |
| loads (NDJSON)      | **#1** / 5              | **13.8% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **14.2% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **0.7% faster** than #2 (orjson)    |
| dumps               | **#1** / 5              | **11.5% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **4.2% faster** than #2 (msgspec)   |
| loads (flat)        | **#3** / 5              | 25.7% behind #1 (orjson)            |
| load (flat)         | **#3** / 5              | 20.2% behind #1 (orjson)            |
| dumps (flat)        | **#1** / 5              | **4.2% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **32.4% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **178.8% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **133.9% faster** than #2 (orjson)  |
| dumps (nested)      | **#2** / 5              | 2.2% behind #1 (orjson)             |
| dump (nested)       | **#1** / 5              | **5.9% faster** than #2 (orjson)    |
| loads (wide_arrays) | **#1** / 5              | **27.4% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **24.4% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **3.0% faster** than #2 (orjson)    |
| dump (wide_arrays)  | **#2** / 5              | 0.7% behind #1 (orjson)             |
| loads (mixed)       | **#3** / 5              | 53.2% behind #1 (orjson)            |
| load (mixed)        | **#2** / 5              | 21.9% behind #1 (orjson)            |
| dumps (mixed)       | **#3** / 5              | 13.0% behind #1 (orjson)            |
| dump (mixed)        | **#3** / 5              | 2.9% behind #1 (msgspec)            |
| search (JSONPath)   | **#1** in 11/11 queries | avg **680% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
