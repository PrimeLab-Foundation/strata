# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a4cb8c0
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.069 | 10.218 | 10.561 | 52.703 | 1.00x |
| users.json | orjson | 10.770 | 10.907 | 11.341 | 52.703 | 0.94x |
| users.json | msgspec | 10.897 | 11.020 | 11.646 | 52.703 | 0.93x |
| users.json | ujson | 14.518 | 14.789 | 15.716 | 52.703 | 0.69x |
| users.json | json | 19.241 | 19.634 | 20.153 | 52.703 | 0.52x |
| flat.json | strata | 0.973 | 0.982 | 1.063 | 69.641 | 1.00x |
| flat.json | orjson | 0.815 | 0.824 | 0.877 | 69.641 | 1.19x |
| flat.json | msgspec | 0.897 | 0.905 | 0.948 | 69.641 | 1.08x |
| flat.json | ujson | 1.292 | 1.312 | 1.554 | 69.641 | 0.75x |
| flat.json | json | 1.797 | 1.811 | 1.974 | 69.641 | 0.54x |
| nested.json | strata | 0.787 | 0.799 | 0.811 | 69.750 | 1.00x |
| nested.json | orjson | 1.557 | 1.565 | 1.753 | 69.750 | 0.51x |
| nested.json | msgspec | 1.553 | 1.586 | 1.801 | 69.750 | 0.50x |
| nested.json | ujson | 1.899 | 1.930 | 2.000 | 69.750 | 0.41x |
| nested.json | json | 2.486 | 2.519 | 2.914 | 69.750 | 0.32x |
| wide_arrays.json | strata | 4.696 | 4.730 | 4.865 | 69.781 | 1.00x |
| wide_arrays.json | orjson | 4.836 | 4.940 | 5.083 | 69.781 | 0.96x |
| wide_arrays.json | msgspec | 5.785 | 5.876 | 6.317 | 69.781 | 0.80x |
| wide_arrays.json | ujson | 7.335 | 7.477 | 8.014 | 69.781 | 0.63x |
| wide_arrays.json | json | 10.421 | 10.486 | 11.407 | 69.781 | 0.45x |
| mixed.json | strata | 0.206 | 0.215 | 0.241 | 69.797 | 1.00x |
| mixed.json | orjson | 0.163 | 0.169 | 0.196 | 69.797 | 1.27x |
| mixed.json | msgspec | 0.179 | 0.186 | 0.210 | 69.797 | 1.16x |
| mixed.json | ujson | 0.238 | 0.245 | 0.270 | 69.797 | 0.88x |
| mixed.json | json | 0.394 | 0.399 | 0.454 | 69.797 | 0.54x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.555 | 2.561 | 2.657 | 55.156 | 1.00x |
| users.json | orjson | 2.572 | 2.598 | 2.671 | 55.156 | 0.99x |
| users.json | msgspec | 3.384 | 3.404 | 3.460 | 55.156 | 0.75x |
| users.json | ujson | 13.834 | 13.968 | 14.435 | 55.156 | 0.18x |
| users.json | json | 19.686 | 19.797 | 20.545 | 55.156 | 0.13x |
| flat.json | strata | 0.278 | 0.281 | 0.288 | 69.734 | 1.00x |
| flat.json | orjson | 0.291 | 0.294 | 0.321 | 69.734 | 0.96x |
| flat.json | msgspec | 0.383 | 0.387 | 0.395 | 69.734 | 0.73x |
| flat.json | ujson | 1.139 | 1.152 | 1.170 | 69.734 | 0.24x |
| flat.json | json | 1.628 | 1.654 | 1.709 | 69.734 | 0.17x |
| nested.json | strata | 0.205 | 0.209 | 0.220 | 69.781 | 1.00x |
| nested.json | orjson | 0.260 | 0.267 | 0.293 | 69.781 | 0.78x |
| nested.json | msgspec | 0.348 | 0.352 | 0.430 | 69.781 | 0.59x |
| nested.json | ujson | 1.266 | 1.278 | 1.303 | 69.781 | 0.16x |
| nested.json | json | 2.082 | 2.108 | 2.217 | 69.781 | 0.10x |
| wide_arrays.json | strata | 1.439 | 1.471 | 1.543 | 69.797 | 1.00x |
| wide_arrays.json | orjson | 1.445 | 1.466 | 1.624 | 69.797 | 1.00x |
| wide_arrays.json | msgspec | 2.519 | 2.540 | 2.698 | 69.797 | 0.58x |
| wide_arrays.json | ujson | 7.827 | 7.889 | 8.214 | 69.797 | 0.19x |
| wide_arrays.json | json | 14.031 | 14.191 | 14.745 | 69.797 | 0.10x |
| mixed.json | strata | 0.058 | 0.061 | 0.070 | 69.797 | 1.00x |
| mixed.json | orjson | 0.056 | 0.058 | 0.083 | 69.797 | 1.04x |
| mixed.json | msgspec | 0.064 | 0.067 | 0.075 | 69.797 | 0.91x |
| mixed.json | ujson | 0.281 | 0.285 | 0.309 | 69.797 | 0.21x |
| mixed.json | json | 0.438 | 0.444 | 0.479 | 69.797 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.421 | 10.506 | 10.826 | 70.188 | 1.00x |
| users.json | orjson | 13.097 | 13.300 | 13.913 | 70.188 | 0.79x |
| users.json | msgspec | 13.286 | 13.658 | 14.024 | 70.188 | 0.77x |
| users.json | ujson | 17.066 | 17.572 | 18.332 | 70.188 | 0.60x |
| users.json | json | 21.536 | 21.725 | 22.443 | 70.188 | 0.48x |
| flat.json | strata | 1.043 | 1.054 | 1.090 | 69.734 | 1.00x |
| flat.json | orjson | 0.879 | 0.892 | 0.975 | 69.734 | 1.18x |
| flat.json | msgspec | 0.961 | 0.974 | 1.020 | 69.734 | 1.08x |
| flat.json | ujson | 1.405 | 1.435 | 1.524 | 69.734 | 0.73x |
| flat.json | json | 1.866 | 1.879 | 2.015 | 69.734 | 0.56x |
| nested.json | strata | 0.865 | 0.880 | 0.942 | 69.781 | 1.00x |
| nested.json | orjson | 1.745 | 1.791 | 1.874 | 69.781 | 0.49x |
| nested.json | msgspec | 1.758 | 1.796 | 1.999 | 69.781 | 0.49x |
| nested.json | ujson | 2.128 | 2.211 | 2.275 | 69.781 | 0.40x |
| nested.json | json | 2.681 | 2.730 | 2.908 | 69.781 | 0.32x |
| wide_arrays.json | strata | 4.871 | 4.920 | 5.279 | 69.797 | 1.00x |
| wide_arrays.json | orjson | 5.448 | 5.530 | 5.857 | 69.797 | 0.89x |
| wide_arrays.json | msgspec | 6.468 | 6.650 | 7.181 | 69.797 | 0.74x |
| wide_arrays.json | ujson | 8.214 | 8.474 | 8.849 | 69.797 | 0.58x |
| wide_arrays.json | json | 11.087 | 11.263 | 11.877 | 69.797 | 0.44x |
| mixed.json | strata | 0.252 | 0.267 | 0.281 | 69.797 | 1.00x |
| mixed.json | orjson | 0.217 | 0.224 | 0.258 | 69.797 | 1.19x |
| mixed.json | msgspec | 0.236 | 0.248 | 0.297 | 69.797 | 1.07x |
| mixed.json | ujson | 0.313 | 0.331 | 0.372 | 69.797 | 0.80x |
| mixed.json | json | 0.453 | 0.466 | 0.492 | 69.797 | 0.57x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 9.944 | 10.127 | 10.403 | 68.766 | 1.00x |
| users.ndjson | orjson | 13.571 | 13.968 | 14.747 | 68.766 | 0.72x |
| users.ndjson | msgspec | 13.222 | 13.505 | 14.270 | 68.766 | 0.75x |
| users.ndjson | ujson | 16.918 | 17.146 | 17.771 | 68.766 | 0.59x |
| users.ndjson | json | 23.378 | 23.698 | 25.174 | 68.766 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.958 | 3.016 | 3.171 | 65.219 | 1.00x |
| users.json | orjson | 2.981 | 3.036 | 3.090 | 65.219 | 0.99x |
| users.json | msgspec | 3.775 | 3.843 | 3.902 | 65.219 | 0.78x |
| users.json | ujson | 14.114 | 14.300 | 14.901 | 65.219 | 0.21x |
| users.json | json | 20.116 | 20.294 | 20.860 | 65.219 | 0.15x |
| flat.json | strata | 0.435 | 0.468 | 0.482 | 69.750 | 1.00x |
| flat.json | orjson | 0.441 | 0.459 | 0.509 | 69.750 | 1.02x |
| flat.json | msgspec | 0.532 | 0.541 | 0.616 | 69.750 | 0.86x |
| flat.json | ujson | 1.298 | 1.317 | 1.356 | 69.750 | 0.35x |
| flat.json | json | 1.787 | 1.801 | 1.878 | 69.750 | 0.26x |
| nested.json | strata | 0.372 | 0.415 | 0.540 | 69.781 | 1.00x |
| nested.json | orjson | 0.406 | 0.440 | 0.505 | 69.781 | 0.94x |
| nested.json | msgspec | 0.488 | 0.513 | 0.548 | 69.781 | 0.81x |
| nested.json | ujson | 1.440 | 1.477 | 1.660 | 69.781 | 0.28x |
| nested.json | json | 2.240 | 2.302 | 2.406 | 69.781 | 0.18x |
| wide_arrays.json | strata | 1.760 | 1.837 | 1.926 | 69.797 | 1.00x |
| wide_arrays.json | orjson | 1.754 | 1.788 | 1.981 | 69.797 | 1.03x |
| wide_arrays.json | msgspec | 2.824 | 2.854 | 2.932 | 69.797 | 0.64x |
| wide_arrays.json | ujson | 8.248 | 8.326 | 8.469 | 69.797 | 0.22x |
| wide_arrays.json | json | 14.478 | 14.593 | 15.028 | 69.797 | 0.13x |
| mixed.json | strata | 0.206 | 0.236 | 0.363 | 68.328 | 1.00x |
| mixed.json | orjson | 0.167 | 0.199 | 0.278 | 68.328 | 1.18x |
| mixed.json | msgspec | 0.187 | 0.217 | 0.406 | 68.328 | 1.09x |
| mixed.json | ujson | 0.415 | 0.436 | 0.527 | 68.328 | 0.54x |
| mixed.json | json | 0.585 | 0.627 | 0.675 | 68.328 | 0.38x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.064 | 0.073 | 0.090 | 65.484 | 1.00x |
| users.json $[*].id | jmespath | 0.431 | 0.438 | 0.475 | 65.484 | 0.17x |
| users.json $[*].id | jsonpath-ng | 4.191 | 4.322 | 4.873 | 65.484 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.409 | 0.424 | 0.451 | 66.812 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.648 | 2.699 | 3.046 | 66.812 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.539 | 14.783 | 17.342 | 66.812 | 0.03x |
| users.json $..total | strata | 1.768 | 1.782 | 1.874 | 69.891 | 1.00x |
| users.json $..total | jsonpath-ng | 272.072 | 273.881 | 278.338 | 69.891 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.868 | 5.008 | 5.156 | 66.594 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.767 | 14.024 | 14.432 | 66.594 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 15.312 | 15.602 | 16.414 | 66.594 | 0.32x |
| users.json $[*].orders[*].total | strata | 4.976 | 5.067 | 5.113 | 69.797 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.025 | 16.079 | 16.943 | 69.797 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 25.611 | 26.274 | 27.746 | 69.797 | 0.19x |
| users.json $..total | strata | 14.791 | 15.321 | 16.052 | 69.891 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 283.667 | 284.791 | 289.347 | 69.891 | 0.05x |

