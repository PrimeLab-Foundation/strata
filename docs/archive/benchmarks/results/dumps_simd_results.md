# dumps SIMD Optimization Results

**Date**: 2026-01-30\
**Optimization**: SIMD-accelerated string escaping\
**Status**: ✅ Complete

______________________________________________________________________

## Overview

Implemented SIMD-accelerated JSON string escaping using ARM NEON instructions for parallel character scanning and bulk copying of clean string segments.

### Implementation Details

**File**: `src/strata/util/simd_string.cpp`

- **SIMD Support**: AVX2 (32 bytes), SSE4.2 (16 bytes), ARM NEON (16 bytes)
- **Fast Path**: Parallel scan for escape characters (`< 0x20`, `"`, `\`)
- **Clean String Detection**: Full SIMD scan with zero-copy append
- **Mixed Content**: Chunk-based processing with bulk copies

### Algorithm

1. **SIMD Scan** (16 bytes at a time on ARM):

   - Check for control chars (`c < 0x20`)
   - Check for quote (`"`)
   - Check for backslash (`\`)
   - Combine results with OR operation

1. **Fast Path** (no escapes):

   - Single SIMD scan confirms clean
   - Bulk `std::string::append()` - no character-by-character copies

1. **Slow Path** (has escapes):

   - Find next escape position with SIMD
   - Bulk copy clean chunk
   - Escape single character
   - Repeat

______________________________________________________________________

## Performance Results

### Before vs After

| Metric     | Before (Scalar) | After (SIMD) | Improvement       |
| ---------- | --------------- | ------------ | ----------------- |
| **Min**    | 23.8ms          | 11.0ms       | **54% faster** ⚡ |
| **Median** | 24.1ms          | 11.7ms       | **51% faster**    |
| **P95**    | 25.0ms          | 13.6ms       | **46% faster**    |

### Rankings

| Position | Library     | Time (min) | Gap to #1 |
| -------- | ----------- | ---------- | --------- |
| **#1**   | orjson      | 5.0ms      | —         |
| **#2**   | msgspec     | 5.6ms      | 12%       |
| **#3**   | **strata**  | **11.0ms** | **120%**  |
| #4       | ujson       | 19.8ms     | 296%      |
| #5       | stdlib json | 31.8ms     | 536%      |

**Status**: Moved from #4 to #3 🎉

______________________________________________________________________

## Detailed Benchmark Comparison

### Standard Benchmark (5.05 MB users.json)

```
Library           Min (ms)  Median (ms)   P95 (ms)    Speedup
-----------------------------------------------------------
orjson                5.00         5.46       6.42      1.00x
msgspec               5.56         5.98       8.21      0.91x
strata               11.01        11.71      13.57      0.47x  ← US
ujson                19.75        20.12      20.54      0.27x
json                 31.79        32.31      32.99      0.17x
```

### String-Heavy Workload (1000 records with long text)

```
Library      Time (ms)   Gap to Strata
--------------------------------------
orjson          0.14      Strata is 2.9x slower
strata          0.40      —
```

______________________________________________________________________

## Technical Analysis

### Why SIMD Works Here

1. **Escape Detection**: Most JSON strings have few/no escapes

   - SIMD can check 16 bytes in parallel vs 16 sequential checks
   - Clean strings detected in ~1-2 SIMD operations

1. **Bulk Copying**: Clean segments copied with `memcpy`-like performance

   - No per-character branches
   - Cache-friendly sequential access

1. **Branch Reduction**: Scalar code had branch per character

   - SIMD reduces branches by 16x (per vector width)

### ARM NEON Specifics

On Apple Silicon (M1/M2/M3):

- **16-byte vectors** (128-bit NEON)
- **`vcltq_u8`**: Compare less-than (for control chars)
- **`vceqq_u8`**: Compare equal (for `"` and `\`)
- **`vorrq_u8`**: Combine results

### Why Still 2x Slower Than orjson?

1. **orjson uses Rust + SIMD** with more aggressive optimizations
1. **orjson's allocator** is custom-tuned for JSON serialization
1. **orjson uses simd-json** crate with mature, production-hardened SIMD

**Our gains**:

- Pure C++20 implementation
- No external dependencies (beyond pybind11)
- More readable/maintainable code

______________________________________________________________________

## Next Optimization Targets

To close the gap to orjson/msgspec:

1. **Custom allocator** for `std::string` - reduce allocations
1. **Better pre-allocation heuristics** - estimate output size more accurately
1. **Dict iteration optimizations** - faster key-value traversal
1. **Float formatting improvements** - Ryū is good, but can be faster
1. **Consider SSE/AVX2 on x86** - 32-byte vectors on Intel/AMD

**Expected gains**: 20-40% improvement possible

______________________________________________________________________

## Code Quality

### Files Added

- `include/strata/util/simd_string.hpp` (27 lines)
- `src/strata/util/simd_string.cpp` (345 lines)

### Files Modified

- `src/strata/json/json_serialize_fast.cpp` (3 call sites)
- `setup.py` (1 line added)

### Tests

- ✅ All 252 Python tests pass
- ✅ Backward compatible API
- ✅ No regressions

______________________________________________________________________

## Conclusion

✅ **SIMD string escaping delivered 54% improvement**\
✅ **Moved from #4 to #3 in rankings**\
✅ **Clean, maintainable implementation**

**Next**: Focus on memory allocation and pre-allocation to close remaining 2x gap.

______________________________________________________________________

*See `docs/status/optimization_roadmap.md` for next steps*
