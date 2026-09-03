# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 85e2353
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
| users.json | strata | 8.734 | 10.661 | 21.773 | 45.289 | 1.00x |
| users.json | orjson | 11.893 | 13.924 | 22.395 | 45.289 | 0.77x |
| users.json | msgspec | 10.463 | 12.743 | 25.403 | 45.289 | 0.84x |
| users.json | ujson | 18.413 | 28.435 | 48.951 | 45.289 | 0.37x |
| users.json | json | 17.906 | 23.036 | 45.906 | 45.289 | 0.46x |
| flat.json | strata | 1.282 | 1.341 | 37.747 | 49.895 | 1.00x |
| flat.json | orjson | 1.123 | 1.490 | 1.655 | 49.895 | 0.90x |
| flat.json | msgspec | 0.993 | 1.405 | 1.538 | 49.895 | 0.95x |
| flat.json | ujson | 2.748 | 2.875 | 3.650 | 49.895 | 0.47x |
| flat.json | json | 2.408 | 2.528 | 2.746 | 49.895 | 0.53x |
| nested.json | strata | 0.968 | 1.086 | 1.256 | 49.922 | 1.00x |
| nested.json | orjson | 1.342 | 1.380 | 36.761 | 49.922 | 0.79x |
| nested.json | msgspec | 1.230 | 1.284 | 1.450 | 49.922 | 0.85x |
| nested.json | ujson | 2.028 | 2.205 | 2.849 | 49.922 | 0.49x |
| nested.json | json | 2.547 | 2.710 | 2.977 | 49.922 | 0.40x |
| wide_arrays.json | strata | 3.774 | 7.016 | 9.704 | 52.195 | 1.00x |
| wide_arrays.json | orjson | 4.790 | 8.507 | 10.504 | 52.195 | 0.82x |
| wide_arrays.json | msgspec | 4.718 | 8.711 | 11.578 | 52.195 | 0.81x |
| wide_arrays.json | ujson | 6.662 | 10.993 | 12.543 | 52.195 | 0.64x |
| wide_arrays.json | json | 9.031 | 16.516 | 17.867 | 52.195 | 0.42x |
| mixed.json | strata | 0.155 | 0.160 | 0.187 | 50.102 | 1.00x |
| mixed.json | orjson | 0.172 | 0.176 | 0.193 | 50.102 | 0.91x |
| mixed.json | msgspec | 0.181 | 0.189 | 0.228 | 50.102 | 0.85x |
| mixed.json | ujson | 0.292 | 0.302 | 0.319 | 50.102 | 0.53x |
| mixed.json | json | 0.364 | 0.368 | 0.394 | 50.102 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.753 | 3.564 | 4.217 | 45.508 | 1.00x |
| users.json | orjson | 2.555 | 2.851 | 4.645 | 45.508 | 1.25x |
| users.json | msgspec | 4.294 | 5.045 | 7.575 | 45.508 | 0.71x |
| users.json | ujson | 13.072 | 17.336 | 25.389 | 45.508 | 0.21x |
| users.json | json | 19.726 | 22.217 | 33.497 | 45.508 | 0.16x |
| flat.json | strata | 0.363 | 0.411 | 0.618 | 50.215 | 1.00x |
| flat.json | orjson | 0.382 | 0.424 | 1.350 | 50.215 | 0.97x |
| flat.json | msgspec | 0.633 | 0.711 | 0.801 | 50.215 | 0.58x |
| flat.json | ujson | 1.998 | 2.045 | 2.352 | 50.215 | 0.20x |
| flat.json | json | 2.674 | 2.809 | 3.082 | 50.215 | 0.15x |
| nested.json | strata | 0.327 | 0.338 | 0.356 | 50.625 | 1.00x |
| nested.json | orjson | 0.290 | 0.356 | 0.393 | 50.625 | 0.95x |
| nested.json | msgspec | 0.418 | 0.596 | 0.645 | 50.625 | 0.57x |
| nested.json | ujson | 1.662 | 1.695 | 1.834 | 50.625 | 0.20x |
| nested.json | json | 3.152 | 3.225 | 3.554 | 50.625 | 0.10x |
| wide_arrays.json | strata | 1.939 | 1.983 | 2.983 | 51.637 | 1.00x |
| wide_arrays.json | orjson | 2.070 | 2.132 | 3.220 | 51.637 | 0.93x |
| wide_arrays.json | msgspec | 3.651 | 3.707 | 5.637 | 51.637 | 0.54x |
| wide_arrays.json | ujson | 8.135 | 8.285 | 14.705 | 51.637 | 0.24x |
| wide_arrays.json | json | 14.708 | 15.034 | 20.729 | 51.637 | 0.13x |
| mixed.json | strata | 0.061 | 0.070 | 0.098 | 50.469 | 1.00x |
| mixed.json | orjson | 0.051 | 0.057 | 0.103 | 50.469 | 1.23x |
| mixed.json | msgspec | 0.079 | 0.109 | 0.205 | 50.469 | 0.64x |
| mixed.json | ujson | 0.264 | 0.334 | 0.590 | 50.469 | 0.21x |
| mixed.json | json | 0.409 | 0.431 | 1.141 | 50.469 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.173 | 13.566 | 17.648 | 55.535 | 1.00x |
| users.json | orjson | 12.775 | 15.161 | 56.513 | 55.535 | 0.89x |
| users.json | msgspec | 11.761 | 14.403 | 20.795 | 55.535 | 0.94x |
| users.json | ujson | 22.201 | 25.890 | 65.185 | 55.535 | 0.52x |
| users.json | json | 18.744 | 22.598 | 30.489 | 55.535 | 0.60x |
| flat.json | strata | 1.478 | 1.556 | 29.661 | 50.094 | 1.00x |
| flat.json | orjson | 1.699 | 1.820 | 2.141 | 50.094 | 0.85x |
| flat.json | msgspec | 1.510 | 1.663 | 1.740 | 50.094 | 0.94x |
| flat.json | ujson | 3.190 | 3.366 | 215.433 | 50.094 | 0.46x |
| flat.json | json | 2.568 | 2.693 | 3.063 | 50.094 | 0.58x |
| nested.json | strata | 0.757 | 0.936 | 1.669 | 50.242 | 1.00x |
| nested.json | orjson | 0.990 | 1.175 | 2.002 | 50.242 | 0.80x |
| nested.json | msgspec | 0.885 | 0.945 | 1.721 | 50.242 | 0.99x |
| nested.json | ujson | 1.563 | 1.721 | 2.787 | 50.242 | 0.54x |
| nested.json | json | 1.718 | 1.843 | 40.309 | 50.242 | 0.51x |
| wide_arrays.json | strata | 4.510 | 4.801 | 7.507 | 51.637 | 1.00x |
| wide_arrays.json | orjson | 5.692 | 5.836 | 8.440 | 51.637 | 0.82x |
| wide_arrays.json | msgspec | 5.622 | 5.809 | 9.728 | 51.637 | 0.83x |
| wide_arrays.json | ujson | 9.089 | 9.546 | 51.154 | 51.637 | 0.50x |
| wide_arrays.json | json | 9.903 | 10.457 | 16.786 | 51.637 | 0.46x |
| mixed.json | strata | 0.240 | 0.247 | 0.275 | 50.477 | 1.00x |
| mixed.json | orjson | 0.279 | 0.294 | 0.302 | 50.477 | 0.84x |
| mixed.json | msgspec | 0.285 | 0.294 | 0.329 | 50.477 | 0.84x |
| mixed.json | ujson | 0.440 | 0.465 | 0.497 | 50.477 | 0.53x |
| mixed.json | json | 0.464 | 0.468 | 0.570 | 50.477 | 0.53x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.078 | 18.843 | 47.972 | 51.023 | 1.00x |
| users.ndjson | orjson | 24.896 | 27.433 | 30.411 | 51.023 | 0.69x |
| users.ndjson | msgspec | 24.076 | 26.713 | 63.131 | 51.023 | 0.71x |
| users.ndjson | ujson | 35.902 | 38.480 | 43.107 | 51.023 | 0.49x |
| users.ndjson | json | 41.093 | 45.561 | 82.522 | 51.023 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.658 | 4.035 | 5.160 | 52.203 | 1.00x |
| users.json | orjson | 3.868 | 4.210 | 5.348 | 52.203 | 0.96x |
| users.json | msgspec | 5.397 | 5.920 | 7.875 | 52.203 | 0.68x |
| users.json | ujson | 21.477 | 21.763 | 23.308 | 52.203 | 0.19x |
| users.json | json | 27.502 | 28.068 | 46.177 | 52.203 | 0.14x |
| flat.json | strata | 0.797 | 0.912 | 1.311 | 50.430 | 1.00x |
| flat.json | orjson | 0.785 | 0.826 | 1.133 | 50.430 | 1.10x |
| flat.json | msgspec | 0.982 | 1.046 | 1.280 | 50.430 | 0.87x |
| flat.json | ujson | 3.749 | 3.898 | 4.439 | 50.430 | 0.23x |
| flat.json | json | 3.939 | 4.557 | 5.151 | 50.430 | 0.20x |
| nested.json | strata | 0.541 | 0.591 | 0.645 | 50.723 | 1.00x |
| nested.json | orjson | 0.549 | 0.595 | 0.624 | 50.723 | 0.99x |
| nested.json | msgspec | 0.715 | 0.741 | 0.800 | 50.723 | 0.80x |
| nested.json | ujson | 1.919 | 1.958 | 2.015 | 50.723 | 0.30x |
| nested.json | json | 2.770 | 2.805 | 2.952 | 50.723 | 0.21x |
| wide_arrays.json | strata | 2.692 | 2.871 | 5.631 | 51.645 | 1.00x |
| wide_arrays.json | orjson | 2.832 | 2.984 | 4.213 | 51.645 | 0.96x |
| wide_arrays.json | msgspec | 4.256 | 4.408 | 12.348 | 51.645 | 0.65x |
| wide_arrays.json | ujson | 13.692 | 14.013 | 237.290 | 51.645 | 0.20x |
| wide_arrays.json | json | 20.254 | 20.532 | 62.964 | 51.645 | 0.14x |
| mixed.json | strata | 0.348 | 0.438 | 0.630 | 50.512 | 1.00x |
| mixed.json | orjson | 0.330 | 0.391 | 0.497 | 50.512 | 1.12x |
| mixed.json | msgspec | 0.358 | 0.418 | 0.689 | 50.512 | 1.05x |
| mixed.json | ujson | 0.670 | 0.732 | 1.156 | 50.512 | 0.60x |
| mixed.json | json | 0.828 | 1.055 | 1.596 | 50.512 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.094 | 0.101 | 0.135 | 52.227 | 1.00x |
| users.json $[*].id | jmespath | 0.350 | 0.372 | 0.609 | 52.227 | 0.27x |
| users.json $[*].id | jsonpath-ng | 2.054 | 2.398 | 3.723 | 52.227 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.426 | 0.580 | 0.757 | 52.301 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.089 | 2.236 | 3.723 | 52.301 | 0.26x |
| users.json $[*].orders[*].total | jsonpath-ng | 13.869 | 15.586 | 26.039 | 52.301 | 0.04x |
| users.json $..total | strata | 1.456 | 1.599 | 2.675 | 53.305 | 1.00x |
| users.json $..total | jsonpath-ng | 252.287 | 272.350 | 518.902 | 53.305 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.445 | 3.634 | 4.220 | 52.230 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.405 | 14.063 | 15.395 | 52.230 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 15.162 | 15.972 | 27.057 | 52.230 | 0.23x |
| users.json $[*].orders[*].total | strata | 3.756 | 3.951 | 6.141 | 53.301 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.376 | 16.895 | 24.881 | 53.301 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 30.564 | 32.557 | 50.264 | 53.301 | 0.12x |
| users.json $..total | strata | 13.439 | 22.075 | 58.225 | 52.930 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 279.939 | 513.941 | 873.239 | 52.930 | 0.04x |

