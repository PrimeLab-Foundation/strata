# Benchmark results - ci-macos-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 208e6f9
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.7-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 25.783 | 30.196 | 34.915 | 52.555 | 1.00x |
| users.json | orjson | 35.019 | 40.620 | 47.795 | 52.555 | 0.74x |
| users.json | msgspec | 37.640 | 42.532 | 49.537 | 52.555 | 0.71x |
| users.json | ujson | 50.500 | 56.382 | 68.114 | 52.555 | 0.54x |
| users.json | pysimdjson | 220.700 | 235.461 | 279.676 | 52.555 | 0.13x |
| users.json | json | 60.132 | 63.074 | 69.812 | 52.555 | 0.48x |
| flat.json | strata | 1.807 | 1.830 | 2.911 | 62.004 | 1.00x |
| flat.json | orjson | 1.916 | 1.936 | 3.066 | 62.004 | 0.95x |
| flat.json | msgspec | 2.170 | 2.206 | 2.535 | 62.004 | 0.83x |
| flat.json | ujson | 3.815 | 3.848 | 4.701 | 62.004 | 0.48x |
| flat.json | pysimdjson | 20.006 | 20.207 | 23.255 | 62.004 | 0.09x |
| flat.json | json | 4.291 | 4.340 | 6.136 | 62.004 | 0.42x |
| nested.json | strata | 2.043 | 2.220 | 3.504 | 57.301 | 1.00x |
| nested.json | orjson | 2.325 | 2.883 | 3.564 | 57.301 | 0.77x |
| nested.json | msgspec | 2.603 | 3.085 | 3.878 | 57.301 | 0.72x |
| nested.json | ujson | 4.309 | 4.695 | 7.413 | 57.301 | 0.47x |
| nested.json | pysimdjson | 18.252 | 19.676 | 22.253 | 57.301 | 0.11x |
| nested.json | json | 5.364 | 5.872 | 7.797 | 57.301 | 0.38x |
| wide_arrays.json | strata | 11.676 | 12.958 | 17.408 | 61.586 | 1.00x |
| wide_arrays.json | orjson | 12.352 | 13.269 | 21.460 | 61.586 | 0.98x |
| wide_arrays.json | msgspec | 13.752 | 15.766 | 19.911 | 61.586 | 0.82x |
| wide_arrays.json | ujson | 17.913 | 22.449 | 25.722 | 61.586 | 0.58x |
| wide_arrays.json | pysimdjson | 110.338 | 119.165 | 220.180 | 61.586 | 0.11x |
| wide_arrays.json | json | 23.221 | 23.845 | 41.571 | 61.586 | 0.54x |
| mixed.json | strata | 0.521 | 0.537 | 0.597 | 58.809 | 1.00x |
| mixed.json | orjson | 0.606 | 0.615 | 0.661 | 58.809 | 0.87x |
| mixed.json | msgspec | 0.637 | 0.649 | 0.849 | 58.809 | 0.83x |
| mixed.json | ujson | 0.878 | 0.898 | 1.101 | 58.809 | 0.60x |
| mixed.json | pysimdjson | 4.379 | 4.439 | 5.035 | 58.809 | 0.12x |
| mixed.json | json | 1.248 | 1.281 | 1.640 | 58.809 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.625 | 3.699 | 4.084 | 51.371 | 1.00x |
| users.json | orjson | 4.480 | 4.683 | 5.460 | 51.371 | 0.79x |
| users.json | msgspec | 7.008 | 7.236 | 7.886 | 51.371 | 0.51x |
| users.json | ujson | 41.341 | 43.638 | 46.800 | 51.371 | 0.08x |
| users.json | json | 53.742 | 55.885 | 60.925 | 51.371 | 0.07x |
| flat.json | strata | 0.410 | 0.423 | 0.462 | 57.168 | 1.00x |
| flat.json | orjson | 0.547 | 0.583 | 0.714 | 57.168 | 0.73x |
| flat.json | msgspec | 0.740 | 0.774 | 0.819 | 57.168 | 0.55x |
| flat.json | ujson | 3.678 | 3.864 | 4.605 | 57.168 | 0.11x |
| flat.json | json | 5.041 | 5.186 | 8.557 | 57.168 | 0.08x |
| nested.json | strata | 0.327 | 0.388 | 0.457 | 57.445 | 1.00x |
| nested.json | orjson | 0.488 | 0.545 | 0.833 | 57.445 | 0.71x |
| nested.json | msgspec | 0.783 | 0.830 | 1.062 | 57.445 | 0.47x |
| nested.json | ujson | 3.698 | 4.127 | 5.278 | 57.445 | 0.09x |
| nested.json | json | 6.281 | 6.983 | 8.157 | 57.445 | 0.06x |
| wide_arrays.json | strata | 2.697 | 2.863 | 3.184 | 59.680 | 1.00x |
| wide_arrays.json | orjson | 3.670 | 3.880 | 4.966 | 59.680 | 0.74x |
| wide_arrays.json | msgspec | 4.461 | 4.727 | 5.331 | 59.680 | 0.61x |
| wide_arrays.json | ujson | 21.001 | 22.473 | 26.467 | 59.680 | 0.13x |
| wide_arrays.json | json | 46.948 | 48.143 | 55.116 | 59.680 | 0.06x |
| mixed.json | strata | 0.115 | 0.127 | 0.145 | 56.672 | 1.00x |
| mixed.json | orjson | 0.115 | 0.130 | 0.179 | 56.672 | 0.97x |
| mixed.json | msgspec | 0.161 | 0.184 | 0.207 | 56.672 | 0.69x |
| mixed.json | ujson | 0.857 | 1.006 | 1.319 | 56.672 | 0.13x |
| mixed.json | json | 1.360 | 1.518 | 1.917 | 56.672 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 25.368 | 28.896 | 30.587 | 61.227 | 1.00x |
| users.json | orjson | 35.675 | 38.905 | 44.613 | 61.227 | 0.74x |
| users.json | msgspec | 35.327 | 39.052 | 43.590 | 61.227 | 0.74x |
| users.json | ujson | 50.945 | 56.909 | 65.265 | 61.227 | 0.51x |
| users.json | json | 59.107 | 60.974 | 65.854 | 61.227 | 0.47x |
| flat.json | strata | 1.938 | 2.037 | 2.732 | 57.168 | 1.00x |
| flat.json | orjson | 2.119 | 2.191 | 2.661 | 57.168 | 0.93x |
| flat.json | msgspec | 2.417 | 2.521 | 3.425 | 57.168 | 0.81x |
| flat.json | ujson | 4.042 | 4.301 | 5.303 | 57.168 | 0.47x |
| flat.json | json | 4.492 | 5.280 | 6.147 | 57.168 | 0.39x |
| nested.json | strata | 2.169 | 2.287 | 2.937 | 57.445 | 1.00x |
| nested.json | orjson | 2.479 | 2.656 | 2.833 | 57.445 | 0.86x |
| nested.json | msgspec | 2.694 | 2.760 | 3.472 | 57.445 | 0.83x |
| nested.json | ujson | 4.330 | 4.466 | 5.762 | 57.445 | 0.51x |
| nested.json | json | 5.400 | 6.006 | 7.542 | 57.445 | 0.38x |
| wide_arrays.json | strata | 12.058 | 12.497 | 15.715 | 60.906 | 1.00x |
| wide_arrays.json | orjson | 13.044 | 13.668 | 15.704 | 60.906 | 0.91x |
| wide_arrays.json | msgspec | 14.222 | 15.850 | 17.372 | 60.906 | 0.79x |
| wide_arrays.json | ujson | 18.454 | 20.275 | 23.724 | 60.906 | 0.62x |
| wide_arrays.json | json | 23.449 | 25.928 | 30.995 | 60.906 | 0.48x |
| mixed.json | strata | 0.640 | 0.653 | 0.696 | 56.672 | 1.00x |
| mixed.json | orjson | 0.784 | 0.795 | 1.002 | 56.672 | 0.82x |
| mixed.json | msgspec | 0.810 | 0.836 | 0.889 | 56.672 | 0.78x |
| mixed.json | ujson | 1.060 | 1.085 | 1.171 | 56.672 | 0.60x |
| mixed.json | json | 1.365 | 1.400 | 1.652 | 56.672 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 26.801 | 28.411 | 31.939 | 60.410 | 1.00x |
| users.ndjson | orjson | 35.835 | 38.813 | 47.353 | 60.410 | 0.73x |
| users.ndjson | msgspec | 36.445 | 37.950 | 42.552 | 60.410 | 0.75x |
| users.ndjson | ujson | 53.320 | 57.011 | 70.657 | 60.410 | 0.50x |
| users.ndjson | json | 65.324 | 76.824 | 91.833 | 60.410 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.794 | 6.154 | 8.240 | 57.312 | 1.00x |
| users.json | orjson | 5.953 | 7.316 | 8.902 | 57.312 | 0.84x |
| users.json | msgspec | 8.359 | 9.819 | 12.595 | 57.312 | 0.63x |
| users.json | ujson | 47.722 | 51.110 | 57.671 | 57.312 | 0.12x |
| users.json | json | 61.711 | 71.802 | 82.944 | 57.312 | 0.09x |
| flat.json | strata | 0.789 | 0.823 | 0.901 | 57.168 | 1.00x |
| flat.json | orjson | 0.924 | 1.068 | 1.341 | 57.168 | 0.77x |
| flat.json | msgspec | 1.120 | 1.205 | 1.248 | 57.168 | 0.68x |
| flat.json | ujson | 3.976 | 4.202 | 4.272 | 57.168 | 0.20x |
| flat.json | json | 5.341 | 5.509 | 6.120 | 57.168 | 0.15x |
| nested.json | strata | 0.749 | 0.880 | 1.138 | 57.445 | 1.00x |
| nested.json | orjson | 0.925 | 1.073 | 1.454 | 57.445 | 0.82x |
| nested.json | msgspec | 1.209 | 1.503 | 1.842 | 57.445 | 0.59x |
| nested.json | ujson | 4.297 | 4.882 | 7.002 | 57.445 | 0.18x |
| nested.json | json | 7.296 | 8.350 | 10.965 | 57.445 | 0.11x |
| wide_arrays.json | strata | 3.613 | 4.078 | 4.970 | 59.746 | 1.00x |
| wide_arrays.json | orjson | 4.106 | 4.516 | 5.220 | 59.746 | 0.90x |
| wide_arrays.json | msgspec | 5.611 | 6.106 | 7.859 | 59.746 | 0.67x |
| wide_arrays.json | ujson | 21.972 | 24.745 | 28.398 | 59.746 | 0.16x |
| wide_arrays.json | json | 47.554 | 51.866 | 59.237 | 59.746 | 0.08x |
| mixed.json | strata | 0.512 | 0.561 | 0.759 | 56.672 | 1.00x |
| mixed.json | orjson | 0.529 | 0.632 | 0.865 | 56.672 | 0.89x |
| mixed.json | msgspec | 0.598 | 0.659 | 0.984 | 56.672 | 0.85x |
| mixed.json | ujson | 1.373 | 1.462 | 2.083 | 56.672 | 0.38x |
| mixed.json | json | 1.829 | 2.013 | 2.516 | 56.672 | 0.28x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.218 | 0.233 | 0.364 | 57.391 | 1.00x |
| users.json $[*].id | jmespath | 1.344 | 1.368 | 1.953 | 57.391 | 0.17x |
| users.json $[*].id | jsonpath-ng | 7.214 | 7.467 | 11.805 | 57.391 | 0.03x |
| users.json $[*].orders[*].total | strata | 1.335 | 1.484 | 1.927 | 55.684 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 8.273 | 8.794 | 10.752 | 55.684 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 49.485 | 53.830 | 66.165 | 55.684 | 0.03x |
| users.json $..total | strata | 4.830 | 5.452 | 6.991 | 57.688 | 1.00x |
| users.json $..total | jsonpath-ng | 986.029 | 1066.950 | 1291.455 | 57.688 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 5.838 | 6.358 | 7.157 | 58.445 | 1.00x |
| users.json $[*].id | orjson+jmespath | 38.016 | 40.007 | 44.935 | 58.445 | 0.16x |
| users.json $[*].id | orjson+jsonpath-ng | 42.953 | 47.506 | 52.758 | 58.445 | 0.13x |
| users.json $[*].orders[*].total | strata | 5.918 | 6.473 | 7.978 | 56.953 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 43.350 | 46.646 | 55.890 | 56.953 | 0.14x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 92.034 | 98.605 | 111.353 | 56.953 | 0.07x |
| users.json $..total | strata | 27.784 | 31.278 | 35.994 | 56.984 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 909.290 | 963.559 | 1088.508 | 56.984 | 0.03x |

