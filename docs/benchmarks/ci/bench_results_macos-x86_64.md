# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c36c406
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
| users.json | strata | 21.480 | 21.926 | 24.823 | 52.539 | 1.00x |
| users.json | orjson | 27.004 | 27.537 | 30.630 | 52.539 | 0.80x |
| users.json | msgspec | 26.777 | 28.352 | 30.794 | 52.539 | 0.77x |
| users.json | ujson | 39.604 | 42.025 | 44.500 | 52.539 | 0.52x |
| users.json | pysimdjson | 176.874 | 179.500 | 190.284 | 52.539 | 0.12x |
| users.json | json | 45.535 | 47.593 | 48.941 | 52.539 | 0.46x |
| flat.json | strata | 1.408 | 1.459 | 1.868 | 60.340 | 1.00x |
| flat.json | orjson | 1.496 | 1.551 | 1.944 | 60.340 | 0.94x |
| flat.json | msgspec | 1.635 | 1.810 | 2.425 | 60.340 | 0.81x |
| flat.json | ujson | 2.873 | 3.089 | 3.815 | 60.340 | 0.47x |
| flat.json | pysimdjson | 15.222 | 16.494 | 21.610 | 60.340 | 0.09x |
| flat.json | json | 3.268 | 3.427 | 4.785 | 60.340 | 0.43x |
| nested.json | strata | 1.637 | 1.683 | 1.738 | 58.238 | 1.00x |
| nested.json | orjson | 1.837 | 1.866 | 1.925 | 58.238 | 0.90x |
| nested.json | msgspec | 2.021 | 2.038 | 2.154 | 58.238 | 0.83x |
| nested.json | ujson | 3.381 | 3.432 | 3.546 | 58.238 | 0.49x |
| nested.json | pysimdjson | 14.999 | 15.162 | 15.472 | 58.238 | 0.11x |
| nested.json | json | 4.317 | 4.360 | 4.577 | 58.238 | 0.39x |
| wide_arrays.json | strata | 9.133 | 9.474 | 10.073 | 60.914 | 1.00x |
| wide_arrays.json | orjson | 9.958 | 10.454 | 12.486 | 60.914 | 0.91x |
| wide_arrays.json | msgspec | 10.826 | 11.130 | 11.872 | 60.914 | 0.85x |
| wide_arrays.json | ujson | 14.049 | 14.416 | 15.916 | 60.914 | 0.66x |
| wide_arrays.json | pysimdjson | 85.926 | 88.804 | 89.594 | 60.914 | 0.11x |
| wide_arrays.json | json | 18.121 | 18.871 | 19.745 | 60.914 | 0.50x |
| mixed.json | strata | 0.390 | 0.395 | 0.431 | 58.074 | 1.00x |
| mixed.json | orjson | 0.449 | 0.457 | 0.489 | 58.074 | 0.86x |
| mixed.json | msgspec | 0.478 | 0.484 | 0.510 | 58.074 | 0.82x |
| mixed.json | ujson | 0.662 | 0.669 | 0.695 | 58.074 | 0.59x |
| mixed.json | pysimdjson | 3.402 | 3.423 | 3.819 | 58.074 | 0.12x |
| mixed.json | json | 0.926 | 0.939 | 1.044 | 58.074 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.172 | 3.281 | 3.469 | 47.051 | 1.00x |
| users.json | orjson | 3.553 | 3.866 | 4.382 | 47.051 | 0.85x |
| users.json | msgspec | 5.477 | 5.669 | 6.222 | 47.051 | 0.58x |
| users.json | ujson | 33.407 | 34.548 | 36.053 | 47.051 | 0.09x |
| users.json | json | 44.390 | 45.984 | 49.554 | 47.051 | 0.07x |
| flat.json | strata | 0.348 | 0.373 | 0.611 | 58.098 | 1.00x |
| flat.json | orjson | 0.431 | 0.481 | 0.623 | 58.098 | 0.78x |
| flat.json | msgspec | 0.580 | 0.636 | 0.693 | 58.098 | 0.59x |
| flat.json | ujson | 2.836 | 3.038 | 4.297 | 58.098 | 0.12x |
| flat.json | json | 3.808 | 4.301 | 6.925 | 58.098 | 0.09x |
| nested.json | strata | 0.290 | 0.295 | 0.319 | 53.176 | 1.00x |
| nested.json | orjson | 0.384 | 0.390 | 0.397 | 53.176 | 0.76x |
| nested.json | msgspec | 0.613 | 0.622 | 0.703 | 53.176 | 0.47x |
| nested.json | ujson | 3.157 | 3.191 | 3.245 | 53.176 | 0.09x |
| nested.json | json | 5.156 | 5.186 | 5.533 | 53.176 | 0.06x |
| wide_arrays.json | strata | 1.890 | 1.932 | 2.141 | 57.801 | 1.00x |
| wide_arrays.json | orjson | 2.251 | 2.390 | 3.004 | 57.801 | 0.81x |
| wide_arrays.json | msgspec | 3.218 | 3.353 | 3.502 | 57.801 | 0.58x |
| wide_arrays.json | ujson | 15.690 | 16.276 | 17.164 | 57.801 | 0.12x |
| wide_arrays.json | json | 35.214 | 36.382 | 38.563 | 57.801 | 0.05x |
| mixed.json | strata | 0.080 | 0.087 | 0.091 | 53.637 | 1.00x |
| mixed.json | orjson | 0.072 | 0.076 | 0.086 | 53.637 | 1.15x |
| mixed.json | msgspec | 0.105 | 0.111 | 0.119 | 53.637 | 0.79x |
| mixed.json | ujson | 0.638 | 0.643 | 0.754 | 53.637 | 0.14x |
| mixed.json | json | 1.007 | 1.015 | 1.105 | 53.637 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 21.569 | 22.764 | 24.275 | 59.332 | 1.00x |
| users.json | orjson | 27.447 | 29.906 | 35.612 | 59.332 | 0.76x |
| users.json | msgspec | 27.750 | 30.188 | 35.756 | 59.332 | 0.75x |
| users.json | ujson | 41.807 | 43.562 | 50.205 | 59.332 | 0.52x |
| users.json | json | 45.678 | 50.182 | 56.063 | 59.332 | 0.45x |
| flat.json | strata | 1.576 | 1.622 | 1.678 | 58.098 | 1.00x |
| flat.json | orjson | 1.663 | 1.768 | 1.931 | 58.098 | 0.92x |
| flat.json | msgspec | 1.880 | 1.945 | 2.551 | 58.098 | 0.83x |
| flat.json | ujson | 3.213 | 3.290 | 3.633 | 58.098 | 0.49x |
| flat.json | json | 3.625 | 3.688 | 3.906 | 58.098 | 0.44x |
| nested.json | strata | 1.766 | 1.796 | 1.841 | 53.176 | 1.00x |
| nested.json | orjson | 1.985 | 2.029 | 2.118 | 53.176 | 0.88x |
| nested.json | msgspec | 2.190 | 2.251 | 2.311 | 53.176 | 0.80x |
| nested.json | ujson | 3.575 | 3.624 | 4.208 | 53.176 | 0.50x |
| nested.json | json | 4.496 | 4.560 | 8.176 | 53.176 | 0.39x |
| wide_arrays.json | strata | 8.808 | 8.928 | 9.818 | 58.898 | 1.00x |
| wide_arrays.json | orjson | 9.560 | 10.553 | 11.559 | 58.898 | 0.85x |
| wide_arrays.json | msgspec | 10.721 | 11.009 | 13.528 | 58.898 | 0.81x |
| wide_arrays.json | ujson | 13.884 | 14.611 | 15.719 | 58.898 | 0.61x |
| wide_arrays.json | json | 17.522 | 18.017 | 19.751 | 58.898 | 0.50x |
| mixed.json | strata | 0.458 | 0.473 | 0.520 | 53.637 | 1.00x |
| mixed.json | orjson | 0.554 | 0.568 | 0.733 | 53.637 | 0.83x |
| mixed.json | msgspec | 0.579 | 0.598 | 0.665 | 53.637 | 0.79x |
| mixed.json | ujson | 0.778 | 0.803 | 0.878 | 53.637 | 0.59x |
| mixed.json | json | 1.011 | 1.036 | 1.115 | 53.637 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 19.544 | 21.468 | 22.824 | 60.676 | 1.00x |
| users.ndjson | orjson | 26.645 | 29.305 | 29.987 | 60.676 | 0.73x |
| users.ndjson | msgspec | 27.298 | 29.602 | 33.140 | 60.676 | 0.73x |
| users.ndjson | ujson | 37.531 | 42.686 | 49.800 | 60.676 | 0.50x |
| users.ndjson | json | 46.670 | 52.436 | 58.169 | 60.676 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.210 | 4.522 | 7.361 | 58.309 | 1.00x |
| users.json | orjson | 4.716 | 5.044 | 7.336 | 58.309 | 0.90x |
| users.json | msgspec | 6.764 | 7.321 | 9.321 | 58.309 | 0.62x |
| users.json | ujson | 35.971 | 36.356 | 40.931 | 58.309 | 0.12x |
| users.json | json | 47.711 | 48.107 | 53.460 | 58.309 | 0.09x |
| flat.json | strata | 0.703 | 0.741 | 1.128 | 58.098 | 1.00x |
| flat.json | orjson | 0.837 | 0.885 | 0.920 | 58.098 | 0.84x |
| flat.json | msgspec | 0.965 | 1.024 | 1.140 | 58.098 | 0.72x |
| flat.json | ujson | 3.321 | 3.463 | 4.566 | 58.098 | 0.21x |
| flat.json | json | 4.410 | 4.594 | 4.866 | 58.098 | 0.16x |
| nested.json | strata | 0.632 | 0.658 | 0.687 | 53.176 | 1.00x |
| nested.json | orjson | 0.715 | 0.762 | 0.806 | 53.176 | 0.86x |
| nested.json | msgspec | 0.958 | 0.998 | 1.070 | 53.176 | 0.66x |
| nested.json | ujson | 3.489 | 3.557 | 4.018 | 53.176 | 0.19x |
| nested.json | json | 5.548 | 5.638 | 5.713 | 53.176 | 0.12x |
| wide_arrays.json | strata | 2.571 | 2.702 | 3.622 | 58.898 | 1.00x |
| wide_arrays.json | orjson | 3.005 | 3.222 | 3.647 | 58.898 | 0.84x |
| wide_arrays.json | msgspec | 4.019 | 4.203 | 5.376 | 58.898 | 0.64x |
| wide_arrays.json | ujson | 16.945 | 17.359 | 19.273 | 58.898 | 0.16x |
| wide_arrays.json | json | 37.085 | 37.574 | 39.726 | 58.898 | 0.07x |
| mixed.json | strata | 0.335 | 0.363 | 0.419 | 53.637 | 1.00x |
| mixed.json | orjson | 0.325 | 0.364 | 0.524 | 53.637 | 1.00x |
| mixed.json | msgspec | 0.372 | 0.391 | 0.517 | 53.637 | 0.93x |
| mixed.json | ujson | 0.901 | 0.982 | 1.094 | 53.637 | 0.37x |
| mixed.json | json | 1.291 | 1.324 | 1.661 | 53.637 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.143 | 0.149 | 0.180 | 58.391 | 1.00x |
| users.json $[*].id | jmespath | 1.037 | 1.063 | 1.110 | 58.391 | 0.14x |
| users.json $[*].id | jsonpath-ng | 5.618 | 5.673 | 6.048 | 58.391 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.909 | 1.076 | 1.206 | 55.500 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.431 | 6.823 | 7.735 | 55.500 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 37.349 | 37.857 | 42.553 | 55.500 | 0.03x |
| users.json $..total | strata | 3.408 | 3.501 | 3.814 | 56.578 | 1.00x |
| users.json $..total | jsonpath-ng | 768.368 | 781.728 | 794.861 | 56.578 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.276 | 4.345 | 4.405 | 58.465 | 1.00x |
| users.json $[*].id | orjson+jmespath | 27.726 | 28.775 | 30.134 | 58.465 | 0.15x |
| users.json $[*].id | orjson+jsonpath-ng | 32.915 | 33.627 | 36.008 | 58.465 | 0.13x |
| users.json $[*].orders[*].total | strata | 4.678 | 4.817 | 5.328 | 55.746 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 33.592 | 34.577 | 37.118 | 55.746 | 0.14x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 69.825 | 71.132 | 76.631 | 55.746 | 0.07x |
| users.json $..total | strata | 23.281 | 24.734 | 29.395 | 55.895 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 729.320 | 753.379 | 794.914 | 55.895 | 0.03x |

