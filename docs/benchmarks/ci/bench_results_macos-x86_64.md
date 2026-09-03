# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 2f12155
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.865 | 21.349 | 31.537 | 52.809 | 1.00x |
| users.json | orjson | 28.703 | 30.924 | 39.769 | 52.809 | 0.69x |
| users.json | msgspec | 28.556 | 33.167 | 40.783 | 52.809 | 0.64x |
| users.json | ujson | 45.120 | 48.876 | 59.265 | 52.809 | 0.44x |
| users.json | pysimdjson | 180.285 | 194.776 | 212.766 | 52.809 | 0.11x |
| users.json | json | 49.999 | 53.990 | 59.322 | 52.809 | 0.40x |
| flat.json | strata | 1.428 | 1.464 | 1.626 | 59.270 | 1.00x |
| flat.json | orjson | 1.551 | 1.591 | 1.779 | 59.270 | 0.92x |
| flat.json | msgspec | 1.855 | 1.890 | 1.979 | 59.270 | 0.77x |
| flat.json | ujson | 3.140 | 3.335 | 4.517 | 59.270 | 0.44x |
| flat.json | pysimdjson | 16.767 | 17.233 | 17.839 | 59.270 | 0.08x |
| flat.json | json | 3.821 | 4.023 | 4.582 | 59.270 | 0.36x |
| nested.json | strata | 1.648 | 1.753 | 2.021 | 47.617 | 1.00x |
| nested.json | orjson | 1.903 | 2.095 | 2.181 | 47.617 | 0.84x |
| nested.json | msgspec | 2.110 | 2.273 | 3.085 | 47.617 | 0.77x |
| nested.json | ujson | 3.475 | 3.632 | 3.869 | 47.617 | 0.48x |
| nested.json | pysimdjson | 15.038 | 15.273 | 17.387 | 47.617 | 0.11x |
| nested.json | json | 4.558 | 4.715 | 5.410 | 47.617 | 0.37x |
| wide_arrays.json | strata | 9.269 | 9.739 | 12.304 | 58.922 | 1.00x |
| wide_arrays.json | orjson | 10.066 | 10.681 | 11.607 | 58.922 | 0.91x |
| wide_arrays.json | msgspec | 11.217 | 11.858 | 13.755 | 58.922 | 0.82x |
| wide_arrays.json | ujson | 14.459 | 15.486 | 21.100 | 58.922 | 0.63x |
| wide_arrays.json | pysimdjson | 85.561 | 89.456 | 94.365 | 58.922 | 0.11x |
| wide_arrays.json | json | 18.240 | 20.310 | 23.225 | 58.922 | 0.48x |
| mixed.json | strata | 0.416 | 0.433 | 0.539 | 57.684 | 1.00x |
| mixed.json | orjson | 0.498 | 0.507 | 0.744 | 57.684 | 0.85x |
| mixed.json | msgspec | 0.517 | 0.530 | 0.829 | 57.684 | 0.82x |
| mixed.json | ujson | 0.728 | 0.745 | 1.066 | 57.684 | 0.58x |
| mixed.json | pysimdjson | 3.577 | 3.757 | 4.243 | 57.684 | 0.12x |
| mixed.json | json | 1.023 | 1.081 | 1.394 | 57.684 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.932 | 3.096 | 3.333 | 48.457 | 1.00x |
| users.json | orjson | 3.791 | 3.923 | 4.070 | 48.457 | 0.79x |
| users.json | msgspec | 5.970 | 6.514 | 6.982 | 48.457 | 0.48x |
| users.json | ujson | 33.810 | 35.422 | 38.454 | 48.457 | 0.09x |
| users.json | json | 45.464 | 47.575 | 54.591 | 48.457 | 0.07x |
| flat.json | strata | 0.351 | 0.374 | 0.480 | 48.051 | 1.00x |
| flat.json | orjson | 0.449 | 0.477 | 0.725 | 48.051 | 0.78x |
| flat.json | msgspec | 0.614 | 0.634 | 0.788 | 48.051 | 0.59x |
| flat.json | ujson | 2.910 | 2.954 | 3.378 | 48.051 | 0.13x |
| flat.json | json | 4.014 | 4.108 | 4.614 | 48.051 | 0.09x |
| nested.json | strata | 0.293 | 0.353 | 0.426 | 47.363 | 1.00x |
| nested.json | orjson | 0.406 | 0.455 | 0.587 | 47.363 | 0.78x |
| nested.json | msgspec | 0.641 | 0.721 | 0.818 | 47.363 | 0.49x |
| nested.json | ujson | 3.164 | 3.473 | 3.868 | 47.363 | 0.10x |
| nested.json | json | 5.232 | 5.671 | 6.636 | 47.363 | 0.06x |
| wide_arrays.json | strata | 2.235 | 2.334 | 2.616 | 60.789 | 1.00x |
| wide_arrays.json | orjson | 2.716 | 2.867 | 3.813 | 60.789 | 0.81x |
| wide_arrays.json | msgspec | 3.567 | 3.667 | 4.113 | 60.789 | 0.64x |
| wide_arrays.json | ujson | 16.859 | 17.184 | 18.098 | 60.789 | 0.14x |
| wide_arrays.json | json | 37.552 | 38.443 | 44.087 | 60.789 | 0.06x |
| mixed.json | strata | 0.097 | 0.107 | 0.135 | 56.488 | 1.00x |
| mixed.json | orjson | 0.097 | 0.106 | 0.168 | 56.488 | 1.01x |
| mixed.json | msgspec | 0.136 | 0.147 | 0.204 | 56.488 | 0.73x |
| mixed.json | ujson | 0.687 | 0.705 | 0.931 | 56.488 | 0.15x |
| mixed.json | json | 1.057 | 1.079 | 1.711 | 56.488 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 21.987 | 22.371 | 22.985 | 60.926 | 1.00x |
| users.json | orjson | 32.106 | 32.860 | 39.420 | 60.926 | 0.68x |
| users.json | msgspec | 31.969 | 34.047 | 38.570 | 60.926 | 0.66x |
| users.json | ujson | 43.512 | 47.937 | 56.132 | 60.926 | 0.47x |
| users.json | json | 50.193 | 54.612 | 65.827 | 60.926 | 0.41x |
| flat.json | strata | 1.539 | 1.571 | 2.027 | 47.391 | 1.00x |
| flat.json | orjson | 1.723 | 1.779 | 1.874 | 47.391 | 0.88x |
| flat.json | msgspec | 1.956 | 2.002 | 2.957 | 47.391 | 0.78x |
| flat.json | ujson | 3.315 | 3.444 | 3.960 | 47.391 | 0.46x |
| flat.json | json | 3.904 | 4.024 | 4.782 | 47.391 | 0.39x |
| nested.json | strata | 1.721 | 1.767 | 1.958 | 47.363 | 1.00x |
| nested.json | orjson | 2.021 | 2.056 | 2.147 | 47.363 | 0.86x |
| nested.json | msgspec | 2.215 | 2.276 | 2.389 | 47.363 | 0.78x |
| nested.json | ujson | 3.558 | 3.633 | 3.741 | 47.363 | 0.49x |
| nested.json | json | 4.624 | 4.723 | 5.328 | 47.363 | 0.37x |
| wide_arrays.json | strata | 9.594 | 11.424 | 12.091 | 60.789 | 1.00x |
| wide_arrays.json | orjson | 10.930 | 12.760 | 14.949 | 60.789 | 0.90x |
| wide_arrays.json | msgspec | 11.773 | 13.219 | 15.707 | 60.789 | 0.86x |
| wide_arrays.json | ujson | 15.190 | 19.267 | 22.978 | 60.789 | 0.59x |
| wide_arrays.json | json | 19.230 | 21.758 | 24.971 | 60.789 | 0.53x |
| mixed.json | strata | 0.513 | 0.537 | 0.679 | 56.488 | 1.00x |
| mixed.json | orjson | 0.643 | 0.681 | 0.836 | 56.488 | 0.79x |
| mixed.json | msgspec | 0.684 | 0.715 | 0.866 | 56.488 | 0.75x |
| mixed.json | ujson | 0.893 | 0.925 | 1.141 | 56.488 | 0.58x |
| mixed.json | json | 1.173 | 1.235 | 1.463 | 56.488 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 22.536 | 23.498 | 30.141 | 59.609 | 1.00x |
| users.ndjson | orjson | 31.736 | 32.642 | 42.055 | 59.609 | 0.72x |
| users.ndjson | msgspec | 32.717 | 34.958 | 40.412 | 59.609 | 0.67x |
| users.ndjson | ujson | 46.523 | 47.201 | 58.201 | 59.609 | 0.50x |
| users.ndjson | json | 60.088 | 62.912 | 79.929 | 59.609 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.429 | 5.707 | 6.322 | 58.023 | 1.00x |
| users.json | orjson | 5.290 | 5.835 | 8.080 | 58.023 | 0.98x |
| users.json | msgspec | 7.035 | 7.852 | 10.456 | 58.023 | 0.73x |
| users.json | ujson | 36.869 | 44.629 | 53.229 | 58.023 | 0.13x |
| users.json | json | 48.365 | 53.697 | 71.226 | 58.023 | 0.11x |
| flat.json | strata | 0.755 | 0.793 | 0.936 | 47.391 | 1.00x |
| flat.json | orjson | 0.921 | 0.968 | 1.201 | 47.391 | 0.82x |
| flat.json | msgspec | 1.037 | 1.154 | 1.640 | 47.391 | 0.69x |
| flat.json | ujson | 3.392 | 3.522 | 4.058 | 47.391 | 0.23x |
| flat.json | json | 4.559 | 4.643 | 5.743 | 47.391 | 0.17x |
| nested.json | strata | 0.680 | 0.760 | 1.002 | 47.363 | 1.00x |
| nested.json | orjson | 0.842 | 0.912 | 1.150 | 47.363 | 0.83x |
| nested.json | msgspec | 1.091 | 1.152 | 1.472 | 47.363 | 0.66x |
| nested.json | ujson | 3.549 | 3.761 | 4.542 | 47.363 | 0.20x |
| nested.json | json | 5.567 | 5.735 | 6.661 | 47.363 | 0.13x |
| wide_arrays.json | strata | 3.084 | 3.558 | 3.863 | 60.789 | 1.00x |
| wide_arrays.json | orjson | 3.684 | 4.234 | 4.972 | 60.789 | 0.84x |
| wide_arrays.json | msgspec | 4.623 | 5.264 | 5.665 | 60.789 | 0.68x |
| wide_arrays.json | ujson | 18.117 | 18.742 | 23.123 | 60.789 | 0.19x |
| wide_arrays.json | json | 39.047 | 41.055 | 48.218 | 60.789 | 0.09x |
| mixed.json | strata | 0.403 | 0.468 | 0.544 | 56.488 | 1.00x |
| mixed.json | orjson | 0.445 | 0.502 | 0.536 | 56.488 | 0.93x |
| mixed.json | msgspec | 0.493 | 0.539 | 0.645 | 56.488 | 0.87x |
| mixed.json | ujson | 1.095 | 1.175 | 3.220 | 56.488 | 0.40x |
| mixed.json | json | 1.522 | 1.584 | 2.086 | 56.488 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.206 | 0.231 | 0.346 | 58.105 | 1.00x |
| users.json $[*].id | jmespath | 1.132 | 1.197 | 1.333 | 58.105 | 0.19x |
| users.json $[*].id | jsonpath-ng | 6.091 | 6.653 | 7.705 | 58.105 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.220 | 1.487 | 2.080 | 55.270 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.911 | 7.344 | 8.575 | 55.270 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 40.544 | 43.869 | 53.983 | 55.270 | 0.03x |
| users.json $..total | strata | 3.768 | 4.015 | 5.868 | 56.238 | 1.00x |
| users.json $..total | jsonpath-ng | 817.281 | 835.553 | 883.230 | 56.238 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.552 | 4.800 | 5.839 | 58.156 | 1.00x |
| users.json $[*].id | orjson+jmespath | 32.250 | 33.406 | 42.506 | 58.156 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 36.980 | 39.984 | 44.553 | 58.156 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.621 | 4.729 | 5.021 | 56.191 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 35.392 | 37.862 | 42.817 | 56.191 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 84.266 | 86.806 | 100.818 | 56.191 | 0.05x |
| users.json $..total | strata | 25.118 | 25.686 | 34.511 | 56.270 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 833.331 | 866.399 | 895.252 | 56.270 | 0.03x |

