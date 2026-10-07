# Benchmark results - ci-linux-x86_64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: ec0411225b6d58a1df905844c946a766d3c39f0a
- python: 3.12.14
- implementation: CPython
- platform: Linux-6.17.0-1022-azure-x86_64-with-glibc2.39
- machine: x86_64
- processor: INTEL(R) XEON(R) PLATINUM 8573C
- compiler_flags: -fno-strict-overflow -Wsign-compare -DNDEBUG -g -O3 -Wall -fPIC -std=c++20 -D_LIBCPP_DISABLE_AVAILABILITY -fomit-frame-pointer -march=native -flto=thin -fprofile-use=/home/runner/work/strata/strata/build/pgo/strata.profdata -fsplit-machine-functions -shared -Wl,--rpath=/opt/hostedtoolcache/Python/3.12.14/x64/lib (19 recorded commands; see the JSON companion)
- repeats: 10
- warmup: 2
- provenance_schema: 1

## loads -- loads (in-memory parsing)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.632 | 8.726 | 10.719 | 66.574 | 1.00x |
| users.json | orjson | 12.275 | 12.402 | 14.239 | 66.574 | 0.70x |
| users.json | msgspec | 12.578 | 12.721 | 14.206 | 66.574 | 0.69x |
| users.json | ujson | 16.045 | 16.358 | 18.955 | 66.574 | 0.53x |
| users.json | pysimdjson | 16.178 | 16.349 | 18.093 | 66.574 | 0.53x |
| users.json | json | 19.114 | 19.321 | 20.079 | 66.574 | 0.45x |
| flat.json | strata | 0.725 | 0.753 | 0.760 | 80.242 | 1.00x |
| flat.json | orjson | 0.893 | 0.905 | 0.945 | 80.242 | 0.83x |
| flat.json | msgspec | 0.860 | 0.903 | 0.912 | 80.242 | 0.83x |
| flat.json | ujson | 1.227 | 1.248 | 1.269 | 80.242 | 0.60x |
| flat.json | pysimdjson | 1.384 | 1.395 | 1.442 | 80.242 | 0.54x |
| flat.json | json | 1.560 | 1.570 | 1.580 | 80.242 | 0.48x |
| nested.json | strata | 0.628 | 0.643 | 0.649 | 80.242 | 1.00x |
| nested.json | orjson | 0.775 | 0.790 | 0.804 | 80.242 | 0.81x |
| nested.json | msgspec | 0.800 | 0.815 | 0.837 | 80.242 | 0.79x |
| nested.json | ujson | 1.143 | 1.155 | 1.170 | 80.242 | 0.56x |
| nested.json | pysimdjson | 1.115 | 1.122 | 1.152 | 80.242 | 0.57x |
| nested.json | json | 1.606 | 1.616 | 1.636 | 80.242 | 0.40x |
| wide_arrays.json | strata | 3.365 | 3.393 | 3.503 | 84.246 | 1.00x |
| wide_arrays.json | orjson | 4.465 | 4.721 | 4.905 | 84.246 | 0.72x |
| wide_arrays.json | msgspec | 4.902 | 4.928 | 4.954 | 84.246 | 0.69x |
| wide_arrays.json | ujson | 6.002 | 6.051 | 6.109 | 84.246 | 0.56x |
| wide_arrays.json | pysimdjson | 4.910 | 4.936 | 4.964 | 84.246 | 0.69x |
| wide_arrays.json | json | 8.143 | 8.164 | 8.285 | 84.246 | 0.42x |
| mixed.json | strata | 0.168 | 0.171 | 0.184 | 84.309 | 1.00x |
| mixed.json | orjson | 0.200 | 0.204 | 0.219 | 84.309 | 0.84x |
| mixed.json | msgspec | 0.212 | 0.214 | 0.230 | 84.309 | 0.80x |
| mixed.json | ujson | 0.266 | 0.268 | 0.283 | 84.309 | 0.64x |
| mixed.json | pysimdjson | 0.266 | 0.269 | 0.288 | 84.309 | 0.64x |
| mixed.json | json | 0.405 | 0.416 | 0.473 | 84.309 | 0.41x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.688 | 1.701 | 1.732 | 64.980 | 1.00x |
| users.json | orjson | 1.913 | 1.956 | 2.078 | 64.980 | 0.87x |
| users.json | msgspec | 3.273 | 3.311 | 3.391 | 64.980 | 0.51x |
| users.json | ujson | 9.820 | 9.878 | 9.924 | 64.980 | 0.17x |
| users.json | json | 17.846 | 17.916 | 18.001 | 64.980 | 0.09x |
| flat.json | strata | 0.240 | 0.242 | 0.256 | 80.242 | 1.00x |
| flat.json | orjson | 0.241 | 0.245 | 0.261 | 80.242 | 0.98x |
| flat.json | msgspec | 0.370 | 0.380 | 0.407 | 80.242 | 0.63x |
| flat.json | ujson | 0.882 | 0.898 | 0.904 | 80.242 | 0.27x |
| flat.json | json | 1.540 | 1.569 | 1.622 | 80.242 | 0.15x |
| nested.json | strata | 0.157 | 0.163 | 0.174 | 80.242 | 1.00x |
| nested.json | orjson | 0.221 | 0.232 | 0.250 | 80.242 | 0.70x |
| nested.json | msgspec | 0.326 | 0.333 | 0.349 | 80.242 | 0.49x |
| nested.json | ujson | 0.911 | 0.922 | 0.938 | 80.242 | 0.18x |
| nested.json | json | 1.975 | 1.986 | 2.014 | 80.242 | 0.08x |
| wide_arrays.json | strata | 1.298 | 1.306 | 1.316 | 84.246 | 1.00x |
| wide_arrays.json | orjson | 1.349 | 1.406 | 1.411 | 84.246 | 0.93x |
| wide_arrays.json | msgspec | 2.278 | 2.303 | 2.328 | 84.246 | 0.57x |
| wide_arrays.json | ujson | 4.936 | 4.952 | 4.979 | 84.246 | 0.26x |
| wide_arrays.json | json | 13.046 | 13.127 | 13.462 | 84.246 | 0.10x |
| mixed.json | strata | 0.049 | 0.051 | 0.066 | 84.309 | 1.00x |
| mixed.json | orjson | 0.051 | 0.052 | 0.063 | 84.309 | 0.98x |
| mixed.json | msgspec | 0.069 | 0.072 | 0.082 | 84.309 | 0.70x |
| mixed.json | ujson | 0.204 | 0.207 | 0.222 | 84.309 | 0.25x |
| mixed.json | json | 0.438 | 0.448 | 0.472 | 84.309 | 0.11x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.713 | 8.821 | 9.589 | 81.094 | 1.00x |
| users.json | orjson | 12.432 | 12.596 | 12.972 | 81.094 | 0.70x |
| users.json | msgspec | 12.813 | 12.865 | 13.078 | 81.094 | 0.69x |
| users.json | ujson | 16.837 | 16.991 | 18.451 | 81.094 | 0.52x |
| users.json | json | 19.302 | 19.431 | 19.622 | 81.094 | 0.45x |
| flat.json | strata | 0.772 | 0.777 | 0.822 | 80.242 | 1.00x |
| flat.json | orjson | 0.951 | 0.964 | 1.030 | 80.242 | 0.81x |
| flat.json | msgspec | 0.929 | 0.947 | 1.229 | 80.242 | 0.82x |
| flat.json | ujson | 1.327 | 1.343 | 1.630 | 80.242 | 0.58x |
| flat.json | json | 1.613 | 1.632 | 1.949 | 80.242 | 0.48x |
| nested.json | strata | 0.653 | 0.669 | 0.730 | 80.242 | 1.00x |
| nested.json | orjson | 0.837 | 0.845 | 0.855 | 80.242 | 0.79x |
| nested.json | msgspec | 0.869 | 0.872 | 0.903 | 80.242 | 0.77x |
| nested.json | ujson | 1.217 | 1.233 | 1.249 | 80.242 | 0.54x |
| nested.json | json | 1.659 | 1.676 | 1.741 | 80.242 | 0.40x |
| wide_arrays.json | strata | 3.418 | 3.441 | 3.476 | 84.309 | 1.00x |
| wide_arrays.json | orjson | 4.440 | 4.794 | 4.895 | 84.309 | 0.72x |
| wide_arrays.json | msgspec | 5.000 | 5.034 | 5.111 | 84.309 | 0.68x |
| wide_arrays.json | ujson | 6.176 | 6.248 | 6.346 | 84.309 | 0.55x |
| wide_arrays.json | json | 8.270 | 8.331 | 8.433 | 84.309 | 0.41x |
| mixed.json | strata | 0.186 | 0.188 | 0.215 | 84.309 | 1.00x |
| mixed.json | orjson | 0.249 | 0.252 | 0.266 | 84.309 | 0.75x |
| mixed.json | msgspec | 0.259 | 0.265 | 0.283 | 84.309 | 0.71x |
| mixed.json | ujson | 0.323 | 0.328 | 0.345 | 84.309 | 0.57x |
| mixed.json | json | 0.456 | 0.464 | 0.467 | 84.309 | 0.40x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.959 | 9.065 | 9.202 | 80.242 | 1.00x |
| users.ndjson | orjson | 14.213 | 14.273 | 14.345 | 80.242 | 0.64x |
| users.ndjson | msgspec | 14.107 | 14.243 | 14.315 | 80.242 | 0.64x |
| users.ndjson | ujson | 18.052 | 18.234 | 19.103 | 80.242 | 0.50x |
| users.ndjson | json | 23.354 | 23.539 | 23.917 | 80.242 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 2.123 | 2.149 | 2.248 | 81.094 | 1.00x |
| users.json | orjson | 2.382 | 2.445 | 2.524 | 81.094 | 0.88x |
| users.json | msgspec | 3.756 | 3.792 | 3.832 | 81.094 | 0.57x |
| users.json | ujson | 10.378 | 10.425 | 10.475 | 81.094 | 0.21x |
| users.json | json | 18.496 | 18.537 | 18.699 | 81.094 | 0.12x |
| flat.json | strata | 0.334 | 0.346 | 0.360 | 80.242 | 1.00x |
| flat.json | orjson | 0.352 | 0.356 | 0.376 | 80.242 | 0.97x |
| flat.json | msgspec | 0.486 | 0.491 | 0.522 | 80.242 | 0.70x |
| flat.json | ujson | 1.020 | 1.025 | 1.045 | 80.242 | 0.34x |
| flat.json | json | 1.677 | 1.699 | 1.719 | 80.242 | 0.20x |
| nested.json | strata | 0.238 | 0.241 | 0.254 | 80.242 | 1.00x |
| nested.json | orjson | 0.313 | 0.322 | 0.342 | 80.242 | 0.75x |
| nested.json | msgspec | 0.419 | 0.425 | 0.438 | 80.242 | 0.57x |
| nested.json | ujson | 1.030 | 1.038 | 1.047 | 80.242 | 0.23x |
| nested.json | json | 2.061 | 2.074 | 2.215 | 80.242 | 0.12x |
| wide_arrays.json | strata | 1.602 | 1.622 | 1.636 | 84.309 | 1.00x |
| wide_arrays.json | orjson | 1.685 | 1.695 | 1.746 | 84.309 | 0.96x |
| wide_arrays.json | msgspec | 2.614 | 2.628 | 2.673 | 84.309 | 0.62x |
| wide_arrays.json | ujson | 5.318 | 5.345 | 7.013 | 84.309 | 0.30x |
| wide_arrays.json | json | 13.359 | 13.470 | 13.679 | 84.309 | 0.12x |
| mixed.json | strata | 0.106 | 0.109 | 0.125 | 84.309 | 1.00x |
| mixed.json | orjson | 0.120 | 0.123 | 0.147 | 84.309 | 0.88x |
| mixed.json | msgspec | 0.139 | 0.141 | 0.162 | 84.309 | 0.77x |
| mixed.json | ujson | 0.284 | 0.288 | 0.309 | 84.309 | 0.38x |
| mixed.json | json | 0.516 | 0.529 | 0.544 | 84.309 | 0.21x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.057 | 0.058 | 0.070 | 81.094 | 1.00x |
| users.json $[*].id | jmespath | 0.414 | 0.422 | 0.430 | 81.094 | 0.14x |
| users.json $[*].id | jsonpath-ng | 2.080 | 2.101 | 2.138 | 81.094 | 0.03x |
| users.json $[*].orders[*].total | strata | 0.429 | 0.435 | 0.446 | 81.109 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.567 | 2.576 | 2.636 | 81.109 | 0.17x |
| users.json $[*].orders[*].total | jsonpath-ng | 13.941 | 14.062 | 14.279 | 81.109 | 0.03x |
| users.json $..total | strata | 1.566 | 1.587 | 1.805 | 81.879 | 1.00x |
| users.json $..total | jsonpath-ng | 272.654 | 273.203 | 310.784 | 81.879 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 2.440 | 2.640 | 2.756 | 81.109 | 1.00x |
| users.json $[*].id | orjson+jmespath | 12.884 | 12.916 | 13.103 | 81.109 | 0.20x |
| users.json $[*].id | orjson+jsonpath-ng | 14.523 | 14.561 | 14.653 | 81.109 | 0.18x |
| users.json $[*].orders[*].total | strata | 2.602 | 2.766 | 2.895 | 81.879 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 14.956 | 15.060 | 15.105 | 81.879 | 0.18x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 31.313 | 31.652 | 31.762 | 81.879 | 0.09x |
| users.json $..total | strata | 11.240 | 11.593 | 11.996 | 81.879 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 286.808 | 288.652 | 292.655 | 81.879 | 0.04x |

