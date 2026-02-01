# Phase 2: Optimization to #1 Ranking

**Date**: 2026-01-30\
**Status**: 🚀 **IN PROGRESS**

______________________________________________________________________

## 🎯 **Mission**

Optimize Strata from #3 (9.96ms) to #1 (5.48ms) - need **1.82x improvement**

______________________________________________________________________

## 📊 **Current Baseline**

```
Library       Median (ms)  Gap to #1  Technology
-------------------------------------------------
msgspec           5.48      1.00x     Cython
orjson            6.31      1.15x     Rust
strata            9.96      1.82x     C++ + Python C API
ujson            20.26      3.70x     C
json             31.48      5.75x     Python
```

**Our Goal**: Close the 1.82x gap

______________________________________________________________________

## 🔬 **Phase 2A: Profile & Low-Hanging Fruit** (Target: +15-20%)

### Step 1: Profiling (30 min)

```bash
# Profile dumps hot paths
python3 -m cProfile -o dumps.prof benchmarks/bench_dumps.py
python3 -m pstats dumps.prof

# C++ profiling with perf
perf record -g python3 benchmarks/bench_dumps.py
perf report
```

**Expected findings:**

- Python object iteration overhead
- String allocation patterns
- Type checking costs
- Python C API call overhead

### Step 2: Python C API Optimizations (2-3 hours)

1. **Optimize dict iteration** (Target: +5-10%)

   - Use `PyDict_Next` more efficiently
   - Cache dict size
   - Reduce bounds checking

1. **Optimize list iteration** (Target: +5-10%)

   - Use `PyList_GET_ITEM` (unchecked) where safe
   - Batch item access
   - Reduce reference counting overhead

1. **Optimize type checking** (Target: +3-5%)

   - Reorder checks by frequency
   - Use fast type checks (`PyLong_CheckExact`)
   - Cache type objects

### Step 3: Memory Management (2-3 hours)

1. **Better string pre-allocation** (Target: +8-12%)

   - Profile actual size patterns
   - Type-specific heuristics:
     - Dict: `50 + 20*len + sum(key_len + val_estimate)`
     - List: `10 + 15*len`
     - String: `len + 10` (escapes)
   - Dynamic adjustment based on input

1. **Reduce string reallocations** (Target: +5-8%)

   - Reserve larger chunks
   - Batch append operations
   - Reuse buffers where possible

### Step 4: Code Quality (1-2 hours)

1. **Inline critical functions**

   - Mark with `inline` or `__attribute__((always_inline))`
   - Especially type checks and simple conversions

1. **Compiler optimizations**

   - Add `-O3` if not already
   - Consider `-march=native` for SIMD
   - Profile-guided optimization (PGO)

**Phase 2A Target**: 9.96ms → 8.0ms (~20% improvement)

______________________________________________________________________

## ⚡ **Phase 2B: Advanced Optimizations** (Target: +20-25%)

### 1. Custom Allocator (Target: +15-20%)

- Pool allocator for small strings
- Arena allocator for result buffer
- Reduce malloc/free overhead

### 2. SIMD Number Formatting (Target: +5-10%)

- SIMD digit conversion
- Parallel byte copying
- Optimize Ryū float formatter

### 3. Specialized Fast Paths (Target: +5-10%)

- Fast path for small dicts/lists
- Fast path for ASCII strings
- Fast path for integer-only lists

**Phase 2B Target**: 8.0ms → 6.0ms (~25% improvement)

______________________________________________________________________

## 🔮 **Phase 2C: Architectural** (Target: +15-20%)

### 1. Consider Cython Wrapper

- msgspec uses Cython for Python interface
- Would reduce Python C API overhead
- Investigate if worth the complexity

### 2. Custom Output Buffer

- Replace `std::string` with custom buffer
- Stack allocation for small outputs (\<4KB)
- Chunked growth strategy

### 3. Streaming Output

- Write directly to Python bytes object
- Avoid intermediate string copy
- Zero-copy where possible

**Phase 2C Target**: 6.0ms → 5.5ms (~10% improvement)

______________________________________________________________________

## 📈 **Expected Progress**

```
Current:   9.96ms (Rank #3)
Phase 2A:  8.00ms (Rank #3, closer to #2)
Phase 2B:  6.00ms (Rank #2)
Phase 2C:  5.50ms (Rank #1!) 🥇
```

______________________________________________________________________

## 🎯 **Success Criteria**

1. ✅ Reach #1 in dumps benchmark
1. ✅ All tests still passing
1. ✅ No memory leaks
1. ✅ Maintain code quality
1. ✅ Document all optimizations

______________________________________________________________________

## 🚀 **Let's Begin!**

**Next Action**: Start Phase 2A - Profiling & Low-Hanging Fruit

______________________________________________________________________

*Plan created: 2026-01-30*\
*Estimated timeline: 2-4 days of focused work*
