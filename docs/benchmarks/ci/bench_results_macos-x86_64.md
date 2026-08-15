# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: dc84475
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.7-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.267 | 19.916 | 25.358 | 52.691 | 1.00x |
| users.json | orjson | 23.266 | 25.336 | 31.329 | 52.691 | 0.79x |
| users.json | msgspec | 23.646 | 25.023 | 30.552 | 52.691 | 0.80x |
| users.json | ujson | 34.588 | 36.945 | 39.838 | 52.691 | 0.54x |
| users.json | pysimdjson | 151.690 | 155.511 | 166.286 | 52.691 | 0.13x |
| users.json | json | 39.490 | 40.179 | 44.609 | 52.691 | 0.50x |
| flat.json | strata | 1.312 | 1.437 | 1.511 | 57.727 | 1.00x |
| flat.json | orjson | 1.255 | 1.393 | 1.623 | 57.727 | 1.03x |
| flat.json | msgspec | 1.447 | 1.571 | 1.626 | 57.727 | 0.91x |
| flat.json | ujson | 2.572 | 2.748 | 2.828 | 57.727 | 0.52x |
| flat.json | pysimdjson | 13.777 | 14.805 | 15.235 | 57.727 | 0.10x |
| flat.json | json | 2.984 | 3.248 | 3.324 | 57.727 | 0.44x |
| nested.json | strata | 1.485 | 1.521 | 1.743 | 46.070 | 1.00x |
| nested.json | orjson | 1.529 | 1.561 | 1.716 | 46.070 | 0.97x |
| nested.json | msgspec | 1.678 | 1.707 | 1.786 | 46.070 | 0.89x |
| nested.json | ujson | 2.826 | 2.859 | 3.233 | 46.070 | 0.53x |
| nested.json | pysimdjson | 12.582 | 12.642 | 12.765 | 46.070 | 0.12x |
| nested.json | json | 3.590 | 3.620 | 3.766 | 46.070 | 0.42x |
| wide_arrays.json | strata | 7.552 | 7.755 | 8.307 | 57.512 | 1.00x |
| wide_arrays.json | orjson | 8.420 | 8.545 | 10.366 | 57.512 | 0.91x |
| wide_arrays.json | msgspec | 8.983 | 9.610 | 10.014 | 57.512 | 0.81x |
| wide_arrays.json | ujson | 12.021 | 12.159 | 18.783 | 57.512 | 0.64x |
| wide_arrays.json | pysimdjson | 73.991 | 74.736 | 79.276 | 57.512 | 0.10x |
| wide_arrays.json | json | 15.362 | 16.094 | 16.728 | 57.512 | 0.48x |
| mixed.json | strata | 0.366 | 0.372 | 0.395 | 54.559 | 1.00x |
| mixed.json | orjson | 0.394 | 0.407 | 0.444 | 54.559 | 0.91x |
| mixed.json | msgspec | 0.410 | 0.428 | 0.446 | 54.559 | 0.87x |
| mixed.json | ujson | 0.552 | 0.588 | 0.619 | 54.559 | 0.63x |
| mixed.json | pysimdjson | 3.012 | 3.074 | 3.919 | 54.559 | 0.12x |
| mixed.json | json | 0.821 | 0.834 | 1.095 | 54.559 | 0.45x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.837 | 3.970 | 4.252 | 48.328 | 1.00x |
| users.json | orjson | 2.826 | 3.165 | 3.741 | 48.328 | 1.25x |
| users.json | msgspec | 4.684 | 5.232 | 5.665 | 48.328 | 0.76x |
| users.json | ujson | 29.092 | 29.443 | 30.083 | 48.328 | 0.13x |
| users.json | json | 38.973 | 39.561 | 41.947 | 48.328 | 0.10x |
| flat.json | strata | 0.379 | 0.409 | 0.456 | 46.504 | 1.00x |
| flat.json | orjson | 0.318 | 0.382 | 0.495 | 46.504 | 1.07x |
| flat.json | msgspec | 0.478 | 0.489 | 0.749 | 46.504 | 0.84x |
| flat.json | ujson | 2.300 | 2.443 | 2.612 | 46.504 | 0.17x |
| flat.json | json | 3.320 | 3.409 | 4.048 | 46.504 | 0.12x |
| nested.json | strata | 0.343 | 0.362 | 0.376 | 45.953 | 1.00x |
| nested.json | orjson | 0.314 | 0.330 | 0.369 | 45.953 | 1.09x |
| nested.json | msgspec | 0.496 | 0.505 | 0.796 | 45.953 | 0.72x |
| nested.json | ujson | 2.584 | 2.591 | 2.650 | 45.953 | 0.14x |
| nested.json | json | 4.283 | 4.326 | 4.365 | 45.953 | 0.08x |
| wide_arrays.json | strata | 1.889 | 1.986 | 2.184 | 52.328 | 1.00x |
| wide_arrays.json | orjson | 2.065 | 2.105 | 2.308 | 52.328 | 0.94x |
| wide_arrays.json | msgspec | 2.828 | 2.939 | 3.341 | 52.328 | 0.68x |
| wide_arrays.json | ujson | 14.126 | 14.263 | 14.917 | 52.328 | 0.14x |
| wide_arrays.json | json | 31.857 | 32.048 | 33.258 | 52.328 | 0.06x |
| mixed.json | strata | 0.097 | 0.101 | 0.110 | 51.324 | 1.00x |
| mixed.json | orjson | 0.066 | 0.067 | 0.080 | 51.324 | 1.51x |
| mixed.json | msgspec | 0.097 | 0.102 | 0.132 | 51.324 | 0.98x |
| mixed.json | ujson | 0.561 | 0.563 | 0.573 | 51.324 | 0.18x |
| mixed.json | json | 0.887 | 0.891 | 0.921 | 51.324 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.726 | 20.396 | 21.568 | 60.637 | 1.00x |
| users.json | orjson | 23.837 | 24.722 | 27.110 | 60.637 | 0.83x |
| users.json | msgspec | 24.043 | 24.564 | 26.179 | 60.637 | 0.83x |
| users.json | ujson | 35.895 | 36.668 | 40.856 | 60.637 | 0.56x |
| users.json | json | 40.351 | 41.314 | 44.570 | 60.637 | 0.49x |
| flat.json | strata | 1.369 | 1.453 | 1.564 | 45.844 | 1.00x |
| flat.json | orjson | 1.365 | 1.424 | 1.512 | 45.844 | 1.02x |
| flat.json | msgspec | 1.601 | 1.624 | 1.712 | 45.844 | 0.89x |
| flat.json | ujson | 2.735 | 2.771 | 2.865 | 45.844 | 0.52x |
| flat.json | json | 3.084 | 3.184 | 3.480 | 45.844 | 0.46x |
| nested.json | strata | 1.577 | 1.612 | 1.746 | 45.953 | 1.00x |
| nested.json | orjson | 1.625 | 1.720 | 1.733 | 45.953 | 0.94x |
| nested.json | msgspec | 1.701 | 1.871 | 1.887 | 45.953 | 0.86x |
| nested.json | ujson | 2.991 | 3.059 | 3.600 | 45.953 | 0.53x |
| nested.json | json | 3.731 | 3.757 | 4.369 | 45.953 | 0.43x |
| wide_arrays.json | strata | 7.848 | 8.144 | 8.287 | 54.516 | 1.00x |
| wide_arrays.json | orjson | 8.558 | 8.967 | 9.172 | 54.516 | 0.91x |
| wide_arrays.json | msgspec | 9.644 | 10.148 | 11.205 | 54.516 | 0.80x |
| wide_arrays.json | ujson | 12.466 | 13.386 | 13.696 | 54.516 | 0.61x |
| wide_arrays.json | json | 16.477 | 17.296 | 17.898 | 54.516 | 0.47x |
| mixed.json | strata | 0.435 | 0.448 | 0.464 | 51.324 | 1.00x |
| mixed.json | orjson | 0.498 | 0.521 | 0.546 | 51.324 | 0.86x |
| mixed.json | msgspec | 0.536 | 0.554 | 0.600 | 51.324 | 0.81x |
| mixed.json | ujson | 0.701 | 0.721 | 0.771 | 51.324 | 0.62x |
| mixed.json | json | 0.914 | 0.934 | 0.979 | 51.324 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 19.883 | 20.422 | 22.937 | 57.434 | 1.00x |
| users.ndjson | orjson | 25.268 | 26.280 | 29.254 | 57.434 | 0.78x |
| users.ndjson | msgspec | 25.490 | 26.848 | 29.407 | 57.434 | 0.76x |
| users.ndjson | ujson | 36.595 | 38.542 | 42.558 | 57.434 | 0.53x |
| users.ndjson | json | 45.797 | 47.066 | 54.523 | 57.434 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.745 | 4.846 | 5.964 | 57.707 | 1.00x |
| users.json | orjson | 4.021 | 4.069 | 4.299 | 57.707 | 1.19x |
| users.json | msgspec | 5.488 | 5.614 | 6.176 | 57.707 | 0.86x |
| users.json | ujson | 29.883 | 30.227 | 31.394 | 57.707 | 0.16x |
| users.json | json | 40.261 | 41.234 | 43.290 | 57.707 | 0.12x |
| flat.json | strata | 0.712 | 0.751 | 0.810 | 45.844 | 1.00x |
| flat.json | orjson | 0.657 | 0.695 | 0.804 | 45.844 | 1.08x |
| flat.json | msgspec | 0.765 | 0.834 | 0.919 | 45.844 | 0.90x |
| flat.json | ujson | 2.759 | 2.821 | 2.914 | 45.844 | 0.27x |
| flat.json | json | 3.774 | 3.822 | 4.017 | 45.844 | 0.20x |
| nested.json | strata | 0.658 | 0.675 | 0.738 | 45.953 | 1.00x |
| nested.json | orjson | 0.618 | 0.673 | 0.751 | 45.953 | 1.00x |
| nested.json | msgspec | 0.804 | 0.854 | 0.959 | 45.953 | 0.79x |
| nested.json | ujson | 2.872 | 2.987 | 3.036 | 45.953 | 0.23x |
| nested.json | json | 4.662 | 4.735 | 5.297 | 45.953 | 0.14x |
| wide_arrays.json | strata | 2.468 | 2.543 | 3.228 | 54.516 | 1.00x |
| wide_arrays.json | orjson | 2.731 | 2.769 | 2.908 | 54.516 | 0.92x |
| wide_arrays.json | msgspec | 3.468 | 3.577 | 3.753 | 54.516 | 0.71x |
| wide_arrays.json | ujson | 14.841 | 15.214 | 15.804 | 54.516 | 0.17x |
| wide_arrays.json | json | 32.721 | 33.452 | 35.021 | 54.516 | 0.08x |
| mixed.json | strata | 0.344 | 0.365 | 0.384 | 51.324 | 1.00x |
| mixed.json | orjson | 0.341 | 0.369 | 0.418 | 51.324 | 0.99x |
| mixed.json | msgspec | 0.370 | 0.383 | 0.469 | 51.324 | 0.95x |
| mixed.json | ujson | 0.848 | 0.877 | 0.922 | 51.324 | 0.42x |
| mixed.json | json | 1.214 | 1.225 | 1.649 | 51.324 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.128 | 0.135 | 0.178 | 57.766 | 1.00x |
| users.json $[*].id | jmespath | 0.879 | 0.893 | 0.949 | 57.766 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.865 | 5.041 | 5.466 | 57.766 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.833 | 1.021 | 1.219 | 54.918 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.573 | 5.996 | 6.596 | 54.918 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.112 | 34.729 | 35.455 | 54.918 | 0.03x |
| users.json $..total | strata | 3.118 | 3.317 | 3.653 | 56.938 | 1.00x |
| users.json $..total | jsonpath-ng | 665.204 | 680.931 | 714.617 | 56.938 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.647 | 3.750 | 4.052 | 57.832 | 1.00x |
| users.json $[*].id | orjson+jmespath | 23.989 | 25.306 | 27.117 | 57.832 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 29.354 | 30.533 | 32.668 | 57.832 | 0.12x |
| users.json $[*].orders[*].total | strata | 3.928 | 3.953 | 4.006 | 56.156 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 29.218 | 30.056 | 33.927 | 56.156 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 61.469 | 64.528 | 69.074 | 56.156 | 0.06x |
| users.json $..total | strata | 23.119 | 23.727 | 25.085 | 56.176 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 684.826 | 705.641 | 726.981 | 56.176 | 0.03x |

