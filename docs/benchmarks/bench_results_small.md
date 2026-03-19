# Strata Benchmark Results

Generated: 2026-03-19 17:13:47

## Environment

- Commit: a14321cee5dc0690142acfa20e3e17374e5476f2
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
| orjson        | flat         | 0.651    | 0.761       | 0.776    | 62.3     |
| msgspec       | flat         | 0.693    | 0.805       | 0.818    | 62.3     |
| strata        | flat         | 0.811    | 0.950       | 0.977    | 61.7     |
| ujson         | flat         | 1.354    | 1.404       | 1.438    | 62.3     |
| json (stdlib) | flat         | 2.431    | 2.482       | 3.118    | 62.3     |
| orjson        | mixed        | 0.583    | 0.728       | 0.852    | 77.0     |
| msgspec       | mixed        | 0.694    | 0.878       | 0.970    | 77.0     |
| strata        | mixed        | 0.793    | 0.933       | 0.938    | 77.0     |
| ujson         | mixed        | 1.129    | 1.306       | 1.342    | 77.0     |
| json (stdlib) | mixed        | 2.288    | 2.499       | 2.547    | 77.0     |
| strata        | nested       | 0.750    | 0.812       | 0.901    | 63.1     |
| msgspec       | nested       | 1.616    | 2.073       | 2.195    | 63.1     |
| orjson        | nested       | 1.668    | 2.077       | 3.589    | 63.1     |
| ujson         | nested       | 2.024    | 2.297       | 2.464    | 63.1     |
| json (stdlib) | nested       | 2.527    | 2.680       | 2.773    | 63.1     |
| strata        | users.json   | 3.989    | 4.655       | 4.958    | 34.4     |
| orjson        | users.json   | 4.393    | 5.895       | 6.227    | 38.4     |
| msgspec       | users.json   | 5.002    | 9.078       | 11.049   | 41.5     |
| ujson         | users.json   | 6.958    | 11.068      | 11.246   | 41.4     |
| json (stdlib) | users.json   | 8.889    | 11.089      | 11.107   | 44.5     |
| strata        | users.ndjson | 4.863    | 5.930       | 6.074    | 56.8     |
| msgspec       | users.ndjson | 6.553    | 8.409       | 11.767   | 59.8     |
| orjson        | users.ndjson | 7.342    | 8.735       | 10.836   | 57.8     |
| ujson         | users.ndjson | 9.669    | 14.223      | 16.938   | 59.8     |
| json (stdlib) | users.ndjson | 10.956   | 15.423      | 17.098   | 59.8     |
| strata        | wide_arrays  | 3.561    | 4.350       | 4.383    | 65.1     |
| orjson        | wide_arrays  | 4.506    | 4.756       | 5.206    | 67.7     |
| msgspec       | wide_arrays  | 4.827    | 5.925       | 6.223    | 67.8     |
| ujson         | wide_arrays  | 7.774    | 8.262       | 8.536    | 67.8     |
| json (stdlib) | wide_arrays  | 14.854   | 16.645      | 16.820   | 69.1     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| msgspec       | flat         | 0.847    | 0.922       | 1.809    | 62.3     |
| orjson        | flat         | 1.071    | 1.109       | 1.630    | 62.3     |
| strata        | flat         | 1.122    | 1.140       | 1.336    | 62.3     |
| ujson         | flat         | 1.497    | 1.685       | 4.189    | 62.3     |
| json (stdlib) | flat         | 2.334    | 3.252       | 3.994    | 62.9     |
| orjson        | mixed        | 0.814    | 0.952       | 0.963    | 77.0     |
| msgspec       | mixed        | 0.855    | 1.013       | 1.104    | 77.0     |
| strata        | mixed        | 0.940    | 1.105       | 1.129    | 77.0     |
| ujson         | mixed        | 1.337    | 1.542       | 1.573    | 77.0     |
| json (stdlib) | mixed        | 2.716    | 2.775       | 2.790    | 77.0     |
| strata        | nested       | 0.786    | 0.883       | 0.959    | 63.1     |
| orjson        | nested       | 1.726    | 1.918       | 2.000    | 63.1     |
| msgspec       | nested       | 1.805    | 2.864       | 2.893    | 63.1     |
| ujson         | nested       | 1.987    | 3.335       | 3.386    | 63.1     |
| json (stdlib) | nested       | 3.453    | 3.626       | 3.957    | 63.1     |
| strata        | users.json   | 4.018    | 4.850       | 4.974    | 44.5     |
| msgspec       | users.json   | 4.924    | 6.252       | 6.436    | 48.6     |
| orjson        | users.json   | 5.040    | 6.684       | 7.655    | 44.5     |
| ujson         | users.json   | 6.249    | 9.288       | 9.311    | 48.6     |
| json (stdlib) | users.json   | 8.725    | 10.353      | 10.378   | 48.6     |
| strata        | users.ndjson | 4.697    | 5.419       | 5.544    | 59.8     |
| orjson        | users.ndjson | 4.656    | 6.324       | 6.416    | 59.8     |
| msgspec       | users.ndjson | 6.235    | 9.247       | 12.102   | 61.1     |
| ujson         | users.ndjson | 6.900    | 12.349      | 14.350   | 61.1     |
| json (stdlib) | users.ndjson | 10.734   | 13.546      | 17.082   | 61.1     |
| strata        | wide_arrays  | 4.062    | 4.747       | 4.860    | 69.1     |
| orjson        | wide_arrays  | 4.546    | 5.432       | 5.557    | 69.8     |
| msgspec       | wide_arrays  | 6.369    | 6.493       | 6.863    | 72.4     |
| ujson         | wide_arrays  | 8.502    | 9.859       | 10.246   | 72.4     |
| json (stdlib) | wide_arrays  | 16.732   | 16.995      | 17.797   | 72.4     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.263    | 0.271       | 0.520    | 62.9     |
| orjson  | flat        | 0.273    | 0.295       | 0.580    | 62.9     |
| msgspec | flat        | 0.315    | 0.317       | 0.319    | 62.9     |
| ujson   | flat        | 1.271    | 1.272       | 1.273    | 62.9     |
| json    | flat        | 2.387    | 2.402       | 8.097    | 63.0     |
| orjson  | mixed       | 0.319    | 0.330       | 0.382    | 77.1     |
| strata  | mixed       | 0.365    | 0.367       | 0.372    | 77.0     |
| msgspec | mixed       | 0.348    | 0.371       | 0.397    | 77.1     |
| ujson   | mixed       | 1.511    | 1.519       | 1.545    | 77.1     |
| json    | mixed       | 3.431    | 3.482       | 3.580    | 77.1     |
| strata  | nested      | 0.200    | 0.225       | 0.273    | 63.1     |
| msgspec | nested      | 0.222    | 0.229       | 0.232    | 63.1     |
| orjson  | nested      | 0.212    | 0.237       | 0.252    | 63.1     |
| ujson   | nested      | 0.997    | 1.008       | 1.019    | 63.1     |
| json    | nested      | 1.342    | 1.342       | 1.400    | 63.1     |
| strata  | users.json  | 1.336    | 1.339       | 1.423    | 49.2     |
| msgspec | users.json  | 1.469    | 1.491       | 1.716    | 52.4     |
| orjson  | users.json  | 1.488    | 1.514       | 1.604    | 51.3     |
| ujson   | users.json  | 6.533    | 6.631       | 6.799    | 53.2     |
| json    | users.json  | 9.027    | 9.052       | 9.080    | 53.8     |
| strata  | wide_arrays | 1.752    | 1.782       | 2.006    | 73.6     |
| orjson  | wide_arrays | 1.770    | 1.786       | 2.865    | 75.5     |
| msgspec | wide_arrays | 1.852    | 1.886       | 1.998    | 75.5     |
| ujson   | wide_arrays | 7.743    | 7.932       | 8.730    | 76.7     |
| json    | wide_arrays | 22.881   | 23.909      | 24.505   | 76.7     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.432    | 0.478       | 0.753    | 63.0     |
| msgspec       | flat        | 0.844    | 0.847       | 1.083    | 63.0     |
| orjson        | flat        | 0.931    | 1.188       | 1.786    | 63.0     |
| ujson         | flat        | 1.676    | 2.112       | 3.543    | 63.0     |
| json (stdlib) | flat        | 9.236    | 9.289       | 13.351   | 63.0     |
| strata        | mixed       | 0.507    | 0.558       | 1.000    | 77.1     |
| msgspec       | mixed       | 0.568    | 0.635       | 0.683    | 77.1     |
| orjson        | mixed       | 0.634    | 0.927       | 0.954    | 77.1     |
| ujson         | mixed       | 1.769    | 2.212       | 2.237    | 77.1     |
| json (stdlib) | mixed       | 10.718   | 11.257      | 12.354   | 77.1     |
| msgspec       | nested      | 0.493    | 0.534       | 1.087    | 63.1     |
| strata        | nested      | 0.431    | 0.749       | 0.845    | 63.1     |
| orjson        | nested      | 0.534    | 0.901       | 1.019    | 63.1     |
| ujson         | nested      | 1.336    | 1.727       | 1.814    | 63.1     |
| json (stdlib) | nested      | 13.841   | 14.165      | 14.388   | 63.1     |
| strata        | users.json  | 1.632    | 1.697       | 1.902    | 53.8     |
| msgspec       | users.json  | 1.742    | 1.742       | 1.933    | 53.8     |
| orjson        | users.json  | 2.143    | 2.152       | 2.278    | 53.8     |
| ujson         | users.json  | 7.122    | 7.377       | 7.532    | 53.8     |
| json (stdlib) | users.json  | 67.749   | 67.961      | 68.352   | 53.8     |
| strata        | wide_arrays | 2.084    | 2.357       | 2.669    | 76.7     |
| orjson        | wide_arrays | 2.324    | 2.439       | 2.540    | 76.7     |
| msgspec       | wide_arrays | 2.298    | 2.501       | 3.260    | 76.7     |
| ujson         | wide_arrays | 10.098   | 10.140      | 15.458   | 76.7     |
| json (stdlib) | wide_arrays | 48.066   | 48.776      | 50.947   | 76.7     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.955    | 1000    | 53.8     |
|                                | orjson+jmespath    | 5.557    | 1000    | 53.9     |
|                                | orjson+jsonpath-ng | 7.528    | 1000    | 54.4     |
| all item prices                | strata             | 2.144    | 12046   | 54.5     |
|                                | orjson+jmespath    | 14.130   | 1000    | 54.5     |
|                                | orjson+jsonpath-ng | 36.341   | 12046   | 54.2     |
| recursive price                | strata             | 2.211    | 12046   | 53.9     |
|                                | orjson+jsonpath-ng | 130.530  | 12046   | 54.7     |
| flat:$.records\[\*\].id        | strata             | 0.614    | 500     | 63.0     |
|                                | orjson+jmespath    | 1.023    | 500     | 63.0     |
|                                | orjson+jsonpath-ng | 3.491    | 500     | 63.0     |
| flat:$.records\[0\]            | strata             | 0.563    | 1       | 63.0     |
|                                | orjson+jmespath    | 0.744    | 1       | 63.0     |
|                                | orjson+jsonpath-ng | 0.748    | 1       | 63.0     |
| nested:$.records\[\*\].id      | strata             | 0.428    | 500     | 63.1     |
|                                | orjson+jmespath    | 1.935    | 500     | 63.1     |
|                                | orjson+jsonpath-ng | 2.468    | 500     | 63.1     |
| nested:$.records\[0\]          | strata             | 0.416    | 1       | 63.1     |
|                                | orjson+jsonpath-ng | 1.736    | 1       | 63.1     |
|                                | orjson+jmespath    | 1.870    | 1       | 63.1     |
| wide_arrays:$.records\[\*\].id | strata             | 2.764    | 500     | 76.7     |
|                                | orjson+jmespath    | 4.996    | 500     | 76.7     |
|                                | orjson+jsonpath-ng | 7.188    | 500     | 76.7     |
| wide_arrays:$.records\[0\]     | strata             | 2.870    | 1       | 76.7     |
|                                | orjson+jsonpath-ng | 4.427    | 1       | 76.7     |
|                                | orjson+jmespath    | 4.756    | 1       | 76.7     |
| mixed:$.records\[\*\].id       | strata             | 0.540    | 500     | 77.1     |
|                                | orjson+jmespath    | 0.838    | 500     | 77.1     |
|                                | orjson+jsonpath-ng | 2.510    | 500     | 77.1     |
| mixed:$.records\[0\]           | strata             | 0.604    | 1       | 77.1     |
|                                | orjson+jsonpath-ng | 0.772    | 1       | 77.1     |
|                                | orjson+jmespath    | 0.798    | 1       | 77.1     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.066    | 1000    | 55.5     |
|                                            | jmespath    | 0.409    | 1000    | 55.5     |
|                                            | jsonpath-ng | 4.955    | 1000    | 55.5     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 1.074    | 12046   | 55.6     |
|                                            | jmespath    | 13.185   | 1000    | 55.6     |
|                                            | jsonpath-ng | 33.771   | 12046   | 58.2     |
| $..price                                   | strata      | 1.481    | 12046   | 56.3     |
|                                            | jsonpath-ng | 133.488  | 12046   | 59.3     |
| $.users\[?(@.age>30)\]                     | strata      | 0.214    | 794     | 56.7     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.634    | 1604    | 56.7     |
| flat:$.records\[\*\].id                    | strata      | 0.038    | 500     | 63.0     |
|                                            | jmespath    | 0.192    | 500     | 63.0     |
|                                            | jsonpath-ng | 1.772    | 500     | 63.0     |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 63.0     |
|                                            | jmespath    | 0.007    | 1       | 63.0     |
|                                            | jsonpath-ng | 0.007    | 1       | 63.0     |
| nested:$.records\[\*\].id                  | strata      | 0.029    | 500     | 63.1     |
|                                            | jmespath    | 0.182    | 500     | 63.1     |
|                                            | jsonpath-ng | 2.585    | 500     | 63.1     |
| nested:$.records\[0\]                      | jsonpath-ng | 0.009    | 1       | 63.1     |
|                                            | strata      | 0.017    | 1       | 63.1     |
|                                            | jmespath    | 0.020    | 1       | 63.1     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.031    | 500     | 76.7     |
|                                            | jmespath    | 0.203    | 500     | 76.7     |
|                                            | jsonpath-ng | 2.334    | 500     | 76.7     |
| wide_arrays:$.records\[0\]                 | jmespath    | 0.006    | 1       | 76.7     |
|                                            | strata      | 0.007    | 1       | 76.7     |
|                                            | jsonpath-ng | 0.008    | 1       | 76.7     |
| mixed:$.records\[\*\].id                   | strata      | 0.048    | 500     | 77.1     |
|                                            | jmespath    | 0.228    | 500     | 77.1     |
|                                            | jsonpath-ng | 2.592    | 500     | 77.1     |
| mixed:$.records\[0\]                       | strata      | 0.004    | 1       | 77.1     |
|                                            | jmespath    | 0.007    | 1       | 77.1     |
|                                            | jsonpath-ng | 0.010    | 1       | 77.1     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **10.1% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **34.8% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **22.5% faster** than #2 (msgspec)  |
| load (NDJSON file)  | **#2** / 5              | 0.9% behind #1 (orjson)             |
| dumps               | **#1** / 5              | **10.0% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **6.7% faster** than #2 (msgspec)   |
| loads (flat)        | **#3** / 5              | 24.5% behind #1 (orjson)            |
| load (flat)         | **#3** / 5              | 32.4% behind #1 (msgspec)           |
| dumps (flat)        | **#1** / 5              | **3.5% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **95.5% faster** than #2 (msgspec)  |
| loads (nested)      | **#1** / 5              | **115.5% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **119.6% faster** than #2 (orjson)  |
| dumps (nested)      | **#1** / 5              | **6.2% faster** than #2 (orjson)    |
| dump (nested)       | **#1** / 5              | **14.4% faster** than #2 (msgspec)  |
| loads (wide_arrays) | **#1** / 5              | **26.5% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **11.9% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **1.0% faster** than #2 (orjson)    |
| dump (wide_arrays)  | **#1** / 5              | **10.3% faster** than #2 (msgspec)  |
| loads (mixed)       | **#3** / 5              | 36.0% behind #1 (orjson)            |
| load (mixed)        | **#3** / 5              | 15.6% behind #1 (orjson)            |
| dumps (mixed)       | **#3** / 5              | 14.4% behind #1 (orjson)            |
| dump (mixed)        | **#1** / 5              | **12.0% faster** than #2 (msgspec)  |
| search (JSONPath)   | **#1** in 11/11 queries | avg **685% faster**                 |
| query (JSONPath)    | **#1** in 11/13 queries | -                                   |
