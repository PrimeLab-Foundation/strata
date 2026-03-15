# Strata Benchmark Results

Generated: 2026-03-15 03:54:00

## Environment

- Commit: 18a352195ff912a27133fc13a4b7ba44d44cbf4a
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
| strata        | users.json   | 173.159  | 209.399     | 212.355  | 340.1    |
| orjson        | users.json   | 201.724  | 263.863     | 277.864  | 478.1    |
| msgspec       | users.json   | 217.857  | 298.725     | 304.951  | 468.6    |
| ujson         | users.json   | 289.070  | 414.132     | 432.565  | 591.0    |
| json (stdlib) | users.json   | 392.583  | 487.886     | 489.945  | 512.5    |
| strata        | users.ndjson | 172.735  | 214.591     | 220.508  | 613.3    |
| orjson        | users.ndjson | 270.980  | 301.466     | 319.697  | 663.9    |
| msgspec       | users.ndjson | 273.852  | 329.454     | 346.000  | 656.3    |
| ujson         | users.ndjson | 375.772  | 464.475     | 470.239  | 778.8    |
| json (stdlib) | users.ndjson | 476.412  | 512.576     | 533.827  | 659.0    |

## load (file-based)

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| strata        | users.json   | 177.370  | 212.696     | 213.289  | 522.3    |
| orjson        | users.json   | 194.663  | 270.830     | 281.333  | 521.4    |
| msgspec       | users.json   | 209.420  | 291.861     | 310.162  | 556.5    |
| ujson         | users.json   | 308.496  | 407.974     | 419.483  | 679.0    |
| json (stdlib) | users.json   | 414.466  | 496.768     | 515.973  | 556.5    |
| strata        | users.ndjson | 178.537  | 221.060     | 229.984  | 664.9    |
| msgspec       | users.ndjson | 281.326  | 318.788     | 327.380  | 700.3    |
| orjson        | users.ndjson | 266.331  | 328.047     | 333.239  | 663.0    |
| ujson         | users.ndjson | 372.307  | 458.926     | 474.828  | 822.8    |
| json (stdlib) | users.ndjson | 459.476  | 497.329     | 541.780  | 703.0    |

## dumps (in-memory serialization)

| Library | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------- | ---------- | -------- | ----------- | -------- | -------- |
| strata  | users.json | 70.075   | 72.907      | 77.480   | 591.0    |
| orjson  | users.json | 78.542   | 78.550      | 84.099   | 747.6    |
| msgspec | users.json | 78.316   | 79.647      | 82.135   | 944.8    |
| ujson   | users.json | 296.476  | 297.289     | 298.523  | 1139.9   |
| json    | users.json | 418.063  | 419.466     | 428.468  | 1354.6   |

## dump (file-based)

| Library       | Dataset    | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ---------- | -------- | ----------- | -------- | -------- |
| strata        | users.json | 74.921   | 76.595      | 76.923   | 1398.5   |
| msgspec       | users.json | 87.519   | 97.144      | 101.208  | 1865.8   |
| orjson        | users.json | 91.296   | 101.654     | 105.458  | 1554.2   |
| ujson         | users.json | 309.538  | 311.824     | 317.373  | 1710.0   |
| json (stdlib) | users.json | 2939.606 | 3007.082    | 3019.492 | 1727.8   |

## search (file-based)

| Query           | Library            | Min (ms) | Results | RSS (MB) |
| --------------- | ------------------ | -------- | ------- | -------- |
| all user ids    | strata             | 68.941   | 4000    | 1505.9   |
|                 | orjson+jsonpath-ng | 206.238  | 4000    | 1440.0   |
|                 | orjson+jmespath    | 210.742  | 4000    | 1430.2   |
| all item prices | strata             | 79.934   | 789913  | 1466.1   |
|                 | orjson+jmespath    | 534.897  | 4000    | 1527.8   |
|                 | orjson+jsonpath-ng | 1945.056 | 789913  | 1142.7   |
| recursive price | strata             | 79.031   | 789913  | 1117.2   |
|                 | orjson+jsonpath-ng | 6171.709 | 789913  | 713.8    |

## Query Benchmarks

| Query                                      | Library     | Min (ms) | Results | RSS (MB) |
| ------------------------------------------ | ----------- | -------- | ------- | -------- |
| $.users\[\*\].id                           | strata      | 0.735    | 4000    | 941.3    |
|                                            | jmespath    | 2.153    | 4000    | 941.3    |
|                                            | jsonpath-ng | 86.271   | 4000    | 943.0    |
| $.users\[*\].orders\[*\].items\[\*\].price | strata      | 44.729   | 789913  | 789.1    |
|                                            | jmespath    | 408.631  | 4000    | 803.9    |
|                                            | jsonpath-ng | 1722.522 | 789913  | 878.9    |
| $..price                                   | strata      | 56.384   | 789913  | 896.5    |
|                                            | jsonpath-ng | 6126.365 | 789913  | 996.4    |
| $.users\[?(@.age>30)\]                     | strata      | 0.826    | 3212    | 830.5    |
| $..orders\[?(@.status=="shipped")\]        | strata      | 60.533   | 26406   | 829.5    |

## Summary

| Category           | Strata Rank           | vs #1                              |
| ------------------ | --------------------- | ---------------------------------- |
| loads (JSON)       | **#1** / 5            | **16.5% faster** than #2 (orjson)  |
| loads (NDJSON)     | **#1** / 5            | **56.9% faster** than #2 (orjson)  |
| load (JSON file)   | **#1** / 5            | **9.8% faster** than #2 (orjson)   |
| load (NDJSON file) | **#1** / 5            | **49.2% faster** than #2 (orjson)  |
| dumps              | **#1** / 5            | **11.8% faster** than #2 (msgspec) |
| dump               | **#1** / 5            | **16.8% faster** than #2 (msgspec) |
| search (JSONPath)  | **#1** in 3/3 queries | avg **2826% faster**               |
| query (JSONPath)   | **#1** in 5/5 queries | -                                  |
