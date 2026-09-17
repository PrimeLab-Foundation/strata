# Benchmark results - ci-linux-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6e4d93e74610755e4c46406e91c8aaf4ab6817a1
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
| users.json | strata | 8.755 | 8.808 | 10.556 | 57.238 | 1.00x |
| users.json | orjson | 11.761 | 11.810 | 13.320 | 57.238 | 0.75x |
| users.json | msgspec | 12.264 | 12.329 | 13.767 | 57.238 | 0.71x |
| users.json | ujson | 16.403 | 16.479 | 18.966 | 57.238 | 0.53x |
| users.json | pysimdjson | 16.418 | 16.542 | 18.413 | 57.238 | 0.53x |
| users.json | json | 20.626 | 20.705 | 21.394 | 57.238 | 0.43x |
| flat.json | strata | 0.785 | 0.798 | 0.811 | 67.996 | 1.00x |
| flat.json | orjson | 0.844 | 0.860 | 0.871 | 67.996 | 0.93x |
| flat.json | msgspec | 0.883 | 0.901 | 0.911 | 67.996 | 0.89x |
| flat.json | ujson | 1.384 | 1.399 | 1.410 | 67.996 | 0.57x |
| flat.json | pysimdjson | 1.422 | 1.436 | 1.475 | 67.996 | 0.56x |
| flat.json | json | 1.717 | 1.730 | 1.750 | 67.996 | 0.46x |
| nested.json | strata | 0.812 | 0.828 | 0.838 | 67.996 | 1.00x |
| nested.json | orjson | 0.893 | 0.904 | 0.967 | 67.996 | 0.92x |
| nested.json | msgspec | 1.015 | 1.021 | 1.027 | 67.996 | 0.81x |
| nested.json | ujson | 1.407 | 1.414 | 1.446 | 67.996 | 0.59x |
| nested.json | pysimdjson | 1.414 | 1.428 | 1.450 | 67.996 | 0.58x |
| nested.json | json | 1.991 | 2.000 | 2.020 | 67.996 | 0.41x |
| wide_arrays.json | strata | 3.889 | 3.917 | 3.948 | 69.566 | 1.00x |
| wide_arrays.json | orjson | 4.099 | 4.156 | 4.242 | 69.566 | 0.94x |
| wide_arrays.json | msgspec | 5.080 | 5.156 | 5.202 | 69.566 | 0.76x |
| wide_arrays.json | ujson | 6.542 | 6.625 | 6.645 | 69.566 | 0.59x |
| wide_arrays.json | pysimdjson | 5.340 | 5.384 | 5.479 | 69.566 | 0.73x |
| wide_arrays.json | json | 9.577 | 9.623 | 9.687 | 69.566 | 0.41x |
| mixed.json | strata | 0.191 | 0.200 | 0.220 | 69.566 | 1.00x |
| mixed.json | orjson | 0.222 | 0.225 | 0.248 | 69.566 | 0.89x |
| mixed.json | msgspec | 0.235 | 0.243 | 0.269 | 69.566 | 0.82x |
| mixed.json | ujson | 0.306 | 0.314 | 0.333 | 69.566 | 0.64x |
| mixed.json | pysimdjson | 0.297 | 0.305 | 0.329 | 69.566 | 0.66x |
| mixed.json | json | 0.453 | 0.473 | 0.484 | 69.566 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.922 | 1.933 | 1.941 | 56.348 | 1.00x |
| users.json | orjson | 2.582 | 2.591 | 2.606 | 56.348 | 0.75x |
| users.json | msgspec | 3.319 | 3.325 | 3.340 | 56.348 | 0.58x |
| users.json | ujson | 10.477 | 10.522 | 10.548 | 56.348 | 0.18x |
| users.json | json | 18.902 | 18.956 | 18.980 | 56.348 | 0.10x |
| flat.json | strata | 0.236 | 0.238 | 0.256 | 67.996 | 1.00x |
| flat.json | orjson | 0.306 | 0.309 | 0.356 | 67.996 | 0.77x |
| flat.json | msgspec | 0.392 | 0.397 | 0.423 | 67.996 | 0.60x |
| flat.json | ujson | 1.000 | 1.013 | 1.018 | 67.996 | 0.23x |
| flat.json | json | 1.716 | 1.727 | 1.736 | 67.996 | 0.14x |
| nested.json | strata | 0.219 | 0.221 | 0.234 | 67.996 | 1.00x |
| nested.json | orjson | 0.281 | 0.286 | 0.306 | 67.996 | 0.77x |
| nested.json | msgspec | 0.368 | 0.372 | 0.394 | 67.996 | 0.60x |
| nested.json | ujson | 1.088 | 1.096 | 1.120 | 67.996 | 0.20x |
| nested.json | json | 2.145 | 2.184 | 2.197 | 67.996 | 0.10x |
| wide_arrays.json | strata | 1.342 | 1.356 | 1.363 | 69.566 | 1.00x |
| wide_arrays.json | orjson | 1.598 | 1.620 | 1.638 | 69.566 | 0.84x |
| wide_arrays.json | msgspec | 2.375 | 2.386 | 2.407 | 69.566 | 0.57x |
| wide_arrays.json | ujson | 4.773 | 4.791 | 4.816 | 69.566 | 0.28x |
| wide_arrays.json | json | 13.597 | 13.616 | 13.678 | 69.566 | 0.10x |
| mixed.json | strata | 0.064 | 0.065 | 0.081 | 69.566 | 1.00x |
| mixed.json | orjson | 0.065 | 0.069 | 0.083 | 69.566 | 0.95x |
| mixed.json | msgspec | 0.078 | 0.081 | 0.092 | 69.566 | 0.81x |
| mixed.json | ujson | 0.237 | 0.241 | 0.266 | 69.566 | 0.27x |
| mixed.json | json | 0.477 | 0.496 | 0.507 | 69.566 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.905 | 8.955 | 9.695 | 68.430 | 1.00x |
| users.json | orjson | 11.892 | 11.947 | 12.414 | 68.430 | 0.75x |
| users.json | msgspec | 12.423 | 12.519 | 12.828 | 68.430 | 0.72x |
| users.json | ujson | 16.858 | 17.107 | 18.357 | 68.430 | 0.52x |
| users.json | json | 20.849 | 20.974 | 21.137 | 68.430 | 0.43x |
| flat.json | strata | 0.829 | 0.841 | 0.860 | 67.996 | 1.00x |
| flat.json | orjson | 0.936 | 0.943 | 0.956 | 67.996 | 0.89x |
| flat.json | msgspec | 0.956 | 0.979 | 0.999 | 67.996 | 0.86x |
| flat.json | ujson | 1.499 | 1.521 | 1.532 | 67.996 | 0.55x |
| flat.json | json | 1.789 | 1.811 | 1.818 | 67.996 | 0.46x |
| nested.json | strata | 0.847 | 0.869 | 0.875 | 67.996 | 1.00x |
| nested.json | orjson | 0.943 | 0.965 | 0.973 | 67.996 | 0.90x |
| nested.json | msgspec | 1.061 | 1.072 | 1.116 | 67.996 | 0.81x |
| nested.json | ujson | 1.478 | 1.498 | 1.520 | 67.996 | 0.58x |
| nested.json | json | 2.047 | 2.058 | 2.074 | 67.996 | 0.42x |
| wide_arrays.json | strata | 3.857 | 3.875 | 3.908 | 69.566 | 1.00x |
| wide_arrays.json | orjson | 4.101 | 4.136 | 4.187 | 69.566 | 0.94x |
| wide_arrays.json | msgspec | 5.124 | 5.144 | 5.159 | 69.566 | 0.75x |
| wide_arrays.json | ujson | 6.699 | 6.722 | 6.759 | 69.566 | 0.58x |
| wide_arrays.json | json | 9.565 | 9.617 | 9.685 | 69.566 | 0.40x |
| mixed.json | strata | 0.217 | 0.224 | 0.252 | 69.566 | 1.00x |
| mixed.json | orjson | 0.286 | 0.294 | 0.311 | 69.566 | 0.76x |
| mixed.json | msgspec | 0.301 | 0.316 | 0.347 | 69.566 | 0.71x |
| mixed.json | ujson | 0.389 | 0.407 | 0.438 | 69.566 | 0.55x |
| mixed.json | json | 0.519 | 0.532 | 0.560 | 69.566 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.256 | 9.337 | 9.419 | 67.988 | 1.00x |
| users.ndjson | orjson | 14.738 | 14.836 | 15.030 | 67.988 | 0.63x |
| users.ndjson | msgspec | 15.131 | 15.213 | 15.392 | 67.988 | 0.61x |
| users.ndjson | ujson | 19.642 | 19.785 | 19.996 | 67.988 | 0.47x |
| users.ndjson | json | 25.677 | 25.831 | 26.144 | 67.988 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.352 | 2.402 | 2.479 | 68.430 | 1.00x |
| users.json | orjson | 3.098 | 3.132 | 3.171 | 68.430 | 0.77x |
| users.json | msgspec | 3.826 | 3.869 | 3.944 | 68.430 | 0.62x |
| users.json | ujson | 11.172 | 11.248 | 11.322 | 68.430 | 0.21x |
| users.json | json | 19.626 | 19.693 | 19.803 | 68.430 | 0.12x |
| flat.json | strata | 0.395 | 0.423 | 0.447 | 67.996 | 1.00x |
| flat.json | orjson | 0.492 | 0.522 | 0.553 | 67.996 | 0.81x |
| flat.json | msgspec | 0.578 | 0.627 | 0.659 | 67.996 | 0.68x |
| flat.json | ujson | 1.227 | 1.252 | 1.283 | 67.996 | 0.34x |
| flat.json | json | 1.945 | 1.969 | 2.001 | 67.996 | 0.21x |
| nested.json | strata | 0.359 | 0.366 | 0.393 | 67.996 | 1.00x |
| nested.json | orjson | 0.463 | 0.476 | 0.490 | 67.996 | 0.77x |
| nested.json | msgspec | 0.552 | 0.567 | 0.580 | 67.996 | 0.64x |
| nested.json | ujson | 1.292 | 1.301 | 1.322 | 67.996 | 0.28x |
| nested.json | json | 2.353 | 2.387 | 2.402 | 67.996 | 0.15x |
| wide_arrays.json | strata | 1.695 | 1.723 | 1.744 | 69.566 | 1.00x |
| wide_arrays.json | orjson | 2.034 | 2.057 | 2.092 | 69.566 | 0.84x |
| wide_arrays.json | msgspec | 2.802 | 2.814 | 2.833 | 69.566 | 0.61x |
| wide_arrays.json | ujson | 5.217 | 5.263 | 5.327 | 69.566 | 0.33x |
| wide_arrays.json | json | 14.037 | 14.081 | 14.115 | 69.566 | 0.12x |
| mixed.json | strata | 0.181 | 0.186 | 0.197 | 69.566 | 1.00x |
| mixed.json | orjson | 0.206 | 0.209 | 0.238 | 69.566 | 0.89x |
| mixed.json | msgspec | 0.219 | 0.224 | 0.258 | 69.566 | 0.83x |
| mixed.json | ujson | 0.396 | 0.413 | 0.442 | 69.566 | 0.45x |
| mixed.json | json | 0.638 | 0.666 | 0.696 | 69.566 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.106 | 0.108 | 0.125 | 68.430 | 1.00x |
| users.json $[*].id | jmespath | 0.477 | 0.485 | 0.506 | 68.430 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.485 | 2.520 | 2.566 | 68.430 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.665 | 0.669 | 0.685 | 68.555 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.025 | 3.057 | 3.080 | 68.555 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.562 | 17.802 | 18.043 | 68.555 | 0.04x |
| users.json $..total | strata | 1.715 | 1.735 | 1.756 | 69.562 | 1.00x |
| users.json $..total | jsonpath-ng | 293.451 | 294.511 | 295.274 | 69.562 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.153 | 3.170 | 3.197 | 68.555 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.545 | 12.652 | 12.842 | 68.555 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 14.526 | 14.584 | 14.736 | 68.555 | 0.22x |
| users.json $[*].orders[*].total | strata | 3.356 | 3.383 | 3.401 | 69.562 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.316 | 15.556 | 15.827 | 69.562 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.281 | 34.528 | 35.134 | 69.562 | 0.10x |
| users.json $..total | strata | 11.220 | 11.630 | 11.884 | 69.625 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 310.208 | 311.924 | 314.398 | 69.625 | 0.04x |

