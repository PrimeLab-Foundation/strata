# Strata Benchmark Results

Generated: 2026-03-20 01:04:23

## Environment

- Commit: aee3e869a1ae51b05b838b86bb2239e87e02b166
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
| strata        | flat         | 2.877    | 3.320       | 3.366    | 125.0    |
| orjson        | flat         | 3.625    | 4.319       | 4.698    | 127.5    |
| msgspec       | flat         | 4.881    | 5.317       | 6.311    | 127.5    |
| ujson         | flat         | 5.670    | 6.741       | 6.786    | 127.5    |
| json (stdlib) | flat         | 10.779   | 11.057      | 11.121   | 129.5    |
| strata        | mixed        | 3.619    | 4.400       | 4.424    | 180.2    |
| msgspec       | mixed        | 7.333    | 7.559       | 7.855    | 182.4    |
| orjson        | mixed        | 7.568    | 8.542       | 8.578    | 182.4    |
| ujson         | mixed        | 10.350   | 11.236      | 11.393   | 182.4    |
| json (stdlib) | mixed        | 15.265   | 15.392      | 16.678   | 182.4    |
| strata        | nested       | 2.766    | 3.009       | 3.576    | 142.7    |
| msgspec       | nested       | 5.106    | 6.084       | 9.756    | 145.1    |
| ujson         | nested       | 7.194    | 8.020       | 12.070   | 145.1    |
| orjson        | nested       | 7.110    | 8.889       | 10.446   | 145.1    |
| json (stdlib) | nested       | 8.099    | 9.402       | 9.603    | 145.1    |
| strata        | users.json   | 26.641   | 39.988      | 44.823   | 77.5     |
| msgspec       | users.json   | 32.518   | 41.865      | 45.026   | 96.3     |
| orjson        | users.json   | 37.875   | 57.356      | 69.716   | 97.5     |
| json (stdlib) | users.json   | 66.833   | 78.322      | 92.930   | 102.5    |
| ujson         | users.json   | 68.599   | 80.447      | 115.040  | 113.9    |
| strata        | users.ndjson | 25.934   | 33.489      | 34.151   | 141.9    |
| msgspec       | users.ndjson | 36.457   | 38.244      | 40.114   | 106.8    |
| orjson        | users.ndjson | 40.763   | 40.835      | 211.049  | 105.4    |
| ujson         | users.ndjson | 49.189   | 52.034      | 63.386   | 105.8    |
| json (stdlib) | users.ndjson | 70.794   | 79.457      | 80.097   | 107.8    |
| strata        | wide_arrays  | 9.931    | 12.618      | 13.058   | 142.8    |
| orjson        | wide_arrays  | 11.516   | 14.967      | 15.711   | 153.0    |
| msgspec       | wide_arrays  | 14.977   | 18.570      | 22.615   | 153.0    |
| ujson         | wide_arrays  | 24.507   | 28.222      | 28.433   | 153.0    |
| json (stdlib) | wide_arrays  | 64.053   | 65.503      | 66.272   | 156.9    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | flat         | 3.681    | 3.768       | 3.849    | 130.7    |
| orjson        | flat         | 5.161    | 5.167       | 6.071    | 130.7    |
| msgspec       | flat         | 7.332    | 8.781       | 9.009    | 133.3    |
| ujson         | flat         | 7.980    | 10.060      | 12.154   | 133.3    |
| json (stdlib) | flat         | 14.482   | 16.594      | 19.902   | 133.3    |
| strata        | mixed        | 3.877    | 4.543       | 4.638    | 182.5    |
| orjson        | mixed        | 6.077    | 6.222       | 6.576    | 182.5    |
| msgspec       | mixed        | 5.235    | 6.480       | 7.529    | 182.5    |
| ujson         | mixed        | 7.174    | 7.760       | 8.283    | 182.5    |
| json (stdlib) | mixed        | 10.501   | 11.146      | 11.944   | 182.5    |
| strata        | nested       | 2.715    | 3.676       | 4.681    | 145.1    |
| orjson        | nested       | 5.229    | 6.755       | 6.795    | 145.1    |
| ujson         | nested       | 6.167    | 8.404       | 24.237   | 138.8    |
| msgspec       | nested       | 5.363    | 11.283      | 11.581   | 138.8    |
| json (stdlib) | nested       | 10.566   | 20.031      | 42.027   | 122.8    |
| strata        | users.json   | 33.274   | 41.901      | 43.485   | 103.9    |
| msgspec       | users.json   | 30.302   | 44.231      | 45.488   | 108.9    |
| orjson        | users.json   | 36.741   | 54.909      | 61.876   | 104.0    |
| ujson         | users.json   | 57.471   | 66.799      | 78.549   | 126.6    |
| json (stdlib) | users.json   | 63.320   | 92.235      | 99.833   | 108.9    |
| strata        | users.ndjson | 27.626   | 32.365      | 32.929   | 110.1    |
| orjson        | users.ndjson | 33.014   | 41.987      | 45.206   | 111.1    |
| msgspec       | users.ndjson | 44.492   | 46.109      | 47.708   | 118.4    |
| ujson         | users.ndjson | 54.285   | 63.235      | 73.781   | 117.4    |
| json (stdlib) | users.ndjson | 74.605   | 86.720      | 86.886   | 119.4    |
| strata        | wide_arrays  | 13.558   | 15.258      | 20.524   | 161.0    |
| orjson        | wide_arrays  | 14.129   | 16.713      | 16.772   | 161.0    |
| msgspec       | wide_arrays  | 18.049   | 22.866      | 25.542   | 166.2    |
| ujson         | wide_arrays  | 26.862   | 31.104      | 34.634   | 166.2    |
| json (stdlib) | wide_arrays  | 59.349   | 67.421      | 70.793   | 166.2    |

