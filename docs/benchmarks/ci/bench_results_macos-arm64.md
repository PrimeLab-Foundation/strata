# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 85e2353
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
| users.json | strata | 9.585 | 12.414 | 18.832 | 64.078 | 1.00x |
| users.json | orjson | 11.275 | 16.079 | 20.000 | 64.078 | 0.77x |
| users.json | msgspec | 13.433 | 19.262 | 26.045 | 64.078 | 0.64x |
| users.json | ujson | 16.673 | 26.923 | 36.741 | 64.078 | 0.46x |
| users.json | pysimdjson | 161.278 | 205.134 | 255.553 | 64.078 | 0.06x |
| users.json | json | 19.513 | 25.298 | 35.711 | 64.078 | 0.49x |
| flat.json | strata | 0.615 | 0.666 | 0.722 | 90.062 | 1.00x |
| flat.json | orjson | 0.765 | 0.852 | 2.133 | 90.062 | 0.78x |
| flat.json | msgspec | 0.705 | 0.774 | 1.844 | 90.062 | 0.86x |
| flat.json | ujson | 1.157 | 1.325 | 3.182 | 90.062 | 0.50x |
| flat.json | pysimdjson | 11.741 | 14.537 | 23.692 | 90.062 | 0.05x |
| flat.json | json | 1.367 | 1.606 | 3.656 | 90.062 | 0.41x |
| nested.json | strata | 0.581 | 0.666 | 1.715 | 90.062 | 1.00x |
| nested.json | orjson | 0.774 | 0.858 | 2.045 | 90.062 | 0.78x |
| nested.json | msgspec | 0.730 | 0.769 | 1.171 | 90.062 | 0.87x |
| nested.json | ujson | 1.116 | 1.311 | 3.083 | 90.062 | 0.51x |
| nested.json | pysimdjson | 10.712 | 12.989 | 27.586 | 90.062 | 0.05x |
| nested.json | json | 1.565 | 1.633 | 4.515 | 90.062 | 0.41x |
| wide_arrays.json | strata | 3.890 | 4.048 | 9.288 | 92.812 | 1.00x |
| wide_arrays.json | orjson | 4.016 | 4.381 | 8.475 | 92.812 | 0.92x |
| wide_arrays.json | msgspec | 4.580 | 4.860 | 6.935 | 92.812 | 0.83x |
| wide_arrays.json | ujson | 5.788 | 6.325 | 10.280 | 92.812 | 0.64x |
| wide_arrays.json | pysimdjson | 73.236 | 88.134 | 112.015 | 92.812 | 0.05x |
| wide_arrays.json | json | 7.954 | 10.526 | 21.238 | 92.812 | 0.38x |
| mixed.json | strata | 0.151 | 0.167 | 0.634 | 92.828 | 1.00x |
| mixed.json | orjson | 0.175 | 0.215 | 0.696 | 92.828 | 0.78x |
| mixed.json | msgspec | 0.187 | 0.214 | 0.752 | 92.828 | 0.78x |
| mixed.json | ujson | 0.242 | 0.397 | 1.410 | 92.828 | 0.42x |
| mixed.json | pysimdjson | 2.652 | 4.164 | 5.750 | 92.828 | 0.04x |
| mixed.json | json | 0.362 | 0.431 | 0.971 | 92.828 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.016 | 2.172 | 3.461 | 78.500 | 1.00x |
| users.json | orjson | 2.679 | 2.931 | 6.068 | 78.500 | 0.74x |
| users.json | msgspec | 3.165 | 6.338 | 8.894 | 78.500 | 0.34x |
| users.json | ujson | 12.003 | 17.979 | 25.987 | 78.500 | 0.12x |
| users.json | json | 22.792 | 28.775 | 35.000 | 78.500 | 0.08x |
| flat.json | strata | 0.230 | 0.262 | 0.662 | 90.062 | 1.00x |
| flat.json | orjson | 0.280 | 0.477 | 0.597 | 90.062 | 0.55x |
| flat.json | msgspec | 0.336 | 0.370 | 0.425 | 90.062 | 0.71x |
| flat.json | ujson | 0.984 | 1.053 | 2.802 | 90.062 | 0.25x |
| flat.json | json | 1.460 | 1.817 | 4.509 | 90.062 | 0.14x |
| nested.json | strata | 0.153 | 0.239 | 0.656 | 90.062 | 1.00x |
| nested.json | orjson | 0.238 | 0.262 | 0.676 | 90.062 | 0.91x |
| nested.json | msgspec | 0.443 | 0.535 | 1.030 | 90.062 | 0.45x |
| nested.json | ujson | 1.208 | 1.305 | 2.145 | 90.062 | 0.18x |
| nested.json | json | 1.806 | 1.895 | 6.628 | 90.062 | 0.13x |
| wide_arrays.json | strata | 1.217 | 1.332 | 1.442 | 92.812 | 1.00x |
| wide_arrays.json | orjson | 1.430 | 1.737 | 3.786 | 92.812 | 0.77x |
| wide_arrays.json | msgspec | 2.257 | 2.448 | 4.711 | 92.812 | 0.54x |
| wide_arrays.json | ujson | 7.070 | 8.108 | 13.695 | 92.812 | 0.16x |
| wide_arrays.json | json | 13.113 | 14.770 | 20.507 | 92.812 | 0.09x |
| mixed.json | strata | 0.052 | 0.070 | 0.310 | 92.828 | 1.00x |
| mixed.json | orjson | 0.053 | 0.062 | 0.218 | 92.828 | 1.12x |
| mixed.json | msgspec | 0.063 | 0.073 | 0.340 | 92.828 | 0.95x |
| mixed.json | ujson | 0.241 | 0.272 | 0.784 | 92.828 | 0.26x |
| mixed.json | json | 0.374 | 0.425 | 1.082 | 92.828 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.830 | 8.687 | 12.055 | 88.531 | 1.00x |
| users.json | orjson | 10.805 | 14.033 | 26.773 | 88.531 | 0.62x |
| users.json | msgspec | 10.635 | 13.103 | 16.856 | 88.531 | 0.66x |
| users.json | ujson | 15.062 | 18.535 | 30.974 | 88.531 | 0.47x |
| users.json | json | 17.796 | 20.617 | 38.480 | 88.531 | 0.42x |
| flat.json | strata | 0.767 | 0.905 | 1.946 | 90.062 | 1.00x |
| flat.json | orjson | 1.001 | 1.190 | 1.424 | 90.062 | 0.76x |
| flat.json | msgspec | 0.933 | 0.963 | 1.388 | 90.062 | 0.94x |
| flat.json | ujson | 1.371 | 1.457 | 2.909 | 90.062 | 0.62x |
| flat.json | json | 1.564 | 1.727 | 3.544 | 90.062 | 0.52x |
| nested.json | strata | 0.681 | 0.720 | 1.450 | 90.062 | 1.00x |
| nested.json | orjson | 1.005 | 1.159 | 2.716 | 90.062 | 0.62x |
| nested.json | msgspec | 0.859 | 0.992 | 2.264 | 90.062 | 0.73x |
| nested.json | ujson | 1.196 | 1.399 | 3.439 | 90.062 | 0.51x |
| nested.json | json | 1.674 | 1.841 | 4.474 | 90.062 | 0.39x |
| wide_arrays.json | strata | 3.780 | 3.948 | 4.484 | 92.812 | 1.00x |
| wide_arrays.json | orjson | 4.054 | 4.279 | 9.274 | 92.812 | 0.92x |
| wide_arrays.json | msgspec | 4.611 | 5.183 | 6.876 | 92.812 | 0.76x |
| wide_arrays.json | ujson | 6.234 | 6.975 | 13.994 | 92.812 | 0.57x |
| wide_arrays.json | json | 7.599 | 8.554 | 16.111 | 92.812 | 0.46x |
| mixed.json | strata | 0.205 | 0.251 | 0.832 | 92.828 | 1.00x |
| mixed.json | orjson | 0.271 | 0.525 | 1.093 | 92.828 | 0.48x |
| mixed.json | msgspec | 0.253 | 0.311 | 1.031 | 92.828 | 0.81x |
| mixed.json | ujson | 0.323 | 0.380 | 1.068 | 92.828 | 0.66x |
| mixed.json | json | 0.429 | 0.521 | 0.794 | 92.828 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.248 | 10.487 | 17.023 | 90.047 | 1.00x |
| users.ndjson | orjson | 12.871 | 17.547 | 24.794 | 90.047 | 0.60x |
| users.ndjson | msgspec | 13.150 | 14.139 | 19.756 | 90.047 | 0.74x |
| users.ndjson | ujson | 16.751 | 18.756 | 31.038 | 90.047 | 0.56x |
| users.ndjson | json | 22.375 | 28.581 | 47.811 | 90.047 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.436 | 2.725 | 4.952 | 87.359 | 1.00x |
| users.json | orjson | 3.085 | 3.558 | 6.713 | 87.359 | 0.77x |
| users.json | msgspec | 3.813 | 3.923 | 5.986 | 87.359 | 0.69x |
| users.json | ujson | 12.050 | 16.351 | 20.482 | 87.359 | 0.17x |
| users.json | json | 18.123 | 22.054 | 28.473 | 87.359 | 0.12x |
| flat.json | strata | 0.403 | 0.523 | 1.123 | 90.062 | 1.00x |
| flat.json | orjson | 0.531 | 0.635 | 1.496 | 90.062 | 0.82x |
| flat.json | msgspec | 0.640 | 0.753 | 1.417 | 90.062 | 0.69x |
| flat.json | ujson | 1.157 | 1.281 | 1.763 | 90.062 | 0.41x |
| flat.json | json | 1.584 | 1.980 | 3.064 | 90.062 | 0.26x |
| nested.json | strata | 0.268 | 0.359 | 0.430 | 90.062 | 1.00x |
| nested.json | orjson | 0.357 | 0.449 | 0.493 | 90.062 | 0.80x |
| nested.json | msgspec | 0.421 | 0.671 | 0.819 | 90.062 | 0.54x |
| nested.json | ujson | 1.279 | 1.405 | 1.623 | 90.062 | 0.26x |
| nested.json | json | 1.825 | 1.972 | 2.222 | 90.062 | 0.18x |
| wide_arrays.json | strata | 1.586 | 1.930 | 3.744 | 92.812 | 1.00x |
| wide_arrays.json | orjson | 1.915 | 2.175 | 3.862 | 92.812 | 0.89x |
| wide_arrays.json | msgspec | 2.864 | 3.093 | 4.070 | 92.812 | 0.62x |
| wide_arrays.json | ujson | 7.447 | 8.691 | 13.648 | 92.812 | 0.22x |
| wide_arrays.json | json | 13.139 | 15.039 | 29.522 | 92.812 | 0.13x |
| mixed.json | strata | 0.202 | 0.303 | 12.083 | 92.828 | 1.00x |
| mixed.json | orjson | 0.269 | 0.473 | 11.920 | 92.828 | 0.64x |
| mixed.json | msgspec | 0.270 | 0.626 | 11.691 | 92.828 | 0.48x |
| mixed.json | ujson | 0.437 | 0.736 | 6.041 | 92.828 | 0.41x |
| mixed.json | json | 0.646 | 0.733 | 14.773 | 92.828 | 0.41x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.115 | 0.148 | 0.311 | 87.391 | 1.00x |
| users.json $[*].id | jmespath | 0.344 | 0.397 | 0.997 | 87.391 | 0.37x |
| users.json $[*].id | jsonpath-ng | 1.609 | 2.118 | 4.345 | 87.391 | 0.07x |
| users.json $[*].orders[*].total | strata | 0.767 | 1.101 | 9.668 | 87.578 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.032 | 2.359 | 33.809 | 87.578 | 0.47x |
| users.json $[*].orders[*].total | jsonpath-ng | 15.483 | 26.278 | 31.009 | 87.578 | 0.04x |
| users.json $..total | strata | 1.403 | 1.571 | 3.640 | 88.422 | 1.00x |
| users.json $..total | jsonpath-ng | 264.392 | 292.744 | 334.905 | 88.422 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.954 | 4.670 | 6.833 | 87.438 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.171 | 16.289 | 24.768 | 87.438 | 0.29x |
| users.json $[*].id | orjson+jsonpath-ng | 14.313 | 19.265 | 68.855 | 87.438 | 0.24x |
| users.json $[*].orders[*].total | strata | 4.082 | 4.351 | 5.693 | 88.406 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.519 | 22.001 | 33.427 | 88.406 | 0.20x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 31.800 | 42.444 | 53.948 | 88.406 | 0.10x |
| users.json $..total | strata | 9.871 | 12.719 | 23.480 | 88.438 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 246.841 | 305.324 | 441.423 | 88.438 | 0.04x |

