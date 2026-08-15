# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 75b7e23
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
| users.json | strata | 13.374 | 14.365 | 25.385 | 45.551 | 1.00x |
| users.json | orjson | 18.522 | 20.733 | 36.152 | 45.551 | 0.69x |
| users.json | msgspec | 17.807 | 19.142 | 29.803 | 45.551 | 0.75x |
| users.json | ujson | 27.809 | 30.457 | 48.822 | 45.551 | 0.47x |
| users.json | json | 26.955 | 29.347 | 32.405 | 45.551 | 0.49x |
| flat.json | strata | 1.388 | 1.503 | 2.441 | 48.625 | 1.00x |
| flat.json | orjson | 1.502 | 1.644 | 2.547 | 48.625 | 0.91x |
| flat.json | msgspec | 1.321 | 1.421 | 2.536 | 48.625 | 1.06x |
| flat.json | ujson | 2.787 | 3.046 | 4.530 | 48.625 | 0.49x |
| flat.json | json | 2.241 | 2.431 | 4.195 | 48.625 | 0.62x |
| nested.json | strata | 1.047 | 1.163 | 2.187 | 48.418 | 1.00x |
| nested.json | orjson | 1.334 | 1.535 | 2.254 | 48.418 | 0.76x |
| nested.json | msgspec | 1.196 | 1.280 | 2.500 | 48.418 | 0.91x |
| nested.json | ujson | 1.946 | 2.187 | 3.728 | 48.418 | 0.53x |
| nested.json | json | 2.312 | 2.612 | 4.658 | 48.418 | 0.45x |
| wide_arrays.json | strata | 5.913 | 6.339 | 13.286 | 50.637 | 1.00x |
| wide_arrays.json | orjson | 7.188 | 7.331 | 14.890 | 50.637 | 0.86x |
| wide_arrays.json | msgspec | 6.754 | 7.028 | 9.060 | 50.637 | 0.90x |
| wide_arrays.json | ujson | 9.083 | 9.737 | 11.347 | 50.637 | 0.65x |
| wide_arrays.json | json | 13.032 | 13.696 | 15.172 | 50.637 | 0.46x |
| mixed.json | strata | 0.233 | 0.255 | 0.353 | 48.496 | 1.00x |
| mixed.json | orjson | 0.243 | 0.258 | 0.336 | 48.496 | 0.99x |
| mixed.json | msgspec | 0.264 | 0.302 | 0.465 | 48.496 | 0.84x |
| mixed.json | ujson | 0.400 | 0.440 | 0.655 | 48.496 | 0.58x |
| mixed.json | json | 0.493 | 0.521 | 0.730 | 48.496 | 0.49x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.230 | 4.613 | 7.397 | 44.938 | 1.00x |
| users.json | orjson | 4.407 | 4.734 | 5.303 | 44.938 | 0.97x |
| users.json | msgspec | 6.399 | 6.686 | 6.997 | 44.938 | 0.69x |
| users.json | ujson | 17.218 | 17.785 | 28.414 | 44.938 | 0.26x |
| users.json | json | 24.960 | 25.675 | 26.142 | 44.938 | 0.18x |
| flat.json | strata | 0.507 | 0.540 | 0.603 | 49.125 | 1.00x |
| flat.json | orjson | 0.466 | 0.547 | 0.675 | 49.125 | 0.99x |
| flat.json | msgspec | 0.698 | 0.762 | 0.847 | 49.125 | 0.71x |
| flat.json | ujson | 1.850 | 1.965 | 2.224 | 49.125 | 0.27x |
| flat.json | json | 2.213 | 2.364 | 2.675 | 49.125 | 0.23x |
| nested.json | strata | 0.323 | 0.346 | 0.440 | 49.160 | 1.00x |
| nested.json | orjson | 0.357 | 0.372 | 0.541 | 49.160 | 0.93x |
| nested.json | msgspec | 0.570 | 0.592 | 0.721 | 49.160 | 0.58x |
| nested.json | ujson | 1.368 | 1.393 | 1.583 | 49.160 | 0.25x |
| nested.json | json | 2.583 | 2.749 | 2.847 | 49.160 | 0.13x |
| wide_arrays.json | strata | 2.816 | 3.301 | 4.145 | 50.223 | 1.00x |
| wide_arrays.json | orjson | 3.038 | 3.281 | 3.562 | 50.223 | 1.01x |
| wide_arrays.json | msgspec | 4.938 | 5.340 | 5.596 | 50.223 | 0.62x |
| wide_arrays.json | ujson | 10.796 | 11.165 | 11.702 | 50.223 | 0.30x |
| wide_arrays.json | json | 19.710 | 20.617 | 34.961 | 50.223 | 0.16x |
| mixed.json | strata | 0.095 | 0.105 | 0.148 | 48.629 | 1.00x |
| mixed.json | orjson | 0.084 | 0.092 | 0.140 | 48.629 | 1.13x |
| mixed.json | msgspec | 0.114 | 0.124 | 0.171 | 48.629 | 0.84x |
| mixed.json | ujson | 0.348 | 0.415 | 0.627 | 48.629 | 0.25x |
| mixed.json | json | 0.543 | 0.633 | 0.984 | 48.629 | 0.17x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 15.275 | 16.882 | 19.056 | 54.125 | 1.00x |
| users.json | orjson | 18.785 | 20.010 | 26.162 | 54.125 | 0.84x |
| users.json | msgspec | 16.643 | 19.436 | 22.765 | 54.125 | 0.87x |
| users.json | ujson | 30.069 | 32.103 | 38.196 | 54.125 | 0.53x |
| users.json | json | 27.040 | 29.086 | 49.188 | 54.125 | 0.58x |
| flat.json | strata | 1.863 | 1.991 | 3.230 | 48.184 | 1.00x |
| flat.json | orjson | 1.877 | 2.075 | 3.172 | 48.184 | 0.96x |
| flat.json | msgspec | 1.674 | 1.868 | 3.191 | 48.184 | 1.07x |
| flat.json | ujson | 3.496 | 3.629 | 5.735 | 48.184 | 0.55x |
| flat.json | json | 2.649 | 2.851 | 4.367 | 48.184 | 0.70x |
| nested.json | strata | 1.366 | 1.682 | 2.778 | 48.668 | 1.00x |
| nested.json | orjson | 1.432 | 1.787 | 2.460 | 48.668 | 0.94x |
| nested.json | msgspec | 1.393 | 1.581 | 1.818 | 48.668 | 1.06x |
| nested.json | ujson | 2.323 | 2.573 | 4.576 | 48.668 | 0.65x |
| nested.json | json | 2.571 | 2.803 | 4.851 | 48.668 | 0.60x |
| wide_arrays.json | strata | 7.277 | 7.575 | 8.585 | 49.461 | 1.00x |
| wide_arrays.json | orjson | 7.479 | 7.573 | 8.069 | 49.461 | 1.00x |
| wide_arrays.json | msgspec | 7.529 | 7.715 | 7.839 | 49.461 | 0.98x |
| wide_arrays.json | ujson | 11.903 | 12.049 | 14.590 | 49.461 | 0.63x |
| wide_arrays.json | json | 13.512 | 13.745 | 23.307 | 49.461 | 0.55x |
| mixed.json | strata | 0.342 | 0.353 | 0.486 | 48.383 | 1.00x |
| mixed.json | orjson | 0.432 | 0.505 | 0.567 | 48.383 | 0.70x |
| mixed.json | msgspec | 0.419 | 0.473 | 0.616 | 48.383 | 0.75x |
| mixed.json | ujson | 0.642 | 0.712 | 0.905 | 48.383 | 0.50x |
| mixed.json | json | 0.651 | 0.711 | 0.802 | 48.383 | 0.50x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.374 | 18.829 | 28.861 | 49.902 | 1.00x |
| users.ndjson | orjson | 24.122 | 25.107 | 40.347 | 49.902 | 0.75x |
| users.ndjson | msgspec | 23.234 | 24.379 | 26.924 | 49.902 | 0.77x |
| users.ndjson | ujson | 33.496 | 34.455 | 50.441 | 49.902 | 0.55x |
| users.ndjson | json | 35.481 | 37.469 | 49.803 | 49.902 | 0.50x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.282 | 5.682 | 7.098 | 50.102 | 1.00x |
| users.json | orjson | 5.587 | 6.417 | 7.046 | 50.102 | 0.89x |
| users.json | msgspec | 7.918 | 8.304 | 8.773 | 50.102 | 0.68x |
| users.json | ujson | 27.946 | 29.011 | 42.636 | 50.102 | 0.20x |
| users.json | json | 35.239 | 37.429 | 56.384 | 50.102 | 0.15x |
| flat.json | strata | 1.041 | 1.157 | 1.282 | 49.023 | 1.00x |
| flat.json | orjson | 1.040 | 1.242 | 1.332 | 49.023 | 0.93x |
| flat.json | msgspec | 1.215 | 1.365 | 1.515 | 49.023 | 0.85x |
| flat.json | ujson | 3.571 | 3.658 | 3.697 | 49.023 | 0.32x |
| flat.json | json | 3.971 | 4.077 | 4.349 | 49.023 | 0.28x |
| nested.json | strata | 0.766 | 0.910 | 1.031 | 49.102 | 1.00x |
| nested.json | orjson | 0.838 | 0.922 | 1.057 | 49.102 | 0.99x |
| nested.json | msgspec | 1.048 | 1.156 | 1.289 | 49.102 | 0.79x |
| nested.json | ujson | 2.592 | 2.744 | 2.902 | 49.102 | 0.33x |
| nested.json | json | 3.985 | 4.093 | 4.198 | 49.102 | 0.22x |
| wide_arrays.json | strata | 3.663 | 3.884 | 6.717 | 50.223 | 1.00x |
| wide_arrays.json | orjson | 3.812 | 4.225 | 5.562 | 50.223 | 0.92x |
| wide_arrays.json | msgspec | 5.854 | 6.069 | 8.898 | 50.223 | 0.64x |
| wide_arrays.json | ujson | 17.963 | 18.372 | 39.392 | 50.223 | 0.21x |
| wide_arrays.json | json | 27.207 | 27.737 | 37.266 | 50.223 | 0.14x |
| mixed.json | strata | 0.479 | 0.522 | 0.568 | 48.512 | 1.00x |
| mixed.json | orjson | 0.481 | 0.530 | 0.670 | 48.512 | 0.98x |
| mixed.json | msgspec | 0.499 | 0.572 | 0.610 | 48.512 | 0.91x |
| mixed.json | ujson | 0.892 | 0.983 | 1.248 | 48.512 | 0.53x |
| mixed.json | json | 1.080 | 1.167 | 1.301 | 48.512 | 0.45x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.151 | 0.183 | 0.231 | 49.297 | 1.00x |
| users.json $[*].id | jmespath | 0.537 | 0.612 | 0.688 | 49.297 | 0.30x |
| users.json $[*].id | jsonpath-ng | 3.311 | 3.503 | 4.210 | 49.297 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.782 | 1.068 | 1.397 | 50.500 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.201 | 3.909 | 6.881 | 50.500 | 0.27x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.961 | 22.469 | 25.532 | 50.500 | 0.05x |
| users.json $..total | strata | 2.474 | 2.722 | 3.028 | 52.562 | 1.00x |
| users.json $..total | jsonpath-ng | 348.184 | 364.689 | 388.155 | 52.562 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 6.099 | 6.182 | 9.105 | 50.340 | 1.00x |
| users.json $[*].id | orjson+jmespath | 20.097 | 20.743 | 26.564 | 50.340 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 21.842 | 23.434 | 39.223 | 50.340 | 0.26x |
| users.json $[*].orders[*].total | strata | 6.232 | 6.417 | 6.593 | 52.109 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 23.160 | 25.122 | 32.187 | 52.109 | 0.26x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 47.244 | 50.488 | 61.035 | 52.109 | 0.13x |
| users.json $..total | strata | 20.829 | 21.923 | 23.936 | 52.145 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 385.269 | 404.742 | 443.606 | 52.145 | 0.05x |

