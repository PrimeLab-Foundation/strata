# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 68d6e74
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
| users.json | strata | 7.373 | 8.138 | 8.719 | 63.547 | 1.00x |
| users.json | orjson | 10.140 | 10.687 | 48.774 | 63.547 | 0.76x |
| users.json | msgspec | 8.801 | 10.330 | 18.047 | 63.547 | 0.79x |
| users.json | ujson | 12.117 | 14.194 | 15.901 | 63.547 | 0.57x |
| users.json | pysimdjson | 126.115 | 137.524 | 186.602 | 63.547 | 0.06x |
| users.json | json | 14.826 | 16.737 | 17.982 | 63.547 | 0.49x |
| flat.json | strata | 0.651 | 0.704 | 0.830 | 90.609 | 1.00x |
| flat.json | orjson | 0.707 | 0.806 | 0.912 | 90.609 | 0.87x |
| flat.json | msgspec | 0.693 | 0.768 | 0.902 | 90.609 | 0.92x |
| flat.json | ujson | 1.115 | 1.261 | 1.448 | 90.609 | 0.56x |
| flat.json | pysimdjson | 11.183 | 12.597 | 12.794 | 90.609 | 0.06x |
| flat.json | json | 1.318 | 1.509 | 1.697 | 90.609 | 0.47x |
| nested.json | strata | 0.622 | 0.648 | 1.155 | 90.641 | 1.00x |
| nested.json | orjson | 0.739 | 0.801 | 0.899 | 90.641 | 0.81x |
| nested.json | msgspec | 0.697 | 0.747 | 0.858 | 90.641 | 0.87x |
| nested.json | ujson | 1.055 | 1.220 | 1.337 | 90.641 | 0.53x |
| nested.json | pysimdjson | 10.653 | 11.025 | 11.588 | 90.641 | 0.06x |
| nested.json | json | 1.522 | 1.560 | 1.657 | 90.641 | 0.42x |
| wide_arrays.json | strata | 3.491 | 3.814 | 4.088 | 92.188 | 1.00x |
| wide_arrays.json | orjson | 3.641 | 4.010 | 6.798 | 92.188 | 0.95x |
| wide_arrays.json | msgspec | 4.240 | 4.588 | 6.001 | 92.188 | 0.83x |
| wide_arrays.json | ujson | 5.570 | 6.103 | 7.001 | 92.188 | 0.62x |
| wide_arrays.json | pysimdjson | 65.774 | 68.648 | 85.227 | 92.188 | 0.06x |
| wide_arrays.json | json | 7.209 | 7.837 | 8.491 | 92.188 | 0.49x |
| mixed.json | strata | 0.137 | 0.144 | 0.149 | 92.203 | 1.00x |
| mixed.json | orjson | 0.157 | 0.160 | 0.196 | 92.203 | 0.90x |
| mixed.json | msgspec | 0.172 | 0.180 | 0.294 | 92.203 | 0.80x |
| mixed.json | ujson | 0.212 | 0.224 | 0.240 | 92.203 | 0.64x |
| mixed.json | pysimdjson | 2.534 | 2.555 | 2.660 | 92.203 | 0.06x |
| mixed.json | json | 0.331 | 0.344 | 0.385 | 92.203 | 0.42x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.504 | 1.812 | 2.615 | 80.719 | 1.00x |
| users.json | orjson | 2.103 | 2.532 | 2.798 | 80.719 | 0.72x |
| users.json | msgspec | 2.690 | 3.142 | 3.441 | 80.719 | 0.58x |
| users.json | ujson | 10.793 | 11.653 | 12.525 | 80.719 | 0.16x |
| users.json | json | 15.171 | 16.840 | 19.130 | 80.719 | 0.11x |
| flat.json | strata | 0.202 | 0.217 | 0.248 | 90.641 | 1.00x |
| flat.json | orjson | 0.265 | 0.309 | 0.598 | 90.641 | 0.70x |
| flat.json | msgspec | 0.329 | 0.346 | 0.445 | 90.641 | 0.63x |
| flat.json | ujson | 0.995 | 1.016 | 1.407 | 90.641 | 0.21x |
| flat.json | json | 1.422 | 1.552 | 1.950 | 90.641 | 0.14x |
| nested.json | strata | 0.135 | 0.150 | 0.336 | 90.641 | 1.00x |
| nested.json | orjson | 0.231 | 0.248 | 0.266 | 90.641 | 0.60x |
| nested.json | msgspec | 0.302 | 0.425 | 0.579 | 90.641 | 0.35x |
| nested.json | ujson | 1.011 | 1.164 | 1.336 | 90.641 | 0.13x |
| nested.json | json | 1.708 | 1.767 | 1.901 | 90.641 | 0.09x |
| wide_arrays.json | strata | 1.114 | 1.228 | 3.365 | 92.188 | 1.00x |
| wide_arrays.json | orjson | 1.417 | 1.533 | 1.801 | 92.188 | 0.80x |
| wide_arrays.json | msgspec | 2.145 | 2.318 | 2.524 | 92.188 | 0.53x |
| wide_arrays.json | ujson | 6.954 | 7.106 | 13.950 | 92.188 | 0.17x |
| wide_arrays.json | json | 12.588 | 12.801 | 20.403 | 92.188 | 0.10x |
| mixed.json | strata | 0.047 | 0.050 | 0.059 | 92.203 | 1.00x |
| mixed.json | orjson | 0.050 | 0.053 | 0.062 | 92.203 | 0.94x |
| mixed.json | msgspec | 0.060 | 0.064 | 0.321 | 92.203 | 0.77x |
| mixed.json | ujson | 0.239 | 0.249 | 0.258 | 92.203 | 0.20x |
| mixed.json | json | 0.377 | 0.394 | 0.441 | 92.203 | 0.13x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 7.315 | 8.666 | 9.958 | 90.750 | 1.00x |
| users.json | orjson | 9.043 | 11.883 | 13.552 | 90.750 | 0.73x |
| users.json | msgspec | 8.671 | 11.836 | 29.304 | 90.750 | 0.73x |
| users.json | ujson | 11.823 | 15.741 | 19.238 | 90.750 | 0.55x |
| users.json | json | 14.196 | 18.004 | 31.314 | 90.750 | 0.48x |
| flat.json | strata | 0.740 | 0.769 | 0.872 | 90.641 | 1.00x |
| flat.json | orjson | 0.932 | 1.062 | 1.223 | 90.641 | 0.72x |
| flat.json | msgspec | 0.830 | 0.857 | 0.917 | 90.641 | 0.90x |
| flat.json | ujson | 1.226 | 1.252 | 1.418 | 90.641 | 0.61x |
| flat.json | json | 1.483 | 1.530 | 1.666 | 90.641 | 0.50x |
| nested.json | strata | 0.690 | 0.709 | 0.843 | 90.641 | 1.00x |
| nested.json | orjson | 0.966 | 0.979 | 1.049 | 90.641 | 0.72x |
| nested.json | msgspec | 0.796 | 0.839 | 0.900 | 90.641 | 0.84x |
| nested.json | ujson | 1.140 | 1.169 | 1.268 | 90.641 | 0.61x |
| nested.json | json | 1.582 | 1.651 | 1.721 | 90.641 | 0.43x |
| wide_arrays.json | strata | 3.762 | 3.895 | 4.308 | 92.188 | 1.00x |
| wide_arrays.json | orjson | 4.003 | 4.161 | 4.633 | 92.188 | 0.94x |
| wide_arrays.json | msgspec | 4.611 | 4.748 | 4.933 | 92.188 | 0.82x |
| wide_arrays.json | ujson | 5.948 | 6.363 | 6.574 | 92.188 | 0.61x |
| wide_arrays.json | json | 7.394 | 7.731 | 8.333 | 92.188 | 0.50x |
| mixed.json | strata | 0.179 | 0.203 | 0.254 | 92.203 | 1.00x |
| mixed.json | orjson | 0.237 | 0.265 | 0.497 | 92.203 | 0.77x |
| mixed.json | msgspec | 0.246 | 0.278 | 0.323 | 92.203 | 0.73x |
| mixed.json | ujson | 0.310 | 0.476 | 0.624 | 92.203 | 0.43x |
| mixed.json | json | 0.435 | 0.464 | 0.491 | 92.203 | 0.44x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.413 | 8.448 | 9.144 | 90.594 | 1.00x |
| users.ndjson | orjson | 11.084 | 12.650 | 13.899 | 90.594 | 0.67x |
| users.ndjson | msgspec | 11.679 | 12.569 | 30.791 | 90.594 | 0.67x |
| users.ndjson | ujson | 13.581 | 15.598 | 27.967 | 90.594 | 0.54x |
| users.ndjson | json | 17.438 | 20.170 | 21.094 | 90.594 | 0.42x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.902 | 2.005 | 2.887 | 87.812 | 1.00x |
| users.json | orjson | 2.553 | 2.729 | 3.202 | 87.812 | 0.73x |
| users.json | msgspec | 3.173 | 3.265 | 4.289 | 87.812 | 0.61x |
| users.json | ujson | 10.860 | 11.151 | 12.221 | 87.812 | 0.18x |
| users.json | json | 15.532 | 16.027 | 18.777 | 87.812 | 0.13x |
| flat.json | strata | 0.360 | 0.457 | 0.498 | 90.641 | 1.00x |
| flat.json | orjson | 0.417 | 0.547 | 0.737 | 90.641 | 0.83x |
| flat.json | msgspec | 0.459 | 0.589 | 0.661 | 90.641 | 0.78x |
| flat.json | ujson | 1.126 | 1.212 | 1.323 | 90.641 | 0.38x |
| flat.json | json | 1.555 | 1.782 | 2.635 | 90.641 | 0.26x |
| nested.json | strata | 0.250 | 0.313 | 0.614 | 90.641 | 1.00x |
| nested.json | orjson | 0.365 | 0.431 | 0.499 | 90.641 | 0.73x |
| nested.json | msgspec | 0.496 | 0.567 | 0.762 | 90.641 | 0.55x |
| nested.json | ujson | 1.257 | 1.369 | 1.774 | 90.641 | 0.23x |
| nested.json | json | 1.836 | 1.906 | 2.227 | 90.641 | 0.16x |
| wide_arrays.json | strata | 1.446 | 1.495 | 15.556 | 92.188 | 1.00x |
| wide_arrays.json | orjson | 1.630 | 1.857 | 2.156 | 92.188 | 0.81x |
| wide_arrays.json | msgspec | 2.490 | 2.599 | 4.226 | 92.188 | 0.58x |
| wide_arrays.json | ujson | 7.043 | 7.110 | 7.549 | 92.188 | 0.21x |
| wide_arrays.json | json | 12.274 | 12.396 | 30.533 | 92.188 | 0.12x |
| mixed.json | strata | 0.147 | 0.175 | 0.237 | 92.203 | 1.00x |
| mixed.json | orjson | 0.158 | 0.167 | 0.244 | 92.203 | 1.05x |
| mixed.json | msgspec | 0.162 | 0.252 | 0.331 | 92.203 | 0.70x |
| mixed.json | ujson | 0.341 | 0.352 | 0.421 | 92.203 | 0.50x |
| mixed.json | json | 0.460 | 0.488 | 0.647 | 92.203 | 0.36x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.047 | 0.056 | 0.113 | 87.938 | 1.00x |
| users.json $[*].id | jmespath | 0.253 | 0.325 | 0.426 | 87.938 | 0.17x |
| users.json $[*].id | jsonpath-ng | 1.388 | 1.495 | 1.979 | 87.938 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.469 | 0.595 | 0.711 | 88.156 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.018 | 2.153 | 2.396 | 88.156 | 0.28x |
| users.json $[*].orders[*].total | jsonpath-ng | 12.390 | 12.797 | 13.287 | 88.156 | 0.05x |
| users.json $..total | strata | 1.275 | 1.495 | 1.592 | 88.984 | 1.00x |
| users.json $..total | jsonpath-ng | 200.163 | 204.097 | 209.059 | 88.984 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.676 | 4.010 | 4.249 | 87.984 | 1.00x |
| users.json $[*].id | orjson+jmespath | 10.418 | 11.978 | 12.734 | 87.984 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 12.547 | 13.588 | 14.152 | 87.984 | 0.30x |
| users.json $[*].orders[*].total | strata | 3.655 | 4.034 | 4.246 | 88.984 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.636 | 12.489 | 14.613 | 88.984 | 0.32x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 21.415 | 24.052 | 35.534 | 88.984 | 0.17x |
| users.json $..total | strata | 9.262 | 10.098 | 11.160 | 88.984 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 206.714 | 217.892 | 229.567 | 88.984 | 0.05x |

