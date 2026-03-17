# Strata Benchmark Results

Generated: 2026-03-17 22:33:51

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
| orjson        | flat         | 0.610    | 0.670       | 0.744    | 60.6     |
| msgspec       | flat         | 0.589    | 0.697       | 0.762    | 60.6     |
| strata        | flat         | 0.665    | 0.717       | 0.767    | 59.9     |
| ujson         | flat         | 1.050    | 1.219       | 1.256    | 60.6     |
| json (stdlib) | flat         | 2.045    | 2.126       | 2.149    | 60.6     |
| orjson        | mixed        | 0.504    | 0.620       | 0.626    | 74.0     |
| strata        | mixed        | 0.671    | 0.782       | 0.914    | 74.0     |
| msgspec       | mixed        | 0.640    | 0.854       | 0.868    | 74.0     |
| ujson         | mixed        | 1.140    | 1.355       | 1.393    | 74.0     |
| json (stdlib) | mixed        | 2.108    | 2.209       | 2.334    | 74.2     |
| strata        | nested       | 0.562    | 0.771       | 0.848    | 60.6     |
| msgspec       | nested       | 1.472    | 1.698       | 2.121    | 60.6     |
| orjson        | nested       | 1.903    | 1.984       | 2.311    | 60.6     |
| ujson         | nested       | 2.332    | 2.819       | 3.797    | 60.6     |
| json (stdlib) | nested       | 2.304    | 2.959       | 3.451    | 60.6     |
| strata        | users.json   | 3.861    | 4.656       | 5.174    | 34.5     |
| orjson        | users.json   | 6.362    | 6.596       | 10.867   | 39.4     |
| msgspec       | users.json   | 4.875    | 6.644       | 6.820    | 41.6     |
| ujson         | users.json   | 6.066    | 8.270       | 9.422    | 41.5     |
| json (stdlib) | users.json   | 9.666    | 12.301      | 12.382   | 44.5     |
| strata        | users.ndjson | 4.047    | 4.694       | 6.150    | 55.2     |
| orjson        | users.ndjson | 4.571    | 6.615       | 11.769   | 56.2     |
| msgspec       | users.ndjson | 5.038    | 8.132       | 9.949    | 57.3     |
| json (stdlib) | users.ndjson | 9.664    | 11.226      | 11.723   | 57.3     |
| ujson         | users.ndjson | 8.413    | 11.584      | 16.471   | 57.3     |
| strata        | wide_arrays  | 3.821    | 3.937       | 4.094    | 62.4     |
| orjson        | wide_arrays  | 4.346    | 4.745       | 6.453    | 65.0     |
| msgspec       | wide_arrays  | 5.311    | 5.315       | 5.400    | 65.0     |
| ujson         | wide_arrays  | 7.177    | 10.873      | 16.598   | 65.0     |
| json (stdlib) | wide_arrays  | 13.675   | 14.839      | 16.789   | 65.7     |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 0.644    | 0.766       | 0.783    | 60.6     |
| strata        | flat         | 0.684    | 0.823       | 0.866    | 60.6     |
| msgspec       | flat         | 0.940    | 0.991       | 1.084    | 60.6     |
| ujson         | flat         | 1.362    | 1.393       | 1.419    | 60.6     |
| json (stdlib) | flat         | 2.268    | 2.316       | 2.412    | 60.6     |
| orjson        | mixed        | 0.811    | 0.823       | 0.887    | 74.2     |
| msgspec       | mixed        | 0.715    | 0.890       | 1.470    | 74.2     |
| strata        | mixed        | 0.808    | 0.978       | 1.203    | 74.2     |
| ujson         | mixed        | 1.214    | 1.470       | 1.474    | 74.2     |
| json (stdlib) | mixed        | 2.408    | 3.011       | 4.073    | 74.2     |
| strata        | nested       | 0.687    | 0.843       | 0.924    | 60.6     |
| msgspec       | nested       | 1.849    | 1.965       | 3.132    | 60.6     |
| orjson        | nested       | 1.884    | 2.020       | 2.494    | 60.6     |
| json (stdlib) | nested       | 2.505    | 2.818       | 3.142    | 60.6     |
| ujson         | nested       | 3.373    | 3.884       | 4.532    | 60.6     |
| strata        | users.json   | 3.587    | 4.738       | 5.046    | 44.6     |
| orjson        | users.json   | 4.770    | 6.273       | 7.949    | 44.6     |
| msgspec       | users.json   | 7.155    | 7.635       | 8.438    | 49.6     |
| ujson         | users.json   | 7.030    | 9.382       | 9.737    | 49.6     |
| json (stdlib) | users.json   | 10.119   | 13.247      | 14.960   | 49.6     |
| strata        | users.ndjson | 3.985    | 4.862       | 5.586    | 57.3     |
| msgspec       | users.ndjson | 4.950    | 6.437       | 6.748    | 59.3     |
| orjson        | users.ndjson | 4.913    | 7.260       | 9.649    | 57.3     |
| ujson         | users.ndjson | 7.272    | 9.667       | 10.047   | 59.3     |
| json (stdlib) | users.ndjson | 9.357    | 12.233      | 15.294   | 59.3     |
| strata        | wide_arrays  | 2.972    | 3.726       | 4.546    | 65.7     |
| msgspec       | wide_arrays  | 4.700    | 5.509       | 8.665    | 69.7     |
| orjson        | wide_arrays  | 5.368    | 6.283       | 7.417    | 67.1     |
| ujson         | wide_arrays  | 7.258    | 8.138       | 9.459    | 69.7     |
| json (stdlib) | wide_arrays  | 14.163   | 15.884      | 17.330   | 69.7     |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.243    | 0.251       | 0.264    | 60.6     |
| orjson  | flat        | 0.250    | 0.298       | 0.303    | 60.6     |
| msgspec | flat        | 0.304    | 0.305       | 0.315    | 60.6     |
| ujson   | flat        | 1.207    | 1.218       | 1.222    | 60.6     |
| json    | flat        | 2.319    | 2.369       | 2.664    | 60.6     |
| strata  | mixed       | 0.287    | 0.294       | 0.301    | 74.2     |
| msgspec | mixed       | 0.318    | 0.393       | 0.395    | 74.5     |
| orjson  | mixed       | 0.311    | 0.477       | 0.562    | 74.3     |
| ujson   | mixed       | 1.331    | 1.434       | 1.633    | 74.5     |
| json    | mixed       | 3.038    | 3.143       | 3.659    | 74.5     |
| strata  | nested      | 0.190    | 0.191       | 0.191    | 60.6     |
| orjson  | nested      | 0.198    | 0.209       | 0.216    | 60.6     |
| msgspec | nested      | 0.222    | 0.228       | 0.302    | 60.6     |
| ujson   | nested      | 0.989    | 0.997       | 1.077    | 60.6     |
| json    | nested      | 1.362    | 1.366       | 1.392    | 60.6     |
| strata  | users.json  | 1.108    | 1.120       | 1.194    | 49.6     |
| orjson  | users.json  | 1.481    | 1.496       | 1.705    | 50.5     |
| msgspec | users.json  | 1.486    | 1.510       | 1.677    | 51.6     |
| ujson   | users.json  | 6.633    | 6.689       | 7.411    | 51.6     |
| json    | users.json  | 9.211    | 9.226       | 9.959    | 51.9     |
| strata  | wide_arrays | 1.443    | 1.555       | 1.690    | 70.9     |
| orjson  | wide_arrays | 1.558    | 1.566       | 1.680    | 72.8     |
| msgspec | wide_arrays | 1.668    | 1.703       | 1.729    | 73.5     |
| ujson   | wide_arrays | 7.099    | 7.258       | 7.338    | 73.9     |
| json    | wide_arrays | 20.009   | 20.739      | 24.211   | 74.0     |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 0.418    | 0.430       | 0.493    | 60.6     |
| orjson        | flat        | 0.565    | 0.567       | 0.591    | 60.6     |
| msgspec       | flat        | 0.558    | 0.590       | 0.697    | 60.6     |
| ujson         | flat        | 1.409    | 1.421       | 1.474    | 60.6     |
| json (stdlib) | flat        | 8.536    | 8.728       | 8.863    | 60.6     |
| msgspec       | mixed       | 0.565    | 0.577       | 0.655    | 74.5     |
| orjson        | mixed       | 0.525    | 0.616       | 0.617    | 74.5     |
| strata        | mixed       | 0.640    | 0.731       | 2.290    | 74.5     |
| ujson         | mixed       | 1.699    | 2.687       | 5.319    | 74.5     |
| json (stdlib) | mixed       | 10.527   | 11.652      | 13.629   | 74.5     |
| strata        | nested      | 0.422    | 0.455       | 0.618    | 60.6     |
| orjson        | nested      | 0.442    | 0.492       | 0.652    | 60.6     |
| msgspec       | nested      | 0.504    | 0.638       | 0.717    | 60.6     |
| ujson         | nested      | 1.214    | 1.232       | 1.381    | 60.6     |
| json (stdlib) | nested      | 12.775   | 12.894      | 13.114   | 60.6     |
| strata        | users.json  | 1.372    | 1.436       | 1.694    | 51.9     |
| msgspec       | users.json  | 2.206    | 2.278       | 2.445    | 51.9     |
| orjson        | users.json  | 1.800    | 2.354       | 2.506    | 51.9     |
| ujson         | users.json  | 6.906    | 7.110       | 7.581    | 51.9     |
| json (stdlib) | users.json  | 70.115   | 72.236      | 75.917   | 52.1     |
| strata        | wide_arrays | 1.940    | 2.333       | 3.449    | 74.0     |
| msgspec       | wide_arrays | 2.346    | 2.434       | 2.848    | 74.0     |
| orjson        | wide_arrays | 2.186    | 2.435       | 3.336    | 74.0     |
| ujson         | wide_arrays | 8.429    | 8.885       | 10.275   | 74.0     |
| json (stdlib) | wide_arrays | 46.140   | 49.429      | 50.235   | 74.0     |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 2.255    | 1000    | 52.2     |
|                                | orjson+jmespath    | 5.742    | 1000    | 52.3     |
|                                | orjson+jsonpath-ng | 7.245    | 1000    | 52.8     |
| all item prices                | strata             | 2.158    | 12046   | 52.9     |
|                                | orjson+jmespath    | 13.935   | 1000    | 52.9     |
|                                | orjson+jsonpath-ng | 37.971   | 12046   | 52.8     |
| recursive price                | strata             | 2.115    | 12046   | 52.4     |
|                                | orjson+jsonpath-ng | 133.510  | 12046   | 53.2     |
| flat:$.records\[\*\].id        | strata             | 0.578    | 500     | 60.6     |
|                                | orjson+jmespath    | 0.850    | 500     | 60.6     |
|                                | orjson+jsonpath-ng | 2.296    | 500     | 60.6     |
| flat:$.records\[0\]            | strata             | 0.479    | 1       | 60.6     |
|                                | orjson+jsonpath-ng | 0.656    | 1       | 60.6     |
|                                | orjson+jmespath    | 0.659    | 1       | 60.6     |
| nested:$.records\[\*\].id      | strata             | 0.442    | 500     | 60.6     |
|                                | orjson+jmespath    | 1.873    | 500     | 60.6     |
|                                | orjson+jsonpath-ng | 2.752    | 500     | 60.6     |
| nested:$.records\[0\]          | strata             | 0.402    | 1       | 60.6     |
|                                | orjson+jsonpath-ng | 1.651    | 1       | 60.7     |
|                                | orjson+jmespath    | 1.932    | 1       | 60.7     |
| wide_arrays:$.records\[\*\].id | strata             | 2.511    | 500     | 74.0     |
|                                | orjson+jmespath    | 4.136    | 500     | 74.0     |
|                                | orjson+jsonpath-ng | 6.735    | 500     | 74.0     |
| wide_arrays:$.records\[0\]     | strata             | 2.527    | 1       | 74.0     |
|                                | orjson+jmespath    | 4.184    | 1       | 74.0     |
|                                | orjson+jsonpath-ng | 4.471    | 1       | 74.0     |
| mixed:$.records\[\*\].id       | strata             | 0.566    | 500     | 74.5     |
|                                | orjson+jmespath    | 0.794    | 500     | 74.5     |
|                                | orjson+jsonpath-ng | 2.414    | 500     | 74.5     |
| mixed:$.records\[0\]           | strata             | 0.598    | 1       | 74.5     |
|                                | orjson+jsonpath-ng | 0.620    | 1       | 74.5     |
|                                | orjson+jmespath    | 0.755    | 1       | 74.5     |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.081    | 1000    | 53.1     |
|                                            | jmespath    | 0.430    | 1000    | 53.1     |
|                                            | jsonpath-ng | 4.474    | 1000    | 53.1     |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 0.944    | 12046   | 53.1     |
|                                            | jmespath    | 12.832   | 1000    | 53.1     |
|                                            | jsonpath-ng | 41.861   | 12046   | 55.8     |
| $..price                                   | strata      | 1.283    | 12046   | 54.8     |
|                                            | jsonpath-ng | 129.941  | 12046   | 56.8     |
| $.users\[?(@.age>30)\]                     | strata      | 0.119    | 794     | 55.1     |
| $..orders\[?(@.status=="shipped")\]        | strata      | 1.504    | 1604    | 55.2     |
| flat:$.records\[\*\].id                    | strata      | 0.034    | 500     | 60.6     |
|                                            | jmespath    | 0.172    | 500     | 60.6     |
|                                            | jsonpath-ng | 3.414    | 500     | 60.6     |
| flat:$.records\[0\]                        | strata      | 0.010    | 1       | 60.6     |
|                                            | jmespath    | 0.016    | 1       | 60.6     |
|                                            | jsonpath-ng | 0.016    | 1       | 60.6     |
| nested:$.records\[\*\].id                  | strata      | 0.028    | 500     | 60.7     |
|                                            | jmespath    | 0.175    | 500     | 60.7     |
|                                            | jsonpath-ng | 1.934    | 500     | 60.7     |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 60.7     |
|                                            | jmespath    | 0.005    | 1       | 60.7     |
|                                            | jsonpath-ng | 0.007    | 1       | 60.7     |
| wide_arrays:$.records\[\*\].id             | strata      | 0.036    | 500     | 74.0     |
|                                            | jmespath    | 0.194    | 500     | 74.0     |
|                                            | jsonpath-ng | 2.790    | 500     | 74.0     |
| wide_arrays:$.records\[0\]                 | strata      | 0.003    | 1       | 74.0     |
|                                            | jmespath    | 0.007    | 1       | 74.0     |
|                                            | jsonpath-ng | 0.009    | 1       | 74.0     |
| mixed:$.records\[\*\].id                   | strata      | 0.032    | 500     | 74.5     |
|                                            | jmespath    | 0.172    | 500     | 74.5     |
|                                            | jsonpath-ng | 1.784    | 500     | 74.5     |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 74.5     |
|                                            | jsonpath-ng | 0.016    | 1       | 74.5     |
|                                            | jmespath    | 0.017    | 1       | 74.5     |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **26.3% faster** than #2 (msgspec)  |
| loads (NDJSON)      | **#1** / 5              | **13.0% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **33.0% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **23.3% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **33.6% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **31.2% faster** than #2 (orjson)   |
| loads (flat)        | **#3** / 5              | 12.9% behind #1 (msgspec)           |
| load (flat)         | **#2** / 5              | 6.2% behind #1 (orjson)             |
| dumps (flat)        | **#1** / 5              | **2.8% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **33.5% faster** than #2 (msgspec)  |
| loads (nested)      | **#1** / 5              | **162.1% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **169.2% faster** than #2 (msgspec) |
| dumps (nested)      | **#1** / 5              | **4.3% faster** than #2 (orjson)    |
| dump (nested)       | **#1** / 5              | **4.8% faster** than #2 (orjson)    |
| loads (wide_arrays) | **#1** / 5              | **13.7% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **58.1% faster** than #2 (msgspec)  |
| dumps (wide_arrays) | **#1** / 5              | **8.0% faster** than #2 (orjson)    |
| dump (wide_arrays)  | **#1** / 5              | **12.7% faster** than #2 (orjson)   |
| loads (mixed)       | **#3** / 5              | 33.1% behind #1 (orjson)            |
| load (mixed)        | **#2** / 5              | 13.1% behind #1 (msgspec)           |
| dumps (mixed)       | **#1** / 5              | **8.4% faster** than #2 (orjson)    |
| dump (mixed)        | **#3** / 5              | 21.9% behind #1 (orjson)            |
| search (JSONPath)   | **#1** in 11/11 queries | avg **710% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
