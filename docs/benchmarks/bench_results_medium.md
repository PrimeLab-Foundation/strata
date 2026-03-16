# Strata Benchmark Results

Generated: 2026-03-16 17:18:33

## Environment

- Commit: efd00fdc01e0951561d325921e36a0c1697c3ea1
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
| strata        | flat         | 2.483    | 2.981       | 3.019    | 270.3    |
| orjson        | flat         | 3.905    | 4.352       | 4.630    | 272.8    |
| msgspec       | flat         | 3.854    | 4.478       | 4.613    | 272.8    |
| ujson         | flat         | 6.474    | 6.989       | 7.331    | 272.8    |
| json (stdlib) | flat         | 10.406   | 10.585      | 10.663   | 272.8    |
| strata        | mixed        | 2.479    | 3.069       | 3.106    | 294.0    |
| orjson        | mixed        | 4.287    | 4.831       | 4.903    | 296.3    |
| msgspec       | mixed        | 4.712    | 5.371       | 5.424    | 296.3    |
| ujson         | mixed        | 6.314    | 7.482       | 8.135    | 296.3    |
| json (stdlib) | mixed        | 10.995   | 11.320      | 11.972   | 296.3    |
| strata        | nested       | 1.970    | 2.485       | 2.532    | 277.2    |
| orjson        | nested       | 4.750    | 5.842       | 6.082    | 279.6    |
| msgspec       | nested       | 4.615    | 6.366       | 6.409    | 279.6    |
| ujson         | nested       | 5.641    | 7.171       | 8.315    | 279.6    |
| json (stdlib) | nested       | 8.196    | 10.323      | 11.066   | 279.6    |
| strata        | users.json   | 20.584   | 26.995      | 34.357   | 73.4     |
| orjson        | users.json   | 26.068   | 37.808      | 38.553   | 93.3     |
| msgspec       | users.json   | 30.334   | 42.795      | 43.566   | 92.1     |
| ujson         | users.json   | 40.380   | 58.166      | 60.220   | 109.8    |
| json (stdlib) | users.json   | 55.175   | 69.874      | 72.004   | 98.4     |
| strata        | users.ndjson | 21.200   | 26.965      | 27.877   | 251.5    |
| orjson        | users.ndjson | 32.091   | 34.314      | 35.820   | 260.6    |
| msgspec       | users.ndjson | 33.703   | 36.172      | 37.064   | 261.8    |
| ujson         | users.ndjson | 43.944   | 48.864      | 51.635   | 260.8    |
| json (stdlib) | users.ndjson | 63.545   | 67.027      | 69.527   | 262.8    |
| strata        | wide_arrays  | 11.959   | 14.441      | 14.520   | 291.8    |
| orjson        | wide_arrays  | 11.561   | 15.121      | 15.238   | 302.0    |
| msgspec       | wide_arrays  | 14.800   | 19.867      | 19.884   | 302.0    |
| ujson         | wide_arrays  | 24.352   | 27.977      | 28.091   | 302.0    |
| json (stdlib) | wide_arrays  | 53.048   | 57.061      | 58.267   | 302.0    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 2.822    | 3.117       | 3.342    | 272.8    |
| orjson        | flat         | 3.846    | 4.512       | 4.733    | 272.8    |
| msgspec       | flat         | 3.913    | 4.657       | 4.893    | 273.2    |
| ujson         | flat         | 6.112    | 7.027       | 7.338    | 273.2    |
| json (stdlib) | flat         | 9.864    | 10.367      | 10.508   | 273.2    |
| strata        | mixed        | 2.794    | 3.446       | 3.979    | 296.3    |
| orjson        | mixed        | 4.181    | 5.102       | 5.362    | 296.3    |
| msgspec       | mixed        | 5.286    | 5.671       | 6.116    | 296.3    |
| ujson         | mixed        | 7.018    | 7.687       | 7.713    | 296.3    |
| json (stdlib) | mixed        | 11.010   | 11.665      | 12.023   | 296.3    |
| strata        | nested       | 2.258    | 2.584       | 2.630    | 279.6    |
| msgspec       | nested       | 4.971    | 6.031       | 6.253    | 279.6    |
| orjson        | nested       | 4.693    | 6.182       | 6.681    | 279.6    |
| ujson         | nested       | 6.372    | 8.096       | 8.240    | 279.6    |
| json (stdlib) | nested       | 8.712    | 9.639       | 9.802    | 279.6    |
| strata        | users.json   | 21.677   | 27.476      | 30.335   | 99.8     |
| msgspec       | users.json   | 28.549   | 41.153      | 42.758   | 104.7    |
| orjson        | users.json   | 28.192   | 41.581      | 41.865   | 99.8     |
| ujson         | users.json   | 40.983   | 57.664      | 60.983   | 122.4    |
| json (stdlib) | users.json   | 55.663   | 68.468      | 70.020   | 104.7    |
| strata        | users.ndjson | 22.446   | 28.600      | 29.399   | 264.8    |
| orjson        | users.ndjson | 33.712   | 35.545      | 36.549   | 265.8    |
| msgspec       | users.ndjson | 34.355   | 36.512      | 37.353   | 266.8    |
| ujson         | users.ndjson | 49.622   | 53.250      | 56.555   | 265.8    |
| json (stdlib) | users.ndjson | 64.707   | 66.769      | 67.803   | 267.8    |
| strata        | wide_arrays  | 12.942   | 15.610      | 49.303   | 302.1    |
| orjson        | wide_arrays  | 12.872   | 16.210      | 19.644   | 301.7    |
| msgspec       | wide_arrays  | 18.278   | 19.887      | 20.275   | 301.7    |
| ujson         | wide_arrays  | 27.680   | 29.609      | 29.958   | 301.7    |
| json (stdlib) | wide_arrays  | 57.538   | 58.636      | 108.655  | 301.7    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 1.014    | 1.021       | 1.028    | 273.2    |
| orjson  | flat        | 1.011    | 1.039       | 1.109    | 275.5    |
| msgspec | flat        | 1.159    | 1.172       | 1.225    | 275.6    |
| ujson   | flat        | 4.947    | 5.343       | 5.502    | 277.2    |
| json    | flat        | 9.055    | 9.494       | 9.576    | 277.2    |
| orjson  | mixed       | 1.085    | 1.086       | 1.118    | 296.3    |
| msgspec | mixed       | 1.183    | 1.191       | 1.258    | 296.3    |
| strata  | mixed       | 1.208    | 1.234       | 1.263    | 296.3    |
| ujson   | mixed       | 5.206    | 5.308       | 5.334    | 296.3    |
| json    | mixed       | 11.789   | 11.935      | 12.124   | 296.3    |
| orjson  | nested      | 0.780    | 0.783       | 0.810    | 279.6    |
| msgspec | nested      | 0.796    | 0.817       | 0.824    | 279.6    |
| strata  | nested      | 0.783    | 0.826       | 0.848    | 279.6    |
| ujson   | nested      | 4.037    | 4.061       | 4.164    | 279.6    |
| json    | nested      | 5.259    | 5.292       | 5.480    | 279.6    |
| strata  | users.json  | 7.530    | 7.565       | 7.685    | 108.9    |
| msgspec | users.json  | 10.791   | 10.794      | 10.866   | 161.0    |
| orjson  | users.json  | 10.871   | 10.906      | 10.967   | 131.2    |
| ujson   | users.json  | 41.087   | 41.105      | 41.937   | 166.8    |
| json    | users.json  | 59.319   | 59.743      | 60.722   | 192.3    |
| msgspec | wide_arrays | 6.859    | 6.874       | 6.914    | 314.2    |
| strata  | wide_arrays | 6.869    | 7.085       | 9.010    | 304.1    |
| orjson  | wide_arrays | 7.133    | 7.138       | 7.306    | 294.5    |
| ujson   | wide_arrays | 29.752   | 31.377      | 31.706   | 316.2    |
| json    | wide_arrays | 83.826   | 89.430      | 105.479  | 337.1    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.313    | 1.321       | 1.340    | 277.2    |
| orjson        | flat        | 1.358    | 1.429       | 1.557    | 277.2    |
| msgspec       | flat        | 1.570    | 1.572       | 1.736    | 277.2    |
| ujson         | flat        | 5.344    | 5.461       | 5.845    | 277.2    |
| json (stdlib) | flat        | 32.467   | 32.687      | 33.074   | 277.2    |
| orjson        | mixed       | 1.420    | 1.446       | 1.544    | 296.3    |
| strata        | mixed       | 1.466    | 1.500       | 1.566    | 296.3    |
| msgspec       | mixed       | 1.550    | 1.556       | 1.562    | 296.3    |
| ujson         | mixed       | 5.679    | 5.746       | 5.747    | 296.3    |
| json (stdlib) | mixed       | 38.582   | 39.278      | 39.994   | 296.3    |
| orjson        | nested      | 1.044    | 1.049       | 1.102    | 279.6    |
| strata        | nested      | 1.024    | 1.072       | 1.108    | 279.6    |
| msgspec       | nested      | 1.061    | 1.123       | 1.129    | 279.6    |
| ujson         | nested      | 4.280    | 4.316       | 4.337    | 279.6    |
| json (stdlib) | nested      | 47.857   | 47.865      | 47.961   | 279.6    |
| strata        | users.json  | 8.462    | 8.465       | 8.542    | 198.5    |
| msgspec       | users.json  | 11.561   | 11.574      | 11.640   | 243.4    |
| orjson        | users.json  | 11.711   | 11.753      | 11.993   | 220.9    |
| ujson         | users.json  | 42.414   | 42.442      | 42.549   | 221.1    |
| json (stdlib) | users.json  | 430.661  | 444.229     | 444.584  | 243.5    |
| strata        | wide_arrays | 7.628    | 7.683       | 13.823   | 337.1    |
| msgspec       | wide_arrays | 8.112    | 8.262       | 8.512    | 356.5    |
| orjson        | wide_arrays | 8.336    | 8.551       | 8.903    | 334.8    |
| ujson         | wide_arrays | 30.345   | 30.702      | 31.620   | 336.8    |
| json (stdlib) | wide_arrays | 178.528  | 179.568     | 179.614  | 356.5    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 9.798    | 2000    | 240.5    |
|                                | orjson+jmespath    | 32.944   | 2000    | 243.7    |
|                                | orjson+jsonpath-ng | 35.562   | 2000    | 245.8    |
| all item prices                | strata             | 11.005   | 100999  | 247.6    |
|                                | orjson+jmespath    | 86.208   | 2000    | 256.8    |
|                                | orjson+jsonpath-ng | 256.288  | 100999  | 262.9    |
| recursive price                | strata             | 10.859   | 100999  | 259.9    |
|                                | orjson+jsonpath-ng | 900.406  | 100999  | 265.4    |
| flat:$.records\[\*\].id        | strata             | 1.520    | 2000    | 277.2    |
|                                | orjson+jmespath    | 4.604    | 2000    | 277.2    |
|                                | orjson+jsonpath-ng | 7.033    | 2000    | 277.2    |
| flat:$.records\[0\]            | strata             | 1.428    | 1       | 277.2    |
|                                | orjson+jsonpath-ng | 4.017    | 1       | 277.2    |
|                                | orjson+jmespath    | 4.087    | 1       | 277.2    |
| nested:$.records\[\*\].id      | strata             | 1.105    | 2000    | 279.6    |
|                                | orjson+jmespath    | 6.299    | 2000    | 279.6    |
|                                | orjson+jsonpath-ng | 8.842    | 2000    | 279.6    |
| nested:$.records\[0\]          | strata             | 1.122    | 1       | 279.6    |
|                                | orjson+jmespath    | 5.079    | 1       | 279.6    |
|                                | orjson+jsonpath-ng | 5.307    | 1       | 279.6    |
| wide_arrays:$.records\[\*\].id | strata             | 9.514    | 2000    | 356.5    |
|                                | orjson+jmespath    | 14.895   | 2000    | 356.5    |
|                                | orjson+jsonpath-ng | 17.547   | 2000    | 356.5    |
| wide_arrays:$.records\[0\]     | strata             | 9.355    | 1       | 356.5    |
|                                | orjson+jsonpath-ng | 14.113   | 1       | 294.0    |
|                                | orjson+jmespath    | 14.732   | 1       | 356.5    |
| mixed:$.records\[\*\].id       | strata             | 1.565    | 2000    | 296.3    |
|                                | orjson+jmespath    | 5.497    | 2000    | 296.3    |
|                                | orjson+jsonpath-ng | 7.515    | 2000    | 296.3    |
| mixed:$.records\[0\]           | strata             | 1.562    | 1       | 296.3    |
|                                | orjson+jmespath    | 4.444    | 1       | 296.3    |
|                                | orjson+jsonpath-ng | 4.500    | 1       | 296.3    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.293    | 2000    | 287.6    |
|                                            | jmespath    | 0.914    | 2000    | 287.6    |
|                                            | jsonpath-ng | 16.799   | 2000    | 288.4    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 6.168    | 100999  | 288.4    |
|                                            | jmespath    | 64.366   | 2000    | 292.2    |
|                                            | jsonpath-ng | 242.900  | 100999  | 294.5    |
| $..price                                   | strata      | 8.555    | 100999  | 294.2    |
|                                            | jsonpath-ng | 852.484  | 100999  | 296.6    |
| $.users\[?(@.age>30)\]                     | strata      | 0.304    | 1606    | 274.8    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 8.578    | 6650    | 274.8    |
| flat:$.records\[\*\].id                    | strata      | 0.122    | 2000    | 277.2    |
|                                            | jmespath    | 0.666    | 2000    | 277.2    |
|                                            | jsonpath-ng | 4.242    | 2000    | 277.2    |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 277.2    |
|                                            | jmespath    | 0.006    | 1       | 277.2    |
|                                            | jsonpath-ng | 0.007    | 1       | 277.2    |
| nested:$.records\[\*\].id                  | strata      | 0.099    | 2000    | 279.7    |
|                                            | jmespath    | 0.740    | 2000    | 279.7    |
|                                            | jsonpath-ng | 5.239    | 2000    | 279.7    |
| nested:$.records\[0\]                      | strata      | 0.003    | 1       | 279.7    |
|                                            | jsonpath-ng | 0.008    | 1       | 279.7    |
|                                            | jmespath    | 0.013    | 1       | 279.7    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.112    | 2000    | 294.0    |
|                                            | jmespath    | 0.723    | 2000    | 294.0    |
|                                            | jsonpath-ng | 6.983    | 2000    | 294.0    |
| wide_arrays:$.records\[0\]                 | strata      | 0.012    | 1       | 294.0    |
|                                            | jsonpath-ng | 0.020    | 1       | 294.0    |
|                                            | jmespath    | 0.020    | 1       | 294.0    |
| mixed:$.records\[\*\].id                   | strata      | 0.123    | 2000    | 296.3    |
|                                            | jmespath    | 0.702    | 2000    | 296.3    |
|                                            | jsonpath-ng | 5.317    | 2000    | 296.3    |
| mixed:$.records\[0\]                       | strata      | 0.009    | 1       | 296.3    |
|                                            | jmespath    | 0.017    | 1       | 296.3    |
|                                            | jsonpath-ng | 0.019    | 1       | 296.3    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **26.6% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **51.4% faster** than #2 (orjson)   |
| load (JSON file)    | **#1** / 5              | **30.1% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **50.2% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **43.3% faster** than #2 (msgspec)  |
| dump                | **#1** / 5              | **36.6% faster** than #2 (msgspec)  |
| loads (flat)        | **#1** / 5              | **55.2% faster** than #2 (msgspec)  |
| load (flat)         | **#1** / 5              | **36.3% faster** than #2 (orjson)   |
| dumps (flat)        | **#2** / 5              | 0.3% behind #1 (orjson)             |
| dump (flat)         | **#1** / 5              | **3.4% faster** than #2 (orjson)    |
| loads (nested)      | **#1** / 5              | **134.2% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **107.9% faster** than #2 (orjson)  |
| dumps (nested)      | **#2** / 5              | 0.3% behind #1 (orjson)             |
| dump (nested)       | **#1** / 5              | **2.0% faster** than #2 (orjson)    |
| loads (wide_arrays) | **#2** / 5              | 3.4% behind #1 (orjson)             |
| load (wide_arrays)  | **#2** / 5              | 0.5% behind #1 (orjson)             |
| dumps (wide_arrays) | **#2** / 5              | 0.2% behind #1 (msgspec)            |
| dump (wide_arrays)  | **#1** / 5              | **6.3% faster** than #2 (msgspec)   |
| loads (mixed)       | **#1** / 5              | **72.9% faster** than #2 (orjson)   |
| load (mixed)        | **#1** / 5              | **49.6% faster** than #2 (orjson)   |
| dumps (mixed)       | **#3** / 5              | 11.4% behind #1 (orjson)            |
| dump (mixed)        | **#2** / 5              | 3.3% behind #1 (orjson)             |
| search (JSONPath)   | **#1** in 11/11 queries | avg **987% faster**                 |
| query (JSONPath)    | **#1** in 13/13 queries | -                                   |
