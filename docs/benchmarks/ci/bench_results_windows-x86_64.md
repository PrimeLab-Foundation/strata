# Benchmark results - ci-windows-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 6ef7acf
- python: 3.12.10
- implementation: CPython
- platform: Windows-2025Server-10.0.26100-SP0
- machine: AMD64
- processor: AMD64 Family 25 Model 1 Stepping 1, AuthenticAMD
- compiler_flags: /std:c++20 /O2 /arch:AVX2 /GL /LTCG /USEPROFILE (PGO)
- repeats: 10
- warmup: 2

Excluded libraries (not installed, or no native equivalent):
- simdjson: not installed

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.629 | 10.773 | 18.868 | 45.383 | 1.00x |
| users.json | orjson | 13.135 | 14.009 | 26.984 | 45.383 | 0.77x |
| users.json | msgspec | 12.660 | 13.184 | 15.156 | 45.383 | 0.82x |
| users.json | ujson | 20.694 | 22.823 | 29.843 | 45.383 | 0.47x |
| users.json | json | 22.280 | 23.032 | 41.198 | 45.383 | 0.47x |
| flat.json | strata | 1.114 | 1.149 | 1.915 | 48.684 | 1.00x |
| flat.json | orjson | 1.169 | 1.345 | 2.313 | 48.684 | 0.85x |
| flat.json | msgspec | 1.119 | 1.196 | 1.626 | 48.684 | 0.96x |
| flat.json | ujson | 2.200 | 2.768 | 3.024 | 48.684 | 0.42x |
| flat.json | json | 1.985 | 2.112 | 3.336 | 48.684 | 0.54x |
| nested.json | strata | 0.821 | 1.142 | 1.632 | 48.426 | 1.00x |
| nested.json | orjson | 1.157 | 1.854 | 2.129 | 48.426 | 0.62x |
| nested.json | msgspec | 1.009 | 1.086 | 2.165 | 48.426 | 1.05x |
| nested.json | ujson | 1.680 | 2.736 | 6.371 | 48.426 | 0.42x |
| nested.json | json | 2.180 | 3.323 | 13.230 | 48.426 | 0.34x |
| wide_arrays.json | strata | 5.039 | 5.569 | 8.764 | 50.473 | 1.00x |
| wide_arrays.json | orjson | 6.590 | 7.395 | 11.451 | 50.473 | 0.75x |
| wide_arrays.json | msgspec | 6.051 | 6.711 | 13.478 | 50.473 | 0.83x |
| wide_arrays.json | ujson | 9.305 | 9.790 | 19.460 | 50.473 | 0.57x |
| wide_arrays.json | json | 12.872 | 13.874 | 16.048 | 50.473 | 0.40x |
| mixed.json | strata | 0.216 | 0.376 | 0.476 | 48.500 | 1.00x |
| mixed.json | orjson | 0.236 | 0.382 | 0.525 | 48.500 | 0.99x |
| mixed.json | msgspec | 0.247 | 0.441 | 0.543 | 48.500 | 0.85x |
| mixed.json | ujson | 0.419 | 0.725 | 0.793 | 48.500 | 0.52x |
| mixed.json | json | 0.511 | 0.860 | 1.143 | 48.500 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 3.240 | 3.328 | 3.399 | 44.582 | 1.00x |
| users.json | orjson | 3.597 | 3.803 | 3.879 | 44.582 | 0.88x |
| users.json | msgspec | 4.936 | 5.235 | 8.246 | 44.582 | 0.64x |
| users.json | ujson | 16.052 | 16.366 | 27.352 | 44.582 | 0.20x |
| users.json | json | 23.223 | 23.428 | 24.473 | 44.582 | 0.14x |
| flat.json | strata | 0.313 | 0.339 | 0.471 | 49.113 | 1.00x |
| flat.json | orjson | 0.356 | 0.451 | 0.542 | 49.113 | 0.75x |
| flat.json | msgspec | 0.496 | 0.623 | 0.794 | 49.113 | 0.54x |
| flat.json | ujson | 1.709 | 1.728 | 2.712 | 49.113 | 0.20x |
| flat.json | json | 1.916 | 2.754 | 3.392 | 49.113 | 0.12x |
| nested.json | strata | 0.295 | 0.388 | 0.494 | 48.965 | 1.00x |
| nested.json | orjson | 0.334 | 0.457 | 0.616 | 48.965 | 0.85x |
| nested.json | msgspec | 0.475 | 0.633 | 0.847 | 48.965 | 0.61x |
| nested.json | ujson | 1.330 | 2.018 | 2.584 | 48.965 | 0.19x |
| nested.json | json | 2.394 | 3.253 | 9.626 | 48.965 | 0.12x |
| wide_arrays.json | strata | 2.360 | 2.482 | 3.375 | 50.578 | 1.00x |
| wide_arrays.json | orjson | 2.682 | 3.086 | 3.414 | 50.578 | 0.80x |
| wide_arrays.json | msgspec | 4.333 | 4.686 | 4.976 | 50.578 | 0.53x |
| wide_arrays.json | ujson | 10.859 | 11.491 | 12.815 | 50.578 | 0.22x |
| wide_arrays.json | json | 20.293 | 21.271 | 40.330 | 50.578 | 0.12x |
| mixed.json | strata | 0.084 | 0.096 | 0.159 | 48.660 | 1.00x |
| mixed.json | orjson | 0.070 | 0.089 | 0.148 | 48.660 | 1.08x |
| mixed.json | msgspec | 0.098 | 0.108 | 0.317 | 48.660 | 0.89x |
| mixed.json | ujson | 0.340 | 0.361 | 0.668 | 48.660 | 0.26x |
| mixed.json | json | 0.532 | 0.581 | 1.112 | 48.660 | 0.16x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 11.090 | 11.781 | 12.795 | 54.609 | 1.00x |
| users.json | orjson | 14.116 | 14.718 | 15.165 | 54.609 | 0.80x |
| users.json | msgspec | 13.719 | 14.063 | 16.810 | 54.609 | 0.84x |
| users.json | ujson | 25.931 | 26.673 | 39.909 | 54.609 | 0.44x |
| users.json | json | 23.544 | 24.090 | 24.441 | 54.609 | 0.49x |
| flat.json | strata | 1.917 | 1.992 | 2.070 | 48.738 | 1.00x |
| flat.json | orjson | 1.888 | 2.054 | 2.209 | 48.738 | 0.97x |
| flat.json | msgspec | 2.013 | 2.134 | 2.243 | 48.738 | 0.93x |
| flat.json | ujson | 4.148 | 4.298 | 4.376 | 48.738 | 0.46x |
| flat.json | json | 3.794 | 3.932 | 4.299 | 48.738 | 0.51x |
| nested.json | strata | 1.106 | 1.637 | 2.200 | 48.512 | 1.00x |
| nested.json | orjson | 1.612 | 2.142 | 2.285 | 48.512 | 0.76x |
| nested.json | msgspec | 1.486 | 2.007 | 2.498 | 48.512 | 0.82x |
| nested.json | ujson | 3.519 | 3.587 | 4.316 | 48.512 | 0.46x |
| nested.json | json | 3.952 | 4.221 | 4.797 | 48.512 | 0.39x |
| wide_arrays.json | strata | 6.062 | 6.505 | 11.680 | 50.578 | 1.00x |
| wide_arrays.json | orjson | 7.343 | 8.083 | 14.609 | 50.578 | 0.80x |
| wide_arrays.json | msgspec | 7.278 | 7.608 | 8.246 | 50.578 | 0.85x |
| wide_arrays.json | ujson | 12.736 | 13.403 | 13.829 | 50.578 | 0.49x |
| wide_arrays.json | json | 13.915 | 14.818 | 31.362 | 50.578 | 0.44x |
| mixed.json | strata | 0.282 | 0.299 | 0.599 | 48.703 | 1.00x |
| mixed.json | orjson | 0.334 | 0.430 | 0.742 | 48.703 | 0.70x |
| mixed.json | msgspec | 0.354 | 0.546 | 0.741 | 48.703 | 0.55x |
| mixed.json | ujson | 0.573 | 0.646 | 1.051 | 48.703 | 0.46x |
| mixed.json | json | 0.592 | 0.746 | 1.274 | 48.703 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 15.804 | 17.934 | 23.631 | 49.762 | 1.00x |
| users.ndjson | orjson | 21.000 | 23.565 | 33.008 | 49.762 | 0.76x |
| users.ndjson | msgspec | 21.178 | 23.442 | 80.250 | 49.762 | 0.77x |
| users.ndjson | ujson | 31.832 | 42.044 | 85.095 | 49.762 | 0.43x |
| users.ndjson | json | 34.795 | 41.408 | 98.910 | 49.762 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 4.132 | 4.236 | 5.987 | 50.305 | 1.00x |
| users.json | orjson | 4.552 | 4.707 | 7.175 | 50.305 | 0.90x |
| users.json | msgspec | 5.778 | 6.090 | 9.203 | 50.305 | 0.70x |
| users.json | ujson | 25.617 | 26.098 | 28.409 | 50.305 | 0.16x |
| users.json | json | 32.415 | 32.643 | 39.762 | 50.305 | 0.13x |
| flat.json | strata | 1.067 | 1.207 | 2.109 | 48.930 | 1.00x |
| flat.json | orjson | 1.160 | 1.199 | 1.608 | 48.930 | 1.01x |
| flat.json | msgspec | 1.270 | 1.387 | 2.060 | 48.930 | 0.87x |
| flat.json | ujson | 5.059 | 5.129 | 19.638 | 48.930 | 0.24x |
| flat.json | json | 5.882 | 6.178 | 6.508 | 48.930 | 0.20x |
| nested.json | strata | 0.668 | 0.920 | 1.187 | 48.961 | 1.00x |
| nested.json | orjson | 0.703 | 0.921 | 1.197 | 48.961 | 1.00x |
| nested.json | msgspec | 0.876 | 1.259 | 1.616 | 48.961 | 0.73x |
| nested.json | ujson | 2.445 | 3.455 | 4.573 | 48.961 | 0.27x |
| nested.json | json | 3.669 | 5.747 | 6.360 | 48.961 | 0.16x |
| wide_arrays.json | strata | 3.374 | 4.048 | 5.103 | 50.578 | 1.00x |
| wide_arrays.json | orjson | 3.296 | 4.099 | 14.667 | 50.578 | 0.99x |
| wide_arrays.json | msgspec | 5.057 | 6.274 | 12.848 | 50.578 | 0.65x |
| wide_arrays.json | ujson | 20.140 | 21.696 | 41.123 | 50.578 | 0.19x |
| wide_arrays.json | json | 31.651 | 32.722 | 89.376 | 50.578 | 0.12x |
| mixed.json | strata | 0.402 | 0.582 | 0.730 | 48.680 | 1.00x |
| mixed.json | orjson | 0.397 | 0.500 | 0.731 | 48.680 | 1.16x |
| mixed.json | msgspec | 0.436 | 0.490 | 0.817 | 48.680 | 1.19x |
| mixed.json | ujson | 0.842 | 0.972 | 1.547 | 48.680 | 0.60x |
| mixed.json | json | 1.053 | 1.302 | 1.951 | 48.680 | 0.45x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.089 | 0.093 | 0.101 | 50.520 | 1.00x |
| users.json $[*].id | jmespath | 0.437 | 0.467 | 0.501 | 50.520 | 0.20x |
| users.json $[*].id | jsonpath-ng | 2.514 | 2.695 | 2.866 | 50.520 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.475 | 0.498 | 0.602 | 50.797 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.753 | 2.813 | 2.926 | 50.797 | 0.18x |
| users.json $[*].orders[*].total | jsonpath-ng | 17.361 | 17.800 | 31.880 | 50.797 | 0.03x |
| users.json $..total | strata | 1.847 | 1.880 | 4.132 | 52.793 | 1.00x |
| users.json $..total | jsonpath-ng | 341.116 | 364.691 | 389.087 | 52.793 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 4.058 | 4.092 | 4.152 | 50.523 | 1.00x |
| users.json $[*].id | orjson+jmespath | 14.785 | 15.040 | 23.262 | 50.523 | 0.27x |
| users.json $[*].id | orjson+jsonpath-ng | 16.736 | 17.100 | 21.318 | 50.523 | 0.24x |
| users.json $[*].orders[*].total | strata | 4.327 | 4.423 | 4.798 | 52.711 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 17.266 | 18.458 | 30.332 | 52.711 | 0.24x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 34.634 | 37.193 | 42.119 | 52.711 | 0.12x |
| users.json $..total | strata | 15.572 | 20.101 | 35.111 | 52.418 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 364.282 | 602.266 | 912.070 | 52.418 | 0.03x |

