# Strata Benchmark Results

Generated: 2026-02-10 01:08:02

## Environment

- Commit: 1f670ba759142c9cd50c0f98cc520859e2b32391
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2

## Configuration

- Repeat: 3
- Warmup: 1
- Datasets: benchmarks/data/generated/small/users.json, benchmarks/data/generated/small/users.ndjson

## Parsing Benchmarks

| Library       | Dataset      | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | ------------ | -------- | ----------- | -------- | -------- |
| msgspec       | users.json   | 3.004    | 3.575       | 3.976    | 48.6     |
| orjson        | users.json   | 2.952    | 3.668       | 3.750    | 45.5     |
| strata        | users.json   | 4.343    | 4.760       | 5.076    | 41.4     |
| json (stdlib) | users.json   | 5.460    | 6.067       | 6.533    | 49.1     |
| ujson         | users.json   | 4.021    | 6.165       | 6.255    | 46.5     |
| orjson        | users.ndjson | 2.922    | 3.476       | 3.655    | 52.4     |
| msgspec       | users.ndjson | 2.917    | 3.617       | 3.702    | 54.5     |
| strata        | users.ndjson | 4.465    | 5.507       | 5.848    | 51.4     |
| ujson         | users.ndjson | 3.995    | 5.523       | 5.740    | 54.5     |
| json (stdlib) | users.ndjson | 5.696    | 6.503       | 6.604    | 54.5     |

## Query Benchmarks

| Query                               | Library     | Min (ms) | Results |
| ----------------------------------- | ----------- | -------- | ------- |
| $.users[*].id                       | query       | 0.028    | 1000    |
|                                     | jmespath    | 0.190    | 1000    |
|                                     | jsonpath-ng | 1.576    | 1000    |
| $.users[*].orders[*].items[*].price | query       | 0.531    | 12046   |
|                                     | jmespath    | 5.537    | 1000    |
|                                     | jsonpath-ng | 18.456   | 12046   |
| $..price                            | jmespath    | 5.679    | 1000    |
|                                     | query       | 5.870    | 12046   |
|                                     | jsonpath-ng | 71.687   | 12046   |
| $.users[?(@.age>30)]                | query       | 0.031    | 794     |
| $..orders[?(@.status=="shipped")]   | query       | 6.365    | 3208    |
## Loads Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Input size: 0.96 MB
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| msgspec       | 2.78     | 3.86        | 4.00     | 43.2     |
| orjson        | 3.21     | 4.05        | 4.71     | 43.1     |
| strata        | 4.74     | 5.30        | 6.07     | 39.7     |
| ujson         | 4.39     | 5.40        | 6.03     | 46.2     |
| json (stdlib) | 5.30     | 6.11        | 6.60     | 46.6     |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Input size: 0.96 MB
- Lines: 1000
- Repeat: 3
- Warmup: 1

| Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB) |
| ------------- | -------- | ----------- | -------- | -------- |
| orjson        | 2.68     | 3.60        | 4.52     | 36.2     |
| msgspec       | 2.74     | 4.02        | 4.03     | 36.3     |
| strata        | 3.86     | 4.41        | 4.92     | 35.7     |
| ujson         | 3.96     | 5.27        | 5.85     | 39.3     |
| json (stdlib) | 5.71     | 6.64        | 6.76     | 38.3     |
## Search Benchmarks

### users.json (JSON)

