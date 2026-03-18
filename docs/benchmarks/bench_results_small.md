# Strata Benchmark Results

Generated: 2026-03-18 23:12:40

## Environment

- Commit: fe648ede876d126597669efe7e2ea296a394b703
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
| orjson        | flat         | 0.566    | 0.655       | 0.666    | 61.5     |
| strata        | flat         | 0.642    | 0.714       | 0.800    | 60.8     |
| msgspec       | flat         | 0.618    | 0.726       | 0.727    | 61.5     |
| ujson         | flat         | 1.058    | 1.212       | 1.219    | 61.5     |
| json (stdlib) | flat         | 2.033    | 2.096       | 2.107    | 61.5     |
| orjson        | mixed        | 0.554    | 0.677       | 0.681    | 76.5     |
| msgspec       | mixed        | 0.573    | 0.718       | 0.734    | 76.5     |
| strata        | mixed        | 0.648    | 0.787       | 0.902    | 76.5     |
| ujson         | mixed        | 0.977    | 1.145       | 1.190    | 76.5     |
| json (stdlib) | mixed        | 2.086    | 2.223       | 2.242    | 76.5     |
| strata        | nested       | 0.577    | 0.692       | 0.726    | 60.8     |
| msgspec       | nested       | 2.046    | 2.451       | 2.513    | 60.8     |
| json (stdlib) | nested       | 2.442    | 2.665       | 2.725    | 60.8     |
| ujson         | nested       | 2.470    | 3.699       | 6.892    | 60.8     |
| orjson        | nested       | 2.633    | 5.082       | 5.875    | 60.8     |
| strata        | users.json   | 4.005    | 5.487       | 5.645    | 34.6     |
| msgspec       | users.json   | 5.430    | 6.796       | 8.233    | 41.7     |
| orjson        | users.json   | 4.607    | 6.974       | 7.274    | 38.5     |
| ujson         | users.json   | 6.137    | 9.628       | 12.374   | 41.6     |
| json (stdlib) | users.json   | 8.916    | 10.767      | 13.848   | 44.4     |
| strata        | users.ndjson | 3.893    | 4.892       | 5.153    | 57.1     |
| msgspec       | users.ndjson | 4.887    | 6.822       | 6.960    | 59.1     |
| orjson        | users.ndjson | 5.102    | 7.677       | 9.373    | 58.1     |
| ujson         | users.ndjson | 6.785    | 9.550       | 9.584    | 59.1     |
| json (stdlib) | users.ndjson | 9.957    | 11.354      | 11.891   | 59.1     |
| strata        | wide_arrays  | 2.544    | 3.170       | 3.249    | 63.1     |
| orjson        | wide_arrays  | 3.782    | 4.313       | 4.420    | 65.7     |
| msgspec       | wide_arrays  | 4.391    | 5.096       | 5.226    | 65.7     |
| ujson         | wide_arrays  | 7.645    | 7.811       | 7.901    | 65.7     |
| json (stdlib) | wide_arrays  | 15.001   | 15.216      | 15.577   | 67.0     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| msgspec       | flat         | 0.822    | 0.847       | 0.905    | 61.5     |
| orjson        | flat         | 0.840    | 0.946       | 1.085    | 61.5     |
| strata        | flat         | 0.692    | 1.045       | 1.052    | 61.5     |
| ujson         | flat         | 1.342    | 1.433       | 1.622    | 61.5     |
| json (stdlib) | flat         | 2.300    | 2.431       | 2.566    | 61.5     |
| orjson        | mixed        | 0.597    | 0.734       | 0.819    | 76.5     |
| strata        | mixed        | 0.738    | 0.962       | 1.111    | 76.5     |
| msgspec       | mixed        | 1.007    | 1.037       | 1.109    | 76.5     |
| ujson         | mixed        | 1.120    | 1.307       | 1.347    | 76.5     |
| json (stdlib) | mixed        | 2.417    | 2.443       | 2.464    | 76.5     |
| strata        | nested       | 0.641    | 0.748       | 0.754    | 60.8     |
| msgspec       | nested       | 1.605    | 1.867       | 1.878    | 60.8     |
| orjson        | nested       | 1.720    | 1.888       | 1.962    | 60.8     |
| ujson         | nested       | 1.901    | 2.209       | 2.245    | 60.8     |
| json (stdlib) | nested       | 2.494    | 2.698       | 2.707    | 60.8     |
| strata        | users.json   | 3.990    | 4.611       | 5.145    | 44.5     |
| orjson        | users.json   | 5.435    | 6.058       | 6.975    | 44.5     |
| msgspec       | users.json   | 5.070    | 6.307       | 6.955    | 48.5     |
| ujson         | users.json   | 6.390    | 9.107       | 9.169    | 48.5     |
| json (stdlib) | users.json   | 9.429    | 14.982      | 15.070   | 48.5     |
| strata        | users.ndjson | 4.241    | 5.155       | 5.714    | 59.1     |
| orjson        | users.ndjson | 6.062    | 6.322       | 6.325    | 59.1     |
| msgspec       | users.ndjson | 5.086    | 6.615       | 6.804    | 60.2     |
| ujson         | users.ndjson | 7.543    | 9.125       | 9.309    | 60.2     |
| json (stdlib) | users.ndjson | 10.384   | 13.859      | 15.140   | 60.2     |
| strata        | wide_arrays  | 2.871    | 3.457       | 3.502    | 67.0     |
| orjson        | wide_arrays  | 4.204    | 4.523       | 4.639    | 68.3     |
| msgspec       | wide_arrays  | 4.708    | 6.964       | 8.464    | 71.0     |
| ujson         | wide_arrays  | 8.043    | 8.058       | 8.679    | 71.0     |
| json (stdlib) | wide_arrays  | 14.581   | 14.956      | 15.002   | 71.0     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.257    | 0.282       | 0.519    | 61.5     |
| msgspec | flat        | 0.302    | 0.305       | 0.319    | 61.5     |
| orjson  | flat        | 0.250    | 0.358       | 0.361    | 61.5     |
| ujson   | flat        | 1.247    | 1.251       | 1.315    | 61.5     |
| json    | flat        | 2.277    | 2.316       | 2.319    | 61.5     |
| orjson  | mixed       | 0.284    | 0.287       | 0.312    | 76.5     |
| strata  | mixed       | 0.289    | 0.302       | 0.303    | 76.5     |
| msgspec | mixed       | 0.312    | 0.326       | 0.328    | 76.5     |
| ujson   | mixed       | 1.329    | 1.428       | 1.491    | 76.5     |
| json    | mixed       | 3.003    | 3.046       | 3.088    | 76.5     |
| strata  | nested      | 0.192    | 0.193       | 0.207    | 60.8     |
| orjson  | nested      | 0.206    | 0.208       | 0.231    | 60.8     |
| msgspec | nested      | 0.219    | 0.222       | 0.222    | 60.8     |
| ujson   | nested      | 1.011    | 1.016       | 1.037    | 60.9     |
| json    | nested      | 1.357    | 1.370       | 2.378    | 60.9     |
| strata  | users.json  | 1.110    | 1.117       | 1.123    | 48.5     |
| msgspec | users.json  | 1.515    | 1.521       | 1.620    | 51.8     |
| orjson  | users.json  | 1.588    | 1.595       | 1.620    | 50.9     |
| ujson   | users.json  | 6.376    | 6.409       | 6.486    | 52.7     |
| json    | users.json  | 8.661    | 8.741       | 8.949    | 54.0     |
| strata  | wide_arrays | 1.444    | 1.451       | 1.519    | 72.7     |
| orjson  | wide_arrays | 1.555    | 1.589       | 1.754    | 74.5     |
| msgspec | wide_arrays | 1.777    | 1.787       | 1.856    | 75.2     |
| ujson   | wide_arrays | 7.292    | 7.315       | 7.377    | 75.2     |
| json    | wide_arrays | 20.037   | 20.377      | 20.447   | 76.5     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.394    | 0.412       | 0.441    | 61.5     |
| orjson        | flat        | 0.424    | 0.454       | 0.496    | 61.5     |
| msgspec       | flat        | 0.522    | 0.554       | 0.563    | 61.8     |
| ujson         | flat        | 1.468    | 1.477       | 1.496    | 61.5     |
| json (stdlib) | flat        | 8.334    | 8.537       | 8.600    | 61.8     |
| strata        | mixed       | 0.420    | 0.428       | 0.467    | 76.5     |
| orjson        | mixed       | 0.449    | 0.469       | 0.526    | 76.5     |
| msgspec       | mixed       | 0.466    | 0.478       | 0.535    | 76.5     |
| ujson         | mixed       | 1.501    | 1.513       | 1.599    | 76.5     |
| json (stdlib) | mixed       | 10.186   | 10.225      | 10.344   | 76.5     |
| strata        | nested      | 0.471    | 0.511       | 0.659    | 60.9     |
| msgspec       | nested      | 0.486    | 0.549       | 0.693    | 60.9     |
| orjson        | nested      | 0.484    | 0.590       | 0.695    | 60.9     |
| ujson         | nested      | 1.163    | 1.178       | 1.277    | 60.9     |
| json (stdlib) | nested      | 12.013   | 12.050      | 12.110   | 60.9     |
| strata        | users.json  | 1.412    | 1.436       | 1.564    | 54.0     |
| orjson        | users.json  | 1.735    | 1.778       | 1.881    | 54.0     |
| msgspec       | users.json  | 1.776    | 1.961       | 1.977    | 54.0     |
| ujson         | users.json  | 6.818    | 7.208       | 7.479    | 54.0     |
| json (stdlib) | users.json  | 66.605   | 69.045      | 69.050   | 54.0     |
| strata        | wide_arrays | 1.808    | 1.894       | 2.080    | 76.5     |
| orjson        | wide_arrays | 1.921    | 2.002       | 2.123    | 76.5     |
| msgspec       | wide_arrays | 2.172    | 2.259       | 2.339    | 76.5     |
| ujson         | wide_arrays | 7.898    | 8.023       | 8.385    | 76.5     |
| json (stdlib) | wide_arrays | 44.837   | 45.199      | 45.384   | 76.5     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 1.974    | 1000    | 54.1     |
|                                | orjson+jmespath    | 5.566    | 1000    | 54.2     |
|                                | orjson+jsonpath-ng | 6.671    | 1000    | 54.7     |
| all item prices                | strata             | 2.392    | 12046   | 54.8     |
|                                | orjson+jmespath    | 13.704   | 1000    | 54.8     |
|                                | orjson+jsonpath-ng | 36.112   | 12046   | 54.7     |
| recursive price                | strata             | 2.057    | 12046   | 54.3     |
|                                | orjson+jsonpath-ng | 127.711  | 12046   | 55.2     |
| flat:$.records\[\*\].id        | strata             | 0.528    | 500     | 61.8     |
|                                | orjson+jmespath    | 0.888    | 500     | 61.8     |
|                                | orjson+jsonpath-ng | 2.648    | 500     | 60.8     |
| flat:$.records\[0\]            | strata             | 0.481    | 1       | 60.8     |
|                                | orjson+jmespath    | 0.664    | 1       | 60.8     |
|                                | orjson+jsonpath-ng | 0.730    | 1       | 60.8     |
| nested:$.records\[\*\].id      | strata             | 0.432    | 500     | 60.9     |
|                                | orjson+jmespath    | 1.894    | 500     | 60.9     |
|                                | orjson+jsonpath-ng | 2.472    | 500     | 60.9     |
| nested:$.records\[0\]          | strata             | 0.446    | 1       | 60.9     |
|                                | orjson+jmespath    | 1.717    | 1       | 60.9     |
|                                | orjson+jsonpath-ng | 2.001    | 1       | 60.9     |
| wide_arrays:$.records\[\*\].id | strata             | 2.140    | 500     | 76.5     |
|                                | orjson+jmespath    | 4.071    | 500     | 76.5     |
|                                | orjson+jsonpath-ng | 4.830    | 500     | 76.5     |
| wide_arrays:$.records\[0\]     | strata             | 2.512    | 1       | 76.5     |
|                                | orjson+jsonpath-ng | 3.991    | 1       | 76.5     |
|                                | orjson+jmespath    | 4.531    | 1       | 76.5     |
| mixed:$.records\[\*\].id       | strata             | 0.493    | 500     | 76.5     |
|                                | orjson+jmespath    | 0.821    | 500     | 76.5     |
|                                | orjson+jsonpath-ng | 2.428    | 500     | 76.5     |
| mixed:$.records\[0\]           | strata             | 0.485    | 1       | 76.5     |
|                                | orjson+jsonpath-ng | 0.614    | 1       | 76.5     |
|                                | orjson+jmespath    | 0.668    | 1       | 76.5     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.079    | 1000    | 55.0     |
|                                            | jmespath    | 0.370    | 1000    | 55.0     |
|                                            | jsonpath-ng | 3.819    | 1000    | 55.0     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.941    | 12046   | 55.0     |
|                                            | jmespath    | 10.851   | 1000    | 55.0     |
|                                            | jsonpath-ng | 32.237   | 12046   | 57.7     |
| $..price                                   | strata      | 1.289    | 12046   | 56.7     |
|                                            | jsonpath-ng | 126.106  | 12046   | 58.7     |
| $.users\[?(@.age>30)\]                     | strata      | 0.110    | 794     | 57.1     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.498    | 1604    | 57.1     |
| flat:$.records\[\*\].id                    | strata      | 0.035    | 500     | 60.8     |
|                                            | jmespath    | 0.190    | 500     | 60.8     |
|                                            | jsonpath-ng | 1.707    | 500     | 60.8     |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 60.8     |
|                                            | jmespath    | 0.006    | 1       | 60.8     |
|                                            | jsonpath-ng | 0.014    | 1       | 60.8     |
| nested:$.records\[\*\].id                  | strata      | 0.028    | 500     | 60.9     |
|                                            | jmespath    | 0.180    | 500     | 60.9     |
|                                            | jsonpath-ng | 1.884    | 500     | 60.9     |
| nested:$.records\[0\]                      | strata      | 0.002    | 1       | 60.9     |
|                                            | jmespath    | 0.006    | 1       | 60.9     |
|                                            | jsonpath-ng | 0.006    | 1       | 60.9     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.031    | 500     | 76.5     |
|                                            | jmespath    | 0.182    | 500     | 76.5     |
|                                            | jsonpath-ng | 2.262    | 500     | 76.5     |
| wide_arrays:$.records\[0\]                 | strata      | 0.003    | 1       | 76.5     |
|                                            | jmespath    | 0.008    | 1       | 76.5     |
|                                            | jsonpath-ng | 0.011    | 1       | 76.5     |
| mixed:$.records\[\*\].id                   | strata      | 0.031    | 500     | 76.5     |
|                                            | jmespath    | 0.184    | 500     | 76.5     |
|                                            | jsonpath-ng | 2.334    | 500     | 76.5     |
| mixed:$.records\[0\]                       | jmespath    | 0.007    | 1       | 76.5     |
|                                            | strata      | 0.017    | 1       | 76.5     |
|                                            | jsonpath-ng | 0.021    | 1       | 76.5     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **15.0% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **25.5% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **27.1% faster** than #2 (msgspec)  |
| load (NDJSON file)  | **#1** / 5              | **19.9% faster** than #2 (msgspec)  |
| dumps               | **#1** / 5              | **36.5% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **22.8% faster** than #2 (orjson)   |
| loads (flat)        | **#3** / 5              | 13.5% behind #1 (orjson)            |
| load (flat)         | **#1** / 5              | **18.9% faster** than #2 (msgspec)  |
| dumps (flat)        | **#2** / 5              | 2.9% behind #1 (orjson)             |
| dump (flat)         | **#1** / 5              | **7.5% faster** than #2 (orjson)    |
| loads (nested)      | **#1** / 5              | **254.8% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **150.4% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **7.5% faster** than #2 (orjson)    |
| dump (nested)       | **#1** / 5              | **2.7% faster** than #2 (orjson)    |
| loads (wide_arrays) | **#1** / 5              | **48.6% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **46.4% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **7.7% faster** than #2 (orjson)    |
| dump (wide_arrays)  | **#1** / 5              | **6.2% faster** than #2 (orjson)    |
| loads (mixed)       | **#3** / 5              | 17.0% behind #1 (orjson)            |
| load (mixed)        | **#2** / 5              | 23.7% behind #1 (orjson)            |
| dumps (mixed)       | **#2** / 5              | 1.7% behind #1 (orjson)             |
| dump (mixed)        | **#1** / 5              | **7.1% faster** than #2 (orjson)    |
| search (JSONPath)   | **#1** in 11/11 queries | avg **703% faster**                 |
| query (JSONPath)    | **#1** in 12/13 queries | -                                   |
