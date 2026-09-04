# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: de68beb
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
| users.json | strata | 6.422 | 6.802 | 6.961 | 52.844 | 1.00x |
| users.json | orjson | 8.128 | 8.475 | 8.775 | 52.844 | 0.80x |
| users.json | msgspec | 8.206 | 8.531 | 8.765 | 52.844 | 0.80x |
| users.json | ujson | 10.896 | 11.442 | 11.550 | 52.844 | 0.59x |
| users.json | json | 15.427 | 15.906 | 16.698 | 52.844 | 0.43x |
| flat.json | strata | 0.601 | 0.643 | 0.704 | 71.031 | 1.00x |
| flat.json | orjson | 0.651 | 0.680 | 0.715 | 71.031 | 0.95x |
| flat.json | msgspec | 0.676 | 0.714 | 0.802 | 71.031 | 0.90x |
| flat.json | ujson | 1.010 | 1.073 | 1.166 | 71.031 | 0.60x |
| flat.json | json | 1.425 | 1.495 | 1.567 | 71.031 | 0.43x |
| nested.json | strata | 0.539 | 0.565 | 0.637 | 71.062 | 1.00x |
| nested.json | orjson | 0.606 | 0.622 | 0.749 | 71.062 | 0.91x |
| nested.json | msgspec | 0.609 | 0.632 | 0.692 | 71.062 | 0.89x |
| nested.json | ujson | 0.891 | 0.935 | 1.089 | 71.062 | 0.60x |
| nested.json | json | 1.415 | 1.451 | 1.555 | 71.062 | 0.39x |
| wide_arrays.json | strata | 3.025 | 3.260 | 3.518 | 71.234 | 1.00x |
| wide_arrays.json | orjson | 3.178 | 3.335 | 3.661 | 71.234 | 0.98x |
| wide_arrays.json | msgspec | 4.094 | 4.436 | 4.841 | 71.234 | 0.73x |
| wide_arrays.json | ujson | 5.220 | 5.676 | 6.136 | 71.234 | 0.57x |
| wide_arrays.json | json | 7.635 | 8.138 | 8.402 | 71.234 | 0.40x |
| mixed.json | strata | 0.131 | 0.135 | 0.151 | 72.234 | 1.00x |
| mixed.json | orjson | 0.130 | 0.137 | 0.147 | 72.234 | 0.99x |
| mixed.json | msgspec | 0.144 | 0.149 | 0.174 | 72.234 | 0.90x |
| mixed.json | ujson | 0.187 | 0.195 | 0.221 | 72.234 | 0.69x |
| mixed.json | json | 0.309 | 0.331 | 0.367 | 72.234 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.430 | 1.491 | 1.538 | 56.328 | 1.00x |
| users.json | orjson | 2.004 | 2.072 | 2.135 | 56.328 | 0.72x |
| users.json | msgspec | 2.666 | 2.773 | 2.915 | 56.328 | 0.54x |
| users.json | ujson | 11.186 | 11.375 | 11.543 | 56.328 | 0.13x |
| users.json | json | 16.021 | 16.417 | 16.755 | 56.328 | 0.09x |
| flat.json | strata | 0.178 | 0.183 | 0.201 | 71.062 | 1.00x |
| flat.json | orjson | 0.227 | 0.234 | 0.253 | 71.062 | 0.78x |
| flat.json | msgspec | 0.296 | 0.309 | 0.428 | 71.062 | 0.59x |
| flat.json | ujson | 0.868 | 0.888 | 1.012 | 71.062 | 0.21x |
| flat.json | json | 1.403 | 1.530 | 1.626 | 71.062 | 0.12x |
| nested.json | strata | 0.135 | 0.140 | 0.173 | 71.125 | 1.00x |
| nested.json | orjson | 0.203 | 0.218 | 0.253 | 71.125 | 0.64x |
| nested.json | msgspec | 0.269 | 0.285 | 0.316 | 71.125 | 0.49x |
| nested.json | ujson | 0.968 | 1.010 | 1.183 | 71.125 | 0.14x |
| nested.json | json | 1.754 | 1.796 | 2.185 | 71.125 | 0.08x |
| wide_arrays.json | strata | 0.918 | 0.953 | 1.002 | 71.234 | 1.00x |
| wide_arrays.json | orjson | 1.100 | 1.122 | 1.187 | 71.234 | 0.85x |
| wide_arrays.json | msgspec | 1.908 | 1.969 | 2.131 | 71.234 | 0.48x |
| wide_arrays.json | ujson | 5.999 | 6.210 | 6.530 | 71.234 | 0.15x |
| wide_arrays.json | json | 11.482 | 11.895 | 12.176 | 71.234 | 0.08x |
| mixed.json | strata | 0.038 | 0.041 | 0.048 | 72.281 | 1.00x |
| mixed.json | orjson | 0.045 | 0.051 | 0.062 | 72.281 | 0.80x |
| mixed.json | msgspec | 0.050 | 0.054 | 0.069 | 72.281 | 0.75x |
| mixed.json | ujson | 0.218 | 0.224 | 0.247 | 72.281 | 0.18x |
| mixed.json | json | 0.356 | 0.405 | 0.458 | 72.281 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.720 | 6.992 | 7.171 | 71.547 | 1.00x |
| users.json | orjson | 8.378 | 8.733 | 8.940 | 71.547 | 0.80x |
| users.json | msgspec | 8.580 | 8.841 | 9.167 | 71.547 | 0.79x |
| users.json | ujson | 11.775 | 12.001 | 12.186 | 71.547 | 0.58x |
| users.json | json | 16.105 | 16.294 | 17.123 | 71.547 | 0.43x |
| flat.json | strata | 0.697 | 0.727 | 0.815 | 71.062 | 1.00x |
| flat.json | orjson | 0.729 | 0.748 | 0.816 | 71.062 | 0.97x |
| flat.json | msgspec | 0.773 | 0.825 | 0.902 | 71.062 | 0.88x |
| flat.json | ujson | 1.128 | 1.215 | 1.258 | 71.062 | 0.60x |
| flat.json | json | 1.501 | 1.620 | 1.666 | 71.062 | 0.45x |
| nested.json | strata | 0.588 | 0.690 | 0.763 | 71.125 | 1.00x |
| nested.json | orjson | 0.677 | 0.717 | 0.831 | 71.125 | 0.96x |
| nested.json | msgspec | 0.681 | 0.719 | 0.949 | 71.125 | 0.96x |
| nested.json | ujson | 0.985 | 1.081 | 1.251 | 71.125 | 0.64x |
| nested.json | json | 1.507 | 1.555 | 1.703 | 71.125 | 0.44x |
| wide_arrays.json | strata | 3.254 | 3.347 | 3.555 | 72.234 | 1.00x |
| wide_arrays.json | orjson | 3.470 | 3.560 | 3.795 | 72.234 | 0.94x |
| wide_arrays.json | msgspec | 4.198 | 4.464 | 4.735 | 72.234 | 0.75x |
| wide_arrays.json | ujson | 5.565 | 5.819 | 6.166 | 72.234 | 0.58x |
| wide_arrays.json | json | 8.025 | 8.337 | 8.761 | 72.234 | 0.40x |
| mixed.json | strata | 0.182 | 0.213 | 0.259 | 72.281 | 1.00x |
| mixed.json | orjson | 0.183 | 0.219 | 0.232 | 72.281 | 0.97x |
| mixed.json | msgspec | 0.196 | 0.215 | 0.358 | 72.281 | 0.99x |
| mixed.json | ujson | 0.259 | 0.266 | 0.337 | 72.281 | 0.80x |
| mixed.json | json | 0.362 | 0.399 | 0.533 | 72.281 | 0.53x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.151 | 7.312 | 7.523 | 70.172 | 1.00x |
| users.ndjson | orjson | 10.543 | 10.764 | 11.650 | 70.172 | 0.68x |
| users.ndjson | msgspec | 10.483 | 10.722 | 11.014 | 70.172 | 0.68x |
| users.ndjson | ujson | 13.430 | 13.899 | 14.218 | 70.172 | 0.53x |
| users.ndjson | json | 19.041 | 19.495 | 19.706 | 70.172 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.888 | 2.015 | 2.338 | 67.547 | 1.00x |
| users.json | orjson | 2.474 | 2.575 | 2.852 | 67.547 | 0.78x |
| users.json | msgspec | 3.091 | 3.235 | 3.650 | 67.547 | 0.62x |
| users.json | ujson | 11.346 | 11.550 | 12.070 | 67.547 | 0.17x |
| users.json | json | 16.541 | 17.041 | 17.510 | 67.547 | 0.12x |
| flat.json | strata | 0.356 | 0.405 | 0.543 | 71.062 | 1.00x |
| flat.json | orjson | 0.380 | 0.437 | 0.475 | 71.062 | 0.93x |
| flat.json | msgspec | 0.445 | 0.494 | 0.627 | 71.062 | 0.82x |
| flat.json | ujson | 1.034 | 1.118 | 1.202 | 71.062 | 0.36x |
| flat.json | json | 1.506 | 1.730 | 1.839 | 71.062 | 0.23x |
| nested.json | strata | 0.284 | 0.341 | 0.381 | 71.125 | 1.00x |
| nested.json | orjson | 0.364 | 0.410 | 0.651 | 71.125 | 0.83x |
| nested.json | msgspec | 0.425 | 0.489 | 0.577 | 71.125 | 0.70x |
| nested.json | ujson | 1.139 | 1.205 | 1.314 | 71.125 | 0.28x |
| nested.json | json | 1.905 | 2.102 | 2.309 | 71.125 | 0.16x |
| wide_arrays.json | strata | 1.241 | 1.384 | 1.567 | 72.234 | 1.00x |
| wide_arrays.json | orjson | 1.413 | 1.491 | 1.928 | 72.234 | 0.93x |
| wide_arrays.json | msgspec | 2.259 | 2.350 | 2.558 | 72.234 | 0.59x |
| wide_arrays.json | ujson | 6.418 | 6.592 | 6.823 | 72.234 | 0.21x |
| wide_arrays.json | json | 11.771 | 12.238 | 12.545 | 72.234 | 0.11x |
| mixed.json | strata | 0.163 | 0.183 | 0.237 | 72.312 | 1.00x |
| mixed.json | orjson | 0.177 | 0.216 | 0.303 | 72.312 | 0.85x |
| mixed.json | msgspec | 0.189 | 0.260 | 0.303 | 72.312 | 0.70x |
| mixed.json | ujson | 0.373 | 0.424 | 0.491 | 72.312 | 0.43x |
| mixed.json | json | 0.506 | 0.581 | 0.662 | 72.312 | 0.31x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.054 | 0.060 | 0.089 | 67.859 | 1.00x |
| users.json $[*].id | jmespath | 0.294 | 0.333 | 0.395 | 67.859 | 0.18x |
| users.json $[*].id | jsonpath-ng | 1.418 | 1.562 | 1.729 | 67.859 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.349 | 0.389 | 0.619 | 68.250 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.710 | 1.967 | 2.135 | 68.250 | 0.20x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.190 | 10.746 | 11.400 | 68.250 | 0.04x |
| users.json $..total | strata | 1.401 | 1.438 | 1.649 | 70.359 | 1.00x |
| users.json $..total | jsonpath-ng | 189.105 | 191.393 | 192.629 | 70.359 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.455 | 3.605 | 3.815 | 68.000 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.827 | 9.062 | 9.642 | 68.000 | 0.40x |
| users.json $[*].id | orjson+jsonpath-ng | 9.970 | 10.187 | 10.748 | 68.000 | 0.35x |
| users.json $[*].orders[*].total | strata | 3.527 | 3.627 | 3.697 | 70.266 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.056 | 10.680 | 10.941 | 70.266 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.569 | 20.876 | 21.329 | 70.266 | 0.17x |
| users.json $..total | strata | 8.509 | 8.797 | 8.975 | 70.391 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 201.132 | 203.610 | 205.696 | 70.391 | 0.04x |

