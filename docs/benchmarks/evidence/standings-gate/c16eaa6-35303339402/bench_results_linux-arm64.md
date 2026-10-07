# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-aarch64-with-glibc2.39
- machine: aarch64
- processor: aarch64
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/arm64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.623 | 8.710 | 10.361 | 57.223 | 1.00x |
| users.json | orjson | 11.348 | 11.399 | 12.816 | 57.223 | 0.76x |
| users.json | msgspec | 11.884 | 11.942 | 13.290 | 57.223 | 0.73x |
| users.json | ujson | 15.875 | 15.988 | 18.036 | 57.223 | 0.54x |
| users.json | pysimdjson | 15.910 | 16.028 | 17.598 | 57.223 | 0.54x |
| users.json | json | 20.112 | 20.184 | 20.860 | 57.223 | 0.43x |
| flat.json | strata | 0.773 | 0.785 | 0.793 | 67.992 | 1.00x |
| flat.json | orjson | 0.834 | 0.846 | 0.858 | 67.992 | 0.93x |
| flat.json | msgspec | 0.890 | 0.900 | 0.904 | 67.992 | 0.87x |
| flat.json | ujson | 1.387 | 1.403 | 1.411 | 67.992 | 0.56x |
| flat.json | pysimdjson | 1.447 | 1.457 | 1.471 | 67.992 | 0.54x |
| flat.json | json | 1.770 | 1.775 | 1.794 | 67.992 | 0.44x |
| nested.json | strata | 0.799 | 0.814 | 0.824 | 67.992 | 1.00x |
| nested.json | orjson | 0.856 | 0.869 | 0.884 | 67.992 | 0.94x |
| nested.json | msgspec | 0.974 | 0.978 | 0.986 | 67.992 | 0.83x |
| nested.json | ujson | 1.364 | 1.378 | 1.405 | 67.992 | 0.59x |
| nested.json | pysimdjson | 1.373 | 1.388 | 1.420 | 67.992 | 0.59x |
| nested.json | json | 1.912 | 1.921 | 1.935 | 67.992 | 0.42x |
| wide_arrays.json | strata | 3.800 | 3.813 | 3.828 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 3.995 | 4.010 | 4.034 | 69.562 | 0.95x |
| wide_arrays.json | msgspec | 4.978 | 4.997 | 5.015 | 69.562 | 0.76x |
| wide_arrays.json | ujson | 6.365 | 6.387 | 6.419 | 69.562 | 0.60x |
| wide_arrays.json | pysimdjson | 5.187 | 5.202 | 5.242 | 69.562 | 0.73x |
| wide_arrays.json | json | 9.330 | 9.345 | 9.459 | 69.562 | 0.41x |
| mixed.json | strata | 0.185 | 0.186 | 0.209 | 69.562 | 1.00x |
| mixed.json | orjson | 0.207 | 0.209 | 0.231 | 69.562 | 0.89x |
| mixed.json | msgspec | 0.229 | 0.233 | 0.247 | 69.562 | 0.80x |
| mixed.json | ujson | 0.296 | 0.305 | 0.313 | 69.562 | 0.61x |
| mixed.json | pysimdjson | 0.289 | 0.292 | 0.310 | 69.562 | 0.64x |
| mixed.json | json | 0.440 | 0.450 | 0.472 | 69.562 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.891 | 1.901 | 1.922 | 56.332 | 1.00x |
| users.json | orjson | 2.558 | 2.566 | 2.572 | 56.332 | 0.74x |
| users.json | msgspec | 3.286 | 3.293 | 3.309 | 56.332 | 0.58x |
| users.json | ujson | 10.449 | 10.464 | 10.520 | 56.332 | 0.18x |
| users.json | json | 18.808 | 18.896 | 18.975 | 56.332 | 0.10x |
| flat.json | strata | 0.228 | 0.231 | 0.246 | 67.992 | 1.00x |
| flat.json | orjson | 0.297 | 0.299 | 0.316 | 67.992 | 0.77x |
| flat.json | msgspec | 0.385 | 0.388 | 0.409 | 67.992 | 0.59x |
| flat.json | ujson | 0.976 | 0.984 | 0.991 | 67.992 | 0.23x |
| flat.json | json | 1.679 | 1.699 | 1.727 | 67.992 | 0.14x |
| nested.json | strata | 0.208 | 0.209 | 0.225 | 67.992 | 1.00x |
| nested.json | orjson | 0.274 | 0.277 | 0.291 | 67.992 | 0.75x |
| nested.json | msgspec | 0.359 | 0.363 | 0.376 | 67.992 | 0.58x |
| nested.json | ujson | 1.058 | 1.065 | 1.073 | 67.992 | 0.20x |
| nested.json | json | 2.105 | 2.125 | 2.176 | 67.992 | 0.10x |
| wide_arrays.json | strata | 1.319 | 1.323 | 1.345 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 1.572 | 1.582 | 1.604 | 69.562 | 0.84x |
| wide_arrays.json | msgspec | 2.333 | 2.346 | 2.365 | 69.562 | 0.56x |
| wide_arrays.json | ujson | 4.725 | 4.735 | 4.751 | 69.562 | 0.28x |
| wide_arrays.json | json | 13.475 | 13.490 | 13.514 | 69.562 | 0.10x |
| mixed.json | strata | 0.057 | 0.058 | 0.059 | 69.562 | 1.00x |
| mixed.json | orjson | 0.060 | 0.061 | 0.074 | 69.562 | 0.95x |
| mixed.json | msgspec | 0.073 | 0.074 | 0.076 | 69.562 | 0.79x |
| mixed.json | ujson | 0.233 | 0.237 | 0.241 | 69.562 | 0.25x |
| mixed.json | json | 0.468 | 0.471 | 0.487 | 69.562 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.807 | 8.842 | 9.468 | 68.426 | 1.00x |
| users.json | orjson | 11.474 | 11.580 | 11.915 | 68.426 | 0.76x |
| users.json | msgspec | 12.097 | 12.138 | 12.408 | 68.426 | 0.73x |
| users.json | ujson | 16.367 | 16.491 | 17.361 | 68.426 | 0.54x |
| users.json | json | 20.381 | 20.438 | 20.480 | 68.426 | 0.43x |
| flat.json | strata | 0.806 | 0.816 | 0.823 | 67.992 | 1.00x |
| flat.json | orjson | 0.911 | 0.914 | 0.922 | 67.992 | 0.89x |
| flat.json | msgspec | 0.964 | 0.968 | 0.984 | 67.992 | 0.84x |
| flat.json | ujson | 1.496 | 1.506 | 1.512 | 67.992 | 0.54x |
| flat.json | json | 1.821 | 1.833 | 1.838 | 67.992 | 0.45x |
| nested.json | strata | 0.829 | 0.846 | 0.868 | 67.992 | 1.00x |
| nested.json | orjson | 0.919 | 0.936 | 0.943 | 67.992 | 0.90x |
| nested.json | msgspec | 1.037 | 1.043 | 1.056 | 67.992 | 0.81x |
| nested.json | ujson | 1.451 | 1.466 | 1.482 | 67.992 | 0.58x |
| nested.json | json | 1.980 | 1.994 | 2.011 | 67.992 | 0.42x |
| wide_arrays.json | strata | 3.803 | 3.812 | 3.821 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 3.965 | 4.002 | 4.016 | 69.562 | 0.95x |
| wide_arrays.json | msgspec | 4.992 | 5.002 | 5.011 | 69.562 | 0.76x |
| wide_arrays.json | ujson | 6.497 | 6.538 | 6.552 | 69.562 | 0.58x |
| wide_arrays.json | json | 9.337 | 9.378 | 9.427 | 69.562 | 0.41x |
| mixed.json | strata | 0.205 | 0.206 | 0.221 | 69.562 | 1.00x |
| mixed.json | orjson | 0.266 | 0.268 | 0.282 | 69.562 | 0.77x |
| mixed.json | msgspec | 0.285 | 0.288 | 0.304 | 69.562 | 0.71x |
| mixed.json | ujson | 0.361 | 0.369 | 0.386 | 69.562 | 0.56x |
| mixed.json | json | 0.494 | 0.505 | 0.519 | 69.562 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.103 | 9.127 | 9.148 | 67.984 | 1.00x |
| users.ndjson | orjson | 14.242 | 14.278 | 14.314 | 67.984 | 0.64x |
| users.ndjson | msgspec | 14.667 | 14.691 | 14.773 | 67.984 | 0.62x |
| users.ndjson | ujson | 18.962 | 19.014 | 19.087 | 67.984 | 0.48x |
| users.ndjson | json | 25.079 | 25.124 | 25.200 | 67.984 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.323 | 2.346 | 2.363 | 68.426 | 1.00x |
| users.json | orjson | 3.032 | 3.050 | 3.125 | 68.426 | 0.77x |
| users.json | msgspec | 3.739 | 3.775 | 3.868 | 68.426 | 0.62x |
| users.json | ujson | 10.974 | 11.035 | 11.167 | 68.426 | 0.21x |
| users.json | json | 19.440 | 19.483 | 19.605 | 68.426 | 0.12x |
| flat.json | strata | 0.375 | 0.403 | 0.420 | 67.992 | 1.00x |
| flat.json | orjson | 0.460 | 0.491 | 0.513 | 67.992 | 0.82x |
| flat.json | msgspec | 0.556 | 0.588 | 0.626 | 67.992 | 0.69x |
| flat.json | ujson | 1.177 | 1.215 | 1.259 | 67.992 | 0.33x |
| flat.json | json | 1.887 | 1.913 | 1.927 | 67.992 | 0.21x |
| nested.json | strata | 0.331 | 0.343 | 0.358 | 67.992 | 1.00x |
| nested.json | orjson | 0.423 | 0.434 | 0.458 | 67.992 | 0.79x |
| nested.json | msgspec | 0.516 | 0.521 | 0.543 | 67.992 | 0.66x |
| nested.json | ujson | 1.229 | 1.263 | 1.295 | 67.992 | 0.27x |
| nested.json | json | 2.295 | 2.316 | 2.356 | 67.992 | 0.15x |
| wide_arrays.json | strata | 1.627 | 1.657 | 1.683 | 69.562 | 1.00x |
| wide_arrays.json | orjson | 1.927 | 1.947 | 1.979 | 69.562 | 0.85x |
| wide_arrays.json | msgspec | 2.690 | 2.715 | 2.761 | 69.562 | 0.61x |
| wide_arrays.json | ujson | 5.132 | 5.156 | 5.178 | 69.562 | 0.32x |
| wide_arrays.json | json | 13.872 | 13.887 | 13.907 | 69.562 | 0.12x |
| mixed.json | strata | 0.152 | 0.160 | 0.182 | 69.562 | 1.00x |
| mixed.json | orjson | 0.176 | 0.181 | 0.205 | 69.562 | 0.88x |
| mixed.json | msgspec | 0.189 | 0.195 | 0.219 | 69.562 | 0.82x |
| mixed.json | ujson | 0.364 | 0.370 | 0.398 | 69.562 | 0.43x |
| mixed.json | json | 0.593 | 0.621 | 0.633 | 69.562 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.097 | 0.098 | 0.114 | 68.426 | 1.00x |
| users.json $[*].id | jmespath | 0.462 | 0.469 | 0.477 | 68.426 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.413 | 2.435 | 2.461 | 68.426 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.591 | 0.601 | 0.611 | 68.551 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.922 | 2.942 | 2.965 | 68.551 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.941 | 17.077 | 17.202 | 68.551 | 0.04x |
| users.json $..total | strata | 1.678 | 1.693 | 1.703 | 69.559 | 1.00x |
| users.json $..total | jsonpath-ng | 292.560 | 293.142 | 294.420 | 69.559 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.093 | 3.099 | 3.111 | 68.551 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.043 | 12.091 | 12.170 | 68.551 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 13.931 | 14.021 | 14.207 | 68.551 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.263 | 3.271 | 3.297 | 69.559 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.705 | 14.746 | 14.913 | 69.559 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 32.599 | 32.765 | 32.794 | 69.559 | 0.10x |
| users.json $..total | strata | 10.925 | 11.103 | 11.269 | 69.621 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 307.059 | 308.642 | 310.527 | 69.621 | 0.04x |

