# Strata Benchmark Results

Generated: 2026-02-10 01:09:03

## Environment

- Commit: 1f670ba759142c9cd50c0f98cc520859e2b32391
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/large/users.json, benchmarks/data/generated/large/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| orjson        | users.json   | 133.475  | 179.700     | 182.829  | 712.0    |
| msgspec       | users.json   | 133.477  | 185.790     | 192.563  | 705.4    |
| strata        | users.json   | 198.400  | 226.547     | 239.052  | 567.1    |
| ujson         | users.json   | 186.243  | 259.440     | 271.618  | 827.8    |
| json (stdlib) | users.json   | 243.214  | 298.417     | 305.373  | 705.4    |
| msgspec       | users.ndjson | 213.454  | 215.390     | 218.460  | 571.1    |
| orjson        | users.ndjson | 188.064  | 220.513     | 225.020  | 575.7    |
| strata        | users.ndjson | 245.455  | 274.694     | 276.089  | 522.5    |
| ujson         | users.ndjson | 257.278  | 312.058     | 316.181  | 693.6    |
| json (stdlib) | users.ndjson | 288.926  | 332.157     | 335.074  | 572.8    |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.608    | 4000    |
|                                     | jmespath    | 1.539    | 4000    |
|                                     | jsonpath-ng | 31.881   | 4000    |
| $.users[*].orders[*].items[*].price | query       | 29.234   | 789913  |
|                                     | jmespath    | 219.820  | 4000    |
|                                     | jsonpath-ng | 1052.637 | 789913  |
| $..price                            | jmespath    | 217.492  | 4000    |
|                                     | query       | 261.149  | 789913  |
|                                     | jsonpath-ng | 3774.418 | 789913  |
| $.users[?(@.age>30)]                | query       | 0.585    | 3212    |
| $..orders[?(@.status=="shipped")]   | query       | 281.947  | 52812   |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 125.83   | 176.98      | 189.10   | 712.3    |
| msgspec       | 133.57   | 189.49      | 192.41   | 703.6    |
| strata        | 200.59   | 223.95      | 233.12   | 565.4    |
| ujson         | 187.83   | 263.74      | 265.32   | 826.1    |
| json (stdlib) | 243.17   | 300.08      | 306.73   | 703.8    |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 169.24   | 207.59      | 208.74   | 878.8    |
| msgspec       | 182.62   | 214.23      | 225.32   | 869.1    |
| strata        | 220.21   | 234.54      | 234.72   | 877.5    |
| ujson         | 236.23   | 299.68      | 312.50   | 991.8    |
| json (stdlib) | 308.26   | 330.27      | 335.05   | 872.0    |
