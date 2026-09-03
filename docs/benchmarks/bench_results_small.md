# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: f6672fd
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.310 | 6.369 | 6.874 | 51.656 | 1.00x |
| users.json | orjson | 7.863 | 7.925 | 8.253 | 51.656 | 0.80x |
| users.json | msgspec | 7.978 | 7.996 | 8.433 | 51.656 | 0.80x |
| users.json | ujson | 10.799 | 10.930 | 11.725 | 51.656 | 0.58x |
| users.json | json | 15.067 | 15.122 | 15.656 | 51.656 | 0.42x |
| flat.json | strata | 0.605 | 0.607 | 0.618 | 70.500 | 1.00x |
| flat.json | orjson | 0.643 | 0.649 | 0.661 | 70.500 | 0.93x |
| flat.json | msgspec | 0.683 | 0.687 | 0.692 | 70.500 | 0.88x |
| flat.json | ujson | 1.020 | 1.028 | 1.035 | 70.500 | 0.59x |
| flat.json | json | 1.434 | 1.439 | 1.455 | 70.500 | 0.42x |
| nested.json | strata | 0.531 | 0.536 | 0.540 | 70.531 | 1.00x |
| nested.json | orjson | 0.603 | 0.606 | 0.613 | 70.531 | 0.88x |
| nested.json | msgspec | 0.603 | 0.610 | 0.611 | 70.531 | 0.88x |
| nested.json | ujson | 0.881 | 0.889 | 0.897 | 70.531 | 0.60x |
| nested.json | json | 1.418 | 1.427 | 1.451 | 70.531 | 0.38x |
| wide_arrays.json | strata | 2.983 | 3.013 | 3.295 | 70.609 | 1.00x |
| wide_arrays.json | orjson | 3.157 | 3.179 | 3.433 | 70.609 | 0.95x |
| wide_arrays.json | msgspec | 3.907 | 3.926 | 4.431 | 70.609 | 0.77x |
| wide_arrays.json | ujson | 5.115 | 5.150 | 5.288 | 70.609 | 0.59x |
| wide_arrays.json | json | 7.497 | 7.573 | 7.864 | 70.609 | 0.40x |
| mixed.json | strata | 0.126 | 0.128 | 0.150 | 70.625 | 1.00x |
| mixed.json | orjson | 0.126 | 0.127 | 0.138 | 70.625 | 1.01x |
| mixed.json | msgspec | 0.139 | 0.143 | 0.168 | 70.625 | 0.89x |
| mixed.json | ujson | 0.180 | 0.184 | 0.193 | 70.625 | 0.70x |
| mixed.json | json | 0.303 | 0.308 | 0.408 | 70.625 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.427 | 1.433 | 1.450 | 57.531 | 1.00x |
| users.json | orjson | 1.989 | 1.994 | 2.032 | 57.531 | 0.72x |
| users.json | msgspec | 2.660 | 2.679 | 2.758 | 57.531 | 0.54x |
| users.json | ujson | 10.794 | 10.837 | 11.299 | 57.531 | 0.13x |
| users.json | json | 15.422 | 15.480 | 16.279 | 57.531 | 0.09x |
| flat.json | strata | 0.177 | 0.178 | 0.181 | 70.516 | 1.00x |
| flat.json | orjson | 0.222 | 0.223 | 0.226 | 70.516 | 0.80x |
| flat.json | msgspec | 0.292 | 0.293 | 0.297 | 70.516 | 0.61x |
| flat.json | ujson | 0.885 | 0.887 | 0.891 | 70.516 | 0.20x |
| flat.json | json | 1.320 | 1.325 | 1.332 | 70.516 | 0.13x |
| nested.json | strata | 0.134 | 0.134 | 0.156 | 70.609 | 1.00x |
| nested.json | orjson | 0.201 | 0.203 | 0.207 | 70.609 | 0.66x |
| nested.json | msgspec | 0.267 | 0.270 | 0.275 | 70.609 | 0.50x |
| nested.json | ujson | 0.965 | 0.973 | 0.983 | 70.609 | 0.14x |
| nested.json | json | 1.615 | 1.628 | 1.733 | 70.609 | 0.08x |
| wide_arrays.json | strata | 0.913 | 0.915 | 0.990 | 70.609 | 1.00x |
| wide_arrays.json | orjson | 1.106 | 1.113 | 1.161 | 70.609 | 0.82x |
| wide_arrays.json | msgspec | 1.923 | 1.930 | 1.977 | 70.609 | 0.47x |
| wide_arrays.json | ujson | 5.963 | 6.005 | 6.040 | 70.609 | 0.15x |
| wide_arrays.json | json | 10.983 | 11.014 | 11.679 | 70.609 | 0.08x |
| mixed.json | strata | 0.035 | 0.036 | 0.054 | 70.672 | 1.00x |
| mixed.json | orjson | 0.042 | 0.044 | 0.049 | 70.672 | 0.83x |
| mixed.json | msgspec | 0.049 | 0.051 | 0.059 | 70.672 | 0.70x |
| mixed.json | ujson | 0.215 | 0.218 | 0.230 | 70.672 | 0.17x |
| mixed.json | json | 0.352 | 0.359 | 0.449 | 70.672 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.643 | 6.693 | 6.961 | 69.188 | 1.00x |
| users.json | orjson | 8.174 | 8.265 | 8.390 | 69.188 | 0.81x |
| users.json | msgspec | 8.311 | 8.398 | 8.734 | 69.188 | 0.80x |
| users.json | ujson | 11.389 | 11.574 | 12.152 | 69.188 | 0.58x |
| users.json | json | 15.393 | 15.572 | 17.105 | 69.188 | 0.43x |
| flat.json | strata | 0.667 | 0.671 | 0.720 | 70.516 | 1.00x |
| flat.json | orjson | 0.715 | 0.722 | 0.820 | 70.516 | 0.93x |
| flat.json | msgspec | 0.759 | 0.766 | 0.813 | 70.516 | 0.88x |
| flat.json | ujson | 1.130 | 1.145 | 1.280 | 70.516 | 0.59x |
| flat.json | json | 1.506 | 1.517 | 1.636 | 70.516 | 0.44x |
| nested.json | strata | 0.591 | 0.612 | 0.653 | 70.609 | 1.00x |
| nested.json | orjson | 0.671 | 0.684 | 0.724 | 70.609 | 0.90x |
| nested.json | msgspec | 0.678 | 0.688 | 0.922 | 70.609 | 0.89x |
| nested.json | ujson | 0.971 | 0.995 | 1.132 | 70.609 | 0.62x |
| nested.json | json | 1.495 | 1.505 | 1.668 | 70.609 | 0.41x |
| wide_arrays.json | strata | 3.168 | 3.184 | 3.247 | 70.609 | 1.00x |
| wide_arrays.json | orjson | 3.399 | 3.412 | 3.656 | 70.609 | 0.93x |
| wide_arrays.json | msgspec | 4.151 | 4.187 | 4.497 | 70.609 | 0.76x |
| wide_arrays.json | ujson | 5.450 | 5.479 | 5.552 | 70.609 | 0.58x |
| wide_arrays.json | json | 7.735 | 7.780 | 7.815 | 70.609 | 0.41x |
| mixed.json | strata | 0.164 | 0.169 | 0.178 | 70.672 | 1.00x |
| mixed.json | orjson | 0.170 | 0.175 | 0.183 | 70.672 | 0.97x |
| mixed.json | msgspec | 0.183 | 0.188 | 0.200 | 70.672 | 0.90x |
| mixed.json | ujson | 0.237 | 0.244 | 0.259 | 70.672 | 0.69x |
| mixed.json | json | 0.348 | 0.352 | 0.370 | 70.672 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.766 | 6.872 | 6.973 | 69.641 | 1.00x |
| users.ndjson | orjson | 10.050 | 10.162 | 10.331 | 69.641 | 0.68x |
| users.ndjson | msgspec | 9.901 | 10.072 | 10.457 | 69.641 | 0.68x |
| users.ndjson | ujson | 12.845 | 13.057 | 13.656 | 69.641 | 0.53x |
| users.ndjson | json | 17.956 | 18.078 | 19.125 | 69.641 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.814 | 1.927 | 2.328 | 66.188 | 1.00x |
| users.json | orjson | 2.394 | 2.497 | 2.993 | 66.188 | 0.77x |
| users.json | msgspec | 3.055 | 3.197 | 3.602 | 66.188 | 0.60x |
| users.json | ujson | 11.057 | 11.248 | 11.554 | 66.188 | 0.17x |
| users.json | json | 15.937 | 16.083 | 16.769 | 66.188 | 0.12x |
| flat.json | strata | 0.331 | 0.349 | 0.385 | 70.516 | 1.00x |
| flat.json | orjson | 0.379 | 0.400 | 0.417 | 70.516 | 0.87x |
| flat.json | msgspec | 0.434 | 0.461 | 0.570 | 70.516 | 0.76x |
| flat.json | ujson | 1.072 | 1.093 | 1.322 | 70.516 | 0.32x |
| flat.json | json | 1.493 | 1.507 | 1.688 | 70.516 | 0.23x |
| nested.json | strata | 0.292 | 0.311 | 0.330 | 70.609 | 1.00x |
| nested.json | orjson | 0.350 | 0.364 | 0.393 | 70.609 | 0.85x |
| nested.json | msgspec | 0.410 | 0.433 | 0.467 | 70.609 | 0.72x |
| nested.json | ujson | 1.128 | 1.139 | 1.187 | 70.609 | 0.27x |
| nested.json | json | 1.778 | 1.810 | 1.839 | 70.609 | 0.17x |
| wide_arrays.json | strata | 1.227 | 1.287 | 1.417 | 70.609 | 1.00x |
| wide_arrays.json | orjson | 1.417 | 1.451 | 1.536 | 70.609 | 0.89x |
| wide_arrays.json | msgspec | 2.259 | 2.307 | 2.446 | 70.609 | 0.56x |
| wide_arrays.json | ujson | 6.311 | 6.358 | 6.490 | 70.609 | 0.20x |
| wide_arrays.json | json | 11.750 | 11.905 | 12.748 | 70.609 | 0.11x |
| mixed.json | strata | 0.154 | 0.164 | 0.229 | 70.703 | 1.00x |
| mixed.json | orjson | 0.158 | 0.167 | 0.277 | 70.703 | 0.98x |
| mixed.json | msgspec | 0.165 | 0.181 | 0.337 | 70.703 | 0.90x |
| mixed.json | ujson | 0.335 | 0.348 | 0.859 | 70.703 | 0.47x |
| mixed.json | json | 0.468 | 0.481 | 0.746 | 70.703 | 0.34x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.050 | 0.053 | 0.064 | 66.562 | 1.00x |
| users.json $[*].id | jmespath | 0.271 | 0.297 | 0.320 | 66.562 | 0.18x |
| users.json $[*].id | jsonpath-ng | 1.419 | 1.432 | 1.473 | 66.562 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.337 | 0.342 | 0.400 | 66.906 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.660 | 1.669 | 1.946 | 66.906 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.489 | 9.558 | 9.916 | 66.906 | 0.04x |
| users.json $..total | strata | 1.392 | 1.401 | 1.458 | 69.656 | 1.00x |
| users.json $..total | jsonpath-ng | 184.879 | 185.838 | 186.957 | 69.656 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.411 | 3.419 | 3.556 | 66.609 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.488 | 8.574 | 9.022 | 66.609 | 0.40x |
| users.json $[*].id | orjson+jsonpath-ng | 9.732 | 9.800 | 9.930 | 66.609 | 0.35x |
| users.json $[*].orders[*].total | strata | 3.487 | 3.515 | 3.800 | 69.609 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 9.995 | 10.048 | 10.271 | 69.609 | 0.35x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.093 | 19.561 | 21.047 | 69.609 | 0.18x |
| users.json $..total | strata | 8.124 | 8.249 | 8.721 | 69.688 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 195.056 | 196.082 | 197.083 | 69.688 | 0.04x |

