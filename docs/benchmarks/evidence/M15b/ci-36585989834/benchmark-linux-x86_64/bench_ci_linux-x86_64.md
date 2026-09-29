# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 565fab210bb851772fcefe65082041084aeafc14
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 9V74 80-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.879 | 10.023 | 14.452 | 66.102 | 1.00x |
| users.json | orjson | 13.894 | 14.121 | 18.057 | 66.102 | 0.71x |
| users.json | msgspec | 14.020 | 14.107 | 17.876 | 66.102 | 0.71x |
| users.json | ujson | 17.793 | 18.293 | 28.948 | 66.102 | 0.55x |
| users.json | pysimdjson | 18.628 | 19.127 | 24.846 | 66.102 | 0.52x |
| users.json | json | 20.939 | 21.406 | 23.363 | 66.102 | 0.47x |
| flat.json | strata | 0.843 | 0.855 | 0.865 | 65.926 | 1.00x |
| flat.json | orjson | 1.017 | 1.035 | 1.058 | 65.926 | 0.83x |
| flat.json | msgspec | 1.053 | 1.061 | 1.200 | 65.926 | 0.81x |
| flat.json | ujson | 1.504 | 1.541 | 1.610 | 65.926 | 0.55x |
| flat.json | pysimdjson | 1.610 | 1.638 | 1.677 | 65.926 | 0.52x |
| flat.json | json | 1.708 | 1.741 | 1.751 | 65.926 | 0.49x |
| nested.json | strata | 0.781 | 0.801 | 0.810 | 65.957 | 1.00x |
| nested.json | orjson | 0.998 | 1.009 | 1.259 | 65.957 | 0.79x |
| nested.json | msgspec | 0.977 | 0.999 | 1.083 | 65.957 | 0.80x |
| nested.json | ujson | 1.406 | 1.430 | 1.460 | 65.957 | 0.56x |
| nested.json | pysimdjson | 1.395 | 1.407 | 1.441 | 65.957 | 0.57x |
| nested.json | json | 1.816 | 1.844 | 1.855 | 65.957 | 0.43x |
| wide_arrays.json | strata | 4.465 | 4.481 | 4.791 | 79.641 | 1.00x |
| wide_arrays.json | orjson | 5.563 | 5.633 | 5.739 | 79.641 | 0.80x |
| wide_arrays.json | msgspec | 6.158 | 6.201 | 6.324 | 79.641 | 0.72x |
| wide_arrays.json | ujson | 7.613 | 7.650 | 7.753 | 79.641 | 0.59x |
| wide_arrays.json | pysimdjson | 6.443 | 6.496 | 6.656 | 79.641 | 0.69x |
| wide_arrays.json | json | 9.796 | 9.971 | 10.028 | 79.641 | 0.45x |
| mixed.json | strata | 0.187 | 0.195 | 0.206 | 79.641 | 1.00x |
| mixed.json | orjson | 0.234 | 0.238 | 0.251 | 79.641 | 0.82x |
| mixed.json | msgspec | 0.249 | 0.253 | 0.269 | 79.641 | 0.77x |
| mixed.json | ujson | 0.307 | 0.310 | 0.350 | 79.641 | 0.63x |
| mixed.json | pysimdjson | 0.304 | 0.311 | 0.321 | 79.641 | 0.63x |
| mixed.json | json | 0.448 | 0.463 | 0.473 | 79.641 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.334 | 2.346 | 2.371 | 47.352 | 1.00x |
| users.json | orjson | 3.091 | 3.101 | 3.110 | 47.352 | 0.76x |
| users.json | msgspec | 4.158 | 4.180 | 4.228 | 47.352 | 0.56x |
| users.json | ujson | 11.306 | 11.381 | 11.661 | 47.352 | 0.21x |
| users.json | json | 21.343 | 21.557 | 22.529 | 47.352 | 0.11x |
| flat.json | strata | 0.308 | 0.314 | 0.329 | 65.957 | 1.00x |
| flat.json | orjson | 0.362 | 0.369 | 0.378 | 65.957 | 0.85x |
| flat.json | msgspec | 0.475 | 0.481 | 0.495 | 65.957 | 0.65x |
| flat.json | ujson | 1.038 | 1.047 | 1.056 | 65.957 | 0.30x |
| flat.json | json | 1.847 | 1.874 | 1.899 | 65.957 | 0.17x |
| nested.json | strata | 0.231 | 0.237 | 0.256 | 65.957 | 1.00x |
| nested.json | orjson | 0.307 | 0.321 | 0.342 | 65.957 | 0.74x |
| nested.json | msgspec | 0.415 | 0.419 | 0.432 | 65.957 | 0.56x |
| nested.json | ujson | 1.063 | 1.072 | 1.246 | 65.957 | 0.22x |
| nested.json | json | 2.298 | 2.330 | 2.416 | 65.957 | 0.10x |
| wide_arrays.json | strata | 1.765 | 1.772 | 1.786 | 79.641 | 1.00x |
| wide_arrays.json | orjson | 1.935 | 1.952 | 1.991 | 79.641 | 0.91x |
| wide_arrays.json | msgspec | 3.080 | 3.097 | 3.119 | 79.641 | 0.57x |
| wide_arrays.json | ujson | 6.358 | 6.380 | 6.430 | 79.641 | 0.28x |
| wide_arrays.json | json | 16.682 | 16.749 | 16.904 | 79.641 | 0.11x |
| mixed.json | strata | 0.062 | 0.065 | 0.076 | 79.641 | 1.00x |
| mixed.json | orjson | 0.069 | 0.071 | 0.074 | 79.641 | 0.91x |
| mixed.json | msgspec | 0.086 | 0.087 | 0.103 | 79.641 | 0.75x |
| mixed.json | ujson | 0.228 | 0.230 | 0.264 | 79.641 | 0.28x |
| mixed.json | json | 0.509 | 0.519 | 0.526 | 79.641 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.368 | 10.730 | 12.374 | 67.562 | 1.00x |
| users.json | orjson | 14.464 | 14.787 | 15.699 | 67.562 | 0.73x |
| users.json | msgspec | 14.485 | 14.828 | 15.299 | 67.562 | 0.72x |
| users.json | ujson | 18.863 | 19.650 | 21.731 | 67.562 | 0.55x |
| users.json | json | 21.771 | 22.156 | 22.345 | 67.562 | 0.48x |
| flat.json | strata | 0.890 | 0.907 | 0.953 | 65.957 | 1.00x |
| flat.json | orjson | 1.090 | 1.102 | 1.113 | 65.957 | 0.82x |
| flat.json | msgspec | 1.118 | 1.135 | 1.181 | 65.957 | 0.80x |
| flat.json | ujson | 1.596 | 1.632 | 1.658 | 65.957 | 0.56x |
| flat.json | json | 1.784 | 1.801 | 1.831 | 65.957 | 0.50x |
| nested.json | strata | 0.816 | 0.838 | 0.852 | 65.957 | 1.00x |
| nested.json | orjson | 1.056 | 1.071 | 1.091 | 65.957 | 0.78x |
| nested.json | msgspec | 1.043 | 1.051 | 1.094 | 65.957 | 0.80x |
| nested.json | ujson | 1.476 | 1.500 | 1.537 | 65.957 | 0.56x |
| nested.json | json | 1.873 | 1.899 | 1.909 | 65.957 | 0.44x |
| wide_arrays.json | strata | 4.469 | 4.528 | 4.610 | 79.641 | 1.00x |
| wide_arrays.json | orjson | 5.641 | 5.712 | 5.800 | 79.641 | 0.79x |
| wide_arrays.json | msgspec | 6.254 | 6.355 | 6.455 | 79.641 | 0.71x |
| wide_arrays.json | ujson | 7.799 | 7.865 | 8.003 | 79.641 | 0.58x |
| wide_arrays.json | json | 9.859 | 9.952 | 10.025 | 79.641 | 0.46x |
| mixed.json | strata | 0.205 | 0.211 | 0.220 | 79.641 | 1.00x |
| mixed.json | orjson | 0.280 | 0.291 | 0.299 | 79.641 | 0.72x |
| mixed.json | msgspec | 0.292 | 0.302 | 0.313 | 79.641 | 0.70x |
| mixed.json | ujson | 0.360 | 0.368 | 0.390 | 79.641 | 0.57x |
| mixed.json | json | 0.499 | 0.507 | 0.524 | 79.641 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.863 | 10.916 | 11.109 | 65.926 | 1.00x |
| users.ndjson | orjson | 18.229 | 18.402 | 18.746 | 65.926 | 0.59x |
| users.ndjson | msgspec | 18.318 | 18.478 | 18.821 | 65.926 | 0.59x |
| users.ndjson | ujson | 22.760 | 23.160 | 24.121 | 65.926 | 0.47x |
| users.ndjson | json | 29.115 | 29.480 | 29.699 | 65.926 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.019 | 3.057 | 3.097 | 67.562 | 1.00x |
| users.json | orjson | 3.817 | 3.853 | 4.033 | 67.562 | 0.79x |
| users.json | msgspec | 4.839 | 4.875 | 4.917 | 67.562 | 0.63x |
| users.json | ujson | 12.139 | 12.222 | 12.313 | 67.562 | 0.25x |
| users.json | json | 22.105 | 22.305 | 22.630 | 67.562 | 0.14x |
| flat.json | strata | 0.510 | 0.520 | 0.557 | 65.957 | 1.00x |
| flat.json | orjson | 0.583 | 0.600 | 0.626 | 65.957 | 0.87x |
| flat.json | msgspec | 0.694 | 0.717 | 0.747 | 65.957 | 0.72x |
| flat.json | ujson | 1.282 | 1.291 | 1.337 | 65.957 | 0.40x |
| flat.json | json | 2.099 | 2.146 | 2.194 | 65.957 | 0.24x |
| nested.json | strata | 0.407 | 0.418 | 0.441 | 65.957 | 1.00x |
| nested.json | orjson | 0.506 | 0.524 | 0.543 | 65.957 | 0.80x |
| nested.json | msgspec | 0.618 | 0.636 | 0.662 | 65.957 | 0.66x |
| nested.json | ujson | 1.272 | 1.295 | 1.308 | 65.957 | 0.32x |
| nested.json | json | 2.531 | 2.562 | 2.578 | 65.957 | 0.16x |
| wide_arrays.json | strata | 2.264 | 2.297 | 2.386 | 79.641 | 1.00x |
| wide_arrays.json | orjson | 2.421 | 2.488 | 2.527 | 79.641 | 0.92x |
| wide_arrays.json | msgspec | 3.579 | 3.655 | 3.674 | 79.641 | 0.63x |
| wide_arrays.json | ujson | 6.952 | 6.980 | 7.038 | 79.641 | 0.33x |
| wide_arrays.json | json | 17.311 | 17.456 | 18.242 | 79.641 | 0.13x |
| mixed.json | strata | 0.203 | 0.225 | 0.244 | 79.641 | 1.00x |
| mixed.json | orjson | 0.234 | 0.255 | 0.291 | 79.641 | 0.88x |
| mixed.json | msgspec | 0.253 | 0.271 | 0.292 | 79.641 | 0.83x |
| mixed.json | ujson | 0.412 | 0.421 | 0.442 | 79.641 | 0.53x |
| mixed.json | json | 0.704 | 0.724 | 0.745 | 79.641 | 0.31x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.072 | 0.075 | 0.085 | 67.562 | 1.00x |
| users.json $[*].id | jmespath | 0.474 | 0.478 | 0.497 | 67.562 | 0.16x |
| users.json $[*].id | jsonpath-ng | 2.733 | 2.814 | 2.911 | 67.562 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.426 | 0.433 | 0.443 | 67.562 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.008 | 3.044 | 3.079 | 67.562 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.428 | 19.871 | 19.985 | 67.562 | 0.02x |
| users.json $..total | strata | 1.808 | 1.845 | 1.885 | 67.562 | 1.00x |
| users.json $..total | jsonpath-ng | 387.472 | 388.416 | 390.424 | 67.562 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.252 | 3.433 | 3.542 | 67.562 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.592 | 15.751 | 16.046 | 67.562 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 17.930 | 18.134 | 18.485 | 67.562 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.474 | 3.670 | 3.809 | 67.562 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.822 | 19.010 | 21.346 | 67.562 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 39.224 | 39.779 | 40.770 | 67.562 | 0.09x |
| users.json $..total | strata | 13.533 | 14.356 | 16.119 | 67.562 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 410.667 | 414.270 | 417.941 | 67.562 | 0.03x |

