# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c36c406
- python: 3.12.13
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: x86_64
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.205 | 10.455 | 25.613 | 60.012 | 1.00x |
| users.json | orjson | 13.110 | 13.480 | 21.144 | 60.012 | 0.78x |
| users.json | msgspec | 13.063 | 13.354 | 24.963 | 60.012 | 0.78x |
| users.json | ujson | 17.898 | 19.163 | 24.333 | 60.012 | 0.55x |
| users.json | pysimdjson | 18.048 | 19.743 | 22.987 | 60.012 | 0.53x |
| users.json | json | 22.069 | 22.459 | 26.844 | 60.012 | 0.47x |
| flat.json | strata | 0.915 | 0.932 | 0.969 | 66.781 | 1.00x |
| flat.json | orjson | 1.023 | 1.033 | 1.053 | 66.781 | 0.90x |
| flat.json | msgspec | 1.031 | 1.044 | 1.104 | 66.781 | 0.89x |
| flat.json | ujson | 1.471 | 1.566 | 1.675 | 66.781 | 0.60x |
| flat.json | pysimdjson | 1.554 | 1.577 | 1.694 | 66.781 | 0.59x |
| flat.json | json | 1.897 | 1.918 | 2.019 | 66.781 | 0.49x |
| nested.json | strata | 0.832 | 0.860 | 0.898 | 66.781 | 1.00x |
| nested.json | orjson | 1.005 | 1.034 | 1.064 | 66.781 | 0.83x |
| nested.json | msgspec | 1.039 | 1.092 | 1.198 | 66.781 | 0.79x |
| nested.json | ujson | 1.493 | 1.585 | 1.633 | 66.781 | 0.54x |
| nested.json | pysimdjson | 1.446 | 1.486 | 1.555 | 66.781 | 0.58x |
| nested.json | json | 2.112 | 2.131 | 2.155 | 66.781 | 0.40x |
| wide_arrays.json | strata | 4.612 | 4.681 | 5.213 | 70.785 | 1.00x |
| wide_arrays.json | orjson | 5.039 | 5.251 | 5.533 | 70.785 | 0.89x |
| wide_arrays.json | msgspec | 5.443 | 5.652 | 5.779 | 70.785 | 0.83x |
| wide_arrays.json | ujson | 7.043 | 7.206 | 7.412 | 70.785 | 0.65x |
| wide_arrays.json | pysimdjson | 6.376 | 6.504 | 6.595 | 70.785 | 0.72x |
| wide_arrays.json | json | 9.697 | 9.890 | 10.002 | 70.785 | 0.47x |
| mixed.json | strata | 0.203 | 0.218 | 0.230 | 70.848 | 1.00x |
| mixed.json | orjson | 0.233 | 0.240 | 0.262 | 70.848 | 0.91x |
| mixed.json | msgspec | 0.251 | 0.257 | 0.321 | 70.848 | 0.85x |
| mixed.json | ujson | 0.309 | 0.336 | 0.359 | 70.848 | 0.65x |
| mixed.json | pysimdjson | 0.302 | 0.307 | 0.346 | 70.848 | 0.71x |
| mixed.json | json | 0.479 | 0.491 | 0.508 | 70.848 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.660 | 2.694 | 2.744 | 59.109 | 1.00x |
| users.json | orjson | 2.900 | 2.908 | 2.939 | 59.109 | 0.93x |
| users.json | msgspec | 3.855 | 3.875 | 4.073 | 59.109 | 0.70x |
| users.json | ujson | 14.023 | 14.259 | 14.487 | 59.109 | 0.19x |
| users.json | json | 21.857 | 22.002 | 22.275 | 59.109 | 0.12x |
| flat.json | strata | 0.269 | 0.272 | 0.288 | 66.781 | 1.00x |
| flat.json | orjson | 0.329 | 0.342 | 0.387 | 66.781 | 0.80x |
| flat.json | msgspec | 0.430 | 0.439 | 0.447 | 66.781 | 0.62x |
| flat.json | ujson | 1.252 | 1.333 | 1.405 | 66.781 | 0.20x |
| flat.json | json | 1.855 | 1.867 | 1.924 | 66.781 | 0.15x |
| nested.json | strata | 0.268 | 0.277 | 0.303 | 66.781 | 1.00x |
| nested.json | orjson | 0.290 | 0.291 | 0.300 | 66.781 | 0.95x |
| nested.json | msgspec | 0.416 | 0.422 | 0.440 | 66.781 | 0.66x |
| nested.json | ujson | 1.319 | 1.353 | 1.430 | 66.781 | 0.20x |
| nested.json | json | 2.427 | 2.451 | 2.492 | 66.781 | 0.11x |
| wide_arrays.json | strata | 1.606 | 1.630 | 1.649 | 70.785 | 1.00x |
| wide_arrays.json | orjson | 1.885 | 1.908 | 1.936 | 70.785 | 0.85x |
| wide_arrays.json | msgspec | 2.666 | 2.690 | 2.745 | 70.785 | 0.61x |
| wide_arrays.json | ujson | 8.500 | 8.544 | 8.643 | 70.785 | 0.19x |
| wide_arrays.json | json | 16.341 | 16.387 | 16.474 | 70.785 | 0.10x |
| mixed.json | strata | 0.069 | 0.071 | 0.084 | 70.848 | 1.00x |
| mixed.json | orjson | 0.066 | 0.067 | 0.081 | 70.848 | 1.06x |
| mixed.json | msgspec | 0.086 | 0.088 | 0.100 | 70.848 | 0.81x |
| mixed.json | ujson | 0.296 | 0.332 | 0.354 | 70.848 | 0.21x |
| mixed.json | json | 0.517 | 0.540 | 0.657 | 70.848 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.920 | 11.165 | 12.572 | 71.406 | 1.00x |
| users.json | orjson | 13.550 | 13.957 | 15.031 | 71.406 | 0.80x |
| users.json | msgspec | 13.528 | 13.963 | 15.166 | 71.406 | 0.80x |
| users.json | ujson | 19.675 | 20.589 | 21.551 | 71.406 | 0.54x |
| users.json | json | 22.711 | 22.983 | 25.095 | 71.406 | 0.49x |
| flat.json | strata | 0.963 | 0.975 | 1.069 | 66.781 | 1.00x |
| flat.json | orjson | 1.082 | 1.094 | 1.126 | 66.781 | 0.89x |
| flat.json | msgspec | 1.073 | 1.095 | 1.148 | 66.781 | 0.89x |
| flat.json | ujson | 1.585 | 1.638 | 1.692 | 66.781 | 0.60x |
| flat.json | json | 1.949 | 1.963 | 2.017 | 66.781 | 0.50x |
| nested.json | strata | 0.866 | 0.881 | 0.896 | 66.781 | 1.00x |
| nested.json | orjson | 1.059 | 1.074 | 1.115 | 66.781 | 0.82x |
| nested.json | msgspec | 1.089 | 1.104 | 1.153 | 66.781 | 0.80x |
| nested.json | ujson | 1.587 | 1.610 | 1.738 | 66.781 | 0.55x |
| nested.json | json | 2.168 | 2.188 | 2.286 | 66.781 | 0.40x |
| wide_arrays.json | strata | 4.716 | 4.762 | 4.805 | 70.848 | 1.00x |
| wide_arrays.json | orjson | 5.038 | 5.194 | 5.302 | 70.848 | 0.92x |
| wide_arrays.json | msgspec | 5.645 | 5.766 | 6.147 | 70.848 | 0.83x |
| wide_arrays.json | ujson | 7.337 | 7.373 | 7.471 | 70.848 | 0.65x |
| wide_arrays.json | json | 9.923 | 9.975 | 10.810 | 70.848 | 0.48x |
| mixed.json | strata | 0.224 | 0.242 | 0.257 | 70.848 | 1.00x |
| mixed.json | orjson | 0.279 | 0.294 | 0.317 | 70.848 | 0.82x |
| mixed.json | msgspec | 0.294 | 0.311 | 0.324 | 70.848 | 0.78x |
| mixed.json | ujson | 0.387 | 0.392 | 0.448 | 70.848 | 0.62x |
| mixed.json | json | 0.528 | 0.539 | 0.573 | 70.848 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.784 | 10.983 | 12.007 | 66.781 | 1.00x |
| users.ndjson | orjson | 16.470 | 16.801 | 17.802 | 66.781 | 0.65x |
| users.ndjson | msgspec | 16.436 | 16.628 | 17.415 | 66.781 | 0.66x |
| users.ndjson | ujson | 21.773 | 22.220 | 23.184 | 66.781 | 0.49x |
| users.ndjson | json | 28.730 | 29.157 | 29.802 | 66.781 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.210 | 3.234 | 3.517 | 68.453 | 1.00x |
| users.json | orjson | 3.506 | 3.545 | 3.705 | 68.453 | 0.91x |
| users.json | msgspec | 4.453 | 4.486 | 4.749 | 68.453 | 0.72x |
| users.json | ujson | 14.792 | 14.995 | 15.169 | 68.453 | 0.22x |
| users.json | json | 22.660 | 22.760 | 22.858 | 68.453 | 0.14x |
| flat.json | strata | 0.413 | 0.423 | 0.457 | 66.781 | 1.00x |
| flat.json | orjson | 0.489 | 0.516 | 0.660 | 66.781 | 0.82x |
| flat.json | msgspec | 0.586 | 0.603 | 0.628 | 66.781 | 0.70x |
| flat.json | ujson | 1.439 | 1.488 | 1.557 | 66.781 | 0.28x |
| flat.json | json | 2.048 | 2.073 | 2.095 | 66.781 | 0.20x |
| nested.json | strata | 0.382 | 0.394 | 0.412 | 66.781 | 1.00x |
| nested.json | orjson | 0.428 | 0.449 | 0.457 | 66.781 | 0.88x |
| nested.json | msgspec | 0.540 | 0.560 | 0.582 | 66.781 | 0.70x |
| nested.json | ujson | 1.463 | 1.489 | 1.584 | 66.781 | 0.26x |
| nested.json | json | 2.587 | 2.634 | 2.664 | 66.781 | 0.15x |
| wide_arrays.json | strata | 2.003 | 2.023 | 2.061 | 70.848 | 1.00x |
| wide_arrays.json | orjson | 2.258 | 2.280 | 2.318 | 70.848 | 0.89x |
| wide_arrays.json | msgspec | 3.131 | 3.153 | 3.187 | 70.848 | 0.64x |
| wide_arrays.json | ujson | 9.071 | 9.127 | 9.289 | 70.848 | 0.22x |
| wide_arrays.json | json | 16.832 | 16.858 | 17.050 | 70.848 | 0.12x |
| mixed.json | strata | 0.156 | 0.158 | 0.173 | 70.848 | 1.00x |
| mixed.json | orjson | 0.170 | 0.173 | 0.199 | 70.848 | 0.91x |
| mixed.json | msgspec | 0.189 | 0.192 | 0.211 | 70.848 | 0.82x |
| mixed.json | ujson | 0.424 | 0.458 | 0.480 | 70.848 | 0.34x |
| mixed.json | json | 0.635 | 0.658 | 0.674 | 70.848 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.063 | 0.064 | 0.075 | 68.453 | 1.00x |
| users.json $[*].id | jmespath | 0.475 | 0.488 | 0.496 | 68.453 | 0.13x |
| users.json $[*].id | jsonpath-ng | 2.736 | 2.816 | 3.084 | 68.453 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.443 | 0.452 | 0.477 | 68.480 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.000 | 3.037 | 3.082 | 68.480 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.120 | 19.278 | 19.657 | 68.480 | 0.02x |
| users.json $..total | strata | 2.015 | 2.160 | 2.270 | 72.395 | 1.00x |
| users.json $..total | jsonpath-ng | 393.693 | 396.188 | 401.574 | 72.395 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.985 | 3.008 | 3.385 | 68.480 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.196 | 14.279 | 15.955 | 68.480 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 16.405 | 16.861 | 17.543 | 68.480 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.206 | 3.308 | 3.438 | 72.395 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.958 | 17.087 | 18.170 | 72.395 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.282 | 37.152 | 37.481 | 72.395 | 0.09x |
| users.json $..total | strata | 13.573 | 14.179 | 15.467 | 72.352 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 412.837 | 415.275 | 422.861 | 72.352 | 0.03x |

