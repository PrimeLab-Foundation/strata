# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 37a96fb
- python: 3.12.14
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
| users.json | strata | 10.319 | 11.253 | 24.430 | 59.941 | 1.00x |
| users.json | orjson | 13.412 | 14.284 | 30.924 | 59.941 | 0.79x |
| users.json | msgspec | 13.264 | 13.553 | 29.685 | 59.941 | 0.83x |
| users.json | ujson | 19.094 | 19.871 | 44.244 | 59.941 | 0.57x |
| users.json | pysimdjson | 19.877 | 21.062 | 29.142 | 59.941 | 0.53x |
| users.json | json | 23.099 | 23.566 | 29.398 | 59.941 | 0.48x |
| flat.json | strata | 0.885 | 0.899 | 0.922 | 65.914 | 1.00x |
| flat.json | orjson | 0.999 | 1.011 | 1.039 | 65.914 | 0.89x |
| flat.json | msgspec | 1.030 | 1.057 | 1.072 | 65.914 | 0.85x |
| flat.json | ujson | 1.537 | 1.576 | 1.671 | 65.914 | 0.57x |
| flat.json | pysimdjson | 1.569 | 1.595 | 1.691 | 65.914 | 0.56x |
| flat.json | json | 1.956 | 1.974 | 2.033 | 65.914 | 0.46x |
| nested.json | strata | 0.816 | 0.822 | 0.855 | 65.914 | 1.00x |
| nested.json | orjson | 1.010 | 1.025 | 1.043 | 65.914 | 0.80x |
| nested.json | msgspec | 1.047 | 1.071 | 1.104 | 65.914 | 0.77x |
| nested.json | ujson | 1.503 | 1.558 | 1.685 | 65.914 | 0.53x |
| nested.json | pysimdjson | 1.424 | 1.442 | 1.495 | 65.914 | 0.57x |
| nested.json | json | 2.085 | 2.103 | 2.286 | 65.914 | 0.39x |
| wide_arrays.json | strata | 4.689 | 4.783 | 5.875 | 72.195 | 1.00x |
| wide_arrays.json | orjson | 5.246 | 5.513 | 6.603 | 72.195 | 0.87x |
| wide_arrays.json | msgspec | 5.704 | 5.966 | 6.537 | 72.195 | 0.80x |
| wide_arrays.json | ujson | 7.393 | 7.715 | 10.430 | 72.195 | 0.62x |
| wide_arrays.json | pysimdjson | 6.301 | 6.917 | 7.233 | 72.195 | 0.69x |
| wide_arrays.json | json | 10.015 | 10.107 | 10.839 | 72.195 | 0.47x |
| mixed.json | strata | 0.197 | 0.201 | 0.214 | 72.258 | 1.00x |
| mixed.json | orjson | 0.231 | 0.234 | 0.247 | 72.258 | 0.86x |
| mixed.json | msgspec | 0.243 | 0.250 | 0.270 | 72.258 | 0.80x |
| mixed.json | ujson | 0.304 | 0.314 | 0.343 | 72.258 | 0.64x |
| mixed.json | pysimdjson | 0.297 | 0.305 | 0.326 | 72.258 | 0.66x |
| mixed.json | json | 0.485 | 0.491 | 0.506 | 72.258 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.624 | 2.682 | 2.732 | 59.031 | 1.00x |
| users.json | orjson | 2.924 | 2.943 | 3.138 | 59.031 | 0.91x |
| users.json | msgspec | 3.846 | 3.902 | 3.942 | 59.031 | 0.69x |
| users.json | ujson | 14.635 | 14.953 | 15.359 | 59.031 | 0.18x |
| users.json | json | 21.878 | 22.179 | 22.702 | 59.031 | 0.12x |
| flat.json | strata | 0.263 | 0.269 | 0.280 | 65.914 | 1.00x |
| flat.json | orjson | 0.329 | 0.334 | 0.349 | 65.914 | 0.80x |
| flat.json | msgspec | 0.426 | 0.434 | 0.452 | 65.914 | 0.62x |
| flat.json | ujson | 1.291 | 1.306 | 1.418 | 65.914 | 0.21x |
| flat.json | json | 1.842 | 1.878 | 2.003 | 65.914 | 0.14x |
| nested.json | strata | 0.244 | 0.261 | 0.296 | 65.914 | 1.00x |
| nested.json | orjson | 0.290 | 0.292 | 0.305 | 65.914 | 0.89x |
| nested.json | msgspec | 0.404 | 0.412 | 0.424 | 65.914 | 0.63x |
| nested.json | ujson | 1.394 | 1.406 | 1.421 | 65.914 | 0.19x |
| nested.json | json | 2.448 | 2.465 | 2.485 | 65.914 | 0.11x |
| wide_arrays.json | strata | 1.576 | 1.586 | 1.636 | 72.195 | 1.00x |
| wide_arrays.json | orjson | 1.802 | 1.833 | 1.907 | 72.195 | 0.87x |
| wide_arrays.json | msgspec | 2.674 | 2.696 | 2.752 | 72.195 | 0.59x |
| wide_arrays.json | ujson | 8.512 | 8.590 | 8.712 | 72.195 | 0.18x |
| wide_arrays.json | json | 16.441 | 16.601 | 16.819 | 72.195 | 0.10x |
| mixed.json | strata | 0.065 | 0.067 | 0.082 | 72.258 | 1.00x |
| mixed.json | orjson | 0.065 | 0.067 | 0.081 | 72.258 | 1.00x |
| mixed.json | msgspec | 0.084 | 0.088 | 0.116 | 72.258 | 0.76x |
| mixed.json | ujson | 0.306 | 0.317 | 0.368 | 72.258 | 0.21x |
| mixed.json | json | 0.519 | 0.533 | 0.549 | 72.258 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.021 | 11.816 | 12.683 | 71.301 | 1.00x |
| users.json | orjson | 13.823 | 14.423 | 15.406 | 71.301 | 0.82x |
| users.json | msgspec | 13.676 | 14.211 | 15.225 | 71.301 | 0.83x |
| users.json | ujson | 18.785 | 20.439 | 21.742 | 71.301 | 0.58x |
| users.json | json | 23.262 | 23.686 | 25.672 | 71.301 | 0.50x |
| flat.json | strata | 0.927 | 0.936 | 1.035 | 65.914 | 1.00x |
| flat.json | orjson | 1.064 | 1.085 | 1.186 | 65.914 | 0.86x |
| flat.json | msgspec | 1.098 | 1.117 | 1.248 | 65.914 | 0.84x |
| flat.json | ujson | 1.605 | 1.670 | 1.824 | 65.914 | 0.56x |
| flat.json | json | 2.026 | 2.037 | 2.095 | 65.914 | 0.46x |
| nested.json | strata | 0.852 | 0.884 | 0.926 | 65.914 | 1.00x |
| nested.json | orjson | 1.077 | 1.098 | 1.123 | 65.914 | 0.81x |
| nested.json | msgspec | 1.091 | 1.118 | 1.169 | 65.914 | 0.79x |
| nested.json | ujson | 1.587 | 1.617 | 1.641 | 65.914 | 0.55x |
| nested.json | json | 2.146 | 2.193 | 2.227 | 65.914 | 0.40x |
| wide_arrays.json | strata | 4.783 | 4.939 | 5.083 | 72.258 | 1.00x |
| wide_arrays.json | orjson | 5.450 | 5.570 | 5.754 | 72.258 | 0.89x |
| wide_arrays.json | msgspec | 5.885 | 6.307 | 6.393 | 72.258 | 0.78x |
| wide_arrays.json | ujson | 7.569 | 7.757 | 8.102 | 72.258 | 0.64x |
| wide_arrays.json | json | 9.975 | 10.270 | 10.736 | 72.258 | 0.48x |
| mixed.json | strata | 0.215 | 0.226 | 0.234 | 72.258 | 1.00x |
| mixed.json | orjson | 0.276 | 0.279 | 0.304 | 72.258 | 0.81x |
| mixed.json | msgspec | 0.286 | 0.295 | 0.309 | 72.258 | 0.76x |
| mixed.json | ujson | 0.361 | 0.384 | 0.411 | 72.258 | 0.59x |
| mixed.json | json | 0.526 | 0.539 | 0.556 | 72.258 | 0.42x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.439 | 10.929 | 12.167 | 65.910 | 1.00x |
| users.ndjson | orjson | 16.877 | 17.220 | 19.420 | 65.910 | 0.63x |
| users.ndjson | msgspec | 16.619 | 17.135 | 18.084 | 65.910 | 0.64x |
| users.ndjson | ujson | 21.869 | 22.492 | 23.422 | 65.910 | 0.49x |
| users.ndjson | json | 29.349 | 29.684 | 33.563 | 65.910 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.143 | 3.322 | 3.575 | 69.332 | 1.00x |
| users.json | orjson | 3.591 | 3.662 | 3.870 | 69.332 | 0.91x |
| users.json | msgspec | 4.479 | 4.569 | 4.819 | 69.332 | 0.73x |
| users.json | ujson | 15.526 | 15.756 | 15.940 | 69.332 | 0.21x |
| users.json | json | 22.743 | 23.013 | 23.655 | 69.332 | 0.14x |
| flat.json | strata | 0.415 | 0.444 | 0.482 | 65.914 | 1.00x |
| flat.json | orjson | 0.501 | 0.522 | 0.586 | 65.914 | 0.85x |
| flat.json | msgspec | 0.602 | 0.631 | 0.667 | 65.914 | 0.70x |
| flat.json | ujson | 1.485 | 1.507 | 1.544 | 65.914 | 0.29x |
| flat.json | json | 2.091 | 2.116 | 2.182 | 65.914 | 0.21x |
| nested.json | strata | 0.371 | 0.447 | 0.481 | 65.914 | 1.00x |
| nested.json | orjson | 0.445 | 0.491 | 0.558 | 65.914 | 0.91x |
| nested.json | msgspec | 0.558 | 0.601 | 0.634 | 65.914 | 0.74x |
| nested.json | ujson | 1.550 | 1.600 | 1.674 | 65.914 | 0.28x |
| nested.json | json | 2.606 | 2.687 | 2.726 | 65.914 | 0.17x |
| wide_arrays.json | strata | 2.017 | 2.066 | 2.186 | 72.258 | 1.00x |
| wide_arrays.json | orjson | 2.292 | 2.333 | 2.395 | 72.258 | 0.89x |
| wide_arrays.json | msgspec | 3.133 | 3.204 | 3.255 | 72.258 | 0.64x |
| wide_arrays.json | ujson | 9.128 | 9.249 | 9.435 | 72.258 | 0.22x |
| wide_arrays.json | json | 17.087 | 17.153 | 17.382 | 72.258 | 0.12x |
| mixed.json | strata | 0.157 | 0.166 | 0.190 | 72.258 | 1.00x |
| mixed.json | orjson | 0.172 | 0.183 | 0.198 | 72.258 | 0.91x |
| mixed.json | msgspec | 0.188 | 0.198 | 0.213 | 72.258 | 0.84x |
| mixed.json | ujson | 0.425 | 0.436 | 0.445 | 72.258 | 0.38x |
| mixed.json | json | 0.632 | 0.668 | 0.688 | 72.258 | 0.25x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.064 | 0.067 | 0.080 | 69.332 | 1.00x |
| users.json $[*].id | jmespath | 0.508 | 0.522 | 0.583 | 69.332 | 0.13x |
| users.json $[*].id | jsonpath-ng | 2.845 | 2.903 | 3.103 | 69.332 | 0.02x |
| users.json $[*].orders[*].total | strata | 0.435 | 0.449 | 0.495 | 69.359 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.130 | 3.160 | 3.202 | 69.359 | 0.14x |
| users.json $[*].orders[*].total | jsonpath-ng | 19.313 | 19.820 | 20.761 | 69.359 | 0.02x |
| users.json $..total | strata | 1.646 | 1.690 | 1.969 | 70.367 | 1.00x |
| users.json $..total | jsonpath-ng | 389.705 | 393.144 | 396.593 | 70.367 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.282 | 3.329 | 3.446 | 69.359 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.081 | 14.417 | 15.013 | 69.359 | 0.23x |
| users.json $[*].id | orjson+jsonpath-ng | 16.522 | 16.942 | 17.414 | 69.359 | 0.20x |
| users.json $[*].orders[*].total | strata | 3.570 | 3.633 | 4.053 | 69.586 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 16.982 | 17.925 | 19.172 | 69.586 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 36.770 | 38.270 | 43.257 | 69.586 | 0.09x |
| users.json $..total | strata | 13.202 | 13.896 | 17.650 | 69.727 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 415.956 | 417.572 | 426.894 | 69.727 | 0.03x |

