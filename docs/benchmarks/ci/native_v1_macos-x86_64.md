# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 565fab210bb851772fcefe65082041084aeafc14
- python: 3.12.10
- implementation: CPython
- platform: macOS-15.7.9-x86_64-i386-64bit
- machine: x86_64
- processor: Intel(R) Core(TM) i7-8700B CPU @ 3.20GHz
- compiler_flags: see build provenance
- provenance_schema: 1

Excluded libraries (not installed, or no native equivalent):
- ujson (native dataset): no native type support; not named

## dumps -- dumps (in-memory serialization)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 1.142 | 1.581 | 2.109 | 24.984 | 1.00x |
| native.small | orjson | 1.834 | 2.646 | 3.912 | 24.984 | 0.60x |
| native.small | msgspec | 0.743 | 1.066 | 2.218 | 24.984 | 1.48x |
| native.small | json | 11.704 | 16.539 | 27.033 | 24.984 | 0.10x |
| mixed.small (native flag) | strata (native=False) | 0.077 | 0.093 | 0.150 | 25.602 | - |
| mixed.small (native flag) | strata (native=True) | 0.077 | 0.093 | 0.144 | 25.602 | - |
| mixed.small (native flag) | orjson | 0.088 | 0.112 | 0.228 | 25.602 | - |
| mixed.small (native flag) | msgspec | 0.128 | 0.149 | 0.235 | 25.602 | - |
| mixed.small (native flag) | ujson | 0.541 | 0.667 | 0.839 | 25.602 | - |
| mixed.small (native flag) | json | 1.153 | 1.307 | 1.707 | 25.602 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 1.527 | 1.819 | 2.366 | 25.398 | 1.00x |
| native.small | orjson | 2.269 | 2.797 | 3.518 | 25.398 | 0.65x |
| native.small | msgspec | 1.120 | 1.399 | 1.917 | 25.398 | 1.30x |
| native.small | json | 13.640 | 15.064 | 19.248 | 25.398 | 0.12x |

