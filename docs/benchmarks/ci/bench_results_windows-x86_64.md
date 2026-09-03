# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ddc9cb7
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD
- compiler_flags: clang-cl /std:c++20 /O2 /arch:AVX2 -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.410 | 11.522 | 32.589 | 45.555 | 1.00x |
| users.json | orjson | 15.595 | 16.497 | 28.683 | 45.555 | 0.70x |
| users.json | msgspec | 13.640 | 15.024 | 31.729 | 45.555 | 0.77x |
| users.json | ujson | 22.053 | 23.766 | 44.019 | 45.555 | 0.48x |
| users.json | json | 24.111 | 25.941 | 38.353 | 45.555 | 0.44x |
| flat.json | strata | 1.091 | 1.143 | 3.576 | 51.848 | 1.00x |
| flat.json | orjson | 1.293 | 1.334 | 2.358 | 51.848 | 0.86x |
| flat.json | msgspec | 1.139 | 1.250 | 2.305 | 51.848 | 0.91x |
| flat.json | ujson | 2.169 | 2.316 | 3.565 | 51.848 | 0.49x |
| flat.json | json | 1.910 | 1.940 | 3.401 | 51.848 | 0.59x |
| nested.json | strata | 1.235 | 1.267 | 1.285 | 51.961 | 1.00x |
| nested.json | orjson | 1.699 | 1.735 | 1.795 | 51.961 | 0.73x |
| nested.json | msgspec | 1.619 | 1.652 | 2.133 | 51.961 | 0.77x |
| nested.json | ujson | 2.626 | 2.706 | 49.218 | 51.961 | 0.47x |
| nested.json | json | 3.480 | 3.548 | 3.799 | 51.961 | 0.36x |
| wide_arrays.json | strata | 7.549 | 7.649 | 16.498 | 54.938 | 1.00x |
| wide_arrays.json | orjson | 9.086 | 9.464 | 13.128 | 54.938 | 0.81x |
| wide_arrays.json | msgspec | 9.640 | 10.519 | 13.879 | 54.938 | 0.73x |
| wide_arrays.json | ujson | 13.024 | 14.411 | 25.602 | 54.938 | 0.53x |
| wide_arrays.json | json | 19.331 | 19.727 | 30.666 | 54.938 | 0.39x |
| mixed.json | strata | 0.296 | 0.301 | 0.332 | 53.676 | 1.00x |
| mixed.json | orjson | 0.342 | 0.349 | 0.373 | 53.676 | 0.86x |
| mixed.json | msgspec | 0.378 | 0.387 | 0.412 | 53.676 | 0.78x |
| mixed.json | ujson | 0.550 | 0.569 | 0.600 | 53.676 | 0.53x |
| mixed.json | json | 0.757 | 0.773 | 2.017 | 53.676 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.297 | 3.485 | 4.768 | 44.852 | 1.00x |
| users.json | orjson | 3.700 | 4.570 | 5.260 | 44.852 | 0.76x |
| users.json | msgspec | 5.412 | 6.016 | 6.584 | 44.852 | 0.58x |
| users.json | ujson | 16.678 | 18.429 | 19.014 | 44.852 | 0.19x |
| users.json | json | 24.917 | 27.297 | 43.827 | 44.852 | 0.13x |
| flat.json | strata | 0.330 | 0.449 | 0.473 | 52.250 | 1.00x |
| flat.json | orjson | 0.499 | 0.574 | 1.976 | 52.250 | 0.78x |
| flat.json | msgspec | 0.705 | 0.759 | 1.004 | 52.250 | 0.59x |
| flat.json | ujson | 2.176 | 2.516 | 3.101 | 52.250 | 0.18x |
| flat.json | json | 2.148 | 3.383 | 7.217 | 52.250 | 0.13x |
| nested.json | strata | 0.430 | 0.445 | 0.846 | 52.422 | 1.00x |
| nested.json | orjson | 0.471 | 0.481 | 0.525 | 52.422 | 0.93x |
| nested.json | msgspec | 0.767 | 0.777 | 0.792 | 52.422 | 0.57x |
| nested.json | ujson | 2.117 | 2.128 | 2.183 | 52.422 | 0.21x |
| nested.json | json | 4.158 | 4.183 | 4.613 | 52.422 | 0.11x |
| wide_arrays.json | strata | 2.129 | 2.953 | 4.754 | 54.875 | 1.00x |
| wide_arrays.json | orjson | 2.237 | 2.927 | 4.669 | 54.875 | 1.01x |
| wide_arrays.json | msgspec | 3.842 | 5.805 | 16.627 | 54.875 | 0.51x |
| wide_arrays.json | ujson | 9.794 | 14.418 | 63.245 | 54.875 | 0.20x |
| wide_arrays.json | json | 17.750 | 32.799 | 86.665 | 54.875 | 0.09x |
| mixed.json | strata | 0.100 | 0.109 | 0.126 | 53.711 | 1.00x |
| mixed.json | orjson | 0.100 | 0.107 | 9.929 | 53.711 | 1.02x |
| mixed.json | msgspec | 0.141 | 0.147 | 0.179 | 53.711 | 0.74x |
| mixed.json | ujson | 0.513 | 0.525 | 0.546 | 53.711 | 0.21x |
| mixed.json | json | 0.895 | 0.916 | 1.379 | 53.711 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.795 | 13.023 | 25.191 | 54.887 | 1.00x |
| users.json | orjson | 16.654 | 17.615 | 18.127 | 54.887 | 0.74x |
| users.json | msgspec | 15.575 | 16.903 | 30.635 | 54.887 | 0.77x |
| users.json | ujson | 26.239 | 29.066 | 47.536 | 54.887 | 0.45x |
| users.json | json | 26.151 | 27.578 | 30.294 | 54.887 | 0.47x |
| flat.json | strata | 1.461 | 1.571 | 1.665 | 52.582 | 1.00x |
| flat.json | orjson | 2.061 | 2.190 | 2.371 | 52.582 | 0.72x |
| flat.json | msgspec | 1.826 | 1.933 | 2.156 | 52.582 | 0.81x |
| flat.json | ujson | 3.820 | 3.984 | 6.509 | 52.582 | 0.39x |
| flat.json | json | 3.129 | 3.350 | 3.437 | 52.582 | 0.47x |
| nested.json | strata | 1.332 | 1.394 | 1.668 | 52.109 | 1.00x |
| nested.json | orjson | 1.837 | 1.907 | 3.088 | 52.109 | 0.73x |
| nested.json | msgspec | 1.778 | 1.818 | 2.464 | 52.109 | 0.77x |
| nested.json | ujson | 2.981 | 3.123 | 3.886 | 52.109 | 0.45x |
| nested.json | json | 3.469 | 3.733 | 4.633 | 52.109 | 0.37x |
| wide_arrays.json | strata | 5.926 | 6.102 | 8.989 | 54.875 | 1.00x |
| wide_arrays.json | orjson | 6.551 | 6.776 | 11.679 | 54.875 | 0.90x |
| wide_arrays.json | msgspec | 6.540 | 6.735 | 12.173 | 54.875 | 0.91x |
| wide_arrays.json | ujson | 10.615 | 10.984 | 11.462 | 54.875 | 0.56x |
| wide_arrays.json | json | 12.025 | 12.364 | 20.488 | 54.875 | 0.49x |
| mixed.json | strata | 0.412 | 0.464 | 0.698 | 53.711 | 1.00x |
| mixed.json | orjson | 0.499 | 0.510 | 0.879 | 53.711 | 0.91x |
| mixed.json | msgspec | 0.530 | 0.559 | 0.812 | 53.711 | 0.83x |
| mixed.json | ujson | 0.783 | 0.851 | 1.229 | 53.711 | 0.55x |
| mixed.json | json | 0.912 | 0.985 | 1.506 | 53.711 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 12.641 | 14.293 | 18.431 | 53.062 | 1.00x |
| users.ndjson | orjson | 19.162 | 23.141 | 30.413 | 53.062 | 0.62x |
| users.ndjson | msgspec | 18.621 | 22.052 | 35.927 | 53.062 | 0.65x |
| users.ndjson | ujson | 27.970 | 29.828 | 49.040 | 53.062 | 0.48x |
| users.ndjson | json | 31.561 | 39.296 | 50.259 | 53.062 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.211 | 4.693 | 10.955 | 52.891 | 1.00x |
| users.json | orjson | 4.740 | 5.319 | 6.277 | 52.891 | 0.88x |
| users.json | msgspec | 6.367 | 7.264 | 16.803 | 52.891 | 0.65x |
| users.json | ujson | 26.956 | 30.030 | 51.335 | 52.891 | 0.16x |
| users.json | json | 35.767 | 38.332 | 39.064 | 52.891 | 0.12x |
| flat.json | strata | 0.946 | 1.128 | 1.664 | 52.324 | 1.00x |
| flat.json | orjson | 1.102 | 1.212 | 47.245 | 52.324 | 0.93x |
| flat.json | msgspec | 1.257 | 1.344 | 1.662 | 52.324 | 0.84x |
| flat.json | ujson | 4.708 | 5.113 | 7.647 | 52.324 | 0.22x |
| flat.json | json | 5.565 | 5.891 | 7.025 | 52.324 | 0.19x |
| nested.json | strata | 0.912 | 1.058 | 1.415 | 52.551 | 1.00x |
| nested.json | orjson | 0.912 | 1.126 | 1.639 | 52.551 | 0.94x |
| nested.json | msgspec | 1.185 | 1.478 | 2.156 | 52.551 | 0.72x |
| nested.json | ujson | 3.978 | 5.753 | 12.390 | 52.551 | 0.18x |
| nested.json | json | 5.933 | 7.050 | 51.796 | 52.551 | 0.15x |
| wide_arrays.json | strata | 3.971 | 4.082 | 50.269 | 54.785 | 1.00x |
| wide_arrays.json | orjson | 4.083 | 4.545 | 8.841 | 54.785 | 0.90x |
| wide_arrays.json | msgspec | 6.148 | 6.825 | 14.237 | 54.785 | 0.60x |
| wide_arrays.json | ujson | 26.075 | 28.917 | 147.067 | 54.785 | 0.14x |
| wide_arrays.json | json | 40.633 | 45.509 | 113.545 | 54.785 | 0.09x |
| mixed.json | strata | 0.534 | 0.551 | 0.901 | 53.785 | 1.00x |
| mixed.json | orjson | 0.507 | 0.556 | 1.575 | 53.785 | 0.99x |
| mixed.json | msgspec | 0.545 | 0.586 | 0.632 | 53.785 | 0.94x |
| mixed.json | ujson | 1.231 | 1.293 | 3.179 | 53.785 | 0.43x |
| mixed.json | json | 1.591 | 1.655 | 3.359 | 53.785 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.095 | 0.097 | 0.142 | 52.961 | 1.00x |
| users.json $[*].id | jmespath | 0.420 | 0.531 | 0.731 | 52.961 | 0.18x |
| users.json $[*].id | jsonpath-ng | 2.349 | 2.657 | 3.921 | 52.961 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.517 | 0.557 | 1.128 | 53.027 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.678 | 2.937 | 3.621 | 53.027 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.922 | 18.994 | 32.457 | 53.027 | 0.03x |
| users.json $..total | strata | 1.937 | 1.948 | 3.450 | 54.027 | 1.00x |
| users.json $..total | jsonpath-ng | 325.079 | 346.126 | 473.880 | 54.027 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.267 | 4.438 | 7.387 | 52.992 | 1.00x |
| users.json $[*].id | orjson+jmespath | 17.161 | 18.075 | 19.054 | 52.992 | 0.25x |
| users.json $[*].id | orjson+jsonpath-ng | 19.066 | 20.667 | 29.685 | 52.992 | 0.21x |
| users.json $[*].orders[*].total | strata | 4.686 | 5.066 | 6.438 | 54.027 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.945 | 21.704 | 38.854 | 54.027 | 0.23x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 39.908 | 42.470 | 125.251 | 54.027 | 0.12x |
| users.json $..total | strata | 19.445 | 24.402 | 75.662 | 54.020 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 470.840 | 639.723 | 994.352 | 54.020 | 0.04x |

