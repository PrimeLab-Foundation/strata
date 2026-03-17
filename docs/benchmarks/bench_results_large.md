# Strata Benchmark Results

Generated: 2026-03-17 23:14:48

## Environment

- Commit: 424c79cc06825213a77b7bb3e900fe94cce6309b
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
| strata        | flat         | 7.262    | 8.444       | 8.600    | 527.2    |
| msgspec       | flat         | 9.356    | 10.543      | 11.917   | 527.3    |
| orjson        | flat         | 9.815    | 11.660      | 15.005   | 527.2    |
| ujson         | flat         | 17.115   | 17.481      | 19.234   | 527.3    |
| json (stdlib) | flat         | 23.094   | 25.910      | 33.311   | 530.2    |
| strata        | mixed        | 7.210    | 8.514       | 10.698   | 831.7    |
| orjson        | mixed        | 12.013   | 16.358      | 16.587   | 837.3    |
| msgspec       | mixed        | 14.442   | 17.882      | 17.925   | 837.3    |
| ujson         | mixed        | 18.872   | 22.598      | 24.754   | 837.3    |
| json (stdlib) | mixed        | 26.378   | 35.148      | 42.591   | 839.7    |
| strata        | nested       | 7.372    | 7.640       | 9.089    | 525.7    |
| msgspec       | nested       | 14.654   | 21.240      | 23.031   | 525.7    |
| orjson        | nested       | 16.903   | 22.063      | 26.056   | 525.7    |
| ujson         | nested       | 16.067   | 22.843      | 23.441   | 525.7    |
| json (stdlib) | nested       | 25.433   | 30.980      | 44.705   | 525.7    |
| strata        | users.json   | 178.722  | 222.392     | 233.441  | 340.2    |
| orjson        | users.json   | 214.576  | 289.453     | 295.889  | 478.2    |
| msgspec       | users.json   | 303.702  | 362.451     | 370.623  | 467.7    |
| ujson         | users.json   | 286.652  | 428.509     | 604.341  | 590.1    |
| json (stdlib) | users.json   | 445.537  | 550.089     | 550.209  | 511.6    |
| strata        | users.ndjson | 201.517  | 250.845     | 260.490  | 582.5    |
| orjson        | users.ndjson | 266.749  | 337.816     | 374.727  | 632.9    |
| msgspec       | users.ndjson | 289.995  | 354.758     | 382.763  | 627.4    |
| ujson         | users.ndjson | 354.754  | 467.345     | 510.102  | 749.9    |
| json (stdlib) | users.ndjson | 517.908  | 547.981     | 564.949  | 630.1    |
| strata        | wide_arrays  | 39.015   | 41.718      | 47.361   | 427.3    |
| orjson        | wide_arrays  | 29.017   | 41.902      | 42.647   | 452.9    |
| msgspec       | wide_arrays  | 42.236   | 47.812      | 53.349   | 453.9    |
| ujson         | wide_arrays  | 66.558   | 81.717      | 86.573   | 453.0    |
| json (stdlib) | wide_arrays  | 139.409  | 141.401     | 148.998  | 467.9    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 7.702    | 8.596       | 8.739    | 530.2    |
| orjson        | flat         | 9.156    | 10.434      | 14.508   | 530.2    |
| msgspec       | flat         | 11.359   | 11.439      | 12.866   | 533.2    |
| json (stdlib) | flat         | 29.562   | 31.203      | 34.804   | 533.2    |
| ujson         | flat         | 15.039   | 31.621      | 46.468   | 533.2    |
| strata        | mixed        | 9.265    | 9.622       | 10.366   | 839.7    |
| orjson        | mixed        | 10.372   | 13.605      | 26.442   | 839.7    |
| msgspec       | mixed        | 11.916   | 17.940      | 21.548   | 842.1    |
| ujson         | mixed        | 16.975   | 19.638      | 25.504   | 842.1    |
| json (stdlib) | mixed        | 26.834   | 30.810      | 42.769   | 842.1    |
| strata        | nested       | 6.905    | 8.329       | 8.586    | 525.7    |
| orjson        | nested       | 15.483   | 19.332      | 19.947   | 525.7    |
| msgspec       | nested       | 19.918   | 21.731      | 23.337   | 529.2    |
| ujson         | nested       | 20.636   | 24.486      | 26.583   | 529.2    |
| json (stdlib) | nested       | 29.815   | 31.508      | 36.264   | 529.3    |
| strata        | users.json   | 189.580  | 236.629     | 239.630  | 522.4    |
| orjson        | users.json   | 221.047  | 287.259     | 305.477  | 522.4    |
| msgspec       | users.json   | 222.898  | 305.767     | 319.417  | 555.5    |
| ujson         | users.json   | 296.161  | 431.168     | 471.705  | 678.0    |
| json (stdlib) | users.json   | 426.281  | 538.938     | 548.276  | 555.5    |
| strata        | users.ndjson | 197.362  | 242.466     | 244.967  | 633.0    |
| orjson        | users.ndjson | 282.603  | 317.982     | 351.991  | 633.1    |
| msgspec       | users.ndjson | 297.551  | 327.975     | 338.171  | 671.4    |
| ujson         | users.ndjson | 391.170  | 504.745     | 540.212  | 793.9    |
| json (stdlib) | users.ndjson | 498.412  | 529.675     | 556.133  | 673.1    |
| strata        | wide_arrays  | 39.048   | 43.292      | 45.705   | 469.9    |
| orjson        | wide_arrays  | 34.284   | 45.785      | 47.140   | 470.9    |
| msgspec       | wide_arrays  | 40.785   | 52.491      | 56.127   | 486.0    |
| ujson         | wide_arrays  | 67.155   | 78.764      | 102.562  | 485.0    |
| json (stdlib) | wide_arrays  | 140.209  | 152.286     | 152.441  | 487.0    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.228    | 2.261       | 2.397    | 533.2    |
| orjson  | flat        | 3.185    | 3.302       | 3.918    | 528.8    |
| msgspec | flat        | 2.954    | 3.658       | 3.708    | 528.9    |
| ujson   | flat        | 13.333   | 14.314      | 15.373   | 530.8    |
| json    | flat        | 23.762   | 29.997      | 30.252   | 530.8    |
| msgspec | mixed       | 2.900    | 3.117       | 4.217    | 842.3    |
| strata  | mixed       | 2.546    | 3.884       | 6.907    | 842.1    |
| orjson  | mixed       | 3.121    | 5.351       | 7.709    | 837.8    |
| ujson   | mixed       | 14.120   | 15.427      | 19.597   | 844.3    |
| json    | mixed       | 27.883   | 29.459      | 30.989   | 853.9    |
| strata  | nested      | 1.877    | 1.964       | 2.514    | 529.3    |
| msgspec | nested      | 1.994    | 2.045       | 2.393    | 529.3    |
| orjson  | nested      | 2.034    | 2.093       | 2.522    | 529.3    |
| ujson   | nested      | 12.069   | 12.088      | 12.481   | 529.4    |
| json    | nested      | 12.914   | 12.963      | 14.012   | 529.4    |
| strata  | users.json  | 48.865   | 51.108      | 53.528   | 591.1    |
| msgspec | users.json  | 84.631   | 84.663      | 88.944   | 944.2    |
| orjson  | users.json  | 89.564   | 90.508      | 92.079   | 748.7    |
| ujson   | users.json  | 299.009  | 304.648     | 324.265  | 1140.1   |
| json    | users.json  | 429.707  | 441.346     | 458.843  | 1354.8   |
| strata  | wide_arrays | 14.895   | 16.371      | 16.628   | 486.0    |
| msgspec | wide_arrays | 17.582   | 17.828      | 19.775   | 584.2    |
| orjson  | wide_arrays | 17.613   | 17.882      | 18.108   | 535.1    |
| ujson   | wide_arrays | 76.611   | 79.306      | 79.577   | 633.2    |
| json    | wide_arrays | 208.344  | 213.628     | 219.582  | 685.3    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 2.879    | 2.992       | 3.634    | 530.8    |
| orjson        | flat        | 3.717    | 4.173       | 5.010    | 530.6    |
| msgspec       | flat        | 4.497    | 5.193       | 5.685    | 530.7    |
| ujson         | flat        | 13.647   | 13.914      | 22.668   | 530.6    |
| json (stdlib) | flat        | 84.709   | 87.029      | 88.095   | 530.7    |
| strata        | mixed       | 3.380    | 3.385       | 4.797    | 853.9    |
| orjson        | mixed       | 3.851    | 3.977       | 4.998    | 853.8    |
| msgspec       | mixed       | 4.712    | 5.521       | 7.724    | 853.8    |
| ujson         | mixed       | 13.966   | 15.383      | 22.399   | 853.8    |
| json (stdlib) | mixed       | 102.259  | 103.042     | 119.581  | 853.8    |
| strata        | nested      | 2.349    | 2.360       | 2.776    | 529.4    |
| msgspec       | nested      | 2.541    | 2.674       | 2.675    | 529.4    |
| orjson        | nested      | 2.694    | 2.696       | 2.842    | 529.4    |
| ujson         | nested      | 11.392   | 12.023      | 12.632   | 529.4    |
| json (stdlib) | nested      | 123.157  | 132.881     | 135.596  | 529.4    |
| strata        | users.json  | 57.307   | 79.893      | 83.816   | 1398.7   |
| msgspec       | users.json  | 99.066   | 99.214      | 123.526  | 1865.8   |
| orjson        | users.json  | 111.349  | 118.486     | 122.478  | 1554.4   |
| ujson         | users.json  | 337.420  | 338.928     | 344.713  | 1710.1   |
| json (stdlib) | users.json  | 3131.332 | 3158.604    | 3249.961 | 1299.8   |
| strata        | wide_arrays | 21.668   | 22.786      | 23.394   | 685.3    |
| orjson        | wide_arrays | 23.175   | 23.797      | 24.369   | 734.3    |
| msgspec       | wide_arrays | 21.720   | 25.516      | 30.752   | 826.8    |
| ujson         | wide_arrays | 80.891   | 83.206      | 83.338   | 783.3    |
| json (stdlib) | wide_arrays | 475.945  | 497.419     | 504.454  | 826.8    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 87.285   | 4000    | 1079.9   |
|                                | orjson+jsonpath-ng | 218.280  | 4000    | 1440.0   |
|                                | orjson+jmespath    | 224.129  | 4000    | 1432.3   |
| all item prices                | strata             | 97.820   | 789913  | 1464.2   |
|                                | orjson+jmespath    | 569.867  | 4000    | 1525.9   |
|                                | orjson+jsonpath-ng | 2038.801 | 789913  | 1104.2   |
| recursive price                | strata             | 101.068  | 789913  | 1077.3   |
|                                | orjson+jsonpath-ng | 6417.859 | 789913  | 713.8    |
| flat:$.records\[\*\].id        | strata             | 5.049    | 5000    | 530.7    |
|                                | orjson+jmespath    | 11.740   | 5000    | 530.7    |
|                                | orjson+jsonpath-ng | 19.754   | 5000    | 527.7    |
| flat:$.records\[0\]            | strata             | 5.487    | 1       | 527.7    |
|                                | orjson+jmespath    | 9.799    | 1       | 527.7    |
|                                | orjson+jsonpath-ng | 10.377   | 1       | 527.7    |
| nested:$.records\[\*\].id      | strata             | 3.756    | 5000    | 529.4    |
|                                | orjson+jmespath    | 19.650   | 5000    | 529.4    |
|                                | orjson+jsonpath-ng | 24.000   | 5000    | 528.4    |
| nested:$.records\[0\]          | strata             | 3.513    | 1       | 528.4    |
|                                | orjson+jmespath    | 16.284   | 1       | 528.4    |
|                                | orjson+jsonpath-ng | 16.419   | 1       | 528.4    |
| wide_arrays:$.records\[\*\].id | strata             | 26.983   | 5000    | 826.8    |
|                                | orjson+jmespath    | 41.860   | 5000    | 826.8    |
|                                | orjson+jsonpath-ng | 49.575   | 5000    | 826.8    |
| wide_arrays:$.records\[0\]     | strata             | 28.179   | 1       | 826.9    |
|                                | orjson+jsonpath-ng | 38.350   | 1       | 826.9    |
|                                | orjson+jmespath    | 39.545   | 1       | 826.9    |
| mixed:$.records\[\*\].id       | strata             | 5.204    | 5000    | 853.8    |
|                                | orjson+jmespath    | 13.096   | 5000    | 853.8    |
|                                | orjson+jsonpath-ng | 20.937   | 5000    | 853.8    |
| mixed:$.records\[0\]           | strata             | 4.666    | 1       | 853.8    |
|                                | orjson+jsonpath-ng | 11.902   | 1       | 853.8    |
|                                | orjson+jmespath    | 12.847   | 1       | 853.8    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.728    | 4000    | 942.3    |
|                                            | jmespath    | 2.219    | 4000    | 942.3    |
|                                            | jsonpath-ng | 82.320   | 4000    | 944.0    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 44.112   | 789913  | 950.1    |
|                                            | jmespath    | 413.928  | 4000    | 965.0    |
|                                            | jsonpath-ng | 1842.067 | 789913  | 862.1    |
| $..price                                   | strata      | 64.269   | 789913  | 879.4    |
|                                            | jsonpath-ng | 6322.242 | 789913  | 979.4    |
| $.users\[?(@.age>30)\]                     | strata      | 0.856    | 3212    | 997.0    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 68.985   | 26406   | 800.7    |
| flat:$.records\[\*\].id                    | strata      | 0.349    | 5000    | 527.7    |
|                                            | jmespath    | 1.731    | 5000    | 527.7    |
|                                            | jsonpath-ng | 10.178   | 5000    | 527.7    |
| flat:$.records\[0\]                        | jmespath    | 0.008    | 1       | 527.7    |
|                                            | strata      | 0.008    | 1       | 527.7    |
|                                            | jsonpath-ng | 0.015    | 1       | 527.7    |
| nested:$.records\[\*\].id                  | strata      | 0.651    | 5000    | 528.4    |
|                                            | jmespath    | 1.819    | 5000    | 528.4    |
|                                            | jsonpath-ng | 18.148   | 5000    | 528.4    |
| nested:$.records\[0\]                      | strata      | 0.009    | 1       | 528.4    |
|                                            | jmespath    | 0.015    | 1       | 528.4    |
|                                            | jsonpath-ng | 0.016    | 1       | 528.4    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.265    | 5000    | 826.9    |
|                                            | jmespath    | 1.957    | 5000    | 826.9    |
|                                            | jsonpath-ng | 18.700   | 5000    | 826.9    |
| wide_arrays:$.records\[0\]                 | strata      | 0.009    | 1       | 826.9    |
|                                            | jmespath    | 0.015    | 1       | 826.9    |
|                                            | jsonpath-ng | 0.021    | 1       | 826.9    |
| mixed:$.records\[\*\].id                   | strata      | 0.397    | 5000    | 853.8    |
|                                            | jmespath    | 1.868    | 5000    | 853.8    |
|                                            | jsonpath-ng | 13.807   | 5000    | 853.8    |
| mixed:$.records\[0\]                       | strata      | 0.008    | 1       | 853.8    |
|                                            | jmespath    | 0.014    | 1       | 853.8    |
|                                            | jsonpath-ng | 0.020    | 1       | 853.8    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **20.1% faster** than #2 (orjson)  |
| loads (NDJSON)      | **#1** / 5              | **32.4% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **16.6% faster** than #2 (orjson)  |
| load (NDJSON file)  | **#1** / 5              | **43.2% faster** than #2 (orjson)  |
| dumps               | **#1** / 5              | **73.2% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **72.9% faster** than #2 (msgspec) |
| loads (flat)        | **#1** / 5              | **28.8% faster** than #2 (msgspec) |
| load (flat)         | **#1** / 5              | **18.9% faster** than #2 (orjson)  |
| dumps (flat)        | **#1** / 5              | **32.6% faster** than #2 (msgspec) |
| dump (flat)         | **#1** / 5              | **29.1% faster** than #2 (orjson)  |
| loads (nested)      | **#1** / 5              | **98.8% faster** than #2 (msgspec) |
| load (nested)       | **#1** / 5              | **124.2% faster** than #2 (orjson) |
| dumps (nested)      | **#1** / 5              | **6.2% faster** than #2 (msgspec)  |
| dump (nested)       | **#1** / 5              | **8.2% faster** than #2 (msgspec)  |
| loads (wide_arrays) | **#2** / 5              | 34.5% behind #1 (orjson)           |
| load (wide_arrays)  | **#2** / 5              | 13.9% behind #1 (orjson)           |
| dumps (wide_arrays) | **#1** / 5              | **18.0% faster** than #2 (msgspec) |
| dump (wide_arrays)  | **#1** / 5              | **0.2% faster** than #2 (msgspec)  |
| loads (mixed)       | **#1** / 5              | **66.6% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **12.0% faster** than #2 (orjson)  |
| dumps (mixed)       | **#1** / 5              | **13.9% faster** than #2 (msgspec) |
| dump (mixed)        | **#1** / 5              | **14.0% faster** than #2 (orjson)  |
| search (JSONPath)   | **#1** in 11/11 queries | avg **753% faster**                |
| query (JSONPath)    | **#1** in 12/13 queries | -                                  |
