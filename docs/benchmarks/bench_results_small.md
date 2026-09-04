# Benchmark results - small

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
| users.json | strata | 6.525 | 6.738 | 7.192 | 52.906 | 1.00x |
| users.json | orjson | 8.256 | 8.471 | 8.639 | 52.906 | 0.80x |
| users.json | msgspec | 8.489 | 8.664 | 8.895 | 52.906 | 0.78x |
| users.json | ujson | 11.161 | 11.477 | 12.537 | 52.906 | 0.59x |
| users.json | json | 15.696 | 15.993 | 16.479 | 52.906 | 0.42x |
| flat.json | strata | 0.598 | 0.616 | 0.658 | 73.688 | 1.00x |
| flat.json | orjson | 0.640 | 0.652 | 0.741 | 73.688 | 0.95x |
| flat.json | msgspec | 0.692 | 0.723 | 0.805 | 73.688 | 0.85x |
| flat.json | ujson | 1.042 | 1.095 | 1.221 | 73.688 | 0.56x |
| flat.json | json | 1.444 | 1.526 | 1.713 | 73.688 | 0.40x |
| nested.json | strata | 0.544 | 0.567 | 0.633 | 73.703 | 1.00x |
| nested.json | orjson | 0.610 | 0.627 | 0.764 | 73.703 | 0.90x |
| nested.json | msgspec | 0.616 | 0.636 | 0.663 | 73.703 | 0.89x |
| nested.json | ujson | 0.890 | 0.948 | 1.032 | 73.703 | 0.60x |
| nested.json | json | 1.408 | 1.486 | 1.720 | 73.703 | 0.38x |
| wide_arrays.json | strata | 3.004 | 3.269 | 3.552 | 73.750 | 1.00x |
| wide_arrays.json | orjson | 3.212 | 3.339 | 3.817 | 73.750 | 0.98x |
| wide_arrays.json | msgspec | 3.950 | 4.425 | 4.619 | 73.750 | 0.74x |
| wide_arrays.json | ujson | 5.298 | 5.667 | 5.812 | 73.750 | 0.58x |
| wide_arrays.json | json | 7.654 | 8.064 | 8.372 | 73.750 | 0.41x |
| mixed.json | strata | 0.134 | 0.142 | 0.161 | 73.750 | 1.00x |
| mixed.json | orjson | 0.131 | 0.135 | 0.148 | 73.750 | 1.05x |
| mixed.json | msgspec | 0.147 | 0.152 | 0.167 | 73.750 | 0.93x |
| mixed.json | ujson | 0.185 | 0.198 | 0.245 | 73.750 | 0.72x |
| mixed.json | json | 0.311 | 0.324 | 0.357 | 73.750 | 0.44x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.437 | 1.498 | 1.556 | 57.547 | 1.00x |
| users.json | orjson | 2.006 | 2.083 | 2.160 | 57.547 | 0.72x |
| users.json | msgspec | 2.693 | 2.762 | 2.844 | 57.547 | 0.54x |
| users.json | ujson | 11.083 | 11.310 | 11.596 | 57.547 | 0.13x |
| users.json | json | 15.827 | 16.417 | 16.981 | 57.547 | 0.09x |
| flat.json | strata | 0.178 | 0.188 | 0.231 | 73.688 | 1.00x |
| flat.json | orjson | 0.222 | 0.234 | 0.258 | 73.688 | 0.81x |
| flat.json | msgspec | 0.291 | 0.311 | 0.354 | 73.688 | 0.61x |
| flat.json | ujson | 0.880 | 0.893 | 0.992 | 73.688 | 0.21x |
| flat.json | json | 1.484 | 1.553 | 1.610 | 73.688 | 0.12x |
| nested.json | strata | 0.136 | 0.143 | 0.188 | 73.750 | 1.00x |
| nested.json | orjson | 0.202 | 0.205 | 0.239 | 73.750 | 0.70x |
| nested.json | msgspec | 0.271 | 0.274 | 0.290 | 73.750 | 0.52x |
| nested.json | ujson | 0.976 | 1.016 | 1.081 | 73.750 | 0.14x |
| nested.json | json | 1.626 | 1.778 | 2.085 | 73.750 | 0.08x |
| wide_arrays.json | strata | 0.920 | 0.937 | 1.066 | 73.750 | 1.00x |
| wide_arrays.json | orjson | 1.112 | 1.131 | 1.346 | 73.750 | 0.83x |
| wide_arrays.json | msgspec | 1.928 | 1.968 | 2.246 | 73.750 | 0.48x |
| wide_arrays.json | ujson | 6.090 | 6.229 | 6.583 | 73.750 | 0.15x |
| wide_arrays.json | json | 11.432 | 11.689 | 12.181 | 73.750 | 0.08x |
| mixed.json | strata | 0.036 | 0.040 | 0.044 | 73.797 | 1.00x |
| mixed.json | orjson | 0.043 | 0.049 | 0.060 | 73.797 | 0.82x |
| mixed.json | msgspec | 0.048 | 0.059 | 0.095 | 73.797 | 0.68x |
| mixed.json | ujson | 0.220 | 0.225 | 0.260 | 73.797 | 0.18x |
| mixed.json | json | 0.375 | 0.410 | 0.559 | 73.797 | 0.10x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 6.794 | 6.979 | 7.249 | 71.172 | 1.00x |
| users.json | orjson | 8.554 | 8.826 | 8.912 | 71.172 | 0.79x |
| users.json | msgspec | 8.514 | 8.681 | 8.992 | 71.172 | 0.80x |
| users.json | ujson | 11.636 | 12.067 | 12.740 | 71.172 | 0.58x |
| users.json | json | 16.130 | 16.207 | 17.045 | 71.172 | 0.43x |
| flat.json | strata | 0.680 | 0.729 | 0.795 | 73.688 | 1.00x |
| flat.json | orjson | 0.737 | 0.771 | 0.911 | 73.688 | 0.94x |
| flat.json | msgspec | 0.772 | 0.833 | 0.884 | 73.688 | 0.88x |
| flat.json | ujson | 1.169 | 1.228 | 1.357 | 73.688 | 0.59x |
| flat.json | json | 1.532 | 1.639 | 1.721 | 73.688 | 0.44x |
| nested.json | strata | 0.619 | 0.649 | 0.720 | 73.750 | 1.00x |
| nested.json | orjson | 0.701 | 0.743 | 0.831 | 73.750 | 0.87x |
| nested.json | msgspec | 0.712 | 0.782 | 0.964 | 73.750 | 0.83x |
| nested.json | ujson | 1.019 | 1.047 | 1.195 | 73.750 | 0.62x |
| nested.json | json | 1.489 | 1.544 | 1.806 | 73.750 | 0.42x |
| wide_arrays.json | strata | 3.203 | 3.387 | 3.483 | 73.750 | 1.00x |
| wide_arrays.json | orjson | 3.404 | 3.565 | 3.864 | 73.750 | 0.95x |
| wide_arrays.json | msgspec | 4.300 | 4.541 | 5.092 | 73.750 | 0.75x |
| wide_arrays.json | ujson | 5.712 | 5.923 | 6.133 | 73.750 | 0.57x |
| wide_arrays.json | json | 8.041 | 8.209 | 8.397 | 73.750 | 0.41x |
| mixed.json | strata | 0.174 | 0.186 | 0.217 | 73.797 | 1.00x |
| mixed.json | orjson | 0.181 | 0.188 | 0.212 | 73.797 | 0.99x |
| mixed.json | msgspec | 0.192 | 0.206 | 0.218 | 73.797 | 0.90x |
| mixed.json | ujson | 0.253 | 0.298 | 0.364 | 73.797 | 0.62x |
| mixed.json | json | 0.374 | 0.400 | 0.435 | 73.797 | 0.46x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 7.180 | 7.330 | 7.462 | 72.828 | 1.00x |
| users.ndjson | orjson | 10.319 | 10.958 | 11.730 | 72.828 | 0.67x |
| users.ndjson | msgspec | 10.213 | 10.955 | 11.516 | 72.828 | 0.67x |
| users.ndjson | ujson | 13.180 | 13.933 | 14.122 | 72.828 | 0.53x |
| users.ndjson | json | 18.959 | 19.485 | 19.616 | 72.828 | 0.38x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.823 | 1.914 | 2.020 | 71.141 | 1.00x |
| users.json | orjson | 2.456 | 2.567 | 2.738 | 71.141 | 0.75x |
| users.json | msgspec | 3.083 | 3.184 | 3.344 | 71.141 | 0.60x |
| users.json | ujson | 11.230 | 11.536 | 11.822 | 71.141 | 0.17x |
| users.json | json | 16.619 | 16.768 | 17.090 | 71.141 | 0.11x |
| flat.json | strata | 0.347 | 0.390 | 0.469 | 73.688 | 1.00x |
| flat.json | orjson | 0.407 | 0.438 | 0.550 | 73.688 | 0.89x |
| flat.json | msgspec | 0.465 | 0.524 | 0.587 | 73.688 | 0.74x |
| flat.json | ujson | 1.086 | 1.134 | 1.202 | 73.688 | 0.34x |
| flat.json | json | 1.637 | 1.719 | 1.820 | 73.688 | 0.23x |
| nested.json | strata | 0.289 | 0.336 | 0.399 | 73.750 | 1.00x |
| nested.json | orjson | 0.357 | 0.418 | 0.480 | 73.750 | 0.81x |
| nested.json | msgspec | 0.435 | 0.487 | 0.533 | 73.750 | 0.69x |
| nested.json | ujson | 1.146 | 1.197 | 1.309 | 73.750 | 0.28x |
| nested.json | json | 1.844 | 2.004 | 2.264 | 73.750 | 0.17x |
| wide_arrays.json | strata | 1.239 | 1.294 | 1.566 | 73.750 | 1.00x |
| wide_arrays.json | orjson | 1.461 | 1.525 | 1.882 | 73.750 | 0.85x |
| wide_arrays.json | msgspec | 2.265 | 2.407 | 2.591 | 73.750 | 0.54x |
| wide_arrays.json | ujson | 6.399 | 6.596 | 6.781 | 73.750 | 0.20x |
| wide_arrays.json | json | 11.665 | 12.138 | 12.370 | 73.750 | 0.11x |
| mixed.json | strata | 0.156 | 0.206 | 0.276 | 73.828 | 1.00x |
| mixed.json | orjson | 0.181 | 0.227 | 0.375 | 73.828 | 0.90x |
| mixed.json | msgspec | 0.189 | 0.226 | 0.363 | 73.828 | 0.91x |
| mixed.json | ujson | 0.358 | 0.405 | 1.050 | 73.828 | 0.51x |
| mixed.json | json | 0.504 | 0.637 | 0.695 | 73.828 | 0.32x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.057 | 0.072 | 0.087 | 71.516 | 1.00x |
| users.json $[*].id | jmespath | 0.307 | 0.329 | 0.378 | 71.516 | 0.22x |
| users.json $[*].id | jsonpath-ng | 1.431 | 1.636 | 1.759 | 71.516 | 0.04x |
| users.json $[*].orders[*].total | strata | 0.350 | 0.417 | 0.490 | 71.891 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 1.860 | 1.927 | 2.121 | 71.891 | 0.22x |
| users.json $[*].orders[*].total | jsonpath-ng | 10.041 | 10.806 | 11.405 | 71.891 | 0.04x |
| users.json $..total | strata | 1.386 | 1.457 | 1.588 | 73.016 | 1.00x |
| users.json $..total | jsonpath-ng | 190.092 | 191.237 | 196.182 | 73.016 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.459 | 3.535 | 3.656 | 71.578 | 1.00x |
| users.json $[*].id | orjson+jmespath | 8.811 | 9.074 | 9.414 | 71.578 | 0.39x |
| users.json $[*].id | orjson+jsonpath-ng | 10.045 | 10.192 | 10.582 | 71.578 | 0.35x |
| users.json $[*].orders[*].total | strata | 3.530 | 3.661 | 3.725 | 72.984 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 10.346 | 10.704 | 11.129 | 72.984 | 0.34x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 20.064 | 20.940 | 21.723 | 72.984 | 0.17x |
| users.json $..total | strata | 8.443 | 8.898 | 9.074 | 73.031 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 201.510 | 203.498 | 204.676 | 73.031 | 0.04x |

