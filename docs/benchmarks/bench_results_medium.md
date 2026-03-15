# Strata Benchmark Results

Generated: 2026-03-15 18:34:58

## Environment

- Commit: 67ef7fc31abef4549164a63037b9929eceed01a5
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/medium/users.json, benchmarks/data/generated/medium/users.ndjson
- Random schemas: flat, nested, wide_arrays, mixed (2000 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 3.648    | 4.063       | 4.133    | 277.5    |
| msgspec       | flat         | 3.569    | 4.316       | 4.356    | 277.5    |
| strata        | flat         | 5.512    | 5.815       | 5.868    | 272.5    |
| ujson         | flat         | 5.875    | 6.275       | 6.477    | 277.5    |
| json (stdlib) | flat         | 9.553    | 9.631       | 9.860    | 277.5    |
| orjson        | mixed        | 4.024    | 4.653       | 5.162    | 154.9    |
| msgspec       | mixed        | 4.357    | 5.012       | 5.534    | 154.9    |
| strata        | mixed        | 5.862    | 6.335       | 6.395    | 152.6    |
| ujson         | mixed        | 6.012    | 6.888       | 7.156    | 154.9    |
| json (stdlib) | mixed        | 9.468    | 10.575      | 10.611   | 154.9    |
| orjson        | nested       | 4.592    | 5.496       | 6.409    | 283.5    |
| msgspec       | nested       | 4.281    | 6.103       | 6.685    | 283.6    |
| ujson         | nested       | 5.981    | 7.053       | 7.263    | 283.6    |
| strata        | nested       | 8.716    | 8.998       | 9.016    | 281.2    |
| json (stdlib) | nested       | 8.208    | 10.541      | 11.095   | 283.6    |
| orjson        | users.json   | 25.831   | 36.167      | 37.193   | 93.5     |
| msgspec       | users.json   | 27.274   | 38.389      | 39.670   | 92.3     |
| ujson         | users.json   | 37.545   | 51.809      | 53.457   | 109.9    |
| json (stdlib) | users.json   | 53.167   | 63.860      | 64.542   | 98.5     |
| strata        | users.json   | 73.069   | 77.113      | 77.312   | 72.9     |
| orjson        | users.ndjson | 31.460   | 33.544      | 34.777   | 260.1    |
| msgspec       | users.ndjson | 32.649   | 34.486      | 35.887   | 261.2    |
| ujson         | users.ndjson | 42.468   | 46.849      | 49.416   | 260.2    |
| json (stdlib) | users.ndjson | 60.274   | 60.865      | 61.812   | 262.2    |
| strata        | users.ndjson | 71.506   | 75.960      | 76.951   | 271.9    |
| orjson        | wide_arrays  | 11.943   | 15.039      | 15.089   | 306.9    |
| msgspec       | wide_arrays  | 14.962   | 17.929      | 18.390   | 306.9    |
| ujson         | wide_arrays  | 23.709   | 26.780      | 26.987   | 306.9    |
| strata        | wide_arrays  | 28.002   | 30.593      | 30.653   | 296.7    |
| json (stdlib) | wide_arrays  | 49.434   | 51.880      | 52.773   | 306.9    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| msgspec       | flat         | 3.992    | 4.553       | 4.733    | 278.7    |
| orjson        | flat         | 3.726    | 4.566       | 4.659    | 277.5    |
| strata        | flat         | 5.583    | 6.086       | 6.211    | 277.5    |
| ujson         | flat         | 6.225    | 7.022       | 7.209    | 278.7    |
| json (stdlib) | flat         | 9.872    | 10.262      | 10.266   | 278.7    |
| msgspec       | mixed        | 4.200    | 5.036       | 5.725    | 155.0    |
| orjson        | mixed        | 4.376    | 5.125       | 5.531    | 154.9    |
| strata        | mixed        | 6.216    | 6.685       | 6.945    | 154.9    |
| ujson         | mixed        | 6.521    | 7.204       | 7.464    | 155.0    |
| json (stdlib) | mixed        | 10.410   | 10.795      | 11.060   | 155.0    |
| orjson        | nested       | 4.923    | 5.848       | 6.213    | 283.6    |
| msgspec       | nested       | 4.507    | 6.047       | 6.485    | 284.4    |
| ujson         | nested       | 5.734    | 7.624       | 8.043    | 284.4    |
| strata        | nested       | 8.797    | 9.131       | 9.402    | 283.6    |
| json (stdlib) | nested       | 8.820    | 9.338       | 9.867    | 284.4    |
| orjson        | users.json   | 26.165   | 36.340      | 38.341   | 99.9     |
| msgspec       | users.json   | 27.483   | 38.818      | 40.882   | 104.8    |
| ujson         | users.json   | 38.412   | 52.865      | 55.086   | 122.5    |
| json (stdlib) | users.json   | 53.554   | 64.448      | 65.782   | 104.8    |
| strata        | users.json   | 72.620   | 77.341      | 78.181   | 99.3     |
| orjson        | users.ndjson | 31.935   | 33.448      | 34.923   | 265.3    |
| msgspec       | users.ndjson | 35.972   | 37.381      | 37.700   | 266.3    |
| ujson         | users.ndjson | 44.813   | 51.294      | 53.296   | 265.3    |
| json (stdlib) | users.ndjson | 60.468   | 61.224      | 61.847   | 267.3    |
| strata        | users.ndjson | 72.981   | 76.956      | 77.399   | 264.3    |
| orjson        | wide_arrays  | 12.181   | 15.419      | 15.941   | 306.6    |
| msgspec       | wide_arrays  | 15.354   | 19.046      | 19.096   | 306.6    |
| ujson         | wide_arrays  | 25.660   | 28.900      | 28.986   | 306.6    |
| strata        | wide_arrays  | 29.147   | 31.273      | 31.376   | 307.0    |
| json (stdlib) | wide_arrays  | 50.818   | 52.853      | 53.076   | 306.6    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.925    | 0.966       | 1.038    | 281.0    |
| msgspec | flat        | 1.170    | 1.178       | 1.228    | 281.0    |
| strata  | flat        | 1.193    | 1.201       | 1.208    | 278.7    |
| ujson   | flat        | 4.549    | 4.668       | 4.776    | 282.2    |
| json    | flat        | 8.746    | 8.801       | 9.003    | 282.2    |
| orjson  | mixed       | 1.051    | 1.061       | 1.125    | 155.0    |
| msgspec | mixed       | 1.133    | 1.189       | 1.228    | 155.0    |
| strata  | mixed       | 1.218    | 1.256       | 1.327    | 155.0    |
| ujson   | mixed       | 4.883    | 4.890       | 4.916    | 155.6    |
| json    | mixed       | 11.173   | 11.216      | 11.252   | 155.6    |
| strata  | nested      | 0.656    | 0.681       | 0.729    | 284.4    |
| orjson  | nested      | 0.735    | 0.740       | 0.743    | 284.4    |
| msgspec | nested      | 0.775    | 0.777       | 0.832    | 284.4    |
| ujson   | nested      | 3.757    | 3.921       | 4.028    | 284.4    |
| json    | nested      | 5.142    | 5.144       | 5.248    | 284.4    |
| orjson  | users.json  | 10.513   | 10.514      | 10.527   | 131.5    |
| msgspec | users.json  | 10.499   | 10.521      | 10.601   | 159.9    |
| strata  | users.json  | 10.556   | 10.570      | 10.751   | 109.2    |
| ujson   | users.json  | 38.637   | 38.953      | 40.227   | 165.7    |
| json    | users.json  | 55.956   | 55.978      | 57.547   | 191.0    |
| msgspec | wide_arrays | 6.418    | 6.468       | 6.493    | 319.1    |
| orjson  | wide_arrays | 6.633    | 6.635       | 6.653    | 299.4    |
| strata  | wide_arrays | 6.963    | 6.966       | 6.994    | 309.0    |
| ujson   | wide_arrays | 26.843   | 26.850      | 26.865   | 321.1    |
| json    | wide_arrays | 75.248   | 75.282      | 75.392   | 342.0    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.408    | 1.415       | 1.425    | 282.2    |
| orjson        | flat        | 1.316    | 1.420       | 1.479    | 282.2    |
| msgspec       | flat        | 1.533    | 1.536       | 1.588    | 282.2    |
| ujson         | flat        | 5.156    | 5.212       | 5.279    | 282.2    |
| json (stdlib) | flat        | 32.197   | 32.309      | 32.617   | 282.2    |
| msgspec       | mixed       | 1.411    | 1.416       | 1.561    | 155.6    |
| orjson        | mixed       | 1.376    | 1.440       | 1.480    | 155.6    |
| strata        | mixed       | 1.425    | 1.455       | 1.482    | 155.6    |
| ujson         | mixed       | 5.365    | 5.545       | 5.632    | 155.6    |
| json (stdlib) | mixed       | 38.680   | 38.740      | 38.959   | 155.6    |
| strata        | nested      | 0.884    | 1.162       | 1.414    | 284.4    |
| msgspec       | nested      | 1.064    | 1.442       | 1.526    | 284.4    |
| orjson        | nested      | 1.044    | 1.444       | 1.496    | 284.4    |
| ujson         | nested      | 4.470    | 4.612       | 4.732    | 284.4    |
| json (stdlib) | nested      | 47.836   | 48.336      | 48.513   | 284.4    |
| strata        | users.json  | 11.237   | 11.279      | 11.728   | 197.3    |
| orjson        | users.json  | 11.652   | 11.886      | 11.975   | 219.6    |
| msgspec       | users.json  | 11.677   | 11.951      | 15.422   | 241.9    |
| ujson         | users.json  | 41.232   | 41.435      | 43.050   | 219.6    |
| json (stdlib) | users.json  | 408.739  | 409.243     | 411.878  | 242.0    |
| strata        | wide_arrays | 7.531    | 7.825       | 8.117    | 342.0    |
| orjson        | wide_arrays | 7.728    | 8.011       | 8.511    | 339.7    |
| msgspec       | wide_arrays | 7.935    | 8.246       | 8.297    | 361.4    |
| ujson         | wide_arrays | 28.573   | 28.718      | 28.924   | 341.7    |
| json (stdlib) | wide_arrays | 168.675  | 170.999     | 171.030  | 361.4    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 9.447    | 2000    | 239.0    |
|                                | orjson+jmespath    | 29.322   | 2000    | 242.1    |
|                                | orjson+jsonpath-ng | 31.303   | 2000    | 244.4    |
| all item prices                | strata             | 11.321   | 100999  | 246.0    |
|                                | orjson+jmespath    | 76.984   | 2000    | 255.2    |
|                                | orjson+jsonpath-ng | 242.666  | 100999  | 262.2    |
| recursive price                | strata             | 11.012   | 100999  | 261.6    |
|                                | orjson+jsonpath-ng | 825.350  | 100999  | 264.1    |
| flat:$.records\[\*\].id        | strata             | 1.376    | 2000    | 282.2    |
|                                | orjson+jmespath    | 4.650    | 2000    | 282.2    |
|                                | orjson+jsonpath-ng | 6.739    | 2000    | 281.2    |
| flat:$.records\[0\]            | strata             | 1.403    | 1       | 281.2    |
|                                | orjson+jsonpath-ng | 4.042    | 1       | 281.2    |
|                                | orjson+jmespath    | 4.059    | 1       | 281.2    |
| nested:$.records\[\*\].id      | strata             | 1.080    | 2000    | 284.4    |
|                                | orjson+jmespath    | 6.089    | 2000    | 284.4    |
|                                | orjson+jsonpath-ng | 8.244    | 2000    | 284.4    |
| nested:$.records\[0\]          | strata             | 1.140    | 1       | 284.4    |
|                                | orjson+jsonpath-ng | 4.625    | 1       | 284.4    |
|                                | orjson+jmespath    | 4.629    | 1       | 284.4    |
| wide_arrays:$.records\[\*\].id | strata             | 7.555    | 2000    | 361.4    |
|                                | orjson+jmespath    | 14.453   | 2000    | 361.4    |
|                                | orjson+jsonpath-ng | 16.907   | 2000    | 361.4    |
| wide_arrays:$.records\[0\]     | strata             | 7.606    | 1       | 361.5    |
|                                | orjson+jsonpath-ng | 13.703   | 1       | 361.5    |
|                                | orjson+jmespath    | 14.258   | 1       | 361.5    |
| mixed:$.records\[\*\].id       | strata             | 1.380    | 2000    | 155.6    |
|                                | orjson+jmespath    | 5.099    | 2000    | 155.6    |
|                                | orjson+jsonpath-ng | 7.673    | 2000    | 155.6    |
| mixed:$.records\[0\]           | strata             | 1.456    | 1       | 155.6    |
|                                | orjson+jsonpath-ng | 4.693    | 1       | 155.6    |
|                                | orjson+jmespath    | 4.736    | 1       | 155.6    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.242    | 2000    | 286.5    |
|                                            | jmespath    | 0.812    | 2000    | 286.5    |
|                                            | jsonpath-ng | 14.710   | 2000    | 287.3    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 5.922    | 100999  | 287.3    |
|                                            | jmespath    | 58.400   | 2000    | 291.9    |
|                                            | jsonpath-ng | 243.362  | 100999  | 293.4    |
| $..price                                   | strata      | 7.806    | 100999  | 293.4    |
|                                            | jsonpath-ng | 813.684  | 100999  | 296.5    |
| $.users\[?(@.age>30)\]                     | strata      | 0.287    | 1606    | 294.5    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 8.461    | 6650    | 294.5    |
| flat:$.records\[\*\].id                    | strata      | 0.120    | 2000    | 281.2    |
|                                            | jmespath    | 0.646    | 2000    | 281.2    |
|                                            | jsonpath-ng | 4.219    | 2000    | 281.2    |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 281.2    |
|                                            | jmespath    | 0.006    | 1       | 281.2    |
|                                            | jsonpath-ng | 0.010    | 1       | 281.2    |
| nested:$.records\[\*\].id                  | strata      | 0.105    | 2000    | 284.4    |
|                                            | jmespath    | 0.673    | 2000    | 284.4    |
|                                            | jsonpath-ng | 5.444    | 2000    | 284.4    |
| nested:$.records\[0\]                      | strata      | 0.006    | 1       | 284.4    |
|                                            | jsonpath-ng | 0.008    | 1       | 284.4    |
|                                            | jmespath    | 0.008    | 1       | 284.4    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.098    | 2000    | 361.5    |
|                                            | jmespath    | 0.677    | 2000    | 361.5    |
|                                            | jsonpath-ng | 6.693    | 2000    | 361.5    |
| wide_arrays:$.records\[0\]                 | jmespath    | 0.009    | 1       | 361.5    |
|                                            | jsonpath-ng | 0.010    | 1       | 361.5    |
|                                            | strata      | 0.012    | 1       | 361.5    |
| mixed:$.records\[\*\].id                   | strata      | 0.133    | 2000    | 155.6    |
|                                            | jmespath    | 0.662    | 2000    | 155.6    |
|                                            | jsonpath-ng | 4.873    | 2000    | 155.6    |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 155.6    |
|                                            | jmespath    | 0.007    | 1       | 155.6    |
|                                            | jsonpath-ng | 0.008    | 1       | 155.6    |

## Summary

| Category            | Strata Rank             | vs #1                             |
| ------------------- | ----------------------- | --------------------------------- |
| loads (JSON)        | **#5** / 5              | 182.9% behind #1 (orjson)         |
| loads (NDJSON)      | **#5** / 5              | 127.3% behind #1 (orjson)         |
| load (JSON file)    | **#5** / 5              | 177.5% behind #1 (orjson)         |
| load (NDJSON file)  | **#5** / 5              | 128.5% behind #1 (orjson)         |
| dumps               | **#3** / 5              | 0.5% behind #1 (msgspec)          |
| dump                | **#1** / 5              | **3.7% faster** than #2 (orjson)  |
| loads (flat)        | **#3** / 5              | 54.5% behind #1 (msgspec)         |
| load (flat)         | **#3** / 5              | 49.8% behind #1 (orjson)          |
| dumps (flat)        | **#3** / 5              | 29.0% behind #1 (orjson)          |
| dump (flat)         | **#2** / 5              | 7.0% behind #1 (orjson)           |
| loads (nested)      | **#5** / 5              | 103.6% behind #1 (msgspec)        |
| load (nested)       | **#4** / 5              | 95.2% behind #1 (msgspec)         |
| dumps (nested)      | **#1** / 5              | **12.1% faster** than #2 (orjson) |
| dump (nested)       | **#1** / 5              | **18.2% faster** than #2 (orjson) |
| loads (wide_arrays) | **#4** / 5              | 134.5% behind #1 (orjson)         |
| load (wide_arrays)  | **#4** / 5              | 139.3% behind #1 (orjson)         |
| dumps (wide_arrays) | **#3** / 5              | 8.5% behind #1 (msgspec)          |
| dump (wide_arrays)  | **#1** / 5              | **2.6% faster** than #2 (orjson)  |
| loads (mixed)       | **#3** / 5              | 45.7% behind #1 (orjson)          |
| load (mixed)        | **#3** / 5              | 48.0% behind #1 (msgspec)         |
| dumps (mixed)       | **#3** / 5              | 15.8% behind #1 (orjson)          |
| dump (mixed)        | **#3** / 5              | 3.5% behind #1 (orjson)           |
| search (JSONPath)   | **#1** in 11/11 queries | avg **913% faster**               |
| query (JSONPath)    | **#1** in 12/13 queries | -                                 |
