# Strata Benchmark Analysis

**Date**: 2026-01-30\
**Baseline**: Initial implementation (recursive descent parser, std::map, no optimizations)

______________________________________________________________________

## Executive Summary

**Strata is already very competitive!**

- **Parsing Performance**: #2 overall, only 3.4% slower than orjson (24.8ms vs 24.0ms median)
- **Memory Efficiency**: #1 overall, 18% less memory than orjson (106 MB vs 130 MB)
- **Status**: Close to the goal! Just need targeted optimizations to beat orjson

______________________________________________________________________

## Parsing Benchmarks (users.json, 5.3 MB)

### Results Summary

| Rank | Library       | Min (ms) | Median (ms) | P95 (ms) | RSS (MB)  | Notes                        |
| ---- | ------------- | -------- | ----------- | -------- | --------- | ---------------------------- |
| 1    | **orjson**    | 15.56    | 23.99       | 25.51    | 129.7     | Rust-based, highly optimized |
| 2    | **strata**    | 22.44    | 24.81       | 26.88    | **106.1** | **Best memory!**             |
| 3    | msgspec       | 23.48    | 32.30       | 33.45    | 134.1     |                              |
| 4    | ujson         | 28.15    | 44.36       | 49.24    | 147.3     |                              |
| 5    | json (stdlib) | 34.79    | 44.47       | 48.60    | 142.6     | Pure Python                  |

### Key Insights

1. **Strata is competitive**: Only 0.82ms slower than orjson on median (3.4% gap)
1. **Memory champion**: 18% less memory than orjson, 21% less than msgspec
1. **Consistency**: P95 time is close to median (26.88ms vs 24.81ms = 8% variance)
1. **Beats established libraries**: Faster than ujson and stdlib json

### Performance Gaps

**vs orjson (target):**

- Min time: 6.88ms slower (44% gap)
- Median time: 0.82ms slower (3.4% gap)  ← **Small gap!**
- P95 time: 1.37ms slower (5.4% gap)

**Interpretation**: The median gap is very small, but min time has more variance. This suggests:

- Our parser is consistent but could have better cache warmup
- Potential for optimizations in hot paths (object lookups, number parsing)

______________________________________________________________________

## Query Benchmarks

**Status**: Baseline queries only (no Search engine in Strata yet)

### Results

| Query               | Baseline (ms) | JMESPath (ms) | Speedup Opportunity |
| ------------------- | ------------- | ------------- | ------------------- |
| 1. $.users\[\*\].id | 0.46          | 1.24          | 2.7x                |
| 2. Deep traversal   | 3.10          | 29.73         | 9.6x                |
| 3. $..price         | 30.71         | 29.99         | ~1x                 |
| 4. Filter age>30    | 0.44          | -             | -                   |
| 5. Recursive filter | 33.00         | -             | -                   |

### Key Insights

1. **Huge opportunity**: Python baseline is 2.7-9.6x faster than JMESPath for simple queries
1. **Target**: A C++-based Search implementation should beat both by 10-50x
1. **Next phase**: Port Search engine from cpp_bkp

______________________________________________________________________

## Memory Analysis

### RSS Memory Usage

```
strata:        106.1 MB  ← Best!
orjson:        129.7 MB  (+22%)
msgspec:       134.1 MB  (+26%)
json (stdlib): 142.6 MB  (+34%)
ujson:         147.3 MB  (+39%)
```

**Why is Strata more memory efficient?**

- Shared pointer design (one JsonValue tree)
- Non-owning cursors (just pointers)
- No intermediate buffers (yet!)

**Opportunity**: As we add optimizations (caching, arenas), we must monitor memory!

______________________________________________________________________

## Bottleneck Analysis

### Likely Bottlenecks (educated guesses based on implementation)

1. **Object field lookup** (std::map → O(log n))

   - **Impact**: High (frequent operation)
   - **Fix**: Switch to std::unordered_map → O(1)
   - **Expected gain**: 5-15%

1. **Number parsing** (std::stod is slow)

   - **Impact**: Medium (depends on number density)
   - **Fix**: Fast-path integer parsing
   - **Expected gain**: 2-5%

1. **String allocations**

   - **Impact**: Medium (every string allocates)
   - **Fix**: String interning, reserved capacity
   - **Expected gain**: 3-7%

1. **No SIMD** (character-by-character parsing)

   - **Impact**: High for string-heavy JSON
   - **Fix**: SIMD for whitespace, string scanning
   - **Expected gain**: 10-30%

______________________________________________________________________

## Optimization Roadmap

### Phase 1: Quick Wins (Target: Beat orjson) ⏳

**Goal**: Close the 3.4% gap to orjson

**Optimizations**:

1. Replace std::map → std::unordered_map (**5-15% gain**)
1. Fast-path integer parsing (**2-5% gain**)
1. Reserve buffer capacities (**1-3% gain**)
1. String interning for keys (**2-5% gain**)

**Total Expected**: **10-28% improvement** → Should beat orjson!

**Effort**: Low (2-4 hours of work)

### Phase 2: SIMD Optimizations (Target: Dominate)

**Goal**: Open a significant lead over orjson

**Optimizations**:

1. SIMD whitespace skipping
1. SIMD string scanning (quotes, backslashes)
1. SIMD number parsing

**Total Expected**: **20-40% additional improvement**

**Effort**: High (8-16 hours of work)

### Phase 3: Search Engine (Target: Feature completeness)

**Goal**: Enable query benchmarks and beat jmespath/jsonpath-ng

**Approach**: Port from cpp_bkp with predicate pushdown

**Expected**: 10-50x faster than Python-based libraries

**Effort**: Medium-High (12-20 hours of work)

______________________________________________________________________

## Success Criteria

### Parsing (loads)

- [x] Beat ujson ✅ (1.8x faster)
- [x] Beat msgspec ✅ (1.3x faster)
- [ ] Beat orjson ⏳ (0.97x currently, need 1.03x)

### Memory

- [x] Best memory efficiency ✅ (18% better than orjson)

### Queries (when implemented)

- [ ] Beat jmespath by >20%
- [ ] Beat jsonpath-ng by >20%

______________________________________________________________________

## Conclusion

**Strata is in excellent shape!**

The initial implementation is already competitive with best-in-class libraries:

- Close to orjson in speed (3.4% gap)
- Better than orjson in memory (18% improvement)
- Faster than all other tested libraries

**Next steps:**

1. Implement Phase 1 optimizations (should beat orjson)
1. Add Search support (enable query benchmarks)
1. Consider SIMD for maximum performance

**Confidence**: High. The gap is small and well-understood optimizations should close it.
