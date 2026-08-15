# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c36c406
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
| users.json | strata | 8.019 | 9.019 | 13.834 | 63.438 | 1.00x |
| users.json | orjson | 10.216 | 11.237 | 14.060 | 63.438 | 0.80x |
| users.json | msgspec | 9.592 | 11.421 | 14.907 | 63.438 | 0.79x |
| users.json | ujson | 13.950 | 16.031 | 21.803 | 63.438 | 0.56x |
| users.json | pysimdjson | 136.466 | 148.808 | 183.055 | 63.438 | 0.06x |
| users.json | json | 16.966 | 18.771 | 26.685 | 63.438 | 0.48x |
| flat.json | strata | 0.741 | 0.825 | 0.916 | 88.031 | 1.00x |
| flat.json | orjson | 0.868 | 0.982 | 1.246 | 88.031 | 0.84x |
| flat.json | msgspec | 0.829 | 0.937 | 1.282 | 88.031 | 0.88x |
| flat.json | ujson | 1.501 | 1.557 | 1.852 | 88.031 | 0.53x |
| flat.json | pysimdjson | 13.967 | 14.330 | 15.715 | 88.031 | 0.06x |
| flat.json | json | 1.451 | 1.718 | 2.658 | 88.031 | 0.48x |
| nested.json | strata | 0.547 | 0.654 | 1.294 | 88.062 | 1.00x |
| nested.json | orjson | 0.688 | 0.840 | 1.054 | 88.062 | 0.78x |
| nested.json | msgspec | 0.637 | 0.766 | 1.000 | 88.062 | 0.85x |
| nested.json | ujson | 1.011 | 1.359 | 2.261 | 88.062 | 0.48x |
| nested.json | pysimdjson | 9.828 | 11.039 | 12.188 | 88.062 | 0.06x |
| nested.json | json | 1.327 | 1.597 | 2.774 | 88.062 | 0.41x |
| wide_arrays.json | strata | 3.896 | 4.541 | 6.089 | 90.703 | 1.00x |
| wide_arrays.json | orjson | 4.493 | 6.260 | 7.209 | 90.703 | 0.73x |
| wide_arrays.json | msgspec | 4.790 | 5.028 | 8.306 | 90.703 | 0.90x |
| wide_arrays.json | ujson | 6.286 | 7.793 | 9.348 | 90.703 | 0.58x |
| wide_arrays.json | pysimdjson | 74.160 | 82.020 | 87.530 | 90.703 | 0.06x |
| wide_arrays.json | json | 7.795 | 8.661 | 11.617 | 90.703 | 0.52x |
| mixed.json | strata | 0.160 | 0.175 | 0.457 | 90.719 | 1.00x |
| mixed.json | orjson | 0.168 | 0.191 | 0.484 | 90.719 | 0.92x |
| mixed.json | msgspec | 0.194 | 0.207 | 0.523 | 90.719 | 0.85x |
| mixed.json | ujson | 0.240 | 0.262 | 1.249 | 90.719 | 0.67x |
| mixed.json | pysimdjson | 2.698 | 2.794 | 4.871 | 90.719 | 0.06x |
| mixed.json | json | 0.354 | 0.388 | 0.959 | 90.719 | 0.45x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.187 | 2.641 | 3.754 | 77.688 | 1.00x |
| users.json | orjson | 2.484 | 2.779 | 7.363 | 77.688 | 0.95x |
| users.json | msgspec | 3.155 | 3.717 | 5.506 | 77.688 | 0.71x |
| users.json | ujson | 12.040 | 13.944 | 20.252 | 77.688 | 0.19x |
| users.json | json | 17.083 | 19.545 | 42.545 | 77.688 | 0.14x |
| flat.json | strata | 0.236 | 0.262 | 0.633 | 88.062 | 1.00x |
| flat.json | orjson | 0.284 | 0.337 | 0.870 | 88.062 | 0.78x |
| flat.json | msgspec | 0.354 | 0.392 | 0.648 | 88.062 | 0.67x |
| flat.json | ujson | 1.018 | 1.094 | 2.593 | 88.062 | 0.24x |
| flat.json | json | 1.488 | 1.636 | 3.917 | 88.062 | 0.16x |
| nested.json | strata | 0.173 | 0.202 | 0.425 | 88.062 | 1.00x |
| nested.json | orjson | 0.243 | 0.286 | 0.346 | 88.062 | 0.71x |
| nested.json | msgspec | 0.367 | 0.477 | 0.583 | 88.062 | 0.42x |
| nested.json | ujson | 1.121 | 1.232 | 1.745 | 88.062 | 0.16x |
| nested.json | json | 1.748 | 1.943 | 2.381 | 88.062 | 0.10x |
| wide_arrays.json | strata | 1.454 | 1.496 | 2.336 | 90.703 | 1.00x |
| wide_arrays.json | orjson | 1.580 | 1.787 | 2.280 | 90.703 | 0.84x |
| wide_arrays.json | msgspec | 2.432 | 2.632 | 3.338 | 90.703 | 0.57x |
| wide_arrays.json | ujson | 7.037 | 7.716 | 12.441 | 90.703 | 0.19x |
| wide_arrays.json | json | 13.155 | 14.412 | 15.627 | 90.703 | 0.10x |
| mixed.json | strata | 0.057 | 0.068 | 0.261 | 90.719 | 1.00x |
| mixed.json | orjson | 0.053 | 0.059 | 0.246 | 90.719 | 1.15x |
| mixed.json | msgspec | 0.061 | 0.074 | 0.104 | 90.719 | 0.92x |
| mixed.json | ujson | 0.244 | 0.269 | 0.800 | 90.719 | 0.25x |
| mixed.json | json | 0.401 | 0.427 | 1.082 | 90.719 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.382 | 8.753 | 18.956 | 88.062 | 1.00x |
| users.json | orjson | 10.744 | 12.422 | 14.094 | 88.062 | 0.70x |
| users.json | msgspec | 10.645 | 17.528 | 23.266 | 88.062 | 0.50x |
| users.json | ujson | 16.098 | 19.087 | 28.596 | 88.062 | 0.46x |
| users.json | json | 18.191 | 21.524 | 42.055 | 88.062 | 0.41x |
| flat.json | strata | 0.809 | 0.872 | 2.332 | 88.062 | 1.00x |
| flat.json | orjson | 1.131 | 1.265 | 2.861 | 88.062 | 0.69x |
| flat.json | msgspec | 0.871 | 0.965 | 1.074 | 88.062 | 0.90x |
| flat.json | ujson | 1.292 | 1.420 | 3.233 | 88.062 | 0.61x |
| flat.json | json | 1.583 | 1.692 | 5.366 | 88.062 | 0.52x |
| nested.json | strata | 0.710 | 0.760 | 1.182 | 88.062 | 1.00x |
| nested.json | orjson | 0.911 | 0.998 | 1.258 | 88.062 | 0.76x |
| nested.json | msgspec | 0.813 | 0.889 | 1.140 | 88.062 | 0.86x |
| nested.json | ujson | 1.046 | 1.233 | 1.729 | 88.062 | 0.62x |
| nested.json | json | 1.524 | 1.659 | 1.940 | 88.062 | 0.46x |
| wide_arrays.json | strata | 4.018 | 4.158 | 4.316 | 90.703 | 1.00x |
| wide_arrays.json | orjson | 4.241 | 4.595 | 9.079 | 90.703 | 0.90x |
| wide_arrays.json | msgspec | 4.916 | 5.799 | 8.913 | 90.703 | 0.72x |
| wide_arrays.json | ujson | 6.858 | 7.221 | 10.295 | 90.703 | 0.58x |
| wide_arrays.json | json | 7.884 | 8.242 | 15.872 | 90.703 | 0.50x |
| mixed.json | strata | 0.253 | 0.295 | 0.374 | 90.719 | 1.00x |
| mixed.json | orjson | 0.344 | 0.824 | 1.485 | 90.719 | 0.36x |
| mixed.json | msgspec | 0.341 | 0.402 | 0.831 | 90.719 | 0.74x |
| mixed.json | ujson | 0.380 | 0.470 | 1.735 | 90.719 | 0.63x |
| mixed.json | json | 0.479 | 0.569 | 0.769 | 90.719 | 0.52x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.685 | 8.981 | 13.839 | 87.375 | 1.00x |
| users.ndjson | orjson | 13.590 | 14.588 | 19.529 | 87.375 | 0.62x |
| users.ndjson | msgspec | 13.081 | 14.900 | 18.295 | 87.375 | 0.60x |
| users.ndjson | ujson | 15.759 | 17.688 | 29.209 | 87.375 | 0.51x |
| users.ndjson | json | 20.715 | 22.662 | 28.089 | 87.375 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.642 | 3.460 | 5.729 | 83.375 | 1.00x |
| users.json | orjson | 3.086 | 3.859 | 5.800 | 83.375 | 0.90x |
| users.json | msgspec | 3.549 | 3.973 | 7.778 | 83.375 | 0.87x |
| users.json | ujson | 13.171 | 14.563 | 19.585 | 83.375 | 0.24x |
| users.json | json | 17.762 | 21.170 | 28.064 | 83.375 | 0.16x |
| flat.json | strata | 0.556 | 0.612 | 0.770 | 88.062 | 1.00x |
| flat.json | orjson | 0.632 | 0.773 | 1.035 | 88.062 | 0.79x |
| flat.json | msgspec | 0.669 | 0.826 | 1.769 | 88.062 | 0.74x |
| flat.json | ujson | 1.414 | 1.527 | 3.079 | 88.062 | 0.40x |
| flat.json | json | 1.943 | 2.113 | 3.164 | 88.062 | 0.29x |
| nested.json | strata | 0.497 | 0.543 | 0.733 | 88.062 | 1.00x |
| nested.json | orjson | 0.578 | 0.622 | 0.914 | 88.062 | 0.87x |
| nested.json | msgspec | 0.795 | 1.000 | 1.410 | 88.062 | 0.54x |
| nested.json | ujson | 1.546 | 1.761 | 2.775 | 88.062 | 0.31x |
| nested.json | json | 2.145 | 2.396 | 4.091 | 88.062 | 0.23x |
| wide_arrays.json | strata | 2.136 | 2.316 | 2.933 | 90.703 | 1.00x |
| wide_arrays.json | orjson | 2.407 | 2.655 | 3.637 | 90.703 | 0.87x |
| wide_arrays.json | msgspec | 3.115 | 3.432 | 7.509 | 90.703 | 0.67x |
| wide_arrays.json | ujson | 8.214 | 9.078 | 18.244 | 90.703 | 0.26x |
| wide_arrays.json | json | 13.009 | 14.309 | 20.176 | 90.703 | 0.16x |
| mixed.json | strata | 0.355 | 0.819 | 6.340 | 90.719 | 1.00x |
| mixed.json | orjson | 0.333 | 0.731 | 6.581 | 90.719 | 1.12x |
| mixed.json | msgspec | 0.301 | 0.818 | 6.690 | 90.719 | 1.00x |
| mixed.json | ujson | 0.600 | 0.890 | 7.626 | 90.719 | 0.92x |
| mixed.json | json | 0.656 | 1.493 | 8.001 | 90.719 | 0.55x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.082 | 0.120 | 0.159 | 83.438 | 1.00x |
| users.json $[*].id | jmespath | 0.358 | 0.427 | 0.984 | 83.438 | 0.28x |
| users.json $[*].id | jsonpath-ng | 1.730 | 1.884 | 2.750 | 83.438 | 0.06x |
| users.json $[*].orders[*].total | strata | 0.385 | 0.801 | 1.123 | 84.641 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.076 | 2.321 | 5.527 | 84.641 | 0.35x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.962 | 13.785 | 26.387 | 84.641 | 0.06x |
| users.json $..total | strata | 1.438 | 1.635 | 4.111 | 86.531 | 1.00x |
| users.json $..total | jsonpath-ng | 215.547 | 243.131 | 296.613 | 86.531 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.902 | 4.086 | 4.560 | 84.500 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.839 | 12.386 | 13.893 | 84.500 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 13.240 | 13.859 | 15.021 | 84.500 | 0.29x |
| users.json $[*].orders[*].total | strata | 3.781 | 4.349 | 5.859 | 86.484 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.647 | 16.261 | 19.419 | 86.484 | 0.27x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 28.130 | 32.680 | 36.709 | 86.484 | 0.13x |
| users.json $..total | strata | 10.319 | 12.117 | 14.602 | 86.547 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 236.566 | 265.512 | 318.638 | 86.547 | 0.05x |

