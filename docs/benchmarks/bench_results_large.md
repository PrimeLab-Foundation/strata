# Strata Benchmark Results — LARGE
Generated: 2026-02-22 01:30:02

## Environment
- Commit: 65f65be99e32d586b0d059d02131d20552789de6
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
| orjson        | 157.97   | 214.01      | 216.65   | 214.87 | 809.2    |
| msgspec       | 167.30   | 226.14      | 229.59   | 203.34 | 791.6    |
| ujson         | 221.02   | 296.05      | 298.83   | 155.32 | 912.1    |
| strata        | 293.37   | 319.99      | 321.69   | 143.70 | 666.2    |
| json (stdlib) | 287.59   | 339.76      | 343.63   | 135.34 | 835.5    |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 191.53   | 196.30      | 198.36   | 234.23 | 1637.8   |
| msgspec       | 199.33   | 202.54      | 204.54   | 227.01 | 1619.1   |
| strata        | 187.26   | 212.68      | 215.86   | 216.19 | 1627.0   |
| ujson         | 271.48   | 274.98      | 278.02   | 167.21 | 1742.7   |
| json (stdlib) | 313.85   | 315.49      | 316.64   | 145.74 | 1622.0   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 49.00    | 49.52       | 51.17    | 823.56 | 40779839       | 772.7    |
| msgspec | 53.85    | 54.92       | 55.56    | 742.58 | 40779839       | 1051.9   |
| ujson   | 181.03   | 183.42      | 185.86   | 222.33 | 40779839       | 1329.3   |
| json    | 293.11   | 294.87      | 298.36   | 155.94 | 45983395       | 1685.6   |
| strata  | 429.57   | 430.64      | 487.11   | 110.66 | 47655981       | 762.5    |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 44.11    | 44.29       | 44.37    | 920.78 | 40779839       | 754.7    |
| msgspec | 51.54    | 51.69       | 52.06    | 788.90 | 40779839       | 1004.6   |
| ujson   | 185.29   | 185.78      | 187.21   | 219.51 | 40779839       | 1313.9   |
| json    | 294.71   | 295.94      | 298.98   | 155.38 | 45983395       | 1670.1   |
| strata  | 429.32   | 430.07      | 433.80   | 110.81 | 47655981       | 762.7    |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 2420175.08 | 1       | 5361.8   |
| Deep path navigation                        | jmespath    | 0.01     | 0.02        | 0.02     | 2181066.17 | 1       | 5361.8   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2002846.39 | 1       | 5362.2   |
| Extract all user IDs                        | strata      | 0.43     | 0.44        | 0.47     | 103692.78  | 4000    | 5362.2   |
| Extract all user IDs                        | jmespath    | 1.78     | 1.82        | 1.84     | 25261.56   | 4000    | 5362.5   |
| Extract all user IDs                        | jsonpath-ng | 33.76    | 33.88       | 34.02    | 1357.41    | 4000    | 5364.8   |
| Extract all user names                      | strata      | 0.51     | 0.51        | 0.85     | 90793.92   | 4000    | 5362.6   |
| Extract all user names                      | jmespath    | 1.33     | 1.34        | 1.50     | 34309.57   | 4000    | 5362.9   |
| Extract all user names                      | jsonpath-ng | 33.19    | 33.27       | 33.34    | 1382.02    | 4000    | 5364.8   |
| Extract nested timestamp field              | strata      | 0.98     | 1.14        | 1.86     | 40398.33   | 4000    | 5362.4   |
| Extract nested timestamp field              | jmespath    | 2.75     | 2.86        | 2.92     | 16096.17   | 4000    | 5362.8   |
| Extract nested timestamp field              | jsonpath-ng | 35.90    | 36.15       | 36.17    | 1272.08    | 4000    | 5365.8   |
| Extract order item prices (double wildcard) | strata      | 49.16    | 49.54       | 59.52    | 928.19     | 789913  | 5415.6   |
| Extract order item prices (double wildcard) | jmespath    | 221.92   | 223.12      | 223.86   | 206.10     | 4000    | 5432.0   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1112.65  | 1113.13     | 1120.16  | 41.31      | 789913  | 5444.0   |
| Filter users by age (numeric predicate)     | strata      | 0.72     | 0.72        | 0.86     | 63553.18   | 3212    | 5362.2   |
| Filter users by age (numeric predicate)     | jmespath    | 5.17     | 5.18        | 5.21     | 8880.53    | 3212    | 5362.4   |
| Recursively find all prices                 | strata      | 105.78   | 107.82      | 109.60   | 426.47     | 789913  | 5395.9   |
| Recursively find all prices                 | jsonpath-ng | 3821.14  | 3827.57     | 3858.50  | 12.01      | 789913  | 5407.6   |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.03     | 3344249.76 | 10      | 5361.8   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.03     | 1902734.46 | 10      | 5361.8   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.02     | 0.03        | 0.04     | 1642261.84 | 10      | 5362.2   |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.02        | 0.02     | 2896343.64 | 1       | 1429.0   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2229302.22 | 1       | 1429.0   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1915807.02 | 1       | 1429.4   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2189496.01 | 0       | 1505.2   |
| Extract all user IDs                        | jmespath    | 2.10     | 2.11        | 2.19     | 21751.65   | 4000    | 1505.2   |
| Extract all user IDs                        | strata      | 31.57    | 31.66       | 32.13    | 1452.40    | 4000    | 1473.5   |
| Extract all user names                      | jsonpath-ng | 0.01     | 0.02        | 0.03     | 2303691.83 | 0       | 1463.7   |
| Extract all user names                      | jmespath    | 2.02     | 2.05        | 2.06     | 22416.22   | 4000    | 1463.7   |
| Extract all user names                      | strata      | 31.84    | 32.16       | 33.38    | 1429.73    | 4000    | 1432.6   |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2110020.15 | 0       | 1433.1   |
| Extract nested timestamp field              | jmespath    | 3.28     | 3.33        | 3.34     | 13803.65   | 4000    | 1433.0   |
| Extract nested timestamp field              | strata      | 31.97    | 33.20       | 33.91    | 1384.72    | 4000    | 1432.7   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2113989.88 | 0       | 1533.1   |
| Extract order item prices (double wildcard) | strata      | 65.75    | 65.93       | 69.58    | 697.39     | 789913  | 1478.5   |
| Extract order item prices (double wildcard) | jmespath    | 181.51   | 185.82      | 188.41   | 247.44     | 789913  | 1533.0   |
| Filter users by age (numeric predicate)     | jmespath    | 5.59     | 5.62        | 5.79     | 8179.08    | 3212    | 1473.7   |
| Filter users by age (numeric predicate)     | strata      | 30.65    | 30.98       | 32.11    | 1484.06    | 3212    | 1473.4   |
| NDJSON first record id                      | strata      | 0.01     | 0.02        | 0.02     | 2873710.53 | 1       | 1428.6   |
| NDJSON first record id                      | jmespath    | 0.01     | 0.02        | 0.02     | 2435994.09 | 1       | 1428.6   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2220260.39 | 1       | 1429.0   |
| Recursively find all prices                 | strata      | 102.27   | 105.43      | 106.87   | 436.12     | 789913  | 1478.3   |
| Recursively find all prices                 | jsonpath-ng | 3781.29  | 3794.35     | 3800.80  | 12.12      | 789913  | 1573.7   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.02        | 0.02     | 2193776.82 | 10      | 1470.3   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.03     | 1836091.94 | 10      | 1470.3   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1489209.30 | 10      | 1470.6   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 739.51   | 855.87      | 1166.80  | 429.81 | 9357.9   |
| strata_cursor_reparse | 7047.75  | 8453.78     | 9314.90  | 43.52  | 18238.3  |
- Speedup: 9.88x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 497.94   | 518.91      | 537.43   | 797.46 | 1053.3   |
| strata_cursor_reparse | 3613.46  | 3616.49     | 3644.56  | 114.42 | 2148.7   |
- Speedup: 6.97x

## Summary
- loads json: orjson (214.01 ms)
- loads ndjson: orjson (196.30 ms)
- dumps str: orjson (49.52 ms)
- dumps bytes: orjson (44.29 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (518.91 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 49.5% behind orjson
- Strata gap in loads/ndjson: 8.3% behind orjson
- Strata gap in dumps/str: 769.7% behind orjson
- Strata gap in dumps/bytes: 871.1% behind orjson
