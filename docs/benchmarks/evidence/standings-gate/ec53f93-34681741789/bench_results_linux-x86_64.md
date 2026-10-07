# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec53f936b8a9245edf5bfeec36c38539666a1775
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.457 | 11.787 | 15.175 | 65.980 | 1.00x |
| users.json | orjson | 15.716 | 16.220 | 20.724 | 65.980 | 0.73x |
| users.json | msgspec | 15.107 | 15.480 | 18.515 | 65.980 | 0.76x |
| users.json | ujson | 20.540 | 21.420 | 26.183 | 65.980 | 0.55x |
| users.json | pysimdjson | 21.078 | 21.721 | 23.784 | 65.980 | 0.54x |
| users.json | json | 23.977 | 24.281 | 25.123 | 65.980 | 0.49x |
| flat.json | strata | 0.972 | 0.998 | 1.088 | 63.535 | 1.00x |
| flat.json | orjson | 1.137 | 1.177 | 1.231 | 63.535 | 0.85x |
| flat.json | msgspec | 1.101 | 1.126 | 1.185 | 63.535 | 0.89x |
| flat.json | ujson | 1.616 | 1.695 | 1.932 | 63.535 | 0.59x |
| flat.json | pysimdjson | 1.834 | 1.892 | 1.948 | 63.535 | 0.53x |
| flat.json | json | 1.879 | 1.912 | 1.989 | 63.535 | 0.52x |
| nested.json | strata | 0.793 | 0.834 | 0.866 | 63.535 | 1.00x |
| nested.json | orjson | 1.023 | 1.034 | 1.079 | 63.535 | 0.81x |
| nested.json | msgspec | 1.036 | 1.068 | 1.168 | 63.535 | 0.78x |
| nested.json | ujson | 1.515 | 1.527 | 1.669 | 63.535 | 0.55x |
| nested.json | pysimdjson | 1.507 | 1.542 | 1.687 | 63.535 | 0.54x |
| nested.json | json | 2.028 | 2.056 | 2.081 | 63.535 | 0.41x |
| wide_arrays.json | strata | 4.146 | 4.440 | 4.814 | 79.012 | 1.00x |
| wide_arrays.json | orjson | 5.376 | 5.625 | 6.154 | 79.012 | 0.79x |
| wide_arrays.json | msgspec | 5.744 | 5.986 | 6.501 | 79.012 | 0.74x |
| wide_arrays.json | ujson | 7.201 | 7.577 | 8.068 | 79.012 | 0.59x |
| wide_arrays.json | pysimdjson | 6.048 | 6.237 | 6.801 | 79.012 | 0.71x |
| wide_arrays.json | json | 10.137 | 10.401 | 10.563 | 79.012 | 0.43x |
| mixed.json | strata | 0.248 | 0.261 | 0.280 | 79.074 | 1.00x |
| mixed.json | orjson | 0.279 | 0.288 | 0.337 | 79.074 | 0.91x |
| mixed.json | msgspec | 0.300 | 0.312 | 0.330 | 79.074 | 0.83x |
| mixed.json | ujson | 0.351 | 0.371 | 0.387 | 79.074 | 0.70x |
| mixed.json | pysimdjson | 0.382 | 0.394 | 0.411 | 79.074 | 0.66x |
| mixed.json | json | 0.516 | 0.537 | 0.566 | 79.074 | 0.49x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.205 | 2.323 | 2.472 | 47.191 | 1.00x |
| users.json | orjson | 2.420 | 2.507 | 2.712 | 47.191 | 0.93x |
| users.json | msgspec | 3.939 | 4.031 | 4.145 | 47.191 | 0.58x |
| users.json | ujson | 11.708 | 11.826 | 12.168 | 47.191 | 0.20x |
| users.json | json | 22.539 | 22.715 | 22.924 | 47.191 | 0.10x |
| flat.json | strata | 0.275 | 0.290 | 0.322 | 63.535 | 1.00x |
| flat.json | orjson | 0.349 | 0.390 | 0.630 | 63.535 | 0.74x |
| flat.json | msgspec | 0.463 | 0.493 | 0.516 | 63.535 | 0.59x |
| flat.json | ujson | 1.045 | 1.052 | 1.080 | 63.535 | 0.28x |
| flat.json | json | 1.988 | 2.016 | 2.030 | 63.535 | 0.14x |
| nested.json | strata | 0.206 | 0.211 | 0.235 | 63.535 | 1.00x |
| nested.json | orjson | 0.307 | 0.333 | 0.455 | 63.535 | 0.63x |
| nested.json | msgspec | 0.414 | 0.418 | 0.436 | 63.535 | 0.50x |
| nested.json | ujson | 1.070 | 1.078 | 1.094 | 63.535 | 0.20x |
| nested.json | json | 2.512 | 2.522 | 2.531 | 63.535 | 0.08x |
| wide_arrays.json | strata | 1.632 | 1.817 | 2.128 | 79.012 | 1.00x |
| wide_arrays.json | orjson | 1.986 | 2.114 | 2.304 | 79.012 | 0.86x |
| wide_arrays.json | msgspec | 2.913 | 3.020 | 3.287 | 79.012 | 0.60x |
| wide_arrays.json | ujson | 6.474 | 6.753 | 7.127 | 79.012 | 0.27x |
| wide_arrays.json | json | 16.304 | 16.421 | 16.640 | 79.012 | 0.11x |
| mixed.json | strata | 0.089 | 0.092 | 0.100 | 79.074 | 1.00x |
| mixed.json | orjson | 0.110 | 0.122 | 0.152 | 79.074 | 0.75x |
| mixed.json | msgspec | 0.090 | 0.096 | 0.103 | 79.074 | 0.96x |
| mixed.json | ujson | 0.242 | 0.246 | 0.260 | 79.074 | 0.37x |
| mixed.json | json | 0.563 | 0.578 | 0.593 | 79.074 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.042 | 12.271 | 13.546 | 64.699 | 1.00x |
| users.json | orjson | 16.176 | 16.676 | 19.259 | 64.699 | 0.74x |
| users.json | msgspec | 15.597 | 16.016 | 17.296 | 64.699 | 0.77x |
| users.json | ujson | 21.587 | 22.539 | 25.397 | 64.699 | 0.54x |
| users.json | json | 24.504 | 25.057 | 27.507 | 64.699 | 0.49x |
| flat.json | strata | 1.000 | 1.032 | 1.075 | 63.535 | 1.00x |
| flat.json | orjson | 1.190 | 1.274 | 1.451 | 63.535 | 0.81x |
| flat.json | msgspec | 1.144 | 1.171 | 1.220 | 63.535 | 0.88x |
| flat.json | ujson | 1.714 | 1.737 | 1.795 | 63.535 | 0.59x |
| flat.json | json | 1.936 | 1.989 | 2.048 | 63.535 | 0.52x |
| nested.json | strata | 0.852 | 0.874 | 0.890 | 63.535 | 1.00x |
| nested.json | orjson | 1.079 | 1.126 | 1.164 | 63.535 | 0.78x |
| nested.json | msgspec | 1.068 | 1.103 | 1.131 | 63.535 | 0.79x |
| nested.json | ujson | 1.569 | 1.608 | 1.758 | 63.535 | 0.54x |
| nested.json | json | 2.059 | 2.076 | 2.221 | 63.535 | 0.42x |
| wide_arrays.json | strata | 4.660 | 4.822 | 5.370 | 79.074 | 1.00x |
| wide_arrays.json | orjson | 5.584 | 5.917 | 6.947 | 79.074 | 0.81x |
| wide_arrays.json | msgspec | 6.277 | 6.608 | 7.005 | 79.074 | 0.73x |
| wide_arrays.json | ujson | 8.077 | 8.353 | 9.386 | 79.074 | 0.58x |
| wide_arrays.json | json | 10.378 | 10.596 | 11.118 | 79.074 | 0.46x |
| mixed.json | strata | 0.274 | 0.289 | 0.321 | 79.074 | 1.00x |
| mixed.json | orjson | 0.357 | 0.379 | 0.412 | 79.074 | 0.76x |
| mixed.json | msgspec | 0.352 | 0.376 | 0.382 | 79.074 | 0.77x |
| mixed.json | ujson | 0.448 | 0.463 | 0.485 | 79.074 | 0.62x |
| mixed.json | json | 0.576 | 0.596 | 0.615 | 79.074 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 12.544 | 12.671 | 23.070 | 63.535 | 1.00x |
| users.ndjson | orjson | 19.086 | 19.489 | 36.080 | 63.535 | 0.65x |
| users.ndjson | msgspec | 18.337 | 19.076 | 21.537 | 63.535 | 0.66x |
| users.ndjson | ujson | 24.027 | 24.464 | 25.244 | 63.535 | 0.52x |
| users.ndjson | json | 30.510 | 31.101 | 32.269 | 63.535 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.811 | 3.002 | 3.329 | 64.699 | 1.00x |
| users.json | orjson | 3.028 | 3.190 | 3.497 | 64.699 | 0.94x |
| users.json | msgspec | 4.573 | 4.850 | 5.081 | 64.699 | 0.62x |
| users.json | ujson | 12.487 | 12.622 | 13.028 | 64.699 | 0.24x |
| users.json | json | 23.575 | 23.740 | 24.234 | 64.699 | 0.13x |
| flat.json | strata | 0.442 | 0.463 | 0.484 | 63.535 | 1.00x |
| flat.json | orjson | 0.491 | 0.512 | 0.563 | 63.535 | 0.90x |
| flat.json | msgspec | 0.645 | 0.682 | 0.709 | 63.535 | 0.68x |
| flat.json | ujson | 1.218 | 1.250 | 1.313 | 63.535 | 0.37x |
| flat.json | json | 2.185 | 2.216 | 2.296 | 63.535 | 0.21x |
| nested.json | strata | 0.340 | 0.358 | 0.411 | 63.754 | 1.00x |
| nested.json | orjson | 0.442 | 0.472 | 0.549 | 63.754 | 0.76x |
| nested.json | msgspec | 0.568 | 0.581 | 0.635 | 63.754 | 0.62x |
| nested.json | ujson | 1.223 | 1.255 | 1.268 | 63.754 | 0.29x |
| nested.json | json | 2.661 | 2.689 | 2.729 | 63.754 | 0.13x |
| wide_arrays.json | strata | 2.243 | 2.344 | 2.544 | 79.074 | 1.00x |
| wide_arrays.json | orjson | 2.543 | 2.664 | 2.819 | 79.074 | 0.88x |
| wide_arrays.json | msgspec | 3.521 | 3.614 | 3.807 | 79.074 | 0.65x |
| wide_arrays.json | ujson | 7.237 | 7.346 | 7.681 | 79.074 | 0.32x |
| wide_arrays.json | json | 16.978 | 17.160 | 17.682 | 79.074 | 0.14x |
| mixed.json | strata | 0.194 | 0.207 | 0.237 | 79.074 | 1.00x |
| mixed.json | orjson | 0.221 | 0.257 | 0.282 | 79.074 | 0.81x |
| mixed.json | msgspec | 0.231 | 0.238 | 0.255 | 79.074 | 0.87x |
| mixed.json | ujson | 0.386 | 0.403 | 0.424 | 79.074 | 0.51x |
| mixed.json | json | 0.692 | 0.732 | 0.763 | 79.074 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.120 | 0.128 | 0.134 | 64.699 | 1.00x |
| users.json $[*].id | jmespath | 0.525 | 0.549 | 0.582 | 64.699 | 0.23x |
| users.json $[*].id | jsonpath-ng | 2.621 | 2.723 | 2.782 | 64.699 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.500 | 0.539 | 0.794 | 64.711 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.189 | 3.252 | 3.439 | 64.711 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.015 | 19.710 | 20.285 | 64.711 | 0.03x |
| users.json $..total | strata | 1.812 | 1.897 | 2.031 | 65.168 | 1.00x |
| users.json $..total | jsonpath-ng | 348.166 | 349.218 | 351.528 | 65.168 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.524 | 3.575 | 3.945 | 64.711 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.624 | 17.122 | 17.290 | 64.711 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 18.863 | 19.646 | 20.339 | 64.711 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.729 | 3.790 | 4.098 | 64.730 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.645 | 20.136 | 20.566 | 64.730 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 40.323 | 41.301 | 43.066 | 64.730 | 0.09x |
| users.json $..total | strata | 14.449 | 15.303 | 16.070 | 65.172 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 373.612 | 376.053 | 380.717 | 65.172 | 0.04x |

