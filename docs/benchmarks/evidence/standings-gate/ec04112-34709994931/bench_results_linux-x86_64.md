# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec0411225b6d58a1df905844c946a766d3c39f0a
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
| users.json | strata | 11.139 | 11.819 | 14.646 | 65.113 | 1.00x |
| users.json | orjson | 15.306 | 15.903 | 17.960 | 65.113 | 0.74x |
| users.json | msgspec | 14.458 | 14.972 | 16.797 | 65.113 | 0.79x |
| users.json | ujson | 20.134 | 20.920 | 23.797 | 65.113 | 0.56x |
| users.json | pysimdjson | 20.613 | 21.249 | 23.684 | 65.113 | 0.56x |
| users.json | json | 23.256 | 23.917 | 24.935 | 65.113 | 0.49x |
| flat.json | strata | 0.929 | 0.968 | 1.006 | 79.637 | 1.00x |
| flat.json | orjson | 1.088 | 1.131 | 1.194 | 79.637 | 0.86x |
| flat.json | msgspec | 1.043 | 1.070 | 1.100 | 79.637 | 0.90x |
| flat.json | ujson | 1.554 | 1.597 | 1.640 | 79.637 | 0.61x |
| flat.json | pysimdjson | 1.735 | 1.773 | 1.792 | 79.637 | 0.55x |
| flat.json | json | 1.812 | 1.846 | 1.868 | 79.637 | 0.52x |
| nested.json | strata | 0.816 | 0.835 | 0.907 | 79.637 | 1.00x |
| nested.json | orjson | 0.993 | 1.013 | 1.066 | 79.637 | 0.82x |
| nested.json | msgspec | 1.004 | 1.042 | 1.353 | 79.637 | 0.80x |
| nested.json | ujson | 1.435 | 1.500 | 1.557 | 79.637 | 0.56x |
| nested.json | pysimdjson | 1.442 | 1.506 | 1.666 | 79.637 | 0.55x |
| nested.json | json | 1.977 | 2.030 | 2.122 | 79.637 | 0.41x |
| wide_arrays.json | strata | 4.300 | 4.445 | 4.656 | 84.062 | 1.00x |
| wide_arrays.json | orjson | 5.513 | 5.642 | 6.093 | 84.062 | 0.79x |
| wide_arrays.json | msgspec | 5.832 | 6.310 | 6.621 | 84.062 | 0.70x |
| wide_arrays.json | ujson | 7.323 | 7.677 | 8.225 | 84.062 | 0.58x |
| wide_arrays.json | pysimdjson | 5.967 | 6.369 | 6.948 | 84.062 | 0.70x |
| wide_arrays.json | json | 9.883 | 10.198 | 11.001 | 84.062 | 0.44x |
| mixed.json | strata | 0.227 | 0.235 | 0.258 | 84.125 | 1.00x |
| mixed.json | orjson | 0.260 | 0.265 | 0.277 | 84.125 | 0.89x |
| mixed.json | msgspec | 0.266 | 0.279 | 0.281 | 84.125 | 0.84x |
| mixed.json | ujson | 0.333 | 0.350 | 0.364 | 84.125 | 0.67x |
| mixed.json | pysimdjson | 0.373 | 0.385 | 0.401 | 84.125 | 0.61x |
| mixed.json | json | 0.507 | 0.512 | 0.527 | 84.125 | 0.46x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.142 | 2.161 | 2.182 | 63.477 | 1.00x |
| users.json | orjson | 2.366 | 2.424 | 2.498 | 63.477 | 0.89x |
| users.json | msgspec | 3.905 | 3.939 | 3.992 | 63.477 | 0.55x |
| users.json | ujson | 11.459 | 11.656 | 12.124 | 63.477 | 0.19x |
| users.json | json | 22.492 | 22.644 | 22.891 | 63.477 | 0.10x |
| flat.json | strata | 0.267 | 0.281 | 0.313 | 79.637 | 1.00x |
| flat.json | orjson | 0.328 | 0.341 | 0.402 | 79.637 | 0.82x |
| flat.json | msgspec | 0.468 | 0.490 | 0.511 | 79.637 | 0.57x |
| flat.json | ujson | 1.030 | 1.042 | 1.064 | 79.637 | 0.27x |
| flat.json | json | 1.992 | 2.011 | 2.030 | 79.637 | 0.14x |
| nested.json | strata | 0.198 | 0.202 | 0.224 | 79.637 | 1.00x |
| nested.json | orjson | 0.292 | 0.307 | 0.352 | 79.637 | 0.66x |
| nested.json | msgspec | 0.407 | 0.414 | 0.427 | 79.637 | 0.49x |
| nested.json | ujson | 1.126 | 1.134 | 1.162 | 79.637 | 0.18x |
| nested.json | json | 2.501 | 2.523 | 2.538 | 79.637 | 0.08x |
| wide_arrays.json | strata | 1.653 | 1.800 | 1.872 | 84.062 | 1.00x |
| wide_arrays.json | orjson | 1.812 | 1.850 | 1.957 | 84.062 | 0.97x |
| wide_arrays.json | msgspec | 2.881 | 2.966 | 3.014 | 84.062 | 0.61x |
| wide_arrays.json | ujson | 6.219 | 6.385 | 6.496 | 84.062 | 0.28x |
| wide_arrays.json | json | 16.135 | 16.400 | 16.914 | 84.062 | 0.11x |
| mixed.json | strata | 0.075 | 0.082 | 0.088 | 84.125 | 1.00x |
| mixed.json | orjson | 0.084 | 0.099 | 0.120 | 84.125 | 0.83x |
| mixed.json | msgspec | 0.087 | 0.090 | 0.100 | 84.125 | 0.92x |
| mixed.json | ujson | 0.236 | 0.241 | 0.250 | 84.125 | 0.34x |
| mixed.json | json | 0.554 | 0.566 | 0.601 | 84.125 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.236 | 12.457 | 13.683 | 81.266 | 1.00x |
| users.json | orjson | 15.775 | 16.119 | 17.047 | 81.266 | 0.77x |
| users.json | msgspec | 15.428 | 15.685 | 18.145 | 81.266 | 0.79x |
| users.json | ujson | 20.899 | 21.843 | 23.219 | 81.266 | 0.57x |
| users.json | json | 24.104 | 24.531 | 34.391 | 81.266 | 0.51x |
| flat.json | strata | 0.973 | 1.011 | 1.075 | 79.637 | 1.00x |
| flat.json | orjson | 1.194 | 1.218 | 1.260 | 79.637 | 0.83x |
| flat.json | msgspec | 1.094 | 1.114 | 1.128 | 79.637 | 0.91x |
| flat.json | ujson | 1.656 | 1.668 | 1.747 | 79.637 | 0.61x |
| flat.json | json | 1.900 | 1.913 | 1.998 | 79.637 | 0.53x |
| nested.json | strata | 0.840 | 0.870 | 0.929 | 79.637 | 1.00x |
| nested.json | orjson | 1.049 | 1.100 | 1.172 | 79.637 | 0.79x |
| nested.json | msgspec | 1.045 | 1.091 | 1.120 | 79.637 | 0.80x |
| nested.json | ujson | 1.533 | 1.583 | 1.638 | 79.637 | 0.55x |
| nested.json | json | 2.047 | 2.076 | 2.135 | 79.637 | 0.42x |
| wide_arrays.json | strata | 4.507 | 4.627 | 4.713 | 84.125 | 1.00x |
| wide_arrays.json | orjson | 5.325 | 5.520 | 5.717 | 84.125 | 0.84x |
| wide_arrays.json | msgspec | 6.186 | 6.290 | 6.780 | 84.125 | 0.74x |
| wide_arrays.json | ujson | 7.860 | 8.017 | 8.320 | 84.125 | 0.58x |
| wide_arrays.json | json | 10.122 | 10.390 | 10.626 | 84.125 | 0.45x |
| mixed.json | strata | 0.243 | 0.260 | 0.275 | 84.125 | 1.00x |
| mixed.json | orjson | 0.313 | 0.334 | 0.360 | 84.125 | 0.78x |
| mixed.json | msgspec | 0.301 | 0.340 | 0.348 | 84.125 | 0.77x |
| mixed.json | ujson | 0.397 | 0.428 | 0.499 | 84.125 | 0.61x |
| mixed.json | json | 0.537 | 0.549 | 0.601 | 84.125 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 12.401 | 12.800 | 13.194 | 79.637 | 1.00x |
| users.ndjson | orjson | 19.070 | 19.315 | 19.693 | 79.637 | 0.66x |
| users.ndjson | msgspec | 18.460 | 18.929 | 19.297 | 79.637 | 0.68x |
| users.ndjson | ujson | 23.863 | 24.159 | 24.545 | 79.637 | 0.53x |
| users.ndjson | json | 30.478 | 30.845 | 31.253 | 79.637 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.772 | 2.819 | 2.875 | 81.266 | 1.00x |
| users.json | orjson | 3.154 | 3.179 | 3.254 | 81.266 | 0.89x |
| users.json | msgspec | 4.561 | 4.666 | 4.777 | 81.266 | 0.60x |
| users.json | ujson | 12.524 | 12.688 | 12.800 | 81.266 | 0.22x |
| users.json | json | 23.465 | 23.750 | 26.958 | 81.266 | 0.12x |
| flat.json | strata | 0.398 | 0.421 | 0.434 | 79.637 | 1.00x |
| flat.json | orjson | 0.444 | 0.467 | 0.593 | 79.637 | 0.90x |
| flat.json | msgspec | 0.632 | 0.651 | 0.687 | 79.637 | 0.65x |
| flat.json | ujson | 1.194 | 1.205 | 1.274 | 79.637 | 0.35x |
| flat.json | json | 2.141 | 2.182 | 2.213 | 79.637 | 0.19x |
| nested.json | strata | 0.318 | 0.329 | 0.361 | 79.637 | 1.00x |
| nested.json | orjson | 0.409 | 0.434 | 0.455 | 79.637 | 0.76x |
| nested.json | msgspec | 0.548 | 0.566 | 0.587 | 79.637 | 0.58x |
| nested.json | ujson | 1.218 | 1.237 | 1.257 | 79.637 | 0.27x |
| nested.json | json | 2.640 | 2.669 | 2.736 | 79.637 | 0.12x |
| wide_arrays.json | strata | 2.111 | 2.250 | 2.340 | 84.125 | 1.00x |
| wide_arrays.json | orjson | 2.276 | 2.349 | 2.526 | 84.125 | 0.96x |
| wide_arrays.json | msgspec | 3.344 | 3.441 | 3.634 | 84.125 | 0.65x |
| wide_arrays.json | ujson | 6.815 | 6.918 | 7.179 | 84.125 | 0.33x |
| wide_arrays.json | json | 16.726 | 17.010 | 17.224 | 84.125 | 0.13x |
| mixed.json | strata | 0.147 | 0.162 | 0.174 | 84.125 | 1.00x |
| mixed.json | orjson | 0.157 | 0.181 | 0.216 | 84.125 | 0.90x |
| mixed.json | msgspec | 0.174 | 0.183 | 0.199 | 84.125 | 0.89x |
| mixed.json | ujson | 0.337 | 0.347 | 0.370 | 84.125 | 0.47x |
| mixed.json | json | 0.655 | 0.677 | 0.714 | 84.125 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.102 | 0.113 | 0.130 | 81.266 | 1.00x |
| users.json $[*].id | jmespath | 0.541 | 0.550 | 0.700 | 81.266 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.613 | 2.700 | 2.757 | 81.266 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.466 | 0.484 | 0.549 | 81.273 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.197 | 3.236 | 3.337 | 81.273 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.578 | 19.175 | 19.620 | 81.273 | 0.03x |
| users.json $..total | strata | 1.780 | 1.828 | 1.904 | 81.273 | 1.00x |
| users.json $..total | jsonpath-ng | 347.074 | 350.871 | 351.583 | 81.273 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.485 | 3.536 | 3.571 | 81.273 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.475 | 16.729 | 16.954 | 81.273 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 18.713 | 19.009 | 19.516 | 81.273 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.684 | 3.746 | 3.755 | 81.273 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.397 | 19.717 | 20.776 | 81.273 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 39.710 | 40.411 | 41.237 | 81.273 | 0.09x |
| users.json $..total | strata | 15.054 | 15.608 | 16.185 | 81.273 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 375.161 | 379.459 | 383.090 | 81.273 | 0.04x |

