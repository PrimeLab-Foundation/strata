### Performance Summary

This document summarizes the benchmark improvements achieved in the 2026-02-02 optimization session.

#### Baseline vs Post-Optimization (Small Dataset ~1MB)

| Case                         | Baseline (Median) | Post (Median) | Delta  | Status      |
| :--------------------------- | :---------------- | :------------ | :----- | :---------- |
| Parse users.json (loads)     | 10.56 ms          | 8.84 ms       | -16.3% | ✅ Improved |
| Parse users.ndjson           | 13.30 ms          | 13.02 ms      | -2.1%  | ✅ Improved |
| Serialize users.json (dumps) | 8.41 ms           | 8.14 ms       | -3.2%  | ✅ Improved |
| JSONPath ($.users\[\*\].id)  | 0.01 ms           | 0.01 ms       | 0%     | ✅ Stable   |

*Note: Baseline measured at start of session. Post measured after all optimizations.*

#### Key Contributors

1. **Python Key Caching (loads)**: Added an interned key cache in `json_value_to_python` to avoid creating thousands of identical Python string objects for JSON keys. This was the largest contributor to parsing speedups.
1. **FlatMap for Objects**: Replaced `std::map` with a vector-based `FlatMap` in the C++ core. This reduced heap allocations for small objects (typical in our datasets) and improved cache locality.
1. **Stack-based Cycle Detection (dumps)**: Replaced `PyDict`/`PyLong` based cycle detection with a zero-allocation stack scan.
1. **SIMD String Scanning**: Integrated SIMD (NEON/AVX2) string scanning into the core JSON parser.
1. **Dragonbox Integration**: Switched to Dragonbox for float serialization in the C++ core.
1. **Numerical Parsing Optimization**: Fixed a performance bottleneck in `parse_double_fast` by removing an $O(N)$ division loop.

#### Memory Impact

- **RSS (loads)**: Decreased from ~55MB to ~50MB due to Python string interning for keys.
- **RSS (dumps)**: Decreased from ~38MB to ~34.4MB.

#### Next Steps

- **Blocked by pybind11**: Direct-to-Python parsing would likely yield another 2-3x speedup by avoiding the intermediate C++ `JsonValue` tree entirely. This requires significant binding changes (blocked).
- **SIMD Whitespace Skipping**: Further gains possible by adding SIMD skip-whitespace to the core parser.
- **Buffer Reuse**: Reusing `JsonValue` nodes across multiple `loads` calls in NDJSON.
