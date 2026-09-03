# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e8ac860
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: Intel64 Family 6 Model 173 Stepping 1, GenuineIntel
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.355 | 6.994 | 9.873 | 45.590 | 1.00x |
| users.json | orjson | 11.640 | 11.959 | 14.812 | 45.590 | 0.58x |
| users.json | msgspec | 10.479 | 10.914 | 15.653 | 45.590 | 0.64x |
| users.json | ujson | 15.699 | 17.558 | 23.157 | 45.590 | 0.40x |
| users.json | json | 16.892 | 17.803 | 24.566 | 45.590 | 0.39x |
| flat.json | strata | 0.857 | 0.941 | 0.983 | 49.777 | 1.00x |
| flat.json | orjson | 0.963 | 0.995 | 1.139 | 49.777 | 0.95x |
| flat.json | msgspec | 0.754 | 0.832 | 0.926 | 49.777 | 1.13x |
| flat.json | ujson | 1.146 | 1.252 | 1.298 | 49.777 | 0.75x |
| flat.json | json | 1.302 | 1.325 | 1.364 | 49.777 | 0.71x |
| nested.json | strata | 0.505 | 0.537 | 0.599 | 49.785 | 1.00x |
| nested.json | orjson | 0.748 | 0.794 | 0.995 | 49.785 | 0.68x |
| nested.json | msgspec | 0.606 | 0.631 | 0.712 | 49.785 | 0.85x |
| nested.json | ujson | 0.951 | 1.026 | 1.112 | 49.785 | 0.52x |
| nested.json | json | 1.339 | 1.379 | 1.462 | 49.785 | 0.39x |
| wide_arrays.json | strata | 3.041 | 4.506 | 5.811 | 51.520 | 1.00x |
| wide_arrays.json | orjson | 4.682 | 6.111 | 7.214 | 51.520 | 0.74x |
| wide_arrays.json | msgspec | 4.529 | 6.048 | 10.796 | 51.520 | 0.75x |
| wide_arrays.json | ujson | 6.055 | 8.420 | 11.368 | 51.520 | 0.54x |
| wide_arrays.json | json | 8.263 | 11.963 | 18.904 | 51.520 | 0.38x |
| mixed.json | strata | 0.131 | 0.143 | 0.154 | 49.582 | 1.00x |
| mixed.json | orjson | 0.151 | 0.159 | 0.193 | 49.582 | 0.90x |
| mixed.json | msgspec | 0.170 | 0.182 | 0.253 | 49.582 | 0.79x |
| mixed.json | ujson | 0.226 | 0.249 | 0.299 | 49.582 | 0.57x |
| mixed.json | json | 0.328 | 0.368 | 0.436 | 49.582 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.151 | 2.331 | 3.246 | 44.840 | 1.00x |
| users.json | orjson | 2.818 | 3.202 | 3.683 | 44.840 | 0.73x |
| users.json | msgspec | 4.224 | 4.660 | 5.213 | 44.840 | 0.50x |
| users.json | ujson | 10.596 | 11.861 | 16.089 | 44.840 | 0.20x |
| users.json | json | 16.177 | 17.425 | 19.891 | 44.840 | 0.13x |
| flat.json | strata | 0.249 | 0.259 | 0.289 | 50.355 | 1.00x |
| flat.json | orjson | 0.242 | 0.252 | 0.304 | 50.355 | 1.03x |
| flat.json | msgspec | 0.371 | 0.393 | 0.431 | 50.355 | 0.66x |
| flat.json | ujson | 0.895 | 0.941 | 1.089 | 50.355 | 0.28x |
| flat.json | json | 1.334 | 1.382 | 1.612 | 50.355 | 0.19x |
| nested.json | strata | 0.171 | 0.188 | 0.361 | 50.266 | 1.00x |
| nested.json | orjson | 0.240 | 0.268 | 0.336 | 50.266 | 0.70x |
| nested.json | msgspec | 0.327 | 0.365 | 1.049 | 50.266 | 0.52x |
| nested.json | ujson | 0.848 | 0.877 | 1.317 | 50.266 | 0.21x |
| nested.json | json | 1.629 | 1.743 | 2.795 | 50.266 | 0.11x |
| wide_arrays.json | strata | 2.219 | 2.286 | 2.784 | 52.926 | 1.00x |
| wide_arrays.json | orjson | 2.032 | 2.102 | 2.232 | 52.926 | 1.09x |
| wide_arrays.json | msgspec | 3.503 | 3.564 | 3.735 | 52.926 | 0.64x |
| wide_arrays.json | ujson | 9.101 | 9.296 | 10.817 | 52.926 | 0.25x |
| wide_arrays.json | json | 22.191 | 22.455 | 23.856 | 52.926 | 0.10x |
| mixed.json | strata | 0.058 | 0.063 | 0.073 | 49.793 | 1.00x |
| mixed.json | orjson | 0.055 | 0.059 | 0.082 | 49.793 | 1.06x |
| mixed.json | msgspec | 0.075 | 0.096 | 0.142 | 49.793 | 0.65x |
| mixed.json | ujson | 0.201 | 0.212 | 0.274 | 49.793 | 0.30x |
| mixed.json | json | 0.387 | 0.412 | 0.463 | 49.793 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.459 | 8.179 | 12.518 | 54.871 | 1.00x |
| users.json | orjson | 12.695 | 13.804 | 18.009 | 54.871 | 0.59x |
| users.json | msgspec | 11.806 | 12.574 | 16.059 | 54.871 | 0.65x |
| users.json | ujson | 17.831 | 19.449 | 25.718 | 54.871 | 0.42x |
| users.json | json | 17.896 | 19.484 | 26.108 | 54.871 | 0.42x |
| flat.json | strata | 0.882 | 0.978 | 1.263 | 50.438 | 1.00x |
| flat.json | orjson | 1.241 | 1.323 | 1.852 | 50.438 | 0.74x |
| flat.json | msgspec | 0.891 | 0.949 | 1.225 | 50.438 | 1.03x |
| flat.json | ujson | 1.490 | 1.589 | 2.217 | 50.438 | 0.62x |
| flat.json | json | 1.436 | 1.511 | 2.163 | 50.438 | 0.65x |
| nested.json | strata | 0.598 | 0.627 | 0.683 | 49.562 | 1.00x |
| nested.json | orjson | 0.910 | 0.975 | 1.001 | 49.562 | 0.64x |
| nested.json | msgspec | 0.745 | 0.818 | 0.932 | 49.562 | 0.77x |
| nested.json | ujson | 1.205 | 1.307 | 1.681 | 49.562 | 0.48x |
| nested.json | json | 1.446 | 1.546 | 1.669 | 49.562 | 0.41x |
| wide_arrays.json | strata | 3.705 | 3.876 | 5.753 | 52.926 | 1.00x |
| wide_arrays.json | orjson | 5.278 | 5.381 | 6.305 | 52.926 | 0.72x |
| wide_arrays.json | msgspec | 5.087 | 5.250 | 6.006 | 52.926 | 0.74x |
| wide_arrays.json | ujson | 8.155 | 8.499 | 9.311 | 52.926 | 0.46x |
| wide_arrays.json | json | 8.911 | 9.152 | 12.971 | 52.926 | 0.42x |
| mixed.json | strata | 0.215 | 0.257 | 1.112 | 49.953 | 1.00x |
| mixed.json | orjson | 0.260 | 0.273 | 0.355 | 49.953 | 0.94x |
| mixed.json | msgspec | 0.275 | 0.297 | 0.851 | 49.953 | 0.87x |
| mixed.json | ujson | 0.364 | 0.384 | 0.537 | 49.953 | 0.67x |
| mixed.json | json | 0.437 | 0.492 | 0.631 | 49.953 | 0.52x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.444 | 7.544 | 7.604 | 50.812 | 1.00x |
| users.ndjson | orjson | 13.110 | 13.251 | 13.515 | 50.812 | 0.57x |
| users.ndjson | msgspec | 12.960 | 13.222 | 18.082 | 50.812 | 0.57x |
| users.ndjson | ujson | 16.899 | 16.974 | 21.431 | 50.812 | 0.44x |
| users.ndjson | json | 20.819 | 21.032 | 29.655 | 50.812 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.026 | 3.294 | 5.737 | 51.492 | 1.00x |
| users.json | orjson | 3.515 | 3.811 | 5.172 | 51.492 | 0.86x |
| users.json | msgspec | 5.030 | 5.678 | 149.887 | 51.492 | 0.58x |
| users.json | ujson | 16.663 | 17.936 | 25.321 | 51.492 | 0.18x |
| users.json | json | 22.343 | 23.641 | 74.601 | 51.492 | 0.14x |
| flat.json | strata | 0.539 | 0.569 | 0.648 | 50.402 | 1.00x |
| flat.json | orjson | 0.542 | 0.580 | 0.690 | 50.402 | 0.98x |
| flat.json | msgspec | 0.672 | 0.724 | 0.846 | 50.402 | 0.79x |
| flat.json | ujson | 1.801 | 1.840 | 2.270 | 50.402 | 0.31x |
| flat.json | json | 2.245 | 2.305 | 2.390 | 50.402 | 0.25x |
| nested.json | strata | 0.442 | 0.494 | 4.319 | 50.129 | 1.00x |
| nested.json | orjson | 0.553 | 0.614 | 0.852 | 50.129 | 0.80x |
| nested.json | msgspec | 0.657 | 0.687 | 1.230 | 50.129 | 0.72x |
| nested.json | ujson | 1.595 | 1.658 | 49.489 | 50.129 | 0.30x |
| nested.json | json | 2.378 | 2.433 | 4.355 | 50.129 | 0.20x |
| wide_arrays.json | strata | 2.271 | 2.340 | 3.104 | 50.535 | 1.00x |
| wide_arrays.json | orjson | 2.211 | 2.287 | 3.492 | 50.535 | 1.02x |
| wide_arrays.json | msgspec | 3.387 | 3.520 | 4.376 | 50.535 | 0.66x |
| wide_arrays.json | ujson | 11.079 | 11.483 | 12.618 | 50.535 | 0.20x |
| wide_arrays.json | json | 20.631 | 21.691 | 27.529 | 50.535 | 0.11x |
| mixed.json | strata | 0.314 | 0.346 | 0.522 | 49.961 | 1.00x |
| mixed.json | orjson | 0.315 | 0.342 | 0.508 | 49.961 | 1.01x |
| mixed.json | msgspec | 0.345 | 0.376 | 0.405 | 49.961 | 0.92x |
| mixed.json | ujson | 0.571 | 0.598 | 0.776 | 49.961 | 0.58x |
| mixed.json | json | 0.748 | 0.826 | 1.793 | 49.961 | 0.42x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.066 | 0.074 | 0.080 | 50.777 | 1.00x |
| users.json $[*].id | jmespath | 0.296 | 0.328 | 0.391 | 50.777 | 0.23x |
| users.json $[*].id | jsonpath-ng | 1.535 | 1.697 | 1.848 | 50.777 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.423 | 0.455 | 0.505 | 50.820 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.735 | 2.026 | 2.096 | 50.820 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.844 | 11.319 | 11.883 | 50.820 | 0.04x |
| users.json $..total | strata | 1.345 | 1.497 | 1.571 | 52.824 | 1.00x |
| users.json $..total | jsonpath-ng | 203.602 | 207.989 | 293.559 | 52.824 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.127 | 3.372 | 4.263 | 50.785 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.711 | 13.737 | 14.320 | 50.785 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 13.721 | 14.640 | 17.116 | 50.785 | 0.23x |
| users.json $[*].orders[*].total | strata | 3.424 | 3.765 | 4.017 | 52.820 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.344 | 15.133 | 17.030 | 52.820 | 0.25x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 28.912 | 30.542 | 36.290 | 52.820 | 0.12x |
| users.json $..total | strata | 9.481 | 10.159 | 13.756 | 52.824 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 220.271 | 223.997 | 233.902 | 52.824 | 0.05x |

