# Strata Benchmark Results

Generated: 2026-03-19 17:14:22

## Environment

- Commit: a14321cee5dc0690142acfa20e3e17374e5476f2
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
| strata        | flat         | 2.838    | 3.260       | 3.445    | 270.1    |
| orjson        | flat         | 3.626    | 4.187       | 4.435    | 272.6    |
| msgspec       | flat         | 3.594    | 4.378       | 4.676    | 272.6    |
| ujson         | flat         | 5.956    | 6.423       | 6.435    | 272.6    |
| json (stdlib) | flat         | 9.485    | 9.800       | 9.890    | 272.6    |
| strata        | mixed        | 2.937    | 3.420       | 3.475    | 297.9    |
| orjson        | mixed        | 5.395    | 9.947       | 11.930   | 297.9    |
| msgspec       | mixed        | 6.764    | 11.589      | 12.425   | 297.9    |
| ujson         | mixed        | 9.649    | 11.602      | 15.936   | 297.9    |
| json (stdlib) | mixed        | 12.852   | 38.617      | 41.508   | 297.9    |
| strata        | nested       | 2.927    | 3.421       | 3.632    | 277.1    |
| msgspec       | nested       | 5.352    | 6.121       | 7.469    | 277.1    |
| ujson         | nested       | 7.343    | 7.764       | 8.286    | 277.1    |
| orjson        | nested       | 5.512    | 7.781       | 8.707    | 277.1    |
| json (stdlib) | nested       | 8.034    | 9.558       | 9.592    | 277.1    |
| strata        | users.json   | 27.599   | 35.606      | 39.278   | 73.4     |
| orjson        | users.json   | 30.068   | 43.813      | 44.720   | 93.3     |
| msgspec       | users.json   | 32.105   | 45.378      | 67.029   | 92.1     |
| ujson         | users.json   | 55.030   | 62.279      | 66.095   | 109.8    |
| json (stdlib) | users.json   | 66.636   | 81.597      | 83.134   | 98.4     |
| strata        | users.ndjson | 26.107   | 32.424      | 33.561   | 252.4    |
| orjson        | users.ndjson | 32.875   | 36.860      | 45.618   | 260.4    |
| msgspec       | users.ndjson | 33.942   | 38.793      | 49.380   | 261.5    |
| ujson         | users.ndjson | 47.936   | 50.417      | 52.797   | 260.5    |
| json (stdlib) | users.ndjson | 63.773   | 65.515      | 66.290   | 262.5    |
| strata        | wide_arrays  | 12.742   | 14.091      | 15.821   | 289.6    |
| orjson        | wide_arrays  | 12.755   | 15.152      | 16.590   | 299.9    |
| msgspec       | wide_arrays  | 16.223   | 22.813      | 26.473   | 303.4    |
| ujson         | wide_arrays  | 27.813   | 28.004      | 28.721   | 303.4    |
| json (stdlib) | wide_arrays  | 56.015   | 56.130      | 57.079   | 303.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 3.173    | 3.631       | 3.932    | 272.6    |
| msgspec       | flat         | 4.428    | 4.913       | 5.280    | 273.4    |
| orjson        | flat         | 5.420    | 6.063       | 6.555    | 272.6    |
| ujson         | flat         | 7.507    | 7.509       | 8.650    | 273.4    |
| json (stdlib) | flat         | 10.212   | 11.071      | 11.583   | 273.4    |
| strata        | mixed        | 3.178    | 3.960       | 4.127    | 297.9    |
| msgspec       | mixed        | 5.418    | 5.538       | 7.310    | 297.9    |
| orjson        | mixed        | 4.635    | 5.682       | 6.220    | 297.9    |
| ujson         | mixed        | 7.356    | 7.636       | 8.677    | 297.9    |
| json (stdlib) | mixed        | 13.340   | 15.592      | 16.605   | 297.9    |
| strata        | nested       | 2.894    | 3.522       | 4.113    | 277.1    |
| orjson        | nested       | 4.937    | 7.071       | 7.199    | 277.1    |
| msgspec       | nested       | 6.882    | 8.462       | 9.404    | 277.1    |
| json (stdlib) | nested       | 11.747   | 12.002      | 13.787   | 277.1    |
| ujson         | nested       | 10.897   | 13.132      | 20.048   | 277.1    |
| strata        | users.json   | 30.596   | 36.817      | 36.936   | 99.8     |
| orjson        | users.json   | 30.822   | 45.946      | 48.766   | 99.8     |
| msgspec       | users.json   | 31.426   | 48.857      | 58.414   | 104.7    |
| ujson         | users.json   | 47.422   | 62.450      | 72.235   | 122.4    |
| json (stdlib) | users.json   | 69.107   | 80.750      | 99.295   | 104.7    |
| strata        | users.ndjson | 26.568   | 32.198      | 36.586   | 264.5    |
| orjson        | users.ndjson | 35.128   | 36.056      | 37.489   | 265.6    |
| msgspec       | users.ndjson | 37.872   | 38.216      | 38.809   | 266.6    |
| ujson         | users.ndjson | 45.722   | 60.015      | 65.007   | 265.6    |
| json (stdlib) | users.ndjson | 64.533   | 64.935      | 72.730   | 267.6    |
| strata        | wide_arrays  | 12.507   | 15.049      | 15.098   | 303.4    |
| orjson        | wide_arrays  | 13.614   | 21.127      | 22.926   | 303.0    |
| msgspec       | wide_arrays  | 19.140   | 27.184      | 48.663   | 303.0    |
| ujson         | wide_arrays  | 27.586   | 30.153      | 55.231   | 303.0    |
| json (stdlib) | wide_arrays  | 58.393   | 61.074      | 65.406   | 303.0    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 0.972    | 0.975       | 0.976    | 273.4    |
| orjson  | flat        | 1.016    | 1.022       | 1.129    | 275.7    |
| msgspec | flat        | 1.213    | 1.288       | 1.315    | 276.7    |
| ujson   | flat        | 5.084    | 5.483       | 5.570    | 277.0    |
| json    | flat        | 9.586    | 10.234      | 10.941   | 277.1    |
| orjson  | mixed       | 1.306    | 1.355       | 2.083    | 297.9    |
| strata  | mixed       | 1.203    | 1.357       | 1.397    | 297.9    |
| msgspec | mixed       | 1.416    | 1.420       | 1.571    | 297.9    |
| ujson   | mixed       | 5.918    | 6.026       | 6.317    | 297.9    |
| json    | mixed       | 13.002   | 13.313      | 15.277   | 297.9    |
| strata  | nested      | 0.906    | 0.920       | 0.941    | 277.1    |
| msgspec | nested      | 0.836    | 0.932       | 1.188    | 277.1    |
| orjson  | nested      | 0.902    | 0.961       | 1.094    | 277.1    |
| ujson   | nested      | 4.233    | 4.484       | 4.489    | 277.1    |
| json    | nested      | 5.223    | 5.231       | 5.413    | 277.1    |
| strata  | users.json  | 11.097   | 11.250      | 12.008   | 108.6    |
| msgspec | users.json  | 11.519   | 11.537      | 12.133   | 160.9    |
| orjson  | users.json  | 12.216   | 12.470      | 15.692   | 131.8    |
| ujson   | users.json  | 45.516   | 46.150      | 47.097   | 166.9    |
| json    | users.json  | 62.506   | 63.213      | 64.264   | 192.2    |
| strata  | wide_arrays | 6.239    | 6.637       | 7.342    | 308.0    |
| msgspec | wide_arrays | 6.906    | 6.951       | 6.966    | 255.5    |
| orjson  | wide_arrays | 7.264    | 7.464       | 8.502    | 235.8    |
| ujson   | wide_arrays | 28.699   | 28.941      | 29.829   | 257.5    |
| json    | wide_arrays | 84.041   | 84.832      | 87.405   | 278.4    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.299    | 1.319       | 1.981    | 277.1    |
| orjson        | flat        | 1.393    | 1.496       | 1.760    | 277.1    |
| msgspec       | flat        | 1.605    | 1.801       | 1.923    | 277.1    |
| ujson         | flat        | 5.455    | 5.548       | 6.167    | 277.1    |
| json (stdlib) | flat        | 34.237   | 35.042      | 36.217   | 277.1    |
| orjson        | mixed       | 1.588    | 1.649       | 1.902    | 297.9    |
| strata        | mixed       | 1.734    | 1.819       | 1.914    | 297.9    |
| msgspec       | mixed       | 1.806    | 1.874       | 1.887    | 297.9    |
| ujson         | mixed       | 5.770    | 6.007       | 6.761    | 297.9    |
| json (stdlib) | mixed       | 40.242   | 42.205      | 43.963   | 297.9    |
| strata        | nested      | 1.049    | 1.049       | 1.443    | 277.1    |
| msgspec       | nested      | 1.222    | 1.240       | 1.279    | 277.1    |
| orjson        | nested      | 1.285    | 1.424       | 1.489    | 277.1    |
| ujson         | nested      | 4.709    | 4.748       | 5.029    | 277.1    |
| json (stdlib) | nested      | 48.334   | 48.581      | 49.681   | 277.1    |
| strata        | users.json  | 10.114   | 10.331      | 10.732   | 198.5    |
| orjson        | users.json  | 12.975   | 14.118      | 15.809   | 220.8    |
| msgspec       | users.json  | 14.435   | 14.699      | 15.165   | 243.4    |
| ujson         | users.json  | 44.866   | 45.274      | 49.310   | 221.1    |
| json (stdlib) | users.json  | 430.842  | 436.586     | 440.215  | 243.4    |
| strata        | wide_arrays | 7.734    | 8.037       | 9.228    | 278.4    |
| msgspec       | wide_arrays | 8.611    | 8.887       | 9.363    | 297.8    |
| orjson        | wide_arrays | 10.042   | 11.258      | 12.588   | 276.1    |
| ujson         | wide_arrays | 33.587   | 34.067      | 34.192   | 278.1    |
| json (stdlib) | wide_arrays | 187.262  | 197.201     | 199.627  | 297.8    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 12.882   | 2000    | 240.6    |
|                                | orjson+jsonpath-ng | 35.075   | 2000    | 246.0    |
|                                | orjson+jmespath    | 36.060   | 2000    | 243.7    |
| all item prices                | strata             | 14.539   | 100999  | 247.5    |
|                                | orjson+jmespath    | 85.553   | 2000    | 256.7    |
|                                | orjson+jsonpath-ng | 265.675  | 100999  | 262.8    |
| recursive price                | strata             | 14.296   | 100999  | 260.9    |
|                                | orjson+jsonpath-ng | 897.155  | 100999  | 263.4    |
| flat:$.records\[\*\].id        | strata             | 2.102    | 2000    | 277.1    |
|                                | orjson+jmespath    | 5.830    | 2000    | 277.1    |
|                                | orjson+jsonpath-ng | 7.908    | 2000    | 277.1    |
| flat:$.records\[0\]            | strata             | 1.922    | 1       | 277.1    |
|                                | orjson+jsonpath-ng | 4.366    | 1       | 277.1    |
|                                | orjson+jmespath    | 4.501    | 1       | 277.1    |
| nested:$.records\[\*\].id      | strata             | 1.401    | 2000    | 277.1    |
|                                | orjson+jmespath    | 5.633    | 2000    | 277.1    |
|                                | orjson+jsonpath-ng | 8.027    | 2000    | 277.1    |
| nested:$.records\[0\]          | strata             | 1.413    | 1       | 277.1    |
|                                | orjson+jsonpath-ng | 4.880    | 1       | 277.1    |
|                                | orjson+jmespath    | 5.324    | 1       | 277.1    |
| wide_arrays:$.records\[\*\].id | strata             | 10.009   | 2000    | 297.8    |
|                                | orjson+jmespath    | 15.696   | 2000    | 297.8    |
|                                | orjson+jsonpath-ng | 20.220   | 2000    | 297.8    |
| wide_arrays:$.records\[0\]     | strata             | 10.545   | 1       | 297.8    |
|                                | orjson+jsonpath-ng | 14.836   | 1       | 297.8    |
|                                | orjson+jmespath    | 16.447   | 1       | 297.8    |
| mixed:$.records\[\*\].id       | strata             | 2.008    | 2000    | 297.9    |
|                                | orjson+jmespath    | 5.731    | 2000    | 297.9    |
|                                | orjson+jsonpath-ng | 8.309    | 2000    | 297.9    |
| mixed:$.records\[0\]           | strata             | 1.910    | 1       | 297.9    |
|                                | orjson+jsonpath-ng | 4.713    | 1       | 297.9    |
|                                | orjson+jmespath    | 5.141    | 1       | 297.9    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.279    | 2000    | 286.9    |
|                                            | jmespath    | 0.852    | 2000    | 286.9    |
|                                            | jsonpath-ng | 15.902   | 2000    | 287.7    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.057    | 100999  | 287.7    |
|                                            | jmespath    | 62.583   | 2000    | 292.5    |
|                                            | jsonpath-ng | 247.591  | 100999  | 294.7    |
| $..price                                   | strata      | 8.220    | 100999  | 295.1    |
|                                            | jsonpath-ng | 888.422  | 100999  | 277.6    |
| $.users\[?(@.age>30)\]                     | strata      | 0.332    | 1606    | 275.7    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.985    | 6650    | 275.7    |
| flat:$.records\[\*\].id                    | strata      | 0.134    | 2000    | 277.1    |
|                                            | jmespath    | 0.715    | 2000    | 277.1    |
|                                            | jsonpath-ng | 4.331    | 2000    | 277.1    |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 277.1    |
|                                            | jmespath    | 0.018    | 1       | 277.1    |
|                                            | jsonpath-ng | 0.023    | 1       | 277.1    |
| nested:$.records\[\*\].id                  | strata      | 0.122    | 2000    | 277.1    |
|                                            | jmespath    | 0.684    | 2000    | 277.1    |
|                                            | jsonpath-ng | 7.847    | 2000    | 277.1    |
| nested:$.records\[0\]                      | strata      | 0.011    | 1       | 277.2    |
|                                            | jsonpath-ng | 0.019    | 1       | 277.2    |
|                                            | jmespath    | 0.020    | 1       | 277.2    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.121    | 2000    | 297.9    |
|                                            | jmespath    | 0.690    | 2000    | 297.9    |
|                                            | jsonpath-ng | 8.446    | 2000    | 297.9    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 297.9    |
|                                            | jsonpath-ng | 0.011    | 1       | 297.9    |
|                                            | jmespath    | 0.019    | 1       | 297.9    |
| mixed:$.records\[\*\].id                   | strata      | 0.145    | 2000    | 297.9    |
|                                            | jmespath    | 0.766    | 2000    | 297.9    |
|                                            | jsonpath-ng | 5.703    | 2000    | 297.9    |
| mixed:$.records\[0\]                       | strata      | 0.005    | 1       | 297.9    |
|                                            | jmespath    | 0.010    | 1       | 297.9    |
|                                            | jsonpath-ng | 0.010    | 1       | 297.9    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **8.9% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **25.9% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **0.7% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **32.2% faster** than #2 (orjson)  |
| dumps               | **#1** / 5              | **3.8% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **28.3% faster** than #2 (orjson)  |
| loads (flat)        | **#1** / 5              | **26.6% faster** than #2 (msgspec) |
| load (flat)         | **#1** / 5              | **39.5% faster** than #2 (msgspec) |
| dumps (flat)        | **#1** / 5              | **4.5% faster** than #2 (orjson)   |
| dump (flat)         | **#1** / 5              | **7.2% faster** than #2 (orjson)   |
| loads (nested)      | **#1** / 5              | **82.8% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **70.6% faster** than #2 (orjson)  |
| dumps (nested)      | **#3** / 5              | 8.4% behind #1 (msgspec)           |
| dump (nested)       | **#1** / 5              | **16.5% faster** than #2 (msgspec) |
| loads (wide_arrays) | **#1** / 5              | **0.1% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **8.9% faster** than #2 (orjson)   |
| dumps (wide_arrays) | **#1** / 5              | **10.7% faster** than #2 (msgspec) |
| dump (wide_arrays)  | **#1** / 5              | **11.3% faster** than #2 (msgspec) |
| loads (mixed)       | **#1** / 5              | **83.7% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **45.9% faster** than #2 (orjson)  |
| dumps (mixed)       | **#1** / 5              | **8.6% faster** than #2 (orjson)   |
| dump (mixed)        | **#2** / 5              | 9.2% behind #1 (orjson)            |
| search (JSONPath)   | **#1** in 11/11 queries | avg **738% faster**                |
| query (JSONPath)    | **#1** in 13/13 queries | -                                  |
