# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 32c5fa4
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-aarch64-with-glibc2.39
- machine: aarch64
- processor: aarch64
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.898 | 8.950 | 10.809 | 51.836 | 1.00x |
| users.json | orjson | 11.608 | 11.636 | 13.280 | 51.836 | 0.77x |
| users.json | msgspec | 12.083 | 12.194 | 13.625 | 51.836 | 0.73x |
| users.json | ujson | 16.296 | 16.526 | 18.925 | 51.836 | 0.54x |
| users.json | pysimdjson | 16.227 | 16.501 | 18.389 | 51.836 | 0.54x |
| users.json | json | 20.395 | 20.598 | 21.138 | 51.836 | 0.43x |
| flat.json | strata | 0.810 | 0.815 | 0.831 | 58.578 | 1.00x |
| flat.json | orjson | 0.852 | 0.863 | 0.866 | 58.578 | 0.95x |
| flat.json | msgspec | 0.907 | 0.913 | 0.930 | 58.578 | 0.89x |
| flat.json | ujson | 1.416 | 1.434 | 1.463 | 58.578 | 0.57x |
| flat.json | pysimdjson | 1.464 | 1.474 | 1.514 | 58.578 | 0.55x |
| flat.json | json | 1.766 | 1.773 | 1.787 | 58.578 | 0.46x |
| nested.json | strata | 0.793 | 0.811 | 0.814 | 58.578 | 1.00x |
| nested.json | orjson | 0.865 | 0.884 | 0.893 | 58.578 | 0.92x |
| nested.json | msgspec | 0.983 | 0.989 | 1.005 | 58.578 | 0.82x |
| nested.json | ujson | 1.390 | 1.406 | 1.428 | 58.578 | 0.58x |
| nested.json | pysimdjson | 1.380 | 1.390 | 1.413 | 58.578 | 0.58x |
| nested.json | json | 1.951 | 1.964 | 1.991 | 58.578 | 0.41x |
| wide_arrays.json | strata | 3.789 | 3.845 | 4.003 | 61.992 | 1.00x |
| wide_arrays.json | orjson | 4.019 | 4.079 | 4.151 | 61.992 | 0.94x |
| wide_arrays.json | msgspec | 4.975 | 5.036 | 5.070 | 61.992 | 0.76x |
| wide_arrays.json | ujson | 6.453 | 6.514 | 6.594 | 61.992 | 0.59x |
| wide_arrays.json | pysimdjson | 5.222 | 5.257 | 5.381 | 61.992 | 0.73x |
| wide_arrays.json | json | 9.448 | 9.559 | 9.710 | 61.992 | 0.40x |
| mixed.json | strata | 0.188 | 0.192 | 0.215 | 61.992 | 1.00x |
| mixed.json | orjson | 0.208 | 0.216 | 0.235 | 61.992 | 0.89x |
| mixed.json | msgspec | 0.230 | 0.233 | 0.252 | 61.992 | 0.82x |
| mixed.json | ujson | 0.293 | 0.304 | 0.324 | 61.992 | 0.63x |
| mixed.json | pysimdjson | 0.286 | 0.289 | 0.310 | 61.992 | 0.66x |
| mixed.json | json | 0.448 | 0.457 | 0.487 | 61.992 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.043 | 2.049 | 2.065 | 50.938 | 1.00x |
| users.json | orjson | 2.570 | 2.583 | 2.594 | 50.938 | 0.79x |
| users.json | msgspec | 3.304 | 3.315 | 3.332 | 50.938 | 0.62x |
| users.json | ujson | 10.472 | 10.520 | 10.590 | 50.938 | 0.19x |
| users.json | json | 18.941 | 18.995 | 19.079 | 50.938 | 0.11x |
| flat.json | strata | 0.230 | 0.231 | 0.247 | 58.578 | 1.00x |
| flat.json | orjson | 0.297 | 0.300 | 0.314 | 58.578 | 0.77x |
| flat.json | msgspec | 0.383 | 0.388 | 0.402 | 58.578 | 0.60x |
| flat.json | ujson | 0.980 | 0.991 | 0.997 | 58.578 | 0.23x |
| flat.json | json | 1.699 | 1.714 | 1.727 | 58.578 | 0.13x |
| nested.json | strata | 0.229 | 0.236 | 0.256 | 58.578 | 1.00x |
| nested.json | orjson | 0.282 | 0.286 | 0.314 | 58.578 | 0.83x |
| nested.json | msgspec | 0.370 | 0.378 | 0.394 | 58.578 | 0.62x |
| nested.json | ujson | 1.096 | 1.114 | 1.140 | 58.578 | 0.21x |
| nested.json | json | 2.151 | 2.203 | 2.218 | 58.578 | 0.11x |
| wide_arrays.json | strata | 1.266 | 1.284 | 1.309 | 61.992 | 1.00x |
| wide_arrays.json | orjson | 1.527 | 1.544 | 1.570 | 61.992 | 0.83x |
| wide_arrays.json | msgspec | 2.286 | 2.302 | 2.338 | 61.992 | 0.56x |
| wide_arrays.json | ujson | 4.680 | 4.708 | 4.740 | 61.992 | 0.27x |
| wide_arrays.json | json | 13.502 | 13.588 | 13.684 | 61.992 | 0.09x |
| mixed.json | strata | 0.065 | 0.067 | 0.070 | 61.992 | 1.00x |
| mixed.json | orjson | 0.063 | 0.065 | 0.080 | 61.992 | 1.03x |
| mixed.json | msgspec | 0.077 | 0.079 | 0.080 | 61.992 | 0.84x |
| mixed.json | ujson | 0.240 | 0.245 | 0.248 | 61.992 | 0.27x |
| mixed.json | json | 0.477 | 0.485 | 0.503 | 61.992 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.072 | 9.124 | 9.820 | 63.113 | 1.00x |
| users.json | orjson | 11.690 | 11.848 | 12.218 | 63.113 | 0.77x |
| users.json | msgspec | 12.306 | 12.392 | 12.651 | 63.113 | 0.74x |
| users.json | ujson | 16.752 | 16.884 | 17.984 | 63.113 | 0.54x |
| users.json | json | 20.637 | 20.765 | 20.846 | 63.113 | 0.44x |
| flat.json | strata | 0.827 | 0.869 | 0.891 | 58.578 | 1.00x |
| flat.json | orjson | 0.914 | 0.938 | 0.967 | 58.578 | 0.93x |
| flat.json | msgspec | 0.978 | 0.991 | 1.005 | 58.578 | 0.88x |
| flat.json | ujson | 1.529 | 1.544 | 1.565 | 58.578 | 0.56x |
| flat.json | json | 1.814 | 1.838 | 1.861 | 58.578 | 0.47x |
| nested.json | strata | 0.846 | 0.875 | 0.884 | 58.578 | 1.00x |
| nested.json | orjson | 0.950 | 0.975 | 1.008 | 58.578 | 0.90x |
| nested.json | msgspec | 1.062 | 1.097 | 1.119 | 58.578 | 0.80x |
| nested.json | ujson | 1.479 | 1.525 | 1.549 | 58.578 | 0.57x |
| nested.json | json | 2.016 | 2.061 | 2.094 | 58.578 | 0.42x |
| wide_arrays.json | strata | 3.873 | 4.001 | 4.045 | 61.992 | 1.00x |
| wide_arrays.json | orjson | 4.122 | 4.277 | 4.475 | 61.992 | 0.94x |
| wide_arrays.json | msgspec | 5.147 | 5.241 | 5.538 | 61.992 | 0.76x |
| wide_arrays.json | ujson | 6.753 | 6.959 | 7.308 | 61.992 | 0.58x |
| wide_arrays.json | json | 9.725 | 9.929 | 10.047 | 61.992 | 0.40x |
| mixed.json | strata | 0.213 | 0.218 | 0.235 | 61.992 | 1.00x |
| mixed.json | orjson | 0.271 | 0.281 | 0.292 | 61.992 | 0.78x |
| mixed.json | msgspec | 0.287 | 0.294 | 0.310 | 61.992 | 0.74x |
| mixed.json | ujson | 0.377 | 0.390 | 0.414 | 61.992 | 0.56x |
| mixed.json | json | 0.500 | 0.506 | 0.520 | 61.992 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.653 | 9.924 | 10.309 | 58.578 | 1.00x |
| users.ndjson | orjson | 14.951 | 15.166 | 15.504 | 58.578 | 0.65x |
| users.ndjson | msgspec | 15.287 | 15.406 | 15.733 | 58.578 | 0.64x |
| users.ndjson | ujson | 19.810 | 20.196 | 20.605 | 58.578 | 0.49x |
| users.ndjson | json | 25.951 | 26.455 | 27.069 | 58.578 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.457 | 2.501 | 2.570 | 60.113 | 1.00x |
| users.json | orjson | 3.034 | 3.079 | 3.131 | 60.113 | 0.81x |
| users.json | msgspec | 3.768 | 3.792 | 3.867 | 60.113 | 0.66x |
| users.json | ujson | 11.051 | 11.114 | 11.164 | 60.113 | 0.23x |
| users.json | json | 19.588 | 19.651 | 19.758 | 60.113 | 0.13x |
| flat.json | strata | 0.391 | 0.416 | 0.433 | 58.578 | 1.00x |
| flat.json | orjson | 0.493 | 0.510 | 0.533 | 58.578 | 0.82x |
| flat.json | msgspec | 0.583 | 0.590 | 0.640 | 58.578 | 0.70x |
| flat.json | ujson | 1.209 | 1.220 | 1.236 | 58.578 | 0.34x |
| flat.json | json | 1.928 | 1.945 | 1.973 | 58.578 | 0.21x |
| nested.json | strata | 0.377 | 0.399 | 0.420 | 58.578 | 1.00x |
| nested.json | orjson | 0.468 | 0.488 | 0.521 | 58.578 | 0.82x |
| nested.json | msgspec | 0.548 | 0.575 | 0.602 | 58.578 | 0.69x |
| nested.json | ujson | 1.292 | 1.309 | 1.349 | 58.578 | 0.31x |
| nested.json | json | 2.357 | 2.401 | 2.413 | 58.578 | 0.17x |
| wide_arrays.json | strata | 1.672 | 1.731 | 1.761 | 61.992 | 1.00x |
| wide_arrays.json | orjson | 1.961 | 2.003 | 2.045 | 61.992 | 0.86x |
| wide_arrays.json | msgspec | 2.672 | 2.748 | 2.799 | 61.992 | 0.63x |
| wide_arrays.json | ujson | 5.149 | 5.193 | 5.282 | 61.992 | 0.33x |
| wide_arrays.json | json | 14.028 | 14.063 | 14.151 | 61.992 | 0.12x |
| mixed.json | strata | 0.176 | 0.186 | 0.206 | 61.992 | 1.00x |
| mixed.json | orjson | 0.180 | 0.212 | 0.229 | 61.992 | 0.88x |
| mixed.json | msgspec | 0.192 | 0.215 | 0.242 | 61.992 | 0.86x |
| mixed.json | ujson | 0.379 | 0.402 | 0.429 | 61.992 | 0.46x |
| mixed.json | json | 0.616 | 0.649 | 0.665 | 61.992 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.098 | 0.098 | 0.099 | 60.117 | 1.00x |
| users.json $[*].id | jmespath | 0.457 | 0.466 | 0.474 | 60.117 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.404 | 2.431 | 2.465 | 60.117 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.591 | 0.604 | 0.609 | 60.227 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.898 | 2.926 | 2.973 | 60.227 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.851 | 17.267 | 17.550 | 60.227 | 0.03x |
| users.json $..total | strata | 1.683 | 1.707 | 1.726 | 62.090 | 1.00x |
| users.json $..total | jsonpath-ng | 291.905 | 292.483 | 293.034 | 62.090 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.159 | 3.186 | 3.203 | 60.227 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.254 | 12.328 | 12.513 | 60.227 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 14.120 | 14.225 | 14.339 | 60.227 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.351 | 3.386 | 3.415 | 62.090 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.920 | 15.098 | 15.527 | 62.090 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 32.669 | 32.956 | 33.553 | 62.090 | 0.10x |
| users.json $..total | strata | 11.212 | 11.583 | 12.356 | 62.105 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 309.402 | 312.737 | 314.151 | 62.105 | 0.04x |

