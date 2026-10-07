# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: b32d39824b8fea15839872cab8834a1b0fef1294
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: AMD EPYC 9V74 80-Core Processor
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.935 | 8.752 | 11.390 | 65.453 | 1.00x |
| users.json | orjson | 10.884 | 11.430 | 13.846 | 65.453 | 0.77x |
| users.json | msgspec | 10.655 | 11.087 | 13.539 | 65.453 | 0.79x |
| users.json | ujson | 14.056 | 15.155 | 20.560 | 65.453 | 0.58x |
| users.json | pysimdjson | 14.817 | 17.426 | 22.244 | 65.453 | 0.50x |
| users.json | json | 16.700 | 17.034 | 17.823 | 65.453 | 0.51x |
| flat.json | strata | 0.674 | 0.684 | 0.692 | 65.180 | 1.00x |
| flat.json | orjson | 0.813 | 0.822 | 0.844 | 65.180 | 0.83x |
| flat.json | msgspec | 0.786 | 0.797 | 0.846 | 65.180 | 0.86x |
| flat.json | ujson | 1.168 | 1.191 | 1.209 | 65.180 | 0.57x |
| flat.json | pysimdjson | 1.241 | 1.267 | 1.290 | 65.180 | 0.54x |
| flat.json | json | 1.325 | 1.343 | 1.355 | 65.180 | 0.51x |
| nested.json | strata | 0.619 | 0.637 | 0.649 | 65.180 | 1.00x |
| nested.json | orjson | 0.784 | 0.794 | 0.807 | 65.180 | 0.80x |
| nested.json | msgspec | 0.726 | 0.744 | 0.775 | 65.180 | 0.86x |
| nested.json | ujson | 1.106 | 1.124 | 1.176 | 65.180 | 0.57x |
| nested.json | pysimdjson | 1.088 | 1.107 | 1.133 | 65.180 | 0.58x |
| nested.json | json | 1.405 | 1.422 | 1.443 | 65.180 | 0.45x |
| wide_arrays.json | strata | 3.441 | 3.511 | 3.546 | 77.746 | 1.00x |
| wide_arrays.json | orjson | 4.250 | 4.408 | 4.755 | 77.746 | 0.80x |
| wide_arrays.json | msgspec | 4.731 | 4.797 | 5.029 | 77.746 | 0.73x |
| wide_arrays.json | ujson | 5.806 | 5.949 | 6.185 | 77.746 | 0.59x |
| wide_arrays.json | pysimdjson | 4.942 | 5.097 | 5.384 | 77.746 | 0.69x |
| wide_arrays.json | json | 7.669 | 7.762 | 8.057 | 77.746 | 0.45x |
| mixed.json | strata | 0.148 | 0.153 | 0.154 | 77.809 | 1.00x |
| mixed.json | orjson | 0.182 | 0.188 | 0.195 | 77.809 | 0.81x |
| mixed.json | msgspec | 0.184 | 0.190 | 0.208 | 77.809 | 0.80x |
| mixed.json | ujson | 0.236 | 0.240 | 0.254 | 77.809 | 0.64x |
| mixed.json | pysimdjson | 0.235 | 0.245 | 0.295 | 77.809 | 0.62x |
| mixed.json | json | 0.350 | 0.358 | 0.364 | 77.809 | 0.43x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.790 | 1.810 | 1.951 | 47.363 | 1.00x |
| users.json | orjson | 1.970 | 1.991 | 2.027 | 47.363 | 0.91x |
| users.json | msgspec | 3.229 | 3.254 | 3.338 | 47.363 | 0.56x |
| users.json | ujson | 8.861 | 8.993 | 9.162 | 47.363 | 0.20x |
| users.json | json | 16.596 | 16.940 | 17.266 | 47.363 | 0.11x |
| flat.json | strata | 0.238 | 0.243 | 0.261 | 65.180 | 1.00x |
| flat.json | orjson | 0.241 | 0.248 | 0.263 | 65.180 | 0.98x |
| flat.json | msgspec | 0.367 | 0.380 | 0.385 | 65.180 | 0.64x |
| flat.json | ujson | 0.813 | 0.822 | 0.889 | 65.180 | 0.30x |
| flat.json | json | 1.445 | 1.451 | 1.482 | 65.180 | 0.17x |
| nested.json | strata | 0.176 | 0.180 | 0.206 | 65.184 | 1.00x |
| nested.json | orjson | 0.219 | 0.222 | 0.242 | 65.184 | 0.81x |
| nested.json | msgspec | 0.327 | 0.338 | 0.348 | 65.184 | 0.53x |
| nested.json | ujson | 0.820 | 0.835 | 0.853 | 65.184 | 0.22x |
| nested.json | json | 1.788 | 1.810 | 1.851 | 65.184 | 0.10x |
| wide_arrays.json | strata | 1.365 | 1.390 | 1.420 | 77.746 | 1.00x |
| wide_arrays.json | orjson | 1.475 | 1.504 | 1.536 | 77.746 | 0.92x |
| wide_arrays.json | msgspec | 2.363 | 2.384 | 2.425 | 77.746 | 0.58x |
| wide_arrays.json | ujson | 4.983 | 5.011 | 5.180 | 77.746 | 0.28x |
| wide_arrays.json | json | 13.033 | 13.141 | 13.480 | 77.746 | 0.11x |
| mixed.json | strata | 0.048 | 0.050 | 0.053 | 77.809 | 1.00x |
| mixed.json | orjson | 0.047 | 0.048 | 0.050 | 77.809 | 1.05x |
| mixed.json | msgspec | 0.066 | 0.068 | 0.069 | 77.809 | 0.73x |
| mixed.json | ujson | 0.177 | 0.179 | 0.184 | 77.809 | 0.28x |
| mixed.json | json | 0.409 | 0.413 | 0.417 | 77.809 | 0.12x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.268 | 9.370 | 10.871 | 66.785 | 1.00x |
| users.json | orjson | 11.299 | 12.062 | 13.692 | 66.785 | 0.78x |
| users.json | msgspec | 11.267 | 11.896 | 12.675 | 66.785 | 0.79x |
| users.json | ujson | 15.921 | 17.358 | 18.923 | 66.785 | 0.54x |
| users.json | json | 16.938 | 17.448 | 17.951 | 66.785 | 0.54x |
| flat.json | strata | 0.703 | 0.715 | 0.764 | 65.180 | 1.00x |
| flat.json | orjson | 0.862 | 0.876 | 0.899 | 65.180 | 0.82x |
| flat.json | msgspec | 0.824 | 0.838 | 0.858 | 65.180 | 0.85x |
| flat.json | ujson | 1.246 | 1.276 | 1.330 | 65.180 | 0.56x |
| flat.json | json | 1.364 | 1.383 | 1.397 | 65.180 | 0.52x |
| nested.json | strata | 0.658 | 0.671 | 0.681 | 65.184 | 1.00x |
| nested.json | orjson | 0.821 | 0.848 | 0.866 | 65.184 | 0.79x |
| nested.json | msgspec | 0.782 | 0.797 | 0.820 | 65.184 | 0.84x |
| nested.json | ujson | 1.141 | 1.181 | 1.251 | 65.184 | 0.57x |
| nested.json | json | 1.464 | 1.488 | 1.707 | 65.184 | 0.45x |
| wide_arrays.json | strata | 3.503 | 3.527 | 3.593 | 77.809 | 1.00x |
| wide_arrays.json | orjson | 4.363 | 4.424 | 4.496 | 77.809 | 0.80x |
| wide_arrays.json | msgspec | 4.793 | 4.879 | 4.956 | 77.809 | 0.72x |
| wide_arrays.json | ujson | 5.972 | 6.072 | 6.258 | 77.809 | 0.58x |
| wide_arrays.json | json | 7.632 | 7.648 | 8.232 | 77.809 | 0.46x |
| mixed.json | strata | 0.164 | 0.171 | 0.184 | 77.809 | 1.00x |
| mixed.json | orjson | 0.220 | 0.222 | 0.239 | 77.809 | 0.77x |
| mixed.json | msgspec | 0.221 | 0.225 | 0.244 | 77.809 | 0.76x |
| mixed.json | ujson | 0.284 | 0.289 | 0.299 | 77.809 | 0.59x |
| mixed.json | json | 0.383 | 0.389 | 0.405 | 77.809 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.301 | 8.446 | 8.852 | 65.180 | 1.00x |
| users.ndjson | orjson | 13.859 | 14.191 | 16.739 | 65.180 | 0.60x |
| users.ndjson | msgspec | 13.649 | 13.916 | 14.480 | 65.180 | 0.61x |
| users.ndjson | ujson | 17.926 | 18.122 | 18.484 | 65.180 | 0.47x |
| users.ndjson | json | 22.027 | 22.546 | 22.897 | 65.180 | 0.37x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.261 | 2.376 | 2.431 | 66.785 | 1.00x |
| users.json | orjson | 2.451 | 2.550 | 2.749 | 66.785 | 0.93x |
| users.json | msgspec | 3.673 | 3.774 | 3.885 | 66.785 | 0.63x |
| users.json | ujson | 9.339 | 9.467 | 9.946 | 66.785 | 0.25x |
| users.json | json | 17.292 | 17.576 | 20.485 | 66.785 | 0.14x |
| flat.json | strata | 0.356 | 0.367 | 0.399 | 65.180 | 1.00x |
| flat.json | orjson | 0.374 | 0.385 | 0.405 | 65.180 | 0.95x |
| flat.json | msgspec | 0.495 | 0.511 | 0.530 | 65.180 | 0.72x |
| flat.json | ujson | 0.953 | 0.970 | 0.982 | 65.180 | 0.38x |
| flat.json | json | 1.603 | 1.626 | 1.654 | 65.180 | 0.23x |
| nested.json | strata | 0.274 | 0.290 | 67.495 | 65.184 | 1.00x |
| nested.json | orjson | 0.333 | 0.347 | 0.408 | 65.184 | 0.83x |
| nested.json | msgspec | 0.439 | 0.453 | 59.670 | 65.184 | 0.64x |
| nested.json | ujson | 0.952 | 0.968 | 1.016 | 65.184 | 0.30x |
| nested.json | json | 1.927 | 1.974 | 79.927 | 65.184 | 0.15x |
| wide_arrays.json | strata | 1.678 | 1.740 | 2.345 | 77.809 | 1.00x |
| wide_arrays.json | orjson | 1.810 | 1.871 | 2.045 | 77.809 | 0.93x |
| wide_arrays.json | msgspec | 2.726 | 2.763 | 2.809 | 77.809 | 0.63x |
| wide_arrays.json | ujson | 5.402 | 5.442 | 8.293 | 77.809 | 0.32x |
| wide_arrays.json | json | 13.473 | 13.600 | 16.200 | 77.809 | 0.13x |
| mixed.json | strata | 0.111 | 0.116 | 0.125 | 77.809 | 1.00x |
| mixed.json | orjson | 0.121 | 0.125 | 0.140 | 77.809 | 0.93x |
| mixed.json | msgspec | 0.139 | 0.143 | 0.151 | 77.809 | 0.81x |
| mixed.json | ujson | 0.262 | 0.265 | 0.283 | 77.809 | 0.44x |
| mixed.json | json | 0.480 | 0.505 | 0.512 | 77.809 | 0.23x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.055 | 0.057 | 0.069 | 66.785 | 1.00x |
| users.json $[*].id | jmespath | 0.355 | 0.365 | 0.380 | 66.785 | 0.16x |
| users.json $[*].id | jsonpath-ng | 2.108 | 2.146 | 2.330 | 66.785 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.367 | 0.380 | 0.466 | 66.797 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.305 | 2.381 | 2.485 | 66.797 | 0.16x |
| users.json $[*].orders[*].total | jsonpath-ng | 15.611 | 16.018 | 16.761 | 66.797 | 0.02x |
| users.json $..total | strata | 1.362 | 1.400 | 1.431 | 66.812 | 1.00x |
| users.json $..total | jsonpath-ng | 297.952 | 301.125 | 304.604 | 66.812 | 0.00x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.611 | 2.645 | 2.660 | 66.797 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.326 | 13.147 | 13.448 | 66.797 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 13.830 | 14.802 | 16.022 | 66.797 | 0.18x |
| users.json $[*].orders[*].total | strata | 2.811 | 2.863 | 2.898 | 66.812 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 15.587 | 15.973 | 16.573 | 66.812 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 31.319 | 32.147 | 35.766 | 66.812 | 0.09x |
| users.json $..total | strata | 10.730 | 13.175 | 14.996 | 66.816 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 321.446 | 324.711 | 336.019 | 66.816 | 0.04x |