- Source: benchmarks/data/generated/small/users.json
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.00     | 0.01        | 0.01     | 1       | 65.4     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.01     | 1       | 65.4     |
| Deep path navigation                        | jsonpath-ng | 0.01     | 0.01        | 0.01     | 1       | 65.4     |
| Extract all user IDs                        | jmespath    | 0.19     | 0.19        | 0.20     | 1000    | 49.6     |
| Extract all user IDs                        | strata      | 0.95     | 1.00        | 1.06     | 1000    | 49.4     |
| Extract all user IDs                        | jsonpath-ng | 1.60     | 1.93        | 2.25     | 1000    | 50.3     |
| Extract all user names                      | jmespath    | 0.19     | 0.20        | 0.22     | 1000    | 65.4     |
| Extract all user names                      | strata      | 1.30     | 1.30        | 1.31     | 1000    | 65.4     |
| Extract all user names                      | jsonpath-ng | 1.49     | 1.64        | 1.78     | 1000    | 65.4     |
| Extract nested timestamp field              | jmespath    | 0.65     | 0.71        | 0.78     | 1000    | 61.0     |
| Extract nested timestamp field              | strata      | 1.19     | 1.25        | 1.30     | 1000    | 61.0     |
| Extract nested timestamp field              | jsonpath-ng | 2.42     | 2.76        | 3.10     | 1000    | 61.4     |
| Extract order item prices (double wildcard) | jmespath    | 5.74     | 5.78        | 5.83     | 1000    | 64.2     |
| Extract order item prices (double wildcard) | strata      | 10.15    | 11.07       | 11.99    | 12046   | 63.7     |
| Extract order item prices (double wildcard) | jsonpath-ng | 17.40    | 17.58       | 17.76    | 12046   | 67.6     |
| Filter users by age (numeric predicate)     | strata      | 0.92     | 0.99        | 1.05     | 794     | 65.4     |
| Filter users by age (numeric predicate)     | jmespath    | 1.13     | 1.14        | 1.15     | 794     | 65.4     |
| Recursively find all prices                 | strata      | 28.92    | 28.92       | 28.92    | 12046   | 65.8     |
| Recursively find all prices                 | jsonpath-ng | 71.76    | 71.86       | 71.96    | 12046   | 67.5     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 10      | 65.8     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 10      | 65.8     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.03     | 10      | 65.8     |
### users.ndjson (NDJSON)

- Source: benchmarks/data/generated/small/users.ndjson
- Size: 0.96 MB
- Records: 1000
- Repeat: 2
- Warmup: 1
- Strata mode: ndjson_cursor

| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.01     | 0       | 58.4     |
| Deep path navigation                        | jmespath    | 0.01     | 0.01        | 0.02     | 1       | 58.4     |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 1       | 58.4     |
| Extract all user IDs                        | jmespath    | 0.22     | 0.22        | 0.22     | 1000    | 46.5     |
| Extract all user IDs                        | jsonpath-ng | 1.55     | 1.57        | 1.60     | 1000    | 47.1     |
| Extract all user IDs                        | strata      | 2.48     | 2.49        | 2.50     | 0       | 46.5     |
| Extract all user names                      | jmespath    | 0.24     | 0.24        | 0.24     | 1000    | 58.4     |
| Extract all user names                      | jsonpath-ng | 1.72     | 1.88        | 2.04     | 1000    | 58.4     |
| Extract all user names                      | strata      | 2.52     | 2.56        | 2.61     | 0       | 58.4     |
| Extract nested timestamp field              | jmespath    | 0.59     | 0.61        | 0.63     | 1000    | 57.8     |
| Extract nested timestamp field              | jsonpath-ng | 2.16     | 2.36        | 2.56     | 1000    | 58.1     |
| Extract nested timestamp field              | strata      | 2.50     | 2.53        | 2.55     | 0       | 57.8     |
| Extract order item prices (double wildcard) | strata      | 2.44     | 2.44        | 2.44     | 0       | 58.1     |
| Extract order item prices (double wildcard) | jmespath    | 3.44     | 3.54        | 3.63     | 12046   | 58.3     |
| Extract order item prices (double wildcard) | jsonpath-ng | 18.36    | 18.44       | 18.52    | 12046   | 60.4     |
| Filter users by age (numeric predicate)     | strata      | 0.01     | 0.01        | 0.02     | 0       | 58.4     |
| Filter users by age (numeric predicate)     | jmespath    | 1.10     | 1.16        | 1.22     | 794     | 58.4     |
| Recursively find all prices                 | strata      | 29.74    | 30.90       | 32.06    | 878     | 59.2     |
| Recursively find all prices                 | jsonpath-ng | 71.42    | 71.68       | 71.94    | 12046   | 62.4     |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.01     | 0       | 61.4     |
| Slice first 10 users, extract IDs           | jmespath    | 0.01     | 0.01        | 0.01     | 10      | 61.4     |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.01     | 0.02        | 0.02     | 10      | 60.7     |
