# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: f8c17d0
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: Intel64 Family 6 Model 207 Stepping 2, GenuineIntel
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.049 | 9.411 | 17.487 | 45.500 | 1.00x |
| users.json | orjson | 14.470 | 14.974 | 23.566 | 45.500 | 0.63x |
| users.json | msgspec | 13.362 | 13.646 | 19.894 | 45.500 | 0.69x |
| users.json | ujson | 19.702 | 20.604 | 33.284 | 45.500 | 0.46x |
| users.json | json | 22.428 | 23.352 | 31.320 | 45.500 | 0.40x |
| flat.json | strata | 1.162 | 1.216 | 1.277 | 50.348 | 1.00x |
| flat.json | orjson | 1.297 | 1.358 | 1.579 | 50.348 | 0.90x |
| flat.json | msgspec | 1.173 | 1.202 | 1.369 | 50.348 | 1.01x |
| flat.json | ujson | 1.705 | 1.759 | 2.059 | 50.348 | 0.69x |
| flat.json | json | 1.898 | 1.989 | 2.126 | 50.348 | 0.61x |
| nested.json | strata | 0.668 | 0.711 | 0.898 | 50.176 | 1.00x |
| nested.json | orjson | 0.986 | 1.045 | 1.284 | 50.176 | 0.68x |
| nested.json | msgspec | 0.817 | 0.854 | 1.103 | 50.176 | 0.83x |
| nested.json | ujson | 1.313 | 1.341 | 1.559 | 50.176 | 0.53x |
| nested.json | json | 1.857 | 1.918 | 2.230 | 50.176 | 0.37x |
| wide_arrays.json | strata | 4.178 | 4.443 | 6.908 | 52.148 | 1.00x |
| wide_arrays.json | orjson | 6.044 | 6.245 | 8.873 | 52.148 | 0.71x |
| wide_arrays.json | msgspec | 5.725 | 5.924 | 7.470 | 52.148 | 0.75x |
| wide_arrays.json | ujson | 8.039 | 8.231 | 8.641 | 52.148 | 0.54x |
| wide_arrays.json | json | 11.269 | 11.921 | 18.379 | 52.148 | 0.37x |
| mixed.json | strata | 0.184 | 0.187 | 0.308 | 49.930 | 1.00x |
| mixed.json | orjson | 0.287 | 0.292 | 0.393 | 49.930 | 0.64x |
| mixed.json | msgspec | 0.235 | 0.244 | 0.446 | 49.930 | 0.77x |
| mixed.json | ujson | 0.337 | 0.347 | 0.430 | 49.930 | 0.54x |
| mixed.json | json | 0.462 | 0.472 | 0.675 | 49.930 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.815 | 2.880 | 4.357 | 44.684 | 1.00x |
| users.json | orjson | 3.478 | 3.553 | 4.937 | 44.684 | 0.81x |
| users.json | msgspec | 6.097 | 6.115 | 8.245 | 44.684 | 0.47x |
| users.json | ujson | 14.933 | 15.015 | 21.443 | 44.684 | 0.19x |
| users.json | json | 22.432 | 22.921 | 32.660 | 44.684 | 0.13x |
| flat.json | strata | 0.336 | 0.355 | 0.489 | 50.488 | 1.00x |
| flat.json | orjson | 0.343 | 0.406 | 0.524 | 50.488 | 0.87x |
| flat.json | msgspec | 0.525 | 0.593 | 0.850 | 50.488 | 0.60x |
| flat.json | ujson | 1.348 | 1.455 | 2.022 | 50.488 | 0.24x |
| flat.json | json | 1.912 | 2.371 | 3.464 | 50.488 | 0.15x |
| nested.json | strata | 0.226 | 0.270 | 0.402 | 50.312 | 1.00x |
| nested.json | orjson | 0.338 | 0.410 | 1.899 | 50.312 | 0.66x |
| nested.json | msgspec | 0.469 | 0.511 | 0.814 | 50.312 | 0.53x |
| nested.json | ujson | 1.310 | 1.412 | 1.933 | 50.312 | 0.19x |
| nested.json | json | 2.488 | 2.648 | 5.030 | 50.312 | 0.10x |
| wide_arrays.json | strata | 2.190 | 2.288 | 3.501 | 50.961 | 1.00x |
| wide_arrays.json | orjson | 2.463 | 2.622 | 3.669 | 50.961 | 0.87x |
| wide_arrays.json | msgspec | 4.214 | 4.441 | 5.713 | 50.961 | 0.52x |
| wide_arrays.json | ujson | 9.448 | 9.593 | 9.798 | 50.961 | 0.24x |
| wide_arrays.json | json | 17.064 | 17.685 | 26.804 | 50.961 | 0.13x |
| mixed.json | strata | 0.071 | 0.077 | 0.104 | 50.047 | 1.00x |
| mixed.json | orjson | 0.067 | 0.069 | 0.101 | 50.047 | 1.11x |
| mixed.json | msgspec | 0.096 | 0.102 | 0.132 | 50.047 | 0.76x |
| mixed.json | ujson | 0.283 | 0.297 | 0.461 | 50.047 | 0.26x |
| mixed.json | json | 0.517 | 0.572 | 0.634 | 50.047 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.211 | 10.456 | 15.340 | 54.711 | 1.00x |
| users.json | orjson | 15.455 | 15.827 | 23.252 | 54.711 | 0.66x |
| users.json | msgspec | 14.611 | 14.869 | 22.565 | 54.711 | 0.70x |
| users.json | ujson | 23.507 | 24.079 | 37.314 | 54.711 | 0.43x |
| users.json | json | 23.513 | 24.013 | 24.668 | 54.711 | 0.44x |
| flat.json | strata | 1.456 | 1.541 | 1.770 | 49.953 | 1.00x |
| flat.json | orjson | 1.658 | 1.700 | 2.008 | 49.953 | 0.91x |
| flat.json | msgspec | 1.348 | 1.444 | 2.384 | 49.953 | 1.07x |
| flat.json | ujson | 2.320 | 2.355 | 2.972 | 49.953 | 0.65x |
| flat.json | json | 2.300 | 2.328 | 2.778 | 49.953 | 0.66x |
| nested.json | strata | 0.773 | 0.807 | 1.072 | 50.195 | 1.00x |
| nested.json | orjson | 1.150 | 1.186 | 1.380 | 50.195 | 0.68x |
| nested.json | msgspec | 0.959 | 1.022 | 1.266 | 50.195 | 0.79x |
| nested.json | ujson | 1.611 | 1.663 | 2.004 | 50.195 | 0.49x |
| nested.json | json | 2.034 | 2.062 | 2.125 | 50.195 | 0.39x |
| wide_arrays.json | strata | 5.024 | 5.151 | 8.855 | 50.961 | 1.00x |
| wide_arrays.json | orjson | 6.824 | 6.945 | 10.442 | 50.961 | 0.74x |
| wide_arrays.json | msgspec | 6.736 | 6.882 | 9.161 | 50.961 | 0.75x |
| wide_arrays.json | ujson | 10.858 | 11.088 | 11.701 | 50.961 | 0.46x |
| wide_arrays.json | json | 12.194 | 12.706 | 17.040 | 50.961 | 0.41x |
| mixed.json | strata | 0.281 | 0.341 | 0.430 | 49.938 | 1.00x |
| mixed.json | orjson | 0.417 | 0.448 | 0.678 | 49.938 | 0.76x |
| mixed.json | msgspec | 0.356 | 0.404 | 0.592 | 49.938 | 0.84x |
| mixed.json | ujson | 0.522 | 0.646 | 0.819 | 49.938 | 0.53x |
| mixed.json | json | 0.578 | 0.629 | 0.976 | 49.938 | 0.54x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.536 | 11.009 | 13.634 | 51.430 | 1.00x |
| users.ndjson | orjson | 17.272 | 18.241 | 26.618 | 51.430 | 0.60x |
| users.ndjson | msgspec | 16.950 | 17.709 | 25.992 | 51.430 | 0.62x |
| users.ndjson | ujson | 23.184 | 23.461 | 31.843 | 51.430 | 0.47x |
| users.ndjson | json | 28.651 | 29.250 | 31.736 | 51.430 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.030 | 4.195 | 6.037 | 52.902 | 1.00x |
| users.json | orjson | 4.445 | 4.804 | 6.454 | 52.902 | 0.87x |
| users.json | msgspec | 6.396 | 6.899 | 15.941 | 52.902 | 0.61x |
| users.json | ujson | 22.847 | 27.959 | 40.834 | 52.902 | 0.15x |
| users.json | json | 30.738 | 54.564 | 71.730 | 52.902 | 0.08x |
| flat.json | strata | 0.721 | 0.801 | 1.003 | 50.629 | 1.00x |
| flat.json | orjson | 0.734 | 0.786 | 0.967 | 50.629 | 1.02x |
| flat.json | msgspec | 0.919 | 1.027 | 1.388 | 50.629 | 0.78x |
| flat.json | ujson | 2.539 | 2.699 | 4.350 | 50.629 | 0.30x |
| flat.json | json | 3.134 | 3.548 | 6.331 | 50.629 | 0.23x |
| nested.json | strata | 0.601 | 0.634 | 1.053 | 50.438 | 1.00x |
| nested.json | orjson | 0.769 | 0.825 | 1.040 | 50.438 | 0.77x |
| nested.json | msgspec | 0.866 | 0.907 | 1.182 | 50.438 | 0.70x |
| nested.json | ujson | 2.371 | 2.425 | 4.447 | 50.438 | 0.26x |
| nested.json | json | 3.435 | 3.635 | 7.202 | 50.438 | 0.17x |
| wide_arrays.json | strata | 3.052 | 3.208 | 4.366 | 51.012 | 1.00x |
| wide_arrays.json | orjson | 3.519 | 3.678 | 4.874 | 51.012 | 0.87x |
| wide_arrays.json | msgspec | 5.321 | 5.538 | 8.274 | 51.012 | 0.58x |
| wide_arrays.json | ujson | 15.215 | 15.754 | 16.469 | 51.012 | 0.20x |
| wide_arrays.json | json | 23.209 | 23.751 | 29.837 | 51.012 | 0.14x |
| mixed.json | strata | 0.380 | 0.414 | 0.551 | 50.074 | 1.00x |
| mixed.json | orjson | 0.377 | 0.391 | 0.438 | 50.074 | 1.06x |
| mixed.json | msgspec | 0.420 | 0.438 | 0.479 | 50.074 | 0.95x |
| mixed.json | ujson | 0.727 | 0.788 | 0.819 | 50.074 | 0.53x |
| mixed.json | json | 0.963 | 1.012 | 1.097 | 50.074 | 0.41x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.092 | 0.120 | 0.132 | 52.918 | 1.00x |
| users.json $[*].id | jmespath | 0.398 | 0.444 | 0.770 | 52.918 | 0.27x |
| users.json $[*].id | jsonpath-ng | 2.243 | 2.711 | 25.545 | 52.918 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.512 | 0.529 | 3.252 | 52.922 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.384 | 2.456 | 2.536 | 52.922 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 15.167 | 16.082 | 192.183 | 52.922 | 0.03x |
| users.json $..total | strata | 1.817 | 1.838 | 1.932 | 53.551 | 1.00x |
| users.json $..total | jsonpath-ng | 289.252 | 313.851 | 361.141 | 53.551 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.369 | 4.633 | 10.094 | 52.922 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.546 | 22.794 | 54.215 | 52.922 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 18.229 | 19.012 | 25.919 | 52.922 | 0.24x |
| users.json $[*].orders[*].total | strata | 4.309 | 4.536 | 7.995 | 53.547 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.877 | 21.101 | 30.420 | 53.547 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 35.760 | 56.549 | 104.211 | 53.547 | 0.08x |
| users.json $..total | strata | 14.952 | 16.065 | 73.083 | 53.551 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 317.463 | 364.993 | 595.078 | 53.551 | 0.04x |

