# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 79fa3df
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: Intel64 Family 6 Model 173 Stepping 1, GenuineIntel
- compiler_flags: clang-cl /std:c++20 /O2 /arch:AVX2 -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.519 | 6.912 | 10.716 | 45.629 | 1.00x |
| users.json | orjson | 12.643 | 12.931 | 15.362 | 45.629 | 0.53x |
| users.json | msgspec | 11.215 | 11.589 | 13.987 | 45.629 | 0.60x |
| users.json | ujson | 16.040 | 16.586 | 21.012 | 45.629 | 0.42x |
| users.json | json | 18.431 | 18.799 | 20.758 | 45.629 | 0.37x |
| flat.json | strata | 0.772 | 0.798 | 0.866 | 52.203 | 1.00x |
| flat.json | orjson | 1.053 | 1.081 | 1.155 | 52.203 | 0.74x |
| flat.json | msgspec | 0.900 | 0.949 | 0.981 | 52.203 | 0.84x |
| flat.json | ujson | 1.360 | 1.435 | 1.511 | 52.203 | 0.56x |
| flat.json | json | 1.655 | 1.686 | 1.720 | 52.203 | 0.47x |
| nested.json | strata | 0.520 | 0.538 | 0.835 | 52.258 | 1.00x |
| nested.json | orjson | 0.837 | 0.858 | 1.267 | 52.258 | 0.63x |
| nested.json | msgspec | 0.673 | 0.707 | 1.058 | 52.258 | 0.76x |
| nested.json | ujson | 0.993 | 1.080 | 1.722 | 52.258 | 0.50x |
| nested.json | json | 1.496 | 1.574 | 1.616 | 52.258 | 0.34x |
| wide_arrays.json | strata | 2.972 | 3.194 | 3.937 | 54.273 | 1.00x |
| wide_arrays.json | orjson | 5.349 | 5.558 | 7.153 | 54.273 | 0.57x |
| wide_arrays.json | msgspec | 4.936 | 5.060 | 6.149 | 54.273 | 0.63x |
| wide_arrays.json | ujson | 6.603 | 6.846 | 8.204 | 54.273 | 0.47x |
| wide_arrays.json | json | 9.125 | 9.610 | 10.722 | 54.273 | 0.33x |
| mixed.json | strata | 0.142 | 0.151 | 0.202 | 53.516 | 1.00x |
| mixed.json | orjson | 0.171 | 0.173 | 0.210 | 53.516 | 0.88x |
| mixed.json | msgspec | 0.192 | 0.197 | 0.248 | 53.516 | 0.77x |
| mixed.json | ujson | 0.248 | 0.256 | 0.324 | 53.516 | 0.59x |
| mixed.json | json | 0.374 | 0.384 | 0.448 | 53.516 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.254 | 2.356 | 2.443 | 44.895 | 1.00x |
| users.json | orjson | 2.976 | 3.104 | 3.483 | 44.895 | 0.76x |
| users.json | msgspec | 5.140 | 5.261 | 5.422 | 44.895 | 0.45x |
| users.json | ujson | 10.361 | 10.807 | 13.665 | 44.895 | 0.22x |
| users.json | json | 18.166 | 19.078 | 19.824 | 44.895 | 0.12x |
| flat.json | strata | 0.270 | 0.329 | 0.425 | 52.328 | 1.00x |
| flat.json | orjson | 0.267 | 0.305 | 0.372 | 52.328 | 1.08x |
| flat.json | msgspec | 0.426 | 0.480 | 0.632 | 52.328 | 0.68x |
| flat.json | ujson | 0.992 | 1.073 | 1.251 | 52.328 | 0.31x |
| flat.json | json | 1.711 | 1.779 | 2.706 | 52.328 | 0.18x |
| nested.json | strata | 0.184 | 0.199 | 0.208 | 52.594 | 1.00x |
| nested.json | orjson | 0.242 | 0.274 | 0.327 | 52.594 | 0.72x |
| nested.json | msgspec | 0.369 | 0.397 | 0.462 | 52.594 | 0.50x |
| nested.json | ujson | 0.747 | 0.807 | 0.858 | 52.594 | 0.25x |
| nested.json | json | 1.852 | 1.893 | 1.962 | 52.594 | 0.10x |
| wide_arrays.json | strata | 1.562 | 1.636 | 2.738 | 53.492 | 1.00x |
| wide_arrays.json | orjson | 2.329 | 2.374 | 3.204 | 53.492 | 0.69x |
| wide_arrays.json | msgspec | 3.825 | 3.908 | 4.963 | 53.492 | 0.42x |
| wide_arrays.json | ujson | 6.267 | 6.454 | 6.523 | 53.492 | 0.25x |
| wide_arrays.json | json | 13.957 | 14.382 | 14.758 | 53.492 | 0.11x |
| mixed.json | strata | 0.060 | 0.061 | 0.091 | 53.645 | 1.00x |
| mixed.json | orjson | 0.057 | 0.063 | 0.102 | 53.645 | 0.98x |
| mixed.json | msgspec | 0.088 | 0.098 | 0.166 | 53.645 | 0.62x |
| mixed.json | ujson | 0.180 | 0.191 | 0.257 | 53.645 | 0.32x |
| mixed.json | json | 0.421 | 0.442 | 0.699 | 53.645 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.466 | 8.042 | 11.707 | 54.918 | 1.00x |
| users.json | orjson | 13.279 | 13.812 | 15.318 | 54.918 | 0.58x |
| users.json | msgspec | 12.263 | 12.798 | 13.775 | 54.918 | 0.63x |
| users.json | ujson | 19.544 | 20.367 | 23.360 | 54.918 | 0.39x |
| users.json | json | 19.086 | 19.850 | 21.946 | 54.918 | 0.41x |
| flat.json | strata | 0.956 | 0.973 | 1.079 | 52.496 | 1.00x |
| flat.json | orjson | 1.219 | 1.290 | 1.379 | 52.496 | 0.75x |
| flat.json | msgspec | 1.033 | 1.118 | 1.347 | 52.496 | 0.87x |
| flat.json | ujson | 1.810 | 1.879 | 1.991 | 52.496 | 0.52x |
| flat.json | json | 1.730 | 1.863 | 1.890 | 52.496 | 0.52x |
| nested.json | strata | 0.599 | 0.625 | 0.726 | 52.602 | 1.00x |
| nested.json | orjson | 0.966 | 1.016 | 1.063 | 52.602 | 0.61x |
| nested.json | msgspec | 0.800 | 0.847 | 0.891 | 52.602 | 0.74x |
| nested.json | ujson | 1.327 | 1.361 | 1.508 | 52.602 | 0.46x |
| nested.json | json | 1.591 | 1.702 | 1.717 | 52.602 | 0.37x |
| wide_arrays.json | strata | 3.719 | 3.871 | 4.060 | 53.492 | 1.00x |
| wide_arrays.json | orjson | 5.973 | 6.135 | 8.078 | 53.492 | 0.63x |
| wide_arrays.json | msgspec | 5.610 | 5.805 | 8.089 | 53.492 | 0.67x |
| wide_arrays.json | ujson | 8.931 | 9.142 | 9.287 | 53.492 | 0.42x |
| wide_arrays.json | json | 10.090 | 10.299 | 11.608 | 53.492 | 0.38x |
| mixed.json | strata | 0.215 | 0.219 | 0.273 | 53.672 | 1.00x |
| mixed.json | orjson | 0.275 | 0.282 | 0.338 | 53.672 | 0.78x |
| mixed.json | msgspec | 0.294 | 0.302 | 0.406 | 53.672 | 0.73x |
| mixed.json | ujson | 0.391 | 0.410 | 0.454 | 53.672 | 0.53x |
| mixed.json | json | 0.468 | 0.481 | 0.532 | 53.672 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.915 | 8.048 | 8.286 | 53.379 | 1.00x |
| users.ndjson | orjson | 14.355 | 14.664 | 15.810 | 53.379 | 0.55x |
| users.ndjson | msgspec | 14.417 | 14.663 | 15.620 | 53.379 | 0.55x |
| users.ndjson | ujson | 18.579 | 18.947 | 21.014 | 53.379 | 0.42x |
| users.ndjson | json | 22.943 | 23.739 | 30.539 | 53.379 | 0.34x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.284 | 3.389 | 4.637 | 54.098 | 1.00x |
| users.json | orjson | 3.788 | 4.031 | 4.109 | 54.098 | 0.84x |
| users.json | msgspec | 5.414 | 5.672 | 5.913 | 54.098 | 0.60x |
| users.json | ujson | 16.613 | 16.986 | 17.373 | 54.098 | 0.20x |
| users.json | json | 24.595 | 25.200 | 27.664 | 54.098 | 0.13x |
| flat.json | strata | 0.569 | 0.603 | 0.698 | 52.695 | 1.00x |
| flat.json | orjson | 0.570 | 0.649 | 0.843 | 52.695 | 0.93x |
| flat.json | msgspec | 0.749 | 0.804 | 1.036 | 52.695 | 0.75x |
| flat.json | ujson | 1.848 | 1.949 | 2.062 | 52.695 | 0.31x |
| flat.json | json | 2.453 | 2.589 | 2.774 | 52.695 | 0.23x |
| nested.json | strata | 0.482 | 0.507 | 0.693 | 52.688 | 1.00x |
| nested.json | orjson | 0.581 | 0.607 | 0.715 | 52.688 | 0.84x |
| nested.json | msgspec | 0.687 | 0.734 | 0.798 | 52.688 | 0.69x |
| nested.json | ujson | 1.570 | 1.625 | 1.664 | 52.688 | 0.31x |
| nested.json | json | 2.620 | 2.654 | 4.254 | 52.688 | 0.19x |
| wide_arrays.json | strata | 2.247 | 2.338 | 2.457 | 54.656 | 1.00x |
| wide_arrays.json | orjson | 2.710 | 2.845 | 3.217 | 54.656 | 0.82x |
| wide_arrays.json | msgspec | 4.158 | 4.603 | 5.017 | 54.656 | 0.51x |
| wide_arrays.json | ujson | 11.021 | 11.398 | 14.208 | 54.656 | 0.21x |
| wide_arrays.json | json | 18.101 | 18.671 | 19.199 | 54.656 | 0.13x |
| mixed.json | strata | 0.321 | 0.328 | 0.394 | 52.859 | 1.00x |
| mixed.json | orjson | 0.320 | 0.331 | 0.412 | 52.859 | 0.99x |
| mixed.json | msgspec | 0.353 | 0.412 | 0.520 | 52.859 | 0.80x |
| mixed.json | ujson | 0.556 | 0.584 | 0.664 | 52.859 | 0.56x |
| mixed.json | json | 0.809 | 0.867 | 0.940 | 52.859 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.067 | 0.071 | 0.096 | 52.473 | 1.00x |
| users.json $[*].id | jmespath | 0.322 | 0.339 | 0.522 | 52.473 | 0.21x |
| users.json $[*].id | jsonpath-ng | 1.734 | 1.772 | 2.637 | 52.473 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.389 | 0.455 | 0.618 | 52.984 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.954 | 2.050 | 2.158 | 52.984 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.536 | 12.146 | 13.549 | 52.984 | 0.04x |
| users.json $..total | strata | 1.555 | 1.604 | 2.447 | 53.047 | 1.00x |
| users.json $..total | jsonpath-ng | 229.172 | 234.448 | 243.721 | 53.047 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.568 | 3.647 | 3.792 | 52.582 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.768 | 14.207 | 15.956 | 52.582 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 15.300 | 15.650 | 15.954 | 52.582 | 0.23x |
| users.json $[*].orders[*].total | strata | 3.772 | 3.861 | 5.185 | 53.008 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.660 | 15.912 | 16.247 | 53.008 | 0.24x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 30.757 | 31.012 | 32.758 | 53.008 | 0.12x |
| users.json $..total | strata | 9.486 | 10.855 | 12.292 | 53.062 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 248.378 | 254.391 | 264.508 | 53.062 | 0.04x |

