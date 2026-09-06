# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 79fa3df
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
| users.json | strata | 19.507 | 19.872 | 25.122 | 52.715 | 1.00x |
| users.json | orjson | 28.039 | 28.401 | 37.127 | 52.715 | 0.70x |
| users.json | msgspec | 27.555 | 28.134 | 33.677 | 52.715 | 0.71x |
| users.json | ujson | 39.789 | 41.982 | 50.663 | 52.715 | 0.47x |
| users.json | pysimdjson | 178.235 | 181.738 | 190.923 | 52.715 | 0.11x |
| users.json | json | 46.094 | 47.497 | 50.687 | 52.715 | 0.42x |
| flat.json | strata | 1.430 | 1.449 | 1.514 | 58.461 | 1.00x |
| flat.json | orjson | 1.609 | 1.628 | 1.688 | 58.461 | 0.89x |
| flat.json | msgspec | 1.853 | 1.896 | 2.028 | 58.461 | 0.76x |
| flat.json | ujson | 3.242 | 3.306 | 3.451 | 58.461 | 0.44x |
| flat.json | pysimdjson | 17.348 | 17.569 | 17.871 | 58.461 | 0.08x |
| flat.json | json | 3.780 | 3.809 | 4.227 | 58.461 | 0.38x |
| nested.json | strata | 1.580 | 1.603 | 1.801 | 54.469 | 1.00x |
| nested.json | orjson | 1.814 | 1.863 | 1.940 | 54.469 | 0.86x |
| nested.json | msgspec | 1.985 | 2.057 | 2.185 | 54.469 | 0.78x |
| nested.json | ujson | 3.307 | 3.402 | 4.009 | 54.469 | 0.47x |
| nested.json | pysimdjson | 15.030 | 15.149 | 15.492 | 54.469 | 0.11x |
| nested.json | json | 4.337 | 4.524 | 4.580 | 54.469 | 0.35x |
| wide_arrays.json | strata | 7.343 | 7.802 | 8.215 | 60.254 | 1.00x |
| wide_arrays.json | orjson | 9.543 | 9.913 | 10.217 | 60.254 | 0.79x |
| wide_arrays.json | msgspec | 10.239 | 10.694 | 11.082 | 60.254 | 0.73x |
| wide_arrays.json | ujson | 12.925 | 13.498 | 14.499 | 60.254 | 0.58x |
| wide_arrays.json | pysimdjson | 81.802 | 86.142 | 87.602 | 60.254 | 0.09x |
| wide_arrays.json | json | 17.273 | 18.292 | 19.465 | 60.254 | 0.43x |
| mixed.json | strata | 0.343 | 0.350 | 0.372 | 57.598 | 1.00x |
| mixed.json | orjson | 0.421 | 0.431 | 0.439 | 57.598 | 0.81x |
| mixed.json | msgspec | 0.449 | 0.455 | 0.472 | 57.598 | 0.77x |
| mixed.json | ujson | 0.616 | 0.630 | 0.645 | 57.598 | 0.56x |
| mixed.json | pysimdjson | 3.228 | 3.248 | 3.347 | 57.598 | 0.11x |
| mixed.json | json | 0.875 | 0.885 | 0.960 | 57.598 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.895 | 2.959 | 3.145 | 51.355 | 1.00x |
| users.json | orjson | 3.869 | 3.952 | 4.276 | 51.355 | 0.75x |
| users.json | msgspec | 5.453 | 5.553 | 6.201 | 51.355 | 0.53x |
| users.json | ujson | 26.142 | 26.937 | 27.459 | 51.355 | 0.11x |
| users.json | json | 44.661 | 45.941 | 48.680 | 51.355 | 0.06x |
| flat.json | strata | 0.346 | 0.392 | 0.435 | 54.500 | 1.00x |
| flat.json | orjson | 0.425 | 0.475 | 0.787 | 54.500 | 0.83x |
| flat.json | msgspec | 0.578 | 0.618 | 0.702 | 54.500 | 0.63x |
| flat.json | ujson | 2.585 | 2.638 | 3.099 | 54.500 | 0.15x |
| flat.json | json | 4.293 | 4.368 | 4.391 | 54.500 | 0.09x |
| nested.json | strata | 0.294 | 0.304 | 0.430 | 54.613 | 1.00x |
| nested.json | orjson | 0.383 | 0.394 | 0.405 | 54.613 | 0.77x |
| nested.json | msgspec | 0.616 | 0.633 | 0.672 | 54.613 | 0.48x |
| nested.json | ujson | 2.596 | 2.617 | 2.683 | 54.613 | 0.12x |
| nested.json | json | 5.216 | 5.255 | 5.472 | 54.613 | 0.06x |
| wide_arrays.json | strata | 1.805 | 1.855 | 1.998 | 56.500 | 1.00x |
| wide_arrays.json | orjson | 2.318 | 2.457 | 2.988 | 56.500 | 0.76x |
| wide_arrays.json | msgspec | 3.275 | 3.449 | 3.587 | 56.500 | 0.54x |
| wide_arrays.json | ujson | 10.599 | 11.347 | 11.545 | 56.500 | 0.16x |
| wide_arrays.json | json | 35.704 | 38.912 | 39.333 | 56.500 | 0.05x |
| mixed.json | strata | 0.064 | 0.066 | 0.078 | 54.359 | 1.00x |
| mixed.json | orjson | 0.073 | 0.077 | 0.088 | 54.359 | 0.86x |
| mixed.json | msgspec | 0.104 | 0.106 | 0.117 | 54.359 | 0.62x |
| mixed.json | ujson | 0.456 | 0.459 | 0.471 | 54.359 | 0.14x |
| mixed.json | json | 0.960 | 0.962 | 1.059 | 54.359 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.700 | 20.477 | 23.019 | 61.383 | 1.00x |
| users.json | orjson | 27.477 | 29.709 | 34.890 | 61.383 | 0.69x |
| users.json | msgspec | 28.028 | 28.767 | 34.443 | 61.383 | 0.71x |
| users.json | ujson | 41.337 | 42.401 | 51.478 | 61.383 | 0.48x |
| users.json | json | 46.142 | 47.571 | 55.191 | 61.383 | 0.43x |
| flat.json | strata | 1.507 | 1.585 | 1.703 | 54.500 | 1.00x |
| flat.json | orjson | 1.687 | 1.772 | 1.834 | 54.500 | 0.89x |
| flat.json | msgspec | 1.956 | 2.027 | 2.108 | 54.500 | 0.78x |
| flat.json | ujson | 3.373 | 3.498 | 3.588 | 54.500 | 0.45x |
| flat.json | json | 3.861 | 3.940 | 4.041 | 54.500 | 0.40x |
| nested.json | strata | 1.736 | 1.798 | 2.444 | 54.613 | 1.00x |
| nested.json | orjson | 2.036 | 2.088 | 2.262 | 54.613 | 0.86x |
| nested.json | msgspec | 2.239 | 2.343 | 2.546 | 54.613 | 0.77x |
| nested.json | ujson | 3.605 | 3.753 | 4.112 | 54.613 | 0.48x |
| nested.json | json | 4.567 | 4.785 | 5.359 | 54.613 | 0.38x |
| wide_arrays.json | strata | 7.339 | 7.807 | 8.765 | 58.535 | 1.00x |
| wide_arrays.json | orjson | 9.167 | 9.633 | 11.161 | 58.535 | 0.81x |
| wide_arrays.json | msgspec | 10.223 | 10.693 | 12.293 | 58.535 | 0.73x |
| wide_arrays.json | ujson | 13.589 | 14.134 | 15.718 | 58.535 | 0.55x |
| wide_arrays.json | json | 17.273 | 18.396 | 20.899 | 58.535 | 0.42x |
| mixed.json | strata | 0.407 | 0.423 | 0.491 | 54.359 | 1.00x |
| mixed.json | orjson | 0.514 | 0.551 | 0.584 | 54.359 | 0.77x |
| mixed.json | msgspec | 0.533 | 0.602 | 0.692 | 54.359 | 0.70x |
| mixed.json | ujson | 0.712 | 0.774 | 0.828 | 54.359 | 0.55x |
| mixed.json | json | 0.944 | 1.028 | 1.064 | 54.359 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 21.539 | 22.522 | 31.382 | 58.352 | 1.00x |
| users.ndjson | orjson | 31.915 | 32.754 | 37.114 | 58.352 | 0.69x |
| users.ndjson | msgspec | 32.207 | 32.892 | 36.867 | 58.352 | 0.68x |
| users.ndjson | ujson | 46.728 | 50.540 | 132.033 | 58.352 | 0.45x |
| users.ndjson | json | 58.371 | 60.267 | 97.935 | 58.352 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.863 | 4.015 | 4.490 | 59.438 | 1.00x |
| users.json | orjson | 4.894 | 5.151 | 8.139 | 59.438 | 0.78x |
| users.json | msgspec | 6.501 | 6.741 | 7.607 | 59.438 | 0.60x |
| users.json | ujson | 27.870 | 29.121 | 31.571 | 59.438 | 0.14x |
| users.json | json | 46.822 | 48.062 | 50.395 | 59.438 | 0.08x |
| flat.json | strata | 0.770 | 0.819 | 0.924 | 54.500 | 1.00x |
| flat.json | orjson | 0.843 | 0.887 | 0.973 | 54.500 | 0.92x |
| flat.json | msgspec | 0.981 | 1.051 | 1.161 | 54.500 | 0.78x |
| flat.json | ujson | 3.068 | 3.089 | 3.302 | 54.500 | 0.27x |
| flat.json | json | 4.678 | 4.765 | 4.857 | 54.500 | 0.17x |
| nested.json | strata | 0.612 | 0.642 | 0.692 | 54.613 | 1.00x |
| nested.json | orjson | 0.710 | 0.744 | 0.859 | 54.613 | 0.86x |
| nested.json | msgspec | 0.938 | 1.014 | 1.527 | 54.613 | 0.63x |
| nested.json | ujson | 2.930 | 3.025 | 3.144 | 54.613 | 0.21x |
| nested.json | json | 5.546 | 5.619 | 5.805 | 54.613 | 0.11x |
| wide_arrays.json | strata | 2.256 | 2.341 | 2.646 | 58.535 | 1.00x |
| wide_arrays.json | orjson | 3.043 | 3.084 | 3.346 | 58.535 | 0.76x |
| wide_arrays.json | msgspec | 3.823 | 3.950 | 4.418 | 58.535 | 0.59x |
| wide_arrays.json | ujson | 11.085 | 11.237 | 12.802 | 58.535 | 0.21x |
| wide_arrays.json | json | 35.036 | 35.515 | 42.524 | 58.535 | 0.07x |
| mixed.json | strata | 0.292 | 0.339 | 0.412 | 54.359 | 1.00x |
| mixed.json | orjson | 0.298 | 0.356 | 0.412 | 54.359 | 0.95x |
| mixed.json | msgspec | 0.331 | 0.377 | 0.445 | 54.359 | 0.90x |
| mixed.json | ujson | 0.683 | 0.796 | 0.906 | 54.359 | 0.43x |
| mixed.json | json | 1.214 | 1.315 | 1.854 | 54.359 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.168 | 0.208 | 0.222 | 59.512 | 1.00x |
| users.json $[*].id | jmespath | 1.031 | 1.098 | 1.177 | 59.512 | 0.19x |
| users.json $[*].id | jsonpath-ng | 5.856 | 5.901 | 6.715 | 59.512 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.910 | 0.982 | 1.266 | 56.566 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.276 | 6.513 | 6.676 | 56.566 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 37.780 | 38.612 | 40.137 | 56.566 | 0.03x |
| users.json $..total | strata | 3.359 | 3.623 | 5.434 | 56.633 | 1.00x |
| users.json $..total | jsonpath-ng | 719.268 | 785.829 | 944.648 | 56.633 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.316 | 4.352 | 4.859 | 59.598 | 1.00x |
| users.json $[*].id | orjson+jmespath | 28.684 | 29.803 | 34.294 | 59.598 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 33.587 | 34.621 | 36.243 | 59.598 | 0.13x |
| users.json $[*].orders[*].total | strata | 4.532 | 4.574 | 4.815 | 56.566 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 34.314 | 36.006 | 39.447 | 56.566 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 70.960 | 74.073 | 78.472 | 56.566 | 0.06x |
| users.json $..total | strata | 23.645 | 24.066 | 27.925 | 56.672 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 785.358 | 810.676 | 904.859 | 56.672 | 0.03x |