## dumps (in-memory serialization)

| Library | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ----------- | -------- | ----------- | -------- | -------- |
| strata  | flat        | 1.477    | 1.488       | 1.702    | 135.5    |
| orjson  | flat        | 1.502    | 1.547       | 1.664    | 138.3    |
| msgspec | flat        | 1.766    | 1.770       | 1.822    | 138.4    |
| ujson   | flat        | 6.823    | 7.441       | 7.599    | 140.3    |
| json    | flat        | 14.200   | 14.241      | 20.901   | 141.3    |
| orjson  | mixed       | 1.069    | 1.079       | 1.090    | 183.0    |
| strata  | mixed       | 1.106    | 1.124       | 1.125    | 183.0    |
| msgspec | mixed       | 1.170    | 1.180       | 1.273    | 183.0    |
| ujson   | mixed       | 5.099    | 5.291       | 5.379    | 183.0    |
| json    | mixed       | 11.343   | 11.406      | 11.472   | 183.0    |
| orjson  | nested      | 0.798    | 0.801       | 0.882    | 122.9    |
| msgspec | nested      | 0.796    | 0.811       | 0.823    | 122.9    |
| strata  | nested      | 0.821    | 0.823       | 0.957    | 122.9    |
| ujson   | nested      | 3.970    | 4.023       | 4.042    | 122.9    |
| json    | nested      | 5.273    | 5.322       | 5.422    | 122.9    |
| strata  | users.json  | 11.750   | 11.760      | 11.815   | 109.3    |
| orjson  | users.json  | 15.021   | 15.203      | 15.831   | 131.6    |
| msgspec | users.json  | 16.358   | 16.768      | 17.216   | 159.9    |
| ujson   | users.json  | 43.181   | 46.320      | 57.207   | 167.6    |
| json    | users.json  | 63.245   | 71.105      | 89.349   | 128.4    |
| strata  | wide_arrays | 6.114    | 6.146       | 6.893    | 168.6    |
| msgspec | wide_arrays | 6.825    | 6.886       | 6.952    | 189.2    |
| orjson  | wide_arrays | 6.983    | 7.191       | 7.373    | 169.5    |
| ujson   | wide_arrays | 28.767   | 29.163      | 29.577   | 191.2    |
| json    | wide_arrays | 105.304  | 107.330     | 112.476  | 192.7    |

## dump (file-based)

