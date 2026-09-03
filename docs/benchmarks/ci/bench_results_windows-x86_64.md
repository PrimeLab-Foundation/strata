# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 37a96fb
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
| users.json | strata | 15.804 | 19.777 | 46.969 | 45.410 | 1.00x |
| users.json | orjson | 21.373 | 23.237 | 43.551 | 45.410 | 0.85x |
| users.json | msgspec | 20.219 | 21.198 | 96.849 | 45.410 | 0.93x |
| users.json | ujson | 32.329 | 50.093 | 106.091 | 45.410 | 0.39x |
| users.json | json | 37.088 | 45.282 | 62.279 | 45.410 | 0.44x |
| flat.json | strata | 1.056 | 1.086 | 1.150 | 49.797 | 1.00x |
| flat.json | orjson | 1.196 | 1.247 | 1.304 | 49.797 | 0.87x |
| flat.json | msgspec | 1.095 | 1.132 | 1.808 | 49.797 | 0.96x |
| flat.json | ujson | 2.167 | 2.218 | 2.599 | 49.797 | 0.49x |
| flat.json | json | 1.930 | 1.938 | 2.096 | 49.797 | 0.56x |
| nested.json | strata | 0.797 | 0.841 | 1.387 | 49.965 | 1.00x |
| nested.json | orjson | 1.088 | 1.141 | 1.960 | 49.965 | 0.74x |
| nested.json | msgspec | 0.970 | 1.007 | 1.665 | 49.965 | 0.84x |
| nested.json | ujson | 1.543 | 1.662 | 2.859 | 49.965 | 0.51x |
| nested.json | json | 2.075 | 2.089 | 3.408 | 49.965 | 0.40x |
| wide_arrays.json | strata | 4.828 | 4.884 | 4.928 | 51.574 | 1.00x |
| wide_arrays.json | orjson | 5.892 | 5.955 | 8.259 | 51.574 | 0.82x |
| wide_arrays.json | msgspec | 5.910 | 5.945 | 9.689 | 51.574 | 0.82x |
| wide_arrays.json | ujson | 8.042 | 8.226 | 12.883 | 51.574 | 0.59x |
| wide_arrays.json | json | 11.365 | 11.482 | 15.458 | 51.574 | 0.43x |
| mixed.json | strata | 0.196 | 0.199 | 0.224 | 49.316 | 1.00x |
| mixed.json | orjson | 0.294 | 0.312 | 0.342 | 49.316 | 0.64x |
| mixed.json | msgspec | 0.228 | 0.233 | 0.272 | 49.316 | 0.86x |
| mixed.json | ujson | 0.370 | 0.379 | 0.461 | 49.316 | 0.53x |
| mixed.json | json | 0.458 | 0.463 | 0.494 | 49.316 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.715 | 4.883 | 6.462 | 44.641 | 1.00x |
| users.json | orjson | 5.696 | 5.828 | 7.474 | 44.641 | 0.84x |
| users.json | msgspec | 8.519 | 8.746 | 42.407 | 44.641 | 0.56x |
| users.json | ujson | 24.852 | 26.500 | 85.013 | 44.641 | 0.18x |
| users.json | json | 40.755 | 51.114 | 105.687 | 44.641 | 0.10x |
| flat.json | strata | 0.332 | 0.352 | 0.530 | 50.348 | 1.00x |
| flat.json | orjson | 0.372 | 0.385 | 0.614 | 50.348 | 0.92x |
| flat.json | msgspec | 0.537 | 0.582 | 0.770 | 50.348 | 0.61x |
| flat.json | ujson | 1.603 | 1.723 | 2.611 | 50.348 | 0.20x |
| flat.json | json | 1.973 | 2.018 | 3.453 | 50.348 | 0.17x |
| nested.json | strata | 0.282 | 0.289 | 0.320 | 49.934 | 1.00x |
| nested.json | orjson | 0.320 | 0.324 | 0.427 | 49.934 | 0.89x |
| nested.json | msgspec | 0.505 | 0.518 | 0.551 | 49.934 | 0.56x |
| nested.json | ujson | 1.294 | 1.332 | 1.361 | 49.934 | 0.22x |
| nested.json | json | 2.487 | 2.503 | 2.560 | 49.934 | 0.12x |
| wide_arrays.json | strata | 2.353 | 2.391 | 2.461 | 50.395 | 1.00x |
| wide_arrays.json | orjson | 2.454 | 2.622 | 2.777 | 50.395 | 0.91x |
| wide_arrays.json | msgspec | 4.154 | 4.274 | 4.483 | 50.395 | 0.56x |
| wide_arrays.json | ujson | 9.972 | 10.061 | 14.860 | 50.395 | 0.24x |
| wide_arrays.json | json | 18.208 | 18.474 | 28.781 | 50.395 | 0.13x |
| mixed.json | strata | 0.077 | 0.082 | 0.118 | 49.500 | 1.00x |
| mixed.json | orjson | 0.071 | 0.074 | 0.130 | 49.500 | 1.11x |
| mixed.json | msgspec | 0.097 | 0.100 | 0.160 | 49.500 | 0.83x |
| mixed.json | ujson | 0.326 | 0.334 | 0.509 | 49.500 | 0.25x |
| mixed.json | json | 0.517 | 0.547 | 0.979 | 49.500 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.468 | 20.031 | 27.040 | 54.668 | 1.00x |
| users.json | orjson | 23.756 | 28.688 | 47.361 | 54.668 | 0.70x |
| users.json | msgspec | 21.988 | 26.943 | 31.753 | 54.668 | 0.74x |
| users.json | ujson | 49.923 | 72.085 | 104.278 | 54.668 | 0.28x |
| users.json | json | 38.589 | 46.154 | 55.457 | 54.668 | 0.43x |
| flat.json | strata | 1.209 | 1.291 | 1.351 | 49.715 | 1.00x |
| flat.json | orjson | 1.370 | 1.397 | 1.428 | 49.715 | 0.92x |
| flat.json | msgspec | 1.245 | 1.344 | 1.437 | 49.715 | 0.96x |
| flat.json | ujson | 2.629 | 2.701 | 2.771 | 49.715 | 0.48x |
| flat.json | json | 2.175 | 2.189 | 2.335 | 49.715 | 0.59x |
| nested.json | strata | 0.891 | 0.940 | 0.966 | 50.086 | 1.00x |
| nested.json | orjson | 1.217 | 1.259 | 1.295 | 50.086 | 0.75x |
| nested.json | msgspec | 1.095 | 1.112 | 1.177 | 50.086 | 0.85x |
| nested.json | ujson | 1.823 | 1.883 | 2.002 | 50.086 | 0.50x |
| nested.json | json | 2.194 | 2.218 | 2.356 | 50.086 | 0.42x |
| wide_arrays.json | strata | 5.250 | 5.327 | 8.368 | 50.398 | 1.00x |
| wide_arrays.json | orjson | 6.273 | 6.349 | 21.354 | 50.398 | 0.84x |
| wide_arrays.json | msgspec | 6.383 | 6.484 | 11.080 | 50.398 | 0.82x |
| wide_arrays.json | ujson | 10.528 | 10.613 | 18.628 | 50.398 | 0.50x |
| wide_arrays.json | json | 11.756 | 11.889 | 13.430 | 50.398 | 0.45x |
| mixed.json | strata | 0.266 | 0.279 | 0.309 | 49.613 | 1.00x |
| mixed.json | orjson | 0.326 | 0.333 | 0.371 | 49.613 | 0.84x |
| mixed.json | msgspec | 0.336 | 0.345 | 0.409 | 49.613 | 0.81x |
| mixed.json | ujson | 0.513 | 0.532 | 0.562 | 49.613 | 0.53x |
| mixed.json | json | 0.572 | 0.585 | 0.720 | 49.613 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.333 | 11.742 | 18.951 | 50.742 | 1.00x |
| users.ndjson | orjson | 18.339 | 18.544 | 22.912 | 50.742 | 0.63x |
| users.ndjson | msgspec | 17.916 | 18.100 | 25.223 | 50.742 | 0.65x |
| users.ndjson | ujson | 25.683 | 26.293 | 34.535 | 50.742 | 0.45x |
| users.ndjson | json | 30.491 | 30.859 | 33.680 | 50.742 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.181 | 7.982 | 10.682 | 51.344 | 1.00x |
| users.json | orjson | 7.416 | 10.518 | 12.448 | 51.344 | 0.76x |
| users.json | msgspec | 9.321 | 13.006 | 31.027 | 51.344 | 0.61x |
| users.json | ujson | 54.348 | 58.219 | 61.942 | 51.344 | 0.14x |
| users.json | json | 77.912 | 85.371 | 143.270 | 51.344 | 0.09x |
| flat.json | strata | 0.692 | 0.739 | 0.781 | 50.023 | 1.00x |
| flat.json | orjson | 0.729 | 0.743 | 0.819 | 50.023 | 1.00x |
| flat.json | msgspec | 0.941 | 0.972 | 1.051 | 50.023 | 0.76x |
| flat.json | ujson | 3.040 | 3.090 | 3.193 | 50.023 | 0.24x |
| flat.json | json | 3.335 | 3.546 | 3.716 | 50.023 | 0.21x |
| nested.json | strata | 0.664 | 0.690 | 0.875 | 50.086 | 1.00x |
| nested.json | orjson | 0.686 | 0.722 | 0.950 | 50.086 | 0.96x |
| nested.json | msgspec | 0.876 | 0.942 | 1.218 | 50.086 | 0.73x |
| nested.json | ujson | 2.408 | 2.473 | 11.351 | 50.086 | 0.28x |
| nested.json | json | 3.582 | 3.608 | 3.828 | 50.086 | 0.19x |
| wide_arrays.json | strata | 3.073 | 3.131 | 3.906 | 50.395 | 1.00x |
| wide_arrays.json | orjson | 3.043 | 3.375 | 3.550 | 50.395 | 0.93x |
| wide_arrays.json | msgspec | 4.751 | 5.070 | 5.494 | 50.395 | 0.62x |
| wide_arrays.json | ujson | 16.612 | 16.904 | 20.429 | 50.395 | 0.19x |
| wide_arrays.json | json | 24.881 | 25.116 | 55.652 | 50.395 | 0.12x |
| mixed.json | strata | 0.383 | 0.406 | 0.434 | 49.641 | 1.00x |
| mixed.json | orjson | 0.378 | 0.384 | 0.400 | 49.641 | 1.06x |
| mixed.json | msgspec | 0.414 | 0.447 | 0.540 | 49.641 | 0.91x |
| mixed.json | ujson | 0.805 | 0.847 | 0.985 | 49.641 | 0.48x |
| mixed.json | json | 0.983 | 1.039 | 1.104 | 49.641 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.145 | 0.156 | 0.181 | 51.645 | 1.00x |
| users.json $[*].id | jmespath | 0.775 | 1.094 | 2.775 | 51.645 | 0.14x |
| users.json $[*].id | jsonpath-ng | 4.684 | 6.619 | 26.904 | 51.645 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.886 | 0.905 | 2.673 | 51.824 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 4.990 | 6.820 | 8.534 | 51.824 | 0.13x |
| users.json $[*].orders[*].total | jsonpath-ng | 38.144 | 41.941 | 86.290 | 51.824 | 0.02x |
| users.json $..total | strata | 3.273 | 3.324 | 5.199 | 52.453 | 1.00x |
| users.json $..total | jsonpath-ng | 619.273 | 752.981 | 1569.459 | 52.453 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 8.023 | 8.172 | 9.683 | 51.648 | 1.00x |
| users.json $[*].id | orjson+jmespath | 33.056 | 35.934 | 95.205 | 51.648 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 35.341 | 39.792 | 44.212 | 51.648 | 0.21x |
| users.json $[*].orders[*].total | strata | 6.768 | 8.612 | 10.590 | 52.449 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 29.738 | 40.780 | 94.956 | 52.449 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 58.816 | 82.099 | 86.984 | 52.449 | 0.10x |
| users.json $..total | strata | 14.673 | 15.262 | 26.086 | 52.453 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 347.933 | 371.247 | 685.829 | 52.453 | 0.04x |

