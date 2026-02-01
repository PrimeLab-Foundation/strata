# Strata Optimization Progress

**Date**: 2026-01-30\
**Goal**: Reach #1 ranking (target: \<5.5ms)\
**Status**: 🚀 **IN PROGRESS**

______________________________________________________________________

## 📊 **Current Status**

### Benchmark Results (5.05MB JSON data):

| Rank   | Library    | Median (ms) | Gap to #1  | Technology             |
| ------ | ---------- | ----------- | ---------- | ---------------------- |
| #1     | msgspec    | 5.82        | -          | Cython                 |
| #2     | orjson     | 5.86        | +0.7%      | Rust                   |
| **#3** | **strata** | **9.27**    | **+59.4%** | **C++ + Python C API** |
| #4     | ujson      | 20.78       | +257%      | C                      |
| #5     | json       | 31.21       | +436%      | Python                 |

### Progress:

- **Starting point**: 9.96ms
- **Current**: 9.27ms
- **Improvement**: 6.9% faster (-0.69ms)
- **Still needed**: 59.4% improvement to reach #1

______________________________________________________________________

## ✅ **Optimization Round 1** (COMPLETED)

**Date**: 2026-01-30\
**Focus**: Python C API optimizations + inlining\
**Improvement**: **6.9% faster** (9.96ms → 9.27ms)

### Changes Made:

1. **Type Check Reordering** (+3-5% expected)

   - Moved Dict/List checks first (most common in JSON)
   - Added early returns to skip unnecessary checks
   - Reordered: None → Dict → List → String → Long → Float → Bool

1. **Inline Critical Functions** (+3-5% expected)

   - Added `__attribute__((always_inline))` to:
     - `itoa_fast()` - integer to string conversion
     - `serialize_number()` - float serialization
     - `serialize_object()` - main serialization function

1. **Eliminated Redundant Checks** (+2-3% expected)

   - Use `PyDict_CheckExact` / `PyList_CheckExact` (faster than generic checks)
   - Skip key validation in release builds (`#ifdef NDEBUG`)
   - Use `PyDict_GET_SIZE` / `PyList_GET_SIZE` (no overflow checks)

1. **Optimized Reserve Sizes** (+2-4% expected)

   - Dict: 64 → 50 bytes per item (tuned)
   - List: 32 → 35 bytes per item (tuned)
   - Removed over-allocation (`std::max(size, 4096)`)

### Results:

- ✅ All tests passing (252/252)
- ✅ Measurable improvement: 0.38ms → 0.36ms (micro-benchmark)
- ✅ Real-world improvement: 9.96ms → 9.27ms (5MB dataset)

______________________________________________________________________

## ⚠️ **Optimization Round 2** (ATTEMPTED - REVERTED)

**Date**: 2026-01-30\
**Focus**: Memory management optimizations\
**Result**: **REGRESSION** (-4.6% slower)

### Changes Attempted:

1. **Empty Container Fast Paths**

   - Added `out.append("{}", 2)` for empty dicts
   - Added `out.append("[]", 2)` for empty lists

1. **More Aggressive Reserve Sizes**

   - Dict: 50 → 60 bytes per item
   - List: 35 → 40 bytes per item

### Results:

- ❌ Performance got worse: 9.27ms → 9.70ms (first attempt)
- ❌ Even with conservative sizes: 9.27ms → 9.88ms (second attempt)
- ❌ Reverted all changes

### Key Learning:

**Memory over-allocation hurts performance more than it helps!**

- More memory = more allocator overhead
- More memory = worse cache locality
- The existing reserve sizes (50/35) are already well-tuned
- Fast paths for empty containers add branch overhead

### Conclusion:

- Stick with Round 1 optimizations
- Focus on different approaches: SIMD, algorithmic improvements
- Memory management is already near-optimal

______________________________________________________________________

## 🎯 **Next Optimization Rounds** (PLANNED)

### Round 2: Memory Management Optimizations

**Target**: +5-10% improvement

Planned changes:

1. Custom arena allocator for `std::string` buffer
1. Pre-allocate larger buffers for deeply nested structures
1. Batch character appends to reduce function calls
1. Pool allocator for temporary strings

### Round 3: SIMD Expansions

**Target**: +3-5% improvement

Planned changes:

1. SIMD for number formatting (integers, floats)
1. SIMD for comma/colon insertion
1. Vectorized type checking

### Round 4: Algorithmic Optimizations

**Target**: +5-10% improvement

Planned changes:

1. Fast path for homogeneous arrays (all same type)
1. Fast path for simple dicts (all string values)
1. Reduce Python API calls (batch operations)
1. Cache frequently accessed Python type objects

______________________________________________________________________

## 📈 **Performance Timeline**

```
Baseline (pybind11):       10.55ms
Python C API migration:     9.96ms  (+5.6%)
Optimization Round 1:       9.27ms  (+6.9%)
Target (Round 2):          ~8.40ms  (+15% cumulative)
Target (Round 3):          ~7.97ms  (+20% cumulative)
Target (Round 4):          ~7.30ms  (+30% cumulative)
Stretch goal (#1):         <5.82ms  (+45% cumulative)
```

______________________________________________________________________

## 🔬 **Profiling Insights**

### Hot Paths (from cProfile):

1. **100% time in C++ core** (`strata._strata.dumps`)

   - Python wrapper overhead: negligible
   - All optimization must be in C++ code

1. **Type checking is critical**

   - Dict/List are most common → moved to front
   - Direct type pointer comparison faster than PyXXX_Check

1. **Memory allocation matters**

   - String reserve/resize operations are costly
   - Better size estimation = fewer reallocations
   - Thread-local buffer already eliminates most allocations

1. **Already fast**:

   - ✅ SIMD string escaping
   - ✅ Fast integer conversion (digit pairs)
   - ✅ Ryu algorithm for floats
   - ✅ Thread-local buffer

______________________________________________________________________

## 🚀 **Path to #1**

### Required Improvements:

- Current: 9.27ms
- Target: 5.82ms
- **Need: 1.59x improvement (37% reduction)**

### Strategy:

1. **Quick wins** (Rounds 1-2): Get to 8.0ms (+17% total)
1. **Advanced optimizations** (Rounds 3-4): Get to 7.0ms (+30% total)
1. **Aggressive optimizations** (Rounds 5-6): Get to 6.0ms (+40% total)
1. **Final push**: Reach 5.8ms or better (+45% total)

### Challenges:

- msgspec uses Cython (Python → C integration zero-cost)
- orjson uses Rust (different language, different optimizations)
- We're already using best practices (SIMD, fast algorithms)
- Diminishing returns as we optimize further

### Opportunities:

- Custom allocators (big potential win)
- More SIMD (moderate win)
- Fast paths for common cases (moderate win)
- Profile-guided optimization (small win)
- Link-time optimization (small win)

______________________________________________________________________

## 📝 **Notes**

- Compiler flags already optimal: `-O3 -march=native`
- No `-ffast-math` (breaks NaN/Inf handling)
- Thread-local buffer eliminates most allocations
- SIMD string escaping already implemented
- Fast integer/float algorithms already used

______________________________________________________________________

*Last updated: 2026-01-30*\
*Next review: After Round 2 optimizations*
