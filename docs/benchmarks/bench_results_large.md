# Strata Benchmark Results

Generated: 2026-03-15 18:37:49

## Environment

- Commit: 67ef7fc31abef4549164a63037b9929eceed01a5
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
| msgspec       | flat         | 8.204    | 9.040       | 9.161    | 395.9    |
| orjson        | flat         | 7.849    | 9.291       | 9.382    | 395.9    |
| ujson         | flat         | 12.802   | 14.606      | 14.862   | 395.9    |
| strata        | flat         | 13.807   | 15.256      | 15.866   | 519.8    |
| json (stdlib) | flat         | 21.393   | 22.596      | 22.756   | 398.9    |
| orjson        | mixed        | 8.753    | 11.338      | 11.357   | 856.2    |
| msgspec       | mixed        | 9.349    | 11.786      | 11.927   | 856.2    |
| strata        | mixed        | 14.586   | 15.886      | 16.093   | 850.6    |
| ujson         | mixed        | 13.010   | 16.313      | 16.393   | 856.2    |
| json (stdlib) | mixed        | 23.111   | 25.650      | 25.687   | 858.6    |
| orjson        | nested       | 12.289   | 16.245      | 16.440   | 399.4    |
| msgspec       | nested       | 12.742   | 16.546      | 16.911   | 399.4    |
| ujson         | nested       | 15.353   | 19.952      | 20.132   | 399.4    |
| strata        | nested       | 20.687   | 21.573      | 21.665   | 399.4    |
| json (stdlib) | nested       | 21.485   | 25.382      | 26.261   | 399.4    |
| orjson        | users.json   | 174.035  | 245.399     | 246.433  | 478.0    |
| msgspec       | users.json   | 186.256  | 260.137     | 263.573  | 467.4    |
| ujson         | users.json   | 255.291  | 354.578     | 355.237  | 589.8    |
| json (stdlib) | users.json   | 367.389  | 440.779     | 444.237  | 511.3    |
| strata        | users.json   | 439.433  | 476.519     | 477.448  | 340.1    |
| orjson        | users.ndjson | 230.452  | 285.066     | 285.297  | 620.7    |
| msgspec       | users.ndjson | 240.668  | 295.834     | 302.724  | 615.3    |
| ujson         | users.ndjson | 339.309  | 396.002     | 413.801  | 737.8    |
| strata        | users.ndjson | 443.436  | 480.177     | 485.435  | 569.9    |
| json (stdlib) | users.ndjson | 429.885  | 481.605     | 486.134  | 618.0    |
| orjson        | wide_arrays  | 27.272   | 36.193      | 38.220   | 481.8    |
| msgspec       | wide_arrays  | 35.698   | 45.566      | 47.366   | 447.3    |
| ujson         | wide_arrays  | 58.608   | 67.468      | 68.578   | 447.3    |
| strata        | wide_arrays  | 70.988   | 76.348      | 79.339   | 456.3    |
| json (stdlib) | wide_arrays  | 123.469  | 130.127     | 132.730  | 461.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | flat         | 8.373    | 9.382       | 9.509    | 398.9    |
| msgspec       | flat         | 8.467    | 9.606       | 9.677    | 401.8    |
| strata        | flat         | 13.323   | 14.713      | 14.874   | 398.9    |
| ujson         | flat         | 13.359   | 15.649      | 15.679   | 401.8    |
| json (stdlib) | flat         | 22.573   | 24.133      | 24.521   | 401.8    |
| orjson        | mixed        | 9.335    | 11.826      | 11.891   | 858.6    |
| msgspec       | mixed        | 10.037   | 12.296      | 12.449   | 861.0    |
| ujson         | mixed        | 14.192   | 17.095      | 17.319   | 861.0    |
| strata        | mixed        | 16.267   | 17.530      | 17.603   | 858.6    |
| json (stdlib) | mixed        | 24.083   | 26.095      | 26.611   | 861.0    |
| orjson        | nested       | 13.141   | 17.592      | 20.488   | 399.4    |
| msgspec       | nested       | 14.711   | 17.619      | 18.816   | 401.2    |
| ujson         | nested       | 16.784   | 21.252      | 22.991   | 401.2    |
| strata        | nested       | 21.896   | 22.114      | 22.850   | 399.4    |
| json (stdlib) | nested       | 21.608   | 24.904      | 25.575   | 401.2    |
| orjson        | users.json   | 184.978  | 252.021     | 253.858  | 522.1    |
| msgspec       | users.json   | 192.562  | 268.177     | 268.938  | 555.2    |
| ujson         | users.json   | 266.205  | 364.020     | 369.353  | 677.7    |
| json (stdlib) | users.json   | 373.848  | 449.188     | 454.801  | 555.2    |
| strata        | users.json   | 446.829  | 483.651     | 486.162  | 522.3    |
| orjson        | users.ndjson | 235.653  | 287.867     | 289.120  | 621.0    |
| msgspec       | users.ndjson | 256.688  | 302.893     | 305.803  | 659.2    |
| ujson         | users.ndjson | 336.073  | 418.106     | 430.561  | 781.7    |
| json (stdlib) | users.ndjson | 426.393  | 477.037     | 484.256  | 660.9    |
| strata        | users.ndjson | 457.551  | 494.102     | 501.452  | 620.1    |
| orjson        | wide_arrays  | 30.684   | 39.433      | 41.122   | 489.9    |
| msgspec       | wide_arrays  | 36.860   | 45.698      | 47.238   | 505.0    |
| ujson         | wide_arrays  | 64.693   | 70.790      | 73.377   | 504.0    |
| strata        | wide_arrays  | 77.993   | 83.316      | 86.326   | 463.4    |
| json (stdlib) | wide_arrays  | 123.293  | 133.028     | 134.222  | 506.0    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| msgspec | flat        | 2.755    | 2.762       | 2.856    | 403.4    |
| strata  | flat        | 2.848    | 2.864       | 2.885    | 401.8    |
| orjson  | flat        | 2.855    | 2.877       | 2.920    | 403.4    |
| ujson   | flat        | 11.586   | 11.659      | 11.686   | 405.4    |
| json    | flat        | 21.681   | 21.830      | 21.890   | 405.4    |
| msgspec | mixed       | 2.741    | 2.748       | 2.860    | 865.5    |
| strata  | mixed       | 2.909    | 2.957       | 2.960    | 861.0    |
| orjson  | mixed       | 2.935    | 2.957       | 3.021    | 861.6    |
| ujson   | mixed       | 12.087   | 12.134      | 12.193   | 865.5    |
| json    | mixed       | 27.086   | 27.254      | 27.312   | 870.7    |
| strata  | nested      | 1.664    | 1.671       | 1.807    | 401.2    |
| orjson  | nested      | 1.858    | 1.874       | 1.874    | 401.2    |
| msgspec | nested      | 1.903    | 1.927       | 1.975    | 401.2    |
| ujson   | nested      | 9.823    | 9.918       | 10.243   | 404.5    |
| json    | nested      | 12.641   | 12.761      | 13.280   | 404.5    |
| strata  | users.json  | 73.634   | 73.800      | 74.123   | 590.8    |
| msgspec | users.json  | 74.314   | 74.317      | 74.427   | 943.0    |
| orjson  | users.json  | 74.282   | 74.323      | 74.399   | 747.5    |
| ujson   | users.json  | 280.552  | 280.631     | 282.399  | 1139.8   |
| json    | users.json  | 389.137  | 389.304     | 392.281  | 1354.6   |
| orjson  | wide_arrays | 15.620   | 15.679      | 15.746   | 548.2    |
| msgspec | wide_arrays | 16.373   | 16.403      | 16.552   | 597.2    |
| strata  | wide_arrays | 17.787   | 17.792      | 17.812   | 505.0    |
| ujson   | wide_arrays | 69.485   | 69.505      | 69.567   | 646.3    |
| json    | wide_arrays | 191.118  | 191.915     | 191.936  | 698.4    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 3.114    | 3.130       | 3.167    | 405.4    |
| msgspec       | flat        | 3.368    | 3.413       | 3.477    | 405.4    |
| orjson        | flat        | 3.365    | 3.540       | 3.619    | 405.4    |
| ujson         | flat        | 12.263   | 12.417      | 12.425   | 405.4    |
| json (stdlib) | flat        | 77.869   | 77.927      | 79.880   | 405.4    |
| strata        | mixed       | 3.238    | 3.353       | 3.526    | 870.7    |
| msgspec       | mixed       | 3.306    | 3.374       | 3.395    | 873.0    |
| orjson        | mixed       | 3.711    | 3.729       | 3.797    | 872.9    |
| ujson         | mixed       | 13.109   | 13.113      | 13.521   | 872.9    |
| json (stdlib) | mixed       | 91.875   | 92.058      | 92.325   | 873.0    |
| strata        | nested      | 1.978    | 2.211       | 2.215    | 404.5    |
| msgspec       | nested      | 2.527    | 2.540       | 2.778    | 404.8    |
| orjson        | nested      | 2.588    | 2.606       | 2.611    | 404.5    |
| ujson         | nested      | 10.405   | 10.424      | 10.436   | 404.8    |
| json (stdlib) | nested      | 113.726  | 113.743     | 114.301  | 404.8    |
| strata        | users.json  | 79.100   | 81.028      | 90.715   | 1398.5   |
| msgspec       | users.json  | 93.503   | 96.639      | 98.942   | 1865.6   |
| orjson        | users.json  | 81.306   | 108.278     | 109.154  | 1554.2   |
| ujson         | users.json  | 306.192  | 311.655     | 322.588  | 1709.9   |
| json (stdlib) | users.json  | 2803.045 | 2804.471    | 2806.760 | 1688.8   |
| orjson        | wide_arrays | 18.627   | 18.851      | 18.932   | 747.5    |
| msgspec       | wide_arrays | 18.829   | 19.423      | 24.514   | 845.7    |
| strata        | wide_arrays | 22.122   | 22.651      | 22.774   | 698.4    |
| ujson         | wide_arrays | 71.896   | 72.120      | 73.042   | 796.6    |
| json (stdlib) | wide_arrays | 427.219  | 427.479     | 429.500  | 845.7    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 65.203   | 4000    | 1469.9   |
|                                | orjson+jmespath    | 194.020  | 4000    | 1822.3   |
|                                | orjson+jsonpath-ng | 199.013  | 4000    | 1829.2   |
| all item prices                | strata             | 77.092   | 789913  | 1854.2   |
|                                | orjson+jmespath    | 493.615  | 4000    | 1487.0   |
|                                | orjson+jsonpath-ng | 1794.021 | 789913  | 1518.6   |
| recursive price                | strata             | 77.573   | 789913  | 940.2    |
|                                | orjson+jsonpath-ng | 5876.562 | 789913  | 712.6    |
| flat:$.records\[\*\].id        | strata             | 3.377    | 5000    | 405.4    |
|                                | orjson+jmespath    | 10.077   | 5000    | 405.4    |
|                                | orjson+jsonpath-ng | 15.333   | 5000    | 401.4    |
| flat:$.records\[0\]            | strata             | 3.268    | 1       | 401.4    |
|                                | orjson+jsonpath-ng | 8.479    | 1       | 401.4    |
|                                | orjson+jmespath    | 8.493    | 1       | 401.4    |
| nested:$.records\[\*\].id      | strata             | 2.763    | 5000    | 404.8    |
|                                | orjson+jmespath    | 16.008   | 5000    | 404.8    |
|                                | orjson+jsonpath-ng | 21.051   | 5000    | 404.8    |
| nested:$.records\[0\]          | strata             | 2.690    | 1       | 404.8    |
|                                | orjson+jsonpath-ng | 13.979   | 1       | 404.8    |
|                                | orjson+jmespath    | 14.024   | 1       | 404.8    |
| wide_arrays:$.records\[\*\].id | strata             | 18.995   | 5000    | 845.7    |
|                                | orjson+jmespath    | 35.032   | 5000    | 845.7    |
|                                | orjson+jsonpath-ng | 41.513   | 5000    | 845.7    |
| wide_arrays:$.records\[0\]     | strata             | 18.794   | 1       | 845.7    |
|                                | orjson+jsonpath-ng | 32.897   | 1       | 845.7    |
|                                | orjson+jmespath    | 35.073   | 1       | 845.7    |
| mixed:$.records\[\*\].id       | strata             | 3.485    | 5000    | 873.0    |
|                                | orjson+jmespath    | 11.719   | 5000    | 873.0    |
|                                | orjson+jsonpath-ng | 17.337   | 5000    | 873.0    |
| mixed:$.records\[0\]           | strata             | 3.393    | 1       | 873.0    |
|                                | orjson+jsonpath-ng | 10.073   | 1       | 873.0    |
|                                | orjson+jmespath    | 10.156   | 1       | 873.0    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.703    | 4000    | 941.3    |
|                                            | jmespath    | 2.102    | 4000    | 941.3    |
|                                            | jsonpath-ng | 76.216   | 4000    | 943.0    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 41.002   | 789913  | 949.0    |
|                                            | jmespath    | 376.794  | 4000    | 963.9    |
|                                            | jsonpath-ng | 1565.972 | 789913  | 824.2    |
| $..price                                   | strata      | 55.383   | 789913  | 831.9    |
|                                            | jsonpath-ng | 5705.888 | 789913  | 830.3    |
| $.users\[?(@.age>30)\]                     | strata      | 0.812    | 3212    | 833.8    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 58.513   | 26406   | 833.8    |
| flat:$.records\[\*\].id                    | strata      | 0.404    | 5000    | 401.4    |
|                                            | jmespath    | 1.611    | 5000    | 401.4    |
|                                            | jsonpath-ng | 9.428    | 5000    | 401.4    |
| flat:$.records\[0\]                        | strata      | 0.003    | 1       | 401.4    |
|                                            | jmespath    | 0.007    | 1       | 401.4    |
|                                            | jsonpath-ng | 0.009    | 1       | 401.4    |
| nested:$.records\[\*\].id                  | strata      | 0.447    | 5000    | 404.8    |
|                                            | jmespath    | 1.769    | 5000    | 404.8    |
|                                            | jsonpath-ng | 14.868   | 5000    | 404.8    |
| nested:$.records\[0\]                      | strata      | 0.006    | 1       | 404.8    |
|                                            | jmespath    | 0.010    | 1       | 404.8    |
|                                            | jsonpath-ng | 0.013    | 1       | 404.8    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.255    | 5000    | 845.7    |
|                                            | jmespath    | 1.792    | 5000    | 845.7    |
|                                            | jsonpath-ng | 15.672   | 5000    | 845.7    |
| wide_arrays:$.records\[0\]                 | strata      | 0.006    | 1       | 845.7    |
|                                            | jmespath    | 0.009    | 1       | 845.7    |
|                                            | jsonpath-ng | 0.010    | 1       | 845.7    |
| mixed:$.records\[\*\].id                   | strata      | 0.341    | 5000    | 873.0    |
|                                            | jmespath    | 1.660    | 5000    | 873.0    |
|                                            | jsonpath-ng | 11.197   | 5000    | 873.0    |
| mixed:$.records\[0\]                       | strata      | 0.005    | 1       | 873.0    |
|                                            | jmespath    | 0.007    | 1       | 873.0    |
|                                            | jsonpath-ng | 0.009    | 1       | 873.0    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#5** / 5              | 152.5% behind #1 (orjson)          |
| loads (NDJSON)      | **#5** / 5              | 92.4% behind #1 (orjson)           |
| load (JSON file)    | **#5** / 5              | 141.6% behind #1 (orjson)          |
| load (NDJSON file)  | **#5** / 5              | 94.2% behind #1 (orjson)           |
| dumps               | **#1** / 5              | **0.9% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **2.8% faster** than #2 (orjson)   |
| loads (flat)        | **#4** / 5              | 75.9% behind #1 (orjson)           |
| load (flat)         | **#3** / 5              | 59.1% behind #1 (orjson)           |
| dumps (flat)        | **#2** / 5              | 3.4% behind #1 (msgspec)           |
| dump (flat)         | **#1** / 5              | **8.1% faster** than #2 (orjson)   |
| loads (nested)      | **#4** / 5              | 68.3% behind #1 (orjson)           |
| load (nested)       | **#5** / 5              | 66.6% behind #1 (orjson)           |
| dumps (nested)      | **#1** / 5              | **11.7% faster** than #2 (orjson)  |
| dump (nested)       | **#1** / 5              | **27.8% faster** than #2 (msgspec) |
| loads (wide_arrays) | **#4** / 5              | 160.3% behind #1 (orjson)          |
| load (wide_arrays)  | **#4** / 5              | 154.2% behind #1 (orjson)          |
| dumps (wide_arrays) | **#3** / 5              | 13.9% behind #1 (orjson)           |
| dump (wide_arrays)  | **#3** / 5              | 18.8% behind #1 (orjson)           |
| loads (mixed)       | **#4** / 5              | 66.6% behind #1 (orjson)           |
| load (mixed)        | **#4** / 5              | 74.3% behind #1 (orjson)           |
| dumps (mixed)       | **#2** / 5              | 6.1% behind #1 (msgspec)           |
| dump (mixed)        | **#1** / 5              | **2.1% faster** than #2 (msgspec)  |
| search (JSONPath)   | **#1** in 11/11 queries | avg **915% faster**                |
| query (JSONPath)    | **#1** in 13/13 queries | -                                  |
