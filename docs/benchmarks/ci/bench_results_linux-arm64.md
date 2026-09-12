# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec0411225b6d58a1df905844c946a766d3c39f0a
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
| users.json | strata | 8.782 | 8.891 | 11.167 | 57.207 | 1.00x |
| users.json | orjson | 11.596 | 11.748 | 13.759 | 57.207 | 0.76x |
| users.json | msgspec | 12.164 | 12.271 | 14.124 | 57.207 | 0.72x |
| users.json | ujson | 16.490 | 16.846 | 19.301 | 57.207 | 0.53x |
| users.json | pysimdjson | 16.388 | 16.920 | 18.998 | 57.207 | 0.53x |
| users.json | json | 20.581 | 20.775 | 21.802 | 57.207 | 0.43x |
| flat.json | strata | 0.795 | 0.819 | 0.831 | 68.102 | 1.00x |
| flat.json | orjson | 0.843 | 0.860 | 0.862 | 68.102 | 0.95x |
| flat.json | msgspec | 0.909 | 0.926 | 0.931 | 68.102 | 0.88x |
| flat.json | ujson | 1.450 | 1.460 | 1.480 | 68.102 | 0.56x |
| flat.json | pysimdjson | 1.489 | 1.505 | 1.520 | 68.102 | 0.54x |
| flat.json | json | 1.790 | 1.795 | 1.803 | 68.102 | 0.46x |
| nested.json | strata | 0.806 | 0.827 | 0.843 | 68.102 | 1.00x |
| nested.json | orjson | 0.861 | 0.884 | 0.895 | 68.102 | 0.94x |
| nested.json | msgspec | 0.979 | 0.990 | 1.011 | 68.102 | 0.84x |
| nested.json | ujson | 1.399 | 1.418 | 1.500 | 68.102 | 0.58x |
| nested.json | pysimdjson | 1.390 | 1.407 | 1.538 | 68.102 | 0.59x |
| nested.json | json | 1.941 | 1.951 | 1.968 | 68.102 | 0.42x |
| wide_arrays.json | strata | 3.808 | 3.936 | 4.020 | 69.676 | 1.00x |
| wide_arrays.json | orjson | 4.046 | 4.129 | 4.203 | 69.676 | 0.95x |
| wide_arrays.json | msgspec | 5.017 | 5.124 | 5.239 | 69.676 | 0.77x |
| wide_arrays.json | ujson | 6.538 | 6.613 | 6.705 | 69.676 | 0.60x |
| wide_arrays.json | pysimdjson | 5.269 | 5.347 | 5.577 | 69.676 | 0.74x |
| wide_arrays.json | json | 9.541 | 9.618 | 9.720 | 69.676 | 0.41x |
| mixed.json | strata | 0.190 | 0.192 | 0.216 | 69.676 | 1.00x |
| mixed.json | orjson | 0.209 | 0.216 | 0.242 | 69.676 | 0.89x |
| mixed.json | msgspec | 0.231 | 0.243 | 0.259 | 69.676 | 0.79x |
| mixed.json | ujson | 0.308 | 0.331 | 0.339 | 69.676 | 0.58x |
| mixed.json | pysimdjson | 0.295 | 0.306 | 0.325 | 69.676 | 0.63x |
| mixed.json | json | 0.456 | 0.477 | 0.507 | 69.676 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.932 | 1.938 | 1.948 | 56.316 | 1.00x |
| users.json | orjson | 2.595 | 2.607 | 2.624 | 56.316 | 0.74x |
| users.json | msgspec | 3.307 | 3.327 | 3.344 | 56.316 | 0.58x |
| users.json | ujson | 10.502 | 10.609 | 10.635 | 56.316 | 0.18x |
| users.json | json | 18.951 | 19.048 | 19.113 | 56.316 | 0.10x |
| flat.json | strata | 0.235 | 0.236 | 0.254 | 68.102 | 1.00x |
| flat.json | orjson | 0.299 | 0.303 | 0.317 | 68.102 | 0.78x |
| flat.json | msgspec | 0.388 | 0.405 | 0.413 | 68.102 | 0.58x |
| flat.json | ujson | 0.992 | 1.000 | 1.017 | 68.102 | 0.24x |
| flat.json | json | 1.704 | 1.714 | 1.747 | 68.102 | 0.14x |
| nested.json | strata | 0.214 | 0.219 | 0.232 | 68.105 | 1.00x |
| nested.json | orjson | 0.279 | 0.280 | 0.295 | 68.105 | 0.78x |
| nested.json | msgspec | 0.365 | 0.367 | 0.384 | 68.105 | 0.60x |
| nested.json | ujson | 1.069 | 1.073 | 1.088 | 68.105 | 0.20x |
| nested.json | json | 2.138 | 2.152 | 2.183 | 68.105 | 0.10x |
| wide_arrays.json | strata | 1.330 | 1.354 | 1.450 | 69.676 | 1.00x |
| wide_arrays.json | orjson | 1.586 | 1.613 | 1.673 | 69.676 | 0.84x |
| wide_arrays.json | msgspec | 2.364 | 2.384 | 2.425 | 69.676 | 0.57x |
| wide_arrays.json | ujson | 4.769 | 4.807 | 4.858 | 69.676 | 0.28x |
| wide_arrays.json | json | 13.543 | 13.611 | 13.762 | 69.676 | 0.10x |
| mixed.json | strata | 0.063 | 0.065 | 0.079 | 69.676 | 1.00x |
| mixed.json | orjson | 0.065 | 0.066 | 0.068 | 69.676 | 0.99x |
| mixed.json | msgspec | 0.079 | 0.080 | 0.103 | 69.676 | 0.81x |
| mixed.json | ujson | 0.244 | 0.250 | 0.262 | 69.676 | 0.26x |
| mixed.json | json | 0.486 | 0.501 | 0.515 | 69.676 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.988 | 9.235 | 9.978 | 68.535 | 1.00x |
| users.json | orjson | 12.113 | 12.274 | 12.593 | 68.535 | 0.75x |
| users.json | msgspec | 12.581 | 12.760 | 12.950 | 68.535 | 0.72x |
| users.json | ujson | 16.894 | 17.795 | 18.682 | 68.535 | 0.52x |
| users.json | json | 20.993 | 21.277 | 21.731 | 68.535 | 0.43x |
| flat.json | strata | 0.834 | 0.864 | 0.893 | 68.102 | 1.00x |
| flat.json | orjson | 0.929 | 0.938 | 0.954 | 68.102 | 0.92x |
| flat.json | msgspec | 0.991 | 1.004 | 1.012 | 68.102 | 0.86x |
| flat.json | ujson | 1.548 | 1.572 | 1.601 | 68.102 | 0.55x |
| flat.json | json | 1.848 | 1.857 | 1.874 | 68.102 | 0.47x |
| nested.json | strata | 0.858 | 0.867 | 0.876 | 68.105 | 1.00x |
| nested.json | orjson | 0.938 | 0.949 | 0.958 | 68.105 | 0.91x |
| nested.json | msgspec | 1.055 | 1.065 | 1.077 | 68.105 | 0.81x |
| nested.json | ujson | 1.468 | 1.494 | 1.507 | 68.105 | 0.58x |
| nested.json | json | 1.998 | 2.013 | 2.052 | 68.105 | 0.43x |
| wide_arrays.json | strata | 3.870 | 3.954 | 4.160 | 69.676 | 1.00x |
| wide_arrays.json | orjson | 4.106 | 4.292 | 4.515 | 69.676 | 0.92x |
| wide_arrays.json | msgspec | 5.094 | 5.277 | 5.429 | 69.676 | 0.75x |
| wide_arrays.json | ujson | 6.675 | 6.894 | 7.092 | 69.676 | 0.57x |
| wide_arrays.json | json | 9.592 | 9.895 | 10.061 | 69.676 | 0.40x |
| mixed.json | strata | 0.220 | 0.230 | 0.256 | 69.676 | 1.00x |
| mixed.json | orjson | 0.278 | 0.290 | 0.316 | 69.676 | 0.80x |
| mixed.json | msgspec | 0.293 | 0.300 | 0.335 | 69.676 | 0.77x |
| mixed.json | ujson | 0.379 | 0.396 | 0.421 | 69.676 | 0.58x |
| mixed.json | json | 0.502 | 0.525 | 0.545 | 69.676 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.412 | 9.601 | 9.756 | 68.094 | 1.00x |
| users.ndjson | orjson | 14.696 | 14.972 | 15.293 | 68.094 | 0.64x |
| users.ndjson | msgspec | 15.009 | 15.305 | 15.488 | 68.094 | 0.63x |
| users.ndjson | ujson | 19.698 | 19.989 | 20.447 | 68.094 | 0.48x |
| users.ndjson | json | 25.569 | 26.314 | 26.735 | 68.094 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.412 | 2.486 | 2.536 | 68.535 | 1.00x |
| users.json | orjson | 3.114 | 3.155 | 3.256 | 68.535 | 0.79x |
| users.json | msgspec | 3.833 | 3.906 | 3.991 | 68.535 | 0.64x |
| users.json | ujson | 11.230 | 11.327 | 11.479 | 68.535 | 0.22x |
| users.json | json | 19.684 | 19.831 | 19.926 | 68.535 | 0.13x |
| flat.json | strata | 0.379 | 0.410 | 0.445 | 68.102 | 1.00x |
| flat.json | orjson | 0.483 | 0.509 | 0.538 | 68.102 | 0.80x |
| flat.json | msgspec | 0.572 | 0.605 | 0.641 | 68.102 | 0.68x |
| flat.json | ujson | 1.181 | 1.225 | 1.250 | 68.102 | 0.33x |
| flat.json | json | 1.921 | 1.940 | 1.962 | 68.102 | 0.21x |
| nested.json | strata | 0.343 | 0.370 | 0.402 | 68.105 | 1.00x |
| nested.json | orjson | 0.456 | 0.477 | 0.502 | 68.105 | 0.78x |
| nested.json | msgspec | 0.530 | 0.566 | 0.608 | 68.105 | 0.65x |
| nested.json | ujson | 1.268 | 1.305 | 1.349 | 68.105 | 0.28x |
| nested.json | json | 2.337 | 2.369 | 2.406 | 68.105 | 0.16x |
| wide_arrays.json | strata | 1.755 | 1.840 | 1.910 | 69.676 | 1.00x |
| wide_arrays.json | orjson | 2.033 | 2.129 | 2.199 | 69.676 | 0.86x |
| wide_arrays.json | msgspec | 2.766 | 2.858 | 2.919 | 69.676 | 0.64x |
| wide_arrays.json | ujson | 5.277 | 5.317 | 5.440 | 69.676 | 0.35x |
| wide_arrays.json | json | 14.075 | 14.275 | 14.382 | 69.676 | 0.13x |
| mixed.json | strata | 0.163 | 0.182 | 0.202 | 69.676 | 1.00x |
| mixed.json | orjson | 0.183 | 0.203 | 0.221 | 69.676 | 0.90x |
| mixed.json | msgspec | 0.194 | 0.214 | 0.240 | 69.676 | 0.85x |
| mixed.json | ujson | 0.374 | 0.408 | 0.434 | 69.676 | 0.45x |
| mixed.json | json | 0.611 | 0.647 | 0.677 | 69.676 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.106 | 0.110 | 0.133 | 68.535 | 1.00x |
| users.json $[*].id | jmespath | 0.482 | 0.485 | 0.504 | 68.535 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.472 | 2.571 | 2.654 | 68.535 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.619 | 0.641 | 0.664 | 68.660 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.979 | 3.016 | 3.035 | 68.660 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.552 | 18.193 | 18.819 | 68.660 | 0.04x |
| users.json $..total | strata | 1.708 | 1.724 | 1.742 | 69.668 | 1.00x |
| users.json $..total | jsonpath-ng | 293.029 | 293.551 | 294.054 | 69.668 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.246 | 3.254 | 3.276 | 68.660 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.005 | 13.199 | 13.472 | 68.660 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.872 | 15.039 | 15.245 | 68.660 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.418 | 3.442 | 3.464 | 69.668 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.698 | 15.886 | 16.318 | 69.668 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 33.825 | 34.400 | 34.649 | 69.668 | 0.10x |
| users.json $..total | strata | 11.836 | 12.089 | 12.705 | 69.730 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 314.284 | 315.109 | 315.796 | 69.730 | 0.04x |

