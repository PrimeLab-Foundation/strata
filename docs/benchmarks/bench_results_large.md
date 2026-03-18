# Strata Benchmark Results

Generated: 2026-03-18 13:24:36

## Environment

- Commit: 61b2175f7dd9503191430e1dff7b62dbe525ea97
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
| strata        | flat         | 7.298    | 8.218       | 8.449    | 509.0    |
| orjson        | flat         | 8.090    | 9.615       | 9.709    | 509.0    |
| msgspec       | flat         | 8.221    | 9.635       | 11.192   | 509.0    |
| ujson         | flat         | 13.043   | 15.212      | 15.471   | 509.0    |
| json (stdlib) | flat         | 22.406   | 23.723      | 23.996   | 511.9    |
| strata        | mixed        | 6.965    | 8.519       | 8.551    | 782.9    |
| orjson        | mixed        | 8.667    | 11.585      | 11.606   | 788.5    |
| msgspec       | mixed        | 9.667    | 12.269      | 12.391   | 788.5    |
| ujson         | mixed        | 13.742   | 16.647      | 16.809   | 788.5    |
| json (stdlib) | mixed        | 24.295   | 27.966      | 28.435   | 790.9    |
| strata        | nested       | 6.067    | 7.280       | 7.331    | 508.3    |
| orjson        | nested       | 12.904   | 17.022      | 17.207   | 508.3    |
| msgspec       | nested       | 13.464   | 17.554      | 18.363   | 508.3    |
| ujson         | nested       | 15.451   | 20.609      | 20.886   | 508.3    |
| json (stdlib) | nested       | 21.877   | 26.377      | 27.484   | 508.3    |
| strata        | users.json   | 172.298  | 208.286     | 209.553  | 340.3    |
| orjson        | users.json   | 180.167  | 258.523     | 273.152  | 478.3    |
| msgspec       | users.json   | 198.589  | 273.696     | 275.750  | 467.8    |
| ujson         | users.json   | 269.345  | 378.896     | 394.961  | 590.2    |
| json (stdlib) | users.json   | 386.625  | 462.995     | 479.772  | 511.7    |
| strata        | users.ndjson | 194.908  | 230.899     | 243.042  | 566.5    |
| orjson        | users.ndjson | 255.973  | 307.348     | 317.395  | 616.7    |
| msgspec       | users.ndjson | 264.895  | 312.505     | 334.503  | 611.2    |
| ujson         | users.ndjson | 348.197  | 429.436     | 447.467  | 733.7    |
| json (stdlib) | users.ndjson | 458.601  | 514.205     | 525.032  | 612.9    |
| orjson        | wide_arrays  | 28.098   | 37.128      | 38.803   | 452.3    |
| strata        | wide_arrays  | 32.441   | 37.898      | 40.420   | 426.7    |
| msgspec       | wide_arrays  | 37.952   | 45.126      | 47.568   | 454.3    |
| ujson         | wide_arrays  | 59.744   | 69.363      | 70.638   | 453.3    |
| json (stdlib) | wide_arrays  | 126.184  | 134.705     | 137.521  | 468.3    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 7.594    | 8.654       | 8.665    | 511.9    |
| orjson        | flat         | 8.612    | 10.234      | 10.423   | 511.9    |
| msgspec       | flat         | 8.984    | 10.626      | 11.100   | 514.9    |
| ujson         | flat         | 14.322   | 16.818      | 16.958   | 514.9    |
| json (stdlib) | flat         | 23.734   | 24.986      | 56.869   | 514.9    |
| strata        | mixed        | 7.615    | 9.663       | 10.263   | 790.9    |
| msgspec       | mixed        | 10.653   | 13.147      | 13.677   | 793.3    |
| orjson        | mixed        | 9.814    | 13.394      | 14.473   | 790.9    |
| ujson         | mixed        | 14.990   | 18.057      | 18.274   | 793.3    |
| json (stdlib) | mixed        | 25.446   | 28.049      | 28.328   | 793.3    |
| strata        | nested       | 6.430    | 7.619       | 7.646    | 508.3    |
| orjson        | nested       | 13.321   | 17.390      | 17.487   | 508.3    |
| msgspec       | nested       | 14.533   | 18.894      | 24.379   | 513.6    |
| ujson         | nested       | 16.252   | 21.257      | 22.011   | 511.8    |
| json (stdlib) | nested       | 22.504   | 26.900      | 30.076   | 513.6    |
| strata        | users.json   | 175.524  | 217.785     | 223.286  | 522.5    |
| orjson        | users.json   | 187.140  | 263.864     | 272.644  | 522.5    |
| msgspec       | users.json   | 213.136  | 281.289     | 304.207  | 555.7    |
| ujson         | users.json   | 280.747  | 385.544     | 391.364  | 678.2    |
| json (stdlib) | users.json   | 393.115  | 474.481     | 477.296  | 555.7    |
| strata        | users.ndjson | 189.858  | 236.208     | 236.555  | 616.8    |
| orjson        | users.ndjson | 242.628  | 304.494     | 319.675  | 616.9    |
| msgspec       | users.ndjson | 254.625  | 321.065     | 327.927  | 655.2    |
| ujson         | users.ndjson | 343.412  | 433.830     | 436.941  | 777.7    |
| json (stdlib) | users.ndjson | 443.410  | 494.627     | 506.779  | 656.9    |
| orjson        | wide_arrays  | 30.347   | 38.979      | 41.496   | 471.3    |
| strata        | wide_arrays  | 34.057   | 40.358      | 42.495   | 470.3    |
| msgspec       | wide_arrays  | 38.929   | 48.011      | 49.346   | 486.3    |
| ujson         | wide_arrays  | 63.604   | 73.354      | 74.805   | 485.3    |
| json (stdlib) | wide_arrays  | 129.123  | 139.145     | 140.588  | 487.3    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 2.471    | 2.537       | 2.547    | 514.9    |
| msgspec | flat        | 2.847    | 2.918       | 3.195    | 510.6    |
| orjson  | flat        | 2.901    | 2.962       | 2.972    | 510.5    |
| ujson   | flat        | 12.013   | 12.057      | 12.398   | 512.5    |
| json    | flat        | 22.504   | 22.557      | 22.574   | 512.5    |
| strata  | mixed       | 2.458    | 2.466       | 2.521    | 793.3    |
| msgspec | mixed       | 2.818    | 2.832       | 2.963    | 797.9    |
| orjson  | mixed       | 3.107    | 3.111       | 3.118    | 793.9    |
| ujson   | mixed       | 12.560   | 12.629      | 12.654   | 797.9    |
| json    | mixed       | 27.921   | 28.109      | 30.950   | 803.0    |
| strata  | nested      | 1.831    | 1.838       | 1.847    | 513.6    |
| orjson  | nested      | 1.953    | 1.956       | 1.975    | 513.6    |
| msgspec | nested      | 1.960    | 1.964       | 1.970    | 513.6    |
| ujson   | nested      | 9.928    | 10.011      | 10.501   | 513.7    |
| json    | nested      | 12.927   | 13.023      | 13.088   | 513.8    |
| strata  | users.json  | 44.207   | 44.615      | 45.251   | 594.5    |
| msgspec | users.json  | 79.439   | 80.098      | 82.120   | 947.6    |
| orjson  | users.json  | 80.813   | 81.725      | 83.906   | 751.5    |
| ujson   | users.json  | 308.443  | 310.955     | 315.919  | 1143.5   |
| json    | users.json  | 411.730  | 413.948     | 421.957  | 1167.8   |
| strata  | wide_arrays | 13.656   | 13.659      | 13.663   | 485.3    |
| orjson  | wide_arrays | 15.803   | 16.059      | 16.142   | 522.2    |
| msgspec | wide_arrays | 17.430   | 17.752      | 17.871   | 540.0    |
| ujson   | wide_arrays | 72.159   | 72.277      | 72.303   | 589.0    |
| json    | wide_arrays | 197.267  | 199.417     | 199.611  | 627.2    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 2.604    | 2.746       | 2.839    | 512.5    |
| msgspec       | flat        | 3.482    | 3.487       | 3.511    | 512.3    |
| orjson        | flat        | 3.302    | 3.494       | 3.723    | 512.3    |
| ujson         | flat        | 12.725   | 12.736      | 12.775   | 512.3    |
| json (stdlib) | flat        | 80.584   | 80.907      | 81.472   | 512.3    |
| strata        | mixed       | 2.975    | 3.048       | 3.565    | 803.0    |
| msgspec       | mixed       | 3.599    | 3.752       | 3.873    | 802.9    |
| orjson        | mixed       | 3.758    | 5.296       | 7.647    | 802.9    |
| ujson         | mixed       | 13.575   | 13.601      | 13.649   | 802.9    |
| json (stdlib) | mixed       | 95.896   | 95.987      | 96.253   | 802.9    |
| strata        | nested      | 2.179    | 2.183       | 2.255    | 513.8    |
| orjson        | nested      | 2.392    | 2.479       | 2.520    | 513.8    |
| msgspec       | nested      | 2.471    | 2.491       | 2.493    | 513.8    |
| ujson         | nested      | 10.497   | 10.516      | 10.652   | 513.8    |
| json (stdlib) | nested      | 115.666  | 117.356     | 119.874  | 513.8    |
| strata        | users.json  | 52.553   | 52.624      | 52.937   | 1211.7   |
| msgspec       | users.json  | 85.924   | 86.223      | 89.155   | 1684.3   |
| orjson        | users.json  | 86.198   | 88.930      | 94.699   | 1367.4   |
| ujson         | users.json  | 305.334  | 310.846     | 314.715  | 1516.3   |
| json (stdlib) | users.json  | 2976.045 | 3114.419    | 3155.021 | 1563.7   |
| strata        | wide_arrays | 15.588   | 15.658      | 15.689   | 627.2    |
| orjson        | wide_arrays | 19.531   | 19.599      | 19.684   | 676.3    |
| msgspec       | wide_arrays | 20.269   | 20.574      | 20.691   | 781.8    |
| ujson         | wide_arrays | 74.403   | 75.038      | 75.152   | 732.7    |
| json (stdlib) | wide_arrays | 442.571  | 444.659     | 445.638  | 781.8    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 86.504   | 4000    | 1344.8   |
|                                | orjson+jmespath    | 201.625  | 4000    | 1697.2   |
|                                | orjson+jsonpath-ng | 205.892  | 4000    | 1703.9   |
| all item prices                | strata             | 96.237   | 789913  | 1729.0   |
|                                | orjson+jmespath    | 520.711  | 4000    | 1277.3   |
|                                | orjson+jsonpath-ng | 1854.923 | 789913  | 1312.4   |
| recursive price                | strata             | 91.926   | 789913  | 980.5    |
|                                | orjson+jsonpath-ng | 6124.592 | 789913  | 713.0    |
| flat:$.records\[\*\].id        | strata             | 4.556    | 5000    | 512.3    |
|                                | orjson+jmespath    | 10.826   | 5000    | 512.3    |
|                                | orjson+jsonpath-ng | 16.446   | 5000    | 509.3    |
| flat:$.records\[0\]            | strata             | 4.515    | 1       | 509.3    |
|                                | orjson+jsonpath-ng | 9.048    | 1       | 509.3    |
|                                | orjson+jmespath    | 9.379    | 1       | 509.3    |
| nested:$.records\[\*\].id      | strata             | 3.534    | 5000    | 513.8    |
|                                | orjson+jmespath    | 15.598   | 5000    | 513.8    |
|                                | orjson+jsonpath-ng | 21.654   | 5000    | 512.8    |
| nested:$.records\[0\]          | strata             | 3.449    | 1       | 512.8    |
|                                | orjson+jmespath    | 14.246   | 1       | 512.8    |
|                                | orjson+jsonpath-ng | 14.504   | 1       | 512.8    |
| wide_arrays:$.records\[\*\].id | strata             | 26.961   | 5000    | 781.8    |
|                                | orjson+jmespath    | 36.128   | 5000    | 781.8    |
|                                | orjson+jsonpath-ng | 42.732   | 5000    | 781.8    |
| wide_arrays:$.records\[0\]     | strata             | 26.348   | 1       | 781.8    |
|                                | orjson+jsonpath-ng | 34.038   | 1       | 781.8    |
|                                | orjson+jmespath    | 36.353   | 1       | 781.8    |
| mixed:$.records\[\*\].id       | strata             | 4.790    | 5000    | 802.9    |
|                                | orjson+jmespath    | 11.969   | 5000    | 802.9    |
|                                | orjson+jsonpath-ng | 17.876   | 5000    | 802.9    |
| mixed:$.records\[0\]           | strata             | 4.770    | 1       | 802.9    |
|                                | orjson+jsonpath-ng | 10.513   | 1       | 802.9    |
|                                | orjson+jmespath    | 10.727   | 1       | 802.9    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.705    | 4000    | 941.5    |
|                                            | jmespath    | 2.190    | 4000    | 941.5    |
|                                            | jsonpath-ng | 78.382   | 4000    | 943.2    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 41.258   | 789913  | 943.3    |
|                                            | jmespath    | 390.389  | 4000    | 958.2    |
|                                            | jsonpath-ng | 1601.968 | 789913  | 819.6    |
| $..price                                   | strata      | 56.169   | 789913  | 827.3    |
|                                            | jsonpath-ng | 5969.040 | 789913  | 826.0    |
| $.users\[?(@.age>30)\]                     | strata      | 0.852    | 3212    | 829.6    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 62.344   | 26406   | 829.6    |
| flat:$.records\[\*\].id                    | strata      | 0.311    | 5000    | 509.3    |
|                                            | jmespath    | 1.660    | 5000    | 509.3    |
|                                            | jsonpath-ng | 9.431    | 5000    | 509.3    |
| flat:$.records\[0\]                        | strata      | 0.004    | 1       | 509.3    |
|                                            | jmespath    | 0.007    | 1       | 509.3    |
|                                            | jsonpath-ng | 0.009    | 1       | 509.3    |
| nested:$.records\[\*\].id                  | strata      | 0.427    | 5000    | 512.8    |
|                                            | jmespath    | 1.753    | 5000    | 512.8    |
|                                            | jsonpath-ng | 15.129   | 5000    | 512.8    |
| nested:$.records\[0\]                      | strata      | 0.010    | 1       | 512.8    |
|                                            | jsonpath-ng | 0.011    | 1       | 512.8    |
|                                            | jmespath    | 0.014    | 1       | 512.8    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.267    | 5000    | 781.8    |
|                                            | jmespath    | 1.795    | 5000    | 781.8    |
|                                            | jsonpath-ng | 16.302   | 5000    | 781.8    |
| wide_arrays:$.records\[0\]                 | strata      | 0.010    | 1       | 781.8    |
|                                            | jmespath    | 0.015    | 1       | 781.8    |
|                                            | jsonpath-ng | 0.022    | 1       | 781.8    |
| mixed:$.records\[\*\].id                   | strata      | 0.319    | 5000    | 802.9    |
|                                            | jmespath    | 1.706    | 5000    | 802.9    |
|                                            | jsonpath-ng | 11.778   | 5000    | 802.9    |
| mixed:$.records\[0\]                       | strata      | 0.006    | 1       | 802.9    |
|                                            | jsonpath-ng | 0.016    | 1       | 802.9    |
|                                            | jmespath    | 0.017    | 1       | 802.9    |

