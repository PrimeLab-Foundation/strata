# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9108337
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.025 | 8.776 | 16.249 | 45.453 | 1.00x |
| users.json | orjson | 11.571 | 12.149 | 19.337 | 45.453 | 0.72x |
| users.json | msgspec | 10.076 | 10.822 | 19.521 | 45.453 | 0.81x |
| users.json | ujson | 17.294 | 19.591 | 26.139 | 45.453 | 0.45x |
| users.json | json | 17.440 | 18.653 | 29.543 | 45.453 | 0.47x |
| flat.json | strata | 0.771 | 0.841 | 1.208 | 48.156 | 1.00x |
| flat.json | orjson | 1.137 | 1.208 | 1.638 | 48.156 | 0.70x |
| flat.json | msgspec | 0.897 | 0.948 | 1.012 | 48.156 | 0.89x |
| flat.json | ujson | 1.923 | 2.048 | 2.383 | 48.156 | 0.41x |
| flat.json | json | 1.521 | 1.642 | 2.651 | 48.156 | 0.51x |
| nested.json | strata | 0.639 | 1.059 | 1.340 | 47.547 | 1.00x |
| nested.json | orjson | 0.886 | 1.342 | 2.045 | 47.547 | 0.79x |
| nested.json | msgspec | 0.778 | 1.252 | 1.838 | 47.547 | 0.85x |
| nested.json | ujson | 1.403 | 2.119 | 2.543 | 47.547 | 0.50x |
| nested.json | json | 1.715 | 2.685 | 3.186 | 47.547 | 0.39x |
| wide_arrays.json | strata | 6.263 | 6.956 | 9.795 | 49.605 | 1.00x |
| wide_arrays.json | orjson | 7.449 | 8.631 | 11.029 | 49.605 | 0.81x |
| wide_arrays.json | msgspec | 5.774 | 8.279 | 46.402 | 49.605 | 0.84x |
| wide_arrays.json | ujson | 7.726 | 11.410 | 19.639 | 49.605 | 0.61x |
| wide_arrays.json | json | 10.980 | 16.348 | 59.632 | 49.605 | 0.43x |
| mixed.json | strata | 0.150 | 0.267 | 2.005 | 49.250 | 1.00x |
| mixed.json | orjson | 0.178 | 0.273 | 0.314 | 49.250 | 0.98x |
| mixed.json | msgspec | 0.190 | 0.315 | 0.460 | 49.250 | 0.85x |
| mixed.json | ujson | 0.306 | 0.462 | 0.729 | 49.250 | 0.58x |
| mixed.json | json | 0.371 | 0.676 | 1.308 | 49.250 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.588 | 2.675 | 2.889 | 44.816 | 1.00x |
| users.json | orjson | 2.700 | 2.812 | 3.436 | 44.816 | 0.95x |
| users.json | msgspec | 4.383 | 4.516 | 6.738 | 44.816 | 0.59x |
| users.json | ujson | 12.551 | 12.775 | 16.294 | 44.816 | 0.21x |
| users.json | json | 18.141 | 18.588 | 24.707 | 44.816 | 0.14x |
| flat.json | strata | 0.270 | 0.288 | 1.669 | 47.898 | 1.00x |
| flat.json | orjson | 0.274 | 0.325 | 0.353 | 47.898 | 0.89x |
| flat.json | msgspec | 0.441 | 0.479 | 0.742 | 47.898 | 0.60x |
| flat.json | ujson | 1.321 | 1.425 | 1.753 | 47.898 | 0.20x |
| flat.json | json | 1.548 | 1.803 | 2.017 | 47.898 | 0.16x |
| nested.json | strata | 0.324 | 0.337 | 0.387 | 47.957 | 1.00x |
| nested.json | orjson | 0.349 | 0.365 | 0.554 | 47.957 | 0.92x |
| nested.json | msgspec | 0.569 | 0.589 | 0.720 | 47.957 | 0.57x |
| nested.json | ujson | 1.604 | 1.722 | 47.799 | 47.957 | 0.20x |
| nested.json | json | 3.089 | 3.411 | 3.980 | 47.957 | 0.10x |
| wide_arrays.json | strata | 1.795 | 2.659 | 3.493 | 50.492 | 1.00x |
| wide_arrays.json | orjson | 1.876 | 2.778 | 38.895 | 50.492 | 0.96x |
| wide_arrays.json | msgspec | 3.423 | 4.572 | 5.602 | 50.492 | 0.58x |
| wide_arrays.json | ujson | 7.644 | 11.578 | 16.281 | 50.492 | 0.23x |
| wide_arrays.json | json | 14.126 | 24.115 | 28.686 | 50.492 | 0.11x |
| mixed.json | strata | 0.064 | 0.081 | 0.111 | 47.625 | 1.00x |
| mixed.json | orjson | 0.051 | 0.074 | 0.107 | 47.625 | 1.10x |
| mixed.json | msgspec | 0.078 | 0.114 | 1.486 | 47.625 | 0.71x |
| mixed.json | ujson | 0.254 | 0.351 | 0.527 | 47.625 | 0.23x |
| mixed.json | json | 0.414 | 0.672 | 0.733 | 47.625 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.887 | 10.227 | 11.022 | 54.848 | 1.00x |
| users.json | orjson | 12.288 | 13.029 | 13.460 | 54.848 | 0.78x |
| users.json | msgspec | 11.281 | 12.167 | 16.585 | 54.848 | 0.84x |
| users.json | ujson | 20.871 | 22.521 | 24.799 | 54.848 | 0.45x |
| users.json | json | 18.486 | 19.161 | 24.002 | 54.848 | 0.53x |
| flat.json | strata | 1.091 | 1.172 | 1.411 | 47.723 | 1.00x |
| flat.json | orjson | 1.351 | 1.437 | 1.953 | 47.723 | 0.82x |
| flat.json | msgspec | 1.093 | 1.245 | 1.649 | 47.723 | 0.94x |
| flat.json | ujson | 2.280 | 2.371 | 2.622 | 47.723 | 0.49x |
| flat.json | json | 1.658 | 1.701 | 2.286 | 47.723 | 0.69x |
| nested.json | strata | 1.181 | 1.280 | 1.574 | 47.801 | 1.00x |
| nested.json | orjson | 1.549 | 1.670 | 1.953 | 47.801 | 0.77x |
| nested.json | msgspec | 1.029 | 1.725 | 2.535 | 47.801 | 0.74x |
| nested.json | ujson | 1.548 | 2.599 | 3.206 | 47.801 | 0.49x |
| nested.json | json | 1.725 | 3.132 | 3.875 | 47.801 | 0.41x |
| wide_arrays.json | strata | 6.516 | 6.988 | 7.559 | 50.492 | 1.00x |
| wide_arrays.json | orjson | 7.738 | 8.427 | 11.342 | 50.492 | 0.83x |
| wide_arrays.json | msgspec | 8.000 | 8.680 | 10.275 | 50.492 | 0.81x |
| wide_arrays.json | ujson | 12.717 | 14.290 | 18.668 | 50.492 | 0.49x |
| wide_arrays.json | json | 15.189 | 17.036 | 21.382 | 50.492 | 0.41x |
| mixed.json | strata | 0.215 | 0.349 | 0.510 | 47.637 | 1.00x |
| mixed.json | orjson | 0.252 | 0.406 | 0.476 | 47.637 | 0.86x |
| mixed.json | msgspec | 0.273 | 0.377 | 1.093 | 47.637 | 0.93x |
| mixed.json | ujson | 0.412 | 0.606 | 0.859 | 47.637 | 0.58x |
| mixed.json | json | 0.447 | 0.598 | 3.514 | 47.637 | 0.58x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.578 | 13.216 | 14.417 | 49.098 | 1.00x |
| users.ndjson | orjson | 17.506 | 19.310 | 28.723 | 49.098 | 0.68x |
| users.ndjson | msgspec | 15.089 | 17.827 | 34.717 | 49.098 | 0.74x |
| users.ndjson | ujson | 23.807 | 26.989 | 30.079 | 49.098 | 0.49x |
| users.ndjson | json | 27.360 | 30.416 | 51.447 | 49.098 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.450 | 3.594 | 8.334 | 50.762 | 1.00x |
| users.json | orjson | 3.521 | 3.805 | 4.796 | 50.762 | 0.94x |
| users.json | msgspec | 5.095 | 5.578 | 9.417 | 50.762 | 0.64x |
| users.json | ujson | 20.314 | 20.809 | 29.716 | 50.762 | 0.17x |
| users.json | json | 25.665 | 26.644 | 28.517 | 50.762 | 0.13x |
| flat.json | strata | 0.610 | 0.720 | 3.686 | 48.086 | 1.00x |
| flat.json | orjson | 0.604 | 0.683 | 2.010 | 48.086 | 1.05x |
| flat.json | msgspec | 0.776 | 0.828 | 2.223 | 48.086 | 0.87x |
| flat.json | ujson | 2.437 | 2.530 | 5.109 | 48.086 | 0.28x |
| flat.json | json | 2.758 | 3.002 | 5.246 | 48.086 | 0.24x |
| nested.json | strata | 0.521 | 0.882 | 2.974 | 48.086 | 1.00x |
| nested.json | orjson | 0.541 | 0.957 | 4.345 | 48.086 | 0.92x |
| nested.json | msgspec | 0.753 | 0.906 | 3.080 | 48.086 | 0.97x |
| nested.json | ujson | 1.954 | 3.164 | 7.457 | 48.086 | 0.28x |
| nested.json | json | 2.783 | 4.868 | 13.273 | 48.086 | 0.18x |
| wide_arrays.json | strata | 3.666 | 3.914 | 7.049 | 50.391 | 1.00x |
| wide_arrays.json | orjson | 3.287 | 3.996 | 7.963 | 50.391 | 0.98x |
| wide_arrays.json | msgspec | 5.486 | 6.231 | 9.625 | 50.391 | 0.63x |
| wide_arrays.json | ujson | 21.532 | 23.428 | 25.665 | 50.391 | 0.17x |
| wide_arrays.json | json | 21.274 | 34.715 | 38.661 | 50.391 | 0.11x |
| mixed.json | strata | 0.322 | 0.421 | 0.780 | 47.781 | 1.00x |
| mixed.json | orjson | 0.304 | 0.428 | 0.501 | 47.781 | 0.98x |
| mixed.json | msgspec | 0.339 | 0.479 | 0.972 | 47.781 | 0.88x |
| mixed.json | ujson | 0.648 | 1.048 | 2.338 | 47.781 | 0.40x |
| mixed.json | json | 0.799 | 1.504 | 2.591 | 47.781 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.079 | 0.084 | 0.094 | 50.777 | 1.00x |
| users.json $[*].id | jmespath | 0.329 | 0.359 | 0.372 | 50.777 | 0.24x |
| users.json $[*].id | jsonpath-ng | 1.934 | 2.057 | 2.364 | 50.777 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.693 | 0.736 | 0.965 | 50.789 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.514 | 3.837 | 7.756 | 50.789 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 22.910 | 25.073 | 60.917 | 50.789 | 0.03x |
| users.json $..total | strata | 1.467 | 1.585 | 3.725 | 52.789 | 1.00x |
| users.json $..total | jsonpath-ng | 262.843 | 281.171 | 348.620 | 52.789 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.458 | 4.741 | 9.760 | 50.789 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.575 | 19.486 | 21.895 | 50.789 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 14.478 | 22.964 | 25.443 | 50.789 | 0.21x |
| users.json $[*].orders[*].total | strata | 3.644 | 3.769 | 4.217 | 52.414 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.076 | 15.904 | 23.621 | 52.414 | 0.24x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 29.110 | 30.967 | 54.408 | 52.414 | 0.12x |
| users.json $..total | strata | 11.940 | 13.615 | 23.935 | 52.414 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 284.167 | 344.306 | 481.794 | 52.414 | 0.04x |

