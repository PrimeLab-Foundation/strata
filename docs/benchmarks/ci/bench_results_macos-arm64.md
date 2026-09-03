# Benchmark results - ci-macos-arm64

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 9108337
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
| users.json | strata | 7.197 | 8.584 | 12.105 | 63.719 | 1.00x |
| users.json | orjson | 10.062 | 12.403 | 18.780 | 63.719 | 0.69x |
| users.json | msgspec | 10.189 | 12.860 | 17.335 | 63.719 | 0.67x |
| users.json | ujson | 13.197 | 16.368 | 21.813 | 63.719 | 0.52x |
| users.json | pysimdjson | 135.324 | 159.901 | 190.238 | 63.719 | 0.05x |
| users.json | json | 16.398 | 21.377 | 27.697 | 63.719 | 0.40x |
| flat.json | strata | 0.671 | 0.789 | 1.868 | 87.281 | 1.00x |
| flat.json | orjson | 0.862 | 1.191 | 1.665 | 87.281 | 0.66x |
| flat.json | msgspec | 0.780 | 0.940 | 3.332 | 87.281 | 0.84x |
| flat.json | ujson | 1.179 | 1.836 | 2.769 | 87.281 | 0.43x |
| flat.json | pysimdjson | 13.891 | 17.302 | 74.643 | 87.281 | 0.05x |
| flat.json | json | 1.434 | 1.999 | 2.618 | 87.281 | 0.39x |
| nested.json | strata | 0.581 | 0.709 | 1.013 | 87.297 | 1.00x |
| nested.json | orjson | 0.863 | 0.955 | 1.862 | 87.297 | 0.74x |
| nested.json | msgspec | 0.792 | 0.935 | 1.376 | 87.297 | 0.76x |
| nested.json | ujson | 1.283 | 1.358 | 2.425 | 87.297 | 0.52x |
| nested.json | pysimdjson | 11.750 | 13.344 | 16.367 | 87.297 | 0.05x |
| nested.json | json | 1.464 | 1.965 | 2.563 | 87.297 | 0.36x |
| wide_arrays.json | strata | 4.215 | 4.592 | 4.888 | 90.047 | 1.00x |
| wide_arrays.json | orjson | 4.371 | 4.881 | 6.063 | 90.047 | 0.94x |
| wide_arrays.json | msgspec | 5.012 | 5.511 | 6.117 | 90.047 | 0.83x |
| wide_arrays.json | ujson | 6.703 | 7.344 | 9.701 | 90.047 | 0.63x |
| wide_arrays.json | pysimdjson | 73.424 | 79.723 | 91.345 | 90.047 | 0.06x |
| wide_arrays.json | json | 7.727 | 8.650 | 10.008 | 90.047 | 0.53x |
| mixed.json | strata | 0.143 | 0.152 | 0.267 | 90.062 | 1.00x |
| mixed.json | orjson | 0.164 | 0.187 | 0.514 | 90.062 | 0.81x |
| mixed.json | msgspec | 0.181 | 0.198 | 0.244 | 90.062 | 0.77x |
| mixed.json | ujson | 0.229 | 0.240 | 0.511 | 90.062 | 0.63x |
| mixed.json | pysimdjson | 2.605 | 2.735 | 3.417 | 90.062 | 0.06x |
| mixed.json | json | 0.347 | 0.382 | 0.425 | 90.062 | 0.40x |

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.701 | 1.947 | 2.162 | 74.203 | 1.00x |
| users.json | orjson | 2.452 | 2.818 | 2.997 | 74.203 | 0.69x |
| users.json | msgspec | 3.019 | 3.439 | 3.820 | 74.203 | 0.57x |
| users.json | ujson | 11.529 | 13.323 | 14.292 | 74.203 | 0.15x |
| users.json | json | 16.852 | 19.170 | 26.946 | 74.203 | 0.10x |
| flat.json | strata | 0.235 | 0.265 | 0.313 | 87.297 | 1.00x |
| flat.json | orjson | 0.276 | 0.293 | 0.491 | 87.297 | 0.91x |
| flat.json | msgspec | 0.340 | 0.357 | 0.417 | 87.297 | 0.74x |
| flat.json | ujson | 0.962 | 1.013 | 1.253 | 87.297 | 0.26x |
| flat.json | json | 1.470 | 1.704 | 1.822 | 87.297 | 0.16x |
| nested.json | strata | 0.145 | 0.170 | 0.328 | 87.297 | 1.00x |
| nested.json | orjson | 0.242 | 0.271 | 0.694 | 87.297 | 0.63x |
| nested.json | msgspec | 0.329 | 0.530 | 0.593 | 87.297 | 0.32x |
| nested.json | ujson | 1.163 | 1.328 | 1.639 | 87.297 | 0.13x |
| nested.json | json | 1.717 | 1.826 | 2.289 | 87.297 | 0.09x |
| wide_arrays.json | strata | 1.240 | 1.313 | 1.542 | 90.047 | 1.00x |
| wide_arrays.json | orjson | 1.580 | 1.839 | 2.494 | 90.047 | 0.71x |
| wide_arrays.json | msgspec | 2.479 | 2.543 | 2.915 | 90.047 | 0.52x |
| wide_arrays.json | ujson | 7.012 | 8.119 | 8.363 | 90.047 | 0.16x |
| wide_arrays.json | json | 13.346 | 14.323 | 16.613 | 90.047 | 0.09x |
| mixed.json | strata | 0.053 | 0.061 | 0.071 | 90.062 | 1.00x |
| mixed.json | orjson | 0.059 | 0.065 | 0.207 | 90.062 | 0.94x |
| mixed.json | msgspec | 0.067 | 0.080 | 0.187 | 90.062 | 0.75x |
| mixed.json | ujson | 0.251 | 0.273 | 0.360 | 90.062 | 0.22x |
| mixed.json | json | 0.398 | 0.435 | 0.557 | 90.062 | 0.14x |

