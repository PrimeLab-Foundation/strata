# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e44489e
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 17 Stepping 1, AuthenticAMD
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 13.206 | 14.697 | 22.341 | 45.527 | 1.00x |
| users.json | orjson | 17.199 | 18.721 | 29.303 | 45.527 | 0.79x |
| users.json | msgspec | 15.599 | 17.823 | 33.487 | 45.527 | 0.82x |
| users.json | ujson | 27.380 | 29.924 | 49.033 | 45.527 | 0.49x |
| users.json | json | 26.256 | 30.021 | 50.635 | 45.527 | 0.49x |
| flat.json | strata | 1.252 | 1.294 | 1.794 | 50.574 | 1.00x |
| flat.json | orjson | 1.267 | 1.376 | 2.685 | 50.574 | 0.94x |
| flat.json | msgspec | 1.178 | 1.286 | 1.908 | 50.574 | 1.01x |
| flat.json | ujson | 2.471 | 2.613 | 3.843 | 50.574 | 0.50x |
| flat.json | json | 1.995 | 2.045 | 3.596 | 50.574 | 0.63x |
| nested.json | strata | 0.788 | 0.835 | 1.357 | 50.422 | 1.00x |
| nested.json | orjson | 1.133 | 1.235 | 1.686 | 50.422 | 0.68x |
| nested.json | msgspec | 0.954 | 1.015 | 1.643 | 50.422 | 0.82x |
| nested.json | ujson | 1.564 | 1.627 | 2.877 | 50.422 | 0.51x |
| nested.json | json | 2.103 | 2.260 | 3.768 | 50.422 | 0.37x |
| wide_arrays.json | strata | 5.106 | 5.526 | 8.990 | 52.391 | 1.00x |
| wide_arrays.json | orjson | 6.271 | 6.873 | 10.706 | 52.391 | 0.80x |
| wide_arrays.json | msgspec | 6.376 | 6.730 | 66.025 | 52.391 | 0.82x |
| wide_arrays.json | ujson | 8.785 | 9.113 | 15.764 | 52.391 | 0.61x |
| wide_arrays.json | json | 12.299 | 13.635 | 63.826 | 52.391 | 0.41x |
| mixed.json | strata | 0.199 | 0.325 | 0.332 | 50.367 | 1.00x |
| mixed.json | orjson | 0.327 | 0.447 | 2.528 | 50.367 | 0.73x |
| mixed.json | msgspec | 0.262 | 0.396 | 0.471 | 50.367 | 0.82x |
| mixed.json | ujson | 0.350 | 0.610 | 1.447 | 50.367 | 0.53x |
| mixed.json | json | 0.796 | 0.824 | 1.068 | 50.367 | 0.39x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.128 | 5.182 | 6.130 | 44.758 | 1.00x |
| users.json | orjson | 4.913 | 6.173 | 12.086 | 44.758 | 0.84x |
| users.json | msgspec | 7.404 | 8.878 | 10.454 | 44.758 | 0.58x |
| users.json | ujson | 24.655 | 26.362 | 73.508 | 44.758 | 0.20x |
| users.json | json | 39.303 | 42.057 | 85.940 | 44.758 | 0.12x |
| flat.json | strata | 0.339 | 0.349 | 0.379 | 50.992 | 1.00x |
| flat.json | orjson | 0.386 | 0.423 | 0.648 | 50.992 | 0.83x |
| flat.json | msgspec | 0.559 | 0.583 | 0.762 | 50.992 | 0.60x |
| flat.json | ujson | 1.707 | 1.781 | 1.903 | 50.992 | 0.20x |
| flat.json | json | 1.996 | 2.104 | 2.393 | 50.992 | 0.17x |
| nested.json | strata | 0.281 | 0.287 | 0.459 | 50.809 | 1.00x |
| nested.json | orjson | 0.325 | 0.366 | 0.603 | 50.809 | 0.79x |
| nested.json | msgspec | 0.508 | 0.559 | 0.771 | 50.809 | 0.51x |
| nested.json | ujson | 1.362 | 1.419 | 2.758 | 50.809 | 0.20x |
| nested.json | json | 2.496 | 2.761 | 4.475 | 50.809 | 0.10x |
| wide_arrays.json | strata | 2.411 | 2.749 | 4.179 | 51.980 | 1.00x |
| wide_arrays.json | orjson | 2.658 | 3.094 | 5.329 | 51.980 | 0.89x |
| wide_arrays.json | msgspec | 4.491 | 4.980 | 9.523 | 51.980 | 0.55x |
| wide_arrays.json | ujson | 10.598 | 11.427 | 18.547 | 51.980 | 0.24x |
| wide_arrays.json | json | 19.802 | 22.571 | 34.851 | 51.980 | 0.12x |
| mixed.json | strata | 0.080 | 0.085 | 0.115 | 50.473 | 1.00x |
| mixed.json | orjson | 0.074 | 0.076 | 0.107 | 50.473 | 1.13x |
| mixed.json | msgspec | 0.103 | 0.109 | 0.147 | 50.473 | 0.78x |
| mixed.json | ujson | 0.328 | 0.344 | 0.503 | 50.473 | 0.25x |
| mixed.json | json | 0.524 | 0.553 | 0.898 | 50.473 | 0.15x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 12.820 | 14.788 | 20.726 | 54.785 | 1.00x |
| users.json | orjson | 16.340 | 17.605 | 28.080 | 54.785 | 0.84x |
| users.json | msgspec | 14.614 | 16.248 | 42.115 | 54.785 | 0.91x |
| users.json | ujson | 27.321 | 29.076 | 45.262 | 54.785 | 0.51x |
| users.json | json | 23.986 | 25.929 | 46.797 | 54.785 | 0.57x |
| flat.json | strata | 1.435 | 2.047 | 2.530 | 50.188 | 1.00x |
| flat.json | orjson | 1.578 | 2.225 | 2.700 | 50.188 | 0.92x |
| flat.json | msgspec | 1.284 | 1.947 | 2.253 | 50.188 | 1.05x |
| flat.json | ujson | 2.857 | 4.281 | 4.602 | 50.188 | 0.48x |
| flat.json | json | 2.274 | 3.706 | 4.240 | 50.188 | 0.55x |
| nested.json | strata | 0.918 | 1.011 | 1.503 | 50.809 | 1.00x |
| nested.json | orjson | 1.286 | 1.307 | 1.591 | 50.809 | 0.77x |
| nested.json | msgspec | 1.118 | 1.194 | 1.592 | 50.809 | 0.85x |
| nested.json | ujson | 1.899 | 2.007 | 3.011 | 50.809 | 0.50x |
| nested.json | json | 2.255 | 2.341 | 2.736 | 50.809 | 0.43x |
| wide_arrays.json | strata | 5.661 | 6.676 | 47.485 | 51.980 | 1.00x |
| wide_arrays.json | orjson | 6.973 | 7.494 | 72.276 | 51.980 | 0.89x |
| wide_arrays.json | msgspec | 7.025 | 7.897 | 17.082 | 51.980 | 0.85x |
| wide_arrays.json | ujson | 11.258 | 12.783 | 30.275 | 51.980 | 0.52x |
| wide_arrays.json | json | 12.718 | 14.206 | 30.358 | 51.980 | 0.47x |
| mixed.json | strata | 0.286 | 0.295 | 0.448 | 50.367 | 1.00x |
| mixed.json | orjson | 0.346 | 0.415 | 0.904 | 50.367 | 0.71x |
| mixed.json | msgspec | 0.352 | 0.367 | 1.632 | 50.367 | 0.80x |
| mixed.json | ujson | 0.524 | 0.705 | 0.838 | 50.367 | 0.42x |
| mixed.json | json | 0.583 | 0.620 | 0.938 | 50.367 | 0.48x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 14.499 | 16.776 | 30.678 | 51.605 | 1.00x |
| users.ndjson | orjson | 21.142 | 33.410 | 45.504 | 51.605 | 0.50x |
| users.ndjson | msgspec | 20.968 | 32.436 | 90.995 | 51.605 | 0.52x |
| users.ndjson | ujson | 30.431 | 33.687 | 83.977 | 51.605 | 0.50x |
| users.ndjson | json | 34.536 | 36.743 | 101.144 | 51.605 | 0.46x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.726 | 4.942 | 10.237 | 52.039 | 1.00x |
| users.json | orjson | 5.156 | 7.470 | 10.463 | 52.039 | 0.66x |
| users.json | msgspec | 7.231 | 10.036 | 11.311 | 52.039 | 0.49x |
| users.json | ujson | 26.167 | 35.660 | 68.037 | 52.039 | 0.14x |
| users.json | json | 33.438 | 46.492 | 111.504 | 52.039 | 0.11x |
| flat.json | strata | 0.708 | 0.764 | 0.892 | 50.957 | 1.00x |
| flat.json | orjson | 0.737 | 0.765 | 5.552 | 50.957 | 1.00x |
| flat.json | msgspec | 0.912 | 0.967 | 1.081 | 50.957 | 0.79x |
| flat.json | ujson | 3.050 | 3.126 | 3.337 | 50.957 | 0.24x |
| flat.json | json | 3.382 | 3.510 | 3.725 | 50.957 | 0.22x |
| nested.json | strata | 0.657 | 0.797 | 0.897 | 50.809 | 1.00x |
| nested.json | orjson | 0.711 | 0.888 | 1.042 | 50.809 | 0.90x |
| nested.json | msgspec | 0.887 | 1.038 | 1.220 | 50.809 | 0.77x |
| nested.json | ujson | 2.450 | 2.686 | 2.971 | 50.809 | 0.30x |
| nested.json | json | 3.721 | 4.010 | 4.493 | 50.809 | 0.20x |
| wide_arrays.json | strata | 3.239 | 5.743 | 7.554 | 51.980 | 1.00x |
| wide_arrays.json | orjson | 3.412 | 4.817 | 7.554 | 51.980 | 1.19x |
| wide_arrays.json | msgspec | 5.324 | 9.468 | 13.134 | 51.980 | 0.61x |
| wide_arrays.json | ujson | 17.462 | 33.978 | 42.721 | 51.980 | 0.17x |
| wide_arrays.json | json | 29.190 | 58.665 | 157.373 | 51.980 | 0.10x |
| mixed.json | strata | 0.398 | 0.490 | 0.605 | 50.473 | 1.00x |
| mixed.json | orjson | 0.404 | 0.535 | 0.633 | 50.473 | 0.92x |
| mixed.json | msgspec | 0.430 | 0.566 | 3.288 | 50.473 | 0.87x |
| mixed.json | ujson | 0.809 | 1.142 | 2.775 | 50.473 | 0.43x |
| mixed.json | json | 1.072 | 1.518 | 3.950 | 50.473 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.162 | 0.171 | 0.205 | 51.168 | 1.00x |
| users.json $[*].id | jmespath | 0.754 | 0.784 | 0.979 | 51.168 | 0.22x |
| users.json $[*].id | jsonpath-ng | 4.354 | 4.670 | 4.919 | 51.168 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.543 | 0.971 | 2.367 | 51.441 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.730 | 5.065 | 9.347 | 51.441 | 0.19x |
| users.json $[*].orders[*].total | jsonpath-ng | 18.615 | 36.726 | 68.724 | 51.441 | 0.03x |
| users.json $..total | strata | 3.246 | 3.481 | 4.428 | 52.445 | 1.00x |
| users.json $..total | jsonpath-ng | 490.282 | 603.554 | 790.159 | 52.445 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 6.091 | 6.396 | 9.796 | 51.242 | 1.00x |
| users.json $[*].id | orjson+jmespath | 24.920 | 29.350 | 84.477 | 51.242 | 0.22x |
| users.json $[*].id | orjson+jsonpath-ng | 28.062 | 36.047 | 97.047 | 51.242 | 0.18x |
| users.json $[*].orders[*].total | strata | 4.724 | 5.145 | 6.740 | 52.441 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 19.552 | 23.285 | 126.219 | 52.441 | 0.22x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 37.896 | 53.986 | 571.389 | 52.441 | 0.10x |
| users.json $..total | strata | 15.340 | 16.303 | 19.078 | 52.445 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 349.957 | 385.353 | 588.593 | 52.445 | 0.04x |

