# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 49acca3
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 19.050 | 19.498 | 24.892 | 52.652 | 1.00x |
| users.json | orjson | 26.782 | 28.460 | 33.314 | 52.652 | 0.69x |
| users.json | msgspec | 27.468 | 29.511 | 38.743 | 52.652 | 0.66x |
| users.json | ujson | 36.528 | 41.348 | 53.001 | 52.652 | 0.47x |
| users.json | pysimdjson | 159.647 | 172.237 | 188.280 | 52.652 | 0.11x |
| users.json | json | 42.523 | 47.697 | 86.329 | 52.652 | 0.41x |
| flat.json | strata | 1.232 | 1.306 | 1.531 | 57.289 | 1.00x |
| flat.json | orjson | 1.316 | 1.402 | 1.450 | 57.289 | 0.93x |
| flat.json | msgspec | 1.518 | 1.608 | 1.783 | 57.289 | 0.81x |
| flat.json | ujson | 2.717 | 2.792 | 3.015 | 57.289 | 0.47x |
| flat.json | pysimdjson | 14.332 | 14.491 | 15.422 | 57.289 | 0.09x |
| flat.json | json | 3.081 | 3.169 | 10.591 | 57.289 | 0.41x |
| nested.json | strata | 1.487 | 1.622 | 1.746 | 54.582 | 1.00x |
| nested.json | orjson | 1.691 | 1.778 | 1.913 | 54.582 | 0.91x |
| nested.json | msgspec | 1.894 | 1.957 | 2.206 | 54.582 | 0.83x |
| nested.json | ujson | 3.097 | 3.243 | 3.494 | 54.582 | 0.50x |
| nested.json | pysimdjson | 13.364 | 14.420 | 15.030 | 54.582 | 0.11x |
| nested.json | json | 3.941 | 4.164 | 4.651 | 54.582 | 0.39x |
| wide_arrays.json | strata | 8.886 | 9.186 | 10.031 | 58.863 | 1.00x |
| wide_arrays.json | orjson | 9.832 | 10.487 | 12.344 | 58.863 | 0.88x |
| wide_arrays.json | msgspec | 10.944 | 11.572 | 14.752 | 58.863 | 0.79x |
| wide_arrays.json | ujson | 13.659 | 15.638 | 18.099 | 58.863 | 0.59x |
| wide_arrays.json | pysimdjson | 83.258 | 84.842 | 88.880 | 58.863 | 0.11x |
| wide_arrays.json | json | 18.079 | 18.474 | 19.486 | 58.863 | 0.50x |
| mixed.json | strata | 0.395 | 0.423 | 0.485 | 55.879 | 1.00x |
| mixed.json | orjson | 0.463 | 0.495 | 0.653 | 55.879 | 0.86x |
| mixed.json | msgspec | 0.495 | 0.534 | 0.794 | 55.879 | 0.79x |
| mixed.json | ujson | 0.669 | 0.699 | 0.976 | 55.879 | 0.61x |
| mixed.json | pysimdjson | 3.372 | 3.642 | 4.018 | 55.879 | 0.12x |
| mixed.json | json | 0.924 | 0.987 | 1.198 | 55.879 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.499 | 2.852 | 3.489 | 48.457 | 1.00x |
| users.json | orjson | 3.136 | 3.434 | 5.207 | 48.457 | 0.83x |
| users.json | msgspec | 5.612 | 6.069 | 8.464 | 48.457 | 0.47x |
| users.json | ujson | 30.797 | 31.714 | 39.976 | 48.457 | 0.09x |
| users.json | json | 41.137 | 43.291 | 62.246 | 48.457 | 0.07x |
| flat.json | strata | 0.322 | 0.336 | 0.364 | 54.414 | 1.00x |
| flat.json | orjson | 0.419 | 0.432 | 0.453 | 54.414 | 0.78x |
| flat.json | msgspec | 0.561 | 0.569 | 0.616 | 54.414 | 0.59x |
| flat.json | ujson | 2.624 | 2.703 | 3.127 | 54.414 | 0.12x |
| flat.json | json | 3.578 | 3.662 | 4.073 | 54.414 | 0.09x |
| nested.json | strata | 0.267 | 0.294 | 0.352 | 49.484 | 1.00x |
| nested.json | orjson | 0.360 | 0.384 | 0.473 | 49.484 | 0.76x |
| nested.json | msgspec | 0.578 | 0.602 | 0.717 | 49.484 | 0.49x |
| nested.json | ujson | 2.822 | 2.950 | 3.118 | 49.484 | 0.10x |
| nested.json | json | 4.701 | 4.824 | 5.547 | 49.484 | 0.06x |
| wide_arrays.json | strata | 1.984 | 2.088 | 2.320 | 57.387 | 1.00x |
| wide_arrays.json | orjson | 2.508 | 2.630 | 2.856 | 57.387 | 0.79x |
| wide_arrays.json | msgspec | 3.466 | 3.636 | 3.823 | 57.387 | 0.57x |
| wide_arrays.json | ujson | 16.176 | 17.350 | 20.640 | 57.387 | 0.12x |
| wide_arrays.json | json | 35.661 | 38.576 | 42.123 | 57.387 | 0.05x |
| mixed.json | strata | 0.081 | 0.093 | 0.116 | 52.559 | 1.00x |
| mixed.json | orjson | 0.093 | 0.101 | 0.131 | 52.559 | 0.92x |
| mixed.json | msgspec | 0.132 | 0.141 | 0.159 | 52.559 | 0.66x |
| mixed.json | ujson | 0.642 | 0.717 | 0.892 | 52.559 | 0.13x |
| mixed.json | json | 1.016 | 1.063 | 1.288 | 52.559 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 18.509 | 19.030 | 20.417 | 61.000 | 1.00x |
| users.json | orjson | 24.926 | 25.925 | 27.006 | 61.000 | 0.73x |
| users.json | msgspec | 25.054 | 26.015 | 27.538 | 61.000 | 0.73x |
| users.json | ujson | 36.762 | 38.041 | 40.818 | 61.000 | 0.50x |
| users.json | json | 40.738 | 42.190 | 44.212 | 61.000 | 0.45x |
| flat.json | strata | 1.433 | 1.521 | 1.656 | 54.445 | 1.00x |
| flat.json | orjson | 1.531 | 1.706 | 1.914 | 54.445 | 0.89x |
| flat.json | msgspec | 1.780 | 1.925 | 2.171 | 54.445 | 0.79x |
| flat.json | ujson | 3.052 | 3.271 | 4.836 | 54.445 | 0.47x |
| flat.json | json | 3.331 | 3.523 | 5.199 | 54.445 | 0.43x |
| nested.json | strata | 1.657 | 1.688 | 1.718 | 49.484 | 1.00x |
| nested.json | orjson | 1.898 | 1.953 | 1.991 | 49.484 | 0.86x |
| nested.json | msgspec | 2.070 | 2.152 | 2.813 | 49.484 | 0.78x |
| nested.json | ujson | 3.370 | 3.431 | 3.521 | 49.484 | 0.49x |
| nested.json | json | 4.058 | 4.221 | 4.394 | 49.484 | 0.40x |
| wide_arrays.json | strata | 9.184 | 9.455 | 12.872 | 57.387 | 1.00x |
| wide_arrays.json | orjson | 10.219 | 10.619 | 14.640 | 57.387 | 0.89x |
| wide_arrays.json | msgspec | 11.306 | 11.492 | 13.565 | 57.387 | 0.82x |
| wide_arrays.json | ujson | 14.726 | 15.103 | 20.531 | 57.387 | 0.63x |
| wide_arrays.json | json | 18.381 | 19.255 | 28.375 | 57.387 | 0.49x |
| mixed.json | strata | 0.480 | 0.512 | 0.590 | 52.559 | 1.00x |
| mixed.json | orjson | 0.603 | 0.660 | 0.764 | 52.559 | 0.77x |
| mixed.json | msgspec | 0.652 | 0.683 | 1.008 | 52.559 | 0.75x |
| mixed.json | ujson | 0.824 | 0.875 | 1.119 | 52.559 | 0.58x |
| mixed.json | json | 1.060 | 1.098 | 1.218 | 52.559 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 20.524 | 21.184 | 22.779 | 56.961 | 1.00x |
| users.ndjson | orjson | 28.072 | 29.299 | 34.849 | 56.961 | 0.72x |
| users.ndjson | msgspec | 27.567 | 29.537 | 33.327 | 56.961 | 0.72x |
| users.ndjson | ujson | 39.877 | 43.547 | 46.802 | 56.961 | 0.49x |
| users.ndjson | json | 50.516 | 52.639 | 58.786 | 56.961 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.307 | 3.713 | 4.916 | 56.418 | 1.00x |
| users.json | orjson | 3.870 | 4.397 | 6.521 | 56.418 | 0.84x |
| users.json | msgspec | 5.963 | 7.168 | 10.040 | 56.418 | 0.52x |
| users.json | ujson | 30.558 | 33.509 | 35.643 | 56.418 | 0.11x |
| users.json | json | 40.982 | 42.368 | 47.171 | 56.418 | 0.09x |
| flat.json | strata | 0.696 | 0.746 | 0.821 | 54.445 | 1.00x |
| flat.json | orjson | 0.821 | 0.879 | 1.025 | 54.445 | 0.85x |
| flat.json | msgspec | 0.972 | 1.036 | 1.127 | 54.445 | 0.72x |
| flat.json | ujson | 3.162 | 3.288 | 3.751 | 54.445 | 0.23x |
| flat.json | json | 4.111 | 4.416 | 4.865 | 54.445 | 0.17x |
| nested.json | strata | 0.619 | 0.656 | 0.756 | 49.484 | 1.00x |
| nested.json | orjson | 0.739 | 0.840 | 1.001 | 49.484 | 0.78x |
| nested.json | msgspec | 0.993 | 1.047 | 1.235 | 49.484 | 0.63x |
| nested.json | ujson | 3.340 | 3.499 | 3.908 | 49.484 | 0.19x |
| nested.json | json | 5.215 | 5.404 | 6.096 | 49.484 | 0.12x |
| wide_arrays.json | strata | 2.685 | 3.420 | 3.979 | 57.387 | 1.00x |
| wide_arrays.json | orjson | 3.523 | 4.502 | 5.220 | 57.387 | 0.76x |
| wide_arrays.json | msgspec | 4.289 | 5.184 | 6.136 | 57.387 | 0.66x |
| wide_arrays.json | ujson | 17.545 | 20.168 | 23.402 | 57.387 | 0.17x |
| wide_arrays.json | json | 37.279 | 44.026 | 50.716 | 57.387 | 0.08x |
| mixed.json | strata | 0.389 | 0.463 | 0.585 | 52.559 | 1.00x |
| mixed.json | orjson | 0.443 | 0.498 | 0.583 | 52.559 | 0.93x |
| mixed.json | msgspec | 0.445 | 0.492 | 0.561 | 52.559 | 0.94x |
| mixed.json | ujson | 1.010 | 1.088 | 1.187 | 52.559 | 0.43x |
| mixed.json | json | 1.361 | 1.392 | 1.687 | 52.559 | 0.33x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.138 | 0.152 | 0.192 | 56.473 | 1.00x |
| users.json $[*].id | jmespath | 0.909 | 0.950 | 1.009 | 56.473 | 0.16x |
| users.json $[*].id | jsonpath-ng | 4.949 | 4.979 | 5.177 | 56.473 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.920 | 0.977 | 1.072 | 56.355 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 5.707 | 5.811 | 6.715 | 56.355 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 33.571 | 34.553 | 35.748 | 56.355 | 0.03x |
| users.json $..total | strata | 3.006 | 3.445 | 4.029 | 57.484 | 1.00x |
| users.json $..total | jsonpath-ng | 671.479 | 691.818 | 1068.268 | 57.484 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.622 | 3.707 | 4.475 | 60.504 | 1.00x |
| users.json $[*].id | orjson+jmespath | 24.913 | 26.614 | 30.370 | 60.504 | 0.14x |
| users.json $[*].id | orjson+jsonpath-ng | 29.164 | 31.217 | 37.241 | 60.504 | 0.12x |
| users.json $[*].orders[*].total | strata | 4.061 | 4.257 | 4.638 | 56.648 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 31.030 | 34.913 | 35.909 | 56.648 | 0.12x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 64.082 | 74.110 | 85.610 | 56.648 | 0.06x |
| users.json $..total | strata | 23.828 | 25.112 | 30.810 | 56.730 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 758.190 | 782.548 | 930.025 | 56.730 | 0.03x |

