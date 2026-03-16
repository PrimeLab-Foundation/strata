# Strata Benchmark Results

Generated: 2026-03-16 21:41:45

## Environment

- Commit: c0e3b5a0f341ed8571ff3e143865c7dff53226aa
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
| strata        | flat         | 2.483    | 3.054       | 3.054    | 271.4    |
| orjson        | flat         | 3.895    | 4.302       | 4.341    | 273.9    |
| msgspec       | flat         | 3.951    | 4.373       | 6.808    | 273.9    |
| ujson         | flat         | 6.150    | 6.750       | 7.049    | 273.9    |
| json (stdlib) | flat         | 10.690   | 11.033      | 12.033   | 273.9    |
| strata        | mixed        | 2.439    | 3.042       | 3.096    | 298.1    |
| orjson        | mixed        | 4.134    | 5.027       | 5.219    | 300.4    |
| msgspec       | mixed        | 4.798    | 5.308       | 5.737    | 300.4    |
| ujson         | mixed        | 7.775    | 7.947       | 8.912    | 300.4    |
| json (stdlib) | mixed        | 10.720   | 11.072      | 11.721   | 300.4    |
| strata        | nested       | 2.105    | 2.501       | 2.643    | 278.5    |
| orjson        | nested       | 4.952    | 6.201       | 6.303    | 280.9    |
| msgspec       | nested       | 6.258    | 7.059       | 7.947    | 280.9    |
| json (stdlib) | nested       | 8.572    | 10.869      | 11.428   | 280.9    |
| ujson         | nested       | 6.990    | 11.861      | 12.694   | 280.9    |
| strata        | users.json   | 21.791   | 27.069      | 27.608   | 73.7     |
| orjson        | users.json   | 31.167   | 40.394      | 48.626   | 93.6     |
| msgspec       | users.json   | 29.343   | 49.270      | 55.600   | 92.4     |
| ujson         | users.json   | 44.640   | 61.210      | 67.625   | 110.1    |
| json (stdlib) | users.json   | 65.867   | 69.798      | 82.107   | 98.7     |
| strata        | users.ndjson | 21.545   | 27.507      | 28.367   | 253.7    |
| msgspec       | users.ndjson | 37.590   | 37.702      | 43.217   | 262.9    |
| orjson        | users.ndjson | 35.334   | 38.459      | 39.676   | 261.7    |
| ujson         | users.ndjson | 47.802   | 53.105      | 53.545   | 261.9    |
| json (stdlib) | users.ndjson | 66.047   | 67.042      | 69.939   | 263.9    |
| strata        | wide_arrays  | 11.629   | 14.220      | 14.476   | 293.1    |
| orjson        | wide_arrays  | 11.876   | 15.507      | 21.057   | 303.4    |
| msgspec       | wide_arrays  | 16.697   | 19.070      | 20.536   | 303.4    |
| ujson         | wide_arrays  | 25.501   | 28.458      | 28.644   | 303.4    |
| json (stdlib) | wide_arrays  | 52.063   | 55.245      | 55.931   | 303.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 2.744    | 3.119       | 3.174    | 273.9    |
| orjson        | flat         | 4.189    | 4.594       | 5.518    | 273.9    |
| msgspec       | flat         | 4.475    | 4.810       | 5.005    | 274.7    |
| ujson         | flat         | 6.616    | 7.453       | 7.529    | 274.7    |
| json (stdlib) | flat         | 11.039   | 11.203      | 11.450   | 274.7    |
| strata        | mixed        | 3.149    | 3.406       | 3.547    | 300.4    |
| orjson        | mixed        | 4.536    | 5.703       | 8.965    | 300.4    |
| msgspec       | mixed        | 4.537    | 5.832       | 6.245    | 300.4    |
| ujson         | mixed        | 8.728    | 9.915       | 11.600   | 300.4    |
| json (stdlib) | mixed        | 10.943   | 11.750      | 12.087   | 300.4    |
| strata        | nested       | 2.157    | 2.809       | 3.373    | 280.9    |
| msgspec       | nested       | 5.924    | 6.187       | 11.842   | 280.9    |
| orjson        | nested       | 5.410    | 6.258       | 6.773    | 280.9    |
| ujson         | nested       | 6.347    | 8.133       | 8.691    | 280.9    |
| json (stdlib) | nested       | 9.416    | 10.810      | 12.213   | 280.9    |
| strata        | users.json   | 21.251   | 27.302      | 28.221   | 100.1    |
| orjson        | users.json   | 36.283   | 41.268      | 41.323   | 100.1    |
| msgspec       | users.json   | 29.360   | 42.747      | 49.514   | 105.0    |
| ujson         | users.json   | 44.318   | 59.956      | 66.109   | 122.7    |
| json (stdlib) | users.json   | 56.209   | 73.094      | 76.442   | 105.0    |
| strata        | users.ndjson | 22.967   | 30.955      | 31.365   | 265.9    |
| orjson        | users.ndjson | 33.858   | 36.732      | 43.155   | 266.9    |
| msgspec       | users.ndjson | 36.569   | 36.836      | 45.573   | 267.9    |
| ujson         | users.ndjson | 48.054   | 56.506      | 57.948   | 266.9    |
| json (stdlib) | users.ndjson | 67.200   | 68.872      | 70.130   | 268.9    |
| strata        | wide_arrays  | 13.191   | 15.350      | 15.955   | 303.4    |
| orjson        | wide_arrays  | 12.918   | 16.446      | 17.143   | 303.0    |
| msgspec       | wide_arrays  | 16.538   | 22.180      | 27.210   | 303.0    |
| ujson         | wide_arrays  | 28.159   | 30.820      | 30.910   | 303.0    |
| json (stdlib) | wide_arrays  | 54.920   | 57.081      | 58.387   | 303.0    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson  | flat        | 0.984    | 1.011       | 1.101    | 277.0    |
| strata  | flat        | 1.058    | 1.066       | 1.215    | 274.7    |
| msgspec | flat        | 1.146    | 1.154       | 1.157    | 277.0    |
| ujson   | flat        | 4.854    | 4.871       | 4.931    | 278.5    |
| json    | flat        | 9.220    | 9.391       | 9.630    | 278.5    |
| strata  | mixed       | 1.190    | 1.207       | 1.214    | 300.4    |
| msgspec | mixed       | 1.181    | 1.210       | 1.248    | 300.4    |
| orjson  | mixed       | 1.306    | 1.369       | 1.453    | 300.4    |
| ujson   | mixed       | 5.177    | 5.233       | 5.314    | 300.4    |
| json    | mixed       | 11.555   | 11.669      | 11.703   | 300.4    |
| orjson  | nested      | 0.782    | 0.793       | 0.967    | 280.9    |
| msgspec | nested      | 0.798    | 0.812       | 0.830    | 280.9    |
| strata  | nested      | 0.815    | 0.816       | 0.841    | 280.9    |
| ujson   | nested      | 4.047    | 4.069       | 4.089    | 280.9    |
| json    | nested      | 5.290    | 5.359       | 5.372    | 280.9    |
| strata  | users.json  | 7.799    | 7.815       | 8.073    | 109.1    |
| msgspec | users.json  | 10.820   | 10.886      | 11.102   | 159.8    |
| orjson  | users.json  | 11.029   | 11.250      | 12.353   | 131.5    |
| ujson   | users.json  | 41.017   | 41.146      | 41.604   | 165.6    |
| json    | users.json  | 59.677   | 59.789      | 61.029   | 190.9    |
| msgspec | wide_arrays | 6.995    | 7.022       | 7.035    | 315.6    |
| strata  | wide_arrays | 7.392    | 7.408       | 7.428    | 305.4    |
| orjson  | wide_arrays | 7.288    | 7.499       | 8.360    | 295.8    |
| ujson   | wide_arrays | 28.773   | 29.222      | 29.427   | 317.5    |
| json    | wide_arrays | 81.456   | 81.711      | 81.838   | 338.4    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| orjson        | flat        | 1.408    | 1.477       | 1.853    | 278.5    |
| strata        | flat        | 1.455    | 1.510       | 1.673    | 278.5    |
| msgspec       | flat        | 1.597    | 1.995       | 2.001    | 278.5    |
| ujson         | flat        | 5.209    | 5.289       | 5.316    | 278.5    |
| json (stdlib) | flat        | 32.953   | 33.084      | 33.554   | 278.5    |
| orjson        | mixed       | 1.370    | 1.448       | 1.572    | 300.4    |
| strata        | mixed       | 1.408    | 1.463       | 1.930    | 300.4    |
| msgspec       | mixed       | 1.455    | 1.551       | 1.676    | 300.4    |
| ujson         | mixed       | 5.608    | 5.620       | 5.911    | 300.4    |
| json (stdlib) | mixed       | 39.604   | 39.818      | 40.131   | 300.4    |
| strata        | nested      | 1.073    | 1.077       | 1.211    | 280.9    |
| orjson        | nested      | 1.076    | 1.123       | 1.151    | 280.9    |
| msgspec       | nested      | 1.075    | 1.139       | 1.163    | 280.9    |
| ujson         | nested      | 4.221    | 4.297       | 4.333    | 280.9    |
| json (stdlib) | nested      | 47.423   | 47.809      | 48.741   | 280.9    |
| strata        | users.json  | 9.059    | 9.285       | 10.030   | 197.2    |
| orjson        | users.json  | 12.192   | 12.591      | 12.765   | 219.5    |
| msgspec       | users.json  | 11.676   | 12.666      | 14.670   | 244.0    |
| ujson         | users.json  | 44.198   | 44.279      | 44.316   | 221.6    |
| json (stdlib) | users.json  | 435.192  | 440.499     | 444.364  | 244.0    |
| strata        | wide_arrays | 8.283    | 8.322       | 8.370    | 338.4    |
| msgspec       | wide_arrays | 8.747    | 9.022       | 9.093    | 298.1    |
| orjson        | wide_arrays | 7.966    | 9.323       | 10.585   | 336.2    |
| ujson         | wide_arrays | 29.913   | 30.275      | 32.891   | 278.3    |
| json (stdlib) | wide_arrays | 180.867  | 181.725     | 183.361  | 298.1    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 8.011    | 2000    | 241.0    |
|                                | orjson+jmespath    | 30.549   | 2000    | 244.2    |
|                                | orjson+jsonpath-ng | 33.883   | 2000    | 246.4    |
| all item prices                | strata             | 9.503    | 100999  | 247.2    |
|                                | orjson+jmespath    | 84.259   | 2000    | 256.4    |
|                                | orjson+jsonpath-ng | 263.330  | 100999  | 262.3    |
| recursive price                | strata             | 9.935    | 100999  | 261.1    |
|                                | orjson+jsonpath-ng | 889.613  | 100999  | 265.6    |
| flat:$.records\[\*\].id        | strata             | 1.322    | 2000    | 278.5    |
|                                | orjson+jmespath    | 4.976    | 2000    | 278.5    |
|                                | orjson+jsonpath-ng | 7.485    | 2000    | 278.5    |
| flat:$.records\[0\]            | strata             | 1.326    | 1       | 278.5    |
|                                | orjson+jsonpath-ng | 4.200    | 1       | 278.5    |
|                                | orjson+jmespath    | 4.578    | 1       | 278.5    |
| nested:$.records\[\*\].id      | strata             | 1.140    | 2000    | 280.9    |
|                                | orjson+jmespath    | 5.701    | 2000    | 280.9    |
|                                | orjson+jsonpath-ng | 8.009    | 2000    | 280.9    |
| nested:$.records\[0\]          | strata             | 0.998    | 1       | 281.0    |
|                                | orjson+jmespath    | 5.304    | 1       | 281.0    |
|                                | orjson+jsonpath-ng | 6.230    | 1       | 281.0    |
| wide_arrays:$.records\[\*\].id | strata             | 9.425    | 2000    | 298.1    |
|                                | orjson+jmespath    | 14.855   | 2000    | 298.1    |
|                                | orjson+jsonpath-ng | 18.584   | 2000    | 298.1    |
| wide_arrays:$.records\[0\]     | strata             | 9.474    | 1       | 298.1    |
|                                | orjson+jsonpath-ng | 14.527   | 1       | 298.1    |
|                                | orjson+jmespath    | 15.172   | 1       | 298.1    |
| mixed:$.records\[\*\].id       | strata             | 1.540    | 2000    | 300.4    |
|                                | orjson+jmespath    | 5.422    | 2000    | 300.4    |
|                                | orjson+jsonpath-ng | 7.718    | 2000    | 300.4    |
| mixed:$.records\[0\]           | strata             | 1.579    | 1       | 300.4    |
|                                | orjson+jsonpath-ng | 4.670    | 1       | 300.4    |
|                                | orjson+jmespath    | 4.863    | 1       | 300.4    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.289    | 2000    | 287.8    |
|                                            | jmespath    | 0.937    | 2000    | 287.8    |
|                                            | jsonpath-ng | 16.425   | 2000    | 288.6    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.107    | 100999  | 288.7    |
|                                            | jmespath    | 62.774   | 2000    | 292.5    |
|                                            | jsonpath-ng | 253.733  | 100999  | 294.8    |
| $..price                                   | strata      | 8.160    | 100999  | 294.5    |
|                                            | jsonpath-ng | 887.079  | 100999  | 277.0    |
| $.users\[?(@.age>30)\]                     | strata      | 0.358    | 1606    | 275.1    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.266    | 6650    | 275.1    |
| flat:$.records\[\*\].id                    | strata      | 0.126    | 2000    | 278.5    |
|                                            | jmespath    | 0.676    | 2000    | 278.5    |
|                                            | jsonpath-ng | 4.707    | 2000    | 278.5    |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 278.5    |
|                                            | jmespath    | 0.007    | 1       | 278.5    |
|                                            | jsonpath-ng | 0.007    | 1       | 278.5    |
| nested:$.records\[\*\].id                  | strata      | 0.108    | 2000    | 281.0    |
|                                            | jmespath    | 0.682    | 2000    | 281.0    |
|                                            | jsonpath-ng | 6.098    | 2000    | 281.0    |
| nested:$.records\[0\]                      | jmespath    | 0.007    | 1       | 281.0    |
|                                            | strata      | 0.008    | 1       | 281.0    |
|                                            | jsonpath-ng | 0.008    | 1       | 281.0    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.107    | 2000    | 298.1    |
|                                            | jmespath    | 0.723    | 2000    | 298.1    |
|                                            | jsonpath-ng | 7.196    | 2000    | 298.1    |
| wide_arrays:$.records\[0\]                 | strata      | 0.004    | 1       | 298.1    |
|                                            | jmespath    | 0.008    | 1       | 298.1    |
|                                            | jsonpath-ng | 0.011    | 1       | 298.1    |
| mixed:$.records\[\*\].id                   | strata      | 0.114    | 2000    | 300.4    |
|                                            | jmespath    | 0.677    | 2000    | 300.4    |
|                                            | jsonpath-ng | 5.992    | 2000    | 300.4    |
| mixed:$.records\[0\]                       | strata      | 0.003    | 1       | 300.4    |
|                                            | jmespath    | 0.007    | 1       | 300.4    |
|                                            | jsonpath-ng | 0.007    | 1       | 300.4    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **34.7% faster** than #2 (msgspec) |
| loads (NDJSON)      | **#1** / 5              | **64.0% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **38.2% faster** than #2 (msgspec) |
| load (NDJSON file)  | **#1** / 5              | **47.4% faster** than #2 (orjson)  |
| dumps               | **#1** / 5              | **38.7% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **28.9% faster** than #2 (msgspec) |
| loads (flat)        | **#1** / 5              | **56.8% faster** than #2 (orjson)  |
| load (flat)         | **#1** / 5              | **52.7% faster** than #2 (orjson)  |
| dumps (flat)        | **#2** / 5              | 7.6% behind #1 (orjson)            |
| dump (flat)         | **#2** / 5              | 3.3% behind #1 (orjson)            |
| loads (nested)      | **#1** / 5              | **135.2% faster** than #2 (orjson) |
| load (nested)       | **#1** / 5              | **150.8% faster** than #2 (orjson) |
| dumps (nested)      | **#3** / 5              | 4.1% behind #1 (orjson)            |
| dump (nested)       | **#1** / 5              | **0.3% faster** than #2 (msgspec)  |
| loads (wide_arrays) | **#1** / 5              | **2.1% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#2** / 5              | 2.1% behind #1 (orjson)            |
| dumps (wide_arrays) | **#3** / 5              | 5.7% behind #1 (msgspec)           |
| dump (wide_arrays)  | **#2** / 5              | 4.0% behind #1 (orjson)            |
| loads (mixed)       | **#1** / 5              | **69.5% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **44.1% faster** than #2 (orjson)  |
| dumps (mixed)       | **#2** / 5              | 0.8% behind #1 (msgspec)           |
| dump (mixed)        | **#2** / 5              | 2.8% behind #1 (orjson)            |
| search (JSONPath)   | **#1** in 11/11 queries | avg **1073% faster**               |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
