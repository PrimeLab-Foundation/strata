# Strata Benchmark Results

Generated: 2026-03-17 20:43:55

## Environment

- Commit: aa7d859c5354df2117bd93cad8c93b96f7917890
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
| orjson        | flat         | 0.595    | 0.721       | 0.830    | 63.8     |
| msgspec       | flat         | 0.647    | 0.743       | 0.753    | 63.8     |
| strata        | flat         | 0.874    | 1.158       | 1.253    | 63.1     |
| ujson         | flat         | 1.112    | 1.255       | 1.266    | 63.8     |
| json (stdlib) | flat         | 2.039    | 2.148       | 2.211    | 63.8     |
| orjson        | mixed        | 0.488    | 0.648       | 0.655    | 75.5     |
| msgspec       | mixed        | 0.614    | 0.770       | 0.870    | 75.5     |
| strata        | mixed        | 1.051    | 1.164       | 1.178    | 75.5     |
| ujson         | mixed        | 1.027    | 1.235       | 1.270    | 75.5     |
| json (stdlib) | mixed        | 2.365    | 2.534       | 2.754    | 75.5     |
| strata        | nested       | 1.006    | 1.017       | 1.020    | 63.8     |
| msgspec       | nested       | 1.859    | 1.928       | 2.177    | 63.8     |
| orjson        | nested       | 2.223    | 2.241       | 3.698    | 63.8     |
| ujson         | nested       | 1.930    | 2.721       | 4.430    | 63.8     |
| json (stdlib) | nested       | 2.823    | 3.041       | 3.087    | 63.8     |
| strata        | users.json   | 5.312    | 6.407       | 6.510    | 34.4     |
| orjson        | users.json   | 4.844    | 6.726       | 11.179   | 38.4     |
| ujson         | users.json   | 6.299    | 9.178       | 11.152   | 41.4     |
| msgspec       | users.json   | 6.542    | 10.204      | 10.911   | 41.6     |
| json (stdlib) | users.json   | 9.626    | 11.126      | 11.332   | 44.5     |
| strata        | users.ndjson | 5.596    | 6.590       | 33.212   | 58.5     |
| orjson        | users.ndjson | 7.085    | 7.463       | 7.552    | 59.5     |
| msgspec       | users.ndjson | 5.497    | 7.739       | 10.193   | 61.5     |
| ujson         | users.ndjson | 11.329   | 11.992      | 12.810   | 61.5     |
| json (stdlib) | users.ndjson | 11.545   | 12.008      | 15.031   | 61.5     |
| orjson        | wide_arrays  | 3.999    | 4.589       | 5.027    | 68.2     |
| msgspec       | wide_arrays  | 4.995    | 5.324       | 5.457    | 68.2     |
| strata        | wide_arrays  | 5.166    | 5.521       | 9.193    | 65.6     |
| ujson         | wide_arrays  | 7.128    | 7.896       | 9.565    | 68.2     |
| json (stdlib) | wide_arrays  | 14.164   | 14.338      | 16.211   | 69.3     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| msgspec       | flat         | 0.719    | 0.836       | 0.861    | 63.8     |
| orjson        | flat         | 0.789    | 0.843       | 0.868    | 63.8     |
| strata        | flat         | 1.092    | 1.095       | 1.108    | 63.8     |
| ujson         | flat         | 1.218    | 1.468       | 1.672    | 63.8     |
| json (stdlib) | flat         | 2.165    | 2.358       | 2.370    | 63.8     |
| orjson        | mixed        | 0.677    | 0.807       | 0.836    | 75.5     |
| msgspec       | mixed        | 0.924    | 0.934       | 0.941    | 75.5     |
| ujson         | mixed        | 1.175    | 1.404       | 1.588    | 75.5     |
| strata        | mixed        | 1.175    | 1.440       | 1.530    | 75.5     |
| json (stdlib) | mixed        | 2.439    | 2.595       | 6.838    | 75.5     |
| strata        | nested       | 0.932    | 1.128       | 1.206    | 63.8     |
| msgspec       | nested       | 1.901    | 2.186       | 2.245    | 63.8     |
| ujson         | nested       | 1.994    | 2.594       | 5.110    | 63.8     |
| orjson        | nested       | 2.502    | 3.124       | 3.295    | 63.8     |
| json (stdlib) | nested       | 2.923    | 3.356       | 3.576    | 63.8     |
| strata        | users.json   | 5.986    | 6.507       | 6.946    | 44.5     |
| msgspec       | users.json   | 5.242    | 6.858       | 8.263    | 48.6     |
| orjson        | users.json   | 6.388    | 7.095       | 7.982    | 44.5     |
| json (stdlib) | users.json   | 11.819   | 12.241      | 13.147   | 48.6     |
| ujson         | users.json   | 7.015    | 12.251      | 19.270   | 48.6     |
| orjson        | users.ndjson | 5.341    | 6.735       | 9.314    | 61.5     |
| strata        | users.ndjson | 6.886    | 6.937       | 6.988    | 61.5     |
| msgspec       | users.ndjson | 6.127    | 8.723       | 8.785    | 62.5     |
| ujson         | users.ndjson | 7.171    | 9.532       | 10.049   | 62.5     |
| json (stdlib) | users.ndjson | 9.480    | 11.420      | 11.725   | 62.5     |
| orjson        | wide_arrays  | 5.161    | 5.434       | 8.218    | 69.3     |
| msgspec       | wide_arrays  | 5.224    | 5.538       | 5.742    | 70.6     |
| strata        | wide_arrays  | 5.051    | 5.586       | 5.623    | 69.3     |
| ujson         | wide_arrays  | 8.652    | 8.877       | 12.558   | 70.6     |
| json (stdlib) | wide_arrays  | 13.849   | 15.715      | 17.316   | 70.6     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.248    | 0.251       | 0.253    | 63.8     |
| strata  | flat        | 0.305    | 0.319       | 0.680    | 63.8     |
| msgspec | flat        | 0.326    | 0.418       | 0.510    | 63.8     |
| ujson   | flat        | 1.230    | 1.232       | 1.275    | 63.8     |
| json    | flat        | 2.389    | 2.394       | 2.850    | 63.8     |
| orjson  | mixed       | 0.278    | 0.280       | 0.286    | 75.5     |
| msgspec | mixed       | 0.313    | 0.314       | 0.344    | 75.5     |
| strata  | mixed       | 0.384    | 0.388       | 0.402    | 75.5     |
| ujson   | mixed       | 1.338    | 1.341       | 1.348    | 75.5     |
| json    | mixed       | 3.043    | 3.056       | 3.064    | 75.5     |
| msgspec | nested      | 0.208    | 0.212       | 0.226    | 63.8     |
| orjson  | nested      | 0.210    | 0.213       | 0.219    | 63.8     |
| strata  | nested      | 0.271    | 0.272       | 0.282    | 63.8     |
| ujson   | nested      | 0.996    | 0.997       | 1.000    | 63.8     |
| json    | nested      | 1.342    | 1.376       | 1.566    | 63.8     |
| strata  | users.json  | 1.449    | 1.455       | 1.487    | 50.0     |
| msgspec | users.json  | 1.539    | 1.544       | 2.853    | 53.3     |
| orjson  | users.json  | 1.717    | 1.862       | 2.097    | 51.0     |
| ujson   | users.json  | 6.495    | 7.160       | 10.714   | 54.2     |
| json    | users.json  | 8.889    | 8.903       | 8.939    | 54.5     |
| orjson  | wide_arrays | 1.582    | 1.680       | 1.757    | 72.5     |
| msgspec | wide_arrays | 1.644    | 1.747       | 1.763    | 75.0     |
| strata  | wide_arrays | 2.055    | 2.073       | 2.087    | 70.6     |
| ujson   | wide_arrays | 7.129    | 7.166       | 7.170    | 75.4     |
| json    | wide_arrays | 19.878   | 19.951      | 20.032   | 75.4     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 0.500    | 0.515       | 0.866    | 63.8     |
| strata        | flat        | 0.476    | 0.540       | 0.976    | 63.8     |
| msgspec       | flat        | 0.702    | 0.872       | 0.909    | 63.8     |
| ujson         | flat        | 1.475    | 1.652       | 1.949    | 63.8     |
| json (stdlib) | flat        | 8.765    | 8.908       | 8.999    | 63.8     |
| orjson        | mixed       | 0.460    | 0.463       | 0.830    | 75.5     |
| msgspec       | mixed       | 0.585    | 0.605       | 0.616    | 75.5     |
| strata        | mixed       | 0.565    | 0.652       | 0.664    | 75.5     |
| ujson         | mixed       | 1.543    | 1.721       | 1.769    | 75.5     |
| json (stdlib) | mixed       | 10.150   | 10.162      | 10.975   | 75.5     |
| msgspec       | nested      | 0.387    | 0.407       | 0.431    | 63.8     |
| strata        | nested      | 0.427    | 0.467       | 0.480    | 63.8     |
| orjson        | nested      | 0.513    | 0.703       | 0.732    | 63.8     |
| ujson         | nested      | 1.239    | 1.242       | 2.037    | 63.8     |
| json (stdlib) | nested      | 12.220   | 12.430      | 12.857   | 63.8     |
| msgspec       | users.json  | 1.792    | 1.806       | 1.920    | 55.3     |
| orjson        | users.json  | 1.933    | 1.967       | 2.247    | 55.3     |
| strata        | users.json  | 2.039    | 2.456       | 2.479    | 55.3     |
| ujson         | users.json  | 6.885    | 7.318       | 7.887    | 55.3     |
| json (stdlib) | users.json  | 70.521   | 71.914      | 75.952   | 55.3     |
| orjson        | wide_arrays | 1.885    | 1.932       | 1.970    | 75.4     |
| msgspec       | wide_arrays | 2.264    | 2.462       | 3.161    | 75.5     |
| strata        | wide_arrays | 2.323    | 2.480       | 2.713    | 75.4     |
| ujson         | wide_arrays | 7.447    | 7.542       | 7.661    | 75.4     |
| json (stdlib) | wide_arrays | 45.228   | 46.012      | 46.248   | 75.5     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 2.909    | 1000    | 55.4     |
|                                | orjson+jmespath    | 6.808    | 1000    | 55.5     |
|                                | orjson+jsonpath-ng | 9.238    | 1000    | 56.1     |
| all item prices                | strata             | 3.496    | 12046   | 56.1     |
|                                | orjson+jmespath    | 14.920   | 1000    | 56.1     |
|                                | orjson+jsonpath-ng | 40.340   | 12046   | 56.0     |
| recursive price                | strata             | 4.140    | 12046   | 55.7     |
|                                | orjson+jsonpath-ng | 136.676  | 12046   | 56.5     |
| flat:$.records\[\*\].id        | strata             | 0.672    | 500     | 63.8     |
|                                | orjson+jmespath    | 0.869    | 500     | 63.8     |
|                                | orjson+jsonpath-ng | 2.922    | 500     | 63.8     |
| flat:$.records\[0\]            | strata             | 0.636    | 1       | 63.8     |
|                                | orjson+jsonpath-ng | 0.697    | 1       | 63.8     |
|                                | orjson+jmespath    | 0.712    | 1       | 63.8     |
| nested:$.records\[\*\].id      | strata             | 0.593    | 500     | 63.8     |
|                                | orjson+jmespath    | 1.931    | 500     | 63.8     |
|                                | orjson+jsonpath-ng | 2.809    | 500     | 63.8     |
| nested:$.records\[0\]          | strata             | 0.627    | 1       | 63.8     |
|                                | orjson+jmespath    | 1.660    | 1       | 63.8     |
|                                | orjson+jsonpath-ng | 1.966    | 1       | 63.8     |
| wide_arrays:$.records\[\*\].id | strata             | 4.133    | 500     | 75.5     |
|                                | orjson+jmespath    | 4.366    | 500     | 75.5     |
|                                | orjson+jsonpath-ng | 5.066    | 500     | 75.5     |
| wide_arrays:$.records\[0\]     | strata             | 3.903    | 1       | 75.5     |
|                                | orjson+jmespath    | 4.246    | 1       | 75.5     |
|                                | orjson+jsonpath-ng | 6.973    | 1       | 75.5     |
| mixed:$.records\[\*\].id       | strata             | 0.755    | 500     | 75.5     |
|                                | orjson+jmespath    | 0.793    | 500     | 75.5     |
|                                | orjson+jsonpath-ng | 2.398    | 500     | 75.5     |
| mixed:$.records\[0\]           | orjson+jsonpath-ng | 0.658    | 1       | 75.5     |
|                                | orjson+jmespath    | 0.697    | 1       | 75.5     |
|                                | strata             | 0.781    | 1       | 75.5     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.096    | 1000    | 57.4     |
|                                            | jmespath    | 0.370    | 1000    | 57.4     |
|                                            | jsonpath-ng | 3.948    | 1000    | 57.4     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 1.029    | 12046   | 57.4     |
|                                            | jmespath    | 12.509   | 1000    | 57.4     |
|                                            | jsonpath-ng | 35.704   | 12046   | 60.1     |
| $..price                                   | strata      | 1.463    | 12046   | 58.1     |
|                                            | jsonpath-ng | 154.157  | 12046   | 61.1     |
| $.users\[?(@.age>30)\]                     | strata      | 0.206    | 794     | 58.4     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.572    | 1604    | 58.5     |
| flat:$.records\[\*\].id                    | strata      | 0.037    | 500     | 63.8     |
|                                            | jmespath    | 0.191    | 500     | 63.8     |
|                                            | jsonpath-ng | 1.623    | 500     | 63.8     |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 63.8     |
|                                            | jsonpath-ng | 0.013    | 1       | 63.8     |
|                                            | jmespath    | 0.013    | 1       | 63.8     |
| nested:$.records\[\*\].id                  | strata      | 0.049    | 500     | 63.9     |
|                                            | jmespath    | 0.172    | 500     | 63.9     |
|                                            | jsonpath-ng | 1.880    | 500     | 63.9     |
| nested:$.records\[0\]                      | strata      | 0.005    | 1       | 63.9     |
|                                            | jsonpath-ng | 0.008    | 1       | 63.9     |
|                                            | jmespath    | 0.008    | 1       | 63.9     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.060    | 500     | 75.5     |
|                                            | jmespath    | 0.181    | 500     | 75.5     |
|                                            | jsonpath-ng | 2.419    | 500     | 75.5     |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 75.5     |
|                                            | jmespath    | 0.010    | 1       | 75.5     |
|                                            | jsonpath-ng | 0.010    | 1       | 75.5     |
| mixed:$.records\[\*\].id                   | strata      | 0.038    | 500     | 75.5     |
|                                            | jmespath    | 0.173    | 500     | 75.5     |
|                                            | jsonpath-ng | 2.545    | 500     | 75.5     |
| mixed:$.records\[0\]                       | strata      | 0.004    | 1       | 75.5     |
|                                            | jmespath    | 0.006    | 1       | 75.5     |
|                                            | jsonpath-ng | 0.009    | 1       | 75.5     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#2** / 5              | 9.7% behind #1 (orjson)             |
| loads (NDJSON)      | **#2** / 5              | 1.8% behind #1 (msgspec)            |
| load (JSON file)    | **#2** / 5              | 14.2% behind #1 (msgspec)           |
| load (NDJSON file)  | **#3** / 5              | 28.9% behind #1 (orjson)            |
| dumps               | **#1** / 5              | **6.2% faster** than #2 (msgspec)   |
| dump                | **#3** / 5              | 13.8% behind #1 (msgspec)           |
| loads (flat)        | **#3** / 5              | 46.9% behind #1 (orjson)            |
| load (flat)         | **#3** / 5              | 51.8% behind #1 (msgspec)           |
| dumps (flat)        | **#2** / 5              | 23.1% behind #1 (orjson)            |
| dump (flat)         | **#1** / 5              | **5.1% faster** than #2 (orjson)    |
| loads (nested)      | **#1** / 5              | **84.8% faster** than #2 (msgspec)  |
| load (nested)       | **#1** / 5              | **104.0% faster** than #2 (msgspec) |
| dumps (nested)      | **#3** / 5              | 29.9% behind #1 (msgspec)           |
| dump (nested)       | **#2** / 5              | 10.5% behind #1 (msgspec)           |
| loads (wide_arrays) | **#3** / 5              | 29.2% behind #1 (orjson)            |
| load (wide_arrays)  | **#1** / 5              | **2.2% faster** than #2 (orjson)    |
| dumps (wide_arrays) | **#3** / 5              | 29.9% behind #1 (orjson)            |
| dump (wide_arrays)  | **#3** / 5              | 23.3% behind #1 (orjson)            |
| loads (mixed)       | **#4** / 5              | 115.5% behind #1 (orjson)           |
| load (mixed)        | **#3** / 5              | 73.5% behind #1 (orjson)            |
| dumps (mixed)       | **#3** / 5              | 37.9% behind #1 (orjson)            |
| dump (mixed)        | **#2** / 5              | 22.9% behind #1 (orjson)            |
| search (JSONPath)   | #1 in 10/11 queries     | -                                   |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
