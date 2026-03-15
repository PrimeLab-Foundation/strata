# Strata Benchmark Results

Generated: 2026-03-15 02:20:08

## Environment

- Commit: 1ad8828b764067899097f4fab331b0010dccd034
- OS: macOS-26.3-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.3

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/large/users.json, benchmarks/data/generated/large/users.ndjson

## loads (in-memory parsing)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 166.405  | 201.771     | 207.223  | 340.4    |
| orjson        | users.json   | 188.327  | 256.332     | 260.632  | 478.4    |
| msgspec       | users.json   | 196.302  | 273.819     | 287.907  | 470.8    |
| ujson         | users.json   | 268.842  | 367.236     | 371.579  | 593.2    |
| json (stdlib) | users.json   | 400.995  | 464.148     | 472.466  | 514.7    |
| strata        | users.ndjson | 169.269  | 211.943     | 212.793  | 563.8    |
| orjson        | users.ndjson | 240.814  | 298.137     | 303.966  | 611.0    |
| msgspec       | users.ndjson | 247.532  | 308.457     | 313.002  | 604.7    |
| ujson         | users.ndjson | 337.759  | 413.056     | 425.811  | 727.2    |
| json (stdlib) | users.ndjson | 431.339  | 492.390     | 509.699  | 607.4    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 171.706  | 213.168     | 214.897  | 522.5    |
| orjson        | users.json   | 188.413  | 260.708     | 270.427  | 521.5    |
| msgspec       | users.json   | 200.978  | 282.523     | 284.791  | 558.6    |
| ujson         | users.json   | 282.146  | 386.126     | 399.731  | 681.1    |
| json (stdlib) | users.json   | 397.381  | 467.295     | 522.663  | 557.7    |
| strata        | users.ndjson | 175.729  | 213.172     | 218.922  | 612.2    |
| orjson        | users.ndjson | 244.867  | 296.088     | 305.808  | 612.4    |
| msgspec       | users.ndjson | 244.972  | 322.446     | 323.656  | 648.7    |
| ujson         | users.ndjson | 339.512  | 422.465     | 430.583  | 771.2    |
| json (stdlib) | users.ndjson | 428.565  | 481.578     | 489.730  | 650.4    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 67.936   | 68.005      | 68.725   | 593.2    |
| msgspec | users.json | 74.475   | 74.588      | 77.724   | 945.4    |
| orjson  | users.json | 77.253   | 77.456      | 78.417   | 750.0    |
| ujson   | users.json | 282.886  | 283.402     | 286.320  | 1140.3   |
| json    | users.json | 405.279  | 406.402     | 410.467  | 1218.8   |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 74.411   | 75.438      | 76.337   | 1262.7   |
| msgspec       | users.json | 80.113   | 80.970      | 81.310   | 1691.0   |
| orjson        | users.json | 81.893   | 82.125      | 82.284   | 1418.4   |
| ujson         | users.json | 290.885  | 293.768     | 295.037  | 1574.1   |
| json (stdlib) | users.json | 2870.591 | 2898.516    | 2933.966 | 1535.3   |

## search (file-based)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 70.304   | 4000    | 1334.0   |
|                 | orjson+jsonpath-ng | 201.078  | 4000    | 1695.1   |
|                 | orjson+jmespath    | 205.790  | 4000    | 1685.5   |
| all item prices | strata             | 74.390   | 789913  | 1722.2   |
|                 | orjson+jmespath    | 519.292  | 4000    | 1316.1   |
|                 | orjson+jsonpath-ng | 1829.717 | 789913  | 1346.9   |
| recursive price | strata             | 77.688   | 789913  | 922.8    |
|                 | orjson+jsonpath-ng | 6109.730 | 789913  | 716.6    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.726    | 4000    | 943.1    |
|                                            | jmespath    | 2.096    | 4000    | 943.1    |
|                                            | jsonpath-ng | 77.285   | 4000    | 944.8    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 43.046   | 789913  | 944.2    |
|                                            | jmespath    | 390.092  | 4000    | 959.7    |
|                                            | jsonpath-ng | 1605.440 | 789913  | 809.3    |
| $..price                                   | strata      | 56.548   | 789913  | 822.6    |
|                                            | jsonpath-ng | 5893.479 | 789913  | 821.4    |
| $.users\[?(@.age>30)\]                     | strata      | 0.855    | 3212    | 824.9    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 60.677   | 26406   | 824.0    |

## Summary

| Category           | Strata Rank           | vs #1                             |
| ------------------ | --------------------- | --------------------------------- |
| loads (JSON)       | **#1** / 5            | **13.2% faster** than #2 (orjson) |
| loads (NDJSON)     | **#1** / 5            | **42.3% faster** than #2 (orjson) |
| load (JSON file)   | **#1** / 5            | **9.7% faster** than #2 (orjson)  |
| load (NDJSON file) | **#1** / 5            | **39.3% faster** than #2 (orjson) |
| dumps              | **#1** / 5            | **9.6% faster** than #2 (msgspec) |
| dump               | **#1** / 5            | **7.7% faster** than #2 (msgspec) |
| search (JSONPath)  | **#1** in 3/3 queries | avg **2849% faster**              |
| query (JSONPath)   | **#1** in 5/5 queries | -                                 |
