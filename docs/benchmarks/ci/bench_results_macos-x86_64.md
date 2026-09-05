# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 1cc7049
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
| users.json | strata | 16.912 | 17.099 | 19.946 | 52.570 | 1.00x |
| users.json | orjson | 23.822 | 24.683 | 27.531 | 52.570 | 0.69x |
| users.json | msgspec | 22.807 | 23.794 | 26.720 | 52.570 | 0.72x |
| users.json | ujson | 34.693 | 36.080 | 39.776 | 52.570 | 0.47x |
| users.json | pysimdjson | 152.237 | 152.865 | 156.384 | 52.570 | 0.11x |
| users.json | json | 39.798 | 40.352 | 42.519 | 52.570 | 0.42x |
| flat.json | strata | 1.177 | 1.221 | 1.261 | 56.184 | 1.00x |
| flat.json | orjson | 1.297 | 1.341 | 1.384 | 56.184 | 0.91x |
| flat.json | msgspec | 1.479 | 1.509 | 1.893 | 56.184 | 0.81x |
| flat.json | ujson | 2.570 | 2.651 | 3.045 | 56.184 | 0.46x |
| flat.json | pysimdjson | 13.789 | 13.932 | 14.882 | 56.184 | 0.09x |
| flat.json | json | 2.982 | 3.027 | 3.169 | 56.184 | 0.40x |
| nested.json | strata | 1.337 | 1.408 | 1.739 | 44.547 | 1.00x |
| nested.json | orjson | 1.530 | 1.582 | 1.635 | 44.547 | 0.89x |
| nested.json | msgspec | 1.619 | 1.762 | 1.850 | 44.547 | 0.80x |
| nested.json | ujson | 2.801 | 2.919 | 3.252 | 44.547 | 0.48x |
| nested.json | pysimdjson | 12.578 | 12.860 | 13.959 | 44.547 | 0.11x |
| nested.json | json | 3.564 | 3.746 | 3.863 | 44.547 | 0.38x |
| wide_arrays.json | strata | 7.217 | 7.359 | 7.815 | 56.008 | 1.00x |
| wide_arrays.json | orjson | 8.693 | 9.086 | 9.896 | 56.008 | 0.81x |
| wide_arrays.json | msgspec | 9.365 | 9.454 | 10.472 | 56.008 | 0.78x |
| wide_arrays.json | ujson | 11.364 | 12.046 | 12.991 | 56.008 | 0.61x |
| wide_arrays.json | pysimdjson | 74.455 | 75.132 | 76.611 | 56.008 | 0.10x |
| wide_arrays.json | json | 15.812 | 15.978 | 16.436 | 56.008 | 0.46x |
| mixed.json | strata | 0.345 | 0.358 | 0.364 | 51.266 | 1.00x |
| mixed.json | orjson | 0.420 | 0.428 | 0.440 | 51.266 | 0.84x |
| mixed.json | msgspec | 0.446 | 0.456 | 0.646 | 51.266 | 0.79x |
| mixed.json | ujson | 0.608 | 0.617 | 0.636 | 51.266 | 0.58x |
| mixed.json | pysimdjson | 3.083 | 3.183 | 3.454 | 51.266 | 0.11x |
| mixed.json | json | 0.846 | 0.856 | 0.904 | 51.266 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.246 | 2.297 | 2.537 | 48.012 | 1.00x |
| users.json | orjson | 3.042 | 3.161 | 3.486 | 48.012 | 0.73x |
| users.json | msgspec | 4.770 | 5.002 | 5.865 | 48.012 | 0.46x |
| users.json | ujson | 23.093 | 23.229 | 23.959 | 48.012 | 0.10x |
| users.json | json | 38.825 | 39.870 | 40.661 | 48.012 | 0.06x |
| flat.json | strata | 0.273 | 0.294 | 0.356 | 44.117 | 1.00x |
| flat.json | orjson | 0.355 | 0.419 | 0.560 | 44.117 | 0.70x |
| flat.json | msgspec | 0.474 | 0.489 | 0.548 | 44.117 | 0.60x |
| flat.json | ujson | 2.092 | 2.153 | 2.239 | 44.117 | 0.14x |
| flat.json | json | 3.400 | 3.452 | 4.134 | 44.117 | 0.09x |
| nested.json | strata | 0.223 | 0.225 | 0.238 | 44.410 | 1.00x |
| nested.json | orjson | 0.319 | 0.326 | 0.469 | 44.410 | 0.69x |
| nested.json | msgspec | 0.515 | 0.520 | 0.597 | 44.410 | 0.43x |
| nested.json | ujson | 2.177 | 2.185 | 2.216 | 44.410 | 0.10x |
| nested.json | json | 4.327 | 4.382 | 4.770 | 44.410 | 0.05x |
| wide_arrays.json | strata | 1.543 | 1.671 | 1.756 | 54.246 | 1.00x |
| wide_arrays.json | orjson | 2.055 | 2.190 | 2.380 | 54.246 | 0.76x |
| wide_arrays.json | msgspec | 2.981 | 3.098 | 3.355 | 54.246 | 0.54x |
| wide_arrays.json | ujson | 9.557 | 9.854 | 9.974 | 54.246 | 0.17x |
| wide_arrays.json | json | 31.989 | 32.287 | 32.770 | 54.246 | 0.05x |
| mixed.json | strata | 0.059 | 0.068 | 0.176 | 49.055 | 1.00x |
| mixed.json | orjson | 0.070 | 0.078 | 0.091 | 49.055 | 0.86x |
| mixed.json | msgspec | 0.099 | 0.105 | 0.134 | 49.055 | 0.64x |
| mixed.json | ujson | 0.429 | 0.437 | 0.574 | 49.055 | 0.15x |
| mixed.json | json | 0.899 | 0.912 | 1.049 | 49.055 | 0.07x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.656 | 17.778 | 18.782 | 60.469 | 1.00x |
| users.json | orjson | 24.244 | 25.088 | 27.805 | 60.469 | 0.71x |
| users.json | msgspec | 24.082 | 24.476 | 26.462 | 60.469 | 0.73x |
| users.json | ujson | 35.905 | 36.528 | 38.009 | 60.469 | 0.49x |
| users.json | json | 40.165 | 40.651 | 43.219 | 60.469 | 0.44x |
| flat.json | strata | 1.221 | 1.288 | 1.359 | 44.305 | 1.00x |
| flat.json | orjson | 1.415 | 1.479 | 1.538 | 44.305 | 0.87x |
| flat.json | msgspec | 1.502 | 1.668 | 1.810 | 44.305 | 0.77x |
| flat.json | ujson | 2.674 | 2.771 | 3.173 | 44.305 | 0.46x |
| flat.json | json | 3.093 | 3.121 | 3.787 | 44.305 | 0.41x |
| nested.json | strata | 1.448 | 1.464 | 1.536 | 44.410 | 1.00x |
| nested.json | orjson | 1.674 | 1.709 | 2.129 | 44.410 | 0.86x |
| nested.json | msgspec | 1.860 | 1.880 | 1.940 | 44.410 | 0.78x |
| nested.json | ujson | 2.996 | 3.022 | 3.475 | 44.410 | 0.48x |
| nested.json | json | 3.548 | 3.797 | 3.972 | 44.410 | 0.39x |
| wide_arrays.json | strata | 7.307 | 7.401 | 8.076 | 54.371 | 1.00x |
| wide_arrays.json | orjson | 8.307 | 8.870 | 10.073 | 54.371 | 0.83x |
| wide_arrays.json | msgspec | 9.561 | 9.671 | 10.498 | 54.371 | 0.77x |
| wide_arrays.json | ujson | 12.085 | 12.435 | 12.899 | 54.371 | 0.60x |
| wide_arrays.json | json | 16.014 | 16.128 | 16.958 | 54.371 | 0.46x |
| mixed.json | strata | 0.397 | 0.416 | 0.682 | 49.055 | 1.00x |
| mixed.json | orjson | 0.511 | 0.524 | 0.569 | 49.055 | 0.79x |
| mixed.json | msgspec | 0.531 | 0.551 | 0.616 | 49.055 | 0.75x |
| mixed.json | ujson | 0.695 | 0.717 | 1.018 | 49.055 | 0.58x |
| mixed.json | json | 0.920 | 0.950 | 1.057 | 49.055 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.586 | 17.832 | 18.756 | 56.656 | 1.00x |
| users.ndjson | orjson | 25.167 | 25.316 | 26.142 | 56.656 | 0.70x |
| users.ndjson | msgspec | 25.308 | 25.722 | 26.799 | 56.656 | 0.69x |
| users.ndjson | ujson | 36.340 | 36.823 | 37.654 | 56.656 | 0.48x |
| users.ndjson | json | 46.109 | 46.663 | 49.805 | 56.656 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.957 | 3.086 | 3.375 | 55.648 | 1.00x |
| users.json | orjson | 4.016 | 4.058 | 4.387 | 55.648 | 0.76x |
| users.json | msgspec | 5.725 | 5.919 | 6.183 | 55.648 | 0.52x |
| users.json | ujson | 24.217 | 24.648 | 24.839 | 55.648 | 0.13x |
| users.json | json | 40.348 | 40.975 | 85.758 | 55.648 | 0.08x |
| flat.json | strata | 0.570 | 0.601 | 0.656 | 44.305 | 1.00x |
| flat.json | orjson | 0.654 | 0.690 | 0.762 | 44.305 | 0.87x |
| flat.json | msgspec | 0.816 | 0.833 | 0.905 | 44.305 | 0.72x |
| flat.json | ujson | 2.391 | 2.482 | 2.670 | 44.305 | 0.24x |
| flat.json | json | 3.752 | 3.821 | 4.757 | 44.305 | 0.16x |
| nested.json | strata | 0.486 | 0.529 | 0.612 | 44.410 | 1.00x |
| nested.json | orjson | 0.585 | 0.636 | 0.705 | 44.410 | 0.83x |
| nested.json | msgspec | 0.789 | 0.841 | 0.906 | 44.410 | 0.63x |
| nested.json | ujson | 2.467 | 2.551 | 2.775 | 44.410 | 0.21x |
| nested.json | json | 4.685 | 4.721 | 5.232 | 44.410 | 0.11x |
| wide_arrays.json | strata | 2.136 | 2.292 | 36.991 | 54.371 | 1.00x |
| wide_arrays.json | orjson | 2.803 | 2.930 | 7.345 | 54.371 | 0.78x |
| wide_arrays.json | msgspec | 3.572 | 3.732 | 49.797 | 54.371 | 0.61x |
| wide_arrays.json | ujson | 10.246 | 10.603 | 77.047 | 54.371 | 0.22x |
| wide_arrays.json | json | 32.869 | 33.462 | 81.833 | 54.371 | 0.07x |
| mixed.json | strata | 0.280 | 0.352 | 0.520 | 49.055 | 1.00x |
| mixed.json | orjson | 0.286 | 0.350 | 0.485 | 49.055 | 1.00x |
| mixed.json | msgspec | 0.320 | 0.375 | 0.473 | 49.055 | 0.94x |
| mixed.json | ujson | 0.682 | 0.733 | 0.781 | 49.055 | 0.48x |
| mixed.json | json | 1.074 | 1.191 | 1.287 | 49.055 | 0.30x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.123 | 0.131 | 0.135 | 55.727 | 1.00x |
| users.json $[*].id | jmespath | 0.887 | 0.900 | 1.002 | 55.727 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.845 | 4.903 | 5.251 | 55.727 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.799 | 0.899 | 1.170 | 54.133 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.581 | 5.696 | 7.422 | 54.133 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.478 | 34.958 | 43.227 | 54.133 | 0.03x |
| users.json $..total | strata | 3.069 | 3.115 | 3.885 | 57.129 | 1.00x |
| users.json $..total | jsonpath-ng | 658.231 | 665.441 | 692.374 | 57.129 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.744 | 3.822 | 4.154 | 56.988 | 1.00x |
| users.json $[*].id | orjson+jmespath | 24.659 | 26.763 | 30.156 | 56.988 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 29.074 | 30.955 | 32.527 | 56.988 | 0.12x |
| users.json $[*].orders[*].total | strata | 3.955 | 4.024 | 4.509 | 56.316 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 29.611 | 31.137 | 34.128 | 56.316 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 61.834 | 63.965 | 68.344 | 56.316 | 0.06x |
| users.json $..total | strata | 20.910 | 21.382 | 32.635 | 56.406 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 685.067 | 691.378 | 726.975 | 56.406 | 0.03x |

