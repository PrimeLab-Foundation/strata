# Strata Benchmark Results

Generated: 2026-03-18 23:08:59

## Environment

- Commit: fe648ede876d126597669efe7e2ea296a394b703
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/users.json, benchmarks/data/generated/users.ndjson
- Random schemas: flat, nested, wide_arrays, mixed (2000 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 2.552    | 2.956       | 3.099    | 226.5    |
| orjson        | flat         | 3.766    | 4.722       | 4.818    | 229.0    |
| msgspec       | flat         | 4.643    | 5.379       | 6.448    | 229.0    |
| ujson         | flat         | 5.738    | 6.970       | 7.358    | 229.0    |
| json (stdlib) | flat         | 10.660   | 10.681      | 13.367   | 230.2    |
| strata        | mixed        | 2.964    | 3.293       | 6.327    | 143.4    |
| orjson        | mixed        | 5.296    | 5.797       | 6.984    | 145.6    |
| msgspec       | mixed        | 4.883    | 8.162       | 9.851    | 145.6    |
| ujson         | mixed        | 6.455    | 9.489       | 9.685    | 145.6    |
| json (stdlib) | mixed        | 12.541   | 12.912      | 12.933   | 145.6    |
| strata        | nested       | 2.319    | 2.839       | 3.051    | 232.8    |
| orjson        | nested       | 5.459    | 7.381       | 8.794    | 232.8    |
| ujson         | nested       | 8.380    | 9.338       | 9.418    | 232.8    |
| msgspec       | nested       | 5.350    | 9.637       | 10.757   | 232.8    |
| json (stdlib) | nested       | 10.335   | 11.382      | 11.995   | 232.8    |
| strata        | users.json   | 20.234   | 26.954      | 28.352   | 64.2     |
| orjson        | users.json   | 26.530   | 41.125      | 55.097   | 80.2     |
| msgspec       | users.json   | 27.489   | 41.254      | 62.958   | 80.0     |
| ujson         | users.json   | 35.193   | 53.482      | 68.830   | 94.0     |
| json (stdlib) | users.json   | 49.161   | 65.637      | 65.889   | 85.0     |
| strata        | users.ndjson | 19.574   | 24.713      | 25.238   | 208.8    |
| msgspec       | users.ndjson | 30.748   | 32.618      | 32.704   | 216.3    |
| orjson        | users.ndjson | 32.614   | 38.875      | 42.801   | 215.3    |
| ujson         | users.ndjson | 43.337   | 46.412      | 51.747   | 216.3    |
| json (stdlib) | users.ndjson | 62.807   | 66.702      | 69.331   | 217.3    |
| strata        | wide_arrays  | 11.591   | 13.580      | 13.678   | 244.9    |
| orjson        | wide_arrays  | 12.066   | 16.070      | 18.687   | 255.2    |
| msgspec       | wide_arrays  | 17.634   | 19.817      | 19.922   | 258.7    |
| ujson         | wide_arrays  | 25.298   | 29.099      | 31.264   | 258.7    |
| json (stdlib) | wide_arrays  | 55.360   | 58.574      | 58.797   | 259.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 2.849    | 3.712       | 3.893    | 230.2    |
| msgspec       | flat         | 4.293    | 4.945       | 4.967    | 230.2    |
| orjson        | flat         | 4.813    | 5.099       | 6.134    | 230.2    |
| ujson         | flat         | 6.884    | 7.168       | 7.903    | 230.2    |
| json (stdlib) | flat         | 10.104   | 10.614      | 10.829   | 230.2    |
| strata        | mixed        | 3.215    | 3.328       | 3.418    | 145.6    |
| orjson        | mixed        | 5.062    | 5.391       | 6.480    | 145.6    |
| msgspec       | mixed        | 4.814    | 6.006       | 6.210    | 145.6    |
| ujson         | mixed        | 7.371    | 8.409       | 8.888    | 145.6    |
| json (stdlib) | mixed        | 11.732   | 11.990      | 13.071   | 145.6    |
| strata        | nested       | 2.584    | 3.513       | 3.544    | 232.8    |
| msgspec       | nested       | 6.718    | 7.550       | 8.880    | 232.8    |
| orjson        | nested       | 6.247    | 8.395       | 8.480    | 232.8    |
| ujson         | nested       | 8.349    | 8.819       | 11.029   | 232.8    |
| json (stdlib) | nested       | 10.009   | 11.230      | 12.822   | 232.8    |
| strata        | users.json   | 23.835   | 24.548      | 30.542   | 85.5     |
| orjson        | users.json   | 24.106   | 38.335      | 48.017   | 85.5     |
| msgspec       | users.json   | 27.040   | 42.649      | 51.553   | 90.2     |
| ujson         | users.json   | 43.351   | 57.064      | 57.465   | 104.3    |
| json (stdlib) | users.json   | 57.324   | 66.968      | 97.331   | 90.2     |
| strata        | users.ndjson | 19.746   | 25.108      | 28.941   | 218.3    |
| orjson        | users.ndjson | 31.247   | 32.131      | 34.056   | 219.3    |
| msgspec       | users.ndjson | 34.121   | 34.292      | 34.747   | 221.9    |
| ujson         | users.ndjson | 45.178   | 48.311      | 60.674   | 221.9    |
| json (stdlib) | users.ndjson | 59.737   | 60.672      | 69.654   | 222.9    |
| strata        | wide_arrays  | 13.413   | 14.808      | 14.938   | 259.6    |
| orjson        | wide_arrays  | 13.237   | 17.670      | 17.965   | 259.6    |
| msgspec       | wide_arrays  | 20.400   | 21.039      | 28.122   | 259.7    |
| ujson         | wide_arrays  | 28.003   | 32.066      | 36.708   | 259.7    |
| json (stdlib) | wide_arrays  | 54.907   | 58.394      | 59.814   | 259.7    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.913    | 0.916       | 0.920    | 230.2    |
| orjson  | flat        | 0.986    | 0.987       | 1.074    | 232.5    |
| msgspec | flat        | 1.155    | 1.182       | 1.223    | 232.5    |
| ujson   | flat        | 4.820    | 4.947       | 5.025    | 234.7    |
| json    | flat        | 9.046    | 9.067       | 9.074    | 234.7    |
| strata  | mixed       | 1.099    | 1.151       | 1.622    | 145.6    |
| orjson  | mixed       | 1.170    | 1.215       | 1.301    | 145.6    |
| msgspec | mixed       | 1.205    | 1.231       | 1.233    | 145.6    |
| ujson   | mixed       | 5.160    | 5.165       | 5.189    | 145.6    |
| json    | mixed       | 11.989   | 12.005      | 12.278   | 145.6    |
| strata  | nested      | 0.803    | 0.810       | 0.826    | 232.8    |
| orjson  | nested      | 0.801    | 0.817       | 1.212    | 232.8    |
| msgspec | nested      | 0.802    | 0.882       | 1.263    | 232.8    |
| ujson   | nested      | 3.934    | 3.947       | 3.965    | 232.8    |
| json    | nested      | 5.191    | 5.221       | 5.392    | 232.8    |
| strata  | users.json  | 5.740    | 6.013       | 6.718    | 94.2     |
| orjson  | users.json  | 8.435    | 8.534       | 8.676    | 113.3    |
| msgspec | users.json  | 8.693    | 8.772       | 9.801    | 133.4    |
| ujson   | users.json  | 34.097   | 36.340      | 39.187   | 138.5    |
| json    | users.json  | 49.887   | 50.087      | 50.309   | 159.0    |
| strata  | wide_arrays | 5.913    | 5.959       | 6.164    | 264.7    |
| msgspec | wide_arrays | 7.484    | 7.635       | 7.757    | 255.2    |
| orjson  | wide_arrays | 7.466    | 8.419       | 9.282    | 256.5    |
| ujson   | wide_arrays | 28.615   | 29.105      | 30.235   | 263.8    |
| json    | wide_arrays | 84.028   | 84.488      | 86.127   | 256.9    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.227    | 1.308       | 1.341    | 234.7    |
| orjson        | flat        | 1.316    | 1.319       | 1.360    | 234.7    |
| msgspec       | flat        | 1.755    | 1.815       | 1.990    | 234.7    |
| ujson         | flat        | 5.203    | 5.243       | 5.305    | 234.7    |
| json (stdlib) | flat        | 32.719   | 32.812      | 32.931   | 234.7    |
| strata        | mixed       | 1.575    | 1.584       | 1.951    | 145.6    |
| msgspec       | mixed       | 1.561    | 1.710       | 1.716    | 145.6    |
| orjson        | mixed       | 1.691    | 1.734       | 1.773    | 145.6    |
| ujson         | mixed       | 5.837    | 6.040       | 6.313    | 145.6    |
| json (stdlib) | mixed       | 41.645   | 41.940      | 43.392   | 145.6    |
| strata        | nested      | 1.083    | 1.130       | 1.197    | 232.8    |
| orjson        | nested      | 1.114    | 1.176       | 1.187    | 232.8    |
| msgspec       | nested      | 1.188    | 1.492       | 1.608    | 232.8    |
| ujson         | nested      | 4.297    | 4.754       | 5.025    | 232.8    |
| json (stdlib) | nested      | 49.966   | 50.457      | 50.569   | 232.8    |
| strata        | users.json  | 6.645    | 7.465       | 9.464    | 164.1    |
| orjson        | users.json  | 9.450    | 10.055      | 10.741   | 182.2    |
| msgspec       | users.json  | 10.094   | 11.213      | 12.681   | 195.4    |
| ujson         | users.json  | 39.499   | 43.516      | 52.310   | 182.3    |
| json (stdlib) | users.json  | 374.491  | 382.703     | 409.278  | 195.4    |
| strata        | wide_arrays | 6.951    | 7.221       | 7.278    | 256.9    |
| msgspec       | wide_arrays | 8.716    | 8.998       | 9.576    | 278.6    |
| orjson        | wide_arrays | 8.924    | 9.232       | 9.483    | 261.8    |
| ujson         | wide_arrays | 31.091   | 32.176      | 34.414   | 278.5    |
| json (stdlib) | wide_arrays | 183.196  | 185.472     | 201.570  | 278.6    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 10.496   | 5000    | 195.6    |
|                                | orjson+jmespath    | 31.097   | 5000    | 195.7    |
|                                | orjson+jsonpath-ng | 35.955   | 5000    | 199.1    |
| all item prices                | strata             | 11.422   | 62331   | 199.4    |
|                                | orjson+jmespath    | 78.263   | 5000    | 204.2    |
|                                | orjson+jsonpath-ng | 219.886  | 62331   | 211.7    |
| recursive price                | strata             | 10.871   | 62331   | 210.0    |
|                                | orjson+jsonpath-ng | 730.351  | 62331   | 212.1    |
| flat:$.records\[\*\].id        | strata             | 1.852    | 2000    | 234.7    |
|                                | orjson+jmespath    | 4.785    | 2000    | 234.8    |
|                                | orjson+jsonpath-ng | 7.790    | 2000    | 232.8    |
| flat:$.records\[0\]            | strata             | 1.945    | 1       | 232.8    |
|                                | orjson+jmespath    | 4.876    | 1       | 232.8    |
|                                | orjson+jsonpath-ng | 5.086    | 1       | 232.8    |
| nested:$.records\[\*\].id      | strata             | 1.581    | 2000    | 232.8    |
|                                | orjson+jmespath    | 6.133    | 2000    | 232.8    |
|                                | orjson+jsonpath-ng | 8.556    | 2000    | 232.8    |
| nested:$.records\[0\]          | strata             | 1.505    | 1       | 232.8    |
|                                | orjson+jsonpath-ng | 5.064    | 1       | 232.8    |
|                                | orjson+jmespath    | 5.302    | 1       | 232.8    |
| wide_arrays:$.records\[\*\].id | strata             | 9.525    | 2000    | 278.8    |
|                                | orjson+jmespath    | 16.976   | 2000    | 278.8    |
|                                | orjson+jsonpath-ng | 21.779   | 2000    | 227.0    |
| wide_arrays:$.records\[0\]     | strata             | 9.346    | 1       | 227.0    |
|                                | orjson+jsonpath-ng | 14.657   | 1       | 227.0    |
|                                | orjson+jmespath    | 15.315   | 1       | 227.0    |
| mixed:$.records\[\*\].id       | strata             | 1.775    | 2000    | 145.6    |
|                                | orjson+jmespath    | 6.329    | 2000    | 145.6    |
|                                | orjson+jsonpath-ng | 7.681    | 2000    | 145.6    |
| mixed:$.records\[0\]           | strata             | 1.686    | 1       | 145.6    |
|                                | orjson+jmespath    | 4.615    | 1       | 145.6    |
|                                | orjson+jsonpath-ng | 5.341    | 1       | 145.6    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.681    | 5000    | 230.2    |
|                                            | jmespath    | 2.146    | 5000    | 230.2    |
|                                            | jsonpath-ng | 23.083   | 5000    | 232.3    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 4.686    | 62331   | 231.9    |
|                                            | jmespath    | 56.654   | 5000    | 235.9    |
|                                            | jsonpath-ng | 220.747  | 62331   | 240.0    |
| $..price                                   | strata      | 7.351    | 62331   | 238.1    |
|                                            | jsonpath-ng | 801.829  | 62331   | 240.4    |
| $.users\[?(@.age>30)\]                     | strata      | 0.862    | 3999    | 237.4    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 8.519    | 8402    | 223.9    |
| flat:$.records\[\*\].id                    | strata      | 0.129    | 2000    | 232.8    |
|                                            | jmespath    | 0.673    | 2000    | 232.8    |
|                                            | jsonpath-ng | 6.301    | 2000    | 232.8    |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 232.8    |
|                                            | jmespath    | 0.006    | 1       | 232.8    |
|                                            | jsonpath-ng | 0.013    | 1       | 232.8    |
| nested:$.records\[\*\].id                  | strata      | 0.133    | 2000    | 232.8    |
|                                            | jmespath    | 0.748    | 2000    | 232.8    |
|                                            | jsonpath-ng | 5.748    | 2000    | 232.8    |
| nested:$.records\[0\]                      | strata      | 0.006    | 1       | 232.8    |
|                                            | jmespath    | 0.011    | 1       | 232.8    |
|                                            | jsonpath-ng | 0.012    | 1       | 232.8    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.117    | 2000    | 227.0    |
|                                            | jmespath    | 0.719    | 2000    | 227.0    |
|                                            | jsonpath-ng | 8.067    | 2000    | 227.0    |
| wide_arrays:$.records\[0\]                 | strata      | 0.009    | 1       | 227.0    |
|                                            | jmespath    | 0.011    | 1       | 227.0    |
|                                            | jsonpath-ng | 0.016    | 1       | 227.0    |
| mixed:$.records\[\*\].id                   | strata      | 0.135    | 2000    | 145.7    |
|                                            | jmespath    | 0.723    | 2000    | 145.7    |
|                                            | jsonpath-ng | 5.277    | 2000    | 145.7    |
| mixed:$.records\[0\]                       | strata      | 0.005    | 1       | 145.7    |
|                                            | jsonpath-ng | 0.008    | 1       | 145.7    |
|                                            | jmespath    | 0.008    | 1       | 145.7    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **31.1% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **57.1% faster** than #2 (msgspec)  |
| load (JSON file)    | **#1** / 5              | **1.1% faster** than #2 (orjson)    |
| load (NDJSON file)  | **#1** / 5              | **58.2% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **46.9% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **42.2% faster** than #2 (orjson)   |
| loads (flat)        | **#1** / 5              | **47.5% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **50.7% faster** than #2 (msgspec)  |
| dumps (flat)        | **#1** / 5              | **7.9% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **7.2% faster** than #2 (orjson)    |
| loads (nested)      | **#1** / 5              | **130.7% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **141.8% faster** than #2 (orjson)  |
| dumps (nested)      | **#3** / 5              | 0.2% behind #1 (orjson)             |
| dump (nested)       | **#1** / 5              | **2.9% faster** than #2 (orjson)    |
| loads (wide_arrays) | **#1** / 5              | **4.1% faster** than #2 (orjson)    |
| load (wide_arrays)  | **#2** / 5              | 1.3% behind #1 (orjson)             |
| dumps (wide_arrays) | **#1** / 5              | **26.3% faster** than #2 (orjson)   |
| dump (wide_arrays)  | **#1** / 5              | **25.4% faster** than #2 (msgspec)  |
| loads (mixed)       | **#1** / 5              | **64.8% faster** than #2 (msgspec)  |
| load (mixed)        | **#1** / 5              | **49.7% faster** than #2 (msgspec)  |
| dumps (mixed)       | **#1** / 5              | **6.4% faster** than #2 (orjson)    |
| dump (mixed)        | **#2** / 5              | 0.9% behind #1 (msgspec)            |
| search (JSONPath)   | **#1** in 11/11 queries | avg **800% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
