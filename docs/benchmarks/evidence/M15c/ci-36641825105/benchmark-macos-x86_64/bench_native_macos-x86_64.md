# Benchmark results - native-v1

Machine-written by `make bench-*`. Do not hand-edit.

`speedup_vs_strata` above 1.00 means that library is faster than strata.

- commit: 271a2a0864aa430c4690ca9b609e4201e13cfb51
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
| native.small | strata | 0.545 | 0.609 | 0.923 | 25.043 | 1.00x |
| native.small | orjson | 1.835 | 1.965 | 2.431 | 25.043 | 0.31x |
| native.small | msgspec | 0.686 | 0.785 | 1.098 | 25.043 | 0.77x |
| native.small | json | 10.999 | 13.076 | 16.111 | 25.043 | 0.05x |
| mixed.small (native flag) | strata (native=False) | 0.074 | 0.085 | 0.152 | 25.793 | - |
| mixed.small (native flag) | strata (native=True) | 0.072 | 0.095 | 0.174 | 25.793 | - |
| mixed.small (native flag) | orjson | 0.092 | 0.104 | 0.179 | 25.793 | - |
| mixed.small (native flag) | msgspec | 0.128 | 0.146 | 0.223 | 25.793 | - |
| mixed.small (native flag) | ujson | 0.523 | 0.641 | 1.483 | 25.793 | - |
| mixed.small (native flag) | json | 1.100 | 1.279 | 2.039 | 25.793 | - |

## dump -- dump (tree to file)

| dataset | library | min_ms | median_ms | p95_ms | rss_mb | speedup_vs_strata |
|---|---|---|---|---|---|---|
| native.small | strata | 0.904 | 1.052 | 1.332 | 25.559 | 1.00x |
| native.small | orjson | 2.267 | 2.865 | 3.446 | 25.559 | 0.37x |
| native.small | msgspec | 1.122 | 1.279 | 1.861 | 25.559 | 0.82x |
| native.small | json | 11.963 | 13.304 | 14.945 | 25.559 | 0.08x |

