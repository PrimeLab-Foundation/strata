# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: a4d78c1
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
| users.json | strata | 6.906 | 7.008 | 7.707 | 52.859 | 1.00x |
| users.json | orjson | 8.168 | 8.476 | 8.709 | 52.859 | 0.83x |
| users.json | msgspec | 8.291 | 8.602 | 8.940 | 52.859 | 0.81x |
| users.json | ujson | 11.040 | 11.385 | 12.429 | 52.859 | 0.62x |
| users.json | json | 15.646 | 15.928 | 16.512 | 52.859 | 0.44x |
| flat.json | strata | 0.596 | 0.636 | 0.719 | 74.641 | 1.00x |
| flat.json | orjson | 0.639 | 0.657 | 0.802 | 74.641 | 0.97x |
| flat.json | msgspec | 0.707 | 0.729 | 0.782 | 74.641 | 0.87x |
| flat.json | ujson | 1.028 | 1.055 | 1.220 | 74.641 | 0.60x |
| flat.json | json | 1.408 | 1.480 | 1.577 | 74.641 | 0.43x |
| nested.json | strata | 0.533 | 0.562 | 0.611 | 74.688 | 1.00x |
| nested.json | orjson | 0.610 | 0.617 | 0.673 | 74.688 | 0.91x |
| nested.json | msgspec | 0.610 | 0.635 | 0.829 | 74.688 | 0.88x |
| nested.json | ujson | 0.898 | 0.940 | 0.976 | 74.688 | 0.60x |
| nested.json | json | 1.400 | 1.452 | 1.588 | 74.688 | 0.39x |
| wide_arrays.json | strata | 3.226 | 3.445 | 3.684 | 74.766 | 1.00x |
| wide_arrays.json | orjson | 3.178 | 3.437 | 3.717 | 74.766 | 1.00x |
| wide_arrays.json | msgspec | 4.054 | 4.385 | 4.462 | 74.766 | 0.79x |
| wide_arrays.json | ujson | 5.178 | 5.529 | 5.778 | 74.766 | 0.62x |
| wide_arrays.json | json | 7.740 | 8.000 | 8.172 | 74.766 | 0.43x |
| mixed.json | strata | 0.131 | 0.138 | 0.193 | 75.438 | 1.00x |
| mixed.json | orjson | 0.130 | 0.137 | 0.157 | 75.438 | 1.01x |
| mixed.json | msgspec | 0.142 | 0.150 | 0.194 | 75.438 | 0.92x |
| mixed.json | ujson | 0.185 | 0.190 | 0.220 | 75.438 | 0.73x |
| mixed.json | json | 0.307 | 0.322 | 0.379 | 75.438 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.535 | 1.596 | 1.682 | 57.688 | 1.00x |
| users.json | orjson | 1.992 | 2.082 | 2.153 | 57.688 | 0.77x |
| users.json | msgspec | 2.626 | 2.800 | 2.925 | 57.688 | 0.57x |
| users.json | ujson | 11.088 | 11.224 | 11.496 | 57.688 | 0.14x |
| users.json | json | 16.029 | 16.357 | 16.840 | 57.688 | 0.10x |
| flat.json | strata | 0.181 | 0.183 | 0.197 | 74.656 | 1.00x |
| flat.json | orjson | 0.227 | 0.234 | 0.264 | 74.656 | 0.78x |
| flat.json | msgspec | 0.294 | 0.305 | 0.325 | 74.656 | 0.60x |
| flat.json | ujson | 0.897 | 0.915 | 0.965 | 74.656 | 0.20x |
| flat.json | json | 1.439 | 1.543 | 1.601 | 74.656 | 0.12x |
| nested.json | strata | 0.136 | 0.143 | 0.174 | 74.766 | 1.00x |
| nested.json | orjson | 0.203 | 0.211 | 0.242 | 74.766 | 0.68x |
| nested.json | msgspec | 0.271 | 0.280 | 0.339 | 74.766 | 0.51x |
| nested.json | ujson | 0.960 | 0.979 | 1.063 | 74.766 | 0.15x |
| nested.json | json | 1.642 | 1.859 | 1.969 | 74.766 | 0.08x |
| wide_arrays.json | strata | 0.985 | 1.019 | 1.105 | 75.438 | 1.00x |
| wide_arrays.json | orjson | 1.107 | 1.157 | 1.294 | 75.438 | 0.88x |
| wide_arrays.json | msgspec | 1.944 | 1.997 | 2.189 | 75.438 | 0.51x |
| wide_arrays.json | ujson | 6.118 | 6.220 | 6.531 | 75.438 | 0.16x |
| wide_arrays.json | json | 11.349 | 11.870 | 12.146 | 75.438 | 0.09x |
| mixed.json | strata | 0.039 | 0.045 | 0.050 | 75.516 | 1.00x |
| mixed.json | orjson | 0.044 | 0.048 | 0.063 | 75.516 | 0.95x |
| mixed.json | msgspec | 0.050 | 0.052 | 0.072 | 75.516 | 0.87x |
| mixed.json | ujson | 0.217 | 0.223 | 0.246 | 75.516 | 0.20x |
| mixed.json | json | 0.378 | 0.437 | 0.481 | 75.516 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.083 | 7.336 | 7.551 | 72.781 | 1.00x |
| users.json | orjson | 8.405 | 8.657 | 8.923 | 72.781 | 0.85x |
| users.json | msgspec | 8.489 | 8.919 | 9.104 | 72.781 | 0.82x |
| users.json | ujson | 11.731 | 12.148 | 12.362 | 72.781 | 0.60x |
| users.json | json | 15.873 | 16.224 | 17.114 | 72.781 | 0.45x |
| flat.json | strata | 0.679 | 0.717 | 0.797 | 74.656 | 1.00x |
| flat.json | orjson | 0.723 | 0.771 | 0.864 | 74.656 | 0.93x |
| flat.json | msgspec | 0.763 | 0.798 | 0.927 | 74.656 | 0.90x |
| flat.json | ujson | 1.147 | 1.212 | 1.392 | 74.656 | 0.59x |
| flat.json | json | 1.509 | 1.535 | 1.684 | 74.656 | 0.47x |
| nested.json | strata | 0.604 | 0.632 | 0.684 | 74.766 | 1.00x |
| nested.json | orjson | 0.670 | 0.703 | 0.806 | 74.766 | 0.90x |
| nested.json | msgspec | 0.693 | 0.755 | 0.795 | 74.766 | 0.84x |
| nested.json | ujson | 0.984 | 1.046 | 1.112 | 74.766 | 0.60x |
| nested.json | json | 1.483 | 1.613 | 1.703 | 74.766 | 0.39x |
| wide_arrays.json | strata | 3.445 | 3.550 | 3.686 | 75.438 | 1.00x |
| wide_arrays.json | orjson | 3.378 | 3.590 | 3.862 | 75.438 | 0.99x |
| wide_arrays.json | msgspec | 4.181 | 4.570 | 4.830 | 75.438 | 0.78x |
| wide_arrays.json | ujson | 5.793 | 5.931 | 6.051 | 75.438 | 0.60x |
| wide_arrays.json | json | 7.991 | 8.256 | 8.402 | 75.438 | 0.43x |
| mixed.json | strata | 0.183 | 0.198 | 0.307 | 75.516 | 1.00x |
| mixed.json | orjson | 0.181 | 0.237 | 0.321 | 75.516 | 0.84x |
| mixed.json | msgspec | 0.205 | 0.219 | 0.266 | 75.516 | 0.91x |
| mixed.json | ujson | 0.258 | 0.278 | 0.328 | 75.516 | 0.71x |
| mixed.json | json | 0.359 | 0.381 | 0.458 | 75.516 | 0.52x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.301 | 7.546 | 7.741 | 73.781 | 1.00x |
| users.ndjson | orjson | 10.628 | 11.032 | 11.458 | 73.781 | 0.68x |
| users.ndjson | msgspec | 10.320 | 10.718 | 11.321 | 73.781 | 0.70x |
| users.ndjson | ujson | 13.570 | 14.003 | 14.441 | 73.781 | 0.54x |
| users.ndjson | json | 18.985 | 19.230 | 19.637 | 73.781 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.904 | 1.982 | 2.674 | 70.203 | 1.00x |
| users.json | orjson | 2.417 | 2.604 | 2.939 | 70.203 | 0.76x |
| users.json | msgspec | 3.045 | 3.270 | 4.106 | 70.203 | 0.61x |
| users.json | ujson | 11.361 | 11.607 | 12.007 | 70.203 | 0.17x |
| users.json | json | 16.515 | 17.019 | 17.515 | 70.203 | 0.12x |
| flat.json | strata | 0.334 | 0.367 | 0.396 | 74.656 | 1.00x |
| flat.json | orjson | 0.396 | 0.419 | 0.498 | 74.656 | 0.88x |
| flat.json | msgspec | 0.418 | 0.472 | 0.591 | 74.656 | 0.78x |
| flat.json | ujson | 1.056 | 1.111 | 1.215 | 74.656 | 0.33x |
| flat.json | json | 1.515 | 1.644 | 1.830 | 74.656 | 0.22x |
| nested.json | strata | 0.312 | 0.340 | 0.416 | 74.766 | 1.00x |
| nested.json | orjson | 0.361 | 0.411 | 0.516 | 74.766 | 0.83x |
| nested.json | msgspec | 0.432 | 0.543 | 0.649 | 74.766 | 0.63x |
| nested.json | ujson | 1.133 | 1.270 | 1.340 | 74.766 | 0.27x |
| nested.json | json | 1.841 | 2.174 | 2.436 | 74.766 | 0.16x |
| wide_arrays.json | strata | 1.274 | 1.370 | 2.082 | 75.438 | 1.00x |
| wide_arrays.json | orjson | 1.482 | 1.590 | 2.575 | 75.438 | 0.86x |
| wide_arrays.json | msgspec | 2.232 | 2.335 | 2.552 | 75.438 | 0.59x |
| wide_arrays.json | ujson | 6.372 | 6.677 | 6.822 | 75.438 | 0.21x |
| wide_arrays.json | json | 11.888 | 12.363 | 12.990 | 75.438 | 0.11x |
| mixed.json | strata | 0.155 | 0.172 | 0.206 | 75.547 | 1.00x |
| mixed.json | orjson | 0.156 | 0.178 | 0.274 | 75.547 | 0.97x |
| mixed.json | msgspec | 0.166 | 0.184 | 0.334 | 75.547 | 0.93x |
| mixed.json | ujson | 0.346 | 0.417 | 0.511 | 75.547 | 0.41x |
| mixed.json | json | 0.490 | 0.600 | 0.686 | 75.547 | 0.29x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.052 | 0.060 | 0.081 | 70.406 | 1.00x |
| users.json $[*].id | jmespath | 0.276 | 0.320 | 0.372 | 70.406 | 0.19x |
| users.json $[*].id | jsonpath-ng | 1.444 | 1.544 | 1.645 | 70.406 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.335 | 0.441 | 0.602 | 70.859 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.671 | 1.935 | 2.103 | 70.859 | 0.23x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.171 | 10.837 | 11.367 | 70.859 | 0.04x |
| users.json $..total | strata | 1.386 | 1.479 | 1.513 | 73.828 | 1.00x |
| users.json $..total | jsonpath-ng | 189.772 | 190.995 | 192.167 | 73.828 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.457 | 3.556 | 3.735 | 70.578 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.735 | 9.114 | 9.368 | 70.578 | 0.39x |
| users.json $[*].id | orjson+jsonpath-ng | 9.927 | 10.302 | 10.550 | 70.578 | 0.35x |
| users.json $[*].orders[*].total | strata | 3.491 | 3.672 | 3.903 | 73.797 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.138 | 10.716 | 11.008 | 73.797 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.180 | 20.496 | 21.643 | 73.797 | 0.18x |
| users.json $..total | strata | 8.632 | 8.883 | 9.362 | 73.828 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 200.698 | 201.922 | 203.590 | 73.828 | 0.04x |

