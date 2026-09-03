# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e44489e
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.5.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.669 | 8.080 | 14.169 | 63.719 | 1.00x |
| users.json | orjson | 11.222 | 11.836 | 19.766 | 63.719 | 0.68x |
| users.json | msgspec | 10.723 | 12.284 | 48.045 | 63.719 | 0.66x |
| users.json | ujson | 15.387 | 17.760 | 35.282 | 63.719 | 0.45x |
| users.json | pysimdjson | 150.494 | 174.396 | 276.276 | 63.719 | 0.05x |
| users.json | json | 17.348 | 21.464 | 29.076 | 63.719 | 0.38x |
| flat.json | strata | 0.581 | 0.626 | 0.750 | 90.406 | 1.00x |
| flat.json | orjson | 0.689 | 0.792 | 0.857 | 90.406 | 0.79x |
| flat.json | msgspec | 0.666 | 0.731 | 0.788 | 90.406 | 0.86x |
| flat.json | ujson | 1.108 | 1.302 | 1.437 | 90.406 | 0.48x |
| flat.json | pysimdjson | 11.282 | 12.359 | 14.385 | 90.406 | 0.05x |
| flat.json | json | 1.318 | 1.421 | 1.560 | 90.406 | 0.44x |
| nested.json | strata | 0.550 | 0.595 | 0.630 | 90.422 | 1.00x |
| nested.json | orjson | 0.743 | 0.810 | 1.800 | 90.422 | 0.73x |
| nested.json | msgspec | 0.717 | 0.798 | 1.198 | 90.422 | 0.74x |
| nested.json | ujson | 1.152 | 1.201 | 1.671 | 90.422 | 0.50x |
| nested.json | pysimdjson | 10.611 | 11.299 | 12.501 | 90.422 | 0.05x |
| nested.json | json | 1.463 | 1.562 | 1.708 | 90.422 | 0.38x |
| wide_arrays.json | strata | 3.492 | 4.105 | 6.124 | 93.172 | 1.00x |
| wide_arrays.json | orjson | 3.920 | 4.651 | 5.267 | 93.172 | 0.88x |
| wide_arrays.json | msgspec | 4.518 | 5.206 | 5.638 | 93.172 | 0.79x |
| wide_arrays.json | ujson | 6.350 | 6.704 | 8.363 | 93.172 | 0.61x |
| wide_arrays.json | pysimdjson | 66.930 | 74.450 | 96.499 | 93.172 | 0.06x |
| wide_arrays.json | json | 6.940 | 7.710 | 9.072 | 93.172 | 0.53x |
| mixed.json | strata | 0.121 | 0.122 | 0.161 | 93.188 | 1.00x |
| mixed.json | orjson | 0.143 | 0.155 | 0.195 | 93.188 | 0.79x |
| mixed.json | msgspec | 0.157 | 0.163 | 0.225 | 93.188 | 0.75x |
| mixed.json | ujson | 0.193 | 0.260 | 0.425 | 93.188 | 0.47x |
| mixed.json | pysimdjson | 2.343 | 2.369 | 2.834 | 93.188 | 0.05x |
| mixed.json | json | 0.300 | 0.307 | 0.373 | 93.188 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.746 | 1.885 | 2.831 | 76.344 | 1.00x |
| users.json | orjson | 2.491 | 2.582 | 3.304 | 76.344 | 0.73x |
| users.json | msgspec | 3.155 | 3.394 | 4.596 | 76.344 | 0.56x |
| users.json | ujson | 11.979 | 12.388 | 16.886 | 76.344 | 0.15x |
| users.json | json | 17.088 | 17.864 | 24.448 | 76.344 | 0.11x |
| flat.json | strata | 0.169 | 0.188 | 0.234 | 90.422 | 1.00x |
| flat.json | orjson | 0.235 | 0.264 | 0.330 | 90.422 | 0.71x |
| flat.json | msgspec | 0.292 | 0.313 | 0.371 | 90.422 | 0.60x |
| flat.json | ujson | 0.878 | 0.925 | 1.162 | 90.422 | 0.20x |
| flat.json | json | 1.275 | 1.379 | 1.759 | 90.422 | 0.14x |
| nested.json | strata | 0.122 | 0.144 | 0.232 | 90.422 | 1.00x |
| nested.json | orjson | 0.229 | 0.259 | 0.482 | 90.422 | 0.55x |
| nested.json | msgspec | 0.285 | 0.326 | 0.855 | 90.422 | 0.44x |
| nested.json | ujson | 1.136 | 1.265 | 1.554 | 90.422 | 0.11x |
| nested.json | json | 1.670 | 1.833 | 3.759 | 90.422 | 0.08x |
| wide_arrays.json | strata | 1.171 | 1.234 | 1.661 | 93.172 | 1.00x |
| wide_arrays.json | orjson | 1.577 | 1.666 | 3.998 | 93.172 | 0.74x |
| wide_arrays.json | msgspec | 2.186 | 2.341 | 2.813 | 93.172 | 0.53x |
| wide_arrays.json | ujson | 6.991 | 7.547 | 8.397 | 93.172 | 0.16x |
| wide_arrays.json | json | 12.237 | 13.330 | 18.334 | 93.172 | 0.09x |
| mixed.json | strata | 0.043 | 0.058 | 0.073 | 93.188 | 1.00x |
| mixed.json | orjson | 0.043 | 0.057 | 0.078 | 93.188 | 1.03x |
| mixed.json | msgspec | 0.056 | 0.187 | 0.400 | 93.188 | 0.31x |
| mixed.json | ujson | 0.226 | 0.259 | 0.349 | 93.188 | 0.22x |
| mixed.json | json | 0.360 | 0.389 | 0.616 | 93.188 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.716 | 8.774 | 16.858 | 86.906 | 1.00x |
| users.json | orjson | 11.199 | 12.978 | 18.981 | 86.906 | 0.68x |
| users.json | msgspec | 10.706 | 12.060 | 25.975 | 86.906 | 0.73x |
| users.json | ujson | 15.755 | 17.735 | 22.438 | 86.906 | 0.49x |
| users.json | json | 16.966 | 19.286 | 21.309 | 86.906 | 0.45x |
| flat.json | strata | 0.652 | 0.728 | 0.920 | 90.422 | 1.00x |
| flat.json | orjson | 0.927 | 0.998 | 1.959 | 90.422 | 0.73x |
| flat.json | msgspec | 0.769 | 0.930 | 1.185 | 90.422 | 0.78x |
| flat.json | ujson | 1.224 | 1.336 | 1.545 | 90.422 | 0.54x |
| flat.json | json | 1.424 | 1.624 | 2.041 | 90.422 | 0.45x |
| nested.json | strata | 0.654 | 0.790 | 1.085 | 90.422 | 1.00x |
| nested.json | orjson | 1.110 | 1.314 | 2.490 | 90.422 | 0.60x |
| nested.json | msgspec | 0.952 | 1.165 | 1.693 | 90.422 | 0.68x |
| nested.json | ujson | 1.258 | 1.412 | 2.894 | 90.422 | 0.56x |
| nested.json | json | 1.697 | 1.874 | 2.140 | 90.422 | 0.42x |
| wide_arrays.json | strata | 3.793 | 4.892 | 9.628 | 93.172 | 1.00x |
| wide_arrays.json | orjson | 4.094 | 4.601 | 8.640 | 93.172 | 1.06x |
| wide_arrays.json | msgspec | 4.813 | 5.371 | 6.454 | 93.172 | 0.91x |
| wide_arrays.json | ujson | 6.328 | 6.789 | 12.483 | 93.172 | 0.72x |
| wide_arrays.json | json | 7.583 | 8.729 | 13.068 | 93.172 | 0.56x |
| mixed.json | strata | 0.158 | 0.172 | 0.187 | 93.188 | 1.00x |
| mixed.json | orjson | 0.196 | 0.217 | 0.301 | 93.188 | 0.80x |
| mixed.json | msgspec | 0.208 | 0.219 | 0.528 | 93.188 | 0.79x |
| mixed.json | ujson | 0.259 | 0.277 | 0.303 | 93.188 | 0.62x |
| mixed.json | json | 0.372 | 0.388 | 0.400 | 93.188 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.759 | 9.802 | 14.818 | 90.391 | 1.00x |
| users.ndjson | orjson | 11.399 | 13.348 | 25.762 | 90.391 | 0.73x |
| users.ndjson | msgspec | 11.051 | 13.890 | 26.437 | 90.391 | 0.71x |
| users.ndjson | ujson | 13.735 | 16.244 | 42.586 | 90.391 | 0.60x |
| users.ndjson | json | 20.245 | 22.910 | 41.600 | 90.391 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.081 | 2.323 | 2.465 | 85.656 | 1.00x |
| users.json | orjson | 2.894 | 3.276 | 4.678 | 85.656 | 0.71x |
| users.json | msgspec | 3.489 | 3.765 | 4.515 | 85.656 | 0.62x |
| users.json | ujson | 11.832 | 12.482 | 13.378 | 85.656 | 0.19x |
| users.json | json | 16.843 | 18.994 | 29.565 | 85.656 | 0.12x |
| flat.json | strata | 0.495 | 0.604 | 0.959 | 90.422 | 1.00x |
| flat.json | orjson | 0.487 | 0.771 | 1.298 | 90.422 | 0.78x |
| flat.json | msgspec | 0.684 | 0.844 | 1.565 | 90.422 | 0.72x |
| flat.json | ujson | 1.386 | 1.688 | 3.055 | 90.422 | 0.36x |
| flat.json | json | 1.873 | 2.464 | 2.843 | 90.422 | 0.25x |
| nested.json | strata | 0.355 | 0.454 | 0.753 | 90.422 | 1.00x |
| nested.json | orjson | 0.510 | 0.577 | 0.758 | 90.422 | 0.79x |
| nested.json | msgspec | 0.623 | 0.724 | 1.120 | 90.422 | 0.63x |
| nested.json | ujson | 1.419 | 1.673 | 2.672 | 90.422 | 0.27x |
| nested.json | json | 1.914 | 2.262 | 2.753 | 90.422 | 0.20x |
| wide_arrays.json | strata | 1.713 | 1.848 | 3.980 | 93.172 | 1.00x |
| wide_arrays.json | orjson | 1.842 | 2.446 | 5.620 | 93.172 | 0.76x |
| wide_arrays.json | msgspec | 2.525 | 3.093 | 4.554 | 93.172 | 0.60x |
| wide_arrays.json | ujson | 7.058 | 8.101 | 19.381 | 93.172 | 0.23x |
| wide_arrays.json | json | 11.818 | 13.998 | 16.079 | 93.172 | 0.13x |
| mixed.json | strata | 0.111 | 0.132 | 0.381 | 93.188 | 1.00x |
| mixed.json | orjson | 0.132 | 0.257 | 0.459 | 93.188 | 0.52x |
| mixed.json | msgspec | 0.127 | 0.155 | 0.657 | 93.188 | 0.85x |
| mixed.json | ujson | 0.289 | 0.324 | 1.082 | 93.188 | 0.41x |
| mixed.json | json | 0.422 | 0.458 | 0.766 | 93.188 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.062 | 0.085 | 0.319 | 85.734 | 1.00x |
| users.json $[*].id | jmespath | 0.308 | 0.366 | 1.062 | 85.734 | 0.23x |
| users.json $[*].id | jsonpath-ng | 1.536 | 1.833 | 3.491 | 85.734 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.482 | 0.685 | 0.867 | 85.906 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.800 | 2.226 | 3.034 | 85.906 | 0.31x |
| users.json $[*].orders[*].total | jsonpath-ng | 11.235 | 13.322 | 22.932 | 85.906 | 0.05x |
| users.json $..total | strata | 1.275 | 1.526 | 1.807 | 86.922 | 1.00x |
| users.json $..total | jsonpath-ng | 210.170 | 217.395 | 276.215 | 86.922 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.028 | 4.234 | 4.672 | 85.781 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.559 | 12.224 | 18.108 | 85.781 | 0.35x |
| users.json $[*].id | orjson+jsonpath-ng | 12.955 | 13.710 | 18.130 | 85.781 | 0.31x |
| users.json $[*].orders[*].total | strata | 4.044 | 4.225 | 5.731 | 86.906 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.540 | 13.880 | 20.753 | 86.906 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 28.561 | 29.388 | 39.761 | 86.906 | 0.14x |
| users.json $..total | strata | 9.125 | 9.536 | 10.062 | 86.938 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 215.766 | 226.521 | 235.515 | 86.938 | 0.04x |