## load -- load (file to tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 8.110 | 9.684 | 13.285 | 85.469 | 1.00x |
| users.json | orjson | 11.391 | 13.612 | 17.445 | 85.469 | 0.71x |
| users.json | msgspec | 12.569 | 13.154 | 17.872 | 85.469 | 0.74x |
| users.json | ujson | 15.582 | 18.246 | 27.736 | 85.469 | 0.53x |
| users.json | json | 17.867 | 21.027 | 25.595 | 85.469 | 0.46x |
| flat.json | strata | 0.651 | 0.705 | 1.563 | 87.297 | 1.00x |
| flat.json | orjson | 0.965 | 0.999 | 1.189 | 87.297 | 0.71x |
| flat.json | msgspec | 0.782 | 0.889 | 0.985 | 87.297 | 0.79x |
| flat.json | ujson | 1.168 | 1.223 | 1.375 | 87.297 | 0.58x |
| flat.json | json | 1.424 | 1.476 | 1.591 | 87.297 | 0.48x |
| nested.json | strata | 0.708 | 0.897 | 1.123 | 87.297 | 1.00x |
| nested.json | orjson | 1.031 | 1.365 | 1.851 | 87.297 | 0.66x |
| nested.json | msgspec | 0.821 | 1.127 | 1.705 | 87.297 | 0.80x |
| nested.json | ujson | 1.183 | 1.648 | 2.045 | 87.297 | 0.54x |
| nested.json | json | 1.889 | 2.209 | 2.754 | 87.297 | 0.41x |
| wide_arrays.json | strata | 3.958 | 4.437 | 5.740 | 90.047 | 1.00x |
| wide_arrays.json | orjson | 4.043 | 4.682 | 5.125 | 90.047 | 0.95x |
| wide_arrays.json | msgspec | 4.869 | 5.523 | 6.353 | 90.047 | 0.80x |
| wide_arrays.json | ujson | 6.323 | 7.290 | 8.620 | 90.047 | 0.61x |
| wide_arrays.json | json | 7.806 | 8.705 | 9.373 | 90.047 | 0.51x |
| mixed.json | strata | 0.186 | 0.220 | 0.252 | 90.062 | 1.00x |
| mixed.json | orjson | 0.285 | 0.393 | 0.445 | 90.062 | 0.56x |
| mixed.json | msgspec | 0.258 | 0.309 | 0.356 | 90.062 | 0.71x |
| mixed.json | ujson | 0.307 | 0.389 | 0.433 | 90.062 | 0.56x |
| mixed.json | json | 0.420 | 0.468 | 0.826 | 90.062 | 0.47x |

