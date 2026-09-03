# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e8ac860
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: x86_64
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.134 | 10.431 | 18.631 | 58.301 | 1.00x |
| users.json | orjson | 13.343 | 13.485 | 19.488 | 58.301 | 0.77x |
| users.json | msgspec | 13.264 | 13.516 | 16.826 | 58.301 | 0.77x |
| users.json | ujson | 18.026 | 19.376 | 24.155 | 58.301 | 0.54x |
| users.json | pysimdjson | 18.700 | 19.974 | 30.692 | 58.301 | 0.52x |
| users.json | json | 22.515 | 23.100 | 34.333 | 58.301 | 0.45x |
| flat.json | strata | 0.859 | 0.879 | 0.899 | 64.641 | 1.00x |
| flat.json | orjson | 0.979 | 0.990 | 1.016 | 64.641 | 0.89x |
| flat.json | msgspec | 1.016 | 1.024 | 1.220 | 64.641 | 0.86x |
| flat.json | ujson | 1.469 | 1.528 | 1.659 | 64.641 | 0.58x |
| flat.json | pysimdjson | 1.539 | 1.559 | 1.583 | 64.641 | 0.56x |
| flat.json | json | 1.898 | 1.925 | 2.163 | 64.641 | 0.46x |
| nested.json | strata | 0.800 | 0.809 | 0.821 | 64.641 | 1.00x |
| nested.json | orjson | 1.001 | 1.009 | 1.015 | 64.641 | 0.80x |
| nested.json | msgspec | 1.015 | 1.027 | 1.095 | 64.641 | 0.79x |
| nested.json | ujson | 1.475 | 1.492 | 1.589 | 64.641 | 0.54x |
| nested.json | pysimdjson | 1.413 | 1.422 | 1.453 | 64.641 | 0.57x |
| nested.json | json | 2.007 | 2.020 | 2.066 | 64.641 | 0.40x |
| wide_arrays.json | strata | 4.619 | 4.643 | 4.818 | 73.023 | 1.00x |
| wide_arrays.json | orjson | 5.013 | 5.198 | 5.325 | 73.023 | 0.89x |
| wide_arrays.json | msgspec | 5.573 | 5.702 | 5.805 | 73.023 | 0.81x |
| wide_arrays.json | ujson | 7.157 | 7.256 | 7.635 | 73.023 | 0.64x |
| wide_arrays.json | pysimdjson | 5.978 | 6.159 | 6.358 | 73.023 | 0.75x |
| wide_arrays.json | json | 9.652 | 9.731 | 10.043 | 73.023 | 0.48x |
| mixed.json | strata | 0.194 | 0.201 | 0.210 | 73.086 | 1.00x |
| mixed.json | orjson | 0.230 | 0.233 | 0.258 | 73.086 | 0.86x |
| mixed.json | msgspec | 0.241 | 0.244 | 0.262 | 73.086 | 0.83x |
| mixed.json | ujson | 0.304 | 0.313 | 0.335 | 73.086 | 0.64x |
| mixed.json | pysimdjson | 0.299 | 0.303 | 0.324 | 73.086 | 0.66x |
| mixed.json | json | 0.474 | 0.485 | 0.523 | 73.086 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.506 | 2.513 | 2.664 | 57.234 | 1.00x |
| users.json | orjson | 2.897 | 2.914 | 3.199 | 57.234 | 0.86x |
| users.json | msgspec | 3.840 | 3.903 | 4.043 | 57.234 | 0.64x |
| users.json | ujson | 14.792 | 14.919 | 15.490 | 57.234 | 0.17x |
| users.json | json | 21.534 | 21.769 | 22.069 | 57.234 | 0.12x |
| flat.json | strata | 0.266 | 0.272 | 0.281 | 64.641 | 1.00x |
| flat.json | orjson | 0.329 | 0.332 | 0.338 | 64.641 | 0.82x |
| flat.json | msgspec | 0.429 | 0.436 | 0.454 | 64.641 | 0.63x |
| flat.json | ujson | 1.291 | 1.305 | 1.327 | 64.641 | 0.21x |
| flat.json | json | 1.843 | 1.858 | 1.977 | 64.641 | 0.15x |
| nested.json | strata | 0.256 | 0.259 | 0.280 | 64.641 | 1.00x |
| nested.json | orjson | 0.288 | 0.292 | 0.307 | 64.641 | 0.89x |
| nested.json | msgspec | 0.401 | 0.413 | 0.429 | 64.641 | 0.63x |
| nested.json | ujson | 1.375 | 1.392 | 1.613 | 64.641 | 0.19x |
| nested.json | json | 2.361 | 2.411 | 2.497 | 64.641 | 0.11x |
| wide_arrays.json | strata | 1.560 | 1.572 | 1.620 | 73.023 | 1.00x |
| wide_arrays.json | orjson | 1.797 | 1.817 | 1.874 | 73.023 | 0.86x |
| wide_arrays.json | msgspec | 2.676 | 2.684 | 2.718 | 73.023 | 0.59x |
| wide_arrays.json | ujson | 8.486 | 8.538 | 8.636 | 73.023 | 0.18x |
| wide_arrays.json | json | 16.289 | 16.372 | 16.568 | 73.023 | 0.10x |
| mixed.json | strata | 0.065 | 0.066 | 0.076 | 73.086 | 1.00x |
| mixed.json | orjson | 0.063 | 0.065 | 0.078 | 73.086 | 1.02x |
| mixed.json | msgspec | 0.084 | 0.085 | 0.094 | 73.086 | 0.77x |
| mixed.json | ujson | 0.304 | 0.311 | 0.331 | 73.086 | 0.21x |
| mixed.json | json | 0.506 | 0.522 | 0.527 | 73.086 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.966 | 11.211 | 13.440 | 69.820 | 1.00x |
| users.json | orjson | 13.845 | 14.229 | 14.967 | 69.820 | 0.79x |
| users.json | msgspec | 14.041 | 14.097 | 14.611 | 69.820 | 0.80x |
| users.json | ujson | 19.502 | 19.835 | 32.683 | 69.820 | 0.57x |
| users.json | json | 23.196 | 23.580 | 24.043 | 69.820 | 0.48x |
| flat.json | strata | 0.898 | 0.904 | 0.943 | 64.641 | 1.00x |
| flat.json | orjson | 1.035 | 1.057 | 1.072 | 64.641 | 0.86x |
| flat.json | msgspec | 1.080 | 1.086 | 1.116 | 64.641 | 0.83x |
| flat.json | ujson | 1.571 | 1.624 | 1.888 | 64.641 | 0.56x |
| flat.json | json | 1.963 | 1.979 | 1.994 | 64.641 | 0.46x |
| nested.json | strata | 0.819 | 0.838 | 0.869 | 64.641 | 1.00x |
| nested.json | orjson | 1.052 | 1.066 | 1.083 | 64.641 | 0.79x |
| nested.json | msgspec | 1.089 | 1.105 | 1.130 | 64.641 | 0.76x |
| nested.json | ujson | 1.550 | 1.567 | 1.607 | 64.641 | 0.54x |
| nested.json | json | 2.077 | 2.100 | 2.152 | 64.641 | 0.40x |
| wide_arrays.json | strata | 4.726 | 4.779 | 4.885 | 73.086 | 1.00x |
| wide_arrays.json | orjson | 5.198 | 5.289 | 5.443 | 73.086 | 0.90x |
| wide_arrays.json | msgspec | 5.771 | 5.835 | 7.026 | 73.086 | 0.82x |
| wide_arrays.json | ujson | 7.486 | 7.562 | 7.707 | 73.086 | 0.63x |
| wide_arrays.json | json | 9.781 | 9.834 | 9.979 | 73.086 | 0.49x |
| mixed.json | strata | 0.213 | 0.216 | 0.283 | 73.086 | 1.00x |
| mixed.json | orjson | 0.276 | 0.282 | 0.304 | 73.086 | 0.77x |
| mixed.json | msgspec | 0.280 | 0.285 | 0.302 | 73.086 | 0.76x |
| mixed.json | ujson | 0.357 | 0.376 | 0.386 | 73.086 | 0.58x |
| mixed.json | json | 0.515 | 0.518 | 0.540 | 73.086 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.255 | 12.801 | 14.980 | 64.637 | 1.00x |
| users.ndjson | orjson | 17.586 | 18.200 | 21.525 | 64.637 | 0.70x |
| users.ndjson | msgspec | 17.250 | 17.816 | 22.093 | 64.637 | 0.72x |
| users.ndjson | ujson | 22.278 | 25.462 | 27.970 | 64.637 | 0.50x |
| users.ndjson | json | 30.476 | 32.896 | 34.233 | 64.637 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.091 | 3.128 | 3.170 | 65.883 | 1.00x |
| users.json | orjson | 3.545 | 3.588 | 3.664 | 65.883 | 0.87x |
| users.json | msgspec | 4.454 | 4.485 | 4.540 | 65.883 | 0.70x |
| users.json | ujson | 15.566 | 15.687 | 16.684 | 65.883 | 0.20x |
| users.json | json | 22.628 | 22.941 | 34.292 | 65.883 | 0.14x |
| flat.json | strata | 0.415 | 0.433 | 0.577 | 64.641 | 1.00x |
| flat.json | orjson | 0.493 | 0.505 | 0.529 | 64.641 | 0.86x |
| flat.json | msgspec | 0.612 | 0.620 | 0.642 | 64.641 | 0.70x |
| flat.json | ujson | 1.469 | 1.493 | 1.521 | 64.641 | 0.29x |
| flat.json | json | 2.029 | 2.064 | 2.111 | 64.641 | 0.21x |
| nested.json | strata | 0.372 | 0.395 | 0.403 | 64.641 | 1.00x |
| nested.json | orjson | 0.423 | 0.445 | 0.468 | 64.641 | 0.89x |
| nested.json | msgspec | 0.531 | 0.555 | 0.617 | 64.641 | 0.71x |
| nested.json | ujson | 1.564 | 1.575 | 1.637 | 64.641 | 0.25x |
| nested.json | json | 2.524 | 2.564 | 2.637 | 64.641 | 0.15x |
| wide_arrays.json | strata | 1.997 | 2.023 | 2.123 | 73.086 | 1.00x |
| wide_arrays.json | orjson | 2.250 | 2.306 | 2.584 | 73.086 | 0.88x |
| wide_arrays.json | msgspec | 3.158 | 3.198 | 3.306 | 73.086 | 0.63x |
| wide_arrays.json | ujson | 9.067 | 9.160 | 9.279 | 73.086 | 0.22x |
| wide_arrays.json | json | 16.918 | 17.064 | 17.797 | 73.086 | 0.12x |
| mixed.json | strata | 0.155 | 0.157 | 0.194 | 73.086 | 1.00x |
| mixed.json | orjson | 0.171 | 0.175 | 0.225 | 73.086 | 0.90x |
| mixed.json | msgspec | 0.192 | 0.197 | 0.223 | 73.086 | 0.80x |
| mixed.json | ujson | 0.418 | 0.444 | 0.468 | 73.086 | 0.35x |
| mixed.json | json | 0.621 | 0.642 | 0.670 | 73.086 | 0.24x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.064 | 0.067 | 0.078 | 65.883 | 1.00x |
| users.json $[*].id | jmespath | 0.490 | 0.510 | 0.523 | 65.883 | 0.13x |
| users.json $[*].id | jsonpath-ng | 2.866 | 2.885 | 2.980 | 65.883 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.451 | 0.461 | 0.473 | 66.992 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.112 | 3.170 | 3.557 | 66.992 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.589 | 20.334 | 22.310 | 66.992 | 0.02x |
| users.json $..total | strata | 1.680 | 1.697 | 1.753 | 69.094 | 1.00x |
| users.json $..total | jsonpath-ng | 390.121 | 394.414 | 404.952 | 69.094 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.224 | 3.264 | 3.306 | 66.992 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.237 | 14.511 | 15.021 | 66.992 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 16.607 | 16.931 | 17.833 | 66.992 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.463 | 3.525 | 3.699 | 68.328 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.943 | 17.457 | 18.281 | 68.328 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.258 | 37.670 | 38.774 | 68.328 | 0.09x |
| users.json $..total | strata | 13.186 | 13.580 | 14.381 | 68.469 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 412.976 | 420.452 | 428.119 | 68.469 | 0.03x |

