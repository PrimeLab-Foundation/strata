# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec53f936b8a9245edf5bfeec36c38539666a1775
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: /nologo /O2 /W3 /DNDEBUG /MD /EHsc /std:c++20 /Zc:__cplusplus /arch:AVX2 /clang:-O3 /clang:-fprofile-use=D:\a\strata\strata\build\pgo\strata.profdata /DLL /MANIFEST:EMBED,ID=2 /MANIFESTUAC:NO /EXPORT:PyInit__strata (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.534 | 10.303 | 15.719 | 48.871 | 1.00x |
| users.json | orjson | 13.433 | 14.584 | 18.978 | 48.871 | 0.71x |
| users.json | msgspec | 12.834 | 15.094 | 21.821 | 48.871 | 0.68x |
| users.json | ujson | 21.902 | 24.149 | 30.255 | 48.871 | 0.43x |
| users.json | json | 22.849 | 23.649 | 33.037 | 48.871 | 0.44x |
| flat.json | strata | 0.994 | 1.025 | 1.071 | 56.883 | 1.00x |
| flat.json | orjson | 1.168 | 1.190 | 1.365 | 56.883 | 0.86x |
| flat.json | msgspec | 1.110 | 1.134 | 1.155 | 56.883 | 0.90x |
| flat.json | ujson | 2.165 | 2.218 | 2.306 | 56.883 | 0.46x |
| flat.json | json | 1.975 | 1.981 | 1.999 | 56.883 | 0.52x |
| nested.json | strata | 0.768 | 0.775 | 0.813 | 56.727 | 1.00x |
| nested.json | orjson | 1.108 | 1.145 | 1.194 | 56.727 | 0.68x |
| nested.json | msgspec | 0.983 | 1.020 | 1.060 | 56.727 | 0.76x |
| nested.json | ujson | 1.548 | 1.586 | 1.778 | 56.727 | 0.49x |
| nested.json | json | 2.114 | 2.127 | 2.168 | 56.727 | 0.36x |
| wide_arrays.json | strata | 4.258 | 4.352 | 4.712 | 58.699 | 1.00x |
| wide_arrays.json | orjson | 5.795 | 6.159 | 6.632 | 58.699 | 0.71x |
| wide_arrays.json | msgspec | 5.773 | 6.077 | 8.883 | 58.699 | 0.72x |
| wide_arrays.json | ujson | 8.405 | 8.587 | 11.948 | 58.699 | 0.51x |
| wide_arrays.json | json | 12.110 | 12.428 | 13.271 | 58.699 | 0.35x |
| mixed.json | strata | 0.185 | 0.192 | 0.227 | 57.566 | 1.00x |
| mixed.json | orjson | 0.212 | 0.221 | 0.269 | 57.566 | 0.87x |
| mixed.json | msgspec | 0.235 | 0.242 | 0.265 | 57.566 | 0.79x |
| mixed.json | ujson | 0.353 | 0.386 | 0.485 | 57.566 | 0.50x |
| mixed.json | json | 0.467 | 0.478 | 0.546 | 57.566 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.190 | 3.428 | 5.038 | 48.105 | 1.00x |
| users.json | orjson | 3.587 | 4.368 | 5.630 | 48.105 | 0.78x |
| users.json | msgspec | 5.153 | 6.004 | 13.277 | 48.105 | 0.57x |
| users.json | ujson | 14.355 | 16.671 | 28.803 | 48.105 | 0.21x |
| users.json | json | 23.454 | 28.990 | 42.884 | 48.105 | 0.12x |
| flat.json | strata | 0.310 | 0.317 | 0.341 | 57.621 | 1.00x |
| flat.json | orjson | 0.358 | 0.363 | 0.411 | 57.621 | 0.87x |
| flat.json | msgspec | 0.502 | 0.507 | 0.543 | 57.621 | 0.62x |
| flat.json | ujson | 1.469 | 1.555 | 1.591 | 57.621 | 0.20x |
| flat.json | json | 1.898 | 1.927 | 1.968 | 57.621 | 0.16x |
| nested.json | strata | 0.304 | 0.312 | 0.362 | 57.172 | 1.00x |
| nested.json | orjson | 0.325 | 0.332 | 0.421 | 57.172 | 0.94x |
| nested.json | msgspec | 0.474 | 0.480 | 0.663 | 57.172 | 0.65x |
| nested.json | ujson | 1.150 | 1.218 | 1.487 | 57.172 | 0.26x |
| nested.json | json | 2.591 | 2.646 | 3.061 | 57.172 | 0.12x |
| wide_arrays.json | strata | 2.015 | 2.063 | 2.222 | 58.695 | 1.00x |
| wide_arrays.json | orjson | 2.532 | 2.563 | 2.932 | 58.695 | 0.80x |
| wide_arrays.json | msgspec | 4.097 | 4.155 | 4.323 | 58.695 | 0.50x |
| wide_arrays.json | ujson | 7.728 | 7.957 | 12.425 | 58.695 | 0.26x |
| wide_arrays.json | json | 18.555 | 18.913 | 21.177 | 58.695 | 0.11x |
| mixed.json | strata | 0.074 | 0.076 | 0.106 | 57.668 | 1.00x |
| mixed.json | orjson | 0.069 | 0.071 | 0.111 | 57.668 | 1.06x |
| mixed.json | msgspec | 0.094 | 0.096 | 0.131 | 57.668 | 0.79x |
| mixed.json | ujson | 0.264 | 0.269 | 0.308 | 57.668 | 0.28x |
| mixed.json | json | 0.514 | 0.519 | 0.558 | 57.668 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.565 | 11.795 | 19.993 | 58.133 | 1.00x |
| users.json | orjson | 14.187 | 15.895 | 25.389 | 58.133 | 0.74x |
| users.json | msgspec | 13.830 | 15.592 | 24.830 | 58.133 | 0.76x |
| users.json | ujson | 27.647 | 30.181 | 42.606 | 58.133 | 0.39x |
| users.json | json | 24.026 | 24.563 | 31.001 | 58.133 | 0.48x |
| flat.json | strata | 1.103 | 1.178 | 1.252 | 56.914 | 1.00x |
| flat.json | orjson | 1.283 | 1.383 | 1.452 | 56.914 | 0.85x |
| flat.json | msgspec | 1.328 | 1.376 | 2.138 | 56.914 | 0.86x |
| flat.json | ujson | 2.742 | 2.829 | 3.671 | 56.914 | 0.42x |
| flat.json | json | 2.106 | 2.146 | 2.232 | 56.914 | 0.55x |
| nested.json | strata | 0.846 | 0.867 | 0.908 | 56.875 | 1.00x |
| nested.json | orjson | 1.229 | 1.274 | 1.308 | 56.875 | 0.68x |
| nested.json | msgspec | 1.135 | 1.185 | 1.207 | 56.875 | 0.73x |
| nested.json | ujson | 2.032 | 2.082 | 2.391 | 56.875 | 0.42x |
| nested.json | json | 2.296 | 2.348 | 2.439 | 56.875 | 0.37x |
| wide_arrays.json | strata | 4.811 | 4.896 | 5.038 | 58.695 | 1.00x |
| wide_arrays.json | orjson | 6.251 | 6.385 | 6.818 | 58.695 | 0.77x |
| wide_arrays.json | msgspec | 6.424 | 6.561 | 6.765 | 58.695 | 0.75x |
| wide_arrays.json | ujson | 11.462 | 11.659 | 12.209 | 58.695 | 0.42x |
| wide_arrays.json | json | 12.329 | 12.445 | 13.387 | 58.695 | 0.39x |
| mixed.json | strata | 0.265 | 0.277 | 0.329 | 57.668 | 1.00x |
| mixed.json | orjson | 0.323 | 0.333 | 0.372 | 57.668 | 0.83x |
| mixed.json | msgspec | 0.343 | 0.363 | 0.404 | 57.668 | 0.76x |
| mixed.json | ujson | 0.539 | 0.546 | 0.620 | 57.668 | 0.51x |
| mixed.json | json | 0.581 | 0.591 | 0.668 | 57.668 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 11.218 | 12.034 | 12.672 | 58.012 | 1.00x |
| users.ndjson | orjson | 17.929 | 18.573 | 19.875 | 58.012 | 0.65x |
| users.ndjson | msgspec | 18.495 | 19.222 | 21.524 | 58.012 | 0.63x |
| users.ndjson | ujson | 27.274 | 28.005 | 31.919 | 58.012 | 0.43x |
| users.ndjson | json | 31.524 | 32.300 | 40.193 | 58.012 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.189 | 4.732 | 5.843 | 58.312 | 1.00x |
| users.json | orjson | 4.491 | 5.018 | 6.991 | 58.312 | 0.94x |
| users.json | msgspec | 5.869 | 6.530 | 9.451 | 58.312 | 0.72x |
| users.json | ujson | 23.312 | 23.727 | 37.521 | 58.312 | 0.20x |
| users.json | json | 32.229 | 37.705 | 60.084 | 58.312 | 0.13x |
| flat.json | strata | 0.679 | 0.713 | 0.795 | 57.570 | 1.00x |
| flat.json | orjson | 0.721 | 0.759 | 0.841 | 57.570 | 0.94x |
| flat.json | msgspec | 0.876 | 0.918 | 0.957 | 57.570 | 0.78x |
| flat.json | ujson | 2.836 | 2.881 | 2.949 | 57.570 | 0.25x |
| flat.json | json | 3.313 | 3.343 | 3.406 | 57.570 | 0.21x |
| nested.json | strata | 0.657 | 0.691 | 0.789 | 57.289 | 1.00x |
| nested.json | orjson | 0.686 | 0.731 | 0.844 | 57.289 | 0.94x |
| nested.json | msgspec | 0.822 | 0.873 | 0.922 | 57.289 | 0.79x |
| nested.json | ujson | 2.309 | 2.331 | 2.422 | 57.289 | 0.30x |
| nested.json | json | 3.680 | 3.712 | 3.751 | 57.289 | 0.19x |
| wide_arrays.json | strata | 2.813 | 2.907 | 4.746 | 58.703 | 1.00x |
| wide_arrays.json | orjson | 3.217 | 3.317 | 3.373 | 58.703 | 0.88x |
| wide_arrays.json | msgspec | 4.805 | 4.927 | 5.224 | 58.703 | 0.59x |
| wide_arrays.json | ujson | 14.532 | 14.819 | 15.211 | 58.703 | 0.20x |
| wide_arrays.json | json | 25.386 | 25.851 | 26.611 | 58.703 | 0.11x |
| mixed.json | strata | 0.390 | 0.415 | 0.486 | 57.699 | 1.00x |
| mixed.json | orjson | 0.386 | 0.412 | 0.464 | 57.699 | 1.01x |
| mixed.json | msgspec | 0.415 | 0.434 | 0.485 | 57.699 | 0.96x |
| mixed.json | ujson | 0.744 | 0.767 | 0.834 | 57.699 | 0.54x |
| mixed.json | json | 1.010 | 1.053 | 1.084 | 57.699 | 0.39x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.090 | 0.097 | 0.154 | 57.699 | 1.00x |
| users.json $[*].id | jmespath | 0.440 | 0.451 | 0.746 | 57.699 | 0.22x |
| users.json $[*].id | jsonpath-ng | 2.527 | 2.686 | 3.878 | 57.699 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.464 | 0.507 | 0.798 | 57.910 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.782 | 2.863 | 5.451 | 57.910 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.621 | 18.748 | 32.283 | 57.910 | 0.03x |
| users.json $..total | strata | 1.922 | 1.963 | 2.740 | 57.977 | 1.00x |
| users.json $..total | jsonpath-ng | 335.625 | 370.701 | 392.004 | 57.977 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.033 | 4.079 | 4.816 | 57.660 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.427 | 15.894 | 26.163 | 57.660 | 0.26x |
| users.json $[*].id | orjson+jsonpath-ng | 17.378 | 18.663 | 27.543 | 57.660 | 0.22x |
| users.json $[*].orders[*].total | strata | 4.278 | 4.368 | 4.908 | 57.895 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.628 | 19.584 | 36.154 | 57.895 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.791 | 41.291 | 52.387 | 57.895 | 0.11x |
| users.json $..total | strata | 14.282 | 16.688 | 17.344 | 57.980 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 354.295 | 358.641 | 362.652 | 57.980 | 0.05x |

