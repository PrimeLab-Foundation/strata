# Strata Benchmark Results

Generated: 2026-02-09 15:58:17

## Environment

- Commit: 997f6c7369aa9f5a3a7262bf5222e121a1ac2e09
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/large/users.json, benchmarks/data/generated/large/users.ndjson

## Parsing Benchmarks

| Library | Dataset | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|---------|----------|-------------|----------|---------|
| orjson | users.json | 129.388 | 181.905 | 189.619 | 713.1 |
| msgspec | users.json | 137.399 | 191.537 | 193.515 | 704.5 |
| strata | users.json | 207.511 | 230.037 | 234.574 | 566.2 |
| ujson | users.json | 190.641 | 264.297 | 283.510 | 827.9 |
| json (stdlib) | users.json | 250.574 | 299.976 | 308.494 | 704.5 |
| msgspec | users.ndjson | 181.874 | 222.753 | 235.332 | 591.9 |
| orjson | users.ndjson | 176.583 | 226.411 | 226.615 | 598.4 |
| strata | users.ndjson | 253.523 | 282.426 | 290.181 | 544.2 |
| ujson | users.ndjson | 264.919 | 309.774 | 328.518 | 715.3 |
| json (stdlib) | users.ndjson | 297.247 | 343.712 | 346.190 | 594.6 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | query | 0.599 | 4000 |
|  | jmespath | 1.773 | 4000 |
|  | jsonpath-ng | 33.797 | 4000 |
| $.users[*].orders[*].items[*].price | query | 28.160 | 789913 |
|  | jmespath | 219.464 | 4000 |
|  | jsonpath-ng | 1084.353 | 789913 |
| $..price | jmespath | 220.559 | 4000 |
|  | query | 265.948 | 789913 |
|  | jsonpath-ng | 3869.237 | 789913 |
| $.users[?(@.age>30)] | query | 0.608 | 3212 |
| $..orders[?(@.status=="shipped")] | query | 285.969 | 52812 |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 132.91 | 187.29 | 189.79 | 712.5 |
| msgspec | 148.19 | 193.09 | 203.10 | 703.8 |
| strata | 211.71 | 238.56 | 241.19 | 565.6 |
| ujson | 186.28 | 277.66 | 280.55 | 826.4 |
| json (stdlib) | 251.73 | 306.12 | 316.95 | 703.9 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 187.70 | 233.59 | 240.56 | 1291.4 |
| msgspec | 223.61 | 249.69 | 250.67 | 1282.7 |
| strata | 244.74 | 256.99 | 304.05 | 1291.0 |
| ujson | 257.55 | 327.11 | 349.51 | 1405.3 |
| json (stdlib) | 310.24 | 368.24 | 370.06 | 962.4 |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | strata | 0.01 | 0.01 | 0.02 | 1 | 1606.5 |
| Deep path navigation | jmespath | 0.02 | 0.02 | 0.03 | 1 | 1583.5 |
| Deep path navigation | jsonpath-ng | 0.03 | 0.03 | 0.03 | 1 | 1583.5 |
| Extract all user IDs | jmespath | 1.89 | 1.93 | 1.96 | 4000 | 1059.7 |
| Extract all user IDs | strata | 3.76 | 3.96 | 4.17 | 4000 | 1059.4 |
| Extract all user IDs | jsonpath-ng | 32.20 | 33.65 | 35.10 | 4000 | 1061.6 |
| Extract all user names | jmespath | 1.28 | 1.35 | 1.41 | 4000 | 1583.7 |
| Extract all user names | strata | 3.65 | 3.82 | 3.99 | 4000 | 1583.7 |
| Extract all user names | jsonpath-ng | 35.84 | 36.16 | 36.48 | 4000 | 1583.7 |
| Extract nested timestamp field | jmespath | 2.54 | 2.61 | 2.68 | 4000 | 1411.5 |
| Extract nested timestamp field | strata | 4.04 | 4.04 | 4.04 | 4000 | 1411.5 |
| Extract nested timestamp field | jsonpath-ng | 34.58 | 34.85 | 35.12 | 4000 | 1412.6 |
| Extract order item prices (double wildcard) | jmespath | 227.93 | 231.72 | 235.50 | 4000 | 1600.4 |
| Extract order item prices (double wildcard) | strata | 793.38 | 858.54 | 923.70 | 789913 | 1586.7 |
| Extract order item prices (double wildcard) | jsonpath-ng | 1081.91 | 1100.15 | 1118.40 | 789913 | 1616.3 |
| Filter users by age (numeric predicate) | strata | 3.21 | 3.60 | 4.00 | 3212 | 1583.7 |
| Filter users by age (numeric predicate) | jmespath | 4.94 | 5.00 | 5.06 | 3212 | 1583.8 |
| Recursively find all prices | strata | 1079.08 | 1079.15 | 1079.21 | 789913 | 1622.7 |
| Recursively find all prices | jsonpath-ng | 3851.50 | 3878.54 | 3905.59 | 789913 | 1627.1 |
| Slice first 10 users, extract IDs | jmespath | 0.02 | 0.02 | 0.02 | 10 | 1612.7 |
| Slice first 10 users, extract IDs | strata | 0.03 | 0.03 | 0.03 | 10 | 1629.7 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.03 | 0.03 | 0.04 | 10 | 1611.7 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | jsonpath-ng | 0.02 | 0.02 | 0.02 | 1 | 1901.7 |
| Deep path navigation | jmespath | 0.02 | 0.02 | 0.03 | 1 | 1901.7 |
| Deep path navigation | strata | 468.10 | 470.80 | 473.50 | 1 | 1918.7 |
| Extract all user IDs | jmespath | 1.97 | 1.99 | 2.02 | 4000 | 1369.5 |
| Extract all user IDs | jsonpath-ng | 61.92 | 63.07 | 64.21 | 4000 | 1371.3 |
| Extract all user IDs | strata | 485.97 | 492.26 | 498.55 | 4000 | 1369.2 |
| Extract all user names | jmespath | 2.00 | 2.21 | 2.42 | 4000 | 2033.1 |
| Extract all user names | jsonpath-ng | 62.34 | 65.55 | 68.76 | 4000 | 2033.6 |
| Extract all user names | strata | 475.83 | 477.59 | 479.35 | 4000 | 2033.1 |
| Extract nested timestamp field | jmespath | 3.48 | 3.54 | 3.60 | 4000 | 1511.2 |
| Extract nested timestamp field | jsonpath-ng | 70.62 | 73.91 | 77.20 | 4000 | 1511.2 |
| Extract nested timestamp field | strata | 476.84 | 496.24 | 515.64 | 4000 | 1511.1 |
| Extract order item prices (double wildcard) | jmespath | 182.84 | 183.74 | 184.64 | 789913 | 1815.1 |
| Extract order item prices (double wildcard) | jsonpath-ng | 1019.47 | 1023.75 | 1028.03 | 789913 | 1824.1 |
| Extract order item prices (double wildcard) | strata | 1226.38 | 1278.57 | 1330.76 | 789913 | 1803.5 |
| Filter users by age (numeric predicate) | jmespath | 5.54 | 5.55 | 5.57 | 3212 | 2178.2 |
| Filter users by age (numeric predicate) | strata | 473.21 | 480.93 | 488.66 | 3212 | 2178.2 |
| Recursively find all prices | strata | 1566.32 | 1580.52 | 1594.71 | 789913 | 2330.0 |
| Recursively find all prices | jsonpath-ng | 3722.77 | 3752.18 | 3781.58 | 789913 | 1985.1 |
| Slice first 10 users, extract IDs | jmespath | 0.02 | 0.03 | 0.03 | 10 | 2084.6 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.03 | 0.04 | 0.04 | 10 | 2083.6 |
| Slice first 10 users, extract IDs | strata | 479.50 | 480.50 | 481.49 | 10 | 2093.6 |
