# Strata Benchmark Results

Generated: 2026-03-20 01:03:45

## Environment

- Commit: aee3e869a1ae51b05b838b86bb2239e87e02b166
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 10
- Warmup: 5
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson
- Random schemas: flat, nested, wide_arrays, mixed (500 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.608    | 0.726       | 0.897    | 60.8     |
| strata        | flat         | 0.725    | 0.818       | 0.935    | 60.2     |
| msgspec       | flat         | 0.838    | 0.966       | 0.979    | 60.8     |
| ujson         | flat         | 1.553    | 1.823       | 1.943    | 60.8     |
| json (stdlib) | flat         | 2.820    | 2.952       | 3.052    | 60.8     |
| orjson        | mixed        | 0.531    | 0.608       | 0.704    | 77.5     |
| msgspec       | mixed        | 0.577    | 0.724       | 0.878    | 77.5     |
| strata        | mixed        | 0.686    | 0.915       | 1.181    | 77.5     |
| ujson         | mixed        | 1.130    | 1.150       | 1.437    | 77.5     |
| json (stdlib) | mixed        | 2.090    | 2.297       | 2.321    | 77.5     |
| strata        | nested       | 0.637    | 0.755       | 0.812    | 60.0     |
| orjson        | nested       | 1.810    | 2.045       | 2.218    | 60.0     |
| msgspec       | nested       | 1.786    | 2.095       | 2.915    | 60.0     |
| ujson         | nested       | 1.996    | 2.400       | 2.723    | 60.0     |
| json (stdlib) | nested       | 3.139    | 4.088       | 4.527    | 60.0     |
| strata        | users.json   | 4.204    | 5.101       | 6.188    | 40.6     |
| msgspec       | users.json   | 4.835    | 6.715       | 6.990    | 47.9     |
| orjson        | users.json   | 4.508    | 6.985       | 8.462    | 43.8     |
| ujson         | users.json   | 7.510    | 8.316       | 18.765   | 48.8     |
| json (stdlib) | users.json   | 8.727    | 11.005      | 13.744   | 48.6     |
| msgspec       | users.ndjson | 4.770    | 6.301       | 7.105    | 59.6     |
| strata        | users.ndjson | 5.650    | 6.930       | 7.353    | 58.5     |
| orjson        | users.ndjson | 6.763    | 9.676       | 11.492   | 59.6     |
| ujson         | users.ndjson | 9.249    | 10.558      | 11.265   | 59.6     |
| json (stdlib) | users.ndjson | 10.196   | 13.542      | 17.167   | 59.6     |
| strata        | wide_arrays  | 4.191    | 4.659       | 6.846    | 62.8     |
| msgspec       | wide_arrays  | 5.175    | 5.290       | 6.016    | 65.5     |
| orjson        | wide_arrays  | 5.346    | 6.641       | 8.742    | 65.4     |
| ujson         | wide_arrays  | 10.423   | 12.850      | 16.727   | 65.5     |
| json (stdlib) | wide_arrays  | 13.694   | 14.320      | 15.202   | 66.8     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.911    | 1.082       | 1.109    | 60.8     |
| msgspec       | flat         | 0.998    | 1.134       | 1.317    | 60.9     |
| strata        | flat         | 1.121    | 1.266       | 1.455    | 60.8     |
| ujson         | flat         | 1.834    | 2.009       | 2.102    | 60.9     |
| json (stdlib) | flat         | 3.409    | 3.601       | 4.196    | 60.9     |
| orjson        | mixed        | 0.719    | 0.885       | 0.903    | 77.5     |
| msgspec       | mixed        | 0.951    | 1.019       | 1.092    | 77.5     |
| strata        | mixed        | 0.889    | 1.072       | 1.154    | 77.5     |
| ujson         | mixed        | 1.356    | 1.572       | 1.748    | 77.5     |
| json (stdlib) | mixed        | 2.822    | 3.243       | 3.397    | 77.5     |
| strata        | nested       | 1.021    | 1.077       | 1.132    | 60.0     |
| msgspec       | nested       | 1.929    | 2.104       | 2.277    | 60.0     |
| orjson        | nested       | 2.074    | 2.352       | 2.397    | 60.0     |
| ujson         | nested       | 2.352    | 2.505       | 3.206    | 60.0     |
| json (stdlib) | nested       | 2.704    | 2.901       | 3.163    | 60.0     |
| strata        | users.json   | 3.893    | 5.298       | 5.741    | 49.2     |
| orjson        | users.json   | 5.766    | 6.725       | 7.548    | 49.5     |
| ujson         | users.json   | 6.720    | 8.659       | 9.748    | 52.6     |
| msgspec       | users.json   | 4.961    | 9.487       | 10.616   | 52.6     |
| json (stdlib) | users.json   | 9.274    | 14.773      | 19.199   | 52.6     |
| strata        | users.ndjson | 5.831    | 7.167       | 7.323    | 59.6     |
| msgspec       | users.ndjson | 6.159    | 7.693       | 9.220    | 59.6     |
| orjson        | users.ndjson | 6.291    | 9.071       | 9.197    | 59.6     |
| json (stdlib) | users.ndjson | 9.642    | 11.557      | 14.360   | 59.6     |
| ujson         | users.ndjson | 10.424   | 13.634      | 15.950   | 59.6     |
| strata        | wide_arrays  | 3.051    | 3.436       | 4.372    | 67.1     |
| msgspec       | wide_arrays  | 4.390    | 5.281       | 9.211    | 71.0     |
| orjson        | wide_arrays  | 3.609    | 5.646       | 6.426    | 68.4     |
| ujson         | wide_arrays  | 6.903    | 7.963       | 12.385   | 71.0     |
| json (stdlib) | wide_arrays  | 14.240   | 15.071      | 15.203   | 71.0     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.386    | 0.396       | 0.679    | 60.9     |
| strata  | flat        | 0.422    | 0.429       | 0.953    | 60.9     |
| msgspec | flat        | 0.426    | 0.432       | 0.441    | 60.9     |
| ujson   | flat        | 1.510    | 1.517       | 1.781    | 60.9     |
| json    | flat        | 2.407    | 2.424       | 6.452    | 60.9     |
| orjson  | mixed       | 0.382    | 0.388       | 0.421    | 77.5     |
| strata  | mixed       | 0.396    | 0.407       | 0.455    | 77.5     |
| msgspec | mixed       | 0.419    | 0.422       | 0.453    | 77.5     |
| ujson   | mixed       | 1.484    | 1.632       | 1.694    | 77.5     |
| json    | mixed       | 3.120    | 3.212       | 3.500    | 77.5     |
| orjson  | nested      | 0.197    | 0.203       | 0.219    | 60.0     |
| msgspec | nested      | 0.203    | 0.205       | 0.233    | 60.0     |
| strata  | nested      | 0.198    | 0.206       | 0.216    | 60.0     |
| ujson   | nested      | 1.022    | 1.030       | 1.059    | 60.0     |
| json    | nested      | 1.330    | 1.364       | 1.563    | 60.0     |
| strata  | users.json  | 1.853    | 1.874       | 2.024    | 53.4     |
| msgspec | users.json  | 2.083    | 2.098       | 2.218    | 57.8     |
| orjson  | users.json  | 2.090    | 2.108       | 2.372    | 57.1     |
| ujson   | users.json  | 9.698    | 9.730       | 10.910   | 57.8     |
| json    | users.json  | 9.179    | 10.028      | 12.881   | 59.2     |
| strata  | wide_arrays | 1.558    | 1.575       | 1.592    | 72.3     |
| orjson  | wide_arrays | 1.577    | 1.684       | 1.833    | 74.1     |
| msgspec | wide_arrays | 2.035    | 2.079       | 2.180    | 77.1     |
| ujson   | wide_arrays | 7.433    | 8.035       | 8.084    | 77.5     |
| json    | wide_arrays | 19.579   | 19.965      | 20.314   | 77.5     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.418    | 0.454       | 1.238    | 60.9     |
| orjson        | flat        | 0.472    | 0.561       | 1.976    | 60.9     |
| msgspec       | flat        | 0.545    | 0.597       | 1.517    | 60.9     |
| ujson         | flat        | 1.440    | 1.466       | 1.743    | 60.9     |
| json (stdlib) | flat        | 8.344    | 8.615       | 8.884    | 60.9     |
| orjson        | mixed       | 0.448    | 0.478       | 0.500    | 77.5     |
| strata        | mixed       | 0.419    | 0.485       | 0.564    | 77.5     |
| msgspec       | mixed       | 0.533    | 0.663       | 0.741    | 77.5     |
| ujson         | mixed       | 1.662    | 1.712       | 1.869    | 77.5     |
| json (stdlib) | mixed       | 11.539   | 13.761      | 13.998   | 77.5     |
| strata        | nested      | 0.363    | 0.428       | 0.583    | 60.0     |
| orjson        | nested      | 0.481    | 0.554       | 0.908    | 60.0     |
| msgspec       | nested      | 0.494    | 0.563       | 0.810    | 60.0     |
| ujson         | nested      | 1.567    | 1.610       | 1.650    | 60.0     |
| json (stdlib) | nested      | 14.896   | 16.439      | 16.618   | 60.0     |
| strata        | users.json  | 1.573    | 1.619       | 1.651    | 59.2     |
| orjson        | users.json  | 1.880    | 2.229       | 2.434    | 59.2     |
| msgspec       | users.json  | 2.369    | 2.410       | 2.442    | 59.2     |
| ujson         | users.json  | 7.172    | 9.101       | 9.559    | 59.2     |
| json (stdlib) | users.json  | 65.207   | 68.293      | 92.229   | 59.2     |
| strata        | wide_arrays | 1.789    | 1.833       | 2.086    | 77.5     |
| orjson        | wide_arrays | 2.143    | 2.221       | 2.585    | 77.5     |
| msgspec       | wide_arrays | 2.479    | 2.530       | 2.597    | 77.5     |
| ujson         | wide_arrays | 8.490    | 9.278       | 9.414    | 77.5     |
| json (stdlib) | wide_arrays | 44.928   | 45.163      | 57.412   | 77.5     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 2.407    | 1000    | 59.3     |
|                                | orjson+jmespath    | 7.198    | 1000    | 59.4     |
|                                | orjson+jsonpath-ng | 8.876    | 1000    | 59.9     |
| all item prices                | strata             | 2.848    | 12046   | 59.9     |
|                                | orjson+jmespath    | 13.474   | 1000    | 59.9     |
|                                | orjson+jsonpath-ng | 34.758   | 12046   | 54.7     |
| recursive price                | strata             | 2.179    | 12046   | 54.7     |
|                                | orjson+jsonpath-ng | 124.255  | 12046   | 54.7     |
| flat:$.records\[\*\].id        | strata             | 0.540    | 500     | 60.9     |
|                                | orjson+jmespath    | 0.841    | 500     | 60.9     |
|                                | orjson+jsonpath-ng | 2.243    | 500     | 59.9     |
| flat:$.records\[0\]            | strata             | 0.440    | 1       | 59.9     |
|                                | orjson+jmespath    | 0.663    | 1       | 59.9     |
|                                | orjson+jsonpath-ng | 0.666    | 1       | 59.9     |
| nested:$.records\[\*\].id      | strata             | 0.522    | 500     | 60.0     |
|                                | orjson+jmespath    | 2.316    | 500     | 60.0     |
|                                | orjson+jsonpath-ng | 3.088    | 500     | 60.0     |
| nested:$.records\[0\]          | strata             | 0.500    | 1       | 60.0     |
|                                | orjson+jsonpath-ng | 1.637    | 1       | 60.0     |
|                                | orjson+jmespath    | 1.644    | 1       | 60.0     |
| wide_arrays:$.records\[\*\].id | strata             | 2.042    | 500     | 77.5     |
|                                | orjson+jmespath    | 6.096    | 500     | 77.5     |
|                                | orjson+jsonpath-ng | 7.089    | 500     | 77.5     |
| wide_arrays:$.records\[0\]     | strata             | 3.427    | 1       | 77.5     |
|                                | orjson+jsonpath-ng | 4.114    | 1       | 77.5     |
|                                | orjson+jmespath    | 5.365    | 1       | 77.5     |
| mixed:$.records\[\*\].id       | strata             | 0.622    | 500     | 77.5     |
|                                | orjson+jmespath    | 1.106    | 500     | 77.5     |
|                                | orjson+jsonpath-ng | 3.381    | 500     | 77.5     |
| mixed:$.records\[0\]           | strata             | 0.616    | 1       | 77.5     |
|                                | orjson+jmespath    | 0.880    | 1       | 77.5     |
|                                | orjson+jsonpath-ng | 0.886    | 1       | 77.5     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.063    | 1000    | 55.1     |
|                                            | jmespath    | 0.387    | 1000    | 55.1     |
|                                            | jsonpath-ng | 4.277    | 1000    | 55.1     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 1.252    | 12046   | 55.1     |
|                                            | jmespath    | 16.377   | 1000    | 55.1     |
|                                            | jsonpath-ng | 31.850   | 12046   | 56.3     |
| $..price                                   | strata      | 1.267    | 12046   | 57.3     |
|                                            | jsonpath-ng | 124.725  | 12046   | 58.3     |
| $.users\[?(@.age>30)\]                     | strata      | 0.111    | 794     | 57.6     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 2.024    | 1604    | 57.7     |
| flat:$.records\[\*\].id                    | strata      | 0.031    | 500     | 59.9     |
|                                            | jmespath    | 0.172    | 500     | 59.9     |
|                                            | jsonpath-ng | 1.562    | 500     | 59.9     |
| flat:$.records\[0\]                        | strata      | 0.002    | 1       | 59.9     |
|                                            | jmespath    | 0.005    | 1       | 59.9     |
|                                            | jsonpath-ng | 0.006    | 1       | 59.9     |
| nested:$.records\[\*\].id                  | strata      | 0.026    | 500     | 60.0     |
|                                            | jmespath    | 0.170    | 500     | 60.0     |
|                                            | jsonpath-ng | 1.728    | 500     | 60.0     |
| nested:$.records\[0\]                      | strata      | 0.004    | 1       | 60.0     |
|                                            | jmespath    | 0.005    | 1       | 60.0     |
|                                            | jsonpath-ng | 0.006    | 1       | 60.0     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.028    | 500     | 77.5     |
|                                            | jmespath    | 0.176    | 500     | 77.5     |
|                                            | jsonpath-ng | 2.244    | 500     | 77.5     |
| wide_arrays:$.records\[0\]                 | strata      | 0.002    | 1       | 77.5     |
|                                            | jmespath    | 0.007    | 1       | 77.5     |
|                                            | jsonpath-ng | 0.009    | 1       | 77.5     |
| mixed:$.records\[\*\].id                   | strata      | 0.040    | 500     | 77.5     |
|                                            | jmespath    | 0.235    | 500     | 77.5     |
|                                            | jsonpath-ng | 2.263    | 500     | 77.5     |
| mixed:$.records\[0\]                       | strata      | 0.002    | 1       | 77.5     |
|                                            | jmespath    | 0.005    | 1       | 77.5     |
|                                            | jsonpath-ng | 0.006    | 1       | 77.5     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **7.2% faster** than #2 (orjson)    |
| loads (NDJSON)      | **#2** / 5              | 18.5% behind #1 (msgspec)           |
| load (JSON file)    | **#1** / 5              | **27.4% faster** than #2 (msgspec)  |
| load (NDJSON file)  | **#1** / 5              | **5.6% faster** than #2 (msgspec)   |
| dumps               | **#1** / 5              | **12.4% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **19.5% faster** than #2 (orjson)   |
| loads (flat)        | **#2** / 5              | 19.3% behind #1 (orjson)            |
| load (flat)         | **#3** / 5              | 23.0% behind #1 (orjson)            |
| dumps (flat)        | **#2** / 5              | 9.3% behind #1 (orjson)             |
| dump (flat)         | **#1** / 5              | **12.9% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **180.5% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **89.0% faster** than #2 (msgspec)  |
| dumps (nested)      | **#2** / 5              | 0.9% behind #1 (orjson)             |
| dump (nested)       | **#1** / 5              | **32.5% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#1** / 5              | **23.5% faster** than #2 (msgspec)  |
| load (wide_arrays)  | **#1** / 5              | **18.3% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **1.2% faster** than #2 (orjson)    |
| dump (wide_arrays)  | **#1** / 5              | **19.8% faster** than #2 (orjson)   |
| loads (mixed)       | **#3** / 5              | 29.2% behind #1 (orjson)            |
| load (mixed)        | **#2** / 5              | 23.6% behind #1 (orjson)            |
| dumps (mixed)       | **#2** / 5              | 3.7% behind #1 (orjson)             |
| dump (mixed)        | **#1** / 5              | **6.7% faster** than #2 (orjson)    |
| search (JSONPath)   | **#1** in 11/11 queries | avg **654% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
