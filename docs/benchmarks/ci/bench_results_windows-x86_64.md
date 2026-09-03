# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a4f3b0e
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
| users.json | strata | 9.840 | 10.814 | 18.860 | 45.293 | 1.00x |
| users.json | orjson | 13.342 | 14.340 | 24.474 | 45.293 | 0.75x |
| users.json | msgspec | 13.099 | 13.972 | 16.068 | 45.293 | 0.77x |
| users.json | ujson | 22.830 | 24.025 | 36.792 | 45.293 | 0.45x |
| users.json | json | 22.496 | 23.780 | 29.543 | 45.293 | 0.45x |
| flat.json | strata | 1.124 | 1.235 | 1.336 | 51.129 | 1.00x |
| flat.json | orjson | 1.164 | 1.270 | 1.325 | 51.129 | 0.97x |
| flat.json | msgspec | 1.145 | 1.194 | 1.336 | 51.129 | 1.03x |
| flat.json | ujson | 2.458 | 2.664 | 2.865 | 51.129 | 0.46x |
| flat.json | json | 2.021 | 2.108 | 2.266 | 51.129 | 0.59x |
| nested.json | strata | 0.847 | 0.871 | 0.941 | 50.887 | 1.00x |
| nested.json | orjson | 1.077 | 1.154 | 1.250 | 50.887 | 0.76x |
| nested.json | msgspec | 1.026 | 1.115 | 1.399 | 50.887 | 0.78x |
| nested.json | ujson | 1.679 | 1.734 | 1.901 | 50.887 | 0.50x |
| nested.json | json | 2.220 | 2.267 | 2.341 | 50.887 | 0.38x |
| wide_arrays.json | strata | 4.809 | 5.045 | 8.324 | 52.852 | 1.00x |
| wide_arrays.json | orjson | 5.849 | 6.036 | 9.087 | 52.852 | 0.84x |
| wide_arrays.json | msgspec | 6.001 | 6.238 | 7.128 | 52.852 | 0.81x |
| wide_arrays.json | ujson | 9.765 | 10.020 | 10.626 | 52.852 | 0.50x |
| wide_arrays.json | json | 12.292 | 12.687 | 21.641 | 52.852 | 0.40x |
| mixed.json | strata | 0.195 | 0.201 | 0.247 | 50.789 | 1.00x |
| mixed.json | orjson | 0.214 | 0.226 | 0.243 | 50.789 | 0.89x |
| mixed.json | msgspec | 0.238 | 0.244 | 0.247 | 50.789 | 0.83x |
| mixed.json | ujson | 0.430 | 0.437 | 0.476 | 50.789 | 0.46x |
| mixed.json | json | 0.476 | 0.491 | 0.545 | 50.789 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.442 | 3.509 | 4.898 | 46.059 | 1.00x |
| users.json | orjson | 3.745 | 3.834 | 5.845 | 46.059 | 0.92x |
| users.json | msgspec | 5.521 | 5.614 | 8.792 | 46.059 | 0.62x |
| users.json | ujson | 16.210 | 16.447 | 27.885 | 46.059 | 0.21x |
| users.json | json | 23.868 | 24.122 | 27.078 | 46.059 | 0.15x |
| flat.json | strata | 0.343 | 0.371 | 0.413 | 51.258 | 1.00x |
| flat.json | orjson | 0.363 | 0.434 | 0.515 | 51.258 | 0.86x |
| flat.json | msgspec | 0.516 | 0.541 | 0.572 | 51.258 | 0.69x |
| flat.json | ujson | 1.931 | 2.023 | 2.116 | 51.258 | 0.18x |
| flat.json | json | 2.012 | 2.253 | 2.577 | 51.258 | 0.16x |
| nested.json | strata | 0.318 | 0.332 | 0.542 | 51.641 | 1.00x |
| nested.json | orjson | 0.330 | 0.334 | 0.526 | 51.641 | 1.00x |
| nested.json | msgspec | 0.473 | 0.595 | 0.932 | 51.641 | 0.56x |
| nested.json | ujson | 1.426 | 1.500 | 2.469 | 51.641 | 0.22x |
| nested.json | json | 2.708 | 2.769 | 4.660 | 51.641 | 0.12x |
| wide_arrays.json | strata | 2.409 | 2.549 | 3.713 | 52.477 | 1.00x |
| wide_arrays.json | orjson | 2.680 | 2.794 | 3.040 | 52.477 | 0.91x |
| wide_arrays.json | msgspec | 4.208 | 4.407 | 4.558 | 52.477 | 0.58x |
| wide_arrays.json | ujson | 11.302 | 11.657 | 11.886 | 52.477 | 0.22x |
| wide_arrays.json | json | 19.455 | 20.070 | 33.473 | 52.477 | 0.13x |
| mixed.json | strata | 0.085 | 0.088 | 0.145 | 50.922 | 1.00x |
| mixed.json | orjson | 0.069 | 0.070 | 0.108 | 50.922 | 1.26x |
| mixed.json | msgspec | 0.092 | 0.094 | 0.119 | 50.922 | 0.93x |
| mixed.json | ujson | 0.328 | 0.333 | 0.457 | 50.922 | 0.27x |
| mixed.json | json | 0.502 | 0.552 | 0.580 | 50.922 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.249 | 11.758 | 18.098 | 56.082 | 1.00x |
| users.json | orjson | 14.180 | 14.554 | 15.472 | 56.082 | 0.81x |
| users.json | msgspec | 14.303 | 14.768 | 17.868 | 56.082 | 0.80x |
| users.json | ujson | 27.189 | 29.203 | 38.357 | 56.082 | 0.40x |
| users.json | json | 23.677 | 24.494 | 39.518 | 56.082 | 0.48x |
| flat.json | strata | 1.384 | 1.493 | 2.213 | 50.656 | 1.00x |
| flat.json | orjson | 1.484 | 1.582 | 2.096 | 50.656 | 0.94x |
| flat.json | msgspec | 1.370 | 1.448 | 2.396 | 50.656 | 1.03x |
| flat.json | ujson | 3.000 | 3.191 | 4.639 | 50.656 | 0.47x |
| flat.json | json | 2.332 | 2.388 | 4.385 | 50.656 | 0.63x |
| nested.json | strata | 0.963 | 1.066 | 1.114 | 50.895 | 1.00x |
| nested.json | orjson | 1.245 | 1.344 | 2.365 | 50.895 | 0.79x |
| nested.json | msgspec | 1.144 | 1.310 | 2.739 | 50.895 | 0.81x |
| nested.json | ujson | 2.187 | 2.277 | 4.325 | 50.895 | 0.47x |
| nested.json | json | 2.331 | 2.518 | 2.829 | 50.895 | 0.42x |
| wide_arrays.json | strata | 5.455 | 5.596 | 10.052 | 52.477 | 1.00x |
| wide_arrays.json | orjson | 6.350 | 6.670 | 10.925 | 52.477 | 0.84x |
| wide_arrays.json | msgspec | 6.536 | 6.702 | 12.897 | 52.477 | 0.84x |
| wide_arrays.json | ujson | 12.949 | 13.136 | 18.821 | 52.477 | 0.43x |
| wide_arrays.json | json | 12.559 | 12.954 | 23.413 | 52.477 | 0.43x |
| mixed.json | strata | 0.278 | 0.293 | 0.351 | 50.852 | 1.00x |
| mixed.json | orjson | 0.327 | 0.383 | 0.502 | 50.852 | 0.77x |
| mixed.json | msgspec | 0.350 | 0.367 | 0.416 | 50.852 | 0.80x |
| mixed.json | ujson | 0.604 | 0.633 | 0.690 | 50.852 | 0.46x |
| mixed.json | json | 0.587 | 0.605 | 0.768 | 50.852 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 12.357 | 13.960 | 19.529 | 52.230 | 1.00x |
| users.ndjson | orjson | 17.890 | 19.262 | 23.344 | 52.230 | 0.72x |
| users.ndjson | msgspec | 18.606 | 20.180 | 23.951 | 52.230 | 0.69x |
| users.ndjson | ujson | 27.421 | 29.884 | 33.612 | 52.230 | 0.47x |
| users.ndjson | json | 32.781 | 34.759 | 55.774 | 52.230 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.361 | 4.451 | 6.628 | 53.164 | 1.00x |
| users.json | orjson | 4.436 | 4.564 | 7.400 | 53.164 | 0.98x |
| users.json | msgspec | 5.811 | 6.050 | 9.635 | 53.164 | 0.74x |
| users.json | ujson | 25.348 | 25.725 | 26.452 | 53.164 | 0.17x |
| users.json | json | 32.370 | 33.070 | 50.673 | 53.164 | 0.13x |
| flat.json | strata | 0.704 | 0.764 | 0.952 | 51.316 | 1.00x |
| flat.json | orjson | 0.722 | 0.813 | 0.950 | 51.316 | 0.94x |
| flat.json | msgspec | 0.932 | 1.068 | 1.173 | 51.316 | 0.72x |
| flat.json | ujson | 3.383 | 3.489 | 3.645 | 51.316 | 0.22x |
| flat.json | json | 3.736 | 3.853 | 4.127 | 51.316 | 0.20x |
| nested.json | strata | 0.730 | 0.777 | 0.916 | 51.625 | 1.00x |
| nested.json | orjson | 0.713 | 0.777 | 1.088 | 51.625 | 1.00x |
| nested.json | msgspec | 0.847 | 0.911 | 1.364 | 51.625 | 0.85x |
| nested.json | ujson | 2.653 | 2.736 | 4.459 | 51.625 | 0.28x |
| nested.json | json | 3.815 | 3.890 | 6.413 | 51.625 | 0.20x |
| wide_arrays.json | strata | 3.108 | 3.227 | 12.912 | 52.477 | 1.00x |
| wide_arrays.json | orjson | 3.235 | 3.409 | 5.000 | 52.477 | 0.95x |
| wide_arrays.json | msgspec | 4.736 | 4.899 | 7.688 | 52.477 | 0.66x |
| wide_arrays.json | ujson | 17.834 | 18.270 | 23.600 | 52.477 | 0.18x |
| wide_arrays.json | json | 25.426 | 25.949 | 35.684 | 52.477 | 0.12x |
| mixed.json | strata | 0.429 | 0.451 | 0.597 | 50.973 | 1.00x |
| mixed.json | orjson | 0.391 | 0.431 | 0.707 | 50.973 | 1.05x |
| mixed.json | msgspec | 0.412 | 0.458 | 0.557 | 50.973 | 0.98x |
| mixed.json | ujson | 0.826 | 0.885 | 1.300 | 50.973 | 0.51x |
| mixed.json | json | 1.021 | 1.078 | 1.930 | 50.973 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.089 | 0.099 | 0.144 | 53.176 | 1.00x |
| users.json $[*].id | jmespath | 0.500 | 0.517 | 0.860 | 53.176 | 0.19x |
| users.json $[*].id | jsonpath-ng | 2.679 | 2.782 | 5.094 | 53.176 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.799 | 0.827 | 0.913 | 53.293 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.034 | 5.514 | 5.985 | 53.293 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.153 | 33.308 | 81.892 | 53.293 | 0.02x |
| users.json $..total | strata | 1.876 | 2.186 | 5.788 | 54.293 | 1.00x |
| users.json $..total | jsonpath-ng | 390.587 | 467.728 | 634.513 | 54.293 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.094 | 4.220 | 9.203 | 53.180 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.911 | 15.667 | 25.039 | 53.180 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 16.848 | 19.420 | 29.588 | 53.180 | 0.22x |
| users.json $[*].orders[*].total | strata | 6.302 | 6.436 | 6.634 | 54.293 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 28.829 | 31.161 | 33.774 | 54.293 | 0.21x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 60.949 | 62.308 | 64.814 | 54.293 | 0.10x |
| users.json $..total | strata | 16.297 | 18.108 | 23.067 | 53.918 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 389.421 | 408.064 | 461.248 | 53.918 | 0.04x |