| Library       | Dataset     | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ----------- | -------- | ----------- | -------- | -------- |
| strata        | flat        | 1.876    | 1.990       | 2.315    | 141.3    |
| orjson        | flat        | 1.992    | 2.070       | 2.176    | 141.3    |
| msgspec       | flat        | 2.318    | 2.385       | 2.607    | 141.3    |
| ujson         | flat        | 7.994    | 7.996       | 8.063    | 141.3    |
| json (stdlib) | flat        | 45.683   | 58.318      | 80.570   | 141.3    |
| strata        | mixed       | 1.380    | 1.451       | 1.501    | 183.0    |
| msgspec       | mixed       | 1.468    | 1.512       | 1.666    | 183.0    |
| orjson        | mixed       | 1.440    | 1.513       | 1.723    | 183.0    |
| ujson         | mixed       | 5.526    | 5.578       | 5.776    | 183.0    |
| json (stdlib) | mixed       | 38.992   | 39.079      | 40.019   | 183.0    |
| strata        | nested      | 0.997    | 1.182       | 2.265    | 122.9    |
| orjson        | nested      | 1.165    | 1.212       | 1.668    | 122.9    |
| msgspec       | nested      | 1.473    | 1.623       | 1.669    | 122.9    |
| ujson         | nested      | 4.750    | 4.773       | 4.850    | 122.9    |
| json (stdlib) | nested      | 48.816   | 49.122      | 53.005   | 122.9    |
| strata        | users.json  | 9.663    | 10.014      | 13.686   | 123.5    |
| orjson        | users.json  | 12.513   | 12.626      | 12.725   | 146.4    |
| msgspec       | users.json  | 13.112   | 13.946      | 14.074   | 186.8    |
| ujson         | users.json  | 51.585   | 55.350      | 58.410   | 164.4    |
| json (stdlib) | users.json  | 428.869  | 474.266     | 519.248  | 186.9    |
| strata        | wide_arrays | 8.389    | 8.993       | 9.294    | 192.7    |
| msgspec       | wide_arrays | 9.269    | 9.598       | 10.101   | 199.4    |
| orjson        | wide_arrays | 11.452   | 11.697      | 11.785   | 198.3    |
| ujson         | wide_arrays | 41.413   | 42.195      | 45.268   | 199.3    |
| json (stdlib) | wide_arrays | 203.611  | 218.703     | 230.845  | 170.0    |

## search (file-based)

| Query                          | Library            | Min (ms) | Results | RSS (MB) |
| ------------------------------ | ------------------ | -------- | ------- | -------- |
| all user ids                   | strata             | 12.671   | 2000    | 184.6    |
|                                | orjson+jmespath    | 32.956   | 2000    | 207.6    |
|                                | orjson+jsonpath-ng | 39.985   | 2000    | 210.1    |
| all item prices                | strata             | 19.410   | 100999  | 210.8    |
|                                | orjson+jmespath    | 84.454   | 2000    | 220.0    |
|                                | orjson+jsonpath-ng | 277.675  | 100999  | 227.2    |
| recursive price                | strata             | 17.432   | 100999  | 224.9    |
|                                | orjson+jsonpath-ng | 1032.370 | 100999  | 228.7    |
| flat:$.records\[\*\].id        | strata             | 1.952    | 2000    | 142.5    |
|                                | orjson+jmespath    | 5.636    | 2000    | 142.6    |
|                                | orjson+jsonpath-ng | 7.259    | 2000    | 142.6    |
| flat:$.records\[0\]            | strata             | 1.783    | 1       | 142.6    |
|                                | orjson+jmespath    | 4.115    | 1       | 142.6    |
|                                | orjson+jsonpath-ng | 4.118    | 1       | 142.6    |
| nested:$.records\[\*\].id      | strata             | 1.435    | 2000    | 122.9    |
|                                | orjson+jmespath    | 6.252    | 2000    | 123.2    |
|                                | orjson+jsonpath-ng | 8.704    | 2000    | 123.2    |
| nested:$.records\[0\]          | strata             | 1.442    | 1       | 123.2    |
|                                | orjson+jsonpath-ng | 5.003    | 1       | 123.2    |
|                                | orjson+jmespath    | 5.109    | 1       | 123.2    |
| wide_arrays:$.records\[\*\].id | strata             | 10.604   | 2000    | 174.1    |
|                                | orjson+jmespath    | 20.028   | 2000    | 179.8    |
|                                | orjson+jsonpath-ng | 21.650   | 2000    | 180.0    |
| wide_arrays:$.records\[0\]     | strata             | 8.381    | 1       | 180.1    |
|                                | orjson+jmespath    | 14.609   | 1       | 180.1    |
|                                | orjson+jsonpath-ng | 17.879   | 1       | 180.1    |
| mixed:$.records\[\*\].id       | strata             | 1.630    | 2000    | 183.0    |
|                                | orjson+jmespath    | 5.691    | 2000    | 183.0    |
|                                | orjson+jsonpath-ng | 9.861    | 2000    | 183.0    |
| mixed:$.records\[0\]           | strata             | 1.721    | 1       | 183.0    |
|                                | orjson+jmespath    | 5.228    | 1       | 183.0    |
|                                | orjson+jsonpath-ng | 5.527    | 1       | 183.0    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.277    | 2000    | 255.4    |
|                                            | jmespath    | 1.154    | 2000    | 255.4    |
|                                            | jsonpath-ng | 22.042   | 2000    | 256.2    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 9.385    | 100999  | 256.3    |
|                                            | jmespath    | 67.187   | 2000    | 152.0    |
|                                            | jsonpath-ng | 248.324  | 100999  | 157.2    |
| $..price                                   | strata      | 8.028    | 100999  | 157.2    |
|                                            | jsonpath-ng | 930.062  | 100999  | 160.7    |
| $.users\[?(@.age>30)\]                     | strata      | 0.366    | 1606    | 157.8    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 9.657    | 6650    | 153.3    |
| flat:$.records\[\*\].id                    | strata      | 0.122    | 2000    | 142.6    |
|                                            | jmespath    | 0.668    | 2000    | 142.6    |
|                                            | jsonpath-ng | 4.758    | 2000    | 142.6    |
| flat:$.records\[0\]                        | jmespath    | 0.007    | 1       | 142.6    |
|                                            | strata      | 0.007    | 1       | 142.6    |
|                                            | jsonpath-ng | 0.021    | 1       | 142.6    |
| nested:$.records\[\*\].id                  | strata      | 0.183    | 2000    | 123.2    |
|                                            | jmespath    | 0.704    | 2000    | 123.2    |
|                                            | jsonpath-ng | 6.057    | 2000    | 123.2    |
| nested:$.records\[0\]                      | strata      | 0.004    | 1       | 123.2    |
|                                            | jmespath    | 0.010    | 1       | 123.2    |
|                                            | jsonpath-ng | 0.019    | 1       | 123.2    |
| wide_arrays:$.records\[\*\].id             | strata      | 0.106    | 2000    | 180.1    |
|                                            | jmespath    | 0.864    | 2000    | 180.1    |
|                                            | jsonpath-ng | 9.283    | 2000    | 180.1    |
| wide_arrays:$.records\[0\]                 | strata      | 0.013    | 1       | 180.1    |
|                                            | jsonpath-ng | 0.013    | 1       | 180.1    |
|                                            | jmespath    | 0.013    | 1       | 180.1    |
| mixed:$.records\[\*\].id                   | strata      | 0.130    | 2000    | 183.0    |
|                                            | jmespath    | 0.789    | 2000    | 183.0    |
|                                            | jsonpath-ng | 6.283    | 2000    | 183.0    |
| mixed:$.records\[0\]                       | strata      | 0.005    | 1       | 183.0    |
|                                            | jsonpath-ng | 0.010    | 1       | 183.0    |
|                                            | jmespath    | 0.010    | 1       | 183.0    |

