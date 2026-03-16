# Strata Benchmark Results

Generated: 2026-03-16 02:14:56

## Environment

- Commit: 3f0d7fe886713eb553cbe090544e067cb4f6daf3
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
| orjson        | flat         | 0.540    | 0.681       | 0.700    | 61.8     |
| msgspec       | flat         | 0.653    | 0.736       | 0.750    | 61.8     |
| strata        | flat         | 0.634    | 0.748       | 0.840    | 61.2     |
| ujson         | flat         | 1.132    | 1.248       | 1.370    | 61.8     |
| json (stdlib) | flat         | 2.117    | 2.186       | 2.419    | 61.8     |
| orjson        | mixed        | 0.552    | 0.703       | 0.818    | 78.7     |
| msgspec       | mixed        | 0.684    | 0.734       | 0.759    | 78.7     |
| strata        | mixed        | 0.724    | 0.831       | 0.874    | 78.7     |
| ujson         | mixed        | 1.035    | 1.242       | 1.258    | 78.7     |
| json (stdlib) | mixed        | 2.149    | 2.315       | 2.400    | 78.7     |
| strata        | nested       | 0.601    | 0.715       | 0.729    | 60.9     |
| msgspec       | nested       | 1.889    | 2.263       | 2.860    | 60.9     |
| ujson         | nested       | 2.267    | 2.845       | 5.107    | 60.9     |
| json (stdlib) | nested       | 2.809    | 3.103       | 3.193    | 60.9     |
| orjson        | nested       | 2.510    | 3.537       | 4.555    | 60.9     |
| strata        | users.json   | 3.980    | 4.689       | 4.850    | 34.3     |
| msgspec       | users.json   | 4.767    | 6.666       | 10.610   | 41.5     |
| orjson        | users.json   | 5.016    | 7.173       | 7.754    | 38.3     |
| ujson         | users.json   | 6.700    | 8.835       | 9.410    | 41.3     |
| json (stdlib) | users.json   | 8.952    | 10.682      | 10.754   | 43.4     |
| strata        | users.ndjson | 3.550    | 4.660       | 5.394    | 57.6     |
| msgspec       | users.ndjson | 4.841    | 6.514       | 7.239    | 59.6     |
| orjson        | users.ndjson | 5.325    | 8.177       | 9.126    | 58.6     |
| ujson         | users.ndjson | 6.451    | 9.446       | 9.780    | 59.6     |
| json (stdlib) | users.ndjson | 9.755    | 13.860      | 15.840   | 59.6     |
| strata        | wide_arrays  | 3.200    | 3.641       | 3.755    | 63.9     |
| orjson        | wide_arrays  | 3.868    | 4.471       | 4.827    | 66.5     |
| msgspec       | wide_arrays  | 5.268    | 5.376       | 7.866    | 67.4     |
| ujson         | wide_arrays  | 7.086    | 8.274       | 10.275   | 67.4     |
| json (stdlib) | wide_arrays  | 13.653   | 14.453      | 14.798   | 68.7     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.721    | 0.776       | 0.789    | 61.8     |
| strata        | flat         | 0.746    | 0.833       | 0.915    | 61.8     |
| msgspec       | flat         | 0.791    | 0.898       | 1.057    | 61.8     |
| ujson         | flat         | 1.399    | 1.566       | 1.572    | 61.8     |
| json (stdlib) | flat         | 2.190    | 2.324       | 2.531    | 61.8     |
| orjson        | mixed        | 0.665    | 0.925       | 1.411    | 78.7     |
| msgspec       | mixed        | 1.111    | 1.271       | 1.553    | 78.7     |
| strata        | mixed        | 1.304    | 1.317       | 2.730    | 78.7     |
| ujson         | mixed        | 1.409    | 1.471       | 1.507    | 78.7     |
| json (stdlib) | mixed        | 2.657    | 2.812       | 2.868    | 78.7     |
| strata        | nested       | 0.644    | 0.795       | 0.839    | 60.9     |
| orjson        | nested       | 1.905    | 2.125       | 2.534    | 60.9     |
| ujson         | nested       | 2.505    | 2.671       | 3.019    | 60.9     |
| msgspec       | nested       | 1.768    | 3.200       | 3.616    | 60.9     |
| json (stdlib) | nested       | 4.127    | 4.784       | 4.967    | 60.9     |
| strata        | users.json   | 3.638    | 4.641       | 4.659    | 43.5     |
| orjson        | users.json   | 5.275    | 6.628       | 6.761    | 43.5     |
| msgspec       | users.json   | 4.965    | 7.145       | 8.782    | 46.4     |
| ujson         | users.json   | 9.257    | 9.642       | 10.642   | 46.4     |
| json (stdlib) | users.json   | 11.128   | 11.230      | 11.235   | 46.4     |
| strata        | users.ndjson | 4.026    | 5.006       | 5.045    | 59.6     |
| orjson        | users.ndjson | 4.730    | 6.563       | 6.945    | 59.6     |
| msgspec       | users.ndjson | 7.043    | 7.282       | 11.815   | 60.6     |
| ujson         | users.ndjson | 7.330    | 9.041       | 10.421   | 60.6     |
| json (stdlib) | users.ndjson | 10.356   | 11.760      | 12.783   | 60.6     |
| strata        | wide_arrays  | 3.523    | 4.022       | 4.918    | 68.7     |
| orjson        | wide_arrays  | 4.983    | 5.023       | 9.415    | 70.1     |
| msgspec       | wide_arrays  | 5.294    | 6.475       | 6.990    | 71.4     |
| ujson         | wide_arrays  | 8.293    | 8.701       | 8.824    | 71.4     |
| json (stdlib) | wide_arrays  | 14.857   | 15.451      | 16.565   | 72.7     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.248    | 0.258       | 0.276    | 61.8     |
| strata  | flat        | 0.283    | 0.289       | 0.299    | 61.8     |
| msgspec | flat        | 0.309    | 0.318       | 0.366    | 61.8     |
| ujson   | flat        | 1.197    | 1.200       | 1.222    | 61.8     |
| json    | flat        | 2.285    | 2.340       | 2.391    | 61.8     |
| orjson  | mixed       | 0.299    | 0.326       | 0.329    | 78.7     |
| msgspec | mixed       | 0.351    | 0.393       | 0.472    | 78.7     |
| strata  | mixed       | 0.370    | 0.507       | 0.592    | 78.7     |
| ujson   | mixed       | 1.384    | 1.445       | 1.948    | 78.7     |
| json    | mixed       | 3.211    | 3.232       | 4.181    | 78.7     |
| orjson  | nested      | 0.194    | 0.203       | 0.210    | 60.9     |
| msgspec | nested      | 0.210    | 0.223       | 0.233    | 60.9     |
| strata  | nested      | 0.207    | 0.226       | 0.261    | 60.9     |
| ujson   | nested      | 1.067    | 1.067       | 1.143    | 60.9     |
| json    | nested      | 1.317    | 1.327       | 1.328    | 60.9     |
| strata  | users.json  | 1.305    | 1.379       | 1.507    | 47.8     |
| msgspec | users.json  | 1.463    | 1.484       | 1.540    | 51.0     |
| orjson  | users.json  | 1.511    | 1.589       | 1.857    | 50.8     |
| ujson   | users.json  | 6.172    | 6.309       | 6.817    | 53.0     |
| json    | users.json  | 8.750    | 8.945       | 9.372    | 53.5     |
| orjson  | wide_arrays | 1.517    | 1.548       | 1.691    | 75.8     |
| msgspec | wide_arrays | 1.591    | 1.700       | 1.742    | 78.3     |
| strata  | wide_arrays | 1.773    | 1.795       | 1.818    | 73.9     |
| ujson   | wide_arrays | 6.946    | 7.042       | 7.159    | 78.7     |
| json    | wide_arrays | 19.347   | 19.351      | 19.540   | 78.7     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| msgspec       | flat        | 0.602    | 0.610       | 0.906    | 61.8     |
| orjson        | flat        | 0.591    | 0.611       | 0.866    | 61.8     |
| strata        | flat        | 0.497    | 0.719       | 0.778    | 61.8     |
| ujson         | flat        | 1.525    | 1.625       | 1.868    | 61.8     |
| json (stdlib) | flat        | 8.311    | 8.935       | 9.478    | 61.8     |
| msgspec       | mixed       | 0.608    | 0.872       | 0.880    | 78.7     |
| orjson        | mixed       | 0.906    | 1.358       | 3.967    | 78.7     |
| ujson         | mixed       | 1.669    | 3.099       | 3.184    | 78.7     |
| strata        | mixed       | 0.666    | 3.627       | 10.442   | 78.7     |
| json (stdlib) | mixed       | 10.185   | 10.261      | 12.291   | 78.7     |
| strata        | nested      | 0.365    | 0.389       | 0.648    | 60.9     |
| orjson        | nested      | 0.458    | 0.486       | 0.511    | 60.9     |
| msgspec       | nested      | 0.672    | 0.768       | 0.823    | 60.9     |
| ujson         | nested      | 1.187    | 1.256       | 1.271    | 60.9     |
| json (stdlib) | nested      | 12.123   | 12.433      | 12.505   | 60.9     |
| strata        | users.json  | 1.564    | 1.581       | 1.636    | 53.5     |
| orjson        | users.json  | 1.752    | 1.896       | 1.904    | 53.5     |
| msgspec       | users.json  | 1.794    | 2.742       | 2.984    | 53.5     |
| ujson         | users.json  | 6.825    | 6.937       | 7.145    | 53.5     |
| json (stdlib) | users.json  | 67.589   | 68.101      | 68.670   | 53.5     |
| orjson        | wide_arrays | 2.187    | 2.342       | 2.397    | 78.7     |
| strata        | wide_arrays | 2.157    | 2.394       | 3.343    | 78.7     |
| msgspec       | wide_arrays | 2.131    | 2.469       | 2.797    | 78.7     |
| ujson         | wide_arrays | 7.740    | 7.842       | 8.982    | 78.7     |
| json (stdlib) | wide_arrays | 45.261   | 45.689      | 46.190   | 78.7     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.786    | 1000    | 53.5     |
|                                | orjson+jmespath    | 5.645    | 1000    | 53.6     |
|                                | orjson+jsonpath-ng | 7.026    | 1000    | 54.2     |
| all item prices                | strata             | 2.062    | 12046   | 54.2     |
|                                | orjson+jmespath    | 13.854   | 1000    | 54.2     |
|                                | orjson+jsonpath-ng | 37.166   | 12046   | 54.1     |
| recursive price                | strata             | 2.067    | 12046   | 53.5     |
|                                | orjson+jsonpath-ng | 130.138  | 12046   | 54.6     |
| flat:$.records\[\*\].id        | strata             | 0.548    | 500     | 61.8     |
|                                | orjson+jmespath    | 0.855    | 500     | 61.8     |
|                                | orjson+jsonpath-ng | 2.332    | 500     | 60.8     |
| flat:$.records\[0\]            | strata             | 0.577    | 1       | 60.8     |
|                                | orjson+jmespath    | 0.671    | 1       | 60.8     |
|                                | orjson+jsonpath-ng | 0.713    | 1       | 60.8     |
| nested:$.records\[\*\].id      | strata             | 0.399    | 500     | 60.9     |
|                                | orjson+jmespath    | 1.969    | 500     | 60.9     |
|                                | orjson+jsonpath-ng | 2.518    | 500     | 60.9     |
| nested:$.records\[0\]          | strata             | 0.404    | 1       | 60.9     |
|                                | orjson+jsonpath-ng | 1.737    | 1       | 60.9     |
|                                | orjson+jmespath    | 2.658    | 1       | 60.9     |
| wide_arrays:$.records\[\*\].id | strata             | 2.626    | 500     | 78.7     |
|                                | orjson+jmespath    | 4.082    | 500     | 78.7     |
|                                | orjson+jsonpath-ng | 5.490    | 500     | 78.7     |
| wide_arrays:$.records\[0\]     | strata             | 2.746    | 1       | 78.7     |
|                                | orjson+jsonpath-ng | 3.949    | 1       | 78.7     |
|                                | orjson+jmespath    | 3.982    | 1       | 78.7     |
| mixed:$.records\[\*\].id       | strata             | 0.509    | 500     | 78.7     |
|                                | orjson+jmespath    | 0.817    | 500     | 78.7     |
|                                | orjson+jsonpath-ng | 2.824    | 500     | 78.7     |
| mixed:$.records\[0\]           | strata             | 0.544    | 1       | 78.7     |
|                                | orjson+jmespath    | 0.624    | 1       | 78.7     |
|                                | orjson+jsonpath-ng | 0.638    | 1       | 78.7     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.147    | 1000    | 53.7     |
|                                            | jmespath    | 0.426    | 1000    | 53.7     |
|                                            | jsonpath-ng | 4.953    | 1000    | 53.7     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.942    | 12046   | 53.7     |
|                                            | jmespath    | 10.336   | 1000    | 54.0     |
|                                            | jsonpath-ng | 31.097   | 12046   | 58.2     |
| $..price                                   | strata      | 1.291    | 12046   | 57.2     |
|                                            | jsonpath-ng | 121.589  | 12046   | 59.2     |
| $.users\[?(@.age>30)\]                     | strata      | 0.176    | 794     | 56.6     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.656    | 1604    | 56.6     |
| flat:$.records\[\*\].id                    | strata      | 0.037    | 500     | 60.9     |
|                                            | jmespath    | 0.175    | 500     | 60.9     |
|                                            | jsonpath-ng | 1.870    | 500     | 60.9     |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 60.9     |
|                                            | jmespath    | 0.006    | 1       | 60.9     |
|                                            | jsonpath-ng | 0.011    | 1       | 60.9     |
| nested:$.records\[\*\].id                  | strata      | 0.041    | 500     | 60.9     |
|                                            | jmespath    | 0.172    | 500     | 60.9     |
|                                            | jsonpath-ng | 1.979    | 500     | 60.9     |
| nested:$.records\[0\]                      | strata      | 0.004    | 1       | 60.9     |
|                                            | jsonpath-ng | 0.007    | 1       | 60.9     |
|                                            | jmespath    | 0.008    | 1       | 60.9     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.031    | 500     | 78.7     |
|                                            | jmespath    | 0.185    | 500     | 78.7     |
|                                            | jsonpath-ng | 2.579    | 500     | 78.7     |
| wide_arrays:$.records\[0\]                 | jmespath    | 0.007    | 1       | 78.7     |
|                                            | strata      | 0.009    | 1       | 78.7     |
|                                            | jsonpath-ng | 0.010    | 1       | 78.7     |
| mixed:$.records\[\*\].id                   | strata      | 0.033    | 500     | 78.7     |
|                                            | jmespath    | 0.174    | 500     | 78.7     |
|                                            | jsonpath-ng | 1.643    | 500     | 78.7     |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 78.7     |
|                                            | jsonpath-ng | 0.006    | 1       | 78.7     |
|                                            | jmespath    | 0.006    | 1       | 78.7     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **19.8% faster** than #2 (msgspec)  |
| loads (NDJSON)      | **#1** / 5              | **36.4% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **36.5% faster** than #2 (msgspec)  |
| load (NDJSON file)  | **#1** / 5              | **17.5% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **12.1% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **12.0% faster** than #2 (orjson)   |
| loads (flat)        | **#2** / 5              | 17.3% behind #1 (orjson)            |
| load (flat)         | **#2** / 5              | 3.5% behind #1 (orjson)             |
| dumps (flat)        | **#2** / 5              | 14.3% behind #1 (orjson)            |
| dump (flat)         | **#1** / 5              | **18.8% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **214.3% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **174.4% faster** than #2 (msgspec) |
| dumps (nested)      | **#2** / 5              | 6.8% behind #1 (orjson)             |
| dump (nested)       | **#1** / 5              | **25.5% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#1** / 5              | **20.9% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **41.4% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#3** / 5              | 16.8% behind #1 (orjson)            |
| dump (wide_arrays)  | **#2** / 5              | 1.2% behind #1 (msgspec)            |
| loads (mixed)       | **#3** / 5              | 31.1% behind #1 (orjson)            |
| load (mixed)        | **#3** / 5              | 96.1% behind #1 (orjson)            |
| dumps (mixed)       | **#3** / 5              | 23.8% behind #1 (orjson)            |
| dump (mixed)        | **#2** / 5              | 9.6% behind #1 (msgspec)            |
| search (JSONPath)   | **#1** in 11/11 queries | avg **723% faster**                 |
| query (JSONPath)    | **#1** in 12/13 queries | -                                   |
