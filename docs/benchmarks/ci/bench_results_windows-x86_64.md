# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 060bd7b
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD
- compiler_flags: clang-cl /std:c++20 /O2 /arch:AVX2 -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.788 | 11.633 | 17.693 | 45.488 | 1.00x |
| users.json | orjson | 14.783 | 15.746 | 21.665 | 45.488 | 0.74x |
| users.json | msgspec | 13.202 | 13.746 | 15.799 | 45.488 | 0.85x |
| users.json | ujson | 22.552 | 23.699 | 26.590 | 45.488 | 0.49x |
| users.json | json | 23.351 | 24.301 | 30.069 | 45.488 | 0.48x |
| flat.json | strata | 0.908 | 1.005 | 1.440 | 51.668 | 1.00x |
| flat.json | orjson | 1.203 | 1.271 | 1.869 | 51.668 | 0.79x |
| flat.json | msgspec | 1.114 | 1.160 | 2.207 | 51.668 | 0.87x |
| flat.json | ujson | 2.246 | 2.318 | 3.495 | 51.668 | 0.43x |
| flat.json | json | 1.928 | 1.948 | 3.247 | 51.668 | 0.52x |
| nested.json | strata | 0.773 | 0.794 | 0.853 | 50.910 | 1.00x |
| nested.json | orjson | 1.125 | 1.159 | 1.216 | 50.910 | 0.69x |
| nested.json | msgspec | 0.975 | 1.004 | 1.037 | 50.910 | 0.79x |
| nested.json | ujson | 1.647 | 1.761 | 4.287 | 50.910 | 0.45x |
| nested.json | json | 2.107 | 2.148 | 2.674 | 50.910 | 0.37x |
| wide_arrays.json | strata | 5.767 | 6.206 | 8.269 | 52.699 | 1.00x |
| wide_arrays.json | orjson | 6.596 | 7.219 | 10.331 | 52.699 | 0.86x |
| wide_arrays.json | msgspec | 6.033 | 6.567 | 11.423 | 52.699 | 0.95x |
| wide_arrays.json | ujson | 8.293 | 9.196 | 14.975 | 52.699 | 0.67x |
| wide_arrays.json | json | 12.009 | 12.811 | 21.476 | 52.699 | 0.48x |
| mixed.json | strata | 0.292 | 0.306 | 0.327 | 50.711 | 1.00x |
| mixed.json | orjson | 0.291 | 0.354 | 0.380 | 50.711 | 0.86x |
| mixed.json | msgspec | 0.387 | 0.408 | 0.436 | 50.711 | 0.75x |
| mixed.json | ujson | 0.546 | 0.579 | 0.721 | 50.711 | 0.53x |
| mixed.json | json | 0.742 | 0.775 | 0.802 | 50.711 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.525 | 3.624 | 5.320 | 44.750 | 1.00x |
| users.json | orjson | 4.018 | 4.076 | 6.610 | 44.750 | 0.89x |
| users.json | msgspec | 5.990 | 6.200 | 8.885 | 44.750 | 0.58x |
| users.json | ujson | 16.507 | 16.696 | 18.457 | 44.750 | 0.22x |
| users.json | json | 23.499 | 24.018 | 24.523 | 44.750 | 0.15x |
| flat.json | strata | 0.336 | 0.386 | 0.476 | 51.066 | 1.00x |
| flat.json | orjson | 0.460 | 0.485 | 0.562 | 51.066 | 0.79x |
| flat.json | msgspec | 0.649 | 0.675 | 0.827 | 51.066 | 0.57x |
| flat.json | ujson | 1.777 | 1.806 | 1.924 | 51.066 | 0.21x |
| flat.json | json | 2.247 | 2.289 | 2.560 | 51.066 | 0.17x |
| nested.json | strata | 0.273 | 0.286 | 0.324 | 51.359 | 1.00x |
| nested.json | orjson | 0.324 | 0.333 | 0.365 | 51.359 | 0.86x |
| nested.json | msgspec | 0.517 | 0.535 | 0.582 | 51.359 | 0.53x |
| nested.json | ujson | 1.345 | 1.365 | 1.378 | 51.359 | 0.21x |
| nested.json | json | 2.478 | 2.519 | 3.141 | 51.359 | 0.11x |
| wide_arrays.json | strata | 3.013 | 3.184 | 4.515 | 52.293 | 1.00x |
| wide_arrays.json | orjson | 3.542 | 3.924 | 51.606 | 52.293 | 0.81x |
| wide_arrays.json | msgspec | 5.816 | 6.255 | 7.778 | 52.293 | 0.51x |
| wide_arrays.json | ujson | 14.393 | 15.045 | 62.571 | 52.293 | 0.21x |
| wide_arrays.json | json | 28.453 | 29.406 | 32.205 | 52.293 | 0.11x |
| mixed.json | strata | 0.103 | 0.110 | 0.117 | 50.664 | 1.00x |
| mixed.json | orjson | 0.100 | 0.109 | 0.140 | 50.664 | 1.01x |
| mixed.json | msgspec | 0.145 | 0.147 | 0.188 | 50.664 | 0.74x |
| mixed.json | ujson | 0.474 | 0.493 | 45.776 | 50.664 | 0.22x |
| mixed.json | json | 0.801 | 0.837 | 0.911 | 50.664 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.432 | 13.082 | 15.886 | 54.781 | 1.00x |
| users.json | orjson | 15.623 | 16.833 | 23.524 | 54.781 | 0.78x |
| users.json | msgspec | 14.474 | 15.492 | 23.106 | 54.781 | 0.84x |
| users.json | ujson | 27.689 | 28.798 | 33.873 | 54.781 | 0.45x |
| users.json | json | 24.027 | 25.087 | 43.463 | 54.781 | 0.52x |
| flat.json | strata | 1.138 | 1.215 | 1.267 | 51.090 | 1.00x |
| flat.json | orjson | 1.393 | 1.510 | 1.689 | 51.090 | 0.81x |
| flat.json | msgspec | 1.304 | 1.353 | 1.477 | 51.090 | 0.90x |
| flat.json | ujson | 2.813 | 2.876 | 2.945 | 51.090 | 0.42x |
| flat.json | json | 2.094 | 2.185 | 2.370 | 51.090 | 0.56x |
| nested.json | strata | 1.001 | 1.092 | 1.657 | 50.746 | 1.00x |
| nested.json | orjson | 1.440 | 1.589 | 2.706 | 50.746 | 0.69x |
| nested.json | msgspec | 1.281 | 1.351 | 1.890 | 50.746 | 0.81x |
| nested.json | ujson | 1.998 | 2.228 | 3.122 | 50.746 | 0.49x |
| nested.json | json | 2.301 | 2.454 | 3.824 | 50.746 | 0.44x |
| wide_arrays.json | strata | 6.819 | 8.336 | 16.201 | 52.293 | 1.00x |
| wide_arrays.json | orjson | 7.644 | 11.277 | 22.643 | 52.293 | 0.74x |
| wide_arrays.json | msgspec | 7.647 | 10.818 | 20.520 | 52.293 | 0.77x |
| wide_arrays.json | ujson | 12.542 | 16.974 | 20.131 | 52.293 | 0.49x |
| wide_arrays.json | json | 14.847 | 19.745 | 66.206 | 52.293 | 0.42x |
| mixed.json | strata | 0.287 | 0.419 | 0.437 | 50.715 | 1.00x |
| mixed.json | orjson | 0.374 | 0.539 | 0.614 | 50.715 | 0.78x |
| mixed.json | msgspec | 0.368 | 0.402 | 0.632 | 50.715 | 1.04x |
| mixed.json | ujson | 0.559 | 0.718 | 0.889 | 50.715 | 0.58x |
| mixed.json | json | 0.631 | 0.908 | 0.977 | 50.715 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 13.772 | 14.483 | 15.015 | 52.059 | 1.00x |
| users.ndjson | orjson | 20.318 | 20.999 | 21.815 | 52.059 | 0.69x |
| users.ndjson | msgspec | 19.938 | 20.622 | 21.542 | 52.059 | 0.70x |
| users.ndjson | ujson | 29.822 | 30.966 | 44.194 | 52.059 | 0.47x |
| users.ndjson | json | 33.149 | 33.958 | 35.773 | 52.059 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.492 | 4.566 | 4.929 | 52.832 | 1.00x |
| users.json | orjson | 5.052 | 5.300 | 6.075 | 52.832 | 0.86x |
| users.json | msgspec | 7.059 | 7.246 | 7.455 | 52.832 | 0.63x |
| users.json | ujson | 26.254 | 26.849 | 42.652 | 52.832 | 0.17x |
| users.json | json | 32.945 | 33.398 | 41.282 | 52.832 | 0.14x |
| flat.json | strata | 0.779 | 0.869 | 1.179 | 51.402 | 1.00x |
| flat.json | orjson | 0.831 | 0.867 | 1.272 | 51.402 | 1.00x |
| flat.json | msgspec | 1.053 | 1.112 | 1.506 | 51.402 | 0.78x |
| flat.json | ujson | 3.227 | 3.270 | 5.122 | 51.402 | 0.27x |
| flat.json | json | 3.751 | 3.934 | 6.267 | 51.402 | 0.22x |
| nested.json | strata | 0.806 | 0.891 | 3.855 | 51.121 | 1.00x |
| nested.json | orjson | 0.809 | 0.900 | 0.968 | 51.121 | 0.99x |
| nested.json | msgspec | 1.023 | 1.100 | 1.512 | 51.121 | 0.81x |
| nested.json | ujson | 2.502 | 2.659 | 4.097 | 51.121 | 0.33x |
| nested.json | json | 3.698 | 3.910 | 4.242 | 51.121 | 0.23x |
| wide_arrays.json | strata | 3.256 | 4.004 | 4.879 | 52.555 | 1.00x |
| wide_arrays.json | orjson | 3.636 | 4.613 | 5.917 | 52.555 | 0.87x |
| wide_arrays.json | msgspec | 5.606 | 6.561 | 8.222 | 52.555 | 0.61x |
| wide_arrays.json | ujson | 18.987 | 20.293 | 27.132 | 52.555 | 0.20x |
| wide_arrays.json | json | 31.314 | 32.758 | 90.939 | 52.555 | 0.12x |
| mixed.json | strata | 0.559 | 0.602 | 0.960 | 50.973 | 1.00x |
| mixed.json | orjson | 0.610 | 0.648 | 1.818 | 50.973 | 0.93x |
| mixed.json | msgspec | 0.646 | 0.747 | 0.932 | 50.973 | 0.81x |
| mixed.json | ujson | 1.285 | 1.458 | 2.562 | 50.973 | 0.41x |
| mixed.json | json | 1.304 | 1.793 | 4.356 | 50.973 | 0.34x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.099 | 0.110 | 0.145 | 52.863 | 1.00x |
| users.json $[*].id | jmespath | 0.443 | 0.469 | 0.514 | 52.863 | 0.24x |
| users.json $[*].id | jsonpath-ng | 2.454 | 2.810 | 2.904 | 52.863 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.551 | 0.593 | 1.198 | 52.887 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.785 | 2.971 | 5.136 | 52.887 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.688 | 19.616 | 22.735 | 52.887 | 0.03x |
| users.json $..total | strata | 2.071 | 2.118 | 2.321 | 53.887 | 1.00x |
| users.json $..total | jsonpath-ng | 323.846 | 343.054 | 345.961 | 53.887 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.288 | 4.372 | 4.487 | 52.887 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.429 | 16.990 | 17.821 | 52.887 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 18.708 | 19.667 | 28.457 | 52.887 | 0.22x |
| users.json $[*].orders[*].total | strata | 4.578 | 4.638 | 4.797 | 53.887 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 20.228 | 21.865 | 28.199 | 53.887 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 41.324 | 43.854 | 56.255 | 53.887 | 0.11x |
| users.json $..total | strata | 16.873 | 18.256 | 20.230 | 53.887 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 350.731 | 367.472 | 394.213 | 53.887 | 0.05x |

