# Benchmark results - ci-windows-latest

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 16b0a58
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: -std=c++20 -O3 -march=native
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.507 | 13.155 | 20.436 | 45.629 | 1.00x |
| users.json | orjson | 12.922 | 13.545 | 19.682 | 45.629 | 0.97x |
| users.json | msgspec | 12.530 | 12.773 | 21.462 | 45.629 | 1.03x |
| users.json | ujson | 20.541 | 22.848 | 34.972 | 45.629 | 0.58x |
| users.json | json | 22.516 | 23.173 | 43.152 | 45.629 | 0.57x |
| flat.json | strata | 1.270 | 1.313 | 1.419 | 50.355 | 1.00x |
| flat.json | orjson | 1.112 | 1.171 | 1.194 | 50.355 | 1.12x |
| flat.json | msgspec | 1.088 | 1.116 | 1.154 | 50.355 | 1.18x |
| flat.json | ujson | 2.048 | 2.066 | 2.113 | 50.355 | 0.64x |
| flat.json | json | 2.015 | 2.053 | 2.335 | 50.355 | 0.64x |
| nested.json | strata | 1.061 | 1.098 | 1.808 | 49.926 | 1.00x |
| nested.json | orjson | 1.060 | 1.098 | 1.633 | 49.926 | 1.00x |
| nested.json | msgspec | 0.998 | 1.036 | 1.988 | 49.926 | 1.06x |
| nested.json | ujson | 1.577 | 1.615 | 2.794 | 49.926 | 0.68x |
| nested.json | json | 2.136 | 2.184 | 3.938 | 49.926 | 0.50x |
| wide_arrays.json | strata | 5.963 | 6.012 | 6.392 | 52.184 | 1.00x |
| wide_arrays.json | orjson | 5.494 | 5.538 | 8.696 | 52.184 | 1.09x |
| wide_arrays.json | msgspec | 5.810 | 5.848 | 9.925 | 52.184 | 1.03x |
| wide_arrays.json | ujson | 8.401 | 8.524 | 13.174 | 52.184 | 0.71x |
| wide_arrays.json | json | 11.624 | 11.688 | 15.450 | 52.184 | 0.51x |
| mixed.json | strata | 0.285 | 0.294 | 0.522 | 50.121 | 1.00x |
| mixed.json | orjson | 0.208 | 0.211 | 0.236 | 50.121 | 1.39x |
| mixed.json | msgspec | 0.230 | 0.233 | 0.341 | 50.121 | 1.26x |
| mixed.json | ujson | 0.358 | 0.366 | 0.516 | 50.121 | 0.80x |
| mixed.json | json | 0.467 | 0.479 | 0.523 | 50.121 | 0.61x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.380 | 4.437 | 5.774 | 44.816 | 1.00x |
| users.json | orjson | 3.467 | 3.515 | 5.565 | 44.816 | 1.26x |
| users.json | msgspec | 4.857 | 4.904 | 7.877 | 44.816 | 0.90x |
| users.json | ujson | 16.392 | 17.332 | 21.783 | 44.816 | 0.26x |
| users.json | json | 22.908 | 23.250 | 24.046 | 44.816 | 0.19x |
| flat.json | strata | 0.497 | 0.545 | 0.625 | 49.754 | 1.00x |
| flat.json | orjson | 0.330 | 0.369 | 0.440 | 49.754 | 1.48x |
| flat.json | msgspec | 0.503 | 0.572 | 0.973 | 49.754 | 0.95x |
| flat.json | ujson | 1.703 | 1.777 | 2.510 | 49.754 | 0.31x |
| flat.json | json | 1.944 | 2.094 | 2.235 | 49.754 | 0.26x |
| nested.json | strata | 0.360 | 0.371 | 0.395 | 50.461 | 1.00x |
| nested.json | orjson | 0.317 | 0.348 | 0.362 | 50.461 | 1.07x |
| nested.json | msgspec | 0.465 | 0.469 | 0.505 | 50.461 | 0.79x |
| nested.json | ujson | 1.387 | 1.421 | 1.454 | 50.461 | 0.26x |
| nested.json | json | 2.438 | 2.482 | 2.787 | 50.461 | 0.15x |
| wide_arrays.json | strata | 2.486 | 2.510 | 3.180 | 51.078 | 1.00x |
| wide_arrays.json | orjson | 2.356 | 2.590 | 2.834 | 51.078 | 0.97x |
| wide_arrays.json | msgspec | 3.543 | 3.611 | 3.728 | 51.078 | 0.70x |
| wide_arrays.json | ujson | 9.440 | 9.693 | 12.315 | 51.078 | 0.26x |
| wide_arrays.json | json | 18.454 | 18.807 | 30.969 | 51.078 | 0.13x |
| mixed.json | strata | 0.098 | 0.099 | 0.156 | 50.250 | 1.00x |
| mixed.json | orjson | 0.069 | 0.070 | 0.089 | 50.250 | 1.42x |
| mixed.json | msgspec | 0.094 | 0.095 | 0.104 | 50.250 | 1.04x |
| mixed.json | ujson | 0.329 | 0.332 | 0.380 | 50.250 | 0.30x |
| mixed.json | json | 0.515 | 0.544 | 0.651 | 50.250 | 0.18x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 15.360 | 15.921 | 25.462 | 54.262 | 1.00x |
| users.json | orjson | 14.371 | 14.754 | 19.686 | 54.262 | 1.08x |
| users.json | msgspec | 14.052 | 14.208 | 15.282 | 54.262 | 1.12x |
| users.json | ujson | 26.767 | 27.232 | 41.773 | 54.262 | 0.58x |
| users.json | json | 23.929 | 24.485 | 30.157 | 54.262 | 0.65x |
| flat.json | strata | 1.503 | 1.662 | 2.519 | 50.371 | 1.00x |
| flat.json | orjson | 1.514 | 1.587 | 2.557 | 50.371 | 1.05x |
| flat.json | msgspec | 1.230 | 1.334 | 2.171 | 50.371 | 1.25x |
| flat.json | ujson | 2.706 | 2.828 | 4.131 | 50.371 | 0.59x |
| flat.json | json | 2.136 | 2.162 | 3.854 | 50.371 | 0.77x |
| nested.json | strata | 1.223 | 1.244 | 1.255 | 50.234 | 1.00x |
| nested.json | orjson | 1.178 | 1.207 | 1.240 | 50.234 | 1.03x |
| nested.json | msgspec | 1.171 | 1.180 | 1.221 | 50.234 | 1.05x |
| nested.json | ujson | 2.036 | 2.058 | 2.142 | 50.234 | 0.60x |
| nested.json | json | 2.312 | 2.339 | 2.448 | 50.234 | 0.53x |
| wide_arrays.json | strata | 7.364 | 7.465 | 10.342 | 51.078 | 1.00x |
| wide_arrays.json | orjson | 6.002 | 6.097 | 9.593 | 51.078 | 1.22x |
| wide_arrays.json | msgspec | 6.353 | 6.515 | 10.461 | 51.078 | 1.15x |
| wide_arrays.json | ujson | 11.580 | 11.691 | 14.163 | 51.078 | 0.64x |
| wide_arrays.json | json | 12.351 | 12.420 | 12.772 | 51.078 | 0.60x |
| mixed.json | strata | 0.357 | 0.364 | 0.461 | 50.250 | 1.00x |
| mixed.json | orjson | 0.321 | 0.325 | 0.450 | 50.250 | 1.12x |
| mixed.json | msgspec | 0.343 | 0.348 | 0.411 | 50.250 | 1.05x |
| mixed.json | ujson | 0.547 | 0.563 | 0.612 | 50.250 | 0.65x |
| mixed.json | json | 0.587 | 0.599 | 0.651 | 50.250 | 0.61x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 14.247 | 14.700 | 23.965 | 50.734 | 1.00x |
| users.ndjson | orjson | 17.533 | 17.726 | 25.163 | 50.734 | 0.83x |
| users.ndjson | msgspec | 17.204 | 17.549 | 20.657 | 50.734 | 0.84x |
| users.ndjson | ujson | 26.574 | 27.778 | 41.243 | 50.734 | 0.53x |
| users.ndjson | json | 30.807 | 31.912 | 50.545 | 50.734 | 0.46x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.391 | 5.505 | 6.646 | 51.027 | 1.00x |
| users.json | orjson | 4.370 | 4.469 | 7.260 | 51.027 | 1.23x |
| users.json | msgspec | 5.821 | 5.931 | 9.316 | 51.027 | 0.93x |
| users.json | ujson | 25.398 | 25.781 | 26.244 | 51.027 | 0.21x |
| users.json | json | 32.646 | 33.076 | 48.837 | 51.027 | 0.17x |
| flat.json | strata | 0.896 | 0.947 | 1.037 | 50.398 | 1.00x |
| flat.json | orjson | 0.711 | 0.752 | 0.825 | 50.398 | 1.26x |
| flat.json | msgspec | 0.863 | 0.903 | 0.935 | 50.398 | 1.05x |
| flat.json | ujson | 3.077 | 3.108 | 3.273 | 50.398 | 0.30x |
| flat.json | json | 3.303 | 3.364 | 3.611 | 50.398 | 0.28x |
| nested.json | strata | 0.735 | 0.782 | 1.097 | 50.422 | 1.00x |
| nested.json | orjson | 0.662 | 0.707 | 0.927 | 50.422 | 1.11x |
| nested.json | msgspec | 0.807 | 0.848 | 1.276 | 50.422 | 0.92x |
| nested.json | ujson | 2.369 | 2.391 | 4.087 | 50.422 | 0.33x |
| nested.json | json | 3.490 | 3.550 | 6.432 | 50.422 | 0.22x |
| wide_arrays.json | strata | 3.268 | 3.333 | 6.380 | 51.082 | 1.00x |
| wide_arrays.json | orjson | 3.256 | 3.324 | 3.688 | 51.082 | 1.00x |
| wide_arrays.json | msgspec | 4.330 | 4.360 | 5.382 | 51.082 | 0.76x |
| wide_arrays.json | ujson | 16.251 | 16.508 | 27.199 | 51.082 | 0.20x |
| wide_arrays.json | json | 25.331 | 25.416 | 30.037 | 51.082 | 0.13x |
| mixed.json | strata | 0.448 | 0.461 | 0.604 | 50.277 | 1.00x |
| mixed.json | orjson | 0.387 | 0.404 | 0.531 | 50.277 | 1.14x |
| mixed.json | msgspec | 0.410 | 0.468 | 0.537 | 50.277 | 0.98x |
| mixed.json | ujson | 0.852 | 0.873 | 1.274 | 50.277 | 0.53x |
| mixed.json | json | 0.996 | 1.002 | 1.816 | 50.277 | 0.46x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.090 | 0.095 | 0.098 | 51.051 | 1.00x |
| users.json $[*].id | jmespath | 0.437 | 0.446 | 0.481 | 51.051 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.451 | 2.673 | 2.788 | 51.051 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.492 | 0.518 | 0.598 | 50.945 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.738 | 2.787 | 5.380 | 50.945 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 16.944 | 17.258 | 32.279 | 50.945 | 0.03x |
| users.json $..total | strata | 1.966 | 1.996 | 2.102 | 52.922 | 1.00x |
| users.json $..total | jsonpath-ng | 327.906 | 351.195 | 410.340 | 52.922 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 6.791 | 7.004 | 11.926 | 50.430 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.789 | 15.092 | 19.205 | 50.430 | 0.46x |
| users.json $[*].id | orjson+jsonpath-ng | 16.919 | 17.535 | 20.152 | 50.430 | 0.40x |
| users.json $[*].orders[*].total | strata | 6.791 | 6.898 | 7.178 | 52.836 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.423 | 18.095 | 18.678 | 52.836 | 0.38x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.151 | 36.628 | 50.603 | 52.836 | 0.19x |
| users.json $..total | strata | 17.794 | 18.423 | 30.230 | 52.875 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 347.246 | 372.714 | 382.229 | 52.875 | 0.05x |

