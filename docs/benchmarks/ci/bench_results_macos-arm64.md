# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9b6124a
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
| users.json | strata | 6.758 | 7.311 | 8.631 | 63.750 | 1.00x |
| users.json | orjson | 9.427 | 10.007 | 11.628 | 63.750 | 0.73x |
| users.json | msgspec | 8.974 | 9.635 | 10.165 | 63.750 | 0.76x |
| users.json | ujson | 12.405 | 13.286 | 14.508 | 63.750 | 0.55x |
| users.json | pysimdjson | 124.325 | 129.287 | 132.978 | 63.750 | 0.06x |
| users.json | json | 14.845 | 15.883 | 16.987 | 63.750 | 0.46x |
| flat.json | strata | 0.602 | 0.662 | 0.742 | 91.906 | 1.00x |
| flat.json | orjson | 0.749 | 0.826 | 0.885 | 91.906 | 0.80x |
| flat.json | msgspec | 0.708 | 0.786 | 0.882 | 91.906 | 0.84x |
| flat.json | ujson | 1.065 | 1.245 | 1.413 | 91.906 | 0.53x |
| flat.json | pysimdjson | 11.574 | 12.565 | 13.189 | 91.906 | 0.05x |
| flat.json | json | 1.332 | 1.454 | 1.667 | 91.906 | 0.46x |
| nested.json | strata | 0.513 | 0.524 | 0.628 | 91.922 | 1.00x |
| nested.json | orjson | 0.707 | 0.719 | 0.883 | 91.922 | 0.73x |
| nested.json | msgspec | 0.659 | 0.684 | 0.796 | 91.922 | 0.77x |
| nested.json | ujson | 1.043 | 1.132 | 1.346 | 91.922 | 0.46x |
| nested.json | pysimdjson | 10.120 | 10.200 | 10.835 | 91.922 | 0.05x |
| nested.json | json | 1.370 | 1.426 | 1.543 | 91.922 | 0.37x |
| wide_arrays.json | strata | 3.410 | 3.493 | 3.946 | 95.719 | 1.00x |
| wide_arrays.json | orjson | 3.557 | 3.628 | 4.188 | 95.719 | 0.96x |
| wide_arrays.json | msgspec | 3.993 | 4.014 | 4.665 | 95.719 | 0.87x |
| wide_arrays.json | ujson | 5.260 | 5.479 | 6.783 | 95.719 | 0.64x |
| wide_arrays.json | pysimdjson | 62.616 | 63.592 | 68.858 | 95.719 | 0.05x |
| wide_arrays.json | json | 6.721 | 6.844 | 7.442 | 95.719 | 0.51x |
| mixed.json | strata | 0.128 | 0.133 | 0.152 | 96.500 | 1.00x |
| mixed.json | orjson | 0.152 | 0.169 | 0.188 | 96.500 | 0.79x |
| mixed.json | msgspec | 0.165 | 0.175 | 0.199 | 96.500 | 0.76x |
| mixed.json | ujson | 0.207 | 0.300 | 0.395 | 96.500 | 0.44x |
| mixed.json | pysimdjson | 2.456 | 2.504 | 2.717 | 96.500 | 0.05x |
| mixed.json | json | 0.316 | 0.333 | 0.383 | 96.500 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.580 | 1.667 | 2.314 | 80.125 | 1.00x |
| users.json | orjson | 2.260 | 2.372 | 3.061 | 80.125 | 0.70x |
| users.json | msgspec | 2.898 | 3.005 | 3.546 | 80.125 | 0.55x |
| users.json | ujson | 10.746 | 11.081 | 12.491 | 80.125 | 0.15x |
| users.json | json | 15.515 | 16.008 | 17.496 | 80.125 | 0.10x |
| flat.json | strata | 0.197 | 0.202 | 0.472 | 91.922 | 1.00x |
| flat.json | orjson | 0.247 | 0.252 | 0.311 | 91.922 | 0.80x |
| flat.json | msgspec | 0.302 | 0.309 | 0.556 | 91.922 | 0.65x |
| flat.json | ujson | 0.903 | 0.927 | 1.225 | 91.922 | 0.22x |
| flat.json | json | 1.341 | 1.368 | 1.524 | 91.922 | 0.15x |
| nested.json | strata | 0.134 | 0.159 | 0.174 | 91.922 | 1.00x |
| nested.json | orjson | 0.224 | 0.263 | 0.292 | 91.922 | 0.60x |
| nested.json | msgspec | 0.282 | 0.310 | 0.425 | 91.922 | 0.51x |
| nested.json | ujson | 1.093 | 1.195 | 1.303 | 91.922 | 0.13x |
| nested.json | json | 1.596 | 1.733 | 1.902 | 91.922 | 0.09x |
| wide_arrays.json | strata | 1.023 | 1.138 | 1.277 | 95.734 | 1.00x |
| wide_arrays.json | orjson | 1.278 | 1.470 | 1.701 | 95.734 | 0.77x |
| wide_arrays.json | msgspec | 2.114 | 2.277 | 2.536 | 95.734 | 0.50x |
| wide_arrays.json | ujson | 6.501 | 6.660 | 8.696 | 95.734 | 0.17x |
| wide_arrays.json | json | 11.523 | 11.772 | 13.104 | 95.734 | 0.10x |
| mixed.json | strata | 0.039 | 0.054 | 0.065 | 96.500 | 1.00x |
| mixed.json | orjson | 0.041 | 0.053 | 0.070 | 96.500 | 1.02x |
| mixed.json | msgspec | 0.052 | 0.135 | 0.216 | 96.500 | 0.40x |
| mixed.json | ujson | 0.222 | 0.245 | 0.271 | 96.500 | 0.22x |
| mixed.json | json | 0.329 | 0.388 | 0.500 | 96.500 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.216 | 7.466 | 8.478 | 90.797 | 1.00x |
| users.json | orjson | 9.661 | 10.122 | 11.392 | 90.797 | 0.74x |
| users.json | msgspec | 9.420 | 9.853 | 11.050 | 90.797 | 0.76x |
| users.json | ujson | 12.845 | 13.631 | 15.855 | 90.797 | 0.55x |
| users.json | json | 15.735 | 16.194 | 17.915 | 90.797 | 0.46x |
| flat.json | strata | 0.648 | 0.759 | 0.903 | 91.922 | 1.00x |
| flat.json | orjson | 1.005 | 1.124 | 1.487 | 91.922 | 0.68x |
| flat.json | msgspec | 0.786 | 0.878 | 1.174 | 91.922 | 0.86x |
| flat.json | ujson | 1.136 | 1.286 | 1.507 | 91.922 | 0.59x |
| flat.json | json | 1.409 | 1.520 | 1.693 | 91.922 | 0.50x |
| nested.json | strata | 0.564 | 0.593 | 0.660 | 91.922 | 1.00x |
| nested.json | orjson | 0.846 | 0.851 | 0.961 | 91.922 | 0.70x |
| nested.json | msgspec | 0.724 | 0.758 | 0.867 | 91.922 | 0.78x |
| nested.json | ujson | 1.010 | 1.023 | 1.165 | 91.922 | 0.58x |
| nested.json | json | 1.427 | 1.488 | 1.615 | 91.922 | 0.40x |
| wide_arrays.json | strata | 3.596 | 3.696 | 4.117 | 95.734 | 1.00x |
| wide_arrays.json | orjson | 3.819 | 4.081 | 4.591 | 95.734 | 0.91x |
| wide_arrays.json | msgspec | 4.350 | 4.459 | 5.246 | 95.734 | 0.83x |
| wide_arrays.json | ujson | 5.747 | 5.817 | 6.726 | 95.734 | 0.64x |
| wide_arrays.json | json | 7.020 | 7.099 | 8.027 | 95.734 | 0.52x |
| mixed.json | strata | 0.144 | 0.179 | 0.271 | 96.500 | 1.00x |
| mixed.json | orjson | 0.288 | 0.402 | 0.545 | 96.500 | 0.44x |
| mixed.json | msgspec | 0.233 | 0.274 | 0.343 | 96.500 | 0.65x |
| mixed.json | ujson | 0.275 | 0.318 | 0.491 | 96.500 | 0.56x |
| mixed.json | json | 0.375 | 0.409 | 0.639 | 96.500 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.234 | 8.166 | 9.473 | 91.891 | 1.00x |
| users.ndjson | orjson | 11.411 | 12.756 | 21.658 | 91.891 | 0.64x |
| users.ndjson | msgspec | 11.319 | 12.360 | 23.029 | 91.891 | 0.66x |
| users.ndjson | ujson | 13.997 | 15.414 | 29.845 | 91.891 | 0.53x |
| users.ndjson | json | 18.291 | 20.213 | 27.381 | 91.891 | 0.40x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.014 | 2.352 | 2.493 | 86.172 | 1.00x |
| users.json | orjson | 2.814 | 3.098 | 3.578 | 86.172 | 0.76x |
| users.json | msgspec | 3.454 | 3.758 | 4.144 | 86.172 | 0.63x |
| users.json | ujson | 12.154 | 12.970 | 15.356 | 86.172 | 0.18x |
| users.json | json | 17.042 | 18.422 | 28.155 | 86.172 | 0.13x |
| flat.json | strata | 0.324 | 0.424 | 0.605 | 91.922 | 1.00x |
| flat.json | orjson | 0.381 | 0.424 | 0.514 | 91.922 | 1.00x |
| flat.json | msgspec | 0.427 | 0.586 | 0.700 | 91.922 | 0.72x |
| flat.json | ujson | 1.062 | 1.186 | 1.345 | 91.922 | 0.36x |
| flat.json | json | 1.475 | 1.519 | 1.629 | 91.922 | 0.28x |
| nested.json | strata | 0.275 | 0.375 | 0.648 | 91.922 | 1.00x |
| nested.json | orjson | 0.351 | 0.416 | 0.549 | 91.922 | 0.90x |
| nested.json | msgspec | 0.411 | 0.501 | 0.971 | 91.922 | 0.75x |
| nested.json | ujson | 1.223 | 1.374 | 1.817 | 91.922 | 0.27x |
| nested.json | json | 1.781 | 2.035 | 2.289 | 91.922 | 0.18x |
| wide_arrays.json | strata | 1.317 | 1.621 | 1.976 | 96.484 | 1.00x |
| wide_arrays.json | orjson | 1.778 | 1.953 | 2.363 | 96.484 | 0.83x |
| wide_arrays.json | msgspec | 2.436 | 2.741 | 3.273 | 96.484 | 0.59x |
| wide_arrays.json | ujson | 7.433 | 7.834 | 9.937 | 96.484 | 0.21x |
| wide_arrays.json | json | 12.060 | 13.385 | 14.369 | 96.484 | 0.12x |
| mixed.json | strata | 0.188 | 0.205 | 0.282 | 96.500 | 1.00x |
| mixed.json | orjson | 0.175 | 0.242 | 0.274 | 96.500 | 0.85x |
| mixed.json | msgspec | 0.177 | 0.203 | 0.348 | 96.500 | 1.01x |
| mixed.json | ujson | 0.341 | 0.396 | 0.488 | 96.500 | 0.52x |
| mixed.json | json | 0.469 | 0.568 | 0.683 | 96.500 | 0.36x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.060 | 0.089 | 0.191 | 86.250 | 1.00x |
| users.json $[*].id | jmespath | 0.285 | 0.370 | 0.435 | 86.250 | 0.24x |
| users.json $[*].id | jsonpath-ng | 1.498 | 1.683 | 1.971 | 86.250 | 0.05x |
| users.json $[*].orders[*].total | strata | 0.341 | 0.498 | 0.668 | 87.391 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.686 | 2.000 | 2.490 | 87.391 | 0.25x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.763 | 11.862 | 14.195 | 87.391 | 0.04x |
| users.json $..total | strata | 1.269 | 1.446 | 1.887 | 89.234 | 1.00x |
| users.json $..total | jsonpath-ng | 191.178 | 199.324 | 223.156 | 89.234 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.615 | 3.740 | 4.339 | 87.297 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.296 | 11.057 | 12.090 | 87.297 | 0.34x |
| users.json $[*].id | orjson+jsonpath-ng | 11.451 | 11.889 | 13.721 | 87.297 | 0.31x |
| users.json $[*].orders[*].total | strata | 3.796 | 4.543 | 6.329 | 89.219 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 12.059 | 12.659 | 15.053 | 89.219 | 0.36x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 23.277 | 28.868 | 31.967 | 89.219 | 0.16x |
| users.json $..total | strata | 8.708 | 9.561 | 10.545 | 89.266 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 206.420 | 217.420 | 224.060 | 89.266 | 0.04x |

