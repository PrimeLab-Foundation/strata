# Benchmark results - large

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 688ea51
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
| users.json | strata | 331.114 | 334.379 | 510.685 | 493.281 | 1.00x |
| users.json | orjson | 389.219 | 395.372 | 588.426 | 493.281 | 0.85x |
| users.json | msgspec | 397.253 | 420.124 | 593.084 | 493.281 | 0.80x |
| users.json | ujson | 535.869 | 602.349 | 833.785 | 493.281 | 0.56x |
| users.json | json | 647.177 | 757.906 | 1076.924 | 493.281 | 0.44x |
| flat.json | strata | 6.720 | 6.865 | 6.982 | 353.672 | 1.00x |
| flat.json | orjson | 8.203 | 8.350 | 8.483 | 353.672 | 0.82x |
| flat.json | msgspec | 8.121 | 8.258 | 8.494 | 353.672 | 0.83x |
| flat.json | ujson | 11.770 | 11.968 | 12.409 | 353.672 | 0.57x |
| flat.json | json | 15.672 | 15.915 | 16.024 | 353.672 | 0.43x |
| nested.json | strata | 5.405 | 5.530 | 5.661 | 355.656 | 1.00x |
| nested.json | orjson | 6.825 | 6.893 | 7.214 | 355.656 | 0.80x |
| nested.json | msgspec | 6.752 | 6.893 | 6.952 | 355.656 | 0.80x |
| nested.json | ujson | 9.585 | 9.636 | 9.854 | 355.656 | 0.57x |
| nested.json | json | 14.003 | 14.259 | 14.973 | 355.656 | 0.39x |
| wide_arrays.json | strata | 33.800 | 33.995 | 34.244 | 425.422 | 1.00x |
| wide_arrays.json | orjson | 32.610 | 33.159 | 33.690 | 425.422 | 1.03x |
| wide_arrays.json | msgspec | 40.689 | 41.049 | 41.943 | 425.422 | 0.83x |
| wide_arrays.json | ujson | 52.228 | 52.672 | 53.436 | 425.422 | 0.65x |
| wide_arrays.json | json | 75.894 | 76.558 | 76.856 | 425.422 | 0.44x |
| mixed.json | strata | 1.278 | 1.298 | 1.412 | 769.875 | 1.00x |
| mixed.json | orjson | 2.072 | 2.143 | 2.396 | 769.875 | 0.61x |
| mixed.json | msgspec | 2.211 | 2.225 | 2.449 | 769.875 | 0.58x |
| mixed.json | ujson | 2.576 | 2.621 | 2.912 | 769.875 | 0.50x |
| mixed.json | json | 3.685 | 3.766 | 4.107 | 769.875 | 0.34x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 86.172 | 97.629 | 116.374 | 1202.203 | 1.00x |
| users.json | orjson | 103.247 | 120.805 | 214.188 | 1202.203 | 0.81x |
| users.json | msgspec | 136.257 | 159.836 | 257.378 | 1202.203 | 0.61x |
| users.json | ujson | 609.246 | 622.518 | 670.094 | 1202.203 | 0.16x |
| users.json | json | 865.803 | 969.420 | 1003.034 | 1202.203 | 0.10x |
| flat.json | strata | 1.698 | 1.732 | 1.750 | 355.641 | 1.00x |
| flat.json | orjson | 2.121 | 2.176 | 2.205 | 355.641 | 0.80x |
| flat.json | msgspec | 2.812 | 2.864 | 2.906 | 355.641 | 0.60x |
| flat.json | ujson | 8.481 | 8.662 | 8.803 | 355.641 | 0.20x |
| flat.json | json | 12.242 | 12.406 | 12.496 | 355.641 | 0.14x |
| nested.json | strata | 1.222 | 1.240 | 1.276 | 355.672 | 1.00x |
| nested.json | orjson | 2.002 | 2.037 | 2.074 | 355.672 | 0.61x |
| nested.json | msgspec | 2.589 | 2.632 | 2.657 | 355.672 | 0.47x |
| nested.json | ujson | 9.483 | 9.638 | 9.861 | 355.672 | 0.13x |
| nested.json | json | 14.854 | 15.038 | 15.135 | 355.672 | 0.08x |
| wide_arrays.json | strata | 12.282 | 12.708 | 12.892 | 621.375 | 1.00x |
| wide_arrays.json | orjson | 11.447 | 11.646 | 12.078 | 621.375 | 1.09x |
| wide_arrays.json | msgspec | 19.760 | 20.041 | 20.484 | 621.375 | 0.63x |
| wide_arrays.json | ujson | 62.460 | 62.629 | 63.546 | 621.375 | 0.20x |
| wide_arrays.json | json | 108.259 | 109.169 | 109.742 | 621.375 | 0.12x |
| mixed.json | strata | 0.310 | 0.320 | 0.331 | 769.891 | 1.00x |
| mixed.json | orjson | 0.373 | 0.376 | 0.421 | 769.891 | 0.85x |
| mixed.json | msgspec | 0.408 | 0.416 | 0.466 | 769.891 | 0.77x |
| mixed.json | ujson | 2.031 | 2.071 | 2.292 | 769.891 | 0.15x |
| mixed.json | json | 3.111 | 3.147 | 3.373 | 769.891 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 521.873 | 531.085 | 589.169 | 837.109 | 1.00x |
| users.json | orjson | 622.244 | 652.229 | 677.139 | 837.109 | 0.81x |
| users.json | msgspec | 673.776 | 686.634 | 695.182 | 837.109 | 0.77x |
| users.json | ujson | 937.465 | 972.750 | 1015.948 | 837.109 | 0.55x |
| users.json | json | 1121.921 | 1148.664 | 1206.713 | 837.109 | 0.46x |
| flat.json | strata | 7.108 | 7.237 | 7.285 | 355.641 | 1.00x |
| flat.json | orjson | 9.032 | 9.237 | 9.322 | 355.641 | 0.78x |
| flat.json | msgspec | 9.036 | 9.276 | 9.466 | 355.641 | 0.78x |
| flat.json | ujson | 13.077 | 13.296 | 13.675 | 355.641 | 0.54x |
| flat.json | json | 16.475 | 16.855 | 17.541 | 355.641 | 0.43x |
| nested.json | strata | 5.577 | 5.720 | 5.798 | 357.406 | 1.00x |
| nested.json | orjson | 8.417 | 8.575 | 8.793 | 357.406 | 0.67x |
| nested.json | msgspec | 8.516 | 8.658 | 9.088 | 357.406 | 0.66x |
| nested.json | ujson | 11.469 | 11.704 | 11.993 | 357.406 | 0.49x |
| nested.json | json | 15.788 | 16.032 | 16.655 | 357.406 | 0.36x |
| wide_arrays.json | strata | 35.000 | 35.306 | 35.825 | 632.375 | 1.00x |
| wide_arrays.json | orjson | 38.150 | 38.583 | 39.806 | 632.375 | 0.92x |
| wide_arrays.json | msgspec | 46.334 | 46.786 | 49.034 | 632.375 | 0.75x |
| wide_arrays.json | ujson | 59.439 | 59.925 | 61.003 | 632.375 | 0.59x |
| wide_arrays.json | json | 81.036 | 81.737 | 82.597 | 632.375 | 0.43x |
| mixed.json | strata | 1.363 | 1.398 | 1.486 | 770.250 | 1.00x |
| mixed.json | orjson | 2.380 | 2.478 | 2.880 | 770.250 | 0.56x |
| mixed.json | msgspec | 2.476 | 2.554 | 2.784 | 770.250 | 0.55x |
| mixed.json | ujson | 2.922 | 3.076 | 3.226 | 770.250 | 0.45x |
| mixed.json | json | 3.965 | 4.059 | 4.373 | 770.250 | 0.34x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 330.769 | 335.697 | 339.096 | 380.062 | 1.00x |
| users.ndjson | orjson | 526.097 | 530.736 | 533.700 | 380.062 | 0.63x |
| users.ndjson | msgspec | 531.376 | 536.262 | 565.939 | 380.062 | 0.63x |
| users.ndjson | ujson | 685.425 | 690.104 | 697.623 | 380.062 | 0.49x |
| users.ndjson | json | 786.510 | 792.219 | 809.855 | 380.062 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 77.099 | 79.026 | 81.247 | 2070.797 | 1.00x |
| users.json | orjson | 93.613 | 95.340 | 98.113 | 2070.797 | 0.83x |
| users.json | msgspec | 119.676 | 126.269 | 131.958 | 2070.797 | 0.63x |
| users.json | ujson | 441.218 | 450.335 | 468.932 | 2070.797 | 0.18x |
| users.json | json | 602.647 | 606.367 | 615.818 | 2070.797 | 0.13x |
| flat.json | strata | 2.077 | 2.130 | 4.283 | 355.641 | 1.00x |
| flat.json | orjson | 2.536 | 2.612 | 3.459 | 355.641 | 0.82x |
| flat.json | msgspec | 3.240 | 3.288 | 3.505 | 355.641 | 0.65x |
| flat.json | ujson | 9.340 | 9.499 | 9.592 | 355.641 | 0.22x |
| flat.json | json | 12.661 | 12.911 | 13.428 | 355.641 | 0.17x |
| nested.json | strata | 1.553 | 1.583 | 1.984 | 357.406 | 1.00x |
| nested.json | orjson | 2.344 | 2.422 | 2.723 | 357.406 | 0.65x |
| nested.json | msgspec | 2.971 | 3.037 | 3.585 | 357.406 | 0.52x |
| nested.json | ujson | 9.962 | 10.038 | 10.397 | 357.406 | 0.16x |
| nested.json | json | 15.313 | 15.633 | 16.220 | 357.406 | 0.10x |
| wide_arrays.json | strata | 14.513 | 14.870 | 15.209 | 768.719 | 1.00x |
| wide_arrays.json | orjson | 13.733 | 13.954 | 14.627 | 768.719 | 1.07x |
| wide_arrays.json | msgspec | 22.204 | 22.598 | 22.955 | 768.719 | 0.66x |
| wide_arrays.json | ujson | 65.020 | 65.805 | 66.797 | 768.719 | 0.23x |
| wide_arrays.json | json | 111.028 | 111.824 | 113.563 | 768.719 | 0.13x |
| mixed.json | strata | 0.466 | 0.497 | 0.548 | 770.250 | 1.00x |
| mixed.json | orjson | 0.532 | 0.561 | 0.604 | 770.250 | 0.89x |
| mixed.json | msgspec | 0.580 | 0.618 | 1.010 | 770.250 | 0.80x |
| mixed.json | ujson | 2.224 | 2.258 | 2.380 | 770.250 | 0.22x |
| mixed.json | json | 3.286 | 3.394 | 3.611 | 770.250 | 0.15x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.672 | 0.692 | 0.712 | 1156.734 | 1.00x |
| users.json $[*].id | jmespath | 2.004 | 2.036 | 18.917 | 1156.734 | 0.34x |
| users.json $[*].id | jsonpath-ng | 71.111 | 71.855 | 72.126 | 1156.734 | 0.01x |
| users.json $[*].orders[*].total | strata | 13.068 | 13.361 | 13.812 | 817.750 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 34.746 | 35.218 | 40.343 | 817.750 | 0.38x |
| users.json $[*].orders[*].total | jsonpath-ng | 217.987 | 221.114 | 223.975 | 817.750 | 0.06x |
| users.json $..total | strata | 52.931 | 53.534 | 54.242 | 621.969 | 1.00x |
| users.json $..total | jsonpath-ng | 8594.606 | 8621.966 | 8734.149 | 621.969 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 130.237 | 134.640 | 134.940 | 824.969 | 1.00x |
| users.json $[*].id | orjson+jmespath | 453.529 | 455.542 | 458.610 | 824.969 | 0.30x |
| users.json $[*].id | orjson+jsonpath-ng | 459.800 | 461.856 | 465.992 | 824.969 | 0.29x |
| users.json $[*].orders[*].total | strata | 135.376 | 135.992 | 136.801 | 847.312 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 484.374 | 489.242 | 494.660 | 847.312 | 0.28x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 612.849 | 617.332 | 620.393 | 847.312 | 0.22x |
| users.json $..total | strata | 438.747 | 446.419 | 448.661 | 860.359 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 8959.445 | 8975.241 | 8985.120 | 860.359 | 0.05x |

