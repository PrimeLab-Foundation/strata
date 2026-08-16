# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9c9d784
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.353 | 11.092 | 17.063 | 45.469 | 1.00x |
| users.json | orjson | 12.950 | 13.354 | 20.723 | 45.469 | 0.83x |
| users.json | msgspec | 12.452 | 12.746 | 23.093 | 45.469 | 0.87x |
| users.json | ujson | 20.517 | 23.266 | 31.941 | 45.469 | 0.48x |
| users.json | json | 21.957 | 22.790 | 41.220 | 45.469 | 0.49x |
| flat.json | strata | 1.642 | 1.774 | 3.946 | 49.695 | 1.00x |
| flat.json | orjson | 1.714 | 1.772 | 4.886 | 49.695 | 1.00x |
| flat.json | msgspec | 1.827 | 1.865 | 2.586 | 49.695 | 0.95x |
| flat.json | ujson | 3.383 | 3.494 | 3.818 | 49.695 | 0.51x |
| flat.json | json | 3.407 | 3.534 | 5.408 | 49.695 | 0.50x |
| nested.json | strata | 1.368 | 1.466 | 2.339 | 49.703 | 1.00x |
| nested.json | orjson | 1.667 | 1.711 | 2.251 | 49.703 | 0.86x |
| nested.json | msgspec | 1.580 | 1.724 | 1.872 | 49.703 | 0.85x |
| nested.json | ujson | 2.742 | 2.882 | 3.188 | 49.703 | 0.51x |
| nested.json | json | 3.697 | 3.846 | 4.625 | 49.703 | 0.38x |
| wide_arrays.json | strata | 4.996 | 5.182 | 6.972 | 51.668 | 1.00x |
| wide_arrays.json | orjson | 5.819 | 6.219 | 12.655 | 51.668 | 0.83x |
| wide_arrays.json | msgspec | 5.905 | 6.351 | 10.826 | 51.668 | 0.82x |
| wide_arrays.json | ujson | 8.564 | 9.173 | 13.011 | 51.668 | 0.56x |
| wide_arrays.json | json | 12.081 | 12.419 | 13.695 | 51.668 | 0.42x |
| mixed.json | strata | 0.206 | 0.209 | 0.231 | 51.793 | 1.00x |
| mixed.json | orjson | 0.213 | 0.218 | 0.371 | 51.793 | 0.96x |
| mixed.json | msgspec | 0.234 | 0.239 | 0.360 | 51.793 | 0.87x |
| mixed.json | ujson | 0.362 | 0.366 | 0.453 | 51.793 | 0.57x |
| mixed.json | json | 0.474 | 0.482 | 0.533 | 51.793 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.540 | 3.586 | 3.907 | 44.691 | 1.00x |
| users.json | orjson | 3.714 | 3.783 | 3.846 | 44.691 | 0.95x |
| users.json | msgspec | 5.435 | 5.499 | 6.346 | 44.691 | 0.65x |
| users.json | ujson | 16.701 | 16.860 | 21.949 | 44.691 | 0.21x |
| users.json | json | 23.083 | 24.507 | 40.834 | 44.691 | 0.15x |
| flat.json | strata | 0.526 | 0.541 | 0.592 | 49.879 | 1.00x |
| flat.json | orjson | 0.543 | 0.563 | 0.600 | 49.879 | 0.96x |
| flat.json | msgspec | 0.733 | 0.764 | 0.839 | 49.879 | 0.71x |
| flat.json | ujson | 2.543 | 2.568 | 2.798 | 49.879 | 0.21x |
| flat.json | json | 3.243 | 3.343 | 3.512 | 49.879 | 0.16x |
| nested.json | strata | 0.310 | 0.457 | 0.518 | 50.059 | 1.00x |
| nested.json | orjson | 0.354 | 0.504 | 0.531 | 50.059 | 0.91x |
| nested.json | msgspec | 0.492 | 0.747 | 0.775 | 50.059 | 0.61x |
| nested.json | ujson | 1.460 | 2.181 | 2.436 | 50.059 | 0.21x |
| nested.json | json | 2.422 | 4.186 | 4.517 | 50.059 | 0.11x |
| wide_arrays.json | strata | 2.349 | 2.427 | 4.834 | 50.516 | 1.00x |
| wide_arrays.json | orjson | 2.260 | 2.598 | 4.038 | 50.516 | 0.93x |
| wide_arrays.json | msgspec | 3.525 | 4.053 | 5.481 | 50.516 | 0.60x |
| wide_arrays.json | ujson | 9.655 | 10.022 | 16.567 | 50.516 | 0.24x |
| wide_arrays.json | json | 18.436 | 19.757 | 28.815 | 50.516 | 0.12x |
| mixed.json | strata | 0.083 | 0.086 | 0.165 | 49.539 | 1.00x |
| mixed.json | orjson | 0.069 | 0.072 | 0.104 | 49.539 | 1.19x |
| mixed.json | msgspec | 0.093 | 0.139 | 0.144 | 49.539 | 0.62x |
| mixed.json | ujson | 0.337 | 0.344 | 0.829 | 49.539 | 0.25x |
| mixed.json | json | 0.522 | 0.586 | 1.179 | 49.539 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.753 | 20.847 | 21.988 | 54.039 | 1.00x |
| users.json | orjson | 14.156 | 22.237 | 23.752 | 54.039 | 0.94x |
| users.json | msgspec | 14.102 | 22.928 | 26.000 | 54.039 | 0.91x |
| users.json | ujson | 27.254 | 41.756 | 46.089 | 54.039 | 0.50x |
| users.json | json | 23.317 | 41.797 | 44.177 | 54.039 | 0.50x |
| flat.json | strata | 2.051 | 2.182 | 5.141 | 49.723 | 1.00x |
| flat.json | orjson | 1.906 | 2.085 | 3.682 | 49.723 | 1.05x |
| flat.json | msgspec | 1.952 | 2.076 | 2.200 | 49.723 | 1.05x |
| flat.json | ujson | 3.914 | 4.178 | 6.411 | 49.723 | 0.52x |
| flat.json | json | 3.389 | 3.857 | 7.135 | 49.723 | 0.57x |
| nested.json | strata | 1.533 | 1.573 | 4.099 | 49.711 | 1.00x |
| nested.json | orjson | 1.783 | 1.840 | 2.503 | 49.711 | 0.85x |
| nested.json | msgspec | 1.789 | 1.855 | 2.185 | 49.711 | 0.85x |
| nested.json | ujson | 3.127 | 3.283 | 6.855 | 49.711 | 0.48x |
| nested.json | json | 3.836 | 3.914 | 5.044 | 49.711 | 0.40x |
| wide_arrays.json | strata | 6.320 | 8.370 | 11.407 | 50.516 | 1.00x |
| wide_arrays.json | orjson | 6.153 | 6.931 | 13.349 | 50.516 | 1.21x |
| wide_arrays.json | msgspec | 6.427 | 8.893 | 11.473 | 50.516 | 0.94x |
| wide_arrays.json | ujson | 11.752 | 14.877 | 18.073 | 50.516 | 0.56x |
| wide_arrays.json | json | 12.579 | 20.436 | 23.271 | 50.516 | 0.41x |
| mixed.json | strata | 0.279 | 0.404 | 0.547 | 49.582 | 1.00x |
| mixed.json | orjson | 0.348 | 0.506 | 0.699 | 49.582 | 0.80x |
| mixed.json | msgspec | 0.352 | 0.533 | 1.636 | 49.582 | 0.76x |
| mixed.json | ujson | 0.554 | 0.765 | 0.891 | 49.582 | 0.53x |
| mixed.json | json | 0.589 | 0.956 | 1.173 | 49.582 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.799 | 21.594 | 23.159 | 50.992 | 1.00x |
| users.ndjson | orjson | 22.432 | 27.159 | 28.243 | 50.992 | 0.80x |
| users.ndjson | msgspec | 23.997 | 27.819 | 32.003 | 50.992 | 0.78x |
| users.ndjson | ujson | 39.091 | 41.350 | 53.118 | 50.992 | 0.52x |
| users.ndjson | json | 46.086 | 49.141 | 71.223 | 50.992 | 0.44x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.377 | 4.633 | 12.814 | 51.980 | 1.00x |
| users.json | orjson | 4.657 | 4.819 | 5.076 | 51.980 | 0.96x |
| users.json | msgspec | 6.261 | 6.438 | 6.999 | 51.980 | 0.72x |
| users.json | ujson | 27.076 | 28.215 | 43.890 | 51.980 | 0.16x |
| users.json | json | 33.264 | 35.021 | 57.090 | 51.980 | 0.13x |
| flat.json | strata | 0.996 | 1.151 | 1.373 | 50.047 | 1.00x |
| flat.json | orjson | 0.980 | 1.112 | 1.321 | 50.047 | 1.03x |
| flat.json | msgspec | 1.184 | 1.284 | 1.515 | 50.047 | 0.90x |
| flat.json | ujson | 4.442 | 5.055 | 5.857 | 50.047 | 0.23x |
| flat.json | json | 5.211 | 6.160 | 6.683 | 50.047 | 0.19x |
| nested.json | strata | 0.862 | 0.898 | 0.972 | 50.031 | 1.00x |
| nested.json | orjson | 0.899 | 0.964 | 1.059 | 50.031 | 0.93x |
| nested.json | msgspec | 1.158 | 1.193 | 1.254 | 50.031 | 0.75x |
| nested.json | ujson | 3.978 | 4.040 | 4.199 | 50.031 | 0.22x |
| nested.json | json | 5.967 | 6.075 | 6.407 | 50.031 | 0.15x |
| wide_arrays.json | strata | 3.055 | 3.276 | 4.683 | 52.941 | 1.00x |
| wide_arrays.json | orjson | 2.987 | 3.152 | 4.796 | 52.941 | 1.04x |
| wide_arrays.json | msgspec | 4.497 | 4.847 | 14.436 | 52.941 | 0.68x |
| wide_arrays.json | ujson | 16.828 | 17.228 | 24.172 | 52.941 | 0.19x |
| wide_arrays.json | json | 25.447 | 25.937 | 27.553 | 52.941 | 0.13x |
| mixed.json | strata | 0.502 | 0.522 | 0.595 | 49.887 | 1.00x |
| mixed.json | orjson | 0.489 | 0.511 | 0.648 | 49.887 | 1.02x |
| mixed.json | msgspec | 0.515 | 0.538 | 0.629 | 49.887 | 0.97x |
| mixed.json | ujson | 1.226 | 1.245 | 1.285 | 49.887 | 0.42x |
| mixed.json | json | 1.562 | 1.608 | 1.711 | 49.887 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.085 | 0.095 | 0.172 | 51.332 | 1.00x |
| users.json $[*].id | jmespath | 0.436 | 0.452 | 1.162 | 51.332 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.611 | 2.972 | 6.864 | 51.332 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.452 | 0.497 | 0.853 | 51.691 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.858 | 3.214 | 6.499 | 51.691 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.629 | 19.209 | 48.102 | 51.691 | 0.03x |
| users.json $..total | strata | 1.798 | 1.881 | 3.860 | 52.844 | 1.00x |
| users.json $..total | jsonpath-ng | 353.687 | 460.434 | 604.488 | 52.844 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 5.343 | 5.698 | 6.930 | 51.398 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.938 | 15.369 | 23.237 | 51.398 | 0.37x |
| users.json $[*].id | orjson+jsonpath-ng | 16.948 | 18.122 | 31.526 | 51.398 | 0.31x |
| users.json $[*].orders[*].total | strata | 5.611 | 5.855 | 9.240 | 52.387 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.543 | 18.364 | 18.821 | 52.387 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.657 | 38.288 | 42.453 | 52.387 | 0.15x |
| users.json $..total | strata | 21.737 | 25.792 | 39.218 | 52.430 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 513.532 | 608.985 | 680.452 | 52.430 | 0.04x |

