# Benchmark results - medium

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
| users.json | strata | 41.655 | 43.402 | 48.254 | 139.547 | 1.00x |
| users.json | orjson | 50.600 | 53.343 | 56.588 | 139.547 | 0.81x |
| users.json | msgspec | 52.835 | 54.384 | 58.916 | 139.547 | 0.80x |
| users.json | ujson | 71.798 | 75.756 | 80.060 | 139.547 | 0.57x |
| users.json | json | 89.407 | 93.124 | 96.852 | 139.547 | 0.47x |
| flat.json | strata | 4.090 | 4.237 | 4.366 | 162.094 | 1.00x |
| flat.json | orjson | 5.595 | 5.836 | 6.186 | 162.094 | 0.73x |
| flat.json | msgspec | 5.690 | 5.842 | 6.125 | 162.094 | 0.73x |
| flat.json | ujson | 7.722 | 7.905 | 8.308 | 162.094 | 0.54x |
| flat.json | json | 10.278 | 10.544 | 11.277 | 162.094 | 0.40x |
| nested.json | strata | 3.472 | 3.478 | 3.765 | 163.781 | 1.00x |
| nested.json | orjson | 4.760 | 4.926 | 5.301 | 163.781 | 0.71x |
| nested.json | msgspec | 4.825 | 4.928 | 5.384 | 163.781 | 0.71x |
| nested.json | ujson | 6.504 | 6.559 | 6.846 | 163.781 | 0.53x |
| nested.json | json | 9.437 | 9.690 | 10.425 | 163.781 | 0.36x |
| wide_arrays.json | strata | 19.440 | 20.411 | 21.800 | 191.141 | 1.00x |
| wide_arrays.json | orjson | 19.153 | 20.571 | 21.410 | 191.141 | 0.99x |
| wide_arrays.json | msgspec | 24.241 | 25.792 | 26.859 | 191.141 | 0.79x |
| wide_arrays.json | ujson | 31.165 | 32.894 | 34.544 | 191.141 | 0.62x |
| wide_arrays.json | json | 45.104 | 46.960 | 49.880 | 191.141 | 0.43x |
| mixed.json | strata | 0.828 | 0.844 | 0.996 | 172.047 | 1.00x |
| mixed.json | orjson | 1.891 | 1.962 | 2.135 | 172.047 | 0.43x |
| mixed.json | msgspec | 1.967 | 2.014 | 2.290 | 172.047 | 0.42x |
| mixed.json | ujson | 2.191 | 2.252 | 2.344 | 172.047 | 0.37x |
| mixed.json | json | 2.940 | 2.993 | 3.368 | 172.047 | 0.28x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 9.060 | 9.111 | 9.544 | 602.188 | 1.00x |
| users.json | orjson | 12.224 | 12.297 | 12.561 | 602.188 | 0.74x |
| users.json | msgspec | 15.831 | 15.974 | 16.090 | 602.188 | 0.57x |
| users.json | ujson | 63.818 | 64.071 | 64.856 | 602.188 | 0.14x |
| users.json | json | 87.225 | 87.644 | 88.610 | 602.188 | 0.10x |
| flat.json | strata | 1.125 | 1.138 | 1.183 | 162.156 | 1.00x |
| flat.json | orjson | 1.406 | 1.420 | 1.434 | 162.156 | 0.80x |
| flat.json | msgspec | 1.848 | 1.872 | 1.971 | 162.156 | 0.61x |
| flat.json | ujson | 5.765 | 5.845 | 6.019 | 162.156 | 0.19x |
| flat.json | json | 7.941 | 7.998 | 8.372 | 162.156 | 0.14x |
| nested.json | strata | 0.779 | 0.801 | 0.832 | 163.781 | 1.00x |
| nested.json | orjson | 1.145 | 1.280 | 1.296 | 163.781 | 0.63x |
| nested.json | msgspec | 1.527 | 1.703 | 1.808 | 163.781 | 0.47x |
| nested.json | ujson | 5.558 | 6.178 | 6.472 | 163.781 | 0.13x |
| nested.json | json | 8.969 | 10.024 | 10.291 | 163.781 | 0.08x |
| wide_arrays.json | strata | 6.696 | 7.334 | 8.136 | 187.016 | 1.00x |
| wide_arrays.json | orjson | 7.065 | 7.687 | 8.399 | 187.016 | 0.95x |
| wide_arrays.json | msgspec | 11.752 | 12.311 | 13.521 | 187.016 | 0.60x |
| wide_arrays.json | ujson | 35.202 | 36.290 | 39.148 | 187.016 | 0.20x |
| wide_arrays.json | json | 64.190 | 66.731 | 70.244 | 187.016 | 0.11x |
| mixed.json | strata | 0.189 | 0.211 | 0.232 | 172.062 | 1.00x |
| mixed.json | orjson | 0.224 | 0.244 | 0.262 | 172.062 | 0.86x |
| mixed.json | msgspec | 0.247 | 0.276 | 0.326 | 172.062 | 0.77x |
| mixed.json | ujson | 1.203 | 1.230 | 1.474 | 172.062 | 0.17x |
| mixed.json | json | 1.886 | 2.004 | 2.157 | 172.062 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 43.445 | 44.867 | 46.969 | 657.281 | 1.00x |
| users.json | orjson | 64.976 | 67.532 | 69.442 | 657.281 | 0.66x |
| users.json | msgspec | 66.905 | 69.595 | 72.780 | 657.281 | 0.64x |
| users.json | ujson | 87.888 | 89.904 | 92.206 | 657.281 | 0.50x |
| users.json | json | 104.940 | 107.222 | 108.922 | 657.281 | 0.42x |
| flat.json | strata | 4.346 | 4.386 | 4.563 | 163.781 | 1.00x |
| flat.json | orjson | 6.141 | 6.248 | 6.768 | 163.781 | 0.70x |
| flat.json | msgspec | 6.239 | 6.420 | 6.886 | 163.781 | 0.68x |
| flat.json | ujson | 8.586 | 8.739 | 9.198 | 163.781 | 0.50x |
| flat.json | json | 10.783 | 10.914 | 11.228 | 163.781 | 0.40x |
| nested.json | strata | 3.275 | 3.661 | 3.738 | 163.781 | 1.00x |
| nested.json | orjson | 4.918 | 5.264 | 5.649 | 163.781 | 0.70x |
| nested.json | msgspec | 4.914 | 5.248 | 6.156 | 163.781 | 0.70x |
| nested.json | ujson | 6.603 | 7.035 | 7.350 | 163.781 | 0.52x |
| nested.json | json | 9.062 | 9.745 | 10.336 | 163.781 | 0.38x |
| wide_arrays.json | strata | 20.128 | 21.430 | 22.038 | 165.000 | 1.00x |
| wide_arrays.json | orjson | 22.036 | 23.708 | 24.406 | 165.000 | 0.90x |
| wide_arrays.json | msgspec | 27.239 | 28.339 | 29.966 | 165.000 | 0.76x |
| wide_arrays.json | ujson | 34.447 | 36.653 | 37.904 | 165.000 | 0.58x |
| wide_arrays.json | json | 46.793 | 48.828 | 52.623 | 165.000 | 0.44x |
| mixed.json | strata | 0.821 | 0.895 | 0.936 | 172.062 | 1.00x |
| mixed.json | orjson | 1.900 | 2.024 | 2.229 | 172.062 | 0.44x |
| mixed.json | msgspec | 1.963 | 2.154 | 2.457 | 172.062 | 0.42x |
| mixed.json | ujson | 2.244 | 2.338 | 2.583 | 172.062 | 0.38x |
| mixed.json | json | 2.840 | 3.130 | 3.635 | 172.062 | 0.29x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 60.936 | 68.051 | 68.940 | 165.859 | 1.00x |
| users.ndjson | orjson | 92.061 | 100.802 | 103.545 | 165.859 | 0.68x |
| users.ndjson | msgspec | 90.171 | 100.009 | 100.978 | 165.859 | 0.68x |
| users.ndjson | ujson | 116.790 | 128.392 | 129.350 | 165.859 | 0.53x |
| users.ndjson | json | 150.030 | 167.504 | 180.885 | 165.859 | 0.41x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 10.908 | 12.156 | 12.825 | 673.328 | 1.00x |
| users.json | orjson | 14.987 | 15.465 | 16.376 | 673.328 | 0.79x |
| users.json | msgspec | 18.940 | 19.686 | 20.122 | 673.328 | 0.62x |
| users.json | ujson | 65.781 | 66.576 | 71.331 | 673.328 | 0.18x |
| users.json | json | 89.869 | 91.565 | 96.036 | 673.328 | 0.13x |
| flat.json | strata | 1.401 | 1.490 | 1.561 | 163.781 | 1.00x |
| flat.json | orjson | 1.708 | 1.749 | 1.869 | 163.781 | 0.85x |
| flat.json | msgspec | 2.165 | 2.256 | 2.440 | 163.781 | 0.66x |
| flat.json | ujson | 5.840 | 5.975 | 6.368 | 163.781 | 0.25x |
| flat.json | json | 8.268 | 8.496 | 8.631 | 163.781 | 0.18x |
| nested.json | strata | 0.961 | 1.133 | 1.204 | 163.891 | 1.00x |
| nested.json | orjson | 1.466 | 1.591 | 1.793 | 163.891 | 0.71x |
| nested.json | msgspec | 1.811 | 2.077 | 2.187 | 163.891 | 0.55x |
| nested.json | ujson | 6.450 | 6.560 | 6.744 | 163.891 | 0.17x |
| nested.json | json | 9.156 | 10.289 | 10.606 | 163.891 | 0.11x |
| wide_arrays.json | strata | 7.937 | 8.937 | 9.715 | 172.047 | 1.00x |
| wide_arrays.json | orjson | 8.351 | 9.296 | 9.964 | 172.047 | 0.96x |
| wide_arrays.json | msgspec | 13.247 | 14.280 | 15.142 | 172.047 | 0.63x |
| wide_arrays.json | ujson | 36.663 | 39.510 | 41.628 | 172.047 | 0.23x |
| wide_arrays.json | json | 67.033 | 70.837 | 73.907 | 172.047 | 0.13x |
| mixed.json | strata | 0.383 | 0.410 | 0.750 | 172.062 | 1.00x |
| mixed.json | orjson | 0.389 | 0.458 | 0.561 | 172.062 | 0.90x |
| mixed.json | msgspec | 0.423 | 0.468 | 0.568 | 172.062 | 0.88x |
| mixed.json | ujson | 1.547 | 1.574 | 1.754 | 172.062 | 0.26x |
| mixed.json | json | 2.053 | 2.312 | 2.443 | 172.062 | 0.18x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.192 | 0.225 | 0.265 | 673.766 | 1.00x |
| users.json $[*].id | jmespath | 0.898 | 0.942 | 1.023 | 673.766 | 0.24x |
| users.json $[*].id | jsonpath-ng | 14.291 | 14.775 | 15.238 | 673.766 | 0.02x |
| users.json $[*].orders[*].total | strata | 2.878 | 3.288 | 3.726 | 674.047 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 10.942 | 13.019 | 17.072 | 674.047 | 0.25x |
| users.json $[*].orders[*].total | jsonpath-ng | 63.139 | 72.249 | 91.724 | 674.047 | 0.05x |
| users.json $..total | strata | 10.588 | 11.635 | 14.169 | 147.328 | 1.00x |
| users.json $..total | jsonpath-ng | 1810.678 | 1929.476 | 2476.376 | 147.328 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 21.164 | 34.372 | 57.497 | 683.828 | 1.00x |
| users.json $[*].id | orjson+jmespath | 78.825 | 105.510 | 359.369 | 683.828 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 82.354 | 118.930 | 222.810 | 683.828 | 0.29x |
| users.json $[*].orders[*].total | strata | 27.852 | 35.060 | 40.859 | 485.000 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 97.383 | 116.107 | 146.669 | 485.000 | 0.30x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 140.717 | 182.388 | 233.455 | 485.000 | 0.19x |
| users.json $..total | strata | 78.038 | 83.831 | 96.194 | 203.094 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 1705.772 | 1867.685 | 2356.688 | 203.094 | 0.04x |

