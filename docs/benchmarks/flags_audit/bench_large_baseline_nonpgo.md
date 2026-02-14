# Strata Benchmark Results — LARGE-BASELINE-NONPGO
Generated: 2026-02-14 21:05:43

## Environment
- Commit: ce4d80c0cfcd8bbfe70be79dc3e5b1150ad13bb4
- OS: macOS-26.1-arm64-arm-64bit-Mach-O
- CPU: arm64
- Python: 3.14.2
- Compiler: Apple clang version 17.0.0 (clang-1700.6.3.2)

## Configuration
- Repeat/Warmup (loads): 5 / 2
- Repeat/Warmup (dumps): 5 / 2
- Repeat/Warmup (search): 3 / 1
- Repeat/Warmup (cursor_reuse): 3 / 1

## Datasets
- json: benchmarks/data/generated/large/users.json (43.85 MB, 4000 records)
- ndjson: benchmarks/data/generated/large/users.ndjson (43.85 MB, 4000 records, 4000 lines)

## Parsing Benchmarks (loads)
### JSON — users.json (43.85 MB)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 210.47   | 266.76      | 270.07   | 172.37 | 1402.5   |
| msgspec       | 217.41   | 277.94      | 286.24   | 165.44 | 1383.9   |
| ujson         | 271.69   | 358.53      | 360.90   | 128.25 | 1505.4   |
| strata        | 348.15   | 377.08      | 381.27   | 121.95 | 1263.5   |
| json (stdlib) | 337.18   | 397.88      | 403.99   | 115.57 | 1383.9   |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| strata        | 203.80   | 230.06      | 231.67   | 199.85 | 2139.7   |
| orjson        | 181.90   | 242.18      | 247.27   | 189.85 | 2140.7   |
| msgspec       | 188.81   | 252.05      | 255.34   | 182.42 | 2120.0   |
| ujson         | 242.34   | 333.09      | 334.03   | 138.04 | 2241.7   |
| json (stdlib) | 312.24   | 375.69      | 387.45   | 122.39 | 2122.9   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 47.75    | 48.21       | 48.77    | 845.83 | 40779839       | 2885.6   |
| msgspec | 59.41    | 59.66       | 76.61    | 683.55 | 40779839       | 3198.4   |
| ujson   | 198.21   | 199.14      | 199.79   | 204.78 | 40779839       | 3514.0   |
| strata  | 240.17   | 240.67      | 241.33   | 198.01 | 47655981       | 2574.1   |
| json    | 313.24   | 316.88      | 318.02   | 145.11 | 45983395       | 3846.9   |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 49.99    | 50.23       | 51.23    | 811.92 | 40779839       | 3907.8   |
| msgspec | 56.07    | 56.48       | 56.99    | 722.04 | 40779839       | 4159.9   |
| ujson   | 199.43   | 204.95      | 215.38   | 198.98 | 40779839       | 3482.6   |
| strata  | 239.03   | 239.32      | 240.01   | 199.13 | 47655981       | 3934.6   |
| json    | 325.99   | 331.18      | 332.42   | 138.85 | 45983395       | 3540.7   |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 2399216.18 | 1       | 10017.4  |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 1929401.72 | 1       | 9994.4   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1735222.58 | 1       | 9889.2   |
| Extract all user IDs                        | jmespath    | 2.03     | 2.17        | 13.48    | 21154.77   | 4000    | 7562.1   |
| Extract all user IDs                        | strata      | 5.44     | 5.72        | 5.78     | 8032.74    | 4000    | 8462.4   |
| Extract all user IDs                        | jsonpath-ng | 104.18   | 105.43      | 105.64   | 436.15     | 4000    | 7564.0   |
| Extract all user names                      | jmespath    | 1.56     | 1.60        | 1.72     | 28651.57   | 4000    | 11594.9  |
| Extract all user names                      | strata      | 5.17     | 5.30        | 5.44     | 8677.48    | 4000    | 11594.8  |
| Extract all user names                      | jsonpath-ng | 102.79   | 103.33      | 110.14   | 445.02     | 4000    | 11431.8  |
| Extract nested timestamp field              | jmespath    | 2.77     | 2.82        | 2.97     | 16301.59   | 4000    | 8556.5   |
| Extract nested timestamp field              | strata      | 6.47     | 6.58        | 6.74     | 6983.54    | 4000    | 8843.9   |
| Extract nested timestamp field              | jsonpath-ng | 104.58   | 106.90      | 107.12   | 430.16     | 4000    | 8550.8   |
| Extract order item prices (double wildcard) | jmespath    | 309.07   | 310.83      | 311.32   | 147.94     | 4000    | 9111.3   |
| Extract order item prices (double wildcard) | strata      | 1001.34  | 1003.71     | 1018.91  | 45.81      | 789913  | 9093.7   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1215.20  | 1249.35     | 1263.32  | 36.81      | 789913  | 9092.3   |
| Filter users by age (numeric predicate)     | strata      | 4.95     | 5.01        | 5.28     | 9183.13    | 3212    | 12928.6  |
| Filter users by age (numeric predicate)     | jmespath    | 5.57     | 5.67        | 5.85     | 8105.24    | 3212    | 12928.6  |
| Recursively find all prices                 | strata      | 1035.12  | 1042.44     | 1049.47  | 44.11      | 789913  | 14129.0  |
| Recursively find all prices                 | jsonpath-ng | 3846.13  | 3852.97     | 3936.20  | 11.93      | 789913  | 13705.8  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 2608988.63 | 10      | 13999.1  |
| Slice first 10 users, extract IDs           | strata      | 0.03     | 0.03        | 0.03     | 1771451.88 | 10      | 14022.1  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.03     | 1618164.49 | 10      | 13981.5  |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 2640066.58 | 1       | 14537.1  |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2298968.42 | 1       | 14505.2  |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.04     | 1886332.85 | 1       | 12935.1  |
| Extract all user IDs                        | jsonpath-ng | 0.01     | 0.01        | 0.02     | 3226625.23 | 0       | 14939.5  |
| Extract all user IDs                        | jmespath    | 2.08     | 2.09        | 2.13     | 21994.00   | 4000    | 15020.6  |
| Extract all user IDs                        | strata      | 95.66    | 96.60       | 96.65    | 475.96     | 4000    | 15160.0  |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1977607.71 | 0       | 11596.2  |
| Extract all user names                      | jmespath    | 2.00     | 2.27        | 2.31     | 20266.02   | 4000    | 11596.2  |
| Extract all user names                      | strata      | 101.38   | 103.07      | 104.72   | 446.10     | 4000    | 11882.9  |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2198181.45 | 0       | 14674.3  |
| Extract nested timestamp field              | jmespath    | 3.22     | 3.34        | 3.35     | 13784.86   | 4000    | 14846.5  |
| Extract nested timestamp field              | strata      | 95.85    | 96.13       | 96.73    | 478.32     | 4000    | 14882.0  |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.01     | 0.02        | 0.02     | 2829499.28 | 0       | 14527.7  |
| Extract order item prices (double wildcard) | jmespath    | 184.84   | 186.27      | 187.05   | 246.85     | 789913  | 14556.8  |
| Extract order item prices (double wildcard) | strata      | 1058.09  | 1105.04     | 1114.72  | 41.61      | 789913  | 14575.9  |
| Filter users by age (numeric predicate)     | jmespath    | 5.98     | 6.09        | 6.30     | 7552.05    | 3212    | 11599.5  |
| Filter users by age (numeric predicate)     | strata      | 105.45   | 105.51      | 106.22   | 435.77     | 3212    | 11599.5  |
| NDJSON first record id                      | jmespath    | 0.01     | 0.01        | 0.02     | 3503187.79 | 1       | 15285.4  |
| NDJSON first record id                      | strata      | 0.01     | 0.02        | 0.02     | 2966411.56 | 1       | 15370.5  |
| NDJSON first record id                      | jsonpath-ng | 0.01     | 0.02        | 0.02     | 2363005.94 | 1       | 15226.3  |
| Recursively find all prices                 | strata      | 1173.09  | 1201.33     | 1232.16  | 38.27      | 789913  | 11638.5  |
| Recursively find all prices                 | jsonpath-ng | 4013.00  | 4028.83     | 4090.51  | 11.41      | 789913  | 10240.1  |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 1909206.34 | 10      | 10220.0  |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1360658.01 | 10      | 10220.0  |
| Slice first 10 users, extract IDs           | strata      | 0.04     | 0.04        | 0.04     | 1111284.34 | 10      | 10248.0  |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2767.96  | 2800.35     | 3284.71  | 131.36 | 14564.3  |
| strata_cursor_reparse | 10074.27 | 11250.20    | 11898.42 | 32.70  | 14795.6  |
- Speedup: 4.02x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 2772.94  | 2782.71     | 2788.47  | 148.71 | 10117.9  |
| strata_cursor_reparse | 6168.61  | 6228.40     | 6232.15  | 66.44  | 10017.3  |
- Speedup: 2.24x

## Summary
- loads json: orjson (266.76 ms)
- loads ndjson: strata (230.06 ms)
- dumps str: orjson (48.21 ms)
- dumps bytes: orjson (50.23 ms)
- search: jmespath (0.01 ms)
- cursor reuse: strata_cursor_reuse (2782.71 ms)
- Rule 13: strata not #1 in: loads/json, dumps/str, dumps/bytes
- Strata gap in loads/json: 41.4% behind orjson
- Strata gap in dumps/str: 399.2% behind orjson
- Strata gap in dumps/bytes: 376.5% behind orjson
