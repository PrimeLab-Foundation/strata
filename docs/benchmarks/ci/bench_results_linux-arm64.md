# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b6e2a27
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
| users.json | strata | 8.764 | 8.843 | 10.405 | 51.887 | 1.00x |
| users.json | orjson | 11.354 | 11.500 | 12.715 | 51.887 | 0.77x |
| users.json | msgspec | 11.908 | 12.016 | 13.122 | 51.887 | 0.74x |
| users.json | ujson | 16.088 | 16.592 | 18.235 | 51.887 | 0.53x |
| users.json | pysimdjson | 16.112 | 16.599 | 17.771 | 51.887 | 0.53x |
| users.json | json | 20.209 | 20.388 | 20.846 | 51.887 | 0.43x |
| flat.json | strata | 0.797 | 0.817 | 0.839 | 61.754 | 1.00x |
| flat.json | orjson | 0.857 | 0.870 | 0.881 | 61.754 | 0.94x |
| flat.json | msgspec | 0.908 | 0.930 | 0.948 | 61.754 | 0.88x |
| flat.json | ujson | 1.435 | 1.452 | 1.475 | 61.754 | 0.56x |
| flat.json | pysimdjson | 1.475 | 1.488 | 1.507 | 61.754 | 0.55x |
| flat.json | json | 1.789 | 1.805 | 1.825 | 61.754 | 0.45x |
| nested.json | strata | 0.788 | 0.814 | 0.817 | 61.754 | 1.00x |
| nested.json | orjson | 0.866 | 0.887 | 0.900 | 61.754 | 0.92x |
| nested.json | msgspec | 0.975 | 0.996 | 1.241 | 61.754 | 0.82x |
| nested.json | ujson | 1.386 | 1.403 | 1.424 | 61.754 | 0.58x |
| nested.json | pysimdjson | 1.389 | 1.401 | 1.411 | 61.754 | 0.58x |
| nested.json | json | 1.958 | 1.971 | 1.978 | 61.754 | 0.41x |
| wide_arrays.json | strata | 4.319 | 4.339 | 4.402 | 63.320 | 1.00x |
| wide_arrays.json | orjson | 3.953 | 3.999 | 4.026 | 63.320 | 1.08x |
| wide_arrays.json | msgspec | 4.951 | 4.975 | 5.139 | 63.320 | 0.87x |
| wide_arrays.json | ujson | 6.377 | 6.406 | 6.505 | 63.320 | 0.68x |
| wide_arrays.json | pysimdjson | 5.147 | 5.192 | 5.281 | 63.320 | 0.84x |
| wide_arrays.json | json | 9.421 | 9.461 | 9.487 | 63.320 | 0.46x |
| mixed.json | strata | 0.190 | 0.196 | 0.221 | 63.320 | 1.00x |
| mixed.json | orjson | 0.213 | 0.219 | 0.243 | 63.320 | 0.89x |
| mixed.json | msgspec | 0.237 | 0.241 | 0.264 | 63.320 | 0.82x |
| mixed.json | ujson | 0.303 | 0.313 | 0.329 | 63.320 | 0.63x |
| mixed.json | pysimdjson | 0.293 | 0.298 | 0.328 | 63.320 | 0.66x |
| mixed.json | json | 0.451 | 0.459 | 0.486 | 63.320 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.055 | 2.064 | 2.076 | 50.984 | 1.00x |
| users.json | orjson | 2.569 | 2.577 | 2.592 | 50.984 | 0.80x |
| users.json | msgspec | 3.301 | 3.314 | 3.325 | 50.984 | 0.62x |
| users.json | ujson | 10.478 | 10.524 | 10.540 | 50.984 | 0.20x |
| users.json | json | 19.029 | 19.092 | 19.376 | 50.984 | 0.11x |
| flat.json | strata | 0.229 | 0.233 | 0.263 | 61.754 | 1.00x |
| flat.json | orjson | 0.300 | 0.301 | 0.319 | 61.754 | 0.77x |
| flat.json | msgspec | 0.383 | 0.388 | 0.407 | 61.754 | 0.60x |
| flat.json | ujson | 0.985 | 0.990 | 1.002 | 61.754 | 0.24x |
| flat.json | json | 1.686 | 1.708 | 1.729 | 61.754 | 0.14x |
| nested.json | strata | 0.228 | 0.233 | 0.253 | 61.754 | 1.00x |
| nested.json | orjson | 0.284 | 0.287 | 0.298 | 61.754 | 0.81x |
| nested.json | msgspec | 0.367 | 0.371 | 0.391 | 61.754 | 0.63x |
| nested.json | ujson | 1.068 | 1.083 | 1.111 | 61.754 | 0.22x |
| nested.json | json | 2.148 | 2.154 | 2.204 | 61.754 | 0.11x |
| wide_arrays.json | strata | 1.254 | 1.264 | 1.283 | 63.320 | 1.00x |
| wide_arrays.json | orjson | 1.529 | 1.557 | 1.566 | 63.320 | 0.81x |
| wide_arrays.json | msgspec | 2.303 | 2.312 | 2.338 | 63.320 | 0.55x |
| wide_arrays.json | ujson | 4.693 | 4.721 | 4.743 | 63.320 | 0.27x |
| wide_arrays.json | json | 13.498 | 13.526 | 13.566 | 63.320 | 0.09x |
| mixed.json | strata | 0.067 | 0.069 | 0.070 | 63.320 | 1.00x |
| mixed.json | orjson | 0.064 | 0.065 | 0.067 | 63.320 | 1.07x |
| mixed.json | msgspec | 0.077 | 0.079 | 0.081 | 63.320 | 0.88x |
| mixed.json | ujson | 0.239 | 0.245 | 0.267 | 63.320 | 0.28x |
| mixed.json | json | 0.473 | 0.478 | 0.495 | 63.320 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.105 | 9.254 | 10.074 | 63.152 | 1.00x |
| users.json | orjson | 11.783 | 12.052 | 12.341 | 63.152 | 0.77x |
| users.json | msgspec | 12.410 | 12.531 | 12.633 | 63.152 | 0.74x |
| users.json | ujson | 17.221 | 17.535 | 17.988 | 63.152 | 0.53x |
| users.json | json | 20.726 | 21.209 | 21.366 | 63.152 | 0.44x |
| flat.json | strata | 0.840 | 0.856 | 0.893 | 61.754 | 1.00x |
| flat.json | orjson | 0.926 | 0.936 | 0.954 | 61.754 | 0.92x |
| flat.json | msgspec | 0.985 | 0.996 | 1.015 | 61.754 | 0.86x |
| flat.json | ujson | 1.537 | 1.547 | 1.611 | 61.754 | 0.55x |
| flat.json | json | 1.851 | 1.860 | 1.877 | 61.754 | 0.46x |
| nested.json | strata | 0.842 | 0.848 | 0.852 | 61.754 | 1.00x |
| nested.json | orjson | 0.947 | 0.952 | 0.958 | 61.754 | 0.89x |
| nested.json | msgspec | 1.051 | 1.067 | 1.071 | 61.754 | 0.80x |
| nested.json | ujson | 1.467 | 1.475 | 1.487 | 61.754 | 0.58x |
| nested.json | json | 2.013 | 2.026 | 2.042 | 61.754 | 0.42x |
| wide_arrays.json | strata | 4.386 | 4.415 | 4.462 | 63.320 | 1.00x |
| wide_arrays.json | orjson | 4.040 | 4.080 | 4.232 | 63.320 | 1.08x |
| wide_arrays.json | msgspec | 5.053 | 5.072 | 5.143 | 63.320 | 0.87x |
| wide_arrays.json | ujson | 6.621 | 6.671 | 6.749 | 63.320 | 0.66x |
| wide_arrays.json | json | 9.529 | 9.588 | 9.656 | 63.320 | 0.46x |
| mixed.json | strata | 0.220 | 0.224 | 0.247 | 63.320 | 1.00x |
| mixed.json | orjson | 0.276 | 0.284 | 0.291 | 63.320 | 0.79x |
| mixed.json | msgspec | 0.291 | 0.298 | 0.323 | 63.320 | 0.75x |
| mixed.json | ujson | 0.375 | 0.385 | 0.394 | 63.320 | 0.58x |
| mixed.json | json | 0.499 | 0.512 | 0.528 | 63.320 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.378 | 9.599 | 9.687 | 61.746 | 1.00x |
| users.ndjson | orjson | 14.590 | 14.826 | 14.997 | 61.746 | 0.65x |
| users.ndjson | msgspec | 14.762 | 15.046 | 15.290 | 61.746 | 0.64x |
| users.ndjson | ujson | 19.251 | 19.719 | 20.023 | 61.746 | 0.49x |
| users.ndjson | json | 25.720 | 26.030 | 26.149 | 61.746 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.560 | 2.606 | 2.645 | 61.152 | 1.00x |
| users.json | orjson | 3.097 | 3.162 | 3.243 | 61.152 | 0.82x |
| users.json | msgspec | 3.819 | 3.873 | 3.972 | 61.152 | 0.67x |
| users.json | ujson | 11.126 | 11.213 | 11.311 | 61.152 | 0.23x |
| users.json | json | 19.680 | 19.804 | 19.940 | 61.152 | 0.13x |
| flat.json | strata | 0.389 | 0.414 | 0.450 | 61.754 | 1.00x |
| flat.json | orjson | 0.492 | 0.505 | 0.528 | 61.754 | 0.82x |
| flat.json | msgspec | 0.573 | 0.598 | 0.619 | 61.754 | 0.69x |
| flat.json | ujson | 1.209 | 1.223 | 1.280 | 61.754 | 0.34x |
| flat.json | json | 1.905 | 1.932 | 1.971 | 61.754 | 0.21x |
| nested.json | strata | 0.383 | 0.396 | 0.435 | 61.754 | 1.00x |
| nested.json | orjson | 0.451 | 0.486 | 0.502 | 61.754 | 0.82x |
| nested.json | msgspec | 0.553 | 0.581 | 0.595 | 61.754 | 0.68x |
| nested.json | ujson | 1.283 | 1.314 | 1.341 | 61.754 | 0.30x |
| nested.json | json | 2.333 | 2.373 | 2.407 | 61.754 | 0.17x |
| wide_arrays.json | strata | 1.616 | 1.669 | 1.730 | 63.320 | 1.00x |
| wide_arrays.json | orjson | 1.949 | 1.986 | 2.009 | 63.320 | 0.84x |
| wide_arrays.json | msgspec | 2.696 | 2.738 | 2.752 | 63.320 | 0.61x |
| wide_arrays.json | ujson | 5.128 | 5.168 | 5.204 | 63.320 | 0.32x |
| wide_arrays.json | json | 13.947 | 13.983 | 14.106 | 63.320 | 0.12x |
| mixed.json | strata | 0.167 | 0.174 | 0.196 | 63.320 | 1.00x |
| mixed.json | orjson | 0.180 | 0.185 | 0.201 | 63.320 | 0.94x |
| mixed.json | msgspec | 0.191 | 0.198 | 0.209 | 63.320 | 0.88x |
| mixed.json | ujson | 0.364 | 0.377 | 0.407 | 63.320 | 0.46x |
| mixed.json | json | 0.604 | 0.617 | 0.641 | 63.320 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.099 | 0.102 | 0.103 | 61.152 | 1.00x |
| users.json $[*].id | jmespath | 0.463 | 0.476 | 0.484 | 61.152 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.431 | 2.484 | 2.567 | 61.152 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.592 | 0.610 | 0.623 | 61.281 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.946 | 2.952 | 2.980 | 61.281 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.217 | 17.921 | 18.220 | 61.281 | 0.03x |
| users.json $..total | strata | 1.698 | 1.719 | 1.729 | 63.320 | 1.00x |
| users.json $..total | jsonpath-ng | 292.910 | 293.244 | 293.786 | 63.320 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.362 | 3.385 | 3.398 | 61.281 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.463 | 12.687 | 12.834 | 61.281 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 14.227 | 14.378 | 14.646 | 61.281 | 0.24x |
| users.json $[*].orders[*].total | strata | 3.516 | 3.525 | 3.587 | 63.320 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.972 | 15.254 | 15.456 | 63.320 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 32.805 | 33.035 | 33.962 | 63.320 | 0.11x |
| users.json $..total | strata | 11.428 | 11.628 | 11.755 | 63.379 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 311.907 | 313.076 | 314.152 | 63.379 | 0.04x |

