# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ddc9cb7
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
| users.json | strata | 19.107 | 21.756 | 41.861 | 52.613 | 1.00x |
| users.json | orjson | 29.761 | 32.205 | 60.440 | 52.613 | 0.68x |
| users.json | msgspec | 26.161 | 33.788 | 62.115 | 52.613 | 0.64x |
| users.json | ujson | 42.678 | 47.078 | 86.657 | 52.613 | 0.46x |
| users.json | pysimdjson | 159.684 | 178.457 | 377.713 | 52.613 | 0.12x |
| users.json | json | 46.161 | 50.272 | 79.668 | 52.613 | 0.43x |
| flat.json | strata | 1.326 | 1.480 | 1.833 | 59.133 | 1.00x |
| flat.json | orjson | 1.435 | 1.547 | 1.845 | 59.133 | 0.96x |
| flat.json | msgspec | 1.612 | 1.751 | 2.295 | 59.133 | 0.84x |
| flat.json | ujson | 2.795 | 3.140 | 3.644 | 59.133 | 0.47x |
| flat.json | pysimdjson | 14.419 | 15.881 | 17.256 | 59.133 | 0.09x |
| flat.json | json | 3.251 | 3.650 | 4.046 | 59.133 | 0.41x |
| nested.json | strata | 1.505 | 1.608 | 2.412 | 48.367 | 1.00x |
| nested.json | orjson | 1.697 | 1.846 | 2.338 | 48.367 | 0.87x |
| nested.json | msgspec | 1.897 | 2.222 | 2.997 | 48.367 | 0.72x |
| nested.json | ujson | 3.078 | 3.167 | 4.343 | 48.367 | 0.51x |
| nested.json | pysimdjson | 12.976 | 13.632 | 16.879 | 48.367 | 0.12x |
| nested.json | json | 3.867 | 4.453 | 5.364 | 48.367 | 0.36x |
| wide_arrays.json | strata | 8.354 | 9.792 | 10.838 | 59.750 | 1.00x |
| wide_arrays.json | orjson | 9.383 | 10.598 | 12.968 | 59.750 | 0.92x |
| wide_arrays.json | msgspec | 10.285 | 11.642 | 13.193 | 59.750 | 0.84x |
| wide_arrays.json | ujson | 13.215 | 15.208 | 17.451 | 59.750 | 0.64x |
| wide_arrays.json | pysimdjson | 78.267 | 83.018 | 93.265 | 59.750 | 0.12x |
| wide_arrays.json | json | 16.925 | 18.995 | 24.310 | 59.750 | 0.52x |
| mixed.json | strata | 0.442 | 0.567 | 0.616 | 58.609 | 1.00x |
| mixed.json | orjson | 0.471 | 0.623 | 0.705 | 58.609 | 0.91x |
| mixed.json | msgspec | 0.616 | 0.700 | 0.820 | 58.609 | 0.81x |
| mixed.json | ujson | 0.797 | 0.920 | 1.102 | 58.609 | 0.62x |
| mixed.json | pysimdjson | 3.508 | 3.913 | 4.444 | 58.609 | 0.14x |
| mixed.json | json | 0.930 | 1.321 | 1.416 | 58.609 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.598 | 2.705 | 3.198 | 47.281 | 1.00x |
| users.json | orjson | 3.527 | 3.687 | 4.303 | 47.281 | 0.73x |
| users.json | msgspec | 6.039 | 6.606 | 7.153 | 47.281 | 0.41x |
| users.json | ujson | 30.731 | 33.214 | 34.947 | 47.281 | 0.08x |
| users.json | json | 40.044 | 42.862 | 47.243 | 47.281 | 0.06x |
| flat.json | strata | 0.326 | 0.341 | 0.439 | 47.969 | 1.00x |
| flat.json | orjson | 0.426 | 0.459 | 0.615 | 47.969 | 0.74x |
| flat.json | msgspec | 0.679 | 0.788 | 1.024 | 47.969 | 0.43x |
| flat.json | ujson | 2.732 | 3.127 | 3.706 | 47.969 | 0.11x |
| flat.json | json | 3.620 | 4.241 | 4.962 | 47.969 | 0.08x |
| nested.json | strata | 0.288 | 0.355 | 0.463 | 48.203 | 1.00x |
| nested.json | orjson | 0.414 | 0.467 | 0.588 | 48.203 | 0.76x |
| nested.json | msgspec | 0.586 | 0.668 | 0.945 | 48.203 | 0.53x |
| nested.json | ujson | 2.798 | 3.107 | 3.889 | 48.203 | 0.11x |
| nested.json | json | 4.730 | 5.567 | 6.075 | 48.203 | 0.06x |
| wide_arrays.json | strata | 1.852 | 2.096 | 2.752 | 59.672 | 1.00x |
| wide_arrays.json | orjson | 2.526 | 2.689 | 2.985 | 59.672 | 0.78x |
| wide_arrays.json | msgspec | 3.347 | 3.832 | 4.625 | 59.672 | 0.55x |
| wide_arrays.json | ujson | 15.201 | 16.627 | 20.222 | 59.672 | 0.13x |
| wide_arrays.json | json | 35.840 | 38.202 | 40.937 | 59.672 | 0.05x |
| mixed.json | strata | 0.075 | 0.090 | 0.166 | 54.555 | 1.00x |
| mixed.json | orjson | 0.086 | 0.107 | 0.113 | 54.555 | 0.84x |
| mixed.json | msgspec | 0.126 | 0.143 | 0.167 | 54.555 | 0.63x |
| mixed.json | ujson | 0.598 | 0.626 | 0.699 | 54.555 | 0.14x |
| mixed.json | json | 0.946 | 1.001 | 1.467 | 54.555 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.025 | 20.557 | 23.204 | 59.781 | 1.00x |
| users.json | orjson | 27.723 | 31.058 | 38.202 | 59.781 | 0.66x |
| users.json | msgspec | 28.090 | 31.189 | 35.693 | 59.781 | 0.66x |
| users.json | ujson | 42.153 | 44.282 | 50.348 | 59.781 | 0.46x |
| users.json | json | 45.992 | 48.059 | 55.123 | 59.781 | 0.43x |
| flat.json | strata | 1.400 | 1.676 | 2.011 | 48.148 | 1.00x |
| flat.json | orjson | 1.553 | 1.930 | 2.157 | 48.148 | 0.87x |
| flat.json | msgspec | 1.846 | 2.152 | 2.529 | 48.148 | 0.78x |
| flat.json | ujson | 2.975 | 3.670 | 3.971 | 48.148 | 0.46x |
| flat.json | json | 3.299 | 4.025 | 4.529 | 48.148 | 0.42x |
| nested.json | strata | 1.607 | 1.666 | 2.083 | 48.203 | 1.00x |
| nested.json | orjson | 1.834 | 1.931 | 2.403 | 48.203 | 0.86x |
| nested.json | msgspec | 2.051 | 2.113 | 2.726 | 48.203 | 0.79x |
| nested.json | ujson | 3.259 | 3.340 | 3.693 | 48.203 | 0.50x |
| nested.json | json | 3.962 | 4.269 | 5.168 | 48.203 | 0.39x |
| wide_arrays.json | strata | 8.631 | 8.914 | 11.288 | 59.672 | 1.00x |
| wide_arrays.json | orjson | 9.687 | 10.579 | 13.244 | 59.672 | 0.84x |
| wide_arrays.json | msgspec | 10.657 | 11.572 | 13.990 | 59.672 | 0.77x |
| wide_arrays.json | ujson | 13.876 | 14.167 | 17.359 | 59.672 | 0.63x |
| wide_arrays.json | json | 17.303 | 17.959 | 20.142 | 59.672 | 0.50x |
| mixed.json | strata | 0.483 | 0.498 | 0.740 | 54.555 | 1.00x |
| mixed.json | orjson | 0.593 | 0.608 | 0.743 | 54.555 | 0.82x |
| mixed.json | msgspec | 0.627 | 0.650 | 0.919 | 54.555 | 0.77x |
| mixed.json | ujson | 0.810 | 0.827 | 1.149 | 54.555 | 0.60x |
| mixed.json | json | 1.036 | 1.058 | 1.542 | 54.555 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.572 | 26.915 | 31.007 | 58.109 | 1.00x |
| users.ndjson | orjson | 29.084 | 38.014 | 51.601 | 58.109 | 0.71x |
| users.ndjson | msgspec | 30.393 | 37.518 | 48.477 | 58.109 | 0.72x |
| users.ndjson | ujson | 41.680 | 52.593 | 76.496 | 58.109 | 0.51x |
| users.ndjson | json | 53.996 | 65.552 | 81.186 | 58.109 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.570 | 5.099 | 5.775 | 51.402 | 1.00x |
| users.json | orjson | 4.441 | 6.699 | 7.432 | 51.402 | 0.76x |
| users.json | msgspec | 6.990 | 8.407 | 10.007 | 51.402 | 0.61x |
| users.json | ujson | 34.158 | 39.518 | 89.959 | 51.402 | 0.13x |
| users.json | json | 49.906 | 55.770 | 149.017 | 51.402 | 0.09x |
| flat.json | strata | 0.722 | 0.903 | 1.047 | 48.129 | 1.00x |
| flat.json | orjson | 0.856 | 1.076 | 1.416 | 48.129 | 0.84x |
| flat.json | msgspec | 1.026 | 1.296 | 1.366 | 48.129 | 0.70x |
| flat.json | ujson | 3.118 | 3.874 | 4.440 | 48.129 | 0.23x |
| flat.json | json | 4.102 | 5.184 | 6.275 | 48.129 | 0.17x |
| nested.json | strata | 0.597 | 0.777 | 1.038 | 48.203 | 1.00x |
| nested.json | orjson | 0.698 | 0.968 | 1.525 | 48.203 | 0.80x |
| nested.json | msgspec | 0.955 | 1.230 | 1.371 | 48.203 | 0.63x |
| nested.json | ujson | 3.171 | 3.592 | 4.478 | 48.203 | 0.22x |
| nested.json | json | 4.907 | 5.847 | 6.303 | 48.203 | 0.13x |
| wide_arrays.json | strata | 2.424 | 3.469 | 3.928 | 59.672 | 1.00x |
| wide_arrays.json | orjson | 3.446 | 4.530 | 4.866 | 59.672 | 0.77x |
| wide_arrays.json | msgspec | 4.061 | 5.149 | 6.216 | 59.672 | 0.67x |
| wide_arrays.json | ujson | 16.228 | 19.577 | 22.421 | 59.672 | 0.18x |
| wide_arrays.json | json | 37.761 | 43.698 | 61.751 | 59.672 | 0.08x |
| mixed.json | strata | 0.407 | 0.522 | 0.723 | 54.555 | 1.00x |
| mixed.json | orjson | 0.482 | 0.549 | 0.703 | 54.555 | 0.95x |
| mixed.json | msgspec | 0.506 | 0.581 | 0.826 | 54.555 | 0.90x |
| mixed.json | ujson | 1.027 | 1.265 | 1.662 | 54.555 | 0.41x |
| mixed.json | json | 1.345 | 1.738 | 2.131 | 54.555 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.187 | 0.219 | 0.324 | 51.461 | 1.00x |
| users.json $[*].id | jmespath | 1.010 | 1.168 | 1.515 | 51.461 | 0.19x |
| users.json $[*].id | jsonpath-ng | 5.247 | 6.435 | 8.562 | 51.461 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.169 | 1.327 | 1.775 | 54.090 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.282 | 6.801 | 8.275 | 54.090 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 37.187 | 38.958 | 44.360 | 54.090 | 0.03x |
| users.json $..total | strata | 3.649 | 4.521 | 5.419 | 56.156 | 1.00x |
| users.json $..total | jsonpath-ng | 741.464 | 808.086 | 983.734 | 56.156 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.636 | 4.473 | 5.402 | 58.270 | 1.00x |
| users.json $[*].id | orjson+jmespath | 28.472 | 30.362 | 34.580 | 58.270 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 32.544 | 35.541 | 38.786 | 58.270 | 0.13x |
| users.json $[*].orders[*].total | strata | 3.956 | 4.471 | 6.089 | 55.328 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 33.744 | 39.572 | 47.502 | 55.328 | 0.11x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 80.843 | 88.066 | 96.055 | 55.328 | 0.05x |
| users.json $..total | strata | 23.010 | 25.014 | 30.565 | 55.418 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 761.798 | 795.237 | 839.653 | 55.418 | 0.03x |

