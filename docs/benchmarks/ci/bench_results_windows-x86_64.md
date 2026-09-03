# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 59554e7
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
| users.json | strata | 10.282 | 10.896 | 24.812 | 45.562 | 1.00x |
| users.json | orjson | 13.697 | 14.236 | 16.597 | 45.562 | 0.77x |
| users.json | msgspec | 12.782 | 13.308 | 15.525 | 45.562 | 0.82x |
| users.json | ujson | 21.537 | 22.026 | 34.786 | 45.562 | 0.49x |
| users.json | json | 22.527 | 23.303 | 45.307 | 45.562 | 0.47x |
| flat.json | strata | 1.468 | 1.785 | 3.502 | 48.152 | 1.00x |
| flat.json | orjson | 1.757 | 2.027 | 3.777 | 48.152 | 0.88x |
| flat.json | msgspec | 1.801 | 2.104 | 3.753 | 48.152 | 0.85x |
| flat.json | ujson | 3.180 | 4.358 | 52.813 | 48.152 | 0.41x |
| flat.json | json | 3.819 | 4.680 | 5.515 | 48.152 | 0.38x |
| nested.json | strata | 1.331 | 1.353 | 3.237 | 47.629 | 1.00x |
| nested.json | orjson | 1.733 | 1.772 | 1.867 | 47.629 | 0.76x |
| nested.json | msgspec | 1.688 | 1.915 | 3.770 | 47.629 | 0.71x |
| nested.json | ujson | 2.761 | 4.548 | 4.974 | 47.629 | 0.30x |
| nested.json | json | 4.146 | 5.754 | 6.993 | 47.629 | 0.24x |
| wide_arrays.json | strata | 8.368 | 9.280 | 66.800 | 49.602 | 1.00x |
| wide_arrays.json | orjson | 9.818 | 11.310 | 13.633 | 49.602 | 0.82x |
| wide_arrays.json | msgspec | 10.531 | 12.437 | 60.407 | 49.602 | 0.75x |
| wide_arrays.json | ujson | 14.441 | 16.905 | 21.995 | 49.602 | 0.55x |
| wide_arrays.json | json | 22.793 | 24.908 | 30.568 | 49.602 | 0.37x |
| mixed.json | strata | 0.333 | 0.335 | 2.133 | 47.328 | 1.00x |
| mixed.json | orjson | 0.437 | 0.464 | 2.184 | 47.328 | 0.72x |
| mixed.json | msgspec | 0.411 | 0.424 | 2.251 | 47.328 | 0.79x |
| mixed.json | ujson | 0.630 | 0.658 | 2.407 | 47.328 | 0.51x |
| mixed.json | json | 0.872 | 0.903 | 1.230 | 47.328 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.316 | 3.441 | 3.632 | 45.801 | 1.00x |
| users.json | orjson | 3.463 | 3.815 | 3.980 | 45.801 | 0.90x |
| users.json | msgspec | 5.089 | 5.534 | 9.177 | 45.801 | 0.62x |
| users.json | ujson | 17.214 | 17.620 | 30.865 | 45.801 | 0.20x |
| users.json | json | 23.865 | 24.433 | 25.127 | 45.801 | 0.14x |
| flat.json | strata | 0.428 | 0.465 | 0.615 | 48.383 | 1.00x |
| flat.json | orjson | 0.531 | 0.556 | 2.451 | 48.383 | 0.84x |
| flat.json | msgspec | 0.717 | 0.803 | 3.316 | 48.383 | 0.58x |
| flat.json | ujson | 2.648 | 2.733 | 5.237 | 48.383 | 0.17x |
| flat.json | json | 3.464 | 5.554 | 22.629 | 48.383 | 0.08x |
| nested.json | strata | 0.419 | 0.430 | 2.214 | 48.039 | 1.00x |
| nested.json | orjson | 0.487 | 0.496 | 2.438 | 48.039 | 0.87x |
| nested.json | msgspec | 0.735 | 0.762 | 2.960 | 48.039 | 0.56x |
| nested.json | ujson | 2.311 | 2.354 | 4.186 | 48.039 | 0.18x |
| nested.json | json | 4.382 | 6.199 | 6.679 | 48.039 | 0.07x |
| wide_arrays.json | strata | 3.360 | 3.721 | 5.330 | 50.219 | 1.00x |
| wide_arrays.json | orjson | 3.474 | 3.627 | 5.661 | 50.219 | 1.03x |
| wide_arrays.json | msgspec | 5.381 | 6.328 | 7.584 | 50.219 | 0.59x |
| wide_arrays.json | ujson | 16.551 | 18.489 | 21.833 | 50.219 | 0.20x |
| wide_arrays.json | json | 33.036 | 34.243 | 88.562 | 50.219 | 0.11x |
| mixed.json | strata | 0.112 | 0.114 | 0.179 | 47.652 | 1.00x |
| mixed.json | orjson | 0.104 | 0.106 | 0.133 | 47.652 | 1.08x |
| mixed.json | msgspec | 0.140 | 0.147 | 2.002 | 47.652 | 0.78x |
| mixed.json | ujson | 0.550 | 0.565 | 2.418 | 47.652 | 0.20x |
| mixed.json | json | 0.924 | 0.961 | 2.910 | 47.652 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.131 | 12.014 | 17.002 | 54.312 | 1.00x |
| users.json | orjson | 14.538 | 15.318 | 23.205 | 54.312 | 0.78x |
| users.json | msgspec | 14.014 | 14.698 | 25.928 | 54.312 | 0.82x |
| users.json | ujson | 27.011 | 28.805 | 36.680 | 54.312 | 0.42x |
| users.json | json | 23.568 | 25.025 | 30.682 | 54.312 | 0.48x |
| flat.json | strata | 1.897 | 2.163 | 4.212 | 47.812 | 1.00x |
| flat.json | orjson | 2.172 | 2.278 | 4.335 | 47.812 | 0.95x |
| flat.json | msgspec | 2.037 | 2.337 | 48.234 | 47.812 | 0.93x |
| flat.json | ujson | 4.156 | 6.440 | 13.242 | 47.812 | 0.34x |
| flat.json | json | 3.691 | 5.934 | 6.231 | 47.812 | 0.36x |
| nested.json | strata | 1.481 | 1.634 | 3.303 | 48.039 | 1.00x |
| nested.json | orjson | 1.874 | 1.943 | 45.270 | 48.039 | 0.84x |
| nested.json | msgspec | 1.854 | 2.093 | 4.030 | 48.039 | 0.78x |
| nested.json | ujson | 3.263 | 3.616 | 5.553 | 48.039 | 0.45x |
| nested.json | json | 4.055 | 4.738 | 51.361 | 48.039 | 0.34x |
| wide_arrays.json | strata | 9.365 | 11.115 | 53.862 | 50.219 | 1.00x |
| wide_arrays.json | orjson | 10.629 | 11.699 | 16.092 | 50.219 | 0.95x |
| wide_arrays.json | msgspec | 11.249 | 12.172 | 17.045 | 50.219 | 0.91x |
| wide_arrays.json | ujson | 18.736 | 19.717 | 30.673 | 50.219 | 0.56x |
| wide_arrays.json | json | 23.280 | 24.658 | 87.027 | 50.219 | 0.45x |
| mixed.json | strata | 0.426 | 0.434 | 0.741 | 47.488 | 1.00x |
| mixed.json | orjson | 0.599 | 0.623 | 2.519 | 47.488 | 0.70x |
| mixed.json | msgspec | 0.573 | 0.600 | 0.630 | 47.488 | 0.72x |
| mixed.json | ujson | 0.853 | 0.890 | 1.173 | 47.488 | 0.49x |
| mixed.json | json | 0.993 | 1.032 | 1.339 | 47.488 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 23.361 | 25.549 | 118.451 | 48.906 | 1.00x |
| users.ndjson | orjson | 36.603 | 40.474 | 95.117 | 48.906 | 0.63x |
| users.ndjson | msgspec | 31.578 | 38.873 | 44.899 | 48.906 | 0.66x |
| users.ndjson | ujson | 47.847 | 59.674 | 117.652 | 48.906 | 0.43x |
| users.ndjson | json | 60.595 | 72.597 | 127.570 | 48.906 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.336 | 4.408 | 6.661 | 51.695 | 1.00x |
| users.json | orjson | 4.407 | 4.563 | 5.246 | 51.695 | 0.97x |
| users.json | msgspec | 5.999 | 6.182 | 6.732 | 51.695 | 0.71x |
| users.json | ujson | 26.898 | 27.397 | 45.450 | 51.695 | 0.16x |
| users.json | json | 32.831 | 33.712 | 56.422 | 51.695 | 0.13x |
| flat.json | strata | 0.908 | 0.973 | 3.194 | 48.531 | 1.00x |
| flat.json | orjson | 1.019 | 1.053 | 3.721 | 48.531 | 0.92x |
| flat.json | msgspec | 1.189 | 1.252 | 4.048 | 48.531 | 0.78x |
| flat.json | ujson | 5.030 | 7.234 | 7.820 | 48.531 | 0.13x |
| flat.json | json | 6.188 | 8.489 | 11.627 | 48.531 | 0.11x |
| nested.json | strata | 0.893 | 0.901 | 1.373 | 48.039 | 1.00x |
| nested.json | orjson | 0.930 | 1.001 | 2.602 | 48.039 | 0.90x |
| nested.json | msgspec | 1.210 | 1.302 | 2.978 | 48.039 | 0.69x |
| nested.json | ujson | 4.449 | 4.535 | 9.389 | 48.039 | 0.20x |
| nested.json | json | 6.178 | 6.743 | 9.394 | 48.039 | 0.13x |
| wide_arrays.json | strata | 4.708 | 6.489 | 50.460 | 50.219 | 1.00x |
| wide_arrays.json | orjson | 4.496 | 5.331 | 6.596 | 50.219 | 1.22x |
| wide_arrays.json | msgspec | 6.425 | 7.788 | 42.665 | 50.219 | 0.83x |
| wide_arrays.json | ujson | 30.085 | 34.607 | 106.795 | 50.219 | 0.19x |
| wide_arrays.json | json | 45.415 | 48.630 | 158.858 | 50.219 | 0.13x |
| mixed.json | strata | 0.522 | 0.536 | 0.791 | 47.680 | 1.00x |
| mixed.json | orjson | 0.510 | 0.527 | 0.850 | 47.680 | 1.02x |
| mixed.json | msgspec | 0.551 | 0.572 | 1.003 | 47.680 | 0.94x |
| mixed.json | ujson | 1.279 | 1.339 | 7.696 | 47.680 | 0.40x |
| mixed.json | json | 1.639 | 1.824 | 2.525 | 47.680 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.086 | 0.099 | 0.146 | 50.598 | 1.00x |
| users.json $[*].id | jmespath | 0.477 | 0.496 | 0.795 | 50.598 | 0.20x |
| users.json $[*].id | jsonpath-ng | 2.539 | 2.780 | 3.070 | 50.598 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.471 | 0.514 | 0.858 | 50.840 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.997 | 3.166 | 7.372 | 50.840 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.336 | 18.772 | 31.344 | 50.840 | 0.03x |
| users.json $..total | strata | 1.805 | 2.059 | 5.156 | 52.469 | 1.00x |
| users.json $..total | jsonpath-ng | 364.988 | 418.359 | 824.900 | 52.469 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.095 | 4.279 | 6.713 | 50.609 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.294 | 15.893 | 17.220 | 50.609 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 17.128 | 18.296 | 29.153 | 50.609 | 0.23x |
| users.json $[*].orders[*].total | strata | 4.339 | 4.523 | 7.546 | 52.465 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.273 | 18.840 | 19.655 | 52.465 | 0.24x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.806 | 38.172 | 41.628 | 52.465 | 0.12x |
| users.json $..total | strata | 14.281 | 20.489 | 39.056 | 52.469 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 448.206 | 620.587 | 1050.805 | 52.469 | 0.03x |

