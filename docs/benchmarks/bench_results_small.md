# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 3758c97
- python: 3.14.7
- implementation: CPython
- platform: macOS-26.6.2-arm64-arm-64bit-Mach-O
- machine: arm64
- processor: Apple M1 Max
- compiler_flags: -std=c++20 -O3 -march=native -flto -fprofile-use (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 5.877 | 6.195 | 6.794 | 52.891 | 1.00x |
| users.json | orjson | 7.981 | 8.275 | 8.577 | 52.891 | 0.75x |
| users.json | msgspec | 8.201 | 8.470 | 8.774 | 52.891 | 0.73x |
| users.json | ujson | 11.080 | 11.567 | 11.913 | 52.891 | 0.54x |
| users.json | json | 15.296 | 15.818 | 16.160 | 52.891 | 0.39x |
| flat.json | strata | 0.574 | 0.589 | 0.679 | 71.062 | 1.00x |
| flat.json | orjson | 0.649 | 0.681 | 0.799 | 71.062 | 0.87x |
| flat.json | msgspec | 0.686 | 0.700 | 0.843 | 71.062 | 0.84x |
| flat.json | ujson | 1.036 | 1.092 | 1.199 | 71.062 | 0.54x |
| flat.json | json | 1.435 | 1.481 | 1.600 | 71.062 | 0.40x |
| nested.json | strata | 0.493 | 0.511 | 0.563 | 71.094 | 1.00x |
| nested.json | orjson | 0.613 | 0.620 | 0.690 | 71.094 | 0.82x |
| nested.json | msgspec | 0.622 | 0.638 | 0.837 | 71.094 | 0.80x |
| nested.json | ujson | 0.900 | 0.947 | 1.153 | 71.094 | 0.54x |
| nested.json | json | 1.418 | 1.463 | 1.623 | 71.094 | 0.35x |
| wide_arrays.json | strata | 2.930 | 3.108 | 3.308 | 71.875 | 1.00x |
| wide_arrays.json | orjson | 3.194 | 3.319 | 3.622 | 71.875 | 0.94x |
| wide_arrays.json | msgspec | 3.957 | 4.215 | 4.777 | 71.875 | 0.74x |
| wide_arrays.json | ujson | 5.196 | 5.547 | 6.120 | 71.875 | 0.56x |
| wide_arrays.json | json | 7.619 | 8.044 | 8.327 | 71.875 | 0.39x |
| mixed.json | strata | 0.117 | 0.120 | 0.134 | 71.906 | 1.00x |
| mixed.json | orjson | 0.130 | 0.133 | 0.144 | 71.906 | 0.90x |
| mixed.json | msgspec | 0.142 | 0.152 | 0.167 | 71.906 | 0.79x |
| mixed.json | ujson | 0.186 | 0.196 | 0.234 | 71.906 | 0.61x |
| mixed.json | json | 0.309 | 0.327 | 0.375 | 71.906 | 0.37x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.448 | 1.491 | 1.625 | 56.297 | 1.00x |
| users.json | orjson | 1.983 | 2.100 | 2.191 | 56.297 | 0.71x |
| users.json | msgspec | 2.637 | 2.703 | 2.846 | 56.297 | 0.55x |
| users.json | ujson | 10.791 | 11.113 | 11.294 | 56.297 | 0.13x |
| users.json | json | 15.587 | 16.022 | 16.274 | 56.297 | 0.09x |
| flat.json | strata | 0.209 | 0.214 | 0.260 | 71.078 | 1.00x |
| flat.json | orjson | 0.224 | 0.230 | 0.263 | 71.078 | 0.93x |
| flat.json | msgspec | 0.293 | 0.308 | 0.349 | 71.078 | 0.70x |
| flat.json | ujson | 0.895 | 0.907 | 1.050 | 71.078 | 0.24x |
| flat.json | json | 1.449 | 1.524 | 1.626 | 71.078 | 0.14x |
| nested.json | strata | 0.130 | 0.138 | 0.154 | 71.109 | 1.00x |
| nested.json | orjson | 0.202 | 0.207 | 0.261 | 71.109 | 0.66x |
| nested.json | msgspec | 0.270 | 0.278 | 0.313 | 71.109 | 0.49x |
| nested.json | ujson | 0.977 | 1.010 | 1.176 | 71.109 | 0.14x |
| nested.json | json | 1.659 | 1.794 | 2.083 | 71.109 | 0.08x |
| wide_arrays.json | strata | 0.956 | 0.998 | 1.164 | 71.906 | 1.00x |
| wide_arrays.json | orjson | 1.106 | 1.206 | 1.286 | 71.906 | 0.83x |
| wide_arrays.json | msgspec | 1.932 | 2.017 | 2.108 | 71.906 | 0.49x |
| wide_arrays.json | ujson | 6.010 | 6.320 | 6.679 | 71.906 | 0.16x |
| wide_arrays.json | json | 11.223 | 11.665 | 12.005 | 71.906 | 0.09x |
| mixed.json | strata | 0.034 | 0.037 | 0.050 | 71.922 | 1.00x |
| mixed.json | orjson | 0.044 | 0.047 | 0.086 | 71.922 | 0.79x |
| mixed.json | msgspec | 0.049 | 0.051 | 0.055 | 71.922 | 0.73x |
| mixed.json | ujson | 0.217 | 0.219 | 0.245 | 71.922 | 0.17x |
| mixed.json | json | 0.355 | 0.440 | 0.473 | 71.922 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.117 | 6.394 | 6.669 | 71.500 | 1.00x |
| users.json | orjson | 8.356 | 8.774 | 9.034 | 71.500 | 0.73x |
| users.json | msgspec | 8.400 | 8.795 | 9.168 | 71.500 | 0.73x |
| users.json | ujson | 11.865 | 12.036 | 12.322 | 71.500 | 0.53x |
| users.json | json | 15.843 | 16.299 | 17.286 | 71.500 | 0.39x |
| flat.json | strata | 0.658 | 0.719 | 0.805 | 71.078 | 1.00x |
| flat.json | orjson | 0.773 | 0.833 | 0.924 | 71.078 | 0.86x |
| flat.json | msgspec | 0.796 | 0.848 | 1.013 | 71.078 | 0.85x |
| flat.json | ujson | 1.204 | 1.318 | 1.384 | 71.078 | 0.55x |
| flat.json | json | 1.542 | 1.596 | 1.790 | 71.078 | 0.45x |
| nested.json | strata | 0.576 | 0.624 | 0.685 | 71.109 | 1.00x |
| nested.json | orjson | 0.697 | 0.749 | 0.990 | 71.109 | 0.83x |
| nested.json | msgspec | 0.697 | 0.808 | 1.000 | 71.109 | 0.77x |
| nested.json | ujson | 1.013 | 1.062 | 1.343 | 71.109 | 0.59x |
| nested.json | json | 1.504 | 1.580 | 1.733 | 71.109 | 0.40x |
| wide_arrays.json | strata | 3.218 | 3.325 | 3.483 | 71.906 | 1.00x |
| wide_arrays.json | orjson | 3.431 | 3.628 | 3.921 | 71.906 | 0.92x |
| wide_arrays.json | msgspec | 4.332 | 4.521 | 4.701 | 71.906 | 0.74x |
| wide_arrays.json | ujson | 5.631 | 5.908 | 6.272 | 71.906 | 0.56x |
| wide_arrays.json | json | 7.916 | 8.226 | 8.462 | 71.906 | 0.40x |
| mixed.json | strata | 0.169 | 0.190 | 0.211 | 71.922 | 1.00x |
| mixed.json | orjson | 0.168 | 0.193 | 0.205 | 71.922 | 0.98x |
| mixed.json | msgspec | 0.195 | 0.200 | 0.255 | 71.922 | 0.95x |
| mixed.json | ujson | 0.255 | 0.279 | 0.361 | 71.922 | 0.68x |
| mixed.json | json | 0.369 | 0.434 | 0.460 | 71.922 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.624 | 6.802 | 6.975 | 70.188 | 1.00x |
| users.ndjson | orjson | 10.466 | 10.986 | 11.331 | 70.188 | 0.62x |
| users.ndjson | msgspec | 10.110 | 10.670 | 11.103 | 70.188 | 0.64x |
| users.ndjson | ujson | 13.636 | 14.158 | 14.625 | 70.188 | 0.48x |
| users.ndjson | json | 19.102 | 19.386 | 19.744 | 70.188 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.816 | 2.264 | 2.370 | 69.516 | 1.00x |
| users.json | orjson | 2.393 | 2.669 | 3.712 | 69.516 | 0.85x |
| users.json | msgspec | 3.019 | 3.321 | 3.732 | 69.516 | 0.68x |
| users.json | ujson | 11.330 | 11.432 | 12.165 | 69.516 | 0.20x |
| users.json | json | 16.238 | 16.674 | 17.084 | 69.516 | 0.14x |
| flat.json | strata | 0.365 | 0.406 | 0.498 | 71.094 | 1.00x |
| flat.json | orjson | 0.392 | 0.436 | 0.536 | 71.094 | 0.93x |
| flat.json | msgspec | 0.454 | 0.488 | 0.637 | 71.094 | 0.83x |
| flat.json | ujson | 1.117 | 1.151 | 1.294 | 71.094 | 0.35x |
| flat.json | json | 1.561 | 1.722 | 1.894 | 71.094 | 0.24x |
| nested.json | strata | 0.253 | 0.306 | 0.348 | 71.109 | 1.00x |
| nested.json | orjson | 0.361 | 0.422 | 0.561 | 71.109 | 0.73x |
| nested.json | msgspec | 0.437 | 0.483 | 0.677 | 71.109 | 0.63x |
| nested.json | ujson | 1.131 | 1.204 | 1.371 | 71.109 | 0.25x |
| nested.json | json | 1.843 | 2.023 | 2.198 | 71.109 | 0.15x |
| wide_arrays.json | strata | 1.258 | 1.385 | 1.800 | 71.906 | 1.00x |
| wide_arrays.json | orjson | 1.414 | 1.476 | 2.000 | 71.906 | 0.94x |
| wide_arrays.json | msgspec | 2.193 | 2.289 | 2.924 | 71.906 | 0.60x |
| wide_arrays.json | ujson | 6.389 | 6.558 | 6.982 | 71.906 | 0.21x |
| wide_arrays.json | json | 11.409 | 11.986 | 12.477 | 71.906 | 0.12x |
| mixed.json | strata | 0.163 | 0.182 | 0.203 | 71.953 | 1.00x |
| mixed.json | orjson | 0.164 | 0.196 | 0.317 | 71.953 | 0.93x |
| mixed.json | msgspec | 0.175 | 0.232 | 0.302 | 71.953 | 0.79x |
| mixed.json | ujson | 0.362 | 0.405 | 0.491 | 71.953 | 0.45x |
| mixed.json | json | 0.490 | 0.563 | 0.644 | 71.953 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.059 | 0.071 | 0.124 | 69.812 | 1.00x |
| users.json $[*].id | jmespath | 0.279 | 0.302 | 0.351 | 69.812 | 0.23x |
| users.json $[*].id | jsonpath-ng | 1.506 | 1.538 | 1.679 | 69.812 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.335 | 0.400 | 0.480 | 70.219 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.649 | 1.790 | 1.930 | 70.219 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.003 | 10.337 | 10.739 | 70.219 | 0.04x |
| users.json $..total | strata | 1.391 | 1.431 | 1.536 | 70.391 | 1.00x |
| users.json $..total | jsonpath-ng | 188.806 | 189.932 | 193.005 | 70.391 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.395 | 3.456 | 3.488 | 69.922 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.827 | 8.956 | 9.270 | 69.922 | 0.39x |
| users.json $[*].id | orjson+jsonpath-ng | 9.991 | 10.202 | 10.454 | 69.922 | 0.34x |
| users.json $[*].orders[*].total | strata | 3.484 | 3.552 | 3.747 | 70.328 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.153 | 10.579 | 11.221 | 70.328 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.204 | 21.342 | 22.094 | 70.328 | 0.17x |
| users.json $..total | strata | 7.829 | 8.118 | 8.351 | 70.391 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 200.679 | 201.978 | 206.592 | 70.391 | 0.04x |

