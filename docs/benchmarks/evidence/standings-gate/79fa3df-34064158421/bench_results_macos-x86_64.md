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
| users.json | strata | 16.902 | 18.281 | 20.305 | 52.641 | 1.00x |
| users.json | orjson | 23.400 | 25.591 | 26.259 | 52.641 | 0.71x |
| users.json | msgspec | 23.211 | 25.533 | 26.869 | 52.641 | 0.72x |
| users.json | ujson | 34.968 | 37.679 | 41.170 | 52.641 | 0.49x |
| users.json | pysimdjson | 152.992 | 164.444 | 166.848 | 52.641 | 0.11x |
| users.json | json | 40.888 | 42.818 | 45.188 | 52.641 | 0.43x |
| flat.json | strata | 1.179 | 1.217 | 1.645 | 58.539 | 1.00x |
| flat.json | orjson | 1.308 | 1.372 | 1.776 | 58.539 | 0.89x |
| flat.json | msgspec | 1.526 | 1.541 | 1.696 | 58.539 | 0.79x |
| flat.json | ujson | 2.640 | 2.729 | 3.242 | 58.539 | 0.45x |
| flat.json | pysimdjson | 13.974 | 14.775 | 16.300 | 58.539 | 0.08x |
| flat.json | json | 3.006 | 3.112 | 3.266 | 58.539 | 0.39x |
| nested.json | strata | 1.335 | 1.347 | 1.443 | 55.801 | 1.00x |
| nested.json | orjson | 1.539 | 1.552 | 1.619 | 55.801 | 0.87x |
| nested.json | msgspec | 1.693 | 1.719 | 1.863 | 55.801 | 0.78x |
| nested.json | ujson | 2.807 | 2.821 | 2.874 | 55.801 | 0.48x |
| nested.json | pysimdjson | 12.607 | 12.698 | 13.031 | 55.801 | 0.11x |
| nested.json | json | 3.608 | 3.683 | 4.005 | 55.801 | 0.37x |
| wide_arrays.json | strata | 7.271 | 7.713 | 39.348 | 60.090 | 1.00x |
| wide_arrays.json | orjson | 9.092 | 10.009 | 13.258 | 60.090 | 0.77x |
| wide_arrays.json | msgspec | 9.447 | 10.448 | 12.462 | 60.090 | 0.74x |
| wide_arrays.json | ujson | 12.540 | 13.541 | 16.100 | 60.090 | 0.57x |
| wide_arrays.json | pysimdjson | 78.742 | 84.754 | 90.988 | 60.090 | 0.09x |
| wide_arrays.json | json | 16.129 | 17.390 | 20.056 | 60.090 | 0.44x |
| mixed.json | strata | 0.336 | 0.360 | 0.568 | 57.098 | 1.00x |
| mixed.json | orjson | 0.407 | 0.447 | 0.763 | 57.098 | 0.81x |
| mixed.json | msgspec | 0.440 | 0.448 | 0.493 | 57.098 | 0.80x |
| mixed.json | ujson | 0.604 | 0.643 | 0.739 | 57.098 | 0.56x |
| mixed.json | pysimdjson | 3.137 | 3.250 | 3.859 | 57.098 | 0.11x |
| mixed.json | json | 0.845 | 0.899 | 0.946 | 57.098 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.310 | 2.488 | 2.904 | 48.500 | 1.00x |
| users.json | orjson | 3.072 | 3.224 | 3.491 | 48.500 | 0.77x |
| users.json | msgspec | 5.475 | 5.581 | 6.282 | 48.500 | 0.45x |
| users.json | ujson | 23.008 | 24.010 | 24.379 | 48.500 | 0.10x |
| users.json | json | 39.825 | 42.453 | 45.010 | 48.500 | 0.06x |
| flat.json | strata | 0.314 | 0.330 | 0.344 | 55.664 | 1.00x |
| flat.json | orjson | 0.384 | 0.394 | 0.419 | 55.664 | 0.84x |
| flat.json | msgspec | 0.512 | 0.520 | 0.531 | 55.664 | 0.64x |
| flat.json | ujson | 2.112 | 2.162 | 2.322 | 55.664 | 0.15x |
| flat.json | json | 3.478 | 3.554 | 4.004 | 55.664 | 0.09x |
| nested.json | strata | 0.243 | 0.248 | 0.274 | 50.703 | 1.00x |
| nested.json | orjson | 0.321 | 0.324 | 0.343 | 50.703 | 0.76x |
| nested.json | msgspec | 0.516 | 0.524 | 0.535 | 50.703 | 0.47x |
| nested.json | ujson | 2.162 | 2.172 | 2.453 | 50.703 | 0.11x |
| nested.json | json | 4.319 | 4.346 | 4.391 | 50.703 | 0.06x |
| wide_arrays.json | strata | 1.576 | 1.664 | 2.162 | 58.609 | 1.00x |
| wide_arrays.json | orjson | 2.084 | 2.177 | 2.390 | 58.609 | 0.76x |
| wide_arrays.json | msgspec | 2.936 | 3.004 | 3.345 | 58.609 | 0.55x |
| wide_arrays.json | ujson | 9.567 | 9.803 | 11.200 | 58.609 | 0.17x |
| wide_arrays.json | json | 31.987 | 33.324 | 37.488 | 58.609 | 0.05x |
| mixed.json | strata | 0.068 | 0.086 | 0.140 | 53.777 | 1.00x |
| mixed.json | orjson | 0.073 | 0.078 | 0.093 | 53.777 | 1.10x |
| mixed.json | msgspec | 0.102 | 0.113 | 0.135 | 53.777 | 0.76x |
| mixed.json | ujson | 0.453 | 0.469 | 0.650 | 53.777 | 0.18x |
| mixed.json | json | 0.921 | 0.976 | 1.302 | 53.777 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 17.409 | 18.495 | 18.987 | 61.039 | 1.00x |
| users.json | orjson | 24.315 | 25.470 | 27.017 | 61.039 | 0.73x |
| users.json | msgspec | 24.690 | 25.946 | 27.111 | 61.039 | 0.71x |
| users.json | ujson | 37.404 | 39.502 | 41.181 | 61.039 | 0.47x |
| users.json | json | 41.080 | 43.381 | 43.987 | 61.039 | 0.43x |
| flat.json | strata | 1.244 | 1.291 | 1.478 | 55.664 | 1.00x |
| flat.json | orjson | 1.391 | 1.486 | 1.958 | 55.664 | 0.87x |
| flat.json | msgspec | 1.621 | 1.715 | 2.100 | 55.664 | 0.75x |
| flat.json | ujson | 2.725 | 2.900 | 3.338 | 55.664 | 0.45x |
| flat.json | json | 3.170 | 3.263 | 3.488 | 55.664 | 0.40x |
| nested.json | strata | 1.448 | 1.476 | 1.531 | 50.703 | 1.00x |
| nested.json | orjson | 1.654 | 1.714 | 1.751 | 50.703 | 0.86x |
| nested.json | msgspec | 1.838 | 1.874 | 1.899 | 50.703 | 0.79x |
| nested.json | ujson | 2.977 | 3.052 | 3.771 | 50.703 | 0.48x |
| nested.json | json | 3.742 | 3.865 | 4.142 | 50.703 | 0.38x |
| wide_arrays.json | strata | 7.303 | 7.570 | 7.932 | 58.609 | 1.00x |
| wide_arrays.json | orjson | 8.855 | 9.476 | 9.913 | 58.609 | 0.80x |
| wide_arrays.json | msgspec | 9.750 | 10.606 | 10.999 | 58.609 | 0.71x |
| wide_arrays.json | ujson | 13.108 | 13.687 | 15.126 | 58.609 | 0.55x |
| wide_arrays.json | json | 16.764 | 17.883 | 18.593 | 58.609 | 0.42x |
| mixed.json | strata | 0.423 | 0.441 | 0.547 | 53.777 | 1.00x |
| mixed.json | orjson | 0.524 | 0.566 | 0.609 | 53.777 | 0.78x |
| mixed.json | msgspec | 0.554 | 0.600 | 0.782 | 53.777 | 0.73x |
| mixed.json | ujson | 0.743 | 0.770 | 1.153 | 53.777 | 0.57x |
| mixed.json | json | 0.979 | 0.996 | 1.254 | 53.777 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 17.475 | 18.147 | 19.080 | 58.754 | 1.00x |
| users.ndjson | orjson | 25.234 | 25.942 | 28.349 | 58.754 | 0.70x |
| users.ndjson | msgspec | 25.419 | 26.191 | 27.825 | 58.754 | 0.69x |
| users.ndjson | ujson | 36.975 | 37.940 | 41.546 | 58.754 | 0.48x |
| users.ndjson | json | 46.470 | 47.875 | 51.166 | 58.754 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.156 | 3.773 | 4.299 | 55.469 | 1.00x |
| users.json | orjson | 3.943 | 4.395 | 4.848 | 55.469 | 0.86x |
| users.json | msgspec | 6.261 | 6.583 | 8.265 | 55.469 | 0.57x |
| users.json | ujson | 24.152 | 25.198 | 28.226 | 55.469 | 0.15x |
| users.json | json | 40.960 | 44.479 | 65.611 | 55.469 | 0.08x |
| flat.json | strata | 0.630 | 0.689 | 0.778 | 55.664 | 1.00x |
| flat.json | orjson | 0.683 | 0.770 | 0.851 | 55.664 | 0.90x |
| flat.json | msgspec | 0.831 | 0.887 | 0.948 | 55.664 | 0.78x |
| flat.json | ujson | 2.440 | 2.475 | 2.661 | 55.664 | 0.28x |
| flat.json | json | 3.751 | 3.849 | 4.148 | 55.664 | 0.18x |
| nested.json | strata | 0.505 | 0.569 | 0.652 | 50.703 | 1.00x |
| nested.json | orjson | 0.620 | 0.646 | 0.972 | 50.703 | 0.88x |
| nested.json | msgspec | 0.803 | 0.822 | 0.918 | 50.703 | 0.69x |
| nested.json | ujson | 2.484 | 2.540 | 2.640 | 50.703 | 0.22x |
| nested.json | json | 4.681 | 4.741 | 5.296 | 50.703 | 0.12x |
| wide_arrays.json | strata | 2.239 | 2.391 | 45.079 | 58.609 | 1.00x |
| wide_arrays.json | orjson | 3.007 | 3.247 | 45.748 | 58.609 | 0.74x |
| wide_arrays.json | msgspec | 3.652 | 3.948 | 46.801 | 58.609 | 0.61x |
| wide_arrays.json | ujson | 10.661 | 11.181 | 12.519 | 58.609 | 0.21x |
| wide_arrays.json | json | 33.729 | 35.777 | 78.468 | 58.609 | 0.07x |
| mixed.json | strata | 0.321 | 0.432 | 0.586 | 53.777 | 1.00x |
| mixed.json | orjson | 0.395 | 0.470 | 0.647 | 53.777 | 0.92x |
| mixed.json | msgspec | 0.410 | 0.482 | 0.663 | 53.777 | 0.90x |
| mixed.json | ujson | 0.781 | 0.828 | 1.177 | 53.777 | 0.52x |
| mixed.json | json | 1.270 | 1.359 | 2.368 | 53.777 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.129 | 0.134 | 0.275 | 55.570 | 1.00x |
| users.json $[*].id | jmespath | 0.882 | 0.896 | 0.911 | 55.570 | 0.15x |
| users.json $[*].id | jsonpath-ng | 4.845 | 4.927 | 5.045 | 55.570 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.761 | 0.890 | 1.096 | 55.477 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.482 | 5.597 | 6.247 | 55.477 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 33.000 | 34.332 | 35.209 | 55.477 | 0.03x |
| users.json $..total | strata | 3.099 | 3.154 | 3.632 | 57.492 | 1.00x |
| users.json $..total | jsonpath-ng | 668.885 | 680.645 | 716.615 | 57.492 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.541 | 3.638 | 3.968 | 59.598 | 1.00x |
| users.json $[*].id | orjson+jmespath | 25.684 | 26.393 | 26.595 | 59.598 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 29.187 | 30.731 | 31.192 | 59.598 | 0.12x |
| users.json $[*].orders[*].total | strata | 3.938 | 4.066 | 4.432 | 56.707 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 30.229 | 31.286 | 34.273 | 56.707 | 0.13x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 63.414 | 65.528 | 67.713 | 56.707 | 0.06x |
| users.json $..total | strata | 20.995 | 22.440 | 117.476 | 56.730 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 711.768 | 797.240 | 930.136 | 56.730 | 0.03x |

