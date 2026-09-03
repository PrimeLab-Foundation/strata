# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 07e47e6
- python: 3.12.14
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
| users.json | strata | 11.200 | 12.449 | 16.682 | 58.039 | 1.00x |
| users.json | orjson | 13.930 | 14.698 | 16.716 | 58.039 | 0.85x |
| users.json | msgspec | 13.577 | 14.162 | 16.468 | 58.039 | 0.88x |
| users.json | ujson | 20.245 | 22.909 | 34.806 | 58.039 | 0.54x |
| users.json | pysimdjson | 21.002 | 22.468 | 37.359 | 58.039 | 0.55x |
| users.json | json | 23.619 | 24.409 | 27.910 | 58.039 | 0.51x |
| flat.json | strata | 0.913 | 0.939 | 1.303 | 65.270 | 1.00x |
| flat.json | orjson | 1.003 | 1.039 | 1.330 | 65.270 | 0.90x |
| flat.json | msgspec | 1.049 | 1.088 | 1.335 | 65.270 | 0.86x |
| flat.json | ujson | 1.666 | 1.743 | 1.981 | 65.270 | 0.54x |
| flat.json | pysimdjson | 1.548 | 1.716 | 1.851 | 65.270 | 0.55x |
| flat.json | json | 1.965 | 2.009 | 2.296 | 65.270 | 0.47x |
| nested.json | strata | 0.839 | 0.868 | 1.469 | 65.270 | 1.00x |
| nested.json | orjson | 1.048 | 1.062 | 1.492 | 65.270 | 0.82x |
| nested.json | msgspec | 1.050 | 1.082 | 1.141 | 65.270 | 0.80x |
| nested.json | ujson | 1.613 | 1.681 | 1.902 | 65.270 | 0.52x |
| nested.json | pysimdjson | 1.455 | 1.601 | 2.060 | 65.270 | 0.54x |
| nested.json | json | 2.108 | 2.151 | 2.567 | 65.270 | 0.40x |
| wide_arrays.json | strata | 5.061 | 5.552 | 6.450 | 72.438 | 1.00x |
| wide_arrays.json | orjson | 5.842 | 6.230 | 7.092 | 72.438 | 0.89x |
| wide_arrays.json | msgspec | 6.037 | 6.530 | 7.736 | 72.438 | 0.85x |
| wide_arrays.json | ujson | 7.616 | 8.330 | 9.175 | 72.438 | 0.67x |
| wide_arrays.json | pysimdjson | 6.823 | 7.263 | 7.858 | 72.438 | 0.76x |
| wide_arrays.json | json | 10.693 | 11.059 | 12.038 | 72.438 | 0.50x |
| mixed.json | strata | 0.199 | 0.203 | 0.218 | 72.539 | 1.00x |
| mixed.json | orjson | 0.236 | 0.241 | 0.256 | 72.539 | 0.84x |
| mixed.json | msgspec | 0.247 | 0.256 | 0.315 | 72.539 | 0.80x |
| mixed.json | ujson | 0.319 | 0.323 | 0.342 | 72.539 | 0.63x |
| mixed.json | pysimdjson | 0.304 | 0.316 | 0.329 | 72.539 | 0.64x |
| mixed.json | json | 0.488 | 0.494 | 0.514 | 72.539 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.440 | 2.533 | 3.127 | 56.832 | 1.00x |
| users.json | orjson | 2.935 | 3.051 | 3.597 | 56.832 | 0.83x |
| users.json | msgspec | 3.905 | 4.014 | 4.285 | 56.832 | 0.63x |
| users.json | ujson | 15.067 | 15.329 | 15.812 | 56.832 | 0.17x |
| users.json | json | 22.411 | 22.601 | 23.308 | 56.832 | 0.11x |
| flat.json | strata | 0.315 | 0.350 | 0.374 | 65.270 | 1.00x |
| flat.json | orjson | 0.365 | 0.427 | 0.456 | 65.270 | 0.82x |
| flat.json | msgspec | 0.455 | 0.536 | 0.629 | 65.270 | 0.65x |
| flat.json | ujson | 1.313 | 1.360 | 1.425 | 65.270 | 0.26x |
| flat.json | json | 1.965 | 2.040 | 2.108 | 65.270 | 0.17x |
| nested.json | strata | 0.262 | 0.274 | 0.331 | 65.270 | 1.00x |
| nested.json | orjson | 0.298 | 0.313 | 0.411 | 65.270 | 0.87x |
| nested.json | msgspec | 0.415 | 0.440 | 0.464 | 65.270 | 0.62x |
| nested.json | ujson | 1.399 | 1.426 | 1.554 | 65.270 | 0.19x |
| nested.json | json | 2.397 | 2.432 | 2.518 | 65.270 | 0.11x |
| wide_arrays.json | strata | 1.513 | 1.581 | 2.188 | 72.438 | 1.00x |
| wide_arrays.json | orjson | 1.856 | 1.919 | 2.254 | 72.438 | 0.82x |
| wide_arrays.json | msgspec | 2.745 | 2.786 | 3.148 | 72.438 | 0.57x |
| wide_arrays.json | ujson | 8.710 | 8.989 | 9.346 | 72.438 | 0.18x |
| wide_arrays.json | json | 16.895 | 17.265 | 17.767 | 72.438 | 0.09x |
| mixed.json | strata | 0.062 | 0.068 | 0.109 | 72.539 | 1.00x |
| mixed.json | orjson | 0.065 | 0.070 | 0.090 | 72.539 | 0.96x |
| mixed.json | msgspec | 0.089 | 0.095 | 0.112 | 72.539 | 0.71x |
| mixed.json | ujson | 0.309 | 0.326 | 0.453 | 72.539 | 0.21x |
| mixed.json | json | 0.521 | 0.545 | 0.649 | 72.539 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.219 | 12.933 | 13.663 | 71.582 | 1.00x |
| users.json | orjson | 14.311 | 15.048 | 16.110 | 71.582 | 0.86x |
| users.json | msgspec | 14.480 | 14.923 | 27.033 | 71.582 | 0.87x |
| users.json | ujson | 22.484 | 22.862 | 31.419 | 71.582 | 0.57x |
| users.json | json | 23.774 | 25.429 | 36.659 | 71.582 | 0.51x |
| flat.json | strata | 0.961 | 1.089 | 1.216 | 65.270 | 1.00x |
| flat.json | orjson | 1.117 | 1.227 | 1.309 | 65.270 | 0.89x |
| flat.json | msgspec | 1.164 | 1.225 | 1.308 | 65.270 | 0.89x |
| flat.json | ujson | 1.762 | 1.955 | 2.091 | 65.270 | 0.56x |
| flat.json | json | 2.023 | 2.113 | 2.268 | 65.270 | 0.52x |
| nested.json | strata | 0.875 | 0.956 | 1.058 | 65.270 | 1.00x |
| nested.json | orjson | 1.138 | 1.185 | 1.286 | 65.270 | 0.81x |
| nested.json | msgspec | 1.106 | 1.164 | 1.242 | 65.270 | 0.82x |
| nested.json | ujson | 1.609 | 1.667 | 1.838 | 65.270 | 0.57x |
| nested.json | json | 2.113 | 2.210 | 2.413 | 65.270 | 0.43x |
| wide_arrays.json | strata | 5.235 | 5.395 | 5.516 | 72.500 | 1.00x |
| wide_arrays.json | orjson | 5.545 | 5.856 | 5.981 | 72.500 | 0.92x |
| wide_arrays.json | msgspec | 6.346 | 6.466 | 6.905 | 72.500 | 0.83x |
| wide_arrays.json | ujson | 7.898 | 8.184 | 8.612 | 72.500 | 0.66x |
| wide_arrays.json | json | 10.484 | 10.710 | 10.842 | 72.500 | 0.50x |
| mixed.json | strata | 0.221 | 0.231 | 0.235 | 72.539 | 1.00x |
| mixed.json | orjson | 0.282 | 0.302 | 0.331 | 72.539 | 0.76x |
| mixed.json | msgspec | 0.295 | 0.310 | 0.357 | 72.539 | 0.74x |
| mixed.json | ujson | 0.380 | 0.398 | 0.577 | 72.539 | 0.58x |
| mixed.json | json | 0.535 | 0.557 | 0.651 | 72.539 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 14.099 | 14.572 | 15.066 | 65.262 | 1.00x |
| users.ndjson | orjson | 20.209 | 21.090 | 21.488 | 65.262 | 0.69x |
| users.ndjson | msgspec | 19.879 | 21.088 | 22.952 | 65.262 | 0.69x |
| users.ndjson | ujson | 26.594 | 27.318 | 28.900 | 65.262 | 0.53x |
| users.ndjson | json | 33.227 | 34.308 | 37.790 | 65.262 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.149 | 3.298 | 3.890 | 69.613 | 1.00x |
| users.json | orjson | 3.738 | 3.881 | 4.517 | 69.613 | 0.85x |
| users.json | msgspec | 4.628 | 4.917 | 5.463 | 69.613 | 0.67x |
| users.json | ujson | 16.038 | 16.368 | 16.815 | 69.613 | 0.20x |
| users.json | json | 23.686 | 23.917 | 24.535 | 69.613 | 0.14x |
| flat.json | strata | 0.493 | 0.584 | 0.684 | 65.270 | 1.00x |
| flat.json | orjson | 0.590 | 0.662 | 0.755 | 65.270 | 0.88x |
| flat.json | msgspec | 0.670 | 0.780 | 1.016 | 65.270 | 0.75x |
| flat.json | ujson | 1.566 | 1.620 | 1.871 | 65.270 | 0.36x |
| flat.json | json | 2.212 | 2.281 | 2.478 | 65.270 | 0.26x |
| nested.json | strata | 0.424 | 0.463 | 0.624 | 65.270 | 1.00x |
| nested.json | orjson | 0.473 | 0.500 | 0.627 | 65.270 | 0.93x |
| nested.json | msgspec | 0.592 | 0.609 | 0.657 | 65.270 | 0.76x |
| nested.json | ujson | 1.608 | 1.675 | 1.828 | 65.270 | 0.28x |
| nested.json | json | 2.605 | 2.677 | 2.814 | 65.270 | 0.17x |
| wide_arrays.json | strata | 2.012 | 2.144 | 2.602 | 72.539 | 1.00x |
| wide_arrays.json | orjson | 2.386 | 2.462 | 3.240 | 72.539 | 0.87x |
| wide_arrays.json | msgspec | 3.259 | 3.371 | 4.229 | 72.539 | 0.64x |
| wide_arrays.json | ujson | 9.456 | 9.804 | 10.703 | 72.539 | 0.22x |
| wide_arrays.json | json | 17.625 | 17.879 | 18.432 | 72.539 | 0.12x |
| mixed.json | strata | 0.169 | 0.182 | 0.229 | 72.539 | 1.00x |
| mixed.json | orjson | 0.183 | 0.203 | 0.267 | 72.539 | 0.90x |
| mixed.json | msgspec | 0.201 | 0.233 | 0.278 | 72.539 | 0.78x |
| mixed.json | ujson | 0.441 | 0.477 | 0.541 | 72.539 | 0.38x |
| mixed.json | json | 0.669 | 0.697 | 0.740 | 72.539 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.071 | 0.080 | 0.116 | 69.613 | 1.00x |
| users.json $[*].id | jmespath | 0.518 | 0.544 | 0.617 | 69.613 | 0.15x |
| users.json $[*].id | jsonpath-ng | 3.003 | 3.186 | 3.461 | 69.613 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.469 | 0.517 | 1.187 | 69.727 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.192 | 3.374 | 4.399 | 69.727 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 21.550 | 23.022 | 29.211 | 69.727 | 0.02x |
| users.json $..total | strata | 1.769 | 1.985 | 2.130 | 70.828 | 1.00x |
| users.json $..total | jsonpath-ng | 396.738 | 398.763 | 402.242 | 70.828 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.172 | 3.220 | 3.396 | 69.727 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.422 | 16.762 | 21.111 | 69.727 | 0.19x |
| users.json $[*].id | orjson+jsonpath-ng | 18.983 | 19.838 | 21.845 | 69.727 | 0.16x |
| users.json $[*].orders[*].total | strata | 3.441 | 3.472 | 3.652 | 70.047 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.638 | 21.391 | 26.207 | 70.047 | 0.16x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 41.875 | 45.522 | 48.954 | 70.047 | 0.08x |
| users.json $..total | strata | 16.301 | 17.374 | 18.618 | 70.062 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 419.736 | 422.059 | 425.017 | 70.062 | 0.04x |

