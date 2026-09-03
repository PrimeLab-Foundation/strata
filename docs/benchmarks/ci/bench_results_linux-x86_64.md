# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6ef7acf
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
| users.json | strata | 12.698 | 13.347 | 21.000 | 57.641 | 1.00x |
| users.json | orjson | 16.269 | 17.412 | 24.946 | 57.641 | 0.77x |
| users.json | msgspec | 16.341 | 17.710 | 32.089 | 57.641 | 0.75x |
| users.json | ujson | 24.146 | 24.709 | 33.429 | 57.641 | 0.54x |
| users.json | pysimdjson | 25.184 | 26.307 | 28.465 | 57.641 | 0.51x |
| users.json | json | 24.071 | 25.130 | 26.101 | 57.641 | 0.53x |
| flat.json | strata | 0.896 | 0.983 | 1.036 | 63.895 | 1.00x |
| flat.json | orjson | 1.054 | 1.139 | 1.484 | 63.895 | 0.86x |
| flat.json | msgspec | 1.097 | 1.148 | 1.200 | 63.895 | 0.86x |
| flat.json | ujson | 1.701 | 1.875 | 2.013 | 63.895 | 0.52x |
| flat.json | pysimdjson | 1.743 | 1.967 | 2.052 | 63.895 | 0.50x |
| flat.json | json | 1.736 | 1.807 | 1.929 | 63.895 | 0.54x |
| nested.json | strata | 0.875 | 0.896 | 0.968 | 63.895 | 1.00x |
| nested.json | orjson | 1.070 | 1.115 | 1.176 | 63.895 | 0.80x |
| nested.json | msgspec | 1.035 | 1.068 | 1.114 | 63.895 | 0.84x |
| nested.json | ujson | 1.609 | 1.721 | 1.748 | 63.895 | 0.52x |
| nested.json | pysimdjson | 1.478 | 1.623 | 1.807 | 63.895 | 0.55x |
| nested.json | json | 1.905 | 1.940 | 2.055 | 63.895 | 0.46x |
| wide_arrays.json | strata | 5.527 | 5.793 | 6.403 | 70.250 | 1.00x |
| wide_arrays.json | orjson | 6.318 | 6.577 | 7.046 | 70.250 | 0.88x |
| wide_arrays.json | msgspec | 6.763 | 6.950 | 7.129 | 70.250 | 0.83x |
| wide_arrays.json | ujson | 8.421 | 8.812 | 9.077 | 70.250 | 0.66x |
| wide_arrays.json | pysimdjson | 7.195 | 7.654 | 7.821 | 70.250 | 0.76x |
| wide_arrays.json | json | 10.691 | 10.999 | 11.434 | 70.250 | 0.53x |
| mixed.json | strata | 0.226 | 0.263 | 0.353 | 70.312 | 1.00x |
| mixed.json | orjson | 0.278 | 0.298 | 0.326 | 70.312 | 0.88x |
| mixed.json | msgspec | 0.282 | 0.301 | 0.339 | 70.312 | 0.87x |
| mixed.json | ujson | 0.384 | 0.399 | 0.428 | 70.312 | 0.66x |
| mixed.json | pysimdjson | 0.330 | 0.384 | 0.403 | 70.312 | 0.68x |
| mixed.json | json | 0.494 | 0.512 | 0.536 | 70.312 | 0.51x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.762 | 2.902 | 3.458 | 56.180 | 1.00x |
| users.json | orjson | 3.339 | 3.471 | 4.056 | 56.180 | 0.84x |
| users.json | msgspec | 4.292 | 4.609 | 4.766 | 56.180 | 0.63x |
| users.json | ujson | 15.080 | 15.537 | 15.929 | 56.180 | 0.19x |
| users.json | json | 22.370 | 22.808 | 23.240 | 56.180 | 0.13x |
| flat.json | strata | 0.343 | 0.393 | 0.453 | 63.895 | 1.00x |
| flat.json | orjson | 0.394 | 0.469 | 0.634 | 63.895 | 0.84x |
| flat.json | msgspec | 0.509 | 0.609 | 0.654 | 63.895 | 0.65x |
| flat.json | ujson | 1.317 | 1.403 | 1.936 | 63.895 | 0.28x |
| flat.json | json | 1.940 | 2.017 | 2.155 | 63.895 | 0.19x |
| nested.json | strata | 0.256 | 0.277 | 0.312 | 63.898 | 1.00x |
| nested.json | orjson | 0.307 | 0.324 | 0.338 | 63.898 | 0.86x |
| nested.json | msgspec | 0.432 | 0.447 | 0.470 | 63.898 | 0.62x |
| nested.json | ujson | 1.360 | 1.376 | 1.404 | 63.898 | 0.20x |
| nested.json | json | 2.385 | 2.407 | 2.438 | 63.898 | 0.12x |
| wide_arrays.json | strata | 1.786 | 1.857 | 2.335 | 70.250 | 1.00x |
| wide_arrays.json | orjson | 1.965 | 2.002 | 2.103 | 70.250 | 0.93x |
| wide_arrays.json | msgspec | 3.035 | 3.088 | 3.133 | 70.250 | 0.60x |
| wide_arrays.json | ujson | 8.780 | 8.844 | 9.231 | 70.250 | 0.21x |
| wide_arrays.json | json | 16.945 | 17.191 | 17.506 | 70.250 | 0.11x |
| mixed.json | strata | 0.077 | 0.082 | 0.095 | 70.312 | 1.00x |
| mixed.json | orjson | 0.077 | 0.084 | 0.091 | 70.312 | 0.97x |
| mixed.json | msgspec | 0.097 | 0.109 | 0.149 | 70.312 | 0.75x |
| mixed.json | ujson | 0.320 | 0.329 | 0.352 | 70.312 | 0.25x |
| mixed.json | json | 0.536 | 0.544 | 0.577 | 70.312 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.928 | 13.564 | 15.109 | 70.449 | 1.00x |
| users.json | orjson | 16.430 | 16.950 | 18.571 | 70.449 | 0.80x |
| users.json | msgspec | 16.203 | 16.915 | 18.492 | 70.449 | 0.80x |
| users.json | ujson | 22.792 | 25.018 | 26.680 | 70.449 | 0.54x |
| users.json | json | 23.560 | 25.102 | 26.757 | 70.449 | 0.54x |
| flat.json | strata | 0.982 | 1.021 | 1.075 | 63.895 | 1.00x |
| flat.json | orjson | 1.161 | 1.211 | 1.270 | 63.895 | 0.84x |
| flat.json | msgspec | 1.188 | 1.232 | 1.342 | 63.895 | 0.83x |
| flat.json | ujson | 1.806 | 1.940 | 2.087 | 63.895 | 0.53x |
| flat.json | json | 1.788 | 1.893 | 1.943 | 63.895 | 0.54x |
| nested.json | strata | 0.908 | 0.988 | 1.055 | 63.898 | 1.00x |
| nested.json | orjson | 1.188 | 1.286 | 1.374 | 63.898 | 0.77x |
| nested.json | msgspec | 1.111 | 1.195 | 1.348 | 63.898 | 0.83x |
| nested.json | ujson | 1.644 | 1.775 | 1.910 | 63.898 | 0.56x |
| nested.json | json | 1.994 | 2.075 | 2.294 | 63.898 | 0.48x |
| wide_arrays.json | strata | 5.500 | 5.614 | 5.748 | 70.312 | 1.00x |
| wide_arrays.json | orjson | 6.189 | 6.394 | 6.524 | 70.312 | 0.88x |
| wide_arrays.json | msgspec | 6.928 | 7.008 | 7.168 | 70.312 | 0.80x |
| wide_arrays.json | ujson | 8.788 | 8.889 | 9.019 | 70.312 | 0.63x |
| wide_arrays.json | json | 10.597 | 10.811 | 11.209 | 70.312 | 0.52x |
| mixed.json | strata | 0.253 | 0.265 | 0.298 | 70.312 | 1.00x |
| mixed.json | orjson | 0.333 | 0.364 | 0.368 | 70.312 | 0.73x |
| mixed.json | msgspec | 0.332 | 0.344 | 0.363 | 70.312 | 0.77x |
| mixed.json | ujson | 0.450 | 0.465 | 0.616 | 70.312 | 0.57x |
| mixed.json | json | 0.540 | 0.563 | 0.648 | 70.312 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 14.833 | 15.002 | 16.030 | 63.895 | 1.00x |
| users.ndjson | orjson | 21.698 | 22.009 | 23.416 | 63.895 | 0.68x |
| users.ndjson | msgspec | 21.498 | 21.966 | 22.611 | 63.895 | 0.68x |
| users.ndjson | ujson | 27.581 | 28.054 | 28.784 | 63.895 | 0.53x |
| users.ndjson | json | 33.647 | 33.865 | 34.702 | 63.895 | 0.44x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.322 | 3.471 | 3.670 | 65.527 | 1.00x |
| users.json | orjson | 4.003 | 4.162 | 4.275 | 65.527 | 0.83x |
| users.json | msgspec | 5.027 | 5.285 | 5.441 | 65.527 | 0.66x |
| users.json | ujson | 16.113 | 16.343 | 16.655 | 65.527 | 0.21x |
| users.json | json | 23.465 | 23.806 | 24.335 | 65.527 | 0.15x |
| flat.json | strata | 0.558 | 0.580 | 0.652 | 63.895 | 1.00x |
| flat.json | orjson | 0.631 | 0.680 | 0.792 | 63.895 | 0.85x |
| flat.json | msgspec | 0.755 | 0.807 | 0.882 | 63.895 | 0.72x |
| flat.json | ujson | 1.557 | 1.637 | 1.682 | 63.895 | 0.35x |
| flat.json | json | 2.207 | 2.260 | 2.353 | 63.895 | 0.26x |
| nested.json | strata | 0.433 | 0.450 | 0.498 | 63.898 | 1.00x |
| nested.json | orjson | 0.509 | 0.524 | 0.563 | 63.898 | 0.86x |
| nested.json | msgspec | 0.605 | 0.642 | 0.663 | 63.898 | 0.70x |
| nested.json | ujson | 1.566 | 1.603 | 1.655 | 63.898 | 0.28x |
| nested.json | json | 2.596 | 2.625 | 2.743 | 63.898 | 0.17x |
| wide_arrays.json | strata | 2.307 | 2.388 | 2.508 | 70.312 | 1.00x |
| wide_arrays.json | orjson | 2.574 | 2.617 | 3.268 | 70.312 | 0.91x |
| wide_arrays.json | msgspec | 3.621 | 3.745 | 3.811 | 70.312 | 0.64x |
| wide_arrays.json | ujson | 9.536 | 9.649 | 9.845 | 70.312 | 0.25x |
| wide_arrays.json | json | 17.689 | 17.919 | 18.110 | 70.312 | 0.13x |
| mixed.json | strata | 0.185 | 0.202 | 0.215 | 70.312 | 1.00x |
| mixed.json | orjson | 0.203 | 0.216 | 0.236 | 70.312 | 0.94x |
| mixed.json | msgspec | 0.220 | 0.233 | 0.247 | 70.312 | 0.87x |
| mixed.json | ujson | 0.456 | 0.471 | 0.500 | 70.312 | 0.43x |
| mixed.json | json | 0.680 | 0.692 | 0.740 | 70.312 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.092 | 0.100 | 0.122 | 65.527 | 1.00x |
| users.json $[*].id | jmespath | 0.491 | 0.520 | 0.557 | 65.527 | 0.19x |
| users.json $[*].id | jsonpath-ng | 2.879 | 3.154 | 3.244 | 65.527 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.511 | 0.567 | 0.652 | 67.492 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 3.332 | 3.570 | 4.057 | 67.492 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 22.334 | 22.957 | 24.955 | 67.492 | 0.02x |
| users.json $..total | strata | 1.880 | 2.107 | 2.170 | 69.461 | 1.00x |
| users.json $..total | jsonpath-ng | 388.139 | 393.399 | 449.038 | 69.461 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.295 | 3.310 | 3.362 | 67.492 | 1.00x |
| users.json $[*].id | orjson+jmespath | 16.795 | 17.258 | 17.993 | 67.492 | 0.19x |
| users.json $[*].id | orjson+jsonpath-ng | 19.381 | 20.340 | 21.018 | 67.492 | 0.16x |
| users.json $[*].orders[*].total | strata | 3.589 | 3.648 | 3.680 | 68.695 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 21.621 | 22.037 | 22.777 | 68.695 | 0.17x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 46.023 | 47.146 | 52.093 | 68.695 | 0.08x |
| users.json $..total | strata | 16.648 | 18.721 | 19.518 | 68.715 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 418.614 | 422.381 | 426.988 | 68.715 | 0.04x |

