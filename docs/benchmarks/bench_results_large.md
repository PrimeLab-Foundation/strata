# Strata Benchmark Results — LARGE
Generated: 2026-02-18 00:47:34

## Environment
- Commit: 9217ce489741272a1a08b92b88515eeecb3e49b3
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
| orjson        | 154.86   | 214.08      | 217.47   | 214.80 | 1047.2   |
| msgspec       | 163.60   | 220.59      | 222.80   | 208.46 | 1031.5   |
| ujson         | 218.37   | 299.90      | 300.94   | 153.33 | 1152.0   |
| json (stdlib) | 282.30   | 350.55      | 358.18   | 131.17 | 1031.5   |
| strata        | 328.79   | 358.25      | 373.57   | 128.35 | 910.1    |

### NDJSON — users.ndjson (43.85 MB, 4000 lines)
| Library       | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| ------------- | -------- | ----------- | -------- | ------ | -------- |
| orjson        | 187.50   | 198.55      | 209.67   | 231.57 | 1672.7   |
| strata        | 189.67   | 216.17      | 229.89   | 212.70 | 1669.2   |
| msgspec       | 210.86   | 217.82      | 221.92   | 211.09 | 1656.0   |
| ujson         | 269.37   | 286.49      | 296.63   | 160.49 | 1779.8   |
| json (stdlib) | 313.64   | 315.85      | 333.86   | 145.57 | 1659.0   |

## Serialization Benchmarks (dumps)
### dumps (str) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 51.80    | 53.38       | 55.81    | 763.91 | 40779839       | 772.8    |
| msgspec | 56.20    | 58.73       | 61.11    | 694.34 | 40779839       | 1052.8   |
| ujson   | 183.59   | 186.93      | 199.44   | 218.16 | 40779839       | 1329.4   |
| json    | 292.84   | 296.21      | 304.57   | 155.24 | 45983395       | 1685.6   |
| strata  | 425.21   | 431.65      | 442.81   | 110.40 | 47655981       | 763.2    |

### dumps (bytes) — users.json (43.85 MB)
| Library | Min (ms) | Median (ms) | P95 (ms) | MB/s   | Output (bytes) | RSS (MB) |
| ------- | -------- | ----------- | -------- | ------ | -------------- | -------- |
| orjson  | 45.01    | 45.30       | 45.82    | 900.22 | 40779839       | 754.7    |
| msgspec | 50.60    | 50.94       | 51.60    | 800.54 | 40779839       | 1004.7   |
| ujson   | 184.01   | 185.40      | 190.62   | 219.95 | 40779839       | 1313.1   |
| json    | 294.24   | 295.81      | 296.47   | 155.45 | 45983395       | 1670.0   |
| strata  | 423.59   | 430.19      | 445.99   | 110.78 | 47655981       | 762.8    |

## Search Benchmarks (query)
### JSON — users.json (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.01     | 0.01        | 0.02     | 3594697.57 | 1       | 5283.3   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.02     | 2039896.59 | 1       | 5229.6   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1842358.79 | 1       | 5230.1   |
| Extract all user IDs                        | strata      | 0.57     | 0.60        | 0.61     | 77201.92   | 4000    | 3403.8   |
| Extract all user IDs                        | jmespath    | 1.67     | 1.74        | 1.93     | 26450.66   | 4000    | 3364.5   |
| Extract all user IDs                        | jsonpath-ng | 32.38    | 33.57       | 38.85    | 1369.63    | 4000    | 3346.2   |
| Extract all user names                      | strata      | 0.68     | 0.68        | 0.69     | 67947.39   | 4000    | 5362.4   |
| Extract all user names                      | jmespath    | 1.31     | 1.53        | 1.68     | 30071.70   | 4000    | 5362.7   |
| Extract all user names                      | jsonpath-ng | 32.03    | 33.23       | 33.82    | 1383.94    | 4000    | 5364.5   |
| Extract nested timestamp field              | strata      | 0.93     | 1.15        | 1.31     | 39821.08   | 4000    | 4592.8   |
| Extract nested timestamp field              | jmespath    | 2.51     | 3.05        | 3.18     | 15087.45   | 4000    | 4547.3   |
| Extract nested timestamp field              | jsonpath-ng | 34.67    | 36.90       | 38.30    | 1246.22    | 4000    | 4548.6   |
| Extract order item prices (double wildcard) | strata      | 58.06    | 58.74       | 62.10    | 782.85     | 789913  | 5011.4   |
| Extract order item prices (double wildcard) | jmespath    | 218.76   | 219.60      | 236.05   | 209.40     | 4000    | 4928.7   |
| Extract order item prices (double wildcard) | jsonpath-ng | 1111.85  | 1133.41     | 1151.96  | 40.57      | 789913  | 4346.3   |
| Filter users by age (numeric predicate)     | strata      | 1.14     | 1.17        | 1.28     | 39293.65   | 3212    | 5363.0   |
| Filter users by age (numeric predicate)     | jmespath    | 4.72     | 4.75        | 4.85     | 9676.05    | 3212    | 5363.2   |
| Recursively find all prices                 | strata      | 112.13   | 116.27      | 123.48   | 395.50     | 789913  | 5398.4   |
| Recursively find all prices                 | jsonpath-ng | 3899.82  | 3933.30     | 3970.75  | 11.69      | 789913  | 3889.0   |
| Slice first 10 users, extract IDs           | strata      | 0.01     | 0.01        | 0.02     | 3180475.27 | 10      | 5122.6   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.02        | 0.02     | 1977780.22 | 10      | 5103.3   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.03     | 0.03        | 0.04     | 1347498.84 | 10      | 5103.7   |

