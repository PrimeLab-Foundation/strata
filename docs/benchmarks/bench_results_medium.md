# Benchmark results - medium

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: d61f035
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
| users.json | strata | 39.990 | 43.781 | 48.943 | 139.641 | 1.00x |
| users.json | orjson | 51.676 | 55.345 | 58.338 | 139.641 | 0.79x |
| users.json | msgspec | 53.123 | 56.433 | 59.985 | 139.641 | 0.78x |
| users.json | ujson | 75.452 | 78.347 | 81.861 | 139.641 | 0.56x |
| users.json | json | 105.582 | 108.850 | 115.003 | 139.641 | 0.40x |
| flat.json | strata | 2.442 | 2.544 | 2.715 | 160.797 | 1.00x |
| flat.json | orjson | 2.788 | 2.925 | 3.133 | 160.797 | 0.87x |
| flat.json | msgspec | 2.878 | 3.076 | 3.374 | 160.797 | 0.83x |
| flat.json | ujson | 4.226 | 4.578 | 4.703 | 160.797 | 0.56x |
| flat.json | json | 5.964 | 6.221 | 6.395 | 160.797 | 0.41x |
| nested.json | strata | 2.197 | 2.354 | 2.470 | 163.359 | 1.00x |
| nested.json | orjson | 2.414 | 2.518 | 3.083 | 163.359 | 0.93x |
| nested.json | msgspec | 2.430 | 2.632 | 2.863 | 163.359 | 0.89x |
| nested.json | ujson | 3.535 | 3.914 | 4.188 | 163.359 | 0.60x |
| nested.json | json | 5.760 | 6.108 | 6.527 | 163.359 | 0.39x |
| wide_arrays.json | strata | 12.390 | 12.602 | 12.886 | 190.688 | 1.00x |
| wide_arrays.json | orjson | 13.725 | 14.184 | 14.445 | 190.688 | 0.89x |
| wide_arrays.json | msgspec | 16.847 | 17.334 | 17.493 | 190.688 | 0.73x |
| wide_arrays.json | ujson | 21.600 | 22.046 | 22.798 | 190.688 | 0.57x |
| wide_arrays.json | json | 31.723 | 31.960 | 32.278 | 190.688 | 0.39x |
| mixed.json | strata | 0.502 | 0.514 | 0.551 | 189.828 | 1.00x |
| mixed.json | orjson | 0.595 | 0.629 | 0.775 | 189.828 | 0.82x |
| mixed.json | msgspec | 0.643 | 0.664 | 0.729 | 189.828 | 0.77x |
| mixed.json | ujson | 0.802 | 0.820 | 1.014 | 189.828 | 0.63x |
| mixed.json | json | 1.347 | 1.423 | 1.598 | 189.828 | 0.36x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.533 | 8.991 | 9.548 | 604.125 | 1.00x |
| users.json | orjson | 12.761 | 12.977 | 13.319 | 604.125 | 0.69x |
| users.json | msgspec | 16.573 | 16.941 | 17.484 | 604.125 | 0.53x |
| users.json | ujson | 65.218 | 65.788 | 66.744 | 604.125 | 0.14x |
| users.json | json | 93.066 | 93.803 | 95.359 | 604.125 | 0.10x |
| flat.json | strata | 0.701 | 0.706 | 0.803 | 160.891 | 1.00x |
| flat.json | orjson | 0.871 | 0.914 | 1.046 | 160.891 | 0.77x |
| flat.json | msgspec | 1.144 | 1.221 | 1.285 | 160.891 | 0.58x |
| flat.json | ujson | 3.511 | 3.583 | 3.706 | 160.891 | 0.20x |
| flat.json | json | 5.494 | 5.994 | 6.243 | 160.891 | 0.12x |
| nested.json | strata | 0.533 | 0.574 | 0.641 | 163.438 | 1.00x |
| nested.json | orjson | 0.795 | 0.823 | 0.968 | 163.438 | 0.70x |
| nested.json | msgspec | 1.070 | 1.105 | 1.235 | 163.438 | 0.52x |
| nested.json | ujson | 3.909 | 4.052 | 4.203 | 163.438 | 0.14x |
| nested.json | json | 6.702 | 7.187 | 7.410 | 163.438 | 0.08x |
| wide_arrays.json | strata | 4.260 | 4.447 | 4.824 | 181.609 | 1.00x |
| wide_arrays.json | orjson | 5.166 | 5.302 | 5.494 | 181.609 | 0.84x |
| wide_arrays.json | msgspec | 8.691 | 8.849 | 9.001 | 181.609 | 0.50x |
| wide_arrays.json | ujson | 25.795 | 26.206 | 26.415 | 181.609 | 0.17x |
| wide_arrays.json | json | 46.146 | 47.013 | 48.216 | 181.609 | 0.09x |
| mixed.json | strata | 0.132 | 0.137 | 0.167 | 189.828 | 1.00x |
| mixed.json | orjson | 0.154 | 0.162 | 0.175 | 189.828 | 0.85x |
| mixed.json | msgspec | 0.176 | 0.199 | 0.254 | 189.828 | 0.69x |
| mixed.json | ujson | 0.847 | 0.872 | 0.914 | 189.828 | 0.16x |
| mixed.json | json | 1.421 | 1.572 | 1.597 | 189.828 | 0.09x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 41.981 | 43.577 | 46.801 | 657.109 | 1.00x |
| users.json | orjson | 53.736 | 56.635 | 61.526 | 657.109 | 0.77x |
| users.json | msgspec | 55.821 | 57.882 | 65.222 | 657.109 | 0.75x |
| users.json | ujson | 77.325 | 82.781 | 86.630 | 657.109 | 0.53x |
| users.json | json | 106.802 | 111.337 | 117.643 | 657.109 | 0.39x |
| flat.json | strata | 2.574 | 2.664 | 2.793 | 162.547 | 1.00x |
| flat.json | orjson | 2.937 | 3.161 | 3.366 | 162.547 | 0.84x |
| flat.json | msgspec | 3.097 | 3.214 | 3.395 | 162.547 | 0.83x |
| flat.json | ujson | 4.494 | 4.763 | 4.970 | 162.547 | 0.56x |
| flat.json | json | 6.023 | 6.253 | 6.502 | 162.547 | 0.43x |
| nested.json | strata | 2.262 | 2.364 | 2.495 | 163.438 | 1.00x |
| nested.json | orjson | 2.505 | 2.637 | 2.976 | 163.438 | 0.90x |
| nested.json | msgspec | 2.541 | 2.589 | 2.772 | 163.438 | 0.91x |
| nested.json | ujson | 3.691 | 3.912 | 4.346 | 163.438 | 0.60x |
| nested.json | json | 5.637 | 5.875 | 6.079 | 163.438 | 0.40x |
| wide_arrays.json | strata | 12.910 | 13.106 | 13.354 | 186.578 | 1.00x |
| wide_arrays.json | orjson | 14.321 | 14.634 | 15.612 | 186.578 | 0.90x |
| wide_arrays.json | msgspec | 17.686 | 17.991 | 18.966 | 186.578 | 0.73x |
| wide_arrays.json | ujson | 23.109 | 23.380 | 23.780 | 186.578 | 0.56x |
| wide_arrays.json | json | 32.300 | 32.507 | 33.088 | 186.578 | 0.40x |
| mixed.json | strata | 0.580 | 0.634 | 0.709 | 189.828 | 1.00x |
| mixed.json | orjson | 0.628 | 0.710 | 0.822 | 189.828 | 0.89x |
| mixed.json | msgspec | 0.727 | 0.809 | 0.886 | 189.828 | 0.78x |
| mixed.json | ujson | 0.911 | 0.976 | 1.159 | 189.828 | 0.65x |
| mixed.json | json | 1.455 | 1.531 | 1.630 | 189.828 | 0.41x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 41.268 | 41.600 | 42.437 | 165.031 | 1.00x |
| users.ndjson | orjson | 64.214 | 64.922 | 65.302 | 165.031 | 0.64x |
| users.ndjson | msgspec | 64.750 | 65.439 | 66.396 | 165.031 | 0.64x |
| users.ndjson | ujson | 83.276 | 84.771 | 85.518 | 165.031 | 0.49x |
| users.ndjson | json | 106.991 | 108.867 | 109.321 | 165.031 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.535 | 11.147 | 11.946 | 675.469 | 1.00x |
| users.json | orjson | 14.687 | 15.163 | 16.164 | 675.469 | 0.74x |
| users.json | msgspec | 19.105 | 19.376 | 20.240 | 675.469 | 0.58x |
| users.json | ujson | 66.911 | 68.419 | 69.928 | 675.469 | 0.16x |
| users.json | json | 94.721 | 96.560 | 98.024 | 675.469 | 0.12x |
| flat.json | strata | 0.961 | 1.055 | 1.348 | 163.328 | 1.00x |
| flat.json | orjson | 1.167 | 1.240 | 1.351 | 163.328 | 0.85x |
| flat.json | msgspec | 1.474 | 1.589 | 1.694 | 163.328 | 0.66x |
| flat.json | ujson | 3.786 | 3.939 | 4.162 | 163.328 | 0.27x |
| flat.json | json | 5.686 | 6.205 | 6.348 | 163.328 | 0.17x |
| nested.json | strata | 0.790 | 0.864 | 0.945 | 163.438 | 1.00x |
| nested.json | orjson | 1.059 | 1.178 | 1.245 | 163.438 | 0.73x |
| nested.json | msgspec | 1.272 | 1.396 | 1.517 | 163.438 | 0.62x |
| nested.json | ujson | 4.269 | 4.477 | 4.642 | 163.438 | 0.19x |
| nested.json | json | 6.823 | 7.289 | 7.696 | 163.438 | 0.12x |
| wide_arrays.json | strata | 5.422 | 5.571 | 6.067 | 189.828 | 1.00x |
| wide_arrays.json | orjson | 6.150 | 6.375 | 7.038 | 189.828 | 0.87x |
| wide_arrays.json | msgspec | 9.810 | 10.035 | 10.352 | 189.828 | 0.56x |
| wide_arrays.json | ujson | 27.100 | 27.369 | 28.591 | 189.828 | 0.20x |
| wide_arrays.json | json | 47.105 | 47.590 | 48.339 | 189.828 | 0.12x |
| mixed.json | strata | 0.295 | 0.336 | 0.418 | 189.828 | 1.00x |
| mixed.json | orjson | 0.310 | 0.348 | 0.466 | 189.828 | 0.96x |
| mixed.json | msgspec | 0.323 | 0.359 | 0.461 | 189.828 | 0.94x |
| mixed.json | ujson | 1.020 | 1.062 | 1.165 | 189.828 | 0.32x |
| mixed.json | json | 1.522 | 1.703 | 1.839 | 189.828 | 0.20x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.180 | 0.201 | 0.271 | 675.844 | 1.00x |
| users.json $[*].id | jmespath | 0.700 | 0.748 | 0.824 | 675.844 | 0.27x |
| users.json $[*].id | jsonpath-ng | 3.033 | 3.081 | 3.490 | 675.844 | 0.07x |
| users.json $[*].orders[*].total | strata | 2.667 | 3.071 | 5.057 | 623.547 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 6.949 | 7.195 | 8.794 | 623.547 | 0.43x |
| users.json $[*].orders[*].total | jsonpath-ng | 37.024 | 37.808 | 40.251 | 623.547 | 0.08x |
| users.json $..total | strata | 8.114 | 8.471 | 8.654 | 231.391 | 1.00x |
| users.json $..total | jsonpath-ng | 1131.598 | 1135.204 | 1140.615 | 231.391 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 19.696 | 20.206 | 21.282 | 708.938 | 1.00x |
| users.json $[*].id | orjson+jmespath | 54.122 | 56.519 | 61.153 | 708.938 | 0.36x |
| users.json $[*].id | orjson+jsonpath-ng | 56.462 | 58.626 | 60.806 | 708.938 | 0.34x |
| users.json $[*].orders[*].total | strata | 19.921 | 19.942 | 20.552 | 690.844 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 58.454 | 60.639 | 62.491 | 690.844 | 0.33x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 100.800 | 103.242 | 107.744 | 690.844 | 0.19x |
| users.json $..total | strata | 49.531 | 49.939 | 52.003 | 202.422 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1188.493 | 1196.966 | 1202.478 | 202.422 | 0.04x |

