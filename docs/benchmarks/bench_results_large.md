# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: e0e780e
- python: 3.14.3
- implementation: CPython
- platform: macOS-26.3-arm64-arm-64bit-Mach-O
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
| users.json | strata | 353.146 | 359.305 | 364.633 | 280.109 | 1.00x |
| users.json | orjson | 405.263 | 413.077 | 417.502 | 280.109 | 0.87x |
| users.json | msgspec | 414.005 | 421.690 | 429.161 | 280.109 | 0.85x |
| users.json | ujson | 564.012 | 568.854 | 577.289 | 280.109 | 0.63x |
| users.json | json | 687.022 | 695.571 | 701.586 | 280.109 | 0.52x |
| flat.json | strata | 6.539 | 6.624 | 6.969 | 347.625 | 1.00x |
| flat.json | orjson | 7.970 | 8.336 | 8.784 | 347.625 | 0.79x |
| flat.json | msgspec | 8.341 | 8.855 | 9.175 | 347.625 | 0.75x |
| flat.json | ujson | 11.741 | 12.061 | 12.867 | 347.625 | 0.55x |
| flat.json | json | 15.696 | 15.956 | 16.130 | 347.625 | 0.42x |
| nested.json | strata | 5.627 | 5.875 | 6.093 | 349.859 | 1.00x |
| nested.json | orjson | 6.815 | 6.982 | 7.459 | 349.859 | 0.84x |
| nested.json | msgspec | 6.795 | 6.882 | 7.510 | 349.859 | 0.85x |
| nested.json | ujson | 9.647 | 10.179 | 10.360 | 349.859 | 0.58x |
| nested.json | json | 14.044 | 14.585 | 15.026 | 349.859 | 0.40x |
| wide_arrays.json | strata | 33.206 | 33.345 | 34.081 | 420.922 | 1.00x |
| wide_arrays.json | orjson | 32.914 | 33.137 | 34.222 | 420.922 | 1.01x |
| wide_arrays.json | msgspec | 40.647 | 41.486 | 42.036 | 420.922 | 0.80x |
| wide_arrays.json | ujson | 53.055 | 53.417 | 54.880 | 420.922 | 0.62x |
| wide_arrays.json | json | 76.419 | 77.057 | 78.251 | 420.922 | 0.43x |
| mixed.json | strata | 1.314 | 1.329 | 1.526 | 774.547 | 1.00x |
| mixed.json | orjson | 2.076 | 2.160 | 2.371 | 774.547 | 0.62x |
| mixed.json | msgspec | 2.204 | 2.270 | 2.669 | 774.547 | 0.59x |
| mixed.json | ujson | 2.608 | 2.724 | 3.077 | 774.547 | 0.49x |
| mixed.json | json | 3.718 | 3.797 | 4.270 | 774.547 | 0.35x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 86.012 | 88.534 | 89.737 | 1421.812 | 1.00x |
| users.json | orjson | 83.723 | 88.832 | 94.410 | 1421.812 | 1.00x |
| users.json | msgspec | 114.320 | 117.511 | 124.054 | 1421.812 | 0.75x |
| users.json | ujson | 453.198 | 461.031 | 482.241 | 1421.812 | 0.19x |
| users.json | json | 628.114 | 632.690 | 635.868 | 1421.812 | 0.14x |
| flat.json | strata | 2.019 | 2.076 | 2.166 | 349.547 | 1.00x |
| flat.json | orjson | 2.140 | 2.203 | 2.328 | 349.547 | 0.94x |
| flat.json | msgspec | 2.816 | 2.907 | 3.032 | 349.547 | 0.71x |
| flat.json | ujson | 8.884 | 8.956 | 9.355 | 349.547 | 0.23x |
| flat.json | json | 12.247 | 12.608 | 12.973 | 349.547 | 0.16x |
| nested.json | strata | 1.514 | 1.571 | 1.649 | 349.875 | 1.00x |
| nested.json | orjson | 2.002 | 2.051 | 2.152 | 349.875 | 0.77x |
| nested.json | msgspec | 2.589 | 2.661 | 2.799 | 349.875 | 0.59x |
| nested.json | ujson | 9.583 | 9.719 | 10.126 | 349.875 | 0.16x |
| nested.json | json | 14.816 | 15.056 | 15.550 | 349.875 | 0.10x |
| wide_arrays.json | strata | 12.655 | 12.821 | 13.389 | 625.906 | 1.00x |
| wide_arrays.json | orjson | 11.514 | 11.676 | 11.807 | 625.906 | 1.10x |
| wide_arrays.json | msgspec | 19.759 | 20.176 | 20.727 | 625.906 | 0.64x |
| wide_arrays.json | ujson | 62.817 | 63.341 | 64.501 | 625.906 | 0.20x |
| wide_arrays.json | json | 108.367 | 108.897 | 110.045 | 625.906 | 0.12x |
| mixed.json | strata | 0.376 | 0.381 | 0.423 | 774.562 | 1.00x |
| mixed.json | orjson | 0.372 | 0.374 | 0.388 | 774.562 | 1.02x |
| mixed.json | msgspec | 0.407 | 0.408 | 0.444 | 774.562 | 0.93x |
| mixed.json | ujson | 2.059 | 2.076 | 2.186 | 774.562 | 0.18x |
| mixed.json | json | 3.143 | 3.169 | 3.284 | 774.562 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 355.795 | 358.029 | 373.911 | 902.672 | 1.00x |
| users.json | orjson | 468.650 | 479.441 | 532.635 | 902.672 | 0.75x |
| users.json | msgspec | 474.403 | 484.825 | 510.814 | 902.672 | 0.74x |
| users.json | ujson | 621.190 | 631.899 | 658.286 | 902.672 | 0.57x |
| users.json | json | 740.292 | 744.575 | 779.285 | 902.672 | 0.48x |
| flat.json | strata | 6.883 | 7.116 | 7.179 | 349.844 | 1.00x |
| flat.json | orjson | 8.754 | 9.003 | 9.663 | 349.844 | 0.79x |
| flat.json | msgspec | 9.090 | 9.508 | 9.862 | 349.844 | 0.75x |
| flat.json | ujson | 12.636 | 13.068 | 14.032 | 349.844 | 0.54x |
| flat.json | json | 15.997 | 16.803 | 17.526 | 349.844 | 0.42x |
| nested.json | strata | 5.859 | 6.069 | 6.487 | 352.844 | 1.00x |
| nested.json | orjson | 8.420 | 8.657 | 9.564 | 352.844 | 0.70x |
| nested.json | msgspec | 8.533 | 9.031 | 9.347 | 352.844 | 0.67x |
| nested.json | ujson | 11.515 | 12.062 | 12.658 | 352.844 | 0.50x |
| nested.json | json | 15.632 | 16.331 | 17.032 | 352.844 | 0.37x |
| wide_arrays.json | strata | 34.496 | 34.972 | 35.795 | 640.906 | 1.00x |
| wide_arrays.json | orjson | 38.521 | 38.798 | 40.055 | 640.906 | 0.90x |
| wide_arrays.json | msgspec | 46.352 | 47.088 | 48.405 | 640.906 | 0.74x |
| wide_arrays.json | ujson | 59.832 | 60.189 | 61.239 | 640.906 | 0.58x |
| wide_arrays.json | json | 81.427 | 82.140 | 82.700 | 640.906 | 0.43x |
| mixed.json | strata | 1.431 | 1.460 | 1.516 | 774.562 | 1.00x |
| mixed.json | orjson | 2.393 | 2.499 | 2.718 | 774.562 | 0.58x |
| mixed.json | msgspec | 2.469 | 2.560 | 2.916 | 774.562 | 0.57x |
| mixed.json | ujson | 2.929 | 3.017 | 3.354 | 774.562 | 0.48x |
| mixed.json | json | 3.998 | 4.040 | 4.328 | 774.562 | 0.36x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 341.907 | 344.873 | 356.904 | 373.906 | 1.00x |
| users.ndjson | orjson | 526.873 | 531.759 | 545.705 | 373.906 | 0.65x |
| users.ndjson | msgspec | 533.665 | 536.226 | 552.137 | 373.906 | 0.64x |
| users.ndjson | ujson | 686.616 | 696.526 | 708.669 | 373.906 | 0.50x |
| users.ndjson | json | 801.286 | 803.181 | 809.653 | 373.906 | 0.43x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 93.865 | 95.055 | 97.334 | 2465.844 | 1.00x |
| users.json | orjson | 93.270 | 93.937 | 94.613 | 2465.844 | 1.01x |
| users.json | msgspec | 119.090 | 120.431 | 125.805 | 2465.844 | 0.79x |
| users.json | ujson | 441.868 | 446.559 | 463.365 | 2465.844 | 0.21x |
| users.json | json | 607.523 | 620.269 | 638.817 | 2465.844 | 0.15x |
| flat.json | strata | 2.433 | 2.611 | 2.799 | 349.844 | 1.00x |
| flat.json | orjson | 2.516 | 2.685 | 2.777 | 349.844 | 0.97x |
| flat.json | msgspec | 3.235 | 3.284 | 3.344 | 349.844 | 0.79x |
| flat.json | ujson | 9.363 | 9.615 | 9.890 | 349.844 | 0.27x |
| flat.json | json | 12.751 | 13.017 | 13.431 | 349.844 | 0.20x |
| nested.json | strata | 1.844 | 1.976 | 2.355 | 352.844 | 1.00x |
| nested.json | orjson | 2.359 | 2.521 | 2.902 | 352.844 | 0.78x |
| nested.json | msgspec | 2.908 | 3.081 | 3.604 | 352.844 | 0.64x |
| nested.json | ujson | 9.939 | 10.228 | 11.249 | 352.844 | 0.19x |
| nested.json | json | 15.306 | 15.647 | 16.211 | 352.844 | 0.13x |
| wide_arrays.json | strata | 14.138 | 14.535 | 15.327 | 773.391 | 1.00x |
| wide_arrays.json | orjson | 13.270 | 13.710 | 13.796 | 773.391 | 1.06x |
| wide_arrays.json | msgspec | 21.960 | 22.382 | 22.546 | 773.391 | 0.65x |
| wide_arrays.json | ujson | 65.577 | 66.448 | 67.723 | 773.391 | 0.22x |
| wide_arrays.json | json | 111.154 | 112.160 | 112.861 | 773.391 | 0.13x |
| mixed.json | strata | 0.524 | 0.565 | 0.577 | 774.562 | 1.00x |
| mixed.json | orjson | 0.522 | 0.539 | 0.785 | 774.562 | 1.05x |
| mixed.json | msgspec | 0.569 | 0.582 | 0.992 | 774.562 | 0.97x |
| mixed.json | ujson | 2.234 | 2.286 | 2.736 | 774.562 | 0.25x |
| mixed.json | json | 3.334 | 3.394 | 3.577 | 774.562 | 0.17x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.679 | 0.691 | 0.717 | 517.562 | 1.00x |
| users.json $[*].id | jmespath | 2.020 | 2.065 | 14.778 | 517.562 | 0.33x |
| users.json $[*].id | jsonpath-ng | 71.350 | 72.144 | 86.346 | 517.562 | 0.01x |
| users.json $[*].orders[*].total | strata | 13.284 | 13.913 | 16.945 | 603.516 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 34.937 | 36.678 | 40.769 | 603.516 | 0.38x |
| users.json $[*].orders[*].total | jsonpath-ng | 217.837 | 225.110 | 240.016 | 603.516 | 0.06x |
| users.json $..total | strata | 53.021 | 53.400 | 53.618 | 493.766 | 1.00x |
| users.json $..total | jsonpath-ng | 8553.271 | 8578.043 | 8611.011 | 493.766 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 132.820 | 133.406 | 135.239 | 824.016 | 1.00x |
| users.json $[*].id | orjson+jmespath | 449.203 | 454.838 | 462.431 | 824.016 | 0.29x |
| users.json $[*].id | orjson+jsonpath-ng | 455.351 | 458.813 | 470.902 | 824.016 | 0.29x |
| users.json $[*].orders[*].total | strata | 135.024 | 137.191 | 148.945 | 787.969 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 491.026 | 497.676 | 513.993 | 787.969 | 0.28x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 626.179 | 635.024 | 656.342 | 787.969 | 0.22x |
| users.json $..total | strata | 453.737 | 456.134 | 459.463 | 798.656 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8919.455 | 8928.686 | 9193.039 | 798.656 | 0.05x |

