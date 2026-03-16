# Strata Benchmark Results

Generated: 2026-03-16 15:46:59

## Environment

- Commit: a676cc453da8a24953da243df7465313ffd5cbfd
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/large/users.json, benchmarks/data/generated/large/users.ndjson
- Random schemas: flat, nested, wide_arrays, mixed (5000 records each, seed=42)

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 6.293    | 7.309       | 7.608    | 510.7    |
| orjson        | flat         | 8.179    | 9.833       | 10.663   | 510.7    |
| msgspec       | flat         | 8.913    | 10.653      | 10.693   | 510.8    |
| ujson         | flat         | 14.682   | 16.277      | 16.656   | 510.8    |
| json (stdlib) | flat         | 23.965   | 24.267      | 24.692   | 513.7    |
| strata        | mixed        | 6.103    | 7.538       | 7.641    | 516.7    |
| orjson        | mixed        | 9.286    | 12.804      | 19.702   | 522.3    |
| msgspec       | mixed        | 14.303   | 16.522      | 16.742   | 522.3    |
| ujson         | mixed        | 14.347   | 16.935      | 17.568   | 522.3    |
| json (stdlib) | mixed        | 24.860   | 28.047      | 32.289   | 522.3    |
| strata        | nested       | 5.181    | 6.425       | 6.459    | 510.1    |
| orjson        | nested       | 13.679   | 17.401      | 18.311   | 510.1    |
| msgspec       | nested       | 14.524   | 18.411      | 18.493   | 510.1    |
| ujson         | nested       | 20.920   | 21.233      | 23.162   | 510.1    |
| json (stdlib) | nested       | 24.026   | 26.216      | 26.619   | 510.1    |
| strata        | users.json   | 150.872  | 193.570     | 202.197  | 340.3    |
| orjson        | users.json   | 182.733  | 261.044     | 281.900  | 478.3    |
| msgspec       | users.json   | 198.471  | 277.397     | 285.363  | 467.8    |
| ujson         | users.json   | 280.218  | 376.910     | 385.338  | 590.2    |
| json (stdlib) | users.json   | 387.722  | 476.165     | 476.675  | 511.7    |
| strata        | users.ndjson | 162.301  | 200.843     | 204.576  | 567.3    |
| orjson        | users.ndjson | 269.046  | 304.376     | 342.424  | 617.5    |
| msgspec       | users.ndjson | 277.603  | 314.839     | 318.327  | 612.0    |
| ujson         | users.ndjson | 358.605  | 450.637     | 451.368  | 734.5    |
| json (stdlib) | users.ndjson | 451.431  | 508.797     | 532.099  | 613.7    |
| strata        | wide_arrays  | 29.263   | 37.259      | 37.959   | 423.8    |
| orjson        | wide_arrays  | 28.442   | 37.806      | 44.570   | 449.4    |
| msgspec       | wide_arrays  | 36.452   | 48.883      | 49.539   | 451.4    |
| ujson         | wide_arrays  | 59.814   | 69.770      | 78.255   | 450.4    |
| json (stdlib) | wide_arrays  | 133.594  | 143.317     | 143.563  | 465.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 6.715    | 7.831       | 7.895    | 513.7    |
| msgspec       | flat         | 9.558    | 12.879      | 14.717   | 516.6    |
| orjson        | flat         | 11.803   | 13.304      | 13.452   | 513.7    |
| ujson         | flat         | 15.129   | 17.176      | 17.514   | 516.6    |
| json (stdlib) | flat         | 24.179   | 25.469      | 25.536   | 516.6    |
| strata        | mixed        | 6.395    | 7.895       | 7.982    | 522.3    |
| msgspec       | mixed        | 12.300   | 13.418      | 13.475   | 523.1    |
| orjson        | mixed        | 10.790   | 13.441      | 19.268   | 520.7    |
| ujson         | mixed        | 15.468   | 18.824      | 26.768   | 523.1    |
| json (stdlib) | mixed        | 28.340   | 32.420      | 32.908   | 523.1    |
| strata        | nested       | 5.229    | 6.670       | 6.766    | 510.1    |
| orjson        | nested       | 13.372   | 18.221      | 34.853   | 510.1    |
| msgspec       | nested       | 15.296   | 18.271      | 19.056   | 511.9    |
| ujson         | nested       | 21.773   | 21.815      | 24.779   | 511.9    |
| json (stdlib) | nested       | 22.104   | 27.084      | 35.511   | 511.9    |
| strata        | users.json   | 152.643  | 187.803     | 195.843  | 522.5    |
| orjson        | users.json   | 186.686  | 268.744     | 272.166  | 522.5    |
| msgspec       | users.json   | 235.228  | 296.137     | 312.058  | 555.7    |
| ujson         | users.json   | 296.659  | 409.700     | 412.796  | 678.2    |
| json (stdlib) | users.json   | 410.432  | 506.794     | 513.054  | 555.7    |
| strata        | users.ndjson | 165.989  | 205.438     | 209.689  | 617.6    |
| orjson        | users.ndjson | 250.170  | 315.147     | 344.382  | 617.7    |
| msgspec       | users.ndjson | 274.642  | 334.804     | 341.584  | 655.9    |
| ujson         | users.ndjson | 379.296  | 447.904     | 478.228  | 778.4    |
| json (stdlib) | users.ndjson | 460.473  | 525.836     | 537.378  | 658.6    |
| strata        | wide_arrays  | 30.318   | 37.627      | 39.343   | 467.4    |
| orjson        | wide_arrays  | 30.096   | 41.562      | 53.624   | 468.4    |
| msgspec       | wide_arrays  | 42.597   | 49.318      | 60.551   | 483.4    |
| ujson         | wide_arrays  | 70.077   | 76.000      | 76.613   | 482.4    |
| json (stdlib) | wide_arrays  | 140.754  | 152.785     | 154.968  | 484.4    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.558    | 2.611       | 2.641    | 516.6    |
| msgspec | flat        | 2.892    | 2.980       | 3.017    | 512.3    |
| orjson  | flat        | 2.898    | 3.068       | 3.361    | 512.3    |
| ujson   | flat        | 12.158   | 12.276      | 12.545   | 514.3    |
| json    | flat        | 22.862   | 22.865      | 23.100   | 514.3    |
| strata  | mixed       | 2.725    | 2.728       | 2.739    | 523.1    |
| msgspec | mixed       | 2.819    | 2.827       | 3.130    | 527.9    |
| orjson  | mixed       | 3.231    | 3.537       | 3.984    | 523.7    |
| ujson   | mixed       | 12.845   | 12.988      | 13.331   | 527.9    |
| json    | mixed       | 28.140   | 28.215      | 28.441   | 535.3    |
| strata  | nested      | 1.727    | 1.756       | 1.760    | 511.9    |
| orjson  | nested      | 1.918    | 1.951       | 2.096    | 511.9    |
| msgspec | nested      | 1.979    | 1.985       | 2.048    | 511.9    |
| ujson   | nested      | 9.866    | 9.983       | 10.171   | 511.9    |
| json    | nested      | 12.963   | 12.984      | 13.493   | 511.9    |
| strata  | users.json  | 52.425   | 52.594      | 59.206   | 591.2    |
| orjson  | users.json  | 80.128   | 80.320      | 82.863   | 747.5    |
| msgspec | users.json  | 80.568   | 81.426      | 83.680   | 944.5    |
| ujson   | users.json  | 296.506  | 297.030     | 298.783  | 1139.7   |
| json    | users.json  | 423.335  | 424.048     | 426.582  | 1216.4   |
| orjson  | wide_arrays | 16.064   | 16.213      | 16.488   | 478.5    |
| msgspec | wide_arrays | 17.373   | 17.557      | 17.574   | 475.8    |
| strata  | wide_arrays | 17.144   | 17.868      | 23.427   | 482.4    |
| ujson   | wide_arrays | 73.950   | 74.673      | 77.240   | 505.7    |
| json    | wide_arrays | 202.581  | 203.527     | 204.669  | 482.6    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 3.028    | 3.049       | 3.264    | 514.3    |
| orjson        | flat        | 3.563    | 3.567       | 4.003    | 514.1    |
| msgspec       | flat        | 3.618    | 3.654       | 3.660    | 514.1    |
| ujson         | flat        | 13.001   | 13.020      | 13.657   | 514.1    |
| json (stdlib) | flat        | 81.448   | 82.398      | 83.239   | 514.1    |
| strata        | mixed       | 3.416    | 3.493       | 3.622    | 535.3    |
| msgspec       | mixed       | 3.457    | 3.602       | 3.653    | 535.1    |
| orjson        | mixed       | 3.494    | 3.866       | 4.166    | 535.1    |
| ujson         | mixed       | 13.369   | 13.574      | 13.972   | 535.1    |
| json (stdlib) | mixed       | 95.771   | 97.146      | 97.513   | 535.1    |
| strata        | nested      | 2.126    | 2.177       | 2.324    | 511.9    |
| orjson        | nested      | 2.857    | 2.882       | 2.885    | 511.9    |
| msgspec       | nested      | 2.885    | 3.261       | 3.591    | 511.9    |
| ujson         | nested      | 10.349   | 10.916      | 11.088   | 511.9    |
| json (stdlib) | nested      | 119.306  | 119.517     | 119.723  | 511.9    |
| strata        | users.json  | 59.143   | 64.900      | 73.719   | 1260.2   |
| orjson        | users.json  | 90.312   | 101.488     | 115.804  | 1416.0   |
| msgspec       | users.json  | 110.238  | 111.732     | 116.105  | 1688.8   |
| ujson         | users.json  | 332.602  | 333.667     | 335.356  | 1571.9   |
| json (stdlib) | users.json  | 2996.088 | 3000.524    | 3002.133 | 1533.1   |
| strata        | wide_arrays | 18.976   | 19.243      | 26.514   | 482.6    |
| orjson        | wide_arrays | 22.863   | 23.167      | 24.555   | 499.7    |
| msgspec       | wide_arrays | 22.174   | 25.861      | 27.446   | 518.0    |
| ujson         | wide_arrays | 77.770   | 80.023      | 80.708   | 522.1    |
| json (stdlib) | wide_arrays | 451.623  | 457.729     | 461.895  | 518.0    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 62.378   | 4000    | 1332.8   |
|                                | orjson+jmespath    | 209.400  | 4000    | 1174.3   |
|                                | orjson+jsonpath-ng | 213.548  | 4000    | 1182.1   |
| all item prices                | strata             | 78.501   | 789913  | 1209.1   |
|                                | orjson+jmespath    | 552.985  | 4000    | 1270.0   |
|                                | orjson+jsonpath-ng | 1957.024 | 789913  | 1125.3   |
| recursive price                | strata             | 72.680   | 789913  | 1100.4   |
|                                | orjson+jsonpath-ng | 6295.278 | 789913  | 832.0    |
| flat:$.records\[\*\].id        | strata             | 3.815    | 5000    | 514.1    |
|                                | orjson+jmespath    | 11.668   | 5000    | 514.1    |
|                                | orjson+jsonpath-ng | 16.846   | 5000    | 511.1    |
| flat:$.records\[0\]            | strata             | 3.909    | 1       | 511.1    |
|                                | orjson+jmespath    | 9.318    | 1       | 511.1    |
|                                | orjson+jsonpath-ng | 9.609    | 1       | 511.1    |
| nested:$.records\[\*\].id      | strata             | 2.728    | 5000    | 511.9    |
|                                | orjson+jmespath    | 17.061   | 5000    | 511.9    |
|                                | orjson+jsonpath-ng | 22.763   | 5000    | 511.9    |
| nested:$.records\[0\]          | strata             | 2.652    | 1       | 511.9    |
|                                | orjson+jsonpath-ng | 15.660   | 1       | 511.9    |
|                                | orjson+jmespath    | 15.935   | 1       | 511.9    |
| wide_arrays:$.records\[\*\].id | strata             | 23.845   | 5000    | 518.0    |
|                                | orjson+jmespath    | 38.444   | 5000    | 518.0    |
|                                | orjson+jsonpath-ng | 44.184   | 5000    | 518.0    |
| wide_arrays:$.records\[0\]     | strata             | 23.594   | 1       | 518.0    |
|                                | orjson+jsonpath-ng | 34.735   | 1       | 518.0    |
|                                | orjson+jmespath    | 36.600   | 1       | 518.0    |
| mixed:$.records\[\*\].id       | strata             | 4.028    | 5000    | 535.1    |
|                                | orjson+jmespath    | 13.059   | 5000    | 535.1    |
|                                | orjson+jsonpath-ng | 18.865   | 5000    | 535.1    |
| mixed:$.records\[0\]           | strata             | 3.899    | 1       | 535.1    |
|                                | orjson+jsonpath-ng | 10.846   | 1       | 535.1    |
|                                | orjson+jmespath    | 10.941   | 1       | 535.1    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.755    | 4000    | 805.8    |
|                                            | jmespath    | 2.171    | 4000    | 805.8    |
|                                            | jsonpath-ng | 95.650   | 4000    | 807.5    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 48.096   | 789913  | 807.6    |
|                                            | jmespath    | 423.922  | 4000    | 822.4    |
|                                            | jsonpath-ng | 1716.205 | 789913  | 820.7    |
| $..price                                   | strata      | 55.966   | 789913  | 828.0    |
|                                            | jsonpath-ng | 6113.629 | 789913  | 826.8    |
| $.users\[?(@.age>30)\]                     | strata      | 0.871    | 3212    | 830.4    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 66.767   | 26406   | 830.4    |
| flat:$.records\[\*\].id                    | strata      | 0.354    | 5000    | 511.1    |
|                                            | jmespath    | 1.733    | 5000    | 511.1    |
|                                            | jsonpath-ng | 9.641    | 5000    | 511.1    |
| flat:$.records\[0\]                        | strata      | 0.007    | 1       | 511.1    |
|                                            | jsonpath-ng | 0.008    | 1       | 511.1    |
|                                            | jmespath    | 0.009    | 1       | 511.1    |
| nested:$.records\[\*\].id                  | strata      | 0.454    | 5000    | 511.9    |
|                                            | jmespath    | 1.836    | 5000    | 511.9    |
|                                            | jsonpath-ng | 16.628   | 5000    | 511.9    |
| nested:$.records\[0\]                      | strata      | 0.010    | 1       | 511.9    |
|                                            | jsonpath-ng | 0.010    | 1       | 511.9    |
|                                            | jmespath    | 0.013    | 1       | 511.9    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.259    | 5000    | 518.0    |
|                                            | jmespath    | 1.769    | 5000    | 518.0    |
|                                            | jsonpath-ng | 16.428   | 5000    | 518.0    |
| wide_arrays:$.records\[0\]                 | strata      | 0.005    | 1       | 518.0    |
|                                            | jsonpath-ng | 0.013    | 1       | 518.0    |
|                                            | jmespath    | 0.018    | 1       | 518.0    |
| mixed:$.records\[\*\].id                   | strata      | 0.316    | 5000    | 535.1    |
|                                            | jmespath    | 1.818    | 5000    | 535.1    |
|                                            | jsonpath-ng | 11.449   | 5000    | 535.1    |
| mixed:$.records\[0\]                       | strata      | 0.005    | 1       | 535.1    |
|                                            | jmespath    | 0.008    | 1       | 535.1    |
|                                            | jsonpath-ng | 0.010    | 1       | 535.1    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **21.1% faster** than #2 (orjson)  |
| loads (NDJSON)      | **#1** / 5              | **65.8% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **22.3% faster** than #2 (orjson)  |
| load (NDJSON file)  | **#1** / 5              | **50.7% faster** than #2 (orjson)  |
| dumps               | **#1** / 5              | **52.8% faster** than #2 (orjson)  |
| dump                | **#1** / 5              | **52.7% faster** than #2 (orjson)  |
| loads (flat)        | **#1** / 5              | **30.0% faster** than #2 (orjson)  |
| load (flat)         | **#1** / 5              | **42.3% faster** than #2 (msgspec) |
| dumps (flat)        | **#1** / 5              | **13.0% faster** than #2 (msgspec) |
| dump (flat)         | **#1** / 5              | **17.7% faster** than #2 (orjson)  |
| loads (nested)      | **#1** / 5              | **164.0% faster** than #2 (orjson) |
| load (nested)       | **#1** / 5              | **155.7% faster** than #2 (orjson) |
| dumps (nested)      | **#1** / 5              | **11.1% faster** than #2 (orjson)  |
| dump (nested)       | **#1** / 5              | **34.4% faster** than #2 (orjson)  |
| loads (wide_arrays) | **#2** / 5              | 2.9% behind #1 (orjson)            |
| load (wide_arrays)  | **#2** / 5              | 0.7% behind #1 (orjson)            |
| dumps (wide_arrays) | **#2** / 5              | 6.7% behind #1 (orjson)            |
| dump (wide_arrays)  | **#1** / 5              | **16.9% faster** than #2 (msgspec) |
| loads (mixed)       | **#1** / 5              | **52.2% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **68.7% faster** than #2 (orjson)  |
| dumps (mixed)       | **#1** / 5              | **3.5% faster** than #2 (msgspec)  |
| dump (mixed)        | **#1** / 5              | **1.2% faster** than #2 (msgspec)  |
| search (JSONPath)   | **#1** in 11/11 queries | avg **1025% faster**               |
| query (JSONPath)    | **#1** in 13/13 queries | -                                  |
