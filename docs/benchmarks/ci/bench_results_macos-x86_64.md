# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e44489e
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
| users.json | strata | 21.312 | 21.968 | 34.766 | 52.625 | 1.00x |
| users.json | orjson | 26.862 | 29.708 | 35.673 | 52.625 | 0.74x |
| users.json | msgspec | 27.373 | 29.794 | 33.810 | 52.625 | 0.74x |
| users.json | ujson | 41.528 | 43.243 | 48.363 | 52.625 | 0.51x |
| users.json | pysimdjson | 176.847 | 184.565 | 196.388 | 52.625 | 0.12x |
| users.json | json | 46.946 | 48.232 | 67.523 | 52.625 | 0.46x |
| flat.json | strata | 1.317 | 1.395 | 1.615 | 58.777 | 1.00x |
| flat.json | orjson | 1.422 | 1.503 | 1.623 | 58.777 | 0.93x |
| flat.json | msgspec | 1.639 | 1.723 | 1.967 | 58.777 | 0.81x |
| flat.json | ujson | 2.905 | 3.058 | 3.300 | 58.777 | 0.46x |
| flat.json | pysimdjson | 15.580 | 16.111 | 16.664 | 58.777 | 0.09x |
| flat.json | json | 3.353 | 3.550 | 3.766 | 58.777 | 0.39x |
| nested.json | strata | 1.510 | 1.549 | 1.649 | 47.176 | 1.00x |
| nested.json | orjson | 1.692 | 1.778 | 1.845 | 47.176 | 0.87x |
| nested.json | msgspec | 1.861 | 1.920 | 1.992 | 47.176 | 0.81x |
| nested.json | ujson | 3.139 | 3.215 | 3.340 | 47.176 | 0.48x |
| nested.json | pysimdjson | 13.928 | 14.008 | 14.336 | 47.176 | 0.11x |
| nested.json | json | 4.094 | 4.122 | 4.280 | 47.176 | 0.38x |
| wide_arrays.json | strata | 8.658 | 9.026 | 9.595 | 58.473 | 1.00x |
| wide_arrays.json | orjson | 9.336 | 9.844 | 11.261 | 58.473 | 0.92x |
| wide_arrays.json | msgspec | 10.385 | 10.834 | 11.524 | 58.473 | 0.83x |
| wide_arrays.json | ujson | 13.452 | 13.785 | 15.502 | 58.473 | 0.65x |
| wide_arrays.json | pysimdjson | 81.836 | 83.155 | 88.199 | 58.473 | 0.11x |
| wide_arrays.json | json | 17.459 | 18.278 | 22.014 | 58.473 | 0.49x |
| mixed.json | strata | 0.399 | 0.425 | 0.484 | 57.230 | 1.00x |
| mixed.json | orjson | 0.468 | 0.494 | 0.577 | 57.230 | 0.86x |
| mixed.json | msgspec | 0.502 | 0.515 | 0.532 | 57.230 | 0.82x |
| mixed.json | ujson | 0.692 | 0.707 | 0.736 | 57.230 | 0.60x |
| mixed.json | pysimdjson | 3.519 | 3.576 | 3.846 | 57.230 | 0.12x |
| mixed.json | json | 0.978 | 1.021 | 1.068 | 57.230 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.688 | 2.816 | 3.796 | 48.332 | 1.00x |
| users.json | orjson | 3.568 | 3.813 | 5.823 | 48.332 | 0.74x |
| users.json | msgspec | 5.740 | 6.304 | 8.179 | 48.332 | 0.45x |
| users.json | ujson | 34.718 | 35.815 | 44.197 | 48.332 | 0.08x |
| users.json | json | 45.872 | 46.716 | 49.789 | 48.332 | 0.06x |
| flat.json | strata | 0.257 | 0.289 | 0.325 | 47.555 | 1.00x |
| flat.json | orjson | 0.363 | 0.408 | 0.615 | 47.555 | 0.71x |
| flat.json | msgspec | 0.503 | 0.551 | 0.582 | 47.555 | 0.52x |
| flat.json | ujson | 2.739 | 2.805 | 2.983 | 47.555 | 0.10x |
| flat.json | json | 3.742 | 3.878 | 4.143 | 47.555 | 0.07x |
| nested.json | strata | 0.228 | 0.249 | 0.312 | 46.914 | 1.00x |
| nested.json | orjson | 0.353 | 0.370 | 0.399 | 46.914 | 0.67x |
| nested.json | msgspec | 0.563 | 0.586 | 0.656 | 46.914 | 0.43x |
| nested.json | ujson | 2.929 | 2.942 | 3.320 | 46.914 | 0.08x |
| nested.json | json | 4.779 | 4.826 | 5.252 | 46.914 | 0.05x |
| wide_arrays.json | strata | 1.952 | 2.012 | 2.324 | 60.336 | 1.00x |
| wide_arrays.json | orjson | 2.425 | 2.457 | 2.613 | 60.336 | 0.82x |
| wide_arrays.json | msgspec | 3.203 | 3.249 | 4.229 | 60.336 | 0.62x |
| wide_arrays.json | ujson | 16.153 | 16.329 | 17.449 | 60.336 | 0.12x |
| wide_arrays.json | json | 35.538 | 35.887 | 38.647 | 60.336 | 0.06x |
| mixed.json | strata | 0.079 | 0.088 | 0.107 | 56.035 | 1.00x |
| mixed.json | orjson | 0.077 | 0.084 | 0.094 | 56.035 | 1.04x |
| mixed.json | msgspec | 0.110 | 0.117 | 0.177 | 56.035 | 0.75x |
| mixed.json | ujson | 0.682 | 0.685 | 0.718 | 56.035 | 0.13x |
| mixed.json | json | 1.053 | 1.076 | 1.196 | 56.035 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 20.705 | 21.572 | 23.476 | 60.633 | 1.00x |
| users.json | orjson | 26.871 | 28.082 | 30.385 | 60.633 | 0.77x |
| users.json | msgspec | 26.688 | 28.241 | 28.651 | 60.633 | 0.76x |
| users.json | ujson | 39.816 | 42.630 | 45.971 | 60.633 | 0.51x |
| users.json | json | 44.814 | 47.666 | 51.075 | 60.633 | 0.45x |
| flat.json | strata | 1.429 | 1.511 | 1.601 | 46.926 | 1.00x |
| flat.json | orjson | 1.562 | 1.634 | 1.677 | 46.926 | 0.92x |
| flat.json | msgspec | 1.770 | 1.883 | 2.235 | 46.926 | 0.80x |
| flat.json | ujson | 3.085 | 3.244 | 3.634 | 46.926 | 0.47x |
| flat.json | json | 3.460 | 3.691 | 3.724 | 46.926 | 0.41x |
| nested.json | strata | 1.743 | 1.780 | 1.834 | 46.914 | 1.00x |
| nested.json | orjson | 1.985 | 2.034 | 2.419 | 46.914 | 0.88x |
| nested.json | msgspec | 2.093 | 2.207 | 2.493 | 46.914 | 0.81x |
| nested.json | ujson | 3.500 | 3.609 | 4.015 | 46.914 | 0.49x |
| nested.json | json | 4.422 | 4.560 | 4.696 | 46.914 | 0.39x |
| wide_arrays.json | strata | 8.938 | 9.702 | 10.271 | 60.336 | 1.00x |
| wide_arrays.json | orjson | 9.715 | 10.647 | 13.923 | 60.336 | 0.91x |
| wide_arrays.json | msgspec | 10.838 | 11.562 | 13.879 | 60.336 | 0.84x |
| wide_arrays.json | ujson | 14.188 | 15.099 | 16.916 | 60.336 | 0.64x |
| wide_arrays.json | json | 18.393 | 19.153 | 21.286 | 60.336 | 0.51x |
| mixed.json | strata | 0.467 | 0.476 | 0.515 | 56.035 | 1.00x |
| mixed.json | orjson | 0.558 | 0.589 | 0.631 | 56.035 | 0.81x |
| mixed.json | msgspec | 0.592 | 0.628 | 0.677 | 56.035 | 0.76x |
| mixed.json | ujson | 0.809 | 0.819 | 0.873 | 56.035 | 0.58x |
| mixed.json | json | 1.068 | 1.111 | 1.198 | 56.035 | 0.43x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 21.188 | 21.792 | 22.304 | 58.457 | 1.00x |
| users.ndjson | orjson | 28.361 | 29.136 | 30.588 | 58.457 | 0.75x |
| users.ndjson | msgspec | 29.127 | 30.292 | 37.416 | 58.457 | 0.72x |
| users.ndjson | ujson | 41.564 | 43.812 | 46.004 | 58.457 | 0.50x |
| users.ndjson | json | 51.974 | 53.452 | 55.041 | 58.457 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.498 | 3.622 | 17.089 | 57.703 | 1.00x |
| users.json | orjson | 4.352 | 4.600 | 5.530 | 57.703 | 0.79x |
| users.json | msgspec | 6.165 | 6.530 | 6.930 | 57.703 | 0.55x |
| users.json | ujson | 34.203 | 35.826 | 40.823 | 57.703 | 0.10x |
| users.json | json | 44.404 | 48.191 | 128.917 | 57.703 | 0.08x |
| flat.json | strata | 0.542 | 0.644 | 0.745 | 46.953 | 1.00x |
| flat.json | orjson | 0.749 | 0.789 | 0.906 | 46.953 | 0.82x |
| flat.json | msgspec | 0.876 | 0.929 | 0.990 | 46.953 | 0.69x |
| flat.json | ujson | 3.136 | 3.202 | 3.322 | 46.953 | 0.20x |
| flat.json | json | 4.173 | 4.230 | 4.501 | 46.953 | 0.15x |
| nested.json | strata | 0.539 | 0.573 | 0.592 | 46.914 | 1.00x |
| nested.json | orjson | 0.698 | 0.725 | 0.788 | 46.914 | 0.79x |
| nested.json | msgspec | 0.906 | 0.978 | 1.036 | 46.914 | 0.59x |
| nested.json | ujson | 3.292 | 3.486 | 3.649 | 46.914 | 0.16x |
| nested.json | json | 5.245 | 5.608 | 6.040 | 46.914 | 0.10x |
| wide_arrays.json | strata | 2.723 | 2.853 | 3.321 | 60.336 | 1.00x |
| wide_arrays.json | orjson | 3.227 | 3.332 | 3.594 | 60.336 | 0.86x |
| wide_arrays.json | msgspec | 4.080 | 4.250 | 5.046 | 60.336 | 0.67x |
| wide_arrays.json | ujson | 17.989 | 18.177 | 18.636 | 60.336 | 0.16x |
| wide_arrays.json | json | 38.212 | 38.490 | 119.178 | 60.336 | 0.07x |
| mixed.json | strata | 0.339 | 0.383 | 0.498 | 56.035 | 1.00x |
| mixed.json | orjson | 0.355 | 0.409 | 0.480 | 56.035 | 0.94x |
| mixed.json | msgspec | 0.403 | 0.438 | 0.480 | 56.035 | 0.87x |
| mixed.json | ujson | 0.973 | 1.057 | 1.233 | 56.035 | 0.36x |
| mixed.json | json | 1.362 | 1.411 | 1.816 | 56.035 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.138 | 0.147 | 0.204 | 57.762 | 1.00x |
| users.json $[*].id | jmespath | 0.982 | 1.005 | 1.040 | 57.762 | 0.15x |
| users.json $[*].id | jsonpath-ng | 5.478 | 5.565 | 6.649 | 57.762 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.028 | 1.237 | 1.333 | 54.945 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.482 | 7.058 | 7.394 | 54.945 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 38.291 | 40.350 | 41.141 | 54.945 | 0.03x |
| users.json $..total | strata | 3.379 | 3.697 | 3.932 | 56.215 | 1.00x |
| users.json $..total | jsonpath-ng | 724.665 | 753.079 | 805.380 | 56.215 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.325 | 4.350 | 4.513 | 57.844 | 1.00x |
| users.json $[*].id | orjson+jmespath | 28.612 | 28.986 | 31.019 | 57.844 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 33.351 | 34.062 | 36.952 | 57.844 | 0.13x |
| users.json $[*].orders[*].total | strata | 4.574 | 4.618 | 5.387 | 56.168 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 34.180 | 36.662 | 38.302 | 56.168 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 71.724 | 76.684 | 81.753 | 56.168 | 0.06x |
| users.json $..total | strata | 25.080 | 25.752 | 28.239 | 56.242 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 775.703 | 810.802 | 837.179 | 56.242 | 0.03x |

