# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: c16eaa65c61f1cf9d4fac46e9f6d8e6b9f3b087f
- python: 3.12.10
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit
- machine: arm64
- processor: Apple M1 (Virtual)
- compiler_flags: -fno-strict-overflow -Wsign-compare -Wunreachable-code -fno-common -dynamic -DNDEBUG -g -O3 -Wall -arch arm64 -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -march=native -flto=thin -fprofile-use=/Users/runner/work/strata/strata/build/pgo/strata.profdata -bundle -undefined (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.304 | 9.203 | 17.248 | 69.734 | 1.00x |
| users.json | orjson | 11.566 | 16.958 | 33.981 | 69.734 | 0.54x |
| users.json | msgspec | 10.411 | 13.632 | 33.292 | 69.734 | 0.68x |
| users.json | ujson | 13.990 | 17.774 | 36.522 | 69.734 | 0.52x |
| users.json | pysimdjson | 139.537 | 160.840 | 235.748 | 69.734 | 0.06x |
| users.json | json | 17.873 | 21.706 | 33.305 | 69.734 | 0.42x |
| flat.json | strata | 0.613 | 0.652 | 1.951 | 99.672 | 1.00x |
| flat.json | orjson | 0.792 | 0.865 | 1.734 | 99.672 | 0.75x |
| flat.json | msgspec | 0.743 | 0.768 | 1.042 | 99.672 | 0.85x |
| flat.json | ujson | 1.246 | 1.401 | 2.233 | 99.672 | 0.47x |
| flat.json | pysimdjson | 12.086 | 12.580 | 22.315 | 99.672 | 0.05x |
| flat.json | json | 1.410 | 1.439 | 3.834 | 99.672 | 0.45x |
| nested.json | strata | 0.538 | 0.600 | 0.853 | 99.672 | 1.00x |
| nested.json | orjson | 0.752 | 0.787 | 0.953 | 99.672 | 0.76x |
| nested.json | msgspec | 0.698 | 0.748 | 0.967 | 99.672 | 0.80x |
| nested.json | ujson | 1.212 | 1.283 | 1.892 | 99.672 | 0.47x |
| nested.json | pysimdjson | 10.546 | 11.632 | 14.620 | 99.672 | 0.05x |
| nested.json | json | 1.454 | 1.509 | 2.024 | 99.672 | 0.40x |
| wide_arrays.json | strata | 3.511 | 3.859 | 5.083 | 102.422 | 1.00x |
| wide_arrays.json | orjson | 4.258 | 5.799 | 8.419 | 102.422 | 0.67x |
| wide_arrays.json | msgspec | 4.724 | 6.322 | 15.572 | 102.422 | 0.61x |
| wide_arrays.json | ujson | 5.967 | 7.004 | 15.629 | 102.422 | 0.55x |
| wide_arrays.json | pysimdjson | 68.329 | 78.604 | 118.858 | 102.422 | 0.05x |
| wide_arrays.json | json | 7.670 | 8.282 | 16.466 | 102.422 | 0.47x |
| mixed.json | strata | 0.134 | 0.149 | 0.227 | 102.438 | 1.00x |
| mixed.json | orjson | 0.168 | 0.199 | 0.210 | 102.438 | 0.75x |
| mixed.json | msgspec | 0.179 | 0.206 | 0.234 | 102.438 | 0.72x |
| mixed.json | ujson | 0.247 | 0.328 | 0.415 | 102.438 | 0.45x |
| mixed.json | pysimdjson | 2.631 | 2.727 | 3.305 | 102.438 | 0.05x |
| mixed.json | json | 0.362 | 0.382 | 0.960 | 102.438 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.476 | 1.804 | 4.108 | 82.750 | 1.00x |
| users.json | orjson | 2.481 | 2.962 | 5.759 | 82.750 | 0.61x |
| users.json | msgspec | 3.187 | 3.401 | 5.875 | 82.750 | 0.53x |
| users.json | ujson | 9.010 | 10.179 | 15.698 | 82.750 | 0.18x |
| users.json | json | 16.883 | 18.979 | 37.090 | 82.750 | 0.10x |
| flat.json | strata | 0.230 | 0.267 | 0.513 | 99.672 | 1.00x |
| flat.json | orjson | 0.264 | 0.304 | 0.553 | 99.672 | 0.88x |
| flat.json | msgspec | 0.339 | 0.372 | 0.685 | 99.672 | 0.72x |
| flat.json | ujson | 0.792 | 0.910 | 2.232 | 99.672 | 0.29x |
| flat.json | json | 1.606 | 1.813 | 3.921 | 99.672 | 0.15x |
| nested.json | strata | 0.131 | 0.135 | 0.285 | 99.672 | 1.00x |
| nested.json | orjson | 0.220 | 0.237 | 0.291 | 99.672 | 0.57x |
| nested.json | msgspec | 0.293 | 0.316 | 0.569 | 99.672 | 0.43x |
| nested.json | ujson | 0.810 | 1.039 | 1.271 | 99.672 | 0.13x |
| nested.json | json | 1.646 | 1.699 | 3.499 | 99.672 | 0.08x |
| wide_arrays.json | strata | 1.364 | 1.607 | 2.890 | 102.422 | 1.00x |
| wide_arrays.json | orjson | 1.590 | 1.786 | 3.815 | 102.422 | 0.90x |
| wide_arrays.json | msgspec | 2.489 | 2.889 | 4.026 | 102.422 | 0.56x |
| wide_arrays.json | ujson | 5.254 | 5.585 | 10.361 | 102.422 | 0.29x |
| wide_arrays.json | json | 12.451 | 14.802 | 20.448 | 102.422 | 0.11x |
| mixed.json | strata | 0.047 | 0.051 | 0.072 | 102.438 | 1.00x |
| mixed.json | orjson | 0.054 | 0.064 | 0.291 | 102.438 | 0.81x |
| mixed.json | msgspec | 0.061 | 0.069 | 0.249 | 102.438 | 0.75x |
| mixed.json | ujson | 0.189 | 0.196 | 0.207 | 102.438 | 0.26x |
| mixed.json | json | 0.375 | 0.397 | 0.429 | 102.438 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.306 | 8.021 | 15.212 | 93.766 | 1.00x |
| users.json | orjson | 11.711 | 13.333 | 37.963 | 93.766 | 0.60x |
| users.json | msgspec | 10.487 | 12.453 | 14.529 | 93.766 | 0.64x |
| users.json | ujson | 15.794 | 16.835 | 18.628 | 93.766 | 0.48x |
| users.json | json | 18.816 | 19.910 | 33.796 | 93.766 | 0.40x |
| flat.json | strata | 0.743 | 0.932 | 1.264 | 99.672 | 1.00x |
| flat.json | orjson | 1.122 | 1.285 | 3.262 | 99.672 | 0.73x |
| flat.json | msgspec | 0.908 | 1.214 | 2.425 | 99.672 | 0.77x |
| flat.json | ujson | 1.482 | 1.585 | 3.810 | 99.672 | 0.59x |
| flat.json | json | 1.634 | 1.995 | 3.562 | 99.672 | 0.47x |
| nested.json | strata | 0.634 | 0.653 | 0.706 | 99.672 | 1.00x |
| nested.json | orjson | 1.017 | 1.066 | 1.122 | 99.672 | 0.61x |
| nested.json | msgspec | 0.863 | 0.890 | 0.928 | 99.672 | 0.73x |
| nested.json | ujson | 1.166 | 1.217 | 1.302 | 99.672 | 0.54x |
| nested.json | json | 1.592 | 1.647 | 1.891 | 99.672 | 0.40x |
| wide_arrays.json | strata | 3.329 | 3.730 | 6.692 | 102.422 | 1.00x |
| wide_arrays.json | orjson | 3.920 | 4.694 | 9.242 | 102.422 | 0.79x |
| wide_arrays.json | msgspec | 4.552 | 5.125 | 11.255 | 102.422 | 0.73x |
| wide_arrays.json | ujson | 5.886 | 6.479 | 16.962 | 102.422 | 0.58x |
| wide_arrays.json | json | 7.387 | 8.393 | 17.191 | 102.422 | 0.44x |
| mixed.json | strata | 0.192 | 0.227 | 0.267 | 102.438 | 1.00x |
| mixed.json | orjson | 0.280 | 0.304 | 0.349 | 102.438 | 0.75x |
| mixed.json | msgspec | 0.298 | 0.317 | 0.807 | 102.438 | 0.72x |
| mixed.json | ujson | 0.536 | 0.626 | 1.461 | 102.438 | 0.36x |
| mixed.json | json | 0.438 | 0.484 | 0.562 | 102.438 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.423 | 7.748 | 10.370 | 99.656 | 1.00x |
| users.ndjson | orjson | 12.529 | 13.506 | 19.656 | 99.656 | 0.57x |
| users.ndjson | msgspec | 12.329 | 14.110 | 20.241 | 99.656 | 0.55x |
| users.ndjson | ujson | 15.898 | 17.402 | 21.480 | 99.656 | 0.45x |
| users.ndjson | json | 19.904 | 21.235 | 28.500 | 99.656 | 0.36x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.186 | 2.784 | 4.209 | 93.969 | 1.00x |
| users.json | orjson | 3.129 | 3.720 | 6.285 | 93.969 | 0.75x |
| users.json | msgspec | 3.554 | 4.302 | 5.930 | 93.969 | 0.65x |
| users.json | ujson | 10.091 | 11.787 | 13.453 | 93.969 | 0.24x |
| users.json | json | 17.268 | 19.563 | 29.171 | 93.969 | 0.14x |
| flat.json | strata | 0.442 | 0.579 | 1.111 | 99.672 | 1.00x |
| flat.json | orjson | 0.559 | 0.628 | 1.363 | 99.672 | 0.92x |
| flat.json | msgspec | 0.595 | 0.741 | 0.957 | 99.672 | 0.78x |
| flat.json | ujson | 1.031 | 1.178 | 1.440 | 99.672 | 0.49x |
| flat.json | json | 1.693 | 1.802 | 2.088 | 99.672 | 0.32x |
| nested.json | strata | 0.451 | 0.493 | 0.532 | 99.672 | 1.00x |
| nested.json | orjson | 0.583 | 0.663 | 0.789 | 99.672 | 0.74x |
| nested.json | msgspec | 0.822 | 0.878 | 0.969 | 99.672 | 0.56x |
| nested.json | ujson | 1.456 | 1.506 | 1.551 | 99.672 | 0.33x |
| nested.json | json | 2.155 | 2.244 | 2.372 | 99.672 | 0.22x |
| wide_arrays.json | strata | 1.682 | 1.948 | 2.874 | 102.422 | 1.00x |
| wide_arrays.json | orjson | 2.047 | 2.378 | 3.010 | 102.422 | 0.82x |
| wide_arrays.json | msgspec | 3.000 | 3.390 | 5.764 | 102.422 | 0.57x |
| wide_arrays.json | ujson | 5.861 | 6.553 | 11.822 | 102.422 | 0.30x |
| wide_arrays.json | json | 12.908 | 14.599 | 23.158 | 102.422 | 0.13x |
| mixed.json | strata | 0.259 | 0.352 | 0.869 | 102.438 | 1.00x |
| mixed.json | orjson | 0.237 | 0.489 | 0.942 | 102.438 | 0.72x |
| mixed.json | msgspec | 0.264 | 0.349 | 0.830 | 102.438 | 1.01x |
| mixed.json | ujson | 0.426 | 0.599 | 1.104 | 102.438 | 0.59x |
| mixed.json | json | 0.682 | 0.752 | 0.898 | 102.438 | 0.47x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.067 | 0.106 | 0.173 | 94.016 | 1.00x |
| users.json $[*].id | jmespath | 0.316 | 0.375 | 0.494 | 94.016 | 0.28x |
| users.json $[*].id | jsonpath-ng | 1.569 | 1.712 | 5.424 | 94.016 | 0.06x |
| users.json $[*].orders[*].total | strata | 0.695 | 0.793 | 1.179 | 94.188 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.122 | 2.320 | 3.635 | 94.188 | 0.34x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.045 | 14.080 | 15.035 | 94.188 | 0.06x |
| users.json $..total | strata | 1.502 | 1.711 | 3.992 | 94.203 | 1.00x |
| users.json $..total | jsonpath-ng | 203.395 | 262.929 | 387.393 | 94.203 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.670 | 4.603 | 7.520 | 94.109 | 1.00x |
| users.json $[*].id | orjson+jmespath | 13.051 | 14.122 | 20.116 | 94.109 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 13.359 | 14.962 | 26.567 | 94.109 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.809 | 4.106 | 27.761 | 94.203 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.184 | 16.392 | 31.672 | 94.203 | 0.25x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 26.362 | 34.626 | 71.308 | 94.203 | 0.12x |
| users.json $..total | strata | 8.321 | 9.656 | 14.803 | 94.203 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 215.607 | 228.933 | 375.712 | 94.203 | 0.04x |