## Summary

| Category            | Strata Rank             | vs #1                              |
| ------------------- | ----------------------- | ---------------------------------- |
| loads (JSON)        | **#1** / 5              | **4.6% faster** than #2 (orjson)   |
| loads (NDJSON)      | **#1** / 5              | **31.3% faster** than #2 (orjson)  |
| load (JSON file)    | **#1** / 5              | **6.6% faster** than #2 (orjson)   |
| load (NDJSON file)  | **#1** / 5              | **27.8% faster** than #2 (orjson)  |
| dumps               | **#1** / 5              | **79.7% faster** than #2 (msgspec) |
| dump                | **#1** / 5              | **63.5% faster** than #2 (msgspec) |
| loads (flat)        | **#1** / 5              | **10.9% faster** than #2 (orjson)  |
| load (flat)         | **#1** / 5              | **13.4% faster** than #2 (orjson)  |
| dumps (flat)        | **#1** / 5              | **15.2% faster** than #2 (msgspec) |
| dump (flat)         | **#1** / 5              | **26.8% faster** than #2 (orjson)  |
| loads (nested)      | **#1** / 5              | **112.7% faster** than #2 (orjson) |
| load (nested)       | **#1** / 5              | **107.2% faster** than #2 (orjson) |
| dumps (nested)      | **#1** / 5              | **6.6% faster** than #2 (orjson)   |
| dump (nested)       | **#1** / 5              | **9.8% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#2** / 5              | 15.5% behind #1 (orjson)           |
| load (wide_arrays)  | **#2** / 5              | 12.2% behind #1 (orjson)           |
| dumps (wide_arrays) | **#1** / 5              | **15.7% faster** than #2 (orjson)  |
| dump (wide_arrays)  | **#1** / 5              | **25.3% faster** than #2 (orjson)  |
| loads (mixed)       | **#1** / 5              | **24.4% faster** than #2 (orjson)  |
| load (mixed)        | **#1** / 5              | **28.9% faster** than #2 (orjson)  |
| dumps (mixed)       | **#1** / 5              | **14.6% faster** than #2 (msgspec) |
| dump (mixed)        | **#1** / 5              | **21.0% faster** than #2 (msgspec) |
| search (JSONPath)   | **#1** in 11/11 queries | avg **760% faster**                |
| query (JSONPath)    | **#1** in 13/13 queries | -                                  |
