# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: fb7997e
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
| users.json | strata | 10.353 | 11.496 | 22.810 | 45.539 | 1.00x |
| users.json | orjson | 13.571 | 14.241 | 35.841 | 45.539 | 0.81x |
| users.json | msgspec | 12.724 | 13.547 | 25.162 | 45.539 | 0.85x |
| users.json | ujson | 22.452 | 26.102 | 32.736 | 45.539 | 0.44x |
| users.json | json | 23.115 | 24.517 | 60.481 | 45.539 | 0.47x |
| flat.json | strata | 1.654 | 1.843 | 4.660 | 48.703 | 1.00x |
| flat.json | orjson | 1.840 | 3.647 | 14.158 | 48.703 | 0.51x |
| flat.json | msgspec | 2.042 | 2.580 | 4.645 | 48.703 | 0.71x |
| flat.json | ujson | 3.752 | 5.545 | 56.309 | 48.703 | 0.33x |
| flat.json | json | 3.655 | 4.872 | 6.125 | 48.703 | 0.38x |
| nested.json | strata | 1.409 | 1.597 | 2.324 | 48.277 | 1.00x |
| nested.json | orjson | 1.853 | 2.002 | 4.839 | 48.277 | 0.80x |
| nested.json | msgspec | 1.788 | 1.844 | 4.557 | 48.277 | 0.87x |
| nested.json | ujson | 2.983 | 3.253 | 98.909 | 48.277 | 0.49x |
| nested.json | json | 3.999 | 5.271 | 6.869 | 48.277 | 0.30x |
| wide_arrays.json | strata | 7.862 | 9.322 | 13.292 | 50.559 | 1.00x |
| wide_arrays.json | orjson | 9.593 | 11.557 | 67.755 | 50.559 | 0.81x |
| wide_arrays.json | msgspec | 9.998 | 10.809 | 19.716 | 50.559 | 0.86x |
| wide_arrays.json | ujson | 13.913 | 15.225 | 75.284 | 50.559 | 0.61x |
| wide_arrays.json | json | 21.976 | 22.930 | 36.941 | 50.559 | 0.41x |
| mixed.json | strata | 0.340 | 0.372 | 0.460 | 49.883 | 1.00x |
| mixed.json | orjson | 0.360 | 0.397 | 0.410 | 49.883 | 0.94x |
| mixed.json | msgspec | 0.417 | 0.464 | 0.534 | 49.883 | 0.80x |
| mixed.json | ujson | 0.605 | 0.672 | 0.802 | 49.883 | 0.55x |
| mixed.json | json | 0.863 | 0.880 | 0.964 | 49.883 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.265 | 3.455 | 3.751 | 44.809 | 1.00x |
| users.json | orjson | 3.821 | 3.988 | 4.335 | 44.809 | 0.87x |
| users.json | msgspec | 5.556 | 5.753 | 6.050 | 44.809 | 0.60x |
| users.json | ujson | 17.512 | 17.972 | 18.661 | 44.809 | 0.19x |
| users.json | json | 24.267 | 24.606 | 41.183 | 44.809 | 0.14x |
| flat.json | strata | 0.507 | 0.572 | 0.774 | 48.527 | 1.00x |
| flat.json | orjson | 0.671 | 0.865 | 3.037 | 48.527 | 0.66x |
| flat.json | msgspec | 0.782 | 0.868 | 2.959 | 48.527 | 0.66x |
| flat.json | ujson | 2.948 | 3.130 | 5.021 | 48.527 | 0.18x |
| flat.json | json | 3.961 | 5.622 | 6.047 | 48.527 | 0.10x |
| nested.json | strata | 0.426 | 0.443 | 3.313 | 48.742 | 1.00x |
| nested.json | orjson | 0.490 | 0.519 | 2.814 | 48.742 | 0.85x |
| nested.json | msgspec | 0.741 | 0.767 | 0.782 | 48.742 | 0.58x |
| nested.json | ujson | 2.446 | 2.478 | 2.518 | 48.742 | 0.18x |
| nested.json | json | 4.390 | 4.955 | 7.323 | 48.742 | 0.09x |
| wide_arrays.json | strata | 3.491 | 4.208 | 8.458 | 51.043 | 1.00x |
| wide_arrays.json | orjson | 4.086 | 4.204 | 62.542 | 51.043 | 1.00x |
| wide_arrays.json | msgspec | 6.093 | 6.808 | 12.379 | 51.043 | 0.62x |
| wide_arrays.json | ujson | 15.866 | 18.466 | 73.827 | 51.043 | 0.23x |
| wide_arrays.json | json | 30.620 | 34.142 | 69.067 | 51.043 | 0.12x |
| mixed.json | strata | 0.117 | 0.128 | 2.255 | 49.918 | 1.00x |
| mixed.json | orjson | 0.111 | 0.122 | 0.150 | 49.918 | 1.05x |
| mixed.json | msgspec | 0.146 | 0.155 | 0.176 | 49.918 | 0.82x |
| mixed.json | ujson | 0.573 | 0.593 | 2.445 | 49.918 | 0.22x |
| mixed.json | json | 0.916 | 0.956 | 2.974 | 49.918 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.056 | 13.219 | 15.201 | 54.832 | 1.00x |
| users.json | orjson | 14.828 | 15.814 | 22.335 | 54.832 | 0.84x |
| users.json | msgspec | 13.911 | 14.425 | 24.882 | 54.832 | 0.92x |
| users.json | ujson | 27.474 | 29.118 | 31.935 | 54.832 | 0.45x |
| users.json | json | 23.804 | 25.186 | 37.008 | 54.832 | 0.52x |
| flat.json | strata | 2.184 | 3.769 | 4.262 | 48.457 | 1.00x |
| flat.json | orjson | 2.177 | 2.589 | 4.729 | 48.457 | 1.46x |
| flat.json | msgspec | 2.281 | 2.769 | 4.636 | 48.457 | 1.36x |
| flat.json | ujson | 4.444 | 5.791 | 7.192 | 48.457 | 0.65x |
| flat.json | json | 4.304 | 5.350 | 9.025 | 48.457 | 0.70x |
| nested.json | strata | 1.549 | 1.668 | 3.873 | 48.594 | 1.00x |
| nested.json | orjson | 2.129 | 2.756 | 55.177 | 48.594 | 0.61x |
| nested.json | msgspec | 1.991 | 2.245 | 4.641 | 48.594 | 0.74x |
| nested.json | ujson | 3.440 | 5.475 | 6.069 | 48.594 | 0.30x |
| nested.json | json | 4.272 | 6.187 | 8.315 | 48.594 | 0.27x |
| wide_arrays.json | strata | 8.966 | 10.411 | 49.129 | 51.047 | 1.00x |
| wide_arrays.json | orjson | 10.683 | 11.046 | 19.562 | 51.047 | 0.94x |
| wide_arrays.json | msgspec | 11.346 | 11.499 | 19.326 | 51.047 | 0.91x |
| wide_arrays.json | ujson | 18.212 | 19.490 | 32.538 | 51.047 | 0.53x |
| wide_arrays.json | json | 22.836 | 24.473 | 79.542 | 51.047 | 0.43x |
| mixed.json | strata | 0.466 | 0.539 | 0.623 | 49.918 | 1.00x |
| mixed.json | orjson | 0.569 | 0.666 | 48.428 | 49.918 | 0.81x |
| mixed.json | msgspec | 0.602 | 0.744 | 2.945 | 49.918 | 0.72x |
| mixed.json | ujson | 0.864 | 1.026 | 1.357 | 49.918 | 0.53x |
| mixed.json | json | 1.031 | 1.177 | 1.247 | 49.918 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.797 | 34.514 | 85.990 | 49.613 | 1.00x |
| users.ndjson | orjson | 27.778 | 49.769 | 55.498 | 49.613 | 0.69x |
| users.ndjson | msgspec | 35.663 | 48.717 | 102.836 | 49.613 | 0.71x |
| users.ndjson | ujson | 48.822 | 69.322 | 120.898 | 49.613 | 0.50x |
| users.ndjson | json | 69.743 | 86.666 | 113.060 | 49.613 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.104 | 4.320 | 6.110 | 51.930 | 1.00x |
| users.json | orjson | 4.714 | 5.163 | 9.335 | 51.930 | 0.84x |
| users.json | msgspec | 6.487 | 6.726 | 9.073 | 51.930 | 0.64x |
| users.json | ujson | 26.693 | 27.238 | 38.987 | 51.930 | 0.16x |
| users.json | json | 32.793 | 33.511 | 33.773 | 51.930 | 0.13x |
| flat.json | strata | 0.990 | 1.390 | 3.432 | 48.691 | 1.00x |
| flat.json | orjson | 1.017 | 1.770 | 4.016 | 48.691 | 0.79x |
| flat.json | msgspec | 1.281 | 1.782 | 4.019 | 48.691 | 0.78x |
| flat.json | ujson | 5.269 | 7.724 | 66.487 | 48.691 | 0.18x |
| flat.json | json | 6.228 | 9.106 | 11.667 | 48.691 | 0.15x |
| nested.json | strata | 0.919 | 1.078 | 3.021 | 48.891 | 1.00x |
| nested.json | orjson | 1.002 | 1.057 | 3.105 | 48.891 | 1.02x |
| nested.json | msgspec | 1.238 | 1.406 | 3.441 | 48.891 | 0.77x |
| nested.json | ujson | 4.629 | 6.314 | 68.297 | 48.891 | 0.17x |
| nested.json | json | 6.279 | 8.939 | 10.419 | 48.891 | 0.12x |
| wide_arrays.json | strata | 4.489 | 5.236 | 6.932 | 51.027 | 1.00x |
| wide_arrays.json | orjson | 4.381 | 5.257 | 12.381 | 51.027 | 1.00x |
| wide_arrays.json | msgspec | 6.989 | 7.758 | 16.243 | 51.027 | 0.67x |
| wide_arrays.json | ujson | 28.850 | 29.785 | 47.444 | 51.027 | 0.18x |
| wide_arrays.json | json | 43.201 | 45.284 | 111.173 | 51.027 | 0.12x |
| mixed.json | strata | 0.600 | 0.742 | 0.835 | 49.918 | 1.00x |
| mixed.json | orjson | 0.559 | 0.614 | 0.842 | 49.918 | 1.21x |
| mixed.json | msgspec | 0.587 | 0.637 | 0.874 | 49.918 | 1.16x |
| mixed.json | ujson | 1.357 | 1.430 | 1.806 | 49.918 | 0.52x |
| mixed.json | json | 1.733 | 1.856 | 1.976 | 49.918 | 0.40x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.095 | 0.098 | 0.157 | 51.941 | 1.00x |
| users.json $[*].id | jmespath | 0.444 | 0.460 | 0.880 | 51.941 | 0.21x |
| users.json $[*].id | jsonpath-ng | 2.619 | 2.821 | 4.989 | 51.941 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.478 | 0.491 | 1.051 | 51.945 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.781 | 3.019 | 5.594 | 51.945 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.358 | 20.226 | 38.905 | 51.945 | 0.02x |
| users.json $..total | strata | 1.865 | 2.049 | 3.959 | 52.945 | 1.00x |
| users.json $..total | jsonpath-ng | 339.112 | 365.550 | 490.454 | 52.945 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.028 | 4.126 | 6.707 | 51.945 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.385 | 17.053 | 19.810 | 51.945 | 0.24x |
| users.json $[*].id | orjson+jsonpath-ng | 17.460 | 18.938 | 24.584 | 51.945 | 0.22x |
| users.json $[*].orders[*].total | strata | 4.253 | 4.416 | 4.613 | 52.570 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.696 | 19.401 | 48.108 | 52.570 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.270 | 42.239 | 47.321 | 52.570 | 0.10x |
| users.json $..total | strata | 16.021 | 20.425 | 81.044 | 52.570 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 415.510 | 536.432 | 1222.700 | 52.570 | 0.04x |

