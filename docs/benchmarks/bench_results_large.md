# Benchmark results - large

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
| users.json | strata | 279.877 | 303.917 | 333.557 | 493.469 | 1.00x |
| users.json | orjson | 393.610 | 407.812 | 445.827 | 493.469 | 0.75x |
| users.json | msgspec | 396.819 | 419.499 | 449.956 | 493.469 | 0.72x |
| users.json | ujson | 552.117 | 562.981 | 579.175 | 493.469 | 0.54x |
| users.json | json | 798.257 | 818.273 | 841.699 | 493.469 | 0.37x |
| flat.json | strata | 5.539 | 5.581 | 5.604 | 355.078 | 1.00x |
| flat.json | orjson | 6.931 | 6.958 | 6.993 | 355.078 | 0.80x |
| flat.json | msgspec | 7.298 | 7.334 | 7.401 | 355.078 | 0.76x |
| flat.json | ujson | 10.761 | 10.863 | 10.939 | 355.078 | 0.51x |
| flat.json | json | 14.475 | 14.546 | 14.608 | 355.078 | 0.38x |
| nested.json | strata | 4.905 | 4.953 | 5.291 | 357.250 | 1.00x |
| nested.json | orjson | 6.131 | 6.268 | 6.704 | 357.250 | 0.79x |
| nested.json | msgspec | 6.115 | 6.279 | 6.671 | 357.250 | 0.79x |
| nested.json | ujson | 9.028 | 9.323 | 9.653 | 357.250 | 0.53x |
| nested.json | json | 13.978 | 14.187 | 14.950 | 357.250 | 0.35x |
| wide_arrays.json | strata | 29.936 | 30.882 | 31.757 | 428.250 | 1.00x |
| wide_arrays.json | orjson | 35.170 | 37.141 | 39.752 | 428.250 | 0.83x |
| wide_arrays.json | msgspec | 42.662 | 43.889 | 46.974 | 428.250 | 0.70x |
| wide_arrays.json | ujson | 55.241 | 55.889 | 59.346 | 428.250 | 0.55x |
| wide_arrays.json | json | 79.549 | 82.144 | 83.633 | 428.250 | 0.38x |
| mixed.json | strata | 1.167 | 1.250 | 1.346 | 810.125 | 1.00x |
| mixed.json | orjson | 1.492 | 1.572 | 1.661 | 810.125 | 0.80x |
| mixed.json | msgspec | 1.621 | 1.803 | 2.122 | 810.125 | 0.69x |
| mixed.json | ujson | 2.004 | 2.123 | 2.359 | 810.125 | 0.59x |
| mixed.json | json | 3.314 | 3.757 | 3.919 | 810.125 | 0.33x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 56.439 | 58.572 | 66.192 | 3111.469 | 1.00x |
| users.json | orjson | 85.515 | 88.178 | 94.795 | 3111.469 | 0.66x |
| users.json | msgspec | 112.420 | 115.844 | 123.831 | 3111.469 | 0.51x |
| users.json | ujson | 450.211 | 452.796 | 466.505 | 3111.469 | 0.13x |
| users.json | json | 636.729 | 638.166 | 648.980 | 3111.469 | 0.09x |
| flat.json | strata | 1.846 | 1.874 | 2.019 | 356.984 | 1.00x |
| flat.json | orjson | 2.144 | 2.158 | 2.372 | 356.984 | 0.87x |
| flat.json | msgspec | 2.835 | 2.861 | 3.184 | 356.984 | 0.66x |
| flat.json | ujson | 8.813 | 8.858 | 9.293 | 356.984 | 0.21x |
| flat.json | json | 13.055 | 13.174 | 14.314 | 356.984 | 0.14x |
| nested.json | strata | 1.264 | 1.273 | 1.346 | 357.328 | 1.00x |
| nested.json | orjson | 1.993 | 2.023 | 2.190 | 357.328 | 0.63x |
| nested.json | msgspec | 2.631 | 2.680 | 2.802 | 357.328 | 0.48x |
| nested.json | ujson | 9.648 | 9.711 | 10.095 | 357.328 | 0.13x |
| nested.json | json | 15.220 | 15.466 | 15.972 | 357.328 | 0.08x |
| wide_arrays.json | strata | 10.245 | 10.477 | 11.094 | 594.344 | 1.00x |
| wide_arrays.json | orjson | 12.316 | 12.519 | 13.521 | 594.344 | 0.84x |
| wide_arrays.json | msgspec | 19.957 | 20.583 | 21.825 | 594.344 | 0.51x |
| wide_arrays.json | ujson | 62.549 | 62.995 | 64.952 | 594.344 | 0.17x |
| wide_arrays.json | json | 114.325 | 115.557 | 117.522 | 594.344 | 0.09x |
| mixed.json | strata | 0.303 | 0.309 | 0.407 | 810.188 | 1.00x |
| mixed.json | orjson | 0.378 | 0.387 | 0.442 | 810.188 | 0.80x |
| mixed.json | msgspec | 0.412 | 0.433 | 0.566 | 810.188 | 0.72x |
| mixed.json | ujson | 2.063 | 2.153 | 2.356 | 810.188 | 0.14x |
| mixed.json | json | 3.368 | 3.751 | 3.933 | 810.188 | 0.08x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 315.950 | 321.184 | 375.866 | 902.656 | 1.00x |
| users.json | orjson | 421.573 | 442.922 | 470.477 | 902.656 | 0.73x |
| users.json | msgspec | 430.774 | 437.418 | 460.942 | 902.656 | 0.73x |
| users.json | ujson | 581.143 | 595.363 | 619.684 | 902.656 | 0.54x |
| users.json | json | 825.608 | 841.361 | 897.640 | 902.656 | 0.38x |
| flat.json | strata | 5.861 | 6.036 | 6.118 | 357.234 | 1.00x |
| flat.json | orjson | 7.158 | 7.406 | 7.659 | 357.234 | 0.82x |
| flat.json | msgspec | 7.673 | 8.009 | 8.245 | 357.234 | 0.75x |
| flat.json | ujson | 11.291 | 11.830 | 12.176 | 357.234 | 0.51x |
| flat.json | json | 14.881 | 15.352 | 15.593 | 357.234 | 0.39x |
| nested.json | strata | 5.217 | 5.380 | 5.443 | 360.234 | 1.00x |
| nested.json | orjson | 6.319 | 6.511 | 6.599 | 360.234 | 0.83x |
| nested.json | msgspec | 6.387 | 6.677 | 6.805 | 360.234 | 0.81x |
| nested.json | ujson | 9.562 | 9.714 | 9.876 | 360.234 | 0.55x |
| nested.json | json | 14.404 | 14.744 | 14.987 | 360.234 | 0.36x |
| wide_arrays.json | strata | 31.906 | 32.541 | 33.511 | 604.344 | 1.00x |
| wide_arrays.json | orjson | 37.667 | 37.980 | 38.689 | 604.344 | 0.86x |
| wide_arrays.json | msgspec | 45.458 | 46.319 | 49.109 | 604.344 | 0.70x |
| wide_arrays.json | ujson | 59.757 | 60.638 | 61.412 | 604.344 | 0.54x |
| wide_arrays.json | json | 82.638 | 83.082 | 85.391 | 604.344 | 0.39x |
| mixed.json | strata | 1.262 | 1.327 | 1.437 | 810.188 | 1.00x |
| mixed.json | orjson | 1.556 | 1.587 | 1.798 | 810.188 | 0.84x |
| mixed.json | msgspec | 1.682 | 1.751 | 1.945 | 810.188 | 0.76x |
| mixed.json | ujson | 2.137 | 2.241 | 2.369 | 810.188 | 0.59x |
| mixed.json | json | 3.362 | 3.511 | 3.586 | 810.188 | 0.38x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 299.670 | 312.317 | 333.455 | 385.391 | 1.00x |
| users.ndjson | orjson | 702.535 | 718.906 | 748.834 | 385.391 | 0.43x |
| users.ndjson | msgspec | 699.773 | 713.257 | 730.827 | 385.391 | 0.44x |
| users.ndjson | ujson | 894.500 | 910.885 | 932.396 | 385.391 | 0.34x |
| users.ndjson | json | 1060.529 | 1072.399 | 1101.876 | 385.391 | 0.29x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 65.872 | 70.822 | 97.731 | 1517.766 | 1.00x |
| users.json | orjson | 98.914 | 101.352 | 120.635 | 1517.766 | 0.70x |
| users.json | msgspec | 123.827 | 129.191 | 164.506 | 1517.766 | 0.55x |
| users.json | ujson | 451.532 | 456.995 | 471.540 | 1517.766 | 0.15x |
| users.json | json | 648.375 | 652.330 | 692.095 | 1517.766 | 0.11x |
| flat.json | strata | 2.280 | 2.430 | 2.575 | 357.234 | 1.00x |
| flat.json | orjson | 2.640 | 2.847 | 2.989 | 357.234 | 0.85x |
| flat.json | msgspec | 3.331 | 3.623 | 3.836 | 357.234 | 0.67x |
| flat.json | ujson | 9.551 | 9.806 | 10.176 | 357.234 | 0.25x |
| flat.json | json | 14.157 | 14.240 | 14.492 | 357.234 | 0.17x |
| nested.json | strata | 1.683 | 1.799 | 1.963 | 360.234 | 1.00x |
| nested.json | orjson | 2.492 | 2.594 | 2.913 | 360.234 | 0.69x |
| nested.json | msgspec | 3.141 | 3.249 | 3.399 | 360.234 | 0.55x |
| nested.json | ujson | 10.275 | 10.470 | 10.713 | 360.234 | 0.17x |
| nested.json | json | 15.957 | 16.360 | 17.182 | 360.234 | 0.11x |
| wide_arrays.json | strata | 12.549 | 13.085 | 14.261 | 813.078 | 1.00x |
| wide_arrays.json | orjson | 14.554 | 14.932 | 15.608 | 813.078 | 0.88x |
| wide_arrays.json | msgspec | 22.403 | 22.994 | 24.692 | 813.078 | 0.57x |
| wide_arrays.json | ujson | 64.938 | 65.550 | 67.275 | 813.078 | 0.20x |
| wide_arrays.json | json | 117.955 | 118.552 | 120.360 | 813.078 | 0.11x |
| mixed.json | strata | 0.508 | 0.560 | 0.623 | 810.188 | 1.00x |
| mixed.json | orjson | 0.576 | 0.610 | 0.651 | 810.188 | 0.92x |
| mixed.json | msgspec | 0.715 | 0.778 | 0.863 | 810.188 | 0.72x |
| mixed.json | ujson | 2.312 | 2.429 | 2.578 | 810.188 | 0.23x |
| mixed.json | json | 3.879 | 4.107 | 4.250 | 810.188 | 0.14x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.728 | 0.757 | 0.829 | 930.094 | 1.00x |
| users.json $[*].id | jmespath | 1.919 | 1.953 | 2.117 | 930.094 | 0.39x |
| users.json $[*].id | jsonpath-ng | 7.150 | 7.263 | 17.960 | 930.094 | 0.10x |
| users.json $[*].orders[*].total | strata | 14.319 | 14.732 | 16.508 | 538.734 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 32.830 | 33.872 | 43.811 | 538.734 | 0.43x |
| users.json $[*].orders[*].total | jsonpath-ng | 161.070 | 167.921 | 176.123 | 538.734 | 0.09x |
| users.json $..total | strata | 55.259 | 55.972 | 57.631 | 625.266 | 1.00x |
| users.json $..total | jsonpath-ng | 7810.383 | 7837.594 | 7861.864 | 625.266 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 136.319 | 137.356 | 144.346 | 1000.953 | 1.00x |
| users.json $[*].id | orjson+jmespath | 404.385 | 412.185 | 426.586 | 1000.953 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 493.698 | 505.387 | 529.306 | 1000.953 | 0.27x |
| users.json $[*].orders[*].total | strata | 137.089 | 137.778 | 143.410 | 849.188 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 431.155 | 447.824 | 463.405 | 849.188 | 0.31x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 1199.237 | 1227.491 | 1295.508 | 849.188 | 0.11x |
| users.json $..total | strata | 354.567 | 358.680 | 363.056 | 863.766 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8862.931 | 8890.808 | 8934.131 | 863.766 | 0.04x |

