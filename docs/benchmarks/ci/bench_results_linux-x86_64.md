# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: f35a664
- python: 3.12.13
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: x86_64
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.534 | 11.382 | 21.106 | 57.551 | 1.00x |
| users.json | orjson | 13.798 | 14.784 | 23.486 | 57.551 | 0.77x |
| users.json | msgspec | 13.716 | 14.552 | 24.542 | 57.551 | 0.78x |
| users.json | ujson | 18.325 | 20.242 | 36.653 | 57.551 | 0.56x |
| users.json | pysimdjson | 19.201 | 20.876 | 32.077 | 57.551 | 0.55x |
| users.json | json | 21.140 | 22.387 | 34.705 | 57.551 | 0.51x |
| flat.json | strata | 0.982 | 1.003 | 1.039 | 70.105 | 1.00x |
| flat.json | orjson | 1.067 | 1.100 | 1.156 | 70.105 | 0.91x |
| flat.json | msgspec | 1.057 | 1.072 | 1.116 | 70.105 | 0.94x |
| flat.json | ujson | 1.538 | 1.575 | 1.693 | 70.105 | 0.64x |
| flat.json | pysimdjson | 1.655 | 1.714 | 1.821 | 70.105 | 0.59x |
| flat.json | json | 1.746 | 1.770 | 1.818 | 70.105 | 0.57x |
| nested.json | strata | 0.859 | 0.868 | 0.885 | 70.105 | 1.00x |
| nested.json | orjson | 0.993 | 0.997 | 1.020 | 70.105 | 0.87x |
| nested.json | msgspec | 0.950 | 0.976 | 0.991 | 70.105 | 0.89x |
| nested.json | ujson | 1.417 | 1.470 | 1.518 | 70.105 | 0.59x |
| nested.json | pysimdjson | 1.391 | 1.407 | 1.420 | 70.105 | 0.62x |
| nested.json | json | 1.833 | 1.853 | 1.922 | 70.105 | 0.47x |
| wide_arrays.json | strata | 5.173 | 5.300 | 5.641 | 74.691 | 1.00x |
| wide_arrays.json | orjson | 5.519 | 5.625 | 5.782 | 74.691 | 0.94x |
| wide_arrays.json | msgspec | 6.019 | 6.112 | 6.300 | 74.691 | 0.87x |
| wide_arrays.json | ujson | 7.738 | 7.797 | 7.997 | 74.691 | 0.68x |
| wide_arrays.json | pysimdjson | 6.301 | 6.509 | 7.262 | 74.691 | 0.81x |
| wide_arrays.json | json | 9.850 | 10.024 | 10.584 | 74.691 | 0.53x |
| mixed.json | strata | 0.202 | 0.207 | 0.246 | 74.754 | 1.00x |
| mixed.json | orjson | 0.232 | 0.237 | 0.251 | 74.754 | 0.87x |
| mixed.json | msgspec | 0.244 | 0.252 | 0.324 | 74.754 | 0.82x |
| mixed.json | ujson | 0.309 | 0.323 | 0.369 | 74.754 | 0.64x |
| mixed.json | pysimdjson | 0.299 | 0.306 | 0.321 | 74.754 | 0.68x |
| mixed.json | json | 0.451 | 0.464 | 0.483 | 74.754 | 0.45x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.480 | 2.520 | 2.671 | 56.035 | 1.00x |
| users.json | orjson | 3.064 | 3.091 | 3.183 | 56.035 | 0.82x |
| users.json | msgspec | 4.172 | 4.221 | 5.086 | 56.035 | 0.60x |
| users.json | ujson | 14.448 | 14.638 | 16.540 | 56.035 | 0.17x |
| users.json | json | 21.785 | 22.066 | 24.238 | 56.035 | 0.11x |
| flat.json | strata | 0.279 | 0.282 | 0.298 | 70.105 | 1.00x |
| flat.json | orjson | 0.368 | 0.375 | 0.384 | 70.105 | 0.75x |
| flat.json | msgspec | 0.475 | 0.478 | 0.491 | 70.105 | 0.59x |
| flat.json | ujson | 1.258 | 1.269 | 1.298 | 70.105 | 0.22x |
| flat.json | json | 1.918 | 1.926 | 1.961 | 70.105 | 0.15x |
| nested.json | strata | 0.233 | 0.241 | 0.251 | 70.105 | 1.00x |
| nested.json | orjson | 0.300 | 0.309 | 0.323 | 70.105 | 0.78x |
| nested.json | msgspec | 0.419 | 0.422 | 0.452 | 70.105 | 0.57x |
| nested.json | ujson | 1.355 | 1.362 | 1.432 | 70.105 | 0.18x |
| nested.json | json | 2.359 | 2.388 | 2.458 | 70.105 | 0.10x |
| wide_arrays.json | strata | 1.749 | 1.759 | 1.783 | 74.691 | 1.00x |
| wide_arrays.json | orjson | 1.891 | 1.910 | 1.947 | 74.691 | 0.92x |
| wide_arrays.json | msgspec | 2.940 | 2.973 | 3.008 | 74.691 | 0.59x |
| wide_arrays.json | ujson | 8.579 | 8.673 | 8.995 | 74.691 | 0.20x |
| wide_arrays.json | json | 16.820 | 16.995 | 17.325 | 74.691 | 0.10x |
| mixed.json | strata | 0.065 | 0.066 | 0.073 | 74.754 | 1.00x |
| mixed.json | orjson | 0.068 | 0.069 | 0.081 | 74.754 | 0.96x |
| mixed.json | msgspec | 0.086 | 0.089 | 0.107 | 74.754 | 0.74x |
| mixed.json | ujson | 0.296 | 0.302 | 0.318 | 74.754 | 0.22x |
| mixed.json | json | 0.528 | 0.533 | 0.543 | 74.754 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.358 | 13.023 | 15.489 | 70.441 | 1.00x |
| users.json | orjson | 14.943 | 15.849 | 16.953 | 70.441 | 0.82x |
| users.json | msgspec | 14.819 | 15.672 | 17.913 | 70.441 | 0.83x |
| users.json | ujson | 21.105 | 22.475 | 27.299 | 70.441 | 0.58x |
| users.json | json | 21.868 | 22.822 | 25.502 | 70.441 | 0.57x |
| flat.json | strata | 1.022 | 1.066 | 1.140 | 70.105 | 1.00x |
| flat.json | orjson | 1.142 | 1.197 | 1.256 | 70.105 | 0.89x |
| flat.json | msgspec | 1.106 | 1.165 | 1.189 | 70.105 | 0.91x |
| flat.json | ujson | 1.639 | 1.775 | 1.809 | 70.105 | 0.60x |
| flat.json | json | 1.823 | 1.841 | 1.933 | 70.105 | 0.58x |
| nested.json | strata | 0.890 | 0.911 | 0.933 | 70.105 | 1.00x |
| nested.json | orjson | 1.044 | 1.089 | 1.104 | 70.105 | 0.84x |
| nested.json | msgspec | 1.019 | 1.056 | 1.162 | 70.105 | 0.86x |
| nested.json | ujson | 1.468 | 1.541 | 1.598 | 70.105 | 0.59x |
| nested.json | json | 1.888 | 1.942 | 1.992 | 70.105 | 0.47x |
| wide_arrays.json | strata | 5.203 | 5.425 | 5.611 | 74.754 | 1.00x |
| wide_arrays.json | orjson | 5.521 | 5.893 | 6.085 | 74.754 | 0.92x |
| wide_arrays.json | msgspec | 6.283 | 6.411 | 6.654 | 74.754 | 0.85x |
| wide_arrays.json | ujson | 7.964 | 8.241 | 8.690 | 74.754 | 0.66x |
| wide_arrays.json | json | 9.817 | 10.181 | 10.603 | 74.754 | 0.53x |
| mixed.json | strata | 0.234 | 0.248 | 0.259 | 74.754 | 1.00x |
| mixed.json | orjson | 0.288 | 0.297 | 0.328 | 74.754 | 0.83x |
| mixed.json | msgspec | 0.296 | 0.314 | 0.331 | 74.754 | 0.79x |
| mixed.json | ujson | 0.395 | 0.405 | 0.427 | 74.754 | 0.61x |
| mixed.json | json | 0.503 | 0.516 | 0.532 | 74.754 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 12.679 | 13.569 | 14.391 | 70.105 | 1.00x |
| users.ndjson | orjson | 18.677 | 19.532 | 20.084 | 70.105 | 0.69x |
| users.ndjson | msgspec | 18.273 | 19.480 | 21.799 | 70.105 | 0.70x |
| users.ndjson | ujson | 24.656 | 25.518 | 27.015 | 70.105 | 0.53x |
| users.ndjson | json | 30.655 | 31.491 | 32.029 | 70.105 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.113 | 3.179 | 3.385 | 67.457 | 1.00x |
| users.json | orjson | 3.732 | 3.839 | 4.009 | 67.457 | 0.83x |
| users.json | msgspec | 4.797 | 4.937 | 5.061 | 67.457 | 0.64x |
| users.json | ujson | 15.505 | 15.635 | 15.836 | 67.457 | 0.20x |
| users.json | json | 23.107 | 25.608 | 26.052 | 67.457 | 0.12x |
| flat.json | strata | 0.441 | 0.453 | 0.474 | 70.105 | 1.00x |
| flat.json | orjson | 0.533 | 0.554 | 0.763 | 70.105 | 0.82x |
| flat.json | msgspec | 0.656 | 0.674 | 0.722 | 70.105 | 0.67x |
| flat.json | ujson | 1.456 | 1.478 | 1.520 | 70.105 | 0.31x |
| flat.json | json | 2.143 | 2.164 | 2.208 | 70.105 | 0.21x |
| nested.json | strata | 0.366 | 0.379 | 0.397 | 70.105 | 1.00x |
| nested.json | orjson | 0.455 | 0.465 | 0.512 | 70.105 | 0.81x |
| nested.json | msgspec | 0.576 | 0.587 | 0.620 | 70.105 | 0.65x |
| nested.json | ujson | 1.531 | 1.557 | 1.737 | 70.105 | 0.24x |
| nested.json | json | 2.570 | 2.601 | 2.958 | 70.105 | 0.15x |
| wide_arrays.json | strata | 2.140 | 2.210 | 2.271 | 74.754 | 1.00x |
| wide_arrays.json | orjson | 2.314 | 2.405 | 2.488 | 74.754 | 0.92x |
| wide_arrays.json | msgspec | 3.403 | 3.428 | 4.100 | 74.754 | 0.64x |
| wide_arrays.json | ujson | 9.060 | 9.155 | 9.261 | 74.754 | 0.24x |
| wide_arrays.json | json | 17.250 | 17.432 | 17.805 | 74.754 | 0.13x |
| mixed.json | strata | 0.166 | 0.183 | 0.210 | 74.754 | 1.00x |
| mixed.json | orjson | 0.184 | 0.202 | 0.226 | 74.754 | 0.91x |
| mixed.json | msgspec | 0.202 | 0.214 | 0.251 | 74.754 | 0.85x |
| mixed.json | ujson | 0.420 | 0.452 | 0.495 | 74.754 | 0.40x |
| mixed.json | json | 0.659 | 0.687 | 0.740 | 74.754 | 0.27x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.079 | 0.085 | 0.099 | 67.457 | 1.00x |
| users.json $[*].id | jmespath | 0.488 | 0.520 | 0.597 | 67.457 | 0.16x |
| users.json $[*].id | jsonpath-ng | 2.894 | 3.032 | 3.322 | 67.457 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.437 | 0.453 | 0.476 | 67.457 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.250 | 3.332 | 4.371 | 67.457 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.668 | 20.217 | 21.562 | 67.457 | 0.02x |
| users.json $..total | strata | 1.768 | 1.836 | 2.153 | 71.734 | 1.00x |
| users.json $..total | jsonpath-ng | 386.474 | 390.004 | 418.806 | 71.734 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.414 | 3.454 | 4.352 | 67.457 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.362 | 15.845 | 16.981 | 67.457 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 17.710 | 18.252 | 20.604 | 67.457 | 0.19x |
| users.json $[*].orders[*].total | strata | 3.679 | 3.724 | 3.890 | 71.734 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.752 | 19.806 | 22.380 | 71.734 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 40.519 | 44.055 | 47.821 | 71.734 | 0.08x |
| users.json $..total | strata | 14.357 | 15.676 | 17.695 | 71.734 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 412.368 | 416.618 | 429.582 | 71.734 | 0.04x |

