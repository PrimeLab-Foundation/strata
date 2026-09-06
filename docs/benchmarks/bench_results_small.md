# Benchmark results - small

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 90dd3b7
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
| users.json | strata | 5.826 | 5.964 | 6.421 | 51.375 | 1.00x |
| users.json | orjson | 7.824 | 8.255 | 8.655 | 51.375 | 0.72x |
| users.json | msgspec | 8.000 | 8.130 | 8.581 | 51.375 | 0.73x |
| users.json | ujson | 11.032 | 11.219 | 11.476 | 51.375 | 0.53x |
| users.json | json | 15.341 | 15.618 | 15.843 | 51.375 | 0.38x |
| flat.json | strata | 0.571 | 0.620 | 0.692 | 71.719 | 1.00x |
| flat.json | orjson | 0.664 | 0.690 | 0.762 | 71.719 | 0.90x |
| flat.json | msgspec | 0.683 | 0.716 | 0.748 | 71.719 | 0.87x |
| flat.json | ujson | 1.025 | 1.070 | 1.206 | 71.719 | 0.58x |
| flat.json | json | 1.436 | 1.466 | 1.570 | 71.719 | 0.42x |
| nested.json | strata | 0.497 | 0.528 | 0.729 | 72.266 | 1.00x |
| nested.json | orjson | 0.626 | 0.652 | 2.252 | 72.266 | 0.81x |
| nested.json | msgspec | 0.625 | 0.650 | 2.394 | 72.266 | 0.81x |
| nested.json | ujson | 0.909 | 0.943 | 2.184 | 72.266 | 0.56x |
| nested.json | json | 1.471 | 1.510 | 3.960 | 72.266 | 0.35x |
| wide_arrays.json | strata | 2.927 | 3.123 | 3.364 | 73.062 | 1.00x |
| wide_arrays.json | orjson | 3.254 | 3.343 | 3.713 | 73.062 | 0.93x |
| wide_arrays.json | msgspec | 3.964 | 4.272 | 4.719 | 73.062 | 0.73x |
| wide_arrays.json | ujson | 5.240 | 5.418 | 5.695 | 73.062 | 0.58x |
| wide_arrays.json | json | 7.829 | 8.070 | 8.323 | 73.062 | 0.39x |
| mixed.json | strata | 0.116 | 0.125 | 0.135 | 74.094 | 1.00x |
| mixed.json | orjson | 0.128 | 0.134 | 0.150 | 74.094 | 0.93x |
| mixed.json | msgspec | 0.143 | 0.148 | 0.264 | 74.094 | 0.84x |
| mixed.json | ujson | 0.182 | 0.189 | 0.212 | 74.094 | 0.66x |
| mixed.json | json | 0.308 | 0.324 | 0.375 | 74.094 | 0.38x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.355 | 1.384 | 1.551 | 56.281 | 1.00x |
| users.json | orjson | 1.994 | 2.040 | 2.094 | 56.281 | 0.68x |
| users.json | msgspec | 2.631 | 2.658 | 2.698 | 56.281 | 0.52x |
| users.json | ujson | 10.846 | 11.023 | 11.349 | 56.281 | 0.13x |
| users.json | json | 15.609 | 16.135 | 16.566 | 56.281 | 0.09x |
| flat.json | strata | 0.191 | 0.199 | 0.212 | 71.922 | 1.00x |
| flat.json | orjson | 0.222 | 0.229 | 0.284 | 71.922 | 0.87x |
| flat.json | msgspec | 0.294 | 0.310 | 0.349 | 71.922 | 0.64x |
| flat.json | ujson | 0.873 | 0.896 | 1.221 | 71.922 | 0.22x |
| flat.json | json | 1.353 | 1.488 | 1.583 | 71.922 | 0.13x |
| nested.json | strata | 0.131 | 0.134 | 0.163 | 72.297 | 1.00x |
| nested.json | orjson | 0.203 | 0.208 | 0.234 | 72.297 | 0.64x |
| nested.json | msgspec | 0.272 | 0.290 | 0.308 | 72.297 | 0.46x |
| nested.json | ujson | 0.970 | 0.994 | 1.123 | 72.297 | 0.14x |
| nested.json | json | 1.627 | 1.712 | 2.012 | 72.297 | 0.08x |
| wide_arrays.json | strata | 0.946 | 1.004 | 1.117 | 74.094 | 1.00x |
| wide_arrays.json | orjson | 1.103 | 1.154 | 1.289 | 74.094 | 0.87x |
| wide_arrays.json | msgspec | 1.911 | 2.044 | 2.195 | 74.094 | 0.49x |
| wide_arrays.json | ujson | 6.112 | 6.280 | 6.838 | 74.094 | 0.16x |
| wide_arrays.json | json | 11.406 | 11.815 | 12.195 | 74.094 | 0.08x |
| mixed.json | strata | 0.033 | 0.037 | 0.039 | 74.109 | 1.00x |
| mixed.json | orjson | 0.043 | 0.047 | 0.051 | 74.109 | 0.79x |
| mixed.json | msgspec | 0.048 | 0.052 | 0.061 | 74.109 | 0.72x |
| mixed.json | ujson | 0.214 | 0.237 | 0.301 | 74.109 | 0.16x |
| mixed.json | json | 0.371 | 0.398 | 0.468 | 74.109 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.144 | 6.217 | 7.517 | 70.031 | 1.00x |
| users.json | orjson | 8.257 | 8.381 | 8.836 | 70.031 | 0.74x |
| users.json | msgspec | 8.359 | 8.419 | 8.804 | 70.031 | 0.74x |
| users.json | ujson | 11.333 | 11.493 | 12.146 | 70.031 | 0.54x |
| users.json | json | 15.627 | 15.849 | 16.727 | 70.031 | 0.39x |
| flat.json | strata | 0.662 | 0.706 | 0.851 | 71.922 | 1.00x |
| flat.json | orjson | 0.751 | 0.825 | 0.922 | 71.922 | 0.86x |
| flat.json | msgspec | 0.765 | 0.832 | 0.917 | 71.922 | 0.85x |
| flat.json | ujson | 1.169 | 1.292 | 1.392 | 71.922 | 0.55x |
| flat.json | json | 1.536 | 1.633 | 1.859 | 71.922 | 0.43x |
| nested.json | strata | 0.584 | 0.603 | 0.636 | 72.297 | 1.00x |
| nested.json | orjson | 0.727 | 0.745 | 0.794 | 72.297 | 0.81x |
| nested.json | msgspec | 0.706 | 0.746 | 0.802 | 72.297 | 0.81x |
| nested.json | ujson | 1.025 | 1.053 | 1.096 | 72.297 | 0.57x |
| nested.json | json | 1.550 | 1.618 | 1.758 | 72.297 | 0.37x |
| wide_arrays.json | strata | 3.192 | 3.320 | 3.458 | 74.094 | 1.00x |
| wide_arrays.json | orjson | 3.413 | 3.608 | 3.937 | 74.094 | 0.92x |
| wide_arrays.json | msgspec | 4.213 | 4.422 | 4.917 | 74.094 | 0.75x |
| wide_arrays.json | ujson | 5.573 | 5.804 | 6.322 | 74.094 | 0.57x |
| wide_arrays.json | json | 7.754 | 8.195 | 8.514 | 74.094 | 0.41x |
| mixed.json | strata | 0.158 | 0.178 | 0.226 | 74.109 | 1.00x |
| mixed.json | orjson | 0.173 | 0.183 | 0.245 | 74.109 | 0.97x |
| mixed.json | msgspec | 0.186 | 0.233 | 0.327 | 74.109 | 0.76x |
| mixed.json | ujson | 0.265 | 0.296 | 0.362 | 74.109 | 0.60x |
| mixed.json | json | 0.367 | 0.380 | 0.477 | 74.109 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 6.554 | 6.778 | 6.976 | 70.844 | 1.00x |
| users.ndjson | orjson | 10.297 | 10.866 | 11.380 | 70.844 | 0.62x |
| users.ndjson | msgspec | 10.323 | 10.633 | 11.238 | 70.844 | 0.64x |
| users.ndjson | ujson | 13.605 | 13.938 | 14.360 | 70.844 | 0.49x |
| users.ndjson | json | 19.414 | 19.625 | 30.688 | 70.844 | 0.35x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.738 | 1.787 | 1.986 | 66.031 | 1.00x |
| users.json | orjson | 2.402 | 2.495 | 2.794 | 66.031 | 0.72x |
| users.json | msgspec | 3.028 | 3.080 | 3.547 | 66.031 | 0.58x |
| users.json | ujson | 11.055 | 11.237 | 11.943 | 66.031 | 0.16x |
| users.json | json | 16.022 | 16.101 | 16.670 | 66.031 | 0.11x |
| flat.json | strata | 0.383 | 0.422 | 0.515 | 71.938 | 1.00x |
| flat.json | orjson | 0.414 | 0.465 | 0.591 | 71.938 | 0.91x |
| flat.json | msgspec | 0.500 | 0.558 | 0.623 | 71.938 | 0.76x |
| flat.json | ujson | 1.090 | 1.181 | 1.291 | 71.938 | 0.36x |
| flat.json | json | 1.524 | 1.706 | 2.044 | 71.938 | 0.25x |
| nested.json | strata | 0.286 | 0.341 | 0.432 | 72.297 | 1.00x |
| nested.json | orjson | 0.377 | 0.435 | 0.571 | 72.297 | 0.78x |
| nested.json | msgspec | 0.459 | 0.500 | 0.752 | 72.297 | 0.68x |
| nested.json | ujson | 1.195 | 1.256 | 1.361 | 72.297 | 0.27x |
| nested.json | json | 1.842 | 1.882 | 2.165 | 72.297 | 0.18x |
| wide_arrays.json | strata | 1.265 | 1.473 | 1.775 | 74.094 | 1.00x |
| wide_arrays.json | orjson | 1.463 | 1.686 | 1.992 | 74.094 | 0.87x |
| wide_arrays.json | msgspec | 2.236 | 2.494 | 2.745 | 74.094 | 0.59x |
| wide_arrays.json | ujson | 6.515 | 6.680 | 7.276 | 74.094 | 0.22x |
| wide_arrays.json | json | 11.822 | 12.419 | 13.109 | 74.094 | 0.12x |
| mixed.json | strata | 0.154 | 0.194 | 0.268 | 74.141 | 1.00x |
| mixed.json | orjson | 0.178 | 0.237 | 0.480 | 74.141 | 0.82x |
| mixed.json | msgspec | 0.200 | 0.222 | 0.312 | 74.141 | 0.87x |
| mixed.json | ujson | 0.368 | 0.409 | 0.530 | 74.141 | 0.47x |
| mixed.json | json | 0.490 | 0.552 | 0.749 | 74.141 | 0.35x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.053 | 0.065 | 0.092 | 66.484 | 1.00x |
| users.json $[*].id | jmespath | 0.278 | 0.285 | 0.324 | 66.484 | 0.23x |
| users.json $[*].id | jsonpath-ng | 1.434 | 1.448 | 1.473 | 66.484 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.324 | 0.350 | 0.441 | 67.016 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.653 | 1.687 | 1.767 | 67.016 | 0.21x |
| users.json $[*].orders[*].total | jsonpath-ng | 9.607 | 9.721 | 10.597 | 67.016 | 0.04x |
| users.json $..total | strata | 1.375 | 1.381 | 1.531 | 69.953 | 1.00x |
| users.json $..total | jsonpath-ng | 186.791 | 187.298 | 196.614 | 69.953 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.355 | 3.396 | 3.473 | 66.672 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.556 | 8.680 | 8.712 | 66.672 | 0.39x |
| users.json $[*].id | orjson+jsonpath-ng | 9.777 | 9.874 | 9.964 | 66.672 | 0.34x |
| users.json $[*].orders[*].total | strata | 3.408 | 3.517 | 3.620 | 69.938 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.187 | 10.318 | 10.572 | 69.938 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 19.907 | 20.391 | 21.277 | 69.938 | 0.17x |
| users.json $..total | strata | 7.684 | 7.802 | 8.096 | 69.969 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 197.047 | 198.833 | 201.580 | 69.969 | 0.04x |

