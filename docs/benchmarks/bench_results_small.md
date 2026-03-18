# Strata Benchmark Results

Generated: 2026-03-18 15:01:33

## Environment

- Commit: 26bebc11f29d91458f5425c35bb69bdfbc551de7
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
| orjson        | flat         | 0.539    | 0.639       | 0.643    | 59.8     |
| msgspec       | flat         | 0.608    | 0.707       | 0.713    | 59.8     |
| strata        | flat         | 0.750    | 0.835       | 0.901    | 59.2     |
| ujson         | flat         | 1.096    | 1.251       | 1.478    | 59.8     |
| json (stdlib) | flat         | 1.989    | 2.117       | 2.125    | 59.8     |
| orjson        | mixed        | 0.474    | 0.634       | 0.683    | 76.1     |
| msgspec       | mixed        | 0.586    | 0.759       | 0.787    | 76.1     |
| strata        | mixed        | 0.833    | 0.889       | 0.925    | 76.1     |
| ujson         | mixed        | 1.071    | 1.166       | 1.360    | 76.1     |
| json (stdlib) | mixed        | 2.128    | 2.248       | 2.304    | 76.1     |
| strata        | nested       | 0.645    | 0.737       | 0.769    | 59.9     |
| orjson        | nested       | 1.437    | 1.700       | 1.770    | 59.9     |
| msgspec       | nested       | 1.550    | 1.708       | 2.198    | 59.9     |
| ujson         | nested       | 2.008    | 2.112       | 2.184    | 59.9     |
| json (stdlib) | nested       | 2.399    | 2.559       | 2.580    | 59.9     |
| strata        | users.json   | 3.897    | 4.748       | 5.046    | 34.1     |
| orjson        | users.json   | 4.317    | 5.952       | 6.491    | 38.1     |
| msgspec       | users.json   | 4.631    | 6.142       | 6.462    | 41.2     |
| ujson         | users.json   | 6.017    | 8.230       | 8.666    | 41.1     |
| json (stdlib) | users.json   | 8.783    | 10.243      | 10.282   | 43.9     |
| strata        | users.ndjson | 3.972    | 4.860       | 4.928    | 55.6     |
| orjson        | users.ndjson | 4.456    | 6.002       | 6.015    | 56.6     |
| msgspec       | users.ndjson | 4.721    | 6.137       | 6.460    | 57.6     |
| ujson         | users.ndjson | 6.508    | 8.357       | 8.442    | 57.6     |
| json (stdlib) | users.ndjson | 9.120    | 10.938      | 11.090   | 57.6     |
| strata        | wide_arrays  | 3.173    | 3.816       | 3.893    | 62.3     |
| orjson        | wide_arrays  | 3.388    | 4.307       | 4.345    | 64.9     |
| msgspec       | wide_arrays  | 4.165    | 5.222       | 5.630    | 64.9     |
| ujson         | wide_arrays  | 7.227    | 7.485       | 7.533    | 64.9     |
| json (stdlib) | wide_arrays  | 13.420   | 14.142      | 14.446   | 66.2     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.629    | 0.712       | 0.722    | 59.8     |
| msgspec       | flat         | 0.718    | 0.829       | 0.917    | 59.8     |
| strata        | flat         | 0.776    | 0.869       | 0.901    | 59.8     |
| ujson         | flat         | 1.364    | 1.450       | 1.456    | 59.8     |
| json (stdlib) | flat         | 2.290    | 2.462       | 2.587    | 59.8     |
| orjson        | mixed        | 0.615    | 0.769       | 0.857    | 76.1     |
| msgspec       | mixed        | 0.702    | 0.828       | 0.845    | 76.1     |
| strata        | mixed        | 0.907    | 1.015       | 1.053    | 76.1     |
| ujson         | mixed        | 1.329    | 1.389       | 1.616    | 76.1     |
| json (stdlib) | mixed        | 2.181    | 2.280       | 2.316    | 76.1     |
| strata        | nested       | 0.655    | 0.829       | 0.856    | 59.9     |
| msgspec       | nested       | 2.064    | 2.117       | 4.324    | 59.9     |
| orjson        | nested       | 2.163    | 2.383       | 3.463    | 59.9     |
| ujson         | nested       | 2.318    | 2.483       | 2.795    | 59.9     |
| json (stdlib) | nested       | 2.963    | 2.990       | 4.270    | 59.9     |
| strata        | users.json   | 3.771    | 4.607       | 4.682    | 43.9     |
| orjson        | users.json   | 4.478    | 6.082       | 6.669    | 44.9     |
| msgspec       | users.json   | 4.703    | 6.377       | 6.834    | 49.1     |
| ujson         | users.json   | 6.402    | 8.404       | 8.807    | 49.1     |
| json (stdlib) | users.json   | 8.927    | 10.460      | 10.462   | 49.1     |
| strata        | users.ndjson | 4.096    | 4.984       | 5.274    | 57.6     |
| orjson        | users.ndjson | 4.587    | 6.109       | 6.670    | 57.6     |
| msgspec       | users.ndjson | 4.865    | 6.348       | 6.723    | 58.6     |
| ujson         | users.ndjson | 6.771    | 8.667       | 8.894    | 58.6     |
| json (stdlib) | users.ndjson | 9.237    | 10.971      | 11.026   | 58.6     |
| strata        | wide_arrays  | 3.488    | 4.041       | 4.222    | 66.2     |
| orjson        | wide_arrays  | 3.699    | 4.373       | 4.411    | 66.6     |
| msgspec       | wide_arrays  | 4.425    | 5.239       | 5.388    | 69.3     |
| ujson         | wide_arrays  | 7.552    | 7.967       | 8.086    | 69.3     |
| json (stdlib) | wide_arrays  | 13.678   | 14.678      | 16.002   | 69.3     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.236    | 0.247       | 0.253    | 59.8     |
| orjson  | flat        | 0.245    | 0.249       | 0.269    | 59.8     |
| msgspec | flat        | 0.317    | 0.320       | 0.393    | 59.9     |
| ujson   | flat        | 1.206    | 1.214       | 1.218    | 59.9     |
| json    | flat        | 2.317    | 2.398       | 2.668    | 59.9     |
| strata  | mixed       | 0.265    | 0.267       | 0.267    | 76.1     |
| orjson  | mixed       | 0.279    | 0.285       | 0.289    | 76.2     |
| msgspec | mixed       | 0.307    | 0.309       | 0.350    | 76.2     |
| ujson   | mixed       | 1.317    | 1.332       | 1.356    | 76.2     |
| json    | mixed       | 2.959    | 2.966       | 3.125    | 76.2     |
| strata  | nested      | 0.186    | 0.187       | 0.190    | 59.9     |
| orjson  | nested      | 0.212    | 0.232       | 0.243    | 60.1     |
| msgspec | nested      | 0.220    | 0.263       | 0.442    | 60.1     |
| ujson   | nested      | 0.982    | 0.987       | 1.044    | 60.1     |
| json    | nested      | 1.332    | 1.341       | 1.355    | 60.2     |
| strata  | users.json  | 1.086    | 1.096       | 1.197    | 50.0     |
| orjson  | users.json  | 1.488    | 1.490       | 1.583    | 50.8     |
| msgspec | users.json  | 1.484    | 1.502       | 1.583    | 51.0     |
| ujson   | users.json  | 6.242    | 6.246       | 6.284    | 51.0     |
| json    | users.json  | 8.833    | 8.837       | 8.967    | 51.3     |
| strata  | wide_arrays | 1.369    | 1.377       | 1.391    | 70.5     |
| msgspec | wide_arrays | 1.649    | 1.661       | 1.876    | 72.6     |
| orjson  | wide_arrays | 1.551    | 1.697       | 1.799    | 72.5     |
| ujson   | wide_arrays | 7.245    | 7.264       | 7.689    | 75.7     |
| json    | wide_arrays | 19.860   | 20.095      | 21.093   | 75.8     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.378    | 0.379       | 0.381    | 59.9     |
| orjson        | flat        | 0.426    | 0.460       | 0.496    | 59.9     |
| msgspec       | flat        | 0.545    | 0.547       | 0.578    | 59.9     |
| ujson         | flat        | 1.417    | 1.419       | 1.426    | 59.9     |
| json (stdlib) | flat        | 8.308    | 8.607       | 8.756    | 59.9     |
| strata        | mixed       | 0.386    | 0.388       | 0.404    | 76.2     |
| orjson        | mixed       | 0.441    | 0.444       | 0.447    | 76.2     |
| msgspec       | mixed       | 0.515    | 0.575       | 0.601    | 76.2     |
| ujson         | mixed       | 1.532    | 1.562       | 1.659    | 76.2     |
| json (stdlib) | mixed       | 10.161   | 10.188      | 10.349   | 76.2     |
| strata        | nested      | 0.321    | 0.343       | 1.555    | 60.2     |
| msgspec       | nested      | 0.387    | 0.406       | 0.414    | 60.2     |
| orjson        | nested      | 0.400    | 0.425       | 0.478    | 60.2     |
| ujson         | nested      | 1.155    | 1.214       | 1.492    | 60.2     |
| json (stdlib) | nested      | 12.075   | 13.788      | 15.724   | 60.2     |
| strata        | users.json  | 1.287    | 1.293       | 1.296    | 51.3     |
| msgspec       | users.json  | 1.730    | 1.790       | 1.839    | 51.3     |
| orjson        | users.json  | 1.789    | 1.797       | 1.988    | 51.3     |
| ujson         | users.json  | 6.742    | 6.750       | 6.785    | 51.3     |
| json (stdlib) | users.json  | 66.682   | 67.262      | 67.894   | 51.3     |
| strata        | wide_arrays | 1.574    | 1.614       | 1.695    | 75.8     |
| orjson        | wide_arrays | 1.891    | 1.958       | 1.964    | 75.8     |
| msgspec       | wide_arrays | 1.930    | 2.000       | 2.052    | 75.8     |
| ujson         | wide_arrays | 7.573    | 7.597       | 7.647    | 75.8     |
| json (stdlib) | wide_arrays | 44.453   | 44.640      | 44.720   | 75.8     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.883    | 1000    | 51.5     |
|                                | orjson+jmespath    | 5.335    | 1000    | 51.6     |
|                                | orjson+jsonpath-ng | 6.547    | 1000    | 52.0     |
| all item prices                | strata             | 2.038    | 12046   | 52.0     |
|                                | orjson+jmespath    | 13.424   | 1000    | 52.1     |
|                                | orjson+jsonpath-ng | 34.577   | 12046   | 51.9     |
| recursive price                | strata             | 2.171    | 12046   | 51.4     |
|                                | orjson+jsonpath-ng | 124.047  | 12046   | 52.6     |
| flat:$.records\[\*\].id        | strata             | 0.517    | 500     | 59.9     |
|                                | orjson+jmespath    | 0.814    | 500     | 59.9     |
|                                | orjson+jsonpath-ng | 2.270    | 500     | 59.9     |
| flat:$.records\[0\]            | strata             | 0.496    | 1       | 59.9     |
|                                | orjson+jmespath    | 0.632    | 1       | 59.9     |
|                                | orjson+jsonpath-ng | 0.651    | 1       | 59.9     |
| nested:$.records\[\*\].id      | strata             | 0.386    | 500     | 60.2     |
|                                | orjson+jmespath    | 2.035    | 500     | 60.2     |
|                                | orjson+jsonpath-ng | 2.592    | 500     | 60.2     |
| nested:$.records\[0\]          | strata             | 0.374    | 1       | 60.2     |
|                                | orjson+jmespath    | 1.769    | 1       | 60.2     |
|                                | orjson+jsonpath-ng | 1.791    | 1       | 60.2     |
| wide_arrays:$.records\[\*\].id | strata             | 2.691    | 500     | 75.8     |
|                                | orjson+jmespath    | 4.096    | 500     | 75.8     |
|                                | orjson+jsonpath-ng | 4.717    | 500     | 75.8     |
| wide_arrays:$.records\[0\]     | strata             | 2.632    | 1       | 75.8     |
|                                | orjson+jmespath    | 3.930    | 1       | 75.8     |
|                                | orjson+jsonpath-ng | 3.948    | 1       | 75.8     |
| mixed:$.records\[\*\].id       | strata             | 0.515    | 500     | 76.2     |
|                                | orjson+jmespath    | 0.810    | 500     | 76.2     |
|                                | orjson+jsonpath-ng | 2.287    | 500     | 76.2     |
| mixed:$.records\[0\]           | strata             | 0.527    | 1       | 76.2     |
|                                | orjson+jsonpath-ng | 0.621    | 1       | 76.2     |
|                                | orjson+jmespath    | 0.626    | 1       | 76.2     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.066    | 1000    | 52.6     |
|                                            | jmespath    | 0.375    | 1000    | 52.6     |
|                                            | jsonpath-ng | 3.786    | 1000    | 52.6     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.880    | 12046   | 52.6     |
|                                            | jmespath    | 10.417   | 1000    | 52.9     |
|                                            | jsonpath-ng | 31.113   | 12046   | 57.1     |
| $..price                                   | strata      | 1.329    | 12046   | 56.2     |
|                                            | jsonpath-ng | 122.338  | 12046   | 58.2     |
| $.users\[?(@.age>30)\]                     | strata      | 0.116    | 794     | 55.5     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.507    | 1604    | 55.5     |
| flat:$.records\[\*\].id                    | strata      | 0.033    | 500     | 59.9     |
|                                            | jmespath    | 0.198    | 500     | 59.9     |
|                                            | jsonpath-ng | 1.574    | 500     | 59.9     |
| flat:$.records\[0\]                        | jmespath    | 0.005    | 1       | 59.9     |
|                                            | strata      | 0.007    | 1       | 59.9     |
|                                            | jsonpath-ng | 0.014    | 1       | 59.9     |
| nested:$.records\[\*\].id                  | strata      | 0.038    | 500     | 60.2     |
|                                            | jmespath    | 0.197    | 500     | 60.2     |
|                                            | jsonpath-ng | 1.993    | 500     | 60.2     |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 60.2     |
|                                            | jmespath    | 0.006    | 1       | 60.2     |
|                                            | jsonpath-ng | 0.008    | 1       | 60.2     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.028    | 500     | 75.8     |
|                                            | jmespath    | 0.196    | 500     | 75.8     |
|                                            | jsonpath-ng | 2.279    | 500     | 75.8     |
| wide_arrays:$.records\[0\]                 | strata      | 0.003    | 1       | 75.8     |
|                                            | jmespath    | 0.008    | 1       | 75.8     |
|                                            | jsonpath-ng | 0.008    | 1       | 75.8     |
| mixed:$.records\[\*\].id                   | strata      | 0.029    | 500     | 76.2     |
|                                            | jmespath    | 0.174    | 500     | 76.2     |
|                                            | jsonpath-ng | 1.766    | 500     | 76.2     |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 76.2     |
|                                            | jmespath    | 0.005    | 1       | 76.2     |
|                                            | jsonpath-ng | 0.007    | 1       | 76.2     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **10.8% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **12.2% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **18.7% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **12.0% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **36.5% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **34.5% faster** than #2 (msgspec)  |
| loads (flat)        | **#3** / 5              | 39.3% behind #1 (orjson)            |
| load (flat)         | **#3** / 5              | 23.4% behind #1 (orjson)            |
| dumps (flat)        | **#1** / 5              | **3.7% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **12.5% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **122.7% faster** than #2 (orjson)  |
| load (nested)       | **#1** / 5              | **215.3% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **13.5% faster** than #2 (orjson)   |
| dump (nested)       | **#1** / 5              | **20.8% faster** than #2 (msgspec)  |
| loads (wide_arrays) | **#1** / 5              | **6.8% faster** than #2 (orjson)    |
| load (wide_arrays)  | **#1** / 5              | **6.0% faster** than #2 (orjson)    |
| dumps (wide_arrays) | **#1** / 5              | **13.3% faster** than #2 (orjson)   |
| dump (wide_arrays)  | **#1** / 5              | **20.1% faster** than #2 (orjson)   |
| loads (mixed)       | **#3** / 5              | 75.7% behind #1 (orjson)            |
| load (mixed)        | **#3** / 5              | 47.5% behind #1 (orjson)            |
| dumps (mixed)       | **#1** / 5              | **5.4% faster** than #2 (orjson)    |
| dump (mixed)        | **#1** / 5              | **14.3% faster** than #2 (orjson)   |
| search (JSONPath)   | **#1** in 11/11 queries | avg **674% faster**                 |
| query (JSONPath)    | **#1** in 12/13 queries | -                                   |