## load (ndjson) -- load (NDJSON file to records)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.ndjson | strata | 8.028 | 9.601 | 13.315 | 87.266 | 1.00x |
| users.ndjson | orjson | 12.567 | 16.175 | 20.795 | 87.266 | 0.59x |
| users.ndjson | msgspec | 12.349 | 15.820 | 18.809 | 87.266 | 0.61x |
| users.ndjson | ujson | 15.562 | 19.299 | 30.972 | 87.266 | 0.50x |
| users.ndjson | json | 20.566 | 24.549 | 29.646 | 87.266 | 0.39x |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json | strata | 1.955 | 2.288 | 2.602 | 82.484 | 1.00x |
| users.json | orjson | 2.744 | 2.996 | 3.354 | 82.484 | 0.76x |
| users.json | msgspec | 3.360 | 3.748 | 3.813 | 82.484 | 0.61x |
| users.json | ujson | 11.527 | 12.405 | 12.890 | 82.484 | 0.18x |
| users.json | json | 16.647 | 17.475 | 17.965 | 82.484 | 0.13x |
| flat.json | strata | 0.423 | 0.485 | 0.722 | 87.297 | 1.00x |
| flat.json | orjson | 0.472 | 0.515 | 0.802 | 87.297 | 0.94x |
| flat.json | msgspec | 0.485 | 0.603 | 0.906 | 87.297 | 0.80x |
| flat.json | ujson | 1.149 | 1.250 | 1.451 | 87.297 | 0.39x |
| flat.json | json | 1.605 | 1.725 | 2.119 | 87.297 | 0.28x |
| nested.json | strata | 0.359 | 0.462 | 1.516 | 87.297 | 1.00x |
| nested.json | orjson | 0.486 | 0.642 | 1.719 | 87.297 | 0.72x |
| nested.json | msgspec | 0.627 | 0.801 | 5.809 | 87.297 | 0.58x |
| nested.json | ujson | 1.559 | 1.779 | 13.096 | 87.297 | 0.26x |
| nested.json | json | 2.213 | 2.508 | 4.092 | 87.297 | 0.18x |
| wide_arrays.json | strata | 1.381 | 1.553 | 2.726 | 90.047 | 1.00x |
| wide_arrays.json | orjson | 1.675 | 1.956 | 2.850 | 90.047 | 0.79x |
| wide_arrays.json | msgspec | 2.545 | 2.832 | 3.790 | 90.047 | 0.55x |
| wide_arrays.json | ujson | 7.348 | 7.848 | 9.834 | 90.047 | 0.20x |
| wide_arrays.json | json | 12.631 | 12.927 | 50.362 | 90.047 | 0.12x |
| mixed.json | strata | 0.228 | 0.274 | 0.359 | 90.062 | 1.00x |
| mixed.json | orjson | 0.220 | 0.287 | 5.387 | 90.062 | 0.95x |
| mixed.json | msgspec | 0.213 | 0.342 | 5.443 | 90.062 | 0.80x |
| mixed.json | ujson | 0.440 | 0.514 | 12.051 | 90.062 | 0.53x |
| mixed.json | json | 0.623 | 0.689 | 0.783 | 90.062 | 0.40x |

## query -- query (JSONPath over an in-memory tree)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 0.056 | 0.109 | 0.185 | 82.547 | 1.00x |
| users.json $[*].id | jmespath | 0.286 | 0.415 | 0.542 | 82.547 | 0.26x |
| users.json $[*].id | jsonpath-ng | 1.527 | 1.946 | 2.509 | 82.547 | 0.06x |
| users.json $[*].orders[*].total | strata | 0.672 | 0.801 | 1.010 | 82.750 | 1.00x |
| users.json $[*].orders[*].total | jmespath | 2.213 | 2.484 | 3.009 | 82.750 | 0.32x |
| users.json $[*].orders[*].total | jsonpath-ng | 14.302 | 15.079 | 20.194 | 82.750 | 0.05x |
| users.json $..total | strata | 1.371 | 1.601 | 2.692 | 83.578 | 1.00x |
| users.json $..total | jsonpath-ng | 207.336 | 225.455 | 261.141 | 83.578 | 0.01x |

## search -- search (JSONPath over a file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| users.json $[*].id | strata | 3.959 | 4.320 | 5.583 | 82.625 | 1.00x |
| users.json $[*].id | orjson+jmespath | 11.719 | 13.184 | 19.061 | 82.625 | 0.33x |
| users.json $[*].id | orjson+jsonpath-ng | 12.698 | 14.916 | 21.469 | 82.625 | 0.29x |
| users.json $[*].orders[*].total | strata | 4.059 | 4.560 | 5.437 | 83.562 | 1.00x |
| users.json $[*].orders[*].total | orjson+jmespath | 13.744 | 15.811 | 17.263 | 83.562 | 0.29x |
| users.json $[*].orders[*].total | orjson+jsonpath-ng | 29.137 | 33.577 | 46.010 | 83.562 | 0.14x |
| users.json $..total | strata | 9.400 | 11.010 | 29.283 | 83.609 | 1.00x |
| users.json $..total | orjson+jsonpath-ng | 208.767 | 260.861 | 328.174 | 83.609 | 0.04x |

