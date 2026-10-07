# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 85eb3583e98587844415f5d32f85a61cbedfcf49
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 7763 64-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.393 | 12.345 | 16.317 | 64.008 | 1.00x |
| users.json | orjson | 13.613 | 16.358 | 18.716 | 64.008 | 0.75x |
| users.json | msgspec | 13.420 | 14.740 | 16.943 | 64.008 | 0.84x |
| users.json | ujson | 19.653 | 21.899 | 27.469 | 64.008 | 0.56x |
| users.json | pysimdjson | 19.565 | 22.301 | 27.120 | 64.008 | 0.55x |
| users.json | json | 23.214 | 24.690 | 27.093 | 64.008 | 0.50x |
| flat.json | strata | 0.869 | 0.910 | 1.089 | 79.496 | 1.00x |
| flat.json | orjson | 0.983 | 1.030 | 1.154 | 79.496 | 0.88x |
| flat.json | msgspec | 1.042 | 1.093 | 1.225 | 79.496 | 0.83x |
| flat.json | ujson | 1.624 | 1.732 | 1.866 | 79.496 | 0.53x |
| flat.json | pysimdjson | 1.551 | 1.689 | 2.076 | 79.496 | 0.54x |
| flat.json | json | 1.925 | 1.974 | 2.128 | 79.496 | 0.46x |
| nested.json | strata | 0.920 | 0.949 | 1.369 | 79.496 | 1.00x |
| nested.json | orjson | 1.094 | 1.185 | 1.550 | 79.496 | 0.80x |
| nested.json | msgspec | 1.077 | 1.149 | 1.275 | 79.496 | 0.83x |
| nested.json | ujson | 1.632 | 1.758 | 1.912 | 79.496 | 0.54x |
| nested.json | pysimdjson | 1.511 | 1.703 | 2.107 | 79.496 | 0.56x |
| nested.json | json | 2.183 | 2.290 | 2.447 | 79.496 | 0.41x |
| wide_arrays.json | strata | 5.458 | 6.289 | 7.098 | 83.520 | 1.00x |
| wide_arrays.json | orjson | 6.495 | 7.552 | 8.369 | 83.520 | 0.83x |
| wide_arrays.json | msgspec | 7.171 | 8.019 | 9.498 | 83.520 | 0.78x |
| wide_arrays.json | ujson | 8.841 | 9.799 | 11.564 | 83.520 | 0.64x |
| wide_arrays.json | pysimdjson | 8.141 | 8.428 | 8.629 | 83.520 | 0.75x |
| wide_arrays.json | json | 12.013 | 12.536 | 13.614 | 83.520 | 0.50x |
| mixed.json | strata | 0.205 | 0.230 | 0.242 | 83.520 | 1.00x |
| mixed.json | orjson | 0.250 | 0.276 | 0.292 | 83.520 | 0.83x |
| mixed.json | msgspec | 0.254 | 0.295 | 0.502 | 83.520 | 0.78x |
| mixed.json | ujson | 0.326 | 0.380 | 0.459 | 83.520 | 0.61x |
| mixed.json | pysimdjson | 0.332 | 0.351 | 0.403 | 83.520 | 0.66x |
| mixed.json | json | 0.515 | 0.537 | 0.571 | 83.520 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.314 | 2.419 | 3.225 | 62.770 | 1.00x |
| users.json | orjson | 2.987 | 3.337 | 3.778 | 62.770 | 0.72x |
| users.json | msgspec | 3.966 | 4.100 | 4.520 | 62.770 | 0.59x |
| users.json | ujson | 11.495 | 11.779 | 12.640 | 62.770 | 0.21x |
| users.json | json | 21.614 | 22.108 | 22.536 | 62.770 | 0.11x |
| flat.json | strata | 0.287 | 0.314 | 0.376 | 79.496 | 1.00x |
| flat.json | orjson | 0.346 | 0.364 | 0.393 | 79.496 | 0.86x |
| flat.json | msgspec | 0.450 | 0.466 | 0.484 | 79.496 | 0.67x |
| flat.json | ujson | 1.035 | 1.049 | 1.084 | 79.496 | 0.30x |
| flat.json | json | 1.869 | 1.903 | 1.929 | 79.496 | 0.16x |
| nested.json | strata | 0.245 | 0.267 | 0.287 | 79.496 | 1.00x |
| nested.json | orjson | 0.298 | 0.319 | 0.337 | 79.496 | 0.84x |
| nested.json | msgspec | 0.421 | 0.439 | 0.457 | 79.496 | 0.61x |
| nested.json | ujson | 1.101 | 1.129 | 1.203 | 79.496 | 0.24x |
| nested.json | json | 2.374 | 2.427 | 2.496 | 79.496 | 0.11x |
| wide_arrays.json | strata | 1.910 | 2.096 | 3.031 | 83.520 | 1.00x |
| wide_arrays.json | orjson | 1.965 | 2.122 | 2.873 | 83.520 | 0.99x |
| wide_arrays.json | msgspec | 2.859 | 3.294 | 4.090 | 83.520 | 0.64x |
| wide_arrays.json | ujson | 6.866 | 7.642 | 8.377 | 83.520 | 0.27x |
| wide_arrays.json | json | 17.771 | 18.481 | 18.955 | 83.520 | 0.11x |
| mixed.json | strata | 0.061 | 0.067 | 0.079 | 83.520 | 1.00x |
| mixed.json | orjson | 0.065 | 0.071 | 0.084 | 83.520 | 0.95x |
| mixed.json | msgspec | 0.086 | 0.097 | 0.108 | 83.520 | 0.69x |
| mixed.json | ujson | 0.231 | 0.244 | 0.264 | 83.520 | 0.27x |
| mixed.json | json | 0.512 | 0.540 | 0.577 | 83.520 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.260 | 12.755 | 13.626 | 81.133 | 1.00x |
| users.json | orjson | 13.741 | 14.796 | 19.428 | 81.133 | 0.86x |
| users.json | msgspec | 13.904 | 14.434 | 18.069 | 81.133 | 0.88x |
| users.json | ujson | 19.636 | 22.172 | 26.380 | 81.133 | 0.58x |
| users.json | json | 23.444 | 25.171 | 28.599 | 81.133 | 0.51x |
| flat.json | strata | 0.914 | 1.041 | 1.249 | 79.496 | 1.00x |
| flat.json | orjson | 1.072 | 1.199 | 1.406 | 79.496 | 0.87x |
| flat.json | msgspec | 1.135 | 1.253 | 1.486 | 79.496 | 0.83x |
| flat.json | ujson | 1.713 | 1.974 | 2.236 | 79.496 | 0.53x |
| flat.json | json | 1.994 | 2.128 | 2.436 | 79.496 | 0.49x |
| nested.json | strata | 0.896 | 1.003 | 2.238 | 79.496 | 1.00x |
| nested.json | orjson | 1.191 | 1.273 | 2.170 | 79.496 | 0.79x |
| nested.json | msgspec | 1.205 | 1.286 | 2.481 | 79.496 | 0.78x |
| nested.json | ujson | 1.660 | 1.815 | 2.222 | 79.496 | 0.55x |
| nested.json | json | 2.225 | 2.395 | 2.500 | 79.496 | 0.42x |
| wide_arrays.json | strata | 4.696 | 4.822 | 5.069 | 83.520 | 1.00x |
| wide_arrays.json | orjson | 5.845 | 6.090 | 6.893 | 83.520 | 0.79x |
| wide_arrays.json | msgspec | 6.408 | 6.614 | 7.430 | 83.520 | 0.73x |
| wide_arrays.json | ujson | 7.921 | 8.249 | 9.041 | 83.520 | 0.58x |
| wide_arrays.json | json | 10.558 | 10.851 | 11.097 | 83.520 | 0.44x |
| mixed.json | strata | 0.205 | 0.218 | 0.240 | 83.520 | 1.00x |
| mixed.json | orjson | 0.275 | 0.293 | 0.378 | 83.520 | 0.74x |
| mixed.json | msgspec | 0.286 | 0.298 | 0.396 | 83.520 | 0.73x |
| mixed.json | ujson | 0.357 | 0.379 | 0.526 | 83.520 | 0.57x |
| mixed.json | json | 0.521 | 0.541 | 0.575 | 83.520 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 13.246 | 13.984 | 14.879 | 79.496 | 1.00x |
| users.ndjson | orjson | 19.473 | 21.051 | 22.432 | 79.496 | 0.66x |
| users.ndjson | msgspec | 19.865 | 20.702 | 22.280 | 79.496 | 0.68x |
| users.ndjson | ujson | 26.781 | 27.328 | 29.051 | 79.496 | 0.51x |
| users.ndjson | json | 33.641 | 34.282 | 35.277 | 79.496 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.882 | 2.988 | 3.834 | 81.133 | 1.00x |
| users.json | orjson | 3.735 | 3.906 | 4.442 | 81.133 | 0.76x |
| users.json | msgspec | 4.524 | 4.685 | 5.456 | 81.133 | 0.64x |
| users.json | ujson | 12.552 | 12.896 | 13.343 | 81.133 | 0.23x |
| users.json | json | 22.719 | 22.996 | 23.371 | 81.133 | 0.13x |
| flat.json | strata | 0.529 | 0.561 | 0.664 | 79.496 | 1.00x |
| flat.json | orjson | 0.603 | 0.638 | 0.712 | 79.496 | 0.88x |
| flat.json | msgspec | 0.688 | 0.740 | 0.833 | 79.496 | 0.76x |
| flat.json | ujson | 1.306 | 1.352 | 1.443 | 79.496 | 0.41x |
| flat.json | json | 2.144 | 2.225 | 2.328 | 79.496 | 0.25x |
| nested.json | strata | 0.387 | 0.478 | 0.551 | 79.496 | 1.00x |
| nested.json | orjson | 0.499 | 0.565 | 0.627 | 79.496 | 0.85x |
| nested.json | msgspec | 0.626 | 0.674 | 0.715 | 79.496 | 0.71x |
| nested.json | ujson | 1.294 | 1.389 | 1.490 | 79.496 | 0.34x |
| nested.json | json | 2.611 | 2.704 | 2.785 | 79.496 | 0.18x |
| wide_arrays.json | strata | 2.435 | 2.680 | 3.387 | 83.520 | 1.00x |
| wide_arrays.json | orjson | 2.545 | 2.949 | 3.690 | 83.520 | 0.91x |
| wide_arrays.json | msgspec | 3.482 | 4.013 | 4.903 | 83.520 | 0.67x |
| wide_arrays.json | ujson | 7.596 | 8.202 | 9.370 | 83.520 | 0.33x |
| wide_arrays.json | json | 18.490 | 19.001 | 19.639 | 83.520 | 0.14x |
| mixed.json | strata | 0.145 | 0.168 | 0.222 | 83.520 | 1.00x |
| mixed.json | orjson | 0.171 | 0.178 | 0.251 | 83.520 | 0.95x |
| mixed.json | msgspec | 0.191 | 0.205 | 0.375 | 83.520 | 0.82x |
| mixed.json | ujson | 0.346 | 0.370 | 0.471 | 83.520 | 0.45x |
| mixed.json | json | 0.625 | 0.682 | 0.796 | 83.520 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.064 | 0.076 | 0.090 | 81.133 | 1.00x |
| users.json $[*].id | jmespath | 0.495 | 0.515 | 0.533 | 81.133 | 0.15x |
| users.json $[*].id | jsonpath-ng | 2.924 | 2.968 | 3.196 | 81.133 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.482 | 0.532 | 0.730 | 81.133 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.156 | 3.255 | 4.161 | 81.133 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 21.240 | 22.438 | 25.063 | 81.133 | 0.02x |
| users.json $..total | strata | 1.679 | 1.961 | 2.428 | 81.133 | 1.00x |
| users.json $..total | jsonpath-ng | 391.054 | 398.040 | 405.502 | 81.133 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.298 | 3.337 | 3.423 | 81.133 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.727 | 17.338 | 21.999 | 81.133 | 0.19x |
| users.json $[*].id | orjson+jsonpath-ng | 17.743 | 18.722 | 22.462 | 81.133 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.571 | 3.623 | 3.674 | 81.133 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.191 | 19.548 | 25.619 | 81.133 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.321 | 41.739 | 46.292 | 81.133 | 0.09x |
| users.json $..total | strata | 15.796 | 18.484 | 23.180 | 81.133 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 420.655 | 426.991 | 433.924 | 81.133 | 0.04x |

