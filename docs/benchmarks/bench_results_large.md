# Strata Benchmark Results

Generated: 2026-02-09 12:58:47

## Environment

- Commit: 488cf8bec2f660d9fa5b253b8abf51696ad6b26d
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
| orjson | users.json | 126.835 | 178.860 | 184.995 | 713.0 |
| msgspec | users.json | 138.059 | 194.405 | 229.229 | 704.5 |
| strata | users.json | 195.782 | 219.906 | 226.879 | 566.1 |
| ujson | users.json | 187.979 | 263.764 | 266.067 | 827.9 |
| json (stdlib) | users.json | 246.986 | 300.137 | 314.152 | 704.5 |
| orjson | users.ndjson | 170.371 | 209.294 | 212.395 | 577.7 |
| msgspec | users.ndjson | 179.578 | 218.739 | 224.299 | 571.1 |
| strata | users.ndjson | 237.272 | 268.899 | 269.476 | 523.6 |
| ujson | users.ndjson | 240.337 | 301.237 | 301.685 | 694.6 |
| json (stdlib) | users.ndjson | 293.333 | 327.642 | 331.277 | 573.8 |

## Query Benchmarks

| Query | Library | Min (ms) | Results |
|-------|---------|----------|----------|
| $.users[*].id | query | 0.633 | 4000 |
|  | jmespath | 1.708 | 4000 |
|  | jsonpath-ng | 34.463 | 4000 |
| $.users[*].orders[*].items[*].price | query | 29.575 | 789913 |
|  | jmespath | 221.918 | 4000 |
|  | jsonpath-ng | 1080.743 | 789913 |
| $..price | jmespath | 218.948 | 4000 |
|  | query | 266.699 | 789913 |
|  | jsonpath-ng | 3783.052 | 789913 |
| $.users[?(@.age>30)] | query | 0.582 | 3212 |
| $..orders[?(@.status=="shipped")] | query | 278.968 | 52812 |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/large/users.json
- Input size: 43.85 MB
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 127.35 | 179.83 | 182.02 | 712.4 |
| msgspec | 134.27 | 189.10 | 190.74 | 703.7 |
| strata | 197.83 | 221.81 | 222.07 | 565.5 |
| ujson | 185.89 | 264.44 | 264.52 | 826.2 |
| json (stdlib) | 247.52 | 301.59 | 301.83 | 703.8 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Input size: 43.85 MB
- Lines: 4000
- Repeat: 3
- Warmup: 1

| Library | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
|---------|----------|-------------|----------|---------|
| orjson | 171.80 | 210.98 | 211.94 | 878.0 |
| msgspec | 174.90 | 217.26 | 217.40 | 869.3 |
| strata | 216.73 | 228.95 | 231.39 | 877.6 |
| ujson | 241.67 | 299.20 | 299.30 | 991.9 |
| json (stdlib) | 291.88 | 330.32 | 334.83 | 872.1 |
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
| Deep path navigation | jmespath | 0.01 | 0.01 | 0.02 | 1 | 1583.6 |
| Deep path navigation | strata | 0.01 | 0.01 | 0.02 | 1 | 1606.6 |
| Deep path navigation | jsonpath-ng | 0.02 | 0.02 | 0.02 | 1 | 1583.6 |
| Extract all user IDs | jmespath | 1.67 | 1.74 | 1.82 | 4000 | 1059.1 |
| Extract all user IDs | strata | 3.54 | 3.57 | 3.61 | 4000 | 1058.9 |
| Extract all user IDs | jsonpath-ng | 32.39 | 32.44 | 32.49 | 4000 | 1061.0 |
| Extract all user names | jmespath | 1.26 | 1.29 | 1.32 | 4000 | 1583.8 |
| Extract all user names | strata | 3.69 | 3.72 | 3.76 | 4000 | 1583.8 |
| Extract all user names | jsonpath-ng | 32.16 | 32.22 | 32.29 | 4000 | 1583.8 |
| Extract nested timestamp field | jmespath | 2.56 | 2.63 | 2.69 | 4000 | 1410.9 |
| Extract nested timestamp field | strata | 4.18 | 4.49 | 4.81 | 4000 | 1410.9 |
| Extract nested timestamp field | jsonpath-ng | 35.12 | 35.16 | 35.19 | 4000 | 1412.0 |
| Extract order item prices (double wildcard) | jmespath | 218.06 | 219.81 | 221.56 | 4000 | 1600.3 |
| Extract order item prices (double wildcard) | strata | 745.33 | 746.64 | 747.96 | 789913 | 1586.6 |
| Extract order item prices (double wildcard) | jsonpath-ng | 1071.05 | 1074.67 | 1078.29 | 789913 | 1616.4 |
| Filter users by age (numeric predicate) | strata | 3.07 | 3.12 | 3.17 | 3212 | 1583.8 |
| Filter users by age (numeric predicate) | jmespath | 4.84 | 4.84 | 4.84 | 3212 | 1583.8 |
| Recursively find all prices | strata | 1045.41 | 1045.59 | 1045.76 | 789913 | 1624.6 |
| Recursively find all prices | jsonpath-ng | 3794.95 | 3796.33 | 3797.70 | 789913 | 1629.0 |
| Slice first 10 users, extract IDs | jmespath | 0.02 | 0.02 | 0.02 | 10 | 1614.6 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.02 | 0.02 | 0.02 | 10 | 1613.6 |
| Slice first 10 users, extract IDs | strata | 0.02 | 0.02 | 0.02 | 10 | 1631.6 |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/large/users.ndjson
- Size: 43.85 MB
- Records: 4000
- Repeat: 2
- Warmup: 1
- Strata mode: dict (NDJSON cursor unsupported)

