# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: dc84475
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
| users.json | strata | 10.971 | 12.067 | 24.182 | 59.262 | 1.00x |
| users.json | orjson | 13.129 | 14.333 | 25.432 | 59.262 | 0.84x |
| users.json | msgspec | 13.051 | 13.488 | 19.337 | 59.262 | 0.89x |
| users.json | ujson | 18.242 | 21.776 | 37.413 | 59.262 | 0.55x |
| users.json | pysimdjson | 17.868 | 19.603 | 30.716 | 59.262 | 0.62x |
| users.json | json | 22.248 | 24.641 | 38.724 | 59.262 | 0.49x |
| flat.json | strata | 0.973 | 0.991 | 1.026 | 69.945 | 1.00x |
| flat.json | orjson | 1.016 | 1.026 | 1.050 | 69.945 | 0.97x |
| flat.json | msgspec | 0.994 | 1.010 | 1.053 | 69.945 | 0.98x |
| flat.json | ujson | 1.515 | 1.598 | 1.787 | 69.945 | 0.62x |
| flat.json | pysimdjson | 1.534 | 1.584 | 1.771 | 69.945 | 0.63x |
| flat.json | json | 1.873 | 1.895 | 1.945 | 69.945 | 0.52x |
| nested.json | strata | 0.881 | 0.896 | 0.908 | 69.945 | 1.00x |
| nested.json | orjson | 1.017 | 1.025 | 1.037 | 69.945 | 0.87x |
| nested.json | msgspec | 1.016 | 1.022 | 1.141 | 69.945 | 0.88x |
| nested.json | ujson | 1.506 | 1.525 | 1.578 | 69.945 | 0.59x |
| nested.json | pysimdjson | 1.423 | 1.442 | 1.473 | 69.945 | 0.62x |
| nested.json | json | 2.102 | 2.118 | 2.150 | 69.945 | 0.42x |
| wide_arrays.json | strata | 4.729 | 4.860 | 5.045 | 73.949 | 1.00x |
| wide_arrays.json | orjson | 5.088 | 5.202 | 5.837 | 73.949 | 0.93x |
| wide_arrays.json | msgspec | 5.613 | 5.669 | 5.932 | 73.949 | 0.86x |
| wide_arrays.json | ujson | 7.125 | 7.269 | 7.723 | 73.949 | 0.67x |
| wide_arrays.json | pysimdjson | 6.077 | 6.183 | 6.837 | 73.949 | 0.79x |
| wide_arrays.json | json | 9.792 | 10.119 | 10.776 | 73.949 | 0.48x |
| mixed.json | strata | 0.219 | 0.225 | 0.240 | 74.012 | 1.00x |
| mixed.json | orjson | 0.232 | 0.240 | 0.256 | 74.012 | 0.94x |
| mixed.json | msgspec | 0.240 | 0.247 | 0.264 | 74.012 | 0.91x |
| mixed.json | ujson | 0.305 | 0.320 | 0.341 | 74.012 | 0.70x |
| mixed.json | pysimdjson | 0.300 | 0.304 | 0.325 | 74.012 | 0.74x |
| mixed.json | json | 0.472 | 0.485 | 0.505 | 74.012 | 0.46x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.926 | 2.938 | 2.989 | 58.359 | 1.00x |
| users.json | orjson | 2.904 | 2.933 | 4.069 | 58.359 | 1.00x |
| users.json | msgspec | 3.844 | 3.858 | 4.005 | 58.359 | 0.76x |
| users.json | ujson | 14.168 | 14.395 | 15.158 | 58.359 | 0.20x |
| users.json | json | 22.548 | 22.878 | 25.154 | 58.359 | 0.13x |
| flat.json | strata | 0.299 | 0.307 | 0.318 | 69.945 | 1.00x |
| flat.json | orjson | 0.331 | 0.335 | 0.351 | 69.945 | 0.92x |
| flat.json | msgspec | 0.437 | 0.440 | 0.455 | 69.945 | 0.70x |
| flat.json | ujson | 1.250 | 1.261 | 1.274 | 69.945 | 0.24x |
| flat.json | json | 1.877 | 1.884 | 1.912 | 69.945 | 0.16x |
| nested.json | strata | 0.274 | 0.298 | 0.326 | 69.945 | 1.00x |
| nested.json | orjson | 0.294 | 0.296 | 0.308 | 69.945 | 1.01x |
| nested.json | msgspec | 0.412 | 0.428 | 0.438 | 69.945 | 0.70x |
| nested.json | ujson | 1.305 | 1.321 | 1.339 | 69.945 | 0.23x |
| nested.json | json | 2.420 | 2.431 | 2.486 | 69.945 | 0.12x |
| wide_arrays.json | strata | 1.671 | 1.683 | 1.729 | 73.949 | 1.00x |
| wide_arrays.json | orjson | 1.834 | 1.842 | 1.897 | 73.949 | 0.91x |
| wide_arrays.json | msgspec | 2.677 | 2.693 | 2.714 | 73.949 | 0.63x |
| wide_arrays.json | ujson | 8.600 | 8.673 | 9.030 | 73.949 | 0.19x |
| wide_arrays.json | json | 16.376 | 16.517 | 16.826 | 73.949 | 0.10x |
| mixed.json | strata | 0.074 | 0.076 | 0.090 | 74.012 | 1.00x |
| mixed.json | orjson | 0.064 | 0.067 | 0.080 | 74.012 | 1.13x |
| mixed.json | msgspec | 0.084 | 0.087 | 0.104 | 74.012 | 0.88x |
| mixed.json | ujson | 0.306 | 0.311 | 0.373 | 74.012 | 0.25x |
| mixed.json | json | 0.511 | 0.521 | 0.537 | 74.012 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.080 | 12.783 | 14.969 | 74.633 | 1.00x |
| users.json | orjson | 13.823 | 14.363 | 16.654 | 74.633 | 0.89x |
| users.json | msgspec | 14.026 | 14.240 | 16.282 | 74.633 | 0.90x |
| users.json | ujson | 20.100 | 21.722 | 27.834 | 74.633 | 0.59x |
| users.json | json | 23.012 | 23.438 | 26.197 | 74.633 | 0.55x |
| flat.json | strata | 1.009 | 1.031 | 1.054 | 69.945 | 1.00x |
| flat.json | orjson | 1.087 | 1.106 | 1.114 | 69.945 | 0.93x |
| flat.json | msgspec | 1.055 | 1.068 | 1.111 | 69.945 | 0.96x |
| flat.json | ujson | 1.628 | 1.686 | 1.988 | 69.945 | 0.61x |
| flat.json | json | 1.931 | 1.958 | 1.966 | 69.945 | 0.53x |
| nested.json | strata | 0.920 | 0.932 | 0.962 | 69.945 | 1.00x |
| nested.json | orjson | 1.066 | 1.086 | 1.272 | 69.945 | 0.86x |
| nested.json | msgspec | 1.086 | 1.093 | 1.108 | 69.945 | 0.85x |
| nested.json | ujson | 1.565 | 1.605 | 1.635 | 69.945 | 0.58x |
| nested.json | json | 2.153 | 2.172 | 2.236 | 69.945 | 0.43x |
| wide_arrays.json | strata | 4.944 | 4.999 | 5.041 | 74.012 | 1.00x |
| wide_arrays.json | orjson | 5.219 | 5.313 | 5.629 | 74.012 | 0.94x |
| wide_arrays.json | msgspec | 5.760 | 5.901 | 6.012 | 74.012 | 0.85x |
| wide_arrays.json | ujson | 7.468 | 7.649 | 7.725 | 74.012 | 0.65x |
| wide_arrays.json | json | 9.999 | 10.138 | 10.482 | 74.012 | 0.49x |
| mixed.json | strata | 0.244 | 0.249 | 0.266 | 74.012 | 1.00x |
| mixed.json | orjson | 0.278 | 0.290 | 0.305 | 74.012 | 0.86x |
| mixed.json | msgspec | 0.286 | 0.288 | 0.331 | 74.012 | 0.86x |
| mixed.json | ujson | 0.370 | 0.388 | 0.470 | 74.012 | 0.64x |
| mixed.json | json | 0.517 | 0.531 | 0.544 | 74.012 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 13.597 | 14.637 | 15.158 | 69.945 | 1.00x |
| users.ndjson | orjson | 17.699 | 19.124 | 20.161 | 69.945 | 0.77x |
| users.ndjson | msgspec | 17.676 | 18.837 | 19.628 | 69.945 | 0.78x |
| users.ndjson | ujson | 24.400 | 25.520 | 26.566 | 69.945 | 0.57x |
| users.ndjson | json | 31.496 | 32.475 | 33.346 | 69.945 | 0.45x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.517 | 3.561 | 3.624 | 70.695 | 1.00x |
| users.json | orjson | 3.542 | 3.584 | 3.666 | 70.695 | 0.99x |
| users.json | msgspec | 4.469 | 4.500 | 4.548 | 70.695 | 0.79x |
| users.json | ujson | 15.204 | 15.369 | 15.685 | 70.695 | 0.23x |
| users.json | json | 23.438 | 23.492 | 25.825 | 70.695 | 0.15x |
| flat.json | strata | 0.445 | 0.465 | 0.659 | 69.945 | 1.00x |
| flat.json | orjson | 0.504 | 0.520 | 0.555 | 69.945 | 0.89x |
| flat.json | msgspec | 0.607 | 0.621 | 0.677 | 69.945 | 0.75x |
| flat.json | ujson | 1.404 | 1.427 | 1.502 | 69.945 | 0.33x |
| flat.json | json | 2.080 | 2.098 | 2.176 | 69.945 | 0.22x |
| nested.json | strata | 0.403 | 0.425 | 0.663 | 69.945 | 1.00x |
| nested.json | orjson | 0.438 | 0.464 | 0.539 | 69.945 | 0.92x |
| nested.json | msgspec | 0.556 | 0.575 | 0.631 | 69.945 | 0.74x |
| nested.json | ujson | 1.471 | 1.485 | 1.752 | 69.945 | 0.29x |
| nested.json | json | 2.632 | 2.677 | 2.992 | 69.945 | 0.16x |
| wide_arrays.json | strata | 2.137 | 2.158 | 2.309 | 74.012 | 1.00x |
| wide_arrays.json | orjson | 2.321 | 2.349 | 2.362 | 74.012 | 0.92x |
| wide_arrays.json | msgspec | 3.173 | 3.190 | 3.207 | 74.012 | 0.68x |
| wide_arrays.json | ujson | 9.206 | 9.248 | 9.365 | 74.012 | 0.23x |
| wide_arrays.json | json | 17.049 | 17.133 | 17.457 | 74.012 | 0.13x |
| mixed.json | strata | 0.167 | 0.169 | 0.200 | 74.012 | 1.00x |
| mixed.json | orjson | 0.174 | 0.176 | 0.192 | 74.012 | 0.96x |
| mixed.json | msgspec | 0.194 | 0.198 | 0.242 | 74.012 | 0.85x |
| mixed.json | ujson | 0.418 | 0.431 | 0.478 | 74.012 | 0.39x |
| mixed.json | json | 0.630 | 0.647 | 0.686 | 74.012 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.063 | 0.066 | 0.098 | 70.695 | 1.00x |
| users.json $[*].id | jmespath | 0.485 | 0.501 | 0.517 | 70.695 | 0.13x |
| users.json $[*].id | jsonpath-ng | 2.775 | 2.859 | 2.954 | 70.695 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.440 | 0.465 | 0.509 | 70.797 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.094 | 3.153 | 3.214 | 70.797 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.055 | 20.830 | 21.795 | 70.797 | 0.02x |
| users.json $..total | strata | 1.664 | 1.673 | 1.708 | 73.562 | 1.00x |
| users.json $..total | jsonpath-ng | 390.270 | 393.576 | 398.111 | 73.562 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.961 | 2.991 | 4.074 | 70.797 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.321 | 14.553 | 15.856 | 70.797 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 16.739 | 16.872 | 21.872 | 70.797 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.247 | 3.297 | 5.981 | 73.562 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.671 | 17.794 | 18.247 | 73.562 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.081 | 39.927 | 50.580 | 73.562 | 0.08x |
| users.json $..total | strata | 14.696 | 15.822 | 17.383 | 73.562 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 415.138 | 420.355 | 423.481 | 73.562 | 0.04x |