## Summary

| Category            | Strata Rank             | vs #1                               |
| ------------------- | ----------------------- | ----------------------------------- |
| loads (JSON)        | **#1** / 5              | **22.1% faster** than #2 (msgspec)  |
| loads (NDJSON)      | **#1** / 5              | **40.6% faster** than #2 (msgspec)  |
| load (JSON file)    | **#2** / 5              | 9.8% behind #1 (msgspec)            |
| load (NDJSON file)  | **#1** / 5              | **19.5% faster** than #2 (orjson)   |
| dumps               | **#1** / 5              | **27.8% faster** than #2 (orjson)   |
| dump                | **#1** / 5              | **29.5% faster** than #2 (orjson)   |
| loads (flat)        | **#1** / 5              | **26.0% faster** than #2 (orjson)   |
| load (flat)         | **#1** / 5              | **40.2% faster** than #2 (orjson)   |
| dumps (flat)        | **#1** / 5              | **1.7% faster** than #2 (orjson)    |
| dump (flat)         | **#1** / 5              | **6.2% faster** than #2 (orjson)    |
| loads (nested)      | **#1** / 5              | **84.6% faster** than #2 (msgspec)  |
| load (nested)       | **#1** / 5              | **92.6% faster** than #2 (orjson)   |
| dumps (nested)      | **#3** / 5              | 3.2% behind #1 (msgspec)            |
| dump (nested)       | **#1** / 5              | **16.9% faster** than #2 (orjson)   |
| loads (wide_arrays) | **#1** / 5              | **16.0% faster** than #2 (orjson)   |
| load (wide_arrays)  | **#1** / 5              | **4.2% faster** than #2 (orjson)    |
| dumps (wide_arrays) | **#1** / 5              | **11.6% faster** than #2 (msgspec)  |
| dump (wide_arrays)  | **#1** / 5              | **10.5% faster** than #2 (msgspec)  |
| loads (mixed)       | **#1** / 5              | **102.6% faster** than #2 (msgspec) |
| load (mixed)        | **#1** / 5              | **35.0% faster** than #2 (msgspec)  |
| dumps (mixed)       | **#2** / 5              | 3.5% behind #1 (orjson)             |
| dump (mixed)        | **#1** / 5              | **4.3% faster** than #2 (orjson)    |
| search (JSONPath)   | **#1** in 11/11 queries | avg **712% faster**                 |
| query (JSONPath)    | **#1** in 12/13 queries | -                                   |