| Query | Library | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
|-------|---------|----------|-------------|----------|---------|---------|
| Deep path navigation | jsonpath-ng | 0.01 | 0.01 | 0.02 | 1 | 1903.3 |
| Deep path navigation | jmespath | 0.01 | 0.02 | 0.02 | 1 | 1903.3 |
| Deep path navigation | strata | 467.43 | 469.65 | 471.86 | 1 | 1920.3 |
| Extract all user IDs | jmespath | 2.02 | 2.03 | 2.04 | 4000 | 1370.0 |
| Extract all user IDs | jsonpath-ng | 61.64 | 62.39 | 63.14 | 4000 | 1371.9 |
| Extract all user IDs | strata | 478.78 | 485.48 | 492.18 | 4000 | 1369.7 |
| Extract all user names | jmespath | 2.00 | 2.03 | 2.07 | 4000 | 2033.7 |
| Extract all user names | jsonpath-ng | 61.65 | 61.73 | 61.82 | 4000 | 2034.2 |
| Extract all user names | strata | 468.45 | 471.48 | 474.50 | 4000 | 2033.7 |
| Extract nested timestamp field | jmespath | 3.25 | 3.26 | 3.26 | 4000 | 1512.6 |
| Extract nested timestamp field | jsonpath-ng | 64.78 | 65.15 | 65.51 | 4000 | 1512.7 |
| Extract nested timestamp field | strata | 470.58 | 471.38 | 472.19 | 4000 | 1512.6 |
| Extract order item prices (double wildcard) | jmespath | 182.71 | 183.48 | 184.25 | 789913 | 1816.3 |
| Extract order item prices (double wildcard) | jsonpath-ng | 1011.53 | 1011.80 | 1012.06 | 789913 | 1825.7 |
| Extract order item prices (double wildcard) | strata | 1225.77 | 1240.52 | 1255.27 | 789913 | 1805.1 |
| Filter users by age (numeric predicate) | jmespath | 5.19 | 5.23 | 5.28 | 3212 | 2179.7 |
| Filter users by age (numeric predicate) | strata | 474.17 | 475.15 | 476.13 | 3212 | 2179.7 |
| Recursively find all prices | strata | 1518.57 | 1529.06 | 1539.56 | 789913 | 2192.0 |
| Recursively find all prices | jsonpath-ng | 3700.58 | 3724.70 | 3748.82 | 789913 | 2020.6 |
| Slice first 10 users, extract IDs | jmespath | 0.02 | 0.02 | 0.02 | 10 | 2119.0 |
| Slice first 10 users, extract IDs | jsonpath-ng | 0.02 | 0.03 | 0.04 | 10 | 2119.0 |
| Slice first 10 users, extract IDs | strata | 470.32 | 475.87 | 481.42 | 10 | 2129.0 |
