# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: f35a664
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
| users.json | strata | 11.371 | 12.703 | 21.152 | 45.449 | 1.00x |
| users.json | orjson | 14.232 | 15.573 | 17.701 | 45.449 | 0.82x |
| users.json | msgspec | 13.339 | 13.728 | 21.004 | 45.449 | 0.93x |
| users.json | ujson | 23.357 | 24.656 | 47.206 | 45.449 | 0.52x |
| users.json | json | 23.630 | 24.223 | 45.776 | 45.449 | 0.52x |
| flat.json | strata | 0.971 | 1.030 | 1.880 | 49.234 | 1.00x |
| flat.json | orjson | 1.262 | 1.902 | 2.192 | 49.234 | 0.54x |
| flat.json | msgspec | 1.097 | 1.155 | 2.255 | 49.234 | 0.89x |
| flat.json | ujson | 2.090 | 2.455 | 4.144 | 49.234 | 0.42x |
| flat.json | json | 2.001 | 2.741 | 4.188 | 49.234 | 0.38x |
| nested.json | strata | 0.878 | 0.931 | 3.759 | 48.891 | 1.00x |
| nested.json | orjson | 1.186 | 1.617 | 1.908 | 48.891 | 0.58x |
| nested.json | msgspec | 1.000 | 1.504 | 1.962 | 48.891 | 0.62x |
| nested.json | ujson | 1.607 | 1.929 | 2.698 | 48.891 | 0.48x |
| nested.json | json | 2.195 | 3.045 | 4.381 | 48.891 | 0.31x |
| wide_arrays.json | strata | 5.128 | 5.495 | 9.595 | 51.105 | 1.00x |
| wide_arrays.json | orjson | 5.950 | 6.243 | 9.795 | 51.105 | 0.88x |
| wide_arrays.json | msgspec | 6.017 | 6.455 | 9.707 | 51.105 | 0.85x |
| wide_arrays.json | ujson | 8.557 | 9.316 | 20.225 | 51.105 | 0.59x |
| wide_arrays.json | json | 12.005 | 13.531 | 22.326 | 51.105 | 0.41x |
| mixed.json | strata | 0.208 | 0.211 | 0.354 | 48.969 | 1.00x |
| mixed.json | orjson | 0.214 | 0.218 | 0.351 | 48.969 | 0.97x |
| mixed.json | msgspec | 0.236 | 0.240 | 0.414 | 48.969 | 0.88x |
| mixed.json | ujson | 0.372 | 0.399 | 0.843 | 48.969 | 0.53x |
| mixed.json | json | 0.473 | 0.496 | 0.871 | 48.969 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.461 | 3.588 | 5.023 | 44.871 | 1.00x |
| users.json | orjson | 3.762 | 3.868 | 6.187 | 44.871 | 0.93x |
| users.json | msgspec | 5.496 | 5.543 | 8.801 | 44.871 | 0.65x |
| users.json | ujson | 16.877 | 17.160 | 17.837 | 44.871 | 0.21x |
| users.json | json | 23.243 | 23.675 | 39.927 | 44.871 | 0.15x |
| flat.json | strata | 0.327 | 0.452 | 2.633 | 49.559 | 1.00x |
| flat.json | orjson | 0.359 | 0.637 | 0.658 | 49.559 | 0.71x |
| flat.json | msgspec | 0.505 | 0.616 | 0.996 | 49.559 | 0.73x |
| flat.json | ujson | 1.797 | 2.316 | 11.262 | 49.559 | 0.20x |
| flat.json | json | 1.932 | 2.562 | 7.640 | 49.559 | 0.18x |
| nested.json | strata | 0.277 | 0.308 | 0.507 | 49.410 | 1.00x |
| nested.json | orjson | 0.319 | 0.341 | 0.592 | 49.410 | 0.90x |
| nested.json | msgspec | 0.467 | 0.635 | 1.039 | 49.410 | 0.48x |
| nested.json | ujson | 1.333 | 2.222 | 7.547 | 49.410 | 0.14x |
| nested.json | json | 2.415 | 3.658 | 6.908 | 49.410 | 0.08x |
| wide_arrays.json | strata | 2.384 | 2.400 | 19.413 | 49.914 | 1.00x |
| wide_arrays.json | orjson | 2.415 | 2.454 | 3.778 | 49.914 | 0.98x |
| wide_arrays.json | msgspec | 3.472 | 3.515 | 4.714 | 49.914 | 0.68x |
| wide_arrays.json | ujson | 9.463 | 9.612 | 10.118 | 49.914 | 0.25x |
| wide_arrays.json | json | 17.878 | 18.063 | 30.301 | 49.914 | 0.13x |
| mixed.json | strata | 0.080 | 0.083 | 0.117 | 49.176 | 1.00x |
| mixed.json | orjson | 0.071 | 0.074 | 0.112 | 49.176 | 1.12x |
| mixed.json | msgspec | 0.094 | 0.098 | 0.146 | 49.176 | 0.85x |
| mixed.json | ujson | 0.334 | 0.340 | 0.561 | 49.176 | 0.24x |
| mixed.json | json | 0.516 | 0.524 | 0.950 | 49.176 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 13.858 | 14.156 | 18.056 | 54.230 | 1.00x |
| users.json | orjson | 14.770 | 15.049 | 15.894 | 54.230 | 0.94x |
| users.json | msgspec | 14.351 | 15.147 | 24.055 | 54.230 | 0.93x |
| users.json | ujson | 27.317 | 28.281 | 43.264 | 54.230 | 0.50x |
| users.json | json | 23.843 | 24.581 | 34.666 | 54.230 | 0.58x |
| flat.json | strata | 1.265 | 1.445 | 2.217 | 49.129 | 1.00x |
| flat.json | orjson | 1.297 | 1.439 | 2.222 | 49.129 | 1.00x |
| flat.json | msgspec | 1.318 | 1.482 | 2.442 | 49.129 | 0.97x |
| flat.json | ujson | 2.852 | 3.325 | 4.830 | 49.129 | 0.43x |
| flat.json | json | 2.145 | 3.174 | 3.768 | 49.129 | 0.46x |
| nested.json | strata | 1.015 | 1.105 | 1.816 | 49.145 | 1.00x |
| nested.json | orjson | 1.294 | 1.390 | 2.169 | 49.145 | 0.80x |
| nested.json | msgspec | 1.132 | 1.216 | 2.256 | 49.145 | 0.91x |
| nested.json | ujson | 2.031 | 2.390 | 3.567 | 49.145 | 0.46x |
| nested.json | json | 2.284 | 2.522 | 3.912 | 49.145 | 0.44x |
| wide_arrays.json | strata | 6.292 | 6.473 | 7.437 | 49.918 | 1.00x |
| wide_arrays.json | orjson | 6.058 | 6.321 | 6.906 | 49.918 | 1.02x |
| wide_arrays.json | msgspec | 6.324 | 6.618 | 7.141 | 49.918 | 0.98x |
| wide_arrays.json | ujson | 11.385 | 11.747 | 17.195 | 49.918 | 0.55x |
| wide_arrays.json | json | 12.196 | 12.527 | 22.205 | 49.918 | 0.52x |
| mixed.json | strata | 0.284 | 0.289 | 0.339 | 49.180 | 1.00x |
| mixed.json | orjson | 0.329 | 0.336 | 0.388 | 49.180 | 0.86x |
| mixed.json | msgspec | 0.347 | 0.357 | 0.568 | 49.180 | 0.81x |
| mixed.json | ujson | 0.552 | 0.566 | 0.609 | 49.180 | 0.51x |
| mixed.json | json | 0.593 | 0.612 | 0.940 | 49.180 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 14.805 | 17.314 | 31.484 | 49.820 | 1.00x |
| users.ndjson | orjson | 20.803 | 21.713 | 24.705 | 49.820 | 0.80x |
| users.ndjson | msgspec | 19.797 | 22.360 | 38.632 | 49.820 | 0.77x |
| users.ndjson | ujson | 30.116 | 32.817 | 60.307 | 49.820 | 0.53x |
| users.ndjson | json | 35.721 | 38.342 | 49.350 | 49.820 | 0.45x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.451 | 4.521 | 6.612 | 52.098 | 1.00x |
| users.json | orjson | 4.746 | 4.822 | 7.151 | 52.098 | 0.94x |
| users.json | msgspec | 6.378 | 6.519 | 6.944 | 52.098 | 0.69x |
| users.json | ujson | 26.384 | 26.634 | 42.872 | 52.098 | 0.17x |
| users.json | json | 32.891 | 33.131 | 53.382 | 52.098 | 0.14x |
| flat.json | strata | 0.707 | 0.863 | 1.197 | 49.262 | 1.00x |
| flat.json | orjson | 0.843 | 1.225 | 1.378 | 49.262 | 0.70x |
| flat.json | msgspec | 0.947 | 1.198 | 6.813 | 49.262 | 0.72x |
| flat.json | ujson | 3.538 | 4.978 | 8.942 | 49.262 | 0.17x |
| flat.json | json | 3.656 | 4.780 | 11.611 | 49.262 | 0.18x |
| nested.json | strata | 0.624 | 0.687 | 1.071 | 49.441 | 1.00x |
| nested.json | orjson | 0.680 | 0.886 | 1.142 | 49.441 | 0.78x |
| nested.json | msgspec | 0.827 | 0.887 | 7.924 | 49.441 | 0.77x |
| nested.json | ujson | 2.395 | 3.404 | 8.769 | 49.441 | 0.20x |
| nested.json | json | 3.521 | 5.764 | 11.661 | 49.441 | 0.12x |
| wide_arrays.json | strata | 3.145 | 3.306 | 3.385 | 49.914 | 1.00x |
| wide_arrays.json | orjson | 3.099 | 3.303 | 4.834 | 49.914 | 1.00x |
| wide_arrays.json | msgspec | 4.152 | 4.333 | 6.111 | 49.914 | 0.76x |
| wide_arrays.json | ujson | 16.362 | 16.855 | 27.575 | 49.914 | 0.20x |
| wide_arrays.json | json | 24.869 | 25.442 | 25.830 | 49.914 | 0.13x |
| mixed.json | strata | 0.411 | 0.455 | 0.521 | 49.180 | 1.00x |
| mixed.json | orjson | 0.408 | 0.440 | 0.552 | 49.180 | 1.03x |
| mixed.json | msgspec | 0.431 | 0.448 | 0.488 | 49.180 | 1.02x |
| mixed.json | ujson | 0.831 | 0.857 | 0.963 | 49.180 | 0.53x |
| mixed.json | json | 1.026 | 1.075 | 1.243 | 49.180 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.098 | 0.101 | 0.106 | 52.117 | 1.00x |
| users.json $[*].id | jmespath | 0.460 | 0.497 | 0.571 | 52.117 | 0.20x |
| users.json $[*].id | jsonpath-ng | 2.548 | 2.885 | 3.454 | 52.117 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.485 | 0.496 | 0.609 | 51.707 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.825 | 2.931 | 4.371 | 51.707 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.714 | 18.844 | 33.252 | 51.707 | 0.03x |
| users.json $..total | strata | 1.807 | 1.869 | 1.950 | 52.793 | 1.00x |
| users.json $..total | jsonpath-ng | 333.342 | 356.742 | 373.883 | 52.793 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 5.567 | 5.643 | 8.834 | 51.488 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.273 | 15.536 | 20.017 | 51.488 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 17.344 | 18.006 | 31.533 | 51.488 | 0.31x |
| users.json $[*].orders[*].total | strata | 5.556 | 5.627 | 8.541 | 52.715 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.408 | 18.901 | 25.976 | 52.715 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.584 | 38.828 | 55.685 | 52.715 | 0.14x |
| users.json $..total | strata | 19.633 | 28.074 | 64.821 | 52.719 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 400.395 | 607.496 | 759.041 | 52.719 | 0.05x |

