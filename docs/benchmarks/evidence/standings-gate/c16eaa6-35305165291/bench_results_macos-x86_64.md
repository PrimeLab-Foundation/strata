# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch x86_64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.478 | 21.987 | 25.491 | 57.016 | 1.00x |
| users.json | orjson | 31.355 | 33.733 | 39.062 | 57.016 | 0.65x |
| users.json | msgspec | 30.858 | 32.885 | 38.236 | 57.016 | 0.67x |
| users.json | ujson | 45.051 | 47.178 | 51.609 | 57.016 | 0.47x |
| users.json | pysimdjson | 191.905 | 201.830 | 218.468 | 57.016 | 0.11x |
| users.json | json | 51.257 | 54.163 | 58.290 | 57.016 | 0.41x |
| flat.json | strata | 1.276 | 1.326 | 1.410 | 66.152 | 1.00x |
| flat.json | orjson | 1.424 | 1.478 | 1.571 | 66.152 | 0.90x |
| flat.json | msgspec | 1.634 | 1.664 | 1.782 | 66.152 | 0.80x |
| flat.json | ujson | 2.871 | 2.953 | 3.202 | 66.152 | 0.45x |
| flat.json | pysimdjson | 15.224 | 15.837 | 16.807 | 66.152 | 0.08x |
| flat.json | json | 3.267 | 3.441 | 3.556 | 66.152 | 0.39x |
| nested.json | strata | 1.509 | 1.549 | 2.005 | 64.633 | 1.00x |
| nested.json | orjson | 1.748 | 1.783 | 1.860 | 64.633 | 0.87x |
| nested.json | msgspec | 1.919 | 1.963 | 2.317 | 64.633 | 0.79x |
| nested.json | ujson | 3.170 | 3.203 | 3.730 | 64.633 | 0.48x |
| nested.json | pysimdjson | 13.906 | 14.103 | 15.485 | 64.633 | 0.11x |
| nested.json | json | 4.013 | 4.090 | 4.483 | 64.633 | 0.38x |
| wide_arrays.json | strata | 8.431 | 8.642 | 9.061 | 70.594 | 1.00x |
| wide_arrays.json | orjson | 10.302 | 10.584 | 11.837 | 70.594 | 0.82x |
| wide_arrays.json | msgspec | 11.277 | 11.559 | 12.517 | 70.594 | 0.75x |
| wide_arrays.json | ujson | 14.118 | 14.531 | 15.271 | 70.594 | 0.59x |
| wide_arrays.json | pysimdjson | 86.734 | 87.822 | 88.983 | 70.594 | 0.10x |
| wide_arrays.json | json | 18.577 | 18.959 | 19.741 | 70.594 | 0.46x |
| mixed.json | strata | 0.368 | 0.389 | 0.448 | 63.391 | 1.00x |
| mixed.json | orjson | 0.458 | 0.489 | 0.528 | 63.391 | 0.80x |
| mixed.json | msgspec | 0.483 | 0.507 | 0.562 | 63.391 | 0.77x |
| mixed.json | ujson | 0.665 | 0.710 | 0.798 | 63.391 | 0.55x |
| mixed.json | pysimdjson | 3.376 | 3.539 | 3.981 | 63.391 | 0.11x |
| mixed.json | json | 0.935 | 0.969 | 1.080 | 63.391 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.134 | 3.316 | 4.609 | 52.391 | 1.00x |
| users.json | orjson | 4.108 | 4.492 | 6.201 | 52.391 | 0.74x |
| users.json | msgspec | 6.333 | 7.061 | 8.962 | 52.391 | 0.47x |
| users.json | ujson | 29.306 | 31.407 | 39.835 | 52.391 | 0.11x |
| users.json | json | 50.117 | 54.412 | 65.645 | 52.391 | 0.06x |
| flat.json | strata | 0.367 | 0.372 | 0.435 | 64.664 | 1.00x |
| flat.json | orjson | 0.439 | 0.452 | 0.462 | 64.664 | 0.82x |
| flat.json | msgspec | 0.585 | 0.609 | 0.626 | 64.664 | 0.61x |
| flat.json | ujson | 2.344 | 2.414 | 2.475 | 64.664 | 0.15x |
| flat.json | json | 4.026 | 4.096 | 4.231 | 64.664 | 0.09x |
| nested.json | strata | 0.251 | 0.269 | 0.382 | 64.762 | 1.00x |
| nested.json | orjson | 0.370 | 0.390 | 0.659 | 64.762 | 0.69x |
| nested.json | msgspec | 0.581 | 0.605 | 0.964 | 64.762 | 0.44x |
| nested.json | ujson | 2.406 | 2.502 | 2.807 | 64.762 | 0.11x |
| nested.json | json | 4.836 | 4.924 | 5.656 | 64.762 | 0.05x |
| wide_arrays.json | strata | 2.114 | 2.234 | 2.474 | 64.320 | 1.00x |
| wide_arrays.json | orjson | 2.626 | 2.771 | 3.033 | 64.320 | 0.81x |
| wide_arrays.json | msgspec | 3.709 | 3.915 | 4.348 | 64.320 | 0.57x |
| wide_arrays.json | ujson | 11.519 | 11.689 | 12.690 | 64.320 | 0.19x |
| wide_arrays.json | json | 35.629 | 37.605 | 38.053 | 64.320 | 0.06x |
| mixed.json | strata | 0.067 | 0.083 | 0.145 | 60.191 | 1.00x |
| mixed.json | orjson | 0.080 | 0.096 | 0.109 | 60.191 | 0.86x |
| mixed.json | msgspec | 0.113 | 0.131 | 0.150 | 60.191 | 0.63x |
| mixed.json | ujson | 0.479 | 0.526 | 0.676 | 60.191 | 0.16x |
| mixed.json | json | 1.002 | 1.065 | 1.180 | 60.191 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 21.368 | 22.300 | 26.416 | 63.172 | 1.00x |
| users.json | orjson | 31.568 | 33.010 | 40.446 | 63.172 | 0.68x |
| users.json | msgspec | 31.734 | 34.120 | 39.045 | 63.172 | 0.65x |
| users.json | ujson | 46.693 | 48.688 | 52.129 | 63.172 | 0.46x |
| users.json | json | 52.362 | 54.179 | 59.586 | 63.172 | 0.41x |
| flat.json | strata | 1.451 | 1.497 | 1.528 | 64.664 | 1.00x |
| flat.json | orjson | 1.623 | 1.689 | 1.718 | 64.664 | 0.89x |
| flat.json | msgspec | 1.883 | 1.920 | 2.022 | 64.664 | 0.78x |
| flat.json | ujson | 3.207 | 3.265 | 3.343 | 64.664 | 0.46x |
| flat.json | json | 3.612 | 3.665 | 3.733 | 64.664 | 0.41x |
| nested.json | strata | 1.678 | 1.753 | 2.200 | 64.762 | 1.00x |
| nested.json | orjson | 1.923 | 2.086 | 2.841 | 64.762 | 0.84x |
| nested.json | msgspec | 2.116 | 2.270 | 2.418 | 64.762 | 0.77x |
| nested.json | ujson | 3.398 | 3.697 | 4.634 | 64.762 | 0.47x |
| nested.json | json | 4.443 | 4.726 | 5.215 | 64.762 | 0.37x |
| wide_arrays.json | strata | 7.903 | 8.648 | 8.873 | 65.551 | 1.00x |
| wide_arrays.json | orjson | 9.723 | 10.667 | 10.938 | 65.551 | 0.81x |
| wide_arrays.json | msgspec | 10.697 | 11.896 | 12.580 | 65.551 | 0.73x |
| wide_arrays.json | ujson | 13.536 | 15.282 | 15.434 | 65.551 | 0.57x |
| wide_arrays.json | json | 17.793 | 18.946 | 20.253 | 65.551 | 0.46x |
| mixed.json | strata | 0.469 | 0.486 | 0.562 | 60.191 | 1.00x |
| mixed.json | orjson | 0.614 | 0.654 | 0.782 | 60.191 | 0.74x |
| mixed.json | msgspec | 0.649 | 0.675 | 0.725 | 60.191 | 0.72x |
| mixed.json | ujson | 0.842 | 0.888 | 0.930 | 60.191 | 0.55x |
| mixed.json | json | 1.075 | 1.093 | 1.195 | 60.191 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 19.270 | 20.005 | 28.074 | 65.730 | 1.00x |
| users.ndjson | orjson | 27.935 | 31.321 | 32.790 | 65.730 | 0.64x |
| users.ndjson | msgspec | 28.575 | 29.388 | 32.589 | 65.730 | 0.68x |
| users.ndjson | ujson | 40.838 | 42.599 | 46.204 | 65.730 | 0.47x |
| users.ndjson | json | 50.949 | 52.604 | 55.847 | 65.730 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.808 | 3.995 | 4.459 | 63.211 | 1.00x |
| users.json | orjson | 5.309 | 5.464 | 6.585 | 63.211 | 0.73x |
| users.json | msgspec | 7.212 | 7.364 | 8.274 | 63.211 | 0.54x |
| users.json | ujson | 30.542 | 31.514 | 36.349 | 63.211 | 0.13x |
| users.json | json | 50.180 | 53.400 | 60.489 | 63.211 | 0.07x |
| flat.json | strata | 0.683 | 0.717 | 0.774 | 64.664 | 1.00x |
| flat.json | orjson | 0.775 | 0.826 | 0.872 | 64.664 | 0.87x |
| flat.json | msgspec | 0.961 | 0.998 | 1.071 | 64.664 | 0.72x |
| flat.json | ujson | 2.693 | 2.739 | 2.803 | 64.664 | 0.26x |
| flat.json | json | 4.224 | 4.293 | 4.640 | 64.664 | 0.17x |
| nested.json | strata | 0.551 | 0.626 | 0.801 | 64.762 | 1.00x |
| nested.json | orjson | 0.710 | 0.815 | 1.073 | 64.762 | 0.77x |
| nested.json | msgspec | 0.986 | 1.061 | 1.352 | 64.762 | 0.59x |
| nested.json | ujson | 2.867 | 2.988 | 3.291 | 64.762 | 0.21x |
| nested.json | json | 5.307 | 5.729 | 6.626 | 64.762 | 0.11x |
| wide_arrays.json | strata | 2.801 | 2.917 | 3.423 | 64.320 | 1.00x |
| wide_arrays.json | orjson | 3.469 | 3.578 | 4.015 | 64.320 | 0.82x |
| wide_arrays.json | msgspec | 4.480 | 4.722 | 5.211 | 64.320 | 0.62x |
| wide_arrays.json | ujson | 12.552 | 12.722 | 13.713 | 64.320 | 0.23x |
| wide_arrays.json | json | 36.569 | 36.986 | 40.192 | 64.320 | 0.08x |
| mixed.json | strata | 0.389 | 0.433 | 0.471 | 60.191 | 1.00x |
| mixed.json | orjson | 0.438 | 0.480 | 0.521 | 60.191 | 0.90x |
| mixed.json | msgspec | 0.490 | 0.538 | 0.762 | 60.191 | 0.80x |
| mixed.json | ujson | 0.853 | 0.952 | 1.255 | 60.191 | 0.45x |
| mixed.json | json | 1.390 | 1.482 | 1.777 | 60.191 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.201 | 0.233 | 0.338 | 63.281 | 1.00x |
| users.json $[*].id | jmespath | 1.164 | 1.242 | 1.542 | 63.281 | 0.19x |
| users.json $[*].id | jsonpath-ng | 6.484 | 6.940 | 7.796 | 63.281 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.386 | 1.476 | 1.944 | 60.488 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 7.524 | 7.751 | 9.393 | 60.488 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 43.299 | 46.045 | 54.706 | 60.488 | 0.03x |
| users.json $..total | strata | 4.051 | 4.466 | 7.498 | 60.555 | 1.00x |
| users.json $..total | jsonpath-ng | 852.534 | 905.114 | 1320.905 | 60.555 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.446 | 4.863 | 6.299 | 63.359 | 1.00x |
| users.json $[*].id | orjson+jmespath | 32.922 | 34.747 | 37.259 | 63.359 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 37.629 | 40.243 | 48.175 | 63.359 | 0.12x |
| users.json $[*].orders[*].total | strata | 5.136 | 6.163 | 7.724 | 60.543 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 40.773 | 49.067 | 52.704 | 60.543 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 93.468 | 98.057 | 119.690 | 60.543 | 0.06x |
| users.json $..total | strata | 23.617 | 25.385 | 26.843 | 60.555 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 787.185 | 843.416 | 910.357 | 60.555 | 0.03x |

