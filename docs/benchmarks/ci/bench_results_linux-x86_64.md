# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ab20434
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
| users.json | strata | 10.352 | 12.127 | 20.387 | 57.480 | 1.00x |
| users.json | orjson | 13.702 | 14.698 | 17.688 | 57.480 | 0.83x |
| users.json | msgspec | 13.689 | 13.960 | 16.792 | 57.480 | 0.87x |
| users.json | ujson | 18.391 | 19.711 | 28.047 | 57.480 | 0.62x |
| users.json | pysimdjson | 18.910 | 20.703 | 26.491 | 57.480 | 0.59x |
| users.json | json | 20.896 | 22.040 | 40.893 | 57.480 | 0.55x |
| flat.json | strata | 0.886 | 0.917 | 1.001 | 68.047 | 1.00x |
| flat.json | orjson | 1.061 | 1.067 | 1.093 | 68.047 | 0.86x |
| flat.json | msgspec | 1.039 | 1.050 | 1.065 | 68.047 | 0.87x |
| flat.json | ujson | 1.471 | 1.513 | 1.584 | 68.047 | 0.61x |
| flat.json | pysimdjson | 1.614 | 1.633 | 1.668 | 68.047 | 0.56x |
| flat.json | json | 1.686 | 1.725 | 1.841 | 68.047 | 0.53x |
| nested.json | strata | 0.796 | 0.809 | 0.839 | 68.047 | 1.00x |
| nested.json | orjson | 0.992 | 1.005 | 1.042 | 68.047 | 0.80x |
| nested.json | msgspec | 0.954 | 0.982 | 1.040 | 68.047 | 0.82x |
| nested.json | ujson | 1.433 | 1.470 | 1.495 | 68.047 | 0.55x |
| nested.json | pysimdjson | 1.361 | 1.412 | 1.449 | 68.047 | 0.57x |
| nested.json | json | 1.813 | 1.824 | 1.865 | 68.047 | 0.44x |
| wide_arrays.json | strata | 5.039 | 5.101 | 5.202 | 72.328 | 1.00x |
| wide_arrays.json | orjson | 5.584 | 5.760 | 5.852 | 72.328 | 0.89x |
| wide_arrays.json | msgspec | 6.188 | 6.244 | 6.469 | 72.328 | 0.82x |
| wide_arrays.json | ujson | 7.821 | 8.042 | 8.523 | 72.328 | 0.63x |
| wide_arrays.json | pysimdjson | 6.415 | 6.682 | 6.995 | 72.328 | 0.76x |
| wide_arrays.json | json | 9.872 | 9.904 | 10.254 | 72.328 | 0.52x |
| mixed.json | strata | 0.198 | 0.203 | 0.325 | 72.391 | 1.00x |
| mixed.json | orjson | 0.236 | 0.242 | 0.253 | 72.391 | 0.84x |
| mixed.json | msgspec | 0.245 | 0.251 | 0.271 | 72.391 | 0.81x |
| mixed.json | ujson | 0.307 | 0.321 | 0.470 | 72.391 | 0.63x |
| mixed.json | pysimdjson | 0.300 | 0.309 | 0.500 | 72.391 | 0.66x |
| mixed.json | json | 0.441 | 0.461 | 0.481 | 72.391 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.376 | 2.433 | 2.570 | 55.992 | 1.00x |
| users.json | orjson | 3.073 | 3.117 | 3.151 | 55.992 | 0.78x |
| users.json | msgspec | 4.170 | 4.207 | 4.244 | 55.992 | 0.58x |
| users.json | ujson | 14.491 | 14.643 | 15.088 | 55.992 | 0.17x |
| users.json | json | 21.534 | 22.003 | 23.816 | 55.992 | 0.11x |
| flat.json | strata | 0.285 | 0.295 | 0.312 | 68.047 | 1.00x |
| flat.json | orjson | 0.362 | 0.369 | 0.384 | 68.047 | 0.80x |
| flat.json | msgspec | 0.468 | 0.483 | 0.510 | 68.047 | 0.61x |
| flat.json | ujson | 1.260 | 1.279 | 1.312 | 68.047 | 0.23x |
| flat.json | json | 1.868 | 1.882 | 1.940 | 68.047 | 0.16x |
| nested.json | strata | 0.232 | 0.243 | 0.254 | 68.047 | 1.00x |
| nested.json | orjson | 0.303 | 0.315 | 0.323 | 68.047 | 0.77x |
| nested.json | msgspec | 0.414 | 0.422 | 0.439 | 68.047 | 0.58x |
| nested.json | ujson | 1.348 | 1.359 | 1.380 | 68.047 | 0.18x |
| nested.json | json | 2.347 | 2.392 | 2.576 | 68.047 | 0.10x |
| wide_arrays.json | strata | 1.706 | 1.726 | 1.762 | 72.328 | 1.00x |
| wide_arrays.json | orjson | 1.868 | 1.906 | 1.959 | 72.328 | 0.91x |
| wide_arrays.json | msgspec | 3.079 | 3.109 | 3.155 | 72.328 | 0.56x |
| wide_arrays.json | ujson | 8.573 | 8.647 | 8.720 | 72.328 | 0.20x |
| wide_arrays.json | json | 16.655 | 16.845 | 18.728 | 72.328 | 0.10x |
| mixed.json | strata | 0.063 | 0.066 | 0.079 | 72.391 | 1.00x |
| mixed.json | orjson | 0.069 | 0.072 | 0.073 | 72.391 | 0.92x |
| mixed.json | msgspec | 0.087 | 0.089 | 0.096 | 72.391 | 0.74x |
| mixed.json | ujson | 0.301 | 0.313 | 0.330 | 72.391 | 0.21x |
| mixed.json | json | 0.508 | 0.515 | 0.524 | 72.391 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.738 | 12.815 | 13.540 | 72.629 | 1.00x |
| users.json | orjson | 14.751 | 15.440 | 15.840 | 72.629 | 0.83x |
| users.json | msgspec | 14.698 | 14.990 | 15.909 | 72.629 | 0.85x |
| users.json | ujson | 20.493 | 21.989 | 24.330 | 72.629 | 0.58x |
| users.json | json | 21.948 | 23.003 | 23.694 | 72.629 | 0.56x |
| flat.json | strata | 0.982 | 1.014 | 1.067 | 68.047 | 1.00x |
| flat.json | orjson | 1.157 | 1.186 | 1.292 | 68.047 | 0.85x |
| flat.json | msgspec | 1.131 | 1.155 | 1.212 | 68.047 | 0.88x |
| flat.json | ujson | 1.647 | 1.764 | 1.872 | 68.047 | 0.57x |
| flat.json | json | 1.772 | 1.806 | 1.884 | 68.047 | 0.56x |
| nested.json | strata | 0.836 | 0.860 | 0.896 | 68.047 | 1.00x |
| nested.json | orjson | 1.059 | 1.078 | 1.133 | 68.047 | 0.80x |
| nested.json | msgspec | 1.039 | 1.080 | 1.112 | 68.047 | 0.80x |
| nested.json | ujson | 1.521 | 1.554 | 1.635 | 68.047 | 0.55x |
| nested.json | json | 1.901 | 1.955 | 2.049 | 68.047 | 0.44x |
| wide_arrays.json | strata | 5.211 | 5.363 | 5.466 | 72.391 | 1.00x |
| wide_arrays.json | orjson | 5.796 | 5.953 | 6.220 | 72.391 | 0.90x |
| wide_arrays.json | msgspec | 6.337 | 6.506 | 6.900 | 72.391 | 0.82x |
| wide_arrays.json | ujson | 8.280 | 8.344 | 8.648 | 72.391 | 0.64x |
| wide_arrays.json | json | 10.084 | 10.293 | 10.651 | 72.391 | 0.52x |
| mixed.json | strata | 0.222 | 0.227 | 0.238 | 72.391 | 1.00x |
| mixed.json | orjson | 0.287 | 0.290 | 0.307 | 72.391 | 0.78x |
| mixed.json | msgspec | 0.284 | 0.294 | 0.299 | 72.391 | 0.77x |
| mixed.json | ujson | 0.356 | 0.380 | 0.395 | 72.391 | 0.60x |
| mixed.json | json | 0.492 | 0.500 | 0.510 | 72.391 | 0.45x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 10.911 | 11.066 | 11.571 | 68.047 | 1.00x |
| users.ndjson | orjson | 17.716 | 17.984 | 18.613 | 68.047 | 0.62x |
| users.ndjson | msgspec | 17.779 | 18.110 | 18.855 | 68.047 | 0.61x |
| users.ndjson | ujson | 22.315 | 22.695 | 23.326 | 68.047 | 0.49x |
| users.ndjson | json | 28.315 | 28.873 | 29.562 | 68.047 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.967 | 3.041 | 3.190 | 68.691 | 1.00x |
| users.json | orjson | 3.788 | 3.831 | 4.002 | 68.691 | 0.79x |
| users.json | msgspec | 4.782 | 4.852 | 5.060 | 68.691 | 0.63x |
| users.json | ujson | 15.384 | 15.534 | 145.448 | 68.691 | 0.20x |
| users.json | json | 22.441 | 22.911 | 188.441 | 68.691 | 0.13x |
| flat.json | strata | 0.454 | 0.463 | 0.480 | 68.047 | 1.00x |
| flat.json | orjson | 0.525 | 0.547 | 0.570 | 68.047 | 0.85x |
| flat.json | msgspec | 0.653 | 0.667 | 0.681 | 68.047 | 0.69x |
| flat.json | ujson | 1.450 | 1.466 | 1.495 | 68.047 | 0.32x |
| flat.json | json | 2.074 | 2.086 | 2.109 | 68.047 | 0.22x |
| nested.json | strata | 0.355 | 0.381 | 0.397 | 68.047 | 1.00x |
| nested.json | orjson | 0.455 | 0.478 | 0.508 | 68.047 | 0.80x |
| nested.json | msgspec | 0.567 | 0.594 | 0.628 | 68.047 | 0.64x |
| nested.json | ujson | 1.526 | 1.544 | 1.595 | 68.047 | 0.25x |
| nested.json | json | 2.680 | 2.698 | 2.762 | 68.047 | 0.14x |
| wide_arrays.json | strata | 2.132 | 2.173 | 2.235 | 72.391 | 1.00x |
| wide_arrays.json | orjson | 2.339 | 2.368 | 2.414 | 72.391 | 0.92x |
| wide_arrays.json | msgspec | 3.540 | 3.598 | 4.743 | 72.391 | 0.60x |
| wide_arrays.json | ujson | 9.092 | 9.162 | 9.461 | 72.391 | 0.24x |
| wide_arrays.json | json | 17.144 | 17.255 | 18.033 | 72.391 | 0.13x |
| mixed.json | strata | 0.160 | 0.168 | 0.192 | 72.391 | 1.00x |
| mixed.json | orjson | 0.182 | 0.188 | 0.198 | 72.391 | 0.89x |
| mixed.json | msgspec | 0.196 | 0.209 | 0.221 | 72.391 | 0.80x |
| mixed.json | ujson | 0.423 | 0.431 | 0.459 | 72.391 | 0.39x |
| mixed.json | json | 0.637 | 0.648 | 0.688 | 72.391 | 0.26x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.081 | 0.086 | 0.091 | 68.691 | 1.00x |
| users.json $[*].id | jmespath | 0.479 | 0.493 | 0.521 | 68.691 | 0.17x |
| users.json $[*].id | jsonpath-ng | 2.823 | 3.125 | 3.567 | 68.691 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.430 | 0.450 | 0.465 | 68.754 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.965 | 3.026 | 3.285 | 68.754 | 0.15x |
| users.json $[*].orders[*].total | jsonpath-ng | 20.052 | 20.570 | 23.272 | 68.754 | 0.02x |
| users.json $..total | strata | 1.802 | 1.900 | 1.916 | 71.645 | 1.00x |
| users.json $..total | jsonpath-ng | 382.011 | 385.457 | 388.493 | 71.645 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.235 | 3.252 | 3.305 | 68.754 | 1.00x |
| users.json $[*].id | orjson+jmespath | 15.535 | 15.856 | 16.467 | 68.754 | 0.21x |
| users.json $[*].id | orjson+jsonpath-ng | 17.959 | 18.211 | 19.161 | 68.754 | 0.18x |
| users.json $[*].orders[*].total | strata | 3.490 | 3.529 | 3.609 | 71.645 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 18.455 | 18.609 | 18.945 | 71.645 | 0.19x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 38.796 | 39.511 | 42.081 | 71.645 | 0.09x |
| users.json $..total | strata | 14.404 | 15.635 | 16.238 | 70.867 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 405.703 | 412.416 | 413.195 | 70.867 | 0.04x |