### NDJSON — users.ndjson (43.85 MB, 4000 records, cursor mode)
| Query                                       | Library     | Min (ms) | Median (ms) | P95 (ms) | MB/s       | Results | RSS (MB) |
| ------------------------------------------- | ----------- | -------- | ----------- | -------- | ---------- | ------- | -------- |
| Deep path navigation                        | strata      | 0.02     | 0.02        | 0.02     | 2896343.64 | 1       | 1470.5   |
| Deep path navigation                        | jmespath    | 0.02     | 0.02        | 0.03     | 2275304.68 | 1       | 1470.5   |
| Deep path navigation                        | jsonpath-ng | 0.02     | 0.02        | 0.04     | 1929231.07 | 1       | 1470.8   |
| Extract all user IDs                        | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2211396.86 | 0       | 1505.5   |
| Extract all user IDs                        | jmespath    | 2.00     | 2.01        | 2.12     | 22891.92   | 4000    | 1505.5   |
| Extract all user IDs                        | strata      | 32.22    | 34.35       | 35.47    | 1338.46    | 4000    | 1473.3   |
| Extract all user names                      | jsonpath-ng | 0.02     | 0.02        | 0.03     | 2010025.20 | 0       | 1463.8   |
| Extract all user names                      | jmespath    | 2.07     | 2.08        | 2.08     | 22139.63   | 4000    | 1463.7   |
| Extract all user names                      | strata      | 31.13    | 32.24       | 35.28    | 1426.16    | 4000    | 1433.1   |
| Extract nested timestamp field              | jsonpath-ng | 0.02     | 0.02        | 0.03     | 1876713.10 | 0       | 1474.3   |
| Extract nested timestamp field              | jmespath    | 3.35     | 3.37        | 3.67     | 13624.20   | 4000    | 1474.2   |
| Extract nested timestamp field              | strata      | 31.56    | 34.77       | 35.04    | 1322.51    | 4000    | 1473.9   |
| Extract order item prices (double wildcard) | jsonpath-ng | 0.02     | 0.02        | 0.02     | 2159572.53 | 0       | 1571.6   |
| Extract order item prices (double wildcard) | strata      | 65.79    | 67.12       | 78.27    | 685.08     | 789913  | 1519.2   |
| Extract order item prices (double wildcard) | jmespath    | 185.39   | 187.15      | 188.92   | 245.68     | 789913  | 1571.6   |
| Filter users by age (numeric predicate)     | jmespath    | 5.24     | 5.49        | 5.56     | 8374.03    | 3212    | 1473.5   |
| Filter users by age (numeric predicate)     | strata      | 34.04    | 34.11       | 34.41    | 1347.91    | 3212    | 1473.5   |
| NDJSON first record id                      | strata      | 0.02     | 0.02        | 0.02     | 2684929.90 | 1       | 1470.0   |
| NDJSON first record id                      | jmespath    | 0.02     | 0.02        | 0.02     | 2202603.80 | 1       | 1470.0   |
| NDJSON first record id                      | jsonpath-ng | 0.02     | 0.03        | 0.03     | 1833092.31 | 1       | 1470.4   |
| Recursively find all prices                 | strata      | 109.01   | 112.45      | 114.80   | 408.90     | 789913  | 1519.7   |
| Recursively find all prices                 | jsonpath-ng | 3895.45  | 3906.63     | 3920.13  | 11.77      | 789913  | 1614.8   |
| Slice first 10 users, extract IDs           | jmespath    | 0.02     | 0.03        | 0.04     | 1827042.05 | 10      | 1470.6   |
| Slice first 10 users, extract IDs           | strata      | 0.02     | 0.04        | 0.04     | 1280157.33 | 10      | 1470.6   |
| Slice first 10 users, extract IDs           | jsonpath-ng | 0.04     | 0.04        | 0.05     | 1063107.63 | 10      | 1470.9   |

## Cursor Reuse
### JSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 920.04   | 1075.31     | 1156.46  | 342.10 | 5458.0   |
| strata_cursor_reparse | 10277.54 | 11690.86    | 12683.58 | 31.47  | 9530.5   |
- Speedup: 10.87x

### NDJSON
| Mode                  | Min (ms) | Median (ms) | P95 (ms) | MB/s   | RSS (MB) |
| --------------------- | -------- | ----------- | -------- | ------ | -------- |
| strata_cursor_reuse   | 504.86   | 518.66      | 549.02   | 797.85 | 1068.1   |
| strata_cursor_reparse | 3655.79  | 3758.32     | 3790.12  | 110.11 | 2145.1   |
- Speedup: 7.25x

## Summary
- loads json: orjson (214.08 ms)
- loads ndjson: orjson (198.55 ms)
- dumps str: orjson (53.38 ms)
- dumps bytes: orjson (45.30 ms)
- search: strata (0.01 ms)
- cursor reuse: strata_cursor_reuse (518.66 ms)
- Rule 13: strata not #1 in: loads/json, loads/ndjson, dumps/str, dumps/bytes
- Strata gap in loads/json: 67.3% behind orjson
- Strata gap in loads/ndjson: 8.9% behind orjson
- Strata gap in dumps/str: 708.6% behind orjson
- Strata gap in dumps/bytes: 849.6% behind orjson
