# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 1e1e47c
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
| users.json | strata | 12.391 | 13.327 | 22.588 | 58.551 | 1.00x |
| users.json | orjson | 14.967 | 15.618 | 18.329 | 58.551 | 0.85x |
| users.json | msgspec | 14.002 | 15.742 | 19.777 | 58.551 | 0.85x |
| users.json | ujson | 21.522 | 22.621 | 25.785 | 58.551 | 0.59x |
| users.json | pysimdjson | 21.785 | 23.972 | 26.402 | 58.551 | 0.56x |
| users.json | json | 24.236 | 24.653 | 25.999 | 58.551 | 0.54x |
| flat.json | strata | 0.875 | 0.891 | 0.909 | 69.660 | 1.00x |
| flat.json | orjson | 0.989 | 1.001 | 1.062 | 69.660 | 0.89x |
| flat.json | msgspec | 1.031 | 1.049 | 1.076 | 69.660 | 0.85x |
| flat.json | ujson | 1.496 | 1.574 | 1.679 | 69.660 | 0.57x |
| flat.json | pysimdjson | 1.540 | 1.558 | 1.620 | 69.660 | 0.57x |
| flat.json | json | 1.926 | 1.944 | 2.039 | 69.660 | 0.46x |
| nested.json | strata | 0.873 | 0.898 | 0.951 | 69.660 | 1.00x |
| nested.json | orjson | 1.006 | 1.015 | 1.053 | 69.660 | 0.89x |
| nested.json | msgspec | 1.030 | 1.042 | 1.087 | 69.660 | 0.86x |
| nested.json | ujson | 1.510 | 1.545 | 1.625 | 69.660 | 0.58x |
| nested.json | pysimdjson | 1.412 | 1.433 | 1.443 | 69.660 | 0.63x |
| nested.json | json | 2.034 | 2.054 | 2.141 | 69.660 | 0.44x |
| wide_arrays.json | strata | 4.632 | 4.750 | 5.533 | 77.270 | 1.00x |
| wide_arrays.json | orjson | 5.181 | 5.510 | 6.097 | 77.270 | 0.86x |
| wide_arrays.json | msgspec | 5.752 | 5.805 | 7.045 | 77.270 | 0.82x |
| wide_arrays.json | ujson | 7.221 | 7.363 | 7.995 | 77.270 | 0.65x |
| wide_arrays.json | pysimdjson | 6.005 | 6.188 | 6.599 | 77.270 | 0.77x |
| wide_arrays.json | json | 9.710 | 9.992 | 10.538 | 77.270 | 0.48x |
| mixed.json | strata | 0.207 | 0.214 | 0.240 | 77.332 | 1.00x |
| mixed.json | orjson | 0.229 | 0.239 | 0.277 | 77.332 | 0.89x |
| mixed.json | msgspec | 0.240 | 0.245 | 0.310 | 77.332 | 0.87x |
| mixed.json | ujson | 0.317 | 0.330 | 0.390 | 77.332 | 0.65x |
| mixed.json | pysimdjson | 0.296 | 0.303 | 0.333 | 77.332 | 0.71x |
| mixed.json | json | 0.471 | 0.489 | 0.504 | 77.332 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.632 | 2.829 | 3.195 | 57.625 | 1.00x |
| users.json | orjson | 2.951 | 3.111 | 3.398 | 57.625 | 0.91x |
| users.json | msgspec | 3.906 | 4.066 | 4.298 | 57.625 | 0.70x |
| users.json | ujson | 15.217 | 15.683 | 15.955 | 57.625 | 0.18x |
| users.json | json | 22.612 | 22.933 | 23.199 | 57.625 | 0.12x |
| flat.json | strata | 0.253 | 0.256 | 0.268 | 69.660 | 1.00x |
| flat.json | orjson | 0.328 | 0.337 | 0.404 | 69.660 | 0.76x |
| flat.json | msgspec | 0.433 | 0.447 | 0.465 | 69.660 | 0.57x |
| flat.json | ujson | 1.283 | 1.296 | 1.331 | 69.660 | 0.20x |
| flat.json | json | 1.828 | 1.874 | 1.966 | 69.660 | 0.14x |
| nested.json | strata | 0.244 | 0.250 | 0.274 | 69.660 | 1.00x |
| nested.json | orjson | 0.291 | 0.304 | 0.319 | 69.660 | 0.82x |
| nested.json | msgspec | 0.398 | 0.400 | 0.417 | 69.660 | 0.62x |
| nested.json | ujson | 1.419 | 1.426 | 1.437 | 69.660 | 0.18x |
| nested.json | json | 2.352 | 2.370 | 2.681 | 69.660 | 0.11x |
| wide_arrays.json | strata | 1.571 | 1.587 | 1.644 | 77.270 | 1.00x |
| wide_arrays.json | orjson | 1.812 | 1.824 | 1.961 | 77.270 | 0.87x |
| wide_arrays.json | msgspec | 2.675 | 2.700 | 2.772 | 77.270 | 0.59x |
| wide_arrays.json | ujson | 8.603 | 8.660 | 8.865 | 77.270 | 0.18x |
| wide_arrays.json | json | 16.535 | 16.604 | 17.127 | 77.270 | 0.10x |
| mixed.json | strata | 0.065 | 0.066 | 0.231 | 77.332 | 1.00x |
| mixed.json | orjson | 0.065 | 0.067 | 0.135 | 77.332 | 0.99x |
| mixed.json | msgspec | 0.083 | 0.086 | 0.163 | 77.332 | 0.77x |
| mixed.json | ujson | 0.300 | 0.305 | 0.732 | 77.332 | 0.21x |
| mixed.json | json | 0.521 | 0.527 | 0.865 | 77.332 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.731 | 13.163 | 14.907 | 70.117 | 1.00x |
| users.json | orjson | 14.329 | 15.409 | 16.613 | 70.117 | 0.85x |
| users.json | msgspec | 15.148 | 15.885 | 18.455 | 70.117 | 0.83x |
| users.json | ujson | 21.137 | 23.398 | 24.395 | 70.117 | 0.56x |
| users.json | json | 24.619 | 25.130 | 25.928 | 70.117 | 0.52x |
| flat.json | strata | 0.902 | 0.925 | 0.956 | 69.660 | 1.00x |
| flat.json | orjson | 1.047 | 1.065 | 1.157 | 69.660 | 0.87x |
| flat.json | msgspec | 1.078 | 1.092 | 1.138 | 69.660 | 0.85x |
| flat.json | ujson | 1.574 | 1.646 | 1.794 | 69.660 | 0.56x |
| flat.json | json | 1.972 | 2.000 | 2.017 | 69.660 | 0.46x |
| nested.json | strata | 0.913 | 0.930 | 0.968 | 69.660 | 1.00x |
| nested.json | orjson | 1.085 | 1.103 | 1.199 | 69.660 | 0.84x |
| nested.json | msgspec | 1.083 | 1.108 | 1.179 | 69.660 | 0.84x |
| nested.json | ujson | 1.578 | 1.602 | 1.648 | 69.660 | 0.58x |
| nested.json | json | 2.095 | 2.121 | 2.149 | 69.660 | 0.44x |
| wide_arrays.json | strata | 4.804 | 4.954 | 5.537 | 77.332 | 1.00x |
| wide_arrays.json | orjson | 5.286 | 5.572 | 6.151 | 77.332 | 0.89x |
| wide_arrays.json | msgspec | 5.894 | 6.018 | 6.389 | 77.332 | 0.82x |
| wide_arrays.json | ujson | 7.547 | 7.746 | 7.957 | 77.332 | 0.64x |
| wide_arrays.json | json | 9.896 | 10.116 | 10.482 | 77.332 | 0.49x |
| mixed.json | strata | 0.227 | 0.245 | 0.506 | 77.332 | 1.00x |
| mixed.json | orjson | 0.274 | 0.284 | 0.891 | 77.332 | 0.86x |
| mixed.json | msgspec | 0.283 | 0.298 | 0.511 | 77.332 | 0.82x |
| mixed.json | ujson | 0.362 | 0.380 | 0.654 | 77.332 | 0.65x |
| mixed.json | json | 0.516 | 0.534 | 1.102 | 77.332 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 13.906 | 14.725 | 15.359 | 69.660 | 1.00x |
| users.ndjson | orjson | 19.333 | 20.722 | 22.230 | 69.660 | 0.71x |
| users.ndjson | msgspec | 18.899 | 20.672 | 21.816 | 69.660 | 0.71x |
| users.ndjson | ujson | 24.511 | 26.509 | 27.933 | 69.660 | 0.56x |
| users.ndjson | json | 30.393 | 32.974 | 34.937 | 69.660 | 0.45x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.497 | 3.722 | 3.896 | 68.188 | 1.00x |
| users.json | orjson | 3.833 | 3.987 | 4.372 | 68.188 | 0.93x |
| users.json | msgspec | 4.663 | 5.008 | 5.427 | 68.188 | 0.74x |
| users.json | ujson | 16.434 | 16.748 | 17.049 | 68.188 | 0.22x |
| users.json | json | 23.820 | 24.001 | 24.145 | 68.188 | 0.16x |
| flat.json | strata | 0.405 | 0.435 | 0.491 | 69.660 | 1.00x |
| flat.json | orjson | 0.498 | 0.517 | 0.598 | 69.660 | 0.84x |
| flat.json | msgspec | 0.599 | 0.646 | 0.677 | 69.660 | 0.67x |
| flat.json | ujson | 1.467 | 1.505 | 1.543 | 69.660 | 0.29x |
| flat.json | json | 2.057 | 2.102 | 2.156 | 69.660 | 0.21x |
| nested.json | strata | 0.368 | 0.384 | 0.433 | 69.660 | 1.00x |
| nested.json | orjson | 0.431 | 0.447 | 0.476 | 69.660 | 0.86x |
| nested.json | msgspec | 0.537 | 0.552 | 0.801 | 69.660 | 0.70x |
| nested.json | ujson | 1.593 | 1.626 | 1.668 | 69.660 | 0.24x |
| nested.json | json | 2.523 | 2.558 | 2.778 | 69.660 | 0.15x |
| wide_arrays.json | strata | 2.027 | 2.036 | 2.087 | 77.332 | 1.00x |
| wide_arrays.json | orjson | 2.288 | 2.299 | 2.357 | 77.332 | 0.89x |
| wide_arrays.json | msgspec | 3.163 | 3.205 | 3.290 | 77.332 | 0.64x |
| wide_arrays.json | ujson | 9.212 | 9.269 | 9.366 | 77.332 | 0.22x |
| wide_arrays.json | json | 17.001 | 17.229 | 17.843 | 77.332 | 0.12x |
| mixed.json | strata | 0.156 | 0.158 | 0.178 | 77.332 | 1.00x |
| mixed.json | orjson | 0.173 | 0.179 | 0.208 | 77.332 | 0.89x |
| mixed.json | msgspec | 0.190 | 0.194 | 0.242 | 77.332 | 0.82x |
| mixed.json | ujson | 0.420 | 0.435 | 0.468 | 77.332 | 0.36x |
| mixed.json | json | 0.627 | 0.642 | 0.699 | 77.332 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.079 | 0.087 | 0.101 | 68.188 | 1.00x |
| users.json $[*].id | jmespath | 0.533 | 0.549 | 0.614 | 68.188 | 0.16x |
| users.json $[*].id | jsonpath-ng | 3.107 | 3.267 | 3.668 | 68.188 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.488 | 0.738 | 0.920 | 68.316 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.156 | 3.559 | 3.930 | 68.316 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 22.354 | 23.499 | 24.648 | 68.316 | 0.03x |
| users.json $..total | strata | 1.872 | 2.053 | 2.458 | 72.152 | 1.00x |
| users.json $..total | jsonpath-ng | 394.144 | 400.615 | 402.598 | 72.152 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.275 | 3.322 | 3.487 | 68.316 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.457 | 16.812 | 20.353 | 68.316 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 18.450 | 20.114 | 27.823 | 68.316 | 0.17x |
| users.json $[*].orders[*].total | strata | 3.504 | 3.578 | 3.618 | 68.652 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.971 | 21.132 | 22.342 | 68.652 | 0.17x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 43.833 | 45.072 | 48.044 | 68.652 | 0.08x |
| users.json $..total | strata | 16.262 | 17.314 | 18.016 | 71.508 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 421.261 | 422.996 | 436.790 | 71.508 | 0.04x |

