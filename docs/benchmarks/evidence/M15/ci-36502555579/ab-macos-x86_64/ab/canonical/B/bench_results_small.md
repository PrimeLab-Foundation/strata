# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 296d2ea02694ce7811592deeaa970aaa11c9432f
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch x86_64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/Users/runner/work/_temp/strata-arm/build/pgo/strata.profdata -bundle -undefined (24 recorded commands; see the JSON companion)
- repeats: 60
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.886 | 21.617 | 25.708 | 56.770 | 1.00x |
| users.json | orjson | 28.858 | 35.317 | 43.341 | 56.770 | 0.61x |
| users.json | msgspec | 28.720 | 36.094 | 42.236 | 56.770 | 0.60x |
| users.json | ujson | 41.881 | 49.206 | 58.957 | 56.770 | 0.44x |
| users.json | pysimdjson | 178.969 | 195.204 | 217.913 | 56.770 | 0.11x |
| users.json | json | 46.447 | 56.553 | 64.044 | 56.770 | 0.38x |
| flat.json | strata | 1.165 | 1.268 | 1.631 | 70.891 | 1.00x |
| flat.json | orjson | 1.290 | 1.416 | 1.511 | 70.891 | 0.90x |
| flat.json | msgspec | 1.457 | 1.593 | 1.689 | 70.891 | 0.80x |
| flat.json | ujson | 2.598 | 2.800 | 3.052 | 70.891 | 0.45x |
| flat.json | pysimdjson | 13.963 | 14.811 | 15.341 | 70.891 | 0.09x |
| flat.json | json | 2.980 | 3.178 | 3.360 | 70.891 | 0.40x |
| nested.json | strata | 1.368 | 1.571 | 1.747 | 58.457 | 1.00x |
| nested.json | orjson | 1.584 | 1.787 | 2.212 | 58.457 | 0.88x |
| nested.json | msgspec | 1.729 | 1.975 | 2.138 | 58.457 | 0.80x |
| nested.json | ujson | 2.865 | 3.198 | 3.405 | 58.457 | 0.49x |
| nested.json | pysimdjson | 12.784 | 13.945 | 14.725 | 58.457 | 0.11x |
| nested.json | json | 3.685 | 4.092 | 4.510 | 58.457 | 0.38x |
| wide_arrays.json | strata | 7.240 | 7.709 | 8.576 | 68.465 | 1.00x |
| wide_arrays.json | orjson | 9.263 | 10.108 | 11.311 | 68.465 | 0.76x |
| wide_arrays.json | msgspec | 9.926 | 10.833 | 12.085 | 68.465 | 0.71x |
| wide_arrays.json | ujson | 12.627 | 13.494 | 14.854 | 68.465 | 0.57x |
| wide_arrays.json | pysimdjson | 76.116 | 80.168 | 84.927 | 68.465 | 0.10x |
| wide_arrays.json | json | 16.467 | 17.579 | 19.175 | 68.465 | 0.44x |
| mixed.json | strata | 0.342 | 0.359 | 0.408 | 68.477 | 1.00x |
| mixed.json | orjson | 0.419 | 0.442 | 0.488 | 68.477 | 0.81x |
| mixed.json | msgspec | 0.445 | 0.471 | 0.522 | 68.477 | 0.76x |
| mixed.json | ujson | 0.608 | 0.638 | 0.695 | 68.477 | 0.56x |
| mixed.json | pysimdjson | 3.154 | 3.218 | 3.366 | 68.477 | 0.11x |
| mixed.json | json | 0.860 | 0.890 | 0.939 | 68.477 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.531 | 3.265 | 4.479 | 52.457 | 1.00x |
| users.json | orjson | 3.526 | 4.429 | 6.435 | 52.457 | 0.74x |
| users.json | msgspec | 6.167 | 7.089 | 9.578 | 52.457 | 0.46x |
| users.json | ujson | 25.094 | 27.911 | 35.071 | 52.457 | 0.12x |
| users.json | json | 44.351 | 49.031 | 60.047 | 52.457 | 0.07x |
| flat.json | strata | 0.300 | 0.353 | 0.491 | 59.367 | 1.00x |
| flat.json | orjson | 0.373 | 0.434 | 0.561 | 59.367 | 0.81x |
| flat.json | msgspec | 0.519 | 0.574 | 0.688 | 59.367 | 0.61x |
| flat.json | ujson | 2.137 | 2.287 | 2.857 | 59.367 | 0.15x |
| flat.json | json | 3.508 | 3.699 | 4.592 | 59.367 | 0.10x |
| nested.json | strata | 0.222 | 0.248 | 0.282 | 56.938 | 1.00x |
| nested.json | orjson | 0.337 | 0.373 | 0.474 | 56.938 | 0.66x |
| nested.json | msgspec | 0.536 | 0.584 | 0.668 | 56.938 | 0.42x |
| nested.json | ujson | 2.227 | 2.319 | 2.500 | 56.938 | 0.11x |
| nested.json | json | 4.443 | 4.691 | 5.075 | 56.938 | 0.05x |
| wide_arrays.json | strata | 1.866 | 2.175 | 2.324 | 71.582 | 1.00x |
| wide_arrays.json | orjson | 2.597 | 2.914 | 3.040 | 71.582 | 0.75x |
| wide_arrays.json | msgspec | 3.390 | 3.696 | 4.195 | 71.582 | 0.59x |
| wide_arrays.json | ujson | 10.447 | 11.032 | 11.914 | 71.582 | 0.20x |
| wide_arrays.json | json | 33.929 | 35.394 | 37.264 | 71.582 | 0.06x |
| mixed.json | strata | 0.064 | 0.079 | 0.101 | 67.281 | 1.00x |
| mixed.json | orjson | 0.077 | 0.096 | 0.140 | 67.281 | 0.82x |
| mixed.json | msgspec | 0.110 | 0.130 | 0.163 | 67.281 | 0.61x |
| mixed.json | ujson | 0.448 | 0.467 | 0.519 | 67.281 | 0.17x |
| mixed.json | json | 0.937 | 0.970 | 1.045 | 67.281 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.210 | 20.319 | 25.442 | 66.227 | 1.00x |
| users.json | orjson | 26.723 | 31.967 | 39.610 | 66.227 | 0.64x |
| users.json | msgspec | 26.756 | 32.253 | 40.620 | 66.227 | 0.63x |
| users.json | ujson | 38.555 | 46.350 | 56.350 | 66.227 | 0.44x |
| users.json | json | 43.340 | 50.872 | 64.541 | 66.227 | 0.40x |
| flat.json | strata | 1.349 | 1.437 | 1.919 | 59.652 | 1.00x |
| flat.json | orjson | 1.495 | 1.647 | 2.087 | 59.652 | 0.87x |
| flat.json | msgspec | 1.716 | 1.817 | 2.627 | 59.652 | 0.79x |
| flat.json | ujson | 2.939 | 3.087 | 3.660 | 59.652 | 0.47x |
| flat.json | json | 3.267 | 3.433 | 4.228 | 59.652 | 0.42x |
| nested.json | strata | 1.486 | 1.659 | 1.789 | 56.973 | 1.00x |
| nested.json | orjson | 1.698 | 1.910 | 2.178 | 56.973 | 0.87x |
| nested.json | msgspec | 1.849 | 2.104 | 2.340 | 56.973 | 0.79x |
| nested.json | ujson | 2.989 | 3.344 | 3.734 | 56.973 | 0.50x |
| nested.json | json | 3.759 | 4.158 | 4.476 | 56.973 | 0.40x |
| wide_arrays.json | strata | 7.283 | 7.732 | 8.330 | 71.582 | 1.00x |
| wide_arrays.json | orjson | 9.223 | 9.901 | 11.319 | 71.582 | 0.78x |
| wide_arrays.json | msgspec | 10.025 | 10.760 | 11.992 | 71.582 | 0.72x |
| wide_arrays.json | ujson | 12.909 | 13.819 | 15.089 | 71.582 | 0.56x |
| wide_arrays.json | json | 16.325 | 17.687 | 19.056 | 71.582 | 0.44x |
| mixed.json | strata | 0.423 | 0.467 | 0.521 | 67.281 | 1.00x |
| mixed.json | orjson | 0.541 | 0.606 | 0.633 | 67.281 | 0.77x |
| mixed.json | msgspec | 0.586 | 0.643 | 0.687 | 67.281 | 0.73x |
| mixed.json | ujson | 0.753 | 0.823 | 0.878 | 67.281 | 0.57x |
| mixed.json | json | 0.981 | 1.050 | 1.106 | 67.281 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 18.490 | 19.269 | 20.039 | 69.777 | 1.00x |
| users.ndjson | orjson | 26.397 | 28.206 | 29.559 | 69.777 | 0.68x |
| users.ndjson | msgspec | 27.122 | 28.789 | 30.713 | 69.777 | 0.67x |
| users.ndjson | ujson | 38.873 | 41.414 | 44.883 | 69.777 | 0.47x |
| users.ndjson | json | 47.544 | 50.908 | 53.835 | 69.777 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.615 | 4.459 | 6.455 | 60.957 | 1.00x |
| users.json | orjson | 4.495 | 5.609 | 7.152 | 60.957 | 0.80x |
| users.json | msgspec | 6.924 | 8.400 | 10.808 | 60.957 | 0.53x |
| users.json | ujson | 25.756 | 31.533 | 38.553 | 60.957 | 0.14x |
| users.json | json | 43.946 | 53.762 | 69.795 | 60.957 | 0.08x |
| flat.json | strata | 0.635 | 0.706 | 0.822 | 58.387 | 1.00x |
| flat.json | orjson | 0.765 | 0.845 | 0.993 | 58.387 | 0.84x |
| flat.json | msgspec | 0.890 | 0.975 | 1.156 | 58.387 | 0.72x |
| flat.json | ujson | 2.649 | 2.795 | 3.109 | 58.387 | 0.25x |
| flat.json | json | 4.077 | 4.253 | 4.935 | 58.387 | 0.17x |
| nested.json | strata | 0.558 | 0.608 | 0.697 | 56.973 | 1.00x |
| nested.json | orjson | 0.702 | 0.790 | 0.900 | 56.973 | 0.77x |
| nested.json | msgspec | 0.895 | 1.007 | 1.130 | 56.973 | 0.60x |
| nested.json | ujson | 2.725 | 2.905 | 3.278 | 56.973 | 0.21x |
| nested.json | json | 5.076 | 5.384 | 6.104 | 56.973 | 0.11x |
| wide_arrays.json | strata | 2.494 | 2.807 | 3.234 | 71.582 | 1.00x |
| wide_arrays.json | orjson | 3.374 | 3.622 | 4.232 | 71.582 | 0.77x |
| wide_arrays.json | msgspec | 4.082 | 4.565 | 5.103 | 71.582 | 0.61x |
| wide_arrays.json | ujson | 11.158 | 11.827 | 12.741 | 71.582 | 0.24x |
| wide_arrays.json | json | 34.209 | 36.116 | 37.905 | 71.582 | 0.08x |
| mixed.json | strata | 0.307 | 0.363 | 0.419 | 67.281 | 1.00x |
| mixed.json | orjson | 0.332 | 0.411 | 0.494 | 67.281 | 0.88x |
| mixed.json | msgspec | 0.350 | 0.461 | 0.531 | 67.281 | 0.79x |
| mixed.json | ujson | 0.719 | 0.819 | 0.971 | 67.281 | 0.44x |
| mixed.json | json | 1.209 | 1.308 | 1.392 | 67.281 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.139 | 0.176 | 0.214 | 58.562 | 1.00x |
| users.json $[*].id | jmespath | 0.950 | 1.013 | 1.141 | 58.562 | 0.17x |
| users.json $[*].id | jsonpath-ng | 5.096 | 5.433 | 6.005 | 58.562 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.853 | 1.200 | 1.534 | 63.949 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.607 | 6.516 | 7.323 | 63.949 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 32.534 | 37.022 | 41.043 | 63.949 | 0.03x |
| users.json $..total | strata | 3.274 | 3.686 | 4.618 | 63.973 | 1.00x |
| users.json $..total | jsonpath-ng | 681.723 | 711.606 | 782.598 | 63.973 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.676 | 4.029 | 5.313 | 63.859 | 1.00x |
| users.json $[*].id | orjson+jmespath | 26.112 | 30.509 | 41.412 | 63.859 | 0.13x |
| users.json $[*].id | orjson+jsonpath-ng | 31.303 | 35.236 | 47.218 | 63.859 | 0.11x |
| users.json $[*].orders[*].total | strata | 4.031 | 4.245 | 4.574 | 63.953 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 30.978 | 34.643 | 37.900 | 63.953 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 64.108 | 73.273 | 80.442 | 63.953 | 0.06x |
| users.json $..total | strata | 22.286 | 23.608 | 24.831 | 63.973 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 714.911 | 749.120 | 777.758 | 63.973 | 0.03x |

